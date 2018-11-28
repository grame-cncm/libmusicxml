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
#include <algorithm>    // for_each

#include "conversions.h"

#include "msr.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "musicXMLOptions.h"
#include "msrOptions.h"
#include "brailleOptions.h"

#include "xml2brlOptionsHandling.h"

#include "msr2BsrTranslator.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
msr2BsrTranslator::msr2BsrTranslator (
  indentedOstream& ios,
  S_msrScore       mScore)
    : fLogOutputStream (ios)
{
  // the MSR score we're visiting
  fVisitedMsrScore = mScore;

  // pages & lines
  fCurrentPrintPageNumber = 1;
  fCurrentPrintLineNumber = 1;

/*
  // identification
  fOnGoingIdentification = false;

  // header
  fWorkNumberKnown       = false;
  fWorkTitleKnown        = false;
  fMovementNumberKnown   = false;
  fMovementTitleKnown    = false;
   
  // staves
  fOnGoingStaff = false;

  // voices
  fOnGoingHarmonyVoice     = false;
  fOnGoingFiguredBassVoice = false;

  // repeats
  fOnGoingRepeat = false;

  // measures
  fMeasuresCounter = 0;
    
  // notes
  fOnGoingNote = false;

  // double tremolos
  fOnGoingDoubleTremolo = false;

  // grace notes
  fOnGoingGraceNotesGroup = false;
  
  // chords
  fOnGoingChord = false;
  
  // stanzas
  fOnGoingStanza = false;

  // syllables
  fOnGoingSyllableExtend = false;
  */
};
  
msr2BsrTranslator::~msr2BsrTranslator ()
{}

//________________________________________________________________________
void msr2BsrTranslator::buildBsrScoreFromMsrScore ()
{
  if (fVisitedMsrScore) {    
    // create a msrScore browser
    msrBrowser<msrScore> browser (this);

    // browse the score with the browser
    browser.browse (*fVisitedMsrScore);
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrPageBreak& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
  
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrPageBreak" <<
      ", line " << inputLineNumber <<
      endl;
  }

  fCurrentPage =
    bsrPage::create (
      inputLineNumber,
      ++fCurrentPrintPageNumber,
      gBrailleOptions->fLinesPerPage);
    
  fBsrScore->
    appendPageToScore (fCurrentPage);
}

void msr2BsrTranslator::visitEnd (S_msrPageBreak& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrPageBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrLineBreak& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
  
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrLineBreak" <<
      ", line " << inputLineNumber <<
      endl;
  }

  fCurrentLine =
    bsrLine::create (
      inputLineNumber,
      ++fCurrentPrintLineNumber,
      gBrailleOptions->fCellsPerLine);
    
  fCurrentPage->
    appendLineToPage (fCurrentLine);
}

void msr2BsrTranslator::visitEnd (S_msrLineBreak& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrLineBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrScore& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrScore" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // create the BSR score
  fBsrScore =
    bsrScore::create (
      NO_INPUT_LINE_NUMBER,
      fVisitedMsrScore);

  // append a first transcription note to it
  S_bsrTranscriptionNotesElement
    transcriptionNotesElement =
      bsrTranscriptionNotesElement::create (
        inputLineNumber,
        "This is a first transcription note");

  fBsrScore->
    getTranscriptionNotes ()->
      appendNoteElementToTranscriptionNotes (
        transcriptionNotesElement);
      
  // create the first page
  fCurrentPage =
    bsrPage::create (
      inputLineNumber,
      fCurrentPrintPageNumber,
      gBrailleOptions->fLinesPerPage);
    
  // append it to the score
  fBsrScore->
    appendPageToScore (fCurrentPage);

  // create the fisrt line
  fCurrentLine =
    bsrLine::create (
      inputLineNumber,
      fCurrentPrintLineNumber,
      gBrailleOptions->fCellsPerLine);

  // append the fisrt line to the first page
  fCurrentPage->
    appendLineToPage (fCurrentLine);

  /*
  // append a number to the fisrt line
  S_bsrNumber
    number =
      bsrNumber::create (
        inputLineNumber,
        7,
        bsrNumber::kNumberSignIsNeededYes);
    
  fCurrentLine->
    appendNumberToLine (number);
    
  // append a note to the fisrt line
  S_bsrNote
    note =
      bsrNote::create (
        inputLineNumber,
        bsrNote::kBsrCEighthKind,
        bsrNote::kBrlOctave4Kind,
        bsrNote::kNoteOctaveIsNeededYes);
  
  fCurrentLine->
    appendNoteToLine (note);
    */
    
/*
  // create an empty clone of fVisitedMsrScore for use by the BSR score
  // not sharing the visitiged MSR score allows cleaner data handling
  // and optimisations of the BSR data
  fCurrentMsrScoreClone =
    fVisitedMsrScore->
      createScoreNewbornClone ();

  // fetch score header
  fBsrScoreHeader =
    fBsrScore-> getHeader();

  // is there a rights option?
  if (gLilypondOptions->fRights.size ()) {
    // define rights
    
    fBsrScoreHeader->
      addRights (
        inputLineNumber,
        gLilypondOptions->fRights);
  }

  // is there a composer option?
  if (gLilypondOptions->fComposer.size ()) {
    // define composer
    
    fBsrScoreHeader->
      addComposer (
        inputLineNumber,
        gLilypondOptions->fComposer);
  }

  // is there an arranger option?
  if (gLilypondOptions->fArranger.size ()) {
    // define arranger
    
    fBsrScoreHeader->
      addArranger (
        inputLineNumber,
        gLilypondOptions->fArranger);
  }

  // is there a poet option?
  if (gLilypondOptions->fPoet.size ()) {
    // define poet
    
    fBsrScoreHeader->
      addPoet (
        inputLineNumber,
        gLilypondOptions->fPoet);
  }

  // is there a lyricist option?
  if (gLilypondOptions->fLyricist.size ()) {
    // define lyricist
    
    fBsrScoreHeader->
      addLyricist (
        inputLineNumber,
        gLilypondOptions->fLyricist);
  }

  // is there a software option?
  if (gLilypondOptions->fSoftware.size ()) {
    // define software
    
    fBsrScoreHeader->
      addSoftware (
        inputLineNumber,
        gLilypondOptions->fSoftware);
  }

  // is the Scheme function 'whiteNoteHeads' to be generated?
  if (gLilypondOptions->fWhiteNoteHeads) {
    fBsrScore->
      // this score needs the 'whiteNoteHeads' Scheme function
      setWhiteNoteHeadsIsNeeded ();
  }

  // is Jianpu notation to be generated?
  if (gLilypondOptions->fJianpu) {
    fBsrScore->
      // this score needs the 'jianpu file include' Scheme function
      setJianpuFileIncludeIsNeeded ();
  }
    */
}

void msr2BsrTranslator::visitEnd (S_msrScore& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrScore" <<
      ", line " << inputLineNumber <<
      endl;
  }

/*
  if (fWorkTitleKnown && fMovementTitleKnown) {
    string
      workTitle =
        fCurrentIdentification->
          getWorkTitle ()->
            getVariableValue (),
      movementTitle =
        fCurrentIdentification->
          getMovementTitle ()->
            getVariableValue ();
    
    if (
      workTitle.size () == 0
        &&
      movementTitle.size () > 0) {
      // use the movement title as the work title
      fCurrentIdentification->
        setWorkTitle (
          inputLineNumber, movementTitle);

      fBsrScoreHeader->
        setWorkTitle (
          inputLineNumber, movementTitle);

      // forget the movement title
      fCurrentIdentification->
        setMovementTitle (
          inputLineNumber, "");

      fBsrScoreHeader->
        setMovementTitle (
          inputLineNumber, "");
    }
  }

  else if (! fWorkTitleKnown && fMovementTitleKnown) {
    string
      movementTitle =
        fCurrentIdentification->
          getMovementTitle ()->
            getVariableValue ();
            
    // use the movement title as the work title
    fCurrentIdentification->
      setWorkTitle (
        inputLineNumber, movementTitle);

    fBsrScoreHeader->
      setWorkTitle (
        inputLineNumber, movementTitle);

    // forget the movement title
    fCurrentIdentification->
      setMovementTitle (
        inputLineNumber, "");

    fBsrScoreHeader->
      setMovementTitle (
        inputLineNumber, "");
  }

  // set ident and short indent if needed
  setPaperIndentsIfNeeded (
    elt->getPageGeometry ());
    */ 
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrPartGroup& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrPartGroup " <<
      elt->getPartGroupCombinedName () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentPartGroup = elt;
}

void msr2BsrTranslator::visitEnd (S_msrPartGroup& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrPartGroup " <<
      elt->getPartGroupCombinedName () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrPart& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  string
    partCombinedName =
      elt->getPartCombinedName ();
      
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrPart " <<
      partCombinedName <<
      ", line " << inputLineNumber <<
      endl;
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceParts) {
    fLogOutputStream <<
      endl <<
      "<!--=== part \"" << partCombinedName << "\"" <<
      ", line " << inputLineNumber << " ===-->" <<
      endl;
  }
#endif

  gIndenter++;

  fCurrentPart = elt;
}

void msr2BsrTranslator::visitEnd (S_msrPart& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  gIndenter--;

  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrPart " <<
      elt->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrStaffTuning& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrStaffTuning" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
  
  // create a staff tuning clone
  fCurrentStaffTuning = elt;
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrStaffDetails& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrStaffDetails" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentStaffTuning = nullptr;
}

void msr2BsrTranslator::visitEnd (S_msrStaffDetails& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrStaffDetails" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrStaff& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrStaff \"" <<
      elt->getStaffName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  gIndenter++;

  fCurrentStaff = elt;
}

void msr2BsrTranslator::visitEnd (S_msrStaff& elt)
{
  gIndenter--;

  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting S_msrStaff \"" <<
      elt->getStaffName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrVoice& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrVoice \"" <<
      elt->getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }

  fCurrentVoice = elt;
  
  gIndenter++;
}

void msr2BsrTranslator::visitEnd (S_msrVoice& elt)
{
  gIndenter--;

  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrVoice \"" <<
      elt->getVoiceName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrVoiceStaffChange& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrVoiceStaffChange '" <<
      elt->asString () << "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrMeasure& elt)
{    
  int
    inputLineNumber =
      elt->getInputLineNumber ();

  string
    measureNumber =
      elt->getMeasureNumber ();

  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrMeasure '" <<
      measureNumber <<
      "'" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // create a 1-spaces
  S_bsrSpaces
    spaces =
      bsrSpaces::create (
        inputLineNumber, 1);

  // append it to the current line
  fCurrentLine->
    appendSpacesToLine (spaces);
  
  // create a measure
  fCurrentMeasure =
    bsrMeasure::create (inputLineNumber);

  // append it to the current line
  fCurrentLine->
    appendMeasureToLine (fCurrentMeasure);
}

void msr2BsrTranslator::visitEnd (S_msrMeasure& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrMeasure '" <<
      elt->getMeasureNumber () <<
      "'" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string
    measureNumber =
      elt->getMeasureNumber ();
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrClef& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
  
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrClef" <<
      ", line " << inputLineNumber <<
      endl;
  }

  bsrClef::bsrClefKind clefKind = bsrClef::kClefNone;

/* JMI
        kClefModifiedBassForRightHandPartKind,
        kClefModifiedTrebleForLeftHandPartKind };
        */
        
  switch (elt->getClefKind ()) {
    case msrClef::k_NoClef:
      clefKind = bsrClef::kClefNone;
      break;
    case msrClef::kTrebleClef:
      clefKind = bsrClef::kClefGTrebleKind;
      break;
    case msrClef::kSopranoClef:
      clefKind = bsrClef::kClefNone;
      break;
    case msrClef::kMezzoSopranoClef:
      break;
    case msrClef::kAltoClef:
      clefKind = bsrClef::kClefNone;
      break;
    case msrClef::kTenorClef:
      clefKind = bsrClef::kClefCTenorKind;
      break;
    case msrClef::kBaritoneClef:
      clefKind = bsrClef::kClefFBaritoneKind;
      break;
    case msrClef::kBassClef:
      clefKind = bsrClef::kClefFBassKind;
      break;
    case msrClef::kTrebleLine1Clef:
      clefKind = bsrClef::kClefGSopranoKind;
      break;
    case msrClef::kTrebleMinus15Clef:
      break;
    case msrClef::kTrebleMinus8Clef:
      clefKind = bsrClef::kClefGOttavaBassaKind; // JMI permute???
      break;
    case msrClef::kTreblePlus8Clef:
      clefKind = bsrClef::kClefGOttavaAltaKind; // JMI permute???
      break;
    case msrClef::kTreblePlus15Clef:
      break;
    case msrClef::kBassMinus15Clef:
      break;
    case msrClef::kBassMinus8Clef:
      break;
    case msrClef::kBassPlus8Clef:
      break;
    case msrClef::kBassPlus15Clef:
      break;
    case msrClef::kVarbaritoneClef:
      break;
    case msrClef::kTablature4Clef:
      break;
    case msrClef::kTablature5Clef:
      break;
    case msrClef::kTablature6Clef:
      break;
    case msrClef::kTablature7Clef:
      break;
    case msrClef::kPercussionClef:
      break;
    case msrClef::kJianpuClef:
      break;
  } // switch

  if (clefKind != bsrClef::kClefNone) {
    // message
    // transcription note
    
    S_bsrClef
      clef =
        bsrClef::create (
          inputLineNumber, clefKind);
      
    fCurrentMeasure->
      appendClefToMeasure (clef);
  }
}

