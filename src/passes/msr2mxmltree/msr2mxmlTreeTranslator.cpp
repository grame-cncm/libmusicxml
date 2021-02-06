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
#include <iomanip>      // setw, setprecision, ...

#include "conversions.h"

#include "msr2mxmlTreeTranslator.h"

#include "elements.h"
#include "factory.h"
#include "xml.h"
#include "xmlfile.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "musicxmlOah.h"
#include "mxmlTreeOah.h"
#include "msrOah.h"
#include "msr2mxmlTreeOah.h"

#include "version.h"

#include "mxmlTree.h"


using namespace std;

namespace MusicXML2
{
//________________________________________________________________________
msr2mxmlTreeTranslator::msr2mxmlTreeTranslator (
  S_msrScore visitedMsrScore)
{
  // the MSR score we're visiting
  fVisitedMsrScore = visitedMsrScore;

  // print layouts
  fOnGoingPrintLayout = false;

  // attributes
  fAnAttributeElementIsNeeded = false;

  // notes
  fANoteHasBeenMetInCurrentMeasure = false;
  fCurrentNoteAwaitsGraceNotes = false;
  fPendingMsrNoteAwaitingGraceNotes = nullptr;
  fPendingNoteElement = nullptr;

  // chords
  fOnGoingChord = false;

  // backup and forward handling
  fCurrentPositionInMeasure = rational (0, 1);

  fCurrentCumulatedSkipsDurations = rational (0, 1);
  fCurrentCumulatedSkipsStaffNumber = -1;
  fCurrentCumulatedSkipsVoiceNumber = -1;

/*
  // double tremolos
  fOnGoingDoubleTremolo = false;

  // stanzas
  fOnGoingStanza = false;

  // syllables
  fOnGoingSyllableExtend = false;
  */
};

msr2mxmlTreeTranslator::~msr2mxmlTreeTranslator ()
{}

//________________________________________________________________________
Sxmlelement msr2mxmlTreeTranslator::buildMxmltreeFromMsrScore ()
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    fVisitedMsrScore != nullptr,
    "fVisitedMsrScore is null");

  // create the current score part-wise element
  fResultingMusicxmlElement =
    createMxmlScorePartWiseElement ();

  // create a msrScore browser
  msrBrowser<msrScore> browser (this);

  // browse the visited score with the browser
  browser.browse (*fVisitedMsrScore);

  return fResultingMusicxmlElement;
}

//________________________________________________________________________
int msr2mxmlTreeTranslator::wholeNotesAsDivisions (
  int      inputLineNumber,
  rational wholeNotes)
{
  rational
    durationAsRational =
      wholeNotes
        /
      fPartShortestNoteDuration
        *
      fDivisionsMultiplyingFactor
        * 4; // divisions are per quarter note
  durationAsRational.rationalise ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMusicxmlOahGroup->getTraceDivisions ()) {
    gLogStream <<
      "--> durationAsRational: " <<
      durationAsRational <<
      "--> line " << inputLineNumber <<
      endl;
  }
#endif

  if (durationAsRational.getDenominator () != 1) {
    stringstream s;

    s <<
      "durationAsRational '" << durationAsRational <<
      "' is no integer number" <<
      ", line " << inputLineNumber;

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return durationAsRational.getNumerator ();
}

//______________________________________________________________________________
string msr2mxmlTreeTranslator::msrLengthAsTenths (
  msrLength length)
{
  float lengthValue = length.getLengthValue ();

  // convert lengthValue to millimeters
  switch (length.getLengthUnitKind ()) {
    case kInchUnit:
      lengthValue *= 25.4f;
      break;
    case kCentimeterUnit:
      lengthValue *= 10.0f;
      break;
    case kMillimeterUnit:
      break;
  } // switch

  // compute the number of tenths
  float tenths = lengthValue / fMillimeters * fTenths;

  // compute result
  stringstream s;

  s << tenths;

  return s.str ();
}

string msr2mxmlTreeTranslator::S_msrLengthAsTenths (
  S_msrLength length)
{
  return msrLengthAsTenths (* length);
}

//______________________________________________________________________________
void msr2mxmlTreeTranslator::populatePageMargins (
  Sxmlelement       elem,
  S_msrMarginsGroup marginsGroup)
{
  // left margin
  S_msrMargin leftMargin = marginsGroup->getLeftMargin ();

  if (leftMargin) {
    // append a left margin element to the page margins element
    msrLength
      leftMarginLength =
        leftMargin->getMarginLength ();

    elem->push (
      createMxmlElement (
        k_left_margin,
        msrLengthAsTenths (leftMarginLength)));
  }

  // right margin
  S_msrMargin rightMargin = marginsGroup->getRightMargin ();

  if (rightMargin) {
    // append a right margin element to the page margins element
    msrLength
      rightMarginLength =
        rightMargin->getMarginLength ();

    elem->push (
      createMxmlElement (
        k_right_margin,
        msrLengthAsTenths (rightMarginLength)));
  }

  // top margin
  S_msrMargin topMargin = marginsGroup->getTopMargin ();

  if (topMargin) {
    // append a top margin element to the page margins element
    msrLength
      topMarginLength =
        topMargin->getMarginLength ();

    elem->push (
      createMxmlElement (
        k_top_margin,
        msrLengthAsTenths (topMarginLength)));
  }

  // bottom margin
  S_msrMargin bottomMargin = marginsGroup->getBottomMargin ();

  if (bottomMargin) {
    // append a bottom margin element to the page margins element
    msrLength
      bottomMarginLength =
        bottomMargin->getMarginLength ();

    elem->push (
      createMxmlElement (
        k_bottom_margin,
        msrLengthAsTenths (bottomMarginLength)));
  }
}

void msr2mxmlTreeTranslator::appendPageMarginsToScoreDefaultsPageLayout (
  S_msrMarginsGroup marginsGroup)
{
  // create a page margins element
  Sxmlelement
    pageMargins =
      createMxmlElement (k_page_margins, "");

  // set its "type" element
  pageMargins->add (
    createMxmlAttribute (
      "type",
      msrMarginTypeKindAsString (
        marginsGroup->getMarginsGroupTypeKind ())));

  // populate it
  populatePageMargins (
    pageMargins,
    marginsGroup);

  // append it to the defaults page layout element
  appendToScoreDefaultsPageLayout (
    pageMargins);
}

//______________________________________________________________________________
void msr2mxmlTreeTranslator::populateSystemMargins (
  Sxmlelement       elem,
  S_msrSystemLayout systemLayout)
{
  // left margin
  S_msrMargin leftMargin = systemLayout->getLeftMargin ();

  if (leftMargin) {
    // append a left margin element to the page margins element
    msrLength
      leftMarginLength =
        leftMargin->getMarginLength ();

    elem->push (
      createMxmlElement (
        k_left_margin,
        msrLengthAsTenths (leftMarginLength)));
  }

  // right margin
  S_msrMargin rightMargin = systemLayout->getRightMargin ();

  if (rightMargin) {
    // append a right margin element to the page margins element
    msrLength
      rightMarginLength =
        rightMargin->getMarginLength ();

    elem->push (
      createMxmlElement (
        k_right_margin,
        msrLengthAsTenths (rightMarginLength)));
  }
}

void msr2mxmlTreeTranslator::appendSystemMarginsToScoreDefaultsSystemLayout (
  S_msrSystemLayout systemLayout)
{
  // JMI
}

