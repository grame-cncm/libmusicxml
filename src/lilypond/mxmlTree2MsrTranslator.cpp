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
#include <climits>      // INT_MIN, INT_MAX
#include <iomanip>      // setw, setprecision, ...
#include <algorithm>    // for_each

#include "xml_tree_browser.h"

#include "conversions.h"

#include "utilities.h"

#include "msr.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "musicXMLOptions.h"
#include "msrOptions.h"

#include "xml2lyOptionsHandling.h"

#include "mxmlTree2MsrTranslator.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
mxmlTree2MsrTranslator::mxmlTree2MsrTranslator (
  S_msrScore       scoreSkeleton,
  indentedOstream& ios)
  : fLogOutputStream (ios)
{
  // initialize note data to a neutral state
  initializeNoteData ();

  // the MSR score we're populating
  fMsrScore = scoreSkeleton;

  // divisions
  fCurrentDivisionsPerQuarterNote = 1;
 
  // part handling

  // measure style handling
  fCurrentSlashTypeKind     = k_NoSlashType;
  fCurrentSlashUseDotsKind  = k_NoSlashUseDots;
  fCurrentSlashUseStemsKind = k_NoSlashUseStems;

  fCurrentBeatRepeatSlashes = -1;

  fCurrentMeasuresRepeatKind =
    msrMeasuresRepeat::k_NoMeasuresRepeat;

  fCurrentMeasuresRepeatMeasuresNumber = -1;
  fCurrentMeasuresRepeatSlashesNumber  = -1;
  
  fCurrentMultipleRestMeasuresNumber   = 0;
  fRemainingMultipleRestMeasuresNumber = 0;
  fOnGoingMultipleRest = false;
  fCurrentMultipleRestHasBeenCreated = false;

  fCurrentSlashDotsNumber = -1;
  fCurrentSlashGraphicDurationKind = k_NoDuration;
  
  // staff details handling
  fStaffDetailsStaffNumber = K_NO_STAFF_NUMBER;
  
  fCurrentStaffTypeKind =
    msrStaffDetails::kRegularStaffType;

  fCurrentShowFretsKind =
    msrStaffDetails::kShowFretsNumbers; // default value

  fCurrentPrintObjectKind =
    msrStaffDetails::kPrintObjectYes; // default value

  fCurrentPrintSpacingKind =
    msrStaffDetails::kPrintSpacingNo; // default value ??? JMI

  // staff tuning handling
  fCurrentStaffTuningAlterationKind = k_NoAlteration;
  fCurrentStaffTuningOctave         = -1;

  fCurrentStaffDetailsCapo = 0;
  fCurrentStaffDetailsStaffSize = 0;

  fOnGoingStaffTuning = false;

  // scordatura handling
  
  fCurrentStringTuningNumber = -1;
  fCurrentStringTuningDiatonicPitchKind = k_NoDiatonicPitch;
  fCurrentStringTuningAlterationKind = k_NoAlteration;
  fCurrentStringTuningOctave = -1;
  
  fOnGoingAccord = false;

  // staff handling
  fCurrentMusicXMLStaffNumber = K_NO_STAFF_NUMBER;

  // voice handling
  fCurrentMusicXMLVoiceNumber = K_NO_VOICE_NUMBER;

  // clef handling
  fCurrentClefStaffNumber = K_NO_STAFF_NUMBER;
  fCurrentClefSign = "";
  fCurrentClefLine = -1;
  fCurrentClefOctaveChange = -77;

  // key handling
  fCurrentKeyKind = msrKey::kTraditionalKind;
  
  fCurrentKeyStaffNumber = K_NO_STAFF_NUMBER;
  fCurrentKeyFifths = -1;
  fCurrentKeyCancelFifths = -37;
  fCurrentKeyModeKind = msrKey::kMajorMode;

  // time handling
  fCurrentTimeSymbolKind =
    msrTime::kTimeSymbolNone; // default value

  fOnGoingInterchangeable = false;

  // measures
  fCurrentMeasureNumber = "???";
  fCurrentMeasureOrdinalNumber = 0;
  
  // transpose handling
  fCurrentTransposeNumber = -213;
  fCurrentTransposeDiatonic = -214;
  fCurrentTransposeChromatic = - 215;

  // direction handling
  fCurrentDirectionStaffNumber = K_NO_STAFF_NUMBER; // it may be absent
  fOnGoingDirection     = true;

  // direction-type handling
  fOnGoingDirectionType = false;

  // accordion-registration handling
  fCurrentAccordionHigh   = 0;
  fCurrentAccordionMiddle = 0;
  fCurrentAccordionLow    = 0;
  
  fCurrentAccordionNumbersCounter = 0;

  // metronome handling
  fCurrentMetrenomePerMinute = "";
  fCurrentMetronomeParenthesedKind = msrTempo::kTempoParenthesizedNo;

  fCurrentMetrenomeDotsNumber = 0;
  fCurrentMetrenomeRelationKind = msrTempo::kTempoRelationNone;
  fCurrentMetronomeDurationKind = k_NoDuration;
  fCurrentMetronomeBeamValue = "";

  fOnGoingMetronomeTuplet = false;
  fCurrentMetrenomeNormalDotsNumber = 0;

  // time handling
  fCurrentTimeStaffNumber = K_NO_STAFF_NUMBER;
  fCurrentTimeBeats = "";

  // lyrics handling
  fOnGoingLyric = false;
  
  fCurrentStanzaNumber = K_NO_STANZA_NUMBER;
  fCurrentStanzaName = K_NO_STANZA_NAME;
  
  fCurrentSyllabic = "";

  fCurrentSyllableKind       = msrSyllable::kSyllableNone;
  fCurrentSyllableExtendKind = msrSyllable::kSyllableExtendNone;
  fOnGoingSyllableExtend     = false;

  fFirstSyllableInSlurKind     = msrSyllable::kSyllableNone;
  fFirstSyllableInLigatureKind = msrSyllable::kSyllableNone;

  fCurrentNoteHasLyrics = false;
  fASkipSyllableHasBeenGeneratedForcurrentNote = false;
  fLastHandledNoteInVoiceHasLyrics = false;

  // scordatura handling
  fOnGoingAccord = false;

  // harmonies handling
  fHarmonyVoicesCounter = 0;
    
  fCurrentHarmonyRootDiatonicPitchKind = k_NoDiatonicPitch;
  fCurrentHarmonyRootAlterationKind    = k_NoAlteration;
  fCurrentHarmonyKind                  = k_NoHarmony;
  fCurrentHarmonyKindText              = "";
  fCurrentHarmonyBassDiatonicPitchKind = k_NoDiatonicPitch;
  fCurrentHarmonyBassAlterationKind    = k_NoAlteration;
  fCurrentHarmonyDegreeValue           = -1;
  fCurrentHarmonyDegreeAlterationKind  = k_NoAlteration;

  // figured bass handling
  fOnGoingFiguredBass                   = false;
  fPendingFiguredBass                   = false;
  fCurrentFiguredBassSoundingWholeNotes = rational (0, 1);
  fCurrentFiguredBassParenthesesKind =
    msrFiguredBass::kFiguredBassParenthesesNo; // default value
  fCurrentFigureNumber                  = -1;
  
  // frames handling
  fCurrentFrameStrings  = -1;
  fCurrentFrameFrets    = -1;
  fCurrentFrameFirstFret = -1;
  fCurrentFrameNoteStringNumber = -1;
  fCurrentFrameNoteFretNumber = -1;
  fCurrentFrameNoteFingering = -1;
  fCurrentFrameNoteBarreTypeKind = msrFrameNote::kBarreTypeNone;

  fOnGoingFrame = false;
  fOnGoingFrameNote = false;

  // barline handling
  fOnGoingBarline = false;
  
  fCurrentEndingStartBarline = nullptr;
    
  // repeats handling
  fOnGoingRepeat = false;
  fRepeatEndCounter = 0;
  fOnGoingRepeatHasBeenCreated = false;

  // MusicXML notes handling
  fCurrentNoteDiatonicPitchKind = k_NoDiatonicPitch;
  fCurrentNoteAlterationKind    = k_NoAlteration;

  // note print kind
  fCurrentNotePrintKind = msrNote::kNotePrintYes;

  // note head
  fCurrentNoteHeadKind = msrNote::kNoteHeadNormal;
  fCurrentNoteHeadFilledKind = msrNote::kNoteHeadFilledYes;
  fCurrentNoteHeadParenthesesKind = msrNote::kNoteHeadParenthesesNo;
  
  // ongoing note
  fOnGoingNote = false;

  // staff
  fCurrentMusicXMLStaffNumber  = K_NO_STAFF_NUMBER;
  fPreviousNoteMusicXMLStaffNumber = K_NO_STAFF_NUMBER;

  // staff change detection
  fCurrentStaffNumberToInsertInto = 1; // default value JMI K_NO_STAFF_NUMBER;

  // cross staff chords
  fCurrentChordStaffNumber = K_NO_STAFF_NUMBER;
  fCurrentNoteIsCrossStaves = false;
    
  // voice
  fCurrentMusicXMLVoiceNumber = K_NO_VOICE_NUMBER;

  // technicals handling
  fOnGoingTechnical = false;
  fBendAlterValue = -39.3;

  // ornaments handling

  // grace notes handling

  // tremolos handling
  fCurrentNoteBelongsToADoubleTremolo = false;
  fCurrentTremoloTypeKind             = k_NoTremoloType;

  // chords handling
  fOnGoingChord = false;

  // tuplets handling
  fCurrentTupletNumber  = -1;
  fPreviousTupletNumber = -1;
  fCurrentATupletStopIsPending = false;

  fOnGoingTupletActual = false;
  fCurrentTupletActualNumber = -1;
  fCurrentTupletActualType = "?";
  fCurrentTupletActualDotsNumber = -1;

  fOnGoingTupletNormal = false;
  fCurrentTupletNormalNumber = -1;
  fCurrentTupletNormalType = "?";
  fCurrentTupletNormalDotsNumber = -1;

  fOnGoingTupletActual = false;
  fCurrentTupletActualNumber = -1;
  fCurrentTupletActualType = "";
  fCurrentTupletActualDotsNumber = 0;
  
  fOnGoingTupletNormal = false;
  fCurrentTupletNormalNumber = -1;
  fCurrentTupletNormalType = "";
  fCurrentTupletNormalDotsNumber = 0;

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
  
  fCurrentForwardStaffNumber = K_NO_STAFF_NUMBER;
  fCurrentForwardVoiceNumber = K_NO_VOICE_NUMBER;
  
  fOnGoingForward = false;
}

mxmlTree2MsrTranslator::~mxmlTree2MsrTranslator ()
{}

//________________________________________________________________________
void mxmlTree2MsrTranslator::browseMxmlTree (
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
void mxmlTree2MsrTranslator::initializeNoteData ()
{
  // basic note description

// JMI  fCurrentNoteKind = k_NoNoteKind;

  fCurrentNoteQuarterTonesPitchKind = k_NoQuarterTonesPitch_QTP;
  
  fCurrentNoteSoundingWholeNotes             = rational (-13, 1);
  fCurrentNoteSoundingWholeNotesFromDuration = rational (-17, 1);
  
  fCurrentNoteDisplayWholeNotes         = rational (-25, 1);
  fCurrentNoteDisplayWholeNotesFromType = rational (-29, 1);
  
  fCurrentNoteDotsNumber = 0;
  
  fCurrentNoteGraphicDurationKind = k_NoDuration;

  fCurrentNoteOctave = K_NO_OCTAVE;

  fCurrentNoteQuarterTonesDisplayPitchKind = k_NoQuarterTonesPitch_QTP;
  fCurrentDisplayDiatonicPitchKind = k_NoDiatonicPitch;  
  fCurrentDisplayOctave = K_NO_OCTAVE;

  // rests
  
  fCurrentNoteIsARest = false;
  fCurrentRestMeasure = false;

  // unpitched notes
  
  fCurrentNoteIsUnpitched = false;

  // cue notes
  
  fCurrentNoteIsACueNote = false;

  // grace notes
  
  fCurrentNoteIsAGraceNote = false;

  // accidentals
  
  fCurrentNoteAccidentalKind =
    msrNote::kNoteAccidentalNone; // default value
    
  fCurrentNoteEditorialAccidentalKind =
    msrNote::kNoteEditorialAccidentalNo; // default value
    
  fCurrentNoteCautionaryAccidentalKind =
    msrNote::kNoteCautionaryAccidentalNo; // default value

  // current note staff number
  fCurrentMusicXMLStaffNumber = 1; // default value, it may be absent

  // current note voice number
  fCurrentMusicXMLVoiceNumber = 1; // default value, it may be absent

  // staff change detection
 // fCurrentStaffNumberToInsertInto = 1; JMI ???

  // time modification
  fCurrentNoteHasATimeModification = false;

  // tuplets
  
  fCurrentNoteBelongsToATuplet = false;

  fCurrentNoteActualNotes = -1;
  fCurrentNoteNormalNotes = -1;

  fCurrentTupletActualNumber = -1;
  fCurrentTupletActualType = "";
  fCurrentTupletActualDotsNumber = 0;
  
  fCurrentTupletNormalNumber = -1;
  fCurrentTupletNormalType = "";
  fCurrentTupletNormalDotsNumber = 0;

  // chords
  
  fCurrentNoteBelongsToAChord = false;


  // note lyrics

// JMI  fCurrentNoteSyllableExtendKind = kSyllableExtendNone;
}

//________________________________________________________________________
void mxmlTree2MsrTranslator::printVoicesLastMetNoteMap (
  int    inputLineNumber)
{
  int
    voicesLastMetNoteMapSize =
      fVoicesLastMetNoteMap.size ();
      
  fLogOutputStream <<
    endl <<
    "fVoicesLastMetNoteMap contains " <<
    singularOrPlural (
      voicesLastMetNoteMapSize, "element", "elements") <<
    ", line " << inputLineNumber <<
    ":" <<
    endl;
   
  if (voicesLastMetNoteMapSize) {
    gIndenter++;
    
//    map<S_msrVoice, S_msrNote>::const_iterator
    map<pair<int, int>, S_msrNote>::const_iterator
      iBegin = fVoicesLastMetNoteMap.begin (),
      iEnd   = fVoicesLastMetNoteMap.end (),
      i      = iBegin;
    for ( ; ; ) {
      fLogOutputStream <<
  //      (*i).first->getVoiceName () <<
        "staff " << (*i).first.first <<
        ", voice " <<  (*i).first.second <<
        ":" <<
        endl;

      gIndenter++;
      
      fLogOutputStream <<
        (*i).second <<
        endl;
        
      gIndenter--;

      if (++i == iEnd) break;
      
      fLogOutputStream <<
        endl;
    } // for

    gIndenter--;
  }
}

//________________________________________________________________________
void mxmlTree2MsrTranslator::checkStep (
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
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  else {
    stringstream s;
    
    s <<
      "root step value " << stepValue <<
      " should be a single letter from A to G";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
      s.str ());
  }
}

//______________________________________________________________________________
S_msrStaff mxmlTree2MsrTranslator::fetchStaffFromCurrentPart (
  int            inputLineNumber,
  int            staffNumber)
{    
  // fetch the staff from current part
  S_msrStaff
    staff =
      fCurrentPart->
        fetchStaffFromPart (staffNumber);

  // sanity check
  if (! staff) {
    stringstream s;

    s <<
      "staff '" << staffNumber <<
      "' not found in score skeleton's part " <<
      fCurrentPart->getPartCombinedName ();

    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceStaves) {
    fLogOutputStream <<
      "--> fetchStaffFromCurrentPart returns " <<
      staff->getStaffName () <<
      endl;
  }  
#endif

  return staff;
}  

//______________________________________________________________________________
S_msrVoice mxmlTree2MsrTranslator::fetchVoiceFromPart (
  int inputLineNumber,
  int staffNumber,
  int voiceNumber)
{
  // the voice number is relative to a part,
  // we'll call it its part-relative ID

  // fetch the staff from current part
  S_msrStaff
    staff =
      fetchStaffFromCurrentPart (
        inputLineNumber,
        staffNumber);

  // fetch the voice from the staff
  S_msrVoice
    voice =
      staff->
        fetchVoiceFromStaffByItsNumber (
          inputLineNumber,
          voiceNumber);

  // sanity check
  if (! staff) {
    stringstream s;

    s <<
      "voice '" << voiceNumber <<
      "' not found in score skeleton's staff \"" <<
      staff->getStaffName () <<
      "\"";

    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

    /* JMI
  // fetch registered voice displaying staff number
  int voiceDisplayingStaffNumber = 1; //K_NO_VOICE_NUMBER; JMI
    // default, there may be no <staff /> markups
    */

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices) {
    fLogOutputStream <<
      "--> fetchVoiceFromPart() returns " <<
      voice->getVoiceName () <<
      endl;
  }  
#endif

  return voice;
}

//________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart (S_part& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_part" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string partID = elt->getAttributeValue ("id");

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceParts || gTraceOptions->fTracePasses) {
    fLogOutputStream <<
      endl <<
      "<!--=== part \"" << partID << "\"" <<
      ", line " << inputLineNumber << " ===-->" <<
      endl;
  }
#endif

  // fetch current part from its partID
  fCurrentPart =
    fMsrScore->
      fetchPartFromScoreByItsPartID (
        inputLineNumber,
        partID);

  // sanity check
  if (! fCurrentPart) {
    // fetch fMsrScore's part list
    list<S_msrPart> partsList;

    fMsrScore->
      collectScorePartsList (
        inputLineNumber,
        partsList);
  
    if (partsList.size () == 1) {
      // there's only one part in the part list,
      // assume this is the one
      fCurrentPart =
        partsList.front ();
        
      partID =
        fCurrentPart->
          getPartID ();

      stringstream s;

      s <<
        "part 'id' is empty, using '" <<
        partID <<
        "' since it is the only part in the <part-list />";

      msrMusicXMLWarning (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        s.str ());
    }

    else {
      stringstream s;
  
      s <<
        "part \"" << partID <<
        "\" not found in score skeleton";
  
      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceParts) {
    fLogOutputStream <<
      "--------------------------------------------" <<
      endl <<
      "Analyzing part " <<
      fCurrentPart->
        getPartCombinedName () <<
        " -- start" <<
      endl;
  }
#endif

  // no time has been defined yet
  fCurrentTime = nullptr;

  // staff numbers
  fPreviousNoteMusicXMLStaffNumber = K_NO_STAFF_NUMBER;
  fCurrentMusicXMLStaffNumber  = K_NO_STAFF_NUMBER;
  
  // staff change detection
  fCurrentStaffNumberToInsertInto = 1; // default value JMI K_NO_STAFF_NUMBER;

  // cross staff chords
  fCurrentNoteIsCrossStaves = false;
    
  // get this part's staves map
  map<int, S_msrStaff>
    partStavesMap =
      fCurrentPart->
        getPartStavesMap ();

  // miscellaneous
  fCurrentMeasureNumber = "???";
  fCurrentMeasureOrdinalNumber = 0;
  
  fCurrentMusicXMLStaffNumber = K_NO_STAFF_NUMBER;
  fCurrentMusicXMLVoiceNumber = K_NO_VOICE_NUMBER;
  
  fCurrentEndingStartBarline = nullptr; // JMI

  fOnGoingRepeat = false;
  fOnGoingRepeatHasBeenCreated = false;

  gIndenter++;
}

void mxmlTree2MsrTranslator::visitEnd (S_part& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_part" <<
      ", line " << inputLineNumber <<
      endl;
  }

  gIndenter--;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceParts) {
    fLogOutputStream <<
      "Analyzing part " <<
      fCurrentPart->
        getPartCombinedName () <<
        " -- end" <<
      endl <<
      "--------------------------------------------" <<
      endl <<
      endl;
  }
#endif

  if (fOnGoingRepeat) {
    /*
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      "unterminated repeat in MusicXML data, exiting");
    */
    msrMusicXMLWarning (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      "unterminated repeat in MusicXML data, ignoring the repeat altogether");

    /* JMI
    // let's recover from this error

    // create an extra barline
    S_msrBarline
      barline =
        msrBarline::create (
          inputLineNumber,
          msrBarline::kBarlineCategoryRepeatEnd, // JMI
          msrBarline::kBarlineHasSegnoNo,
          msrBarline::kBarlineHasCodaNo,
          msrBarline::kBarlineLocationRight,
          msrBarline::kBarlineStyleNone,
          msrBarline::kBarlineEndingTypeStop,
          "0", // JMI
          msrBarline::kBarlineRepeatDirectionBackward,
          msrBarline::kBarlineRepeatWingedNone,
          2); // JMI
  
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceBarlines) {
      fLogOutputStream <<
        "Creating an extra barline in part " <<
        fCurrentPart->getPartCombinedName () << ":" <<
        endl;
        
      gIndenter++;
      
      fLogOutputStream <<
        barline;
        
      gIndenter--;
    }
#endif

    // handle it
  // JMI ???  handleRepeatEnd (barline);
    handleHooklessEndingEnd (barline);
  */
  }

  // finalize the current part
  fCurrentPart->
    finalizePart (
      inputLineNumber);
}

//________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart (S_attributes& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_attributes" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void mxmlTree2MsrTranslator::visitEnd (S_attributes& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_attributes" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // have the divisions been defined alright?
  if (! fCurrentDivisions) {
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      "no <divisions/> markup found in MusicXML data, exiting");
  }
  
  // JMI and if there's no <attributes/> ???
  // time is crucially needed for measures management,
  // we cannot stay without any
  if (! fCurrentTime) {
    /* JMI
    // create the default 4/4 time
    fCurrentTime =
      msrTime::createFourQuartersTime (
        inputLineNumber);
        
    // register time in staff
    fCurrentPart->
      appendTimeToPart (fCurrentTime);
      */
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_divisions& elt ) 
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_divisions" <<
      ", line " << inputLineNumber <<
      endl;
  }
    
  fCurrentDivisionsPerQuarterNote = (int)(*elt);
  
  if (
    fCurrentDivisionsPerQuarterNote < 1
      ||
    fCurrentDivisionsPerQuarterNote > 16383
  ) {
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      "divisions per quarter note should be between 1 and 16383");
  }

  // set current part's divisions per quarter note
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceDivisions) {
    if (fCurrentDivisionsPerQuarterNote == 1) {
      fLogOutputStream <<
        "There is 1 division";
    }
    else {
      fLogOutputStream <<
        "There are " <<
        fCurrentDivisionsPerQuarterNote <<
        " divisions";
    }
    
    fLogOutputStream <<
      " per quarter note in part " <<
      fCurrentPart->getPartCombinedName() <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create current divisions
  fCurrentDivisions =
    msrDivisions::create (
      inputLineNumber,
      fCurrentDivisionsPerQuarterNote);
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_clef& elt )
{ 
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_clef" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // https://usermanuals.musicxml.com/MusicXML/Content/EL-MusicXML-clef.htm

  // The optional number attribute refers to staff numbers.
  // If absent (0), apply to all part staves.
  
  fCurrentClefStaffNumber =
    elt->getAttributeIntValue ("number", 0); 

  fCurrentClefLine = 0;
  fCurrentClefOctaveChange = 0;
  fCurrentClefSign = "";
}

void mxmlTree2MsrTranslator::visitStart ( S_sign& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_sign" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentClefSign = elt->getValue();
}

void mxmlTree2MsrTranslator::visitStart ( S_line& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_line" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentClefLine = (int)(*elt);
}
  
void mxmlTree2MsrTranslator::visitStart ( S_clef_octave_change& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_clef_octave_change" <<
      ", line " << inputLineNumber <<
      endl;
  }

  fCurrentClefOctaveChange = (int)(*elt);

  if (fCurrentClefOctaveChange < -2 || fCurrentClefOctaveChange > 2) {
    stringstream s;
    
    s <<
      "clef-octave-change \"" << fCurrentClefOctaveChange <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }
}
  
void mxmlTree2MsrTranslator::visitEnd ( S_clef& elt ) 
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_clef" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // convert clef to upper case for analysis
  transform (
    fCurrentClefSign.begin (),
    fCurrentClefSign.end (),
    fCurrentClefSign.begin (),
    ::toupper);

  msrClef::msrClefKind clefKind = msrClef::k_NoClef;
  
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
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());    
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
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());    
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
            gXml2lyOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());    
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
            gXml2lyOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());    
        }
    } // switch
  }

  else if (fCurrentClefSign == "PERCUSSION") {
    clefKind = msrClef::kPercussionClef;
  }

  else if (fCurrentClefSign == "jianpu") {
    clefKind = msrClef::kJianpuClef;
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
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
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
        fetchStaffFromCurrentPart (
          inputLineNumber,
          fCurrentClefStaffNumber);
    
    staff->
      appendClefToStaff (clef);
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_key& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_key" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

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

  fCurrentHumdrumScotKeyItem = nullptr;
}
  
void mxmlTree2MsrTranslator::visitStart ( S_cancel& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_cancel" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentKeyCancelFifths = (int)(*elt);
}

void mxmlTree2MsrTranslator::visitStart ( S_fifths& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_fifths" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentKeyKind = msrKey::kTraditionalKind;

  fCurrentKeyFifths = (int)(*elt);
}

void mxmlTree2MsrTranslator::visitStart ( S_mode& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_mode" <<
      ", line " << inputLineNumber <<
      endl;
  }

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
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxmlTree2MsrTranslator::visitStart ( S_key_step& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_key_step" <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (fCurrentHumdrumScotKeyItem) {
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      "Humdrum/Scot key step found while another one is being handled");
  }
  
  fCurrentKeyKind = msrKey::kHumdrumScotKind;

  string step = elt->getValue();

  // check the step value
  checkStep (
    inputLineNumber,
    step);

  // determine diatonic pitch
  msrDiatonicPitchKind keyDiatonicPitchKind =
    msrDiatonicPitchKindFromString (step [0]);

  // create the Humdrum/Scot item
  fCurrentHumdrumScotKeyItem =
    msrHumdrumScotKeyItem::create (
      inputLineNumber);

  // populate it with the diatonic pitch
  fCurrentHumdrumScotKeyItem->
    setKeyItemDiatonicPitchKind (
      keyDiatonicPitchKind);

  // insert it into the items vector
  fCurrentHumdrumScotKeyItemsVector.
    insert (
      fCurrentHumdrumScotKeyItemsVector.end (),
      fCurrentHumdrumScotKeyItem);
}

void mxmlTree2MsrTranslator::visitStart ( S_key_alter& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_key_alter" <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (! fCurrentHumdrumScotKeyItem) {
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      "Humdrum/Scot key alter found while no key step is being handled");
  }
  
  float keyAlter = (float)(*elt);

  // determine the alteration
  msrAlterationKind
    keyAlterationKind =
      msrAlterationKindFromMusicXMLAlter (
        keyAlter);
      
  if (keyAlterationKind == k_NoAlteration) {
    stringstream s;

    s <<
      "key alter '" << keyAlter << "'"
      "' should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";
      
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // complement the current Humdrum/Scot item with the alteration
  fCurrentHumdrumScotKeyItem->
    setKeyItemAlterationKind (
      keyAlterationKind);

  // forget about this item
  fCurrentHumdrumScotKeyItem = nullptr;
}

void mxmlTree2MsrTranslator::visitStart ( S_key_octave& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_key_octave" <<
      ", line " << inputLineNumber <<
      endl;
  }

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
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  if (! item) { // JMI

    fLogOutputStream << "----------------" << endl;
    int counter = 0;
    for (
      vector<S_msrHumdrumScotKeyItem>::const_iterator i=
        fCurrentHumdrumScotKeyItemsVector.begin ();
      i!=fCurrentHumdrumScotKeyItemsVector.end ();
      i++) {
      fLogOutputStream <<
        ++counter << ": " << (*i) <<
        endl;
    } // for
    fLogOutputStream << "----------------" << endl;
      
    stringstream s;
    
    s <<
      "item " << number <<
      " not found in Humdrum/Scot key items";

    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
  
  // complement the item with the octave
  item->
    setKeyItemOctave (
      keyOctave);
}

void mxmlTree2MsrTranslator::visitEnd ( S_key& elt ) 
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_key" <<
      ", line " << inputLineNumber <<
      endl;
  }

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
        fetchStaffFromCurrentPart (
          inputLineNumber, fCurrentKeyStaffNumber);

    staff->
      appendKeyToStaff (key);
  }
}

S_msrKey mxmlTree2MsrTranslator::handleTraditionalKey (
  int inputLineNumber)
{
  // key fifths number
  msrQuarterTonesPitchKind
    keyTonicPitchKind =
      k_NoQuarterTonesPitch_QTP;
  
  switch (fCurrentKeyFifths) {
    case 0:
      keyTonicPitchKind = kC_Natural_QTP;
      break;
    case 1:
      keyTonicPitchKind = kG_Natural_QTP;
      break;
    case 2:
      keyTonicPitchKind = kD_Natural_QTP;
      break;
    case 3:
      keyTonicPitchKind = kA_Natural_QTP;
      break;
    case 4:
      keyTonicPitchKind = kE_Natural_QTP;
      break;
    case 5:
      keyTonicPitchKind = kB_Natural_QTP;
      break;
    case 6:
      keyTonicPitchKind = kF_Sharp_QTP;
      break;
    case 7:
      keyTonicPitchKind = kC_Sharp_QTP;
      break;
      
    case 8: // JMI
      keyTonicPitchKind = kG_Sharp_QTP;
      break;
    case 9:
      keyTonicPitchKind = kD_Sharp_QTP;
      break;
    case 10:
      keyTonicPitchKind = kA_Sharp_QTP;
      break;
    case 11:
      keyTonicPitchKind = kE_Sharp_QTP;
      break;
      
    case -1:
      keyTonicPitchKind = kF_Natural_QTP;
      break;
    case -2:
      keyTonicPitchKind = kB_Flat_QTP;
      break;
    case -3:
      keyTonicPitchKind = kE_Flat_QTP;
      break;
    case -4:
      keyTonicPitchKind = kA_Flat_QTP;
      break;
    case -5:
      keyTonicPitchKind = kD_Flat_QTP;
      break;
    case -6:
      keyTonicPitchKind = kG_Flat_QTP;
      break;
    case -7:
      keyTonicPitchKind = kC_Flat_QTP;
      break;

    case -8: // JMI
      keyTonicPitchKind = kF_Flat_QTP;
      break;
    case -9:
      keyTonicPitchKind = kB_DoubleFlat_QTP;
      break;
    case -10:
      keyTonicPitchKind = kE_DoubleFlat_QTP;
      break;
    case -11:
      keyTonicPitchKind = kA_DoubleFlat_QTP;
      break;
      
    default: // unknown key sign!!
      {
      stringstream s;
      
      s << 
        "unknown key fifths number \"" << fCurrentKeyFifths << "\"";
        
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
      }
  } // switch

  // create the key
  S_msrKey
    key =
      msrKey::createTraditional (
        inputLineNumber,
        keyTonicPitchKind,
        fCurrentKeyModeKind,
        fCurrentKeyCancelFifths);

  // return it
  return key;
}

S_msrKey mxmlTree2MsrTranslator::handleHumdrumScotKey (
  int inputLineNumber)
{
 //  msrQuarterTonesPitch fCurrentNoteQuarterTonesPitch; // JMI BOF

  fCurrentNoteQuarterTonesPitchKind =
    quarterTonesPitchKindFromDiatonicPitchAndAlteration (
      inputLineNumber,
      fCurrentNoteDiatonicPitchKind,
      fCurrentNoteAlterationKind);

  // create the key
  S_msrKey
    key =
      msrKey::createHumdrumScot (
        inputLineNumber);

  // populate the key with the Humdrum/Scot items  
  if (fCurrentHumdrumScotKeyItemsVector.size ()) {
    for (
      vector<S_msrHumdrumScotKeyItem>::const_iterator i=
        fCurrentHumdrumScotKeyItemsVector.begin ();
      i!=fCurrentHumdrumScotKeyItemsVector.end ();
      i++) {
      key->
        appendHumdrumScotKeyItem ((*i));
    } // for

    fCurrentHumdrumScotKeyItemsVector.clear ();
  }
  
  else
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      "Humdrum/Scot key is empty");
  
  // return it
  return key;
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_time& elt )
{  
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_time" <<
      ", line " << inputLineNumber <<
      endl;
  }
  
  fCurrentTimeStaffNumber =
    elt->getAttributeIntValue ("number", 0);
    
  string timeSymbol =
    elt->getAttributeValue ("symbol");

  fCurrentTimeSymbolKind =
    msrTime::kTimeSymbolNone; // default value
  
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
  // \numericTimeSignature par default si pas de symbol // JMI
  
  else {
    if (timeSymbol.size ()) {
      stringstream s;
      
      s <<
        "time symbol " << timeSymbol << " is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  fCurrentTimeBeats = "";
  
  fOnGoingInterchangeable = false;
}

void mxmlTree2MsrTranslator::visitStart ( S_beats& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_beats" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentTimeBeats = elt->getValue (); // can be a string such as 3+2
}
  
void mxmlTree2MsrTranslator::visitStart ( S_beat_type& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_beat_type" <<
      ", line " << inputLineNumber <<
      endl;
  }

  int beatType = (int)(*elt);

  // extract the numbers list from the beat type
  list<int>
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
      list<int>::const_iterator i = beatNumbers.begin ();
      i != beatNumbers.end ();
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
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      "beat type doesn't contain any beats numbers");
  }

  // append the time item to the current time items vector
  fCurrentTimeItemsVector.insert (
    fCurrentTimeItemsVector.end (),
    timeItem);
}
 
void mxmlTree2MsrTranslator::visitStart ( S_senza_misura& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_senza_misura" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentTimeSymbolKind = msrTime::kTimeSymbolSenzaMisura;
}

void mxmlTree2MsrTranslator::visitStart ( S_interchangeable& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_interchangeable" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string interchangeableSymbol =
    elt->getAttributeValue ("symbol");

  fCurrentInterchangeableSymbolKind =
    msrTime::kTimeSymbolNone; // default value
  
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
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  string interchangeableSeparator =
    elt->getAttributeValue ("separator");

  fCurrentInterchangeableSeparatorKind =
    msrTime::kTimeSeparatorNone; // default value
  
  if       (interchangeableSymbol == "none") {
    fCurrentInterchangeableSeparatorKind = msrTime::kTimeSeparatorNone;
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
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  fOnGoingInterchangeable = true;
}

void mxmlTree2MsrTranslator::visitStart ( S_time_relation& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_time_relation" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string timeRelation = elt->getValue ();
  
  fCurrentInterchangeableRelationKind =
    msrTime::kTimeRelationNone; // default value
  
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
        "time-relation " << timeRelation << " is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }
}

void mxmlTree2MsrTranslator::visitEnd ( S_time& elt ) 
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_time" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // create the time
  fCurrentTime =
    msrTime::create (
      inputLineNumber,
      fCurrentTimeSymbolKind);
        
  // populate the time with the time items  
  if (fCurrentTimeItemsVector.size ()) {
    for (
      vector<S_msrTimeItem>::const_iterator i =
        fCurrentTimeItemsVector.begin ();
      i!=fCurrentTimeItemsVector.end ();
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
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
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
        fetchStaffFromCurrentPart (
          inputLineNumber, fCurrentTimeStaffNumber);

    staff->
      appendTimeToStaff (fCurrentTime);
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_instruments& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_time" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

/*
        <instruments>2</instruments>
*/

//  int instruments = (int)(*elt); // JMI
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_transpose& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_transpose" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentTransposeNumber = elt->getAttributeIntValue ("number", 0);
  
  fCurrentTransposeDiatonic     = 0;
  fCurrentTransposeChromatic    = 0;
  fCurrentTransposeOctaveChange = 0;
  fCurrentTransposeDouble       = false;
}

void mxmlTree2MsrTranslator::visitStart ( S_diatonic& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_diatonic" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentTransposeDiatonic = (int)(*elt);
}
  
void mxmlTree2MsrTranslator::visitStart ( S_chromatic& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_chromatic" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentTransposeChromatic = (int)(*elt);
}
 
void mxmlTree2MsrTranslator::visitStart ( S_octave_change& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting octave_change" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentTransposeOctaveChange = (int)(*elt);
}
 
void mxmlTree2MsrTranslator::visitStart ( S_double& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting double" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentTransposeDouble = true;
}
 
void mxmlTree2MsrTranslator::visitEnd ( S_transpose& elt ) 
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_transpose" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // bring the transpose chromatic value in the -11..+11 interval
  if (fCurrentTransposeChromatic < -11) {
    int
      auxTransposeChromatic =
        fCurrentTransposeChromatic,
      octaveOffset = 0;
    
    while (auxTransposeChromatic < -11) {
      auxTransposeChromatic += 12;
      octaveOffset++;
    } // while
  
    stringstream s;

    s <<
      "transpose: augmenting chromatic " <<
      fCurrentTransposeChromatic <<
      " to " << auxTransposeChromatic <<
      " and decrementing octave change by " << octaveOffset;

    msrMusicXMLWarning (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      s.str ());

    fCurrentTransposeChromatic    =  auxTransposeChromatic;
    fCurrentTransposeOctaveChange -= octaveOffset;
  }
  
  else if (fCurrentTransposeChromatic > 11) {
    int
      auxTransposeChromatic =
        fCurrentTransposeChromatic,
      octaveOffset = 0;
        
    while (auxTransposeChromatic > 11) {
      auxTransposeChromatic -= 12;
      octaveOffset++;
    } // while
  
    stringstream s;

    s <<
      "transpose: diminishing  chromatic to " <<
      fCurrentTransposeChromatic <<
      " to " << auxTransposeChromatic <<
      " and incrementing octave change by " << octaveOffset;

    msrMusicXMLWarning (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      s.str ());

    fCurrentTransposeChromatic    =  auxTransposeChromatic;
    fCurrentTransposeOctaveChange += octaveOffset;
  }
  
  // create msrTranspose
  S_msrTranspose
    transpose =
      msrTranspose::create (
        inputLineNumber,
        fCurrentTransposeDiatonic,
        fCurrentTransposeChromatic,
        fCurrentTransposeOctaveChange,
        fCurrentTransposeDouble);

  if (fCurrentTransposeNumber == 0)
    fCurrentPart->
      appendTransposeToPart (transpose);
    
  else {
    S_msrStaff
      staff =
        fetchStaffFromCurrentPart (
          inputLineNumber, fCurrentTransposeNumber);

    staff->
      appendTransposeToStaff (transpose);
  }
}

//________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart (S_direction& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_direction" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string directionPlacementString =
    elt->getAttributeValue ("placement");

  fCurrentDirectionPlacementKind = kPlacementNone;
  
  if      (directionPlacementString == "above")
    fCurrentDirectionPlacementKind = kPlacementAbove;
  else if (directionPlacementString == "below")
    fCurrentDirectionPlacementKind = kPlacementBelow;
  else if (directionPlacementString.size ()) {
    stringstream s;
    
    s <<
      "direction placement \"" << directionPlacementString <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  fCurrentMetronomeWords = nullptr;
  fCurrentMetronomeTempo = nullptr;

  fOnGoingDirection = true;
}

void mxmlTree2MsrTranslator::visitEnd (S_direction& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_direction" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

/* JMI
  if (fCurrentMetronomeTempo) {
    int pendingWordsSize = fPendingWords.size ();
    
    if (pendingWordsSize) {
      if (pendingWordsSize > 1) {
        stringstream s;
    
        s <<
          "<direction/> contains " <<
          pendingWordsSize <<
          " <words/> markups";
          
        msrMusicXMLWarning (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          s.str ());
      }

      // handling only the first of the pending words
      string words =
        fPendingWords.front ();
        
      // register words in current metronome tempo
      fCurrentMetronomeTempo->
        setTempoWords (words);

      // forget about this words
      fPendingWords.pop_front ();
    }
  }
*/

  fOnGoingDirection = false;
}

void mxmlTree2MsrTranslator::visitStart (S_direction_type& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_direction_type" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fOnGoingDirectionType = true; // JMI
}

//________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart (S_offset& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_offset" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // sound
  
  string offsetSound =elt->getAttributeValue ("sound");

  bool offsetSoundValue; // JMI
  
  if (offsetSound.size ()) {    
    if (offsetSound == "yes")
      offsetSoundValue = true;
      
    else if (offsetSound == "no")
      offsetSoundValue = false;
      
    else {
      stringstream s;
      
      s <<
        "offset sound value " << offsetSound <<
        " should be 'yes' or 'no'";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  if (false && offsetSoundValue) {
    // JMI
  }
  
  if (fOnGoingDirection) { // JMI
  }
}

//________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart (S_octave_shift& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_octave_shift" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // size
  
  string octaveShiftSizeString = elt->getAttributeValue ("size");
  int    octaveShiftSize = 8;

  if (! octaveShiftSizeString.size ()) {
    stringstream s;

    s <<
      "octave shift size absent, assuming 8";
      
    msrMusicXMLWarning (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      s.str ());
  }
  
  else {
    istringstream inputStream (octaveShiftSizeString);
  
    inputStream >> octaveShiftSize;
    
    if (
      octaveShiftSize != 8
        &&
      octaveShiftSize != 15
      /* JMI allow larger values???
        &&
      octaveShiftSize != 22
        &&
      octaveShiftSize != 27
      */
      ) {
      stringstream s;
  
      s <<
        "octave-shift size \"" << octaveShiftSize <<
    // JMI    "\" should be 8, 15, 22 or 27";
        "\" is wrong, should be 8 or 15";
        
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // type
  
  string type = elt->getAttributeValue ("type");

  msrOctaveShift::msrOctaveShiftKind
    octaveShiftKind = msrOctaveShift::kOctaveShiftNone;
  
  if      (type == "up")
    octaveShiftKind = msrOctaveShift::kOctaveShiftUp;
  else if (type == "down")
    octaveShiftKind = msrOctaveShift::kOctaveShiftDown;
  else if (type == "stop")
    octaveShiftKind = msrOctaveShift::kOctaveShiftStop;
  else if (type == "continue")
    octaveShiftKind = msrOctaveShift::kOctaveShiftContinue;
    
  else {
    stringstream s;
    
    s <<
      "octave-shift type \"" << type <<
      "\"" << "is unknown";
      
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create an octave shift
  S_msrOctaveShift
    octaveShift =
      msrOctaveShift::create (
        inputLineNumber,
        octaveShiftKind,
        octaveShiftSize);

  // append the octave shift to the pending octave shifts list
  fPendingOctaveShifts.push_back (octaveShift);
}

//________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart (S_words& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_words" <<
      ", line " << inputLineNumber <<
      endl;
  }

/*
  <words default-y="-73" font-style="italic" relative-x="5">cresc.</words>
*/

  string wordsValue = elt->getValue ();

  // justify

  string wordsJustify = elt->getAttributeValue ("justify");

  msrJustifyKind justifyKind = kJustifyNone; // default value

  if      (wordsJustify == "left")
    justifyKind = kJustifyLeft;
  else if (wordsJustify == "center")
    justifyKind = kJustifyCenter;
  else if (wordsJustify == "right")
    justifyKind = kJustifyRight;
  else {
    if (wordsJustify.size ()) {
      stringstream s;
      
      s <<
        "justify value " << wordsJustify <<
        " should be 'left', 'center' or 'right'";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // valign

  string wordsVerticalAlignment = elt->getAttributeValue ("valign");

  msrVerticalAlignmentKind
    verticalAlignmentKind = kVerticalAlignmentNone; // default value

  if      (wordsVerticalAlignment == "top")
    verticalAlignmentKind = kVerticalAlignmentTop;
  else if (wordsVerticalAlignment == "middle")
    verticalAlignmentKind = kVerticalAlignmentMiddle;
  else if (wordsVerticalAlignment == "bottom")
    verticalAlignmentKind = kVerticalAlignmentBottom;
  else {
    if (wordsVerticalAlignment.size ()) {
      stringstream s;
      
      s <<
        "valign value " << wordsVerticalAlignment <<
        " should be 'top', 'middle' or 'bottom'";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // font style

  string wordsFontStyle = elt->getAttributeValue ("font-style");

  msrFontStyleKind fontStyleKind = kFontStyleNone; // default value

  if      (wordsFontStyle == "normal")
    fontStyleKind = kFontStyleNormal;
  else if (wordsFontStyle == "italic")
    fontStyleKind = KFontStyleItalic;
  else {
    if (wordsFontStyle.size ()) {
      stringstream s;
      
      s <<
        "font-style value " << wordsFontStyle <<
        " should be 'normal' or 'italic'";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // font size

  string wordsFontSize = elt->getAttributeValue ("font-size");
  
  msrFontSize::msrFontSizeKind
    fontSizeKind =
      msrFontSize::kFontSizeNone; // default value

  float fontSizeFloatValue = 0.0;

  if      (wordsFontSize == "xx-smal")
    fontSizeKind = msrFontSize::kFontSizeXXSmall;
  else if (wordsFontSize == "x-small")
    fontSizeKind = msrFontSize::kFontSizeXSmall;
  else if (wordsFontSize == "small")
    fontSizeKind = msrFontSize::kFontSizeSmall;
  else if (wordsFontSize == "medium")
    fontSizeKind = msrFontSize::kFontSizeMedium;
  else if (wordsFontSize == "large")
    fontSizeKind = msrFontSize::kFontSizeLarge;
  else if (wordsFontSize == "x-large")
    fontSizeKind = msrFontSize::kFontSizeXLarge;
  else if (wordsFontSize == "xx-large")
    fontSizeKind = msrFontSize::kFontSizeXXLarge;
  else {
    elt->getAttributeFloatValue ("font-size", 0.0);
    fontSizeKind = msrFontSize::kFontSizeNumeric;
  }

  S_msrFontSize fontSize;

  switch (fontSizeKind) {
    case msrFontSize::kFontSizeNone:
    case msrFontSize::kFontSizeXXSmall:
    case msrFontSize::kFontSizeXSmall:
    case msrFontSize::kFontSizeSmall:
    case msrFontSize::kFontSizeMedium:
    case msrFontSize::kFontSizeLarge:
    case msrFontSize::kFontSizeXLarge:
    case msrFontSize::kFontSizeXXLarge:
      fontSize =
        msrFontSize::create (fontSizeKind);
      break;
      
    case msrFontSize::kFontSizeNumeric:
      fontSize =
        msrFontSize::create (fontSizeFloatValue);
      break;
    } // switch
    
  // font weight

  string wordsFontWeight = elt->getAttributeValue ("font-weight");
  
  msrFontWeightKind fontWeightKind = kFontWeightNone; // default value

  if      (wordsFontWeight == "normal")
    fontWeightKind = kFontWeightNormal;
  else if (wordsFontWeight == "bold")
    fontWeightKind = kFontWeightBold;
  else {
    if (wordsFontWeight.size ()) {
      stringstream s;
      
      s <<
        "font-weight value " << wordsFontWeight <<
        " should be 'normal' or 'bold'";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }
  
  // XML language

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
  else if (wordsXMLLang == "la")
    wordsXMLLangKind = msrWords::kLaLang;
  else {
    if (wordsXMLLang.size ()) {
      stringstream s;
      
      s <<
        "xml:lang value '" << wordsXMLLang <<
        "' should be 'it', 'en', 'de' or 'fr'";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // create the words
  if (wordsValue.size ()) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceWords) {
      fLogOutputStream <<
        "Creating words \"" << wordsValue << "\"" <<
        ", placement = \"" <<
        msrPlacementKindAsString (
          fCurrentDirectionPlacementKind) << "\"" <<
        endl;
    }
#endif

    S_msrWords
      words =
        msrWords::create (
          inputLineNumber,
          fCurrentDirectionPlacementKind,
          wordsValue,
          justifyKind,
          verticalAlignmentKind,
          fontStyleKind,
          fontSize,
          fontWeightKind,
          wordsXMLLangKind);

    fPendingWords.push_back (words);
  }
}

void mxmlTree2MsrTranslator::visitStart ( S_accordion_registration& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_accordion_registration" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

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

void mxmlTree2MsrTranslator::visitStart ( S_accordion_high& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_accordion_high" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentAccordionHigh = 1;

  fCurrentAccordionNumbersCounter++;
}

void mxmlTree2MsrTranslator::visitStart ( S_accordion_middle& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_accordion_middle" <<
       ", line " << inputLineNumber <<
     endl;
  }

  fCurrentAccordionMiddle = (int)(*elt);

  if (fCurrentAccordionMiddle < 1 || fCurrentAccordionMiddle > 3) {
    stringstream s;
    
    s <<
      "accordion middle " <<
      fCurrentAccordionMiddle << " should be 1, 2 or 3" <<
      ", replaced by 1";
    
    msrMusicXMLWarning (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      s.str ());

    fCurrentAccordionMiddle = 1;
  }

  fCurrentAccordionNumbersCounter++;
}

void mxmlTree2MsrTranslator::visitStart ( S_accordion_low& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_accordion_low" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentAccordionLow = 1;

  fCurrentAccordionNumbersCounter++;
}

void mxmlTree2MsrTranslator::visitEnd ( S_accordion_registration& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_accordion_registration" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // An accordion-registration element needs to have 
  // at least one of the child elements present

  if (fCurrentAccordionNumbersCounter == 0) {
    msrMusicXMLWarning (
      gXml2lyOptions->fInputSourceName,
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

void mxmlTree2MsrTranslator::visitEnd (S_direction_type& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_direction_type" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fOnGoingDirectionType = false;
}

//________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_metronome& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_metronome" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string parentheses = elt->getAttributeValue ("parentheses");
  
  if (parentheses.size ()) {    
    if (parentheses == "yes")
      fCurrentMetronomeParenthesedKind = msrTempo::kTempoParenthesizedYes;
    else if (parentheses == "no")
      fCurrentMetronomeParenthesedKind = msrTempo::kTempoParenthesizedNo;
    else {
      stringstream s;
      
      s <<
        "parentheses value " << parentheses <<
        " should be 'yes' or 'no'";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  fCurrentMetronomeBeatUnitsVector.clear();
  
  fCurrentMetrenomePerMinute = "";

  fCurrentMetrenomeDotsNumber = 0;
  fCurrentMetrenomeRelationKind = msrTempo::kTempoRelationNone;
  fCurrentMetronomeDurationKind = k_NoDuration;
  fCurrentMetronomeBeamValue = "";

  fOnGoingMetronomeTuplet = false;
  fCurrentMetrenomeNormalDotsNumber = 0;

  fCurrentMetronomeRelationLeftElements  = nullptr;
  fCurrentMetronomeRelationRightElements = nullptr;

  fCurrentMetronomeTuplet = nullptr;
}
  
void mxmlTree2MsrTranslator::visitStart ( S_beat_unit& elt )
{ 
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_beat_unit" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string beatUnitString = elt->getValue();
   
  // the type contains a display duration
  msrDurationKind
    beatUnitDurationKind =
      msrDurationKindFromString (
        inputLineNumber,
        beatUnitString);

  // there can be several <beat-unit/> in a <metronome/> markup,
  // register beat unit in in dotted durations list
  fCurrentMetronomeBeatUnitsVector.push_back (
    msrDottedDuration (
      beatUnitDurationKind,
      0));
}

void mxmlTree2MsrTranslator::visitStart ( S_beat_unit_dot& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_beat_unit_dot" <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (fCurrentMetronomeBeatUnitsVector.size ()) {
    fCurrentMetronomeBeatUnitsVector.back ().fDotsNumber++;
  }
  else {
    stringstream s;
    
    s <<
      "beat unit dot occurs without prior beat unit";

    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}
  
void mxmlTree2MsrTranslator::visitStart ( S_per_minute& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_per_minute" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentMetrenomePerMinute = elt->getValue ();
}

void mxmlTree2MsrTranslator::visitStart ( S_metronome_note& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_metronome_note" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fOnGoingMetronomeNote = true;
}

void mxmlTree2MsrTranslator::visitStart ( S_metronome_type& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_metronome_type" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string noteType = elt->getValue();

  // the type contains a display duration,
  fCurrentMetronomeDurationKind =
    msrDurationKindFromString (
     inputLineNumber,
     noteType);
}

void mxmlTree2MsrTranslator::visitStart ( S_metronome_dot& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_metronome_dot" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentMetrenomeDotsNumber++;
}

void mxmlTree2MsrTranslator::visitStart ( S_metronome_beam& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_metronome_beam" <<
      ", line " << inputLineNumber <<
     endl;
  }

  // value
  
  fCurrentMetronomeBeamValue = elt->getValue();

  msrBeam::msrBeamKind beamKind = msrBeam::k_NoBeam;

  if      (fCurrentMetronomeBeamValue == "begin") {
    beamKind = msrBeam::kBeginBeam;
  }
  else if (fCurrentMetronomeBeamValue == "continue") {
    beamKind = msrBeam::kContinueBeam;
  }
  else if (fCurrentMetronomeBeamValue == "end") {
    beamKind = msrBeam::kEndBeam;
  }
  else if (fCurrentMetronomeBeamValue == "forward hook") {
    beamKind = msrBeam::kForwardHookBeam;
  }
  else if (fCurrentMetronomeBeamValue == "backward hook") {
    beamKind = msrBeam::kBackwardHookBeam;
  }
  else {
    stringstream s;
    
    s <<
      "metronome beam \"" << fCurrentMetronomeBeamValue <<
      "\"" << "is not known";
      
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
    
  // number
  
  fCurrentBeamNumber = 
    elt->getAttributeIntValue ("number", 0);

  // create metronome note beam
  S_msrBeam
    beam =
      msrBeam::create (
        inputLineNumber,
        fCurrentBeamNumber,
        beamKind);

  // register it
  fPendingMetronomeBeams.push_back (beam);
}

void mxmlTree2MsrTranslator::attachCurrentMetronomeBeamsToMetronomeNote (
  S_msrTempoNote tempoNote)
{
  // attach the current articulations if any to the note
  if (fPendingMetronomeBeams.size ()) {

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceBeams) {
      fLogOutputStream <<
        "Attaching current beams to tempoNote " <<
        tempoNote->asString () <<
        endl;
    }
#endif

    while (fPendingMetronomeBeams.size ()) {
      S_msrBeam
        beam =
          fPendingMetronomeBeams.front();
        
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceBeams) {
        fLogOutputStream <<
          "Attaching beam '" <<
          beam->asString () <<
          "' to tempoNote '" << tempoNote->asString () << "'" <<
          endl;
      }
#endif
  
      tempoNote->
        appendBeamToTempoNote (beam);

      // forget about this articulation
      fPendingMetronomeBeams.pop_front();
    } // while
  }
}

void mxmlTree2MsrTranslator::visitEnd ( S_metronome_note& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_metronome_note" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // convert metronome note duration into whole notes
  fCurrentMetronomeNoteWholeNotesFromMetronomeType =
    msrDurationKindAsWholeNotes (
      fCurrentMetronomeDurationKind);

  // take metronome dots into account if any
  if (fCurrentNoteDotsNumber > 0) {
    int dots = fCurrentMetrenomeDotsNumber;

    while (dots > 0) {
      fCurrentMetronomeNoteWholeNotesFromMetronomeType *=
        rational (3, 2);
      fCurrentMetronomeNoteWholeNotesFromMetronomeType.rationalise ();

      dots--;
    } // while
  }

  // create the metronome note
  S_msrTempoNote
    tempoNote =
      msrTempoNote::create (
        inputLineNumber,
        fCurrentMetronomeNoteWholeNotesFromMetronomeType,
        false /* tempoNoteBelongsToATuplet JMI */);
  
  // attach beams if any to metronome note
  if (fPendingMetronomeBeams.size ()) {
    attachCurrentMetronomeBeamsToMetronomeNote (
      tempoNote);
  }
  
  if (fCurrentMetronomeTuplet) {
    // register metronome note as metronome tuplet member
    
    fCurrentMetronomeTuplet->
      addTempoNoteToTempoTuplet (
        tempoNote);
  }

  else {
    // register stand-alone metronome note
    
    if (fCurrentMetrenomeRelationKind == msrTempo::kTempoRelationNone) {
      // this metronome note belongs to the left elements list
  
      if (! fCurrentMetronomeRelationLeftElements) {
        // create the relation left elements
        fCurrentMetronomeRelationLeftElements =
          msrTempoRelationshipElements::create (
            inputLineNumber,
            msrTempoRelationshipElements::kTempoRelationshipElementsLeft);
      }
      
      fCurrentMetronomeRelationLeftElements->
        addElementToTempoRelationshipElements (
          tempoNote);
    }
    else {
      // this metronome note belongs to the right elements list
  
      if (! fCurrentMetronomeRelationRightElements) {
        // create the relation right elements
        fCurrentMetronomeRelationRightElements =
          msrTempoRelationshipElements::create (
            inputLineNumber,
            msrTempoRelationshipElements::kTempoRelationshipElementsRight);
      }
      
      fCurrentMetronomeRelationRightElements->
        addElementToTempoRelationshipElements (
          tempoNote);
    }
  }

  fOnGoingMetronomeNote = false;
}

void mxmlTree2MsrTranslator::visitStart ( S_metronome_relation& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_metronome_relation" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string metrenomeRelation = elt->getValue ();

  fCurrentMetrenomeRelationKind = msrTempo::kTempoRelationNone;

  if (metrenomeRelation == "equals") {
    fCurrentMetrenomeRelationKind = msrTempo::kTempoRelationEquals; }
  else {
    stringstream s;
    
    s <<
      "metronome relation \"" << metrenomeRelation <<
      "\" is unknown";

    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxmlTree2MsrTranslator::visitStart ( S_metronome_tuplet& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_metronome_tuplet" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // number

  fCurrentTupletNumber = elt->getAttributeIntValue ("number", 0);

  // bracket

  {
    string tupletBracket = elt->getAttributeValue ("bracket");
      
    fCurrentTempoTupletBracketKind = msrTempoTuplet::kTempoTupletBracketYes; // option ??? JMI
    
    if      (tupletBracket == "yes")
      fCurrentTempoTupletBracketKind = msrTempoTuplet::kTempoTupletBracketYes;
    else if (tupletBracket == "no")
      fCurrentTempoTupletBracketKind = msrTempoTuplet::kTempoTupletBracketNo;
    else {
      if (tupletBracket.size ()) {
        stringstream s;
        
        s <<
          "tuplet bracket \"" << tupletBracket <<
          "\" is unknown";
        
        msrMusicXMLError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      else {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceTuplets) {
          stringstream s;
          
          s <<
            "tuplet bracket is empty: this is implementation dependent," <<
            " \"yes\" is assumed"; // option ??? JMI
          
          msrMusicXMLWarning (
            gXml2lyOptions->fInputSourceName,
            inputLineNumber,
            s.str ());
        }
#endif
      }
    }
  }

  // type

  {
    string tupletType = elt->getAttributeValue ("type");
      
    fCurrentTempoTupletTypeKind = msrTempoTuplet::kTempoTupletTypeNone;
    
    if      (tupletType == "start")
      fCurrentTempoTupletTypeKind = msrTempoTuplet::kTempoTupletTypeStart;
    else if (tupletType == "stop")
      fCurrentTempoTupletTypeKind = msrTempoTuplet::kTempoTupletTypeStop;
    else {
      stringstream s;
      
      s <<
        "tuplet type \"" << tupletType <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // show-number

  {
    string tupletShowNumber = elt->getAttributeValue ("show-number");
  
    fCurrentTempoTupletShowNumberKind =
      msrTempoTuplet::kTempoTupletShowNumberActual; // default value
    
    if      (tupletShowNumber == "actual") {
      fCurrentTempoTupletShowNumberKind = msrTempoTuplet::kTempoTupletShowNumberActual;
    }
    else if (tupletShowNumber == "both") {
      fCurrentTempoTupletShowNumberKind = msrTempoTuplet::kTempoTupletShowNumberBoth;
    }
    else if (tupletShowNumber == "none") {
      fCurrentTempoTupletShowNumberKind = msrTempoTuplet::kTempoTupletShowNumberNone;
    }
    else {
      if (tupletShowNumber.size ()) {
        msrMusicXMLError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          "tuplet show-number \"" + tupletShowNumber + "\" is unknown");
      }
    }
  }
  
#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceNotesDetails
      ||
    gTraceOptions->fTraceTuplets) {
    fLogOutputStream <<
      "fCurrentTempoTupletTypeKind: " <<
      msrTempoTuplet::tempoTupletTypeKindAsString (
        fCurrentTempoTupletTypeKind) <<
      "fCurrentTempoTupletBracketKind: " <<
      msrTempoTuplet::tempoTupletBracketKindAsString (
        fCurrentTempoTupletBracketKind) <<
      "fCurrentTempoTupletShowNumberKind: " <<
      msrTempoTuplet::tempoTupletShowNumberKindAsString (
        fCurrentTempoTupletShowNumberKind) <<
      endl;
  }
#endif

  fOnGoingMetronomeTuplet = true;
}

void mxmlTree2MsrTranslator::visitStart ( S_normal_dot& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_normal_dot" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentMetrenomeNormalDotsNumber++;
}

void mxmlTree2MsrTranslator::visitEnd ( S_metronome_tuplet& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_metronome_tuplet" <<
      ", line " << inputLineNumber <<
      endl;
  }

  switch (fCurrentTempoTupletTypeKind) {
    case msrTempoTuplet::kTempoTupletTypeNone:
      break;
      
    case msrTempoTuplet::kTempoTupletTypeStart:
      // create metronome tuplet
      fCurrentMetronomeTuplet =
        msrTempoTuplet::create (
          inputLineNumber,
          fCurrentTempoTupletNumber,
          fCurrentTempoTupletBracketKind,
          fCurrentTempoTupletShowNumberKind,
          fCurrentMetronomeNoteActualNotes,
          fCurrentMetronomeNoteNormalNotes,
          fCurrentMetronomeNoteWholeNotesFromMetronomeType);
    
      // register the metronome tuplet 
      if (fCurrentMetrenomeRelationKind == msrTempo::kTempoRelationNone) {
        // this metronome tuplet belongs to the left elements list
    
        if (! fCurrentMetronomeRelationLeftElements) {
          // create the relation left elements
          fCurrentMetronomeRelationLeftElements =
            msrTempoRelationshipElements::create (
              inputLineNumber,
              msrTempoRelationshipElements::kTempoRelationshipElementsLeft);
        }
        
        fCurrentMetronomeRelationLeftElements->
          addElementToTempoRelationshipElements (
            fCurrentMetronomeTuplet);
      }
      else {
        // this metronome tuplet belongs to the right elements list
    
        if (! fCurrentMetronomeRelationRightElements) {
          // create the relation right elements
          fCurrentMetronomeRelationRightElements =
            msrTempoRelationshipElements::create (
              inputLineNumber,
              msrTempoRelationshipElements::kTempoRelationshipElementsRight);
        }
        
        fCurrentMetronomeRelationRightElements->
          addElementToTempoRelationshipElements (
            fCurrentMetronomeTuplet);
      }
      break;
      
    case msrTempoTuplet::kTempoTupletTypeStop:
      // don't set fCurrentMetronomeTuplet to nullptr here,
      // it will be needed for the current metronome note a bit later
      break;
  } // switch

  fOnGoingMetronomeTuplet = false;
}

void mxmlTree2MsrTranslator::visitEnd ( S_metronome& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_metronome" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // fCurrentMetronomeParenthesedKind ??? JMI

  // determine tempo kind
  msrTempo::msrTempoKind tempoKind = msrTempo::k_NoTempoKind;

  int  beatUnitsSize    = fCurrentMetronomeBeatUnitsVector.size ();
  bool perMinutePresent = fCurrentMetrenomePerMinute.size () > 0;
  
  if (beatUnitsSize == 1 && perMinutePresent) {
    tempoKind = msrTempo::kTempoBeatUnitsPerMinute;
  }
  else if (beatUnitsSize == 2 && ! perMinutePresent) {
    tempoKind = msrTempo::kTempoBeatUnitsEquivalence;
  }
  else if (fCurrentMetrenomeRelationKind != msrTempo::kTempoRelationNone) {
    tempoKind = msrTempo::kTempoNotesRelationShip;
  }

  if (tempoKind == msrTempo::k_NoTempoKind) {
      stringstream s;
  
      s <<
        "ill-formed <metronome/> markup: there are " <<
        fCurrentMetronomeBeatUnitsVector.size () <<
        " beat units";
        
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  // create the tempo
  S_msrTempo tempo;
    
  switch (tempoKind) {
    case msrTempo::k_NoTempoKind:
      break;

    case msrTempo::kTempoBeatUnitsPerMinute:
      {
        msrDottedDuration
          beatUnits =
            fCurrentMetronomeBeatUnitsVector [0];
  
        tempo =
          msrTempo::create (
            inputLineNumber,
            beatUnits,
            fCurrentMetrenomePerMinute,
            fCurrentMetronomeParenthesedKind,
            fCurrentDirectionPlacementKind);
        }
      break;

    case msrTempo::kTempoBeatUnitsEquivalence:
      {
        msrDottedDuration
          beatUnits =
            fCurrentMetronomeBeatUnitsVector [0];
  
        tempo =
          msrTempo::create (
            inputLineNumber,
            beatUnits,
            fCurrentMetronomeBeatUnitsVector [1],
            fCurrentMetronomeParenthesedKind,
            fCurrentDirectionPlacementKind);
      }
      break;

    case msrTempo::kTempoNotesRelationShip:
      {
      }
      tempo =
        msrTempo::create (
          inputLineNumber,
          fCurrentMetronomeRelationLeftElements,
          fCurrentMetronomeRelationRightElements,
          fCurrentMetronomeParenthesedKind,
          fCurrentDirectionPlacementKind);
      break;
  } // switch

  // append metrenome words to tempo if any
  S_msrWords tempoWords;

  int pendingWordsSize = fPendingWords.size ();
  
  if (pendingWordsSize) {
    if (pendingWordsSize > 1) {
      stringstream s;
  
      s <<
        "<direction/> contains " <<
        pendingWordsSize <<
        " <words/> markups";
        
      msrMusicXMLWarning (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        s.str ());
    }

    while (fPendingWords.size ()) {
      S_msrWords
        words =
          fPendingWords.front ();

      // append the words to the temp
      tempo->appendWordsToTempo (
        words);
        
       // remove it from the list
      fPendingWords.pop_front ();
    } // while
  }

  // append the tempo to the pending tempos list
  fPendingTempos.push_back (tempo);
}

//________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart (S_staves& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_direction" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart (S_staff& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_staff" <<
      ", line " << inputLineNumber <<
      endl;
  }

  fCurrentMusicXMLStaffNumber = int(*elt);

  // the staff number should be positive
  if (fCurrentMusicXMLStaffNumber <= 0) {
    stringstream s;

    s <<
      "staff number " << fCurrentMusicXMLStaffNumber <<
      " is not positive" <<
      ", line " << inputLineNumber;
      
    msrAssert (false, s.str ());
  }
  
  S_msrStaff
    staff =
      fetchStaffFromCurrentPart (
        inputLineNumber, fCurrentMusicXMLStaffNumber);

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceStaves) {
    fLogOutputStream <<
      "--> S_staff, fCurrentMusicXMLStaffNumber = " <<
      fCurrentMusicXMLStaffNumber <<
      endl <<
      "--> S_staff, current staff name  = " <<
      staff->getStaffName() <<
      endl;
  }
#endif

  if (fOnGoingForward) {
    fCurrentForwardStaffNumber = fCurrentMusicXMLStaffNumber;
  }
  
  else if (fOnGoingNote) {
    // regular staff indication in note/rest, fine
  }
  
  else if (fOnGoingDirection) {
    // regular staff indication in <direction/>, such as <staff/>
    fCurrentDirectionStaffNumber = fCurrentMusicXMLStaffNumber;
  }
  
  else if (fOnGoingDirectionType) {
    // JMI ???
  }
  
  else {
    stringstream s;
    
    s << "staff " << fCurrentMusicXMLStaffNumber << " is out of context";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }
}
    
//________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart (S_staff_details& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_staff_details" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // number

  fStaffDetailsStaffNumber =
    elt->getAttributeIntValue ("number", 1); // default value is 1, as for clef

  // show-frets

  {
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
        
        s << "show-frets " << showFrets << " unknown";
        
        msrMusicXMLError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
    }
  }
  
  // print-object

  {
    string
      printObject =
        elt->getAttributeValue ("print-object");
  
    fCurrentPrintObjectKind =
      msrStaffDetails::kPrintObjectYes; // default value
    
    if       (printObject == "yes") {
    fCurrentPrintObjectKind =
      msrStaffDetails::kPrintObjectYes;
    }
    else  if (printObject == "no") {
    fCurrentPrintObjectKind =
      msrStaffDetails::kPrintObjectNo;
    }
    else {
      if (printObject.size ()) {
        stringstream s;
        
        s << "print-object " << printObject << " is unknown";
        
        msrMusicXMLError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
    }
  }

  // print-spacing
  
  string
    printSpacing =
      elt->getAttributeValue ("print-spacing");

  fCurrentPrintSpacingKind =
    msrStaffDetails::kPrintSpacingNo; // default value ??? JMI
  
  if       (printSpacing == "yes") {
    fCurrentPrintObjectKind =
      msrStaffDetails::kPrintObjectYes;
  }
  else  if (printSpacing == "no") {
    fCurrentPrintObjectKind =
      msrStaffDetails::kPrintObjectNo;
  }
  else {
    if (printSpacing.size ()) {
      stringstream s;
      
      s << "print-spacing " << printSpacing << " is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceStaves) {
    fLogOutputStream <<
      "Handling staff details:" <<
      endl <<
      gTab << "StaffDetailsStaffNumber" << " = " <<
      fStaffDetailsStaffNumber <<
      endl;
  }
#endif
  
  fCurrentStaffTypeKind =
    msrStaffDetails::kRegularStaffType;

  fCurrentStaffTuningAlterationKind = k_NoAlteration;
  fCurrentStaffTuningOctave         = -1;

  fCurrentStaffDetailsStaffSize = 0;
  
  fCurrentStaffLinesNumber = 5; // default value
  
  fCurrentStaffDetailsCapo = 0;

  // create the staff details
  fCurrentStaffDetails =
    msrStaffDetails::create (
      inputLineNumber,
      fCurrentStaffTypeKind,
      fCurrentShowFretsKind,
      fCurrentPrintObjectKind,
      fCurrentPrintSpacingKind);

  gIndenter++;
}

void mxmlTree2MsrTranslator::visitStart (S_staff_type& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_staff_type" <<
      ", line " << inputLineNumber <<
      endl;
  }

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
    
    s << "staff-type" << staffType << "unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());      
  }
}

void mxmlTree2MsrTranslator::visitStart (S_staff_lines& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_staff_lines" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentStaffLinesNumber = (int)(*elt);
}

void mxmlTree2MsrTranslator::visitStart (S_staff_tuning& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_staff_tuning" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentStaffTuningLine =
    elt->getAttributeIntValue ("line", 0);

  fCurrentStaffTuningAlterationKind = kNatural; // may be absent
  fCurrentStaffTuningOctave         = -1;

  fOnGoingStaffTuning = true;
}
    
void mxmlTree2MsrTranslator::visitStart (S_tuning_step& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_tuning_step" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string tuningStep = elt->getValue();

  checkStep (
    inputLineNumber,
    tuningStep);

  msrDiatonicPitchKind
    tuningDiatonicKind =
      msrDiatonicPitchKindFromString (
        tuningStep [0]);
        
  if (fOnGoingStaffTuning) {
    fCurrentStaffTuningDiatonicPitchKind = tuningDiatonicKind;
  }
  else if (fOnGoingAccord) {
    fCurrentStringTuningDiatonicPitchKind = tuningDiatonicKind;
  }
  else {
    stringstream s;
    
    s << "tuning step " << tuningStep << " is out of context";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxmlTree2MsrTranslator::visitStart (S_tuning_octave& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_tuning_octave" <<
      ", line " << inputLineNumber <<
      endl;
  }

  int tuningOctave = (int)(*elt);
  
  if (fOnGoingStaffTuning) {
    fCurrentStaffTuningOctave = tuningOctave;
  }
  else if (fOnGoingAccord) {
    fCurrentStringTuningOctave = tuningOctave;
  }
  else {
    stringstream s;
    
    s << "tuning octave " << tuningOctave << " is out of context";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxmlTree2MsrTranslator::visitStart (S_tuning_alter& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_tuning_alter" <<
      ", line " << inputLineNumber <<
      endl;
  }

  float tuningAlter = (float)(*elt);

  msrAlterationKind
    tuningAlterationKind =
      msrAlterationKindFromMusicXMLAlter (
        tuningAlter);
      
  if (tuningAlterationKind == k_NoAlteration) {
    stringstream s;

    s <<
      "tuning alter '" << tuningAlter <<
      "' should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";
      
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  if (fOnGoingStaffTuning) {
    fCurrentStaffTuningAlterationKind = tuningAlterationKind;
  }
  else if (fOnGoingAccord) {
    fCurrentStringTuningAlterationKind = tuningAlterationKind;
  }
  else {
    stringstream s;
    
    s << "tuning alter " << tuningAlter << " is out of context";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxmlTree2MsrTranslator::visitEnd (S_staff_tuning& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_staff_tuning" <<
       ", line " << inputLineNumber <<
     endl;
  }

  // fetch relevant staff
  S_msrStaff
    staff =
      fetchStaffFromCurrentPart (
        inputLineNumber,
        fStaffDetailsStaffNumber); // test its value??? JMI

  msrQuarterTonesPitchKind
    quarterTonesPitchKind =
      quarterTonesPitchKindFromDiatonicPitchAndAlteration (
        inputLineNumber,
        fCurrentStaffTuningDiatonicPitchKind,
        fCurrentStaffTuningAlterationKind);

  // create the staff tuning
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceStaffTuning) {
    fLogOutputStream <<
      "Creating staff tuning:" <<
      endl;

    gIndenter++;

    const int fieldWidth = 32;

    fLogOutputStream << left <<
      setw (fieldWidth) <<
      "fCurrentStaffTuningLine" << " = " <<
      fCurrentStaffTuningLine <<
      endl <<
      setw (fieldWidth) <<
      "fCurrentStaffTuningDiatonicPitch" << " = " <<
      msrDiatonicPitchKindAsString (
        gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
        fCurrentStaffTuningDiatonicPitchKind) <<
      endl <<
      setw (fieldWidth) <<
      "fCurrentStaffTuningAlteration" << " = " <<
      msrAlterationKindAsString (
        fCurrentStaffTuningAlterationKind) <<
      endl <<
      setw (fieldWidth) <<
      "quarterTonesPitch" << " = " <<
      msrQuarterTonesPitchKindAsString (
        gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
        quarterTonesPitchKind) <<
      endl <<
      setw (fieldWidth) <<
      "CurrentStaffTuningOctave" << " = " <<
      fCurrentStaffTuningOctave <<
      endl;

    gIndenter--;
  }
#endif

  // create the staff tuning
  S_msrStaffTuning
    staffTuning =
      msrStaffTuning::create (
        inputLineNumber,
        fCurrentStaffTuningLine,
        quarterTonesPitchKind,
        fCurrentStaffTuningOctave);

  // add it to the current staff details
  fCurrentStaffDetails->
    addStaffTuningToStaffDetails (
      staffTuning);
      
  fOnGoingStaffTuning = false;
}

//________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart (S_voice& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_staff_details" <<
      ", line " << inputLineNumber <<
      endl;
  }

  fCurrentMusicXMLVoiceNumber = int(*elt);
  
  // the voice number can be out of 1..4 range
  
  if (fOnGoingForward) {
    fCurrentForwardVoiceNumber = fCurrentMusicXMLVoiceNumber;

/* JMI
    S_msrVoice
      voice =
        fetchVoiceFromPart (
          inputLineNumber,
          fCurrentForwardStaffNumber,
          fCurrentForwardVoiceNumber);
  
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceVoices) {
      fLogOutputStream <<
        "--> S_voice, fCurrentForwardVoiceNumber = " <<
        fCurrentForwardVoiceNumber << endl <<
        "--> S_voice, current voice name  = " <<
        voice->getVoiceName() <<
        endl;
    }
#endif
*/
  }
  
  else if (fOnGoingNote) {
    // regular voice indication in note/rest, fine
  }
  
  else {
    stringstream s;
    
    s << "voice " << fCurrentMusicXMLVoiceNumber << " is out of context";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart (S_backup& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_backup" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // handle the pending tuplets if any
  handleTupletsPendingOnTupletsStack (
    inputLineNumber);
  
  fOnGoingBackup = true;
}

void mxmlTree2MsrTranslator::visitEnd (S_backup& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_backup" <<
      ", line " << inputLineNumber <<
      endl;
  }

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceNotes
      ||
    gTraceOptions->fTraceChords
      ||
    gTraceOptions->fTraceMeasures
      ||
    gTraceOptions->fTraceVoices
      ||
    gTraceOptions->fTraceStaves
      ||
    gTraceOptions->fTraceLyrics
    ) {
    fLogOutputStream <<
      "Handling 'backup <<< " << fCurrentBackupDurationDivisions <<
      " divisions >>>" <<
      ", fCurrentStaffNumberToInsertInto = " <<
      fCurrentStaffNumberToInsertInto <<
      ", fPreviousNoteMusicXMLStaffNumber = " <<
      fPreviousNoteMusicXMLStaffNumber <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  // reset notes staff numbers
  fPreviousNoteMusicXMLStaffNumber = K_NO_STAFF_NUMBER;
  fCurrentMusicXMLStaffNumber  = K_NO_STAFF_NUMBER;

  // reset staff change detection
  fCurrentStaffNumberToInsertInto = K_NO_STAFF_NUMBER;

  fCurrentPart->
    handleBackup (
      inputLineNumber,
      fCurrentBackupDurationDivisions,
      fCurrentDivisionsPerQuarterNote);

  fOnGoingBackup = false;
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_forward& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_forward" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  /*
      <forward>
        <duration>96</duration>
        <voice>1</voice>
        <staff>1</staff>
      </forward>
  */

//* JMI ???
  // the <staff /> element is present only
  // in case of a staff change
  fCurrentForwardStaffNumber = fCurrentMusicXMLStaffNumber;

  // the <voice /> element is present only
  // in case of a voice change
  fCurrentForwardVoiceNumber = fCurrentMusicXMLVoiceNumber;
//*/

  fOnGoingForward = true;
}

void mxmlTree2MsrTranslator::visitEnd ( S_forward& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_forward" <<
      ", line " << inputLineNumber <<
      endl;
  }

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceNotes
      ||
    gTraceOptions->fTraceChords
      ||
    gTraceOptions->fTraceMeasures
      ||
    gTraceOptions->fTraceVoices
      ||
    gTraceOptions->fTraceStaves
      ||
    gTraceOptions->fTraceLyrics
    ) {
    fLogOutputStream <<
      "Handling 'forward <<< " << fCurrentBackupDurationDivisions <<
      " divisions >>>" <<
      ", fCurrentForwardStaffNumber = " <<
      fCurrentForwardStaffNumber <<
      ", fCurrentForwardVoiceNumber = " <<
      fCurrentForwardVoiceNumber <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  // the staff number should be positive
  if (fCurrentForwardStaffNumber <= 0) {
    stringstream s;

    s <<
      "staff number " << fCurrentForwardStaffNumber <<
      " is not positive" <<
      ", line " << inputLineNumber;
      
    msrAssert (false, s.str ());
  }

  // fetch the voice to be forwarded
  S_msrVoice
    voiceToBeForwarded =
      fetchVoiceFromPart (
        inputLineNumber,
        fCurrentForwardStaffNumber,
        fCurrentForwardVoiceNumber);

  // sanity check
  msrAssert (
    voiceToBeForwarded != nullptr,
    "voiceToBeForwarded is null");

  // append a padding note to the voice to be forwarded
  voiceToBeForwarded ->
    appendPaddingNoteToVoice (
      inputLineNumber,
      fCurrentForwardDurationDivisions,
      fCurrentDivisionsPerQuarterNote);

  // reset staff change detection
 // fCurrentStaffNumberToInsertInto = 1; // default value JMI K_NO_STAFF_NUMBER;
  fCurrentStaffNumberToInsertInto = K_NO_STAFF_NUMBER;

/* JMI ???  
  // handle the pending tuplets if any
  handleTupletsPendingOnTupletsStack (
    inputLineNumber);  
  */

  fOnGoingForward = false;
}

//________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart (S_tied& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_tied" <<
      ", line " << inputLineNumber <<
      endl;
  }

// <tied orientation="over" type="start"/>

  string tiedType =
    elt->getAttributeValue ("type");
  
  fCurrentTiedOrientation =
    elt->getAttributeValue ("orientation");

  fCurrentTieKind = msrTie::kTieNone;
  
  if      (tiedType == "start") {
    fCurrentTieKind = msrTie::kTieStart;
  }
  else if (tiedType == "continue") {
    fCurrentTieKind = msrTie::kTieContinue;
  }
  else if (tiedType == "stop") {
    fCurrentTieKind = msrTie::kTieStop;
  }
  else {

    // inner tied notes may miss the "continue" type:
    // let' complain on slur notes outside of slurs 
    if (! fOnGoingSlur) {
      if (tiedType.size ()) {
        stringstream s;
        
        s << "tied type '" << fCurrentSlurType << "' inside a slur is unknown";
        
        msrMusicXMLError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
    }
      
    // inner tied notes may miss the "continue" type:
    // let' complain on ligature notes outside of ligatures 
    if (! fOnGoingLigature) {
      if (tiedType.size ()) {
        stringstream s;
        
        s << "tied type '" << fCurrentSlurType << "' inside a ligature is unknown";
        
        msrMusicXMLError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
    }
    
  }

  if (fCurrentTieKind != msrTie::kTieNone) {
    fCurrentTie =
      msrTie::create (
        inputLineNumber,
        fCurrentTieKind);
  }
}

//________________________________________________________________________
void mxmlTree2MsrTranslator::displaySlurStartsStack (
  string context)
{
  fLogOutputStream <<
    endl <<
    ">>++++++++++++++++ " <<
    "The slurs starts stack contains:" <<
    endl;

  if (fSlurStartsStack.size ()) {  
    list<S_msrSlur>::const_iterator
      iBegin = fSlurStartsStack.begin (),
      iEnd   = fSlurStartsStack.end (),
      i      = iBegin;
        
    gIndenter++;
  
    for ( ; ; ) {
      fLogOutputStream << "v " << (*i);
      if (++i == iEnd) break;
      // no endl here
    } // for
  
    gIndenter--;
  }
  
  fLogOutputStream <<
    "<<++++++++++++++++ " <<
    endl <<
    endl;
}

//________________________________________________________________________
void mxmlTree2MsrTranslator::displayTupletsStack (
  string context)
{
  int tupletStackSize = fTupletsStack.size ();
  
  fLogOutputStream <<
    endl <<
    ">>++++++++++++++++ " <<
    "The tuplet starts stack contains " << tupletStackSize << " elements:" <<
    endl;

  if (tupletStackSize) {  
    list<S_msrTuplet>::const_iterator
      iBegin = fTupletsStack.begin (),
      iEnd   = fTupletsStack.end (),
      i      = iBegin;
        
    gIndenter++;

    int n = tupletStackSize;
    for ( ; ; ) {
      fLogOutputStream <<
        "v (" << n << ")" <<
        endl;

      gIndenter++;
      (*i)->printShort (fLogOutputStream);
      gIndenter--;

      n--;
      
      if (++i == iEnd) break;
      
      fLogOutputStream <<
        endl;
    } // for
  
    gIndenter--;
  }
  
  fLogOutputStream <<
    "<<++++++++++++++++ " <<
    endl <<
    endl;
}

//________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart (S_slur& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_slur" <<
      ", line " << inputLineNumber <<
      endl;
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSlurs) {
    displaySlurStartsStack ("BEFORE handling slur");
  }
#endif
  
  /*
    Only the  first note of the chord should get the slur notation.
    Some applications print out the slur for all notes,
    i.e. a stop and a start in sequqnce:
    these should be ignored
  */
  if (fCurrentNoteBelongsToAChord) {
    stringstream s;
    
    s <<
      "ignoring a slur in a chord member note other than the first one";
      
    msrMusicXMLWarning (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      s.str ());
  }

  else {

    // number
    
    int slurNumber = elt->getAttributeIntValue ("number", 0);
  
    // type
    
    fCurrentSlurType = elt->getAttributeValue ("type");
  
    fCurrentSlurPlacement =
      elt->getAttributeValue ("placement");
  
    // a phrasing slur is recognized as such
    // when the nested regular slur start is met
  
    int slurStartsStackSize = fSlurStartsStack.size ();
  
    if      (fCurrentSlurType == "start") {
      switch (slurStartsStackSize) {
        case 0:
          fCurrentSlurTypeKind = msrSlur::kRegularSlurStart;
          break;
          
        case 1:
          {
            S_msrSlur
              containingSlur =
                fSlurStartsStack.front ();
            
            fCurrentSlurTypeKind = msrSlur::kRegularSlurStart;
    
            // the stack top is in fact a phrasing slur start
#ifdef TRACE_OPTIONS
            if (gTraceOptions->fTraceSlurs) {
              fLogOutputStream <<
                "The slur start '" <<
                containingSlur->asString () <<
                "' contains a nested slur, it is thus a phrasing slur start" <<
                ", line " << inputLineNumber <<
                endl;
            }
#endif
            
            containingSlur->
              setSlurTypeKind (
                msrSlur::kPhrasingSlurStart);
          }
          break;
          
        default:
          {
            stringstream s;
            
            s <<
              "only one slur nesting level is meaningfull";
            
      //      msrMusicXMLError ( // JMI
            msrMusicXMLWarning (
              gXml2lyOptions->fInputSourceName,
              inputLineNumber,
      //        __FILE__, __LINE__,
              s.str ());
          }
      } // switch
        
      fOnGoingSlur = true;    
    }
    
    else if (fCurrentSlurType == "continue") {
      fCurrentSlurTypeKind = msrSlur::kSlurContinue;
    }
    
    else if (fCurrentSlurType == "stop") {
      fCurrentSlurTypeKind = msrSlur::kRegularSlurStop;
      switch (slurStartsStackSize) {
        case 0:
          {
            stringstream s;
            
            s <<
              "a standalone slur 'stop' is meaningless, ignoring it";
            
            msrMusicXMLWarning (
              gXml2lyOptions->fInputSourceName,
              inputLineNumber,
              s.str ());
          }
          break;
          
        case 1:
        /* JMI
          // the current slur stop is regular
          fCurrentSlurTypeKind = msrSlur::kRegularSlurStop;
  
          // pop the top element off the stack
          fSlurStartsStack.pop_front ();
          break;
  */
        case 2:
          // the current slur stop kind depends on that of the stack's top
          switch (fSlurStartsStack.front ()->getSlurTypeKind ()) {
            case msrSlur::kRegularSlurStart:
              fCurrentSlurTypeKind = msrSlur::kRegularSlurStop;
              break;
  
            case msrSlur::kPhrasingSlurStart:
              // the stack top is in fact a phrasing slur start
#ifdef TRACE_OPTIONS
              if (gTraceOptions->fTraceSlurs) {
                fLogOutputStream <<
                  "A slur stop matches a phrasing slur start, it is thus a phrasing slur stop" <<
                  ", line " << inputLineNumber <<
                  endl;
              }
#endif
              
              fCurrentSlurTypeKind = msrSlur::kPhrasingSlurStop;
              break;
  
            default:
              ; // should not occur
          } // switch
  
          // pop the top element off the stack
          fSlurStartsStack.pop_front ();
          break;
          
        default:
          ; // should not occur
      } // switch
  
      fOnGoingSlur = false;
    }
    
    else {
      // inner slur notes may miss the "continue" type:
      // let' complain only on slur notes outside of slurs 
      if (! fOnGoingSlur) {
        if (fCurrentSlurType.size ()) {
          stringstream s;
          
          s <<
            "slur type \"" << fCurrentSlurType <<
            "\" is unknown";
          
          msrMusicXMLError (
            gXml2lyOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
      }
    }

    // line-type
  
    string slurLineType = elt->getAttributeValue ("line-type");
  
    msrLineTypeKind
      slurLineTypeKind =
        kLineTypeSolid; // default value
    
    if      (slurLineType == "solid") {
      slurLineTypeKind = kLineTypeSolid;
    }
    else if (slurLineType == "dashed") {
      slurLineTypeKind = kLineTypeDashed;
    }
    else if (slurLineType == "dotted") {
      slurLineTypeKind = kLineTypeDotted;
    }
    else if (slurLineType == "wavy") {
      slurLineTypeKind = kLineTypeWavy;
    }
    else {
      if (slurLineType.size ()) {
        msrMusicXMLError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          "slur line-type \"" + slurLineType + "\" is unknown");
      }
    }
  
#ifdef TRACE_OPTIONS
    if (
      gTraceOptions->fTraceNotesDetails
        ||
      gTraceOptions->fTraceSlurs) {
      fLogOutputStream <<
        "slurNumber: " <<
        slurNumber <<
        "slurTypeKind: " <<
        msrSlur::slurTypeKindAsString (
          fCurrentSlurTypeKind) <<
        "slurLineType: " <<
        msrLineTypeKindAsString (
          slurLineTypeKind) <<
        endl;
    }
#endif

    S_msrSlur
      slur =
        msrSlur::create (
          inputLineNumber,
          slurNumber,
          fCurrentSlurTypeKind,
          slurLineTypeKind);
          
    fPendingSlurs.push_back (slur);
  
    // push slurs starts onto the stack
    switch (fCurrentSlurTypeKind) {
      case msrSlur::kRegularSlurStart:
      case msrSlur::kPhrasingSlurStart:
        fSlurStartsStack.push_front (slur);
        break;
      default:
        ;
    } // switch
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSlurs) {
    displaySlurStartsStack ("AFTER handling slur");
  }
#endif
}

//________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart (S_bracket& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_bracket" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // number
  
  int ligatureNumber =
    elt->getAttributeIntValue ("number", 0);

  // type

  string ligatureType =
    elt->getAttributeValue ("type");

  fCurrentLigatureKind = msrLigature::kLigatureNone;

  if      (ligatureType == "start") {
    fCurrentLigatureKind = msrLigature::kLigatureStart;
    fOnGoingLigature = true;
  }
  else if (ligatureType == "continue") {
    fCurrentLigatureKind = msrLigature::kLigatureContinue;
  }
  else if (ligatureType == "stop") {
    fCurrentLigatureKind = msrLigature::kLigatureStop;
    fOnGoingLigature = false;
  }
  else {

    // inner ligature notes may miss the "continue" type:
    // let's complain on ligature notes outside of ligatures 
    if (! fOnGoingLigature) {
      if (ligatureType.size ()) {
        stringstream s;
        
        s <<
          "ligature type \"" << ligatureType <<
          "\" is unknown";
        
        msrMusicXMLError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
    }
  }
  
  // line-end

  string ligatureLineEndValue = elt->getAttributeValue ("line-end");

  msrLigature::msrLigatureLineEndKind
    ligatureLineEndKind = msrLigature::kLigatureLineEndNone;
    
  if      (ligatureLineEndValue == "up") {
    ligatureLineEndKind = msrLigature::kLigatureLineEndUp;
  }
  else if (ligatureLineEndValue == "down") {
    ligatureLineEndKind = msrLigature::kLigatureLineEndDown;
  }
  else if (ligatureLineEndValue == "both") {
    ligatureLineEndKind = msrLigature::kLigatureLineEndBoth;
  }
  else if (ligatureLineEndValue == "arrow") {
    ligatureLineEndKind = msrLigature::kLigatureLineEndArrow;
  }
  else if (ligatureLineEndValue == "none") {
    ligatureLineEndKind = msrLigature::kLigatureLineEndNone;
  }
  else {
    if (ligatureLineEndValue.size ()) {
      stringstream s;
      
      s <<
        "ligature line-end \"" << ligatureLineEndValue <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // line-type

  string ligatureLineTypeValue = elt->getAttributeValue ("line-type");

  msrLineTypeKind
    ligatureLineTypeKind =
      kLineTypeSolid; // default value
  
  if      (ligatureLineTypeValue == "solid") {
    ligatureLineTypeKind = kLineTypeSolid;
  }
  else if (ligatureLineTypeValue == "dashed") {
    ligatureLineTypeKind = kLineTypeDashed;
  }
  else if (ligatureLineTypeValue == "dotted") {
    ligatureLineTypeKind = kLineTypeDotted;
  }
  else if (ligatureLineTypeValue == "wavy") {
    ligatureLineTypeKind = kLineTypeWavy;
  }
  else {
    if (ligatureLineTypeValue.size ()) {
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        "ligature line-type \"" + ligatureLineTypeValue + "\" is unknown");
    }
  }

  S_msrLigature
    ligature =
      msrLigature::create (
        inputLineNumber,
        ligatureNumber,
        fCurrentLigatureKind,
        ligatureLineEndKind,
        ligatureLineTypeKind,
        fCurrentDirectionPlacementKind);
        
  fPendingLigatures.push_back (ligature);

  switch (fCurrentLigatureKind) {
    case msrLigature::kLigatureStart:
      // remember this ligature spanner start
      switch (fCurrentDirectionPlacementKind) {
        case msrPlacementKind::kPlacementNone:
          {
            stringstream s;
    
            s <<
              "Bracket start found with no placement";
    
            msrMusicXMLError (
              gXml2lyOptions->fInputSourceName,
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
          
        case msrPlacementKind::kPlacementAbove:
          fCurrentLigatureStartAbove = ligature;
          break;
          
        case msrPlacementKind::kPlacementBelow:
          fCurrentLigatureStartBelow = ligature;
          break;
      } // switch
      break;
      
    case msrLigature::kLigatureStop:
      // set spanner two-way sidelinks
      // between both ends of the ligature spanner

      switch (fCurrentDirectionPlacementKind) {
        case msrPlacementKind::kPlacementNone:
          break;
          
        case msrPlacementKind::kPlacementAbove:
          if (! fCurrentLigatureStartAbove) {
            stringstream s;
    
            s <<
              "Bracket stop above found with no corresponding bracket start";
    
            msrMusicXMLError (
              gXml2lyOptions->fInputSourceName,
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          else {
            ligature->
              setLigatureOtherEndSidelink (
                fCurrentLigatureStartAbove);
          }
              
          // forget this ligature spanner start
          fCurrentLigatureStartAbove = nullptr;
          break;
          
        case msrPlacementKind::kPlacementBelow:
          if (! fCurrentLigatureStartBelow) {
            stringstream s;
    
            s <<
              "Bracket stop below found with no corresponding bracket start";
    
            msrMusicXMLError (
              gXml2lyOptions->fInputSourceName,
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          else {
            ligature->
              setLigatureOtherEndSidelink (
                fCurrentLigatureStartBelow);
          }
              
          // forget this ligature spanner start
          fCurrentLigatureStartBelow = nullptr;
          break;
      } // switch
      break;
      
    case msrLigature::kLigatureContinue:
      break;
      
    case msrLigature::kLigatureNone:
      // JMI ???
      break;
  } // switch
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_wedge& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_wedge" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // type
  
  string type = elt->getAttributeValue("type");
  
  msrWedge::msrWedgeKind wedgeKind = msrWedge::kWedgeKindNone;

  if      (type == "crescendo") {
    wedgeKind = msrWedge::kWedgeCrescendo;
  }
  else if (type == "diminuendo") {
    wedgeKind = msrWedge::kWedgeDecrescendo;
  }
  else if (type == "stop") {
    wedgeKind = msrWedge::kWedgeStop;
  }
  else {
    if (type.size ()) {
      stringstream s;

      s <<
        "unknown wedge type \"" +
        type +
        "\", should be 'crescendo', 'diminuendo' or 'stop'";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // niente
  
  string nienteString = elt->getAttributeValue ("niente");
    
  msrWedge::msrWedgeNienteKind
    wedgeNienteKind = msrWedge::kWedgeNienteNo;

  if       (nienteString == "yes") {
    wedgeNienteKind = msrWedge::kWedgeNienteYes;
  }
  else  if (nienteString == "no") {
    wedgeNienteKind = msrWedge::kWedgeNienteNo;
  }
  else {
    if (nienteString.size ()) {
      stringstream s;
      
      s <<
        "wedge niente \"" << nienteString <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }
    
  // line-type

  string wedgeLineType = elt->getAttributeValue ("line-type");

  msrLineTypeKind
    wedgeLineTypeKind =
      kLineTypeSolid; // default value
  
  if      (wedgeLineType == "solid") {
    wedgeLineTypeKind = kLineTypeSolid;
  }
  else if (wedgeLineType == "dashed") {
    wedgeLineTypeKind = kLineTypeDashed;
  }
  else if (wedgeLineType == "dotted") {
    wedgeLineTypeKind = kLineTypeDotted;
  }
  else if (wedgeLineType == "wavy") {
    wedgeLineTypeKind = kLineTypeWavy;
  }
  else {
    if (wedgeLineType.size ()) {
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        "wedge line-type \"" + wedgeLineType + "\" is unknown");
    }
  }

  S_msrWedge
    wedge =
      msrWedge::create (
        inputLineNumber,
        wedgeKind,
        wedgeNienteKind,
        wedgeLineTypeKind,
        fCurrentDirectionPlacementKind);
        
  fPendingWedges.push_back (wedge);
}
    
//________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart (S_lyric& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_lyric" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // number

  {
    fCurrentStanzaNumber =
      elt->getAttributeValue ("number");
    
    if (fCurrentStanzaNumber.size () == 0) {
      msrMusicXMLWarning (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        "lyric number is empty, using \"1\" by default");

      fCurrentStanzaNumber = "1";
    }
    
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceLyrics) {
      fLogOutputStream <<
        "Setting fCurrentStanzaNumber to " <<
        fCurrentStanzaNumber <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif
        
    // register it as current stanza number,
    // that remains set another positive value is met,
    // thus allowing a skip syllable to be generated
    // for notes without lyrics
  }
  
  // name

  {
    fCurrentStanzaName =
      elt->getAttributeValue ("name");

    if (fCurrentStanzaName.size () == 0) {
      // lyrics names are not so frequent after all...
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceLyrics) {
        fLogOutputStream <<
          "Lyric name is empty, using \"" <<
          K_NO_STANZA_NAME <<
          "\" by default" <<
          endl;
      }
#endif

      fCurrentStanzaName = K_NO_STANZA_NAME;
    }
    
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceLyrics) {
      fLogOutputStream <<
        "Setting fCurrentStanzaName to \"" <<
        fCurrentStanzaName <<
        "\", line " << inputLineNumber <<
        endl;
    }
#endif
        
    // register it as current stanza name,
    // that remains set another positive value is met,
    // thus allowing a skip syllable to be generated
    // for notes without lyrics
  }


  // forget about any previous texts met,
  // in case there are <text> occurrences without <syllabic> around them
  fCurrentLyricTextsList.clear ();

  fCurrentStanzaHasText = false;

  fOnGoingLyric = true;
}

void mxmlTree2MsrTranslator::visitStart ( S_syllabic& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_syllabic" <<
      ", line " << inputLineNumber <<
      endl;
  }

  fCurrentSyllabic = elt->getValue();
  
  if      (fCurrentSyllabic == "single") {
    fCurrentSyllableKind = msrSyllable::kSyllableSingle;
  }
  else if (fCurrentSyllabic == "begin") {
    fCurrentSyllableKind = msrSyllable::kSyllableBegin;
  }
  else if (fCurrentSyllabic == "middle") {
    fCurrentSyllableKind = msrSyllable::kSyllableMiddle;
  }
  else if (fCurrentSyllabic == "end") {
    fCurrentSyllableKind = msrSyllable::kSyllableEnd;
  }
  else {
    stringstream s;
    
    s <<
      "syllabic \"" << fCurrentSyllabic <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // forget about any previous texts met
  fCurrentLyricTextsList.clear ();
}

void mxmlTree2MsrTranslator::visitStart ( S_text& elt ) 
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_text" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string textValue = elt->getValue();

  // there can be several <text/>'s and <elision/> in a row, hence the list
  fCurrentLyricTextsList.push_back (textValue);

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    fLogOutputStream <<
      "textValue = \""<< textValue << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fCurrentNoteHasLyrics = true;
  fCurrentStanzaHasText = true;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gIndenter++;
    
    const int fieldWidth = 20;

    fLogOutputStream << left <<
      setw (fieldWidth) <<
      "line " << " = " << inputLineNumber <<
      endl <<
      setw (fieldWidth) <<
      "fCurrentStanzaNumber" << " = " << fCurrentStanzaNumber <<
      endl <<
      setw (fieldWidth) <<
      "fCurrentStanzaName" << " = \"" << fCurrentStanzaName << "\"" <<
      endl <<
      setw (fieldWidth) <<
      "fCurrentSyllabic" << " = " << fCurrentSyllabic <<
      endl <<
      setw (fieldWidth) <<
      "fCurrentLyricTextsList" << " = ";

    msrSyllable::writeTextsList (
      fCurrentLyricTextsList,
      fLogOutputStream);
    
    fLogOutputStream <<
      endl;

    gIndenter--;
  }
#endif

  // a <text/> markup puts an end to the effect of <extend/>
  fCurrentSyllableExtendKind = msrSyllable::kSyllableExtendNone;
}

void mxmlTree2MsrTranslator::visitStart ( S_elision& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_elision" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  string elisionValue = elt->getValue ();

  if (! elisionValue.size ()) {
    elisionValue = " ";
  } 

  // there can be several <text/>'s and <elision/> in a row, hence the list
  fCurrentLyricTextsList.push_back (elisionValue);
  
  fCurrentStanzaHasText = true;
}

void mxmlTree2MsrTranslator::visitStart ( S_extend& elt ) 
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_extend" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string extendType =
    elt->getAttributeValue ("type");

  if (fOnGoingLyric) {
    fCurrentSyllableExtendKind =
      msrSyllable::kSyllableExtendSingle; // default value

    if      (extendType == "start") {
      fCurrentSyllableExtendKind =
        msrSyllable::kSyllableExtendStart;
    }
    else if (extendType == "continue") {
      fCurrentSyllableExtendKind =
        msrSyllable::kSyllableExtendContinue;
    }
    else if (extendType == "stop") {
      fCurrentSyllableExtendKind =
        msrSyllable::kSyllableExtendStop;
    }
    else {
      if (extendType.size ()) {
        stringstream s;
        
        s <<
          "extend type \"" << extendType <<
          "\" is unknown";
        
        msrMusicXMLError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      else {
  //   JMI   fOnGoingSyllableExtend = true;
      }
    }
  }

  else if (fOnGoingFiguredBass) { // JMI
  }
}

void mxmlTree2MsrTranslator::visitEnd ( S_lyric& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_lyric" <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (fCurrentSyllableKind == msrSyllable::kSyllableNone) {
    // syllabic is not mandatory...
    stringstream s;

    s <<
      "<lyric /> has no <syllabic/> component, using 'single' by default";

    msrMusicXMLWarning (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      s.str ());
    
    fCurrentSyllableKind = msrSyllable::kSyllableSingle;
  }

  if (fCurrentNoteIsARest) {
    stringstream s;

    s <<
      "syllable ";

   msrSyllable::writeTextsList (
    fCurrentLyricTextsList,
    s);

    s <<
      " is attached to a rest";

    msrMusicXMLWarning (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      s.str ());
      
    fCurrentSyllableKind =
      msrSyllable::kSyllableSkip; // kSyllableRest ??? JMI
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyricsDetails) {
    fLogOutputStream <<
      "==> visitEnd ( S_lyric&), fCurrentSyllableKind = " <<
      msrSyllable::syllableKindAsString (fCurrentSyllableKind) <<
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
        "fCurrentMusicXMLStaffNumber" << " = " << fCurrentMusicXMLStaffNumber <<
        endl <<
        setw (fieldwidth) <<
        "fCurrentStanzaNumber" << " = " << fCurrentStanzaNumber <<
        endl <<
        setw (fieldwidth) <<
        "fCurrentStanzaName" << " = \"" << fCurrentStanzaName << "\"" <<
        endl <<
        setw (fieldwidth) <<
        "fCurrentLyricTextsList" << " = ";

      msrSyllable::writeTextsList (
        fCurrentLyricTextsList,
        fLogOutputStream);
  
      fLogOutputStream << left <<
        endl <<
        setw (fieldwidth) <<
        "fCurrentSyllableExtendKind" << " = " <<
        msrSyllable::syllableExtendKindAsString (
          fCurrentSyllableExtendKind) <<
        endl <<
        setw (fieldwidth) <<
        "fCurrentNoteIsARest" << " = " <<
        booleanAsString (fCurrentNoteIsARest) <<
        endl <<
        setw (fieldwidth) <<
        "fCurrentRestMeasure" << " = " <<
        booleanAsString (fCurrentRestMeasure) <<
        endl;
  
      fLogOutputStream << left <<
        setw (fieldwidth) <<
        "fCurrentTieKind" << " = \"" <<
        msrTie::tieKindAsString (fCurrentTieKind) <<
        "\"" <<
        endl;
          
      fLogOutputStream << left <<
        setw (fieldwidth) <<
        "fCurrentSlurTypeKind" << " = \"" <<
        msrSlur::slurTypeKindAsString (fCurrentSlurTypeKind) <<
        "\"" <<
        endl;
  
      fLogOutputStream << left <<
        setw (fieldwidth) <<
        "fCurrentLigatureKind" << " = \"" <<
        msrLigature::ligatureKindAsString (
          fCurrentLigatureKind) <<
        "\"" <<
        endl;
  
      fLogOutputStream << left <<
        setw (fieldwidth) <<
        "fOnGoingSlur" << " = " <<
        booleanAsString (fOnGoingSlur) <<
        endl <<
        setw (fieldwidth) <<
        "fOnGoingSlurHasStanza" << " = " <<
        booleanAsString (fOnGoingSlurHasStanza) <<
        endl;
  
      fLogOutputStream << left <<
        setw (fieldwidth) <<
        "fOnGoingLigature" << " = " <<
        booleanAsString (fOnGoingLigature) <<
        endl <<
        setw (fieldwidth) <<
        "fOnGoingLigatureHasStanza" << " = " <<
        booleanAsString (fOnGoingLigatureHasStanza) <<
        endl;
  
      fLogOutputStream << left <<
        setw (fieldwidth) <<
        "fFirstSyllableInSlurKind" << " = \"" <<
        msrSyllable::syllableKindAsString (
          fFirstSyllableInSlurKind) <<
        "\"" <<
        endl;
  
      fLogOutputStream << left <<
        setw (fieldwidth) <<
        "fFirstSyllableInLigatureKind" << " = \"" <<
        msrSyllable::syllableKindAsString (
          fFirstSyllableInLigatureKind) <<
        "\"" <<
      endl;
  
      fLogOutputStream << left <<
        setw (fieldwidth) <<
        "fCurrentSyllableKind" << " = \""<<
        msrSyllable::syllableKindAsString (
          fCurrentSyllableKind) <<
        "\"" <<
      endl;
          
      gIndenter--;
    }
    
    gIndenter--;
  }
#endif

  // fetch current voice
  S_msrVoice
    currentVoice =
      fetchVoiceFromPart (
        inputLineNumber,
        fCurrentStaffNumberToInsertInto, // JMI fCurrentMusicXMLStaffNumber,
        fCurrentMusicXMLVoiceNumber);

  // fetch stanzaNumber in current voice
  S_msrStanza
    stanza =
      currentVoice->
        fetchStanzaInVoice (
          inputLineNumber,
          fCurrentStanzaNumber,
          fCurrentStanzaName);

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {      
    fLogOutputStream <<
      "Creating a syllable '" <<
      msrSyllable::syllableKindAsString (
        fCurrentSyllableKind) <<
      "\", fCurrentLyricTextsList = \"";

    msrSyllable::writeTextsList (
      fCurrentLyricTextsList,
      fLogOutputStream);

    fLogOutputStream <<
      "\"" <<
      ", whole notes: " <<
      fCurrentNoteSoundingWholeNotesFromDuration <<
      " sounding from duration, " <<
       fCurrentNoteDisplayWholeNotesFromType << 
      ", display from type" <<
      ", syllabic = \"" <<
      msrSyllable::syllableKindAsString (
        fCurrentSyllableKind) << "\"" <<
      ", in stanza " << stanza->getStanzaName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
    
  // create a syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        fCurrentSyllableKind,
        fCurrentSyllableExtendKind,
        fCurrentNoteSoundingWholeNotesFromDuration,
        msrTupletFactor (
          fCurrentNoteActualNotes,
          fCurrentNoteNormalNotes),
        stanza);

  // append the lyric texts to the syllable
  for (
    list<string>::const_iterator i = fCurrentLyricTextsList.begin ();
    i!=fCurrentLyricTextsList.end ();
    i++) {
    syllable->
      appendLyricTextToSyllable ((*i));
  } // for

  // don't forget about fCurrentLyricTextsList here,
  // this will be done in visitStart ( S_syllabic& )
  
  // appendSyllableToNoteAndSetItsNoteUplink()
  // will be called in handleLyrics(),
  // after the note has been created
    
  // append syllable to current note's syllables list
  fCurrentNoteSyllables.push_back (
    syllable);

  // append syllable to stanza
  stanza->
    appendSyllableToStanza (syllable);

  // DON'T register current note as having lyrics,
  // it's only the case when there are <text/> inside the <lyric/>:
  // the latter may contain only an <extend/> markup,

  fOnGoingLyric = false;
}

//________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart (S_measure& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_measure" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // take this measure into account
  fCurrentMeasureOrdinalNumber++;
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasuresDetails) {
    gLogIOstream <<
      "==> visitStart (S_measure" <<
      ", fCurrentMeasureOrdinalNumber = '" <<
        fCurrentMeasureOrdinalNumber <<
      "', fCurrentMeasureNumber = '" <<
        fCurrentMeasureNumber <<
      "', line " << inputLineNumber <<
      ", in part \"" <<
      fCurrentPart->getPartCombinedName () << "\"" <<
      endl;
  }
#endif

  // number

  fCurrentMeasureNumber =
    elt->getAttributeValue ("number");

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTracePasses) {
    fLogOutputStream <<
      endl <<
      "<!--=== measure '" << fCurrentMeasureNumber <<
      "', line " << inputLineNumber << " ===-->" <<
      endl;
  }
#endif

  // implicit

/*
  Measures with an implicit attribute set to "yes"
  never display a measure number,
  regardless of the measure-numbering setting.
  * 
  The implicit attribute is set to "yes" for measures where
  the measure number should never appear, such as pickup
  measures and the last half of mid-measure repeats. The
  value is "no" if not specified.
*/

  string
    implicit =
      elt->getAttributeValue ("implicit");

  msrMeasure::msrMeasureImplicitKind
    measureImplicitKind = msrMeasure::kMeasureImplicitNo; // default value
  
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
      
      s <<
        "implicit \"" << implicit <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }
  
  // set next measure number in current part
  // if this measure is not the first one
  if (fCurrentMeasureOrdinalNumber > 1) {
    fCurrentPart->
      setNextMeasureNumberInPart (
        inputLineNumber,
        fCurrentMeasureNumber);
  }
    
  // append a new measure to the current part
  fCurrentPart->
    createMeasureAndAppendItToPart (
      inputLineNumber,
      fCurrentMeasureNumber,
      fCurrentMeasureOrdinalNumber,
      measureImplicitKind);

  // reset staff change detection
  fPreviousNoteMusicXMLStaffNumber = K_NO_STAFF_NUMBER;
  fCurrentStaffNumberToInsertInto  = 1; // default value JMI K_NO_STAFF_NUMBER;
  
/* JMI
  // is this measure number in the debug set?
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceDetailed) {
    set<int>::const_iterator
      it =
        gGeneralOptions->
          fTraceDetailedMeasureNumbersSet.find (fMeasuresCounter);
          
    if (it != gTraceOptions->fTraceDetailedMeasureNumbersSet.end ()) {
      // yes, activate detailed trace for it
      gMusicXMLOptions = gMusicXMLOptionsWithDetailedTrace;
      gGeneralOptions  = gGeneralOptionsWithDetailedTrace;
      gMsrOptions      = gMsrOptionsWithDetailedTrace;
      gLpsrOptions     = gLpsrOptionsWithDetailedTrace;
      gLilypondOptions = gLilypondOptionsWithDetailedTrace;
    }
  }
#endif
  */
}

void mxmlTree2MsrTranslator::visitEnd (S_measure& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_measure" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // is there a pending grace notes group?
  if (fPendingGraceNotesGroup) {
#ifdef TRACE_OPTIONS
    if (
      gTraceOptions->fTraceGraceNotes
        ||
      gTraceOptions->fTraceNotes
    ) {
      fLogOutputStream <<
        endl <<
        endl <<
        "fCurrentGraceNotes IS NOT NULL at the end of measure '" << // JMI
        elt->getAttributeValue ("number") <<
        "'" <<
        endl <<
        endl;
  
      fLogOutputStream <<
        endl <<
        endl <<
        endl <<
        "+++++++++++++++++" <<
        fCurrentPart <<
        endl <<
        endl <<
        endl;
    }
#endif

    // attach these grace notes group as an after grace notes group
    // to the last note found in its voice

    // fetch the voice
    S_msrVoice
      voice =
        fetchVoiceFromPart (
          inputLineNumber,
          fCurrentStaffNumberToInsertInto,
          fCurrentMusicXMLVoiceNumber);

    // fetch note to attach to
    S_msrNote
      noteToAttachTo =
      /* JMI
      // JMI might prove not precise enough???
  //      fVoicesLastMetNoteMap [currentVoice];
        fVoicesLastMetNoteMap [
          make_pair (
            fCurrentStaffNumberToInsertInto,
            fCurrentMusicXMLVoiceNumber)
          ];
      */
        voice->
          getVoiceLastAppendedNote (); // ??? JMI
          
    
    if (! noteToAttachTo) {
      stringstream s;

      s <<
        "cannot find voice to insert after grace notes group into: " <<
        "chordFirstNote is null" <<
        ", fCurrentStaffNumberToInsertInto = " <<
        fCurrentStaffNumberToInsertInto <<
        endl <<
        ", fCurrentMusicXMLVoiceNumber = " <<
        fCurrentMusicXMLVoiceNumber;
        
      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }

    // set the pending grace notes group's kind to 'after'
    fPendingGraceNotesGroup->
      setGraceNotesGroupKind (
        msrGraceNotesGroup::kGraceNotesGroupAfter);
    
    // attach it to the note
    noteToAttachTo->
      setNoteGraceNotesGroupAfter (
// JMI        afterGraceNotesGroup);
        fPendingGraceNotesGroup);
        
    // forget about this grace notes group
    fPendingGraceNotesGroup = nullptr;
  }
  
  if (fCurrentATupletStopIsPending) {
    if (fTupletsStack.size ()) { // JMI
      // finalize the tuplet, only now in case the last element
      // is actually a chord
      finalizeTupletAndPopItFromTupletsStack (
        inputLineNumber);
    }

    fCurrentATupletStopIsPending = false;
  }

  // finalize current measure in the part,
  // to set measures kind
  fCurrentPart->
    finalizeCurrentMeasureInPart (
      inputLineNumber);

  // handle an on going multiple rest if any only now,
  // so that the necessary staves/voices have been created
  if (fOnGoingMultipleRest) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMultipleRests) {
      fLogOutputStream <<
        "--> onGoingMultipleRest" <<
        endl <<
        gTab << "fCurrentMultipleRestHasBeenCreated:" <<
        booleanAsString (
          fCurrentMultipleRestHasBeenCreated) <<
        endl <<
        gTab << "fRemainingMultipleRestMeasuresNumber:" <<
        fRemainingMultipleRestMeasuresNumber <<
        endl <<
        endl;
    }
#endif
    
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

    if (fRemainingMultipleRestMeasuresNumber <= 0) {
      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        "fRemainingMultipleRestMeasuresNumber problem");
    }
    
    // account for one more rest measure in the multiple rest
    fRemainingMultipleRestMeasuresNumber--;
    
    if (fRemainingMultipleRestMeasuresNumber == 0) {
      // all rest measures have been met,
      // the current one is the first after the multiple rest
      fCurrentPart->
        appendPendingMultipleRestToPart (
          inputLineNumber);

      if (fRemainingMultipleRestMeasuresNumber == 1) {
        fCurrentPart-> // JMI ??? BOF
          setNextMeasureNumberInPart (
            inputLineNumber,
            fCurrentMeasureNumber);
      }
  
      // forget about and multiple rest having been created
      fCurrentMultipleRestHasBeenCreated = false;
      
      fOnGoingMultipleRest = false;
    }

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMultipleRests) {
      fLogOutputStream <<
        "<-- onGoingMultipleRest" <<
        endl <<
        gTab << "fCurrentMultipleRestHasBeenCreated:" <<
        booleanAsString (
          fCurrentMultipleRestHasBeenCreated) <<
        endl <<
        gTab << "fRemainingMultipleRestMeasuresNumber:" <<
        fRemainingMultipleRestMeasuresNumber <<
        endl <<
        gTab << "fOnGoingMultipleRest:" <<
        fOnGoingMultipleRest <<
        endl <<
        endl;
    }
#endif
  }

/* JMI
  // restore debug options in case they were set in visitStart()
  gMusicXMLOptions = gMusicXMLOptionsUserChoices;
  gGeneralOptions  = gGeneralOptionsUserChoices;
  gMsrOptions      = gMsrOptionsUserChoices;
  gLpsrOptions     = gLpsrOptionsUserChoices;
  gLilypondOptions = gLilypondOptionsUserChoices;
  */
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_print& elt ) 
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_print" <<
      ", line " << inputLineNumber <<
      endl;
  }

/* JMI
  const int staffSpacing =
    elt->getAttributeIntValue ("staff-spacing", 0); // JMI
  */
  
  // handle 'new-system' if present
  
  const string newSystem = elt->getAttributeValue ("new-system");
  
  if (newSystem.size ()) {
    
    if (newSystem == "yes") {
      
      // create a barNumberCheck
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceMeasures) {
        fLogOutputStream << 
          "Creating a barnumber check, " <<
          "line = " << inputLineNumber <<
          endl;
      }
#endif

      // fetch current voice
      S_msrVoice
        currentVoice =
          fetchVoiceFromPart (
            inputLineNumber,
            fCurrentMusicXMLStaffNumber,
            fCurrentMusicXMLVoiceNumber);

      S_msrBarNumberCheck
        barNumberCheck_ =
          msrBarNumberCheck::create (
            inputLineNumber,
            currentVoice->
              getVoiceCurrentMeasureNumber ());
            
      // append it to the voice
// JMI      S_msrElement bnc = barNumberCheck_;
      currentVoice->
        appendBarNumberCheckToVoice (barNumberCheck_);
  
      // create a line break
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceMeasures) {
        fLogOutputStream << 
          "Creating a line break, " <<
          "line = " << inputLineNumber << endl;
      }
#endif

      S_msrLineBreak
        lineBreak =
          msrLineBreak::create (
            inputLineNumber,
            currentVoice->
              getVoiceCurrentMeasureNumber ());
  
      // append it to the voice
      currentVoice->
        appendLineBreakToVoice (lineBreak);
     }
    
    else if (newSystem == "no") {
      // ignore it
    }
    
    else {
      stringstream s;
  
      s << "new-system \"" << newSystem <<
      "\" is unknown in '<print />', should be 'yes', 'no' or empty";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // handle 'new-page' if present

  const string newPage = elt->getAttributeValue ("new-page");
  
  if (newPage.size ()) {
    
    if (newPage == "yes") { // JMI
      
      // fetch current voice
      S_msrVoice
        currentVoice =
          fetchVoiceFromPart (
            inputLineNumber,
            fCurrentMusicXMLStaffNumber,
            fCurrentMusicXMLVoiceNumber);
  
      // create a page break
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceMeasures) {
        fLogOutputStream << 
          "Creating a page break, " <<
          "line = " << inputLineNumber << endl;
      }
#endif

      S_msrPageBreak
        pageBreak =
          msrPageBreak::create (
            inputLineNumber);
  
      // append it to the voice
      currentVoice->
        appendPageBreakToVoice (pageBreak);
     }
    
    else if (newPage == "no") {
      // ignore it
    }
    
    else {
      stringstream s;
  
      s << "new-page \"" << newPage <<
      "\" is unknown in '<print />', should be 'yes', 'no' or empty";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // handle 'blank-page' if present

  const string blankPage = elt->getAttributeValue ("blank-page"); // JMI
  
  // handle 'page-number' if present

  const string pageNumber = elt->getAttributeValue ("page-number"); // JMI

  fCurrentDisplayText = "";
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_system_layout& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_system_layout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

/* JMI
  int inputLineNumber =
    elt->getInputLineNumber ();

  string measureNumberingString = elt->getValue ();

  / *  JMI
  fCurrentBarlineStyleKind =
    msrBarline::k_NoStyle; // default value
* /

  if      (measureNumberingString == "none") {
 //   fCurrentBarlineStyleKind =
 //     msrBarline::kRegularStyle;
  }
  else if (measureNumberingString == "measure") {
//    fCurrentBarlineStyleKind =
 //     msrBarline::kDottedStyle;
  }
  else if (measureNumberingString == "system") {
 //   fCurrentBarlineStyleKind =
 //     msrBarline::kDashedStyle;
  }
  else {
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      "measure-numbering \"" + measureNumberingString + "\" is unknown");
  }
*/
}

void mxmlTree2MsrTranslator::visitStart ( S_measure_numbering& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_measure_numbering" <<
       ", line " << elt->getInputLineNumber () <<
     endl;
  }

  // JMI
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_barline& elt ) 
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_barline" <<
      ", line " << inputLineNumber <<
      endl;
  }

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

  fCurrentBarlineHasSegnoKind = msrBarline::kBarlineHasSegnoNo;
  fCurrentBarlineHasCodaKind  = msrBarline::kBarlineHasCodaNo;

  fCurrentBarlineLocationKind        = msrBarline::kBarlineLocationNone;
  fCurrentBarlineStyleKind           = msrBarline::kBarlineStyleNone;
  fCurrentBarlineEndingTypeKind      = msrBarline::kBarlineEndingNone;
  fCurrentBarlineRepeatDirectionKind = msrBarline::kBarlineRepeatDirectionNone;
  fCurrentBarlineRepeatWingedKind    = msrBarline::kBarlineRepeatWingedNone;

  fCurrentBarlineTimes = 2; // default value JMI ???
  
  // location

  {
    string
      location =
        elt->getAttributeValue ("location");
  
    fCurrentBarlineLocationKind =
      msrBarline::kBarlineLocationRight; // by default
      
    if       (location == "left") {
      fCurrentBarlineLocationKind = msrBarline::kBarlineLocationLeft;
    }
    else  if (location == "middle") {
      fCurrentBarlineLocationKind = msrBarline::kBarlineLocationMiddle;
    }
    else if  (location == "right") {
      fCurrentBarlineLocationKind = msrBarline::kBarlineLocationRight;
    }
    else {
      stringstream s;
      
      s <<
        "barline location \"" << location <<
        "\" is unknown";
        
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }
  
  fOnGoingBarline = true;
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_bar_style& elt ) 
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_bar_style" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string barStyle = elt->getValue();

  fCurrentBarlineStyleKind =
    msrBarline::kBarlineStyleNone; // default value

  if      (barStyle == "regular") {
    fCurrentBarlineStyleKind =
      msrBarline::kBarlineStyleRegular;
  }
  else if (barStyle == "dotted") {
    fCurrentBarlineStyleKind =
      msrBarline::kBarlineStyleDotted;
  }
  else if (barStyle == "dashed") {
    fCurrentBarlineStyleKind =
      msrBarline::kBarlineStyleDashed;
  }
  else if (barStyle == "heavy") {
    fCurrentBarlineStyleKind =
      msrBarline::kBarlineStyleHeavy;
  }
  else if (barStyle == "light-light") {
    fCurrentBarlineStyleKind =
      msrBarline::kBarlineStyleLightLight;
  }
  else if (barStyle == "light-heavy") {
    fCurrentBarlineStyleKind =
      msrBarline::kBarlineStyleLightHeavy;
  }
  else if (barStyle == "heavy-light") {
    fCurrentBarlineStyleKind =
      msrBarline::kBarlineStyleHeavyLight;
  }
  else if (barStyle == "heavy-heavy") {
    fCurrentBarlineStyleKind =
      msrBarline::kBarlineStyleHeavyHeavy;
  }
  else if (barStyle == "tick") {
    fCurrentBarlineStyleKind =
      msrBarline::kBarlineStyleTick;
  }
  else if (barStyle == "short") {
    fCurrentBarlineStyleKind =
      msrBarline::kBarlineStyleShort;
  }
  else if (barStyle == "none") {
    fCurrentBarlineStyleKind =
      msrBarline::kBarlineStyleNone;
  }
  else {
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      "bar-style \"" + barStyle + "\" is unknown");
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_segno& elt ) 
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_segno" <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (fOnGoingDirectionType) {
    // fetch current voice
    S_msrVoice
      currentVoice =
        fetchVoiceFromPart (
          inputLineNumber,
          fCurrentMusicXMLStaffNumber,
          fCurrentMusicXMLVoiceNumber);
  
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
    fCurrentBarlineHasSegnoKind =
      msrBarline::kBarlineHasSegnoYes;
  }
}

void mxmlTree2MsrTranslator::visitStart ( S_coda& elt ) 
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_coda" <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (fOnGoingDirectionType) {
    // fetch current voice
    S_msrVoice
      currentVoice =
        fetchVoiceFromPart (
          inputLineNumber,
          fCurrentMusicXMLStaffNumber,
          fCurrentMusicXMLVoiceNumber);
  
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
    fCurrentBarlineHasCodaKind = msrBarline::kBarlineHasCodaYes;
  }
}

void mxmlTree2MsrTranslator::visitStart ( S_eyeglasses& elt ) 
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_eyeglasses" <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (fOnGoingDirectionType) {
    // fetch current voice
    S_msrVoice
      currentVoice =
        fetchVoiceFromPart (
          inputLineNumber,
          fCurrentMusicXMLStaffNumber,
          fCurrentMusicXMLVoiceNumber);
  
    // create the eyeglasses
    S_msrEyeGlasses
      eyeGlasses =
        msrEyeGlasses::create (
          inputLineNumber);

    // append it to the pending eyeglasses list
    fPendingEyeGlasses.push_back (eyeGlasses);
  }
}

void mxmlTree2MsrTranslator::visitStart ( S_pedal& elt ) 
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_pedal" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // type
  
  string type = elt->getAttributeValue ("type");
    
  msrPedal::msrPedalTypeKind pedalTypeKind = msrPedal::k_NoPedalType;

  if       (type == "start") {
    pedalTypeKind = msrPedal::kPedalStart;
  }
  else  if (type == "continue") {
    pedalTypeKind = msrPedal::kPedalContinue;
  }
  else  if (type == "change") {
    pedalTypeKind = msrPedal::kPedalChange;
  }
  else  if (type == "stop") {
    pedalTypeKind = msrPedal::kPedalStop;
  }
  else {
    stringstream s;
    
    s << "pedal type '" << type << "' is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // line
  
  string line = elt->getAttributeValue ("line");
    
  msrPedal::msrPedalLineKind pedalLineKind = msrPedal::kPedalLineNo;

  if       (line == "yes") {
    pedalLineKind = msrPedal::kPedalLineYes;
  }
  else  if (line == "no") {
    pedalLineKind = msrPedal::kPedalLineNo;
  }
  else {
    if (line.size ()) {
      stringstream s;
      
      s <<
        "pedal line \"" << line <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }
  
  // sign
  
  string sign = elt->getAttributeValue ("sign");
    
  msrPedal::msrPedalSignKind
    pedalSignKind =
      msrPedal::kPedalSignNo;

  if       (sign == "yes") {
    pedalSignKind = msrPedal::kPedalSignYes;
  }
  else  if (sign == "no") {
    pedalSignKind = msrPedal::kPedalSignNo;
  }
  else {
    if (sign.size ()) {
      stringstream s;
      
      s <<
        "pedal sign \"" << sign <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }
  
  if (fOnGoingDirectionType) {      
    // fetch current voice
    S_msrVoice
      currentVoice =
        fetchVoiceFromPart (
          inputLineNumber,
          fCurrentMusicXMLStaffNumber,
          fCurrentMusicXMLVoiceNumber);
  
    // create the pedal
    S_msrPedal
      pedal =
        msrPedal::create (
          inputLineNumber,
          pedalTypeKind,
          pedalLineKind,
          pedalSignKind);

    // append it to the pending pedals list
    fPendingPedals.push_back (pedal);
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_ending& elt ) 
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_ending" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // number

  {
    fCurrentBarlineEndingNumber =
      elt->getAttributeValue ("number"); // may be "1, 2"        

    if (! fCurrentBarlineEndingNumber.size ()) {
      msrMusicXMLWarning (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        "mandatory ending number is missing, assuming \"1\"");

      fCurrentBarlineEndingNumber = "1";
    }
  }
    
  // type

  {
    string type =
      elt->getAttributeValue ("type");
        
    fCurrentBarlineEndingTypeKind =
      msrBarline::kBarlineEndingNone;

    if       (type == "start") {
      fCurrentBarlineEndingTypeKind =
        msrBarline::kBarlineEndingTypeStart;
    }
    else  if (type == "stop") {
      fCurrentBarlineEndingTypeKind =
        msrBarline::kBarlineEndingTypeStop;
    }
    else  if (type == "discontinue") {
      fCurrentBarlineEndingTypeKind =
        msrBarline::kBarlineEndingTypeDiscontinue;
    }
    else {
      stringstream s;
      
      s <<
        "ending type \"" << type <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_repeat& elt ) 
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_repeat" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // direction

  {
    string direction = elt->getAttributeValue ("direction");
  
    fCurrentBarlineRepeatDirectionKind =
      msrBarline::kBarlineRepeatDirectionNone;
      
    if       (direction == "forward") {
      fCurrentBarlineRepeatDirectionKind =
        msrBarline::kBarlineRepeatDirectionForward;
    }
    else  if (direction == "backward") {
      fCurrentBarlineRepeatDirectionKind =
        msrBarline::kBarlineRepeatDirectionBackward;
    }
    else {
      stringstream s;
      
      s <<
        "repeat direction \"" << direction <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }
  
  // winged

  {
    string winged = elt->getAttributeValue ("winged");
  
    fCurrentBarlineRepeatWingedKind =
      msrBarline::kBarlineRepeatWingedNone; // default value
  
    if (winged.size ()) {
      if       (winged == "none") {
        fCurrentBarlineRepeatWingedKind =
          msrBarline::kBarlineRepeatWingedNone;
      }
      else if (winged == "straight") {
        fCurrentBarlineRepeatWingedKind =
          msrBarline::kBarlineRepeatWingedStraight;
      }
      else  if (winged == "curved") {
        fCurrentBarlineRepeatWingedKind =
          msrBarline::kBarlineRepeatWingedCurved;
      }
      else  if (winged == "doubleStraight") {
        fCurrentBarlineRepeatWingedKind =
          msrBarline::kBarlineRepeatWingedDoubleStraight;
      }
      else  if (winged == "doubleCurved") {
        fCurrentBarlineRepeatWingedKind =
          msrBarline::kBarlineRepeatWingedDoubleCurved;
      }
      else {
        stringstream s;
        
        s <<
          "repeat winged \"" << winged <<
          "\" is unknown";
        
        msrMusicXMLError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
    }
  }

  // times

  {
    fCurrentBarlineTimes =
      elt->getAttributeIntValue ("times", 2); // default value JMI ???
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitEnd ( S_barline& elt ) 
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_barline" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // is there a pending tuplet?
  if (fTupletsStack.size ()) { // JMI
    // finalize the tuplet,
    // for it to be created before the barline
    finalizeTupletAndPopItFromTupletsStack (
      inputLineNumber);
  }

  // create the barline
  S_msrBarline
    barline =
      msrBarline::create (
        inputLineNumber,
        msrBarline::k_NoBarlineCategory, // will be set below
        fCurrentBarlineHasSegnoKind,
        fCurrentBarlineHasCodaKind,
        fCurrentBarlineLocationKind,
        fCurrentBarlineStyleKind,
        fCurrentBarlineEndingTypeKind,
        fCurrentBarlineEndingNumber,
        fCurrentBarlineRepeatDirectionKind,
        fCurrentBarlineRepeatWingedKind,
        fCurrentBarlineTimes);

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceBarlines) {
    fLogOutputStream <<
      "Creating barline in part " <<
      fCurrentPart->getPartCombinedName () << ":" <<
      endl;
      
    gIndenter++;
    
    fLogOutputStream <<
      barline;
      
    gIndenter--;
  }
#endif

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
    fCurrentBarlineLocationKind == msrBarline::kBarlineLocationLeft
      &&
    fCurrentBarlineEndingTypeKind == msrBarline::kBarlineEndingTypeStart
      &&
    fCurrentBarlineEndingNumber.size () != 0) {
    // ending start, don't know yet whether it's hooked or hookless
    // ------------------------------------------------------
    handleEndingStart (barline);

    barlineIsAlright = true;
  }

/* JMI
  else if (
    fCurrentBarlineLocationKind == msrBarline::kBarlineLocationLeft
      &&
    fCurrentBarlineEndingTypeKind == msrBarline::kBarlineEndingTypeStart
      &&
    fCurrentBarlineRepeatDirectionKind == msrBarline::kBarlineRepeatDirectionForward) {
    // hooked ending start
    // ------------------------------------------------------
    handleHookedEndingStart (elt, barline);

    barlineIsAlright = true;
  }
*/

  else if (
    fCurrentBarlineLocationKind == msrBarline::kBarlineLocationLeft
      &&
    fCurrentBarlineRepeatDirectionKind == msrBarline::kBarlineRepeatDirectionForward) {
    // repeat start
    // ------------------------------------------------------
    
    handleRepeatStart (barline);

    barlineIsAlright = true;
  }

  /* JMI
  else if (
    fCurrentBarlineLocationKind == msrBarline::kBarlineLocationLeft
      &&
    fCurrentBarlineEndingTypeKind == msrBarline::kBarlineEndingTypeStart) { // no forward
    // hookless ending start
    // ------------------------------------------------------
    handleHooklessEndingStart (elt, barline);

    barlineIsAlright = true;
  }
*/

  else if (
    fCurrentBarlineLocationKind == msrBarline::kBarlineLocationRight
      &&
    fCurrentBarlineEndingTypeKind == msrBarline::kBarlineEndingTypeStop
      &&
    fCurrentBarlineEndingNumber.size () != 0) {
    // hooked ending end
    // ------------------------------------------------------
    
    handleHookedEndingEnd (barline);
    
    barlineIsAlright = true;
  }

  else if (
    fCurrentBarlineLocationKind == msrBarline::kBarlineLocationRight
      &&
    fCurrentBarlineRepeatDirectionKind == msrBarline::kBarlineRepeatDirectionBackward) {
    // repeat end
    // ------------------------------------------------------

    handleRepeatEnd (barline);

/* JMI
    if (fRepeatEndCounter == 0 ) {
      // this is a regular repeat end
      handleRepeatEnd (barline);
    }
    else {
      // this is the end of an implicit repeat
      createAndPrependImplicitBarLine (
        inputLineNumber);

#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceRepeats) {
        fLogOutputStream <<
          "Appending an implicit repeat to part " <<
          fCurrentPart->getPartCombinedName () <<
          endl;
      }
#endif
    
      fCurrentPart->
        createRepeatUponItsEndAndAppendItToPart (
          inputLineNumber,
          fCurrentMeasureNumber,
          barline->getBarlineTimes ());
    }
    */

    barlineIsAlright = true;
  }

  else if (
    fCurrentBarlineLocationKind == msrBarline::kBarlineLocationRight
      &&
    fCurrentBarlineEndingTypeKind == msrBarline::kBarlineEndingTypeDiscontinue
      &&
    fCurrentBarlineEndingNumber.size () != 0) {
    // hookless ending end
    // ------------------------------------------------------
    handleHooklessEndingEnd (barline);
        
    barlineIsAlright = true;
  }

  else {

    // set the barline category
    switch (fCurrentBarlineStyleKind) {
      case msrBarline::kBarlineStyleRegular:
      case msrBarline::kBarlineStyleDotted:
      case msrBarline::kBarlineStyleDashed:
      case msrBarline::kBarlineStyleHeavy:
      case msrBarline::kBarlineStyleLightLight:
      case msrBarline::kBarlineStyleLightHeavy:
      case msrBarline::kBarlineStyleHeavyLight:
      case msrBarline::kBarlineStyleHeavyHeavy:
      case msrBarline::kBarlineStyleTick:
      case msrBarline::kBarlineStyleShort:
      case msrBarline::kBarlineStyleNone:
        barline->
          setBarlineCategory (
            msrBarline::kBarlineCategoryStandalone);
              
        // append the bar line to the current part
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceBarlines) {
          fLogOutputStream <<
            "Appending a barline to part " <<
            fCurrentPart->getPartCombinedName () << ":" <<
            endl;
            
          gIndenter++;
          
          fLogOutputStream <<
            barline;
            
          gIndenter--;
        }
#endif
      
        fCurrentPart->
          appendBarlineToPart (barline);
      
        barlineIsAlright = true;
        break;

  /* JMI
      case msrBarline::kBarlineStyleNone:
        ; // no <bar-style> has been found
        */
    } // switch
  }
      
  // has this barline been handled?
  if (! barlineIsAlright) {
    stringstream s;
    
    s << left <<
      "cannot handle a barline containing: " <<
      barline->asString ();
      
    msrInternalWarning (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      s.str ());
  }
  
  fOnGoingBarline = false;
}
  
  /* JMI
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
void mxmlTree2MsrTranslator::visitStart ( S_note& elt ) 
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_note" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // save previous note staff number
  fPreviousNoteMusicXMLStaffNumber = fCurrentMusicXMLStaffNumber;

  // initialize note data to a neutral state
  initializeNoteData ();

  fCurrentNoteDiatonicPitchKind = k_NoDiatonicPitch;
  fCurrentNoteAlterationKind    = kNatural;

  fCurrentNoteOctave = K_NO_OCTAVE;

  fCurrentNoteSoundingWholeNotes             = rational (0, 1);
  fCurrentNoteSoundingWholeNotesFromDuration = rational (0, 1);

  fCurrentDisplayDiatonicPitchKind      = k_NoDiatonicPitch;
  fCurrentDisplayOctave                 = K_NO_OCTAVE;
  fCurrentNoteDisplayWholeNotes         = rational (0, 1);
  fCurrentNoteDisplayWholeNotesFromType = rational (0, 1);
    
  // note head
  
  fCurrentNoteHeadKind = msrNote::kNoteHeadNormal;
  fCurrentNoteHeadFilledKind = msrNote::kNoteHeadFilledYes;
  fCurrentNoteHeadParenthesesKind = msrNote::kNoteHeadParenthesesNo;
  
  // assuming staff number 1, unless S_staff states otherwise afterwards
  fCurrentMusicXMLStaffNumber = 1; // JMI

  // assuming voice number 1, unless S_voice states otherwise afterwards
  fCurrentMusicXMLVoiceNumber = 1; // JMI

  // tuplets
  
  fCurrentNoteActualNotes = -1;
  fCurrentNoteNormalNotes = -1;

  // lyrics

  fCurrentNoteHasLyrics = false;
  fASkipSyllableHasBeenGeneratedForcurrentNote = false;
  
  fCurrentStanzaNumber = K_NO_STANZA_NUMBER;
  fCurrentStanzaName = K_NO_STANZA_NAME;

  fCurrentSyllabic = "";
  // don't forget about fCurrentLyricTextsList here,
  // this will be done in visitStart ( S_syllabic& )
  fCurrentSyllableKind = msrSyllable::kSyllableNone;

  if (fOnGoingSyllableExtend) {
    fCurrentSyllableExtendKind =
      msrSyllable::kSyllableExtendContinue; // it may be absent
  }
  else {
    fCurrentSyllableExtendKind =
      msrSyllable::kSyllableExtendNone;
  }

  // stems
  
  fCurrentStem = nullptr;

  // tremolos

  fCurrentNoteBelongsToADoubleTremolo = false;
  fCurrentTremoloTypeKind             = k_NoTremoloType;

  // ties
  
  fCurrentTie = nullptr;
  fCurrentTiedOrientation = "";

  // slurs
  
  fCurrentSlurType = "";
  fCurrentSlurPlacement = "";
  fCurrentSlurTypeKind = msrSlur::k_NoSlur;

  // ligatures
  
  fCurrentLigatureKind = msrLigature::kLigatureNone;

  // print-object
  
  // note print kind
  
  string notePrintObject = elt->getAttributeValue ("print-object");
  
  fCurrentNotePrintKind = msrNote::kNotePrintYes; // default value
      
  if      (notePrintObject == "yes")
    fCurrentNotePrintKind = msrNote::kNotePrintYes;
  else if (notePrintObject == "no")
    fCurrentNotePrintKind = msrNote::kNotePrintNo;
  else {
    if (notePrintObject.size ()) {
      stringstream s;
      
      s <<
        "note print-object \"" << notePrintObject <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  fOnGoingNote = true;
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_step& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_step" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string step = elt->getValue();
  
  checkStep (
    inputLineNumber,
    step);

  fCurrentNoteDiatonicPitchKind =
    msrDiatonicPitchKindFromString (step [0]);
}

void mxmlTree2MsrTranslator::visitStart ( S_alter& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_alter" <<
      ", line " << inputLineNumber <<
      endl;
  }

  float alter = (float)(*elt);

  fCurrentNoteAlterationKind =
    msrAlterationKindFromMusicXMLAlter (
      alter);
      
  if (fCurrentNoteAlterationKind == k_NoAlteration) {
    stringstream s;

    s <<
      "alter '" << alter << "'"
      "' should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";
      
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxmlTree2MsrTranslator::visitStart ( S_octave& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_octave" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentNoteOctave = (int)(*elt);

  if (fCurrentNoteOctave < 0 || fCurrentNoteOctave > 9) {
    stringstream s;
    
    s <<
      "ocrave value '" << fCurrentNoteOctave <<
      "' is not in the 0..9 range, '0' is assumed";
    
    msrMusicXMLWarning (
      gXml2lyOptions->fInputSourceName,
      elt->getInputLineNumber (),
      s.str ());

    fCurrentNoteOctave = 0;
  }
}

void mxmlTree2MsrTranslator::visitStart ( S_duration& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_duration" <<
      ", line " << inputLineNumber <<
      endl;
  }

  int duration = (int)(*elt); // divisions

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotesDetails) {
    fLogOutputStream <<
      "Note duration: " << duration <<
      endl;
  }
#endif

  if (fOnGoingBackup) {
  
    fCurrentBackupDurationDivisions = duration;

  }
  
  else if (fOnGoingForward) {
  
    fCurrentForwardDurationDivisions = duration;
    
  }
  
  else if (fOnGoingNote) {
  
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceNotesDetails) {
      fLogOutputStream <<
        "fCurrentDivisionsPerQuarterNote: " <<
        fCurrentDivisionsPerQuarterNote <<
        endl;
    }
#endif

    // set current grace note whole notes      
    fCurrentNoteSoundingWholeNotesFromDuration =
      rational (
        duration,
        fCurrentDivisionsPerQuarterNote * 4); // hence a whole note

    fCurrentNoteSoundingWholeNotesFromDuration.rationalise ();

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceNotesDetails) {
      fLogOutputStream <<
        "fCurrentNoteSoundingWholeNotesFromDuration: " <<
        fCurrentNoteSoundingWholeNotesFromDuration <<
        endl;
    }
#endif

/* JMI
    // set current grace note display whole notes
    // to note sounding whole notes
    fCurrentNoteDisplayWholeNotes =
      fCurrentNoteSoundingWholeNotesFromDuration; // by default
     */
  }

  else if (fOnGoingFiguredBass) {

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceFiguredBass) {
      fLogOutputStream <<
        "fCurrentDivisionsPerQuarterNote: " <<
        fCurrentDivisionsPerQuarterNote <<
        endl;
    }
#endif

    // set current figured bass whole notes      
    fCurrentFiguredBassSoundingWholeNotes =
      rational (
        duration,
        fCurrentDivisionsPerQuarterNote * 4); // hence a whole note

    fCurrentFiguredBassSoundingWholeNotes.rationalise ();

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceFiguredBass) {
      fLogOutputStream <<
        "fCurrentFiguredBassSoundingWholeNotes: " <<
        fCurrentFiguredBassSoundingWholeNotes <<
        endl;
    }
#endif
  }
  
  else {
    
    stringstream s;
    
    s << "duration " << duration << " is out of context";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
    
//  fLogOutputStream << "=== mxmlTree2MsrTranslator::visitStart ( S_duration& elt ), fCurrentDuration = " << fCurrentDuration << endl; JMI
}

void mxmlTree2MsrTranslator::visitStart ( S_instrument& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_instrument" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

/*
  <instrument id="P2-I4"/>
*/
 string id = elt->getAttributeValue ("id"); // JMI
}
       
void mxmlTree2MsrTranslator::visitStart ( S_dot& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_dot" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentNoteDotsNumber++;
}
       
void mxmlTree2MsrTranslator::visitStart ( S_type& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_type" <<
      ", line " << inputLineNumber <<
      endl;
  }

/*
 Type indicates the graphic note type, Valid values (from shortest to longest) are 1024th, 512th, 256th, 128th, 64th, 32nd, 16th, eighth, quarter, half, whole, breve, long, and maxima. The size attribute indicates full, cue, or large size, with full the default for regular notes and cue the default for cue and grace notes.
*/

  {
    string noteType = elt->getValue();
  
    // the type contains a display duration,
    fCurrentNoteGraphicDurationKind =
      msrDurationKindFromString (
        inputLineNumber,
        noteType);
  }

  // size
  
  {
    string noteTypeSize = elt->getAttributeValue ("size");
  
    if (noteTypeSize == "full") {
      // a regular note
    }
    else if (noteTypeSize == "cue") { // USE IT! JMI ???
    }
  
    else {
      if (noteTypeSize.size ()) {
        msrMusicXMLError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
            "note type size \"" + noteTypeSize + "\" is unknown");
      }
    }
  }
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotesDetails) {
    /* JMI
    fLogOutputStream <<
      "noteType: \"" <<
      noteType <<
      "\"" <<
      endl <<
      "noteTypeSize: \"" <<
      noteTypeSize <<
      "\"" <<
      endl;
        */
  }
#endif
}

void mxmlTree2MsrTranslator::visitStart ( S_notehead& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_notehead" <<
      ", line " << inputLineNumber <<
      endl;
  }

  {
    string noteHead = elt->getValue();
   
    if      (noteHead == "slash") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadSlash; }
    else if (noteHead == "triangle") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadTriangle; }
    else if (noteHead == "diamond")   {
      fCurrentNoteHeadKind = msrNote::kNoteHeadDiamond; } 
    else if (noteHead == "square") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadSquare; } 
    else if (noteHead == "cross") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadCross; } 
    else if (noteHead == "x") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadX; } 
    else if (noteHead == "circle-x") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadCircleX; } 
    else if (noteHead == "inverted triangle") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadInvertedTriangle; } 
    else if (noteHead == "arrow down") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadArrowDown; } 
    else if (noteHead == "arrow up") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadArrowUp; } 
    else if (noteHead == "slashed") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadSlashed; } 
    else if (noteHead == "back slashed") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadBackSlashed; } 
    else if (noteHead == "normal") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadNormal; } 
    else if (noteHead == "cluster") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadCluster; }
    else if (noteHead == "circle dot") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadCircleDot; }
    else if (noteHead == "left triangle") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadLeftTriangle; }
    else if (noteHead == "rectangle") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadRectangle; }
    else if (noteHead == "none") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadNone; }
    else if (noteHead == "do") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadDo; }
    else if (noteHead == "re") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadRe; }
    else if (noteHead == "mi") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadMi; }
    else if (noteHead == "fa") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadFa; }
    else if (noteHead == "fa up") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadFaUp; }
    else if (noteHead == "so") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadSo; }
    else if (noteHead == "la") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadLa; }
    else if (noteHead == "ti") {
      fCurrentNoteHeadKind = msrNote::kNoteHeadTi; }
    else {
      stringstream s;
      
      s <<
        "note head \"" << noteHead <<
        "\" is unknown";
  
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // filled
  
  {
    string noteHeadFilled = elt->getAttributeValue ("filled");
    
    if      (noteHeadFilled == "yes")
      fCurrentNoteHeadFilledKind = msrNote::kNoteHeadFilledYes;
    else if (noteHeadFilled == "no")
      fCurrentNoteHeadFilledKind = msrNote::kNoteHeadFilledNo;
    else {
      if (noteHeadFilled.size ()) {
        stringstream s;
        
        s <<
          "note head filled \"" << noteHeadFilled <<
          "\" is unknown";
        
        msrMusicXMLError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
    }
  }

  // parentheses
  
  {
    string noteHeadParentheses = elt->getAttributeValue ("parentheses");
    
    if      (noteHeadParentheses == "yes")
      fCurrentNoteHeadParenthesesKind = msrNote::kNoteHeadParenthesesYes;
    else if (noteHeadParentheses == "no")
      fCurrentNoteHeadParenthesesKind = msrNote::kNoteHeadParenthesesNo;
    else {
      if (noteHeadParentheses.size ()) {
        stringstream s;
        
        s <<
          "note head parentheses \"" << noteHeadParentheses <<
          "\" is unknown";
        
        msrMusicXMLError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
    }
  }
}

void mxmlTree2MsrTranslator::visitStart ( S_accidental& elt ) // JMI
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_accidental" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // value

  {
    string accidentalValue = elt->getValue ();
  
    fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalNone;

    if      (accidentalValue == "sharp")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalSharp;
    else if (accidentalValue == "natural")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalNatural;
    else if (accidentalValue == "flat")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalFlat;
    else if (accidentalValue == "double-sharp")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentaldoubleSharp;
    else if (accidentalValue == "sharp-sharp")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalSharpSharp;
    else if (accidentalValue == "flat-flat")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalFlatFlat;
    else if (accidentalValue == "natural-sharp")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalNaturalSharp;
    else if (accidentalValue == "natural-flat")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalNaturalFlat;
    else if (accidentalValue == "quarter-flat")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalQuarterFlat;
    else if (accidentalValue == "quarter-sharp")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalQuarterSharp;
    else if (accidentalValue == "three-quarters-flat")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalThreeQuartersFlat;
    else if (accidentalValue == "three-quarters-sharp")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalThreeQuartersSharp;
      
    else if (accidentalValue == "sharp-down")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalSharpDown;
    else if (accidentalValue == "sharp-up")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalSharpUp;
    else if (accidentalValue == "natural-down")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalNaturalDown;
    else if (accidentalValue == "natural-up")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalNaturalUp;
    else if (accidentalValue == "flat-down")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalFlatDown;
    else if (accidentalValue == "flat-up")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalFlatUp;
    else if (accidentalValue == "triple-sharp")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalTripleSharp;
    else if (accidentalValue == "triple-flat")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalTripleFlat;
    else if (accidentalValue == "slash-quarter-sharp")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalSlashQuarterSharp;
    else if (accidentalValue == "slash-sharp")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalSlashSharp;
    else if (accidentalValue == "slash-flat")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalSlashFlat;
    else if (accidentalValue == "double-slash-flat")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentaldoubleSlashFlat;
    else if (accidentalValue == "sharp-1")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalSharp_1;
    else if (accidentalValue == "sharp-2")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalSharp_2;
    else if (accidentalValue == "sharp-3")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalSharp_3;
    else if (accidentalValue == "sharp-5")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalSharp_5;
    else if (accidentalValue == "flat-1")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalFlat_1;
    else if (accidentalValue == "flat-2")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalFlat_2;
    else if (accidentalValue == "flat-3")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalFlat_3;
    else if (accidentalValue == "flat-4")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalFlat_4;
    else if (accidentalValue == "sori")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalSori;
    else if (accidentalValue == "koron")
      fCurrentNoteAccidentalKind = msrNote::kNoteAccidentalKoron;
    else {
      if (accidentalValue.size ()) {
        stringstream s;
        
        s <<
          "accidental \"" << accidentalValue <<
          "\" is unknown";
        
        msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
          s.str ());
      }
    }
  }

  // editorial
  
  {
    string editorialAccidental = elt->getAttributeValue ("editorial");

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
        
        s <<
          "editorial accidental \"" << editorialAccidental <<
          "\" is unknown";
        
        msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
          s.str ());
      }
    }
  }

  // cautionary
  
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
        
        s <<
          "cautionary accidental \"" << cautionaryAccidental <<
          "\" is unknown";
        
        msrMusicXMLError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
    }
  }
}

void mxmlTree2MsrTranslator::visitStart ( S_stem& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_stem" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string        stem = elt->getValue();

  msrStem::msrStemKind stemKind = msrStem::kStemNone;
  
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
      "\" is unknown";
      
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fCurrentStem =
    msrStem::create (
      inputLineNumber,
      stemKind);
}

void mxmlTree2MsrTranslator::visitStart ( S_beam& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_beam" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // value
  
  fCurrentBeamValue = elt->getValue();

  msrBeam::msrBeamKind beamKind = msrBeam::k_NoBeam;

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
      "\"" << "is not known";
      
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
    
  // number
  
  fCurrentBeamNumber = 
    elt->getAttributeIntValue ("number", 0);

  S_msrBeam
    beam =
      msrBeam::create (
        inputLineNumber,
        fCurrentBeamNumber,
        beamKind);

  fPendingBeams.push_back (beam);
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_measure_style& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_measure_style" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}
       
void mxmlTree2MsrTranslator::visitStart ( S_beat_repeat& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_beat_repeat" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentBeatRepeatSlashes = elt->getAttributeIntValue ("slashes", 0);

  string beatRepeatUseDots = elt->getAttributeValue ("use-dots");

/* JMI
  if      (beatRepeatUseDots == "yes")
    fCurrentTupletTypeKind = msrTuplet::kStartTuplet; // JMI
  else if (beatRepeatUseDots == "no")
    fCurrentTupletTypeKind = msrTuplet::kStopTuplet;
  else {
    stringstream s;
    
    s << "beat repeat use dots " << beatRepeatUseDots << " is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
  */
}
       
void mxmlTree2MsrTranslator::visitStart ( S_measure_repeat& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_measure_repeat" <<
      ", line " << inputLineNumber <<
      endl;
  }

/*
  The multiple-rest and measure-repeat symbols indicate the
  number of measures covered in the element content. The
  beat-repeat and slash elements can cover partial measures.
  All but the multiple-rest element use a type attribute to 
  indicate starting and stopping the use of the style. The
  optional number attribute specifies the staff number from
  top to bottom on the system, as with clef.
*/
  
  fCurrentMeasuresRepeatMeasuresNumber = (int)(*elt);

  // slashes
  
  fCurrentMeasuresRepeatSlashesNumber =
    elt->getAttributeIntValue ("slashes", 1); // default value

  // type
  
  string measuresRepeatType =
    elt->getAttributeValue ("type");

  fCurrentMeasuresRepeatKind = msrMeasuresRepeat::k_NoMeasuresRepeat;
  
  if      (measuresRepeatType == "start") {
    fCurrentMeasuresRepeatKind = msrMeasuresRepeat::kStartMeasuresRepeat; // JMI

    fCurrentPart->
      createMeasuresRepeatFromItsFirstMeasuresInPart (
        inputLineNumber,
        fCurrentMeasuresRepeatMeasuresNumber,
        fCurrentMeasuresRepeatSlashesNumber);
  }
  
  else if (measuresRepeatType == "stop") {
    fCurrentMeasuresRepeatKind = msrMeasuresRepeat::kStopMeasuresRepeat; // JMI

    fCurrentPart->
      appendPendingMeasuresRepeatToPart (
        inputLineNumber);
  }
  
  else {
    stringstream s;
    
    s <<
      "measure-repeat type \"" << measuresRepeatType <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}
       
void mxmlTree2MsrTranslator::visitStart ( S_multiple_rest& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_multiple_rest" <<
      ", line " << inputLineNumber <<
      endl;
  }
  
  fCurrentMultipleRestMeasuresNumber = (int)(*elt);

  string multipleRestUseSymbols = elt->getAttributeValue ("use-symbols");

  if      (multipleRestUseSymbols == "yes") {
    // JMI
  }
  else if (multipleRestUseSymbols == "no") {
    // JMI
  }
  else {
    if (multipleRestUseSymbols.size ()) {
      stringstream s;
      
      s <<
        "multiple rest use symbols " <<
        multipleRestUseSymbols <<
        " is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
      }
  }

  // register number of remeaining rest measures
  fRemainingMultipleRestMeasuresNumber =
    fCurrentMultipleRestMeasuresNumber;

  // the multiple rest will created at the end of its first measure,
  // so that the needed staves/voices have been created
  
  fOnGoingMultipleRest = true;
}
       
void mxmlTree2MsrTranslator::visitStart ( S_slash& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_slash" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // type
  
  string slashType = elt->getAttributeValue ("type");

  if      (slashType == "start")
    fCurrentSlashTypeKind = kSlashTypeStart;
  else if (slashType == "stop")
    fCurrentSlashTypeKind = kSlashTypeStop;
  else {
    stringstream s;
    
    s <<
      "slash type \"" << slashType <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // use-dots
  
  string slashUseDots = elt->getAttributeValue ("use-dots");

  if      (slashUseDots == "yes")
    fCurrentSlashUseDotsKind = kSlashUseDotsYes;
  else if (slashUseDots == "no")
    fCurrentSlashUseDotsKind = kSlashUseDotsNo;
  else {
    if (slashUseDots.size ()) {
      stringstream s;
      
      s <<
        "slash use-dots \"" << slashUseDots <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // use-stems

  string slashUseStems = elt->getAttributeValue ("use-stems");

  if      (slashUseStems == "yes")
    fCurrentSlashUseStemsKind = kSlashUseStemsYes;
  else if (slashUseStems == "no")
    fCurrentSlashUseStemsKind = kSlashUseStemsNo;
  else {
    if (slashUseStems.size ()) {
      stringstream s;
      
      s <<
        "slash use-stems \"" << slashUseStems <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  fCurrentSlashDotsNumber = 0;
}

void mxmlTree2MsrTranslator::visitStart ( S_slash_type& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_slash_type" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string slashType = elt->getValue();

  // the type contains a display duration,
  fCurrentSlashGraphicDurationKind =
    msrDurationKindFromString (
      inputLineNumber,
      slashType);
        
  // size
  
  string slashTypeSize = elt->getAttributeValue ("size");

  if (slashTypeSize == "cue") { // USE IT! JMI ???
  }

  else {
    if (slashTypeSize.size ()) {
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
          "slash type size \"" + slashTypeSize + "\" is unknown");
    }
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSlashes) {
    fLogOutputStream <<
      "slashType: \"" <<
      slashType <<
      "\"" <<
      endl <<
      "slashTypeSize: \"" <<
      slashTypeSize <<
      "\"" <<
      endl;
  }
#endif
}

void mxmlTree2MsrTranslator::visitStart ( S_slash_dot& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_slash_dot" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentSlashDotsNumber++;
}

void mxmlTree2MsrTranslator::visitEnd ( S_slash& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_slash" <<
      ", line " << inputLineNumber <<
      endl;
  }

  S_msrSlash
    slash =
      msrSlash::create (
        inputLineNumber,
        fCurrentSlashTypeKind,
        fCurrentSlashUseDotsKind,
        fCurrentSlashUseStemsKind);

  fPendingSlashes.push_back (slash);
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_articulations& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_articulations" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void mxmlTree2MsrTranslator::visitStart ( S_accent& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_accent" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind placementKind = kPlacementNone;

  if      (placementString == "above")
    placementKind = kPlacementAbove;
  else if (placementString == "below")
    placementKind = kPlacementBelow;    
  else {
    if (placementString.size ()) {
      stringstream s;
      
      s <<
        "accent placement \"" << placementString <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());    
    }
  }
  
  // create the articulation  
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulation::kAccent,
        placementKind);
      
  fCurrentArticulations.push_back (articulation);
}

void mxmlTree2MsrTranslator::visitStart ( S_breath_mark& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_breath_mark" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind placementKind = kPlacementNone;

  if      (placementString == "above")
    placementKind = kPlacementAbove;
  else if (placementString == "below")
    placementKind = kPlacementBelow;    
  else {
    if (placementString.size ()) {
      stringstream s;
      
      s <<
        "breath-mark placement \"" << placementString <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());    
    }
  }

  // create the articulation  
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulation::kBreathMark,
        placementKind);
      
  fCurrentArticulations.push_back (articulation);
}

void mxmlTree2MsrTranslator::visitStart ( S_caesura& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_caesura" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind placementKind = kPlacementNone;

  if      (placementString == "above")
    placementKind = kPlacementAbove;
  else if (placementString == "below")
    placementKind = kPlacementBelow;    
  else {
    if (placementString.size ()) {
      stringstream s;
      
      s <<
        "caesura placement \"" << placementString <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());    
    }
  }

  // create the articulation  
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulation::kCaesura,
        placementKind);
      
  fCurrentArticulations.push_back (articulation);
}

void mxmlTree2MsrTranslator::visitStart ( S_spiccato& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_spiccato" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind placementKind = kPlacementNone;

  if      (placementString == "above")
    placementKind = kPlacementAbove;
  else if (placementString == "below")
    placementKind = kPlacementBelow;    
  else {
    if (placementString.size ()) {
      stringstream s;
      
      s <<
        "spiccato placement \"" << placementString <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());    
    }
  }

  // create the articulation  
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulation::kSpiccato,
        placementKind);
      
  fCurrentArticulations.push_back (articulation);
}

void mxmlTree2MsrTranslator::visitStart ( S_staccato& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_staccato" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind placementKind = kPlacementNone;

  if      (placementString == "above")
    placementKind = kPlacementAbove;
  else if (placementString == "below")
    placementKind = kPlacementBelow;    
  else {
    if (placementString.size ()) {
      stringstream s;
      
      s <<
        "staccato placement \"" << placementString <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());    
    }
  }

  // create the articulation  
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulation::kStaccato,
        placementKind);
      
  fCurrentArticulations.push_back (articulation);
}

void mxmlTree2MsrTranslator::visitStart ( S_staccatissimo& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_staccatissimo" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind placementKind = kPlacementNone;

  if      (placementString == "above")
    placementKind = kPlacementAbove;
  else if (placementString == "below")
    placementKind = kPlacementBelow;    
  else {
    if (placementString.size ()) {
      stringstream s;
      
      s <<
        "staccatissimo placement \"" << placementString <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());    
    }
  }

  // create the articulation  
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulation::kStaccatissimo,
        placementKind);
      
  fCurrentArticulations.push_back (articulation);
}

void mxmlTree2MsrTranslator::visitStart ( S_stress& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_stress" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind placementKind = kPlacementNone;

  if      (placementString == "above")
    placementKind = kPlacementAbove;
  else if (placementString == "below")
    placementKind = kPlacementBelow;    
  else {
    if (placementString.size ()) {
      stringstream s;
      
      s <<
        "stress placement \"" << placementString <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());    
    }
  }

  // create the articulation  
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulation::kStress,
        placementKind);
      
  fCurrentArticulations.push_back (articulation);
}

void mxmlTree2MsrTranslator::visitStart ( S_unstress& elt )
{
   int inputLineNumber =
    elt->getInputLineNumber ();

 if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_unstress" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind placementKind = kPlacementNone;

  if      (placementString == "above")
    placementKind = kPlacementAbove;
  else if (placementString == "below")
    placementKind = kPlacementBelow;    
  else {
    if (placementString.size ()) {
      stringstream s;
      
      s <<
        "unstress placement \"" << placementString <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());    
    }
  }

  // create the articulation  
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulation::kUnstress,
        placementKind);
      
  fCurrentArticulations.push_back (articulation);
}

void mxmlTree2MsrTranslator::visitStart ( S_detached_legato& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

 if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_detached_legato" <<
      ", line " << inputLineNumber <<
      endl;
  }
    
  // placement
  
  string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind placementKind = kPlacementNone;

  if      (placementString == "above")
    placementKind = kPlacementAbove;
  else if (placementString == "below")
    placementKind = kPlacementBelow;    
  else {
    if (placementString.size ()) {
      stringstream s;
      
      s <<
        "detached-legato placement \"" << placementString <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());    
    }
  }

  // create the articulation  
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulation::kDetachedLegato,
        placementKind);
      
  fCurrentArticulations.push_back (articulation);
}

void mxmlTree2MsrTranslator::visitStart ( S_strong_accent& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_strong_accent" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind placementKind = kPlacementNone;

  if      (placementString == "above")
    placementKind = kPlacementAbove;
  else if (placementString == "below")
    placementKind = kPlacementBelow;    
  else {
    if (placementString.size ()) {
      stringstream s;
      
      s <<
        "strong-accent placement \"" << placementString <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());    
    }
  }

  // create the articulation  
  // type : upright inverted  (Binchois20.xml) // JMI
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulation::kStrongAccent,
        placementKind);
      
  fCurrentArticulations.push_back (articulation);
}

void mxmlTree2MsrTranslator::visitStart ( S_tenuto& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_tenuto" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind placementKind = kPlacementNone;

  if      (placementString == "above")
    placementKind = kPlacementAbove;
  else if (placementString == "below")
    placementKind = kPlacementBelow;    
  else {
    if (placementString.size ()) {
      stringstream s;
      
      s <<
        "tenuto placement \"" << placementString <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());    
    }
  }

  // create the articulation  
  // type : upright inverted  (Binchois20.xml) // JMI
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulation::kTenuto,
        placementKind);
      
  fCurrentArticulations.push_back (articulation);
}

void mxmlTree2MsrTranslator::visitStart ( S_doit& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_doit" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind placementKind = kPlacementNone;

  if      (placementString == "above")
    placementKind = kPlacementAbove;
  else if (placementString == "below")
    placementKind = kPlacementBelow;    
  else {
    if (placementString.size ()) {
      stringstream s;
      
      s <<
        "doit placement \"" << placementString <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());    
    }
  }

  // create the articulation  
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulation::kDoit,
        placementKind);
      
  fCurrentArticulations.push_back (articulation);
}

void mxmlTree2MsrTranslator::visitStart ( S_falloff& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_falloff" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind placementKind = kPlacementNone;

  if      (placementString == "above")
    placementKind = kPlacementAbove;
  else if (placementString == "below")
    placementKind = kPlacementBelow;    
  else {
    if (placementString.size ()) {
      stringstream s;
      
      s <<
        "falloff placement \"" << placementString <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());    
    }
  }

  // create the articulation  
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulation::kFalloff,
        placementKind);
      
  fCurrentArticulations.push_back (articulation);
}

void mxmlTree2MsrTranslator::visitStart ( S_plop& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_plop" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind placementKind = kPlacementNone;

  if      (placementString == "above")
    placementKind = kPlacementAbove;
  else if (placementString == "below")
    placementKind = kPlacementBelow;    
  else {
    if (placementString.size ()) {
      stringstream s;
      
      s <<
        "plop placement \"" << placementString <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());    
    }
  }

  // create the articulation  
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulation::kPlop,
        placementKind);
      
  fCurrentArticulations.push_back (articulation);
}

void mxmlTree2MsrTranslator::visitStart ( S_scoop& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_scoop" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind placementKind = kPlacementNone;

  if      (placementString == "above")
    placementKind = kPlacementAbove;
  else if (placementString == "below")
    placementKind = kPlacementBelow;    
  else {
    if (placementString.size ()) {
      stringstream s;
      
      s <<
        "scoop placement \"" << placementString <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());    
    }
  }

  // create the articulation  
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulation::kScoop,
        placementKind);
      
  fCurrentArticulations.push_back (articulation);
}

void mxmlTree2MsrTranslator::visitEnd ( S_articulations& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_articulations" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // JMI
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_arpeggiate& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_arpeggiate" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind placementKind = kPlacementNone; // default value

  if      (placementString == "above")
    placementKind = kPlacementAbove;
  else if (placementString == "below")
    placementKind = kPlacementBelow;
  else {
    if (placementString.size ()) {
      stringstream s;
      
      s <<
        "arpeggiate placement \"" << placementString <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());    
    }
  }

  // number

  int number = elt->getAttributeIntValue ("number", 0);

  // direction
  
  string directionString = elt->getAttributeValue ("direction");

  msrDirectionKind directionKind = kDirectionNone; // default value
  
  if      (directionString == "up")
    directionKind = kDirectionUp;
  else if (directionString == "down")
    directionKind = kDirectionDown;
  else {
    if (directionString.size ()) {
      stringstream s;
      
      s <<
        "arpeggiate direction \"" << directionString << "\"" << "is unknown";
        
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }
  
  // create the arpeggiato  
  S_msrArpeggiato
    arpeggiato =
      msrArpeggiato::create (
        inputLineNumber,
        placementKind,
        directionKind,
        number);
      
  fCurrentArticulations.push_back (arpeggiato);
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_non_arpeggiate& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_non_arpeggiate" << // JMI
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind placementKind = kPlacementNone; // default value

  if      (placementString == "above")
    placementKind = kPlacementAbove;
  else if (placementString == "below")
    placementKind = kPlacementBelow;
  else {
    if (placementString.size ()) {
      stringstream s;
      
      s <<
        "non-arpeggiate placement \"" << placementString <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());    
    }
  }

  // type

  string typeString = elt->getAttributeValue ("type");

  msrNonArpeggiato::msrNonArpeggiatoTypeKind
    nonArpeggiatoTypeKind =
      msrNonArpeggiato::kNonArpeggiatoTypeNone; // default value

  if      (typeString == "top")
    nonArpeggiatoTypeKind =
      msrNonArpeggiato::kNonArpeggiatoTypeTop;
  else if (typeString == "bottom")
    nonArpeggiatoTypeKind =
      msrNonArpeggiato::kNonArpeggiatoTypeBottom;
  else {
    if (typeString.size ()) {
      
      stringstream s;
      
      s <<
        "non-arpeggiate type \"" << typeString <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());    
    }
  }

  // number

  int number = elt->getAttributeIntValue ("number", 0);

  // create the non arpeggiato  
  S_msrNonArpeggiato
    nonArpeggiato =
      msrNonArpeggiato::create (
        inputLineNumber,
        placementKind,
        nonArpeggiatoTypeKind,
        number);
      
  fCurrentArticulations.push_back (nonArpeggiato);
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_technical& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_technical" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fOnGoingTechnical = true;
}

void mxmlTree2MsrTranslator::visitEnd ( S_technical& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_technical" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fOnGoingTechnical = false;
}

void mxmlTree2MsrTranslator::visitStart ( S_arrow& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_arrow" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string placement = elt->getAttributeValue ("placement");

  msrPlacementKind
    arrowPlacementKind = kPlacementNone;

  if      (placement == "above")
    arrowPlacementKind = kPlacementAbove;
  else if (placement == "below")
    arrowPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "arrow placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kArrow,
        arrowPlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void mxmlTree2MsrTranslator::visitStart ( S_bend_alter& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_bend_alter" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBendAlterValue = (float)(*elt);
}
  
void mxmlTree2MsrTranslator::visitStart ( S_bend& elt ) // JMI
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_bend" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void mxmlTree2MsrTranslator::visitEnd ( S_bend& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_bend" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string placement = elt->getAttributeValue ("placement");

  msrPlacementKind
    bendPlacementKind = kPlacementNone;

  if      (placement == "above")
    bendPlacementKind = kPlacementAbove;
  else if (placement == "below")
    bendPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    stringstream s;
    
    s <<
      "bend placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrTechnicalWithFloat
    technicalWithFloat =
      msrTechnicalWithFloat::create (
        inputLineNumber,
        msrTechnicalWithFloat::kBend,
        fBendAlterValue,
        bendPlacementKind);
      
  fCurrentTechnicalWithFloatsList.push_back (
    technicalWithFloat);
}

void mxmlTree2MsrTranslator::visitStart ( S_double_tongue& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_double_tongue" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string
    placement =
      elt->getAttributeValue ("placement");

  msrPlacementKind
    doubleTonguePlacementKind = kPlacementNone;

  if      (placement == "above")
    doubleTonguePlacementKind = kPlacementAbove;
  else if (placement == "below")
    doubleTonguePlacementKind = kPlacementBelow;
  else if (placement.size ()) {    
    stringstream s;
    
    s <<
      "double-tongue placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kDoubleTongue,
        doubleTonguePlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void mxmlTree2MsrTranslator::visitStart ( S_down_bow& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_down_bow" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string
    placement =
      elt->getAttributeValue ("placement");

  msrPlacementKind
    downBowPlacementKind = kPlacementNone;

  if      (placement == "above")
    downBowPlacementKind = kPlacementAbove;
  else if (placement == "below")
    downBowPlacementKind = kPlacementBelow;
  else if (placement.size ()) {    
    stringstream s;
    
    s <<
      "down-bow placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kDownBow,
        downBowPlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void mxmlTree2MsrTranslator::visitStart ( S_fingering& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_fingering" <<
      ", line " << inputLineNumber <<
      endl;
  }

  int fingeringValue = (int)(*elt);

  if (fOnGoingTechnical) {
    string placement = elt->getAttributeValue ("placement");
  
    msrPlacementKind
      fingeringPlacementKind = kPlacementNone;
  
    if      (placement == "above")
      fingeringPlacementKind = kPlacementAbove;    
    else if (placement == "below")
      fingeringPlacementKind = kPlacementBelow;
    else if (placement.size ()) {    
      stringstream s;
      
      s <<
        "fingering placement \"" << placement <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());    
    }
  
    S_msrTechnicalWithInteger
      technicalWithInteger =
        msrTechnicalWithInteger::create (
          inputLineNumber,
          msrTechnicalWithInteger::kFingering,
          fingeringValue,
          fingeringPlacementKind);
        
    fCurrentTechnicalWithIntegersList.push_back (
      technicalWithInteger);
  }

  else if (fOnGoingFrameNote) {
    fCurrentFrameNoteFingering = fingeringValue;
  }

  else {
    stringstream s;
    
    s <<
      "fingering \"" << fingeringValue <<
      "\" is out of context";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }
}

void mxmlTree2MsrTranslator::visitStart ( S_fingernails& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_fingernails" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string
    placement =
      elt->getAttributeValue ("placement");

  msrPlacementKind
    fingernailsPlacementKind = kPlacementNone;

  if      (placement == "above")
    fingernailsPlacementKind = kPlacementAbove;
  else if (placement == "below")
    fingernailsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    stringstream s;
    
    s <<
      "fingernails placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kFingernails,
        fingernailsPlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void mxmlTree2MsrTranslator::visitStart ( S_fret& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_fret" <<
      ", line " << inputLineNumber <<
      endl;
  }

  int fretValue = (int)(*elt);

  if (fOnGoingTechnical) {
    string placement = elt->getAttributeValue ("placement");
  
    msrPlacementKind
      fretPlacementKind =  kPlacementNone;
  
    if      (placement == "above")
      fretPlacementKind = kPlacementAbove;    
    else if (placement == "below")
      fretPlacementKind = kPlacementBelow;
    else if (placement.size ()) {
      stringstream s;
      
      s <<
        "fret placement \"" << placement <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());    
    }
  
    S_msrTechnicalWithInteger
      technicalWithInteger =
        msrTechnicalWithInteger::create (
          inputLineNumber,
          msrTechnicalWithInteger::kFret,
          fretValue,
          fretPlacementKind);
        
    fCurrentTechnicalWithIntegersList.push_back (
      technicalWithInteger);
  }

  else if (fOnGoingFrameNote) {
    fCurrentFrameNoteFretNumber = fretValue;
  }

  else {
    stringstream s;
    
    s <<
      "fret \"" << fretValue <<
      "\" is out of context";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxmlTree2MsrTranslator::visitStart ( S_hammer_on& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_hammer_on" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // type
  
  string hammerOnType = elt->getAttributeValue ("type");
  
  msrTechnicalTypeKind hammerOnTechnicalTypeKind = k_NoTechnicalType;

  if      (hammerOnType == "start")
    hammerOnTechnicalTypeKind = kTechnicalTypeStart;
  else if (hammerOnType == "stop")
    hammerOnTechnicalTypeKind = kTechnicalTypeStop;
  else {
    if (hammerOnType.size ()) {
      stringstream s;
      
      s <<
        "hammer-on type \"" << hammerOnType <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }   
  }

  // placement
  
  string hammerOnValue = elt->getValue ();
    
  string placement = elt->getAttributeValue ("placement");

  msrPlacementKind
    hammerOnPlacementKind = kPlacementNone;

  if      (placement == "above")
    hammerOnPlacementKind = kPlacementAbove;    
  else if (placement == "below")
    hammerOnPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    stringstream s;
    
    s <<
      "hammer-on placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrTechnicalWithString
    technicalWithString =
      msrTechnicalWithString::create (
        inputLineNumber,
        msrTechnicalWithString::kHammerOn,
        hammerOnTechnicalTypeKind,
        hammerOnValue,
        hammerOnPlacementKind);
      
  fCurrentTechnicalWithStringsList.push_back (technicalWithString);
}

void mxmlTree2MsrTranslator::visitStart ( S_handbell& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_handbell" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string handBellValue = elt->getValue ();

  // placement
  
  string
    placement =
      elt->getAttributeValue ("placement");

  msrPlacementKind
    handbellPlacementKind = kPlacementNone;

  if      (placement == "above")
    handbellPlacementKind = kPlacementAbove;
  else if (placement == "below")
    handbellPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    stringstream s;
    
    s <<
      "handbell placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrTechnicalWithString
    technicalWithString =
      msrTechnicalWithString::create (
        inputLineNumber,
        msrTechnicalWithString::kHandbell,
        k_NoTechnicalType,
        handBellValue,
        handbellPlacementKind);
      
  fCurrentTechnicalWithStringsList.push_back (technicalWithString);
}

void mxmlTree2MsrTranslator::visitStart ( S_harmonic& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_harmonic" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string
    placement =
      elt->getAttributeValue ("placement");

  msrPlacementKind
    harmonicPlacementKind = kPlacementNone;

  if      (placement == "above")
    harmonicPlacementKind = kPlacementAbove;
  else if (placement == "below")
    harmonicPlacementKind = kPlacementBelow;
  else if (placement.size ()) {    
    stringstream s;
    
    s <<
      "harmonic placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kHarmonic,
        harmonicPlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void mxmlTree2MsrTranslator::visitStart ( S_heel& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_heel" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string
    placement =
      elt->getAttributeValue ("placement");

  msrPlacementKind
    heelPlacementKind = kPlacementNone;

  if      (placement == "above")
    heelPlacementKind = kPlacementAbove;
  else if (placement == "below")
    heelPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    stringstream s;
    
    s <<
      "heel placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kHeel,
        heelPlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void mxmlTree2MsrTranslator::visitStart ( S_hole& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_hole" <<
      ", line " << inputLineNumber <<
      endl;
  }
    
  string
    placement =
      elt->getAttributeValue ("placement");

  msrPlacementKind
    holePlacementKind = kPlacementNone;

  if      (placement == "above")
    holePlacementKind = kPlacementAbove;
  else if (placement == "below")
    holePlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    stringstream s;
    
    s <<
      "hole placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kHole,
        holePlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void mxmlTree2MsrTranslator::visitStart ( S_open_string& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_open_string" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string
    placement =
      elt->getAttributeValue ("placement");

  msrPlacementKind
    openStringPlacementKind = kPlacementNone;

  if      (placement == "above")
    openStringPlacementKind = kPlacementAbove;
  else if (placement == "below")
    openStringPlacementKind = kPlacementBelow;
  else if (placement.size ()) {    
    stringstream s;
    
    s <<
      "open-string placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kOpenString,
        openStringPlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void mxmlTree2MsrTranslator::visitStart ( S_other_technical& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_other_technical" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string otherTechnicalValue = elt->getValue ();
    
  string
    placement =
      elt->getAttributeValue ("placement");

  msrPlacementKind
    otherTechnicalWithStringPlacementKind = kPlacementNone;

  if      (placement == "above")
    otherTechnicalWithStringPlacementKind = kPlacementAbove;
  else if (placement == "below")
    otherTechnicalWithStringPlacementKind = kPlacementBelow;
  else if (placement.size ()) {    
    stringstream s;
    
    s <<
      "other-technical placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrTechnicalWithString
    technicalWithString =
      msrTechnicalWithString::create (
        inputLineNumber,
        msrTechnicalWithString::kOtherTechnical,
        k_NoTechnicalType,
        otherTechnicalValue,
        otherTechnicalWithStringPlacementKind);
      
  fCurrentTechnicalWithStringsList.push_back (technicalWithString);
}

void mxmlTree2MsrTranslator::visitStart ( S_pluck& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_pluck" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string pluckValue = elt->getValue ();
    
  string
    placement =
      elt->getAttributeValue ("placement");

  msrPlacementKind
    pluckPlacementKind = kPlacementNone;

  if      (placement == "above")
    pluckPlacementKind = kPlacementAbove;
  else if (placement == "below")
    pluckPlacementKind = kPlacementBelow;
  else if (placement.size ()) {    
    stringstream s;
    
    s <<
      "pluck placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrTechnicalWithString
    technicalWithString =
      msrTechnicalWithString::create (
        inputLineNumber,
        msrTechnicalWithString::kPluck,
        k_NoTechnicalType,
        pluckValue,
        pluckPlacementKind);
      
  fCurrentTechnicalWithStringsList.push_back (technicalWithString);
}

void mxmlTree2MsrTranslator::visitStart ( S_pull_off& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_pull_off" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string pullOffValue = elt->getValue ();
    
  // type
  
  string pullOffType = elt->getAttributeValue ("type");
  
  msrTechnicalTypeKind pullOffTechnicalTypeKind = k_NoTechnicalType;

  if      (pullOffType == "start")
    pullOffTechnicalTypeKind = kTechnicalTypeStart;
  else if (pullOffType == "stop")
    pullOffTechnicalTypeKind = kTechnicalTypeStop;
  else {
    if (pullOffType.size ()) {
      stringstream s;
      
      s <<
        "pull-off type \"" << pullOffType <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }   
  }

  // placement
  
  string
    placement =
      elt->getAttributeValue ("placement");

  msrPlacementKind
    pullOffPlacementKind = kPlacementNone;

  if      (placement == "above")
    pullOffPlacementKind = kPlacementAbove;
  else if (placement == "below")
    pullOffPlacementKind = kPlacementBelow;
  else if (placement.size ()) {    
    stringstream s;
    
    s <<
      "pull-off placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrTechnicalWithString
    technicalWithString =
      msrTechnicalWithString::create (
        inputLineNumber,
        msrTechnicalWithString::kPullOff,
        pullOffTechnicalTypeKind,
        pullOffValue,
        pullOffPlacementKind);
      
  fCurrentTechnicalWithStringsList.push_back (technicalWithString);
}

void mxmlTree2MsrTranslator::visitStart ( S_snap_pizzicato& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_snap_pizzicato" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string
    placement =
      elt->getAttributeValue ("placement");

  msrPlacementKind
    snapPizzicatoPlacementKind = kPlacementNone;

  if      (placement == "above")
    snapPizzicatoPlacementKind = kPlacementAbove;
  else if (placement == "below")
    snapPizzicatoPlacementKind = kPlacementBelow;
  else if (placement.size ()) {    
    stringstream s;
    
    s <<
      "snap-pizzicato placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kSnapPizzicato,
        snapPizzicatoPlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void mxmlTree2MsrTranslator::visitStart ( S_stopped& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_stopped" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string
    placement =
      elt->getAttributeValue ("placement");

  msrPlacementKind
    stoppedPlacementKind = kPlacementNone;

  if      (placement == "above")
    stoppedPlacementKind = kPlacementAbove;
  else if (placement == "below")
    stoppedPlacementKind = kPlacementBelow;
  else if (placement.size ()) {    
    stringstream s;
    
    s <<
      "stopped placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kStopped,
        stoppedPlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void mxmlTree2MsrTranslator::visitStart ( S_string& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_string" <<
      ", line " << inputLineNumber <<
      endl;
  }

/*
        <notations>
          <technical>
            <string>5</string>
            <fret>0</fret>
          </technical>
        </notations>
*/

  string stringValue = elt->getValue();

  int stringIntegerValue;

  istringstream inputStream (stringValue);

  inputStream >> stringIntegerValue;

  if (! stringValue.size ()) {
    stringstream s;
    
    stringIntegerValue = 0;

    s <<
      "string value \"" << stringValue <<
      "\" is empoty, '" << stringIntegerValue << "' is assumed";
    
    msrMusicXMLWarning (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      s.str ());    
  }

  if (fOnGoingTechnical) {
    string placement = elt->getAttributeValue ("placement");
  
    msrPlacementKind
      stringPlacementKind = kPlacementNone;
  
    if      (placement == "above")
      stringPlacementKind = kPlacementAbove;
    else if (placement == "below")
      stringPlacementKind = kPlacementBelow;
    else if (placement.size ()) {    
      stringstream s;
      
      s <<
        "string placement \"" << placement <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());    
    }
  
    S_msrTechnicalWithInteger
      technicalWithInteger =
        msrTechnicalWithInteger::create (
          inputLineNumber,
          msrTechnicalWithInteger::kString,
          stringIntegerValue,
          stringPlacementKind);
        
    fCurrentTechnicalWithIntegersList.push_back (
      technicalWithInteger);
  }

  else if (fOnGoingFrame) {
    fCurrentFrameNoteStringNumber = stringIntegerValue;
  }

  else {
    stringstream s;
    
    s <<
      "string \"" << stringValue <<
      "\" is out of context";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxmlTree2MsrTranslator::visitStart ( S_tap& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_tap" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string
    placement =
      elt->getAttributeValue ("placement");

  msrPlacementKind
    tapPlacementKind = kPlacementNone;

  if      (placement == "above")
    tapPlacementKind = kPlacementAbove;
  else if (placement == "below")
    tapPlacementKind = kPlacementBelow;
  else if (placement.size ()) {    
    stringstream s;
    
    s <<
      "tap placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kTap,
        tapPlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void mxmlTree2MsrTranslator::visitStart ( S_thumb_position& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_thumb_position" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string
    placement =
      elt->getAttributeValue ("placement");

  msrPlacementKind
    thumbPositionPlacementKind = kPlacementNone;

  if      (placement == "above")
    thumbPositionPlacementKind = kPlacementAbove;    
  else if (placement == "below")
    thumbPositionPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    stringstream s;
    
    s <<
      "thumb-position placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kThumbPosition,
        thumbPositionPlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void mxmlTree2MsrTranslator::visitStart ( S_toe& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_toe" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string
    placement =
      elt->getAttributeValue ("placement");

  msrPlacementKind
    toePlacementKind = kPlacementNone;

  if      (placement == "above")
    toePlacementKind = kPlacementAbove;    
  else if (placement == "below")
    toePlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    stringstream s;
    
    s <<
      "toe placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kToe,
        toePlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void mxmlTree2MsrTranslator::visitStart ( S_triple_tongue& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_triple_tongue" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string
    placement =
      elt->getAttributeValue ("placement");

  msrPlacementKind
    tripleTonguePlacementKind = kPlacementNone;

  if      (placement == "above")
    tripleTonguePlacementKind = kPlacementAbove;    
  else if (placement == "below")
    tripleTonguePlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    stringstream s;
    
    s <<
      "triple-tongue placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnical::kTripleTongue,
        tripleTonguePlacementKind);
      
  fCurrentTechnicalsList.push_back (technical);
}

void mxmlTree2MsrTranslator::visitStart ( S_up_bow& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_up_bow" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string
    placement =
      elt->getAttributeValue ("placement");

  msrPlacementKind
    upBowPlacementKind = kPlacementNone;

  if      (placement == "above")
    upBowPlacementKind = kPlacementAbove;    
  else if (placement == "below")
    upBowPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    stringstream s;
    
    s <<
      "up-bow placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
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
void mxmlTree2MsrTranslator::visitStart ( S_fermata& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_fermata" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string fermataTextValue = elt->getValue ();

  // kind
  
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
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }   
  }

  // type
  
  string fermataTypeValue = elt->getAttributeValue ("type");
  
  msrFermata::msrFermataTypeKind
    fermataTypeKind =
      msrFermata::kFermataTypeNone; // default value

  if      (fermataTypeValue == "upright")
    fermataTypeKind = msrFermata::kFermataTypeUpright;
    
  else if (fermataTypeValue == "inverted")
    fermataTypeKind = msrFermata::kFermataTypeInverted;
    
  else {
    if (fermataTypeValue.size ()) {
      stringstream s;
      
      s <<
        "fermata type \"" << fermataTypeValue <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }   
  }

  // create the fermata
  S_msrFermata
    fermata =
      msrFermata::create (
        inputLineNumber,
        fermataKind,
        fermataTypeKind);
        
  fCurrentArticulations.push_back (fermata);
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_ornaments& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_ornaments" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void mxmlTree2MsrTranslator::visitStart ( S_tremolo& elt )
{   
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting tremolo" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // value (tremolo marks number)
  
  string tremoloMarksNumberString =
    elt->getValue ();
    
  int tremoloMarksNumber = (int)(*elt);

  if (! tremoloMarksNumberString.size ()) {
    stringstream s;

    tremoloMarksNumber = 1;
    
    s <<
      "--> tremolo value is missing, '" << tremoloMarksNumber << "' assumed";
    
    msrMusicXMLWarning (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      s.str ());
  }

  if (tremoloMarksNumber < 0 || tremoloMarksNumber > 8) { // JMI what does 0 mean?
    stringstream s;
    
    s <<
      "tremolo value \"" << tremoloMarksNumber <<
      "\" should be between 0 and 8";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // type
  
  string tremoloType = elt->getAttributeValue ("type");

  fCurrentTremoloTypeKind = kTremoloTypeSingle; // default value
    
  if      (tremoloType == "single")
    fCurrentTremoloTypeKind = kTremoloTypeSingle;
    
  else if (tremoloType == "start")
    fCurrentTremoloTypeKind = kTremoloTypeStart;
    
  else if (tremoloType == "stop")
    fCurrentTremoloTypeKind = kTremoloTypeStop;
    
  else if (tremoloType.size ()) {
    stringstream s;
    
    s <<
      "tremolo type \"" << tremoloType <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // placement
  
  string
    tremoloPlacement =
      elt->getAttributeValue ("placement");

  msrPlacementKind
    singleTremoloPlacementKind = kPlacementNone;
      
  msrPlacementKind
    doubleTremoloPlacementKind = kPlacementNone;

  if      (tremoloPlacement == "above") {
    switch (fCurrentTremoloTypeKind) {
      case k_NoTremoloType:
        // just to avoid a compiler message
        break;
        
      case kTremoloTypeSingle:
        singleTremoloPlacementKind = kPlacementAbove;
        break;
        
      case kTremoloTypeStart:
      case kTremoloTypeStop:
        doubleTremoloPlacementKind = kPlacementAbove;
        break;
    } // switch
  }
  
  else if (tremoloPlacement == "below") {
    switch (fCurrentTremoloTypeKind) {
      case k_NoTremoloType:
        // just to avoid a compiler message
        break;
        
      case kTremoloTypeSingle:
        singleTremoloPlacementKind = kPlacementBelow;
        break;
        
      case kTremoloTypeStart:
      case kTremoloTypeStop:
        doubleTremoloPlacementKind = kPlacementBelow;
        break;
    } // switch
  }
    
  else if (tremoloPlacement.size ()) {
    
    stringstream s;
    
    s <<
      "tremolo placement \"" << tremoloPlacement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  // handle double tremolos
  switch (fCurrentTremoloTypeKind) {
    case k_NoTremoloType:
      // just to avoid a compiler message
      break;
        
    case kTremoloTypeSingle:
      // create a single tremolo, it will be attached to current note
      // in attachCurrentSingleTremoloToNote()
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceTremolos) {
        fLogOutputStream <<
          "Creating a single tremolo" <<
          ", line " << inputLineNumber <<
          ", " <<
          singularOrPlural (
            tremoloMarksNumber, "mark", "marks") <<
          ", placement : " <<
          msrPlacementKindAsString (
            singleTremoloPlacementKind) <<
          endl;
      }
#endif

      fCurrentSingleTremolo =
        msrSingleTremolo::create (
          inputLineNumber,
          tremoloMarksNumber,
          singleTremoloPlacementKind);
      break;
      
    case kTremoloTypeStart:
  //    if (! fCurrentDoubleTremolo) { JMI
      {
        // fetch current voice
        S_msrVoice
          currentVoice =
            fetchVoiceFromPart (
              inputLineNumber,
              fCurrentStaffNumberToInsertInto, // JMI fCurrentMusicXMLStaffNumber,
              fCurrentMusicXMLVoiceNumber);

        // create a double tremolo start
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceTremolos) {
          fLogOutputStream <<
            "Creating a double tremolo" <<
            ", line " << inputLineNumber <<
            ", " <<
            singularOrPlural (
              tremoloMarksNumber, "mark", "marks") <<
            ", placement : " <<
            msrPlacementKindAsString (
              doubleTremoloPlacementKind) <<
            endl;
        }
#endif
      
        fCurrentDoubleTremolo =
          msrDoubleTremolo::create (
            inputLineNumber,
            msrDoubleTremolo::kNotesDoubleTremolo,
            kTremoloTypeStart,
            tremoloMarksNumber,
            doubleTremoloPlacementKind);
      }
      
/*
      else {
        stringstream s;

        s <<
          "<tremolo/> start when a current double tremolo is already open";

        msrMusicXMLError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());    
      }
*/
      break;

    case kTremoloTypeStop:
      if (fCurrentDoubleTremolo) {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceTremolos) {
          fLogOutputStream <<
            "Meeting a double tremolo stop" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        // it will be handled in
        // handleStandaloneOrDoubleTremoloNoteOrGraceNoteOrRest()
      }
      
      else {
        stringstream s;

        s <<
          "<tremolo/> stop whit no preceeding <tremolo/> start";

        msrMusicXMLError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());    
      }
      break;
  } // switch
}

void mxmlTree2MsrTranslator::visitStart ( S_trill_mark& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_trill_mark" <<
      ", line " << inputLineNumber <<
      endl;
  }

 // type : upright inverted  (Binchois20.xml) JMI

  // placement
  
  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    ornamentPlacementKind = kPlacementNone;

  if      (placement == "above")
    ornamentPlacementKind = kPlacementAbove;
  else if (placement == "below")
    ornamentPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrOrnament
    ornament =
      msrOrnament::create (
        inputLineNumber,
        msrOrnament::kOrnamentTrill,
        ornamentPlacementKind);
      
  fCurrentOrnamentsList.push_back (ornament);
}

void mxmlTree2MsrTranslator::visitStart ( S_dashes& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_dashes" <<
      ", line " << inputLineNumber <<
      endl;
  }

 // type : upright inverted  (Binchois20.xml) JMI

  // number

  int dashesNumber = elt->getAttributeIntValue ("number", 0); // JMI

  // type
  
  string dashesType = elt->getAttributeValue ("type");
  
  msrSpannerTypeKind fDashesSpannerTypeKind = k_NoSpannerType;

  if      (dashesType == "start")
    fDashesSpannerTypeKind = kSpannerTypeStart;
  else if (dashesType == "continue")
    fDashesSpannerTypeKind = kSpannerTypeContinue;
  else if (dashesType == "stop")
    fDashesSpannerTypeKind = kSpannerTypeStop;
  else {
    if (dashesType.size ()) {
      stringstream s;
      
      s <<
        "dashes type \"" << dashesType <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }   
  }

  // placement
  
  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    ornamentPlacementKind = kPlacementNone;

  if      (placement == "above")
    ornamentPlacementKind = kPlacementAbove;
  else if (placement == "below")
    ornamentPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrSpanner
    spanner =
      msrSpanner::create (
        inputLineNumber,
        dashesNumber,
        msrSpanner::kSpannerDashes,
        fDashesSpannerTypeKind,
        ornamentPlacementKind,
        nullptr); // will be set later REMOVE??? JMI
      
  fCurrentSpannersList.push_back (spanner);
}

void mxmlTree2MsrTranslator::visitStart ( S_wavy_line& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_wavy_line" <<
      ", line " << inputLineNumber <<
      endl;
  }

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

  // number

  int wavyLineNumber = elt->getAttributeIntValue ("number", 0); // JMI

  // type
  
  string wavyLineType = elt->getAttributeValue ("type");
  
  msrSpannerTypeKind fWavyLineSpannerTypeKind = k_NoSpannerType;

  if      (wavyLineType == "start")
    fWavyLineSpannerTypeKind = kSpannerTypeStart;
  else if (wavyLineType == "continue")
    fWavyLineSpannerTypeKind = kSpannerTypeContinue;
  else if (wavyLineType == "stop")
    fWavyLineSpannerTypeKind = kSpannerTypeStop;
  else {
    if (wavyLineType.size ()) {
      stringstream s;
      
      s <<
        "wavy-line type \"" << wavyLineType <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }   
  }

  // placement

  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    ornamentPlacementKind = kPlacementNone;

  if      (placement == "above")
    ornamentPlacementKind = kPlacementAbove;
  else if (placement == "below")
    ornamentPlacementKind = kPlacementBelow;
  else if (placement.size ()) {    
    stringstream s;
    
    s <<
      "wavy-line placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }
  
  S_msrSpanner
    spanner =
      msrSpanner::create (
        inputLineNumber,
        wavyLineNumber,
        msrSpanner::kSpannerWavyLine,
        fWavyLineSpannerTypeKind,
        ornamentPlacementKind,
        nullptr); // will be set later REMOVE??? JMI
      
  fCurrentSpannersList.push_back (spanner);

  switch (fWavyLineSpannerTypeKind) {
    case kSpannerTypeStart:
      // remember this wavy line spanner start
      fCurrentWavyLineSpannerStart = spanner;
      break;
      
    case kSpannerTypeStop:
      // set spanner two-way sidelinks
      // between both ends of the wavy line spanner
      if (! fCurrentWavyLineSpannerStart) {
        stringstream s;
        
        s <<
          "wavy-line stop found without corresponding start, ignoring it";
        
        msrMusicXMLWarning (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          s.str ());    
      }
      else {
        spanner->
          setSpannerOtherEndSidelink (
            fCurrentWavyLineSpannerStart);
        // forget this wavy line spanner start
      }
      fCurrentWavyLineSpannerStart = nullptr;
      break;
      
    case kSpannerTypeContinue:
      break;
      
    case k_NoSpannerType:
      // JMI ???
      break;
  } // switch
}

void mxmlTree2MsrTranslator::visitStart ( S_turn& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_turn" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    ornamentPlacementKind = kPlacementNone;

  if      (placement == "above")
    ornamentPlacementKind = kPlacementAbove;
  else if (placement == "below")
    ornamentPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
  
    stringstream s;
    
    s <<
      "turn placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrOrnament
    ornament =
      msrOrnament::create (
        inputLineNumber,
        msrOrnament::kOrnamentTurn,
        ornamentPlacementKind);
      
  fCurrentOrnamentsList.push_back (ornament);
}

void mxmlTree2MsrTranslator::visitStart ( S_inverted_turn& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_inverted_turn" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    ornamentPlacementKind = kPlacementNone;

  if      (placement == "above")
    ornamentPlacementKind = kPlacementAbove;
  else if (placement == "below")
    ornamentPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "inverted-turn placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrOrnament
    ornament =
      msrOrnament::create (
        inputLineNumber,
        msrOrnament::kOrnamentInvertedTurn,
        ornamentPlacementKind);
      
  fCurrentOrnamentsList.push_back (ornament);
}

void mxmlTree2MsrTranslator::visitStart ( S_delayed_turn& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_delayed_turn" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    ornamentPlacementKind = kPlacementNone;

  if      (placement == "above")
    ornamentPlacementKind = kPlacementAbove;
  else if (placement == "below")
    ornamentPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "delayed-turn placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrOrnament
    ornament =
      msrOrnament::create (
        inputLineNumber,
        msrOrnament::kOrnamentDelayedTurn,
        ornamentPlacementKind);
      
  fCurrentOrnamentsList.push_back (ornament);
}

void mxmlTree2MsrTranslator::visitStart ( S_delayed_inverted_turn& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_delayed_inverted_turn" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    ornamentPlacementKind = kPlacementNone;

  if      (placement == "above")
    ornamentPlacementKind = kPlacementAbove;
  else if (placement == "below")
    ornamentPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "delayed-inverted-turn placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrOrnament
    ornament =
      msrOrnament::create (
        inputLineNumber,
        msrOrnament::kOrnamentDelayedInvertedTurn,
        ornamentPlacementKind);
      
  fCurrentOrnamentsList.push_back (ornament);
}

void mxmlTree2MsrTranslator::visitStart ( S_vertical_turn& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_vertical_turn" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    ornamentPlacementKind = kPlacementNone;

  if      (placement == "above")
    ornamentPlacementKind = kPlacementAbove;
  else if (placement == "below")
    ornamentPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "vertical-turn placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrOrnament
    ornament =
      msrOrnament::create (
        inputLineNumber,
        msrOrnament::kOrnamentVerticalTurn,
        ornamentPlacementKind);
      
  fCurrentOrnamentsList.push_back (ornament);
}

void mxmlTree2MsrTranslator::visitStart ( S_mordent& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_mordent" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    ornamentPlacementKind = kPlacementNone;

  if      (placement == "above")
    ornamentPlacementKind = kPlacementAbove;
  else if (placement == "below")
    ornamentPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "mordent placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrOrnament
    ornament =
      msrOrnament::create (
        inputLineNumber,
        msrOrnament::kOrnamentMordent,
        ornamentPlacementKind);
      
  fCurrentOrnamentsList.push_back (ornament);
}

void mxmlTree2MsrTranslator::visitStart ( S_inverted_mordent& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_inverted_mordent" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    ornamentPlacementKind = kPlacementNone;

  if      (placement == "above")
    ornamentPlacementKind = kPlacementAbove;
  else if (placement == "below")
    ornamentPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "inverted-mordent placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrOrnament
    ornament =
      msrOrnament::create (
        inputLineNumber,
        msrOrnament::kOrnamentInvertedMordent,
        ornamentPlacementKind);
      
  fCurrentOrnamentsList.push_back (ornament);
}

void mxmlTree2MsrTranslator::visitStart ( S_schleifer& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_schleifer" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    ornamentPlacementKind = kPlacementNone;

  if      (placement == "above")
    ornamentPlacementKind = kPlacementAbove;
  else if (placement == "below")
    ornamentPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "schleifer placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrOrnament
    ornament =
      msrOrnament::create (
        inputLineNumber,
        msrOrnament::kOrnamentSchleifer,
        ornamentPlacementKind);
      
  fCurrentOrnamentsList.push_back (ornament);
}

void mxmlTree2MsrTranslator::visitStart ( S_shake& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_shake" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    ornamentPlacementKind = kPlacementNone;

  if      (placement == "above")
    ornamentPlacementKind = kPlacementAbove;
  else if (placement == "below")
    ornamentPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "shake placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrOrnament
    ornament =
      msrOrnament::create (
        inputLineNumber,
        msrOrnament::kOrnamentShake,
        ornamentPlacementKind);
      
  fCurrentOrnamentsList.push_back (ornament);
}

void mxmlTree2MsrTranslator::visitStart ( S_accidental_mark& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_accidental_mark" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string accidentalMark = elt->getValue ();

  msrAlterationKind
    currentOrnamentAccidentalMark =
      k_NoAlteration;
    
  if      (accidentalMark == "double-flat")
    currentOrnamentAccidentalMark = kDoubleFlat;
    
  else if (accidentalMark == "three-quarters-flat")
    currentOrnamentAccidentalMark = kSesquiFlat;
    
  else if (accidentalMark == "flat")
    currentOrnamentAccidentalMark = kFlat;
    
  else if (accidentalMark == "quarter-flat")
    currentOrnamentAccidentalMark = kSemiFlat;
    
  else if (accidentalMark == "natural")
    currentOrnamentAccidentalMark = kNatural;
    
  else if (accidentalMark == "quarter-sharp")
    currentOrnamentAccidentalMark = kSemiSharp;
    
  else if (accidentalMark == "sharp")
    currentOrnamentAccidentalMark = kSharp;
    
  else if (accidentalMark == "three-quarters-sharp")
    currentOrnamentAccidentalMark = kSesquiSharp;
    
  else if (accidentalMark == "double-sharp")
    currentOrnamentAccidentalMark = kDoubleSharp;
        
  else if (accidentalMark.size ()) {
    stringstream s;
    
    s <<
      "accidental-mark \"" << accidentalMark <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  // placement
  
  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    ornamentPlacementKind = kPlacementNone;

  if      (placement == "above")
    ornamentPlacementKind = kPlacementAbove;
  else if (placement == "below")
    ornamentPlacementKind = kPlacementBelow;
  else if (placement.size ()) {    
    stringstream s;
    
    s <<
      "accidental-mark placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrOrnament
    ornament =
      msrOrnament::create (
        inputLineNumber,
        msrOrnament::kOrnamentAccidentalMark,
        ornamentPlacementKind);
      
  ornament->
    setOrnamentAccidentalMarkKind (
      currentOrnamentAccidentalMark);

  fCurrentOrnamentsList.push_back (ornament);
}

void mxmlTree2MsrTranslator::visitEnd ( S_ornaments& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_ornaments" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart( S_f& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_f" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // placement
  
  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    dynamicsPlacementKind = kPlacementNone;

  if      (placement == "above")
    dynamicsPlacementKind = kPlacementAbove;
  else if (placement == "below")
    dynamicsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        inputLineNumber,
        msrDynamics::kF,
        dynamicsPlacementKind);
        
  fPendingDynamics.push_back(dynamics);
}
void mxmlTree2MsrTranslator::visitStart( S_ff& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_ff" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    dynamicsPlacementKind = kPlacementNone;

  if      (placement == "above")
    dynamicsPlacementKind = kPlacementAbove;
  else if (placement == "below")
    dynamicsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        inputLineNumber,
        msrDynamics::kFF,
        dynamicsPlacementKind);
        
  fPendingDynamics.push_back(dynamics);
}
void mxmlTree2MsrTranslator::visitStart( S_fff& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_fff" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    dynamicsPlacementKind = kPlacementNone;

  if      (placement == "above")
    dynamicsPlacementKind = kPlacementAbove;
  else if (placement == "below")
    dynamicsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        inputLineNumber,
        msrDynamics::kFFF,
        dynamicsPlacementKind);
        
  fPendingDynamics.push_back(dynamics);
}
void mxmlTree2MsrTranslator::visitStart( S_ffff& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_ffff" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    dynamicsPlacementKind = kPlacementNone;

  if      (placement == "above")
    dynamicsPlacementKind = kPlacementAbove;
  else if (placement == "below")
    dynamicsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        inputLineNumber,
        msrDynamics::kFFFF,
        dynamicsPlacementKind);
        
  fPendingDynamics.push_back(dynamics);
}
void mxmlTree2MsrTranslator::visitStart( S_fffff& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_fffff" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    dynamicsPlacementKind = kPlacementNone;

  if      (placement == "above")
    dynamicsPlacementKind = kPlacementAbove;
  else if (placement == "below")
    dynamicsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        inputLineNumber,
        msrDynamics::kFFFFF,
        dynamicsPlacementKind);
        
  fPendingDynamics.push_back(dynamics);
}
void mxmlTree2MsrTranslator::visitStart( S_ffffff& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_ffffff" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    dynamicsPlacementKind = kPlacementNone;

  if      (placement == "above")
    dynamicsPlacementKind = kPlacementAbove;
  else if (placement == "below")
    dynamicsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        inputLineNumber,
        msrDynamics::kFFFFFF,
        dynamicsPlacementKind);
        
  fPendingDynamics.push_back(dynamics);
}

void mxmlTree2MsrTranslator::visitStart( S_p& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_p" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    dynamicsPlacementKind = kPlacementNone;

  if      (placement == "above")
    dynamicsPlacementKind = kPlacementAbove;
  else if (placement == "below")
    dynamicsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        inputLineNumber,
        msrDynamics::kP,
        dynamicsPlacementKind);
        
  fPendingDynamics.push_back(dynamics);
}
void mxmlTree2MsrTranslator::visitStart( S_pp& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_pp" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    dynamicsPlacementKind = kPlacementNone;

  if      (placement == "above")
    dynamicsPlacementKind = kPlacementAbove;
  else if (placement == "below")
    dynamicsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        inputLineNumber,
        msrDynamics::kPP,
        dynamicsPlacementKind);
        
  fPendingDynamics.push_back(dynamics);
}
void mxmlTree2MsrTranslator::visitStart( S_ppp& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_ppp" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    dynamicsPlacementKind = kPlacementNone;

  if      (placement == "above")
    dynamicsPlacementKind = kPlacementAbove;
  else if (placement == "below")
    dynamicsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        inputLineNumber,
        msrDynamics::kPPP,
        dynamicsPlacementKind);
        
  fPendingDynamics.push_back(dynamics);
}
void mxmlTree2MsrTranslator::visitStart( S_pppp& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_pppp" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    dynamicsPlacementKind = kPlacementNone;

  if      (placement == "above")
    dynamicsPlacementKind = kPlacementAbove;
  else if (placement == "below")
    dynamicsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        inputLineNumber,
        msrDynamics::kPPPP,
        dynamicsPlacementKind);
        
  fPendingDynamics.push_back(dynamics);
}
void mxmlTree2MsrTranslator::visitStart( S_ppppp& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_ppppp" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    dynamicsPlacementKind = kPlacementNone;

  if      (placement == "above")
    dynamicsPlacementKind = kPlacementAbove;    
  else if (placement == "below")
    dynamicsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        inputLineNumber,
        msrDynamics::kPPPPP,
        dynamicsPlacementKind);
        
  fPendingDynamics.push_back(dynamics);
}
void mxmlTree2MsrTranslator::visitStart( S_pppppp& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_pppppp" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    dynamicsPlacementKind = kPlacementNone;

  if      (placement == "above")
    dynamicsPlacementKind = kPlacementAbove;
  else if (placement == "below")
    dynamicsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        inputLineNumber,
        msrDynamics::kPPPPPP,
        dynamicsPlacementKind);
        
  fPendingDynamics.push_back(dynamics);
}


void mxmlTree2MsrTranslator::visitStart( S_mf& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_mf" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    dynamicsPlacementKind = kPlacementNone;

  if      (placement == "above")
    dynamicsPlacementKind = kPlacementAbove;
  else if (placement == "below")
    dynamicsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        inputLineNumber,
        msrDynamics::kMF,
        dynamicsPlacementKind);
        
  fPendingDynamics.push_back(dynamics);
}
void mxmlTree2MsrTranslator::visitStart( S_mp& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_mp" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    dynamicsPlacementKind = kPlacementNone;

  if      (placement == "above")
    dynamicsPlacementKind = kPlacementAbove;
  else if (placement == "below")
    dynamicsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        inputLineNumber,
        msrDynamics::kMP,
        dynamicsPlacementKind);
        
  fPendingDynamics.push_back(dynamics);
}

void mxmlTree2MsrTranslator::visitStart( S_fp& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_fp" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    dynamicsPlacementKind = kPlacementNone;

  if      (placement == "above")
    dynamicsPlacementKind = kPlacementAbove;
  else if (placement == "below")
    dynamicsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        inputLineNumber,
        msrDynamics::kFP,
        dynamicsPlacementKind);
        
  fPendingDynamics.push_back(dynamics);
}
void mxmlTree2MsrTranslator::visitStart( S_fz& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_fz" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    dynamicsPlacementKind = kPlacementNone;

  if      (placement == "above")
    dynamicsPlacementKind = kPlacementAbove;
  else if (placement == "below")
    dynamicsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        inputLineNumber,
        msrDynamics::kFZ,
        dynamicsPlacementKind);
        
  fPendingDynamics.push_back(dynamics);
}

void mxmlTree2MsrTranslator::visitStart( S_rf& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_rf" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    dynamicsPlacementKind = kPlacementNone;

  if      (placement == "above")
    dynamicsPlacementKind = kPlacementAbove;
  else if (placement == "below")
    dynamicsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        inputLineNumber,
        msrDynamics::kRF,
        dynamicsPlacementKind);
        
  fPendingDynamics.push_back(dynamics);
}

void mxmlTree2MsrTranslator::visitStart( S_sf& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_sf" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    dynamicsPlacementKind = kPlacementNone;

  if      (placement == "above")
    dynamicsPlacementKind = kPlacementAbove;
  else if (placement == "below")
    dynamicsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        inputLineNumber,
        msrDynamics::kSF,
        dynamicsPlacementKind);
        
  fPendingDynamics.push_back(dynamics);
}

void mxmlTree2MsrTranslator::visitStart( S_rfz& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_rfz" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    dynamicsPlacementKind = kPlacementNone;

  if      (placement == "above")
    dynamicsPlacementKind = kPlacementAbove;
  else if (placement == "below")
    dynamicsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        inputLineNumber,
        msrDynamics::kRFZ,
        dynamicsPlacementKind);
        
  fPendingDynamics.push_back(dynamics);
}

void mxmlTree2MsrTranslator::visitStart( S_sfz& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_sfz" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    dynamicsPlacementKind = kPlacementNone;

  if      (placement == "above")
    dynamicsPlacementKind = kPlacementAbove;
  else if (placement == "below")
    dynamicsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {    
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        inputLineNumber,
        msrDynamics::kSFZ,
        dynamicsPlacementKind);
        
  fPendingDynamics.push_back(dynamics);
}

void mxmlTree2MsrTranslator::visitStart( S_sfp& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_sfp" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    dynamicsPlacementKind = kPlacementNone;

  if      (placement == "above")
    dynamicsPlacementKind = kPlacementAbove;
  else if (placement == "below")
    dynamicsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {    
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        inputLineNumber,
        msrDynamics::kSFP,
        dynamicsPlacementKind);
        
  fPendingDynamics.push_back(dynamics);
}

void mxmlTree2MsrTranslator::visitStart( S_sfpp& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_sfpp" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    dynamicsPlacementKind = kPlacementNone;

  if      (placement == "above")
    dynamicsPlacementKind = kPlacementAbove;
  else if (placement == "below")
    dynamicsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        inputLineNumber,
        msrDynamics::kSFPP,
        dynamicsPlacementKind);
        
  fPendingDynamics.push_back(dynamics);
}

void mxmlTree2MsrTranslator::visitStart( S_sffz& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

 if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_sffz" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    dynamicsPlacementKind = kPlacementNone;

  if      (placement == "above")
    dynamicsPlacementKind = kPlacementAbove;
  else if (placement == "below")
    dynamicsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

  S_msrDynamics
    dynamics =
      msrDynamics::create (
        inputLineNumber,
        msrDynamics::kSFFZ,
        dynamicsPlacementKind);
        
  fPendingDynamics.push_back(dynamics);
}

void mxmlTree2MsrTranslator::visitStart( S_other_dynamics& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_other_dynamics" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string otherDynamicsValue = elt->getValue ();

  string placement =
    elt->getAttributeValue ("placement");

  msrPlacementKind
    otherDynamicsPlacementKind = kPlacementNone;

  if      (placement == "above")
    otherDynamicsPlacementKind = kPlacementAbove;
  else if (placement == "below")
    otherDynamicsPlacementKind = kPlacementBelow;
  else if (placement.size ()) {
    
    stringstream s;
    
    s <<
      "other dynamics placement \"" << placement <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }
  
  S_msrOtherDynamics
    otherDynamics =
      msrOtherDynamics::create (
        inputLineNumber,
        otherDynamicsValue,
        otherDynamicsPlacementKind);
        
  fPendingOtherDynamics.push_back(otherDynamics);
}

//______________________________________________________________________________
/*
  The damper-pedal, soft-pedal, and sostenuto-pedal 
  attributes effect playback of the three common piano
  pedals and their MIDI controller equivalents. The yes
  value indicates the pedal is depressed; no indicates 
  the pedal is released. A numeric value from 0 to 100
  may also be used for half pedaling. This value is the
  percentage that the pedal is depressed. A value of 0 is
  equivalent to no, and a value of 100 is equivalent to yes.

    damper-pedal %yes-no-number; #IMPLIED
    soft-pedal %yes-no-number; #IMPLIED
    sostenuto-pedal %yes-no-number; #IMPLIED
*/

/*
void mxmlTree2MsrTranslator::visitStart( S_damper_pedal& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_damper_pedal" <<
      ", line " << inputLineNumber <<
      endl;

  string damperPedalValue = elt->getValue ();
  
  // check damper pedal value
  if      (damperPedalValue == "yes")
    fCurrentDamperPedalKind
      msrDamperPedal::kDamperPedalValueYes;
    
  else if (damperPedalValue == "no")
    fCurrentDamperPedalKind =
      msrDamperPedal::kDamperPedalValueNo;
    
  else {
    istringstream inputStream (damperPedalValue);

    inputStream >> fCurrentDamperPedalIntegerValue;
    
    if (
      fCurrentDamperPedalIntegerValue < 0
        &&
      fCurrentDamperPedalIntegerValue > 100) {
      stringstream s;
  
      s <<
        "damper pedal integer value \"" <<
        fCurrentDamperPedalIntegerValue <<
        "\" should be between 0 and 100";
        
      msrMusicXMLError (
        inputLineNumber,
        s.str ());
    }

    fCurrentDamperPedalKind =
      msrDamperPedal::kDamperPedalValueZeroToAHundred;

/ * JMI
    if (damperPedalValue.size ()) {
      msrMusicXMLError (
        inputLineNumber,
        "unknown damper pedal \"" +
          damperPedalValue +
          "\", should be 'yes', 'no' or a number from 0 to 100");
    }
    * /
  }


  S_msrDamperPedal
    damperPedal =
      msrDamperPedal::create (
        inputLineNumber,
        otherDynamicsValue);
        
}

void mxmlTree2MsrTranslator::visitStart( S_soft_pedal& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_soft_pedal" <<
       ", line " << inputLineNumber <<
     endl;
  }

  string softPedalValue = elt->getValue ();
  
  S_msrOtherDynamics
    otherDynamics =
      msrOtherDynamics::create (
        inputLineNumber,
        otherDynamicsValue);
        
  fPendingOtherDynamics.push_back(otherDynamics);
}

void mxmlTree2MsrTranslator::visitStart( S_sostenuto_pedal& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_sostenuto_pedal" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string sostenutoPedalValue = elt->getValue ();
  
  S_msrOtherDynamics
    otherDynamics =
      msrOtherDynamics::create (
        inputLineNumber,
        otherDynamicsValue);
        
  fPendingOtherDynamics.push_back(otherDynamics);
}
*/

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_cue& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_cue" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
 
  fCurrentNoteIsACueNote = true;
}
       
//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_grace& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_grace" <<
      ", line " << inputLineNumber <<
      endl;
  }
 
  fCurrentNoteIsAGraceNote = true;

  // slash
  
  string slash = elt->getAttributeValue ("slash");

  fCurrentGraceIsSlashed = false; // default value

  // check part group barline
  if      (slash == "yes")
    fCurrentGraceIsSlashed = true;
    
  else if (slash == "no")
    fCurrentGraceIsSlashed = false;
    
  else {
    if (slash.size ()) {
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        "grace slash \"" + slash + "\" unknown, should be 'yes' or 'no'");
    }
  }

/*
      <note>
        <grace steal-time-previous="20"/>
        <pitch>
          <step>G</step>
          <octave>5</octave>
        </pitch>
        <voice>1</voice>
        <type>16th</type>
        <staff>1</staff>
      </note>
*/

  fCurrentStealTimeFollowing =
    elt->getAttributeValue ("steal-time-following");

  fCurrentStealTimePrevious =
    elt->getAttributeValue ("steal-time-previous");

  fCurrentMakeTime =
    elt->getAttributeValue ("make-time");
}
       
//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_chord& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_chord" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // the current note belongs to a chord,
  // placed in the corresponding staff AND voice
  fCurrentNoteBelongsToAChord = true;

  // delay the handling until 'visitEnd ( S_note& elt)',
  // because we don't know yet the voice and staff numbers for sure
  // (they can be specified after <chord/> in the <note/>)
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_time_modification& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_time_modification" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // there may be no '<tuplet number="n" type="start" />'
  // in the tuplet notes after the first one,
  // so we detect tuplet notes on '<time-modification>' ??? JMI
  fCurrentNoteHasATimeModification = true;
}

void mxmlTree2MsrTranslator::visitStart ( S_actual_notes& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_actual_notes" <<
      ", line " << inputLineNumber <<
      endl;
  }

  int actualNotes = (int)(*elt);

  if (fOnGoingNote) {
    fCurrentNoteActualNotes = actualNotes;
    
#ifdef TRACE_OPTIONS
    if (
      gTraceOptions->fTraceNotesDetails
        ||
      gTraceOptions->fTraceTuplets) {
      fLogOutputStream <<
        "fCurrentNoteActualNotes: " <<
        fCurrentNoteActualNotes <<
        endl;
    }
#endif
  
    // notes inside a tuplet have no <tuplet/> markup
    // and 2 actual notes indicate a double tremolo
    switch (fCurrentNoteActualNotes) {
      case 2:
        fCurrentNoteBelongsToADoubleTremolo = true;
        break;
      default:
        fCurrentNoteBelongsToATuplet = true;
    } // switch
  }

  else if (fOnGoingMetronomeNote) {
    fCurrentMetronomeNoteActualNotes = actualNotes;
    
#ifdef TRACE_OPTIONS
    if (
      gTraceOptions->fTraceTempos
        ||
      gTraceOptions->fTraceTuplets) {
      fLogOutputStream <<
        "fCurrentMetronomeNoteActualNotes: " <<
        fCurrentMetronomeNoteActualNotes <<
        endl;
    }
#endif
  }

  else {
    stringstream s;
    
    s <<
      "actual notes \"" << actualNotes <<
      "\" is out of context";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxmlTree2MsrTranslator::visitStart ( S_normal_notes& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_normal_notes" <<
      ", line " << inputLineNumber <<
      endl;
  }

  int normalNotes = (int)(*elt);

  if (fOnGoingNote) {
    fCurrentNoteNormalNotes = normalNotes;
    
#ifdef TRACE_OPTIONS
    if (
      gTraceOptions->fTraceNotesDetails
        ||
      gTraceOptions->fTraceTuplets) {
      fLogOutputStream <<
        "fCurrentNoteNormalNotes: " <<
        fCurrentNoteNormalNotes <<
        endl;
    }
#endif
  
    // notes inside a tuplet have no <tuplet/> markup
    // and 1 actual note indicates a double tremolo
    switch (fCurrentNoteNormalNotes) {
      case 1:
        fCurrentNoteBelongsToADoubleTremolo = true;
        break;
      default:
        fCurrentNoteBelongsToATuplet = true;
    } // switch
  }
  
  else if (fOnGoingMetronomeNote) {
    fCurrentMetronomeNoteNormalNotes = normalNotes;
    
#ifdef TRACE_OPTIONS
    if (
      gTraceOptions->fTraceTempos
        ||
      gTraceOptions->fTraceTuplets) {
      fLogOutputStream <<
        "fCurrentMetronomeNoteNormalNotes: " <<
        fCurrentMetronomeNoteNormalNotes <<
        endl;
    }
#endif
  }

  else {
    stringstream s;
    
    s <<
      "normal notes \"" << normalNotes <<
      "\" is out of context";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxmlTree2MsrTranslator::visitStart ( S_normal_type& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_normal_type" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string normalTypeString = elt->getValue();

  if (fOnGoingNote) {        
#ifdef TRACE_OPTIONS
    if (
      gTraceOptions->fTraceNotesDetails
        ||
      gTraceOptions->fTraceTuplets) {
      fLogOutputStream <<
        "normalTypeString: " <<
        normalTypeString <<
        endl;
    }
#endif

    // the type contains a display duration    
    fCurrentNoteNormalTypeDuration =
      msrDurationKindFromString (
        inputLineNumber,
        normalTypeString);
        
  /*
    // there can be several <beat-unit/> in a <metronome/> markup,
    // register beat unit in in dotted durations list
    fCurrentMetronomeBeatUnitsVector.push_back (
      msrDottedDuration (
        fCurrentNoteNormalTypeDuration,
        0));
        */
  }
  
  else if (fOnGoingMetronomeNote) { // JMI ???
    fCurrentMetronomeNoteNormalType = normalTypeString;
    
#ifdef TRACE_OPTIONS
    if (
      gTraceOptions->fTraceTempos
        ||
      gTraceOptions->fTraceTuplets) {
      fLogOutputStream <<
        "fCurrentMetronomeNoteNormalType: " <<
        fCurrentMetronomeNoteNormalType <<
        endl;
    }
#endif

  }

  else {
    stringstream s;
    
    s <<
      "normal type \"" << normalTypeString <<
      "\" is out of context";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_tuplet& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_tuplet" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // number

  fPreviousTupletNumber = fCurrentTupletNumber;
  
  fCurrentTupletNumber = elt->getAttributeIntValue ("number", 0);

  // bracket

  {
    string tupletBracket = elt->getAttributeValue ("bracket");
      
    fCurrentTupletBracketKind = msrTuplet::kTupletBracketYes; // option ??? JMI
    
    if      (tupletBracket == "yes")
      fCurrentTupletBracketKind = msrTuplet::kTupletBracketYes;
    else if (tupletBracket == "no")
      fCurrentTupletBracketKind = msrTuplet::kTupletBracketNo;
    else {
      if (tupletBracket.size ()) {
        stringstream s;
        
        s <<
          "tuplet bracket \"" << tupletBracket <<
          "\" is unknown";
        
        msrMusicXMLError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      else {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceTuplets) {
          stringstream s;
          
          s <<
            "tuplet bracket is empty: this is implementation dependent," <<
            " \"yes\" is assumed"; // option ??? JMI
          
          msrMusicXMLWarning (
            gXml2lyOptions->fInputSourceName,
            inputLineNumber,
            s.str ());
        }
#endif
      }
    }
  }

  // line-shape

  {
    string tupletLineShape = elt->getAttributeValue ("line-shape");
      
    fCurrentTupletLineShapeKind =
      msrTuplet::kTupletLineShapeStraight; // default value
    
    if      (tupletLineShape == "straight")
      fCurrentTupletLineShapeKind = msrTuplet::kTupletLineShapeStraight;
    else if (tupletLineShape == "curved")
      fCurrentTupletLineShapeKind = msrTuplet::kTupletLineShapeCurved;
    else {
      if (tupletLineShape.size ()) {
        stringstream s;
        
        s <<
          "tuplet line-shape \"" << tupletLineShape <<
          "\" is unknown";
        
        msrMusicXMLError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
    }
  }

  // type

  {
    string tupletType = elt->getAttributeValue ("type");

    msrTuplet::msrTupletTypeKind
      previousTupletTypeKind = fCurrentTupletTypeKind;
      
    fCurrentTupletTypeKind = msrTuplet::kTupletTypeNone;
    
    if      (tupletType == "start") {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceTuplets) {
        fLogOutputStream <<
          "--> There is a tuplet start (kTupletTypeStart)" <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif

      fCurrentTupletTypeKind = msrTuplet::kTupletTypeStart;
    }
    else if (tupletType == "continue") {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceTuplets) {
        fLogOutputStream <<
          "--> There is a tuplet continue (kTupletTypeContinue)" <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif

      fCurrentTupletTypeKind = msrTuplet::kTupletTypeContinue;
    }
    else if (tupletType == "stop") {
      if (
        fPreviousTupletNumber == fCurrentTupletNumber
          &&
        (
          previousTupletTypeKind == msrTuplet::kTupletTypeStart
  // JMI          ||
    //      previousTupletTypeKind == msrTuplet::kTupletTypeContinue
        )
        ) {
        // this is a tuplet stop right after a tuplet start
        // for one and the same tuplet number:
        // possible if the note is a tremolo
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceTuplets) {
          fLogOutputStream <<
            "--> There is a tuplet stop right after a tuplet start for tuplet nummber " << fCurrentTupletNumber <<
            " (kTupletTypeStartAndStopInARow)" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        fCurrentTupletTypeKind = msrTuplet::kTupletTypeStartAndStopInARow;
      }
      else {
        // this is a 'regular' tuplet stop
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceTuplets) {
          fLogOutputStream <<
            "--> There is a tuplet stop (kTupletTypeStop)" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        fCurrentTupletTypeKind = msrTuplet::kTupletTypeStop;
      }
    }
    else {
      stringstream s;
      
      s <<
        "tuplet type \"" << tupletType <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // show-number

  {
    string tupletShowNumber = elt->getAttributeValue ("show-number");
  
    fCurrentTupletShowNumberKind =
      msrTuplet::kTupletShowNumberActual; // default value
    
    if      (tupletShowNumber == "actual") {
      fCurrentTupletShowNumberKind = msrTuplet::kTupletShowNumberActual;
    }
    else if (tupletShowNumber == "both") {
      fCurrentTupletShowNumberKind = msrTuplet::kTupletShowNumberBoth;
    }
    else if (tupletShowNumber == "none") {
      fCurrentTupletShowNumberKind = msrTuplet::kTupletShowNumberNone;
    }
    else {
      if (tupletShowNumber.size ()) {
        msrMusicXMLError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          "tuplet show-number \"" + tupletShowNumber + "\" is unknown");
      }
    }
  }
  
  // show-type

  {
    string tupletShowType = elt->getAttributeValue ("show-type");
  
    fCurrentTupletShowTypeKind = msrTuplet::kTupletShowTypeNone; // default value
    
    if      (tupletShowType == "actual") {
      fCurrentTupletShowTypeKind = msrTuplet::kTupletShowTypeActual;
    }
    else if (tupletShowType == "both") {
      fCurrentTupletShowTypeKind = msrTuplet::kTupletShowTypeBoth;
    }
    else if (tupletShowType == "none") {
      fCurrentTupletShowTypeKind = msrTuplet::kTupletShowTypeNone;
    }
    else {
      if (tupletShowType.size ()) {
        msrMusicXMLError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          "tuplet show-type \"" + tupletShowType + "\" is unknown");
      }
    }
  }  

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceNotesDetails
      ||
    gTraceOptions->fTraceTuplets) {
    fLogOutputStream <<
      "fCurrentTupletNumber: " <<
      fCurrentTupletNumber <<
      ", fCurrentTupletTypeKind: " <<
      msrTuplet::tupletTypeKindAsString (
        fCurrentTupletTypeKind) <<
      ", fCurrentTupletBracketKind: " <<
      msrTuplet::tupletBracketKindAsString (
        fCurrentTupletBracketKind) <<
      ", fCurrentTupletShowNumberKind: " <<
      msrTuplet::tupletShowNumberKindAsString (
        fCurrentTupletShowNumberKind) <<
      ", fCurrentTupletShowTypeKind: " <<
      msrTuplet::tupletShowTypeKindAsString (
        fCurrentTupletShowTypeKind) <<
      endl;
  }
#endif

  fCurrentNoteBelongsToATuplet = true;
}

void mxmlTree2MsrTranslator::visitStart ( S_tuplet_actual& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_tuplet_actual" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fOnGoingTupletActual = true;
}

void mxmlTree2MsrTranslator::visitEnd ( S_tuplet_actual& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_tuplet_actual" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fOnGoingTupletActual = false;
}

void mxmlTree2MsrTranslator::visitStart ( S_tuplet_normal& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_tuplet_normal" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fOnGoingTupletNormal = true;
}

void mxmlTree2MsrTranslator::visitEnd ( S_tuplet_normal& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_tuplet_normal" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fOnGoingTupletNormal = false;
}

void mxmlTree2MsrTranslator::visitStart ( S_tuplet_number& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_tuplet_number" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // not handled JMI
  int tupletNumberValue = (int)(*elt);
  
  if (fOnGoingTupletActual) {
    fCurrentTupletActualNumber = tupletNumberValue;
  }
  else if (fOnGoingTupletNormal) {
    fCurrentTupletNormalNumber = tupletNumberValue;
  }
  else {
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      "found a tuplet number out of context");
  }

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceNotesDetails
      ||
    gTraceOptions->fTraceTuplets) {
    fLogOutputStream <<
      "tuplet number (not handled): " <<
      tupletNumberValue <<
      endl;
  }
#endif
}

void mxmlTree2MsrTranslator::visitStart ( S_tuplet_type& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_tuplet_type" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // not handled JMI
  string tupletTypeValue = elt->getValue();
    
  if (fOnGoingTupletActual) {
    fCurrentTupletActualType = tupletTypeValue;
  }
  else if (fOnGoingTupletNormal) {
    fCurrentTupletNormalType = tupletTypeValue;
  }
  else {
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      "found a tuplet number out of context");
  }

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceNotesDetails
      ||
    gTraceOptions->fTraceTuplets) {
    fLogOutputStream <<
      "tuplet type (not handled): " <<
      tupletTypeValue <<
      endl;
  }
#endif
}

void mxmlTree2MsrTranslator::visitStart ( S_tuplet_dot& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_tuplet_dot" <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (fOnGoingTupletActual) {
    fCurrentTupletActualDotsNumber++;
  }
  else if (fOnGoingTupletNormal) {
    fCurrentTupletNormalDotsNumber++;
  }
  else {
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      "found a tuplet dot out of context");
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_glissando& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_glissando" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // glissando text, i.e. the text along it
  string glissandoTextValue = elt->getValue ();

  // number

  int glissandoNumber = elt->getAttributeIntValue ("number", 0);

  // type

  string glissandoType = elt->getAttributeValue ("type");
    
  msrGlissando::msrGlissandoTypeKind
    glissandoTypeKind = msrGlissando::kGlissandoTypeNone;
  
  if      (glissandoType == "start")
    glissandoTypeKind = msrGlissando::kGlissandoTypeStart;
  else if (glissandoType == "stop")
    glissandoTypeKind = msrGlissando::kGlissandoTypeStop;
  else {
    stringstream s;
    
    s <<
      "glissando type \"" << glissandoType <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // line-type

  string glissandoLineType = elt->getAttributeValue ("line-type");

  msrLineTypeKind
    glissandoLineTypeKind =
      kLineTypeSolid; // default value
  
  if      (glissandoLineType == "solid") {
    glissandoLineTypeKind = kLineTypeSolid;
  }
  else if (glissandoLineType == "dashed") {
    glissandoLineTypeKind = kLineTypeDashed;
  }
  else if (glissandoLineType == "dotted") {
    glissandoLineTypeKind = kLineTypeDotted;
  }
  else if (glissandoLineType == "wavy") {
    glissandoLineTypeKind = kLineTypeWavy;
  }
  else {
    if (glissandoLineType.size ()) {
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        "glissando line-type \"" + glissandoLineType + "\" is unknown");
    }
  }

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceNotesDetails
      ||
    gTraceOptions->fTraceGlissandos) {
    fLogOutputStream <<
      "glissandoNumber: " <<
      glissandoNumber <<
      "glissandoType: " <<
      msrGlissando::glissandoTypeKindAsString (
        glissandoTypeKind) <<
      "glissandoLineType: " <<
      msrLineTypeKindAsString (
        glissandoLineTypeKind) <<
      "glissandoTextValue: " <<
      glissandoTextValue <<
      endl;
  }
#endif

  // create glissando
  S_msrGlissando
    glissando =
      msrGlissando::create (
        inputLineNumber,
        glissandoNumber,
        glissandoTypeKind,
        glissandoLineTypeKind,
        glissandoTextValue);

  // register glissando in this visitor
#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceNotesDetails
      ||
    gTraceOptions->fTraceGlissandos) {
    fLogOutputStream <<
      "Appending glissando '" <<
      glissando->asString () <<
      "' to the glissandos pending list" <<
      endl;
  }
#endif
      
  fPendingGlissandos.push_back (glissando);
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_slide& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_slide" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // slide text, i.e. the text along it
  string slideTextValue = elt->getValue ();

  // number

  int slideNumber = elt->getAttributeIntValue ("number", 0);

  // type

  string slideType = elt->getAttributeValue ("type");
    
  msrSlide::msrSlideTypeKind
    slideTypeKind = msrSlide::kSlideTypeNone;
  
  if      (slideType == "start")
    slideTypeKind = msrSlide::kSlideTypeStart;
  else if (slideType == "stop")
    slideTypeKind = msrSlide::kSlideTypeStop;
  else {
    stringstream s;
    
    s <<
      "slide type \"" << slideType <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // line-type

  string slideLineType = elt->getAttributeValue ("line-type");

  msrLineTypeKind
    slideLineTypeKind =
      kLineTypeSolid; // default value
  
  if      (slideLineType == "solid") {
    slideLineTypeKind = kLineTypeSolid;
  }
  else if (slideLineType == "dashed") {
    slideLineTypeKind = kLineTypeDashed;
  }
  else if (slideLineType == "dotted") {
    slideLineTypeKind = kLineTypeDotted;
  }
  else if (slideLineType == "wavy") {
    slideLineTypeKind = kLineTypeWavy;
  }
  else {
    if (slideLineType.size ()) {
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        "slide line-type \"" + slideLineType + "\" is unknown");
    }
  }

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceNotesDetails
      ||
    gTraceOptions->fTraceSlides) {
    fLogOutputStream <<
      "slideNumber: " <<
      slideNumber <<
      "slideType: " <<
      msrSlide::slideTypeKindAsString (
        slideTypeKind) <<
      "slideLineType: " <<
      msrLineTypeKindAsString (
        slideLineTypeKind) <<
      endl;
  }
#endif

  // create slide
  S_msrSlide
    slide =
      msrSlide::create (
        inputLineNumber,
        slideNumber,
        slideTypeKind,
        slideLineTypeKind,
        slideTextValue);
    
  // register glissando in this visitor
#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceNotesDetails
      ||
    gTraceOptions->fTraceSlides) {
    fLogOutputStream <<
      "Appending slide '" <<
      slide->asString () <<
      "' to the slides pending list" <<
      endl;
  }
#endif
      
  fPendingSlides.push_back (slide);    
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_rest& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_rest" <<
      ", line " << inputLineNumber <<
      endl;
  }

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
  //  fLogOutputStream << "--> mxmlTree2MsrTranslator::visitStart ( S_rest& elt ) " <<endl;
  fCurrentNoteQuarterTonesPitchKind = k_Rest_QTP;
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
    if (restMeasure.size ()) {
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        "rest measure \"" + restMeasure + "\" is unknown");
    }
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_display_step& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_display_step" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string displayStep = elt->getValue();
  
  checkStep (
    inputLineNumber,
    displayStep);

  fCurrentDisplayDiatonicPitchKind =
    msrDiatonicPitchKindFromString (
      displayStep [0]);

  // pitched rests don't allow for alterations since
  // the display-step merely indicates where to place them on the staff
  fCurrentNoteAlterationKind = kNatural;
}

void mxmlTree2MsrTranslator::visitStart ( S_display_octave& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_display_octave" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentDisplayOctave = (int)(*elt);
}

void mxmlTree2MsrTranslator::visitEnd ( S_unpitched& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_unpitched" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentNoteIsUnpitched = true;
  
 // fCurrentNoteDiatonicPitch = // JMI
   // fCurrentHarmonyRootDiatonicPitch;
}

//______________________________________________________________________________
S_msrChord mxmlTree2MsrTranslator::createChordFromItsFirstNote (
  int                  inputLineNumber,
  S_msrVoice           voice,
  S_msrNote            chordFirstNote,
  msrNote::msrNoteKind noteKind)
{
  int firstNoteInputLineNumber =
    chordFirstNote->getInputLineNumber ();
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords || gTraceOptions->fTraceNotes) {
    fLogOutputStream <<
      "--> creating a chord from its first note '" <<
      chordFirstNote->asShortString () <<
      ", in voice \"" << voice->getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // firstNote has been registered standalone in the part element sequence,
  // but it is actually the first note of a chord
  
  // create a chord
  S_msrChord
    chord =
      msrChord::create (
        firstNoteInputLineNumber,
        chordFirstNote->getNoteSoundingWholeNotes (),
        chordFirstNote->getNoteDisplayWholeNotes (),
        chordFirstNote->getNoteGraphicDurationKind ());
  
  // chord's tie kind is that of its first note
  chord->
    setChordTie (
      chordFirstNote->
        getNoteTie ());
  
  // register note as first member of chord
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords || gTraceOptions->fTraceNotes) {
    fLogOutputStream <<
      "Adding first note " <<
      chordFirstNote->
        asShortString () <<
      ", line " << inputLineNumber <<
      ", to new chord" <<
      endl;
  }
#endif
      
  chord->
    addFirstNoteToChord (
      chordFirstNote, voice);

  // set chord first note's kind
  chordFirstNote->
    setNoteKind (noteKind);

  // copy firstNote's elements if any to the chord
  copyNoteElementsToChord (
    chordFirstNote, chord);

#ifdef TRACE_OPTIONS
  if (false && (gTraceOptions->fTraceChords || gTraceOptions->fTraceNotes)) {
    fLogOutputStream << // JMI
      endl <<
      endl <<
      "++++++++++++++++ chordFirstNote =" <<
      endl <<
      endl <<
      chordFirstNote <<
      endl <<
      "+++++++++++++++++" <<
      endl <<
      endl <<
      "++++++++++++++++ chordFirstNote->getNoteMeasureUplink () =" <<
      chordFirstNote->
        getNoteMeasureUplink () <<
      endl <<
      endl;
  }
#endif

  // grace notes cannot be cross staff
  if (! chordFirstNote->getNoteIsAGraceNote ()) { // JMI
    // register the chord as non cross staves
    fCurrentChordStaffNumber =
      chordFirstNote->
        getNoteMeasureUplink ()->
          getMeasureSegmentUplink ()->
            getSegmentVoiceUplink ()->
              getVoiceStaffUplink ()->
                getStaffNumber ();
  }
    
  return chord;
}

//______________________________________________________________________________
/* JMI
void mxmlTree2MsrTranslator::registerVoiceCurrentChordInMap (
  int        inputLineNumber,
  S_msrVoice voice,
  S_msrChord chord)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    fLogOutputStream <<
      "Registering chord '" <<
      chord->asString () <<
      "'" <<
      " as current chord in voice \"" <<
      voice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

 // fVoicesCurrentChordMap [voice] = chord;
  fVoicesCurrentChordMap [
    make_pair (
      voice->getVoiceStaffUplink ()->getStaffNumber (),
      voice->getVoiceNumber ())] =
      chord;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChordsDetails) {
    printCurrentChord ();
  }
#endif
}
*/

//______________________________________________________________________________
/* JMI
void mxmlTree2MsrTranslator::printVoicesCurrentChordMap ()
{
  fLogOutputStream <<
    "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
    endl <<
    "fVoicesCurrentChordMap contains " <<
    singularOrPlural (
      fVoicesCurrentChordMap.size (), "element", "elements") <<
    ":";
     
  if (fVoicesCurrentChordMap.size ()) {
    fLogOutputStream <<
      endl;

    gIndenter++;

//    map<S_msrVoice, S_msrChord>::const_iterator
    map<pair<int, int>, S_msrChord>::const_iterator
      iBegin = fVoicesCurrentChordMap.begin (),
      iEnd   = fVoicesCurrentChordMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      gIndenter++;

      / * JMI
      S_msrVoice voice = (*i).first;
      S_msrChord chord = (*i).second;
    * /
      fLogOutputStream <<
//        voice->getVoiceName () <<
        "staff " << (*i).first.first <<
        ", voice " <<  (*i).first.second <<
        ":" <<
        endl;

// * JMI
      S_msrChord chord = (*i).second;

      fLogOutputStream <<
        chord;
// * /
        
      gIndenter--;

      if (++i == iEnd) break;
      
      fLogOutputStream << endl;
    } // for

    gIndenter--;
  }

  fLogOutputStream <<
    "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
    endl;
}
*/

//______________________________________________________________________________
void mxmlTree2MsrTranslator::printCurrentChord ()
{
  fLogOutputStream <<
    "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
    endl;

  if (fCurrentChord) {
    fLogOutputStream <<
      "fCurrentChord contains:" <<
      endl;
       
    gIndenter++;

    fLogOutputStream <<
      fCurrentChord;

    gIndenter--;
  }
  
  else {
    fLogOutputStream <<
     "fCurrentChord is null" <<
     endl;
  }

  fLogOutputStream <<
    "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
    endl;
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::copyNoteArticulationsToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's articulations if any from the first note to chord
  
  list<S_msrArticulation>
    noteArticulations =
      note->
        getNoteArticulations ();
                          
  list<S_msrArticulation>::const_iterator i;
  for (
    i=noteArticulations.begin ();
    i!=noteArticulations.end ();
    i++) {

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceChords || gTraceOptions->fTraceNotes) {
      fLogOutputStream <<
        "Copying articulation '" <<
        (*i)->articulationKindAsString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        endl;
    }
#endif

    chord->appendArticulationToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::copyNoteTechnicalsToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's technicals if any from the first note to chord
  
  list<S_msrTechnical>
    noteTechnicals =
      note->
        getNoteTechnicals ();
                          
  list<S_msrTechnical>::const_iterator i;
  for (
    i=noteTechnicals.begin ();
    i!=noteTechnicals.end ();
    i++) {

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceNotesDetails
      ||
    gTraceOptions->fTraceTechnicals
    ) {
    fLogOutputStream <<
      "Copying technical '" <<
      (*i)->technicalKindAsString () <<
      "' from note " << note->asString () <<
      " to chord" <<
      endl;
    }
#endif
    
    chord->appendTechnicalToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::copyNoteTechnicalWithIntegersToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's technicals if any from the first note to chord
  
  list<S_msrTechnicalWithInteger>
    noteTechnicalWithIntegers =
      note->
        getNoteTechnicalWithIntegers ();
                          
  list<S_msrTechnicalWithInteger>::const_iterator i;
  for (
    i=noteTechnicalWithIntegers.begin ();
    i!=noteTechnicalWithIntegers.end ();
    i++) {

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceNotesDetails
      ||
    gTraceOptions->fTraceTechnicals
    ) {
    fLogOutputStream <<
      "Copying technical '" <<
      (*i)->technicalWithIntegerKindAsString () <<
      "' from note " << note->asString () <<
      " to chord" <<
      endl;
    }
#endif

    chord->appendTechnicalWithIntegerToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::copyNoteTechnicalWithFloatsToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's technicals if any from the first note to chord
  
  list<S_msrTechnicalWithFloat>
    noteTechnicalWithFloats =
      note->
        getNoteTechnicalWithFloats ();
                          
  list<S_msrTechnicalWithFloat>::const_iterator i;
  for (
    i=noteTechnicalWithFloats.begin ();
    i!=noteTechnicalWithFloats.end ();
    i++) {

#ifdef TRACE_OPTIONS
    if (
      gTraceOptions->fTraceNotesDetails
        ||
      gTraceOptions->fTraceTechnicals
      ) {
      fLogOutputStream <<
        "Copying technical '" <<
        (*i)->technicalWithFloatKindAsString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        endl;
    }
#endif

    chord->appendTechnicalWithFloatToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::copyNoteTechnicalWithStringsToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's technicals if any from the first note to chord
  
  list<S_msrTechnicalWithString>
    noteTechnicalWithStrings =
      note->
        getNoteTechnicalWithStrings ();
                          
  list<S_msrTechnicalWithString>::const_iterator i;
  for (
    i=noteTechnicalWithStrings.begin ();
    i!=noteTechnicalWithStrings.end ();
    i++) {

#ifdef TRACE_OPTIONS
    if (
      gTraceOptions->fTraceTechnicals
        ||
      gTraceOptions->fTraceChords
      ) {
      fLogOutputStream <<
        "Copying technical '" <<
        (*i)->technicalWithStringKindAsString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        endl;
    }
#endif

    chord->appendTechnicalWithStringToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::copyNoteOrnamentsToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's ornaments if any from the first note to chord
  
  list<S_msrOrnament>
    noteOrnaments =
      note->
        getNoteOrnaments ();
                          
  list<S_msrOrnament>::const_iterator i;
  for (
    i=noteOrnaments.begin ();
    i!=noteOrnaments.end ();
    i++) {

#ifdef TRACE_OPTIONS
    if (
      gTraceOptions->fTraceOrnaments
        ||
      gTraceOptions->fTraceChords) {
      fLogOutputStream <<
        "Copying ornament '" <<
        (*i)->ornamentKindAsString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        endl;
    }
#endif

    chord->
      appendOrnamentToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::copyNoteSpannersToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's spanners if any from the first note to chord
  
  list<S_msrSpanner>
    noteSpanners =
      note->
        getNoteSpanners ();
                          
  list<S_msrSpanner>::const_iterator i;
  for (
    i=noteSpanners.begin ();
    i!=noteSpanners.end ();
    i++) {

#ifdef TRACE_OPTIONS
    if (
      gTraceOptions->fTraceSpanners
        ||
      gTraceOptions->fTraceChords) {
      fLogOutputStream <<
        "Copying spanner '" <<
        (*i)->spannerKindAsString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        endl;
    }
#endif

    chord->
      appendSpannerToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::copyNoteSingleTremoloToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's singleTremolo if any from the first note to chord
  
  S_msrSingleTremolo
    noteSingleTremolo =
      note->
        getNoteSingleTremolo ();

  if (noteSingleTremolo) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceTremolos || gTraceOptions->fTraceChords) {
      fLogOutputStream <<
        "Copying singleTremolo '" <<
        noteSingleTremolo->asString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        endl;
    }
#endif
    
    chord->
      setChordSingleTremolo (noteSingleTremolo);
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::copyNoteDynamicsToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's dynamics if any from the first note to chord
  
  list<S_msrDynamics>
    noteDynamics =
      note->
        getNoteDynamics ();
                          
  list<S_msrDynamics>::const_iterator i;
  for (
    i=noteDynamics.begin ();
    i!=noteDynamics.end ();
    i++) {

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceChords || gTraceOptions->fTraceDynamics) {
      fLogOutputStream <<
        "Copying dynamics '" <<
        (*i)->dynamicsKindAsString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        endl;
    }
#endif

    chord->
      appendDynamicsToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::copyNoteOtherDynamicsToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's other dynamics if any from the first note to chord
  
  list<S_msrOtherDynamics>
    noteOtherDynamics =
      note->
        getNoteOtherDynamics ();
                          
  list<S_msrOtherDynamics>::const_iterator i;
  for (
    i=noteOtherDynamics.begin ();
    i!=noteOtherDynamics.end ();
    i++) {

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceChords || gTraceOptions->fTraceDynamics) {
      fLogOutputStream <<
        "Copying other dynamics '" <<
        (*i)->asString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        endl;
    }
#endif

    chord->
      appendOtherDynamicsToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::copyNoteWordsToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's words if any from the first note to chord
  
  list<S_msrWords>
    noteWords =
      note->
        getNoteWords ();
                          
  list<S_msrWords>::const_iterator i;
  for (
    i=noteWords.begin ();
    i!=noteWords.end ();
    i++) {

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceChords || gTraceOptions->fTraceWords) {
      fLogOutputStream <<
        "Copying words '" <<
        (*i)->asString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        endl;
    }
#endif

    chord->
      appendWordsToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::copyNoteStemToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's stems if any from the first note to chord
  
  S_msrStem
    noteStem =
      note->
        getNoteStem ();

  if (noteStem) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceStems || gTraceOptions->fTraceChords) {
      fLogOutputStream <<
        "Copying stem '" <<
        noteStem->asString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        endl;
    }
#endif

    chord->
      appendStemToChord (noteStem);
  }    
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::copyNoteBeamsToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's beams if any from the first note to chord
  
  list<S_msrBeam>
    noteBeams =
      note->
        getNoteBeams ();
                          
  list<S_msrBeam>::const_iterator i;
  for (
    i=noteBeams.begin ();
    i!=noteBeams.end ();
    i++) {

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceBeams || gTraceOptions->fTraceChords) {
      fLogOutputStream <<
        "Copying beam '" <<
        (*i)->asString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        endl;
    }
#endif

    chord->
      appendBeamToChord ((*i));
  } // for      

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceBeams || gTraceOptions->fTraceChords) {
    fLogOutputStream <<
      "==> AFTER copying beams to chord:" <<
      endl;

    gIndenter++;
    
    fLogOutputStream <<
      chord <<
      endl;

    gIndenter--;
  }
#endif
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::copyNoteTieToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's tie if any from the first note to chord
  
  S_msrTie
    noteTie =
      note->
        getNoteTie ();

  if (noteTie) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceChords || gTraceOptions->fTraceTies) {
      fLogOutputStream <<
        "Copying tie '" <<
        noteTie->asString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        endl;
    }
#endif

    chord->
      appendTieToChord (noteTie);
  }   
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::copyNoteSlursToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's slurs if any from the first note to chord
  
  list<S_msrSlur>
    noteSlurs =
      note->
        getNoteSlurs ();
                          
  list<S_msrSlur>::const_iterator i;
  for (
    i=noteSlurs.begin ();
    i!=noteSlurs.end ();
    i++) {

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceChords || gTraceOptions->fTraceSlurs) {
      fLogOutputStream <<
        "Copying slur '" <<
        (*i)->asString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        endl;
    }
#endif

    chord->
      appendSlurToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::copyNoteLigaturesToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's ligatures if any from the first note to chord
  
  list<S_msrLigature>
    noteLigatures =
      note->
        getNoteLigatures ();
                          
  list<S_msrLigature>::const_iterator i;
  for (
    i=noteLigatures.begin ();
    i!=noteLigatures.end ();
    i++) {

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceChords || gTraceOptions->fTraceLigatures) {
      fLogOutputStream <<
        "Copying ligature '" <<
        (*i)->ligatureKindAsString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        endl;
    }
#endif

    chord->
      appendLigatureToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::copyNotePedalsToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's pedals if any from the first note to chord
  
  list<S_msrPedal>
    notePedals =
      note->
        getNotePedals ();
                          
  list<S_msrPedal>::const_iterator i;
  for (
    i=notePedals.begin ();
    i!=notePedals.end ();
    i++) {

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceChords || gTraceOptions->fTracePedals) {
      fLogOutputStream <<
        "Copying pedal '" <<
        (*i)->pedalTypeAsString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        endl;
    }
#endif

    chord->
      appendPedalToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::copyNoteSlashesToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's slashes if any from the first note to chord
  
  list<S_msrSlash>
    noteSlashes =
      note->
        getNoteSlashes ();
                          
  list<S_msrSlash>::const_iterator i;
  for (
    i=noteSlashes.begin ();
    i!=noteSlashes.end ();
    i++) {

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceChords || gTraceOptions->fTraceSlashes) {
      fLogOutputStream <<
        "Copying slash '" <<
        (*i)->asString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        endl;
    }
#endif

    chord->
      appendSlashToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::copyNoteWedgesToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's wedges if any from the first note to chord
  
  list<S_msrWedge>
    noteWedges =
      note->
        getNoteWedges ();
                          
  list<S_msrWedge>::const_iterator i;
  for (
    i=noteWedges.begin ();
    i!=noteWedges.end ();
    i++) {

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceChords || gTraceOptions->fTraceWedges) {
      fLogOutputStream <<
        "Copying wedges '" <<
        (*i)->wedgeKindAsString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        endl;
    }
#endif

    chord->
      appendWedgeToChord ((*i));
  } // for      
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::copyNoteGraceNotesGroupsToChord (
  S_msrNote note, S_msrChord chord)
{
  S_msrGraceNotesGroup
    graceNotesGroupBefore =
      note->
        getNoteGraceNotesGroupBefore ();
                          
  if (graceNotesGroupBefore) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceGraceNotes || gTraceOptions->fTraceChords) {
      fLogOutputStream <<
        "Copying grace notes group before '" <<
        graceNotesGroupBefore->asShortString () <<
        "' from note " << note->asString () <<
        " to chord '" << chord->asString () <<
        "'" <<
        endl;
    }
#endif

    chord->
      setChordGraceNotesGroupBefore (graceNotesGroupBefore);
  }
   
  S_msrGraceNotesGroup
    graceNotesGroupAfter =
      note->
        getNoteGraceNotesGroupAfter ();
                          
  if (graceNotesGroupAfter) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceGraceNotes || gTraceOptions->fTraceChords) {
      fLogOutputStream <<
        "Copying grace notes group after '" <<
        graceNotesGroupAfter->asShortString () <<
        "' from note " << note->asString () <<
        " to chord '" << chord->asString () <<
        "'" <<
        endl;
    }
#endif

    chord->
      setChordGraceNotesGroupAfter (graceNotesGroupAfter);
  }   
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::copyNoteHarmonyToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's harmony if any from the first note to chord
  
  S_msrHarmony
    harmony =
      note->
        getNoteHarmony ();
                          
  if (harmony) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceChords) {
      fLogOutputStream <<
        "Copying harmony '" <<
        harmony->asString () <<
        "' from note " << note->asString () <<
        " to chord '" << chord->asString () <<
        "'" <<
        endl;
    }
#endif

    chord->
      setChordHarmony (harmony);
  }   
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::copyNoteElementsToChord (
  S_msrNote note, S_msrChord chord)
{  
  // copy note's articulations if any to the chord
  copyNoteArticulationsToChord (note, chord);

  // copy note's technicals if any to the chord
  copyNoteTechnicalsToChord (note, chord);
  copyNoteTechnicalWithIntegersToChord (note, chord);
  copyNoteTechnicalWithFloatsToChord (note, chord);
  copyNoteTechnicalWithStringsToChord (note, chord);

  // copy note's ornaments if any to the chord
  copyNoteOrnamentsToChord (note, chord);

  // copy note's spanners if any to the chord
  copyNoteSpannersToChord (note, chord);

  // copy note's single tremolo if any to the chord
  copyNoteSingleTremoloToChord (note, chord);

  // copy note's dynamics if any to the chord
  copyNoteDynamicsToChord (note, chord);

  // copy note's other dynamics if any to the chord
  copyNoteOtherDynamicsToChord (note, chord);

  // copy note's words if any to the chord
  copyNoteWordsToChord (note, chord);

  // copy note's stem if any to the chord
  copyNoteStemToChord (note, chord);

  // copy note's beams if any to the chord
  copyNoteBeamsToChord (note, chord);

  // copy note's ties if any to the chord
  copyNoteTieToChord (note, chord);

  // copy note's tie if any to the chord
  copyNoteSlursToChord (note, chord);

  // copy note's ligatures if any to the chord
  copyNoteLigaturesToChord (note, chord);

  // copy note's pedals if any to the chord
  copyNotePedalsToChord (note, chord);

  // copy note's slashes if any to the chord
  copyNoteSlashesToChord (note, chord);

  // copy note's wedges if any to the chord
  copyNoteWedgesToChord (note, chord);

  // copy note's grace notes groups if any to the chord
  copyNoteGraceNotesGroupsToChord (note, chord);

  // copy note's harmony if any to the chord
  copyNoteHarmonyToChord (note, chord);
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::createTupletWithItsFirstNoteAndPushItToTupletsStack (
  S_msrNote firstNote)
{
  // firstNote is the first tuplet note,
  // and is currently at the end of the voice

  // create a tuplet
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets) {
    fLogOutputStream <<
      "Creating a '" <<
      fCurrentNoteActualNotes <<
      "/" <<
      fCurrentNoteNormalNotes <<
      "' tuplet with first note " <<
      firstNote->
        asShortString () <<
      endl;
  }
#endif
      
  // account for note duration
  rational
    memberNotesSoundingWholeNotes =
      firstNote->getNoteSoundingWholeNotes ();
  memberNotesSoundingWholeNotes.rationalise ();
  
  rational
    memberNotesDisplayWholeNotes =
      firstNote->getNoteDisplayWholeNotes ();  
  memberNotesDisplayWholeNotes.rationalise ();

  S_msrTuplet
    tuplet =
      msrTuplet::create (
        firstNote->getInputLineNumber (),
        fCurrentMeasureNumber,
        fCurrentTupletNumber,
        fCurrentTupletBracketKind,
        fCurrentTupletLineShapeKind,
        fCurrentTupletShowNumberKind,
        fCurrentTupletShowTypeKind,
        msrTupletFactor (
          fCurrentNoteActualNotes,
          fCurrentNoteNormalNotes),
        memberNotesSoundingWholeNotes,
        memberNotesDisplayWholeNotes,
        firstNote->getNotePositionInMeasure ());

  // add note as first note of the stack top tuplet
  tuplet->
    addNoteToTuplet (
      firstNote,
      nullptr /* JMI */);

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets) {
    // only after addNoteToTuplet() has set the note's tuplet uplink
    fLogOutputStream <<
      "Adding first note " <<
      firstNote->
        asShortString () <<
      " to tuplet '" <<
      tuplet->asString () <<
       "'" <<
      endl;
  }
#endif
      
  // register tuplet in this visitor
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets || gTraceOptions->fTraceNotes) {
    fLogOutputStream <<
      "++> pushing tuplet '" <<
      tuplet->asString () <<
      "' to tuplets stack" <<
      endl;
  }
#endif
      
  fTupletsStack.push_front (tuplet);

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTupletsDetails) {
    displayTupletsStack (
      "############## After  createTupletWithItsFirstNoteAndPushItToTupletsStack()");
  }
#endif

/* JMI
  // set note displayed divisions
  firstNote->
    applyTupletMemberDisplayFactor (
      fCurrentNoteActualNotes,
      fCurrentNoteNormalNotes);
  */


  // keep track of current tuplet in the current voice,
  // in case we learn later by <chord/> in the next note
  // that it is actually the first note of a chord ?? JMI XXL

  // register tuplet as last one found in this voice
  // for chords in tuplets handling
// JMI  fLastHandledTupletInVoiceMap [currentVoice] = tuplet;
  fLastHandledTupletInVoiceMap [
    make_pair (
      fCurrentMusicXMLStaffNumber, // JMI fCurrentStaffNumberToInsertInto,
      fCurrentMusicXMLVoiceNumber)
    ] =
    tuplet;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTupletsDetails) {
    displayLastHandledTupletInVoiceMap (
      "############## Before fLastHandledTupletInVoiceMap");
  }
#endif
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::finalizeTupletAndPopItFromTupletsStack (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets) {
    fLogOutputStream <<
      "mxmlTree2MsrTranslator::finalizeTupletAndPopItFromTupletsStack(), " <<
      "line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets) {
    displayTupletsStack (
      "############## Before  finalizeTupletAndPopItFromTupletsStack()");
  }
#endif

  // fetch current voice
  S_msrVoice
    currentVoice =
      fetchVoiceFromPart (
        inputLineNumber,
        fCurrentStaffNumberToInsertInto, // fCurrentMusicXMLStaffNumber,
        fCurrentMusicXMLVoiceNumber);

  // get tuplet from top of tuplet stack
  S_msrTuplet
    tuplet =
      fTupletsStack.front ();

/*  // set note displayed divisions JMI
  note->
    applyTupletMemberDisplayFactor (
      fCurrentNoteActualNotes,
      fCurrentNoteNormalNotes);
*/

/* JMI
  // add lastNote to the tuplet
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets) {
    fLogOutputStream <<
      "==> adding last note " << lastNote->asString () <<
      " to tuplets stack top " <<
      fTupletsStack.front ()->asString () <<
      endl;
  }
#endif

  tuplet->addNoteToTuplet (lastNote);
*/

  // pop from the tuplets stack
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets) {
    fLogOutputStream <<
      "Popping tuplet '" <<
      tuplet->asString () <<
      "' from tuplets stack" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
      
  fTupletsStack.pop_front ();        

  if (fTupletsStack.size ()) {
    // tuplet is a nested tuplet
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceTuplets) {
      fLogOutputStream <<
        "=== adding nested tuplet '" <<
      tuplet->asString () <<
        "' to current stack top tuplet '" <<
      fTupletsStack.front ()->asString () <<
      "'" <<
      ", line " << inputLineNumber <<
      endl;
    }
#endif
    
    fTupletsStack.front ()->
      addTupletToTuplet (tuplet);
  }
  
  else {
    // tuplet is a top level tuplet
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceTuplets) {
      fLogOutputStream <<
        "=== adding top level tuplet '" <<
      tuplet->asString () <<
      "' to voice \"" <<
      currentVoice->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
    }
#endif
      
    currentVoice->
      appendTupletToVoice (tuplet);

 // JMI BAD HERE   // the tuplet stop is not to be handled later
 //   fCurrentATupletStopIsPending = false;
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets) {
    displayTupletsStack (
      "############## After  finalizeTupletAndPopItFromTupletsStack()");
  }
#endif
}          

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachCurrentArticulationsToNote (
  S_msrNote note)
{
  // attach the current articulations if any to the note
  if (fCurrentArticulations.size ()) {

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceArticulations) {
      fLogOutputStream <<
        "Attaching current articulations to note " <<
        note->asString () <<
        endl;
    }
#endif

    while (fCurrentArticulations.size ()) {
      S_msrArticulation
        art =
          fCurrentArticulations.front();
          
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceNotes) {
        fLogOutputStream <<
          "Attaching articulation '" <<
          art->articulationKindAsString () <<
          "' to note " << note->asString () <<
          endl;
      }
#endif
  
      note->
        appendArticulationToNote (art);

      // forget about this articulation
      fCurrentArticulations.pop_front();
    } // while
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachCurrentTechnicalsToNote (
  S_msrNote note)
{
  // attach the current technicals if any to the note
  if (fCurrentTechnicalsList.size ()) {
    
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceTechnicals) {
      fLogOutputStream <<
        "Attaching current technicals to note " <<
        note->asString () <<
        endl;
    }
#endif

    while (fCurrentTechnicalsList.size ()) {
      S_msrTechnical
        tech =
          fCurrentTechnicalsList.front();
          
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceTechnicals) {
        fLogOutputStream <<
          "Attaching technical '" <<
          tech->asString () <<
          "' to note " << note->asString () <<
          endl;
      }
#endif
  
      note->
        appendTechnicalToNote (tech);

      // forget about this technical
      fCurrentTechnicalsList.pop_front();
    } // while
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachCurrentTechnicalWithIntegersToNote (
  S_msrNote note)
{
  // attach the current technicals if any to the note
  if (fCurrentTechnicalWithIntegersList.size ()) {
    
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceTechnicals) {
      fLogOutputStream <<
        "Attaching current technical with integers to note " <<
        note->asString () <<
        endl;
    }
#endif

    while (fCurrentTechnicalWithIntegersList.size ()) {
      S_msrTechnicalWithInteger
        tech =
          fCurrentTechnicalWithIntegersList.front();
          
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceTechnicals) {
        fLogOutputStream <<
          "Attaching technical with integer '" <<
          tech->asString () <<
          "' to note " << note->asString () <<
          endl;
      }
#endif
  
      note->
        appendTechnicalWithIntegerToNote (tech);

      // forget about this technical
      fCurrentTechnicalWithIntegersList.pop_front();
    } // while
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachCurrentTechnicalWithFloatsToNote (
  S_msrNote note)
{
  // attach the current technicals if any to the note
  if (fCurrentTechnicalWithFloatsList.size ()) {
    
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceTechnicals) {
      fLogOutputStream <<
        "Attaching current technical with floats to note " <<
        note->asString () <<
        endl;
    }
#endif

    while (fCurrentTechnicalWithFloatsList.size ()) {
      S_msrTechnicalWithFloat
        tech =
          fCurrentTechnicalWithFloatsList.front();
          
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceTechnicals) {
        fLogOutputStream <<
          "Attaching technical with integer '" <<
          tech->asString () <<
          "' to note " << note->asString () <<
          endl;
      }
#endif
  
      note->
        appendTechnicalWithFloatToNote (tech);

      // forget about this technical
      fCurrentTechnicalWithFloatsList.pop_front();
    } // while
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachCurrentTechnicalWithStringsToNote (
  S_msrNote note)
{
  // attach the current technicals if any to the note
  if (fCurrentTechnicalWithStringsList.size ()) {
    
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceTechnicals) {
      fLogOutputStream <<
        "Attaching current technical with strings to note " <<
        note->asString () <<
        endl;
    }
#endif

    while (fCurrentTechnicalWithStringsList.size ()) {
      S_msrTechnicalWithString
        tech =
          fCurrentTechnicalWithStringsList.front();
          
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceTechnicals) {
        fLogOutputStream <<
          "Attaching technical with string '" <<
          tech->asString () <<
          "' to note " << note->asString () <<
          endl;
      }
#endif
  
      note->
        appendTechnicalWithStringToNote (tech);

      // forget about this technical
      fCurrentTechnicalWithStringsList.pop_front();
    } // while
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachCurrentOrnamentsToNote (
  S_msrNote note)
{
  // attach the current ornaments if any to the note
  if (fCurrentOrnamentsList.size ()) {
    
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOrnaments) {
      fLogOutputStream <<
        "Attaching current ornaments to note " <<
        note->asString () <<
        endl;
    }
#endif

    while (fCurrentOrnamentsList.size ()) {
      S_msrOrnament
        orn =
          fCurrentOrnamentsList.front();
          
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceNotes) {
        fLogOutputStream <<
          "Attaching ornament '" <<
          orn->ornamentKindAsString () <<
          "' to note " << note->asString () <<
          endl;
      }
#endif
  
      note->
        appendOrnamentToNote (orn);

      // forget about this ornament
      fCurrentOrnamentsList.pop_front();
    } // while
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachCurrentSpannersToNote (
  S_msrNote note)
{
  // attach the current spanners if any to the note
  if (fCurrentSpannersList.size ()) {
    
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceSpanners) {
      fLogOutputStream <<
        "Attaching current spanners to note " <<
        note->asString () <<
        endl;
    }
#endif

    bool doHandleSpanner = true;
    bool spannerStopMetForThisNote = false;

    S_msrSpanner delayedStopSpanner;
    
    while (fCurrentSpannersList.size ()) {
      S_msrSpanner
        spanner =
          fCurrentSpannersList.front();
          
      switch (spanner->getSpannerKind ()) {
        case msrSpanner::kSpannerDashes: // JMI
          switch (spanner->getSpannerTypeKind ()) {
            case kSpannerTypeStart:
              spannerStopMetForThisNote = true;
              break;
            case kSpannerTypeStop:
              doHandleSpanner =
                ! spannerStopMetForThisNote;
              break;
            case kSpannerTypeContinue:
              break;
            case k_NoSpannerType:
              // JMI ???
              break;
          } // switch
          break;
               
        case msrSpanner::kSpannerWavyLine:
          switch (spanner->getSpannerTypeKind ()) {
            case kSpannerTypeStart:
              spannerStopMetForThisNote = true;
              break;
            case kSpannerTypeStop:
              doHandleSpanner =
                ! spannerStopMetForThisNote;
              break;
            case kSpannerTypeContinue:
              break;
            case k_NoSpannerType:
              // JMI ???
              break;
          } // switch
          break;      
      } // switch

      if (doHandleSpanner) {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceSpanners) {
          fLogOutputStream <<
            "Attaching spanner '" <<
            spanner->spannerKindAsString () <<
            "' to note " << note->asString () <<
            endl;
        }
#endif
    
        note->
          appendSpannerToNote (spanner);

        // set spanner note uplink
        spanner->
          setSpannerNoteUplink (note);
  
        // forget about this spanner
        fCurrentSpannersList.pop_front ();
      }

      else { // check it is the same spanner kind JMI
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceSpanners) {
          fLogOutputStream <<
            "Spanner start amd stop on one and the same note' to note " <<
            note->asString () <<
            ", delaying 'stop' until next note" <<
            endl;
        }
#endif

        // keep track of this stop spanner
        delayedStopSpanner = spanner;
        
        // forget about this spanner to avoid infinite loop
        fCurrentSpannersList.pop_front ();
      }
    } // while

    // append delayed stop spanner if any again to the list
    if (delayedStopSpanner) {
      fCurrentSpannersList.push_back (
        delayedStopSpanner);
    }
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachCurrentSingleTremoloToNote (
  S_msrNote note)
{
  // attach the current singleTremolo if any to the note
  if (fCurrentSingleTremolo) {
    
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceNotes) {
      fLogOutputStream <<
        "Attaching current singleTremolo to note " <<
        note->asString () <<
        endl;
    }
#endif

    note->
      setNoteSingleTremolo (fCurrentSingleTremolo);

    // set single tremolo graphic duration kind
    fCurrentSingleTremolo->
      setSingleTremoloGraphicDurationKind (
        note->
          getNoteGraphicDurationKind ());

    // forget about this single tremolo
    fCurrentSingleTremolo = nullptr;
  }
}

//______________________________________________________________________________
/* JMI
void mxmlTree2MsrTranslator::attachCurrentArticulationsToChord ( // JMI
  S_msrChord chord)
{
  if (fCurrentArticulations.size ()) {

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceChords) {
      fLogOutputStream <<
        "Attaching current articulations to chord " <<
        chord->asString () <<
        endl;
    }
#endif

    list<S_msrArticulation>::const_iterator i;
    for (
      i=fCurrentArticulations.begin ();
      i!=fCurrentArticulations.end ();
      i++) {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceChords) {
        fLogOutputStream <<
          "Attaching articulation " <<  (*i) <<
          " to chord " << chord <<
          endl;
      }
#endif
          
      chord->
        appendArticulationToChord ((*i));
      } // for
  }
}
*/

/*
//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachCurrentOrnamentsToChord ( // JMI
  S_msrChord chord)
{
  if (fCurrentOrnamentsList.size ()) {

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceChords) {
      fLogOutputStream <<
        "Attaching current ornaments to chord " <<
        chord->asString () <<
        endl;
    }
#endif

    list<S_msrOrnament>::const_iterator i;
    for (
      i=fCurrentOrnamentsList.begin ();
      i!=fCurrentOrnamentsList.end ();
      i++) {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceChords) {
        fLogOutputStream <<
          "Attaching ornament " <<  (*i) << " to chord " <<
          chord <<
          endl;
      }
#endif
          
      chord->
        appendOrnamentToChord ((*i));
      } // for
  }
}
*/

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachPendingTemposToTheVoiceOfNote (
  S_msrNote note)
{
 // attach the pending tempos if any to the note
  if (fPendingTempos.size ()) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTracePasses) { // tempos ??? JMI
      fLogOutputStream <<
        "Attaching pending tempos to note " <<
        note->asString () <<
        endl;
    }
#endif

    // fetch the voice
    S_msrVoice
      voice =
        fetchVoiceFromPart (
          note->getInputLineNumber (),
     //     fCurrentMusicXMLStaffNumber, // JMI fCurrentStaffNumberToInsertInto,
          fCurrentStaffNumberToInsertInto, // JMI fCurrentMusicXMLStaffNumber,
          fCurrentMusicXMLVoiceNumber);

    while (fPendingTempos.size ()) {
      S_msrTempo
        tempo =
          fPendingTempos.front ();
          
      voice->
        appendTempoToVoice (tempo);
        
      fPendingTempos.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachPendingRehearsalsToTheVoiceOfNote (
  S_msrNote note)
{
 // attach the pending rehearsals if any to the note
  if (fPendingRehearsals.size ()) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTracePasses) { // Rehearsals ??? JMI
      fLogOutputStream <<
        "Attaching pending rehearsals to note " <<
        note->asString () <<
        endl;
    }
#endif

    // fetch the voice
    S_msrVoice
      voice =
        fetchVoiceFromPart (
          note->getInputLineNumber (),
     //     fCurrentMusicXMLStaffNumber, // JMI fCurrentStaffNumberToInsertInto,
          fCurrentStaffNumberToInsertInto, // JMI fCurrentMusicXMLStaffNumber,
          fCurrentMusicXMLVoiceNumber);

    while (fPendingRehearsals.size ()) {
      S_msrRehearsal
        rehearsal =
          fPendingRehearsals.front ();
          
      voice->
        appendRehearsalToVoice (rehearsal);
        
      fPendingRehearsals.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachPendingEyeGlassesToNote (
  S_msrNote note)
{
 // attach the pending eyeglasses if any to the note
  if (fPendingEyeGlasses.size ()) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTracePasses) { // EyeGlasses ??? JMI
      fLogOutputStream <<
        "Attaching pending eyeglasses to note " <<
        note->asString () <<
        endl;
    }
#endif

    while (fPendingEyeGlasses.size ()) {
      S_msrEyeGlasses
        eyeGlasses =
          fPendingEyeGlasses.front ();
          
      note->
        appendEyeGlassesToNote (eyeGlasses);
        
      fPendingEyeGlasses.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachPendingDampsToNote (
  S_msrNote note)
{
 // attach the pending damps if any to the note
  if (fPendingDamps.size ()) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTracePasses) {
      fLogOutputStream <<
        "Attaching pending damps to note " <<
        note->asString () <<
        endl;
    }
#endif

    while (fPendingDamps.size ()) {
      S_msrDamp
        damp =
          fPendingDamps.front ();
          
      note->
        appendDampToNote (damp);
        
      fPendingDamps.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachPendingDampAllsToNote (
  S_msrNote note)
{
 // attach the pending damp alls if any to the note
  if (fPendingDampAlls.size ()) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTracePasses) {
      fLogOutputStream <<
        "Attaching pending damp alls to note " <<
        note->asString () <<
        endl;
    }
#endif

    while (fPendingDampAlls.size ()) {
      S_msrDampAll
        dampAll =
          fPendingDampAlls.front ();
          
      note->
        appendDampAllToNote (dampAll);
        
      fPendingDampAlls.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachPendingOctaveShiftsToNote (
  S_msrNote note)
{
 // attach the pending octave shifts if any to the note
  if (fPendingOctaveShifts.size ()) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOctaveShifts) {
      fLogOutputStream <<
        "Attaching pending octave shifts to note " <<
        note->asString () <<
        endl;
    }
#endif

/* JMI
    // fetch the voice ??? JMI
    S_msrVoice
      voice =
        fetchVoiceFromPart (
          note->getInputLineNumber (),
          fCurrentMusicXMLStaffNumber,
          fCurrentMusicXMLVoiceNumber);
*/

    while (fPendingOctaveShifts.size ()) {
      S_msrOctaveShift
        octaveShift =
          fPendingOctaveShifts.front ();
          
      note->
        setNoteOctaveShift (octaveShift);
        
      fPendingOctaveShifts.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachPendingScordaturasToNote (
  S_msrNote note)
{
 // attach the pending scordatura if any to the note
  if (fPendingScordaturas.size ()) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTracePasses) {
      fLogOutputStream <<
        "Attaching pending scordaturas to note " <<
        note->asString () <<
        endl;
    }
#endif

    while (fPendingScordaturas.size ()) {
      S_msrScordatura
        scordatura =
          fPendingScordaturas.front ();
          
      note->
        appendScordaturaToNote (scordatura);
        
      fPendingScordaturas.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachPendingDynamicsToNote (
  S_msrNote note)
{
 // attach the pending dynamics if any to the note
  if (fPendingDynamics.size ()) {
    bool delayAttachment = false;
    
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceDynamics) {
      fLogOutputStream <<
        "Attaching pending dynamics to note " <<
        note->asString () <<
        endl;
    }
#endif

    if (fCurrentNoteIsARest) {
      if (gMsrOptions->fDelayRestsDynamics) {
        fLogOutputStream <<
          "Delaying dynamics attached to a rest until next note" <<
          endl;

        delayAttachment = true;
      }
      
      else {
        stringstream s;

        int numberOfDynamics = fPendingDynamics.size ();

        if (numberOfDynamics > 1) {
          s <<
            "there are " << numberOfDynamics << " dynamics";
        }
        else {
          s <<
            "there is 1 dynamics";
        }
        s <<
          " attached to a rest";
          
        msrMusicXMLWarning (
          gXml2lyOptions->fInputSourceName,
          note->getInputLineNumber (),
          s.str ());
      }
    }
    
    if (! delayAttachment) {
      while (fPendingDynamics.size ()) {
        S_msrDynamics
          dynamics =
            fPendingDynamics.front ();
            
        note->appendDynamicsToNote (dynamics);
        fPendingDynamics.pop_front ();
      } // while
    }
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachPendingOtherDynamicsToNote (
  S_msrNote note)
{
 // attach the pending other dynamics if any to the note
  if (fPendingOtherDynamics.size ()) {
    bool delayAttachment = false;
        
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceDynamics) {
      fLogOutputStream <<
        "Attaching pending other dynamics to note " <<
        note->asString () <<
        endl;
    }
#endif

    if (fCurrentNoteIsARest) {
      if (gMsrOptions->fDelayRestsDynamics) {
        fLogOutputStream <<
          "Delaying dynamics attached to a rest until next note" <<
          endl;

        delayAttachment = true;
      }
      
      else {
        stringstream s;

        int numberOfOtherDynamics = fPendingOtherDynamics.size ();

        if (numberOfOtherDynamics > 1) {
          s <<
            "there are " << numberOfOtherDynamics << " other dynamics";
        }
        else {
          s <<
            "there is 1 other dynamics";
        }
        s <<
          " attached to a rest";
          
        msrMusicXMLWarning (
          gXml2lyOptions->fInputSourceName,
          note->getInputLineNumber (),
          s.str ());
      }
    }
    
    if (! delayAttachment) {
      while (fPendingOtherDynamics.size ()) {
        S_msrOtherDynamics
          otherDynamics =
            fPendingOtherDynamics.front ();
            
        note->appendOtherDynamicsToNote (otherDynamics);
        fPendingOtherDynamics.pop_front ();
      } // while
    }
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachPendingWordsToNote (
  S_msrNote note)
{
  // attach the pending words if any to the note
  if (fPendingWords.size ()) {
    bool delayAttachment = false;
    
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceWords) {
      fLogOutputStream <<
        "Attaching pending words to note " <<
        note->asString () <<
        endl;
    }
#endif

    if (fCurrentNoteIsARest) {
      if (gMsrOptions->fDelayRestsWords) {
        fLogOutputStream <<
          "Delaying word(s) attached to a rest until next note" <<
          endl;

        delayAttachment = true;
      }
      
      else {
        stringstream s;

        int numberOfWords = fPendingWords.size ();

        if (numberOfWords > 1) {
          s <<
            "there are " << numberOfWords << " words";
        }
        else {
          s <<
            "there is 1 word";
        }
        s <<
          " attached to a rest";
          
        msrMusicXMLWarning (
          gXml2lyOptions->fInputSourceName,
          note->getInputLineNumber (),
          s.str ());
      }
    }

    if (! delayAttachment) {
      while (fPendingWords.size ()) {
        S_msrWords
          words =
            fPendingWords.front ();
            
        note->appendWordsToNote (words);
        
        fPendingWords.pop_front ();
      } // while
    }
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachPendingBeamsToNote (
  S_msrNote note)
{
  // attach the pending beams if any to the note
  if (fPendingBeams.size ()) {
    bool delayAttachment = false;
        
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceBeams) {
      fLogOutputStream <<
        "Attaching pending beams to note " <<
        note->asString () <<
        endl;
    }
#endif

    if (fCurrentNoteIsARest) {
      if (gMsrOptions->fDelayRestsBeams) {
        fLogOutputStream <<
          "Delaying beam attached to a rest until next note" <<
          endl;

        delayAttachment = true;
      }
      
      else {
        stringstream s;

        int numberOfBeams = fPendingBeams.size ();

        if (numberOfBeams > 1) {
          s <<
            "there are " << numberOfBeams << " beams";
        }
        else {
          s <<
            "there is 1 beam";
        }
        s <<
          " attached to a rest";
          
        msrMusicXMLWarning (
          gXml2lyOptions->fInputSourceName,
          note->getInputLineNumber (),
          s.str ());
      }
    }
    
    if (! delayAttachment) {
      while (fPendingBeams.size ()) {
        S_msrBeam
          beam =
            fPendingBeams.front ();
            
        note->appendBeamToNote (beam);
        fPendingBeams.pop_front ();
      } // while
    }
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachPendingSlursToNote (
  S_msrNote note)
{
  // attach the pending slurs if any to the note
  if (fPendingSlurs.size ()) {
    bool delayAttachment = false;
        
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceSlurs) {
      fLogOutputStream <<
        "Attaching pending slurs to note " <<
        note->asString () <<
        endl;
    }
#endif

    if (fCurrentNoteIsARest) {
      if (gMsrOptions->fDelayRestsSlurs) {
        fLogOutputStream <<
          "Delaying slur attached to a rest until next note" <<
          endl;

        delayAttachment = true;
      }
      
      else {
        stringstream s;

        int numberOfSlurs = fPendingSlurs.size ();

        if (numberOfSlurs > 1) {
          s <<
            "there are " << numberOfSlurs << " slurs";
        }
        else {
          s <<
            "there is 1 slur";
        }
        s <<
          " attached to a rest";
          
        msrMusicXMLWarning (
          gXml2lyOptions->fInputSourceName,
          note->getInputLineNumber (),
          s.str ());
      }
    }
    
    if (! delayAttachment) {
      while (fPendingSlurs.size ()) {
        S_msrSlur
          slur =
            fPendingSlurs.front ();
            
        note->appendSlurToNote (slur);
        fPendingSlurs.pop_front ();
      } // while
    }
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachPendingLigaturesToNote (
  S_msrNote note)
{
  // attach the pending ligatures if any to the note
  if (fPendingLigatures.size ()) {
    bool delayAttachment = false;
        
    if (fCurrentNoteIsARest) {
      if (gMsrOptions->fDelayRestsLigatures) {
        fLogOutputStream <<
          "Delaying ligature attached to a rest until next note" <<
          endl;

        delayAttachment = true;
      }

      else {
        stringstream s;

        int numberOfLigatures = fPendingLigatures.size ();

        if (numberOfLigatures > 1) {
          s <<
            "there are " << numberOfLigatures << " ligatures";
        }
        else {
          s <<
            "there is 1 ligature";
        }
        s <<
          " attached to a rest";
          
        msrMusicXMLWarning (
          gXml2lyOptions->fInputSourceName,
          note->getInputLineNumber (),
          s.str ());
      }
    }
    
    if (! delayAttachment) {
      stringstream s;

      int numberOfLigatures = fPendingLigatures.size ();

      if (numberOfLigatures > 1) {
        s <<
          "There are " << numberOfLigatures << " pending ligatures";
      }
      else {
        s <<
          "There is 1 pending ligature";
      }
      msrMusicXMLWarning (
        gXml2lyOptions->fInputSourceName,
        note->getInputLineNumber (),
        s.str ());

      // append ligatures to note only if they belong to a suitable voice,
      // i.e. above goes to voice 1 or 3, and below to voice 2 or 4
      
      list<S_msrLigature>::iterator
        iBegin = fPendingLigatures.begin (),
        iEnd   = fPendingLigatures.end (),
        i      = iBegin;
      for ( ; ; ) {
  //    list<S_msrLigature>::iterator i;
  //    for (i=fPendingLigatures.begin (); i!=fPendingLigatures.end (); i++) {

     //   if (i == iEnd) break;
        
        S_msrLigature
          ligature = (*i);

        // fetch ligatures placement kind
        msrPlacementKind
          ligaturePlacementKind =
            ligature->
              getLigaturePlacementKind ();

        // fetch note's voice
        S_msrVoice
          noteVoice =
            note->
              getNoteMeasureUplink ()->
                getMeasureSegmentUplink ()->
                  getSegmentVoiceUplink ();

        // handle ligature placement kind
        switch (ligaturePlacementKind) {
          case msrPlacementKind::kPlacementNone:
            break;
            
          case msrPlacementKind::kPlacementAbove:
            switch (noteVoice->getRegularVoiceStaffSequentialNumber ()) {
              case 1:
              case 3:
#ifdef TRACE_OPTIONS
                if (gTraceOptions->fTraceLigatures) {
                  fLogOutputStream <<
                    "Attaching pending ligature above to note '" <<
                    note->asString () <<
                    "' in voice \"" <<
                    noteVoice->getVoiceName () <<
                    "\"" <<
                    ", line " << ligature->getInputLineNumber () <<
                    endl;
                }
#endif

                note->appendLigatureToNote (ligature);
                i = fPendingLigatures.erase (i);
                break;
              default:
                ;
            } // switch
            break;
            
          case msrPlacementKind::kPlacementBelow:
            switch (noteVoice->getRegularVoiceStaffSequentialNumber ()) {
              case 2:
              case 4:
#ifdef TRACE_OPTIONS
                if (gTraceOptions->fTraceLigatures) {
                  fLogOutputStream <<
                    "Attaching pending ligature below to note '" <<
                    note->asString () <<
                    "' in voice \"" <<
                    noteVoice->getVoiceName () <<
                    "\"" <<
                    ", line " << ligature->getInputLineNumber () <<
                    endl;
                }
#endif

                note->appendLigatureToNote (ligature);
                i = fPendingLigatures.erase (i);
                break;
              default:
                ;
            } // switch
            break;
        } // switch

        if (++i == iEnd) break;
      } // for

      
  /*    while (fPendingLigatures.size ()) {
        S_msrLigature
          ligature =
            fPendingLigatures.front ();
            
        note->appendLigatureToNote (ligature);
        fPendingLigatures.pop_front ();
      } // while
 */
    }
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachPendingPedalsToNote (
  S_msrNote note)
{
  // attach the pending pedals if any to the note
  if (fPendingPedals.size ()) {
    bool delayAttachment = false;
        
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTracePedals) {
      fLogOutputStream <<
        "Attaching pending pedals to note " <<
        note->asString () <<
        endl;
    }
#endif

    if (fCurrentNoteIsARest) {
      if (gMsrOptions->fDelayRestsPedals) {
        fLogOutputStream <<
          "Delaying pedal attached to a rest until next note" <<
          endl;

        delayAttachment = true;
      }

      else {
        stringstream s;

        int numberOfPedals = fPendingPedals.size ();

        if (numberOfPedals > 1) {
          s <<
            "there are " << numberOfPedals << " pedals";
        }
        else {
          s <<
            "there is 1 pedal";
        }
        s <<
          " attached to a rest";
          
        msrMusicXMLWarning (
          gXml2lyOptions->fInputSourceName,
          note->getInputLineNumber (),
          s.str ());
      }
    }
    
    if (! delayAttachment) {
      while (fPendingPedals.size ()) {
        S_msrPedal
          pedal =
            fPendingPedals.front ();
            
        note->appendPedalToNote (pedal);
        fPendingPedals.pop_front ();
      } // while
    }
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachPendingSlashesToNote (
  S_msrNote note)
{
  // attach the pending slashes if any to the note
  if (fPendingSlashes.size ()) {
    bool delayAttachment = false;
        
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceSlashes) {
      fLogOutputStream <<
        "Attaching pending slashes to note " <<
        note->asString () <<
        endl;
    }
#endif

    if (fCurrentNoteIsARest) {
      if (gMsrOptions->fDelayRestsSlashes) {
        fLogOutputStream <<
          "Delaying slash attached to a rest until next note" <<
      endl;

        delayAttachment = true;
      }

      else {
        stringstream s;

        int numberOfSlashes = fPendingSlashes.size ();

        if (numberOfSlashes > 1) {
          s <<
            "there are " << numberOfSlashes << " slashes";
        }
        else {
          s <<
            "there is 1 slash";
        }
        s <<
          " attached to a rest";
          
        msrMusicXMLWarning (
          gXml2lyOptions->fInputSourceName,
          note->getInputLineNumber (),
          s.str ());
      }
    }
    
    if (! delayAttachment) {
      while (fPendingSlashes.size ()) {
        S_msrSlash
          slash =
            fPendingSlashes.front ();
            
        note->appendSlashToNote (slash);
        fPendingSlashes.pop_front ();
      } // while
    }
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachPendingWedgesToNote (
  S_msrNote note)
{
  // attach the pending wedges if any to the note
  if (fPendingWedges.size ()) {
    bool delayAttachment = false;
        
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceWedges) {
      fLogOutputStream <<
        "Attaching pending wedges to note " <<
        note->asString () <<
        endl;
    }
#endif

    if (fCurrentNoteIsARest) {
      if (gMsrOptions->fDelayRestsWedges) {
        fLogOutputStream <<
          "Delaying wedge attached to a rest until next note" <<
      endl;

        delayAttachment = true;
      }

      else {
        stringstream s;

        int numberOfWedges = fPendingWedges.size ();

        if (numberOfWedges > 1) {
          s <<
            "there are " << numberOfWedges << " wedges";
        }
        else {
          s <<
            "there is 1 wedge";
        }
        s <<
          " attached to a rest";
          
        msrMusicXMLWarning (
          gXml2lyOptions->fInputSourceName,
          note->getInputLineNumber (),
          s.str ());
      }
    }
    
    if (! delayAttachment) {
      while (fPendingWedges.size ()) {
        S_msrWedge
          wedge =
            fPendingWedges.front ();
            
        note->appendWedgeToNote (wedge);
        fPendingWedges.pop_front ();
      } // while
    }
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachPendingGlissandosToNote (
  S_msrNote note)
{
 // attach the pending glissandos if any to the note
  if (fPendingGlissandos.size ()) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceGlissandos) {
      fLogOutputStream <<
        "Attaching pending glissandos to note " <<
        note->asString () <<
        endl;
    }
#endif

    while (fPendingGlissandos.size ()) {
      S_msrGlissando
        glissando =
          fPendingGlissandos.front ();
          
      note->
        appendGlissandoToNote (glissando);

      // take care of no lyrics on kGlissandoTypeStop
      switch (glissando->getGlissandoTypeKind ()) {
        case msrGlissando::kGlissandoTypeNone:
        case msrGlissando::kGlissandoTypeStart:
          break;
          
        case msrGlissando::kGlissandoTypeStop:
          int inputLineNumber =
            glissando->getInputLineNumber ();
            
          // fetch the voice
          S_msrVoice
            voice =
              fetchVoiceFromPart (
                inputLineNumber,
     //     fCurrentMusicXMLStaffNumber, // JMI fCurrentStaffNumberToInsertInto,
                fCurrentStaffNumberToInsertInto, // JMI fCurrentMusicXMLStaffNumber,
                fCurrentMusicXMLVoiceNumber);
              
          // get the voice's stanzas map
          const map<string, S_msrStanza>&
            voiceStanzasMap =
              voice->
                getVoiceStanzasMap ();
                    
#ifdef TRACE_OPTIONS
              if (gTraceOptions->fTraceGlissandos) {
                fLogOutputStream <<
                  "--> attachPendingGlissandosToNote()"
                  ", voiceStanzasMap.size () = " <<
                  voiceStanzasMap.size () <<
                  ", fCurrentNoteHasLyrics = " <<
                  booleanAsString (fCurrentNoteHasLyrics) <<
                  ", line " << inputLineNumber <<
                  endl;
              }
#endif

          if (voiceStanzasMap.size ()) {
            // there are lyrics in this voice
            /* JMI
            if (! fCurrentNoteHasLyrics) {
              // append a skip to lyrics the same duration as the note
#ifdef TRACE_OPTIONS
              if (gTraceOptions->fTraceGlissandos) {
                fLogOutputStream <<
                  "Attaching a skip syllable to note '" <<
                  note->asString () <<
                  "' that has a glissando stop and no lyrics " <<
                  ", line " << inputLineNumber <<
                  endl;
              }
#endif

              for (
                map<string, S_msrStanza>::const_iterator i = voiceStanzasMap.begin ();
                i != voiceStanzasMap.end ();
                i++) {
                S_msrStanza stanza = (*i).second;
                // create a skip syllable
                S_msrSyllable
                  syllable =
                    msrSyllable::create (
                      inputLineNumber,
                      msrSyllable::kSyllableSkip,
                      msrSyllable::kSyllableExtendNone, // fCurrentSyllableExtendKind, // JMI
                      fCurrentNoteSoundingWholeNotesFromDuration,
                      stanza);
          
                // append syllable to current note's syllables list
                fCurrentNoteSyllables.push_back (
                  syllable);

                // append syllable to stanza
                stanza->
                  appendSyllableToStanza (syllable);
              } // for

              fASkipSyllableHasBeenGeneratedForcurrentNote = true;
            }
            */
          }
          break;
      } // switch

      fPendingGlissandos.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::attachPendingSlidesToNote (
  S_msrNote note)
{
 // attach the pending slides if any to the note
  if (fPendingSlides.size ()) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTracePasses) { // slides ??? JMI
      fLogOutputStream <<
        "Attaching pending slides to note " <<
        note->asString () <<
        endl;
    }
#endif

    while (fPendingSlides.size ()) {
      S_msrSlide
        slide =
          fPendingSlides.front ();
          
      note->
        appendSlideToNote (slide);
        
      // take care of no lyrics on kSlideTypeStop
      switch (slide->getSlideTypeKind ()) {
        case msrSlide::kSlideTypeNone:
        case msrSlide::kSlideTypeStart:
          break;
          
        case msrSlide::kSlideTypeStop:
          int inputLineNumber =
            slide->getInputLineNumber ();
            
          // fetch the voice
          S_msrVoice
            voice =
              fetchVoiceFromPart (
                inputLineNumber,
     //     fCurrentMusicXMLStaffNumber, // JMI fCurrentStaffNumberToInsertInto,
                fCurrentStaffNumberToInsertInto, // JMI fCurrentMusicXMLStaffNumber,
                fCurrentMusicXMLVoiceNumber);
              
          // get the voice's stanzas map
          const map<string, S_msrStanza>&
            voiceStanzasMap =
              voice->
                getVoiceStanzasMap ();
                    
#ifdef TRACE_OPTIONS
              if (gTraceOptions->fTraceSlides) {
                fLogOutputStream <<
                  "--> attachPendingSlidesToNote()"
                  ", voiceStanzasMap.size () = " <<
                  voiceStanzasMap.size () <<
                  ", fCurrentNoteHasLyrics = " <<
                  booleanAsString (fCurrentNoteHasLyrics) <<
                  ", line " << inputLineNumber <<
                  endl;
              }
#endif

          if (voiceStanzasMap.size ()) {
            // there are lyrics in this voice
            /* JMI
            if (! fCurrentNoteHasLyrics) {
              // append a skip to lyrics the same duration as the note
#ifdef TRACE_OPTIONS
              if (gTraceOptions->fTraceSlides) {
                fLogOutputStream <<
                  "Attaching a skip syllable to note '" <<
                  note->asString () <<
                  "' that has a slide stop and no lyrics " <<
                  ", line " << inputLineNumber <<
                  endl;
              }
#endif

              for (
                map<string, S_msrStanza>::const_iterator i = voiceStanzasMap.begin ();
                i != voiceStanzasMap.end ();
                i++) {
                S_msrStanza stanza = (*i).second;
                // create a skip syllable
                S_msrSyllable
                  syllable =
                    msrSyllable::create (
                      inputLineNumber,
                      msrSyllable::kSyllableSkip,
                      msrSyllable::kSyllableExtendNone, // fCurrentSyllableExtendKind, // JMI
                      fCurrentNoteSoundingWholeNotesFromDuration,
                      stanza);
          
                // append syllable to current note's syllables list
                fCurrentNoteSyllables.push_back (
                  syllable);

                // append syllable to stanza
                stanza->
                  appendSyllableToStanza (syllable);
              } // for

              fASkipSyllableHasBeenGeneratedForcurrentNote = true;
            }
            */
          }
          break;
      } // switch

      fPendingSlides.pop_front ();
    } // while
  }
}

void mxmlTree2MsrTranslator::attachPendingElementsToNote (
  S_msrNote note)
{
  // attach the pending tempos, if any, to the note's voice
 // JMI attachPendingTemposToTheVoiceOfNote (note);

  // attach the pending rehearsals, if any, to the note's voice
  attachPendingRehearsalsToTheVoiceOfNote (note);

  // attach the pending eyeglasses, if any, to the note
  attachPendingEyeGlassesToNote (note);

  // attach the pending damps, if any, to the note
  attachPendingDampsToNote (note);

  // attach the pending damp alls, if any, to the note
  attachPendingDampAllsToNote (note);

  // attach the pending scordaturas, if any, to the note
  attachPendingScordaturasToNote (note);

  // attach the pending octave shifts, if any, to the note
  attachPendingOctaveShiftsToNote (note);

  // attach the pending dynamics, if any, to the note
  attachPendingDynamicsToNote (note);

  // attach the pending other dynamics, if any, to the note
  attachPendingOtherDynamicsToNote (note);
  
  // attach the pending words, if any, to the note
  attachPendingWordsToNote (note);

  // attach the pending beams, if any, to the note
  attachPendingBeamsToNote (note);

  // attach the pending slurs, if any, to the note
  attachPendingSlursToNote (note);

  // attach the pending ligatures, if any, to the note
  attachPendingLigaturesToNote (note);

  // attach the pending pedals, if any, to the note
  attachPendingPedalsToNote (note);

  // attach the pending slashes, if any, to the note
  attachPendingSlashesToNote (note);

  // attach the pending wedges, if any, to the note
  attachPendingWedgesToNote (note);

  // attach the pending glissandos, if any, to the note
  attachPendingGlissandosToNote (note);

  // attach the pending slides, if any, to the note
  attachPendingSlidesToNote (note);
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitEnd ( S_note& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_note" <<
      ", line " << inputLineNumber <<
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

  /*
  Staff assignment is only needed for music notated on multiple staves.
  Used by both notes and directions.
  Staff values are numbers, with 1 referring to the top-most staff in a part.
  */
  
  // determine quarter tones note pitch
  fCurrentNoteQuarterTonesPitchKind =
    quarterTonesPitchKindFromDiatonicPitchAndAlteration (
      inputLineNumber,
      fCurrentNoteDiatonicPitchKind,
      fCurrentNoteAlterationKind);

  // determine quarter tones note display pitch
  fCurrentNoteQuarterTonesDisplayPitchKind =
    quarterTonesPitchKindFromDiatonicPitchAndAlteration (
      inputLineNumber,
      fCurrentDisplayDiatonicPitchKind,
      fCurrentNoteAlterationKind);

  // has the current note graphic duration been specified
  // in a '<type>' markup?
  switch (fCurrentNoteGraphicDurationKind) {
    case k_NoDuration:
      // use the same duration as the one from the duration
      // internally ??? JMI
      fCurrentNoteDisplayWholeNotesFromType =
        fCurrentNoteSoundingWholeNotesFromDuration;
      break;

    default:
      // convert note graphic duration into whole notes
      fCurrentNoteDisplayWholeNotesFromType =
        msrDurationKindAsWholeNotes (
          fCurrentNoteGraphicDurationKind);

#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceNotesDetails) {
        fLogOutputStream <<
          endl <<
          "(1):" <<
          endl <<
          "fCurrentNoteGraphicDurationKind = " <<
          msrDurationKindAsString (
            fCurrentNoteGraphicDurationKind) <<
          endl <<
          "fCurrentNoteDisplayWholeNotesFromType = " <<
          fCurrentNoteDisplayWholeNotesFromType <<
          endl <<
          "fCurrentNoteDotsNumber = " <<
          fCurrentNoteDotsNumber <<
          endl <<
          endl;
      }
#endif
    
      // take dots into account if any
      if (fCurrentNoteDotsNumber > 0) {
        rational
          wholeNotesIncrement =
            fCurrentNoteDisplayWholeNotesFromType * rational (1, 2);
        int
          dots =
            fCurrentNoteDotsNumber;
    
        while (dots > 0) {
          fCurrentNoteDisplayWholeNotesFromType +=
            wholeNotesIncrement;

          wholeNotesIncrement *= rational (1, 2);
    
          dots--;

#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceNotesDetails) {
            fLogOutputStream <<
              endl <<
              "(2):" <<
              endl <<
              "fCurrentNoteDisplayWholeNotesFromType = " <<
              fCurrentNoteDisplayWholeNotesFromType <<
              endl <<
              "wholeNotesIncrement = " <<
              wholeNotesIncrement <<
              endl <<
              "dots = " <<
              dots <<
              endl <<
              endl;
          }
#endif
        } // while

        fCurrentNoteDisplayWholeNotesFromType.rationalise ();
      }
      
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceNotesDetails) {
        fLogOutputStream <<
          endl <<
          "(3):" <<
          endl <<
          "fCurrentNoteGraphicDurationKind = " <<
          msrDurationKindAsString (
            fCurrentNoteGraphicDurationKind) <<
          endl <<
          "fCurrentNoteDisplayWholeNotesFromType = " <<
          fCurrentNoteDisplayWholeNotesFromType <<
          endl <<
          "fCurrentNoteDotsNumber = " <<
          fCurrentNoteDotsNumber <<
          endl <<
          endl;
      }
#endif
  } // switch

/* JMI
  // fetch the staff
  S_msrStaff
    staff =
      fetchStaffFromCurrentPart (
        inputLineNumber,
        fCurrentMusicXMLStaffNumber, // JMI fCurrentStaffNumberToInsertInto
        );

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceVoices) {
    fLogOutputStream <<
      "--> fCurrentMusicXMLVoiceNumber        = " <<
      fCurrentMusicXMLVoiceNumber <<
      endl <<
      "--> S_voice, fCurrentStaffNumberToInsertInto = " <<
      fCurrentStaffNumberToInsertInto <<
      endl <<
      "--> S_voice, current staff name  = " <<
      staff->getStaffName() <<
      endl;
  }
#endif

  // fetch the voice
  S_msrVoice
    voice =
      fetchVoiceFromPart (
        inputLineNumber,
        fCurrentStaffNumberToInsertInto,
        fCurrentMusicXMLVoiceNumber);
*/

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes) {
    fLogOutputStream <<
      "--> Gathered note information:" <<
      endl;

    gIndenter++;

    const int fieldWidth = 42;
    
    fLogOutputStream << left <<
      setw (fieldWidth) <<
      "CurrentNoteSoundingWholeNotesFromDuration" << " = " << 
      fCurrentNoteSoundingWholeNotesFromDuration <<
      endl <<
      setw (fieldWidth) <<
      "fCurrentNoteGraphicDuration" << " : " <<
        msrDurationKindAsString (
          fCurrentNoteGraphicDurationKind) <<
      endl <<
      setw (fieldWidth) <<
      "fCurrentNoteDotsNumber" << " : " <<
        fCurrentNoteDotsNumber <<
      endl <<
      setw (fieldWidth) <<
      "fCurrentNoteDisplayWholeNotesFromType" << " = " << 
      fCurrentNoteDisplayWholeNotesFromType <<
      endl <<
      
      setw (fieldWidth) <<
      "fCurrentNoteIsARest" << " = " << 
      booleanAsString (fCurrentNoteIsARest) <<
      endl <<
      
      setw (fieldWidth) <<
      "fCurrentDivisionsPerQuarterNote" << " = " <<
      fCurrentDivisionsPerQuarterNote <<
      endl <<
      
      setw (fieldWidth) <<
      "fCurrentNotePrintKind" << " = " <<
      msrNote::notePrintKindAsString (
        fCurrentNotePrintKind) <<
      endl <<

      setw (fieldWidth) <<
      "fCurrentMusicXMLStaffNumber =" << " = " <<
      fCurrentMusicXMLStaffNumber <<
      endl <<
      setw (fieldWidth) <<
      "fCurrentMusicXMLStaffNumber" << " = " <<
      fCurrentMusicXMLStaffNumber <<
      endl <<
      
      setw (fieldWidth) <<
      "fCurrentStaffNumberToInsertInto" << " = " <<
      fCurrentStaffNumberToInsertInto <<
      endl <<
      setw (fieldWidth) <<
      "fPreviousNoteMusicXMLStaffNumber" << " = " <<
      fPreviousNoteMusicXMLStaffNumber <<
      endl <<

      setw (fieldWidth) <<
      "fCurrentMusicXMLVoiceNumber" << " = " <<
      fCurrentMusicXMLVoiceNumber <<
      endl <<
      
      setw (fieldWidth) <<
      "inputLineNumber" << " = " <<
      inputLineNumber <<
      endl <<
      endl;

    gIndenter--;
  }
#endif

/* JMI
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes) { // JMI
    fLogOutputStream <<
      endl <<
      "==> BEFORE visitEnd (S_note&)" <<
      ", line " << inputLineNumber <<
      " we have:" <<
      endl;

    gIndenter++;

    const int fieldWidth = 27;

    fLogOutputStream << left <<
      setw (fieldWidth) << "--> fCurrentMusicXMLStaffNumber" << " = " <<
      fCurrentMusicXMLStaffNumber <<
      endl <<
      setw (fieldWidth) << "--> fCurrentMusicXMLVoiceNumber" << " = " <<
      fCurrentMusicXMLVoiceNumber <<
      endl <<
      setw (fieldWidth) << "--> current voice" << " = \"" <<
      currentVoice->getVoiceName () << "\"" <<
      endl;

    gIndenter--;

    fLogOutputStream <<
      "<==" <<
      endl <<
      * endl;
  }
#endif
*/

  if (fCurrentNoteIsAGraceNote) {
    // set current grace note display whole notes      
    fCurrentNoteDisplayWholeNotes =
      fCurrentNoteDisplayWholeNotesFromType;
  }
  
  else if (
    fCurrentTremoloTypeKind == kTremoloTypeStart
      ||
    fCurrentTremoloTypeKind == kTremoloTypeStop) {
    // double tremolo note
    if (fCurrentNoteGraphicDurationKind == k_NoDuration) {
      stringstream s;

      s <<
        "double tremolo note lacks a <type/>"; // JMI a completer

      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }

    // set current double tremolo note display whole notes 
    fCurrentNoteDisplayWholeNotes =
      fCurrentNoteDisplayWholeNotesFromType;
  }

  else if (fCurrentNoteIsARest) {
    // rest

    // set current rest sounding and display whole notes
    fCurrentNoteSoundingWholeNotes =
      fCurrentNoteSoundingWholeNotesFromDuration;

    fCurrentNoteDisplayWholeNotes =
      fCurrentNoteDisplayWholeNotesFromType;

/* JMI
    // set current note sounding and display whole notes
    if (fCurrentNoteSoundingWholeNotesFromDuration.getNumerator () == 0) {
      // only <type /> was met, no <duration /> was specified
      fCurrentNoteDisplayWholeNotes =
        fCurrentNoteDisplayWholeNotesFromType;
  
      fCurrentNoteSoundingWholeNotes =
        fCurrentNoteDisplayWholeNotes; // same value by default
    }
    else {
      // <duration /> was met
      fCurrentNoteSoundingWholeNotes =
        fCurrentNoteSoundingWholeNotesFromDuration;
  
      fCurrentNoteDisplayWholeNotes =
        fCurrentNoteSoundingWholeNotes; // same value by default
    }
    */
  }

  else {
    // other note
    
    // set current note sounding and display whole notes
    fCurrentNoteSoundingWholeNotes =
      fCurrentNoteSoundingWholeNotesFromDuration;

    fCurrentNoteDisplayWholeNotes =
      fCurrentNoteDisplayWholeNotesFromType;

    /* JMI
    if (fCurrentNoteSoundingWholeNotesFromDuration.getNumerator () == 0) {
      // only <type /> was met, no <duration /> was specified
      fCurrentNoteDisplayWholeNotes =
        fCurrentNoteDisplayWholeNotesFromType;
  
      fCurrentNoteSoundingWholeNotes =
        fCurrentNoteDisplayWholeNotes; // same value by default
    }
    else {
      // <duration /> was met
      fCurrentNoteSoundingWholeNotes =
        fCurrentNoteSoundingWholeNotesFromDuration;
  
      fCurrentNoteDisplayWholeNotes =
        fCurrentNoteSoundingWholeNotes; // same value by default
    }
    */
  }

  // create the (new) note
  S_msrNote
    newNote =
      msrNote::create (
        inputLineNumber,
        fCurrentMeasureNumber,
        
        msrNote::k_NoNoteKind,
          // will be set by 'setNoteKind()' when it becomes known later
        
        fCurrentNoteQuarterTonesPitchKind,
        
        fCurrentNoteSoundingWholeNotes,
        fCurrentNoteDisplayWholeNotes,
        
        fCurrentNoteDotsNumber,
        
        fCurrentNoteGraphicDurationKind,
        
        fCurrentNoteOctave,
        
        fCurrentNoteQuarterTonesDisplayPitchKind,
        fCurrentDisplayOctave,
        
        fCurrentNoteIsARest,
        fCurrentNoteIsUnpitched,

        fCurrentNoteIsACueNote,
        fCurrentNoteIsAGraceNote,

        fCurrentNotePrintKind,

        fCurrentNoteHeadKind,
        fCurrentNoteHeadFilledKind,
        fCurrentNoteHeadParenthesesKind);

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotesDetails) {
    fLogOutputStream <<
      "Creating note:" <<
      endl;

    gIndenter++;
    
    fLogOutputStream <<
      newNote <<
      endl;

    gIndenter--;
  }
#endif
  
  // set note accidentals
  newNote->
    setNoteAccidentalKind (
      fCurrentNoteAccidentalKind);

  newNote->
    setNoteEditorialAccidentalKind (
      fCurrentNoteEditorialAccidentalKind);

  newNote->
    setNoteCautionaryAccidentalKind (
      fCurrentNoteCautionaryAccidentalKind);

/* JMI
  // set note print kind
  newNote->
    setNotePrintKind (
      fCurrentNotePrintKind);
*/

  // fetch current note's voice
  S_msrVoice
    currentNotesVoice =
      fetchVoiceFromPart (
        inputLineNumber,
        fCurrentMusicXMLStaffNumber,
        fCurrentMusicXMLVoiceNumber);
  
  // sanity check
  msrAssert (
    currentNotesVoice != nullptr,
    "currentNotesVoice is null");
        
  // set current staff number to insert into if needed JMI ???
  if (fCurrentStaffNumberToInsertInto == K_NO_STAFF_NUMBER) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceStaves) {
      fLogOutputStream <<
        "==> setting fCurrentStaffNumberToInsertInto to " <<
        fCurrentMusicXMLStaffNumber <<
        ", in voice \"" <<
        currentNotesVoice->getVoiceName () <<
        "\"" <<
        ", line " << inputLineNumber <<
        endl;      
    }
#endif

    fCurrentStaffNumberToInsertInto = fCurrentMusicXMLStaffNumber;
  }
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceStaves) {
    fLogOutputStream <<
      "==> fetching voice to insert into" <<
      ", fCurrentStaffNumberToInsertInto = " <<
      fCurrentStaffNumberToInsertInto <<
      ", fPreviousNoteMusicXMLStaffNumber = " <<
      fPreviousNoteMusicXMLStaffNumber <<
      ", fCurrentMusicXMLStaffNumber = " <<
      fCurrentMusicXMLStaffNumber <<
      ", fCurrentMusicXMLVoiceNumber = " <<
      fCurrentMusicXMLVoiceNumber <<
      ", line " << inputLineNumber <<
      endl;      
  }
#endif

  // fetch voice to insert into
  S_msrVoice
    voiceToInsertInto =
      fetchVoiceFromPart (
        inputLineNumber,
        fCurrentStaffNumberToInsertInto,
        fCurrentMusicXMLVoiceNumber);
  
  // sanity check
  msrAssert (
    voiceToInsertInto != nullptr,
    "voiceToInsertInto is null");
        
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceStaves) {
    fLogOutputStream <<
      "==> is there a staff change?" <<
      ", fCurrentStaffNumberToInsertInto = " <<
      fCurrentStaffNumberToInsertInto <<
      ", fPreviousNoteMusicXMLStaffNumber = " <<
      fPreviousNoteMusicXMLStaffNumber <<
      ", fCurrentMusicXMLStaffNumber = " <<
      fCurrentMusicXMLStaffNumber <<
      ", in voice \"" <<
      voiceToInsertInto->getVoiceName() <<
      "\"" <<
      /* JMI
      ", fCurrentMusicXMLStaffNumber = " << fCurrentMusicXMLStaffNumber <<
      ", in staff \"" <<
      staff->getStaffName() <<
      "\"" <<
      */
      ", line " << inputLineNumber <<
      endl;      
  }
#endif

  // is there a staff change?
  fCurrentStaffChangeKind = k_NoStaffChange;
  
  if (
    fCurrentMusicXMLStaffNumber != fPreviousNoteMusicXMLStaffNumber
      &&
    fPreviousNoteMusicXMLStaffNumber != K_NO_STAFF_NUMBER
  ) {
    // yes, there is a staff change

    // fetch staff to change to
    S_msrStaff
      staffToChangeTo =
        fetchStaffFromCurrentPart (
          inputLineNumber,
          fCurrentMusicXMLStaffNumber);
  
    // is newNote a chord member note?
    if (fCurrentNoteBelongsToAChord) {
      // yes, newNote is a chord member note
      fCurrentStaffChangeKind = kStaffChangeChordMemberNote;

      // register the note as cross sta
      fCurrentNoteIsCrossStaves = true;

#ifdef TRACE_OPTIONS
      if (
        gTraceOptions->fTraceStaves
          ||
        gTraceOptions->fTraceVoices
      ) {
        fLogOutputStream <<
          "*** There is staff change for chord member note '" <<
          newNote->asShortString () <<
          "'in voice \"" <<
          voiceToInsertInto->getVoiceName () <<
          "\"" <<
          " from staff " << fPreviousNoteMusicXMLStaffNumber <<
          " to staff " << fCurrentMusicXMLStaffNumber <<
          ", \"" << staffToChangeTo->getStaffName () << "\"" <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif
  
      fCurrentStaffChangeKind = kStaffChangeChordMemberNote;

      // LilyPond doesn't support cross staff chords, JMI
      // so place newNote in its 'official' staff
    }
    
    else {
      // no, newNote is another note
      fCurrentStaffChangeKind = kStaffChangeOtherNote;

      // a staff change is necessary
      // to remain in this staff and not use the note's one

#ifdef TRACE_OPTIONS
      if (
        gTraceOptions->fTraceStaves
          ||
        gTraceOptions->fTraceVoices
      ) {
        fLogOutputStream <<
          "*** There is staff change for note '" <<
          newNote->asShortString () <<
          "'in voice \"" <<
          voiceToInsertInto->getVoiceName () <<
          "\"" <<
          " from staff " << fPreviousNoteMusicXMLStaffNumber <<
          " to staff " << fCurrentMusicXMLStaffNumber <<
          ", \"" << staffToChangeTo->getStaffName () << "\"" <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif
  
      // create the voice staff change
      S_msrVoiceStaffChange
        voiceStaffChange =
          msrVoiceStaffChange::create (
            inputLineNumber,
            staffToChangeTo);

  /* JMI
      // fetch the voice to insert into
      S_msrVoice
        voiceToInsertInto =
          fetchVoiceFromPart (
            inputLineNumber,
            fCurrentStaffNumberToInsertInto,
            fCurrentMusicXMLVoiceNumber);
*/

      // append it to the current sequence voice
      // before the note itself is appended
      voiceToInsertInto->
        appendVoiceStaffChangeToVoice (
          voiceStaffChange);

      // the actual note staff is already stored in newNote,
      // now fake its belonging to the current sequence staff
   // JMI  BOFBOFBOF ??? fCurrentMusicXMLStaffNumber = fCurrentStaffNumberToInsertInto;
    }
  }

  // handle note
  if (fCurrentNoteBelongsToAChord) {
    
    if (fCurrentNoteBelongsToATuplet) {
    
      // note is the second, third, ..., member of a chord
      // that is a member of a tuplet
      handleNoteBelongingToAChordInATuplet (
        newNote);
      
    }
    
    else if (fCurrentNoteIsAGraceNote) {

/* JMI
      fLogOutputStream <<
        "FOO" <<
        endl;
        */
        
      // note is the second, third, ..., member of a chord
      // that is a part of grace notes
      handleNoteBelongingToAChordInAGraceNotesGroup (
        newNote);
      
    }
    
    else {
      
      // note is the second, third, ..., member of a chord
      // whose first member is in
      // JMI ??? 'fLastHandledNoteInVoiceMap [currentVoice]'
      handleNoteBelongingToAChord (
        newNote);
      
    }
  }
  
  else if (fCurrentNoteBelongsToATuplet) {
    
    // note/rest is the first, second, third, ..., member of a tuplet
    handleNoteBelongingToATuplet (
      newNote);
    
  }
  
  else {
    
    // note/rest is standalone or a member of grace notes

    // this terminates a tuplet if any
    handlePendingTupletStopIfAny (
      inputLineNumber,
      newNote);

    // handle it
    handleStandaloneOrDoubleTremoloNoteOrGraceNoteOrRest (
      newNote);
  }
    
  // check <duration/> and <type/> consistency if relevant
  if (
    fCurrentNoteSoundingWholeNotesFromDuration
      !=
    fCurrentNoteDisplayWholeNotesFromType) {
    switch (newNote->getNoteKind ()) {
      case msrNote::k_NoNoteKind:
        break;

      case msrNote::kTupletMemberNote:
      case msrNote::kGraceTupletMemberNote:
      case msrNote::kTupletMemberUnpitchedNote:
        break;

      case msrNote::kRestNote:
      case msrNote::kSkipNote:
      case msrNote::kUnpitchedNote:
      case msrNote::kStandaloneNote:
      case msrNote::kChordMemberNote:
        if (! fCurrentNoteIsAGraceNote) {
          stringstream s;
      
          s <<
            "note duration inconsistency: divisions indicates " <<
            fCurrentNoteSoundingWholeNotesFromDuration <<
            " while type indicates " <<
            fCurrentNoteDisplayWholeNotesFromType <<
            ", using the latter";
      
          msrMusicXMLWarning (
            gXml2lyOptions->fInputSourceName,
            inputLineNumber,
            s.str ());
        }

        newNote->
          setNoteSoundingWholeNotes (
            fCurrentNoteDisplayWholeNotesFromType);
        break;

      case msrNote::kGraceNote:
      case msrNote::kGraceChordMemberNote:
      case msrNote::kDoubleTremoloMemberNote:
        break;
    } // switch
  }  

  // set newNote tie if any
  if (fCurrentTie) {
    newNote->
      setNoteTie (fCurrentTie);
  }
  
  // set its stem if any
  if (fCurrentStem) {
    newNote->
      setNoteStem (fCurrentStem);
  }

  // attach the beams if any to the note
  if (fPendingBeams.size ()) {
    for (
      list<S_msrBeam>::const_iterator i=fPendingBeams.begin ();
      i!=fPendingBeams.end ();
      i++) {
      newNote->
        appendBeamToNote ((*i));
    } // for

    fPendingBeams.clear ();
  }

  // attach the articulations if any to the note
  attachCurrentArticulationsToNote (newNote);

  // attach the technicals if any to the note
  attachCurrentTechnicalsToNote (newNote);
  attachCurrentTechnicalWithIntegersToNote (newNote);
  attachCurrentTechnicalWithFloatsToNote (newNote);
  attachCurrentTechnicalWithStringsToNote (newNote);

  // attach the ornaments if any to the note
  attachCurrentOrnamentsToNote (newNote);

  // attach the spanners if any to the note
  attachCurrentSpannersToNote (newNote);

  // attach the singleTremolo if any to the note
  attachCurrentSingleTremoloToNote (newNote);

/* JMI
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes) { // JMI
     const int fieldWidth = 27;

    fLogOutputStream << left <<
      endl <<
      "==> AFTER visitEnd (S_note&) " <<
      newNote->asString () <<
      ", line " << inputLineNumber <<
      " we have:" <<
      endl <<
      setw (fieldWidth) <<
      "--> fCurrentMusicXMLStaffNumber" << " = " <<
      fCurrentMusicXMLStaffNumber <<
      endl <<
      setw (fieldWidth) <<
      "--> fCurrentMusicXMLVoiceNumber" << " = " <<
      fCurrentMusicXMLVoiceNumber <<
      endl <<
      setw (fieldWidth) <<
      "--> current voice" << " = \"" <<
      currentVoice->getVoiceName () << "\"" <<
      endl <<
      "<==" <<
      endl <<
      endl;
  }
#endif
*/

  // handling the current pending harmonies if any,
  // so that they get attached to the note right now
  if (fPendingHarmoniesList.size ()) {    
    while (fPendingHarmoniesList.size ()) {
      S_msrHarmony
        harmony =
          fPendingHarmoniesList.front ();

      // set the harmony's voice uplink
      harmony->
        setHarmonyVoiceUplink (
          voiceToInsertInto);
      
      // set the harmony's whole notes
      harmony->
        setHarmonySoundingWholeNotes (
          fCurrentNoteSoundingWholeNotes);
      
      // attach the harmony to the note
      newNote->
        setNoteHarmony (harmony);
  
      // append the harmony to the harmony voice for the current voice
      S_msrVoice
        voiceHarmonyVoice =
          voiceToInsertInto->
            getHarmonyVoiceForRegularVoice ();
            
      voiceHarmonyVoice->
        appendHarmonyToVoice (
          harmony);
  
      // remove it from the list
      fPendingHarmoniesList.pop_front ();
    } // while
  
    // reset harmony counter
    fHarmonyVoicesCounter = 0;
  }

  // handling the current pending frames if any,
  // so that they get attached to the note right now
  if (fPendingFramesList.size ()) {    
    while (fPendingFramesList.size ()) {
      S_msrFrame
        frame =
          fPendingFramesList.front ();
  
      // attach the frame to the note
      newNote->
        setNoteFrame (frame);
  
      // remove it from the list
      fPendingFramesList.pop_front ();
    } // while
  }

  // handling the current pending figured bass if any,
  // so that it gets attached to the note right now
  if (fPendingFiguredBass) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceFiguredBass) {
      fLogOutputStream <<
        "--> figured bass" <<
        ", line " << inputLineNumber << ":" <<
        endl;
  
      gIndenter++;

      const int fieldWidth = 31;
      
      fLogOutputStream << left <<
        setw (fieldWidth) << "fCurrentPart" << " = " <<
        fCurrentPart->getPartCombinedName () <<
        endl <<
        setw (fieldWidth) << "fCurrentFiguredBassSoundingWholeNotes" << " = " <<
        fCurrentFiguredBassSoundingWholeNotes <<
        endl;
          
      gIndenter--;
    }
#endif
  
    if (fCurrentFiguredBassSoundingWholeNotes.getNumerator () == 0) {
      // no duration has been found,
      // use the note's sounding whole notes
      fCurrentFiguredBassSoundingWholeNotes =
        fCurrentNoteSoundingWholeNotes;
    }
  
    // create the figured bass
    // if the sounding whole notes is 0/1 (no <duration /> was met),
    // it will be set to the next note's sounding whole notes later
    S_msrFiguredBass
      figuredBass =
        msrFiguredBass::create (
          inputLineNumber,
          fCurrentPart,
          fCurrentFiguredBassSoundingWholeNotes,
          fCurrentFiguredBassParenthesesKind);
  
    // attach pending figures to the figured bass
    if (fPendingFiguredBassFigures.size ()) {
      for (
        list<S_msrFigure>::const_iterator i=fPendingFiguredBassFigures.begin ();
        i!=fPendingFiguredBassFigures.end ();
        i++) {
        figuredBass->
          appendFiguredFigureToFiguredBass ((*i));
      } // for
  
      fPendingFiguredBassFigures.clear ();
    }
  
    // append the figured bass to the current part
    fCurrentPart->
      appendFiguredBassToPart (
        voiceToInsertInto,
        figuredBass);
  
    fPendingFiguredBass = false;
  }

  // handling the pending grace notes group if any
  if (fPendingGraceNotesGroup && ! fCurrentNoteIsAGraceNote) {
    // this is the first note after the grace notes group

    // attach the current grace notes to this note
    switch (fPendingGraceNotesGroup->getGraceNotesGroupKind ()) {
      case msrGraceNotesGroup::kGraceNotesGroupBefore:
        newNote->
          setNoteGraceNotesGroupBefore (
            fPendingGraceNotesGroup);
        break;
      case msrGraceNotesGroup::kGraceNotesGroupAfter:
        newNote->
          setNoteGraceNotesGroupAfter (
            fPendingGraceNotesGroup);
        break;
    } // switch

    // forget about the pending grace notes
    fPendingGraceNotesGroup = nullptr;
  }
  
  // handling voices current chord map if needed
  if (! fCurrentNoteBelongsToAChord) {
    if (fOnGoingChord) {
      // newNote is the first note after the chord in the current voice
      // JMI ???
    }

    if (fCurrentDoubleTremolo) {
      // forget about a double tremolo containing a chord
    // JMI XXL BOFS  fCurrentDoubleTremolo = 0;
    }

    // forget about the current chord
    fCurrentChord = nullptr;
    
    fOnGoingChord = false;
  }

  // register newNote as the last found note for the current voice  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    fLogOutputStream <<
      "--> STORING " <<
      newNote->asShortString () <<
      " as last note found in voice " <<
      voiceToInsertInto->getVoiceName () <<
      endl <<
      "-->  fCurrentMusicXMLStaffNumber = " <<
      fCurrentMusicXMLStaffNumber <<
      endl <<
      "--> fCurrentMusicXMLVoiceNumber  = " <<
      fCurrentMusicXMLVoiceNumber <<
      endl <<
      /* JMI
      "--> staff name  = " <<
      staff->getStaffName () <<
      endl <<
      */
      "--> voice name  = " <<
      voiceToInsertInto->getVoiceName () <<
      endl;
  }
#endif
  
  fVoicesLastMetNoteMap [
    make_pair (
      fCurrentStaffNumberToInsertInto, // JMI fCurrentSequenceStaffNumber,
      fCurrentMusicXMLVoiceNumber)
    ] =
      newNote;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    fLogOutputStream <<
      "--> AFTER STORING " <<
      endl;
      
    printVoicesLastMetNoteMap (
      inputLineNumber);
  }
#endif
  
  // attach the pending elements, if any, to newNote
  // only now because <lyric> follows <glissando> and <slide> in MusicXML
  attachPendingElementsToNote (newNote);

  // lyrics if any have to be handled in all cases
  // only now because attachPendingElementsToNote()
  // may append skip syllables to the notes
  handleLyricsForNote (
    voiceToInsertInto,
    newNote);

  fOnGoingNote = false;
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::handleStandaloneOrDoubleTremoloNoteOrGraceNoteOrRest (
  S_msrNote newNote)
{
  int inputLineNumber =
    newNote->getInputLineNumber ();
    
  // register note/rest kind right now, to have a nice trace below
  if (fCurrentNoteIsAGraceNote) {
    // gracenote
    newNote->
      setNoteKind (
        msrNote::kGraceNote);
  }
  
  else if (
    fCurrentTremoloTypeKind == kTremoloTypeStart
      ||
    fCurrentTremoloTypeKind == kTremoloTypeStop) {
    // double tremolo note
    newNote->
      setNoteKind (
        msrNote::kDoubleTremoloMemberNote);
  }

  else {
    // standalone or unpitched note or rest
    if (fCurrentNoteIsARest) {
      newNote->
        setNoteKind (
          msrNote::kRestNote);
    }
    else if (fCurrentNoteIsUnpitched) {
      newNote->
        setNoteKind (
          msrNote::kUnpitchedNote);
    }
    else {
      newNote->
        setNoteKind (
          msrNote::kStandaloneNote);
    }
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) { // JMI
    fLogOutputStream <<
      "handleStandaloneOrDoubleTremoloNoteOrGraceNoteOrRest(), newNote = " <<
      endl;
    gIndenter++;
    fLogOutputStream <<
      newNote <<
      endl;
    gIndenter--;
  }
#endif
  
  // fetch current voice
  S_msrVoice
    currentVoice =
      fetchVoiceFromPart (
        inputLineNumber,
        fCurrentStaffNumberToInsertInto,
        fCurrentMusicXMLVoiceNumber);
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes) {    
    fLogOutputStream <<
      "Handling a standalone, double tremolo or grace note or rest" <<
      ", currentVoice = \"" <<
      currentVoice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      ":" <<
      endl;

    gIndenter++;

    fLogOutputStream <<
      newNote->asString () <<
      endl;

    const int fieldWidth = 25;

    fLogOutputStream << left <<
      setw (fieldWidth) << "voice" << " : \"" <<
      currentVoice->getVoiceName () << "\"" <<
      endl <<
      setw (fieldWidth) << "line:" << " : " <<
      inputLineNumber <<
      endl <<
      setw (fieldWidth) << "fCurrentNoteIsAGraceNote" << " : " <<
      booleanAsString (fCurrentNoteIsAGraceNote) <<
      endl;

    if (gTraceOptions->fTraceNotesDetails) {
      fLogOutputStream <<
        setw (fieldWidth) << "fPendingGraceNotesGroup" << " : " <<
        endl <<
        "======================= handleStandaloneOrDoubleTremoloNoteOrGraceNoteOrRest" <<
        ", line " << inputLineNumber <<
        endl;
      fCurrentPart->print (fLogOutputStream);
      fLogOutputStream <<
        "=======================" <<
        endl <<
        endl;
  
      if (fPendingGraceNotesGroup) {
        fLogOutputStream <<
          fPendingGraceNotesGroup;
      }
      else {
 //       fLogOutputStream <<
 //         "fCurrentGraceNotes is NULL"; // JMI
      }
  
      fLogOutputStream <<
        endl;
    }

    gIndenter--;
  }
#endif

  // handle the pending tuplets if any,
  // unless the current note is a grace note,
  // i.e. it is attached to a tuplet member note JMI
  if (! fCurrentNoteIsAGraceNote) {
    handleTupletsPendingOnTupletsStack (
      inputLineNumber);
  }

  if (fCurrentNoteIsAGraceNote) {
    if (! fPendingGraceNotesGroup) {
      // this is the first grace note in grace notes

#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceTuplets || gTraceOptions->fTraceGraceNotes) {
        fLogOutputStream <<
          "Creating grace notes for note '" <<
          newNote->asString () <<
          "' in voice \"" <<
          currentVoice->getVoiceName () << "\"" <<
          endl;
      }
#endif

      // create grace notes group
      fPendingGraceNotesGroup =
        msrGraceNotesGroup::create (
          inputLineNumber,
          msrGraceNotesGroup::kGraceNotesGroupBefore, // default value
          fCurrentGraceIsSlashed,
          currentVoice);

/* JMI
      // register that last handled note if any is followed by grace notes
      S_msrNote
        lastHandledNoteInVoice =
          currentVoice->
            getVoiceLastAppendedNote ();
          
      if (lastHandledNoteInVoice) {
        lastHandledNoteInVoice->
          setNoteIsFollowedByGraceNotes ();
      }
*/
      
      // append the grace notes to the current voice // NO JMI
      /*
      currentVoice->
        appendGraceNotesToVoice (
          fCurrentGraceNotes);
        //  */
    }

    // register that last handled note if any is followed by grace notes JMI ???
    S_msrNote
      lastHandledNoteInVoice =
        currentVoice->
          getVoiceLastAppendedNote ();
          
    if (lastHandledNoteInVoice) {
      lastHandledNoteInVoice->
        setNoteIsFollowedByGraceNotesGroup ();
    }

    // append newNote to the current grace notes
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceTuplets || gTraceOptions->fTraceGraceNotes) {
      fLogOutputStream <<
        "Appending note " <<
        newNote->asString () <<
        " to grace notes in voice \"" <<
        currentVoice->getVoiceName () <<
        "\", line " << inputLineNumber <<
        endl;
    }
#endif

    // append newNote to the pending grace notes group
    fPendingGraceNotesGroup->
      appendNoteToGraceNotesGroup (newNote);
  }

  else if (fCurrentTremoloTypeKind != k_NoTremoloType) {
    // newNote belongs to a tremolo

    switch (fCurrentTremoloTypeKind) {
      case k_NoTremoloType:
        // just to avoid a compiler message
        break;
        
      case kTremoloTypeSingle:
        // append newNote to the current voice
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceNotes) {
          fLogOutputStream <<
            "Appending standalone " <<
            newNote->asString () <<
            ", line " << newNote->getInputLineNumber () <<
            ", to voice \"" <<
            currentVoice->getVoiceName () <<
            "\"" <<
            endl;
        }
#endif
    
        currentVoice->
          appendNoteToVoice (newNote);

        // fCurrentSingleTremolo is handled in
        // attachCurrentSingleTremoloToNote()
        break;
        
      case kTremoloTypeStart:
        // register newNote as first element of the current double tremolo
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceNotes) {
          fLogOutputStream <<
            "Setting standalone note '" <<
            newNote->asString () <<
            "', line " << newNote->getInputLineNumber () <<
            ", as double tremolo first element" <<
            " in voice \"" <<
            currentVoice->getVoiceName () <<
            "\"" <<
            endl;
        }
#endif

        fCurrentDoubleTremolo->
          setDoubleTremoloNoteFirstElement (
            newNote);
        break;

      case kTremoloTypeStop:
        // register newNote as second element of the current double tremolo
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceNotes) {
          fLogOutputStream <<
            "Setting standalone note '" <<
            newNote->asString () <<
            "', line " << newNote->getInputLineNumber () <<
            ", as double tremolo second element" <<
            " in voice \"" <<
            currentVoice->getVoiceName () <<
            "\"" <<
            endl;
        }
#endif

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

/* YES JMI
    // attach pending tempos if any to the voice of note,
    // priot to the note itself
    attachPendingTemposToTheVoiceOfNote (newNote);
    */
    
    // append newNote to the current voice
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceNotes) {
      fLogOutputStream <<
        "Appending standalone " <<
        newNote->asString () <<
        ", line " << newNote->getInputLineNumber () <<
        ", to voice \"" <<
        currentVoice->getVoiceName () <<
        "\"" <<
        endl;
    }
#endif

    currentVoice->
      appendNoteToVoice (newNote);

    if (false) { // XXL, syllable sans fSyllableNote assigne JMI
      fLogOutputStream <<
        endl << endl <<
        "&&&&&&&&&&&&&&&&&& currentVoice (" <<
        currentVoice->getVoiceName () <<
        ") contents &&&&&&&&&&&&&&&&&&" <<
        endl <<
        currentVoice <<
        endl << endl;
    }
  }

  // take care of slurs JMI ???
  switch (fCurrentSlurTypeKind) {
    case msrSlur::kRegularSlurStart:
    case msrSlur::kPhrasingSlurStart:
      fFirstSyllableInSlurKind = fCurrentSyllableKind;
      break;
    case msrSlur::kSlurContinue:
      break;
    case msrSlur::kRegularSlurStop:
    case msrSlur::kPhrasingSlurStop:
      fFirstSyllableInSlurKind = msrSyllable::kSyllableNone;
      break;
    case msrSlur::k_NoSlur:
      ;
  } // switch

  // take care of ligatures JMI ???
  switch (fCurrentLigatureKind) {
    case msrLigature::kLigatureStart:
      fFirstSyllableInLigatureKind = fCurrentSyllableKind;
      break;
    case msrLigature::kLigatureContinue:
      break;
    case msrLigature::kLigatureStop:
      fFirstSyllableInLigatureKind = msrSyllable::kSyllableNone;
      break;
    default:
      ;
  } // switch
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::handleLyricsForNote (
  S_msrVoice currentVoice,
  S_msrNote  newNote)
{
  int inputLineNumber =
    newNote->getInputLineNumber ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    fLogOutputStream <<
      "Handling lyrics for note '" <<
      newNote->asShortString () <<
      "', line " << inputLineNumber <<
      endl;

    gIndenter++;

    const int fieldWidth = 33;
    
    fLogOutputStream <<
      setw (fieldWidth) <<
      "currentVoice" << " = \"" << currentVoice->getVoiceName () <<"\"" <<
      endl <<
      setw (fieldWidth) <<
      "newNote" << " = \"" << newNote->asShortString () << "\"" <<
      endl <<
      setw (fieldWidth) <<
      "fLastHandledNoteInVoiceHasLyrics" << " = " <<
      booleanAsString (
        fLastHandledNoteInVoiceHasLyrics) <<
      endl <<
      setw (fieldWidth) <<
      "fCurrentSyllableExtendKind" << "" << " = " <<
      msrSyllable::syllableExtendKindAsString (
        fCurrentSyllableExtendKind) <<
      endl <<
      setw (fieldWidth) <<
      "fOnGoingSyllableExtend" << " = " <<
      booleanAsString (
        fOnGoingSyllableExtend) <<
      endl <<

      setw (fieldWidth) <<
      "fCurrentNoteHasLyrics" << " = " <<
      booleanAsString (
        fCurrentNoteHasLyrics) <<
      endl <<
      setw (fieldWidth) <<
      "fASkipSyllableHasBeenGeneratedForcurrentNote" << " = " <<
      booleanAsString (
        fASkipSyllableHasBeenGeneratedForcurrentNote) <<
      endl <<

      setw (fieldWidth) <<
      "fCurrentStanzaNumber" << " = " << fCurrentStanzaNumber <<
      endl <<
      setw (fieldWidth) <<
      "fCurrentStanzaName" << " = " << fCurrentStanzaName << "\"" <<
      endl <<
      setw (fieldWidth) <<
      "fCurrentLyricTextsList" << " = ";

    msrSyllable::writeTextsList (
      fCurrentLyricTextsList,
      fLogOutputStream);
    
    fLogOutputStream <<
      endl;

    gIndenter--;
  }
#endif

  if (fCurrentNoteHasLyrics) {
    // newNote has lyrics attached to it
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceNotesDetails || gTraceOptions->fTraceLyricsDetails) {
      fLogOutputStream <<
        "Note '" <<
        newNote->asShortString () <<
        "' has lyrics attached to it" << " ***" <<
        endl;
    }
#endif
      
    for (
      list<S_msrSyllable>::const_iterator i =
        fCurrentNoteSyllables.begin ();
      i != fCurrentNoteSyllables.end ();
      i++ ) {
      S_msrSyllable
        syllable = (*i);
        
      // set syllable note uplink to newNote
      syllable->
        appendSyllableToNoteAndSetItsNoteUplink (
          newNote);
    } // for

    // forget about all of newNote's syllables
    fCurrentNoteSyllables.clear ();
  }

  else {
    // newNote has no lyrics attached to it:
    // don't create a skip for chord note members except the first
    // nor for grace notes

    bool doCreateASkipSyllable =
     // ! fASkipSyllableHasBeenGeneratedForcurrentNote; JMI
      ! fCurrentNoteHasLyrics;
    
    switch (fCurrentSyllableExtendKind) {
      case msrSyllable::kSyllableExtendSingle:
        break;
      case msrSyllable::kSyllableExtendStart:
        break;
      case msrSyllable::kSyllableExtendContinue:
 //       doCreateASkipSyllable = true; // JMI
        break;
      case msrSyllable::kSyllableExtendStop:
        break;
      case msrSyllable::kSyllableExtendNone:
  //      doCreateASkipSyllable = true; // JMI
        break;
    } // switch

    if (doCreateASkipSyllable) {
      if (
        !
          (
            fCurrentNoteBelongsToAChord
              ||
            fCurrentNoteIsAGraceNote
          )
      ) {
        // get the current voice's stanzas map
        const map<string, S_msrStanza>&
          voiceStanzasMap =
            currentVoice->
              getVoiceStanzasMap ();
              
        for (
          map<string, S_msrStanza>::const_iterator i = voiceStanzasMap.begin ();
          i != voiceStanzasMap.end ();
          i++) {
          S_msrStanza stanza = (*i).second;
          // create a skip syllable
          S_msrSyllable
            syllable =
              msrSyllable::create (
                inputLineNumber,
                msrSyllable::kSyllableSkip,
                fCurrentSyllableExtendKind,
                fCurrentNoteSoundingWholeNotesFromDuration,
                msrTupletFactor (
                  fCurrentNoteActualNotes,
                  fCurrentNoteNormalNotes),
                stanza);
    
          // set syllable note uplink to newNote
          syllable->
            appendSyllableToNoteAndSetItsNoteUplink (
              newNote);
    
          // append syllable to stanza
          stanza->
            appendSyllableToStanza (syllable);
        } // for
      }
    }
  }

  // register whether the new last handled note has lyrics
  fLastHandledNoteInVoiceHasLyrics =
    fCurrentNoteHasLyrics;

  // take care of ongoing extends
  switch (fCurrentSyllableExtendKind) {
    case msrSyllable::kSyllableExtendSingle:
      fOnGoingSyllableExtend = true;
      break;
    case msrSyllable::kSyllableExtendStart:
      fOnGoingSyllableExtend = true;
      break;
    case msrSyllable::kSyllableExtendContinue:
      msrAssert (
        fOnGoingSyllableExtend,
        "fOnGoingSyllableExtend is false");
      break;
    case msrSyllable::kSyllableExtendStop:
      fOnGoingSyllableExtend = false;
      break;
    case msrSyllable::kSyllableExtendNone:
      break;
  } // switch
    
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::handleNoteBelongingToAChord (
  S_msrNote newChordNote)
{
  int inputLineNumber =
    newChordNote->getInputLineNumber ();
    
  // set newChordNote kind as a chord member
  newChordNote->
    setNoteKind (
      msrNote::kChordMemberNote);

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    fLogOutputStream <<
      "Handling a note belonging to a chord" <<
      ", newChordNote:" <<
      endl;

    gIndenter++;

    fLogOutputStream << newChordNote <<
      endl;

    gIndenter--;
  }
#endif

  if (fCurrentNoteIsARest) {
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      "a rest cannot belong to a chord");
  }

  // fetch current voice
  int staffNumberToUse = -1001;
    // to avoid compiler warning for uninitialized variable
  
  switch (fCurrentStaffChangeKind) {
    case k_NoStaffChange:
      staffNumberToUse =
        fCurrentStaffNumberToInsertInto; // JMI fCurrentMusicXMLStaffNumber;
      break;
    case kStaffChangeChordMemberNote:
      if (fCurrentNoteIsCrossStaves) {
        staffNumberToUse =
          fCurrentStaffNumberToInsertInto;
    // JMI    staffNumberToUse = fCurrentMusicXMLStaffNumber; // keep it!
      }
      else {
   //   JMI   staffNumberToUse = fCurrentChordStaffNumber;
        staffNumberToUse =
          fCurrentStaffNumberToInsertInto;
      }
      break;
    case kStaffChangeOtherNote:
      staffNumberToUse =
        fCurrentStaffNumberToInsertInto;
     // JMI staffNumberToUse = fCurrentMusicXMLStaffNumber;
      break;
  } // switch
          
#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceNotes
      ||
    gTraceOptions->fTraceChords
    ) {
    fLogOutputStream << // JMI
      endl <<
      "***==> fCurrentStaffNumberToInsertInto = " <<
      fCurrentStaffNumberToInsertInto <<
      ", fCurrentChordStaffNumber = " <<
      fCurrentChordStaffNumber <<
      ", fPreviousNoteMusicXMLStaffNumber = " <<
      fPreviousNoteMusicXMLStaffNumber <<
      ", fCurrentMusicXMLStaffNumber = " <<
      fCurrentMusicXMLStaffNumber <<
      ", staffNumberToUse = " <<
      staffNumberToUse <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  S_msrVoice
    currentVoice =
      fetchVoiceFromPart (
        inputLineNumber,
        staffNumberToUse,
        fCurrentMusicXMLVoiceNumber);

    // sanity check JMI ???
    msrAssert (
      currentVoice != nullptr,
      "currentVoice is null");
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceChords) {
    fLogOutputStream <<
      "Handling a chord member note" <<
      ", currentVoice = \"" <<
      currentVoice->getVoiceName () <<
      "\", fOnGoingChord = " <<
      booleanAsString (fOnGoingChord) <<
      ", line " << inputLineNumber <<
      endl;
  }  
#endif

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceNotesDetails
      ||
    gTraceOptions->fTraceChordsDetails
  ) {
    fLogOutputStream <<
      endl <<
      "======================= handleNoteBelongingToAChord" <<
      ", line " << inputLineNumber <<
      endl;
    fCurrentPart->print (fLogOutputStream);
    fLogOutputStream <<
      "=======================" <<
      endl <<
      endl;

/* JMI
    if (fCurrentGraceNotes) {
      fLogOutputStream <<
        fCurrentGraceNotes;
    }
    else {
//       fLogOutputStream <<
//         "fCurrentGraceNotes is NULL"; // JMI
    }
*/

    fLogOutputStream <<
      endl;
  }
#endif

  // should a chord be created?
  if (! fOnGoingChord) {
    // newChordNote is the second note of the chord to be created

    // fetch this chord's first note,
    // i.e the last handled note for this voice

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceChords) {
      printVoicesLastMetNoteMap (
        inputLineNumber);
    }
#endif
    
    S_msrNote
      chordFirstNote =
  //      fVoicesLastMetNoteMap [currentVoice];
        fVoicesLastMetNoteMap [
          make_pair (
            fCurrentStaffNumberToInsertInto,
            fCurrentMusicXMLVoiceNumber)
          ];
      /*
        currentVoice->
          getVoiceLastAppendedNote (); ??? JMI
          */
    
    if (! chordFirstNote) {
      stringstream s;

      s <<
        "handleNoteBelongingToAChord():" <<
        endl <<
        "chordFirstNote is null on " <<
        newChordNote->asString () <<
        endl <<
        "fCurrentMusicXMLStaffNumber = " << fCurrentMusicXMLStaffNumber <<
        endl <<
        "fCurrentMusicXMLVoiceNumber = " << fCurrentMusicXMLVoiceNumber;
        
      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
        
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceChords) {
      fLogOutputStream <<
        "mxmlTree2MsrTranslator::handleNoteBelongingToAChord()" <<
        ", chordFirstNote:" <<
        endl;

      gIndenter++;

      fLogOutputStream <<
        chordFirstNote <<
        endl;

      gIndenter--;
    }
#endif

    // fetch chord first note's kind before createChordFromItsFirstNote(),
    // because the latter will change it to kChordMemberNote or kGraceChordMemberNote
    msrNote::msrNoteKind
      savedChordFirstNoteKind =
        chordFirstNote->getNoteKind ();
        
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceChords) {
      fLogOutputStream <<
        "Handling a note belonging to a chord" <<
        ", savedChordFirstNoteKind = " <<
        msrNote::noteKindAsString (savedChordFirstNoteKind) <<
        endl;

      gIndenter++;

      fLogOutputStream <<
        chordFirstNote <<
        endl;

      gIndenter--;
    }
#endif

    // create the current chord from its first note
    fCurrentChord =
      createChordFromItsFirstNote (
        inputLineNumber,
        currentVoice,
        chordFirstNote,
        msrNote::kChordMemberNote);
    
    // handle chord's first note
    switch (savedChordFirstNoteKind) {
      case msrNote::kRestNote:
        break;
        
      case msrNote::kSkipNote:
        break;
                
      case msrNote::kUnpitchedNote:
      case msrNote::kStandaloneNote:
        // remove last handled (previous current) note from the current voice
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceChords) {
          fLogOutputStream <<
            "Removing chord first note " <<
            chordFirstNote->asShortString () <<
            ", line " << inputLineNumber <<
            ", from voice \"" << currentVoice->getVoiceName () << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
    
#ifdef TRACE_OPTIONS
        if (
          gTraceOptions->fTraceNotesDetails
            ||
          gTraceOptions->fTraceChordsDetails
        ) {
          fLogOutputStream <<
            endl << endl <<
            "&&&&&&&&&&&&&&&&&& fCurrentPart contents &&&&&&&&&&&&&&&&&&" <<
            endl <<
            fCurrentPart <<
            endl <<
            endl;
        }
#endif
        
#ifdef TRACE_OPTIONS
        if (
          gTraceOptions->fTraceNotes
            ||
          gTraceOptions->fTraceChords
            ||
          gTraceOptions->fTraceStaves
            ||
          gTraceOptions->fTraceMeasures
            ||
          gTraceOptions->fTraceLyrics
        ) {
          fLogOutputStream << // JMI
            endl <<
            "***==> fCurrentStaffNumberToInsertInto = " <<
            fCurrentStaffNumberToInsertInto <<
            ", fPreviousNoteMusicXMLStaffNumber = " <<
            fPreviousNoteMusicXMLStaffNumber <<
            ", fCurrentMusicXMLStaffNumber = " <<
            fCurrentMusicXMLStaffNumber <<
            "', line " << inputLineNumber <<
            endl;
        }
#endif

        currentVoice->
          removeNoteFromVoice (
            inputLineNumber,
            chordFirstNote);
    
        // add chord to the voice instead
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceChords) {
          fLogOutputStream <<
            "Appending chord " << fCurrentChord->asString () <<
            " to voice \"" <<
            currentVoice->getVoiceName () <<
            "\"" <<
            endl;
        }
#endif
            
        currentVoice->
          appendChordToVoice (
            fCurrentChord);
        break;
        
      case msrNote::kDoubleTremoloMemberNote:
        {
          /* JMI
          // fetch chordFirstNote's sounding divisions
          int chordFirstNoteSoundingWholeNotes = // JMI
            chordFirstNote->
              getNoteSoundingWholeNotes ();
              */

          /* JMI
          // updating chord's divisions // JMI
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceChords) {
            fLogOutputStream <<
              "Updating sounding divisions for double tremolo chord '" <<
              "' " << chord->asString () <<
              " to '" << chordFirstNoteSoundingWholeNotes <<
              "' in voice \"" <<
              currentVoice->getVoiceName () <<
              "\"" <<
              endl;
          }
#endif
          
          chord->
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
              "chord first note '" <<
              chordFirstNote->asShortString () <<
              "' belongs to a double tremolo, but is not marked as such";
  
            msrInternalError (
              gXml2lyOptions->fInputSourceName,
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
        }
        break;
        
      case msrNote::kGraceNote:
      case msrNote::kGraceChordMemberNote:
        break;
        
      case msrNote::kChordMemberNote:
        // error? JMI
        break;
        
      case msrNote::kTupletMemberNote:
      case msrNote::kGraceTupletMemberNote:
      case msrNote::kTupletMemberUnpitchedNote:
        break;
  
      case msrNote::k_NoNoteKind:
        break;
    } // switch

    // account for chord being built
    fOnGoingChord = true;
  }

  else {
    // is newChordNote in the same chord but in another voice,
    // implying it is a member of a crossstaff chord?

/* JMI
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceChords) {
      fLogOutputStream <<
        "is newChordNote in the same chord but in another voice?" <<
        ", currentVoice = " <<
        currentVoice->getVoiceName () <<
        endl;

      printVoicesCurrentChordMap ();
    }
#endif
*/
  
  // register newChordNote as another member of chord
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceChords) {
      fLogOutputStream <<
        "Adding another note " <<
        newChordNote->asString() <<
        ", line " << inputLineNumber <<
        " to current chord in voice " <<
        currentVoice->getVoiceName () <<
        endl;
    }
#endif
  }

  // add newChordNote to current chord
  fCurrentChord->
    addAnotherNoteToChord (
      newChordNote, currentVoice);

  // copy newChordNote's elements if any to the current chord
  copyNoteElementsToChord (
    newChordNote, fCurrentChord);

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChordsDetails) {
    printCurrentChord ();
  }
#endif
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::handlePendingTupletStopIfAny (
  int       inputLineNumber,
  S_msrNote note)
{
  if (fCurrentATupletStopIsPending) {
    // finalize the tuplet, only now
    // in case the last element is a chord
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceTuplets) {
      fLogOutputStream <<
        "--> kTupletTypeStart: handling pending tuplet stop, note '" <<
        note->
          asShortString () <<
        "', line " << inputLineNumber <<
        endl;
    }
#endif

    finalizeTupletAndPopItFromTupletsStack (
      inputLineNumber);

    // the tuplet stop is not to be handled later
    fCurrentATupletStopIsPending = false;
  }
}

void mxmlTree2MsrTranslator::handleNoteBelongingToATuplet (
  S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();
    
 // register note as a tuplet member
 if (! fCurrentNoteIsUnpitched) {
    note->
      setNoteKind (msrNote::kTupletMemberNote);
  }
  else {
    note->
      setNoteKind (msrNote::kTupletMemberUnpitchedNote);
  }

  if (fCurrentNoteSoundingWholeNotesFromDuration.getNumerator () == 0) {
    // no duration has been found,
    // determine sounding from display whole notes
    note->
      determineTupletMemberSoundingFromDisplayWholeNotes (
        fCurrentNoteActualNotes,
        fCurrentNoteNormalNotes);
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceTuplets) {
    fLogOutputStream <<
      "Handling a note belonging to a tuplet" <<
      ", note: " <<
      note->
        asShortString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // is there an ongoing chord?
  if (! fOnGoingChord) {
    // note is the first one after a chord in a tuplet,
    // JMI
  }

  switch (fCurrentTupletTypeKind) {
    case msrTuplet::kTupletTypeStart:
      {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceTuplets) {
          fLogOutputStream <<
            "--> kTupletTypeStart: note = '" <<
            note->
              asShortString () <<
            "', line " << inputLineNumber <<
            endl;
        }
#endif

        handlePendingTupletStopIfAny (
          inputLineNumber,
          note);
        /* JMI
        if (fCurrentATupletStopIsPending) {
          // finalize the tuplet, only now
          // in case the last element is a chord
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceTuplets) {
            fLogOutputStream <<
              "--> kTupletTypeStart: handling pending tuplet stop, note '" <<
              note->
                asShortString () <<
              "', line " << inputLineNumber <<
              endl;
          }
#endif

          finalizeTupletAndPopItFromTupletsStack (
            inputLineNumber);

          // the tuplet stop is not to be handled later
          fCurrentATupletStopIsPending = false;
        }
        */

        // create the tuplet
        createTupletWithItsFirstNoteAndPushItToTupletsStack (
          note);
      
        // swith to continuation mode
        // this is handy in case the forthcoming tuplet members
        // are not explictly of the "continue" type
        fCurrentTupletTypeKind = msrTuplet::kTupletTypeContinue;
      }
      break;

    case msrTuplet::kTupletTypeContinue:
      {
        if (fTupletsStack.size ()) {
          S_msrTuplet
            currentTuplet =
              fTupletsStack.front ();
              
          // populate the tuplet at the top of the stack
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceTuplets) {
            fLogOutputStream <<
              "--> kTupletTypeContinue: adding tuplet member note '" <<
              note->
                asShortString () <<
              "' to stack top tuplet '" <<
              currentTuplet->asString () <<
              "', line " << inputLineNumber <<
              endl;
          }
#endif
  
          currentTuplet->
            addNoteToTuplet (
              note,
              nullptr /* JMI */);

#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceTupletsDetails) {
            displayTupletsStack (
              "############## kTupletTypeContinue");
          }
#endif

/* JMI
          // set note displayed divisions
          note->
            applyTupletMemberDisplayFactor (
              fTupletsStack.front ()->getTupletActualNotes (),
              fTupletsStack.front ()->getTupletNormalNotes ());
*/
        }
        
        else {
          stringstream s;

          s <<
            "handleNoteBelongingToATuplet():" <<
            endl <<
            "tuplet member note '" <<
            note->
              asShortString () <<
            "' cannot be added, tuplets stack is empty";

          msrInternalError (
            gXml2lyOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
      }
      break;

    case msrTuplet::kTupletTypeStop:
      {
        switch (fTupletsStack.size ()) {
          case 0:
            {
              stringstream s;
    
              s <<
                "handleNoteBelongingToATuplet():" <<
                endl <<
                "tuplet member note '" <<
                note->
                  asShortString () <<
                "' cannot be added, tuplets stack is empty";
    
              msrInternalError (
                gXml2lyOptions->fInputSourceName,
                inputLineNumber,
                __FILE__, __LINE__,
                s.str ());
            }
            break;

          case 1:
            {
              // outer-most tuplet:
              // add the note to to it before finalizing it
              S_msrTuplet
                currentTuplet =
                  fTupletsStack.front ();
                
              // populate the tuplet at the top of the stack
#ifdef TRACE_OPTIONS
              if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceTuplets) {
                fLogOutputStream <<
                  "--> kTupletTypeStop: adding outer-most tuplet member note '" <<
                  note->
                    asShortString () <<
                  "' to stack top tuplet '" <<
                  currentTuplet->asString () <<
                  "', line " << inputLineNumber <<
                  endl;
              }
#endif
          
              currentTuplet->
                addNoteToTuplet (
                  note,
                  nullptr /* JMI */);

#ifdef TRACE_OPTIONS
              if (gTraceOptions->fTraceTupletsDetails) {
                displayTupletsStack (
                  "############## kTupletTypeStop, outer-most");
              }
#endif
                
    //* JMI
              if (fCurrentATupletStopIsPending) {
                // end of a tuplet forces handling of the pending one 
#ifdef TRACE_OPTIONS
                if (gTraceOptions->fTraceTuplets) {
                  fLogOutputStream <<
                    "--> kTupletTypeStop: finalizing pending tuplet" <<
                    ", line " << inputLineNumber <<
                    endl;
                }
#endif

                finalizeTupletAndPopItFromTupletsStack (
                  inputLineNumber);
    
                // the tuplet stop is not to be handled later
                fCurrentATupletStopIsPending = false;
              }
              else {
#ifdef TRACE_OPTIONS
                if (gTraceOptions->fTraceTuplets) {
                  fLogOutputStream <<
                    "--> kTupletTypeStop: finalizing non-pending tuplet" << // JMI ???
                    ", line " << inputLineNumber <<
                    endl;
                }
#endif
    
                finalizeTupletAndPopItFromTupletsStack (
                  inputLineNumber);
              }
    //*/
    
              // don't pop the inner-most tuplet from the stack yet

        //      fCurrentATupletStopIsPending = true;
            }
            break;

          default:
            {
              // nested tuplet:
              // finalize it before adding the note to the containing tuplet
              handlePendingTupletStopIfAny (
                inputLineNumber,
                note);
              /* JMI
              if (fCurrentATupletStopIsPending) {
                // end of a tuplet forces handling of the pending one 
#ifdef TRACE_OPTIONS
                if (gTraceOptions->fTraceTuplets) {
                  fLogOutputStream <<
                    "--> kTupletTypeStop: finalizing tuplet" <<
                    ", line " << inputLineNumber <<
                    endl;
                }
#endif

                finalizeTupletAndPopItFromTupletsStack (
                  inputLineNumber);
    
                // the tuplet stop is not to be handled later
                fCurrentATupletStopIsPending = false;
              }
              */

              S_msrTuplet
                currentTuplet =
                  fTupletsStack.front ();
                
              // populate the tuplet at the top of the stack
#ifdef TRACE_OPTIONS
              if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceTuplets) {
                fLogOutputStream <<
                  "--> kTupletTypeStop: adding nested tuplet member note '" <<
                  note->
                    asShortString () <<
                  "' to stack top tuplet '" <<
                  currentTuplet->asString () <<
                  "', line " << inputLineNumber <<
                  endl;
              }
#endif
          
              currentTuplet->
                addNoteToTuplet (
                  note,
                  nullptr /* JMI */);
                

#ifdef TRACE_OPTIONS
              if (gTraceOptions->fTraceTupletsDetails) {
                displayTupletsStack (
                  "############## kTupletTypeStop, nested");
              }
#endif

      /* JMI
              // set note displayed divisions
              note->
                applyTupletMemberDisplayFactor (
                  fTupletsStack.front ()->getTupletActualNotes (),
                  fTupletsStack.front ()->getTupletNormalNotes ());
    */
            }
        } // switch

        // finalizeTupletAndPopItFromTupletsStack() should be delayed
        // in case this note
        // is the first one of a chord in a tuplet JMI XXL ???

        // the tuplet stop hast to be handled later
        fCurrentATupletStopIsPending = false; // JMI ???
      }
      break;

    case msrTuplet::kTupletTypeStartAndStopInARow:
      {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceTuplets) {
          fLogOutputStream <<
            "--> kTupletTypeStartAndStopInARow: note = '" <<
            note->
              asShortString () <<
            "', line " << inputLineNumber <<
            endl;
        }
#endif

        if (fCurrentSingleTremolo) {
          fCurrentTupletTypeKind = msrTuplet::kTupletTypeStartAndStopInARow;
        }
        else {
          stringstream s;
          
          s <<
            "one-note tuplet with a non single tremolo contents met";
          
          msrMusicXMLError (
            gXml2lyOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

        // create the tuplet
        createTupletWithItsFirstNoteAndPushItToTupletsStack (
          note);
        
        // the tuplet stop is not to be handled later
 //       fCurrentATupletStopIsPending = false; // JMI
      
        // finalize it
        finalizeTupletAndPopItFromTupletsStack (
          inputLineNumber);
      }
      break;

    case msrTuplet::kTupletTypeNone:
      break;
  } // switch

/* JMI ???
  // forget about this tuplet type, needed for nested tuplets
  fCurrentTupletTypeKind =
    msrTuplet::kTupletTypeNone;
    */
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::handleNoteBelongingToAChordInATuplet (
  S_msrNote newChordNote)
{
  /*
   The first note of a chord belonging to a tuplet
   is marked in MusicXML as a tuplet member only,
   it has already been appended to the voice in
   handleStandaloneOrDoubleTremoloNoteOrGraceNoteOrRest (),
   and the following ones are marked as both a tuplet and a chord member
  */
  
  int inputLineNumber =
    newChordNote->getInputLineNumber ();
    
  // set new note kind as a chord or grace chord member
  newChordNote->
    setNoteKind (
      msrNote::kChordMemberNote);

  // apply tuplet sounding factor to note
  if (fCurrentNoteSoundingWholeNotesFromDuration.getNumerator () == 0) {
    // no duration has been found,
    // determine sounding from display whole notes
    newChordNote->
      determineTupletMemberSoundingFromDisplayWholeNotes (
        fCurrentNoteActualNotes,
        fCurrentNoteNormalNotes);
  }

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceNotes
      ||
    gTraceOptions->fTraceChords
      ||
      gTraceOptions->fTraceTuplets
    ) {
    fLogOutputStream <<
      "Handling a note belonging to a chord in a tuplet" <<
      ", newChordNote: " <<
      newChordNote->
        asShortString () <<
      endl;
  }
#endif

  if (fCurrentNoteIsARest) {
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      "a rest cannot belong to a chord");
  }

  // fetch current voice
  S_msrVoice
    currentVoice =
      fetchVoiceFromPart (
        inputLineNumber,
        fCurrentStaffNumberToInsertInto,
        fCurrentMusicXMLVoiceNumber);

  // should a chord be created?
  if (! fOnGoingChord) {
    // this is the second note of the chord to be created,

    // fetch the current tuplet, i.e. the top of the stack
    S_msrTuplet currentTuplet;

    /* JMI
    if (fTupletsStack.size ()) {
      currentTuplet =
        fTupletsStack.front ();
    }
    else {
      stringstream s;
  
      s <<
        "handleNoteBelongingToAChordInATuplet():" <<
        endl <<
        " a tuplet member chord " <<
        "cannot be added, tuplets stack is empty";
  
      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
    */

    currentTuplet =
      fLastHandledTupletInVoiceMap [
        make_pair (
          fCurrentStaffNumberToInsertInto,
          fCurrentMusicXMLVoiceNumber)
      ];
    
    // remove and fetch tupletLastNote from the current tuplet,
    // it will be the first chord member note
    S_msrNote
      tupletLastNote =
        currentTuplet->
          removeLastNoteFromTuplet (
            inputLineNumber);

/* JMI
    S_msrNote
      tupletLastNote =
  //      fVoicesLastMetNoteMap [currentVoice];
        fVoicesLastMetNoteMap [
          make_pair (fCurrentMusicXMLStaffNumber, fCurrentMusicXMLVoiceNumber)
          ];

    currentVoice->
      removeNoteFromVoice (
        inputLineNumber,
        tupletLastNote);
*/

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceTupletsDetails) {
      displayTupletsStack (
        "############## After  removeLastNoteFromTuplet()");
    }
#endif

    // create the current chord from its first note
    fCurrentChord =
      createChordFromItsFirstNote (
        inputLineNumber,
        currentVoice,
        tupletLastNote,
        msrNote::kChordMemberNote);
    
    if (false) {
      fLogOutputStream <<
        endl << endl <<
        "&&&&&&&&&&&&&&&&&& currentVoice (" <<
        currentVoice->getVoiceName () <<
        ") contents &&&&&&&&&&&&&&&&&&" <<
        endl <<
        currentVoice <<
        endl << endl;
    }

    // add chord to the current tuplet instead of tupletLastNote
#ifdef TRACE_OPTIONS
    if (
      gTraceOptions->fTraceNotes
        ||
      gTraceOptions->fTraceChords
        ||
      gTraceOptions->fTraceTuplets
      ) {
      fLogOutputStream <<
        "Adding chord '" <<
        fCurrentChord->asString () <<
        "' to stack top tuplet '" <<
        currentTuplet->asString () <<
        "', line " << inputLineNumber <<
        endl;
    }
#endif

    currentTuplet->
      addChordToTuplet (fCurrentChord);

    if (fCurrentNoteSoundingWholeNotesFromDuration.getNumerator () == 0) {
      // no duration has been found,
      // determine sounding from display whole notes
      newChordNote->
        determineTupletMemberSoundingFromDisplayWholeNotes (
          fCurrentNoteActualNotes,
          fCurrentNoteNormalNotes);
    }

    // account for a chord being built
    fOnGoingChord = true;
  }

  // register note as another member of chord
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceChords) {
    fLogOutputStream <<
      "Adding another note " <<
      newChordNote->
        asShortString () <<
      ", line " << inputLineNumber <<
      " to current chord in voice " <<
      currentVoice->getVoiceName () <<
      endl;
  }
#endif

  fCurrentChord->
    addAnotherNoteToChord (
      newChordNote, currentVoice);

  // copy newChordNote's elements if any to the chord
  copyNoteElementsToChord (
    newChordNote, fCurrentChord);

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChordsDetails) {
    printCurrentChord ();
  }
#endif
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::handleNoteBelongingToAChordInAGraceNotesGroup (
  S_msrNote newChordNote)
{
  /*
   The first note of a chord belonging to a tuplet
   is marked in MusicXML as a tuplet member only,
   it has already been appended to the voice in
   handleStandaloneOrDoubleTremoloNoteOrGraceNoteOrRest (),
   and the following ones are marked as both a tuplet and a chord member
  */
  
  int inputLineNumber =
    newChordNote->getInputLineNumber ();
    
  // set new note kind as a grace chord member
  newChordNote->
    setNoteKind (
      msrNote::kGraceChordMemberNote);

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceNotes
      ||
    gTraceOptions->fTraceChords
      ||
    gTraceOptions->fTraceGraceNotes
    ) {
    fLogOutputStream <<
      "Handling a note belonging to a chord in grace notes" <<
      ", newChordNote is '" <<
      newChordNote->
        asShortString () <<
      "'" <<
      endl;
  }
#endif

  if (fCurrentNoteIsARest) {
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      "a rest cannot belong to a chord");
  }

  // fetch current voice
  S_msrVoice
    currentVoice =
      fetchVoiceFromPart (
        inputLineNumber,
        fCurrentStaffNumberToInsertInto,
        fCurrentMusicXMLVoiceNumber);

  // should a chord be created?
  if (! fOnGoingChord) {
    // this is the second note of the chord to be created

    S_msrNote chordFirstNote;

    if (fPendingGraceNotesGroup) {
      // the first note of the chord is currently the last one of fPendingGraceNotesGroup
      chordFirstNote =
        fPendingGraceNotesGroup->
          removeLastNoteFromGraceNotesGroup (
            inputLineNumber);
    }

    else {
      // the last handled note for the current voice
      // is the first note of the chord
  
      // fetch last handled note for this voice
      chordFirstNote =
        currentVoice->
          getVoiceLastAppendedNote ();
  
      if (! chordFirstNote) {
        stringstream s;
  
        s <<
          "handleNoteBelongingToAChordInGraceNotes():" <<
          endl <<
          "chordFirstNote is null on " <<
          newChordNote->asString ();
          
        msrInternalError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
          s.str ());
      }
    }
       
#ifdef TRACE_OPTIONS
    if (
      gTraceOptions->fTraceNotes
        ||
      gTraceOptions->fTraceChords
        ||
      gTraceOptions->fTraceGraceNotes
    ) {
      fLogOutputStream <<
        "The grace notes chord's first note is '" <<
        chordFirstNote->
          asShortString () <<
        "'" <<
        endl;
    }
#endif

    // set the first note's kind as grace chord member
    chordFirstNote->
      setNoteKind (
        msrNote::kGraceChordMemberNote);
        
    // create the current chord from its first note
    fCurrentChord =
      createChordFromItsFirstNote (
        inputLineNumber,
        currentVoice,
        chordFirstNote,
        msrNote::kGraceChordMemberNote);
    
    if (false) {
      fLogOutputStream <<
        endl << endl <<
        "&&&&&&&&&&&&&&&&&& currentVoice (" <<
        currentVoice->getVoiceName () <<
        ") contents &&&&&&&&&&&&&&&&&&" <<
        endl <<
        currentVoice <<
        endl <<
        endl;
    }

    if (fPendingGraceNotesGroup) {
      // append current chord to pending grace notes
      fPendingGraceNotesGroup->
        appendChordToGraceNotesGroup (
          fCurrentChord);
    }
    else {
      // append current chord to pending voice JMI ???
      currentVoice->
        appendChordToVoice (
          fCurrentChord);
    }
    

    /* JMI
    else {
      stringstream s;

      s <<
        "handleNoteBelongingToAChordInGraceNotes():" <<
        endl <<
        "tuplet member chord " << chord->asString () <<
        "cannot be added, tuplets stack is empty";

      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
    */

    // account for chord being built
    fOnGoingChord = true;
  }

  // register note as another member of chord
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceChords) {
    fLogOutputStream <<
      "Adding another note " <<
      newChordNote->
        asShortString () <<
      ", line " << inputLineNumber <<
      " to current chord in voice " <<
      currentVoice->getVoiceName () <<
      endl;
  }
#endif

  fCurrentChord->
    addAnotherNoteToChord (
      newChordNote, currentVoice);

  // copy newChordNote's elements if any to the chord
  copyNoteElementsToChord (
    newChordNote, fCurrentChord);

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChordsDetails) {
    printCurrentChord ();
  }
#endif
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::handleTupletsPendingOnTupletsStack (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets) {
    fLogOutputStream <<
      "Handling tuplets pending on tuplet stack" <<
      ", fCurrentStaffNumberToInsertInto = " <<
      fCurrentStaffNumberToInsertInto <<
      ", fCurrentMusicXMLStaffNumber = " <<
      fCurrentMusicXMLStaffNumber <<
      ", line: " << inputLineNumber <<
      endl;
  }

  if (gTraceOptions->fTraceTupletsDetails) {
    displayTupletsStack (
      "############## Before  handleTupletsPendingOnTupletsStack()");
  }
#endif

/* superfluous JMI
  // fetch current voice
  S_msrVoice
    currentVoice =
      fetchVoiceFromPart (
        inputLineNumber,
        fCurrentStaffNumberToInsertInto, // JMI fCurrentMusicXMLStaffNumber,
        fCurrentMusicXMLVoiceNumber);
*/

  // handle tuplets pending on the tuplet stack
  while (fTupletsStack.size ()) {
    S_msrTuplet
      pendingTuplet =
        fTupletsStack.front ();

    // finalize the tuplet, thus popping it off the stack
    finalizeTupletAndPopItFromTupletsStack (
      inputLineNumber);
  } // while

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTupletsDetails) {
    displayTupletsStack (
      "############## Before  handleTupletsPendingOnTupletsStack()");
  }
#endif
}

void mxmlTree2MsrTranslator::displayLastHandledTupletInVoiceMap (string header)
{
  fLogOutputStream <<
    endl <<
    header <<
    ", fLastHandledTupletInVoiceMap contains:";

  if (! fLastHandledTupletInVoiceMap.size ()) {
    fLogOutputStream <<
      " none" <<
      endl;
  }
  
  else {
    map<pair<int, int>, S_msrTuplet>::const_iterator
      iBegin = fLastHandledTupletInVoiceMap.begin (),
      iEnd   = fLastHandledTupletInVoiceMap.end (),
      i      = iBegin;
      
    fLogOutputStream <<
      endl;
    
    gIndenter++;
    
    for ( ; ; ) {
      fLogOutputStream <<
        "staff " << (*i).first.first <<
        ", voice " <<  (*i).first.second <<
        endl;
//        "\"" << (*i).first->getVoiceName () <<
//        "\" ----> " << (*i).second->asString ();
      if (++i == iEnd) break;
      fLogOutputStream << endl;
    } // for
    
    fLogOutputStream <<
      endl;
      
    gIndenter--;
  }

  fLogOutputStream <<
    endl;
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::createAndPrependImplicitBarLine (
  int inputLineNumber)
{     
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceBarlines || gTraceOptions->fTraceRepeats) {
    fLogOutputStream <<
      "Prepending an implicit repeat start barline at the beginning of part" <<
      fCurrentPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // fetch current voice
  S_msrVoice
    currentVoice =
      fetchVoiceFromPart (
        inputLineNumber,
        fCurrentMusicXMLStaffNumber,
        fCurrentMusicXMLVoiceNumber);

  // create the implicit barline
  S_msrBarline
    implicitBarline =
      msrBarline::create (
        inputLineNumber,
        msrBarline::kBarlineCategoryRepeatStart,
        msrBarline::kBarlineHasSegnoNo,
        msrBarline::kBarlineHasCodaNo,
        msrBarline::kBarlineLocationLeft,
        msrBarline::kBarlineStyleHeavyLight,
        msrBarline::kBarlineEndingTypeStart,
        fCurrentBarlineEndingNumber,
        msrBarline::kBarlineRepeatDirectionForward,
        fCurrentBarlineRepeatWingedKind,
        fCurrentBarlineTimes);

  // prepend the implicit barline to the voice
  gIndenter++;
  
  currentVoice->
    prependBarlineToVoice (implicitBarline);

  gIndenter--;
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::handleRepeatStart (
  S_msrBarline& barline)
{
  int inputLineNumber =
    barline->getInputLineNumber ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    fLogOutputStream <<
      "Handling repeat start" <<
    /* JMI
      ", measure '" <<
        barline->getBarlineMeasureNumber () <<
      "', position " <<
      barline->getBarlinePositionInMeasure () <<
      */
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // set the barline category
  barline->
    setBarlineCategory (
      msrBarline::kBarlineCategoryRepeatStart);

  // prepare for repeat in current part
  fCurrentPart->
    prepareForRepeatInPart (
      inputLineNumber);

  // append the bar line to the current part
  fCurrentPart->
    appendBarlineToPart (barline);

  fOnGoingRepeat = true;
  fOnGoingRepeatHasBeenCreated = false; 
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::handleRepeatEnd (
  S_msrBarline& barline)
{
  int inputLineNumber =
    barline->getInputLineNumber ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    fLogOutputStream <<
      "Handling repeat end" <<
    /* JMI
      ", measure '" <<
        barline->getBarlineMeasureNumber () <<
      "', position " <<
      barline->getBarlinePositionInMeasure () <<
      */
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // set the barline category
  barline->
    setBarlineCategory (
      msrBarline::kBarlineCategoryRepeatEnd);

  // append the bar line to the current part
  fCurrentPart->
    appendBarlineToPart (barline);

  // prepend an implicit bar line to the part if needed
  if (
    ! fOnGoingRepeat
      &&
    gMsrOptions->fCreateImplicitInitialRepeatBarline
  ) {
    createAndPrependImplicitBarLine (
      inputLineNumber);
   }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    fLogOutputStream <<
      "Appending a repeat to part " <<
      fCurrentPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fCurrentPart->
    createRepeatUponItsEndAndAppendItToPart (
      inputLineNumber,
      fCurrentMeasureNumber,
      barline->getBarlineTimes ());

  fOnGoingRepeat = false;
  fRepeatEndCounter++;
  fOnGoingRepeatHasBeenCreated = true; // JMI
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::handleEndingStart (
  S_msrBarline& barline)
{
  int inputLineNumber =
    barline->getInputLineNumber ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    fLogOutputStream <<
      "Handling repeat ending start" <<
    /* JMI
      ", measure '" <<
        barline->getBarlineMeasureNumber () <<
      "', position " <<
      barline->getBarlinePositionInMeasure () <<
    */
      ", fOnGoingRepeat = " <<
      booleanAsString (fOnGoingRepeat) <<
      ", fOnGoingRepeatHasBeenCreated = " <<
      booleanAsString (fOnGoingRepeatHasBeenCreated) <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // ending start, don't know yet whether it's hooked or hookless
  fCurrentEndingStartBarline = barline;
  
  // is there an ongoing repeat?
  if (fOnGoingRepeat) {
    // yes

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceRepeats) {
      fLogOutputStream <<
        endl <<
        endl <<
        "****************** handleEndingStart" <<
        ", line " << inputLineNumber <<
        endl <<
        fCurrentPart <<
        endl <<
        endl <<
        endl;
    }
#endif

    if (fOnGoingRepeatHasBeenCreated) {
      /* JMI
      fLogOutputStream <<
        "!!!!! YESYESYES !!!!!" <<
        endl;
        */
    }
    
    else {
      /* JMI
      fLogOutputStream <<
        "!!!!! NONONO !!!!!" <<
        endl;
        */

      // create the enclosing repeat and append it to the part
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceRepeats) {
        fLogOutputStream <<
          "Creating a regular repeat in part " <<
          fCurrentPart->getPartCombinedName () <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif
    
      fCurrentPart->
        createRegularRepeatUponItsFirstEndingInPart (
          inputLineNumber,
          barline->getBarlineTimes ());

      fOnGoingRepeatHasBeenCreated = true;
    }
  }

  else {
    // no, there is an implicit repeat starting at the beginning of the part,
    // that encloses everything from the beginning on

    // append an implicit repeat to the current part
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceRepeats) {
      fLogOutputStream <<
        "Prepending an implicit barline ahead of part " <<
        fCurrentPart->getPartCombinedName () <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    createAndPrependImplicitBarLine (
      inputLineNumber);
 
    // create the enclosing repeat and append it to the part
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceRepeats) {
      fLogOutputStream <<
        "Creating a repeat enclosing everything from the beginning of part " <<
        fCurrentPart->getPartCombinedName () <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif
    
    fCurrentPart->
      createEnclosingRepeatUponItsFirstEndingInPart (
        inputLineNumber,
        barline->getBarlineTimes ());

    fOnGoingRepeat = true;
    fOnGoingRepeatHasBeenCreated = true;
  }


/* JMI
  // create a new last segment to collect the repeat ending contents
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceVoices) {
    fLogOutputStream <<
      "Creating a new last segment for a repeat ending contents for voice \"" <<
      currentVoice->getVoiceName () << "\"" <<
      endl;
  }
#endif
      
  currentVoice->
    createNewLastSegmentForVoice (
      barline->getInputLineNumber ());
*/

/* JMI
  // set the barline category
  barline->
    setBarlineCategory (
      msrBarline::kBarlineCategoryHookedEndingStart);
  */

  // append the bar line to the current part
  fCurrentPart->
    appendBarlineToPart (barline);
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::handleHookedEndingEnd (
  S_msrBarline& barline)
{
  int inputLineNumber =
    barline->getInputLineNumber ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    fLogOutputStream <<
      "Handling repeat hooked ending end" <<
    /* JMI
      ", measure '" <<
        barline->getBarlineMeasureNumber () <<
      "', position " <<
      barline->getBarlinePositionInMeasure () <<
      */
      ", line " << inputLineNumber <<
      endl;
  }
#endif

/* JMI
  // fetch current voice
  S_msrVoice
    currentVoice =
      fetchVoiceFromPart (
        inputLineNumber,
        fCurrentMusicXMLStaffNumber,
        fCurrentMusicXMLVoiceNumber);
*/

  if (! fOnGoingRepeat) {
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      "found a repeat hooked ending out of context");
  }

  // set current barline start category
  fCurrentEndingStartBarline->
    setBarlineCategory (
      msrBarline::kBarlineCategoryHookedEndingStart);
  
  // set the barline category
  barline->
    setBarlineCategory (
      msrBarline::kBarlineCategoryHookedEndingEnd);

  // append the bar line to the current part
  fCurrentPart->
    appendBarlineToPart (barline);

  // create a hooked repeat ending from the current segment
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    fLogOutputStream <<
      "Appending a new hooked repeat ending to part " <<
      fCurrentPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
          
  fCurrentPart->
    appendRepeatEndingToPart (
      inputLineNumber,
      fCurrentBarlineEndingNumber,
      msrRepeatEnding::kHookedEnding);
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::handleHooklessEndingEnd (
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
    barline->getInputLineNumber ();
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    fLogOutputStream <<
      "Handling repeat hookless ending end" <<
    /* JMI
      ", measure '" <<
        barline->getBarlineMeasureNumber () <<
      "', position " <<
      barline->getBarlinePositionInMeasure () <<
      */
      ", line " << inputLineNumber <<
      endl;
  }
#endif

/* JMI
  // fetch current voice
  S_msrVoice
    currentVoice =
      fetchVoiceFromPart (
        inputLineNumber,
        fCurrentMusicXMLStaffNumber,
        fCurrentMusicXMLVoiceNumber);
*/

  if (! fOnGoingRepeat) {
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      "found a repeat hookless ending out of context");
  }
  
  // set current barline start category
  fCurrentEndingStartBarline->
    setBarlineCategory (
      msrBarline::kBarlineCategoryHooklessEndingStart);
  
  // set the barline category
  barline->
    setBarlineCategory (
      msrBarline::kBarlineCategoryHooklessEndingEnd);
  
  // append the bar line to the current part
  fCurrentPart->
    appendBarlineToPart (barline);

  // create a hookless repeat ending from the current segment
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    fLogOutputStream <<
      "Appending a new hookless repeat ending to part " <<
      fCurrentPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
                
  fCurrentPart->
    appendRepeatEndingToPart (
      inputLineNumber,
      fCurrentBarlineEndingNumber,
      msrRepeatEnding::kHooklessEnding);

  // forget about the current ending start barline
  fCurrentEndingStartBarline = nullptr;
  
  fOnGoingRepeat = false;
  fOnGoingRepeatHasBeenCreated = false; // JMI
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_rehearsal& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_rehearsal" <<
      ", line " << inputLineNumber <<
      endl;
  }

/*
<!ATTLIST sound
    tempo CDATA #IMPLIED
    dynamics CDATA #IMPLIED
    dacapo %yes-no; #IMPLIED
    segno CDATA #IMPLIED
    dalsegno CDATA #IMPLIED
    coda CDATA #IMPLIED
    tocoda CDATA #IMPLIED
    divisions CDATA #IMPLIED
    forward-repeat %yes-no; #IMPLIED
    fine CDATA #IMPLIED
    %time-only;
    pizzicato %yes-no; #IMPLIED
    pan CDATA #IMPLIED
    elevation CDATA #IMPLIED
    damper-pedal %yes-no-number; #IMPLIED
    soft-pedal %yes-no-number; #IMPLIED
    sostenuto-pedal %yes-no-number; #IMPLIED
>

<sound id="brass.trombone.bass"/>

<sound dynamics="106.67"/>

<sound dynamics="69"/>

      <direction placement="above">
        <direction-type>
          <rehearsal default-y="15" font-size="11.3" font-weight="bold">A</rehearsal>
        </direction-type>
      </direction>
*/

  string rehearsalValue = elt->getValue();

  string rehearsalEnclosure = 
    elt->getAttributeValue ("enclosure");

  msrRehearsal::msrRehearsalKind
    rehearsalKind =
      msrRehearsal::kNone; // default value

  if      (rehearsalEnclosure == "none") {
    rehearsalKind = msrRehearsal::kNone;
  }
  else if (rehearsalEnclosure == "rectangle") {
    rehearsalKind = msrRehearsal::kRectangle;
  }
  else if (rehearsalEnclosure == "oval") {
    rehearsalKind = msrRehearsal::kOval;
  }
  else if (rehearsalEnclosure == "circle") {
    rehearsalKind = msrRehearsal::kCircle;
  }
  else if (rehearsalEnclosure == "bracket") {
    rehearsalKind = msrRehearsal::kBracket;
  }
  else if (rehearsalEnclosure == "triangle") {
    rehearsalKind = msrRehearsal::kTriangle;
  }
  else if (rehearsalEnclosure == "diamond") {
    rehearsalKind = msrRehearsal::kDiamond;
  }
  else {
    if (rehearsalEnclosure.size ()) {
      stringstream s;
      
      s <<
        "rehearsal enclosure \"" << rehearsalEnclosure <<
        "\"" << " is not handled, ignored";
        
      msrMusicXMLWarning (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        s.str ());
    }    
  }

  // create a rehearsal
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRehearsals) {
    fLogOutputStream <<
      "Creating rehearsal \"" << rehearsalValue << "\"" <<
      " in part " <<
      fCurrentPart->getPartCombinedName () <<
      endl;
  }
#endif

  S_msrRehearsal
    rehearsal =
      msrRehearsal::create (
        inputLineNumber,
        rehearsalKind,
        rehearsalValue,
        fCurrentDirectionPlacementKind);

  // append the rehearsal to the pending tempos list
  fPendingRehearsals.push_back (rehearsal);
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_harmony& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_harmony" <<
      ", line " << inputLineNumber <<
      endl;
  }

  fHarmonyVoicesCounter++;
  
  fCurrentHarmonyInputLineNumber       = inputLineNumber;
  fCurrentHarmonyRootDiatonicPitchKind = k_NoDiatonicPitch;
  fCurrentHarmonyRootAlterationKind    = kNatural;
  fCurrentHarmonyKind                  = k_NoHarmony;
  fCurrentHarmonyKindText              = "";
  fCurrentHarmonyInversion             = K_HARMONY_NO_INVERSION;
  fCurrentHarmonyBassDiatonicPitchKind = k_NoDiatonicPitch;
  fCurrentHarmonyBassAlterationKind    = kNatural;
  fCurrentHarmonyDegreeValue           = -1;
  fCurrentHarmonyDegreeAlterationKind  = kNatural;
}

void mxmlTree2MsrTranslator::visitStart ( S_root_step& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_root_step" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string step = elt->getValue ();

  checkStep (
    inputLineNumber,
    step);
     
  fCurrentHarmonyRootDiatonicPitchKind =
    msrDiatonicPitchKindFromString (
      step [0]);
}

void mxmlTree2MsrTranslator::visitStart ( S_root_alter& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_root_alter" <<
      ", line " << inputLineNumber <<
      endl;
  }

  float rootAlter = (float)(*elt);

  fCurrentHarmonyRootAlterationKind =
    msrAlterationKindFromMusicXMLAlter (
      rootAlter);
      
  if (fCurrentHarmonyRootAlterationKind == k_NoAlteration) {
    stringstream s;

    s <<
      "root alter '" << rootAlter << "'"
      "' should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";
      
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxmlTree2MsrTranslator::visitStart ( S_kind& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_kind" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // harmony kind
  // ----------------------------------

  string kind = elt->getValue ();

  fCurrentHarmonyKindText =
    elt->getAttributeValue ("text");

  // check harmony kind
  if      (kind == "major")
    fCurrentHarmonyKind = kMajorHarmony;
  else if (kind == "minor")
    fCurrentHarmonyKind = kMinorHarmony;
  else if (kind == "augmented")
    fCurrentHarmonyKind = kAugmentedHarmony;
  else if (kind == "diminished")
    fCurrentHarmonyKind = kDiminishedHarmony;
    
  else if (kind == "dominant")
    fCurrentHarmonyKind = kDominantHarmony;
  else if (kind == "major-seventh")
    fCurrentHarmonyKind = kMajorSeventhHarmony;
  else if (kind == "minor-seventh")
    fCurrentHarmonyKind = kMinorSeventhHarmony;
  else if (kind == "diminished-seventh")
    fCurrentHarmonyKind = kDiminishedSeventhHarmony;
    
  else if (kind == "augmented-seventh")
    fCurrentHarmonyKind = kAugmentedSeventhHarmony;
  else if (kind == "half-diminished")
    fCurrentHarmonyKind = kHalfDiminishedHarmony;
  else if (kind == "major-minor")
    fCurrentHarmonyKind = kMinorMajorSeventhHarmony;
    
  else if (kind == "major-sixth")
    fCurrentHarmonyKind = kMajorSixthHarmony;
  else if (kind == "minor-sixth")
    fCurrentHarmonyKind = kMinorSixthHarmony;
    
  else if (kind == "dominant-ninth")
    fCurrentHarmonyKind = kDominantNinthHarmony;
  else if (kind == "major-ninth")
    fCurrentHarmonyKind = kMajorNinthHarmony;
  else if (kind == "minor-ninth")
    fCurrentHarmonyKind = kMinorNinthHarmony;
    
  else if (kind == "dominant-11th")
    fCurrentHarmonyKind = kDominantEleventhHarmony;
  else if (kind == "major-11th")
    fCurrentHarmonyKind = kMajorEleventhHarmony;
  else if (kind == "minor-11th")
    fCurrentHarmonyKind = kMinorEleventhHarmony;
    
  else if (kind == "dominant-13th")
    fCurrentHarmonyKind = kDominantThirteenthHarmony;
  else if (kind == "major-13th")
    fCurrentHarmonyKind = kMajorThirteenthHarmony;
  else if (kind == "minor-13th")
    fCurrentHarmonyKind = kMinorThirteenthHarmony;
    
  else if (kind == "suspended-second")
    fCurrentHarmonyKind = kSuspendedSecondHarmony;
  else if (kind == "suspended-fourth")
    fCurrentHarmonyKind = kSuspendedFourthHarmony;
    
  else if (kind == "Neapolitan")
    fCurrentHarmonyKind = kNeapolitanHarmony;
  else if (kind == "Italian")
    fCurrentHarmonyKind = kItalianHarmony;
  else if (kind == "French")
    fCurrentHarmonyKind = kFrenchHarmony;
  else if (kind == "German")
    fCurrentHarmonyKind = kGermanHarmony;

  else if (kind == "pedal")
    fCurrentHarmonyKind = kPedalHarmony;
  else if (kind == "power")
    fCurrentHarmonyKind = kPowerHarmony;
  else if (kind == "Tristan")
    fCurrentHarmonyKind = kTristanHarmony;
  else if (kind == "other")
    fCurrentHarmonyKind = kOtherHarmony;
  else if (kind == "none") {
    fCurrentHarmonyKind = kNoneHarmony;
  }
    
  else {
    if (kind.size ()) {
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        "harmony kind \"" + kind + "\" os unknown");
    }
    else {
      msrMusicXMLWarning (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        "empty harmony kind, replaced by 'major'");

      fCurrentHarmonyKind = kMajorHarmony; 
    }
  }

  // harmony use symbols
  // ----------------------------------

  string kindUseSymbols = elt->getAttributeValue ("use-symbols");

/* JMI
  if      (kindUseSymbols == "yes")
    fCurrentTupletTypeKind = msrTuplet::kStartTuplet; // JMI
  else if (kindUseSymbols == "no")
    fCurrentTupletTypeKind = msrTuplet::kStopTuplet;
  else {
    if (kindUseSymbols.size ()) {
      stringstream s;
      
      s <<
        "kind use-symbols \"" << kindUseSymbols <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
      }
  }
*/

  // harmony use stack degrees
  // ----------------------------------
  
  string kindStackDegrees = elt->getAttributeValue ("stack-degrees");

/* JMI
  if      (kindStackDegrees == "yes")
    fCurrentTupletTypeKind = msrTuplet::kStartTuplet; // JMI
  else if (kindStackDegrees == "no")
    fCurrentTupletTypeKind = msrTuplet::kStopTuplet;
  else {
    if (kindStackDegrees.size ()) {
      stringstream s;
      
      s <<
        "kind stack-degrees " << kindStackDegrees <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
      }
  }
  */

  // harmony use parentheses degrees
  // ----------------------------------
  
  string kindParenthesesDegrees = elt->getAttributeValue ("parentheses-degrees");

/* JMI
  if      (kindParenthesesDegrees == "yes")
    fCurrentTupletTypeKind = msrTuplet::kStartTuplet; // JMI
  else if (kindParenthesesDegrees == "no")
    fCurrentTupletTypeKind = msrTuplet::kStopTuplet;
  else {
    if (kindParenthesesDegrees.size ()) {
      stringstream s;
      
      s <<
        "kind parentheses-degrees \"" << kindParenthesesDegrees <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
      }
  }
  */

  // harmony use bracket degrees
  // ------------------
  
  string kindBracketDegrees = elt->getAttributeValue ("bracket-degrees");

/* JMI
  if      (kindBracketDegrees == "yes")
    fCurrentTupletTypeKind = msrTuplet::kStartTuplet; // JMI
  else if (kindBracketDegrees == "no")
    fCurrentTupletTypeKind = msrTuplet::kStopTuplet;
  else {
    if (kindBracketDegrees.size ()) {
      stringstream s;
      
      s <<
        "kind bracket-degrees \"" << kindBracketDegrees <<
        "\" is unknown";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
      }
  }
  */
}

void mxmlTree2MsrTranslator::visitStart ( S_inversion& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_bass_step" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

/*
  Inversion is a number indicating which inversion is used:
  0 for root position, 1 for first inversion, etc.
*/

  fCurrentHarmonyInversion = (int)(*elt);
}

void mxmlTree2MsrTranslator::visitStart ( S_bass_step& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_bass_step" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string step = elt->getValue();
  
  checkStep (
    inputLineNumber,
    step);

  fCurrentHarmonyBassDiatonicPitchKind =
    msrDiatonicPitchKindFromString (
      step [0]);
}

void mxmlTree2MsrTranslator::visitStart ( S_bass_alter& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_bass_alter" <<
      ", line " << inputLineNumber <<
      endl;
  }

  float bassAlter = (float)(*elt);

  fCurrentHarmonyBassAlterationKind =
    msrAlterationKindFromMusicXMLAlter (
      bassAlter);
      
  if (fCurrentHarmonyBassAlterationKind == k_NoAlteration) {
    stringstream s;

    s <<
      "bass alter '" << bassAlter << "'"
      "' should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";
      
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxmlTree2MsrTranslator::visitStart ( S_degree& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_degree" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void mxmlTree2MsrTranslator::visitStart ( S_degree_value& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_degree_value" <<
       ", line " << elt->getInputLineNumber () <<
     endl;
  }

  fCurrentHarmonyDegreeValue = (int)(*elt);
}

void mxmlTree2MsrTranslator::visitStart ( S_degree_alter& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_degree_alter" <<
      ", line " << inputLineNumber <<
      endl;
  }

  float degreeAlter = (float)(*elt);

  fCurrentHarmonyDegreeAlterationKind =
    msrAlterationKindFromMusicXMLAlter (
      degreeAlter);
      
  if (fCurrentHarmonyDegreeAlterationKind == k_NoAlteration) {
    stringstream s;

    s <<
      "degree alter '" << degreeAlter << "'"
      "' should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";
      
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxmlTree2MsrTranslator::visitStart ( S_degree_type& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_degree_type" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string degreeType = elt->getValue ();

  // check harmony degree type
  if      (degreeType == "add")
    fCurrentHarmonyDegreeTypeKind =
      msrHarmonyDegree::kHarmonyDegreeTypeAdd;
    
  else if (degreeType == "alter")
    fCurrentHarmonyDegreeTypeKind =
      msrHarmonyDegree::kHarmonyDegreeTypeAlter;
    
  else if (degreeType == "subtract")
    fCurrentHarmonyDegreeTypeKind =
      msrHarmonyDegree::kHarmonyDegreeTypeSubstract;
    
  else {
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        "harmony degree-type \"" + degreeType + "\" is unknown");
  }
}

void mxmlTree2MsrTranslator::visitEnd ( S_degree& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_degree" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // create harmony degree
  S_msrHarmonyDegree
    harmonyDegree =
      msrHarmonyDegree::create (
        inputLineNumber,
        fCurrentHarmonyDegreeValue,
        fCurrentHarmonyDegreeAlterationKind,
        fCurrentHarmonyDegreeTypeKind);

  // register it in current harmony degrees list
  fCurrentHarmonyDegreesList.push_back (
    harmonyDegree);
}

void mxmlTree2MsrTranslator::visitEnd ( S_harmony& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_harmony" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // convert root diatonic pitch to a quarter tone pitch
  fCurrentHarmonyRootQuarterTonesPitchKind =
    quarterTonesPitchKindFromDiatonicPitchAndAlteration (
      inputLineNumber,
      fCurrentHarmonyRootDiatonicPitchKind,
      fCurrentHarmonyRootAlterationKind);

  // convert bass diatonic pitch to a quarter tone pitch
  fCurrentHarmonyBassQuarterTonesPitchKind =
    quarterTonesPitchKindFromDiatonicPitchAndAlteration (
      inputLineNumber,
      fCurrentHarmonyBassDiatonicPitchKind,
      fCurrentHarmonyBassAlterationKind);

  // check data consistency
  if (
    fCurrentHarmonyKind == kOtherHarmony
      &&
    fCurrentHarmonyDegreesList.size () == 0) {
    msrMusicXMLWarning (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      "harmony kind is 'other' but there are no harmony degrees, this is strange...");
  }

  if (
    fCurrentHarmonyRootQuarterTonesPitchKind
      ==
    fCurrentHarmonyBassQuarterTonesPitchKind) {
    stringstream s;

    s <<
      "harmony root and bass notes are both equal to '" <<
      msrDiatonicPitchKindAsString (
        gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
        diatonicPitchKindFromQuarterTonesPitchKind (
          inputLineNumber,
          fCurrentHarmonyRootQuarterTonesPitchKind)) <<        
      "', ignoring the latter";

    msrMusicXMLWarning (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      s.str ());

    fCurrentHarmonyBassQuarterTonesPitchKind =
      k_NoQuarterTonesPitch_QTP;
  }

  // create the harmony
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies) {
    fLogOutputStream <<
      "Creating a harmony" <<
      ", line " << inputLineNumber << ":" <<
      endl;

    gIndenter++;

    const int fieldWidth = 32;
    
    fLogOutputStream << left <<
      setw (fieldWidth) << "fCurrentPart" << " = " <<
      fCurrentPart->getPartCombinedName () <<
      endl <<
      /*
      setw (fieldWidth) << "harmonyVoice" << " = " <<
      harmonyVoice->getVoiceName () <<
      endl <<
      */
      
      setw (fieldWidth) << "fCurrentHarmonyRootDiatonicPitch" << " = " <<
      msrDiatonicPitchKindAsString (
        gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
        fCurrentHarmonyRootDiatonicPitchKind) <<
      endl <<
      setw (fieldWidth) << "fCurrentHarmonyRootAlteration" << " = " <<
      msrAlterationKindAsString(
        fCurrentHarmonyRootAlterationKind) <<
      endl <<
      
      setw (fieldWidth) << "fCurrentHarmonyKind" << " = " <<
      msrHarmonyKindAsString (
        fCurrentHarmonyKind) <<
      endl <<
      setw (fieldWidth) << "fCurrentHarmonyKindText" << " = " <<
      fCurrentHarmonyKindText <<
      endl <<
      
      setw (fieldWidth) << "fCurrentHarmonyInversion" << " = " <<
      fCurrentHarmonyInversion <<
      endl <<
      
      setw (fieldWidth) << "fCurrentHarmonyBassDiatonicPitch" << " = " <<
      msrDiatonicPitchKindAsString (
        gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
        fCurrentHarmonyBassDiatonicPitchKind) <<
      endl <<
      
      setw (fieldWidth) << "fCurrentHarmonyBassAlteration" << " = " <<
      msrAlterationKindAsString(
        fCurrentHarmonyBassAlterationKind) <<
      endl <<
      
      setw (fieldWidth) << "fCurrentNoteSoundingWholeNotes" << " = " <<
      fCurrentNoteSoundingWholeNotes <<
      endl;
        
    gIndenter--;
  }
#endif

  S_msrHarmony
    harmony =
      msrHarmony::create (
        fCurrentHarmonyInputLineNumber,
        // no harmonyVoiceUplink yet
        
        fCurrentHarmonyRootQuarterTonesPitchKind,
        
        fCurrentHarmonyKind,
        fCurrentHarmonyKindText,

        fCurrentHarmonyInversion,
        
        fCurrentHarmonyBassQuarterTonesPitchKind,
        
        rational (1, 1)); // will be set upon next note handling

  // append pending harmony degrees if any to harmony
  while (fCurrentHarmonyDegreesList.size ()) {
    S_msrHarmonyDegree
      harmonyDegree =
        fCurrentHarmonyDegreesList.front ();

    // set harmony degree harmony uplink
    harmonyDegree->
      setHarmonyDegreeHarmonyUplink (
        harmony);

    // append it to harmony's degrees list
    harmony->
      appendHarmonyDegreeToHarmony (
        harmonyDegree);

    // remove it from the list
    fCurrentHarmonyDegreesList.pop_front ();
  } // while

  // append the harmony to the pending harmonies list
  fPendingHarmoniesList.push_back (harmony);
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_frame& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_frame" <<
      ", line " << inputLineNumber <<
      endl;
  }

/*
      <harmony>
        <root>
          <root-step>C</root-step>
        </root>
        <kind>major</kind>
        <frame>
          <frame-strings>6</frame-strings>
          <frame-frets>4</frame-frets>
          <frame-note>
            <string>6</string>
            <fret>0</fret>
          </frame-note>
          <frame-note>
            <string>5</string>
            <fret>3</fret>
          </frame-note>
          <frame-note>
            <string>4</string>
            <fret>2</fret>
          </frame-note>
          <frame-note>
            <string>3</string>
            <fret>0</fret>
          </frame-note>
          <frame-note>
            <string>2</string>
            <fret>1</fret>
          </frame-note>
          <frame-note>
            <string>1</string>
            <fret>0</fret>
          </frame-note>
        </frame>
      </harmony>
*/

  float degreeAlter = (float)(*elt);

  fCurrentHarmonyDegreeAlterationKind =
    msrAlterationKindFromMusicXMLAlter (
      degreeAlter);
      
  if (fCurrentHarmonyDegreeAlterationKind == k_NoAlteration) {
    stringstream s;

    s <<
      "degree alter '" << degreeAlter << "'"
      "' should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";
      
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fOnGoingFrame = true;
}

void mxmlTree2MsrTranslator::visitStart ( S_frame_strings& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_frame_strings" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentFrameStrings = (int)(*elt);
}

void mxmlTree2MsrTranslator::visitStart ( S_frame_frets& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_frame_frets" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentFrameFrets = (int)(*elt);
}

void mxmlTree2MsrTranslator::visitStart ( S_first_fret& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_first_fret" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

/*
          <first-fret location="right" text="12fr.">12</first-fret>
*/
 
  fCurrentFrameFirstFret = (int)(*elt);

  // the following two are not used yet JMI
  string firstFretLocation = elt->getAttributeValue ("location");
  string firstFretText = elt->getAttributeValue ("text");
}

void mxmlTree2MsrTranslator::visitStart ( S_frame_note& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_frame_note" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentFrameNoteStringNumber = -1;
  fCurrentFrameNoteFretNumber = -1;
  fCurrentFrameNoteFingering = -1;
  fCurrentFrameNoteBarreTypeKind = msrFrameNote::kBarreTypeNone;

  fOnGoingFrameNote = true;
}

void mxmlTree2MsrTranslator::visitStart ( S_barre& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_barre" <<
      ", line " << inputLineNumber <<
      endl;
  }
 
  string barreType = elt->getAttributeValue ("type");
    
  fCurrentFrameNoteBarreTypeKind = msrFrameNote::kBarreTypeNone;
  
  if      (barreType == "start")
    fCurrentFrameNoteBarreTypeKind = msrFrameNote::kBarreTypeStart;
  else if (barreType == "stop")
    fCurrentFrameNoteBarreTypeKind = msrFrameNote::kBarreTypeStop;
  else {
    stringstream s;
    
    s <<
      "barre type \"" << barreType <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }  
}

void mxmlTree2MsrTranslator::visitEnd ( S_frame_note& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_frame_note" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // create the frame note
  S_msrFrameNote
    frameNote =
      msrFrameNote::create (
        inputLineNumber,
        fCurrentFrameNoteStringNumber,
        fCurrentFrameNoteFretNumber,
        fCurrentFrameNoteFingering,
        fCurrentFrameNoteBarreTypeKind);

  // append the frame note to the pending frame notes list
  fPendingFramesNotesList.push_back (frameNote);

  fOnGoingFrameNote = false;
}

void mxmlTree2MsrTranslator::visitEnd ( S_frame& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_frame" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // create the frame
  S_msrFrame
    frame =
      msrFrame::create (
        inputLineNumber,
        fCurrentFrameStrings,
        fCurrentFrameFrets,
        fCurrentFrameFirstFret);
        
  // handling the current pending frame notes if any,
  // so that they get attached to the note right now
  if (fPendingFramesNotesList.size ()) {    
    while (fPendingFramesNotesList.size ()) {
      S_msrFrameNote
        frameNote =
          fPendingFramesNotesList.front ();
  
      // append the frame note to the frame
      frame->
        appendFrameNoteToFrame (frameNote);
  
      // remove it from the list
      fPendingFramesNotesList.pop_front ();
    } // while
  }

  // append the frame to the pending frames list
  fPendingFramesList.push_back (frame);

  fOnGoingFrame = false;
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_figured_bass& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_figured_bass" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string parentheses = elt->getAttributeValue("parentheses");
  
  fCurrentFiguredBassParenthesesKind =
    msrFiguredBass::kFiguredBassParenthesesNo; // default value

  if (parentheses.size ()) {    
    if (parentheses == "yes")
      fCurrentFiguredBassParenthesesKind =
        msrFiguredBass::kFiguredBassParenthesesYes;
      
    else if (parentheses == "no")
     fCurrentFiguredBassParenthesesKind =
        msrFiguredBass::kFiguredBassParenthesesNo;
      
    else {
      stringstream s;
      
      s <<
        "parentheses value " << parentheses <<
        " should be 'yes' or 'no'";
      
      msrMusicXMLError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  fCurrentFiguredBassInputLineNumber   = -1;
  
  fCurrentFigureNumber = -1;

  fCurrentFigurePrefixKind = msrFigure::k_NoFigurePrefix;
  fCurrentFigureSuffixKind = msrFigure::k_NoFigureSuffix;

  fCurrentFiguredBassSoundingWholeNotes = rational (0, 1);
  
  fOnGoingFiguredBass = true;
  fPendingFiguredBass = true;
}

void mxmlTree2MsrTranslator::visitStart ( S_figure& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_figure" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void mxmlTree2MsrTranslator::visitStart ( S_prefix& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_prefix" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string prefix =
    elt->getValue ();

  fCurrentFigurePrefixKind = msrFigure::k_NoFigurePrefix;
    
  if      (prefix == "double-flat")
    fCurrentFigurePrefixKind = msrFigure::kDoubleFlatPrefix;
    
  else if (prefix == "flat")
    fCurrentFigurePrefixKind = msrFigure::kFlatPrefix;
    
  else if (prefix == "flat-flat")
    fCurrentFigurePrefixKind = msrFigure::kFlatFlatPrefix;
    
  else if (prefix == "natural")
    fCurrentFigurePrefixKind = msrFigure::kNaturalPrefix;
    
  else if (prefix == "sharp-sharp")
    fCurrentFigurePrefixKind = msrFigure::kSharpSharpPrefix;
    
  else if (prefix == "sharp")
    fCurrentFigurePrefixKind = msrFigure::kSharpPrefix;
    
  else if (prefix == "double-sharp")
    fCurrentFigurePrefixKind = msrFigure::kDoubleSharpPrefix;
        
  else if (prefix.size ()) {
    stringstream s;
    
    s <<
      "prefix \"" << prefix <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }

}

void mxmlTree2MsrTranslator::visitStart ( S_figure_number& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_figure_number" <<
      ", line " << inputLineNumber <<
      endl;
  }

  fCurrentFigureNumber = (int)(*elt);

  if (fCurrentFigureNumber > 13) {
    stringstream s;

    s <<
      "figure-number '" << fCurrentFigureNumber <<
      "' is greater that 13, that's strange...";

    msrMusicXMLWarning (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      s.str ());
  }
}

void mxmlTree2MsrTranslator::visitStart ( S_suffix& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_suffix" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string suffix =
    elt->getValue ();

  fCurrentFigureSuffixKind = msrFigure::k_NoFigureSuffix;
    
  if      (suffix == "double-flat")
    fCurrentFigureSuffixKind = msrFigure::kDoubleFlatSuffix;
    
  else if (suffix == "flat")
    fCurrentFigureSuffixKind = msrFigure::kFlatSuffix;
    
  else if (suffix == "flat-flat")
    fCurrentFigureSuffixKind = msrFigure::kFlatFlatSuffix;
    
  else if (suffix == "natural")
    fCurrentFigureSuffixKind = msrFigure::kNaturalSuffix;
    
  else if (suffix == "sharp-sharp")
    fCurrentFigureSuffixKind = msrFigure::kSharpSharpSuffix;
    
  else if (suffix == "sharp")
    fCurrentFigureSuffixKind = msrFigure::kSharpSuffix;
    
  else if (suffix == "double-sharp")
    fCurrentFigureSuffixKind = msrFigure::kDoubleSharpSuffix;
        
  else if (suffix == "slash")
    fCurrentFigureSuffixKind = msrFigure::kSlashSuffix;
        
  else if (suffix.size ()) {
    stringstream s;
    
    s <<
      "suffix \"" << suffix <<
      "\" is unknown";
    
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());    
  }
}

void mxmlTree2MsrTranslator::visitEnd ( S_figure& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_figure" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // create the figure
  S_msrFigure
    figure =
      msrFigure::create (
        inputLineNumber,
        fCurrentPart,
        fCurrentFigurePrefixKind,
        fCurrentFigureNumber,
        fCurrentFigureSuffixKind);

  // append it to the pending figures list
  fPendingFiguredBassFigures.push_back (
    figure);
}

void mxmlTree2MsrTranslator::visitEnd ( S_figured_bass& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_figured_bass" <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (! fPendingFiguredBassFigures.size ()) {
    msrMusicXMLWarning (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      "figured-bass has no figures contents");
  }

  fOnGoingFiguredBass = false;
}

//________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart (S_harp_pedals& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_harp_pedals" <<
      ", line " << inputLineNumber <<
      endl;
  }

/*
<!-- 
  The harp-pedals element is used to create harp pedal
  diagrams. The pedal-step and pedal-alter elements use
  the same values as the step and alter elements. For
  easiest reading, the pedal-tuning elements should follow
  standard harp pedal order, with pedal-step values of
  D, C, B, E, F, G, and A.
-->
<!ELEMENT harp-pedals (pedal-tuning)+>
<!ATTLIST harp-pedals
    %print-style-align; 
>
<!ELEMENT pedal-tuning (pedal-step, pedal-alter)>
<!ELEMENT pedal-step (#PCDATA)>
<!ELEMENT pedal-alter (#PCDATA)>

<!-- Harp damping marks -->
<!ELEMENT damp EMPTY>
<!ATTLIST damp
    %print-style-align; 
>
<!ELEMENT damp-all EMPTY>
<!ATTLIST damp-all
    %print-style-align; 
>

        <direction-type>
          <harp-pedals>
            <pedal-tuning>
              <pedal-step>D</pedal-step>
              <pedal-alter>0</pedal-alter>
            </pedal-tuning>
            <pedal-tuning>
              <pedal-step>C</pedal-step>
              <pedal-alter>-1</pedal-alter>
            </pedal-tuning>
            <pedal-tuning>
              <pedal-step>B</pedal-step>
              <pedal-alter>-1</pedal-alter>
            </pedal-tuning>
            <pedal-tuning>
              <pedal-step>E</pedal-step>
              <pedal-alter>0</pedal-alter>
            </pedal-tuning>
            <pedal-tuning>
              <pedal-step>F</pedal-step>
              <pedal-alter>0</pedal-alter>
            </pedal-tuning>
            <pedal-tuning>
              <pedal-step>G</pedal-step>
              <pedal-alter>1</pedal-alter>
            </pedal-tuning>
            <pedal-tuning>
              <pedal-step>A</pedal-step>
              <pedal-alter>-1</pedal-alter>
            </pedal-tuning>
          </harp-pedals>
        </direction-type>

*/

  // create the harp pedals tuning
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarpPedalsTuning) {
    fLogOutputStream <<
      "Creating harp pedals tuning:" <<
      endl;
  }
#endif
    
  fCurrentHarpPedalsTuning =
    msrHarpPedalsTuning::create (
      inputLineNumber);


  // add it to the current part
  fCurrentPart->
    appendHarpPedalsTuningToPart (
      fCurrentHarpPedalsTuning);
}

void mxmlTree2MsrTranslator::visitStart (S_pedal_tuning& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_pedal_tuning" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}
    
void mxmlTree2MsrTranslator::visitStart (S_pedal_step& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_pedal_step" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string tuningStep = elt->getValue();

  checkStep (
    inputLineNumber,
    tuningStep);

  fCurrentHarpPedalDiatonicPitchKind =
    msrDiatonicPitchKindFromString (
      tuningStep [0]);
}

void mxmlTree2MsrTranslator::visitStart (S_pedal_alter& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_pedal_alter" <<
      ", line " << inputLineNumber <<
      endl;
  }

  float pedalAlter = (float)(*elt);

  fCurrentHarpPedalAlterationKind =
    msrAlterationKindFromMusicXMLAlter (
      pedalAlter);
      
  if (fCurrentHarpPedalAlterationKind == k_NoAlteration) {
    stringstream s;

    s <<
      "pedal alter '" << pedalAlter << "'"
      "' should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";
      
    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxmlTree2MsrTranslator::visitEnd (S_pedal_tuning& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_pedal_tuning" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // create a harp pedals tuning
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceStaffTuning) {
    fLogOutputStream <<
      "Creating harp pedal tuning:" <<
      endl;

    gIndenter++;

    const int fieldWidth = 31;

    fLogOutputStream << left <<
      setw (fieldWidth) <<
      "fCurrentHarpPedalDiatonicPitch" << " = " <<
      msrDiatonicPitchKindAsString (
        gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
        fCurrentHarpPedalDiatonicPitchKind) <<
      endl <<
      setw (fieldWidth) <<
      "fCurrentHarpPedalAlteration" << " = " <<
      msrAlterationKindAsString (
        fCurrentHarpPedalAlterationKind) <<
      endl;

    gIndenter--;
  }
#endif
    
  fCurrentHarpPedalsTuning->
    addPedalTuning (
      inputLineNumber,
      fCurrentHarpPedalDiatonicPitchKind,
      fCurrentHarpPedalAlterationKind);
}

void mxmlTree2MsrTranslator::visitStart( S_damp& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_damp" <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (fOnGoingDirectionType) {      
    // fetch current voice
    S_msrVoice
      currentVoice =
        fetchVoiceFromPart (
          inputLineNumber,
          fCurrentMusicXMLStaffNumber,
          fCurrentMusicXMLVoiceNumber);
  
    // create the damp
    S_msrDamp
      damp =
        msrDamp::create (
          inputLineNumber);

    // append it to the pending damps list
    fPendingDamps.push_back (damp);
  }
}

void mxmlTree2MsrTranslator::visitStart( S_damp_all& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_damp_all" <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (fOnGoingDirectionType) {      
    // fetch current voice
    S_msrVoice
      currentVoice =
        fetchVoiceFromPart (
          inputLineNumber,
          fCurrentMusicXMLStaffNumber,
          fCurrentMusicXMLVoiceNumber);
  
    // create the damp all
    S_msrDampAll
      dampAll =
        msrDampAll::create (
          inputLineNumber);

    // append it to the pending damp alls list
    fPendingDampAlls.push_back (dampAll);
  }
}

//________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart (S_capo& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_capo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentStaffDetailsCapo = (int)(*elt);
}

void mxmlTree2MsrTranslator::visitStart (S_staff_size& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_staff_size" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentStaffDetailsStaffSize = (int)(*elt);
  // JMI not used
}

void mxmlTree2MsrTranslator::visitEnd (S_staff_details& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_staff_details" <<
      ", line " << inputLineNumber <<
      endl;
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceStaves) {
    const int fieldWidth = 29;

    fLogOutputStream << left <<
      setw (fieldWidth) <<
      "fCurrentStaffLinesNumber" << " = " <<
      fCurrentStaffLinesNumber <<
      endl <<
      setw (fieldWidth) <<
      "fStaffDetailsStaffNumber" << " = " <<
      fStaffDetailsStaffNumber <<
      endl <<
      setw (fieldWidth) <<
      "fCurrentStaffDetailsCapo" << " = " <<
      fCurrentStaffDetailsCapo <<
      endl <<
      setw (fieldWidth) <<
      "fCurrentStaffDetailsStaffSize" << " = " <<
      fCurrentStaffDetailsStaffSize <<
      endl;
  }
#endif

  gIndenter--;

  // set staff details line number
  fCurrentStaffDetails->
    setStaffLinesNumber (
      fCurrentStaffLinesNumber);
  
  // append staff details in part or staff
  if (fStaffDetailsStaffNumber == 0)
    fCurrentPart->
      appendStaffDetailsToPart (
        fCurrentStaffDetails);
    
  else {
    S_msrStaff
      staff =
        fetchStaffFromCurrentPart (
          inputLineNumber,
          fStaffDetailsStaffNumber);
    
    staff->
      appendStaffDetailsToStaff (
        fCurrentStaffDetails);
  }

  // forget about this staff details
  fCurrentStaffDetails = nullptr;
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart (S_scordatura& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_scordatura" <<
      ", line " << inputLineNumber <<
      endl;
  }

/*
  <direction>
    <direction-type>
      <scordatura>
        <accord string="3">
          <tuning-step>C</tuning-step>
          <tuning-octave>3</tuning-octave>
          </accord>
        <accord string="2">
          <tuning-step>G</tuning-step>
          <tuning-octave>5</tuning-octave>
          </accord>
        <accord string="1">
          <tuning-step>E</tuning-step><
          tuning-octave>5</tuning-octave>
          </accord>
      </scordatura>
    </direction-type>
  </direction>
*/

  fCurrentScordatura =
    msrScordatura::create (
      inputLineNumber);
}

void mxmlTree2MsrTranslator::visitStart (S_accord& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_accord" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

/*
        <accord string="3">
          <tuning-step>C</tuning-step>
          <tuning-octave>3</tuning-octave>
          </accord>
*/

  fCurrentStringTuningNumber = elt->getAttributeIntValue ("string", 0);

  fCurrentStringTuningDiatonicPitchKind = k_NoDiatonicPitch;
  fCurrentStringTuningAlterationKind = kNatural; // default value
  fCurrentStringTuningOctave = -1;

  fOnGoingAccord = true;
}

void mxmlTree2MsrTranslator::visitEnd (S_accord& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_accord" <<
      ", line " << inputLineNumber <<
      endl;
  }

  S_msrStringTuning
    stringTuning =
      msrStringTuning::create (
        inputLineNumber,
        fCurrentStringTuningNumber,
        fCurrentStringTuningDiatonicPitchKind,
        fCurrentStringTuningAlterationKind,
        fCurrentStringTuningOctave);

  fCurrentScordatura->
    addStringTuningToScordatura (
      stringTuning);
}

void mxmlTree2MsrTranslator::visitEnd (S_scordatura& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_scordatura" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // append the current scordatura to the pending scordatura list
  fPendingScordaturas.push_back (fCurrentScordatura);

  // forget about this scordatura
  fCurrentScordatura = nullptr;
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_sound& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_sound" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void mxmlTree2MsrTranslator::visitEnd ( S_sound& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_sound" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_instrument_sound& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_instrument_sound" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_virtual_instrument& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_virtual_instrument" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_midi_device& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_midi_device" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

/*
      <midi-device>SmartMusic SoftSynth</midi-device>
*/
}

//______________________________________________________________________________
void mxmlTree2MsrTranslator::visitStart ( S_midi_instrument& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_midi_instrument" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

/*
      <midi-instrument id="P2-I2">
        <midi-channel>2</midi-channel>
        <midi-program>70</midi-program>
        <volume>80</volume>
        <pan>4</pan>
      </midi-instrument>
*/
}


} // namespace