void msr2BsrTranslator::visitEnd (S_msrClef& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrClef" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrKey& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
  
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrKey" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // get MSR key attributes
  
  msrKey::msrKeyKind
    mKeyKind = elt->getKeyKind ();

  msrQuarterTonesPitchKind
    mTonicQuarterTonesPitchKind =
      elt->getKeyTonicQuarterTonesPitchKind ();
      
  msrDiatonicPitchKind
    mDiatonicPitchKind =
      diatonicPitchKindFromQuarterTonesPitchKind (
        inputLineNumber,
        mTonicQuarterTonesPitchKind);
       
  msrKey::msrKeyModeKind
    mKeyModeKind =
      elt->getKeyModeKind ();

  // let's go
  
  bsrKey::bsrKeyKind bKeyKind;
  int                numberOfAlterations;

  switch (mKeyKind) {
    case msrKey::kTraditionalKind:
    {
      // traditional keys
      switch (mDiatonicPitchKind) {
        case k_NoDiatonicPitch:
          break;
    
        case kA:
          switch (mKeyModeKind) {
            case msrKey::kMajorMode:
              bKeyKind = bsrKey::kKeyKindSharps;
              numberOfAlterations = 3;
              break;
            case msrKey::kMinorMode:
              bKeyKind = bsrKey::kKeyKindNaturals;
              numberOfAlterations = 0;
              break;
            case msrKey::kIonianMode:
              break;
            case msrKey::kDorianMode:
              break;
            case msrKey::kPhrygianMode:
              break;
            case msrKey::kLydianMode:
              break;
            case msrKey::kMixolydianMode:
              break;
            case msrKey::kAeolianMode:
              break;
            case msrKey::kLocrianMode:
              break;
          } // switch
          break;
          
        case kB:
          switch (mKeyModeKind) {
            case msrKey::kMajorMode:
              bKeyKind = bsrKey::kKeyKindSharps;
              numberOfAlterations = 5;
              break;
            case msrKey::kMinorMode:
              bKeyKind = bsrKey::kKeyKindSharps;
              numberOfAlterations = 2;
              break;
            case msrKey::kIonianMode:
              break;
            case msrKey::kDorianMode:
              break;
            case msrKey::kPhrygianMode:
              break;
            case msrKey::kLydianMode:
              break;
            case msrKey::kMixolydianMode:
              break;
            case msrKey::kAeolianMode:
              break;
            case msrKey::kLocrianMode:
              break;
          } // switch
          break;
          
        case kC:
          switch (mKeyModeKind) {
            case msrKey::kMajorMode:
              bKeyKind = bsrKey::kKeyKindNaturals;
              numberOfAlterations = 0;
              break;
            case msrKey::kMinorMode:
              bKeyKind = bsrKey::kKeyKindFlats;
              numberOfAlterations = 3;
              break;
            case msrKey::kIonianMode:
              break;
            case msrKey::kDorianMode:
              break;
            case msrKey::kPhrygianMode:
              break;
            case msrKey::kLydianMode:
              break;
            case msrKey::kMixolydianMode:
              break;
            case msrKey::kAeolianMode:
              break;
            case msrKey::kLocrianMode:
              break;
          } // switch
          break;
          
        case kD:
          switch (mKeyModeKind) {
            case msrKey::kMajorMode:
              bKeyKind = bsrKey::kKeyKindSharps;
              numberOfAlterations = 2;
              break;
            case msrKey::kMinorMode:
              bKeyKind = bsrKey::kKeyKindFlats;
              numberOfAlterations = 1;
              break;
            case msrKey::kIonianMode:
              break;
            case msrKey::kDorianMode:
              break;
            case msrKey::kPhrygianMode:
              break;
            case msrKey::kLydianMode:
              break;
            case msrKey::kMixolydianMode:
              break;
            case msrKey::kAeolianMode:
              break;
            case msrKey::kLocrianMode:
              break;
          } // switch
          break;
          
        case kE:
          switch (mKeyModeKind) {
            case msrKey::kMajorMode:
              bKeyKind = bsrKey::kKeyKindSharps;
              numberOfAlterations = 4;
              break;
            case msrKey::kMinorMode:
              bKeyKind = bsrKey::kKeyKindSharps;
              numberOfAlterations = 1;
              break;
            case msrKey::kIonianMode:
              break;
            case msrKey::kDorianMode:
              break;
            case msrKey::kPhrygianMode:
              break;
            case msrKey::kLydianMode:
              break;
            case msrKey::kMixolydianMode:
              break;
            case msrKey::kAeolianMode:
              break;
            case msrKey::kLocrianMode:
              break;
          } // switch
          break;
          
        case kF:
          switch (mKeyModeKind) {
            case msrKey::kMajorMode:
              bKeyKind = bsrKey::kKeyKindFlats;
              numberOfAlterations = 1;
              break;
            case msrKey::kMinorMode:
              bKeyKind = bsrKey::kKeyKindFlats;
              numberOfAlterations = 4;
              break;
            case msrKey::kIonianMode:
              break;
            case msrKey::kDorianMode:
              break;
            case msrKey::kPhrygianMode:
              break;
            case msrKey::kLydianMode:
              break;
            case msrKey::kMixolydianMode:
              break;
            case msrKey::kAeolianMode:
              break;
            case msrKey::kLocrianMode:
              break;
          } // switch
          break;
          
        case kG:
          switch (mKeyModeKind) {
            case msrKey::kMajorMode:
              bKeyKind = bsrKey::kKeyKindSharps;
              numberOfAlterations = 1;
              break;
            case msrKey::kMinorMode:
              bKeyKind = bsrKey::kKeyKindFlats;
              numberOfAlterations = 2;
              break;
            case msrKey::kIonianMode:
              break;
            case msrKey::kDorianMode:
              break;
            case msrKey::kPhrygianMode:
              break;
            case msrKey::kLydianMode:
              break;
            case msrKey::kMixolydianMode:
              break;
            case msrKey::kAeolianMode:
              break;
            case msrKey::kLocrianMode:
              break;
          } // switch
          break;
      } // switch
    
        // JMI ??? int                   getKeyCancel () const
  
        /* JMI
        switch (semiTonesPitchKind) {
          case k_NoSemiTonesPitch_STP:
            result = "k_NoSemiTonesPitch_STP";
            break;
            
          case kC_TripleFlat_STP:
            result = "C_TripleFlat_STP";
            break;
          case kC_DoubleFlat_STP:
            result = "C_DoubleFlat_STP";
            break;
          case kC_Flat_STP:
            result = "C_Flat_STP";
            break;
          case kC_Natural_STP:
            result = "C_Natural_STP";
            break;
          case kC_Sharp_STP:
            result = "C_Sharp_STP";
            break;
          case kC_DoubleSharp_STP:
            result = "C_DoubleSharp_STP";
            break;
          case kC_TripleSharp_STP:
            result = "C_TripleSharp_STP";
            break;
      
          case kD_TripleFlat_STP:
            result = "D_TripleFlat_STP";
            break;
          case kD_DoubleFlat_STP:
            result = "D_DoubleFlat_STP";
            break;
          case kD_Flat_STP:
            result = "D_Flat_STP";
            break;
          case kD_Natural_STP:
            result = "D_Natural_STP";
            break;      
          case kD_Sharp_STP:
            result = "D_Sharp_STP";
            break;
          case kD_DoubleSharp_STP:
            result = "D_DoubleSharp_STP";
            break;
          case kD_TripleSharp_STP:
            result = "D_TripleSharp_STP";
            break;
            
          case kE_TripleFlat_STP:
            result = "E_TripleFlat_STP";
            break;
          case kE_DoubleFlat_STP:
            result = "E_DoubleFlat_STP";
            break;
          case kE_Flat_STP:
            result = "E_Flat_STP";
            break;
          case kE_Natural_STP:
            result = "E_Natural_STP";
            break;
          case kE_Sharp_STP:
            result = "E_Sharp_STP";
            break;
          case kE_DoubleSharp_STP:
            result = "E_DoubleSharp_STP";
            break;
          case kE_TripleSharp_STP:
            result = "E_TripleSharp_STP";
            break;
            
          case kF_TripleFlat_STP:
            result = "F_TripleFlat_STP";
            break;
          case kF_DoubleFlat_STP:
            result = "F_DoubleFlat_STP";
            break;
          case kF_Flat_STP:
            result = "F_Flat_STP";
            break;
          case kF_Natural_STP:
            result = "F_Natural_STP";
            break;
          case kF_Sharp_STP:
            result = "F_Sharp_STP";
            break;      
          case kF_DoubleSharp_STP:
            result = "F_DoubleSharp_STP";
            break;
          case kF_TripleSharp_STP:
            result = "F_TripleSharp_STP";
            break;
            
          case kG_TripleFlat_STP:
            result = "G_TripleFlat_STP";
            break;
          case kG_DoubleFlat_STP:
            result = "G_DoubleFlat_STP";
            break;
          case kG_Flat_STP:
            result = "G_Flat_STP";
            break;
          case kG_Natural_STP:
            result = "G_Natural_STP";
            break;
          case kG_Sharp_STP:
            result = "G_Sharp_STP";
            break;
          case kG_DoubleSharp_STP:
            result = "G_DoubleSharp_STP";
            break;
          case kG_TripleSharp_STP:
            result = "G_TripleSharp_STP";
            break;
      
          case kA_TripleFlat_STP:
            result = "A_TripleFlat_STP";
            break;
          case kA_DoubleFlat_STP:
            result = "A_DoubleFlat_STP";
            break;
          case kA_Flat_STP:
            result = "A_Flat_STP";
            break;
          case kA_Natural_STP:
            result = "A_Natural_STP";
            break;
          case kA_Sharp_STP:
            result = "A_Sharp_STP";
            break;
          case kA_DoubleSharp_STP:
            result = "A_DoubleSharp_STP";
            break;
          case kA_TripleSharp_STP:
            result = "A_TripleSharp_STP";
            break;
      
          case kB_TripleFlat_STP:
            result = "B_TripleFlat_STP";
            break;
          case kB_DoubleFlat_STP:
            result = "B_DoubleFlat_STP";
            break;      
          case kB_Flat_STP:
            result = "B_Flat_STP";
            break;
          case kB_Natural_STP:
            result = "B_Natural_STP";
            break;
          case kB_Sharp_STP:
            result = "B_Sharp_STP";
            break;
          case kB_DoubleSharp_STP:
            result = "B_DoubleSharp_STP";
            break;
          case kB_TripleSharp_STP:
            result = "B_TripleSharp_STP";
            break;
        } // switch
        */
      }
      break;
      
    case msrKey::kHumdrumScotKind:
      // JMI
      break;
  } // switch

  S_bsrKey
    key =
      bsrKey::create (
        inputLineNumber,
        bKeyKind,
        numberOfAlterations);
      
  fCurrentMeasure->
    appendKeyToMeasure (key);
}

void msr2BsrTranslator::visitEnd (S_msrKey& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrKey" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrTime& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
  
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrTime" <<
      ", line " << inputLineNumber <<
      endl;
  }

  S_bsrTime
    time =
      bsrTime::create (inputLineNumber);
    
  fCurrentMeasure->
    appendTimeToMeasure (time);
}

