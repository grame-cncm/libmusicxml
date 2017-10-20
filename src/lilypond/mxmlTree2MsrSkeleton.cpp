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

#include "utilities.h"

#include "musicXMLOptions.h"
#include "msrOptions.h"

#include "msr.h"

#include "musicXMLTree2MsrSkeleton.h"


using namespace std;

namespace MusicXML2
{

musicXMLTree2MsrSkeleton::musicXMLTree2MsrSkeleton (
  indentedOstream& ios)
  : fLogOutputStream (ios)
{
  // the MSR score we're building
  fMsrScore =
    msrScore::create (0);

  // part groups handling
  fCurrentPartUsesImplicitPartGroup = false;
  
  fOnGoingGroupNameDisplay = false;

  // parts handling

  // staff handling
  fCurrentStaffNumber = -1;

  // voice handling
  fCurrentVoiceNumber = -1;

  // measures
  fMeasuresCounter = 0;
    
  // lyrics handling
  fOnGoingLyric = false;
  fCurrentStanzaNumber = -1; // JMI

  // harmonies handling
  fPendingHarmony                  = false;
 
  // figured bass handling
  fOnGoingFiguredBass                   = false;
  fPendingFiguredBass                   = false;
    
  // ongoing note
  fOnGoingNote = false;

  // note context
  fCurrentNoteStaffNumber = 0;
  fCurrentNoteVoiceNumber = 0;
}

musicXMLTree2MsrSkeleton::~musicXMLTree2MsrSkeleton ()
{}

//________________________________________________________________________
S_msrScore musicXMLTree2MsrSkeleton::buildMsrSkeletonFromXMLElementTree (
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
S_msrPartGroup musicXMLTree2MsrSkeleton::createImplicitMsrPartGroupIfNotYetDone (
  int inputLineNumber)
{
  /*
  A first part group is created with all the needed contents
  if none is specified in the MusicXML data.
  */

  if (! fImplicitPartGroup) {
    // create an implicit part group
    fCurrentPartGroupNumber = 1;
    
    if (gGeneralOptions->fTracePartGroups) {
      fLogOutputStream <<
        "Creating an implicit part group with number " <<
        fCurrentPartGroupNumber <<
        ", line " << inputLineNumber <<
        endl;
    }
  
    fImplicitPartGroup =
      msrPartGroup::create (
        inputLineNumber,
        fCurrentPartGroupNumber,
        "Implicit", // partGroupName
        "",         // partGroupDisplayText
        "",         // partGroupAccidentalText
        "Impl.",    // partGroupAbbreviation
        msrPartGroup::kBracketPartGroupSymbol,
        0,          // partGroupSymbolDefaultX
        true,       // partGroupBarline
        0,          // the top level part group has an empty uplink
        fMsrScore);
  
    // append it to the MSR score
    if (gGeneralOptions->fTracePartGroups) {
      fLogOutputStream <<
        "Appending implicit part group " <<
        fImplicitPartGroup->getPartGroupNumber () <<
        " to MSR score" <<
        endl;
    }
        
    fMsrScore->
      addPartGroupToScore (fImplicitPartGroup);
      
    /* JMI
      this implicit part group will be added to the MSR score
      in method 'visitEnd (S_part_list& elt)'
    */
  }

  // add implicit part group to the map of this visitor
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Adding implicit part group " << fCurrentPartGroupNumber <<
      " to visitor's data" <<
      endl;
  }
      
  fPartGroupsMap [fCurrentPartGroupNumber] = fImplicitPartGroup;
  fPartGroupsList.push_front (fImplicitPartGroup);

  fCurrentPartUsesImplicitPartGroup = true;
  
  return fImplicitPartGroup;
}

//______________________________________________________________________________
S_msrPartGroup musicXMLTree2MsrSkeleton::fetchPartGroupInThisVisitor (
  int partGroupNumber)
{
  S_msrPartGroup result;
  
  if (fPartGroupsMap.count (partGroupNumber)) {
    result =
      fPartGroupsMap [partGroupNumber];
  }

  return result;
}

//______________________________________________________________________________
S_msrStaff musicXMLTree2MsrSkeleton::createStaffInCurrentPartIfNotYetDone (
  int            inputLineNumber,
  int            staffNumber)
{    
  // is staffNumber already present in part?
  S_msrStaff
    staff =
      fCurrentPart->
        fetchStaffFromPart (staffNumber);

  if (! staff) {
    // no, add it to fCurrentPart
    staff =
      fCurrentPart->
        addStaffToPartByItsNumber (
          inputLineNumber,
          msrStaff::kRegularStaff,
          staffNumber);
  }

  return staff;
}  

//______________________________________________________________________________
S_msrVoice musicXMLTree2MsrSkeleton::createVoiceInStaffInCurrentPartIfNotYetDone (
  int inputLineNumber,
  int staffNumber,
  int voiceNumber)
{
  // the voice number is relative to a part,
  // we'll call it its part-relative ID

  // create the staff if not yet done
  S_msrStaff
    staff =
      createStaffInCurrentPartIfNotYetDone (
        inputLineNumber,
        staffNumber);

  // the voice
  S_msrVoice voice;
  
  // get registered voice staff number
  bool
    voiceNumberHasAlreadyBeenRegistered =
      fPartVoiceNumberToVoiceMap.count (
        voiceNumber);
        
  if (! voiceNumberHasAlreadyBeenRegistered) {
    // create the voice and append it to the staff
    voice =
      staff->
        createVoiceInStaffByItsPartRelativeID (
          inputLineNumber,
          msrVoice::kRegularVoice,
          voiceNumber,
          fCurrentMeasureNumber);

    // register that voice 'voiceNumber' is mapped to 'voice'
    fPartVoiceNumberToVoiceMap [voiceNumber] = voice;
    
    // register that voice 'voiceNumber' is currently displayed
    // by staff 'staffNumber'
    fPartVoiceNumberToDisplayingStaffNumberMap [voiceNumber] =
      staffNumber;
  }
  
  else {
    // the voice is the one registered for 'voiceNumber'
    voice =
      fPartVoiceNumberToVoiceMap [voiceNumber];

    // fetch registered voice displaying staff number
    int voiceDisplayingStaffNumber =
      fPartVoiceNumberToDisplayingStaffNumberMap [
        voiceNumber];

    if (staffNumber == voiceDisplayingStaffNumber) {
      // voice 'voiceNumber' remains displayed by staff 'staffNumber'
    }
    
    else {
      // voice 'voiceNumber' changes
      // from staff 'voiceDisplayingStaffNumber'
      // to staff 'staffNumber'

      if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceVoices) {
        fLogOutputStream <<
          "Voice \"" <<  voice->getVoiceName () << "\"" <<
          " changes from staff " << voiceDisplayingStaffNumber <<
          " to staff " << staffNumber <<
          endl;
      }

      // create the voice staff change
      S_msrVoiceStaffChange
        voiceStaffChange =
          msrVoiceStaffChange::create (
            inputLineNumber,
            staff);
  
      // append it to the voice
      voice->
        appendVoiceStaffChangeToVoice (
          voiceStaffChange);
  
      // register that voice 'voiceNumber' is currently displayed
      // by staff 'staffNumber'
      fPartVoiceNumberToDisplayingStaffNumberMap [voiceNumber] =
        staffNumber;
    }
  }
  
