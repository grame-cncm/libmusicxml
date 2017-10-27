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

#include <climits>      /* INT_MIN */

#include "messagesHandling.h"

#include "mxmlOptions.h"
#include "msrOptions.h"

#include "msr2Summary.h"

#include "mxmlTree2MsrSkeletonBuilder.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
mxmlTree2MsrSkeletonBuilder::mxmlTree2MsrSkeletonBuilder (
  indentedOstream& ios)
  : fLogOutputStream (ios)
{
  // the MSR score we're building
  fMsrScore =
    msrScore::create (0);

  // score handling
  fScoreNumberOfMeasures = 0;

  // part groups handling
  fCurrentPartUsesImplicitPartGroup = false;
  
  fOnGoingGroupNameDisplay = false;

  // parts handling
  fCurrentNoIDPartNumber = 0;
  
  // staff handling
  fCurrentStaffNumber = -1;

  // voice handling
  fCurrentVoiceNumber = -1;

  // measures
  fPartNumberOfMeasures = 0;
    
  // lyrics handling
  fCurrentStanzaNumber = -1; // JMI

  // harmonies handling
 
  // figured bass handling
    
  // ongoing note
  fOnGoingNote = false;

  // note context
  fCurrentNoteStaffNumber = 0;
  fCurrentNoteVoiceNumber = 0;
}

mxmlTree2MsrSkeletonBuilder::~mxmlTree2MsrSkeletonBuilder ()
{}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::browseMxmlTree (
  const Sxmlelement& mxmlTree)
{  
  if (mxmlTree) {
    // create a tree browser on this visitor
    tree_browser<xmlelement> browser (this);
    
    // browse the xmlelement tree
    browser.browse (*mxmlTree);
  }
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::createImplicitPartGroup (
  int inputLineNumber)
{
  /*
  A first part group is created with all the needed contents
  if none is specified in the MusicXML data.
  */

  // sanity check
  msrAssert (
    fImplicitPartGroup == 0,
    "fImplicitPartGroup already exists");
  
  // create an implicit part group
  fCurrentPartGroupNumber = 0;
  
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Creating an implicit part group with number '" <<
      fCurrentPartGroupNumber <<
      "' to contain part \"" << fCurrentPartID << "\"" <<
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
      msrPartGroup::kPartGroupBarlineYes,
      0,          // the top level part group has an empty uplink
      fMsrScore);

  // append it to the MSR score
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Appending implicit part group '" <<
      fImplicitPartGroup->getPartGroupNumber () <<
      "' to MSR score" <<
      ", line " << inputLineNumber <<
      endl;
  }
      
  fMsrScore->
    addPartGroupToScore (fImplicitPartGroup);
    
  // register implicit group in the map
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Adding implicit part group '" <<
      fCurrentPartGroupNumber <<
      "' to this visitor's map" <<
      ", line " << inputLineNumber <<
      endl;
  }

  fPartGroupsMap [fCurrentPartGroupNumber] =
    fImplicitPartGroup;

  // make it the current group
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Pushing implicit part group '" <<
      fCurrentPartGroupNumber <<
      "' onto this visitor's stack" <<
      ", line " << inputLineNumber <<
      endl;
  }
  
  fPartGroupsStack.push (fImplicitPartGroup);
  
  if (gGeneralOptions->fTracePartGroups) {
    showPartGroupsStack (
      inputLineNumber,
      "after pushing fImplicitPartGroup");
  }

// JMI  fPartGroupsList.push_front (fImplicitPartGroup);

  fCurrentPartUsesImplicitPartGroup = true;
}