void msr2BsrTranslator::visitEnd (S_msrTime& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrTime" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrNote& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrNote '" <<
      elt->asString () <<
      "'" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // get MSR note attributes
  
  msrQuarterTonesPitchKind
    noteQuarterTonesPitchKind =
      elt->getNoteQuarterTonesPitchKind ();

  msrDiatonicPitchKind
    noteDiatonicPitchKind =
      diatonicPitchKindFromQuarterTonesPitchKind (
        inputLineNumber,
        noteQuarterTonesPitchKind);

  msrNote::msrNoteAccidentalKind
    noteAccidentalKind =
      elt->getNoteAccidentalKind ();

  int noteDotsNumber = elt->getNoteDotsNumber ();

  msrDurationKind
    noteGraphicDurationKind =
      elt->getNoteGraphicDurationKind ();

  int noteOctave = elt->getNoteOctave ();

  // let's go
  
  bsrNote::bsrNoteOctaveKind noteOctaveKind;
  // middle C starts octave 4, as in MusicXML
  switch (noteOctave) {
    case 1: noteOctaveKind = bsrNote::kNoteOctave1Kind; break;
    case 2: noteOctaveKind = bsrNote::kNoteOctave2Kind; break;
    case 3: noteOctaveKind = bsrNote::kNoteOctave3Kind; break;
    case 4: noteOctaveKind = bsrNote::kNoteOctave4Kind; break;
    case 5: noteOctaveKind = bsrNote::kNoteOctave5Kind; break;
    case 6: noteOctaveKind = bsrNote::kNoteOctave6Kind; break;
    case 7: noteOctaveKind = bsrNote::kNoteOctave7Kind; break;
    default:
      // kNoteOctaveBelow1Kind and kNoteOctaveAbove7Kind
      // cannot occur in MusicXML
      ;
  } // switch
  
  bsrNote::bsrNoteValueKind noteValueKind;

  if (elt->getNoteIsARest ()) {
    switch (noteGraphicDurationKind) {
      case k_NoDuration:
        break;
  
      case k1024th:
        break;
      case k512th:
        break;
      case k256th:
        noteValueKind = bsrNote::kNoteRest256thKind;
        break;
      case k128th:
        noteValueKind = bsrNote::kNoteRest128thKind;
        break;
      case k64th:
        noteValueKind = bsrNote::kNoteRest64thKind;
        break;
      case k32nd:
        noteValueKind = bsrNote::kNoteRest32ndKind;
        break;
      case k16th:
        noteValueKind = bsrNote::kNoteRest16thKind;
        break;
      case kEighth:
        noteValueKind = bsrNote::kNoteRest8thKind;
        break;
      case kQuarter:
        noteValueKind = bsrNote::kNoteRestQuarterKind;
        break;
      case kHalf:
        noteValueKind = bsrNote::kNoteRestHalfKind;
        break;
      case kWhole:
        noteValueKind = bsrNote::kNoteRestWholeKind;
        break;
      case kBreve:
        noteValueKind = bsrNote::kNoteRestBreveKind;
        break;
      case kLong:
        break;
      case kMaxima:
        break;
    } // switch
  }

  else {
    switch (noteDiatonicPitchKind) {
      case k_NoDiatonicPitch:
        break;
  
      case kA:
        switch (noteGraphicDurationKind) {
          case k_NoDuration:
            break;
      
          case k1024th:
            break;
          case k512th:
            break;
          case k256th:
            noteValueKind = bsrNote::kNoteA256thKind;
            break;
          case k128th:
            noteValueKind = bsrNote::kNoteA128thKind;
            break;
          case k64th:
            noteValueKind = bsrNote::kNoteA64thKind;
            break;
          case k32nd:
            noteValueKind = bsrNote::kNoteA32ndKind;
            break;
          case k16th:
            noteValueKind = bsrNote::kNoteA16thKind;
            break;
          case kEighth:
            noteValueKind = bsrNote::kNoteA8thKind;
            break;
          case kQuarter:
            noteValueKind = bsrNote::kNoteAQuarterKind;
            break;
          case kHalf:
            noteValueKind = bsrNote::kNoteAHalfKind;
            break;
          case kWhole:
            noteValueKind = bsrNote::kNoteAWholeKind;
            break;
          case kBreve:
            noteValueKind = bsrNote::kNoteABreveKind;
            break;
          case kLong:
            break;
          case kMaxima:
            break;
        } // switch
        break;
        
      case kB:
        switch (noteGraphicDurationKind) {
          case k_NoDuration:
            break;
      
          case k1024th:
            break;
          case k512th:
            break;
          case k256th:
            noteValueKind = bsrNote::kNoteB256thKind;
            break;
          case k128th:
            noteValueKind = bsrNote::kNoteB128thKind;
            break;
          case k64th:
            noteValueKind = bsrNote::kNoteB64thKind;
            break;
          case k32nd:
            noteValueKind = bsrNote::kNoteB32ndKind;
            break;
          case k16th:
            noteValueKind = bsrNote::kNoteB16thKind;
            break;
          case kEighth:
            noteValueKind = bsrNote::kNoteB8thKind;
            break;
          case kQuarter:
            noteValueKind = bsrNote::kNoteBQuarterKind;
            break;
          case kHalf:
            noteValueKind = bsrNote::kNoteBHalfKind;
            break;
          case kWhole:
            noteValueKind = bsrNote::kNoteBWholeKind;
            break;
          case kBreve:
            noteValueKind = bsrNote::kNoteBBreveKind;
            break;
          case kLong:
            break;
          case kMaxima:
            break;
        } // switch
        break;
        
      case kC:
        switch (noteGraphicDurationKind) {
          case k_NoDuration:
            break;
      
          case k1024th:
            break;
          case k512th:
            break;
          case k256th:
            noteValueKind = bsrNote::kNoteC256thKind;
            break;
          case k128th:
            noteValueKind = bsrNote::kNoteC128thKind;
            break;
          case k64th:
            noteValueKind = bsrNote::kNoteC64thKind;
            break;
          case k32nd:
            noteValueKind = bsrNote::kNoteC32ndKind;
            break;
          case k16th:
            noteValueKind = bsrNote::kNoteC16thKind;
            break;
          case kEighth:
            noteValueKind = bsrNote::kNoteC8thKind;
            break;
          case kQuarter:
            noteValueKind = bsrNote::kNoteCQuarterKind;
            break;
          case kHalf:
            noteValueKind = bsrNote::kNoteCHalfKind;
            break;
          case kWhole:
            noteValueKind = bsrNote::kNoteCWholeKind;
            break;
          case kBreve:
            noteValueKind = bsrNote::kNoteCBreveKind;
            break;
          case kLong:
            break;
          case kMaxima:
            break;
        } // switch
        break;
        
      case kD:
        switch (noteGraphicDurationKind) {
          case k_NoDuration:
            break;
      
          case k1024th:
            break;
          case k512th:
            break;
          case k256th:
            noteValueKind = bsrNote::kNoteD256thKind;
            break;
          case k128th:
            noteValueKind = bsrNote::kNoteD128thKind;
            break;
          case k64th:
            noteValueKind = bsrNote::kNoteD64thKind;
            break;
          case k32nd:
            noteValueKind = bsrNote::kNoteD32ndKind;
            break;
          case k16th:
            noteValueKind = bsrNote::kNoteD16thKind;
            break;
          case kEighth:
            noteValueKind = bsrNote::kNoteD8thKind;
            break;
          case kQuarter:
            noteValueKind = bsrNote::kNoteDQuarterKind;
            break;
          case kHalf:
            noteValueKind = bsrNote::kNoteDHalfKind;
            break;
          case kWhole:
            noteValueKind = bsrNote::kNoteDWholeKind;
            break;
          case kBreve:
            noteValueKind = bsrNote::kNoteDBreveKind;
            break;
          case kLong:
            break;
          case kMaxima:
            break;
        } // switch
        break;
        
      case kE:
        switch (noteGraphicDurationKind) {
          case k_NoDuration:
            break;
      
          case k1024th:
            break;
          case k512th:
            break;
          case k256th:
            noteValueKind = bsrNote::kNoteE256thKind;
            break;
          case k128th:
            noteValueKind = bsrNote::kNoteE128thKind;
            break;
          case k64th:
            noteValueKind = bsrNote::kNoteE64thKind;
            break;
          case k32nd:
            noteValueKind = bsrNote::kNoteE32ndKind;
            break;
          case k16th:
            noteValueKind = bsrNote::kNoteE16thKind;
            break;
          case kEighth:
            noteValueKind = bsrNote::kNoteE8thKind;
            break;
          case kQuarter:
            noteValueKind = bsrNote::kNoteEQuarterKind;
            break;
          case kHalf:
            noteValueKind = bsrNote::kNoteEHalfKind;
            break;
          case kWhole:
            noteValueKind = bsrNote::kNoteEWholeKind;
            break;
          case kBreve:
            noteValueKind = bsrNote::kNoteEBreveKind;
            break;
          case kLong:
            break;
          case kMaxima:
            break;
        } // switch
        break;
        
      case kF:
        switch (noteGraphicDurationKind) {
          case k_NoDuration:
            break;
      
          case k1024th:
            break;
          case k512th:
            break;
          case k256th:
            noteValueKind = bsrNote::kNoteF256thKind;
            break;
          case k128th:
            noteValueKind = bsrNote::kNoteF128thKind;
            break;
          case k64th:
            noteValueKind = bsrNote::kNoteF64thKind;
            break;
          case k32nd:
            noteValueKind = bsrNote::kNoteF32ndKind;
            break;
          case k16th:
            noteValueKind = bsrNote::kNoteF16thKind;
            break;
          case kEighth:
            noteValueKind = bsrNote::kNoteF8thKind;
            break;
          case kQuarter:
            noteValueKind = bsrNote::kNoteFQuarterKind;
            break;
          case kHalf:
            noteValueKind = bsrNote::kNoteFHalfKind;
            break;
          case kWhole:
            noteValueKind = bsrNote::kNoteFWholeKind;
            break;
          case kBreve:
            noteValueKind = bsrNote::kNoteFBreveKind;
            break;
          case kLong:
            break;
          case kMaxima:
            break;
        } // switch
        break;
        
      case kG:
        switch (noteGraphicDurationKind) {
          case k_NoDuration:
            break;
      
          case k1024th:
            break;
          case k512th:
            break;
          case k256th:
            noteValueKind = bsrNote::kNoteG256thKind;
            break;
          case k128th:
            noteValueKind = bsrNote::kNoteG128thKind;
            break;
          case k64th:
            noteValueKind = bsrNote::kNoteG64thKind;
            break;
          case k32nd:
            noteValueKind = bsrNote::kNoteG32ndKind;
            break;
          case k16th:
            noteValueKind = bsrNote::kNoteG16thKind;
            break;
          case kEighth:
            noteValueKind = bsrNote::kNoteG8thKind;
            break;
          case kQuarter:
            noteValueKind = bsrNote::kNoteGQuarterKind;
            break;
          case kHalf:
            noteValueKind = bsrNote::kNoteGHalfKind;
            break;
          case kWhole:
            noteValueKind = bsrNote::kNoteGWholeKind;
            break;
          case kBreve:
            noteValueKind = bsrNote::kNoteGBreveKind;
            break;
          case kLong:
            break;
          case kMaxima:
            break;
        } // switch
        break;
    } // switch
  }
  
  S_bsrNote note =
    bsrNote::create (
      inputLineNumber,
      noteValueKind,
      noteDotsNumber,
      noteOctaveKind,
      bsrNote::kNoteOctaveIsNeededYes); // JMI ???

  fCurrentMeasure->
    appendNoteToMeasure (note);
}

void msr2BsrTranslator::visitEnd (S_msrNote& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrNote " <<
      elt->asString () <<
      ", line " << inputLineNumber <<
      endl;
  }
}
   
/*
//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrIdentification& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrIdentification" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  gIndenter++;

  fCurrentIdentification =
    fBsrScore->
      getMsrScore ()->
        getIdentification ();
    
  fOnGoingIdentification = true;
}

void msr2BsrTranslator::visitEnd (S_msrIdentification& elt)
{
  fOnGoingIdentification = false;
  
  gIndenter--;

  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrIdentification" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}
*/

