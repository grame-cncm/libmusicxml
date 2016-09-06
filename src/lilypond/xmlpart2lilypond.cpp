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

#include "conversions.h"
#include "partsummary.h"
#include "rational.h"
#include "xml2lilypondvisitor.h"
#include "xmlpart2lilypond.h"
#include "xml_tree_browser.h"
#include "lilypond.h"

using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
xmlpart2lilypond::xmlpart2lilypond( translationSwitches& sw ) : 
  fGenerateComments(sw.fGenerateComments),
  fGenerateStem(sw.fGenerateStems), 
  fGenerateBars(sw.fGenerateBars),
  fNotesOnly(false), // USER
  fCurrentStaffIndex(0), fCurrentStaff(0),
  fTargetStaff(0), fTargetVoice(0)
{
  fGeneratePositions = true;
  xmlpart2lilypond::reset();
}

//______________________________________________________________________________
void xmlpart2lilypond::reset ()
{
  lilypondnotestatus::resetall();
  fCurrentBeamNumber = 0;
  fMeasNum = 0;
  fInCue = fInGrace = fInhibitNextBar = fPendingBar 
       = fBeamOpened = fCrescPending = fSkipDirection = false;
  fCurrentStemDirection = kStemUndefined;
  fCurrentDivision = 1;
  fCurrentOffset = 0;
  fPendingPops = 0;
  fMeasNum = 0;
}

//______________________________________________________________________________
void xmlpart2lilypond::initialize (
  Slilypondelement seq,
  int staff,
  int lilypondstaff, 
  int voice, 
  bool notesonly,
  rational defaultTimeSign) 
{
  fCurrentStaff = fTargetStaff = staff; // the current and target staff
  fTargetVoice = voice;                 // the target voice
  fNotesOnly = notesonly;               // prevent multiple output for keys, clefs etc... 
  fCurrentTimeSign = defaultTimeSign;   // a default time signature
  fCurrentStaffIndex = lilypondstaff;   // the current lilypond staff index
  startPartStack (seq);
}

//________________________________________________________________________
// some code for the delayed elements management
// delayed elements are elements enclosed in a <direction> element that
// contains a non-null <offset> element. This offset postpones the graphic
// appearance of the element in 'offset' time units in the future.
// Time units are <division> time units 
//________________________________________________________________________
// add an element to the list of delayed elements
void xmlpart2lilypond::addDelayed (Slilypondelement elt, long offset) 
{
  addToPartStack(elt);
  return;
  
  if (offset > 0) {
    delayedElement de;
    de.fDelay = offset;
    de.fElement = elt;
    fDelayed.push_back(de);
  }
  else addToPartStack (elt);
}

//________________________________________________________________________
// checks ready elements in the list of delayed elements
// 'time' is the time elapsed since the last check, it is expressed in
// <division> time units
void xmlpart2lilypond::checkDelayed (long time)
{
  vector<delayedElement>::iterator it = fDelayed.begin();
  while (it!=fDelayed.end()) {
    it->fDelay -= time;
    if (it->fDelay < 0) {
      addToPartStack (it->fElement);
      it = fDelayed.erase(it);
    }
    else it++;
  }
}

//______________________________________________________________________________
void xmlpart2lilypond::stackClean () 
{
  if (fInCue) {
    popFromPartStack();      
    fInCue = false;
  }
  if (fInGrace) {
    popFromPartStack();      
    fInGrace = false;
  }
}

//______________________________________________________________________________
void xmlpart2lilypond::checkStaff (int staff)
{
  if (staff != fCurrentStaff) {
    Slilypondelement tag = lilypondcmd::create("new Staff {\n", lilypondcmd::kWithoutBackslash);
    
    int offset = staff - fCurrentStaff;
    //cout << "move from staff " << fCurrentStaffIndex << " to " << (fCurrentStaffIndex + offset) << endl;
    fCurrentStaff = staff;
    fCurrentStaffIndex += offset;
    // tag->add (lilypondparam::create(fCurrentStaffIndex, false)); // USER
    // JMI voir add (tag);
    }
}

//______________________________________________________________________________
void xmlpart2lilypond::moveMeasureTime (int duration, bool moveVoiceToo)
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
void xmlpart2lilypond::checkVoiceTime (
  const rational& currTime, const rational& voiceTime)
{
  rational diff = currTime - voiceTime;
  diff.rationalise();
  
  if (diff.getNumerator() > 0) {
    lilypondnoteduration dur (diff.getNumerator(), diff.getDenominator());
    Slilypondelement note = lilypondnote::create(fTargetVoice, "s", 0, dur, "");
    addToPartStack (note);
    fCurrentVoicePosition += diff;
    fCurrentVoicePosition.rationalise();
  }
  else if (diff.getNumerator() < 0)
  cerr << 
    "warning! checkVoiceTime: measure time behind voice time " << 
    string(diff) << 
    endl;
}

