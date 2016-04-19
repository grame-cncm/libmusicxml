/*
  Copyright (C) 2013 Thomas Coffy

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  thomas.coffy@gmail.com
  http://repmus.ircam.fr/antescofo
*/

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "conversions.h"
#include "partsummary.h"
#include "rational.h"
#include "xml2antescofovisitor.h"
#include "xmlpart2antescofo.h"
#include "xml_tree_browser.h"

using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
xmlpart2antescofo::xmlpart2antescofo(antescofowriter& _w, bool generateComments, bool
		generateStem, bool generateBar) :
	fGenerateComments(generateComments), fGenerateStem(generateStem), 
	fGenerateBars(generateBar),
	fNotesOnly(false), fCurrentStaffIndex(0), fCurrentStaff(0),
	fTargetStaff(0), fTargetVoice(0), w(_w)
{
	fGeneratePositions = true;
	fRepeatForward = fRepeatBackward = false;
	xmlpart2antescofo::reset();
}

//______________________________________________________________________________
void xmlpart2antescofo::reset ()
{
	antescofonotestatus::resetall();
	fCurrentBeamNumber = 0;
	fInCue = fInGrace = fInhibitNextBar = fPendingBar
		   = fBeamOpened = fCrescPending = fSkipDirection = false;
	fCurrentStemDirection = kStemUndefined;
	fCurrentDivision = 1;
	fCurrentOffset = 0;
	fPendingPops = 0;
	fMeasNum = 0;
	fCurBeat = rational(1);
	fLastDur = 0;
	fTrill = fGlissandoStart = fGlissandoStop = fInBackup = fInForward = false;
}

