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
//#include <climits>      // INT_MIN, INT_MAX
#include <iomanip>      // setw, setprecision, ...
//#include <algorithm>    // for_each

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

#include "version.h"

#include "mxmlTree.h"


using namespace std;

namespace MusicXML2
{
/* JMI
//________________________________________________________________________
// a comparison class to sort elements
class musicXMLOrder
{
	public:

    // constructors/destructor
    // ------------------------------------------------------

    musicXMLOrder (
      map<int,int>& order, Sxmlelement container);

		virtual	~musicXMLOrder ();

	public:

    // services
    // ------------------------------------------------------

		bool	                operator()	(Sxmlelement a, Sxmlelement b);

  private:

    // fields
    // ------------------------------------------------------

	  map<int,int>&	        fOrder;
	  Sxmlelement		        fContainer;
};

musicXMLOrder::musicXMLOrder (
  map<int,int>& order, Sxmlelement container)
  : fOrder (order)
{
  fContainer = container;
}

musicXMLOrder::~musicXMLOrder ()
{}

bool musicXMLOrder::operator() (Sxmlelement a, Sxmlelement b)
{
	int aIndex = fOrder [a->getType ()];
	int bIndex = fOrder [b->getType ()];

	if (aIndex == 0) return false; // wrong a element: reject to end of list
	if (bIndex == 0) return true;	 // wrong b element: reject to end of list

	return aIndex < bIndex;
}
*/

//________________________________________________________________________
void msr2MxmltreeTranslator::handleWorkSubElement (
  Sxmlelement elem)
{
  if (! fScoreWorkElement) {
    // create a work element
    fScoreWorkElement = createElement (k_work, "");
  }

  fScoreWorkElement->push (elem);
}

void msr2MxmltreeTranslator::handleIdentificationSubElement (
  Sxmlelement elem)
{
  if (! fScoreIdentificationElement) {
    // create an identification element
    fScoreIdentificationElement = createElement (k_identification, "");
  }

  fScoreIdentificationElement->push (elem);
}

void msr2MxmltreeTranslator::handleIdentificationEncodingSubElement (
  Sxmlelement elem)
{
  if (! fScoreIdentificationEncodingElement) {
    // create an encoding element
    fScoreIdentificationEncodingElement = createElement (k_encoding, "");

    // append it to the identification element
    handleIdentificationSubElement (fScoreIdentificationEncodingElement);
  }

  fScoreIdentificationEncodingElement->push (elem);
}

void msr2MxmltreeTranslator::handleAttributesSubElement (
  Sxmlelement elem)
{
  if (! fCurrentPartAttributes) {
    // create an attributes element
    fCurrentPartAttributes = createElement (k_attributes, "");

    // append the attributes element to the current measure element
    fCurrentMeasureElement->push (fCurrentPartAttributes);
  }

  // append elem to the current attributes element
  fCurrentPartAttributes->push (elem);
}

void msr2MxmltreeTranslator::handleDirectionSubElement (
  Sxmlelement      elem,
  msrPlacementKind placementKind)
{
  string placementString;

  switch (placementKind) {
    case msrPlacementKind::kPlacementNone:
      break;
    case msrPlacementKind::kPlacementAbove:
      placementString = "above";
      break;
    case msrPlacementKind::kPlacementBelow:
      placementString = "below";
      break;
  } // switch

  // create a direction element
  Sxmlelement directionElement = createElement (k_direction, "");

  if (placementString.size ()) {
    // set it's "placement" attribute
    directionElement->add (createAttribute ("placement", placementString));
  }

  // append it to the current measure element
  fCurrentMeasureElement->push (directionElement);

  // create a direction type element
  Sxmlelement directionTypeElement = createElement (k_direction_type, "");

  // append it to the current direction element
  directionElement->push (directionTypeElement);

  // append elem to the direction type element
  directionTypeElement->push (elem);
}

void msr2MxmltreeTranslator::handleMeasureLevelElement (
  Sxmlelement elem)
{
  // append the attributes element the current measure element
  fCurrentMeasureElement->push (elem);

  // forget about the current attributes element
  fCurrentPartAttributes = nullptr;
}

void msr2MxmltreeTranslator::handleNoteNotationsSubElement (
  Sxmlelement      noteElement,
  Sxmlelement      elem,
  msrPlacementKind placementKind)
{
  if (! fCurrentNoteNotationsElement) {
    // create an notations element
    fCurrentNoteNotationsElement = createElement (k_notations, "");

    // append it to noteElement
    noteElement->push (fCurrentNoteNotationsElement);
  }

  // append elem to the note notations element
  fCurrentNoteNotationsElement->push (elem);
}

//________________________________________________________________________
msr2MxmltreeTranslator::msr2MxmltreeTranslator (
  indentedOstream& ios,
  S_msrScore       mScore)
    : fLogOutputStream (ios)
{
  // the MSR score we're visiting
  fVisitedMsrScore = mScore;

  // create the current score part-wise element
  fScorePartWiseElement = createScorePartWiseElement ();

/*
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

  // stanzas
  fOnGoingStanza = false;

  // syllables
  fOnGoingSyllableExtend = false;
  */
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

  // create the initial creation comment
  stringstream s;
  s <<
    " ===== " <<
    "Created on " <<
    gGeneralOah->fTranslationDateFull <<
    " from " <<
    gOahOah->fInputSourceName <<
    " ===== ";

  fInitialCreationComment = createElement (kComment, s.str ());

  // create a software element
  Sxmlelement
    softwareElement =
      createElement (
        k_software,
        gOahOah->fHandlerExecutableName
          + " "
          + currentVersionNumber () +
          ", https://github.com/dfober/libmusicxml/tree/lilypond");

  // append it to the identification encoding
  handleIdentificationEncodingSubElement (softwareElement);

  // create an encoding date element
  Sxmlelement
    encodingDateElement =
      createElement (
        k_encoding_date,
        gGeneralOah->fTranslationDateYYYYMMDD);

  // append it to the identification encoding
  handleIdentificationEncodingSubElement (encodingDateElement);

  // create the part list element
  fScorePartListElement = createElement (k_part_list, "");
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

  // append the initial creation comment to the score part wise element
  fScorePartWiseElement->push (fInitialCreationComment);

  // append the score work element to the score part wise element
  fScorePartWiseElement->push (fScoreWorkElement);

  // append the identification element to the score part wise element
  fScorePartWiseElement->push (fScoreIdentificationElement);

  // append the part list element to the score part wise element
  fScorePartWiseElement->push (fScorePartListElement);

  // append the pending parts elements to the score part wise element
  list<Sxmlelement>::const_iterator i;

  for (i=fPendingPartElementsList.begin (); i!=fPendingPartElementsList.end (); i++) {
    Sxmlelement partElement = (*i);

    // create a part comment
    stringstream s;
    s <<
      " ============================ " <<
      "PART" <<
      " \"" << partElement->getAttributeValue ("id") << "\"" <<
      " ============================= ";
    Sxmlelement partComment = createElement (kComment, s.str ());

    // append it to the score part wise element
    fScorePartWiseElement->push (partComment);

    // append the part element to the score part wise element
    fScorePartWiseElement->push (partElement);
  } // for
}

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