//______________________________________________________________________________
void xmlpart2lilypond::visitStart ( S_backup& elt )
{
  stackClean(); // closes pending chords, cue and grace
  int duration = elt->getIntValue(k_duration, 0);
  
  if (duration) {   
    // backup is supposed to be used only for moving between voices
    // thus we don't move the voice time (which is supposed to be 0)
    moveMeasureTime (-duration, false);
  }
}

//______________________________________________________________________________
void xmlpart2lilypond::visitStart ( S_forward& elt )
{
  bool scanElement = 
    (elt->getIntValue(k_voice, 0) == fTargetVoice) 
      && 
    (elt->getIntValue(k_staff, 0) == fTargetStaff);
  int duration = elt->getIntValue(k_duration, 0);
  
  moveMeasureTime(duration, scanElement);
  if (!scanElement) return;

  stackClean(); // closes pending chords, cue and grace

  if (duration) {   
    rational r(duration, fCurrentDivision*4);
    r.rationalise();
    lilypondnoteduration dur (r.getNumerator(), r.getDenominator());
    Slilypondelement note = 
      lilypondnote::create(fTargetVoice, "empty", 0, dur, "");
    addToPartStack (note);
    fMeasureEmpty = false;
  }
}

//______________________________________________________________________________
void xmlpart2lilypond::visitStart ( S_part& elt ) 
{
  reset();
  if (!current()) {
    Slilypondelement seq = lilypondseq::create();
    startPartStack (seq);
  }
}


//______________________________________________________________________________
void xmlpart2lilypond::visitStart ( S_measure& elt ) 
{
  const string& implicit = elt->getAttributeValue ("implicit");
  
  if (implicit == "yes") fPendingBar = false;
  
  if (fPendingBar) {
    // before adding a bar, we need to check that there are no repeat begin at this location
    ctree<xmlelement>::iterator repeat = elt->find(k_repeat);
    if ((repeat == elt->end()) || (repeat->getAttributeValue("direction") != "forward")) {
      checkStaff (fTargetStaff);
      Slilypondelement tag = lilypondcmd::create("|", lilypondcmd::kWithoutBackslash);
      addToPartStack (tag);
    }
  }
  
  fCurrentMeasure = elt;
  fMeasNum++;
  fCurrentMeasureLength.set  (0, 1);
  fCurrentMeasurePosition.set(0, 1);
  fCurrentVoicePosition.set  (0, 1);
  fInhibitNextBar = false; // fNotesOnly;
  fPendingBar = false;
  fPendingPops = 0;
  fMeasureEmpty = true;
  
  //* USER
  if (fGenerateComments) {
    stringstream s;
    s << "% " << fMeasNum; // USER
    string comment=s.str()+"\n";
    Slilypondelement elt = lilypondelement ::create(comment);
    addToPartStack (elt);
  }
  //*/
}

//______________________________________________________________________________
void xmlpart2lilypond::visitEnd ( S_measure& elt ) 
{
  stackClean(); // closes pending chords, cue and grace
  checkVoiceTime (fCurrentMeasureLength, fCurrentVoicePosition);  

  if (!fInhibitNextBar) {
    if (fGenerateBars) fPendingBar = true;
    else if (!fMeasureEmpty) {
      if (fCurrentVoicePosition < fCurrentMeasureLength)
        fPendingBar = true;
    }
  }
  
  /* USER
  if (fGenerateComments) {
    stringstream s;
    s << "% " << fMeasNum + 1; // USER
      // to announce number of first measure on next line
    string comment=s.str()+"\n";
    Slilypondelement elt = lilypondelement ::create(comment);
    addToPartStack (elt);
  }
  */

}

//______________________________________________________________________________
void xmlpart2lilypond::visitStart ( S_direction& elt ) 
{
  if (fNotesOnly || (elt->getIntValue(k_staff, 0) != fTargetStaff)) {
    fSkipDirection = true;
  }
  else {
    fCurrentOffset = elt->getLongValue(k_offset, 0);
  }
}

//______________________________________________________________________________
void xmlpart2lilypond::visitEnd ( S_direction& elt ) 
{
  fSkipDirection = false;
  fCurrentOffset = 0;
}

