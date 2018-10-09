/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iostream>
#include <sstream>
#include "notevisitor.h"

//#define PRINTNOTE

using namespace std;

namespace MusicXML2 {


//________________________________________________________________________
ostream& operator<< (ostream& os, const notevisitor& elt)
{
    elt.print(os);
    return os;
}

//________________________________________________________________________
notevisitor::notevisitor() 
	: fInNote(false)
{ 
	reset();
}

//________________________________________________________________________
void notevisitor::reset ()
{
	fGrace = fCue = fChord = fFermata = false;
	fType		= kUndefinedType;
	fTie		= StartStop::undefined;
	fDuration	= 0;
	fDots		= 0;
	fDynamics	= kUndefinedDynamics;
	fStaff		= kUndefinedStaff;
	fVoice		= kUndefinedVoice;		
	fAlter		= 0.f;
	fOctave		= 0;
	fStem		= (void*)0;
	fAccent		= (void*)0;
	fStrongAccent = (void*)0;
	fStaccato	= (void*)0;
    fTenuto		= (void*)0;
    fTrill		= (void*)0;
	fTurn		= (void*)0;
    fTremolo    = (void*)0;
    fInvertedTurn = (void*)0;
    fAccidentalMark = (void*)0;
    fMordent	= (void*)0;
    fNotehead	= (void*)0;
    fInvertedMordent = (void*)0;
	fBreathMark = (void*)0;
    fThisSNote = (void*)0;
	fTimeModification.set(1,1);

	fStep = "";
	fInstrument = "";
	fTied.clear();
	fSlur.clear();
	fBeam.clear();
    fTuplet.clear();
    fWaveLine.clear();
    
    fLyric.clear();
    fSyllabic = "";
    fLyricText="";
    fGraphicType="";
    fAccidental = "";
    fCautionary = "";
}

//________________________________________________________________________
string notevisitor::i2step(int i)
{
	switch (i) {
		case notevisitor::A:	return "A";
		case notevisitor::B:	return "B";
		case notevisitor::C:	return "C";
		case notevisitor::D:	return "D";
		case notevisitor::E:	return "E";
		case notevisitor::F:	return "F";
		case notevisitor::G:	return "G";
	}
	return "";
}

//________________________________________________________________________
int notevisitor::step2i(const std::string& step)
{
	if (step.size() != 1) return -1;
	switch (step[0]) {
		case 'A':	return notevisitor::A;
		case 'B':	return notevisitor::B;
		case 'C':	return notevisitor::C;
		case 'D':	return notevisitor::D;
		case 'E':	return notevisitor::E;
		case 'F':	return notevisitor::F;
		case 'G':	return notevisitor::G;
	}
	return -1;
}

//________________________________________________________________________
float notevisitor::getMidiPitch() const
{
    if (fType == kPitched) {
		int step = step2i(getStep());
		if (step >= 0) {
			short step2pitch [] = { 0, 2, 4, 5, 7, 9, 11 };
			float pitch = (getOctave() * 12.f) + step2pitch[step];
			return pitch + getAlter();
		}
	}
    return -1;
}

//________________________________________________________________________
void notevisitor::visitStart ( S_time_modification& elt )
{
	fTimeModification.set ( elt->getIntValue(k_normal_notes,1), elt->getIntValue(k_actual_notes,1));
}

//________________________________________________________________________
void notevisitor::visitStart ( S_tie& elt )
{
	const string& value = elt->getAttributeValue("type");
	fTie = StartStop::type(fTie | StartStop::xml(value));
/*
	if (value == "start") fTie |= kTieStart;
	else if (value == "stop") fTie |= kTieStop;	
*/
}

//________________________________________________________________________
void notevisitor::print (ostream& out) const
{
		if (isGrace()) out << "grace ";
		if (isCue())   out << "cue ";
		int type = getType();
		if (type == kUndefinedType)
			out << "type undefined";
		else if (type == kUnpitched) {
			out << "unpitched note - duration " << getDuration() << " ";
		}
		else if (type == kRest) {
			out << "rest - duration " << getDuration() << " ";
		}
		else if (type == kPitched) {
			out << "note " << getStep();
			int alter = int(getAlter());
			float diff = getAlter() - alter;
			if (diff >= 0.5) alter++; 
			else if (diff <= -0.5) alter--; 
			while (alter < 0) { out << 'b'; alter++; }
			while (alter > 0) { out << '#'; alter--; }
			out << getOctave() << " (" << getMidiPitch() << ")";
			out << " - duration " << getDuration() << " ";
		}
		else out << "unknown type " <<  type << " ";
		if (inChord())	out << "in chord ";
		if (getTie() & StartStop::start)	out << "- tie start ";
		if (getTie() & StartStop::stop)	out << "- tie stop ";
		string instr = getInstrument();
		if (!instr.empty()) out << "instrument " << instr << " ";
		if (getDynamics() >= 0) out << "dynamics " << getDynamics();
}

    
    //________________________________________________________________________
    void notevisitor::visitStart ( S_lyric& elt )
    {
        fLyric.push_back (elt);
        // get attributes
        float posy = elt->getAttributeFloatValue("default-y", 0) + elt->getAttributeFloatValue("relative-y", 0);
        fLyricsDy = (posy / 10) * 2;   // convert to half spaces
        fLyricsDy += 8;		  // anchor point convertion (defaults to upper line in xml)
        
        /// Get content information:
        fSyllabic = elt->getValue(k_syllabic);
        
        /// Browse inside and take into account elision which translates to "~"
        auto lyrText = elt->find(k_text);
        stringstream lyrStream;
        while (lyrText != elt->end()) {
            lyrStream << lyrText->getValue();
            if (elt->find(k_elision, lyrText) != elt->end()) {
                lyrStream << "~";
            }
            lyrText = elt->find(k_text, lyrText++);
        }        
        fLyricText = lyrStream.str();
    }
    
//________________________________________________________________________
void notevisitor::visitStart ( S_note& elt )
{
	fInNote = true;
	reset();
	fDynamics = elt->getAttributeLongValue("dynamics", kUndefinedDynamics);
    fAccidental = elt->getValue(k_accidental);
    if (!fAccidental.empty()) {
        auto accidental = elt->find(k_accidental);
        fCautionary = accidental->getAttributeValue("cautionary");
    }
    
    fThisSNote = elt;
    x_default = elt->getAttributeIntValue("default-x", -1);
}

//________________________________________________________________________
void notevisitor::visitEnd ( S_note& elt )
{
#ifdef PRINTNOTE
	cerr << *this << endl;
#endif
}
    