  return voice;
}  

//________________________________________________________________________
void musicXMLTree2MsrSkeleton::visitStart (S_part_list& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_part_list" <<
      endl;
  }

  if (gGeneralOptions->fTraceParts)
    fLogOutputStream <<
      "Analysing part list" <<
      endl;

  gIndenter++;
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

void musicXMLTree2MsrSkeleton::visitEnd (S_part_list& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_part_list" <<
      endl;
  }

  gIndenter--;

  if (fCurrentPartUsesImplicitPartGroup) {
    // force an implicit part group "stop" on it,
    // fCurrentPartGroupNumber holds the value 1
    handlePartGroupStop (
      elt->getInputLineNumber ());
    
 // JMI   fCurrentPartUsesImplicitPartGroup = false;
  }
}

//________________________________________________________________________
/*
    The part-group element indicates groupings of parts in the
    score, usually indicated by braces and brackets. Braces
    that are used for multi-staff parts should be defined in
    the attributes element for that part. The part-group start
    element appears before the first score-part in the group.
    The part-group stop element appears after the last
    score-part in the group.
    
    The number attribute is used to distinguish overlapping
    and nested part-groups, not the sequence of groups. As
    with parts, groups can have a name and abbreviation.
    Formatting attributes for group-name and group-abbreviation
    are deprecated in Version 2.0 in favor of the new
    group-name-display and group-abbreviation-display elements.
    Formatting specified in the group-name-display and
    group-abbreviation-display elements overrides formatting
    specified in the group-name and group-abbreviation
    elements, respectively.

    The group-symbol element indicates how the symbol for
    a group is indicated in the score. Values include none,
    brace, line, bracket, and square; the default is none.
    The group-barline element indicates if the group should
    have common barlines. Values can be yes, no, or
    Mensurstrich. The group-time element indicates that the
    displayed time signatures should stretch across all parts
    and staves in the group. Values for the child elements
    are ignored at the stop of a group.

    A part-group element is not needed for a single multi-staff
    part. By default, multi-staff parts include a brace symbol
    and (if appropriate given the bar-style) common barlines.
    The symbol formatting for a multi-staff part can be more
    fully specified using the part-symbol element, defined in
    the attributes.mod file.
*/

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
void musicXMLTree2MsrSkeleton::visitStart (S_part_group& elt)
{  
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_part_group" <<
      endl;
  }

  fCurrentPartGroupNumber =
    elt->getAttributeIntValue ("number", 0);
    
  fCurrentPartGroupType =
    elt->getAttributeValue ("type");

  fCurrentPartGroupName = "";
  fCurrentPartGroupDisplayText = "";
  fCurrentPartGroupAccidentalText = "";
  fCurrentPartGroupAbbreviation = "";
  fCurrentPartGroupSymbol = "";
  fCurrentPartGroupSymbolDefaultX = INT_MIN;
  fCurrentPartGroupBarline = "yes";
}

