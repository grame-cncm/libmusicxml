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

#include "msr2lpsrTranslator.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "mxmlTreeOah.h"
#include "msrOah.h"
#include "msr2lpsrOah.h"
#include "lpsrOah.h"
#include "lpsr2lilypondOah.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
msr2lpsrTranslator::msr2lpsrTranslator (
  S_msrScore visitedMsrScore)
{
  // the MSR score we're visiting
  fVisitedMsrScore = visitedMsrScore;

  // identification
  fOnGoingIdentification = false;

  // header
  fWorkNumberKnown       = false;
  fWorkTitleKnown        = false;
  fOpusKnown             = false;

  fMovementNumberKnown   = false;
  fMovementTitleKnown    = false;

  // staves
  fOnGoingStaff = false;

  // voices
  fOnGoingHarmonyVoice     = false;
  fOnGoingFiguredBassVoice = false;

  fOnGoingHarmony = false;

  // repeats

  // notes
  fOnGoingNonGraceNote = false;

  // beams
  fOnGoingChordBeamLink = false;

  // double tremolos
  fOnGoingDoubleTremolo = false;

  // grace notes
  fOnGoingGraceNotesGroup = false;
  fOnGoingChordGraceNotesGroupLink = false;

  // chords
  fOnGoingChord = false;

  // slurs
  fOnGoingChordSlurLink = false;

  // stanzas
  fOnGoingStanza = false;

  // syllables
  fOnGoingSyllableExtend = false;
};

msr2lpsrTranslator::~msr2lpsrTranslator ()
{}

//________________________________________________________________________
S_lpsrScore msr2lpsrTranslator::buildLpsrScoreFromMsrScore ()
{
  // sanity check
  msgAssert (
    fVisitedMsrScore != nullptr,
    "fVisitedMsrScore is null");

  // create another MSR score
  fCurrentMsrScoreClone =
    msrScore::create (
      K_NO_INPUT_LINE_NUMBER);

  // create the resulting LPSR score
  fResultingLpsrScore =
    lpsrScore::create (
      K_NO_INPUT_LINE_NUMBER,
      fCurrentMsrScoreClone);

  // create the current book block
  fCurrentLpsrBookBlock =
    lpsrBookBlock::create (
      K_NO_INPUT_LINE_NUMBER);

  // append it to the current book blocks list
  fResultingLpsrScore->
    appendBookBlockToBookBlocksList (
      fCurrentLpsrBookBlock);

  // create the current score block if relevant
  switch (gGlobalMsr2lpsrOahGroup->getScoreOutputKind ()) {
    case kScoreOnly:
    case kScoreAndParts:
    case kPartsAndScore:
    case kScoreAndPartsOneFile:
    case kPartsAndScoreOneFile:
      {
        // create the current score block
        fCurrentScoreBlock =
          lpsrScoreBlock::create (
            K_NO_INPUT_LINE_NUMBER);

        // append it to the book block elements list
        fCurrentLpsrBookBlock->
          appendLpsrScoreBlockToBookBlockElementsList (
            fCurrentScoreBlock);
      }
      break;
    case kPartsOnly:
    case kPartsOnlyOneFile:
      break;
  } // switch

  // create a msrScore browser
  msrBrowser<msrScore> browser (this);

  // browse the visited score with the browser
  browser.browse (*fVisitedMsrScore);

  return fResultingLpsrScore;
}

//________________________________________________________________________
void msr2lpsrTranslator::displayCurrentOnGoingValues ()
{
  gLogStream <<
    "Current ongoing values:" <<
    endl;

  gIndenter++;

  const int fieldWidth = 25;

  gLogStream <<
    setw (fieldWidth) <<
    "fOnGoingIdentification" << ": " << booleanAsString (fOnGoingIdentification) <<
    endl<<
    setw (fieldWidth) <<
    "fOnGoingStaff" << ": " << booleanAsString (fOnGoingStaff) <<
    endl<<
    setw (fieldWidth) <<
    "fOnGoingHarmonyVoice" << ": " << booleanAsString (fOnGoingHarmonyVoice) <<
    endl<<
    setw (fieldWidth) <<
    "fOnGoingHarmony" << ": " << booleanAsString (fOnGoingHarmony) <<
    endl<<
    setw (fieldWidth) <<
    "fOnGoingFiguredBassVoice" << ": " << booleanAsString (fOnGoingFiguredBassVoice) <<
    endl<<
    setw (fieldWidth) <<
    "fOnGoingNotesStack.size ()" << ": " << fOnGoingNotesStack.size () <<
    endl<<
    setw (fieldWidth) <<
    "fOnGoingNonGraceNote" << ": " << booleanAsString (fOnGoingNonGraceNote) <<
    endl<<
    setw (fieldWidth) <<
    "fOnGoingDoubleTremolo" << ": " << booleanAsString (fOnGoingDoubleTremolo) <<
    endl<<
    setw (fieldWidth) <<
    "fOnGoingGraceNotesGroup" << ": " << booleanAsString (fOnGoingGraceNotesGroup) <<
    endl<<
    setw (fieldWidth) <<
    "fOnGoingChord" << ": " << booleanAsString (fOnGoingChord) <<
    endl<<
    setw (fieldWidth) <<
    "fOnGoingStanza" << ": " << booleanAsString (fOnGoingStanza) <<
    endl<<
    setw (fieldWidth) <<
    "fOnGoingSyllableExtend" << ": " << booleanAsString (fOnGoingSyllableExtend) <<
    endl;

  gIndenter--;
}

//________________________________________________________________________
void msr2lpsrTranslator::displayPartHiddenMeasureAndBarlineDescrList ()
{
  gLogStream <<
    "fPartHiddenMeasureAndBarlineDescrList:" <<
    endl;

  if (fPartHiddenMeasureAndBarlineDescrList.size ()) {
    gIndenter++;

    const int fieldWidth = 19;

    list<S_msrHiddenMeasureAndBarlineDescr>::const_iterator
      iBegin = fPartHiddenMeasureAndBarlineDescrList.begin (),
      iEnd   = fPartHiddenMeasureAndBarlineDescrList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrHiddenMeasureAndBarlineDescr
        hiddenMeasureAndBarlineDescr = (*i);

      // sanity check
      msgAssert (
        hiddenMeasureAndBarlineDescr != nullptr,
        "hiddenMeasureAndBarlineDescr is null");

      S_msrDalSegno
        dalSegno =
          hiddenMeasureAndBarlineDescr->getDalSegno ();

      gLogStream << left <<
        setw (fieldWidth) <<
        "inputLineNumber" << " : " <<
        hiddenMeasureAndBarlineDescr->getInputLineNumber () <<
        endl <<
        setw (fieldWidth) <<
        "dalSegno" << " : " <<
        dalSegno <<
        endl;

      if (++i == iEnd) break;
    } // for

    gIndenter--;
  }
  else {
    gLogStream << "empty" << endl;
  }
}

//________________________________________________________________________
void msr2lpsrTranslator::displayOnGoingNotesStack (
  string context)
{
  int onGoingNotesStackSize = fOnGoingNotesStack.size ();

  gLogStream <<
    endl <<
    ">>++++++++++++++++ " <<
    "The on-going notes stack contains " << onGoingNotesStackSize << " elements:" <<
    endl;

  if (onGoingNotesStackSize) {
    list<S_msrNote>::const_iterator
      iBegin = fOnGoingNotesStack.begin (),
      iEnd   = fOnGoingNotesStack.end (),
      i      = iBegin;

    S_msrNote note = (*i);

    gIndenter++;

    int n = onGoingNotesStackSize;
    for ( ; ; ) {
      gLogStream <<
        "v (" << n << ")" <<
        endl;

      gIndenter++;
      note->printShort (gLogStream);
      gIndenter--;

      n--;

      if (++i == iEnd) break;

      gLogStream << endl;
    } // for

    gIndenter--;
  }

  gLogStream <<
    " <<++++++++++++++++ " <<
    endl << endl;
}

//________________________________________________________________________
void msr2lpsrTranslator::handlePartHiddenMeasureAndBarlineDescrList ()
{
  gLogStream <<
    "fPartHiddenMeasureAndBarlineDescrList:" <<
    endl;

  if (fPartHiddenMeasureAndBarlineDescrList.size ()) {
    list<S_msrHiddenMeasureAndBarlineDescr>::const_iterator
      iBegin = fPartHiddenMeasureAndBarlineDescrList.begin (),
      iEnd   = fPartHiddenMeasureAndBarlineDescrList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrHiddenMeasureAndBarlineDescr
        hiddenMeasureAndBarlineDescr = (*i);

      // sanity check
      msgAssert (
        hiddenMeasureAndBarlineDescr != nullptr,
        "hiddenMeasureAndBarlineDescr is null");

      S_msrDalSegno
        dalSegno =
          hiddenMeasureAndBarlineDescr->getDalSegno ();

      fCurrentPartClone->
        insertHiddenMeasureAndBarlineInPartClone (
          hiddenMeasureAndBarlineDescr->getInputLineNumber (),
          dalSegno->getMeasureElementPositionInMeasure ());

      if (++i == iEnd) break;
    } // for
  }
}

