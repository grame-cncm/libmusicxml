/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <sstream>
#include <stdlib.h>     /* abort, NULL */
#include <algorithm>    /* for_each */

#include "xml_tree_browser.h"

#include "conversions.h"

#include "msr.h"
#include "xml2MsrScoreVisitor.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
xml2MsrScoreVisitor::xml2MsrScoreVisitor (
  S_translationSettings& ts)
{
  fTranslationSettings = ts;
  
  // create the score
  fMsrScore =
    msrScore::create (fTranslationSettings);

  /*
  A first part is created with an empty name and
  all the nneded contents.
  It will be reused when the first actual part is met,
  changing its name on the fly
  in msrPartGroup::tryAndReUseInitialAnonymousPart()
  */


  cerr <<
    "Creating a full-fledged anonymous part group in case the MusicXML is poor" << endl;
    
  idtr++;

  // add the anonymous part group to the ccore
  fCurrentPartGroup =
    fMsrScore->
      addPartGroupToScore (1);

  // add the anonymous part to the anonymous part group
  string partXMLName = "";
  fCurrentPart =
    fCurrentPartGroup->
      addPartToPartGroup (partXMLName);

  // add a staff to the anonymous part
  fCurrentStaffNumber = 1;
  fCurrentStaff =
    fCurrentPart->
      addStaffToPart (fCurrentStaffNumber);

  // add a voice to the staff
  fCurrentVoice =
    fCurrentStaff->
      addVoiceToStaff (1);

  // add a lyrics to the voice
  fCurrentLyrics =
    fCurrentVoice->
      addLyricsToVoice (1);
  
  fCurrentMeasureNumber = 0;

  fOnGoingChord = false;
  
  fOnGoingTuplet = false;

  fCurrentBackupDuration = -1;

  fOnGoingNote = false;

  fOnGoingChord = false;
  
  fOnGoingSlur = false;

  fOnGoingBackup  = false;
  fOnGoingForward = false;

  idtr--;
}

xml2MsrScoreVisitor::~xml2MsrScoreVisitor ()
{}

//________________________________________________________________________
S_msrScore
xml2MsrScoreVisitor::buildMsrScoreFromXMLElementTree (
  const Sxmlelement& xmlTree)
{
  S_msrScore result;
  
  if (xmlTree) {
    // create a tree browser on this visitor
    tree_browser<xmlelement> browser (this);
    
    // browse the xmlelement tree
    browser.browse (*xmlTree);
    
    // grab the resulting Score
    result = fMsrScore;
  }

  return result;
}

void xml2MsrScoreVisitor::internalError (
  string message)
{
  cerr <<
    "### Internal error: measure " << fCurrentMeasureNumber << endl <<
    message << endl;
}                        

//________________________________________________________________________
void xml2MsrScoreVisitor::resetCurrentTime ()
{
  fCurrentKeyStaffNumber = -1;
  fCurrentTimeStaffNumber = -1;
  fCurrentClefStaffNumber = -1;
  
  fCurrentTimeSenzaMisura = false;

  fCurrentTimeBeats = 0;
  fCurrentTimeBeatType = 0;
  
//  fTimeSignatures.clear();
  fCurrentTimeSymbol = "";
}

//________________________________________________________________________
void xml2MsrScoreVisitor::visitStart (S_part_list& elt)
{
  cerr << "Analysing part list" << endl;

  idtr++;
}
/*
  <part-list>
    <part-group number="1" type="start">
      <group-symbol default-x="-7">bracket</group-symbol>
      <group-barline>yes</group-barline>
    </part-group>
    <score-part id="P1">
      <part-name>Piccolo</part-name>
      <part-abbreviation>Picc.</part-abbreviation>
      <score-instrument id="P1-I18">
        <instrument-name>Picc.</instrument-name>
      </score-instrument>
      <midi-instrument id="P1-I18">
        <midi-channel>1</midi-channel>
        <midi-program>73</midi-program>
      </midi-instrument>
    </score-part>
    <part-group number="2" type="start">
      <group-name>1
2</group-name>
      <group-barline>yes</group-barline>
    </part-group>
    <score-part id="P2">
*/

void xml2MsrScoreVisitor::visitEnd (S_part_list& elt)
{
  idtr--;
}

//________________________________________________________________________

/*
    <part-group type="start" number="1">
    </part-group>
    <part-group type="stop" number="1"/>
 */

S_msrPartGroup xml2MsrScoreVisitor::fetchScorePartGroup (
  int partGroupNumber)
{
  S_msrPartGroup result;
  
  if (fPartGroupsMap.count (partGroupNumber)) {
    result = fPartGroupsMap [partGroupNumber];
  }

  return result;
}

void xml2MsrScoreVisitor::visitStart (S_part_group& elt)
{
  /*
    <part-group number="1" type="start">
    <group-name>Trombones</group-name>
    <group-abbreviation>Trb.</group-abbreviation>
    <group-symbol default-x="-12">brace</group-symbol>
    <group-barline>yes</group-barline>
    </part-group>
  */
  
  // the part group number indicates nested/overlapping groups
  int partGroupNumber =
    elt->getAttributeIntValue ("number", 0);
    
  string partGroupType =
    elt->getAttributeValue ("type");

  if (fTranslationSettings->fTrace)
    cerr << idtr <<
      "Handling part group " << partGroupNumber <<
      ", type: \"" << partGroupType << "\""  << endl;

  idtr++;
  
  if (partGroupType == "start") {

    // is this part group number already present?
    fCurrentPartGroup =
      fetchScorePartGroup (partGroupNumber);

    // no, add it to the score
    if (! fCurrentPartGroup) 
    fCurrentPartGroup =
      fMsrScore->
        addPartGroupToScore (partGroupNumber);

    // add it to the map of this visitor
    if (fTranslationSettings->fTrace)
      cerr <<
        "Adding part group " << partGroupNumber <<
        " to visitor's part group map" << endl;
    fPartGroupsMap [partGroupNumber] = fCurrentPartGroup;
    
    if (true || fTranslationSettings->fDebug) {
  //  if (fTranslationSettings->fDebug) {
      cerr << idtr <<
        "==> Now, fPartGroupsMap contains:" << endl;
      idtr++;
      for (
          msrPartGroupsMap::const_iterator i = fPartGroupsMap.begin();
          i != fPartGroupsMap.end();
          i++) {
        cerr << idtr <<
          (*i).first << endl;
    //      "\"" << (*i).first << "\" ----> " <<
    //      (*i).second->getPartCombinedName() << endl;
      } // for
      idtr--;
      cerr << idtr << "<== fPartGroupsMap" << endl;
    }
        
  } else if (partGroupType == "stop") {

    // remove part group from the map,
    // it remains is the score's part group list
    if (fTranslationSettings->fTrace)
      cerr <<
        "Removing part group " << partGroupNumber <<
        " from visitor's part group map" << endl;
    fPartGroupsMap.erase (partGroupNumber);
    
    if (true || fTranslationSettings->fDebug) {
  //  if (fTranslationSettings->fDebug) {
      cerr << idtr <<
        "==> Now, fPartGroupsMap contains:" << endl;
      idtr++;
      for (
          msrPartGroupsMap::const_iterator i = fPartGroupsMap.begin();
          i != fPartGroupsMap.end();
          i++) {
        cerr << idtr <<
          (*i).first << endl;
    //      "\"" << (*i).first << "\" ----> " <<
    //      (*i).second->getPartCombinedName() << endl;
      } // for
      idtr--;
      cerr << idtr << "<== fPartGroupsMap" << endl;
    }
        
  } else {
    
    msrMusicXMLError (
      "unknown part group type \"" + partGroupType + "\"");
  }
}

