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

#include "conversions.h"
#include "utilities.h"

#include "lpsr.h"
#include "xml2LpsrVisitor.h"
#include "xmlPartSummaryVisitor.h"
#include "xmlPart2LpsrVisitor.h"

using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
// JMI std::map<std::string, int> xmlPart2LpsrVisitor::fQuatertonesFromA;


//______________________________________________________________________________
xmlPart2LpsrVisitor::xmlPart2LpsrVisitor(
  S_translationSettings& ts,
  S_lpsrPart part,
  S_lpsrElement seq,
  int staff,
  int lpsrstaff, 
  int targetVoice, 
  bool notesonly,
  rational defaultTimeSign) 
{
  fTranslationSettings = ts;
  fLpsrpart = part;

  fMusicXMLNoteData.fMusicxmlDuration = -8;
  fMusicXMLNoteData.fNoteBelongsToAChord = false;    
  fMusicXMLNoteData.fNoteBelongsToATuplet = false;
  fMusicXMLNoteData.fVoiceNumber = 1; // it may be absent from MusicXML

//  fCurrentStaffIndex = 0;
  fCurrentStaff = 0; // JMI ???

  fCurrentStaff = fTargetStaff = staff; // the current and target staff
  fTargetVoice = targetVoice;                 // the target voice
//  fNotesOnly = notesonly;               // prevent multiple output for keys, clefs etc... 
//  fCurrentTimeSign = defaultTimeSign;   // a default time signature
//  fCurrentStaffIndex = lpsrstaff;   // the current lpsr staff index

  fCurrentDivisions = -1;

  fAChordIsBeingBuilt = false;

  
  fATupletIsBeingBuilt = false;
  fCurrentTupletNumber = -1;
  fCurrentTupletKind = lpsrTuplet::k_NoTuplet;
}

//______________________________________________________________________________
// time management
//______________________________________________________________________________
void xmlPart2LpsrVisitor::visitStart ( S_divisions& elt ) 
{
  fCurrentDivisions = (int)(*elt);
  if (fTranslationSettings->fTrace) {
    if (fCurrentDivisions == 1)
      cerr << "There is 1 division per quater note" << std::endl;
    else
      cerr <<
        "There are " << fCurrentDivisions <<
        " divisions per quater note" << std::endl;
  }
}

void xmlPart2LpsrVisitor::resetCurrentTime ()
{
  fStaffNumber = kNoStaffNumber;

  fSenzaMisura = false;

  fCurrentBeats = 0;
  fCurrentBeatType = 0;
  
//  fTimeSignatures.clear();
  fSymbol = "";
}

void xmlPart2LpsrVisitor::visitStart ( S_time& elt ) {
  resetCurrentTime();
  fStaffNumber = elt->getAttributeIntValue("number", kNoStaffNumber);
  fSymbol = elt->getAttributeValue("symbol");
}

void xmlPart2LpsrVisitor::visitStart ( S_beats& elt )
  { fCurrentBeats = (int)(*elt); }
  
void xmlPart2LpsrVisitor::visitStart ( S_beat_type& elt )
  { fCurrentBeatType = (int)(*elt); }
 
void xmlPart2LpsrVisitor::visitStart ( S_senza_misura& elt )
  { fSenzaMisura = true; }

/*
rational xmlPart2LpsrVisitor::timeSignatureFromIndex(int index)
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
*/
//______________________________________________________________________________
void xmlPart2LpsrVisitor::visitEnd ( S_time& elt ) 
{
  S_lpsrTime time = lpsrTime::create(
    fCurrentBeats, fCurrentBeatType, fTranslationSettings->fGenerateNumericalTime);
  S_lpsrElement t = time;
  addElementToPartSequence (t);
}

//______________________________________________________________________________
void xmlPart2LpsrVisitor::visitStart ( S_part& elt ) 
{
  fMeasureNumber = 0;
}

//______________________________________________________________________________
void xmlPart2LpsrVisitor::addElementToPartSequence (S_lpsrElement& elt) {
//  bool doDebug = fTranslationSettings->fDebug;
  bool doDebug = false;

  if (doDebug) cout << "!!! addElementToPartSequence : " << elt << std::endl;
  fLpsrpart->getPartLpsrsequence()->appendElementToSequence (elt);
}