void musicXMLTree2MsrSkeleton::visitStart (S_group_name& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_group_name" <<
      endl;
  }

  fCurrentPartGroupName = elt->getValue();
}

void musicXMLTree2MsrSkeleton::visitStart (S_group_name_display& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_group_name_display" <<
      endl;
  }

  fOnGoingGroupNameDisplay = true;
}

void musicXMLTree2MsrSkeleton::visitStart (S_display_text& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_display_text" <<
      endl;
  }

  if (fOnGoingGroupNameDisplay)
    fCurrentPartGroupDisplayText = elt->getValue();
}

void musicXMLTree2MsrSkeleton::visitStart (S_accidental_text& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_accidental_text" <<
      endl;
  }

    fCurrentPartGroupAccidentalText = elt->getValue();
}

void musicXMLTree2MsrSkeleton::visitStart (S_group_abbreviation& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_group_abbreviation" <<
      endl;
  }

  fCurrentPartGroupAbbreviation = elt->getValue ();
}

void musicXMLTree2MsrSkeleton::visitStart (S_group_symbol& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_group_symbol" <<
      endl;
  }

  fCurrentPartGroupSymbol = elt->getValue ();

  fCurrentPartGroupSymbolDefaultX =
    elt->getAttributeIntValue ("default-x", 0);
}

void musicXMLTree2MsrSkeleton::visitStart ( S_group_barline& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_group_barline" <<
      endl;
  }

  fCurrentPartGroupBarline = elt->getValue ();
}

//________________________________________________________________________
void musicXMLTree2MsrSkeleton::showPartGroupsData (string context)
{    
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "==> " << context << ": fPartGroupsMap contains:" <<
      endl;
      
    if (fPartGroupsMap.size()) {
      map<int, S_msrPartGroup>::const_iterator
        iBegin = fPartGroupsMap.begin(),
        iEnd   = fPartGroupsMap.end(),
        i      = iBegin;
        
      gIndenter++;
      for ( ; ; ) {
        fLogOutputStream <<
          "\"" << (*i).first << "\" ----> " << (*i).second;
        if (++i == iEnd) break;
        fLogOutputStream << endl;
      } // for
      gIndenter--;
    }
    fLogOutputStream <<
      "<== fPartGroupsMap" <<
      endl <<
      endl <<
      "==> " << context << ": fPartGroupsList contains:" <<
      endl;
      
    if (fPartGroupsList.size()) {
      list<S_msrPartGroup>::const_iterator
        iBegin = fPartGroupsList.begin(),
        iEnd   = fPartGroupsList.end(),
        i      = iBegin;
        
      gIndenter++;
      for ( ; ; ) {
        fLogOutputStream << (*i);
        if (++i == iEnd) break;
        fLogOutputStream << endl;
      } // for
      gIndenter--;
    }
    fLogOutputStream <<
      "<== fPartGroupsList" <<
      endl <<
      endl;
  }
}