void xml2MsrScoreVisitor::visitEnd (S_part_group& elt)
{
  idtr--;
}

void xml2MsrScoreVisitor::visitStart (S_group_name& elt)
{
  string groupName = elt->getValue();

  fCurrentPartGroup->setPartGroupName (groupName);
}

void xml2MsrScoreVisitor::visitStart (S_group_abbreviation& elt)
{
  string groupAbbreviation = elt->getValue ();

  fCurrentPartGroup->setPartGroupAbbreviation (groupAbbreviation);
}

void xml2MsrScoreVisitor::visitStart (S_group_symbol& elt)
{
  // occurs after "<part-group number="nnn" type="start"> in MusicXML
  // that has set fCurrentPartGroup

  string partGroupSymbol = elt->getValue ();

  fCurrentPartGroup->setPartGroupSymbol (partGroupSymbol);
}

void xml2MsrScoreVisitor::visitStart ( S_group_barline& elt)
{
  string groupBarline = elt->getValue ();

  fCurrentPartGroup->
    setPartGroupBarline (groupBarline);
}

//________________________________________________________________________
void xml2MsrScoreVisitor::visitStart (S_score_part& elt)
{
  string partMusicXMLName = elt->getAttributeValue ("id");

  if (fTranslationSettings->fTrace)
    cerr << idtr <<
      "Found part name \"" << partMusicXMLName << "\"" << endl;

  if (! fCurrentPartGroup) {
    cerr << idtr <<
      "Adding an implicit part group to the score" << endl;
      
    fCurrentPartGroup =
      fMsrScore->
        addPartGroupToScore (1);
  }

  // is this part already present in the current part group?
  fCurrentPart =
    fCurrentPartGroup->
      fetchPartFromPartGroup (
        partMusicXMLName);

  // no, add it to the current part group
  if (! fCurrentPart) {
    fCurrentPart =
      fCurrentPartGroup->
        addPartToPartGroup (
          partMusicXMLName);
  }
}

void xml2MsrScoreVisitor::visitStart (S_part_name& elt)
{
  string partName = elt->getValue ();

  fCurrentPart->
    setPartName (partName);
}

void xml2MsrScoreVisitor::visitStart (S_part_abbreviation& elt)
{
  string fCurrentPartAbbreviation = elt->getValue ();

  fCurrentPart->
    setPartAbbreviation (fCurrentPartAbbreviation);
}

void xml2MsrScoreVisitor::visitStart (S_instrument_name& elt)
{
  string fCurrentPartInstrumentName = elt->getValue(); // jMI

  fCurrentPart->
    setPartInstrumentName (fCurrentPartInstrumentName);
}

//________________________________________________________________________
void xml2MsrScoreVisitor::visitStart (S_part& elt)
{
  string partID = elt->getAttributeValue ("id");

  // is this part already present?
  fCurrentPart =
    fCurrentPartGroup->
      fetchPartFromPartGroup (partID);

  // no, add it to the current part group
  if (! fCurrentPart) 
    fCurrentPart =
      fCurrentPartGroup->
        addPartToPartGroup (partID);

  if (fTranslationSettings->fTrace)
    cerr << idtr <<
      "Analyzing part " << fCurrentPart->getPartCombinedName() << endl;

  fCurrentStaffNumber = 1; // default if there are no <staff> element

  // is this staff already present?
  fCurrentStaff =
    fCurrentPart->
      fetchStaffFromPart (fCurrentStaffNumber);

  // no, add it to the current part
  if (! fCurrentStaff) 
    fCurrentStaff =
      fCurrentPart->
        addStaffToPart (fCurrentStaffNumber);

  fCurrentMeasureNumber = 0; // in case of an anacrusis

  idtr++;
}

void xml2MsrScoreVisitor::visitEnd (S_part& elt)
{
  idtr--;
}

//______________________________________________________________________________
void xml2MsrScoreVisitor::visitStart ( S_divisions& elt ) 
{
  fCurrentMusicXMLDivisions = (int)(*elt);
  
  if (fTranslationSettings->fTrace) {
    cerr << idtr;
    if (fCurrentMusicXMLDivisions == 1)
      cerr << "There is 1 division";
    else
      cerr <<
        "There are " << fCurrentMusicXMLDivisions <<
        " divisions";
    cerr <<
      " per quater note in part " <<
      fCurrentPart->getPartCombinedName() << endl;
  }

  fCurrentPart->setPartDivisions (fCurrentMusicXMLDivisions);
}

//______________________________________________________________________________

void xml2MsrScoreVisitor::visitStart ( S_key& elt ) {
  // The optional number attribute refers to staff numbers.
  // If absent (-1), apply to all part staves.
  fCurrentKeyStaffNumber =
    elt->getAttributeIntValue ("number", -1);

  if (fCurrentKeyStaffNumber < 0) {
    // number was not found JMI
  }
  
  fCurrentFifths = 0;
  fCurrentCancel = 0;
  fCurrentMode   = "";
}
  
void xml2MsrScoreVisitor::visitStart ( S_fifths& elt )
  { fCurrentFifths = (int)(*elt); }
  
void xml2MsrScoreVisitor::visitStart ( S_mode& elt )
  { fCurrentMode = elt->getValue(); }

void xml2MsrScoreVisitor::visitStart ( S_cancel& elt )
  { fCurrentCancel = (int)(*elt); }

void xml2MsrScoreVisitor::visitEnd ( S_key& elt ) 
{    
  // create msrKey
  S_msrKey
    key =
      msrKey::create (fCurrentFifths, fCurrentMode, fCurrentCancel);

  if (fCurrentKeyStaffNumber == -1)
    fCurrentPart->setAllPartStavesKey (key);
  else {
    S_msrStaff
      staff =
        fCurrentPart->
          fetchStaffFromPart (fCurrentKeyStaffNumber);
    staff->setStaffKey (key);
  }
}

//______________________________________________________________________________
void xml2MsrScoreVisitor::visitStart ( S_time& elt ) {
  resetCurrentTime(); // JMI ???
  
  // The optional number attribute refers to staff numbers.
  // If absent (-1), apply to all part staves.
  fCurrentTimeStaffNumber =
    elt->getAttributeIntValue ("number", -1);
    
  fCurrentTimeSymbol =
    elt->getAttributeValue ("symbol");
  // time symbol="cut" or "common" JMI
}

void xml2MsrScoreVisitor::visitStart ( S_beats& elt )
{
  /*
The optional number attribute refers to staff numbers,
    from top to bottom on the system. If absent, the key
    signature applies to all staves in the part.
   */

  fCurrentTimeBeats = (int)(*elt); }
  
void xml2MsrScoreVisitor::visitStart ( S_beat_type& elt )
  { fCurrentTimeBeatType = (int)(*elt); }
 
void xml2MsrScoreVisitor::visitStart ( S_senza_misura& elt )
  { fCurrentTimeSenzaMisura = true; }

