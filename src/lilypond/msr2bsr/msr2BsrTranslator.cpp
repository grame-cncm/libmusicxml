/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <sstream>
#include <climits>      // INT_MIN, INT_MAX
#include <algorithm>    // for_each

#include "conversions.h"

#include "msr2BsrTranslator.h"

#include "msr.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "mxmlTreeOah.h"
#include "msrOah.h"
#include "brailleOah.h"


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

  // notes
  fCurrentNoteOctaveKind    = bsrNote::kNoteOctaveNone;
  fCurrentNoteValueSizeKind = bsrNote::kNoteValueSizeLarger;

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

void msr2BsrTranslator::notSupportedMessage (
  int    inputLineNumber,
  string message)
{
  // issue a warning message to the output log stream
  bsrMusicXMLWarning (
    gOahOah->fInputSourceName,
    inputLineNumber,
    message);

  // append a note to the BSR JMI
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrPageBreak& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrPageBreak" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fCurrentPage =
    bsrPage::create (
      inputLineNumber,
      ++fCurrentPrintPageNumber,
      gBrailleOah->fLinesPerPage);

  fBsrScore->
    appendPageToScore (fCurrentPage);
}

void msr2BsrTranslator::visitEnd (S_msrPageBreak& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrPageBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrLineBreak& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrLineBreak" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create the line
  fCurrentLine =
    bsrLine::create (
      inputLineNumber,
      ++fCurrentPrintLineNumber,
      gBrailleOah->fCellsPerLine);

  // append it to the current page
  fCurrentPage->
    appendLineToPage (fCurrentLine);

  // a note octave will be needed for the next note to come,
  // i.e., the first one in the new line
  fCurrentNoteOctaveKind = bsrNote::kNoteOctaveNone;
}

void msr2BsrTranslator::visitEnd (S_msrLineBreak& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrLineBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrScore& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrScore" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create the BSR score
  fBsrScore =
    bsrScore::create (
      K_NO_INPUT_LINE_NUMBER,
      fVisitedMsrScore);

  // get the worktitle if any
  S_msrIdentification
    identification =
      elt->getIdentification ();
  S_msrVarValAssoc
    workTitleVarValAssoc =
      identification->getWorkTitle ();

  string workTitle;

  if (workTitleVarValAssoc) {
    workTitle =
      workTitleVarValAssoc->getVariableValue ();
  }

  // append a first transcription note to it
  S_bsrTranscriptionNotesElement
    transcriptionNotesElement =
      bsrTranscriptionNotesElement::create (
        inputLineNumber,
        "This braille music data created by "
          +
        gOahOah->fHandlerExecutableName);

  fBsrScore->
    getTranscriptionNotes ()->
      appendElementToTranscriptionNotes (
        transcriptionNotesElement);

  // create the first page
  fCurrentPage =
    bsrPage::create (
      inputLineNumber,
      fCurrentPrintPageNumber,
      gBrailleOah->fLinesPerPage);

    /* JMI
  // create a pagination
  S_bsrPagination
    pagination =
      bsrPagination::create (
        inputLineNumber,
        1,  // printPageNumber
        1); // braillePageNumber

  // create the first page heading
  fFirstPageHeading =
    bsrPageHeading::create (
      inputLineNumber,
      workTitle,
      pagination,
      1); //    pageHeadingNumber

  // append it to the first page
  fCurrentPage->
    appendPageHeadingToPage (fFirstPageHeading);
    */

  // create the first music heading
  fFirstMusicHeading =
    bsrMusicHeading::create (
      inputLineNumber);

  // append it to the first page
  fCurrentPage->
    appendMusicHeadingToPage (fFirstMusicHeading);

/* JMI
  // create a foot notes
  S_bsrFootNotes
    footNotes =
      bsrFootNotes::create (
        inputLineNumber);

  // append a first transcription note to it
  S_bsrFootNotesElement
    footNotesElement =
      bsrFootNotesElement::create (
        inputLineNumber,
        "A sympathetic and most useful foot note");

  footNotes->
    appendElementToFootNotes (
      footNotesElement);

  // append foot notes to the first page
  fCurrentPage->
    appendFootNotesToPage (footNotes);
*/

  // append first page to the score
  fBsrScore->
    appendPageToScore (fCurrentPage);

  // create the fisrt line
  fCurrentLine =
    bsrLine::create (
      inputLineNumber,
      fCurrentPrintLineNumber,
      gBrailleOah->fCellsPerLine);

  // append the fisrt line to the first page
  fCurrentPage->
    appendLineToPage (fCurrentLine);


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
  if (gLilypondOah->fRights.size ()) {
    // define rights

    fBsrScoreHeader->
      addRights (
        inputLineNumber,
        gLilypondOah->fRights);
  }

  // is there a composer option?
  if (gLilypondOah->fComposer.size ()) {
    // define composer

    fBsrScoreHeader->
      addComposer (
        inputLineNumber,
        gLilypondOah->fComposer);
  }

  // is there an arranger option?
  if (gLilypondOah->fArranger.size ()) {
    // define arranger

    fBsrScoreHeader->
      addArranger (
        inputLineNumber,
        gLilypondOah->fArranger);
  }

  // is there a poet option?
  if (gLilypondOah->fPoet.size ()) {
    // define poet

    fBsrScoreHeader->
      addPoet (
        inputLineNumber,
        gLilypondOah->fPoet);
  }

  // is there a lyricist option?
  if (gLilypondOah->fLyricist.size ()) {
    // define lyricist

    fBsrScoreHeader->
      addLyricist (
        inputLineNumber,
        gLilypondOah->fLyricist);
  }

  // is there a software option?
  if (gLilypondOah->fSoftware.size ()) {
    // define software

    fBsrScoreHeader->
      addSoftware (
        inputLineNumber,
        gLilypondOah->fSoftware);
  }

  // is the Scheme function 'whiteNoteHeads' to be generated?
  if (gLilypondOah->fWhiteNoteHeads) {
    fBsrScore->
      // this score needs the 'whiteNoteHeads' Scheme function
      setWhiteNoteHeadsIsNeeded ();
  }

  // is Jianpu notation to be generated?
  if (gLilypondOah->fJianpu) {
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

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrScore" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

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
    elt->getGeometry ());
    */
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrPartGroup& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrPartGroup " <<
      elt->getPartGroupCombinedName () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentPartGroup = elt;
}

void msr2BsrTranslator::visitEnd (S_msrPartGroup& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrPartGroup " <<
      elt->getPartGroupCombinedName () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrPart& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  string
    partCombinedName =
      elt->getPartCombinedName ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrPart " <<
      partCombinedName <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceParts || gTraceOah->fTraceMeasures) {
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceParts || gTraceOah->fTraceMeasures) {
    fLogOutputStream <<
      "--> End visiting msrPart " <<
      elt->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrStaffTuning& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrStaffTuning" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create a staff tuning clone
  fCurrentStaffTuning = elt;
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrStaffDetails& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrStaffDetails" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentStaffTuning = nullptr;
}

void msr2BsrTranslator::visitEnd (S_msrStaffDetails& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrStaffDetails" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrStaff& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrStaff \"" <<
      elt->getStaffName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter++;

  fCurrentStaff = elt;
}

void msr2BsrTranslator::visitEnd (S_msrStaff& elt)
{
  gIndenter--;

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting S_msrStaff \"" <<
      elt->getStaffName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrVoice& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrVoice \"" <<
      elt->getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fCurrentVoice = elt;

  fRelativeOctaveReference = nullptr;

  gIndenter++;
}

void msr2BsrTranslator::visitEnd (S_msrVoice& elt)
{
  gIndenter--;

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrVoice \"" <<
      elt->getVoiceName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrVoiceStaffChange& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrVoiceStaffChange '" <<
      elt->asString () << "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrBarline& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrBarline" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceBarlines) {
    fLogOutputStream <<
      "Handling '" <<
      msrBarline::barlineCategoryKindAsString (
        elt->getBarlineCategory ()) <<
      endl;
  }
#endif

  // get the MSR barline attributes

  msrBarline::msrBarlineStyleKind
    mBarlineStyleKind =
      elt->getBarlineStyleKind ();

  // let's go

  bsrBarline::bsrBarlineKind
    bBarlineKind = bsrBarline::kBarlineKindNone;

  switch (mBarlineStyleKind) {
    case msrBarline::kBarlineStyleNone:
      break;
    case msrBarline::kBarlineStyleRegular:
      break;
    case msrBarline::kBarlineStyleDotted:
      bBarlineKind = bsrBarline::kBarlineKindSpecial;
      break;
    case msrBarline::kBarlineStyleDashed:
      bBarlineKind = bsrBarline::kBarlineKindSpecial;
      break;
    case msrBarline::kBarlineStyleHeavy:
      break;
    case msrBarline::kBarlineStyleLightLight:
      bBarlineKind = bsrBarline::kBarlineKindSectionalDouble;
      break;
    case msrBarline::kBarlineStyleLightHeavy:
      bBarlineKind = bsrBarline::kBarlineKindFinalDouble;
      break;
    case msrBarline::kBarlineStyleHeavyLight:
      break;
    case msrBarline::kBarlineStyleHeavyHeavy:
      break;
    case msrBarline::kBarlineStyleTick:
      break;
    case msrBarline::kBarlineStyleShort:
      break;
  } // switch

  if (bBarlineKind == bsrBarline::kBarlineKindNone) {
    stringstream s;

    s <<
      "MSR barline kind '" <<
      msrBarline::barlineStyleKindAsString (mBarlineStyleKind) <<
      "' is not supported in Braille music";

    notSupportedMessage (
      inputLineNumber,
      s.str ());
  }
  else {
    S_bsrBarline
      barline =
        bsrBarline::create (
          inputLineNumber, bBarlineKind);

    fCurrentMeasure->
      appendBarlineToMeasure (barline);
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
      elt->getMeasureElementMeasureNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrMeasure '" <<
      measureNumber <<
      "'" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create a measure
  fCurrentMeasure =
    bsrMeasure::create (
      inputLineNumber,
      measureNumber);

  // append it to the current line
  fCurrentLine->
    appendMeasureToLine (fCurrentMeasure);
}