//______________________________________________________________________________
void xmlpart2lilypond::visitEnd ( S_key& elt ) 
{
  if (fNotesOnly) return;
  Slilypondelement tag = lilypondcmd::create("key");
  switch (keysignvisitor::fFifths) { // no backslashed for the following lilypondparam instances
    case 0:
      tag->add (lilypondparam::create("c \\major", false));
      break;
      ;;
    case 1:
      tag->add (lilypondparam::create("g \\major", false));
      break;
      ;;
    case 2:
      tag->add (lilypondparam::create("d \\major", false));
      break;
      ;;
    case 3:
      tag->add (lilypondparam::create("a \\major", false));
      break;
      ;;
    case 4:
      tag->add (lilypondparam::create("e \\major", false));
      break;
      ;;
    case 5:
      tag->add (lilypondparam::create("b \\major", false));
      break;
      ;;
    case 6:
      tag->add (lilypondparam::create("fis \\major", false));
      break;
      ;;
    case 7:
      tag->add (lilypondparam::create("cis \\major", false));
      break;
      ;;
    case -1:
      tag->add (lilypondparam::create("f \\major", false));
      break;
      ;;
    case -2:
      tag->add (lilypondparam::create("bes \\major", false));
      break;
      ;;
    case -3:
      tag->add (lilypondparam::create("ees \\major", false));
      break;
      ;;
    case -4:
      tag->add (lilypondparam::create("aes \\major", false));
      break;
      ;;
    case -5:
      tag->add (lilypondparam::create("des \\major", false));
      break;
      ;;
    case -6:
      tag->add (lilypondparam::create("ges \\major", false));
      break;
      ;;
    case -7:
      tag->add (lilypondparam::create("ces \\major", false));
      break;
      ;;
    default: { // unknown key sign !!
      cerr << 
        "warning: unknown key sign \"" <<
        keysignvisitor::fFifths << 
        "\"" <<
        endl;
      return; 
    }
  }
//  tag->add (lilypondparam::create(keysignvisitor::fFifths, false));
  addToPartStack (tag);
}

//______________________________________________________________________________
void xmlpart2lilypond::visitStart ( S_coda& elt )
{
  if (fSkipDirection) return;
  Slilypondelement tag = lilypondcmd::create("coda");
  addToPartStack(tag);
}

//______________________________________________________________________________
void xmlpart2lilypond::visitStart ( S_segno& elt )
{
  if (fSkipDirection) return;
  Slilypondelement tag = lilypondcmd::create("segno");
  addToPartStack(tag);
}

//______________________________________________________________________________
void xmlpart2lilypond::visitStart ( S_wedge& elt )
{
  string type = elt->getAttributeValue("type");
  lilypondwedge::WedgeKind wk;

  if (type == "crescendo") {
    wk = lilypondwedge::kCrescendoWedge;
    fCrescPending = true;
  }
  else if (type == "diminuendo") {
    wk = lilypondwedge::kDecrescendoWedge;
    fCrescPending = false;
  }
  else if (type == "stop") {
    //tag = lilypondcmd::create(fCrescPending ? "crescEnd" : "dimEnd"); // USER
    wk = lilypondwedge::kStopWedge;
  }
  
  Slilypondwedge wedg = lilypondwedge::create(wk);;
  fPendingWedges.push_back(wedg);
}

void xmlpart2lilypond::visitEnd ( S_wedge& elt )
{
  if (fSkipDirection) return;

  string type = elt->getAttributeValue("type");
  Slilypondelement elem;
  
  if (type == "crescendo") {
    elem = lilypondcmd::create("\\<", lilypondcmd::kWithoutBackslash);
    fCrescPending = true;
  }
  else if (type == "diminuendo") {
    elem = lilypondcmd::create("\\>", lilypondcmd::kWithoutBackslash);
    fCrescPending = false;
  }
  else if (type == "stop") {
    //tag = lilypondcmd::create(fCrescPending ? "crescEnd" : "dimEnd"); // USER
    elem = lilypondcmd::create("\\!", lilypondcmd::kWithoutBackslash);
  }
  
  if (elem) {
    if (fCurrentOffset) addDelayed(elem, fCurrentOffset);
    else addToPartStack (elem);
  }
}

