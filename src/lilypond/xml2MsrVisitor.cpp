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

#include "xml2MsrVisitor.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
xml2MsrVisitor::xml2MsrVisitor (
  S_msrOptions& msrOpts)
{
  fMsrOptions = msrOpts;

 // JMI fCurrentMeasureLocation.fMeasureNumber     = 0; // in case of an anacrusis
 // fCurrentMeasureLocation.fPositionInMeasure = 1;

  fMillimeters       = -1;
  fTenths            = -1;
  fOnGoingPageLayout = false;

  fCurrentDivisionsPerQuarterNote = 0;

  // create the MSR score
  fMsrScore =
    msrScore::create (fMsrOptions, 0);

  fOnGoingBarline = false;
  
  fCurrentWordsContents = "";

  fCurrentForwardStaffNumber = 1; // JMI
  fCurrentForwardVoiceNumber = 1; // JMI
  fCurrentVoiceNumber = 1; // JMI
  
  fCurrentLyricsNumber = -1; // JMI
  fCurrentSyllabic = "";
  fCurrentText = "";
  fCurrentElision = false;
  
  fCurrentLyricschunkKind     = msrLyricschunk::k_NoChunk;
  fFirstLyricschunkInSlurKind = msrLyricschunk::k_NoChunk;
  
  fCurrentBackupDuration = -1;

  fCurrentNoteStaffNumber = 0;
  fCurrentNoteVoiceNumber = 0;
  fOnGoingNote            = false;

  fOnGoingChord = false;
  
  fOnGoingSlur          = false;
  fOnGoingSlurHasLyrics = false;

  fOnGoingDirection     = true;
  fOnGoingDirectionType = false;

  fOnGoingRepeat = false;
  
  fOnGoingBackup  = false;
  fOnGoingForward = false;
}

xml2MsrVisitor::~xml2MsrVisitor ()
{}

//________________________________________________________________________
S_msrScore xml2MsrVisitor::buildMsrScoreFromXMLElementTree (
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
S_msrPartgroup xml2MsrVisitor::createImplicitMSRPartgroup (
  int inputLineNumber)
{
  /*
  A first part group is created with all the needed contents
  if none is specified in the MusicXML data.
  */

  // create an implicit part group
  fCurrentPartgroupNumber = 1;
  
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Creating an implicit part group with number " <<
      fCurrentPartgroupNumber << endl;

  S_msrPartgroup
    partgroup =
      msrPartgroup::create (
        fMsrOptions,
        inputLineNumber,
        fCurrentPartgroupNumber,
        "Implicit",
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
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Adding implicit part group " << fCurrentPartgroupNumber <<
      " to visitor's data" << endl;
      
  fPartgroupsMap [fCurrentPartgroupNumber] = partgroup;
  fPartgroupsList.push_front (partgroup);

  fImplicitPartgroup = partgroup;
  
  return partgroup;
}

//______________________________________________________________________________
S_msrPartgroup xml2MsrVisitor::fetchPartgroupInThisVisitor (
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
S_msrPart xml2MsrVisitor::createDownToPartIfNeeded (
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
S_msrStaff xml2MsrVisitor::createStaffInCurrentPartIfNeeded (
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
        addStaffToPart (
          inputLineNumber, staffNumber);

  return staff;
}  

//______________________________________________________________________________
S_msrVoice xml2MsrVisitor::createVoiceInStaffInCurrentPartIfNeeded (
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
        fetchVoiceFromStaff (voiceNumber);

  if (! voice) 
    // no, add it to the staff
    voice =
      staff->
        addVoiceToStaff (
          inputLineNumber, voiceNumber);

  return voice;
}  

//______________________________________________________________________________
void xml2MsrVisitor::visitStart ( S_work_number& elt )
{
  fMsrScore->getIdentification () ->
    setWorkNumber (
      elt->getInputLineNumber (),
      elt->getValue ());
}

void xml2MsrVisitor::visitStart ( S_work_title& elt )
{
  fMsrScore->getIdentification () ->
    setWorkTitle (
      elt->getInputLineNumber (),
      elt->getValue ());
}
  
void xml2MsrVisitor::visitStart ( S_movement_number& elt )
{
  fMsrScore->getIdentification () ->
    setMovementNumber (
      elt->getInputLineNumber (),
      elt->getValue ());
}

void xml2MsrVisitor::visitStart ( S_movement_title& elt )
{
  fMsrScore->getIdentification () ->
    setMovementTitle (
      elt->getInputLineNumber (),
      elt->getValue ());
}

void xml2MsrVisitor::visitStart ( S_creator& elt )
{
  fMsrScore->getIdentification () ->
    addCreator (
      elt->getInputLineNumber (),
      elt->getAttributeValue ("type"),
      elt->getValue ());
}

void xml2MsrVisitor::visitStart ( S_rights& elt )
{
  fMsrScore->getIdentification () ->
    setRights (
      elt->getInputLineNumber (),
      elt->getValue ());
}

void xml2MsrVisitor::visitStart ( S_software& elt )
{
  fMsrScore->getIdentification () ->
    addSoftware (
      elt->getInputLineNumber (),
      elt->getValue ());
}

void xml2MsrVisitor::visitStart ( S_encoding_date& elt )
{
  fMsrScore->getIdentification () ->
    setEncodingDate (
      elt->getInputLineNumber (),
      elt->getValue ());
}

//______________________________________________________________________________
void xml2MsrVisitor::visitStart ( S_millimeters& elt )
{ 
  fMillimeters = (int)(*elt);
  
  fMsrScore->getPageGeometry ()->
    setMillimeters (fMillimeters);
}

void xml2MsrVisitor::visitStart ( S_tenths& elt )
{
  fTenths = (int)(*elt);

  fMsrScore->getPageGeometry ()->
    setTenths (fTenths);
}

void xml2MsrVisitor::visitEnd ( S_scaling& elt)
{
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "There are " << fTenths <<
      " tenths for " <<  fMillimeters <<
      " millimeters, hence the global staff size is " <<
      fMsrScore->getPageGeometry ()->globalStaffSize () <<
      endl;
}

//______________________________________________________________________________
void xml2MsrVisitor::visitStart ( S_system_distance& elt )
{
  int systemDistance = (int)(*elt);
  
//  cerr << "--> systemDistance = " << systemDistance << endl;
  fMsrScore->getPageGeometry ()->
    setBetweenSystemSpace (
      1.0 * systemDistance * fMillimeters / fTenths / 10);  
}

void xml2MsrVisitor::visitStart ( S_top_system_distance& elt )
{
  int topSystemDistance = (int)(*elt);
  
//  cerr << "--> fTopSystemDistance = " << topSystemDistance << endl;
    fMsrScore->getPageGeometry ()->
    setPageTopSpace (
      1.0 * topSystemDistance * fMillimeters / fTenths / 10);  
}

//______________________________________________________________________________
void xml2MsrVisitor::visitStart ( S_page_layout& elt )
{
  fOnGoingPageLayout = true;
}
void xml2MsrVisitor::visitEnd ( S_page_layout& elt )
{
  fOnGoingPageLayout = false;
}

void xml2MsrVisitor::visitStart ( S_page_height& elt )
{
  if (fOnGoingPageLayout) {
    int pageHeight = (int)(*elt);
    
    //cerr << "--> pageHeight = " << pageHeight << endl;
    fMsrScore->getPageGeometry ()->
      setPaperHeight (
        1.0 * pageHeight * fMillimeters / fTenths / 10);  
  }
}

void xml2MsrVisitor::visitStart ( S_page_width& elt )
{
  if (fOnGoingPageLayout) {
    int pageWidth = (int)(*elt);
    
    //cerr << "--> pageWidth = " << pageWidth << endl;
    fMsrScore->getPageGeometry ()->
      setPaperWidth (
        1.0 * pageWidth * fMillimeters / fTenths / 10);  
  }
}

void xml2MsrVisitor::visitStart ( S_left_margin& elt )
{
  if (fOnGoingPageLayout) {
    int leftMargin = (int)(*elt);
    
    //cerr << "--> leftMargin = " << leftMargin << endl;
    fMsrScore->getPageGeometry ()->
      setLeftMargin (
        1.0 * leftMargin * fMillimeters / fTenths / 10);  
  }
}

void xml2MsrVisitor::visitStart ( S_right_margin& elt )
{
  if (fOnGoingPageLayout) {
    int rightMargin = (int)(*elt);
    
    //cerr << "--> rightMargin = " << rightMargin << endl;
    fMsrScore->getPageGeometry ()->
      setRightMargin (
        1.0 * rightMargin * fMillimeters / fTenths / 10);  
  }
}

void xml2MsrVisitor::visitStart ( S_top_margin& elt )
{
  if (fOnGoingPageLayout) {
    int topMargin = (int)(*elt);
    
    //cerr << "--> topMargin = " << topMargin << endl;
    fMsrScore->getPageGeometry ()->
      setTopMargin (
        1.0 * topMargin * fMillimeters / fTenths / 10);  
  }
}

void xml2MsrVisitor::visitStart ( S_bottom_margin& elt )
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
/* JMI
void xml2MsrVisitor::visitStart ( S_instrument_name& elt)
{
  fCurrentInstrumentName = elt->getValue();
}
*/

//________________________________________________________________________
void xml2MsrVisitor::visitStart (S_part_list& elt)
{
  if (fMsrOptions->fTrace)
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

void xml2MsrVisitor::visitEnd (S_part_list& elt)
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
*/

//________________________________________________________________________
void xml2MsrVisitor::visitStart (S_part_group& elt)
{  
  fCurrentPartgroupNumber =
    elt->getAttributeIntValue ("number", 0);
    
  fCurrentPartgroupType =
    elt->getAttributeValue ("type");

  fCurrentPartgroupName = "";
  fCurrentPartgroupAbbreviation = "";
  fCurrentPartgroupSymbol = "";
  fCurrentPartgroupSymbolDefaultX = INT_MIN;
  fCurrentPartgroupBarline = "yes";
}

void xml2MsrVisitor::visitStart (S_group_name& elt)
{
  fCurrentPartgroupName = elt->getValue();
}

void xml2MsrVisitor::visitStart (S_group_abbreviation& elt)
{
  fCurrentPartgroupAbbreviation = elt->getValue ();
}

void xml2MsrVisitor::visitStart (S_group_symbol& elt)
{
  fCurrentPartgroupSymbol = elt->getValue ();

  fCurrentPartgroupSymbolDefaultX =
    elt->getAttributeIntValue ("default-x", 0);
}

void xml2MsrVisitor::visitStart ( S_group_barline& elt)
{
  fCurrentPartgroupBarline = elt->getValue ();
}

//________________________________________________________________________
void xml2MsrVisitor::showPartgroupsData (string context)
{    
// JMI  if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
  if (fMsrOptions->fDebug) {
    cerr << idtr <<
      "==> " << context << ": fPartgroupsMap contains:" << endl;
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
    cerr << idtr << "<== fPartgroupsMap" << endl;
  }

  if (fMsrOptions->fForceDebug || fMsrOptions->fDebugDebug) {
    cerr << idtr <<
      "==> " << context << ": fPartgroupsList contains:" << endl;
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
    cerr << idtr << "<== fPartgroupsList" << endl;
  }
}

//________________________________________________________________________
void xml2MsrVisitor::handlePartgroupStart (
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
        fMsrOptions,
        inputLineNumber,
        fCurrentPartgroupNumber,
        fCurrentPartgroupName,
        fCurrentPartgroupAbbreviation,
        partgroupSymbol,
        fCurrentPartgroupSymbolDefaultX,
        partgroupBarline,
        currentPartgroup);

    // add it to the part group map of this visitor
    if (fMsrOptions->fTrace)
      cerr << idtr <<
        "Adding part group " << fCurrentPartgroupNumber <<
        " to visitor's part group map" << endl;
    fPartgroupsMap [fCurrentPartgroupNumber] =
      partgroupToBeStarted;
  }
  
  // add it to the part group list of this visitor
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Adding part group " << fCurrentPartgroupNumber <<
      " to visitor's part groups list" << endl;

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
void xml2MsrVisitor::handlePartgroupStop (int inputLineNumber)
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
      fMsrOptions->fInputSourceName,
      inputLineNumber,
      s.str());
  }

  // remove the part group to be stopped from the part group list
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Removing part group " <<
      partgroupToBeStopped->getPartgroupNumber () <<
      " from visitor's part groups list" << endl;

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
      fMsrOptions->fInputSourceName,
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
    if (fMsrOptions->fTrace)
      cerr << idtr <<
        "Appending part group " <<
        partgroupToBeStopped->getPartgroupNumber () <<
        " to MSR score" << endl;
        
    fMsrScore->
      addPartgroupToScore (partgroupToBeStopped);
      
  } else {

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
        ", newCurrentPartgroup = " << newCurrentPartgroup << endl;

      stringstream s;
      s <<
        "cannot append part group " <<
        partgroupToBeStopped->getPartgroupNumber () <<
        " as sub part group of itself";
      msrInternalError (
        fMsrOptions->fInputSourceName,
        inputLineNumber,
        s.str());
    }
    
    // insert current group into future current group
    if (fMsrOptions->fTrace)
      cerr << idtr <<
        "Preending (sub-)part group " <<
        partgroupToBeStopped->getPartgroupNumber () <<
        " at the beginning of part group " <<
        newCurrentPartgroup->getPartgroupNumber () << endl;

    newCurrentPartgroup->
      prependSubPartgroupToPartgroup (
        partgroupToBeStopped);
  }

  // remove part group from the map
  // CAUTION: erase() destroys the element it removes!
  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Removing part group " << fCurrentPartgroupNumber <<
      " from visitor's part group map" << endl;
  try {
    fPartgroupsMap.erase (fCurrentPartgroupNumber);
  }
  catch (int e) {
    cerr << "An exception number " << e << " occurred" << endl;
  }

  showPartgroupsData ("AFTER STOP");
} // handlePartgroupStop ()

