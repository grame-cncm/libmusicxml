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
void checkStep (
  int    inputLineNumber,
  string stepValue)
{  
  if (stepValue.size () == 1) {
    char firstChar = stepValue [0];

    if (firstChar < 'A' || firstChar > 'G') {
      stringstream s;
      
      s <<
        "step value " << firstChar <<
        " is not a letter from A to G";
          
      msrMusicXMLError (
        inputLineNumber,
        s.str());
    }
  }

  else {
    stringstream s;
    
    s <<
      "root step value " << stepValue <<
      " should be a single letter from A to G";
      
    msrMusicXMLError (
      inputLineNumber,
      s.str());
  }
}

//________________________________________________________________________
void xml2MsrTranslator::initializeNoteData ()
{
  // basic note description

// JMI  fCurrentNoteKind = k_NoNoteKind;

  fCurrentNoteQuatertonesPitch   = k_NoQuaterTonesPitch;
  fCurrentNoteSoundingDivisions  = -17;
  fCurrentNoteDisplayedDivisions = -19;
  fCurrentNoteDotsNumber         = 0;
  fCurrentNoteGraphicDuration    = k_NoDuration;

  fCurrentNoteOctave = -1;

  fCurrentNoteIsARest = false;
  fCurrentRestMeasure = false;
  
  fCurrentNoteIsUnpitched = false;
  
  fCurrentNoteIsAGraceNote = false;

  // note context
  
  fCurrentNoteStaffNumber = 0;
  fCurrentNoteVoiceNumber = 0;

  fCurrentNoteHasATimeModification = false;
  fCurrentActualNotes = -1;
  fCurrentNormalNotes = -1;
  
  fCurrentNoteBelongsToAChord = false;

  fCurrentNoteBelongsToATuplet = false;

  // note lyrics

// JMI  fCurrentNoteSyllableExtendKind = k_NoSyllableExtend;
}

xml2MsrTranslator::xml2MsrTranslator ()
{
  // initialize note data to a neutral state
  initializeNoteData ();

  fCurrentMillimeters = -1;
  fCurrentTenths      = -1;
  
  fOnGoingPageLayout = false;

  fCurrentDivisionsPerQuarterNote = 0;

  // create the MSR score
  fMsrScore =
    msrScore::create (0);

  fCurrentPartUsesImplicitPartgroup = false;
  
  fOnGoingGroupNameDisplay = false;

  fCurrentMultipleRestCounter = 0;
  
  fOnGoingBarline = false;
  
  fCurrentWordsContents = "";

  fCurrentNoteDiatonicPitch = k_NoDiatonicPitch;
  fCurrentNoteAlteration    = k_NoAlteration;

  fCurrentForwardStaffNumber = 1; // JMI
  fCurrentForwardVoiceNumber = 1; // JMI
  fCurrentVoiceNumber = 1; // JMI

  fOnGoingLyric = false;
  fCurrentStanzaNumber = -1; // JMI
  fCurrentSyllabic = "";
  fCurrentLyricText = "";
  fCurrentLyricElision = false;
  
  fCurrentSyllableKind       = msrSyllable::k_NoSyllable;
  fCurrentSyllableExtendKind = msrSyllable::k_NoSyllableExtend;
  fOnGoingSyllableExtend     = false;

  fFirstSyllableInSlurKind     = msrSyllable::k_NoSyllable;
  fFirstSyllableInLigatureKind = msrSyllable::k_NoSyllable;
  
  fCurrentBackupDuration = -1;

  fCurrentDirectionStaffNumber = 1; // it may be absent

  fCurrentNoteStaffNumber = 0;
  fCurrentNoteVoiceNumber = 0;
  fOnGoingNote            = false;

  fCurrentMusicXMLTremoloType = k_NoTremolo;

  fOnGoingChord = false;

  fCurrentATupletStopIsPending = false;
  
  fOnGoingSlur          = false;
  fOnGoingSlurHasStanza = false;

  fOnGoingLigature          = false;
  fOnGoingLigatureHasStanza = false;

  fBendAlterValue = -39;
  
  fPendingHarmony                  = false;
  fCurrentHarmonyRootDiatonicPitch = k_NoDiatonicPitch;
  fCurrentHarmonyRootAlteration    = k_NoAlteration;
  fCurrentHarmonyKind              = msrHarmony::k_NoHarmony;
  fCurrentHarmonyKindText          = "";
  fCurrentHarmonyBassDiatonicPitch = k_NoDiatonicPitch;
  fCurrentHarmonyBassAlteration    = k_NoAlteration;
  fCurrentHarmonyDegreeValue       = -1;
  fCurrentHarmonyDegreeAlteration  = k_NoAlteration;

  fOnGoingDirection     = true;
  fOnGoingDirectionType = false;

  fRepeatHasBeenCreatedForCurrentPart = false;
  
  fOnGoingBackup  = false;
  fOnGoingForward = false;

  fCurrentStaffTuningAlteration = k_NoAlteration;
  fCurrentStaffTuningOctave     = -1;
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
S_msrPartgroup xml2MsrTranslator::createImplicitMsrPartgroupIfNeeded (
  int inputLineNumber)
{
  /*
  A first part group is created with all the needed contents
  if none is specified in the MusicXML data.
  */

  if (! fImplicitPartgroup) {
    // create an implicit part group
    fCurrentPartgroupNumber = 1;
    
    if (gGeneralOptions->fTracePartgroups)
      cerr << idtr <<
        "Creating an implicit part group with number " <<
        fCurrentPartgroupNumber <<
        ", line " << inputLineNumber <<
        endl;
  
    fImplicitPartgroup =
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
  
    // append it to the MSR score
    if (gGeneralOptions->fTracePartgroups)
      cerr << idtr <<
        "Appending implicit part group " <<
        fImplicitPartgroup->getPartgroupNumber () <<
        " to MSR score" <<
        endl;
        
    fMsrScore->
      addPartgroupToScore (fImplicitPartgroup);
      
    /* JMI
      this implicit part group will be added to the MSR score
      in method 'visitEnd (S_part_list& elt)'
    */
  }

  // add implicit part group to the map of this visitor
  if (gGeneralOptions->fTracePartgroups)
    cerr << idtr <<
      "Adding implicit part group " << fCurrentPartgroupNumber <<
      " to visitor's data" <<
      endl;
      
  fPartgroupsMap [fCurrentPartgroupNumber] = fImplicitPartgroup;
  fPartgroupsList.push_front (fImplicitPartgroup);

  fCurrentPartUsesImplicitPartgroup = true;
  
  return fImplicitPartgroup;
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
S_msrVoice xml2MsrTranslator::createVoiceInStaffInCurrentPartIfNeeded (
  int inputLineNumber,
  int staffNumber,
  int voiceNumber)
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
        fetchVoiceFromStaffByItsExternalNumber (
          inputLineNumber,
          voiceNumber);

  if (! voice) {
    // no, add it to the staff
    voice =
      staff->
        createVoiceInStaffByItsExternalNumber (
          inputLineNumber,
          voiceNumber);
  }
  
  return voice;
}  

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_work_number& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_work_number" <<
      endl;

  fMsrScore->getIdentification () ->
    setWorkNumber (
      elt->getInputLineNumber (),
      elt->getValue ());
}

void xml2MsrTranslator::visitStart ( S_work_title& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_work_title" <<
      endl;

  fMsrScore->getIdentification () ->
    setWorkTitle (
      elt->getInputLineNumber (),
      elt->getValue ());
}
  
void xml2MsrTranslator::visitStart ( S_movement_number& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_movement_number" <<
      endl;

  fMsrScore->getIdentification () ->
    setMovementNumber (
      elt->getInputLineNumber (),
      elt->getValue ());
}

void xml2MsrTranslator::visitStart ( S_movement_title& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_movement_title" <<
      endl;

  fMsrScore->getIdentification () ->
    setMovementTitle (
      elt->getInputLineNumber (),
      elt->getValue ());
}

void xml2MsrTranslator::visitStart ( S_creator& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_creator" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  string creatorType = elt->getAttributeValue ("type");

  if      (creatorType == "composer") {
    fMsrScore->getIdentification () ->
      addComposer (
        inputLineNumber,
        creatorType,
        elt->getValue ());
  }
  
  else if (creatorType == "arranger") {
    fMsrScore->getIdentification () ->
      addArranger (
        inputLineNumber,
        creatorType,
        elt->getValue ());
  }
  
  else if (creatorType == "poet") {
    fMsrScore->getIdentification () ->
      addPoet (
        inputLineNumber,
        creatorType,
        elt->getValue ());
  }
  
  else if (creatorType == "lyricist") {
    fMsrScore->getIdentification () ->
      addLyricist (
        inputLineNumber,
        creatorType,
        elt->getValue ());
  }
  
  else {
    stringstream s;

    s <<
      "creator type '" << creatorType << "'" <<
      " is unknown";

    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }
}

void xml2MsrTranslator::visitStart ( S_rights& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_rights" <<
      endl;

  fMsrScore->getIdentification () ->
    setRights (
      elt->getInputLineNumber (),
      elt->getValue ());
}

void xml2MsrTranslator::visitStart ( S_software& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_software" <<
      endl;

  fMsrScore->getIdentification () ->
    addSoftware (
      elt->getInputLineNumber (),
      elt->getValue ());
}

void xml2MsrTranslator::visitStart ( S_encoding_date& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_encoding_date" <<
      endl;

  fMsrScore->getIdentification () ->
    setEncodingDate (
      elt->getInputLineNumber (),
      elt->getValue ());
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_millimeters& elt )
{ 
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_millimeters" <<
      endl;

  fCurrentMillimeters = (float)(*elt);
  
  fMsrScore->getPageGeometry ()->
    setMillimeters (fCurrentMillimeters);
}

void xml2MsrTranslator::visitStart ( S_tenths& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_tenths" <<
      endl;

  fCurrentTenths = (int)(*elt);

  fMsrScore->getPageGeometry ()->
    setTenths (fCurrentTenths);
}

void xml2MsrTranslator::visitEnd ( S_scaling& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_scaling" <<
      endl;

  if (gGeneralOptions->fTraceGeneral)
    cerr << idtr <<
      "There are " << fCurrentTenths <<
      " tenths for " <<  fCurrentMillimeters <<
      " millimeters, hence the global staff size is " <<
      fMsrScore->getPageGeometry ()->globalStaffSize () <<
      endl;
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_system_distance& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_system_distance" <<
      endl;

  int systemDistance = (int)(*elt);
  
//  cerr << "--> systemDistance = " << systemDistance << endl;
  fMsrScore->getPageGeometry ()->
    setBetweenSystemSpace (
      systemDistance * fCurrentMillimeters / fCurrentTenths / 10);  
}

void xml2MsrTranslator::visitStart ( S_top_system_distance& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_top_system_distance" <<
      endl;

  int topSystemDistance = (int)(*elt);
  
//  cerr << "--> fTopSystemDistance = " << topSystemDistance << endl;
    fMsrScore->getPageGeometry ()->
    setPageTopSpace (
      topSystemDistance * fCurrentMillimeters / fCurrentTenths / 10);  
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_page_layout& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_page_layout" <<
      endl;

  fOnGoingPageLayout = true;
}
void xml2MsrTranslator::visitEnd ( S_page_layout& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_page_layout" <<
      endl;

  fOnGoingPageLayout = false;
}

void xml2MsrTranslator::visitStart ( S_page_height& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_page_height" <<
      endl;

  if (fOnGoingPageLayout) {
    int pageHeight = (int)(*elt);
    
    //cerr << "--> pageHeight = " << pageHeight << endl;
    fMsrScore->getPageGeometry ()->
      setPaperHeight (
        pageHeight * fCurrentMillimeters / fCurrentTenths / 10);  
  }
}

void xml2MsrTranslator::visitStart ( S_page_width& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_page_width" <<
      endl;

  if (fOnGoingPageLayout) {
    int pageWidth = (int)(*elt);
    
    //cerr << "--> pageWidth = " << pageWidth << endl;
    fMsrScore->getPageGeometry ()->
      setPaperWidth (
        pageWidth * fCurrentMillimeters / fCurrentTenths / 10);  
  }
}

void xml2MsrTranslator::visitStart ( S_left_margin& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_left_margin" <<
      endl;

  if (fOnGoingPageLayout) {
    int leftMargin = (int)(*elt);
    
    //cerr << "--> leftMargin = " << leftMargin << endl;
    fMsrScore->getPageGeometry ()->
      setLeftMargin (
        leftMargin * fCurrentMillimeters / fCurrentTenths / 10);  
  }
}

void xml2MsrTranslator::visitStart ( S_right_margin& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_right_margin" <<
      endl;

  if (fOnGoingPageLayout) {
    int rightMargin = (int)(*elt);
    
    //cerr << "--> rightMargin = " << rightMargin << endl;
    fMsrScore->getPageGeometry ()->
      setRightMargin (
        rightMargin * fCurrentMillimeters / fCurrentTenths / 10);  
  }
}

void xml2MsrTranslator::visitStart ( S_top_margin& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_top_margin" <<
      endl;

  if (fOnGoingPageLayout) {
    int topMargin = (int)(*elt);
    
    //cerr << "--> topMargin = " << topMargin << endl;
    fMsrScore->getPageGeometry ()->
      setTopMargin (
        topMargin * fCurrentMillimeters / fCurrentTenths / 10);  
  }
}

void xml2MsrTranslator::visitStart ( S_bottom_margin& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_bottom_margin" <<
      endl;

  if (fOnGoingPageLayout) {
    int bottomMargin = (int)(*elt);
    
    //cerr << "--> bottomMargin = " << bottomMargin << endl;
    fMsrScore->getPageGeometry ()->
      setBottomMargin (
        bottomMargin * fCurrentMillimeters / fCurrentTenths / 10);  
  }
}

//________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_credit& elt )
{
/*
  <credit page="1">
    <credit-words default-x="607" default-y="1443" font-family="ＭＳ ゴシック" font-size="24" font-weight="bold" justify="center" valign="top" xml:lang="ja">越後獅子</credit-words>
  </credit>
  <credit page="1">
    <credit-words default-x="1124" default-y="1345" font-size="12" font-weight="bold" justify="right" valign="top">Arr. Y. Nagai , K. Kobatake</credit-words>
  </credit>
  <credit page="1">
    <credit-words default-x="602" default-y="73" font-size="9" halign="center" valign="bottom">Transcription donated to the public domain, 2005 by Tom Potter</credit-words>
  </credit>
  <credit page="1">
    <credit-words default-x="129" default-y="244" font-size="11" valign="top">Source: "Japanese Popular Music: a collection of the popular music of Japan rendered in to the 
staff notation", by Y. Nagai and K. Kobatake, 2nd ed., Osaka, S. Miki &amp; Co., 1892, pp. 96-97.

Transcribed into Finale music notation by Tom Potter, 2005.  See http://www.daisyfield.com/music/
Lyrics added by Karen Tanaka and Michael Good, 2006. See http://www.recordare.com/</credit-words>
  </credit>
  <credit page="1">
    <credit-words default-x="607" default-y="1395" font-size="24" font-weight="bold" halign="center" valign="top">Echigo-Jishi</credit-words>
  </credit>
*/

  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_credit" <<
      endl;

  int creditPageNumber =
    elt->getAttributeIntValue ("page", 0);
  
  fCurrentCredit =
    msrCredit::create (
      elt->getInputLineNumber (),
      creditPageNumber);
}

void xml2MsrTranslator::visitStart ( S_credit_words& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_credit_words" <<
      endl;

  string creditWordsContents =
    elt->getValue ();
  
  string creditWordsFontFamily =
    elt->getAttributeValue ("font-family");

  int creditWordsFontSize =
    elt->getAttributeIntValue ("font-size", 0);

  string creditWordsFontWeight =
    elt->getAttributeValue ("font-weight"); // JMI etc

  string creditWordsFontJustify =
    elt->getAttributeValue ("justify");

  string creditWordsFontHAlign =
    elt->getAttributeValue ("halign");

  string creditWordsFontVAlign =
    elt->getAttributeValue ("valign");

  string creditWordsFontXMLLanguage =
    elt->getAttributeValue ("xml:lang");

  // create the credit words
  S_msrCreditWords
    creditWords =
      msrCreditWords::create (
        elt->getInputLineNumber (),
        creditWordsContents,
        creditWordsFontFamily,
        creditWordsFontSize,
        creditWordsFontWeight,
        creditWordsFontJustify,
        creditWordsFontHAlign,
        creditWordsFontVAlign,
        creditWordsFontXMLLanguage);

  // append it to the current credit
  fCurrentCredit->
    appendCreditWordsToCredit (
      creditWords);
}

void xml2MsrTranslator::visitEnd ( S_credit& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_credit" <<
      endl;

  fMsrScore->
    setCredit (fCurrentCredit);
  
  fCurrentCredit = 0;
}

