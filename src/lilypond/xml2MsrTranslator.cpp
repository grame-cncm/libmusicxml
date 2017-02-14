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
#include <climits>      /* INT_MIN */
#include <iomanip>      // setw, set::precision, ...
#include <algorithm>    /* for_each */

#include "xml_tree_browser.h"

#include "conversions.h"

#include "xml2MsrTranslator.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
xml2MsrTranslator::xml2MsrTranslator ()
{
  fMillimeters       = -1;
  fTenths            = -1;
  fOnGoingPageLayout = false;

  fCurrentDivisionsPerQuarterNote = 0;

  // create the MSR score
  fMsrScore =
    msrScore::create (0);

  fOnGoingGroupNameDisplay = false;
  
  fOnGoingBarline = false;
  
  fCurrentWordsContents = "";

  fCurrentForwardStaffNumber = 1; // JMI
  fCurrentForwardVoiceNumber = 1; // JMI
  fCurrentVoiceNumber = 1; // JMI

  fOnGoingLyric = false;
  fCurrentStanzaNumber = -1; // JMI
  fCurrentSyllabic = "";
  fCurrentText = "";
  fCurrentElision = false;
  
  fCurrentSyllableKind =
    msrSyllable::k_NoSyllable;
  fCurrentSyllableExtendKind =
    msrSyllable::k_NoSyllableExtend;
  fOnGoingSyllableExtend = false;

  fFirstSyllableInSlurKind =
    msrSyllable::k_NoSyllable;
  
  fCurrentBackupDuration = -1;

  fCurrentDirectionStaffNumber = 1; // it may be absent

  fCurrentNoteStaffNumber = 0;
  fCurrentNoteVoiceNumber = 0;
  fOnGoingNote            = false;

  fOnGoingChord        = false;
  
  fOnGoingSlur          = false;
  fOnGoingSlurHasStanza = false;

  fOnGoingDirection     = true;
  fOnGoingDirectionType = false;

  fOnGoingRepeat = false;
  
  fOnGoingBackup  = false;
  fOnGoingForward = false;
}

xml2MsrTranslator::~xml2MsrTranslator ()
{}

//________________________________________________________________________
S_msrScore xml2MsrTranslator::buildMsrScoreFromXMLElementTree (
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

//________________________________________________________________________
S_msrPartgroup xml2MsrTranslator::createImplicitMSRPartgroup (
  int inputLineNumber)
{
  /*
  A first part group is created with all the needed contents
  if none is specified in the MusicXML data.
  */

  // create an implicit part group
  fCurrentPartgroupNumber = 1;
  
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Creating an implicit part group with number " <<
      fCurrentPartgroupNumber <<
      endl;

  S_msrPartgroup
    partgroup =
      msrPartgroup::create (
        inputLineNumber,
        fCurrentPartgroupNumber,
        "Implicit",
        "",
        "",
        "Impl.",
        msrPartgroup::kBracketPartgroupSymbol,
        0,
        true,
        0); // the top level part group has an empty uplink

  /*
    this implicit part group will be added to the MSR score
    in method 'visitEnd (S_part_list& elt)'
  */
  
  // add implicit part group to the map of this visitor
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Adding implicit part group " << fCurrentPartgroupNumber <<
      " to visitor's data" <<
      endl;
      
  fPartgroupsMap [fCurrentPartgroupNumber] = partgroup;
  fPartgroupsList.push_front (partgroup);

  fImplicitPartgroup = partgroup;
  
  return partgroup;
}

//______________________________________________________________________________
S_msrPartgroup xml2MsrTranslator::fetchPartgroupInThisVisitor (
  int partgroupNumber)
{
  S_msrPartgroup result;
  
  if (fPartgroupsMap.count (partgroupNumber)) {
    result =
      fPartgroupsMap [partgroupNumber];
  }

  return result;
}

/*
//______________________________________________________________________________
S_msrPart xml2MsrTranslator::createDownToPartIfNeeded (
  int            inputLineNumber,
  S_msrPartgroup partgroup)
{
  // is this part already present in the current part group?
  S_msrPart
    part =
      partgroup->
        fetchPartFromPartgroup (fCurrentPartID);

  if (! part) {
    // no, add it to the current part group
    part =
      partgroup->
        addPartToPartgroup (
          inputLineNumber, fCurrentPartID);
  }

  return part;
}
*/

//______________________________________________________________________________
S_msrStaff xml2MsrTranslator::createStaffInCurrentPartIfNeeded (
  int            inputLineNumber,
  int            staffNumber)
{    
  // is staffNumber already present in part?
  S_msrStaff
    staff =
      fCurrentPart->
        fetchStaffFromPart (staffNumber);

  if (! staff) 
    // no, add it to fCurrentPart
    staff =
      fCurrentPart->
        addStaffToPartByItsNumber (
          inputLineNumber, staffNumber);

  return staff;
}  

//______________________________________________________________________________
S_msrVoice xml2MsrTranslator::registerVoiceInStaffInCurrentPartIfNeeded (
  int            inputLineNumber,
  int            staffNumber,
  int            voiceNumber)
{
  S_msrStaff
    staff =
      createStaffInCurrentPartIfNeeded (
        inputLineNumber,
        staffNumber);

  // is voiceNumber already present in staff?
  S_msrVoice
    voice =
      staff->
        fetchVoiceFromStaff (
          inputLineNumber, voiceNumber);

  if (! voice) 
    // no, add it to the staff
    voice =
      staff->
        registerVoiceInStaffByItsExternalNumber (
          inputLineNumber, voiceNumber);

  return voice;
}  

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_work_number& elt )
{
  fMsrScore->getIdentification () ->
    setWorkNumber (
      elt->getInputLineNumber (),
      elt->getValue ());
}

void xml2MsrTranslator::visitStart ( S_work_title& elt )
{
  fMsrScore->getIdentification () ->
    setWorkTitle (
      elt->getInputLineNumber (),
      elt->getValue ());
}
  
void xml2MsrTranslator::visitStart ( S_movement_number& elt )
{
  fMsrScore->getIdentification () ->
    setMovementNumber (
      elt->getInputLineNumber (),
      elt->getValue ());
}

void xml2MsrTranslator::visitStart ( S_movement_title& elt )
{
  fMsrScore->getIdentification () ->
    setMovementTitle (
      elt->getInputLineNumber (),
      elt->getValue ());
}

void xml2MsrTranslator::visitStart ( S_creator& elt )
{
  fMsrScore->getIdentification () ->
    addCreator (
      elt->getInputLineNumber (),
      elt->getAttributeValue ("type"),
      elt->getValue ());
}

void xml2MsrTranslator::visitStart ( S_rights& elt )
{
  fMsrScore->getIdentification () ->
    setRights (
      elt->getInputLineNumber (),
      elt->getValue ());
}

void xml2MsrTranslator::visitStart ( S_software& elt )
{
  fMsrScore->getIdentification () ->
    addSoftware (
      elt->getInputLineNumber (),
      elt->getValue ());
}

void xml2MsrTranslator::visitStart ( S_encoding_date& elt )
{
  fMsrScore->getIdentification () ->
    setEncodingDate (
      elt->getInputLineNumber (),
      elt->getValue ());
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_millimeters& elt )
{ 
  fMillimeters = (int)(*elt);
  
  fMsrScore->getPageGeometry ()->
    setMillimeters (fMillimeters);
}

void xml2MsrTranslator::visitStart ( S_tenths& elt )
{
  fTenths = (int)(*elt);

  fMsrScore->getPageGeometry ()->
    setTenths (fTenths);
}

void xml2MsrTranslator::visitEnd ( S_scaling& elt)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "There are " << fTenths <<
      " tenths for " <<  fMillimeters <<
      " millimeters, hence the global staff size is " <<
      fMsrScore->getPageGeometry ()->globalStaffSize () <<
      endl;
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_system_distance& elt )
{
  int systemDistance = (int)(*elt);
  
//  cerr << "--> systemDistance = " << systemDistance << endl;
  fMsrScore->getPageGeometry ()->
    setBetweenSystemSpace (
      1.0 * systemDistance * fMillimeters / fTenths / 10);  
}

void xml2MsrTranslator::visitStart ( S_top_system_distance& elt )
{
  int topSystemDistance = (int)(*elt);
  
//  cerr << "--> fTopSystemDistance = " << topSystemDistance << endl;
    fMsrScore->getPageGeometry ()->
    setPageTopSpace (
      1.0 * topSystemDistance * fMillimeters / fTenths / 10);  
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_page_layout& elt )
{
  fOnGoingPageLayout = true;
}
void xml2MsrTranslator::visitEnd ( S_page_layout& elt )
{
  fOnGoingPageLayout = false;
}

void xml2MsrTranslator::visitStart ( S_page_height& elt )
{
  if (fOnGoingPageLayout) {
    int pageHeight = (int)(*elt);
    
    //cerr << "--> pageHeight = " << pageHeight << endl;
    fMsrScore->getPageGeometry ()->
      setPaperHeight (
        1.0 * pageHeight * fMillimeters / fTenths / 10);  
  }
}

void xml2MsrTranslator::visitStart ( S_page_width& elt )
{
  if (fOnGoingPageLayout) {
    int pageWidth = (int)(*elt);
    
    //cerr << "--> pageWidth = " << pageWidth << endl;
    fMsrScore->getPageGeometry ()->
      setPaperWidth (
        1.0 * pageWidth * fMillimeters / fTenths / 10);  
  }
}

void xml2MsrTranslator::visitStart ( S_left_margin& elt )
{
  if (fOnGoingPageLayout) {
    int leftMargin = (int)(*elt);
    
    //cerr << "--> leftMargin = " << leftMargin << endl;
    fMsrScore->getPageGeometry ()->
      setLeftMargin (
        1.0 * leftMargin * fMillimeters / fTenths / 10);  
  }
}

void xml2MsrTranslator::visitStart ( S_right_margin& elt )
{
  if (fOnGoingPageLayout) {
    int rightMargin = (int)(*elt);
    
    //cerr << "--> rightMargin = " << rightMargin << endl;
    fMsrScore->getPageGeometry ()->
      setRightMargin (
        1.0 * rightMargin * fMillimeters / fTenths / 10);  
  }
}

void xml2MsrTranslator::visitStart ( S_top_margin& elt )
{
  if (fOnGoingPageLayout) {
    int topMargin = (int)(*elt);
    
    //cerr << "--> topMargin = " << topMargin << endl;
    fMsrScore->getPageGeometry ()->
      setTopMargin (
        1.0 * topMargin * fMillimeters / fTenths / 10);  
  }
}

void xml2MsrTranslator::visitStart ( S_bottom_margin& elt )
{
  if (fOnGoingPageLayout) {
    int bottomMargin = (int)(*elt);
    
    //cerr << "--> bottomMargin = " << bottomMargin << endl;
    fMsrScore->getPageGeometry ()->
      setBottomMargin (
        1.0 * bottomMargin * fMillimeters / fTenths / 10);  
  }
}

//________________________________________________________________________
void xml2MsrTranslator::visitStart (S_part_list& elt)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Analysing part list" <<
      endl;

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

void xml2MsrTranslator::visitEnd (S_part_list& elt)
{
  idtr--;

  if (fImplicitPartgroup) {
    // force an implicit part group "stop" on it
    // fCurrentPartgroupNumber holds the value 1
    handlePartgroupStop (elt->getInputLineNumber ());
    
    fImplicitPartgroup = 0;
  }
}

//________________________________________________________________________
/*
  There is no hierarchy implied in part-group elements.
  All that matters is the sequence of part-group elements
  relative to score-part elements.
  The sequencing of two consecutive part-group elements does not matter.
  It is the default-x attribute that indicates
  the left-to-right ordering of the group symbols.

  <part-group number="1" type="start">
  <group-name>Trombones</group-name>
  <group-abbreviation>Trb.</group-abbreviation>
  <group-symbol default-x="-12">brace</group-symbol>
  <group-barline>yes</group-barline>
  </part-group>

    <part-group number="3" type="start">
      <group-name>Trumpet in B flat</group-name>
      <group-name-display>
        <display-text>Trumpet in B</display-text>
        <accidental-text>flat</accidental-text>
      </group-name-display>
      <group-abbreviation>Trp. in B flat</group-abbreviation>
      <group-abbreviation-display>
        <display-text>Trp. in B</display-text>
        <accidental-text>flat</accidental-text>
      </group-abbreviation-display>
      <group-symbol default-x="-10">square</group-symbol>
      <group-barline>yes</group-barline>
    </part-group>
*/

//________________________________________________________________________
void xml2MsrTranslator::visitStart (S_part_group& elt)
{  
  fCurrentPartgroupNumber =
    elt->getAttributeIntValue ("number", 0);
    
  fCurrentPartgroupType =
    elt->getAttributeValue ("type");

  fCurrentPartgroupName = "";
  fCurrentPartgroupDisplayText = "";
  fCurrentPartgroupAccidentalText = "";
  fCurrentPartgroupAbbreviation = "";
  fCurrentPartgroupSymbol = "";
  fCurrentPartgroupSymbolDefaultX = INT_MIN;
  fCurrentPartgroupBarline = "yes";
}

void xml2MsrTranslator::visitStart (S_group_name& elt)
{
  fCurrentPartgroupName = elt->getValue();
}

void xml2MsrTranslator::visitStart (S_group_name_display& elt)
{
  fOnGoingGroupNameDisplay = true;
}

void xml2MsrTranslator::visitStart (S_display_text& elt)
{
  if (fOnGoingGroupNameDisplay)
    fCurrentPartgroupDisplayText = elt->getValue();
}

void xml2MsrTranslator::visitStart (S_accidental_text& elt)
{
  if (fOnGoingGroupNameDisplay)
    fCurrentPartgroupAccidentalText = elt->getValue();
}

void xml2MsrTranslator::visitStart (S_group_abbreviation& elt)
{
  fCurrentPartgroupAbbreviation = elt->getValue ();
}

void xml2MsrTranslator::visitStart (S_group_symbol& elt)
{
  fCurrentPartgroupSymbol = elt->getValue ();

  fCurrentPartgroupSymbolDefaultX =
    elt->getAttributeIntValue ("default-x", 0);
}

void xml2MsrTranslator::visitStart ( S_group_barline& elt)
{
  fCurrentPartgroupBarline = elt->getValue ();
}

//________________________________________________________________________
void xml2MsrTranslator::showPartgroupsData (string context)
{    
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebugDebug) {
    cerr << idtr <<
      "==> " << context << ": fPartgroupsMap contains:" <<
      endl;
      
    if (fPartgroupsMap.size()) {
      map<int, S_msrPartgroup>::const_iterator
        iBegin = fPartgroupsMap.begin(),
        iEnd   = fPartgroupsMap.end(),
        i      = iBegin;
        
      idtr++;
      for ( ; ; ) {
        cerr << idtr <<
          "\"" << (*i).first << "\" ----> " << (*i).second;
        if (++i == iEnd) break;
        cerr << endl;
      } // for
      idtr--;
    }
    cerr <<
      idtr << "<== fPartgroupsMap" <<
      endl << endl <<
      
    idtr <<
      "==> " << context << ": fPartgroupsList contains:" <<
      endl;
      
    if (fPartgroupsList.size()) {
      list<S_msrPartgroup>::const_iterator
        iBegin = fPartgroupsList.begin(),
        iEnd   = fPartgroupsList.end(),
        i      = iBegin;
        
      idtr++;
      for ( ; ; ) {
        cerr << idtr << (*i);
        if (++i == iEnd) break;
        cerr << endl;
      } // for
      idtr--;
    }
    cerr <<
      idtr << "<== fPartgroupsList" <<
      endl << endl;
  }
}

//________________________________________________________________________
void xml2MsrTranslator::handlePartgroupStart (
  int     inputLineNumber,
  msrPartgroup::msrPartgroupSymbolKind
          partgroupSymbol,
  bool    partgroupBarline)
{
  showPartgroupsData ("BEFORE START");

  // fetch part group to be started
  S_msrPartgroup
    partgroupToBeStarted =
      fetchPartgroupInThisVisitor (
        fCurrentPartgroupNumber);

  // the current part group is either null
  // or the head of the part group list
  S_msrPartgroup
    currentPartgroup =
      fPartgroupsList.size ()
        ? fPartgroupsList.front ()
        : 0;
        
  if (! partgroupToBeStarted) {
    // no, create it
    partgroupToBeStarted =
      msrPartgroup::create (
        inputLineNumber,
        fCurrentPartgroupNumber,
        fCurrentPartgroupName,
        fCurrentPartgroupDisplayText,
        fCurrentPartgroupAccidentalText,
        fCurrentPartgroupAbbreviation,
        partgroupSymbol,
        fCurrentPartgroupSymbolDefaultX,
        partgroupBarline,
        currentPartgroup);

    // add it to the part group map of this visitor
    if (gGeneralOptions->fTrace)
      cerr << idtr <<
        "Adding part group " << fCurrentPartgroupNumber <<
        " to visitor's part group map" <<
        endl;
    fPartgroupsMap [fCurrentPartgroupNumber] =
      partgroupToBeStarted;
  }
  
  // add it to the part group list of this visitor
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Adding part group " << fCurrentPartgroupNumber <<
      " to visitor's part groups list" <<
      endl;

  if (! fPartgroupsList.size())
    // insert first part group ahead of the list
    fPartgroupsList.push_front (partgroupToBeStarted);
  else {
    // place in the part groups list so as to
    // have them ordered by increasing order
    // (all of them they are negative)
    list<S_msrPartgroup>::iterator
      iBegin = fPartgroupsList.begin(),
      iEnd   = fPartgroupsList.end(),
      i      = iBegin;

    while (true) {
      if (i == iEnd) {
        fPartgroupsList.push_back (partgroupToBeStarted);
        break;
      }

      // CAUTION: insert() inserts before the position
      // indicated by its iterator argument
      if (
          fCurrentPartgroupSymbolDefaultX
            <
          (*i)->getPartgroupSymbolDefaultX ()) {
        fPartgroupsList.insert (i, partgroupToBeStarted);
        break;
      }
      
      i++;
    } // while
  }
  
  showPartgroupsData ("AFTER START");
}
  
//________________________________________________________________________
void xml2MsrTranslator::handlePartgroupStop (int inputLineNumber)
{
  showPartgroupsData ("BEFORE STOP");

  // is the part group to be stopped known?
  S_msrPartgroup
    partgroupToBeStopped =
      fetchPartgroupInThisVisitor (
        fCurrentPartgroupNumber);

  if (! partgroupToBeStopped) {
    // no, but we should have fount it
    stringstream s;

    s <<
      "part group " << fCurrentPartgroupNumber <<
      " not found in this visitor's part groups map" << endl;
    msrInternalError (
      inputLineNumber,
      s.str());
  }

  // remove the part group to be stopped from the part group list
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Removing part group " <<
      partgroupToBeStopped->getPartgroupNumber () <<
      " from visitor's part groups list" <<
      endl;

  list<S_msrPartgroup>::iterator
    iBegin = fPartgroupsList.begin(),
    iEnd   = fPartgroupsList.end(),
    i      = iBegin;

  while (true) {
    if (i == iEnd) {
      stringstream s;
      s <<
        "part group " <<
        fCurrentPartgroupNumber <<
        " not found in part groups list";
        
      msrInternalError (
        inputLineNumber,
        s.str());
      break;
    }

    if ((*i) == partgroupToBeStopped) {
      fPartgroupsList.erase (i);
      break;
    }
    
    i++;
  } // while

  showPartgroupsData ("AFTER REMOVAL FROM LIST");
  
  // take care of the part group to be stopped
  // in the part groups list
  if (! fPartgroupsList.size()) {
    
    // we're just removed the only part group in the list:
    // append it to the MSR score
    if (gGeneralOptions->fTrace)
      cerr << idtr <<
        "Appending part group " <<
        partgroupToBeStopped->getPartgroupNumber () <<
        " to MSR score" <<
        endl;
        
    fMsrScore->
      addPartgroupToScore (partgroupToBeStopped);
      
  }
  
  else {

    // the front element in the part group list is
    // the new current part group
    S_msrPartgroup
      newCurrentPartgroup = fPartgroupsList.front ();

    if (
        partgroupToBeStopped->getPartgroupNumber ()
          ==
        newCurrentPartgroup->getPartgroupNumber () ) {
      cerr << idtr <<
        "--> partgroupToBeStopped = " << partgroupToBeStopped <<
        ", newCurrentPartgroup = " << newCurrentPartgroup <<
        endl;

      stringstream s;
      s <<
        "cannot append part group " <<
        partgroupToBeStopped->getPartgroupNumber () <<
        " as sub part group of itself";
      msrInternalError (
        inputLineNumber,
        s.str());
    }
    
    // insert current group into future current group
    if (gGeneralOptions->fTrace)
      cerr << idtr <<
        "Preending (sub-)part group " <<
        partgroupToBeStopped->getPartgroupNumber () <<
        " at the beginning of part group " <<
        newCurrentPartgroup->getPartgroupNumber () <<
        endl;

    newCurrentPartgroup->
      prependSubPartgroupToPartgroup (
        partgroupToBeStopped);
  }

  // remove part group from the map
  // CAUTION: erase() destroys the element it removes!
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Removing part group " << fCurrentPartgroupNumber <<
      " from visitor's part group map" <<
      endl;
  try {
    fPartgroupsMap.erase (fCurrentPartgroupNumber);
  }
  catch (int e) {
    cerr <<
      "An exception number " << e << " occurred" <<
      endl;
  }

  showPartgroupsData ("AFTER STOP");
}

//________________________________________________________________________
void xml2MsrTranslator::visitEnd (S_part_group& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Handling part group " << fCurrentPartgroupNumber <<
      ", type: \"" << fCurrentPartgroupType << "\""  <<
      endl;

  idtr++;
  
  msrPartgroup::msrPartgroupTypeKind partgroupTypeKind;

  // check part group type
  if      (fCurrentPartgroupType == "start")
    partgroupTypeKind = msrPartgroup::kStartPartgroupType;
    
  else if (fCurrentPartgroupType == "stop")
    partgroupTypeKind = msrPartgroup::kStopPartgroupType;
    
  else {
    if (fCurrentPartgroupType.size())
      // part group type may be absent
      msrMusicXMLError (
        inputLineNumber,
        "unknown part group type \"" + fCurrentPartgroupType + "\"");
        
    partgroupTypeKind = msrPartgroup::k_NoPartgroupType;
  }

  msrPartgroup::msrPartgroupSymbolKind partgroupSymbolKind;
  
  // check part group symbol
  // Values include none,
  //  brace, line, bracket, and square; the default is none
 
  if      (fCurrentPartgroupSymbol == "brace")
    partgroupSymbolKind = msrPartgroup::kBracePartgroupSymbol;
    
  else if (fCurrentPartgroupSymbol == "bracket")
    partgroupSymbolKind = msrPartgroup::kBracketPartgroupSymbol;
    
  else if (fCurrentPartgroupSymbol == "line")
    partgroupSymbolKind = msrPartgroup::kLinePartgroupSymbol;
    
  else if (fCurrentPartgroupSymbol == "square")
    partgroupSymbolKind = msrPartgroup::kSquarePartgroupSymbol;
    
  else if (fCurrentPartgroupSymbol == "none")
    partgroupSymbolKind = msrPartgroup::k_NoPartgroupSymbol;
    
  else {
   if (fCurrentPartgroupSymbol.size())
      // part group type may be absent
      msrMusicXMLError (
        inputLineNumber,
        "unknown part group symbol \"" + fCurrentPartgroupSymbol + "\"");
    partgroupSymbolKind = msrPartgroup::k_NoPartgroupSymbol;
  }

  bool partgroupBarline;
  
  // check part group barline
  if      (fCurrentPartgroupBarline == "yes")
    partgroupBarline = true;
    
  else if (fCurrentPartgroupBarline == "no")
    partgroupBarline = false;
    
  else {
    msrMusicXMLError (
      inputLineNumber,
      "unknown part group barline \"" + fCurrentPartgroupBarline + "\"");
    partgroupBarline = false;
  }

  switch (partgroupTypeKind) {
    
    case msrPartgroup::kStartPartgroupType:
      handlePartgroupStart (
        inputLineNumber,
        partgroupSymbolKind, partgroupBarline);
      break;
      
    case msrPartgroup::kStopPartgroupType:
      handlePartgroupStop (
        inputLineNumber);
      break;
      
    case msrPartgroup::k_NoPartgroupType:
      {}
      break;
  } // switch

  idtr--;
}