/*
//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrCredit& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrCredit" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentCredit = elt;

  // set elt as credit of the MSR score part of the BSR score
  fBsrScore->
    getMsrScore ()->
      appendCreditToScore (fCurrentCredit);
}

void msr2BsrTranslator::visitEnd (S_msrCredit& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrCredit" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentCredit = nullptr;
}

void msr2BsrTranslator::visitStart (S_msrCreditWords& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrCreditWords" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // don't append it to the current credit, since the latter is no clone
  / * JMI
  fCurrentCredit->
    appendCreditWordsToCredit (
      elt);
      * /
}

void msr2BsrTranslator::visitEnd (S_msrCreditWords& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrCreditWords" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrSegment& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

/ * JMI

  // fetch the current segment clone
  fCurrentSegmentClone =
    fCurrentVoiceClone->
      getVoiceLastSegment ();
      * /

  // create a clone of the segment
  S_msrSegment
    segmentClone =
      elt->createSegmentNewbornClone (
        fCurrentVoiceClone);

  // push it onto the segment clones stack
  fCurrentSegmentClonesStack.push (
    segmentClone);
    
  // append it to the current voice
  fCurrentVoiceClone->
    setVoiceCloneLastSegment ( // cuts link to the one created by default JMI ???
      segmentClone);
}

void msr2BsrTranslator::visitEnd (S_msrSegment& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // forget current segment clone
  fCurrentSegmentClonesStack.pop ();
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrHarmony& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrHarmony '" <<
      elt->asString () <<
      ", fOnGoingNote = " << booleanAsString (fOnGoingNote) <<
      ", fOnGoingChord = " << booleanAsString (fOnGoingChord) <<
      ", fOnGoingHarmonyVoice = " << booleanAsString (fOnGoingHarmonyVoice) <<
      "', line " << elt->getInputLineNumber () <<
      endl;
  }

  // create a harmony new born clone
  fCurrentHarmonyClone =
    elt->createHarmonyNewbornClone (
      fCurrentVoiceClone);
      
  if (fOnGoingNote) {
    // register the harmony in the current non-grace note clone
    fCurrentNonGraceNoteClone->
      setNoteHarmony (fCurrentHarmonyClone);

  // don't append the harmony to the part harmony,
  // this has been done in pass2b
  }
  
  else if (fOnGoingChord) {
    // register the harmony in the current chord clone
    fCurrentChordClone->
      setChordHarmony (fCurrentHarmonyClone); // JMI
  }

  else if (fOnGoingHarmonyVoice) {
    fCurrentVoiceClone->
      appendHarmonyToVoiceClone (
        fCurrentHarmonyClone);
  }
}

void msr2BsrTranslator::visitStart (S_msrHarmonyDegree& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_msrHarmonyDegree '" <<
      elt->asString () <<
      ", fOnGoingNote = " << booleanAsString (fOnGoingNote) <<
      ", fOnGoingChord = " << booleanAsString (fOnGoingChord) <<
      ", fOnGoingHarmonyVoice = " << booleanAsString (fOnGoingHarmonyVoice) <<
      "', line " << elt->getInputLineNumber () <<
      endl;
  }

  // append the harmony degree to the current harmony clone
  fCurrentHarmonyClone->
    appendHarmonyDegreeToHarmony (
      elt);
}

void msr2BsrTranslator::visitEnd (S_msrHarmony& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrHarmony '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrFrame& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrFrame '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (fOnGoingNote) {
    // register the frame in the current non-grace note clone
    fCurrentNonGraceNoteClone->
      setNoteFrame (elt);
  }
}  

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrFiguredBass& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrFiguredBass '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // create a deep copy of the figured bass
  fCurrentFiguredBass =
    elt->
      createFiguredBassDeepCopy (
        fCurrentPartClone);
  
  if (fOnGoingNote) {
    // register the figured bass in the current non-grace note clone
    fCurrentNonGraceNoteClone->
      setNoteFiguredBass (fCurrentFiguredBass);

  // don't append the figured bass to the part figured bass,
  // this will be done below
  }
  
  else if (fOnGoingChord) {
    // register the figured bass in the current chord clone
    fCurrentChordClone->
      setChordFiguredBass (fCurrentFiguredBass); // JMI
  }
  
  else if (fOnGoingFiguredBassVoice) { // JMI
    // register the figured bass in the part clone figured bass
    fCurrentPartClone->
      appendFiguredBassToPartClone (
        fCurrentVoiceClone,
        fCurrentFiguredBass);
  }
}

void msr2BsrTranslator::visitStart (S_msrFigure& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrFigure '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // append the figure to the current figured bass
  fCurrentFiguredBass->
    appendFiguredFigureToFiguredBass (
      elt);
}

void msr2BsrTranslator::visitEnd (S_msrFiguredBass& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrFiguredBass '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentFiguredBass = nullptr;
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrMeasure& elt)
{    
  int
    inputLineNumber =
      elt->getInputLineNumber ();

  string
    measureNumber =
      elt->getMeasureNumber ();

  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrMeasure '" <<
      measureNumber <<
      "'" <<
      ", line " << inputLineNumber <<
      endl;
  }

/ * JMI
  {
    fLogOutputStream <<
      endl <<
      elt <<
      endl;
  }
     * /
      
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    fLogOutputStream <<
      endl <<
      "<!--=== measure '" << measureNumber <<
      "', voice \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber << " ===-->" <<
      endl;
  }
#endif

  // measure 1 is created by default initially ??? JMI
  
  // create a clone of the measure
  fCurrentMeasureClone =
    elt->
      createMeasureNewbornClone (
        fCurrentSegmentClonesStack.top ());
      
  // append it to the current segment clone
  fCurrentSegmentClonesStack.top ()->
    appendMeasureToSegment (
      fCurrentMeasureClone);
      
// JMI utile???
  fCurrentPartClone->
    setPartCurrentMeasureNumber (
      measureNumber);

  // should the last bar check's measure be set?
  if (fLastBarCheck) {
    fLastBarCheck->
      setNextBarNumber (
        measureNumber);
      
    fLastBarCheck = nullptr;
  }
}

void msr2BsrTranslator::finalizeCurrentMeasureClone (
  int          inputLineNumber,
  S_msrMeasure originalMeasure)
{
  // take this measure into account
  fMeasuresCounter++;
  
  // fetch the voice
  S_msrVoice
    voice =
      fCurrentMeasureClone->
        getMeasureSegmentUplink ()->
          getSegmentVoiceUplink ();
    
  // fetch the part measure position high tide
  rational
    partMeasureLengthHighTide = // JMI
      fCurrentMeasureClone->
        fetchMeasurePartUplink ()->
          getPartMeasureLengthHighTide ();

  // get the measure number
  string
    measureNumber =
      fCurrentMeasureClone->
        getMeasureNumber ();

  // get the measure length
  rational
    measureLength =
      fCurrentMeasureClone->
        getMeasureLength ();

  // get the full measure length
  rational
    measureFullLength =
      fCurrentMeasureClone->
        getMeasureFullLength ();
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    fLogOutputStream <<
      "Finalizing measure " << measureNumber <<
      " in voice \"" << voice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl <<
      "measureLength = " << measureLength <<
      endl <<
      "partMeasureLengthHighTide = " <<
      partMeasureLengthHighTide <<
      endl;
  }
#endif

  msrMeasure::msrMeasureKind
    measureKind =
      msrMeasure::kUnknownMeasureKind; // JMI
 // JMI     fMeasureKind = kFullMeasure; // may be changed afterwards
    
  if (measureLength == measureFullLength ) {
    // this measure is full
    measureKind =
      msrMeasure::kFullMeasureKind;
  }
      
  else if (measureLength < measureFullLength) {
    / *
    if (fSegmentMeasuresList.size () == 1) { // JMI
      // this is the first measure in the segment
      measureKind =
        msrMeasure::kIncompleteLeftMeasure;
    }
    
    else {
      // this is the last measure in the segment
      measureKind =
        msrMeasure::kIncompleteRightMeasure;
    }
    * /

    // this measure is an up beat
    measureKind =
      msrMeasure::kUpbeatMeasureKind; // JMI
  }

  else if (measureLength > measureFullLength) {
    // this measure is overfull
    measureKind =
      msrMeasure::kOverfullMeasureKind;
  }

  if (false && / * JMI * / measureKind != originalMeasure->getMeasureKind ()) { // JMI
    stringstream s;

    s <<
      "line " << inputLineNumber << ":" <<
      " clone measure:" <<
      endl <<
      fCurrentMeasureClone <<
      endl <<
      "differs for measure kind from original measure:" <<
      endl <<
      originalMeasure;

    msrInternalError (
      gXml2brlOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2BsrTranslator::visitEnd (S_msrMeasure& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrMeasure '" <<
      elt->getMeasureNumber () <<
      "'" <<
      ", line " << inputLineNumber <<
      endl;
  }

  string
    measureNumber =
      elt->getMeasureNumber ();

  finalizeCurrentMeasureClone ( // JMI
    inputLineNumber,
    elt); // original measure

  bool doCreateABarCheck = false; // JMI

  switch (elt->getMeasureKind ()) {
    
    case msrMeasure::kUnknownMeasureKind:
      {
        stringstream s;

        s <<
          "measure '" << measureNumber <<
          "' in voice \"" <<
          elt->
            fetchMeasureVoiceUplink ()->
              getVoiceName () <<
          "\" is of unknown kind";

      // JMI  msrInternalError (
        msrInternalWarning (
          gXml2brlOptions->fInputSourceName,
          inputLineNumber,
  //        __FILE__, __LINE__,
          s.str ());
      }
      break;
      
    case msrMeasure::kFullMeasureKind:
      doCreateABarCheck = true;
      break;
      
    case msrMeasure::kUpbeatMeasureKind:
      doCreateABarCheck = true;
      break;
      
    case msrMeasure::kUnderfullMeasureKind:
      doCreateABarCheck = true;
      break;
      
    case msrMeasure::kOverfullMeasureKind:
      doCreateABarCheck = true;
      break;
      
    case msrMeasure::kSenzaMisuraMeasureKind:
      doCreateABarCheck = true;
      break;
      
    case msrMeasure::kEmptyMeasureKind:
      // JMI
      break;
  } // switch

  if (doCreateABarCheck) {
    // create a bar check without next bar number,
    // it will be set upon visitStart (S_msrMeasure&)
    // for the next measure
    fLastBarCheck =
      msrBarCheck::create (
        inputLineNumber);

           / * JMI   
  fLogOutputStream <<
    endl <<
    "***********" <<
    endl <<
    endl;
  fCurrentPartClone->print (fLogOutputStream);
  fLogOutputStream <<
    "***********" <<
    endl <<
    endl;
    * /
/ * JMI
    // append it to the current voice clone
    fCurrentVoiceClone->
      appendBarCheckToVoice (fLastBarCheck);
      * /
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrStanza& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  gIndenter++;

//  if (elt->getStanzaTextPresent ()) { // JMI
    fCurrentStanzaClone =
      elt->createStanzaNewbornClone (
        fCurrentVoiceClone);
    
    // append the stanza clone to the BSR score elements list
    fBsrScore ->
      appendStanzaToScoreElements (
        fCurrentStanzaClone);
  
    // append a use of the stanza to the current staff block
    fCurrentStaffBlock ->
      appendLyricsUseToStaffBlock (
        fCurrentStanzaClone);
//  }
//  else
  //  fCurrentStanzaClone = 0; // JMI

  fOnGoingStanza = true;
}

void msr2BsrTranslator::visitEnd (S_msrStanza& elt)
{
  gIndenter--;
  
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // forget about this stanza
  fCurrentStanzaClone = nullptr;
  
  fOnGoingStanza = false;
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrSyllable& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrSyllable" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // create the syllable clone
  fCurrentSyllableClone =
    elt->createSyllableNewbornClone (
      fCurrentPartClone);

  // add it to the current stanza clone or current note clone
  if (fOnGoingStanza) { // fCurrentStanzaClone JM
    // visiting a syllable as a stanza member
    fCurrentStanzaClone->
      appendSyllableToStanza (
        fCurrentSyllableClone);
  }
  
  else if (fOnGoingNote) { // JMI
    // visiting a syllable as attached to the current non-grace note
    fCurrentSyllableClone->
      appendSyllableToNoteAndSetItsNoteUplink (
        fCurrentNonGraceNoteClone);

    if (gBsrOptions->fAddWordsFromTheLyrics) {
      // get the syllable texts list
      const list<string>&
        syllableTextsList =
          elt->getSyllableTextsList ();

      if (syllableTextsList.size ()) {
        // build a single words value from the texts list
        // JMI create an msrWords instance for each???  
        string wordsValue =
          elt->syllableTextsListAsString();
  
        // create the words
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceLyrics || gTraceOptions->fTraceWords) {
          fLogOutputStream <<
            "Changing lyrics '" <<
            wordsValue <<
            "' into words for note '" <<
            fCurrentNonGraceNoteClone->asShortString () <<
            "'" <<
      // JMI      fCurrentSyllableClone->asString () <<
            endl;
        }
#endif

        S_msrWords
          words =
            msrWords::create (
              inputLineNumber,
              kPlacementNone,                // default value
              wordsValue,
              kJustifyNone,                  // default value
              kVerticalAlignmentNone,        // default value
              kFontStyleNone,                // default value
              msrFontSize::create (
                msrFontSize::kFontSizeNone), // default value
              kFontWeightNone,               // default value
              msrWords::kItLang);            // default value
  
        // append it to the current non-grace note
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceLyrics || gTraceOptions->fTraceWords) {
          fLogOutputStream <<
            "Appending words '" <<
            words->asShortString () <<
            "' to note '" <<
            fCurrentNonGraceNoteClone->asShortString () <<
            "'" <<
            endl;
        }
#endif
        fCurrentNonGraceNoteClone->
          appendWordsToNote (
            words);
      }
    }
  }
    
  // a syllable ends the sysllable extend range if any
  if (fOnGoingSyllableExtend) {
    / * JMI ???
    // create melisma end command
    S_bsrMelismaCommand
      melismaCommand =
        bsrMelismaCommand::create (
          inputLineNumber,
          bsrMelismaCommand::kMelismaEnd);

    // append it to current voice clone
    fCurrentVoiceClone->
      appendOtherElementToVoice (melismaCommand);
* /

    fOnGoingSyllableExtend = false;
  }
}

void msr2BsrTranslator::visitEnd (S_msrSyllable& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrSyllable" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}
*/

