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
  
  // create the MSR score
  fMsrScore =
    msrScore::create (fTranslationSettings);
  
  fCurrentMeasureNumber = 0;

  fOnGoingChord = false;
  
  fOnGoingTuplet = false;

  fCurrentBackupDuration = -1;

  fOnGoingNote = false;

  fOnGoingChord = false;
  
  fOnGoingSlur = false;

  fOnGoingBackup  = false;
  fOnGoingForward = false;
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
  if (fTranslationSettings->fTrace)
    cerr << idtr <<
      "Analysing part list" << endl;

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
  There is no hierarchy implied in part-group elements.
  All that matters is the sequence of part-group elements relative to score-part elements.
  The sequencing of two consecutive part-group elements does not matter.
  It is the default-x attribute that indicates the left-to-right ordering of the group symbols.

  <part-group number="1" type="start">
  <group-name>Trombones</group-name>
  <group-abbreviation>Trb.</group-abbreviation>
  <group-symbol default-x="-12">brace</group-symbol>
  <group-barline>yes</group-barline>
  </part-group>
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
  fCurrentPartGroupNumber =
    elt->getAttributeIntValue ("number", 0);
    
  fCurrentPartGroupType =
    elt->getAttributeValue ("type");

  fCurrentPartGroupBarline = "yes";
}

void xml2MsrScoreVisitor::visitStart (S_group_name& elt)
{
  fCurrentPartGroupName = elt->getValue();
}

void xml2MsrScoreVisitor::visitStart (S_group_abbreviation& elt)
{
  fCurrentPartGroupAbbreviation = elt->getValue ();
}

void xml2MsrScoreVisitor::visitStart (S_group_symbol& elt)
{
  fCurrentPartGroupSymbol = elt->getValue ();

  fCurrentPartGroupSymbolDefaultX =
    elt->getAttributeIntValue ("default-x", 0);
}

void xml2MsrScoreVisitor::visitStart ( S_group_barline& elt)
{
  fCurrentPartGroupBarline = elt->getValue ();
}

void xml2MsrScoreVisitor::visitEnd (S_part_group& elt)
{
  if (fTranslationSettings->fTrace)
    cerr << idtr <<
      "Handling part group " << fCurrentPartGroupNumber <<
      ", type: \"" << fCurrentPartGroupType << "\""  << endl;

  msrPartGroup::PartGroupTypeKind partGroupType;
  
  if (fCurrentPartGroupType == "start")
    partGroupType = msrPartGroup::kStartPartGroupType;
    
  else if (fCurrentPartGroupType == "stop")
    partGroupType = msrPartGroup::kStopPartGroupType;
    
  else {
    msrMusicXMLError (
      "unknown part group type \"" + fCurrentPartGroupType + "\"");
    partGroupType = msrPartGroup::k_NoPartGroupType;
  }

  msrPartGroup::PartGroupSymbolKind partGroupSymbol;
  
  if (fCurrentPartGroupSymbol == "brace")
    partGroupSymbol = msrPartGroup::kBracePartGroupSymbol;
    
  else if (fCurrentPartGroupSymbol == "bracket")
    partGroupSymbol = msrPartGroup::kBracketPartGroupSymbol;
    
  else {
    msrMusicXMLError (
      "unknown part group type \"" + fCurrentPartGroupSymbol + "\"");
    partGroupSymbol = msrPartGroup::k_NoPartGroupSymbol;
  }

  bool partGroupBarline;
  
  if (fCurrentPartGroupBarline == "yes")
    partGroupBarline = true;
    
  else if (fCurrentPartGroupBarline == "no")
    partGroupBarline = false;
    
  else {
    msrMusicXMLError (
      "unknown part group barline \"" + fCurrentPartGroupBarline + "\"");
    partGroupBarline = false;
  }

  switch (partGroupType) {
    
    case msrPartGroup::kStartPartGroupType:
      {
      // is this part group number already present?
      fCurrentPartGroup =
        fetchScorePartGroup (fCurrentPartGroupNumber);
    
      // no, create it
      if (! fCurrentPartGroup) 
        fCurrentPartGroup =
          msrPartGroup::create (
            fTranslationSettings,
            fCurrentPartGroupNumber,
            partGroupType,
            fCurrentPartGroupName,
            fCurrentPartGroupAbbreviation,
            partGroupSymbol,
            fCurrentPartGroupSymbolDefaultX,
            partGroupBarline);
    
      // add it to the score
      if (fTranslationSettings->fTrace)
        cerr << idtr <<
          "Adding part group " << fCurrentPartGroupNumber <<
          " to the score" << endl;

      fMsrScore->
        addPartGroupToScore (fCurrentPartGroup);
    
      // add it to the map of this visitor
      if (fTranslationSettings->fTrace)
        cerr << idtr <<
          "Adding part group " << fCurrentPartGroupNumber <<
          " to visitor's part group map" << endl;
      fPartGroupsMap [fCurrentPartGroupNumber] = fCurrentPartGroup;
        
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
      }
      break;
      
    case msrPartGroup::kStopPartGroupType:
      {
      // remove part group from the map,
      // it remains is the score's part group list
      if (fTranslationSettings->fTrace)
        cerr << idtr <<
          "Removing part group " << fCurrentPartGroupNumber <<
          " from visitor's part group map" << endl;
      fPartGroupsMap.erase (fCurrentPartGroupNumber);
      
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
      }
      break;
      
    case msrPartGroup::k_NoPartGroupType:
      {}
      break;
  } // switch
}

