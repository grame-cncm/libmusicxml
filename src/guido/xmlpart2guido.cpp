/*
 MusicXML Library
 Copyright (C) Grame 2006-2013
 
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 
 Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 research@grame.fr
 */

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>


#include "conversions.h"
#include "partsummary.h"
#include "rational.h"
#include "xml2guidovisitor.h"
#include "xmlpart2guido.h"
#include "xml_tree_browser.h"

using namespace std;

namespace MusicXML2
{
    
    //______________________________________________________________________________
    xmlpart2guido::xmlpart2guido(bool generateComments, bool generateStem, bool generateBar) :
    fGenerateComments(generateComments), //fGenerateStem(generateStem),
    fGenerateBars(generateBar),
    fNotesOnly(false), fCurrentStaffIndex(0), fCurrentStaff(0),
    fTargetStaff(0), fTargetVoice(0)
    {
        fGeneratePositions = true;
        fGenerateAutoMeasureNum = true;
        xmlpart2guido::reset();
        fHasLyrics = false;
        directionWord = false;
        fGenerateTempo = false;
        fNonStandardNoteHead = false;
        fLyricsManualSpacing = false;
        fDynamics= (void*)0;
        fIgnoreWedgeWithOffset = false;
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::reset ()
    {
        guidonotestatus::resetall();
        fCurrentBeamNumber = 0;
        fCurrentTupletNumber = 0;
        fMeasNum = 0;
        fInCue = fInGrace = fInhibitNextBar = fPendingBar = fDoubleBar
        = fBeamOpened = fCrescPending = fSkipDirection = fTupletOpened = fWavyTrillOpened = fSingleScopeTrill = fNonStandardNoteHead = false;
        fCurrentStemDirection = kStemUndefined;
        fCurrentDivision = 1;
        fCurrentOffset = 0;
        fPendingPops = 0;
        fMeasNum = 0;
        fLyricsManualSpacing = false;
        directionWord = false;
        fGenerateTempo = false;
        fTextTagOpen = 0;
        fDynamics= (void*)0;
        fIgnoreWedgeWithOffset = false;
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::initialize (Sguidoelement seq, int staff, int guidostaff, int voice,
                                    bool notesonly, rational defaultTimeSign)
    {
        fCurrentStaff = fTargetStaff = staff;	// the current and target staff
        fTargetVoice = voice;					// the target voice
        fNotesOnly = notesonly;					// prevent multiple output for keys, clefs etc...
        fCurrentTimeSign = defaultTimeSign;		// a default time signature
        fCurrentStaffIndex = guidostaff;		// the current guido staff index
        fHasLyrics = false;
        fLyricsManualSpacing = false;
        fDynamics= (void*)0;
        fIgnoreWedgeWithOffset = false;
        start (seq);
    }
    
    //________________________________________________________________________
    // some code for the delayed elements management
    // delayed elements are elements enclosed in a <direction> element that
    // contains a non-null <offset> element. This offset postpone the graphic
    // appearance of the element in 'offset' time units in the futur.
    // Time units are <division> time units
    //________________________________________________________________________
    // add an element to the list of delayed elements
    void xmlpart2guido::addDelayed (Sguidoelement elt, long offset)
    {
        add(elt);
        return;
        
        if (offset > 0) {
            delayedElement de;
            de.delay = offset;
            de.element = elt;
            fDelayed.push_back(de);
        }
        else add (elt);
    }
    
    //________________________________________________________________________
    // checks ready elements in the list of delayed elements
    // 'time' is the time elapsed since the last check, it is expressed in
    // <division> time units
    void xmlpart2guido::checkDelayed (long time)
    {
        vector<delayedElement>::iterator it = fDelayed.begin();
        while (it!=fDelayed.end()) {
            it->delay -= time;
            if (it->delay < 0) {
                add (it->element);
                it = fDelayed.erase(it);
            }
            else it++;
        }
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::stackClean ()
    {
        if (fInCue) {
            pop();
            fInCue = false;
            
            // add EMPTY if durationInCue>0 and fCurrentMeasurePosition is not equal to fCurrentMeasureLength
            durationInCue.rationalise();
            if (durationInCue.getNumerator() > 0) {
                guidonoteduration dur (durationInCue.getNumerator(), durationInCue.getDenominator());
                Sguidoelement note = guidonote::create(fTargetVoice, "empty", 0, dur, "");
                add (note);
                fCurrentVoicePosition += durationInCue;
                fCurrentVoicePosition.rationalise();
            }
            
            durationInCue = 0;
        }
        if (fInGrace) {
            pop();
            fInGrace = false;
        }
        if (fTextTagOpen>0) {
            while (fTextTagOpen>0) {
                pop();
                fTextTagOpen--;
            }
        }
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::checkStaff (int staff) {
        if (staff != fCurrentStaff) {
            Sguidoelement tag = guidotag::create("staff");
            int offset = staff - fCurrentStaff;
            fCurrentStaff = staff;
            fCurrentStaffIndex += offset;
            tag->add (guidoparam::create(fCurrentStaffIndex, false));
            add (tag);
			
            //// Add staffFormat if needed
            // Case1: If previous staff has Lyrics, then move current staff lower to create space: \staffFormat<dy=-5>
        }
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::moveMeasureTime (int duration, bool moveVoiceToo, int x_default)
    {
        rational r(duration, fCurrentDivision*4);
        r.rationalise();
        fCurrentMeasurePosition += r;
        fCurrentMeasurePosition.rationalise();
        if (fCurrentMeasurePosition > fCurrentMeasureLength)
            fCurrentMeasureLength = fCurrentMeasurePosition;
        if (moveVoiceToo) {
            fCurrentVoicePosition += r;
            fCurrentVoicePosition.rationalise();
        }
    }
    
    //______________________________________________________________________________
    // check the current position in the current voice:  when it lags behind
    // the current measure position, it creates the corresponding empty element
    //______________________________________________________________________________
    void xmlpart2guido::checkVoiceTime ( const rational& currTime, const rational& voiceTime)
    {
        rational diff = currTime - voiceTime;
        diff.rationalise();
        if (diff.getNumerator() > 0) {
            guidonoteduration dur (diff.getNumerator(), diff.getDenominator());
            Sguidoelement note = guidonote::create(fTargetVoice, "empty", 0, dur, "");
            add (note);
            fCurrentVoicePosition += diff;
            fCurrentVoicePosition.rationalise();
        }
        else if (diff.getNumerator() < 0)
        {
            if (!fInCue)
                cerr << "warning! checkVoiceTime: measure time behind voice time " << string(diff) << "(measure "<< fMeasNum<<")" << endl;
        }
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::visitStart ( S_backup& elt )
    {
        stackClean();	// closes pending chords, cue and grace
        int duration = elt->getIntValue(k_duration, 0);
        if (duration) {
            // backup is supposed to be used only for moving between voices
            // thus we don't move the voice time (which is supposed to be 0)
            moveMeasureTime (-duration, false);
        }
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::visitStart ( S_forward& elt )
    {
        bool scanElement = (elt->getIntValue(k_voice, 0) == fTargetVoice)
        && (elt->getIntValue(k_staff, 0) == fTargetStaff);
        int duration = elt->getIntValue(k_duration, 0);
        moveMeasureTime(duration, scanElement);
        if (!scanElement) return;
        
        stackClean();	// closes pending chords, cue and grace
        
        if (duration) {
            rational r(duration, fCurrentDivision*4);
            r.rationalise();
            guidonoteduration dur (r.getNumerator(), r.getDenominator());
            Sguidoelement note = guidonote::create(fTargetVoice, "empty", 0, dur, "");
            add (note);
            fMeasureEmpty = false;
        }
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::visitStart ( S_part& elt )
    {
        reset();
        if (!current()) {
            Sguidoelement seq = guidoseq::create();
            start (seq);
        }
        fCurrentPart = elt;
    }
    
    
    //______________________________________________________________________________
    void xmlpart2guido::visitStart ( S_measure& elt )
    {
        const string& implicit = elt->getAttributeValue ("implicit");
        if (implicit == "yes") fPendingBar = false;
        if (fPendingBar) {
            // before adding a bar, we need to check that there are no repeat begin at this location
            ctree<xmlelement>::iterator repeat = elt->find(k_repeat);
            if ((repeat == elt->end()) || (repeat->getAttributeValue("direction") != "forward")) {
                checkStaff (fTargetStaff);
                
                Sguidoelement tag;
                
                // Check bar-style if doubleBar
                if (fDoubleBar)
                    tag = guidotag::create("doubleBar");
                else
                    tag = guidotag::create("bar");
                
                add (tag);
            }
        }
        fCurrentMeasure = elt;
        fMeasNum++;
        fCurrentMeasureLength.set  (0, 1);
        fCurrentMeasurePosition.set(0, 1);
        fCurrentVoicePosition.set  (0, 1);
        fInhibitNextBar = false; // fNotesOnly;
        fPendingBar = false;
        fDoubleBar = false;
        fPendingPops = 0;
        fMeasureEmpty = true;
        if (fGenerateComments) {
            stringstream s;
            s << "   (* meas. " << fMeasNum << " *) ";
            string comment="\n"+s.str();
            Sguidoelement elt = guidoelement ::create(comment);
            add (elt);
        }
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::visitEnd ( S_measure& elt )
    {
        stackClean();	// closes pending chords, cue and grace
        checkVoiceTime (fCurrentMeasureLength, fCurrentVoicePosition);
        
        if (!fInhibitNextBar) {
            if (fGenerateBars) fPendingBar = true;
            else if (!fMeasureEmpty) {
                if (fCurrentVoicePosition < fCurrentMeasureLength)
                    fPendingBar = true;
            }
        }
        
        // Inhibit barline generation next time if bar-style of barline of this measure is "none"
        ctree<xmlelement>::iterator barStyle = elt->find(k_bar_style);
        if (barStyle != elt->end())
        {
            if (barStyle->getValue() == "none")
                fPendingBar = false;
            else if (barStyle->getValue() == "light-light")
                fDoubleBar = true;
            
        }
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::visitStart ( S_direction& elt )
    {
        if (fNotesOnly || (elt->getIntValue(k_staff, 0) != fTargetStaff)) {
            fSkipDirection = true;
        }
        else {
            fCurrentOffset = elt->getLongValue(k_offset, 0);
        }
        
        string placement = elt->getAttributeValue("placement");
        if (placement == "above")
        {
            directionPlacementAbove = true;
        }else
            directionPlacementAbove = false;
    }
    
    void xmlpart2guido::visitStart ( S_words& elt )
    {
        if (fSkipDirection) return;
        
        tempoWord = elt->getValue();
        
        string font_family = elt->getAttributeValue("font-family");
        string font_size = elt->getAttributeValue("font-size");
        string font_weight = elt->getAttributeValue("font-weight");
        string font_style = elt->getAttributeValue("font-style");
        wordParams = "\""+tempoWord+"\"";
        if (font_family.size())
            wordParams += ",font=\""+font_family+"\"";
        if (font_size.size())
            wordParams += ",fsize="+font_size+"pt";
        
        // Add font styles
        string fattrib;
        if (font_weight=="bold")
            fattrib +="b";
        if (font_style=="italic")
            fattrib +="i";
        if (fattrib.size())
            wordParams += ",fattrib=\""+fattrib+"\"";
        
        wordPointer = elt;
        
        if (wordParams.size())
        {
            directionWord = true;
        }
    }
    
    void xmlpart2guido::visitStart ( S_rehearsal& elt )
    {
        if (fSkipDirection) return;
        
        string rehearsalValue = elt->getValue();
        rehearsalValue = "\""+rehearsalValue+"\"";
        
        string enclosure = elt->getAttributeValue("enclosure");
        string font_size = elt->getAttributeValue("font-size");
        string font_weight = elt->getAttributeValue("font-weight");
        string font_style = elt->getAttributeValue("font-style");
        
        /// NOTE:
        /*
         We should ideally use the MARK tag of Guido. However MARK does not have
         text styling, nor enclosure, nor positioning parameters.
         
         For now, we use the TEXT tag which ignores only Enclosure.
         */
        
        if (rehearsalValue.size())
        {
            //// Using MARK tag:
            Sguidoelement tag = guidotag::create("mark");
            if (enclosure.size())
            {
                rehearsalValue += ", enclosure=\""+enclosure+"\"";
            }else
            {
                // declare rectangle by default
                rehearsalValue += ", enclosure=\"square\"";
            }
            if (font_size.size())
                rehearsalValue += ", fsize="+font_size+"pt";
            
            tag->add (guidoparam::create(rehearsalValue.c_str(), false));
            xml2guidovisitor::addPosition(elt, tag, -2, -4);
            
            //// Using TEXT tag:
            /*
             // Add font styles
             string fattrib;
             if (font_weight=="bold")
             fattrib +="b";
             if (font_style=="italic")
             fattrib +="i";
             if (fattrib.size())
             rehearsalValue += ",fattrib=\""+fattrib+"\"";
             Sguidoelement tag = guidotag::create("text");
             tag->add (guidoparam::create(rehearsalValue.c_str(), false));
             xml2guidovisitor::addPosition(elt, tag, 11);
             */
            add (tag);
            
            // add an additional SPACE<0> tag in case
            Sguidoelement tag2 = guidotag::create("space");
            tag2->add (guidoparam::create(0, false));
            add (tag2);
        }
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::visitEnd ( S_direction& elt )
    {
        // Generate Tempo tag here to take into account WORDS and Metronome alltogether in Tempo tag
        if (fGenerateTempo)
        {
            Sguidoelement tag = guidotag::create("tempo");
            string tempoParams;
            if (directionPlacementAbove && directionWord)
            {
                tempoParams = "\""+tempoWord+" "+tempoMetronome+"\"";
                directionWord = false;
            }
            else
                tempoParams = "\""+tempoMetronome+"\"";
            
            // add a dy=4 to avoid eventual collisions
            tempoParams += ", dy=4";
            
            if (tempoParams.size())
            {
                tag->add (guidoparam::create(tempoParams.c_str(), false));
                if (fCurrentOffset) addDelayed(tag, fCurrentOffset);
                add (tag);
            }
        }
        
        if (directionWord)
        {
            Sguidoelement tag = guidotag::create("text");
            tag->add (guidoparam::create(wordParams.c_str(), false));
            xml2guidovisitor::addPosition(wordPointer, tag, 11);
            //add (tag);
            push(tag);
            fTextTagOpen++;
            
            /// add an additional SPACE<0> tag in case
            //Sguidoelement tag2 = guidotag::create("space");
            //tag2->add (guidoparam::create(0, false));
            //add (tag2);
        }
        
        
        fSkipDirection = false;
        fCurrentOffset = 0;
        tempoWord.clear();
        tempoMetronome.clear();
        wordParams.clear();
        fGenerateTempo = false;
        directionWord = false;
        wordPointer = NULL;
    }
    
    /// Closing Guido TEXT tags; to be used once a NOTE/CHORD is embedded.
    void xmlpart2guido::checkTextEnd() {
        if (fTextTagOpen>0) {
            while (fTextTagOpen>0) {
                pop();
                fTextTagOpen--;
            }
        }
    }
    
    void xmlpart2guido::visitEnd ( S_words& elt )
    {
        // Nothing to do here?
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::visitEnd ( S_key& elt )
    {
        if (fNotesOnly) return;
        Sguidoelement tag = guidotag::create("key");
        tag->add (guidoparam::create(keysignvisitor::fFifths, false));
        //add (tag);
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::visitStart ( S_coda& elt )
    {
        if (fSkipDirection) return;
        Sguidoelement tag = guidotag::create("coda");
        add(tag);
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::visitStart ( S_segno& elt )
    {
        if (fSkipDirection) return;
        Sguidoelement tag = guidotag::create("segno");
        add(tag);
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::visitStart ( S_wedge& elt )
    {
        if (fSkipDirection) return;
        
        bool wedgeStart = false;
        
        string type = elt->getAttributeValue("type");
        Sguidoelement tag;
        if (type == "crescendo") {
            tag = guidotag::create("crescBegin");
            fCrescPending = true;
            wedgeStart = true;
        }
        else if (type == "diminuendo") {
            tag = guidotag::create("dimBegin");
            fCrescPending = false;
            wedgeStart = true;
        }
        else if (type == "stop") {
            if (fIgnoreWedgeWithOffset) {
                fIgnoreWedgeWithOffset = false;
                return; // FIXME: Ignore Offset Wedge à la Verovio
            }

            tag = guidotag::create(fCrescPending ? "crescEnd" : "dimEnd");
        }
        
        if (tag) {
            //// Also add SPREAD values (in mXML tenths - conversion: (X / 10) * 2)
            //// Spread is present right away for a diminuendo, it'll be present for crescendo at its STOP type
            if (type == "diminuendo") {
                float spreadValue = elt->getAttributeFloatValue("spread", 15.0);
                if (spreadValue != 15.0) {
                    stringstream s;
                    s << "deltaY=" << (spreadValue/10)*2 << "hs";
                    tag->add (guidoparam::create(s.str(), false));
                }
                
                /*stringstream s;
                s << "autopos=\"on\"";
                tag->add (guidoparam::create(s.str(), false));*/
            
            }else if (type == "crescendo")
            {
                ctree<xmlelement>::iterator nextevent = find(fCurrentPart->begin(), fCurrentPart->end(), elt);
                int crescendoNumber = elt->getAttributeIntValue("number", 1);   // default is 1 for wedge!
                
                nextevent++;    // advance one step
                
                // find next S_direction in measure
                ctree<xmlelement>::iterator nextWedge = fCurrentMeasure->find(k_wedge, nextevent);
                
                while ( ( nextWedge->getAttributeIntValue("number", 1) != crescendoNumber)
                       &&
                       (nextWedge->getAttributeValue("type")!="stop") )
                {
                    nextWedge = fCurrentMeasure->find(k_wedge, nextevent++);
                }
                
                /// fetch dx1 and dx2 value based on ending
                float posx2 = nextWedge->getAttributeFloatValue("relative-x", 0);
                float posx1 = elt->getAttributeFloatValue("relative-x", 0);  //elt->getAttributeFloatValue("default-x", 0) +

                if (fCurrentOffset) {
                    /*rational offsetDur(fCurrentOffset, fCurrentDivision);
                    offsetDur.rationalise();
                    
                    rational offsetPosition = fCurrentMeasurePosition + offsetDur;
                    
                    int measureJump = int(floor(offsetPosition.toFloat()+0.5f));    //((int)floor(x + 0.5f))
                    rational measureJumpRat(measureJump, 1);
                    offsetPosition -= measureJumpRat;
                    
                    auto timePos4measure = timePositions.find(fMeasNum+measureJump);

                    cout<<"Crescendo Begin at "<< fMeasNum<< " default-x="<< elt->getAttributeIntValue("default-x", 0)<< " relative-x="<<elt->getAttributeIntValue("relative-x", 0)<<" Offset="<<fCurrentOffset<<" ENDING: default-x="<< nextWedge->getAttributeIntValue("default-x", 0) << " relative-x="<<nextWedge->getAttributeIntValue("relative-x", 0)<<endl;

                    cout<<"-----Dealing with offset "<< fCurrentOffset<< " with Division "<< fCurrentDivision<<" measure jump: "<<measureJump<<endl;

                    
                    if (timePos4measure != timePositions.end()) {
                        auto voiceInTimePosition = timePos4measure->second.find(offsetPosition);
                        if (voiceInTimePosition != timePos4measure->second.end()) {
                            auto minXPos = std::min_element(voiceInTimePosition->second.begin(),voiceInTimePosition->second.end() );
                            posx1 = posx1 + *minXPos;
                            cout<<"\tOFFSET TimePosition is="<< *minXPos <<" "<<posx1<<endl;;
                            //posx2 = posx2 + *minXPos;
                        }else {
                            cerr<<"\tERROR: NO TIME POS FOR VOICE POSITION "<<offsetPosition.toString()<<" TO INFER WEDGE OFFSET!"<<endl;
                        }
                    }else
                    {
                        cerr<<"\tNO TIMEPOS for measure "<<fMeasNum<<" timePositions size="<<timePositions.size()<<endl;
                    }*/
                    
                    // FIXME: Impossible for now to handle Wedges with Direction Offset! Ignoring... .
                    fIgnoreWedgeWithOffset = true;


                }
                if (fIgnoreWedgeWithOffset)
                {
                    cerr <<"\tIgnoring Wedge with Offset on measure "<<fMeasNum<<endl;
                    return;         // FIXME: Ignoring Offset wedges à la Verovio
                }

                
                //// Add dx1 and dx2 parameters
                if (posx1!=0.0) {
                    posx1 = (posx1 / 10) * 2;   // convert to half spaces
                    
                    stringstream s;
                    s << "dx1=" << posx1 << "hs";
                    tag->add (guidoparam::create(s.str(), false));
                }
                if (posx2!=0.0) {
                    posx2 = (posx2 / 10) * 2;   // convert to half spaces
                    
                    stringstream s;
                    s << "dx2=" << posx2 << "hs";
                    tag->add (guidoparam::create(s.str(), false));
                }
                
                //// Add spreadvalue from the Crescendo Ending
                float spreadValue = nextWedge->getAttributeFloatValue("spread", 15.0);
                if (spreadValue != 15.0) {
                    stringstream s;
                    s << "deltaY=" << (spreadValue/10)*2 << "hs";
                    tag->add (guidoparam::create(s.str(), false));
                }
                
                
                // Add new AutoPos="on"
                /*stringstream s;
                s << "autopos=\"on\"";
                tag->add (guidoparam::create(s.str(), false));*/
            }
            
            
            xml2guidovisitor::addPosY(elt, tag, 12, 1.0);    // removed negative multiplier. Fixed in GuidoLib 1.6.5
            
            // add only RELATIVE-X since it doesn't depend on TimePosition
            /*float posx = elt->getAttributeFloatValue("relative-x", 0);  //elt->getAttributeFloatValue("default-x", 0) +
            if (posx && wedgeStart && (type != "diminuendo")) {
                posx = (posx / 10) * 2;   // convert to half spaces
                
                /// FIXME: Can't handle OFFSET with Guido! If positive, just add a small value for coherence!
                //if (fCurrentOffset>0)
                //    posx +=3;
                
                stringstream s;
                s << "dx1=" << posx << "hs";
                tag->add (guidoparam::create(s.str(), false));
            }*/
            
            
            if (fCurrentOffset) {
                addDelayed(tag, fCurrentOffset);
            }
            else {
                add (tag);
            }
        }
        
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::visitEnd ( S_metronome& elt )
    {
        if (fSkipDirection) return;
        
        metronomevisitor::visitEnd (elt);
        if (fBeats.size() != 1) return;					// support per minute tempo only (for now)
        if (!metronomevisitor::fPerMinute) return;		// support per minute tempo only (for now)
        
        //Sguidoelement tag = guidotag::create("tempo");
        beat b = fBeats[0];
        rational r = NoteType::type2rational(NoteType::xml(b.fUnit)), rdot(3,2);
        while (b.fDots-- > 0) {
            r *= rdot;
        }
        r.rationalise();
        
        stringstream s;
        s << "[" << (string)r << "] = " << metronomevisitor::fPerMinute;
        //tag->add (guidoparam::create("tempo=\""+s.str()+"\"", false));
        tempoMetronome = s.str();
        //if (fCurrentOffset) addDelayed(tag, fCurrentOffset);
        //add (tag);
        
        fGenerateTempo = true;
    }
    
    int xmlpart2guido::checkDynamics(rational posInMeasure )
    {
        ctree<xmlelement>::literator iter;
        
        if (fDynamics) {
            
            int tagCounter=0;
            
            for (iter = fDynamics->lbegin(); iter != fDynamics->lend(); iter++) {
                if ((*iter)->getType() != k_other_dynamics) {
                    Sguidoelement tag = guidotag::create("intens");
                    tag->add (guidoparam::create((*iter)->getName()));
                    if (fGeneratePositions) xml2guidovisitor::addPosY(fDynamics, tag, 12, 1);
                    //if (fGeneratePositions) xml2guidovisitor::addPosition(elt, tag, 12, 1);  // Avoid using default-x since it is measured from the beginning of the measure for S_direction!
                    /// Infer X-Position from TimePosition
                    
                    int measureNum = fCurrentMeasure->getAttributeIntValue("number", 0);
                    auto timePos4measure = timePositions.find(measureNum);
                    float intens_default_x =fDynamics->getAttributeFloatValue("default-x", 0);
                    float intens_relative_x =fDynamics->getAttributeFloatValue("relative-x", 0);
                    float intens_xpos = intens_default_x + intens_relative_x;
                    //cout<<"Measure: "<<fMeasNum <<": Got to Intens "<< (*iter)->getName()<<" with default-x="<< intens_default_x<< " relative-x="<<intens_relative_x ;
                    
                    if ((intens_xpos!=0)&&(timePos4measure != timePositions.end())) {
                        auto voiceInTimePosition = timePos4measure->second.find(posInMeasure);
                        if (voiceInTimePosition != timePos4measure->second.end()) {
                            auto minXPos = std::min_element(voiceInTimePosition->second.begin(),voiceInTimePosition->second.end() );
                            if (intens_xpos != *minXPos) {
                                int intensDx = (intens_relative_x/10)*2;
                                // apply default-x ONLY if it exists
                                if (intens_default_x!=0)
                                    intensDx = ( (intens_xpos - *minXPos)/ 10 ) * 2;   // convert to half spaces
                                
                                /// FIXME: Can't handle OFFSET with Guido! If positive, just add a small value for coherence!
                                if (fCurrentOffset>0)
                                    intensDx +=3;
                                
                                stringstream s;
                                s << "dx=" << intensDx ;
                                tag->add (guidoparam::create(s.str(), false));
                            }
                        }else {
                            cerr<<"ERROR: NO TIME POS FOR VOICE POSITION"<<fCurrentVoicePosition.toString()<<" TO INFER Dx for DYNAMICS!"<<endl;
                        }
                    }
                    
                    ///
                    
                    if (fCurrentOffset)
                        addDelayed(tag, fCurrentOffset);
                    else {
                        push (tag);
                        tagCounter++;
                    }
                }
            }
            
            fDynamics = (void*)0;
            return tagCounter;
            
        }else {
            return 0;
        }
        
        fDynamics = (void*)0;
        return 0;
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::visitStart( S_dynamics& elt)
    {
        if (fSkipDirection) return;
        
        /// MusicXML Note for default-x in S_direction: For these elements, the default-x attribute is measured from the start of the current measure.
        ///         For other elements, it is measured from the left-hand side of the note or the musical position within the bar.)
        /// So in order to infer Dx out of default-x for S_dynamics, we should generate it once TimePosition has been advanced!
        
        fDynamics = elt;
        
        return;
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::visitStart( S_octave_shift& elt)
    {
        if (fSkipDirection) return;
        
        const string& type = elt->getAttributeValue("type");
        int size = elt->getAttributeIntValue("size", 0);
        
        switch (size) {
            case 8:		size = 1; break;
            case 15:	size = 2; break;
            default:	return;
        }
        
        if (type == "up")
            size = -size;
        else if (type == "stop")
            size = 0;
        else if (type != "down") return;
        
        Sguidoelement tag = guidotag::create("oct");
        if (tag) {
            tag->add (guidoparam::create(size, false));
            add (tag);
        }
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::visitStart ( S_note& elt )
    {
        notevisitor::visitStart ( elt );
    }
    
    //______________________________________________________________________________
    string xmlpart2guido::alter2accident ( float alter )
    {
        stringstream s;
        while (alter > 0.5) {
            s << "#";
            alter -= 1;
        }
        while (alter < -0.5) {
            s << "&";
            alter += 1;
        }
        
        string accident;
        s >> accident;
        return accident;
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::visitEnd ( S_sound& elt )
    {
        if (fNotesOnly) return;
        
        Sguidoelement tag = 0;
        Sxmlattribute attribute;
        
        if ((attribute = elt->getAttribute("dacapo")))
            tag = guidotag::create("daCapo");
        else {
            if ((attribute = elt->getAttribute("dalsegno"))) {
                tag = guidotag::create("dalSegno");
            }
            else if ((attribute = elt->getAttribute("tocoda"))) {
                tag = guidotag::create("daCoda");
            }
            else if ((attribute = elt->getAttribute("fine"))) {
                tag = guidotag::create("fine");
            }
            //		if (tag) tag->add(guidoparam::create("id="+attribute->getValue(), false));
        }
        if (tag) add (tag);
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::visitEnd ( S_ending& elt )
    {
        string type = elt->getAttributeValue("type");
        if (type == "start") {
            Sguidoelement tag = guidotag::create("volta");
            string num = elt->getAttributeValue ("number");
            tag->add(guidoparam::create(num, true));
            tag->add(guidoparam::create(num + ".", true));
            push(tag);
        }
        else {
            if (type == "discontinue")
                current()->add(guidoparam::create("format=\"|-\"", false));
            pop();
        }
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::visitEnd ( S_repeat& elt )
    {
        Sguidoelement tag;
        string direction = elt->getAttributeValue("direction");
        if (direction == "forward")
            tag = guidotag::create("repeatBegin");
        else if (direction == "backward") {
            tag = guidotag::create("repeatEnd");
            fInhibitNextBar = true;
        }
        if (tag) add(tag);
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::visitStart ( S_barline& elt )
    {
        const string& location = elt->getAttributeValue("location");
        if (location == "middle") {
            // todo: handling bar-style (not yet supported in guido)
            Sguidoelement tag = guidotag::create("bar");
            add(tag);
        }
        // todo: support for left and right bars
        // currently automatically handled at measure boundaries
        else if (location == "right") {
        }
        else if (location == "left") {
        }
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::visitEnd ( S_time& elt )
    {
        string timesign;
        if (!timesignvisitor::fSenzaMisura) {
            if (timesignvisitor::fSymbol == "common") {
                rational ts = timesignvisitor::timesign(0);
                if ((ts.getDenominator() == 2) && (ts.getNumerator() == 2))
                    timesign = "C/";
                else if ((ts.getDenominator() == 4) && (ts.getNumerator() == 4))
                    timesign = "C";
                else
                    timesign = string(ts);
                fCurrentTimeSign = ts;
            }
            else if (timesignvisitor::fSymbol == "cut") {
                timesign = "C/";
                fCurrentTimeSign = rational(2,2);
            }
            else {
                stringstream s; string sep ="";
                fCurrentTimeSign.set(0,1);
                for (unsigned int i = 0; i < timesignvisitor::fTimeSign.size(); i++) {
                    s << sep << timesignvisitor::fTimeSign[i].first << "/" << timesignvisitor::fTimeSign[i].second;
                    sep = "+";
                    //				rational ts = timesignvisitor::timesign(i);
                    fCurrentTimeSign += timesignvisitor::timesign(i);
                }
                s >> timesign;
            }
            
        }
        if (fNotesOnly) return;
        
        Sguidoelement tag = guidotag::create("meter");
        tag->add (guidoparam::create(timesign));
        if (fGenerateBars) tag->add (guidoparam::create("autoBarlines=\"off\"", false));
        if (fGenerateAutoMeasureNum) tag->add (guidoparam::create("autoMeasuresNum=\"system\"", false));
        //add(tag);
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::visitStart ( S_attributes& elt )
    {
        // get clef, key, division, staves, time and key in order!
        /* Example:
         <divisions>2</divisions>
         <key>
         <fifths>1</fifths>
         <mode>major</mode>
         </key>
         <time>
         <beats>3</beats>
         <beat-type>2</beat-type>
         </time>
         <staves>2</staves>
         <clef number="1">
         <sign>G</sign>
         <line>2</line>
         </clef>
         <clef number="2">
         <sign>F</sign>
         <line>4</line>
         <transpose>
         <diatonic>-1</diatonic>
         <chromatic>-2</chromatic>
         </transpose>
         </clef>
         *****/
		
        ctree<xmlelement>::iterator iter = elt->begin();
        
        // set division
        int divisions = (elt)->getIntValue(k_divisions, -1);
        if (divisions != -1)
            fCurrentDivision = divisions;
        
        // Generate Clef first
        iter = elt->find(k_clef);
        while (iter != elt->end())
        {
            // There is a clef!
            string clefsign = iter->getValue(k_sign);
            int clefline = iter->getIntValue(k_line, 0);
            int clefoctavechange = iter->getIntValue(k_clef_octave_change, 0);
            
            /// Actions:
            int staffnum = iter->getAttributeIntValue("number", 0);
            if ((staffnum != fTargetStaff) || fNotesOnly)
            {
                /// Search again for other clefs:
                iter++;
                iter = elt->find(k_clef, iter);
                continue;
            }
            
            stringstream s;
            if ( clefsign == "G")			s << "g";
            else if ( clefsign == "F")	s << "f";
            else if ( clefsign == "C")	s << "c";
            else if ( clefsign == "percussion")	s << "perc";
            else if ( clefsign == "TAB")	s << "TAB";
            else if ( clefsign == "none")	s << "none";
            else {													// unknown clef sign !!
                cerr << "warning: unknown clef sign \"" << clefsign << "\"" << endl;
                return;
            }
            
            string param;
            if (clefline != 0)  // was clefvisitor::kStandardLine
                s << clefline;
            s >> param;
            if (clefoctavechange == 1)
                param += "+8";
            else if (clefoctavechange == -1)
                param += "-8";
            Sguidoelement tag = guidotag::create("clef");
            checkStaff (staffnum);
            tag->add (guidoparam::create(param));
            add(tag);
            
            std::pair<rational, std::string> foo = std::pair<rational, std::string>(fCurrentVoicePosition ,clefsign);
            staffClefMap.insert(std::pair<int, std::pair < int , std::pair<rational, std::string> > >(fCurrentStaffIndex, std::pair< int, std::pair< rational, std::string > >(fMeasNum, foo) ) );
			
            /// Search again for other clefs:
            iter++;
            iter = elt->find(k_clef, iter);
        }
        
        // staves are treated at the S_Part level (see xml2guidovisitor) to create accolades
        
        // Generate key
        iter = elt->find(k_key);
        if ((iter != elt->end())&&(!fNotesOnly))
        {
            string keymode = iter->getValue(k_mode);
            int keyfifths = iter->getIntValue(k_fifths, 0);
            Sguidoelement tag = guidotag::create("key");
            tag->add (guidoparam::create(keyfifths, false));
            add (tag);
        }
        
        // Generate Time Signature info and METER info
        iter = elt->find(k_time);
        if ((iter != elt->end())&&(!fNotesOnly))
        {
            //int timebeat_type = iter->getIntValue(k_beat_type, 0);
            //int timebeats = iter->getIntValue(k_beats, 0);
            bool senzamesura = (iter->find(k_senza_misura) != iter->end());
            string timesymbol = iter->getAttributeValue("symbol");
            std::vector<std::pair<std::string,std::string> > fTimeSignInternal ;
            
            // IOSEPRAC-185: Get all pairs for Composite Time Signatures
            ctree<xmlelement>::iterator iter_beat = iter->find(k_beats);
            ctree<xmlelement>::iterator iter_beatType = iter->find(k_beat_type);

            while (iter_beat != iter->end())
            {
                //                fTimeSignInternal.push_back(make_pair(iter_beat->getValue(k_beats), iter2->getValue(k_beat_type)));
                fTimeSignInternal.push_back(make_pair(iter_beat->getValue(),
                                                      iter_beatType->getValue()));
                iter_beat = iter->find(k_beats, iter_beat++);
                iter_beatType = iter->find(k_beat_type, iter_beatType++);
            }
            
            //// Actions:
            string timesign;
            if (!senzamesura) {
                if (timesymbol == "common") {
                    // simulation of timesignvisitor::timesign function
                    //rational ts = timesignvisitor::timesign(0);
                    size_t index = 0;
                    rational ts(0,1);
                    if (index < fTimeSignInternal.size()) {
                        const pair<string,string>& tss = fTimeSignInternal[index];
                        long num = strtol (tss.first.c_str(), 0, 10);
                        long denum = strtol (tss.second.c_str(), 0, 10);
                        if (num && denum) ts.set(num, denum);
                    }
                    ///
                    if ((ts.getDenominator() == 2) && (ts.getNumerator() == 2))
                        timesign = "C/";
                    else if ((ts.getDenominator() == 4) && (ts.getNumerator() == 4))
                        timesign = "C";
                    else
                        timesign = string(ts);
                    fCurrentTimeSign = ts;
                }
                else if (timesymbol == "cut") {
                    timesign = "C/";
                    fCurrentTimeSign = rational(2,2);
                }
                else {

                    stringstream s; string sep ="";
                    fCurrentTimeSign.set(0,1);
                    for (unsigned int i = 0; i < fTimeSignInternal.size(); i++) {
                        s << sep << fTimeSignInternal[i].first << "/" << fTimeSignInternal[i].second;
                        sep = "+";
                        //fCurrentTimeSign += timesignvisitor::timesign(i);
                        // simulation of timesignvisitor::timesign function
                        //rational ts = timesignvisitor::timesign(i);
                        size_t index = i;
                        rational ts(0,1);
                        if (index < fTimeSignInternal.size()) {
                            const pair<string,string>& tss = fTimeSignInternal[index];
                            long num = strtol (tss.first.c_str(), 0, 10);
                            long denum = strtol (tss.second.c_str(), 0, 10);
                            if (num && denum) ts.set(num, denum);
                        }
                        ///
                        fCurrentTimeSign += ts;
                    }
                    s >> timesign;
                }
                
            }
            
            Sguidoelement tag = guidotag::create("meter");
            tag->add (guidoparam::create(timesign));
            if (fGenerateBars) tag->add (guidoparam::create("autoBarlines=\"off\"", false));
            if (fGenerateAutoMeasureNum) tag->add (guidoparam::create("autoMeasuresNum=\"system\"", false));
            if (iter->getAttributeValue("print-object")!="no")
                add(tag);
        }
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::visitEnd ( S_clef& elt )
    {
        int staffnum = elt->getAttributeIntValue("number", 0);
        if ((staffnum != fTargetStaff) || fNotesOnly) return;
        
        stringstream s;
        if ( clefvisitor::fSign == "G")			s << "g";
        else if ( clefvisitor::fSign == "F")	s << "f";
        else if ( clefvisitor::fSign == "C")	s << "c";
        else if ( clefvisitor::fSign == "percussion")	s << "perc";
        else if ( clefvisitor::fSign == "TAB")	s << "TAB";
        else if ( clefvisitor::fSign == "none")	s << "none";
        else {													// unknown clef sign !!
            cerr << "warning: unknown clef sign \"" << clefvisitor::fSign << "\"" << endl;
            return;
        }
        
        string param;
        if (clefvisitor::fLine != clefvisitor::kStandardLine)
            s << clefvisitor::fLine;
        s >> param;
        if (clefvisitor::fOctaveChange == 1)
            param += "+8";
        else if (clefvisitor::fOctaveChange == -1)
            param += "-8";
        Sguidoelement tag = guidotag::create("clef");
        checkStaff (staffnum);
        tag->add (guidoparam::create(param));
        //add(tag);
    }
    
    //______________________________________________________________________________
    // tools and methods for converting notes
    //______________________________________________________________________________
    vector<S_slur>::const_iterator xmlpart2guido::findTypeValue ( const std::vector<S_slur>& slurs, const string& val ) const
    {
        std::vector<S_slur>::const_iterator i;
        for (i = slurs.begin(); i != slurs.end(); i++) {
            if ((*i)->getAttributeValue("type") == val) break;
        }
        return i;
    }
    
    //______________________________________________________________________________
    vector<S_tied>::const_iterator xmlpart2guido::findTypeValue ( const std::vector<S_tied>& tied, const string& val ) const
    {
        std::vector<S_tied>::const_iterator i;
        for (i = tied.begin(); i != tied.end(); i++) {
            if ((*i)->getAttributeValue("type") == val) break;
        }
        return i;
    }
    
    //______________________________________________________________________________
    vector<S_beam>::const_iterator xmlpart2guido::findValue ( const std::vector<S_beam>& beams, const string& val ) const
    {
        std::vector<S_beam>::const_iterator i;
        for (i = beams.begin(); i != beams.end(); i++) {
            if ((*i)->getValue() == val) break;
        }
        return i;
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::checkTiedBegin ( const std::vector<S_tied>& tied )
    {
        std::vector<S_tied>::const_iterator i;
        for (i = tied.begin(); i != tied.end(); i++) {
            if ((*i)->getAttributeValue("type") == "start") {
                Sguidoelement tag = guidotag::create("tieBegin");
                string num = (*i)->getAttributeValue ("number");
                if (num.size())
                    tag->add (guidoparam::create(num, false));
                string placement = (*i)->getAttributeValue("placement");
                if (placement == "below")
                    tag->add (guidoparam::create("curve=\"down\"", false));
                add(tag);
            }
        }
    }
    
    void xmlpart2guido::checkTiedEnd ( const std::vector<S_tied>& tied )
    {
        std::vector<S_tied>::const_iterator i;
        for (i = tied.begin(); i != tied.end(); i++) {
            if ((*i)->getAttributeValue("type") == "stop") {
                Sguidoelement tag = guidotag::create("tieEnd");
                string num = (*i)->getAttributeValue ("number");
                if (num.size())
                    tag->add (guidoparam::create(num, false));
                add(tag);
            }
        }
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::checkSlurBegin ( const std::vector<S_slur>& slurs )
    {
        std::vector<S_slur>::const_iterator i;
        for (i = slurs.begin(); i != slurs.end(); i++) {
            if ((*i)->getAttributeValue("type") == "start") {
                string tagName = "slurBegin";
                string num = (*i)->getAttributeValue("number");
                if (num.size()) tagName += ":" + num;
                Sguidoelement tag = guidotag::create(tagName);
                string placement = (*i)->getAttributeValue("placement");
                string orientation = (*i)->getAttributeValue("placement");
                if ((placement == "below")||(orientation=="under"))
                    tag->add (guidoparam::create("curve=\"down\"", false));
                if ((placement == "above")||(orientation=="over"))
                    tag->add (guidoparam::create("curve=\"up\"", false));
                add(tag);
            }
        }
    }
    
    void xmlpart2guido::checkSlurEnd ( const std::vector<S_slur>& slurs )
    {
        std::vector<S_slur>::const_iterator i;
        for (i = slurs.begin(); i != slurs.end(); i++) {
            if ((*i)->getAttributeValue("type") == "stop") {
                string tagName = "slurEnd";
                string num = (*i)->getAttributeValue("number");
                if (num.size()) tagName += ":" + num;
                Sguidoelement tag = guidotag::create (tagName);
                add(tag);
            }
        }
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::checkBeamBegin ( const std::vector<S_beam>& beams )
    {
        /// !IMPORTANT NOTE from MXML DOC: Note that the beam number does not distinguish sets of beams that overlap, as it does for slur and other elements.
        ///             So we need to track them with s Stack
        
        std::vector<S_beam>::const_iterator i = findValue(beams, "begin");
        if (i != beams.end()) {
            // There is a Beam Begin. Creat BeamBegin tag, and add its number to Stack
            int lastBeamInternalNumber = 1;
            if (!fBeamStack.empty()) {
                std::pair<int, int> toto = fBeamStack.top();
                lastBeamInternalNumber = toto.first + 1;
            }
            std::pair<int,int> toto2(lastBeamInternalNumber, (*i)->getAttributeIntValue("number", 0));
            fBeamStack.push(toto2);
            
            /// Using \beamBegin:NUMBER
            stringstream tagName;
            tagName << "beamBegin" << ":"<< lastBeamInternalNumber;
            Sguidoelement tag = guidotag::create(tagName.str());	// poor support of the begin end form in guido
            add (tag);
            
            /// OR using \beam(...)
            //Sguidoelement tag = guidotag::create("beam");
            //push (tag);
        }
        
        if (beams.empty() && fBeamStack.empty() && notevisitor::getType()!=kRest)
        {
            // Possible candidate for \beamsOff
            Sguidoelement tag = guidotag::create("beamsOff");
            add (tag);
        }
    }
    
    void xmlpart2guido::checkBeamEnd ( const std::vector<S_beam>& beams )
    {
        /// IMPORTANT: Beam Numbering in MusicXML is not the same as in Slurs and are NOT incremental.
        ///            The only assumption we make here is that the numbers are sorted. So we use a REVERSE iterator to close Beams in Order.
        std::vector<S_beam>::const_reverse_iterator i ;
        size_t beamStackSizeBeforeClosing = fBeamStack.size();
        for (i = beams.rbegin(); (i != beams.rend() && (!fBeamStack.empty())); i++)
        {
            if (((*i)->getValue() == "end") && ((*i)->getAttributeIntValue("number", 1) == fBeamStack.top().second)) {
                // There is a Beam End. create tag and pop from stack
                int lastBeamInternalNumber = 0;
                if (!fBeamStack.empty()) {
                    lastBeamInternalNumber = fBeamStack.top().first;
                }else {
                    cerr<< "XML2Guido: Got Beam End without a beam in Stack. Skipping!"<<endl;
                    return;
                }
                
                /// using \beamEnd:NUMBER
                stringstream tagName;
                tagName << "beamEnd" << ":"<< lastBeamInternalNumber;
                Sguidoelement tag = guidotag::create(tagName.str());	// poor support of the begin end form in guido
                add (tag);
                
                /// OR using \beam(...)
                //pop();
				
                fBeamStack.pop();
            }
        }
        
        // Experimental
        if (beamStackSizeBeforeClosing > fBeamStack.size())
        {
            checkTextEnd();
        }
    }
    
    //_______________________Tuplets___________________________________
    void xmlpart2guido::checkTupletBegin ( const std::vector<S_tuplet>& tuplets,
                                          const notevisitor& nv,
                                          const S_note& elt )
    {
        std::vector<S_tuplet>::const_iterator i;
        
        for (i = tuplets.begin(); i != tuplets.end(); i++) {
            if ( (*i)->getAttributeValue("type") == "start") break;
        }
        
        if (i != tuplets.end()) {
            if (!fTupletOpened ) {
                /// Determine whether we need Brackets or not
                bool withBracket = ((*i)->getAttributeValue("bracket")=="yes");
                /// Get Tuplet Number
                int thisTupletNumber = (*i)->getAttributeIntValue("number", 1);
                /// Get Tuplet Placement and graphic type
                std::string tupletPlacement = (*i)->getAttributeValue("placement");
                std::string tupletGraphicType = nv.fGraphicType;
                long numberOfEventsInTuplet = 1;
                
                ///// Use Time-Modification to get Number of Events in Tuplet
                numberOfEventsInTuplet = nv.getTimeModification().getDenominator();
                
                //// Rational : If all note durations are equal, then use the dispNote attribute. If not, then don't!
                bool useDispNoteAttribute = true;
                long topNoteDur = nv.getDuration();
                /// Browse through all elements of Tuplet until "stop"!
                ctree<xmlelement>::iterator nextnote = find(fCurrentMeasure->begin(), fCurrentMeasure->end(), elt);
                if (nextnote != fCurrentMeasure->end()) {
                    nextnote++;	// advance one step
                }
                while (nextnote != fCurrentMeasure->end()) {
                    // looking for the next note on the target voice
                    if ((nextnote->getType() == k_note) && (nextnote->getIntValue(k_voice,0) == fTargetVoice)) {
                        
                        if ( abs( nextnote->getIntValue(k_duration, 0) - topNoteDur) > (fCurrentDivision/10) ) {
                            useDispNoteAttribute =  false;
                            cerr <<"TUPLET EVADED DISPNOTE Measure:"<<fMeasNum <<"Division:"<< fCurrentDivision <<"--> topBoteDur:"<<topNoteDur<<" this note dur="<<nextnote->getIntValue(k_duration, 0)<<endl;
                            break;
                        }
                        
                        ctree<xmlelement>::iterator iter;
                        iter = nextnote->find(k_notations);
                        if (iter != nextnote->end())
                        {
                            // There is a notation tag. Now check if there's a tuplet END with the same tuplet number.
                            //      If yes, then increment and break.
                            ctree<xmlelement>::iterator iterTuplet;
                            iterTuplet = iter->find(k_tuplet);
                            if (iterTuplet != iter->end())
                            {
                                // There is a tuplet tag!
                                int newTupletNumber = iterTuplet->getAttributeIntValue("number", 0);
                                if ((iterTuplet->getAttributeValue("type")=="stop")&&(newTupletNumber==thisTupletNumber))
                                {
                                    break;
                                }
                            }
                        }
                    }
                    nextnote++;
                }
                
                /// Determine the graphical format inside Tuplet
                std::string dispNotePar ;
                int dy1offset = 6;
                if (tupletGraphicType=="32nd")
                {
                    dispNotePar = "\"/32\"";
                    dy1offset+=4;
                }
                else if (tupletGraphicType=="16th")
                {
                    dispNotePar = "\"/16\"";
                    dy1offset+=3;
                }
                else if (tupletGraphicType=="eighth")
                {
                    dispNotePar = "\"/8\"";
                    dy1offset+=2;
                }
                else if (tupletGraphicType=="quarter")
                {
                    dispNotePar = "\"/4\"";
                    dy1offset+=1;
                }
                else if (tupletGraphicType=="half")
                {
                    dispNotePar = "\"/2\"";
                }
                else if (tupletGraphicType=="whole")
                {
                    dispNotePar = "\"/1\"";
                    dy1offset-=5;
                }
                
                /// Generate tag and parameters
                // Avoid generating parameter for triplets since Guido does this automatically
                if ((numberOfEventsInTuplet!=3) ||(dispNotePar.size()))
                {
                    Sguidoelement tag = guidotag::create("tuplet");
                    /// Add number visualiser
                    stringstream tuplet;
                    if (numberOfEventsInTuplet>1)   // workaround for pianistic Tremolos that come out of Finale as Tuplets!
                        tuplet << (withBracket? "-" : "") << numberOfEventsInTuplet << (withBracket? "-" : "");
                    tag->add (guidoparam::create(tuplet.str()));
                    
                    /// set dispNote, Possible values : "/1", "/2" "/4", "/8", "/16"
                    if (dispNotePar.size() && useDispNoteAttribute)
                    {
                        tag->add(guidoparam::create(("dispNote="+dispNotePar),false));
                    }
                    
                    //// Add Placement
                    if (tupletPlacement.size())
                    {
                        tag->add(guidoparam::create(("position=\""+tupletPlacement+"\""),false));
                    }
                    
                    //// Tuplet text position is now automatic as of Commit of Fri 2 December 2016
                    /*
                     if (fCurrentStemDirection == kStemDown)
                     {
                     if (tupletPlacement=="below")
                     {
                     dy1offset*=-1;
                     }else
                     dy1offset-=5;
                     
                     tag->add(guidoparam::create(("dy1="+std::to_string(dy1offset)),false));
                     }else if (fCurrentStemDirection == kStemUp)
                     {
                     tag->add(guidoparam::create(("dy1="+std::to_string(dy1offset)),false));
                     }else // kStemNone or kStemUndefined
                     {
                     tag->add(guidoparam::create(("dy1="+std::to_string(dy1offset)),false));
                     }*/
                    
                    push (tag);
                    fTupletOpened = true;
                    fCurrentTupletNumber = thisTupletNumber;
                }
            }
        }
    }
    
    void xmlpart2guido::checkTupletEnd ( const std::vector<S_tuplet>& tuplets )
    {
        std::vector<S_tuplet>::const_iterator i;
        for (i = tuplets.begin(); (i != tuplets.end()) && fTupletOpened; i++) {
            if (((*i)->getAttributeValue("type") == "stop") && ((*i)->getAttributeIntValue("number", 1) == fCurrentTupletNumber)) {
                fCurrentTupletNumber = 0;
                pop();
                fTupletOpened = false;
            }
        }
    }
    
    
    
    
    // ------------------- LYRICS Functions
    
    void xmlpart2guido::checkLyricBegin	 ( const std::vector<S_lyric>& lyrics )
    {
        if (notevisitor::getSyllabic()== "single")
        {
            Sguidoelement tag = guidotag::create("lyrics");
            /// replaces Spaces in text by '~' to avoid event progression!
            std::string newTxt = notevisitor::getLyricText();
            std::replace( newTxt.begin(), newTxt.end(), ' ', '~');
            tag->add (guidoparam::create(newTxt, true));
            
            /// Adjust Y-Position by infering from XML
            //tag->add (guidoparam::create(notevisitor::getLyricDy(), false));
            std::string autoPosParam="autopos=\"on\"";
            tag->add(guidoparam::create(autoPosParam, false));
            push (tag);
            
            fHasLyrics = true;
        }
        
        if ((notevisitor::getSyllabic()== "end")
            ||(notevisitor::getSyllabic()== "middle")
            ||(notevisitor::getSyllabic()== "begin"))
        {
            Sguidoelement tag = guidotag::create("lyrics");
            // replaces Spaces in text by '~' to avoid event progression!
            std::string newTxt = notevisitor::getLyricText();
            std::replace( newTxt.begin(), newTxt.end(), ' ', '~');
            if (!(notevisitor::getSyllabic()== "end"))
            {
                // Add a '-' only for Begin/Middle and not at the end
                newTxt.append("-");
            }
            tag->add (guidoparam::create(newTxt, true));
            //tag->add (guidoparam::create(notevisitor::getLyricDy(), false));
            std::string autoPosParam="autopos=\"on\"";
            tag->add(guidoparam::create(autoPosParam, false));
            push (tag);
            
            fHasLyrics = true;
        }
        
        // Alternative 2: Make MIDDLE and BEGIN also a SINGLE
        
        /// Alternative 1: The following code corresponds to time-spanned Lyric definition
        /*if ( (notevisitor::getSyllabic()== "begin") ) {
         std::string tagtxt("lyrics");
         lyricParams = "";
         // replaces Spaces in text by '~' to avoid event progression!
         std::string newTxt = notevisitor::getLyricText();
         std::replace( newTxt.begin(), newTxt.end(), ' ', '~');
         lyricParams += newTxt;
         Sguidoelement tag = guidotag::create(tagtxt);
         push (tag);
         fLyricOpened = fStack.top();
         }
         
         if (notevisitor::getSyllabic()== "middle")
         {
         // Should ONLY update tag parameter here!
         lyricParams += "-";     // space after a word (or syllable) progresses to the following event
         // replaces Spaces in text by '~' to avoid event progression!
         std::string newTxt = notevisitor::getLyricText();
         std::replace( newTxt.begin(), newTxt.end(), ' ', '~');
         lyricParams += newTxt;
         }*/
    }
    
    void xmlpart2guido::checkLyricEnd	 ( const std::vector<S_lyric>& lyrics )
    {
        /*if ( (notevisitor::getSyllabic()== "end") )
         {
         lyricParams += "-";     // space after a word (or syllable) progresses to the following event
         // replaces Spaces in text by '~' to avoid event progression!
         std::string newTxt = notevisitor::getLyricText();
         std::replace( newTxt.begin(), newTxt.end(), ' ', '~');
         lyricParams += newTxt;
         
         fLyricOpened->add((guidoparam::create(lyricParams, true)));
         fLyricOpened->add(guidoparam::create(-3, false));
         
         pop();
         fLyricOpened = NULL;
         lyricParams="";
         }*/
        
        float minDur4Space = 1;
        size_t minStringSize4Space = 2;
        
        float thisDuration = (float)(getDuration()) / (float)(fCurrentDivision*1);
        
        if (notevisitor::getSyllabic()== "single")
        {
            pop();
			
            if ( fLyricsManualSpacing && (thisDuration< minDur4Space) && (notevisitor::getLyricText().size() > minStringSize4Space))
            {
                Sguidoelement tag = guidotag::create("space");
                size_t additionalSpace = notevisitor::getLyricText().size() - minStringSize4Space;
                tag->add (guidoparam::create(8 + additionalSpace, false));
                add(tag);
            }
            
        }
        else if ((notevisitor::getSyllabic()== "end")
                 ||(notevisitor::getSyllabic()== "middle")
                 ||(notevisitor::getSyllabic()== "begin"))
        {
            pop();
			
            if ( fLyricsManualSpacing && (thisDuration< minDur4Space) && (notevisitor::getLyricText().size() > minStringSize4Space))
            {
                Sguidoelement tag = guidotag::create("space");
                size_t lyricStringSize = 0;
                if (notevisitor::getSyllabic()=="end")
                    lyricStringSize = notevisitor::getLyricText().size();
                else
                    lyricStringSize = notevisitor::getLyricText().size() +1;
                
                long additionalSpace =  lyricStringSize - minStringSize4Space;
                tag->add (guidoparam::create(8 + additionalSpace, false));
                add(tag);
            }
            
        }
    }
    
    vector<S_lyric>::const_iterator xmlpart2guido::findValue ( const std::vector<S_lyric>& lyrics,
                                                              const string& val ) const
    {
        std::vector<S_lyric>::const_iterator i;
        for (i = lyrics.begin(); i != lyrics.end(); i++) {
            if ((*i)->getValue() == val) break;
        }
        return i;
    }
    
    
    //______________________________________________________________________________
    void xmlpart2guido::checkStem ( const S_stem& stem )
    {
        // Should regenerate if in a Cue tag
        Sguidoelement tag;
        if (stem) {
            if (stem->getValue() == "down") {
                if (fCurrentStemDirection != kStemDown || fInCue) {
                    tag = guidotag::create("stemsDown");
                    fCurrentStemDirection = kStemDown;
                }
            }
            else if (stem->getValue() == "up") {
                if (fCurrentStemDirection != kStemUp || fInCue) {
                    tag = guidotag::create("stemsUp");
                    fCurrentStemDirection = kStemUp;
                }
            }
            else if (stem->getValue() == "none") {
                if (fCurrentStemDirection != kStemNone || fInCue) {
                    tag = guidotag::create("stemsOff");
                    fCurrentStemDirection = kStemNone;
                }
            }
            else if (stem->getValue() == "double") {
            }
        }
        else if (fCurrentStemDirection != kStemUndefined) {
            tag = guidotag::create("stemsAuto");
            fCurrentStemDirection = kStemUndefined;
        }
        if (tag) add(tag);
    }
    
    //______________________________________________________________________________
    int xmlpart2guido::checkArticulation ( const notevisitor& note )
    {
        int n = 0;
        Sguidoelement tag;
        if (note.fAccent) {
            tag = guidotag::create("accent");
            if (fGeneratePositions) xml2guidovisitor::addPlacement(note.fAccent, tag);
            push(tag);
            n++;
        }
        if (note.fStrongAccent) {
            tag = guidotag::create("marcato");
            if (fGeneratePositions) xml2guidovisitor::addPlacement(note.fStrongAccent, tag);
            push(tag);
            n++;
        }
        if (note.fStaccato) {
            tag = guidotag::create("stacc");
            if (fGeneratePositions) xml2guidovisitor::addPlacement(note.fStaccato, tag);
            push(tag);
            n++;
        }
        if (note.fTenuto) {
            tag = guidotag::create("ten");
            if (fGeneratePositions) xml2guidovisitor::addPlacement(note.fTenuto, tag);
            push(tag);
            n++;
        }
        
        return n;
    }
    
    //---------------------
    void xmlpart2guido::checkWavyTrillBegin	 ( const notevisitor& nv )
    {
        if (nv.fTrill)
        {
            Sguidoelement tag;
            tag = guidotag::create("trill");
            
            // parse accidental-mark as it'll be used by all
            string accidentalMark="";
            if (nv.fAccidentalMark)
                accidentalMark = nv.fAccidentalMark->getValue();
            string guidoAccident="";
            if (accidentalMark=="sharp")
            {
                guidoAccident = "#";
            }
            if (accidentalMark=="flat")
            {
                guidoAccident = "&";
            }
            
            // Generate ornament note ONLY if there is an AccidentalMark in MusicXML
            if (nv.fAccidentalMark) {
                string stepString = nv.getStep();
                int stepi = ( stepString == "B" ? notevisitor::C :  step2i(stepString) + 1);
                string newName = i2step(stepi);
                if (!newName.empty()) newName[0]=tolower(newName[0]);

                stringstream s;
                s << newName << guidoAccident;
                
                tag->add (guidoparam::create(s.str(), true));
            }
            
            // Check for continuous Trills
            if (nv.getWavylines().size()>0)
            {
                std::vector<S_wavy_line>::const_iterator i;
                for (i = nv.getWavylines().begin(); i != nv.getWavylines().end(); i++) {
                    if ((*i)->getAttributeValue("type") == "start") {
                        fWavyTrillOpened = true;
                    }
                }
            }
            else {
                // if there is no wavy-line, then the Trill should be closed in this scope!
                fSingleScopeTrill = true;
            }
            push(tag);
        }
    }
    
    void xmlpart2guido::checkWavyTrillEnd	 ( const notevisitor& nv )
    {
		if (nv.fTrill) pop();

        if (nv.getWavylines().size() > 0)
        {
            std::vector<S_wavy_line>::const_iterator i;
            for (i = nv.getWavylines().begin(); i != nv.getWavylines().end(); i++) {
                if ((*i)->getAttributeValue("type") == "stop") {
                    fWavyTrillOpened = false;
                }
            }
        }
        else
            if (fSingleScopeTrill) {
                fSingleScopeTrill = false;
            }
    }
    
    
    //---------------------
    int xmlpart2guido::checkChordOrnaments(const notevisitor& note)
    {
        // We can visit mordent, tremolo, trill, turn, inverted-turn, vertical-turn, and accidental-mark
        // See http://usermanuals.musicxml.com/MusicXML/Content/EL-MusicXML-ornaments.htm
        /// NOTE: GUIDO 1.6.4+ was upgraded, removing Chord construction by default for ornaments (and thus allowing using ornaments on chords!)
        ///         This implementation complies to GuidoLib 1.6.4 onwards. -- Arshia Cont
        
        int n = 0;
        Sguidoelement tag;
        
        // Inversed mordent in Guido is mordent with inversed chord structure.
        if (note.fMordent || note.fInvertedMordent) {
            tag = guidotag::create("mord");
            
            // if mordent, then we need to add "inverted" parameter with the "correct" pitch!
            if (note.fMordent) {
                // Calculating the ornament note to add requires knowing the KEY! CHEAT: put "-"
                string stepString = note.getStep();
                 int stepi = ( step2i(stepString) == 0 ? notevisitor::B :  step2i(stepString) - 1);
                 string newName = i2step(stepi);
                 
                if (!newName.empty()) {
                    newName[0]=tolower(newName[0]);
                }
                tag->add (guidoparam::create("-", true));
                tag->add (guidoparam::create("inverted", true));
            }
            
            //if (fGeneratePositions) xml2guidovisitor::addPlacement(note.fMordent, tag);
            push(tag);
            n++;
        }
        
        if (note.fTurn || note.fInvertedTurn) {
            tag = guidotag::create("turn");
            
            if (note.fInvertedTurn) {
                tag->add (guidoparam::create("type=\"inverted\"", false));
            }
            
            //if (fGeneratePositions) xml2guidovisitor::addPlacement(note.fInvertedMordent, tag);
            push(tag);
            n++;
        }
        
        if (note.fTremolo) {
            std::string tremType = note.fTremolo->getAttributeValue("type");
            if (tremType == "single") {
                tag = guidotag::create("trem");
                // trem style is the number int value
                int numDashes = int(*(note.fTremolo));
                stringstream ss;
                ss << "style=\"";
                for (int id=0; id<numDashes;id++) {
                    ss << "/";
                }
                ss << "\"";
                tag->add (guidoparam::create(ss.str(), false));
                
                push(tag);
                n++;
            }

        }
        
        if (note.fTrill) {
            tag = guidotag::create("trill");
            
        }
        
        return n;
    }
    
    //______________________________________________________________________________
    vector<Sxmlelement> xmlpart2guido::getChord ( const S_note& elt )
    {
        vector<Sxmlelement> v;
        ctree<xmlelement>::iterator nextnote = find(fCurrentMeasure->begin(), fCurrentMeasure->end(), elt);
        if (nextnote != fCurrentMeasure->end()) nextnote++;	// advance one step
        while (nextnote != fCurrentMeasure->end()) {
            // looking for the next note on the target voice
            if ((nextnote->getType() == k_note) && (nextnote->getIntValue(k_voice,0) == fTargetVoice)) {
                ctree<xmlelement>::iterator iter;			// and when there is one
                iter = nextnote->find(k_chord);
                if (iter != nextnote->end())
                    v.push_back(*nextnote);
                else break;
            }
            nextnote++;
        }
        return v;
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::checkCue (const notevisitor& nv)
    {
        if (nv.isCue()) {
            if (!fInCue) {
                fInCue = true;
                Sguidoelement tag = guidotag::create("cue");
                push(tag);
            }
            
            // add up duration in Cue
            if (!nv.inChord())
            {
                rational r(nv.getDuration(), fCurrentDivision*4);
                r.rationalise();
                durationInCue += r;
            }
        }
        else if (fInCue) {
            fInCue = false;
            pop();
            
            durationInCue.rationalise();
            if (durationInCue.getNumerator() > 0) {
                guidonoteduration dur (durationInCue.getNumerator(), durationInCue.getDenominator());
                Sguidoelement note = guidonote::create(fTargetVoice, "empty", 0, dur, "");
                add (note);
                fCurrentVoicePosition += durationInCue;
                fCurrentVoicePosition.rationalise();
            }
            
            durationInCue = 0;
        }
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::checkGrace (const notevisitor& nv)
    {
        if (nv.isGrace()) {
            if (!fInGrace) {
                fInGrace = true;
                Sguidoelement tag = guidotag::create("grace");
                push(tag);
            }
        }
        else if (fInGrace) {
            fInGrace = false;
            pop();
        }
    }
    
    void xmlpart2guido::checkGraceEnd(const notevisitor& nv)
    {
        // End grace BEFORE the next non-grace note to avoid conflict with S_direction
        if (fInGrace)
        {
            ctree<xmlelement>::iterator nextnote = find(fCurrentMeasure->begin(), fCurrentMeasure->end(), nv.getSnote());
            nextnote++;	// advance one step
            while (nextnote != fCurrentMeasure->end()) {
                if ((nextnote->getType() == k_note) && (nextnote->getIntValue(k_voice,0) == fTargetVoice)){
                    ctree<xmlelement>::iterator iter = nextnote->find(k_grace);
                    if (iter != nextnote->end())
                    {
                        // Next note has a grace.. do nothing.. we'll break anyway!
                    }else {
                        // Pop grace before disaster! :)
                        pop();
                        fInGrace = false;
                    }
                    
                    break;
                }
                nextnote++;
            }
        }
    }
    
    //______________________________________________________________________________
    int xmlpart2guido::checkFermata (const notevisitor& nv)
    {
        if (nv.inFermata()) {
            Sguidoelement tag = guidotag::create("fermata");
            push(tag);
            return 1;
        }
        return 0;
    }
    
    //______________________________________________________________________________
    string xmlpart2guido::noteName ( const notevisitor& nv )
    {
        string accident = alter2accident(nv.getAlter());
        string name;
        if (nv.getType() == notevisitor::kRest)
            name="_";
        else {
            name = nv.getStep();
            if (!name.empty()) name[0]=tolower(name[0]);
            else cerr << "warning: empty note name" << endl;
        }
        return name;
    }
    
    //______________________________________________________________________________
    guidonoteduration xmlpart2guido::noteDuration ( const notevisitor& nv )
    {
        guidonoteduration dur(0,0);
        if (nv.getType() == kRest) {
            /* OLD WAY
             rational r(nv.getDuration(), fCurrentDivision*4);
             r.rationalise();
             dur.set (r.getNumerator(), r.getDenominator());
             */
            
            // Do as in Notes but be aware of TYPE = WHOLE for Rests which should be ignored if we are not 4/4 or similar!
            
            if ( (nv.getGraphicType() == "whole") && (true) ) {
                rational r(nv.getDuration(), fCurrentDivision*4);
                r.rationalise();
                dur.set (r.getNumerator(), r.getDenominator());
            } else {
                rational r = NoteType::type2rational(NoteType::xml(nv.getGraphicType()));
                if (r.getNumerator() == 0) // graphic type missing or unknown
                    r.set (nv.getDuration(), fCurrentDivision*4);
                r.rationalise();
                rational tm = nv.getTimeModification();
                r *= tm;
                r.rationalise();
                dur.set (r.getNumerator(), r.getDenominator(), nv.getDots());
            }
        }
        else {
            rational r = NoteType::type2rational(NoteType::xml(nv.getGraphicType()));
            if (r.getNumerator() == 0) // graphic type missing or unknown
                r.set (nv.getDuration(), fCurrentDivision*4);
            r.rationalise();
            rational tm = nv.getTimeModification();
            r *= tm;
            r.rationalise();
            dur.set (r.getNumerator(), r.getDenominator(), nv.getDots());
        }
        return dur;
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::newNote ( const notevisitor& nv, rational posInMeasure )
    {
        //checkTiedBegin (nv.getTied());
        
        int octave = nv.getOctave() - 3;			// octave offset between MusicXML and GUIDO is -3
        string accident = alter2accident(nv.getAlter());
        string name = noteName(nv);
        guidonoteduration dur = noteDuration(nv);
        
        Sguidoelement note = guidonote::create(fTargetVoice, name, octave, dur, accident);
        
        /// Force Accidental if accidental XML tag is present
        bool forcedAccidental = false;
        if (!nv.fCautionary.empty())
        {
            Sguidoelement accForce = guidotag::create("acc");
            push(accForce);
            forcedAccidental = true;
        }
        
        /// Add Note head of X offset for note if necessary
        bool noteFormat = false;
        int measureNum = fCurrentMeasure->getAttributeIntValue("number", 0);
        auto timePos4measure = timePositions.find(measureNum);
        if ( (nv.fNotehead
             || ((timePos4measure != timePositions.end()) ) )             // if we need to infer default-x
            &&  fInGrace==false  )      // FIXME: Workaround for GUID-74
        {
            Sguidoelement noteFormatTag = guidotag::create("noteFormat");
            
            if (nv.fNotehead) {
                std::string noteFormatType = nv.getNoteheadType();
                
                if (noteFormatType.size())
                {
                    stringstream s;
                    s << "\"" << noteFormatType << "\"";
                    noteFormatTag->add (guidoparam::create(s.str(), false));
                    noteFormat = true;
                }
            }
            
            /// check for dx inference from default_x but avoid doing this for Chords as Guido handles this automatically!
            if (timePos4measure != timePositions.end() && (isProcessingChord==false)) {
                auto voiceInTimePosition = timePos4measure->second.find(posInMeasure);
                if (voiceInTimePosition != timePos4measure->second.end()) {
                    auto minXPos = std::min_element(voiceInTimePosition->second.begin(),voiceInTimePosition->second.end() );
                    if (nv.x_default != *minXPos) {
                        int noteDx = ( (nv.x_default - *minXPos)/ 10 ) * 2;   // convert to half spaces
                        
                        stringstream s;
                        s << "dx=" << noteDx ;
                        noteFormatTag->add (guidoparam::create(s.str(), false));
                        noteFormat = true;
                    }
                }
            }
            
            
            if (noteFormat == true)
            {
                push(noteFormatTag);
            }
        }
        
        add (note);
        
        if (noteFormat)
            pop();
        
        if (forcedAccidental)
            pop();
        
        //checkTiedEnd (nv.getTied());
    }
    
    int xmlpart2guido::checkNoteFormatDx	 ( const notevisitor& nv , rational posInMeasure)
    {
        int measureNum = fCurrentMeasure->getAttributeIntValue("number", 0);
        auto timePos4measure = timePositions.find(measureNum);
        
        if ( timePos4measure != timePositions.end())
        {
            Sguidoelement noteFormatTag = guidotag::create("noteFormat");
            
            /// check for dx inference from default_x but avoid doing this for Chords as Guido handles this automatically!
            if (timePos4measure != timePositions.end() && (isProcessingChord==false)) {
                auto voiceInTimePosition = timePos4measure->second.find(posInMeasure);
                if (voiceInTimePosition != timePos4measure->second.end()) {
                    auto minXPos = std::min_element(voiceInTimePosition->second.begin(),voiceInTimePosition->second.end() );
                    if (nv.x_default != *minXPos) {
                        int noteDx = ( (nv.x_default - *minXPos)/ 10 ) * 2;   // convert to half spaces
                        
                        stringstream s;
                        s << "dx=" << noteDx ;
                        noteFormatTag->add (guidoparam::create(s.str(), false));
                        push(noteFormatTag);
                    }else
                        return 0;
                }else
                    return 0;
            }
        }
        
        return 1;
    }
    
    int xmlpart2guido::checkRestFormat	 ( const notevisitor& nv )
    {
        if (nv.getStep().size())
        {
            // Check out clef for position and voice
            std::string thisClef = getClef(fCurrentStaffIndex , fCurrentVoicePosition, fMeasNum);
            float restformatDy = nv.getRestFormatDy(thisClef);
            if (restformatDy!=0.0)
            {
                Sguidoelement restFormatTag = guidotag::create("restFormat");
                stringstream s;
                s << "dy=" << restformatDy;
                restFormatTag->add (guidoparam::create(s.str(), false));
                push(restFormatTag);
                return 1;
            }
        }
        
        return 0;
    }
    
    std::string xmlpart2guido::getClef(int staffIndex, rational pos, int measureNum) {
        //     std::multimap<int,  std::pair< int, std::pair< rational, string > > > staffClefMap;
        std::string thisClef = "g";
        if (staffClefMap.size()>0) {
            auto staffRange = staffClefMap.equal_range(staffIndex);

            for (auto i = staffRange.first ; i != staffRange.second; i++ )
            {
                // Get the measure number
                if (((i->second).first <= measureNum) && ((i->second).second.first <= pos )){
                    thisClef = (i->second).second.second;
                }else
                    break;
            }
        }
        return thisClef;
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::visitEnd ( S_note& elt )
    {
        notevisitor::visitEnd ( elt );
        
        if (inChord()) return;					// chord notes have already been handled
        
        isProcessingChord = false;
        
        rational thisNoteHeadPosition = fCurrentVoicePosition;
        
        int pendingPops = 0;
        
        bool scanVoice = (notevisitor::getVoice() == fTargetVoice);
        if (!isGrace() ) {
            //////// Track all voice default-x parameters, as positions in measures

            if (true) {     // had fNotesOnly
                int measureNum = fCurrentMeasure->getAttributeIntValue("number", 0);
                auto timePos4measure = timePositions.find(measureNum);
                if (notevisitor::x_default != -1) {
                    if ( timePos4measure !=  timePositions.end())
                    {
                        
                        if (timePos4measure->second.find(fCurrentVoicePosition) != timePos4measure->second.end())
                        {
                            // Exists.. push it to vector
                            timePos4measure->second.find(fCurrentVoicePosition)->second.push_back(notevisitor::x_default);
                        }else {
                            // Doesn't exist.. insert with this element's x_default
                            timePos4measure->second.insert(std::pair<rational, std::vector<int> >
                                                           (fCurrentVoicePosition, std::vector<int>(1, notevisitor::x_default)) );
                        }
                    }else {
                        std::map<rational, std::vector<int> > inner;
                        inner.insert(std::make_pair(fCurrentVoicePosition, std::vector<int>(1, notevisitor::x_default)));
                        timePositions.insert(std::make_pair(measureNum, inner));
                    }
                }
            }
            
            moveMeasureTime (getDuration(), scanVoice);
            checkDelayed (getDuration());		// check for delayed elements (directions with offset)
        }
        if (!scanVoice) return;
        
        checkStaff(notevisitor::getStaff());
        
        checkVoiceTime (fCurrentMeasurePosition, fCurrentVoicePosition);
        
        checkCue(*this);
        if (notevisitor::getType() != notevisitor::kRest)
            checkStem (notevisitor::fStem);
        
        checkGrace(*this);
        checkSlurBegin (notevisitor::getSlur());
        checkBeamBegin (notevisitor::getBeam());
        checkTupletBegin(notevisitor::getTuplet(), *this, elt);
        checkLyricBegin (notevisitor::getLyric());
        checkWavyTrillBegin(*this);
        
        pendingPops += checkFermata(*this);
        pendingPops += checkArticulation(*this);
        pendingPops += checkDynamics(thisNoteHeadPosition);
        
        int chordOrnaments = checkChordOrnaments(*this);
        pendingPops += chordOrnaments;
        
        if (notevisitor::getType()==kRest)
            pendingPops += checkRestFormat(*this);
        
        // Check for TIES before creating the chord sequence. \tieBeing and \tieEnd should live OUTSIDE chord sequence in Guido
        checkTiedBegin((*this).getTied());
        
        vector<Sxmlelement> chord = getChord(elt);
        if (chord.size())
        {
            Sguidoelement chord = guidochord::create();
            //// FIXME: The following line removed since it introduced bad behavior on REINE de La NUIT Piano accompaniment!
            //pendingPops += checkNoteFormatDx(*this, thisNoteHeadPosition);
            push (chord);
            pendingPops++;
            isProcessingChord = true;
        }
        
        newNote (*this, thisNoteHeadPosition);
        // Add chord notes (in case of a real chord)
        for (vector<Sxmlelement>::const_iterator iter = chord.begin(); iter != chord.end(); iter++) {
            isProcessingChord = true;
            notevisitor nv;
            xml_tree_browser browser(&nv);
            Sxmlelement note = *iter;
            browser.browse(*note);
            checkStaff(nv.getStaff());
            newNote (nv, thisNoteHeadPosition);
        }
        
        checkTiedEnd((*this).getTied());
        isProcessingChord = false;
        
        while (pendingPops--) pop();
        
        checkWavyTrillEnd(*this);
        checkLyricEnd (notevisitor::getLyric());

        checkTupletEnd(notevisitor::getTuplet());
        checkBeamEnd (notevisitor::getBeam());
        checkSlurEnd (notevisitor::getSlur());
        if (notevisitor::fBreathMark) {
            Sguidoelement tag = guidotag::create("breathMark");
            add(tag);
        }
        
        
        checkGraceEnd(*this);   // This will end GUIDO Grace tag, before any collision with a S_direction
        
        if (fBeamStack.size()==0)
        {
            // this is will close any ongoing Guido TEXT tag once a sequence is embedded
            // In case of ongoing \Beam, do it after the \beam is closed! (Potential Guido parser issue)
            checkTextEnd();
        }
        
        
        fMeasureEmpty = false;
    }
    
    //______________________________________________________________________________
    // time management
    //______________________________________________________________________________
    void xmlpart2guido::visitStart ( S_divisions& elt )
    {
        fCurrentDivision = (long)(*elt);
    }
    
}
