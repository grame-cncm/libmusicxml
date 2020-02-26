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

#include "msr2MxmltreeTranslator.h"

#include "elements.h"
#include "factory.h"
#include "xml.h"
#include "xmlfile.h"

#include "generalOah.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "mxmlTreeOah.h"
#include "msrOah.h"
#include "lilypondOah.h"

#include "mxmlTree.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
msr2MxmltreeTranslator::msr2MxmltreeTranslator (
  indentedOstream& ios,
  S_msrScore       mScore)
    : fLogOutputStream (ios)
{
  // the MSR score we're visiting
  fVisitedMsrScore = mScore;

  // create an empty clone of fVisitedMsrScore for use by the Mxmltree score
  // not sharing the visitiged MSR score allows cleaner data handling
  // and optimisations of the Mxmltree data
  fCurrentMsrScoreClone =
    fVisitedMsrScore->
      createScoreNewbornClone ();

  // create the current score part-wise element
  fMxmltree = createScorePartWiseElement ();

  // create the part list element
  fPartListElement = createElement (k_part_list, "");

  // append it to the mxmlTree
  fMxmltree->push (fPartListElement);

/*

//------------------------------------------------------------------------
static Sxmlelement makeAttributes() {
	Sxmlelement attributes = factory::instance().create(k_attributes);
	attributes->push (newElementI(k_divisions, kDivision));

	Sxmlelement time = factory::instance().create(k_time);
	time->push (newElement(k_beats, "4"));
	time->push (newElement(k_beat_type, "4"));
	attributes->push (time);

	Sxmlelement clef = factory::instance().create(k_clef);
	clef->push (newElement(k_sign, "G"));
	clef->push (newElement(k_line, "2"));
	attributes->push (clef);

	return attributes;
}



static Sxmlelement makeIdentification() {
	Sxmlelement id = factory::instance().create(k_identification);
	Sxmlelement encoding = factory::instance().create(k_encoding);

	Sxmlelement creator = newElement(k_creator, "Georg Chance");
	creator->add(newAttribute("type", "Composer"));
	id->push (creator);

	encoding->push (newElement(k_software, "MusicXML Library v2"));
	id->push (encoding);
	return id;
}

static Sxmlelement makePartList() {
	Sxmlelement partlist = factory::instance().create(k_part_list);
	Sxmlelement scorepart = factory::instance().create(k_score_part);
	scorepart->add (newAttribute("id", kPartID));
	scorepart->push (newElement(k_part_name, "Part name"));
	Sxmlelement scoreinstr = factory::instance().create(k_score_instrument);
	scoreinstr->add (newAttribute("id", "I1"));
	scoreinstr->push (newElement(k_instrument_name, "Any instr."));
	scorepart->push (scoreinstr);
	partlist->push(scorepart);
	return partlist;
}

Sxmlelement makePart(int count) {
	Sxmlelement part = factory::instance().create(k_part);
	part->add (newAttribute("id", kPartID));
	for (int i=1; i<=count; i++)			// and 'count' times
		part->push (makemeasure(i));			// adds a new measure to the part
	return part;
}


	score->push (newElement(k_movement_title, "Random Music"));
	score->push (makeIdentification());
	score->push (makePartList());
	score->push(makePart(measuresCount));			// adds a part to the score
*/

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

  fOnGoingHarmony = false;

  // repeats

  // notes
  fOnGoingNonGraceNote = false;

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
};

msr2MxmltreeTranslator::~msr2MxmltreeTranslator ()
{}

//________________________________________________________________________
void msr2MxmltreeTranslator::buildMxmltreeFromMsrScore ()
{
  if (fVisitedMsrScore) {
    // create a msrScore browser
    msrBrowser<msrScore> browser (this);

    // browse the visited score with the browser
    browser.browse (*fVisitedMsrScore);
  }
}

//________________________________________________________________________
void msr2MxmltreeTranslator::displayCurrentOnGoingValues ()
{
  fLogOutputStream <<
    "Ongoing value:" <<
    endl;

  gIndenter++;

  const int fieldWidth = 25;

  fLogOutputStream <<
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
void msr2MxmltreeTranslator::visitStart (S_msrScore& elt)
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
}