//________________________________________________________________________
void xml2MsrTranslator::visitStart (S_part_list& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_part_list" <<
      endl;

  if (gGeneralOptions->fTraceParts)
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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_part_list" <<
      endl;

  idtr--;

  if (fCurrentPartUsesImplicitPartgroup) {
    // force an implicit part group "stop" on it,
    // fCurrentPartgroupNumber holds the value 1
    handlePartgroupStop (
      elt->getInputLineNumber ());
    
 // JMI   fCurrentPartUsesImplicitPartgroup = false;
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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_part_group" <<
      endl;

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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_group_name" <<
      endl;

  fCurrentPartgroupName = elt->getValue();
}

void xml2MsrTranslator::visitStart (S_group_name_display& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_group_name_display" <<
      endl;

  fOnGoingGroupNameDisplay = true;
}

void xml2MsrTranslator::visitStart (S_display_text& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_display_text" <<
      endl;

  if (fOnGoingGroupNameDisplay)
    fCurrentPartgroupDisplayText = elt->getValue();
}

void xml2MsrTranslator::visitStart (S_accidental_text& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_accidental_text" <<
      endl;

    fCurrentPartgroupAccidentalText = elt->getValue();
}

void xml2MsrTranslator::visitStart (S_group_abbreviation& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_group_abbreviation" <<
      endl;

  fCurrentPartgroupAbbreviation = elt->getValue ();
}

void xml2MsrTranslator::visitStart (S_group_symbol& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_group_symbol" <<
      endl;

  fCurrentPartgroupSymbol = elt->getValue ();

  fCurrentPartgroupSymbolDefaultX =
    elt->getAttributeIntValue ("default-x", 0);
}

void xml2MsrTranslator::visitStart ( S_group_barline& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_group_barline" <<
      endl;

  fCurrentPartgroupBarline = elt->getValue ();
}

//________________________________________________________________________
void xml2MsrTranslator::showPartgroupsData (string context)
{    
  if (gGeneralOptions->fTracePartgroups) {
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
      idtr <<
        "<== fPartgroupsMap" <<
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
    if (gGeneralOptions->fTracePartgroups)
      cerr << idtr <<
        "Adding part group " << fCurrentPartgroupNumber <<
        " to visitor's part group map" <<
        endl;
    fPartgroupsMap [fCurrentPartgroupNumber] =
      partgroupToBeStarted;
  }
  
  // add it to the part group list of this visitor
  if (gGeneralOptions->fTracePartgroups)
    cerr << idtr <<
      "Adding part group " << fCurrentPartgroupNumber <<
      " to visitor's part groups list" <<
      endl;

  if (! fPartgroupsList.size())
  
    // insert first part group ahead of the list
    fPartgroupsList.push_front (partgroupToBeStarted);
    
  else {
    
    // place in the part groups list so as
    // to have them ordered by increasing order
    // (all of them are negative)
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
      " not found in this visitor's part groups map" <<
      endl;
    msrInternalError (
      inputLineNumber,
      s.str());
  }

  // remove the part group to be stopped from the part group list
  if (gGeneralOptions->fTracePartgroups)
    cerr << idtr <<
      "Removing part group " <<
      partgroupToBeStopped->getPartgroupNumber () <<
      " from visitor's part groups list" <<
      ", line " << inputLineNumber <<
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

  if (partgroupToBeStopped != fImplicitPartgroup) {
    // take care of the part group to be stopped
    // in the part groups list
    
    if (! fPartgroupsList.size()) {
      
      // we're just removed the only part group in the list:
      // append it to the MSR score
      if (gGeneralOptions->fTracePartgroups)
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
      if (gGeneralOptions->fTracePartgroups)
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
  }

  // remove part group from the map
  // CAUTION: erase() destroys the element it removes!
  if (gGeneralOptions->fTracePartgroups)
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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_part_group" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTracePartgroups)
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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_score_part" <<
      endl;

  fCurrentPartID = elt->getAttributeValue ("id");

  if (gGeneralOptions->fTraceParts)
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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_part_name" <<
      endl;

  fCurrentPartName = elt->getValue ();
}

void xml2MsrTranslator::visitStart (S_part_abbreviation& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_part_abbreviation" <<
      endl;

  fCurrentPartAbbreviation = elt->getValue ();
}

void xml2MsrTranslator::visitStart (S_instrument_name& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_instrument_name" <<
      endl;

  fCurrentPartInstrumentName = elt->getValue();
}

void xml2MsrTranslator::visitStart (S_instrument_abbreviation& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_instrument_abbreviation" <<
      endl;

  fCurrentPartInstrumentAbbreviation = elt->getValue();
}

void xml2MsrTranslator::visitEnd (S_score_part& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_score_part" <<
      endl;

  string partID = elt->getAttributeValue ("id");

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTraceParts)
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
    // no, create an implicit one if needed
    partgroup =
      createImplicitMsrPartgroupIfNeeded (
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

  if (fImplicitPartgroup) {
    // force an implicit part group "stop" on it
    // fCurrentPartgroupNumber holds the value 1
    handlePartgroupStop (
      inputLineNumber);

    // forget the implicit group JNMI
  //  fImplicitPartgroup = 0;
  }

  fCurrentPartUsesImplicitPartgroup = false;

  showPartgroupsData (
    "AFTER handling part \"" + partID + "\"");

  idtr--;
}

//________________________________________________________________________
void xml2MsrTranslator::visitStart (S_part& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_score_part" <<
      endl;

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

  fRepeatHasBeenCreatedForCurrentPart = false;
}

void xml2MsrTranslator::visitEnd (S_part& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_part" <<
      endl;

  idtr--;

  if (gGeneralOptions->fTraceParts)
    cerr <<
      idtr <<
        "Analyzing part \"" << fCurrentPartID << "\" -- end" <<
        endl <<
      idtr <<
        "--------------------------------------------" <<
        endl <<
      endl;

  // finalize the current part
  fCurrentPart->
    finalizePart (
      elt->getInputLineNumber ());
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_divisions& elt ) 
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_divisions" <<
      endl;

  fCurrentDivisionsPerQuarterNote = (int)(*elt);
  
  if (fCurrentDivisionsPerQuarterNote <= 0) {
    msrMusicXMLError (
      elt->getInputLineNumber (),
      "divisions per quarter note should be positive");
  }

  // set current part's divisions per quarter note
  if (gGeneralOptions->fTraceGeneral) {
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
    setPartDivisionsPerQuarterNote (
      fCurrentDivisionsPerQuarterNote);

  // print the durations divisions
  fCurrentPart->
    printDurationsDivisions (cerr);
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_clef& elt )
{ 
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_clef" <<
      endl;

  // https://usermanuals.musicxml.com/MusicXML/Content/EL-MusicXML-clef.htm

  // The optional number attribute refers to staff numbers.
  // If absent (0), apply to all part staves.
  
  fCurrentClefStaffNumber =
    elt->getAttributeIntValue("number", 0); 

  fCurrentClefLine = 0;
  fCurrentClefOctaveChange = 0;
  fCurrentClefSign = "";
}

void xml2MsrTranslator::visitStart ( S_sign& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_sign" <<
      endl;

  fCurrentClefSign = elt->getValue();
}

void xml2MsrTranslator::visitStart ( S_line& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_line" <<
      endl;

  fCurrentClefLine = (int)(*elt);
}
  
void xml2MsrTranslator::visitStart ( S_clef_octave_change& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_clef_octave_change" <<
      endl;

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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_clef" <<
      endl;

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

  else if (fCurrentClefSign == "TAB") {
    
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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_clef" <<
      endl;

  // The optional number attribute refers to staff numbers.
  // If absent (0), apply to all part staves.
  fCurrentKeyStaffNumber =
    elt->getAttributeIntValue ("number", 0);

  fCurrentKeyFifths   = 0;
  fCurrentKeyCancel   = 0;
  fCurrentKeyModeKind = msrKey::kMajorMode;
}
  
void xml2MsrTranslator::visitStart ( S_fifths& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_fifths" <<
      endl;

  fCurrentKeyFifths = (int)(*elt);
}

void xml2MsrTranslator::visitStart ( S_mode& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_mode" <<
      endl;

  string mode = elt->getValue();

  if       (mode == "major") {
    fCurrentKeyModeKind = msrKey::kMajorMode;
  }
  else  if (mode == "minor") {
    fCurrentKeyModeKind = msrKey::kMinorMode;
  }
  else {
    stringstream s;
    
    s << "mode " << mode << " is unknown";
    
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }
}

void xml2MsrTranslator::visitStart ( S_cancel& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_cancel" <<
      endl;

  fCurrentKeyCancel = (int)(*elt);
}

void xml2MsrTranslator::visitEnd ( S_key& elt ) 
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_key" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();

  // key fifths number
  msrQuartertonesPitch keyTonicPitch;
  
  switch (fCurrentKeyFifths) {
    case 0:
      keyTonicPitch = k_cNatural;
      break;
    case 1:
      keyTonicPitch = k_gNatural;
      break;
    case 2:
      keyTonicPitch = k_dNatural;
      break;
    case 3:
      keyTonicPitch = k_aNatural;
      break;
    case 4:
      keyTonicPitch = k_eNatural;
      break;
    case 5:
      keyTonicPitch = k_bNatural;
      break;
    case 6:
      keyTonicPitch = k_fSharp;
      break;
    case 7:
      keyTonicPitch = k_cSharp;
      break;
    case -1:
      keyTonicPitch = k_fNatural;
      break;
    case -2:
      keyTonicPitch = k_bFlat;
      break;
    case -3:
      keyTonicPitch = k_eFlat;
      break;
    case -4:
      keyTonicPitch = k_aFlat;
      break;
    case -5:
      keyTonicPitch = k_dFlat;
      break;
    case -6:
      keyTonicPitch = k_gFlat;
      break;
    case -7:
      keyTonicPitch = k_cFlat;
      break;
    default: // unknown key sign !!
      {
      stringstream s;
      
      s << 
        "ERROR: unknown key fifths number \"" << fCurrentKeyFifths << "\"";
        
      msrMusicXMLError (
        inputLineNumber,
        s.str());
      }
  } // switch

  // create msrKey
  S_msrKey
    key =
      msrKey::create (
        inputLineNumber,
        keyTonicPitch, fCurrentKeyModeKind,
        fCurrentKeyCancel);

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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_key" <<
      endl;

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
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_beats" <<
      endl;

  fCurrentTimeBeats = (int)(*elt);
}
  
void xml2MsrTranslator::visitStart ( S_beat_type& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_beat_type" <<
      endl;

  fCurrentTimeBeatType = (int)(*elt);
}
 
void xml2MsrTranslator::visitStart ( S_senza_misura& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_senza_misura" <<
      endl;

  fCurrentTimeSenzaMisura = true;
}

void xml2MsrTranslator::visitEnd ( S_time& elt ) 
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_time" <<
      endl;

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

void xml2MsrTranslator::visitStart ( S_instruments& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_time" <<
      endl;

/*
        <instruments>2</instruments>
*/

  int instruments = (int)(*elt); // JMI
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_transpose& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_transpose" <<
      endl;

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
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_diatonic" <<
      endl;

  fCurrentTransposeDiatonic = (int)(*elt);
}
  
void xml2MsrTranslator::visitStart ( S_chromatic& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_chromatic" <<
      endl;

  fCurrentTransposeChromatic = (int)(*elt);
}
 
void xml2MsrTranslator::visitEnd ( S_transpose& elt ) 
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_transpose" <<
      endl;

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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_direction" <<
      endl;

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
  
  <direction placement="above">
    <direction-type>
      <bracket default-y="20" line-end="down" line-type="solid" number="1" type="start"/>
    </direction-type>
    <offset>1</offset>
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

  fCurrentMetronomeWords = 0;
  fCurrentMetronomeTempo = 0;

  fOnGoingDirection = true;
}

void xml2MsrTranslator::visitStart (S_direction_type& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_direction_type" <<
      endl;

  fOnGoingDirectionType = true;
}

void xml2MsrTranslator::visitStart (S_offset& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_offset" <<
      endl;

/*
    <offset>1</offset>
*/

  string offset = elt->getValue ();
  
  if (fOnGoingDirection) { // JMI
  }
}

void xml2MsrTranslator::visitStart (S_octave_shift& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_octave_shift" <<
      endl;

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
      createVoiceInStaffInCurrentPartIfNeeded (
        elt->getInputLineNumber (),
        fCurrentDirectionStaffNumber,
        fCurrentVoiceNumber);

  // append octave shift to the current voice
  currentVoice->
    appendOctaveShiftToVoice (octaveShift);
}

void xml2MsrTranslator::visitStart (S_words& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_words" <<
      endl;

/*
  <words default-y="-73" font-style="italic" relative-x="5">cresc.</words>
*/

  fCurrentWordsContents = elt->getValue ();

  string wordsFontStyle   = elt->getAttributeValue ("font-style");

  msrWords::msrWordsFontStyleKind
    wordsFontStyleKind =
      msrWords::kNormalStyle; // default value

  if      (wordsFontStyle == "normal")
    wordsFontStyleKind = msrWords::kNormalStyle;
  else if (wordsFontStyle == "italic")
    wordsFontStyleKind = msrWords::KItalicStyle;
  else {
    if (wordsFontStyle.size ()) {
      stringstream s;
      
      s <<
        "font-style value " << wordsFontStyle <<
        " should be 'normal' or 'italic'";
      
      msrMusicXMLError (
        elt->getInputLineNumber (),
        s.str());
    }
  }
  
  string wordsFontSize = elt->getAttributeValue ("font-size");
  
  string wordsFontWeight = elt->getAttributeValue ("font-weight");
  
  msrWords::msrWordsFontWeightKind
    wordsFontWeightKind =
      msrWords::kNormalWeight; // default value

  if      (wordsFontWeight == "normal")
    wordsFontWeightKind = msrWords::kNormalWeight;
  else if (wordsFontWeight == "bold")
    wordsFontWeightKind = msrWords::kBoldWeight;
  else {
    if (wordsFontWeight.size ()) {
      stringstream s;
      
      s <<
        "font-weight value " << wordsFontWeight <<
        " should be 'normal' or 'bold'";
      
      msrMusicXMLError (
        elt->getInputLineNumber (),
        s.str());
    }
  }
  
  string wordsXMLLang = elt->getAttributeValue ("xml:lang");

  msrWords::msrWordsXMLLangKind
    wordsXMLLangKind =
      msrWords::kItLang; // default value

  if      (wordsXMLLang == "it")
    wordsXMLLangKind = msrWords::kItLang;
  else if (wordsXMLLang == "en")
    wordsXMLLangKind = msrWords::kEnLang;
  else if (wordsXMLLang == "de")
    wordsXMLLangKind = msrWords::kDeLang;
  else if (wordsXMLLang == "fr")
    wordsXMLLangKind = msrWords::kFrLang;
  else {
    if (wordsXMLLang.size ()) {
      stringstream s;
      
      s <<
        "xml:lang value " << wordsXMLLang <<
        " should be 'it', 'en', 'de' or 'fr'";
      
      msrMusicXMLError (
        elt->getInputLineNumber (),
        s.str());
    }
  }

  if (fCurrentWordsContents.size ()) {
    if (gGeneralOptions->fTraceWords)
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
          wordsFontStyleKind,
          wordsFontSize,
          wordsFontWeightKind,
          wordsXMLLangKind);

    fPendingWords.push_back (words);
  }
}

void xml2MsrTranslator::visitEnd (S_direction_type& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_direction_type" <<
      endl;

  fOnGoingDirectionType = false;
}

//________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_metronome& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_metronome" <<
      endl;

  string parentheses = elt->getAttributeValue("parentheses");
  
  fCurrentMetronomeBeatsData.clear();
  fCurrentMetrenomePerMinute = 0;
  fCurrentMetronomeBeat.fBeatUnit = "";
  fCurrentMetronomeBeat.fDots = 0;

  if (parentheses.size()) {
    // cerr << "--> S_metronome, parentheses = " << parentheses << endl;
    
    if (parentheses == "yes")
      fCurrentMetronomeParentheses = true;
      
    else if (parentheses == "no")
      fCurrentMetronomeParentheses = true;
      
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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_metronome" <<
      endl;

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

  // fCurrentMetronomeParentheses ??? JMI
  if (fCurrentMetronomeBeat.fBeatUnit.size()) { // JMI
    fCurrentMetronomeBeatsData.push_back(fCurrentMetronomeBeat);
    fCurrentMetronomeBeat.fBeatUnit = "";
    fCurrentMetronomeBeat.fDots = 0;
  }
  
  if (fCurrentMetronomeBeatsData.size() != 1) {
    msrMusicXMLWarning (
      inputLineNumber,
      "multiple beats found, but only per-minute tempo is supported");
    return;
  }
  
  if (! fCurrentMetrenomePerMinute) {
    msrMusicXMLWarning (
      inputLineNumber,
      "per-minute not found, only per-minute tempo is supported");
    return;
  }

  msrBeatData b = fCurrentMetronomeBeatsData[0];
  rational         r = 
    NoteType::type2rational(
      NoteType::xml (b.fBeatUnit)), rdot(3,2);
  
  while (b.fDots-- > 0) { // JMI
    r *= rdot;
  }
  r.rationalise ();

  fCurrentMetronomeTempo =
    msrTempo::create (
      inputLineNumber,
      r.getDenominator(), fCurrentMetrenomePerMinute);

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentDirectionStaffNumber,
        fCurrentVoiceNumber);

  currentVoice->
    appendTempoToVoice (fCurrentMetronomeTempo);
  
  // JMI if (fCurrentOffset) addDelayed(cmd, fCurrentOffset);
}

void xml2MsrTranslator::visitStart ( S_beat_unit& elt )
{ 
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_beat_unit" <<
      endl;

  if (fCurrentMetronomeBeat.fBeatUnit.size()) {
    fCurrentMetronomeBeatsData.push_back (fCurrentMetronomeBeat); 
    fCurrentMetronomeBeat.fBeatUnit = "";
    fCurrentMetronomeBeat.fDots = 0;
  }
  
  fCurrentMetronomeBeat.fBeatUnit = elt->getValue();
}

void xml2MsrTranslator::visitStart ( S_beat_unit_dot& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_beat_unit_dot" <<
      endl;

  fCurrentMetronomeBeat.fDots++;
}
  
void xml2MsrTranslator::visitStart ( S_per_minute& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_per_minute" <<
      endl;

  fCurrentMetrenomePerMinute = (int)(*elt);
}

void xml2MsrTranslator::visitEnd (S_direction& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_direction" <<
      endl;

  if (fCurrentMetronomeTempo) {
    if (fCurrentWordsContents.size ())
      fCurrentMetronomeTempo->
        setTempoIndication (fCurrentWordsContents);
  }

  fOnGoingDirection = false;
}

//________________________________________________________________________
void xml2MsrTranslator::visitStart (S_staves& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_direction" <<
      endl;

  int stavesNumber = int(*elt);

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTraceStaves) {
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
          inputLineNumber,
          msrStaff::kRegularStaff,
          n);
      n++;
    } // while
  }
}

//________________________________________________________________________
void xml2MsrTranslator::visitStart (S_staff& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_staff" <<
      endl;

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

  if (gGeneralOptions->fTraceStaves) {
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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_staff_details" <<
      endl;

  fStaffDetailsStaffNumber =
    elt->getAttributeIntValue ("number", 1);

  // show-frets JMI

  idtr++;

  if (gGeneralOptions->fTraceStaves) {
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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_staff_type" <<
      endl;

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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_staff_lines" <<
      endl;

  fCurrentStaffTuningLinesNumber = (int)(*elt);
}

void xml2MsrTranslator::visitStart (S_staff_tuning& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_staff_tuning" <<
      endl;

  fCurrentStaffTuningLine =
    elt->getAttributeIntValue ("line", 0);

  fCurrentStaffTuningAlteration = kNatural; // may be absent
  fCurrentStaffTuningOctave     = -1;
}
    
void xml2MsrTranslator::visitStart (S_tuning_step& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_tuning_step" <<
      endl;

  string tuningStep = elt->getValue();

  checkStep (
    elt->getInputLineNumber (),
    tuningStep);

  fCurrentStaffTuningDiatonicPitch =
    msrDiatonicPitchFromString (
      tuningStep [0]);
}

void xml2MsrTranslator::visitStart (S_tuning_octave& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_tuning_octave" <<
      endl;

  fCurrentStaffTuningOctave = (int)(*elt);
}

void xml2MsrTranslator::visitStart (S_tuning_alter& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_tuning_alter" <<
      endl;

  float tuningAlter = (float)(*elt);

  fCurrentStaffTuningAlteration =
    msrAlterationFromMusicXMLAlter (
      tuningAlter);
      
  if (fCurrentStaffTuningAlteration == k_NoAlteration) {
    stringstream s;

    s <<
      "tuning alter '" << tuningAlter << "'"
      "' should be -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5 or +2";
      
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }
}

void xml2MsrTranslator::visitEnd (S_staff_tuning& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_staff_tuning" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  // fetch relevant staff
  S_msrStaff
    staff =
      createStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fStaffDetailsStaffNumber);

  msrQuartertonesPitch
    quartertonesPitch =
      quatertonesPitchFromDiatonicPitchAndAlteration (
        inputLineNumber,
        fCurrentStaffTuningDiatonicPitch,
        fCurrentStaffTuningAlteration);
    

  // create the staff tuning
  if (gGeneralOptions->fTraceStafftuning) {
    cerr <<
      idtr <<
        setw(30) << "Creating staff tuning:" <<
        endl;

    idtr++;

    cerr <<
      idtr <<
        setw(34) << "fCurrentStaffTuningLine" << " = " <<
        fCurrentStaffTuningLine <<
        endl <<
      idtr <<
        setw(34) << "fCurrentStaffTuningDiatonicPitch" << " = " <<
        msrDiatonicPitchAsString (
          gMsrOptions->fMsrQuatertonesPitchesLanguage,
          fCurrentStaffTuningDiatonicPitch) <<
        endl <<
      idtr <<
        setw(34) << "fCurrentStaffTuningAlteration" << " = " <<
        msrAlterationAsString (
          fCurrentStaffTuningAlteration) <<
        endl <<
      idtr <<
        setw(34) << "quartertonesPitch" << " = " <<
        msrQuartertonesPitchAsString (
          gMsrOptions->fMsrQuatertonesPitchesLanguage,
          quartertonesPitch) <<
        endl <<
      idtr <<
        setw(34) << "CurrentStaffTuningOctave" << " = " <<
        fCurrentStaffTuningOctave <<
        endl;

    idtr--;
  }
    
  S_msrStafftuning
    stafftuning =
      msrStafftuning::create (
        inputLineNumber,
        fCurrentStaffTuningLine,
        quartertonesPitch,
        fCurrentStaffTuningOctave);
        
  // add it to the staff
  staff->
    addStafftuningToStaff (stafftuning);
}
    
void xml2MsrTranslator::visitStart (S_capo& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_capo" <<
      endl;

  fCurrentStaffDetailsCapo = (int)(*elt);
}

void xml2MsrTranslator::visitStart (S_staff_size& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_staff_size" <<
      endl;

  fCurrentStaffDetailsStaffSize = (int)(*elt);
}

void xml2MsrTranslator::visitEnd (S_staff_details& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_staff_details" <<
      endl;

  if (gGeneralOptions->fTraceStafftuning) {
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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_staff_details" <<
      endl;

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
  
    if (gGeneralOptions->fTraceNotes && gGeneralOptions->fTraceVoices)
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
  
    if (gGeneralOptions->fTraceNotes && gGeneralOptions->fTraceVoices)
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
      inputLineNumber,
      s.str());
  }
}

//________________________________________________________________________
void xml2MsrTranslator::visitStart (S_backup& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_backup" <<
      endl;

/*
 The backup and forward elements are required to coordinate multiple voices in one part, including music on multiple staves. The backup type is generally used to move between voices and staves. Thus the backup element does not include voice or staff elements. Duration values should always be positive, and should not cross measure boundaries or mid-measure changes in the divisions value.
 
      <backup>
        <duration>8</duration>
      </backup>
*/

  // handle the pending tuplets if any
  handleTupletsPendingOnTupletStack (
    elt->getInputLineNumber ());
  
  fOnGoingBackup = true;
}

void xml2MsrTranslator::visitEnd (S_backup& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_backup" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTraceMeasures)
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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_forward" <<
      endl;

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
  
  // handle the pending tuplets if any
  handleTupletsPendingOnTupletStack (
    elt->getInputLineNumber ());
  
  fOnGoingForward = true;
}