//______________________________________________________________________________
void xmlpart2antescofo::initialize (Santescofoelement seq, int staff, int antescofostaff, int voice, 
		bool notesonly, rational defaultTimeSign) 
{
	fCurrentStaff = fTargetStaff = staff;	// the current and target staff
	fTargetVoice = voice;			// the target voice
	fNotesOnly = notesonly;			// prevent multiple output for keys, clefs etc... 
	fCurrentTimeSign = defaultTimeSign;	// a default time signature
	fCurrentStaffIndex = antescofostaff;	// the current antescofo staff index
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
void xmlpart2antescofo::addDelayed (Santescofoelement elt, long offset) 
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
void xmlpart2antescofo::checkDelayed (long time)
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
void xmlpart2antescofo::stackClean () 
{
	if (fInCue) {
		fInCue = false;
	}
	if (fInGrace) {
		fInGrace = false;
	}
}

//______________________________________________________________________________
void xmlpart2antescofo::checkStaff (int staff) {
    if (staff != fCurrentStaff) {
		int offset = staff - fCurrentStaff;
//cout << "move from staff " << fCurrentStaffIndex << " to " << (fCurrentStaffIndex + offset) << endl;
		fCurrentStaff = staff;
        fCurrentStaffIndex += offset;
    }
}

//______________________________________________________________________________
void xmlpart2antescofo::moveMeasureTime (rational duration, bool moveVoiceToo)
{
  //cout << "moveMeasureTime: fCurBeat:"<<fCurBeat.getNumerator() <<"/"<< fCurBeat.getDenominator()<<" dur:"<< duration.toFloat() << " moveVoiceToo:"<<moveVoiceToo<<endl;
	rational r = duration;
	r /= fCurrentDivision;
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
void xmlpart2antescofo::checkVoiceTime ( const rational& currTime, const rational& voiceTime)
{
	rational diff = currTime - voiceTime;
	diff.rationalise();
	if (diff.getNumerator() > 0) {
		cout << "checkVoiceTime: adding rest note dur:"<<diff.getNumerator() << "/"<<diff.getDenominator() << endl;
		w.AddNote(ANTESCOFO_REST, 0, diff, fMeasNum, fCurBeat, 0);
		fCurrentVoicePosition += diff;
		fCurrentVoicePosition.rationalise();
	}
	else if (diff.getNumerator() < 0)
		cerr << "warning! checkVoiceTime: measure time behind voice time " << string(diff) << endl;
}

//______________________________________________________________________________
void xmlpart2antescofo::visitStart ( S_backup& elt )
{
	fInBackup = true;
	stackClean();	// closes pending chords, cue and grace
	int duration = elt->getIntValue(k_duration, 0);
	cout << "BACKUP ----------------< " << duration <<  " fCurBeat:" << fCurBeat.toFloat() << endl;

	if (duration) {
		// backup is supposed to be used only for moving between voices
		// thus we don't move the voice time (which is supposed to be 0)

		bool scanElement = (elt->getIntValue(k_voice, 0) == fTargetVoice) && (elt->getIntValue(k_staff, 0) == fTargetStaff);
		moveMeasureTime (rational(-duration), false);
		if (scanElement) 
		{
			//fCurBeat -= rational(duration, fCurrentDivision);
			cout << "BACKUP -----> going back of "<<noteDuration(*this).toFloat() << endl;
			fCurBeat -= noteDuration(*this); 
			cout << "BACKUP ---------------- fCurBeat:" << fCurBeat.toFloat() << endl;
			if (fCurBeat.toFloat() <= 0)
				fCurBeat = rational(1);
		}
	}
}

//______________________________________________________________________________
void xmlpart2antescofo::visitStart ( S_forward& elt )
{
	fInForward = true;
	bool scanElement = (elt->getIntValue(k_voice, 0) == fTargetVoice)
		&& (elt->getIntValue(k_staff, 0) == fTargetStaff);
	int duration = elt->getIntValue(k_duration, 0);
	cout << "FORWARD("<<scanElement<<") ----------------> " << rational(duration, fCurrentDivision).toFloat() << endl;

	if (fCurBeat.toFloat() == 1 && fMeasNum > 1)
		moveMeasureTime(rational(duration), true);
	else 
		moveMeasureTime(rational(duration),/*false*/ scanElement);
	if (!scanElement) return;


	if (duration) {		
		rational r = noteDuration(*this);
		r.rationalise();
		cout << "forward: adding rest, maybe wrong?" << endl;
		w.AddNote(ANTESCOFO_REST, 0, r, fMeasNum, fCurBeat, 0);
		fMeasureEmpty = false;
	}

	if (scanElement) {
		fCurBeat += rational(duration, fCurrentDivision);
		fCurBeat.rationalise();
	}
}

//______________________________________________________________________________
void xmlpart2antescofo::visitStart ( S_part& elt ) 
{
	if (!current()) {
    cout << endl << "--------------------------------- visit start S_part " << endl;
		reset();
		Santescofoelement seq = antescofoseq::create();
		start (seq);
	}
}

bool is_number(const std::string& s)
{
        std::string::const_iterator it = s.begin();
        while (it != s.end() && isdigit(*it)) ++it;
        return !s.empty() && it == s.end();
}

//______________________________________________________________________________
void xmlpart2antescofo::visitStart ( S_measure& elt ) 
{
	fRehearsals = "";
	const string& implicit = elt->getAttributeValue ("implicit");
	if (implicit == "yes") fPendingBar = false;
	if (fPendingBar) {
		// before adding a bar, we need to check that there are no repeat begin at this location
		ctree<xmlelement>::iterator repeat = elt->find(k_repeat);
		if ((repeat == elt->end()) || (repeat->getAttributeValue("direction") != "forward")) {
			checkStaff (fTargetStaff);
		}
	}
	fCurrentMeasure = elt;
	fMeasNum++;

	int number_attr = 0;
	string n = elt->getAttributeValue("number");

	if (!n.empty() && is_number(n)) number_attr = atoi(n.c_str());

	if (number_attr && number_attr != fMeasNum) fMeasNum = number_attr;
	fCurrentMeasureLength.set  (0, 1);
	fCurrentMeasurePosition.set(0, 1);
	fCurrentVoicePosition.set  (0, 1);
	fInhibitNextBar = false; // fNotesOnly;
	fPendingBar = false;
	fPendingPops = 0;
	fMeasureEmpty = true;

	cout << "--------------------------------- visit start S_measure: fMeasNum:"<<fMeasNum << " fCurBeat:"<< fCurBeat.toFloat() << endl;
}
        

//______________________________________________________________________________
void xmlpart2antescofo::visitEnd ( S_measure& elt ) 
{
	cout << "--------------------------------- visit end S_measure: fCurBeat:"<<fCurBeat.toFloat() << endl;

	cout << "fCurrentMeasureLength: "<<fCurrentMeasureLength.toFloat() << " fCurBeat:"<< fCurBeat.toFloat() << " "<< endl;// << " d:"  << fCurrentMeasureLength.toFloat() - fCurBeat << endl;
	checkVoiceTime (fCurrentMeasureLength, fCurrentVoicePosition);

	if (!fInhibitNextBar) {
		if (fGenerateBars) fPendingBar = true;
		else if (!fMeasureEmpty) {
			if (fCurrentVoicePosition < fCurrentMeasureLength)
				fPendingBar = true;
		}
	}

	bool scanElement = (elt->getIntValue(k_voice, 0) == fTargetVoice) && (elt->getIntValue(k_staff, 0) == fTargetStaff);
	if (!scanElement) {
		rational d(fCurrentMeasureLength.toFloat(), fCurrentDivision); d.rationalise();
		//rational d = noteDuration(*this);
		cout << "visitEnd S_measure: add fCurBeat:" << fCurBeat.toFloat() << " : " << d.toFloat() << endl;
		fCurBeat += d;
		fLastDur = d;
	}
}


//______________________________________________________________________________
void xmlpart2antescofo::visitStart ( S_rehearsal& elt ) 
{
	cout << "rehearsal:"<< elt->getValue() << endl;
	fRehearsals = elt->getValue();
}

//______________________________________________________________________________
void xmlpart2antescofo::visitStart ( S_direction& elt ) 
{
	if (fNotesOnly || (elt->getIntValue(k_staff, 0) != fTargetStaff)) {
		fSkipDirection = true;
	}
	else {
		fCurrentOffset = elt->getLongValue(k_offset, 0);
	}
}

//______________________________________________________________________________
void xmlpart2antescofo::visitEnd ( S_direction& elt ) 
{
	fSkipDirection = false;
	fCurrentOffset = 0;
}

//______________________________________________________________________________
void xmlpart2antescofo::visitEnd ( S_key& elt ) 
{
	if (fNotesOnly) return;
	fFifths = keysignvisitor::fFifths;
}

//______________________________________________________________________________
void xmlpart2antescofo::visitStart ( S_coda& elt )
{
}

//______________________________________________________________________________
void xmlpart2antescofo::visitStart ( S_segno& elt )
{
}

//______________________________________________________________________________
void xmlpart2antescofo::visitStart ( S_wedge& elt )
{
	if (fSkipDirection) return;

	string type = elt->getAttributeValue("type");
	if (type == "crescendo") {
		fCrescPending = true;
	}
	else if (type == "diminuendo") {
		fCrescPending = false;
	}
	else if (type == "stop") {
	}

}

//______________________________________________________________________________
void xmlpart2antescofo::visitEnd ( S_metronome& elt ) 
{
	if (fSkipDirection) return;

	metronomevisitor::visitEnd (elt);
	if (fBeats.size() != 1) return;					// support per minute tempo only (for now)
	if (!metronomevisitor::fPerMinute) return;		// support per minute tempo only (for now)

	beat b = fBeats[0];
	rational r = NoteType::type2rational(NoteType::xml(b.fUnit)), rdot(3,2);
	while (b.fDots-- > 0) {
		r *= rdot;
	}
	r.rationalise();

	stringstream s;
	s << metronomevisitor::fPerMinute;
	string str;
	s >> str;
	w.setBPM(str);
	cout << "xmlpart2antescofo : got metronome : BPM: "<< metronomevisitor::fPerMinute<< endl;
}

//______________________________________________________________________________
void xmlpart2antescofo::visitStart( S_dynamics& elt)
{
	if (fSkipDirection) return;

	ctree<xmlelement>::literator iter;
	for (iter = elt->lbegin(); iter != elt->lend(); iter++) {
		if ((*iter)->getType() != k_other_dynamics) {
		}
	}
}

//______________________________________________________________________________
void xmlpart2antescofo::visitStart( S_octave_shift& elt)
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

}

//______________________________________________________________________________
void xmlpart2antescofo::visitStart ( S_note& elt ) 
{
	notevisitor::visitStart ( elt );
}

//______________________________________________________________________________
string xmlpart2antescofo::alter2accident ( float alter ) 
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
void xmlpart2antescofo::visitEnd ( S_sound& elt )
{
	if (fNotesOnly) return;

	Sxmlattribute attribute;
	
	/*if ((attribute = elt->getAttribute("dacapo")))
	else {
		if ((attribute = elt->getAttribute("dalsegno"))) {
		}
		else if ((attribute = elt->getAttribute("tocoda"))) {
		}
		else if ((attribute = elt->getAttribute("fine"))) {
		} else if ((attribute = elt->getAttribute("tempo"))) {
            cout << "xmlpart2antescofo : got sound tempo : BPM: "<< attribute->getValue() << endl;
            w.setBPM(attribute->getValue());
        }
	}*/
}

//______________________________________________________________________________
void xmlpart2antescofo::visitEnd ( S_ending& elt )
{
	string type = elt->getAttributeValue("type");
	if (type == "start") {
		string num = elt->getAttributeValue ("number");
	}
	else {
	}
}

//______________________________________________________________________________
void xmlpart2antescofo::visitEnd ( S_repeat& elt ) 
{
	string direction = elt->getAttributeValue("direction");
	if (direction == "forward") 
		fRepeatForward = true;
	else if (direction == "backward") {
		fRepeatBackward = true;
	}
	cout << "visitEnd: direction: repeat: "<< direction << endl;
}

//______________________________________________________________________________
void xmlpart2antescofo::visitStart ( S_barline& elt ) 
{
	const string& location = elt->getAttributeValue("location");
	if (location == "middle") {
		// todo: handling bar-style (not yet supported in antescofo)
	}
	// todo: support for left and right bars
	// currently automatically handled at measure boundaries
	else if (location == "right") {
	}
	else if (location == "left") {
	}
}

//______________________________________________________________________________
void xmlpart2antescofo::visitEnd ( S_time& elt ) 
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
}