S_lpsrElement xmlPart2LpsrVisitor::getLastElementOfPartSequence() {
  return
    fLpsrpart->getPartLpsrsequence()->getLastElementOfSequence ();
}

void xmlPart2LpsrVisitor::removeLastElementOfPartSequence () {
//  bool doDebug = fTranslationSettings->fDebug;
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
void xmlPart2LpsrVisitor::addDelayed (S_lpsrElement elt, int offset) 
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
void xmlPart2LpsrVisitor::checkDelayed (int time)
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
void xmlPart2LpsrVisitor::visitStart ( S_backup& elt )
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
void xmlPart2LpsrVisitor::visitStart ( S_forward& elt )
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
    lpsrDuration dur (r.getNumerator(), r.getDenominator(), 57, "////"); // JMI
    /*
    S_lpsrElement note = 
      lpsrNote::create();//(fTargetVoice); // JMI , "empty", 0, dur, "");
    addElementToPartSequence (note);
 //   fMeasureEmpty = false;
 */
  }
}

//______________________________________________________________________________
void xmlPart2LpsrVisitor::visitStart ( S_measure& elt ) 
{
  const string& implicit = elt->getAttributeValue ("implicit");
  /*
  if (implicit == "yes") fPendingBar = false;
  
  if (fPendingBar) {
    // before adding a bar, we need to check that there are no repeat begin at this location
    ctree<xmlelement>::iterator repeat = elt->find(k_repeat);
    if ((repeat == elt->end()) || (repeat->getAttributeValue("direction") != "forward")) {
      checkStaff (fTargetStaff);
      // JMI S_lpsrElement cmd = lpsrcmd::create("|", lpsrcmd::kWithoutBackslash);
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
  
  if (fTranslationSettings->fGenerateComments) {
    S_lpsrBarLine barline = lpsrBarLine::create(fMeasureNumber); // JMI
    S_lpsrElement b = barline;
    addElementToPartSequence(b);
  }
}

void xmlPart2LpsrVisitor::visitEnd ( S_measure& elt ) 
{
//  stackClean(); // closes pending chords, cue and grace
//  checkVoiceTime (fCurrentMeasureLength, fCurrentVoicePosition);  

/*
  if (!fInhibitNextBar) {
    if (fTranslationSettings->fGenerateBars) fPendingBar = true;
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
    S_lpsrElement elt = lpsrElement ::create(comment);
    addElementToPartSequence (elt);
  }
  */
}

//______________________________________________________________________________
void xmlPart2LpsrVisitor::visitStart ( S_direction& elt ) 
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

void xmlPart2LpsrVisitor::visitEnd ( S_direction& elt ) 
{
 // fSkipDirection = false;
  fCurrentOffset = 0;
}

//______________________________________________________________________________

void xmlPart2LpsrVisitor::visitStart ( S_key& elt ) {
  fCurrentFifths = fCurrentCancel = 0;
  fCurrentMode = "";
}
void xmlPart2LpsrVisitor::visitStart ( S_cancel& elt )
  { fCurrentCancel = (int)(*elt); }
  
void xmlPart2LpsrVisitor::visitStart ( S_fifths& elt )
  { fCurrentFifths = (int)(*elt); }
  
void xmlPart2LpsrVisitor::visitStart ( S_mode& elt )
  { fCurrentMode = elt->getValue(); }

void xmlPart2LpsrVisitor::visitEnd ( S_key& elt ) 
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
      {
      stringstream s;
      std::string  message;
      s << 
        "ERROR: unknown key sign \"" << fCurrentFifths << "\"" << endl;
      s >> message;
      lpsrMusicXMLError(message);
      }
  } // switch
  
  // create lpsrKey and add it to part
  S_lpsrElement key = lpsrKey::create(tonicNote, keyMode);
  S_lpsrElement k = key;
  addElementToPartSequence (k);
}

//______________________________________________________________________________
void xmlPart2LpsrVisitor::visitStart ( S_clef& elt )
{ 
  fLine = kStandardLine;
  fOctaveChange = 0;
  fNumber = kNoNumber;
  fSign = "";

  fNumber = elt->getAttributeIntValue("number", kNoNumber); 
}

void xmlPart2LpsrVisitor::visitStart ( S_clef_octave_change& elt )
  { fOctaveChange = (int)(*elt); }
  