//________________________________________________________________________
void msr2lpsrTranslator::populateHeaderFromIdentification (
  S_lpsrHeader        header,
  S_msrIdentification identification)
{
  // sanity check
  msgAssert (
    header != nullptr,
    "header is null");
  msgAssert (
    identification != nullptr,
    "identification is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Populating LPSR header from identification " <<
      identification->asString () <<
      endl;
  }
#endif

  string workNumber = identification->getWorkNumber ();
  if (workNumber.size ()) {
    header->setWorkNumber (workNumber);
  }

  string workTitle = identification->getWorkTitle ();
  if (workTitle.size ()) {
    header->setWorkTitle (workTitle);
  }

  string opus = identification->getOpus ();
  if (opus.size ()) {
    header->setOpus (opus);
  }

  string movementNumber = identification->getMovementNumber ();
  if (movementNumber.size ()) {
    header->setMovementNumber (movementNumber);
  }

  string movementTitle = identification->getMovementTitle ();
  if (movementTitle.size ()) {
    header->setMovementTitle (movementTitle);
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Resulting LPSR header:" <<
      endl;
    gIndenter++;
    gLogStream <<
      header;
    gIndenter--;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::setPaperIndentsIfNeeded (
  S_msrScaling scaling)
{
  S_lpsrPaper
    paper =
      fResultingLpsrScore->getScorePaper ();

  int
    scorePartGroupNamesMaxLength =
      fCurrentMsrScoreClone->
        getScorePartGroupNamesMaxLength ();

  int
    scorePartNamesMaxLength =
      fCurrentMsrScoreClone->
        getScorePartNamesMaxLength ();

  int
    scoreInstrumentNamesMaxLength =
      fCurrentMsrScoreClone->
        getScoreInstrumentNamesMaxLength ();

  int
    scoreInstrumentAbbreviationsMaxLength =
      fCurrentMsrScoreClone->
        getScoreInstrumentAbbreviationsMaxLength ();

  // compute the maximum value
  int maxValue = -1;

  if (scorePartGroupNamesMaxLength > maxValue) {
    maxValue = scorePartGroupNamesMaxLength;
  }

  if (scorePartNamesMaxLength > maxValue) {
    maxValue = scorePartNamesMaxLength;
  }

  if (scoreInstrumentNamesMaxLength > maxValue) {
    maxValue = scoreInstrumentNamesMaxLength;
  }

  // compute the maximum short value
  int maxShortValue = -1;

  if (scoreInstrumentAbbreviationsMaxLength > maxShortValue) {
    maxShortValue = scoreInstrumentAbbreviationsMaxLength;
  }

  // heuristics to determine the number of characters per centimeter
//  float charactersPerCemtimeter = 4.0; // JMI ???

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceGeometry ()) {
  /*
    // get the paper width
    S_msrLength
      paperWidth =
        scaling->
          getPageLayout ()->
            getPageWidth ();

    if (paperWidth) {
      gLogStream <<
        "setPaperIndentsIfNeeded()" << // JMI ???
        endl;
    }
*/
    gIndenter++;

    const int fieldWidth = 40;

    gLogStream << left <<
      setw (fieldWidth) <<
      "scorePartGroupNamesMaxLength" << " : " <<
      scorePartGroupNamesMaxLength <<
      endl <<
      setw (fieldWidth) <<
      "scorePartNamesMaxLength" << " : " <<
      scorePartNamesMaxLength <<
      endl <<

      setw (fieldWidth) <<
      "scoreInstrumentNamesMaxLength" << " : " <<
      scoreInstrumentNamesMaxLength <<
      endl <<
      setw (fieldWidth) <<
      "scoreInstrumentAbbreviationsMaxLength" << " : " <<
      scoreInstrumentAbbreviationsMaxLength <<
      endl <<

      setw (fieldWidth) <<
      "maxValue" << " : " <<
      maxValue <<
      endl <<
      setw (fieldWidth) <<
      "maxShortValue" << " : " <<
      maxShortValue <<
      /* JMI
      endl <<

      setw (fieldWidth) <<
      "charactersPerCemtimeter" << " : " <<
      charactersPerCemtimeter <<
      */
      endl;
/*
    gLogStream << left <<
      setw (fieldWidth) <<
      "paperWidth" << " : ";
    if (paperWidth) {
      gLogStream << paperWidth;
    }
    else {
      gLogStream << "none";
    }
    gLogStream << endl;
*/
    gIndenter--;
  }
#endif

/* JMI
  // set indent if relevant
  if (maxValue > 0) {
    paper->
      setIndent (
        maxValue / charactersPerCemtimeter);
  }

  // set short indent if relevant
  if (maxShortValue > 0) {
    paper->
      setShortIndent (
        maxShortValue / charactersPerCemtimeter);
  }
  */
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrScore& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrScore" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // fetch score header IDENTIFICATIONL JMI
  fCurrentLpsrScoreHeader =
    fResultingLpsrScore-> getScoreHeader();

  // is there a rights option?
  if (gGlobalLpsr2lilypondOahGroup->getRights ().size ()) {
    // define rights
    fCurrentLpsrScoreHeader->
      appendRights (
        inputLineNumber,
        gGlobalLpsr2lilypondOahGroup->getRights ());
  }

  // is there a composer option?
  if (gGlobalLpsr2lilypondOahGroup->getComposer ().size ()) {
    // define composer
    fCurrentLpsrScoreHeader->
      appendComposer (
        inputLineNumber,
        gGlobalLpsr2lilypondOahGroup->getComposer ());
  }

  // is there an arranger option?
  if (gGlobalLpsr2lilypondOahGroup->getArranger ().size ()) {
    // define arranger
    fCurrentLpsrScoreHeader->
      appendArranger (
        inputLineNumber,
        gGlobalLpsr2lilypondOahGroup->getArranger ());
  }

  // is there a poet option?
  if (gGlobalLpsr2lilypondOahGroup->getPoetAtom ()->getVariableHasBeenSet ()) {
    // remove all poets
    fCurrentLpsrScoreHeader->
      removeAllPoets (inputLineNumber);
    // append poet
    fCurrentLpsrScoreHeader->
      appendPoet (
        inputLineNumber,
        gGlobalLpsr2lilypondOahGroup->getPoet ());
  }

  // is there a lyricist option?
  if (gGlobalLpsr2lilypondOahGroup->getLyricist ().size ()) {
    // define lyricist
    fCurrentLpsrScoreHeader->
      appendLyricist (
        inputLineNumber,
        gGlobalLpsr2lilypondOahGroup->getLyricist ());
  }

  // is there a software option?
  if (gGlobalLpsr2lilypondOahGroup->getSoftware ().size ()) {
    // define software
    fCurrentLpsrScoreHeader->
      appendSoftware (
        inputLineNumber,
        gGlobalLpsr2lilypondOahGroup->getSoftware ());
  }

  // is there a dedication?
  if (gGlobalLpsr2lilypondOahGroup->getDedication ().size ()) {
    // define dedication
    fCurrentLpsrScoreHeader->
      setLilypondDedication (
        gGlobalLpsr2lilypondOahGroup->getDedication ());
  }

  // is there a piece?
  if (gGlobalLpsr2lilypondOahGroup->getPiece ().size ()) {
    // define piece
    fCurrentLpsrScoreHeader->
      setLilypondPiece (
        gGlobalLpsr2lilypondOahGroup->getPiece ());
  }

  // is there an opus?
  if (gGlobalLpsr2lilypondOahGroup->getOpus ().size ()) {
    // define opus
    fCurrentLpsrScoreHeader->
      setLilypondOpus (
        gGlobalLpsr2lilypondOahGroup->getOpus ());
  }

  // is there a title?
  if (gGlobalLpsr2lilypondOahGroup->getTitle ().size ()) {
    // define title
    fCurrentLpsrScoreHeader->
      setLilypondTitle (
        gGlobalLpsr2lilypondOahGroup->getTitle ());
  }

  // is there a subtitle?
  if (gGlobalLpsr2lilypondOahGroup->getSubTitle ().size ()) {
    // define subtitle
    fCurrentLpsrScoreHeader->
      setLilypondSubTitle (
        gGlobalLpsr2lilypondOahGroup->getSubTitle ());
  }

  // is there a subsubtitle?
  if (gGlobalLpsr2lilypondOahGroup->getSubSubTitle ().size ()) {
    // define subsubtitle
    fCurrentLpsrScoreHeader->
      setLilypondSubSubTitle (
        gGlobalLpsr2lilypondOahGroup->getSubSubTitle ());
  }

  // is there a meter?
  if (gGlobalLpsr2lilypondOahGroup->getMeter ().size ()) {
    // define meter
    fCurrentLpsrScoreHeader->
      setLilypondMeter (
        gGlobalLpsr2lilypondOahGroup->getMeter ());
  }

  // is there a tagline?
  if (gGlobalLpsr2lilypondOahGroup->getTagline ().size ()) {
    // define tagline
    fCurrentLpsrScoreHeader->
      setLilypondTagline (
        gGlobalLpsr2lilypondOahGroup->getTagline ());
  }

  // is there a copyright?
  if (gGlobalLpsr2lilypondOahGroup->getCopyright ().size ()) {
    // define copyright
    fCurrentLpsrScoreHeader->
      setLilypondCopyright (
        gGlobalLpsr2lilypondOahGroup->getCopyright ());
  }

  // are the rests to be merged?
  if (gGlobalLpsr2lilypondOahGroup->getMergeRests ()) {
    fResultingLpsrScore->
      // this score needs the 'merge rests' Scheme functions
      setMergeRestsIsNeeded ();
  }

  // is the LilyPond macro 'boxAroundNextBarNumber' to be generated?
  if (gGlobalLpsr2lilypondOahGroup->getBoxAroundBarNumberSet ().size ()) {
    fResultingLpsrScore->
      // this score needs the 'boxAroundNextBarNumber' Scheme function
      setBoxAroundNextBarNumberIsNeeded ();
  }

  // is the Scheme function 'whiteNoteHeads' to be generated?
  if (gGlobalLpsr2lilypondOahGroup->getWhiteNoteHeads ()) {
    fResultingLpsrScore->
      // this score needs the 'whiteNoteHeads' Scheme function
      setWhiteNoteHeadsIsNeeded ();
  }

  // is the Scheme function 'jazzChordsDisplay' to be generated?
  if (gGlobalLpsr2lilypondOahGroup->getJazzChordsDisplay ()) {
    fResultingLpsrScore->
      // this score needs the 'jazzChordsDisplay' Scheme function
      setJazzChordsDisplayIsNeeded ();
  }

  // is jiǎnpǔ notation to be generated?
  if (gGlobalLpsr2lilypondOahGroup->getJianpu ()) {
    fResultingLpsrScore->
      // this score needs the 'jianpu file include' Scheme function
      setJianpuFileIncludeIsNeeded ();
  }

  // get the print notes head RGB color atom
  S_oahRGBColorAtom
    ledgerLinesRGBColorAtom =
      gGlobalLpsr2lilypondOahGroup-> // JMI BLARK put that in gGlobalMsr2lpsrOahGroup
        getLedgerLinesRGBColorAtom ();

  // has the atom been used?
  if (ledgerLinesRGBColorAtom->getVariableHasBeenSet ()) {
    // this score needs the 'colored ledger lines' Scheme function
    fResultingLpsrScore->
      setColoredLedgerLinesIsNeeded ();
  }

/* JMI
  // push it onto this visitors's stack,
  // making it the current partGroup
  fPartGroupBlocksStack.push (
    partGroupBlock);
    */
}

void msr2lpsrTranslator::visitEnd (S_msrScore& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrScore" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (fWorkTitleKnown && fMovementTitleKnown) {
    string
      workTitle =
        fCurrentIdentification->
          getWorkTitle (),
      movementTitle =
        fCurrentIdentification->
          getMovementTitle ();

    if (
      workTitle.size () == 0
        &&
      movementTitle.size () > 0
    ) {
      // use the movement title as the work title
      fCurrentIdentification->
        setWorkTitle (
          inputLineNumber,
          movementTitle);

      fCurrentLpsrScoreHeader->
        setWorkTitle (
          movementTitle);

      // forget the movement title
      fCurrentIdentification->
        setMovementTitle (
          inputLineNumber,
          "");

      fCurrentLpsrScoreHeader->
        setMovementTitle (
          "");
    }
  }

  else if (! fWorkTitleKnown && fMovementTitleKnown) {
    string
      movementTitle =
        fCurrentIdentification->
          getMovementTitle ();

    // use the movement title as the work title
    fCurrentIdentification->
      setWorkTitle (
        inputLineNumber,
        movementTitle);

    fCurrentLpsrScoreHeader->
      setWorkTitle (movementTitle);

    // forget the movement title
    fCurrentIdentification->
      setMovementTitle (
        inputLineNumber,
        "");

    fCurrentLpsrScoreHeader->
      setMovementTitle ("");
  }

  if (fWorkNumberKnown && fMovementNumberKnown) {
    string
      workNumber =
        fCurrentIdentification->
          getWorkNumber (),
      movementNumber =
        fCurrentIdentification->
          getMovementNumber ();

    if (
      workNumber.size () == 0
        &&
      movementNumber.size () > 0
    ) {
      // use the movement number as the work number
      fCurrentIdentification->
        setWorkNumber (
        inputLineNumber,
        movementNumber);

      fCurrentLpsrScoreHeader->
        setWorkNumber (movementNumber);

      // forget the movement number
      fCurrentIdentification->
        setMovementNumber (
        inputLineNumber,
        "");

      fCurrentLpsrScoreHeader->
        setMovementNumber ("");
    }
  }

  else if (! fWorkNumberKnown && fMovementNumberKnown) {
    string
      movementNumber =
        fCurrentIdentification->
          getMovementNumber ();

    // use the movement number as the work number
    fCurrentIdentification->
      setWorkNumber (
        inputLineNumber,
        movementNumber);

    fCurrentLpsrScoreHeader->
      setWorkNumber (movementNumber);

    // forget the movement number
    fCurrentIdentification->
      setMovementNumber (
        inputLineNumber,
        "");

    fCurrentLpsrScoreHeader->
      setMovementNumber ("");
  }

  // set ident and short indent if needed
  setPaperIndentsIfNeeded ( // JMI ??? BLARK
    elt->getScaling ());

/* JMI
  // get top level partgroup block from the stack
  S_lpsrPartGroupBlock
    partGroupBlock =
      fPartGroupBlocksStack.top ();

  // pop it from the stack
  fPartGroupBlocksStack.top ();

  // the stack should now be empty
  if (fPartGroupBlocksStack.size ()) {
    msrInternalError (
      1,
      "the partGroup block stack is not exmpty at the end of the visit");
  }
   */
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrIdentification& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrIdentification" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter++;

  // set the identification
  fCurrentIdentification = elt;

  // store it in the current MSR score clone
  fCurrentMsrScoreClone->
    setIdentification (
      fCurrentIdentification);

  // use it to populate the header
  populateHeaderFromIdentification (
    fCurrentLpsrScoreHeader,
    fCurrentIdentification);

  fOnGoingIdentification = true;
}

void msr2lpsrTranslator::visitEnd (S_msrIdentification& elt)
{
  fOnGoingIdentification = false;

  gIndenter--;

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrIdentification" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrScaling& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrScaling" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter++;

  // create a scaling clone
  S_msrScaling
    scalingClone =
      elt->createMsrScalingNewbornClone ();

  // register it in the current MSR score clone
  fCurrentMsrScoreClone-> // JMI BLARK ???
    setScaling (
      scalingClone);

  // get LPSR score paper
  S_lpsrPaper
    paper =
      fResultingLpsrScore->getScorePaper ();

  // sanity check
  msgAssert (
    paper != nullptr,
    "paper is null");

  // set the current book block's paper as a newborn clone of paper
  fCurrentLpsrBookBlock ->
    setBookBlockPaper (
      paper->
        createPaperNewbornClone ());

  // get LPSR score layout
  S_lpsrLayout
    scoreLayout =
      fResultingLpsrScore->getScoreLayout ();
}

void msr2lpsrTranslator::visitEnd (S_msrScaling& elt)
{
  gIndenter--;

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrScaling" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrSystemLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSystemLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // register the aystem layout in the current MSR score clone
  fCurrentMsrScoreClone->
    setSystemLayout (
      elt);
}

void msr2lpsrTranslator::visitEnd (S_msrSystemLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSystemLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrStaffLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStaffLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // register the staff layout in the current MSR score clone
  fCurrentMsrScoreClone->
    setStaffLayout (
      elt);
}

void msr2lpsrTranslator::visitEnd (S_msrStaffLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrStaffLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrAppearance& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrAppearance" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // register the appearance in the current MSR score clone
  fCurrentMsrScoreClone->
    setAppearance (
      elt);
}

void msr2lpsrTranslator::visitEnd (S_msrAppearance& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrAppearance" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrPageLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPageLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // register the page layout in the current MSR score clone
  fCurrentMsrScoreClone->
    setPageLayout (
      elt);
}

void msr2lpsrTranslator::visitEnd (S_msrPageLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPageLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrCredit& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrCredit" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentCredit = elt;

  // set elt as credit of the MSR score part of the LPSR score
  fResultingLpsrScore->
    getMsrScore ()->
      appendCreditToScore (fCurrentCredit);
}

void msr2lpsrTranslator::visitEnd (S_msrCredit& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrCredit" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentCredit = nullptr;
}

void msr2lpsrTranslator::visitStart (S_msrCreditWords& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrCreditWords" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // don't append it to the current credit, since the latter is no clone
  /* JMI
  fCurrentCredit->
    appendCreditWordsToCredit (
      elt);
      */
}

