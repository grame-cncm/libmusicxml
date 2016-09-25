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
#include <ctype.h>

#include "xml_tree_browser.h"

#include "partsummaryvisitor.h"

#include "utilities.h"
#include "conversions.h"

#include "lpsr.h"
#include "xml2lpsrvisitor.h"
#include "xmlpart2lpsrvisitor.h"

using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
std::map<std::string, int> xmlpart2lpsrvisitor::fQuatertonesFromA;


//______________________________________________________________________________
xmlpart2lpsrvisitor::xmlpart2lpsrvisitor( translationSwitches& ts, SlpsrPart part ) : 
  fCurrentStaffIndex(0),
  fCurrentStaff(0),
  fTargetStaff(0),
  fTargetVoice(0)
{
  fTranslationSwitches = ts;
  fLpsrpart = part;

  fCurrentDuration = -8;

  fQuatertonesFromA["A"]=0;
  fQuatertonesFromA["B"]=4;
  fQuatertonesFromA["C"]=6;
  fQuatertonesFromA["D"]=10;
  fQuatertonesFromA["E"]=14;
  fQuatertonesFromA["F"]=16;
  fQuatertonesFromA["G"]=20;
  
  fCurrentChordIsBeingBuilt = false;
  fCurrentNoteBelongsToAChord = false;  
  
  fCurrentNoteBelongsToATuplet = false;
  
  fCurrentTupletNumber = -1;
  fCurrentTupletKind = lpsrTuplet::k_NoTuplet;
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::initialize (
  SlpsrElement seq,
  int staff,
  int lpsrstaff, 
  int voice, 
  bool notesonly,
  rational defaultTimeSign) 
{
  fCurrentStaff = fTargetStaff = staff; // the current and target staff
  fTargetVoice = voice;                 // the target voice
//  fNotesOnly = notesonly;               // prevent multiple output for keys, clefs etc... 
//  fCurrentTimeSign = defaultTimeSign;   // a default time signature
  fCurrentStaffIndex = lpsrstaff;   // the current lpsr staff index
}

//______________________________________________________________________________
// time management
//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitStart ( S_divisions& elt ) 
{
  fCurrentDivisions = (int)(*elt);
  if (fTranslationSwitches.fTrace) {
    if (fCurrentDivisions == 1)
      cerr << "There is 1 division per quater note" << std::endl;
    else
      cerr << "There are " << fCurrentDivisions << " divisions per quater note" << std::endl;
  }
}

void xmlpart2lpsrvisitor::resetCurrentTime ()
{
  fSenzaMisura = false;
  fCurrentBeats = 0;
  fCurrentBeatType = 0;
  fTimeSignatures.clear();
  fStaffNumber = kNoStaffNumber;
  fSymbol = "";
}

void xmlpart2lpsrvisitor::visitStart ( S_time& elt ) {
  resetCurrentTime();
  fStaffNumber = elt->getAttributeIntValue("number", kNoStaffNumber);
  fSymbol = elt->getAttributeValue("symbol");
}

void xmlpart2lpsrvisitor::visitStart ( S_beats& elt )
  { fCurrentBeats = (int)(*elt); }
  
void xmlpart2lpsrvisitor::visitStart ( S_beat_type& elt )
   { fCurrentBeatType = (int)(*elt); }
 
void xmlpart2lpsrvisitor::visitStart ( S_senza_misura& elt )
  { fSenzaMisura = true; }

rational xmlpart2lpsrvisitor::timeSignatureFromIndex(int index)
{
  rational r(0,1);
  if (index < fTimeSignatures.size()) {
    const pair<string,string>& ts = fTimeSignatures[index];
    int   num = strtol (ts.first.c_str(), 0, 10);
    int   denum = strtol (ts.second.c_str(), 0, 10);
    if (num && denum) r.set(num, denum);
  }
  return r;
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitEnd ( S_time& elt ) 
{
  SlpsrTime time = lpsrTime::create(
    fCurrentBeats, fCurrentBeatType, fTranslationSwitches.fGenerateNumericalTime);
  SlpsrElement t = time;
  addElementToPartSequence (t);
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitStart ( S_part& elt ) 
{
  fMeasureNumber = 0;
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::addElementToPartSequence (SlpsrElement& elt) {
//  bool doDebug = fTranslationSwitches.fDebug;
  bool doDebug = false;

  if (doDebug) cout << "!!! addElementToPartSequence : " << elt << std::endl;
  fLpsrpart->getPartLpsrsequence()->appendElementToSequence (elt);
}

SlpsrElement xmlpart2lpsrvisitor::getLastElementOfPartSequence() {
  return
    fLpsrpart->getPartLpsrsequence()->getLastElementOfSequence ();
}

void xmlpart2lpsrvisitor::removeLastElementOfPartSequence () {
//  bool doDebug = fTranslationSwitches.fDebug;
  bool doDebug = false;

  if (doDebug) cout << "!!! removeLastElementOfPartSequence" << std::endl;
  fLpsrpart->getPartLpsrsequence()->removeLastElementOfSequence ();
}

//________________________________________________________________________
// some code for the delayed elements management
// delayed elements are elements enclosed in a <direction> element that
// contains a non-null <offset> element. This offset postpones the graphic
// appearance of the element in 'offset' time units in the future.
// Time units are <division> time units 
//________________________________________________________________________
// add an element to the list of delayed elements
void xmlpart2lpsrvisitor::addDelayed (SlpsrElement elt, int offset) 
{
  /* JMI
  addToPartStack(elt);
  return;
  
  if (offset > 0) {
    delayedElement de;
    de.fDelay = offset;
    de.fElement = elt;
    fDelayed.push_back(de);
  }
  else addToPartStack (elt);
  */
}

//________________________________________________________________________
// checks ready elements in the list of delayed elements
// 'time' is the time elapsed since the last check, it is expressed in
// <division> time units
void xmlpart2lpsrvisitor::checkDelayed (int time)
{
  /*
  vector<delayedElement>::iterator it = fDelayed.begin();
  while (it!=fDelayed.end()) {
    it->fDelay -= time;
    if (it->fDelay < 0) {
      addToPartStack (it->fElement);
      it = fDelayed.erase(it);
    }
    else it++;
  }
  */
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitStart ( S_backup& elt )
{
//  stackClean(); // closes pending chords, cue and grace
  int duration = elt->getIntValue(k_duration, 0);
  
  if (duration) {   
    // backup is supposed to be used only for moving between voices
    // thus we don't move the voice time (which is supposed to be 0)
    //  moveMeasureTime (-duration, false);
  }
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitStart ( S_forward& elt )
{
  bool scanElement = 
    (elt->getIntValue(k_voice, 0) == fTargetVoice) 
      && 
    (elt->getIntValue(k_staff, 0) == fTargetStaff);
  int duration = elt->getIntValue(k_duration, 0);
  
 // moveMeasureTime(duration, scanElement);
  if (!scanElement) return;

 // stackClean(); // closes pending chords, cue and grace

  if (duration) {   
    rational r(duration, fCurrentDivisions*4);
    r.rationalise();
    lpsrDuration dur (r.getNumerator(), r.getDenominator());
    SlpsrElement note = 
      lpsrNote::create();//(fTargetVoice); // JMI , "empty", 0, dur, "");
    addElementToPartSequence (note);
 //   fMeasureEmpty = false;
  }
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitStart ( S_measure& elt ) 
{
  const string& implicit = elt->getAttributeValue ("implicit");
  /*
  if (implicit == "yes") fPendingBar = false;
  
  if (fPendingBar) {
    // before adding a bar, we need to check that there are no repeat begin at this location
    ctree<xmlelement>::iterator repeat = elt->find(k_repeat);
    if ((repeat == elt->end()) || (repeat->getAttributeValue("direction") != "forward")) {
      checkStaff (fTargetStaff);
      // JMI SlpsrElement cmd = lpsrcmd::create("|", lpsrcmd::kWithoutBackslash);
      //addElementToPartSequence (cmd);
    }
  }
  */
  
  fCurrentMeasure = elt;
  fMeasureNumber++;
  fCurrentMeasureLength.set  (0, 1);
  fCurrentMeasurePosition.set(0, 1);
  fCurrentVoicePosition.set  (0, 1);
//  fInhibitNextBar = false; // fNotesOnly;
 // fPendingBar = false;
 // fPendingPops = 0;
 // fMeasureEmpty = true;
  
  if (fTranslationSwitches.fGenerateComments) {
    SlpsrBarLine barline = lpsrBarLine::create(fMeasureNumber); // JMI
    SlpsrElement b = barline;
    addElementToPartSequence(b);
  }
}

void xmlpart2lpsrvisitor::visitEnd ( S_measure& elt ) 
{
//  stackClean(); // closes pending chords, cue and grace
//  checkVoiceTime (fCurrentMeasureLength, fCurrentVoicePosition);  

/*
  if (!fInhibitNextBar) {
    if (fTranslationSwitches.fGenerateBars) fPendingBar = true;
    else if (!fMeasureEmpty) {
      if (fCurrentVoicePosition < fCurrentMeasureLength)
        fPendingBar = true;
    }
  }
 */
  
  /* USER
  if (fGenerateComments) {
    stringstream s;
    s << "% " << fMeasureNumber + 1; // USER
      // to announce number of first measure on next line
    string comment=s.str()+"\n";
    SlpsrElement elt = lpsrElement ::create(comment);
    addElementToPartSequence (elt);
  }
  */
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitStart ( S_direction& elt ) 
{
  /*
  if (fNotesOnly || (elt->getIntValue(k_staff, 0) != fTargetStaff)) {
    fSkipDirection = true;
  }
  else {
    fCurrentOffset = elt->getIntValue(k_offset, 0);
  }
  */
}

void xmlpart2lpsrvisitor::visitEnd ( S_direction& elt ) 
{
 // fSkipDirection = false;
  fCurrentOffset = 0;
}

//______________________________________________________________________________

void xmlpart2lpsrvisitor::visitStart ( S_key& elt ) {
  fCurrentFifths = fCurrentCancel = 0;
  fCurrentMode = "";
}
void xmlpart2lpsrvisitor::visitStart ( S_cancel& elt )
  { fCurrentCancel = (int)(*elt); }
  
void xmlpart2lpsrvisitor::visitStart ( S_fifths& elt )
  { fCurrentFifths = (int)(*elt); }
  
void xmlpart2lpsrvisitor::visitStart ( S_mode& elt )
  { fCurrentMode = elt->getValue(); }

void xmlpart2lpsrvisitor::visitEnd ( S_key& elt ) 
{    
  std::string          tonicNote;
  lpsrKey::KeyMode keyMode;
  
  switch (fCurrentFifths) {
    case 0:
      tonicNote = "c";
      keyMode = lpsrKey::kMajor;
      break;
    case 1:
      tonicNote = "g";
      keyMode = lpsrKey::kMajor;
      break;
    case 2:
      tonicNote = "d";
      keyMode = lpsrKey::kMajor;
      break;
    case 3:
      tonicNote = "a";
      keyMode = lpsrKey::kMajor;
      break;
    case 4:
      tonicNote = "e";
      keyMode = lpsrKey::kMajor;
      break;
    case 5:
      tonicNote = "b";
      keyMode = lpsrKey::kMajor;
      break;
    case 6:
       tonicNote = "fis";
      keyMode = lpsrKey::kMajor;
      break;
    case 7:
      tonicNote = "cis";
      keyMode = lpsrKey::kMajor;
      break;
    case -1:
      tonicNote = "f";
      keyMode = lpsrKey::kMajor;
      break;
    case -2:
      tonicNote = "bes";
      keyMode = lpsrKey::kMajor;
      break;
    case -3:
      tonicNote = "ees";
      keyMode = lpsrKey::kMajor;
      break;
    case -4:
      tonicNote = "aes";
      keyMode = lpsrKey::kMajor;
      break;
    case -5:
      tonicNote = "des";
      keyMode = lpsrKey::kMajor;
      break;
    case -6:
      tonicNote = "ges";
      keyMode = lpsrKey::kMajor;
      break;
    case -7:
      tonicNote = "ces";
      keyMode = lpsrKey::kMajor;
      break;
    default: // unknown key sign !!
      cerr << 
        "ERROR: unknown key sign \"" << fCurrentFifths << "\"" << endl;
      return; 
  } // switch
  
  // create lpsrKey and add it to part
  SlpsrElement key = lpsrKey::create(tonicNote, keyMode);
  SlpsrElement k = key;
  addElementToPartSequence (k);
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitStart ( S_clef& elt )
{ 
  fLine = kStandardLine;
  fOctaveChange = 0;
  fNumber = kNoNumber;
  fSign = "";

  fNumber = elt->getAttributeIntValue("number", kNoNumber); 
}

void xmlpart2lpsrvisitor::visitStart ( S_clef_octave_change& elt )
  { fOctaveChange = (int)(*elt); }
  
void xmlpart2lpsrvisitor::visitStart ( S_line& elt )
  { fLine = (int)(*elt); }
  
void xmlpart2lpsrvisitor::visitStart ( S_sign& elt )
  { fSign = elt->getValue(); }

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitEnd ( S_clef& elt ) 
{
  int staffnum = elt->getAttributeIntValue("number", 0);
  
  // JMI if ((staffnum != fTargetStaff) || fNotesOnly) return;

  stringstream s; 

  // USER
//  checkStaff (staffnum);

  if ( fSign == "G") {
    if ( fLine == 2 )
      s << "treble"; 
    else { // unknown G clef line !!
      cerr << 
        "warning: unknown G clef line \"" <<
        fLine << 
        "\"" <<
        endl;
      return; 
      }
    }
  else if ( fSign == "F") {
    if ( fLine == 4 )
      s << "bass"; 
    else { // unknown F clef line !!
      cerr << 
        "warning: unknown F clef line \"" <<
        fLine << 
        "\"" <<
        endl;
      return; 
      }
    }
  else if ( fSign == "C") {
    if ( fLine == 4 )
      s << "tenor"; 
    else if ( fLine == 3 )
      s << "alto"; 
    else { // unknown C clef line !!
      cerr << 
        "warning: unknown C clef line \"" <<
        fLine << 
        "\"" <<
        endl;
      return; 
      }
    }
  else if ( fSign == "percussion") {
    s << "perc"; }
  else if ( fSign == "TAB") {
    s << "TAB"; }
  else if ( fSign == "none") {
    s << "none"; }
  else { // unknown clef sign !!
    cerr << 
      "warning: unknown clef sign \"" <<
       fSign << 
       "\"" <<
      endl;
    return; 
  }
  
  string param;
  
  if (fLine != kStandardLine) 
    // s << fLine; // USER
    s >> param;
    
  if (fOctaveChange == 1)
    param += "^8"; // USER
  else if (fOctaveChange == -1)
    param += "_8";
    
  SlpsrClef clef = lpsrClef::create(param);
  SlpsrElement c = clef;
  addElementToPartSequence (c);
}

//________________________________________________________________________
void xmlpart2lpsrvisitor::resetMetronome ()
{
  fBeats.clear();
  fPerMinute = 0;
  resetMetronome(fCurrentBeat);
}

void xmlpart2lpsrvisitor::resetMetronome (beat& b)
{
  b.fUnit = "";
  b.fDots = 0;
}

void xmlpart2lpsrvisitor::visitStart ( S_metronome& elt )
  { resetMetronome(); }
  
void xmlpart2lpsrvisitor::visitEnd ( S_metronome& elt ) { 
 // if (fSkipDirection) return;

  if (fCurrentBeat.fUnit.size()) {
    fBeats.push_back(fCurrentBeat); 
    resetMetronome (fCurrentBeat);
  }
  
  if (fBeats.size() != 1) return;         // support per minute tempo only (for now)
  if (! fPerMinute) return;    // support per minute tempo only (for now)

  beat b = fBeats[0];
  rational r = 
    NoteType::type2rational(NoteType::xml(b.fUnit)), rdot(3,2);
  
  while (b.fDots-- > 0) {
    r *= rdot;
  }
  r.rationalise();

  stringstream s;
  s << r.getDenominator() << " = " << fPerMinute; // USER
 // if (fCurrentOffset) addDelayed(cmd, fCurrentOffset);
 // addElementToPartSequence (cmd);
}

void xmlpart2lpsrvisitor::visitStart ( S_beat_unit& elt ) { 
  if (fCurrentBeat.fUnit.size()) {
    fBeats.push_back(fCurrentBeat); 
    resetMetronome (fCurrentBeat);
  }
  fCurrentBeat.fUnit = elt->getValue();
}

void xmlpart2lpsrvisitor::visitStart ( S_beat_unit_dot& elt )
  { fCurrentBeat.fDots++; }
void xmlpart2lpsrvisitor::visitStart ( S_per_minute& elt )
  { fPerMinute = (int)(*elt); }

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitStart ( S_coda& elt )
{
//  if (fSkipDirection) return;
//  SlpsrElement cmd = lpsrcmd::create("coda");
 // addElementToPartSequence(cmd);
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitStart ( S_segno& elt )
{
//  if (fSkipDirection) return;
//  SlpsrElement cmd = lpsrcmd::create("segno");
//  addElementToPartSequence(cmd);
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitStart ( S_trill_mark& elt )
{
//  SlpsrElement cmd = lpsrcmd::create("\\trill");
//  addElementToPartSequence(cmd);
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitStart( S_dynamics& elt)
{}

void xmlpart2lpsrvisitor::visitStart( S_f& elt)
{        
  SlpsrDynamics dyn = lpsrDynamics::create(lpsrDynamics::kFDynamics);
  fPendingDynamics.push_back(dyn);
 }

void xmlpart2lpsrvisitor::visitStart( S_p& elt)
{        
  SlpsrDynamics dyn = lpsrDynamics::create(lpsrDynamics::kPDynamics);
  fPendingDynamics.push_back(dyn);
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitStart ( S_beam& elt )
{
  int number = atoi(elt->getAttributeValue("number").c_str());
  std::string value = elt->getValue();
  
  lpsrBeam::BeamKind bk = lpsrBeam::k_NoBeam;

  if (value == "begin") {
    bk = lpsrBeam::kBeginBeam;
  }
  else if (value == "continue") {
    bk = lpsrBeam::kContinueBeam;
  }
  else if (value == "end") {
    bk = lpsrBeam::kEndBeam;
  }
  
  SlpsrBeam beam = lpsrBeam::create(number, bk);;
  fCurrentBeam = beam;
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitStart ( S_wedge& elt )
{
  string type = elt->getAttributeValue("type");
  lpsrWedge::WedgeKind wk;

  if (type == "crescendo") {
    wk = lpsrWedge::kCrescendoWedge;
  }
  else if (type == "diminuendo") {
    wk = lpsrWedge::kDecrescendoWedge;
  }
  else if (type == "stop") {
    wk = lpsrWedge::kStopWedge;
  }
  
  SlpsrWedge wedg = lpsrWedge::create(wk);;
  fPendingWedges.push_back(wedg);
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitStart( S_octave_shift& elt)
{
//  if (fSkipDirection) return;

  const string& type = elt->getAttributeValue("type");
  int size = elt->getAttributeIntValue("size", 0);

  switch (size) {
    case 8:   size = 1; break;
    case 15:  size = 2; break;
    default:  return;
  }

  if (type == "up")
    size = -size;
  else if (type == "stop")
    size = 0;
  else if (type != "down") return;
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitEnd ( S_sound& elt )
{
 // if (fNotesOnly) return;

  SlpsrElement cmd = 0;
  Sxmlattribute attribute;
  
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitEnd ( S_ending& elt )
{
  string type = elt->getAttributeValue("type");
  
  if (type == "start") {
    stringstream s; // USER
    string num = elt->getAttributeValue ("number");
    s << "repeat volta " << num << " {\n";
  //  addElementToPartSequence(cmd);
  }
  else {
 // JMI    if (type == "discontinue")
   }
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitEnd ( S_repeat& elt ) 
{
  SlpsrElement cmd;
  string direction = elt->getAttributeValue("direction");
  /*
  if (direction == "forward") 
 //   cmd = lpsrcmd::create("{ %repeatBegin \n"); // USER
  else if (direction == "backward") {
  //  cmd = lpsrcmd::create("\n} %repeatEnd \n");
 //   fInhibitNextBar = true;
  }
 // if (cmd) addElementToPartSequence(cmd);  
 */
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitStart ( S_barline& elt ) 
{
  const string& location = elt->getAttributeValue("location");
  if (location == "middle") {
    // todo: handling bar-style (not yet supported in lpsr)
    SlpsrBarLine barline = lpsrBarLine::create(1544442); // JMI
    SlpsrElement b = barline;
    addElementToPartSequence(b);
  }
  // TODO: support for left and right bars
  // currently automatically handled at measure boundaries
  else if (location == "right") {
  }
  else if (location == "left") {
  }
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitStart ( S_print& elt ) 
{
  const string& newSystem = elt->getAttributeValue("new-system");
  if (newSystem == "yes") {
    // create a barnumbercheck command
    SlpsrBarNumberCheck barnumbercheck_ = lpsrBarNumberCheck::create(fMeasureNumber);
    SlpsrElement b2 = barnumbercheck_;
    addElementToPartSequence(b2);

    // create a break command
    SlpsrBreak break_ = lpsrBreak::create(fMeasureNumber);
    SlpsrElement b1 = break_;
    addElementToPartSequence(b1);
  }
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitStart ( S_step& elt )
{
  fCurrentStep=elt->getValue();
//  cout << "=== xmlpart2lpsrvisitor::visitStart ( S_step& elt ) " << fCurrentStep << std::endl;

  lpsrNote::DiatonicPitch diatonicNote = lpsrNote::kNoDiatonicPitch;

  std::transform(fCurrentStep.begin(), fCurrentStep.end(), fCurrentStep.begin(), ::toupper);

  if      (fCurrentStep == "A") diatonicNote = lpsrNote::kA;
  else if (fCurrentStep == "B") diatonicNote = lpsrNote::kB;
  else if (fCurrentStep == "C") diatonicNote = lpsrNote::kC;
  else if (fCurrentStep == "D") diatonicNote = lpsrNote::kD;
  else if (fCurrentStep == "E") diatonicNote = lpsrNote::kE;
  else if (fCurrentStep == "F") diatonicNote = lpsrNote::kF;
  else if (fCurrentStep == "G") diatonicNote = lpsrNote::kG;
  else 
    lpsrAssert(false, 
      "xmlpart2lpsrvisitor::visitStart, fCurrentStep '"+fCurrentStep+
      "' is not a letter from A to G");
}

void xmlpart2lpsrvisitor::visitStart ( S_alter& elt)
{
  fCurrentAlter=(int)(*elt);
}

void xmlpart2lpsrvisitor::visitStart ( S_octave& elt)
{
  fCurrentOctave=(int)(*elt);
}

void xmlpart2lpsrvisitor::visitStart ( S_duration& elt )
{
  fCurrentDuration=(int)(*elt);
//  cout << "=== xmlpart2lpsrvisitor::visitStart ( S_duration& elt ), fCurrentDuration = " << fCurrentDuration << std::endl;
}

void xmlpart2lpsrvisitor::visitStart ( S_dot& elt )
{
  fCurrentDotsNumber++;
}
       
void xmlpart2lpsrvisitor::visitStart ( S_voice& elt )
{
  fCurrentVoice=(int)(*elt);
}

void xmlpart2lpsrvisitor::visitStart ( S_type& elt )
{
  fCurrentType=elt->getValue();
}

void xmlpart2lpsrvisitor::visitStart ( S_stem& elt )
{
  fCurrentStem=elt->getValue();
}

void xmlpart2lpsrvisitor::visitStart ( S_staff& elt )
{
  fCurrentStaff=(int)(*elt);
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitStart ( S_chord& elt)
{
  fCurrentNoteBelongsToAChord = true;
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitStart ( S_actual_notes& elt )
{
  fCurrentActualNotes=(int)(*elt);
}

void xmlpart2lpsrvisitor::visitStart ( S_normal_notes& elt )
{
  fCurrentNormalNotes=(int)(*elt);
}

void xmlpart2lpsrvisitor::visitStart ( S_tuplet& elt )
{
  fCurrentNoteBelongsToATuplet = true;

  fCurrentTupletNumber = atoi(elt->getAttributeValue("number").c_str());
  std::string type     = elt->getAttributeValue("type");
  
  /*
  cout <<
    "xmlpart2lpsrvisitor::visitStart ( S_tuplet, fCurrentTupletNumber = " <<
    fCurrentTupletNumber << ", type = " << type <<std::endl;
  */
  
  fCurrentTupletKind = lpsrTuplet::k_NoTuplet;
  
  if (type == "start")
    fCurrentTupletKind = lpsrTuplet::kStartTuplet;
  else if (type == "continue")
    fCurrentTupletKind = lpsrTuplet::kContinueTuplet;
  else if (type == "stop")
    fCurrentTupletKind = lpsrTuplet::kStopTuplet;
  else
    cerr << "ERROR, unknown tuplet type " << type <<std::endl;
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitStart ( S_note& elt ) 
{
  //  cout << "--> xmlpart2lpsrvisitor::visitStart ( S_note& elt ) " << std::endl;
  fCurrentStep = "_";
  fCurrentStepIsARest = false;
  fCurrentAlter = 0; // natural notes
  fCurrentOctave = -13;
  fCurrentDotsNumber = 0;
   
  // assume this note doesn't belong to a chord until S_chord is met
  fCurrentNoteBelongsToAChord = false;
}

//______________________________________________________________________________

lpsrNote::LpsrPitch xmlpart2lpsrvisitor::computeNoteLpsrPitch(
  int                  noteQuatertonesFromA,
  lpsrNote::Alteration alteration)
{
  // computing the lpsr pitch
  /*
  Alter values of -2 and 2 can be used for double-flat and double-sharp. Decimal values can be used for microtones (e.g., 0.5 for a quarter-tone sharp), but not all programs may convert this into MIDI pitch-bend data.

  For rests, a rest element is used instead of the pitch element. The whole rest in 3/4 that begins the voice part is represented as:
    <note>
      <rest/>
      <duration>72</duration>
    </note>
  
  Quarter tones may be added; the following is a series of Cs with increasing pitches:
    \relative c'' { ceseh ces ceh c cih cis cisih }
   */
  lpsrNote::LpsrPitch lpsrPitch = lpsrNote::k_NoLpsrPitch;
  
  switch (noteQuatertonesFromA) {
    case 0:
      lpsrPitch = lpsrNote::k_a;
      break;
    case 1:
      lpsrPitch =
        alteration == lpsrNote::kDoubleSharp
          ? lpsrNote::k_aih
          : lpsrNote::k_beseh;
      break;
    case 2:
      lpsrPitch =
        alteration == lpsrNote::kSharp
          ? lpsrNote::k_ais
          : lpsrNote::k_bes;
      break;
    case 3:
      lpsrPitch =
        alteration == lpsrNote::kDoubleSharp
          ? lpsrNote::k_aisih
          : lpsrNote::k_beh;
      break;
    case 4:
      lpsrPitch = lpsrNote::k_b;
      break;
    case 5:
      lpsrPitch =
        alteration == lpsrNote::kDoubleSharp
          ? lpsrNote::k_bih
          : lpsrNote::k_ceseh;
      break;
    case 6:
      lpsrPitch = lpsrNote::k_c;
      break;
    case 7:
      lpsrPitch =
        alteration == lpsrNote::kDoubleSharp
          ? lpsrNote::k_cih
          : lpsrNote::k_deseh;
      break;
    case 8:
      lpsrPitch =
        alteration == lpsrNote::kSharp
          ? lpsrNote::k_cis
          : lpsrNote::k_des;
      break;
    case 9:
      lpsrPitch =
        alteration == lpsrNote::kDoubleSharp
          ? lpsrNote::k_cisih
          : lpsrNote::k_deh;
      break;
    case 10:
      lpsrPitch = lpsrNote::k_d;
      break;
    case 11:
      lpsrPitch =
        alteration == lpsrNote::kDoubleSharp
          ? lpsrNote::k_dih
          : lpsrNote::k_eeseh;
      break;
    case 12:
      lpsrPitch =
        alteration == lpsrNote::kSharp
          ? lpsrNote::k_dis
          : lpsrNote::k_ees;
      break;
    case 13:
      lpsrPitch =
        alteration == lpsrNote::kDoubleSharp
          ? lpsrNote::k_disih
          : lpsrNote::k_eeh;
      break;
    case 14:
      lpsrPitch = lpsrNote::k_e;
      break;
    case 15:
      lpsrPitch =
        alteration == lpsrNote::kDoubleSharp
          ? lpsrNote::k_eih
          : lpsrNote::k_feseh;
      break;
    case 16:
      lpsrPitch = lpsrNote::k_f;
      break;
    case 17:
      lpsrPitch =
        alteration == lpsrNote::kDoubleSharp
          ? lpsrNote::k_fih
          : lpsrNote::k_geseh;
      break;
    case 18:
      lpsrPitch =
        alteration == lpsrNote::kSharp
          ? lpsrNote::k_fis
          : lpsrNote::k_ges;
      break;
    case 19:
      lpsrPitch =
        alteration == lpsrNote::kDoubleSharp
          ? lpsrNote::k_fisih
          : lpsrNote::k_geh;
      break;
    case 20:
      lpsrPitch = lpsrNote::k_g;
      break;
    case 21:
      lpsrPitch =
        alteration == lpsrNote::kDoubleSharp
          ? lpsrNote::k_gih
          : lpsrNote::k_aeseh;
      break;
    case 22:
      lpsrPitch =
        alteration == lpsrNote::kSharp
          ? lpsrNote::k_gis
          : lpsrNote::k_aes;
      break;
    case 23:
      lpsrPitch =
        alteration == lpsrNote::kDoubleSharp
          ? lpsrNote::k_gisih
          : lpsrNote::k_aeh;
      break;
  } // switch
  
  return lpsrPitch;
}

void xmlpart2lpsrvisitor::createChord (SlpsrDuration noteDuration) {
  // cout << "--> creating a chord on its 2nd note" << endl;
  
  // fCurrentNote has been registered standalone in the part element sequence,
  // but it is actually the first note of a chord
  
   // create a chord
  fCurrentChord = lpsrChord::create(noteDuration);
  fCurrentElement = fCurrentChord; // another name for it
   
  //cout << "--> adding first note to fCurrentChord" << endl;
  // register fCurrentNote as first member of fCurrentChord
  fCurrentChord->addNoteToChord(fCurrentNote);
  fCurrentNote->setNoteBelongsToAChord();
  
  // move the pending dynamics if any from the first note to the chord
  std::list<SlpsrDynamics> noteDynamics = fCurrentNote->getNoteDynamics();
  while (! noteDynamics.empty()) {
    //cout << "--> moving dynamics from fCurrentNote to fCurrentChord" << endl;
    SlpsrDynamics dyn = noteDynamics.front();
    fCurrentChord->addDynamics(dyn);
    noteDynamics.pop_front();
  } // while
 
  // move the pending wedges if any from the first note to the chord
  std::list<SlpsrWedge> noteWedges = fCurrentNote->getNoteWedges();
  while (! noteWedges.empty()) {
    //cout << "--> moving wedge from fCurrentNote to fCurrentChord" << endl;
    SlpsrWedge wdg = noteWedges.front();
    fCurrentChord->addWedge(wdg);
    noteWedges.pop_front();
  } // while
}

void xmlpart2lpsrvisitor::createTuplet (SlpsrNote note) {
  // create a tuplet element
  SlpsrTuplet fCurrentTuplet = lpsrTuplet::create();
  fCurrentElement = fCurrentNote; // another name for it

  // populate it
  fCurrentTuplet->updateTuplet(
    fCurrentTupletNumber,
    fCurrentActualNotes,
    fCurrentNormalNotes);

  // register it in this visitor
  cout << "--> pushing tuplet to tuplets stack" << std::endl;
  fCurrentTupletsStack.push(fCurrentTuplet);
  
  // update note duration
  cout
    << "--> updating note duration by " << fCurrentActualNotes << 
    "/" << fCurrentNormalNotes << std::endl;
  note->updateNoteDuration(fCurrentActualNotes, fCurrentNormalNotes);

  // add note to the tuplet
  cout << "--> adding note " << note << " to tuplets stack top" << std::endl;
  fCurrentTuplet->addElementToTuplet(note);
}

void xmlpart2lpsrvisitor::finalizeTuplet (SlpsrNote note) {
  // get tuplet from top of tuplet stack
  SlpsrTuplet tup = fCurrentTupletsStack.top();

  // add note to the tuplet
  cout << "--> adding note " << note << " to tuplets stack top" << std::endl;
  tup->addElementToTuplet(note);

  // pop from the tuplets stack
  cout << "--> popping from tuplets stack" << std::endl;
  fCurrentTupletsStack.pop();        

  // add tuplet to the part
  cout << "=== adding tuplet to the part sequence" << std::endl;
  SlpsrElement elem = tup;
  addElementToPartSequence(elem);
}          

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitEnd ( S_note& elt ) 
{
  //  cout << "<-- xmlpart2lpsrvisitor::visitEnd ( S_note& elt ) " << std::endl;

  // create an empty lpsr (with default values) until we know more
  SlpsrNote note = lpsrNote::create();//(fTargetVoice); // JMI , "s", 0, dur, "");

  if (fTranslationSwitches.fDebug)
    std::cerr << "fCurrentDuration = " << fCurrentDuration << ", " << 
    "fCurrentDivisions*4 = " << fCurrentDivisions*4 << std::endl;
  if (fCurrentDivisions*4 == 0)
    {
    std::cerr << 
      std::endl << 
      "%--> xmlpart2lpsrvisitor::visitEnd, fCurrentDuration = " << fCurrentDuration <<
      ", fCurrentDivisions*4 = " << fCurrentDivisions*4 << std::endl;
    //return;
  }

  SlpsrDuration noteDuration =
    lpsrDuration::create(fCurrentDuration, fCurrentDivisions*4, fCurrentDotsNumber);

  // now we know more, update the various informations
  
  // diatonic note
  lpsrNote::DiatonicPitch diatonicNote = lpsrNote::kNoDiatonicPitch;

  if (fCurrentStepIsARest) diatonicNote = lpsrNote::kRest;
  else {
    //std::transform(fCurrentStep.begin(), fCurrentStep.end(), fCurrentStep.begin(), ::toupper);
    
    if      (fCurrentStep == "A") diatonicNote = lpsrNote::kA;
    else if (fCurrentStep == "B") diatonicNote = lpsrNote::kB;
    else if (fCurrentStep == "C") diatonicNote = lpsrNote::kC;
    else if (fCurrentStep == "D") diatonicNote = lpsrNote::kD;
    else if (fCurrentStep == "E") diatonicNote = lpsrNote::kE;
    else if (fCurrentStep == "F") diatonicNote = lpsrNote::kF;
    else if (fCurrentStep == "G") diatonicNote = lpsrNote::kG;
    else
      lpsrAssert(false, 
        "xmlpart2lpsrvisitor::visitEnd. step '"+fCurrentStep+
        "' is not a letter from A to G");
  }
 
  int noteQuatertonesFromA = fQuatertonesFromA[fCurrentStep];
  
  // flat or sharp
  lpsrNote::Alteration alteration;
  
  assert(fCurrentAlter>=-2 && fCurrentAlter<=+2);
  switch (fCurrentAlter) {
    case -2:
      alteration = lpsrNote::kDoubleFlat;
      noteQuatertonesFromA-=3;
      if (noteQuatertonesFromA < 0) noteQuatertonesFromA += 24; // it is below A
      break;
    case -1:
      alteration = lpsrNote::kFlat;
      noteQuatertonesFromA-=2;
      if (noteQuatertonesFromA < 0) noteQuatertonesFromA += 24; // it is below A
      break;
    case 0:
      alteration = lpsrNote::kNatural;
      break;
    case 1:
      alteration = lpsrNote::kSharp;
      noteQuatertonesFromA+=2;
      break;
    case 2:
      alteration = lpsrNote::kDoubleSharp;
      noteQuatertonesFromA+=3;
      break;
    default:
      cout << "fCurrentAlter = " << fCurrentAlter << std::endl << std::flush;
  } // switch

  lpsrNote::LpsrPitch lpsrPitch = 
    computeNoteLpsrPitch(noteQuatertonesFromA, alteration);

  // attach the pending dynamics if any to the note
  while (! fPendingDynamics.empty()) {
    SlpsrDynamics dyn = fPendingDynamics.front();
    note->addDynamics(dyn);
    fPendingDynamics.pop_front();
  } // while
 
  // attach the pending wedges if any to the note
  while (! fPendingWedges.empty()) {
    SlpsrWedge wdg = fPendingWedges.front();
    note->addWedge(wdg);
    fPendingWedges.pop_front();
  } // while
  
  int voice = 37;
  
  // update note with the computed information
  note->updateNote(
    fCurrentStepIsARest,
    diatonicNote, 
    alteration, 
    fCurrentOctave,
    noteDuration,
    lpsrPitch, 
    voice,
    fCurrentNoteBelongsToAChord);

  //cout << "::: creating note " << note << std::endl;
  
  // keep track of note in this visitor
  fCurrentNote = note;
  fCurrentElement = fCurrentNote; // another name for it
  
  if (fCurrentNoteBelongsToAChord) {
    if (! fCurrentChordIsBeingBuilt) {
      createChord (noteDuration);

      // account for chord being built
      fCurrentChordIsBeingBuilt = true;
    }

    //cout << "--> adding note to fCurrentChord" << endl;
    // register note as a member of fCurrentChord
    fCurrentChord->addNoteToChord(note);
      
    // remove (previous) fCurrentNote that is the last element of the part sequence
    removeLastElementOfPartSequence();

    // add fCurrentChord to the part sequence instead
    SlpsrElement elem = fCurrentChord;
    addElementToPartSequence(elem);

  } else if (fCurrentNoteBelongsToATuplet) {

    // handle tuplet
    switch (fCurrentTupletKind) {
      case lpsrTuplet::kStartTuplet:
        {
          createTuplet(note);
        
          // swith to continuation mode
          // this is handy in case the forthcoming tuplet members
          // are not explictly of the "continue" type
          fCurrentTupletKind = lpsrTuplet::kContinueTuplet;
        }
        break;
  
      case lpsrTuplet::kContinueTuplet:
        {
          // populate the tuplet at the top of the stack
          cout << "--> adding note " << note << " to tuplets stack top" << std::endl;
          fCurrentTupletsStack.top()->addElementToTuplet(note);
        }
        break;
  
      case lpsrTuplet::kStopTuplet:
        {
          finalizeTuplet(note);

          // indicate the end of the tuplet
          fCurrentNoteBelongsToATuplet = false;
        }
        break;
    } // switch

  } else {

    // cout << "--> adding standalone note to part sequence" << endl;
    // register note as standalone
    SlpsrElement n = note;
    addElementToPartSequence(n);
  
    // account for chord not being built
    fCurrentChordIsBeingBuilt = false;
  }
}

//______________________________________________________________________________
void xmlpart2lpsrvisitor::visitStart ( S_rest& elt)
{
  //  cout << "--> xmlpart2lpsrvisitor::visitStart ( S_rest& elt ) " << std::endl;
  fCurrentStepIsARest = true;
}


}