void xmlPart2LpsrVisitor::visitStart ( S_line& elt )
  { fLine = (int)(*elt); }
  
void xmlPart2LpsrVisitor::visitStart ( S_sign& elt )
  { fSign = elt->getValue(); }

//______________________________________________________________________________
void xmlPart2LpsrVisitor::visitEnd ( S_clef& elt ) 
{
  int staffnum = elt->getAttributeIntValue("number", 0); // JMI
  
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
    
  S_lpsrClef clef = lpsrClef::create(param);
  S_lpsrElement c = clef;
  addElementToPartSequence (c);
}

//________________________________________________________________________
void xmlPart2LpsrVisitor::visitStart ( S_metronome& elt )
{
  std::string parentheses = elt->getAttributeValue("parentheses");
  
  fBeatsData.clear();
  fPerMinute = 0;
  fCurrentBeat.fBeatUnit = "";
  fCurrentBeat.fDots = 0;

  if (parentheses.size()) {
    cout << "S_metronome, parentheses = " << parentheses << std::endl;
    
    if (parentheses == "yes") 
      fParentheses = true;
    else if (parentheses == "no")
      fParentheses = true;
    else {
      stringstream s;
      std::string  message;
      s << "parentheses value " << parentheses << " should be 'yes' or 'no'";
      s >> message;
      lpsrMusicXMLError (message);
    }
  }
}
  
void xmlPart2LpsrVisitor::visitEnd ( S_metronome& elt ) { 
 // if (fSkipDirection) return;

  if (fCurrentBeat.fBeatUnit.size()) {
    fBeatsData.push_back(fCurrentBeat);
    fCurrentBeat.fBeatUnit = "";
    fCurrentBeat.fDots = 0;
  }
  
  if (fBeatsData.size() != 1) {
    lpsrMusicXMLWarning(
      "multiple beats found, but only per-minute tempos is supported");
    return;         // support per minute tempo only (for now)
  }
  
  if (! fPerMinute) {
    lpsrMusicXMLWarning(
      "per-minute not found, only per-minute tempos is supported");
    return;    // support per minute tempo only (for now)
  }

  musicXMLBeatData b = fBeatsData[0];
  rational         r = 
    NoteType::type2rational(NoteType::xml(b.fBeatUnit)), rdot(3,2);
  
  while (b.fDots-- > 0) {
    r *= rdot;
  }
  r.rationalise();

  S_lpsrTempoCommand tempo =
    lpsrTempoCommand::create (r.getDenominator(), fPerMinute);
  fLpsrpart->getPartLpsrsequence()->appendElementToSequence (tempo);
  
 // JMI if (fCurrentOffset) addDelayed(cmd, fCurrentOffset);
}

void xmlPart2LpsrVisitor::visitStart ( S_beat_unit& elt ) { 
  if (fCurrentBeat.fBeatUnit.size()) {
    fBeatsData.push_back (fCurrentBeat); 
    fCurrentBeat.fBeatUnit = "";
    fCurrentBeat.fDots = 0;
  }
  fCurrentBeat.fBeatUnit = elt->getValue();
}

void xmlPart2LpsrVisitor::visitStart ( S_beat_unit_dot& elt )
  { fCurrentBeat.fDots++; }
  
void xmlPart2LpsrVisitor::visitStart ( S_per_minute& elt )
  { fPerMinute = (int)(*elt); }

//______________________________________________________________________________
void xmlPart2LpsrVisitor::visitStart ( S_coda& elt )
{
//  if (fSkipDirection) return;
//  S_lpsrElement cmd = lpsrcmd::create("coda");
 // addElementToPartSequence(cmd);
}

//______________________________________________________________________________
void xmlPart2LpsrVisitor::visitStart ( S_segno& elt )
{
//  if (fSkipDirection) return;
//  S_lpsrElement cmd = lpsrcmd::create("segno");
//  addElementToPartSequence(cmd);
}

//______________________________________________________________________________
void xmlPart2LpsrVisitor::visitStart ( S_trill_mark& elt )
{
//  S_lpsrElement cmd = lpsrcmd::create("\\trill");
//  addElementToPartSequence(cmd);
}

//______________________________________________________________________________
void xmlPart2LpsrVisitor::visitStart( S_dynamics& elt)
{}