//______________________________________________________________________________
void xmlpart2antescofo::visitEnd ( S_clef& elt ) 
{
	int staffnum = elt->getAttributeIntValue("number", 0);
	if ((staffnum != fTargetStaff) || fNotesOnly) return;

	stringstream s; 
	if ( clefvisitor::fSign == "G")		s << "g";
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
	checkStaff (staffnum);
}

//______________________________________________________________________________
// tools and methods for converting notes
//______________________________________________________________________________
vector<S_slur>::const_iterator xmlpart2antescofo::findTypeValue ( const std::vector<S_slur>& slurs, const string& val ) const 
{
	std::vector<S_slur>::const_iterator i;
	for (i = slurs.begin(); i != slurs.end(); i++) {
		if ((*i)->getAttributeValue("type") == val) break;
	}
	return i;
}

//______________________________________________________________________________
vector<S_tied>::const_iterator xmlpart2antescofo::findTypeValue ( const std::vector<S_tied>& tied, const string& val ) const 
{
	std::vector<S_tied>::const_iterator i;
	for (i = tied.begin(); i != tied.end(); i++) {
		if ((*i)->getAttributeValue("type") == val) break;
	}
	return i;
}

//______________________________________________________________________________
vector<S_beam>::const_iterator xmlpart2antescofo::findValue ( const std::vector<S_beam>& beams, const string& val ) const 
{
	std::vector<S_beam>::const_iterator i;
	for (i = beams.begin(); i != beams.end(); i++) {
		if ((*i)->getValue() == val) break;
	}
	return i;
}