/*
//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrTranspose& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrTranspose" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // append transpose to voice clone
  fCurrentVoiceClone->
    appendTransposeToVoice (elt);
}

void msr2BsrTranslator::visitEnd (S_msrTranspose& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrTranspose" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrPartNameDisplay& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrPartNameDisplay" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // append part name display to voice clone
  fCurrentVoiceClone->
    appendPartNameDisplayToVoice (elt);
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrPartAbbreviationDisplay& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrPartAbbreviationDisplay" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // append part abbreviation display to voice clone
  fCurrentVoiceClone->
    appendPartAbbreviationDisplayToVoice (elt);
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrTempo& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrTempo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  switch (elt->getTempoKind ()) {
    case msrTempo::k_NoTempoKind:
      break;

    case msrTempo::kTempoBeatUnitsPerMinute:
      break;

    case msrTempo::kTempoBeatUnitsEquivalence:
      break;

    case msrTempo::kTempoNotesRelationShip:
      fBsrScore->
        // this score needs the 'tongue' Scheme function
        setTempoRelationshipSchemeFunctionIsNeeded ();
      break;
  } // switch

  fCurrentVoiceClone->
    appendTempoToVoice (elt);
}

void msr2BsrTranslator::visitEnd (S_msrTempo& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrTempo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrRehearsal& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrRehearsal" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentVoiceClone->
    appendRehearsalToVoice (elt);
}

void msr2BsrTranslator::visitEnd (S_msrRehearsal& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrRehearsal" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrArticulation& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrArticulation" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendArticulationToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendArticulationToChord (elt);
  }
}

void msr2BsrTranslator::visitEnd (S_msrArticulation& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrArticulation" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrFermata& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrFermata" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // a fermata is an articulation
  
  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendArticulationToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendArticulationToChord (elt);
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrArpeggiato& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrArpeggiato" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // an arpeggiato is an articulation
  
  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendArticulationToNote (elt); // addArpeggiatoToNote ??? JMI
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendArticulationToChord (elt);
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrNonArpeggiato& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrNonArpeggiato" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // an nonArpeggiato is an articulation
  
  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendArticulationToNote (elt); // addArpeggiatoToNote ??? JMI
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendArticulationToChord (elt);
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrTechnical& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrTechnical" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalToChord (elt);
  }

  // doest the score need the 'tongue' function?
  switch (elt->getTechnicalKind ()) {
    case msrTechnical::kArrow:
      break;
    case msrTechnical::kDoubleTongue:
      fBsrScore->
        // this score needs the 'tongue' Scheme function
        setTongueSchemeFunctionIsNeeded ();
      break;
    case msrTechnical::kDownBow:
      break;
    case msrTechnical::kFingernails:
      break;
    case msrTechnical::kHarmonic:
      break;
    case msrTechnical::kHeel:
      break;
    case msrTechnical::kHole:
      break;
    case msrTechnical::kOpenString:
      break;
    case msrTechnical::kSnapPizzicato:
      break;
    case msrTechnical::kStopped:
      break;
    case msrTechnical::kTap:
      break;
    case msrTechnical::kThumbPosition:
      break;
    case msrTechnical::kToe:
      break;
    case msrTechnical::kTripleTongue:
      fBsrScore->
        // this score needs the 'tongue' Scheme function
        setTongueSchemeFunctionIsNeeded ();
      break;
    case msrTechnical::kUpBow:
      break;
  } // switch
}

void msr2BsrTranslator::visitEnd (S_msrTechnical& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrTechnical" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrTechnicalWithInteger& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrTechnicalWithInteger" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalWithIntegerToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalWithIntegerToChord (elt);
  }
}

void msr2BsrTranslator::visitEnd (S_msrTechnicalWithInteger& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrTechnicalWithInteger" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrTechnicalWithFloat& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrTechnicalWithFloat" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalWithFloatToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalWithFloatToChord (elt);
  }
}

void msr2BsrTranslator::visitEnd (S_msrTechnicalWithFloat& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrTechnicalWithFloat" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrTechnicalWithString& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrTechnicalWithString" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalWithStringToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalWithStringToChord (elt);
  }
  
  switch (elt->getTechnicalWithStringKind ()) {
    case msrTechnicalWithString::kHammerOn:
    case msrTechnicalWithString::kPullOff:
      // this score needs the 'after' Scheme function
      fBsrScore->
        setAfterSchemeFunctionIsNeeded ();     
      break;
    default:
      ;
  } // switch
}

void msr2BsrTranslator::visitEnd (S_msrTechnicalWithString& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrTechnicalWithString" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrOrnament& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrOrnament" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendOrnamentToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendOrnamentToChord (elt);
  }
}

void msr2BsrTranslator::visitEnd (S_msrOrnament& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrOrnament" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrSpanner& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrSpanner" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  switch (elt->getSpannerTypeKind ()) {
    case kSpannerTypeStart:
      break;
    case kSpannerTypeStop:
      break;
    case kSpannerTypeContinue:
      break;
    case k_NoSpannerType:
      break;
  } // switch

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendSpannerToNote (elt);
  }
  
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendSpannerToChord (elt);
  }
}

void msr2BsrTranslator::visitEnd (S_msrSpanner& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrSpanner" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrGlissando& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrGlissando" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendGlissandoToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendGlissandoToChord (elt);
  }

  if (elt->getGlissandoTextValue ().size ()) {
    fBsrScore->
      // this score needs the 'glissandoWithText' Scheme function
      addGlissandoWithTextSchemeFunctionsToScore ();
  }
}

void msr2BsrTranslator::visitEnd (S_msrGlissando& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrGlissando" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrSlide& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrSlide" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendSlideToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendSlideToChord (elt);
  }
}

void msr2BsrTranslator::visitEnd (S_msrSlide& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrSlide" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrSingleTremolo& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrSingleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      setNoteSingleTremolo (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      setChordSingleTremolo (elt);
  }
}

void msr2BsrTranslator::visitEnd (S_msrSingleTremolo& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrSingleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrDoubleTremolo& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrDoubleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // create a double tremolo clone from the two elements
  fCurrentDoubleTremoloClone = elt; // JMI FIX THAT
/ * JMI
    elt->createDoubleTremoloNewbornClone (
      elt->getDoubleTremoloFirstElement ()->
        createNewBornClone (),
      elt->getDoubleTremoloSecondElement ()
        createNewBornClone ());
        * /
  
  fOnGoingDoubleTremolo = true;
}

void msr2BsrTranslator::visitEnd (S_msrDoubleTremolo& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrSingleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // append the current double tremolo clone to the current voice clone
  fCurrentVoiceClone->
    appendDoubleTremoloToVoice (
      fCurrentDoubleTremoloClone);

  // forget about it
  fCurrentDoubleTremoloClone = nullptr;
  
  fOnGoingDoubleTremolo = false;
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrDynamics& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrDynamics" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendDynamicsToNote (elt);

    // is this a non LilyPond native dynamics?
    bool knownToLilyPondNatively = true;
    
    switch (elt->getDynamicsKind ()) {
      case msrDynamics::kFFFFF:
      case msrDynamics::kFFFFFF:
      case msrDynamics::kPPPPP:
      case msrDynamics::kPPPPPP:
      case msrDynamics::kRF:
      case msrDynamics::kSFPP:
      case msrDynamics::kSFFZ:
      case msrDynamics::k_NoDynamics:
        knownToLilyPondNatively = false;
          
      default:
        ;
    } // switch
  
    if (! knownToLilyPondNatively) {
      // this score needs the 'dynamics' Scheme function
      fBsrScore->
        setDynamicsSchemeFunctionIsNeeded ();   
    }
  }
  
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendDynamicsToChord (elt);
  }
}

void msr2BsrTranslator::visitEnd (S_msrDynamics& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrDynamics" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrOtherDynamics& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrOtherDynamics" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendOtherDynamicsToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendOtherDynamicsToChord (elt);
  }
}

void msr2BsrTranslator::visitEnd (S_msrOtherDynamics& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrOtherDynamics" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrWords& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrWords" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendWordsToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendWordsToChord (elt);
  }
}

void msr2BsrTranslator::visitEnd (S_msrWords& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrWords" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrSlur& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrSlur" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  / *
    Only the  first note of the chord should get the slur notation.
    Some applications print out the slur for all notes,
    i.e. a stop and a start in sequqnce:
    these should be ignored
  * /

  if (fOnGoingNote) {
    // don't add slurs to chord member notes except the first one
    switch (fCurrentNonGraceNoteClone->getNoteKind ()) {
      case msrNote::kChordMemberNote:
        if (fCurrentNonGraceNoteClone->getNoteIsAChordsFirstMemberNote ()) {
          fCurrentNonGraceNoteClone->
            appendSlurToNote (elt);
        }
        break;
        
      default:
        fCurrentNonGraceNoteClone->
          appendSlurToNote (elt);
    } // switch
  }
  
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendSlurToChord (elt);
  }
}

void msr2BsrTranslator::visitEnd (S_msrSlur& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrSlur" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrLigature& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrLigature" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendLigatureToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendLigatureToChord (elt);
  }
}

void msr2BsrTranslator::visitEnd (S_msrLigature& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrLigature" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrSlash& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrSlash" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendSlashToNote (elt);
  }
  
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendSlashToChord (elt);
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrWedge& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrWedge" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendWedgeToNote (elt);
  }
  
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendWedgeToChord (elt);
  }
}

void msr2BsrTranslator::visitEnd (S_msrWedge& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrWedge" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrGraceNotesGroup& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber () ;
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrGraceNotesGroup" <<
      ", line " << inputLineNumber <<
      endl;
  }

  bool doCreateAGraceNoteClone = true; // JMI

  if (doCreateAGraceNoteClone) {
    // create a clone of this graceNotesGroup
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceGraceNotes) {
      fLogOutputStream <<
        "Creating a clone of grace notes group '" << 
        elt->asShortString () <<
        "' and attaching it to clone note '" <<
        fCurrentNonGraceNoteClone->asShortString () <<
        "'" << 
        endl;
      }
#endif

    fCurrentGraceNotesGroupClone =
      elt->
        createGraceNotesGroupNewbornClone (
          fCurrentVoiceClone);

    // attach it to the current note clone
    // if (fOnGoingNote) { JMI
   // { // JMI
    
    switch (elt->getGraceNotesGroupKind ()) {
      case msrGraceNotesGroup::kGraceNotesGroupBefore:
        fCurrentNonGraceNoteClone->
          setNoteGraceNotesGroupBefore (
            fCurrentGraceNotesGroupClone);
        break;
      case msrGraceNotesGroup::kGraceNotesGroupAfter:
        fCurrentNonGraceNoteClone->
          setNoteGraceNotesGroupAfter (
            fCurrentGraceNotesGroupClone);
        break;
    } // switch
  //  }
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceGraceNotes) {
    fLogOutputStream <<
      "+++++++++++++++++++++++++ 1" <<
      endl <<
      "fCurrentNonGraceNoteClone:";
  
    if (fCurrentNonGraceNoteClone) {
      fLogOutputStream <<
        fCurrentNonGraceNoteClone;
    }
    else {
      fLogOutputStream <<
        "nullptr";
    }
    fLogOutputStream <<
      endl;
  }
#endif

  // get the note this grace notes group is attached to
  S_msrNote
    noteNotesGroupIsAttachedTo =
      elt->
        getGraceNotesGroupNoteUplink ();

  if (! noteNotesGroupIsAttachedTo) {
    stringstream s;

    s <<
      "grace notes group '" << elt->asShortString () <<
      "' has an empty note uplink";

    msrInternalError (
      gXml2brlOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
  
  fOnGoingGraceNotesGroup = true;

  // is noteNotesGroupIsAttachedTo the first one in its voice?
#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceGraceNotes
      ||
    gTraceOptions->fTraceNotes
      ||
    gTraceOptions->fTraceVoices
  ) {
    fLogOutputStream <<
      "The noteNotesGroupIsAttachedTo voice clone PEOJIOFEIOJEF '" <<
      fCurrentVoiceClone->getVoiceName () <<
      "' is '";

    if (noteNotesGroupIsAttachedTo) {
      fLogOutputStream <<
        noteNotesGroupIsAttachedTo->asShortString ();
    }
    else {
      fLogOutputStream <<
        "none";
    }
    fLogOutputStream <<
       "'" <<
      endl;
  }
#endif

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceGraceNotes
      ||
    gTraceOptions->fTraceNotes
      ||
    gTraceOptions->fTraceVoices
  ) {
    fLogOutputStream <<
      "The first note of voice clone KLJWLPOEF '" <<
      fCurrentVoiceClone->getVoiceName () <<
      "' is '";

    if (fFirstNoteCloneInVoice) {
      fLogOutputStream <<
        fFirstNoteCloneInVoice->asShortString ();
    }
    else {
      fLogOutputStream <<
        "none";
    }
    fLogOutputStream <<
       "'" <<
      endl;
  }
#endif

  // fetch the original voice first non grace note
  S_msrNote
    originalVoiceFirstNonGraceNote =
      fCurrentVoiceOriginal->
        fetchVoiceFirstNonGraceNote ();

  if (originalVoiceFirstNonGraceNote) {
    if (noteNotesGroupIsAttachedTo == originalVoiceFirstNonGraceNote) {
      // bug 34 in LilyPond should be worked around by creating
      // skip grace notes in the other voices of the part
    
      // create the skip grace notes group
  #ifdef TRACE_OPTIONS
        if (
            gTraceOptions->fTraceGraceNotes
              ||
            gTraceOptions->fTraceNotes
              ||
            gTraceOptions->fTraceVoices
        ) {
          fLogOutputStream <<
            "Creating a skip clone of grace notes group '" <<
            elt->asShortString () <<
            "' to work around LilyPond issue 34" <<
            endl;
        }
  #endif
  
      fCurrentSkipGraceNotesGroup =
        elt->
          createSkipGraceNotesGroupClone (
            fCurrentVoiceClone);
    }
  }

  // addSkipGraceNotesGroupBeforeAheadOfVoicesClonesIfNeeded() will
  // append the same skip grace notes to the ofhter voices if needed
  // in visitEnd (S_msrPart&)
}

    / * JMI
  if (fFirstNoteCloneInVoice) {
    // there is at least a note before these grace notes in the voice
    
    if (
      fCurrentNonGraceNoteClone->getNoteTrillOrnament ()
        &&
      fCurrentNonGraceNoteClone->getNoteIsFollowedByGraceNotesGroup ()) {
      // fPendingAfterGraceNotesGroup already contains
      // the afterGraceNotesGroup to use
      
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceGraceNotesGroup) {
        fLogOutputStream <<
          "Optimising grace notes '" << 
          elt->asShortString () <<
          "' into after grace notes" <<
          endl;
      }
#endif

      // attach the current after grace notes clone to the current note clone
      if (fOnGoingNote) { // JMI
        fCurrentNonGraceNoteClone->
          setNoteAfterGraceNotesGroup (
            fPendingAfterGraceNotesGroup);
      }

      doCreateAGraceNoteClone = false;
    }
  }

  if (doCreateAGraceNoteClone) {
    // are these grace notes the last element in a measure?
    if (elt->getGraceNotesGroupIsFollowedByNotes ()) {
      // yes, this is a regular grace notes

      // create a clone of this graceNotesGroup
      fCurrentGraceNotesGroupClone =
        elt->
          createGraceNotesGroupNewbornClone (
            fCurrentVoiceClone);

      // attach it to the current note clone
      if (fOnGoingNote) { // JMI
        fCurrentNonGraceNoteClone->
          setNoteGraceNotesGroup (
            fCurrentGraceNotesGroupClone);
      }

     // JMI XXL find good criterion for this
  
      // these grace notes are at the beginning of a segment JMI
  //    doCreateAGraceNoteClone = true; // JMI
  
      // bug 34 in LilyPond should be worked aroud by creating
      // skip grace notes in the other voices of the part
  
      // create skip graceNotesGroup clone
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceGraceNotesGroup) {
        fLogOutputStream <<
          "Creating a skip clone of grace notes '" <<
          elt->asShortString () <<
          "' to work around LilyPond issue 34" <<
          endl;
      }
#endif
    
      S_msrGraceNotesGroup
        skipGraceNotesGroup =
          elt->
            createSkipGraceNotesGroupClone (
              fCurrentVoiceClone);
  
      // prepend it to the other voices in the part
      fCurrentPartClone->
        prependSkipGraceNotesGroupToVoicesClones (
          fCurrentVoiceClone,
          skipGraceNotesGroup);
    }

    else {
      // no, we should build an msrAfterGraceNotesGroup from this
      // and the last element in the current voice clone

      // fetch the voice last element
      fCurrentAfterGraceNotesGroupElement =
        fCurrentVoiceClone->
          fetchVoiceLastElement (inputLineNumber);

      // create the after grace notes
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceGraceNotesGroup) {
        fLogOutputStream <<
          "Converting grace notes '" <<
          elt->asShortString () <<
          "' into after grace notes attached to:"<<
          endl;

        gIndenter++;
        
        fCurrentAfterGraceNotesGroupElement->
          print (fLogOutputStream);

        gIndenter--;
      }
#endif

      fPendingAfterGraceNotesGroup =
        msrAfterGraceNotesGroup::create (
          inputLineNumber,
            fCurrentAfterGraceNotesGroupElement,
            elt->getGraceNotesGroupIsSlashed (),
            fCurrentVoiceClone);

      // append it to the current note clone
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceGraceNotesGroup) {
        fLogOutputStream <<
          "Appending the after grace notes to current note clone" <<
          endl;
      }
#endif
      
      if (fOnGoingNote) { // JMI
        fCurrentNonGraceNoteClone->
          setNoteAfterGraceNotesGroup (
            fPendingAfterGraceNotesGroup);
      }
    }
  }
*/