//________________________________________________________________________
void xml2MsrScoreVisitor::visitStart (S_score_part& elt)
{
  fCurrentPartMusicXMLName = elt->getAttributeValue ("id");

  if (fTranslationSettings->fTrace)
    cerr << idtr <<
      "Found part name \"" << fCurrentPartMusicXMLName << "\"" << endl;
}

void xml2MsrScoreVisitor::visitStart (S_part_name& elt)
{
  string partName = elt->getValue ();
}

void xml2MsrScoreVisitor::visitStart (S_part_abbreviation& elt)
{
  fCurrentPartGroupAbbreviation = elt->getValue ();
}

void xml2MsrScoreVisitor::visitStart (S_instrument_name& elt)
{
  fCurrentPartInstrumentName = elt->getValue(); // jMI
}

void xml2MsrScoreVisitor::createImplicitMSRPartGroup ()
{
  /*
  A first part group is created with all the nneded contents.
  It single part will be reused when the first actual part is met,
  changing its name on the fly
  in msrPartGroup::tryAndReUseInitialAnonymousPart()
  */

  // create an implicit part group
  fCurrentPartGroupNumber = 1;
  
  if (fTranslationSettings->fTrace)
    cerr << idtr <<
      "Creating an implicit part group with number " <<
      fCurrentPartGroupNumber << endl;

  fCurrentPartGroup =
    msrPartGroup::create (
      fTranslationSettings,
      fCurrentPartGroupNumber,
      msrPartGroup::kStartPartGroupType,
      "Anonymous",
      "Anon.",
      msrPartGroup::kBracketPartGroupSymbol,
      -3,
      true);

  // add implicit part group to the score
  if (fTranslationSettings->fTrace)
    cerr << idtr <<
      "Adding the implicit part group to the score" << endl;
    
  fMsrScore->
    addPartGroupToScore (fCurrentPartGroup);

  // add implicit part group to the map of this visitor
  if (fTranslationSettings->fTrace)
    cerr << idtr <<
      "Adding implicit part group " << fCurrentPartGroupNumber <<
      " to visitor's part group map" << endl;
  fPartGroupsMap [fCurrentPartGroupNumber] = fCurrentPartGroup;

  // create an implicit part in case none is specified in MusicXML
  fCurrentPartMusicXMLName = "";
  fCurrentPart =
    msrPart::create (
      fTranslationSettings, fCurrentPartMusicXMLName);
  
  // add a staff to the implicit part
  fCurrentStaffNumber = 1;
  fCurrentStaff =
    fCurrentPart->
      addStaffToPart (fCurrentStaffNumber);

  // fetch current voice
  fCurrentVoiceNumber = 1;
  fCurrentVoice =
    fCurrentStaff->
      fetchVoiceFromStaff (fCurrentVoiceNumber);

/*
  // add a voice to the staff
  fCurrentVoiceNumber = 1;
  fCurrentVoice =
    fCurrentStaff->
      addVoiceToStaff (fCurrentVoiceNumber);

  // add a lyrics to the voice
  fCurrentLyrics =
    fCurrentVoice->
      addLyricsToVoice (1);
*/
} // xml2MsrScoreVisitor::createImplicitMSRPartGroup ()