void msr2MxmltreeTranslator::visitEnd (S_msrScore& elt)
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
      movementTitle.size () > 0
    ) {
      // use the movement title as the work title
      fCurrentIdentification->
        setWorkTitle (
          inputLineNumber, movementTitle);

      fCurrentLpsrScoreHeader->
        setWorkTitle (
          inputLineNumber,
          movementTitle,
          kFontStyleNone,
          kFontWeightNone);

      // forget the movement title
      fCurrentIdentification->
        setMovementTitle (
          inputLineNumber, "");

      fCurrentLpsrScoreHeader->
        setMovementTitle (
          inputLineNumber,
          "",
          kFontStyleNone,
          kFontWeightNone);
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

    fCurrentLpsrScoreHeader->
      setWorkTitle (
        inputLineNumber,
        movementTitle,
        kFontStyleNone,
        kFontWeightNone);

    // forget the movement title
    fCurrentIdentification->
      setMovementTitle (
        inputLineNumber, "");

    fCurrentLpsrScoreHeader->
      setMovementTitle (
        inputLineNumber,
        "",
        kFontStyleNone,
        kFontWeightNone);
  }

  if (fWorkNumberKnown && fMovementNumberKnown) {
    string
      workNumber =
        fCurrentIdentification->
          getWorkNumber ()->
            getVariableValue (),
      movementNumber =
        fCurrentIdentification->
          getMovementNumber ()->
            getVariableValue ();

    if (
      workNumber.size () == 0
        &&
      movementNumber.size () > 0
    ) {
      // use the movement number as the work number
      fCurrentIdentification->
        setWorkNumber (
          inputLineNumber, movementNumber);

      fCurrentLpsrScoreHeader->
        setWorkNumber (
          inputLineNumber,
          movementNumber,
          kFontStyleNone,
          kFontWeightNone);

      // forget the movement number
      fCurrentIdentification->
        setMovementNumber (
          inputLineNumber, "");

      fCurrentLpsrScoreHeader->
        setMovementNumber (
          inputLineNumber,
          "",
          kFontStyleNone,
          kFontWeightNone);
    }
  }

  else if (! fWorkNumberKnown && fMovementNumberKnown) {
    string
      movementNumber =
        fCurrentIdentification->
          getMovementNumber ()->
            getVariableValue ();

    // use the movement number as the work number
    fCurrentIdentification->
      setWorkNumber (
        inputLineNumber, movementNumber);

    fCurrentLpsrScoreHeader->
      setWorkNumber (
        inputLineNumber,
        movementNumber,
        kFontStyleNone,
        kFontWeightNone);

    // forget the movement number
    fCurrentIdentification->
      setMovementNumber (
        inputLineNumber, "");

    fCurrentLpsrScoreHeader->
      setMovementNumber (
        inputLineNumber,
        "",
        kFontStyleNone,
        kFontWeightNone);
  }
  */
}

/*
//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrIdentification& elt)
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
    fMxmltree->
      getMsrScore ()->
        getIdentification ();

  fOnGoingIdentification = true;
}

void msr2MxmltreeTranslator::visitEnd (S_msrIdentification& elt)
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

//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrScaling& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrScaling" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter++;

  // create a scaling clone
  S_msrScaling
    geometryClone =
      elt->createMsrScalingNewbornClone ();

  // register it in current MSR score clone
  fCurrentMsrScoreClone-> // JMI BLARK ???
    setScaling (
      geometryClone);

  // get LPSR score paper
  S_lpsrPaper
    paper =
      fMxmltree->getScorePaper ();

  // sanity check
  msrAssert (
    paper != nullptr,
    "paper is null");

  // populate LPSR score global staff size
  float globalStaffSize = 0.0;

  if (
    gLpsrOah->fGlobalStaffSize
      !=
    gLpsrOah->fStaffGlobalSizeDefaultValue
  ) {
    // the LPSR option value takes precedence
    globalStaffSize =
      gLpsrOah->fGlobalStaffSize;
  }

  else {
    // fetch LPSR score global staff size
    globalStaffSize =
      elt->fetchGlobalStaffSize ();
  }

  // set score global staff size Scheme variable
  fMxmltree->
    setScoreGlobalStaffSizeSchemeVariable (
      globalStaffSize);

  // get LPSR score block layout
  S_lpsrLayout
    scoreBlockLayout =
      fMxmltree->getScoreLayout ();

  // create the score block layout staff-size Scheme assoc
  stringstream s;
  s << globalStaffSize;
  S_lpsrSchemeVariable
    assoc =
      lpsrSchemeVariable::create (
        K_NO_INPUT_LINE_NUMBER, // JMI
        lpsrSchemeVariable::kCommentedYes,
        "layout-set-staff-size",
        s.str (),
        "Uncomment and adapt next line as needed (default is 20)",
        lpsrSchemeVariable::kEndlOnce);

  // populate score block layout
  scoreBlockLayout->
    addSchemeVariable (assoc);
}

void msr2MxmltreeTranslator::visitEnd (S_msrScaling& elt)
{
  gIndenter--;

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrScaling" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrCredit& elt)
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

  // set elt as credit of the MSR score part of the LPSR score
  fMxmltree->
    getMsrScore ()->
      appendCreditToScore (fCurrentCredit);
}

void msr2MxmltreeTranslator::visitEnd (S_msrCredit& elt)
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

void msr2MxmltreeTranslator::visitStart (S_msrCreditWords& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrCreditWords" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

}

void msr2MxmltreeTranslator::visitEnd (S_msrCreditWords& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrPartGroup& elt)
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

  // create a partGroup clone
  // current partGroup clone, i.e. the top of the stack,
  // is the upLink of the new one if it exists
  S_msrPartGroup
    partGroupClone =
      elt->createPartGroupNewbornClone (
        fPartGroupsStack.size ()
          ? fPartGroupsStack.top ()
          : nullptr,
        fMxmltree->getMsrScore ());

  // push it onto this visitors's stack,
  // making it the current partGroup block
#ifdef TRACE_OAH
  if (gTraceOah->fTracePartGroups) {
    fLogOutputStream <<
      "Pushing part group clone " <<
      partGroupClone->getPartGroupCombinedName () <<
      " onto stack" <<
      endl;
  }
#endif

  fPartGroupsStack.push (
    partGroupClone);

/ * JMI
  // add it to the MSR score clone
  fCurrentMsrScoreClone->
    addPartGroupToScore (fCurrentPartGroupClone);
* /
}

void msr2MxmltreeTranslator::visitEnd (S_msrPartGroup& elt)
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

  S_msrPartGroup
    currentPartGroup =
      fPartGroupsStack.top ();

  if (fPartGroupsStack.size () == 1) {
    // add the current partgroup clone to the MSR score clone
    // if it is the top-level one, i.e it's alone in the stack

#ifdef TRACE_OAH
    if (gTraceOah->fTracePartGroups) {
      fLogOutputStream <<
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
#ifdef TRACE_OAH
    if (gTraceOah->fTracePartGroups) {
      fLogOutputStream <<
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
}
*/