//______________________________________________________________________________
S_msrPartGroup mxmlTree2MsrSkeletonBuilder::fetchPartGroupInTheMap (
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
S_msrStaff mxmlTree2MsrSkeletonBuilder::createStaffIfNotYetDone (
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
S_msrVoice mxmlTree2MsrSkeletonBuilder::createVoiceIfNotYetDone (
  int inputLineNumber,
  int staffNumber,
  int voiceNumber)
{
  // the voice number is relative to a part,
  // we'll call it its part-relative ID

  // create the staff if not yet done
  S_msrStaff
    staff =
      createStaffIfNotYetDone (
        inputLineNumber,
        staffNumber);

  // is voice already present in staff?
  S_msrVoice
    voice =
      staff->
        fetchVoiceFromStaffByItsPartRelativeID (
          inputLineNumber,
          voiceNumber);
  
  if (! voice) {
    // create the voice and append it to the staff
    voice =
      staff->
        createVoiceInStaffByItsPartRelativeID (
          inputLineNumber,
          msrVoice::kRegularVoice,
          voiceNumber,
          fCurrentMeasureNumber);
  }
  
  return voice;
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::visitStart (S_score_partwise& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_score_partwise" <<
      endl;
  }

  if (gGeneralOptions->fTraceParts)
    fLogOutputStream <<
      "Analysing the score partwise" <<
      endl;

  fScoreNumberOfMeasures = 0;
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

void mxmlTree2MsrSkeletonBuilder::visitEnd (S_score_partwise& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_score_partwise" <<
      endl;
  }

  fMsrScore->
    setScoreNumberOfMeasures (
      fScoreNumberOfMeasures);
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::visitStart (S_part_list& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_part_list" <<
      endl;
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

  if (gGeneralOptions->fTraceParts)
    fLogOutputStream <<
      "Analysing part list" <<
      endl;

  gIndenter++;
}

void mxmlTree2MsrSkeletonBuilder::visitEnd (S_part_list& elt)
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
void mxmlTree2MsrSkeletonBuilder::visitStart (S_part_group& elt)
{  
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_part_group" <<
      endl;
  }

/*
<!--
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
-->
<!ELEMENT part-group (group-name?, group-name-display?,
  group-abbreviation?, group-abbreviation-display?,
  group-symbol?, group-barline?, group-time?, %editorial;)>
<!ATTLIST part-group
    type %start-stop; #REQUIRED
    number CDATA "1"
>

<!ELEMENT group-name (#PCDATA)>
<!ATTLIST group-name
    %print-style;
    %justify;
>
<!ELEMENT group-name-display
  ((display-text | accidental-text)*)>
<!ATTLIST group-name-display
    %print-object;
>
<!ELEMENT group-abbreviation (#PCDATA)>
<!ATTLIST group-abbreviation
    %print-style;
    %justify;
>
<!ELEMENT group-abbreviation-display
  ((display-text | accidental-text)*)>
<!ATTLIST group-abbreviation-display
    %print-object;
>

<!ELEMENT group-symbol (#PCDATA)>
<!ATTLIST group-symbol
    %position;
    %color;
>

<!ELEMENT group-barline (#PCDATA)>
<!ATTLIST group-barline
    %color;
>
<!ELEMENT group-time EMPTY>

*/

  fCurrentPartGroupNumber =
    elt->getAttributeIntValue ("number", 0);
    
  fCurrentPartGroupType =
    elt->getAttributeValue ("type");

  fCurrentPartGroupName = "";
  fCurrentPartGroupDisplayText = "";
  fCurrentPartGroupAccidentalText = "";
  fCurrentPartGroupAbbreviation = "";
  fCurrentPartGroupSymbolKind = msrPartGroup::k_NoPartGroupSymbol;
  fCurrentPartGroupSymbolDefaultX = INT_MIN;
  fCurrentPartGroupBarlineKind = msrPartGroup::kPartGroupBarlineYes;


  string printStyle = elt->getAttributeValue ("print-style"); // JMI
  
  string printObject = elt->getAttributeValue ("print-object"); // JMI
}

void mxmlTree2MsrSkeletonBuilder::visitStart (S_group_name& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_group_name" <<
      endl;
  }

  fCurrentPartGroupName = elt->getValue();
}

void mxmlTree2MsrSkeletonBuilder::visitStart (S_group_name_display& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_group_name_display" <<
      endl;
  }

  fOnGoingGroupNameDisplay = true;
}

void mxmlTree2MsrSkeletonBuilder::visitStart (S_display_text& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_display_text" <<
      endl;
  }

  if (fOnGoingGroupNameDisplay)
    fCurrentPartGroupDisplayText = elt->getValue();
}

void mxmlTree2MsrSkeletonBuilder::visitStart (S_accidental_text& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_accidental_text" <<
      endl;
  }

    fCurrentPartGroupAccidentalText = elt->getValue();
}