void msr2BsrTranslator::visitEnd (S_msrMeasure& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrMeasure '" <<
      elt->getMeasureElementMeasureNumber () <<
      "'" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  string
    measureNumber =
      elt->getMeasureElementMeasureNumber ();
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrClef& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrClef" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (gBsrOah->fIncludeClefs) {
    bsrClef::bsrClefKind bClefKind = bsrClef::kClefKindNone;

  /* JMI
          kClefModifiedBassForRightHandPartKind,
          kClefModifiedTrebleForLeftHandPartKind };
          */

    msrClef::msrClefKind mClefKind = elt->getClefKind ();

    switch (mClefKind) {
      case msrClef::k_NoClef:
        bClefKind = bsrClef::kClefKindNone;
        break;
      case msrClef::kTrebleClef:
        bClefKind = bsrClef::kClefKindGTreble;
        break;
      case msrClef::kSopranoClef:
        bClefKind = bsrClef::kClefKindNone;
        break;
      case msrClef::kMezzoSopranoClef:
        break;
      case msrClef::kAltoClef:
        bClefKind = bsrClef::kClefKindNone;
        break;
      case msrClef::kTenorClef:
        bClefKind = bsrClef::kClefKindCTenor;
        break;
      case msrClef::kBaritoneClef:
        bClefKind = bsrClef::kClefKindCBaritone;
        break;
      case msrClef::kBassClef:
        bClefKind = bsrClef::kClefKindFBass;
        break;
      case msrClef::kTrebleLine1Clef:
        bClefKind = bsrClef::kClefKindGSoprano;
        break;
      case msrClef::kTrebleMinus15Clef:
        break;
      case msrClef::kTrebleMinus8Clef:
        bClefKind = bsrClef::kClefKindGOttavaBassa; // JMI permute???
        break;
      case msrClef::kTreblePlus8Clef:
        bClefKind = bsrClef::kClefKindGOttavaAlta; // JMI permute???
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

    if (bClefKind == bsrClef::kClefKindNone) {
      stringstream s;

      s <<
        "MSR clef kind '" <<
        msrClef::clefKindAsString (mClefKind) <<
        "' is not supported in Braille music";

      notSupportedMessage (
        inputLineNumber,
        s.str ());
    }
    else {
      S_bsrClef
        clef =
          bsrClef::create (
            inputLineNumber, bClefKind);

      fCurrentMeasure->
        appendClefToMeasure (clef);
    }
  }
}