//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrPart& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  string
    partID =
      elt->getPartID (),
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
  if (gTraceOah->fTraceParts) {
    fLogOutputStream <<
      endl <<
      "<!--=== part \"" << partCombinedName << "\"" <<
      ", line " << inputLineNumber << " ===-->" <<
      endl;
  }
#endif

  gIndenter++;

  // create a part element
  fCurrentPartElement = createElement (k_part, "");

  // set its "id" attribute
	fCurrentPartElement->add (createAttribute ("id", partID));

  // append it to the mxmltree
  fMxmltree->push (fCurrentPartElement);

  // create a score part element
  Sxmlelement scorePartElement = createElement (k_score_part, "");

  // set it's "id" attribute
  scorePartElement->add (createAttribute ("id", partID));

  // append it to the part list wkwnwbr
  fPartListElement->push (scorePartElement);

/*
  fCurrentPartClone =
    elt->createPartNewbornClone (
      fPartGroupsStack.top ());

  // add it to the partGroup clone
#ifdef TRACE_OAH
  if (gTraceOah->fTraceParts) {
    fLogOutputStream <<
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
    */
}

void msr2MxmltreeTranslator::visitEnd (S_msrPart& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  gIndenter--;

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrPart " <<
      elt->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // forget about the current part element
  fCurrentPartElement = nullptr;
}