void xml2MsrTranslator::visitEnd ( S_forward& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_forward" <<
      endl;

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

  if (gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Handling 'forward >>> " <<
      fCurrentForwardDuration <<
      "', thus switching to voice \"" <<
      currentVoice->getVoiceName () <<
      "\" in staff \"" << staff->getStaffName () << "\"" <<
      endl;
  
  fOnGoingForward = false;
}

//________________________________________________________________________
void xml2MsrTranslator::visitStart (S_tied& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_tied" <<
      endl;

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
    if (! fOnGoingSlur) {
      if (tiedType.size()) {
        stringstream s;
        
        s << "tied type" << fCurrentSlurType << "unknown";
        
        msrMusicXMLError (
          elt->getInputLineNumber (),
          s.str());
      }
    }
      
    // inner tied notes may miss the "continue" type:
    // let' complain on ligature notes outside of ligatures 
    if (! fOnGoingLigature) {
      if (tiedType.size()) {
        stringstream s;
        
        s << "tied type" << fCurrentSlurType << "unknown";
        
        msrMusicXMLError (
          elt->getInputLineNumber (),
          s.str());
      }
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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_slur" <<
      endl;

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

//________________________________________________________________________
void xml2MsrTranslator::visitStart (S_bracket& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_bracket" <<
      endl;

  fCurrentLigatureNumber =
    elt->getAttributeIntValue ("number", 0);

  fCurrentLigatureType =
    elt->getAttributeValue ("type");

  fCurrentLigaturePlacement =
    elt->getAttributeValue ("placement");

  if      (fCurrentLigatureType == "start") {
    
    fCurrentLigatureKind = msrLigature::kStartLigature;
    fOnGoingLigature = true;
    
  }
  else if (fCurrentLigatureType == "continue") { // JMI ???
    
    fCurrentLigatureKind = msrLigature::kContinueLigature;
    
  }
  else if (fCurrentLigatureType == "stop") {
    
    fCurrentLigatureKind = msrLigature::kStopLigature;
    fOnGoingLigature = false;
    
  }
  else {

    // inner ligature notes may miss the "continue" type:
    // let' complain on ligature notes outside of ligatures 
    if (! fOnGoingLigature)
      if (fCurrentLigatureType.size()) {
        stringstream s;
        
        s << "ligature type" << fCurrentLigatureType << "unknown";
        
        msrMusicXMLError (
          elt->getInputLineNumber (),
          s.str());
      }
  }

  S_msrLigature
    ligature =
      msrLigature::create(
        elt->getInputLineNumber (),
        fCurrentLigatureNumber,
        fCurrentLigatureKind);
        
  fPendingLigatures.push_back (ligature);
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_wedge& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_wedge" <<
      endl;

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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_lyric" <<
      endl;

  int stanzaNumber =
    elt->getAttributeIntValue ("number", 0);
  
  if (stanzaNumber < 0) {
    stringstream s;

    s <<
      "lyric number " << stanzaNumber <<
      " is not positive";

    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }

  if (stanzaNumber > 0) {
    if (gGeneralOptions->fTraceLyrics)
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
  fCurrentLyricElision = false;

  fCurrentNoteHasStanza = true;

  fOnGoingLyric = true;
}

void xml2MsrTranslator::visitStart ( S_syllabic& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_syllabic" <<
      endl;

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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_text" <<
      endl;

  string text = elt->getValue();

/* JMI
  // text may be composed of only spaces, so:
  string dest;
  for_each (
    text.begin(), text.end(), stringSpaceRemover (dest));

  if (fCurrentLyricElision)
    fCurrentLyricText += " " + dest; // append to a list? JMI
  else
    fCurrentLyricText = dest;
*/

/*
  string textToUse;
  
  // text may be composed of only spaces, dont' skim them
  for (
    string::const_iterator i = text.begin ();
    i != text.end ();
    i++) {

    if ((*i) == ' ')
      textToUse += "@"; // TEMP JMI
    else
      textToUse += (*i);
  } // for

  // there can be several <text/>'s in a row, hence the concatenation
  if (fCurrentLyricElision)
    fCurrentLyricText += " " + textToUse; // append to a list? JMI
  else
    fCurrentLyricText += textToUse;
*/

  // there can be several <text/>'s in a row, hence the concatenation
  fCurrentLyricText += text;
  
  fCurrentStanzaHasText = true;

  if (gGeneralOptions->fTraceLyrics)
    cerr << idtr <<
      "--> line " << right << setw(5) << elt->getInputLineNumber () <<
      ", fCurrentStanzaNumber" << " = " << fCurrentStanzaNumber <<
      ", fCurrentSyllabic" << " = " << left << setw(6) << fCurrentSyllabic <<
      ", fCurrentLyricText" << " = |" << fCurrentLyricText << "|" <<
      endl;
}

void xml2MsrTranslator::visitStart ( S_elision& elt ) 
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_elision" <<
      endl;

  fCurrentLyricElision = true;
}

void xml2MsrTranslator::visitStart ( S_extend& elt ) 
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_extend" <<
      endl;

  string extendType =
    elt->getAttributeValue ("type");

  if (fOnGoingLyric) {
    if      (extendType == "start") {
      fCurrentSyllableExtendKind =
        msrSyllable::kStartSyllableExtend;
    }
    else if (extendType == "continue") {
      fCurrentSyllableExtendKind =
        msrSyllable::kContinueSyllableExtend;
    }
    else if (extendType == "stop") {
      fCurrentSyllableExtendKind =
        msrSyllable::kStopSyllableExtend;
    }
    else if (extendType.size()) {
        stringstream s;
        
        s << "extend type" << extendType << "unknown";
        
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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_lyric" <<
      endl;

  int inputLineNumber =
      elt->getInputLineNumber ();

  if (gGeneralOptions->fTraceLyrics) {
    cerr <<
      endl <<
      idtr <<
        "visitEnd ( S_lyric& )" <<
        ", line = " << inputLineNumber << ", with:" <<
        endl;

    idtr++;

    cerr << idtr <<
      "Lyric data:" <<
      endl;

    {
      idtr++;

      const int width = 26;
  
      cerr <<
        idtr <<
          setw(width) << "fCurrentStanzaNumber" << " = " << fCurrentStanzaNumber <<
          endl <<
        idtr <<
          setw(width) << "fCurrentLyricText" << " = \"" << fCurrentLyricText << "\"" <<
          endl <<
        idtr <<
          setw(width) << "fCurrentLyricElision" << " = " << fCurrentLyricElision <<
          endl <<
        idtr <<
          setw(width) << "fCurrentSyllableExtendKind" << " = " <<
            fCurrentSyllableExtendKind <<
          endl <<
        idtr <<
          setw(width) << "fCurrentNoteIsARest" << " = " <<
          booleanAsString (fCurrentNoteIsARest) <<
          endl <<
        idtr <<
          setw(width) << "fCurrentRestMeasure" << " = " <<
          booleanAsString (fCurrentRestMeasure) <<
          endl;
  
      cerr <<
        idtr <<
          setw(width) << "fCurrentTieKind" << " = " <<
          msrTie::tieKindAsString (fCurrentTieKind) <<
        endl;
          
      cerr <<
        idtr <<
          setw(width) << "fCurrentSlurKind" << " = \"" <<
          msrSlur::slurKindAsString (fCurrentSlurKind) <<
          "\"" <<
          endl;
  
      cerr <<
        idtr <<
          setw(width) << "fCurrentLigatureKind" << " = \"" <<
          msrLigature::ligatureKindAsString (fCurrentLigatureKind) <<
          "\"" <<
          endl;
  
      cerr <<
        idtr <<
          setw(width) <<
          "fOnGoingSlur" << " = " << fOnGoingSlur <<
          endl <<
        idtr <<
          setw(width) <<
          "fOnGoingSlurHasStanza" << " = " << fOnGoingSlurHasStanza <<
          endl;
  
      cerr <<
        idtr <<
          setw(width) <<
          "fOnGoingLigature" << " = " << fOnGoingLigature <<
          endl <<
        idtr <<
          setw(width) <<
          "fOnGoingLigatureHasStanza" << " = " << fOnGoingLigatureHasStanza <<
          endl;
  
      cerr <<
        idtr <<
          setw(width) << "fFirstSyllableInSlurKind" << " = \"" <<
          fFirstSyllableInSlurKind << // JMI->syllableKindAsString () <<
          "\"" <<
      endl;
  
      cerr <<
        idtr <<
          setw(width) << "fFirstSyllableInLigatureKind" << " = \"" <<
          fFirstSyllableInLigatureKind << // JMI->syllableKindAsString () <<
          "\"" <<
      endl;
  
      cerr <<
        idtr <<
          setw(width) << "fCurrentSyllableKind" << " = \""<<
          fCurrentSyllableKind << // JMI ->syllableKindAsString () <<
          "\"" <<
      endl;
          
      idtr--;
    }
    
    idtr--;
  }

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentNoteStaffNumber,
        fCurrentVoiceNumber);

  // fetch stanzaNumber in current voice
  S_msrStanza
    stanza =
      currentVoice->
        createStanzaInVoiceIfNeeded (
          inputLineNumber,
          fCurrentStanzaNumber);

  S_msrSyllable
    syllable;

  if (fCurrentSyllableKind != msrSyllable::k_NoSyllable) {
    fCurrentSyllableKind = msrSyllable::kSingleSyllable;
    
    // the presence of a '<lyric />' ends the effect
    // of an on going syllable extend
    fOnGoingSyllableExtend = false;
    
    if (fOnGoingSlur)
      fOnGoingSlurHasStanza = true;
      
    if (fOnGoingLigature)
      fOnGoingLigatureHasStanza = true;
      
    fCurrentNoteHasStanza = true;
  }
  
  else {

    if (
      fCurrentSlurKind == msrSlur::kStartSlur
        &&
      fCurrentNoteHasStanza) { // JMI
    }
    
    if (
      fCurrentLigatureKind == msrLigature::kStartLigature
        &&
      fCurrentNoteHasStanza) { // JMI
    }
    
    if (fCurrentTieKind != msrTie::k_NoTie) {
      fCurrentSyllableKind = msrSyllable::kTiedSyllable;
    }
  
    else if (fCurrentNoteIsARest) {
      fCurrentSyllableKind = msrSyllable::kRestSyllable;
    }
  
    else if (
      fOnGoingSlurHasStanza // JMI Ligature ???
        &&
      ! fCurrentLyricText.size ()) {
      if (fFirstSyllableInSlurKind == msrSyllable::kEndSyllable) {
        fCurrentSyllableKind = msrSyllable::kSlurBeyondEndSyllable;
      }
      else {
        fCurrentSyllableKind = msrSyllable::kSlurSyllable;
      }
    }
    
    else if (fOnGoingSlur) {
      if (fFirstSyllableInSlurKind == msrSyllable::kEndSyllable) {
        fCurrentSyllableKind = msrSyllable::kSlurBeyondEndSyllable;
      }
      else {
        fCurrentSyllableKind = msrSyllable::kSlurSyllable;
      }
    }
    
    else { // JMI
    }
  }

  if (gGeneralOptions->fTraceLyrics)    
    cerr <<
      idtr <<
        "@@@@@@@@@@ visitEnd ( S_lyric&), fCurrentSyllableKind = " <<
        msrSyllable::syllableKindAsString (fCurrentSyllableKind) <<
        endl;


  if (fCurrentSyllableKind != msrSyllable::k_NoSyllable) {
   // create a syllable
   //     fCurrentLyricElision ??? JMI
    if (gGeneralOptions->fTraceLyrics) {      
      cerr << idtr <<
        "--> creating a " <<
        msrSyllable::syllableKindAsString (fCurrentSyllableKind) << "\"" <<
        " syllable"
        ", text = \"" << fCurrentLyricText << "\"" <<
        ", line " << inputLineNumber <<
        ", divisions = " << fCurrentNoteSoundingDivisions << 
        ", syllabic = \"" << fCurrentSyllableKind << "\"" <<
        ", elision: " << fCurrentLyricElision << 
        " in stanza " << stanza->getStanzaName () <<
        endl;
    }
    
    syllable =
      msrSyllable::create (
        inputLineNumber,
        fCurrentPart,
        fCurrentSyllableKind,
        fCurrentLyricText,
        msrSyllable::k_NoSyllableExtend,
        fCurrentNoteSoundingDivisions,
        stanza);

/* JMI
    // set syllable's note direct uplink
    syllable->
      setSyllableDirectPartUplink (
        fCurrentPart);
      */
      
    // register syllable in current note's syllables list
    fCurrentNoteSyllables.push_back (syllable);
  }

  fOnGoingLyric = false;
}

//________________________________________________________________________
void xml2MsrTranslator::visitStart (S_measure& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_measure" <<
      endl;

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
  if (gGeneralOptions->fTraceDetailed) {
    set<int>::iterator
      it =
        gGeneralOptions->
          fTraceAllMeasureNumbersSet.find (measureNumber);
          
    if (it != gGeneralOptions->fTraceAllMeasureNumbersSet.end ()) {
      // yes, activate detailed trace for it
      gGeneralOptions = gGeneralOptionsWithDetailedTrace;
    }
  }

  if (gGeneralOptions->fTraceMeasures) {
    cerr <<
      endl <<
      idtr << 
      "<!--=== measure " << measureNumber <<
      ", line = " << inputLineNumber << " ===-->" <<
      endl;
  }
}

void xml2MsrTranslator::visitEnd (S_measure& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_measure" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (fCurrentATupletStopIsPending) {

    if (fTupletsStack.size ()) // JMI
      // finalize the tuplet, only now in case the last element
      // is actually a chord
      finalizeTuplet (inputLineNumber);

    fCurrentATupletStopIsPending = false;
  }

  // finalize last part measure to update master voice
  fCurrentPart->
    finalizeLastMeasureOfPart (
      inputLineNumber);

  // restore debug options in case they were set in visitStart()
  gGeneralOptions = gGeneralOptionsUserChoices;
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_print& elt ) 
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_print" <<
      endl;

  const string newSystem = elt->getAttributeValue ("new-system");
  
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (newSystem.size()) {
    
    if (newSystem == "yes") {
      
      int inputLineNumber =
        elt->getInputLineNumber ();
      
      // create a barnumbercheck command
      if (gGeneralOptions->fTraceMeasures) {
        cerr << idtr << 
          "Creating a barnumber check, " <<
          "line = " << inputLineNumber <<
          endl;
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
            inputLineNumber,
            currentVoice->
              getVoiceMeasureNumber ());
            
      // append it to the voice
// JMI      S_msrElement bnc = barnumbercheck_;
      currentVoice->
        appendBarnumberCheckToVoice (barnumbercheck_);
  
      // create a break command
      if (gGeneralOptions->fTraceMeasures) {
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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_barline" <<
      endl;

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

  fCurrentBarlineEndingNumber    = ""; // may be "1, 2"

  fCurrentBarlineHasSegno = false;
  fCurrentBarlineHasCoda  = false;

  fCurrentBarlineLocation        = msrBarline::k_NoLocation;
  fCurrentBarlineStyle           = msrBarline::k_NoStyle;
  fCurrentBarlineEndingType      = msrBarline::k_NoEndingType;
  fCurrentBarlineRepeatDirection = msrBarline::k_NoRepeatDirection;
  fCurrentBarlineRepeatWinged    = msrBarline::k_NoRepeatWinged;

  string
    location =
      elt->getAttributeValue ("location");

  fCurrentBarlineLocation =
    msrBarline::kRight; // by default
    
  if       (location == "left") {
    fCurrentBarlineLocation =
      msrBarline::kLeft;
  }
  else  if (location == "middle") {
    fCurrentBarlineLocation =
      msrBarline::kMiddle;
  }
  else if  (location == "right") {
    fCurrentBarlineLocation =
      msrBarline::kRight;
  }
  else {
    stringstream s;
    
    s <<
      "barline location " << location << " is unknown";
      
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }

  fOnGoingBarline = true;
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_bar_style& elt ) 
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_bar_style" <<
      endl;

  string barStyle = elt->getValue();

  fCurrentBarlineStyle =
    msrBarline::k_NoStyle;

  if      (barStyle == "regular") {
    fCurrentBarlineStyle =
      msrBarline::kRegular;
  }
  else if (barStyle == "dotted") {
    fCurrentBarlineStyle =
      msrBarline::kDotted;
  }
  else if (barStyle == "dashed") {
    fCurrentBarlineStyle =
      msrBarline::kDashed;
  }
  else if (barStyle == "heavy") {
    fCurrentBarlineStyle =
      msrBarline::kHeavy;
  }
  else if (barStyle == "light-light") {
    fCurrentBarlineStyle =
      msrBarline::kLightLight;
  }
  else if (barStyle == "light-heavy") {
    fCurrentBarlineStyle =
      msrBarline::kLightHeavy;
  }
  else if (barStyle == "heavy-light") {
    fCurrentBarlineStyle =
      msrBarline::kHeavyLight;
  }
  else if (barStyle == "heavy-heavy") {
    fCurrentBarlineStyle =
      msrBarline::kHeavyHeavy;
  }
  else if (barStyle == "tick") {
    fCurrentBarlineStyle =
      msrBarline::kTick;
  }
  else if (barStyle == "short") {
    fCurrentBarlineStyle =
      msrBarline::kShort;
  }
  else {
    msrMusicXMLError (
      elt->getInputLineNumber (),
      "barline style " + barStyle + " is unknown");
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_segno& elt ) 
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_segno" <<
      endl;

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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_coda" <<
      endl;

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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_eyeglasses" <<
      endl;

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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_pedal" <<
      endl;

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
        createVoiceInStaffInCurrentPartIfNeeded (
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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_ending" <<
      endl;

  fCurrentBarlineEndingNumber =
    elt->getAttributeValue ("number"); // may be "1, 2"

  string type =
    elt->getAttributeValue ("type");
      
  if       (type == "start") {
    fCurrentBarlineEndingType =
      msrBarline::kStart;
  }
  else  if (type == "stop") {
    fCurrentBarlineEndingType =
      msrBarline::kStop;
  }
  else  if (type == "discontinue") {
    fCurrentBarlineEndingType =
      msrBarline::kDiscontinue;
  }
  else {
    stringstream s;
    
    s <<
      "ending type " <<
      type <<
      " is unknown";
    
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_repeat& elt ) 
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_repeat" <<
      endl;

  string direction =
    elt->getAttributeValue ("direction");

  string winged =
    elt->getAttributeValue ("winged");

  int inputLineNumber =
    elt->getInputLineNumber ();

  fCurrentBarlineRepeatDirection =
    msrBarline::k_NoRepeatDirection;
    
  if       (direction == "forward") {
    fCurrentBarlineRepeatDirection =
      msrBarline::kForward;
  }
  else  if (direction == "backward") {
    fCurrentBarlineRepeatDirection =
      msrBarline::kBackward;
  }
  else {
    stringstream s;
    
    s <<
      "repeat direction '" <<
      direction <<
      "' is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());
  }

  fCurrentBarlineRepeatWinged =
    msrBarline::k_NoRepeatWinged;

  if (winged.size()) {
    if       (winged == "none") {
      fCurrentBarlineRepeatWinged =
        msrBarline::kNone;
    }
    else if (winged == "straight") {
      fCurrentBarlineRepeatWinged =
        msrBarline::kStraight;
    }
    else  if (winged == "curved") {
      fCurrentBarlineRepeatWinged =
        msrBarline::kCurved;
    }
    else  if (winged == "doubleStraight") {
      fCurrentBarlineRepeatWinged =
        msrBarline::kDoubleStraight;
    }
    else  if (winged == "doubleCurved") {
      fCurrentBarlineRepeatWinged =
        msrBarline::kDoubleCurved;
    }
    else {
      stringstream s;
      
      s <<
        "repeat winged '" <<
        winged <<
        "' is unknown";
      
      msrMusicXMLError (
        inputLineNumber,
        s.str());
    }
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::visitEnd ( S_barline& elt ) 
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_barline" <<
      endl;

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
        inputLineNumber,
        fCurrentBarlineHasSegno,
        fCurrentBarlineHasCoda,
        fCurrentBarlineLocation,
        fCurrentBarlineStyle,
        fCurrentBarlineEndingType,
        fCurrentBarlineEndingNumber,
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
              "--> barline, right:" <<
      endl;
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
  if (gGeneralOptions->fTraceMeasures) {
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
      idtr << "location = " << fCurrentBarlineLocation << endl <<
      idtr << "style = " << fCurrentBarlineStyle << endl <<
      idtr << "ending type = " << fCurrentBarlineEndingType << endl <<
      idtr << "ending number = " << fCurrentBarlineEndingNumber << endl <<
      idtr << "repeat direction = " << fCurrentBarlineRepeatDirection << endl <<
      idtr << "repeat winged = " << fCurrentBarlineRepeatWinged;
      
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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_note" <<
      endl;

  // initialize note data to a neutral state
  initializeNoteData ();

  fCurrentNoteDiatonicPitch = k_NoDiatonicPitch;
  fCurrentNoteAlteration    = kNatural;

  // assuming staff number 1, unless S_staff states otherwise afterwards
  fCurrentStaffNumber = 1;

  // assuming voice number 1, unless S_voice states otherwise afterwards
  fCurrentVoiceNumber = 1;

  fCurrentActualNotes = -1;
  fCurrentNormalNotes = -1;

  // keep fCurrentStanzaNumber unchanged,
  // for use by notes without lyrics
  
  fCurrentSyllabic = "";
  fCurrentLyricText = "";
  fCurrentSyllableKind = msrSyllable::k_NoSyllable;
    // to handle properly a note without any <text/> JMI
  fCurrentSyllableExtendKind = msrSyllable::k_NoSyllableExtend;
  
  // assume this note hasn't got any stanzas until S_lyric is met
  fCurrentNoteHasStanza = false;

  fCurrentStem = 0;

  fCurrentMusicXMLTremoloType = k_NoTremolo;

  fCurrentTie = 0;
  fCurrentTiedOrientation = "";
  
  fCurrentSlurNumber = -1;
  fCurrentSlurType = "";
  fCurrentSlurPlacement = "";
  fCurrentSlurKind = msrSlur::k_NoSlur;

  fCurrentLigatureNumber = -1;
  fCurrentLigatureType = "";
  fCurrentLigaturePlacement = "";
  fCurrentLigatureKind = msrLigature::k_NoLigature;

  fCurrentNoteStaffNumber = 1; // it may be absent
  fCurrentNoteVoiceNumber = 1; // it may be absent
  
  fOnGoingNote = true;
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_step& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_step" <<
      endl;

  string step = elt->getValue();
  
  checkStep (
    elt->getInputLineNumber (),
    step);

  fCurrentNoteDiatonicPitch =
    msrDiatonicPitchFromString (step [0]);
}

void xml2MsrTranslator::visitStart ( S_alter& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_alter" <<
      endl;

  float alter = (float)(*elt);

  fCurrentNoteAlteration =
    msrAlterationFromMusicXMLAlter (
      alter);
      
  if (fCurrentNoteAlteration == k_NoAlteration) {
    stringstream s;

    s <<
      "alter '" << alter << "'"
      "' should be -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5 or +2";
      
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }
}

void xml2MsrTranslator::visitStart ( S_octave& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_octave" <<
      endl;

  fCurrentNoteOctave = (int)(*elt);
}

void xml2MsrTranslator::visitStart ( S_duration& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_duration" <<
      endl;

  int duration = (int)(*elt);

  if      (fOnGoingBackup) {
  
    fCurrentBackupDuration = duration;

  }
  
  else if (fOnGoingForward) {
  
    fCurrentForwardDuration = duration;
    
  }
  
  else if (fOnGoingNote) {
  
    fCurrentNoteSoundingDivisions = duration;
    
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

void xml2MsrTranslator::visitStart ( S_instrument& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_instrument" <<
      endl;

/*
  <instrument id="P2-I4"/>
*/
 string id = elt->getAttributeValue ("id"); // JMI
}
       
void xml2MsrTranslator::visitStart ( S_dot& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_dot" <<
      endl;

  fCurrentNoteDotsNumber++;
}
       
void xml2MsrTranslator::visitStart ( S_type& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_type" <<
      endl;

/*
 Type indicates the graphic note type, Valid values (from shortest to longest) are 1024th, 512th, 256th, 128th, 64th, 32nd, 16th, eighth, quarter, half, whole, breve, long, and maxima. The size attribute indicates full, cue, or large size, with full the default for regular notes and cue the default for cue and grace notes.
*/

  string noteType = elt->getValue();


  if      (noteType == "maxima")  { fCurrentNoteGraphicDuration = kMaxima; }
  else if (noteType == "long")    { fCurrentNoteGraphicDuration = kLong; }
  else if (noteType == "breve")   { fCurrentNoteGraphicDuration = kBreve; } 
  else if (noteType == "whole")   { fCurrentNoteGraphicDuration = kWhole; } 
  else if (noteType == "half")    { fCurrentNoteGraphicDuration = kHalf; } 
  else if (noteType == "quarter") { fCurrentNoteGraphicDuration = kQuarter; } 
  else if (noteType == "eighth")  { fCurrentNoteGraphicDuration = kEighth; } 
  else if (noteType == "16th")    { fCurrentNoteGraphicDuration = k16th; } 
  else if (noteType == "32nd")    { fCurrentNoteGraphicDuration = k32nd; } 
  else if (noteType == "64th")    { fCurrentNoteGraphicDuration = k64th; } 
  else if (noteType == "128th")   { fCurrentNoteGraphicDuration = k128th; } 
  else if (noteType == "256th")   { fCurrentNoteGraphicDuration = k256th; } 
  else if (noteType == "512th")   { fCurrentNoteGraphicDuration = k512th; } 
  else if (noteType == "1024th")  { fCurrentNoteGraphicDuration = k1024th; }
  else {
    stringstream s;
    
    s <<
      endl << 
      "--> unknown note type " << noteType <<
      endl;

    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }

  string noteTypeSize = elt->getAttributeValue ("size");

  if (noteTypeSize == "cue") {
     // USE IT! JMI ???
  }

  else {
    if (noteTypeSize.size ())
      msrMusicXMLError (
        elt->getInputLineNumber (),
          "unknown note type size \"" + noteTypeSize + "\"");
  }  
}

void xml2MsrTranslator::visitStart ( S_stem& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_stem" <<
      endl;

  string        stem = elt->getValue();

  msrStem::msrStemKind stemKind = msrStem::k_NoStem;
  
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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_beam" <<
      endl;

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
    S_msrBeam
      beam =
        msrBeam::create (
          inputLineNumber,
          fCurrentBeamNumber,
          beamKind);

    fCurrentBeams.push_back (beam);
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_measure_style& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_measure_style" <<
      endl;
}
       
void xml2MsrTranslator::visitStart ( S_beat_repeat& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_beat_repeat" <<
      endl;

  fCurrentBeatRepeatSlashes = elt->getAttributeIntValue ("slashes", 0);

  string beatRepeatUseDots = elt->getAttributeValue ("use-dots");

  if      (beatRepeatUseDots == "yes")
    fCurrentTupletKind = msrTuplet::kStartTuplet;
  else if (beatRepeatUseDots == "no")
    fCurrentTupletKind = msrTuplet::kStopTuplet;
  else {
    stringstream s;
    
    s << "beat repeat use dots " << beatRepeatUseDots << " is unknown";
    
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }
}
       
void xml2MsrTranslator::visitStart ( S_measure_repeat& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_measure_repeat" <<
      endl;

  string measureRepeatType = elt->getAttributeValue ("type");

  if      (measureRepeatType == "start")
    fCurrentTupletKind = msrTuplet::kStartTuplet;
  else if (measureRepeatType == "stop")
    fCurrentTupletKind = msrTuplet::kStopTuplet;
  else {
    stringstream s;
    
    s << "measure repeat type " << measureRepeatType << " is unknown";
    
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }

  fCurrentMeasureRepeatSlashes = elt->getAttributeIntValue ("slashes", 0);
}
       
void xml2MsrTranslator::visitStart ( S_multiple_rest& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_multiple_rest" <<
      endl;

  fCurrentMultipleRestCounter = (int)(*elt);

  string multipleRestUseSymbols = elt->getAttributeValue ("use-symbolss");

  if      (multipleRestUseSymbols == "yes")
    fCurrentTupletKind = msrTuplet::kStartTuplet;
  else if (multipleRestUseSymbols == "no")
    fCurrentTupletKind = msrTuplet::kStopTuplet;
  else {
    stringstream s;
    
    s << "multiple rest use symbols " << multipleRestUseSymbols << " is unknown";
    
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }
}
       
void xml2MsrTranslator::visitStart ( S_slash& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_slash" <<
      endl;

  string slashType = elt->getAttributeValue ("type");

  if      (slashType == "start")
    fCurrentTupletKind = msrTuplet::kStartTuplet;
  else if (slashType == "stop")
    fCurrentTupletKind = msrTuplet::kStopTuplet;
  else {
    stringstream s;
    
    s << "slash type " << slashType << " is unknown";
    
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }

  string slashUseDots = elt->getAttributeValue ("use-dots");

  if      (slashUseDots == "yes")
    fCurrentTupletKind = msrTuplet::kStartTuplet;
  else if (slashUseDots == "no")
    fCurrentTupletKind = msrTuplet::kStopTuplet;
  else {
    stringstream s;
    
    s << "slash use dots " << slashUseDots << " is unknown";
    
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }

  string slashUseStems = elt->getAttributeValue ("use-stems");

  if      (slashUseStems == "yes")
    fCurrentTupletKind = msrTuplet::kStartTuplet;
  else if (slashUseStems == "no")
    fCurrentTupletKind = msrTuplet::kStopTuplet;
  else {
    stringstream s;
    
    s << "slash use stems " << slashUseStems << " is unknown";
    
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_articulations& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_articulations" <<
      endl;
}

void xml2MsrTranslator::visitStart ( S_accent& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_accent" <<
      endl;

  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kAccent);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_breath_mark& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_breath_mark" <<
      endl;

  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kBreathMark);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_caesura& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_caesura" <<
      endl;

  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kCaesura);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_spiccato& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_spiccato" <<
      endl;

  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kSpiccato);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_staccato& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_staccato" <<
      endl;

  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kStaccato);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_staccatissimo& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_staccatissimo" <<
      endl;

  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kStaccatissimo);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_stress& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_stress" <<
      endl;

  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kStress);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_unstress& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_unstress" <<
      endl;

  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kUnstress);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_detached_legato& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_detached_legato" <<
      endl;

  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kDetachedLegato);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_strong_accent& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_strong_accent" <<
      endl;

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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_tenuto" <<
      endl;

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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_fermata" <<
      endl;

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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_arpeggiate" <<
      endl;

  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kArpeggiato);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_doit& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_doit" <<
      endl;

  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kDoit);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_falloff& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_falloff" <<
      endl;

  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kFalloff);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_plop& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_plop" <<
      endl;

  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kPlop);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitStart ( S_scoop& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_scoop" <<
      endl;

  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kScoop);
      
  fCurrentArticulations.push_back (articulation);
}

