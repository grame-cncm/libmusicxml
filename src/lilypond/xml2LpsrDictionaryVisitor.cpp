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

#include "xml_tree_browser.h"

#include "lpsrUtilities.h"

#include "lpsr.h"
#include "xml2LpsrDictionaryVisitor.h"


using namespace std;

namespace MusicXML2
{
//________________________________________________________________________
xml2LpsrDictionaryVisitor::xml2LpsrDictionaryVisitor (
  S_translationSettings& ts)
{
  fTranslationSettings = ts;

  // create the dictionary
  fDictionary =
    lpsrDictionary::create (fTranslationSettings);

  // add the implicit part group to the dictionary
  fCurrentPartGroupNumber = 1;
  fCurrentPartGroup =
    fDictionary->
      addPartGroupToDictionary (fCurrentPartGroupNumber);

  // add the implicit part to the implicit part group
  string partXMLName = "implicit";
  fCurrentPart =
    fCurrentPartGroup->
      addPartToPartGroup (partXMLName); // JMI ???

  // add the implicit staff to the implicit part
  fCurrentStaffNumber = 1;
  fCurrentStaff =
    fCurrentPart->
      addStaffToPart (fCurrentStaffNumber); // JMI ???

  // add the implicit voice to the implicit staff
  fCurrentVoiceNumber = 1;
  fCurrentVoice =
    fCurrentStaff->
      addVoiceToStaff (fCurrentVoiceNumber); // JMI ???

  // add the implicit lyrics to the implicit voice
  fCurrentLyricNumber = 1;
  fCurrentLyrics =
    fCurrentVoice->
      addLyricsToVoice (fCurrentLyricNumber); // JMI ???
  
  fCurrentMeasureNumber = 0;
}

xml2LpsrDictionaryVisitor::~xml2LpsrDictionaryVisitor () {}

//________________________________________________________________________
S_lpsrDictionary
xml2LpsrDictionaryVisitor::buildDictionaryFromXMLElementTree (
  const Sxmlelement& xmlTree)
{
  S_lpsrDictionary result;
  
  if (xmlTree) {
    // create a tree browser on this visitor
    tree_browser<xmlelement> browser (this);
    
    // browse the xmlelement tree
    browser.browse (*xmlTree);
    
    // grab the resulting dictionary
    result = fDictionary;
  }

  return result;
}

void xml2LpsrDictionaryVisitor::internalError (
  string message)
{
  cerr <<
    "### Internal error: measure " << fCurrentMeasureNumber << endl <<
    message << endl;
}                        

//________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart (S_part_list& elt)
{
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

//________________________________________________________________________

/*
    <part-group type="start" number="1">
    </part-group>
    <part-group type="stop" number="1"/>
 */
void xml2LpsrDictionaryVisitor::visitStart (S_part_group& elt)
{
  // the part group number indicates nested/overlapping groups
  fCurrentPartGroupNumber = elt->getAttributeIntValue ("number", 0);
  fCurrentPartGroupType = elt->getAttributeValue ("type");

  if (fCurrentPartGroupType == "start") {

    // is this part group number already present?
    fCurrentPartGroup =
      fDictionary->dictionaryContainsPartGroup (
        fCurrentPartGroupNumber);

    // no, add it to the dictionary
    if (! fCurrentPartGroup) 
    fCurrentPartGroup =
      fDictionary->addPartGroupToDictionary (
        fCurrentPartGroupNumber);
        
  } else if (fCurrentPartGroupType == "stop") {

      fCurrentPartGroup->popPartGroupPartsStackTop ();
    
  } else {
    
    lpsrMusicXMLError (
      "unkown part group type \"" + fCurrentPartGroupType + "\"");
  }
}

void xml2LpsrDictionaryVisitor::visitStart (S_group_symbol& elt)
{
  fCurrentGroupSymbol = elt->getValue ();

  fCurrentPartGroup->setPartGroupSymbol (fCurrentGroupSymbol);
}

void xml2LpsrDictionaryVisitor::visitStart ( S_group_barline& elt)
{
  fCurrentGroupBarline = elt->getValue ();

  fCurrentPartGroup->
    setPartGroupBarline (fCurrentGroupBarline);
}

//________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart (S_score_part& elt)
{
  fCurrentPartMusicXMLName = elt->getAttributeValue ("id");
/*
  // is this part already present?
  fCurrentPart =
    fCurrentPart->
      partContainsStaff (
        fCurrentPartMusicXMLName);

  // no, add it to the current JMI ???
  if (! fCurrentPart)
  */
    fCurrentPart =
      fCurrentPartGroup->
        addPartToPartGroup (
          fCurrentPartMusicXMLName);
}

void xml2LpsrDictionaryVisitor::visitStart (S_part_name& elt)
{
  string fCurrentPartName = elt->getValue ();

  fCurrentPart->
    setPartName (fCurrentPartName);
}

void xml2LpsrDictionaryVisitor::visitStart (S_part_abbreviation& elt)
{
  string fCurrentPartAbbreviation = elt->getValue ();

  fCurrentPart->
    setPartAbbreviation (fCurrentPartAbbreviation);
}

void xml2LpsrDictionaryVisitor::visitStart (S_instrument_name& elt)
{
  string fCurrentPartInstrumentName = elt->getValue(); // jMI

  fCurrentPart->
    setPartInstrumentName (fCurrentPartInstrumentName);
}

//________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart (S_part& elt)
{
  fCurrentStaffNumber = 1; // default if there are no <staff> element

  fCurrentVoiceNumber = -1 ;

  fCurrentMeasureNumber = 0; // in case of an anacrusis
}

//________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart (S_staves& elt)
{
  /* by S_measure in fact!
  fStavesNumber = int(*elt);

  if (fTranslationSettings->fTrace) {
    switch (fStavesNumber) {
      case 0:
        cerr << "There isn't any explicit staff (hence 1 by default)";
        break;
      case 1:
        cerr << "There is 1 staff";
        break;
      default:
        cerr << "There are " << fStavesNumber << " staves";
    } // switch
    cerr <<
      " in part " << fCurrentPart->getPartMusicXMLName() <<
      " (" << fCurrentPart->getPartLPSRName() << ")" << endl;
  }
  */
}

void xml2LpsrDictionaryVisitor::visitStart (S_staff& elt)
{
  fCurrentStaffNumber = int(*elt);

  // is this staff already present?
  fCurrentStaff =
    fCurrentPart->
      partContainsStaff (
        fCurrentStaffNumber);

  // no, add it to the current part
  if (! fCurrentStaff) 
    fCurrentStaff =
      fCurrentPart->
        addStaffToPart (fCurrentStaffNumber);
}
    
//________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart (S_voice& elt )
{
  fCurrentVoiceNumber = int(*elt);
  /*
  cout <<
    "--> S_voice, fCurrentVoiceNumber = " << fCurrentVoiceNumber << endl;
  */

  // is this voice already present?
  fCurrentVoice =
    fCurrentStaff->
      staffContainsVoice (
        fCurrentPartGroupNumber);

  // no, add it to the current staff
  if (! fCurrentVoice) 
    fCurrentVoice =
      fCurrentStaff->
        addVoiceToStaff (
          fCurrentVoiceNumber);
}

//________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart (S_lyric& elt ) { 
  fCurrentLyricNumber =
    elt->getAttributeIntValue ("number", 0);

  // is this lyrics already present?
  fCurrentLyrics =
    fCurrentVoice->voiceContainsLyrics (
      fCurrentLyricNumber);

  // no, add it to the current staff
  if (! fCurrentLyrics) 
  fCurrentLyrics =
    fCurrentVoice->
      addLyricsToVoice (
        fCurrentLyricNumber);
}

//________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart (S_measure& elt)
{
  fCurrentMeasureNumber =
    elt->getAttributeIntValue ("number", 0);
}

//________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitEnd (S_note& elt)
{
  // account for note in staff
  //fStavesNotesNumbersMap [fCurrentStaffNumber] ++;
  
  // account for note in voice
 // fVoicesNotesNumbersMap [fCurrentVoiceNumber] ++;
  
  // account for voice in staff
 // fStaffVoicesAndNotesNumbersMap [fCurrentStaffNumber] [fCurrentVoiceNumber]++;
}

/*
  cout <<
    "--> xml2LpsrDictionaryVisitor::visitEnd ( S_note& elt) :" << endl <<
    "  fCurrentStaffNumber = " << fCurrentStaffNumber << endl <<
    "  fCurrentVoiceNumber = " << fCurrentVoiceNumber << endl <<
    "  fStavesNotesNumbersMap [fCurrentStaffNumber] = " << fStavesNotesNumbersMap [fCurrentStaffNumber] << endl <<
    "  fStavesNotesNumbersMap [fCurrentStaffNumber] = " << fStavesNotesNumbersMap [fCurrentStaffNumber] << endl <<
    "  fVoicesLyrisNumbersMap [fCurrentVoiceNumber] = " << fVoicesLyrisNumbersMap [fCurrentVoiceNumber] << endl <<
    "  fStaffVoicesAndNotesNumbersMap [fCurrentStaffNumber][fCurrentVoiceNumber] = " <<
    fStaffVoicesAndNotesNumbersMap  [fCurrentStaffNumber][fCurrentVoiceNumber] << endl;
*/

//     string       lyricsName =
//                  "_Lyrics" + int2EnglishWord (atoi (it1->first.c_str()));

} // namespace