void msr2lpsrTranslator::visitEnd (S_msrCreditWords& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrCreditWords" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrPartGroup& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPartGroup " <<
      elt->getPartGroupCombinedName () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create a partGroup clone
  // current partGroup clone, i.e. the top of the stack,
  // is the upLink of the new one if it exists
  S_msrPartGroup
    partGroupClone =
      elt->createPartGroupNewbornClone (
        fPartGroupsStack.size ()
          ? fPartGroupsStack.top ()
          : nullptr,
        fResultingLpsrScore->getMsrScore ());

  // push it onto this visitors's stack,
  // making it the current partGroup block
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "Pushing part group clone " <<
      partGroupClone->getPartGroupCombinedName () <<
      " onto stack" <<
      endl;
  }
#endif

  fPartGroupsStack.push (
    partGroupClone);

/* JMI
  // add it to the MSR score clone
  fCurrentMsrScoreClone->
    addPartGroupToScore (fCurrentPartGroupClone);
*/

  // create a partGroup block refering to the part group clone
  S_lpsrPartGroupBlock
    partGroupBlock =
      lpsrPartGroupBlock::create (
        partGroupClone);

  // push it onto this visitors's stack,
  // making it the current partGroup block
  fPartGroupBlocksStack.push (
    partGroupBlock);

  // get the LPSR store block
  S_lpsrScoreBlock
    scoreBlock =
      fResultingLpsrScore->getScoreScoreBlock ();

  // don't append the partgroup block to the score/bookpart block now:
  // this will be done when it gets popped from the stack
}

void msr2lpsrTranslator::visitEnd (S_msrPartGroup& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPartGroup " <<
      elt->getPartGroupCombinedName () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  S_msrPartGroup
    currentPartGroup =
      fPartGroupsStack.top ();

  if (fPartGroupsStack.size () == 1) {
    // add the current partgroup clone to the MSR score clone
    // if it is the top-level one, i.e it's alone in the stack

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTracePartGroups ()) {
      gLogStream <<
        "Adding part group clone " <<
        currentPartGroup->getPartGroupCombinedName () <<
        " to MSR score" <<
        endl;
    }
#endif

    fCurrentMsrScoreClone->
      addPartGroupToScore (currentPartGroup);

    fPartGroupsStack.pop ();
  }

  else {

    // pop current partGroup from this visitors's stack
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTracePartGroups ()) {
      gLogStream <<
        "Popping part group clone " <<
        fPartGroupsStack.top ()->getPartGroupCombinedName () <<
        " from stack" <<
        endl;
    }
#endif

    fPartGroupsStack.pop ();

    // append the current part group to the one one level higher,
    // i.e. the new current part group
    fPartGroupsStack.top ()->
      appendSubPartGroupToPartGroup (
        currentPartGroup);
  }

  S_lpsrPartGroupBlock
    currentPartGroupBlock =
      fPartGroupBlocksStack.top ();

  if (fPartGroupBlocksStack.size () == 1) {
    // add the current partgroup clone to the LPSR score's parallel music
    // if it is the top-level one, i.e it's alone in the stack

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTracePartGroups ()) {
      gLogStream <<
        "Adding part group block clone for part group " <<
        currentPartGroupBlock->
          getPartGroup ()->
            getPartGroupCombinedName () <<
        " to LPSR score" <<
        endl;
    }
#endif

    // append the current partgroup block to the current score block if relevant
    switch (gGlobalMsr2lpsrOahGroup->getScoreOutputKind ()) {
      case kScoreOnly:
      case kScoreAndParts:
      case kPartsAndScore:
      case kScoreAndPartsOneFile:
      case kPartsAndScoreOneFile:
        {
          // sanity check
          msgAssert (
            fCurrentScoreBlock != nullptr,
            "fCurrentScoreBlock is null");

          // append the current partgroup block to the current score block
          // if it is the top-level one, i.e it's alone in the stack JMI
          // JMI BOF if (fPartGroupBlocksStack.size () == 1)
#ifdef TRACING_IS_ENABLED
          if (gGlobalLpsrOahGroup->getTraceLpsrBlocks ()) {
            gLogStream <<
              "Appending part group block for part group " <<
              currentPartGroupBlock->
                getPartGroup ()->
                  getPartGroupCombinedName () <<
              " to the current score block '" <<
              fCurrentScoreBlock->asShortString () <<
              "'" <<
              endl;
          }
#endif
          fCurrentScoreBlock->
            appendPartGroupBlockToScoreBlock (
              fPartGroupBlocksStack.top ());
        }
        break;
      case kPartsOnly:
      case kPartsOnlyOneFile:
        break;
    } // switch

    // append the current partgroup block to the current bookpart block if relevant
    switch (gGlobalMsr2lpsrOahGroup->getScoreOutputKind ()) {
      case kScoreOnly:
        break;
      case kScoreAndParts:
      case kPartsAndScore:
      case kScoreAndPartsOneFile:
      case kPartsAndScoreOneFile:
      case kPartsOnly:
      case kPartsOnlyOneFile:
        {
          // sanity check
          msgAssert (
            fCurrentBookPartBlock != nullptr,
            "fCurrentBookPartBlock is null");

          // append the current partgroup block to the current bookpart block
          // if it is the top-level one, i.e it's alone in the stack JMI
          // JMI BOF if (fPartGroupBlocksStack.size () == 1)
#ifdef TRACING_IS_ENABLED
          if (gGlobalLpsrOahGroup->getTraceLpsrBlocks ()) {
            gLogStream <<
              "Appending part group block for part group " <<
              currentPartGroupBlock->
                getPartGroup ()->
                  getPartGroupCombinedName () <<
              " to the current bookpart block '" <<
              fCurrentScoreBlock->asShortString () <<
              "'" <<
              endl;
          }
#endif
          fCurrentBookPartBlock->
            appendPartGroupBlockToBookPartBlock (
              fPartGroupBlocksStack.top ());
        }
        break;
    } // switch

    // pop current partGroup block from this visitors's stack,
    // only now to restore the appearence order
    fPartGroupBlocksStack.pop ();
  }

  else {
    // pop current partGroup block from this visitors's stack
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTracePartGroups ()) {
      gLogStream <<
        "Popping part group block clone for part group " <<
        currentPartGroupBlock->
          getPartGroup ()->
            getPartGroupCombinedName () <<
        " from stack" <<
        endl;
    }
#endif

    fPartGroupBlocksStack.pop ();

    // append the current part group block to the one one level higher,
    // i.e. the new current part group block
    fPartGroupBlocksStack.top ()->
      appendElementToPartGroupBlock (
        currentPartGroupBlock);
  }
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrPart& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  string
    partCombinedName =
      elt->getPartCombinedName ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPart " <<
      partCombinedName <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceParts ()) {
    gLogStream <<
      endl <<
      "<!--=== part \"" << partCombinedName << "\"" <<
      ", line " << inputLineNumber << " ===-->" <<
      endl;
  }
#endif

  gIndenter++;

  // create a part clone
  fCurrentPartClone =
    elt->createPartNewbornClone (
      fPartGroupsStack.top ());

  // add it to the partGroup clone
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceParts ()) {
    gLogStream <<
      "Adding part clone " <<
      fCurrentPartClone->getPartCombinedName () <<
      " to part group clone \"" <<
      fPartGroupsStack.top ()->getPartGroupCombinedName () <<
      "\"" <<
      endl;
  }
#endif

  fPartGroupsStack.top ()->
    appendPartToPartGroup (fCurrentPartClone);

  // create a part block
  fCurrentPartBlock =
    lpsrPartBlock::create (
      fCurrentPartClone);

  // append it to the current partGroup block
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceParts ()) {
    gLogStream <<
      "Appending part block " <<
      fPartGroupsStack.top ()->getPartGroupCombinedName () <<
      " to part group blocks stack top" <<
      endl;
  }
#endif

  fPartGroupBlocksStack.top ()->
    appendElementToPartGroupBlock (
      fCurrentPartBlock);

  // create a bookpart block if relevant
  switch (gGlobalMsr2lpsrOahGroup->getScoreOutputKind ()) {
    case kScoreOnly:
      break;
    case kScoreAndParts:
    case kPartsAndScore:
    case kScoreAndPartsOneFile:
    case kPartsAndScoreOneFile:
    case kPartsOnly:
    case kPartsOnlyOneFile:
      {
        // create the current score block
        fCurrentBookPartBlock =
          lpsrBookPartBlock::create (
            inputLineNumber);

        // append it to the book block elements list
        fCurrentLpsrBookBlock->
          appendLpsrBookPartBlockToBookBlockElementsList (
            fCurrentBookPartBlock);
      }
      break;
  } // switch
}

void msr2lpsrTranslator::visitEnd (S_msrPart& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  gIndenter--;

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPart " <<
      elt->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  string
    partInstrumentAbbreviation =
      fCurrentPartClone->
        getPartInstrumentAbbreviation ();

  // populate part instrument short name if empty and possible
  if (partInstrumentAbbreviation.size () == 0) {
    string
      partAbbreviation =
        elt->getPartAbbreviation ();

    fCurrentPartClone->
      setPartInstrumentAbbreviation (
        partAbbreviation);
  }

  // finalize the current part clone
  fCurrentPartClone->
    finalizePartClone (
      inputLineNumber);

  // handle the hidden measure and barline elements
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceDalSegnos () || gGlobalTraceOahGroup->getTracePositionsInMeasures ()) {
    displayPartHiddenMeasureAndBarlineDescrList ();
  }
#endif

  // JMI handlePartHiddenMeasureAndBarlineDescrList ();

  // handle skip grace notes if needed
  if (fCurrentSkipGraceNotesGroup) {
    // add it ahead of the other voices in the part if needed
    fCurrentPartClone->
      addSkipGraceNotesGroupBeforeAheadOfVoicesClonesIfNeeded ( // JMI only if there's more than one voice???
        fCurrentVoiceClone,
        fCurrentSkipGraceNotesGroup);

    // forget about this skip grace notes group
    fCurrentSkipGraceNotesGroup = nullptr;
  }

  // forget about the current part block
  fCurrentPartBlock = nullptr;

  // forget about the current book part block
  fCurrentBookPartBlock = nullptr;
}

//________________________________________________________________________
/* JMI
void msr2lpsrTranslator::visitStart (S_msrStaffLinesNumber& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStaffLinesNumber" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create a staff lines number clone
  fCurrentStaffLinesNumberClone =
    elt->
      createStaffLinesNumberNewbornClone ();
}
*/

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrStaffTuning& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStaffTuning" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create a staff tuning clone
  fCurrentStaffTuningClone =
    elt->
      createStaffTuningNewbornClone ();
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrStaffDetails& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStaffDetails" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentStaffTuningClone = nullptr;
}

void msr2lpsrTranslator::visitEnd (S_msrStaffDetails& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrStaffDetails" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append the staff details to the current voice clone
  fCurrentVoiceClone->
    appendStaffDetailsToVoice (
      elt);
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrStaff& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStaff \"" <<
      elt->getStaffName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter++;

  switch (elt->getStaffKind ()) {
    case msrStaff::kStaffRegular:
    case msrStaff::kStaffTablature:
    case msrStaff::kStaffDrum:
    case msrStaff::kStaffRythmic:
      {
        // create a staff clone
        fCurrentStaffClone =
          elt->createStaffNewbornClone (
            fCurrentPartClone);

        // add it to the part clone
        fCurrentPartClone->
          addStaffToPartCloneByItsNumber (
            fCurrentStaffClone);

        // create a staff block
        fCurrentStaffBlock =
          lpsrStaffBlock::create (
            fCurrentStaffClone);

        string
          partName =
            fCurrentPartClone->getPartName (),
          partAbbreviation =
            fCurrentPartClone->getPartAbbreviation ();

        string staffBlockInstrumentName;
        string staffBlockShortInstrumentName;

        // don't set instrument name nor short instrument name // JMI
        // if the staff belongs to a piano part where they're already set
        if (! partName.size ()) {
          staffBlockInstrumentName = partName;
        }
        if (! partAbbreviation.size ()) {
          staffBlockShortInstrumentName = partAbbreviation;
        }

        if (staffBlockInstrumentName.size ()) {
          fCurrentStaffBlock->
            setStaffBlockInstrumentName (staffBlockInstrumentName);
        }

        if (staffBlockShortInstrumentName.size ()) {
          fCurrentStaffBlock->
            setStaffBlockShortInstrumentName (staffBlockShortInstrumentName);
        }

        // append the staff block to the current part block
        fCurrentPartBlock->
          appendStaffBlockToPartBlock (
            fCurrentStaffBlock);

        // handle the current staff block
        switch (gGlobalMsr2lpsrOahGroup->getScoreOutputKind ()) {
          case kScoreOnly: // default value
            break;
          case kScoreAndParts:
          case kPartsAndScore:
          case kScoreAndPartsOneFile:
          case kPartsAndScoreOneFile:
            {
            /* JMI
              // create the current score block
              fCurrentScoreBlock =
                lpsrScoreBlock::create (
                  K_NO_INPUT_LINE_NUMBER);

              // append it to the book block elements list
              fCurrentLpsrBookBlock->
                appendLpsrScoreBlockToBookBlockElementsList (
                  fCurrentScoreBlock);
                */
            }
            break;
          case kPartsOnly:
          case kPartsOnlyOneFile:
            break;
        } // switch

        fOnGoingStaff = true;
      }
      break;

    case msrStaff::kStaffHarmony:
      {
        // create a staff clone
        fCurrentStaffClone =
          elt->createStaffNewbornClone (
            fCurrentPartClone);

        // add it to the part clone
        fCurrentPartClone->
          addStaffToPartCloneByItsNumber (
            fCurrentStaffClone);

        fOnGoingStaff = true;
      }
      break;

    case msrStaff::kStaffFiguredBass:
      {
        // create a staff clone
        fCurrentStaffClone =
          elt->createStaffNewbornClone (
            fCurrentPartClone);

        // add it to the part clone
        fCurrentPartClone->
          addStaffToPartCloneByItsNumber (
            fCurrentStaffClone);

        // register it as the part figured bass staff
        fCurrentPartClone->
          setPartFiguredBassStaff (fCurrentStaffClone);

        fOnGoingStaff = true;
      }
      break;
  } // switch
}