void xmlPart2LpsrVisitor::visitStart( S_f& elt)
{        
  S_lpsrDynamics dyn = lpsrDynamics::create(lpsrDynamics::kF);
  fPendingDynamics.push_back(dyn);
 }
void xmlPart2LpsrVisitor::visitStart( S_ff& elt)
{        
  S_lpsrDynamics dyn = lpsrDynamics::create(lpsrDynamics::kFF);
  fPendingDynamics.push_back(dyn);
 }
void xmlPart2LpsrVisitor::visitStart( S_fff& elt)
{        
  S_lpsrDynamics dyn = lpsrDynamics::create(lpsrDynamics::kFFF);
  fPendingDynamics.push_back(dyn);
 }
void xmlPart2LpsrVisitor::visitStart( S_ffff& elt)
{        
  S_lpsrDynamics dyn = lpsrDynamics::create(lpsrDynamics::kFFFF);
  fPendingDynamics.push_back(dyn);
 }
void xmlPart2LpsrVisitor::visitStart( S_fffff& elt)
{        
  S_lpsrDynamics dyn = lpsrDynamics::create(lpsrDynamics::kFFFFF);
  fPendingDynamics.push_back(dyn);
 }
void xmlPart2LpsrVisitor::visitStart( S_ffffff& elt)
{        
  S_lpsrDynamics dyn = lpsrDynamics::create(lpsrDynamics::kFFFFFF);
  fPendingDynamics.push_back(dyn);
 }



void xmlPart2LpsrVisitor::visitStart( S_p& elt)
{        
  S_lpsrDynamics dyn = lpsrDynamics::create(lpsrDynamics::kP);
  fPendingDynamics.push_back(dyn);
}
void xmlPart2LpsrVisitor::visitStart( S_pp& elt)
{        
  S_lpsrDynamics dyn = lpsrDynamics::create(lpsrDynamics::kPP);
  fPendingDynamics.push_back(dyn);
}
void xmlPart2LpsrVisitor::visitStart( S_ppp& elt)
{        
  S_lpsrDynamics dyn = lpsrDynamics::create(lpsrDynamics::kPP);
  fPendingDynamics.push_back(dyn);
}
void xmlPart2LpsrVisitor::visitStart( S_pppp& elt)
{        
  S_lpsrDynamics dyn = lpsrDynamics::create(lpsrDynamics::kPPPP);
  fPendingDynamics.push_back(dyn);
}
void xmlPart2LpsrVisitor::visitStart( S_ppppp& elt)
{        
  S_lpsrDynamics dyn = lpsrDynamics::create(lpsrDynamics::kPPPPP);
  fPendingDynamics.push_back(dyn);
}
void xmlPart2LpsrVisitor::visitStart( S_pppppp& elt)
{        
  S_lpsrDynamics dyn = lpsrDynamics::create(lpsrDynamics::kPPPPPP);
  fPendingDynamics.push_back(dyn);
}

//______________________________________________________________________________
void xmlPart2LpsrVisitor::visitStart ( S_beam& elt )
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
  
  S_lpsrBeam beam = lpsrBeam::create(number, bk);;
  fCurrentBeam = beam;
}

//______________________________________________________________________________
void xmlPart2LpsrVisitor::visitStart ( S_wedge& elt )
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
  
  S_lpsrWedge wedg = lpsrWedge::create(wk);;
  fPendingWedges.push_back(wedg);
}

//______________________________________________________________________________
void xmlPart2LpsrVisitor::visitStart( S_octave_shift& elt)
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
void xmlPart2LpsrVisitor::visitEnd ( S_sound& elt )
{
 // if (fNotesOnly) return;

  S_lpsrElement cmd = 0;
  Sxmlattribute attribute;
  
}