//______________________________________________________________________________
bool xmlpart2antescofo::checkTiedBegin ( const std::vector<S_tied>& tied )
{
	std::vector<S_tied>::const_iterator i = findTypeValue(tied, "start");
	bool r = false;
	if (i != tied.end()) {
		cout << "got start Tied"<<endl;
		r = true;
	}
	return r;
}

bool xmlpart2antescofo::checkTiedEnd ( const std::vector<S_tied>& tied )
{
	std::vector<S_tied>::const_iterator i = findTypeValue(tied, "stop");
	bool r = false;

	if (i != tied.end()) {
		cout << "got end Tied"<<endl;
		r = true;
	}
	return r;
}

//______________________________________________________________________________
void xmlpart2antescofo::checkSlurBegin ( const std::vector<S_slur>& slurs ) 
{
	std::vector<S_slur>::const_iterator i = findTypeValue(slurs, "start");
	if (i != slurs.end()) {
	}
}

void xmlpart2antescofo::checkSlurEnd ( const std::vector<S_slur>& slurs ) 
{
	std::vector<S_slur>::const_iterator i = findTypeValue(slurs, "stop");
	if (i != slurs.end()) {
	}
}

//______________________________________________________________________________
void xmlpart2antescofo::checkBeamBegin ( const std::vector<S_beam>& beams ) 
{
	std::vector<S_beam>::const_iterator i = findValue(beams, "begin");
	if (i != beams.end()) {
		if (!fBeamOpened ) {
			fCurrentBeamNumber = (*i)->getAttributeIntValue("number", 1);
			fBeamOpened = true;
		}
	}
}