//________________________________________________________________________
void musicXMLTree2MsrSkeleton::handlePartGroupStart (
  int     inputLineNumber,
  msrPartGroup::msrPartGroupSymbolKind
          partGroupSymbol,
  bool    partGroupBarline)
{
  showPartGroupsData ("BEFORE START");

  // fetch part group to be started
  S_msrPartGroup
    partGroupToBeStarted =
      fetchPartGroupInThisVisitor (
        fCurrentPartGroupNumber);

  // the current part group is either null
  // or the head of the part group list
  S_msrPartGroup
    currentPartGroup =
      fPartGroupsList.size ()
        ? fPartGroupsList.front ()
        : 0;
        
  if (! partGroupToBeStarted) {
    // no, create it
    partGroupToBeStarted =
      msrPartGroup::create (
        inputLineNumber,
        fCurrentPartGroupNumber,
        fCurrentPartGroupName,
        fCurrentPartGroupDisplayText,
        fCurrentPartGroupAccidentalText,
        fCurrentPartGroupAbbreviation,
        partGroupSymbol,
        fCurrentPartGroupSymbolDefaultX,
        partGroupBarline,
        currentPartGroup,
        fMsrScore);

    // add it to the part group map of this visitor
    if (gGeneralOptions->fTracePartGroups)
      fLogOutputStream <<
        "Adding part group " << fCurrentPartGroupNumber <<
        " to visitor's part group map" <<
        endl;
    fPartGroupsMap [fCurrentPartGroupNumber] =
      partGroupToBeStarted;
  }
  
  // add it to the part group list of this visitor
  if (gGeneralOptions->fTracePartGroups)
    fLogOutputStream <<
      "Adding part group " << fCurrentPartGroupNumber <<
      " to visitor's part groups list" <<
      endl;

  if (! fPartGroupsList.size())
  
    // insert first part group ahead of the list
    fPartGroupsList.push_front (partGroupToBeStarted);
    
  else {
    
    // place in the part groups list so as
    // to have them ordered by increasing order
    // (all of them are negative)
    list<S_msrPartGroup>::const_iterator
      iBegin = fPartGroupsList.begin(),
      iEnd   = fPartGroupsList.end(),
      i      = iBegin;

    while (true) {
      if (i == iEnd) {
        fPartGroupsList.push_back (partGroupToBeStarted);
        break;
      }

      // CAUTION: insert() inserts before the position
      // indicated by its iterator argument
      if (
          fCurrentPartGroupSymbolDefaultX
            <
          (*i)->getPartGroupSymbolDefaultX ()) {
        fPartGroupsList.insert (i, partGroupToBeStarted);
        break;
      }
      
      i++;
    } // while
  }
  
  showPartGroupsData ("AFTER START");
}
  
//________________________________________________________________________
void musicXMLTree2MsrSkeleton::handlePartGroupStop (
  int inputLineNumber)
{
  showPartGroupsData ("BEFORE STOP");

  // is the part group to be stopped known?
  S_msrPartGroup
    partGroupToBeStopped =
      fetchPartGroupInThisVisitor (
        fCurrentPartGroupNumber);

  if (! partGroupToBeStopped) {
    // no, but we should have fount it
    stringstream s;

    s <<
      "part group " << fCurrentPartGroupNumber <<
      " not found in this visitor's part groups map" <<
      endl;
    msrInternalError (
      inputLineNumber,
      s.str ());
  }

  // remove the part group to be stopped from the part group list
  if (gGeneralOptions->fTracePartGroups)
    fLogOutputStream <<
      "Removing part group " <<
      partGroupToBeStopped->getPartGroupNumber () <<
      " from visitor's part groups list" <<
      ", line " << inputLineNumber <<
      endl;

  list<S_msrPartGroup>::const_iterator
    iBegin = fPartGroupsList.begin(),
    iEnd   = fPartGroupsList.end(),
    i      = iBegin;

  while (true) {
    if (i == iEnd) {
      stringstream s;
      s <<
        "part group " <<
        fCurrentPartGroupNumber <<
        " not found in part groups list";
        
      msrInternalError (
        inputLineNumber,
        s.str ());
      break;
    }

    if ((*i) == partGroupToBeStopped) {
      fPartGroupsList.erase (i);
      break;
    }
    
    i++;
  } // while

  showPartGroupsData ("AFTER REMOVAL FROM LIST");

  if (partGroupToBeStopped != fImplicitPartGroup) {
    // take care of the part group to be stopped
    // in the part groups list
    
    if (! fPartGroupsList.size()) {
      
      // we're just removed the only part group in the list:
      // append it to the MSR score
      if (gGeneralOptions->fTracePartGroups)
        fLogOutputStream <<
          "Appending part group " <<
          partGroupToBeStopped->getPartGroupNumber () <<
          " to MSR score" <<
          endl;
          
      fMsrScore->
        addPartGroupToScore (partGroupToBeStopped);
    }
  
    else {
  
      // the front element in the part group list is
      // the new current part group
      S_msrPartGroup
        newCurrentPartGroup = fPartGroupsList.front ();
  
      if (
          partGroupToBeStopped->getPartGroupNumber ()
            ==
          newCurrentPartGroup->getPartGroupNumber () ) {
        fLogOutputStream <<
          "--> partGroupToBeStopped = " << partGroupToBeStopped <<
          ", newCurrentPartGroup = " << newCurrentPartGroup <<
          endl;
  
        stringstream s;
        s <<
          "cannot append part group " <<
          partGroupToBeStopped->getPartGroupNumber () <<
          " as sub part group of itself";
        msrInternalError (
          inputLineNumber,
          s.str ());
      }
      
      // insert current group into future current group
      if (gGeneralOptions->fTracePartGroups)
        fLogOutputStream <<
          "Preending (sub-)part group " <<
          partGroupToBeStopped->getPartGroupNumber () <<
          " at the beginning of part group " <<
          newCurrentPartGroup->getPartGroupNumber () <<
          endl;
  
      newCurrentPartGroup->
        prependSubPartGroupToPartGroup (
          partGroupToBeStopped);
    }
  }

  // remove part group from the map
  // CAUTION: erase() destroys the element it removes!
  if (gGeneralOptions->fTracePartGroups)
    fLogOutputStream <<
      "Removing part group " << fCurrentPartGroupNumber <<
      " from visitor's part group map" <<
      endl;
      
  try {
    fPartGroupsMap.erase (fCurrentPartGroupNumber);
  }
  catch (int e) {
    fLogOutputStream <<
      "An exception number " << e << " occurred" <<
      endl;
  }

  showPartGroupsData ("AFTER STOP");
}