/*
//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrStaffLinesNumber& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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

//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrStaffTuning& elt)
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
  fCurrentStaffTuningClone =
    elt->
      createStaffTuningNewbornClone ();
}

//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrStaffDetails& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrStaffDetails" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentStaffTuningClone = nullptr;
}

void msr2MxmltreeTranslator::visitEnd (S_msrStaffDetails& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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
void msr2MxmltreeTranslator::visitStart (S_msrStaff& elt)
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

void msr2MxmltreeTranslator::visitEnd (S_msrStaff& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrVoice& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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
      fMxmltree ->
        appendVoiceToScoreElementsList (fCurrentVoiceClone);

      // append a use of the voice to the current staff block
      fCurrentStaffBlock->
        appendVoiceUseToStaffBlock (
          fCurrentVoiceClone);
      break;

    case msrVoice::kVoiceHarmony:
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

        if (
          elt->getMusicHasBeenInsertedInVoice () // superfluous test ??? JMI
          ) {
          // append the voice clone to the LPSR score elements list
          fMxmltree ->
            appendVoiceToScoreElementsList (
              fCurrentVoiceClone);

          // create a ChordNames context
          string voiceName =
            elt->getVoiceName ();

          string partCombinedName =
            elt->fetchVoicePartUpLink ()->
              getPartCombinedName ();

#ifdef TRACE_OAH
          if (gTraceOah->fTraceHarmonies) {
            fLogOutputStream <<
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
#ifdef TRACE_OAH
          if (gTraceOah->fTraceHarmonies) {
            fLogOutputStream <<
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
          fMxmltree ->
            appendVoiceToScoreElementsList (
              fCurrentVoiceClone);

          // create a FiguredBass context
          string voiceName =
            elt->getVoiceName ();

          string partCombinedName =
            elt->fetchVoicePartUpLink ()->
              getPartCombinedName ();

#ifdef TRACE_OAH
          if (gTraceOah->fTraceHarmonies) {
            fLogOutputStream <<
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
#ifdef TRACE_OAH
          if (gTraceOah->fTraceHarmonies) {
            fLogOutputStream <<
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

void msr2MxmltreeTranslator::visitEnd (S_msrVoice& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrVoiceStaffChange& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrSegment& elt)
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

  // create a clone of the segment
  fCurrentSegmentClone =
    elt->createSegmentNewbornClone (
      fCurrentVoiceClone);

  // set it as the new voice last segment
  fCurrentVoiceClone->
    setVoiceLastSegmentInVoiceClone (
      fCurrentSegmentClone);
}

void msr2MxmltreeTranslator::visitEnd (S_msrSegment& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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
void msr2MxmltreeTranslator::visitStart (S_msrHarmony& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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
  / * JMI
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
* /

    // append the harmony to the current voice clone
    fCurrentVoiceClone->
      appendHarmonyToVoiceClone (
        fCurrentHarmonyClone);
  }

  else {
    stringstream s;

    s <<
      "harmony '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  fOnGoingHarmony = true;
}

void msr2MxmltreeTranslator::visitStart (S_msrHarmonyDegree& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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

void msr2MxmltreeTranslator::visitEnd (S_msrHarmony& elt)
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

  fCurrentHarmonyClone = nullptr;
  fOnGoingHarmony = false;
}

//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrFrame& elt)
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

  if (fOnGoingHarmony) {
    // register the frame in the current non-grace note clone
    fCurrentHarmonyClone->
      setHarmonyFrame (elt);
  }

  else {
    stringstream s;

    s <<
      "frame '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrFiguredBass& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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

  / * JMI
  else if (fOnGoingChord) {
    // register the figured bass in the current chord clone
    fCurrentChordClone->
      setChordFiguredBass (fCurrentFiguredBassClone); // JMI
  }
  * /

  else if (fOnGoingFiguredBassVoice) { // JMI
    / *
    // register the figured bass in the part clone figured bass
    fCurrentPartClone->
      appendFiguredBassToPartClone (
        fCurrentVoiceClone,
        fCurrentFiguredBassClone);
        * /
    // append the figured bass to the current voice clone
    fCurrentVoiceClone->
      appendFiguredBassToVoiceClone (
        fCurrentFiguredBassClone);
  }

  else {
    stringstream s;

    s <<
      "figured bass '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2MxmltreeTranslator::visitStart (S_msrFigure& elt)
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
  fCurrentFiguredBassClone->
    appendFigureToFiguredBass (
      elt);
}

void msr2MxmltreeTranslator::visitEnd (S_msrFiguredBass& elt)
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

  fCurrentFiguredBassClone = nullptr;
}
*/
//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrMeasure& elt)
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

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrMeasure '" <<
      measureNumber <<
      "', measurePuristNumber = '" <<
      measurePuristNumber <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

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

  // create a measure element
  fCurrentMeasureElement = createElement (k_measure, "");

  // set its "number" attribute
	fCurrentMeasureElement->add (createAttribute ("number", measureNumber));

  // append it to the current part element
  fCurrentPartElement->push (fCurrentMeasureElement);
}

