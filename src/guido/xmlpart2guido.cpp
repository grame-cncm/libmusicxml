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
#include "metronomevisitor.h"
#include "xmlpart2guido.h"
#include "xml_tree_browser.h"

using namespace std;

bool checkTempoMarkup(std::string input);

namespace MusicXML2
{
    
    //______________________________________________________________________________
    xmlpart2guido::xmlpart2guido(bool generateComments, bool generateStem, bool generateBar, int startMeasure, int endMeasure, int endMeasureOffset) :
    fGenerateComments(generateComments), //fGenerateStem(generateStem),
    fGenerateBars(generateBar),
    fNotesOnly(false), fCurrentStaffIndex(0), fCurrentStaff(0),
    fTargetStaff(0), fTargetVoice(0),
    fStartMeasure(startMeasure), fEndMeasure(endMeasure), fEndMeasureOffset(endMeasureOffset)
    {
        fGeneratePositions = true;
        fGenerateAutoMeasureNum = true;
        xmlpart2guido::reset();
        fHasLyrics = false;
        fNonStandardNoteHead = false;
        fLyricsManualSpacing = false;
        fTupletOpen = 0;
        fTremoloInProgress = false;
        fCurrentOctavaShift = 0;
        fShouldStopOctava = false;
        staffClefMap.clear();
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::reset ()
    {
        guidonotestatus::resetall();
        fMeasNum = 0;
        fInCue = fInGrace = fInhibitNextBar = fPendingBar = fDoubleBar
        = fBeamOpened = fBeamGrouping = fSkipDirection = fWavyTrillOpened = fSingleScopeTrill = fNonStandardNoteHead = false;
        fCrescPending = fDiminPending = 0;
        fCurrentStemDirection = kStemUndefined;
        fCurrentDivision = 1;
        fCurrentOffset = 0;
        fPendingPops = 0;
        fLyricsManualSpacing = false;
        fTextTagOpen = 0;
        fTupletOpen = 0;
        fTremoloInProgress = false;
        fCurrentOctavaShift = 0;
        fShouldStopOctava = false;
        fCurrentScorePosition.set(0, 1);
        measurePositionMap.clear();
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
        fTupletOpen = 0;
        fTremoloInProgress = false;
        fCurrentOctavaShift = 0;
        fShouldStopOctava = false;
        start (seq);
        processedDirections.clear();
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
        if (offset > 0) {
            delayedElement de;
            de.delay = offset;
            de.element = elt;
            fDelayed.push_back(de);
        }
    }
    
    //________________________________________________________________________
    // checks ready elements in the list of delayed elements
    // 'time' is the time elapsed since the last check, it is expressed in
    // <division> time units
    void xmlpart2guido::checkDelayed (long time, bool before)
    {
        vector<delayedElement>::iterator it = fDelayed.begin();
        while (it!=fDelayed.end()) {
            if (before) {
                it->delay -= time;
            }
            if (before && it->element->isEndTag()) {
                it++;
                continue;
            }
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
        }
    }

/// Check if measure number is in range for Partial Rendering
bool xmlpart2guido::checkMeasureRange() {
    if (!fCurrentMeasure) return true;
    int currentXmlMeasure = atoi(fCurrentMeasure->getAttributeValue("number").c_str());
    //cerr<<"\t <<< checkMeasureRange "<< currentXmlMeasure<< "|"<<fStartMeasure<<" "<<fEndMeasure<<endl;
    if ((currentXmlMeasure < fStartMeasure)) return false;
     
    if ((fEndMeasure>0) && (currentXmlMeasure > fEndMeasure+fEndMeasureOffset)) return false;
    
    return true;
}
    
    //______________________________________________________________________________
    void xmlpart2guido::moveMeasureTime (long duration, bool moveVoiceToo)
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

void xmlpart2guido::checkOctavaBegin() {
    // Generate Octave-shifts on current measure and current time
    std::string currMeasure = fCurrentMeasure->getAttributeValue("number");
    bool found = octavas.count(currMeasure);
    if (found) {
        for (auto o = octavas[currMeasure].cbegin(); o != octavas[currMeasure].cend(); ) {
            // o.second points to pair< time, type >
            if ( o->first < fCurrentVoicePosition && o->second != 0) {
                parseOctaveShift(o->second);
                octavas[currMeasure].erase(o++);
            }else {
                ++o;
            }
        }
    }
}

void xmlpart2guido::checkOctavaEnd() {
    std::string currMeasure = fCurrentMeasure->getAttributeValue("number");
    bool found = octavas.count(currMeasure);
    if (found) {
        for (auto o = octavas[currMeasure].cbegin(); o != octavas[currMeasure].cend(); ) {
            // o.second points to pair< time, type >
            if ( o->first <= fCurrentVoicePosition && o->second == 0) {
                parseOctaveShift(o->second);
                octavas[currMeasure].erase(o++);
            }else {
                ++o;
            }
        }
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
            if (fCurrentOctavaShift) {
                checkOctavaEnd();
            }else
                checkOctavaBegin();
        }
        // difference can be negative due to S_backup and it is normal!
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
        && (elt->getIntValue(k_staff, 1) == fTargetStaff);
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
            if (fCurrentOctavaShift) {
                checkOctavaEnd();
            }else
                checkOctavaBegin();
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

    void xmlpart2guido::visitEnd ( S_part& elt )
    {
        fCurrentScorePosition += fCurrentMeasureLength;
        fCurrentScorePosition.rationalise();
    }
    
    
    //______________________________________________________________________________
    void xmlpart2guido::visitStart ( S_measure& elt )
    {
        fCurrentMeasure = elt;
        fCurrentScorePosition += fCurrentMeasureLength;
        fCurrentScorePosition.rationalise();
                
        std::string measNum = elt->getAttributeValue("number");
        try {
            fMeasNum = std::stoi(measNum);
        } catch(...) {
            fMeasNum++;
        }
        
        measurePositionMap[fCurrentScorePosition.toDouble()] = fMeasNum ;
        
        bool isFirstPartialMeasure = (fStartMeasure>0) && (fMeasNum == fStartMeasure);
        
        if ( isFirstPartialMeasure ) {
            fStartPosition = fCurrentScorePosition;

            if (!fNotesOnly) {
                if (lastMeter) {
                    add(lastMeter);
                }
                if (lastKey) {
                    add(lastKey);
                }
                    
                // Add last clef
                std::string lastClef = getClef(fTargetStaff , fCurrentVoicePosition, fMeasNum);
                if (!lastClef.empty()) {
                    Sguidoelement tag = guidotag::create("clef");
                    tag->add (guidoparam::create(lastClef));
                    add(tag);
                }
            }
        }

        const string& implicit = elt->getAttributeValue ("implicit");
        if (implicit == "yes") fPendingBar = false;
        if (fPendingBar || fDoubleBar) {
            // before adding a bar, we need to check that there are no repeat begin at this location
            ctree<xmlelement>::iterator repeat = elt->find(k_repeat);
            if ((repeat == elt->end()) || (repeat->getAttributeValue("direction") != "forward")) {
                checkStaff (fTargetStaff);
                
                Sguidoelement tag;
                
                // Check bar-style if doubleBar
                if (fDoubleBar)
                    tag = guidotag::create("doubleBar");
                else {
                    tag = guidotag::create("bar");
                    stringstream parameters;
                    std::string measNum = elt->getAttributeValue("number");
                    if (!measNum.empty())
                        parameters << "measNum="<< measNum;
                    tag->add(guidoparam::create(parameters.str(), false));
                }
                
                if (!isFirstPartialMeasure)
                    add (tag);
            }
        }else if (fCurrentScorePosition.toFloat() != 0.0) {
            // Create a HIDDEN Bar in case of fPendingBar equal to false.
            // This is the case for "bar-style" equal to "none" or "implicit" measures
            if(!fInhibitNextBar){
                Sguidoelement tag = guidotag::create("bar");
                stringstream parameters;
                std::string measNum = elt->getAttributeValue("number");
                if (!measNum.empty()) {
                    parameters << "measNum="<< measNum<<", ";
                }
                parameters << "hidden=\"true\"";
                tag->add(guidoparam::create(parameters.str(), false));
                add(tag);
            }
        }

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
        
        // Take care of StaffOn StaffOff
        // REMOVED: StaffOn / StaffOff in GuidoLib does not behave well in presence of multi-staff Parts
//        auto sStaffDetails = elt->find(k_staff_details);
//        if (sStaffDetails != elt->end()) {
//            if (sStaffDetails->getAttributeIntValue("number", 0) == fTargetStaff) {
//                std::string print = sStaffDetails->getAttributeValue("print-object");
//                if (print=="no") {
//                    Sguidoelement tag = guidotag::create("staffOff");
//                    add(tag);
//                }else if (print=="yes") {
//                    Sguidoelement tag = guidotag::create("staffOn");
//                    add(tag);
//                }
//            }
//        }
        
        // Take care of staff-distance
        auto sLayout = elt->find(k_staff_layout);
        while (sLayout != elt->end() )
        {
            if (sLayout->getAttributeIntValue("number", 0) == fTargetStaff) {
                int xmlStaffDistance = sLayout->getIntValue(k_staff_distance, 0);
                Sguidoelement tag2 = guidotag::create("staffFormat");
                float HalfSpaceDistance = ((float)(xmlStaffDistance) / 10) * 2 ;
                stringstream s;
                s << "distance="<< HalfSpaceDistance;
                tag2->add (guidoparam::create(s.str().c_str(), false));
                add (tag2);
            }
            
            sLayout = elt->find(k_staff_layout, sLayout++);
        }
        
        /// Report ending position for partial parsing
        if ((fEndMeasure>0)) {
            if (fMeasNum == fEndMeasure+1) {
                fEndPosition = fCurrentScorePosition;
            }
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
        // Directions can have voice (not encoded as of Finale v27). Check:
        int voiceNumber = elt->getIntValue(k_voice, -1); // set default to -1 to skip if not available
        if (voiceNumber >0 && voiceNumber != fTargetVoice) {
            fSkipDirection = true;
        }
        // Skip if direction doesn't belong to target staff
        if ((elt->getIntValue(k_staff, 1) != fTargetStaff) ) { //fNotesOnly ||
            fSkipDirection = true;
        }
        // Skip if current voice has scanned in this measure
        if (fCurrentVoicePosition > fCurrentMeasurePosition) {
            fSkipDirection = true;
        }
        // Check if Direction has been processed and proceed
        if (!fSkipDirection) {
            int lineNumber = elt->getInputLineNumber();
            auto it = std::find(processedDirections.begin(), processedDirections.end(), lineNumber);
            if (it == processedDirections.end()) {
                processedDirections.push_back(lineNumber);
            }else {
                fSkipDirection = true;
            }
        }
        
        if (!fSkipDirection) {
            fCurrentOffset = elt->getLongValue(k_offset, 0);
        }
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::visitEnd ( S_direction& elt )
    {
        // !IMPORTANT: Avoid using default-x directly and use timePositions methods instead for all horizontal positioning of Directions, since it is relative to the beginning of a measure.
        
        if (fSkipDirection) {
            // set back to false for next elements!
            fSkipDirection = false;
            return;
        }
        
        /// Skip already visited Direction in case of grace notes (GUID-153)
        if ((!fDirectionEraserStack.empty())) {
            if (fDirectionEraserStack.front() == elt->getInputLineNumber()) {
                fDirectionEraserStack.pop();
                return;
            }
        }
        
        int directionStaff = 0;
        if (elt->find(k_staff) != elt->end()) {
            checkStaff(elt->getIntValue(k_staff, 1));
            directionStaff = elt->getIntValue(k_staff, 0);
        }
        
        // Browse into all S_direction_type elements and parse, by preserving ordering AND grouped direction positions (if missing in proceedings calls)
        ctree<xmlelement>::literator iter = elt->lbegin();
        
        /// IMPORTANT: In case of "metronome", there's a coupling of WORDS and METRONOME which leads to ONE guido element. Take this into account.
        bool generateTempo = false;     // Possible Composite generation of Metronome
        bool generateCompositeDynamic = false;      // true if Dynamic and Word are present all together, resulting into one composite Guido Tag
        string tempoWording;
        string tempoTextParameters;
        ctree<xmlelement>::iterator ito;
        if (elt->find(k_metronome) != elt->end())
        {
            generateTempo = true;   // this will allow grouping of S_Word and S_Metronome into one tag
        }
        /// An S_Word with Sound element in the direction that has a Tempo attribute is NOT a text but a tempo markup
        if (elt->find(k_sound) != elt->end())
        {
            // If "sound" attribute exists then we are in generateTempo
            if (elt->find(k_sound)->getAttributeIntValue("tempo", 0) > 0) {
                generateTempo = true;
            }
        }
        
        /* If we have a combination of k_words and k_dynamics, then the ORDER of appearance is important for Parsing:
         If text is BEFORE dynamics, then impose textformat="rt", dx=-2
         If text order is AFTER dynamics, then impose textformat="lt", dx=2
         */
        Sguidoelement tag;
        bool generateAfter= false;  // hold on to generation until everything is visited
        /// Check if there's a combination of k_words and k_dynamics
        if ((elt->find(k_words) != elt->end()) && (elt->find(k_dynamics) != elt->end()) ) {
            generateCompositeDynamic = true;
            // find k_dynamics first and search for k_words after.
            ito = elt->find(k_dynamics);
            ito++;
            if (elt->find(k_words, ito) != elt->end()) {
                // then there is a WORD after Dynamics
                generateAfter = true;
            }
        }
        
        float commonDy = 0.0;   // This is the inherited group Dy
        string wordParameterBuffer = "";    // used if generateCompositeDynamic
        
        auto branches = elt->elements();
        
        for (iter = elt->lbegin(); iter != elt->lend(); iter++) {
            // S_Direction can accept direction_type, offset, footnote, level, voice, staff
                        
            if ((*iter)->getType() == k_direction_type) {
                
                string font_size;
                
                ctree<xmlelement>::literator directionTypeElements;
                for (directionTypeElements = (*iter)->lbegin(); directionTypeElements != (*iter)->lend(); directionTypeElements++) {
                    int elementType = (*directionTypeElements)->getType();
                    auto element = (*directionTypeElements);
                    
                    switch (elementType) {
                        case k_octave_shift: {
                            //parseOctaveShift(element, directionStaff);
                            // IMPORTANT: DO NOT parse Octave-Shift during part visits! Now using a pre-calculated map.
                        }
                            break;
                        case k_pedal:
                        {
                            bool isPedalChange = false;
                            // default-y for pedal is from the top line of the staff in XML, in Guido dy=0 is the C below the lowest line.
                            std::string pedalType = element->getAttributeValue("type");
                            if ( (pedalType== "start") || (pedalType == "sostenuto")) {
                                tag = guidotag::create("pedalOn");
                                fPreviousPedalYPos = xml2guidovisitor::getYposition(element, 14.0, true);
                                
                                xml2guidovisitor::addPosY(element, tag, 14.0, 1.0);
                            }else
                                if ( (pedalType== "stop") || (pedalType == "discontinue")) {
                                    tag = guidotag::create("pedalOff");
                                    
                                    if (element->getAttributeFloatValue("default-y", 0.0) != 0.0) {
                                        xml2guidovisitor::addPosY(element, tag, 14.0, 1.0);
                                    } else {
                                        xml2guidovisitor::addPosY(element, tag, fPreviousPedalYPos, 1.0);
                                    }
                                }
                                else if ( (pedalType== "change")) {
                                    tag = guidotag::create("pedalOff");
                                    isPedalChange = true;
                                }else {
                                    continue;
                                }
                            
                            // Try to infer default-x position if available
                            stringstream s;
                            rational offset(fCurrentOffset, fCurrentDivision*4);
                            float dx = timePositions.getDxForElement(element, fCurrentVoicePosition.toDouble(), fCurrentMeasure->getAttributeValue("number"), 0, directionStaff, offset.toDouble());
                            if (dx==-999) {
                                return; // Return if no corresponding default-x event is found
                            }else {
                                dx-=2; // -2 offset estimating font width
                            }
                            
                            if (pedalType == "change") {
                                dx-=2; // to allow two tags
                                if (fPreviousPedalYPos) {
                                    s << "dy=" << fPreviousPedalYPos << "hs, dx="<< dx<<"hs"; // dx was -2
                                }
                            }else {
                                s << "dx="<<dx;
                            }

                            tag->add (guidoparam::create(s.str(), false));
                            
                            if (fCurrentOffset > 0)
                                addDelayed(tag, fCurrentOffset);
                            else {
                                add(tag);
                            }
                            
                            if (isPedalChange) {
                                tag = guidotag::create("pedalOn");
                                if (fPreviousPedalYPos) {
                                    stringstream s;
                                    rational offset(fCurrentOffset, fCurrentDivision*4);
                                    float dx = timePositions.getDxForElement(element, fCurrentVoicePosition.toDouble(), fCurrentMeasure->getAttributeValue("number"), 0, directionStaff, offset.toDouble());
                                    if (dx==-999) {
                                        return;
                                    }
                                    s << "dy=" << fPreviousPedalYPos << "hs, dx="<< dx <<"hs";
                                    tag->add (guidoparam::create(s.str(), false));
                                }
                                if (fCurrentOffset)
                                    addDelayed(tag, fCurrentOffset);
                                else {
                                    add(tag);
                                }
                            }
                        }
                            break;
                        case k_words:
                        {
                            /// GUID-147: Detect Tempo Markups using specific substrings such as "Andante" etc.
                            // Candidate for tempo Markup: default-y>10 and font-weight="bold" and font-size > 12.0
                            if ( (element->getAttributeValue("font-weight")=="bold")
                                && (element->getAttributeIntValue("default-y", 0)>10)
                                && (element->getAttributeFloatValue("font-size", 0.0) >= 12.0) ) {
                                generateTempo = true;
                            }
                            
                            std::stringstream wordParameters;
                            std::stringstream parameters;
                            
                            if (generateTempo) {
                                tempoWording += element->getValue();
                            }
                            
                            string wordPrefix="";
                            // in case of composite Dynamics, determine whether text is Before or After
                            if (generateCompositeDynamic) {
                                auto itol = std::find(branches.begin(), branches.end(), *iter);
                                if (itol != branches.end()) {
                                    // found current element!
                                    // check if there is a dynamic after, if yes then this word is "before"!
                                    wordPrefix="after=";
                                    for (auto itol2 = itol; itol2 != branches.end(); itol2++) {
                                        if ((*itol2)->find(k_dynamics) != (*itol2)->end()) {
                                            wordPrefix="before=";
                                        }
                                    }
                                    
                                }else {
                                    cerr << "UNABLE to find current element in S_direction. PLEASE REPORT!"<<endl;
                                }
                            }
                            
//                            string font_family = element->getAttributeValue("font-family");
//                            if (font_family.size())
//                                parameters << ",font=\""+font_family+"\"";
                            
                            string thisFontSize = element->getAttributeValue("font-size");
                            // for composite Words, we retain only the largest font-size
                            if (!font_size.empty()) {
                                float lastFS = std::stof(font_size);
                                float thisFS = std::stof(thisFontSize);
                                if (thisFS>lastFS) {
                                    font_size = thisFontSize;
                                }
                            }
                            string font_weight = element->getAttributeValue("font-weight");
                            string font_style = element->getAttributeValue("font-style");
                            
                            if (font_size.size())
                                parameters << ",fsize="+font_size+"pt";
                            
                            // Add font styles
                            string fattrib;
                            if (font_weight=="bold")
                                fattrib +="b";
                            if (font_style=="italic")
                                fattrib +="i";
                            if (fattrib.size())
                                parameters << ",fattrib=\""+fattrib+"\"";
                            
                            if (generateTempo) {
                                // Convert dy to Guido Tempo Tag origin which is +4hs from top of the staff
                                float tempoDy = xml2guidovisitor::getYposition(element, -4, true);
                                if (tempoDy > commonDy) {
                                    commonDy = tempoDy;
                                }
                                parameters << ", dy="<<commonDy<<"hs";
                                
                                // Determine horizontal position, if either offset, default-x or relative-x are present.
                                float default_x = element->getAttributeFloatValue("default-x", 0.);
                                float rel_x = element->getAttributeFloatValue("relative-x", 0.);
                                if ( (fCurrentOffset != 0)||(default_x != 0.0)||(rel_x != 0.0)) {
                                    // For Tempo, we should always search from the BEGINNING of measure (hence position = 0.0)
                                    rational offset(fCurrentOffset, fCurrentDivision*4);
                                    float wordDx = timePositions.getDxRelativeToMeasureForElement(element, fCurrentMeasure->getAttributeValue("number"), 0, offset.toDouble());
                                    if (wordDx != -999 && wordDx != 0) {
                                        parameters << ", dx=" << wordDx;
                                    }
                                }
                                tempoTextParameters = parameters.str();
                                break;
                            }
                            
                            wordParameters << wordPrefix <<"\"" << element->getValue() << "\""<< parameters.str();
                            
                            /// Take into account group positioning
                            float posy = xml2guidovisitor::getYposition(element, 0, true);
                            if (posy != 0.0) {
                                // then apply and save
                                commonDy += xml2guidovisitor::getYposition(element, 11.0, true);  // Should this be additive?
                            }
                            
                            if (generateCompositeDynamic) {
                                if (tag) {
                                    // This should happen when WORD is After Dynamic
                                    tag->add (guidoparam::create(wordParameters.str(), false));
                                }else {
                                    wordParameterBuffer= wordParameters.str();
                                }
                            }else {
                                tag = guidotag::create("text");
                                
                                rational offset(fCurrentOffset, fCurrentDivision*4);
                                float wordDx = timePositions.getDxForElement(element, fCurrentVoicePosition.toDouble(), fCurrentMeasure->getAttributeValue("number"), 0, directionStaff, offset.toDouble());
                                if (wordDx != -999 && wordDx != 0) {
                                    wordParameters << ", dx=" << wordDx;
                                }

                                tag->add (guidoparam::create(wordParameters.str(), false));
                                // apply inherited Y-position
                                if (commonDy != 0.0) {
                                    stringstream s;
                                    s << "dy=" << commonDy << "hs";
                                    tag->add (guidoparam::create(s.str(), false));
                                }
                                
                                push(tag);
                                fTextTagOpen++;
                            }
                            
                            break;
                        }
                            
                        case k_dynamics:
                        {
                            ctree<xmlelement>::literator iter2;
                            float dynamicsDx = 0.0;
                            /// Take into account group positioning
                            float posy = xml2guidovisitor::getYposition(element, 0, true);
                            if (posy != 0.0) {
                                // then apply and save
                                commonDy += xml2guidovisitor::getYposition(element, 13, true);  // Should this be additive?
                            }
                            for (iter2 = element->lbegin(); iter2 != element->lend(); iter2++) {
                                if ((*iter2)->getType() != k_other_dynamics) {
                                    tag = guidotag::create("intens");
                                    tag->add (guidoparam::create((*iter2)->getName()));
                                    rational offset(fCurrentOffset, fCurrentDivision*4);
                                    float intensDx = timePositions.getDxForElement(element, fCurrentVoicePosition.toDouble(), fCurrentMeasure->getAttributeValue("number"), fTargetVoice, directionStaff, offset.toDouble());
                                    
                                    // add pending word parameters (for "before")
                                    if (!generateAfter) {
                                        if (wordParameterBuffer.size()) {
                                            tag->add (guidoparam::create(wordParameterBuffer, false));
                                            wordParameterBuffer = "";
                                        }
                                        
                                        // apply inherited Y-position
                                        stringstream s;
                                        s << "dy=" << commonDy << "hs";
                                        tag->add (guidoparam::create(s.str(), false));
                                        
                                        // Apply dx in case of consecutive dynamics (e.g. "sf ff")
                                        if (dynamicsDx != 0.0) {
                                            stringstream s;
                                            s << "dx=" << dynamicsDx << "hs";
                                            tag->add (guidoparam::create(s.str(), false));
                                        }else if (intensDx != -999) {
                                            stringstream s;
                                            s << "dx=" << intensDx ;
                                            tag->add (guidoparam::create(s.str(), false));
                                        }
                                        
                                        /// Add Tag
                                        if (fCurrentOffset > 0)
                                            addDelayed(tag, fCurrentOffset);
                                        else {
                                            add(tag);
                                        }
                                    }
                                    dynamicsDx = 4.0;   // heuristic HS to avoid collision between consecutive dynamics
                                }
                            }
                            
                            break;
                        }
                            
                        case k_metronome:
                        {
                            metronomevisitor mv;
                            xml_tree_browser browser(&mv);
                            browser.browse(*elt);
                            
                            std::string tempoMetronome = parseMetronome(mv);
                            
                            tag = guidotag::create("tempo");
                            stringstream tempoParams;
                            if (tempoWording.size())
                            {
                                tempoParams << "\""<<tempoWording<<" "<<tempoMetronome<<"\"";
                            }
                            else
                                tempoParams << "\""+tempoMetronome+"\"";
                            
                            if (tempoTextParameters.size()) {
                                tempoParams << tempoTextParameters;
                            }
                            
                            if (tempoParams.str().size())
                            {
                                tag->add (guidoparam::create(tempoParams.str(), false));
                            }
                            
                            /// Take into account group positioning
                            float posy = xml2guidovisitor::getYposition(element, 0, true);
                            if (posy != 0.0) {
                                // then apply and save
                                commonDy += xml2guidovisitor::getYposition(element, -4.0, true);  // Should this be additive?
                            }
                                                                                    
                            // apply inherited Y-position
                            if (commonDy != 0.0) {
                                stringstream s;
                                s << "dy=" << commonDy << "hs";
                                tag->add (guidoparam::create(s.str(), false));
                            }
                            
                            /// Add Tag
                            if (fCurrentOffset)
                                addDelayed(tag, fCurrentOffset);
                            else {
                                add(tag);
                            }
                            
                            generateTempo = false;
                        }
                            break;
                            
                        case k_rehearsal:
                        {
                            string rehearsalValue = element->getValue();
                            rehearsalValue = "\""+rehearsalValue+"\"";
                            
                            string enclosure = element->getAttributeValue("enclosure");
                            string font_size = element->getAttributeValue("font-size");
                            string font_weight = element->getAttributeValue("font-weight");
                            string font_style = element->getAttributeValue("font-style");
                            
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

                                rational offset(fCurrentOffset, fCurrentDivision*4);
                                float markDx = timePositions.getDxForElement(element, fCurrentVoicePosition.toDouble(), fCurrentMeasure->getAttributeValue("number"), 0, directionStaff, offset.toDouble());
                                if (markDx != -999 && markDx != 0) {
                                    stringstream s;
                                    s << "dx=" << markDx ;
                                    tag->add (guidoparam::create(s.str(), false));
                                }
                                xml2guidovisitor::addPosY(element, tag, -4, 1);
                                
                                add (tag);
                            }
                        }
                            break;
                            
                        case k_wedge:
                        {
                            parseWedge(element, directionStaff);
                        }
                        break;
                            
                        default:
                            break;
                    }
                }
            }
        }
        
        // If composed tag, add here
        if (generateAfter) {
            // apply inherited Y-position
            stringstream s;
            s << "dy=" << commonDy << "hs";
            tag->add (guidoparam::create(s.str(), false));
            /// Add Tag
            if (fCurrentOffset)
                addDelayed(tag, fCurrentOffset);
            else {
                add(tag);
            }
        }
        
        // We get to this block is we have a combination of Words AND Sound only (without Metronome)
        if (generateTempo && tempoWording.size()) {
            Sguidoelement tempotag = guidotag::create("tempo");
            std::stringstream tempoParameters;
            tempoParameters <<"\"" << tempoWording << "\""<< tempoTextParameters;

            tempotag->add (guidoparam::create(tempoParameters.str(), false));
            
            if (fCurrentOffset)
                addDelayed(tempotag, fCurrentOffset);
            else {
                add(tempotag);
            }
        }
        
        
        fSkipDirection = false;
        fCurrentOffset = 0;
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
    
void xmlpart2guido::parseWedge(MusicXML2::xmlelement *elt, int staff)
{
    if (elt->getType() != k_wedge) {
        return;
    }
        
    string type = elt->getAttributeValue("type");
    int number = elt->getAttributeIntValue("number", 1);
    Sguidoelement tag;
    if (type == "crescendo") {
        tag = guidotag::create("crescBegin");
        fCrescPending = number;
    }
    else if (type == "diminuendo") {
        tag = guidotag::create("dimBegin");
        fDiminPending = number;
    }
    else if (type == "stop") {
                
        if (fCrescPending == number) {
            tag = guidotag::create("crescEnd");
            fCrescPending = 0;
        }else
        if (fDiminPending == number) {
            tag = guidotag::create("dimEnd");
            fDiminPending = 0;
        }
    }
    
    if (tag) {
        /// Also add SPREAD values (in mXML tenths - conversion: (X / 10) * 2)
        /// Spread is present right away for a diminuendo, it'll be present for crescendo at its STOP type
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
            ctree<xmlelement>::iterator wedgeBegin= find(fCurrentPart->begin(), fCurrentPart->end(), elt);
            int crescendoNumber = elt->getAttributeIntValue("number", 1);   // default is 1 for wedge!
            ctree<xmlelement>::iterator nextevent  = wedgeBegin;
            nextevent++;    // advance one step
            
            // find next S_direction in measure
            ctree<xmlelement>::iterator nextWedge = fCurrentPart->find(k_wedge, nextevent);
            
            if (nextWedge != fCurrentPart->end()) {
                
                while ( ( nextWedge->getAttributeIntValue("number", 1) != crescendoNumber)
                       &&
                       (nextWedge->getAttributeValue("type")!="stop") )
                {
                    nextWedge = fCurrentPart->find(k_wedge, nextevent++);
                }
                
                ctree<xmlelement>::iterator wedgeEnd = nextWedge;
                
                /// GUID-88: Avoid parsing dx2 and dx1 on "single-event wedge"
                int numberOfNotesInWedge = 0;
                nextevent = wedgeBegin;nextevent++;
                while ( nextevent != wedgeEnd ) {
                    if (nextevent->getType() == k_note) {
                        if (nextevent->hasSubElement(k_chord)) {
                            // in chord, do not increment
                        }else {
                            numberOfNotesInWedge++;
                        }
                    }
                    nextevent++;
                }
                
                //cerr<< "Measure:"<< fMeasNum <<" Wedge has "<< numberOfNotesInWedge<< " events!"<<endl;
                
                if (numberOfNotesInWedge > 1) {
                    /// fetch dx1 and dx2 value based on ending
                    float posx1 = elt->getAttributeFloatValue("relative-x", 0);  //elt->getAttributeFloatValue("default-x", 0) +
                    
                    //// Add dx1 and dx2 parameters
                    if (posx1!=0.0) {
                        posx1 = (posx1 / 10) * 2;   // convert to half spaces
                        
                        stringstream s;
                        s << "dx1=" << posx1 << "hs";
                        tag->add (guidoparam::create(s.str(), false));
                    }
                    
                    /// !Important: the relative-x on the wedge end CAN NOT be directly translated to GMN as it refers to the x-position at the placement of the Wedge Stop. This should be handled by the GDevice in Guido instead.
                    /*
                     float posx2 = nextWedge->getAttributeFloatValue("relative-x", 0);
                     if (posx2!=0.0) {
                     posx2 = (posx2 / 10) * 2;   // convert to half spaces
                     
                     stringstream s;
                     s << "dx2=" << posx2 << "hs";
                     tag->add (guidoparam::create(s.str(), false));
                     }*/
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
        }
        
        stringstream s;
        s << "dy=" << xml2guidovisitor::getYposition(elt, 13, true) << "hs";
        
        rational offset(fCurrentOffset, fCurrentDivision*4);
        float wedgeDx = timePositions.getDxForElement(elt, fCurrentVoicePosition.toDouble(),
                                                     fCurrentMeasure->getAttributeValue("number"),
                                                     0, staff, offset.toDouble());
        if (wedgeDx != -999 && wedgeDx != 0) {
            s << ", dx=" << wedgeDx ;
        }
        tag->add (guidoparam::create(s.str(), false));

        if (fCurrentOffset > 0) {
            addDelayed(tag, fCurrentOffset);
        }
        else {
            add (tag);
        }
    }
}
    

std::string xmlpart2guido::parseMetronome ( metronomevisitor &mv )
{
    if (mv.fBeats.size() != 1) return "";                    // support per minute tempo only (for now)
    if (!mv.fPerMinute) return "";        // support per minute tempo only (for now)
    
    rational r = NoteType::type2rational(NoteType::xml(mv.fBeats[0].fUnit)), rdot(3,2);
    while (mv.fBeats[0].fDots-- > 0) {
        r *= rdot;
    }
    r.rationalise();
    
    stringstream s;
    s << "[" << (string)r << "] = " << mv.fPerMinute;
    return s.str();
}

    
    //______________________________________________________________________________
    void xmlpart2guido::parseOctaveShift(int type) {
        Sguidoelement tag = guidotag::create("oct");

        if (type) {
            tag->add (guidoparam::create(type, false));
            fCurrentOctavaShift = type;
        }else { // stop
            fShouldStopOctava = true;
            //  Here the octave offset should take effect immediately, not the next time it is created
            fCurrentOctavaShift = 0;
            tag->add (guidoparam::create(0, false));
        }
        
        if (fNotesOnly) {
            tag->add(guidoparam::create("hidden=\"on\"", false));
        }
        
        // Try to infer default-x position if available
        stringstream s;
        rational offset(fCurrentOffset, fCurrentDivision*4);
        
        if (fCurrentOffset > 0) {
            addDelayed(tag, fCurrentOffset);
        }
        else {
            add(tag);
        }
    }
    
    void xmlpart2guido::parseOctaveShift(MusicXML2::xmlelement *elt, int staff)
    {
        const string& type = elt->getAttributeValue("type");
        int size = elt->getAttributeIntValue("size", 8);
        
        switch (size) {
            case 8:        size = 1; break;
            case 15:    size = 2; break;
            default:    return;
        }
        if (type != "stop") {
            if (type == "up")
                size = -size;
        }
        else {
            size = 0;
        }
        
        parseOctaveShift(size);
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
            int staffnum = iter->getAttributeIntValue("number", 1);
            
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
            
            // staffClefMap: multimap containing <staff-num, measureNum, position, clef type>
            std::pair<rational, std::string> positionClef = std::pair<rational, std::string>(fCurrentVoicePosition ,param);
            staffClefMap.insert(std::pair<int, std::pair < int , std::pair<rational, std::string> > >(staffnum, std::pair< int, std::pair< rational, std::string > >(fMeasNum, positionClef) ) );
            //cerr<<"\t\t<<< staffClefMap adding "<<param<<" staff:"<<staffnum <<" pos: ";fCurrentVoicePosition.print(cerr);cerr<<" size="<<staffClefMap.size()<<endl;
            
            if ((staffnum != fTargetStaff) || fNotesOnly)
            {
                // We should generate Empty at the boundary of Clef Change IF there's no Note event in the measure
                if (!findNextNote(0, elt)) {
                    checkVoiceTime (fCurrentMeasurePosition, fCurrentVoicePosition);
                }
                /// Search again for other clefs:
                iter++;
                iter = elt->find(k_clef, iter);
                continue;
            }
            // Make sure we are at the right position before adding clef
            checkVoiceTime (fCurrentMeasurePosition, fCurrentVoicePosition);

            Sguidoelement tag = guidotag::create("clef");
            checkStaff (staffnum);
            tag->add (guidoparam::create(param));
            add(tag);
            
            
            /// Search again for other clefs:
            iter++;
            iter = elt->find(k_clef, iter);
        }
        
        // staves are treated at the S_Part level (see xml2guidovisitor) to create accolades
        
        // Generate key
        iter = elt->find(k_key);
        if ((iter != elt->end())&&(!fNotesOnly)) {
            parseKey(iter);
        }
        
        // Generate Time Signature info and METER info
        iter = elt->find(k_time);
        if ((iter != elt->end())&&(!fNotesOnly)) {
            parseTime(iter);
        }
    }

void xmlpart2guido::parseKey(ctree<xmlelement>::iterator &iter) {
    string keymode = iter->getValue(k_mode);
    int keyfifths = iter->getIntValue(k_fifths, 0);
    Sguidoelement tag = guidotag::create("key");
    tag->add (guidoparam::create(keyfifths, false));
    
    lastKey = tag;
    add (tag);
}

void xmlpart2guido::parseTime(ctree<xmlelement>::iterator &iter) {
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
        fTimeSignInternal.push_back(make_pair(iter_beat->getValue(),
                                              iter_beatType->getValue()));
        iter_beat = iter->find(k_beats, iter_beat++);
        iter_beatType = iter->find(k_beat_type, iter_beatType++);
    }
    
    /// Actions:
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
    if (fGenerateBars)
        tag->add (guidoparam::create("autoBarlines=\"off\"", false));
    if (fGenerateAutoMeasureNum)
        tag->add (guidoparam::create("autoMeasuresNum=\"system\"", false));
    
    if (iter->getAttributeValue("print-object")!="no") {
        lastMeter = tag;
        add(tag);
    }
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
                /// MusicXML does not always contain the "number" attribtue! if not, we'll assign them ourselves!
                stringstream tagName;
                if (fTiedOpen.empty()) {
                    fTiedOpen.push(1);
                }else {
                    fTiedOpen.push( fTiedOpen.back()+1 );
                }
                string num = (*i)->getAttributeValue ("number");
                if (num.size()) {
                    tagName << "tieBegin" << ":"<< num;
                }else{
                    tagName << "tieBegin" << ":"<< fTiedOpen.back();
                }
                Sguidoelement tag = guidotag::create(tagName.str());
                string placement = (*i)->getAttributeValue("orientation");
                if (placement == "under")
                    tag->add (guidoparam::create("curve=\"down\"", false));
                if (placement == "over")
                    tag->add (guidoparam::create("curve=\"up\"", false));
                add(tag);
            }
        }
    }
    
    void xmlpart2guido::checkTiedEnd ( const std::vector<S_tied>& tied )
    {
        // Don't even bother if there is no priorly opened Tied
        if (fTiedOpen.empty()) {
            return;
        }
        std::vector<S_tied>::const_iterator i;
        for (i = tied.begin(); i != tied.end(); i++) {
            if ((*i)->getAttributeValue("type") == "stop") {
                /// MusicXML does not always contain the "number" attribtue! We'll assign them ourselves!
                stringstream tagName;
                string num = (*i)->getAttributeValue ("number");
                if (num.size()) {
                    tagName << "tieEnd" << ":"<< num;
                }else{
                    tagName << "tieEnd" << ":"<< fTiedOpen.front();
                }
                
                Sguidoelement tag = guidotag::create(tagName.str());
                
                fTiedOpen.pop();
                add(tag);
            }
        }
    }
    
    //______________________________________________________________________________
    void xmlpart2guido::checkSlurBegin ( const std::vector<S_slur>& slurs )
    {
        /*
         In Guido, slurBegin should be generated before notename and slurEnd after (or otherwise leading to bad numbering and rendering issues such as big slurs).
         Whereas in MusicXML, Slurs are attributes of Notations inside a note event.
         
         There is one special case where slurEnd might appear before slurBegin: for consecutif slurs where notation is as follows:
         <notations>
           <slur number="1" type="stop"/>
           <slur number="1" placement="above" type="start"/>
         </notations>

         This leads to the conclusion that the numbering in Guido and XML can NOT be the same. So we should track them indefinitely!
         */
        std::vector<S_slur>::const_iterator i;
        int counter = 0;
        for (i = slurs.begin(); i != slurs.end(); i++) {
            if ((*i)->getAttributeValue("type") == "start") {
                // There is a Slur Begin. Creat BeamBegin tag, and add its number to Stack
                int lastSlurInternalNumber = 1;
                if (!fSlurStack.empty()) {
                    std::pair<int, int> toto = fSlurStack.back();
                    lastSlurInternalNumber = toto.first + 1;
                }
                                
                // Skip Slur creation, if the CLOSING Slur is not on the same voice/staff (Guido limitation)
                if (isSlurClosing(*i)==false) {
                    //cerr<< "XML Slur at line:"<< (*i)->getInputLineNumber()<<" measure:"<<fMeasNum << " not closing on same voice! Skipping!"<<endl;
                    counter++;
                    continue;
                }
                
                
                stringstream tagName;
                tagName << "slurBegin" << ":"<< lastSlurInternalNumber;
                Sguidoelement tag = guidotag::create(tagName.str());
                string placement = (*i)->getAttributeValue("placement");
                string orientation = (*i)->getAttributeValue("orientation");
                if ((placement == "below")||(orientation=="under"))
                    tag->add (guidoparam::create("curve=\"down\"", false));
                if ((placement == "above")||(orientation=="over"))
                    tag->add (guidoparam::create("curve=\"up\"", false));
                add(tag);
                
                // Add to stack:
                std::pair<int,int> toto2(lastSlurInternalNumber, (*i)->getAttributeIntValue("number", 0));
                fSlurStack.push_back(toto2);
            }
            counter++;
        }
    }

bool xmlpart2guido::isSlurClosing(S_slur elt) {
    int internalXMLSlurNumber = elt->getAttributeIntValue("number", 0);

    //cerr<< "\tSearching Slur Closing for line:"<<elt->getInputLineNumber() <<" with number "<<internalXMLSlurNumber<< " on Measure:"<<fMeasNum<< " on voice:"<<fTargetVoice<<endl;
    ctree<xmlelement>::iterator nextnote = find(fCurrentPart->begin(), fCurrentPart->end(), elt);
    if (nextnote != fCurrentPart->end()) {
        nextnote.forward_up();    // advance one step
    }
    
    // Stop Conditions: (1) The first occurence of a slur STOP with the same NUMBER attribute should be considered as the target.
    //  (2) If the measureNumber goes beyond current measure + 10 (this will greatly enhance speed!!). We can assume that slurs do not go beyond 10 measures in regular scores!
    // Do not go beyond.
    
    int searchMeasureNum = fMeasNum;
    
    while ((nextnote != fCurrentPart->end()) && (searchMeasureNum <= fMeasNum + 10)) {
        // Check measure
        if (nextnote->getType() == k_measure) {
            std::string measNum = nextnote->getAttributeValue("number");
            try {
                searchMeasureNum = std::stoi(measNum);
            } catch(...) {
                searchMeasureNum++;
            }
            // Move to the first sub-element
            nextnote.forward();
        }
        
        // looking for the next note on the target voice
        if ((nextnote->getType() == k_note)) {
            int thisNoteVoice = nextnote->getIntValue(k_voice,0);
            ctree<xmlelement>::iterator iter;
            iter = nextnote->find(k_notations);
            if (iter != nextnote->end())
            {
                ctree<xmlelement>::iterator iterSlur;
                iterSlur = iter->find(k_slur);
                if (iterSlur != iter->end())
                {
                    
//                    if ((iterSlur->getAttributeValue("type")=="start") &&
//                        ((iterSlur->getAttributeIntValue("number", 0) == internalXMLSlurNumber)) &&
//                        (thisNoteVoice == fTargetVoice)) {
//                        return false;
//                    }
                    
                    if ((iterSlur->getAttributeValue("type")=="stop") &&
                        (iterSlur->getAttributeIntValue("number", 0) == internalXMLSlurNumber)
                        ) {
                        if (thisNoteVoice == fTargetVoice) {
                            //cerr<< "\t\t\t FOUND Slur stop line:"<< iterSlur->getInputLineNumber()<< " voice:"<<thisNoteVoice<<" number:"<<iterSlur->getAttributeIntValue("number", 0)<<endl;

                            return true;
                        }else {
                            return false;   // we found the same slur numbering with "stop" on another voice!
                        }
                    }
                }
            }
        }
        nextnote.forward_up();
    }
    
    return false;
}
    
    void xmlpart2guido::checkSlurEnd ( const std::vector<S_slur>& slurs )
    {
        std::vector<S_slur>::const_iterator i;
        for (i = slurs.begin(); i != slurs.end(); i++) {
            if ((*i)->getAttributeValue("type") == "stop") {
                int lastSlurInternalNumber = 0;
                std::vector< std::pair<int, int> >::const_iterator slurEndToBeErase;
                if (!fSlurStack.empty()) {
                    // find the corresponding open slur in stack that has the same xml-num as this one
                    int xmlnum = (*i)->getAttributeIntValue("number", 0);
                    slurEndToBeErase = findSlur(xmlnum);
                    if (slurEndToBeErase != fSlurStack.end()) {
                        lastSlurInternalNumber = slurEndToBeErase->first;
                    }else {
                        continue;
                    }

                }else {
#if DEBUG
                    cerr<< "XML2Guido SlurEnd, measure "<<fMeasNum<<" xmlLine "<<(*i)->getInputLineNumber() <<": Got Slur Stop with number:"<< (*i)->getAttributeIntValue("number", 0)  <<" without a Slur in Stack. Skipping!"<<endl;
#endif
                    continue;
                }
                
                stringstream tagName;
                tagName << "slurEnd" << ":"<< lastSlurInternalNumber;
                Sguidoelement tag = guidotag::create(tagName.str());
                add(tag);
                
                // remove the slur from stack
                fSlurStack.erase(slurEndToBeErase);
            }
        }
    }

std::vector< std::pair<int, int> >::const_iterator xmlpart2guido::findSlur ( const int xmlnum ) const {
    std::vector< std::pair<int, int> >::const_iterator i;
    
    for (i=fSlurStack.begin(); i != fSlurStack.end(); i++) {
        if (i->second == xmlnum) {
            break;
        }
    }
    return i;
}

    
    //______________________________________________________________________________
    void xmlpart2guido::checkBeamBegin ( const std::vector<S_beam>& beams, const S_note& elt )
    {
        /// !IMPORTANT NOTE from MXML DOC: Note that the beam number does not distinguish sets of beams that overlap, as it does for slur and other elements.
        ///             So we need to track them with s Stack
        /// ! IMPORTANT NOTE from MXML DOC: "Beaming groups are distinguished by being in different voices and/or the presence or absence of grace and cue elements."
        ///             This means that we should treate Grace and Cue elements separately.
        /// !IMPORTANT NOTE on Guido Syntax: In Guido, the semantic of beam is the following: Nested beams make sense up to 2 order. The higher order has ONLY a Grouping semantics (meaning will link the two beams with a single-dashed line). It is only the internal and foremost internal beam that can deduct correct durations. Nested beams beyond level 2 does not have any effect. In MusicXML, beams do not have any semantics and only indicate lines!
        
        /// How to detect a grouping then? A grouping occurs if an xml beam ends, and if the current and next note are of the same xml Type (16th for two beams). One should probably check the number of beams with the type?! This is a look-ahead operation!
        
        std::vector<S_beam>::const_iterator began = findValue(beams, "begin");

        // Create beamBegin only if no beam is already opened. Groupings will be handled upon the initial openning.
        if ( (began != beams.end()) && (!fBeamOpened || fInGrace)){
            stringstream tagName;
            int beamNumber = 1;
            if (fInGrace && fBeamOpened) {
                beamNumber = 2;
            }
            tagName << "beamBegin" << ":"<<beamNumber;     // This is the initial beam!
            Sguidoelement tag = guidotag::create(tagName.str());
            add (tag);
            if ( (!fInCue) && (!fInGrace)) {
                fBeamOpened = true;
            }
            //cerr << "Measure "<< fMeasNum << " beam BEGIN Beam-level="<<(*began)->getAttributeIntValue("number", 0)<< " fBeamOpened?="<<fBeamOpened<< " Grace?"<<fInGrace<< " Line:"<<(*began)->getInputLineNumber()<<endl;
            /// Check for grouping is one is not already initiated
            if (!fBeamGrouping) {
                ctree<xmlelement>::iterator nextnote = find(fCurrentMeasure->begin(), fCurrentMeasure->end(), elt);
                nextnote.forward_up(); // forward one element
                while (nextnote != fCurrentMeasure->end()) {
                    if (( (nextnote->getType() == k_note) && (nextnote->getIntValue(k_voice,0) == fTargetVoice) )) {
                        // Check if there is a beam end with a beam Continue, it can be a Grouping candidate!
                        if (nextnote->hasSubElement(k_beam, "end") && nextnote->hasSubElement(k_beam, "continue") ) {
                            //cerr << " \tNextNote with beam end, line:"<<nextnote->getInputLineNumber()<<" type="<<nextnote->getValue(k_type)<<endl;
                            // Get its Type
                            string endingType = nextnote->getValue(k_type);
                            // Check if the Next note has a beam Begin and if it has the same "type"
                            ctree<xmlelement>::iterator postnote;
                            if (findNextNote(nextnote, postnote)) {
                                //cerr << " \tPostnote with beam end, line:"<<postnote->getInputLineNumber()<<" type="<<postnote->getValue(k_type) <<endl;
                                if (postnote->hasSubElement(k_beam, "continue")) {
                                    string postType = postnote->getValue(k_type);
                                    if (postType == endingType) {
                                        // We are in a grouping continuity!
                                        stringstream tagName2;
                                        tagName2 << "beamBegin" << ":"<< (beamNumber+1);
                                        tag = guidotag::create(tagName2.str());
                                        add (tag);
                                        fBeamGrouping = true;
                                        //cerr << " \t\t CONTINUITY CREATED! Line:"<<postnote->getInputLineNumber()<<endl;
                                        break;
                                    }
                                }
                            }
                        }else if (!nextnote->hasSubElement(k_beam, "continue")) {
                            // If there is no beam element with "continue" value, then our search is over!
                            break;
                        }
                    }
                    nextnote.forward_up();
                }
            }
        }
        
        if (beams.empty() && notevisitor::getType()!=kRest)
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
        
        std::vector<S_beam>::const_iterator end = findValue(beams, "end");
        std::vector<S_beam>::const_iterator continuity = findValue(beams, "continue");
        std::vector<S_beam>::const_iterator begin = findValue(beams, "begin");

        bool ended = (end != beams.end());
        bool began = (begin != beams.end());
        bool withContinuity = (continuity != beams.end());
        
        if (ended && (!began) && !withContinuity && (fBeamOpened || fInGrace)) {
            stringstream tagName;
            int beamNumber = 1;
            if (fInGrace && fBeamOpened) {
                beamNumber = 2;
            }
            tagName << "beamEnd" << ":"<<beamNumber;
            Sguidoelement tag = guidotag::create(tagName.str());
            add (tag);
            if ((fBeamOpened) && (!fInCue) && (!fInGrace)) {
                fBeamOpened = false;
            }
            
            // If there is a grouping, close it!
            if (fBeamGrouping) {
                stringstream tagName2;
                tagName2 << "beamEnd" << ":"<<beamNumber+1;
                tag = guidotag::create(tagName2.str());
                add (tag);
                fBeamGrouping = false;
            }
        }else if (withContinuity && fBeamGrouping && ended) {
            // This occurs only during a grouping continuity:
            stringstream tagName;
            tagName << "beamEnd" << ":2";
            Sguidoelement tag = guidotag::create(tagName.str());
            add (tag);
            stringstream tagName2;
            tagName2 << "beamBegin" << ":2";
            tag = guidotag::create(tagName2.str());
            add (tag);
        }
        
        // Experimental
        //if (beamStackSizeBeforeClosing > fBeamStack.size())
        //{
        //    checkTextEnd();
        //}
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
            /// Determine whether we need Brackets and Numbering
            bool withBracket = ((*i)->getAttributeValue("bracket")=="yes");
            std::string showNumbering = ((*i)->getAttributeValue("show-number"));
            /// Get Tuplet Number
            int thisTupletNumber = (*i)->getAttributeIntValue("number", 1);
            /// Get Tuplet Placement and graphic type
            std::string tupletPlacement = (*i)->getAttributeValue("placement");
            std::string tupletGraphicType = nv.fGraphicType;
            long numberOfEventsInTuplet = nv.getTimeModification().getDenominator();
            long numberOfNormalNotes = nv.getTimeModification().getNumerator();
            
            bool useDispNoteAttribute = false;

            ///// if "tuplet-actual" is present override numberOfEventsInTuplet and tupletGraphicType
            auto tuplet_actual = (*i)->find(k_tuplet_actual);
            if ( tuplet_actual != (*i)->end()) {
                numberOfEventsInTuplet = tuplet_actual->getIntValue(k_tuplet_number, 1);
                tupletGraphicType = tuplet_actual->getValue(k_tuplet_type);
                useDispNoteAttribute = true;
            }
            
            ///// if "tuplet-normal" is present override numberOfEventsInTuplet and tupletGraphicType
            auto tuplet_normal = (*i)->find(k_tuplet_normal);
            if (tuplet_normal != (*i)->end()) {
                numberOfNormalNotes = tuplet_normal->getIntValue(k_tuplet_number, 1);
            }

            //// Rational : If all note durations are equal, then use the dispNote attribute. If not, then don't!
            if (useDispNoteAttribute == false) {
                /// Get Top Note Guido Duration
                rational topNoteDurRational = NoteType::type2rational(NoteType::xml(nv.getGraphicType()));
                if (topNoteDurRational.getNumerator() == 0)
                    topNoteDurRational.set (nv.getDuration(), fCurrentDivision*4);
                topNoteDurRational.rationalise();
                rational tm = nv.getTimeModification();
                topNoteDurRational *= tm;topNoteDurRational.rationalise();

                /// Browse through all elements of Tuplet until "stop"!
                ctree<xmlelement>::iterator nextnote = find(fCurrentMeasure->begin(), fCurrentMeasure->end(), elt);
                if (nextnote != fCurrentMeasure->end()) {
                    nextnote++;	// advance one step
                }
                useDispNoteAttribute = true; // setback to True.. will become false if check doesn't pass!
                while (nextnote != fCurrentMeasure->end()) {
                    // looking for the next note on the target voice
                    if ((nextnote->getType() == k_note) && (nextnote->getIntValue(k_voice,0) == fTargetVoice)) {
                        // Get note's Guido Duration
                        rational nextNoteDur = NoteType::type2rational(NoteType::xml(nextnote->getValue(k_type)));
                        if (nextNoteDur.getNumerator()==0)
                            nextNoteDur.set(nextnote->getIntValue(k_duration, 0), fCurrentDivision*4);
                        nextNoteDur.rationalise();
                        rational nextNoteTm;
                        nextNoteTm.set(nextnote->getIntValue(k_normal_notes, 1), nextnote->getIntValue(k_actual_notes, 1));
                        nextNoteDur *= nextNoteTm; nextNoteDur.rationalise();

                        if ( topNoteDurRational != nextNoteDur ) {
                            useDispNoteAttribute =  false;
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
            }
            
            /// Determine the graphical format inside Tuplet
            std::string dispNotePar ;
            int dy1offset = 6;
            if (tupletGraphicType=="64th")
            {
                dispNotePar = "\"/64\"";
                dy1offset+=4;
            }
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
            //if ((numberOfEventsInTuplet!=3) ||(dispNotePar.size()))
            //{
                Sguidoelement tag = guidotag::create("tuplet");
                /// Add number visualiser
                stringstream tuplet;
                string numbering = std::to_string(numberOfEventsInTuplet);  // default is "actual" for "show-numbering)
                if (showNumbering=="none") {
                    numbering = "";
                }else if (showNumbering == "both") {
                    numbering = std::to_string(numberOfEventsInTuplet)+":"+std::to_string(numberOfNormalNotes);
                }
                
                if (numberOfEventsInTuplet>1)   // workaround for pianistic Tremolos that come out of Finale as Tuplets!
                    tuplet << (withBracket? "-" : "") << numbering << (withBracket? "-" : "");
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
                
                fTupletOpen++;
                
                push (tag);
            //}
        }
    }
    
    void xmlpart2guido::checkTupletEnd ( const std::vector<S_tuplet>& tuplets )
    {
        std::vector<S_tuplet>::const_iterator i;
        for (i = tuplets.begin(); (i != tuplets.end()); i++) {
            // Do not check for tupletNumber (might cause conflict with nested Tuplets) -- We assume everything is there!
            if (((*i)->getAttributeValue("type") == "stop") && (fTupletOpen>0)) { 
                pop();
                fTupletOpen--;
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
    }
    
    void xmlpart2guido::checkLyricEnd	 ( const std::vector<S_lyric>& lyrics )
    {
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
                //if (fCurrentStemDirection != kStemDown || fInCue) {
                tag = guidotag::create("stemsDown");
                fCurrentStemDirection = kStemDown;
                //}
            }
            else if (stem->getValue() == "up") {
                //if (fCurrentStemDirection != kStemUp || fInCue) {
                tag = guidotag::create("stemsUp");
                fCurrentStemDirection = kStemUp;
                //}
            }
            else if (stem->getValue() == "none") {
                //if (fCurrentStemDirection != kStemNone || fInCue) {
                tag = guidotag::create("stemsOff");
                fCurrentStemDirection = kStemNone;
                //}
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
        if (note.fArpeggio) {
            tag = guidotag::create("arpeggio");
            xml2guidovisitor::addDirection(note.fArpeggio, tag);
            push(tag);
            n++;
        }
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
        if (note.fStaccatissimo) {
            tag = guidotag::create("stacc");
            tag->add (guidoparam::create("type=\"heavy\"", false));
            if (fGeneratePositions) xml2guidovisitor::addPlacement(note.fStaccatissimo, tag);
            push(tag);
            n++;
        }
        if (note.fTenuto) {
            tag = guidotag::create("ten");
            if (fGeneratePositions) xml2guidovisitor::addPlacement(note.fTenuto, tag);
            push(tag);
            n++;
        }
        if (note.fHarmonic) {
            tag = guidotag::create("harmonic");
            xml2guidovisitor::addPlacement(note.fHarmonic, tag);
            push(tag);
            n++;
        }
        if (note.fSnapPizzicato) {
            tag = guidotag::create("pizz");
            stringstream s;
            s << "type=\"snap\"";
            tag->add (guidoparam::create(s.str(), false));
            push(tag);
            n++;
        }
        
        return n;
    }

/// Articulations that should be generated AFTER the note creation
void xmlpart2guido::checkPostArticulation ( const notevisitor& note )
{
    if (note.fBreathMark) {
        Sguidoelement tag = guidotag::create("breathMark");
        xml2guidovisitor::addPosY(note.fBreathMark, tag, -3, 1);
        add(tag);
    }
}
    
    //---------------------
    void xmlpart2guido::checkWavyTrillBegin	 ( const notevisitor& nv )
    {
        if (nv.fTrill)
        {
            Sguidoelement tag;
            tag = guidotag::create("trillBegin");
            
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
                // remove the default wavy-line then:
                tag->add(guidoparam::create("wavy=\"off\"", false));
            }
            add(tag);
        }
    }
    
    void xmlpart2guido::checkWavyTrillEnd	 ( const notevisitor& nv )
    {
        Sguidoelement tag;
        tag = guidotag::create("trillEnd");
        
        if (nv.getWavylines().size() > 0)
        {
            std::vector<S_wavy_line>::const_iterator i;
            for (i = nv.getWavylines().begin(); i != nv.getWavylines().end(); i++) {
                if ((*i)->getAttributeValue("type") == "stop") {
                    if (fWavyTrillOpened) {
                        add(tag);
                    }
                    fWavyTrillOpened = false;
                }
            }
        }
        else
            if (fSingleScopeTrill) {
                fSingleScopeTrill = false;
                add(tag);
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
        
        if (note.fTrill) {
            tag = guidotag::create("trill");
            //n++;
        }
        
        if (note.fBowUp || note.fBowDown) {
            tag = guidotag::create("bow");
            // in MusicXML, bow default-y's origin is changed relative to the top line of the staff. Positive y is up and negative y is down.
            // In Guido, vertical origin is from the top of staff IF the associated note head is BELOW the staff top and from the notehead otherwise!
            if (note.fBowDown) {
                stringstream s;
                s << "type=\"down\"";
                tag->add (guidoparam::create(s.str(), false));
                float default_y = (float)(note.fBowDown->getAttributeIntValue("default-y", 0));
                if (default_y != 0) {
                    addDyFromNoteOrStaff(note, note.fBowDown, tag);
                }else {
                    xml2guidovisitor::addPlacement(note.fBowDown, tag);
                }
                xml2guidovisitor::addPosX(note.fBowDown, tag, 0.0);
            } else {
                stringstream s;
                s << "type=\"up\"";
                tag->add (guidoparam::create(s.str(), false));
                float default_y = (float)(note.fBowUp->getAttributeIntValue("default-y", 0));
                if (default_y != 0) {
                    addDyFromNoteOrStaff(note, note.fBowUp, tag);
                }else {
                    xml2guidovisitor::addPlacement(note.fBowUp, tag);
                }
                xml2guidovisitor::addPosX(note.fBowUp, tag, 0.0);
            }
            push(tag);
            n++;
        }
        
        return n;
    }
    
    int xmlpart2guido::checkTremolo(const notevisitor& note, const S_note& elt) {
        // Starting FINALE 24, there is Tremolo types "Single", "start" and "stop". The "Start" type should search for a "stop" to construct the tremolo
        /* Notes from MusicXML Doc:
         When using double-note tremolos, the duration of each note in the tremolo should correspond to half of the notated type value.
         A time-modification element should also be added with an actual-notes value of 2 and a normal-notes value of 1.
         If used within a tuplet, this 2/1 ratio should be multiplied by the existing tuplet ratio.
         */
                
        if (note.fTremolo) {
            Sguidoelement tag;
            stringstream s;
            
            std::string tremType = note.fTremolo->getAttributeValue("type");
            if (tremType == "single") {
                tag = guidotag::create("trem");
                // trem style is the number int value
                std::stringstream convert;
                convert << note.fTremolo->getValue();
                int numDashes = 0;
                try {
                    convert >> numDashes;
                }
                catch (const exception& e) {
                    numDashes = 0;
                }
                
                s << "style=\"";
                for (int id=0; id<numDashes;id++) {
                    s << "/";
                }
                s << "\"";
                tag->add (guidoparam::create(s.str(), false));
                
                push(tag);
                return 1;
            }else
                if (tremType == "start") {
                    fTremoloInProgress = true;
                    tag = guidotag::create("trem");
                    
                    /// Find "stop" pitch
                    ctree<xmlelement>::iterator nextnote = find(fCurrentMeasure->begin(), fCurrentMeasure->end(), elt);
                    if (nextnote != fCurrentMeasure->end()) nextnote++;    // advance one step
                    while (nextnote != fCurrentMeasure->end()) {
                        // looking for the next note on the target voice
                        if ((nextnote->getType() == k_note) && (nextnote->getIntValue(k_voice,0) == fTargetVoice)) {
                            ctree<xmlelement>::iterator iter;            // and when there is one
                            iter = nextnote->find(k_tremolo);
                            if (iter != nextnote->end() && (iter->getAttributeValue("type")=="stop") ) {
                                vector<Sxmlelement> chordStop = getChord( (*nextnote) );
                                s << "pitch=\"";
                                if (chordStop.size())
                                    s<< "{";
                                
                                notevisitor nv;xml_tree_browser browser(&nv);
                                Sxmlelement note = *nextnote;
                                browser.browse(*note);
                                
                                // Add main stop note
                                int octave = nv.getOctave() - 3;            // octave offset between MusicXML and GUIDO is -3
                                string accident = alter2accident(nv.getAlter());
                                string name = noteName(nv);
                                s << name<<accident<<(int)octave;
                                
                                // Add chords if any
                                if (chordStop.size()) {
                                    for (vector<Sxmlelement>::const_iterator chordIter = chordStop.begin(); chordIter != chordStop.end(); chordIter++) {
                                        s<<",";
                                        notevisitor nv;xml_tree_browser browser(&nv);
                                        Sxmlelement note = *chordIter;
                                        browser.browse(*note);
                                        int octave = nv.getOctave() - 3;            // octave offset between MusicXML and GUIDO is -3
                                        string accident = alter2accident(nv.getAlter());
                                        string name = noteName(nv);
                                        s << name<<accident<<(int)octave;
                                    }
                                    s<<"}";
                                }
                                
                                s<<"\"";
                                
                                break;
                            }
                        }
                        nextnote++;
                    }
                    
                    // Only add tag if the Stop has been correctly detected
                    if (s.str().size()) {
                        tag->add (guidoparam::create(s.str(), false));
                        
                        push(tag);
                        // return 0 so that this tag wouldn't be closed by pending Pops. Double-note tremolos are closed using fTremoloInProgress
                        return 0;
                    }
                    
                }
        }
        
        return 0;
    }
    
    //______________________________________________________________________________
    deque<notevisitor> xmlpart2guido::getChord ( const S_note& elt )
    {
        deque<notevisitor> notevisitors;
        
        ctree<xmlelement>::iterator nextnote = find(fCurrentMeasure->begin(), fCurrentMeasure->end(), elt);
        if (nextnote != fCurrentMeasure->end()) nextnote++;	// advance one step
        while (nextnote != fCurrentMeasure->end()) {
            // looking for the next note on the target voice
            if ((nextnote->getType() == k_note) && (nextnote->getIntValue(k_voice,0) == fTargetVoice)) {
                ctree<xmlelement>::iterator iter;			// and when there is one
                iter = nextnote->find(k_chord);
                if (iter != nextnote->end()) {
                    notevisitor nv;
                    xml_tree_browser browser(&nv);
                    Sxmlelement note = *nextnote;
                    browser.browse(*note);
                    notevisitors.push_back(nv);
                }
                else {
                    break;
                }
            }
            nextnote++;
        }
        return notevisitors;
    }
    
    vector<Sxmlelement> xmlpart2guido::getChord ( const Sxmlelement& elt )
    {
        vector<Sxmlelement> v;
        ctree<xmlelement>::iterator nextnote = find(fCurrentMeasure->begin(), fCurrentMeasure->end(), elt);
        if (nextnote != fCurrentMeasure->end()) nextnote++;    // advance one step
        while (nextnote != fCurrentMeasure->end()) {
            // looking for the next note on the target voice
            if ((nextnote->getType() == k_note) && (nextnote->getIntValue(k_voice,0) == fTargetVoice)) {
                ctree<xmlelement>::iterator iter;            // and when there is one
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
        // only consider cues if the note is being printed
        if (!nv.printObject()) {
            return;
        }
        
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
                /// GUID-153: Fetch directions after grace
                Sguidoelement tag = guidotag::create("grace");
                push(tag);
                ctree<xmlelement>::iterator nextnote = find(fCurrentMeasure->begin(), fCurrentMeasure->end(), nv.getSnote());
                nextnote.forward_up(); // forward one element
                while (nextnote != fCurrentMeasure->end()) {
                    // break if next element is a non-grace
                    if (( (nextnote->getType() == k_note) && (nextnote->getIntValue(k_voice,0) == fTargetVoice) )
                        || (nextnote->getType() == k_direction)){
                        if (nextnote->getType() == k_note) {
                            if (nextnote->find(k_grace) != nextnote->end())
                            {
                                // Next note has a grace.. continue loop..
                                nextnote.forward_up(); // forward one element
                            }else {
                                // Non-grace, break...
                                break;
                            }
                        }else if (nextnote->getType() == k_direction) {
                            // should parse direction BEFORE grace occurs: visit the element and put in the eraser stack
                            nextnote->acceptIn(*this);
                            nextnote->acceptOut(*this);
                            fDirectionEraserStack.push(nextnote->getInputLineNumber());
                            nextnote.forward_up(); // forward one element
                        }
                        
                    }else {
                        break;
                    }
                }
                /// End-of Guid-153
                fInGrace = true;
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
            
            // Determine Type ("upright" or "inverted")
            std::string fermataType = nv.fFermata->getAttributeValue("type");
            if (fermataType == "inverted") {
                stringstream s;
                s << "position=" << "\"below\"";
                tag->add (guidoparam::create(s.str(), false));
                
                // We don't add dY since GuidoLib resolves collisions between articulations.
            }
//            else{
//                float noteDistanceFromTopStaff = getNoteDistanceFromStaffTop(nv);
//                // Do not infer fermata position if note is above top of the staff. Let guido do it
//                if (noteDistanceFromTopStaff < 2) {
//                    xml2guidovisitor::addPosY(nv.fFermata, tag, 0, 1.0);
//                }
//            }
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
    guidonoteduration xmlpart2guido::noteDuration ( const notevisitor& nv)
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
            if (fTremoloInProgress) {
                r *= 2.0;
            }
            r.rationalise();
            dur.set (r.getNumerator(), r.getDenominator(), nv.getDots());
        }
        return dur;
    }
    
void xmlpart2guido::newChord(const deque<notevisitor>& nvs, rational posInMeasure) {
    // Generate notes with correct fingering
    std::vector<Sxmlelement> emptyFingerings;
    for ( int index = 0; index < nvs.size(); index++) {
        checkStaff(nvs.at(index).getStaff());
        newNote(nvs.at(index), posInMeasure, nvs.at(index).getFingerings());
    }
}

    //______________________________________________________________________________
    void xmlpart2guido::newNote( const notevisitor& nv, rational posInMeasure, const std::vector<Sxmlelement>& fingerings)
    {
        // Check for Tied Begin
        checkTiedBegin(nv.getTied());
        
        bool printObject = nv.printObject();

        // Fingering is tied to single notes (in chords)
        int hasFingerings = 0;  // 0 if none, greater otherwise!
        
        if (printObject) {
            for (int i=0; i < fingerings.size(); i++) {
                auto f = fingerings[i];
                Sguidoelement tag = guidotag::create("fingering");
                stringstream s;
                float default_y = (float)(f->getAttributeIntValue("default-y", 0));
                if (default_y != 0) {
                    // Fingering default-y is from the top line of staff. In Guido, it is relative to note head
                    float posy = (default_y / 10) * 2;  // convert to half space
                    addPosYforNoteHead(nv, posy, tag, 0.0 );
                }else {
                    std::string placement = f->getAttributeValue("placement");
                    if (placement.size() > 0) {
                        s << "position=\""<<placement<<"\", ";
                    }
                }
                
                float default_x = f->getAttributeFloatValue("default-x", 0);
                float dx = (default_x/10)*2;
                if (dx != 0 && (default_x<20.0)) { // filter values > 20.0 as they might be erroneous offsets from FINALE!
                    s << "dx="<<dx<<", ";
                }
                
                std::string fingeringText = f->getValue();
                s << "text=\"" << fingeringText << "\"";
                
                tag->add (guidoparam::create(s.str(), false));
                push(tag);
                hasFingerings++;
            }
        }
        
        int octave = nv.getOctave() - 3;			// octave offset between MusicXML and GUIDO is -3
        if (fShouldStopOctava) {
            octave -= fCurrentOctavaShift;
        }
        string accident = alter2accident(nv.getAlter());
        string name = noteName(nv);
        guidonoteduration dur = noteDuration(nv);
        
        Sguidoelement note;
        
        if (printObject) {
            note = guidonote::create(fTargetVoice, name, octave, dur, accident);
        }else {
            note = guidonote::create(fTargetVoice, "empty", 0, dur, "");
        }
        
        /// Force Accidental if accidental XML tag is present
        bool forcedAccidental = false;
        if (!nv.fCautionary.empty() && printObject)
        {
            Sguidoelement accForce = guidotag::create("acc");
            push(accForce);
            forcedAccidental = true;
        }
        
        /// MusicXML from Finale can skip the Cautionary and just enter Accidental for those inclued in the key!
        if ((forcedAccidental==false) && (nv.fAccidental.empty() == false) && printObject) {
            Sguidoelement accForce = guidotag::create("acc");
            push(accForce);
            forcedAccidental = true;
        }
        
        /// Add Note head of X offset for note if necessary
        bool noteFormat = false;
        if (nv.getType() != kRest)
            noteFormat = checkNoteFormat(nv, posInMeasure);
                
        add (note);
        
        checkTiedEnd(nv.getTied());

        
        if (noteFormat)
            pop();
        
        if (forcedAccidental)
            pop();
        
        if (hasFingerings > 0) {
            while (hasFingerings>0) {
                pop();
                hasFingerings--;
            }
        }
        
    }
    
    bool xmlpart2guido::checkNoteFormat	 ( const notevisitor& nv , rational posInMeasure)
    {
        bool noteFormat = false;
        
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
        
        float noteDx = timePositions.getDxForElement(nv.getSnote(),
                                                     posInMeasure.toDouble(),
                                                     fCurrentMeasure->getAttributeValue("number"),
                                                     nv.getVoice(),
                                                     nv.getStaff(),
                                                     0);
        // Do not infer default-x on incomplete measures, grace or Chords
        if ( (noteDx != -999 && noteDx != 0) && !fPendingBar && !isProcessingChord && !isGrace() )
        {
            stringstream s;
            s << "dx=" << noteDx ;
            noteFormatTag->add (guidoparam::create(s.str(), false));
            noteFormat = true;
        }
            
        if (noteFormat == true)
        {
            push(noteFormatTag);
        }
        
        return noteFormat;
    }
    
    int xmlpart2guido::checkRestFormat	 ( const notevisitor& nv )
    {
        if (nv.getStep().size())
        {
            // Check out clef for position and voice
            std::string thisClef = getClef(fTargetStaff , fCurrentVoicePosition, fMeasNum);
            if (thisClef.empty()) {
                return 0;
            }
            float noteHeadPos=nv.getNoteHeadDy(thisClef);
            float restformatDy = noteHeadPos;
            // Rest default position in Guido (dy 0) is the middle line of the staff
            // in G-Clef, negative is up and positive is down AND -6 offset if counting from G-Clef C4 which is zero for notehead
            // in C-clef, same but +6 offset
            if (thisClef[0]=='g') {
                restformatDy -= 6;
                restformatDy = restformatDy * -1.0;
            }else if (thisClef[0]=='f') {
                restformatDy += 6;
                restformatDy = restformatDy * -1.0;
            }else if (thisClef[0]=='c') {
                restformatDy -= 6;
                restformatDy = restformatDy * -1.0;
            }
            
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
    
    /// Provides the clef at position, Staff and Measure
    /// @param staffIndex  the staff index **relative to MusicXML** (not guido staff)
    /// @param pos relative position in voice
    /// @param measureNum  measure number
    ///     - IMPORTANT:   the staffIndex in this method should be the XML staffIndex of the PART and not the Guido staff. THis is stored in fTargetStaff
    std::string xmlpart2guido::getClef(int staffIndex, rational pos, int measureNum) {
        //     std::multimap<int,  std::pair< int, std::pair< rational, string > > > staffClefMap;
        std::string thisClef = "";
        if (staffClefMap.size()>0) {
            auto staffRange = staffClefMap.equal_range(staffIndex);
            
            for (auto i = staffRange.first ; i != staffRange.second; i++ )
            {
                // Get the measure number
                if (((i->second).first < measureNum)){
                    thisClef = (i->second).second.second;
                }else if ( ((i->second).first == measureNum)  && ((i->second).second.first <= pos )) {
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
            moveMeasureTime (getDuration(), scanVoice);
            if (scanVoice) {
                checkDelayed (getDuration(), true);
            }
        }
        if (!scanVoice) return;
        
        checkOctavaBegin();

                        
        if ((fTremoloInProgress)&&(fTremolo && (fTremolo->getAttributeValue("type")=="stop"))) {
            fTremoloInProgress = false;
            pop();
            checkTupletEnd(notevisitor::getTuplet());
            checkSlurEnd (notevisitor::getSlur());
            checkBeamEnd (notevisitor::getBeam());
            return;
        }
        
        checkStaff(notevisitor::getStaff());
        
        checkVoiceTime (fCurrentMeasurePosition, fCurrentVoicePosition);
        
        checkCue(*this);
        if (notevisitor::getType() != notevisitor::kRest)
            checkStem (notevisitor::fStem);
        
        checkGrace(*this);
        checkSlurBegin (notevisitor::getSlur());
        checkBeamBegin (notevisitor::getBeam(), elt);
        checkTupletBegin(notevisitor::getTuplet(), *this, elt);
        checkLyricBegin (notevisitor::getLyric());
        checkWavyTrillBegin(*this);
        
        pendingPops += checkFermata(*this);
        pendingPops += checkArticulation(*this);
        
        int chordOrnaments = checkChordOrnaments(*this);
        pendingPops += chordOrnaments;
        
        pendingPops += checkTremolo(*this, elt);   // non-measured tremolos will be popped upon "stop" and not counted here
        
        if (notevisitor::getType()==kRest)
            pendingPops += checkRestFormat(*this);
                
        deque<notevisitor> chord = getChord(elt);
        
        // Add chord or note with proper preprocessing
        if (chord.size())
        {
            Sguidoelement chordtag = guidochord::create();
            push (chordtag);
            pendingPops++;
            isProcessingChord = true;
            // Add current note to the beginning of the Chord vector to separate processing of notes and chords
            chord.push_front(*this);
            
            newChord(chord, thisNoteHeadPosition);
        }else {
            newNote (*this, thisNoteHeadPosition, this->getFingerings());
        }
        
        if (fShouldStopOctava) {
            fShouldStopOctava = false;
            fCurrentOctavaShift = 0;
        }
        
        isProcessingChord = false;
        
        while (pendingPops--) pop();
        
        checkWavyTrillEnd(*this);
        checkLyricEnd (notevisitor::getLyric());
        
        checkTupletEnd(notevisitor::getTuplet());
        checkSlurEnd (notevisitor::getSlur());
        checkBeamEnd (notevisitor::getBeam());
        
        checkGraceEnd(*this);   // This will end GUIDO Grace tag, before any collision with a S_direction
        
        // GUID-126: Do not close texts while a Tuplet is nested inside or expect rendering issues with Tuplet texts.
        if (fTupletOpen <= 0)
        {
            // this is will close any ongoing Guido TEXT tag once a sequence is embedded
            // In case of ongoing \tuplet, do it after the \tuplet is closed! (Potential Guido parser issue)
            checkTextEnd();
        }
        
        checkPostArticulation(*this);
        
        checkDelayed (getDuration(), false);        // check for delayed elements (directions with offset) and indicated before = false
        
        checkOctavaEnd();

        fMeasureEmpty = false;
    }
    
    //______________________________________________________________________________
    // time management
    //______________________________________________________________________________
    void xmlpart2guido::visitStart ( S_divisions& elt )
    {
        fCurrentDivision = (long)(*elt);
    }

bool xmlpart2guido::findNextNote(ctree<xmlelement>::iterator& elt, ctree<xmlelement>::iterator &nextnote) {
    ctree<xmlelement>::iterator nextnotetmp = fCurrentMeasure->find(k_note, elt);
    if (nextnotetmp != fCurrentMeasure->end()) nextnotetmp++;    // advance one step
    while (nextnotetmp != fCurrentMeasure->end()) {
        // looking for the next note on the target voice
        if ((nextnotetmp->getType() == k_note) && (nextnotetmp->getIntValue(k_voice,0) == fTargetVoice)) {
            nextnote = nextnotetmp;
            return true;
        }
        nextnotetmp++;
    }
    return false;
}

bool xmlpart2guido::findNextNote(MusicXML2::xmlelement *elt, MusicXML2::xmlelement *from) {
    ctree<xmlelement>::iterator nextnote = find(fCurrentMeasure->begin(), fCurrentMeasure->end(), from);
    if (nextnote != fCurrentMeasure->end()) nextnote++;    // advance one step
    while (nextnote != fCurrentMeasure->end()) {
        // looking for the next note on the target staff
        if ((nextnote->getType() == k_note) && (nextnote->getIntValue(k_voice,0) == fTargetVoice)) {
            if (elt) {
                elt = *nextnote;
            }
            return true;
        }
        nextnote++;
    }
    return false;

}

void xmlpart2guido::addPositionOrPlacementToNote(const notevisitor& nv, Sxmlelement elt, Sguidoelement& tag, float offset) {
    float default_y = (float)(elt->getAttributeIntValue("default-y", 0));
    if (default_y != 0) {
        float posy = (default_y / 10) * 2;  // convert to half space
        addPosYforNoteHead(nv, posy, tag, offset); // offset to bypass note-head // posy > 0.0 ? 2.0 : 0.0
    }else {
        std::string placement = elt->getAttributeValue("placement");
        if (placement.size() > 0) {
            stringstream s;
            s << "position=\""<<placement<<"\"";
            tag->add (guidoparam::create(s.str(), false));
        }
    }
}

void xmlpart2guido::addDyFromNoteOrStaff(const notevisitor& nv, Sxmlelement elt, Sguidoelement& tag) {
    float default_y = (float)(elt->getAttributeIntValue("default-y", 0));

    if (default_y != 0.0) {
        float noteDistanceFromStaffTop = distanceFromStaffTopForNote(nv);
        // negative distance means we are below the top staff line
        if (noteDistanceFromStaffTop > 0.0) {
            addPosYforNoteHead(nv, (default_y / 10.0) * 2.0, tag, 0.0);
        }else {
            xml2guidovisitor::addPosY(elt, tag, 0.0, 1.0);
        }
        
//        cerr <<"\t addPosYforNoteHead meas:"<<fMeasNum
//            <<" note:"<<nv.getStep()<<nv.getOctave()
//            <<" default_y="<<default_y<<" "<< (default_y / 10) * 2
//            <<" noteDistanceFromStaffTop="<<noteDistanceFromStaffTop
//            <<" -> "<<tag
//        <<endl;
    }
}


void xmlpart2guido::addPosYforNoteHead(const notevisitor& nv, Sxmlelement elt, Sguidoelement& tag, float offset) {
    
    float xmlY = xml2guidovisitor::getYposition(elt, 0, true);
    addPosYforNoteHead(nv, xmlY, tag, offset);

}

void xmlpart2guido::addPosYforNoteHead(const notevisitor& nv, float xmlY, Sguidoelement& tag, float offset) {
    float noteDistanceFromStaffTop = distanceFromStaffTopForNote(nv);
    float posy = xmlY - noteDistanceFromStaffTop + offset ;
    if (posy) {
        stringstream s;
        s << "dy=" << posy << "hs";
        tag->add (guidoparam::create(s.str(), false));
    }
    
//    cerr <<"\t addPosYforNoteHead meas:"<<fMeasNum
//    <<" note:"<<nv.getStep()<<nv.getOctave()
//    <<" xmlY="<<xmlY
//    <<" noteDistanceFromStaffTop="<<noteDistanceFromStaffTop
//    <<" ->pos="<<posy
//    << " "<<tag
//    <<endl;
    
}

float xmlpart2guido::distanceFromStaffTopForNote(const notevisitor& nv) {
    std::string thisClef = getClef(fTargetStaff , fCurrentVoicePosition, fMeasNum);
    if (thisClef.empty()) {
        thisClef = "g";
    }
    float noteHeadDy = nv.getNoteHeadDy(thisClef) + float(-1 * fCurrentOctavaShift * 7);
    /// Notehead placement from top of the staff is (noteheaddy - 10) for G-Clef, and for F-Clef: (2.0 - noteheaddy)
    float noteDistanceFromStaffTop = 0.0;
    if (thisClef[0]=='g') {
        noteDistanceFromStaffTop = (noteHeadDy - 10.0);
    }else if (thisClef[0]=='f') {
        noteDistanceFromStaffTop = noteHeadDy;  //(2.0 - noteHeadDy)
    }else if (thisClef[0]=='c') {
        noteDistanceFromStaffTop = (noteHeadDy - 10.0);
    }
    
    return noteDistanceFromStaffTop;
}


float xmlpart2guido::getNoteDistanceFromStaffTop(const notevisitor& nv) {
    std::string thisClef = getClef(fTargetStaff , fCurrentVoicePosition, fMeasNum);
    if (thisClef.empty()) {
        thisClef = "g";
    }
    float noteHeadDy = nv.getNoteHeadDy(thisClef);
    /// Notehead placement from top of the staff is (noteheaddy - 10) for G-Clef, and for F-Clef: (2.0 - noteheaddy)
    float noteDistanceFromStaffTop = 0.0;
    if (thisClef[0]=='g') {
        noteDistanceFromStaffTop = (noteHeadDy - 10.0);
    }else if (thisClef[0]=='f') {
        noteDistanceFromStaffTop = (2.0 - noteHeadDy);
    }else if (thisClef[0]=='c') {
        noteDistanceFromStaffTop = (noteHeadDy - 10.0);
    }
    
    return noteDistanceFromStaffTop;
}

}

//bool checkTempoMarkup(std::string input) {
//    std::vector<std::string> tempoMarkings = {
//        // Italian terms
//        "andant", "adagi", "a tempo", "agitato", "allegr",
//        "moderato", "largo", "larghetto", "lent", "scherz", "vivace", "vivacissimo", "marcia",
//        // French terms
//        "au mouvement", "grave", "modéré", "vif",
//        // German terms
//        "langsam", "lebhaft", "kräftig", "mässig", "massig", "rasch", "schnell", "bewegt"
//        };
//    
//    // convert input to lowercase
//    std::string victim = input;
//    std::transform(input.begin(), input.end(), victim.begin(), [](unsigned char c){ return std::tolower(c); });
//    
//    vector<string>::const_iterator it_found = find_if(tempoMarkings.begin(), tempoMarkings.end(), [&victim](string s) -> bool {
//        return (victim.find(s) != string::npos);
//    });
//    
//    if (it_found != tempoMarkings.end()) {
//        return true;
//    }else {
//        return false;
//    }
//}