void musicXMLTree2MsrSkeleton::visitEnd (S_part_group& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_part_group" <<
      endl;
  }

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTracePartGroups)
    fLogOutputStream <<
      "Handling part group " << fCurrentPartGroupNumber <<
      ", type: \"" << fCurrentPartGroupType << "\""  <<
      endl;

  gIndenter++;
  
  msrPartGroup::msrPartGroupTypeKind partGroupTypeKind;

  // check part group type
  if      (fCurrentPartGroupType == "start")
    partGroupTypeKind = msrPartGroup::kStartPartGroupType;
    
  else if (fCurrentPartGroupType == "stop")
    partGroupTypeKind = msrPartGroup::kStopPartGroupType;
    
  else {
    if (fCurrentPartGroupType.size())
      // part group type may be absent
      msrMusicXMLError (
        inputLineNumber,
        "unknown part group type \"" + fCurrentPartGroupType + "\"");
        
    partGroupTypeKind = msrPartGroup::k_NoPartGroupType;
  }

  msrPartGroup::msrPartGroupSymbolKind partGroupSymbolKind;
  
  // check part group symbol
  // Values include none,
  //  brace, line, bracket, and square; the default is none
 
  if      (fCurrentPartGroupSymbol == "brace")
    partGroupSymbolKind = msrPartGroup::kBracePartGroupSymbol;
    
  else if (fCurrentPartGroupSymbol == "bracket")
    partGroupSymbolKind = msrPartGroup::kBracketPartGroupSymbol;
    
  else if (fCurrentPartGroupSymbol == "line")
    partGroupSymbolKind = msrPartGroup::kLinePartGroupSymbol;
    
  else if (fCurrentPartGroupSymbol == "square")
    partGroupSymbolKind = msrPartGroup::kSquarePartGroupSymbol;
    
  else if (fCurrentPartGroupSymbol == "none")
    partGroupSymbolKind = msrPartGroup::k_NoPartGroupSymbol;
    
  else {
   if (fCurrentPartGroupSymbol.size())
      // part group type may be absent
      msrMusicXMLError (
        inputLineNumber,
        "unknown part group symbol \"" + fCurrentPartGroupSymbol + "\"");
    partGroupSymbolKind = msrPartGroup::k_NoPartGroupSymbol;
  }

  bool partGroupBarline;
  
  // check part group barline
  if      (fCurrentPartGroupBarline == "yes")
    partGroupBarline = true;
    
  else if (fCurrentPartGroupBarline == "no")
    partGroupBarline = false;
    
  else {
    msrMusicXMLError (
      inputLineNumber,
      "unknown part group barline \"" + fCurrentPartGroupBarline + "\"");
    partGroupBarline = false;
  }

  switch (partGroupTypeKind) {
    
    case msrPartGroup::kStartPartGroupType:
      handlePartGroupStart (
        inputLineNumber,
        partGroupSymbolKind, partGroupBarline);
      break;
      
    case msrPartGroup::kStopPartGroupType:
      handlePartGroupStop (
        inputLineNumber);
      break;
      
    case msrPartGroup::k_NoPartGroupType:
      {}
      break;
  } // switch

  gIndenter--;
}