void msr2lpsrTranslator::visitEnd (S_msrStaff& elt)
{
  gIndenter--;

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_msrStaff \"" <<
      elt->getStaffName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  switch (elt->getStaffKind ()) {
    case msrStaff::kStaffRegular:
    case msrStaff::kStaffDrum:
    case msrStaff::kStaffRythmic:
      {
        fOnGoingStaff = false;
      }
      break;

    case msrStaff::kStaffTablature:
      // JMI
      break;

    case msrStaff::kStaffHarmony:
      // JMI
      break;

    case msrStaff::kStaffFiguredBass:
      // JMI
      break;
  } // switch
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrVoice& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrVoice \"" <<
      elt->asString () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fCurrentVoiceOriginal = elt;

  gIndenter++;

  switch (elt->getVoiceKind ()) {

    case msrVoice::kVoiceRegular:
      // create a voice clone
      fCurrentVoiceClone =
        elt->createVoiceNewbornClone (
          fCurrentStaffClone);

      // add it to the staff clone
      fCurrentStaffClone->
        registerVoiceInStaffClone (
          inputLineNumber,
          fCurrentVoiceClone);

      // append the voice clone to the LPSR score elements list
      fResultingLpsrScore ->
        appendVoiceToLpsrScoreElementsList (fCurrentVoiceClone);

      // append a use of the voice to the current staff block
      fCurrentStaffBlock->
        appendVoiceUseToStaffBlock (
          fCurrentVoiceClone);
      break;

    case msrVoice::kVoiceHarmony:
      {
        /* JMI
        // create the harmony staff and voice if not yet done
        fCurrentPartClone->
          createPartHarmonyStaffAndVoiceIfNotYetDone (
            inputLineNumber);

        // fetch harmony voice
        fCurrentVoiceClone =
          fCurrentPartClone->
            getPartHarmonyVoice ();
*/

        // create a voice clone
        fCurrentVoiceClone =
          elt->createVoiceNewbornClone (
            fCurrentStaffClone);

        // add it to the staff clone
        fCurrentStaffClone->
          registerVoiceInStaffClone (
            inputLineNumber,
            fCurrentVoiceClone);

        if (
          elt->getMusicHasBeenInsertedInVoice () // superfluous test ??? JMI
          ) {
          // append the voice clone to the LPSR score elements list
          fResultingLpsrScore ->
            appendVoiceToLpsrScoreElementsList (
              fCurrentVoiceClone);

          // create a ChordNames context
          string voiceName =
            elt->getVoiceName ();

          string partCombinedName =
            elt->fetchVoicePartUpLink ()->
              getPartCombinedName ();

#ifdef TRACING_IS_ENABLED
          if (gGlobalTraceOahGroup->getTraceHarmonies ()) {
            gLogStream <<
              "Creating a ChordNames context for \"" << voiceName <<
              "\" in part " << partCombinedName <<
              endl;
          }
#endif

          S_lpsrChordNamesContext
            chordNamesContext =
              lpsrChordNamesContext::create (
                inputLineNumber,
                lpsrContext::kExistingContextYes,
                voiceName,
// JMI                elt->getHarmonyVoiceRegularVoiceBackwardLink ());
                elt);

          // append it to the current part block
#ifdef TRACING_IS_ENABLED
          if (gGlobalTraceOahGroup->getTraceHarmonies ()) {
            gLogStream <<
              "Appending the ChordNames context for \"" << voiceName <<
              "\" in part " << partCombinedName <<
              endl;
          }
#endif

          fCurrentPartBlock->
            appendChordNamesContextToPartBlock (
              inputLineNumber,
              chordNamesContext);

          fOnGoingHarmonyVoice = true;
        }
      }
      break;

    case msrVoice::kVoiceFiguredBass:
      {
        // create a voice clone
        fCurrentVoiceClone =
          elt->createVoiceNewbornClone (
            fCurrentStaffClone);

        // add it to the staff clone
        fCurrentStaffClone->
          registerVoiceInStaffClone (
            inputLineNumber,
            fCurrentVoiceClone);

        // register it as the part figured bass voice
        fCurrentPartClone->
          setPartFiguredBassVoice (fCurrentVoiceClone);

        if (
          elt->getMusicHasBeenInsertedInVoice () // superfluous test ??? JMI
          ) {
          // append the voice clone to the LPSR score elements list
          fResultingLpsrScore ->
            appendVoiceToLpsrScoreElementsList (
              fCurrentVoiceClone);

          // create a FiguredBass context
          string voiceName =
            elt->getVoiceName ();

          string partCombinedName =
            elt->fetchVoicePartUpLink ()->
              getPartCombinedName ();

#ifdef TRACING_IS_ENABLED
          if (gGlobalTraceOahGroup->getTraceHarmonies ()) {
            gLogStream <<
              "Creating a FiguredBass context for \"" << voiceName <<
              "\" in part " << partCombinedName <<
              endl;
          }
#endif

          S_lpsrFiguredBassContext
            figuredBassContext =
              lpsrFiguredBassContext::create (
                inputLineNumber,
                lpsrContext::kExistingContextYes,
                voiceName,
                elt-> getVoiceStaffUpLink ());

          // append it to the current part block
#ifdef TRACING_IS_ENABLED
          if (gGlobalTraceOahGroup->getTraceHarmonies ()) {
            gLogStream <<
              "Appending the FiguredBass context for \"" << voiceName <<
              "\" in part " << partCombinedName <<
              endl;
          }
#endif

          fCurrentPartBlock->
            appendFiguredBassContextToPartBlock (
              figuredBassContext);

          fOnGoingFiguredBassVoice = true;
        }
      }
      break;
  } // switch

  // clear the voice notes map
  fVoiceNotesMap.clear ();

  fFirstNoteCloneInVoice = nullptr;
}

void msr2lpsrTranslator::visitEnd (S_msrVoice& elt)
{
  gIndenter--;

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrVoice \"" <<
      elt->getVoiceName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  switch (elt->getVoiceKind ()) {
    case msrVoice::kVoiceRegular:
      // JMI
      break;

    case msrVoice::kVoiceHarmony:
      fOnGoingHarmonyVoice = false;
      break;

    case msrVoice::kVoiceFiguredBass:
      fOnGoingFiguredBassVoice = false;
      break;
  } // switch
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrVoiceStaffChange& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrVoiceStaffChange '" <<
      elt->asString () << "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create a voice staff change clone
  S_msrVoiceStaffChange
    voiceStaffChangeClone =
      elt->
        createStaffChangeNewbornClone ();

  // append it to the current voice clone
  fCurrentVoiceClone->
    appendVoiceStaffChangeToVoice (
      voiceStaffChangeClone);
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrSegment& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create a clone of the segment
  fCurrentSegmentClone =
    elt->createSegmentNewbornClone (
      fCurrentVoiceClone);

  // set it as the new voice last segment
  fCurrentVoiceClone->
    setVoiceLastSegmentInVoiceClone (
      fCurrentSegmentClone);
}