void xml2MsrTranslator::visitEnd ( S_articulations& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_articulations" <<
      endl;

  // JMI
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_technical& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_technical" <<
      endl;
}

void xml2MsrTranslator::visitEnd ( S_technical& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_technical" <<
      endl;
}

void xml2MsrTranslator::visitStart ( S_arrow& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_arrow" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  string placement = elt->getAttributeValue ("placement");

  msrTechnical::msrTechnicalPlacementKind
    arrowPlacementKind =
      msrTechnical::k_NoPlacementKind;

  if      (placement == "above")
    arrowPlacementKind = msrTechnical::kAbove;
    
  else if (placement == "below")
    arrowPlacementKind = msrTechnical::kBelow;
    
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kArrow,
        arrowPlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void xml2MsrTranslator::visitStart ( S_bend_alter& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_bend_alter" <<
      endl;

  fBendAlterValue = (int)(*elt);
}
  
void xml2MsrTranslator::visitStart ( S_bend& elt ) // JMI
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_bend" <<
      endl;
}

void xml2MsrTranslator::visitEnd ( S_bend& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_bend" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();

  string placement = elt->getAttributeValue ("placement");

  msrTechnicalWithInteger::msrTechnicalWithIntegerPlacementKind
    bendPlacementKind =
      msrTechnicalWithInteger::k_NoPlacementKind;

  if      (placement == "above")
    bendPlacementKind = msrTechnicalWithInteger::kAbove;
    
  else if (placement == "below")
    bendPlacementKind = msrTechnicalWithInteger::kBelow;
    
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  S_msrTechnicalWithInteger
    technicalWithInteger =
      msrTechnicalWithInteger::create (
        inputLineNumber,
        msrTechnicalWithInteger::kBend,
        fBendAlterValue,
        bendPlacementKind);
      
  fCurrentTechnicalWithIntegersList.push_back (technicalWithInteger);
}

void xml2MsrTranslator::visitStart ( S_double_tongue& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_double_tongue" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  string
    placement =
      elt->getAttributeValue ("placement");

  msrTechnical::msrTechnicalPlacementKind
    doubleTonguePlacementKind =
      msrTechnical::k_NoPlacementKind;

  if      (placement == "above")
    doubleTonguePlacementKind = msrTechnical::kAbove;
    
  else if (placement == "below")
    doubleTonguePlacementKind = msrTechnical::kBelow;
    
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kDoubleTongue,
        doubleTonguePlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void xml2MsrTranslator::visitStart ( S_down_bow& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_down_bow" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  string
    placement =
      elt->getAttributeValue ("placement");

  msrTechnical::msrTechnicalPlacementKind
    downBowPlacementKind =
      msrTechnical::k_NoPlacementKind;

  if      (placement == "above")
    downBowPlacementKind = msrTechnical::kAbove;
    
  else if (placement == "below")
    downBowPlacementKind = msrTechnical::kBelow;
    
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kDownBow,
        downBowPlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void xml2MsrTranslator::visitStart ( S_fingering& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_fingering" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  int fingeringValue = (int)(*elt);

  string placement = elt->getAttributeValue ("placement");

  msrTechnicalWithInteger::msrTechnicalWithIntegerPlacementKind
    fingeringPlacementKind =
      msrTechnicalWithInteger::k_NoPlacementKind;

  if      (placement == "above")
    fingeringPlacementKind = msrTechnicalWithInteger::kAbove;
    
  else if (placement == "below")
    fingeringPlacementKind = msrTechnicalWithInteger::kBelow;
    
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  S_msrTechnicalWithInteger
    technicalWithInteger =
      msrTechnicalWithInteger::create (
        inputLineNumber,
        msrTechnicalWithInteger::kFingering,
        fingeringValue,
        fingeringPlacementKind);
      
  fCurrentTechnicalWithIntegersList.push_back (technicalWithInteger);
}

void xml2MsrTranslator::visitStart ( S_fingernails& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_fingernails" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  string
    placement =
      elt->getAttributeValue ("placement");

  msrTechnical::msrTechnicalPlacementKind
    fingernailsPlacementKind =
      msrTechnical::k_NoPlacementKind;

  if      (placement == "above")
    fingernailsPlacementKind = msrTechnical::kAbove;
    
  else if (placement == "below")
    fingernailsPlacementKind = msrTechnical::kBelow;
    
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kFingernails,
        fingernailsPlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void xml2MsrTranslator::visitStart ( S_fret& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_fret" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();

  int fretValue = (int)(*elt);
    
  string placement = elt->getAttributeValue ("placement");

  msrTechnicalWithInteger::msrTechnicalWithIntegerPlacementKind
    fretPlacementKind =
      msrTechnicalWithInteger::k_NoPlacementKind;

  if      (placement == "above")
    fretPlacementKind = msrTechnicalWithInteger::kAbove;
    
  else if (placement == "below")
    fretPlacementKind = msrTechnicalWithInteger::kBelow;
    
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  S_msrTechnicalWithInteger
    technicalWithInteger =
      msrTechnicalWithInteger::create (
        inputLineNumber,
        msrTechnicalWithInteger::kFret,
        fretValue,
        fretPlacementKind);
      
  fCurrentTechnicalWithIntegersList.push_back (technicalWithInteger);
}

void xml2MsrTranslator::visitStart ( S_hammer_on& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_hammer_on" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();

  string hammerOnValue = elt->getValue ();
    
  string placement = elt->getAttributeValue ("placement");

  msrTechnicalWithString::msrTechnicalWithStringPlacementKind
    hammerOnPlacementKind =
      msrTechnicalWithString::k_NoPlacementKind;

  if      (placement == "above")
    hammerOnPlacementKind = msrTechnicalWithString::kAbove;
    
  else if (placement == "below")
    hammerOnPlacementKind = msrTechnicalWithString::kBelow;
    
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  S_msrTechnicalWithString
    technicalWithString =
      msrTechnicalWithString::create (
        inputLineNumber,
        msrTechnicalWithString::kHammerOn,
        hammerOnValue,
        hammerOnPlacementKind);
      
  fCurrentTechnicalWithStringsList.push_back (technicalWithString);
}

void xml2MsrTranslator::visitStart ( S_handbell& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_handbell" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  string handBellValue = elt->getValue ();
    
  string
    placement =
      elt->getAttributeValue ("placement");

  msrTechnicalWithString::msrTechnicalWithStringPlacementKind
    handbellPlacementKind =
      msrTechnicalWithString::k_NoPlacementKind;

  if      (placement == "above")
    handbellPlacementKind = msrTechnicalWithString::kAbove;
    
  else if (placement == "below")
    handbellPlacementKind = msrTechnicalWithString::kBelow;
    
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  S_msrTechnicalWithString
    technicalWithString =
      msrTechnicalWithString::create (
        inputLineNumber,
        msrTechnicalWithString::kHandbell,
        handBellValue,
        handbellPlacementKind);
      
  fCurrentTechnicalWithStringsList.push_back (technicalWithString);
}

void xml2MsrTranslator::visitStart ( S_harmonic& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_harmonic" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  string
    placement =
      elt->getAttributeValue ("placement");

  msrTechnical::msrTechnicalPlacementKind
    harmonicPlacementKind =
      msrTechnical::k_NoPlacementKind;

  if      (placement == "above")
    harmonicPlacementKind = msrTechnical::kAbove;
    
  else if (placement == "below")
    harmonicPlacementKind = msrTechnical::kBelow;
    
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kHarmonic,
        harmonicPlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void xml2MsrTranslator::visitStart ( S_heel& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_heel" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  string
    placement =
      elt->getAttributeValue ("placement");

  msrTechnical::msrTechnicalPlacementKind
    heelPlacementKind =
      msrTechnical::k_NoPlacementKind;

  if      (placement == "above")
    heelPlacementKind = msrTechnical::kAbove;
    
  else if (placement == "below")
    heelPlacementKind = msrTechnical::kBelow;
    
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kHeel,
        heelPlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void xml2MsrTranslator::visitStart ( S_hole& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_hole" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  string
    placement =
      elt->getAttributeValue ("placement");

  msrTechnical::msrTechnicalPlacementKind
    holePlacementKind =
      msrTechnical::k_NoPlacementKind;

  if      (placement == "above")
    holePlacementKind = msrTechnical::kAbove;
    
  else if (placement == "below")
    holePlacementKind = msrTechnical::kBelow;
    
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kHole,
        holePlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void xml2MsrTranslator::visitStart ( S_open_string& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_open_string" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  string
    placement =
      elt->getAttributeValue ("placement");

  msrTechnical::msrTechnicalPlacementKind
    openStringPlacementKind =
      msrTechnical::k_NoPlacementKind;

  if      (placement == "above")
    openStringPlacementKind = msrTechnical::kAbove;
    
  else if (placement == "below")
    openStringPlacementKind = msrTechnical::kBelow;
    
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kOpenString,
        openStringPlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void xml2MsrTranslator::visitStart ( S_other_technical& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_other_technical" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  string otherTechnicalValue = elt->getValue ();
    
  string
    placement =
      elt->getAttributeValue ("placement");

  msrTechnicalWithString::msrTechnicalWithStringPlacementKind
    otherTechnicalWithStringPlacementKind =
      msrTechnicalWithString::k_NoPlacementKind;

  if      (placement == "above")
    otherTechnicalWithStringPlacementKind = msrTechnicalWithString::kAbove;
    
  else if (placement == "below")
    otherTechnicalWithStringPlacementKind = msrTechnicalWithString::kBelow;
    
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  S_msrTechnicalWithString
    technicalWithString =
      msrTechnicalWithString::create (
        inputLineNumber,
        msrTechnicalWithString::kOtherTechnical,
        otherTechnicalValue,
        otherTechnicalWithStringPlacementKind);
      
  fCurrentTechnicalWithStringsList.push_back (technicalWithString);
}

void xml2MsrTranslator::visitStart ( S_pluck& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_pluck" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  string pluckValue = elt->getValue ();
    
  string
    placement =
      elt->getAttributeValue ("placement");

  msrTechnicalWithString::msrTechnicalWithStringPlacementKind
    pluckPlacementKind =
      msrTechnicalWithString::k_NoPlacementKind;

  if      (placement == "above")
    pluckPlacementKind = msrTechnicalWithString::kAbove;
    
  else if (placement == "below")
    pluckPlacementKind = msrTechnicalWithString::kBelow;
    
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  S_msrTechnicalWithString
    technicalWithString =
      msrTechnicalWithString::create (
        inputLineNumber,
        msrTechnicalWithString::kPluck,
        pluckValue,
        pluckPlacementKind);
      
  fCurrentTechnicalWithStringsList.push_back (technicalWithString);
}

void xml2MsrTranslator::visitStart ( S_pull_off& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_pull_off" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  string pullOffValue = elt->getValue ();
    
  string
    placement =
      elt->getAttributeValue ("placement");

  msrTechnicalWithString::msrTechnicalWithStringPlacementKind
    pullOffPlacementKind =
      msrTechnicalWithString::k_NoPlacementKind;

  if      (placement == "above")
    pullOffPlacementKind = msrTechnicalWithString::kAbove;
    
  else if (placement == "below")
    pullOffPlacementKind = msrTechnicalWithString::kBelow;
    
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  S_msrTechnicalWithString
    technicalWithString =
      msrTechnicalWithString::create (
        inputLineNumber,
        msrTechnicalWithString::kPullOff,
        pullOffValue,
        pullOffPlacementKind);
      
  fCurrentTechnicalWithStringsList.push_back (technicalWithString);
}

void xml2MsrTranslator::visitStart ( S_snap_pizzicato& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_snap_pizzicato" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  string
    placement =
      elt->getAttributeValue ("placement");

  msrTechnical::msrTechnicalPlacementKind
    snapPizzicatoPlacementKind =
      msrTechnical::k_NoPlacementKind;

  if      (placement == "above")
    snapPizzicatoPlacementKind = msrTechnical::kAbove;
    
  else if (placement == "below")
    snapPizzicatoPlacementKind = msrTechnical::kBelow;
    
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kSnapPizzicato,
        snapPizzicatoPlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void xml2MsrTranslator::visitStart ( S_stopped& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_stopped" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  string
    placement =
      elt->getAttributeValue ("placement");

  msrTechnical::msrTechnicalPlacementKind
    stoppedPlacementKind =
      msrTechnical::k_NoPlacementKind;

  if      (placement == "above")
    stoppedPlacementKind = msrTechnical::kAbove;
    
  else if (placement == "below")
    stoppedPlacementKind = msrTechnical::kBelow;
    
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kStopped,
        stoppedPlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void xml2MsrTranslator::visitStart ( S_string& elt )
{
/*
        <notations>
          <technical>
            <string>5</string>
            <fret>0</fret>
          </technical>
        </notations>
*/
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_string" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();

  string stringValue = elt->getValue();

  int stringIntegerValue = atoi (stringValue.c_str());

  if (! stringValue.size ()) {
    stringstream s;
    
    stringIntegerValue = 0;

    s <<
      "string value \"" << stringValue <<
      "\" is empoty, '" << stringIntegerValue << "' is assumed";
    
    msrMusicXMLWarning (
      inputLineNumber,
      s.str());    
  }
  
  string placement = elt->getAttributeValue ("placement");

  msrTechnicalWithInteger::msrTechnicalWithIntegerPlacementKind
    stringPlacementKind =
      msrTechnicalWithInteger::k_NoPlacementKind;

  if      (placement == "above")
    stringPlacementKind = msrTechnicalWithInteger::kAbove;
    
  else if (placement == "below")
    stringPlacementKind = msrTechnicalWithInteger::kBelow;
    
  else if (placement.size ()) {    
    stringstream s;
    
    s <<
      "placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  S_msrTechnicalWithInteger
    technicalWithInteger =
      msrTechnicalWithInteger::create (
        inputLineNumber,
        msrTechnicalWithInteger::kString,
        stringIntegerValue,
        stringPlacementKind);
      
  fCurrentTechnicalWithIntegersList.push_back (technicalWithInteger);
}

void xml2MsrTranslator::visitStart ( S_tap& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_tap" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  string
    placement =
      elt->getAttributeValue ("placement");

  msrTechnical::msrTechnicalPlacementKind
    tapPlacementKind =
      msrTechnical::k_NoPlacementKind;

  if      (placement == "above")
    tapPlacementKind = msrTechnical::kAbove;
    
  else if (placement == "below")
    tapPlacementKind = msrTechnical::kBelow;
    
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kTap,
        tapPlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void xml2MsrTranslator::visitStart ( S_thumb_position& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_thumb_position" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  string
    placement =
      elt->getAttributeValue ("placement");

  msrTechnical::msrTechnicalPlacementKind
    thumbPositionPlacementKind =
      msrTechnical::k_NoPlacementKind;

  if      (placement == "above")
    thumbPositionPlacementKind = msrTechnical::kAbove;
    
  else if (placement == "below")
    thumbPositionPlacementKind = msrTechnical::kBelow;
    
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kThumbPosition,
        thumbPositionPlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void xml2MsrTranslator::visitStart ( S_toe& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_toe" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  string
    placement =
      elt->getAttributeValue ("placement");

  msrTechnical::msrTechnicalPlacementKind
    toePlacementKind =
      msrTechnical::k_NoPlacementKind;

  if      (placement == "above")
    toePlacementKind = msrTechnical::kAbove;
    
  else if (placement == "below")
    toePlacementKind = msrTechnical::kBelow;
    
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kToe,
        toePlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void xml2MsrTranslator::visitStart ( S_triple_tongue& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_triple_tongue" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  string
    placement =
      elt->getAttributeValue ("placement");

  msrTechnical::msrTechnicalPlacementKind
    tripleTonguePlacementKind =
      msrTechnical::k_NoPlacementKind;

  if      (placement == "above")
    tripleTonguePlacementKind = msrTechnical::kAbove;
    
  else if (placement == "below")
    tripleTonguePlacementKind = msrTechnical::kBelow;
    
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kTripleTongue,
        tripleTonguePlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void xml2MsrTranslator::visitStart ( S_up_bow& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_up_bow" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  string
    placement =
      elt->getAttributeValue ("placement");

  msrTechnical::msrTechnicalPlacementKind
    upBowPlacementKind =
      msrTechnical::k_NoPlacementKind;

  if      (placement == "above")
    upBowPlacementKind = msrTechnical::kAbove;
    
  else if (placement == "below")
    upBowPlacementKind = msrTechnical::kBelow;
    
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kUpBow,
        upBowPlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_ornaments& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_ornaments" <<
      endl;
}

void xml2MsrTranslator::visitStart ( S_tremolo& elt )
{
  /*
    <notations>
      <ornaments>
        <tremolo type="single">3</tremolo>
      </ornaments>
    </notations>

The tremolo ornament can be used to indicate either single-note or double-note tremolos. Single-note tremolos use the single type, while double-note tremolos use the start and stop types. The default is "single" for compatibility with Version 1.1.

The text of the element indicates the number of tremolo marks and is an integer from 0 to 8.
Note that the number of attached beams is not included in this value, but is represented separately using the beam element.

When using double-note tremolos, the duration of each note in the tremolo should correspond to half of the notated type value.
A time-modification element should also be added with an actual-notes value of 2 and a normal-notes value of 1.
If used within a tuplet, this 2/1 ratio should be multiplied by the existing tuplet ratio.

Using repeater beams for indicating tremolos is deprecated as of MusicXML 3.0.

  */
   
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting tremolo" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();

  string tremoloMarksNumberString =
    elt->getValue ();
    
  int tremoloMarksNumber = (int)(*elt);

  if (! tremoloMarksNumberString.size ()) {
    stringstream s;

    tremoloMarksNumber = 1;
    
    s <<
      "--> tremolo value is missing, '" << tremoloMarksNumber << "' assumed";
    
    msrMusicXMLWarning (
      inputLineNumber,
      s.str());
  }

  if (tremoloMarksNumber < 0 || tremoloMarksNumber > 8) { // JMI what does 0 mean?
    stringstream s;
    
    s <<
      "--> tremolo value \"" << tremoloMarksNumber <<
      "\" should be between 0 and 8";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());
  }
  
  string tremoloType = elt->getAttributeValue ("type");

  fCurrentMusicXMLTremoloType = kSingleTremolo; // default value
    
  if      (tremoloType == "single")
    fCurrentMusicXMLTremoloType = kSingleTremolo;
    
  else if (tremoloType == "start")
    fCurrentMusicXMLTremoloType = kStartTremolo;
    
  else if (tremoloType == "stop")
    fCurrentMusicXMLTremoloType = kStopTremolo;
    
  else if (tremoloType.size ()) {
    stringstream s;
    
    s <<
      "--> tremolo type \"" << tremoloType <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());
  }
  
  string
    tremoloPlacement =
      elt->getAttributeValue ("placement");

  msrSingleTremolo::msrSingleTremoloPlacementKind
    singleTremoloPlacementKind =
      msrSingleTremolo::k_NoPlacementKind;
  msrDoubleTremolo::msrDoubleTremoloPlacementKind
    doubleTremoloPlacementKind =
      msrDoubleTremolo::k_NoPlacementKind;

  if      (tremoloPlacement == "above") {
    switch (fCurrentMusicXMLTremoloType) {
      case k_NoTremolo:
        // just to avoid a compiler message
        break;
        
      case kSingleTremolo:
        singleTremoloPlacementKind = msrSingleTremolo::kAbove;
        break;
        
      case kStartTremolo:
      case kStopTremolo:
        doubleTremoloPlacementKind = msrDoubleTremolo::kAbove;
        break;
    } // switch
  }
  
  else if (tremoloPlacement == "below") {
    switch (fCurrentMusicXMLTremoloType) {
      case k_NoTremolo:
        // just to avoid a compiler message
        break;
        
      case kSingleTremolo:
        singleTremoloPlacementKind = msrSingleTremolo::kBelow;
        break;
        
      case kStartTremolo:
      case kStopTremolo:
        doubleTremoloPlacementKind = msrDoubleTremolo::kBelow;
        break;
    } // switch
  }
    
  else if (tremoloPlacement.size ()) {
    
    stringstream s;
    
    s <<
      "tremolo placement \"" << tremoloPlacement <<
      "\" is unknown";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());    
  }

  switch (fCurrentMusicXMLTremoloType) {
    case k_NoTremolo:
      // just to avoid a compiler message
      break;
        
    case kSingleTremolo:
      if (gGeneralOptions->fTraceTremolos) {
        cerr << idtr <<
          "Creating a single tremolo" <<
          ", line " << inputLineNumber <<
          endl;
      }
      
      // create a single tremolo, it will be attached to current note
      // in attachCurrentSingleTremoloToNote()
      if (gGeneralOptions->fTraceTremolos) {
        cerr << idtr <<
          "Creating a single tremolo" <<
          ", line " << inputLineNumber <<
          singularOrPlural (
            tremoloMarksNumber, "mark", "marks") <<
          ", placement : " <<
          msrSingleTremolo::singleTremoloPlacementKindAsString (
            singleTremoloPlacementKind) <<
          endl;
      }

      fCurrentSingleTremolo =
        msrSingleTremolo::create (
          inputLineNumber,
          tremoloMarksNumber,
          singleTremoloPlacementKind);
      break;
      
    case kStartTremolo:
  //    if (! fCurrentDoubleTremolo) { JMI
      {
        // fetch current voice
        S_msrVoice
          currentVoice =
            createVoiceInStaffInCurrentPartIfNeeded (
              inputLineNumber,
              fCurrentNoteStaffNumber,
              fCurrentVoiceNumber);

        // create a double tremolo
        if (gGeneralOptions->fTraceTremolos) {
          cerr << idtr <<
            "Creating a double tremolo" <<
            ", line " << inputLineNumber <<
            ", " <<
            singularOrPlural (
              tremoloMarksNumber, "mark", "marks") <<
            ", placement : " <<
            msrDoubleTremolo::doubleTremoloPlacementKindAsString (
              doubleTremoloPlacementKind) <<
            endl;
        }
      
        fCurrentDoubleTremolo =
          msrDoubleTremolo::create (
            inputLineNumber,
            msrDoubleTremolo::kNotesDoubleTremolo,
            tremoloMarksNumber,
            doubleTremoloPlacementKind,
            currentVoice);
      }
      
/*
      else {
        stringstream s;

        s <<
          "<tremolo/> start when a current double tremolo is already open";

        msrMusicXMLError (
          elt->getInputLineNumber (),
          s.str());    
      }
*/
      break;

    case kStopTremolo:
      if (fCurrentDoubleTremolo) {
        if (gGeneralOptions->fTraceTremolos) {
          cerr << idtr <<
            "Meeting a double tremolo stop" <<
            ", line " << inputLineNumber <<
            endl;
        }

        // it will be handled in
        // handleStandaloneOrDoubleTremoloNoteOrGraceNoteOrRest()
      }
      
      else {
        stringstream s;

        s <<
          "<tremolo/> stop whit no preceeding <tremolo/> start";

        msrMusicXMLError (
          elt->getInputLineNumber (),
          s.str());    
      }
      break;
  } // switch
}

void xml2MsrTranslator::visitStart ( S_trill_mark& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_trill_mark" <<
      endl;

 // type : upright inverted  (Binchois20.xml)
  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kTrillMark);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void xml2MsrTranslator::visitStart ( S_wavy_line& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_wavy_line" <<
      endl;

  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kWavyLine);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void xml2MsrTranslator::visitStart ( S_turn& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_turn" <<
      endl;

  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kTurn);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void xml2MsrTranslator::visitStart ( S_inverted_turn& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_inverted_turn" <<
      endl;

  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kInvertedTurn);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void xml2MsrTranslator::visitStart ( S_delayed_turn& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_delayed_turn" <<
      endl;

  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kDelayedTurn);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void xml2MsrTranslator::visitStart ( S_delayed_inverted_turn& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_delayed_inverted_turn" <<
      endl;

  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kDelayedInvertedTurn);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void xml2MsrTranslator::visitStart ( S_vertical_turn& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_vertical_turn" <<
      endl;

  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kVerticalTurn);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void xml2MsrTranslator::visitStart ( S_mordent& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_mordent" <<
      endl;

  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kMordent);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void xml2MsrTranslator::visitStart ( S_inverted_mordent& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_inverted_mordent" <<
      endl;

  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kInvertedMordent);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void xml2MsrTranslator::visitStart ( S_schleifer& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_schleifer" <<
      endl;

  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kSchleifer);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void xml2MsrTranslator::visitStart ( S_shake& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_shake" <<
      endl;

  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kShake);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void xml2MsrTranslator::visitStart ( S_accidental_mark& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_accidental_mark" <<
      endl;

  string accidentalMark =
    elt->getValue ();

  msrOrnament::msrOrnamentAccidentalMarkKind
    currentOrnamentAccidentalMarkKind;
    
  if      (accidentalMark == "double-flat")
    currentOrnamentAccidentalMarkKind = msrOrnament::kDoubleFlat;
    
  else if (accidentalMark == "three-quarters-flat")
    currentOrnamentAccidentalMarkKind = msrOrnament::kSesquiFlat;
    
  else if (accidentalMark == "flat")
    currentOrnamentAccidentalMarkKind = msrOrnament::kFlat;
    
  else if (accidentalMark == "quarter-flat")
    currentOrnamentAccidentalMarkKind = msrOrnament::kSemiFlat;
    
  else if (accidentalMark == "natural")
    currentOrnamentAccidentalMarkKind = msrOrnament::kNatural;
    
  else if (accidentalMark == "quarter-sharp")
    currentOrnamentAccidentalMarkKind = msrOrnament::kSemiSharp;
    
  else if (accidentalMark == "sharp")
    currentOrnamentAccidentalMarkKind = msrOrnament::kSharp;
    
  else if (accidentalMark == "three-quarters-sharp")
    currentOrnamentAccidentalMarkKind = msrOrnament::kSesquiSharp;
    
  else if (accidentalMark == "double-sharp")
    currentOrnamentAccidentalMarkKind = msrOrnament::kDoubleSharp;
        
  else if (accidentalMark.size ()) {
    stringstream s;
    
    s <<
      "accidental mark \"" << accidentalMark <<
      "\" is unknown";
    
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());    
  }

  string placement =
    elt->getAttributeValue ("placement");

  fCurrentOrnamentPlacementKind =
    msrOrnament::k_NoPlacementKind;

  if      (placement == "above")
    fCurrentOrnamentPlacementKind = msrOrnament::kAbove;
    
  else if (placement == "below")
    fCurrentOrnamentPlacementKind = msrOrnament::kBelow;
    
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "ornament placement \"" << placement <<
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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_ornaments" <<
      endl;

  // JMI
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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_f" <<
      endl;

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kF);
        
  fPendingDynamics.push_back(dynamics);
}
void xml2MsrTranslator::visitStart( S_ff& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_ff" <<
      endl;

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kFF);
        
  fPendingDynamics.push_back(dynamics);
}
void xml2MsrTranslator::visitStart( S_fff& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_fff" <<
      endl;

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kFFF);
        
  fPendingDynamics.push_back(dynamics);
}
void xml2MsrTranslator::visitStart( S_ffff& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_ffff" <<
      endl;

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kFFFF);
        
  fPendingDynamics.push_back(dynamics);
}
void xml2MsrTranslator::visitStart( S_fffff& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_fffff" <<
      endl;

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kFFFFF);
        
  fPendingDynamics.push_back(dynamics);
}
void xml2MsrTranslator::visitStart( S_ffffff& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_ffffff" <<
      endl;

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kFFFFFF);
        
  fPendingDynamics.push_back(dynamics);
}

void xml2MsrTranslator::visitStart( S_p& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_p" <<
      endl;

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kP);
        
  fPendingDynamics.push_back(dynamics);
}
void xml2MsrTranslator::visitStart( S_pp& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_pp" <<
      endl;

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kPP);
        
  fPendingDynamics.push_back(dynamics);
}
void xml2MsrTranslator::visitStart( S_ppp& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_ppp" <<
      endl;

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kPPP);
        
  fPendingDynamics.push_back(dynamics);
}
void xml2MsrTranslator::visitStart( S_pppp& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_pppp" <<
      endl;

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kPPPP);
        
  fPendingDynamics.push_back(dynamics);
}
void xml2MsrTranslator::visitStart( S_ppppp& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_ppppp" <<
      endl;

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kPPPPP);
        
  fPendingDynamics.push_back(dynamics);
}
void xml2MsrTranslator::visitStart( S_pppppp& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_pppppp" <<
      endl;

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kPPPPPP);
        
  fPendingDynamics.push_back(dynamics);
}