//________________________________________________________________________
void musicXMLTree2MsrSkeleton::visitStart (S_score_part& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_score_part" <<
      endl;
  }

  fCurrentPartID = elt->getAttributeValue ("id");

  if (gGeneralOptions->fTraceParts)
    fLogOutputStream <<
      "Found part name \"" << fCurrentPartID << "\"" <<
      endl;

  fCurrentPartName         = "";
  fCurrentPartAbbreviation = "";
  
  fCurrentPartInstrumentName = "";
  fCurrentPartInstrumentAbbreviation = "";
}

void musicXMLTree2MsrSkeleton::visitStart (S_part_name& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_part_name" <<
      endl;
  }

  fCurrentPartName = elt->getValue ();
}

void musicXMLTree2MsrSkeleton::visitStart (S_part_abbreviation& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_part_abbreviation" <<
      endl;
  }

  fCurrentPartAbbreviation = elt->getValue ();
}

void musicXMLTree2MsrSkeleton::visitStart (S_instrument_name& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_instrument_name" <<
      endl;
  }

  fCurrentPartInstrumentName = elt->getValue();
}

void musicXMLTree2MsrSkeleton::visitStart (S_instrument_abbreviation& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_instrument_abbreviation" <<
      endl;
  }

  fCurrentPartInstrumentAbbreviation = elt->getValue();
}

void musicXMLTree2MsrSkeleton::visitEnd (S_score_part& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_score_part" <<
      endl;
  }

  string partID = elt->getAttributeValue ("id");

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTraceParts)
   fLogOutputStream <<
    "--------------------------------------------" <<
    endl <<
    "Handling part \"" << fCurrentPartID << "\"" <<
    endl;

  gIndenter++;

  S_msrPartGroup partGroup;

  // is there a current part group?
  if (! fPartGroupsList.size()) {
    // no, create an implicit one if needed
    partGroup =
      createImplicitMsrPartGroupIfNotYetDone (
        inputLineNumber);
  }

  // fetch current part group
  try {
    partGroup = fPartGroupsList.front ();
  }
  catch (int e) {
    fLogOutputStream <<
      "An exception number " << e << " occurred" <<
      endl;
  }
        
  // is this part already present in the current part group?
  S_msrPart
    part =
      partGroup->
        fetchPartFromPartGroup (fCurrentPartID);

  if (! part) {
    // no, add it to the current part group
    part =
      partGroup->
        addPartToPartGroupByItsID (
          inputLineNumber,
          fCurrentPartID);
  }
  
  // populate current part
  // fPartMsrName has already been set by the constructor // JMI
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

  if (fImplicitPartGroup) {
    // force an implicit part group "stop" on it
    // fCurrentPartGroupNumber holds the value 1
    handlePartGroupStop (
      inputLineNumber);

    // forget the implicit group JNMI
  //  fImplicitPartGroup = 0;
  }

  fCurrentPartUsesImplicitPartGroup = false;

  showPartGroupsData (
    "AFTER handling part \"" + partID + "\"");
  gIndenter--;
}

//________________________________________________________________________
void musicXMLTree2MsrSkeleton::visitStart (S_part& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_score_part" <<
      endl;
  }

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

  if (gGeneralOptions->fTraceParts)
    fLogOutputStream <<
      "--------------------------------------------" <<
      endl <<
      "Analyzing part \"" << fCurrentPartID << "\" -- start" <<
      endl;

  gIndenter++;

  fCurrentStaffNumber = 1; // default if there are no <staff> element
  fCurrentVoiceNumber = 1; // default if there are no <voice> element

  fPartVoiceNumberToVoiceMap.clear ();
  fPartVoiceNumberToDisplayingStaffNumberMap.clear ();
}

void musicXMLTree2MsrSkeleton::visitEnd (S_part& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_part" <<
      endl;
  }

  gIndenter--;

  if (gGeneralOptions->fTraceParts)
    fLogOutputStream <<
      "Analyzing part \"" << fCurrentPartID << "\" -- end" <<
      endl <<
      "--------------------------------------------" <<
      endl <<
      endl;

  // finalize the current part
  fCurrentPart->
    finalizePart (
      elt->getInputLineNumber ());
}