void mxmlTree2MsrSkeletonBuilder::visitStart (S_group_abbreviation& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_group_abbreviation" <<
      endl;
  }

  fCurrentPartGroupAbbreviation = elt->getValue ();
}

void mxmlTree2MsrSkeletonBuilder::visitStart (S_group_symbol& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_group_symbol" <<
      endl;
  }

  string groupSymbol = elt->getValue ();

  fCurrentPartGroupSymbolKind =
    msrPartGroup::k_NoPartGroupSymbol; // default value
   
  if      (groupSymbol == "brace")
    fCurrentPartGroupSymbolKind = msrPartGroup::kBracePartGroupSymbol;
    
  else if (groupSymbol == "bracket")
    fCurrentPartGroupSymbolKind = msrPartGroup::kBracketPartGroupSymbol;
    
  else if (groupSymbol == "line")
    fCurrentPartGroupSymbolKind = msrPartGroup::kLinePartGroupSymbol;
    
  else if (groupSymbol == "square")
    fCurrentPartGroupSymbolKind = msrPartGroup::kSquarePartGroupSymbol;
    
  else if (groupSymbol == "none")
    fCurrentPartGroupSymbolKind = msrPartGroup::k_NoPartGroupSymbol;
    
  else {
    if (groupSymbol.size()) {
      // part group type may be absent
      stringstream s;

      s <<
        "unknown part group symbol \"" + groupSymbol + "\"";
        
      msrMusicXMLError (
        elt->getInputLineNumber (),
        s.str ());
    }
  }

  fCurrentPartGroupSymbolDefaultX =
    elt->getAttributeIntValue ("default-x", 0);
}

void mxmlTree2MsrSkeletonBuilder::visitStart ( S_group_barline& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_group_barline" <<
      endl;
  }

  string groupBarline = elt->getValue ();
  
  // check part group barline
  if      (groupBarline == "yes")
    fCurrentPartGroupBarlineKind = msrPartGroup::kPartGroupBarlineYes;
  else if (groupBarline == "no")
    fCurrentPartGroupBarlineKind = msrPartGroup::kPartGroupBarlineNo;
  else {
    stringstream s;

    s <<
      "unknown part group barline \"" + groupBarline + "\"";
      
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str ());
  }
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::showPartGroupsMap (
  int    inputLineNumber,
  string context)
{
  fLogOutputStream <<
    "==> " << context <<
    ", line " << inputLineNumber <<
    ", fPartGroupsMap contains:" <<
    endl;
    
  if (fPartGroupsMap.size()) {
    gIndenter++;
    
    map<int, S_msrPartGroup>::const_iterator
      iBegin = fPartGroupsMap.begin (),
      iEnd   = fPartGroupsMap.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      fLogOutputStream <<
        "'" << (*i).first <<
        "' --> " <<
        (*i).second->getPartGroupCombinedName () <<
        endl;
      if (++i == iEnd) break;
      // no endl here
    } // for
    
    gIndenter--;
  }
  
  else {
    fLogOutputStream <<
      gTab << "empty map" <<
      endl;
  }
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::showPartGroupsStack (
  int    inputLineNumber,
  string context)
{
  fLogOutputStream <<
    "==> " << context <<
    ", line " << inputLineNumber <<
    ", fPartGroupsStack contains:" <<
    endl;

  if (! fPartGroupsStack.empty ()) {
    // a stack cannot be iterated, hence we use a copy
    stack<S_msrPartGroup>
      fPartGroupsStackCopy = fPartGroupsStack;

    gIndenter++;

    for ( ; ; ) {
      // fetch top part group
      S_msrPartGroup
        topPartGroup =
          fPartGroupsStackCopy.top ();

      // print it
      fLogOutputStream <<
        topPartGroup->getPartGroupCombinedName () <<
        endl;

      // pop it from stack
      fPartGroupsStackCopy.pop ();

      if (fPartGroupsStackCopy.empty ()) break;
      
      fLogOutputStream <<
        endl;
    } // for

    gIndenter--;
  }
  
  else {
    fLogOutputStream <<
      gTab << "empty stack" <<
      endl;
  }
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::showPartGroupsData (
  int    inputLineNumber,
  string context)
{
  showPartGroupsMap (
    inputLineNumber,
    context);
  showPartGroupsStack (
    inputLineNumber,
    context);

/* JMI
  {
    fLogOutputStream <<
      "==> fPartGroupsList contains:" <<
      endl;
      
    if (fPartGroupsList.size()) {
      list<S_msrPartGroup>::const_iterator
        iBegin = fPartGroupsList.begin (),
        iEnd   = fPartGroupsList.end (),
        i      = iBegin;
        
      gIndenter++;
      
      for ( ; ; ) {
        fLogOutputStream <<
          (*i);
        if (++i == iEnd) break;
        fLogOutputStream <<
          endl;
      } // for
      
      gIndenter--;
    }
    
    fLogOutputStream <<
      "<== fPartGroupsList" <<
      endl;
  
  */
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::handlePartGroupStart (
  int inputLineNumber)
{
  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "BEFORE handlePartGroupStart()");
  }

  // is the part group to be stopped known?
  S_msrPartGroup
    partGroupToBeStarted =
      fetchPartGroupInTheMap (
        fCurrentPartGroupNumber);

  if (partGroupToBeStarted) {
    stringstream s;

    s <<
      "part group '" << fCurrentPartGroupNumber << "'" <<
      " is already known";
      
    msrMusicXMLError (
      inputLineNumber,
      s.str ());
  }

  // fetch containing part group if any
  S_msrPartGroup
    containingPartGroup =
      ! fPartGroupsStack.empty ()
        ? fPartGroupsStack.top ()
        : 0;
      
  // create the part group
  partGroupToBeStarted =
    msrPartGroup::create (
      inputLineNumber,
      fCurrentPartGroupNumber,
      fCurrentPartGroupName,
      fCurrentPartGroupDisplayText,
      fCurrentPartGroupAccidentalText,
      fCurrentPartGroupAbbreviation,
      fCurrentPartGroupSymbolKind,
      fCurrentPartGroupSymbolDefaultX,
      fCurrentPartGroupBarlineKind,
      containingPartGroup,
      fMsrScore);

  // register partGroupToBeStarted in the map
  fPartGroupsMap [fCurrentPartGroupNumber] =
    partGroupToBeStarted;

  // push it onto the part group stack of this visitor
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Pushing part group " << fCurrentPartGroupNumber <<
      " onto this visitor's part group stack" <<
      ", line " << inputLineNumber <<
      endl;
  }

  fPartGroupsStack.push (partGroupToBeStarted);
  if (gGeneralOptions->fTracePartGroups) {
    showPartGroupsStack (
      inputLineNumber,
      "after pushing partGroupToBeStarted");
  }