void xml2MsrTranslator::visitStart( S_mf& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_mf" <<
      endl;

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kMF);
        
  fPendingDynamics.push_back(dynamics);
}
void xml2MsrTranslator::visitStart( S_mp& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_mp" <<
      endl;

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kMP);
        
  fPendingDynamics.push_back(dynamics);
}

void xml2MsrTranslator::visitStart( S_fp& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_fp" <<
      endl;

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kFP);
        
  fPendingDynamics.push_back(dynamics);
}
void xml2MsrTranslator::visitStart( S_fz& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_fz" <<
      endl;

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kFZ);
        
  fPendingDynamics.push_back(dynamics);
}

void xml2MsrTranslator::visitStart( S_rf& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_rf" <<
      endl;

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kRF);
        
  fPendingDynamics.push_back(dynamics);
}

void xml2MsrTranslator::visitStart( S_sf& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_sf" <<
      endl;

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kSF);
        
  fPendingDynamics.push_back(dynamics);
}

void xml2MsrTranslator::visitStart( S_rfz& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_rfz" <<
      endl;

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kRFZ);
        
  fPendingDynamics.push_back(dynamics);
}

void xml2MsrTranslator::visitStart( S_sfz& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_sfz" <<
      endl;

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kSFZ);
        
  fPendingDynamics.push_back(dynamics);
}

void xml2MsrTranslator::visitStart( S_sfp& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_sfp" <<
      endl;

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kSFP);
        
  fPendingDynamics.push_back(dynamics);
}

void xml2MsrTranslator::visitStart( S_sfpp& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_sfpp" <<
      endl;

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kSFPP);
        
  fPendingDynamics.push_back(dynamics);
}

void xml2MsrTranslator::visitStart( S_sffz& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_sffz" <<
      endl;

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        elt->getInputLineNumber (),
        msrDynamics::kSFFZ);
        
  fPendingDynamics.push_back(dynamics);
}

void xml2MsrTranslator::visitStart( S_other_dynamics& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_other_dynamics" <<
      endl;

  string otherDynamicsValue = elt->getValue ();
  
  S_msrOtherDynamics
    otherDynamics =
      msrOtherDynamics::create (
        elt->getInputLineNumber (),
        otherDynamicsValue);
        
  fPendingOtherDynamics.push_back(otherDynamics);
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_grace& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_grace" <<
      endl;

  fCurrentNoteIsAGraceNote = true;

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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_chord" <<
      endl;

  fCurrentNoteBelongsToAChord = true;

  // delay the handling until 'visitEnd ( S_note& elt)',
  // because we don't know yet the voice and staff numbers for sure
  // (they can be specified after <chord/> in the <note/>)
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_time_modification& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_time_modification" <<
      endl;

  // there may be no '<tuplet number="n" type="start" />'
  // in the tuplet notes after the first one,
  // so we detect tuplet notes on '<time-modification>' ??? JMI
  fCurrentNoteHasATimeModification = true;
}

void xml2MsrTranslator::visitStart ( S_actual_notes& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_actual_notes" <<
      endl;

  fCurrentActualNotes = (int)(*elt);

  // notes inside a tuplet have no <tuplet/> markup
  // and 2 actual notes indicate a double tremolo
  if (fCurrentActualNotes != 2)
    fCurrentNoteBelongsToATuplet = true;
}

void xml2MsrTranslator::visitStart ( S_normal_notes& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_normal_notes" <<
      endl;

  fCurrentNormalNotes = (int)(*elt);

  // notes inside a tuplet have no <tuplet/> markup
  // and 1 actual note indicates a double tremolo
  if (fCurrentNormalNotes != 1)
    fCurrentNoteBelongsToATuplet = true;
}

void xml2MsrTranslator::visitStart ( S_normal_type& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_normal_type" <<
      endl;

  fCurrentNormalNoteType = elt->getValue();
}

/*
   A tuplet element is present when a tuplet is to be displayed graphically, in addition to the sound data provided by the time-modification elements. The number attribute is used to distinguish nested tuplets. The bracket attribute is used to indicate the presence of a bracket. If unspecified, the results are implementation-dependent. The line-shape attribute is used to specify whether the bracket is straight or in the older curved or slurred style. It is straight by default.
Whereas a time-modification element shows how the cumulative, sounding effect of tuplets and double-note tremolos compare to the written note type, the tuplet element describes how this is displayed. The tuplet element also provides more detailed representation information than the time-modification element, and is needed to represent nested tuplets and other complex tuplets accurately. 
The show-number attribute is used to display either the number of actual notes, the number of both actual and normal notes, or neither. It is actual by default. The show-type attribute is used to display either the actual type, both the actual and normal types, or neither. It is none by default.

tuplet-actual  

The tuplet-actual element provides optional full control over how the actual part of the tuplet is displayed, including number and note type (with dots). If any of these elements are absent, their values are based on the time-modification element.

tuplet-dot  

The tuplet-dot type is used to specify dotted normal tuplet types.

tuplet-normal   The tuplet-normal element provides optional full control over how the normal part of the tuplet is displayed, including number and note type (with dots). If any of these elements are absent, their values are based on the time-modification element.
tuplet-number  

The tuplet-number type indicates the number of notes for this portion of the tuplet.

tuplet-type  

The tuplet-type type indicates the graphical note type of the notes for this portion of the tuplet.

From NestedTuplets.xml:

          <tuplet bracket="no" number="2" placement="below" type="start">
            <tuplet-actual>
              <tuplet-number>3</tuplet-number>
              <tuplet-type>16th</tuplet-type>
              <tuplet-dot/>
            </tuplet-actual>
            <tuplet-normal>
              <tuplet-number>2</tuplet-number>
              <tuplet-type>16th</tuplet-type>
              <tuplet-dot/>
            </tuplet-normal>
          </tuplet>

        <notations>
          <tuplet number="2" type="start">
            <tuplet-actual>
              <tuplet-number>3</tuplet-number>
              <tuplet-type>eighth</tuplet-type>
            </tuplet-actual>
            <tuplet-normal>
              <tuplet-number>2</tuplet-number>
              <tuplet-type>eighth</tuplet-type>
            </tuplet-normal>
          </tuplet>
        </notations>

*/

void xml2MsrTranslator::visitStart ( S_tuplet& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_tuplet" <<
      endl;

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

  fCurrentNoteBelongsToATuplet = true;
}

void xml2MsrTranslator::visitStart ( S_tuplet_number& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_tuplet_number" <<
      endl;

  // not handled JMI
  fCurrentTupletDisplayNumber = (int)(*elt);
}

void xml2MsrTranslator::visitStart ( S_tuplet_type& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_tuplet_type" <<
      endl;

/*
 Type indicates the graphic note type, Valid values (from shortest to longest) are 1024th, 512th, 256th, 128th, 64th, 32nd, 16th, eighth, quarter, half, whole, breve, long, and maxima. The size attribute indicates full, cue, or large size, with full the default for regular notes and cue the default for cue and grace notes.
*/

  // not handled JMI
  fCurrentTupletDisplayType = elt->getValue();
}


//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_rest& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_rest" <<
      endl;

  /*
        <note>
        <rest/>
        <duration>24</duration>
        <voice>1</voice>
      </note>
*/
  //  cerr << "--> xml2MsrTranslator::visitStart ( S_rest& elt ) " <<endl;
  fCurrentNoteIsARest = true;

  string restMeasure = elt->getAttributeValue ("measure");
  
  fCurrentRestMeasure = false;

  if (restMeasure == "yes") {
    fCurrentRestMeasure = true; // USE IT! JMI ???
  }

  else {
    if (restMeasure.size ())
      msrMusicXMLError (
        elt->getInputLineNumber (),
          "unknown rest measure \"" + restMeasure + "\"");
  }  
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_display_step& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_display_step" <<
      endl;

  string displayStep = elt->getValue();
  
  checkStep (
    elt->getInputLineNumber (),
    displayStep);

  fCurrentHarmonyRootDiatonicPitch =
    msrDiatonicPitchFromString (displayStep [0]);
}

void xml2MsrTranslator::visitStart ( S_display_octave& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_display_octave" <<
      endl;

  fDisplayOctave = (int)(*elt);
}

void xml2MsrTranslator::visitEnd ( S_unpitched& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_unpitched" <<
      endl;

/*
        <unpitched>
          <display-step>E</display-step>
          <display-octave>5</display-octave>
        </unpitched>
*/
  fCurrentNoteIsUnpitched = true;
  fCurrentNoteDiatonicPitch = // JMI
    fCurrentHarmonyRootDiatonicPitch;
  fCurrentNoteOctave = fDisplayOctave;
}

