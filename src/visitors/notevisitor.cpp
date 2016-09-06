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
  fType   = kUndefinedType;
  fTie    = StartStop::undefined;
  fDuration = 0;
  fDots   = 0;
  fDynamics = kUndefinedDynamics;
  fStaff    = kUndefinedStaff;
  fVoice    = kUndefinedVoice;    
  fAlter = 0.f;
  fOctave = 0;
  fStem = 0;
  fAccent = 0;
  fStrongAccent = 0;
  fStaccato = 0;
  fTenuto = 0;
  fBreathMark = 0;
  fTimeModification.set(1,1);
#ifdef VC6
  fStep = "";
  fInstrument = "";
#else
  fStep.clear();
  fInstrument.clear();
#endif
  fTied.clear();
  fSlur.clear();
  fBeam.clear();
  
  fLastLyric = 0;
  fLastSyllabic = 0;
  fStanzas.clear();
}

//________________________________________________________________________
string notevisitor::i2step(int i)
{
  switch (i) {
    case notevisitor::A:  return "A";
    case notevisitor::B:  return "B";
    case notevisitor::C:  return "C";
    case notevisitor::D:  return "D";
    case notevisitor::E:  return "E";
    case notevisitor::F:  return "F";
    case notevisitor::G:  return "G";
  }
  return "";
}

//________________________________________________________________________
int notevisitor::step2i(const std::string& step)
{
  if (step.size() != 1) return -1;
  switch (step[0]) {
    case 'A': return notevisitor::A;
    case 'B': return notevisitor::B;
    case 'C': return notevisitor::C;
    case 'D': return notevisitor::D;
    case 'E': return notevisitor::E;
    case 'F': return notevisitor::F;
    case 'G': return notevisitor::G;
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
  fTimeModification.set ( 
    elt->getIntValue(k_normal_notes,1), 
    elt->getIntValue(k_actual_notes,1) );
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
void notevisitor::visitStart( S_text& elt ) 
{
  /*
        <lyric number="2">
          <syllabic>single</syllabic>
          <text>2. For</text>
        </lyric>
  */

  S_lyric     lastLyric =    getLastLyric();
  S_syllabic  lastSyllabic = getLastSyllabic();
  std::map<std::string, std::list<std::list<std::string> > > 
              stanzas =      getStanzas();
  string      text =         elt->getValue();
  
  std::string lastLyricNumber  = lastLyric->getAttributeValue("number");
  std::string lastLyricNumber2  = 
    lastLyric
      ? lastLyric->getAttributeValue("number")
      : "1";
  std::string lastSyllabicValue = lastSyllabic->getValue();
  
  cout <<
    "--> lastLyricNumber = " << lastLyricNumber <<
    ", lastSyllabicValue = " << lastSyllabicValue <<
    ", text = " << text << endl <<
    flush;
    
  std::map<std::string, std::list<std::list<std::string> > >::iterator it;
  
  it = stanzas.find(lastLyricNumber);
  if (it != stanzas.end()) {
    // stanzas[lastLyricNumber] has already been created
  }
  else {
    // create stanzas[lastLyricNumber]
    stanzas[lastLyricNumber] = std::list<std::list<std::string> >();
  }
    
  if (lastSyllabicValue == "single") {
    stanzas[lastLyricNumber].push_back(std::list<std::string>());
    stanzas[lastLyricNumber].back().push_back(text);
  }
  else if (lastSyllabicValue == "begin") {
 //   stanzas.push_back(text);
  }
  else if (lastSyllabicValue == "middle") {
//    stanzas.back() += " -- "+text;
  }
  else if (lastSyllabicValue == "end") {
//    stanzas.back() += " -- "+text;
  }
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
    if (inChord())  out << "in chord ";
    if (getTie() & StartStop::start)  out << "- tie start ";
    if (getTie() & StartStop::stop) out << "- tie stop ";
    string instr = getInstrument();
    if (!instr.empty()) out << "instrument " << instr << " ";
    if (getDynamics() >= 0) out << "dynamics " << getDynamics();
}

//________________________________________________________________________
void notevisitor::visitStart ( S_note& elt )
{
  fInNote = true;
  reset();
  fDynamics = elt->getAttributeLongValue("dynamics", kUndefinedDynamics);
}

//________________________________________________________________________
void notevisitor::visitEnd ( S_note& elt )
{
#ifdef PRINTNOTE
  cout << *this << endl;
#endif
}

}