//______________________________________________________________________________
string msr2mxmlTreeTranslator::msrPlacementKindAsMusicXMLString (
  msrPlacementKind placementKind)
{
  string result;

  switch (placementKind) {
    case msrPlacementKind::k_NoPlacement:
      result = "";
      break;
    case msrPlacementKind::kPlacementAbove:
      result = "above";
      break;
    case msrPlacementKind::kPlacementBelow:
      result = "below";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
string msr2mxmlTreeTranslator::msrSpannerTypeKindAsMusicXMLString (
  msrSpannerTypeKind spannerTypeKind)
{
  string result;

  switch (spannerTypeKind) {
    case k_NoSpannerType:
      // should not occur
      break;
    case kSpannerTypeStart:
      result = "start";
      break;
    case kSpannerTypeStop:
      result = "stop";
      break;
    case kSpannerTypeContinue:
      result = "continue";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::appendToScoreWork (
  Sxmlelement elem)
{
  if (! fScoreWorkElement) {
    // create a work element
    fScoreWorkElement = createMxmlElement (k_work, "");
  }

  fScoreWorkElement->push (elem);
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::appendToScoreIdentification (
  Sxmlelement elem)
{
  if (! fScoreIdentificationElement) {
    // create an identification element
    fScoreIdentificationElement = createMxmlElement (k_identification, "");
  }

  fScoreIdentificationElement->push (elem);
}

void msr2mxmlTreeTranslator::appendToScoreIdentificationEncoding (
  Sxmlelement elem)
{
  if (! fScoreIdentificationEncodingElement) {
    // create an encoding element
    fScoreIdentificationEncodingElement = createMxmlElement (k_encoding, "");
  }

  fScoreIdentificationEncodingElement->push (elem);
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::appendToScoreDefaults (
  Sxmlelement elem)
{
  if (! fScoreDefaultsElement) {
    // create a defaults element
    fScoreDefaultsElement = createMxmlElement (k_defaults, "");
  }

  fScoreDefaultsElement->push (elem);
}

void msr2mxmlTreeTranslator::appendToScoreDefaultsPageLayout (
  Sxmlelement elem)
{
  if (! fScoreDefaultsPageLayoutElement) {
    // create a page layout element
    fScoreDefaultsPageLayoutElement = createMxmlElement (k_page_layout, "");
  }

  fScoreDefaultsPageLayoutElement->push (elem);
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::createMxmlAttributesElementAndAppendItToMeasure ()
{
  if (gGlobalMsr2mxmlTreeOahGroup->getMusicXMLComments ()) {
    // create an attributes comment
    stringstream s;
    s <<
      " ===== " <<
      "Attributes " <<
      " ===== ";
    Sxmlelement comment = createMxmlElement (kComment, s.str ());

    // append it to the current measure element
    // maybe not if it not in the first measure seen with the given number??? JMI
    fCurrentMeasureElement->push (comment);
  }

  // create the attributes element
  Sxmlelement
    attributesElement =
      createMxmlElement (k_attributes, "");

  if (fDivisionsElement) {
    // append divisions to the current measure attributes element
    attributesElement->push (fDivisionsElement);
    fDivisionsElement = nullptr;
  }

  if (fKeyElement) {
    // append key to the current measure attributes element
    attributesElement->push (fKeyElement);
    fKeyElement = nullptr;
  }

  if (fTimeElement) {
    // append time to the current measure attributes element
    attributesElement->push (fTimeElement);
    fTimeElement = nullptr;
  }

  if (fStavesElement) {
    // append it the current measure attributes element
    attributesElement->push (fStavesElement);
    fStavesElement = nullptr;
  }

  if (fClefElementsList.size ()) {
    // append the clef  elements if any to the current measure element
    for (
      list<Sxmlelement>::const_iterator i =
        fClefElementsList.begin ();
      i!=fClefElementsList.end ();
      ++i
    ) {
      Sxmlelement clefElement = (*i);

      attributesElement->push (clefElement);
    } // for

    // forget about those clef elements
    fClefElementsList.clear ();
  }

  // append the attributes element to the current measure element
  fCurrentMeasureElement->push (attributesElement);
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::appendToMeasureDirection (
  Sxmlelement      elem,
  msrPlacementKind placementKind)
{
  if (gGlobalMsr2mxmlTreeOahGroup->getMusicXMLComments ()) {
    // create a direction comment
    stringstream s;
    s <<
      " ===== " <<
      "Direction" <<
      " ===== ";
    Sxmlelement comment = createMxmlElement (kComment, s.str ());

    // append it to the current measure element
    fCurrentMeasureElement->push (comment);
  }

  // create a direction element
  Sxmlelement directionElement = createMxmlElement (k_direction, "");

  // set it's "placement" attribute if relevant
  string
    placementString =
      msrPlacementKindAsMusicXMLString (placementKind);

  if (placementString.size ()) {
    directionElement->add (createMxmlAttribute ("placement",  placementString));
  }

  // append the direction element to the current measure element
  fCurrentMeasureElement->push (directionElement);

  // create a direction type element
  Sxmlelement directionTypeElement = createMxmlElement (k_direction_type, "");

  // append it to the current direction element
  directionElement->push (directionTypeElement);

  // append elem to the direction type element
  directionTypeElement->push (elem);
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::appendNoteToMeasure (
  Sxmlelement note,
  S_msrNote   theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    note->getInputLineNumber ();

  if (gGlobalTraceOahGroup->getTraceNotes ()) {
    gLogStream <<
      "--> appendNoteToMeasure(), note = " <<
      ", note: " << mxmlElementAsString (note) <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // append the 'before spanner' elements if any
  appendNoteSpannersBeforeNote (theMsrNote);

  // append note to the current measure element
  fCurrentMeasureElement->push (note);

  // account for theMsrNote's whole notes duration in the measure
  fCurrentPositionInMeasure +=
    theMsrNote->
      getNoteSoundingWholeNotes ();

  // append the 'after spanner' elements if any
  appendNoteSpannersAfterNote (theMsrNote);
}

void msr2mxmlTreeTranslator::appendOtherToMeasure (
  Sxmlelement elem)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elem->getInputLineNumber ();

  if (gGlobalTraceOahGroup->getTraceNotes ()) {
    gLogStream <<
      "--> appendOtherToMeasure()" <<
      ", elem: " << mxmlElementAsString (elem) <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // append elem to the current measure element
  fCurrentMeasureElement->push (elem);
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::appendToNoteNotations (
  Sxmlelement      elem,
  msrPlacementKind placementKind)
{
  if (! fCurrentNoteNotationsElement) {
    // create an notations element
    fCurrentNoteNotationsElement = createMxmlElement (k_notations, "");

    // append it to fCurrentNoteElement
    fCurrentNoteElement->push (fCurrentNoteNotationsElement);
  }

  // set elem's "placement" attribute if relevant
  string
    placementString =
      msrPlacementKindAsMusicXMLString (placementKind);

  if (placementString.size ()) {
    elem->add (createMxmlAttribute ("placement", placementString));
  }

  // append elem to the note notations element
  fCurrentNoteNotationsElement->push (elem);
}

void msr2mxmlTreeTranslator::appendToNoteNotationsOrnaments (
  Sxmlelement      elem,
  msrPlacementKind placementKind)
{
  if (! fCurrentNoteNotationsOrnamentsElement) {
    // create an notations element
    fCurrentNoteNotationsOrnamentsElement = createMxmlElement (k_ornaments, "");

    // append it to fCurrentNoteNotationsElement
    appendToNoteNotations (
      fCurrentNoteNotationsOrnamentsElement,
      k_NoPlacement); // no placement for '<ornaments/>', only <trill-mark> has JMI ???
  }

  // set elem's "placement" attribute if relevant
  string
    placementString =
      msrPlacementKindAsMusicXMLString (placementKind);

  if (placementString.size ()) {
    elem->add (createMxmlAttribute ("placement", placementString));
  }

  // append elem to the note notations ornaments element
  fCurrentNoteNotationsOrnamentsElement->push (elem);
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::appendToNoteNotationsArticulations (
  Sxmlelement      elem,
  msrPlacementKind placementKind)
{
  if (! fCurrentNoteNotationsArticulationsElement) {
    // create the note notations articulations element
    fCurrentNoteNotationsArticulationsElement = createMxmlElement (k_articulations, "");

    // append it to fCurrentNoteNotationsArticulationsElement
    appendToNoteNotations (
      fCurrentNoteNotationsArticulationsElement,
      placementKind);
  }

  // set elem's "placement" attribute if relevant
  string
    placementString =
      msrPlacementKindAsMusicXMLString (placementKind);

  if (placementString.size ()) {
    elem->add (createMxmlAttribute ("placement", placementString));
  }

  // append elem to the note notations articulations element
  fCurrentNoteNotationsArticulationsElement->push (elem);
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::appendToNoteNotationsTechnicals (
  Sxmlelement      elem,
  msrPlacementKind placementKind)
{
  if (! fCurrentNoteNotationsTechnicalsElement) {
    // create an notations element
    fCurrentNoteNotationsTechnicalsElement = createMxmlElement (k_technical, "");

    // append it to fCurrentNoteNotationsElement
    appendToNoteNotations (
      fCurrentNoteNotationsTechnicalsElement,
      placementKind);
  }

  // set elem's "placement" attribute if relevant
  string
    placementString =
      msrPlacementKindAsMusicXMLString (placementKind);

  if (placementString.size ()) {
    elem->add (createMxmlAttribute ("placement", placementString));
  }

  // append elem to the note notations technicals element
  fCurrentNoteNotationsTechnicalsElement->push (elem);
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrScore& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrScore" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create the initial creation comment
  stringstream s;
  s <<
    " ===== " <<
    "Created by " <<
    gGlobalOahOahGroup->getHandlerExecutableName () <<
    " " <<
    currentVersionNumber () <<
    " on " <<
    gGlobalGeneralOahGroup->getTranslationDateFull () <<
    " from " <<
    gGlobalOahOahGroup->getInputSourceName () <<
    " ===== ";

  // append the initial creation comment to the score part wise element
  fResultingMusicxmlElement->push (createMxmlElement (kComment, s.str ()));

  // create a software element
  Sxmlelement
    softwareElement =
      createMxmlElement (
        k_software,
        gGlobalOahOahGroup->getHandlerExecutableName ()
          + " "
          + currentVersionNumber () +
          ", https://github.com/grame-cncm/libmusicxml/tree/lilypond");

  // append it to the identification encoding
  appendToScoreIdentificationEncoding (softwareElement);

  // create an encoding date element
  Sxmlelement
    encodingDateElement =
      createMxmlElement (
        k_encoding_date,
        gGlobalGeneralOahGroup->getTranslationDateYYYYMMDD ());

  // append it to the identification encoding
  appendToScoreIdentificationEncoding (encodingDateElement);

  // create the part list element
  fScorePartListElement = createMxmlElement (k_part_list, "");
}

void msr2mxmlTreeTranslator::visitEnd (S_msrScore& elt)
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

  // append the score work element if any to the score part wise element
  if (fScoreWorkElement) {
    fResultingMusicxmlElement->push (fScoreWorkElement);
  }

  // append the score movement number element if any to the score part wise element
  if (fScoreMovementNumberElement) {
    fResultingMusicxmlElement->push (fScoreMovementNumberElement);
  }

  // append the score movement title element if any to the score part wise element
  if (fScoreMovementTitleElement) {
    fResultingMusicxmlElement->push (fScoreMovementTitleElement);
  }

  // append the score identification element if any to the score part wise element
  if (fScoreIdentificationElement) {
    if (gGlobalMsr2mxmlTreeOahGroup->getMusicXMLComments ()) {
      // create an identification comment
      stringstream s;
      s <<
        " ===== " <<
        "Identification" <<
        " ===== ";
      Sxmlelement comment = createMxmlElement (kComment, s.str ());

      // append it to the score partwise element
      fResultingMusicxmlElement->push (comment);
    }

    fResultingMusicxmlElement->push (fScoreIdentificationElement);
  }

  if (fScoreDefaultsScalingElement) {
    // append the scaling element to the score defaults element
    appendToScoreDefaults (fScoreDefaultsScalingElement);
  }

  if (fScoreDefaultsPageLayoutElement) {
    // append the page layout element to the score defaults element
    appendToScoreDefaults (fScoreDefaultsPageLayoutElement);
  }

  if (fScoreDefaultsSystemLayoutElement) {
    // append the system layout element to the score defaults element
    appendToScoreDefaults (fScoreDefaultsSystemLayoutElement);
  }

  // append the staff layout elements if any to the score defaults element
  for (
    list<Sxmlelement>::const_iterator i =
      fScoreDefaultsStaffLayoutElementsList.begin ();
    i!=fScoreDefaultsStaffLayoutElementsList.end ();
    ++i
  ) {
    Sxmlelement staffLayoutElement = (*i);

    appendToScoreDefaults (staffLayoutElement);
  } // for

  if (fScoreDefaultsAppearanceElement) {
    // append the appearance element to the score defaults element
    appendToScoreDefaults (fScoreDefaultsAppearanceElement);
  }

  // append the score defaults element if any to the score part wise element
  if (fScoreDefaultsElement) {
    if (gGlobalMsr2mxmlTreeOahGroup->getMusicXMLComments ()) {
      // create an defaults comment
      stringstream s;
      s <<
        " ===== " <<
        "Defaults" <<
        " ===== ";
      Sxmlelement comment = createMxmlElement (kComment, s.str ());

      // append it to the score partwise element
      fResultingMusicxmlElement->push (comment);
    }

    fResultingMusicxmlElement->push (fScoreDefaultsElement);
  }

  // append the score credits element if any to the score part wise element
  unsigned int pendingScoreCreditElementsListSize =
    fPendingScoreCreditElementsList.size ();

  if (pendingScoreCreditElementsListSize) {
    if (gGlobalMsr2mxmlTreeOahGroup->getMusicXMLComments ()) {
      stringstream s;
      s <<
        " ===== " <<
        singularOrPlural (
          pendingScoreCreditElementsListSize, "Credit", "Credits") <<
        " ===== ";
      Sxmlelement comment = createMxmlElement (kComment, s.str ());

      // append it to the score partwise element
      fResultingMusicxmlElement->push (comment);
    }

    for (
      list<Sxmlelement>::const_iterator i =
        fPendingScoreCreditElementsList.begin ();
      i!=fPendingScoreCreditElementsList.end ();
      ++i
    ) {
      Sxmlelement creditElement = (*i);

      fResultingMusicxmlElement->push (creditElement);
    } // for
  }

  if (gGlobalMsr2mxmlTreeOahGroup->getMusicXMLComments ()) {
    // create an part-list comment
    stringstream s;
    s <<
      " ===== " <<
      "PART-LIST" <<
      " ===== ";
    Sxmlelement comment = createMxmlElement (kComment, s.str ());

    // append it to the score partwise element
    fResultingMusicxmlElement->push (comment);
  }

  // append the part list element to the score part wise element
  fResultingMusicxmlElement->push (fScorePartListElement);

  // append the pending parts elements to the score part wise element
  for (
    list<Sxmlelement>::const_iterator i =
      fPendingPartElementsList.begin ();
    i!=fPendingPartElementsList.end ();
    ++i
  ) {
    Sxmlelement partElement = (*i);

    if (gGlobalMsr2mxmlTreeOahGroup->getMusicXMLComments ()) {
      // create a part comment
      stringstream s;
      s <<
        " ============================ " <<
        "PART" <<
        " \"" << partElement->getAttributeValue ("id") << "\"" <<
        ", line " << inputLineNumber <<
        " ============================= ";
      Sxmlelement partComment = createMxmlElement (kComment, s.str ());

      // append it to the score part wise element
      fResultingMusicxmlElement->push (partComment);
    }

    // append the part element to the score part wise element
    fResultingMusicxmlElement->push (partElement);
  } // for
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrIdentification& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrIdentification" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // work number
  string
    workNumber =
      elt->getWorkNumber ();

  // create the work number element
  Sxmlelement
    workNumberElement =
      createMxmlElement (
        k_work_number,
        workNumber);

  // append it to the score part wise element
  appendToScoreWork (workNumberElement);

  // work title
  string
    workTitle =
      elt->getWorkTitle ();

  // create the work title element
  Sxmlelement
    workTitleElement =
      createMxmlElement (
        k_work_title,
        workTitle);

  // append it to the score work element
  appendToScoreWork (workTitleElement);

  // opus
  string
    opus =
      elt->getOpus ();

/* JMI <opus/> NOT YET USED ANYWAY?

https://forums.makemusic.com/viewtopic.php?f=12&t=2017&p=9869&hilit=opus&sid=fd2554e5ca94ddd842f10d40ae429772#p9869

Michael Good wrote:
Hi Mario,

I don't know if any distributed software is currently supporting the opus. However, we have used it for some internal programs here. Here is a short example from Schumann's Frauenliebe und Leben cycle. The opus file collects the 8 MusicXML score-partwise files for each individual song into a single opus:

<?xml version="1.0" standalone="no"?>
<!DOCTYPE opus PUBLIC "-//Recordare//DTD MusicXML 2.0 Opus//EN"
"http://www.musicxml.org/dtds/opus.dtd">
<opus version="2.0">
<title>Frauenliebe und Leben, Op. 42</title>
<score xlink:href="Frauenliebe1.xml"/>
<score xlink:href="Frauenliebe2.xml"/>
<score xlink:href="Frauenliebe3.xml"/>
<score xlink:href="Frauenliebe4.xml"/>
<score xlink:href="Frauenliebe5.xml"/>
<score xlink:href="Frauenliebe6.xml"/>
<score xlink:href="Frauenliebe7.xml"/>
<score xlink:href="Frauenliebe8.xml"/>
</opus>
...

  // create the opus element
  Sxmlelement
    opusElement =
      createMxmlElement (
        k_opus,
        opus);

    // append it to the score part wise element
    appendToScoreWork (opusElement);
*/

  // movement number
  string
    movementNumber = elt->getMovementNumber ();

  // create the movement number element
  fScoreMovementNumberElement =
    createMxmlElement (
      k_movement_number,
      movementNumber);

  // don't append it at once to the score defaults element

  // movement title
  string
    movementTitle = elt->getMovementTitle ();

  // create the movement title element
  fScoreMovementTitleElement =
    createMxmlElement (
      k_movement_title,
      movementTitle);

  // don't append it at once to the score defaults element

  // miscellaneous field
  string
    miscellaneousField =
      elt->getMiscellaneousField ();

  // create the miscellaneous field element
  Sxmlelement
    miscellaneousFieldElement =
      createMxmlElement (
        k_miscellaneous_field,
        miscellaneousField);

  // set its "name" attribute
  miscellaneousFieldElement->add (
    createMxmlAttribute ("name", "description")); // ??? JMI sometines "comment"

  // create a miscellaneous element
  fIdentificationMiscellaneousElement = createMxmlElement (k_miscellaneous, "");

  // append the miscellaneous field element to it
  fIdentificationMiscellaneousElement->push (miscellaneousFieldElement);

  // score instrument
  string
    scoreInstrument =
      elt->getScoreInstrument ();

  // create the score instrument element,
  // it will be appended to the score part element
  // when the latter is created
  fScoreInstrumentElement =
    createMxmlElement (
      k_score_instrument,
      "");
  // set it's "id" attribute later, when the partID is known!
  // create an instrument name element
  Sxmlelement
    scoreInstrumentNameElement =
      createMxmlElement (
        k_instrument_name,
        scoreInstrument);
  // append it to the score instrument element
  fScoreInstrumentElement->push (scoreInstrumentNameElement);

  // composers
  const list<string>&
    composersList =
      elt->getComposersList ();

  for (
    list<string>::const_iterator i=composersList.begin ();
    i!=composersList.end ();
    ++i
  ) {
    string variableValue = (*i);

    // create a creator element
    Sxmlelement creatorElement = createMxmlElement (k_creator, variableValue);

    // set its "type" attribute
    creatorElement->add (createMxmlAttribute ("type", "composer"));

    // append it to the composers elements list
    fComposersElementsList.push_back (creatorElement);
  } // for

  // arrangers
  const list<string>&
    arrangersList =
      elt->getArrangersList ();

  for (
    list<string>::const_iterator i=arrangersList.begin ();
    i!=arrangersList.end ();
    ++i
  ) {
    string variableValue = (*i);

    // create a creator element
    Sxmlelement creatorElement = createMxmlElement (k_creator, variableValue);

    // set its "type" attribute
    creatorElement->add (createMxmlAttribute ("type", "arranger"));

    // append it to the composers elements list
    fComposersElementsList.push_back (creatorElement);
  } // for

  // lyricists
  const list<string>&
    lyricistsList =
      elt->getLyricistsList ();

  for (
    list<string>::const_iterator i=lyricistsList.begin ();
    i!=lyricistsList.end ();
    ++i
  ) {
    string variableValue = (*i);

    // append a creator element
    Sxmlelement creatorElement = createMxmlElement (k_creator, variableValue);

    // set its "type" attribute
    creatorElement->add (createMxmlAttribute ("type", "lyricist"));

    // append it to the composers elements list
    fComposersElementsList.push_back (creatorElement);
  } // for

  // poets
  const list<string>&
    poetsList =
      elt->getPoetsList ();

  for (
    list<string>::const_iterator i=poetsList.begin ();
    i!=poetsList.end ();
    ++i
  ) {
    string variableValue = (*i);

    // append a creator element
    Sxmlelement creatorElement = createMxmlElement (k_creator, variableValue);

    // set its "type" attribute
    creatorElement->add (createMxmlAttribute ("type", "poet"));

    // append it to the composers elements list
    fComposersElementsList.push_back (creatorElement);
  } // for

  // translators
  const list<string>&
    translatorsList =
      elt->getTranslatorsList ();

  for (
    list<string>::const_iterator i=translatorsList.begin ();
    i!=translatorsList.end ();
    ++i
  ) {
    string variableValue = (*i);

    // append a creator element
    Sxmlelement creatorElement = createMxmlElement (k_creator, variableValue);

    // set its "type" attribute
    creatorElement->add (createMxmlAttribute ("type", "translator"));

    // append it to the composers elements list
    fComposersElementsList.push_back (creatorElement);
  } // for

  // artists
  const list<string>&
    artistsList =
      elt->getArtistsList ();

  for (
    list<string>::const_iterator i=artistsList.begin ();
    i!=artistsList.end ();
    ++i
  ) {
    string variableValue = (*i);

    // append a creator element
    Sxmlelement creatorElement = createMxmlElement (k_creator, variableValue);

    // set its "type" attribute
    creatorElement->add (createMxmlAttribute ("type", "artist"));

    // append it to the composers elements list
    fComposersElementsList.push_back (creatorElement);
  } // for

  // softwares
  const list<string>&
    softwaresList =
      elt->getSoftwaresList ();

  for (
    list<string>::const_iterator i=softwaresList.begin ();
    i!=softwaresList.end ();
    ++i
  ) {
    string variableValue = (*i);

    // append a software element to the softwares elements list
    fSoftwaresElementsList.push_back (
      createMxmlElement (
      k_software,
      variableValue));
  } // for

  // rights
  const list<string>&
    rightsList =
      elt->getRightsList ();

  for (
    list<string>::const_iterator i=rightsList.begin ();
    i!=rightsList.end ();
    ++i
  ) {
    string variableValue = (*i);

    // append a rights element to the rights elements list
    fRightsElementsList.push_back (
      createMxmlElement (
      k_rights,
      variableValue));
  } // for
}

void msr2mxmlTreeTranslator::visitEnd (S_msrIdentification& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrIdentification" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fRightsElementsList.size ()) {
    // append the rights elements to the score identification element
    list<Sxmlelement>::const_iterator i;

    for (
      i=fRightsElementsList.begin ();
      i!=fRightsElementsList.end ();
      ++i) {
      Sxmlelement rightsElement = (*i);

      appendToScoreIdentification (rightsElement);
    } // for
  }

  if (fScoreIdentificationEncodingElement) {
    if (fSoftwaresElementsList.size ()) {
      // append the software elements to the score identification  encoding element
      for (
        list<Sxmlelement>::const_iterator i = fSoftwaresElementsList.begin ();
        i!=fSoftwaresElementsList.end ();
        ++i
      ) {
        Sxmlelement softwareElement = (*i);

        appendToScoreIdentificationEncoding (softwareElement);
      } // for
    }

    // append the score identification element to the score identification
    appendToScoreIdentification (fScoreIdentificationEncodingElement);
  }

  if (fIdentificationMiscellaneousElement) {
    // append the miscellaneous element to the score identification element
    appendToScoreIdentification (fIdentificationMiscellaneousElement);
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrScaling& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrScaling" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  ++gIndenter;

  // get the values
  fMillimeters = elt->getMillimeters ();
  fTenths      = elt->getTenths ();

  if (fMillimeters > 0) {
    // create a scaling element
    fScoreDefaultsScalingElement = createMxmlElement (k_scaling, "");

    // append a millimeters sub-element to it
    {
      stringstream s;

      s << fMillimeters;

      fScoreDefaultsScalingElement->push (
        createMxmlElement (
          k_millimeters,
          s.str ()));
    }

    // append a tenths sub-element to it
    {
      stringstream s;

      s << fTenths;

      fScoreDefaultsScalingElement->push (
        createMxmlElement (
          k_tenths,
          s.str ()));
    }
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrScaling& elt)
{
  --gIndenter;

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
void msr2mxmlTreeTranslator::visitStart (S_msrPageLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPageLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // page size
  S_msrLength pageHeight = elt->getPageHeight ();

  if (pageHeight) {
    // append a page height element to the defaults page layout element
    appendToScoreDefaultsPageLayout (
      createMxmlElement (
        k_page_height,
        S_msrLengthAsTenths (pageHeight)));
  }

  S_msrLength pageWidth = elt->getPageWidth ();

  if (pageWidth) {
    // append a page width element to the defaults page layout element
    appendToScoreDefaultsPageLayout (
      createMxmlElement (
        k_page_width,
        S_msrLengthAsTenths (pageWidth)));
  }

  // margins
  S_msrMarginsGroup oddMarginsGroup = elt->getOddMarginsGroup ();

  if (oddMarginsGroup) {
    // append an odd page margins element to the defaults page layout element
    appendPageMarginsToScoreDefaultsPageLayout (
      oddMarginsGroup);
  }

  S_msrMarginsGroup evenMarginsGroup = elt->getEvenMarginsGroup ();

  if (evenMarginsGroup) {
    // append an even page margins element to the defaults page layout element
    appendPageMarginsToScoreDefaultsPageLayout (
      evenMarginsGroup);
  }

  S_msrMarginsGroup bothMarginsGroup = elt->getBothMarginsGroup ();

  if (bothMarginsGroup) {
    // append a both page margins element to the defaults page layout element
    appendPageMarginsToScoreDefaultsPageLayout (
      bothMarginsGroup);
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrPageLayout& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrSystemLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSystemLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create a system layout element
  Sxmlelement
    systemLayoutElement =
      createMxmlElement (k_system_layout, "");

  if (fOnGoingPrintLayout) {
    // append it to the current print element
    fCurrentPrintElement->push (
      systemLayoutElement);
  }
  else {
    // don't append it at once to the score defaults element
    fScoreDefaultsSystemLayoutElement = systemLayoutElement;
  }

  // create a system margins element
  Sxmlelement
    systemMargins =
      createMxmlElement (k_system_margins, "");

  // populate it
  populateSystemMargins (
    systemMargins,
    elt);

  // append it to the system layout element
  systemLayoutElement->push (
    systemMargins);

  // distances
  S_msrLength systemDistance = elt->getSystemDistance ();

  if (systemDistance) {
    // create a system distance element
    Sxmlelement
      systemDistanceElement =
        createMxmlElement (
          k_system_distance,
          S_msrLengthAsTenths (systemDistance));

  // append it to the system layout element
  systemLayoutElement->push (
    systemDistanceElement);
  }

  S_msrLength topSystemDistance = elt->getTopSystemDistance ();

  if (topSystemDistance) {
    // create a top system distance element
    Sxmlelement
      topSystemDistanceElement =
        createMxmlElement (
          k_top_system_distance,
          S_msrLengthAsTenths (topSystemDistance));

  // append it to the system layout element
  systemLayoutElement->push (
    topSystemDistanceElement);
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrSystemLayout& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrStaffLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStaffLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // number
  int staffNumber = elt->getStaffNumber ();

  // create a staff layout element
  Sxmlelement
    staffLayoutElement =
      createMxmlElement (k_staff_layout, "");

  // set its "number" attribute if relevant
  // (it is 0 inside the <defaults /> element)
  if (staffNumber > 0) {
    staffLayoutElement->add (createMxmlIntegerAttribute ("number", staffNumber));
  }

  // distance
  S_msrLength staffDistance = elt->getStaffDistance ();

  if (staffDistance) {
    // create a staff distance element
    Sxmlelement
      staffDistanceElement =
        createMxmlElement (
          k_staff_distance,
          S_msrLengthAsTenths (staffDistance));

    // append it to the staff layout element
    staffLayoutElement->push (
      staffDistanceElement);
  }

  if (fOnGoingPrintLayout) {
    // append it to the current print element
    fCurrentPrintElement->push (
      staffLayoutElement);
  }
  else {
    // don't append it at once to the score defaults element
    fScoreDefaultsStaffLayoutElementsList.push_back (staffLayoutElement);
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrStaffLayout& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrMeasureLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMeasureLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingPrintLayout) {
    // create a measure layout element
    Sxmlelement
      measureLayoutElement =
        createMxmlElement (k_measure_layout, "");

    // append it to the current print element
    fCurrentPrintElement->push (
      measureLayoutElement);

    // distance
    S_msrLength measureDistance = elt->getMeasureDistance ();

    if (measureDistance) {
      // create a measure distance element
      Sxmlelement
        measureDistanceElement =
          createMxmlElement (
            k_measure_distance,
            S_msrLengthAsTenths (measureDistance));

      // append it to the measure layout element
      measureLayoutElement->push (
        measureDistanceElement);
    }
  }

  else {
    // JMI
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrMeasureLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMeasureLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::populateAppearanceLineWidths (
  Sxmlelement                 appearanceElement,
  const list<S_msrLineWidth>& lineWidthsList)
{
  for (
    list<S_msrLineWidth>::const_iterator i =
      lineWidthsList.begin ();
    i!=lineWidthsList.end ();
    ++i
  ) {
    S_msrLineWidth lineWidth = (*i);

    // get line width type
    msrLineWidth::msrLineWidthTypeKind
      lineWidthTypeKind =
        lineWidth->getLineWidthTypeKind ();

    // get line width value
    S_msrLength
      lineWidthValue =
        lineWidth->getLineWidthValue ();

    // create a line width element
    Sxmlelement
      lineWidthElement =
        createMxmlElement (
          k_line_width,
          S_msrLengthAsTenths (lineWidthValue));

    // set its "type" attribute
    string lineWidthTypeString;

    switch (lineWidthTypeKind) {
      case msrLineWidth::k_NoLineWidthTypeKind:
        lineWidthTypeString = "k_NoLineWidthTypeKind";
        break;
      case msrLineWidth::kBeamLineWidth:
        lineWidthTypeString = "beam";
        break;
      case msrLineWidth::kBracketLineWidth:
        lineWidthTypeString = "bracket";
        break;
      case msrLineWidth::kDashesLineWidth:
        lineWidthTypeString = "dashes";
        break;
      case msrLineWidth::kEnclosureLineWidth:
        lineWidthTypeString = "enclosure";
        break;
      case msrLineWidth::kEndingLineWidth:
        lineWidthTypeString = "ending";
        break;
      case msrLineWidth::kExtendLineWidth:
        lineWidthTypeString = "extend";
        break;
      case msrLineWidth::kHeavyBarLineLineWidth:
        lineWidthTypeString = "heavy barline";
        break;
      case msrLineWidth::kLegerLineWidth:
        lineWidthTypeString = "leger";
        break;
      case msrLineWidth::kLightBarLineLineWidthLineWidth:
        lineWidthTypeString = "light barline";
        break;
      case msrLineWidth::kOctaveShiftLineWidth:
        lineWidthTypeString = "octave shift";
        break;
      case msrLineWidth::kPedalLineWidth:
        lineWidthTypeString = "pedal";
        break;
      case msrLineWidth::kSlurMiddleLineWidth:
        lineWidthTypeString = "slur middle";
        break;
      case msrLineWidth::kSlurTipLineWidth:
        lineWidthTypeString = "slur tip";
        break;
      case msrLineWidth::kStaffLineWidth:
        lineWidthTypeString = "staff";
        break;
      case msrLineWidth::kStemLineWidthLineWidth: // better name ???
        lineWidthTypeString = "stem";
        break;
      case msrLineWidth::kTieMiddleLineWidth:
        lineWidthTypeString = "tie middle";
        break;
      case msrLineWidth::kTieTipLineWidth:
        lineWidthTypeString = "tie tip";
        break;
      case msrLineWidth::kTupletBracketLineWidth:
        lineWidthTypeString = "tuplet bracket";
        break;
      case msrLineWidth::kWedgeLineWidth:
        lineWidthTypeString = "wedge";
        break;
    } // switch

    lineWidthElement->add (createMxmlAttribute ("type", lineWidthTypeString));

    // append the line width element to the appearance element
    appearanceElement->push (
      lineWidthElement);
  } // for
}

void msr2mxmlTreeTranslator::populateAppearanceNoteSizes (
  Sxmlelement                appearanceElement,
  const list<S_msrNoteSize>& noteSizesList)
{
  for (
    list<S_msrNoteSize>::const_iterator i =
      noteSizesList.begin ();
    i!=noteSizesList.end ();
    ++i
  ) {
    S_msrNoteSize noteSize = (*i);

    // get note size type
    msrNoteSize::msrNoteSizeTypeKind
      noteSizeTypeKind =
        noteSize->getNoteSizeTypeKind ();

    // get note size value
    float
      noteSizeValue =
        noteSize->getNoteSizeValue ();

    // create a note size element
    Sxmlelement
      noteSizeElement =
        createMxmlFloatElement (
          k_note_size,
          noteSizeValue);

    // set its "type" attribute
    string noteSizeTypeString;

    switch (noteSizeTypeKind) {
      case msrNoteSize::k_NoNoteSizeTypeKind:
        noteSizeTypeString = "k_NoNoteSizeTypeKind";
        break;
      case msrNoteSize::kCueNoteSize:
        noteSizeTypeString = "cue";
        break;
      case msrNoteSize::kGraceNoteSize:
        noteSizeTypeString = "grace";
        break;
      case msrNoteSize::kLargeNoteSize:
        noteSizeTypeString = "large";
        break;
    } // switch

    noteSizeElement->add (createMxmlAttribute ("type", noteSizeTypeString));

    // append the note size element to the appearance element
    appearanceElement->push (
      noteSizeElement);
  } // for
}

void msr2mxmlTreeTranslator::populateAppearanceDistances (
  Sxmlelement                appearanceElement,
  const list<S_msrDistance>& distancesList)
{
  for (
    list<S_msrDistance>::const_iterator i =
      distancesList.begin ();
    i!=distancesList.end ();
    ++i
  ) {
    S_msrDistance distance = (*i);

    // get line width type
    msrDistance::msrDistanceTypeKind
      distanceTypeKind =
        distance->getDistanceTypeKind ();

    // get line width value
    S_msrLength
      distanceValue =
        distance->getDistanceValue ();

    // create a line width element
    Sxmlelement
      distanceElement =
        createMxmlElement (
          k_distance,
          S_msrLengthAsTenths (distanceValue));

    // set its "type" attribute
    string distanceTypeString;

    switch (distanceTypeKind) {
      case msrDistance::k_NoDistanceTypeKind:
        distanceTypeString = "k_NoDistanceTypeKind";
        break;
      case msrDistance::kHyphenDistance:
        distanceTypeString = "hyphen";
        break;
      case msrDistance::kBeamDistance:
        distanceTypeString = "beam";
        break;
    } // switch

    distanceElement->add (createMxmlAttribute ("type", distanceTypeString));

    // append the line width element to the appearance element
    appearanceElement->push (
      distanceElement);
  } // for
}

void msr2mxmlTreeTranslator::populateAppearanceGlyphs (
  Sxmlelement             appearanceElement,
  const list<S_msrGlyph>& glyphsList)
{
  for (
    list<S_msrGlyph>::const_iterator i =
      glyphsList.begin ();
    i!=glyphsList.end ();
    ++i
  ) {
    S_msrGlyph glyph = (*i);

    // get line width type
    msrGlyph::msrGlyphTypeKind
      glyphTypeKind =
        glyph->getGlyphTypeKind ();

    // get line width value
    string
      glyphValue =
        glyph->getGlyphValue ();

    // create a line width element
    Sxmlelement
      glyphElement =
        createMxmlElement (
          k_glyph,
          glyphValue);

    // set its "type" attribute
    string glyphTypeString;

    switch (glyphTypeKind) {
      case msrGlyph::k_NoGlyphTypeKind:
        glyphTypeString = "k_NoGlyphTypeKind";
        break;
      case msrGlyph::kQuarterRestGlyph:
        glyphTypeString = "quarter-rest";
        break;
      case msrGlyph::kGClefOttavaBassaGlyph:
        glyphTypeString = "g-clef-ottava-bassa";
        break;
      case msrGlyph::kCClefGlyph:
        glyphTypeString = "c-clef";
        break;
      case msrGlyph::kFClefGlyph:
        glyphTypeString = "f-clef";
        break;
      case msrGlyph::kPercussionClefGlyph:
        glyphTypeString = "percussion-clef";
        break;
      case msrGlyph::kOctaveShiftUp8Glyph:
        glyphTypeString = "octave-shift-up-8";
        break;
      case msrGlyph::kOctaveShiftDown8Glyph:
        glyphTypeString = "octave-shift-down-8";
        break;
      case msrGlyph::kOctaveShiftContinue8Glyph:
        glyphTypeString = "octave-shift-continue-8";
        break;
      case msrGlyph::kOctaveShiftDown15Glyph:
        glyphTypeString = "octave-shift-down-15";
        break;
      case msrGlyph::kOctaveShiftUp15Glyph:
        glyphTypeString = "octave-shift-up-15";
        break;
      case msrGlyph::kOctaveShiftContinue15Glyph:
        glyphTypeString = "octave-shift-continue-15";
        break;
      case msrGlyph::kOctaveShiftDown22Glyph:
        glyphTypeString = "octave-shift-down-22";
        break;
      case msrGlyph::kOctaveShiftUp22Glyph:
        glyphTypeString = "octave-shift-up-22";
        break;
      case msrGlyph::kOctaveShiftContinue22Glyph:
        glyphTypeString = "octave-shift-continue-22";
        break;
    } // switch

    glyphElement->add (createMxmlAttribute ("type", glyphTypeString));

    // append the line width element to the appearance element
    appearanceElement->push (
      glyphElement);
  } // for
}

void msr2mxmlTreeTranslator::populateAppearanceOtherAppearances (
  Sxmlelement       appearanceElement,
  const list<S_msrOtherAppearance>&
                    otherAppearancesList)
{
  for (
    list<S_msrOtherAppearance>::const_iterator i =
      otherAppearancesList.begin ();
    i!=otherAppearancesList.end ();
    ++i
  ) {
    S_msrOtherAppearance otherAppearance = (*i);

    // get line width type
    msrOtherAppearance::msrOtherAppearanceTypeKind
      otherAppearanceTypeKind =
        otherAppearance->getOtherAppearanceTypeKind ();

    // get line width value
    string
      otherAppearanceValue =
        otherAppearance->getOtherAppearanceValue ();

    // create a line width element
    Sxmlelement
      otherAppearanceElement =
        createMxmlElement (
          k_other_appearance,
          otherAppearanceValue);

    // set its "type" attribute
    string otherAppearanceTypeString;

    switch (otherAppearanceTypeKind) {
      case msrOtherAppearance::k_NoOtherAppearanceTypeKind:
        otherAppearanceTypeString = "k_NoOtherAppearanceTypeKind";
        break;
    } // switch

    otherAppearanceElement->add (createMxmlAttribute (
      "type",
      otherAppearanceTypeString));

    // append the line width element to the appearance element
    appearanceElement->push (
      otherAppearanceElement);
  } // for
}

void msr2mxmlTreeTranslator::visitStart (S_msrAppearance& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrAppearance" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create an appearance element
  fScoreDefaultsAppearanceElement =
    createMxmlElement (k_appearance, "");

  // don't append it to the score defaults element yet,
  // this will be done in visitEnd (S_msrScore&)

  // append the line width elements if any to the appearance element
  const list<S_msrLineWidth>&
    lineWidthsList =
      elt->getLineWidthsList ();

  populateAppearanceLineWidths (
    fScoreDefaultsAppearanceElement,
    lineWidthsList);

  // append the note size elements if any to the appearance element
  const list<S_msrNoteSize>&
    noteSizesList =
      elt->getNoteSizesList ();

  populateAppearanceNoteSizes (
    fScoreDefaultsAppearanceElement,
    noteSizesList);

  // append the distance elements if any to the appearance element
  const list<S_msrDistance>&
    distancesList =
      elt->getDistancesList ();

  populateAppearanceDistances (
    fScoreDefaultsAppearanceElement,
    distancesList);

  // append the glyph elements if any to the appearance element
  const list<S_msrGlyph>&
    glyphsList =
      elt->getGlyphsList ();

  populateAppearanceGlyphs (
    fScoreDefaultsAppearanceElement,
    glyphsList);

  // append the other appearance elements if any to the appearance element
  const list<S_msrOtherAppearance>&
    otherAppearancesList =
      elt->getOtherAppearancesList ();

  populateAppearanceOtherAppearances (
    fScoreDefaultsAppearanceElement,
    otherAppearancesList);
}

void msr2mxmlTreeTranslator::visitEnd (S_msrAppearance& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrCredit& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrCredit" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create a credit element
  fCurrentScoreCreditElement = createMxmlElement (k_credit, "");

  // set its "page" attribute
  fCurrentScoreCreditElement->add (
    createMxmlIntegerAttribute ("page", elt->getCreditPageNumber ()));

  // append the credit element to the credit elements pending list
  fPendingScoreCreditElementsList.push_back (fCurrentScoreCreditElement);
}

void msr2mxmlTreeTranslator::visitEnd (S_msrCredit& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrCredit" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // forget about the current credit element
  fCurrentScoreCreditElement = nullptr;
}

void msr2mxmlTreeTranslator::visitStart (S_msrCreditWords& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrCreditWords" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create a credit words element
  Sxmlelement
    creditWordsElement =
      createMxmlElement (
        k_credit_words,
        elt->getCreditWordsContents ());

  // set its "default-x" attribute
  float
    creditWordsDefaultX =
      elt->getCreditWordsDefaultX ();
  if (creditWordsDefaultX > 0.0) {
    stringstream s;
    s << setprecision (8) << creditWordsDefaultX;
    creditWordsElement->add (createMxmlAttribute ("default-x", s.str ()));
  }

  // set its "default-y" attribute
  float
    creditWordsDefaultY =
      elt->getCreditWordsDefaultY ();
  if (creditWordsDefaultY > 0.0) {
    stringstream s;
    s << setprecision (8) << creditWordsDefaultY;
    creditWordsElement->add (createMxmlAttribute ("default-y", s.str ()));
  }

  // set its "font-family" attribute
  string
    creditWordsFontFamilyString =
      elt->getCreditWordsFontFamily ();

  if (creditWordsFontFamilyString.size ()) {
    creditWordsElement->add (
      createMxmlAttribute (
        "font-family",
        creditWordsFontFamilyString));
  }

  // set its "font-size" attribute
  float
    creditWordsFontSize =
      elt->getCreditWordsFontSize ();
  stringstream s;
  s << setprecision (2) << creditWordsFontSize;
  creditWordsElement->add (createMxmlAttribute ("font-size", s.str ()));

  // set its "font-weight" attribute
  string fontWeightString;

  switch (elt->getCreditWordsFontWeightKind ()) {
    case kFontWeightNone:
      break;
    case kFontWeightNormal:
      fontWeightString = "normal";
      break;
    case kFontWeightBold:
      fontWeightString = "bold";
      break;
    } // switch

  if (fontWeightString.size ()) {
    creditWordsElement->add (createMxmlAttribute ("font-weight", fontWeightString));
  }

  // set its "font-style" attribute
  string fontStyleString;

  switch (elt->getCreditWordsFontStyleKind ()) {
    case kFontStyleNone:
      break;
    case kFontStyleNormal:
      fontStyleString = "normal";
      break;
    case KFontStyleItalic:
      fontStyleString = "italic";
      break;
    } // switch

  if (fontStyleString.size ()) {
    creditWordsElement->add (createMxmlAttribute ("font-style", fontStyleString));
  }

  // set its "justify" attribute
  string justifyString;

  switch (elt->getCreditWordsJustifyKind ()) {
    case kJustifyNone:
      break;
    case kJustifyLeft:
      justifyString = "left";
      break;
    case kJustifyCenter:
      justifyString = "center";
      break;
    case kJustifyRight:
      justifyString = "right";
      break;
    } // switch

  if (justifyString.size ()) {
    creditWordsElement->add (createMxmlAttribute ("justify", justifyString));
  }

  // set its "halign" attribute
  string horizontalAlignmentString;

  switch (elt->getCreditWordsHorizontalAlignmentKind ()) {
    case kHorizontalAlignmentNone:
      break;
    case kHorizontalAlignmentLeft:
      horizontalAlignmentString = "left";
      break;
    case kHorizontalAlignmentCenter:
      horizontalAlignmentString = "center";
      break;
    case kHorizontalAlignmentRight:
      horizontalAlignmentString = "right";
      break;
    } // switch

  if (horizontalAlignmentString.size ()) {
    creditWordsElement->add (createMxmlAttribute ("halign", horizontalAlignmentString));
  }

  // set its "valign" attribute
  string verticalAlignmentString;

  switch (elt->getCreditWordsVerticalAlignmentKind ()) {
    case kVerticalAlignmentNone:
      break;
    case kVerticalAlignmentTop:
      verticalAlignmentString = "top";
      break;
    case kVerticalAlignmentMiddle:
      verticalAlignmentString = "middle";
      break;
    case kVerticalAlignmentBottom:
      verticalAlignmentString = "bottom";
      break;
    } // switch

  if (verticalAlignmentString.size ()) {
    creditWordsElement->add (createMxmlAttribute ("valign", verticalAlignmentString));
  }

  // set its "xml:lang" attribute
  creditWordsElement->add (
    createMxmlAttribute (
      "xml:lang",
      msrXMLLangKindAsString (elt->getCreditWordsXMLLang ())));

  // append it to the current credit element
  fCurrentScoreCreditElement->push (creditWordsElement);
}

void msr2mxmlTreeTranslator::visitEnd (S_msrCreditWords& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrPartGroup& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPartGroup " <<
      elt->getPartGroupCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  switch (elt->getPartGroupImplicitKind ()) {
    case msrPartGroup::kPartGroupImplicitYes:
      // this is an MSR concept, ignore this part group
      break;

    case msrPartGroup::kPartGroupImplicitNo:
      {
        if (gGlobalMsr2mxmlTreeOahGroup->getMusicXMLComments ()) {
          // create a start comment
          stringstream s;
          s <<
            " ========== " <<
            elt->getPartGroupCombinedName () <<
            " START" <<
              ", line " << inputLineNumber <<
            " ========== ";
          Sxmlelement comment = createMxmlElement (kComment, s.str ());

          // append it to the current part list element
          fScorePartListElement->push (comment);
        }

        // create a part group element
        Sxmlelement scorePartGroupElement = createMxmlElement (k_part_group, "");

        // set it's "number" attribute
        scorePartGroupElement->add (
          createMxmlIntegerAttribute (
            "number", elt->getPartGroupNumber ()));

        // set it's "type" attribute
        scorePartGroupElement->add (createMxmlAttribute ("type", "start"));

        // create a group symbol element to the part group element if relevant
        string groupSymbolString;

        switch (elt->getPartGroupSymbolKind ()) {
          case msrPartGroup::kPartGroupSymbolNone:
            break;
          case msrPartGroup::kPartGroupSymbolBrace:
            groupSymbolString = "brace";
            break;
          case msrPartGroup::kPartGroupSymbolBracket:
            groupSymbolString = "bracket";
            break;
          case msrPartGroup::kPartGroupSymbolLine:
            groupSymbolString = "line";
            break;
          case msrPartGroup::kPartGroupSymbolSquare:
            groupSymbolString = "square";
            break;
        } // switch

        if (groupSymbolString.size ()) {
          Sxmlelement groupSymbolElement =
            createMxmlElement (
              k_group_symbol,
              groupSymbolString);

          // set its "default-x" attribute if relevant
          int
            partGroupSymbolDefaultX  =
              elt->getPartGroupSymbolDefaultX ();

          if (partGroupSymbolDefaultX != INT_MIN) { // JMI superfluous???
            groupSymbolElement->add (
              createMxmlIntegerAttribute (
                "default-x",
                partGroupSymbolDefaultX));
          }

          // append it to the part group element
          scorePartGroupElement->push (groupSymbolElement);
        }

        // append a group name element to the part group element
        string
          groupName = elt->getPartGroupName ();

        if (groupName.size ()) {
          scorePartGroupElement->push (
            createMxmlElement (
              k_group_name,
              groupName));
        }

        // append a group barline element to the part group element
        string groupBarlineString;

        switch (elt->getPartGroupBarlineKind ()) {
          case msrPartGroup::kPartGroupBarlineYes:
            groupBarlineString = "yes";
            break;
          case msrPartGroup::kPartGroupBarlineNo:
            groupBarlineString = "no";
            break;
        } // switch

        scorePartGroupElement->push (
          createMxmlElement (
            k_group_barline,
            groupBarlineString));

        // append the part group element to the part list element
        fScorePartListElement->push (scorePartGroupElement);

        // push the part group element onto the stack
        fPartGroupElementsStack.push (scorePartGroupElement);
      }
      break;
  } // switch
}

void msr2mxmlTreeTranslator::visitEnd (S_msrPartGroup& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPartGroup " <<
      elt->getPartGroupCombinedName () <<
      ", line " << inputLineNumber <<
      endl;

    if (false) { // JMI TEMP
      gLogStream <<
        endl <<
        "--> partgroup elt:" <<
        endl <<
        elt <<
        endl << endl;
    }
  }
#endif

  switch (elt->getPartGroupImplicitKind ()) {
    case msrPartGroup::kPartGroupImplicitYes:
      // this is an MSR concept, ignore this part group
      break;

    case msrPartGroup::kPartGroupImplicitNo:
      {
        if (gGlobalMsr2mxmlTreeOahGroup->getMusicXMLComments ()) {
          // create an end comment
          stringstream s;
          s <<
            " ========== " <<
            elt->getPartGroupCombinedName () <<
            " END" <<
              ", line " << inputLineNumber <<
            " ========== ";
          Sxmlelement comment = createMxmlElement (kComment, s.str ());

          // append it to the current part list element
          fScorePartListElement->push (comment);
        }

        // fetch the top-most part group element on the stack
        Sxmlelement
          partGroupElementsStackTop =
            fPartGroupElementsStack.top ();

/* JMI ???
        // sanity check
        if (elt != partGroupElementsStackTop) {
          s <<
            "elt " <<
            mxmlElementAsString (elt) <<
            " and partGroupElementsStackTop " <<
            mxmlElementAsString (partGroupElementsStackTop) <<
            " are different" <<
            ", line " << inputLineNumber;

          msrInternalError (
            gGlobalOahOahGroup->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
*/
        // create a part group element
        Sxmlelement scorePartGroupElement = createMxmlElement (k_part_group, "");

        // set it's "number" attribute
        scorePartGroupElement->add (
          createMxmlIntegerAttribute (
            "number", elt->getPartGroupNumber ()));

        // set it's "type" attribute
        scorePartGroupElement->add (createMxmlAttribute ("type", "stop"));

        // append the part group element to the part list element
        fScorePartListElement->push (scorePartGroupElement);

        // pop the part group element from the stack
        fPartGroupElementsStack.pop ();
      }
      break;
  } // switch
}

void msr2mxmlTreeTranslator::visitStart (S_msrPart& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();
#endif

if (false) // JMI
  gLogStream << elt << endl;

  string
    partID =
      elt->getPartID (),
    partName =
      elt->getPartName (),
    partAbbreviation =
      elt->getPartAbbreviation (),
    partCombinedName =
      elt->getPartCombinedName ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPart " <<
      partCombinedName <<
      ", line " << inputLineNumber <<
      endl;

    if (false) { // JMI TEMP
      gLogStream <<
        endl <<
        "--> part elt:" <<
        endl <<
        elt <<
        endl << endl;
    }
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

  fCurrentMSRPart = elt;

  ++gIndenter;

  // create a score part element
  fScorePartElement = createMxmlElement (k_score_part, "");
  // set it's "id" attribute
  fScorePartElement->add (createMxmlAttribute ("id", partID));

  // append it to the part list element
  fScorePartListElement->push (fScorePartElement);

  // append a part name element to the score part element
  fScorePartElement->push (
    createMxmlElement (
      k_part_name,
      partName));

  // append a part abbreviation element to the score part element
  if (partAbbreviation.size ()) {
    fScorePartElement->push (
      createMxmlElement (
        k_part_abbreviation,
        partAbbreviation));
  }

  // append the score instrument element if any to the score part element
  if (fScoreInstrumentElement) {
    // set its id
    string
      partID = fCurrentMSRPart->getPartID ();
    fScoreInstrumentElement->add (createMxmlAttribute ("id", partID + "I1"));

    // append it to the score instrument element
    fScorePartElement->push (
      fScoreInstrumentElement);
  }

  // create a part element
  fCurrentPartElement = createMxmlElement (k_part, "");
  // set its "id" attribute
	fCurrentPartElement->add (createMxmlAttribute ("id", partID));

  // append it to the pending part elements list
  fPendingPartElementsList.push_back (fCurrentPartElement);

  // get the part shortest note's duration
  fPartShortestNoteDuration =
    elt->getPartShortestNoteDuration ();
    /* JMI ???
  fPartShortestNoteTupletFactor =
    elt->getPartShortestNoteTupletFactor ();

  // compute the divisions per quarter note
  if (fPartShortestNoteDuration > rational (1, 4)) {
    // the shortest duration should be a quarter note at most
    fPartShortestNoteDuration = rational (1, 4);
  }
*/

/*
  rational
    partShortestNoteTupletFactorAsRational =
      fPartShortestNoteTupletFactor.asRational ();
*/

  rational
    divisionsPerQuarterNoteAsRational =
      rational (1, 4)
       /
      fPartShortestNoteDuration;
  divisionsPerQuarterNoteAsRational.rationalise ();

    /* JMI
  rational
    rationalDivisionsPerQuarterNote =
      divisionsPerQuarterNoteAsRational
        *
      partShortestNoteTupletFactorAsRational;
      divisionsPerQuarterNoteAsRational.getNumerator ()
        *
      divisionsPerQuarterNoteAsRational.getDenominator ();
  rationalDivisionsPerQuarterNote.rationalise ();
    */

  // compute divisions per quarter note and multiplying factor
  fDivisionsPerQuarterNote =
    divisionsPerQuarterNoteAsRational.getNumerator ();
  fDivisionsMultiplyingFactor =
    divisionsPerQuarterNoteAsRational.getDenominator ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMusicxmlOahGroup->getTraceDivisions ()) {
    gLogStream <<
      "--> partShortestNoteDuration: " <<
      fPartShortestNoteDuration <<
      endl <<
      "--> divisionsPerQuarterNoteAsRational: " <<
      divisionsPerQuarterNoteAsRational <<
      endl <<
      /* JMI
      "--> partShortestNoteTupletFactor: " <<
      fPartShortestNoteTupletFactor.asString () <<
      endl <<
      "--> partShortestNoteTupletFactorAsRational: " <<
      partShortestNoteTupletFactorAsRational <<
      endl <<
      */
      "--> divisionsPerQuarterNote: " <<
      fDivisionsPerQuarterNote <<
      endl <<
      "--> divisionsMultiplyingFactor: " <<
      fDivisionsMultiplyingFactor <<
      endl;
  }
#endif

/* JMI SUPERFLUOUS ???
  if (fDivisionsMultiplyingFactor.getDenominator () != 1) {
    stringstream s;

    s <<
      "divisionsMultiplyingFactor '" << fDivisionsMultiplyingFactor <<
      "' is no integer number" <<
      ", line " << inputLineNumber;

    msrInternalError (
// JMI    msrInternalWarning (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
*/

  // a divisions element has to be appended for this part
  fPartDivisionsElementHasToBeAppended = true;

  // should a <staves /> element be createMxmld?
  int partStavesNumber =
    fCurrentMSRPart->getPartStavesMap ().size ();
  if (partStavesNumber > 1) {
    // create a staves element
    fStavesElement =
      createMxmlIntegerElement (
        k_staves,
        partStavesNumber);
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrPart& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPart " <<
      elt->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  --gIndenter;

  // forget about measure elements in the map
  fPartMeasureNumbersToElementsMap.clear ();

  // forget about the current part element
  fCurrentPartElement = nullptr;

  // forget about the current part clef
  fCurrentPartClef = nullptr;

  // forget about the current part key
  fCurrentPartKey = nullptr;

  // forget about the current part time
  fCurrentPartTime = nullptr;
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrStaff& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStaff \"" <<
      elt->getStaffName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

    if (false) { // JMI TEMP
      gLogStream <<
        endl <<
        "--> staff elt:" <<
        endl <<
        elt <<
        endl << endl;
    }
#endif

/*
  ++gIndenter;

  switch (elt->getStaffKind ()) {
    case kStaffRegular:
    case kStaffTablature:
    case kStaffDrum:
    case kStaffRythmic:
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

    case kStaffHarmony:
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

    case kStaffFiguredBass:
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
  */
}

void msr2mxmlTreeTranslator::visitEnd (S_msrStaff& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_msrStaff \"" <<
      elt->getStaffName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

/*
  --gIndenter;

  switch (elt->getStaffKind ()) {
    case kStaffRegular:
    case kStaffDrum:
    case kStaffRythmic:
      {
        fOnGoingStaff = false;
      }
      break;

    case kStaffTablature:
      // JMI
      break;

    case kStaffHarmony:
      // JMI
      break;

    case kStaffFiguredBass:
      // JMI
      break;
  } // switch
  */
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrVoice& elt)
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

    if (false) { // JMI TEMP
      gLogStream <<
        endl <<
        "--> voice elt:" <<
        endl <<
        elt <<
        endl << endl;
    }
#endif

/*
  ++gIndenter;
*/
}

void msr2mxmlTreeTranslator::visitEnd (S_msrVoice& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrVoice \"" <<
      elt->getVoiceName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

/*
  --gIndenter;

  switch (elt->getVoiceKind ()) {
    case kVoiceRegular:
      // JMI
      break;

    case kVoiceHarmony:
      fOnGoingHarmonyVoice = false;
      break;

    case kVoiceFiguredBass:
      fOnGoingFiguredBassVoice = false;
      break;
  } // switch
  */
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrSegment& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber () ;

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      ", line " << inputLineNumber <<
      endl;

    if (false) { // JMI TEMP
      gLogStream <<
        endl <<
        "--> segment elt:" <<
        endl <<
        elt <<
        endl << endl;
    }
  }
#endif

  if (gGlobalMsr2mxmlTreeOahGroup->getMusicXMLComments ()) {
    // create a start comment
    stringstream s;
    s <<
      " ==================== " <<
      "Segment " <<
      elt->getSegmentAbsoluteNumber () <<
      " START" <<
        ", line " << inputLineNumber <<
      " ==================== ";
    Sxmlelement comment = createMxmlElement (kComment, s.str ());

    // append it to the current part element
    fCurrentPartElement->push (comment);
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrSegment& elt)
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

    if (false) { // JMI TEMP
      gLogStream <<
        endl <<
        "--> segment elt:" <<
        endl <<
        elt <<
        endl << endl;
    }
  }
#endif

  if (gGlobalMsr2mxmlTreeOahGroup->getMusicXMLComments ()) {
    // create an end comment
    stringstream s;
    s <<
      " ==================== " <<
      "Segment " <<
      elt->getSegmentAbsoluteNumber () <<
      " END" <<
        ", line " << inputLineNumber <<
      " ==================== ";
    Sxmlelement comment = createMxmlElement (kComment, s.str ());

    // append it to the current part element
    fCurrentPartElement->push (comment);
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrMeasure& elt)
{
  int
    inputLineNumber =
      elt->getInputLineNumber ();

  string
    measureNumber =
      elt->getMeasureElementMeasureNumber ();

#ifdef TRACING_IS_ENABLED
  int
    measurePuristNumber =
      elt->getMeasurePuristNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMeasure '" <<
      measureNumber <<
      "', measurePuristNumber = '" <<
      measurePuristNumber <<
      "', line " << inputLineNumber <<
      endl;

    if (false) { // JMI TEMP
      gLogStream <<
        endl <<
        "--> measure elt:" <<
        endl <<
        elt <<
        endl << endl;
    }
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasures ()) {
    gLogStream <<
      endl <<
      "<!--=== measure '" << measureNumber <<
//      "', voice \"" <<
//      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber << " ===-->" <<
      endl;
  }
#endif

  // has a measure element for this measure number been createMxmld already?
  map<string, Sxmlelement>::iterator
    it =
      fPartMeasureNumbersToElementsMap.find (measureNumber);

  if (it != fPartMeasureNumbersToElementsMap.end ()) {
    // yes, use existing measure element
    fCurrentMeasureElement = (*it).second;
  }
  else {
    // no

    if (gGlobalMsr2mxmlTreeOahGroup->getMusicXMLComments ()) {
      // create a comment
      stringstream s;
      s <<
        " ===== " <<
        "MEASURE " <<
        "ordinal number: " << elt->getMeasureOrdinalNumberInVoice () <<
        ", line " << inputLineNumber <<
        " ===== ";
      Sxmlelement comment = createMxmlElement (kComment, s.str ());

      // append it to the current part element
      fCurrentPartElement->push (comment);
    }

    // create a measure element
    fCurrentMeasureElement = createMxmlElement (k_measure, "");
    // set its "number" attribute
    fCurrentMeasureElement->add (createMxmlAttribute ("number", measureNumber));

    // append it to the current part element
    fCurrentPartElement->push (fCurrentMeasureElement);

    // register it in the part measures map
    fPartMeasureNumbersToElementsMap [measureNumber] = fCurrentMeasureElement;
  }

  // is there a print element to be appended?
  S_msrPrintLayout
    measurePrintLayout =
      elt->getMeasurePrintLayout ();

  if (measurePrintLayout) {
    if (gGlobalMsr2mxmlTreeOahGroup->getMusicXMLComments ()) {
      // create a print comment
      stringstream s;
      s <<
        " ===== " <<
        "Print" <<
        ", line " << inputLineNumber <<
        " ===== ";
      Sxmlelement comment = createMxmlElement (kComment, s.str ());

      // append it to the current measure element
      fCurrentMeasureElement->push (comment);
    }

    // create a print element
    fCurrentPrintElement = createMxmlElement (k_print, "");

    // append it to the current measure element at once,
    // since is must be the first one in the measure
    fCurrentMeasureElement->push (fCurrentPrintElement);
  }

  // is there a divisions element to be appended?
  if (fPartDivisionsElementHasToBeAppended) {
    // append a divisions element to the attributes element
    fDivisionsElement =
      createMxmlIntegerElement (
        k_divisions,
        fDivisionsPerQuarterNote);
    fAnAttributeElementIsNeeded = true;

    fPartDivisionsElementHasToBeAppended = false;
  }

if (false) { // JMI
  // there's no previous MSR note yet in this measure
#ifdef TRACING_IS_ENABLED
    if (
      gGlobalMusicxmlOahGroup->getTraceBackup ()
        ||
      gGlobalMusicxmlOahGroup->getTraceForward ()
    ) {
      gLogStream <<
        "--> setting previous note  to nullptr at " <<
        elt->asString () <<
        " at beginning of measure '" <<
        elt->getMeasurePuristNumber () <<
        "', line " << inputLineNumber <<
        endl;
    }
#endif

  fPreviousMSRNote = nullptr;
}

  fANoteHasBeenMetInCurrentMeasure = false;
}

void msr2mxmlTreeTranslator::visitEnd (S_msrMeasure& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();
#endif

  string
    measureNumber =
      elt->getMeasureElementMeasureNumber ();

  string
    nextMeasureNumber =
      elt->getNextMeasureNumber ();

#ifdef TRACING_IS_ENABLED
  int
    measurePuristNumber =
      elt->getMeasurePuristNumber ();

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

  // forget about the current measure element
  fCurrentMeasureElement = nullptr;

  // reset the current position in the measure
  fCurrentPositionInMeasure = rational (0, 1);

  // reset the cumulated skip durations informations
  fCurrentCumulatedSkipsDurations = rational (0, 1);
  fCurrentCumulatedSkipsStaffNumber = -1;
  fCurrentCumulatedSkipsVoiceNumber = -1;
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrPrintLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPrintLayout '" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePrintLayouts ()) {
    gLogStream <<
      endl <<
//      "', voice \"" <<
//      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber << " ===-->" <<
      endl;
  }
#endif

  // don't create a print element,
  // this has already been done in visitStart (S_msrMeasure&)

  // populate the current print element
  float staffSpacing = elt->getStaffSpacing ();
  if (staffSpacing > 0) {
    stringstream s;
    s << staffSpacing;
  	fCurrentPrintElement->add (createMxmlAttribute ("staff-spacing", s.str ()));
  }

  bool newSystem = elt->getNewSystem ();
  if (newSystem) {
  	fCurrentPrintElement->add (createMxmlAttribute ("new-system", "yes"));
  }

  bool newPage = elt->getNewPage ();
  if (newPage) {
  	fCurrentPrintElement->add (createMxmlAttribute ("new-page", "yes"));
  }

  int blankPage = elt->getBlankPage ();
  if (blankPage > 0) {
    stringstream s;
    s << blankPage;
  	fCurrentPrintElement->add (createMxmlAttribute ("blank-page", s.str ()));
  }

  int pageNumber = elt->getPageNumber ();
  if (pageNumber > 0) {
    stringstream s;
    s << pageNumber;
  	fCurrentPrintElement->add (createMxmlAttribute ("page-number", s.str ()));
  }

  fOnGoingPrintLayout = true;
}

void msr2mxmlTreeTranslator::visitEnd (S_msrPrintLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPrintLayout '" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  // forget about the current print layout element
  fCurrentPrintElement = nullptr;

  fOnGoingPrintLayout = false;
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrClef& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrClef" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  bool doAppendAClefElementToTheMeasure = false;

  if (fCurrentPartClef) {
    // a clef element has already been createMxmld for the current part:
    // each voice in an MSR staff has its own clefs,
    // another one should be createMxmld only if it is different the the current one
    doAppendAClefElementToTheMeasure =
      ! elt->isEqualTo (fCurrentPartClef);
  }
  else {
    // this the first clef met in the part
    doAppendAClefElementToTheMeasure = true;
  }

  if (doAppendAClefElementToTheMeasure) {
    // create the clef element
    Sxmlelement clefElement = createMxmlElement (k_clef, "");

    // set its "number" attribute
    clefElement->add (
      createMxmlIntegerAttribute ("number", elt->getClefStaffNumber ()));

    // populate it
    switch (elt->getClefKind ()) {
      case k_NoClef:
        {
          clefElement->push (
            createMxmlElement (
              k_sign,
              "none"));
        }
        break;
      case kTrebleClef:
        {
          clefElement->push (
            createMxmlElement (
              k_sign,
              "G"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              2));
        }
        break;
      case kSopranoClef:
        {
          clefElement->push (
            createMxmlElement (
              k_sign,
              "G"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              2));
        }
        break;
      case kMezzoSopranoClef:
        {
          clefElement->push (
            createMxmlElement (
              k_sign,
              "G"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              2));
        }
        break;
      case kAltoClef:
        {
          clefElement->push (
            createMxmlElement (
              k_sign,
              "G"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              2));
        }
        break;
      case kTenorClef:
        {
          clefElement->push (
            createMxmlElement (
              k_sign,
              "G"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              2));
        }
        break;
      case kBaritoneClef:
        {
          clefElement->push (
            createMxmlElement (
              k_sign,
              "G"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              2));
        }
        break;
      case kBassClef:
        {
          clefElement->push (
            createMxmlElement (
              k_sign,
              "F"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              4));
        }
        break;
      case kTrebleLine1Clef:
        {
          clefElement->push (
            createMxmlElement (
              k_sign,
              "G"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              1));
        }
        break;
      case kTrebleMinus15Clef:
        {
          clefElement->push (
            createMxmlElement (
              k_sign,
              "G"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              2));
          clefElement->push (
            createMxmlIntegerElement (
              k_clef_octave_change,
              -2));
        }
        break;
      case kTrebleMinus8Clef:
        {
          clefElement->push (
            createMxmlElement (
              k_sign,
              "G"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              2));
          clefElement->push (
            createMxmlIntegerElement (
              k_clef_octave_change,
              -1));
        }
        break;
      case kTreblePlus8Clef:
        {
          clefElement->push (
            createMxmlElement (
              k_sign,
              "G"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              2));
          clefElement->push (
            createMxmlIntegerElement (
              k_clef_octave_change,
              1));
        }
        break;
      case kTreblePlus15Clef:
        {
          clefElement->push (
            createMxmlElement (
              k_sign,
              "G"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              2));
          clefElement->push (
            createMxmlIntegerElement (
              k_clef_octave_change,
              2));
        }
        break;
      case kBassMinus15Clef:
        {
          clefElement->push (
            createMxmlElement (
              k_sign,
              "F"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              4));
          clefElement->push (
            createMxmlIntegerElement (
              k_clef_octave_change,
              -2));
        }
        break;
      case kBassMinus8Clef:
        {
          clefElement->push (
            createMxmlElement (
              k_sign,
              "F"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              4));
          clefElement->push (
            createMxmlIntegerElement (
              k_clef_octave_change,
              -1));
        }
        break;
      case kBassPlus8Clef:
        {
          clefElement->push (
            createMxmlElement (
              k_sign,
              "F"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              4));
          clefElement->push (
            createMxmlIntegerElement (
              k_clef_octave_change,
              1));
        }
        break;
      case kBassPlus15Clef:
        {
          clefElement->push (
            createMxmlElement (
              k_sign,
              "F"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              4));
          clefElement->push (
            createMxmlIntegerElement (
              k_clef_octave_change,
              2));
        }
        break;
      case kVarbaritoneClef:
        {
          clefElement->push (
            createMxmlElement (
              k_sign,
              "F"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              3));
        }
        break;

      case kTablature4Clef:
        {
          clefElement->push (
            createMxmlElement (
              k_sign,
              "tab"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              4));
        }
      case kTablature5Clef:
        {
          clefElement->push (
            createMxmlElement (
              k_sign,
              "tab"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              5));
        }
        break;
      case kTablature6Clef:
        {
          clefElement->push (
            createMxmlElement (
              k_sign,
              "tab"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              6));
        }
      case kTablature7Clef:
        {
          clefElement->push (
            createMxmlElement (
              k_sign,
              "tab"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              7));
        }
        break;

      case kPercussionClef:
        {
          clefElement->push (
            createMxmlElement (
              k_sign,
              "percussion"));
        }
        break;

      case kJianpuClef:
        {
          clefElement->push (
            createMxmlElement (
              k_sign,
              "jianpu"));
        }
        break;
    } // switch

    // append the clef element to the measure attributes element
    fClefElementsList.push_back (clefElement);
    fAnAttributeElementIsNeeded = true;

    // this clef becomes the new current part clef
    fCurrentPartClef = elt;
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrClef& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrKey& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrKey" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  bool doAppendAKeyElementToTheMeasure = false;

  if (fCurrentPartKey) {
    // a key element has already been createMxmld for the current part:
    // each voice in an MSR staff has its own keys,
    // another one should be createMxmld only if it is different the the current one
    doAppendAKeyElementToTheMeasure =
      ! elt->isEqualTo (fCurrentPartKey);
  }
  else {
    // this the first key met in the part
    doAppendAKeyElementToTheMeasure = true;
  }

  if (doAppendAKeyElementToTheMeasure) {
    // create the key element
    fKeyElement = createMxmlElement (k_key, "");

    // populate it
    switch (elt->getKeyKind ()) {
      case kTraditionalKind:
        {
          // compute the number of fifths
          const int K_NO_FIFTHS_NUMBER = -99;
          int       fifthsNumber = K_NO_FIFTHS_NUMBER;

          msrQuarterTonesPitchKind
            keyTonicQuarterTonesPitchKind =
              elt->getKeyTonicQuarterTonesPitchKind ();

          switch (keyTonicQuarterTonesPitchKind) {
            case k_NoQuarterTonesPitch_QTP:
            case k_Rest_QTP:
            case k_Skip_QTP:
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

          if (fifthsNumber != K_NO_FIFTHS_NUMBER) {
            // populate the key element
            fKeyElement->push (
              createMxmlIntegerElement (
                k_fifths,
                fifthsNumber));

            fKeyElement->push (
              createMxmlElement (
                k_mode,
                msrModeKindAsString (elt->getModeKind ())));
          }

          else {
            stringstream s;

            s <<
              "key fifthsNumber not specified for key '" <<
              elt->asShortString ();

            msrInternalError (
              gGlobalOahOahGroup->getInputSourceName (),
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
        }
        break;

      case kHumdrumScotKind:
        {
          // JMI
        }
        break;
    } // switch

    // append the key element to the measure attributes element
    fAnAttributeElementIsNeeded = true;

    // this key becomes the new current part key
    fCurrentPartKey = elt;
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrKey& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrTime& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTime" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  bool doAppendATimeElementToTheMeasure = false;

  if (fCurrentPartTime) {
    // a time element has already been createMxmld for the current part:
    // each voice in an MSR staff has its own times,
    // another one should be createMxmld only if it is different the the current one
    doAppendATimeElementToTheMeasure =
      ! elt->isEqualTo (fCurrentPartTime);
  }
  else {
    // this the first time met in the part
    doAppendATimeElementToTheMeasure = true;
  }

  if (doAppendATimeElementToTheMeasure) {
    // create the time element
    fTimeElement = createMxmlElement (k_time, "");

    // populate it
    switch (elt->getTimeSymbolKind ()) {
      case kTimeSymbolCommon:
        {
          fTimeElement->add (createMxmlAttribute ("symbol", "common"));

          fTimeElement->push (
            createMxmlIntegerElement (
              k_beats,
              4));
          fTimeElement->push (
            createMxmlIntegerElement (
              k_beat_type,
              4));
        }
        break;

      case kTimeSymbolCut:
         {
          fTimeElement->add (createMxmlAttribute ("symbol", "cut"));

          fTimeElement->push (
            createMxmlIntegerElement (
              k_beats,
              2));
          fTimeElement->push (
            createMxmlIntegerElement (
              k_beat_type,
              2));
        }
       break;

      case kTimeSymbolNote:
        break;

      case kTimeSymbolDottedNote:
        break;

      case kTimeSymbolSingleNumber:
        break;

      case kTimeSymbolSenzaMisura:
        break;

      case kTimeSymbolNone:
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

            fTimeElement->push (
              createMxmlIntegerElement (
                k_beats,
                beatsNumbersVector [0])); // the only element
            fTimeElement->push (
              createMxmlIntegerElement (
                k_beat_type,
                timeItem->getTimeBeatValue ()));
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

    // append the time element to the measure attributes element
    fAnAttributeElementIsNeeded = true;

    // this time becomes the new current part time
    fCurrentPartTime = elt;
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrTime& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrTempo& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTempo" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

/*
  const list<S_msrWords>&
    tempoWordsList =
      elt->getTempoWordsList ();

 unsigned int tempoWordsListSize = tempoWordsList.size ();
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
        fOutputStream <<
          "\\tempo ";

        if (tempoWordsListSize) {
          list<S_msrWords>::const_iterator
            iBegin = tempoWordsList.begin (),
            iEnd   = tempoWordsList.end (),
            i      = iBegin;

          for ( ; ; ) {
            S_msrWords words = (*i);

            fOutputStream <<
              "\"" << words->getWordsContents () << "\"";

            if (++i == iEnd) break;

            fOutputStream <<
              ' ';
          } // for

          fOutputStream <<
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
          Sxmlelement metronomeElement = createMxmlElement (k_metronome, "");

          // set its "parentheses" attribute
          metronomeElement->add (createMxmlAttribute ("parentheses", "yes"));

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
            createMxmlElement (
              k_beat_unit,
              msrDurationKindAsMusicXMLType (durationKind)));

          // append the per minute element to the metronome elements
          metronomeElement-> push (createMxmlElement (k_per_minute, tempoPerMinute));

          // append the dynamics element to the current measure element
          appendToMeasureDirection (
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

              fOutputStream <<
                "\"" << words->getWordsContents () << "\"";

              if (++i == iEnd) break;

              fOutputStream <<
                ' ';
            } // for
          }
          */
        }
        break;

      case msrTempo::kTempoParenthesizedNo:
        {
          // create the metronome element
          Sxmlelement metronomeElement = createMxmlElement (k_metronome, "");

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
            createMxmlElement (
              k_beat_unit,
              msrDurationKindAsMusicXMLType (durationKind)));

          // append the per minute element to the metronome elements
          metronomeElement-> push (createMxmlElement (k_per_minute, tempoPerMinute));

          // append the metronome element to the current measure element
          appendToMeasureDirection (
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

              fOutputStream <<
                "\"" << words->getWordsContents () << "\"";

              if (++i == iEnd) break;

              fOutputStream <<
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
      fOutputStream <<
        "\\tempo ";

      if (tempoWordsListSize) {
        list<S_msrWords>::const_iterator
          iBegin = tempoWordsList.begin (),
          iEnd   = tempoWordsList.end (),
          i      = iBegin;

        for ( ; ; ) {
          S_msrWords words = (*i);

          fOutputStream <<
            "\"" << words->getWordsContents () << "\"";

          if (++i == iEnd) break;

          fOutputStream <<
            ' ';
        } // for
      }

      fOutputStream <<
        ' ' <<
        "\\markup {" <<
        endl;

      ++gIndenter;

      fOutputStream <<
        "\\concat {" <<
        endl;

      ++gIndenter;

      switch (tempoParenthesizedKind) {
        case msrTempo::kTempoParenthesizedYes:
          fOutputStream <<
            "(" <<
            endl;
          break;
        case msrTempo::kTempoParenthesizedNo:
          break;
      } // switch

      ++gIndenter;

      if (gGlobalLpsrOahGroup->versionNumberGreaterThanOrEqualTo ("2.20")) {
        fOutputStream <<
          " \\smaller \\general-align #Y #DOWN \\note {";
      }
      else {
        fOutputStream <<
          " \\smaller \\general-align #Y #DOWN \\note #\"";
      }

      fOutputStream <<
        dottedDurationAsLilypondStringWithoutBackSlash (
          inputLineNumber,
          tempoBeatUnit);

      if (gGlobalLpsrOahGroup->versionNumberGreaterThanOrEqualTo ("2.20")) {
        fOutputStream <<
          "} #UP";
      }
      else {
        fOutputStream <<
          "\" #UP";
      }

      fOutputStream <<
        endl <<
        "\" = \"" <<
        endl;

      fOutputStream <<
        "(";
      if (gGlobalLpsrOahGroup->versionNumberGreaterThanOrEqualTo ("2.20")) {
        fOutputStream <<
          " \\smaller \\general-align #Y #DOWN \\note {";
      }
      else {
        fOutputStream <<
          " \\smaller \\general-align #Y #DOWN \\note #\"";
      }

      fOutputStream <<
        dottedDurationAsLilypondStringWithoutBackSlash (
          inputLineNumber,
          elt->getTempoEquivalentBeatUnit ());

      if (gGlobalLpsrOahGroup->versionNumberGreaterThanOrEqualTo ("2.20")) {
        fOutputStream <<
          "} #UP";
      }
      else {
        fOutputStream <<
          "\" #UP";
      }

      fOutputStream << endl;

      --gIndenter;

      switch (tempoParenthesizedKind) {
        case msrTempo::kTempoParenthesizedYes:
          fOutputStream <<
            ")" <<
            endl;
          break;
        case msrTempo::kTempoParenthesizedNo:
          break;
      } // switch

      --gIndenter;

      fOutputStream <<
        "}" <<
        endl;

      --gIndenter;

      fOutputStream <<
        "}" <<
        endl;
        */
        }
      break;

    case msrTempo::kTempoNotesRelationShip:
      {
    /*
      fOutputStream <<
        "\\tempoRelationship #\"";

      if (tempoWordsListSize) {
        list<S_msrWords>::const_iterator
          iBegin = tempoWordsList.begin (),
          iEnd   = tempoWordsList.end (),
          i      = iBegin;

        for ( ; ; ) {
          S_msrWords words = (*i);

          fOutputStream <<
     // JMI       "\"" <<
            words->getWordsContents (); // JMI <<
      // JMI      "\"";

          if (++i == iEnd) break;

          fOutputStream <<
            ' ';
        } // for
      }

      fOutputStream <<
        "\"";

      switch (tempoParenthesizedKind) {
        case msrTempo::kTempoParenthesizedYes:
          fOutputStream <<
            " ##t";
          break;
        case msrTempo::kTempoParenthesizedNo:
          fOutputStream <<
            " ##f";
          break;
      } // switch

      fOutputStream << endl;
      */
      }
      break;
  } // switch
}

void msr2mxmlTreeTranslator::visitEnd (S_msrTempo& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrChord& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrChord" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (gGlobalMsr2mxmlTreeOahGroup->getMusicXMLComments ()) {
    // create a chord start comment
    stringstream s;
    s <<
      " ===== " <<
      "Chord start " <<
      ", chordSoundingWholeNotes: " <<
      elt->getChordSoundingWholeNotes () <<
      ", " <<
      elt->getChordNotesVector ().size () <<
      " elements" <<
      ", line " << inputLineNumber <<
      " ===== ";
    fPendingChordStartCommentElement = createMxmlElement (kComment, s.str ());
  }

  // append it to the current measure element
  fCurrentMeasureElement->push (fPendingChordStartCommentElement);

  fOnGoingChord = true;
}

void msr2mxmlTreeTranslator::visitEnd (S_msrChord& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrChord" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (gGlobalMsr2mxmlTreeOahGroup->getMusicXMLComments ()) {
    // create a chord end comment
    stringstream s;
    s <<
      " ===== " <<
      "Chord end " <<
      ", chordSoundingWholeNotes: " <<
      elt->getChordSoundingWholeNotes () <<
      ", " <<
      elt->getChordNotesVector ().size () <<
      " elements" <<
      ", line " << inputLineNumber <<
      " ===== ";
    Sxmlelement comment = createMxmlElement (kComment, s.str ());

    // append it to the current measure element
    fCurrentMeasureElement->push (comment);
  }

  // forget about the pending chord start comment
  fPendingChordStartCommentElement = nullptr;

  fOnGoingChord = false;
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrTuplet& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTuplet" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (gGlobalMsr2mxmlTreeOahGroup->getMusicXMLComments ()) {
    // create a tuplet start comment
    stringstream s;
    s <<
      " ===== " <<
      "Tuplet start " <<
      ", tupletFactor: " <<
      elt->getTupletFactor ().asRational () <<
      ", " <<
      elt->getTupletElementsList ().size () << " elements" <<
      ", line " << inputLineNumber <<
      " ===== ";
    Sxmlelement comment = createMxmlElement (kComment, s.str ());

    // append it to the current measure element
    fCurrentMeasureElement->push (comment);
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrTuplet& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTuplet" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (gGlobalMsr2mxmlTreeOahGroup->getMusicXMLComments ()) {
    // create a tuplet end comment
    stringstream s;
    s <<
      " ===== " <<
      "Tuplet end " <<
      ", tupletFactor: " <<
      elt->getTupletFactor ().asRational () <<
      ", tupletElementsList: " <<
      elt->getTupletElementsList ().size () << " elements" <<
      ", line " << inputLineNumber <<
      " ===== ";
    Sxmlelement comment = createMxmlElement (kComment, s.str ());

    // append it to the current measure element
    fCurrentMeasureElement->push (comment);
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator:: appendNoteWedges (
  S_msrNote theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceWedges ()) {
    gLogStream <<
      "--> appendNoteWedges, theMsrNote = " <<
      theMsrNote->asShortString () <<
      endl;
  }
#endif

  const list<S_msrWedge>&
    noteWedges =
      theMsrNote->getNoteWedges () ;

  if (noteWedges.size ()) {
    list<S_msrWedge>::const_iterator i;

    for (i=noteWedges.begin (); i!=noteWedges.end (); ++i) {
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
      Sxmlelement wedgeElement = createMxmlElement (k_wedge, "");

      // set its "type" attribute
      wedgeElement->add (createMxmlAttribute ("type", typeString));

      // append the wedge element to the current measure element
      appendToMeasureDirection (
        wedgeElement,
        wedge->getWedgePlacementKind ());
    } // for
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator:: appendNoteDynamics (
  S_msrNote theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceDynamics ()) {
    gLogStream <<
      "--> appendNoteDynamics, theMsrNote = " <<
      theMsrNote->asShortString () <<
      endl;
  }
#endif

  const list<S_msrDynamics>&
    noteDynamics =
      theMsrNote->getNoteDynamics () ;

  if (noteDynamics.size ()) {
    list<S_msrDynamics>::const_iterator i;

    for (i=noteDynamics.begin (); i!=noteDynamics.end (); ++i) {
      S_msrDynamics dynamics = (*i);

      // create the dynamics element
      Sxmlelement dynamicsElement = createMxmlElement (k_dynamics, "");

      // create the dynamics specific sub-element
      int subElementID = -1;

      switch (dynamics->getDynamicsKind ()) {
        case kDynamicsF:
          subElementID = k_f;
          break;
        case kDynamicsFF:
          subElementID = k_ff;
          break;
        case kDynamicsFFF:
          subElementID = k_fff;
          break;
        case kDynamicsFFFF:
          subElementID = k_ffff;
          break;
        case kDynamicsFFFFF:
          subElementID = k_fffff;
          break;
        case kDynamicsFFFFFF:
          subElementID = k_ffffff;
          break;

        case kDynamicsP:
          subElementID = k_p;
          break;
        case kDynamicsPP:
          subElementID = k_pp;
          break;
        case kDynamicsPPP:
          subElementID = k_ppp;
          break;
        case kDynamicsPPPP:
          subElementID = k_pppp;
          break;
        case kDynamicsPPPPP:
          subElementID = k_ppppp;
          break;
        case kDynamicsPPPPPP:
          subElementID = k_pppppp;
          break;

        case kDynamicsMF:
          subElementID = k_mf;
          break;
        case kDynamicsMP:
          subElementID = k_mp;
          break;
        case kDynamicsFP:
          subElementID = k_fp;
          break;
        case kDynamicsFZ:
          subElementID = k_fz;
          break;
        case kDynamicsRF:
          subElementID = k_rf;
          break;
        case kDynamicsSF:
          subElementID = k_sf;
          break;

        case kDynamicsRFZ:
          subElementID = k_rfz;
          break;
        case kDynamicsSFZ:
          subElementID = k_sfz;
          break;
        case kDynamicsSFP:
          subElementID = k_sfp;
          break;
        case kDynamicsSFPP:
          subElementID = k_sfpp;
          break;
        case kDynamicsSFFZ:
          subElementID = k_sffz;
          break;
        case k_NoDynamics:
          ; // should not occur
          break;
      } // switch

      Sxmlelement dynamicsElementSubElement = createMxmlElement (subElementID, "");

      // append the dynamics sub-element to the dynamics elements
      dynamicsElement-> push (dynamicsElementSubElement);

      // append the dynamics element to the current measure element
      appendToMeasureDirection (
        dynamicsElement,
        dynamics->getDynamicsPlacementKind ());
    } // for
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::appendABackupToMeasure (
  S_msrNote theMsrNote)
{
  int inputLineNumber =
    theMsrNote->getInputLineNumber ();

  // fetch the backup duration divisions
  rational
    previousNoteMeasureElementPositionInMeasure =
      fPreviousMSRNote->getMeasureElementPositionInMeasure (),
    previousNoteSoundingWholeNotes =
      fPreviousMSRNote->getNoteSoundingWholeNotes (),

    theMsrNoteMeasureElementPositionInMeasure =
      fPreviousMSRNote->getMeasureElementPositionInMeasure (),
    theMsrNoteSoundingWholeNotes =
      theMsrNote->getNoteSoundingWholeNotes ();

  rational
    backupDuration =
      previousNoteMeasureElementPositionInMeasure
        +
      previousNoteSoundingWholeNotes
        -
      theMsrNoteMeasureElementPositionInMeasure;
  backupDuration.rationalise ();

  int
    backupDurationDivisions =
      wholeNotesAsDivisions (
        inputLineNumber,
        backupDuration);

#ifdef TRACING_IS_ENABLED
  if (gGlobalMusicxmlOahGroup->getTraceBackup ()) {
    gLogStream <<
      "Creating a backup element, theMsrNote = " <<
      theMsrNote->asShortString () <<

      ", previousNoteMeasureElementPositionInMeasure: " <<
      previousNoteMeasureElementPositionInMeasure <<
      ", previousNoteSoundingWholeNotes: " <<
      previousNoteSoundingWholeNotes <<

      ", theMsrNoteMeasureElementPositionInMeasure: " <<
      theMsrNoteMeasureElementPositionInMeasure <<
      ", theMsrNoteSoundingWholeNotes: " <<
      theMsrNoteSoundingWholeNotes <<

      ", backupDuration: " << backupDuration <<
      ", backupDurationDivisions: " << backupDurationDivisions <<

      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (gGlobalMsr2mxmlTreeOahGroup->getMusicXMLComments ()) {
    S_msrVoice
      noteVoice =
        theMsrNote->fetchNoteVoiceUpLink ();

    int
      noteStaffNumber =
        theMsrNote->
          fetchNoteStaffUpLink ()->
            getStaffNumber (),
      previousMSRNoteStaffNumber =
        fPreviousMSRNoteStaff->
          getStaffNumber ();

    // create a backup comment
    stringstream s;
    s <<
      " ===== " <<
      "Backup" <<

      ", previousNoteMeasureElementPositionInMeasure: " <<
      previousNoteMeasureElementPositionInMeasure <<
      ", previousNoteSoundingWholeNotes: " <<
      previousNoteSoundingWholeNotes <<

      ", theMsrNoteMeasureElementPositionInMeasure: " <<
      theMsrNoteMeasureElementPositionInMeasure <<
      ", theMsrNoteSoundingWholeNotes: " <<
      theMsrNoteSoundingWholeNotes <<

      ", backupDuration: " << backupDuration <<
      ", backupDurationDivisions: " << backupDurationDivisions <<

      ", from staff: " << previousMSRNoteStaffNumber <<
      ", to staff: " << noteStaffNumber <<
      ", line " << inputLineNumber <<
      " ===== ";
    Sxmlelement comment = createMxmlElement (kComment, s.str ());

    // append it to the current measure element
    appendOtherToMeasure (comment);
  }

  // create a backup element
  Sxmlelement backupElement = createMxmlElement (k_backup, "");

  // append a duration sub-element to it
  backupElement->push (
    createMxmlIntegerElement (k_duration, backupDurationDivisions));

  // append it to the current measure element
  appendOtherToMeasure (backupElement);

  // reset the cumulated skip durations informations
  fCurrentCumulatedSkipsDurations = rational (0, 1);
  fCurrentCumulatedSkipsStaffNumber = -1;
  fCurrentCumulatedSkipsVoiceNumber = -1;
}

//________________________________________________________________________
void msr2mxmlTreeTranslator:: appendAForwardToMeasure (
  S_msrNote theMsrNote)
{
  int inputLineNumber =
     theMsrNote->getInputLineNumber ();

  /*
    <forward>
      <duration>16</duration>
      <voice>1</voice>
      <staff>1</staff>
    </forward>
  */

  // fetch the forward duration divisions
  int
    forwardDurationDivisions =
      wholeNotesAsDivisions (
        inputLineNumber,
        fCurrentCumulatedSkipsDurations);

#ifdef TRACING_IS_ENABLED
  if (gGlobalMusicxmlOahGroup->getTraceForward ()) {
    gLogStream <<
      "Creating a forward element, note = " <<
      theMsrNote->asShortString () <<
      ", forwardDurationDivisions: " << forwardDurationDivisions <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  int
    noteStaffNumber =
      theMsrNote->
        fetchNoteStaffUpLink ()->
          getStaffNumber (),
    noteVoiceNumber =
      theMsrNote->
        fetchNoteVoiceUpLink ()->
          getVoiceNumber ();

  if (gGlobalMsr2mxmlTreeOahGroup->getMusicXMLComments ()) {
    int
      previousMSRNoteStaffNumber =
        fPreviousMSRNoteStaff->
          getStaffNumber (),
      previousMSRNoteVoiceNumber =
        fPreviousMSRNoteVoice->
          getVoiceNumber ();

    // create a forward comment
    S_msrVoice
      noteVoice =
        theMsrNote->fetchNoteVoiceUpLink ();

    stringstream s;
    s <<
      " ===== " <<
      "Forward" <<
      ", forwardDurationDivisions: " << forwardDurationDivisions <<
      ", in staff: " << previousMSRNoteStaffNumber <<
      ", in voice: " << previousMSRNoteVoiceNumber <<
      ", line " << inputLineNumber <<
      " ===== ";
    Sxmlelement comment = createMxmlElement (kComment, s.str ());

    // append it to the current measure element
    appendOtherToMeasure (comment);
  }

  // create a forward element
  Sxmlelement forwardElement = createMxmlElement (k_forward, "");

  // append a duration sub-element to it
  forwardElement->push (
    createMxmlIntegerElement (k_duration, forwardDurationDivisions));

  // append a voice sub-element to it
  forwardElement->push (
    createMxmlIntegerElement (k_voice, noteVoiceNumber));

  // append a staff sub-element to it
  forwardElement->push (
    createMxmlIntegerElement (k_staff, noteStaffNumber));

  // append it to the current measure element
  appendOtherToMeasure (forwardElement);

  // reset the cumulated skip durations informations
  fCurrentCumulatedSkipsDurations = rational (0, 1);
  fCurrentCumulatedSkipsStaffNumber = -1;
  fCurrentCumulatedSkipsVoiceNumber = -1;
}

//________________________________________________________________________
void msr2mxmlTreeTranslator:: appendABackupOrForwardToMeasureIfNeeded (
  S_msrNote theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
     theMsrNote->getInputLineNumber ();
#endif

  int
    noteStaffNumber =
      theMsrNote->
        fetchNoteStaffUpLink ()->
          getStaffNumber (),
    noteVoiceNumber =
      theMsrNote->
        fetchNoteVoiceUpLink ()->
          getVoiceNumber (),
    previousMSRNoteStaffNumber =
      fPreviousMSRNoteStaff
        ? fPreviousMSRNoteStaff->getStaffNumber ()
        : 0,
    previousMSRNoteVoiceNumber =
      fPreviousMSRNoteVoice
        ? fPreviousMSRNoteVoice->getVoiceNumber ()
        : 0;

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalMusicxmlOahGroup->getTraceBackup ()
      ||
    gGlobalMusicxmlOahGroup->getTraceForward ()
  ) {
    gLogStream <<
      "--> appendABackupOrForwardToMeasureIfNeeded(1), theMsrNote = " <<
      theMsrNote->asShortString () <<
      ", fCurrentCumulatedSkipsDurations: " << fCurrentCumulatedSkipsDurations <<
      ", noteStaffNumber: " << noteStaffNumber <<
      ", noteVoiceNumber: " << noteVoiceNumber <<
      ", previousMSRNote: ";
    if (fPreviousMSRNote) {
      gLogStream <<
        fPreviousMSRNote->asShortString ();
    }
    else {
      gLogStream << "none";
    }

    gLogStream <<
      ", previousMSRNoteStaffNumber: " << previousMSRNoteStaffNumber <<
      ", previousMSRNoteVoiceNumber: " << previousMSRNoteVoiceNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

/*
  A <backup /> is needed when a note is in another voice as the previous one.
  A <forward /> is needed when a note follows one or more skip(s) in the same voice.
  Consecutive skips are not createMxmld by mxmlTree2msr from MusicXML data,
  but this may happen if the  MSR API is used freely

  A skip in MSR may have been createMxmld when there was a <backup />
  to a position after the beginning of the measure,
  in which case a <backu> /? should be createMxmld in the MusicXML output,
  and not a <forward />
*/

  if (fOnGoingChord) { // JMI ???
  }

/*
fCurrentCumulatedSkipsStaffNumber
fCurrentCumulatedSkipsVoiceNumber
*/

  if (fPreviousMSRNote) {
    // compare the staff and voice numbers of note and fPreviousMSRNote

    if (noteStaffNumber == previousMSRNoteStaffNumber) {
      if (noteVoiceNumber == previousMSRNoteVoiceNumber) {
        // same staff, same voice

        // is a <forward /> or <backup /> element needed?
        if (fCurrentCumulatedSkipsDurations.getNumerator () != 0) {
          // a skip may have been createMxmld due to a <backup /> to a position
          // that is not at the beginning of the measure
          rational
            notePositionInMeasure =
              theMsrNote->getMeasureElementPositionInMeasure ();

          rational
            positionAfterNoteInMeasure =
              notePositionInMeasure +
                theMsrNote->getNoteSoundingWholeNotes ();
          positionAfterNoteInMeasure.rationalise ();

#ifdef TRACING_IS_ENABLED
          if (
            gGlobalMusicxmlOahGroup->getTraceBackup ()
              ||
            gGlobalMusicxmlOahGroup->getTraceForward ()
          ) {
            gLogStream <<
              "--> appendABackupOrForwardToMeasureIfNeeded(2), note = " <<
              theMsrNote->asShortString () <<
              ", notePositionInMeasure: " << notePositionInMeasure <<
              ", positionAfterNoteInMeasure: " << positionAfterNoteInMeasure <<
              ", fCurrentPositionInMeasure: " << fCurrentPositionInMeasure <<
              ", line " << inputLineNumber <<
              endl;
          }
#endif

          if (positionAfterNoteInMeasure < fCurrentPositionInMeasure) { // JMI TEST
            appendABackupToMeasure (theMsrNote);
          }
          else if (positionAfterNoteInMeasure > fCurrentPositionInMeasure) {
            appendAForwardToMeasure (theMsrNote);
          }
        }
      }

      else {
        // same staff, different voice
        appendABackupToMeasure (theMsrNote);
      }
    }

    else {
      // there is a staff change
      appendABackupToMeasure (theMsrNote);

      if (noteVoiceNumber == previousMSRNoteVoiceNumber) {
        // JMI
      }
      else {
        // JMI
      }
    }
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator:: populateNoteDirections (
  S_msrNote theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceNotes ()) {
    gLogStream <<
      "--> populateNoteDirections, theMsrNote = " <<
      theMsrNote->asShortString () <<
      endl;
  }
#endif

/*
<!ELEMENT direction-type (rehearsal+ | segno+ | coda+ |
	(words | symbol)+ | wedge | dynamics+ | dashes |
	bracket | pedal | metronome | octave-shift | harp-pedals |
	damp | damp-all | eyeglasses | string-mute |
	scordatura | image | principal-voice | percussion+ |
	accordion-registration | staff-divide | other-direction)>
<!ATTLIST direction-type
    %optional-unique-id;
>
*/

  // append the wedges elements if any
  appendNoteWedges (theMsrNote);

  // append the dynamics elements if any
  appendNoteDynamics (theMsrNote);
}

//________________________________________________________________________
void msr2mxmlTreeTranslator:: appendNoteOrnaments (
  S_msrNote theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOrnaments ()) {
    gLogStream <<
      "--> appendNoteOrnaments, theMsrNote = " <<
      theMsrNote->asShortString () <<
      endl;
  }
#endif

/* JMI
<!ELEMENT ornaments
	(((trill-mark
	| turn | delayed-turn | inverted-turn |
	   delayed-inverted-turn | vertical-turn |
	   inverted-vertical-turn | shake | wavy-line |
	   mordent | inverted-mordent | schleifer | tremolo |
	   haydn | other-ornament), accidental-mark*)*)>
<!ATTLIST ornaments
    %optional-unique-id;
>
<!ELEMENT trill-mark EMPTY>
<!ATTLIST trill-mark
    %print-style;
    %placement;
    %trill-sound;
>
	  */

  // append the ornament elements if any
  const list<S_msrOrnament>&
    noteOrnaments =
      theMsrNote->getNoteOrnaments () ;

  if (noteOrnaments.size ()) {
    list<S_msrOrnament>::const_iterator i;

    for (i=noteOrnaments.begin (); i!=noteOrnaments.end (); ++i) {
      S_msrOrnament ornament = (*i);

      msrOrnament::msrOrnamentKind
        ornamentKind =
          ornament->getOrnamentKind ();

      int ornamentType = kComment; // JMI

      switch (ornamentKind) {
        case msrOrnament::kOrnamentTrill:
          ornamentType = k_trill_mark;
          break;
        case msrOrnament::kOrnamentTurn:
          ornamentType = k_turn;
          break;
        case msrOrnament::kOrnamentInvertedTurn:
          ornamentType = k_inverted_turn;
          break;
        case msrOrnament::kOrnamentDelayedTurn:
          ornamentType = k_delayed_turn;
          break;
        case msrOrnament::kOrnamentDelayedInvertedTurn:
          ornamentType = k_delayed_inverted_turn;
          break;
        case msrOrnament::kOrnamentVerticalTurn:
          ornamentType = k_vertical_turn;
          break;
        case msrOrnament::kOrnamentMordent:
          ornamentType = k_mordent;
          break;
        case msrOrnament::kOrnamentInvertedMordent:
          ornamentType = k_inverted_mordent;
          break;
        case msrOrnament::kOrnamentSchleifer:
          ornamentType = k_schleifer;
          break;
        case msrOrnament::kOrnamentShake:
          ornamentType = k_shake;
          break;
        case msrOrnament::kOrnamentAccidentalKind:
          ornamentType = k_accidental_mark;
          break;
      } // switch

      // append the note ornament element to the current note element
      switch (ornamentKind) {
        case msrOrnament::kOrnamentAccidentalKind:
          {
            Sxmlelement ornamentElement =
              createMxmlElement (
                ornamentType,
                msrAccidentalKindAsMusicXMLString (
                  ornament->getOrnamentAccidentalKind ()));

            appendToNoteNotations (
              ornamentElement,
              ornament->getOrnamentPlacementKind ());
          }
          break;

        default:
          {
            Sxmlelement ornamentElement =
              createMxmlElement (
                ornamentType,
                "");

            appendToNoteNotationsOrnaments (
              ornamentElement,
              ornament->getOrnamentPlacementKind ());
          }
      } // switch
    } // for
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator:: appendNoteTechnicals (
  S_msrNote theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTechnicals ()) {
    gLogStream <<
      "--> appendNoteTechnicals, theMsrNote = " <<
      theMsrNote->asShortString () <<
      endl;
  }
#endif

/* JMI
<!ELEMENT technical
	((up-bow | down-bow | harmonic | open-string |
	  thumb-position | fingering | pluck | double-tongue |
	  triple-tongue | stopped | snap-pizzicato | fret |
	  string | hammer-on | pull-off | bend | tap | heel |
	  toe | fingernails | hole | arrow | handbell |
	  brass-bend | flip | smear | open | half-muted |
	  harmon-mute | golpe | other-technical)*)>
<!ATTLIST technical
    %optional-unique-id;

	  */

  // append the technical elements if any
  const list<S_msrTechnical>&
    noteTechnicals =
      theMsrNote->getNoteTechnicals () ;

  if (noteTechnicals.size ()) {
    list<S_msrTechnical>::const_iterator i;

    for (i=noteTechnicals.begin (); i!=noteTechnicals.end (); ++i) {
      S_msrTechnical
        technical = (*i);

      msrTechnical::msrTechnicalKind
        technicalKind =
          technical->getTechnicalKind ();

      int technicalType = kComment; // JMI

      switch (technicalKind) {
        case msrTechnical::kArrow:
          technicalType = k_arrow;
          break;
        case msrTechnical::kDoubleTongue:
          technicalType = k_double_tongue;
          break;
        case msrTechnical::kDownBow:
          technicalType = k_down_bow;
          break;
        case msrTechnical::kFingernails:
          technicalType = k_fingernails;
          break;
        case msrTechnical::kHarmonic:
          technicalType = k_harmonic;
          break;
        case msrTechnical::kHeel:
          technicalType = k_heel;
          break;
        case msrTechnical::kHole:
          technicalType = k_hole;
          break;
        case msrTechnical::kOpenString:
          technicalType = k_open_string;
          break;
        case msrTechnical::kSnapPizzicato:
          technicalType = k_snap_pizzicato;
          break;
        case msrTechnical::kStopped:
          technicalType = k_stopped;
          break;
        case msrTechnical::kTap:
          technicalType = k_tap;
          break;
        case msrTechnical::kThumbPosition:
          technicalType = k_thumb_position;
          break;
        case msrTechnical::kToe:
          technicalType = k_toe;
          break;
        case msrTechnical::kTripleTongue:
          technicalType = k_triple_tongue;
          break;
        case msrTechnical::kUpBow:
          technicalType = k_up_bow;
          break;
      } // switch

      // create the technical element
      Sxmlelement technicalElement = createMxmlElement (technicalType, "");

      // append the note technicals element to the current note element
      appendToNoteNotationsTechnicals (
        technicalElement,
        technical->getTechnicalPlacementKind ());
    } // for
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator:: appendNoteTechnicalWithIntegers (
  S_msrNote theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTechnicals ()) {
    gLogStream <<
      "--> appendNoteTechnicalWithIntegers, theMsrNote = " <<
      theMsrNote->asShortString () <<
      endl;
  }
#endif

/* JMI
<!ELEMENT technical
	((up-bow | down-bow | harmonic | open-string |
	  thumb-position | fingering | pluck | double-tongue |
	  triple-tongue | stopped | snap-pizzicato | fret |
	  string | hammer-on | pull-off | bend | tap | heel |
	  toe | fingernails | hole | arrow | handbell |
	  brass-bend | flip | smear | open | half-muted |
	  harmon-mute | golpe | other-technical)*)>
<!ATTLIST technical
    %optional-unique-id;

	  */

  // append the technicalWithInteger elements if any
  const list<S_msrTechnicalWithInteger>&
    noteTechnicalWithIntegers =
      theMsrNote->getNoteTechnicalWithIntegers () ;

  if (noteTechnicalWithIntegers.size ()) {
    list<S_msrTechnicalWithInteger>::const_iterator i;

    for (i=noteTechnicalWithIntegers.begin (); i!=noteTechnicalWithIntegers.end (); ++i) {
      S_msrTechnicalWithInteger
        technicalWithInteger = (*i);

      msrTechnicalWithInteger::msrTechnicalWithIntegerKind
        technicalWithIntegerKind =
          technicalWithInteger->getTechnicalWithIntegerKind ();

      int technicalWithIntegerType = kComment; // JMI

      switch (technicalWithIntegerKind) {
        case msrTechnicalWithInteger::kFingering:
          technicalWithIntegerType = k_fingering;
          break;
        case msrTechnicalWithInteger::kFret:
          technicalWithIntegerType = k_fret;
          break;
        case msrTechnicalWithInteger::kString:
          technicalWithIntegerType = k_string;
          break;
      } // switch

      // create the technicalWithInteger element
      int technicalWithIntegerValue =
        technicalWithInteger->getTechnicalWithIntegerValue ();

      Sxmlelement technicalWithIntegerElement =
        technicalWithIntegerValue > 0
          ? createMxmlIntegerElement (
              technicalWithIntegerType,
              technicalWithIntegerValue)
          : createMxmlElement (
              technicalWithIntegerType,
              "");

      // append the note technicalWithIntegers element to the current note element
      appendToNoteNotationsTechnicals (
        technicalWithIntegerElement,
        technicalWithInteger->getTechnicalWithIntegerPlacementKind ());
    } // for
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator:: appendNoteTechnicalWithFloats (
  S_msrNote theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTechnicals ()) {
    gLogStream <<
      "--> appendNoteTechnicalWithFloats, theMsrNote = " <<
      theMsrNote->asShortString () <<
      endl;
  }
#endif

/* JMI
<!ELEMENT technical
	((up-bow | down-bow | harmonic | open-string |
	  thumb-position | fingering | pluck | double-tongue |
	  triple-tongue | stopped | snap-pizzicato | fret |
	  string | hammer-on | pull-off | bend | tap | heel |
	  toe | fingernails | hole | arrow | handbell |
	  brass-bend | flip | smear | open | half-muted |
	  harmon-mute | golpe | other-technical)*)>
<!ATTLIST technical
    %optional-unique-id;

	  */

  // append the technicalWithFloat elements if any
  const list<S_msrTechnicalWithFloat>&
    noteTechnicalWithFloats =
      theMsrNote->getNoteTechnicalWithFloats () ;

  if (noteTechnicalWithFloats.size ()) {
    list<S_msrTechnicalWithFloat>::const_iterator i;

    for (i=noteTechnicalWithFloats.begin (); i!=noteTechnicalWithFloats.end (); ++i) {
      S_msrTechnicalWithFloat
        technicalWithFloat = (*i);

      msrTechnicalWithFloat::msrTechnicalWithFloatKind
        technicalWithFloatKind =
          technicalWithFloat->getTechnicalWithFloatKind ();

      int technicalWithFloatType = kComment; // JMI

      switch (technicalWithFloatKind) {
        case msrTechnicalWithFloat::kBend:
          technicalWithFloatType = k_bend;
          break;
      } // switch

      // create the technicalWithFloat element
      stringstream s;

      s << technicalWithFloat->getTechnicalWithFloatValue ();

      Sxmlelement technicalWithFloatElement =
        createMxmlElement (
          technicalWithFloatType,
          s.str ());

      // append the note technicalWithFloats element to the current note element
      appendToNoteNotationsTechnicals (
        technicalWithFloatElement,
        technicalWithFloat->getTechnicalWithFloatPlacementKind ());
    } // for
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator:: appendNoteTechnicalWithStrings (
  S_msrNote theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTechnicals ()) {
    gLogStream <<
      "--> appendNoteTechnicalWithStrings, theMsrNote = " <<
      theMsrNote->asShortString () <<
      endl;
  }
#endif

/* JMI
<!ELEMENT technical
	((up-bow | down-bow | harmonic | open-string |
	  thumb-position | fingering | pluck | double-tongue |
	  triple-tongue | stopped | snap-pizzicato | fret |
	  string | hammer-on | pull-off | bend | tap | heel |
	  toe | fingernails | hole | arrow | handbell |
	  brass-bend | flip | smear | open | half-muted |
	  harmon-mute | golpe | other-technical)*)>
<!ATTLIST technical
    %optional-unique-id;

	  */

  // append the technicalWithString elements if any
  const list<S_msrTechnicalWithString>&
    noteTechnicalWithStrings =
      theMsrNote->getNoteTechnicalWithStrings () ;

  if (noteTechnicalWithStrings.size ()) {
    list<S_msrTechnicalWithString>::const_iterator i;

    for (i=noteTechnicalWithStrings.begin (); i!=noteTechnicalWithStrings.end (); ++i) {
      S_msrTechnicalWithString
        technicalWithString = (*i);

      msrTechnicalWithString::msrTechnicalWithStringKind
        technicalWithStringKind =
          technicalWithString->getTechnicalWithStringKind ();

      int technicalWithStringType = kComment; // JMI

      switch (technicalWithStringKind) {
        case msrTechnicalWithString::kHammerOn:
          technicalWithStringType = k_hammer_on;
          break;
        case msrTechnicalWithString::kHandbell:
          technicalWithStringType = k_handbell;
          break;
        case msrTechnicalWithString::kOtherTechnical:
          technicalWithStringType = k_other_technical;
          break;
        case msrTechnicalWithString::kPluck:
          technicalWithStringType = k_pluck;
          break;
        case msrTechnicalWithString::kPullOff:
          technicalWithStringType = k_pull_off;
          break;
      } // switch

      // create the technicalWithString element
      Sxmlelement technicalWithStringElement =
        createMxmlElement (
          technicalWithStringType,
          technicalWithString->getTechnicalWithStringValue ());

      // append the note technicalWithStrings element to the current note element
      appendToNoteNotationsTechnicals (
        technicalWithStringElement,
        technicalWithString->getTechnicalWithStringPlacementKind ());
    } // for
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator:: appendNoteArticulations (
  S_msrNote theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceArticulations ()) {
    gLogStream <<
      "--> appendNoteArticulations, theMsrNote = " <<
      theMsrNote->asShortString () <<
      endl;
  }
#endif

  // append the articulation elements if any
  const list<S_msrArticulation>&
    noteArticulations =
      theMsrNote->getNoteArticulations () ;

  if (noteArticulations.size ()) {
    list<S_msrArticulation>::const_iterator i;

    for (i=noteArticulations.begin (); i!=noteArticulations.end (); ++i) {
      S_msrArticulation articulation = (*i);

      msrArticulation::msrArticulationKind
        articulationKind =
          articulation->getArticulationKind ();

      int articulationType = kComment; // JMI

/* JMI
	((accent | strong-accent | staccato | tenuto |
	  detached-legato | staccatissimo | spiccato |
	  scoop | plop | doit | falloff | breath-mark |
	  caesura | stress | unstress | soft-accent |
	  other-articulation)*)>
	  */

      switch (articulationKind) {
        case msrArticulation::k_NoArticulation:
          // JMI ???
          break;

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

      // create the articulation element
      Sxmlelement articulationElement = createMxmlElement (articulationType, "");

      // append it to the current note notations articulations element
      switch (articulationKind) {
        case msrArticulation::k_NoArticulation:
          // JMI ???
          break;

        case msrArticulation::kAccent:
        case msrArticulation::kBreathMark:
        case msrArticulation::kCaesura:
        case msrArticulation::kSpiccato:
        case msrArticulation::kStaccato:
        case msrArticulation::kStaccatissimo:
        case msrArticulation::kStress:
        case msrArticulation::kUnstress:
        case msrArticulation::kDetachedLegato:
        case msrArticulation::kStrongAccent:
        case msrArticulation::kTenuto:
        case msrArticulation::kDoit:
        case msrArticulation::kFalloff:
        case msrArticulation::kPlop:
        case msrArticulation::kScoop:

        case msrArticulation::kFermata:
          appendToNoteNotationsArticulations (
            articulationElement,
            articulation->getArticulationPlacementKind ());
          break;

        case msrArticulation::kArpeggiato:
        case msrArticulation::kNonArpeggiato:
          appendToNoteNotationsArticulations (
            articulationElement,
            articulation->getArticulationPlacementKind ());
          break;
      } // switch

        /*
        */
    } // for
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator:: appendNoteTieIfAny (
  S_msrNote theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTies ()) {
    gLogStream <<
      "--> appendNoteTieIfAny, theMsrNote = " <<
      theMsrNote->asShortString () <<
      endl;
  }
#endif

  // append the tie element if any
  S_msrTie noteTie = theMsrNote->getNoteTie ();

  if (noteTie) {
    // create the tied element
    Sxmlelement tiedElement = createMxmlElement (k_tied, "");

    // set its "type" attribute if any
    string typeString;

    switch (noteTie->getTieKind ()) {
      case kTieNone:
        break;
      case kTieStart:
        typeString = "start";
        break;
      case kTieContinue:
        typeString = "continue";
        break;
      case kTieStop:
        typeString = "stop";
        break;
    } // switch

    if (typeString.size ()) {
      tiedElement->add (
        createMxmlAttribute ("type", typeString));
    }

    // append it to the current note notations element
    appendToNoteNotations (
      tiedElement,
      noteTie->getTiePlacementKind ());
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator:: appendNoteSlursIfAny (
  S_msrNote theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceSlurs ()) {
    gLogStream <<
      "--> appendNoteSlursIfAny, theMsrNote = " <<
      theMsrNote->asShortString () <<
      endl;
  }
#endif

  // append the slur elements if any
  const list<S_msrSlur>&
    noteSlurs =
      theMsrNote->getNoteSlurs () ;

  if (noteSlurs.size ()) {
    list<S_msrSlur>::const_iterator i;

    for (i=noteSlurs.begin (); i!=noteSlurs.end (); ++i) {
      S_msrSlur
        slur = (*i);

      // create the slur element
      Sxmlelement slurElement = createMxmlElement (k_slur, "");

      // create the slur number attribute
      int slurNumber = slur->getSlurNumber ();

      if (slurNumber > 0) {
        slurElement->add (createMxmlIntegerAttribute ("number", slurNumber));
      }

      // create the slur type attribute
      msrSlurTypeKind
        slurTypeKind =
          slur->getSlurTypeKind ();

      string slurTypeString;

      switch (slurTypeKind) {
        case k_NoSlur:
          break;
        case kRegularSlurStart:
          slurTypeString = "start";
          break;
        case kPhrasingSlurStart:
          slurTypeString = "start";
          break;
        case kSlurContinue:
          slurTypeString = "slurContinue";
          break;
        case kRegularSlurStop:
          slurTypeString = "stop";
          break;
        case kPhrasingSlurStop:
          slurTypeString = "stop";
          break;
      } // switch

      if (slurTypeString.size ()) {
        slurElement->add (createMxmlAttribute ("type", slurTypeString));
      }

      // get the slur placement kind
      msrPlacementKind
        slurPlacementKind =
          slur->getSlurPlacementKind ();

      // append the slur element to the current note note notations element
      appendToNoteNotations (
        slurElement,
        slurPlacementKind);
    } // for
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator:: appendNoteTupletIfRelevant (
  S_msrNote theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "--> appendNoteTupletIfRelevant, theMsrNote = " <<
      theMsrNote->asShortString () <<
      endl;
  }
#endif

  switch (theMsrNote->getNoteKind ()) {
    case k_NoNote:
    case kNoteRestInMeasure:
    case kNoteSkipInMeasure:
    case kNoteUnpitchedInMeasure:
    case kNoteRegularInMeasure:
    case kNoteRegularInChord:
      break;

    case kNoteRegularInTuplet:
    case kNoteRestInTuplet:
      {
        // get theMsrNote's tuplet uplink
        S_msrTuplet
          noteDirectTupletUpLink =
            theMsrNote->
              getNoteDirectTupletUpLink ();

        // get theMsrNote's position in tuplet
        unsigned int
          notePositionInTuplet =
            theMsrNote->
              getPositionInTuplet ();

        // compute the type string if relevant
        string typeString;

        if (notePositionInTuplet == 1) {
          typeString = "start";
        }
        else if (
          notePositionInTuplet
            ==
          noteDirectTupletUpLink->getTupletElementsList ().size ()
        ) {
          typeString = "stop";
        }

        if (typeString.size ()) {
          // create a tuplet element
          Sxmlelement tupletElement = createMxmlElement (k_tuplet, "");

          // set its "number" attribute
          tupletElement->add (
            createMxmlIntegerAttribute (
              "number",
              noteDirectTupletUpLink->getTupletNumber ()));

          // set its "type" attribute
          tupletElement->add (
            createMxmlAttribute (
              "type",
              typeString));

          // append it to the current note notations elements
          appendToNoteNotations (
            tupletElement,
            k_NoPlacement);
        }
      }
      break;

    case kNoteInDoubleTremolo:
    case kNoteRegularInGraceNotesGroup:
    case kNoteSkipInGraceNotesGroup:
      break;

    case kNoteInChordInGraceNotesGroup:
      break;

    case kNoteInTupletInGraceNotesGroup:
      break;

    case kNoteUnpitchedInTuplet:
      break;
  } // switch
}

//________________________________________________________________________
void msr2mxmlTreeTranslator:: appendNoteSpannersBeforeNote (
  S_msrNote theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceSpanners ()) {
    gLogStream <<
      "--> appendNoteSpannersBeforeNote, theMsrNote = " <<
      theMsrNote->asShortString () <<
      endl;
  }
#endif

  // append the spanner elements if any
  const list<S_msrSpanner>&
    noteSpanners =
      theMsrNote->
        getNoteSpanners () ;

  if (noteSpanners.size ()) {
    list<S_msrSpanner>::const_iterator i;

    for (i=noteSpanners.begin (); i!=noteSpanners.end (); ++i) {
      S_msrSpanner spanner = (*i);

      msrSpannerTypeKind
        spannerTypeKind =
          spanner->
            getSpannerTypeKind ();

      // should we handle this spanner at all?
      bool doHandleSpanner = true;

      switch (spannerTypeKind) {
        case kSpannerTypeStart:
          break;
        case kSpannerTypeStop:
          doHandleSpanner = false;;
          break;
        case kSpannerTypeContinue:
          break;
        case k_NoSpannerType:
          // should not occur
          break;
      } // switch

      if (doHandleSpanner) {
        msrSpanner::msrSpannerKind // twice ??? JMI
          spannerKind =
            spanner->
              getSpannerKind ();

        int spannerType = kComment; // JMI

        Sxmlelement containingElement;

        switch (spannerKind) {
          case msrSpanner::kSpannerDashes:
            // dashes go into the measure direction element
            spannerType = k_dashes;
            break;
          case msrSpanner::kSpannerWavyLine:
            // wavy lines go into the note notations ornaments
            spannerType = k_wavy_line;
            break;
        } // switch

        // create the spanner element
        Sxmlelement spannerElement = createMxmlElement (spannerType, "");

        // set spannerElement's "number" attribute if relevant
        int
          spannerNumber =
            spanner->getSpannerNumber ();

        if (spannerNumber > 0) {
          spannerElement->add (createMxmlIntegerAttribute ("number", spannerNumber));
        }

        // set spannerElement's "type" attribute if relevant
        string
          typeString =
            msrSpannerTypeKindAsMusicXMLString (spannerTypeKind);

        if (typeString.size ()) {
          spannerElement->add (createMxmlAttribute ("type", typeString));
        }

        switch (spannerKind) {
          case msrSpanner::kSpannerDashes:
            // dashes go into the measure direction element
            appendToMeasureDirection (
              spannerElement,
              spanner->getSpannerPlacementKind ());
            break;
          case msrSpanner::kSpannerWavyLine:
            // wavy lines go into the note notations ornaments
            appendToNoteNotationsOrnaments (
              spannerElement,
              spanner->getSpannerPlacementKind ());
              break;
        } // switch
      }
    } // for
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator:: appendNoteSpannersAfterNote (
  S_msrNote theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceSpanners ()) {
    gLogStream <<
      "--> appendNoteSpannersAfterNote, theMsrNote = " <<
      theMsrNote->asShortString () <<
      endl;
  }
#endif

  // append the spanner elements if any
  const list<S_msrSpanner>&
    noteSpanners =
      theMsrNote->getNoteSpanners () ;

  if (noteSpanners.size ()) {
    list<S_msrSpanner>::const_iterator i;

    for (i=noteSpanners.begin (); i!=noteSpanners.end (); ++i) {
      S_msrSpanner spanner = (*i);

      msrSpannerTypeKind
        spannerTypeKind =
          spanner->getSpannerTypeKind ();

      // should we handle this spanner at all?
      bool doHandleSpanner = true;

      switch (spannerTypeKind) {
        case kSpannerTypeStart:
          doHandleSpanner = false;;
          break;
        case kSpannerTypeStop:
          break;
        case kSpannerTypeContinue:
          break;
        case k_NoSpannerType:
          // should not occur
          break;
      } // switch

      if (doHandleSpanner) {
        msrSpanner::msrSpannerKind
          spannerKind =
            spanner->getSpannerKind ();

        int spannerType = kComment; // JMI

        Sxmlelement containingElement;

        switch (spannerKind) {
          case msrSpanner::kSpannerDashes:
            // dashes go into the measure direction element
            spannerType = k_dashes;
            break;
          case msrSpanner::kSpannerWavyLine:
            // wavy lines go into the note notations ornaments
            spannerType = k_wavy_line;
            break;
        } // switch

        // create the spanner element
        Sxmlelement spannerElement = createMxmlElement (spannerType, "");

        // set spannerElement's "number" attribute if relevant
        int
          spannerNumber =
            spanner->getSpannerNumber ();

        if (spannerNumber > 1) {
          spannerElement->add (createMxmlIntegerAttribute ("number", spannerNumber));
        }

        // set spannerElement's "type" attribute if relevant
        string
          typeString =
            msrSpannerTypeKindAsMusicXMLString (spannerTypeKind);

        if (typeString.size ()) {
          spannerElement->add (createMxmlAttribute ("type", typeString));
        }

        switch (spannerKind) {
          case msrSpanner::kSpannerDashes:
            // dashes go into the measure direction element
            appendToMeasureDirection (
              spannerElement,
              spanner->getSpannerPlacementKind ());
            break;
          case msrSpanner::kSpannerWavyLine:
            // wavy lines go into the note notations ornaments
            appendToNoteNotationsOrnaments (
              spannerElement,
              spanner->getSpannerPlacementKind ());
              break;
        } // switch
      }
    } // for
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator:: appendStemToNote (
  S_msrNote theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceStems ()) {
    gLogStream <<
      "--> appendStemToNote, theMsrNote = " <<
      theMsrNote->asShortString () <<
      endl;
  }
#endif

  S_msrStem stem = theMsrNote->getNoteStem ();

  if (stem) {
    msrStem::msrStemKind
      stemKind = stem->getStemKind ();

    string stemString;

    switch (stemKind) {
      case msrStem::kStemNeutral:
        stemString = "none";
        break;
      case msrStem::kStemUp:
        stemString = "up";
        break;
      case msrStem::kStemDown:
        stemString = "down";
        break;
      case msrStem::kStemDouble:
        stemString = "double";
        break;
    } // switch

    // append a slur element to the current note element
    fCurrentNoteElement->push (
      createMxmlElement (
        k_stem,
        stemString));
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::appendBeamsToNote (
  S_msrNote theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceBeams ()) {
    gLogStream <<
      "--> appendBeamsToNote, theMsrNote = " <<
      theMsrNote->asShortString () <<
      endl;
  }
#endif

  // append the beam elements if any
  const list<S_msrBeam>&
    noteBeams =
      theMsrNote->getNoteBeams () ;

  if (noteBeams.size ()) {
    list<S_msrBeam>::const_iterator i;

    for (i=noteBeams.begin (); i!=noteBeams.end (); ++i) {
      S_msrBeam beam = (*i);

      msrBeamKind
        beamKind =
          beam->getBeamKind ();

      // create a beam element
      string beamString;

      switch (beamKind) {
        case k_NoBeam:
          break;
        case kBeamBegin:
          beamString = "begin";
          break;
        case kBeamContinue:
          beamString = "continue";
          break;
        case kBeamEnd:
          beamString = "end";
          break;
        case kBeamForwardHook:
          beamString = "forward hook";
          break;
        case kBeamBackwardHook:
          beamString = "backward hook";
          break;
      } // switch

      Sxmlelement beamElement = createMxmlElement (k_beam, beamString);

      // set its "number" attribute
      beamElement->add (createMxmlIntegerAttribute ("number", beam->getBeamNumber ()));

      // append the beam element to the current note element
      fCurrentNoteElement->push (beamElement);
    } // for
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator:: appendStaffToNoteIfRelevant (
  S_msrNote theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    theMsrNote->getInputLineNumber ();

  if (gGlobalTraceOahGroup->getTraceNotes ()) {
    gLogStream <<
      "--> appendStaffToNoteIfRelevant(), theMsrNote = " <<
      theMsrNote->asShortString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // fetch theMsrNote staff
  S_msrStaff
    noteStaff =
      theMsrNote->
        fetchNoteVoiceUpLink ()->
          getVoiceStaffUpLink ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceNotesDetails ()) {
    gLogStream <<
      endl <<
      "--> noteStaff: ";
    if (noteStaff) {
      gLogStream <<
        noteStaff;
    }
    else {
      gLogStream <<
        "none";
    }
  }
#endif

  // append the staff attribute if relevant
  if (noteStaff) {
    int
      noteStaffNumber =
        noteStaff->
          getStaffNumber ();

    if (true || noteStaffNumber != 1) { // options ? JMI
      fCurrentNoteElement->push (
        createMxmlIntegerElement (
          k_staff,
          noteStaffNumber));
    }
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::appendVoiceToNoteIfRelevant (
  S_msrNote theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    theMsrNote->getInputLineNumber ();

  if (gGlobalTraceOahGroup->getTraceNotes ()) {
    gLogStream <<
      "--> appendVoiceToNoteIfRelevant(), theMsrNote = " <<
      theMsrNote->asShortString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // fetch theMsrNote voice
  S_msrVoice
    noteVoice =
      theMsrNote->fetchNoteVoiceUpLink ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceNotesDetails ()) {
    gLogStream <<
      endl <<
      "--> noteVoice: ";
    if (noteVoice) {
      gLogStream <<
        noteVoice;
    }
    else {
      gLogStream <<
        "none";
    }
  }
#endif

  // append the voice attribute if relevant
  if (noteVoice) {
    int
      noteVoiceNumber =
        noteVoice->
          getVoiceNumber ();

    if (true || noteVoiceNumber != 1) { // options ? JMI
      fCurrentNoteElement->push (
        createMxmlIntegerElement (
          k_voice,
          noteVoiceNumber));
    }
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator:: appendNoteNotationsToNote (S_msrNote theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceNotes ()) {
    gLogStream <<
      "--> appendNoteNotationsToNote, theMsrNote = " <<
      theMsrNote->asShortString () <<
      endl;
  }
#endif

/*
<!ELEMENT notations
	(%editorial;,
	 (tied | slur | tuplet | glissando | slide |
	  ornaments | technical | articulations | dynamics |
	  fermata | arpeggiate | non-arpeggiate |
	  accidental-mark | other-notation)*)>
<!ATTLIST notations
    %print-object;
    %optional-unique-id;
>
*/

  // append the ornaments elements if any
  appendNoteOrnaments (theMsrNote);

  // append the articulation elements if any
  appendNoteArticulations (theMsrNote);

  // append the technical elements if any
  appendNoteTechnicals (theMsrNote);
  appendNoteTechnicalWithIntegers (theMsrNote);
  appendNoteTechnicalWithFloats (theMsrNote);
  appendNoteTechnicalWithStrings (theMsrNote);

  // append the tie element if any
  appendNoteTieIfAny (theMsrNote);

  // append the slur elements if any
  appendNoteSlursIfAny (theMsrNote);

  // append the tuplet elements if relevant
  appendNoteTupletIfRelevant (theMsrNote);
}

//________________________________________________________________________
void msr2mxmlTreeTranslator:: appendNoteLyricsToNote (
  S_msrNote theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "--> appendNoteLyricsToNote, theMsrNote = " <<
      theMsrNote->asShortString () <<
      endl;
  }
#endif

/*
<!ELEMENT lyric
	((((syllabic?, text),
	   (elision?, syllabic?, text)*, extend?) |
	   extend | laughing | humming),
	  end-line?, end-paragraph?, %editorial;)>
<!ATTLIST lyric
    number NMTOKEN #IMPLIED
    name CDATA #IMPLIED
    %justify;
    %position;
    %placement;
    %color;
    %print-object;
>
*/

  // append the lyric elements if any
  const list<S_msrSyllable>&
    noteSyllables =
      theMsrNote->getNoteSyllables () ;

  if (noteSyllables.size ()) {
    list<S_msrSyllable>::const_iterator i;

    for (i=noteSyllables.begin (); i!=noteSyllables.end (); ++i) {
      S_msrSyllable syllable = (*i);

      // create and append a lyric element if relevant
      bool   doCreateALyricElement = false;
      string syllabicString;

      switch ( syllable->getSyllableKind ()) {
        case msrSyllable::kSyllableNone:
          // should not occur
          break;

        case msrSyllable::kSyllableSingle:
          doCreateALyricElement = true;
          syllabicString = "single";
          break;

        case msrSyllable::kSyllableBegin:
          doCreateALyricElement = true;
          syllabicString = "begin";
          break;
        case msrSyllable::kSyllableMiddle:
          doCreateALyricElement = true;
          syllabicString = "middle";
          break;
        case msrSyllable::kSyllableEnd:
          doCreateALyricElement = true;
          syllabicString = "end";
          break;

        case msrSyllable::kSyllableOnRestNote:
          break;

        case msrSyllable::kSyllableSkipRestNote:
          break;
        case msrSyllable::kSyllableSkipNonRestNote:
          break;

        case msrSyllable::kSyllableMeasureEnd:
          break;

        case msrSyllable::kSyllableLineBreak:
          break;
        case msrSyllable::kSyllablePageBreak:
          break;
      } // switch

      if (doCreateALyricElement) {
        // create the lyric element
        Sxmlelement lyricElement = createMxmlElement (k_lyric, "");

        // set its "number" attribute
        lyricElement->add (
          createMxmlAttribute (
            "number",
            syllable->getSyllableStanzaNumber ()));

        // append a syllabic element to the lyric element if relevant
        if (syllabicString.size ()) {
          lyricElement->push (
            createMxmlElement (k_syllabic, syllabicString));
        }

        // append a text elements to the lyric element if relevant
        const list<string>&
          syllableTextsList =
            syllable->getSyllableTextsList ();

        for (
          list<string>::const_iterator i = syllableTextsList.begin ();
          i!=syllableTextsList.end ();
          ++i
        ) {
          string text = (*i);

          lyricElement->push (
            createMxmlElement (k_text, text));
        } // for

        // append the extend element to the lyric element if relevant
        string extendTypeString;
        bool   doCreateAnExtendElement = true;

        switch (syllable->getSyllableExtendKind ()) {
          case msrSyllable::kSyllableExtendNone:
          doCreateAnExtendElement = false;
            break;
          case msrSyllable::kSyllableExtendEmpty:
            break;
          case msrSyllable::kSyllableExtendSingle:
            extendTypeString = "single";
            break;
          case msrSyllable::kSyllableExtendStart:
            extendTypeString = "start";
            break;
          case msrSyllable::kSyllableExtendContinue:
            extendTypeString = "continue";
            break;
          case msrSyllable::kSyllableExtendStop:
            extendTypeString = "stop";
            break;
        } // switch

        if (doCreateAnExtendElement) {
          // create an extend element
          Sxmlelement extendElement = createMxmlElement (k_extend, "");

          if (extendTypeString.size ()) {
            // set its "type" attribute
            extendElement->add (createMxmlAttribute ("type", extendTypeString));
          }

          // append the extend element to the lyric element
          lyricElement->push (extendElement);
        }

        // append a syllabic element to the lyric element if relevant

        // append the lyric element to the current note element
        fCurrentNoteElement->push (lyricElement);
      }
    } // for
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::appendBasicsToNote (
  S_msrNote theMsrNote)
{
  int inputLineNumber =
    theMsrNote->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceNotes ()) {
    gLogStream <<
      "--> appendBasicsToNote(), theMsrNote = " <<
      theMsrNote->asShortString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // grab theMsrNote's informations
  msrNoteKind
    noteKind =
      theMsrNote->getNoteKind ();

  msrQuarterTonesPitchKind
    noteQuarterTonesPitchKind =
      theMsrNote->getNoteQuarterTonesPitchKind ();

  msrDiatonicPitchKind noteDiatonicPitchKind;
  msrAlterationKind    noteAlterationKind;

  fetchDiatonicPitchKindAndAlterationKindFromQuarterTonesPitchKind (
    inputLineNumber,
    noteQuarterTonesPitchKind,
    noteDiatonicPitchKind,
    noteAlterationKind);

  msrOctaveKind
    noteOctaveKind =
      theMsrNote->getNoteOctaveKind ();

  float
    noteMusicXMLAlter =
      msrMusicXMLAlterFromAlterationKind (
        noteAlterationKind);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceNotes ()) {
    gLogStream <<
      "-->  noteKind: " << noteKindAsString (noteKind) <<
      "-->  noteOctaveKind: " <<
      msrOctaveKindAsString (noteOctaveKind) <<
      "-->  noteDiatonicPitchKind: " <<
      msrDiatonicPitchKindAsString (noteDiatonicPitchKind) <<
      endl;
  }
#endif

  // append the chord sub element if relevant
  switch (noteKind) {
    case kNoteRegularInChord:
      if (! theMsrNote->getNoteIsAChordsFirstMemberNote ()) {
        fCurrentNoteElement->push (createMxmlElement (k_chord, ""));
      }
      break;
    default:
      ;
  } // switch

  // append the grace sub element if relevant
  switch (noteKind) {
    case kNoteRegularInGraceNotesGroup:
    case kNoteSkipInGraceNotesGroup:
      fCurrentNoteElement->push (createMxmlElement (k_grace, ""));
      break;
    default:
      ;
  } // switch

  // append the step and pitch or rest sub elements
  switch (noteKind) {
    case k_NoNote:
      break;

    case kNoteRestInMeasure:
      fCurrentNoteElement->push (createMxmlElement (k_rest, ""));
      break;

    case kNoteSkipInMeasure:
      break;

    case kNoteUnpitchedInMeasure:
      break;

    case kNoteRegularInMeasure:
    case kNoteRegularInChord:
    case kNoteRegularInTuplet:
      {
        // create the pitch element
        Sxmlelement pitchElement = createMxmlElement (k_pitch, "");

        // append the step element
        pitchElement->push (
          createMxmlElement (
            k_step,
            msrDiatonicPitchKindAsString (noteDiatonicPitchKind)));

        if (noteMusicXMLAlter != 0.0) {
          // append the alter element
          stringstream s;
          s << setprecision (2) << noteMusicXMLAlter;
          pitchElement->push (
            createMxmlElement (
              k_alter,
              s.str ()));
        }

        // append the octave element
        pitchElement->push (
          createMxmlIntegerElement (
            k_octave,
            octaveNumberFromOctaveKind (noteOctaveKind)));

        fCurrentNoteElement->push (pitchElement);
      }
      break;

    case kNoteRestInTuplet:
      fCurrentNoteElement->push (createMxmlElement (k_rest, ""));
      break;

    case kNoteInDoubleTremolo:
      break;

    case kNoteRegularInGraceNotesGroup:
    case kNoteSkipInGraceNotesGroup:
      {
        // create the pitch element
        Sxmlelement pitchElement = createMxmlElement (k_pitch, "");

        // append the step element
        pitchElement->push (
          createMxmlElement (
            k_step,
            msrDiatonicPitchKindAsString (noteDiatonicPitchKind)));

        if (noteMusicXMLAlter != 0.0) {
          // append the alter element
          stringstream s;
          s << setprecision (2) << noteMusicXMLAlter;
          pitchElement->push (
            createMxmlElement (
              k_alter,
              s.str ()));
        }

        // append the octave element
        pitchElement->push (
          createMxmlIntegerElement (
            k_octave,
            octaveNumberFromOctaveKind (noteOctaveKind)));

        // append the pitch element to the current note
        fCurrentNoteElement->push (pitchElement);
      }
      break;

    case kNoteInChordInGraceNotesGroup:
      break;

    case kNoteInTupletInGraceNotesGroup:
      break;

    case kNoteUnpitchedInTuplet:
      break;
  } // switch
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::appendDurationToNoteIfRelevant (
  S_msrNote theMsrNote)
{
  int inputLineNumber =
    theMsrNote->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceNotes ()) {
    gLogStream <<
      "--> appendDurationToNoteIfRelevant(1), theMsrNote = " <<
      theMsrNote->asShortString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // grab theMsrNote's informations
  msrNoteKind
    noteKind =
      theMsrNote->getNoteKind ();

  rational
    noteSoundingWholeNotes =
      theMsrNote->getMeasureElementSoundingWholeNotes ();

#ifdef TRACING_IS_ENABLED
  rational
    noteDisplayWholeNotes =
      theMsrNote->getNoteDisplayWholeNotes ();

  if (gGlobalTraceOahGroup->getTraceNotes ()) {
    gLogStream <<
      "---> noteKind: " << noteKindAsString (noteKind) <<
      ", noteSoundingWholeNotes: " << noteSoundingWholeNotes <<
      ", noteDisplayWholeNotes: " << noteDisplayWholeNotes <<
      ", noteTupletFactor: " << theMsrNote->getNoteTupletFactor ().asRational () <<
      ", divisionsPerQuarterNote: " << fDivisionsPerQuarterNote <<
      ", partShortestNoteDuration: " << fPartShortestNoteDuration <<
      ", fDivisionsMultiplyingFactor: " << fDivisionsMultiplyingFactor <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  bool doAppendDuration = false;

  switch (noteKind) {
    case k_NoNote:
      break;

    case kNoteSkipInMeasure:
      // nothing is generated at once for a skip note
      break;

    case kNoteUnpitchedInMeasure:
      break;

    case kNoteRestInMeasure:
    case kNoteRegularInMeasure:
    case kNoteRegularInChord:
      doAppendDuration = true;
      break;

    case kNoteRegularInTuplet:
    case kNoteRestInTuplet:
      doAppendDuration = true;
      break;


    case kNoteInDoubleTremolo:
      break;

    case kNoteRegularInGraceNotesGroup:
    case kNoteSkipInGraceNotesGroup:
      break;

    case kNoteInChordInGraceNotesGroup:
      break;

    case kNoteInTupletInGraceNotesGroup:
      break;

    case kNoteUnpitchedInTuplet:
      break;
  } // switch

  if (doAppendDuration) {
    rational
      soundingDurationAsRational =
        noteSoundingWholeNotes
          /
        fPartShortestNoteDuration
          *
        fDivisionsMultiplyingFactor;
    soundingDurationAsRational.rationalise ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceNotes ()) {
      gLogStream <<
        "---> soundingDurationAsRational: " <<
        soundingDurationAsRational <<
        "--> line " << inputLineNumber <<
        endl;
    }
#endif

    if (soundingDurationAsRational.getDenominator () != 1) {
      stringstream s;

      s <<
        "soundingDurationAsRational '" << soundingDurationAsRational <<
        "' is no integer number" <<
        ", line " << inputLineNumber;

      msrInternalError (
        gGlobalOahOahGroup->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }

    fCurrentNoteElement->push (
      createMxmlIntegerElement (
        k_duration,
        soundingDurationAsRational.getNumerator ()));

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceNotesDetails ()) {
      gLogStream <<
        endl <<
        "--> appendDurationToNoteIfRelevant(2): " <<
        theMsrNote <<
        endl;
    }
#endif
  }
}

void msr2mxmlTreeTranslator::appendTimeModificationToNoteIfRelevant (
  S_msrNote theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    theMsrNote->getInputLineNumber ();

  if (gGlobalTraceOahGroup->getTraceNotes ()) {
    gLogStream <<
      "--> appendTimeModificationToNoteIfRelevant(), theMsrNote = " <<
      theMsrNote->asShortString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  msrNoteKind
    noteKind =
      theMsrNote->getNoteKind ();

  // append the time modification if relevant
  switch (noteKind) {
    case k_NoNote:
      break;

    case kNoteRestInMeasure:
      break;

    case kNoteSkipInMeasure:
      break;

    case kNoteUnpitchedInMeasure:
      break;

    case kNoteRegularInMeasure:
      break;

    case kNoteInDoubleTremolo:
      break;

    case kNoteRegularInGraceNotesGroup:
    case kNoteSkipInGraceNotesGroup:
      break;

    case kNoteInChordInGraceNotesGroup:
      break;

    case kNoteRegularInChord:
      break;

    case kNoteRegularInTuplet:
    case kNoteRestInTuplet:
      {
        Sxmlelement
          timeModificationElement = createMxmlElement (k_time_modification, "");

        timeModificationElement->push (
          createMxmlIntegerElement (
            k_actual_notes,
            theMsrNote->getNoteTupletFactor ().getTupletActualNotes ()));
        timeModificationElement->push (
          createMxmlIntegerElement (
            k_normal_notes,
            theMsrNote->getNoteTupletFactor ().getTupletNormalNotes ()));

        fCurrentNoteElement->push (timeModificationElement);
      }
      break;

    case kNoteInTupletInGraceNotesGroup:
      break;

    case kNoteUnpitchedInTuplet:
      break;
  } // switch
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::appendMsrNoteToMesureIfRelevant (
  S_msrNote theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceNotes ()) {
    gLogStream <<
      "--> appendMsrNoteToMesureIfRelevant, theMsrNote = " <<
      theMsrNote->asShortString () <<
      endl;
  }
#endif

  int inputLineNumber =
    theMsrNote->getInputLineNumber ();

  // should a note sub-element be generated?
  bool doGenerateNote = true;

  switch (theMsrNote->getNoteKind ()) {
    case k_NoNote:
      break;
    case kNoteRestInMeasure:
      break;
    case kNoteSkipInMeasure:
      doGenerateNote = false;
      // cumulating the skip notes durations for <forward /> elements generation
      fCurrentCumulatedSkipsDurations +=
        theMsrNote->getNoteSoundingWholeNotes ();
      break;
    case kNoteUnpitchedInMeasure:
      break;
    case kNoteRegularInMeasure:
      break;

    case kNoteRegularInChord:
      if (theMsrNote->getNoteIsAChordsFirstMemberNote ()) {
        if (false && fPendingChordStartCommentElement) { // JMI
          // append the pending chord start comment to the current part element
          fCurrentPartElement->push (fPendingChordStartCommentElement);
        }
      }
      break;

    case kNoteRegularInTuplet:
    case kNoteRestInTuplet:
      break;

    case kNoteInDoubleTremolo:
    case kNoteRegularInGraceNotesGroup:
    case kNoteSkipInGraceNotesGroup:
      break;

    case kNoteInChordInGraceNotesGroup:
      break;

    case kNoteInTupletInGraceNotesGroup:
      break;

    case kNoteUnpitchedInTuplet:
      break;
  } // switch

  if (doGenerateNote) {
    // create a note element
    fCurrentNoteElement = createMxmlElement (k_note, "");

    // append theMsrNote basic sub-elements
    appendBasicsToNote (theMsrNote);

    // append the duration sub-element if relevant
    appendDurationToNoteIfRelevant (theMsrNote);

    // append the voice sub-element if relevant
    appendVoiceToNoteIfRelevant (theMsrNote);

    // append the type sub-element if relevant
    bool doGenerateType = true;

    switch (theMsrNote->getNoteKind ()) {
      case k_NoNote:
        break;

      case kNoteRestInMeasure:
        doGenerateType = false;
        break;

      case kNoteSkipInMeasure:
        break;

      case kNoteUnpitchedInMeasure:
        break;

      case kNoteRegularInMeasure:
        break;

      case kNoteRegularInChord:
        break;

      case kNoteRegularInTuplet:
      case kNoteRestInTuplet:
        break;

      case kNoteInDoubleTremolo:
        break;

      case kNoteRegularInGraceNotesGroup:
      case kNoteSkipInGraceNotesGroup:
        break;

      case kNoteInChordInGraceNotesGroup:
        break;

      case kNoteInTupletInGraceNotesGroup:
        break;

      case kNoteUnpitchedInTuplet:
        break;
    } // switch

    if (doGenerateType) {
      // append a type element containing the graphic duration
      // as a MusicXML string
      fCurrentNoteElement->push (
        createMxmlElement (
          k_type,
          theMsrNote->noteGraphicDurationAsMusicXMLString ()));
    }

    // append the time-modification sub-element if relevant
    appendTimeModificationToNoteIfRelevant (theMsrNote);

    // append the dots sub-element if relevant
    int
      noteDotsNumber =
        theMsrNote->getNoteDotsNumber ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceNotes ()) {
      gLogStream <<
        "-->  noteDotsNumber: " << noteDotsNumber <<
        "--> line " << inputLineNumber <<
        endl;
    }
#endif

    for (int i = 0; i < noteDotsNumber; ++i) {
      fCurrentNoteElement->push (
        createMxmlElement (
          k_dot, ""));
    } // for

    // append the accidental if any
    msrAccidentalKind
      accidentalKind =
        theMsrNote->getNoteAccidentalKind ();

    string
      accidentalString =
        msrAccidentalKindAsMusicXMLString (
          accidentalKind);

    if (accidentalString.size ()) {
      fCurrentNoteElement->push (
        createMxmlElement (
          k_accidental,
          accidentalString));
    }

    // append the stem if any
    appendStemToNote (theMsrNote);

    // append the staff if any
    appendStaffToNoteIfRelevant (theMsrNote);

    // append the beams if any
    appendBeamsToNote (theMsrNote);

    // append the articulations if any
    appendNoteNotationsToNote (theMsrNote);

    // append the lyrics if any
    appendNoteLyricsToNote (theMsrNote);

    // append theMsrNote element to the current measure element right now,
    // unless it contains a grace notes group
    S_msrGraceNotesGroup
      noteGraceNotesGroupBefore =
        theMsrNote->
          getNoteGraceNotesGroupBefore (),
      noteGraceNotesGroupAfter =
        theMsrNote->
          getNoteGraceNotesGroupAfter ();

    if (! (noteGraceNotesGroupBefore || noteGraceNotesGroupAfter)) {
      if (gGlobalMsr2mxmlTreeOahGroup->getMusicXMLComments ()) {
        // create a note comment
        S_msrVoice
          noteVoice =
            theMsrNote->fetchNoteVoiceUpLink ();

        stringstream s;
        s <<
          " ===== " <<
          "Note " <<
          theMsrNote->notePitchAndSoundingWholeNotesAsString () <<
          ", staff: " <<
            noteVoice->getVoiceStaffUpLink ()->getStaffNumber () <<
          ", voice: " <<
            noteVoice->getVoiceNumber () <<
          ", position: " <<
            theMsrNote->getMeasureElementPositionInMeasure () <<
          ", moment: " <<
            theMsrNote->getMeasureElementMomentInMeasure () <<
          ", sounding: " <<
            theMsrNote->getNoteSoundingWholeNotes () <<
          ", line " << inputLineNumber <<
          " ===== ";
        Sxmlelement comment = createMxmlElement (kComment, s.str ());

        // append it to the current measure element
        appendOtherToMeasure (comment);
      }

      // append theMsrNote to the current measure element
      appendNoteToMeasure (
        fCurrentNoteElement,
        theMsrNote);
    }
    else {
      fCurrentNoteAwaitsGraceNotes = true;
      fPendingMsrNoteAwaitingGraceNotes = theMsrNote;
      fPendingNoteElement = fCurrentNoteElement;
    }
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrGraceNotesGroup& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber () ;

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrGraceNotesGroup" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (gGlobalMsr2mxmlTreeOahGroup->getMusicXMLComments ()) {
    // create a start comment
    stringstream s;
    s <<
      " ==================== " <<
      "Grace notes group " <<
      inputLineNumber <<
      " START" <<
        ", line " << inputLineNumber <<
      " ==================== ";
    Sxmlelement comment = createMxmlElement (kComment, s.str ());

    // append it to the current measure element
    fCurrentMeasureElement->push (comment);
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrGraceNotesGroup& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber () ;

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrGraceNotesGroup" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (gGlobalMsr2mxmlTreeOahGroup->getMusicXMLComments ()) {
    // create an end comment
    stringstream s;
    s <<
      " ==================== " <<
      "Grace notes group " <<
      inputLineNumber <<
      " END" <<
        ", line " << inputLineNumber <<
      " ==================== ";
    Sxmlelement comment = createMxmlElement (kComment, s.str ());

    // append it to the current measure element
    fCurrentMeasureElement->push (comment);
  }

  // append the note element to the current measure element only now
  // if it contains a grace notes group
  /*
  S_msrGraceNotesGroup
    noteGraceNotesGroupBefore =
      theMsrNote->getNoteGraceNotesGroupBefore (),
    noteGraceNotesGroupAfter =
      theMsrNote->getNoteGraceNotesGroupAfter ();
*/
  if (fCurrentNoteAwaitsGraceNotes) {
    appendNoteToMeasure (
      fPendingNoteElement,
      fPendingMsrNoteAwaitingGraceNotes);

    // forget about these after the pending grace notes
    fCurrentNoteAwaitsGraceNotes = false;
    fPendingMsrNoteAwaitingGraceNotes = nullptr;
    fPendingNoteElement = nullptr;
  }
}

/*
<!ELEMENT note
	(((grace, ((%full-note;, (tie, tie?)?) | (cue, %full-note;))) |
	  (cue, %full-note;, duration) |
	  (%full-note;, duration, (tie, tie?)?)),
	 instrument?, %editorial-voice;, type?, dot*,
	 accidental?, time-modification?, stem?, notehead?,
	 notehead-text?, staff?, beam*, notations*, lyric*, play?)>
*/

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrNote& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrNote '" <<
      elt->asString () <<
      "'" <<
      ", line " << inputLineNumber <<
      endl;

    if (false) { // JMI TEMP
      gLogStream <<
        endl <<
        "--> note elt:" <<
        endl <<
        elt <<
        endl << endl;
    }
  }
#endif

  // append a backup or forward sub-element if needed
  appendABackupOrForwardToMeasureIfNeeded (elt);

  // populate the measure attributes if needed
  if (fAnAttributeElementIsNeeded) {
    createMxmlAttributesElementAndAppendItToMeasure ();

    fAnAttributeElementIsNeeded = false;
  }

  // append the note directions to the note element
  populateNoteDirections (elt);

  // append the note element to the measure element if relevant
  appendMsrNoteToMesureIfRelevant (elt);

  fANoteHasBeenMetInCurrentMeasure = true;
}

void msr2mxmlTreeTranslator::visitEnd (S_msrNote& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrNote " <<
      elt->asString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // remember this note and its voice and staff if relevant
  // skips are just taken into account in fCurrentCumulatedSkipsDurations,
  // and a <backup /> or <forward /> is generated upon the next non-skip note

  bool doRememberThisNote = false;

  switch (elt->getNoteKind ()) {
    case k_NoNote:
      break;

    case kNoteRestInMeasure:
      doRememberThisNote = true;
      break;

    case kNoteSkipInMeasure:
      break;

    case kNoteUnpitchedInMeasure:
      doRememberThisNote = true;
      break;

    case kNoteRegularInMeasure:
    case kNoteRegularInChord:
    case kNoteRegularInTuplet:
    case kNoteRestInTuplet:
      doRememberThisNote = true;
      break;

    case kNoteInDoubleTremolo:
      doRememberThisNote = true; // JMI ???
      break;

    case kNoteRegularInGraceNotesGroup:
    case kNoteSkipInGraceNotesGroup:
      break;

    case kNoteInChordInGraceNotesGroup:
      break;

    case kNoteInTupletInGraceNotesGroup:
      break;

    case kNoteUnpitchedInTuplet:
      doRememberThisNote = true;
      break;
  } // switch

  if (doRememberThisNote) {
#ifdef TRACING_IS_ENABLED
    if (
      gGlobalMusicxmlOahGroup->getTraceBackup ()
        ||
      gGlobalMusicxmlOahGroup->getTraceForward ()
    ) {
      gLogStream <<
        "--> remembering note " <<
        elt->asString () <<
        " as previous note" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    fPreviousMSRNote = elt;
    fPreviousMSRNoteVoice = fPreviousMSRNote->fetchNoteVoiceUpLink ();
    fPreviousMSRNoteStaff = fPreviousMSRNoteVoice->getVoiceStaffUpLink ();
  }

  // forget about the note element
  fCurrentNoteElement = nullptr;

  // forget about the note notations element
  fCurrentNoteNotationsElement              = nullptr;
  fCurrentNoteNotationsOrnamentsElement     = nullptr;
  fCurrentNoteNotationsArticulationsElement = nullptr;
  fCurrentNoteNotationsTechnicalsElement    = nullptr;
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrBarline& elt)
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

  string barLStyleString;

  switch (elt->getBarlineStyleKind ()) {
    case msrBarline::kBarlineStyleNone:
      break;
    case msrBarline::kBarlineStyleRegular:
      barLStyleString = "regular";
      break;
    case msrBarline::kBarlineStyleDotted:
      barLStyleString = "dotted";
      break;
    case msrBarline::kBarlineStyleDashed:
      barLStyleString = "dashed";
      break;
    case msrBarline::kBarlineStyleHeavy:
      barLStyleString = "heavy";
      break;
    case msrBarline::kBarlineStyleLightLight:
      barLStyleString = "light-light";
      break;
    case msrBarline::kBarlineStyleLightHeavy:
      barLStyleString = "light-heavy";
      break;
    case msrBarline::kBarlineStyleHeavyLight:
      barLStyleString = "heavy-light";
      break;
    case msrBarline::kBarlineStyleHeavyHeavy:
      barLStyleString = "heavy-heavy";
      break;
    case msrBarline::kBarlineStyleTick:
      barLStyleString = "tick";
      break;
    case msrBarline::kBarlineStyleShort:
      barLStyleString = "short";
      break;
  } // switch

  if (barLStyleString.size ()) {
    // create the bar style element
    Sxmlelement barStyleElement = createMxmlElement (k_bar_style, barLStyleString);

    // create the barline element
    Sxmlelement barlineElement = createMxmlElement (k_barline, "");

    // set its "location" attribute if any
    msrBarline::msrBarlineLocationKind
      barlineLocationKind =
        elt->getLocationKind ();

    string barLineLocationString;

    switch (barlineLocationKind) {
      case msrBarline::kBarlineLocationNone:
        break;
      case msrBarline::kBarlineLocationLeft:
        barLineLocationString = "left";
        break;
      case msrBarline::kBarlineLocationMiddle:
        barLineLocationString = "middle";
        break;
      case msrBarline::kBarlineLocationRight:
        barLineLocationString = "right";
        break;
    } // switch

    if (barLineLocationString.size ()) {
      barlineElement->add (createMxmlAttribute ("location", barLineLocationString));
    }

    // append the barline element to the barline element
    barlineElement->push (barStyleElement);

    // append the barline element to the measure element
    appendOtherToMeasure (barlineElement);
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrBarline& elt)
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


/*
//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrStaffLinesNumber& elt)
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
      createMxmlStaffLinesNumberNewbornClone ();
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrStaffTuning& elt)
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
      createMxmlStaffTuningNewbornClone ();
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrStaffDetails& elt)
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

void msr2mxmlTreeTranslator::visitEnd (S_msrStaffDetails& elt)
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
*/

/*
//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrVoiceStaffChange& elt)
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
        createMxmlStaffChangeNewbornClone ();

  // append it to the current voice clone
  fCurrentVoiceClone->
    appendVoiceStaffChangeToVoice (
      voiceStaffChangeClone);
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrHarmony& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
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
      createMxmlHarmonyNewbornClone (
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
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  fOnGoingHarmony = true;
}

void msr2mxmlTreeTranslator::visitStart (S_msrHarmonyDegree& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
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

void msr2mxmlTreeTranslator::visitEnd (S_msrHarmony& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrFrame& elt)
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
      "frame '" << elt->asShortString () <<
      "' is out of context, cannot be appendd";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrFiguredBass& elt)
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
      createMxmlFiguredBassNewbornClone (
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
      "' is out of context, cannot be append";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxmlTreeTranslator::visitStart (S_msrFigure& elt)
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

void msr2mxmlTreeTranslator::visitEnd (S_msrFiguredBass& elt)
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
*/

/*
//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrStanza& elt)
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

  ++gIndenter;

//  if (elt->getStanzaTextPresent ()) { // JMI
    fCurrentStanzaClone =
      elt->createStanzaNewbornClone (
        fCurrentVoiceClone);

    // append the stanza clone to the LPSR score elements list
    fResultingMusicxmlElement->
      appendStanzaToLpsrScoreElementsList (
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

void msr2mxmlTreeTranslator::visitEnd (S_msrStanza& elt)
{
  --gIndenter;

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
void msr2mxmlTreeTranslator::visitStart (S_msrSyllable& elt)
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
              k_NoPlacement,                // default value
              wordsValue,
              kJustifyNone,                  // default value
              kHorizontalAlignmentNone,      // default value
              kVerticalAlignmentNone,        // default value
              kFontStyleNone,                // default value
              msrFontSize::create (
                kFontSizeNone), // default value
              kFontWeightNone,               // default value
              kXMLLangIt,                    // default value
              elt->getSyllableNoteUpLink ()->getNoteStaffNumber ());

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
      "syllable '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
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

void msr2mxmlTreeTranslator::visitEnd (S_msrSyllable& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrTranspose& elt)
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

void msr2mxmlTreeTranslator::visitEnd (S_msrTranspose& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrPartNameDisplay& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrPartAbbreviationDisplay& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrRehearsal& elt)
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

void msr2mxmlTreeTranslator::visitEnd (S_msrRehearsal& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrFermata& elt)
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
  else {
    stringstream s;

    s <<
      "fermata '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrArpeggiato& elt)
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
  else {
    stringstream s;

    s <<
      "arpeggiato '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrNonArpeggiato& elt)
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
      "nonArpeggiato '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrTechnical& elt)
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
      "technical '" << elt->asShortString () <<
      "' is out of context, cannot be append";

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
      fResultingMusicxmlElement->
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
      fResultingMusicxmlElement->
        // this score needs the 'tongue' Scheme function
        setTongueSchemeFunctionIsNeeded ();
      break;
    case msrTechnical::kUpBow:
      break;
  } // switch
}

void msr2mxmlTreeTranslator::visitEnd (S_msrTechnical& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrTechnicalWithInteger& elt)
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
      "technicalWithInteger '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrTechnicalWithInteger& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrTechnicalWithFloat& elt)
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
      "technicalWithFloat '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrTechnicalWithFloat& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrTechnicalWithString& elt)
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
      "technicalWithString '" << elt->asShortString () <<
      "' is out of context, cannot be append";

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
      fResultingMusicxmlElement->
        setAfterSchemeFunctionIsNeeded ();
      break;
    default:
      ;
  } // switch
}

void msr2mxmlTreeTranslator::visitEnd (S_msrTechnicalWithString& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrOrnament& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
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
      "' is out of context, cannot be append";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrOrnament& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrOrnament" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrGlissando& elt)
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
      "glissando '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  if (elt->getGlissandoTextValue ().size ()) {
    fResultingMusicxmlElement->
      // this score needs the 'glissandoWithText' Scheme functions
      addGlissandoWithTextSchemeFunctionsToScore ();
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrGlissando& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrSlide& elt)
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
      "slide '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrSlide& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrSingleTremolo& elt)
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
      "singleTremolo '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrSingleTremolo& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrDoubleTremolo& elt)
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
/ * JMI
    elt->createDoubleTremoloNewbornClone (
      elt->getDoubleTremoloFirstElement ()->
        createMxmlNewBornClone (),
      elt->getDoubleTremoloSecondElement ()
        createMxmlNewBornClone ());
        * /

  fOnGoingDoubleTremolo = true;
}

void msr2mxmlTreeTranslator::visitEnd (S_msrDoubleTremolo& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrDynamics& elt)
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
      case kDynamicsFFFFF:
      case kDynamicsFFFFFF:
      case kDynamicsPPPPP:
      case kDynamicsPPPPPP:
      case kDynamicsRF:
      case kDynamicsSFPP:
      case kDynamicsSFFZ:
      case k_NoDynamics:
        knownToLilyPondNatively = false;

      default:
        ;
    } // switch

    if (! knownToLilyPondNatively) {
      // this score needs the 'dynamics' Scheme function
      fResultingMusicxmlElement->
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
      "' is out of context, cannot be append";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrDynamics& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrOtherDynamics& elt)
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
      "otherDynamics '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  fResultingMusicxmlElement->
    // this score needs the 'otherDynamic' Scheme function
    setOtherDynamicSchemeFunctionIsNeeded ();
}

void msr2mxmlTreeTranslator::visitEnd (S_msrOtherDynamics& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrWords& elt)
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
    bool wordsHasBeenappendd = false;

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

      wordsHasBeenappendd = true;
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

      wordsHasBeenappendd = true;
    }

    else {
    / * JMI
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

      wordsHasBeenappendd = true;
      }
      * /
    }

    if (! wordsHasBeenappendd) {
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
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrWords& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrSlur& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
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
      case kNoteRegularInChord:
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
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrSlur& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSlur" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrLigature& elt)
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
      "ligature '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrLigature& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrSlash& elt)
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
      "slash '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrWedge& elt)
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
      "wedge '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrWedge& elt)
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

*/

/*
//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrOctaveShift& elt)
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
      "octaveShift '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrOctaveShift& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrAccordionRegistration& elt)
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

  // the generate code needs modules scm and accreg
  fResultingMusicxmlElement->
    setScmAndAccregSchemeModulesAreNeeded ();
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrHarpPedalsTuning& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrStem& elt)
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
    fCurrentChordClone->
      appendStemToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "stem '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrStem& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrBeam& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
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

void msr2mxmlTreeTranslator::visitEnd (S_msrBeam& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrTie& elt)
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
      "tie '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxmlTreeTranslator::visitEnd (S_msrTie& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrSegno& elt)
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
      "segno '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxmlTreeTranslator::visitStart (S_msrDalSegno& elt)
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
      "dal segno '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxmlTreeTranslator::visitStart (S_msrCoda& elt)
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
      "coda '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrEyeGlasses& elt)
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

void msr2mxmlTreeTranslator::visitStart (S_msrScordatura& elt)
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

void msr2mxmlTreeTranslator::visitStart (S_msrPedal& elt)
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

void msr2mxmlTreeTranslator::visitStart (S_msrDamp& elt)
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

  fResultingMusicxmlElement->
    // this score needs the 'custom short barline' Scheme function
    setDampMarkupIsNeeded ();
}

void msr2mxmlTreeTranslator::visitStart (S_msrDampAll& elt)
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

  fResultingMusicxmlElement->
    // this score needs the 'custom short barline' Scheme function
    setDampAllMarkupIsNeeded ();
}

//________________________________________________________________________
void msr2mxmlTreeTranslator::visitStart (S_msrBarCheck& elt)
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

void msr2mxmlTreeTranslator::visitEnd (S_msrBarCheck& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrBarNumberCheck& elt)
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

void msr2mxmlTreeTranslator::visitEnd (S_msrBarNumberCheck& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrLineBreak& elt)
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

void msr2mxmlTreeTranslator::visitEnd (S_msrLineBreak& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrPageBreak& elt)
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

void msr2mxmlTreeTranslator::visitEnd (S_msrPageBreak& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrRepeat& elt)
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

void msr2mxmlTreeTranslator::visitEnd (S_msrRepeat& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrRepeatCommonPart& elt)
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

void msr2mxmlTreeTranslator::visitEnd (S_msrRepeatCommonPart& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrRepeatEnding& elt)
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

void msr2mxmlTreeTranslator::visitEnd (S_msrRepeatEnding& elt)
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
void msr2mxmlTreeTranslator::visitStart (S_msrRestMeasures& elt)
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

  ++gIndenter;

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

void msr2mxmlTreeTranslator::visitEnd (S_msrRestMeasures& elt)
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

  --gIndenter;

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
void msr2mxmlTreeTranslator::visitStart (S_msrRestMeasuresContents& elt)
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

  ++gIndenter;

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

void msr2mxmlTreeTranslator::visitEnd (S_msrRestMeasuresContents& elt)
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

  --gIndenter;

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
void msr2mxmlTreeTranslator::visitStart (S_msrMeasuresRepeat& elt)
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

  ++gIndenter;

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

void msr2mxmlTreeTranslator::visitEnd (S_msrMeasuresRepeat& elt)
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

  --gIndenter;

/ * JMI
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
* /

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
void msr2mxmlTreeTranslator::visitStart (S_msrMeasuresRepeatPattern& elt)
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

  ++gIndenter;

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

void msr2mxmlTreeTranslator::visitEnd (S_msrMeasuresRepeatPattern& elt)
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

  --gIndenter;

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
void msr2mxmlTreeTranslator::visitStart (S_msrMeasuresRepeatReplicas& elt)
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

  ++gIndenter;

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

void msr2mxmlTreeTranslator::visitEnd (S_msrMeasuresRepeatReplicas& elt)
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

  --gIndenter;


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
void msr2mxmlTreeTranslator::visitStart (S_msrMidiTempo& elt)
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

void msr2mxmlTreeTranslator::visitEnd (S_msrMidiTempo& elt)
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
*/

} // namespace


/*
//________________________________________________________________________
void msr2mxmlTreeTranslator::displayCurrentOnGoingValues ()
{
  gLogStream <<
    "Ongoing value:" <<
    endl;

  ++gIndenter;

  const unsigned int fieldWidth = 25;

  gLogStream <<
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

  --gIndenter;
}
*/

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

  public:

    // public services
    // ------------------------------------------------------

		bool	                operator()	(Sxmlelement a, Sxmlelement b);

  private:

    // private fields
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


/*
  if       (timeSymbol == "common") {
    fCurrentTimeSymbolKind = kTimeSymbolCommon;
  }
  else  if (timeSymbol == "cut") {
    fCurrentTimeSymbolKind = kTimeSymbolCut;
  }
  else  if (timeSymbol == "note") {
    fCurrentTimeSymbolKind = kTimeSymbolNote;
  }
  else  if (timeSymbol == "dotted-note") {
    fCurrentTimeSymbolKind = kTimeSymbolDottedNote;
  }
  else  if (timeSymbol == "single-number") {
    fCurrentTimeSymbolKind = kTimeSymbolSingleNumber;
  }

  // handle the time
  if (timeSymbolKind == kTimeSymbolSenzaMisura) {
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
        case kTimeSymbolCommon:
          break;
        case kTimeSymbolCut:
          break;
        case kTimeSymbolNote:
          break;
        case kTimeSymbolDottedNote:
          break;
        case kTimeSymbolSingleNumber:
          fOutputStream <<
            "\\once\\override Staff.TimeSignature.style = #'single-digit" <<
            endl;
          break;
        case kTimeSymbolSenzaMisura:
          break;
        case kTimeSymbolNone:
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
          timeSymbolKind == kTimeSymbolNone
            ||
          gGlobalLpsr2lilypondOahGroup->getNumericalTime ()) {
          fOutputStream <<
            "\\numericTimeSignature ";
        }

        fOutputStream <<
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

        fOutputStream <<
          "\\compoundMeter #`(";

        // handle all the time items in the vector
        for (int i = 0; i < timesItemsNumber; ++i) {
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
          fOutputStream <<
            "(";

          // then generate all beats numbers in the vector
          for (int j = 0; j < beatsNumbersNumber; ++j) {
            fOutputStream <<
              beatsNumbersVector [j] <<
              ' ';
          } // for

          // then generate the beat type
          fOutputStream <<
            timeItem->getTimeBeatValue ();

          // and finally generate the closing parenthesis
          fOutputStream <<
            ")";

          if (i != timesItemsNumber - 1) {
            fOutputStream <<
              ' ';
          }
        } // for

      fOutputStream <<
        ")" <<
        endl;
      }
    }

    else {
      // there are no time items
      if (timeSymbolKind != kTimeSymbolSenzaMisura) {
        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          elt->getInputLineNumber (),
          __FILE__, __LINE__,
          "time items vector is empty");
      }
    }
  }
*/