//______________________________________________________________________________
void xmlpart2lilypond::visitEnd ( S_metronome& elt ) 
{
  if (fSkipDirection) return;

  metronomevisitor::visitEnd (elt);
  if (fBeats.size() != 1) return;         // support per minute tempo only (for now)
  if (!metronomevisitor::fPerMinute) return;    // support per minute tempo only (for now)

  Slilypondelement tag = lilypondcmd::create("tempo");
  beat b = fBeats[0];
  rational r = 
    NoteType::type2rational(NoteType::xml(b.fUnit)), rdot(3,2);
  
  while (b.fDots-- > 0) {
    r *= rdot;
  }
  r.rationalise();

  stringstream s;
  s << r.getDenominator() << " = " << metronomevisitor::fPerMinute; // USER
  tag->add (lilypondparam::create(s.str(), false)); // USER
  if (fCurrentOffset) addDelayed(tag, fCurrentOffset);
  addToPartStack (tag);
}

//______________________________________________________________________________
void xmlpart2lilypond::visitStart( S_dynamics& elt)
{
  if (fSkipDirection) return;

  ctree<xmlelement>::literator iter;
  for (iter = elt->lbegin(); iter != elt->lend(); iter++) {
    if ((*iter)->getType() != k_other_dynamics) {
      /* USER
      Slilypondelement tag = lilypondcmd::create("intens");
      tag->add (lilypondparam::create((*iter)->getName()));
      */
      /*
      stringstream s;
      s << "\\" << (*iter)->getName();
      Slilypondelement tag = lilypondcmd::create(s.str());
      */
      Slilypondelement tag = lilypondcmd::create((*iter)->getName()); // JMI
      if (fGeneratePositions) 
        xml2lilypondvisitor::addPosition(elt, tag, 12);
      if (fCurrentOffset) addDelayed(tag, fCurrentOffset);
      else addToPartStack (tag);
    }
  }
}

//______________________________________________________________________________
void xmlpart2lilypond::visitStart( S_octave_shift& elt)
{
  if (fSkipDirection) return;

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

  Slilypondelement tag = lilypondcmd::create("oct");
  if (tag) {
    tag->add (lilypondparam::create(size, false));
  //  addToPartStack (tag);
  
  // TODO: handling of octave offset with notes
  
  // USER ???
  // in addition, there is actually a poor support 
  // for the oct tag in lilypond 
  }
}

//______________________________________________________________________________
void xmlpart2lilypond::visitStart ( S_note& elt ) 
{
  notevisitor::visitStart ( elt );
}

//______________________________________________________________________________
string xmlpart2lilypond::alter2accident ( float alter ) 
{
  stringstream s;
  
  while (alter > 0.5) { // USER
    s << "is";
    alter -= 1;
  }
  while (alter < -0.5) {
    s << "es";
    alter += 1;
  }
  
  string accident;
  s >> accident;
  
  return accident;
}

//______________________________________________________________________________
void xmlpart2lilypond::visitEnd ( S_sound& elt )
{
  if (fNotesOnly) return;

  Slilypondelement tag = 0;
  Sxmlattribute attribute;
  
  if ((attribute = elt->getAttribute("dacapo")))
    tag = lilypondcmd::create("daCapo");
  else {
    if ((attribute = elt->getAttribute("dalsegno"))) {
      tag = lilypondcmd::create("dalSegno");
    }
    else if ((attribute = elt->getAttribute("tocoda"))) {
      tag = lilypondcmd::create("daCoda");
    }
    else if ((attribute = elt->getAttribute("fine"))) {
      tag = lilypondcmd::create("fine");
    }
  // if (tag) tag->add(lilypondparam::create("id="+attribute->getValue(), false));
  }
  
  if (tag) addToPartStack (tag);
}

//______________________________________________________________________________
void xmlpart2lilypond::visitEnd ( S_ending& elt )
{
  string type = elt->getAttributeValue("type");
  
  if (type == "start") {
    stringstream s; // USER
    string num = elt->getAttributeValue ("number");
    s << "repeat volta " << num << " {\n";
    Slilypondelement tag = lilypondcmd::create(s.str()); // USER
    //tag->add(lilypondparam::create(num, true));
    //tag->add(lilypondparam::create(num + ".", true));
    pushToPartStack(tag);
  }
  else {
    if (type == "discontinue")
      current()->add(lilypondparam::create("format=\"|-\"", false));
    popFromPartStack();
  }
}

//______________________________________________________________________________
void xmlpart2lilypond::visitEnd ( S_repeat& elt ) 
{
  Slilypondelement tag;
  string direction = elt->getAttributeValue("direction");
  
  if (direction == "forward") 
    tag = lilypondcmd::create("{ %repeatBegin \n"); // USER
  else if (direction == "backward") {
    tag = lilypondcmd::create("\n} %repeatEnd \n");
    fInhibitNextBar = true;
  }
  if (tag) addToPartStack(tag);  
}