/*

void msr2BsrTranslator::visitEnd (S_msrGraceNotesGroup& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrGraceNotesGroup" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceGraceNotes) {
    fLogOutputStream <<
      "+++++++++++++++++++++++++ 2" <<
      endl <<
      "fCurrentNonGraceNoteClone:";
  
    if (fCurrentNonGraceNoteClone) {
      fLogOutputStream <<
        fCurrentNonGraceNoteClone;
    }
    else {
      fLogOutputStream <<
        "nullptr";
    }
    fLogOutputStream <<
      endl;
  }
#endif

  // forget about these grace notes
  fCurrentGraceNotesGroupClone = nullptr;

  fOnGoingGraceNotesGroup = false;
  
/ * JMI
  if (fPendingAfterGraceNotesGroup) {
    // remove the current afterGraceNotesGroup note clone
    // from the current voice clone
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceGraceNotesGroup) {
      fLogOutputStream <<
        "Removing the after grace notes element from the current voice clone" <<
        endl;
    }
#endif

    fCurrentVoiceClone->
      removeElementFromVoice (
        inputLineNumber,
        fCurrentAfterGraceNotesGroupElement);

    // forget about the current after grace notes element
    fCurrentAfterGraceNotesGroupElement = nullptr;
  
    // forget about these after the pending grace notes
    fPendingAfterGraceNotesGroup = nullptr;
  }
  * /
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrNote& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrNote '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
    
  // create the note clone
  S_msrNote
    noteClone =
      elt->createNoteNewbornClone (
        fCurrentPartClone);

  // register clone in this tranlastors' voice notes map
  fVoiceNotesMap [elt] = noteClone; // JMI XXL
  
  // don't register grace notes as the current note clone,
  // but as the current grace note clone instead
/ * JMI
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceVoices) {
          fLogOutputStream <<
            "The first note of voice clone GFFF '" <<
            fCurrentVoiceClone->getVoiceName () <<
            "' is '";

          if (fFirstNoteCloneInVoice) {
            fLogOutputStream <<
              fFirstNoteCloneInVoice->asShortString ();
          }
          else {
            fLogOutputStream <<
              "none";
          }
          fLogOutputStream <<
             "'" <<
            endl;
        }
#endif
* /

  switch (elt->getNoteKind ()) {
    
    case msrNote::kGraceNote:
    case msrNote::kGraceChordMemberNote:
    case msrNote::kGraceTupletMemberNote:
      fCurrentGraceNoteClone = noteClone;
      break;
      
    default:
      fCurrentNonGraceNoteClone = noteClone;

      if (! fFirstNoteCloneInVoice) {
        fFirstNoteCloneInVoice =
          fCurrentNonGraceNoteClone;

#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceVoices) {
          fLogOutputStream <<
            "The first note of voice clone RJIRWR '" <<
            fCurrentVoiceClone->getVoiceName () <<
            "' is '" <<
            fFirstNoteCloneInVoice->asShortString () <<
             "'" <<
            endl;
        }
#endif
      }

      fOnGoingNote = true;
  } // switch

/ * JMI
  // can we optimize graceNotesGroup into afterGraceNotesGroup?
  if (
    elt->getNoteIsFollowedByGraceNotesGroup ()
      &&
    elt->getNoteTrillOrnament ()) {
    // yes, create the after grace notes
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceNotesGroup) {
      fLogOutputStream <<
        "Optimizing grace notes on trilled note '" <<
        elt->asShortString () <<
        "' as after grace notes " <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    fPendingAfterGraceNotesGroup =
      msrAfterGraceNotesGroup::create (
        inputLineNumber,
        fCurrentNonGraceNoteClone,
        false, // aftergracenoteIsSlashed, may be updated later
        fCurrentVoiceClone);

    // register current afterGraceNotesGroup element
    fCurrentAfterGraceNotesGroupElement =
      fCurrentNonGraceNoteClone;
  }
* /
}

void msr2BsrTranslator::visitEnd (S_msrNote& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrNote " <<
      elt->asString () <<
      ", line " << inputLineNumber <<
      endl;
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotesDetails) {
    fLogOutputStream <<
      "FAA fCurrentNonGraceNoteClone = " <<
      endl;
    if (fCurrentNonGraceNoteClone) {
      fLogOutputStream <<
        fCurrentNonGraceNoteClone;
    }
    else {
      fLogOutputStream <<
        "nullptr" <<
        endl;
    }
    
    fLogOutputStream <<
      "FAA fCurrentGraceNoteClone = " <<
      endl;
    if (fCurrentGraceNoteClone) {
      fLogOutputStream <<
        fCurrentGraceNoteClone;
    }
    else {
      fLogOutputStream <<
        "nullptr" <<
        endl;
    }
  }
#endif

  switch (elt->getNoteKind ()) {
    
    case msrNote::k_NoNoteKind:
      break;
      
    case msrNote::kRestNote:
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceNotes) {
        fLogOutputStream <<
          "Appending rest note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          endl;
      }
#endif
          
      fCurrentVoiceClone->
        appendNoteToVoiceClone (
          fCurrentNonGraceNoteClone);
      break;
      
    case msrNote::kSkipNote: // JMI
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceNotes) {
        fLogOutputStream <<
          "Appending skip note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          endl;
      }
#endif
          
      fCurrentVoiceClone->
        appendNoteToVoiceClone (
          fCurrentNonGraceNoteClone);
      break;
      
    case msrNote::kUnpitchedNote:
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceNotes) {
        fLogOutputStream <<
          "Appending unpitched note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          endl;
      }
#endif
          
      fCurrentVoiceClone->
        appendNoteToVoiceClone (
          fCurrentNonGraceNoteClone);
      break;
      
    case msrNote::kStandaloneNote:
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceNotes) {
        fLogOutputStream <<
          "Appending standalone note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          endl;
      }
#endif
          
      fCurrentVoiceClone->
        appendNoteToVoiceClone (
          fCurrentNonGraceNoteClone);
      break;
      
    case msrNote::kDoubleTremoloMemberNote:
      if (fOnGoingDoubleTremolo) {
        
        if (fCurrentNonGraceNoteClone->getNoteIsFirstNoteInADoubleTremolo ()) {
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceNotes) {
            fLogOutputStream <<
              "Setting note '" <<
              fCurrentNonGraceNoteClone->asString () <<
              "', line " << fCurrentNonGraceNoteClone->getInputLineNumber () <<
              ", as double tremolo first element" <<
              " in voice \"" <<
              fCurrentVoiceClone->getVoiceName () <<
              "\"" <<
              endl;
          }
#endif
              
          fCurrentDoubleTremoloClone->
            setDoubleTremoloNoteFirstElement (
              fCurrentNonGraceNoteClone);
        }
        
        else if (fCurrentNonGraceNoteClone->getNoteIsSecondNoteInADoubleTremolo ()) {
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceNotes) {
            fLogOutputStream <<
              "Setting note '" <<
              fCurrentNonGraceNoteClone->asString () <<
              "', line " << fCurrentNonGraceNoteClone->getInputLineNumber () <<
              ", as double tremolo second element" <<
              " in voice \"" <<
              fCurrentVoiceClone->getVoiceName () <<
              "\"" <<
              endl;
          }
#endif
              
          fCurrentDoubleTremoloClone->
            setDoubleTremoloNoteSecondElement (
              fCurrentNonGraceNoteClone);
        }
        
        else {
          stringstream s;

          s <<
            "note '" << fCurrentNonGraceNoteClone->asShortString () <<
            "' belongs to a double tremolo, but is not marked as such";

          msrInternalError (
            gXml2brlOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
      }

      else {
        stringstream s;

        s <<
          "double tremolo note '" << fCurrentNonGraceNoteClone->asShortString () <<
          "' found outside of a double tremolo";

        msrInternalError (
          gXml2brlOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
      
    case msrNote::kGraceNote:
    / * JMI
      fLogOutputStream <<
        "fOnGoingGraceNotesGroup = " <<
        booleanAsString (
          fOnGoingGraceNotesGroup) <<
        endl;
        * /
        
      if (! fOnGoingGraceNotesGroup) {
        stringstream s;

        s <<
          "grace note '" << fCurrentNonGraceNoteClone->asShortString () <<
          "' found outside of grace notes";

        msrInternalError (
          gXml2brlOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      else {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceGraceNotes || gTraceOptions->fTraceNotes) {
          fLogOutputStream <<
            "Appending grace note '" <<
            fCurrentGraceNoteClone->asShortString () <<
            "' to the grace notes group'" <<
            fCurrentGraceNotesGroupClone->asShortString () <<
            "' in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"" <<
            endl;
        }
#endif
  
        fCurrentGraceNotesGroupClone->
          appendNoteToGraceNotesGroup (
            fCurrentGraceNoteClone);
      }
      
    / * JMI ???
      if (fCurrentGraceNotesGroupClone) {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceGraceNotes || gTraceOptions->fTraceNotes) {
          fLogOutputStream <<
            "Appending note '" <<
            fCurrentNonGraceNoteClone->asShortString () <<
            "' to the grace notes in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"" <<
            endl;
        }
#endif
  
        fCurrentGraceNotesClone->
          appendNoteToGraceNotes (
            fCurrentNonGraceNoteClone);
      }

      else if (fPendingAfterGraceNotes) {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceGraceNotes || gTraceOptions->fTraceNotes) {
          fLogOutputStream <<
            "Appending note '" <<
            fCurrentNonGraceNoteClone->asShortString () <<
            "' to the after grace notes in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"" <<
            endl;
        }
#endif
  
        fPendingAfterGraceNotes->
          appendNoteToAfterGraceNotesContents (
            fCurrentNonGraceNoteClone);
      }
      
      else {
        stringstream s;

        s <<
          "both fCurrentGraceNoteGroupsClone and fPendingAfterGraceNoteGroup are null," <<
          endl <<
          "cannot handle grace note'" <<
          elt->asString () <<
          "'";

        msrInternalError (
          gXml2brlOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      * /
      break;
      
    case msrNote::kChordMemberNote:
      if (fOnGoingChord) {
        fCurrentChordClone->
          addAnotherNoteToChord (
            fCurrentNonGraceNoteClone,
            fCurrentVoiceClone);
      }
      
      else {
        stringstream s;

        s <<
          "msr2BsrTranslator:::visitEnd (S_msrNote& elt): chord member note " <<
          elt->asString () <<
          " appears outside of a chord";

        msrInternalError (
          gXml2brlOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
        }
      break;

    case msrNote::kGraceChordMemberNote:
      if (fOnGoingChord) {
        fCurrentChordClone->
          addAnotherNoteToChord (
            fCurrentGraceNoteClone,
            fCurrentVoiceClone);
      }
      
      else {
        stringstream s;

        s <<
          "msr2BsrTranslator:::visitEnd (S_msrNote& elt): chord member note " <<
          elt->asString () <<
          " appears outside of a chord";

        msrInternalError (
          gXml2brlOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
        }
      break;
      
    case msrNote::kTupletMemberNote:
    case msrNote::kGraceTupletMemberNote:
    case msrNote::kTupletMemberUnpitchedNote:
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceNotes) {
        fLogOutputStream <<
          "Appending note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "'' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          endl;
      }
#endif
          
      fTupletClonesStack.top ()->
        addNoteToTuplet (
          fCurrentNonGraceNoteClone,
          fCurrentVoiceClone);
      break;
  } // switch

  // handle editorial accidentals
  switch (fCurrentNonGraceNoteClone->getNoteEditorialAccidentalKind ()) {
    case msrNote::kNoteEditorialAccidentalYes:
      fBsrScore->
        // this score needs the 'editorial accidental' Scheme function
        setEditorialAccidentalSchemeFunctionIsNeeded ();
      break;
    case msrNote::kNoteEditorialAccidentalNo:
      break;
  } // switch
  
  // handle cautionary accidentals
  switch (fCurrentNonGraceNoteClone->getNoteCautionaryAccidentalKind ()) {
    case msrNote::kNoteCautionaryAccidentalYes:
      break;
    case msrNote::kNoteCautionaryAccidentalNo:
      break;
  } // switch

/ * JMI
  // handle melisma
  msrSyllable::msrSyllableExtendKind
    noteSyllableExtendKind =
      elt->getNoteSyllableExtendKind ();

  switch (noteSyllableExtendKind) {
    case msrSyllable::kStandaloneSyllableExtend:
      {
        / * JMI ???
        // create melisma start command
        S_bsrMelismaCommand
          melismaCommand =
            bsrMelismaCommand::create (
              inputLineNumber,
              bsrMelismaCommand::kMelismaStart);
    
        // append it to current voice clone
        fCurrentVoiceClone->
          appendOtherElementToVoice (melismaCommand);

        // append
        * /

        fOnGoingSyllableExtend = true;
      }
      break;
    case msrSyllable::kStartSyllableExtend:
      break;
    case msrSyllable::kContinueSyllableExtend:
      break;
    case msrSyllable::kStopSyllableExtend:
      break;
    case msrSyllable::k_NoSyllableExtend:
      break;
  } // switch
* /

  fOnGoingNote = false;
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrOctaveShift& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrOctaveShift" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentNonGraceNoteClone->
    setNoteOctaveShift (elt);
}

void msr2BsrTranslator::visitEnd (S_msrOctaveShift& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrOctaveShift" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrAccordionRegistration& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrAccordionRegistration" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // append the accordion registration to the voice clone
  fCurrentVoiceClone->
    appendAccordionRegistrationToVoice (elt);

  // the generated code needs modules scm and accreg
  fBsrScore->
    setScmAndAccregSchemeModulesAreNeeded ();
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrHarpPedalsTuning& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrHarpPedalsTuning" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // append the harp pedals tuning to the voice clone
  fCurrentVoiceClone->
    appendHarpPedalsTuningToVoice (elt);
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrStem& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrStem" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      setNoteStem (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendStemToChord (elt);
  }
}

void msr2BsrTranslator::visitEnd (S_msrStem& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrStem" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrBeam& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrBeam" <<
      ", line " << elt->getInputLineNumber () <<
// JMI      ", fOnGoingNote = " << booleanAsString (fOnGoingNote) <<
// JMI      ", fOnGoingChord = " << booleanAsString (fOnGoingChord) <<
      endl;
  }

  // a beam may be present at the same time
  // in a note or grace note and the chord the latter belongs to
  
  if (fOnGoingGraceNotesGroup) {
    fCurrentGraceNoteClone->
      appendBeamToNote (elt);
  }
  else if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendBeamToNote (elt);
  }

  if (fOnGoingChord) {
    fCurrentChordClone->
      appendBeamToChord (elt);
  }
}

void msr2BsrTranslator::visitEnd (S_msrBeam& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrBeam" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrChord& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrChord" <<
      ", line " << inputLineNumber <<
      endl;
  }

  fCurrentChordClone =
    elt->createChordNewbornClone (
      fCurrentPartClone);

  if (fTupletClonesStack.size ()) {
    // a chord in a tuplet is handled as part of the tuplet JMI
    fTupletClonesStack.top ()->
      addChordToTuplet (
        fCurrentChordClone);
  }

  else if (fOnGoingDoubleTremolo) {
    if (elt->getChordIsFirstChordInADoubleTremolo ()) {
      // replace double tremolo's first element by chord
      fCurrentDoubleTremoloClone->
        setDoubleTremoloChordFirstElement (
          elt);
    }
    
    else if (elt->getChordIsSecondChordInADoubleTremolo ()) {
      // replace double tremolo's second element by chord
      fCurrentDoubleTremoloClone->
        setDoubleTremoloChordSecondElement (
          elt);
    }
    
    else {
      stringstream s;

      s <<
        "chord '" << elt->asString () <<
        "' belongs to a double tremolo, but is not marked as such";

      msrInternalError (
        gXml2brlOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }
  
  else if (fCurrentGraceNotesGroupClone) {
    // append the chord to the grace notes
    fCurrentGraceNotesGroupClone->
      appendChordToGraceNotesGroup (
        fCurrentChordClone);
  }
  
  else {
    // appending the chord to the voice clone at once
    fCurrentVoiceClone->
      appendChordToVoice (
        fCurrentChordClone);
  }

  fOnGoingChord = true;
}

void msr2BsrTranslator::visitEnd (S_msrChord& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrChord" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fOnGoingChord = false;
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrTuplet& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrTuplet" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // create the tuplet clone
  S_msrTuplet
    tupletClone =
      elt->createTupletNewbornClone ();

  // register it in this visitor
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets) {
    fLogOutputStream <<
      "++> pushing tuplet '" <<
      tupletClone->asString () <<
      "' to tuplets stack" <<
      endl;
  }
#endif
  
  fTupletClonesStack.push (tupletClone);

  switch (elt->getTupletLineShapeKind ()) {
    case msrTuplet::kTupletLineShapeStraight:
    case msrTuplet::kTupletLineShapeCurved:
      fBsrScore->
        // this score needs the 'tuplets curved brackets' Scheme function
        setTupletsCurvedBracketsSchemeFunctionIsNeeded ();   
      break;
  } // switch
}

void msr2BsrTranslator::visitEnd (S_msrTuplet& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrTuplet" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets) {
    fLogOutputStream <<
      "Popping tuplet '" <<
      elt->asString () <<
      "' from tuplets stack" <<
      endl;
  }
#endif
      
  fTupletClonesStack.pop ();

  if (fTupletClonesStack.size ()) {
    // tuplet is a nested tuplet
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceTuplets) {
      fLogOutputStream <<
        "Adding nested tuplet '" <<
      elt->asString () <<
        "' to stack top tuplet '" <<
      fTupletClonesStack.top ()->asString () <<
      "'" <<
      endl;
    }
#endif
    
    fTupletClonesStack.top ()->
      addTupletToTupletClone (elt);
  }
  
  else {
    // tuplet is a top level tuplet
    
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceTuplets) {
      fLogOutputStream <<
        "Adding top level tuplet '" <<
      elt->asString () <<
      "' to voice" <<
      fCurrentVoiceClone->getVoiceName () <<
      endl;
    }
#endif
      
    fCurrentVoiceClone->
      appendTupletToVoice (elt);
  }  
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrTie& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrTie" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentNonGraceNoteClone->
    setNoteTie (elt);
}

void msr2BsrTranslator::visitEnd (S_msrTie& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrTie" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrSegno& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrSegno" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentVoiceClone->
    appendSegnoToVoice (elt);
}

void msr2BsrTranslator::visitStart (S_msrCoda& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrCoda" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentVoiceClone->
    appendCodaToVoice (elt);
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrEyeGlasses& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting eyeGlasses" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentNonGraceNoteClone->
    appendEyeGlassesToNote (elt);
}

void msr2BsrTranslator::visitStart (S_msrScordatura& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting scordatura" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentNonGraceNoteClone->
    appendScordaturaToNote (elt);
}

void msr2BsrTranslator::visitStart (S_msrPedal& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting pedal" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentNonGraceNoteClone->
    appendPedalToNote (elt);
}

void msr2BsrTranslator::visitStart (S_msrDamp& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting damp" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentNonGraceNoteClone->
    appendDampToNote (elt);

  fBsrScore->
    // this score needs the 'custom short barline' Scheme function
    setDampMarkupIsNeeded ();
}

void msr2BsrTranslator::visitStart (S_msrDampAll& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting dampAll" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentNonGraceNoteClone->
    appendDampAllToNote (elt);

  fBsrScore->
    // this score needs the 'custom short barline' Scheme function
    setDampAllMarkupIsNeeded ();
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrBarCheck& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrBarCheck" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentVoiceClone->
    appendBarCheckToVoice (elt);
}

void msr2BsrTranslator::visitEnd (S_msrBarCheck& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrBarCheck" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrBarNumberCheck& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrBarNumberCheck" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentVoiceClone->
    appendBarNumberCheckToVoice (elt);
}

void msr2BsrTranslator::visitEnd (S_msrBarNumberCheck& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrBarNumberCheck" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrRepeat" <<
      ", line " << inputLineNumber <<
      endl;
  }

/ * JMI
  fLogOutputStream <<
    endl <<
    "*********** fCurrentPartClone" <<
    endl <<
    endl;
  fCurrentPartClone->print (fLogOutputStream);
  fLogOutputStream <<
    "*********** fCurrentPartClone" <<
    endl <<
    endl;
    * /

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    fLogOutputStream <<
      "Preparing for repeat in part clone" <<
      fCurrentPartClone->getPartCombinedName () <<
      endl;
  }
#endif

/ * JMI ???
  fCurrentPartClone->
    prepareForRepeatInPart (
      inputLineNumber);
      * /
  fCurrentVoiceClone->
    prepareForRepeatInVoiceClone (
      inputLineNumber,
      elt->getRepeatTimes ());

/ * JMI
  // create a repeat clone
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    fLogOutputStream <<
      "Creating a repeat newborn clone" <<
      ", line " << inputLineNumber <<
      ", in voice \"" <<
      elt->
        getRepeatVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fCurrentRepeatClone =
    elt->
      createRepeatNewbornClone (
        fCurrentVoiceClone);
* /
        
  fOnGoingRepeat = true; // JMI
}

void msr2BsrTranslator::visitEnd (S_msrRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrRepeat" <<
      ", line " << inputLineNumber <<
      endl;
  }

/ * JMI
  // append the repeat clone to the current part clone
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    fLogOutputStream <<
      "Appending a repeat to part clone " <<
      fCurrentPartClone->getPartCombinedName () << "\"" <<
      endl;
  }
#endif

  fCurrentPartClone-> // no test needed JMI
    appendRepeatCloneToPart (
      inputLineNumber,
      fCurrentRepeatClone);
* /

  // forget about current repeat clone // JMI
// JMI  fCurrentRepeatClone = 0;
  
  fOnGoingRepeat = false;
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrRepeatCommonPart& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrRepeatCommonPart" <<
      ", line " << inputLineNumber <<
      endl;
  }

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceRepeats
      ||
    gTraceOptions->fTraceVoicesDetails
  ) {
    gLogIOstream <<
      endl <<
      "Upon visitStart (S_msrRepeatCommonPart&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      endl;

    gIndenter++;
      
    gLogIOstream <<
      fCurrentVoiceClone;

    gIndenter--;
  }
#endif
}

void msr2BsrTranslator::visitEnd (S_msrRepeatCommonPart& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrRepeatCommonPart" <<
      ", line " << inputLineNumber <<
      endl;
  }

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceRepeats
      ||
    gTraceOptions->fTraceVoicesDetails
  ) {
    gLogIOstream <<
      endl <<
      "Upon visitEnd (S_msrRepeatCommonPart&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      endl;

    gIndenter++;
      
    gLogIOstream <<
      fCurrentVoiceClone;

    gIndenter--;
  }
#endif

  // create a repeat and append it to voice clone
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    fLogOutputStream <<
      "Appending a repeat to voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    createRepeatUponItsEndAndAppendItToVoiceClone ( // JMI
      inputLineNumber,
      elt->
        getRepeatCommonPartRepeatUplink ()->
          getRepeatTimes ());
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrRepeatEnding& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrRepeatEnding" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void msr2BsrTranslator::visitEnd (S_msrRepeatEnding& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrRepeatEnding" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // create a repeat ending clone and append it to voice clone
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    fLogOutputStream <<
      "Appending a repeat ending clone to voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }

  if (gTraceOptions->fTraceRepeats || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      endl <<
      "*********>> msrRepeatEnding HHH " <<
      ", line " << inputLineNumber <<
      " contains:" <<
      endl <<
      elt <<
      endl <<
      "<<*********" <<
      endl <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendRepeatEndingToVoice (
      inputLineNumber,
      elt->getRepeatEndingNumber (),
      elt->getRepeatEndingKind ());
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrMeasuresRepeat& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrMeasuresRepeat" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  gIndenter++;
}

void msr2BsrTranslator::visitEnd (S_msrMeasuresRepeat& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrMeasuresRepeat" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  gIndenter--;

  // set last segment as the measure repeat pattern segment
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    fLogOutputStream <<
      "Setting current last segment as measure repeat pattern segment in voice \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrMeasuresRepeatPattern& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrMeasuresRepeatPattern" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  gIndenter++;
}

void msr2BsrTranslator::visitEnd (S_msrMeasuresRepeatPattern& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrMeasuresRepeatPattern" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  gIndenter--;

  // get the measures repeat uplink
  S_msrMeasuresRepeat
    measuresRepeat =
      elt->getMeasuresRepeatUplink ();

  // create a measures repeat and append it to voice clone
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasureRepeats) {
    fLogOutputStream <<
      "Appending a measures repeat to voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    createMeasuresRepeatAndAppendItToVoiceClone (
      inputLineNumber,
      measuresRepeat->
        getMeasuresRepeatMeasuresNumber (),
      measuresRepeat->
        getMeasuresRepeatSlashesNumber ());

  // forget about the current measure repeat pattern clone
  fCurrentMeasuresRepeatPatternClone = nullptr;
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrMeasuresRepeatReplicas& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrMeasuresRepeatReplicas" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  gIndenter++;
}

void msr2BsrTranslator::visitEnd (S_msrMeasuresRepeatReplicas& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrMeasuresRepeatReplicas" <<
      ", line " << inputLineNumber <<
      endl;
  }

  gIndenter--;

  // create a measures repeat replica clone and append it to voice clone
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    fLogOutputStream <<
      "Appending a repeat replica clone to voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendMeasuresRepeatReplicaToVoice (
      inputLineNumber);

  // forget about the current measure repeat replicas clone
 // JMI ??? fCurrentMeasuresRepeatReplicasClone = nullptr;
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrMultipleRest& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrMultipleRest" <<
      ", line " << inputLineNumber <<
      endl;
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMultipleRests) {
    fLogOutputStream <<
      "Preparing for multiple rest in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceMultipleRests
      ||
    gTraceOptions->fTraceVoicesDetails
  ) {
    gLogIOstream <<
      endl <<
      "Upon I visitStart (S_msrMultipleRest&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      endl;

    gIndenter++;
      
    gLogIOstream <<
      fCurrentVoiceClone;

    gIndenter--;
  }
#endif

  fCurrentVoiceClone->
    prepareForMultipleRestInVoiceClone (
      inputLineNumber);

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceMultipleRests
      ||
    gTraceOptions->fTraceVoicesDetails
  ) {
    gLogIOstream <<
      endl <<
      "Upon II visitStart (S_msrMultipleRest&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      endl;

    gIndenter++;
      
    gLogIOstream <<
      fCurrentVoiceClone;

    gIndenter--;
  }
#endif
}

void msr2BsrTranslator::visitEnd (S_msrMultipleRest& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrMultipleRest" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // create the multiple rest clone
#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceMultipleRests
      ||
    gTraceOptions->fTraceSegments
      ||
    gTraceOptions->fTraceVoices
  ) {
    fLogOutputStream <<
      "Creating a clone of multiple rest '" <<
      "'" <<
      elt->asShortString () <<
      "' in voice \"" <<
      fCurrentVoiceClone->getVoiceName () << "\"" <<
      ", fCurrentMultipleRestContentsClone =" <<
      endl;

    gIndenter++;

    fLogOutputStream <<
      fCurrentMultipleRestContentsClone;

    gIndenter--;
  }
#endif

  S_msrMultipleRest
    multipleRestClone =
      elt->createMultipleRestNewbornClone (
        fCurrentVoiceClone);

  // set the multiple rest clone's contents
  multipleRestClone->
    setMultipleRestContents (
      fCurrentMultipleRestContentsClone);
    
  // create a new last segment to collect the remainder of the voice,
  // containing the next, yet incomplete, measure
#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceMultipleRests
      ||
    gTraceOptions->fTraceSegments
      ||
    gTraceOptions->fTraceVoices
  ) {
    fLogOutputStream <<
      "Creating a new last segment for the remainder of voice \"" <<
      fCurrentVoiceClone->getVoiceName () << "\"" <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    createNewLastSegmentForVoice (
      inputLineNumber);

  // append the multiple rest clone to the current voice clone
  fCurrentVoiceClone->
    appendMultipleRestCloneToVoice (
      inputLineNumber, // JMI ???
      multipleRestClone);
      
  // forget about the current multiple rest contents clone
  fCurrentMultipleRestContentsClone = nullptr;

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceMultipleRests
      ||
    gTraceOptions->fTraceVoicesDetails
  ) {
    gLogIOstream <<
      endl <<
      "Upon visitEnd (S_msrMultipleRest&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      endl;

    gIndenter++;
      
    gLogIOstream <<
      fCurrentVoiceClone;

    gIndenter--;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrMultipleRestContents& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrMultipleRestContents" <<
      ", line " << inputLineNumber <<
      endl;
  }

  gIndenter++;

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceMultipleRests
      ||
    gTraceOptions->fTraceVoicesDetails
  ) {
    gLogIOstream <<
      endl <<
      "Upon visitStart (S_msrMultipleRestContents&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      endl;

    gIndenter++;
      
    gLogIOstream <<
      fCurrentVoiceClone;

    gIndenter--;
  }
#endif
}

void msr2BsrTranslator::visitEnd (S_msrMultipleRestContents& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrMultipleRestContents" <<
      ", line " << inputLineNumber <<
      endl;
  }

  gIndenter--;

  // create a multiple rest contents clone
  fCurrentMultipleRestContentsClone =
    elt->createMultipleRestContentsNewbornClone (
      fCurrentVoiceClone);

  // set last segment as the multiple rest contents segment
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMultipleRests) {
    fLogOutputStream <<
      "Setting current last segment as multiple rest contents segment in voice \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fCurrentMultipleRestContentsClone->
    setMultipleRestContentsSegment (
      fCurrentVoiceClone->
        getVoiceLastSegment ());

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceMultipleRests
      ||
    gTraceOptions->fTraceVoicesDetails
  ) {
    gLogIOstream <<
      endl <<
      "Upon visitEnd (S_msrMultipleRestContents&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      endl;

    gIndenter++;
      
    gLogIOstream <<
      fCurrentVoiceClone;

    gIndenter--;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrBarline& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrBarline" <<
      ", line " << inputLineNumber <<
      endl;
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceBarlines) {
    fLogOutputStream <<
      "Handling '" <<
      msrBarline::barlineCategoryKindAsString (
        elt->getBarlineCategory ()) <<
      "' in voice \"" <<
      fCurrentVoiceClone->getVoiceName () << "\"" <<
      endl;
  }
#endif

  switch (elt->getBarlineStyleKind ()) {
    case msrBarline::kBarlineStyleNone:
      break;
    case msrBarline::kBarlineStyleRegular:
      break;
    case msrBarline::kBarlineStyleDotted:
      break;
    case msrBarline::kBarlineStyleDashed:
      break;
    case msrBarline::kBarlineStyleHeavy:
      break;
    case msrBarline::kBarlineStyleLightLight:
      break;
    case msrBarline::kBarlineStyleLightHeavy:
      break;
    case msrBarline::kBarlineStyleHeavyLight:
      break;
    case msrBarline::kBarlineStyleHeavyHeavy:
      break;
    case msrBarline::kBarlineStyleTick:
      break;
    case msrBarline::kBarlineStyleShort:
      fBsrScore->
        // this score needs the 'custom short barline' Scheme function
        setCustomShortBarLineSchemeFunctionIsNeeded ();
      break;
      / * JMI
    case msrBarline::kBarlineStyleNone:
      break;
      * /
  } // switch

  // append the barline to the current voice clone
  fCurrentVoiceClone->
    appendBarlineToVoice (elt);
}

void msr2BsrTranslator::visitEnd (S_msrBarline& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrBarline" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrVarValAssoc& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrVarValAssoc" <<
      ", line " << inputLineNumber <<
      endl;
  }

  msrVarValAssoc::msrVarValAssocKind
    varValAssocKind =
      elt->getVarValAssocKind ();
  string variableValueAux = elt->getVariableValue ();
  string variableValue;

  // escape quotes if any
  for_each (
    variableValueAux.begin (),
    variableValueAux.end (),
    stringQuoteEscaper (variableValue));

  switch (varValAssocKind) {
    case msrVarValAssoc::kWorkNumber:
      fCurrentIdentification->
        setWorkNumber (
          inputLineNumber, variableValue);
      
      fBsrScoreHeader->
        setWorkNumber (
          inputLineNumber, variableValue);
  
      fWorkNumberKnown = true;
      break;
  
    case msrVarValAssoc::kWorkTitle:
      fCurrentIdentification->
        setWorkTitle (
          inputLineNumber, variableValue);
      
      fBsrScoreHeader->
        setWorkTitle (
          inputLineNumber, variableValue);
          
      fWorkTitleKnown = true;
      break;
  
    case msrVarValAssoc::kMovementNumber:
      fCurrentIdentification->
        setMovementNumber (
          inputLineNumber, variableValue);
      
      fBsrScoreHeader->
        setMovementNumber (
          inputLineNumber, variableValue);
  
      fMovementNumberKnown = true;
      break;
  
    case msrVarValAssoc::kMovementTitle:
      fCurrentIdentification->
        setMovementTitle (
          inputLineNumber, variableValue);
      
      fBsrScoreHeader->
        setMovementTitle (
          inputLineNumber, variableValue);
          
      fMovementTitleKnown = true;
      break;
  
    case msrVarValAssoc::kEncodingDate:
      fCurrentIdentification->
        setEncodingDate (
          inputLineNumber, variableValue);
      
      fBsrScoreHeader->
        setEncodingDate (
          inputLineNumber, variableValue);
      break;

    case msrVarValAssoc::kScoreInstrument:
      fCurrentIdentification->
        setScoreInstrument (
          inputLineNumber, variableValue);
      
      fBsrScoreHeader->
        setScoreInstrument (
          inputLineNumber, variableValue);
      break;

    case msrVarValAssoc::kMiscellaneousField:
      fCurrentIdentification->
        setMiscellaneousField (
          inputLineNumber, variableValue);
      
      fBsrScoreHeader->
        setMiscellaneousField (
          inputLineNumber, variableValue);
      break;

    default:
      {
      stringstream s;
    
      s <<
        "### msrVarValAssoc kind '" <<
        msrVarValAssoc::varValAssocKindAsString (
          varValAssocKind) <<
        "' is not handled";
    
      msrMusicXMLWarning (
        gXml2brlOptions->fInputSourceName,
        inputLineNumber,
        s.str ());
      }
  } // switch
}

void msr2BsrTranslator::visitEnd (S_msrVarValAssoc& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrVarValAssoc" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrVarValsListAssoc& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrVarValsListAssoc" <<
      ", line " << inputLineNumber <<
      endl;
  }

  msrVarValsListAssoc::msrVarValsListAssocKind
    varValsListAssocKind =
      elt->getVarValsListAssocKind ();

  const list<string>&
    variableValuesList =
      elt->getVariableValuesList ();

  switch (varValsListAssocKind) {
    case msrVarValsListAssoc::kRights:
      for (list<string>::const_iterator i = variableValuesList.begin ();
        i != variableValuesList.end ();
        i++) {
        fBsrScoreHeader->
          addRights (
            inputLineNumber, (*i));
      } // for
      break;
  
    case msrVarValsListAssoc::kComposer:
      for (list<string>::const_iterator i = variableValuesList.begin ();
        i != variableValuesList.end ();
        i++) {
        fBsrScoreHeader->
          addComposer (
            inputLineNumber, (*i));
      } // for
      break;
  
    case msrVarValsListAssoc::kArranger:
      for (list<string>::const_iterator i = variableValuesList.begin ();
        i != variableValuesList.end ();
        i++) {
        fBsrScoreHeader->
          addArranger (
            inputLineNumber, (*i));
      } // for
      break;
  
    case msrVarValsListAssoc::kLyricist:
      for (list<string>::const_iterator i = variableValuesList.begin ();
        i != variableValuesList.end ();
        i++) {
        fBsrScoreHeader->
          addLyricist (
            inputLineNumber, (*i));
      } // for
      break;

    case msrVarValsListAssoc::kPoet:
      for (list<string>::const_iterator i = variableValuesList.begin ();
        i != variableValuesList.end ();
        i++) {
        fBsrScoreHeader->
          addLyricist ( // JMI
            inputLineNumber, (*i));
      } // for
      break;
  
    case msrVarValsListAssoc::kTranslator:
      for (list<string>::const_iterator i = variableValuesList.begin ();
        i != variableValuesList.end ();
        i++) {
        fBsrScoreHeader->
          addTranslator (
            inputLineNumber, (*i));
      } // for
      break;

    case msrVarValsListAssoc::kSoftware:
      for (list<string>::const_iterator i = variableValuesList.begin ();
        i != variableValuesList.end ();
        i++) {
        fBsrScoreHeader->
          addSoftware (
            inputLineNumber, (*i));
      } // for
      break;
/ * JMI
    default:
      {
      stringstream s;
    
      s <<
        "### msrVarValsListAssoc kind '" <<
        msrVarValsListAssoc::varValsListAssocKindAsString (
          varValsListAssocKind) <<
        "' is not handled";
   
      msrMusicXMLWarning (
        gXml2brlOptions->fInputSourceName,
        inputLineNumber,
        s.str ());
      }
      * /
  } // switch
}

void msr2BsrTranslator::visitEnd (S_msrVarValsListAssoc& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrVarValsListAssoc" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrLayout& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  gIndenter++;
}

void msr2BsrTranslator::visitEnd (S_msrLayout& elt)
{
  gIndenter--;

  if (gMsrOptions->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}
*/


} // namespace