//________________________________________________________________________
void xml2MsrVisitor::visitEnd (S_part_group& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Handling part group " << fCurrentPartgroupNumber <<
      ", type: \"" << fCurrentPartgroupType << "\""  << endl;

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
        fMsrOptions->fInputSourceName,
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
        fMsrOptions->fInputSourceName,
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
      fMsrOptions->fInputSourceName,
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
void xml2MsrVisitor::visitStart (S_score_part& elt)
{
  fCurrentPartID = elt->getAttributeValue ("id");

  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "Found part name \"" << fCurrentPartID << "\"" << endl;

  fCurrentPartName         = "";
  fCurrentPartAbbreviation = "";
  
  fCurrentPartInstrumentName = "";
}

void xml2MsrVisitor::visitStart (S_part_name& elt)
{
  fCurrentPartName = elt->getValue ();
}

void xml2MsrVisitor::visitStart (S_part_abbreviation& elt)
{
  fCurrentPartAbbreviation = elt->getValue ();
}

void xml2MsrVisitor::visitStart (S_instrument_name& elt)
{
  fCurrentPartInstrumentName = elt->getValue();
}

void xml2MsrVisitor::visitEnd (S_score_part& elt)
{
  string partID = elt->getAttributeValue ("id");

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (fMsrOptions->fTrace)
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
    cerr << "An exception number " << e << " occurred" << endl;
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
        addPartToPartgroup (
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
void xml2MsrVisitor::visitStart (S_part& elt)
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
      fMsrOptions->fInputSourceName,
      inputLineNumber,
      "part " +
        fCurrentPartID +
        " is not registered in this visitor's part map");

  if (fMsrOptions->fTrace)
    cerr <<
      idtr <<
      "--------------------------------------------" <<
      endl <<
      idtr <<
      "Analyzing part \"" << fCurrentPartID << "\"" <<
      endl;

  idtr++;

  fCurrentStaffNumber = 1; // default if there are no <staff> element
  fCurrentVoiceNumber = 1; // default if there are no <voice> element

  fOnGoingRepeat = false;
}

void xml2MsrVisitor::visitEnd (S_part& elt)
{
  idtr--;
}

//______________________________________________________________________________
void xml2MsrVisitor::visitStart ( S_divisions& elt ) 
{
  fCurrentDivisionsPerQuarterNote = (int)(*elt);
  
  if (fCurrentDivisionsPerQuarterNote <= 0)
    msrMusicXMLError (
      fMsrOptions->fInputSourceName,
      elt->getInputLineNumber (),
      "divisions per quarter note should be positive");
  
  if (fMsrOptions->fTrace) {
    cerr << idtr;
    if (fCurrentDivisionsPerQuarterNote== 1)
      cerr << "There is 1 division";
    else
      cerr <<
        "There are " << fCurrentDivisionsPerQuarterNote<<
        " divisions";
    cerr <<
      " per quater note in part " <<
      fCurrentPart->getPartCombinedName() <<
      endl;
  }

  fCurrentPart->
    setDivisionsPerWholeNote (
      fCurrentDivisionsPerQuarterNote * 4);
}

//______________________________________________________________________________
void xml2MsrVisitor::visitStart ( S_clef& elt )
{ 
  // The optional number attribute refers to staff numbers.
  // If absent (0), apply to all part staves.

  // https://usermanuals.musicxml.com/MusicXML/Content/EL-MusicXML-clef.htm
  
  fCurrentClefStaffNumber =
    elt->getAttributeIntValue("number", 0); 

  fCurrentClefLine = 0;;
  fCurrentClefOctaveChange = 0;
  fCurrentClefSign = "";
}

void xml2MsrVisitor::visitStart ( S_clef_octave_change& elt )
  { fCurrentClefOctaveChange = (int)(*elt); }
  
void xml2MsrVisitor::visitStart ( S_line& elt )
  { fCurrentClefLine = (int)(*elt); }
  
void xml2MsrVisitor::visitStart ( S_sign& elt )
  { fCurrentClefSign = elt->getValue(); }

void xml2MsrVisitor::visitEnd ( S_clef& elt ) 
{    
  int inputLineNumber =
    elt->getInputLineNumber ();

  // create msrClef
  S_msrClef
    clef =
      msrClef::create (
        fMsrOptions,
        inputLineNumber,
        fCurrentClefSign, fCurrentClefLine, fCurrentClefOctaveChange);
 
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
void xml2MsrVisitor::visitStart ( S_key& elt )
{
  
  // The optional number attribute refers to staff numbers.
  // If absent (0), apply to all part staves.
  fCurrentKeyStaffNumber =
    elt->getAttributeIntValue ("number", 0);

  fCurrentFifths = 0;
  fCurrentCancel = 0;
  fCurrentMode   = "";
}
  
void xml2MsrVisitor::visitStart ( S_fifths& elt )
  { fCurrentFifths = (int)(*elt); }
  
void xml2MsrVisitor::visitStart ( S_mode& elt )
  { fCurrentMode = elt->getValue(); }

void xml2MsrVisitor::visitStart ( S_cancel& elt )
  { fCurrentCancel = (int)(*elt); }

void xml2MsrVisitor::visitEnd ( S_key& elt ) 
{  
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  // create msrKey
  S_msrKey
    key =
      msrKey::create (
        fMsrOptions,
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
void xml2MsrVisitor::visitStart ( S_time& elt )
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

void xml2MsrVisitor::visitStart ( S_beats& elt )
{ fCurrentTimeBeats = (int)(*elt); }
  
void xml2MsrVisitor::visitStart ( S_beat_type& elt )
  { fCurrentTimeBeatType = (int)(*elt); }
 
void xml2MsrVisitor::visitStart ( S_senza_misura& elt )
  { fCurrentTimeSenzaMisura = true; }

void xml2MsrVisitor::visitEnd ( S_time& elt ) 
{  
  int inputLineNumber =
    elt->getInputLineNumber ();

  // create msrTime
  S_msrTime
    time =
      msrTime::create (
        fMsrOptions,
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
void xml2MsrVisitor::visitStart ( S_transpose& elt )
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

void xml2MsrVisitor::visitStart ( S_diatonic& elt )
{ fCurrentTransposeDiatonic = (int)(*elt); }
  
void xml2MsrVisitor::visitStart ( S_chromatic& elt )
  { fCurrentTransposeChromatic = (int)(*elt); }
 
void xml2MsrVisitor::visitEnd ( S_transpose& elt ) 
{  
  int inputLineNumber =
    elt->getInputLineNumber ();

  // create msrTranspose
  S_msrTranspose
    transpose =
      msrTranspose::create (
        fMsrOptions,
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
void xml2MsrVisitor::visitStart (S_direction& elt)
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

  fCurrentDirectionPlacement = elt->getAttributeValue ("placement");

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
  //  msrMusicXMLWarning (
      fMsrOptions->fInputSourceName,
      elt->getInputLineNumber (),
      s.str());    
  }

  fCurrentWordsContents = ""; // there can be several such

  fCurrentWords = 0;
  fCurrentTempo = 0;

  fOnGoingDirection = true;
}

void xml2MsrVisitor::visitStart (S_direction_type& elt)
{
  fOnGoingDirectionType = true;
}

void xml2MsrVisitor::visitEnd (S_direction_type& elt)
{
  fOnGoingDirectionType = false;
}

void xml2MsrVisitor::visitStart (S_octave_shift& elt)
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
      fMsrOptions->fInputSourceName,
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
      fMsrOptions->fInputSourceName,
      elt->getInputLineNumber (),
      s.str());
  }

  // create an octave shift
  S_msrOctaveShift
    octaveShift =
      msrOctaveShift::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        octaveShiftKind,
        size);

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        elt->getInputLineNumber (),
        fCurrentDirectionStaffNumber,
        fCurrentVoiceNumber);

  // append octave shift to the current voice
  currentVoice->
    appendOctaveShiftToVoice (octaveShift);
}

void xml2MsrVisitor::visitStart (S_words& elt)
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
    if (fMsrOptions->fTrace)
      cerr << idtr <<
        "Creating words \"" << fCurrentWordsContents << "\"" <<
        ", placement = \"" << fCurrentDirectionPlacement << "\"" <<
        endl;

    S_msrWords
      words =
        msrWords::create (
          fMsrOptions,
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
void xml2MsrVisitor::visitStart ( S_metronome& elt )
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
        fMsrOptions->fInputSourceName,
        elt->getInputLineNumber (),
        s.str());
    }
  }
}
  
void xml2MsrVisitor::visitEnd ( S_metronome& elt )
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
      fMsrOptions->fInputSourceName,
      inputLineNumber,
      "multiple beats found, but only per-minute tempo is supported");
    return;
  }
  
  if (! fPerMinute) {
    msrMusicXMLWarning (
      fMsrOptions->fInputSourceName,
      inputLineNumber,
      "per-minute not found, only per-minute tempo is supported");
    return;
  }

  musicXMLBeatData b = fBeatsData[0];
  rational         r = 
    NoteType::type2rational(
      NoteType::xml (b.fBeatUnit)), rdot(3,2);
  
  while (b.fDots-- > 0) { // JMI
    r *= rdot;
  }
  r.rationalise ();

  fCurrentTempo =
    msrTempo::create (
      fMsrOptions,
      inputLineNumber,
      r.getDenominator(), fPerMinute);

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentDirectionStaffNumber,
        fCurrentVoiceNumber);

  currentVoice->
    appendTempoToVoice (fCurrentTempo);
  
  // JMI if (fCurrentOffset) addDelayed(cmd, fCurrentOffset);
}

void xml2MsrVisitor::visitStart ( S_beat_unit& elt )
{ 
  if (fCurrentBeat.fBeatUnit.size()) {
    fBeatsData.push_back (fCurrentBeat); 
    fCurrentBeat.fBeatUnit = "";
    fCurrentBeat.fDots = 0;
  }
  
  fCurrentBeat.fBeatUnit = elt->getValue();
}

void xml2MsrVisitor::visitStart ( S_beat_unit_dot& elt )
  { fCurrentBeat.fDots++; }
  
void xml2MsrVisitor::visitStart ( S_per_minute& elt )
  { fPerMinute = (int)(*elt); }

void xml2MsrVisitor::visitEnd (S_direction& elt)
{
  int inputLineNumber = // JMI
    elt->getInputLineNumber ();

  if (fCurrentTempo) {
    if (fCurrentWordsContents.size ())
      fCurrentTempo->
        setTempoIndication (fCurrentWordsContents);
  }

  fOnGoingDirection = false;
}

//________________________________________________________________________
void xml2MsrVisitor::visitStart (S_staves& elt)
{
  int stavesNumber = int(*elt);

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (fMsrOptions->fTrace) {
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
    // add n-1 staves to current part
    int n = 2;
    
    while (n <= stavesNumber) {
      fCurrentPart->
        addStaffToPart (
          inputLineNumber, n);
      n++;
    } // while
  }
}

void xml2MsrVisitor::visitStart (S_staff& elt)
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

  if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
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
      fMsrOptions->fInputSourceName,
      inputLineNumber,
      s.str());    
  }
}
    
//________________________________________________________________________
void xml2MsrVisitor::visitStart (S_voice& elt )
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

  // the voice number should be in the 1..4 range
  if (fCurrentVoiceNumber < 1 || fCurrentVoiceNumber > 4) {
    stringstream s;

    s <<
      "voice number " << fCurrentVoiceNumber <<
      " is not in the 1..4 range";
      
    msrAssert (false, s.str());
  }
  
  if (fOnGoingForward) {

    fCurrentForwardVoiceNumber = fCurrentVoiceNumber;

    S_msrStaff
      staff =
        createStaffInCurrentPartIfNeeded (
          inputLineNumber, fCurrentForwardVoiceNumber);
  
  //  if (fMsrOptions->fDebug)
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
  
    if (fMsrOptions->fDebug)
      cerr <<
        idtr <<
          "--> fCurrentNoteVoiceNumber        = " <<
          fCurrentNoteVoiceNumber << endl <<
        idtr <<
          "--> S_voice, fCurrentNoteStaffNumber = " <<
          fCurrentNoteStaffNumber << endl <<
        idtr <<
          "--> S_voice, current staff name  = " <<
          staff->getStaffName() <<
        endl;
/*
    currentVoice = // ???
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);
  */  
  }
  
  else {
    
    stringstream s;
    
    s << "voice " << fCurrentVoiceNumber << " is out of context";
    
    msrMusicXMLError (
      fMsrOptions->fInputSourceName,
      inputLineNumber,
      s.str());
  }
}

//________________________________________________________________________
void xml2MsrVisitor::visitStart (S_backup& elt )
{
/*
 The backup and forward elements are required to coordinate multiple voices in one part, including music on multiple staves. The backup type is generally used to move between voices and staves. Thus the backup element does not include voice or staff elements. Duration values should always be positive, and should not cross measure boundaries or mid-measure changes in the divisions value.
 
      <backup>
        <duration>8</duration>
      </backup>
*/

  handleTupletsPendingOnTupletStack ();
  
  fOnGoingBackup = true;
}