void xml2MsrScoreVisitor::visitEnd (S_score_part& elt)
{
  if (! fCurrentPartGroup) {
    createImplicitMSRPartGroup ();
  }

  // is this part already present in the current part group?
  fCurrentPart =
    fCurrentPartGroup->
      fetchPartFromPartGroup (fCurrentPartMusicXMLName);

  if (! fCurrentPart) {
    // no, add it to the current part group
    fCurrentPart =
      fCurrentPartGroup->
        addPartToPartGroup (fCurrentPartMusicXMLName);
  }

  // populate current part
  fCurrentPart->
    setPartName (fCurrentPartMusicXMLName);
  fCurrentPart->
    setPartAbbreviation (fCurrentPartAbbreviation);
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

  if (! fCurrentPart) 
    // no, add it to the current part group
    fCurrentPart =
      fCurrentPartGroup->
        addPartToPartGroup (partID);

  if (fTranslationSettings->fTrace)
    cerr << idtr <<
      "Analyzing part " << fCurrentPart->getPartCombinedName() << endl;

  idtr++;

  fCurrentStaffNumber = 1; // default if there are no <staff> element

  // is this staff already present?
  fCurrentStaff =
    fCurrentPart->
      fetchStaffFromPart (fCurrentStaffNumber);

  if (! fCurrentStaff) 
    // no, add it to the current part
    fCurrentStaff =
      fCurrentPart->
        addStaffToPart (fCurrentStaffNumber);

  fCurrentMeasureNumber = 0; // in case of an anacrusis
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
        fCurrentClefSign, fCurrentClefLine, fCurrentClefOctaveChange);

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
    s << "staff " << staffNumber << " is out of context";
    msrMusicXMLError (s.str());
    
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
    fCurrentVoice =
      fCurrentStaff->
        fetchVoiceFromStaff (fCurrentVoiceNumber);
    
  } else {
    
    stringstream s;
    s << "voice " << voiceNumber << " is out of context";
    msrMusicXMLError (s.str());
    
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
      "Handling 'backup <<< " << fCurrentBackupDuration <<
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
  // change staff
  fCurrentStaffNumber = fCurrentForwardStaffNumber;

  // is the new staff already present?
  fCurrentStaff =
    fCurrentPart->
      fetchStaffFromPart (fCurrentStaffNumber);

  if (! fCurrentStaff) 
    // no, add it to the current part
    fCurrentStaff =
      fCurrentPart->
        addStaffToPart (fCurrentStaffNumber);

  // fetch the new current voice
  fCurrentVoiceNumber = fCurrentForwardVoiceNumber;
  fCurrentVoice =
    fCurrentStaff->
      fetchVoiceFromStaff (fCurrentVoiceNumber);

  if (fTranslationSettings->fTrace)
    cerr << idtr <<
      "Handling 'forward >>> " << fCurrentForwardDuration <<
      "', thus switching to " <<
      "voice " << fCurrentVoice->getVoiceName () <<
      " in staff " << fCurrentStaff->getStaffName () << endl;

/* JMI
  // no, add it to the current staff
  if (! fCurrentVoice) 
    fCurrentVoice =
      fCurrentStaff->
        addVoiceToStaff (fCurrentVoiceNumber);
*/

  fOnGoingForward = false;
}

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
      s << "parentheses value " << parentheses << " should be 'yes' or 'no'";
      msrMusicXMLError (s.str());
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

 if (fCurrentTiedType == "start") { // JMI
    
//    fCurrentTiedKind = msrTied::kStartTied;
    
  } else if (fCurrentTiedType == "continue") {
    
//    fCurrentTiedKind = msrTied::kContinueTied;
    fMusicXMLNoteData.fMusicXMLNoteIsTied = true;
    
  } else if (fCurrentTiedType == "stop") {
    
//    fCurrentTiedKind = msrTied::kStopTied;
    fMusicXMLNoteData.fMusicXMLNoteIsTied = true;
    
  } else {

    // inner tied notes may miss the "continue" type:
    // let' complain on slur notes outside of slurs 
//    if (! fOnGoingSlur) JMI
      if (fCurrentTiedType.size()) {
        stringstream s;
        s << "tied type" << fCurrentSlurType << "unknown";
        msrMusicXMLError (s.str());
      }
      
    }
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
        s << "slur type" << fCurrentSlurType << "unknown";
        msrMusicXMLError (s.str());
      }
      
    }
}