//________________________________________________________________________
void xml2MsrTranslator::visitStart (S_score_part& elt)
{
  fCurrentPartID = elt->getAttributeValue ("id");

  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "Found part name \"" << fCurrentPartID << "\"" <<
      endl;

  fCurrentPartName         = "";
  fCurrentPartAbbreviation = "";
  
  fCurrentPartInstrumentName = "";
  fCurrentPartInstrumentAbbreviation = "";
}

void xml2MsrTranslator::visitStart (S_part_name& elt)
{
  fCurrentPartName = elt->getValue ();
}

void xml2MsrTranslator::visitStart (S_part_abbreviation& elt)
{
  fCurrentPartAbbreviation = elt->getValue ();
}

void xml2MsrTranslator::visitStart (S_instrument_name& elt)
{
  fCurrentPartInstrumentName = elt->getValue();
}

void xml2MsrTranslator::visitStart (S_instrument_abbreviation& elt)
{
  fCurrentPartInstrumentAbbreviation = elt->getValue();
}

void xml2MsrTranslator::visitEnd (S_score_part& elt)
{
  string partID = elt->getAttributeValue ("id");

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTrace)
   cerr <<
      idtr <<
      "--------------------------------------------" <<
      endl <<
      idtr <<
      "Handling part \"" << fCurrentPartID << "\"" <<
      endl;

  idtr++;

  S_msrPartgroup partgroup;

  // is there a current part group?
  if (! fPartgroupsList.size()) {
    // no, create an implicit one
    partgroup =
      createImplicitMSRPartgroup (
        inputLineNumber);
  }

  // fetch current part group
  try {
    partgroup = fPartgroupsList.front ();
  }
  catch (int e) {
    cerr <<
      "An exception number " << e << " occurred" <<
      endl;
  }
        
  // is this part already present in the current part group?
  S_msrPart
    part =
      partgroup->
        fetchPartFromPartgroup (fCurrentPartID);

  if (! part) {
    // no, add it to the current part group
    part =
      partgroup->
        addPartToPartgroupByItsID (
          inputLineNumber, fCurrentPartID);
  }
  
  // populate current part
  // fPartMSRName has already been set by the constructor // JMI
  part->
    setPartName (fCurrentPartName);
  part->
    setPartAbbreviation (fCurrentPartAbbreviation);
  part->
    setPartInstrumentName (fCurrentPartInstrumentName);
  part->
    setPartInstrumentAbbreviation (fCurrentPartInstrumentAbbreviation);

  // register it in this visitor's parts map
  fPartsMap [partID] = part;

  if (fImplicitPartgroup) {
    // force an implicit part group "stop" on it
    // fCurrentPartgroupNumber hold the value 1
    handlePartgroupStop (
      inputLineNumber);

    // forget the implicit group
    fImplicitPartgroup = 0;
  }
    
  showPartgroupsData (
    "AFTER handling part \"" + partID + "\"");

  idtr--;
}

//________________________________________________________________________
void xml2MsrTranslator::visitStart (S_part& elt)
{
  // fCurrentPartID is used throughout
  fCurrentPartID = elt->getAttributeValue ("id");

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  // is this part already known?
  if (fPartsMap.count (fCurrentPartID))
    fCurrentPart = // used thoughout
      fPartsMap [fCurrentPartID];
      
  else
    msrInternalError (
      inputLineNumber,
      "part " +
        fCurrentPartID +
        " is not registered in this visitor's part map");

  if (gGeneralOptions->fTrace)
    cerr <<
      idtr <<
        "--------------------------------------------" <<
        endl <<
      idtr <<
        "Analyzing part \"" << fCurrentPartID << "\" -- start" <<
      endl;

  idtr++;

  fCurrentStaffNumber = 1; // default if there are no <staff> element
  fCurrentVoiceNumber = 1; // default if there are no <voice> element

  fOnGoingRepeat = false;
}

void xml2MsrTranslator::visitEnd (S_part& elt)
{
  if (gGeneralOptions->fTrace)
    cerr <<
      idtr <<
        "Analyzing part \"" << fCurrentPartID << "\" -- end" <<
        endl <<
      idtr <<
        "--------------------------------------------" <<
        endl <<
      endl;

  // all voices were generated from the start, // JMI
  // but some may have remained empty
  fCurrentPart->
    removePartEmptyVoices ();

  idtr--;
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_divisions& elt ) 
{
  fCurrentDivisionsPerQuarterNote = (int)(*elt);
  
  if (fCurrentDivisionsPerQuarterNote <= 0) {
    msrMusicXMLError (
      elt->getInputLineNumber (),
      "divisions per quarter note should be positive");
  }
  
  if (true || gGeneralOptions->fTrace) {
 // JMI if (gGeneralOptions->fTrace) {
    cerr << idtr;
    if (fCurrentDivisionsPerQuarterNote== 1)
      cerr << "There is 1 division";
    else
      cerr <<
        "There are " << fCurrentDivisionsPerQuarterNote <<
        " divisions";
    cerr <<
      " per quater note in part " <<
      fCurrentPart->getPartCombinedName() <<
      endl;
  }

  fCurrentPart->
    setPartDivisionsPerWholeNote (
      fCurrentDivisionsPerQuarterNote * 4);
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_clef& elt )
{ 
  // https://usermanuals.musicxml.com/MusicXML/Content/EL-MusicXML-clef.htm

  // The optional number attribute refers to staff numbers.
  // If absent (0), apply to all part staves.
  
  fCurrentClefStaffNumber =
    elt->getAttributeIntValue("number", 0); 

  fCurrentClefLine = 0;;
  fCurrentClefOctaveChange = 0;
  fCurrentClefSign = "";
}

void xml2MsrTranslator::visitStart ( S_sign& elt )
{
  fCurrentClefSign = elt->getValue();
}

void xml2MsrTranslator::visitStart ( S_line& elt )
{
  fCurrentClefLine = (int)(*elt);
}
  
void xml2MsrTranslator::visitStart ( S_clef_octave_change& elt )
{
  fCurrentClefOctaveChange = (int)(*elt);

  if (fCurrentClefOctaveChange < -2 || fCurrentClefOctaveChange > 2) {
    stringstream s;
    
    s <<
      "octave change \"" << fCurrentClefOctaveChange <<
      "\" is unknown";
    
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());    
  }
}
  
void xml2MsrTranslator::visitEnd ( S_clef& elt ) 
{    
  int inputLineNumber =
    elt->getInputLineNumber ();

  // convert clef to upper case for analysis
  std::transform (
    fCurrentClefSign.begin(),
    fCurrentClefSign.end(),
    fCurrentClefSign.begin(),
    ::toupper);

  msrClef::msrClefKind clefKind;
  
  if (fCurrentClefSign == "G") {
    
    if      (fCurrentClefLine == 2) {
      switch (fCurrentClefOctaveChange) {
        case -2:
          clefKind = msrClef::kTrebleMinus15Clef;
          break;
        case -1:
          clefKind = msrClef::kTrebleMinus8Clef;
          break;
        case 0:
          clefKind = msrClef::kTrebleClef;
          break;
        case +1:
          clefKind = msrClef::kTreblePlus8Clef;
          break;
        case +2:
          clefKind = msrClef::kTreblePlus15Clef;
          break;
        default:
          {
            // should not occur
          }
      } // switch
    }
      
    else if (fCurrentClefLine == 1)
      clefKind = msrClef::kTrebleLine1Clef;
      
    else {
      stringstream s;
      
      s <<
        "'G' clef line \"" << fCurrentClefLine <<
        "\" is unknown";
      
      msrMusicXMLError (
        elt->getInputLineNumber (),
        s.str());    
    }
  }

  else if (fCurrentClefSign == "F") {
    
    if ( fCurrentClefLine == 4 ) {
      switch (fCurrentClefOctaveChange) {
        case -2:
          clefKind = msrClef::kBassMinus15Clef;
          break;
        case -1:
          clefKind = msrClef::kBassMinus8Clef;
          break;
        case 0:
          clefKind = msrClef::kBassClef;
          break;
        case +1:
          clefKind = msrClef::kBassPlus8Clef;
          break;
        case +2:
          clefKind = msrClef::kBassPlus15Clef;
          break;
        default:
          {
            // should not occur
          }
      } // switch
    }
    
    else {
      stringstream s;
      
      s <<
        "'F' clef line \"" << fCurrentClefLine <<
        "\" is unknown";
      
      msrMusicXMLError (
        elt->getInputLineNumber (),
        s.str());    
    }
  }
    
  else if (fCurrentClefSign == "C") {
    
    if      ( fCurrentClefLine == 4 )
      clefKind = msrClef::kTenorClef;
      
    else if ( fCurrentClefLine == 3 )
      clefKind = msrClef::kAltoClef;
      
    else {
      stringstream s;
      
      s <<
        "'C' clef line \"" << fCurrentClefLine <<
        "\" is unknown";
      
      msrMusicXMLError (
        elt->getInputLineNumber (),
        s.str());    
    }
  }

  else if ("TAB") {
    
    switch (fCurrentClefLine) {
      case 4:
        clefKind = msrClef::kTablature4Clef;
        break;
      case 5:
        clefKind = msrClef::kTablature5Clef;
        break;
      case 6:
        clefKind = msrClef::kTablature6Clef;
        break;
      case 7:
        clefKind = msrClef::kTablature7Clef;
        break;
      default:
        {
          stringstream s;
          
          s <<
            "tablature line \"" << fCurrentClefLine <<
            "\" is unknown";
          
          msrMusicXMLError (
            elt->getInputLineNumber (),
            s.str());    
        }
    } // switch
  }

  else if (fCurrentClefSign == "PERCUSSION") {
    clefKind = msrClef::kPercussionClef;
  }

  else if (fCurrentClefSign == "NONE") {
    clefKind = msrClef::k_NoClef;
  }
    
  else {
    // unknown clef sign
    stringstream s;
    
    s <<
      "clef sign \"" << fCurrentClefSign <<
      "\" is unknown";
    
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());    
  }

  // create clef
  S_msrClef
    clef =
      msrClef::create (
        inputLineNumber,
        clefKind);

  // register it in part or staff
  if (fCurrentClefStaffNumber == 0)
    fCurrentPart->
      setPartClef (clef);
    
  else {
    S_msrStaff
      staff =
        createStaffInCurrentPartIfNeeded (
          inputLineNumber, fCurrentClefStaffNumber);
    
    staff->setStaffClef (clef);
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_key& elt )
{
  
  // The optional number attribute refers to staff numbers.
  // If absent (0), apply to all part staves.
  fCurrentKeyStaffNumber =
    elt->getAttributeIntValue ("number", 0);

  fCurrentFifths = 0;
  fCurrentCancel = 0;
  fCurrentMode   = "";
}
  
void xml2MsrTranslator::visitStart ( S_fifths& elt )
  { fCurrentFifths = (int)(*elt); }
  
void xml2MsrTranslator::visitStart ( S_mode& elt )
  { fCurrentMode = elt->getValue(); }

void xml2MsrTranslator::visitStart ( S_cancel& elt )
  { fCurrentCancel = (int)(*elt); }

void xml2MsrTranslator::visitEnd ( S_key& elt ) 
{  
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  // create msrKey
  S_msrKey
    key =
      msrKey::create (
        inputLineNumber,
        fCurrentFifths, fCurrentMode, fCurrentCancel);

  if (fCurrentKeyStaffNumber == 0)
    fCurrentPart->
      setPartKey (key);
    
  else {
    S_msrStaff
      staff =
        createStaffInCurrentPartIfNeeded (
          inputLineNumber, fCurrentKeyStaffNumber);

    staff->setStaffKey (key);
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_time& elt )
{  
  /*
    The optional number attribute refers to staff numbers,
    from top to bottom on the system. If absent, the key
    signature applies to all staves in the part.
  */
  fCurrentTimeStaffNumber =
    elt->getAttributeIntValue ("number", 0);
    
  fCurrentTimeSymbol =
    elt->getAttributeValue ("symbol");
  // time symbol="cut" or "common" JMI
  
  fCurrentTimeSenzaMisura = false;

  fCurrentTimeBeats = 0;
  fCurrentTimeBeatType = 0;
  
  fCurrentTimeSymbol = "";
}

void xml2MsrTranslator::visitStart ( S_beats& elt )
{ fCurrentTimeBeats = (int)(*elt); }
  
void xml2MsrTranslator::visitStart ( S_beat_type& elt )
  { fCurrentTimeBeatType = (int)(*elt); }
 
void xml2MsrTranslator::visitStart ( S_senza_misura& elt )
  { fCurrentTimeSenzaMisura = true; }

void xml2MsrTranslator::visitEnd ( S_time& elt ) 
{  
  int inputLineNumber =
    elt->getInputLineNumber ();

  // create msrTime
  S_msrTime
    time =
      msrTime::create (
        inputLineNumber,
        fCurrentTimeBeats,
        fCurrentTimeBeatType);

  if (fCurrentTimeStaffNumber == 0)
    fCurrentPart->
      setPartTime (time);
    
  else {
    S_msrStaff
      staff =
        createStaffInCurrentPartIfNeeded (
          inputLineNumber, fCurrentTimeStaffNumber);

    staff->setStaffTime (time);
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_transpose& elt )
{
  /*
  https://usermanuals.musicxml.com/MusicXML/Content/EL-MusicXML-transpose.htm

  The optional number attribute refers to staff numbers,
  from top to bottom on the system.
  If absent, the transposition applies to all staves in the part.
  Per-staff transposition is most often used in parts
  that represent multiple instruments.

        <transpose>
          <diatonic>-2</diatonic>
          <chromatic>-3</chromatic>
        </transpose>
  */

  fCurrentTransposeNumber = elt->getAttributeIntValue ("number", 0);
  
  fCurrentTransposeDiatonic  = 0;
  fCurrentTransposeChromatic = 0;
}

void xml2MsrTranslator::visitStart ( S_diatonic& elt )
{ fCurrentTransposeDiatonic = (int)(*elt); }
  
void xml2MsrTranslator::visitStart ( S_chromatic& elt )
  { fCurrentTransposeChromatic = (int)(*elt); }
 
void xml2MsrTranslator::visitEnd ( S_transpose& elt ) 
{  
  int inputLineNumber =
    elt->getInputLineNumber ();

  // create msrTranspose
  S_msrTranspose
    transpose =
      msrTranspose::create (
        inputLineNumber,
        fCurrentTransposeDiatonic,
        fCurrentTransposeChromatic);

  if (fCurrentTransposeNumber == 0)
    fCurrentPart->
      setPartTranspose (transpose);
    
  else {
    S_msrStaff
      staff =
        createStaffInCurrentPartIfNeeded (
          inputLineNumber, fCurrentTransposeNumber);

    staff->setStaffTranspose (transpose);
  }
}

//________________________________________________________________________
void xml2MsrTranslator::visitStart (S_direction& elt)
{
/*
  A direction is a musical indication that is not attached to a specific note.
  Two or more may be combined to indicate starts and stops of wedges, dashes, etc.
   
  By default, a series of direction-type elements and a series
  of child elements of a direction-type within a single direction element
  follow one another in sequence visually.
  
  For a series of direction-type children, non-positional formatting attributes
  are carried over from the previous element by default.

  <direction placement="above">
    <direction-type>
      <words default-y="48" font-size="10.5" font-weight="bold" relative-x="-40" xml:lang="de">Sehr langsam</words>
    </direction-type>
    <staff>1</staff>
    <sound tempo="26"/>
  </direction>

  <direction placement="above" directive="yes">
    <direction-type>
      <words default-y="40" font-size="6.6" font-weight="bold">Moderato</words>
    </direction-type>
    <direction-type>
      <words font-size="6.6" font-weight="normal"> </words>
    </direction-type>
    <direction-type>
      <metronome font-family="EngraverTextT" font-size="5.7" parentheses="yes">
        <beat-unit>quarter</beat-unit>
        <per-minute>85</per-minute>
      </metronome>
    </direction-type>
    <sound tempo="85"/>
  </direction>
*/

  fCurrentDirectionPlacement =
    elt->getAttributeValue ("placement");

  if (fCurrentDirectionPlacement == "above")
    fCurrentWordsPlacementKind = msrWords::kAbove;
    
  else if (fCurrentDirectionPlacement == "below")
    fCurrentWordsPlacementKind = msrWords::kBelow;
    
  else if (fCurrentDirectionPlacement.size ()) {
    stringstream s;
    
    s <<
      "direction placement \"" << fCurrentDirectionPlacement <<
      "\" is unknown";
    
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());    
  }

  fCurrentWordsContents = ""; // there can be several such

  fCurrentWords = 0;
  fCurrentTempo = 0;

  fOnGoingDirection = true;
}

void xml2MsrTranslator::visitStart (S_direction_type& elt)
{
  fOnGoingDirectionType = true;
}

void xml2MsrTranslator::visitEnd (S_direction_type& elt)
{
  fOnGoingDirectionType = false;
}

void xml2MsrTranslator::visitStart (S_octave_shift& elt)
{
/*
      <direction>
        <direction-type>
          <octave-shift default-y="29" size="8" type="down"/>
        </direction-type>
        <offset>-1</offset>
        <staff>1</staff>
      </direction>

      <direction>
        <direction-type>
          <octave-shift size="8" type="stop"/>
        </direction-type>
        <offset>-2</offset>
        <staff>1</staff>
      </direction>
*/
  int    size = elt->getAttributeIntValue ("size", 0);

  if (size != 8 && size != 15) {
    stringstream s;
    
    s <<
      "octave shift size \"" << size <<
      "\"" << "is unknown";
      
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }

  string type = elt->getAttributeValue ("type");

  msrOctaveShift::msrOctaveShiftKind octaveShiftKind;
  
  if      (type == "up")
    octaveShiftKind = msrOctaveShift::kOctaveShiftUp;
    
  else if (type == "down")
    octaveShiftKind = msrOctaveShift::kOctaveShiftDown;
    
  else if (type == "stop")
    octaveShiftKind = msrOctaveShift::kOctaveShiftStop;
    
  else {
    stringstream s;
    
    s <<
      "octave shift type \"" << type <<
      "\"" << "is unknown";
      
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }

  // create an octave shift
  S_msrOctaveShift
    octaveShift =
      msrOctaveShift::create (
        elt->getInputLineNumber (),
        octaveShiftKind,
        size);

  // fetch current voice
  S_msrVoice
    currentVoice =
      registerVoiceInStaffInCurrentPartIfNeeded (
        elt->getInputLineNumber (),
        fCurrentDirectionStaffNumber,
        fCurrentVoiceNumber);

  // append octave shift to the current voice
  currentVoice->
    appendOctaveShiftToVoice (octaveShift);
}

void xml2MsrTranslator::visitStart (S_words& elt)
{
/*
          <words default-y="-73" font-style="italic" relative-x="5">cresc.</words>
*/

  fCurrentWordsContents = elt->getValue ();

  fCurrentFontStyle   = elt->getAttributeValue ("font-style");
  fCurrentFontSize    = elt->getAttributeValue ("font-size");
  fCurrentFontWeight  = elt->getAttributeValue ("font-weight");
  fCurrentFontXMLLang = elt->getAttributeValue ("xml:lanG");

  if (! fCurrentFontXMLLang.size ())
    fCurrentFontXMLLang = "it"; // default value

  if (fCurrentWordsContents.size ()) {
    if (gGeneralOptions->fTrace)
      cerr << idtr <<
        "Creating words \"" << fCurrentWordsContents << "\"" <<
        ", placement = \"" << fCurrentDirectionPlacement << "\"" <<
        endl;

    S_msrWords
      words =
        msrWords::create (
          elt->getInputLineNumber (),
          fCurrentWordsPlacementKind,
          fCurrentWordsContents,
          fCurrentFontStyle,
          fCurrentFontSize,
          fCurrentFontWeight,
          fCurrentFontXMLLang);

    fPendingWords.push_back (words);
  }
}

//________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_metronome& elt )
{
  string parentheses = elt->getAttributeValue("parentheses");
  
  fBeatsData.clear();
  fPerMinute = 0;
  fCurrentBeat.fBeatUnit = "";
  fCurrentBeat.fDots = 0;

  if (parentheses.size()) {
    // cerr << "--> S_metronome, parentheses = " << parentheses << endl;
    
    if (parentheses == "yes")
      fParentheses = true;
      
    else if (parentheses == "no")
      fParentheses = true;
      
    else {
      stringstream s;
      
      s <<
        "parentheses value " << parentheses <<
        " should be 'yes' or 'no'";
      
      msrMusicXMLError (
        elt->getInputLineNumber (),
        s.str());
    }
  }
}
  
void xml2MsrTranslator::visitEnd ( S_metronome& elt )
{
/*
     <direction placement="above">
        <direction-type>
          <metronome default-y="20" font-family="EngraverTextT" font-size="12" halign="left" relative-x="-32">
            <beat-unit>eighth</beat-unit>
            <per-minute>132-144</per-minute>
          </metronome>
        </direction-type>
        <sound tempo="69"/>
      </direction>

*/
 // if (fSkipDirection) return;

  int inputLineNumber =
    elt->getInputLineNumber ();

  // fParentheses ??? JMI
  if (fCurrentBeat.fBeatUnit.size()) { // JMI
    fBeatsData.push_back(fCurrentBeat);
    fCurrentBeat.fBeatUnit = "";
    fCurrentBeat.fDots = 0;
  }
  
  if (fBeatsData.size() != 1) {
    msrMusicXMLWarning (
      inputLineNumber,
      "multiple beats found, but only per-minute tempo is supported");
    return;
  }
  
  if (! fPerMinute) {
    msrMusicXMLWarning (
      inputLineNumber,
      "per-minute not found, only per-minute tempo is supported");
    return;
  }

  msrBeatData b = fBeatsData[0];
  rational         r = 
    NoteType::type2rational(
      NoteType::xml (b.fBeatUnit)), rdot(3,2);
  
  while (b.fDots-- > 0) { // JMI
    r *= rdot;
  }
  r.rationalise ();

  fCurrentTempo =
    msrTempo::create (
      inputLineNumber,
      r.getDenominator(), fPerMinute);

  // fetch current voice
  S_msrVoice
    currentVoice =
      registerVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentDirectionStaffNumber,
        fCurrentVoiceNumber);

  currentVoice->
    appendTempoToVoice (fCurrentTempo);
  
  // JMI if (fCurrentOffset) addDelayed(cmd, fCurrentOffset);
}