void xml2MsrVisitor::visitEnd (S_backup& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Handling 'backup <<< " << fCurrentBackupDuration <<
      " divisions'" << endl;

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  int
    saveCurrentPositionInMeasure =
      currentVoice->
        getVoiceMeasureLocation ().fPositionInMeasure;
  
  currentVoice->
    setPositionInMeasure (
      currentVoice->getVoicePositionInMeasure ()
        -
      fCurrentBackupDuration);

  if (currentVoice->getVoicePositionInMeasure () < 0) {
    stringstream s;
    s <<
      "backup divisions " << fCurrentBackupDuration <<
      " from position " << saveCurrentPositionInMeasure <<
      " in voice \"" << currentVoice->getVoiceName () <<
      "\" crosses measure left boundary";
      
// JMI    msrMusicXMLError (s.str());
    msrMusicXMLWarning (
      fMsrOptions->fInputSourceName,
      inputLineNumber,
      s.str());
  }
  
  fOnGoingBackup = false;
}

//______________________________________________________________________________
void xml2MsrVisitor::visitStart ( S_forward& elt )
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
  
  handleTupletsPendingOnTupletStack ();
  
  fOnGoingForward = true;
}

void xml2MsrVisitor::visitEnd ( S_forward& elt )
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
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  if (fMsrOptions->fTrace)
    cerr << idtr <<
      "Handling 'forward >>> " << fCurrentForwardDuration <<
      "', thus switching to voice \"" << currentVoice->getVoiceName () <<
      "\" in staff \"" << staff->getStaffName () << "\"" <<
      endl;

//  currentVoice->handleForward (fCurrentForwardDuration); // JMI

  for (int i = 0; i < fCurrentForwardDuration; i++) {
    // generate rests for the duration of the forward move
    int restDivisions = 1;
    
    S_msrNote
      rest =
        msrNote::createRest (
          fMsrOptions,
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
  
  fOnGoingForward = false;
}

//________________________________________________________________________
void xml2MsrVisitor::visitStart (S_tied& elt )
{
// <tied orientation="over" type="start"/>

  string tiedType =
    elt->getAttributeValue ("type");
  
  fCurrentTiedOrientation =
    elt->getAttributeValue ("orientation");
  
  if (tiedType == "start") { // JMI
    
    fMusicXMLNoteData.fMusicXMLTieKind =
      msrMusicXMLNoteData::kStartTie;
    
  }
  else if (tiedType == "continue") {
    
    fMusicXMLNoteData.fMusicXMLTieKind =
      msrMusicXMLNoteData::kContinueTie;
    
  }
  else if (tiedType == "stop") {
    
    fMusicXMLNoteData.fMusicXMLTieKind =
      msrMusicXMLNoteData::kStopTie;
    
  }
  else {

    // inner tied notes may miss the "continue" type:
    // let' complain on slur notes outside of slurs 
    if (! fOnGoingSlur)
      if (tiedType.size()) {
        stringstream s;
        
        s << "tied type" << fCurrentSlurType << "unknown";
        
        msrMusicXMLError (
          fMsrOptions->fInputSourceName,
          elt->getInputLineNumber (),
          s.str());
      }
      
  }
}

//________________________________________________________________________
void xml2MsrVisitor::visitStart (S_slur& elt )
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

  if (fCurrentSlurType == "start") {
    
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
          fMsrOptions->fInputSourceName,
          elt->getInputLineNumber (),
          s.str());
      }
  }
}

//________________________________________________________________________
void xml2MsrVisitor::visitStart (S_lyric& elt )
{ 
  fCurrentLyricsNumber =
    elt->getAttributeIntValue ("number", 0);

  fCurrentLyricsHasText = false;
  fCurrentElision = false;

  fCurrentNoteHasLyrics = true;
}

void xml2MsrVisitor::visitStart ( S_syllabic& elt )
{
  fCurrentSyllabic = elt->getValue();
  
  if      (fCurrentSyllabic == "single")
    fCurrentLyricschunkKind = msrLyricschunk::kSingleChunk;
  else if (fCurrentSyllabic == "begin")
    fCurrentLyricschunkKind = msrLyricschunk::kBeginChunk;
  else if (fCurrentSyllabic == "middle")
    fCurrentLyricschunkKind = msrLyricschunk::kMiddleChunk;
  else if (fCurrentSyllabic == "end")
    fCurrentLyricschunkKind = msrLyricschunk::kEndChunk;
  else {
    stringstream s;
    
    s << "--> syllabic \"" << fCurrentSyllabic << "\" is unknown";
    
    msrMusicXMLError (
      fMsrOptions->fInputSourceName,
      elt->getInputLineNumber (),
      s.str());
  }
}

void xml2MsrVisitor::visitEnd ( S_text& elt ) 
{
  string text = elt->getValue();

  // text may be composed of only spaces, so:
  string dest;
  for_each (
    text.begin(), text.end(), stringSpaceRemover (dest));

  if (fCurrentElision)
    fCurrentText += " "+dest;
  else
    fCurrentText = dest;

  fCurrentLyricsHasText = true;

  if (fMsrOptions->fForceDebug || fMsrOptions->fDebug)
    cerr << idtr <<
      "--> fCurrentLyricsNumber = " << fCurrentLyricsNumber <<
      ", fCurrentSyllabic = " << fCurrentSyllabic <<
      ", fCurrentText = |" << fCurrentText << "|" << endl;
}

void xml2MsrVisitor::visitEnd ( S_elision& elt ) 
{
  fCurrentElision = true;
}

void xml2MsrVisitor::visitEnd ( S_lyric& elt )
{
  // the handling of lyrics is done in handleLyrics (),
  // at the end of visitEnd ( S_note )
}

//________________________________________________________________________
void xml2MsrVisitor::visitStart (S_measure& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  int measureNumber =
      elt->getAttributeIntValue ("number", 0);

  // Measures with an implicit attribute set to "yes"
  // never display a measure number,
  // regardless of the measure-numbering setting.
  string
    implicit =
      elt->getAttributeValue ("implicit");

  msrMeasureLocation
    measureLocation (measureNumber, 1);
  
  fCurrentPart->
    setPartMeasureLocation (
      inputLineNumber, measureLocation);

/*
  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

 // JMI if (implicit != "yes")
    currentVoice->
      setMeasureNumber (
        inputLineNumber, measureNumber);

  // is this measure number in the debug set?
  set<int>::iterator
    it =
      fMsrOptions->
        fDebugMeasureNumbersSet.find (
          currentVoice->
            getVoiceMeasureLocation ().fMeasureNumber);
        
  if (it != fMsrOptions->fDebugMeasureNumbersSet.end ()) {
    // yes, activate debug for it
    fMsrOptions->fSaveDebug = fMsrOptions->fDebug;
    fMsrOptions->fSaveDebugDebug = fMsrOptions->fDebugDebug;
  }

  currentVoice->
    setDivisionsPerWholeNote (
      fCurrentDivisionsPerQuarterNote);
    
  currentVoice->setPositionInMeasure (1);
    
  if (fMsrOptions->fDebug)
    cerr << idtr << 
      "=== MEASURE " <<
      currentVoice->
        getVoiceMeasureLocation ().fMeasureNumber <<
      " === " <<
      "PART " << fCurrentPart->getPartCombinedName () <<
      " ===" <<
      endl;

  if (measureNumber != 0) {
    // don't generate a bar check after the anacrusis
    S_msrBarCheck
      barCheck =
        msrBarCheck::create (
          fMsrOptions,
          inputLineNumber,
          currentVoice->
            getVoiceMeasureLocation ().fMeasureNumber);
              
    // append it to the voice
    if (currentVoice) {
      // it may not have been created yet JMI
      currentVoice->
        appendBarCheckToVoice (barCheck);
/ *
      // add a break chunk to the voice master lyrics
      currentVoice->
        getVoiceMasterLyrics ()->
          addBarCheckChunkToLyrics (
            inputLineNumber,
            currentVoice->getVoiceMeasureLocation ().fMeasureNumber);
            * /
    }
  }
  */
}

void xml2MsrVisitor::visitEnd (S_measure& elt)
{
  // restore debug options in case they were set in visitStart()
  fMsrOptions->fDebug = fMsrOptions->fSaveDebug;
  fMsrOptions->fDebugDebug = fMsrOptions->fSaveDebugDebug;
}

//______________________________________________________________________________
void xml2MsrVisitor::visitStart ( S_print& elt ) 
{
  const string& newSystem = elt->getAttributeValue ("new-system");
  
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (newSystem.size()) {
    
    if (newSystem == "yes") {
      
      int inputLineNumber =
        elt->getInputLineNumber ();
      
      // create a barnumbercheck command
      if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
        cerr << idtr << 
          "Creating a barnumber check, " <<
          "line = " << inputLineNumber << endl;
      }

      // fetch current voice
      S_msrVoice
        currentVoice =
          createVoiceInStaffInCurrentPartIfNeeded (
            inputLineNumber,
            fCurrentStaffNumber,
            fCurrentVoiceNumber);

      S_msrBarnumberCheck
        barnumbercheck_ =
          msrBarnumberCheck::create (
            fMsrOptions,
            inputLineNumber,
            currentVoice->
              getVoiceMeasureLocation ().fMeasureNumber);
            
      // append it to the voice
// JMI      S_msrElement bnc = barnumbercheck_;
      currentVoice->
        appendBarnumberCheckToVoice (barnumbercheck_);
  
      // create a break command
      if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
        cerr << idtr << 
          "Creating a break, " <<
          "line = " << inputLineNumber << endl;
      }

      S_msrBreak
        break_ =
          msrBreak::create(
            fMsrOptions,
            inputLineNumber,
            currentVoice->
              getVoiceMeasureLocation ().fMeasureNumber);
  
      // append it to the voice
      currentVoice->
        appendBreakToVoice (break_);
 /* JMI   
      // add a break chunk to the voice master lyrics
      currentVoice->
        getVoiceMasterLyrics ()->
          addBreakChunkToLyrics (
            inputLineNumber,
            currentVoice->getVoiceMeasureLocation ().fMeasureNumber);
            */
    }
    
    else if (newSystem == "no") {
      // ignore it
    }
    
    else {
      stringstream s;
  
      s << "unknown 'new-system' value '" << newSystem <<
      "' in '<print />', should be 'yes', 'no' or empty";
      
      msrMusicXMLError (
        fMsrOptions->fInputSourceName,
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
void xml2MsrVisitor::visitStart ( S_barline& elt ) 
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
  fCurrentEndingNumber    = "";
  fCurrentRepeatDirection = "";
  fCurrentRepeatWinged    = "";

  fCurrentBarlineHasSegno = false;
  fCurrentBarlineHasCoda  = false;

  fCurrentBarlineLocation        = msrBarline::k_NoLocation;
  fCurrentBarlineStyle           = msrBarline::k_NoStyle;
  fCurrentBarlineEndingType      = msrBarline::k_NoEndingType;
  fCurrentBarlineEndingNumber    = ""; // may be "1, 2"
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
      fMsrOptions->fInputSourceName,
      elt->getInputLineNumber (),
      s.str());
  }

  fOnGoingBarline = true;
}

//______________________________________________________________________________
void xml2MsrVisitor::visitStart ( S_bar_style& elt ) 
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
      fMsrOptions->fInputSourceName,
      elt->getInputLineNumber (),
      "barline style " + fCurrentStyle + " is unknown");
  }
}

//______________________________________________________________________________
void xml2MsrVisitor::visitStart ( S_segno& elt ) 
{
  if (fOnGoingDirectionType) {
    int inputLineNumber =
      elt->getInputLineNumber ();
      
    // fetch current voice
    S_msrVoice
      currentVoice =
        createVoiceInStaffInCurrentPartIfNeeded (
          inputLineNumber,
          fCurrentStaffNumber,
          fCurrentVoiceNumber);
  
    // create the segno
    S_msrSegno
      segno =
        msrSegno::create (
          fMsrOptions,
          inputLineNumber);
  
    // set the segno measure location
    segno->
      setSegnoMeasureLocation (
        currentVoice->getVoiceMeasureLocation ());

    // append it to the current voice
    currentVoice->
      appendSegnoToVoice (segno);
  }
  
  else if (fOnGoingBarline) {
    fCurrentBarlineHasSegno = true;
  }
}

void xml2MsrVisitor::visitStart ( S_coda& elt ) 
{
  if (fOnGoingDirectionType) {
    int inputLineNumber =
      elt->getInputLineNumber ();
      
    // fetch current voice
    S_msrVoice
      currentVoice =
        createVoiceInStaffInCurrentPartIfNeeded (
          inputLineNumber,
          fCurrentStaffNumber,
          fCurrentVoiceNumber);
  
    // create the coda
    S_msrCoda
      coda =
        msrCoda::create (
          fMsrOptions,
          inputLineNumber);
  
    // set the coda measure location
    coda->
      setCodaMeasureLocation (
        currentVoice->getVoiceMeasureLocation ());

    // append it to the current voice
    currentVoice->
      appendCodaToVoice (coda);
  }
  
  else if (fOnGoingBarline) {
    fCurrentBarlineHasCoda = true;
  }
}