//______________________________________________________________________________
void xmlpart2lilypond::visitStart ( S_barline& elt ) 
{
  const string& location = elt->getAttributeValue("location");
  if (location == "middle") {
    // todo: handling bar-style (not yet supported in lilypond)
    Slilypondelement tag = lilypondcmd::create("|", lilypondcmd::kWithoutBackslash);
    addToPartStack(tag);
  }
  // TODO: support for left and right bars
  // currently automatically handled at measure boundaries
  else if (location == "right") {
  }
  else if (location == "left") {
  }
}

//______________________________________________________________________________
void xmlpart2lilypond::visitEnd ( S_time& elt ) 
{
        //  cout << 0/0 << endl; // ABORT

  string timesign;
  
  if (!timesignvisitor::fSenzaMisura) {
    if (timesignvisitor::fSymbol == "common") {
      rational ts = timesignvisitor::timesign(0);
      /* USER
      if ((ts.getDenominator() == 2) && (ts.getNumerator() == 2))
        timesign = "C/";
      else if ((ts.getDenominator() == 4) && (ts.getNumerator() == 4))
        timesign = "C";
      else 
      */
        timesign = string(ts);
      fCurrentTimeSign = ts;
      }
    else if (timesignvisitor::fSymbol == "cut") {
      timesign = "C/";
      fCurrentTimeSign = rational(2,2);
      }
    else {
      stringstream s;
      string sep ="";
      fCurrentTimeSign.set(0,1);
      
      for (unsigned int i = 0; i < timesignvisitor::fTimeSign.size(); i++) {
        s << 
          sep << timesignvisitor::fTimeSign[i].first <<
          "/" << timesignvisitor::fTimeSign[i].second;
        sep = "+";
//        rational ts = timesignvisitor::timesign(i);
        fCurrentTimeSign += timesignvisitor::timesign(i);
      }
      s >> timesign;
    }
  }
  
  if (fNotesOnly) return;

  Slilypondelement tag = lilypondcmd::create("time"); // USER
  
  tag->add (lilypondparam::create(timesign, false)); // no quotes
  /* USER
  if (fGenerateBars) 
    tag->add (lilypondparam::create("autoBarlines=\"off\"", false));
    */
  addToPartStack(tag);
}

//______________________________________________________________________________
void xmlpart2lilypond::visitEnd ( S_clef& elt ) 
{
  int staffnum = elt->getAttributeIntValue("number", 0);
  
  if ((staffnum != fTargetStaff) || fNotesOnly) return;

  stringstream s; 

  // USER
  checkStaff (staffnum);

  if ( clefvisitor::fSign == "G") {
    if ( clefvisitor::fLine == 2 )
      s << "treble"; 
    else { // unknown G clef line !!
      cerr << 
        "warning: unknown G clef line \"" <<
        clefvisitor::fLine << 
        "\"" <<
        endl;
      return; 
      }
    }
  else if ( clefvisitor::fSign == "F") {
    if ( clefvisitor::fLine == 4 )
      s << "bass"; 
    else { // unknown F clef line !!
      cerr << 
        "warning: unknown F clef line \"" <<
        clefvisitor::fLine << 
        "\"" <<
        endl;
      return; 
      }
    }
  else if ( clefvisitor::fSign == "C") {
    if ( clefvisitor::fLine == 4 )
      s << "tenor"; 
    else if ( clefvisitor::fLine == 3 )
      s << "alto"; 
    else { // unknown C clef line !!
      cerr << 
        "warning: unknown C clef line \"" <<
        clefvisitor::fLine << 
        "\"" <<
        endl;
      return; 
      }
    }
  else if ( clefvisitor::fSign == "percussion") {
    s << "perc"; }
  else if ( clefvisitor::fSign == "TAB") {
    s << "TAB"; }
  else if ( clefvisitor::fSign == "none") {
    s << "none"; }
  else { // unknown clef sign !!
    cerr << 
      "warning: unknown clef sign \"" <<
       clefvisitor::fSign << 
       "\"" <<
      endl;
    return; 
  }
  
  string param;
  
  if (clefvisitor::fLine != clefvisitor::kStandardLine) 
    // s << clefvisitor::fLine; // USER
    s >> param;
    
  if (clefvisitor::fOctaveChange == 1)
    param += "_8"; // USER
  else if (clefvisitor::fOctaveChange == -1)
    param += "-8";
    
  Slilypondelement tag = lilypondcmd::create("clef");
  
  checkStaff (staffnum);
    tag->add (lilypondparam::create(param));
    addToPartStack(tag);
}