void xml2MsrTranslator::visitStart ( S_beat_unit& elt )
{ 
  if (fCurrentBeat.fBeatUnit.size()) {
    fBeatsData.push_back (fCurrentBeat); 
    fCurrentBeat.fBeatUnit = "";
    fCurrentBeat.fDots = 0;
  }
  
  fCurrentBeat.fBeatUnit = elt->getValue();
}

void xml2MsrTranslator::visitStart ( S_beat_unit_dot& elt )
  { fCurrentBeat.fDots++; }
  
void xml2MsrTranslator::visitStart ( S_per_minute& elt )
  { fPerMinute = (int)(*elt); }

void xml2MsrTranslator::visitEnd (S_direction& elt)
{
  if (fCurrentTempo) {
    if (fCurrentWordsContents.size ())
      fCurrentTempo->
        setTempoIndication (fCurrentWordsContents);
  }

  fOnGoingDirection = false;
}

//________________________________________________________________________
void xml2MsrTranslator::visitStart (S_staves& elt)
{
  int stavesNumber = int(*elt);

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTrace) {
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
      " in part " << fCurrentPart->getPartCombinedName() <<
      endl;
  }

  if (stavesNumber > 1) {
    // add n-1 staves to current part since 1 already exists
    int n = 2;
    
    while (n <= stavesNumber) {
      fCurrentPart->
        addStaffToPartByItsNumber (
          inputLineNumber, n);
      n++;
    } // while
  }
}

//________________________________________________________________________
void xml2MsrTranslator::visitStart (S_staff& elt)
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
  fCurrentStaffNumber = int(*elt);

  int inputLineNumber =
    elt->getInputLineNumber ();

  // the staff number should be positive
  if (fCurrentStaffNumber <= 0) {
    stringstream s;

    s <<
      "staff number " << fCurrentStaffNumber <<
      " is not positive";
      
    msrAssert (false, s.str());
  }
  
  S_msrStaff
    staff =
      createStaffInCurrentPartIfNeeded (
        inputLineNumber, fCurrentStaffNumber);

  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
    cerr <<
      idtr <<
        "--> S_staff, fCurrentStaffNumber = " <<
        fCurrentStaffNumber << endl <<
      idtr <<
      "--> S_staff, current staff name  = " <<
        staff->getStaffName() <<
      endl;
  }

  if (fOnGoingForward) {

    fCurrentForwardStaffNumber = fCurrentStaffNumber;

  }
  
  else if (fOnGoingNote) {

    // regular staff indication in note/rest
    fCurrentNoteStaffNumber = fCurrentStaffNumber;

  }
  
  else if (fOnGoingDirection) {

    // regular staff indication in <direction/>, such as <staff/>
    fCurrentDirectionStaffNumber = fCurrentStaffNumber;
    
  }
  
  else if (fOnGoingDirectionType) {

    // JMI ???
    
  }
  
  else {
    
    stringstream s;
    
    s << "staff " << fCurrentStaffNumber << " is out of context";
    
    msrMusicXMLError (
// JMI    msrMusicXMLWarning (
      inputLineNumber,
      s.str());    
  }
}
    
//________________________________________________________________________
void xml2MsrTranslator::visitStart (S_staff_details& elt )
{
  fStaffDetailsStaffNumber =
    elt->getAttributeIntValue ("number", 1);

  // show-frets

  idtr++;

  if (true || gGeneralOptions->fDebug) {
//  if (gGeneralOptions->fDebug) {
    cerr <<
      idtr <<
        "Hangling staff details:" <<
        endl <<
      idtr <<
        setw(32) << "StaffDetailsStaffNumber" << " = " <<
        fStaffDetailsStaffNumber <<
        endl;
  }

  fCurrentStaffTuningLinesNumber = 5; // default

  fCurrentStaffDetailsCapo = 0;
}

void xml2MsrTranslator::visitStart (S_staff_type& elt )
{
  string staffType = elt->getValue ();
  
  if (staffType == "ossia") {
    
    fCurrentStaffTypeKind =
      msrStaff::kOssiaStaffType;
    
  }
  else if (staffType == "cue") {
    
    fCurrentStaffTypeKind =
      msrStaff::kCueStaffType;
    
  }
  else if (staffType == "editorial") {
    
    fCurrentStaffTypeKind =
      msrStaff::kEditorialStaffType;
    
  }
  else if (staffType == "regular") {
    
    fCurrentStaffTypeKind =
      msrStaff::kRegularStaffType;
    
  }
  else if (staffType == "alternate") {
    
    fCurrentStaffTypeKind =
      msrStaff::kAlternateStaffType;
    
  }
else {
  stringstream s;
  
  s << "staff type" << staffType << "unknown";
  
  msrMusicXMLError (
    elt->getInputLineNumber (),
    s.str());      
  }
}

void xml2MsrTranslator::visitStart (S_staff_lines& elt )
{
  fCurrentStaffTuningLinesNumber = (int)(*elt);
}

void xml2MsrTranslator::visitStart (S_staff_tuning& elt )
{
  fCurrentStaffTuningLine =
    elt->getAttributeIntValue ("line", 0);

  fCurrentStaffTuningAlter = 0.0;
  /*
          <staff-tuning line="1">
            <tuning-step>E</tuning-step>
            <tuning-octave>2</tuning-octave>
          </staff-tuning>  */
}
    
void xml2MsrTranslator::visitStart (S_tuning_step& elt )
{
  string tuningStep = elt->getValue();
  
  if (tuningStep.length() != 1) {
    stringstream s;
    
    s <<
      "tuning step value \"" << tuningStep <<
      "\" should be a single letter from A to G";
    
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }

  fCurrentStaffTuningStep = tuningStep [0];
}

void xml2MsrTranslator::visitStart (S_tuning_octave& elt )
{
  fCurrentStaffTuningOctave = (int)(*elt);
}

void xml2MsrTranslator::visitStart (S_tuning_alter& elt )
{
  fCurrentStaffTuningAlter = (float)(*elt);
}

void xml2MsrTranslator::visitEnd (S_staff_tuning& elt )
{
  // fetch relevant staff
  S_msrStaff
    staff =
      createStaffInCurrentPartIfNeeded (
        elt->getInputLineNumber (),
        fStaffDetailsStaffNumber);

  // create the staff tuning
  if (true || gGeneralOptions->fDebug) {
//  if (gGeneralOptions->fDebug) {
    cerr <<
      idtr <<
        setw(30) << "Creating staff tuning:" <<
        endl;

    idtr++;

    cerr <<
      idtr <<
        setw(24) << "fCurrentStaffTuningLine" << " = " <<
        fCurrentStaffTuningLine <<
        endl <<
      idtr <<
        setw(24) << "fCurrentStaffTuningStep" << " = " <<
        fCurrentStaffTuningStep <<
        endl <<
      idtr <<
        setw(24) << "CurrentStaffTuningOctave" << " = " <<
        fCurrentStaffTuningOctave <<
        endl;

    idtr--;
  } 
    
  S_msrStafftuning
    stafftuning =
      msrStafftuning::create (
        elt->getInputLineNumber (),
        fCurrentStaffTuningLine,
        fCurrentStaffTuningStep,
        fCurrentStaffTuningOctave,
        fCurrentStaffTuningAlter);
        
  // add it to the staff
  staff->
    addStafftuningToStaff (stafftuning);
}
    
void xml2MsrTranslator::visitStart (S_capo& elt )
{
  fCurrentStaffDetailsCapo = (int)(*elt);
}

void xml2MsrTranslator::visitStart (S_staff_size& elt )
{
  fCurrentStaffDetailsStaffSize = (int)(*elt);
}

void xml2MsrTranslator::visitEnd (S_staff_details& elt )
{
  if (true || gGeneralOptions->fDebug) {
//  if (gGeneralOptions->fDebug) {
    cerr <<
      idtr <<
        setw(32) << "fStaffDetailsStaffNumber" << " = " <<
        fStaffDetailsStaffNumber <<
        endl <<
      idtr <<
        setw(32) << "fCurrentStaffDetailsCapo" << " = " <<
        fCurrentStaffDetailsCapo <<
        endl <<
      idtr <<
        setw(24) << "fCurrentStaffDetailsStaffSize" << " = " <<
        fCurrentStaffDetailsStaffSize <<
        endl;
  }

  idtr--;
}

//________________________________________________________________________
void xml2MsrTranslator::visitStart (S_voice& elt )
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
  fCurrentVoiceNumber = int(*elt);
  
  int inputLineNumber =
    elt->getInputLineNumber ();

/* JMI
  // the voice number should be in the 1..4 range
  if (fCurrentVoiceNumber < 1 || fCurrentVoiceNumber > 4) {
    stringstream s;

    s <<
      "voice number " << fCurrentVoiceNumber <<
      " is not in the 1..4 range";
      
    msrAssert (false, s.str());
  }
  */
  
  if (fOnGoingForward) {

    fCurrentForwardVoiceNumber = fCurrentVoiceNumber;

    S_msrStaff
      staff =
        createStaffInCurrentPartIfNeeded (
          inputLineNumber, fCurrentForwardVoiceNumber);
  
  //  if (gGeneralOptions->fDebug)
      cerr <<
        idtr <<
          "--> S_voice, fCurrentVoiceNumber         = " <<
          fCurrentVoiceNumber << endl <<
        idtr <<
          "--> S_voice, fCurrentForwardVoiceNumber = " <<
          fCurrentForwardVoiceNumber << endl <<
        idtr <<
          "--> S_voice, current staff name  = " <<
          staff->getStaffName() <<
        endl;
  }
  
  else if (fOnGoingNote) {

    // regular voice indication in note/rest
    fCurrentNoteVoiceNumber = fCurrentVoiceNumber;

    S_msrStaff
      staff =
        createStaffInCurrentPartIfNeeded (
          inputLineNumber, fCurrentNoteStaffNumber);
  
    if (gGeneralOptions->fDebug)
      cerr <<
        idtr <<
          "--> fCurrentNoteVoiceNumber        = " <<
          fCurrentNoteVoiceNumber <<
          endl <<
        idtr <<
          "--> S_voice, fCurrentNoteStaffNumber = " <<
          fCurrentNoteStaffNumber <<
          endl <<
        idtr <<
          "--> S_voice, current staff name  = " <<
          staff->getStaffName() <<
        endl;
/*
    currentVoice = // ???
      registerVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);
  */  
  }
  
  else {
    
    stringstream s;
    
    s << "voice " << fCurrentVoiceNumber << " is out of context";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());
  }
}

//________________________________________________________________________
void xml2MsrTranslator::visitStart (S_backup& elt )
{
/*
 The backup and forward elements are required to coordinate multiple voices in one part, including music on multiple staves. The backup type is generally used to move between voices and staves. Thus the backup element does not include voice or staff elements. Duration values should always be positive, and should not cross measure boundaries or mid-measure changes in the divisions value.
 
      <backup>
        <duration>8</duration>
      </backup>
*/

  handleTupletsPendingOnTupletStack (
    elt->getInputLineNumber ());
  
  fOnGoingBackup = true;
}

void xml2MsrTranslator::visitEnd (S_backup& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Handling 'backup <<< " << fCurrentBackupDuration <<
      " divisions'" <<
      ", line " << inputLineNumber <<
      endl;

  fCurrentPart->
    handleBackup (fCurrentBackupDuration);

  fOnGoingBackup = false;
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_forward& elt )
{
  /*
      <forward>
        <duration>96</duration>
        <voice>1</voice>
        <staff>1</staff>
      </forward>
*/

  // the <staff /> element is present only
  // in case of a staff change
  fCurrentForwardStaffNumber = fCurrentStaffNumber;

  // the <voice /> element is present only
  // in case of a voice change
  fCurrentForwardVoiceNumber = fCurrentVoiceNumber;
  
  handleTupletsPendingOnTupletStack (
    elt->getInputLineNumber ());
  
  fOnGoingForward = true;
}

void xml2MsrTranslator::visitEnd ( S_forward& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  // change staff
  fCurrentStaffNumber = fCurrentForwardStaffNumber;

  S_msrStaff
    staff =
      createStaffInCurrentPartIfNeeded (
        inputLineNumber, fCurrentStaffNumber);

  // change voice
  fCurrentVoiceNumber = fCurrentForwardVoiceNumber;

  S_msrVoice
    currentVoice =
      registerVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Handling 'forward >>> " << fCurrentForwardDuration <<
      "', thus switching to voice \"" << currentVoice->getVoiceName () <<
      "\" in staff \"" << staff->getStaffName () << "\"" <<
      endl;

/* JMI
  currentVoice->
    catchupToMeasureLocation (
      inputLineNumber,
      divisionsPerWholeNote,
      measureLocation);
        */
                    
/* JMI
  for (int i = 0; i < fCurrentForwardDuration; i++) {
    // generate rests for the duration of the forward move
    int restDivisions = 1;
    
    S_msrNote
      rest =
        msrNote::createRest (
            inputLineNumber,
          1, // JMI
          fCurrentStaffNumber,
          fCurrentVoiceNumber);
  
    // set rest's divisions per whole note
    rest->
      setDivisionsPerWholeNote (
        currentVoice-> getDivisionsPerWholeNote ());

    // set its location
    rest->setNoteMeasureLocation (
      currentVoice->getVoiceMeasureLocation ());

    // append it to the current voice
    currentVoice->appendNoteToVoice (rest);
  
    // take it's duration into account
    currentVoice->incrementPositionInMeasure (
      restDivisions);
  } // for
  */
  
  fOnGoingForward = false;
}

//________________________________________________________________________
void xml2MsrTranslator::visitStart (S_tied& elt )
{
// <tied orientation="over" type="start"/>

  string tiedType =
    elt->getAttributeValue ("type");
  
  fCurrentTiedOrientation =
    elt->getAttributeValue ("orientation");

  fCurrentTieKind = msrTie::k_NoTie;
  
  if      (tiedType == "start") {
    
    fCurrentTieKind =
      msrTie::kStartTie;
    
  }
  else if (tiedType == "continue") {
    
    fCurrentTieKind =
      msrTie::kContinueTie;
    
  }
  else if (tiedType == "stop") {
    
    fCurrentTieKind =
      msrTie::kStopTie;
    
  }
  else {

    // inner tied notes may miss the "continue" type:
    // let' complain on slur notes outside of slurs 
    if (! fOnGoingSlur)
      if (tiedType.size()) {
        stringstream s;
        
        s << "tied type" << fCurrentSlurType << "unknown";
        
        msrMusicXMLError (
          elt->getInputLineNumber (),
          s.str());
      }
      
  }

  if (fCurrentTieKind != msrTie::k_NoTie)
    fCurrentTie =
      msrTie::create (
        elt->getInputLineNumber (),
        fCurrentTieKind);
}

//________________________________________________________________________
void xml2MsrTranslator::visitStart (S_slur& elt )
{
/*
Slur types are empty. Most slurs are represented with two elements: one with a start type, and one with a stop type. Slurs can add more elements using a continue type. This is typically used to specify the formatting of cross-system slurs, or to specify the shape of very complex slurs.

      <note default-x="80">
        <pitch>
          <step>F</step>
          <octave>4</octave>
        </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem default-y="5">up</stem>
        <beam number="1">begin</beam>
        <notations>
          <slur number="1" placement="below" type="start"/>
        </notations>
      </note>

http://usermanuals.musicxml.com/MusicXML/Content/EL-MusicXML-notations.htm

*/

  fCurrentSlurNumber =
    elt->getAttributeIntValue ("number", 0);

  fCurrentSlurType =
    elt->getAttributeValue ("type");

  fCurrentSlurPlacement =
    elt->getAttributeValue ("placement");

  if      (fCurrentSlurType == "start") {
    
    fCurrentSlurKind = msrSlur::kStartSlur;
    fOnGoingSlur = true;
    
  }
  else if (fCurrentSlurType == "continue") {
    
    fCurrentSlurKind = msrSlur::kContinueSlur;
    
  }
  else if (fCurrentSlurType == "stop") {
    
    fCurrentSlurKind = msrSlur::kStopSlur;
    fOnGoingSlur = false;
    
  }
  else {

    // inner slur notes may miss the "continue" type:
    // let' complain on slur notes outside of slurs 
    if (! fOnGoingSlur)
      if (fCurrentSlurType.size()) {
        stringstream s;
        
        s << "slur type" << fCurrentSlurType << "unknown";
        
        msrMusicXMLError (
          elt->getInputLineNumber (),
          s.str());
      }
  }

  S_msrSlur
    slur =
      msrSlur::create(
        elt->getInputLineNumber (),
        fCurrentSlurNumber,
        fCurrentSlurKind);
        
  fPendingSlurs.push_back (slur);
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_wedge& elt )
{
  string type = elt->getAttributeValue("type");
  
  msrWedge::msrWedgeKind wedgeKind;

  if      (type == "crescendo") {
    wedgeKind = msrWedge::kCrescendoWedge;
  }
  else if (type == "diminuendo") {
    wedgeKind = msrWedge::kDecrescendoWedge;
  }
  else if (type == "stop") {
    wedgeKind = msrWedge::kStopWedge;
  }
  else {
    if (type.size ()) {
      msrMusicXMLError (
        elt->getInputLineNumber (),
        "unknown wedge type \"" +
          type +
          "\", should be 'crescendo', 'diminuendo' or 'stop'");
    }
  }
  
  S_msrWedge
    wedge =
      msrWedge::create(
        elt->getInputLineNumber (),
        wedgeKind);
        
  fPendingWedges.push_back (wedge);
}
    