void xml2MsrVisitor::visitStart ( S_eyeglasses& elt ) 
{
  if (fOnGoingDirectionType) {
    int inputLineNumber =
      elt->getInputLineNumber ();
      
    // fetch current voice
    S_msrVoice
      currentVoice =
        createVoiceInStaffInCurrentPartIfNeeded (
          inputLineNumber,
          fCurrentStaffNumber,
          fCurrentVoiceNumber);
  
    // create the eyeglasses
    S_msrEyeglasses
      eyeglasses =
        msrEyeglasses::create (
          fMsrOptions,
          inputLineNumber);
  
    // set the eyeglasses measure location
    eyeglasses->
      setEyeglassesMeasureLocation (
        currentVoice->getVoiceMeasureLocation ());

    // append it to the current voice
    currentVoice->
      appendEyeglassesToVoice (eyeglasses);
  }
  
  else if (fOnGoingBarline) {
    fCurrentBarlineHasCoda = true;
  }
}

void xml2MsrVisitor::visitStart ( S_pedal& elt ) 
{
  /* start-stop-change-continue */
  /*
    The pedal type represents piano pedal marks. The change and continue types are used when the line attribute is yes. The change type indicates a pedal lift and retake indicated with an inverted V marking. The continue type allows more precise formatting across system breaks and for more complex pedaling lines. The alignment attributes are ignored if the line attribute is yes.
    
    The line attribute is yes if pedal lines are used. The change and continue types are used when the line attribute is yes.
    
    The sign attribute is yes if Ped and * signs are used. For MusicXML 2.0 compatibility, the sign attribute is yes by default if the line attribute is no, and is no by default if the line attribute is yes. 
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
      fMsrOptions->fInputSourceName,
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
      fMsrOptions->fInputSourceName,
      elt->getInputLineNumber (),
      s.str());
  }

  if (fOnGoingDirectionType) {
    int inputLineNumber =
      elt->getInputLineNumber ();
      
    // fetch current voice
    S_msrVoice
      currentVoice =
        createVoiceInStaffInCurrentPartIfNeeded (
          inputLineNumber,
          fCurrentStaffNumber,
          fCurrentVoiceNumber);
  
    // create the pedal
    S_msrPedal
      pedal =
        msrPedal::create (
          fMsrOptions,
          inputLineNumber,
          pedalType,
          pedalLine);
  
    // set the pedal measure location
    pedal->
      setPedalMeasureLocation (
        currentVoice->getVoiceMeasureLocation ());

    // append it to the current voice
    currentVoice->
      appendPedalToVoice (pedal);
  }
  
  else if (fOnGoingBarline) {
    fCurrentBarlineHasCoda = true;
  }
}

//______________________________________________________________________________
void xml2MsrVisitor::visitStart ( S_ending& elt ) 
{  
  fCurrentEndingtype   = elt->getAttributeValue ("type");  
  fCurrentEndingNumber = elt->getAttributeValue ("number");  
  
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
      fMsrOptions->fInputSourceName,
      elt->getInputLineNumber (),
      s.str());
  }

  fCurrentBarlineEndingNumber =
    elt->getAttributeValue ("number"); // may be "1, 2"
}

//______________________________________________________________________________
void xml2MsrVisitor::visitStart ( S_repeat& elt ) 
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
    
    s << "repeat direction " << fCurrentRepeatDirection << " is unknown";
    
    msrMusicXMLError (
      fMsrOptions->fInputSourceName,
      inputLineNumber,
      s.str());
  }

  fCurrentBarlineRepeatWinged =
    msrBarline::k_NoRepeatWinged;

  if (fCurrentRepeatWinged.size()) {
    if       (fCurrentRepeatWinged == "straight") {
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
      
      s << "repeat winged " << fCurrentRepeatWinged << " is unknown";
      
      msrMusicXMLError (
        fMsrOptions->fInputSourceName,
        inputLineNumber,
        s.str());
    }
  }
}

//______________________________________________________________________________
void xml2MsrVisitor::visitEnd ( S_barline& elt ) 
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  // there may be a barline in a part before any music
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  // create the barline
  S_msrBarline
    barline =
      msrBarline::create (
        fMsrOptions,
        inputLineNumber,
        fCurrentBarlineHasSegno,
        fCurrentBarlineHasCoda,
        fCurrentBarlineLocation,
        fCurrentBarlineStyle,
        fCurrentBarlineEndingType,
        fCurrentBarlineEndingNumber,
        fCurrentBarlineRepeatDirection,
        fCurrentBarlineRepeatWinged);

  // set the barline measure location
  barline->
    setBarlineMeasureLocation (
      currentVoice->getVoiceMeasureLocation ());

  // don't display the barline yet in case of debug,
  // wait until its category is defined
  // to append the barline to the current voice chunk

  // handle the barline according to:
  // http://www.musicxml.com/tutorial/the-midi-compatible-part/repeats/

  bool barlineIsAlright = false;

  if (
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
    fCurrentBarlineLocation == msrBarline::kRight
      &&
    fCurrentBarlineEndingType == msrBarline::kStop
 // JMI     &&
 //   fCurrentBarlineRepeatDirection == msrBarline::kBackward
 ) {
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
         
    handleEndingEnd (elt, barline);

    barlineIsAlright = true;
  }

  else if (
    fCurrentBarlineLocation == msrBarline::kLeft
      &&
    fCurrentBarlineEndingType == msrBarline::kStart) {
    // ending start
    // ------------------------------------------------------
    handleEndingStart (elt, barline);

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
        
        // append the bar line to the current voice chunk
        currentVoice->
          appendBarlineToVoice (barline);
      
        barlineIsAlright = true;
        break;
        
      case msrBarline::kDotted:
      //---------------------------------------
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBar);
        
        // append the bar line to the current voice chunk
        currentVoice->
          appendBarlineToVoice (barline);
              
        barlineIsAlright = true;
        break;
        
      case msrBarline::kDashed:
      //---------------------------------------    
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBar);
        
        // append the bar line to the current voice chunk
        currentVoice->
          appendBarlineToVoice (barline);
      
        barlineIsAlright = true;
        break;
        
      case msrBarline::kHeavy:
      //---------------------------------------    
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBar);
        
        // append the bar line to the current voice chunk
        currentVoice->
          appendBarlineToVoice (barline);
      
        barlineIsAlright = true;
        break;
        
      case msrBarline::kLightLight:
      //---------------------------------------
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBar);
  
        // append the bar line to the current voice chunk
        currentVoice->
          appendBarlineToVoice (barline);
    
        barlineIsAlright = true;
        break;
        
      case msrBarline::kLightHeavy:
      //---------------------------------------
  
/* JMI
      if (
        fCurrentBarlineLocation == msrBarline::msrBarline::kRight) {
   //       if (fMsrOptions->fDebug)
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
        
        // append the bar line to the current voice chunk
        currentVoice->
          appendBarlineToVoice (barline);
          
        barlineIsAlright = true;        
        break;
  
      case msrBarline::kHeavyLight:
      //---------------------------------------
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBar);
        
        // append the bar line to the current voice chunk
        currentVoice->
          appendBarlineToVoice (barline);
          
        barlineIsAlright = true;
        break;
         
      case msrBarline::kHeavyHeavy:
      //---------------------------------------    
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBar);
        
        // append the bar line to the current voice chunk
        currentVoice->
          appendBarlineToVoice (barline);
  
        barlineIsAlright = true;
        break;
        
      case msrBarline::kTick:
      //---------------------------------------
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBar);
        
        // append the bar line to the current voice chunk
        currentVoice->
          appendBarlineToVoice (barline);
  
        barlineIsAlright = true;
        break;
        
      case msrBarline::kShort:
      //---------------------------------------
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBar);
        
        // append the bar line to the current voice chunk
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
     //       if (fMsrOptions->fDebug)
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
            
            // append the bar line to the current voice chunk
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
  if (fMsrOptions->fDebug) {
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
      fMsrOptions->fInputSourceName,
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
void xml2MsrVisitor::visitStart ( S_note& elt ) 
{
  //  cerr << "--> xml2MsrVisitor::visitStart ( S_note& elt ) " << endl;
  fMusicXMLNoteData.fMusicXMLStep = '_';
  fMusicXMLNoteData.fMusicXMLStepIsARest = false;
  fMusicXMLNoteData.fMusicXMLStepIsUnpitched = false;

  fMusicXMLNoteData.fMusicXMLAlter = 0.0;
  fMusicXMLNoteData.fMusicXMLAlteration = msrMusicXMLNoteData::kNatural;
  
  fMusicXMLNoteData.fMusicXMLOctave = -13;
  
  fMusicXMLNoteData.fMusicXMLDivisions  = -13;
  fMusicXMLNoteData.fMusicXMLDotsNumber = 0;
  
  fMusicXMLNoteData.fMusicXMLNoteIsAGraceNote = false;
  
  // assume this note doesn't belong to a chord until S_chord is met
  fMusicXMLNoteData.fMusicXMLNoteBelongsToAChord = false;
  
  // assume this note doesn't belong to a tuplet until S_tuplet is met
  fMusicXMLNoteData.fMusicXMLNoteBelongsToATuplet = false;
  fMusicXMLNoteData.fMusicXMLType = "";
  
  fMusicXMLNoteData.fMusicXMLTieKind =
    msrMusicXMLNoteData::k_NoTie;
  
  fMusicXMLNoteData.fMusicXMLStaffNumber = 0;
  fMusicXMLNoteData.fMusicXMLVoiceNumber = 0;

  // assuming staff number 1, unless S_staff states otherwise afterwards
  fCurrentStaffNumber = 1;

  // assuming voice number 1, unless S_voice states otherwise afterwards
  fCurrentVoiceNumber = 1;

  fCurrentSyllabic = "";
  fCurrentText = "";  
  // assume this note hasn't got lyrics until S_lyric is met
  fCurrentNoteHasLyrics = false;

  fCurrentBeam = 0;
  
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
void xml2MsrVisitor::visitStart ( S_step& elt )
{
  string step = elt->getValue();
  
  if (step.length() != 1) {
    stringstream s;
    s << "step value " << step << " should be a single letter from A to G";
    msrMusicXMLError (
      fMsrOptions->fInputSourceName,
      elt->getInputLineNumber (),
      s.str());
  }

  fMusicXMLNoteData.fMusicXMLStep = step[0];
}

void xml2MsrVisitor::visitStart ( S_alter& elt)
{
  fMusicXMLNoteData.fMusicXMLAlter = (float)(*elt);
}

void xml2MsrVisitor::visitStart ( S_octave& elt)
{
  fMusicXMLNoteData.fMusicXMLOctave = (int)(*elt);
}

void xml2MsrVisitor::visitStart ( S_duration& elt )
{
  int musicXMLduration = (int)(*elt);

  if      (fOnGoingBackup) {
  
    fCurrentBackupDuration = musicXMLduration;

  }
  
  else if (fOnGoingForward) {
  
    fCurrentForwardDuration = musicXMLduration;
    
  }
  
  else if (fOnGoingNote) {
  
    fMusicXMLNoteData.fMusicXMLDivisions = musicXMLduration;
    
    // all notes have their fMusicXMLDisplayDivisions
    // set to fMusicXMLNoteData.fMusicXMLDivision,
    // except tuplet member notes
    fMusicXMLNoteData.fMusicXMLDisplayDivisions =
      fMusicXMLNoteData.fMusicXMLDivisions;
  }
  
  else {
    
    stringstream s;
    
    s << "duration " << musicXMLduration << " is out of context";
    
 // JMI   msrMusicXMLError (s.str());
    msrMusicXMLWarning (
      fMsrOptions->fInputSourceName,
      elt->getInputLineNumber (),
      s.str());
  }
    
//  cerr << "=== xml2MsrVisitor::visitStart ( S_duration& elt ), fCurrentMusicXMLDuration = " << fCurrentMusicXMLDuration << endl; JMI
}

void xml2MsrVisitor::visitStart ( S_dot& elt )
{
  fMusicXMLNoteData.fMusicXMLDotsNumber++;
}
       
void xml2MsrVisitor::visitStart ( S_type& elt )
{
/*
 Type indicates the graphic note type, Valid values (from shortest to longest) are 1024th, 512th, 256th, 128th, 64th, 32nd, 16th, eighth, quarter, half, whole, breve, long, and maxima. The size attribute indicates full, cue, or large size, with full the default for regular notes and cue the default for cue and grace notes.
*/

  fCurrentNoteType = elt->getValue();
}

void xml2MsrVisitor::visitStart ( S_stem& elt )
{
  //         <stem default-y="28.5">up</stem>

  string        stem = elt->getValue();

  msrStem::msrStemKind stemKind;
  
  if      (stem == "up")
    stemKind = msrStem::kStemUp;
    
  else if (stem == "down")
    stemKind = msrStem::kStemDown;
    
  else {
    stringstream s;
    
    s <<
      "stem \"" << fCurrentBeamValue <<
      "\"" << "is unknown";
      
    msrMusicXMLError (
      fMsrOptions->fInputSourceName,
      elt->getInputLineNumber (),
      s.str());
  }

  fCurrentStem =
    msrStem::create (
      fMsrOptions,
      elt->getInputLineNumber (),
      stemKind);
}

void xml2MsrVisitor::visitStart ( S_beam& elt )
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
      fMsrOptions->fInputSourceName,
      inputLineNumber,
      s.str());

    beamIsOK = false;
  }
    
  if (beamIsOK) {
    fCurrentBeam =
      msrBeam::create (
        fMsrOptions,
        inputLineNumber,
        fCurrentBeamNumber,
        beamKind);
  }
}