/*
rational xml2MsrScoreVisitor::timeSignatureFromIndex(int index) JMI
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

void xml2MsrScoreVisitor::visitEnd ( S_time& elt ) 
{
  S_msrTime
    time =
      msrTime::create (
        fCurrentTimeBeats,
        fCurrentTimeBeatType,
        fTranslationSettings->fGenerateNumericalTime);

  if (fCurrentTimeStaffNumber == -1)
    fCurrentPart->setAllPartStavesTime (time);
  else {
    S_msrStaff
      staff =
        fCurrentPart->
          fetchStaffFromPart (fCurrentTimeStaffNumber);
    staff->setStaffTime (time);
  }
}

//______________________________________________________________________________
void xml2MsrScoreVisitor::visitStart ( S_clef& elt )
{ 
  // The optional number attribute refers to staff numbers.
  // If absent (-1), apply to all part staves.
  fCurrentClefStaffNumber =
    elt->getAttributeIntValue("number", -1); 

  fCurrentClefLine = 0;;
  fCurrentClefOctaveChange = 0;
  fCurrentClefSign = "";
}

void xml2MsrScoreVisitor::visitStart ( S_clef_octave_change& elt )
  { fCurrentClefOctaveChange = (int)(*elt); }
  
void xml2MsrScoreVisitor::visitStart ( S_line& elt )
  { fCurrentClefLine = (int)(*elt); }
  
void xml2MsrScoreVisitor::visitStart ( S_sign& elt )
  { fCurrentClefSign = elt->getValue(); }

void xml2MsrScoreVisitor::visitEnd ( S_clef& elt ) 
{  
  S_msrClef
    clef =
      msrClef::create (
        fCurrentClefSign, fCurrentClefLine, fCurrentClefStaffNumber);

  if (fCurrentClefStaffNumber == -1)
    fCurrentPart->setAllPartStavesClef (clef);
  else {
    S_msrStaff
      staff =
        fCurrentPart->
          fetchStaffFromPart (fCurrentClefStaffNumber);
    staff->setStaffClef (clef);
  }
}

//________________________________________________________________________
void xml2MsrScoreVisitor::visitStart (S_staves& elt)
{
  int stavesNumber = int(*elt);

  if (fTranslationSettings->fTrace) {
    switch (stavesNumber) {
      case 0:
        cerr << idtr <<
          "There isn't any explicit staff (hence 1 by default)"; // JMI
        break;
      case 1:
        cerr << idtr <<
          "There is 1 staff";
        break;
      default:
        cerr << idtr <<
          "There are " << stavesNumber << " staves";
    } // switch
    cerr <<
      " in part " << fCurrentPart->getPartCombinedName() << endl;
  }

  if (stavesNumber > 1) {
    // add n-1 staves to current part
    int n = 2;
    
    while (n <= stavesNumber) {
      fCurrentPart->addStaffToPart (n);
      n++;
    } // while
  }
}

void xml2MsrScoreVisitor::visitStart (S_staff& elt)
{
  /*
        <note>
        <pitch>
          <step>A</step>
          <octave>3</octave>
        </pitch>
        <duration>2</duration>
        <voice>3</voice>
        <type>eighth</type>
        <stem>down</stem>
        <staff>2</staff>
        <beam number="1">end</beam>
      </note>
*/
  int  staffNumber = int(*elt);

//  if (true || fTranslationSettings->fDebug)
  if (fTranslationSettings->fDebug)
    cerr <<
      idtr <<
      "--> S_staff, staffNumber         = " << staffNumber << endl <<
      idtr <<
      "--> S_staff, fCurrentStaffNumber = " << fCurrentStaffNumber << endl <<
      idtr <<
      "--> S_staff, current staff name  = " << fCurrentStaff->getStaffName() << endl;

  if (fOnGoingForward) {

    fCurrentForwardStaffNumber = staffNumber;

  } else if (fOnGoingNote) {

    // regular staff indication in note/rest
    fCurrentStaffNumber = staffNumber;
    
  } else {
    
    cerr << "##### staff " << staffNumber << " is out of context" << endl;
    
    stringstream s;
    string       message;
    s << "staff " << staffNumber << " is out of context";
    s >> message;
    // JMI msrMusicXMLError (message);
    
  }
}
    
//________________________________________________________________________
void xml2MsrScoreVisitor::visitStart (S_voice& elt )
{
  /*
        <note>
        <pitch>
          <step>A</step>
          <octave>3</octave>
        </pitch>
        <duration>2</duration>
        <voice>3</voice>
        <type>eighth</type>
        <stem>down</stem>
        <staff>2</staff>
        <beam number="1">end</beam>
      </note>
*/
  int voiceNumber = int(*elt);
  
//  if (true || fTranslationSettings->fDebug)
  if (fTranslationSettings->fDebug)
    cerr <<
      idtr <<
      "--> S_voice, voiceNumber         = " << voiceNumber << endl <<
      idtr <<
      "--> S_voice, fCurrentStaffNumber = " << fCurrentStaffNumber << endl <<
      idtr <<
      "--> S_voice, current staff name  = " << fCurrentStaff->getStaffName() << endl;

  if (fOnGoingForward) {

    fCurrentForwardVoiceNumber = voiceNumber;

  } else if (fOnGoingNote) {

    // regular voice indication in note/rest
    fCurrentVoiceNumber = voiceNumber;
    
  } else {
    
    stringstream s;
    string       message;
    s << "voice " << voiceNumber << " is out of context";
    s >> message;
    msrMusicXMLError (message);
    
  }
}

//________________________________________________________________________
void xml2MsrScoreVisitor::visitStart (S_backup& elt )
{
  /*
        <backup>
        <duration>8</duration>
      </backup>
*/
  
  fOnGoingBackup = true;
}

void xml2MsrScoreVisitor::visitEnd (S_backup& elt )
{
  if (fTranslationSettings->fTrace)
    cerr << idtr <<
      "Handling 'backup " << fCurrentBackupDuration <<
      " divisions'" << endl;

  fOnGoingBackup = false;
}

//______________________________________________________________________________
void xml2MsrScoreVisitor::visitStart ( S_forward& elt )
{
  /*
         <forward>
        <duration>96</duration>
        <voice>1</voice>
        <staff>1</staff>
      </forward>
*/
  fOnGoingForward = true;
}

void xml2MsrScoreVisitor::visitEnd ( S_forward& elt )
{
  // changing voice and staff
  fCurrentVoiceNumber = fCurrentForwardVoiceNumber;
  fCurrentStaffNumber = fCurrentForwardStaffNumber;


  if (fTranslationSettings->fTrace)
    cerr << idtr <<
      "Handling 'forward " << fCurrentForwardDuration <<
      "', thus switching to " <<
      "voice " << fCurrentVoiceNumber <<
      " in staff " << fCurrentStaffNumber << endl;

  
  // is the new voice already present?
  fCurrentVoice =
    fCurrentStaff->
      fetchVoiceFromStaff (fCurrentVoiceNumber);

  // no, add it to the current staff
  if (! fCurrentVoice) 
    fCurrentVoice =
      fCurrentStaff->
        addVoiceToStaff (fCurrentVoiceNumber);

  // is the new staff already present?
  fCurrentStaff =
    fCurrentPart->
      fetchStaffFromPart (fCurrentStaffNumber);

  // no, add it to the current part
  if (! fCurrentStaff) 
    fCurrentStaff =
      fCurrentPart->
        addStaffToPart (fCurrentStaffNumber);

  fOnGoingForward = false;
}

/*
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
    msrDuration dur (r.getNumerator(), r.getDenominator(), 57, "////"); // JMI
    / *
    S_msrElement note = 
      msrNote::create();//(fTargetVoice); // JMI , "empty", 0, dur, "");
    fCurrentVoice->appendElementToVoiceSequence (note);
 //   fMeasureEmpty = false;
  }
 */