  // work title
  S_msrVarValAssoc
    workTitle = elt->getWorkTitle ();

  if (workTitle) {
  /* JMI
    msrVarValAssoc::msrVarValAssocKind
      varValAssocKind =
        workTitle->getVarValAssocKind ();
        */
    string
      variableValue =
        workTitle->getVariableValue ();

    // create the work title element
    Sxmlelement workTitleElement = createElement (k_work_title, variableValue);

    // append it to the score part wise element
    handleWorkSubElement (workTitleElement);
  }
}

void msr2MxmltreeTranslator::visitEnd (S_msrIdentification& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> End visiting msrIdentification" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

/*
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
      fScorePartWiseElement->getScorePaper ();

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
  fScorePartWiseElement->
    setScoreGlobalStaffSizeSchemeVariable (
      globalStaffSize);

  // get LPSR score block layout
  S_lpsrLayout
    scoreBlockLayout =
      fScorePartWiseElement->getScoreLayout ();

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
  fScorePartWiseElement->
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
        fScorePartWiseElement->getMsrScore ());

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
    partName =
      elt->getPartName (),
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

  // create a score part element
  Sxmlelement scorePartElement = createElement (k_score_part, "");
  // set it's "id" attribute
  scorePartElement->add (createAttribute ("id", partID));

  // append it to the part list element
  fScorePartListElement->push (scorePartElement);

  // create a part name element
  Sxmlelement partNameElement = createElement (k_part_name, partName);

  // append it to the score part element
  scorePartElement->push (partNameElement);

  // create a part element
  fCurrentPartElement = createElement (k_part, "");
  // set its "id" attribute
	fCurrentPartElement->add (createAttribute ("id", partID));

  // append it to the pending part elements list
  fPendingPartElementsList.push_back (fCurrentPartElement);

  // get the part shortest note's duration and tuplet factor
  fPartShortestNoteDuration =
    elt->getPartShortestNoteDuration (),
  fPartShortestNoteTupletFactor =
    elt->getPartShortestNoteTupletFactor ();

  // compute the divisions per quarter note
  if (fPartShortestNoteDuration > rational (1, 4)) {
    // the shortest duration should be a quarter note at most
    fPartShortestNoteDuration = rational (1, 4);
  }

  fDivisionsPerQuarterNote =
    rational (1, 4)
      /
    fPartShortestNoteDuration;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotes) {
    fLogOutputStream <<
      "-->  partShortestNoteDuration: " << fPartShortestNoteDuration <<
      "-->  partShortestNoteTupletFactor: " << fPartShortestNoteTupletFactor <<
      "-->  fDivisionsPerQuarterNote: " << fDivisionsPerQuarterNote <<
      endl;
  }
#endif

  // create a divisions element
  fCurrentDivisionsElement =
    createIntegerElement (
      k_divisions,
      fDivisionsPerQuarterNote);
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

  // forget about the current divisions element
  fCurrentDivisionsElement = nullptr;

  // forget about the current part element
  fCurrentPartElement = nullptr;

  // forget about the current attributes element
  fCurrentPartAttributes = nullptr;
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

  // create a comment
  stringstream s;
  s <<
    " ==================== " <<
    "Segment " <<
    elt->getSegmentAbsoluteNumber () <<
    " START" <<
    " ==================== ";
  Sxmlelement comment = createElement (kComment, s.str ());

  // append it to the current part element
  fCurrentPartElement->push (comment);
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

  // create a comment
  stringstream s;
  s <<
    " ==================== " <<
    "Segment " <<
    elt->getSegmentAbsoluteNumber () <<
    " END" <<
    " ==================== ";
  Sxmlelement comment = createElement (kComment, s.str ());

  // append it to the current part element
  fCurrentPartElement->push (comment);
}

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
//      "', voice \"" <<
//      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber << " ===-->" <<
      endl;
  }
#endif

  // create a comment
  stringstream s;
  s <<
    " ===== " <<
    "MEASURE " <<
    "ordinal number: " << elt->getMeasureOrdinalNumberInVoice () <<
    " ===== ";
  Sxmlelement comment = createElement (kComment, s.str ());

  // append it to the current part element
  fCurrentPartElement->push (comment);

  // create a measure element
  fCurrentMeasureElement = createElement (k_measure, "");
  // set its "number" attribute
	fCurrentMeasureElement->add (createAttribute ("number", measureNumber));
  // append it to the current part element
  fCurrentPartElement->push (fCurrentMeasureElement);

  // is there a divisions element to be appended?
  if (fCurrentDivisionsElement) {
    // append the divisions element to the attributes element
    handleAttributesSubElement (fCurrentDivisionsElement);
  }
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

/*
      switch (fCurrentClefOctaveChange) {
        case -2:
          clefKind = kTrebleMinus15Clef;
          break;
        case -1:
          clefKind = kTrebleMinus8Clef;
          break;
        case 0:
          clefKind = kTrebleClef;
          break;
        case +1:
          clefKind = kTreblePlus8Clef;
          break;
        case +2:
          clefKind = kTreblePlus15Clef;
          break;
        default:
          {
            // should not occur
          }
      } // switch
*/
  switch (elt->getClefKind ()) {
    case k_NoClef:
      break;
    case kTrebleClef:
      {
        Sxmlelement clefElement = createElement (k_clef, "");

        clefElement->push (
          createElement (
            k_sign,
            "G"));
        clefElement->push (
          createIntegerElement (
            k_line,
            2));

        handleAttributesSubElement (clefElement);
      }
      break;
    case kSopranoClef:
      break;
    case kMezzoSopranoClef:
      break;
    case kAltoClef:
      break;
    case kTenorClef:
      break;
    case kBaritoneClef:
      break;
    case kBassClef:
      break;
    case kTrebleLine1Clef:
      break;
    case kTrebleMinus15Clef:
      break;
    case kTrebleMinus8Clef:
      {
        Sxmlelement clefElement = createElement (k_clef, "");

        clefElement->push (
          createElement (
            k_sign,
            "G"));
        clefElement->push (
          createIntegerElement (
            k_line,
            2));
        clefElement->push (
          createIntegerElement (
            k_clef_octave_change,
            -1));

        handleAttributesSubElement (clefElement);
      }
      break;
    case kTreblePlus8Clef:
      break;
    case kTreblePlus15Clef:
      break;
    case kBassMinus15Clef:
      break;
    case kBassMinus8Clef:
      break;
    case kBassPlus8Clef:
      break;
    case kBassPlus15Clef:
      break;
    case kVarbaritoneClef:
      break;

    case kTablature4Clef:
    case kTablature5Clef:
    case kTablature6Clef:
    case kTablature7Clef:
      break;

    case kPercussionClef:
      break;
    case kJianpuClef:
      break;
  } // switch
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

  switch (elt->getKeyKind ()) {
    case msrKey::kTraditionalKind:
      {
        // create the key element
        Sxmlelement keyElement = createElement (k_key, "");

        // compute the number of fifths
        int fifthsNumber = -1;

        msrQuarterTonesPitchKind
          keyTonicQuarterTonesPitchKind =
            elt->getKeyTonicQuarterTonesPitchKind ();

        switch (keyTonicQuarterTonesPitchKind) {
          case k_NoQuarterTonesPitch_QTP:
          case k_Rest_QTP:
            // should not occur
            break;

          case kC_Natural_QTP:
            fifthsNumber = 0;
            break;
          case kG_Natural_QTP:
            fifthsNumber = 1;
            break;
          case kD_Natural_QTP:
            fifthsNumber = 2;
            break;
          case kA_Natural_QTP:
            fifthsNumber = 3;
            break;
          case kE_Natural_QTP:
            fifthsNumber = 4;
            break;
          case kB_Natural_QTP:
            fifthsNumber = 5;
            break;
          case kF_Sharp_QTP:
            fifthsNumber = 6;
            break;
          case kC_Sharp_QTP:
            fifthsNumber = 7;
            break;

          case kG_Sharp_QTP: // JMI
            fifthsNumber = 8;
            break;
          case kD_Sharp_QTP:
            fifthsNumber = 9;
            break;
          case kA_Sharp_QTP:
            fifthsNumber = 10;
            break;
          case kE_Sharp_QTP:
            fifthsNumber = 11;
            break;

          case kF_Natural_QTP:
            fifthsNumber = -1;
            break;
          case kB_Flat_QTP:
            fifthsNumber = -2;
            break;
          case kE_Flat_QTP:
            fifthsNumber = -3;
            break;
          case kA_Flat_QTP:
            fifthsNumber = -4;
            break;
          case kD_Flat_QTP:
            fifthsNumber = -5;
            break;
          case kG_Flat_QTP:
            fifthsNumber = -6;
            break;
          case kC_Flat_QTP:
            fifthsNumber = -7;
            break;

          case kF_Flat_QTP: // JMI
            fifthsNumber = -8;
            break;
          case kB_DoubleFlat_QTP:
            fifthsNumber = -9;
            break;
          case kE_DoubleFlat_QTP:
            fifthsNumber = -10;
            break;
          case kA_DoubleFlat_QTP:
            fifthsNumber = -11;
            break;

          default:
            // should not occur
            break;
        } // switch

        if (fifthsNumber >= 0) {
          // populate the key element
          keyElement->push (
            createIntegerElement (
              k_fifths,
              fifthsNumber));

          keyElement->push (
            createElement (
              k_mode,
              msrKey::keyModeKindAsString (elt->getKeyModeKind ())));

          // append it to the attributes element
          handleAttributesSubElement (keyElement);
        }

        else {
          stringstream s;

          s <<
            "keyTonicQuarterTonesPitchKind '" <<
            msrQuarterTonesPitchKindAsString (
              gMsrOah->fMsrQuarterTonesPitchesLanguageKind,
              keyTonicQuarterTonesPitchKind) <<
            "' cannot be handled";

          msrInternalError (
            gOahOah->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
      }
      break;

    case msrKey::kHumdrumScotKind:
      {
        // JMI
      }
      break;
  } // switch
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

/*
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


  // handle the time
  if (timeSymbolKind == msrTime::kTimeSymbolSenzaMisura) {
    // senza misura time

    fVoiceIsCurrentlySenzaMisura = true;
  }

  else {
    // con misura time

    int timesItemsNumber =
      timeItemsVector.size ();

    if (timesItemsNumber) {
      // should there be a single number?
      switch (timeSymbolKind) {
        case msrTime::kTimeSymbolCommon:
          break;
        case msrTime::kTimeSymbolCut:
          break;
        case msrTime::kTimeSymbolNote:
          break;
        case msrTime::kTimeSymbolDottedNote:
          break;
        case msrTime::kTimeSymbolSingleNumber:
          fLilypondCodeOstream <<
            "\\once\\override Staff.TimeSignature.style = #'single-digit" <<
            endl;
          break;
        case msrTime::kTimeSymbolSenzaMisura:
          break;
        case msrTime::kTimeSymbolNone:
          break;
      } // switch

      if (! elt->getTimeIsCompound ()) {
        // simple time
        // \time "3/4" for 3/4
        // or senza misura

        S_msrTimeItem
          timeItem =
            timeItemsVector [0]; // the only element;

        // fetch the time item beat numbers vector
        const vector<int>&
          beatsNumbersVector =
            timeItem->
              getTimeBeatsNumbersVector ();

        // should the time be numeric?
        if (
          timeSymbolKind == msrTime::kTimeSymbolNone
            ||
          gLilypondOah->fNumericalTime) {
          fLilypondCodeOstream <<
            "\\numericTimeSignature ";
        }

        fLilypondCodeOstream <<
          "\\time " <<
          beatsNumbersVector [0] << // the only element
          "/" <<
          timeItem->getTimeBeatValue () <<
          endl;
      }

      else {
        // compound time
        // \compoundMeter #'(3 2 8) for 3+2/8
        // \compoundMeter #'((3 8) (2 8) (3 4)) for 3/8+2/8+3/4
        // \compoundMeter #'((3 2 8) (3 4)) for 3+2/8+3/4

        fLilypondCodeOstream <<
          "\\compoundMeter #`(";

        // handle all the time items in the vector
        for (int i = 0; i < timesItemsNumber; i++) {
          S_msrTimeItem
            timeItem =
              timeItemsVector [i];

          // fetch the time item beat numbers vector
          const vector<int>&
            beatsNumbersVector =
              timeItem->
                getTimeBeatsNumbersVector ();

          int beatsNumbersNumber =
            beatsNumbersVector.size ();

          // first generate the opening parenthesis
          fLilypondCodeOstream <<
            "(";

          // then generate all beats numbers in the vector
          for (int j = 0; j < beatsNumbersNumber; j++) {
            fLilypondCodeOstream <<
              beatsNumbersVector [j] <<
              ' ';
          } // for

          // then generate the beat type
          fLilypondCodeOstream <<
            timeItem->getTimeBeatValue ();

          // and finally generate the closing parenthesis
          fLilypondCodeOstream <<
            ")";

          if (i != timesItemsNumber - 1) {
            fLilypondCodeOstream <<
              ' ';
          }
        } // for

      fLilypondCodeOstream <<
        ")" <<
        endl;
      }
    }

    else {
      // there are no time items
      if (timeSymbolKind != msrTime::kTimeSymbolSenzaMisura) {
        msrInternalError (
          gOahOah->fInputSourceName,
          elt->getInputLineNumber (),
          __FILE__, __LINE__,
          "time items vector is empty");
      }
    }
  }
*/

  switch (elt->getTimeSymbolKind ()) {
    case msrTime::kTimeSymbolCommon:
      {
        Sxmlelement timeElement = createElement (k_time, "");

        timeElement->add (createAttribute ("symbol", "common"));

        timeElement->push (
          createIntegerElement (
            k_beats,
            4));
        timeElement->push (
          createIntegerElement (
            k_beat_type,
            4));

        handleAttributesSubElement (timeElement);
      }
      break;
    case msrTime::kTimeSymbolCut:
       {
        Sxmlelement timeElement = createElement (k_time, "");

        timeElement->add (createAttribute ("symbol", "cut"));

        timeElement->push (
          createIntegerElement (
            k_beats,
            2));
        timeElement->push (
          createIntegerElement (
            k_beat_type,
            2));

        handleAttributesSubElement (timeElement);
      }
     break;
    case msrTime::kTimeSymbolNote:
      break;
    case msrTime::kTimeSymbolDottedNote:
      break;
    case msrTime::kTimeSymbolSingleNumber:
      break;
    case msrTime::kTimeSymbolSenzaMisura:
      break;
    case msrTime::kTimeSymbolNone:
      {
        const vector<S_msrTimeItem>&
          timeItemsVector =
            elt->getTimeItemsVector ();

        if (! elt->getTimeIsCompound ()) {
          // simple time
          // \time "3/4" for 3/4
          // or senza misura

          S_msrTimeItem
            timeItem =
              timeItemsVector [0]; // the only element;

          // fetch the time item beat numbers vector
          const vector<int>&
            beatsNumbersVector =
              timeItem->
                getTimeBeatsNumbersVector ();

          Sxmlelement timeElement = createElement (k_time, "");

          timeElement->push (
            createIntegerElement (
              k_beats,
              beatsNumbersVector [0])); // the only element
          timeElement->push (
            createIntegerElement (
              k_beat_type,
              timeItem->getTimeBeatValue ()));

          handleAttributesSubElement (timeElement);
        }

        else {
          // compound time
          // \compoundMeter #'(3 2 8) for 3+2/8
          // \compoundMeter #'((3 8) (2 8) (3 4)) for 3/8+2/8+3/4
          // \compoundMeter #'((3 2 8) (3 4)) for 3+2/8+3/4

        }
      }
      break;
  } // switch
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
void msr2MxmltreeTranslator::visitStart (S_msrTempo& elt)
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

/*
  const list<S_msrWords>&
    tempoWordsList =
      elt->getTempoWordsList ();

 int tempoWordsListSize = tempoWordsList.size ();
*/

  msrDottedDuration tempoBeatUnit  = elt->getTempoBeatUnit ();
  string            tempoPerMinute = elt->getTempoPerMinute ();

  msrTempo::msrTempoParenthesizedKind
    tempoParenthesizedKind =
      elt->getTempoParenthesizedKind ();

  msrPlacementKind
    tempoPlacementKind =
      elt->getTempoPlacementKind ();

  switch (elt->getTempoKind ()) {
    case msrTempo::k_NoTempoKind:
      break;

    case msrTempo::kTempoBeatUnitsWordsOnly:
      {
    /*
        fLilypondCodeOstream <<
          "\\tempo ";

        if (tempoWordsListSize) {
          list<S_msrWords>::const_iterator
            iBegin = tempoWordsList.begin (),
            iEnd   = tempoWordsList.end (),
            i      = iBegin;

          for ( ; ; ) {
            S_msrWords words = (*i);

            fLilypondCodeOstream <<
              "\"" << words->getWordsContents () << "\"";

            if (++i == iEnd) break;

            fLilypondCodeOstream <<
              ' ';
          } // for

          fLilypondCodeOstream <<
            endl;
        }
        */
        }
      break;

    case msrTempo::kTempoBeatUnitsPerMinute:
      switch (tempoParenthesizedKind) {
        case msrTempo::kTempoParenthesizedYes:
          {
          // create the metronome element
          Sxmlelement metronomeElement = createElement (k_metronome, "");

          // set its "parentheses" attribute
          metronomeElement->add (createAttribute ("parentheses", "yes"));

          // append the beat unit element to the metronome elements
          msrDurationKind
            durationKind =
              tempoBeatUnit.getDurationKind ();
              /* JMI
          int
            dotsNumber =
              tempoBeatUnit.getDotsNumber ();
              */

          metronomeElement-> push (
            createElement (
              k_beat_unit,
              msrDurationKindAsMusicXMLType (durationKind)));

          // append the per minute element to the metronome elements
          metronomeElement-> push (createElement (k_per_minute, tempoPerMinute));

          // append the dynamics element to the current measure element
          handleDirectionSubElement (
            metronomeElement,
            tempoPlacementKind);

  /*
          if (tempoWordsListSize) {
            list<S_msrWords>::const_iterator
              iBegin = tempoWordsList.begin (),
              iEnd   = tempoWordsList.end (),
              i      = iBegin;

            for ( ; ; ) {
              S_msrWords words = (*i);

              fLilypondCodeOstream <<
                "\"" << words->getWordsContents () << "\"";

              if (++i == iEnd) break;

              fLilypondCodeOstream <<
                ' ';
            } // for
          }
          */
        }
        break;

      case msrTempo::kTempoParenthesizedNo:
        {
          // create the metronome element
          Sxmlelement metronomeElement = createElement (k_metronome, "");

          // append the beat unit element to the metronome elements
          msrDurationKind
            durationKind =
              tempoBeatUnit.getDurationKind ();
              /*
          int
            dotsNumber =
              tempoBeatUnit.getDotsNumber ();
              */

          metronomeElement-> push (
            createElement (
              k_beat_unit,
              msrDurationKindAsMusicXMLType (durationKind)));

          // append the per minute element to the metronome elements
          metronomeElement-> push (createElement (k_per_minute, tempoPerMinute));

          // append the metronome element to the current measure element
          handleDirectionSubElement (
            metronomeElement,
            tempoPlacementKind);

  /*
          if (tempoWordsListSize) {
            list<S_msrWords>::const_iterator
              iBegin = tempoWordsList.begin (),
              iEnd   = tempoWordsList.end (),
              i      = iBegin;

            for ( ; ; ) {
              S_msrWords words = (*i);

              fLilypondCodeOstream <<
                "\"" << words->getWordsContents () << "\"";

              if (++i == iEnd) break;

              fLilypondCodeOstream <<
                ' ';
            } // for
          }
          */
        }
          break;
        } // switch
      break;

    case msrTempo::kTempoBeatUnitsEquivalence:
      {
    /*
      fLilypondCodeOstream <<
        "\\tempo ";

      if (tempoWordsListSize) {
        list<S_msrWords>::const_iterator
          iBegin = tempoWordsList.begin (),
          iEnd   = tempoWordsList.end (),
          i      = iBegin;

        for ( ; ; ) {
          S_msrWords words = (*i);

          fLilypondCodeOstream <<
            "\"" << words->getWordsContents () << "\"";

          if (++i == iEnd) break;

          fLilypondCodeOstream <<
            ' ';
        } // for
      }

      fLilypondCodeOstream <<
        ' ' <<
        "\\markup {" <<
        endl;

      gIndenter++;

      fLilypondCodeOstream <<
        "\\concat {" <<
        endl;

      gIndenter++;

      switch (tempoParenthesizedKind) {
        case msrTempo::kTempoParenthesizedYes:
          fLilypondCodeOstream <<
            "(" <<
            endl;
          break;
        case msrTempo::kTempoParenthesizedNo:
          break;
      } // switch

      gIndenter++;

      if (gLpsrOah->versionNumberGreaterThanOrEqualTo ("2.20")) {
        fLilypondCodeOstream <<
          " \\smaller \\general-align #Y #DOWN \\note {";
      }
      else {
        fLilypondCodeOstream <<
          " \\smaller \\general-align #Y #DOWN \\note #\"";
      }

      fLilypondCodeOstream <<
        dottedDurationAsLilypondStringWithoutBackSlash (
          inputLineNumber,
          tempoBeatUnit);

      if (gLpsrOah->versionNumberGreaterThanOrEqualTo ("2.20")) {
        fLilypondCodeOstream <<
          "} #UP";
      }
      else {
        fLilypondCodeOstream <<
          "\" #UP";
      }

      fLilypondCodeOstream <<
        endl <<
        "\" = \"" <<
        endl;

      fLilypondCodeOstream <<
        "(";
      if (gLpsrOah->versionNumberGreaterThanOrEqualTo ("2.20")) {
        fLilypondCodeOstream <<
          " \\smaller \\general-align #Y #DOWN \\note {";
      }
      else {
        fLilypondCodeOstream <<
          " \\smaller \\general-align #Y #DOWN \\note #\"";
      }

      fLilypondCodeOstream <<
        dottedDurationAsLilypondStringWithoutBackSlash (
          inputLineNumber,
          elt->getTempoEquivalentBeatUnit ());

      if (gLpsrOah->versionNumberGreaterThanOrEqualTo ("2.20")) {
        fLilypondCodeOstream <<
          "} #UP";
      }
      else {
        fLilypondCodeOstream <<
          "\" #UP";
      }

      fLilypondCodeOstream << endl;

      gIndenter--;

      switch (tempoParenthesizedKind) {
        case msrTempo::kTempoParenthesizedYes:
          fLilypondCodeOstream <<
            ")" <<
            endl;
          break;
        case msrTempo::kTempoParenthesizedNo:
          break;
      } // switch

      gIndenter--;

      fLilypondCodeOstream <<
        "}" <<
        endl;

      gIndenter--;

      fLilypondCodeOstream <<
        "}" <<
        endl;
        */
        }
      break;

    case msrTempo::kTempoNotesRelationShip:
      {
    /*
      fLilypondCodeOstream <<
        "\\tempoRelationship #\"";

      if (tempoWordsListSize) {
        list<S_msrWords>::const_iterator
          iBegin = tempoWordsList.begin (),
          iEnd   = tempoWordsList.end (),
          i      = iBegin;

        for ( ; ; ) {
          S_msrWords words = (*i);

          fLilypondCodeOstream <<
     // JMI       "\"" <<
            words->getWordsContents (); // JMI <<
      // JMI      "\"";

          if (++i == iEnd) break;

          fLilypondCodeOstream <<
            ' ';
        } // for
      }

      fLilypondCodeOstream <<
        "\"";

      switch (tempoParenthesizedKind) {
        case msrTempo::kTempoParenthesizedYes:
          fLilypondCodeOstream <<
            " ##t";
          break;
        case msrTempo::kTempoParenthesizedNo:
          fLilypondCodeOstream <<
            " ##f";
          break;
      } // switch

      fLilypondCodeOstream << endl;
      */
      }
      break;
  } // switch
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
void msr2MxmltreeTranslator:: createNoteDirections (S_msrNote note)
{
  // append the wedges elements if any
  const list<S_msrWedge>&
    noteWedges =
      note->getNoteWedges () ;

  if (noteWedges.size ()) {
    list<S_msrWedge>::const_iterator i;

    for (i=noteWedges.begin (); i!=noteWedges.end (); i++) {
      S_msrWedge wedge = (*i);

      msrWedge::msrWedgeKind wedgeKind = wedge->getWedgeKind ();

      string typeString;

      switch (wedgeKind) {
        case msrWedge::kWedgeKindNone:
          // should not occur
          break;
        case msrWedge::kWedgeCrescendo:
          typeString = "crescendo";
          break;
        case msrWedge::kWedgeDecrescendo:
          typeString = "diminuendo";
          break;
        case msrWedge::kWedgeStop:
          typeString = "stop";
          break;
      } // switch

      // create the wedge element
      Sxmlelement wedgeElement = createElement (k_wedge, "");

      // set its "type" attribute
      wedgeElement->add (createAttribute ("type", typeString));

      // append the wedge element to the current measure element
      handleDirectionSubElement (
        wedgeElement,
        wedge->getWedgePlacementKind ());
    } // for
  }

  // append the dynamics elements if any
  const list<S_msrDynamics>&
    noteDynamics =
      note->getNoteDynamics () ;

  if (noteDynamics.size ()) {
    list<S_msrDynamics>::const_iterator i;

    for (i=noteDynamics.begin (); i!=noteDynamics.end (); i++) {
      S_msrDynamics dynamics = (*i);

      // create the dynamics element
      Sxmlelement dynamicsElement = createElement (k_dynamics, "");

      // create the dynamics specific sub-element
      int subElementID = -1;

      switch (dynamics->getDynamicsKind ()) {
        case msrDynamics::kF:
          subElementID = k_f;
          break;
        case msrDynamics::kFF:
          subElementID = k_ff;
          break;
        case msrDynamics::kFFF:
          subElementID = k_fff;
          break;
        case msrDynamics::kFFFF:
          subElementID = k_ffff;
          break;
        case msrDynamics::kFFFFF:
          subElementID = k_fffff;
          break;
        case msrDynamics::kFFFFFF:
          subElementID = k_ffffff;
          break;

        case msrDynamics::kP:
          subElementID = k_p;
          break;
        case msrDynamics::kPP:
          subElementID = k_pp;
          break;
        case msrDynamics::kPPP:
          subElementID = k_ppp;
          break;
        case msrDynamics::kPPPP:
          subElementID = k_pppp;
          break;
        case msrDynamics::kPPPPP:
          subElementID = k_ppppp;
          break;
        case msrDynamics::kPPPPPP:
          subElementID = k_pppppp;
          break;

        case msrDynamics::kMF:
          subElementID = k_mf;
          break;
        case msrDynamics::kMP:
          subElementID = k_mp;
          break;
        case msrDynamics::kFP:
          subElementID = k_fp;
          break;
        case msrDynamics::kFZ:
          subElementID = k_fz;
          break;
        case msrDynamics::kRF:
          subElementID = k_rf;
          break;
        case msrDynamics::kSF:
          subElementID = k_sf;
          break;

        case msrDynamics::kRFZ:
          subElementID = k_rfz;
          break;
        case msrDynamics::kSFZ:
          subElementID = k_sfz;
          break;
        case msrDynamics::kSFP:
          subElementID = k_sfp;
          break;
        case msrDynamics::kSFPP:
          subElementID = k_sfpp;
          break;
        case msrDynamics::kSFFZ:
          subElementID = k_sffz;
          break;
        case msrDynamics::k_NoDynamics:
          ; // should not occur
          break;
      } // switch

      Sxmlelement dynamicsSpecificSubElement = createElement (subElementID, "");

      // append the dynamics specific sub-element to the dynamics elements
      dynamicsElement-> push (dynamicsSpecificSubElement);

      // append the dynamics element to the current measure element
      handleDirectionSubElement (
        dynamicsElement,
        dynamics->getDynamicsPlacementKind ());
    } // for
  }
}

//________________________________________________________________________
void msr2MxmltreeTranslator:: createNoteNotations (S_msrNote note)
{
  // append the articulation elements if any
  const list<S_msrArticulation>&
    noteArticulations =
      note->getNoteArticulations () ;

  if (noteArticulations.size ()) {
    list<S_msrArticulation>::const_iterator i;

    for (i=noteArticulations.begin (); i!=noteArticulations.end (); i++) {
      S_msrArticulation articulation = (*i);

      msrArticulation::msrArticulationKind
        articulationKind =
          articulation->getArticulationKind ();

      int articulationType;

/* JMI
	((accent | strong-accent | staccato | tenuto |
	  detached-legato | staccatissimo | spiccato |
	  scoop | plop | doit | falloff | breath-mark |
	  caesura | stress | unstress | soft-accent |
	  other-articulation)*)>
	  */

      switch (articulationKind) {
        case msrArticulation::kAccent:
          articulationType = k_accent;
          break;
        case msrArticulation::kBreathMark:
          articulationType = k_breath_mark;
          break;
        case msrArticulation::kCaesura:
          articulationType = k_caesura;
          break;
        case msrArticulation::kSpiccato:
          articulationType = k_spiccato;
          break;
        case msrArticulation::kStaccato:
          articulationType = k_staccato;
          break;
        case msrArticulation::kStaccatissimo:
          articulationType = k_staccatissimo;
          break;
        case msrArticulation::kStress:
          articulationType = k_stress;
          break;
        case msrArticulation::kUnstress:
          articulationType = k_unstress;
          break;
        case msrArticulation::kDetachedLegato:
          articulationType = k_detached_legato;
          break;
        case msrArticulation::kStrongAccent:
          articulationType = k_strong_accent;
          break;
        case msrArticulation::kTenuto:
          articulationType = k_tenuto;
          break;
        case msrArticulation::kFermata:
          articulationType = k_fermata;
          break;
        case msrArticulation::kArpeggiato:
          articulationType = k_arpeggiate;
          break;
        case msrArticulation::kNonArpeggiato:
          articulationType = k_non_arpeggiate;
          break;
        case msrArticulation::kDoit:
          articulationType = k_doit;
          break;
        case msrArticulation::kFalloff:
          articulationType = k_falloff;
          break;
        case msrArticulation::kPlop:
          articulationType = k_plop;
          break;
        case msrArticulation::kScoop:
          articulationType = k_scoop;
          break;
      } // switch

      // create the note articulations element
      Sxmlelement noteArticulationsElement = createElement (k_articulations, "");

      // create the articulation element
      Sxmlelement articulationElement = createElement (articulationType, "");

      // append it to the note articulations element
      noteArticulationsElement->push (articulationElement);

      // append the note articulations element to the current note element
      handleNoteNotationsSubElement (
        fCurrentNoteElement,
        noteArticulationsElement,
        articulation->getArticulationPlacementKind ());
    } // for
  }
}

//________________________________________________________________________
void msr2MxmltreeTranslator:: createNoteElement (S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

  // grab the note's informations
  msrQuarterTonesPitchKind
    noteQuarterTonesPitchKind = note->getNoteQuarterTonesPitchKind ();

  msrDiatonicPitchKind noteDiatonicPitchKind;
  msrAlterationKind    noteAlterationKind;

  fetchDiatonicPitchKindAndAlterationKindFromQuarterTonesPitchKind (
    inputLineNumber,
    noteQuarterTonesPitchKind,
    noteDiatonicPitchKind,
    noteAlterationKind);

  int
    noteOctave     = note->getNoteOctave (),
    noteDotsNumber = note->getNoteDotsNumber ();

  float
    noteMusicXMLAlter =
      msrMusicXMLAlterFromAlterationKind (
        noteAlterationKind);

  rational
    noteSoundingWholeNotes =
      note->getMeasureElementSoundingWholeNotes (),
    noteDisplayWholeNotes =
      note->getNoteDisplayWholeNotes ();

  msrDurationKind
    noteGraphicDurationKind =
      note->getNoteGraphicDurationKind ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotes) {
    fLogOutputStream <<
      "-->  noteSoundingWholeNotes: " << noteSoundingWholeNotes <<
      "-->  noteDisplayWholeNotes: " << noteDisplayWholeNotes <<
      "-->  fDivisionsPerQuarterNote: " << fDivisionsPerQuarterNote <<
      "-->  noteDiatonicPitchKind: " <<
      msrDiatonicPitchKindAsString (noteDiatonicPitchKind) <<
      "-->  noteGraphicDurationKind: " <<
      msrDurationKindAsMusicXMLType (noteGraphicDurationKind) <<
      endl;
  }
#endif

  // create a note element
  fCurrentNoteElement = createElement (k_note, "");

  // append the note element to the current measure element
  handleMeasureLevelElement (fCurrentNoteElement);

/*
  if (! note->getNoteIsAChordsFirstMemberNote ()) {
    // append the chord element
//    fCurrentNoteElement->push (createElement (k_chord, ""));
  }
  */

  // create the step and pitch attributes
  switch (note->getNoteKind ()) {
    case msrNote::k_NoNoteKind:
      break;

    case msrNote::kRestNote:
      fCurrentNoteElement->push (createElement (k_rest, ""));
      break;

    case msrNote::kSkipNote:
      break;

    case msrNote::kUnpitchedNote:
      break;

    case msrNote::kRegularNote:
      {
        // create the pitch element
        Sxmlelement pitchElement = createElement (k_pitch, "");

        // append the step element
        pitchElement->push (
          createElement (
            k_step,
            msrDiatonicPitchKindAsString (noteDiatonicPitchKind)));

        if (noteMusicXMLAlter != 0.0) {
          // append the alter element
          stringstream s;
          s << setprecision (2) << noteMusicXMLAlter;
          pitchElement->push (
            createElement (
              k_alter,
              s.str ()));
        }

        // append the octave element
        pitchElement->push (
          createIntegerElement (
            k_octave,
            noteOctave));

        fCurrentNoteElement->push (pitchElement);
      }
      break;

    case msrNote::kDoubleTremoloMemberNote:
      break;

    case msrNote::kGraceNote:
      break;

    case msrNote::kGraceChordMemberNote:
      break;

    case msrNote::kChordMemberNote:
      {
      /*
        if (! note->getNoteIsAChordsFirstMemberNote ()) {
          // append the chord element
    //      fCurrentNoteElement->push (createElement (k_chord, ""));
        }
        */

        // create the pitch element
        Sxmlelement pitchElement = createElement (k_pitch, "");

        // append the step element
        pitchElement->push (
          createElement (
            k_step,
            msrDiatonicPitchKindAsString (noteDiatonicPitchKind)));

        if (noteMusicXMLAlter != 0.0) {
          // append the alter element
          stringstream s;
          s << setprecision (2) << noteMusicXMLAlter;
          pitchElement->push (
            createElement (
              k_alter,
              s.str ()));
        }

        // append the octave element
        pitchElement->push (
          createIntegerElement (
            k_octave,
            noteOctave));

        fCurrentNoteElement->push (pitchElement);
      }
      break;

    case msrNote::kTupletMemberNote:
      break;

    case msrNote::kGraceTupletMemberNote:
      break;

    case msrNote::kTupletMemberUnpitchedNote:
      break;
  } // switch

  // create the duration attribute
  rational
    durationAsRational =
      noteSoundingWholeNotes / fPartShortestNoteDuration;
  durationAsRational.rationalise ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotes) {
    fLogOutputStream <<
      "--> durationAsRational: " <<
      durationAsRational <<
      endl;
  }
#endif

  if (durationAsRational.getDenominator () != 1) {
    stringstream s;

    s <<
      "durationAsRational '" << durationAsRational <<
      "' is no integer number";

    msrInternalError (
      gOahOah->fInputSourceName,
      note->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  fCurrentNoteElement->push (
    createIntegerElement (
      k_duration,
      durationAsRational.getNumerator ()));

  // append the voice attribute if relevant
  S_msrVoice
    noteVoice =
      note->
        getNoteMeasureUpLink ()->
          getMeasureSegmentUpLink ()->
            getSegmentVoiceUpLink ();
  int
    voiceNumber =
      noteVoice->
        getVoiceNumber ();

  if (voiceNumber != 1) {
    fCurrentNoteElement->push (
      createIntegerElement (
        k_voice,
        voiceNumber));
  }

  // append the type attribute
  fCurrentNoteElement->push (
    createElement (
      k_type,
      msrDurationKindAsMusicXMLType (noteGraphicDurationKind)));

  // append the dots attributes if relevant
  for (int i = 0; i < noteDotsNumber; i++) {
    fCurrentNoteElement->push (
      createElement (
        k_dot, ""));
  } // for

  // append the accidental if any
  msrNote::msrNoteAccidentalKind
    noteAccidentalKind =
      note->getNoteAccidentalKind ();

  string
    accidentalString =
      msrNote::noteAccidentalKindAsMusicXMLString (
        noteAccidentalKind);

  if (accidentalString.size ()) {
    fCurrentNoteElement->push (
      createElement (
        k_accidental,
        accidentalString));
  }

  // append the articulations if any
  createNoteNotations (note);
}

//________________________________________________________________________
void msr2MxmltreeTranslator::visitStart (S_msrNote& elt)
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
  // create the note directions
  createNoteDirections (elt);

  // create the note element
  createNoteElement (elt);
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
      {
        Sxmlelement barlineElement = createElement (k_barline, "");

        barlineElement->add (createAttribute ("location", "right"));

        barlineElement->push (
          createElement (
            k_bar_style,
            "light-heavy"));

        handleMeasureLevelElement (barlineElement);
      }
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
void msr2MxmltreeTranslator::visitStart (S_msrHarmony& elt)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    fLogOutputStream <<
      "--> Start visiting msrHarmony '" <<
      elt->asString () <<
      ", fOnGoingNonGraceNote: " << booleanAsString (fOnGoingNonGraceNote) <<
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
    fScorePartWiseElement ->
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
      fScorePartWiseElement->
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
      fScorePartWiseElement->
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
      fScorePartWiseElement->
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
    fScorePartWiseElement->
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
      fScorePartWiseElement->
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

  fScorePartWiseElement->
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
  fScorePartWiseElement->
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
      fScorePartWiseElement->
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

  fScorePartWiseElement->
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

  fScorePartWiseElement->
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


/*
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

*/