//______________________________________________________________________________
void xml2MsrVisitor::visitStart ( S_staccato& elt )
{
  S_msrArticulation
    articulation =
      msrArticulation::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrArticulation::kStaccato);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrVisitor::visitStart ( S_staccatissimo& elt )
{
  S_msrArticulation
    articulation =
      msrArticulation::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrArticulation::kStaccatissimo);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrVisitor::visitStart ( S_fermata& elt )
{
  // type : upright inverted  (Binchois20.xml)
  S_msrArticulation
    articulation =
      msrArticulation::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrArticulation::kFermata);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrVisitor::visitStart ( S_trill_mark& elt )
{
  // type : upright inverted  (Binchois20.xml)
  S_msrArticulation
    articulation =
      msrArticulation::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrArticulation::kTrill);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrVisitor::visitStart ( S_arpeggiate& elt )
{
  S_msrArticulation
    articulation =
      msrArticulation::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrArticulation::kArpeggiato);
      
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
void xml2MsrVisitor::visitStart( S_f& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrDynamics::kF);
        
  fPendingDynamics.push_back(dyn);
}
void xml2MsrVisitor::visitStart( S_ff& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrDynamics::kFF);
        
  fPendingDynamics.push_back(dyn);
}
void xml2MsrVisitor::visitStart( S_fff& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrDynamics::kFFF);
        
  fPendingDynamics.push_back(dyn);
}
void xml2MsrVisitor::visitStart( S_ffff& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrDynamics::kFFFF);
        
  fPendingDynamics.push_back(dyn);
}
void xml2MsrVisitor::visitStart( S_fffff& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrDynamics::kFFFFF);
        
  fPendingDynamics.push_back(dyn);
}
void xml2MsrVisitor::visitStart( S_ffffff& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrDynamics::kFFFFFF);
        
  fPendingDynamics.push_back(dyn);
}

void xml2MsrVisitor::visitStart( S_p& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrDynamics::kP);
        
  fPendingDynamics.push_back(dyn);
}
void xml2MsrVisitor::visitStart( S_pp& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrDynamics::kPP);
        
  fPendingDynamics.push_back(dyn);
}
void xml2MsrVisitor::visitStart( S_ppp& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrDynamics::kPP);
        
  fPendingDynamics.push_back(dyn);
}
void xml2MsrVisitor::visitStart( S_pppp& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrDynamics::kPPPP);
        
  fPendingDynamics.push_back(dyn);
}
void xml2MsrVisitor::visitStart( S_ppppp& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrDynamics::kPPPPP);
        
  fPendingDynamics.push_back(dyn);
}
void xml2MsrVisitor::visitStart( S_pppppp& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrDynamics::kPPPPPP);
        
  fPendingDynamics.push_back(dyn);
}

void xml2MsrVisitor::visitStart( S_fp& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrDynamics::kFP);
        
  fPendingDynamics.push_back(dyn);
}
void xml2MsrVisitor::visitStart( S_fz& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrDynamics::kFZ);
        
  fPendingDynamics.push_back(dyn);
}

void xml2MsrVisitor::visitStart( S_rf& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrDynamics::kRF);
        
  fPendingDynamics.push_back(dyn);
}

void xml2MsrVisitor::visitStart( S_sf& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrDynamics::kSF);
        
  fPendingDynamics.push_back(dyn);
}

void xml2MsrVisitor::visitStart( S_rfz& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrDynamics::kRFZ);
        
  fPendingDynamics.push_back(dyn);
}

void xml2MsrVisitor::visitStart( S_sfz& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrDynamics::kSFZ);
        
  fPendingDynamics.push_back(dyn);
}

void xml2MsrVisitor::visitStart( S_sfp& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrDynamics::kSFP);
        
  fPendingDynamics.push_back(dyn);
}

void xml2MsrVisitor::visitStart( S_sfpp& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrDynamics::kSFPP);
        
  fPendingDynamics.push_back(dyn);
}

void xml2MsrVisitor::visitStart( S_sffz& elt)
{        
  S_msrDynamics
    dyn =
      msrDynamics::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        msrDynamics::kSFFZ);
        
  fPendingDynamics.push_back(dyn);
}

//______________________________________________________________________________
void xml2MsrVisitor::visitStart ( S_wedge& elt )
{
  string type = elt->getAttributeValue("type");
  
  msrWedge::msrWedgeKind wedgeKind;

  if (type == "crescendo") {
    wedgeKind = msrWedge::kCrescendoWedge;
  }
  else if (type == "diminuendo") {
    wedgeKind = msrWedge::kDecrescendoWedge;
  }
  else if (type == "stop") {
    wedgeKind = msrWedge::kStopWedge;
  }
  
  S_msrWedge
    wedge =
      msrWedge::create(
        fMsrOptions,
        elt->getInputLineNumber (),
        wedgeKind);
        
  fPendingWedges.push_back (wedge);
}
    
//______________________________________________________________________________
void xml2MsrVisitor::visitStart ( S_grace& elt )
{
  fMusicXMLNoteData.fMusicXMLNoteIsAGraceNote = true;

  string slash = elt->getAttributeValue ("slash");

  // check part group barline
  if (slash == "yes")
    fCurrentGraceIsSlashed = true;
    
  else
  if (slash == "no")
    fCurrentGraceIsSlashed = false;
    
  else {
    if (slash.size ()) {
      msrMusicXMLError (
        fMsrOptions->fInputSourceName,
        elt->getInputLineNumber (),
        "unknown grace slash \"" +
          slash +
          "\", should be 'yes' or 'no'");
    }
  }
}
       
//______________________________________________________________________________
void xml2MsrVisitor::visitStart ( S_chord& elt)
{
  fMusicXMLNoteData.fMusicXMLNoteBelongsToAChord = true;
}

//______________________________________________________________________________
void xml2MsrVisitor::visitStart ( S_time_modification& elt )
{
  // there may be no '<tuplet number="n" type="start" />'
  // in the tuplet notes after the first one,
  // so we detect tuplet notes on '<time-modification>'
  // so we detect tuplet notes on '<time-modification>'
  fMusicXMLNoteData.fMusicXMLNoteBelongsToATuplet = true;
}

void xml2MsrVisitor::visitStart ( S_actual_notes& elt )
{
  fCurrentActualNotes = (int)(*elt);
}

void xml2MsrVisitor::visitStart ( S_normal_notes& elt )
{
  fCurrentNormalNotes = (int)(*elt);
}

void xml2MsrVisitor::visitStart ( S_normal_type& elt )
{
  fCurrentNormalNoteType = elt->getValue();
}

void xml2MsrVisitor::visitStart ( S_tuplet& elt )
{
  fCurrentTupletNumber =
    elt->getAttributeIntValue ("number", 0);
    
  string tupletType =
    elt->getAttributeValue("type");
  
  /* JMI* /
  cerr <<
    "--> xml2MsrVisitor::visitStart ( S_tuplet, fCurrentTupletNumber = " <<
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
      fMsrOptions->fInputSourceName,
      elt->getInputLineNumber (),
      s.str());
  }
}

//______________________________________________________________________________
void xml2MsrVisitor::visitStart ( S_rest& elt)
{
  /*
        <note>
        <rest/>
        <duration>24</duration>
        <voice>1</voice>
      </note>
*/
  //  cerr << "--> xml2MsrVisitor::visitStart ( S_rest& elt ) " << endl;
  fMusicXMLNoteData.fMusicXMLStepIsARest = true;
}

//______________________________________________________________________________
void xml2MsrVisitor::visitStart ( S_display_step& elt)
{
  string displayStep = elt->getValue();
  
  if (displayStep.length() != 1) {
    stringstream s;
    
    s <<
      "sdisplay step value " << displayStep <<
      " should be a single letter from A to G";
      
    msrMusicXMLError (
      fMsrOptions->fInputSourceName,
      elt->getInputLineNumber (),
      s.str());
  }

  fDisplayStep = displayStep[0];
}

void xml2MsrVisitor::visitStart ( S_display_octave& elt)
{
  fDisplayOctave = (int)(*elt);
}

void xml2MsrVisitor::visitEnd ( S_unpitched& elt)
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
S_msrChord xml2MsrVisitor::createChordFromItsFirstNote (
  S_msrNote firstNote)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> creating a chord on its 2nd note" <<
      firstNote <<
      endl;
  
  // firstNote has been registered standalone in the part element sequence,
  // but it is actually the first note of a chord
  
  // create a chord
  S_msrChord
    chord =
      msrChord::create (
        fMsrOptions,
        firstNote->getInputLineNumber (),
        firstNote->getNoteMusicXMLDivisions ());

  // chord's divisions per whole note is that of its first note
  chord->
    setDivisionsPerWholeNote (
      firstNote-> getDivisionsPerWholeNote ());
  
  // chord's location is that of its first note
  chord->
    setChordMeasureLocation (
      firstNote->getNoteMeasureLocation ());

  // chord's tie kind is that of its first note
  chord->
    setChordTieKind (
      firstNote->getNoteTieKind ());
  
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> adding first note " << firstNote->notePitchAsString() <<
      " to new chord" << endl;
    
  // register firstNote as a member of chord
  chord->
    addNoteToChord (firstNote);

  // copy firstNote's articulations if any to the chord
  copyNoteArticulationsToChord (firstNote, chord);
  
  // copy firstNote's dynamics if any to the chord
  copyNoteDynamicsToChord (firstNote, chord);
  
  // copy firstNote's words if any to the chord
  copyNoteWordsToChord (firstNote, chord);
  
  // copy firstNote's wedges if any to the chord
  copyNoteWedgesToChord (firstNote, chord);
  
/* JMI ???
  // move the firstNote's dynamics if any from the first note to the chord
  moveNoteDynamicsToChord (firstNote, chord);
  
  // move the firstNote's words if any from the first note to the chord
  moveNoteWordsToChord (firstNote, chord);
  
  // move the firstNote's wedges if any from the first note to the chord
  moveNoteWedgesToChord (firstNote, chord);
*/
  return chord;
}