//______________________________________________________________________________
// tools and methods for converting notes
//______________________________________________________________________________
vector<S_slur>::const_iterator xmlpart2lilypond::findTypeValue (
  const std::vector<S_slur>& slurs, const string& val ) const 
{
  std::vector<S_slur>::const_iterator i;
  
  for (i = slurs.begin(); i != slurs.end(); i++) {
    if ((*i)->getAttributeValue("type") == val) break;
  }
  
  return i;
}

//______________________________________________________________________________
vector<S_tied>::const_iterator xmlpart2lilypond::findTypeValue ( 
  const std::vector<S_tied>& tied, const string& val ) const 
{
  std::vector<S_tied>::const_iterator i;
  
  for (i = tied.begin(); i != tied.end(); i++) {
    if ((*i)->getAttributeValue("type") == val) break;
  }
  
  return i;
}

//______________________________________________________________________________
vector<S_beam>::const_iterator xmlpart2lilypond::findValue (
  const std::vector<S_beam>& beams, const string& val ) const 
{
  std::vector<S_beam>::const_iterator i;
  
  for (i = beams.begin(); i != beams.end(); i++) {
    if ((*i)->getValue() == val) break;
  }
  return i;
}

//______________________________________________________________________________
void xmlpart2lilypond::checkTiedBegin ( const std::vector<S_tied>& tied ) 
{
  std::vector<S_tied>::const_iterator i = findTypeValue(tied, "start");
  
  if (i != tied.end()) {
    Slilypondelement tag = lilypondcmd::create("~"); // USER
    /*
    string num = (*i)->getAttributeValue ("number");
    if (num.size())
      tag->add (lilypondparam::create(num, false));
    string placement = (*i)->getAttributeValue("placement");
    if (placement == "below")
      tag->add (lilypondparam::create("curve=\"down\"", false));
      */
    addToPartStack(tag);
  }
}

void xmlpart2lilypond::checkTiedEnd ( const std::vector<S_tied>& tied ) 
{
  std::vector<S_tied>::const_iterator i = findTypeValue(tied, "stop");
  
  if (i != tied.end()) {
    Slilypondelement tag = lilypondcmd::create("~"); // USER
    /*
    string num = (*i)->getAttributeValue ("number");
    if (num.size())
      tag->add (lilypondparam::create(num, false));
      */
    addToPartStack(tag); // USER
  }
}

//______________________________________________________________________________
void xmlpart2lilypond::checkSlurBegin ( const std::vector<S_slur>& slurs ) 
{
  std::vector<S_slur>::const_iterator i = findTypeValue(slurs, "start");
  
  if (i != slurs.end()) {
    string tagName = "( "; // USER
    /*
    string num = (*i)->getAttributeValue("number");
    if (num.size()) tagName += ":" + num;
    */
    Slilypondelement tag = lilypondcmd::create(tagName);
    /*
    string placement = (*i)->getAttributeValue("placement");
    if (placement == "below")
      tag->add (lilypondparam::create("curve=\"down\"", false));
      */
    addToPartStack(tag);
  }
}

void xmlpart2lilypond::checkSlurEnd ( const std::vector<S_slur>& slurs ) 
{
  std::vector<S_slur>::const_iterator i = findTypeValue(slurs, "stop");
  if (i != slurs.end()) {
    string tagName = " )"; // USER
    /*
    string num = (*i)->getAttributeValue("number");
    
    if (num.size()) 
      tagName += ":" + num;
      */
    Slilypondelement tag = lilypondcmd::create (tagName);
    addToPartStack(tag);
  }
}

//______________________________________________________________________________
void xmlpart2lilypond::checkBeamBegin ( const std::vector<S_beam>& beams ) 
{
  std::vector<S_beam>::const_iterator i = findValue(beams, "begin");
  
  if (i != beams.end()) {
    if (!fBeamOpened ) {
      fCurrentBeamNumber = (*i)->getAttributeIntValue("number", 1);
//  USER ???    Slilypondelement tag = lilypondcmd::create("beamBegin");  // poor support of the begin end form in lilypond
//      add (tag);
      Slilypondelement tag = lilypondcmd::create("["); // USER
      
      pushToPartStack (tag);
      fBeamOpened = true;
    }
  }
}