//________________________________________________________________________
void xml2MsrTranslator::visitStart (S_lyric& elt )
{ 
  int stanzaNumber =
    elt->getAttributeIntValue ("number", 0);

  if (stanzaNumber < 0) {
    stringstream s;

    s <<
      "lyric number " << fCurrentStanzaNumber <<
      " is not positive";

    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }

  if (stanzaNumber > 0) {
    if (true || gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> setting fCurrentStanzaNumber to " <<
        stanzaNumber <<
        ", line " << elt->getInputLineNumber () <<
        endl;
        
    // register it as current stanza number,
    // that remains set another positive value is met,
    // thus allowing a skip syllable to be generated
    // for notes without lyrics
    fCurrentStanzaNumber = stanzaNumber;
  }
  
  fCurrentStanzaHasText = false;
  fCurrentElision = false;

  fCurrentNoteHasStanza = true;

  fOnGoingLyric = true;
}

void xml2MsrTranslator::visitStart ( S_syllabic& elt )
{
  fCurrentSyllabic = elt->getValue();
  
  if      (fCurrentSyllabic == "single")
    fCurrentSyllableKind = msrSyllable::kSingleSyllable;
  else if (fCurrentSyllabic == "begin")
    fCurrentSyllableKind = msrSyllable::kBeginSyllable;
  else if (fCurrentSyllabic == "middle")
    fCurrentSyllableKind = msrSyllable::kMiddleSyllable;
  else if (fCurrentSyllabic == "end")
    fCurrentSyllableKind = msrSyllable::kEndSyllable;
  else {
    stringstream s;
    
    s << "--> syllabic \"" << fCurrentSyllabic << "\" is unknown";
    
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }
}

void xml2MsrTranslator::visitStart ( S_text& elt ) 
{
  string text = elt->getValue();

  // text may be composed of only spaces, so:
  string dest;
  for_each (
    text.begin(), text.end(), stringSpaceRemover (dest));

  if (fCurrentElision)
    fCurrentText += " " + dest; // append to a list? JMI
  else
    fCurrentText = dest;

  fCurrentStanzaHasText = true;

  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> line " << right << setw(5) << elt->getInputLineNumber () <<
      ", fCurrentStanzaNumber" << " = " << fCurrentStanzaNumber <<
      ", fCurrentSyllabic" << " = " << left << setw(6) << fCurrentSyllabic <<
      ", fCurrentText" << " = |" << fCurrentText << "|" << endl;
}

void xml2MsrTranslator::visitStart ( S_elision& elt ) 
{
  fCurrentElision = true;
}

void xml2MsrTranslator::visitStart ( S_extend& elt ) 
{
  fCurrentExtendType =
    elt->getAttributeValue ("type");

  if (fOnGoingLyric) {
    if      (fCurrentExtendType == "start") {
      
      fCurrentSyllableExtendKind =
        msrSyllable::kStartSyllableExtend;
      
    }
    else if (fCurrentExtendType == "continue") {
      
      fCurrentSyllableExtendKind =
        msrSyllable::kContinueSyllableExtend;
      
    }
    else if (fCurrentExtendType == "stop") {
      
      fCurrentSyllableExtendKind =
        msrSyllable::kStopSyllableExtend;
      
    }
    else if (fCurrentExtendType.size()) {
        stringstream s;
        
        s << "extend type" << fCurrentExtendType << "unknown";
        
        msrMusicXMLError (
          elt->getInputLineNumber (),
          s.str());
    }
    else {
      fCurrentSyllableExtendKind =
        msrSyllable::kStandaloneSyllableExtend;
    }
  }
}

void xml2MsrTranslator::visitEnd ( S_lyric& elt )
{
   int inputLineNumber =
      elt->getInputLineNumber ();

  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
 // JMI if (true) {  
    cerr <<
      endl <<
      idtr <<
        "Handling lyric" <<
        ", line = " << inputLineNumber << ", with:" <<
        endl;

    idtr++;

    cerr <<
      fNoteData <<
        
      idtr <<
        setw(27) << "fCurrentStanzaNumber" << " = " << fCurrentStanzaNumber <<
        endl <<
      idtr <<
        setw(27) << "fCurrentText" << " = \"" << fCurrentText <<
        "\":" << fNoteData.fDivisions << ", " <<
        endl <<
      idtr <<
        setw(27) << "fCurrentElision" << " = " << fCurrentElision <<
        endl <<
      idtr <<
        setw(27) << "fCurrentSyllableExtendKind" << " = " <<
          fCurrentSyllableExtendKind <<
        endl <<
      idtr <<
        setw(27) << "fNoteData.fStepIsARest" << " = " <<
        booleanAsString (fNoteData.fStepIsARest) <<
        endl;

    cerr <<
      idtr <<
        setw(27) << "fCurrentTieKind" << " = " <<
        msrTie::tieKindAsString (fCurrentTieKind) <<
      endl;
        
    cerr <<
      idtr <<
        setw(27) << "fCurrentSlurKind" << " = \"" <<
        msrSlur::slurKindAsString (fCurrentSlurKind) <<
        "\"" <<
        endl;

    cerr <<
      idtr <<
        setw(27) <<
        "fOnGoingSlur" << " = " << fOnGoingSlur <<
        endl <<
      idtr <<
        setw(27) <<
        "fOnGoingSlurHasStanza" << " = " << fOnGoingSlurHasStanza <<
        endl;

    cerr <<
      idtr <<
        setw(27) << "fFirstSyllableInSlurKind" << " = \"" <<
        fFirstSyllableInSlurKind << // JMI->syllableKindAsString () <<
        "\"" << endl;

    cerr <<
      idtr <<
        setw(27) << "fCurrentSyllableKind" << " = \""<<
        fCurrentSyllableKind << // JMI ->syllableKindAsString () <<
        "\"" << endl;
        
    idtr--;
  }

  if      (fCurrentSyllabic == "single")
    fCurrentSyllableKind = msrSyllable::kSingleSyllable;
  else if (fCurrentSyllabic == "begin")
    fCurrentSyllableKind = msrSyllable::kBeginSyllable;
  else if (fCurrentSyllabic == "middle")
    fCurrentSyllableKind = msrSyllable::kMiddleSyllable;
  else if (fCurrentSyllabic == "end")
    fCurrentSyllableKind = msrSyllable::kEndSyllable;
  else
    // no <syllabic /> specified for this note
    fCurrentSyllableKind = msrSyllable::k_NoSyllable;

  // fetch current voice
  S_msrVoice
    currentVoice =
      registerVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentNoteStaffNumber,
        fCurrentVoiceNumber);

  S_msrSyllable
    syllable;

  if (fCurrentSyllableKind != msrSyllable::k_NoSyllable) {
    
 //   string syllableKindAsString; JMI
    
    if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
      /*
      cerr <<
        ", type = \"" << syllableKindAsString << "\"" <<
        ", elision: " << fCurrentElision << 
        " to " << getStanzaName () << endl;
*/
    }

    // create the syllable
    syllable =
      currentVoice->
        addTextSyllableToVoice (
          inputLineNumber,
          fCurrentStanzaNumber,
          fCurrentSyllabic,
          fCurrentSyllableKind,
          fCurrentText,
          fCurrentElision,
          fCurrentSyllableExtendKind,
          fNoteData.fDivisions);

    // the presense of a '<lyric />' ends the effect
    // of an on going syllable extend
    fOnGoingSyllableExtend = false;
    
    if (fOnGoingSlur)
      fOnGoingSlurHasStanza = true;
      
    fCurrentNoteHasStanza = true;
  }
  
  else {

    if (
      fCurrentSlurKind == msrSlur::kStartSlur
        &&
      fCurrentNoteHasStanza) { // JMI
    }
    
    if (fCurrentTieKind != msrTie::k_NoTie) {
      fCurrentSyllableKind = msrSyllable::kTiedSyllable;
      
      syllable =
        currentVoice->
          addTiedSyllableToVoice (
            inputLineNumber,
            fCurrentStanzaNumber,
            fNoteData.fDivisions);
    }
  
    else if (fNoteData.fStepIsARest) {
      fCurrentSyllableKind = msrSyllable::kRestSyllable;

      syllable =
        currentVoice->
          addRestSyllableToVoice (
            inputLineNumber,
            fCurrentStanzaNumber,
            fNoteData.fDivisions);
    }
  
    else if (
      fOnGoingSlurHasStanza
        &&
      ! fCurrentText.size ()) {
      if (fFirstSyllableInSlurKind == msrSyllable::kEndSyllable) {
        fCurrentSyllableKind = msrSyllable::kSlurBeyondEndSyllable;
  
        syllable =
          currentVoice->
            addSlurBeyondEndSyllableToVoice ( 
              inputLineNumber,
              fCurrentStanzaNumber,
              fNoteData.fDivisions);
      }
      else {        
        fCurrentSyllableKind = msrSyllable::kSlurSyllable;
  
        syllable =
          currentVoice->
            addSlurSyllableToVoice ( 
              inputLineNumber,
              fCurrentStanzaNumber,
              fNoteData.fDivisions);
      }
    }
    
    else if (fOnGoingSlur) {
      if (fFirstSyllableInSlurKind == msrSyllable::kEndSyllable) {
        fCurrentSyllableKind = msrSyllable::kSlurBeyondEndSyllable;
  
        syllable =
          currentVoice->
            addSlurBeyondEndSyllableToVoice (
              inputLineNumber,
              fCurrentStanzaNumber,
              fNoteData.fDivisions);
      }
      else {        
        fCurrentSyllableKind = msrSyllable::kSlurSyllable;
  
        syllable =
          currentVoice->
            addSlurSyllableToVoice ( 
              inputLineNumber,
              fCurrentStanzaNumber,
              fNoteData.fDivisions);
      }
    }
    
    else { // JMI
    }
  }

  if (syllable)
    // register syllable in current note's syllables list
    fCurrentNoteSyllables.push_back (syllable);

  fOnGoingLyric = false;
}

//________________________________________________________________________
void xml2MsrTranslator::visitStart (S_measure& elt)
{
/*
  Measures have a required number attribute (going from partwise to timewise,
  measures are grouped via the number).
*/

  int inputLineNumber =
    elt->getInputLineNumber ();

  int measureNumber =
      elt->getAttributeIntValue ("number", -1);

  // Measures with an implicit attribute set to "yes"
  // never display a measure number,
  // regardless of the measure-numbering setting.
  string
    implicit =
      elt->getAttributeValue ("implicit");

  // set part measure number
  fCurrentPart->
    setPartMeasureNumber (
      inputLineNumber, measureNumber);

  // is this measure number in the debug set?
  set<int>::iterator
    it =
      gGeneralOptions->
        fDebugMeasureNumbersSet.find (measureNumber);
        
  if (it != gGeneralOptions->fDebugMeasureNumbersSet.end ()) {
    // yes, activate debug for it
    gGeneralOptions->fSaveDebug = gGeneralOptions->fDebug;
    gGeneralOptions->fSaveDebugDebug = gGeneralOptions->fDebugDebug;
  }

  if (gGeneralOptions->fTrace) {
    cerr <<
      endl <<
      idtr << 
      "<!--=== measure " << measureNumber <<
      ", line = " << inputLineNumber << "===-->" <<
      endl;
  }

}

void xml2MsrTranslator::visitEnd (S_measure& elt)
{
  // finalize last part measure to update master voice
  fCurrentPart->
    finalizeLastMeasureOfPart (
      elt->getInputLineNumber ());

  // restore debug options in case they were set in visitStart()
  gGeneralOptions->fDebug = gGeneralOptions->fSaveDebug;
  gGeneralOptions->fDebugDebug = gGeneralOptions->fSaveDebugDebug;
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_print& elt ) 
{
  const string& newSystem = elt->getAttributeValue ("new-system");
  
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (newSystem.size()) {
    
    if (newSystem == "yes") {
      
      int inputLineNumber =
        elt->getInputLineNumber ();
      
      // create a barnumbercheck command
      if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
        cerr << idtr << 
          "Creating a barnumber check, " <<
          "line = " << inputLineNumber <<
          endl;
      }

      // fetch current voice
      S_msrVoice
        currentVoice =
          registerVoiceInStaffInCurrentPartIfNeeded (
            inputLineNumber,
            fCurrentStaffNumber,
            fCurrentVoiceNumber);

      S_msrBarnumberCheck
        barnumbercheck_ =
          msrBarnumberCheck::create (
            inputLineNumber,
            currentVoice->
              getVoiceMeasureNumber ());
            
      // append it to the voice
// JMI      S_msrElement bnc = barnumbercheck_;
      currentVoice->
        appendBarnumberCheckToVoice (barnumbercheck_);
  
      // create a break command
      if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
        cerr << idtr << 
          "Creating a break, " <<
          "line = " << inputLineNumber << endl;
      }

      S_msrBreak
        break_ =
          msrBreak::create(
            inputLineNumber,
            currentVoice->
              getVoiceMeasureNumber ());
  
      // append it to the voice
      currentVoice->
        appendBreakToVoice (break_);
     }
    
    else if (newSystem == "no") {
      // ignore it
    }
    
    else {
      stringstream s;
  
      s << "unknown 'new-system' value '" << newSystem <<
      "' in '<print />', should be 'yes', 'no' or empty";
      
      msrMusicXMLError (
        inputLineNumber,
        s.str());
    }
  }
}

/*
  http://www.musicxml.com/for-developers/musicxml-dtd/barline-elements/
 
  <barline location="left">
    <bar-style>heavy-light</bar-style>
    <repeat direction="forward"/>
  </barline>
  
  <barline location="right">
    <bar-style>light-heavy</bar-style>
    <repeat direction="backward"/>
  </barline>

http://usermanuals.musicxml.com/MusicXML/Content/EL-MusicXML-repeat.htm

      <barline location="left">
        <bar-style>heavy-light</bar-style>
        <repeat direction="forward" winged="none"/>
      </barline>
      
      <barline location="right">
        <bar-style>light-heavy</bar-style>
        <ending number="1, 2" type="stop"/>
        <repeat direction="backward" winged="none"/>
      </barline>

  Repeat start:
      <barline location="left">
        <bar-style>heavy-light</bar-style>
        <repeat direction="forward"/>
      </barline>

  In the middle of a measure: (MozartTrio.xml)
    <measure number="X1" implicit="yes">
      <barline location="left">
        <bar-style>heavy-light</bar-style>
        <repeat direction="forward"/>
      </barline>
      <note>
        <rest/>
        <duration>6</duration>
        <voice>1</voice>
        <type>quarter</type>
      </note>
    </measure>

  Repeat end:
    implicit at end or part if nothing specified

  In the middle of a measure: (MozartTrio.xml)
    <measure number="12">
      <note>
        <pitch>
          <step>C</step>
          <octave>5</octave>
        </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>down</stem>
      </note>
      <note>
        <rest/>
        <duration>6</duration>
        <voice>1</voice>
        <type>quarter</type>
      </note>
      <barline location="right">
        <bar-style>light-heavy</bar-style>
        <repeat direction="backward"/>
      </barline>
    </measure>
  
  Double bar:
      <barline location="right">
        <bar-style>light-light</bar-style>
      </barline>

  End of part:
      <barline location="right">
        <bar-style>light-light</bar-style>
      </barline>

(Saltarello.xml):
      <barline location="left">
        <ending type="start" number="1"/>
      </barline>

      <barline location="right">
        <bar-style>light-heavy</bar-style>
        <ending type="stop" number="1"/>
        <repeat direction="backward"/>
      </barline>

    Endings refers to multiple (e.g. first and second) endings.
    Typically, the start type is associated with the left
    barline of the first measure in an ending. The stop and
    discontinue types are associated with the right barline of
    the last measure in an ending. Stop is used when the ending
    mark concludes with a downward jog, as is typical for first
    endings. Discontinue is used when there is no downward jog,
    as is typical for second endings that do not conclude a
    piece. The length of the jog can be specified using the
    end-length attribute. The text-x and text-y attributes
    are offsets that specify where the baseline of the start
    of the ending text appears, relative to the start of the
    ending line.

    The number attribute reflects the numeric values of what
    is under the ending line. Single endings such as "1" or
    comma-separated multiple endings such as "1, 2" may be
    used. The ending element text is used when the text
    displayed in the ending is different than what appears in
    the number attribute. The print-object element is used to
    indicate when an ending is present but not printed, as is
    often the case for many parts in a full score.
    
*/

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_barline& elt ) 
{
/*
      <barline location="right">
        <bar-style>light-heavy</bar-style>
        <ending type="stop" number="1"/>
        <repeat direction="backward"/>
      </barline>

      <barline>
        <segno default-y="16" relative-x="0"/>
      </barline>

      <barline>
        <coda default-y="16" relative-x="0"/>
      </barline>
*/
  fCurrentLocation        = "";
  fCurrentStyle           = "";
  fCurrentEndingtype      = "";
  fCurrentEndingNumber    = ""; // may be "1, 2"
  fCurrentRepeatDirection = "";
  fCurrentRepeatWinged    = "";

  fCurrentBarlineHasSegno = false;
  fCurrentBarlineHasCoda  = false;

  fCurrentBarlineLocation        = msrBarline::k_NoLocation;
  fCurrentBarlineStyle           = msrBarline::k_NoStyle;
  fCurrentBarlineEndingType      = msrBarline::k_NoEndingType;
  fCurrentBarlineRepeatDirection = msrBarline::k_NoRepeatDirection;
  fCurrentBarlineRepeatWinged    = msrBarline::k_NoRepeatWinged;

  fCurrentLocation = elt->getAttributeValue ("location");

  fCurrentBarlineLocation =
    msrBarline::kRight; // by default
    
  if       (fCurrentLocation == "left") {
    fCurrentBarlineLocation =
      msrBarline::kLeft;
  }
  else  if (fCurrentLocation == "middle") {
    fCurrentBarlineLocation =
      msrBarline::kMiddle;
  }
  else if  (fCurrentLocation == "right") {
    fCurrentBarlineLocation =
      msrBarline::kRight;
  }
  else {
    stringstream s;
    s << "barline location " << fCurrentLocation << " is unknown";
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }

  fOnGoingBarline = true;
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_bar_style& elt ) 
{
  fCurrentStyle = elt->getValue();

  fCurrentBarlineStyle =
    msrBarline::k_NoStyle;

  if      (fCurrentStyle == "regular") {
    fCurrentBarlineStyle =
      msrBarline::kRegular;
  }
  else if (fCurrentStyle == "dotted") {
    fCurrentBarlineStyle =
      msrBarline::kDotted;
  }
  else if (fCurrentStyle == "dashed") {
    fCurrentBarlineStyle =
      msrBarline::kDashed;
  }
  else if (fCurrentStyle == "heavy") {
    fCurrentBarlineStyle =
      msrBarline::kHeavy;
  }
  else if (fCurrentStyle == "light-light") {
    fCurrentBarlineStyle =
      msrBarline::kLightLight;
  }
  else if (fCurrentStyle == "light-heavy") {
    fCurrentBarlineStyle =
      msrBarline::kLightHeavy;
  }
  else if (fCurrentStyle == "heavy-light") {
    fCurrentBarlineStyle =
      msrBarline::kHeavyLight;
  }
  else if (fCurrentStyle == "heavy-heavy") {
    fCurrentBarlineStyle =
      msrBarline::kHeavyHeavy;
  }
  else if (fCurrentStyle == "tick") {
    fCurrentBarlineStyle =
      msrBarline::kTick;
  }
  else if (fCurrentStyle == "short") {
    fCurrentBarlineStyle =
      msrBarline::kShort;
  }
  else {
    msrMusicXMLError (
      elt->getInputLineNumber (),
      "barline style " + fCurrentStyle + " is unknown");
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_segno& elt ) 
{
  if (fOnGoingDirectionType) {
    int inputLineNumber =
      elt->getInputLineNumber ();
      
    // fetch current voice
    S_msrVoice
      currentVoice =
        registerVoiceInStaffInCurrentPartIfNeeded (
          inputLineNumber,
          fCurrentStaffNumber,
          fCurrentVoiceNumber);
  
    // create the segno
    S_msrSegno
      segno =
        msrSegno::create (
          inputLineNumber);

    // append it to the current voice
    currentVoice->
      appendSegnoToVoice (segno);
  }
  
  else if (fOnGoingBarline) {
    fCurrentBarlineHasSegno = true;
  }
}

void xml2MsrTranslator::visitStart ( S_coda& elt ) 
{
  if (fOnGoingDirectionType) {
    int inputLineNumber =
      elt->getInputLineNumber ();
      
    // fetch current voice
    S_msrVoice
      currentVoice =
        registerVoiceInStaffInCurrentPartIfNeeded (
          inputLineNumber,
          fCurrentStaffNumber,
          fCurrentVoiceNumber);
  
    // create the coda
    S_msrCoda
      coda =
        msrCoda::create (
          inputLineNumber);

    // append it to the current voice
    currentVoice->
      appendCodaToVoice (coda);
  }
  
  else if (fOnGoingBarline) {
    fCurrentBarlineHasCoda = true;
  }
}

void xml2MsrTranslator::visitStart ( S_eyeglasses& elt ) 
{
  if (fOnGoingDirectionType) {
    int inputLineNumber =
      elt->getInputLineNumber ();
      
    // fetch current voice
    S_msrVoice
      currentVoice =
        registerVoiceInStaffInCurrentPartIfNeeded (
          inputLineNumber,
          fCurrentStaffNumber,
          fCurrentVoiceNumber);
  
    // create the eyeglasses
    S_msrEyeglasses
      eyeglasses =
        msrEyeglasses::create (
          inputLineNumber);

    // append it to the current voice
    currentVoice->
      appendEyeglassesToVoice (eyeglasses);
  }
  
  else if (fOnGoingBarline) {
    fCurrentBarlineHasCoda = true;
  }
}

void xml2MsrTranslator::visitStart ( S_pedal& elt ) 
{
  /* start-stop-change-continue */
  /*
    The pedal type represents piano pedal marks. The change and continue types are used when the line attribute is yes. The change type indicates a pedal lift and retake indicated with an inverted V marking. The continue type allows more precise formatting across system breaks and for more complex pedaling lines. The alignment attributes are ignored if the line attribute is yes.
    
    The line attribute is yes if pedal lines are used. The change and continue types are used when the line attribute is yes.
    
    The sign attribute is yes if Ped and signs are used. For MusicXML 2.0 compatibility, the sign attribute is yes by default if the line attribute is no, and is no by default if the line attribute is yes. 
    */

  string
    type = elt->getAttributeValue ("type");
    
  msrPedal::msrPedalType
    pedalType;

  if       (type == "start") {
    pedalType =
      msrPedal::kPedalStart;
  }
  else  if (type == "continue") {
    pedalType =
      msrPedal::kPedalContinue;
  }
  else  if (type == "change") {
    pedalType =
      msrPedal::kPedalChange;
  }
  else  if (type == "stop") {
    pedalType =
      msrPedal::kPedalStop;
  }
  else {
    stringstream s;
    
    s << "pedal type " << type << " is unknown";
    
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }

  string
    line = elt->getAttributeValue ("line");
    
  msrPedal::msrPedalLine
    pedalLine;

  if       (line == "yes") {
    pedalLine =
      msrPedal::kPedalLineYes;
  }
  else  if (line == "no") {
    pedalLine =
      msrPedal::kPedalLineNo;
  }
  else {
    stringstream s;
    
    s << "pedal line " << line << " is unknown";
    
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }

  if (fOnGoingDirectionType) {
    int inputLineNumber =
      elt->getInputLineNumber ();
      
    // fetch current voice
    S_msrVoice
      currentVoice =
        registerVoiceInStaffInCurrentPartIfNeeded (
          inputLineNumber,
          fCurrentStaffNumber,
          fCurrentVoiceNumber);
  
    // create the pedal
    S_msrPedal
      pedal =
        msrPedal::create (
          inputLineNumber,
          pedalType,
          pedalLine);

    // append it to the current voice
    currentVoice->
      appendPedalToVoice (pedal);
  }
  
  else if (fOnGoingBarline) {
    fCurrentBarlineHasCoda = true;
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_ending& elt ) 
{  
  fCurrentEndingNumber =
    elt->getAttributeValue ("number"); // may be "1, 2"

  fCurrentEndingtype   = elt->getAttributeValue ("type");
      
  if       (fCurrentEndingtype == "start") {
    fCurrentBarlineEndingType =
      msrBarline::kStart;
  }
  else  if (fCurrentEndingtype == "stop") {
    fCurrentBarlineEndingType =
      msrBarline::kStop;
  }
  else  if (fCurrentEndingtype == "discontinue") {
    fCurrentBarlineEndingType =
      msrBarline::kDiscontinue;
  }
  else {
    stringstream s;
    
    s << "ending type " << fCurrentEndingtype << " is unknown";
    
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_repeat& elt ) 
{
  fCurrentRepeatDirection =
    elt->getAttributeValue ("direction");

  fCurrentRepeatWinged =
    elt->getAttributeValue ("winged");

  int inputLineNumber =
    elt->getInputLineNumber ();

  fCurrentBarlineRepeatDirection =
    msrBarline::k_NoRepeatDirection;
    
  if       (fCurrentRepeatDirection == "forward") {
    fCurrentBarlineRepeatDirection =
      msrBarline::kForward;
  }
  else  if (fCurrentRepeatDirection == "backward") {
    fCurrentBarlineRepeatDirection =
      msrBarline::kBackward;
  }
  else {
    stringstream s;
    
    s << "repeat direction '" << fCurrentRepeatDirection << "' is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());
  }

  fCurrentBarlineRepeatWinged =
    msrBarline::k_NoRepeatWinged;

  if (fCurrentRepeatWinged.size()) {
    if       (fCurrentRepeatWinged == "none") {
      fCurrentBarlineRepeatWinged =
        msrBarline::kNone;
    }
    else if (fCurrentRepeatWinged == "straight") {
      fCurrentBarlineRepeatWinged =
        msrBarline::kStraight;
    }
    else  if (fCurrentRepeatWinged == "curved") {
      fCurrentBarlineRepeatWinged =
        msrBarline::kCurved;
    }
    else  if (fCurrentRepeatWinged == "doubleStraight") {
      fCurrentBarlineRepeatWinged =
        msrBarline::kDoubleStraight;
    }
    else  if (fCurrentRepeatWinged == "doubleCurved") {
      fCurrentBarlineRepeatWinged =
        msrBarline::kDoubleCurved;
    }
    else {
      stringstream s;
      
      s << "repeat winged '" << fCurrentRepeatWinged << "' is unknown";
      
      msrMusicXMLError (
        inputLineNumber,
        s.str());
    }
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::visitEnd ( S_barline& elt ) 
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  // there may be a barline in a part before any music
  S_msrVoice
    currentVoice =
      registerVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  // create the barline
  S_msrBarline
    barline =
      msrBarline::create (
        inputLineNumber,
        fCurrentBarlineHasSegno,
        fCurrentBarlineHasCoda,
        fCurrentBarlineLocation,
        fCurrentBarlineStyle,
        fCurrentBarlineEndingType,
        fCurrentEndingNumber,
        fCurrentBarlineRepeatDirection,
        fCurrentBarlineRepeatWinged);

  // don't display the barline yet in case of debug,
  // wait until its category is defined
  // to append the barline to the current segment

  // handle the barline according to:
  // http://www.musicxml.com/tutorial/the-midi-compatible-part/repeats/

  /*
    CAUTION:
      The order of the tests in the following is most important!
  */
  
  bool barlineIsAlright = false;

  if (
    fCurrentBarlineLocation == msrBarline::kLeft
      &&
    fCurrentBarlineEndingType == msrBarline::kStart
     &&
    fCurrentBarlineRepeatDirection == msrBarline::kForward) {
    // hooked ending start
    // ------------------------------------------------------
    handleHookedEndingStart (elt, barline);

    barlineIsAlright = true;
  }

  else if (
    fCurrentBarlineLocation == msrBarline::kLeft
      &&
    fCurrentBarlineRepeatDirection == msrBarline::kForward) {
    // repeat start
    // ------------------------------------------------------
    
    /*
    A forward repeat mark is represented by a left barline at the beginning of the measure (following the attributes element, if there is one):
    
      <barline location="left">
        <bar-style>heavy-light</bar-style>
        <repeat direction="forward"/>
      </barline>
    */

    handleRepeatStart (elt, barline);

    barlineIsAlright = true;
  }
  
  else if (
    fCurrentBarlineLocation == msrBarline::kLeft
      &&
    fCurrentBarlineEndingType == msrBarline::kStart) { // no forward
    // hookless ending start
    // ------------------------------------------------------
    handleHooklessEndingStart (elt, barline);

    barlineIsAlright = true;
  }

  else if (
    fCurrentBarlineLocation == msrBarline::kRight
      &&
    fCurrentBarlineEndingType == msrBarline::kStop
      &&
    fCurrentBarlineRepeatDirection == msrBarline::kBackward) {
    // hooked ending end
    // ------------------------------------------------------
    
    /*
    The stop value is used when the end of the ending is marked with a downward hook, as is typical for a first ending. It is usually used together with a backward repeat at the end of a measure:
    
      <barline location="right">
        <bar-style>light-heavy</bar-style>
        <ending type="stop" number="1"/>
        <repeat direction="backward"/>
      </barline>
    */

    handleHookedEndingEnd (elt, barline);
    
    barlineIsAlright = true;
  }

  else if (
    fCurrentBarlineLocation == msrBarline::kRight
      &&
    fCurrentBarlineRepeatDirection == msrBarline::kBackward) {
    // repeat end
    // ------------------------------------------------------
    
    /*
    Similarly, a backward repeat mark is represented by a right barline at the end of the measure:
    
      <barline location="right">
        <bar-style>light-heavy</bar-style>
        <repeat direction="backward"/>
      </barline>
    */
         
    handleRepeatEnd (elt, barline);

    barlineIsAlright = true;
  }

  else if (
    fCurrentBarlineLocation == msrBarline::kRight
      &&
    fCurrentBarlineEndingType == msrBarline::kDiscontinue) {
    // hookless ending end
    // ------------------------------------------------------
    handleHooklessEndingEnd (elt, barline);
        
    barlineIsAlright = true;
  }

  else {

    switch (fCurrentBarlineStyle) {
      
      case msrBarline::kRegular:
      //---------------------------------------
        // don't handle regular barlines specifically,
        // they'll handled later by the software
        // that handles the generated text output
  
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBar);
        
        // append the bar line to the current segment
        currentVoice->
          appendBarlineToVoice (barline);
      
        barlineIsAlright = true;
        break;
        
      case msrBarline::kDotted:
      //---------------------------------------
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBar);
        
        // append the bar line to the current segment
        currentVoice->
          appendBarlineToVoice (barline);
              
        barlineIsAlright = true;
        break;
        
      case msrBarline::kDashed:
      //---------------------------------------    
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBar);
        
        // append the bar line to the current segment
        currentVoice->
          appendBarlineToVoice (barline);
      
        barlineIsAlright = true;
        break;
        
      case msrBarline::kHeavy:
      //---------------------------------------    
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBar);
        
        // append the bar line to the current segment
        currentVoice->
          appendBarlineToVoice (barline);
      
        barlineIsAlright = true;
        break;
        
      case msrBarline::kLightLight:
      //---------------------------------------
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBar);
  
        // append the bar line to the current segment
        currentVoice->
          appendBarlineToVoice (barline);
    
        barlineIsAlright = true;
        break;
        
      case msrBarline::kLightHeavy:
      //---------------------------------------
  
/* JMI
      if (
        fCurrentBarlineLocation == msrBarline::msrBarline::kRight) {
   //       if (gGeneralOptions->fDebug)
            cerr <<
              idtr << "--> input line " << inputLineNumber <<
              endl <<
              idtr <<
              "--> barline, right:" << endl;
            }
*/

        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBar);
        
        // append the bar line to the current segment
        currentVoice->
          appendBarlineToVoice (barline);
          
        barlineIsAlright = true;        
        break;
  
      case msrBarline::kHeavyLight:
      //---------------------------------------
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBar);
        
        // append the bar line to the current segment
        currentVoice->
          appendBarlineToVoice (barline);
          
        barlineIsAlright = true;
        break;
         
      case msrBarline::kHeavyHeavy:
      //---------------------------------------    
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBar);
        
        // append the bar line to the current segment
        currentVoice->
          appendBarlineToVoice (barline);
  
        barlineIsAlright = true;
        break;
        
      case msrBarline::kTick:
      //---------------------------------------
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBar);
        
        // append the bar line to the current segment
        currentVoice->
          appendBarlineToVoice (barline);
  
        barlineIsAlright = true;
        break;
        
      case msrBarline::kShort:
      //---------------------------------------
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBar);
        
        // append the bar line to the current segment
        currentVoice->
          appendBarlineToVoice (barline);
  
        barlineIsAlright = true;
        break;
  
      case msrBarline::k_NoStyle:
      //---------------------------------------
        {
          // no <bar-style> has been found
    /*
          / *
          While repeats can have forward or backward direction, endings can have three different type attributes: start, stop, and discontinue. The start value is used at the beginning of an ending, at the beginning of a measure. A typical first ending starts like this:
          
            <barline location="left">
              <ending type="start" number="1"/>
            </barline>
          * /
          if (
            fCurrentBarlineLocation == msrBarline::msrBarline::kLeft
              &&
            fCurrentBarlineEndingType == msrBarline::kStart) {
          }
    
          else if (
            fCurrentBarlineLocation == msrBarline::msrBarline::kRight
              &&
            fCurrentBarlineEndingType == msrBarline::kStop) {
            / *
            The discontinue value is typically used for the last ending in a set,
            where there is no downward hook to mark the end of an ending:
            
            <barline location="right">
              <ending number="2" type="stop"/>
            </barline>
            * /
     //       if (gGeneralOptions->fDebug)
              cerr <<
                idtr << "--> input line " << inputLineNumber <<
                endl <<
                idtr <<
                "--> barline with right and stop:" << endl <<
                idtr <<
                "    end of an hooked ending" <<
                endl;
      
            // set the barline category
            barline->
              setBarlineCategory (msrBarline::kEndOfAHookedEnding);
            
            // append the bar line to the current segment
            currentVoice->
              appendBarlineToVoice (barline);
  
            barlineIsAlright = true;
          }
          
          else if (
            fCurrentBarlineLocation == msrBarline::msrBarline::kRight
              &&
            fCurrentBarlineEndingType == msrBarline::kDiscontinue) {
          }
        */
        }
    } // switch
  }
  
  // now we can display the barline in case of debug
  if (gGeneralOptions->fDebug) {
    cerr << idtr <<
      "Creating a barline in voice " <<
      currentVoice->getVoiceName () << ":" <<
      endl;
    idtr++;
    cerr << idtr << barline;
    idtr--;
  }

  // has this barline been handled?
  if (! barlineIsAlright) {
    stringstream s;
    
    s << left <<
      "cannot handle a barline containing:" << endl <<
      idtr << "location = " << fCurrentLocation << endl <<
      idtr << "style = " << fCurrentStyle << endl <<
      idtr << "ending type = " << fCurrentEndingtype << endl <<
      idtr << "ending number = " << fCurrentEndingNumber << endl <<
      idtr << "repeat direction = " << fCurrentRepeatDirection << endl <<
      idtr << "repeat winged = " << fCurrentRepeatWinged;
      
    msrMusicXMLError (
      inputLineNumber,
      s.str());
  }

  fOnGoingBarline = false;
}
  
  /*
Repeats and endings are represented by the <repeat> and <ending> elements with a <barline>, as defined in the barline.mod file.

In regular measures, there is no need to include the <barline> element. It is only need to represent repeats, endings, and graphical styles such as double barlines.

A forward repeat mark is represented by a left barline at the beginning of the measure (following the attributes element, if there is one):

  <barline location="left">
    <bar-style>heavy-light</bar-style>
    <repeat direction="forward"/>
  </barline>

The repeat element is what is used for sound generation; the bar-style element only indicates graphic appearance.

Similarly, a backward repeat mark is represented by a right barline at the end of the measure:

  <barline location="right">
    <bar-style>light-heavy</bar-style>
    <repeat direction="backward"/>
  </barline>

While repeats can have forward or backward direction, endings can have three different type attributes: start, stop, and discontinue. The start value is used at the beginning of an ending, at the beginning of a measure. A typical first ending starts like this:

  <barline location="left">
    <ending type="start" number="1"/>
  </barline>

The stop value is used when the end of the ending is marked with a downward hook, as is typical for a first ending. It is usually used together with a backward repeat at the end of a measure:

  <barline location="right">
    <bar-style>light-heavy</bar-style>
    <ending type="stop" number="1"/>
    <repeat direction="backward"/>
  </barline>

The discontinue value is typically used for the last ending in a set, where there is no downward hook to mark the end of an ending:

  <barline location="right">
    <ending type="discontinue" number="2"/>
  </barline>

    */

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_note& elt ) 
{
  //  cerr << "--> xml2MsrTranslator::visitStart ( S_note& elt ) " << endl;

  // initialize note data to a neutral state
  fNoteData.init ();

  // assuming staff number 1, unless S_staff states otherwise afterwards
  fCurrentStaffNumber = 1;

  // assuming voice number 1, unless S_voice states otherwise afterwards
  fCurrentVoiceNumber = 1;

  fCurrentNoteType = "";

  // keep fCurrentStanzaNumber unchanged,
  // for use by notes without lyrics
  
  fCurrentSyllabic = "";
  fCurrentText = "";
  fCurrentSyllableExtendKind =
  msrSyllable::k_NoSyllableExtend;
  
  // assume this note hasn't got any stanzas until S_lyric is met
  fCurrentNoteHasStanza = false;

  fCurrentBeam = 0;

  fCurrentTie = 0;
  fCurrentTiedOrientation = "";
  
  fCurrentSlurNumber = -1;
  fCurrentSlurType = "";
  fCurrentSlurPlacement = "";
  fCurrentSlurKind = msrSlur::k_NoSlur;

  fCurrentNoteStaffNumber = 1; // it may be absent
  fCurrentNoteVoiceNumber = 1; // it may be absent
  
  fOnGoingNote = true;
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_step& elt )
{
  string step = elt->getValue();
  
  if (step.length() != 1) {
    stringstream s;
    
    s <<
      "step value " << step <<
      " should be a single letter from A to G";
      
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }

  fNoteData.fStep = step [0];
}