//________________________________________________________________________
void xml2MsrScoreVisitor::visitStart ( S_metronome& elt )
{
  string parentheses = elt->getAttributeValue("parentheses");
  
  fBeatsData.clear();
  fPerMinute = 0;
  fCurrentBeat.fBeatUnit = "";
  fCurrentBeat.fDots = 0;

  if (parentheses.size()) {
    // cout << "--> S_metronome, parentheses = " << parentheses << endl;
    
    if (parentheses == "yes") 
      fParentheses = true;
    else if (parentheses == "no")
      fParentheses = true;
    else {
      stringstream s;
      string  message;
      s << "parentheses value " << parentheses << " should be 'yes' or 'no'";
      s >> message;
      msrMusicXMLError (message);
    }
  }
}
  
void xml2MsrScoreVisitor::visitEnd ( S_metronome& elt )
{ 
 // if (fSkipDirection) return;

  // fParentheses ??? JMI
  if (fCurrentBeat.fBeatUnit.size()) { // JMI
    fBeatsData.push_back(fCurrentBeat);
    fCurrentBeat.fBeatUnit = "";
    fCurrentBeat.fDots = 0;
  }
  
  if (fBeatsData.size() != 1) {
    msrMusicXMLWarning(
      "multiple beats found, but only per-minute tempos is supported");
    return;         // support per minute tempo only (for now)
  }
  
  if (! fPerMinute) {
    msrMusicXMLWarning(
      "per-minute not found, only per-minute tempos is supported");
    return;    // support per minute tempo only (for now)
  }

  musicXMLBeatData b = fBeatsData[0];
  rational         r = 
    NoteType::type2rational(
      NoteType::xml (b.fBeatUnit)), rdot(3,2);
  
  while (b.fDots-- > 0) {
    r *= rdot;
  }
  r.rationalise();

//  S_msrTempoCommand tempo =
  //  msrTempoCommand::create (r.getDenominator(), fPerMinute);
    
 // JMI fCurrentVoice->appendElementToVoiceSequence (tempo);
  
 // JMI if (fCurrentOffset) addDelayed(cmd, fCurrentOffset);
}

void xml2MsrScoreVisitor::visitStart ( S_beat_unit& elt )
{ 
  if (fCurrentBeat.fBeatUnit.size()) {
    fBeatsData.push_back (fCurrentBeat); 
    fCurrentBeat.fBeatUnit = "";
    fCurrentBeat.fDots = 0;
  }
  fCurrentBeat.fBeatUnit = elt->getValue();
}

void xml2MsrScoreVisitor::visitStart ( S_beat_unit_dot& elt )
  { fCurrentBeat.fDots++; }
  
void xml2MsrScoreVisitor::visitStart ( S_per_minute& elt )
  { fPerMinute = (int)(*elt); }


//________________________________________________________________________
void xml2MsrScoreVisitor::visitStart (S_tied& elt )
{
//           <tied orientation="over" type="start"/>

fCurrentTiedType =
  elt->getAttributeValue ("type");

fCurrentTiedOrientation =
  elt->getAttributeValue ("orientation");
}

void xml2MsrScoreVisitor::visitStart (S_slur& elt )
{
//          <slur number="1" placement="above" type="start"/>
  fCurrentSlurNumber =
    elt->getAttributeIntValue ("number", 0);

  fCurrentSlurType =
    elt->getAttributeValue ("type");

  fCurrentSlurPlacement =
    elt->getAttributeValue ("placement");

  if (fCurrentSlurType == "start") {
    
    fCurrentSlurKind = msrSlur::kStartSlur;
    fOnGoingSlur = true;
    
  } else if (fCurrentSlurType == "continue") {
    
    fCurrentSlurKind = msrSlur::kContinueSlur;
    
  } else if (fCurrentSlurType == "stop") {
    
    fCurrentSlurKind = msrSlur::kStopSlur;
    fOnGoingSlur = false;
    
  } else {

    // inner slur notes may miss the "continue" type:
    // let' complain on slur notes outside of slurs 
    if (! fOnGoingSlur)
      if (fCurrentSlurType.size()) {
        stringstream s;
        string       message;
        s << "slur type" << fCurrentSlurType << "unknown";
        s >> message;
        msrMusicXMLError(message);
      }
      
    }
}

//________________________________________________________________________
void xml2MsrScoreVisitor::visitStart (S_lyric& elt )
{ 
  int lyricNumber =
    elt->getAttributeIntValue ("number", 0);

  // is this lyrics already present?
  fCurrentLyrics =
    fCurrentVoice->
      voiceContainsLyrics (
        lyricNumber);

  // no, add it to the current staff
  if (! fCurrentLyrics) 
  fCurrentLyrics =
    fCurrentVoice->
      addLyricsToVoice (
        lyricNumber);

  fCurrentLyricsHasText = false;
  fCurrentText = "";
  fCurrentElision = false;

  fCurrentNoteHasLyrics = true;
}

void xml2MsrScoreVisitor::visitStart ( S_syllabic& elt )
{
  fCurrentSyllabic = elt->getValue();
/* JMI
  if (fCurrentSyllabic == "begin") {
    fOnGoingLyrics = true;
  }
  else if (fCurrentSyllabic == "end") {
    fOnGoingLyrics = true;
  }
  */
}

void xml2MsrScoreVisitor::visitEnd ( S_text& elt ) 
{
  string text = elt->getValue();

  // text may be composed of only spaces, so:
  string dest;
  std::for_each (
    text.begin(), text.end(), stringSpaceRemover (dest));

  if (fCurrentElision)
    fCurrentText += " "+dest;
  else
    fCurrentText = dest;

  fCurrentLyricsHasText = true;
}

/*
  cout <<
    "--> lyricNumber = " << lyricNumber <<
    ", fCurrentSyllabic = " << fCurrentSyllabic <<
    ", fCurrentText = |" << fCurrentText << "|" << endl;
*/

/*
      <note default-x="143">
        <pitch>
          <step>E</step>
          <alter>-1</alter>
          <octave>4</octave>
        </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem default-y="-5">up</stem>
        <beam number="1">begin</beam>
        <lyric default-y="-80" justify="left" number="1">
          <syllabic>single</syllabic>
          <text font-family="FreeSerif" font-size="11">1.</text>
          <elision> </elision>
          <syllabic>begin</syllabic>
          <text font-family="FreeSerif" font-size="11">A</text>
        </lyric>
        <lyric default-y="-97" justify="left" number="2">
          <syllabic>single</syllabic>
          <text font-family="FreeSerif" font-size="11">2.</text>
          <elision> </elision>
          <syllabic>single</syllabic>
          <text font-family="FreeSerif" font-size="11">'T</text>
          <elision> </elision>
          <syllabic>single</syllabic>
          <text font-family="FreeSerif" font-size="11">was</text>
        </lyric>
        <lyric default-y="-113" justify="left" number="3">
          <syllabic>single</syllabic>
          <text font-family="FreeSerif" font-size="11">3.</text>
          <elision> </elision>
          <syllabic>single</syllabic>
          <text font-family="FreeSerif" font-size="11">Throug</text>
          <extend type="start"/>
        </lyric>
*/

void xml2MsrScoreVisitor::visitEnd ( S_elision& elt ) 
{
  fCurrentElision = true;
}

void xml2MsrScoreVisitor::visitEnd ( S_lyric& elt )
{
  if (fCurrentLyricsHasText) {
    S_msrDuration
      lyricMsrDuration =
        msrDuration::create (
          fMusicXMLNoteData.fMusicxmlDuration,
          fCurrentMusicXMLDivisions,
          fMusicXMLNoteData.fDotsNumber,
          fMusicXMLNoteData.fTupletMemberNoteType);
  
    fCurrentLyrics->
      addTextChunkToLyrics (
        fCurrentSyllabic,
        fCurrentText,
        fCurrentElision,
        lyricMsrDuration);
}

/*
      <note default-x="61">
        <pitch>
          <step>D</step>
          <octave>4</octave>
        </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem default-y="-10">up</stem>
        <lyric default-y="-80" number="1">
          <syllabic>single</syllabic>
          <text>オノ</text>
        </lyric>
      </note>
*/

/* JMI lyric without text
        <lyric name="verse" number="3">
          <extend type="stop"/>
        </lyric>
*/
}