void msr2MxmltreeTranslator::visitEnd (S_msrMeasure& elt)
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

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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

  // forget about the current measure element
  fCurrentMeasureElement = nullptr;
}
/*
//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrStanza& elt)
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

    // append the stanza clone to the LPSR score elements list
    fMxmltree ->
      appendStanzaToScoreElementsList (
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

void msr2MxmltreeTranslator::visitEnd (S_msrStanza& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrSyllable& elt)
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

  else if (fOnGoingNonGraceNote) { // JMI
    // visiting a syllable as attached to the current non-grace note
    fCurrentSyllableClone->
      appendSyllableToNoteAndSetItsNoteUpLink (
        fCurrentNonGraceNoteClone);

    if (gLpsrOah->fAddWordsFromTheLyrics) {
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
        if (gTraceOah->fTraceLyrics) {
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
              kHorizontalAlignmentNone,      // default value
              kVerticalAlignmentNone,        // default value
              kFontStyleNone,                // default value
              msrFontSize::create (
                msrFontSize::kFontSizeNone), // default value
              kFontWeightNone,               // default value
              kXMLLangIt,                    // default value
              elt->getSyllableNoteUpLink ()->getNoteStaffNumber ());

        // append it to the current non-grace note
#ifdef TRACE_OAH
        if (gTraceOah->fTraceWords) {
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
  else {
    stringstream s;

    s <<
      "syllable '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // a syllable ends the sysllable extend range if any
  if (fOnGoingSyllableExtend) {
    / * JMI ???
    // create melisma end command
    S_lpsrMelismaCommand
      melismaCommand =
        lpsrMelismaCommand::create (
          inputLineNumber,
          lpsrMelismaCommand::kMelismaEnd);

    // append it to current voice clone
    fCurrentVoiceClone->
      appendOtherElementToVoice (melismaCommand);
* /

    fOnGoingSyllableExtend = false;
  }
}

void msr2MxmltreeTranslator::visitEnd (S_msrSyllable& elt)
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

//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrClef& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrClef" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendClefToVoice (elt);
}

void msr2MxmltreeTranslator::visitEnd (S_msrClef& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrKey& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrKey" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendKeyToVoice (elt);
}

void msr2MxmltreeTranslator::visitEnd (S_msrKey& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrTime& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrTime" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append time to voice clone
  fCurrentVoiceClone->
    appendTimeToVoice (elt);
}

void msr2MxmltreeTranslator::visitEnd (S_msrTime& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrTranspose& elt)
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

void msr2MxmltreeTranslator::visitEnd (S_msrTranspose& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrPartNameDisplay& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrPartAbbreviationDisplay& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrTempo& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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
      fMxmltree->
        // this score needs the 'tempo relationship' Scheme function
        setTempoRelationshipSchemeFunctionIsNeeded ();
      break;
  } // switch

  if (gLpsrOah->fConvertTemposToRehearsalMarks) {
    // create a rehearsal mark containing elt's words

    S_msrRehearsal
      rehearsal =
        msrRehearsal::create (
          elt->getInputLineNumber (),
          msrRehearsal::kNone,
          elt->tempoWordsListAsString (" "), //JMI ???
          elt->getTempoPlacementKind ());

#ifdef TRACE_OAH
    if (gTraceOah->fTraceTempos) {
      fLogOutputStream <<
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

void msr2MxmltreeTranslator::visitEnd (S_msrTempo& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrRehearsal& elt)
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

void msr2MxmltreeTranslator::visitEnd (S_msrRehearsal& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrArticulation& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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
      "articulation '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2MxmltreeTranslator::visitEnd (S_msrArticulation& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrFermata& elt)
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
      "fermata '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrArpeggiato& elt)
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
      "arpeggiato '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrNonArpeggiato& elt)
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
      "nonArpeggiato '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrTechnical& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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
      "technical '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // doest the score need the 'tongue' function?
  switch (elt->getTechnicalKind ()) {
    case msrTechnical::kArrow:
      break;
    case msrTechnical::kDoubleTongue:
      fMxmltree->
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
      fMxmltree->
        // this score needs the 'tongue' Scheme function
        setTongueSchemeFunctionIsNeeded ();
      break;
    case msrTechnical::kUpBow:
      break;
  } // switch
}

void msr2MxmltreeTranslator::visitEnd (S_msrTechnical& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrTechnicalWithInteger& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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
      "technicalWithInteger '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2MxmltreeTranslator::visitEnd (S_msrTechnicalWithInteger& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrTechnicalWithFloat& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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
      "technicalWithFloat '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2MxmltreeTranslator::visitEnd (S_msrTechnicalWithFloat& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrTechnicalWithString& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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
      "technicalWithString '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  switch (elt->getTechnicalWithStringKind ()) {
    case msrTechnicalWithString::kHammerOn:
    case msrTechnicalWithString::kPullOff:
      // this score needs the 'after' Scheme function
      fMxmltree->
        setAfterSchemeFunctionIsNeeded ();
      break;
    default:
      ;
  } // switch
}

void msr2MxmltreeTranslator::visitEnd (S_msrTechnicalWithString& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrOrnament& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrOrnament" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
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
      "ornament '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2MxmltreeTranslator::visitEnd (S_msrOrnament& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrSpanner& elt)
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
      "spanner '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2MxmltreeTranslator::visitEnd (S_msrSpanner& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrGlissando& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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
      "glissando '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  if (elt->getGlissandoTextValue ().size ()) {
    fMxmltree->
      // this score needs the 'glissandoWithText' Scheme functions
      addGlissandoWithTextSchemeFunctionsToScore ();
  }
}

void msr2MxmltreeTranslator::visitEnd (S_msrGlissando& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrSlide& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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
      "slide '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2MxmltreeTranslator::visitEnd (S_msrSlide& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrSingleTremolo& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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
      "singleTremolo '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2MxmltreeTranslator::visitEnd (S_msrSingleTremolo& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrDoubleTremolo& elt)
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

void msr2MxmltreeTranslator::visitEnd (S_msrDoubleTremolo& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrDynamics& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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
      fMxmltree->
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
      "dynamics '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2MxmltreeTranslator::visitEnd (S_msrDynamics& elt)
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

//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrOtherDynamics& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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
      "otherDynamics '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  fMxmltree->
    // this score needs the 'otherDynamic' Scheme function
    setOtherDynamicSchemeFunctionIsNeeded ();
}

void msr2MxmltreeTranslator::visitEnd (S_msrOtherDynamics& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrWords& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrWords" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote || fOnGoingChord) {
    bool wordsHasBeenHandled = false;

    if (gLpsrOah->fConvertWordsToTempo) {
      // create a tempo containing elt
      S_msrTempo
        tempo =
          msrTempo::create (
            inputLineNumber,
            elt);

#ifdef TRACE_OAH
      if (gTraceOah->fTraceWords) {
        fLogOutputStream <<
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

    else if (gLpsrOah->fConvertWordsToRehearsalMarks) {
      // create a rehearsal mark containing elt's words contents
      S_msrRehearsal
        rehearsal =
          msrRehearsal::create (
            inputLineNumber,
            msrRehearsal::kNone,
            elt->getWordsContents (),
            elt->getWordsPlacementKind ()); // above ??? JMI

#ifdef TRACE_OAH
      if (gTraceOah->fTraceWords) {
        fLogOutputStream <<
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
    / * JMI
      string wordsContents = elt->getWordsContents ();

      // is this words contents in the string to dal segno kind map?
      map<string, msrDalSegno::msrDalSegnoKind>::iterator
        it =
          gLpsrOah->fConvertWordsToDalSegno.find (wordsContents);

      if (it != gLpsrOah->fConvertWordsToDalSegno.end ()) {
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

#ifdef TRACE_OAH
        if (gTraceOah->fTraceWords) {
          fLogOutputStream <<
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
      * /
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
      "words '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2MxmltreeTranslator::visitEnd (S_msrWords& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrSlur& elt)
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
    i.e. a stop and a start in sequence:
    these should be ignored
  * /

  if (fOnGoingNonGraceNote) {
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

  else {
    displayCurrentOnGoingValues ();

    stringstream s;

    s <<
      "slur '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2MxmltreeTranslator::visitEnd (S_msrSlur& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrLigature& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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
      "ligature '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2MxmltreeTranslator::visitEnd (S_msrLigature& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrSlash& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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
      "slash '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrWedge& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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
      "wedge '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2MxmltreeTranslator::visitEnd (S_msrWedge& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrGraceNotesGroup& elt)
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
    if (gTraceOah->fTraceGraceNotes) {
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
    // if (fOnGoingNonGraceNote) { JMI
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
  if (gTraceOah->fTraceGraceNotes) {
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
  if (gTraceOah->fTraceGraceNotes) {
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceGraceNotes) {
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
      // issue #34 in LilyPond should be worked around by creating
      // skip grace notes in the other voices of the part

      // create the skip grace notes group
#ifdef TRACE_OAH
        if (gTraceOah->fTraceGraceNotes) {
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

void msr2MxmltreeTranslator::visitEnd (S_msrGraceNotesGroup& elt)
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
    if (gTraceOah->fTraceGraceNotes) {
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
*/
//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrNote& elt)
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

  // create a note element
  Sxmlelement noteElement = createElement (k_note, "");

  // append it to the current measure element
  fCurrentMeasureElement->push (noteElement);
}