//______________________________________________________________________________
S_msrChord xml2MsrTranslator::createChordFromItsFirstNote (
  S_msrVoice voice,
  S_msrNote  chordFirstNote)
{
  int inputLineNumber =
    chordFirstNote->getInputLineNumber ();
    
  if (gGeneralOptions->fTraceChords || gGeneralOptions->fTraceNotes)
    cerr << idtr <<
      "--> creating a chord from its first note " <<
      chordFirstNote->noteAsShortString () <<
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
        fCurrentPart,
        chordFirstNote->getNoteSoundingDivisions (),
        chordFirstNote->getNoteDisplayedDivisions (),
        chordFirstNote->getNoteGraphicDuration ());

  // chord's tie kind is that of its first note
  chord->
    setChordTie (
      chordFirstNote->getNoteTie ());
  
  // register note as first member of chord
  if (gGeneralOptions->fTraceChords || gGeneralOptions->fTraceNotes)
    cerr << idtr <<
      "--> adding first note 2 " <<
      chordFirstNote->noteAsShortStringWithRawDivisions() <<
      ", line " << inputLineNumber <<
      ", to new chord" <<
      endl;
      
  chord->
    addFirstNoteToChord (chordFirstNote);

  // copy firstNote's elements if any to the chord
  copyNoteElementsToChord (chordFirstNote, chord);
    
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

    if (gGeneralOptions->fTraceChords || gGeneralOptions->fTraceNotes)
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
void xml2MsrTranslator::copyNoteTechnicalsToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's technicals if any from the first note to chord
  
  list<S_msrTechnical>
    noteTechnicals =
      note->
        getNoteTechnicals ();
                          
  list<S_msrTechnical>::const_iterator i;
  for (
    i=noteTechnicals.begin();
    i!=noteTechnicals.end();
    i++) {

    if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceTechnicals)
      cerr << idtr <<
        "--> copying technical '" <<
        (*i)->technicalKindAsString () <<
        "' from note " << note->noteAsString () <<
        " to chord" <<
        endl;

    chord->addTechnicalToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void xml2MsrTranslator::copyNoteTechnicalWithIntegersToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's technicals if any from the first note to chord
  
  list<S_msrTechnicalWithInteger>
    noteTechnicalWithIntegers =
      note->
        getNoteTechnicalWithIntegers ();
                          
  list<S_msrTechnicalWithInteger>::const_iterator i;
  for (
    i=noteTechnicalWithIntegers.begin();
    i!=noteTechnicalWithIntegers.end();
    i++) {

    if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceTechnicals)
      cerr << idtr <<
        "--> copying technical '" <<
        (*i)->technicalWithIntegerKindAsString () <<
        "' from note " << note->noteAsString () <<
        " to chord" <<
        endl;

    chord->addTechnicalWithIntegerToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void xml2MsrTranslator::copyNoteTechnicalWithStringsToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's technicals if any from the first note to chord
  
  list<S_msrTechnicalWithString>
    noteTechnicalWithStrings =
      note->
        getNoteTechnicalWithStrings ();
                          
  list<S_msrTechnicalWithString>::const_iterator i;
  for (
    i=noteTechnicalWithStrings.begin();
    i!=noteTechnicalWithStrings.end();
    i++) {

    if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceTechnicals)
      cerr << idtr <<
        "--> copying technical '" <<
        (*i)->technicalWithStringKindAsString () <<
        "' from note " << note->noteAsString () <<
        " to chord" <<
        endl;

    chord->addTechnicalWithStringToChord ((*i));
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

    if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceChords) // JMI
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
void xml2MsrTranslator::copyNoteSingleTremoloToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's singleTremolo if any from the first note to chord
  
  S_msrSingleTremolo
    noteSingleTremolo =
      note->
        getNoteSingleTremolo ();
                          
  if (gGeneralOptions->fTraceTremolos || gGeneralOptions->fTraceChords)
    cerr << idtr <<
      "--> copying singleTremolo '" <<
      noteSingleTremolo->singleTremoloAsString () <<
      "' from note " << note->noteAsString () <<
      " to chord" <<
      endl;

  chord->addSingleTremoloToChord (noteSingleTremolo);
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

    if (gGeneralOptions->fTraceChords || gGeneralOptions->fTraceDynamics)
      cerr << idtr <<
        "--> copying dynamics '" <<
        (*i)->dynamicsKindAsString () <<
        "' from note " << note->noteAsString () <<
        " to chord" <<
        endl;

    chord->addDynamicsToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void xml2MsrTranslator::copyNoteOtherDynamicsToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's other dynamics if any from the first note to chord
  
  list<S_msrOtherDynamics>
    noteOtherDynamics =
      note->
        getNoteOtherDynamics ();
                          
  list<S_msrOtherDynamics>::const_iterator i;
  for (
    i=noteOtherDynamics.begin();
    i!=noteOtherDynamics.end();
    i++) {

    if (gGeneralOptions->fTraceChords || gGeneralOptions->fTraceDynamics)
      cerr << idtr <<
        "--> copying other dynamics '" <<
        (*i)->otherDynamicsAsString () <<
        "' from note " << note->noteAsString () <<
        " to chord" <<
        endl;

    chord->addOtherDynamicsToChord ((*i));
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

    if (gGeneralOptions->fTraceChords || gGeneralOptions->fTraceWords)
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

    if (gGeneralOptions->fTraceChords || gGeneralOptions->fTraceSlurs)
      cerr << idtr <<
        "--> copying slur '" <<
        (*i)->slurKindAsString () <<
        "' from note " << note->noteAsString () <<
        " to chord" <<
        endl;

    chord->addSlurToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void xml2MsrTranslator::copyNoteLigaturesToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's ligatures if any from the first note to chord
  
  list<S_msrLigature>
    noteLigatures =
      note->
        getNoteLigatures ();
                          
  list<S_msrLigature>::const_iterator i;
  for (
    i=noteLigatures.begin();
    i!=noteLigatures.end();
    i++) {

    if (gGeneralOptions->fTraceChords || gGeneralOptions->fTraceLigatures)
      cerr << idtr <<
        "--> copying ligature '" <<
        (*i)->ligatureKindAsString () <<
        "' from note " << note->noteAsString () <<
        " to chord" <<
        endl;

    chord->addLigatureToChord ((*i));
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

    if (gGeneralOptions->fTraceChords || gGeneralOptions->fTraceWedges)
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
void xml2MsrTranslator::copyNoteHarmonyToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's harmony if any from the first note to chord
  
  S_msrHarmony
    harmony =
      note->
        getNoteHarmony ();
                          
  if (harmony) {
    if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceChords)
      cerr << idtr <<
        "--> copying harmony '" <<
        harmony->harmonyAsString () <<
        "' from note " << note->noteAsString () <<
        " to chord '" << chord->chordAsString () <<
        "'" <<
        endl;

    chord->setChordHarmony (harmony);
  }   
}

//______________________________________________________________________________
void xml2MsrTranslator::copyNoteElementsToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's articulations if any to the chord
  copyNoteArticulationsToChord (note, chord);

  // copy note's technicals if any to the chord
  copyNoteTechnicalsToChord (note, chord);
  copyNoteTechnicalWithIntegersToChord (note, chord);
  copyNoteTechnicalWithStringsToChord (note, chord);

  // copy note's ornaments if any to the chord
  copyNoteOrnamentsToChord (note, chord);

  // copy note's dynamics if any to the chord
  copyNoteDynamicsToChord (note, chord);

  // copy note's other dynamics if any to the chord
  copyNoteOtherDynamicsToChord (note, chord);

  // copy note's words if any to the chord
  copyNoteWordsToChord (note, chord);

  // copy note's slurs if any to the chord
  copyNoteSlursToChord (note, chord);

  // copy note's ligatures if any to the chord
  copyNoteLigaturesToChord (note, chord);

  // copy note's wedges if any to the chord
  copyNoteWedgesToChord (note, chord);

  // copy note's harmony if any to the chord
  copyNoteHarmonyToChord (note, chord);
}

//______________________________________________________________________________
void xml2MsrTranslator::createTupletWithItsFirstNote (S_msrNote firstNote)
{
  // firstNote is the first tuplet note,
  // and is currently at the end of the voice

  // create a tuplet
  if (gGeneralOptions->fTraceTuplets)
    cerr << idtr <<
      "Creating a '" <<
      fCurrentActualNotes <<
      "/" <<
      fCurrentNormalNotes <<
      "' tuplet with first note " <<
      firstNote->noteAsShortStringWithRawDivisions () <<
      endl;
      
  S_msrTuplet
    tuplet =
      msrTuplet::create (
        firstNote->getInputLineNumber (),
        fCurrentTupletNumber,
        fCurrentActualNotes,
        fCurrentNormalNotes,
        firstNote->getNotePositionInMeasure ());

  // add note as first note of the stack top tuplet
  tuplet->addNoteToTuplet (firstNote);

  if (gGeneralOptions->fTraceTuplets)
    // only after addNoteToTuplet() has set the note's tuplet uplink
    cerr << idtr <<
      "Adding first note " << firstNote->noteAsShortStringWithRawDivisions () <<
      " to tuplet '" <<
      tuplet->tupletAsShortString () <<
       "'" <<
      endl;
      
  // register tuplet in this visitor
  if (gGeneralOptions->fTraceTuplets || gGeneralOptions->fTraceNotes)
    cerr << idtr <<
      "++> pushing tuplet '" <<
      tuplet->tupletAsShortString () <<
      "' to tuplets stack" <<
      endl;
      
  fTupletsStack.push (tuplet);

/* JMI
  // set note displayed divisions
  firstNote->
    applyTupletMemberDisplayFactor (
      fCurrentActualNotes, fCurrentNormalNotes);
  */


  // keep track of current tuplet in the current voice,
  // in case we learn later by <chord/> in the next note
  // that it is actually the first note of a chord ?? JMI XXL

  /* JMI
  if (gGeneralOptions->fTraceTuplets) {
    displayLastHandledTupletInVoice (
      "############## Before fLastHandledTupletInVoice");
  }
  
// JMI  fLastHandledTupletInVoice [currentVoice] = tuplet;
  
  if (gGeneralOptions->fTraceTuplets) {
    displayLastHandledTupletInVoice (
      "############## After  fLastHandledNoteInVoice");
  }
  */
}

//______________________________________________________________________________
void xml2MsrTranslator::finalizeTuplet (
  int inputLineNumber)
{
  if (gGeneralOptions->fTraceTuplets)
    cerr << idtr <<
      "xml2MsrTranslator::finalizeTuplet(), " <<
      "line " << inputLineNumber <<
      endl;

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentNoteStaffNumber,
        fCurrentVoiceNumber);

  // get tuplet from top of tuplet stack
  S_msrTuplet
    tuplet =
      fTupletsStack.top ();

/*  // set note displayed divisions JMI
  note->
    applyTupletMemberDisplayFactor (
      fCurrentActualNotes, fCurrentNormalNotes);
*/

/* JMI
  // add lastNote to the tuplet
  if (gGeneralOptions->fTraceTuplets)
    cerr << idtr <<
      "==> adding last note " << lastNote->noteAsString () <<
      " to tuplets stack top " <<
      fTupletsStack.top ()->tupletAsShortString () <<
      endl;
  tuplet->addNoteToTuplet (lastNote);
*/

  // pop from the tuplets stack
  if (gGeneralOptions->fTraceTuplets)
    cerr << idtr <<
      "--> popping tuplet 2 '" <<
      tuplet->tupletAsShortString () <<
      "' from tuplets stack" <<
      endl;
      
  fTupletsStack.pop ();        

  if (fTupletsStack.size ()) {
    // tuplet is a nested tuplet
    if (gGeneralOptions->fTraceTuplets)
      cerr << idtr <<
        "=== adding nested tuplet '" <<
      tuplet->tupletAsShortString () <<
        "' to current stack top tuplet '" <<
      fTupletsStack.top ()->tupletAsShortString () <<
      "'" <<
      endl;
    
    fTupletsStack.top ()->
      addTupletToTuplet (tuplet);
  }
  
  else {
    // tuplet is a top level tuplet
    if (gGeneralOptions->fTraceTuplets)
      cerr << idtr <<
        "=== adding top level tuplet 2 '" <<
      tuplet->tupletAsString () <<
      "' to voice \"" <<
      currentVoice->getVoiceName () <<
      "\"" <<
      endl;
      
    currentVoice->
      appendTupletToVoice (tuplet);

    // the tuplet stop is not to be handled later
    fCurrentATupletStopIsPending = false;
  }  
}          