/*
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
    if (gGeneralOptions->fTracePartGroups) {
      fLogOutputStream <<
        "Adding part group " << fCurrentPartGroupNumber <<
        " to visitor's part group map" <<
        ", line " << inputLineNumber <<
        endl;
    }
    
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
      iBegin = fPartGroupsList.begin (),
      iEnd   = fPartGroupsList.end (),
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
  */
  
  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER handlePartGroupStart()");
  }
}
  
//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::handlePartGroupStop (
  int inputLineNumber)
{
  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "BEFORE handlePartGroupStop()");
  }

  // is the part group to be stopped known?
  S_msrPartGroup
    partGroupToBeStopped =
      fetchPartGroupInTheMap (
        fCurrentPartGroupNumber);

  if (! partGroupToBeStopped) {
    stringstream s;

    s <<
      "part group '" << fCurrentPartGroupNumber << "'" <<
      " is not known in this visitor's map";

    msrMusicXMLError (
      inputLineNumber,
      s.str ());
  }

  if (fPartGroupsStack.empty ()) {
    stringstream s;

    s <<
      "part group is empty, has '" << fCurrentPartGroupNumber << "'" <<
      " has already been stopped???";

    msrMusicXMLError (
      inputLineNumber,
      s.str ());
  }

  S_msrPartGroup
    partGroupStackTop =
      fPartGroupsStack.top ();

  if (partGroupToBeStopped == partGroupStackTop) {
    // handle this 'stop'
    fPartGroupsStack.pop ();

    if (gGeneralOptions->fTracePartGroups) {
      showPartGroupsData (
        inputLineNumber,
        "after popping part group '" + partGroupStackTop->getPartGroupCombinedName () + "'");
    }
  }

  else {
    stringstream s;

    s <<
      "part groups '" << fCurrentPartGroupNumber << "'" <<
      " and " <<
      "part group '" << partGroupStackTop->getPartGroupNumber () << "'" <<
      "overlap: this is not supported";

    msrMusicXMLError (
      inputLineNumber,
      s.str ());
  }