void msr2MxmltreeTranslator::visitEnd (S_msrNote& elt)
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
/*
//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrOctaveShift& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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
      "octaveShift '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2MxmltreeTranslator::visitEnd (S_msrOctaveShift& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrAccordionRegistration& elt)
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
  fMxmltree->
    setScmAndAccregSchemeModulesAreNeeded ();
}

//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrHarpPedalsTuning& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrStem& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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
    fCurrentChordClone->
      appendStemToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "stem '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2MxmltreeTranslator::visitEnd (S_msrStem& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrBeam& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrBeam" <<
      ", line " << elt->getInputLineNumber () <<
// JMI      ", fOnGoingNonGraceNote = " << booleanAsString (fOnGoingNonGraceNote) <<
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
  else if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendBeamToNote (elt);
  }

  if (fOnGoingChord) { // else ??? JMI
    fCurrentChordClone->
      appendBeamToChord (elt);
  }
}

void msr2MxmltreeTranslator::visitEnd (S_msrBeam& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrChord& elt)
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

void msr2MxmltreeTranslator::visitEnd (S_msrChord& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrTuplet& elt)
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
      break;
    case msrTuplet::kTupletLineShapeCurved:
      fMxmltree->
        // this score needs the 'tuplets curved brackets' Scheme function
        setTupletsCurvedBracketsSchemeFunctionIsNeeded ();
      break;
  } // switch
}

void msr2MxmltreeTranslator::visitEnd (S_msrTuplet& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrTie& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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
      "tie '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2MxmltreeTranslator::visitEnd (S_msrTie& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrSegno& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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
      "segno '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2MxmltreeTranslator::visitStart (S_msrDalSegno& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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
      "dal segno '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2MxmltreeTranslator::visitStart (S_msrCoda& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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
      "coda '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gOahOah->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrEyeGlasses& elt)
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

void msr2MxmltreeTranslator::visitStart (S_msrScordatura& elt)
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

void msr2MxmltreeTranslator::visitStart (S_msrPedal& elt)
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

void msr2MxmltreeTranslator::visitStart (S_msrDamp& elt)
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

  fMxmltree->
    // this score needs the 'custom short barline' Scheme function
    setDampMarkupIsNeeded ();
}

void msr2MxmltreeTranslator::visitStart (S_msrDampAll& elt)
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

  fMxmltree->
    // this score needs the 'custom short barline' Scheme function
    setDampAllMarkupIsNeeded ();
}

//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrBarCheck& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
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

void msr2MxmltreeTranslator::visitEnd (S_msrBarCheck& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrBarNumberCheck& elt)
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

void msr2MxmltreeTranslator::visitEnd (S_msrBarNumberCheck& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrLineBreak& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrLineBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendLineBreakToVoice (elt);
}

void msr2MxmltreeTranslator::visitEnd (S_msrLineBreak& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrPageBreak& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrPageBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendPageBreakToVoice (elt);
}

void msr2MxmltreeTranslator::visitEnd (S_msrPageBreak& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrRepeat& elt)
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    fLogOutputStream <<
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

void msr2MxmltreeTranslator::visitEnd (S_msrRepeat& elt)
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    fLogOutputStream <<
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
void msr2MxmltreeTranslator::visitStart (S_msrRepeatCommonPart& elt)
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

  fCurrentVoiceClone->
    handleRepeatCommonPartStartInVoiceClone (
      inputLineNumber);
}

void msr2MxmltreeTranslator::visitEnd (S_msrRepeatCommonPart& elt)
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

  fCurrentVoiceClone->
    handleRepeatCommonPartEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrRepeatEnding& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrRepeatEnding" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // handle the repeat ending start in the voice clone
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    fLogOutputStream <<
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

void msr2MxmltreeTranslator::visitEnd (S_msrRepeatEnding& elt)
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

  // handle the repeat ending end in the voice clone
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    fLogOutputStream <<
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
void msr2MxmltreeTranslator::visitStart (S_msrRestMeasures& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrRestMeasures" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    fLogOutputStream <<
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

void msr2MxmltreeTranslator::visitEnd (S_msrRestMeasures& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrRestMeasures" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter--;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    fLogOutputStream <<
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
void msr2MxmltreeTranslator::visitStart (S_msrRestMeasuresContents& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrRestMeasuresContents" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
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

void msr2MxmltreeTranslator::visitEnd (S_msrRestMeasuresContents& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrRestMeasuresContents" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter--;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
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
void msr2MxmltreeTranslator::visitStart (S_msrMeasuresRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrMeasuresRepeat" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    fLogOutputStream <<
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

void msr2MxmltreeTranslator::visitEnd (S_msrMeasuresRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrMeasuresRepeat" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter--;

/ * JMI
  // set last segment as the measures repeat pattern segment
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    fLogOutputStream <<
      "Setting current last segment as measures repeat pattern segment in voice \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
* /

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    fLogOutputStream <<
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
void msr2MxmltreeTranslator::visitStart (S_msrMeasuresRepeatPattern& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrMeasuresRepeatPattern" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
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

void msr2MxmltreeTranslator::visitEnd (S_msrMeasuresRepeatPattern& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrMeasuresRepeatPattern" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter--;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
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
void msr2MxmltreeTranslator::visitStart (S_msrMeasuresRepeatReplicas& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrMeasuresRepeatReplicas" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
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

void msr2MxmltreeTranslator::visitEnd (S_msrMeasuresRepeatReplicas& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting S_msrMeasuresRepeatReplicas" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter--;


#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
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
void msr2MxmltreeTranslator::visitStart (S_msrBarline& elt)
{
#ifdef TRACE_OAH
  int inputLineNumber =
    elt->getInputLineNumber ();
#endif

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
      fMxmltree->
        // this score needs the 'custom short barline' Scheme function
        setCustomShortBarlineSchemeFunctionIsNeeded ();
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

void msr2MxmltreeTranslator::visitEnd (S_msrBarline& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrBarline" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrVarValAssoc& elt)
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

      fCurrentLpsrScoreHeader->
        setWorkNumber (
          inputLineNumber,
          variableValue,
          kFontStyleNone,
          kFontWeightNone);

      fWorkNumberKnown = true;
      break;

    case msrVarValAssoc::kWorkTitle:
      fCurrentIdentification->
        setWorkTitle (
          inputLineNumber, variableValue);

      fCurrentLpsrScoreHeader->
        setWorkTitle (
          inputLineNumber,
          variableValue,
          kFontStyleNone,
          kFontWeightNone);

      fWorkTitleKnown = true;
      break;

    case msrVarValAssoc::kMovementNumber:
      fCurrentIdentification->
        setMovementNumber (
          inputLineNumber, variableValue);

      fCurrentLpsrScoreHeader->
        setMovementNumber (
          inputLineNumber,
          variableValue,
          kFontStyleNone,
          kFontWeightNone);

      fMovementNumberKnown = true;
      break;

    case msrVarValAssoc::kMovementTitle:
      fCurrentIdentification->
        setMovementTitle (
          inputLineNumber, variableValue);

      fCurrentLpsrScoreHeader->
        setMovementTitle (
          inputLineNumber,
          variableValue,
          kFontStyleNone,
          kFontWeightNone);

      fMovementTitleKnown = true;
      break;

    case msrVarValAssoc::kEncodingDate:
      fCurrentIdentification->
        setEncodingDate (
          inputLineNumber, variableValue);

      fCurrentLpsrScoreHeader->
        setEncodingDate (
          inputLineNumber,
          variableValue,
          kFontStyleNone,
          kFontWeightNone);
      break;

    case msrVarValAssoc::kScoreInstrument:
      fCurrentIdentification->
        setScoreInstrument (
          inputLineNumber, variableValue);

      fCurrentLpsrScoreHeader->
        setScoreInstrument (
          inputLineNumber,
          variableValue,
          kFontStyleNone,
          kFontWeightNone);
      break;

    case msrVarValAssoc::kMiscellaneousField:
      fCurrentIdentification->
        setMiscellaneousField (
          inputLineNumber, variableValue);

      fCurrentLpsrScoreHeader->
        setMiscellaneousField (
          inputLineNumber,
          variableValue,
          kFontStyleNone,
          kFontWeightNone);
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

void msr2MxmltreeTranslator::visitEnd (S_msrVarValAssoc& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrVarValsListAssoc& elt)
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
        fCurrentLpsrScoreHeader->
          addRights (
            inputLineNumber, (*i));
      } // for
      break;

    case msrVarValsListAssoc::kComposer:
      for (list<string>::const_iterator i = variableValuesList.begin ();
        i != variableValuesList.end ();
        i++
      ) {
        fCurrentLpsrScoreHeader->
          addComposer (
            inputLineNumber, (*i));
      } // for
      break;

    case msrVarValsListAssoc::kArranger:
      for (list<string>::const_iterator i = variableValuesList.begin ();
        i != variableValuesList.end ();
        i++
      ) {
        fCurrentLpsrScoreHeader->
          addArranger (
            inputLineNumber, (*i));
      } // for
      break;

    case msrVarValsListAssoc::kLyricist:
      for (list<string>::const_iterator i = variableValuesList.begin ();
        i != variableValuesList.end ();
        i++
      ) {
        fCurrentLpsrScoreHeader->
          addLyricist (
            inputLineNumber, (*i));
      } // for
      break;

    case msrVarValsListAssoc::kPoet:
      for (list<string>::const_iterator i = variableValuesList.begin ();
        i != variableValuesList.end ();
        i++
      ) {
        fCurrentLpsrScoreHeader->
          addPoet (
            inputLineNumber, (*i));
      } // for
      break;

    case msrVarValsListAssoc::kTranslator:
      for (list<string>::const_iterator i = variableValuesList.begin ();
        i != variableValuesList.end ();
        i++
      ) {
        fCurrentLpsrScoreHeader->
          addTranslator (
            inputLineNumber, (*i));
      } // for
      break;

    case msrVarValsListAssoc::kArtist:
      for (list<string>::const_iterator i = variableValuesList.begin ();
        i != variableValuesList.end ();
        i++
      ) {
        fCurrentLpsrScoreHeader->
          addArtist (
            inputLineNumber, (*i));
      } // for
      break;

    case msrVarValsListAssoc::kSoftware:
      for (list<string>::const_iterator i = variableValuesList.begin ();
        i != variableValuesList.end ();
        i++
      ) {
        fCurrentLpsrScoreHeader->
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

void msr2MxmltreeTranslator::visitEnd (S_msrVarValsListAssoc& elt)
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
void msr2MxmltreeTranslator::visitStart (S_msrPageLayout& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrPageLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter++;
}

void msr2MxmltreeTranslator::visitEnd (S_msrPageLayout& elt)
{
  gIndenter--;

#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrPageLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrMidiTempo& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrMidiTempo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void msr2MxmltreeTranslator::visitEnd (S_msrMidiTempo& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrMidiTempo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}
*/

} // namespace