//________________________________________________________________________
void musicXMLTree2MsrSkeleton::visitStart (S_staves& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_direction" <<
      endl;
  }

  int stavesNumber = int(*elt);

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTraceStaves) {
    switch (stavesNumber) {
      case 0:
        fLogOutputStream <<
          "There isn't any explicit staff (hence 1 by default)"; // JMI
        break;
        
      case 1:
        fLogOutputStream <<
          "There is 1 staff";
        break;
        
      default:
        fLogOutputStream <<
          "There are " << stavesNumber << " staves";
    } // switch
    
    fLogOutputStream <<
      " in part " << fCurrentPart->getPartCombinedName() <<
      endl;
  }

  if (stavesNumber > 1) {
    // add n-1 staves to current part since 1 already exists JMI
    int n = 2;
    
    while (n <= stavesNumber) {
      fCurrentPart->
        addStaffToPartByItsNumber (
          inputLineNumber,
          msrStaff::kRegularStaff,
          n);
      n++;
    } // while
  }
}

//________________________________________________________________________
void musicXMLTree2MsrSkeleton::visitStart (S_staff& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_staff" <<
      endl;
  }

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
      
    msrAssert (false, s.str ());
  }
  
  S_msrStaff
    staff =
      createStaffInCurrentPartIfNotYetDone (
        inputLineNumber, fCurrentStaffNumber);

  if (gGeneralOptions->fTraceStaves) {
    fLogOutputStream <<
      "--> S_staff, fCurrentStaffNumber = " <<
      fCurrentStaffNumber << endl <<
    "--> S_staff, current staff name  = " <<
      staff->getStaffName() <<
      endl;
  }

  if (fOnGoingNote) {
    // regular staff indication in note/rest
    fCurrentNoteStaffNumber = fCurrentStaffNumber;
  }
}

//________________________________________________________________________
void musicXMLTree2MsrSkeleton::visitStart (S_voice& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_staff_details" <<
      endl;
  }

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
      
    msrAssert (false, s.str ());
  }
  */
  
  if (fOnGoingNote) {

    // regular voice indication in note/rest
    fCurrentNoteVoiceNumber = fCurrentVoiceNumber;

    S_msrStaff
      staff =
        createStaffInCurrentPartIfNotYetDone (
          inputLineNumber, fCurrentNoteStaffNumber);
  
    if (gGeneralOptions->fTraceNotes && gGeneralOptions->fTraceVoices)
      fLogOutputStream <<
        "--> fCurrentNoteVoiceNumber        = " <<
        fCurrentNoteVoiceNumber <<
        endl <<
        "--> S_voice, fCurrentNoteStaffNumber = " <<
        fCurrentNoteStaffNumber <<
        endl <<
        "--> S_voice, current staff name  = " <<
        staff->getStaffName() <<
        endl;
/*
    currentVoice = // ??? JMI
      createVoiceInStaffInCurrentPartIfNotYetDone (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);
  */  
  }
}

//________________________________________________________________________
void musicXMLTree2MsrSkeleton::visitStart (S_measure& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_measure" <<
      endl;
  }

/*
  Measures have a required number attribute (going from partwise to timewise,
  measures are grouped via the number).
*/

  int inputLineNumber =
    elt->getInputLineNumber ();

  fCurrentMeasureNumber = // JMI local variable???
    elt->getAttributeValue ("number");

  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceGeneral) {
    fLogOutputStream <<
      endl <<
      "<!--=== measure " << fCurrentMeasureNumber <<
      ", line " << inputLineNumber << " ===-->" <<
      endl;
  }


  // take this measure into account
  fMeasuresCounter++;
}

void musicXMLTree2MsrSkeleton::visitEnd (S_measure& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_measure" <<
      endl;
  }

  int inputLineNumber =
    elt->getInputLineNumber ();
}

//______________________________________________________________________________
void musicXMLTree2MsrSkeleton::visitStart ( S_note& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_note" <<
      endl;
  }

/*
<!ELEMENT note 
  (((grace, %full-note;, (tie, tie?)?) |
    (cue, %full-note;, duration) |
    (%full-note;, duration, (tie, tie?)?)),
   instrument?, %editorial-voice;, type?, dot*,
   accidental?, time-modification?, stem?, notehead?,
   notehead-text?, staff?, beam*, notations*, lyric*, play?)>

*/

//       <note print-object="no"> JMI grise les notes
//           <staff-lines>5</staff-lines> revient a la normale
  
  // assuming staff number 1, unless S_staff states otherwise afterwards
  fCurrentStaffNumber = 1;

  // assuming voice number 1, unless S_voice states otherwise afterwards
  fCurrentVoiceNumber = 1;

  // keep fCurrentStanzaNumber unchanged, JMI
  // for use by notes without lyrics
    
  fCurrentNoteStaffNumber = 1; // it may be absent
  fCurrentNoteVoiceNumber = 1; // it may be absent

  fOnGoingNote = true;
}