//________________________________________________________________________
void xml2MsrScoreVisitor::visitStart (S_measure& elt)
{
  fCurrentMeasureNumber =
    elt->getAttributeIntValue ("number", 0);

  fCurrentPositionInMeasure = rational (0, 0);
    
  if (fTranslationSettings->fTrace)
    cerr << idtr << 
      "=== MEASURE " << fCurrentMeasureNumber << " === " <<
      "PART " << fCurrentPart->getPartCombinedName () <<" ===" << endl;

  if (
    fCurrentPart->getPartMSRName() == "P17"
      &&
    fCurrentMeasureNumber == 26) {
    fTranslationSettings->fTrace = true; // JMI pour tests
    fTranslationSettings->fDebug = true; // JMI pour tests
  }
}

//______________________________________________________________________________
void xml2MsrScoreVisitor::visitStart ( S_print& elt ) 
{
  const string& newSystem = elt->getAttributeValue ("new-system");
  
  if (newSystem == "yes") {
    /* JMI
    // create a barnumbercheck command
    S_msrBarNumberCheck barnumbercheck_ =
      msrBarNumberCheck::create (fCurrentMeasureNumber);
    S_msrElement b2 = barnumbercheck_;
    fCurrentVoice->appendElementToVoiceSequence (b2);

    // create a break command
    S_msrBreak break_ =
      msrBreak::create(fCurrentMeasureNumber);
    S_msrElement b1 = break_;
    fCurrentVoice->appendElementToVoiceSequence (b1);
  */
    // add a break chunk to the lyrics
    fCurrentLyrics->
      addBreakChunkToLyrics (fCurrentMeasureNumber);
  }
}

/*
  <barline location="left">
    <bar-style>heavy-light</bar-style>
    <repeat direction="forward"/>
  </barline>
  *
  *   <barline location="right">
    <bar-style>light-heavy</bar-style>
    <repeat direction="backward"/>
  </barline>
  * 
  */

//______________________________________________________________________________
void xml2MsrScoreVisitor::visitStart ( S_barline& elt ) 
{
  fCurrentBarlineLocation =
    elt->getAttributeValue ("location");
  
  if (fCurrentBarlineLocation == "left") {

    
  } else  if (fCurrentBarlineLocation == "middle") {
    
    S_msrBarLine
      barline =
        msrBarLine::create (fCurrentMeasureNumber+1);
    S_msrElement b = barline;
    fCurrentVoice->appendElementToVoiceSequence (b);
    
  } else if (fCurrentBarlineLocation == "right") {

    
  } else {

  }
}

void xml2MsrScoreVisitor::visitEnd ( S_barline& elt ) 
{
 /*
  *       <barline location="right">
        <bar-style>light-heavy</bar-style>
      </barline>
*/
  if (
      fCurrentBarlineLocation == "right"
        &&
      fCurrentBarStyle == "light-heavy") {
        /* JMI
    S_msrBarCommand
      barCommand =
        msrBarCommand::create ();
    S_msrElement b = barCommand;
    fCurrentVoice->appendElementToVoiceSequence (b);
    */
      
  }
}

void xml2MsrScoreVisitor::visitStart ( S_bar_style& elt ) 
{
  fCurrentBarStyle = elt->getValue();
}

void xml2MsrScoreVisitor::visitStart ( S_repeat& elt ) 
{
  fCurrentRepeatDirection =
    elt->getAttributeValue ("direction");  
}

/*
 *   <barline location="left">
    <ending type="start" number="1"/>
  </barline>

*/

void xml2MsrScoreVisitor::visitStart ( S_ending& elt ) 
{
  // start, stop, discontinue
  
  fCurrentEndingType =
    elt->getAttributeValue ("type");  

  fCurrentEndingNumber =
    elt->getAttributeIntValue ("number", 0);
}

//______________________________________________________________________________
void xml2MsrScoreVisitor::visitStart ( S_note& elt ) 
{
  //  cout << "--> xml2MsrScoreVisitor::visitStart ( S_note& elt ) " << endl;
  fMusicXMLNoteData.fMusicxmlStep = '_';
  fMusicXMLNoteData.fMusicxmlStepIsARest = false;
  fMusicXMLNoteData.fMusicxmlStepIsUnpitched = false;
  fMusicXMLNoteData.fMusicxmlAlteration = 0; // natural notes
  fMusicXMLNoteData.fMusicxmlOctave = -13;
  fMusicXMLNoteData.fDotsNumber = 0;
  fMusicXMLNoteData.fNoteIsAGraceNote = false;;

  // assuming staff number 1, unless S_staff states otherwise afterwards
  fCurrentStaffNumber = 1;

  fCurrentStem = "";

  // assume this note hasn't got lyrics until S_lyric is met
  fCurrentNoteHasLyrics = false;
  
  // assume this note doesn't belong to a chord until S_chord is met
  fMusicXMLNoteData.fNoteBelongsToAChord = false;

  // assume this note doesn't belong to a tuplet until S_chord is met
  fMusicXMLNoteData.fNoteBelongsToATuplet = fOnGoingTuplet;

  fCurrentTiedType = "";
  fCurrentTiedOrientation = "";

  fCurrentSlurNumber = -1;
  fCurrentSlurType = "";
  fCurrentSlurPlacement = "";
  fCurrentSlurKind = msrSlur::k_NoSlur;

  fOnGoingNote = true;
}

//______________________________________________________________________________
void xml2MsrScoreVisitor::visitStart ( S_step& elt )
{
  string step = elt->getValue();
  
  if (step.length() != 1) {
    stringstream s;
    string       message;
    s << "step value " << step << " should be a single letter from A to G";
    s >> message;
    msrMusicXMLError (message);
  }

  fMusicXMLNoteData.fMusicxmlStep = step[0];
}

void xml2MsrScoreVisitor::visitStart ( S_alter& elt)
{
  fMusicXMLNoteData.fMusicxmlAlteration = (int)(*elt);
}

void xml2MsrScoreVisitor::visitStart ( S_octave& elt)
{
  fMusicXMLNoteData.fMusicxmlOctave = (int)(*elt);
}

void xml2MsrScoreVisitor::visitStart ( S_duration& elt )
{
  int musicXMLduration = (int)(*elt);

  if (fOnGoingBackup)
  
    fCurrentBackupDuration = musicXMLduration;
    
  else if (fOnGoingForward)
  
    fCurrentForwardDuration = musicXMLduration;
    
  else if (fOnGoingNote)
  
    fMusicXMLNoteData.fMusicxmlDuration = musicXMLduration;
    
  else {
    
    stringstream s;
    string       message;
    s << "duration " << musicXMLduration << " is out of context";
    s >> message;
    msrMusicXMLError (message);
    
  }
    
//  cout << "=== xml2MsrScoreVisitor::visitStart ( S_duration& elt ), fCurrentMusicXMLDuration = " << fCurrentMusicXMLDuration << endl; JMI
}

void xml2MsrScoreVisitor::visitStart ( S_dot& elt )
{
  fMusicXMLNoteData.fDotsNumber++;
}
       