void msr2BsrTranslator::visitEnd (S_msrClef& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrClef" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrKey& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrKey" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // get MSR key attributes

  msrKey::msrKeyKind
    mKeyKind = elt->getKeyKind ();

  msrQuarterTonesPitchKind
    mTonicQuarterTonesPitchKind =
      elt->getKeyTonicQuarterTonesPitchKind ();

  msrSemiTonesPitchKind
    mSemiTonesPitchKind =
      semiTonesPitchKindFromQuarterTonesPitchKind (
        mTonicQuarterTonesPitchKind);

  msrKey::msrKeyModeKind
    mKeyModeKind =
      elt->getKeyModeKind ();

  // let's go

  bsrKey::bsrKeyKind bKeyKind = bsrKey::kKeyKindNone;
  int                numberOfAlterations = 0;

  switch (mKeyKind) {
    case msrKey::kTraditionalKind:

      // traditional keys
      switch (mSemiTonesPitchKind) {
        case k_NoSemiTonesPitch_STP:
          break;

        case kC_TripleFlat_STP:
          break;
        case kC_DoubleFlat_STP:
          break;
        case kC_Flat_STP:
          break;
        case kC_Natural_STP:
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
        case kC_Sharp_STP:
          switch (mKeyModeKind) {
            case msrKey::kMajorMode:
              bKeyKind = bsrKey::kKeyKindSharps;
              numberOfAlterations = 7;
              break;
            case msrKey::kMinorMode:
              bKeyKind = bsrKey::kKeyKindSharps;
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
        case kC_DoubleSharp_STP:
          break;
        case kC_TripleSharp_STP:
          break;

        case kD_TripleFlat_STP:
          break;
        case kD_DoubleFlat_STP:
          break;
        case kD_Flat_STP:
          switch (mKeyModeKind) {
            case msrKey::kMajorMode:
              bKeyKind = bsrKey::kKeyKindFlats;
              numberOfAlterations = 5;
              break;
            case msrKey::kMinorMode:
              bKeyKind = bsrKey::kKeyKindFlats;
              numberOfAlterations = 8;
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
        case kD_Natural_STP:
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
        case kD_Sharp_STP:
          break;
        case kD_DoubleSharp_STP:
          break;
        case kD_TripleSharp_STP:
          break;

        case kE_TripleFlat_STP:
          break;
        case kE_DoubleFlat_STP:
          break;
        case kE_Flat_STP:
          switch (mKeyModeKind) {
            case msrKey::kMajorMode:
              bKeyKind = bsrKey::kKeyKindFlats;
              numberOfAlterations = 3;
              break;
            case msrKey::kMinorMode:
              bKeyKind = bsrKey::kKeyKindFlats;
              numberOfAlterations = 6;
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
        case kE_Natural_STP:
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
        case kE_Sharp_STP:
        case kE_DoubleSharp_STP:
          break;
        case kE_TripleSharp_STP:
          break;

        case kF_TripleFlat_STP:
          break;
        case kF_DoubleFlat_STP:
          break;
        case kF_Flat_STP:
          break;
        case kF_Natural_STP:
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
        case kF_Sharp_STP:
          switch (mKeyModeKind) {
            case msrKey::kMajorMode:
              bKeyKind = bsrKey::kKeyKindSharps;
              numberOfAlterations = 6;
              break;
            case msrKey::kMinorMode:
              bKeyKind = bsrKey::kKeyKindSharps;
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
        case kF_DoubleSharp_STP:
          break;
        case kF_TripleSharp_STP:
          break;

        case kG_TripleFlat_STP:
          break;
        case kG_DoubleFlat_STP:
          break;
        case kG_Flat_STP:
          break;
        case kG_Natural_STP:
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
        case kG_Sharp_STP:
          break;
        case kG_DoubleSharp_STP:
          break;
        case kG_TripleSharp_STP:
          break;

        case kA_TripleFlat_STP:
          break;
        case kA_DoubleFlat_STP:
          break;
        case kA_Flat_STP:
          switch (mKeyModeKind) {
            case msrKey::kMajorMode:
              bKeyKind = bsrKey::kKeyKindFlats;
              numberOfAlterations = 4;
              break;
            case msrKey::kMinorMode:
              bKeyKind = bsrKey::kKeyKindFlats;
              numberOfAlterations = 7;
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
        case kA_Natural_STP:
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
        case kA_Sharp_STP:
          break;
        case kA_DoubleSharp_STP:
          break;
        case kA_TripleSharp_STP:
          break;

        case kB_TripleFlat_STP:
          break;
        case kB_DoubleFlat_STP:
          break;
        case kB_Flat_STP:
          switch (mKeyModeKind) {
            case msrKey::kMajorMode:
              bKeyKind = bsrKey::kKeyKindFlats;
              numberOfAlterations = 2;
              break;
            case msrKey::kMinorMode:
              bKeyKind = bsrKey::kKeyKindFlats;
              numberOfAlterations = 5;
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
        case kB_Natural_STP:
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
        case kB_Sharp_STP:
          break;
        case kB_DoubleSharp_STP:
          break;
        case kB_TripleSharp_STP:
          break;
      } // switch
      break;

    case msrKey::kHumdrumScotKind:
      // JMI
      break;
  } // switch

  // create the BSR key
  if (bKeyKind == bsrKey::kKeyKindNone) {
    stringstream s;

    s <<
      "MSR key kind '" <<
      msrKey::keyKindAsString (mKeyKind) <<
      ", " <<
     msrSemiTonesPitchKindAsString (mSemiTonesPitchKind) <<
      "' is not supported in Braille music";

    notSupportedMessage (
      inputLineNumber,
      s.str ());
  }
  else {
    S_bsrKey
      key =
        bsrKey::create (
          inputLineNumber,
          bKeyKind,
          numberOfAlterations);

    if (! fFirstKey) {
      // register key in first page heading
      fFirstMusicHeading->
        setMusicHeadingKey (key);
      fFirstKey = key;
    }
    else {
      // append the BSR key to the current measure
      fCurrentLine->
        appendKeyToLine (key);
    }
  }
}

void msr2BsrTranslator::visitEnd (S_msrKey& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrKey" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrTime& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrTime" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // get MSR time attributes

  msrTime::msrTimeSymbolKind
    mTimeSymbolKind =
      elt->getTimeSymbolKind ();

/* JMI
  bool
    mTimeIsCompound =
      elt->getTimeIsCompound ();
  */

  const vector<S_msrTimeItem>&
    mTimeItemsVector =
      elt->getTimeItemsVector ();

  // let's go

  bsrTime::bsrTimeKind bTimeKind = bsrTime::kTimeNone;

  switch (mTimeSymbolKind) {
    case msrTime::kTimeSymbolNone:
      bTimeKind = bsrTime::kTimeNone;

    case msrTime::kTimeSymbolCommon:
      bTimeKind = bsrTime::kTimeCommon;
      break;
    case msrTime::kTimeSymbolCut:
      bTimeKind = bsrTime::kTimeCut;
      break;

    case msrTime::kTimeSymbolNote:
      bTimeKind = bsrTime::kTimeNote;
      break;
    case msrTime::kTimeSymbolDottedNote:
      bTimeKind = bsrTime::kTimeDottedNote;
      break;

    case msrTime::kTimeSymbolSingleNumber:
      bTimeKind = bsrTime::kTimeSingleNumber;
      break;

    case msrTime::kTimeSymbolSenzaMisura:
      bTimeKind = bsrTime::kTimeSenzaMisura;
      break;
    break;
  } // switch

  // create the BSR time
  S_bsrTime
    time =
      bsrTime::create (
        inputLineNumber,
        bTimeKind);

/*
  if (mTimeIsCompound) {
    // JMI ???
  }
  else {
  */

  if (mTimeItemsVector.size ()) {
    for (
      vector<S_msrTimeItem>::const_iterator i =
        mTimeItemsVector.begin ();
      i != mTimeItemsVector.end ();
      i++
    ) {
      S_msrTimeItem mTimeItem = (*i);

      // get the MSR time item attributes

      int
        mTimeBeatValue =
          mTimeItem->getTimeBeatValue ();

      const vector<int>&
        mTimeBeatsNumbersVector =
          mTimeItem->getTimeBeatsNumbersVector ();

      int vectorSize = mTimeBeatsNumbersVector.size ();

      for (int i = 0; i < vectorSize; i++) {
        int
          mTimeItemBeatsNumber =
            mTimeBeatsNumbersVector [i];

        // create the BSR time item
        S_bsrTimeItem
          bTimeItem =
            bsrTimeItem::create (inputLineNumber);

        // populate it
        bTimeItem->
          appendBeatsNumber (mTimeItemBeatsNumber);
        bTimeItem->
          setTimeBeatValue (mTimeBeatValue);

        // append it to the time
        time->
          appendTimeItem (bTimeItem);
      } // for
    } // for
  }

  if (! fFirstTime) {
    fFirstMusicHeading->
      // register time in first page heading
      setMusicHeadingTime (time);
    fFirstTime = time;
  }
  else {
    // this time belongs to the current measure,
    // which has already been created and appended
    // to the current line,
    // but it should be placed before it in the latter, so:
    fCurrentLine->
      insertTimeBeforeLastElementOfLine (time);
  }

  // a note octave will be needed for the next note to come,
  // i.e., the first one after a numeric indicator
  fCurrentNoteOctaveKind = bsrNote::kNoteOctaveNone;
}

void msr2BsrTranslator::visitEnd (S_msrTime& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrTime" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrTempo& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrTempo" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (! gBsrOah->fNoTempos) {
    // create the BSR tempo
    S_bsrTempo
      tempo =
        bsrTempo::create (
          inputLineNumber,
          elt);

    // append the BSR tempo to the current measure
    fCurrentLine->
      appendTempoToLine (tempo);
  }
}

void msr2BsrTranslator::visitEnd (S_msrTempo& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrTempo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
bsrNote::bsrNoteOctaveIsNeeded msr2BsrTranslator::brailleOctaveMarkInNeeded (
  S_msrNote note)
{
  bsrNote::bsrNoteOctaveIsNeeded
    result = bsrNote::kNoteOctaveIsNeededNo;

  int inputLineNumber =
    note->getInputLineNumber ();

  // select BSR octave sign relative to fRelativeOctaveReference

  // in braille music, octave number is 4 for the octave starting with middle C
  int noteAbsoluteOctave =
    note->getNoteOctave ();

  msrDiatonicPitchKind
    noteDiatonicPitchKind =
      note->
        noteDiatonicPitchKind (
          inputLineNumber);

  msrDiatonicPitchKind
    referenceDiatonicPitchKind =
      fRelativeOctaveReference->
        noteDiatonicPitchKind (
          inputLineNumber);

  string
    referenceDiatonicPitchKindAsString =
      fRelativeOctaveReference->
        noteDiatonicPitchKindAsString (
          inputLineNumber);

  int
    referenceAbsoluteOctave =
      fRelativeOctaveReference->
        getNoteOctave ();

  /*
    In a melodic progression:

    (a) the octave is not marked for the second of two consecutive notes if the interval is less than a fourth,

    (b) the octave is always marked in a skip greater than a fifth, and

    (c) the octave is only marked in a skip of a fourth or fifth when the second note is in a different octave from the first.
  */

  int
    noteAboluteDiatonicOrdinal =
      noteAbsoluteOctave * 7
        +
      noteDiatonicPitchKind - kC,

    referenceAboluteDiatonicOrdinal =
      referenceAbsoluteOctave * 7
        +
      referenceDiatonicPitchKind - kC;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotesDetails) {
    const int fieldWidth = 28;

    fLogOutputStream << left <<
/*
      setw (fieldWidth) <<
      "% referenceDiatonicPitch" <<
      " = " <<
      referenceDiatonicPitch <<
      endl <<
*/
      setw (fieldWidth) <<
      "% fRelativeOctaveReference" <<
      " = " <<
      fRelativeOctaveReference->asShortString () <<
      endl <<
      setw (fieldWidth) <<
      "% note" <<
      " = " <<
      note->asShortString () <<
      endl <<
      setw (fieldWidth) <<
      "% referenceDiatonicPitchAsString" <<
      " = " <<
      referenceDiatonicPitchKindAsString <<
      endl <<
      setw (fieldWidth) <<
      "% referenceAbsoluteOctave" <<
       " = " <<
      referenceAbsoluteOctave <<
      endl <<
      setw (fieldWidth) <<
      "% noteAbsoluteOctave" <<
       " = " <<
      noteAbsoluteOctave <<
      endl <<
      endl <<
      setw (fieldWidth) <<
      "% referenceAboluteDiatonicOrdinal" <<
      " = " <<
      referenceAboluteDiatonicOrdinal <<
      endl <<
      setw (fieldWidth) <<
      "% noteAboluteDiatonicOrdinal" <<
      " = " <<
      noteAboluteDiatonicOrdinal <<
      endl <<
      setw (fieldWidth) <<
      "% noteAboluteDiatonicOrdinal - referenceAboluteDiatonicOrdinal" <<
      " = " <<
      noteAboluteDiatonicOrdinal - referenceAboluteDiatonicOrdinal <<
      endl <<
      setw (fieldWidth) <<
      ", line " << inputLineNumber <<
      endl <<
      endl;
  }
#endif

  stringstream s;

  // should an octave sign be used?
  switch (noteAboluteDiatonicOrdinal - referenceAboluteDiatonicOrdinal) {
    case 0:
    case 1: case -1:
    case 2: case -2:
      // less than a fourth, no octave sign needed
      break;
    case 3: case -3:
    case 4: case -4:
      // a fourth or fifth, octave sign needed if there an octave change
      if (noteAbsoluteOctave == referenceAbsoluteOctave) {
        result = bsrNote::kNoteOctaveIsNeededNo;
      }
      else {
        result = bsrNote::kNoteOctaveIsNeededYes;
      }
      break;
    default:
      // more that a fifth, use an octave sign
      result = bsrNote::kNoteOctaveIsNeededYes;
  } // switch

  return result;
}

void msr2BsrTranslator::createBsrForNote (S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

  // get MSR note attributes

  msrQuarterTonesPitchKind
    noteQuarterTonesPitchKind =
      note->getNoteQuarterTonesPitchKind ();

/* JMI
  msrSemiTonesPitchKind
    noteSemiTonesPitchKind =
      semiTonesPitchKindFromQuarterTonesPitchKind (
        noteQuarterTonesPitchKind);
        */

  msrDiatonicPitchKind
    noteDiatonicPitchKind =
      diatonicPitchKindFromQuarterTonesPitchKind (
        inputLineNumber,
        noteQuarterTonesPitchKind);

  msrNote::msrNoteAccidentalKind
    mNoteAccidentalKind =
      note->getNoteAccidentalKind ();

  int noteDotsNumber = note->getNoteDotsNumber ();

  msrDurationKind
    noteGraphicDurationKind =
      note->getNoteGraphicDurationKind ();

  int noteOctave = note->getNoteOctave ();

  // let's go

  // braille the note words if any,
  // which should precede the articulations in LilyPond
  // generate code for the words only
  // if note doesn't belong to a chord,
  // otherwise it will be generated for the chord itself
  if (! note->getNoteBelongsToAChord ()) {
    const list<S_msrWords>&
      noteWords =
        note->getNoteWords ();

    if (noteWords.size ()) {
      list<S_msrWords>::const_iterator i;

      for (
        i=noteWords.begin ();
        i!=noteWords.end ();
        i++
      ) {
        msrPlacementKind
          wordsPlacementKind =
            (*i)->getWordsPlacementKind ();

        string wordsContents =
          (*i)->getWordsContents ();

        msrFontStyleKind
          wordsFontStyleKind =
            (*i)->getWordsFontStyleKind ();

        S_msrFontSize
          wordsFontSize =
            (*i)->getWordsFontSize ();

        msrFontWeightKind
          wordsFontWeightKind =
            (*i)->getWordsFontWeightKind ();

        {
          switch (wordsPlacementKind) {
            case kPlacementNone:
              break;
            case kPlacementAbove:
              break;
            case kPlacementBelow:
              break;
          } // switch

          switch (wordsFontStyleKind) {
            case kFontStyleNone:
              break;
            case kFontStyleNormal:
              break;
            case KFontStyleItalic:
              break;
          } // switch

          switch (wordsFontWeightKind) {
            case kFontWeightNone:
              break;
            case kFontWeightNormal:
              break;
            case kFontWeightBold:
              break;
          } // switch

          switch (wordsFontSize->getFontSizeKind ()) {
            case msrFontSize::kFontSizeNone:
              break;
            case msrFontSize::kFontSizeXXSmall:
              break;
            case msrFontSize::kFontSizeXSmall:
              break;
            case msrFontSize::kFontSizeSmall:
              break;
            case msrFontSize::kFontSizeMedium:
              break;
            case msrFontSize::kFontSizeLarge:
              break;
            case msrFontSize::kFontSizeXLarge:
              break;
            case msrFontSize::kFontSizeXXLarge:
              break;
            case msrFontSize::kFontSizeNumeric:
              break;
          } // switch

          // create the words string
          S_bsrWords
            bWords =
              bsrWords::create (
                inputLineNumber, wordsContents);

          // append it to the current measure
          fCurrentMeasure->
            appendWordsToMeasure (bWords);

          // a note octave will be needed for the next note to come,
          // i.e., the first one after the word sign
          fCurrentNoteOctaveKind = bsrNote::kNoteOctaveNone;
        }
      } // for
    }
  }

  bsrNote::bsrNoteOctaveKind
    noteOctaveKind =
      bsrNote::kNoteOctaveNone;

  // middle C starts octave 4, as in MusicXML
  switch (noteOctave) {
    case 1: noteOctaveKind = bsrNote::kNoteOctave1; break;
    case 2: noteOctaveKind = bsrNote::kNoteOctave2; break;
    case 3: noteOctaveKind = bsrNote::kNoteOctave3; break;
    case 4: noteOctaveKind = bsrNote::kNoteOctave4; break;
    case 5: noteOctaveKind = bsrNote::kNoteOctave5; break;
    case 6: noteOctaveKind = bsrNote::kNoteOctave6; break;
    case 7: noteOctaveKind = bsrNote::kNoteOctave7; break;
    default:
      // kNoteOctaveBelow1Kind and kNoteOctaveAbove7Kind
      // cannot occur in MusicXML
      ;
  } // switch

  bsrNote::bsrNoteValueKind noteValueKind = bsrNote::kNoteValueNone;

  if (note->getNoteIsARest ()) {
    switch (noteGraphicDurationKind) {
      case k_NoDuration:
        break;

      case k1024th:
        break;
      case k512th:
        break;
      case k256th:
        noteValueKind = bsrNote::kNoteValueRest256th;
        break;
      case k128th:
        noteValueKind = bsrNote::kNoteValueRest128th;
        break;
      case k64th:
        noteValueKind = bsrNote::kNoteValueRest64th;
        break;
      case k32nd:
        noteValueKind = bsrNote::kNoteValueRest32nd;
        break;
      case k16th:
        noteValueKind = bsrNote::kNoteValueRest16th;
        break;
      case kEighth:
        noteValueKind = bsrNote::kNoteValueRest8th;
        break;
      case kQuarter:
        noteValueKind = bsrNote::kNoteValueRestQuarter;
        break;
      case kHalf:
        noteValueKind = bsrNote::kNoteValueRestHalf;
        break;
      case kWhole:
        noteValueKind = bsrNote::kNoteValueRestWhole;
        break;
      case kBreve:
        noteValueKind = bsrNote::kNoteValueRestBreve;
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
            noteValueKind = bsrNote::kNoteValueA256th;
            break;
          case k128th:
            noteValueKind = bsrNote::kNoteValueA128th;
            break;
          case k64th:
            noteValueKind = bsrNote::kNoteValueA64th;
            break;
          case k32nd:
            noteValueKind = bsrNote::kNoteValueA32nd;
            break;
          case k16th:
            noteValueKind = bsrNote::kNoteValueA16th;
            break;
          case kEighth:
            noteValueKind = bsrNote::kNoteValueA8th;
            break;
          case kQuarter:
            noteValueKind = bsrNote::kNoteValueAQuarter;
            break;
          case kHalf:
            noteValueKind = bsrNote::kNoteValueAHalf;
            break;
          case kWhole:
            noteValueKind = bsrNote::kNoteValueAWhole;
            break;
          case kBreve:
            noteValueKind = bsrNote::kNoteValueABreve;
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
            noteValueKind = bsrNote::kNoteValueB256th;
            break;
          case k128th:
            noteValueKind = bsrNote::kNoteValueB128th;
            break;
          case k64th:
            noteValueKind = bsrNote::kNoteValueB64th;
            break;
          case k32nd:
            noteValueKind = bsrNote::kNoteValueB32nd;
            break;
          case k16th:
            noteValueKind = bsrNote::kNoteValueB16th;
            break;
          case kEighth:
            noteValueKind = bsrNote::kNoteValueB8th;
            break;
          case kQuarter:
            noteValueKind = bsrNote::kNoteValueBQuarter;
            break;
          case kHalf:
            noteValueKind = bsrNote::kNoteValueBHalf;
            break;
          case kWhole:
            noteValueKind = bsrNote::kNoteValueBWhole;
            break;
          case kBreve:
            noteValueKind = bsrNote::kNoteValueBBreve;
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
            noteValueKind = bsrNote::kNoteValueC256th;
            break;
          case k128th:
            noteValueKind = bsrNote::kNoteValueC128th;
            break;
          case k64th:
            noteValueKind = bsrNote::kNoteValueC64th;
            break;
          case k32nd:
            noteValueKind = bsrNote::kNoteValueC32nd;
            break;
          case k16th:
            noteValueKind = bsrNote::kNoteValueC16th;
            break;
          case kEighth:
            noteValueKind = bsrNote::kNoteValueC8th;
            break;
          case kQuarter:
            noteValueKind = bsrNote::kNoteValueCQuarter;
            break;
          case kHalf:
            noteValueKind = bsrNote::kNoteValueCHalf;
            break;
          case kWhole:
            noteValueKind = bsrNote::kNoteValueCWhole;
            break;
          case kBreve:
            noteValueKind = bsrNote::kNoteValueCBreve;
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
            noteValueKind = bsrNote::kNoteValueD256th;
            break;
          case k128th:
            noteValueKind = bsrNote::kNoteValueD128th;
            break;
          case k64th:
            noteValueKind = bsrNote::kNoteValueD64th;
            break;
          case k32nd:
            noteValueKind = bsrNote::kNoteValueD32nd;
            break;
          case k16th:
            noteValueKind = bsrNote::kNoteValueD16th;
            break;
          case kEighth:
            noteValueKind = bsrNote::kNoteValueD8th;
            break;
          case kQuarter:
            noteValueKind = bsrNote::kNoteValueDQuarter;
            break;
          case kHalf:
            noteValueKind = bsrNote::kNoteValueDHalf;
            break;
          case kWhole:
            noteValueKind = bsrNote::kNoteValueDWhole;
            break;
          case kBreve:
            noteValueKind = bsrNote::kNoteValueDBreve;
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
            noteValueKind = bsrNote::kNoteValueE256th;
            break;
          case k128th:
            noteValueKind = bsrNote::kNoteValueE128th;
            break;
          case k64th:
            noteValueKind = bsrNote::kNoteValueE64th;
            break;
          case k32nd:
            noteValueKind = bsrNote::kNoteValueE32nd;
            break;
          case k16th:
            noteValueKind = bsrNote::kNoteValueE16th;
            break;
          case kEighth:
            noteValueKind = bsrNote::kNoteValueE8th;
            break;
          case kQuarter:
            noteValueKind = bsrNote::kNoteValueEQuarter;
            break;
          case kHalf:
            noteValueKind = bsrNote::kNoteValueEHalf;
            break;
          case kWhole:
            noteValueKind = bsrNote::kNoteValueEWhole;
            break;
          case kBreve:
            noteValueKind = bsrNote::kNoteValueEBreve;
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
            noteValueKind = bsrNote::kNoteValueF256th;
            break;
          case k128th:
            noteValueKind = bsrNote::kNoteValueF128th;
            break;
          case k64th:
            noteValueKind = bsrNote::kNoteValueF64th;
            break;
          case k32nd:
            noteValueKind = bsrNote::kNoteValueF32nd;
            break;
          case k16th:
            noteValueKind = bsrNote::kNoteValueF16th;
            break;
          case kEighth:
            noteValueKind = bsrNote::kNoteValueF8th;
            break;
          case kQuarter:
            noteValueKind = bsrNote::kNoteValueFQuarter;
            break;
          case kHalf:
            noteValueKind = bsrNote::kNoteValueFHalf;
            break;
          case kWhole:
            noteValueKind = bsrNote::kNoteValueFWhole;
            break;
          case kBreve:
            noteValueKind = bsrNote::kNoteValueFBreve;
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
            noteValueKind = bsrNote::kNoteValueG256th;
            break;
          case k128th:
            noteValueKind = bsrNote::kNoteValueG128th;
            break;
          case k64th:
            noteValueKind = bsrNote::kNoteValueG64th;
            break;
          case k32nd:
            noteValueKind = bsrNote::kNoteValueG32nd;
            break;
          case k16th:
            noteValueKind = bsrNote::kNoteValueG16th;
            break;
          case kEighth:
            noteValueKind = bsrNote::kNoteValueG8th;
            break;
          case kQuarter:
            noteValueKind = bsrNote::kNoteValueGQuarter;
            break;
          case kHalf:
            noteValueKind = bsrNote::kNoteValueGHalf;
            break;
          case kWhole:
            noteValueKind = bsrNote::kNoteValueGWhole;
            break;
          case kBreve:
            noteValueKind = bsrNote::kNoteValueGBreve;
            break;
          case kLong:
            break;
          case kMaxima:
            break;
        } // switch
        break;
    } // switch
  }

  // is the note octave needed?
  bsrNote::bsrNoteOctaveIsNeeded
    noteOctaveIsNeeded =
      bsrNote::kNoteOctaveIsNeededNo;

  if (fCurrentNoteOctaveKind == bsrNote::kNoteOctaveNone) {
    // this note is the first one in the voice,
    // or a preceding braille element forces the octave sign for it
    // unless it is a rest
    if (note->getNoteIsARest ()) {
      noteOctaveIsNeeded =
        bsrNote::kNoteOctaveIsNeededNo;
    }
    else {
      noteOctaveIsNeeded =
        bsrNote::kNoteOctaveIsNeededYes;
    }
  }
  else if (fRelativeOctaveReference) {
    // analyye relationship to relative octave reference
    noteOctaveIsNeeded =
      brailleOctaveMarkInNeeded (note);
  }

  // register note's octave kind
  fCurrentNoteOctaveKind = noteOctaveKind;

  // is there an accidental attached to the note?
  bsrNote::bsrNoteAccidentalKind
    bNoteAccidentalKind =
      bsrNote::kNoteAccidentalNone;

  switch (mNoteAccidentalKind) {
    case msrNote::kNoteAccidentalNone:
      break;

    case msrNote::kNoteAccidentalSharp:
      bNoteAccidentalKind = bsrNote::kNoteAccidentalSharp;
      break;
    case msrNote::kNoteAccidentalNatural:
      bNoteAccidentalKind = bsrNote::kNoteAccidentalNatural;
      break;
    case msrNote::kNoteAccidentalFlat:
      bNoteAccidentalKind = bsrNote::kNoteAccidentalFlat;
      break;

    case msrNote::kNoteAccidentalDoubleSharp:
      bNoteAccidentalKind = bsrNote::kNoteAccidentalDoubleSharp;
      break;
    case msrNote::kNoteAccidentalSharpSharp:
      break;

    case msrNote::kNoteAccidentalFlatFlat:
      bNoteAccidentalKind = bsrNote::kNoteAccidentalDoubleFlat;
      break;
    case msrNote::kNoteAccidentalNaturalSharp:
      break;
    case msrNote::kNoteAccidentalNaturalFlat:
      break;

    case msrNote::kNoteAccidentalQuarterFlat:
      bNoteAccidentalKind = bsrNote::kNoteAccidentalDoubleFlat;
      break;
    case msrNote::kNoteAccidentalQuarterSharp:
      bNoteAccidentalKind = bsrNote::kNoteAccidentalQuarterSharp;
      break;

    case msrNote::kNoteAccidentalThreeQuartersFlat:
      bNoteAccidentalKind = bsrNote::kNoteAccidentalThreeQuarterFlat;
      break;
    case msrNote::kNoteAccidentalThreeQuartersSharp:
      bNoteAccidentalKind = bsrNote::kNoteAccidentalThreeQuarterSharp;
      break;

    case msrNote::kNoteAccidentalSharpDown:
      break;
    case msrNote::kNoteAccidentalSharpUp:
      break;
    case msrNote::kNoteAccidentalNaturalDown:
      break;
    case msrNote::kNoteAccidentalNaturalUp:
      break;
    case msrNote::kNoteAccidentalFlatDown:
      break;
    case msrNote::kNoteAccidentalFlatUp:
      break;
    case msrNote::kNoteAccidentalTripleSharp:
      break;
    case msrNote::kNoteAccidentalTripleFlat:
      break;
    case msrNote::kNoteAccidentalSlashQuarterSharp:
      break;
    case msrNote::kNoteAccidentalSlashSharp:
      break;
    case msrNote::kNoteAccidentalSlashFlat:
      break;
    case msrNote::kNoteAccidentaldoubleSlashFlat:
      break;
    case msrNote::kNoteAccidentalSharp_1:
      break;
    case msrNote::kNoteAccidentalSharp_2:
      break;
    case msrNote::kNoteAccidentalSharp_3:
      break;
    case msrNote::kNoteAccidentalSharp_5:
      break;
    case msrNote::kNoteAccidentalFlat_1:
      break;
    case msrNote::kNoteAccidentalFlat_2:
      break;
    case msrNote::kNoteAccidentalFlat_3:
      break;
    case msrNote::kNoteAccidentalFlat_4:
      break;
    case msrNote::kNoteAccidentalSori:
      break;
    case msrNote::kNoteAccidentalKoron:
      break;
  } // switch

  // create the note
  S_bsrNote bNote =
    bsrNote::create (
      inputLineNumber,
      noteValueKind,
      noteDotsNumber,
      noteOctaveKind,
      noteOctaveIsNeeded,
      bNoteAccidentalKind);

  // append it to the current measure
  fCurrentMeasure->
    appendNoteToMeasure (bNote);

  // determine the note value size kind
  bsrNote::bsrNoteValueSizeKind
    noteValueSizeKind =
      bsrNote::noteValueSizeKindFromNoteValueKind (
        noteValueKind);

  // is a note value sign needed?
#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotesDetails) {
    fLogOutputStream <<
      "--> fCurrentNoteValueSizeKind = " <<
      bsrNote::noteValueSizeKindAsString (fCurrentNoteValueSizeKind) <<
      ", noteValueSizeKind = " <<
      bsrNote::noteValueSizeKindAsString (noteValueSizeKind) <<
      ", noteOctaveIsNeeded = " <<
      bsrNote::noteOctaveIsNeededAsString (noteOctaveIsNeeded) <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (false && noteValueSizeKind != fCurrentNoteValueSizeKind) { // JMI
    fLogOutputStream <<
      "--> note = '" <<
      bNote->asShortString () <<
      "', needs a note value size sign" <<
      endl;

    // set the note value size kind as needed
    bNote->setNoteValueSizeIsNeeded ();
    // register new note value size kind
    fCurrentNoteValueSizeKind = noteValueSizeKind;
  }
}

void msr2BsrTranslator::visitStart (S_msrNote& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrNote '" <<
      elt->asString () <<
      "'" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  createBsrForNote (elt);

  fRelativeOctaveReference = elt;
}

void msr2BsrTranslator::visitEnd (S_msrNote& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrNote " <<
      elt->asString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrDynamics& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrDynamics" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create the dynamics
  S_bsrDynamics
    dynamics =
      bsrDynamics::create (
        elt->getInputLineNumber (),
        elt->getDynamicsKind ());

  // append it to the current measure
  fCurrentMeasure->
    appendDynamicsToMeasure (dynamics);
}

void msr2BsrTranslator::visitEnd (S_msrDynamics& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrDynamics" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

/*
//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrIdentification& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrIdentification" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrIdentification" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}
*/

/*
//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrCredit& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrCredit" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentCredit = elt;

  // set elt as credit of the MSR score part of the BSR score
  fBsrScore->
    getMsrScore ()->
      appendCreditToScore (fCurrentCredit);
}

void msr2BsrTranslator::visitEnd (S_msrCredit& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrCredit" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentCredit = nullptr;
}

void msr2BsrTranslator::visitStart (S_msrCreditWords& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrCreditWords" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // don't append it to the current credit, since the latter is no clone
  / * JMI
  fCurrentCredit->
    appendCreditWordsToCredit (
      elt);
      * /
}

void msr2BsrTranslator::visitEnd (S_msrCreditWords& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrCreditWords" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrSegment& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
    appendSegmentToVoiceClone (
      segmentClone);
}

void msr2BsrTranslator::visitEnd (S_msrSegment& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // forget current segment clone
  fCurrentSegmentClonesStack.pop ();
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrHarmony& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrHarmony '" <<
      elt->asString () <<
      ", fOnGoingNote = " << booleanAsString (fOnGoingNote) <<
      ", fOnGoingChord = " << booleanAsString (fOnGoingChord) <<
      ", fOnGoingHarmonyVoice = " << booleanAsString (fOnGoingHarmonyVoice) <<
      "', line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create a harmony new born clone
  fCurrentHarmonyClone =
    elt->createHarmonyNewbornClone (
      fCurrentVoiceClone);

  if (fOnGoingNote) {
    // register the harmony in the current non-grace note clone
    fCurrentNonGraceNoteClone->
      appendHarmonyToNoteHarmoniesList (
        fCurrentHarmonyClone);

  // don't append the harmony to the part harmony,
  // this has been done in pass2b
  }

  else if (fOnGoingChord) {
    // register the harmony in the current chord clone
    fCurrentChordClone->
      appendHarmonyToChord (fCurrentHarmonyClone); // JMI
  }

  else if (fOnGoingHarmonyVoice) {
    fCurrentVoiceClone->
      appendHarmonyToVoiceClone (
        fCurrentHarmonyClone);
  }
}

void msr2BsrTranslator::visitStart (S_msrHarmonyDegree& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_msrHarmonyDegree '" <<
      elt->asString () <<
      ", fOnGoingNote = " << booleanAsString (fOnGoingNote) <<
      ", fOnGoingChord = " << booleanAsString (fOnGoingChord) <<
      ", fOnGoingHarmonyVoice = " << booleanAsString (fOnGoingHarmonyVoice) <<
      "', line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append the harmony degree to the current harmony clone
  fCurrentHarmonyClone->
    appendHarmonyDegreeToHarmony (
      elt);
}

void msr2BsrTranslator::visitEnd (S_msrHarmony& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrHarmony '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrFrame& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrFrame '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNote) {
    // register the frame in the current non-grace note clone
    fCurrentNonGraceNoteClone->
      setNoteFrame (elt);
  }
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrFiguredBass& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrFiguredBass '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create a deep copy of the figured bass
  fCurrentFiguredBass =
    elt->
      createFiguredBassDeepCopy (
        fCurrentPartClone);

  if (fOnGoingNote) {
    // append the figured bass to the current non-grace note clone
    fCurrentNonGraceNoteClone->
      appendFiguredBassToNoteFiguredBassesList (fCurrentFiguredBassClone);

    // don't append the figured bass to the part figured bass,  JMI ???
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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrFigure '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append the figure to the current figured bass
  fCurrentFiguredBass->
    appendFigureToFiguredBass (
      elt);
}

void msr2BsrTranslator::visitEnd (S_msrFiguredBass& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrFiguredBass '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
      elt->getMeasureElementMeasureNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrMeasure '" <<
      measureNumber <<
      "'" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

/ * JMI
  {
    fLogOutputStream <<
      endl <<
      elt <<
      endl;
  }
     * /

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
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
        getMeasureSegmentUpLink ()->
          getSegmentVoiceUpLink ();

  // get the measure number
  string
    measureNumber =
      fCurrentMeasureClone->
        getMeasureElementMeasureNumber ();

  // get the current measure whole notes
  rational
    currentMeasureWholeNotesDuration =
      fCurrentMeasureClone->
        getCurrentMeasureWholeNotesDuration ();

  // get the full current measure whole notes
  rational
    measureFullLength =
      fCurrentMeasureClone->
        getMeasureFullLength ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    fLogOutputStream <<
      "Finalizing measure '" << measureNumber <<
      "' in voice \"" << voice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl <<
      "currentMeasureWholeNotesDuration = " << currentMeasureWholeNotesDuration <<
      endl;
  }
#endif

  msrMeasure::msrMeasureKind
    measureKind =
      msrMeasure::kUnknownMeasure; // JMI
 // JMI     fMeasureKind = kFullMeasure; // may be changed afterwards

  if (currentMeasureWholeNotesDuration == measureFullLength ) {
    // this measure is full
    measureKind =
      msrMeasure::kFullMeasure;
  }

  else if (currentMeasureWholeNotesDuration < measureFullLength) {
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
      msrMeasure::kUpbeatMeasure; // JMI
  }

  else if (currentMeasureWholeNotesDuration > measureFullLength) {
    // this measure is overfull
    measureKind =
      msrMeasure::kOverfullMeasure;
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
      gOahOah->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2BsrTranslator::visitEnd (S_msrMeasure& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrMeasure '" <<
      elt->getMeasureElementMeasureNumber () <<
      "'" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  string
    measureNumber =
      elt->getMeasureElementMeasureNumber ();

  finalizeCurrentMeasureClone ( // JMI
    inputLineNumber,
    elt); // original measure

  bool doCreateABarCheck = false; // JMI

  switch (elt->getMeasureKind ()) {

    case msrMeasure::kMeasureKindUnknown:
      {
        stringstream s;

        s <<
          "measure '" << measureNumber <<
          "' in voice \"" <<
          elt->
            fetchMeasureVoiceUpLink ()->
              getVoiceName () <<
          "\" is of unknown kind in msr2BsrTranslator";

      // JMI  msrInternalError (
        msrInternalWarning (
          gOahOah->fInputSourceName,
          inputLineNumber,
  //        __FILE__, __LINE__,
          s.str ());
      }
      break;

    case msrMeasure::kMeasureKindRegular:
      doCreateABarCheck = true;
      break;

    case msrMeasure::kMeasureKindAnacrusis:
      doCreateABarCheck = true;
      break;

    case msrMeasure::kMeasureKindIncompleteStandalone:
      doCreateABarCheck = true;
      break;
    case msrMeasure::kMeasureKindIncompleteLastInRepeat:
      doCreateABarCheck = true;
      break;
    case msrMeasure::kMeasureKindIncompleteAfterRepeat:
      doCreateABarCheck = true;
      break;

    case msrMeasure::kMeasureKindOvercomplete:
      doCreateABarCheck = true;
      break;

    case msrMeasure::kMeasureKindCadenza:
      doCreateABarCheck = true;
      break;

    case msrMeasure::kMeasureKindEmpty:
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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // forget about this stanza
  fCurrentStanzaClone = nullptr;

  fOnGoingStanza = false;
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrSyllable& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrSyllable" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

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
      appendSyllableToNoteAndSetItsNoteUpLink (
        fCurrentNonGraceNoteClone);

    if (gBsrOah->fAddWordsFromTheLyrics) {
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
#ifdef TRACE_OAH
        if (gTraceOah->fTraceLyrics || gTraceOah->fTraceWords) {
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
#ifdef TRACE_OAH
        if (gTraceOah->fTraceLyrics || gTraceOah->fTraceWords) {
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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrSyllable" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}
*/

/*
//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrTranspose& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrTranspose" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append transpose to voice clone
  fCurrentVoiceClone->
    appendTransposeToVoice (elt);
}

void msr2BsrTranslator::visitEnd (S_msrTranspose& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrTranspose" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrPartNameDisplay& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrPartNameDisplay" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append part name display to voice clone
  fCurrentVoiceClone->
    appendPartNameDisplayToVoice (elt);
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrPartAbbreviationDisplay& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrPartAbbreviationDisplay" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append part abbreviation display to voice clone
  fCurrentVoiceClone->
    appendPartAbbreviationDisplayToVoice (elt);
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrRehearsal& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrRehearsal" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendRehearsalToVoice (elt);
}

void msr2BsrTranslator::visitEnd (S_msrRehearsal& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrRehearsal" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrArticulation& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrArticulation" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrArticulation" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrFermata& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrFermata" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrArpeggiato" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrNonArpeggiato" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrTechnical" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrTechnical" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrTechnicalWithInteger& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrTechnicalWithInteger" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrTechnicalWithInteger" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrTechnicalWithFloat& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrTechnicalWithFloat" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrTechnicalWithFloat" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrTechnicalWithString& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrTechnicalWithString" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrTechnicalWithString" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrOrnament& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrOrnament" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrOrnament" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrSpanner& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrSpanner" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrSpanner" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrGlissando& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrGlissando" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrGlissando" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrSlide& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrSlide" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrSlide" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrSingleTremolo& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrSingleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrSingleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrDoubleTremolo& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrDoubleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrSingleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append the current double tremolo clone to the current voice clone
  fCurrentVoiceClone->
    appendDoubleTremoloToVoice (
      fCurrentDoubleTremoloClone);

  // forget about it
  fCurrentDoubleTremoloClone = nullptr;

  fOnGoingDoubleTremolo = false;
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrOtherDynamics& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrOtherDynamics" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrOtherDynamics" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrWords& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrWords" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrWords" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrSlur& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrSlur" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrSlur" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrLigature& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrLigature" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrLigature" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrSlash& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrSlash" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrWedge" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrWedge" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrGraceNotesGroup& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber () ;

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrGraceNotesGroup" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  bool doCreateAGraceNoteClone = true; // JMI

  if (doCreateAGraceNoteClone) {
    // create a clone of this graceNotesGroup
#ifdef TRACE_OAH
    if (gTraceOah->fTraceNotes || gTraceOah->fTraceGraceNotes) {
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotes || gTraceOah->fTraceGraceNotes) {
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
    fLogOutputStream << endl;
  }
#endif

  // get the note this grace notes group is attached to
  S_msrNote
    noteNotesGroupIsAttachedTo =
      elt->
        getGraceNotesGroupNoteUpLink ();

  if (! noteNotesGroupIsAttachedTo) {
    stringstream s;

    s <<
      "grace notes group '" << elt->asShortString () <<
      "' has an empty note upLink";

    msrInternalError (
      gOahOah->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fOnGoingGraceNotesGroup = true;

  // is noteNotesGroupIsAttachedTo the first one in its voice?
#ifdef TRACE_OAH
  if (
    gTraceOah->fTraceGraceNotes
      ||
    gTraceOah->fTraceNotes
      ||
    gTraceOah->fTraceVoices
  ) {
    fLogOutputStream <<
      "The noteNotesGroupIsAttachedTo voice clone PEOJIOFEIOJEF '" <<
      fCurrentVoiceClone->getVoiceName () <<
      "' is '";

    if (noteNotesGroupIsAttachedTo) {
      fLogOutputStream <<
        noteNotesGroupIsAttachedTo->asShortString () const;
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

#ifdef TRACE_OAH
  if (
    gTraceOah->fTraceGraceNotes
      ||
    gTraceOah->fTraceNotes
      ||
    gTraceOah->fTraceVoices
  ) {
    fLogOutputStream <<
      "The first note of voice clone KLJWLPOEF '" <<
      fCurrentVoiceClone->getVoiceName () <<
      "' is '";

    if (fFirstNoteCloneInVoice) {
      fLogOutputStream <<
        fFirstNoteCloneInVoice->asShortString () const;
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
#ifdef TRACE_OAH
        if (
            gTraceOah->fTraceGraceNotes
              ||
            gTraceOah->fTraceNotes
              ||
            gTraceOah->fTraceVoices
        ) {
          fLogOutputStream <<
            "Creating a skip clone of grace notes group '" <<
            elt->asShortString () <<
            "' to work around LilyPond issue #34" <<
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

#ifdef TRACE_OAH
      if (gTraceOah->fTraceGraceNotesGroup) {
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
#ifdef TRACE_OAH
      if (gTraceOah->fTraceGraceNotesGroup) {
        fLogOutputStream <<
          "Creating a skip clone of grace notes '" <<
          elt->asShortString () <<
          "' to work around LilyPond issue #34" <<
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
#ifdef TRACE_OAH
      if (gTraceOah->fTraceGraceNotesGroup) {
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
#ifdef TRACE_OAH
      if (gTraceOah->fTraceGraceNotesGroup) {
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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrGraceNotesGroup" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

#ifdef TRACE_OAH
    if (gTraceOah->fTraceNotes || gTraceOah->fTraceGraceNotes) {
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
    fLogOutputStream << endl;
  }
#endif

  // forget about these grace notes
  fCurrentGraceNotesGroupClone = nullptr;

  fOnGoingGraceNotesGroup = false;

/ * JMI
  if (fPendingAfterGraceNotesGroup) {
    // remove the current afterGraceNotesGroup note clone
    // from the current voice clone
#ifdef TRACE_OAH
    if (gTraceOah->fTraceGraceNotesGroup) {
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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrNote '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
#ifdef TRACE_OAH
        if (gTraceOah->fTraceNotes || gTraceOah->fTraceVoices) {
          fLogOutputStream <<
            "The first note of voice clone GFFF '" <<
            fCurrentVoiceClone->getVoiceName () <<
            "' is '";

          if (fFirstNoteCloneInVoice) {
            fLogOutputStream <<
              fFirstNoteCloneInVoice->asShortString () const;
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

#ifdef TRACE_OAH
        if (gTraceOah->fTraceNotes || gTraceOah->fTraceVoices) {
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
#ifdef TRACE_OAH
    if (gTraceOah->fTraceNotesGroup) {
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

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrNote " <<
      elt->asString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotesDetails) {
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
#ifdef TRACE_OAH
      if (gTraceOah->fTraceNotes) {
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
#ifdef TRACE_OAH
      if (gTraceOah->fTraceNotes) {
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
#ifdef TRACE_OAH
      if (gTraceOah->fTraceNotes) {
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

    case msrNote::kRegularNote:
#ifdef TRACE_OAH
      if (gTraceOah->fTraceNotes) {
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
#ifdef TRACE_OAH
          if (gTraceOah->fTraceNotes) {
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
#ifdef TRACE_OAH
          if (gTraceOah->fTraceNotes) {
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
            gOahOah->fInputSourceName,
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
          gOahOah->fInputSourceName,
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
          gOahOah->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      else {
#ifdef TRACE_OAH
        if (gTraceOah->fTraceGraceNotes || gTraceOah->fTraceNotes) {
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
#ifdef TRACE_OAH
        if (gTraceOah->fTraceGraceNotes || gTraceOah->fTraceNotes) {
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
#ifdef TRACE_OAH
        if (gTraceOah->fTraceGraceNotes || gTraceOah->fTraceNotes) {
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
          gOahOah->fInputSourceName,
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
          gOahOah->fInputSourceName,
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
          gOahOah->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
        }
      break;

    case msrNote::kTupletMemberNote:
    case msrNote::kGraceTupletMemberNote:
    case msrNote::kTupletMemberUnpitchedNote:
#ifdef TRACE_OAH
      if (gTraceOah->fTraceNotes) {
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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrOctaveShift" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    setNoteOctaveShift (elt);
}

void msr2BsrTranslator::visitEnd (S_msrOctaveShift& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrOctaveShift" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrAccordionRegistration& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrAccordionRegistration" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrHarpPedalsTuning" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append the harp pedals tuning to the voice clone
  fCurrentVoiceClone->
    appendHarpPedalsTuningToVoice (elt);
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrStem& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrStem" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrStem" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrBeam& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrBeam" <<
      ", line " << elt->getInputLineNumber () <<
// JMI      ", fOnGoingNote = " << booleanAsString (fOnGoingNote) <<
// JMI      ", fOnGoingChord = " << booleanAsString (fOnGoingChord) <<
      endl;
  }
#endif

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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrBeam" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrChord& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrChord" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

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
        gOahOah->fInputSourceName,
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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrChord" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fOnGoingChord = false;
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrTuplet& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrTuplet" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create the tuplet clone
  S_msrTuplet
    tupletClone =
      elt->createTupletNewbornClone ();

  // register it in this visitor
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTuplets) {
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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrTuplet" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceTuplets) {
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
#ifdef TRACE_OAH
    if (gTraceOah->fTraceTuplets) {
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

#ifdef TRACE_OAH
    if (gTraceOah->fTraceTuplets) {
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
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrTie" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    setNoteTie (elt);
}

void msr2BsrTranslator::visitEnd (S_msrTie& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrTie" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrSegno& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrSegno" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendSegnoToVoice (elt);
}

void msr2BsrTranslator::visitStart (S_msrCoda& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrCoda" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendCodaToVoice (elt);
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrEyeGlasses& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting eyeGlasses" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendEyeGlassesToNote (elt);
}

void msr2BsrTranslator::visitStart (S_msrScordatura& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting scordatura" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendScordaturaToNote (elt);
}

void msr2BsrTranslator::visitStart (S_msrPedal& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting pedal" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendPedalToNote (elt);
}

void msr2BsrTranslator::visitStart (S_msrDamp& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting damp" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendDampToNote (elt);

  fBsrScore->
    // this score needs the 'custom short barline' Scheme function
    setDampMarkupIsNeeded ();
}

void msr2BsrTranslator::visitStart (S_msrDampAll& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting dampAll" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendDampAllToNote (elt);

  fBsrScore->
    // this score needs the 'custom short barline' Scheme function
    setDampAllMarkupIsNeeded ();
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrBarCheck& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrBarCheck" <<
      ", nextBarNumber: " <<
      elt->getNextBarNumber () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendBarCheckToVoice (elt);
}

void msr2BsrTranslator::visitEnd (S_msrBarCheck& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrBarCheck" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrBarNumberCheck& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrBarNumberCheck" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendBarNumberCheckToVoice (elt);
}

void msr2BsrTranslator::visitEnd (S_msrBarNumberCheck& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrBarNumberCheck" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrRepeat" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    fLogOutputStream <<
      "Handling repeat start in part clone" <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    fLogOutputStream <<
      "Creating a repeat newborn clone" <<
      ", line " << inputLineNumber <<
      ", in voice \"" <<
      elt->
        getRepeatVoiceUpLink ()->
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
}

void msr2BsrTranslator::visitEnd (S_msrRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrRepeat" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

/ * JMI
  // append the repeat clone to the current part clone
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
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
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrRepeatCommonPart& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrRepeatCommonPart" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (
    gTraceOah->fTraceRepeats
      ||
    gTraceOah->fTraceVoicesDetails
  ) {
    gLogOstream <<
      endl <<
      "Upon visitStart (S_msrRepeatCommonPart&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      endl;

    gIndenter++;

    gLogOstream <<
      fCurrentVoiceClone;

    gIndenter--;
  }
#endif
}

void msr2BsrTranslator::visitEnd (S_msrRepeatCommonPart& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrRepeatCommonPart" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (
    gTraceOah->fTraceRepeats
      ||
    gTraceOah->fTraceVoicesDetails
  ) {
    gLogOstream <<
      endl <<
      "Upon visitEnd (S_msrRepeatCommonPart&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      endl;

    gIndenter++;

    gLogOstream <<
      fCurrentVoiceClone;

    gIndenter--;
  }
#endif

  // create a repeat and append it to voice clone
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
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
        getRepeatCommonPartRepeatUpLink ()->
          getRepeatTimes ());
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrRepeatEnding& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrRepeatEnding" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void msr2BsrTranslator::visitEnd (S_msrRepeatEnding& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrRepeatEnding" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create a repeat ending clone and append it to voice clone
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    fLogOutputStream <<
      "Appending a repeat ending clone to voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }

  if (gTraceOah->fTraceRepeats || gTraceOah->fTraceVoices) {
    gLogOstream <<
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
    handleRepeatEndingEndInVoice (
      inputLineNumber,
      elt->getRepeatEndingNumber (),
      elt->getRepeatEndingKind ());
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrMeasuresRepeat& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrMeasuresRepeat" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter++;
}

void msr2BsrTranslator::visitEnd (S_msrMeasuresRepeat& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrMeasuresRepeat" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter--;

  // set last segment as the measures repeat pattern segment
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    fLogOutputStream <<
      "Setting current last segment as measures repeat pattern segment in voice \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrMeasuresRepeatPattern& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrMeasuresRepeatPattern" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter++;
}

void msr2BsrTranslator::visitEnd (S_msrMeasuresRepeatPattern& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrMeasuresRepeatPattern" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter--;

  // get the measures repeat upLink
  S_msrMeasuresRepeat
    measuresRepeat =
      elt->getMeasuresRepeatUpLink ();

  // create a measures repeat and append it to voice clone
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
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

  // forget about the current measures repeat pattern clone
  fCurrentMeasuresRepeatPatternClone = nullptr;
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrMeasuresRepeatReplicas& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrMeasuresRepeatReplicas" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter++;
}

void msr2BsrTranslator::visitEnd (S_msrMeasuresRepeatReplicas& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrMeasuresRepeatReplicas" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter--;

  // create a measures repeat replica clone and append it to voice clone
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
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

  // forget about the current measures repeat replicas clone
 // JMI ??? fCurrentMeasuresRepeatReplicasClone = nullptr;
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrMultipleRest& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrMultipleRest" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    fLogOutputStream <<
      "Handling multiple rest start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (
    gTraceOah->fTraceRestMeasures
      ||
    gTraceOah->fTraceVoicesDetails
  ) {
    gLogOstream <<
      endl <<
      "Upon I visitStart (S_msrMultipleRest&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      endl;

    gIndenter++;

    gLogOstream <<
      fCurrentVoiceClone;

    gIndenter--;
  }
#endif

  fCurrentVoiceClone->
    handleMultipleRestInVoiceClone (
      inputLineNumber);

#ifdef TRACE_OAH
  if (
    gTraceOah->fTraceRestMeasures
      ||
    gTraceOah->fTraceVoicesDetails
  ) {
    gLogOstream <<
      endl <<
      "Upon II visitStart (S_msrMultipleRest&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      endl;

    gIndenter++;

    gLogOstream <<
      fCurrentVoiceClone;

    gIndenter--;
  }
#endif
}

void msr2BsrTranslator::visitEnd (S_msrMultipleRest& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrMultipleRest" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create the multiple rest clone
#ifdef TRACE_OAH
  if (
    gTraceOah->fTraceRestMeasures
      ||
    gTraceOah->fTraceSegments
      ||
    gTraceOah->fTraceVoices
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
#ifdef TRACE_OAH
  if (
    gTraceOah->fTraceRestMeasures
      ||
    gTraceOah->fTraceSegments
      ||
    gTraceOah->fTraceVoices
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

#ifdef TRACE_OAH
  if (
    gTraceOah->fTraceRestMeasures
      ||
    gTraceOah->fTraceVoicesDetails
  ) {
    gLogOstream <<
      endl <<
      "Upon visitEnd (S_msrMultipleRest&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      endl;

    gIndenter++;

    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrMultipleRestContents" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OAH
  if (
    gTraceOah->fTraceRestMeasures
      ||
    gTraceOah->fTraceVoicesDetails
  ) {
    gLogOstream <<
      endl <<
      "Upon visitStart (S_msrMultipleRestContents&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      endl;

    gIndenter++;

    gLogOstream <<
      fCurrentVoiceClone;

    gIndenter--;
  }
#endif
}

void msr2BsrTranslator::visitEnd (S_msrMultipleRestContents& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrMultipleRestContents" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter--;

  // create a multiple rest contents clone
  fCurrentMultipleRestContentsClone =
    elt->createMultipleRestContentsNewbornClone (
      fCurrentVoiceClone);

  // set last segment as the multiple rest contents segment
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
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

#ifdef TRACE_OAH
  if (
    gTraceOah->fTraceRestMeasures
      ||
    gTraceOah->fTraceVoicesDetails
  ) {
    gLogOstream <<
      endl <<
      "Upon visitEnd (S_msrMultipleRestContents&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      endl;

    gIndenter++;

    gLogOstream <<
      fCurrentVoiceClone;

    gIndenter--;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrVarValAssoc& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrVarValAssoc" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

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
        gOahOah->fInputSourceName,
        inputLineNumber,
        s.str ());
      }
  } // switch
}

void msr2BsrTranslator::visitEnd (S_msrVarValAssoc& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrVarValAssoc" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrVarValsListAssoc& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrVarValsListAssoc" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

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
        i++
      ) {
        fBsrScoreHeader->
          addRights (
            inputLineNumber, (*i));
      } // for
      break;

    case msrVarValsListAssoc::kComposer:
      for (list<string>::const_iterator i = variableValuesList.begin ();
        i != variableValuesList.end ();
        i++
      ) {
        fBsrScoreHeader->
          addComposer (
            inputLineNumber, (*i));
      } // for
      break;

    case msrVarValsListAssoc::kArranger:
      for (list<string>::const_iterator i = variableValuesList.begin ();
        i != variableValuesList.end ();
        i++
      ) {
        fBsrScoreHeader->
          addArranger (
            inputLineNumber, (*i));
      } // for
      break;

    case msrVarValsListAssoc::kLyricist:
      for (list<string>::const_iterator i = variableValuesList.begin ();
        i != variableValuesList.end ();
        i++
      ) {
        fBsrScoreHeader->
          addLyricist (
            inputLineNumber, (*i));
      } // for
      break;

    case msrVarValsListAssoc::kPoet:
      for (list<string>::const_iterator i = variableValuesList.begin ();
        i != variableValuesList.end ();
        i++
      ) {
        fBsrScoreHeader->
          addLyricist ( // JMI
            inputLineNumber, (*i));
      } // for
      break;

    case msrVarValsListAssoc::kTranslator:
      for (list<string>::const_iterator i = variableValuesList.begin ();
        i != variableValuesList.end ();
        i++
      ) {
        fBsrScoreHeader->
          addTranslator (
            inputLineNumber, (*i));
      } // for
      break;

    case msrVarValsListAssoc::kArtist:
      for (list<string>::const_iterator i = variableValuesList.begin ();
        i != variableValuesList.end ();
        i++
      ) {
        fBsrScoreHeader->
          addArtist (
            inputLineNumber, (*i));
      } // for
      break;

    case msrVarValsListAssoc::kSoftware:
      for (list<string>::const_iterator i = variableValuesList.begin ();
        i != variableValuesList.end ();
        i++
      ) {
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
        gOahOah->fInputSourceName,
        inputLineNumber,
        s.str ());
      }
      * /
  } // switch
}

void msr2BsrTranslator::visitEnd (S_msrVarValsListAssoc& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrVarValsListAssoc" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2BsrTranslator::visitStart (S_msrLayout& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter++;
}

void msr2BsrTranslator::visitEnd (S_msrLayout& elt)
{
  gIndenter--;

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}
*/


} // namespace