/* JMI
  // is the part group to be stopped known?
  S_msrPartGroup
    partGroupToBeStopped =
      fetchPartGroupInTheMap (
        fCurrentPartGroupNumber);

  S_msrPartGroup
    partGroupTop =
      fPartGroupsStack.push (fImplicitPartGroup);

  if (! partGroupToBeStopped) {
    // no, but we should have fount it
    stringstream s;

    s <<
      "part group " << fCurrentPartGroupNumber <<
      " not found in this visitor's part groups map" <<
      ", line " << inputLineNumber <<
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
    iBegin = fPartGroupsList.begin (),
    iEnd   = fPartGroupsList.end (),
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

  if (gGeneralOptions->fTracePartGroups) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER REMOVAL FROM LIST");
  }

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
*/

/* JMI ???
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
  */

  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "BEFORE handlePartGroupStop()");
  }
}

void mxmlTree2MsrSkeletonBuilder::visitEnd (S_part_group& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_part_group" <<
      endl;
  }

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Handling part group '" <<
      fCurrentPartGroupNumber <<
      "', type: \"" << fCurrentPartGroupType << "\""  <<
      ", line " << inputLineNumber <<
      endl;
  }

  gIndenter++;
  
  msrPartGroup::msrPartGroupTypeKind partGroupTypeKind;

  // check part group type
  if      (fCurrentPartGroupType == "start")
    partGroupTypeKind = msrPartGroup::kStartPartGroupType;
  else if (fCurrentPartGroupType == "stop")
    partGroupTypeKind = msrPartGroup::kStopPartGroupType;
  else {
    if (fCurrentPartGroupType.size()) {
      // part group type may be absent
      stringstream s;

      s <<
        "unknown part group type \"" <<
         fCurrentPartGroupType <<
          "\"";

      msrMusicXMLError (
        inputLineNumber,
        s.str ());
    }
        
    partGroupTypeKind = msrPartGroup::k_NoPartGroupType;
  }

  switch (partGroupTypeKind) {
    
    case msrPartGroup::kStartPartGroupType:
      handlePartGroupStart (
        inputLineNumber);
      break;
      
    case msrPartGroup::kStopPartGroupType:
      handlePartGroupStop (
        inputLineNumber);
      break;
      
    case msrPartGroup::k_NoPartGroupType:
      break;
  } // switch

  gIndenter--;
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::visitStart (S_score_part& elt)
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
      " in part list" <<
      ", line " << elt->getInputLineNumber () <<
      endl;

  fCurrentPartName         = "";
  fCurrentPartAbbreviation = "";
  
  fCurrentPartInstrumentName = "";
  fCurrentPartInstrumentAbbreviation = "";
}

void mxmlTree2MsrSkeletonBuilder::visitStart (S_part_name& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_part_name" <<
      endl;
  }

/*
<!--
  The part-name indicates the full name of the musical part.
  The part-abbreviation indicates the abbreviated version of
  the name of the musical part. The part-name will often
  precede the first system, while the part-abbreviation will
  precede the other systems. The formatting attributes for
  these elements are deprecated in Version 2.0 in favor of
  the new part-name-display and part-abbreviation-display
  elements. These are defined in the common.mod file as they
  are used in both the part-list and print elements. They
  provide more complete formatting control for how part names
  and abbreviations appear in a score.
-->
<!ELEMENT part-name (#PCDATA)>
<!ATTLIST part-name
    %print-style;
    %print-object;
    %justify;
>
<!ELEMENT part-abbreviation (#PCDATA)>
<!ATTLIST part-abbreviation
    %print-style;
    %print-object;
    %justify;
>
*/

  fCurrentPartName = elt->getValue ();

  string printStyle = elt->getAttributeValue ("print-style"); // JMI
  
  string printObject = elt->getAttributeValue ("print-object"); // JMI
}