void xmlpart2antescofo::checkBeamEnd ( const std::vector<S_beam>& beams ) 
{
	std::vector<S_beam>::const_iterator i;
	for (i = beams.begin(); (i != beams.end()) && fBeamOpened; i++) {
		if (((*i)->getValue() == "end") && ((*i)->getAttributeIntValue("number", 1) == fCurrentBeamNumber)) {
			fCurrentBeamNumber = 0;
			pop();
			fBeamOpened = false;
		}
	}
}

//______________________________________________________________________________
void xmlpart2antescofo::checkStem ( const S_stem& stem ) 
{
	if (stem) {
		if (stem->getValue() == "down") {
			if (fCurrentStemDirection != kStemDown) {
				fCurrentStemDirection = kStemDown;
			}
		}
		else if (stem->getValue() == "up") {
			if (fCurrentStemDirection != kStemUp) {
				fCurrentStemDirection = kStemUp;
			}
		}
		else if (stem->getValue() == "none") {
			if (fCurrentStemDirection != kStemNone) {
				fCurrentStemDirection = kStemNone;
			}
		}
		else if (stem->getValue() == "double") {
		}
	}
	else if (fCurrentStemDirection != kStemUndefined) {
		fCurrentStemDirection = kStemUndefined;
	}
}

//______________________________________________________________________________
int xmlpart2antescofo::checkArticulation ( const notevisitor& note ) 
{
	int n = 0;
	if (note.fAccent) {
		n++;
	}
	if (note.fStrongAccent) {
		n++;
	}
	if (note.fStaccato) {
		n++;
	}
	if (note.fTenuto) {
		n++;
	}
	return n;
}

//______________________________________________________________________________
vector<Sxmlelement> xmlpart2antescofo::getChord ( const S_note& elt ) 
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
void xmlpart2antescofo::checkCue (const notevisitor& nv) 
{
	if (nv.isCue()) {
		if (!fInCue) {
			fInCue = true;
		}
	}
	else if (fInCue) {
		fInCue = false;
		pop();			
	}
}

//______________________________________________________________________________
void xmlpart2antescofo::checkGrace (const notevisitor& nv) 
{
	if (nv.isGrace()) {
		if (!fInGrace) {
			fInGrace = true;
		}
	}
	else if (fInGrace) {
		fInGrace = false;
	}
}

//______________________________________________________________________________
int xmlpart2antescofo::checkFermata (const notevisitor& nv) 
{
	if (nv.inFermata()) {
		return 1;
	}
	return 0;
}

//________________________________________________________________________
int xmlpart2antescofo::step2i(const std::string& step) const
{
	if (step.size() != 1) return -1;
	switch (step[0]) {
		case 'A':	return xmlpart2antescofo::A;
		case 'B':	return xmlpart2antescofo::B;
		case 'C':	return xmlpart2antescofo::C;
		case 'D':	return xmlpart2antescofo::D;
		case 'E':	return xmlpart2antescofo::E;
		case 'F':	return xmlpart2antescofo::F;
		case 'G':	return xmlpart2antescofo::G;
	}
	return -1;
}

//________________________________________________________________________
float xmlpart2antescofo::getMidiPitch(const notevisitor& nv) const
{
	int step = step2i(nv.getStep());
	if (step >= 0) {
		short step2pitch [] = { 0, 2, 4, 5, 7, 9, 11 };
		float pitch = ((nv.getOctave() + 1) * 12.f) + step2pitch[step];
		return pitch + nv.getAlter();
	}
	return -1;
}