void msr2lpsrTranslator::visitEnd (S_msrSegment& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    handleSegmentCloneEndInVoiceClone (
      inputLineNumber,
      fCurrentSegmentClone);

  // forget current segment clone
  fCurrentSegmentClone = nullptr;
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrHarmony& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrHarmony '" <<
      elt->asString () <<
      ", fOnGoingNonGraceNote: " << booleanAsString (fOnGoingNonGraceNote) <<
      ", fOnGoingChord: " << booleanAsString (fOnGoingChord) <<
      ", fOnGoingHarmonyVoice: " << booleanAsString (fOnGoingHarmonyVoice) <<
      ", fOnGoingHarmony: " << booleanAsString (fOnGoingHarmony) <<
      "', line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create a harmony new born clone
  fCurrentHarmonyClone =
    elt->
      createHarmonyNewbornClone (
        fCurrentVoiceClone);

  if (fOnGoingNonGraceNote) {
    // register the harmony in the current non-grace note clone
    fCurrentNonGraceNoteClone->
      appendHarmonyToNoteHarmoniesList (
        fCurrentHarmonyClone);

    // don't append the harmony to the part harmony,
    // this has been done in pass2b // JMI ???
  }

  else if (fOnGoingChord) {
    // register the harmony in the current chord clone
    fCurrentChordClone->
      appendHarmonyToChord (fCurrentHarmonyClone); // JMI
  }

  else if (fOnGoingHarmonyVoice) {
  /* JMI
    // get the harmony whole notes offset
    rational
      harmonyWholeNotesOffset =
        elt->getHarmonyWholeNotesOffset ();

    // is harmonyWholeNotesOffset not equal to 0?
    if (harmonyWholeNotesOffset.getNumerator () != 0) {
      // create skip with duration harmonyWholeNotesOffset
      S_msrNote
        skip =
          msrNote::createSkipNote (
            elt->                getInputLineNumber (),
            "666", // JMI elt->                getHarmonyMeasureNumber (),
            elt->                getHarmonyDisplayWholeNotes (), // would be 0/1 otherwise JMI
            elt->                getHarmonyDisplayWholeNotes (),
            0, // JMI elt->                getHarmonyDotsNumber (),
            fCurrentVoiceClone-> getRegularVoiceStaffSequentialNumber (), // JMI
            fCurrentVoiceClone-> getVoiceNumber ());

      // append it to the current voice clone
      // to 'push' the harmony aside
      fCurrentVoiceClone->
        appendNoteToVoice (skip);
    }
*/

    // append the harmony to the current voice clone
    fCurrentVoiceClone->
      appendHarmonyToVoiceClone (
        fCurrentHarmonyClone);
  }

  else {
    stringstream s;

    s <<
      "harmony is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  fOnGoingHarmony = true;
}

void msr2lpsrTranslator::visitStart (S_msrHarmonyDegree& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_msrHarmonyDegree '" <<
      elt->asString () <<
      ", fOnGoingNonGraceNote: " << booleanAsString (fOnGoingNonGraceNote) <<
      ", fOnGoingChord: " << booleanAsString (fOnGoingChord) <<
      ", fOnGoingHarmonyVoice: " << booleanAsString (fOnGoingHarmonyVoice) <<
      ", fOnGoingHarmony: " << booleanAsString (fOnGoingHarmony) <<
      "', line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append the harmony degree to the current harmony clone
  fCurrentHarmonyClone->
    appendHarmonyDegreeToHarmony (
      elt);
}

void msr2lpsrTranslator::visitEnd (S_msrHarmony& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrHarmony '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentHarmonyClone = nullptr;
  fOnGoingHarmony = false;
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrFrame& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrFrame '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingHarmony) {
    // register the frame in the current non-grace note clone
    fCurrentHarmonyClone->
      setHarmonyFrame (elt);
  }

  else {
    stringstream s;

    s <<
      "frame is out of context, cannot be handled:'" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrFiguredBass& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrFiguredBass '" <<
      elt->asString () <<
      "'" <<
      ", fOnGoingFiguredBassVoice = " << booleanAsString (fOnGoingFiguredBassVoice) <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create a figured bass new born clone
  fCurrentFiguredBassClone =
    elt->
      createFiguredBassNewbornClone (
        fCurrentVoiceClone);

  if (fOnGoingNonGraceNote) {
    // append the figured bass to the current non-grace note clone
    fCurrentNonGraceNoteClone->
      appendFiguredBassToNoteFiguredBassesList (fCurrentFiguredBassClone);

    // don't append the figured bass to the part figured bass,  JMI ???
    // this will be done below
  }

  /* JMI
  else if (fOnGoingChord) {
    // register the figured bass in the current chord clone
    fCurrentChordClone->
      setChordFiguredBass (fCurrentFiguredBassClone); // JMI
  }
  */

  else if (fOnGoingFiguredBassVoice) { // JMI
    /*
    // register the figured bass in the part clone figured bass
    fCurrentPartClone->
      appendFiguredBassToPartClone (
        fCurrentVoiceClone,
        fCurrentFiguredBassClone);
        */
    // append the figured bass to the current voice clone
    fCurrentVoiceClone->
      appendFiguredBassToVoiceClone (
        fCurrentFiguredBassClone);
  }

  else {
    stringstream s;

    s <<
      "figured bass is out of context, cannot be handled:'" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2lpsrTranslator::visitStart (S_msrFigure& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrFigure '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append the figure to the current figured bass
  fCurrentFiguredBassClone->
    appendFigureToFiguredBass (
      elt);
}

void msr2lpsrTranslator::visitEnd (S_msrFiguredBass& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrFiguredBass '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentFiguredBassClone = nullptr;
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrMeasure& elt)
{
  int
    inputLineNumber =
      elt->getInputLineNumber ();

  string
    measureNumber =
      elt->getMeasureElementMeasureNumber ();

  int
    measurePuristNumber =
      elt->getMeasurePuristNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMeasure '" <<
      measureNumber <<
      "', measurePuristNumber = '" <<
      measurePuristNumber <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasures ()) {
    gLogStream <<
      endl <<
      "<!--=== measure '" << measureNumber <<
      "', voice \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber << " ===-->" <<
      endl;
  }
#endif

  // create a clone of the measure
  fCurrentMeasureClone =
    elt->
      createMeasureNewbornClone (
        fCurrentSegmentClone);
/* JMI
  // is this a full measures rest?
  if (elt->getMeasureIsAFullMeasureRest ()) {
    // yes

    // should we compress full measures rests?
    if (gGlobalLpsr2lilypondOahGroup->getCompressFullMeasureRests ()) {
      // yes

      if (! fCurrentRestMeasure) {
        // this is the first full measure rest in the sequence

        // create a rest measures  containing fCurrentMeasureClone
        fCurrentRestMeasures =
          msrRestMeasures::create (
            inputLineNumber,
            fCurrentMeasureClone,
            fCurrentVoiceClone);

/ * JMI
        // append the current rest measures to the current voice clone
        fCurrentVoiceClone->
          appendRestMeasuresToVoice (
            inputLineNumber,
            fCurrentRestMeasures);
            * /
      }

      else {
        // this is a subsequent full measure rest, merely append it
        fCurrentRestMeasures->
          appendMeasureCloneToRestMeasures (
            fCurrentMeasureClone);
      }

      fCurrentRestMeasure = fCurrentMeasureClone;
    }

    else {
      // no

      // append current measure clone to the current voice clone
      fCurrentVoiceClone->
        appendMeasureCloneToVoiceClone (
          inputLineNumber,
          fCurrentMeasureClone);
    }
  }

  else {
    // no

    // append current measure clone to the current voice clone
    fCurrentVoiceClone->
      appendMeasureCloneToVoiceClone (
        inputLineNumber,
        fCurrentMeasureClone);
  }
  */

  // append current measure clone to the current voice clone
  fCurrentVoiceClone->
    appendMeasureCloneToVoiceClone (
      inputLineNumber,
      fCurrentMeasureClone);

  // JMI superflous???
  fCurrentPartClone->
    setPartCurrentMeasureNumber (
      measureNumber);

  // should the last bar check's measure purist number be set?
  if (fLastBarCheck) {
    fLastBarCheck->
      setNextBarPuristNumber (
        measurePuristNumber);

    fLastBarCheck = nullptr;
  }
}

void msr2lpsrTranslator::visitEnd (S_msrMeasure& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  string
    measureNumber =
      elt->getMeasureElementMeasureNumber ();

  string
    nextMeasureNumber =
      elt->getNextMeasureNumber ();

  int
    measurePuristNumber =
      elt->getMeasurePuristNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMeasure '" <<
      measureNumber <<
      "', nextMeasureNumber = '" <<
      nextMeasureNumber <<
      "', measurePuristNumber = '" <<
      measurePuristNumber <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  fCurrentMeasureClone->
    finalizeMeasureClone (
      inputLineNumber,
      elt, // original measure
      fCurrentVoiceClone);

  bool doCreateABarCheck = false;

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
          "\" is of unknown kind in msr2lpsrTranslator";

      // JMI  msrInternalError (
        msrInternalWarning (
          gGlobalOahOahGroup->getInputSourceName (),
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
    case msrMeasure::kMeasureKindIncompleteLastInRepeatCommonPart:
    case msrMeasure::kMeasureKindIncompleteLastInRepeatHookedEnding:
    case msrMeasure::kMeasureKindIncompleteLastInRepeatHooklessEnding:
    case msrMeasure::kMeasureKindIncompleteNextMeasureAfterCommonPart:
    case msrMeasure::kMeasureKindIncompleteNextMeasureAfterHookedEnding:
    case msrMeasure::kMeasureKindIncompleteNextMeasureAfterHooklessEnding:
      // generate a bar check if relevant
      switch (elt-> getMeasureEndRegularKind ()) {
        case msrMeasure::kMeasureEndRegularKindUnknown:
          break;
        case msrMeasure::kMeasureEndRegularKindYes:
          doCreateABarCheck = true;
          break;
        case msrMeasure::kMeasureEndRegularKindNo:
          break;
      } // switch
      break;

    case msrMeasure::kMeasureKindOvercomplete:
      doCreateABarCheck = true;
      break;

    case msrMeasure::kMeasureKindCadenza:
      doCreateABarCheck = true;
      break;

    case msrMeasure::kMeasureKindMusicallyEmpty:
      // JMI
      break;
  } // switch

  // is this a full measures rest?
  if (elt->getMeasureIsAFullMeasureRest ()) {
    // yes JMI
  }

  else {
    // no

    // should we compress full measures rests?
    if (gGlobalLpsr2lilypondOahGroup->getCompressFullMeasureRests ()) {
      // yes

      if (fCurrentRestMeasures) {
        // append the current rest measures to the current voice clone
        fCurrentVoiceClone->
          appendRestMeasuresToVoice (
            inputLineNumber,
            fCurrentRestMeasures);

        // forget about the current rest measure
        fCurrentRestMeasure = nullptr;

        // forget about the current rest measures
        fCurrentRestMeasures = nullptr;
      }

      else {
        stringstream s;

        s <<
          "fCurrentRestMeasures is null upon full measure rest end" <<
          measureNumber <<
          "', measurePuristNumber = '" <<
          measurePuristNumber <<
          "', line " << inputLineNumber;

/* JMI ???
        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
          */
      }
    }
  }

  if (doCreateABarCheck) {
    // create a bar check
    int voiceCurrentMeasurePuristNumber =
      fCurrentVoiceClone->
        getVoiceCurrentMeasurePuristNumber ();

    fLastBarCheck =
      msrBarCheck::createWithNextBarPuristNumber (
        inputLineNumber,
        nextMeasureNumber,
        voiceCurrentMeasurePuristNumber);

    // append it to the current voice clone
    fCurrentVoiceClone->
      appendBarCheckToVoice (fLastBarCheck);

    // create a bar number check
    // should NOT be done in cadenza, SEE TO IT JMI
    S_msrBarNumberCheck
      barNumberCheck_ =
        msrBarNumberCheck::create (
          inputLineNumber,
          nextMeasureNumber,
          voiceCurrentMeasurePuristNumber);

    // append it to the current voice clone
    fCurrentVoiceClone->
      appendBarNumberCheckToVoice (barNumberCheck_);
  }
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrStanza& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
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

    // append the stanza clone to the LPSR score elements list
    fResultingLpsrScore ->
      appendStanzaToLpsrScoreElementsList (
        fCurrentStanzaClone);

    // append a use of the stanza to the current staff block
    fCurrentStaffBlock ->
      appendLyricsUseToStaffBlock (
        fCurrentStanzaClone);
//  }
//  else
  //  fCurrentStanzaClone = nullptr; // JMI

  fOnGoingStanza = true;
}

void msr2lpsrTranslator::visitEnd (S_msrStanza& elt)
{
  gIndenter--;

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
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
void msr2lpsrTranslator::visitStart (S_msrSyllable& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
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

  else if (fOnGoingNonGraceNote) { // JMI
    // visiting a syllable as attached to the current non-grace note
    fCurrentSyllableClone->
      appendSyllableToNoteAndSetItsNoteUpLink (
        fCurrentNonGraceNoteClone);

    if (gGlobalLpsrOahGroup->getAddWordsFromTheLyrics ()) {
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
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceLyrics ()) {
          gLogStream <<
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
              kHorizontalAlignmentNone,      // default value
              kVerticalAlignmentNone,        // default value
              kFontStyleNone,                // default value
              msrFontSize::create (
                msrFontSize::kFontSizeNone), // default value
              kFontWeightNone,               // default value
              kXMLLangIt,                    // default value
              elt->
                getSyllableNoteUpLink ()->
                  fetchNoteStaffUpLink ()->
                    getStaffNumber ());

        // append it to the current non-grace note
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceWords ()) {
          gLogStream <<
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
  else {
    stringstream s;

    s <<
      "syllable is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "' ";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // a syllable ends the sysllable extend range if any
  if (fOnGoingSyllableExtend) {
    /* JMI ???
    // create melisma end command
    S_lpsrMelismaCommand
      melismaCommand =
        lpsrMelismaCommand::create (
          inputLineNumber,
          lpsrMelismaCommand::kMelismaEnd);

    // append it to current voice clone
    fCurrentVoiceClone->
      appendOtherElementToVoice (melismaCommand);
*/

    fOnGoingSyllableExtend = false;
  }
}

void msr2lpsrTranslator::visitEnd (S_msrSyllable& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSyllable" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrClef& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrClef" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendClefToVoice (elt);
}

void msr2lpsrTranslator::visitEnd (S_msrClef& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrClef" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrKey& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrKey" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendKeyToVoice (elt);
}

void msr2lpsrTranslator::visitEnd (S_msrKey& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrKey" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrTime& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTime" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append time to voice clone
  fCurrentVoiceClone->
    appendTimeToVoice (elt);
}

void msr2lpsrTranslator::visitEnd (S_msrTime& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTime" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrTranspose& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTranspose" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append transpose to voice clone
  fCurrentVoiceClone->
    appendTransposeToVoice (elt);
}

void msr2lpsrTranslator::visitEnd (S_msrTranspose& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTranspose" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrPartNameDisplay& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
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
void msr2lpsrTranslator::visitStart (S_msrPartAbbreviationDisplay& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
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
void msr2lpsrTranslator::visitStart (S_msrTempo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTempo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  switch (elt->getTempoKind ()) {
    case msrTempo::k_NoTempoKind:
      break;

    case msrTempo::kTempoBeatUnitsWordsOnly:
      break;

    case msrTempo::kTempoBeatUnitsPerMinute:
      break;

    case msrTempo::kTempoBeatUnitsEquivalence:
      break;

    case msrTempo::kTempoNotesRelationShip:
      fResultingLpsrScore->
        // this score needs the 'tempo relationship' Scheme function
        setTempoRelationshipSchemeFunctionIsNeeded ();
      break;
  } // switch

  if (gGlobalLpsrOahGroup->getConvertTemposToRehearsalMarks ()) {
    // create a rehearsal mark containing elt's words

    S_msrRehearsal
      rehearsal =
        msrRehearsal::create (
          elt->getInputLineNumber (),
          msrRehearsal::kNone,
          elt->tempoWordsListAsString (" "), //JMI ???
          elt->getTempoPlacementKind ());

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceTempos ()) {
      gLogStream <<
        "Converting tempos '" <<
        elt->asShortString () <<
        "' to rehearsal mark '" <<
        rehearsal->asShortString () <<
        "'" <<
        endl;
    }
#endif

    // append the rehearsal to the current voice clone
    fCurrentVoiceClone->
      appendRehearsalToVoice (rehearsal);
  }

  else {
    fCurrentVoiceClone->
      appendTempoToVoice (elt);
  }
}

void msr2lpsrTranslator::visitEnd (S_msrTempo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTempo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrRehearsal& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrRehearsal" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendRehearsalToVoice (elt);
}

void msr2lpsrTranslator::visitEnd (S_msrRehearsal& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrRehearsal" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrArticulation& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrArticulation" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendArticulationToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendArticulationToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "articulation is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2lpsrTranslator::visitEnd (S_msrArticulation& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrArticulation" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrFermata& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrFermata" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // a fermata is an articulation

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendArticulationToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendArticulationToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "fermata is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrArpeggiato& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrArpeggiato" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // an arpeggiato is an articulation

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendArticulationToNote (elt); // addArpeggiatoToNote ??? JMI
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendArticulationToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "arpeggiato is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrNonArpeggiato& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrNonArpeggiato" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // an nonArpeggiato is an articulation

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendArticulationToNote (elt); // addArpeggiatoToNote ??? JMI
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendArticulationToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "nonArpeggiato is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "' ";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrTechnical& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTechnical" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "technical is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // doest the score need the 'tongue' function?
  switch (elt->getTechnicalKind ()) {
    case msrTechnical::kArrow:
      break;
    case msrTechnical::kDoubleTongue:
      fResultingLpsrScore->
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
      fResultingLpsrScore->
        // this score needs the 'tongue' Scheme function
        setTongueSchemeFunctionIsNeeded ();
      break;
    case msrTechnical::kUpBow:
      break;
  } // switch
}

void msr2lpsrTranslator::visitEnd (S_msrTechnical& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTechnical" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrTechnicalWithInteger& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTechnicalWithInteger" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalWithIntegerToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalWithIntegerToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "technicalWithInteger is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2lpsrTranslator::visitEnd (S_msrTechnicalWithInteger& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTechnicalWithInteger" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrTechnicalWithFloat& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTechnicalWithFloat" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalWithFloatToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalWithFloatToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "technicalWithFloat is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2lpsrTranslator::visitEnd (S_msrTechnicalWithFloat& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTechnicalWithFloat" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrTechnicalWithString& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTechnicalWithString" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalWithStringToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalWithStringToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "technicalWithString is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  switch (elt->getTechnicalWithStringKind ()) {
    case msrTechnicalWithString::kHammerOn:
    case msrTechnicalWithString::kPullOff:
      // this score needs the 'after' Scheme function
      fResultingLpsrScore->
        setAfterSchemeFunctionIsNeeded ();
      break;
    default:
      ;
  } // switch
}

void msr2lpsrTranslator::visitEnd (S_msrTechnicalWithString& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTechnicalWithString" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrOrnament& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrOrnament " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOrnaments ()) {
      displayOnGoingNotesStack ("visitStart (S_msrOrnament&)");
    }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendOrnamentToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendOrnamentToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "ornament is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2lpsrTranslator::visitEnd (S_msrOrnament& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrOrnament " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrSpanner& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
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

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendSpannerToNote (elt);
  }

  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendSpannerToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "spanner is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2lpsrTranslator::visitEnd (S_msrSpanner& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSpanner" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrGlissando& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrGlissando" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendGlissandoToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendGlissandoToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "glissando is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  if (elt->getGlissandoTextValue ().size ()) {
    fResultingLpsrScore->
      // this score needs the 'glissandoWithText' Scheme functions
      addGlissandoWithTextSchemeFunctionsToScore ();
  }
}

void msr2lpsrTranslator::visitEnd (S_msrGlissando& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrGlissando" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrSlide& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSlide" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendSlideToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendSlideToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "slide is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2lpsrTranslator::visitEnd (S_msrSlide& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSlide" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrSingleTremolo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSingleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      setNoteSingleTremolo (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      setChordSingleTremolo (elt);
  }
  else {
    stringstream s;

    s <<
      "singleTremolo is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2lpsrTranslator::visitEnd (S_msrSingleTremolo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSingleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrDoubleTremolo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrDoubleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create a double tremolo clone from the two elements
  fCurrentDoubleTremoloClone = elt; // JMI FIX THAT
/* JMI
    elt->createDoubleTremoloNewbornClone (
      elt->getDoubleTremoloFirstElement ()->
        createNewBornClone (),
      elt->getDoubleTremoloSecondElement ()
        createNewBornClone ());
        */

  fOnGoingDoubleTremolo = true;
}

void msr2lpsrTranslator::visitEnd (S_msrDoubleTremolo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
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
void msr2lpsrTranslator::visitStart (S_msrDynamics& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrDynamics" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
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
      fResultingLpsrScore->
        setDynamicsSchemeFunctionIsNeeded ();
    }
  }

  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendDynamicsToChord (elt);
  }

  else {
    stringstream s;

    s <<
      "dynamics is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2lpsrTranslator::visitEnd (S_msrDynamics& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrDynamics" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrOtherDynamics& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrOtherDynamics" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendOtherDynamicsToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendOtherDynamicsToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "otherDynamics is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  fResultingLpsrScore->
    // this score needs the 'otherDynamic' Scheme function
    setOtherDynamicSchemeFunctionIsNeeded ();
}

void msr2lpsrTranslator::visitEnd (S_msrOtherDynamics& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrOtherDynamics" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrWords& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrWords" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote || fOnGoingChord) {
    bool wordsHasBeenHandled = false;

    if (gGlobalLpsrOahGroup->getConvertWordsToTempo ()) {
      // create a tempo containing elt
      S_msrTempo
        tempo =
          msrTempo::create (
            inputLineNumber,
            elt);

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceWords ()) {
        gLogStream <<
          "Converting words '" <<
          elt->asShortString () <<
          "' to tempo '" <<
          tempo->asShortString () <<
          "'" <<
          endl;
      }
#endif

      // append the tempo to the current voice clone
      fCurrentVoiceClone->
        appendTempoToVoice (tempo);

      wordsHasBeenHandled = true;
    }

    else if (gGlobalLpsrOahGroup->getConvertWordsToRehearsalMarks ()) {
      // create a rehearsal mark containing elt's words contents
      S_msrRehearsal
        rehearsal =
          msrRehearsal::create (
            inputLineNumber,
            msrRehearsal::kNone,
            elt->getWordsContents (),
            elt->getWordsPlacementKind ()); // above ??? JMI

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceWords ()) {
        gLogStream <<
          "Converting words '" <<
          elt->asShortString () <<
          "' to rehearsal mark '" <<
          rehearsal->asShortString () <<
          "'" <<
          endl;
      }
#endif

      // append the rehearsal to the current voice clone
      fCurrentVoiceClone->
        appendRehearsalToVoice (rehearsal);

      wordsHasBeenHandled = true;
    }

    else {
    /* JMI
      string wordsContents = elt->getWordsContents ();

      // is this words contents in the string to dal segno kind map?
      map<string, msrDalSegno::msrDalSegnoKind>::iterator
        it =
          gGlobalLpsrOahGroup->getConvertWordsToDalSegno ().find (wordsContents);

      if (it != gGlobalLpsrOahGroup->getConvertWordsToDalSegno ().end ()) {
        // yes
        msrDalSegno::msrDalSegnoKind
          dalSegnoKind =
            (*it).second;

        // create a dal segno element containing elt's words contents
        S_msrDalSegno
          dalSegno =
            msrDalSegno::create (
              inputLineNumber,
              dalSegnoKind,
              wordsContents,
              elt->getWordsStaffNumber ());

#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceWords ()) {
          gLogStream <<
            "Converting words '" <<
            elt->asShortString () <<
            "' to dal segno element '" <<
            dalSegno->asShortString () <<
            "'" <<
            endl;
        }
#endif

        if (fOnGoingNonGraceNote) {
          fCurrentNonGraceNoteClone->
            appendDalSegnoToNote (dalSegno);
        }
        else if (fOnGoingChord) {
          fCurrentChordClone->
            appendDalSegnoToChord (dalSegno);
        }

      wordsHasBeenHandled = true;
      }
      */
    }

    if (! wordsHasBeenHandled) {
      if (fOnGoingNonGraceNote) {
        fCurrentNonGraceNoteClone->
          appendWordsToNote (elt);
      }
      else if (fOnGoingChord) {
        fCurrentChordClone->
          appendWordsToChord (elt);
      }
    }
  }

  else {
    stringstream s;

    s <<
      "words is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2lpsrTranslator::visitEnd (S_msrWords& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrWords" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrSlur& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSlur " <<
      elt->asShortString () <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (gGlobalTraceOahGroup->getTraceSlurs ()) {
    gLogStream <<
      "--> visitStart (S_msrSlur&), " <<
      elt->asShortString () <<
      ", fOnGoingNotesStack.size (): " <<
      fOnGoingNotesStack.size () <<
      ", onGoingChord: " <<
      booleanAsString (fOnGoingChord) <<
      ", onGoingNonGraceNote: " <<
      booleanAsString (fOnGoingNonGraceNote) <<
      ", onGoingGraceNotesGroup: " <<
      booleanAsString (fOnGoingGraceNotesGroup) <<
      ", onGoingChordSlurLink: " <<
      booleanAsString (fOnGoingChordSlurLink) <<
      endl;
  }
#endif

  /* JMI ???
    Only the  first note of the chord should get the slur notation.
    Some applications print out the slur for all notes,
    i.e. a stop and a start in sequence:
    these should be ignored
  */

  if (fOnGoingNotesStack.size () > 0) {
//    if (fOnGoingNonGraceNote) {
      fOnGoingNotesStack.front ()->
        appendSlurToNote (elt);
//    }
  }

  else if (fOnGoingChord) {
    // don't append a slur if we're inside a slur link JMI ???
    if (fOnGoingNonGraceNote) {
      S_msrChordSlurLink
        chordSlurLink =
          msrChordSlurLink::create (
            fCurrentChordClone->getInputLineNumber (),
            elt,
            fCurrentChordClone);

      fCurrentChordClone->
        appendChordSlurLinkToChord (chordSlurLink);
    }
  }

  else {
    displayCurrentOnGoingValues ();

    stringstream s;

    s <<
      "slur is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2lpsrTranslator::visitEnd (S_msrSlur& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSlur " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrChordSlurLink& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrChordSlurLink " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  S_msrSlur originalSlur = elt->getOriginalSlur ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceSlurs ()) {
    gLogStream <<
      "--> visitStart (S_msrChordSlurLink&), " <<
      elt->asShortString () <<
      ", originalSlur: " <<
      originalSlur->asShortString () <<
      endl;
  }
#endif

  if (fOnGoingChord) {
    S_msrChordSlurLink
      chordSlurLink =
        msrChordSlurLink::create (
          elt->getInputLineNumber (),
          originalSlur,
          fCurrentChordClone);

    fCurrentChordClone->
      appendChordSlurLinkToChord (chordSlurLink);
  }

  else {
    displayCurrentOnGoingValues ();

    stringstream s;

    s <<
      "slur link is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  fOnGoingChordSlurLink = true;
}

void msr2lpsrTranslator::visitEnd (S_msrChordSlurLink& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrChordSlurLink " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fOnGoingChordSlurLink = false;
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrLigature& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrLigature" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendLigatureToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendLigatureToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "ligature is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2lpsrTranslator::visitEnd (S_msrLigature& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrLigature" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrSlash& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSlash" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendSlashToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendSlashToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "slash is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrWedge& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrWedge" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendWedgeToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendWedgeToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "wedge is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2lpsrTranslator::visitEnd (S_msrWedge& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrWedge" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrGraceNotesGroup& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber () ;

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrGraceNotesGroup " <<
      elt->asShortString () <<
      ", fOnGoingNotesStack.size (): " << fOnGoingNotesStack.size () <<
      ", fOnGoingChord: " << fOnGoingChord <<
      ", fOnGoingChordGraceNotesGroupLink: " <<
        booleanAsString (fOnGoingChordGraceNotesGroupLink) <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  bool doCreateAGraceNoteClone = true; // JMI

  if (doCreateAGraceNoteClone) {
    // create a clone of this graceNotesGroup
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
      gLogStream <<
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
    // if (fOnGoingNonGraceNote) { JMI
   // { // JMI

/*
    if (fOnGoingChord) {
      switch (elt->getGraceNotesGroupKind ()) {
        case msrGraceNotesGroup::kGraceNotesGroupBefore:
          fCurrentChordClone->
            setChordGraceNotesGroupBefore (
              fCurrentGraceNotesGroupClone);
          break;
        case msrGraceNotesGroup::kGraceNotesGroupAfter:
          fCurrentChordClone->
            setChordGraceNotesGroupAfter (
              fCurrentGraceNotesGroupClone);
          break;
      } // switch
    }
    else {
    */

    if (fOnGoingNotesStack.size ()) {
      switch (elt->getGraceNotesGroupKind ()) {
        case msrGraceNotesGroup::kGraceNotesGroupBefore:
      //    fCurrentNonGraceNoteClone-> JMI
          fOnGoingNotesStack.front ()->
            setNoteGraceNotesGroupBefore (
              fCurrentGraceNotesGroupClone);
          break;
        case msrGraceNotesGroup::kGraceNotesGroupAfter:
      //    fCurrentNonGraceNoteClone-> JMI
          fOnGoingNotesStack.front ()->
            setNoteGraceNotesGroupAfter (
              fCurrentGraceNotesGroupClone);
          break;
      } // switch
    }

    else if (fOnGoingChordGraceNotesGroupLink) {
      // JMI ???
    }

    else {
      stringstream s;

      s <<
        "graceNotesGroup is out of context, fOnGoingNotesStack.size () == 0, cannot be handled: '" <<
        elt->asShortString () <<
        "'";

      msrInternalError (
        gGlobalOahOahGroup->getInputSourceName (),
        elt->getInputLineNumber (),
        __FILE__, __LINE__,
        s.str ());
    }
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "+++++++++++++++++++++++++ visitStart (S_msrGraceNotesGroup&)" <<
      endl <<
      "fCurrentNonGraceNoteClone: ";

    if (fCurrentNonGraceNoteClone) {
      fCurrentNonGraceNoteClone->printShort (
        gLogStream);
    }
    else {
      gLogStream <<
        "nullptr";
    }
    gLogStream << endl;
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
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fOnGoingGraceNotesGroup = true;

  // is noteNotesGroupIsAttachedTo the first one in its voice?
#ifdef TRACING_IS_ENABLED
  if (false && gGlobalTraceOahGroup->getTraceGraceNotes ()) { // JMI
    gLogStream <<
      "The noteNotesGroupIsAttachedTo voice clone FIRST_ONE??? '" <<
      fCurrentVoiceClone->getVoiceName () <<
      "' is '";

    if (noteNotesGroupIsAttachedTo) {
      gLogStream <<
        noteNotesGroupIsAttachedTo->asShortString ();
    }
    else {
      gLogStream <<
        "none";
    }
    gLogStream <<
       "'" <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (false && gGlobalTraceOahGroup->getTraceGraceNotes ()) { // JMI
    gLogStream <<
      "The first note of voice clone KLJWLPOEF '" <<
      fCurrentVoiceClone->getVoiceName () <<
      "' is '";

    if (fFirstNoteCloneInVoice) {
      gLogStream <<
        fFirstNoteCloneInVoice->asShortString ();
    }
    else {
      gLogStream <<
        "none";
    }
    gLogStream <<
       "'" <<
      endl;
  }
#endif

  // fetch the original voice first non grace note
  S_msrNote
    originalVoiceFirstNonGraceNote =
      fCurrentVoiceOriginal->
        fetchVoiceFirstNonGraceNote ();

  if (originalVoiceFirstNonGraceNote) { // JMI
    if (noteNotesGroupIsAttachedTo == originalVoiceFirstNonGraceNote) {
    // don't createSkipGraceNotesGroupClone() is there's only a single voice JMI

      // issue #34 in LilyPond should be worked around by creating
      // skip grace notes in the other voices of the part

      // create the skip grace notes group
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
          gLogStream <<
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

void msr2lpsrTranslator::visitEnd (S_msrGraceNotesGroup& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrGraceNotesGroup" <<
      ", fOnGoingNotesStack.size (): " << fOnGoingNotesStack.size () <<
      ", fOnGoingChord: " << fOnGoingChord <<
      ", fOnGoingChordGraceNotesGroupLink: " <<
        booleanAsString (fOnGoingChordGraceNotesGroupLink) <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "+++++++++++++++++++++++++ visitEnd (S_msrGraceNotesGroup&)" <<
      endl <<
      "fCurrentNonGraceNoteClone: ";

    if (fCurrentNonGraceNoteClone) {
      fCurrentNonGraceNoteClone->printShort (
        gLogStream);
    }
    else {
      gLogStream <<
        "nullptr";
    }
    gLogStream << endl;
  }
#endif

  // forget about these grace notes
  fCurrentGraceNotesGroupClone = nullptr;

  fOnGoingGraceNotesGroup = false;

//* JMI
  if (fPendingAfterGraceNotesGroup) {
    // remove the current afterGraceNotesGroup note clone
    // from the current voice clone
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
      gLogStream <<
        "Removing the after grace notes element from the current voice clone" <<
        endl;
    }
#endif

    fCurrentVoiceClone->
      removeElementFromVoice (
        elt->getInputLineNumber (),
        fCurrentAfterGraceNotesGroupElement);

    // forget about the current after grace notes element
    fCurrentAfterGraceNotesGroupElement = nullptr;

    // forget about these after the pending grace notes
    fPendingAfterGraceNotesGroup = nullptr;
  }
//  */
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrChordGraceNotesGroupLink& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrChordGraceNotesGroupLink " <<
      elt->asShortString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  S_msrGraceNotesGroup
    originalGraceNotesGroup =
      elt->getOriginalGraceNotesGroup ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "--> visitStart (S_msrChordGraceNotesGroupLink&), " <<
      elt->asShortString () <<
      ", originalGraceNotesGroup: " <<
      originalGraceNotesGroup->asShortString () <<
      ", fOnGoingNotesStack.size (): " << fOnGoingNotesStack.size () <<
      ", fOnGoingChord: " << fOnGoingChord <<
      ", fOnGoingChordGraceNotesGroupLink: " <<
        booleanAsString (fOnGoingChordGraceNotesGroupLink) <<
      endl;
  }
#endif

  if (fOnGoingChord) {
    S_msrChordGraceNotesGroupLink
      chordChordGraceNotesGroupLink =
        msrChordGraceNotesGroupLink::create (
          elt->getInputLineNumber (),
          originalGraceNotesGroup,
          fCurrentChordClone);

    switch (originalGraceNotesGroup->getGraceNotesGroupKind ()) {
      case msrGraceNotesGroup::kGraceNotesGroupBefore:
        fCurrentChordClone->
          setChordGraceNotesGroupLinkBefore (
            chordChordGraceNotesGroupLink);
        break;
      case msrGraceNotesGroup::kGraceNotesGroupAfter:
        fCurrentChordClone->
          setChordGraceNotesGroupLinkAfter (
            chordChordGraceNotesGroupLink);
        break;
    } // switch
  }

  else {
    displayCurrentOnGoingValues ();

    stringstream s;

    s <<
      "grace notes group link is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  fOnGoingChordGraceNotesGroupLink = true;
}

void msr2lpsrTranslator::visitEnd (S_msrChordGraceNotesGroupLink& elt)
{
#ifdef TRACING_IS_ENABLED
    S_msrGraceNotesGroup
      originalGraceNotesGroup =
        elt->getOriginalGraceNotesGroup ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrChordGraceNotesGroupLink" <<
      elt->asShortString () <<
      ", originalGraceNotesGroup: " <<
      originalGraceNotesGroup->asShortString () <<
      ", fOnGoingNotesStack.size (): " << fOnGoingNotesStack.size () <<
      ", fOnGoingChord: " << fOnGoingChord <<
      ", fOnGoingChordGraceNotesGroupLink: " << fOnGoingChordGraceNotesGroupLink <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fOnGoingChordGraceNotesGroupLink = false;
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrNote& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
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
      elt->
        createNoteNewbornClone (
          fCurrentPartClone);

  // register clone in this tranlastors' voice notes map and ongoing notes stack
  fVoiceNotesMap [elt] = noteClone; // JMI XXL
  fOnGoingNotesStack.push_front (noteClone);

  // don't register grace notes as the current note clone,
  // but as the current grace note clone instead
/* JMI
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
          gLogStream <<
            "The first note of voice clone GFFF '" <<
            fCurrentVoiceClone->getVoiceName () <<
            "' is '";

          if (fFirstNoteCloneInVoice) {
            gLogStream <<
              fFirstNoteCloneInVoice->asShortString ();
          }
          else {
            gLogStream <<
              "none";
          }
          gLogStream <<
             "'" <<
            endl;
        }
#endif
*/

  switch (elt->getNoteKind ()) {
    case msrNote::kGraceNote:
    case msrNote::kGraceSkipNote:
    case msrNote::kGraceChordMemberNote:
    case msrNote::kGraceTupletMemberNote:
      fCurrentGraceNoteClone = noteClone;
      break;

    default:
      fCurrentNonGraceNoteClone = noteClone;

      if (! fFirstNoteCloneInVoice) {
        fFirstNoteCloneInVoice =
          fCurrentNonGraceNoteClone;

#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceNotes ()) {
          gLogStream <<
            "The first note of voice clone RJIRWR '" <<
            fCurrentVoiceClone->getVoiceName () <<
            "' is '" <<
            fFirstNoteCloneInVoice->asShortString () <<
             "'" <<
            endl;
        }
#endif
      }

      fOnGoingNonGraceNote = true;
  } // switch

//* JMI
  // can we optimize graceNotesGroup into afterGraceNotesGroup?
  if (
    elt->getNoteIsFollowedByGraceNotesGroup ()
      &&
    elt->getNoteTrillOrnament ()
  ) {
    int inputLineNumber =
      elt->getInputLineNumber ();

    // yes, create the after grace notes
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
      gLogStream <<
        "Optimizing grace notes on trilled note '" <<
        elt->asShortString () <<
        "' as after grace notes " <<
        ", line " << inputLineNumber<<
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
//*/
}

void msr2lpsrTranslator::visitEnd (S_msrNote& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  msrNote::msrNoteKind
    noteKind = elt->getNoteKind ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrNote " <<
      elt->asString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceNotesDetails ()) {
    gLogStream <<
      "FAA fCurrentNonGraceNoteClone = " <<
      endl;
    if (fCurrentNonGraceNoteClone) {
      gLogStream <<
        fCurrentNonGraceNoteClone;
    }
    else {
      gLogStream <<
        "nullptr" <<
        endl;
    }

    gLogStream <<
      "FAA fCurrentGraceNoteClone = " <<
      endl;
    if (fCurrentGraceNoteClone) {
      gLogStream <<
        fCurrentGraceNoteClone;
    }
    else {
      gLogStream <<
        "nullptr" <<
        endl;
    }
  }
#endif

  switch (noteKind) {

    case msrNote::k_NoNoteKind:
      break;

    case msrNote::kRestNote:
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceNotes ()) {
        gLogStream <<
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
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceNotes ()) {
        gLogStream <<
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
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceNotes ()) {
        gLogStream <<
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
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceNotes ()) {
        gLogStream <<
          "Appending regular note clone '" <<
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
#ifdef TRACING_IS_ENABLED
          if (gGlobalTraceOahGroup->getTraceNotes ()) {
            gLogStream <<
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
#ifdef TRACING_IS_ENABLED
          if (gGlobalTraceOahGroup->getTraceNotes ()) {
            gLogStream <<
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
            gGlobalOahOahGroup->getInputSourceName (),
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
          gGlobalOahOahGroup->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;

    case msrNote::kGraceNote:
    case msrNote::kGraceSkipNote:
    /* JMI
      gLogStream <<
        "fOnGoingGraceNotesGroup = " <<
        booleanAsString (
          fOnGoingGraceNotesGroup) <<
        endl;
        */

      if (! fOnGoingGraceNotesGroup) {
        stringstream s;

        s <<
          "grace note '" << fCurrentNonGraceNoteClone->asShortString () <<
          "' found outside of grace notes";

        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      else {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
          gLogStream <<
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

    /* JMI ???
      if (fCurrentGraceNotesGroupClone) {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
          gLogStream <<
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
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
          gLogStream <<
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
          gGlobalOahOahGroup->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      */
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
          "msr2lpsrTranslator::visitEnd (S_msrNote& elt): chord member note " <<
          elt->asString () <<
          " appears outside of a chord";

        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
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
          "msr2lpsrTranslator::visitEnd (S_msrNote& elt): chord member note " <<
          elt->asString () <<
          " appears outside of a chord";

        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
        }
      break;

    case msrNote::kTupletMemberNote:
    case msrNote::kTupletRestMemberNote:
    case msrNote::kGraceTupletMemberNote:
    case msrNote::kTupletUnpitchedMemberNote:
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceNotes ()) {
        gLogStream <<
          "Appending note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "'' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          endl;
      }
#endif

      fTupletClonesStack.top ()->
        appendNoteToTuplet (
          fCurrentNonGraceNoteClone,
          fCurrentVoiceClone);
      break;
  } // switch

  if (fCurrentNonGraceNoteClone) { // JMI
    // handle editorial accidentals
    switch (fCurrentNonGraceNoteClone->getNoteEditorialAccidentalKind ()) {
      case kEditorialAccidentalYes:
        fResultingLpsrScore->
          // this score needs the 'editorial accidental' Scheme function
          setEditorialAccidentalSchemeFunctionIsNeeded ();
        break;
      case kEditorialAccidentalNo:
        break;
    } // switch

    // handle cautionary accidentals // JMI ???
    switch (fCurrentNonGraceNoteClone->getNoteCautionaryAccidentalKind ()) {
      case kCautionaryAccidentalYes:
        break;
      case kCautionaryAccidentalNo:
        break;
    } // switch
  }

/* JMI
  // handle melisma
  msrSyllable::msrSyllableExtendKind
    noteSyllableExtendKind =
      elt->getNoteSyllableExtendKind ();

  switch (noteSyllableExtendKind) {
    case msrSyllable::kStandaloneSyllableExtend:
      {
        / * JMI ???
        // create melisma start command
        S_lpsrMelismaCommand
          melismaCommand =
            lpsrMelismaCommand::create (
              inputLineNumber,
              lpsrMelismaCommand::kMelismaStart);

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
*/

  switch (noteKind) {
    case msrNote::kGraceNote:
    case msrNote::kGraceSkipNote:
    case msrNote::kGraceChordMemberNote:
    case msrNote::kGraceTupletMemberNote:
      break;

    default:
      fOnGoingNonGraceNote = false;
  } // switch

  // forget about current note
  fOnGoingNotesStack.pop_front ();
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrOctaveShift& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrOctaveShift" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      setNoteOctaveShift (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      setChordOctaveShift (elt);
  }
  else {
    stringstream s;

    s <<
      "octaveShift is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2lpsrTranslator::visitEnd (S_msrOctaveShift& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrOctaveShift" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrAccordionRegistration& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrAccordionRegistration" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append the accordion registration to the voice clone
  fCurrentVoiceClone->
    appendAccordionRegistrationToVoice (elt);

  // the generated code needs modules scm and accreg
  fResultingLpsrScore->
    setScmAndAccregSchemeModulesAreNeeded ();
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrHarpPedalsTuning& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
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
void msr2lpsrTranslator::visitStart (S_msrStem& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStem" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      setNoteStem (elt);
  }
  else if (fOnGoingChord) {
  if (false) // JMI
    fCurrentChordClone->
      appendStemToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "stem is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2lpsrTranslator::visitEnd (S_msrStem& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrStem" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrBeam& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBeam" <<
      ", line " << elt->getInputLineNumber () <<
// JMI      ", fOnGoingNonGraceNote: " << booleanAsString (fOnGoingNonGraceNote) <<
// JMI      ", fOnGoingChord: " << booleanAsString (fOnGoingChord) <<
      endl;
  }
#endif

  // a beam may be present at the same time
  // in a note or grace note and the chord the latter belongs to

  if (fOnGoingGraceNotesGroup) {
    fCurrentGraceNoteClone->
      appendBeamToNote (elt);
  }
  else if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendBeamToNote (elt);
  }

  if (fOnGoingChord) { // else ??? JMI
    // don't append a beam if we're inside a beam link JMI ???
    if (fOnGoingNonGraceNote) {
      S_msrChordBeamLink
        chordBeamLink =
          msrChordBeamLink::create (
            fCurrentChordClone->getInputLineNumber (),
            elt,
            fCurrentChordClone);

      fCurrentChordClone->
        appendChordBeamLinkToChord (chordBeamLink);
    }
  }
/* JMI
    fCurrentChordClone->
      appendBeamToChord (elt);
      */
}

void msr2lpsrTranslator::visitEnd (S_msrBeam& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrBeam" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrChordBeamLink& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrChordBeamLink " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  S_msrBeam originalBeam = elt->getOriginalBeam ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceBeams ()) {
    gLogStream <<
      "--> visitStart (S_msrChordBeamLink&), " <<
      elt->asShortString () <<
      ", originalBeam: " <<
      originalBeam->asShortString () <<
      endl;
  }
#endif

  if (fOnGoingChord) {
    S_msrChordBeamLink
      chordBeamLink =
        msrChordBeamLink::create (
          elt->getInputLineNumber (),
          originalBeam,
          fCurrentChordClone);

    fCurrentChordClone->
      appendChordBeamLinkToChord (chordBeamLink);
  }

  else {
    displayCurrentOnGoingValues ();

    stringstream s;

    s <<
      "beam link is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  fOnGoingChordBeamLink = true;
}

void msr2lpsrTranslator::visitEnd (S_msrChordBeamLink& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrChordBeamLink " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fOnGoingChordBeamLink = false;
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrChord& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrChord " <<
      elt->asShortString () <<
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
      appendChordToTuplet (
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
        gGlobalOahOahGroup->getInputSourceName (),
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

void msr2lpsrTranslator::visitEnd (S_msrChord& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrChord" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fOnGoingChord = false;
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrTuplet& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "++> pushing tuplet '" <<
      tupletClone->asString () <<
      "' to tuplets stack" <<
      endl;
  }
#endif

  fTupletClonesStack.push (tupletClone);

  switch (elt->getTupletLineShapeKind ()) {
    case msrTuplet::kTupletLineShapeStraight:
      break;
    case msrTuplet::kTupletLineShapeCurved:
      fResultingLpsrScore->
        // this score needs the 'tuplets curved brackets' Scheme function
        setTupletsCurvedBracketsSchemeFunctionIsNeeded ();
      break;
  } // switch
}

void msr2lpsrTranslator::visitEnd (S_msrTuplet& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTuplet" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Popping tuplet '" <<
      elt->asString () <<
      "' from tuplets stack" <<
      endl;
  }
#endif

  fTupletClonesStack.pop ();

  if (fTupletClonesStack.size ()) {
    // tuplet is a nested tuplet
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceTuplets ()) {
      gLogStream <<
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

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceTuplets ()) {
      gLogStream <<
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
void msr2lpsrTranslator::visitStart (S_msrTie& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTie" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      setNoteTie (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTieToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "tie is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2lpsrTranslator::visitEnd (S_msrTie& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTie" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrSegno& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSegno" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingChord || fOnGoingNonGraceNote) {
    if (fOnGoingChord) {
      fCurrentChordClone->
        appendSegnoToChord (elt);
    }
    if (fOnGoingNonGraceNote) {
      fCurrentNonGraceNoteClone->
        appendSegnoToNote (elt);
    }
  }
  else {
    stringstream s;

    s <<
      "segno is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2lpsrTranslator::visitStart (S_msrDalSegno& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrDalSegno" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (fOnGoingChord || fOnGoingNonGraceNote) {
    if (fOnGoingChord) {
      fCurrentChordClone->
        appendDalSegnoToChord (elt);
    }
    if (fOnGoingNonGraceNote) {
      fCurrentNonGraceNoteClone->
        appendDalSegnoToNote (elt);
    }
  }
  else {
    stringstream s;

    s <<
      "dal segno is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // this score needs hiddenMeasureAndBarline
  fResultingLpsrScore->
    setHiddenMeasureAndBarlineIsNeeded ();

/* JMI
  // create a hidden measure and barline
  S_msrHiddenMeasureAndBarline
    hiddenMeasureAndBarline =
      msrHiddenMeasureAndBarline::create (
        inputLineNumber);

  // create a hidden measure and barline descr
  S_msrHiddenMeasureAndBarlineDescr
    hiddenMeasureAndBarlineDescr =
      msrHiddenMeasureAndBarlineDescr::create (
        inputLineNumber,
        positionInMeasure);
*/

  // register it in the hidden measure and barline descr list
  fPartHiddenMeasureAndBarlineDescrList.push_back (
    msrHiddenMeasureAndBarlineDescr::create (
      inputLineNumber,
       elt));

//* JMI BLARK
  fCurrentPartClone->
    insertHiddenMeasureAndBarlineInPartClone (
      inputLineNumber,
      elt->getMeasureElementPositionInMeasure ());
     // */
}

void msr2lpsrTranslator::visitStart (S_msrCoda& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrCoda" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingChord || fOnGoingNonGraceNote) {
    if (fOnGoingChord) {
      fCurrentChordClone->
        appendCodaToChord (elt);
    }
    if (fOnGoingNonGraceNote) {
      fCurrentNonGraceNoteClone->
        appendCodaToNote (elt);
    }
  }
  else {
    stringstream s;

    s <<
      "coda is out of context, cannot be handled: '" << elt->asShortString () <<
      "'";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrEyeGlasses& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting eyeGlasses" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendEyeGlassesToNote (elt);
}

void msr2lpsrTranslator::visitStart (S_msrScordatura& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting scordatura" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendScordaturaToNote (elt);
}

void msr2lpsrTranslator::visitStart (S_msrPedal& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting pedal" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendPedalToNote (elt);
}

void msr2lpsrTranslator::visitStart (S_msrDamp& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting damp" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendDampToNote (elt);

  fResultingLpsrScore->
    // this score needs the 'custom short barline' Scheme function
    setDampMarkupIsNeeded ();
}

void msr2lpsrTranslator::visitStart (S_msrDampAll& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting dampAll" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendDampAllToNote (elt);

  fResultingLpsrScore->
    // this score needs the 'custom short barline' Scheme function
    setDampAllMarkupIsNeeded ();
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrBarCheck& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBarCheck" <<
      ", nextBarNumber: " <<
      elt->getNextBarPuristNumber () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendBarCheckToVoice (elt);
}

void msr2lpsrTranslator::visitEnd (S_msrBarCheck& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrBarCheck" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrBarNumberCheck& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBarNumberCheck" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendBarNumberCheckToVoice (elt);
}

void msr2lpsrTranslator::visitEnd (S_msrBarNumberCheck& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrBarNumberCheck" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrLineBreak& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrLineBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendLineBreakToVoice (elt);
}

void msr2lpsrTranslator::visitEnd (S_msrLineBreak& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrLineBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrPageBreak& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPageBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendPageBreakToVoice (elt);
}

void msr2lpsrTranslator::visitEnd (S_msrPageBreak& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPageBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrRepeat" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling repeat start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    handleRepeatStartInVoiceClone (
      inputLineNumber,
      elt);
}

void msr2lpsrTranslator::visitEnd (S_msrRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrRepeat" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling repeat end in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
//      "\" in part \"" <<
//      fCurrentPartClone->getPartCombinedName () <<
      "\"" <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    handleRepeatEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrRepeatCommonPart& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrRepeatCommonPart" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    handleRepeatCommonPartStartInVoiceClone (
      inputLineNumber);
}

void msr2lpsrTranslator::visitEnd (S_msrRepeatCommonPart& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrRepeatCommonPart" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    handleRepeatCommonPartEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrRepeatEnding& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrRepeatEnding" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // handle the repeat ending start in the voice clone
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat ending start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    handleRepeatEndingStartInVoiceClone (
      inputLineNumber,
      elt->getRepeatEndingKind (),
      elt->getRepeatEndingNumber ());
}

void msr2lpsrTranslator::visitEnd (S_msrRepeatEnding& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrRepeatEnding" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // handle the repeat ending end in the voice clone
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat ending end in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    handleRepeatEndingEndInVoiceClone (
      inputLineNumber,
      elt->getRepeatEndingNumber (),
      elt->getRepeatEndingKind ());
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrRestMeasures& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrRestMeasures" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    gLogStream <<
      "Handling multiple rest start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    handleRestMeasuresStartInVoiceClone (
      inputLineNumber,
      elt);
}

void msr2lpsrTranslator::visitEnd (S_msrRestMeasures& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrRestMeasures" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter--;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    gLogStream <<
      "Handling multiple rest start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    handleRestMeasuresEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrRestMeasuresContents& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrRestMeasuresContents" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    fCurrentVoiceClone->
      displayVoice (
        inputLineNumber,
        "Upon visitStart (S_msrRestMeasuresContents&)");
  }
#endif

  fCurrentVoiceClone->
    handleRestMeasuresContentsStartInVoiceClone (
      inputLineNumber);
}

void msr2lpsrTranslator::visitEnd (S_msrRestMeasuresContents& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrRestMeasuresContents" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter--;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    fCurrentVoiceClone->
      displayVoice (
        inputLineNumber,
        "Upon visitEnd (S_msrRestMeasuresContents&) 1");
  }
#endif

  fCurrentVoiceClone->
    handleRestMeasuresContentsEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrMeasuresRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMeasuresRepeat" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Handling measures repeat start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    handleMeasuresRepeatStartInVoiceClone (
      inputLineNumber,
      elt);
}

void msr2lpsrTranslator::visitEnd (S_msrMeasuresRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMeasuresRepeat" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter--;

/* JMI
  // set last segment as the measures repeat pattern segment
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Setting current last segment as measures repeat pattern segment in voice \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
*/

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Handling measures repeat end in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    handleMeasuresRepeatEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrMeasuresRepeatPattern& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMeasuresRepeatPattern" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    fCurrentVoiceClone->
      displayVoice (
        inputLineNumber,
        "Upon visitStart (S_msrMeasuresRepeatPattern&)");
  }
#endif

  fCurrentVoiceClone->
    handleMeasuresRepeatPatternStartInVoiceClone (
      inputLineNumber);
}

void msr2lpsrTranslator::visitEnd (S_msrMeasuresRepeatPattern& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMeasuresRepeatPattern" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter--;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    fCurrentVoiceClone->
      displayVoice (
        inputLineNumber,
        "Upon visitEnd (S_msrMeasuresRepeatPattern&) 1");
  }
#endif

  fCurrentVoiceClone->
    handleMeasuresRepeatPatternEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrMeasuresRepeatReplicas& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMeasuresRepeatReplicas" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    fCurrentVoiceClone->
      displayVoice (
        inputLineNumber,
        "Upon visitStart (S_msrMeasuresRepeatReplicas&)");
  }
#endif

  fCurrentVoiceClone->
    handleMeasuresRepeatReplicasStartInVoiceClone (
      inputLineNumber);
}

void msr2lpsrTranslator::visitEnd (S_msrMeasuresRepeatReplicas& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_msrMeasuresRepeatReplicas" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter--;


#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    fCurrentVoiceClone->
      displayVoice (
        inputLineNumber,
        "Upon visitEnd (S_msrMeasuresRepeatReplicas&) 1");
  }
#endif

  fCurrentVoiceClone->
    handleMeasuresRepeatReplicasEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrBarline& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBarline" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceBarlines ()) {
    gLogStream <<
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
      fResultingLpsrScore->
        // this score needs the 'custom short barline' Scheme function
        setCustomShortBarlineSchemeFunctionIsNeeded ();
      break;
      /* JMI
    case msrBarline::kBarlineStyleNone:
      break;
      */
  } // switch

  // append the barline to the current voice clone
  fCurrentVoiceClone->
    appendBarlineToVoice (elt);
}