//________________________________________________________________________
void xml2MsrScoreVisitor::visitStart (S_lyric& elt )
{ 
  fCurrentLyricsNumber =
    elt->getAttributeIntValue ("number", 0);

  // is voice fCurrentVoiceNumber present in current staff?
  fCurrentVoice =
    fCurrentStaff->
      fetchVoiceFromStaff (fCurrentVoiceNumber);

  if (! fCurrentVoice)
    // no, add it to the staff
    fCurrentVoice =
      fCurrentStaff->
        addVoiceToStaff (fCurrentVoiceNumber);
        
  // is lyrics fCurrentLyricsNumber present in current voice?
  fCurrentLyrics =
    fCurrentVoice->
      voiceContainsLyrics (fCurrentLyricsNumber);

  if (! fCurrentLyrics)
    // no, add it to the voice
    fCurrentLyrics =
      fCurrentVoice->
        addLyricsToVoice (fCurrentLyricsNumber);
        
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

//________________________________________________________________________
void xml2MsrScoreVisitor::visitStart (S_measure& elt)
{
  fCurrentMeasureNumber =
    elt->getAttributeIntValue ("number", 0);

  fCurrentPositionInMeasure = rational (0, 0);
    
  if (fTranslationSettings->fDebug)
    cerr << idtr << 
      "=== MEASURE " << fCurrentMeasureNumber << " === " <<
      "PART " << fCurrentPart->getPartCombinedName () <<" ===" << endl;

  if (
    fCurrentPart->getPartMSRName() == "P17"
      &&
    fCurrentMeasureNumber == 26) {
//    fTranslationSettings->fTrace = true; // JMI pour tests
//    fTranslationSettings->fDebug = true; // JMI pour tests
  }
}

//______________________________________________________________________________
void xml2MsrScoreVisitor::visitStart ( S_print& elt ) 
{
  const string& newSystem = elt->getAttributeValue ("new-system");
  
  if (newSystem == "yes") {

    // create a barnumbercheck command
    S_msrBarNumberCheck
      barnumbercheck_ =
        msrBarNumberCheck::create (fCurrentMeasureNumber);
    S_msrElement bnc = barnumbercheck_;
    fCurrentVoice->
      appendElementToVoice (bnc);

    // create a break command
    S_msrBreak
      break_ =
        msrBreak::create(fCurrentMeasureNumber);
    S_msrElement brk = break_;
    fCurrentVoice->
      appendElementToVoice (brk);
  
    // add a break chunk to the voice master lyrics
    fCurrentVoice->
      getMasterLyrics ()->
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
    S_msrElement bar = barline;
    fCurrentVoice->
      appendElementToVoice (bar);
    
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
  fMusicXMLNoteData.fMusicXMLStep = '_';
  fMusicXMLNoteData.fMusicXMLStepIsARest = false;
  fMusicXMLNoteData.fMusicXMLStepIsUnpitched = false;
  fMusicXMLNoteData.fMusicXMLAlteration = 0; // natural notes
  fMusicXMLNoteData.fMusicXMLOctave = -13;
  fMusicXMLNoteData.fMusicXMLDotsNumber = 0;
  fMusicXMLNoteData.fMusicXMLNoteIsAGraceNote = false;
  fMusicXMLNoteData.fMusicXMLNoteIsTied = false;

  // assuming staff number 1, unless S_staff states otherwise afterwards
  fCurrentStaffNumber = 1;

  // assuming voice number 1, unless S_voice states otherwise afterwards
  fCurrentVoiceNumber = 1;
  
  fCurrentStem = "";

  // assume this note hasn't got lyrics until S_lyric is met
  fCurrentNoteHasLyrics = false;
  
  // assume this note doesn't belong to a chord until S_chord is met
  fMusicXMLNoteData.fMusicXMLNoteBelongsToAChord = false;

  // assume this note doesn't belong to a tuplet until S_chord is met
  fMusicXMLNoteData.fMusicXMLNoteBelongsToATuplet = fOnGoingTuplet;

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
    s << "step value " << step << " should be a single letter from A to G";
    msrMusicXMLError (s.str());
  }

  fMusicXMLNoteData.fMusicXMLStep = step[0];
}

void xml2MsrScoreVisitor::visitStart ( S_alter& elt)
{
  fMusicXMLNoteData.fMusicXMLAlteration = (int)(*elt);
}

void xml2MsrScoreVisitor::visitStart ( S_octave& elt)
{
  fMusicXMLNoteData.fMusicXMLOctave = (int)(*elt);
}

void xml2MsrScoreVisitor::visitStart ( S_duration& elt )
{
  int musicXMLduration = (int)(*elt);

  if (fOnGoingBackup)
  
    fCurrentBackupDuration = musicXMLduration;
    
  else if (fOnGoingForward)
  
    fCurrentForwardDuration = musicXMLduration;
    
  else if (fOnGoingNote)
  
    fMusicXMLNoteData.fMusicXMLDuration = musicXMLduration;
    
  else {
    
    stringstream s;
    s << "duration " << musicXMLduration << " is out of context";
    msrMusicXMLError (s.str());
    
  }
    
//  cout << "=== xml2MsrScoreVisitor::visitStart ( S_duration& elt ), fCurrentMusicXMLDuration = " << fCurrentMusicXMLDuration << endl; JMI
}

void xml2MsrScoreVisitor::visitStart ( S_dot& elt )
{
  fMusicXMLNoteData.fMusicXMLDotsNumber++;
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

void xml2MsrScoreVisitor::visitStart ( S_fermata& elt )
{
  S_msrArticulation
    articulation =
      msrArticulation::create (msrArticulation::kFermata);
      
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
  fMusicXMLNoteData.fMusicXMLNoteIsAGraceNote = true;;
}
       
//______________________________________________________________________________
void xml2MsrScoreVisitor::visitStart ( S_chord& elt)
{
  fMusicXMLNoteData.fMusicXMLNoteBelongsToAChord = true;
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
  fMusicXMLNoteData.fMusicXMLNoteBelongsToATuplet = true;

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
    s << "tuplet type " << tupletType << " is unknown";
    msrMusicXMLError (s.str());
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
  fMusicXMLNoteData.fMusicXMLStepIsARest = true;
}

//______________________________________________________________________________
void xml2MsrScoreVisitor::visitStart ( S_display_step& elt)
{
  string displayStep = elt->getValue();
  
  if (displayStep.length() != 1) {
    stringstream s;
    s << "sdisplay step value " << displayStep << " should be a single letter from A to G";
    msrMusicXMLError (s.str());
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
  fMusicXMLNoteData.fMusicXMLStepIsUnpitched = true;
  fMusicXMLNoteData.fMusicXMLStep = fDisplayStep;
  fMusicXMLNoteData.fMusicXMLOctave = fDisplayOctave;
}

//______________________________________________________________________________
S_msrChord xml2MsrScoreVisitor::createChordFromCurrentNote ()
{
  if (fTranslationSettings->fDebug)
    cerr << idtr <<
      "--> creating a chord on its 2nd note" << endl;
  
  // fCurrentNote has been registered standalone in the part element sequence,
  // but it is actually the first note of a chord
  
  // create a chord
  S_msrChord chord;
  
  chord = msrChord::create (
    fCurrentNote->getNoteMsrDuration ());
// JMI  fCurrentElement = chord; // another name for it
   
  if (fTranslationSettings->fDebug)
    cerr << idtr <<
      "--> adding first note " << fCurrentNote->notePitchAsLilypondString() <<
      " to new chord" << endl;
    
  // register fCurrentNote as first member of chord
  chord->addNoteToChord (fCurrentNote);
  fCurrentNote->setNoteBelongsToAChord ();

  // move the pending articulations if any from the first note to the chord
  list<S_msrArticulation>
    noteArticulations =
      fCurrentNote->getNoteArticulations ();

  if (! noteArticulations.empty()) {
    if (fTranslationSettings->fDebug)
      cerr << idtr <<
        "--> moving articulations from current note to chord" << endl;
        
    while (! noteArticulations.empty()) {
      S_msrArticulation
        art = noteArticulations.front();
      chord->addArticulation (art);
      noteArticulations.pop_front ();
    } // while
  }
  
  // move the pending dynamics if any from the first note to the chord
  list<S_msrDynamics>
    noteDynamics =
      fCurrentNote->getNoteDynamics();
    
  if (! noteDynamics.empty()) {
    if (fTranslationSettings->fDebug)
      cerr << idtr <<
        "--> moving dynamics from current note to chord" << endl;
        
    while (! noteDynamics.empty()) {
      S_msrDynamics
        dyn = noteDynamics.front();
      chord->addDynamics (dyn);
      noteDynamics.pop_front ();
    } // while
  }
 
  // move the pending wedges if any from the first note to the chord
  list<S_msrWedge>
    noteWedges =
      fCurrentNote->getNoteWedges();
    
  if (! noteWedges.empty()) {
    if (fTranslationSettings->fDebug)
      cerr << idtr <<
        "--> moving wedges from current note to chord" << endl;
        
    while (! noteWedges.empty()) {
      S_msrWedge
        wdg = noteWedges.front();
      chord->addWedge (wdg);
      noteWedges.pop_front();
    } // while
  }
  
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
      "--> adding note " << note->notePitchAsLilypondString() <<
      " to tuplets stack top" << endl;
  tuplet->addElementToTuplet (note);
}

void xml2MsrScoreVisitor::finalizeTuplet (S_msrNote note) {
  // get tuplet from top of tuplet stack
  S_msrTuplet tup = fCurrentTupletsStack.top();

  // add note to the tuplet
  if (fTranslationSettings->fDebug)
    cout << idtr <<
      "--> adding note " << note->notePitchAsLilypondString () <<
      " to tuplets stack top" << endl;
  tup->addElementToTuplet(note);

  // pop from the tuplets stack
  if (fTranslationSettings->fDebug)
    cout << "--> popping from tuplets stack" << endl;
  fCurrentTupletsStack.pop();        

  // add tuplet to current voice
  if (fTranslationSettings->fDebug)
    cout << "=== adding tuplet to the part sequence" << endl;
  fCurrentVoice->
    appendTupletToVoice (tup);
}          

void xml2MsrScoreVisitor::attachPendingDynamicsAndWedgesToNote (
  S_msrNote note)
{
  // attach the pending dynamics if any to the note
  if (! fPendingDynamics.empty()) {
    if (fMusicXMLNoteData.fMusicXMLStepIsARest) {
      if (fTranslationSettings->fDelayRestsDynamics) {
      cerr << idtr <<
        "--> Delaying dynamics attached to a rest until next note" << endl;
      } else {
        cerr << idtr <<
          "--> There is dynamics attached to a rest" << endl;
      }
    } else {
      while (! fPendingDynamics.empty()) {
        S_msrDynamics
          dyn =
            fPendingDynamics.front();
        note->addDynamics (dyn);
        fPendingDynamics.pop_front();
      } // while
    }
  }
  
  // attach the pending wedges if any to the note
  if (! fPendingWedges.empty()) {
    if (fMusicXMLNoteData.fMusicXMLStepIsARest) {
      if (fTranslationSettings->fDelayRestsDynamics) {
      cerr << idtr <<
        "--> Delaying wedge attached to a rest until next note" << endl;
      } else {
        cerr << idtr <<
          "--> There is a wedge attached to a rest" << endl;
      }
    } else {
      while (! fPendingWedges.empty()) {
        S_msrWedge
          wdg =
            fPendingWedges.front();
        note->addWedge (wdg);
        fPendingWedges.pop_front();
      } // while
    }
  }
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
  
//  if (true || fTranslationSettings->fDebug)
  if (fTranslationSettings->fDebug)
    cerr <<
      idtr <<
      "!!!! BEFORE visitEnd (S_note) we have:" << endl <<
      idtr << idtr <<
      "--> fCurrentStaffNumber = " << fCurrentStaffNumber << endl <<
      idtr << idtr <<
      "--> current staff name  = " << fCurrentStaff->getStaffName() << endl <<
      idtr << idtr <<
      "--> fCurrentVoiceNumber = " << fCurrentVoiceNumber << endl;

  // is fCurrentStaffNumber already present in fCurrentPart?
  fCurrentStaff =
    fCurrentPart->
      fetchStaffFromPart (fCurrentStaffNumber);

  if (! fCurrentStaff) 
    // no, add it to the current part
    fCurrentStaff =
      fCurrentPart->
        addStaffToPart (fCurrentStaffNumber);
    
  // fetch the note's voice in the current staff
  fCurrentVoice =
    fCurrentStaff->
      fetchVoiceFromStaff (fCurrentVoiceNumber);

/* JMI
  // no, add it to the current staff
  if (! fCurrentVoice) 
    fCurrentVoice =
      fCurrentStaff->
        addVoiceToStaff (fCurrentVoiceNumber);
*/

  // store voice number in MusicXML note data
  fMusicXMLNoteData.fMusicXMLVoiceNumber = fCurrentVoiceNumber;

  fCurrentStemDirection = kStemNeutral;
  
  if (fTranslationSettings->fDebug)
    cerr <<
      "fMusicXMLNoteData.fMusicXMLDuration = " << 
      fMusicXMLNoteData.fMusicXMLDuration << ", " << 
      "fCurrentMusicXMLDivisions*4 = " <<
      fCurrentMusicXMLDivisions*4 << endl;
      
  if (fCurrentMusicXMLDivisions <= 0)
    msrMusicXMLError ("divisions cannot be 0 nor negative");
  
  fMusicXMLNoteData.fMusicXMLDivisions =
    fCurrentMusicXMLDivisions;
  fMusicXMLNoteData.fMusicXMLTupletMemberNoteType =
    fCurrentNoteType;
  
  //cout << "::: creating a note" << endl;
  S_msrNote newNote =
    msrNote::createFromMusicXMLData (
      fTranslationSettings,
      fMusicXMLNoteData,
      fCurrentSlurKind);

  // attach the articulations if any to the note
  while (! fCurrentArticulations.empty()) {
    S_msrArticulation
      art =
        fCurrentArticulations.front();
    newNote->
      addArticulation (art);
    fCurrentArticulations.pop_front();
  } // while
   
  attachPendingDynamicsAndWedgesToNote (newNote);

  /*
  A note can be standalone
  or a member of a chord,
  and the latter can belong to a tuplet.
  
  A rest can be standalone or belong to a tuplet
  */
  
  if (fMusicXMLNoteData.fMusicXMLNoteBelongsToAChord) {

    handleNoteBelongingToAChord (newNote);

  } else if (fMusicXMLNoteData.fMusicXMLNoteBelongsToATuplet) {

    handleNoteBelongingToATuplet (newNote);
    
  } else { // standalone note/rest

    handleStandaloneNoteOrRest (newNote);
    
  }

  // keep track of note/rest in this visitor
  fCurrentNote = newNote;
  fCurrentPositionInMeasure +=
    fCurrentNote->
      getNoteMsrDuration ()->durationAsRational ();
    
// JMI  fCurrentElement = fCurrentNote; // another name for it

//  if (true || fTranslationSettings->fDebug)
  if (fTranslationSettings->fDebug)
    cerr <<
      idtr <<
      "!!!! AFTER visitEnd (S_note) " << fCurrentNote << "we have:" << endl <<
      idtr << idtr <<
      "--> fCurrentStaffNumber = " << fCurrentStaffNumber << endl <<
      idtr << idtr <<
      "--> current staff name  = " << fCurrentStaff->getStaffName() << endl <<
      idtr << idtr <<
      "--> fCurrentVoiceNumber = " << fCurrentVoiceNumber << endl <<
      idtr << idtr <<
      "--> fCurrentVoice        = " << fCurrentVoice->getVoiceName() << endl;

  fOnGoingNote = false;
} // xml2MsrScoreVisitor::visitEnd ( S_note& elt )

void xml2MsrScoreVisitor::handleStandaloneNoteOrRest (
  S_msrNote newNote)
{
  // register note/rest as standalone
//  if (true || fTranslationSettings->fDebug)
  if (fTranslationSettings->fDebug)
    cerr <<  idtr <<
      "--> adding standalone " <<
      newNote->notePitchAsLilypondString () <<
      " to current voice" << endl;

  // is voice fCurrentVoiceNumber present in current staff?
  fCurrentVoice =
    fCurrentStaff->
      fetchVoiceFromStaff (fCurrentVoiceNumber);

  if (! fCurrentVoice)
    // no, add it to the staff
    fCurrentVoice =
      fCurrentStaff->
        addVoiceToStaff (fCurrentVoiceNumber);
    
  fCurrentVoice->
    appendNoteToVoice (newNote);

  if (fCurrentLyricsHasText)
    addLyricsToCurrentVoice ();

  // account for chord not being built
  fOnGoingChord = false;
}

void xml2MsrScoreVisitor::handleNoteBelongingToAChord (S_msrNote newNote)
{
  if (fMusicXMLNoteData.fMusicXMLStepIsARest)
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
      "--> adding new note " <<
      newNote->notePitchAsLilypondString() <<
      " to current chord" << endl;
    
  // register note as a member of fCurrentChord
  if (fTranslationSettings->fDebug)
    cout << idtr <<
      "--> registering new note " <<
      newNote->notePitchAsLilypondString() <<
      " as a member of current chord" << endl;
  fCurrentChord->
    addNoteToChord (newNote);
    
  // remove previous current note or the previous state of the chord
  // from the current voice sequence
  if (fTranslationSettings->fDebug)
    cout << idtr <<
      "--> removing last element " <<
      fCurrentVoice->getVoiceSequenceLastElement () <<
      " from current voice" << endl;
// JMI    fCurrentVoice->removeElementFromVoiceSequence (fCurrentNote);
  fCurrentVoice->
    removeLastElementFromVoiceSequence ();

  // add fCurrentChord to the part sequence instead
  if (fTranslationSettings->fDebug)
    cout << idtr <<
      "--> appending chord " << fCurrentChord <<
      " to current voice" << endl;
  fCurrentVoice->
    appendChordToVoice (fCurrentChord);
}

void xml2MsrScoreVisitor::handleNoteBelongingToATuplet (
  S_msrNote newNote)
{
  fMusicXMLNoteData.fMusicXMLTupletMemberNoteType = fCurrentNoteType;
  
  switch (fCurrentTupletKind) {
    case msrTuplet::kStartTuplet:
      {
        createTuplet (newNote);
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
            "--> adding note " << newNote <<
            " to tuplets stack top" << endl;
        fCurrentTupletsStack.top()->
          addElementToTuplet (newNote);
      }
      break;

    case msrTuplet::kStopTuplet:
      {
        finalizeTuplet(newNote);

        // indicate the end of the tuplet
        fOnGoingTuplet = false;
      }
      break;
    default:
      {}
  } // switch
}

void xml2MsrScoreVisitor::addLyricsToCurrentVoice ()
{
 // is lyrics fCurrentLyricsNumber present in current voice?
  fCurrentLyrics =
    fCurrentVoice->
      voiceContainsLyrics (fCurrentLyricsNumber);

  if (! fCurrentLyrics)
    // no, add it to the voice
    fCurrentLyrics =
      fCurrentVoice->
        addLyricsToVoice (fCurrentLyricsNumber);

  S_msrDuration
    lyricMsrDuration =
      msrDuration::create (
        fMusicXMLNoteData.fMusicXMLDuration,
        fCurrentMusicXMLDivisions,
        fMusicXMLNoteData.fMusicXMLDotsNumber,
        fMusicXMLNoteData.fMusicXMLTupletMemberNoteType);
  
 // JMI if (fOnGoingSlur) {
  if (
    fCurrentSlurKind == msrSlur::kContinueSlur
      ||
    fCurrentSlurKind == msrSlur::kStopSlur) {
      
    fCurrentLyrics->
      addSlurChunkToLyrics (
        lyricMsrDuration);
        
  } else if (fMusicXMLNoteData.fMusicXMLNoteIsTied) {
    
    fCurrentLyrics->
      addTiedChunkToLyrics (
        lyricMsrDuration);
        
  } else {
    
    fCurrentLyrics->
      addTextChunkToLyrics (
        fCurrentSyllabic,
        fCurrentText,
        fCurrentElision,
        lyricMsrDuration);
  }
}


} // namespace