void xmlpart2lilypond::checkBeamEnd ( const std::vector<S_beam>& beams ) 
{
  std::vector<S_beam>::const_iterator i;
  for (i = beams.begin(); (i != beams.end()) && fBeamOpened; i++) {
    if (
        ((*i)->getValue() == "end") 
          &&
        ((*i)->getAttributeIntValue("number", 1) == fCurrentBeamNumber)) {
      fCurrentBeamNumber = 0;
      popFromPartStack();
      fBeamOpened = false;
      }
  }
/*
  std::vector<S_beam>::const_iterator i = findValue(beams, "end");
  if (i != beams.end()) {
    if (fCurrentBeamNumber == (*i)->getAttributeIntValue("number", 1)) {
      fCurrentBeamNumber = 0;
//      Slilypondelement tag = lilypondcmd::create("beamEnd");  // poor support of the begin end form in lilypond
//      addToPartStack (tag);
      popFromPartStack();
      fBeamOpened = false;
    }
  }
*/
}

//______________________________________________________________________________
void xmlpart2lilypond::checkStem ( const S_stem& stem ) 
{
  Slilypondelement tag;
  
  if (stem) {
    if (stem->getValue() == "down") {
      if (fCurrentStemDirection != kStemDown) {
        // USER tag = lilypondcmd::create("stemDown");
        fCurrentStemDirection = kStemDown;
      }
    }
    else if (stem->getValue() == "up") {
      if (fCurrentStemDirection != kStemUp) {
        // tag = lilypondcmd::create("stemUp");
        fCurrentStemDirection = kStemUp;
      }
    }
    else if (stem->getValue() == "none") {
      if (fCurrentStemDirection != kStemNone) {
        // tag = lilypondcmd::create("stemsOff");
        fCurrentStemDirection = kStemNone;
      }
    }
    else if (stem->getValue() == "double") {
    }
  }
  else if (fCurrentStemDirection != kStemUndefined) {
    // tag = lilypondcmd::create("stemNeutral");
    fCurrentStemDirection = kStemUndefined;
  }
  
  if (tag) addToPartStack(tag);
}

//______________________________________________________________________________
int xmlpart2lilypond::checkArticulation ( const notevisitor& note ) 
{
  int n = 0;
  Slilypondelement tag;
  
  if (note.fAccent) { // USER
    tag = lilypondcmd::create("-'");
    pushToPartStack(tag);
    n++;
  }
  if (note.fStrongAccent) {
    tag = lilypondcmd::create("-!");
    pushToPartStack(tag);
    n++;
  }
  if (note.fStaccato) {
    tag = lilypondcmd::create("-.");
    pushToPartStack(tag);
    n++;
  }
  if (note.fTenuto) {
    tag = lilypondcmd::create("-_");
    pushToPartStack(tag);
    n++;
  }
  
  return n;
}