void msr2lpsrTranslator::visitEnd (S_msrBarline& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrBarline" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrMidiTempo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMidiTempo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void msr2lpsrTranslator::visitEnd (S_msrMidiTempo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMidiTempo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}


} // namespace

//________________________________________________________________________
/* JMI
void msr2lpsrTranslator::prependSkipGraceNotesGroupToPartOtherVoices (
  S_msrPart            partClone,
  S_msrVoice           voiceClone,
  S_msrGraceNotesGroup skipGraceNotesGroup)
{
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
      gLogStream <<
        "--> prepending a skip graceNotesGroup clone " <<
        skipGraceNotesGroup->asShortString () <<
        " to voices other than \"" <<
        voiceClone->getVoiceName () << "\"" <<
        " in part " <<
        partClone->getPartCombinedName () <<
        ", line " << skipGraceNotesGroup->getInputLineNumber () <<
        endl;
    }
#endif

  map<int, S_msrStaff>
    partStavesMap =
      partClone->
        getPartStavesMap ();

  for (
    map<int, S_msrStaff>::const_iterator i=partStavesMap.begin ();
    i!=partStavesMap.end ();
    i++
  ) {
    list<S_msrVoice>
      staffAllVoicesVector =
        (*i).second->
          getStaffAllVoicesVector ();

    for (
      list<S_msrVoice>::const_iterator j=staffAllVoicesVector.begin ();
      j!=staffAllVoicesVector.end ();
      j++
    ) {
      S_msrVoice voice = (*j);

      if (voice != voiceClone) {
        // prepend skip grace notes to voice JMI
        / *
        voice->
          prependGraceNotesGroupToVoice (
            skipGraceNotesGroup);
            * /
      }
    } // for

  } // for
}
*/