void xml2MsrTranslator::visitStart ( S_alter& elt)
{
  fNoteData.fAlter = (float)(*elt);
}

void xml2MsrTranslator::visitStart ( S_octave& elt)
{
  fNoteData.fOctave = (int)(*elt);
}

void xml2MsrTranslator::visitStart ( S_duration& elt )
{
  int duration = (int)(*elt);

  if      (fOnGoingBackup) {
  
    fCurrentBackupDuration = duration;

  }
  
  else if (fOnGoingForward) {
  
    fCurrentForwardDuration = duration;
    
  }
  
  else if (fOnGoingNote) {
  
    fNoteData.fDivisions = duration;
    
    // all notes have their fDisplayDivisions
    // set to fNoteData.fDivision,
    // except tuplet member notes
    fNoteData.fDisplayDivisions =
      fNoteData.fDivisions;
  }
  
  else {
    
    stringstream s;
    
    s << "duration " << duration << " is out of context";
    
 // JMI   msrMusicXMLError (s.str());
    msrMusicXMLWarning (
      elt->getInputLineNumber (),
      s.str());
  }
    
//  cerr << "=== xml2MsrTranslator::visitStart ( S_duration& elt ), fCurrentDuration = " << fCurrentDuration << endl; JMI
}

void xml2MsrTranslator::visitStart ( S_dot& elt )
{
  fNoteData.fDotsNumber++;
}
       
void xml2MsrTranslator::visitStart ( S_type& elt )
{
/*
 Type indicates the graphic note type, Valid values (from shortest to longest) are 1024th, 512th, 256th, 128th, 64th, 32nd, 16th, eighth, quarter, half, whole, breve, long, and maxima. The size attribute indicates full, cue, or large size, with full the default for regular notes and cue the default for cue and grace notes.
*/

  fCurrentNoteType = elt->getValue();
}

void xml2MsrTranslator::visitStart ( S_stem& elt )
{
  //         <stem default-y="28.5">up</stem>

  string        stem = elt->getValue();

  msrStem::msrStemKind stemKind;
  
  if      (stem == "up")
    stemKind = msrStem::kStemUp;
    
  else if (stem == "down")
    stemKind = msrStem::kStemDown;
    
  else if (stem == "none")
    stemKind = msrStem::kStemNone;
    
  else if (stem == "double")
    stemKind = msrStem::kStemDouble;
    
  else {
    stringstream s;
    
    s <<
      "stem \"" << fCurrentBeamValue <<
      "\"" << "is unknown";
      
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }

  fCurrentStem =
    msrStem::create (
      elt->getInputLineNumber (),
      stemKind);
}

void xml2MsrTranslator::visitStart ( S_beam& elt )
{
/*
  Each beam in a note is represented with a separate beam element,
  starting with the eighth note beam using a number attribute of 1.
  Note that the beam number does not distinguish sets of beams
  that overlap, as it does for slur and other elements.
*/
  //        <beam number="1">begin</beam>

  fCurrentBeamValue = elt->getValue();

  fCurrentBeamNumber = 
    elt->getAttributeIntValue ("number", 0);

  int inputLineNumber =
    elt->getInputLineNumber ();

  bool beamIsOK = true;
  
  msrBeam::msrBeamKind beamKind;

  if      (fCurrentBeamValue == "begin") {
    beamKind = msrBeam::kBeginBeam;
  }
  else if (fCurrentBeamValue == "continue") {
    beamKind = msrBeam::kContinueBeam;
  }
  else if (fCurrentBeamValue == "end") {
    beamKind = msrBeam::kEndBeam;
  }
  else if (fCurrentBeamValue == "forward hook") {
    beamKind = msrBeam::kForwardHookBeam;
  }
  else if (fCurrentBeamValue == "backward hook") {
    beamKind = msrBeam::kBackwardHookBeam;
  }
  else {
    stringstream s;
    s <<
      "beam \"" << fCurrentBeamValue <<
      "\"" << "is not handled, ignored";
    msrMusicXMLWarning (
      inputLineNumber,
      s.str());

    beamIsOK = false;
  }
    
  if (beamIsOK) {
    fCurrentBeam =
      msrBeam::create (
        inputLineNumber,
        fCurrentBeamNumber,
        beamKind);
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_articulations& elt )
{
}

void xml2MsrTranslator::visitStart ( S_accent& elt )
{
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kAccent);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_breath_mark& elt )
{
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kBreathMark);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_caesura& elt )
{
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kCaesura);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_spiccato& elt )
{
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kSpiccato);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_staccato& elt )
{
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kStaccato);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_staccatissimo& elt )
{
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kStaccatissimo);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_stress& elt )
{
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kStress);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_unstress& elt )
{
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kUnstress);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_detached_legato& elt )
{
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kDetachedLegato);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_strong_accent& elt )
{
  // type : upright inverted  (Binchois20.xml)
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kStrongAccent);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_tenuto& elt )
{
  // type : upright inverted  (Binchois20.xml)
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kTenuto);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_fermata& elt )
{
  // type : upright inverted  (Binchois20.xml)
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kFermata);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_arpeggiate& elt )
{
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kArpeggiato);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_doit& elt )
{
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kDoit);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_falloff& elt )
{
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kFalloff);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_plop& elt )
{
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kPlop);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_scoop& elt )
{
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kScoop);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitEnd ( S_articulations& elt )
{
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_ornaments& elt )
{
}

void xml2MsrTranslator::visitStart ( S_trill_mark& elt )
{
  // type : upright inverted  (Binchois20.xml)
  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kTrillMark);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void xml2MsrTranslator::visitStart ( S_wavy_line& elt )
{
  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kWavyLine);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void xml2MsrTranslator::visitStart ( S_turn& elt )
{
  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kTurn);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void xml2MsrTranslator::visitStart ( S_inverted_turn& elt )
{
  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kInvertedTurn);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void xml2MsrTranslator::visitStart ( S_delayed_turn& elt )
{
  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kDelayedTurn);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void xml2MsrTranslator::visitStart ( S_delayed_inverted_turn& elt )
{
  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kDelayedInvertedTurn);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void xml2MsrTranslator::visitStart ( S_vertical_turn& elt )
{
  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kVerticalTurn);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void xml2MsrTranslator::visitStart ( S_mordent& elt )
{
  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kMordent);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void xml2MsrTranslator::visitStart ( S_inverted_mordent& elt )
{
  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kInvertedMordent);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void xml2MsrTranslator::visitStart ( S_schleifer& elt )
{
  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kSchleifer);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void xml2MsrTranslator::visitStart ( S_shake& elt )
{
  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kShake);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void xml2MsrTranslator::visitStart ( S_accidental_mark& elt )
{
  fCurrentAccidentalMark =
    elt->getValue ();

  msrOrnament::msrOrnamentAccidentalMarkKind
    currentOrnamentAccidentalMarkKind;
    
  if (fCurrentAccidentalMark == "natural")
    currentOrnamentAccidentalMarkKind = msrOrnament::kNatural;
    
  else if (fCurrentAccidentalMark == "sharp")
    currentOrnamentAccidentalMarkKind = msrOrnament::kSharp;
    
  else if (fCurrentAccidentalMark == "flat")
    currentOrnamentAccidentalMarkKind = msrOrnament::kFlat;
    
  else if (fCurrentAccidentalMark.size ()) {
    stringstream s;
    
    s <<
      "accidental mark \"" << fCurrentAccidentalMark <<
      "\" is unknown";
    
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());    
  }

  string
    currentAccidentalMarkPlacement =
      elt->getAttributeValue ("placement");

  fCurrentOrnamentPlacementKind =
    msrOrnament::k_NoPlacementKind;

  if (currentAccidentalMarkPlacement == "above")
    fCurrentOrnamentPlacementKind = msrOrnament::kAbove;
    
  else if (currentAccidentalMarkPlacement == "below")
    fCurrentOrnamentPlacementKind = msrOrnament::kBelow;
    
  else if (currentAccidentalMarkPlacement.size ()) {
    
    stringstream s;
    
    s <<
      "ornament placement \"" << currentAccidentalMarkPlacement <<
      "\" is unknown";
    
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());    
  }

  fCurrentOrnament->
    setOrnamentPlacementKind (
      fCurrentOrnamentPlacementKind);
      
  fCurrentOrnament->
    setOrnamentAccidentalMarkKind (
      currentOrnamentAccidentalMarkKind);
}

void xml2MsrTranslator::visitEnd ( S_ornaments& elt )
{
}


/*
          <ornaments>
            <turn placement="above"/>
            <accidental-mark placement="below">natural</accidental-mark>
          </ornaments>
*/

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
void xml2MsrTranslator::visitStart( S_f& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kF);
        
  fPendingDynamics.push_back(dyn);
}
void xml2MsrTranslator::visitStart( S_ff& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kFF);
        
  fPendingDynamics.push_back(dyn);
}
void xml2MsrTranslator::visitStart( S_fff& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kFFF);
        
  fPendingDynamics.push_back(dyn);
}
void xml2MsrTranslator::visitStart( S_ffff& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kFFFF);
        
  fPendingDynamics.push_back(dyn);
}
void xml2MsrTranslator::visitStart( S_fffff& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kFFFFF);
        
  fPendingDynamics.push_back(dyn);
}
void xml2MsrTranslator::visitStart( S_ffffff& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kFFFFFF);
        
  fPendingDynamics.push_back(dyn);
}

void xml2MsrTranslator::visitStart( S_p& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kP);
        
  fPendingDynamics.push_back(dyn);
}
void xml2MsrTranslator::visitStart( S_pp& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kPP);
        
  fPendingDynamics.push_back(dyn);
}
void xml2MsrTranslator::visitStart( S_ppp& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kPP);
        
  fPendingDynamics.push_back(dyn);
}
void xml2MsrTranslator::visitStart( S_pppp& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kPPPP);
        
  fPendingDynamics.push_back(dyn);
}
void xml2MsrTranslator::visitStart( S_ppppp& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kPPPPP);
        
  fPendingDynamics.push_back(dyn);
}
void xml2MsrTranslator::visitStart( S_pppppp& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kPPPPPP);
        
  fPendingDynamics.push_back(dyn);
}


void xml2MsrTranslator::visitStart( S_mf& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kMF);
        
  fPendingDynamics.push_back(dyn);
}
void xml2MsrTranslator::visitStart( S_mp& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kMP);
        
  fPendingDynamics.push_back(dyn);
}

void xml2MsrTranslator::visitStart( S_fp& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kFP);
        
  fPendingDynamics.push_back(dyn);
}
void xml2MsrTranslator::visitStart( S_fz& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kFZ);
        
  fPendingDynamics.push_back(dyn);
}

void xml2MsrTranslator::visitStart( S_rf& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kRF);
        
  fPendingDynamics.push_back(dyn);
}

void xml2MsrTranslator::visitStart( S_sf& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kSF);
        
  fPendingDynamics.push_back(dyn);
}

void xml2MsrTranslator::visitStart( S_rfz& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kRFZ);
        
  fPendingDynamics.push_back(dyn);
}

void xml2MsrTranslator::visitStart( S_sfz& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kSFZ);
        
  fPendingDynamics.push_back(dyn);
}

void xml2MsrTranslator::visitStart( S_sfp& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kSFP);
        
  fPendingDynamics.push_back(dyn);
}

void xml2MsrTranslator::visitStart( S_sfpp& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kSFPP);
        
  fPendingDynamics.push_back(dyn);
}

void xml2MsrTranslator::visitStart( S_sffz& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kSFFZ);
        
  fPendingDynamics.push_back(dyn);
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_grace& elt )
{
  fNoteData.fNoteIsAGraceNote = true;

  string slash = elt->getAttributeValue ("slash");

  // check part group barline
  if      (slash == "yes")
    fCurrentGraceIsSlashed = true;
    
  else if (slash == "no")
    fCurrentGraceIsSlashed = false;
    
  else {
    if (slash.size ()) {
      msrMusicXMLError (
        elt->getInputLineNumber (),
        "unknown grace slash \"" +
          slash +
          "\", should be 'yes' or 'no'");
    }
  }
}
       
//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_chord& elt)
{
  fNoteData.fNoteBelongsToAChord = true;

  // delay the handling until 'visitEnd ( S_note& elt)',
  // because we don't know yet the voice and staff numbers for sure
  // (they can be specified after <chord/> in the <note/>)
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_time_modification& elt )
{
  // there may be no '<tuplet number="n" type="start" />'
  // in the tuplet notes after the first one,
  // so we detect tuplet notes on '<time-modification>'
  // so we detect tuplet notes on '<time-modification>'
  fNoteData.fNoteBelongsToATuplet = true;
}