//______________________________________________________________________________
vector<Sxmlelement> xmlpart2lilypond::getChord ( const S_note& elt ) 
{
  vector<Sxmlelement> v;
  ctree<xmlelement>::iterator nextnote = 
    find(fCurrentMeasure->begin(), fCurrentMeasure->end(), elt);
    
  if (nextnote != fCurrentMeasure->end()) 
    nextnote++; // advance one step
    
  while (nextnote != fCurrentMeasure->end()) {
    // looking for the next note on the target voice
    if (
        (nextnote->getType() == k_note)
          &&
        (nextnote->getIntValue(k_voice,0) == fTargetVoice)) { 
      ctree<xmlelement>::iterator iter;     // and when there is one
      
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
void xmlpart2lilypond::checkCue (const notevisitor& nv) 
{
  if (nv.isCue()) {
    if (!fInCue) {
      fInCue = true;
      
      Slilypondelement tag = lilypondcmd::create("cue");
      
      pushToPartStack(tag);
    }
  }
  else if (fInCue) {
    fInCue = false;
    popFromPartStack();      
  }
}

//______________________________________________________________________________
void xmlpart2lilypond::checkGrace (const notevisitor& nv) 
{
  if (nv.isGrace()) {
    if (!fInGrace) {
      fInGrace = true;
      
      Slilypondelement tag = lilypondcmd::create("grace");
      
      pushToPartStack(tag);
    }
  }
  else if (fInGrace) {
    fInGrace = false;
    popFromPartStack();      
  }
}

//______________________________________________________________________________
int xmlpart2lilypond::checkFermata (const notevisitor& nv) 
{
  if (nv.inFermata()) {
    Slilypondelement tag = lilypondcmd::create("fermata");
    
    pushToPartStack(tag);
    return 1;
  }
  return 0;
}

//______________________________________________________________________________
string xmlpart2lilypond::noteName ( const notevisitor& nv )
{
  string accident = alter2accident(nv.getAlter());
  string name;
  
  if (nv.getType() == notevisitor::kRest)
    name="r";
  else {
    name = nv.getStep();
    
    if (!name.empty()) 
      name[0]=tolower(name[0]); // USER JMI : F0/4 -> f'
    else
     cerr << "warning: empty note name" << endl;
  }
  return name;
}

//______________________________________________________________________________
lilypondnoteduration xmlpart2lilypond::noteDuration ( const notevisitor& nv )
{
  lilypondnoteduration dur(0,0);
  
  if (nv.getType() == kRest) {
    rational r(nv.getDuration(), fCurrentDivision*4);
    r.rationalise();
    dur.set (r.getNumerator(), r.getDenominator());
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
void xmlpart2lilypond::newNote ( const notevisitor& nv ) 
{
  checkTiedBegin (nv.getTied());

  // JMI "c" in octave 4 in MusicXML is "c'" in LilyPond,
  // thus we have octave 0 for "c" and -1 for "c,"
  int octave = nv.getOctave() - 3;     
  string accident = alter2accident(nv.getAlter());
  string name = noteName(nv);
  lilypondnoteduration dur = noteDuration(nv);
  Slilypondelement note = lilypondnote::create(
    fTargetVoice, name, octave, dur, accident);

  addToPartStack (note);

  checkTiedEnd (nv.getTied());
}

//______________________________________________________________________________
void xmlpart2lilypond::visitEnd ( S_note& elt ) 
{
  notevisitor::visitEnd ( elt );

  if (inChord()) return;          // chord notes have already been handled

  bool scanVoice = (notevisitor::getVoice() == fTargetVoice);
  
  if (!isGrace()) {
    moveMeasureTime (getDuration(), scanVoice);
    checkDelayed (getDuration());   // check for delayed elements (directions with offset)
  }
  if (!scanVoice) return;

  checkStaff(notevisitor::getStaff());

  checkVoiceTime (fCurrentMeasurePosition, fCurrentVoicePosition);

  if (notevisitor::getType() != notevisitor::kRest)
    checkStem (notevisitor::fStem);
  checkCue(*this);    // inhibited due to poor support in lilypond (including crashes) // ENABLED // USER
  checkGrace(*this);
  checkSlurBegin (notevisitor::getSlur());
  checkBeamBegin (notevisitor::getBeam());
  
  int pendingPops  = checkFermata(*this);
  
  pendingPops += checkArticulation(*this);

  vector<Sxmlelement> chord = getChord(elt);
  if (chord.size()) {
    Slilypondelement chord = lilypondchord::create();
    pushToPartStack (chord);
    pendingPops++;
  }
  
  newNote (*this);
  
  for (
      vector<Sxmlelement>::const_iterator iter = chord.begin(); 
      iter != chord.end(); 
      iter++) {
    notevisitor nv;
    xml_tree_browser browser(&nv);
    Sxmlelement note = *iter;
    browser.browse(*note);
    checkStaff(nv.getStaff());
    newNote (nv);   
  } 
  while (pendingPops--) popFromPartStack();
  
  checkBeamEnd (notevisitor::getBeam());
  checkSlurEnd (notevisitor::getSlur());
  if (notevisitor::fBreathMark) {
    Slilypondelement tag = lilypondcmd::create("breath"); // USER
    addToPartStack(tag);
  }

/*
  if (fPendingWedges.size()) {
    while (fPendingWedges.size()) {
      cout << "% FOO ";
      Slilypondwedge wedg = fPendingWedges.front();
      if (wedg) {
        switch (wedg->getWedgeKind()) {
          case lilypondwedge::kCrescendoWedge:
            cout << "\\<";
            break;
          case lilypondwedge::kDecrescendoWedge:
            cout << "\\>";
            break;
          case lilypondwedge::kStopWedge:
            cout << "\\!";
            break;
        } // switch
      } // while
      fPendingWedges.pop_front();
    }
    cout << endl;
  }
*/

  fMeasureEmpty = false;
}

/*
 * void xmlelement::acceptIn(basevisitor& v) {
  visitor<Sxmlelement>* p = dynamic_cast<visitor<Sxmlelement>*>(&v);
  if (p) {
    Sxmlelement xml = this;
    p->visitStart (xml);
  }
}
*/

//______________________________________________________________________________
// time management
//______________________________________________________________________________
void xmlpart2lilypond::visitStart ( S_divisions& elt ) 
{
  fCurrentDivision = (long)(*elt);
}

}