//______________________________________________________________________________
string xmlpart2antescofo::noteName ( const notevisitor& nv )
{
	//cout << "================== getalter: " << nv.getAlter() << endl;;
	string accident = alter2accident(nv.getAlter());
	string name;
	if (nv.getType() == notevisitor::kRest)
		name="_";
	else {
		name = nv.getStep();
		if (!name.empty()) name[0]=tolower(name[0]);
		else cerr << "warning: empty note name" << endl;
	}

	//cout << "================== getalter name:: " << name << endl;;
	return name;
}

//______________________________________________________________________________
rational xmlpart2antescofo::noteDuration ( const notevisitor& nv )
{
	rational dur(0);
	rational r = NoteType::type2rational(NoteType::xml(nv.getGraphicType()));
	if (r.getNumerator() == 0) { // graphic type missing or unknown
		r.set (nv.getDuration(), fCurrentDivision);
		if (nv.getType() == kRest)
			r /= 4;
	}
	r.rationalise();
	rational tm = nv.getTimeModification();
	r *= tm;
	r.rationalise();
	dur.set (r.getNumerator()*4, r.getDenominator());
	int dots = nv.getDots();
	if (dots) 
		dur += (dur * dots) / 2;
	dur.rationalise();

	return dur;
}
    
    
bool xmlpart2antescofo::checkNotation( S_note& elt )
{
	fTrill = fGlissandoStart = fGlissandoStop = false;
	ctree<xmlelement>::iterator next;
	for (ctree<xmlelement>::iterator i = elt->begin(); i != elt->end(); ) {
		next = i;
		next++;
		switch (i->getType()) {                
			case k_notations:
				//case k_lyric:
				for (ctree<xmlelement>::iterator j = elt->begin(); j != elt->end(); j++) {
					switch (j->getType()) {
						case k_ornaments:
							for (ctree<xmlelement>::iterator k = elt->begin(); k != elt->end(); k++) {
								switch (k->getType()) {
									case k_trill_mark:
									case k_tremolo:
										fTrill = true;
										return true;
								}
							}
							//break;
						case k_glissando:
							if (j->getAttributeValue("type") == "start") fGlissandoStart = true;
							if (j->getAttributeValue("type") == "stop") fGlissandoStop = true;
							return true;
						case k_slide:
							if (j->getAttributeValue("type") == "start") fGlissandoStart = true;
							if (j->getAttributeValue("type") == "stop") fGlissandoStop = true;
							return true;
					}
				}
				break;
		}
		i = next;
	}
	return false;
}

//______________________________________________________________________________
void xmlpart2antescofo::visitStart ( S_duration& elt )
{
	notevisitor::visitStart( elt );

	bool scanElement = (elt->getIntValue(k_voice, 0) == fTargetVoice) && (elt->getIntValue(k_staff, 0) == fTargetStaff);	
	if (!scanElement) return;

	stringstream s;
	s << elt->getValue();
	int duration;
	s >> duration;
	if (duration) {
		rational r(duration, fCurrentDivision);
		r.rationalise();
		if (fInBackup) {
			cout << "Backup duration : removing "<< r.toFloat()<<" to fCurBeat:"<<fCurBeat.toFloat() << endl;
			fCurBeat -= r;
			cout << "Backup duration : new fCurBeat:"<<fCurBeat.toFloat() << endl;
		}
		else if (fInForward) {
			cout << "Forward duration : adding "<< r.toFloat()<<" to fCurBeat:"<< fCurBeat.toFloat() << endl;
			fCurBeat += r;
			cout << "Forward duration : new fCurBeat:"<<fCurBeat.toFloat() << endl;
		}
	}
	if (fCurBeat.toFloat() <= 0) fCurBeat = rational(1);
}

// check we want to convert this measure
bool xmlpart2antescofo::checkWriteMeasure()
{
		bool bad = w.write_measures.empty() ? false : true;
		for (vector<int>::const_iterator v = w.write_measures.begin(); v != w.write_measures.end(); v++) {
			if (*v == fMeasNum)
				bad = false;
		}
		return !bad;
}

