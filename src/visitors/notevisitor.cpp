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
	fGrace = fCue = fChord = false;
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
    fMordent    = (void*)0;
    fArpeggio	= (void*)0;
    fNotehead	= (void*)0;
    fInvertedMordent = (void*)0;
	fBreathMark = (void*)0;
    fThisSNote = (void*)0;
    fFermata = (void*)0;
	fTimeModification.set(1,1);

	fStep = "";
	fInstrument = "";
	fTied.clear();
	fSlur.clear();
	fBeam.clear();
    fTuplet.clear();
    fWaveLine.clear();
    fFingering.clear();
    
    fLyric.clear();
    fSyllabic = "";
    fLyricText="";
    fGraphicType="";
    fAccidental = "";
    fCautionary = "";
    fBowUp = (void*)0;
    fBowDown = (void*)0;
    fHarmonic = (void*)0;
    fSnapPizzicato = (void*)0;
    fStaccatissimo = (void*)0;
    shouldPrint = true;
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
    
    if (elt->getAttributeValue("print-object")=="no") {
        shouldPrint = false;
    }else {
        shouldPrint = true;
    }
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

float notevisitor::getNoteHeadDy( string fCurClef ) const
{
    float offset = 0.0;
    string display_step = fStep;
    int display_octave = fOctave;
    
    /// Compensate for display octave +8/-8 from clef
    if (fCurClef.find("+8")!= std::string::npos) {
        display_octave--;   // on +8, reduce the octave to reflect correct display?!
    }
    if (fCurClef.find("-8")!= std::string::npos) {
        display_octave++;
    }
    
    /// Compensate for clef line
    // For C-clef: C3=0, otherwise it is the difference *(2)
    // For others, zero is F4 and g2
    int kLine = atoi(&fCurClef[1]);
    switch (fCurClef[0]) {
        case 'g':
            offset = (2-kLine)*(-2.0);
            break;
        case 'c':
            offset = (3-kLine)*(-2.0);
            break;
        case 'f':
            offset = (4-kLine)*(-2.0);
            break;
        default:
            break;
    }
    
    //cerr<<"\t\t getNoteHeadDy withClef=\""<<fCurClef<<"\" octave:"<<display_octave<<" step:\""<<display_step<<"\""<< " kLine="<<kLine<<endl;
    // IN G CLEF: b3=-1, c4=0, D4=1, e4=2, F4=3, g4=4, A4=5, b4=6, c5=7, d5=8, f5=10, g5=11, a5=12 -> Octave is a cycle of 7
    // IN F CLEF: e4=2 , d4=1, c4=0, b3=-1, a3=-2, g3=-3, f3=-4, d3=-6, b2=-8, g2=-10 (same as G clef)
    // C CLEF: c4=6, d4=7, e4=8, f4=9, g4=10, a4=11, b4=12, c5=13
    float base_distance = (float(display_octave) - 4)*7;
    if ((fCurClef[0] == 'g') || (fCurClef[0] == 'f')) {
        switch (display_step[0]) {
            case 'C':
                return base_distance + 0 + offset;
            case 'D':
                return base_distance + 1 + offset;
            case 'E':
                return base_distance + 2 + offset;
            case 'F':
                return base_distance + 3 + offset;
            case 'G':
                return base_distance + 4 + offset;
            case 'A':
                return base_distance + 5 + offset;
            case 'B':
                return base_distance + 6 + offset;
        }
    }
    
    if (fCurClef[0] == 'c') {
        switch (display_step[0]) {
            case 'C':
                return base_distance + 6 + offset;
            case 'D':
                return base_distance + 7 + offset;
            case 'E':
                return base_distance + 8 + offset;
            case 'F':
                return base_distance + 9 + offset;
            case 'G':
                return base_distance + 10 + offset;
            case 'A':
                return base_distance + 11 + offset;
            case 'B':
                return base_distance + 12 + offset;
        }
    }
    return offset;
}

}