void xml2MsrScoreVisitor::visitStart ( S_type& elt )
{
  fCurrentNoteType=elt->getValue();
}

void xml2MsrScoreVisitor::visitStart ( S_stem& elt )
{
  //         <stem default-y="28.5">up</stem>

  string        stem = elt->getValue();
  StemDirection stemDirection;
  
  if (stem == "up")
    stemDirection = kStemUp;
  else if (stem == "down")
    stemDirection = kStemDown;
  else
    stemDirection = kStemNeutral; // JMI

  if (stemDirection != fCurrentStemDirection) {
    if (fTranslationSettings->fGenerateStems) {
      switch (stemDirection) {
        case kStemNeutral:
          // \stemNeutral JMI
          break;
        case kStemUp:
          // \stemUp JMI
          break;
        case kStemDown:
          // \stemDown JMI
          break;
      } // switch
    }
    fCurrentStemDirection = stemDirection;
  }
  
  fCurrentStem = stem;
}

void xml2MsrScoreVisitor::visitStart ( S_beam& elt )
{
  //        <beam number="1">begin</beam>

  fCurrentBeam = elt->getValue();

  fCurrentBeamNumber = 
    elt->getAttributeIntValue ("number", 0);
  
  msrBeam::BeamKind bk = msrBeam::k_NoBeam;

  if (fCurrentBeam == "begin") {
    bk = msrBeam::kBeginBeam;
  }
  else if (fCurrentBeam == "continue") {
    bk = msrBeam::kContinueBeam;
  }
  else if (fCurrentBeam == "end") {
    bk = msrBeam::kEndBeam;
  }
  
//  S_msrBeam beam = msrBeam::create(number, bk); // JMI
//  fCurrentBeam = beam;
}