//______________________________________________________________________________
void xmlpart2antescofo::newNote ( const notevisitor& nv,  S_note& elt  )
{
	checkNotation(elt);

	bool tiedStart = checkTiedBegin (nv.getTied());
	bool tiedEnd = checkTiedEnd (nv.getTied());

	rational d = noteDuration(nv);

	int flag = ANTESCOFO_FLAG_NULL;
	if (tiedStart)          flag = ANTESCOFO_FLAG_TIED_START;
	if (tiedEnd)            flag = ANTESCOFO_FLAG_TIED_END;
	if (fGlissandoStart)    flag = ANTESCOFO_FLAG_GLISSANDO_START;
	if (fGlissandoStop)     flag = ANTESCOFO_FLAG_GLISSANDO_STOP;
	if (checkFermata(nv))   flag = ANTESCOFO_FLAG_FERMATA;
	if (fRepeatForward)     flag = ANTESCOFO_FLAG_REPEAT_FORWARD;
	if (fRepeatBackward)    flag = ANTESCOFO_FLAG_REPEAT_BACKWARD;

	// check we want to convert this staff
	bool badstaff = w.write_staves.empty() ? false : true;
	for (vector<string>::const_iterator v = w.write_staves.begin(); v != w.write_staves.end(); v++) {
		stringstream ss;
		ss << fCurrentStaff;
		if (*v == ss.str()) {
			badstaff = false;
			cout << " ------------------------------------------ WARNING not converting this staff: " << *v << endl;
		}
	}
	if (badstaff) {
		return;
	}

	assert(fCurBeat.toFloat() > 0);
	fCurBeat.rationalise();
	if (nv.inChord() && !fTrill && !fGlissandoStart && !fGlissandoStop) {
		cout << "newNote: isInChord so removing "<< fLastDur.toFloat() << " to curBeat: " << fCurBeat.toFloat() << endl;
		fCurBeat -= fLastDur; // because fucking MusicXML notation <chord/> is full of shit
		fLastDur = 0;
		w.AddNote(ANTESCOFO_CHORD, getMidiPitch(nv), d, fMeasNum, fCurBeat, flag, fRehearsals);
	}
	else if (nv.getType() == notevisitor::kRest)
		w.AddNote(ANTESCOFO_REST, 0, d, fMeasNum, fCurBeat, flag, fRehearsals);
	else if (fGlissandoStart || fGlissandoStop)
		w.AddNote(ANTESCOFO_MULTI, getMidiPitch(nv), d, fMeasNum, fCurBeat, flag, fRehearsals);
	else if (fTrill)
		w.AddNote(ANTESCOFO_TRILL, getMidiPitch(nv), d, fMeasNum, fCurBeat, flag, fRehearsals);
	else {
		if (nv.isGrace()) d = rational(0);
		w.AddNote(ANTESCOFO_NOTE, getMidiPitch(nv), d, fMeasNum, fCurBeat, flag, fRehearsals);
	}
	fCurBeat += d;
	fLastDur = d;
}

//______________________________________________________________________________
void xmlpart2antescofo::visitEnd ( S_note& elt )
{
	notevisitor::visitEnd ( elt );

	if (inChord()) return;					// chord notes have already been handled
	bool scanVoice = (notevisitor::getVoice() == fTargetVoice);

	if (!scanVoice) {
		return; 
	}
	checkStaff(notevisitor::getStaff());

	checkVoiceTime (fCurrentMeasurePosition, fCurrentVoicePosition);

	checkGrace(*this);
	vector<Sxmlelement> chord = getChord(elt);

	newNote (*this, elt);
	for (vector<Sxmlelement>::const_iterator iter = chord.begin(); iter != chord.end(); iter++) {
		notevisitor nv;
		xml_tree_browser browser(&nv);
		Sxmlelement note = *iter;
		browser.browse(*note);
		checkStaff(nv.getStaff());
		newNote (nv, elt);
	}

	fMeasureEmpty = false;
}

//______________________________________________________________________________
// time management
//______________________________________________________________________________
void xmlpart2antescofo::visitStart ( S_divisions& elt ) 
{
	fCurrentDivision = (long)(*elt);
}

}