void musicXMLTree2MsrSkeleton::visitEnd ( S_note& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_note" <<
      endl;
  }

  /*
  This is a complex method, due to the fact that
  dynamics, wedges, chords and tuplets
  are not ordered in the same way in MusicXML and LilyPond.

  Staff number is analyzed before voice number
  but occurs after it in the MusicXML tree.
  That's why the treatment below has been postponed until this method
  */
  
  int inputLineNumber =
    elt->getInputLineNumber ();

  // store voice and staff numbers in MusicXML note data
  fCurrentNoteStaffNumber = fCurrentStaffNumber;
  fCurrentNoteVoiceNumber = fCurrentVoiceNumber;

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNotYetDone (
        inputLineNumber,
        fCurrentNoteStaffNumber,
        fCurrentNoteVoiceNumber);

  fPendingFiguredBass = false;

/* JMI
  if (gGeneralOptions->fTraceNotes) { // JMI
     const int fieldWidth = 27;

    fLogOutputStream << left <<
      endl <<
      "==> AFTER visitEnd (S_note&) " <<
      newNote->noteAsString () <<
      ", line " << inputLineNumber <<
      " we have:" <<
      endl <<
      setw (fieldWidth) <<
      "--> fCurrentNoteStaffNumber" << " = " <<
      fCurrentNoteStaffNumber <<
      endl <<
      setw (fieldWidth) <<
      "--> fCurrentNoteVoiceNumber" << " = " <<
      fCurrentNoteVoiceNumber <<
      endl <<
      setw (fieldWidth) <<
      "--> current voice" << " = \"" <<
      currentVoice->getVoiceName () << "\"" <<
      endl <<
      "<==" <<
      endl <<
      endl;
  }
*/
  
  fOnGoingNote = false;
}

//________________________________________________________________________
void musicXMLTree2MsrSkeleton::visitStart (S_lyric& elt )
{
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
      </note>
  */
  
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_lyric" <<
      endl;
  }

  string stanzaNumber =
    elt->getAttributeValue ("number");
  
  if (stanzaNumber.size () == 0) {
    stringstream s;

    s <<
      "lyric number " << stanzaNumber <<
      " is empty";

    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str ());
  }
  
  else {
    if (gGeneralOptions->fTraceLyrics)
      fLogOutputStream <<
        "--> setting fCurrentStanzaNumber to " <<
        stanzaNumber <<
        ", line " << elt->getInputLineNumber () <<
        endl;
        
    // register it as current stanza number, JMI
    // that remains set another positive value is met,
    // thus allowing a skip syllable to be generated
    // for notes without lyrics
    fCurrentStanzaNumber = stanzaNumber;
  }
  
  fOnGoingLyric = true;
}

void musicXMLTree2MsrSkeleton::visitEnd ( S_lyric& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_lyric" <<
      endl;
  }

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTraceLyrics) {
    fLogOutputStream <<
      endl <<
      "visitEnd ( S_lyric& )" <<
      ", line = " << inputLineNumber << ", with:" <<
      endl;

    gIndenter++;

    fLogOutputStream <<
      "Lyric data:" <<
      endl;

    {
      gIndenter++;

      const int fieldwidth = 28;
  
      fLogOutputStream << left <<
        setw (fieldwidth) <<
        "fCurrentStanzaNumber" << " = " << fCurrentStanzaNumber <<
        endl;
          
      gIndenter--;
    }
    
    gIndenter--;
  }

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNotYetDone (
        inputLineNumber,
        fCurrentNoteStaffNumber,
        fCurrentVoiceNumber);

  // fetch stanzaNumber in current voice
  S_msrStanza
    stanza =
      currentVoice->
        createStanzaInVoiceIfNotYetDone (
          inputLineNumber,
          fCurrentStanzaNumber);

  fOnGoingLyric = false;
}

//______________________________________________________________________________
void musicXMLTree2MsrSkeleton::visitStart ( S_harmony& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_harmony" <<
      endl;
  }

  fPendingHarmony                  = true;
}

void musicXMLTree2MsrSkeleton::visitEnd ( S_harmony& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_harmony" <<
      endl;
  }

  int inputLineNumber =
    elt->getInputLineNumber ();
  
  fPendingHarmony                  = false; 
}

//______________________________________________________________________________
void musicXMLTree2MsrSkeleton::visitStart ( S_figured_bass& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_figured_bass" <<
      endl;
  }

  fOnGoingFiguredBass = true;
  fPendingFiguredBass = true;
}

void musicXMLTree2MsrSkeleton::visitEnd ( S_figured_bass& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_figured_bass" <<
      endl;
  }

  fOnGoingFiguredBass = false;
}


} // namespace