//______________________________________________________________________________
void xml2MsrTranslator::attachCurrentArticulationsToNote (
  S_msrNote note)
{
  // attach the current articulations if any to the note
  if (! fCurrentArticulations.empty()) {

    if (gGeneralOptions->fTraceNotes)
      cerr << idtr <<
        "--> attaching current articulations to note " <<
        note->noteAsString () <<
        endl;

    while (! fCurrentArticulations.empty()) {
      S_msrArticulation
        art =
          fCurrentArticulations.front();
          
      if (gGeneralOptions->fTraceNotes)
        cerr << idtr <<
          "--> attaching articulation '" <<
          art->articulationKindAsString () <<
          "' to note " << note->noteAsString () <<
          endl;
  
      note->
        addArticulationToNote (art);

      // forget about this articulation
      fCurrentArticulations.pop_front();
    } // while
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::attachCurrentTechnicalsToNote (
  S_msrNote note)
{
  // attach the current technicals if any to the note
  if (! fCurrentTechnicalsList.empty()) {
    
    if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceTechnicals)
      cerr << idtr <<
        "--> attaching current technicals to note " <<
        note->noteAsString () <<
        endl;

    while (! fCurrentTechnicalsList.empty()) {
      S_msrTechnical
        tech =
          fCurrentTechnicalsList.front();
          
      if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceTechnicals)
        cerr << idtr <<
          "--> attaching technical '" <<
          tech->technicalAsString () <<
          "' to note " << note->noteAsString () <<
          endl;
  
      note->
        addTechnicalToNote (tech);

      // forget about this technical
      fCurrentTechnicalsList.pop_front();
    } // while
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::attachCurrentTechnicalWithIntegersToNote (
  S_msrNote note)
{
  // attach the current technicals if any to the note
  if (! fCurrentTechnicalWithIntegersList.empty()) {
    
    if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceTechnicals)
      cerr << idtr <<
        "--> attaching current technical with integers to note " <<
        note->noteAsString () <<
        endl;

    while (! fCurrentTechnicalWithIntegersList.empty()) {
      S_msrTechnicalWithInteger
        tech =
          fCurrentTechnicalWithIntegersList.front();
          
      if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceTechnicals)
        cerr << idtr <<
          "--> attaching technical with integer '" <<
          tech->technicalWithIntegerAsString () <<
          "' to note " << note->noteAsString () <<
          endl;
  
      note->
        addTechnicalWithIntegerToNote (tech);

      // forget about this technical
      fCurrentTechnicalWithIntegersList.pop_front();
    } // while
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::attachCurrentTechnicalWithStringsToNote (
  S_msrNote note)
{
  // attach the current technicals if any to the note
  if (! fCurrentTechnicalWithStringsList.empty()) {
    
    if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceTechnicals)
      cerr << idtr <<
        "--> attaching current technical with strings to note " <<
        note->noteAsString () <<
        endl;

    while (! fCurrentTechnicalWithStringsList.empty()) {
      S_msrTechnicalWithString
        tech =
          fCurrentTechnicalWithStringsList.front();
          
      if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceTechnicals)
        cerr << idtr <<
          "--> attaching technical with string '" <<
          tech->technicalWithStringAsString () <<
          "' to note " << note->noteAsString () <<
          endl;
  
      note->
        addTechnicalWithStringToNote (tech);

      // forget about this technical
      fCurrentTechnicalWithStringsList.pop_front();
    } // while
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::attachCurrentOrnamentsToNote (
  S_msrNote note)
{
  // attach the current ornaments if any to the note
  if (! fCurrentOrnamentsList.empty()) {
    
    if (gGeneralOptions->fTraceNotes)
      cerr << idtr <<
        "--> attaching current ornaments to note " <<
        note->noteAsString () <<
        endl;

    while (! fCurrentOrnamentsList.empty()) {
      S_msrOrnament
        orn =
          fCurrentOrnamentsList.front();
          
      if (gGeneralOptions->fTraceNotes)
        cerr << idtr <<
          "--> attaching ornament '" <<
          orn->ornamentKindAsString () <<
          "' to note " << note->noteAsString () <<
          endl;
  
      note->
        addOrnamentToNote (orn);

      // forget about this ornament
      fCurrentOrnamentsList.pop_front();
    } // while
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::attachCurrentSingleTremoloToNote (
  S_msrNote note)
{
  // attach the current singleTremolo if any to the note
  if (fCurrentSingleTremolo) {
    
    if (gGeneralOptions->fTraceNotes)
      cerr << idtr <<
        "--> attaching current singleTremolo to note " <<
        note->noteAsString () <<
        endl;

    note->
      addSingleTremoloToNote (fCurrentSingleTremolo);
      
    fCurrentSingleTremolo = 0;
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::attachCurrentArticulationsToChord ( // JMI
  S_msrChord chord)
{
  if (! fCurrentArticulations.empty()) {

    if (gGeneralOptions->fTraceChords)
      cerr << idtr <<
        "--> attaching current articulations to chord " <<
        chord->chordAsString () <<
        endl;

    list<S_msrArticulation>::const_iterator i;
    for (
      i=fCurrentArticulations.begin();
      i!=fCurrentArticulations.end();
      i++) {
      if (gGeneralOptions->fTraceChords)
        cerr << idtr <<
          "--> attaching articulation " <<  (*i) <<
          " to chord " << chord <<
          endl;
          
      chord->
        addArticulationToChord ((*i));
      } // for
  }
}

/*
//______________________________________________________________________________
void xml2MsrTranslator::attachCurrentOrnamentsToChord ( // JMI
  S_msrChord chord)
{
  if (! fCurrentOrnamentsList.empty()) {

    if (gGeneralOptions->fTraceChords)
      cerr << idtr <<
        "--> attaching current ornaments to chord " <<
        chord->chordAsString () <<
        endl;

    list<S_msrOrnament>::const_iterator i;
    for (
      i=fCurrentOrnamentsList.begin();
      i!=fCurrentOrnamentsList.end();
      i++) {
      if (gGeneralOptions->fTraceChords)
        cerr << idtr <<
          "--> attaching ornament " <<  (*i) << " to chord " <<
          chord <<
          endl;
          
      chord->
        addOrnamentToChord ((*i));
      } // for
  }
}
*/

//______________________________________________________________________________
void xml2MsrTranslator::attachPendingDynamicsToNote (
  S_msrNote note)
{
 // attach the pending dynamics if any to the note
  if (! fPendingDynamics.empty()) {
    bool delayAttachment = false;
    
    
    if (gGeneralOptions->fTraceDynamics)
      cerr << idtr <<
        "--> attaching pending dynamics to note " <<
        note->noteAsString () <<
        endl;

    if (fCurrentNoteIsARest) {
      if (gMsrOptions->fDelayRestsDynamics) {
        cerr << idtr <<
          "--> Delaying dynamics attached to a rest until next note" <<
      endl;

        delayAttachment = true;
      }
      else {
        stringstream s;

        s <<
          singularOrPlural (
            fPendingDynamics.size (), "there is", "there are") <<
          " dynamics attached to a rest";
          
        msrMusicXMLWarning (
          note->getInputLineNumber (),
          s.str());
      }
    }
    
    if (! delayAttachment) {
      while (! fPendingDynamics.empty ()) {
        S_msrDynamics
          dynamics =
            fPendingDynamics.front ();
            
        note->addDynamicsToNote (dynamics);
        fPendingDynamics.pop_front ();
      } // while
    }
  }
}

//______________________________________________________________________________
void xml2MsrTranslator::attachPendingOtherDynamicsToNote (
  S_msrNote note)
{
 // attach the pending dynamics if any to the note
  if (! fPendingOtherDynamics.empty()) {
    bool delayAttachment = false;
    
    
    if (gGeneralOptions->fTraceDynamics)
      cerr << idtr <<
        "--> attaching pending dynamics to note " <<
        note->noteAsString () <<
        endl;

    if (fCurrentNoteIsARest) {
      if (gMsrOptions->fDelayRestsDynamics) {
        cerr << idtr <<
          "--> Delaying dynamics attached to a rest until next note" <<
      endl;

        delayAttachment = true;
      }
      else {
        stringstream s;

        s <<
          singularOrPlural (
            fPendingOtherDynamics.size (), "there is", "there are") <<
          " other dynamics attached to a rest";
          
        msrMusicXMLWarning (
          note->getInputLineNumber (),
          s.str());
      }
    }
    
    if (! delayAttachment) {
      while (! fPendingOtherDynamics.empty ()) {
        S_msrOtherDynamics
          otherDynamics =
            fPendingOtherDynamics.front ();
            
        note->addOtherDynamicsToNote (otherDynamics);
        fPendingOtherDynamics.pop_front ();
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
    bool delayAttachment = false;
    
    if (gGeneralOptions->fTraceWords)
      cerr << idtr <<
        "--> attaching pending words to note " <<
        note->noteAsString () <<
        endl;

    if (fCurrentNoteIsARest) {
      if (gMsrOptions->fDelayRestsWords) {
        cerr << idtr <<
          "--> Delaying word(s) attached to a rest until next note" <<
          endl;

        delayAttachment = true;
      }
      else {
        /* JMI
        for (
            list<S_msrWords>::const_iterator i = fPendingWords.begin();
            i != fPendingWords.end();
            i++) {
          msrMusicXMLWarning (
            (*i)->getInputLineNumber (),
            "there is words attached to a rest");
        } // for
        */
        stringstream s;

        s <<
          singularOrPlural (
            fPendingWords.size (), "there is", "there are") <<
          " word(s) attached to a rest";
          
        msrMusicXMLWarning (
          note->getInputLineNumber (),
          s.str());
      }
    }

    if (! delayAttachment) {
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
    bool delayAttachment = false;
        
    if (gGeneralOptions->fTraceSlurs)
      cerr << idtr <<
        "--> attaching pending slurs to note " <<
        note->noteAsString () <<
        endl;

    if (fCurrentNoteIsARest) {
      if (gMsrOptions->fDelayRestsSlurs) {
        cerr << idtr <<
          "--> Delaying slur attached to a rest until next note" <<
      endl;

        delayAttachment = true;
      }
      else {
        stringstream s;

        s <<
          singularOrPlural (
            fPendingSlurs.size (), "there is", "there are") <<
          " slur(s) attached to a rest";
          
        msrMusicXMLWarning (
          note->getInputLineNumber (),
          s.str());
      }
    }
    
    if (! delayAttachment) {
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
void xml2MsrTranslator::attachPendingLigaturesToNote (
  S_msrNote note)
{
  // attach the pending ligatures if any to the note
  if (! fPendingLigatures.empty ()) {
    bool delayAttachment = false;
        
    if (gGeneralOptions->fTraceLigatures)
      cerr << idtr <<
        "--> attaching pending ligatures to note " <<
        note->noteAsString () <<
        endl;

    if (fCurrentNoteIsARest) {
      if (gMsrOptions->fDelayRestsLigatures) {
        cerr << idtr <<
          "--> Delaying ligature attached to a rest until next note" <<
      endl;

        delayAttachment = true;
      }
      else {
        stringstream s;

        s <<
          singularOrPlural (
            fPendingLigatures.size (), "there is", "there are") <<
          " ligatures(s) attached to a rest";
          
        msrMusicXMLWarning (
          note->getInputLineNumber (),
          s.str());
      }
    }
    
    if (! delayAttachment) {
      while (! fPendingLigatures.empty ()) {
        S_msrLigature
          ligature =
            fPendingLigatures.front ();
            
        note->addLigatureToNote (ligature);
        fPendingLigatures.pop_front ();
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
    bool delayAttachment = false;
        
    if (gGeneralOptions->fTraceWedges)
      cerr << idtr <<
        "--> attaching pending wedges to note " <<
        note->noteAsString () <<
        endl;

    if (fCurrentNoteIsARest) {
      if (gMsrOptions->fDelayRestsWedges) {
        cerr << idtr <<
          "--> Delaying wedge attached to a rest until next note" <<
      endl;

        delayAttachment = true;
      }
      else {
        stringstream s;

        s <<
          singularOrPlural (
            fPendingWedges.size (), "there is", "there are") <<
          " wedge(s) attached to a rest";
          
        msrMusicXMLWarning (
          note->getInputLineNumber (),
          s.str());
      }
    }
    
    if (! delayAttachment) {
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

  // attach the pending other dynamics, if any, to the note
  attachPendingOtherDynamicsToNote (note);
  
  // attach the pending words, if any, to the note
  attachPendingWordsToNote (note);

  // attach the pending slurs, if any, to the note
  attachPendingSlursToNote (note);

  // attach the pending libatures, if any, to the note
  attachPendingLigaturesToNote (note);

  // attach the pending wedges, if any, to the note
  attachPendingWedgesToNote (note);
}

//______________________________________________________________________________
void xml2MsrTranslator::visitEnd ( S_note& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_note" <<
      endl;

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

  // register note pitch
  fCurrentNoteQuatertonesPitch =
    quatertonesPitchFromDiatonicPitchAndAlteration (
      inputLineNumber,
      fCurrentNoteDiatonicPitch,
      fCurrentNoteAlteration);

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentNoteStaffNumber,
        fCurrentNoteVoiceNumber);

  if (false && gGeneralOptions->fTraceNotes) { // JMI
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

  // store voice and staff numbers in MusicXML note data
  fCurrentNoteStaffNumber = fCurrentStaffNumber;
  fCurrentNoteVoiceNumber = fCurrentVoiceNumber;

  // set current voices' 'notes divisions per quarter note
  if (gGeneralOptions->fTraceNotes)
    cerr << idtr <<
      "fCurrentNoteSoundingDivisions = " << 
      fCurrentNoteSoundingDivisions << ", " << 
      "fCurrentDivisionsPerQuarterNote = " <<
      fCurrentDivisionsPerQuarterNote << endl;

  // set current note displayed divisions right now,
  // before we create the note

  if (fCurrentNoteIsAGraceNote) {
    // set current grace note divisions      
    fCurrentNoteSoundingDivisions =
      fCurrentPart->
        durationAsDivisions (
          inputLineNumber,
          fCurrentNoteGraphicDuration);
  
    // set current grace note displayed divisions to note divisions JMI   ???
    fCurrentNoteDisplayedDivisions =
      fCurrentNoteSoundingDivisions;
  }
  
  else if (
    fCurrentMusicXMLTremoloType == kStartTremolo
     ||
     fCurrentMusicXMLTremoloType == kStopTremolo) {
    // double tremolo note
    if (fCurrentNoteGraphicDuration == k_NoDuration) {
      stringstream s;

      s <<
        "double tremolo note lacks a <type/>"; // JMI a completer

      msrMusicXMLError (
        inputLineNumber,
        s.str());
    }

    // set current double tremolo note displayed divisions
    fCurrentNoteDisplayedDivisions =
      fCurrentPart->
        durationAsDivisions (
          inputLineNumber,
          fCurrentNoteGraphicDuration);
  }

  else {
    // standalone note or rest
    fCurrentNoteDisplayedDivisions =
      fCurrentNoteSoundingDivisions;
  }

  // create the (new) note
  S_msrNote
    newNote =
      msrNote::create (
        inputLineNumber,
        fCurrentPart,
        
        msrNote::k_NoNoteKind, // will be set by 'setNodeKind()' later
        
        fCurrentNoteQuatertonesPitch,
        fCurrentNoteSoundingDivisions,
        fCurrentNoteDisplayedDivisions,
        fCurrentNoteDotsNumber,
        fCurrentNoteGraphicDuration,
        
        fCurrentNoteOctave,
        
        fCurrentNoteIsARest,
        fCurrentNoteIsUnpitched,
        
        fCurrentNoteIsAGraceNote);

  // handling the current pending harmony if any,
  // so that it gets attached to the note right now
  if (fPendingHarmony) {
    if (gGeneralOptions->fTraceHarmonies) {
      cerr << idtr <<
        "--> harmony" <<
        ", line " << inputLineNumber << ":" <<
        endl;
  
      idtr++;

      const int fieldWidth = 31;
      
      cerr <<
        idtr <<
          setw(fieldWidth) << "fCurrentPart" << " = " <<
          fCurrentPart->getPartCombinedName () <<
          endl <<
        idtr <<
          setw(fieldWidth) << "fCurrentHarmonyRootDiatonicPitch" << " = " <<
          msrDiatonicPitchAsString (
            gMsrOptions->fMsrQuatertonesPitchesLanguage,
            fCurrentHarmonyRootDiatonicPitch) <<
          endl <<
        idtr <<
          setw(fieldWidth) << "fCurrentHarmonyRootAlteration" << " = " <<
          msrAlterationAsString(
            fCurrentHarmonyRootAlteration) <<
          endl <<
        idtr <<
          setw(fieldWidth) << "fCurrentHarmonyKind" << " = " <<
          msrHarmony::harmonyKindAsString (
            fCurrentHarmonyKind) <<
          endl <<
        idtr <<
          setw(fieldWidth) << "fCurrentHarmonyKindText" << " = " <<
          fCurrentHarmonyKindText <<
          endl <<
        idtr <<
          setw(fieldWidth) << "fCurrentHarmonyBassDiatonicPitch" << " = " <<
          msrDiatonicPitchAsString (
            gMsrOptions->fMsrQuatertonesPitchesLanguage,
            fCurrentHarmonyBassDiatonicPitch) <<
          endl <<
        idtr <<
          setw(fieldWidth) << "fCurrentHarmonyBassAlteration" << " = " <<
          msrAlterationAsString(
            fCurrentHarmonyBassAlteration) <<
          endl;
          
      idtr--;
    }
  
    S_msrHarmony
      harmony =
        msrHarmony::create (
          fCurrentHarmonyInputLineNumber,
          fCurrentPart,
          fCurrentHarmonyRootQuartertonesPitch,
          fCurrentHarmonyKind,
          fCurrentHarmonyKindText,
          fCurrentHarmonyBassQuartertonesPitch,
          fCurrentNoteSoundingDivisions);
  
    // attach the current harmony to the note
    newNote->
      setNoteHarmony (harmony);

    // append the current harmony to the part
    fCurrentPart->
      appendHarmonyToPart (
        currentVoice,
        harmony);

    fPendingHarmony = false;
  }

  // handle note
  if (fCurrentNoteBelongsToAChord && fCurrentNoteBelongsToATuplet) {
    
    // note is the second, third, ..., member of a chord
    // that is a member of a tuplet
    handleNoteBelongingToAChordInATuplet (newNote);
  }
  
  else if (fCurrentNoteBelongsToAChord) {
    
    // note is the second, third, ..., member of a chord
    // whose first member is 'fLastHandledNoteInVoice [currentVoice]'
    handleNoteBelongingToAChord (newNote);
  }
  
  else if (fCurrentNoteBelongsToATuplet) {
    
    // note/rest is the first, second, third, ..., member of a tuplet
    handleNoteBelongingToATuplet (newNote);
  }
  
  else {
    
    // note/rest is standalone or a member of grace notes
    handleStandaloneOrDoubleTremoloNoteOrGraceNoteOrRest (newNote);
  }

  // set its tie if any
  if (fCurrentTie) {
    newNote->
      setNoteTie (fCurrentTie);
  }
  
  // set its stem if any
  if (fCurrentStem)
    newNote->
      setNoteStem (fCurrentStem);

  // add its beams if any
  if (fCurrentBeams.size ()) {
    for (
      vector<S_msrBeam>::const_iterator i=fCurrentBeams.begin();
      i!=fCurrentBeams.end();
      i++) {
      newNote->
        addBeamToNote ((*i));
    } // for

    fCurrentBeams.clear ();
  }

  // attach the articulations if any to the note
  attachCurrentArticulationsToNote (newNote);

  // attach the technicals if any to the note
  attachCurrentTechnicalsToNote (newNote);
  attachCurrentTechnicalWithIntegersToNote (newNote);
  attachCurrentTechnicalWithStringsToNote (newNote);

  // attach the ornaments if any to the note
  attachCurrentOrnamentsToNote (newNote);

  // attach the singleTremolo if any to the note
  attachCurrentSingleTremoloToNote (newNote);

  if (false && gGeneralOptions->fTraceNotes) { // JMI
    cerr <<
      endl <<
      idtr <<
        "==> AFTER visitEnd (S_note&) " <<
        newNote->noteAsString () <<
        ", line " << inputLineNumber <<
        " we have:" <<
        endl <<
        
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
        endl <<
        "<==" <<
        endl <<
      endl;
  }

  if (! fCurrentNoteBelongsToAChord) {
    if (fOnGoingChord) {
      // newNote is the first note after the chord

  // JMI    msrAssert (fCurrentChord != 0, "fCurrentChord is null");
      
      // forget about this chord
      if (gGeneralOptions->fTraceChords) {
        cerr << idtr <<
          "Forgetting about chord '" <<
          fCurrentChord->chordAsString () <<
          "'" <<
          endl;
      }
      
      fCurrentChord = 0;

      if (fCurrentDoubleTremolo) {
        // forget about a double tremolo containing a chord
      // JMI XXL BOFS  fCurrentDoubleTremolo = 0;
      }

      fOnGoingChord = false;
    }
  }

  // keep track of current note in the current voice,
  // in case we learn later by <chord/> in the next note
  // that it is actually the first note of a chord
  /* JMI
  if (gGeneralOptions->fTraceNotes) {
    displayLastHandledNoteInVoice (
      "############## Before fLastHandledNoteInVoice");
  }
  */
  
  fLastHandledNoteInVoice [currentVoice] = newNote;

  /* JMI
  if (gGeneralOptions->fTraceNotes) {
    displayLastHandledNoteInVoice (
      "############## After  fLastHandledNoteInVoice");
  }
  */
  
  fOnGoingNote = false;
}

//______________________________________________________________________________
void xml2MsrTranslator::handleStandaloneOrDoubleTremoloNoteOrGraceNoteOrRest (
  S_msrNote newNote)
{
  int inputLineNumber =
    newNote->getInputLineNumber ();
    
  // register note/rest kind right now, to have a nice trace below
  if (fCurrentNoteIsAGraceNote) {
    // gracenote
    newNote->
      setNoteKind (msrNote::kGraceNote);
  }
  
  else if (
    fCurrentMusicXMLTremoloType == kStartTremolo
      ||
    fCurrentMusicXMLTremoloType == kStopTremolo) {
    // double tremolo note
    newNote->
      setNoteKind (msrNote::kDoubleTremoloMemberNote);
  }

  else {
    // standalone note or rest
    if (fCurrentNoteIsARest)
      newNote->
        setNoteKind (msrNote::kRestNote);
    else
      newNote->
        setNoteKind (msrNote::kStandaloneNote);
  }

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentNoteStaffNumber,
        fCurrentVoiceNumber);
    
  if (gGeneralOptions->fTraceNotes) {    
    idtr++; // JMI

    cerr <<
      idtr <<
        "--> handleStandaloneOrDoubleTremoloNoteOrGraceNoteOrRest: " <<
        "newNote = " << newNote->noteAsString () <<
        endl;

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
        setw(31) << "--> fCurrentNoteIsAGraceNote" << " = " <<
        booleanAsString (fCurrentNoteIsAGraceNote) <<
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

  // handle the pending tuplets if any ??? JMI XXL
  handleTupletsPendingOnTupletStack (
    inputLineNumber);

  if (fCurrentNoteIsAGraceNote) {
    if (! fCurrentGracenotes) {
      // this is the first grace note in grace notes

      if (gGeneralOptions->fTraceTuplets || gGeneralOptions->fTraceGracenotes) {
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
          fCurrentPart,
          fCurrentGraceIsSlashed,
          currentVoice);

      // register that last handled note if any is followed by grace notes
      S_msrNote
        lastHandledNoteInVoice =
          fLastHandledNoteInVoice [currentVoice];
          
      if (lastHandledNoteInVoice)
        lastHandledNoteInVoice->
          setNoteIsFollowedByGracenotes ();
      
      // append the grace notes to the current voice
      currentVoice->
        appendGracenotesToVoice (
          fCurrentGracenotes);
    }

    // append newNote to the current grace notes
    if (gGeneralOptions->fTraceTuplets || gGeneralOptions->fTraceGracenotes) {
      cerr <<  idtr <<
        "Appending note " <<
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

  else if (fCurrentMusicXMLTremoloType != k_NoTremolo) {
    // newNote belongs to a tremolo

    switch (fCurrentMusicXMLTremoloType) {
      case k_NoTremolo:
        // just to avoid a compiler message
        break;
        
      case kSingleTremolo:
        // append newNote to the current voice
        if (gGeneralOptions->fTraceNotes) {
          cerr <<  idtr <<
            "--> adding standalone " <<
            newNote->noteAsString () <<
            ", line " << newNote->getInputLineNumber () <<
            ", to voice \"" <<
            currentVoice->getVoiceName () <<
            "\"" <<
            endl;
        }
    
        currentVoice->
          appendNoteToVoice (newNote);

        // fCurrentSingleTremolo is handled in attachCurrentSingleTremoloToNote()
        break;
        
      case kStartTremolo:
        // register newNote as first element of the current double tremolo
        if (gGeneralOptions->fTraceNotes) {
          cerr <<  idtr <<
            "--> setting standalone note '" <<
            newNote->noteAsString () <<
            "', line " << newNote->getInputLineNumber () <<
            ", as double tremolo first element" <<
            " in voice \"" <<
            currentVoice->getVoiceName () <<
            "\"" <<
            endl;
        }

        fCurrentDoubleTremolo->
          setDoubleTremoloNoteFirstElement (
            newNote);
        break;

      case kStopTremolo:
        // register newNote as second element of the current double tremolo
        if (gGeneralOptions->fTraceNotes) {
          cerr <<  idtr <<
            "--> setting standalone note '" <<
            newNote->noteAsString () <<
            "', line " << newNote->getInputLineNumber () <<
            ", as double tremolo second element" <<
            " in voice \"" <<
            currentVoice->getVoiceName () <<
            "\"" <<
            endl;
        }

        fCurrentDoubleTremolo->
          setDoubleTremoloNoteSecondElement (
            newNote);

        // append current double tremolo to current voice
        currentVoice->
          appendDoubleTremoloToVoice (
            fCurrentDoubleTremolo);

        // forget about the current double tremolo
       // fCurrentDoubleTremolo = 0; // JMI not if there's a chord in the double tremolo XXL BOF
        break;
    } // switch
  }

  
  else {
    // standalone note or rest

    if (fCurrentGracenotes)
      // this is the first note after the grace notes,
      // forget about the latter
      fCurrentGracenotes = 0;
  
    // attach the pending elements, if any, to the note
    attachPendingElementsToNote (newNote);
  
    // append newNote to the current voice
    if (gGeneralOptions->fTraceNotes) {
      cerr <<  idtr <<
        "--> adding standalone " <<
        newNote->noteAsString () <<
        ", line " << newNote->getInputLineNumber () <<
        ", to voice \"" <<
        currentVoice->getVoiceName () <<
        "\"" <<
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
        ":" << fSyllableNote->noteSoundingDivisionsAsMsrString () <<
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

  // lyric has to be handled in all cases
  // in case they are empty at the beginning of the voice JMI
  handleLyric (
    currentVoice, newNote);

  // take care of slurs JMI ???
  if (fCurrentSlurKind == msrSlur::kStartSlur)
    fFirstSyllableInSlurKind = fCurrentSyllableKind;
    
  if (fCurrentSlurKind == msrSlur::kStopSlur)
    fFirstSyllableInSlurKind = msrSyllable::k_NoSyllable;

  // take care of ligatures JMI ???
  if (fCurrentLigatureKind == msrLigature::kStartLigature)
    fFirstSyllableInLigatureKind = fCurrentSyllableKind;
    
  if (fCurrentLigatureKind == msrLigature::kStopLigature)
    fFirstSyllableInLigatureKind = msrSyllable::k_NoSyllable;

  // account for chord not being built
  fOnGoingChord = false;
}

//______________________________________________________________________________
void xml2MsrTranslator::handleLyric (
  S_msrVoice currentVoice,
  S_msrNote newNote)
{
  int inputLineNumber =
    newNote->getInputLineNumber ();

  if (gGeneralOptions->fTraceLyrics) {
    cerr << idtr <<
      "Handling lyric" <<
      ", currentVoice = \"" << currentVoice->getVoiceName () <<"\"" <<
      ", newNote = \"" << newNote->noteAsShortString () << "\"" <<
      endl;
  }

/* JMI 

// JMI ???

    // the presence of a '<lyric />' ends the effect
    // of an on going syllable extend
    fOnGoingSyllableExtend = false;
    
    if (fOnGoingSlur)
      fOnGoingSlurHasStanza = true;
      
    fCurrentNoteHasStanza = true;
  }
  */


  if (fCurrentNoteSyllables.size ()) {
    for (
      list<S_msrSyllable>::const_iterator i =
        fCurrentNoteSyllables.begin();
      i != fCurrentNoteSyllables.end();
      i++ ) {
      // set syllables note uplink to newNote
      (*i)->setSyllableNoteUplink (newNote);

      // register syllable in current voice
      currentVoice->
        appendSyllableToVoice (
          inputLineNumber,
          fCurrentStanzaNumber,
          (*i));

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
        createVoiceInStaffInCurrentPartIfNeeded (
          inputLineNumber,
          fCurrentNoteStaffNumber,
          fCurrentVoiceNumber);

/*
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
void xml2MsrTranslator::handleNoteBelongingToAChord (
  S_msrNote newChordNote)
{
/*
  The chord element indicates that this note is an additional
  chord tone with the preceding note. The duration of this
  note can be no longer than the preceding note. In MuseData,
  a missing duration indicates the same length as the previous
  note, but the MusicXML format requires a duration for chord
  notes too.
  *
  Here:
    fLastHandledNoteInVoice is the note preceding newChordNote
    if we're not yet ??? JMI
*/

  // set newChordNote kind as a chord member
  newChordNote->
    setNoteKind (msrNote::kChordMemberNote);

  if (gGeneralOptions->fTraceChords) {
    cerr << idtr <<
      "xml2MsrTranslator::handleNoteBelongingToAChord()" <<
      ", newChordNote:" <<
      endl;

    idtr++;

    cerr << idtr << newChordNote <<
      endl;

    idtr--;
  }

  int inputLineNumber =
    newChordNote->getInputLineNumber ();
    
  if (fCurrentNoteIsARest) {
    msrMusicXMLError (
      inputLineNumber,
      "a rest cannot belong to a chord");
  }

  // should a chord be created?
  if (! fOnGoingChord) {
    // newChordNote is the second note of the chord to be created

    // fetch current voice
    S_msrVoice
      currentVoice =
        createVoiceInStaffInCurrentPartIfNeeded (
          inputLineNumber,
          fCurrentNoteStaffNumber,
          fCurrentVoiceNumber);

    // fetch this chord's first note,
    // i.e the last handled note for this voice
    S_msrNote
      chordFirstNote =
       fLastHandledNoteInVoice [currentVoice];
    
    if (! chordFirstNote) {
      stringstream s;

      s <<
        "handleNoteBelongingToAChord():" <<
        endl <<
        idtr <<
          "chordFirstNote is null on " <<
          newChordNote->noteAsString ();
        
      msrInternalError (
        inputLineNumber,
        s.str());
    }
        
    if (gGeneralOptions->fTraceChords) {
      cerr << idtr <<
        "xml2MsrTranslator::handleNoteBelongingToAChord()" <<
        ", chordFirstNote:" <<
        endl;

      idtr++;

      cerr << idtr <<
        chordFirstNote <<
        endl;

      idtr--;
    }

    // fetch chord first note's kind before createChordFromItsFirstNote(),
    // because the latter will change it to kChordMemberNote
    msrNote::msrNoteKind
      savedChordFirstNoteKind =
        chordFirstNote->getNoteKind ();
        
    if (gGeneralOptions->fTraceChords) {
      cerr << idtr <<
        "xml2MsrTranslator::handleNoteBelongingToAChord()" <<
        ", savedChordFirstNoteKind = " <<
        msrNote::noteKindAsString (savedChordFirstNoteKind) <<
        endl;

      idtr++;

      cerr << idtr <<
        chordFirstNote <<
        endl;

      idtr--;
    }

    // create the chord from its first note
    fCurrentChord =
      createChordFromItsFirstNote (
        currentVoice,
        chordFirstNote);

    // handle chord's first note
    switch (savedChordFirstNoteKind) {
      case msrNote::kRestNote:
        break;
        
      case msrNote::kSkipNote:
        break;
        
      case msrNote::kStandaloneNote:
        // remove last handled (previous current) note from the current voice
        if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceChords) {
          cerr << idtr <<
            "--> removing chord first note " <<
            chordFirstNote->noteAsShortString () <<
            ", line " << inputLineNumber <<
            ", from voice \"" << currentVoice->getVoiceName () << "\"" <<
            endl;
        }
    
        if (false) { // JMI
          cerr <<
            endl << endl <<
            "&&&&&&&&&&&&&&&&&& currentVoice (" <<
            currentVoice->getVoiceName () <<
            ") contents &&&&&&&&&&&&&&&&&&" <<
            endl <<
            currentVoice <<
            endl << endl;
        }
        
        currentVoice->
          removeNoteFromVoice (
            inputLineNumber,
            chordFirstNote);
    
        // add fCurrentChord to the voice instead
        if (gGeneralOptions->fTraceChords) {
          cerr << idtr <<
            "Appending chord " << fCurrentChord->chordAsString () <<
            " to voice \"" <<
            currentVoice->getVoiceName () <<
            "\"" <<
            endl;
        }
            
        currentVoice->
          appendChordToVoice (fCurrentChord);
        break;
        
      case msrNote::kDoubleTremoloMemberNote:
        {
          // fetch chordFirstNote's sounding divisions
          int chordFirstNoteSoundingDivisions =
            chordFirstNote->
              getNoteSoundingDivisions ();

          /* JMI
          // updating chord's divisions // JMI
          if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceChords) {
            cerr <<
              idtr <<
                "Updating sounding divisions for double tremolo chord '" <<
                "' " << fCurrentChord->chordAsString () <<
                " to '" << chordFirstNoteSoundingDivisions <<
                "' in voice \"" <<
                currentVoice->getVoiceName () <<
                "\"" <<
                endl;
          }
          
          fCurrentChord->
            setChordSoundingDivisions ( // ??? JMI
              chordFirstNoteSoundingDivisions);
              */
              
          if (chordFirstNote->getNoteIsFirstNoteInADoubleTremolo ()) {
            // replace double tremolo's first element by chord
            fCurrentDoubleTremolo->
              setDoubleTremoloChordFirstElement (
                fCurrentChord);
          }
          
          else if (chordFirstNote->getNoteIsSecondNoteInADoubleTremolo ()) {
            // replace double tremolo's second element by chord
            fCurrentDoubleTremolo->
              setDoubleTremoloChordSecondElement (
                fCurrentChord);
          }
          
          else {
            stringstream s;
  
            s <<
              "chord first note '" << chordFirstNote->noteAsShortString () <<
              "' belongs to a double tremolo, but is not marked as such";
  
            msrInternalError (
              newChordNote->getInputLineNumber (),
              s.str());
          }
        }
        break;
        
      case msrNote::kGraceNote:
        break;
        
      case msrNote::kChordMemberNote:
        // error? JMI
        break;
        
      case msrNote::kTupletMemberNote:
        break;
  
      case msrNote::k_NoNoteKind:
        break;
    } // switch

    // account for chord being built
    fOnGoingChord = true;
  }

  // register newChordNote as another member of fCurrentChord
  if (gGeneralOptions->fTraceChords)
    cerr << idtr <<
      "Adding another note " <<
      newChordNote->noteAsString() <<
      ", line " << inputLineNumber <<
      " to current chord" <<
      endl;
  
  fCurrentChord->
    addAnotherNoteToChord (newChordNote);

  // copy newChordNote's elements if any to the chord
  copyNoteElementsToChord (newChordNote, fCurrentChord);
}

//______________________________________________________________________________
void xml2MsrTranslator::handleNoteBelongingToATuplet (
  S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();
    
 // register note as a tuplet member
  note->
    setNoteKind (msrNote::kTupletMemberNote);

  // apply tuplet display factor to note
  note->
    applyTupletMemberDisplayFactorToNote (
      fCurrentActualNotes, fCurrentNormalNotes);

  if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceTuplets)
    cerr << idtr <<
      "xml2MsrTranslator::handleNoteBelongingToATuplet " <<
      note->noteAsShortStringWithRawDivisions () <<
      endl;

  // attach the pending elements, if any, to the note
  attachPendingElementsToNote (note);

  // is there an ongoing chord?
  if (! fOnGoingChord) {
    // note is the first one after a chord in a tuplet,
    // JMI
  }

  switch (fCurrentTupletKind) {
    case msrTuplet::kStartTuplet:
      {
        if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceTuplets)
          cerr << idtr <<
            "--> kStartTuplet: note = '" <<
            note->noteAsShortStringWithRawDivisions () <<
            "', line " << inputLineNumber <<
            endl;

        if (fCurrentATupletStopIsPending) {
          // finalize the tuplet, only now in case the last element
          // is actually a chord
          finalizeTuplet (inputLineNumber);

          fCurrentATupletStopIsPending = false;
        }
        
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
        if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceTuplets)
          cerr << idtr <<
            "--> kContinueTuplet: adding tuplet member note '" <<
            note->noteAsShortStringWithRawDivisions () <<
            "' to stack top tuplet '" <<
            currentTuplet->tupletAsShortString () <<
            "', line " << inputLineNumber <<
            endl;

        fTupletsStack.top()->
          addNoteToTuplet (note);
/* JMI
        // set note displayed divisions
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
            "tuplet member note '" <<
            note->noteAsShortStringWithRawDivisions () <<
            "' cannot be added, tuplets stack is empty";

          msrInternalError (
            inputLineNumber,
            s.str());
        }
      }
      break;

    case msrTuplet::kStopTuplet:
      {
        if (fTupletsStack.size ()) {
          S_msrTuplet
            currentTuplet =
              fTupletsStack.top ();
              
        // populate the tuplet at the top of the stack
        if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceTuplets)
          cerr << idtr <<
            "--> kStopTuplet: adding tuplet member note '" <<
            note->noteAsShortStringWithRawDivisions () <<
            "' to stack top tuplet '" <<
            currentTuplet->tupletAsShortString () <<
            "', line " << inputLineNumber <<
            endl;

        fTupletsStack.top()->
          addNoteToTuplet (note);
/* JMI
        // set note displayed divisions
        note->
          applyTupletMemberDisplayFactor (
            fTupletsStack.top ()->getTupletActualNotes (),
            fTupletsStack.top ()->getTupletNormalNotes ());
*/
          if (fCurrentATupletStopIsPending) {
            // end of a tuplet forces handling of the pending one 
            finalizeTuplet (inputLineNumber);

            fCurrentATupletStopIsPending = false;
          }
        }
        
        else {
          
          stringstream s;

          s <<
            "handleNoteBelongingToATuplet():" <<
            endl <<
            "tuplet member note '" <<
            note->noteAsShortStringWithRawDivisions () <<
            "' cannot be added, tuplets stack is empty";

          msrInternalError (
            inputLineNumber,
            s.str());
        }

        // finalizeTuplet() should be delayed in case this note
        // is the first one of a chord in a tuplet JMI XXL ???

        fCurrentATupletStopIsPending = true;
      }
      break;

    case msrTuplet::k_NoTuplet:
      break;
  } // switch
}

//______________________________________________________________________________
void xml2MsrTranslator::handleNoteBelongingToAChordInATuplet (
  S_msrNote newChordNote)
{
  /*
   The first note of a chord belonging to a tuplet
   is marked in MusicXML as a tuplet member only,
   it has already been appended to the voice in
   handleStandaloneOrDoubleTremoloNoteOrGraceNoteOrRest (),
   and the following ones are marked as both a tuplet and a chord member
  */
  
  // set new note kind as a chord member
  newChordNote->
    setNoteKind (msrNote::kChordMemberNote);

  // apply tuplet display factor to note
  newChordNote->
    applyTupletMemberDisplayFactorToNote (
      fCurrentActualNotes, fCurrentNormalNotes);

  if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceChords || gGeneralOptions->fTraceTuplets)
    cerr << idtr <<
      "xml2MsrTranslator::handleNoteBelongingToAChordInATuplet " <<
      newChordNote->noteAsShortStringWithRawDivisions () <<
      endl;

  int inputLineNumber =
    newChordNote->getInputLineNumber ();
    
  if (fCurrentNoteIsARest)
    msrMusicXMLError (
      inputLineNumber,
      "a rest cannot belong to a chord");

  // should a chord be created?
  if (! fOnGoingChord) {
    // this is the second note of the chord to be created,
    // fLastHandledNote being the first one and marked as a tuplet member

    // fetch current voice
    S_msrVoice
      currentVoice =
        createVoiceInStaffInCurrentPartIfNeeded (
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
        "handleNoteBelongingToAChordInATuplet:" <<
        endl <<
        idtr <<
          "lastHandledNoteInVoice is null on " <<
          newChordNote->noteAsString ();
        
      msrInternalError (
        inputLineNumber,
        s.str());
    }
        
    // create the chord from its first note
    fCurrentChord =
      createChordFromItsFirstNote (
        currentVoice,
        lastHandledNoteInVoice);

    if (false)
      cerr <<
        endl << endl <<
        "&&&&&&&&&&&&&&&&&& currentVoice (" <<
        currentVoice->getVoiceName () <<
        ") contents &&&&&&&&&&&&&&&&&&" <<
        endl <<
        currentVoice <<
        endl << endl;

    if (fTupletsStack.size ()) {
      S_msrTuplet
        currentTuplet =
          fTupletsStack.top ();
          
      // remove last handled (previous current) note from the current tuplet
      if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceChords || gGeneralOptions->fTraceTuplets)
        cerr << idtr <<
          "--> removing last handled note " <<
          lastHandledNoteInVoice->noteAsShortStringWithRawDivisions () <<
          ", line " << inputLineNumber <<
          ", from tuplet '" <<
          currentTuplet->tupletAsShortString () <<
          "'" <<
          endl;

      // remove lastHandledNoteInVoice from the current voice
      currentTuplet->
        removeFirstNoteFromTuplet (
          inputLineNumber,
          lastHandledNoteInVoice);
  
      // add fCurrentChord to the current tuplet instead
      if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceChords || gGeneralOptions->fTraceTuplets)
      cerr << idtr <<
        "--> adding chord " << fCurrentChord->chordAsString () <<
        " to stack top tuplet '" <<
        currentTuplet->tupletAsShortString () <<
        "', line " << inputLineNumber <<
        endl;

      currentTuplet->
        addChordToTuplet (fCurrentChord);
  /* JMI
      // set note displayed divisions
      note->
        applyTupletMemberDisplayFactor (
          fTupletsStack.top ()->getTupletActualNotes (),
          fTupletsStack.top ()->getTupletNormalNotes ());
  */
    }
    
    else {
      stringstream s;

      s <<
        "handleNoteBelongingToAChordInATuplet():" <<
        endl <<
        "tuplet member chord " << fCurrentChord->chordAsString () <<
        "cannot be added, tuplets stack is empty";

      msrInternalError (
        inputLineNumber,
        s.str());
    }

    // account for chord being built
    fOnGoingChord = true;
  }

  // register note as another member of fCurrentChord
  if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceChords)
    cerr << idtr <<
      "--> adding another note " <<
      newChordNote->noteAsShortStringWithRawDivisions() <<
      ", line " << inputLineNumber <<
      " to current chord" <<
      endl;
  
  fCurrentChord->
    addAnotherNoteToChord (newChordNote);

  // copy newChordNote's elements if any to the chord
  copyNoteElementsToChord (newChordNote, fCurrentChord);
}

//______________________________________________________________________________
void xml2MsrTranslator::handleTupletsPendingOnTupletStack (
  int inputLineNumber)
{
  if (gGeneralOptions->fTraceTuplets)
    cerr << idtr <<
      "=== handleTupletsPendingOnTupletStack()" <<
      ", line = " << inputLineNumber <<
      endl;

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentNoteStaffNumber,
        fCurrentVoiceNumber);

  // handle tuplets pending on the tuplet stack
  while (fTupletsStack.size ()) {
    S_msrTuplet
      pendingTuplet =
        fTupletsStack.top ();

    // finalize the tuplet, thus popping it off the stack
    finalizeTuplet (inputLineNumber);

    /* JMI
    // pop it from the tuplets stack
  if (gGeneralOptions->fTraceTuplets)
      cerr << idtr <<
        "--> popping tuplet 1 '" <<
        pendingTuplet->tupletAsShortString () <<
        "' from tuplets stack" <<
        endl;
      fTupletsStack.pop ();        

    if (fTupletsStack.size ()) {
      
      // tuplet is a nested tuplet
  //    if (gGeneralOptions->fDebug)
        cerr << idtr <<
          "=== adding nested tuplet '" <<
        pendingTuplet->tupletAsShortString () <<
        "' to " <<
        fTupletsStack.top ()->tupletAsShortString () <<
        " current stack top tuplet" <<
      endl;
      
      fTupletsStack.top ()->
        addTupletToTuplet (pendingTuplet);
    }
    
    else {
      
      // pendingTuplet is a top level tuplet
  //    if (gGeneralOptions->fDebug)
        cerr << idtr <<
          "=== adding top level tuplet 1 '" <<
        pendingTuplet->tupletAsShortString () <<
        "' to voice \"" <<
        currentVoice->getVoiceName () <<
        "\"" <<
        endl;
        
      currentVoice->
        appendTupletToVoice (pendingTuplet);
    }  
    */
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
void xml2MsrTranslator::handleRepeatStart (
  S_barline     elt,
  S_msrBarline& barline)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTraceRepeats)
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

/* JMI
  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  if (! fRepeatHasBeenCreatedForCurrentPart) {
    if (gGeneralOptions->fTrace)
      cerr << idtr <<
        "Appending a repeat to part " <<
        fCurrentPart->getPartCombinedName () <<
        endl;
  
    fCurrentPart->
      createAndAppendRepeatToPart (inputLineNumber);

    fRepeatHasBeenCreatedForCurrentPart = true;  
  }
*/

  // append the bar line to the part
  fCurrentPart->
    appendBarlineToPart (barline);

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

  if (gGeneralOptions->fTraceRepeats) {
    cerr <<
      idtr << "--> input line " << inputLineNumber <<
      endl <<
      idtr <<
      "--> barline, right and backward: repeat end" <<
      endl;
  }

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  // set the barline category
  barline->
    setBarlineCategory (msrBarline::kRepeatEnd);

  // append the bar line to the current voice
  currentVoice->
    appendBarlineToVoice (barline);

  if (fPendingBarlines.empty ()) {
    if (gGeneralOptions->fTraceRepeats) {
      cerr <<
        idtr <<
        "There is an implicit repeat start at the beginning of part" <<
        fCurrentPart->getPartCombinedName () <<
        endl;
    }

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
          fCurrentBarlineEndingNumber,
          msrBarline::kForward,
          fCurrentBarlineRepeatWinged);

    // set the implicit barline category
    implicitBarline->
      setBarlineCategory (
        msrBarline::kRepeatStart);
  
    // prepend the implicit barline to the voice
    currentVoice->
      prependBarlineToVoice (implicitBarline);
            
    if (! fRepeatHasBeenCreatedForCurrentPart) {
  /*  JMI
      if (gGeneralOptions->fTrace)
        cerr << idtr <<
          "Appending an implicit repeat to part " <<
          fCurrentPart->getPartCombinedName () <<
          endl;

      fCurrentPart->
        createAndAppendRepeatToPart (inputLineNumber);
  
      fRepeatHasBeenCreatedForCurrentPart = true;  
*/
    }
  }

  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Appending an implicit repeat to part " <<
      fCurrentPart->getPartCombinedName () <<
      endl;

  fCurrentPart->
    createAndAppendRepeatToPart (inputLineNumber);

  fRepeatHasBeenCreatedForCurrentPart = true;  
}

//______________________________________________________________________________
void xml2MsrTranslator::handleHookedEndingStart (
  S_barline     elt,
  S_msrBarline& barline)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTraceRepeats)
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

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  if (! fRepeatHasBeenCreatedForCurrentPart) {
    // append an implicit repeat to cht current part
    if (gGeneralOptions->fTraceRepeats)
      cerr << idtr <<
        "Appending an implicit repeat to part " <<
        fCurrentPart->getPartCombinedName () <<
        endl;
  
    fCurrentPart->
      createAndAppendRepeatToPart (inputLineNumber);

    fRepeatHasBeenCreatedForCurrentPart = true;  
  }

  // set the barline category
  barline->
    setBarlineCategory (msrBarline::kHookedEndingStart);
  
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

  if (gGeneralOptions->fTraceRepeats)
    cerr <<
      idtr << "--> input line " << inputLineNumber <<
      endl <<
      idtr <<
      "--> barline right, stop: hooked ending end" <<
      endl;

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  // set the barline category
  barline->
    setBarlineCategory (msrBarline::kHookedEndingEnd);

  // append the bar line to the current segment
  currentVoice->
    appendBarlineToVoice (barline);

  // create a hooked repeat ending from the current segment
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Appending a new hookless repeat ending to part " <<
      fCurrentPart->getPartCombinedName () <<
      endl;
          
  fCurrentPart->
    appendRepeatendingToPart (
      inputLineNumber,
      fCurrentBarlineEndingNumber,
      msrRepeatending::kHookedEnding);
}

//______________________________________________________________________________
void xml2MsrTranslator::handleHooklessEndingStart (
  S_barline     elt,
  S_msrBarline& barline)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTraceRepeats)
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

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  // set the barline category
  barline->
    setBarlineCategory (msrBarline::kHooklessEndingStart);
  
  // append the bar line to the voice
  currentVoice-> // JMI
    appendBarlineToVoice (barline);

  // push the barline onto the stack
  fPendingBarlines.push (barline); // JMI
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
  
  if (gGeneralOptions->fTraceRepeats)
    cerr <<
      idtr << "--> input line " << inputLineNumber <<
      endl <<
      idtr <<
      "--> barline, right and discontinue: hookless ending end" <<
      endl;

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  // set the barline category
  barline->
    setBarlineCategory (msrBarline::kHooklessEndingEnd);
  
  // append the bar line to the current segment
  currentVoice->
    appendBarlineToVoice (barline);

  // create a hookless repeat ending from the current segment
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Appending a new hookless repeat ending to part " <<
      fCurrentPart->getPartCombinedName () <<
      endl;
                
  fCurrentPart->
    appendRepeatendingToPart (
      inputLineNumber,
      fCurrentBarlineEndingNumber,
      msrRepeatending::kHooklessEnding);
}

//______________________________________________________________________________
void xml2MsrTranslator::visitStart ( S_rehearsal& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_rehearsal" <<
      endl;

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
      createVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);
    
  // create a rehearsal
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Creating rehearsal \"" << rehearsalValue << "\"" <<
      " in voice " <<
      currentVoice->getVoiceName () <<
      endl;

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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_harmony" <<
      endl;

  fPendingHarmony                  = true;
  fCurrentHarmonyInputLineNumber   = elt->getInputLineNumber ();
  fCurrentHarmonyRootDiatonicPitch = k_NoDiatonicPitch;
  fCurrentHarmonyRootAlteration    = kNatural;
  fCurrentHarmonyKind              = msrHarmony::k_NoHarmony;
  fCurrentHarmonyKindText          = "";
  fCurrentHarmonyBassDiatonicPitch = k_NoDiatonicPitch;
  fCurrentHarmonyBassAlteration    = kNatural;
  fCurrentHarmonyDegreeValue       = -1;
  fCurrentHarmonyDegreeAlteration  = kNatural;
}

void xml2MsrTranslator::visitStart ( S_root_step& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_root_step" <<
      endl;

  string step = elt->getValue ();

  checkStep (
    elt->getInputLineNumber (),
    step);
     
  fCurrentHarmonyRootDiatonicPitch =
    msrDiatonicPitchFromString (step [0]);
}

void xml2MsrTranslator::visitStart ( S_root_alter& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_root_alter" <<
      endl;

  float rootAlter = (float)(*elt);

  fCurrentHarmonyRootAlteration =
    msrAlterationFromMusicXMLAlter (
      rootAlter);
      
  if (fCurrentHarmonyRootAlteration == k_NoAlteration) {
    stringstream s;

    s <<
      "root alter '" << rootAlter << "'"
      "' should be -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5 or +2";
      
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }
}

void xml2MsrTranslator::visitStart ( S_kind& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_kind" <<
      endl;

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
    
  else if (kind == "none") {
    // ad-hoc error recovery
    msrMusicXMLWarning (
      elt->getInputLineNumber (),
      "unknown harmony kind \"" + kind + "\", replaced by 'major'");

    fCurrentHarmonyKind = msrHarmony::kMajor; 
  }
    
  else {
    if (kind.size ()) {
      msrMusicXMLError (
        elt->getInputLineNumber (),
        "unknown harmony kind \"" + kind + "\"");
    }
    else {
      msrMusicXMLWarning (
        elt->getInputLineNumber (),
        "empty harmony kind, replaced by 'major'");

      fCurrentHarmonyKind = msrHarmony::kMajor; 
    }
  }
}

void xml2MsrTranslator::visitStart ( S_bass_step& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_bass_step" <<
      endl;

  string step = elt->getValue();
  
  checkStep (
    elt->getInputLineNumber (),
    step);

  fCurrentHarmonyBassDiatonicPitch =
    msrDiatonicPitchFromString (step [0]);
}

void xml2MsrTranslator::visitStart ( S_bass_alter& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_bass_alter" <<
      endl;

  float bassAlter = (float)(*elt);

  fCurrentHarmonyBassAlteration =
    msrAlterationFromMusicXMLAlter (
      bassAlter);
      
  if (fCurrentHarmonyBassAlteration == k_NoAlteration) {
    stringstream s;

    s <<
      "bass alter '" << bassAlter << "'"
      "' should be -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5 or +2";
      
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }
}

void xml2MsrTranslator::visitStart ( S_degree_value& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_degree_value" <<
      endl;

  fCurrentHarmonyDegreeValue = (int)(*elt);
}

void xml2MsrTranslator::visitStart ( S_degree_alter& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_degree_alter" <<
      endl;

  float degreeAlter = (float)(*elt);

  fCurrentHarmonyDegreeAlteration =
    msrAlterationFromMusicXMLAlter (
      degreeAlter);
      
  if (fCurrentHarmonyDegreeAlteration == k_NoAlteration) {
    stringstream s;

    s <<
      "degree alter '" << degreeAlter << "'"
      "' should be -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5 or +2";
      
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }
}

void xml2MsrTranslator::visitStart ( S_degree_type& elt )
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> Start visiting S_degree_type" <<
      endl;

  string degreeType = elt->getValue ();

  msrHarmony::msrHarmonyDegreeTypeKind
    degreeTypeKind; // JMI

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
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "--> End visiting S_harmony" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
  
  // convert root diatonic pitch to a quarter tone pitch
  fCurrentHarmonyRootQuartertonesPitch =
    quatertonesPitchFromDiatonicPitchAndAlteration (
      inputLineNumber,
      fCurrentHarmonyRootDiatonicPitch,
      fCurrentHarmonyRootAlteration);

  // convert bass diatonic pitch to a quarter tone pitch
  fCurrentHarmonyBassQuartertonesPitch =
    quatertonesPitchFromDiatonicPitchAndAlteration (
      inputLineNumber,
      fCurrentHarmonyBassDiatonicPitch,
      fCurrentHarmonyBassAlteration);

  if (
    fCurrentHarmonyRootQuartertonesPitch
      ==
    fCurrentHarmonyBassQuartertonesPitch) {
    stringstream s;

    s <<
      "harmony root and bass notes are both equal to '" <<
      msrDiatonicPitchAsString (
        gMsrOptions->fMsrQuatertonesPitchesLanguage,
        msrDiatonicPitchFromQuatertonesPitch (
          inputLineNumber,
          fCurrentHarmonyRootQuartertonesPitch)) <<        
      "', ignoring the latter";

    msrMusicXMLWarning (
     inputLineNumber,
     s.str());

    fCurrentHarmonyBassQuartertonesPitch = k_NoQuaterTonesPitch;
  }

  // the harmony will be created in visisEnd (S_note&),
  // when its duration can be be taken as that of the note following it
  // create the harmony
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


/* JMI
  currentVoice->
    catchupToMeasureLocation (
      inputLineNumber,
      measureLocation);
        */
                    
/* JMI
  for (int i = 0; i < fCurrentForwardDuration; i++) {
    // generate rests for the duration of the forward move
    int restDivisions = 1;
    
    S_msrNote
      rest =
        msrNote::  createRest (
            inputLineNumber,
          1, // JMI
          fCurrentStaffNumber,
          fCurrentVoiceNumber);
  
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