void mxmlTree2MsrSkeletonBuilder::visitStart (S_part_abbreviation& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_part_abbreviation" <<
      endl;
  }

  fCurrentPartAbbreviation = elt->getValue ();
}

void mxmlTree2MsrSkeletonBuilder::visitStart (S_instrument_name& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_instrument_name" <<
      endl;
  }

  fCurrentPartInstrumentName = elt->getValue();
}

void mxmlTree2MsrSkeletonBuilder::visitStart (S_instrument_abbreviation& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_instrument_abbreviation" <<
      endl;
  }

  fCurrentPartInstrumentAbbreviation = elt->getValue();
}

void mxmlTree2MsrSkeletonBuilder::visitEnd (S_score_part& elt)
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
    "Handling part name \"" << fCurrentPartID << "\"" <<
    ", line " << inputLineNumber <<
    endl;

  gIndenter++;

  // has a part group already been seen?
  if (! fPartGroupsMap.size()) {
    // no, create an implicit one if needed
    createImplicitPartGroup (
      inputLineNumber);

    if (gGeneralOptions->fTracePartGroups) {
      showPartGroupsData (
        inputLineNumber,
        "after creating implicit group on visitEnd (S_score_part&)");
    }
  }

  if (gGeneralOptions->fTracePartGroups) {
    showPartGroupsData (
      inputLineNumber,
      "FEE");
  }

  // fetch current part group
  S_msrPartGroup
    currentPartGroup =
      fPartGroupsStack.top ();
        
  // is this part already present in the current part group?
  S_msrPart
    part =
      currentPartGroup->
        fetchPartFromPartGroup (
          fCurrentPartID);

  if (! part) {
    // no, add it to the current part group
    part =
      currentPartGroup->
        appendPartToPartGroupByItsID (
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

  if (gGeneralOptions->fTracePartGroups) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER handling part \"" + partID + "\"");
  }
  
  gIndenter--;
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::visitStart (S_part& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_score_part" <<
      endl;
  }

  int inputLineNumber =
    elt->getInputLineNumber ();

  // fCurrentPartID is used throughout
  fCurrentPartID = elt->getAttributeValue ("id");

  if (! fCurrentPartID.size ()) {
    stringstream s;

    fCurrentNoIDPartNumber++;
    
    s <<
      "NO_ID_PART_" << fCurrentNoIDPartNumber;

    s >> fCurrentPartID;

    msrMusicXMLWarning (
      inputLineNumber,
      "part id is empty, creating one as '"
        +
      fCurrentPartID
        +
      "'");
  
    S_msrPartGroup partGroup;
  
    // has a part group already been seen?
    if (! fPartGroupsMap.size()) {
      // no, create an implicit one if needed
      createImplicitPartGroup (
        inputLineNumber);

      if (gGeneralOptions->fTracePartGroups) {
        showPartGroupsData (
          inputLineNumber,
          "after creating implicit group on visitStart (S_part&");
      }
    }

    if (gGeneralOptions->fTracePartGroups) {
      showPartGroupsData (
        inputLineNumber,
        "FOO");
    }
  
    // fetch current part group
    partGroup = fPartGroupsStack.top ();        
  
    // create the part and add it to the current part group
    S_msrPart
      part =
        partGroup->
          appendPartToPartGroupByItsID (
            inputLineNumber,
            fCurrentPartID);

    // avoid empty part MSR name JMI ???
    part->
      setPartMsrName (fCurrentPartID);

/* JMI
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
*/

    // register it in this visitor's parts map
    fPartsMap [fCurrentPartID] = part;
  }
    
  // is this part already known?
  if (fPartsMap.count (fCurrentPartID)) {
    fCurrentPart = // used thoughout
      fPartsMap [fCurrentPartID];
  }
      
  else {
    msrMusicXMLError (
      inputLineNumber,
      "part \"" +
        fCurrentPartID +
        "\" is not registered in this visitor's part map");
  }

  if (gGeneralOptions->fTraceParts) {
    fLogOutputStream <<
      endl <<
      endl <<
      "--------------------------------------------" <<
      endl <<
      "Analyzing part \"" << fCurrentPartID << "\" -- start" <<
      endl;
  }

  gIndenter++;

  // measures
  fPartNumberOfMeasures = 0;
    
  // staves and voices
  fCurrentStaffNumber = 1; // default if there are no <staff> element
  fCurrentVoiceNumber = 1; // default if there are no <voice> element
}

