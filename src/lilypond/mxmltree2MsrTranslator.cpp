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

#include "msrWarningsAndErrors.h"

#include "mxmltree2MsrTranslator.h"

#include "lpsrOptions.h"


using namespace std;

namespace MusicXML2
{

// useful shortcut macros
#define idtr indenter::gIndenter
#define tab  indenter::gIndenter.getSpacer ()

//________________________________________________________________________
void mxmltree2MsrTranslator::initializeNoteData ()
{
  // basic note description

// JMI  fCurrentNoteKind = k_NoNoteKind;

  fCurrentNoteQuarterTonesPitch  = k_NoQuarterTonesPitch;
  
  fCurrentNoteSoundingWholeNotes  = rational (-17, 1);
  fCurrentNoteDisplayWholeNotes = rational (0, 1);
  
  fCurrentNoteDotsNumber = 0;
  
  fCurrentNoteGraphicDuration = k_NoDuration;

  fCurrentNoteOctave = K_NO_OCTAVE;

  fCurrentNoteQuarterTonesDisplayPitch = k_NoQuarterTonesPitch;
  fCurrentDisplayDiatonicPitch = k_NoDiatonicPitch;  
  fCurrentDisplayOctave = K_NO_OCTAVE;

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

mxmltree2MsrTranslator::mxmltree2MsrTranslator ()
{
  // initialize note data to a neutral state
  initializeNoteData ();

  // the MSR score we're building
  fMsrScore =
    msrScore::create (0);

  // geometry handling
  fCurrentMillimeters = -1;
  fCurrentTenths      = -1;
  
  fOnGoingPageLayout = false;

  // part group handling
  fCurrentPartUsesImplicitPartGroup = false;
  
  fOnGoingGroupNameDisplay = false;

  // part handling

  // measure style handling
  fCurrentBeatRepeatSlashes = -1;

  fCurrentMeasureRepeatKind =
    msrMeasureRepeat::k_NoMeasureRepeat;

  fCurrentMeasureRepeatMeasuresNumber = -1;
  fCurrentMeasureRepeatSlashesNumber  = -1;
  
  fCurrentMultipleRestMeasuresNumber   = 0;
  fRemainingMultipleRestMeasuresNumber = 0;
  fOnGoingMultipleRest = false;
  fCurrentMultipleRestHasBeenCreated = false;
  
  // staff details handling
  fStaffDetailsStaffNumber = -1;
  
  fCurrentStaffTypeKind =
    msrStaffDetails::kRegularStaffType;

  fCurrentShowFretsKind =
    msrStaffDetails::kShowFretsNumbers; // default value

  fCurrentPrintObjectKind =
    msrStaffDetails::kPrintObjectYes; // default value

  fCurrentPrintSpacingKind =
    msrStaffDetails::kPrintSpacingNo; // default value ??? JMI

  fCurrentStaffTuningAlteration = k_NoAlteration;
  fCurrentStaffTuningOctave     = -1;

  fCurrentStaffDetailsCapo = 0;
  fCurrentStaffDetailsStaffSize = 0;

  // staff handling
  fCurrentStaffNumber = -1;

  // voice handling
  fCurrentVoiceNumber = -1;

  // clef handling
  fCurrentClefStaffNumber = -1;
  fCurrentClefSign = "";
  fCurrentClefLine = -1;
  fCurrentClefOctaveChange = -77;

  // key handling
  fCurrentKeyKind = msrKey::kTraditionalKind;
  
  fCurrentKeyStaffNumber = -21;
  fCurrentKeyFifths = -1;
  fCurrentKeyCancelFifths = -37;
  fCurrentKeyModeKind = msrKey::kMajorMode;

  // time handling
  fCurrentTimeSymbolKind =
    msrTime::k_NoTimeSymbol; // default value

  fOnGoingInterchangeable = false;
  
  // transpose handling
  fCurrentTransposeNumber = -213;
  fCurrentTransposeDiatonic = -214;
  fCurrentTransposeChromatic = - 215;

  // direction handling
  fCurrentWordsContents = "";
  fCurrentDirectionStaffNumber = 1; // it may be absent
  fOnGoingDirection     = true;

  // direction-type handling
  fOnGoingDirectionType = false;

  // accordion-registration handling
  fCurrentAccordionHigh   = 0;
  fCurrentAccordionMiddle = 0;
  fCurrentAccordionLow    = 0;
  
  fCurrentAccordionNumbersCounter = 0;

  // metronome handling
  fCurrentMetrenomePerMinute = -1;
  fCurrentMetronomeParentheses = false;

  // time handling
  fCurrentTimeStaffNumber = -2;
  fCurrentTimeBeats = "";

  // measures
  fMeasuresCounter = 0;
    
  // lyric handling
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

  // harmonies handling
  fPendingHarmony                  = false;
  fCurrentHarmonyRootDiatonicPitch = k_NoDiatonicPitch;
  fCurrentHarmonyRootAlteration    = k_NoAlteration;
  fCurrentHarmonyKind              = msrHarmony::k_NoHarmony;
  fCurrentHarmonyKindText          = "";
  fCurrentHarmonyBassDiatonicPitch = k_NoDiatonicPitch;
  fCurrentHarmonyBassAlteration    = k_NoAlteration;
  fCurrentHarmonyDegreeValue       = -1;
  fCurrentHarmonyDegreeAlteration  = k_NoAlteration;

  // barline handling
  fOnGoingBarline = false;

  // repeats handling
  fRepeatHasBeenCreatedForCurrentPart = false;

  // MusicXML notes handling
  fCurrentNoteDiatonicPitch = k_NoDiatonicPitch;
  fCurrentNoteAlteration    = k_NoAlteration;
  fOnGoingNote            = false;

  // note context
  fCurrentNoteStaffNumber = 0;
  fCurrentNoteVoiceNumber = 0;

  // technicals handling
  fBendAlterValue = -39;

  // ornaments handling

  // grace notes handling

  // tremolos handling
  fCurrentMusicXMLTremoloType = k_NoTremolo;

  // chords handling
  fOnGoingChord = false;

  // tuplets handling
  fCurrentATupletStopIsPending = false;

  // ties handling

  // slurs handling
  fOnGoingSlur          = false;
  fOnGoingSlurHasStanza = false;

  // ligatures handling
  fOnGoingLigature          = false;
  fOnGoingLigatureHasStanza = false;

  // backup handling
  fCurrentBackupDurationDivisions = -1;
  fOnGoingBackup  = false;

  // forward handling
  fCurrentForwardDurationDivisions = 1;
  fCurrentForwardStaffNumber = 1;
  fCurrentForwardVoiceNumber = 1;
  fOnGoingForward = false;
}

mxmltree2MsrTranslator::~mxmltree2MsrTranslator ()
{}

//________________________________________________________________________
S_msrScore mxmltree2MsrTranslator::buildMsrScoreFromXMLElementTree (
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
void mxmltree2MsrTranslator::checkStep (
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
S_msrPartGroup mxmltree2MsrTranslator::createImplicitMsrPartGroupIfNotYetDone (
  int inputLineNumber)
{
  /*
  A first part group is created with all the needed contents
  if none is specified in the MusicXML data.
  */

  if (! fImplicitPartGroup) {
    // create an implicit part group
    fCurrentPartGroupNumber = 1;
    
    if (gGeneralOptions->fTracePartGroups)
      cerr << idtr <<
        "Creating an implicit part group with number " <<
        fCurrentPartGroupNumber <<
        ", line " << inputLineNumber <<
        endl;
  
    fImplicitPartGroup =
      msrPartGroup::create (
        inputLineNumber,
        fCurrentPartGroupNumber,
        "Implicit",
        "",
        "",
        "Impl.",
        msrPartGroup::kBracketPartGroupSymbol,
        0,
        true,
        0, // the top level part group has an empty uplink
        fMsrScore);
  
    // append it to the MSR score
    if (gGeneralOptions->fTracePartGroups)
      cerr << idtr <<
        "Appending implicit part group " <<
        fImplicitPartGroup->getPartGroupNumber () <<
        " to MSR score" <<
        endl;
        
    fMsrScore->
      addPartGroupToScore (fImplicitPartGroup);
      
    /* JMI
      this implicit part group will be added to the MSR score
      in method 'visitEnd (S_part_list& elt)'
    */
  }

  // add implicit part group to the map of this visitor
  if (gGeneralOptions->fTracePartGroups)
    cerr << idtr <<
      "Adding implicit part group " << fCurrentPartGroupNumber <<
      " to visitor's data" <<
      endl;
      
  fPartGroupsMap [fCurrentPartGroupNumber] = fImplicitPartGroup;
  fPartGroupsList.push_front (fImplicitPartGroup);

  fCurrentPartUsesImplicitPartGroup = true;
  
  return fImplicitPartGroup;
}

//______________________________________________________________________________
S_msrPartGroup mxmltree2MsrTranslator::fetchPartGroupInThisVisitor (
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
S_msrStaff mxmltree2MsrTranslator::createStaffInCurrentPartIfNotYetDone (
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
S_msrVoice mxmltree2MsrTranslator::createVoiceInStaffInCurrentPartIfNotYetDone (
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

      if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceVoices)
        cerr << idtr <<
          "Voice \"" <<  voice->getVoiceName () << "\"" <<
          " changes from staff " << voiceDisplayingStaffNumber <<
          " to staff " << staffNumber <<
          endl;

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

/* JMI
//______________________________________________________________________________
void mxmltree2MsrTranslator::visitStart ( S_comment& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_comment" <<
      endl;

  string comment = elt->getValue();
  
  cerr <<
    "---> comment = " << comment <<
    endl;

  abort(); // JMI ???
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::visitStart ( S_processing_instruction& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_processing_instruction" <<
      endl;

  string processingInstruction = elt->getValue();
  
  cerr <<
    "---> processingInstruction = " << processingInstruction <<
    endl;

  abort(); // JMI ???
}
*/

//______________________________________________________________________________
void mxmltree2MsrTranslator::visitStart ( S_work_number& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_work_number" <<
      endl;

  fMsrScore->getIdentification () ->
    setWorkNumber (
      elt->getInputLineNumber (),
      elt->getValue ());
}

void mxmltree2MsrTranslator::visitStart ( S_work_title& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_work_title" <<
      endl;

  fMsrScore->getIdentification () ->
    setWorkTitle (
      elt->getInputLineNumber (),
      elt->getValue ());
}
  
void mxmltree2MsrTranslator::visitStart ( S_movement_number& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_movement_number" <<
      endl;

  fMsrScore->getIdentification () ->
    setMovementNumber (
      elt->getInputLineNumber (),
      elt->getValue ());
}

void mxmltree2MsrTranslator::visitStart ( S_movement_title& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_movement_title" <<
      endl;

  fMsrScore->getIdentification () ->
    setMovementTitle (
      elt->getInputLineNumber (),
      elt->getValue ());
}

void mxmltree2MsrTranslator::visitStart ( S_creator& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_rights& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_rights" <<
      endl;

  fMsrScore->getIdentification () ->
    setRights (
      elt->getInputLineNumber (),
      elt->getValue ());
}

void mxmltree2MsrTranslator::visitStart ( S_software& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_software" <<
      endl;

  fMsrScore->getIdentification () ->
    addSoftware (
      elt->getInputLineNumber (),
      elt->getValue ());
}

void mxmltree2MsrTranslator::visitStart ( S_encoding_date& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_encoding_date" <<
      endl;

  fMsrScore->getIdentification () ->
    setEncodingDate (
      elt->getInputLineNumber (),
      elt->getValue ());
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::visitStart ( S_millimeters& elt )
{ 
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_millimeters" <<
      endl;

  fCurrentMillimeters = (float)(*elt);
  
  fMsrScore->getPageGeometry ()->
    setMillimeters (fCurrentMillimeters);
}

void mxmltree2MsrTranslator::visitStart ( S_tenths& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_tenths" <<
      endl;

  fCurrentTenths = (int)(*elt);

  fMsrScore->getPageGeometry ()->
    setTenths (fCurrentTenths);
}

void mxmltree2MsrTranslator::visitEnd ( S_scaling& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> End visiting S_scaling" <<
      endl;

  if (gGeneralOptions->fTraceGeometry)
    cerr << idtr <<
      "There are " << fCurrentTenths <<
      " tenths for " <<  fCurrentMillimeters <<
      " millimeters, hence the global staff size is " <<
      fMsrScore->getPageGeometry ()->globalStaffSize () <<
      endl;
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::visitStart ( S_system_distance& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_system_distance" <<
      endl;

  int systemDistance = (int)(*elt);
  
//  cerr << "--> systemDistance = " << systemDistance << endl;
  fMsrScore->getPageGeometry ()->
    setBetweenSystemSpace (
      systemDistance * fCurrentMillimeters / fCurrentTenths / 10);  
}

void mxmltree2MsrTranslator::visitStart ( S_top_system_distance& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
void mxmltree2MsrTranslator::visitStart ( S_page_layout& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_page_layout" <<
      endl;

  fOnGoingPageLayout = true;
}
void mxmltree2MsrTranslator::visitEnd ( S_page_layout& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> End visiting S_page_layout" <<
      endl;

  fOnGoingPageLayout = false;
}

void mxmltree2MsrTranslator::visitStart ( S_page_height& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_page_width& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_left_margin& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_right_margin& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_top_margin& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_bottom_margin& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
void mxmltree2MsrTranslator::visitStart ( S_credit& elt )
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

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_credit_words& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitEnd ( S_credit& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> End visiting S_credit" <<
      endl;

  fMsrScore->
    setCredit (fCurrentCredit);
  
  fCurrentCredit = 0;
}

//________________________________________________________________________
void mxmltree2MsrTranslator::visitStart (S_part_list& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitEnd (S_part_list& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> End visiting S_part_list" <<
      endl;

  idtr--;

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
void mxmltree2MsrTranslator::visitStart (S_part_group& elt)
{  
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_part_group" <<
      endl;

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

void mxmltree2MsrTranslator::visitStart (S_group_name& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_group_name" <<
      endl;

  fCurrentPartGroupName = elt->getValue();
}

void mxmltree2MsrTranslator::visitStart (S_group_name_display& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_group_name_display" <<
      endl;

  fOnGoingGroupNameDisplay = true;
}

void mxmltree2MsrTranslator::visitStart (S_display_text& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_display_text" <<
      endl;

  if (fOnGoingGroupNameDisplay)
    fCurrentPartGroupDisplayText = elt->getValue();
}

void mxmltree2MsrTranslator::visitStart (S_accidental_text& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_accidental_text" <<
      endl;

    fCurrentPartGroupAccidentalText = elt->getValue();
}

void mxmltree2MsrTranslator::visitStart (S_group_abbreviation& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_group_abbreviation" <<
      endl;

  fCurrentPartGroupAbbreviation = elt->getValue ();
}

void mxmltree2MsrTranslator::visitStart (S_group_symbol& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_group_symbol" <<
      endl;

  fCurrentPartGroupSymbol = elt->getValue ();

  fCurrentPartGroupSymbolDefaultX =
    elt->getAttributeIntValue ("default-x", 0);
}

void mxmltree2MsrTranslator::visitStart ( S_group_barline& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_group_barline" <<
      endl;

  fCurrentPartGroupBarline = elt->getValue ();
}

//________________________________________________________________________
void mxmltree2MsrTranslator::showPartGroupsData (string context)
{    
  if (gGeneralOptions->fTracePartGroups) {
    cerr << idtr <<
      "==> " << context << ": fPartGroupsMap contains:" <<
      endl;
      
    if (fPartGroupsMap.size()) {
      map<int, S_msrPartGroup>::const_iterator
        iBegin = fPartGroupsMap.begin(),
        iEnd   = fPartGroupsMap.end(),
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
        "<== fPartGroupsMap" <<
        endl << endl <<
        
      idtr <<
        "==> " << context << ": fPartGroupsList contains:" <<
        endl;
      
    if (fPartGroupsList.size()) {
      list<S_msrPartGroup>::const_iterator
        iBegin = fPartGroupsList.begin(),
        iEnd   = fPartGroupsList.end(),
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
      idtr << "<== fPartGroupsList" <<
      endl << endl;
  }
}

//________________________________________________________________________
void mxmltree2MsrTranslator::handlePartGroupStart (
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
      cerr << idtr <<
        "Adding part group " << fCurrentPartGroupNumber <<
        " to visitor's part group map" <<
        endl;
    fPartGroupsMap [fCurrentPartGroupNumber] =
      partGroupToBeStarted;
  }
  
  // add it to the part group list of this visitor
  if (gGeneralOptions->fTracePartGroups)
    cerr << idtr <<
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
void mxmltree2MsrTranslator::handlePartGroupStop (int inputLineNumber)
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
      s.str());
  }

  // remove the part group to be stopped from the part group list
  if (gGeneralOptions->fTracePartGroups)
    cerr << idtr <<
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
        s.str());
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
        cerr << idtr <<
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
        cerr << idtr <<
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
          s.str());
      }
      
      // insert current group into future current group
      if (gGeneralOptions->fTracePartGroups)
        cerr << idtr <<
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
    cerr << idtr <<
      "Removing part group " << fCurrentPartGroupNumber <<
      " from visitor's part group map" <<
      endl;
      
  try {
    fPartGroupsMap.erase (fCurrentPartGroupNumber);
  }
  catch (int e) {
    cerr <<
      "An exception number " << e << " occurred" <<
      endl;
  }

  showPartGroupsData ("AFTER STOP");
}

//________________________________________________________________________
void mxmltree2MsrTranslator::visitEnd (S_part_group& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> End visiting S_part_group" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTracePartGroups)
    cerr << idtr <<
      "Handling part group " << fCurrentPartGroupNumber <<
      ", type: \"" << fCurrentPartGroupType << "\""  <<
      endl;

  idtr++;
  
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

  idtr--;
}

//________________________________________________________________________
void mxmltree2MsrTranslator::visitStart (S_score_part& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart (S_part_name& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_part_name" <<
      endl;

  fCurrentPartName = elt->getValue ();
}

void mxmltree2MsrTranslator::visitStart (S_part_abbreviation& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_part_abbreviation" <<
      endl;

  fCurrentPartAbbreviation = elt->getValue ();
}

void mxmltree2MsrTranslator::visitStart (S_instrument_name& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_instrument_name" <<
      endl;

  fCurrentPartInstrumentName = elt->getValue();
}

void mxmltree2MsrTranslator::visitStart (S_instrument_abbreviation& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_instrument_abbreviation" <<
      endl;

  fCurrentPartInstrumentAbbreviation = elt->getValue();
}

void mxmltree2MsrTranslator::visitEnd (S_score_part& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
    cerr <<
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
  idtr--;
}

//________________________________________________________________________
void mxmltree2MsrTranslator::visitStart (S_part& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

  fPartVoiceNumberToVoiceMap.clear ();
  fPartVoiceNumberToDisplayingStaffNumberMap.clear ();
}

void mxmltree2MsrTranslator::visitEnd (S_part& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

//________________________________________________________________________
void mxmltree2MsrTranslator::visitStart (S_attributes& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_attributes" <<
      endl;

  fCurrentTime = 0;
}

void mxmltree2MsrTranslator::visitEnd (S_attributes& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> End visiting S_attributes" <<
      endl;

  // time is crucially needed for measures management,
  // we cannot stay without any
  if (false && ! fCurrentTime) { // JMI BOF
    // create the default 4/4 time
    fCurrentTime =
      msrTime::createFourQuartersTime (
        elt->getInputLineNumber ());
        
    // register time in staff
    if (fCurrentTimeStaffNumber == 0)
      fCurrentPart->
        appendTimeToPart (fCurrentTime);
  }
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::visitStart ( S_divisions& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_divisions" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  fCurrentDivisionsPerQuarterNote = (int)(*elt);
  
  if (fCurrentDivisionsPerQuarterNote <= 0) {
    msrMusicXMLError (
      elt->getInputLineNumber (),
      "divisions per quarter note should be positive");
  }

  // set current part's divisions per quarter note
  if (gGeneralOptions->fTraceDivisions) {
    cerr <<
      idtr;
      
    if (fCurrentDivisionsPerQuarterNote == 1)
      cerr <<
        "There is 1 division";
    else
      cerr <<
        "There are " <<
        fCurrentDivisionsPerQuarterNote <<
        " divisions";
    cerr <<
      " per quarter note in part " <<
      fCurrentPart->getPartCombinedName() <<
      ", line " << inputLineNumber <<
      endl;
  }

  // create current divisions
  fCurrentDivisions =
    msrDivisions::create (
      inputLineNumber,
      fCurrentDivisionsPerQuarterNote);

  // append it to the current part
  fCurrentPart->
    setPartCurrentDivisions (
      fCurrentDivisions);
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::visitStart ( S_clef& elt )
{ 
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_sign& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_sign" <<
      endl;

  fCurrentClefSign = elt->getValue();
}

void mxmltree2MsrTranslator::visitStart ( S_line& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_line" <<
      endl;

  fCurrentClefLine = (int)(*elt);
}
  
void mxmltree2MsrTranslator::visitStart ( S_clef_octave_change& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
  
void mxmltree2MsrTranslator::visitEnd ( S_clef& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

    else if ( fCurrentClefLine == 3 ) {
      clefKind = msrClef::kVarbaritoneClef;
      /* JMI
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
      */
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
    
    switch (fCurrentClefLine) {
      case 1:
        clefKind = msrClef::kSopranoClef;
        break;
      case 2:
        clefKind = msrClef::kMezzoSopranoClef;
        break;
      case 3:
        clefKind = msrClef::kAltoClef;
        break;
      case 4:
        clefKind = msrClef::kTenorClef;
        break;
      case 5:
        clefKind = msrClef::kBaritoneClef;
        break;
      default:
        {
          stringstream s;
          
          s <<
            "'C' clef line \"" << fCurrentClefLine <<
            "\" is unknown";
          
          msrMusicXMLError (
            elt->getInputLineNumber (),
            s.str());    
        }
    } // switch
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

  // register clef in part or staff
  if (fCurrentClefStaffNumber == 0)
    fCurrentPart->
      appendClefToPart (clef);
    
  else {
    S_msrStaff
      staff =
        createStaffInCurrentPartIfNotYetDone (
          inputLineNumber,
          fCurrentClefStaffNumber);
    
    staff->
      appendClefToStaff (clef);
  }
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::visitStart ( S_key& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_clef" <<
      endl;

  // The optional number attribute refers to staff numbers.
  // If absent (0), apply to all part staves.
  fCurrentKeyStaffNumber =
    elt->getAttributeIntValue ("number", 0);

  fCurrentKeyKind = msrKey::kTraditionalKind;
  
  // traditional  
  fCurrentKeyFifths       = 0;
  fCurrentKeyCancelFifths = 0;
  
  fCurrentKeyModeKind = msrKey::kMajorMode;

  // Humdrum-Scot

  fCurrentHumdrumScotKeyItem = 0;
}
  
void mxmltree2MsrTranslator::visitStart ( S_cancel& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_cancel" <<
      endl;

  fCurrentKeyCancelFifths = (int)(*elt);
}

void mxmltree2MsrTranslator::visitStart ( S_fifths& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_fifths" <<
      endl;

  fCurrentKeyKind = msrKey::kTraditionalKind;

  fCurrentKeyFifths = (int)(*elt);
}

void mxmltree2MsrTranslator::visitStart ( S_mode& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
  else  if (mode == "ionian") {
    fCurrentKeyModeKind = msrKey::kIonianMode;
  }
  else  if (mode == "dorian") {
    fCurrentKeyModeKind = msrKey::kDorianMode;
  }
  else  if (mode == "phrygian") {
    fCurrentKeyModeKind = msrKey::kPhrygianMode;
  }
  else  if (mode == "lydian") {
    fCurrentKeyModeKind = msrKey::kLydianMode;
  }
  else  if (mode == "mixolydian") {
    fCurrentKeyModeKind = msrKey::kMixolydianMode;
  }
  else  if (mode == "aeolian") {
    fCurrentKeyModeKind = msrKey::kAeolianMode;
  }
  else  if (mode == "locrian") {
    fCurrentKeyModeKind = msrKey::kLocrianMode;
  }
  else {
    stringstream s;
    
    s <<
      "mode " << mode << " is unknown";
    
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }
}

/*
        <key>
          <key-step>C</key-step>
          <key-alter>-2</key-alter>
          <key-step>G</key-step>
          <key-alter>2</key-alter>
          <key-step>D</key-step>
          <key-alter>-1</key-alter>
          <key-step>B</key-step>
          <key-alter>1</key-alter>
          <key-step>F</key-step>
          <key-alter>0</key-alter>
          <key-octave number="1">2</key-octave>
          <key-octave number="2">3</key-octave>
          <key-octave number="3">4</key-octave>
          <key-octave number="4">5</key-octave>
          <key-octave number="5">6</key-octave>
        </key>
*/

void mxmltree2MsrTranslator::visitStart ( S_key_step& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_key_step" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (fCurrentHumdrumScotKeyItem) {
    msrMusicXMLError (
      inputLineNumber,
      "Humdrum/Scot key step found while another one is being handled");
  }
  
  fCurrentKeyKind = msrKey::kHumdrumScotKind;

  string step = elt->getValue();

  // check the step value
  checkStep (
    inputLineNumber,
    step);

  // determine diatonic pitch
  msrDiatonicPitch keyDiatonicPitch =
    msrDiatonicPitchFromString (step [0]);

  // create the Humdrum/Scot item
  fCurrentHumdrumScotKeyItem =
    msrHumdrumScotKeyItem::create (
      inputLineNumber);

  // populate it with the diatonic pitch
  fCurrentHumdrumScotKeyItem->
    setKeyItemDiatonicPitch (
      keyDiatonicPitch);

  // insert it into the items vector
  fCurrentHumdrumScotKeyItemsVector.
    insert (
      fCurrentHumdrumScotKeyItemsVector.end(),
      fCurrentHumdrumScotKeyItem);
}

void mxmltree2MsrTranslator::visitStart ( S_key_alter& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_key_alter" <<
      endl;

  if (! fCurrentHumdrumScotKeyItem) {
    msrMusicXMLError (
      elt->getInputLineNumber (),
      "Humdrum/Scot key alter found while no key step is being handled");
  }
  
  float alter = (float)(*elt);

  // determine the alteration
  msrAlteration keyAlteration =
    msrAlterationFromMusicXMLAlter (
      alter);
      
  if (keyAlteration == k_NoAlteration) {
    stringstream s;

    s <<
      "alter '" << alter << "'"
      "' should be -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5 or +2";
      
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }

  // complement the current Humdrum/Scot item with the alteration
  fCurrentHumdrumScotKeyItem->
    setKeyItemAlteration (
      keyAlteration);

  // forget about this item
  fCurrentHumdrumScotKeyItem = 0;
}

void mxmltree2MsrTranslator::visitStart ( S_key_octave& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_key_octave" <<
      endl;

  int keyOctave = (int)(*elt);

  int number = elt->getAttributeIntValue ("number", 0);

/* JMI
If the cancel attribute is
  set to yes, then this number refers to an element specified
  by the cancel element. It is no by default.
*/

  // fetch Humdrum/Scot item with 'number' in the vector
  S_msrHumdrumScotKeyItem item;
  
  try {
    // indices start at 0
    item = fCurrentHumdrumScotKeyItemsVector [number - 1];
  }
  catch (int e) {
    stringstream s;
    
    s <<
      "item " << number <<
      " not found in Humdrum/Scot key items" <<
      endl <<
      "An exception number " << e << " occurred";

    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }

  if (! item) { // JMI

    cerr << "----------------" << endl;
    int counter = 0;
    for (
      vector<S_msrHumdrumScotKeyItem>::const_iterator i=
        fCurrentHumdrumScotKeyItemsVector.begin();
      i!=fCurrentHumdrumScotKeyItemsVector.end();
      i++) {
      cerr <<
        ++counter << ": " << (*i) <<
        endl;
    } // for
    cerr << "----------------" << endl;
      
    stringstream s;
    
    s <<
      "item " << number <<
      " not found in Humdrum/Scot key items";

    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }
  
  // complement the item with the octave
  item->
    setKeyItemOctave (
      keyOctave);
}

void mxmltree2MsrTranslator::visitEnd ( S_key& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> End visiting S_key" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();

  // handle key
  S_msrKey key;

  switch (fCurrentKeyKind) {
    case msrKey::kTraditionalKind:
      key =
        handleTraditionalKey (inputLineNumber);
      break;
      
    case msrKey::kHumdrumScotKind:
      key =
        handleHumdrumScotKey (inputLineNumber);    
      break;
  } // switch

  // register key in part or staff
  if (fCurrentKeyStaffNumber == 0)
    fCurrentPart->
      appendKeyToPart (key);
    
  else {
    S_msrStaff
      staff =
        createStaffInCurrentPartIfNotYetDone (
          inputLineNumber, fCurrentKeyStaffNumber);

    staff->
      appendKeyToStaff (key);
  }
}

S_msrKey mxmltree2MsrTranslator::handleTraditionalKey (
  int inputLineNumber)
{
  // key fifths number
  msrQuarterTonesPitch keyTonicPitch;
  
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
      
    case 8: // JMI
      keyTonicPitch = k_gSharp;
      break;
    case 9:
      keyTonicPitch = k_dSharp;
      break;
    case 10:
      keyTonicPitch = k_aSharp;
      break;
    case 11:
      keyTonicPitch = k_eSharp;
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

    case -8: // JMI
      keyTonicPitch = k_fFlat;
      break;
    case -9:
      keyTonicPitch = k_bDoubleFlat;
      break;
    case -10:
      keyTonicPitch = k_eDoubleFlat;
      break;
    case -11:
      keyTonicPitch = k_aDoubleFlat;
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

  // create the key
  S_msrKey
    key =
      msrKey::createTraditional (
        inputLineNumber,
        keyTonicPitch, fCurrentKeyModeKind,
        fCurrentKeyCancelFifths);

  // return it
  return key;
}

S_msrKey mxmltree2MsrTranslator::handleHumdrumScotKey (
  int inputLineNumber)
{
 //  msrQuarterTonesPitch fCurrentNoteQuarterTonesPitch; // JMI BOF

  fCurrentNoteQuarterTonesPitch =
    quarterTonesPitchFromDiatonicPitchAndAlteration (
      inputLineNumber,
      fCurrentNoteDiatonicPitch,
      fCurrentNoteAlteration);

  // create the key
  S_msrKey
    key =
      msrKey::createHumdrumScot (
        inputLineNumber);

  // populate the key with the Humdrum/Scot items  
  if (fCurrentHumdrumScotKeyItemsVector.size ()) {
    for (
      vector<S_msrHumdrumScotKeyItem>::const_iterator i=
        fCurrentHumdrumScotKeyItemsVector.begin();
      i!=fCurrentHumdrumScotKeyItemsVector.end();
      i++) {
      key->
        appendHumdrumScotKeyItem ((*i));
    } // for

    fCurrentHumdrumScotKeyItemsVector.clear ();
  }
  
  else
    msrMusicXMLError (
      inputLineNumber,
      "Humdrum/Scot key is empty");
  
  // return it
  return key;
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::visitStart ( S_time& elt )
{  
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_time" <<
      endl;

  /* JMI
  The time-symbol entity indicates how to display a time
  signature.
  
  The normal value is the usual fractional display,
  and is the implied symbol type if none is specified.
  
  Other options are the common and cut time symbols,
  
  as well as a
  single number with an implied denominator.
  
  The note symbol indicates that the beat-type should be represented with
  the corresponding downstem note rather than a number.
  
  The dotted-note symbol indicates that the beat-type should be
  represented with a dotted downstem note that corresponds to
  three times the beat-type value, and a numerator that is
  one third the beats value.


Common time, also known as  time, is a meter with four quarter-note beats per measure. It’s often symbolized by the common-time symbol: C.

Cut time, also known as  or alla breve, is a meter with two half-note beats per measure. It’s often symbolized by the cut-time symbol: C barre
  
        <time>
          <beats>3</beats>
          <beat-type>4</beat-type>
          <interchangeable>
            <time-relation>parentheses</time-relation>
            <beats>6</beats>
            <beat-type>8</beat-type>
          </interchangeable>
        </time>
  */
  
  fCurrentTimeStaffNumber =
    elt->getAttributeIntValue ("number", 0);
    
  string timeSymbol =
    elt->getAttributeValue ("symbol");

  fCurrentTimeSymbolKind =
    msrTime::k_NoTimeSymbol; // default value
  
  if       (timeSymbol == "common") {
    fCurrentTimeSymbolKind = msrTime::kTimeSymbolCommon;
  }
  else  if (timeSymbol == "cut") {
    fCurrentTimeSymbolKind = msrTime::kTimeSymbolCut;
  }
  else  if (timeSymbol == "note") {
    fCurrentTimeSymbolKind = msrTime::kTimeSymbolNote;
  }
  else  if (timeSymbol == "dotted-note") {
    fCurrentTimeSymbolKind = msrTime::kTimeSymbolDottedNote;
  }
  else  if (timeSymbol == "single-number") {
    fCurrentTimeSymbolKind = msrTime::kTimeSymbolSingleNumber;
  }
  // \numericTimeSignature par default si pas de symbol
  
  else {
    if (timeSymbol.size ()) {
      stringstream s;
      
      s <<
        "time symbol " << timeSymbol << " is unknown";
      
      msrMusicXMLError (
        elt->getInputLineNumber (),
        s.str());
    }
  }

  fCurrentTimeBeats = "";
  
  fOnGoingInterchangeable = false;
}

void mxmltree2MsrTranslator::visitStart ( S_beats& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_beats" <<
      endl;

  fCurrentTimeBeats = elt->getValue (); // can be a string such as 3+2
}
  
void mxmltree2MsrTranslator::visitStart ( S_beat_type& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_beat_type" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  int beatType = (int)(*elt);

  // extract the numbers list from the beat type
  std::list<int>
    beatNumbers =
      extractNumbersFromString (
        fCurrentTimeBeats, // may contain "1+5+3"
        false); // 'true' to debug it

  // create the time item
  S_msrTimeItem
    timeItem =
      msrTimeItem::create (
        inputLineNumber);
  
  // populate it
  if (beatNumbers.size ()) {
    // append the beats numbers to the time item
    for (
      std::list<int>::const_iterator i = beatNumbers.begin();
      i != beatNumbers.end();
      i++) {
      timeItem->
        appendBeatsNumber ((*i));
    } // for

    // set the time item beat type
    timeItem->
      setTimeBeatValue (beatType);
  }

  else {
    msrMusicXMLError (
      inputLineNumber,
      "beat type doesn't contain any beats numbers");
  }

  // append the time item to the current time items vector
  fCurrentTimeItemsVector.insert (
    fCurrentTimeItemsVector.end(),
    timeItem);
}
 
void mxmltree2MsrTranslator::visitStart ( S_senza_misura& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_senza_misura" <<
      endl;

  fCurrentTimeSymbolKind = msrTime::kTimeSymbolSenzaMisura;
}

void mxmltree2MsrTranslator::visitStart ( S_interchangeable& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_interchangeable" <<
      endl;

  string interchangeableSymbol =
    elt->getAttributeValue ("symbol");

  fCurrentInterchangeableSymbolKind =
    msrTime::k_NoTimeSymbol; // default value
  
  if       (interchangeableSymbol == "common") {
    fCurrentInterchangeableSymbolKind = msrTime::kTimeSymbolCommon;
  }
  else  if (interchangeableSymbol == "cut") {
    fCurrentInterchangeableSymbolKind = msrTime::kTimeSymbolCut;
  }
  else  if (interchangeableSymbol == "note") {
    fCurrentInterchangeableSymbolKind = msrTime::kTimeSymbolNote;
  }
  else  if (interchangeableSymbol == "dotted-note") {
    fCurrentInterchangeableSymbolKind = msrTime::kTimeSymbolDottedNote;
  }
  else  if (interchangeableSymbol == "single-number") {
    fCurrentInterchangeableSymbolKind = msrTime::kTimeSymbolSingleNumber;
  }
  
  else {
    if (interchangeableSymbol.size ()) {
      stringstream s;
      
      s <<
        "interchangeable symbol " << interchangeableSymbol << " is unknown";
      
      msrMusicXMLError (
        elt->getInputLineNumber (),
        s.str());
    }
  }

  string interchangeableSeparator =
    elt->getAttributeValue ("separator");

  fCurrentInterchangeableSeparatorKind =
    msrTime::k_NoTimeSeparator; // default value
  
  if       (interchangeableSymbol == "none") {
    fCurrentInterchangeableSeparatorKind = msrTime::k_NoTimeSeparator;
  }
  else  if (interchangeableSymbol == "horizontal") {
    fCurrentInterchangeableSeparatorKind = msrTime::kTimeSeparatorHorizontal;
  }
  else  if (interchangeableSymbol == "diagonal") {
    fCurrentInterchangeableSeparatorKind = msrTime::kTimeSeparatorDiagonal;
  }
  else  if (interchangeableSymbol == "vertical") {
    fCurrentInterchangeableSeparatorKind = msrTime::kTimeSeparatorVertical;
  }
  else  if (interchangeableSymbol == "adjacent") {
    fCurrentInterchangeableSeparatorKind = msrTime::kTimeSeparatorAdjacent;
  }
  
  else {
    if (interchangeableSymbol.size ()) {
      stringstream s;
      
      s <<
        "interchangeable symbol " << interchangeableSymbol << " is unknown";
      
      msrMusicXMLError (
        elt->getInputLineNumber (),
        s.str());
    }
  }

  fOnGoingInterchangeable = true;
}

void mxmltree2MsrTranslator::visitStart ( S_time_relation& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_time_relation" <<
      endl;

 //             <time-relation>parentheses</time-relation>

  string timeRelation = elt->getValue ();
  
  fCurrentInterchangeableRelationKind =
    msrTime::k_NoTimeRelation; // default value
  
  if       (timeRelation == "parentheses") {
    fCurrentInterchangeableRelationKind = msrTime::kTimeRelationParentheses;
  }
  else  if (timeRelation == "bracket") {
    fCurrentInterchangeableRelationKind = msrTime::kTimeRelationBracket;
  }
  else  if (timeRelation == "equals") {
    fCurrentInterchangeableRelationKind = msrTime::kTimeRelationEquals;
  }
  else  if (timeRelation == "slash") {
    fCurrentInterchangeableRelationKind = msrTime::kTimeRelationSlash;
  }
  else  if (timeRelation == "space") {
    fCurrentInterchangeableRelationKind = msrTime::kTimeRelationSpace;
  }
  else  if (timeRelation == "hyphen") {
    fCurrentInterchangeableRelationKind = msrTime::kTimeRelationHyphen;
  }
  
  else {
    if (timeRelation.size ()) {
      stringstream s;
      
      s <<
        "interchangeable symbol " << timeRelation << " is unknown";
      
      msrMusicXMLError (
        elt->getInputLineNumber (),
        s.str());
    }
  }
}

void mxmltree2MsrTranslator::visitEnd ( S_time& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> End visiting S_time" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();

  // create the time
  fCurrentTime =
    msrTime::create (
      inputLineNumber,
      fCurrentTimeSymbolKind);
        
  // populate the time with the time items  
  if (fCurrentTimeItemsVector.size ()) {
    for (
      vector<S_msrTimeItem>::const_iterator i =
        fCurrentTimeItemsVector.begin();
      i!=fCurrentTimeItemsVector.end();
      i++) {
      fCurrentTime->
        appendTimeItem ((*i));
    } // for

    fCurrentTimeItemsVector.clear ();
  }
  
  else {
    // only a 'semza misura' time may be empty
    if (  fCurrentTimeSymbolKind != msrTime::kTimeSymbolSenzaMisura) {
      msrMusicXMLError (
        inputLineNumber,
        "time is empty");
    }
  }

  // register time in part or staff
  if (fCurrentTimeStaffNumber == 0)
    fCurrentPart->
      appendTimeToPart (fCurrentTime);
    
  else {
    S_msrStaff
      staff =
        createStaffInCurrentPartIfNotYetDone (
          inputLineNumber, fCurrentTimeStaffNumber);

    staff->
      appendTimeToStaff (fCurrentTime);
  }
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::visitStart ( S_instruments& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_time" <<
      endl;

/*
        <instruments>2</instruments>
*/

  int instruments = (int)(*elt); // JMI
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::visitStart ( S_transpose& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_diatonic& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_diatonic" <<
      endl;

  fCurrentTransposeDiatonic = (int)(*elt);
}
  
void mxmltree2MsrTranslator::visitStart ( S_chromatic& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_chromatic" <<
      endl;

  fCurrentTransposeChromatic = (int)(*elt);
}
 
void mxmltree2MsrTranslator::visitEnd ( S_transpose& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      appendTransposeToPart (transpose);
    
  else {
    S_msrStaff
      staff =
        createStaffInCurrentPartIfNotYetDone (
          inputLineNumber, fCurrentTransposeNumber);

    staff->
      appendTransposeToStaff (transpose);
  }
}

//________________________________________________________________________
void mxmltree2MsrTranslator::visitStart (S_direction& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
    fCurrentWordsPlacementKind = msrWords::kWordsPlacementAbove;
    
  else if (fCurrentDirectionPlacement == "below")
    fCurrentWordsPlacementKind = msrWords::kWordsPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitStart (S_direction_type& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_direction_type" <<
      endl;

  fOnGoingDirectionType = true;
}

void mxmltree2MsrTranslator::visitStart (S_offset& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart (S_octave_shift& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      createVoiceInStaffInCurrentPartIfNotYetDone (
        elt->getInputLineNumber (),
        fCurrentDirectionStaffNumber,
        fCurrentVoiceNumber);

  // append octave shift to the current voice
  currentVoice->
    appendOctaveShiftToVoice (octaveShift);
}

void mxmltree2MsrTranslator::visitStart (S_words& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
  else if (wordsXMLLang == "ja")
    wordsXMLLangKind = msrWords::kJaLang;
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

void mxmltree2MsrTranslator::visitStart ( S_accordion_registration& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_accordion_registration" <<
      endl;

/*
      <direction>
        <direction-type>
          <accordion-registration>
              <accordion-high/>
              <accordion-middle>5</accordion-middle>
              <accordion-low/>
          </accordion-registration>
        </direction-type>
      </direction>
*/

  fCurrentAccordionHigh   = 0;
  fCurrentAccordionMiddle = 0;
  fCurrentAccordionLow    = 0;

  fCurrentAccordionNumbersCounter = 0;
}

void mxmltree2MsrTranslator::visitStart ( S_accordion_high& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_accordion_high" <<
      endl;

  fCurrentAccordionHigh = 1;

  fCurrentAccordionNumbersCounter++;
}

void mxmltree2MsrTranslator::visitStart ( S_accordion_middle& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_accordion_middle" <<
      endl;

  fCurrentAccordionMiddle = (int)(*elt);

  if (fCurrentAccordionMiddle < 0 || fCurrentAccordionMiddle > 3) {
    stringstream s;
    
    s <<
      "accordion middle " <<
      fCurrentAccordionMiddle << " should be 0, 1, 2 or 3" <<
      ", replaced by 0";
    
    msrMusicXMLWarning (
      elt->getInputLineNumber (),
      s.str());

    fCurrentAccordionMiddle = 0;
  }

  fCurrentAccordionNumbersCounter++;
}

void mxmltree2MsrTranslator::visitStart ( S_accordion_low& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_accordion_low" <<
      endl;

  fCurrentAccordionLow = 1;

  fCurrentAccordionNumbersCounter++;
}

void mxmltree2MsrTranslator::visitEnd ( S_accordion_registration& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> End visiting S_accordion_registration" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  // An accordion-registration element needs to have 
  // at least one of the child elements present

  if (fCurrentAccordionNumbersCounter == 0) {
    msrMusicXMLWarning (
      inputLineNumber,
      "accordion-registration has 0 child element, ignoring it");
  }

  else {
    // create the accordion registration
    S_msrAccordionRegistration
      accordionRegistration =
        msrAccordionRegistration::create (
          inputLineNumber,
          fCurrentAccordionHigh,
          fCurrentAccordionMiddle,
          fCurrentAccordionLow);
  
    // append it to the current part
    fCurrentPart->
      appendAccordionRegistrationToPart (
        accordionRegistration);
  }
}

void mxmltree2MsrTranslator::visitEnd (S_direction_type& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> End visiting S_direction_type" <<
      endl;

  fOnGoingDirectionType = false;
}

//________________________________________________________________________
void mxmltree2MsrTranslator::visitStart ( S_metronome& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
  
void mxmltree2MsrTranslator::visitEnd ( S_metronome& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
    fCurrentMetronomeBeatsData.push_back(
      fCurrentMetronomeBeat);
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

  msrBeatData b = fCurrentMetronomeBeatsData [0];
  
  rational r = 
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
      createVoiceInStaffInCurrentPartIfNotYetDone (
        inputLineNumber,
        fCurrentDirectionStaffNumber,
        fCurrentVoiceNumber);

  currentVoice->
    appendTempoToVoice (fCurrentMetronomeTempo);
  
  // JMI if (fCurrentOffset) addDelayed(cmd, fCurrentOffset);
}

void mxmltree2MsrTranslator::visitStart ( S_beat_unit& elt )
{ 
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_beat_unit" <<
      endl;

  if (fCurrentMetronomeBeat.fBeatUnit.size()) {
    fCurrentMetronomeBeatsData.push_back (
      fCurrentMetronomeBeat); 
    fCurrentMetronomeBeat.fBeatUnit = "";
    fCurrentMetronomeBeat.fDots = 0;
  }
  
  fCurrentMetronomeBeat.fBeatUnit = elt->getValue();
}

void mxmltree2MsrTranslator::visitStart ( S_beat_unit_dot& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_beat_unit_dot" <<
      endl;

  fCurrentMetronomeBeat.fDots++;
}
  
void mxmltree2MsrTranslator::visitStart ( S_per_minute& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_per_minute" <<
      endl;

  fCurrentMetrenomePerMinute = (int)(*elt);
}

void mxmltree2MsrTranslator::visitEnd (S_direction& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
void mxmltree2MsrTranslator::visitStart (S_staves& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
void mxmltree2MsrTranslator::visitStart (S_staff& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      createStaffInCurrentPartIfNotYetDone (
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
void mxmltree2MsrTranslator::visitStart (S_staff_details& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_staff_details" <<
      endl;

  fStaffDetailsStaffNumber =
    elt->getAttributeIntValue ("number", 0);

  string showFrets = elt->getAttributeValue ("show-frets");

  fCurrentShowFretsKind =
    msrStaffDetails::kShowFretsNumbers; // default value

  if      (showFrets == "numbers") {
    
    fCurrentShowFretsKind =
      msrStaffDetails::kShowFretsNumbers;
    
  }
  else if (showFrets == "letters") {
    
    fCurrentShowFretsKind =
      msrStaffDetails::kShowFretsLetters;
    
  }
  else {
    if (showFrets.size ()) {
      stringstream s;
      
      s << "show frets " << showFrets << " unknown";
      
      msrMusicXMLError (
        elt->getInputLineNumber (),
        s.str());
    }
  }

  if (gGeneralOptions->fTraceStaves) {
    cerr <<
      idtr <<
        "Handling staff details:" <<
        endl <<
      idtr <<
        tab << "StaffDetailsStaffNumber" << " = " <<
        fStaffDetailsStaffNumber <<
        endl;
  }
  
  fCurrentStaffTypeKind =
    msrStaffDetails::kRegularStaffType;

  fCurrentPrintObjectKind =
    msrStaffDetails::kPrintObjectYes; // default value

  fCurrentPrintSpacingKind =
    msrStaffDetails::kPrintSpacingNo; // default value ??? JMI

  fCurrentStaffTuningAlteration = k_NoAlteration;
  fCurrentStaffTuningOctave     = -1;

  fCurrentStaffDetailsStaffSize = 0;
  
  fCurrentStaffLinesNumber = 0;
  fCurrentStaffTuning = 0;
  
  fCurrentStaffDetailsCapo = 0;

  idtr++;
}

void mxmltree2MsrTranslator::visitStart (S_staff_type& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_staff_type" <<
      endl;

  string staffType = elt->getValue ();
  
  if      (staffType == "ossia") {
    
    fCurrentStaffTypeKind =
      msrStaffDetails::kOssiaStaffType;
    
  }
  else if (staffType == "cue") {
    
    fCurrentStaffTypeKind =
      msrStaffDetails::kCueStaffType;
    
  }
  else if (staffType == "editorial") {
    
    fCurrentStaffTypeKind =
      msrStaffDetails::kEditorialStaffType;
    
  }
  else if (staffType == "regular") {
    
    fCurrentStaffTypeKind =
      msrStaffDetails::kRegularStaffType;
    
  }
  else if (staffType == "alternate") {
    
    fCurrentStaffTypeKind =
      msrStaffDetails::kAlternateStaffType;
    
  }
  else {
    stringstream s;
    
    s << "staff type" << staffType << "unknown";
    
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());      
  }
}

void mxmltree2MsrTranslator::visitStart (S_staff_lines& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_staff_lines" <<
      endl;

  int staffLines = (int)(*elt);

// JMI           <staff-lines>0</staff-lines> cache la portee

  fCurrentStaffLinesNumber =
    msrStaffLinesNumber::create (
      elt->getInputLineNumber (),
      staffLines);
}

void mxmltree2MsrTranslator::visitStart (S_staff_tuning& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_staff_tuning" <<
      endl;

  fCurrentStaffTuningLine =
    elt->getAttributeIntValue ("line", 0);

  fCurrentStaffTuningAlteration = kNatural; // may be absent
  fCurrentStaffTuningOctave     = -1;
}
    
void mxmltree2MsrTranslator::visitStart (S_tuning_step& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart (S_tuning_octave& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_tuning_octave" <<
      endl;

  fCurrentStaffTuningOctave = (int)(*elt);
}

void mxmltree2MsrTranslator::visitStart (S_tuning_alter& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitEnd (S_staff_tuning& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> End visiting S_staff_tuning" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  // fetch relevant staff
  S_msrStaff
    staff =
      createStaffInCurrentPartIfNotYetDone (
        inputLineNumber,
        fStaffDetailsStaffNumber); // test its value??? JMI

  msrQuarterTonesPitch
    quarterTonesPitch =
      quarterTonesPitchFromDiatonicPitchAndAlteration (
        inputLineNumber,
        fCurrentStaffTuningDiatonicPitch,
        fCurrentStaffTuningAlteration);

  // create the staff tuning
  if (gGeneralOptions->fTraceStaffTuning) {
    cerr <<
      idtr <<
        "Creating staff tuning:" <<
        endl;

    idtr++;

    const int fieldWidth = 32;

    cerr <<
      idtr <<
        setw(fieldWidth) <<
        "fCurrentStaffTuningLine" << " = " <<
        fCurrentStaffTuningLine <<
        endl <<
      idtr <<
        setw(fieldWidth) <<
        "fCurrentStaffTuningDiatonicPitch" << " = " <<
        msrDiatonicPitchAsString (
          gMsrOptions->fMsrQuarterTonesPitchesLanguage,
          fCurrentStaffTuningDiatonicPitch) <<
        endl <<
      idtr <<
        setw(fieldWidth) <<
        "fCurrentStaffTuningAlteration" << " = " <<
        msrAlterationAsString (
          fCurrentStaffTuningAlteration) <<
        endl <<
      idtr <<
        setw(fieldWidth) <<
        "quarterTonesPitch" << " = " <<
        msrQuarterTonesPitchAsString (
          gMsrOptions->fMsrQuarterTonesPitchesLanguage,
          quarterTonesPitch) <<
        endl <<
      idtr <<
        setw(fieldWidth) <<
        "CurrentStaffTuningOctave" << " = " <<
        fCurrentStaffTuningOctave <<
        endl;

    idtr--;
  }
    
  fCurrentStaffTuning =
    msrStaffTuning::create (
      inputLineNumber,
      fCurrentStaffTuningLine,
      quarterTonesPitch,
      fCurrentStaffTuningOctave);
}
    
void mxmltree2MsrTranslator::visitStart (S_capo& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_capo" <<
      endl;

  fCurrentStaffDetailsCapo = (int)(*elt);
}

void mxmltree2MsrTranslator::visitStart (S_staff_size& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_staff_size" <<
      endl;

  fCurrentStaffDetailsStaffSize = (int)(*elt);
  // JMI not used
}

void mxmltree2MsrTranslator::visitEnd (S_staff_details& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> End visiting S_staff_details" <<
      endl;

  if (gGeneralOptions->fTraceStaves) {
    const int fieldWidth = 29;

    cerr << left <<
      idtr <<
        setw(fieldWidth) <<
        "CurrentStaffLinesNumber" << " = " <<
        fCurrentStaffLinesNumber->staffLinesNumberAsString () <<
        endl <<
      idtr <<
        setw(fieldWidth) <<
        "StaffDetailsStaffNumber" << " = " <<
        fStaffDetailsStaffNumber <<
        endl <<
      idtr <<
        setw(fieldWidth) <<
        "CurrentStaffDetailsCapo" << " = " <<
        fCurrentStaffDetailsCapo <<
        endl <<
      idtr <<
        setw(fieldWidth) <<
        "CurrentStaffDetailsStaffSize" << " = " <<
        fCurrentStaffDetailsStaffSize <<
        endl;
  }

  idtr--;
  
  // create the staff details
  S_msrStaffDetails
    staffDetails =
      msrStaffDetails::create (
        elt->getInputLineNumber (),
        fCurrentStaffTypeKind,
        fCurrentStaffLinesNumber,
        fCurrentStaffTuning,
        fCurrentShowFretsKind,
        fCurrentPrintObjectKind,
        fCurrentPrintSpacingKind);

  // append staff details in part or staff
  if (fStaffDetailsStaffNumber == 0)
    fCurrentPart->
      appendStaffDetailsToPart (staffDetails);
    
  else {
    S_msrStaff
      staff =
        createStaffInCurrentPartIfNotYetDone (
          elt->getInputLineNumber (),
          fStaffDetailsStaffNumber);
    
    staff->
      appendStaffDetailsToStaff (staffDetails);
  }
}

//________________________________________________________________________
void mxmltree2MsrTranslator::visitStart (S_voice& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
        createStaffInCurrentPartIfNotYetDone (
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
        createStaffInCurrentPartIfNotYetDone (
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
      createVoiceInStaffInCurrentPartIfNotYetDone (
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
void mxmltree2MsrTranslator::visitStart (S_backup& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitEnd (S_backup& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> End visiting S_backup" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTraceMeasures) {
    cerr << idtr <<
      "Handling 'backup <<< " << fCurrentBackupDurationDivisions <<
      " divisions >>>" <<
      ", line " << inputLineNumber <<
      endl;
  }

  fCurrentPart->
    handleBackup (
      inputLineNumber,
      fCurrentBackupDurationDivisions);

  fOnGoingBackup = false;
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::visitStart ( S_forward& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

  int inputLineNumber =
    elt->getInputLineNumber ();

  // the <staff /> element is present only
  // in case of a staff change
  fCurrentForwardStaffNumber = fCurrentStaffNumber;

  // the <voice /> element is present only
  // in case of a voice change
  fCurrentForwardVoiceNumber = fCurrentVoiceNumber;

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
      createStaffInCurrentPartIfNotYetDone (
        inputLineNumber, fCurrentStaffNumber);

  
  // handle the pending tuplets if any
  handleTupletsPendingOnTupletStack (
    elt->getInputLineNumber ());
  
  fOnGoingForward = true;
}

void mxmltree2MsrTranslator::visitEnd ( S_forward& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> End visiting S_forward" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();

  // change staff
  fCurrentStaffNumber = fCurrentForwardStaffNumber;

  S_msrStaff
    staff =
      createStaffInCurrentPartIfNotYetDone (
        inputLineNumber, fCurrentStaffNumber);

  // change voice
  fCurrentVoiceNumber = fCurrentForwardVoiceNumber;

  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNotYetDone (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  if (gGeneralOptions->fTraceMeasures) {
    cerr << idtr <<
      "Handling 'forward >>> " <<
      fCurrentForwardDurationDivisions <<
      "', thus switching to voice \"" <<
      currentVoice->getVoiceName () <<
      "\" in staff \"" << staff->getStaffName () << "\"" <<
      endl;
  }
  
  fCurrentPart->
    handleForward (
      inputLineNumber,
      fCurrentForwardDurationDivisions,
      currentVoice);

  fOnGoingForward = false;
}

//________________________________________________________________________
void mxmltree2MsrTranslator::visitStart (S_tied& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
void mxmltree2MsrTranslator::visitStart (S_slur& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      msrSlur::create (
        elt->getInputLineNumber (),
        fCurrentSlurNumber,
        fCurrentSlurKind);
        
  fPendingSlurs.push_back (slur);
}

//________________________________________________________________________
void mxmltree2MsrTranslator::visitStart (S_bracket& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      msrLigature::create (
        elt->getInputLineNumber (),
        fCurrentLigatureNumber,
        fCurrentLigatureKind);
        
  fPendingLigatures.push_back (ligature);
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::visitStart ( S_wedge& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      msrWedge::create (
        elt->getInputLineNumber (),
        wedgeKind);
        
  fPendingWedges.push_back (wedge);
}
    
//________________________________________________________________________
void mxmltree2MsrTranslator::visitStart (S_lyric& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_syllabic& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_text& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

  if (gGeneralOptions->fTraceLyrics) {
    idtr++;
    
    const int fieldWidth = 20;

    cerr <<
      idtr <<
        setw(fieldWidth) <<
        "line " << " = " << elt->getInputLineNumber () <<
        endl <<
      idtr <<
        "fCurrentStanzaNumber" << " = " << fCurrentStanzaNumber <<
        endl <<
      idtr <<
        "fCurrentSyllabic" << " = " << fCurrentSyllabic <<
        endl <<
      idtr <<
        ", fCurrentLyricText" << " = |" << fCurrentLyricText << "|" <<
      endl;

    idtr--;
  }
}

void mxmltree2MsrTranslator::visitStart ( S_elision& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_elision" <<
      endl;

  fCurrentLyricElision = true;
}

void mxmltree2MsrTranslator::visitStart ( S_extend& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitEnd ( S_lyric& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

      const int fieldwidth = 26;
  
      cerr <<
        idtr <<
          setw(fieldwidth) <<
          "fCurrentStanzaNumber" << " = " << fCurrentStanzaNumber <<
          endl <<
        idtr <<
          setw(fieldwidth) <<
          "fCurrentLyricText" << " = \"" << fCurrentLyricText << "\"" <<
          endl <<
        idtr <<
          setw(fieldwidth) <<
          "fCurrentLyricElision" << " = " << fCurrentLyricElision <<
          endl <<
        idtr <<
          setw(fieldwidth) <<
          "fCurrentSyllableExtendKind" << " = " <<
            fCurrentSyllableExtendKind <<
          endl <<
        idtr <<
          setw(fieldwidth) <<
          "fCurrentNoteIsARest" << " = " <<
          booleanAsString (fCurrentNoteIsARest) <<
          endl <<
        idtr <<
          setw(fieldwidth) <<
          "fCurrentRestMeasure" << " = " <<
          booleanAsString (fCurrentRestMeasure) <<
          endl;
  
      cerr <<
        idtr <<
          setw(fieldwidth) <<
          "fCurrentTieKind" << " = " <<
          msrTie::tieKindAsString (fCurrentTieKind) <<
        endl;
          
      cerr <<
        idtr <<
          setw(fieldwidth) <<
          "fCurrentSlurKind" << " = \"" <<
          msrSlur::slurKindAsString (fCurrentSlurKind) <<
          "\"" <<
          endl;
  
      cerr <<
        idtr <<
          setw(fieldwidth) <<
          "fCurrentLigatureKind" << " = \"" <<
          msrLigature::ligatureKindAsString (fCurrentLigatureKind) <<
          "\"" <<
          endl;
  
      cerr <<
        idtr <<
          setw(fieldwidth) <<
          "fOnGoingSlur" << " = " << fOnGoingSlur <<
          endl <<
        idtr <<
          setw(fieldwidth) <<
          "fOnGoingSlurHasStanza" << " = " << fOnGoingSlurHasStanza <<
          endl;
  
      cerr <<
        idtr <<
          setw(fieldwidth) <<
          "fOnGoingLigature" << " = " << fOnGoingLigature <<
          endl <<
        idtr <<
          setw(fieldwidth) <<
          "fOnGoingLigatureHasStanza" << " = " << fOnGoingLigatureHasStanza <<
          endl;
  
      cerr <<
        idtr <<
          setw(fieldwidth) <<
          "fFirstSyllableInSlurKind" << " = \"" <<
          fFirstSyllableInSlurKind << // JMI->syllableKindAsString () <<
          "\"" <<
      endl;
  
      cerr <<
        idtr <<
          setw(fieldwidth) <<
          "fFirstSyllableInLigatureKind" << " = \"" <<
          fFirstSyllableInLigatureKind << // JMI->syllableKindAsString () <<
          "\"" <<
      endl;
  
      cerr <<
        idtr <<
          setw(fieldwidth) <<
          "fCurrentSyllableKind" << " = \""<<
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
        ", sounding whole notes = " << fCurrentNoteSoundingWholeNotes << 
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
        fCurrentNoteSoundingWholeNotes,
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
void mxmltree2MsrTranslator::visitStart (S_measure& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_measure" <<
      endl;

/*
  Measures have a required number attribute (going from partwise to timewise,
  measures are grouped via the number).
*/

  int inputLineNumber =
    elt->getInputLineNumber ();

  fCurrentMeasureNumber = // JMI local variable???
      elt->getAttributeValue ("number");

  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceGeneral) {
    cerr <<
      endl <<
      idtr << 
        "<!--=== measure " << fCurrentMeasureNumber <<
        ", line " << inputLineNumber << " ===-->" <<
        endl <<
      endl;
  }

  // Measures with an implicit attribute set to "yes"
  // never display a measure number,
  // regardless of the measure-numbering setting.
  string
    implicit =
      elt->getAttributeValue ("implicit");

  msrMeasure::msrMeasureImplicitKind
    measureImplicitKind; // use it JMI
  
  if       (implicit == "yes") {
    measureImplicitKind =
      msrMeasure::kMeasureImplicitYes;
  }
  else  if (implicit == "no") {
    measureImplicitKind =
      msrMeasure::kMeasureImplicitNo;
  }
  else {
    if (implicit.size ()) {
      stringstream s;
      
      s << "implicit " << implicit << " is unknown";
      
      msrMusicXMLError (
        elt->getInputLineNumber (),
        s.str());
    }
  }

  // take this measure into account
  fMeasuresCounter++;
  
  // append a new measure to the current part
  fCurrentPart->
    createMeasureAndAppendItToPart (
      inputLineNumber,
      fCurrentMeasureNumber);

  // is this measure number in the debug set?
  if (gGeneralOptions->fTraceDetailed) {
    set<int>::const_iterator
      it =
        gGeneralOptions->
          fTraceDetailedMeasureNumbersSet.find (fMeasuresCounter);
          
    if (it != gGeneralOptions->fTraceDetailedMeasureNumbersSet.end ()) {
      // yes, activate detailed trace for it
      gMusicXMLOptions = gMusicXMLOptionsWithDetailedTrace;
      gGeneralOptions  = gGeneralOptionsWithDetailedTrace;
      gMsrOptions      = gMsrOptionsWithDetailedTrace;
      gLpsrOptions     = gLpsrOptionsWithDetailedTrace;
      gLilypondOptions = gLilypondOptionsWithDetailedTrace;
    }
  }
}

void mxmltree2MsrTranslator::visitEnd (S_measure& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

  // finalize current measure in the part,
  // to set measures kinds
  // and update silent and harmony voices // ??? JMI
  fCurrentPart->
    finalizeCurrentMeasureInPart (
      inputLineNumber);

  // handle an on going multiple rest if any only now,
  // so that the necessary staves/voices have been created
  if (fOnGoingMultipleRest) {

    if (! fCurrentMultipleRestHasBeenCreated) {
      // create a pending multiple rest,
      // that will be handled when fRemainingMultipleRestMeasuresNumber
      // comes down to 0 later in this same method'
      fCurrentPart->
        createMultipleRestInPart (
          inputLineNumber,
          fCurrentMultipleRestMeasuresNumber);

      fCurrentMultipleRestHasBeenCreated = true;
    }

    else {
      if (fRemainingMultipleRestMeasuresNumber > 0) {
        // account for one more rest measure in the multiple rest
        fRemainingMultipleRestMeasuresNumber--;
    
        if (fRemainingMultipleRestMeasuresNumber == 0) {
          // all rest measures have been met,
          // the current one is the first after  the multiple rest
          fCurrentPart->
            appendPendingMultipleRestToPart (
              inputLineNumber);

        fOnGoingMultipleRest = false;
        }
      }
    }
  }

  // restore debug options in case they were set in visitStart()
  gMusicXMLOptions = gMusicXMLOptionsUserChoices;
  gGeneralOptions  = gGeneralOptionsUserChoices;
  gMsrOptions      = gMsrOptionsUserChoices;
  gLpsrOptions     = gLpsrOptionsUserChoices;
  gLilypondOptions = gLilypondOptionsUserChoices;
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::visitStart ( S_print& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      
      // create a barNumberCheck command
      if (gGeneralOptions->fTraceMeasures) {
        cerr << idtr << 
          "Creating a barnumber check, " <<
          "line = " << inputLineNumber <<
          endl;
      }

      // fetch current voice
      S_msrVoice
        currentVoice =
          createVoiceInStaffInCurrentPartIfNotYetDone (
            inputLineNumber,
            fCurrentStaffNumber,
            fCurrentVoiceNumber);

      S_msrBarNumberCheck
        barNumberCheck_ =
          msrBarNumberCheck::create (
            inputLineNumber,
            currentVoice->
              getVoiceMeasureNumber ());
            
      // append it to the voice
// JMI      S_msrElement bnc = barNumberCheck_;
      currentVoice->
        appendBarNumberCheckToVoice (barNumberCheck_);
  
      // create a break command
      if (gGeneralOptions->fTraceMeasures) {
        cerr << idtr << 
          "Creating a break, " <<
          "line = " << inputLineNumber << endl;
      }

      S_msrBreak
        break_ =
          msrBreak::create (
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
void mxmltree2MsrTranslator::visitStart ( S_barline& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
    msrBarline::kRightLocation; // by default
    
  if       (location == "left") {
    fCurrentBarlineLocation =
      msrBarline::kLeftLocation;
  }
  else  if (location == "middle") {
    fCurrentBarlineLocation =
      msrBarline::kMiddleLocation;
  }
  else if  (location == "right") {
    fCurrentBarlineLocation =
      msrBarline::kRightLocation;
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
void mxmltree2MsrTranslator::visitStart ( S_bar_style& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_bar_style" <<
      endl;

  string barStyle = elt->getValue();

  fCurrentBarlineStyle =
    msrBarline::k_NoStyle;

  if      (barStyle == "regular") {
    fCurrentBarlineStyle =
      msrBarline::kRegularStyle;
  }
  else if (barStyle == "dotted") {
    fCurrentBarlineStyle =
      msrBarline::kDottedStyle;
  }
  else if (barStyle == "dashed") {
    fCurrentBarlineStyle =
      msrBarline::kDashedStyle;
  }
  else if (barStyle == "heavy") {
    fCurrentBarlineStyle =
      msrBarline::kHeavyStyle;
  }
  else if (barStyle == "light-light") {
    fCurrentBarlineStyle =
      msrBarline::kLightLightStyle;
  }
  else if (barStyle == "light-heavy") {
    fCurrentBarlineStyle =
      msrBarline::kLightHeavyStyle;
  }
  else if (barStyle == "heavy-light") {
    fCurrentBarlineStyle =
      msrBarline::kHeavyLightStyle;
  }
  else if (barStyle == "heavy-heavy") {
    fCurrentBarlineStyle =
      msrBarline::kHeavyHeavyStyle;
  }
  else if (barStyle == "tick") {
    fCurrentBarlineStyle =
      msrBarline::kTickStyle;
  }
  else if (barStyle == "short") {
    fCurrentBarlineStyle =
      msrBarline::kShortStyle;
  }
  else if (barStyle == "none") {
    fCurrentBarlineStyle =
      msrBarline::kNoneStyle;
  }
  else {
    msrMusicXMLError (
      elt->getInputLineNumber (),
      "barline style " + barStyle + " is unknown");
  }
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::visitStart ( S_segno& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_segno" <<
      endl;

  if (fOnGoingDirectionType) {
    int inputLineNumber =
      elt->getInputLineNumber ();
      
    // fetch current voice
    S_msrVoice
      currentVoice =
        createVoiceInStaffInCurrentPartIfNotYetDone (
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

void mxmltree2MsrTranslator::visitStart ( S_coda& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_coda" <<
      endl;

  if (fOnGoingDirectionType) {
    int inputLineNumber =
      elt->getInputLineNumber ();
      
    // fetch current voice
    S_msrVoice
      currentVoice =
        createVoiceInStaffInCurrentPartIfNotYetDone (
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

void mxmltree2MsrTranslator::visitStart ( S_eyeglasses& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_eyeglasses" <<
      endl;

  if (fOnGoingDirectionType) {
    int inputLineNumber =
      elt->getInputLineNumber ();
      
    // fetch current voice
    S_msrVoice
      currentVoice =
        createVoiceInStaffInCurrentPartIfNotYetDone (
          inputLineNumber,
          fCurrentStaffNumber,
          fCurrentVoiceNumber);
  
    // create the eyeglasses
    S_msrEyeGlasses
      eyeGlasses =
        msrEyeGlasses::create (
          inputLineNumber);

    // append it to the current voice
    currentVoice->
      appendEyeGlassesToVoice (eyeGlasses);
  }
  
  else if (fOnGoingBarline) {
    fCurrentBarlineHasCoda = true;
  }
}

void mxmltree2MsrTranslator::visitStart ( S_pedal& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
        createVoiceInStaffInCurrentPartIfNotYetDone (
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
void mxmltree2MsrTranslator::visitStart ( S_ending& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_ending" <<
      endl;

  fCurrentBarlineEndingNumber =
    elt->getAttributeValue ("number"); // may be "1, 2"

  string type =
    elt->getAttributeValue ("type");
      
  if       (type == "start") {
    fCurrentBarlineEndingType =
      msrBarline::kStartEndingType;
  }
  else  if (type == "stop") {
    fCurrentBarlineEndingType =
      msrBarline::kStopEndingType;
  }
  else  if (type == "discontinue") {
    fCurrentBarlineEndingType =
      msrBarline::kDiscontinueEndingType;
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
void mxmltree2MsrTranslator::visitStart ( S_repeat& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      msrBarline::kForwardRepeatDirection;
  }
  else  if (direction == "backward") {
    fCurrentBarlineRepeatDirection =
      msrBarline::kBackwardRepeatDirection;
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
        msrBarline::kNoneRepeatWinged;
    }
    else if (winged == "straight") {
      fCurrentBarlineRepeatWinged =
        msrBarline::kStraightRepeatWinged;
    }
    else  if (winged == "curved") {
      fCurrentBarlineRepeatWinged =
        msrBarline::kCurvedRepeatWinged;
    }
    else  if (winged == "doubleStraight") {
      fCurrentBarlineRepeatWinged =
        msrBarline::kDoubleStraightRepeatWinged;
    }
    else  if (winged == "doubleCurved") {
      fCurrentBarlineRepeatWinged =
        msrBarline::kDoubleCurvedRepeatWinged;
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
void mxmltree2MsrTranslator::visitEnd ( S_barline& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> End visiting S_barline" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();

  // there may be a barline in a part before any music
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNotYetDone (
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
    fCurrentBarlineLocation == msrBarline::kLeftLocation
      &&
    fCurrentBarlineEndingType == msrBarline::kStartEndingType
      &&
    fCurrentBarlineRepeatDirection == msrBarline::kForwardRepeatDirection) {
    // hooked ending start
    // ------------------------------------------------------
    handleHookedEndingStart (elt, barline);

    barlineIsAlright = true;
  }

  else if (
    fCurrentBarlineLocation == msrBarline::kLeftLocation
      &&
    fCurrentBarlineRepeatDirection == msrBarline::kForwardRepeatDirection) {
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
    fCurrentBarlineLocation == msrBarline::kLeftLocation
      &&
    fCurrentBarlineEndingType == msrBarline::kStartEndingType) { // no forward
    // hookless ending start
    // ------------------------------------------------------
    handleHooklessEndingStart (elt, barline);

    barlineIsAlright = true;
  }

  else if (
    fCurrentBarlineLocation == msrBarline::kRightLocation
      &&
    fCurrentBarlineEndingType == msrBarline::kStopEndingType
      &&
    fCurrentBarlineRepeatDirection == msrBarline::kBackwardRepeatDirection) {
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
    fCurrentBarlineLocation == msrBarline::kRightLocation
      &&
    fCurrentBarlineRepeatDirection == msrBarline::kBackwardRepeatDirection) {
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
    fCurrentBarlineLocation == msrBarline::kRightLocation
      &&
    fCurrentBarlineEndingType == msrBarline::kDiscontinueEndingType) {
    // hookless ending end
    // ------------------------------------------------------
    handleHooklessEndingEnd (elt, barline);
        
    barlineIsAlright = true;
  }

  else {

    switch (fCurrentBarlineStyle) {
      
      case msrBarline::kRegularStyle:
      //---------------------------------------
        // don't handle regular barlines specifically,
        // they'll handled later by the software
        // that handles the generated text output
  
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBarline);
        
        // append the bar line to the current segment
        currentVoice->
          appendBarlineToVoice (barline);
      
        barlineIsAlright = true;
        break;
        
      case msrBarline::kDottedStyle:
      //---------------------------------------
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBarline);
        
        // append the bar line to the current segment
        currentVoice->
          appendBarlineToVoice (barline);
              
        barlineIsAlright = true;
        break;
        
      case msrBarline::kDashedStyle:
      //---------------------------------------    
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBarline);
        
        // append the bar line to the current segment
        currentVoice->
          appendBarlineToVoice (barline);
      
        barlineIsAlright = true;
        break;
        
      case msrBarline::kHeavyStyle:
      //---------------------------------------    
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBarline);
        
        // append the bar line to the current segment
        currentVoice->
          appendBarlineToVoice (barline);
      
        barlineIsAlright = true;
        break;
        
      case msrBarline::kLightLightStyle:
      //---------------------------------------
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBarline);
  
        // append the bar line to the current segment
        currentVoice->
          appendBarlineToVoice (barline);
    
        barlineIsAlright = true;
        break;
        
      case msrBarline::kLightHeavyStyle:
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
          setBarlineCategory (msrBarline::kStandaloneBarline);
        
        // append the bar line to the current segment
        currentVoice->
          appendBarlineToVoice (barline);
          
        barlineIsAlright = true;        
        break;
  
      case msrBarline::kHeavyLightStyle:
      //---------------------------------------
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBarline);
        
        // append the bar line to the current segment
        currentVoice->
          appendBarlineToVoice (barline);
          
        barlineIsAlright = true;
        break;
         
      case msrBarline::kHeavyHeavyStyle:
      //---------------------------------------    
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBarline);
        
        // append the bar line to the current segment
        currentVoice->
          appendBarlineToVoice (barline);
  
        barlineIsAlright = true;
        break;
        
      case msrBarline::kTickStyle:
      //---------------------------------------
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBarline);
        
        // append the bar line to the current segment
        currentVoice->
          appendBarlineToVoice (barline);
  
        barlineIsAlright = true;
        break;
        
      case msrBarline::kShortStyle:
      //---------------------------------------
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBarline);
        
        // append the bar line to the current segment
        currentVoice->
          appendBarlineToVoice (barline);
  
        barlineIsAlright = true;
        break;
  
      case msrBarline::kNoneStyle:
      //---------------------------------------
        // set the barline category
        barline->
          setBarlineCategory (msrBarline::kStandaloneBarline);
        
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
void mxmltree2MsrTranslator::visitStart ( S_note& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_note" <<
      endl;

//       <note print-object="no"> JMI grise les notes
//           <staff-lines>5</staff-lines> revient a la normale

  // initialize note data to a neutral state
  initializeNoteData ();

  fCurrentNoteDiatonicPitch = k_NoDiatonicPitch;
  fCurrentNoteAlteration    = kNatural;

  fCurrentNoteOctave = K_NO_OCTAVE;

  fCurrentDisplayDiatonicPitch = k_NoDiatonicPitch;
  fCurrentDisplayOctave = K_NO_OCTAVE;
  fCurrentNoteDisplayWholeNotes = rational (0, 1);
  
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
void mxmltree2MsrTranslator::visitStart ( S_step& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_alter& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_octave& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_octave" <<
      endl;

  fCurrentNoteOctave = (int)(*elt);
}

void mxmltree2MsrTranslator::visitStart ( S_duration& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_duration" <<
      endl;

  int duration = (int)(*elt); // divisions

  if      (fOnGoingBackup) {
  
    fCurrentBackupDurationDivisions = duration;

  }
  
  else if (fOnGoingForward) {
  
    fCurrentForwardDurationDivisions = duration;
    
  }
  
  else if (fOnGoingNote) {
  
    fCurrentNoteSoundingWholeNotes =
      rational (
        duration,
        fCurrentDivisionsPerQuarterNote * 4); // hence a whole note

    fCurrentNoteSoundingWholeNotes.rationalise ();
    
  }
  
  else {
    
    stringstream s;
    
    s << "duration " << duration << " is out of context";
    
 // JMI   msrMusicXMLError (s.str());
    msrMusicXMLWarning (
      elt->getInputLineNumber (),
      s.str());
  }
    
//  cerr << "=== mxmltree2MsrTranslator::visitStart ( S_duration& elt ), fCurrentDuration = " << fCurrentDuration << endl; JMI
}

void mxmltree2MsrTranslator::visitStart ( S_instrument& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_instrument" <<
      endl;

/*
  <instrument id="P2-I4"/>
*/
 string id = elt->getAttributeValue ("id"); // JMI
}
       
void mxmltree2MsrTranslator::visitStart ( S_dot& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_dot" <<
      endl;

  fCurrentNoteDotsNumber++;
}
       
void mxmltree2MsrTranslator::visitStart ( S_type& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_accidental& elt ) // JMI
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_accidental" <<
      endl;

/*
  Notated accidental.
  
  Values:
  
    sharp, natural, flat, double-sharp, sharp-sharp, flat-flat, natural-sharp, natural-flat, quarter-flat, quarter-sharp, three-quarters-flat, three-quarters-sharp;
    
    sharp-down, sharp-up, natural-down, natural-up, flat-down, flat-up, triple-sharp, triple-flat, slash-quarter-sharp, slash-sharp, slash-flat, double-slash-flat, sharp-1, sharp-2, sharp-3, sharp-5, flat-1, flat-2, flat-3, flat-4, sori, and koron added in 3.0
*/

  string editorialAccidental = elt->getAttributeValue ("editorial");

  {
    fCurrentNoteEditorialAccidentalKind =
      msrNote::kNoteEditorialAccidentalNo; // default value
        
    if      (editorialAccidental == "yes")
      fCurrentNoteEditorialAccidentalKind =
        msrNote::kNoteEditorialAccidentalYes;
    else if (editorialAccidental == "no")
      fCurrentNoteEditorialAccidentalKind =
        msrNote::kNoteEditorialAccidentalNo;
    else {
      if (editorialAccidental.size ()) {
        stringstream s;
        
        s << "editorial accidental " << editorialAccidental << " is unknown";
        
        msrMusicXMLError (
          elt->getInputLineNumber (),
          s.str());
      }
    }
  }

  {
    string cautionaryAccidental = elt->getAttributeValue ("cautionary");
  
    fCurrentNoteCautionaryAccidentalKind =
      msrNote::kNoteCautionaryAccidentalNo; // default value
        
    if      (cautionaryAccidental == "yes")
      fCurrentNoteCautionaryAccidentalKind =
        msrNote::kNoteCautionaryAccidentalYes;
    else if (cautionaryAccidental == "no")
      fCurrentNoteCautionaryAccidentalKind =
        msrNote::kNoteCautionaryAccidentalNo;
    else {
      if (cautionaryAccidental.size ()) {
        stringstream s;
        
        s << "cautionary accidental " << cautionaryAccidental << " is unknown";
        
        msrMusicXMLError (
          elt->getInputLineNumber (),
          s.str());
      }
    }
  }
}

void mxmltree2MsrTranslator::visitStart ( S_stem& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_beam& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
void mxmltree2MsrTranslator::visitStart ( S_measure_style& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_measure_style" <<
      endl;
}
       
void mxmltree2MsrTranslator::visitStart ( S_beat_repeat& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_beat_repeat" <<
      endl;

  fCurrentBeatRepeatSlashes = elt->getAttributeIntValue ("slashes", 0);

  string beatRepeatUseDots = elt->getAttributeValue ("use-dots");

  if      (beatRepeatUseDots == "yes")
    fCurrentTupletKind = msrTuplet::kStartTuplet; // JMI
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
       
void mxmltree2MsrTranslator::visitStart ( S_measure_repeat& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_measure_repeat" <<
      endl;

  int inputLineNumber = elt->getInputLineNumber ();
  
  fCurrentMeasureRepeatMeasuresNumber = (int)(*elt);

  string measureRepeatType =
    elt->getAttributeValue ("type");

  fCurrentMeasureRepeatSlashesNumber =
    elt->getAttributeIntValue ("slashes", 0);

  fCurrentMeasureRepeatKind = msrMeasureRepeat::k_NoMeasureRepeat;
  
  if      (measureRepeatType == "start") {
    fCurrentMeasureRepeatKind = msrMeasureRepeat::kStartMeasureRepeat; // JMI

    fCurrentPart->
      createMeasureRepeatFromItsFirstMeasureInPart (
        inputLineNumber,
        fCurrentMeasureRepeatMeasuresNumber,
        fCurrentMeasureRepeatSlashesNumber);
  }
  
  else if (measureRepeatType == "stop") {
    fCurrentMeasureRepeatKind = msrMeasureRepeat::kStopMeasureRepeat; // JMI

    fCurrentPart->
      appendPendingMeasureRepeatToPart (
        inputLineNumber);
  }
  
  else {
    stringstream s;
    
    s << "measure repeat type " << measureRepeatType << " is unknown";
    
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }
}
       
void mxmltree2MsrTranslator::visitStart ( S_multiple_rest& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_multiple_rest" <<
      endl;

  int inputLineNumber = elt->getInputLineNumber ();
  
  fCurrentMultipleRestMeasuresNumber = (int)(*elt);

  string multipleRestUseSymbols = elt->getAttributeValue ("use-symbols");

  if      (multipleRestUseSymbols == "yes")
    fCurrentTupletKind = msrTuplet::kStartTuplet; // JMI
  else if (multipleRestUseSymbols == "no")
    fCurrentTupletKind = msrTuplet::kStopTuplet;
  else {
    if (multipleRestUseSymbols.size ()) {
      stringstream s;
      
      s <<
        "multiple rest use symbols " <<
        multipleRestUseSymbols <<
        " is unknown";
      
      msrMusicXMLError (
        inputLineNumber,
        s.str());
      }
  }

  // register number of remeaining rest measures
  fRemainingMultipleRestMeasuresNumber =
    fCurrentMultipleRestMeasuresNumber;

  // the multiple rest will created at the end of its first measure,
  // so that the needed staves/voices have been created
  
  fOnGoingMultipleRest = true;
}
       
void mxmltree2MsrTranslator::visitStart ( S_slash& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_slash" <<
      endl;

  string slashType = elt->getAttributeValue ("type");

  if      (slashType == "start")
    fCurrentTupletKind = msrTuplet::kStartTuplet; // JMI
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
    fCurrentTupletKind = msrTuplet::kStartTuplet; // JMI
  else if (slashUseDots == "no")
    fCurrentTupletKind = msrTuplet::kStopTuplet;
  else {
    if (slashUseDots.size ()) {
      stringstream s;
      
      s << "slash use dots " << slashUseDots << " is unknown";
      
      msrMusicXMLError (
        elt->getInputLineNumber (),
        s.str());
    }
  }

  string slashUseStems = elt->getAttributeValue ("use-stems");

  if      (slashUseStems == "yes")
    fCurrentTupletKind = msrTuplet::kStartTuplet; // JMI
  else if (slashUseStems == "no")
    fCurrentTupletKind = msrTuplet::kStopTuplet;
  else {
    if (slashUseStems.size ()) {
      stringstream s;
      
      s << "slash use stems " << slashUseStems << " is unknown";
      
      msrMusicXMLError (
        elt->getInputLineNumber (),
        s.str());
    }
  }
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::visitStart ( S_articulations& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_articulations" <<
      endl;
}

void mxmltree2MsrTranslator::visitStart ( S_accent& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_breath_mark& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_caesura& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_spiccato& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_staccato& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_staccatissimo& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_stress& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_unstress& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_detached_legato& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_strong_accent& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_tenuto& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_fermata& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_fermata" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
/*
  Fermata and wavy-line elements can be applied both to
  notes and to measures, so they are defined here. Wavy
  lines are one way to indicate trills; when used with a
  measure element, they should always have type="continue"
  set. The fermata text content represents the shape of the
  fermata sign and may be normal, angled, or square.
  An empty fermata element represents a normal fermata.
  The fermata type is upright if not specified.
-->
<!ELEMENT fermata  (#PCDATA)>
<!ATTLIST fermata
    type (upright | inverted) #IMPLIED
    %print-style;
>
<!ELEMENT wavy-line EMPTY>
<!ATTLIST wavy-line
    type %start-stop-continue; #REQUIRED
    number %number-level; #IMPLIED
    %position;
    %placement; 
    %color;
    %trill-sound; 
>
 */

  string fermataTextValue = elt->getValue ();

  msrFermata::msrFermataKind
    fermataKind =
      msrFermata::kNormalFermataKind; // default value

  if      (fermataTextValue == "normal")
    fermataKind = msrFermata::kNormalFermataKind;
    
  else if (fermataTextValue == "angled")
    fermataKind = msrFermata::kAngledFermataKind;
    
  else if (fermataTextValue == "square")
    fermataKind = msrFermata::kSquareFermataKind;
    
  else {
    if (fermataTextValue.size ()) {
      stringstream s;
      
      s <<
        "fermata kind \"" << fermataTextValue <<
        "\" is unknown";
      
      msrMusicXMLError (
        inputLineNumber,
        s.str());
    }   
  }
  
  string fermataTypeValue = elt->getAttributeValue ("type");
  
  msrFermata::msrFermataType
    fermataType =
      msrFermata::kUprightFermataType; // default value

  if      (fermataTypeValue == "upright")
    fermataType = msrFermata::kUprightFermataType;
    
  else if (fermataTypeValue == "inverted")
    fermataType = msrFermata::kInvertedFermataType;
    
  else {
    if (fermataTypeValue.size ()) {
      stringstream s;
      
      s <<
        "fermata type \"" << fermataTypeValue <<
        "\" is unknown";
      
      msrMusicXMLError (
        inputLineNumber,
        s.str());
    }   
  }

/* JMI
  // type : upright inverted  (Binchois20.xml)
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulation::kFermata);
*/

  S_msrFermata
    fermata =
      msrFermata::create (
        inputLineNumber,
        fermataKind,
        fermataType);
        
  fCurrentArticulations.push_back (fermata);
}

void mxmltree2MsrTranslator::visitStart ( S_arpeggiate& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_non_arpeggiate& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_non_arpeggiate" <<
      endl;

/* JMI
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputLineNumber (),
        msrArticulation::kArpeggiato);
      
  fCurrentArticulations.push_back (articulation);
  */
}

void mxmltree2MsrTranslator::visitStart ( S_doit& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_falloff& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_plop& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_scoop& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitEnd ( S_articulations& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> End visiting S_articulations" <<
      endl;

  // JMI
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::visitStart ( S_technical& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_technical" <<
      endl;
}

void mxmltree2MsrTranslator::visitEnd ( S_technical& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_technical" <<
      endl;
}

void mxmltree2MsrTranslator::visitStart ( S_arrow& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_arrow" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  string placement = elt->getAttributeValue ("placement");

  msrTechnical::msrTechnicalPlacementKind
    arrowPlacementKind =
      msrTechnical::k_NoTechnicalPlacement;

  if      (placement == "above")
    arrowPlacementKind = msrTechnical::kTechnicalPlacementAbove;
    
  else if (placement == "below")
    arrowPlacementKind = msrTechnical::kTechnicalPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitStart ( S_bend_alter& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_bend_alter" <<
      endl;

  fBendAlterValue = (int)(*elt);
}
  
void mxmltree2MsrTranslator::visitStart ( S_bend& elt ) // JMI
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_bend" <<
      endl;
}

void mxmltree2MsrTranslator::visitEnd ( S_bend& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_bend" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();

  string placement = elt->getAttributeValue ("placement");

  msrTechnicalWithInteger::msrTechnicalWithIntegerPlacementKind
    bendPlacementKind =
      msrTechnicalWithInteger::k_NoTechnicalWithIntegerPlacement;

  if      (placement == "above")
    bendPlacementKind = msrTechnicalWithInteger::kTechnicalWithIntegerPlacementAbove;
    
  else if (placement == "below")
    bendPlacementKind = msrTechnicalWithInteger::kTechnicalWithIntegerPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitStart ( S_double_tongue& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      msrTechnical::k_NoTechnicalPlacement;

  if      (placement == "above")
    doubleTonguePlacementKind = msrTechnical::kTechnicalPlacementAbove;
    
  else if (placement == "below")
    doubleTonguePlacementKind = msrTechnical::kTechnicalPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitStart ( S_down_bow& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      msrTechnical::k_NoTechnicalPlacement;

  if      (placement == "above")
    downBowPlacementKind = msrTechnical::kTechnicalPlacementAbove;
    
  else if (placement == "below")
    downBowPlacementKind = msrTechnical::kTechnicalPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitStart ( S_fingering& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_fingering" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  int fingeringValue = (int)(*elt);

  string placement = elt->getAttributeValue ("placement");

  msrTechnicalWithInteger::msrTechnicalWithIntegerPlacementKind
    fingeringPlacementKind =
      msrTechnicalWithInteger::k_NoTechnicalWithIntegerPlacement;

  if      (placement == "above")
    fingeringPlacementKind =
      msrTechnicalWithInteger::kTechnicalWithIntegerPlacementAbove;
    
  else if (placement == "below")
    fingeringPlacementKind =
      msrTechnicalWithInteger::kTechnicalWithIntegerPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitStart ( S_fingernails& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      msrTechnical::k_NoTechnicalPlacement;

  if      (placement == "above")
    fingernailsPlacementKind = msrTechnical::kTechnicalPlacementAbove;
    
  else if (placement == "below")
    fingernailsPlacementKind = msrTechnical::kTechnicalPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitStart ( S_fret& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_fret" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();

  int fretValue = (int)(*elt);
    
  string placement = elt->getAttributeValue ("placement");

  msrTechnicalWithInteger::msrTechnicalWithIntegerPlacementKind
    fretPlacementKind =
      msrTechnicalWithInteger::k_NoTechnicalWithIntegerPlacement;

  if      (placement == "above")
    fretPlacementKind =
      msrTechnicalWithInteger::kTechnicalWithIntegerPlacementAbove;
    
  else if (placement == "below")
    fretPlacementKind =
      msrTechnicalWithInteger::kTechnicalWithIntegerPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitStart ( S_hammer_on& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_hammer_on" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();

  string hammerOnValue = elt->getValue ();
    
  string placement = elt->getAttributeValue ("placement");

  msrTechnicalWithString::msrTechnicalWithStringPlacementKind
    hammerOnPlacementKind =
      msrTechnicalWithString::k_NoTechnicalWithStringPlacement;

  if      (placement == "above")
    hammerOnPlacementKind =
      msrTechnicalWithString::kTechnicalWithStringPlacementAbove;
    
  else if (placement == "below")
    hammerOnPlacementKind =
      msrTechnicalWithString::kTechnicalWithStringPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitStart ( S_handbell& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      msrTechnicalWithString::k_NoTechnicalWithStringPlacement;

  if      (placement == "above")
    handbellPlacementKind =
      msrTechnicalWithString::kTechnicalWithStringPlacementAbove;
    
  else if (placement == "below")
    handbellPlacementKind =
      msrTechnicalWithString::kTechnicalWithStringPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitStart ( S_harmonic& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      msrTechnical::k_NoTechnicalPlacement;

  if      (placement == "above")
    harmonicPlacementKind = msrTechnical::kTechnicalPlacementAbove;
    
  else if (placement == "below")
    harmonicPlacementKind = msrTechnical::kTechnicalPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitStart ( S_heel& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      msrTechnical::k_NoTechnicalPlacement;

  if      (placement == "above")
    heelPlacementKind = msrTechnical::kTechnicalPlacementAbove;
    
  else if (placement == "below")
    heelPlacementKind = msrTechnical::kTechnicalPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitStart ( S_hole& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      msrTechnical::k_NoTechnicalPlacement;

  if      (placement == "above")
    holePlacementKind = msrTechnical::kTechnicalPlacementAbove;
    
  else if (placement == "below")
    holePlacementKind = msrTechnical::kTechnicalPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitStart ( S_open_string& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      msrTechnical::k_NoTechnicalPlacement;

  if      (placement == "above")
    openStringPlacementKind = msrTechnical::kTechnicalPlacementAbove;
    
  else if (placement == "below")
    openStringPlacementKind = msrTechnical::kTechnicalPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitStart ( S_other_technical& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      msrTechnicalWithString::k_NoTechnicalWithStringPlacement;

  if      (placement == "above")
    otherTechnicalWithStringPlacementKind =
      msrTechnicalWithString::kTechnicalWithStringPlacementAbove;
    
  else if (placement == "below")
    otherTechnicalWithStringPlacementKind =
      msrTechnicalWithString::kTechnicalWithStringPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitStart ( S_pluck& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      msrTechnicalWithString::k_NoTechnicalWithStringPlacement;

  if      (placement == "above")
    pluckPlacementKind =
      msrTechnicalWithString::kTechnicalWithStringPlacementAbove;
    
  else if (placement == "below")
    pluckPlacementKind =
      msrTechnicalWithString::kTechnicalWithStringPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitStart ( S_pull_off& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      msrTechnicalWithString::k_NoTechnicalWithStringPlacement;

  if      (placement == "above")
    pullOffPlacementKind =
      msrTechnicalWithString::kTechnicalWithStringPlacementAbove;
    
  else if (placement == "below")
    pullOffPlacementKind =
      msrTechnicalWithString::kTechnicalWithStringPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitStart ( S_snap_pizzicato& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      msrTechnical::k_NoTechnicalPlacement;

  if      (placement == "above")
    snapPizzicatoPlacementKind = msrTechnical::kTechnicalPlacementAbove;
    
  else if (placement == "below")
    snapPizzicatoPlacementKind = msrTechnical::kTechnicalPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitStart ( S_stopped& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      msrTechnical::k_NoTechnicalPlacement;

  if      (placement == "above")
    stoppedPlacementKind = msrTechnical::kTechnicalPlacementAbove;
    
  else if (placement == "below")
    stoppedPlacementKind = msrTechnical::kTechnicalPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitStart ( S_string& elt )
{
/*
        <notations>
          <technical>
            <string>5</string>
            <fret>0</fret>
          </technical>
        </notations>
*/
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      msrTechnicalWithInteger::k_NoTechnicalWithIntegerPlacement;

  if      (placement == "above")
    stringPlacementKind =
      msrTechnicalWithInteger::kTechnicalWithIntegerPlacementAbove;
    
  else if (placement == "below")
    stringPlacementKind =
      msrTechnicalWithInteger::kTechnicalWithIntegerPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitStart ( S_tap& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      msrTechnical::k_NoTechnicalPlacement;

  if      (placement == "above")
    tapPlacementKind = msrTechnical::kTechnicalPlacementAbove;
    
  else if (placement == "below")
    tapPlacementKind = msrTechnical::kTechnicalPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitStart ( S_thumb_position& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      msrTechnical::k_NoTechnicalPlacement;

  if      (placement == "above")
    thumbPositionPlacementKind = msrTechnical::kTechnicalPlacementAbove;
    
  else if (placement == "below")
    thumbPositionPlacementKind = msrTechnical::kTechnicalPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitStart ( S_toe& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      msrTechnical::k_NoTechnicalPlacement;

  if      (placement == "above")
    toePlacementKind = msrTechnical::kTechnicalPlacementAbove;
    
  else if (placement == "below")
    toePlacementKind = msrTechnical::kTechnicalPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitStart ( S_triple_tongue& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      msrTechnical::k_NoTechnicalPlacement;

  if      (placement == "above")
    tripleTonguePlacementKind = msrTechnical::kTechnicalPlacementAbove;
    
  else if (placement == "below")
    tripleTonguePlacementKind = msrTechnical::kTechnicalPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitStart ( S_up_bow& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      msrTechnical::k_NoTechnicalPlacement;

  if      (placement == "above")
    upBowPlacementKind = msrTechnical::kTechnicalPlacementAbove;
    
  else if (placement == "below")
    upBowPlacementKind = msrTechnical::kTechnicalPlacementBelow;
    
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
void mxmltree2MsrTranslator::visitStart ( S_ornaments& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_ornaments" <<
      endl;
}

void mxmltree2MsrTranslator::visitStart ( S_tremolo& elt )
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
   
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      msrSingleTremolo::k_NoSingleTremoloPlacement;
      
  msrDoubleTremolo::msrDoubleTremoloPlacementKind
    doubleTremoloPlacementKind =
      msrDoubleTremolo::k_NoDoubleTremoloPlacement;

  if      (tremoloPlacement == "above") {
    switch (fCurrentMusicXMLTremoloType) {
      case k_NoTremolo:
        // just to avoid a compiler message
        break;
        
      case kSingleTremolo:
        singleTremoloPlacementKind =
          msrSingleTremolo::kSingleTremoloPlacementAbove;
        break;
        
      case kStartTremolo:
      case kStopTremolo:
        doubleTremoloPlacementKind =
          msrDoubleTremolo::kDoubleTremoloPlacementAbove;
        break;
    } // switch
  }
  
  else if (tremoloPlacement == "below") {
    switch (fCurrentMusicXMLTremoloType) {
      case k_NoTremolo:
        // just to avoid a compiler message
        break;
        
      case kSingleTremolo:
        singleTremoloPlacementKind =
          msrSingleTremolo::kSingleTremoloPlacementBelow;
        break;
        
      case kStartTremolo:
      case kStopTremolo:
        doubleTremoloPlacementKind =
          msrDoubleTremolo::kDoubleTremoloPlacementBelow;
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
            createVoiceInStaffInCurrentPartIfNotYetDone (
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

void mxmltree2MsrTranslator::visitStart ( S_trill_mark& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_wavy_line& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_wavy_line" <<
      endl;

/*
<!ELEMENT wavy-line EMPTY>
<!ATTLIST wavy-line
    type %start-stop-continue; #REQUIRED
    number %number-level; #IMPLIED
    %position;
    %placement; 
    %color;
    %trill-sound; 
>
 */

  string wavyLineType = elt->getAttributeValue ("type");
  
  msrTechnical::msrTechnicalPlacementKind
    arrowPlacementKind =
      msrTechnical::k_NoTechnicalPlacement;

  if      (wavyLineType == "start")
    arrowPlacementKind = msrTechnical::kTechnicalPlacementAbove;
    
  else if (wavyLineType == "stop")
    arrowPlacementKind = msrTechnical::kTechnicalPlacementBelow;
    
  else if (wavyLineType == "continue")
    arrowPlacementKind = msrTechnical::kTechnicalPlacementBelow;
    
  else {
    if (wavyLineType.size ()) {
      stringstream s;
      
      s <<
        "wavy line ype \"" << wavyLineType <<
        "\" is unknown";
      
      msrMusicXMLError (
        elt->getInputLineNumber (),
        s.str());
    }   
  }

  int wavyLineNumber = elt->getAttributeIntValue ("number", 0); // JMI use it
  
  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kWavyLine);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void mxmltree2MsrTranslator::visitStart ( S_turn& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_turn" <<
      endl;

  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kTurn);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void mxmltree2MsrTranslator::visitStart ( S_inverted_turn& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_inverted_turn" <<
      endl;

  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kInvertedTurn);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void mxmltree2MsrTranslator::visitStart ( S_delayed_turn& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_delayed_turn" <<
      endl;

  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kDelayedTurn);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void mxmltree2MsrTranslator::visitStart ( S_delayed_inverted_turn& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_delayed_inverted_turn" <<
      endl;

  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kDelayedInvertedTurn);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void mxmltree2MsrTranslator::visitStart ( S_vertical_turn& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_vertical_turn" <<
      endl;

  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kVerticalTurn);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void mxmltree2MsrTranslator::visitStart ( S_mordent& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_mordent" <<
      endl;

  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kMordent);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void mxmltree2MsrTranslator::visitStart ( S_inverted_mordent& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_inverted_mordent" <<
      endl;

  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kInvertedMordent);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void mxmltree2MsrTranslator::visitStart ( S_schleifer& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_schleifer" <<
      endl;

  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kSchleifer);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void mxmltree2MsrTranslator::visitStart ( S_shake& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_shake" <<
      endl;

  fCurrentOrnament =
    msrOrnament::create (
      elt->getInputLineNumber (),
      msrOrnament::kShake);
      
  fCurrentOrnamentsList.push_back (fCurrentOrnament);
}

void mxmltree2MsrTranslator::visitStart ( S_accidental_mark& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
    msrOrnament::k_NoOrnamentPlacement;

  if      (placement == "above")
    fCurrentOrnamentPlacementKind =
      msrOrnament::kOrnamentPlacementAbove;
    
  else if (placement == "below")
    fCurrentOrnamentPlacementKind =
      msrOrnament::kOrnamentPlacementBelow;
    
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

void mxmltree2MsrTranslator::visitEnd ( S_ornaments& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
void mxmltree2MsrTranslator::visitStart( S_f& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
void mxmltree2MsrTranslator::visitStart( S_ff& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
void mxmltree2MsrTranslator::visitStart( S_fff& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
void mxmltree2MsrTranslator::visitStart( S_ffff& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
void mxmltree2MsrTranslator::visitStart( S_fffff& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
void mxmltree2MsrTranslator::visitStart( S_ffffff& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart( S_p& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
void mxmltree2MsrTranslator::visitStart( S_pp& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
void mxmltree2MsrTranslator::visitStart( S_ppp& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
void mxmltree2MsrTranslator::visitStart( S_pppp& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
void mxmltree2MsrTranslator::visitStart( S_ppppp& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
void mxmltree2MsrTranslator::visitStart( S_pppppp& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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


void mxmltree2MsrTranslator::visitStart( S_mf& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
void mxmltree2MsrTranslator::visitStart( S_mp& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart( S_fp& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
void mxmltree2MsrTranslator::visitStart( S_fz& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart( S_rf& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart( S_sf& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart( S_rfz& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart( S_sfz& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart( S_sfp& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart( S_sfpp& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart( S_sffz& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart( S_other_dynamics& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
void mxmltree2MsrTranslator::visitStart ( S_grace& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_grace" <<
      endl;

/* JMI pour positionnement:
        <grace steal-time-previous="20"/>
        <grace steal-time-following="20"/>

 */
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
void mxmltree2MsrTranslator::visitStart ( S_chord& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_chord" <<
      endl;

  fCurrentNoteBelongsToAChord = true;

  // delay the handling until 'visitEnd ( S_note& elt)',
  // because we don't know yet the voice and staff numbers for sure
  // (they can be specified after <chord/> in the <note/>)
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::visitStart ( S_time_modification& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_time_modification" <<
      endl;

  // there may be no '<tuplet number="n" type="start" />'
  // in the tuplet notes after the first one,
  // so we detect tuplet notes on '<time-modification>' ??? JMI
  fCurrentNoteHasATimeModification = true;
}

void mxmltree2MsrTranslator::visitStart ( S_actual_notes& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_actual_notes" <<
      endl;

  fCurrentActualNotes = (int)(*elt);

  // notes inside a tuplet have no <tuplet/> markup
  // and 2 actual notes indicate a double tremolo
  if (fCurrentActualNotes != 2)
    fCurrentNoteBelongsToATuplet = true;
}

void mxmltree2MsrTranslator::visitStart ( S_normal_notes& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_normal_notes" <<
      endl;

  fCurrentNormalNotes = (int)(*elt);

  // notes inside a tuplet have no <tuplet/> markup
  // and 1 actual note indicates a double tremolo
  if (fCurrentNormalNotes != 1)
    fCurrentNoteBelongsToATuplet = true;
}

void mxmltree2MsrTranslator::visitStart ( S_normal_type& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_tuplet& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_tuplet" <<
      endl;

// JMI           <tuplet bracket="yes" number="1" show-number="both" show-type="both" type="start"/>

  fCurrentTupletNumber =
    elt->getAttributeIntValue ("number", 0);
    
  string tupletType =
    elt->getAttributeValue("type");
  
  /* JMI* /
  cerr <<
    "--> mxmltree2MsrTranslator::visitStart ( S_tuplet, fCurrentTupletNumber = " <<
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

  string tupletShowNumber =
    elt->getAttributeValue("show-number");

  msrTuplet::msrTupletShowNumberKind tupletShowNumberKind; // JMI use it
  
  if      (tupletShowNumber == "yes") {
    tupletShowNumberKind = msrTuplet::kTupletShowNumberYes;
  }

  else if (tupletShowNumber == "no") {
    tupletShowNumberKind = msrTuplet::kTupletShowNumberNo;
  }

  else {
 // JMI   if (restMeasure.size ())
      msrMusicXMLError (
        elt->getInputLineNumber (),
          "unknown tuplet show number \"" + tupletShowNumber + "\"");
  }  

  string tupletShowType =
    elt->getAttributeValue("show-type");

  msrTuplet::msrTupletShowTypeKind tupletShowTypeKind; // JMI use it
  
  if      (tupletShowType == "yes") {
    tupletShowTypeKind = msrTuplet::kTupletShowTypeYes;
  }

  else if (tupletShowType == "no") {
    tupletShowTypeKind = msrTuplet::kTupletShowTypeNo;
  }

  else {
// JMI    if (restMeasure.size ())
      msrMusicXMLError (
        elt->getInputLineNumber (),
          "unknown tuplet show type \"" + tupletShowType + "\"");
  }  

  fCurrentNoteBelongsToATuplet = true;
}

void mxmltree2MsrTranslator::visitStart ( S_tuplet_number& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_tuplet_number" <<
      endl;

  // not handled JMI
  fCurrentTupletDisplayNumber = (int)(*elt);
}

void mxmltree2MsrTranslator::visitStart ( S_tuplet_type& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
void mxmltree2MsrTranslator::visitStart ( S_rest& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_rest" <<
      endl;

  /*
      <note>
        <rest/>
        <duration>24</duration>
        <voice>1</voice>
      </note>

      <rest>
        <display-step>E</display-step>
        <display-octave>4</display-octave>
      </rest>

*/
  //  cerr << "--> mxmltree2MsrTranslator::visitStart ( S_rest& elt ) " <<endl;
  fCurrentNoteIsARest = true;

  string restMeasure = elt->getAttributeValue ("measure");
  
  fCurrentRestMeasure = false;

  if (restMeasure == "yes") {
    fCurrentRestMeasure = true; // USE IT! JMI ???
  }

  else if (restMeasure == "no") {
    fCurrentRestMeasure = false; // USE IT! JMI ???
  }

  else {
    if (restMeasure.size ())
      msrMusicXMLError (
        elt->getInputLineNumber (),
          "unknown rest measure \"" + restMeasure + "\"");
  }

  fCurrentNoteDisplayWholeNotes =
    rational (0, 1); // JMI
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::visitStart ( S_display_step& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_display_step" <<
      endl;

  string displayStep = elt->getValue();
  
  checkStep (
    elt->getInputLineNumber (),
    displayStep);

  fCurrentDisplayDiatonicPitch =
    msrDiatonicPitchFromString (displayStep [0]);

  // pitched rests don't allow for alterations since
  // the display-step merely indicates where to place them on the staff
  fCurrentNoteAlteration = kNatural;
}

void mxmltree2MsrTranslator::visitStart ( S_display_octave& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_display_octave" <<
      endl;

  fCurrentDisplayOctave = (int)(*elt);
}

void mxmltree2MsrTranslator::visitEnd ( S_unpitched& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
  
 // fCurrentNoteDiatonicPitch = // JMI
   // fCurrentHarmonyRootDiatonicPitch;
}

//______________________________________________________________________________
S_msrChord mxmltree2MsrTranslator::createChordFromItsFirstNote (
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
        chordFirstNote->getNoteSoundingWholeNotes (),
        chordFirstNote->getNoteDisplayWholeNotes (),
        chordFirstNote->getNoteGraphicDuration ());

  // set the chords's MSR strings
  chord->
    setChordMSRstrings ();
  
  // chord's tie kind is that of its first note
  chord->
    setChordTie (
      chordFirstNote->
        getNoteTie ());
  
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
  copyNoteElementsToChord (
    chordFirstNote, chord);
    
  return chord;
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::copyNoteArticulationsToChord (
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
void mxmltree2MsrTranslator::copyNoteTechnicalsToChord (
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
void mxmltree2MsrTranslator::copyNoteTechnicalWithIntegersToChord (
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
void mxmltree2MsrTranslator::copyNoteTechnicalWithStringsToChord (
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
void mxmltree2MsrTranslator::copyNoteOrnamentsToChord (
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

    chord->
      addOrnamentToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::copyNoteSingleTremoloToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's singleTremolo if any from the first note to chord
  
  S_msrSingleTremolo
    noteSingleTremolo =
      note->
        getNoteSingleTremolo ();

  if (noteSingleTremolo) {
    if (gGeneralOptions->fTraceTremolos || gGeneralOptions->fTraceChords)
      cerr << idtr <<
        "--> copying singleTremolo '" <<
        noteSingleTremolo->singleTremoloAsString () <<
        "' from note " << note->noteAsString () <<
        " to chord" <<
        endl;
    
    chord->
      addSingleTremoloToChord (noteSingleTremolo);
  }
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::copyNoteDynamicsToChord (
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

    chord->
      addDynamicsToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::copyNoteOtherDynamicsToChord (
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

    chord->
      addOtherDynamicsToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::copyNoteWordsToChord (
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

    chord->
      addWordsToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::copyNoteBeamsToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's beams if any from the first note to chord
  
  list<S_msrBeam>
    noteBeams =
      note->
        getNoteBeams ();
                          
  list<S_msrBeam>::const_iterator i;
  for (
    i=noteBeams.begin();
    i!=noteBeams.end();
    i++) {

    if (gGeneralOptions->fTraceBeams || gGeneralOptions->fTraceChords)
      cerr << idtr <<
        "--> copying beam '" <<
        (*i)->beamAsString () <<
        "' from note " << note->noteAsString () <<
        " to chord" <<
        endl;

    chord->
      addBeamToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::copyNoteSlursToChord (
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
        (*i)->slurAsString () <<
        "' from note " << note->noteAsString () <<
        " to chord" <<
        endl;

    chord->
      addSlurToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::copyNoteLigaturesToChord (
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

    chord->
      addLigatureToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::copyNoteWedgesToChord (
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

    chord->
      addWedgeToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::copyNoteHarmonyToChord (
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

    chord->
      setChordHarmony (harmony);
  }   
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::copyNoteElementsToChord (
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

  // copy note's single tremolo if any to the chord
  copyNoteSingleTremoloToChord (note, chord);

  // copy note's dynamics if any to the chord
  copyNoteDynamicsToChord (note, chord);

  // copy note's other dynamics if any to the chord
  copyNoteOtherDynamicsToChord (note, chord);

  // copy note's words if any to the chord
  copyNoteWordsToChord (note, chord);

  // copy note's beams if any to the chord
  copyNoteBeamsToChord (note, chord);

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
void mxmltree2MsrTranslator::createTupletWithItsFirstNote (S_msrNote firstNote)
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
void mxmltree2MsrTranslator::finalizeTuplet (
  int inputLineNumber)
{
  if (gGeneralOptions->fTraceTuplets)
    cerr << idtr <<
      "mxmltree2MsrTranslator::finalizeTuplet(), " <<
      "line " << inputLineNumber <<
      endl;

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNotYetDone (
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
void mxmltree2MsrTranslator::attachCurrentArticulationsToNote (
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
void mxmltree2MsrTranslator::attachCurrentTechnicalsToNote (
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
void mxmltree2MsrTranslator::attachCurrentTechnicalWithIntegersToNote (
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
void mxmltree2MsrTranslator::attachCurrentTechnicalWithStringsToNote (
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
void mxmltree2MsrTranslator::attachCurrentOrnamentsToNote (
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
void mxmltree2MsrTranslator::attachCurrentSingleTremoloToNote (
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
void mxmltree2MsrTranslator::attachCurrentArticulationsToChord ( // JMI
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
void mxmltree2MsrTranslator::attachCurrentOrnamentsToChord ( // JMI
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
void mxmltree2MsrTranslator::attachPendingDynamicsToNote (
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

        int numberOfDynamics = fPendingDynamics.size ();

        if (numberOfDynamics > 1)
          s <<
            "there are " << numberOfDynamics << " dynamics";
        else
          s <<
            "there is 1 dynamics";
        s <<
          " attached to a rest";
          
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
void mxmltree2MsrTranslator::attachPendingOtherDynamicsToNote (
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

        int numberOfOtherDynamics = fPendingOtherDynamics.size ();

        if (numberOfOtherDynamics > 1)
          s <<
            "there are " << numberOfOtherDynamics << " other dynamics";
        else
          s <<
            "there is 1 other dynamics";
        s <<
          " attached to a rest";
          
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
void mxmltree2MsrTranslator::attachPendingWordsToNote (
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
        stringstream s;

        int numberOfWords = fPendingWords.size ();

        if (numberOfWords > 1)
          s <<
            "there are " << numberOfWords << " words";
        else
          s <<
            "there is 1 word";
        s <<
          " attached to a rest";
          
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
void mxmltree2MsrTranslator::attachPendingSlursToNote (
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

        int numberOfSlurs = fPendingSlurs.size ();

        if (numberOfSlurs > 1)
          s <<
            "there are " << numberOfSlurs << " slurs";
        else
          s <<
            "there is 1 slur";
        s <<
          " attached to a rest";
          
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
void mxmltree2MsrTranslator::attachPendingLigaturesToNote (
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

        int numberOfLigatures = fPendingLigatures.size ();

        if (numberOfLigatures > 1)
          s <<
            "there are " << numberOfLigatures << " ligatures";
        else
          s <<
            "there is 1 ligature";
        s <<
          " attached to a rest";
          
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
void mxmltree2MsrTranslator::attachPendingWedgesToNote (
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

        int numberOfWedges = fPendingWedges.size ();

        if (numberOfWedges > 1)
          s <<
            "there are " << numberOfWedges << " wedges";
        else
          s <<
            "there is 1 wedge";
        s <<
          " attached to a rest";
          
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

void mxmltree2MsrTranslator::attachPendingElementsToNote (
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
void mxmltree2MsrTranslator::visitEnd ( S_note& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

  // determine quarter tones note pitch
  fCurrentNoteQuarterTonesPitch =
    quarterTonesPitchFromDiatonicPitchAndAlteration (
      inputLineNumber,
      fCurrentNoteDiatonicPitch,
      fCurrentNoteAlteration);

  // determine quarter tones note display pitch
  fCurrentNoteQuarterTonesDisplayPitch =
    quarterTonesPitchFromDiatonicPitchAndAlteration (
      inputLineNumber,
      fCurrentDisplayDiatonicPitch,
      fCurrentNoteAlteration);

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNotYetDone (
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

    const int fieldWidth = 27;

    cerr <<
      idtr <<
        setw(fieldWidth) << "--> fCurrentNoteStaffNumber" << " = " <<
        fCurrentNoteStaffNumber <<
        endl <<
      idtr <<
        setw(fieldWidth) << "--> fCurrentNoteVoiceNumber" << " = " <<
        fCurrentNoteVoiceNumber <<
        endl <<
      idtr <<
        setw(fieldWidth) << "--> current voice" << " = \"" <<
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
      "--> currentNoteSoundingWholeNotes = " << 
      fCurrentNoteSoundingWholeNotes << ", " << 
      "--> currentDivisionsPerQuarterNote = " <<
      fCurrentDivisionsPerQuarterNote <<
      endl;

  // set current note displayed divisions right now,
  // before we create the note

  if (fCurrentNoteIsAGraceNote) {
    // set current grace note divisions      
    fCurrentNoteSoundingWholeNotes =
      fCurrentDivisions->
        durationAsDivisions (
          inputLineNumber,
          fCurrentNoteGraphicDuration);
  
    // set current grace note displayed divisions to note divisions JMI   ???
    fCurrentNoteDisplayWholeNotes =
      fCurrentNoteSoundingWholeNotes;
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
    fCurrentNoteDisplayWholeNotes =
      fCurrentDivisions->
        durationAsDivisions (
          inputLineNumber,
          fCurrentNoteGraphicDuration);

    // rationalise it
    fCurrentNoteDisplayWholeNotes.rationalise ();
  }

  else if (fCurrentNoteIsARest) {
    // rest
    // leave fCurrentNoteDisplayWholeNotes as it is
  }

  else {
    // standalone note
    fCurrentNoteDisplayWholeNotes =
      fCurrentNoteSoundingWholeNotes;
  }

  // create the (new) note
  S_msrNote
    newNote =
      msrNote::create (
        inputLineNumber,
        fCurrentPart,
        
        msrNote::k_NoNoteKind, // will be set by 'setNoteKind()' later
        
        fCurrentNoteQuarterTonesPitch,

 // JMI       fCurrentDivisionsPerQuarterNote, // JMI
        
        fCurrentNoteSoundingWholeNotes,
        fCurrentNoteDisplayWholeNotes,
        
        fCurrentNoteDotsNumber,
        
        fCurrentNoteGraphicDuration,
        
        fCurrentNoteOctave,
        
        fCurrentNoteQuarterTonesDisplayPitch,
        fCurrentDisplayOctave,
        
        fCurrentNoteIsARest,
        fCurrentNoteIsUnpitched,
        
        fCurrentNoteIsAGraceNote);

  // set note accidentals
  newNote->
    setNoteEditorialAccidentalKind (
      fCurrentNoteEditorialAccidentalKind);

  newNote->
    setNoteCautionaryAccidentalKind (
      fCurrentNoteCautionaryAccidentalKind);

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
            gMsrOptions->fMsrQuarterTonesPitchesLanguage,
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
            gMsrOptions->fMsrQuarterTonesPitchesLanguage,
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
          
          fCurrentHarmonyRootQuarterTonesPitch,
          
          fCurrentHarmonyKind,
          fCurrentHarmonyKindText,
          
          fCurrentHarmonyBassQuarterTonesPitch,
          
          fCurrentNoteSoundingWholeNotes);
  
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

  // set the note's MSR strings, done here
  // to avoid doing it in MSR visitors later,
  // such as lpsr2LilypondTranslator.cpp,
  // which don't have the disisions context available,
  // and only now because this needs fNoteKind to be set
  newNote->
    setNoteMSRstrings ();
    
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
     const int fieldWidth = 27;

    cerr <<
      endl <<
      idtr <<
        "==> AFTER visitEnd (S_note&) " <<
        newNote->noteAsString () <<
        ", line " << inputLineNumber <<
        " we have:" <<
        endl <<
        
      idtr <<
        setw(fieldWidth) <<
        "--> fCurrentNoteStaffNumber" << " = " <<
        fCurrentNoteStaffNumber <<
        endl <<
      idtr <<
        setw(fieldWidth) <<
        "--> fCurrentNoteVoiceNumber" << " = " <<
        fCurrentNoteVoiceNumber <<
        endl <<
      idtr <<
        setw(fieldWidth) <<
        "--> current voice" << " = \"" <<
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
void mxmltree2MsrTranslator::handleStandaloneOrDoubleTremoloNoteOrGraceNoteOrRest (
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
      createVoiceInStaffInCurrentPartIfNotYetDone (
        inputLineNumber,
        fCurrentNoteStaffNumber,
        fCurrentVoiceNumber);
    
  if (gGeneralOptions->fTraceNotes) {    
    cerr <<
      idtr <<
        "Handling standalone, double tremolo or grace note or rest:" <<
        endl;

      idtr++;

    cerr <<
      idtr <<
        newNote->noteAsString () <<
        endl;

    const int fieldWidth = 25;

    cerr <<
      idtr <<
        setw(fieldWidth) << "voice" << " : \"" <<
        currentVoice->getVoiceName () << "\"" <<
        endl <<
      idtr <<
        setw(fieldWidth) << "line:" << " : " <<
        inputLineNumber <<
        endl <<
      idtr <<
        setw(fieldWidth) << "fCurrentNoteIsAGraceNote" << " : " <<
        booleanAsString (fCurrentNoteIsAGraceNote) <<
        endl <<
      idtr <<
        setw(fieldWidth) << "fCurrentGraceNotes" << " : ";
        
    if (fCurrentGraceNotes)
      cerr << fCurrentGraceNotes;
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
    if (! fCurrentGraceNotes) {
      // this is the first grace note in grace notes

      if (gGeneralOptions->fTraceTuplets || gGeneralOptions->fTraceGraceNotes) {
        cerr <<  idtr <<
          "Vreating grace notes for note " <<
          newNote->noteAsString () <<
          " in voice \"" <<
          currentVoice->getVoiceName () << "\"" <<
          endl;
      }

      // create grace notes
      fCurrentGraceNotes =
        msrGraceNotes::create (
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
          setNoteIsFollowedByGraceNotes ();
      
      // append the grace notes to the current voice
      currentVoice->
        appendGraceNotesToVoice (
          fCurrentGraceNotes);
    }

    // append newNote to the current grace notes
    if (gGeneralOptions->fTraceTuplets || gGeneralOptions->fTraceGraceNotes) {
      cerr <<  idtr <<
        "Appending note " <<
        newNote->noteAsString () <<
        " to the grace notes in voice \"" <<
        currentVoice->getVoiceName () << "\"" <<
        endl;
    }

    // attach the pending elements, if any, to newNote
    attachPendingElementsToNote (newNote);

    fCurrentGraceNotes->
      appendNoteToGraceNotes (newNote);
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
            "Appending standalone " <<
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
            "Setting standalone note '" <<
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
            "Setting standalone note '" <<
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

    if (fCurrentGraceNotes)
      // this is the first note after the grace notes,
      // forget about the latter
      fCurrentGraceNotes = 0;
  
    // attach the pending elements, if any, to the note
    attachPendingElementsToNote (newNote);
  
    // append newNote to the current voice
    if (gGeneralOptions->fTraceNotes) {
      cerr <<  idtr <<
        "Appending standalone " <<
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
void mxmltree2MsrTranslator::handleLyric (
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
        createVoiceInStaffInCurrentPartIfNotYetDone (
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
void mxmltree2MsrTranslator::handleNoteBelongingToAChord (
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
      "mxmltree2MsrTranslator::handleNoteBelongingToAChord()" <<
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
        createVoiceInStaffInCurrentPartIfNotYetDone (
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
        "mxmltree2MsrTranslator::handleNoteBelongingToAChord()" <<
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
        "mxmltree2MsrTranslator::handleNoteBelongingToAChord()" <<
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
          int chordFirstNoteSoundingWholeNotes = // JMI
            chordFirstNote->
              getNoteSoundingWholeNotes ();

          /* JMI
          // updating chord's divisions // JMI
          if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceChords) {
            cerr <<
              idtr <<
                "Updating sounding divisions for double tremolo chord '" <<
                "' " << fCurrentChord->chordAsString () <<
                " to '" << chordFirstNoteSoundingWholeNotes <<
                "' in voice \"" <<
                currentVoice->getVoiceName () <<
                "\"" <<
                endl;
          }
          
          fCurrentChord->
            setChordSoundingWholeNotes ( // ??? JMI
              chordFirstNoteSoundingWholeNotes);
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
  copyNoteElementsToChord (
    newChordNote, fCurrentChord);
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::handleNoteBelongingToATuplet (
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
      "mxmltree2MsrTranslator::handleNoteBelongingToATuplet " <<
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
void mxmltree2MsrTranslator::handleNoteBelongingToAChordInATuplet (
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
      "mxmltree2MsrTranslator::handleNoteBelongingToAChordInATuplet " <<
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
        createVoiceInStaffInCurrentPartIfNotYetDone (
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
  copyNoteElementsToChord (
    newChordNote, fCurrentChord);
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::handleTupletsPendingOnTupletStack (
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
      createVoiceInStaffInCurrentPartIfNotYetDone (
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

void mxmltree2MsrTranslator::displayLastHandledNoteInVoice (string header)
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

void mxmltree2MsrTranslator::displayLastHandledTupletInVoice (string header)
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
void mxmltree2MsrTranslator::handleRepeatStart (
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
      msrBarline::kRepeatStartBarline);

/* JMI
  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceInStaffInCurrentPartIfNotYetDone (
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
      createRepeatAndAppendItToPart (inputLineNumber);

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
void mxmltree2MsrTranslator::handleRepeatEnd (
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
      createVoiceInStaffInCurrentPartIfNotYetDone (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  // set the barline category
  barline->
    setBarlineCategory (msrBarline::kRepeatEndBarline);

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
          msrBarline::kLeftLocation,
          msrBarline::kHeavyLightStyle,
          msrBarline::kStartEndingType,
          fCurrentBarlineEndingNumber,
          msrBarline::kForwardRepeatDirection,
          fCurrentBarlineRepeatWinged);

    // set the implicit barline category
    implicitBarline->
      setBarlineCategory (
        msrBarline::kRepeatStartBarline);
  
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
        createRepeatAndAppendItToPart (inputLineNumber);
  
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
    createRepeatAndAppendItToPart (inputLineNumber);

  fRepeatHasBeenCreatedForCurrentPart = true;  
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::handleHookedEndingStart (
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
      createVoiceInStaffInCurrentPartIfNotYetDone (
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
      createRepeatAndAppendItToPart (inputLineNumber);

    fRepeatHasBeenCreatedForCurrentPart = true;  
  }

  // set the barline category
  barline->
    setBarlineCategory (msrBarline::kHookedEndingStartBarline);
  
  // append the bar line to the voice
  currentVoice->
    appendBarlineToVoice (barline);

  // push the barline onto the stack
  fPendingBarlines.push (barline);
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::handleHookedEndingEnd (
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
      createVoiceInStaffInCurrentPartIfNotYetDone (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  // set the barline category
  barline->
    setBarlineCategory (msrBarline::kHookedEndingEndBarline);

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
    appendRepeatEndingToPart (
      inputLineNumber,
      fCurrentBarlineEndingNumber,
      msrRepeatEnding::kHookedEnding);
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::handleHooklessEndingStart (
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
      createVoiceInStaffInCurrentPartIfNotYetDone (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  // set the barline category
  barline->
    setBarlineCategory (msrBarline::kHooklessEndingStartBarline);
  
  // append the bar line to the voice
  currentVoice-> // JMI
    appendBarlineToVoice (barline);

  // push the barline onto the stack
  fPendingBarlines.push (barline); // JMI
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::handleHooklessEndingEnd (
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
      createVoiceInStaffInCurrentPartIfNotYetDone (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  // set the barline category
  barline->
    setBarlineCategory (msrBarline::kHooklessEndingEndBarline);
  
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
    appendRepeatEndingToPart (
      inputLineNumber,
      fCurrentBarlineEndingNumber,
      msrRepeatEnding::kHooklessEnding);
}

//______________________________________________________________________________
void mxmltree2MsrTranslator::visitStart ( S_rehearsal& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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
      createVoiceInStaffInCurrentPartIfNotYetDone (
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
void mxmltree2MsrTranslator::visitStart ( S_harmony& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_root_step& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_root_alter& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_kind& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_bass_step& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_bass_alter& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_degree_value& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> Start visiting S_degree_value" <<
      endl;

  fCurrentHarmonyDegreeValue = (int)(*elt);
}

void mxmltree2MsrTranslator::visitStart ( S_degree_alter& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitStart ( S_degree_type& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
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

void mxmltree2MsrTranslator::visitEnd ( S_harmony& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors)
    cerr << idtr <<
      "--> End visiting S_harmony" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
  
  // convert root diatonic pitch to a quarter tone pitch
  fCurrentHarmonyRootQuarterTonesPitch =
    quarterTonesPitchFromDiatonicPitchAndAlteration (
      inputLineNumber,
      fCurrentHarmonyRootDiatonicPitch,
      fCurrentHarmonyRootAlteration);

  // convert bass diatonic pitch to a quarter tone pitch
  fCurrentHarmonyBassQuarterTonesPitch =
    quarterTonesPitchFromDiatonicPitchAndAlteration (
      inputLineNumber,
      fCurrentHarmonyBassDiatonicPitch,
      fCurrentHarmonyBassAlteration);

  if (
    fCurrentHarmonyRootQuarterTonesPitch
      ==
    fCurrentHarmonyBassQuarterTonesPitch) {
    stringstream s;

    s <<
      "harmony root and bass notes are both equal to '" <<
      msrDiatonicPitchAsString (
        gMsrOptions->fMsrQuarterTonesPitchesLanguage,
        msrDiatonicPitchFromQuarterTonesPitch (
          inputLineNumber,
          fCurrentHarmonyRootQuarterTonesPitch)) <<        
      "', ignoring the latter";

    msrMusicXMLWarning (
     inputLineNumber,
     s.str());

    fCurrentHarmonyBassQuarterTonesPitch = k_NoQuarterTonesPitch;
  }


/*
 * Figured bass elements take their position from the first
    regular note (not a grace note or chord note) that follows
    in score order. The optional duration element is used to
    indicate changes of figures under a note.    

    Figures are ordered from top to bottom. A figure-number is a
    number. Values for prefix and suffix include the accidental
    values sharp, flat, natural, double-sharp, flat-flat, and
    sharp-sharp. Suffixes include both symbols that come after
    the figure number and those that overstrike the figure number.
    The suffix value slash is used for slashed numbers indicating
    chromatic alteration. The orientation and display of the slash
    usually depends on the figure number. The prefix and suffix
    elements may contain additional values for symbols specific
    to particular figured bass styles. The value of parentheses
    is "no" if not present.
    *
    */


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
  for (int i = 0; i < fCurrentForwardDurationDivisions; i++) {
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