//______________________________________________________________________________
void xmlPart2LpsrVisitor::visitEnd ( S_ending& elt )
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
void xmlPart2LpsrVisitor::visitEnd ( S_repeat& elt ) 
{
  S_lpsrElement cmd;
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
void xmlPart2LpsrVisitor::visitStart ( S_barline& elt ) 
{
  const string& location = elt->getAttributeValue("location");
  if (location == "middle") {
    // todo: handling bar-style (not yet supported in lpsr)
    S_lpsrBarLine barline = lpsrBarLine::create(1544442); // JMI
    S_lpsrElement b = barline;
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
void xmlPart2LpsrVisitor::visitStart ( S_print& elt ) 
{
  const string& newSystem = elt->getAttributeValue("new-system");
  if (newSystem == "yes") {
    // create a barnumbercheck command
    S_lpsrBarNumberCheck barnumbercheck_ = lpsrBarNumberCheck::create(fMeasureNumber);
    S_lpsrElement b2 = barnumbercheck_;
    addElementToPartSequence(b2);

    // create a break command
    S_lpsrBreak break_ = lpsrBreak::create(fMeasureNumber);
    S_lpsrElement b1 = break_;
    addElementToPartSequence(b1);
  }
}

//______________________________________________________________________________
void xmlPart2LpsrVisitor::visitStart ( S_step& elt )
{
  std::string value = elt->getValue();
  
  if (value.length() != 1) {
    stringstream s;
    std::string  message;
    s << "step value " << value << " is not a letter from A to G";
    s >> message;
    lpsrMusicXMLError (message);
  }

  char step = value[0];

  if (step <'A' || step > 'G') {
    stringstream s;
    std::string  message;
    s << "step value " << step << " is not a letter from A to G";
    s >> message;
    lpsrMusicXMLError (message);
  }

//  cout << "=== xmlPart2LpsrVisitor::visitStart ( S_step& elt ) " << fCurrentMusicXMLStep << std::endl;

  switch (step) {
    case 'A': fMusicXMLDiatonicPitch = lpsrNote::kA; break;
    case 'B': fMusicXMLDiatonicPitch = lpsrNote::kB; break;
    case 'C': fMusicXMLDiatonicPitch = lpsrNote::kC; break;
    case 'D': fMusicXMLDiatonicPitch = lpsrNote::kD; break;
    case 'E': fMusicXMLDiatonicPitch = lpsrNote::kE; break;
    case 'F': fMusicXMLDiatonicPitch = lpsrNote::kF; break;
    case 'G': fMusicXMLDiatonicPitch = lpsrNote::kG; break;
    default: {}
  } // switch

  fMusicXMLNoteData.fMusicxmlStep = step;
}

void xmlPart2LpsrVisitor::visitStart ( S_alter& elt)
{
  fMusicXMLNoteData.fMusicxmlAlteration = (int)(*elt);
}

void xmlPart2LpsrVisitor::visitStart ( S_octave& elt)
{
  fMusicXMLNoteData.fMusicxmlOctave = (int)(*elt);
}

void xmlPart2LpsrVisitor::visitStart ( S_duration& elt )
{
  fMusicXMLNoteData.fMusicxmlDuration = (int)(*elt);
//  cout << "=== xmlPart2LpsrVisitor::visitStart ( S_duration& elt ), fCurrentMusicXMLDuration = " << fCurrentMusicXMLDuration << std::endl;
}

void xmlPart2LpsrVisitor::visitStart ( S_dot& elt )
{
  fMusicXMLNoteData.fDotsNumber++;
}
       
//______________________________________________________________________________
void xmlPart2LpsrVisitor::visitStart ( S_voice& elt )
{
  fMusicXMLNoteData.fVoiceNumber = (int)(*elt);
}

void xmlPart2LpsrVisitor::visitStart ( S_type& elt )
{
  fCurrentType=elt->getValue();
}

void xmlPart2LpsrVisitor::visitStart ( S_stem& elt )
{
//  fCurrentStem = elt->getValue();
}

void xmlPart2LpsrVisitor::visitStart ( S_staff& elt )
{
  fCurrentStaff = (int)(*elt);
}

//______________________________________________________________________________
void xmlPart2LpsrVisitor::visitStart ( S_chord& elt)
{
  fMusicXMLNoteData.fNoteBelongsToAChord = true;
}

//______________________________________________________________________________
void xmlPart2LpsrVisitor::visitStart ( S_actual_notes& elt )
{
  fCurrentActualNotes = (int)(*elt);
}

void xmlPart2LpsrVisitor::visitStart ( S_normal_notes& elt )
{
  fCurrentNormalNotes = (int)(*elt);
}

void xmlPart2LpsrVisitor::visitStart ( S_normal_type& elt )
{
  fCurrentNormalType = elt->getValue();
}

void xmlPart2LpsrVisitor::visitStart ( S_tuplet& elt )
{
  fMusicXMLNoteData.fNoteBelongsToATuplet = true;

  fCurrentTupletNumber = atoi(elt->getAttributeValue("number").c_str());
  std::string type     = elt->getAttributeValue("type");
  
  /*
  cout <<
    "xmlPart2LpsrVisitor::visitStart ( S_tuplet, fCurrentTupletNumber = " <<
    fCurrentTupletNumber << ", type = " << type <<std::endl;
  */
  
  fCurrentTupletKind = lpsrTuplet::k_NoTuplet;
  
  if (type == "start")
    fCurrentTupletKind = lpsrTuplet::kStartTuplet;
  else if (type == "continue")
    fCurrentTupletKind = lpsrTuplet::kContinueTuplet;
  else if (type == "stop")
    fCurrentTupletKind = lpsrTuplet::kStopTuplet;
  else {
    stringstream s;
    std::string  message;
    s << "stuplet type " << type << " is unknown";
    s >> message;
    lpsrMusicXMLError (message);
  }
}

//______________________________________________________________________________
void xmlPart2LpsrVisitor::visitStart ( S_note& elt ) 
{
  //  cout << "--> xmlPart2LpsrVisitor::visitStart ( S_note& elt ) " << std::endl;
  fMusicXMLNoteData.fMusicxmlStep = '_';
  fMusicXMLNoteData.fMusicxmlStepIsARest = false;
  fMusicXMLNoteData.fMusicxmlAlteration = 0; // natural notes
  fMusicXMLNoteData.fMusicxmlOctave = -13;
  fMusicXMLNoteData.fDotsNumber = 0;
   
  // assume this note doesn't belong to a chord until S_chord is met
  fMusicXMLNoteData.fNoteBelongsToAChord = false;

  // assume this note doesn't belong to a tuplet until S_chord is met
  fMusicXMLNoteData.fNoteBelongsToATuplet = fATupletIsBeingBuilt;
}

void xmlPart2LpsrVisitor::createChord (S_lpsrDuration noteDuration) {
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
  std::list<S_lpsrDynamics> noteDynamics = fCurrentNote->getNoteDynamics();
  while (! noteDynamics.empty()) {
    //cout << "--> moving dynamics from fCurrentNote to fCurrentChord" << endl;
    S_lpsrDynamics dyn = noteDynamics.front();
    fCurrentChord->addDynamics(dyn);
    noteDynamics.pop_front();
  } // while
 
  // move the pending wedges if any from the first note to the chord
  std::list<S_lpsrWedge> noteWedges = fCurrentNote->getNoteWedges();
  while (! noteWedges.empty()) {
    //cout << "--> moving wedge from fCurrentNote to fCurrentChord" << endl;
    S_lpsrWedge wdg = noteWedges.front();
    fCurrentChord->addWedge(wdg);
    noteWedges.pop_front();
  } // while
}

void xmlPart2LpsrVisitor::createTuplet (S_lpsrNote note) {
  // create a tuplet element
  S_lpsrTuplet fCurrentTuplet = lpsrTuplet::create();
  fCurrentElement = fCurrentTuplet; // another name for it

  // populate it
  fCurrentTuplet->updateTuplet(
    fCurrentTupletNumber,
    fCurrentActualNotes,
    fCurrentNormalNotes);

  // register it in this visitor
  cout << "--> pushing tuplet to tuplets stack" << std::endl;
  fCurrentTupletsStack.push(fCurrentTuplet);
  
  // add note to the tuplet
  cout << "--> adding note " << note << " to tuplets stack top" << std::endl;
  fCurrentTuplet->addElementToTuplet(note);
}

void xmlPart2LpsrVisitor::finalizeTuplet (S_lpsrNote note) {
  // get tuplet from top of tuplet stack
  S_lpsrTuplet tup = fCurrentTupletsStack.top();

  // add note to the tuplet
  cout << "--> adding note " << note << " to tuplets stack top" << std::endl;
  tup->addElementToTuplet(note);

  // pop from the tuplets stack
  cout << "--> popping from tuplets stack" << std::endl;
  fCurrentTupletsStack.pop();        

  // add tuplet to the part
  cout << "=== adding tuplet to the part sequence" << std::endl;
  S_lpsrElement elem = tup;
  addElementToPartSequence(elem);
}          

//______________________________________________________________________________
void xmlPart2LpsrVisitor::visitEnd ( S_note& elt ) 
{
  //  cout << "<-- xmlPart2LpsrVisitor::visitEnd ( S_note& elt ) " << std::endl;

  if (fTranslationSettings->fDebug)
    std::cerr <<
      "fMusicXMLNoteData.fMusicxmlDuration = " << 
      fMusicXMLNoteData.fMusicxmlDuration << ", " << 
      "fCurrentDivisions*4 = " << fCurrentDivisions*4 << std::endl;
      
  if (fCurrentDivisions == 0)
    lpsrMusicXMLError ("divisions cannot be 0");
  
  fMusicXMLNoteData.fMusicxmlDivisions = fCurrentDivisions;
  fMusicXMLNoteData.fTupletMemberType = fCurrentType;
  
  //cout << "::: creating a note" << std::endl;
  S_lpsrNote note =
    lpsrNote::createFromMusicXMLData (
      fTranslationSettings, fMusicXMLNoteData);

  // attach the pending dynamics if any to the note
  if (! fPendingDynamics.empty()) {
/* JMI
    if (fMusicXMLNoteData.fMusicxmlStepIsARest)
      lpsrMusicXMLError (
        "dynamics cannot be attached to a rest, delayed until next note");
    else
*/
      while (! fPendingDynamics.empty()) {
        S_lpsrDynamics dyn = fPendingDynamics.front();
        note->addDynamics(dyn);
        fPendingDynamics.pop_front();
      } // while
  }
  
  // attach the pending wedges if any to the note
  if (! fPendingWedges.empty()) {
/* JMI
    if (fMusicXMLNoteData.fMusicxmlStepIsARest)
      lpsrMusicXMLError (
        "wedges cannot be attached to a rest, delayed until next note");
    else
*/
      while (! fPendingWedges.empty()) {
        S_lpsrWedge wdg = fPendingWedges.front();
        note->addWedge(wdg);
        fPendingWedges.pop_front();
      } // while
  }
          
  // a note can be standalone
  // or a member of a chord,
  // and the latter can belong to a tuplet
  // a rest can be standalone or belong to a tuplet
  
  if (fMusicXMLNoteData.fNoteBelongsToAChord) {
    
    if (fMusicXMLNoteData.fMusicxmlStepIsARest)
      lpsrMusicXMLError (
        "a rest cannot belong to a chord");
        
    if (! fAChordIsBeingBuilt) {
      // create a chord with fCurrentNote as its first note
      createChord (note->getNoteLpsrDuration());

      // account for chord being built
      fAChordIsBeingBuilt = true;
    }
    
    //cout << "--> adding note to fCurrentChord" << endl;
    // register note as a member of fCurrentChord
    fCurrentChord->addNoteToChord(note);
      
    // remove (previous) fCurrentNote that is the last element of the part sequence
    removeLastElementOfPartSequence();

    // add fCurrentChord to the part sequence instead
    S_lpsrElement elem = fCurrentChord;
    addElementToPartSequence(elem);

  } else if (fMusicXMLNoteData.fNoteBelongsToATuplet) {

    fMusicXMLNoteData.fTupletMemberType = fCurrentType;
    
    switch (fCurrentTupletKind) {
      case lpsrTuplet::kStartTuplet:
        {
          createTuplet(note);
          fATupletIsBeingBuilt = true;
        
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
          fATupletIsBeingBuilt = false;
        }
        break;
      default:
        {}
    } // switch

  } else {

    // cout << "--> adding standalone note/rest to part sequence" << endl;
    // register note as standalone
    S_lpsrElement n = note;
    addElementToPartSequence(n);
  
    // account for chord not being built
    fAChordIsBeingBuilt = false;
  }
  
   // keep track of note/rest in this visitor
  fCurrentNote    = note;
  fCurrentElement = fCurrentNote; // another name for it
}

//______________________________________________________________________________
void xmlPart2LpsrVisitor::visitStart ( S_rest& elt)
{
  //  cout << "--> xmlPart2LpsrVisitor::visitStart ( S_rest& elt ) " << std::endl;
  fMusicXMLNoteData.fMusicxmlStepIsARest = true;
}


}