//______________________________________________________________________________
void xml2MsrScoreVisitor::visitStart ( S_staccato& elt )
{
  S_msrArticulation
    articulation =
      msrArticulation::create (msrArticulation::kStaccato);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrScoreVisitor::visitStart ( S_staccatissimo& elt )
{
  S_msrArticulation
    articulation =
      msrArticulation::create (msrArticulation::kStaccatissimo);
      
  fCurrentArticulations.push_back (articulation);
}

  /*

Component   Type  Occurs  Default   Description 
    0..*    
accent  empty-placement   1..1    

The accent element indicates a regular horizontal accent mark.
breath-mark   breath-mark   1..1    

The breath-mark element indicates a place to take a breath.
caesura   empty-placement   1..1    

The caesura element indicates a slight pause. It is notated using a "railroad tracks" symbol.
detached-legato   empty-placement   1..1    

The detached-legato element indicates the combination of a tenuto line and staccato dot symbol.
doit  empty-line  1..1    

The doit element is an indeterminate slide attached to a single note. The doit element appears after the main note and goes above the main pitch.
falloff   empty-line  1..1    

The falloff element is an indeterminate slide attached to a single note. The falloff element appears before the main note and goes below the main pitch.
other-articulation  placement-text  1..1    The other-articulation element is used to define any articulations not yet in the MusicXML format. This allows extended representation, though without application interoperability.
plop  empty-line  1..1    

The plop element is an indeterminate slide attached to a single note. The plop element appears before the main note and comes from above the main pitch.
scoop   empty-line  1..1    

The scoop element is an indeterminate slide attached to a single note. The scoop element appears before the main note and comes from below the main pitch.
spiccato  empty-placement   1..1    

The spiccato element is used for a stroke articulation, as opposed to a dot or a wedge.
staccatissimo   empty-placement   1..1    

The staccatissimo element is used for a wedge articulation, as opposed to a dot or a stroke.
staccato  empty-placement   1..1    

The staccato element is used for a dot articulation, as opposed to a stroke or a wedge.
stress  empty-placement   1..1    

The stress element indicates a stressed note.
strong-accent   strong-accent   1..1    

The strong-accent element indicates a vertical accent mark.
tenuto  empty-placement   1..1    

The tenuto element indicates a tenuto line symbol.
unstress
  */

//______________________________________________________________________________
void xml2MsrScoreVisitor::visitStart( S_f& elt)
{        
  S_msrDynamics dyn = msrDynamics::create(msrDynamics::kF);
  fPendingDynamics.push_back(dyn);
 }
void xml2MsrScoreVisitor::visitStart( S_ff& elt)
{        
  S_msrDynamics dyn = msrDynamics::create(msrDynamics::kFF);
  fPendingDynamics.push_back(dyn);
 }
void xml2MsrScoreVisitor::visitStart( S_fff& elt)
{        
  S_msrDynamics dyn = msrDynamics::create(msrDynamics::kFFF);
  fPendingDynamics.push_back(dyn);
 }
void xml2MsrScoreVisitor::visitStart( S_ffff& elt)
{        
  S_msrDynamics dyn = msrDynamics::create(msrDynamics::kFFFF);
  fPendingDynamics.push_back(dyn);
 }
void xml2MsrScoreVisitor::visitStart( S_fffff& elt)
{        
  S_msrDynamics dyn = msrDynamics::create(msrDynamics::kFFFFF);
  fPendingDynamics.push_back(dyn);
 }
void xml2MsrScoreVisitor::visitStart( S_ffffff& elt)
{        
  S_msrDynamics dyn = msrDynamics::create(msrDynamics::kFFFFFF);
  fPendingDynamics.push_back(dyn);
 }


void xml2MsrScoreVisitor::visitStart( S_p& elt)
{        
  S_msrDynamics dyn = msrDynamics::create(msrDynamics::kP);
  fPendingDynamics.push_back(dyn);
}
void xml2MsrScoreVisitor::visitStart( S_pp& elt)
{        
  S_msrDynamics dyn = msrDynamics::create(msrDynamics::kPP);
  fPendingDynamics.push_back(dyn);
}
void xml2MsrScoreVisitor::visitStart( S_ppp& elt)
{        
  S_msrDynamics dyn = msrDynamics::create(msrDynamics::kPP);
  fPendingDynamics.push_back(dyn);
}
void xml2MsrScoreVisitor::visitStart( S_pppp& elt)
{        
  S_msrDynamics dyn = msrDynamics::create(msrDynamics::kPPPP);
  fPendingDynamics.push_back(dyn);
}
void xml2MsrScoreVisitor::visitStart( S_ppppp& elt)
{        
  S_msrDynamics dyn = msrDynamics::create(msrDynamics::kPPPPP);
  fPendingDynamics.push_back(dyn);
}
void xml2MsrScoreVisitor::visitStart( S_pppppp& elt)
{        
  S_msrDynamics dyn = msrDynamics::create(msrDynamics::kPPPPPP);
  fPendingDynamics.push_back(dyn);
}

//______________________________________________________________________________
void xml2MsrScoreVisitor::visitStart ( S_wedge& elt )
{
  string type = elt->getAttributeValue("type");
  msrWedge::WedgeKind wk;

  if (type == "crescendo") {
    wk = msrWedge::kCrescendoWedge;
  }
  else if (type == "diminuendo") {
    wk = msrWedge::kDecrescendoWedge;
  }
  else if (type == "stop") {
    wk = msrWedge::kStopWedge;
  }
  
  S_msrWedge wedg = msrWedge::create(wk);;
  fPendingWedges.push_back(wedg);
}
    
//______________________________________________________________________________
void xml2MsrScoreVisitor::visitStart ( S_grace& elt )
{
  fMusicXMLNoteData.fNoteIsAGraceNote = true;;
}
       
//______________________________________________________________________________
void xml2MsrScoreVisitor::visitStart ( S_chord& elt)
{
  fMusicXMLNoteData.fNoteBelongsToAChord = true;
}

//______________________________________________________________________________
void xml2MsrScoreVisitor::visitStart ( S_actual_notes& elt )
{
  fCurrentActualNotes = (int)(*elt);
}

void xml2MsrScoreVisitor::visitStart ( S_normal_notes& elt )
{
  fCurrentNormalNotes = (int)(*elt);
}

void xml2MsrScoreVisitor::visitStart ( S_normal_type& elt )
{
  fCurrentNormalNoteType = elt->getValue();
}

void xml2MsrScoreVisitor::visitStart ( S_tuplet& elt )
{
  fMusicXMLNoteData.fNoteBelongsToATuplet = true;

  fCurrentTupletNumber =
    elt->getAttributeIntValue ("number", 0);
  string tupletType =
    elt->getAttributeValue("type");
  
  /* JMI
  cout <<
    "xml2MsrScoreVisitor::visitStart ( S_tuplet, fCurrentTupletNumber = " <<
    fCurrentTupletNumber << ", type = " << type <<endl;
  */
  
  fCurrentTupletKind = msrTuplet::k_NoTuplet;
  
  if (tupletType == "start")
    fCurrentTupletKind = msrTuplet::kStartTuplet;
  else if (tupletType == "continue")
    fCurrentTupletKind = msrTuplet::kContinueTuplet;
  else if (tupletType == "stop")
    fCurrentTupletKind = msrTuplet::kStopTuplet;
  else {
    stringstream s;
    string  message;
    s << "tuplet type " << tupletType << " is unknown";
    s >> message;
    msrMusicXMLError (message);
  }
}

//______________________________________________________________________________
void xml2MsrScoreVisitor::visitStart ( S_rest& elt)
{
  /*
        <note>
        <rest/>
        <duration>24</duration>
        <voice>1</voice>
      </note>
*/
  //  cout << "--> xml2MsrScoreVisitor::visitStart ( S_rest& elt ) " << endl;
  fMusicXMLNoteData.fMusicxmlStepIsARest = true;
}

//______________________________________________________________________________
void xml2MsrScoreVisitor::visitStart ( S_display_step& elt)
{
  string displayStep = elt->getValue();
  
  if (displayStep.length() != 1) {
    stringstream s;
    string       message;
    s << "sdisplay step value " << displayStep << " should be a single letter from A to G";
    s >> message;
    msrMusicXMLError (message);
  }

  fDisplayStep = displayStep[0];
}

void xml2MsrScoreVisitor::visitStart ( S_display_octave& elt)
{
  fDisplayOctave = (int)(*elt);
}

void xml2MsrScoreVisitor::visitEnd ( S_unpitched& elt)
{
/*
        <unpitched>
          <display-step>E</display-step>
          <display-octave>5</display-octave>
        </unpitched>
*/
  fMusicXMLNoteData.fMusicxmlStepIsUnpitched = true;
  fMusicXMLNoteData.fMusicxmlStep = fDisplayStep;
  fMusicXMLNoteData.fMusicxmlOctave = fDisplayOctave;
}

//______________________________________________________________________________
S_msrChord xml2MsrScoreVisitor::createChordFromCurrentNote ()
{
  // cout << "--> creating a chord on its 2nd note" << endl;
  
  // fCurrentNote has been registered standalone in the part element sequence,
  // but it is actually the first note of a chord
  
  // create a chord
  S_msrChord chord;
  
  chord = msrChord::create (
    fCurrentNote->getNoteMsrDuration ());
// JMI  fCurrentElement = chord; // another name for it
   
  if (fTranslationSettings->fDebug)
    cerr << idtr <<
      "--> adding first note to chord" << endl;
    
  // register fCurrentNote as first member of chord
  chord->addNoteToChord (fCurrentNote);
  fCurrentNote->setNoteBelongsToAChord ();

  // move the pending articulations if any from the first note to the chord
  list<S_msrArticulation>
    noteArticulations = fCurrentNote->getNoteArticulations ();

  while (! noteArticulations.empty()) {
    //cout << "--> moving dynamics from fCurrentNote to chord" << endl;
    S_msrArticulation
      art = noteArticulations.front();
    chord->addArticulation (art);
    noteArticulations.pop_front ();
  } // while
  
  // move the pending dynamics if any from the first note to the chord
  list<S_msrDynamics>
    noteDynamics = fCurrentNote->getNoteDynamics();
    
  while (! noteDynamics.empty()) {
    //cout << "--> moving dynamics from fCurrentNote to chord" << endl;
    S_msrDynamics
      dyn = noteDynamics.front();
    chord->addDynamics (dyn);
    noteDynamics.pop_front ();
  } // while
 
  // move the pending wedges if any from the first note to the chord
  list<S_msrWedge>
    noteWedges = fCurrentNote->getNoteWedges();
    
  while (! noteWedges.empty()) {
    //cout << "--> moving wedge from fCurrentNote to chord" << endl;
    S_msrWedge
      wdg = noteWedges.front();
    chord->addWedge (wdg);
    noteWedges.pop_front();
  } // while

  return chord;
}

//______________________________________________________________________________
void xml2MsrScoreVisitor::createTuplet (S_msrNote note)
{
  // create a tuplet element
  S_msrTuplet tuplet = msrTuplet::create();
// JMI  fCurrentElement = tuplet; // another name for it

  // populate it
  tuplet->updateTuplet (
    fCurrentTupletNumber,
    fCurrentActualNotes,
    fCurrentNormalNotes);

  // register it in this visitor
  if (fTranslationSettings->fDebug)
    cout << "--> pushing tuplet to tuplets stack" << endl;
  fCurrentTupletsStack.push(tuplet);
  
  // add note to the tuplet
  if (fTranslationSettings->fDebug)
    cout << idtr <<
      "--> adding note " << note << " to tuplets stack top" << endl;
  tuplet->addElementToTuplet (note);
}

void xml2MsrScoreVisitor::finalizeTuplet (S_msrNote note) {
  // get tuplet from top of tuplet stack
  S_msrTuplet tup = fCurrentTupletsStack.top();

  // add note to the tuplet
  if (fTranslationSettings->fDebug)
    cout << idtr <<
      "--> adding note " << note << " to tuplets stack top" << endl;
  tup->addElementToTuplet(note);

  // pop from the tuplets stack
  if (fTranslationSettings->fDebug)
    cout << "--> popping from tuplets stack" << endl;
  fCurrentTupletsStack.pop();        

  // add tuplet to the part
  if (fTranslationSettings->fDebug)
    cout << "=== adding tuplet to the part sequence" << endl;
  S_msrElement elem = tup;
  fCurrentVoice->appendElementToVoiceSequence (elem);
}          

//______________________________________________________________________________
void xml2MsrScoreVisitor::visitEnd ( S_note& elt ) 
{
  /*
  This is a complex method, due to the fact that
  dynamics, wedges, chords and tuplets
  are not ordered in the same way in MusicXML and LilyPond.

  Staff number is analyzed before voice number but occurs
  after it in the MusicXML tree.
  That's why the treatment below has been postponed until this method
  */


  /*
  Staff assignment is only needed for music notated on multiple staves.
  Used by both notes and directions.
  Staff values are numbers, with 1 referring to the top-most staff in a part.
  */
  
  // is fCurrentStaffNumber already present in fCurrentPart?
  fCurrentStaff =
    fCurrentPart->
      fetchStaffFromPart (fCurrentStaffNumber);

  // no, add it to the current part
  if (! fCurrentStaff) 
    fCurrentStaff =
      fCurrentPart->
        addStaffToPart (fCurrentStaffNumber);

  // is fCurrentStaffNumber already present in fCurrentPart?
  /* JMI
  if (
    fCurrentVoiceNumber
      >
    fCurrentPart->getPartStavesMap().size()) {

    // yes, switch to next staff
    fCurrentStaffNumber++;

    fCurrentStaff =
      fCurrentPart->
        fetchStaffFromPart (fCurrentStaffNumber);
    
    if (true || fTranslationSettings->fDebug)
      cerr <<
        idtr <<
        "--> overflowing voice " << fCurrentVoiceNumber <<
       " to staff " << fCurrentStaffNumber << endl <<
       " in part " << fCurrentPart->getPartCombinedName() << endl;
       
  }
  * */
    
  // is fCurrentVoiceNumber voice already present in fCurrentStaff?
  fCurrentVoice =
    fCurrentStaff->
      fetchVoiceFromStaff (fCurrentVoiceNumber);

  // no, add it to the current staff
  if (! fCurrentVoice) 
    fCurrentVoice =
      fCurrentStaff->
        addVoiceToStaff (fCurrentVoiceNumber);

  // store voice number in MusicXML note data
  fMusicXMLNoteData.fVoiceNumber = fCurrentVoiceNumber;

  fCurrentStemDirection = kStemNeutral;
  
  if (fTranslationSettings->fDebug)
    cerr <<
      "fMusicXMLNoteData.fMusicxmlDuration = " << 
      fMusicXMLNoteData.fMusicxmlDuration << ", " << 
      "fCurrentMusicXMLDivisions*4 = " <<
      fCurrentMusicXMLDivisions*4 << endl;
      
  if (fCurrentMusicXMLDivisions <= 0)
    msrMusicXMLError ("divisions cannot be 0 nor negative");
  
  fMusicXMLNoteData.fMusicxmlDivisions =
    fCurrentMusicXMLDivisions;
  fMusicXMLNoteData.fTupletMemberNoteType =
    fCurrentNoteType;
  
  //cout << "::: creating a note" << endl;
  S_msrNote note =
    msrNote::createFromMusicXMLData (
      fTranslationSettings,
      fMusicXMLNoteData,
      fCurrentSlurKind);

  // attach the articulations if any to the note
  while (! fCurrentArticulations.empty()) {
    S_msrArticulation art = fCurrentArticulations.front();
    note->addArticulation (art);
    fCurrentArticulations.pop_front();
  } // while
   
  // attach the pending dynamics if any to the note
  if (! fPendingDynamics.empty()) {
    if (fMusicXMLNoteData.fMusicxmlStepIsARest) {
      if (fTranslationSettings->fDelayRestsDynamics) {
      cerr << idtr <<
        "--> Delaying dynamics attached to a rest until next note" << endl;
      } else {
        cerr << idtr <<
          "--> There is dynamics attached to a rest" << endl;
      }
    } else {
      while (! fPendingDynamics.empty()) {
        S_msrDynamics dyn = fPendingDynamics.front();
        note->addDynamics(dyn);
        fPendingDynamics.pop_front();
      } // while
    }
  }
  
  // attach the pending wedges if any to the note
  if (! fPendingWedges.empty()) {
    if (fMusicXMLNoteData.fMusicxmlStepIsARest) {
      if (fTranslationSettings->fDelayRestsDynamics) {
      cerr << idtr <<
        "--> Delaying wedge attached to a rest until next note" << endl;
      } else {
        cerr << idtr <<
          "--> There is a wedge attached to a rest" << endl;
      }
    } else {
      while (! fPendingWedges.empty()) {
        S_msrWedge wdg = fPendingWedges.front();
        note->addWedge(wdg);
        fPendingWedges.pop_front();
      } // while
    }
  }

  /*
  A note can be standalone
  or a member of a chord,
  and the latter can belong to a tuplet.
  
  A rest can be standalone or belong to a tuplet
  */
  
  if (fMusicXMLNoteData.fNoteBelongsToAChord) {
    
    if (fMusicXMLNoteData.fMusicxmlStepIsARest)
      msrMusicXMLError (
        "a rest cannot belong to a chord");
        
    if (! fOnGoingChord) {
      // create a chord with fCurrentNote as its first note
      fCurrentChord =
        createChordFromCurrentNote ();

      // account for chord being built
      fOnGoingChord = true;
    }
    
    if (fTranslationSettings->fDebug)
      cout << idtr <<
        "--> adding note to current chord" << endl;
      
    // register note as a member of fCurrentChord
    fCurrentChord->addNoteToChord (note);
      
    // remove (previous) fCurrentNote from the current voice sequence
    fCurrentVoice->removeElementFromVoiceSequence (fCurrentNote);

    // add fCurrentChord to the part sequence instead
    S_msrElement elem = fCurrentChord;
    fCurrentVoice->appendElementToVoiceSequence (elem);

  } else if (fMusicXMLNoteData.fNoteBelongsToATuplet) {

    fMusicXMLNoteData.fTupletMemberNoteType = fCurrentNoteType;
    
    switch (fCurrentTupletKind) {
      case msrTuplet::kStartTuplet:
        {
          createTuplet(note);
          fOnGoingTuplet = true;
        
          // swith to continuation mode
          // this is handy in case the forthcoming tuplet members
          // are not explictly of the "continue" type
          fCurrentTupletKind = msrTuplet::kContinueTuplet;
        }
        break;
  
      case msrTuplet::kContinueTuplet:
        {
          // populate the tuplet at the top of the stack
          if (fTranslationSettings->fDebug)
            cout << idtr <<
              "--> adding note " << note <<
              " to tuplets stack top" << endl;
          fCurrentTupletsStack.top()->addElementToTuplet (note);
        }
        break;
  
      case msrTuplet::kStopTuplet:
        {
          finalizeTuplet(note);

          // indicate the end of the tuplet
          fOnGoingTuplet = false;
        }
        break;
      default:
        {}
    } // switch

  } else { // standalone note/rest

    // cout <<  idtr << "--> adding standalone note/rest to part sequence" << endl;
    // register note as standalone
    S_msrElement n = note;
    fCurrentVoice->appendElementToVoiceSequence (n);
  
    // account for chord not being built
    fOnGoingChord = false;
  }
  
  // keep track of note/rest in this visitor
  fCurrentNote    = note;
  fCurrentPositionInMeasure +=
    fCurrentNote->
      getNoteMsrDuration ()->durationAsRational ();
    
// JMI  fCurrentElement = fCurrentNote; // another name for it

  // add a skip chunk for notes/rests without lyrics
  if (! fCurrentNoteHasLyrics) {
    S_msrDuration
      lyricsMsrDuration =
        note->getNoteMsrDuration ();

    fCurrentLyrics->
      addSkipChunkToLyrics (
        lyricsMsrDuration);
  }

//  if (true || fTranslationSettings->fDebug)
  if (fTranslationSettings->fDebug)
    cerr <<
      idtr <<
      "!!!! At note " << fCurrentNote << "we have:" << endl <<
      idtr << idtr <<
      "--> fCurrentVoiceNumber = " << fCurrentVoiceNumber << endl <<
      idtr << idtr <<
      "--> fCurrentVoice        = " << fCurrentVoice->getVoiceName() << endl <<
      idtr << idtr <<
      "--> fCurrentStaffNumber = " << fCurrentStaffNumber << endl <<
      idtr << idtr <<
      "--> current staff name  = " << fCurrentStaff->getStaffName() << endl;

  fOnGoingNote = false;
}


} // namespace