void mxmlTree2MsrSkeletonBuilder::visitEnd (S_part& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_part" <<
      endl;
  }

  gIndenter--;

  if (gGeneralOptions->fTraceParts) {
    fLogOutputStream <<
      "Analyzing part \"" << fCurrentPartID << "\" -- end" <<
      endl <<
      "--------------------------------------------" <<
      endl <<
      endl;
  }

  // handle current part number of measures
  if (fScoreNumberOfMeasures == 0) {
    fScoreNumberOfMeasures =
      fPartNumberOfMeasures;
  }
  else {
    // is the part number of measures consistent?
    if (fPartNumberOfMeasures != fScoreNumberOfMeasures) {
      stringstream s;

      s <<
        "part " << fCurrentPart->getPartCombinedName () <<
        " has " << fPartNumberOfMeasures <<
        " while another one has " << fScoreNumberOfMeasures;
        
      msrMusicXMLWarning (
        elt->getInputLineNumber (),
        s.str ());
    }
  }
  
  // set current part's number of measures
  fCurrentPart->
    setPartNumberOfMeasures (
      fPartNumberOfMeasures);
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::visitStart (S_staves& elt)
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
void mxmlTree2MsrSkeletonBuilder::visitStart (S_staff& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_staff" <<
      endl;
  }

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
      createStaffIfNotYetDone (
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
void mxmlTree2MsrSkeletonBuilder::visitStart (S_voice& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_staff_details" <<
      endl;
  }

  fCurrentVoiceNumber = int(*elt);
  
  int inputLineNumber =
    elt->getInputLineNumber ();
  
  if (fOnGoingNote) {
    // regular voice indication in note/rest
    fCurrentNoteVoiceNumber = fCurrentVoiceNumber;

    S_msrStaff
      staff =
        createStaffIfNotYetDone (
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
      createVoiceIfNotYetDone (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);
  */  
  }
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::visitStart (S_measure& elt)
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
  fPartNumberOfMeasures++;
}

void mxmlTree2MsrSkeletonBuilder::visitEnd (S_measure& elt)
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
void mxmlTree2MsrSkeletonBuilder::visitStart ( S_note& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_note" <<
      endl;
  }
  
  // assuming staff number 1, unless S_staff states otherwise afterwards
  fCurrentStaffNumber = 1;

  // assuming voice number 1, unless S_voice states otherwise afterwards
  fCurrentVoiceNumber = 1;
    
  fCurrentNoteStaffNumber = 1; // it may be absent
  fCurrentNoteVoiceNumber = 1; // it may be absent

  fOnGoingNote = true;
}

void mxmlTree2MsrSkeletonBuilder::visitEnd ( S_note& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_note" <<
      endl;
  }
  
  int inputLineNumber =
    elt->getInputLineNumber ();

  // store voice and staff numbers in MusicXML note data
  fCurrentNoteStaffNumber = fCurrentStaffNumber;
  fCurrentNoteVoiceNumber = fCurrentVoiceNumber;

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceIfNotYetDone (
        inputLineNumber,
        fCurrentNoteStaffNumber,
        fCurrentNoteVoiceNumber);
  
  fOnGoingNote = false;
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::visitStart (S_lyric& elt )
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
}

void mxmlTree2MsrSkeletonBuilder::visitEnd ( S_lyric& elt )
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
      ", line = " << inputLineNumber <<
      ", with:" <<
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
      createVoiceIfNotYetDone (
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
}

//______________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::visitStart ( S_harmony& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_harmony" <<
      endl;
  }

  int inputLineNumber =
    elt->getInputLineNumber ();

  // append a harmony staff and voice to the current part
  fCurrentPart->
    createPartHarmonyStaffAndVoiceIfNotYetDone (
      inputLineNumber);
}

//______________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::visitStart ( S_figured_bass& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_figured_bass" <<
      endl;
  }

  int inputLineNumber =
    elt->getInputLineNumber ();
  
  // append a figured bass staff and voice to the current part
  fCurrentPart->
    createPartFiguredStaffAndVoiceIfNotYetDone (
      inputLineNumber);
}


} // namespace