    //________________________________________________________________________
    std::string notevisitor::getNoteheadType () const
    {
        if (fNotehead)
        {
            std::stringstream noteHeadGuidoType;
            
            if (fNotehead->getAttributeValue("parantheses")=="yes")
            {
                noteHeadGuidoType << "(";
            }
            
            std::string noteHeadXML = fNotehead->getValue();
            if (noteHeadXML == "diamond")
                noteHeadGuidoType << "diamond";
            else if (noteHeadXML == "inverted triangle")
                noteHeadGuidoType << "reversedTriangle";
            else if (noteHeadXML == "x")
                noteHeadGuidoType << "x";
            else if (noteHeadXML == "triangle")
                noteHeadGuidoType << "triangle";
            else if (noteHeadXML == "square")
                noteHeadGuidoType << "square";
            else if (noteHeadXML == "square")
                noteHeadGuidoType << "square";
            else
                noteHeadGuidoType << "";
            
            if (fNotehead->getAttributeValue("parantheses")=="yes")
            {
                noteHeadGuidoType << ")";
            }
            
            return noteHeadGuidoType.str();
        }else
            return "";
    }
    
    //________________________________________________________________________
    float notevisitor::getRestFormatDy ( string fCurClef ) const
    {
        float restFormatDy = 0.0;
        //string display_step = elt->getValue(k_display_step);
        //int display_octave = elt->getIntValue(k_display_octave, 0.0);
        string display_step = fStep;
        int display_octave = fOctave;
        
        // Now convert to half-space! We'd need the Current Clef info for this!
        if (display_octave)
        {
            // dy 0 is B4, dy=-6 for C4, -4 for E4, -2 for G4 ;
            // D5-> +2, F5 -> +4
            switch (display_step[0]) {
                case 'A':
                    switch (display_octave) {
                        case 4:
                            if (fCurClef == "G")
                                restFormatDy = -1.0;
                            else if (fCurClef == "F")
                                restFormatDy = 11.0;
                            break;
                        case 5:
                            restFormatDy = 6.0;
                            break;
                            
                        case 3:
                            if (fCurClef == "G")
                                restFormatDy = -8.0;
                            else if (fCurClef == "F")
                                restFormatDy = 4.0;
                            break;
                            
                        case 2:
                            if (fCurClef == "G")
                                restFormatDy = -15.0;
                            else if (fCurClef == "F")
                                restFormatDy = -3.0;
                            break;
                        
                        // treating F clef directly!
                        case 1:
                            restFormatDy = -10.0;
                            break;
                            
                        default:
                            restFormatDy = 0.0;
                            break;
                    }
                    break;
                    
                case 'B':
                    switch (display_octave) {
                        case 4:
                            if (fCurClef == "G")
                                restFormatDy = 0.0;
                            else if (fCurClef == "F")
                                restFormatDy = 12.0;
                            
                            break;
                        case 5:
                            restFormatDy = 7.0;
                            break;
                            
                        case 3:
                            if (fCurClef == "G")
                                restFormatDy = -7.0;
                            else if (fCurClef == "F")
                                restFormatDy = 5.0;
                            break;
                            
                        case 2:
                            if (fCurClef == "G")
                                restFormatDy = -14.0;
                            else if (fCurClef == "F")
                                restFormatDy = -2.0;
                            break;
                        // F clef:
                        case 1:
                            restFormatDy = -9.0;
                            break;
                            
                        default:
                            restFormatDy = 0.0;
                            break;
                    }
                    break;
                case 'C':
                    switch (display_octave) {
                        case 4:
                            if (fCurClef == "G")
                                restFormatDy = -6.0;
                            else if (fCurClef == "F")
                                restFormatDy = 6.0;
                            break;
                        case 5:
                            if (fCurClef == "G")
                                restFormatDy = 1.0;
                            else if (fCurClef == "F")
                                restFormatDy = 13.0;
                            break;
                        case 6:
                            restFormatDy = 8.0;
                            break;
                        case 3:
                            if (fCurClef == "G")
                                restFormatDy = -13.0;
                            else if (fCurClef == "F")
                                restFormatDy = -1.0;
                            break;
                        case 2:
                            if (fCurClef == "G")
                                restFormatDy = -20.0;
                            else if (fCurClef == "F")
                                restFormatDy = -8.0;
                            break;
                        // F clef:
                        default:
                            restFormatDy = 0.0;
                            break;
                    }
                    break;
                    
                case 'D':
                    switch (display_octave) {
                        case 4:
                            if (fCurClef == "G")
                                restFormatDy = -5.0;
                            else if (fCurClef == "F")
                                restFormatDy = 7.0;
                            break;
                        case 5:
                            restFormatDy = 2.0;
                            break;
                        case 6:
                            restFormatDy = 9.0;
                            break;
                        case 3:
                            if (fCurClef == "G")
                                restFormatDy = -12.0;
                            else if (fCurClef == "F")
                                restFormatDy = 0.0;
                            break;
                        case 2:
                            if (fCurClef == "G")
                                restFormatDy = -19.0;
                            else if (fCurClef == "F")
                                restFormatDy = -7.0;
                            break;
                        // F clef:
                            
                        default:
                            restFormatDy = 0.0;
                            break;
                    }
                    break;
                case 'E':
                    switch (display_octave) {
                        case 4:
                            if (fCurClef == "G")
                                restFormatDy = -4.0;
                            else if (fCurClef == "F")
                                restFormatDy = 8.0;
                            break;
                        case 5:
                            restFormatDy = 3.0;
                            break;
                        case 6:
                            restFormatDy = 10.0;
                            break;
                        case 3:
                            if (fCurClef == "G")
                                restFormatDy = -11.0;
                            else if (fCurClef == "F")
                                restFormatDy = 1.0;
                            break;
                        case 2:
                            if (fCurClef == "G")
                                restFormatDy = -18.0;
                            else if (fCurClef == "F")
                                restFormatDy = -6.0;
                            break;
                        // F clef:
                        default:
                            restFormatDy = 0.0;
                            break;
                    }
                    break;
                case 'F':
                    switch (display_octave) {
                        case 4:
                            if (fCurClef == "G")
                                restFormatDy = -3.0;
                            else if (fCurClef == "F")
                                restFormatDy = 9.0;
                            break;
                        case 5:
                            restFormatDy = 4.0;
                            break;
                        case 3:
                            if (fCurClef == "G")
                                restFormatDy = -10.0;
                            else if (fCurClef == "F")
                                restFormatDy = 2.0;
                            break;
                        case 2:
                            if (fCurClef == "G")
                                restFormatDy = -17.0;
                            else if (fCurClef == "F")
                                restFormatDy = -5.0;
                            break;
                        // F clef:
                            
                        default:
                            restFormatDy = 0.0;
                            break;
                    }
                    break;
                case 'G':
                    switch (display_octave) {
                        // G clef:
                        case 4:
                            if (fCurClef == "G")
                                restFormatDy = -2.0;
                            else if (fCurClef == "F")
                                restFormatDy = 10.0;
                            break;
                        case 5:
                            restFormatDy = 5.0;
                            break;
                        case 3:
                            if (fCurClef == "G")
                                restFormatDy = -9.0;
                            else if (fCurClef == "F")
                                restFormatDy = 3.0;
                            break;
                        case 2:
                            if (fCurClef == "G")
                                restFormatDy = -22.0;
                            else if (fCurClef == "F")
                                restFormatDy = -4.0;
                            break;
                        // F clef:
                        default:
                            restFormatDy = 0.0;
                            break;
                    }
                    break;
                    
                default:
                    break;
            }
            
            // Got it reversed for F clef
            restFormatDy = -1.0 * restFormatDy;
        }
        
        return restFormatDy;
    }

}