//______________________________________________________________________________
void xml2MsrVisitor::copyNoteArticulationsToChord (
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

    // JMI   if (fMsrOptions->fDebug)
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
void xml2MsrVisitor::copyNoteDynamicsToChord (
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

    // JMI   if (fMsrOptions->fDebug)
      cerr << idtr <<
        "--> copying articulation '" <<
        (*i)->dynamicsKindAsString () <<
        "' from note " << note->noteAsString () <<
        " to chord" <<
        endl;

    chord->addDynamicsToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void xml2MsrVisitor::copyNoteWedgesToChord (
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

    // JMI   if (fMsrOptions->fDebug)
      cerr << idtr <<
        "--> copying articulation '" <<
        (*i)->wedgeKindAsString () <<
        "' from note " << note->noteAsString () <<
        " to chord" <<
        endl;

    chord->addWedgeToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void xml2MsrVisitor::copyNoteWordsToChord (
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

    // JMI   if (fMsrOptions->fDebug)
      cerr << idtr <<
        "--> copying articulation '" <<
        (*i)->wordsAsString () <<
        "' from note " << note->noteAsString () <<
        " to chord" <<
        endl;

    chord->addWordsToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void xml2MsrVisitor::moveNoteDynamicsToChord (
  S_msrNote note, S_msrChord chord)
{
  // move note's dynamics if any from the first note to chord

  // don't use a local copy of the dynamics list,
  // but access it directly instead
  
  if (! note->getNoteDynamicsToModify ().empty ()) {
 // JMI   if (fMsrOptions->fDebug)
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
void xml2MsrVisitor::moveNoteWordsToChord (
  S_msrNote note, S_msrChord chord)
{
  // move note's words if any from the first note to chord

  // don't use a local copy of the words list,
  // but access it directly instead
  
  if (! note->getNoteWordsToModify ().empty()) {
 // JMI   if (fMsrOptions->fDebug)
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
void xml2MsrVisitor::moveNoteWedgesToChord (
  S_msrNote note, S_msrChord chord)
{
  // move note's wedges if any from the first note to chord

  // don't use a local copy of the wedges list,
  // but access it directly instead
      
  if (! note->getNoteWedgesToModify ().empty ()) {
 // JMI   if (fMsrOptions->fDebug)
      cerr << idtr <<
        "--> moving wedges from current note to chord" << endl;
        
    while (! note->getNoteWedgesToModify ().empty ()) {
      S_msrWedge
        wdg = note->getNoteWedgesToModify ().front ();
        
      chord->addWedgeToChord (wdg);
      note->getNoteWedgesToModify ().pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void xml2MsrVisitor::createTupletWithItsFirstNote (S_msrNote firstNote)
{
  // firstNote is the first tuplet note,
  // and is currently at the end of the voice

  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "xml2MsrVisitor::createTupletWithItsFirstNote " <<
      firstNote <<
      endl;

  // create a tuplet
  S_msrTuplet
    tuplet =
      msrTuplet::create (
        fMsrOptions,
        firstNote->getInputLineNumber (),
        fCurrentTupletNumber,
        fCurrentActualNotes,
        fCurrentNormalNotes,
        firstNote);

  // tuplets's divisions per whole note is that of its first note
  tuplet->
    setDivisionsPerWholeNote (
      firstNote-> getDivisionsPerWholeNote ());
  
  // register it in this visitor
//  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "++> pushing tuplet " <<
      tuplet->getActualNotes () <<
      "/" <<
      tuplet->getNormalNotes () <<
      " to tuplets stack" << endl;
  fTupletsStack.push (tuplet);

  // add note as first note of the stack top tuplet
//JMI  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> adding first note " << firstNote->notePitchAsString() <<
      " to the " <<
      fTupletsStack.top ()->getActualNotes () <<
       "/" <<
      fTupletsStack.top ()->getNormalNotes () <<
      " tuplet" <<
      endl;
  tuplet->addElementToTuplet (firstNote);

/*
  // set note display divisions
  firstNote->
    applyTupletMemberDisplayFactor (
      fCurrentActualNotes, fCurrentNormalNotes);
  */
}

//______________________________________________________________________________
void xml2MsrVisitor::finalizeTuplet (S_msrNote lastNote)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "xml2MsrVisitor::finalizeTuplet " <<
      lastNote <<
      endl;
      
  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
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
//  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "==> adding last note " << lastNote->notePitchAsString () <<
      " to tuplets stack top " <<
      fTupletsStack.top ()->getActualNotes () <<
       "/" <<
      fTupletsStack.top ()->getNormalNotes () <<
      endl;
  tuplet->addElementToTuplet (lastNote);

  // pop from the tuplets stack
//  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> popping tuplet " <<
      fTupletsStack.top ()->getActualNotes () <<
       "/" <<
      fTupletsStack.top ()->getNormalNotes () <<
      " from tuplets stack" << endl;
  fTupletsStack.pop ();        

  if (fTupletsStack.size ()) {
    // tuplet is an embedded tuplet
//    if (fMsrOptions->fDebug)
      cerr << idtr <<
        "=== adding embedded tuplet " <<
      tuplet->getActualNotes () <<
       "/" <<
      tuplet->getNormalNotes () <<
        " to " <<
      fTupletsStack.top ()->getActualNotes () <<
       "/" <<
      fTupletsStack.top ()->getNormalNotes () <<
      " current stack top tuplet" << endl;
    
    fTupletsStack.top ()->
      addElementToTuplet (tuplet);
  }
  
  else {
    // tup is a top level tuplet
//    if (fMsrOptions->fDebug)
      cerr << idtr <<
        "=== adding top level tuplet " <<
      tuplet->getActualNotes () <<
       "/" <<
      tuplet->getNormalNotes () <<
      " to voice" <<
      currentVoice->getVoiceName () <<
      endl;
      
    currentVoice->
      appendTupletToVoice (tuplet);
  }  
}          

//______________________________________________________________________________
void xml2MsrVisitor::attachCurrentArticulationsToNote (
  S_msrNote note)
{
  /*
  list<S_msrArticulation>::const_iterator i;
  for (
    i=fCurrentArticulations.begin();
    i!=fCurrentArticulations.end();
    i++) {

    note->
      addArticulationToNote ((*i));
    } // for
  */

  // attach the current articulations if any to the note
  while (! fCurrentArticulations.empty()) {
    S_msrArticulation
      art =
        fCurrentArticulations.front();
        
    if (fMsrOptions->fDebug)
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

//______________________________________________________________________________
void xml2MsrVisitor::attachCurrentArticulationsToChord (
  S_msrChord chord)
{
  list<S_msrArticulation>::const_iterator i;
  for (
    i=fCurrentArticulations.begin();
    i!=fCurrentArticulations.end();
    i++) {
    if (fMsrOptions->fDebug)
      cerr << idtr <<
        "--> attaching articulation " <<  (*i) << " to chord " <<
        chord <<
        endl;
        
    chord->
      addArticulationToChord ((*i));
    } // for
/*  
  // attach the current articulations if any to the note
  while (! fCurrentArticulations.empty()) {
    S_msrArticulation
      art =
        fCurrentArticulations.front();
        
    chord->
      addArticulationToChord (art);
// JMI    fCurrentArticulations.pop_front();
  } // while
  */
}

//______________________________________________________________________________
void xml2MsrVisitor::attachPendingDynamicsToNote (
  S_msrNote note)
{
  // attach the pending dynamics if any to the note
  if (! fPendingDynamics.empty()) {
    
    if (fMusicXMLNoteData.fMusicXMLStepIsARest) {
      if (fMsrOptions->fDelayRestsDynamics) {
        cerr << idtr <<
          "--> Delaying dynamics attached to a rest until next note" << endl;
      }
      else {
        msrMusicXMLWarning (
          fMsrOptions->fInputSourceName,
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
void xml2MsrVisitor::attachPendingWordsToNote (
  S_msrNote note)
{
  // attach the pending words if any to the note
  if (! fPendingWords.empty ()) {
    
    if (fMusicXMLNoteData.fMusicXMLStepIsARest) {
      if (fMsrOptions->fDelayRestsDynamics) {
        cerr << idtr <<
          "--> Delaying words attached to a rest until next note" << endl;
      }
      else {
        for (
            list<S_msrWords>::const_iterator i = fPendingWords.begin();
            i != fPendingWords.end();
            i++) {
          msrMusicXMLWarning (
            fMsrOptions->fInputSourceName,
            (*i)->getInputLineNumber (),
            "there is words attached to a rest");
        } // for
      }
    }
    
    else {
      while (! fPendingWords.empty ()) {
        S_msrWords
          wdg =
            fPendingWords.front ();
            
        note->addWordsToNote (wdg);
        fPendingWords.pop_front ();
      } // while
    }
  }
}

//______________________________________________________________________________
void xml2MsrVisitor::attachPendingWedgesToNote (
  S_msrNote note)
{
  // attach the pending wedges if any to the note
  if (! fPendingWedges.empty ()) {
    
    if (fMusicXMLNoteData.fMusicXMLStepIsARest) {
      if (fMsrOptions->fDelayRestsDynamics) {
        cerr << idtr <<
          "--> Delaying wedge attached to a rest until next note" << endl;
      }
      else {
        for (
            list<S_msrWedge>::const_iterator i = fPendingWedges.begin();
            i != fPendingWedges.end();
            i++) {
          msrMusicXMLWarning (
            fMsrOptions->fInputSourceName,
            (*i)->getInputLineNumber (),
            "there is a wedge attached to a rest");
        } // for
      }
    }
    
    else {
      while (! fPendingWedges.empty ()) {
        S_msrWedge
          wdg =
            fPendingWedges.front ();
            
        note->addWedgeToNote (wdg);
        fPendingWedges.pop_front ();
      } // while
    }
  }
}

//______________________________________________________________________________
void xml2MsrVisitor::attachPendingDynamicsToChord (
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
    
void xml2MsrVisitor::attachPendingWordsToChord (
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
  
void xml2MsrVisitor::attachPendingWedgesToChord (
  S_msrChord chord)
{
  // attach the pending wedges if any to the chord
  if (! fPendingWedges.empty ()) {
    while (! fPendingWedges.empty ()) {
      S_msrWedge
        wdg =
          fPendingWedges.front ();
          
      chord->addWedgeToChord (wdg);
      fPendingWedges.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void xml2MsrVisitor::visitEnd ( S_note& elt )
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
  
  int inputLineNumber =
    elt->getInputLineNumber ();

/*
  // fetch current staff
  S_msrStaff
    staff =
      createStaffInCurrentPartIfNeeded (
        inputLineNumber, fCurrentNoteStaffNumber);
*/
  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentNoteStaffNumber,
        fCurrentNoteVoiceNumber);

// JMI  if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
  if (fMsrOptions->fDebug) {
    cerr <<
      idtr <<
      "!!!! BEFORE visitEnd (S_note&) we have:" << endl <<
      idtr << idtr <<
        "--> fCurrentNoteStaffNumber = " <<
        fCurrentNoteStaffNumber << endl <<
      idtr << idtr <<
        "--> fCurrentNoteVoiceNumber = " <<
        fCurrentNoteVoiceNumber <<
      idtr << idtr <<
        "--> current voice  = \"" <<
        currentVoice->getVoiceName () << "\"" <<
        endl <<
      endl;
  }

  // store voice and staff numbers in MusicXML note data
  fMusicXMLNoteData.fMusicXMLStaffNumber = fCurrentStaffNumber;
  fMusicXMLNoteData.fMusicXMLVoiceNumber = fCurrentVoiceNumber;

  // set note's divisions per whole note
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "fMusicXMLNoteData.fMusicXMLDivisions = " << 
      fMusicXMLNoteData.fMusicXMLDivisions << ", " << 
      "fCurrentDivisionsPerQuarterNote * 4 = " <<
      fCurrentDivisionsPerQuarterNote * 4 << endl;
      
  currentVoice->setDivisionsPerWholeNote (
    fCurrentDivisionsPerQuarterNote * 4);

  // register current note type
  fMusicXMLNoteData.fMusicXMLType =
    fCurrentNoteType;
  
  // create the note
  S_msrNote
    note =
      msrNote::createFromMusicXMLData (
        fMsrOptions,
        inputLineNumber,
        fMusicXMLNoteData,
        fCurrentSlurKind);

  // set note's divisions per whole note
  note->
    setDivisionsPerWholeNote (
      currentVoice-> getDivisionsPerWholeNote ());
  
  // set its stem if any
  if (fCurrentStem)
    note->
      setStem (fCurrentStem);

  // set its beam if any
  if (fCurrentBeam)
    note->
      setBeam (fCurrentBeam);

  // set its location
  note->setNoteMeasureLocation (
    currentVoice->getVoiceMeasureLocation ());

  // attach the articulations if any to the note
  attachCurrentArticulationsToNote (note);

  // take it's duration into account
  currentVoice->incrementPositionInMeasure (
    fMusicXMLNoteData.fMusicXMLDivisions);

  /*
  A rest can be standalone or belong to a tuplet

  A note can be standalone or a member of a chord
  and the latter can belong to a tuplet,
  hence a note of a chord inside a tuplet is to be
  displayed as a note in a tuplet but outside of chord
  */

  // are the display divisions different than the duration?
  if (fMusicXMLNoteData.fMusicXMLNoteBelongsToATuplet)
    // set tuplet member note display divisions
    note->
      applyTupletMemberDisplayFactor (
        fCurrentActualNotes, fCurrentNormalNotes);

  // handle note
  if (fMusicXMLNoteData.fMusicXMLNoteBelongsToAChord) {
    // chord member note
    handleNoteBelongingToAChord (note);
  }
  
  else if (fMusicXMLNoteData.fMusicXMLNoteBelongsToATuplet) {
    // tuplet member note
    handleNoteBelongingToATuplet (note);
  }
  
  else {
    // standalone or grace note or rest
    handleStandaloneOrGraceNoteOrRest (note);
  }

 // JMI if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
  if (fMsrOptions->fDebug) {
    cerr <<
      idtr <<
        "!!!! AFTER visitEnd (S_note&) " <<
        note->notePitchAsString () <<
        " we have:" <<
      endl <<
      idtr << idtr <<
        "--> fCurrentNoteStaffNumber = " <<
        fCurrentNoteStaffNumber << endl <<
      idtr << idtr <<
        "--> fCurrentNoteVoiceNumber = " <<
        fCurrentNoteVoiceNumber <<
      idtr << idtr <<
        "--> current voice  = \"" <<
        currentVoice->getVoiceName () << "\"" <<
        endl <<
      endl;
  }

  // keep track of current note
  // in case it is the first note of a chord
  fLastHandledNote = note;
    
  fOnGoingNote = false;
}

//______________________________________________________________________________
void xml2MsrVisitor::handleNoteBelongingToAChord (
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

  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "xml2MsrVisitor::handleNoteBelongingToAChord " <<
      newNote <<
      endl;
      
  if (fMusicXMLNoteData.fMusicXMLStepIsARest)
    msrMusicXMLError (
      fMsrOptions->fInputSourceName,
      newNote->getInputLineNumber (),
      "a rest cannot belong to a chord");

  newNote->
    setNoteKind (msrNote::kChordMemberNote);


  // should a chord be created?
  if (! fOnGoingChord) {
    // create a chord with fLastHandledNote
    // (i.e the note preceding newNote)
    // as its first note
    fCurrentChord =
      createChordFromItsFirstNote (fLastHandledNote);

    // account for chord being built
    fOnGoingChord = true;
  }


  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> adding new note " <<
      newNote->notePitchAsString() <<
      " to current chord" << endl;
    
  // register note as a member of fCurrentChord
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> registering new note " <<
      newNote->notePitchAsString() <<
      " as a member of current chord" << endl;
  fCurrentChord->
    addNoteToChord (newNote);
    
  // a chord member's measure location is that of the chord
  newNote->
    setNoteMeasureLocation (
      fCurrentChord->getChordMeasureLocation ());

  // copy newNote's articulations if any to the chord
  copyNoteArticulationsToChord (newNote, fCurrentChord);

  // copy newNote's dynamics if any to the chord
  copyNoteDynamicsToChord (newNote, fCurrentChord);

  // copy newNote's words if any to the chord
  copyNoteWordsToChord (newNote, fCurrentChord);

  // copy newNote's wedges if any to the chord
  copyNoteWedgesToChord (newNote, fCurrentChord);

/* JMI ???
  // attach the pending dynamics, if any, to the chord
  attachPendingDynamicsToChord (fCurrentChord);

  // attach the pending words, if any, to the chord
  attachPendingWordsToChord (fCurrentChord);

  // attach the pending wedges, if any, to the chord
  attachPendingWedgesToChord (fCurrentChord);

  // move newNote's dynamics if any from the first note to the chord
  moveNoteDynamicsToChord (newNote, fCurrentChord);
  
  // move newNote's words if any from the first note to the chord
  moveNoteWordsToChord (newNote, fCurrentChord);
  
  // move newNote's wedges if any from the first note to the chord
  moveNoteWedgesToChord (newNote, fCurrentChord);
*/
  
  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        newNote->getInputLineNumber (),
        fCurrentNoteStaffNumber,
        fCurrentVoiceNumber);

  // substract it's duration from the current measure location
  currentVoice->incrementPositionInMeasure (
    - newNote-> getNoteMusicXMLDivisions ());

  // remove previous current note or the previous state of the chord JMI ???
  // from the current voice sequence
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> removing last element " <<
      " from voice " << currentVoice->getVoiceName () <<
      endl;
      
  currentVoice->
    removeLastElementFromVoice ();

  // add fCurrentChord to the part sequence instead
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> appending chord " << fCurrentChord <<
      " to current voice" << endl;
      
  currentVoice->
    appendChordToVoice (fCurrentChord);
}

//______________________________________________________________________________
void xml2MsrVisitor::handleNoteBelongingToATuplet (
  S_msrNote note)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "xml2MsrVisitor::handleNoteBelongingToATuplet " <<
      note <<
      endl;

  // register note as a tuplet member
  note->
    setNoteKind (msrNote::kTupletMemberNote);

  // attach the pending dynamics, if any, to the note
  attachPendingDynamicsToNote (note);

  // attach the pending words, if any, to the note
  attachPendingWordsToNote (note);

  // attach the pending dynamics and wedges, if any, to the note
  attachPendingWedgesToNote (note);

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
        // populate the tuplet at the top of the stack
        if (fMsrOptions->fDebug)
          cerr << idtr <<
            "--> adding note " << note <<
            " to stack top tuplet " <<
            fTupletsStack.top ()->getActualNotes () <<
             "/" <<
            fTupletsStack.top ()->getNormalNotes () <<
            endl;

        fTupletsStack.top()->
          addElementToTuplet (note);
/*
        // set note display divisions
        note->
          applyTupletMemberDisplayFactor (
            fTupletsStack.top ()->getActualNotes (),
            fTupletsStack.top ()->getNormalNotes ());
*/
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
void xml2MsrVisitor::handleStandaloneOrGraceNoteOrRest (
  S_msrNote newNote)
{
  if (fMsrOptions->fDebugDebug)
    cerr << idtr <<
      "xml2MsrVisitor::handleStandaloneOrGraceNoteOrRest " <<
      newNote <<
      endl;

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        newNote->getInputLineNumber (),
        fCurrentNoteStaffNumber,
        fCurrentVoiceNumber);
    
  if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
    cerr <<
      endl <<
      idtr <<
        "--> handleStandaloneOrGraceNoteOrRest() on " <<
        newNote->notePitchAsString () <<
        ":" << newNote->getNoteMusicXMLDivisions () <<
        " in voice " <<
        currentVoice->getVoiceName () <<
        endl <<
      idtr <<
        "--> line = " <<
        newNote->getInputLineNumber () <<
        endl <<
      idtr <<
        "--> fMusicXMLNoteData.fMusicXMLNoteIsAGraceNote = " <<
        fMusicXMLNoteData.fMusicXMLNoteIsAGraceNote <<
        endl <<
      idtr <<
        "--> fCurrentGraceexpression = ";
        
    if (fCurrentGraceexpression)
      cerr << fCurrentGraceexpression;
    else
      cerr << "NULL";

    cerr <<
      endl <<
    endl;
  }

  if (fMusicXMLNoteData.fMusicXMLNoteIsAGraceNote) {
    newNote->
      setNoteKind (msrNote::kGraceNote);
  
    if (! fCurrentGraceexpression) {
      // this is the first grace note in a grace expression

      // create the grace expression
      if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
        cerr <<  idtr <<
          "--> creating a grace expression for note " <<
          newNote->notePitchAsString () <<
          ":" << newNote->getNoteMusicXMLDivisions () <<
          " in voice " <<
          currentVoice->getVoiceName () <<
          endl;
      }

      // create grace expression
      fCurrentGraceexpression =
        msrGraceexpression::create (
          fMsrOptions, 
          newNote->getInputLineNumber (),
          fCurrentGraceIsSlashed);

      // append it to the current voice
      currentVoice->
        appendGraceexpressionToVoice (
          fCurrentGraceexpression);
    }

    // append newNote to the current grace expression
    if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
      cerr <<  idtr <<
        "--> appending note " <<
        newNote->notePitchAsString () <<
        ":" << newNote->getNoteMusicXMLDivisions () <<
        " to the grace expression in voice " <<
        currentVoice->getVoiceName () <<
        endl;
    }

    fCurrentGraceexpression->
      appendNoteToGraceexpression (newNote);
  }

  else {
    // standalone note or rest

    if (fCurrentGraceexpression)
      // this is the first note after the grace expression,
      // forget about the latter
      fCurrentGraceexpression = 0;

    if (fMusicXMLNoteData.fMusicXMLStepIsARest)
      newNote->
        setNoteKind (msrNote::kRestNote);
    else
      newNote->
        setNoteKind (msrNote::kStandaloneNote);
  
    // register note/rest as standalone
    if (fMsrOptions->fForceDebug || fMsrOptions->fDebugDebug) {
      cerr <<  idtr <<
        "--> adding standalone " <<
        newNote->notePitchAsString () <<
        ":" << newNote->getNoteMusicXMLDivisions () <<
        " to voice " <<
        currentVoice->getVoiceName () <<
        endl;
    }
    
    // attach the pending dynamics, if any, to the note
    attachPendingDynamicsToNote (newNote);

    // attach the pending words, if any, to the note
    attachPendingWordsToNote (newNote);

    // attach the pending dynamics and wedges, if any, to the note
    attachPendingWedgesToNote (newNote);
  
    // append newNote to the current voice
    currentVoice->
      appendNoteToVoice (newNote);
  }

  // handle the pending tuplets
  handleTupletsPendingOnTupletStack ();

  // lyrics has to be handled in all cases to handle melismata
  handleLyrics (newNote);

  // account for chord not being built
  fOnGoingChord = false;
}

//______________________________________________________________________________
void xml2MsrVisitor::handleTupletsPendingOnTupletStack ()
{
  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        -111, // JMI ??? inputLineNumber,
        fCurrentNoteStaffNumber,
        fCurrentVoiceNumber);

  // handle tuplets pending on the tuplet stack
  while (fTupletsStack.size ()) {
    S_msrTuplet
      pendingTuplet =
        fTupletsStack.top ();
        
    // pop it from the tuplets stack

//  if (fMsrOptions->fDebug)
      cerr << idtr <<
        "--> popping tuplet " <<
        pendingTuplet->getActualNotes () <<
         "/" <<
        pendingTuplet->getNormalNotes () <<
        " from tuplets stack" << endl;
      fTupletsStack.pop ();        

    if (fTupletsStack.size ()) {
      // tuplet is an embedded tuplet
  //    if (fMsrOptions->fDebug)
        cerr << idtr <<
          "=== adding embedded tuplet " <<
        pendingTuplet->getActualNotes () <<
         "/" <<
        pendingTuplet->getNormalNotes () <<
          " to " <<
        fTupletsStack.top ()->getActualNotes () <<
         "/" <<
        fTupletsStack.top ()->getNormalNotes () <<
        " current stack top tuplet" << endl;
      
      fTupletsStack.top ()->
        addElementToTuplet (pendingTuplet);
    }
    else {
      // tup is a top level tuplet
  //    if (fMsrOptions->fDebug)
        cerr << idtr <<
          "=== adding top level tuplet " <<
        pendingTuplet->getActualNotes () <<
         "/" <<
        pendingTuplet->getNormalNotes () <<
        " to voice" <<
        currentVoice->getVoiceName () <<
        endl;
        
      currentVoice->
        appendTupletToVoice (pendingTuplet);
    }  
  } // while
}

//______________________________________________________________________________
void xml2MsrVisitor::handleLyrics (S_msrNote newNote)
{
  int inputLineNumber =
    newNote->getInputLineNumber ();
     
 // JMI if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
  if (fMsrOptions->fDebug) {
    
    cerr <<
      endl <<
      idtr <<
        "Handling lyrics" <<
        ", line = " << inputLineNumber <<
        " with:" << endl <<
        fMusicXMLNoteData;

    idtr++;
    
    cerr <<
      idtr <<
        setw(38) << "fCurrentText" << " = \"" << fCurrentText <<
        "\":" << fMusicXMLNoteData.fMusicXMLDivisions << ", " << endl <<
      idtr <<
        setw(38) << "fCurrentElision" << " = " << fCurrentElision << endl <<
      idtr <<
        setw(38) << "fMusicXMLNoteData.fMusicXMLStepIsARest" << " = ";
    if (fMusicXMLNoteData.fMusicXMLStepIsARest)
      cerr << "true";
    else
      cerr << "false";
    cerr << endl;

    cerr <<
      idtr <<
        setw(38) << "fMusicXMLNoteData.fMusicXMLTieKind" << " = " <<
        fMusicXMLNoteData.musicXMLTieKindAsString () <<
      endl;
        
    cerr <<
      idtr <<
        setw(38) << "fCurrentSlurKind" << " = \"";
    switch (fCurrentSlurKind) {
      case msrSlur::kStartSlur:
        cerr << "start";
        break;
      case msrSlur::kContinueSlur:
        cerr << "start";
        break;
      case msrSlur::kStopSlur:
        cerr << "start";
        break;
      case msrSlur::k_NoSlur:
        cerr << "NO_SLUR";
        break;
    } // switch
    cerr << "\"" << endl;

    cerr <<
      idtr <<
        setw(38) <<
        "fOnGoingSlur" << " = " << fOnGoingSlur <<
        endl <<
      idtr <<
        setw(38) <<
        "fOnGoingSlurHasLyrics" << " = " << fOnGoingSlurHasLyrics <<
        endl;

    cerr <<
      idtr <<
        setw(38) << "fFirstLyricschunkInSlurKind" << " = \"" <<
        fFirstLyricschunkInSlurKind << // JMI->lyricschunkKindAsString () <<
        "\"" << endl;

    cerr <<
      idtr <<
        setw(38) << "fCurrentLyricschunkKind" << " = \""<<
        fCurrentLyricschunkKind << // JMI ->lyricschunkKindAsString () <<
        "\"" << endl;
        
    idtr--;
  }

  if      (fCurrentSyllabic == "single")
    fCurrentLyricschunkKind = msrLyricschunk::kSingleChunk;
  else if (fCurrentSyllabic == "begin")
    fCurrentLyricschunkKind = msrLyricschunk::kBeginChunk;
  else if (fCurrentSyllabic == "middle")
    fCurrentLyricschunkKind = msrLyricschunk::kMiddleChunk;
  else if (fCurrentSyllabic == "end")
    fCurrentLyricschunkKind = msrLyricschunk::kEndChunk;
  else {
    // no <syllabic /> specified for this note
    fCurrentLyricschunkKind = msrLyricschunk::k_NoChunk;
  }

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        newNote->getInputLineNumber (),
        fCurrentNoteStaffNumber,
        fCurrentVoiceNumber);

  if (fCurrentLyricschunkKind != msrLyricschunk::k_NoChunk) {

 //   string lyricschunkKindAsString;
    
    if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
      /*
      cerr <<
        ", type = \"" << lyricschunkKindAsString << "\"" <<
        ", elision: " << fCurrentElision << 
        " to " << getLyricsName () << endl;
*/
    }
      
    currentVoice->
      addTextLyricschunkToVoice (
        fCurrentLyricsNumber,
        fCurrentSyllabic,
        fCurrentLyricschunkKind,
        fCurrentText,
        fCurrentElision,
        fMusicXMLNoteData.fMusicXMLDivisions,
        newNote);

    if (fOnGoingSlur)
      fOnGoingSlurHasLyrics = true;
      
    fCurrentNoteHasLyrics = true;
  }
  
  else {

    if (
      fCurrentSlurKind == msrSlur::kStartSlur
        &&
      fCurrentNoteHasLyrics) { // JMI
    }
    
    if (
      fMusicXMLNoteData.fMusicXMLTieKind
        !=
      msrMusicXMLNoteData::k_NoTie) {
      fCurrentLyricschunkKind = msrLyricschunk::kTiedChunk;
      
      currentVoice->
        addTiedLyricschunkToVoice (
          fCurrentLyricsNumber,
          fMusicXMLNoteData.fMusicXMLDivisions,
          newNote);
    }
  
    else if (fMusicXMLNoteData.fMusicXMLStepIsARest) {
      fCurrentLyricschunkKind = msrLyricschunk::kSkipChunk;

      currentVoice->
        addSkipLyricschunkToVoice (
          fCurrentLyricsNumber,
          fMusicXMLNoteData.fMusicXMLDivisions,
          newNote);
    }
  
    else if (
      fOnGoingSlurHasLyrics
        &&
      ! fCurrentText.size ()) {
      if (fFirstLyricschunkInSlurKind == msrLyricschunk::kEndChunk) {
        fCurrentLyricschunkKind = msrLyricschunk::kSlurBeyondEndChunk;
  
        currentVoice->
          addSlurBeyondEndLyricschunkToVoice ( 
            fCurrentLyricsNumber,
            fMusicXMLNoteData.fMusicXMLDivisions,
            newNote);
      }
      else {        
        fCurrentLyricschunkKind = msrLyricschunk::kSlurChunk;
  
        currentVoice->
          addSlurLyricschunkToVoice ( 
            fCurrentLyricsNumber,
            fMusicXMLNoteData.fMusicXMLDivisions,
            newNote);
      }
    }
    
    else if (fOnGoingSlur) {
      if (fFirstLyricschunkInSlurKind == msrLyricschunk::kEndChunk) {
        fCurrentLyricschunkKind = msrLyricschunk::kSlurBeyondEndChunk;
  
        currentVoice->
          addSlurBeyondEndLyricschunkToVoice (
            fCurrentLyricsNumber,
            fMusicXMLNoteData.fMusicXMLDivisions,
            newNote);
      }
      else {        
        fCurrentLyricschunkKind = msrLyricschunk::kSlurChunk;
  
        currentVoice->
          addSlurLyricschunkToVoice ( 
            fCurrentLyricsNumber,
            fMusicXMLNoteData.fMusicXMLDivisions,
            newNote);
      }
    }
    
    else { // JMI
    }
  }

  if (fCurrentSlurKind == msrSlur::kStartSlur)
    fFirstLyricschunkInSlurKind = fCurrentLyricschunkKind;
    
  if (fCurrentSlurKind == msrSlur::kStopSlur)
    fFirstLyricschunkInSlurKind = msrLyricschunk::k_NoChunk;
}

//______________________________________________________________________________
void xml2MsrVisitor::handleRepeatStart (
  S_barline     elt,
  S_msrBarline& barline)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

//      if (fMsrOptions->fDebug)
    cerr <<
      idtr << "--> input line " << inputLineNumber <<
      endl <<
      idtr <<
      "--> barline, left and forward: repeat start" <<
      endl;

  // set the barline category
  barline->
    setBarlineCategory (msrBarline::kRepeatStart);

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  // get the current voice chunk
  S_msrVoicechunk
    currentVoicechunk =
      currentVoice->
        getVoicechunk ();

  if (! fCurrentRepeat) {
    // create the repeat
    if (fMsrOptions->fTrace)
      cerr << idtr <<
        "Creating a repeat in voice " <<
        currentVoice->getVoiceName () << endl;

    fCurrentRepeat =
      msrRepeat::create (
        fMsrOptions, inputLineNumber,
        currentVoicechunk,
        currentVoice);
  }
  
  // create a new voice chunk for the voice
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> setting new voice chunk for voice " <<
      currentVoice->getVoiceName () << endl;
      
  currentVoice->
    setNewVoicechunkForVoice (
      inputLineNumber);

  // append the bar line to the new current voice chunk
  currentVoice->
    appendBarlineToVoice (barline);

  // push the barline onto the stack
  fPendingBarlines.push (barline);
}

//______________________________________________________________________________
void xml2MsrVisitor::handleHookedEndingEnd (
  S_barline     elt,
  S_msrBarline& barline)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

//       if (fMsrOptions->fDebug)
    cerr <<
      idtr << "--> input line " << inputLineNumber <<
      endl <<
      idtr <<
      "--> barline right, stop and backward: hooked ending end" <<
      endl;

  // set the barline category
  barline->
    setBarlineCategory (msrBarline::kHookedEndingEnd);

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  // append the bar line to the current voice chunk
  currentVoice->
    appendBarlineToVoice (barline);

  // get the current voice chunk
  S_msrVoicechunk
    currentVoicechunk =
      currentVoice->
        getVoicechunk ();

  // create new voice chunk from current voice
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> setting new voice chunk for voice " <<
      currentVoice->getVoiceName () << endl;
      
  currentVoice->
    setNewVoicechunkForVoice (
      inputLineNumber);

  if (! fCurrentRepeat) {
    // create the repeat
    if (fMsrOptions->fTrace)
      cerr << idtr <<
        "Creating a repeat for voice " <<
        currentVoice->getVoiceName () << endl;

    fCurrentRepeat =
      msrRepeat::create (
        fMsrOptions, inputLineNumber,
        currentVoicechunk,
        currentVoice);
  }
    
  // create a repeat ending from the current voice chunk
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> creating a new hooked repeat ending for voice " <<
      currentVoice->getVoiceName () << endl;
      
  S_msrRepeatending
    repeatEnding =
      msrRepeatending::create (
        fMsrOptions, inputLineNumber,
        fCurrentBarlineEndingNumber,
        msrRepeatending::kHookedEnding,
        currentVoicechunk,
        fCurrentRepeat);

  // append it to the current repeat
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> appending repeat ending to current repeat in voice " <<
      currentVoice->getVoiceName () << endl;
      
  fCurrentRepeat->
    addRepeatending (repeatEnding);
  
  if (fPendingBarlines.empty ()) {
//       if (fMsrOptions->fDebug)
    cerr <<
      idtr <<
      "--> there's an implicit repeat start at the beginning of the part" <<
      endl;

    // create the implicit barline
    S_msrBarline
      implicitBarline =
        msrBarline::create (
          fMsrOptions,
          inputLineNumber,
          false, // no segno
          false, // no coda
          msrBarline::kLeft,
          msrBarline::kHeavyLight,
          msrBarline::kStart,
          fCurrentBarlineEndingNumber,
          msrBarline::kForward,
          fCurrentBarlineRepeatWinged);

    // set the implicit barline category
    implicitBarline->
      setBarlineCategory (
        msrBarline::kRepeatStart);
  
    // prepend the implicit barline to the current voice chunk
    currentVoice->
      prependBarlineToVoice (implicitBarline);
            
    // get the current voice chunk
    S_msrVoicechunk
      currentVoicechunk =
        currentVoice->
          getVoicechunk ();

    if (! fCurrentRepeat) {
      // create the repeat
      if (fMsrOptions->fTrace)
        cerr << idtr <<
          "Creating a repeat in voice " <<
          currentVoice->getVoiceName () << endl;

      fCurrentRepeat =
        msrRepeat::create (
          fMsrOptions, inputLineNumber,
          currentVoicechunk,
          currentVoice);
    }
    
    // create a new voice chunk for the voice
    if (fMsrOptions->fDebug)
      cerr << idtr <<
        "--> setting new voice chunk for voice " <<
        currentVoice->getVoiceName () << endl;
        
    currentVoice->
      setNewVoicechunkForVoice (
        inputLineNumber);

    // add the repeat to the new voice chunk
    if (fMsrOptions->fDebug)
      cerr << idtr <<
        "--> appending the repeat to voice " <<
        currentVoice->getVoiceName () << endl;

    currentVoice->
      appendRepeatToVoice (fCurrentRepeat);
  }
  
  else {
    // pop the pending barline off the stack
    fPendingBarlines.pop ();
  }
}

//______________________________________________________________________________
void xml2MsrVisitor::handleHooklessEndingEnd (
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
  
//  if (fMsrOptions->fDebug)
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
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  // append the bar line to the current voice chunk
  currentVoice->
    appendBarlineToVoice (barline);

  // get the current voice chunk
  S_msrVoicechunk
    currentVoicechunk =
      currentVoice->
        getVoicechunk ();

  // create a repeat ending from the current voice chunk
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> creating a new hookless repeat ending for voice " <<
      currentVoice->getVoiceName () << endl;
      
  S_msrRepeatending
    repeatEnding =
      msrRepeatending::create (
        fMsrOptions, inputLineNumber,
        fCurrentBarlineEndingNumber,
        msrRepeatending::kHooklessEnding,
        currentVoicechunk,
        fCurrentRepeat);

  // add the repeat ending it to the current repeat
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> appending repeat ending to current repeat in voice " <<
      currentVoice->getVoiceName () << endl;
      
  fCurrentRepeat->
    addRepeatending (repeatEnding);

  // create new voice chunk from current voice
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> setting new voice chunk for voice " <<
      currentVoice->getVoiceName () << endl;
      
  currentVoice->
    setNewVoicechunkForVoice (
      inputLineNumber);

  // add the repeat to the voice
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> appending the repeat to voice " <<
      currentVoice->getVoiceName () << endl;
      
  currentVoice->
    appendRepeatToVoice (fCurrentRepeat);

  if (fPendingBarlines.empty ()) {
//       if (fMsrOptions->fDebug)
    cerr <<
      idtr <<
      "--> there's an implicit repeat start at the beginning of the part" <<
      endl;

    // create the implicit barline
    S_msrBarline
      implicitBarline =
        msrBarline::create (
          fMsrOptions,
          inputLineNumber,
          false, // no segno
          false, // no coda
          msrBarline::kLeft,
          msrBarline::kHeavyLight,
          msrBarline::kStart,
          fCurrentBarlineEndingNumber,
          msrBarline::kForward,
          fCurrentBarlineRepeatWinged);

    // set the implicit barline category
    implicitBarline->
      setBarlineCategory (
        msrBarline::kRepeatStart);
  
    // prepend the implicit barline to the current voice chunk
    currentVoice->
      prependBarlineToVoice (implicitBarline);
            
    // get the current voice chunk
    S_msrVoicechunk
      currentVoicechunk =
        currentVoice->
          getVoicechunk ();

    if (! fCurrentRepeat) {
      // create the repeat
      if (fMsrOptions->fTrace)
        cerr << idtr <<
          "Creating a repeat in voice " <<
          currentVoice->getVoiceName () << endl;

      fCurrentRepeat =
        msrRepeat::create (
          fMsrOptions, inputLineNumber,
          currentVoicechunk,
          currentVoice);
    }
    
    // create a new voice chunk for the voice
    if (fMsrOptions->fDebug)
      cerr << idtr <<
        "--> setting new voice chunk for voice " <<
        currentVoice->getVoiceName () << endl;
        
    currentVoice->
      setNewVoicechunkForVoice (
        inputLineNumber);

    // add the repeat to the new voice chunk
    if (fMsrOptions->fDebug)
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
}

//______________________________________________________________________________
void xml2MsrVisitor::handleEndingStart (
  S_barline     elt,
  S_msrBarline& barline)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

//  if (fMsrOptions->fDebug)
    cerr <<
      idtr << "--> input line " <<
        inputLineNumber <<
      endl <<
      idtr <<
        "--> measure " <<
          barline->getBarlineMeasureLocation ().fMeasureNumber <<
        ", position " <<
          barline->getBarlineMeasureLocation ().fPositionInMeasure <<
      endl <<
      idtr <<
      "--> barline, left and start: ending start" <<
      endl;

  // set the barline category
  barline->
    setBarlineCategory (msrBarline::kEndingStart);
  
  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  // append the bar line to the current voice chunk
  currentVoice->
    appendBarlineToVoice (barline);


  // get the current voice chunk
  S_msrVoicechunk
    currentVoicechunk =
      currentVoice->
        getVoicechunk ();

  // push the barline onto the stack
  fPendingBarlines.push (barline);
}

//______________________________________________________________________________
void xml2MsrVisitor::handleEndingEnd (
  S_barline     elt,
  S_msrBarline& barline)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
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
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  // append the bar line to the current voice chunk
  currentVoice->
    appendBarlineToVoice (barline);

  if (fPendingBarlines.empty ()) {
//       if (fMsrOptions->fDebug)
    cerr <<
      idtr <<
      "--> there's an implicit repeat start at the beginning of the part" <<
      endl;

    // create the implicit barline
    S_msrBarline
      implicitBarline =
        msrBarline::create (
          fMsrOptions,
          inputLineNumber,
          false, // no segno
          false, // no coda
          msrBarline::kLeft,
          msrBarline::kHeavyLight,
          msrBarline::kStart,
          fCurrentBarlineEndingNumber,
          msrBarline::kForward,
          fCurrentBarlineRepeatWinged);

    // set the implicit barline category
    implicitBarline->
      setBarlineCategory (
        msrBarline::kRepeatStart);
  
    // prepend the implicit barline to the current voice chunk
    currentVoice->
      prependBarlineToVoice (implicitBarline);
            
    // get the current voice chunk
    S_msrVoicechunk
      currentVoicechunk =
        currentVoice->
          getVoicechunk ();

    if (! fCurrentRepeat) {
      // create the repeat
      if (fMsrOptions->fTrace)
        cerr << idtr <<
          "Creating a repeat in voice " <<
          currentVoice->getVoiceName () << endl;

      fCurrentRepeat =
        msrRepeat::create (
          fMsrOptions, inputLineNumber,
          currentVoicechunk,
          currentVoice);
    }
    
    // create a new voice chunk for the voice
    if (fMsrOptions->fDebug)
      cerr << idtr <<
        "--> setting new voice chunk for voice " <<
        currentVoice->getVoiceName () << endl;
        
    currentVoice->
      setNewVoicechunkForVoice (
        inputLineNumber);

    // add the repeat to the new voice chunk
    if (fMsrOptions->fDebug)
      cerr << idtr <<
        "--> appending the repeat to voice " <<
        currentVoice->getVoiceName () << endl;

    currentVoice->
      appendRepeatToVoice (fCurrentRepeat);

    }
    
  else {
    // pop the pending barline off the stack
    fPendingBarlines.pop ();
  }
}

//______________________________________________________________________________
void xml2MsrVisitor::visitStart ( S_rehearsal& elt )
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
        fMsrOptions->fInputSourceName,
        inputLineNumber,
        s.str());
    }    
  }

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);
    
  // create a rehearsal
 // if (fMsrOptions->fDebug)
    cerr << idtr <<
      "Creating rehearsal \"" << rehearsalValue << "\"" <<
      " in voice " <<
      currentVoice->getVoiceName () << endl;

  S_msrRehearsal
    rehearsal =
      msrRehearsal::create (
        fMsrOptions,
        inputLineNumber,
        rehearsalKind,
        rehearsalValue);

  // append the rehearsal to the current voice
  currentVoice->
    appendRehearsalToVoice (rehearsal);
}


} // namespace