void xml2MsrTranslator::visitStart ( S_actual_notes& elt )
{
  fCurrentActualNotes = (int)(*elt);
}

void xml2MsrTranslator::visitStart ( S_normal_notes& elt )
{
  fCurrentNormalNotes = (int)(*elt);
}

void xml2MsrTranslator::visitStart ( S_normal_type& elt )
{
  fCurrentNormalNoteType = elt->getValue();
}

void xml2MsrTranslator::visitStart ( S_tuplet& elt )
{
  fCurrentTupletNumber =
    elt->getAttributeIntValue ("number", 0);
    
  string tupletType =
    elt->getAttributeValue("type");
  
  /* JMI* /
  cerr <<
    "--> xml2MsrTranslator::visitStart ( S_tuplet, fCurrentTupletNumber = " <<
    fCurrentTupletNumber << ", tupletType = " << tupletType <<endl;
 // */
  
  fCurrentTupletKind = msrTuplet::k_NoTuplet;
  
  if      (tupletType == "start")
    fCurrentTupletKind = msrTuplet::kStartTuplet;
  else if (tupletType == "continue")
    fCurrentTupletKind = msrTuplet::kContinueTuplet;
  else if (tupletType == "stop")
    fCurrentTupletKind = msrTuplet::kStopTuplet;
  else {
    stringstream s;
    
    s << "tuplet type " << tupletType << " is unknown";
    
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_rest& elt)
{
  /*
        <note>
        <rest/>
        <duration>24</duration>
        <voice>1</voice>
      </note>
*/
  //  cerr << "--> xml2MsrTranslator::visitStart ( S_rest& elt ) " << endl;
  fNoteData.fStepIsARest = true;
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_display_step& elt)
{
  string displayStep = elt->getValue();
  
  if (displayStep.length() != 1) {
    stringstream s;
    
    s <<
      "sdisplay step value " << displayStep <<
      " should be a single letter from A to G";
      
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }

  fDisplayStep = displayStep[0];
}

void xml2MsrTranslator::visitStart ( S_display_octave& elt)
{
  fDisplayOctave = (int)(*elt);
}

void xml2MsrTranslator::visitEnd ( S_unpitched& elt)
{
/*
        <unpitched>
          <display-step>E</display-step>
          <display-octave>5</display-octave>
        </unpitched>
*/
  fNoteData.fStepIsUnpitched = true;
  fNoteData.fStep = fDisplayStep;
  fNoteData.fOctave = fDisplayOctave;
}

//______________________________________________________________________________
S_msrChord xml2MsrTranslator::createChordFromItsFirstNote (
  S_msrVoice voice,
  S_msrNote firstNote)
{
  int inputLineNumber =
    firstNote->getInputLineNumber ();
    
//  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> creating a chord from its first note " <<
      firstNote <<
      ", line " << inputLineNumber <<
      ", in voice \"" << voice->getVoiceName () << "\"" <<
      endl;

  // firstNote has been registered standalone in the part element sequence,
  // but it is actually the first note of a chord
  
  // create a chord
  S_msrChord
    chord =
      msrChord::create (
        inputLineNumber,
        firstNote->getNoteDivisions ());

  // chord's divisions per whole note is that of its first note
  chord->
    setChordDivisionsPerWholeNote (
      firstNote-> getNoteDivisionsPerWholeNote ());

  // chord's tie kind is that of its first note
  chord->
    setChordTie (
      firstNote->getNoteTie ());
  
  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> adding first note " << firstNote->noteAsString() <<
      ", line " << inputLineNumber <<
      ", to new chord" << endl;
    
  // register firstNote as a member of chord
  chord->
    addNoteToChord (firstNote);

  // copy firstNote's elements if any to the chord
  copyNoteElementsToChord (firstNote, chord);
    
  return chord;
}

//______________________________________________________________________________
void xml2MsrTranslator::copyNoteArticulationsToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's articulations if any from the first note to chord
  
  list<S_msrArticulation>
    noteArticulations =
      note->
        getNoteArticulations ();
                          
  list<S_msrArticulation>::const_iterator i;
  for (
    i=noteArticulations.begin();
    i!=noteArticulations.end();
    i++) {

    // JMI   if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> copying articulation '" <<
        (*i)->articulationKindAsString () <<
        "' from note " << note->noteAsString () <<
        " to chord" <<
        endl;

    chord->addArticulationToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void xml2MsrTranslator::copyNoteOrnamentsToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's ornaments if any from the first note to chord
  
  list<S_msrOrnament>
    noteOrnaments =
      note->
        getNoteOrnaments ();
                          
  list<S_msrOrnament>::const_iterator i;
  for (
    i=noteOrnaments.begin();
    i!=noteOrnaments.end();
    i++) {

    // JMI   if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> copying ornament '" <<
        (*i)->ornamentKindAsString () <<
        "' from note " << note->noteAsString () <<
        " to chord" <<
        endl;

    chord->addOrnamentToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void xml2MsrTranslator::copyNoteDynamicsToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's dynamics if any from the first note to chord
  
  list<S_msrDynamics>
    noteDynamics =
      note->
        getNoteDynamics ();
                          
  list<S_msrDynamics>::const_iterator i;
  for (
    i=noteDynamics.begin();
    i!=noteDynamics.end();
    i++) {

    // JMI   if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> copying dymaics '" <<
        (*i)->dynamicsKindAsString () <<
        "' from note " << note->noteAsString () <<
        " to chord" <<
        endl;

    chord->addDynamicsToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void xml2MsrTranslator::copyNoteWordsToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's words if any from the first note to chord
  
  list<S_msrWords>
    noteWords =
      note->
        getNoteWords ();
                          
  list<S_msrWords>::const_iterator i;
  for (
    i=noteWords.begin();
    i!=noteWords.end();
    i++) {

    // JMI   if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> copying words '" <<
        (*i)->wordsAsString () <<
        "' from note " << note->noteAsString () <<
        " to chord" <<
        endl;

    chord->addWordsToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void xml2MsrTranslator::copyNoteSlursToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's slurs if any from the first note to chord
  
  list<S_msrSlur>
    noteSlurs =
      note->
        getNoteSlurs ();
                          
  list<S_msrSlur>::const_iterator i;
  for (
    i=noteSlurs.begin();
    i!=noteSlurs.end();
    i++) {

    // JMI   if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> copying slurs '" <<
        (*i)->slurKindAsString () <<
        "' from note " << note->noteAsString () <<
        " to chord" <<
        endl;

    chord->addSlurToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void xml2MsrTranslator::copyNoteWedgesToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's wedges if any from the first note to chord
  
  list<S_msrWedge>
    noteWedges =
      note->
        getNoteWedges ();
                          
  list<S_msrWedge>::const_iterator i;
  for (
    i=noteWedges.begin();
    i!=noteWedges.end();
    i++) {

    // JMI   if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> copying wedges '" <<
        (*i)->wedgeKindAsString () <<
        "' from note " << note->noteAsString () <<
        " to chord" <<
        endl;

    chord->addWedgeToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void xml2MsrTranslator::copyNoteElementsToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy newNote's articulations if any to the chord
  copyNoteArticulationsToChord (note, chord);

  // copy newNote's ornaments if any to the chord
  copyNoteOrnamentsToChord (note, chord);

  // copy newNote's dynamics if any to the chord
  copyNoteDynamicsToChord (note, chord);

  // copy newNote's words if any to the chord
  copyNoteWordsToChord (note, chord);

  // copy newNote's slurs if any to the chord
  copyNoteSlursToChord (note, chord);

  // copy newNote's wedges if any to the chord
  copyNoteWedgesToChord (note, chord);
}

//______________________________________________________________________________
/*
void xml2MsrTranslator::moveNoteDynamicsToChord (
  S_msrNote note, S_msrChord chord)
{
  // move note's dynamics if any from the first note to chord

  // don't use a local copy of the dynamics list,
  // but access it directly instead
  
  if (! note->getNoteDynamicsToModify ().empty ()) {
 // JMI   if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> moving dynamics from current note to chord" << endl;
        
    while (! note->getNoteDynamicsToModify ().empty ()) {
      S_msrDynamics
        dyn = note->getNoteDynamicsToModify ().front ();
        
      chord->addDynamicsToChord (dyn);
      note->getNoteDynamicsToModify ().pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::moveNoteWordsToChord (
  S_msrNote note, S_msrChord chord)
{
  // move note's words if any from the first note to chord

  // don't use a local copy of the words list,
  // but access it directly instead
  
  if (! note->getNoteWordsToModify ().empty()) {
 // JMI   if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> moving words from current note to chord" << endl;
        
    while (! note->getNoteWordsToModify ().empty ()) {
      S_msrWords
        wrds = note->getNoteWordsToModify ().front ();
        
      chord->addWordsToChord (wrds);
      note->getNoteWordsToModify ().pop_front ();
      note->getNoteWordsToModify ().pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::moveNoteWedgesToChord (
  S_msrNote note, S_msrChord chord)
{
  // move note's wedges if any from the first note to chord

  // don't use a local copy of the wedges list,
  // but access it directly instead
      
  if (! note->getNoteWedgesToModify ().empty ()) {
 // JMI   if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> moving wedges from current note to chord" << endl;
        
    while (! note->getNoteWedgesToModify ().empty ()) {
      S_msrWedge
        wedge = note->getNoteWedgesToModify ().front ();
        
      chord->addWedgeToChord (wedge);
      note->getNoteWedgesToModify ().pop_front ();
    } // while
  }
}
*/

//______________________________________________________________________________
void xml2MsrTranslator::createTupletWithItsFirstNote (S_msrNote firstNote)
{
  // firstNote is the first tuplet note,
  // and is currently at the end of the voice

  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "xml2MsrTranslator::createTupletWithItsFirstNote " <<
      firstNote <<
      endl;

  // create a tuplet
  S_msrTuplet
    tuplet =
      msrTuplet::create (
        firstNote->getInputLineNumber (),
        fCurrentTupletNumber,
        fCurrentActualNotes,
        fCurrentNormalNotes,
        firstNote->getNotePositionInMeasure ());

  // tuplets's divisions per whole note is that of its first note
  tuplet->
    setTupletDivisionsPerWholeNote (
      firstNote-> getNoteDivisionsPerWholeNote ());
  
  // register it in this visitor
  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "++> pushing tuplet " <<
      tuplet->getTupletActualNotes () <<
      "/" <<
      tuplet->getTupletNormalNotes () <<
      " to tuplets stack" << endl;
  fTupletsStack.push (tuplet);

  // add note as first note of the stack top tuplet
  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "==> adding first note " << firstNote->noteAsString() <<
      " to the " <<
      fTupletsStack.top ()->getTupletActualNotes () <<
       "/" <<
      fTupletsStack.top ()->getTupletNormalNotes () <<
      " tuplet" <<
      endl;
  tuplet->addNoteToTuplet (firstNote);

/* JMI
  // set note display divisions
  firstNote->
    applyTupletMemberDisplayFactor (
      fCurrentActualNotes, fCurrentNormalNotes);
  */


  // keep track of current tuplet in the current voice,
  // in case we learn later by <chord/> in the next note
  // that it is actually the first note of a chord ?? JMI XXL
  if (gGeneralOptions->fDebugDebug) {
    displayLastHandledTupletInVoice (
      "############## Before fLastHandledTupletInVoice");
  }
  
// JMI  fLastHandledTupletInVoice [currentVoice] = tuplet;
  
  if (gGeneralOptions->fDebugDebug) {
    displayLastHandledTupletInVoice (
      "############## After  fLastHandledNoteInVoice");
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::finalizeTuplet (S_msrNote lastNote)
{
  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "xml2MsrTranslator::finalizeTuplet " <<
      lastNote <<
      endl;
      
  // fetch current voice
  S_msrVoice
    currentVoice =
      registerVoiceInStaffInCurrentPartIfNeeded (
        lastNote->getInputLineNumber (),
        fCurrentNoteStaffNumber,
        fCurrentVoiceNumber);

  // get tuplet from top of tuplet stack
  S_msrTuplet
    tuplet =
      fTupletsStack.top ();

/*  // set note display divisions JMI
  note->
    applyTupletMemberDisplayFactor (
      fCurrentActualNotes, fCurrentNormalNotes);
*/

  // add lastNote to the tuplet
//  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "==> adding last note " << lastNote->noteAsString () <<
      " to tuplets stack top " <<
      fTupletsStack.top ()->getTupletActualNotes () <<
       "/" <<
      fTupletsStack.top ()->getTupletNormalNotes () <<
      endl;
  tuplet->addNoteToTuplet (lastNote);

  // pop from the tuplets stack
//  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> popping tuplet " <<
      fTupletsStack.top ()->getTupletActualNotes () <<
       "/" <<
      fTupletsStack.top ()->getTupletNormalNotes () <<
      " from tuplets stack" << endl;
  fTupletsStack.pop ();        

  if (fTupletsStack.size ()) {
    // tuplet is an embedded tuplet
//    if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "=== adding embedded tuplet " <<
      tuplet->getTupletActualNotes () <<
       "/" <<
      tuplet->getTupletNormalNotes () <<
        " to " <<
      fTupletsStack.top ()->getTupletActualNotes () <<
       "/" <<
      fTupletsStack.top ()->getTupletNormalNotes () <<
      " current stack top tuplet" << endl;
    
    fTupletsStack.top ()->
      addTupletToTuplet (tuplet);
  }
  
  else {
    // tup is a top level tuplet
//    if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "=== adding top level tuplet " <<
      tuplet->getTupletActualNotes () <<
       "/" <<
      tuplet->getTupletNormalNotes () <<
      " to voice" <<
      currentVoice->getVoiceName () <<
      endl;
      
    currentVoice->
      appendTupletToVoice (tuplet);
  }  
}          

//______________________________________________________________________________
void xml2MsrTranslator::attachCurrentArticulationsToNote (
  S_msrNote note)
{
  // attach the current articulations if any to the note
  if (! fCurrentArticulations.empty()) {

    if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> attaching current articulations to note " <<
        note->noteAsString () <<
        endl;

    while (! fCurrentArticulations.empty()) {
      S_msrArticulation
        art =
          fCurrentArticulations.front();
          
      if (gGeneralOptions->fDebug)
        cerr << idtr <<
          "--> attaching articulation '" <<
          art->articulationKindAsString () <<
          "' to note " << note->noteAsString () <<
          endl;
  
      note->
        addArticulationToNote (art);
      fCurrentArticulations.pop_front();
    } // while
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::attachCurrentOrnamentsToNote (
  S_msrNote note)
{
  // attach the current ornaments if any to the note
  if (! fCurrentOrnamentsList.empty()) {
    
    if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> attaching current ornaments to note " <<
        note->noteAsString () <<
        endl;

    while (! fCurrentOrnamentsList.empty()) {
      S_msrOrnament
        art =
          fCurrentOrnamentsList.front();
          
      if (gGeneralOptions->fDebug)
        cerr << idtr <<
          "--> attaching ornament '" <<
          art->ornamentKindAsString () <<
          "' to note " << note->noteAsString () <<
          endl;
  
      note->
        addOrnamentToNote (art);
      fCurrentOrnamentsList.pop_front();
    } // while
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::attachCurrentArticulationsToChord (
  S_msrChord chord)
{
  if (! fCurrentArticulations.empty()) {

    if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> attaching current articulations to chord " <<
        chord->chordAsString () <<
        endl;

    list<S_msrArticulation>::const_iterator i;
    for (
      i=fCurrentArticulations.begin();
      i!=fCurrentArticulations.end();
      i++) {
      if (gGeneralOptions->fDebug)
        cerr << idtr <<
          "--> attaching articulation " <<  (*i) << " to chord " <<
          chord <<
          endl;
          
      chord->
        addArticulationToChord ((*i));
      } // for
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::attachCurrentOrnamentsToChord (
  S_msrChord chord)
{
  if (! fCurrentOrnamentsList.empty()) {

    if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> attaching current ornaments to chord " <<
        chord->chordAsString () <<
        endl;

    list<S_msrOrnament>::const_iterator i;
    for (
      i=fCurrentOrnamentsList.begin();
      i!=fCurrentOrnamentsList.end();
      i++) {
      if (gGeneralOptions->fDebug)
        cerr << idtr <<
          "--> attaching ornament " <<  (*i) << " to chord " <<
          chord <<
          endl;
          
      chord->
        addOrnamentToChord ((*i));
      } // for
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::attachPendingDynamicsToNote (
  S_msrNote note)
{
 // attach the pending dynamics if any to the note
  if (! fPendingDynamics.empty()) {
    
    if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> attaching pending dynamics to note " <<
        note->noteAsString () <<
        endl;

    if (fNoteData.fStepIsARest) {
      if (gMsrOptions->fDelayRestsDynamics) {
        cerr << idtr <<
          "--> Delaying dynamics attached to a rest until next note" << endl;
      }
      else {
        msrMusicXMLWarning (
          note->getInputLineNumber (),
          "there is dynamics attached to a rest");
      }
    }
    
    else {
      while (! fPendingDynamics.empty ()) {
        S_msrDynamics
          dyn =
            fPendingDynamics.front ();
            
        note->addDynamicsToNote (dyn);
        fPendingDynamics.pop_front ();
      } // while
    }
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::attachPendingWordsToNote (
  S_msrNote note)
{
  // attach the pending words if any to the note
  if (! fPendingWords.empty ()) {
    
    if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> attaching pending words to note " <<
        note->noteAsString () <<
        endl;

    if (fNoteData.fStepIsARest) {
      if (gMsrOptions->fDelayRestsDynamics) {
        cerr << idtr <<
          "--> Delaying words attached to a rest until next note" << endl;
      }
      else {
        for (
            list<S_msrWords>::const_iterator i = fPendingWords.begin();
            i != fPendingWords.end();
            i++) {
          msrMusicXMLWarning (
            (*i)->getInputLineNumber (),
            "there is words attached to a rest");
        } // for
      }
    }
    
    else {
      while (! fPendingWords.empty ()) {
        S_msrWords
          words =
            fPendingWords.front ();
            
        note->addWordsToNote (words);
        fPendingWords.pop_front ();
      } // while
    }
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::attachPendingSlursToNote (
  S_msrNote note)
{
  // attach the pending slurs if any to the note
  if (! fPendingSlurs.empty ()) {
    
    if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> attaching pending slurs to note " <<
        note->noteAsString () <<
        endl;

    if (fNoteData.fStepIsARest) {
      if (gMsrOptions->fDelayRestsDynamics) {
        cerr << idtr <<
          "--> Delaying slur attached to a rest until next note" << endl;
      }
      else {
        for (
            list<S_msrSlur>::const_iterator i = fPendingSlurs.begin();
            i != fPendingSlurs.end();
            i++) {
          msrMusicXMLWarning (
            (*i)->getInputLineNumber (),
            "there is a slur attached to a rest");
        } // for
      }
    }
    
    else {
      while (! fPendingSlurs.empty ()) {
        S_msrSlur
          slur =
            fPendingSlurs.front ();
            
        note->addSlurToNote (slur);
        fPendingSlurs.pop_front ();
      } // while
    }
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::attachPendingWedgesToNote (
  S_msrNote note)
{
  // attach the pending wedges if any to the note
  if (! fPendingWedges.empty ()) {
    
    if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> attaching pending wedges to note " <<
        note->noteAsString () <<
        endl;

    if (fNoteData.fStepIsARest) {
      if (gMsrOptions->fDelayRestsDynamics) {
        cerr << idtr <<
          "--> Delaying wedge attached to a rest until next note" << endl;
      }
      else {
        for (
            list<S_msrWedge>::const_iterator i = fPendingWedges.begin();
            i != fPendingWedges.end();
            i++) {
          msrMusicXMLWarning (
            (*i)->getInputLineNumber (),
            "there is a wedge attached to a rest");
        } // for
      }
    }
    
    else {
      while (! fPendingWedges.empty ()) {
        S_msrWedge
          wedge =
            fPendingWedges.front ();
            
        note->addWedgeToNote (wedge);
        fPendingWedges.pop_front ();
      } // while
    }
  }
}

void xml2MsrTranslator::attachPendingElementsToNote (
  S_msrNote note)
{
  // attach the pending dynamics, if any, to the note
  attachPendingDynamicsToNote (note);

  // attach the pending words, if any, to the note
  attachPendingWordsToNote (note);

  // attach the pending slurs, if any, to the note
  attachPendingSlursToNote (note);

  // attach the pending wedges, if any, to the note
  attachPendingWedgesToNote (note);
}

//______________________________________________________________________________
/*
void xml2MsrTranslator::attachPendingDynamicsToChord (
  S_msrChord chord)
{
  // attach the pending dynamics if any to the chord
  if (! fPendingDynamics.empty()) {
    while (! fPendingDynamics.empty ()) {
      S_msrDynamics
        dyn =
          fPendingDynamics.front ();
          
      chord->addDynamicsToChord (dyn);
      fPendingDynamics.pop_front ();
    } // while
  }
}
    
void xml2MsrTranslator::attachPendingWordsToChord (
  S_msrChord chord)
{
  // attach the pending words if any to the chord
  if (! fPendingWords.empty()) {
    while (! fPendingWords.empty ()) {
      S_msrWords
        wrds =
          fPendingWords.front ();
          
      chord->addWordsToChord (wrds);
      fPendingWords.pop_front ();
    } // while
  }
}
  
void xml2MsrTranslator::attachPendingWedgesToChord (
  S_msrChord chord)
{
  // attach the pending wedges if any to the chord
  if (! fPendingWedges.empty ()) {
    while (! fPendingWedges.empty ()) {
      S_msrWedge
        wedge =
          fPendingWedges.front ();
          
      chord->addWedgeToChord (wedge);
      fPendingWedges.pop_front ();
    } // while
  }
}
*/

//______________________________________________________________________________
void xml2MsrTranslator::visitEnd ( S_note& elt )
{
  /*
  This is a complex method, due to the fact that
  dynamics, wedges, chords and tuplets
  are not ordered in the same way in MusicXML and LilyPond.

  Staff number is analyzed before voice number
  but occurs after it in the MusicXML tree.
  That's why the treatment below has been postponed until this method
  */

  /*
  Staff assignment is only needed for music notated on multiple staves.
  Used by both notes and directions.
  Staff values are numbers, with 1 referring to the top-most staff in a part.
  */
  
  int inputLineNumber =
    elt->getInputLineNumber ();

  // fetch current voice
  S_msrVoice
    currentVoice =
      registerVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentNoteStaffNumber,
        fCurrentNoteVoiceNumber);

  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebugDebug) {
    cerr <<
      endl <<
      idtr <<
        "==> BEFORE visitEnd (S_note&)" <<
        ", line " << inputLineNumber <<
        " we have:" <<
        endl;

    idtr++;

    cerr <<
      idtr <<
        setw(27) << "--> fCurrentNoteStaffNumber" << " = " <<
        fCurrentNoteStaffNumber <<
        endl <<
      idtr <<
        setw(27) << "--> fCurrentNoteVoiceNumber" << " = " <<
        fCurrentNoteVoiceNumber <<
        endl <<
      idtr <<
        setw(27) << "--> current voice" << " = \"" <<
        currentVoice->getVoiceName () << "\"" <<
      endl;

    idtr--;

    cerr <<
      idtr <<
        "<==" <<
        endl << endl;
  }

  int divisionsPerWholeNote =
    fCurrentDivisionsPerQuarterNote * 4;

  // store voice and staff numbers in MusicXML note data
  fNoteData.fStaffNumber = fCurrentStaffNumber;
  fNoteData.fVoiceNumber = fCurrentVoiceNumber;

  // set current voices' 'notes divisions per whole note
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "fNoteData.fDivisions = " << 
      fNoteData.fDivisions << ", " << 
      "divisionsPerWholeNote = " <<
      divisionsPerWholeNote << endl;
      
  currentVoice->
    setVoiceDivisionsPerWholeNote (
      divisionsPerWholeNote);

  // register current note type
  fNoteData.fType =
    fCurrentNoteType;

  if (fNoteData.fType.size ()) {
    if (fNoteData.fNoteIsAGraceNote) {
      // set current grace note divisions
      string errorMessage;
      
      fNoteData.fDivisions =
        noteTypeAsDivisions (
          fCurrentNoteType,
          divisionsPerWholeNote,
          errorMessage,
          false); // 'true' to debug it
  
      if (errorMessage.size ())
        msrMusicXMLError (
          inputLineNumber,
          errorMessage);
  
      fNoteData.fDisplayDivisions =
        fNoteData.fDivisions;
    }
  }

  // create the (new) note
  S_msrNote
    newNote =
      msrNote::createFromNoteData (
        inputLineNumber,
        fNoteData);

  // set note's divisions per whole note
  newNote->
    setNoteDivisionsPerWholeNote (
      divisionsPerWholeNote);

  // set its tie if any
  if (fCurrentTie) {
    newNote->
      setNoteTie (fCurrentTie);
  }
  
  // set its stem if any
  if (fCurrentStem)
    newNote->
      setNoteStem (fCurrentStem);

  // add its beam if any
  if (fCurrentBeam)
    newNote->
      addBeamToNote (fCurrentBeam);

  // attach the articulations if any to the note
  attachCurrentArticulationsToNote (newNote);

  // attach the ornaments if any to the note
  attachCurrentOrnamentsToNote (newNote);

  /*
  A rest can be standalone or belong to a tuplet

  A note can be standalone or a member of a chord
  which can belong to a tuplet,
  */

  // are the display divisions different than the duration?
  if (fNoteData.fNoteBelongsToATuplet)
    // set tuplet member note display divisions
    newNote->
      applyTupletMemberDisplayFactor (
        fCurrentActualNotes, fCurrentNormalNotes);

  // handle note
  if (fNoteData.fNoteBelongsToAChord && fNoteData.fNoteBelongsToATuplet) {
    
    // note is the second, third, ..., member of a chord
    // that is a member of a tuplet
    handleNoteBelongingToAChord (newNote);
  }
  
  else if (fNoteData.fNoteBelongsToAChord) {
    
    // note is the second, third, ..., member of a chord
    // whose first member is 'fLastHandledNoteInVoice [currentVoice]'
    handleNoteBelongingToAChord (newNote);
  }
  
  else if (fNoteData.fNoteBelongsToATuplet) {
    
    // note/rest is the first, second, third, ..., member of a tuplet
    handleNoteBelongingToATuplet (newNote);
  }
  
  else {
    
    // note/rest is standalone or a member of grace notes
    handleStandaloneOrGraceNoteOrRest (newNote);
  }

 // JMI if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebugDebug) {
  if (gGeneralOptions->fDebugDebug) {
    cerr <<
      endl <<
      idtr <<
        "==> AFTER visitEnd (S_note&) " <<
        newNote->noteAsString () <<
        ", line " << inputLineNumber <<
        " we have:" <<
        endl <<
      idtr << idtr <<
        setw(27) << "--> fCurrentNoteStaffNumber" << " = " <<
        fCurrentNoteStaffNumber <<
        endl <<
      idtr << idtr <<
        setw(27) << "--> fCurrentNoteVoiceNumber" << " = " <<
        fCurrentNoteVoiceNumber <<
        endl <<
      idtr << idtr <<
        setw(27) << "--> current voice" << " = \"" <<
        currentVoice->getVoiceName () << "\"" <<
        endl <<
        "<==" <<
        endl <<
      endl;
  }

  if (! fNoteData.fNoteBelongsToAChord) {
    if (fOnGoingNote) {
      // this is the first note after the chord

      // forget about this chord
      fCurrentChord = 0;
      fOnGoingChord = false;
    }
  }

  // keep track of current note in the current voice,
  // in case we learn later by <chord/> in the next note
  // that it is actually the first note of a chord
  if (gGeneralOptions->fDebugDebug) {
    displayLastHandledNoteInVoice (
      "############## Before fLastHandledNoteInVoice");
  }
  
  fLastHandledNoteInVoice [currentVoice] = newNote;
  
  if (gGeneralOptions->fDebugDebug) {
    displayLastHandledNoteInVoice (
      "############## After  fLastHandledNoteInVoice");
  }
    
  fOnGoingNote = false;
}

//______________________________________________________________________________
void xml2MsrTranslator::handleNoteBelongingToAChord (
  S_msrNote newNote)
{
/*
  The chord element indicates that this note is an additional
  chord tone with the preceding note. The duration of this
  note can be no longer than the preceding note. In MuseData,
  a missing duration indicates the same length as the previous
  note, but the MusicXML format requires a duration for chord
  notes too.
*/

  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "xml2MsrTranslator::handleNoteBelongingToAChord " <<
      newNote <<
      endl;

  int inputLineNumber =
    newNote->getInputLineNumber ();
    
  if (fNoteData.fStepIsARest)
    msrMusicXMLError (
      inputLineNumber,
      "a rest cannot belong to a chord");

  // should a chord be created?
  if (! fOnGoingChord) {
    // this is the second note of the chord to be created,
    // fLastHandledNote being the first one

    // fetch current voice
    S_msrVoice
      currentVoice =
        registerVoiceInStaffInCurrentPartIfNeeded (
          inputLineNumber,
          fCurrentNoteStaffNumber,
          fCurrentVoiceNumber);

    // fetch last handled note for this voice
    S_msrNote
      lastHandledNoteInVoice =
        fLastHandledNoteInVoice [currentVoice];

    if (! lastHandledNoteInVoice) {
      stringstream s;

      s <<
        "handleNoteBelongingToAChord:" <<
        endl <<
        idtr <<
          "lastHandledNoteInVoice is null on " <<
          newNote->noteAsString ();
        
      msrInternalError (
        inputLineNumber,
        s.str());
    }
        
    // create the chord from its first note
    fCurrentChord =
      createChordFromItsFirstNote (
        currentVoice,
        lastHandledNoteInVoice);

    // remove last handled (previous current) note from the current voice
//    if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> removing last element" <<
        ", line " << inputLineNumber <<
        ", from voice \"" << currentVoice->getVoiceName () << "\"" <<
        endl;

    if (false)
      cerr <<
        endl << endl <<
        "&&&&&&&&&&&&&&&&&& currentVoice (" <<
        currentVoice->getVoiceName () <<
        ") contents &&&&&&&&&&&&&&&&&&" <<
        endl <<
        currentVoice <<
        endl << endl;

    // the last element in the voice is a note or tuplet
    S_msrElement
      lastElementFromVoice =
        currentVoice->
          removeLastElementFromVoice (inputLineNumber);

    bool lastElementFromVoiceIsANote = // JMI
      dynamic_cast<S_msrNote *> (&(*lastElementFromVoice)) != 0;

    bool lastElementFromVoiceIsATuplet = // JMI
      dynamic_cast<S_msrTuplet *> (&(*lastElementFromVoice)) != 0;

    if (lastElementFromVoice == lastHandledNoteInVoice) {
    }
    
    if (
      S_msrNote*
        lastNoteFromVoicePtr =
          dynamic_cast<S_msrNote *> (&(*lastElementFromVoice))) {
                  
//        if (gGeneralOptions->fDebugDebug)
        cerr << idtr <<
          "==> lastElementFromVoice is note " <<
          (*lastNoteFromVoicePtr)->noteAsString () <<
          endl;
          
      // register note as a member of fCurrentChord
    // JMI  if (gGeneralOptions->fDebug)
        cerr << idtr <<
          "--> registering " <<
          newNote->noteAsString () <<
          ", line " << inputLineNumber <<
          " as a new member of current chord" <<
          endl;
          
      fCurrentChord->
        addNoteToChord (newNote);
    }

    else if (
      S_msrTuplet*
        lastTupletFromVoicePtr =
          dynamic_cast<S_msrTuplet *> (&(*lastElementFromVoice))) {
                  
//        if (gGeneralOptions->fDebugDebug)
        cerr << idtr <<
          "==> lastElementFromVoice is tuplet " <<
          (*lastTupletFromVoicePtr)->tupletAsString () <<
          endl;
          
      // register fCurrentChord as a member of tuplet
    // JMI  if (gGeneralOptions->fDebug)
        cerr << idtr <<
          "--> registering " <<
          (*lastTupletFromVoicePtr)->tupletAsString () <<
          ", line " << inputLineNumber <<
          " as a new member of current chord" <<
          endl;
          
      fCurrentChord->
        addNoteToChord (newNote);

      // add current to current tuplet
      (*lastTupletFromVoicePtr)->
        addChordToTuplet (fCurrentChord);
    }

    else {
      stringstream s;
  
      s <<
        "last element just removed from voice is:" <<
        endl <<
        lastElementFromVoice << endl <<
        " and it is not a note nor a tuplet:" <<
        endl <<
        lastHandledNoteInVoice <<
        endl <<
        endl;
        
      msrInternalError (
        inputLineNumber,
        s.str());
    }
  
    // add fCurrentChord to the voice instead
    if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> appending chord " << fCurrentChord <<
        " to current voice \"" << "\"" <<
        endl;
        
    currentVoice->
      appendChordToVoice (fCurrentChord);

    // account for chord being built
    fOnGoingChord = true;
  }

  // set new note kind as a chord member
  newNote->
    setNoteKind (msrNote::kChordMemberNote);

  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> adding new note " <<
      newNote->noteAsString() <<
      " to current chord" << endl;

 // JMI   cerr << "###### fOnGoingChord = " << fOnGoingChord << endl;
    
  // copy newNote's elements if any to the chord
  copyNoteElementsToChord (newNote, fCurrentChord);
}

//______________________________________________________________________________
void xml2MsrTranslator::handleNoteBelongingToATuplet (
  S_msrNote note)
{
  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "xml2MsrTranslator::handleNoteBelongingToATuplet " <<
      note <<
      endl;

  int inputLineNumber =
    note->getInputLineNumber ();
    
  // register note as a tuplet member
  note->
    setNoteKind (msrNote::kTupletMemberNote);

  // attach the pending elements, if any, to the note
  attachPendingElementsToNote (note);

  switch (fCurrentTupletKind) {
    case msrTuplet::kStartTuplet:
      {
        createTupletWithItsFirstNote (note);
      
        // swith to continuation mode
        // this is handy in case the forthcoming tuplet members
        // are not explictly of the "continue" type
        fCurrentTupletKind = msrTuplet::kContinueTuplet;
      }
      break;

    case msrTuplet::kContinueTuplet:
      {
        if (fTupletsStack.size ()) {
          S_msrTuplet
            currentTuplet =
              fTupletsStack.top ();
              
        // populate the tuplet at the top of the stack
//        if (gGeneralOptions->fDebug)
          cerr << idtr <<
            "--> adding note " << note <<
            " to stack top tuplet " <<
            currentTuplet->getTupletActualNotes () <<
             "/" <<
            currentTuplet->getTupletNormalNotes () <<
            ", line " << inputLineNumber <<
            endl;

        fTupletsStack.top()->
          addNoteToTuplet (note);
/*
        // set note display divisions
        note->
          applyTupletMemberDisplayFactor (
            fTupletsStack.top ()->getTupletActualNotes (),
            fTupletsStack.top ()->getTupletNormalNotes ());
*/
        }
        else {
          stringstream s;

          s <<
            "handleNoteBelongingToATuplet():" <<
            endl <<
            "tuplet member note " << note->noteAsString () <<
            "cannot be added, tuplets stack is empty";

          msrInternalError (
            inputLineNumber,
            s.str());
        }
      }
      break;

    case msrTuplet::kStopTuplet:
      {
        finalizeTuplet (note);
      }
      break;

    case msrTuplet::k_NoTuplet:
      break;
  } // switch
}

//______________________________________________________________________________
void xml2MsrTranslator::handleStandaloneOrGraceNoteOrRest (
  S_msrNote newNote)
{
  int inputLineNumber =
    newNote->getInputLineNumber ();
    
  // fetch current voice
  S_msrVoice
    currentVoice =
      registerVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentNoteStaffNumber,
        fCurrentVoiceNumber);
    
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {    
    cerr <<
      idtr <<
        "--> handleStandaloneOrGraceNoteOrRest:" <<
        endl;

    idtr++;

    cerr <<
      idtr <<
        "--> in voice \"" <<
        currentVoice->getVoiceName () << "\"" <<
        endl <<
      idtr <<
        setw(31) << "--> inputLineNumber" << " = " <<
        inputLineNumber <<
        endl <<
      idtr <<
        setw(31) << "--> fNoteData.fNoteIsAGraceNote" << " = " <<
        booleanAsString (fNoteData.fNoteIsAGraceNote) <<
        endl <<
      idtr <<
        setw(31) << "--> fCurrentGracenotes" << " = ";
        
    if (fCurrentGracenotes)
      cerr << fCurrentGracenotes;
    else
      cerr << "NULL"; // JMI

    cerr <<
      endl;

    idtr--;
  }

  if (fNoteData.fNoteIsAGraceNote) {
    newNote->
      setNoteKind (msrNote::kGraceNote);
  
    if (! fCurrentGracenotes) {
      // this is the first grace note in grace notes

      if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
        cerr <<  idtr <<
          "--> creating grace notes for note " <<
          newNote->noteAsString () <<
          " in voice \"" <<
          currentVoice->getVoiceName () << "\"" <<
          endl;
      }

      // create grace notes
      fCurrentGracenotes =
        msrGracenotes::create (
          inputLineNumber,
          fCurrentGraceIsSlashed,
          currentVoice);

      // append it to the current voice
      currentVoice->
        appendGracenotesToVoice (
          fCurrentGracenotes);
    }

    // append newNote to the current grace notes
    if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
      cerr <<  idtr <<
        "--> appending note " <<
        newNote->noteAsString () <<
        " to the grace notes in voice \"" <<
        currentVoice->getVoiceName () << "\"" <<
        endl;
    }

    // attach the pending elements, if any, to newNote
    attachPendingElementsToNote (newNote);

    fCurrentGracenotes->
      appendNoteToGracenotes (newNote);
  }

  else {
    // standalone note or rest

    if (fCurrentGracenotes)
      // this is the first note after the grace notes,
      // forget about the latter
      fCurrentGracenotes = 0;

    // register note/rest kind
    if (fNoteData.fStepIsARest)
      newNote->
        setNoteKind (msrNote::kRestNote);
    else
      newNote->
        setNoteKind (msrNote::kStandaloneNote);
  
    // attach the pending elements, if any, to the note
    attachPendingElementsToNote (newNote);
  
    // append newNote to the current voice
    if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebugDebug) {
      cerr <<  idtr <<
        "--> adding standalone " <<
        newNote->noteAsString () <<
        ", line " << newNote->getInputLineNumber () <<
        ", to voice \"" <<
        currentVoice->getVoiceName () << "\"" <<
        endl;
    }
    
    currentVoice->
      appendNoteToVoice (newNote);

    if (false) // XXL, syllable sans fSyllableNote assigne
    /*
xml2MsrTranslator.cpp:4249
   switch (fSyllableKind) {
    case kSingleSyllable:
      s << left <<
        setw(15) << "single" << ":" << fSyllableDivisions <<
        ", line " << right << setw(5) << fInputLineNumber <<
        ", " << fSyllableNote->notePitchAsString () <<
        ":" << fSyllableNote->noteDivisionsAsMSRString () <<
     */
      cerr <<
        endl << endl <<
        "&&&&&&&&&&&&&&&&&& currentVoice (" <<
        currentVoice->getVoiceName () <<
        ") contents &&&&&&&&&&&&&&&&&&" <<
        endl <<
        currentVoice <<
        endl << endl;
  }

  // handle the pending tuplets
  handleTupletsPendingOnTupletStack (
    inputLineNumber);

  // lyric has to be handled in all cases
  // in case they are empty at the beginning of the voice
  handleLyric (newNote);

  // take care of slurs JMI ???
  if (fCurrentSlurKind == msrSlur::kStartSlur)
    fFirstSyllableInSlurKind = fCurrentSyllableKind;
    
  if (fCurrentSlurKind == msrSlur::kStopSlur)
    fFirstSyllableInSlurKind = msrSyllable::k_NoSyllable;

  // account for chord not being built
  fOnGoingChord = false;
}

//______________________________________________________________________________
void xml2MsrTranslator::handleTupletsPendingOnTupletStack (
  int inputLineNumber)
{
  // fetch current voice
  S_msrVoice
    currentVoice =
      registerVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentNoteStaffNumber,
        fCurrentVoiceNumber);

  // handle tuplets pending on the tuplet stack
  while (fTupletsStack.size ()) {
    S_msrTuplet
      pendingTuplet =
        fTupletsStack.top ();
        
    // pop it from the tuplets stack

//  if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> popping tuplet " <<
        pendingTuplet->getTupletActualNotes () <<
         "/" <<
        pendingTuplet->getTupletNormalNotes () <<
        " from tuplets stack" << endl;
      fTupletsStack.pop ();        

    if (fTupletsStack.size ()) {
      // tuplet is an embedded tuplet
  //    if (gGeneralOptions->fDebug)
        cerr << idtr <<
          "=== adding embedded tuplet " <<
        pendingTuplet->getTupletActualNotes () <<
         "/" <<
        pendingTuplet->getTupletNormalNotes () <<
          " to " <<
        fTupletsStack.top ()->getTupletActualNotes () <<
         "/" <<
        fTupletsStack.top ()->getTupletNormalNotes () <<
        " current stack top tuplet" << endl;
      
      fTupletsStack.top ()->
        addTupletToTuplet (pendingTuplet);
    }
    else {
      // tup is a top level tuplet
  //    if (gGeneralOptions->fDebug)
        cerr << idtr <<
          "=== adding top level tuplet " <<
        pendingTuplet->getTupletActualNotes () <<
         "/" <<
        pendingTuplet->getTupletNormalNotes () <<
        " to voice" <<
        currentVoice->getVoiceName () <<
        endl;
        
      currentVoice->
        appendTupletToVoice (pendingTuplet);
    }  
  } // while
}

void xml2MsrTranslator::displayLastHandledNoteInVoice (string header)
{
  cerr <<
    endl <<
    idtr << header << ", fLastHandledNoteInVoice contains:";

  if (! fLastHandledNoteInVoice.size ()) {
    cerr <<
      " none" <<
      endl;
  }
  
  else {
    map<S_msrVoice, S_msrNote>::const_iterator
      iBegin = fLastHandledNoteInVoice.begin(),
      iEnd   = fLastHandledNoteInVoice.end(),
      i      = iBegin;
      
    cerr << endl;
    
    idtr++;
    for ( ; ; ) {
      cerr << idtr <<
        "\"" << (*i).first->getVoiceName () <<
        "\" ----> " << (*i).second->noteAsString ();
      if (++i == iEnd) break;
      cerr << endl;
    } // for
    cerr <<
      endl;
    idtr--;
  }

  cerr <<
    endl;
}

void xml2MsrTranslator::displayLastHandledTupletInVoice (string header)
{
  cerr <<
    endl <<
    idtr << header << ", fLastHandledTupletInVoice contains:";

  if (! fLastHandledTupletInVoice.size ()) {
    cerr <<
      " none" <<
      endl;
  }
  
  else {
    map<S_msrVoice, S_msrTuplet>::const_iterator
      iBegin = fLastHandledTupletInVoice.begin(),
      iEnd   = fLastHandledTupletInVoice.end(),
      i      = iBegin;
      
    cerr << endl;
    
    idtr++;
    for ( ; ; ) {
      cerr << idtr <<
        "\"" << (*i).first->getVoiceName () <<
        "\" ----> " << (*i).second->tupletAsString ();
      if (++i == iEnd) break;
      cerr << endl;
    } // for
    cerr <<
      endl;
    idtr--;
  }

  cerr <<
    endl;
}

//______________________________________________________________________________
void xml2MsrTranslator::handleLyric (S_msrNote newNote)
{
  if (fCurrentNoteSyllables.size ()) {
    for (
      list<S_msrSyllable>::const_iterator i =
        fCurrentNoteSyllables.begin();
      i != fCurrentNoteSyllables.end();
      i++ ) {
      // set syllable note uplink to newNote
      (*i)->setSyllableNoteUplink (newNote);
    } // for

    // forget all of newNote's syllables
    fCurrentNoteSyllables.clear ();
  }

  else {
    int inputLineNumber =
      newNote->getInputLineNumber ();
      
    // fetch current voice
    S_msrVoice
      currentVoice =
        registerVoiceInStaffInCurrentPartIfNeeded (
          inputLineNumber,
          fCurrentNoteStaffNumber,
          fCurrentVoiceNumber);

/*
 // JMI   if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
    if (true || gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
      cerr <<
        idtr <<
          "--> note \"" << newNote->noteAsString () << "\"" <<
          ", line " << inputLineNumber <<
          ", has no lyrics," <<
          endl;

      idtr++;

      cerr <<
        idtr <<
          "  appending a skip syllable to voice \"" <<
          currentVoice-> getVoiceName () <<
          "\"" <<
          ", fCurrentStanzaNumber = " << fCurrentStanzaNumber <<
          endl;

      idtr--;
      }

    // append a skip syllable to the voice
    S_msrSyllable
      syllable =
        currentVoice->
          addSkipSyllableToVoice (
            inputLineNumber,
            fCurrentStanzaNumber,
            fNoteData.fDivisions);

    // this ends the current syllable extension if any
    fOnGoingSyllableExtend = false;
*/
  }

  // is '<extend />' active for newNote?
  switch (fCurrentSyllableExtendKind) {
    case msrSyllable::kStandaloneSyllableExtend:
      fOnGoingSyllableExtend = true;
      break;
    case msrSyllable::kStartSyllableExtend:
      fOnGoingSyllableExtend = true;
      break;
    case msrSyllable::kContinueSyllableExtend:
      // keep fOnGoingSyllableExtend unchanged
      break;
    case msrSyllable::kStopSyllableExtend:
      fOnGoingSyllableExtend = false;
      break;
    case msrSyllable::k_NoSyllableExtend:
      break;
  } // switch

  if (fOnGoingSyllableExtend) // JMI
    // register newNote's extend kind
    newNote->
      setNoteSyllableExtendKind (
        fCurrentSyllableExtendKind);
}

//______________________________________________________________________________
void xml2MsrTranslator::handleRepeatStart (
  S_barline     elt,
  S_msrBarline& barline)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

//      if (gGeneralOptions->fDebug)
    cerr <<
      idtr << "--> input line " << inputLineNumber <<
      endl <<
      idtr <<
      "--> barline, left and forward: repeat start" <<
      endl;

  // set the barline category
  barline->
    setBarlineCategory (
      msrBarline::kRepeatStart);

  // fetch current voice
  S_msrVoice
    currentVoice =
      registerVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);


  // get the current segment
  S_msrSegment
    currentSegment =
      currentVoice->
        getVoiceLastSegment ();

  if (! fCurrentRepeat) {
    if (gGeneralOptions->fTrace)
      cerr << idtr <<
        "Creating a repeat in voice \"" <<
        currentVoice->getVoiceName () << "\"" <<
        endl;

    // create the repeat
    fCurrentRepeat =
      msrRepeat::create (
        inputLineNumber,
        currentVoice);
  }

  // append the repeat to the current voice
  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> appending the repeat to voice \"" <<
      currentVoice->getVoiceName () << "\"" <<
      endl;

  currentVoice->
    appendRepeatToVoice (fCurrentRepeat);

  // append the bar line to the voice
  currentVoice->
    appendBarlineToVoice (barline);

  // push the barline onto the stack
  fPendingBarlines.push (barline);
}

//______________________________________________________________________________
void xml2MsrTranslator::handleRepeatEnd (
  S_barline     elt,
  S_msrBarline& barline)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
    cerr <<
      idtr << "--> input line " << inputLineNumber <<
      endl <<
      idtr <<
      "--> barline, right and backward: repeat end" <<
      endl;
  }

  // set the barline category
  barline->
    setBarlineCategory (msrBarline::kRepeatEnd);

  // fetch current voice
  S_msrVoice
    currentVoice =
      registerVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  // get the current segment
  S_msrSegment
    currentSegment =
      currentVoice->
        getVoiceLastSegment ();

  // append the bar line to the current voice
  currentVoice->
    appendBarlineToVoice (barline);

  if (fPendingBarlines.empty ()) {
    if (gGeneralOptions->fTrace)
      cerr <<
        idtr <<
        "There is an implicit repeat start at the beginning of part" <<
        fCurrentPart->getPartCombinedName () <<
        endl;

    // create the implicit barline
    S_msrBarline
      implicitBarline =
        msrBarline::create (
          inputLineNumber,
          false, // no segno
          false, // no coda
          msrBarline::kLeft,
          msrBarline::kHeavyLight,
          msrBarline::kStart,
          fCurrentEndingNumber,
          msrBarline::kForward,
          fCurrentBarlineRepeatWinged);

    // set the implicit barline category
    implicitBarline->
      setBarlineCategory (
        msrBarline::kRepeatStart);
  
    // prepend the implicit barline to the voice
    currentVoice->
      prependBarlineToVoice (implicitBarline);
            
    if (! fCurrentRepeat) { // JMI
      // create the repeat
      if (gGeneralOptions->fTrace)
        cerr << idtr <<
          "Creating a repeat in voice \"" <<
          currentVoice->getVoiceName () << "\"" <<
          endl;

      fCurrentRepeat =
        msrRepeat::create (
          inputLineNumber,
          currentVoice);

      // set current segment as the repeat common part
      fCurrentRepeat->
        setRepeatCommonPart (
          currentSegment);
        
      // append the repeat to the current voice
      if (gGeneralOptions->fDebug)
        cerr << idtr <<
          "--> appending the repeat to voice \"" <<
          currentVoice->getVoiceName () << "\"" <<
          endl;
    
      currentVoice->
        appendRepeatToVoice (fCurrentRepeat);
    }
  }

  else {
    /* JMI
    if (gGeneralOptions->fTrace)
      cerr << idtr <<
        "Fetching barline from pending barlines stack top" <<
        endl;

    // fetch pending barline top
    S_msrBarline
      barline =
        fPendingBarlines.top ();

    // prepend the barline to the current segment
    currentVoice->
      prependBarlineToVoice (barline);
            
    // pop the barline top off the stack
    fPendingBarlines.pop ();
    */
  }

  // register current segment as current repeat's common part
  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> registering last segment of voice \"" <<
      currentVoice->getVoiceName () << "\"" <<
      " as repeat common part" <<
      endl;

  fCurrentRepeat->
    setRepeatCommonPart (currentSegment);
  
  // create a new segment for the voice
  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> setting a new last segment for voice \"" <<
      currentVoice->getVoiceName () << "\"" <<
      endl;
      
  currentVoice->
    createNewLastSegmentForVoice (
      inputLineNumber);
}

//______________________________________________________________________________
void xml2MsrTranslator::handleHookedEndingStart (
  S_barline     elt,
  S_msrBarline& barline)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

//  if (gGeneralOptions->fDebug)
    cerr <<
      idtr << "--> input line " <<
        inputLineNumber <<
      endl <<
      idtr <<
        "--> measure " <<
          barline->getBarlineMeasureNumber () <<
        ", position " <<
          barline->getBarlinePositionInMeasure () <<
      endl <<
      idtr <<
      "--> barline, left, start and forward: hooked ending start" <<
      endl;

  // set the barline category
  barline->
    setBarlineCategory (msrBarline::kHookedEndingStart);
  
  // fetch current voice
  S_msrVoice
    currentVoice =
      registerVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  // get the current segment
  S_msrSegment
    currentSegment =
      currentVoice->
        getVoiceLastSegment ();

  if (! fCurrentRepeat) { // JMI
    if (gGeneralOptions->fTrace)
      cerr << idtr <<
        "Creating a repeat in voice \"" <<
        currentVoice->getVoiceName () << "\"" <<
        endl;

    // create the repeat
    fCurrentRepeat =
      msrRepeat::create (
        inputLineNumber,
        currentVoice);

    // set the repeat common part
    fCurrentRepeat->
      setRepeatCommonPart (currentSegment);

    // append the repeat to the current voice
    if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> appending the repeat to voice \"" <<
        currentVoice->getVoiceName () << "\"" <<
        endl;
  
    currentVoice->
      appendRepeatToVoice (fCurrentRepeat);

    // create new segment for the just starting ending
    if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> setting new last segment for the just starting ending in voice \"" <<
        currentVoice->getVoiceName () << "\"" <<
        endl;
        
    currentVoice->
      createNewLastSegmentForVoice (
        inputLineNumber);
  }

  // append the bar line to the voice
  currentVoice->
    appendBarlineToVoice (barline);

  // push the barline onto the stack
  fPendingBarlines.push (barline);
}

//______________________________________________________________________________
void xml2MsrTranslator::handleHookedEndingEnd (
  S_barline     elt,
  S_msrBarline& barline)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

//       if (gGeneralOptions->fDebug)
    cerr <<
      idtr << "--> input line " << inputLineNumber <<
      endl <<
      idtr <<
      "--> barline right, stop: hooked ending end" <<
      endl;

  // set the barline category
  barline->
    setBarlineCategory (msrBarline::kHookedEndingEnd);

  // fetch current voice
  S_msrVoice
    currentVoice =
      registerVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  // append the bar line to the current segment
  currentVoice->
    appendBarlineToVoice (barline);

  // get the current segment
  S_msrSegment
    currentSegment =
      currentVoice->
        getVoiceLastSegment ();

/* JMI
  if (! fCurrentRepeat) { // JMI
    // create the repeat
    if (gGeneralOptions->fTrace)
      cerr << idtr <<
        "Creating a repeat for voice \"" <<
        currentVoice->getVoiceName () << "\"" <<
        endl;

    fCurrentRepeat =
      msrRepeat::create (
        inputLineNumber,
        currentSegment,
        currentVoice);
  }
    */
    
  // create a hooked repeat ending from the current segment
  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> creating a new hooked repeat ending for voice \"" <<
      currentVoice->getVoiceName () << "\"" <<
      endl;
      
  S_msrRepeatending
    repeatEnding =
      msrRepeatending::create (
        inputLineNumber,
        fCurrentEndingNumber,
        msrRepeatending::kHookedEnding,
        currentSegment,
        fCurrentRepeat);

  // add the repeat ending it to the current repeat
  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> appending repeat ending to current repeat in voice \"" <<
      currentVoice->getVoiceName () <<
      "\", fPendingBarlines.size() = " << fPendingBarlines.size () <<
      endl;
      
  fCurrentRepeat->
    addRepeatending (repeatEnding);
  
  // create a new segment for the voice
  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> creating new last segment for voice \"" <<
      currentVoice->getVoiceName () << "\"" <<
      endl;
      
  currentVoice->
    createNewLastSegmentForVoice (
      inputLineNumber);

    /*
  if (fPendingBarlines.empty ()) {
    if (gGeneralOptions->fTrace)
      cerr <<
        idtr <<
        "There's an implicit repeat start at the beginning of part " <<
        fCurrentPart->getPartCombinedName () <<
        endl;
        
    // create the implicit barline
    S_msrBarline
      implicitBarline =
        msrBarline::create (
          inputLineNumber,
          false, // no segno
          false, // no coda
          msrBarline::kLeft,
          msrBarline::kHeavyLight,
          msrBarline::kStart,
          fCurrentEndingNumber,
          msrBarline::kForward,
          fCurrentBarlineRepeatWinged);

    // set the implicit barline category
    implicitBarline->
      setBarlineCategory (
        msrBarline::kRepeatStart);
  
    // prepend the implicit barline to the current segment
    currentVoice->
      prependBarlineToVoice (implicitBarline);
            
    // get the current segment
    S_msrSegment
      currentSegment =
        currentVoice->
          getVoiceLastSegment ();

    if (! fCurrentRepeat) { // JMI
      // create the repeat
      if (gGeneralOptions->fTrace)
        cerr << idtr <<
          "Creating a repeat containing current segment in voice \"" <<
          currentVoice->getVoiceName () << "\"" <<
          endl;

      fCurrentRepeat =
        msrRepeat::create (
          inputLineNumber,
          currentSegment,
          currentVoice);
    }
  }
  
  else {
    /* JMI
    if (gGeneralOptions->fTrace)
      cerr << idtr <<
        "Fetching barline from pending barlines stack top" <<
        endl;

    // fetch pending barline top
    S_msrBarline
      barline =
        fPendingBarlines.top ();

    // prepend the barline to the current segment
    currentVoice->
      prependBarlineToVoice (barline);
            
    // pop the barline top off the stack
    fPendingBarlines.pop ();
  }
  */
}

//______________________________________________________________________________
void xml2MsrTranslator::handleHooklessEndingStart (
  S_barline     elt,
  S_msrBarline& barline)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

//  if (gGeneralOptions->fDebug)
    cerr <<
      idtr << "--> input line " <<
        inputLineNumber <<
      endl <<
      idtr <<
        "--> measure " <<
          barline->getBarlineMeasureNumber () <<
        ", position " <<
          barline->getBarlinePositionInMeasure () <<
      endl <<
      idtr <<
      "--> barline, left and start: hookless ending start" <<
      endl;

  // set the barline category
  barline->
    setBarlineCategory (msrBarline::kHooklessEndingStart);
  
  // fetch current voice
  S_msrVoice
    currentVoice =
      registerVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  // get the current segment
  S_msrSegment
    currentSegment =
      currentVoice->
        getVoiceLastSegment ();

/*
  if (! fCurrentRepeat) { // JMI
    if (gGeneralOptions->fTrace)
      cerr << idtr <<
        "Creating a repeat in voice \"" <<
        currentVoice->getVoiceName () << "\"" <<
        endl;

    // create the repeat
    fCurrentRepeat =
      msrRepeat::create (
        inputLineNumber,
        currentVoice);

    // set the repeat common part
    fCurrentRepeat->
      setRepeatCommonPart (currentSegment);

    // append the repeat to the current voice
    if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> appending the repeat to voice \"" <<
        currentVoice->getVoiceName () << "\"" <<
        endl;
  
    currentVoice->
      appendRepeatToVoice (fCurrentRepeat);

    // create new segment for the just starting ending
    if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> setting new last segment for the just starting ending in voice \"" <<
        currentVoice->getVoiceName () << "\"" <<
        endl;
        
    currentVoice->
      createNewLastSegmentForVoice (
        inputLineNumber);
  }
*/

  // append the bar line to the voice
  currentVoice->
    appendBarlineToVoice (barline);

  // push the barline onto the stack
  fPendingBarlines.push (barline);
}

//______________________________________________________________________________
void xml2MsrTranslator::handleHooklessEndingEnd (
  S_barline     elt,
  S_msrBarline& barline)
{
  /*
  The discontinue value is typically used for the last ending in a set,
  where there is no downward hook to mark the end of an ending:
  
    <barline location="right">
      <ending type="discontinue" number="2"/>
    </barline>
  */

  int inputLineNumber =
    elt->getInputLineNumber ();
  
//  if (gGeneralOptions->fDebug)
    cerr <<
      idtr << "--> input line " << inputLineNumber <<
      endl <<
      idtr <<
      "--> barline, right and discontinue: hookless ending end" <<
      endl;

  // set the barline category
  barline->
    setBarlineCategory (msrBarline::kHooklessEndingEnd);
  
  // fetch current voice
  S_msrVoice
    currentVoice =
      registerVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  // append the bar line to the current segment
  currentVoice->
    appendBarlineToVoice (barline);

  // get the current segment
  S_msrSegment
    currentSegment =
      currentVoice->
        getVoiceLastSegment ();

  // create a hookless repeat ending from the current segment
  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> creating a new hookless repeat ending for voice " <<
      currentVoice->getVoiceName () <<
      endl;
      
  S_msrRepeatending
    repeatEnding =
      msrRepeatending::create (
        inputLineNumber,
        fCurrentEndingNumber,
        msrRepeatending::kHooklessEnding,
        currentSegment,
        fCurrentRepeat);

  // add the repeat ending it to the current repeat
  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> appending repeat ending to current repeat in voice " <<
      currentVoice->getVoiceName () <<
      endl;
      
  fCurrentRepeat->
    addRepeatending (repeatEnding);

  // create a new segment for the voice
  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> creating new last segment for voice " <<
      currentVoice->getVoiceName () <<
      endl;
      
  currentVoice->
    createNewLastSegmentForVoice (
      inputLineNumber);

/*
  if (fPendingBarlines.empty ()) {
    if (gGeneralOptions->fTrace)
      cerr <<
        idtr <<
        "There is an implicit repeat start at the beginning of the part" <<
        endl;

    // create the implicit barline
    S_msrBarline
      implicitBarline =
        msrBarline::create (
          inputLineNumber,
          false, // no segno
          false, // no coda
          msrBarline::kLeft,
          msrBarline::kHeavyLight,
          msrBarline::kStart,
          fCurrentEndingNumber,
          msrBarline::kForward,
          fCurrentBarlineRepeatWinged);

    // set the implicit barline category
    implicitBarline->
      setBarlineCategory (
        msrBarline::kRepeatStart);
  
    // prepend the implicit barline to the current segment
    currentVoice->
      prependBarlineToVoice (implicitBarline);
            
    // get the current segment
    S_msrSegment
      currentSegment =
        currentVoice->
          getVoiceLastSegment ();

    if (! fCurrentRepeat) {
      // create the repeat
      if (gGeneralOptions->fTrace)
        cerr << idtr <<
          "Creating a repeat in voice " <<
          currentVoice->getVoiceName () << endl;

      fCurrentRepeat =
        msrRepeat::create (
          inputLineNumber,
          currentSegment,
          currentVoice);
    }
    
    // create a new segment for the voice
    if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> creating new last segment for voice " <<
        currentVoice->getVoiceName () << endl;
        
    currentVoice->
      createNewLastSegmentForVoice (
        inputLineNumber);

    // add the repeat to the new segment
    if (gGeneralOptions->fDebug)
      cerr << idtr <<
        "--> appending the repeat to voice " <<
        currentVoice->getVoiceName () << endl;

    currentVoice->
      appendRepeatToVoice (fCurrentRepeat);

// JMI      barlineIsAlright = true;
  }

  else {
    // pop the pending barline off the stack
    fPendingBarlines.pop ();
  }
  */
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_rehearsal& elt )
{
/*
      <direction placement="above">
        <direction-type>
          <rehearsal default-y="15" font-size="11.3" font-weight="bold">A</rehearsal>
        </direction-type>
      </direction>
*/

  string rehearsalValue = elt->getValue();

  string rehearsalEnclosure = 
    elt->getAttributeValue ("enclosure");

  int inputLineNumber =
    elt->getInputLineNumber ();
  
  msrRehearsal::msrRehearsalKind
    rehearsalKind =
      msrRehearsal::kNone; // default value

  if      (rehearsalEnclosure == "none") {
    rehearsalKind = msrRehearsal::kNone;
  }
  else if (rehearsalEnclosure == "kRectangle") {
    rehearsalKind = msrRehearsal::kRectangle;
  }
  else if (rehearsalEnclosure == "kOval") {
    rehearsalKind = msrRehearsal::kOval;
  }
  else if (rehearsalEnclosure == "kCircle") {
    rehearsalKind = msrRehearsal::kCircle;
  }
  else if (rehearsalEnclosure == "kBracket") {
    rehearsalKind = msrRehearsal::kBracket;
  }
  else if (rehearsalEnclosure == "kTriangle") {
    rehearsalKind = msrRehearsal::kTriangle;
  }
  else if (rehearsalEnclosure == "kDiamond") {
    rehearsalKind = msrRehearsal::kDiamond;
  }
  else {
    if (rehearsalEnclosure.size ()) {
      stringstream s;
      
      s <<
        "rehearsal enclosure \"" << rehearsalEnclosure <<
        "\"" << " is not handled, ignored";
        
      msrMusicXMLWarning (
        inputLineNumber,
        s.str());
    }    
  }

  // fetch current voice
  S_msrVoice
    currentVoice =
      registerVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);
    
  // create a rehearsal
 // if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "Creating rehearsal \"" << rehearsalValue << "\"" <<
      " in voice " <<
      currentVoice->getVoiceName () << endl;

  S_msrRehearsal
    rehearsal =
      msrRehearsal::create (
        inputLineNumber,
        rehearsalKind,
        rehearsalValue);

  // append the rehearsal to the current voice
  currentVoice->
    appendRehearsalToVoice (rehearsal);
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_harmony& elt )
{
}

void xml2MsrTranslator::visitStart ( S_root_step& elt )
{
  string step = elt->getValue ();
  
  if (step.length() != 1) {
    stringstream s;
    
    s <<
      "root step value " << step <<
      " should be a single letter from A to G";
      
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }

  fCurrentHarmonyRootStep = step [0];
}

void xml2MsrTranslator::visitStart ( S_root_alter& elt )
{
  fCurrentHarmonyRootAlter = (float)(*elt);
}

void xml2MsrTranslator::visitStart ( S_kind& elt )
{
  string kind = elt->getValue ();

  fCurrentHarmonyKindText =
    elt->getAttributeValue ("text");

  // check harmony kind
  if      (kind == "major")
    fCurrentHarmonyKind = msrHarmony::kMajor;
    
  else if (kind == "minor")
    fCurrentHarmonyKind = msrHarmony::kMinor;
    
  else if (kind == "dominant")
    fCurrentHarmonyKind = msrHarmony::kDominant;
    
  else if (kind == "augmented")
    fCurrentHarmonyKind = msrHarmony::kAugmented;
    
  else if (kind == "diminished")
    fCurrentHarmonyKind = msrHarmony::kDiminished;
    
  else if (kind == "suspended-fourth")
    fCurrentHarmonyKind = msrHarmony::kSuspendedFourth;
    
  else if (kind == "major-seventh")
    fCurrentHarmonyKind = msrHarmony::kMajorSeventh;
    
  else if (kind == "minor-seventh")
    fCurrentHarmonyKind = msrHarmony::kMinorSeventh;
    
  else if (kind == "major-ninth")
    fCurrentHarmonyKind = msrHarmony::kMajorNinth;
    
  else if (kind == "minor-ninth")
    fCurrentHarmonyKind = msrHarmony::kMinorNinth;
    
  else {
    if (kind.size ()) {
      msrMusicXMLError (
        elt->getInputLineNumber (),
        "unknown harmony kind \"" + kind + "\"");
    }
  }
}

void xml2MsrTranslator::visitStart ( S_bass_step& elt )
{
  string step = elt->getValue();
  
  if (step.length() != 1) {
    stringstream s;
    
    s <<
      "bass step value " << step <<
      " should be a single letter from A to G";
      
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }

  fCurrentHarmonyBassStep = step [0];
}

void xml2MsrTranslator::visitStart ( S_bass_alter& elt )
{
  fCurrentHarmonyBassAlter = (float)(*elt);
}

void xml2MsrTranslator::visitStart ( S_degree_value& elt )
{
  fCurrentHarmonyDegreeValue = (int)(*elt);
}

void xml2MsrTranslator::visitStart ( S_degree_alter& elt )
{
  fCurrentHarmonyDegreeAlter = (float)(*elt);
}

void xml2MsrTranslator::visitStart ( S_degree_type& elt )
{
  string degreeType = elt->getValue ();

  msrHarmony::msrHarmonyDegreeTypeKind // JMI
    degreeTypeKind;

  // check harmony degree type
  if      (degreeType == "add")
    degreeTypeKind = msrHarmony::kAdd;
    
  else if (degreeType == "alter")
    degreeTypeKind = msrHarmony::kAlter;
    
  else if (degreeType == "substract")
    degreeTypeKind = msrHarmony::kSubstract;
    
  else {
      msrMusicXMLError (
        elt->getInputLineNumber (),
        "unknown harmony degree type \"" + degreeType + "\"");
  }
}

void xml2MsrTranslator::visitEnd ( S_harmony& elt )
{
  // create the harmony
  S_msrHarmony
    harmony =
      msrHarmony::create (
        elt->getInputLineNumber (),
        fCurrentHarmonyRootStep, fCurrentHarmonyRootStep,
        fCurrentHarmonyKind, fCurrentHarmonyKindText,
        fCurrentHarmonyBassStep, fCurrentHarmonyBassAlter,
        fCurrentPart);

  // append it to current part
  fCurrentPart->
    appendHarmonyToPart (harmony);
}

/*
       <degree>
          <degree-value>13</degree-value>
          <degree-alter>-1</degree-alter>
          <degree-type>add</degree-type>
        </degree>
*/

/*
 http://usermanuals.musicxml.com/MusicXML/Content/CT-MusicXML-harmony.htm
  
      <harmony default-y="40" font-size="15.4">
        <root>
          <root-step>B</root-step>
        </root>
        <kind text="Maj7">major-seventh</kind>
       <degree>
          <degree-value>13</degree-value>
          <degree-alter>-1</degree-alter>
          <degree-type>add</degree-type>
        </degree>
        <bass>
          <bass-step>D</bass-step>
          <bass-alter>1</bass-alter>
        </bass>
      </harmony>
      
*/


} // namespace
