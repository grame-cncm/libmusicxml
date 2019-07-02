/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <climits>      // INT_MIN, INT_MAX

#include "msrMutualDependencies.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "musicXMLOptions.h"
#include "msrOptions.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
int msrMeasure::gMeasureDebugNumber = 0;

S_msrMeasure msrMeasure::create (
  int          inputLineNumber,
  string       measureNumber,
  S_msrSegment measureSegmentUpLink)
{
  msrMeasure* o =
    new msrMeasure (
      inputLineNumber,
      measureNumber,
      measureSegmentUpLink);
  assert(o!=0);

  return o;
}

msrMeasure::msrMeasure (
  int          inputLineNumber,
  string       measureNumber,
  S_msrSegment measureSegmentUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert(
    measureSegmentUpLink != nullptr,
    "measureSegmentUpLink is null");

  // set measure's segment upLink
  fMeasureSegmentUpLink =
    measureSegmentUpLink;

  // set measure numbers
  fMeasureNumber = measureNumber;
  fMeasurePuristNumber = -1; // default irrealist value
  fNextMeasureNumber = "";

  // set debug number
  fMeasureDebugNumber = ++gMeasureDebugNumber;

  // do other initializations
  initializeMeasure ();
}

void msrMeasure::initializeMeasure ()
{
  S_msrVoice
    voiceUpLink =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Initializing measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in segment '" <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      voiceUpLink->getVoiceName () <<
      "\"" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceRepeats) {
    if (false && fMeasureDebugNumber == 3) { // JMI
      gLogIOstream <<
        endl <<
        "======================= initializeMeasure()" <<
        endl <<

        fMeasureSegmentUpLink->
          getSegmentVoiceUpLink () <<

        "=======================" <<
        endl <<
        endl;

      abort ();
    }
  }
#endif

  // measure kind
  fMeasureKind = kMeasureKindUnknown;

  // measure 'first in segment' kind
  fMeasureFirstInSegmentKind = kMeasureFirstInSegmentKindUnknown;

  // measure 'first in voice'
  fMeasureFirstInVoice = false; // default value

  // single-measure rest?
  fMeasureIsASingleMeasureRest = false;

  // initialize measure whole notes
  setCurrentMeasureWholeNotes (
    fInputLineNumber,
    rational (0, 1)); // ready to receive the first note

  // fetch the staff
  S_msrStaff
    staffUpLink =
      voiceUpLink->
        getVoiceStaffUpLink ();

  // get the staff time
  S_msrTime
    staffCurrentTime =
      staffUpLink->
        getStaffCurrentTime ();

  // set the measure full length if relevant
  if (staffCurrentTime) {
    setFullMeasureWholeNotesFromTime (
      staffCurrentTime);
  }

  // measure doesn't contain music yet
  fMeasureContainsMusic = false;

  // regular measure ends detection
  fMeasureEndRegularKind = kMeasureEndRegularKindUnknown;

  // repeat context
  fMeasureRepeatContextKind = msrMeasure::kMeasureRepeatContextKindUnknown;

  // measure finalization
  fMeasureHasBeenFinalized = false;
  fMeasureKindAndPuristNumberHaveBeenDetermined = false;
}

msrMeasure::~msrMeasure ()
{}

S_msrPart msrMeasure::fetchMeasurePartUpLink () const
{
  return
    fMeasureSegmentUpLink->
      fetchSegmentPartUpLink ();
}

S_msrVoice msrMeasure::fetchMeasureVoiceUpLink () const
{
  return
    fMeasureSegmentUpLink->
      getSegmentVoiceUpLink ();
}

S_msrMeasure msrMeasure::createMeasureNewbornClone (
  S_msrSegment containingSegment)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Creating a newborn clone of measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "'" <<
      " in segment " <<
      containingSegment->asString () <<
      " in voice \"" <<
        containingSegment->
          getSegmentVoiceUpLink ()->
            getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingSegment != nullptr,
    "containingSegment is null");

  // create newborn clone
  S_msrMeasure
    newbornClone =
      msrMeasure::create (
        fInputLineNumber,
        fMeasureNumber,
        containingSegment);

  // lengthes
  newbornClone->fFullMeasureWholeNotes =
    fFullMeasureWholeNotes;

  // next measure number
  newbornClone->fNextMeasureNumber =
    fNextMeasureNumber;

    /* JMI
  // measure purist number
  newbornClone->fMeasurePuristNumber = // JMI don't compute that again for clones ???
    fMeasurePuristNumber;

  // measure 'first in segment' kind
  newbornClone->fMeasureFirstInSegmentKind =
    fMeasureFirstInSegmentKind;
    */

/* JMI
  // single-measure rest?
  newbornClone->fMeasureIsASingleMeasureRest =
    fMeasureIsASingleMeasureRest;

  // regular measure ends detection // JMI TEMP
  newbornClone->fMeasureEndRegularKind =
    fMeasureEndRegularKind;
*/

  /*
  // fCurrentMeasureWholeNotes and fFullMeasureWholeNotes
  // will be computed on the fly
  // when appending notes to the measure newborn clone

  // measure kind
  newbornClone->fMeasureKind =
    fMeasureKind;
*/

  // chords handling

  // elements

  // upLinks

  return newbornClone;
}

S_msrMeasure msrMeasure::createMeasureDeepCopy (
  S_msrSegment containingSegment)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Creating a deep copy of measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "'" <<
      " in segment " <<
      containingSegment->asString () <<
      " in voice \"" <<
        containingSegment->
          getSegmentVoiceUpLink ()->
            getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingSegment != nullptr,
    "containingSegment is null");

  // create deep copy
  S_msrMeasure
    measureDeepCopy =
      msrMeasure::create (
        fInputLineNumber,
        fMeasureNumber,
        containingSegment);

  // lengthes
  measureDeepCopy->fFullMeasureWholeNotes =
    fFullMeasureWholeNotes;

  measureDeepCopy->fCurrentMeasureWholeNotes =
    fCurrentMeasureWholeNotes;

  // measure kind
  measureDeepCopy->fMeasureKind =
    fMeasureKind;

  // next measure number
  measureDeepCopy->fNextMeasureNumber =
    fNextMeasureNumber;

  // measure 'first in segment' kind
  measureDeepCopy->fMeasureFirstInSegmentKind =
    fMeasureFirstInSegmentKind;

  // elements

  int numberOfMeasureElements =
    fMeasureElementsList.size ();

  if (numberOfMeasureElements) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
        singularOrPluralWithoutNumber (
          numberOfMeasureElements, "There is", "There are") <<
        " " <<
        singularOrPlural (
          numberOfMeasureElements, "element", "elements") <<
        " to be deep copied in measure '" <<
        fMeasureNumber <<
        ", measureDebugNumber: '" <<
        fMeasureDebugNumber <<
        "' in segment " <<
        containingSegment->asString () <<
        " in voice \"" <<
          containingSegment->
            getSegmentVoiceUpLink ()->
              getVoiceName () <<
        "\"" <<
        endl;
    }
#endif

    for (
      list<S_msrMeasureElement>::const_iterator i = fMeasureElementsList.begin ();
      i != fMeasureElementsList.end ();
      i++ ) {
      S_msrMeasureElement element = (*i);

      // handlle deep copying
      S_msrMeasureElement
        elementDeepCopy;

      if (
        S_msrNote note = dynamic_cast<msrNote*>(&(*element))
        ) {
        // create the note deep copy
        elementDeepCopy =
          note->createNoteDeepCopy (
            fetchMeasureVoiceUpLink ());

/* JMI
        // append the element deep copy to the measure deep copy
        measureDeepCopy->
          appendElementToMeasure (
            elementDeepCopy);
*/
      }

      else if (
        S_msrTime time = dynamic_cast<msrTime*>(&(*element))
        ) {
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTimes || gTraceOptions->fTraceMeasures) {
          gLogIOstream <<
            "Sharing time '" <<
            time->asShortString () <<
            "' in measure '" <<
            fMeasureNumber <<
            ", measureDebugNumber: '" <<
            fMeasureDebugNumber <<
            "'deep copy" <<
            ", line " << fInputLineNumber <<
            endl;
        }
#endif

        // share the element with the original measure
        elementDeepCopy = time;
      }

      else {
        // share the element with the original measure
        elementDeepCopy = element;
      }

      // append the element deep copy to the measure deep copy
      measureDeepCopy->
        appendElementToMeasure (
          elementDeepCopy);
    } // for
  }

  else {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
        "There are no elements in measure to be deep copied" <<
        " in segment " <<
        containingSegment->asString () <<
        " in voice \"" <<
        containingSegment->
          getSegmentVoiceUpLink ()->
            getVoiceName () <<
        "\"" <<
        endl;
    }
#endif
  }

  // upLinks

  // fMeasureSegmentUpLink JMI ???

  return measureDeepCopy;
}

void msrMeasure::setMeasureEndRegularKind (
  msrMeasureEndRegularKind measureEndRegularKind)
{
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
        "Setting regular kind of measure '" <<
        fMeasureNumber <<
        "' to '" <<
        measureEndRegularKindAsString (measureEndRegularKind) <<
        "' in segment " <<
        fMeasureSegmentUpLink->asString () <<
        " in voice \"" <<
        fMeasureSegmentUpLink->
          getSegmentVoiceUpLink ()->
            getVoiceName () <<
        "\"" <<
        ", line " << fInputLineNumber <<
        endl;
    }
#endif

  fMeasureEndRegularKind = measureEndRegularKind;
}

void msrMeasure::setMeasureRepeatContextKind (
  msrMeasureRepeatContextKind measureRepeatContextKind)
{
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
        "Setting repeat context kind of measure '" <<
        fMeasureNumber <<
        "' to '" <<
        measureRepeatContextKindAsString (measureRepeatContextKind) <<
        "' in segment " <<
        fMeasureSegmentUpLink->asString () <<
        " in voice \"" <<
        fMeasureSegmentUpLink->
          getSegmentVoiceUpLink ()->
            getVoiceName () <<
        "\"" <<
        ", line " << fInputLineNumber <<
        endl;
    }
#endif

  fMeasureRepeatContextKind = measureRepeatContextKind;
}

void msrMeasure::setMeasurePuristNumber (
  int measurePuristNumber)
{
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
        "Setting purist number of measure '" <<
        fMeasureNumber <<
        "' to '" <<
        measurePuristNumber <<
        "' in segment " <<
        fMeasureSegmentUpLink->asString () <<
        " in voice \"" <<
        fMeasureSegmentUpLink->
          getSegmentVoiceUpLink ()->
            getVoiceName () <<
        "\"" <<
        ", line " << fInputLineNumber <<
        endl;
    }
#endif

  fMeasurePuristNumber = measurePuristNumber;
}

void msrMeasure::appendElementToMeasure (S_msrMeasureElement elem)
{
#ifdef TRACE_OPTIONS
  int inputLineNumber =
    elem->getInputLineNumber ();
#endif

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceMeasures
      ||
    gTraceOptions->fTracePositionsInMeasures
//      ||
//    gTraceOptions->fTraceForTests
  ) {
    gLogIOstream <<
      "Appending element " <<
      elem->asShortString () <<
      " to measure " <<
      asShortString () <<
      " in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()
          ->getVoiceName () <<
      "\", line " << inputLineNumber <<
      ", currentMeasureWholeNotes = " << fCurrentMeasureWholeNotes <<
// JMI          ", partCurrentMeasureWholeNotesHighTide = " << partCurrentMeasureWholeNotesHighTide <<
      endl;
  }
#endif

  if (fMeasurePendingMeasureElementsList.size ()) {
    // pad measure up to the elements positions in measure,
    // and then append them

#ifdef TRACE_OPTIONS
    // fetch the part measure whole notes high tide
    rational
      partCurrentMeasureWholeNotesHighTide =
        fetchMeasurePartUpLink ()->
          getPartCurrentMeasureWholeNotesHighTide ();
#endif

    list<S_msrMeasureElement>::iterator
      iBegin = fMeasurePendingMeasureElementsList.begin (),
      iEnd   = fMeasurePendingMeasureElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrMeasureElement
        measureElement = (*i);

#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceMeasures || gTraceOptions->fTracePositionsInMeasures) {
        gLogIOstream <<
          "Considering delayed '" <<
          measureElement->asShortString () <<
          "' in measure '" <<
          asShortString () <<
          "' in voice \"" <<
          fMeasureSegmentUpLink->
            getSegmentVoiceUpLink ()
              ->getVoiceName () <<
          "\", line " << inputLineNumber <<
          ", has position in measure '" <<
          measureElement->getPositionInMeasure () <<
          ", currentMeasureWholeNotes = " << fCurrentMeasureWholeNotes <<
          ", partCurrentMeasureWholeNotesHighTide = " << partCurrentMeasureWholeNotesHighTide <<
          endl;
      }
#endif

      if (measureElement->getPositionInMeasure () == fCurrentMeasureWholeNotes) { // JMI
        // this is where measureElement should be appended

#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceMeasures) {
          gLogIOstream <<
            "Appending delayed '" <<
            measureElement->asShortString () <<
            "' in measure '" <<
            asShortString () <<
            "' in voice \"" <<
            fMeasureSegmentUpLink->
              getSegmentVoiceUpLink ()
                ->getVoiceName () <<
            "\", line " << inputLineNumber <<
            ", has position in measure '" <<
            measureElement->getPositionInMeasure () <<
            ", currentMeasureWholeNotes = " << fCurrentMeasureWholeNotes <<
            ", partCurrentMeasureWholeNotesHighTide = " << partCurrentMeasureWholeNotesHighTide <<
            endl;
        }
#endif

        // set measureElement's position in measure
        measureElement->
          setPositionInMeasure (
            fCurrentMeasureWholeNotes,
            "appendElementToMeasure(), delayed element");

        fMeasureElementsList.push_back (measureElement);

        // remove it from the pending measure elements list
        i = fMeasurePendingMeasureElementsList.erase (i);
      }

      if (++i == iEnd) break;
    } // for
  }

  // set elem's measure number
  elem->
    setMeasureNumber (
      fMeasureNumber);

  // set elem's position in measure
  elem->
    setPositionInMeasure (
      fCurrentMeasureWholeNotes,
      "appendElementToMeasure()");

  fMeasureElementsList.push_back (elem);
}

void msrMeasure::insertElementInMeasureBeforeIterator (
  int                                 inputLineNumber,
  list<S_msrMeasureElement>::iterator iter,
  S_msrMeasureElement                 elem)
{
#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceMeasures
      ||
    gTraceOptions->fTracePositionsInMeasures
      ||
    gTraceOptions->fTraceForTests
  ) {
    gLogIOstream <<
      "Inserting element " <<
      elem->asShortString () <<
      /* JMI ???
      " before element " <<
      (*iter)->asShortString () <<
      */
      " in measure " <<
      asShortString () <<
      " in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()
          ->getVoiceName () <<
      "\", line " << inputLineNumber <<
      ", currentMeasureWholeNotes = " << fCurrentMeasureWholeNotes <<
 // JMI     ", partCurrentMeasureWholeNotesHighTide = " << partCurrentMeasureWholeNotesHighTide <<
      endl;
  }
#endif

  // set elem's measure number
  elem->
    setMeasureNumber (
      fMeasureNumber);

  // set elem's position in measure
  elem->
    setPositionInMeasure (
      fCurrentMeasureWholeNotes, // rational (999/1)); // JMI fCurrentMeasureWholeNotes)
      "insertElementInMeasureBeforeIterator()");

  // insert elem in the measure elements list before (*iter)
  fMeasureElementsList.insert (
    iter, elem);

  // account for elem's duration in current measure whole notes
  setCurrentMeasureWholeNotes (
    inputLineNumber,
    fCurrentMeasureWholeNotes + elem->getSoundingWholeNotes ());
}

void msrMeasure::appendElementAtTheEndOfMeasure (S_msrMeasureElement elem)
{
  int inputLineNumber =
    elem->getInputLineNumber ();

#ifdef TRACE_OPTIONS
  // fetch the part measure whole notes high tide
  rational
    partCurrentMeasureWholeNotesHighTide =
      fetchMeasurePartUpLink ()->
        getPartCurrentMeasureWholeNotesHighTide ();

    if (gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
        "Appending element '" <<
        elem->asShortString () <<
        "' at the end of measure '" <<
        asShortString () <<
        "' in voice \"" <<
        fMeasureSegmentUpLink->
          getSegmentVoiceUpLink ()
            ->getVoiceName () <<
        "\", line " << inputLineNumber <<
        ", has position in measure '" <<
        elem->getPositionInMeasure () <<
        ", currentMeasureWholeNotes = " << fCurrentMeasureWholeNotes <<
        ", partCurrentMeasureWholeNotesHighTide = " <<
        partCurrentMeasureWholeNotesHighTide <<
        endl;

    displayMeasure (
      inputLineNumber,
      "appendElementAtTheEndOfMeasure() 1");
    }
#endif

  if (fMeasurePendingMeasureElementsList.size ()) {
    // pad measure up to the elements positions in measure,
    // and then append them

#ifdef TRACE_OPTIONS
    // fetch the part measure whole notes high tide
    rational
      partCurrentMeasureWholeNotesHighTide =
        fetchMeasurePartUpLink ()->
          getPartCurrentMeasureWholeNotesHighTide ();
#endif

    list<S_msrMeasureElement>::iterator
      iBegin = fMeasurePendingMeasureElementsList.begin (),
      iEnd   = fMeasurePendingMeasureElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrMeasureElement
        measureElement = (*i);

#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceMeasures) {
        gLogIOstream <<
          "Considering delayed '" <<
          measureElement->asShortString () <<
          "' in measure '" <<
          asShortString () <<
          "' in voice \"" <<
          fMeasureSegmentUpLink->
            getSegmentVoiceUpLink ()
              ->getVoiceName () <<
          "\", line " << inputLineNumber <<
          ", has position in measure '" <<
          measureElement->getPositionInMeasure () <<
          ", currentMeasureWholeNotes = " << fCurrentMeasureWholeNotes <<
          ", partCurrentMeasureWholeNotesHighTide = " << partCurrentMeasureWholeNotesHighTide <<
          endl;
      }
#endif

      if (measureElement->getPositionInMeasure () == fCurrentMeasureWholeNotes) { // JMI
        // this is where measureElement should be appended

#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceMeasures) {
          gLogIOstream <<
            "Appending delayed '" <<
            measureElement->asShortString () <<
            "' in measure '" <<
            asShortString () <<
            "' in voice \"" <<
            fMeasureSegmentUpLink->
              getSegmentVoiceUpLink ()
                ->getVoiceName () <<
            "\", line " << inputLineNumber <<
            ", has position in measure '" <<
            measureElement->getPositionInMeasure () <<
            ", currentMeasureWholeNotes = " << fCurrentMeasureWholeNotes <<
            ", partCurrentMeasureWholeNotesHighTide = " << partCurrentMeasureWholeNotesHighTide <<
            endl;
        }
#endif

        // set measureElement's position in measure
        measureElement->
          setPositionInMeasure (
            fCurrentMeasureWholeNotes,
            "appendElementAtTheEndOfMeasure(), delayed element");

        fMeasureElementsList.push_back (measureElement);

        // remove it from the pending measure elements list
        i = fMeasurePendingMeasureElementsList.erase (i);
      }

      if (++i == iEnd) break;
    } // for
  }

  // append elem to the measure elements lists,
  // but place it before barlines if any,
  // to avoid the latter appearing too early in harmony and figured bass measures,
  list<S_msrMeasureElement>::reverse_iterator
    iBegin = fMeasureElementsList.rbegin (),
    iEnd   = fMeasureElementsList.rend (),
    i      = iBegin;
  for ( ; ; ) {
    S_msrMeasureElement
      measureElement = (*i);

/* JMI
    // sanity check
    msrAssert (
      measureElement != nullptr,
      "measureElement is null");

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Reverse iteration on element '" <<
      measureElement <<
      endl;
  }
#endif
*/

    if (
        // barline item?
        S_msrBarline
          barline =
            dynamic_cast<msrBarline*>(&(*measureElement))
    ) {
    /* JMI
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceMeasures) {
        gLogIOstream <<
          "Element is a barline actually" <<
          endl;
      }
#endif
*/
    }
    else {
    /* JMI
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceMeasures) {
        gLogIOstream <<
          "Element is no barline" <<
          endl;
      }
#endif
*/
      break;
    }

    if (++i == iEnd) break;
  } // for

  // insert elem in the measure elements list before (*i)
  insertElementInMeasureBeforeIterator (
    inputLineNumber,
    (i).base (),
    elem);

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    displayMeasure (
      inputLineNumber,
      "appendElementAtTheEndOfMeasure() 2");
  }
#endif
}

void msrMeasure::setNextMeasureNumber (string nextMeasureNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Setting next measure number to '" << nextMeasureNumber <<
      "' in measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in segment '" <<
      fMeasureSegmentUpLink-> asString () <<
      /* JMI
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      */
      "', line " << fInputLineNumber <<
      endl;
  }
#endif

  fNextMeasureNumber = nextMeasureNumber;
}

void msrMeasure::setMeasureFirstInVoice ()
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Setting measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' as first measure in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      "' in segment '" <<
      fMeasureSegmentUpLink-> asString () <<
      "', line " << fInputLineNumber <<
      endl;
  }
#endif

  fMeasureFirstInVoice = true;
}

string msrMeasure::fullMeasureWholeNotesAsMSRString ()
{
  return
    wholeNotesAsMsrString (
      fInputLineNumber,
      fFullMeasureWholeNotes);
}

void msrMeasure::setCurrentMeasureWholeNotes (
  int      inputLineNumber,
  rational wholeNotes)
{
  // rationalise the measure whole notes
  rational
    rationalisedCurrentMeasureWholeNotes =
      wholeNotes;
  rationalisedCurrentMeasureWholeNotes.rationalise ();

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceWholeNotes
      ||
    gTraceOptions->fTracePositionsInMeasures
      ||
    gMusicXMLOptions->fTraceDivisions
  ) {
    gLogIOstream <<
      "Setting current whole notes of measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' to '"  <<
      rationalisedCurrentMeasureWholeNotes <<
      "'";

    if (
      rationalisedCurrentMeasureWholeNotes.getDenominator ()
        !=
      wholeNotes.getDenominator ()
    ) {
      gLogIOstream <<
        " (rationalised from '" << wholeNotes << "')";
    }

    gLogIOstream <<
      " in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  // set measure whole notes
  fCurrentMeasureWholeNotes = rationalisedCurrentMeasureWholeNotes;
}

string msrMeasure::currentMeasureWholeNotesAsMSRString ()
{
  return
    wholeNotesAsMsrString (
      fInputLineNumber,
      fCurrentMeasureWholeNotes);
}

void msrMeasure::setMeasureKind (
  msrMeasureKind measureKind)
{
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
        "Setting measure kind of measure '" <<
        fMeasureNumber <<
        "' to '" <<
        measureKindAsString (measureKind) <<
        "' in segment " <<
        fMeasureSegmentUpLink->asString () <<
        " in voice \"" <<
        fMeasureSegmentUpLink->
          getSegmentVoiceUpLink ()->
            getVoiceName () <<
        "\"" <<
        ", line " << fInputLineNumber <<
        endl;
    }
#endif

  fMeasureKind = measureKind;
}

void msrMeasure::appendClefToMeasure (S_msrClef clef)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceClefs || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending clef '" << clef->asString () <<
      "' to measure " <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      ", in voice \"" <<
      fetchMeasureVoiceUpLink ()->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // append it to the measure elements list
  appendElementToMeasure (clef);
}

void msrMeasure::appendKeyToMeasure (S_msrKey key)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceKeys || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending key '" << key->asString () <<
      "' to measure " <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      ", in voice \"" <<
      fetchMeasureVoiceUpLink ()->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // append it to the measure elements list
  appendElementToMeasure (key);
}

void msrMeasure::appendTimeToMeasure (S_msrTime time)
{
  // sanity check
  msrAssert(
    time != nullptr,
    "time is null");

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTimes || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending time '" <<
      endl;

    gIndenter++;

    gLogIOstream <<
      time;

    gIndenter--;

    gLogIOstream <<
      "' to measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;

  // append time to the measure elements list
  appendElementToMeasure (time);

  // set the measure whole notes per full measure
  setFullMeasureWholeNotesFromTime (
    time);

  gIndenter--;
}

void msrMeasure::appendTimeToMeasureClone (S_msrTime time)
{
  // sanity check
  msrAssert(
    time != nullptr,
    "time is null");

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTimes || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending time:" <<
      endl;

    gIndenter++;

    gLogIOstream <<
      time;

    gIndenter--;

    S_msrVoice
      voiceUpLink =
        fMeasureSegmentUpLink->
          getSegmentVoiceUpLink ();

    gLogIOstream <<
      "to measure clone '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in segment clone '" <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      voiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // append time to the measure elements list
  appendElementToMeasure (time);
}

void msrMeasure::setFullMeasureWholeNotesFromTime (
  S_msrTime time)
{
  // sanity check
  msrAssert(
    time != nullptr,
    "time is null");

#ifdef TRACE_OPTIONS
  if (
    gMusicXMLOptions->fTraceDivisions
      ||
    gTraceOptions->fTraceTimes
      ||
    gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Setting measure full measure whole notes from time:" <<
      endl;

    gIndenter++;

    gLogIOstream <<
      time;

    gIndenter--;

    gLogIOstream <<
      "for measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  switch (time->getTimeSymbolKind ()) {
    case msrTime::kTimeSymbolCommon:
    case msrTime::kTimeSymbolCut:
    case msrTime::kTimeSymbolNote:
    case msrTime::kTimeSymbolDottedNote:
    case msrTime::kTimeSymbolSingleNumber:
    case msrTime::kTimeSymbolNone:
      {
        // this measure is con misura

        rational
          wholeNotesPerMeasure =
            time->
              wholeNotesPerMeasure ();

#ifdef TRACE_OPTIONS
        if (
          gMusicXMLOptions->fTraceDivisions
            ||
          gTraceOptions->fTraceTimes
            ||
          gTraceOptions->fTraceMeasures) {
          gLogIOstream <<
            "There are " <<
            wholeNotesPerMeasure <<
            " whole note(s) per measure in time:" <<
            endl;

          gIndenter++;

          gLogIOstream <<
            time;

          gIndenter--;

          gLogIOstream <<
            "in measure '" <<
            fMeasureNumber << "'" <<
            ", measureDebugNumber: '" <<
            fMeasureDebugNumber <<
            ", line " << fInputLineNumber <<
            "' in voice \"" <<
            fMeasureSegmentUpLink->
              getSegmentVoiceUpLink ()->
                getVoiceName () <<
            "\"" <<
            endl;
        }
#endif

        // set full measure whole notes
        fFullMeasureWholeNotes =
          wholeNotesPerMeasure;

#ifdef TRACE_OPTIONS
        if (
          gMusicXMLOptions->fTraceDivisions
            ||
          gTraceOptions->fTraceTimes
            ||
          gTraceOptions->fTraceMeasures) {
          gLogIOstream <<
            "Measure '" <<
            fMeasureNumber <<
            ", measureDebugNumber: '" <<
            fMeasureDebugNumber <<
            "' in voice \"" <<
            fMeasureSegmentUpLink->
              getSegmentVoiceUpLink ()->
                getVoiceName () <<
            "\"" <<
            " has " <<
            fFullMeasureWholeNotes <<
            " full measure whole notes " <<
            endl;
        }
#endif
      }
      break;

    case msrTime::kTimeSymbolSenzaMisura:

      // this measure is senza misura

#ifdef TRACE_OPTIONS
      if (
        gMusicXMLOptions->fTraceDivisions
          ||
        gTraceOptions->fTraceTimes
          ||
        gTraceOptions->fTraceMeasures
      ) {
        gLogIOstream <<
          "Measure '" <<
          fMeasureNumber <<
          ", measureDebugNumber: '" <<
          fMeasureDebugNumber <<
          "' in voice \"" <<
          fMeasureSegmentUpLink->
            getSegmentVoiceUpLink ()->
              getVoiceName () <<
          "\"" <<
          " is senza misura" <<
          endl;
      }
#endif

      setMeasureKind (kMeasureKindCadenza);

      fFullMeasureWholeNotes =
        rational (INT_MAX, 1);
      break;
  } // switch

  gIndenter--;
}

void msrMeasure::appendTransposeToMeasure (
  S_msrTranspose transpose)
{
  // append it to the measure elements list
  appendElementToMeasure (transpose);
}

void msrMeasure::appendPartNameDisplayToMeasure (
  S_msrPartNameDisplay partNameDisplay)
{
  // append it to the measure elements list
  appendElementToMeasure (partNameDisplay);
}

void msrMeasure::appendPartAbbreviationDisplayToMeasure (
  S_msrPartAbbreviationDisplay partAbbreviationDisplay)
{
  // append it to the measure elements list
  appendElementToMeasure (partAbbreviationDisplay);
}

void msrMeasure::printMeasurePendingMeasureElementsList ()
{
  gLogIOstream <<
    endl <<
    "===> printMeasurePendingMeasureElementsList ()" <<
    endl;

  gIndenter++;

  if (fMeasurePendingMeasureElementsList.size ()) {
    list<S_msrMeasureElement>::const_iterator
      iBegin = fMeasurePendingMeasureElementsList.begin (),
      iEnd   = fMeasurePendingMeasureElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      gLogIOstream << (*i)->asShortString ();
      if (++i == iEnd) break;
      gLogIOstream << endl;
    } // for
  }
  else {
    gLogIOstream <<
      "none" <<
      endl;
  }

  gIndenter--;

  gLogIOstream <<
    "<===" <<
    endl <<
    endl;
}

void msrMeasure::appendBarlineToMeasure (S_msrBarline barline)
{
  int inputLineNumber =
    barline->getInputLineNumber ();

  /* JMI , a barline may appear anywhere}
  // the measure may have to be padded
  padUpToPositionInMeasure (
    inputLineNumber);
    */

  // register barline measure number
  barline->
    setMeasureNumber (
      fMeasureNumber);

  // register barline position in measure,
  barline->
    setPositionInMeasure ( // JMI
      fCurrentMeasureWholeNotes,
      "appendBarlineToMeasure()");

  // is there already a pending barline in this voice?
  if (fMeasurePendingMeasureElementsList.size () > 1) {
    stringstream s;

    s <<
      "should not have more than one pending measure elements in measure " << // JMI
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"";

//    msrInternalError (
    msrInternalWarning (
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
// JMI      __FILE__, __LINE__,
      s.str ());
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceBarLines || gTraceOptions->fTraceMeasures) {
    printMeasurePendingMeasureElementsList ();
  }
#endif

  // fetch the part measure whole notes high tide
  rational
    partCurrentMeasureWholeNotesHighTide =
      fetchMeasurePartUpLink ()->
        getPartCurrentMeasureWholeNotesHighTide ();

  if (true /* JMI */ || // TICINO
    fCurrentMeasureWholeNotes == partCurrentMeasureWholeNotesHighTide
  ) {
    // append barline to the measure elements list at once
    appendElementToMeasure (barline);
  }
  else {
    // delay barline handling until this measure reaches
    // the part measure whole notes high tide

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceBarLines || gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
        "Delaying insertion of barline '" <<
        barline->asShortString () <<
        "' in measure '" <<
        asShortString () <<
        "' in voice \"" <<
        fMeasureSegmentUpLink->
          getSegmentVoiceUpLink ()
            ->getVoiceName () <<
        "\", line " << inputLineNumber <<
        ", has position in measure '" <<
        barline->getPositionInMeasure () <<
        ", currentMeasureWholeNotes = " << fCurrentMeasureWholeNotes <<
        ", partCurrentMeasureWholeNotesHighTide = " << partCurrentMeasureWholeNotesHighTide <<
        endl;
    }
#endif

    fMeasurePendingMeasureElementsList.push_back (barline);
  }
}

void msrMeasure::prependBarlineToMeasure (S_msrBarline barline)
{
  // append it to the measure elements list
  fMeasureElementsList.push_front (barline);
}

void msrMeasure::appendSegnoToMeasure (S_msrSegno segno)
{
  // append it to the measure elements list
  appendElementToMeasure (segno);
}

void msrMeasure::appendCodaToMeasure (S_msrCoda coda)
{
  // append it to the measure elements list
  appendElementToMeasure (coda);
}

void msrMeasure::appendEyeGlassesToMeasure (
  S_msrEyeGlasses eyeGlasses)
{
  // append it to the measure elements list
  appendElementToMeasure (eyeGlasses);
}

void msrMeasure::appendPedalToMeasure (S_msrPedal pedal)
{
  // append it to the measure elements list
  appendElementToMeasure (pedal);
}

void msrMeasure::appendDampToMeasure (S_msrDamp damp)
{
  // append it to the measure elements list
  appendElementToMeasure (damp);
}

void msrMeasure::appendDampAllToMeasure (S_msrDampAll dampAll)
{
  // append it to the measure elements list
  appendElementToMeasure (dampAll);
}

void msrMeasure::appendBarCheckToMeasure (S_msrBarCheck barCheck)
{
  // append it to the measure elements list
  appendElementToMeasure (barCheck);
}

void msrMeasure::appendVoiceStaffChangeToMeasure (
  S_msrVoiceStaffChange voiceStaffChange)
{
  // append it to the measure elements list
  appendElementToMeasure (voiceStaffChange);
}

void msrMeasure::appendNoteToMeasure (S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending note '" << note->asShortString () <<
      "' to measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;

  // populate measure upLink
  note->
    setNoteMeasureUpLink (this);

  // append the note to the measure elements list
  appendElementToMeasure (note);

  // fetch note sounding whole notes
  rational noteSoundingWholeNotes =
    note->getNoteSoundingWholeNotes ();

  string noteSoundingWholeNotesAsMsrString =
    note->noteSoundingWholeNotesAsMsrString ();

  // account for note duration in measure whole notes
  setCurrentMeasureWholeNotes (
    inputLineNumber,
    fCurrentMeasureWholeNotes + noteSoundingWholeNotes);

  // update part measure whole notes high tide if need be
  fetchMeasurePartUpLink ()->
    updatePartCurrentMeasureWholeNotesHighTide (
      inputLineNumber,
      fCurrentMeasureWholeNotes);

/* JMI
  // determine whether the note occupies a full measure
  if (noteSoundingWholeNotes == fFullMeasureWholeNotes)
    note->
      setNoteOccupiesAFullMeasure ();
  */

  // fetch note harmony
  S_msrHarmony
    noteHarmony =
      note->getNoteHarmony ();

/*
  if (false && noteHarmony) { // JMI
    // set its position in measure // JMI
    noteHarmony->
      setPositionInMeasure (
        notePositionInMeasure);
  }
  */

  // register note as the last one in this measure
  fMeasureLastHandledNote = note;

  // is this note the longest one in this measure?
  if (! fMeasureLongestNote) {
    fMeasureLongestNote = note;
  }
  else {
    if (
      note->getNoteSoundingWholeNotes ()
        >
      fMeasureLongestNote->getNoteSoundingWholeNotes ()
    ) {
      fMeasureLongestNote = note;
    }

    if (
      note->getNoteDisplayWholeNotes ()
        >
      fMeasureLongestNote->getNoteSoundingWholeNotes ()
    ) {
      fMeasureLongestNote = note;
    }
  }

  // this measure contains music
  fMeasureContainsMusic = true;

  gIndenter--;
}

void msrMeasure::appendPaddingNoteAtTheEndOfMeasure (S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending padding note '" << note->asShortString () <<
      "' at the end of measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;

  // populate measure upLink
  note->
    setNoteMeasureUpLink (this);

  // append the note to the measure elements list
// JMI  // only now to make it possible to remove it afterwards
  // if it happens to be the first note of a chord
  appendElementAtTheEndOfMeasure (note);

  // fetch note sounding whole notes
  rational noteSoundingWholeNotes =
    note->getNoteSoundingWholeNotes ();

  string noteSoundingWholeNotesAsMsrString =
    note->noteSoundingWholeNotesAsMsrString ();

  // account for note duration in measure whole notes
  setCurrentMeasureWholeNotes (
    inputLineNumber,
    fCurrentMeasureWholeNotes + noteSoundingWholeNotes);

  // update part measure whole notes high tide if need be
  fetchMeasurePartUpLink ()->
    updatePartCurrentMeasureWholeNotesHighTide (
      inputLineNumber,
      fCurrentMeasureWholeNotes);

/* JMI
  // determine whether the note occupies a full measure
  if (noteSoundingWholeNotes == fFullMeasureWholeNotes)
    note->
      setNoteOccupiesAFullMeasure ();
  */

  // register note as the last one in this measure
  fMeasureLastHandledNote = note;

  // is this note the longest one in this measure?
  if (! fMeasureLongestNote) {
    fMeasureLongestNote = note;
  }
  else {
    if (
      note->getNoteSoundingWholeNotes ()
        >
      fMeasureLongestNote->getNoteSoundingWholeNotes ()
    ) {
      fMeasureLongestNote = note;
    }

    if (
      note->getNoteDisplayWholeNotes ()
        >
      fMeasureLongestNote->getNoteSoundingWholeNotes ()
    ) {
      fMeasureLongestNote = note;
    }
  }

  // this measure contains music
  fMeasureContainsMusic = true;

  gIndenter--;
}

void msrMeasure::appendNoteToMeasureClone (S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending note '" << note->asShortString () <<
      "' to measure clone '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in segment clone '" <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      "' in voice clone \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;

    /* JMI
  if (appendMeasureIfOverflow (inputLineNumber)) {
    // a new measure has been appended to the segment
    // append note to it via the segment
    fMeasureSegmentUpLink->
      appendNoteToSegment (note);
  }

  else {
  */
    // regular insertion in current measure

    // populate measure upLink
    note->setNoteMeasureUpLink (
      this);

    // append the note to the measure elements list
  // JMI  // only now to make it possible to remove it afterwards
    // if it happens to be the first note of a chord
    appendElementToMeasure (note);

    // fetch note sounding whole notes
    rational
      noteSoundingWholeNotes =
        note->getNoteSoundingWholeNotes ();

    // account for note duration in measure whole notes
    setCurrentMeasureWholeNotes (
      inputLineNumber,
      fCurrentMeasureWholeNotes + noteSoundingWholeNotes);

    // update part measure whole notes high tide if need be
    fetchMeasurePartUpLink ()->
      updatePartCurrentMeasureWholeNotesHighTide (
        inputLineNumber,
        fCurrentMeasureWholeNotes);

  /* JMI
    // determine whether the note occupies a full measure
    if (noteSoundingWholeNotes == fFullMeasureWholeNotes)
      note->
        setNoteOccupiesAFullMeasure ();
      */

    // register note as the last one in this measure
    fMeasureLastHandledNote = note;

  // this measure contains music
  fMeasureContainsMusic = true;
 // JMI }

  gIndenter--;
}

void msrMeasure::appendDoubleTremoloToMeasure (
  S_msrDoubleTremolo doubleTremolo)
{
  int inputLineNumber =
    doubleTremolo->getInputLineNumber ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTremolos || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending double tremolo '" <<
      doubleTremolo->asShortString () <<
      "' to measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;

  // register doubleTremolo measure number
  doubleTremolo->
    setDoubleTremoloMeasureNumber (
      fMeasureNumber);

  // register doubleTremolo measure position in measure
  doubleTremolo->
    setDoubleTremoloPositionInMeasure (
      fCurrentMeasureWholeNotes);

  // copy measure number to first note, that was created beforehand
  doubleTremolo->
    setDoubleTremoloMeasureNumber (
      fMeasureNumber);

  // copy measure position to first note, that was created beforehand
  doubleTremolo->
    setDoubleTremoloPositionInMeasure (
      fCurrentMeasureWholeNotes);

  // append the doubleTremolo to the measure elements list
  appendElementToMeasure (doubleTremolo);

  // fetch doubleTremolo sounding whole notes
  rational
    doubleTremoloSoundingWholeNotes =
      doubleTremolo->
        getDoubleTremoloSoundingWholeNotes ();

  // account for doubleTremolo duration in measure whole notes
  setCurrentMeasureWholeNotes (
    inputLineNumber,
    fCurrentMeasureWholeNotes + doubleTremoloSoundingWholeNotes);

  // update part measure whole notes high tide if need be
  fetchMeasurePartUpLink ()->
    updatePartCurrentMeasureWholeNotesHighTide (
      inputLineNumber,
      fCurrentMeasureWholeNotes);

  // determine if the doubleTremolo occupies a full measure
// XXL  JMI  if (doubleTremoloSoundingWholeNotes == fMeasureDivisionsPerWholeMeasure)
    // doubleTremolo->setDoubleTremoloOccupiesAFullMeasure ();

  // this measure contains music
  fMeasureContainsMusic = true;

  gIndenter--;
}

void msrMeasure::appendChordToMeasure (S_msrChord chord) // JMI XXL
{
  int inputLineNumber =
    chord->getInputLineNumber ();

  // populate measure upLink
  chord->setChordMeasureUpLink (this);

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceChords || gTraceOptions->fTraceMeasures
      ||
    gTraceOptions->fTracePositionsInMeasures || gTraceOptions->fTraceForTests
  ) {
    gLogIOstream <<
      "Appending chord '" << chord->asString () <<
      "' to measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // append the chord to the measure elements list
  appendElementToMeasure (chord);

  // fetch chord sounding whole notes
  rational
    chordSoundingWholeNotes =
      chord->
        getChordSoundingWholeNotes ();

  // account for chord duration in measure whole notes
  setCurrentMeasureWholeNotes (
    inputLineNumber,
    fCurrentMeasureWholeNotes + chordSoundingWholeNotes);

  // update part measure whole notes high tide
  fetchMeasurePartUpLink ()->
    updatePartCurrentMeasureWholeNotesHighTide (
      inputLineNumber,
      fCurrentMeasureWholeNotes);

  // determine if the chord occupies a full measure
// XXL  JMI  if (chordSoundingWholeNotes == fMeasureDivisionsPerWholeMeasure)
    // chord->setChordOccupiesAFullMeasure ();

  // this measure contains music
  fMeasureContainsMusic = true;
}

void msrMeasure::appendTupletToMeasure (S_msrTuplet tuplet)
{
  int inputLineNumber =
    tuplet->getInputLineNumber ();

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceTuplets || gTraceOptions->fTraceMeasures
      ||
    gTraceOptions->fTracePositionsInMeasures || gTraceOptions->fTraceForTests
  ) {
    gLogIOstream <<
      "Appending tuplet " << tuplet->asString () <<
      " to measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;

  // populate measure upLink
  tuplet->setTupletMeasureUpLink (this);

  // append the tuplet to the measure elements list
  appendElementToMeasure (tuplet);

  // fetch tuplet sousnding whole notes
  rational
    tupletSoundingWholeNotes =
      tuplet->
        getTupletSoundingWholeNotes ();

  if (! tuplet->getTupletTupletUpLink ()) {
    // we can now set the position in measure for all the tuplet members
    tuplet->
      setTupletMembersPositionInMeasure (
        fCurrentMeasureWholeNotes);
  }

  // account for tuplet duration in measure whole notes
  setCurrentMeasureWholeNotes (
    inputLineNumber,
    fCurrentMeasureWholeNotes + tupletSoundingWholeNotes);

  // update part measure whole notes high tide if need be
  fetchMeasurePartUpLink ()->
    updatePartCurrentMeasureWholeNotesHighTide (
      inputLineNumber,
      fCurrentMeasureWholeNotes);

  // this measure contains music
  fMeasureContainsMusic = true;

  gIndenter--;
}

void msrMeasure::appendHarmonyToMeasure (S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceMeasures
      ||
    gTraceOptions->fTracePositionsInMeasures || gTraceOptions->fTraceForTests
  ) {
    gLogIOstream <<
      "Appending harmony " << harmony->asString () <<
      " to measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\", currentMeasureWholeNotes = " << fCurrentMeasureWholeNotes <<
      endl;
  }
#endif

  // set S_msrHarmony's measure number
  harmony->
    setMeasureNumber (
      fMeasureNumber);

  // append the harmony to the measure elements list
  // DON'T call 'appendElementToMeasure (harmony)':
  // that would override harmony's position in measure,
  // which already has the correct value, thus:
  fMeasureElementsList.push_back (harmony);

  // fetch harmony sounding whole notes
  rational
    harmonySoundingWholeNotes =
      harmony->
        getHarmonySoundingWholeNotes ();

  // account for harmony duration in measure whole notes
  setCurrentMeasureWholeNotes (
    inputLineNumber,
    fCurrentMeasureWholeNotes + harmonySoundingWholeNotes);

  // update part measure whole notes high tide if need be
  fetchMeasurePartUpLink ()->
    updatePartCurrentMeasureWholeNotesHighTide (
      inputLineNumber,
      fCurrentMeasureWholeNotes);

  // this measure contains music
  fMeasureContainsMusic = true;
}

void msrMeasure::appendHarmonyToMeasureClone (S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();

  // regular insertion in current measure

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceMeasures
      ||
    gTraceOptions->fTracePositionsInMeasures || gTraceOptions->fTraceForTests
  ) {
    gLogIOstream <<
      "Appending harmony " << harmony->asString () <<
      " to measure clone '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in segment clone '" <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      "' in voice clone \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\", currentMeasureWholeNotes = " << fCurrentMeasureWholeNotes <<
      endl;
  }
#endif

  // append the harmony to the measure elements list
  appendElementToMeasure (harmony);

  // fetch harmony sounding whole notes
  rational
    harmonySoundingWholeNotes =
      harmony->
        getHarmonySoundingWholeNotes ();

  // account for harmony duration in measure whole notes
  setCurrentMeasureWholeNotes (
    inputLineNumber,
    fCurrentMeasureWholeNotes + harmonySoundingWholeNotes);

  // update part measure whole notes high tide if need be
  fetchMeasurePartUpLink ()->
    updatePartCurrentMeasureWholeNotesHighTide (
      inputLineNumber,
      fCurrentMeasureWholeNotes);

  // this measure contains music
  fMeasureContainsMusic = true;
}

void msrMeasure::appendFiguredBassToMeasure (
  S_msrFiguredBass figuredBass)
{
  int inputLineNumber =
    figuredBass->getInputLineNumber ();

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceFiguredBasses || gTraceOptions->fTraceMeasures
      ||
    gTraceOptions->fTracePositionsInMeasures || gTraceOptions->fTraceForTests
  ) {
    gLogIOstream <<
      "Appending figured bass '" << figuredBass->asString () <<
      "' to measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\", currentMeasureWholeNotes = " << fCurrentMeasureWholeNotes <<
      endl;
  }
#endif

  // append the harmony to the measure elements list
  appendElementToMeasure (figuredBass);

  // fetch harmony sounding whole notes
  rational
    figuredBassSoundingWholeNotes =
      figuredBass->
        getFiguredBassSoundingWholeNotes ();

  // account for harmony duration in measure whole notes
  setCurrentMeasureWholeNotes (
    inputLineNumber,
    fCurrentMeasureWholeNotes + figuredBassSoundingWholeNotes);

  // update part measure whole notes high tide if need be
  fetchMeasurePartUpLink ()->
    updatePartCurrentMeasureWholeNotesHighTide (
      inputLineNumber,
      fCurrentMeasureWholeNotes);

  // this measure contains music
  fMeasureContainsMusic = true;
}

void msrMeasure::appendFiguredBassToMeasureClone (
  S_msrFiguredBass figuredBass)
{
  int inputLineNumber =
    figuredBass->getInputLineNumber ();

  // regular insertion in current measure

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceFiguredBasses || gTraceOptions->fTraceMeasures
      ||
    gTraceOptions->fTracePositionsInMeasures || gTraceOptions->fTraceForTests
  ) {
    gLogIOstream <<
      "Appending figured bass '" << figuredBass->asString () <<
      "' to measure clone '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in segment clone '" <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      "' in voice clone \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\", currentMeasureWholeNotes = " << fCurrentMeasureWholeNotes <<
      endl;
  }
#endif

  // append the harmony to the measure elements list
  appendElementToMeasure (figuredBass);

  // fetch harmony sounding whole notes
  rational
    figuredBassSoundingWholeNotes =
      figuredBass->
        getFiguredBassSoundingWholeNotes ();

  // account for harmony duration in measure whole notes
  setCurrentMeasureWholeNotes (
    inputLineNumber,
    fCurrentMeasureWholeNotes + figuredBassSoundingWholeNotes);

  // update part measure whole notes high tide if need be
  fetchMeasurePartUpLink ()->
    updatePartCurrentMeasureWholeNotesHighTide (
      inputLineNumber,
      fCurrentMeasureWholeNotes);

  // this measure contains music
  fMeasureContainsMusic = true;
}

S_msrNote msrMeasure::createPaddingNoteForVoice (
  int        inputLineNumber,
  rational   duration,
  S_msrVoice voice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePositionsInMeasures || gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Creating a padding note for voice \"" <<
      voice->getVoiceName () <<
      "', duration = '" <<
      duration <<
      "\" in measure '" <<
      asShortString () <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  // create a rest or a skip depending on measureVoice kind
  S_msrNote paddingNote;

  switch (voice->getVoiceKind ()) {
    case msrVoice::kVoiceRegular:
      paddingNote =
        true // JMI ??? what criterion should be used?
          ? msrNote::createRestNote (
              inputLineNumber,
              fMeasureNumber,
              duration,
              duration,
              0, // dots number JMI ???
              voice->
                getVoiceStaffUpLink ()->getStaffNumber (),
              voice->
                getVoiceNumber ())
          : msrNote::createSkipNote (
              inputLineNumber,
              fMeasureNumber,
              duration,
              duration,
              0, // dots number JMI ???
              voice->
                getVoiceStaffUpLink ()->getStaffNumber (),
              voice->
                getVoiceNumber ());
      break;

    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
      paddingNote =
        msrNote::createSkipNote (
          inputLineNumber,
          fMeasureNumber,
          duration,
          duration,
          0, // dots number JMI ???
          voice->
            getVoiceStaffUpLink ()->getStaffNumber (),
          voice->
            getVoiceNumber ());
      break;
  } // switch

  return paddingNote;
}

void msrMeasure::padUpToCurrentMeasureWholeNotesInMeasure (
  int      inputLineNumber,
  rational wholeNotes)
{
  // sanity check
  msrAssert (
    wholeNotes.getNumerator () >= 0,
    "wholeNotes.getNumerator () is negative");

  // fetch the measure voice
  S_msrVoice
    measureVoice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceMeasures
      ||
    gTraceOptions->fTracePositionsInMeasures
      ||
    gTraceOptions->fTraceWholeNotes
  ) {
    this->print (gLogIOstream);

    gLogIOstream <<
      "Padding from current measure whole notes '" << fCurrentMeasureWholeNotes <<
      "' to '" << wholeNotes <<
      "' in measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in segment " <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      " in voice \"" <<
      measureVoice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  if (fCurrentMeasureWholeNotes < wholeNotes) {
    gIndenter++;

    // appending a padding rest or skip to this measure to reach wholeNotes
    rational
      missingDuration =
        wholeNotes - fCurrentMeasureWholeNotes;
    missingDuration.rationalise ();

    // create a rest or a skip depending on measureVoice kind
    S_msrNote
      paddingNote =
        createPaddingNoteForVoice (
          inputLineNumber,
          missingDuration,
          measureVoice);

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures || gMusicXMLOptions->fTraceDivisions) {
      gLogIOstream <<
        "Appending rest" << paddingNote->asString () <<
        " (missingDuration " << missingDuration <<
        " whole notes) to skip from length '" << fCurrentMeasureWholeNotes <<
        " to length '" << wholeNotes << "'"
        " in measure '" <<
        fMeasureNumber <<
        ", measureDebugNumber: '" <<
        fMeasureDebugNumber <<
        "in voice \"" <<
        measureVoice->getVoiceName () <<
        endl;
    }
#endif

    // append the rest to the measure elements list
    // only now to make it possible to remove it afterwards
    // if it happens to be the first note of a chord
    appendNoteToMeasure (paddingNote);

/* JMI
    // set this measure as being padded // JMI
    this->
      setMeasureCreatedForARepeatKind (
        msrMeasure::kMeasureKindCreatedForARepeatPadded);
    */

    // this measure contains music
    fMeasureContainsMusic = true;

    gIndenter--;
  }

  else if (fCurrentMeasureWholeNotes == wholeNotes) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
        "No need to pad from current measure whole notes '" <<
        fCurrentMeasureWholeNotes <<
        "' to '" <<
        wholeNotes <<
        "' since they are equal in measure '" <<
        fMeasureNumber <<
        ", measureDebugNumber: '" <<
        fMeasureDebugNumber <<
        "' in segment " <<
        fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
        " in voice \"" <<
        measureVoice->getVoiceName () <<
        "\", line " << inputLineNumber <<
        endl;
    }
#endif
  }

  else {
    measureVoice->print (gLogIOstream); // JMI
    this->print (gLogIOstream);

    stringstream s;

    s <<
        "Cannot pad from current measure whole notes '" <<
        fCurrentMeasureWholeNotes <<
        "' to '" <<
        wholeNotes <<
        "' since the former is larger than the latter in measure '" <<
        fMeasureNumber <<
        "', measureDebugNumber: '" <<
        fMeasureDebugNumber <<
        "' in segment " <<
        fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
        " in voice \"" <<
        measureVoice->getVoiceName () <<
        "\", line " << inputLineNumber;

//    msrMusicXMLError ( JMI
    msrMusicXMLWarning (
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
 //     __FILE__, __LINE__,
      s.str ());
  }
}

void msrMeasure::appendPaddingNoteToMeasure (
  int inputLineNumber,
  int divisions,
  int divisionsPerQuarterNote)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTracePositionsInMeasures) {
    gLogIOstream <<
      "Appending padding note of " << divisions <<
      " divisions to measure " <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in segment " <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  // fetch the measure voice
  S_msrVoice
    measureVoice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

  // compute the forward step length
  rational
    forwardStepLength =
      rational (
        divisions,
        divisionsPerQuarterNote * 4); // hence a whole note

  // create a rest or a skip depending on measureVoice kind
  S_msrNote
    paddingNote =
      createPaddingNoteForVoice (
        inputLineNumber,
        forwardStepLength,
        measureVoice);

  // append the rest to the measure elements list
  // only now to make it possible to remove it afterwards
  // if it happens to be the first note of a chord
  appendNoteToMeasure (paddingNote);

  gIndenter--;
}


void msrMeasure::appendTempoToMeasure (
  S_msrTempo tempo)
{
  appendElementToMeasure (tempo);
}

void msrMeasure::appendRehearsalToMeasure (
  S_msrRehearsal rehearsal)
{
  appendElementToMeasure (rehearsal);
}

void msrMeasure::appendOctaveShiftToMeasure (
  S_msrOctaveShift octaveShift)
{
  appendElementToMeasure (octaveShift);
}

void msrMeasure::appendScordaturaToMeasure (
  S_msrScordatura scordatura)
{
  appendElementToMeasure (scordatura);
}

void msrMeasure::appendAccordionRegistrationToMeasure (
  S_msrAccordionRegistration
    accordionRegistration)
{
  appendElementToMeasure (accordionRegistration);
}

void msrMeasure::appendHarpPedalsTuningToMeasure (
  S_msrHarpPedalsTuning
    harpPedalsTuning)
{
  appendElementToMeasure (harpPedalsTuning);
}

void msrMeasure::appendLineBreakToMeasure (S_msrLineBreak lineBreak)
{
  appendElementToMeasure (lineBreak);
}

void msrMeasure::appendPageBreakToMeasure (S_msrPageBreak pageBreak)
{
  appendElementToMeasure (pageBreak);
}

void msrMeasure::appendStaffDetailsToMeasure (
  S_msrStaffDetails staffDetails)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceStaves || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending staff details '" << staffDetails->asShortString () <<
      "' to measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  appendElementToMeasure (staffDetails);
}

void msrMeasure::appendBarNumberCheckToMeasure (
  S_msrBarNumberCheck barNumberCheck)
{
  appendElementToMeasure (barNumberCheck);
}

void msrMeasure::prependOtherElementToMeasure (S_msrMeasureElement elem)
{
  fMeasureElementsList.push_front (elem); // JMI

  // this measure contains music
  fMeasureContainsMusic = true;
}

void msrMeasure::appendOtherElementToMeasure  (S_msrMeasureElement elem)
{
  appendElementToMeasure (elem);

  // this measure contains music
  fMeasureContainsMusic = true;
}

void msrMeasure::removeNoteFromMeasure (
  int       inputLineNumber,
  S_msrNote note)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Removing note '" <<
      note->asShortString () <<
      "' from measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"," <<
      "fMeasureLastHandledNote:" <<
      endl <<
      fMeasureLastHandledNote->asShortString () <<
      endl;
  }
#endif

  for (
    list<S_msrMeasureElement>::iterator i=fMeasureElementsList.begin ();
    i!=fMeasureElementsList.end ();
    ++i) {
    if ((*i) == note) {
      // found note, erase it
      fMeasureElementsList.erase (i);

      // update measure whole notes
      setCurrentMeasureWholeNotes (
        inputLineNumber,
        fCurrentMeasureWholeNotes
          -
        fMeasureLastHandledNote->getNoteSoundingWholeNotes ());

      // return from function
      return;
    }
  } // for

  S_msrVoice
    segmentVoiceUpLink =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

  S_msrPart
    segmentVoicePart =
      segmentVoiceUpLink->
        getVoiceStaffUpLink ()->
        getStaffPartUpLink ();

  gLogIOstream <<
    endl <<
    endl <<
    "@@@@@@@@@@@@@@@@@ segmentVoicePart" <<
    endl <<
    segmentVoicePart <<
    endl <<
    "@@@@@@@@@@@@@@@@@" <<
    endl <<
    endl;

  gLogIOstream <<
    endl <<
    endl <<
    "@@@@@@@@@@@@@@@@@ segmentVoiceUpLink" <<
    endl <<
    segmentVoiceUpLink <<
    endl <<
    "@@@@@@@@@@@@@@@@@" <<
    endl <<
    endl;

  stringstream s;

  s <<
    "cannot remove note " <<
    note <<
    " from measure " <<
    fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
    "' in voice \"" <<
    segmentVoiceUpLink->getVoiceName () <<
    "\"," <<
    " since it has not been found";

  msrInternalError (
    gGeneralOptions->fInputSourceName,
    inputLineNumber,
    __FILE__, __LINE__,
    s.str ());
}

void msrMeasure::removeElementFromMeasure (
  int          inputLineNumber,
  S_msrElement element)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Removing element:" <<
      endl;

    gIndenter++;
    gLogIOstream <<
      " JMI ??? element->elementAsString ()" << endl;
    gIndenter--;

    gLogIOstream <<
      endl <<
      " from measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"," <<
      endl;

    gIndenter++;
    gLogIOstream <<
      "fMeasureLastHandledNote:" <<
      endl <<
      fMeasureLastHandledNote <<
      endl;
    gIndenter--;
  }
#endif

  for (
    list<S_msrMeasureElement>::iterator i=fMeasureElementsList.begin ();
    i!=fMeasureElementsList.end ();
    ++i) {
    if ((*i) == element) {
      // found element, erase it
      fMeasureElementsList.erase (i);

      // update measure whole notes
      setCurrentMeasureWholeNotes (
        inputLineNumber,
        fCurrentMeasureWholeNotes
          -
        fMeasureLastHandledNote->getNoteSoundingWholeNotes ());

      // return from function
      return;
    }
  } // for

  stringstream s;

  s <<
    "cannot remove element " <<
    element <<
    " from measure " <<
    fMeasureNumber <<
    ", measureDebugNumber: '" <<
    fMeasureDebugNumber <<
    "' in voice \"" <<
    fMeasureSegmentUpLink->
      getSegmentVoiceUpLink ()->
        getVoiceName () <<
    "\"," <<
    " since it has not been found";

  msrInternalError (
    gGeneralOptions->fInputSourceName,
    inputLineNumber,
    __FILE__, __LINE__,
    s.str ());
}

void msrMeasure::determineMeasureKindAndPuristNumber (
  int     inputLineNumber,
  msrMeasure::msrMeasureRepeatContextKind
          measureRepeatContextKind)
{
  if (fMeasureKindAndPuristNumberHaveBeenDetermined) {
    stringstream s;

    s <<
      "Attempting to determine kind and purist number for measure \"" <<
      asShortString () <<
      "\" more than once";

if (false)
    msrInternalError (
      gGeneralOptions->fInputSourceName,
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // register measureRepeatContextKind
// JMI ???
  setMeasureRepeatContextKind (measureRepeatContextKind);

  // fetch the voice
  S_msrVoice
    voice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

  // get voice after repeat component phase kind
  msrVoice::msrVoiceRepeatPhaseKind
    currentVoiceRepeatPhaseKind =
      voice->
        getCurrentVoiceRepeatPhaseKind ();

  // regular measure ends detection
  rational
    wholeNotesSinceLastRegularMeasureEnd =
      voice->
        getWholeNotesSinceLastRegularMeasureEnd ();

  rational
    newWholeNotesSinceLastRegularMeasureEnd =
      wholeNotesSinceLastRegularMeasureEnd
        +
      fCurrentMeasureWholeNotes;
   newWholeNotesSinceLastRegularMeasureEnd.rationalise ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Determining the measure kind and purist number of measure '" <<
      fMeasureNumber <<
      "', measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "', fullMeasureWholeNotes: " <<
      fFullMeasureWholeNotes <<
      "', currentMeasureWholeNotes: " <<
      fCurrentMeasureWholeNotes <<
      ", wholeNotesSinceLastRegularMeasureEnd: " <<
      wholeNotesSinceLastRegularMeasureEnd <<
      ", newWholeNotesSinceLastRegularMeasureEnd: " <<
      newWholeNotesSinceLastRegularMeasureEnd <<
      ", currentVoiceRepeatPhaseKind: " <<
      msrVoice::voiceRepeatPhaseKindAsString (
        currentVoiceRepeatPhaseKind) <<
    "' in voice \"" << voice->getVoiceName () <<
    ", line " << inputLineNumber <<
    endl;

  displayMeasure (
    inputLineNumber,
    "determineMeasureKindAndPuristNumber() 1");
  }
#endif

  gIndenter++;

  // set measure purist number
  setMeasurePuristNumber (
    voice->
      getVoiceCurrentMeasurePuristNumber ());

  if (fCurrentMeasureWholeNotes.getNumerator () == 0) {
    // empty measure

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasuresDetails) {
    voice->
      displayVoiceRepeatsStackRestMeasuresMeasuresRepeatAndVoice (
        inputLineNumber,
        "determineMeasureKindAndPuristNumber() 2 measure has 0 current measure whole notes");

    stringstream s;

    s <<
      "measure '" <<
      fMeasureNumber <<
      "' has 0 current measure whole notes" <<
      ", " <<
      asString () <<
      ", line " << inputLineNumber;

    if (false) // JMI
      msrInternalError (
        gGeneralOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    else
      msrInternalWarning (
        gGeneralOptions->fInputSourceName,
        inputLineNumber,
        s.str ());
  }
#endif

    // set it's measure kind
    setMeasureKind (kMeasureKindMusicallyEmpty);
 }

  else if (fCurrentMeasureWholeNotes == fFullMeasureWholeNotes) {
    // this is a regular measure

    // this is a regular measure end
    setMeasureEndRegularKind (kMeasureEndRegularKindYes);

    // set it's measure kind
    setMeasureKind (kMeasureKindRegular);

    // this is a regular measure end
    setMeasureEndRegularKind (kMeasureEndRegularKindYes);

    // increment voice's current measure purist number
    voice->
      incrementVoiceCurrentMeasurePuristNumber (
        inputLineNumber,
        "determineMeasureKindAndPuristNumber() 3 kMeasureEndRegularKindYes");

    // reset voice whole notes since last regular measure end
    voice->
      setWholeNotesSinceLastRegularMeasureEnd (
        inputLineNumber,
        0);
  }

  else {
    // this measure is incomplete or overfull

    // increment voice whole notes since last regular measure end
    voice->
      setWholeNotesSinceLastRegularMeasureEnd (
        inputLineNumber,
        newWholeNotesSinceLastRegularMeasureEnd);

    if (fCurrentMeasureWholeNotes < fFullMeasureWholeNotes) {
      //  this is an incomplete measure

      if (fMeasureFirstInVoice) {
        // this is an anacrusis

        // set it's measure kind
        setMeasureKind (kMeasureKindAnacrusis);

        // this is a regular measure end
        setMeasureEndRegularKind (kMeasureEndRegularKindYes);

        // fix measure purist number
        setMeasurePuristNumber (0);
      }

      else {
        // this is an incomplete measure within the voice

        if (
          newWholeNotesSinceLastRegularMeasureEnd == fFullMeasureWholeNotes
        ) {
          // this is a regular measure end
          setMeasureEndRegularKind (kMeasureEndRegularKindYes);

          // increment voice's current measure purist number
          voice->
            incrementVoiceCurrentMeasurePuristNumber (
              inputLineNumber,
              "determineMeasureKindAndPuristNumber() 4 kMeasureEndRegularKindYes");

          // reset voice whole notes since last regular measure end
          voice->
            setWholeNotesSinceLastRegularMeasureEnd (
              inputLineNumber,
              0);
        }
        else {
          // this is no regular measure end
          setMeasureEndRegularKind (kMeasureEndRegularKindNo);
        }

        // set measure's kind according to measureRepeatContextKind
        switch (measureRepeatContextKind) {
          case msrMeasure::kMeasureRepeatContextKindUnknown: // JMI ???
            {
              stringstream s;

              displayMeasure (
                inputLineNumber,
                "determineMeasureKindAndPuristNumber() 5 kMeasureRepeatContextKindUnknown");

              s <<
                "measure '" <<
                fMeasureNumber <<
                "' is kMeasureRepeatContextKindUnknown " <<
                asShortString () <<
                ", line " << inputLineNumber;

        // JMI      msrInternalError (
              msrInternalWarning (
                gGeneralOptions->fInputSourceName,
                inputLineNumber,
         //       __FILE__, __LINE__,
                s.str ());
            }

            // set it's measure kind
            setMeasureKind (kMeasureKindIncompleteStandalone); // JMI
            break;

          case msrMeasure::kMeasureRepeatContextKindNone:
            // update the voice current measure purist number if relevant
            switch (fMeasureEndRegularKind) {
              case msrMeasure::kMeasureEndRegularKindUnknown:
                // JMI ???
                break;

              case msrMeasure::kMeasureEndRegularKindYes:
                // set it's measure kind
                setMeasureKind (kMeasureKindIncompleteStandalone);

                // don't increment the voice current measure purist number,
                // this has already been done for the 'first part' of the measure
                break;

              case msrMeasure::kMeasureEndRegularKindNo:
                // set it's measure kind
                setMeasureKind (kMeasureKindIncompleteStandalone);
                break;
            } // switch
            break;

          case msrMeasure::kMeasureRepeatContextKindCommonPartLastMeasure:
            setMeasureKind (
              kMeasureKindIncompleteLastInRepeatCommonPart);
            break;
          case msrMeasure::kMeasureRepeatContextKindHookedEndingLastMeasure:
            setMeasureKind (
              kMeasureKindIncompleteLastInRepeatHookedEnding);
            break;
          case msrMeasure::kMeasureRepeatContextKindHooklessEndingLastMeasure:
            setMeasureKind (
              kMeasureKindIncompleteLastInRepeatHooklessEnding);
            break;

          case msrMeasure::kMeasureRepeatContextKindNextMeasureAfterCommonPart:
            setMeasureKind (
              kMeasureKindIncompleteNextMeasureAfterCommonPart);
            break;
          case msrMeasure::kMeasureRepeatContextKindNextMeasureAfterHookedEnding:
            setMeasureKind (
              kMeasureKindIncompleteNextMeasureAfterHookedEnding);
            break;
          case msrMeasure::kMeasureRepeatContextKindNextMeasureAfterHooklessEnding:
            setMeasureKind (
              kMeasureKindIncompleteNextMeasureAfterHooklessEnding);
            break;
        } // switch
      }
    }

    else { // fCurrentMeasureWholeNotes > fFullMeasureWholeNotes
      // this is an overfull measure

      // set it's measure kind
      setMeasureKind (kMeasureKindOvercomplete);

      // this is a regular measure end
      setMeasureEndRegularKind (kMeasureEndRegularKindYes);

      // increment voice's current measure purist number
      voice->
        incrementVoiceCurrentMeasurePuristNumber (
          inputLineNumber,
          "determineMeasureKindAndPuristNumber() 6 kMeasureKindOvercomplete");

      // reset voice whole notes since last regular measure end
      voice->
        setWholeNotesSinceLastRegularMeasureEnd (
          inputLineNumber,
          0);
    }
  }

  // forget about voice after repeat component phase kind if relevant
  switch (currentVoiceRepeatPhaseKind) {
    case msrVoice::kVoiceRepeatPhaseNone:
      break;

    case msrVoice::kVoiceRepeatPhaseAfterCommonPart:
    case msrVoice::kVoiceRepeatPhaseAfterHookedEnding:
    case msrVoice::kVoiceRepeatPhaseAfterHooklessEnding:
      // reset incomplete measures after repeats detection
      voice->
        setCurrentVoiceRepeatPhaseKind (
          inputLineNumber,
          msrVoice::kVoiceRepeatPhaseNone);
      break;
  } // switch

  fMeasureKindAndPuristNumberHaveBeenDetermined = true;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    displayMeasure (
      inputLineNumber,
      "determineMeasureKindAndPuristNumber() 7");
  }
#endif

  gIndenter--;
}

void msrMeasure::padUpToPositionInMeasure (
  int      inputLineNumber,
  rational positionInMeasureToPadUpTo)
{
  // fetch the voice
  S_msrVoice
    measureVoice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTracePositionsInMeasures) {
    gLogIOstream <<
      "Padding up to position '" <<
      positionInMeasureToPadUpTo <<
      "' in measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in segment " <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      " in voice \"" <<
      measureVoice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTracePositionsInMeasures) {
    displayMeasure (
      inputLineNumber,
      "padUpToPositionInMeasure() 1");
  }
#endif

  gIndenter++;

  // fetch the part measure whole notes high tide
  rational
    partCurrentMeasureWholeNotesHighTide =
      fetchMeasurePartUpLink ()->
        getPartCurrentMeasureWholeNotesHighTide ();

  if (fCurrentMeasureWholeNotes < positionInMeasureToPadUpTo) {
    // appending a rest to this measure to reach positionInMeasureToPadUpTo
    rational
      missingDuration =
        positionInMeasureToPadUpTo - fCurrentMeasureWholeNotes;
    missingDuration.rationalise ();

    // create a rest or a skip depending on measureVoice kind
    S_msrNote
      paddingNote =
        createPaddingNoteForVoice (
          inputLineNumber,
          missingDuration,
          measureVoice);

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
       "Appending '" << paddingNote->asString () <<
       " (" << missingDuration << " whole notes)'" <<
       " to finalize \"" << measureVoice->getVoiceName () <<
       "\" measure: @" <<
       fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
       ", currentMeasureWholeNotes: " <<
       fCurrentMeasureWholeNotes <<
       " % --> @" << fMeasureNumber << // JMI
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
       ":" << partCurrentMeasureWholeNotesHighTide <<
        ", missingDuration = " << missingDuration <<
       endl;
   }
#endif

    // append the rest to the measure elements list
    // only now to make it possible to remove it afterwards
    // if it happens to be the first note of a chord
    appendNoteToMeasure (paddingNote);

    // this measure contains music
    fMeasureContainsMusic = true;
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    displayMeasure (
      inputLineNumber,
      "padUpToPositionInMeasure() 2");
  }
#endif

  gIndenter--;
}

void msrMeasure::padUpToPositionAtTheEndOfMeasure (
  int      inputLineNumber,
  rational positionInMeasureToPadUpTo)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTracePositionsInMeasures) {
    gLogIOstream <<
      "Padding up to position '" <<
      positionInMeasureToPadUpTo <<
      "' at the end of measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in segment " <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      " in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTracePositionsInMeasures) {
    displayMeasure (
      inputLineNumber,
      "padUpToPositionAtTheEndOfMeasure() 1");
  }
#endif

  gIndenter++;

  // fetch the part measure whole notes high tide
  rational
    partCurrentMeasureWholeNotesHighTide =
      fetchMeasurePartUpLink ()->
        getPartCurrentMeasureWholeNotesHighTide ();

  if (fCurrentMeasureWholeNotes < positionInMeasureToPadUpTo) {
    // appending a rest to this measure to reach positionInMeasureToPadUpTo
    rational
      duration =
        positionInMeasureToPadUpTo - fCurrentMeasureWholeNotes;
    duration.rationalise ();

    // fetch the voice
    S_msrVoice
      measureVoice =
        fMeasureSegmentUpLink->
          getSegmentVoiceUpLink ();

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures || gTraceOptions->fTracePositionsInMeasures) {
      gLogIOstream <<
       "Creating a padding note '" <<
       ", duration: " << duration <<
       " at the end of voice \"" << measureVoice->getVoiceName () <<
       "\", measure: '" <<
       fMeasureNumber <<
       "', measureDebugNumber: '" <<
       fMeasureDebugNumber <<
       ", currentMeasureWholeNotes: " <<
       fCurrentMeasureWholeNotes <<
       ", partCurrentMeasureWholeNotesHighTide:" <<
       partCurrentMeasureWholeNotesHighTide <<
       endl;
   }
#endif

    // create a rest or a skip depending on measureVoice kind
    S_msrNote
      paddingNote =
        createPaddingNoteForVoice (
          inputLineNumber,
          duration,
          measureVoice);

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures || gTraceOptions->fTracePositionsInMeasures) {
      gLogIOstream <<
       "Appending '" << paddingNote->asString () <<
       " to finalize \"" << measureVoice->getVoiceName () <<
       "\" measure: '" <<
       fMeasureNumber <<
       "', measureDebugNumber: '" <<
       fMeasureDebugNumber <<
       ", currentMeasureWholeNotes: " <<
       fCurrentMeasureWholeNotes <<
       ", partCurrentMeasureWholeNotesHighTide: " <<
       partCurrentMeasureWholeNotesHighTide <<
 //  JMI       ", missingDuration = " << missingDuration <<
       endl;
   }
#endif

    // append the rest to the measure elements list
    // only now to make it possible to remove it afterwards
    // if it happens to be the first note of a chord JMI
    appendPaddingNoteAtTheEndOfMeasure (paddingNote);

    // this measure contains music
    fMeasureContainsMusic = true;
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTracePositionsInMeasures) {
    displayMeasure (
      inputLineNumber,
      "padUpToPositionAtTheEndOfMeasure() 2");
  }
#endif

  gIndenter--;
}

void msrMeasure::finalizeRegularMeasure (
  int                         inputLineNumber,
  msrMeasureRepeatContextKind measureRepeatContextKind,
  string                      context)
{
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

  // fetch the part measure whole notes high tide
  rational
    partCurrentMeasureWholeNotesHighTide =
      fetchMeasurePartUpLink ()->
        getPartCurrentMeasureWholeNotesHighTide ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Finalizing regular measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in segment '" <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      voice->getVoiceName () <<
      "\" (" << context << ")" <<
      ", partCurrentMeasureWholeNotesHighTide = " <<
      partCurrentMeasureWholeNotesHighTide <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    displayMeasure (
      inputLineNumber,
      "finalizeRegularMeasure() 1");
  }
#endif

  // fetch the voice's time
  S_msrTime
    voiceCurrentTime =
      voice->getVoiceCurrentTime ();

  if (! voiceCurrentTime) {
    // take the implicit 4/4 measure whole notes into account
    fFullMeasureWholeNotes = rational (1, 1);
  }
  else {
    // set the full length from time
    setFullMeasureWholeNotesFromTime (
      voiceCurrentTime);
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    if (fCurrentMeasureWholeNotes.getNumerator () == 0) {
      stringstream s;

      s <<
        "Measure '" <<
        fMeasureNumber <<
        ", measureDebugNumber: '" <<
        fMeasureDebugNumber <<
        "' in segment '" <<
        fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
        "' in voice \"" <<
        voice->getVoiceName () <<
        "\", line " << inputLineNumber <<
        ", doesn't contain any music" <<
        endl;

      msrMusicXMLWarning (
        gGeneralOptions->fInputSourceName,
        inputLineNumber,
        s.str ());
    }
  }
#endif

  if (fMeasurePendingMeasureElementsList.size ()) {
    // pad measure up to the elements positions in measure,
    // and then append them

    list<S_msrMeasureElement>::iterator
      iBegin = fMeasurePendingMeasureElementsList.begin (),
      iEnd   = fMeasurePendingMeasureElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrMeasureElement
        measureElement = (*i);

#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceMeasures) {
        gLogIOstream <<
          "Considering delayed '" <<
          measureElement->asShortString () <<
          "' in measure '" <<
          asShortString () <<
          "' in voice \"" <<
          fMeasureSegmentUpLink->
            getSegmentVoiceUpLink ()
              ->getVoiceName () <<
          "\", line " << inputLineNumber <<
          ", has position in measure '" <<
          measureElement->getPositionInMeasure () <<
          ", currentMeasureWholeNotes = " <<
          fCurrentMeasureWholeNotes <<
          ", partCurrentMeasureWholeNotesHighTide = " <<
          partCurrentMeasureWholeNotesHighTide <<
          endl;
      }
#endif

      if (
        measureElement->getPositionInMeasure () <= fCurrentMeasureWholeNotes
      ) { // JMI
        // this is where measureElement should be appended at last

        padUpToCurrentMeasureWholeNotesInMeasure (
          inputLineNumber,
          measureElement->getPositionInMeasure ()); // ??? rational (1, 4));

#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceMeasures) {
          gLogIOstream <<
            "Appending delayed '" <<
            measureElement->asShortString () <<
            "' in measure '" <<
            asShortString () <<
            "' in voice \"" <<
            fMeasureSegmentUpLink->
              getSegmentVoiceUpLink ()
                ->getVoiceName () <<
            "\", line " << inputLineNumber <<
            ", has position in measure '" <<
            measureElement->getPositionInMeasure () <<
            ", currentMeasureWholeNotes = " << fCurrentMeasureWholeNotes <<
            ", partCurrentMeasureWholeNotesHighTide = " << partCurrentMeasureWholeNotesHighTide <<
            endl;
        }
#endif

        appendElementToMeasure (measureElement); // JMI
// JMI        fMeasureElementsList.push_back (measureElement);

        // remove it from the pending measure elements list
        i = fMeasurePendingMeasureElementsList.erase (i);
      }

      if (++i == iEnd) break;
    } // for
  }

  // pad the measure up if it is in a harmony of figured bass voice
  switch (voice->getVoiceKind ()) {
    case msrVoice::kVoiceRegular:
      break;
    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
      padUpToPositionAtTheEndOfMeasure (
        inputLineNumber,
        partCurrentMeasureWholeNotesHighTide);
      break;
  } // switch

  // determine the measure kind and purist number
  determineMeasureKindAndPuristNumber (
    inputLineNumber,
    measureRepeatContextKind);

  // pad measure up to part measure whole notes high tide
  switch (fMeasureKind) {
    case msrMeasure::kMeasureKindCadenza:
      break;

    case msrMeasure::kMeasureKindOvercomplete:
    case msrMeasure::kMeasureKindAnacrusis:
    case msrMeasure::kMeasureKindRegular:
    case msrMeasure::kMeasureKindIncompleteStandalone: // JMI
    case msrMeasure::kMeasureKindIncompleteLastInRepeatCommonPart: // JMI
    case msrMeasure::kMeasureKindIncompleteLastInRepeatHookedEnding: // JMI
    case msrMeasure::kMeasureKindIncompleteLastInRepeatHooklessEnding: // JMI
    case msrMeasure::kMeasureKindIncompleteNextMeasureAfterCommonPart: // JMI
    case msrMeasure::kMeasureKindIncompleteNextMeasureAfterHookedEnding: // JMI
    case msrMeasure::kMeasureKindIncompleteNextMeasureAfterHooklessEnding: // JMI
      break;

    case msrMeasure::kMeasureKindUnknown:
      // JMI ???
      break;

    case msrMeasure::kMeasureKindMusicallyEmpty:
      {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceMeasures) {
        stringstream s;

        s <<
          "measure '" <<
          fMeasureNumber <<
          ", measureDebugNumber: '" <<
          fMeasureDebugNumber <<
          "' in segment '" <<
          fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
          "' in voice \"" <<
          voice->getVoiceName () <<
          "\" (" << context << context << ")" <<
          ", line " << inputLineNumber <<
          " IS MUSICALLY EMPTY";

        msrInternalWarning (
          gGeneralOptions->fInputSourceName,
          inputLineNumber,
          s.str ());
      }
#endif

        // fetch the part measure whole notes high tide
        rational
          partCurrentMeasureWholeNotesHighTide =
            fetchMeasurePartUpLink ()->
              getPartCurrentMeasureWholeNotesHighTide ();

        // pad up to partCurrentMeasureWholeNotesHighTide if > 0
        // or fFullMeasureWholeNotes otherwise
        rational
          positionToPadUpto =
            partCurrentMeasureWholeNotesHighTide.getNumerator ()
              ? partCurrentMeasureWholeNotesHighTide
              : fFullMeasureWholeNotes;

        padUpToPositionInMeasure (
          inputLineNumber,
          positionToPadUpto);

          /* JMI
        switch (fMeasureCreatedForARepeatKind) {
          case msrMeasure::kMeasureKindCreatedForARepeatNo:
            {
              // fetch the part measure whole notes high tide
              rational
                partCurrentMeasureWholeNotesHighTide =
                  fetchMeasurePartUpLink ()->
                    getPartCurrentMeasureWholeNotesHighTide ();

              padUpToPositionInMeasure (
                inputLineNumber,
                partCurrentMeasureWholeNotesHighTide);
            }
            break;

          case msrMeasure::kMeasureKindCreatedForARepeatBefore:
          case msrMeasure::kMeasureKindCreatedForARepeatAfter:
            // such a measure should not be padded with a rest
            break;

          case msrMeasure::kMeasureKindCreatedForARepeatPadded:
            // should not occur
            break;
        } // switch
        */
      }
      break;
  } // switch

  // is there a single note or rest occupying the full measure?
  if (fMeasureLongestNote) {
    if (
      fMeasureLongestNote-> getNoteSoundingWholeNotes ()
        ==
      fFullMeasureWholeNotes
    ) {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceMeasures) {
        gLogIOstream <<
          "Note '" <<
          fMeasureLongestNote->asShortString () <<
          "' occupies measure '" <<
          fMeasureNumber <<
          ", measureDebugNumber: '" <<
          fMeasureDebugNumber <<
          "' fully in segment '" <<
          fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
          "' in voice \"" <<
          voice->getVoiceName () <<
          "\", line " << inputLineNumber <<
          endl;
      }
#endif

      fMeasureLongestNote->
        setNoteOccupiesAFullMeasure ();
    }
  }

  fMeasureHasBeenFinalized = true;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    displayMeasure (
      inputLineNumber,
      "finalizeRegularMeasure() 2");
  }
#endif

  gIndenter--;
}

void msrMeasure::handleHarmoniesInHarmonyMeasureFinalization (
  int      inputLineNumber,
  rational partCurrentMeasureWholeNotesHighTide, // JMI ???
  string   context)
{
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceHarmonies
      ||
    gTraceOptions->fTracePositionsInMeasures
      ||
    gTraceOptions->fTraceForTests
  ) {
    gLogIOstream <<
      "Finalizing harmonies in harmony measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in segment '" <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      voice->getVoiceName () <<
      "\" (" << context << ")" <<
      ", partCurrentMeasureWholeNotesHighTide = " <<
      partCurrentMeasureWholeNotesHighTide <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (fMeasureElementsList.size ()) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTracePositionsInMeasures) {
      displayMeasure (
        inputLineNumber,
        "handleHarmoniesInHarmonyMeasureFinalization() 1");
    }
#endif

    // harmonies don't have their own duration and may have a non-null offset:
    // we thus have to sort them in the measure by increasing position,
    // taking the latter into account
    fMeasureElementsList.sort (
      msrMeasureElement::compareMeasureElementsByIncreasingPositionInMeasure);

    // consider each measure element (a harmony in fact) in turn,
    // updating their duration and adding skips if needed
    list<S_msrMeasureElement>::iterator
      iBegin = fMeasureElementsList.begin (),
      iEnd   = fMeasureElementsList.end (),
      i      = iBegin;

    S_msrHarmony
      previousHarmony = nullptr;

    while (true) {
      S_msrMeasureElement
        measureElement = (*i);

      int inputLineNumber =
        measureElement->getInputLineNumber ();

      if (
        // harmony?
        S_msrHarmony
          currentHarmony =
            dynamic_cast<msrHarmony*>(&(*measureElement))
      ) {
        // handle the currentHarmony
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTracePositionsInMeasures) {
          gLogIOstream <<
            "handleHarmoniesInHarmonyMeasureFinalization() 2" <<
            ", currentHarmony: ";
            gIndenter++;
            gLogIOstream <<
              currentHarmony->asString () <<
              endl;
            gIndenter--;
        }
#endif

        // it's position in the measure should take it's offset into account
        rational
          positionInMeasureOfCurrentHarmony =
            currentHarmony->getPositionInMeasure ();

        // get the currentHarmony's note uplink
        S_msrNote
          currentHarmonyNoteUpLink  =
            currentHarmony->getHarmonyNoteUpLink ();

        // get the currentHarmony's note uplink position in the measure
        rational
          positionInMeasureOfCurrentHarmonyNoteUpLink =
            currentHarmonyNoteUpLink->getPositionInMeasure ();

#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTracePositionsInMeasures) {
            gLogIOstream <<
              "handleHarmoniesInHarmonyMeasureFinalization() 3" <<
              ", previousHarmony: ";

            if (previousHarmony) {
              gLogIOstream <<
                previousHarmony->asString ();
            }
            else {
              gLogIOstream << "none";
            }

            gLogIOstream <<
              ", positionInMeasureOfCurrentHarmony: " <<
              positionInMeasureOfCurrentHarmony <<
              ", positionInMeasureOfCurrentHarmonyNoteUpLink: " <<
              positionInMeasureOfCurrentHarmonyNoteUpLink <<
              endl;
          }
#endif

        if (! previousHarmony) {
          // currentHarmony is the first harmony in the measure

          // the position to pad up to is the minimum
          // of those of the currentHarmony and currentHarmonyNoteUpLink,
          // to keep comparison points between the regular voice and its harmony voice
          rational
            positionInMeasureToPadUpTo =
              positionInMeasureOfCurrentHarmonyNoteUpLink;

          if (
            positionInMeasureOfCurrentHarmony
              <
            positionInMeasureOfCurrentHarmonyNoteUpLink
          ) {
            positionInMeasureToPadUpTo =
              positionInMeasureOfCurrentHarmony;
          }

#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTracePositionsInMeasures) {
            gLogIOstream <<
              "handleHarmoniesInHarmonyMeasureFinalization() 4" <<
              ", previousHarmony is null, positionInMeasureToPadUpTo: " <<
              positionInMeasureToPadUpTo <<
              endl;
          }
#endif

          // create a padding note
          if (positionInMeasureToPadUpTo.getNumerator () != 0) {
            // a padding note is necessary
            S_msrNote
              paddingNote =
                createPaddingNoteForVoice (
                  inputLineNumber,
                  positionInMeasureToPadUpTo,
                  voice);

            // insert paddingNote before currentHarmony in the measure's elements list
#ifdef TRACE_OPTIONS
            if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTracePositionsInMeasures) {
              gLogIOstream <<
                "Inserting first padding note " <<
                paddingNote->asString () <<
                " before currentHarmony " <<
                currentHarmony->asString () <<
                " in voice \"" <<
                voice->getVoiceName () <<
                "\", line " << inputLineNumber <<
                endl;
            }
#endif

            // insert paddingNote in the measure elements list before (*i)
            insertElementInMeasureBeforeIterator (
              inputLineNumber,
              i,
              paddingNote);

#ifdef TRACE_OPTIONS
            if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTracePositionsInMeasures) {
              displayMeasure (
                inputLineNumber,
                "handleHarmoniesInHarmonyMeasureFinalization() 5");
            }
#endif
          }
        }

        else {
          // this is a subsequent harmony in the measure

          // get the previousHarmony's note uplink position in the measure
          rational
            positionInMeasureOfPreviousHarmony =
              previousHarmony->getPositionInMeasure ();

          // get the previousHarmony's duration
          rational
            previousHarmonySoundingWholeNotes =
              previousHarmony->getHarmonySoundingWholeNotes ();

          // compute the position in measure following previousHarmony
          rational
            positionInMeasureFollowingPreviousHarmony =
              positionInMeasureOfPreviousHarmony
                +
              previousHarmonySoundingWholeNotes;
          positionInMeasureFollowingPreviousHarmony.rationalise ();

          rational
            positionsInMeasureDelta =
              positionInMeasureOfCurrentHarmony
                -
              positionInMeasureFollowingPreviousHarmony;
          positionsInMeasureDelta.rationalise ();

#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTracePositionsInMeasures) {
            gLogIOstream <<
              "handleHarmoniesInHarmonyMeasureFinalization() 6" <<
              ", previousHarmony: ";

            if (previousHarmony) {
              gLogIOstream <<
                previousHarmony->asString ();
            }
            else {
              gLogIOstream << "none";
            }

            gLogIOstream <<
              ", currentHarmony: " <<
              currentHarmony->asString () <<
              ", positionInMeasureOfPreviousHarmony: " <<
              positionInMeasureOfPreviousHarmony <<
              ", positionInMeasureOfCurrentHarmony: " <<
              positionInMeasureOfCurrentHarmony <<
              ", positionInMeasureFollowingPreviousHarmony: " <<
              positionInMeasureFollowingPreviousHarmony <<
              ", positionsInMeasureDelta: " <<
              positionsInMeasureDelta <<
              endl;
          }
#endif

          // is a padding note needed?
          if (positionsInMeasureDelta.getNumerator () > 0) {
            // create a padding note
            S_msrNote
              paddingNote =
                createPaddingNoteForVoice (
                  inputLineNumber,
                  positionsInMeasureDelta,
                  voice);

            // set its position in measure
            paddingNote->
              setPositionInMeasure (
                fCurrentMeasureWholeNotes,
                "handleHarmoniesInHarmonyMeasureFinalization() 7");

            // insert paddingNote before currentHarmony in the measure's elements list
#ifdef TRACE_OPTIONS
            if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTracePositionsInMeasures) {
              gLogIOstream <<
                "Inserting subsequent padding note " <<
                paddingNote->asString () <<
                " before currentHarmony " <<
                currentHarmony->asString () <<
                " in voice \"" <<
                voice->getVoiceName () <<
                "\", line " << inputLineNumber <<
                endl;
            }
#endif

            // insert paddingNote in the measure elements list before (*i)
            insertElementInMeasureBeforeIterator (
              inputLineNumber,
              i,
              paddingNote);
          }

          else if (
              positionsInMeasureDelta.getNumerator () < 0
          ) {
            stringstream s;

            s <<
              "Previous harmony " <<
              previousHarmony->asString () <<
              " bumps into " <<
              currentHarmony->asString ();

            msrInternalError (
              gGeneralOptions->fInputSourceName,
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
        }

        previousHarmony = currentHarmony;
      }

      if (++i == iEnd) break;
    } // while

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTracePositionsInMeasures) {
      displayMeasure (
        inputLineNumber,
        "handleHarmoniesInHarmonyMeasureFinalization() 8");
    }
#endif
  }
}

void msrMeasure::finalizeHarmonyMeasure (
  int                         inputLineNumber,
  msrMeasureRepeatContextKind measureRepeatContextKind,
  string                      context)
{
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

  // fetch the part measure whole notes high tide
  rational
    partCurrentMeasureWholeNotesHighTide =
      fetchMeasurePartUpLink ()->
        getPartCurrentMeasureWholeNotesHighTide ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Finalizing harmony measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in segment '" <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      voice->getVoiceName () <<
      "\" (" << context << ")" <<
      ", partCurrentMeasureWholeNotesHighTide = " <<
      partCurrentMeasureWholeNotesHighTide <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceMeasures) {
    displayMeasure (
      inputLineNumber,
      "finalizeHarmonyMeasure() 1");
  }
#endif

  // fetch the voice's time
  S_msrTime
    voiceCurrentTime =
      voice->getVoiceCurrentTime ();

  if (! voiceCurrentTime) {
    // take the implicit 4/4 measure whole notes into account
    fFullMeasureWholeNotes = rational (1, 1);
  }
  else {
    // set the full length from time
    setFullMeasureWholeNotesFromTime (
      voiceCurrentTime);
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceMeasures) {
    if (fCurrentMeasureWholeNotes.getNumerator () == 0) {
      stringstream s;

      s <<
        "Measure '" <<
        fMeasureNumber <<
        ", measureDebugNumber: '" <<
        fMeasureDebugNumber <<
        "' in segment '" <<
        fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
        "' in voice \"" <<
        voice->getVoiceName () <<
        "\", line " << inputLineNumber <<
        ", doesn't contain any music" <<
        endl;

      msrMusicXMLWarning (
        gGeneralOptions->fInputSourceName,
        inputLineNumber,
        s.str ());
    }
  }
#endif

  // handle the harmonies in this measure
  handleHarmoniesInHarmonyMeasureFinalization (
    inputLineNumber,
    partCurrentMeasureWholeNotesHighTide,
    context);

  // pad the measure up if it is in a harmony of figured bass voice
  switch (voice->getVoiceKind ()) {
    case msrVoice::kVoiceRegular:
      break;
    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
      padUpToPositionAtTheEndOfMeasure (
        inputLineNumber,
        partCurrentMeasureWholeNotesHighTide);
      break;
  } // switch

  // determine the measure kind and purist number
  determineMeasureKindAndPuristNumber (
    inputLineNumber,
    measureRepeatContextKind);

  // pad measure up to part measure whole notes high tide
  switch (fMeasureKind) {
    case msrMeasure::kMeasureKindCadenza:
      break;

    case msrMeasure::kMeasureKindOvercomplete:
    case msrMeasure::kMeasureKindAnacrusis:
    case msrMeasure::kMeasureKindRegular:
    case msrMeasure::kMeasureKindIncompleteStandalone: // JMI
    case msrMeasure::kMeasureKindIncompleteLastInRepeatCommonPart: // JMI
    case msrMeasure::kMeasureKindIncompleteLastInRepeatHookedEnding: // JMI
    case msrMeasure::kMeasureKindIncompleteLastInRepeatHooklessEnding: // JMI
    case msrMeasure::kMeasureKindIncompleteNextMeasureAfterCommonPart: // JMI
    case msrMeasure::kMeasureKindIncompleteNextMeasureAfterHookedEnding: // JMI
    case msrMeasure::kMeasureKindIncompleteNextMeasureAfterHooklessEnding: // JMI
      break;

    case msrMeasure::kMeasureKindUnknown:
      // JMI ???
      break;

    case msrMeasure::kMeasureKindMusicallyEmpty:
      {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceMeasures) {
        stringstream s;

        s <<
          "measure '" <<
          fMeasureNumber <<
          ", measureDebugNumber: '" <<
          fMeasureDebugNumber <<
          "' in segment '" <<
          fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
          "' in voice \"" <<
          voice->getVoiceName () <<
          "\" (" << context << context << ")" <<
          ", line " << inputLineNumber <<
          " IS MUSICALLY EMPTY";

        msrInternalWarning (
          gGeneralOptions->fInputSourceName,
          inputLineNumber,
          s.str ());
      }
#endif

        // fetch the part measure whole notes high tide
        rational
          partCurrentMeasureWholeNotesHighTide =
            fetchMeasurePartUpLink ()->
              getPartCurrentMeasureWholeNotesHighTide ();

        // pad up to partCurrentMeasureWholeNotesHighTide if > 0
        // or fFullMeasureWholeNotes otherwise
        rational
          positionToPadUpto =
            partCurrentMeasureWholeNotesHighTide.getNumerator ()
              ? partCurrentMeasureWholeNotesHighTide
              : fFullMeasureWholeNotes;

        padUpToPositionInMeasure (
          inputLineNumber,
          positionToPadUpto);

          /* JMI
        switch (fMeasureCreatedForARepeatKind) {
          case msrMeasure::kMeasureKindCreatedForARepeatNo:
            {
              // fetch the part measure whole notes high tide
              rational
                partCurrentMeasureWholeNotesHighTide =
                  fetchMeasurePartUpLink ()->
                    getPartCurrentMeasureWholeNotesHighTide ();

              padUpToPositionInMeasure (
                inputLineNumber,
                partCurrentMeasureWholeNotesHighTide);
            }
            break;

          case msrMeasure::kMeasureKindCreatedForARepeatBefore:
          case msrMeasure::kMeasureKindCreatedForARepeatAfter:
            // such a measure should not be padded with a rest
            break;

          case msrMeasure::kMeasureKindCreatedForARepeatPadded:
            // should not occur
            break;
        } // switch
        */
      }
      break;
  } // switch

  // is there a single note or rest occupying the full measure?
  if (fMeasureLongestNote) {
    if (
      fMeasureLongestNote-> getNoteSoundingWholeNotes ()
        ==
      fFullMeasureWholeNotes
    ) {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceMeasures) {
        gLogIOstream <<
          "Note '" <<
          fMeasureLongestNote->asShortString () <<
          "' occupies measure '" <<
          fMeasureNumber <<
          ", measureDebugNumber: '" <<
          fMeasureDebugNumber <<
          "' fully in segment '" <<
          fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
          "' in voice \"" <<
          voice->getVoiceName () <<
          "\", line " << inputLineNumber <<
          endl;
      }
#endif

      fMeasureLongestNote->
        setNoteOccupiesAFullMeasure ();
    }
  }

  fMeasureHasBeenFinalized = true;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceMeasures) {
    displayMeasure (
      inputLineNumber,
      "finalizeHarmonyMeasure() 2");
  }
#endif

  gIndenter--;
}

void msrMeasure::finalizeFiguredBassMeasure (
  int                         inputLineNumber,
  msrMeasureRepeatContextKind measureRepeatContextKind,
  string                      context)
{
}

void msrMeasure::finalizeMeasure (
  int                         inputLineNumber,
  msrMeasureRepeatContextKind measureRepeatContextKind,
  string                      context)
{
  if (false && fMeasureHasBeenFinalized) { // JMI
    stringstream s;

    s <<
      "Attempting to finalize measure \"" <<
      asShortString () <<
      "\" more than once";

    msrInternalError (
      gGeneralOptions->fInputSourceName,
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  S_msrVoice
    voice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

  switch (voice->getVoiceKind ()) {
    case msrVoice::kVoiceRegular:
      finalizeRegularMeasure (
        inputLineNumber,
        measureRepeatContextKind,
        context);
      break;
    case msrVoice::kVoiceHarmony:
      finalizeHarmonyMeasure (
        inputLineNumber,
        measureRepeatContextKind,
        context);
      break;
    case msrVoice::kVoiceFiguredBass:
   // finalizeFiguredBassMeasure (
      finalizeRegularMeasure ( // TEMPORARY JMI to be seen
        inputLineNumber,
        measureRepeatContextKind,
        context);
      break;
  } // switch
}

void msrMeasure::finalizeMeasureClone (
  int          inputLineNumber,
  S_msrMeasure originalMeasure,
  S_msrVoice   voiceClone)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    displayMeasure (
      inputLineNumber,
      "finalizeMeasureClone() 1");
  }
#endif

  if (fMeasureHasBeenFinalized) {
    stringstream s;

    s <<
      "Attempting to finalize measure clone \"" <<
      asShortString () <<
      "\" more than once";

    msrInternalError (
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  gIndenter++;

  // determine the measure kind and purist number
  determineMeasureKindAndPuristNumber (
    inputLineNumber,
    originalMeasure->getMeasureRepeatContextKind ());

  // consistency check
  msrMeasure::msrMeasureKind
    originalMeasureMeasureKind =
      originalMeasure->getMeasureKind ();

  if (fMeasureKind != originalMeasureMeasureKind) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures) {
      this->
        displayMeasure (
          inputLineNumber,
          "finalizeMeasureClone() cloneMeasure");

      originalMeasure->
        displayMeasure (
          inputLineNumber,
          "finalizeMeasureClone() originalMeasure");
    }
#endif

    stringstream s;

    s <<
      "*********>> measure '" <<
      fMeasureNumber <<
      "': clone measure kind '"<<
      msrMeasure::measureKindAsString (
        fMeasureKind) <<
      "' differs for original measure measure kind '" <<
      msrMeasure::measureKindAsString (
        originalMeasureMeasureKind) <<
      "'" <<
      ", line " << inputLineNumber;

 //   msrInternalError ( // JMI
    msrInternalWarning (
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
 //      __FILE__, __LINE__,
      s.str ());
  }

  fMeasureHasBeenFinalized = true;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    displayMeasure (
      inputLineNumber,
      "finalizeMeasureClone() 2");
  }
#endif

  gIndenter--;
}

void msrMeasure::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasure::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrMeasure>*
    p =
      dynamic_cast<visitor<S_msrMeasure>*> (v)) {
        S_msrMeasure elem = this;

        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasure::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasure::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasure::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMeasure>*
    p =
      dynamic_cast<visitor<S_msrMeasure>*> (v)) {
        S_msrMeasure elem = this;

        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasure::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasure::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasure::browseData ()" <<
      endl;
  }

  for (
    list<S_msrMeasureElement>::const_iterator i = fMeasureElementsList.begin ();
    i != fMeasureElementsList.end ();
    i++
  ) {
    // browse the element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% <== msrMeasure::browseData ()" <<
      endl;
  }
}

string msrMeasure::measureKindAsString (
  msrMeasureKind measureKind)
{
  string result;

  switch (measureKind) {
    case msrMeasure::kMeasureKindUnknown:
      result = "***measureKindUnknown***";
      break;
    case msrMeasure::kMeasureKindRegular:
      result = "measureRegular";
      break;
    case msrMeasure::kMeasureKindAnacrusis:
      result = "measureAnacrusis";
      break;
    case msrMeasure::kMeasureKindIncompleteStandalone:
      result = "measureIncompleteStandalone";
      break;
    case msrMeasure::kMeasureKindIncompleteLastInRepeatCommonPart:
      result = "measureIncompleteLastInRepeatCommonPart";
      break;
    case msrMeasure::kMeasureKindIncompleteLastInRepeatHookedEnding:
      result = "measureIncompleteLastInRepeatHookedEnding";
      break;
    case msrMeasure::kMeasureKindIncompleteLastInRepeatHooklessEnding:
      result = "measureIncompleteLastInRepeatHooklessEnding";
      break;
    case msrMeasure::kMeasureKindIncompleteNextMeasureAfterCommonPart:
      result = "measureIncompleteNextMeasureAfterCommonPart";
      break;
    case msrMeasure::kMeasureKindIncompleteNextMeasureAfterHookedEnding:
      result = "measureIncompleteNextMeasureAfterHookedEnding";
      break;
    case msrMeasure::kMeasureKindIncompleteNextMeasureAfterHooklessEnding:
      result = "measureIncompleteNextMeasureAfterHooklessEnding";
      break;
    case msrMeasure::kMeasureKindOvercomplete:
      result = "measureOvercomplete";
      break;
    case msrMeasure::kMeasureKindCadenza:
      result = "measureCadenza";
      break;
    case msrMeasure::kMeasureKindMusicallyEmpty:
      result = "measureMusicallyEmpty";
      break;
  } // switch

  return result;
}

string msrMeasure::measureImplicitKindAsString (
  msrMeasureImplicitKind measureImplicitKind)
{
  string result;

  switch (measureImplicitKind) {
    case msrMeasure::kMeasureImplicitKindYes:
      result = "measureImplicitKindYes";
      break;
    case msrMeasure::kMeasureImplicitKindNo:
      result = "measureImplicitKindNo";
      break;
  } // switch

  return result;
}

string msrMeasure::measureFirstInSegmentKindAsString (
    msrMeasureFirstInSegmentKind measureFirstInSegmentKind)
{
  string result;

  switch (measureFirstInSegmentKind) {
    case msrMeasure::kMeasureFirstInSegmentKindUnknown:
      result = "***measureFirstInSegmentKindUnknown***";
      break;
    case msrMeasure::kMeasureFirstInSegmentKindYes:
      result = "measureFirstInSegmentKindYes";
      break;
    case msrMeasure::kMeasureFirstInSegmentKindNo:
      result = "measureFirstInSegmentKindNo";
      break;
  } // switch

  return result;
}

string msrMeasure::measureRepeatContextKindAsString (
  msrMeasureRepeatContextKind measureRepeatContextKind)
{
  string result;

  switch (measureRepeatContextKind) {
    case msrMeasure::kMeasureRepeatContextKindUnknown:
      result = "***measureRepeatContextUnknown***";
      break;
    case msrMeasure::kMeasureRepeatContextKindNone:
      result = "measureRepeatContextNone";
      break;
    case msrMeasure::kMeasureRepeatContextKindCommonPartLastMeasure:
      result = "measureRepeatContextCommonPartLastMeasure";
      break;
    case msrMeasure::kMeasureRepeatContextKindHookedEndingLastMeasure:
      result = "measureRepeatContextHookedEndingLastMeasure";
      break;
    case msrMeasure::kMeasureRepeatContextKindHooklessEndingLastMeasure:
      result = "measureRepeatContextHooklessEndingLastMeasure";
      break;
    case msrMeasure::kMeasureRepeatContextKindNextMeasureAfterCommonPart:
      result = "measureRepeatContextNextMeasureAfterCommonPart";
      break;
    case msrMeasure::kMeasureRepeatContextKindNextMeasureAfterHookedEnding:
      result = "measureRepeatContextNextMeasureAfterHookedEnding";
      break;
    case msrMeasure::kMeasureRepeatContextKindNextMeasureAfterHooklessEnding:
      result = "measureRepeatContextNextMeasureAfterHooklessEnding";
      break;
  } // switch

  return result;
}

string msrMeasure::measureEndRegularKindAsString (
    msrMeasureEndRegularKind measureEndRegularKind)
{
  string result;

  switch (measureEndRegularKind) {
    case msrMeasure::kMeasureEndRegularKindUnknown:
      result = "***measureEndRegularUnknown***";
      break;
    case msrMeasure::kMeasureEndRegularKindYes:
      result = "measureEndRegularYes";
      break;
    case msrMeasure::kMeasureEndRegularKindNo:
      result = "measureEndRegularNo";
      break;
  } // switch

  return result;
}

string msrMeasure::measureKindAsString () const
{
  return
    measureKindAsString (fMeasureKind);
}

string msrMeasure::asShortString () const
{
  stringstream s;

  s <<
    "["
    "Measure '" <<
    fMeasureNumber <<
    ", measurePuristNumber: " <<
    fMeasurePuristNumber <<
    ", measureDebugNumber: '" <<
    fMeasureDebugNumber <<
    "', " << measureKindAsString () <<
/* JMI
    ", currentMeasureWholeNotes: " << fCurrentMeasureWholeNotes <<
    ", fullMeasureWholeNotes: " << fFullMeasureWholeNotes <<
    */
    ", " <<
    singularOrPlural (
      fMeasureElementsList.size (), "element", "elements") <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

string msrMeasure::asString () const
{
  stringstream s;

  s <<
    "["
    "Measure '" <<
    fMeasureNumber <<
    ", measurePuristNumber: " <<
    fMeasurePuristNumber <<
    ", measureDebugNumber: '" <<
    fMeasureDebugNumber <<
    "', " << measureKindAsString () <<
    ", currentMeasureWholeNotes: " << fCurrentMeasureWholeNotes <<
    ", fullMeasureWholeNotes: " << fFullMeasureWholeNotes <<
    ", " <<
    singularOrPlural (
      fMeasureElementsList.size (), "element", "elements") <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrMeasure::displayMeasure (
  int    inputLineNumber,
  string context)
{
  gLogIOstream <<
    endl <<
    "*********>> Measure '" <<
    fMeasureNumber <<
    /* JMI
    "', measurePuristNumber: " <<
    fMeasurePuristNumber <<
    ", measureDebugNumber: '" <<
    fMeasureDebugNumber <<
    */
    " (" << context << ")" <<
    ", line " << inputLineNumber <<
    " contains:" <<
    endl;

  gIndenter++;
  print (gLogIOstream);
  gIndenter--;

  gLogIOstream <<
    "<<*********" <<
    endl <<
    endl;
}

void msrMeasure::print (ostream& os)
{
  os <<
    "Measure '" <<
    fMeasureNumber <<
    "', " << measureKindAsString () <<
    ", currentMeasureWholeNotes: " << fCurrentMeasureWholeNotes <<
    ", fullMeasureWholeNotes: " << fFullMeasureWholeNotes <<
    ", " <<
    singularOrPlural (
      fMeasureElementsList.size (), "element", "elements") <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 28;

  os << left <<
    setw (fieldWidth) <<
    "measurePuristNumber" << " : " <<
    fMeasurePuristNumber <<
    endl <<
    setw (fieldWidth) <<
    "measureDebugNumber" << " : " <<
    fMeasureDebugNumber <<
    endl <<

    setw (fieldWidth) <<
    "measureEndRegularKind" << " : " <<
    measureEndRegularKindAsString (
      fMeasureEndRegularKind) <<
    endl <<
    setw (fieldWidth) <<
    "measureRepeatContextKind" << " : " <<
    measureRepeatContextKindAsString (
      fMeasureRepeatContextKind) <<
    endl <<

    setw (fieldWidth) <<
    "measureFirstInVoice" << " : " <<
    booleanAsString (
      fMeasureFirstInVoice) <<
    endl <<
    setw (fieldWidth) <<
    "measureFirstInSegmentKind" << " : " <<
    msrMeasure::measureFirstInSegmentKindAsString (
      fMeasureFirstInSegmentKind) <<
    endl <<

    setw (fieldWidth) <<
    "segmentUpLink" << " : " <<
    fMeasureSegmentUpLink->asShortString () <<
    endl;


#ifdef TRACE_OPTIONS
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

  // fetch voice's clef, key and time
  S_msrClef
    voiceCurrentClef =
      voice->getVoiceCurrentClef ();
  S_msrKey
    voiceCurrentKey =
      voice->getVoiceCurrentKey ();
  S_msrTime
    voiceCurrentTime =
      voice->getVoiceCurrentTime ();

  // print the voice current clef, key and time
  os << left <<
    setw (fieldWidth) << "voiceCurrentClef" << " : ";
  if (voiceCurrentClef) {
    os <<
      voiceCurrentClef;
  }
  else {
    os << "null" << endl;
  }

  os << left <<
    setw (fieldWidth) << "voiceCurrentKey" << " : ";
  if (voiceCurrentKey) {
    os <<
      voiceCurrentKey;
  }
  else {
    os << "null" << endl;
  }

  os << left <<
    setw (fieldWidth) << "voiceCurrentTime" << " : ";
  if (voiceCurrentTime) {
    os <<
      voiceCurrentTime;
  }
  else {
    os << "null" << endl;
  }
#endif

    /* JMI

    setw (fieldWidth) <<
    "currentMeasureWholeNotesAsMSRString" << " : " <<
    currentMeasureWholeNotesAsMSRString () <<
    endl <<
    setw (fieldWidth) <<
    "fullMeasureWholeNotesAsMSRString" << " : " <<
    fullMeasureWholeNotesAsMSRString () <<
    endl <<
      */

  os << left <<
    setw (fieldWidth) <<
    "measureLongestNote" << " : ";
  if (fMeasureLongestNote) {
    os <<
      fMeasureLongestNote->asShortString ();
  }
  else {
    os <<
      "none";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "measureContainsMusic" << " : " <<
    booleanAsString (
      fMeasureContainsMusic) <<
    endl <<

    setw (fieldWidth) <<
    "measureHasBeenFinalized" << " : " <<
    booleanAsString (
      fMeasureHasBeenFinalized) <<
    endl <<
    setw (fieldWidth) <<
    "measureKindAndPuristNumberHaveBeenDetermined" << " : " <<
    booleanAsString (
      fMeasureKindAndPuristNumberHaveBeenDetermined) <<
    endl <<

    setw (fieldWidth) <<
    "measureIsASingleMeasureRest" << " : " <<
    booleanAsString (fMeasureIsASingleMeasureRest) <<
    endl;

  os <<
    setw (fieldWidth) <<
    "nextMeasureNumber" << " : \"" <<
    fNextMeasureNumber <<
    "\"" <<
    endl;

  int
    measureElementsListSize =
      fMeasureElementsList.size ();

  os <<
    setw (fieldWidth) <<
    "measureElementsList" << " : " <<
    singularOrPlural (
      measureElementsListSize, "element", "elements") <<
    endl;

  if (measureElementsListSize) {
    os << endl;

    gIndenter++;

    list<S_msrMeasureElement>::const_iterator
      iBegin = fMeasureElementsList.begin (),
      iEnd   = fMeasureElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }

  gIndenter--;
}

void msrMeasure::shortPrint (ostream& os)
{
  os <<
    "Measure '" <<
    fMeasureNumber <<
    "', " << measureKindAsString () <<
    ", " <<
    singularOrPlural (
      fMeasureElementsList.size (), "element", "elements") <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 28;

  os << left <<
    setw (fieldWidth) <<
    "measurePuristNumber" << " : " <<
    fMeasurePuristNumber <<
    endl <<
    setw (fieldWidth) <<
    "measureDebugNumber" << " : " <<
    fMeasureDebugNumber <<
    endl <<

    setw (fieldWidth) <<
    "measureEndRegularKind" << " : " <<
    measureEndRegularKindAsString (
      fMeasureEndRegularKind) <<
    endl <<
    setw (fieldWidth) <<
    "measureRepeatContextKind" << " : " <<
    measureRepeatContextKindAsString (
      fMeasureRepeatContextKind) <<
    endl <<

    setw (fieldWidth) <<
    "measureFirstInVoice" << " : " <<
    booleanAsString (
      fMeasureFirstInVoice) <<
    endl <<

    setw (fieldWidth) <<
    "measureFirstInSegmentKind" << " : " <<
    msrMeasure::measureFirstInSegmentKindAsString (
      fMeasureFirstInSegmentKind) <<
    endl <<

    setw (fieldWidth) <<
    "measureImplicitKind" << " : " <<
    booleanAsString (
      fMeasureImplicitKind) <<
    endl <<

    setw (fieldWidth) <<
    "measureContainsMusic" << " : " <<
    booleanAsString (
      fMeasureContainsMusic) <<
    endl <<

    setw (fieldWidth) <<
    "measureIsASingleMeasureRest" << " : " <<
    booleanAsString (fMeasureIsASingleMeasureRest) <<
    endl <<

    setw (fieldWidth) <<
    "currentMeasureWholeNotes" << " : " <<
    fCurrentMeasureWholeNotes <<
    endl <<

    setw (fieldWidth) <<
    "fullMeasureWholeNotes" << " : " <<
    fFullMeasureWholeNotes <<
    endl;

#ifdef TRACE_OPTIONS
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

  // fetch voice's clef, key and time
  S_msrClef
    voiceCurrentClef =
      voice->getVoiceCurrentClef ();
  S_msrKey
    voiceCurrentKey =
      voice->getVoiceCurrentKey ();
  S_msrTime
    voiceCurrentTime =
      voice->getVoiceCurrentTime ();

  // print the voice current clef, key and time
  os << left <<
    setw (fieldWidth) << "voiceCurrentClef" << " : ";
  if (voiceCurrentClef) {
    os <<
      voiceCurrentClef;
  }
  else {
    os << "null" << endl;
  }

  os << left <<
    setw (fieldWidth) << "voiceCurrentKey" << " : ";
  if (voiceCurrentKey) {
    os <<
      voiceCurrentKey;
  }
  else {
    os << "null" << endl;
  }

  os << left <<
    setw (fieldWidth) << "voiceCurrentTime" << " : ";
  if (voiceCurrentTime) {
    os <<
      voiceCurrentTime;
  }
  else {
    os << "null" << endl;
  }
#endif

  os <<
    setw (fieldWidth) <<
    "measureContainsMusic" << " : " <<
    booleanAsString (
      fMeasureContainsMusic) <<
    endl <<

    setw (fieldWidth) <<
    "measureHasBeenFinalized" << " : " <<
    booleanAsString (
      fMeasureHasBeenFinalized) <<
    endl <<
    setw (fieldWidth) <<
    "measureKindAndPuristNumberHaveBeenDetermined" << " : " <<
    booleanAsString (
      fMeasureKindAndPuristNumberHaveBeenDetermined) <<
    endl <<

    setw (fieldWidth) <<
    "nextMeasureNumber" << " : \"" <<
    fNextMeasureNumber <<
    "\"" <<
    endl;

  int
    measureElementsListSize =
      fMeasureElementsList.size ();

  os <<
    setw (fieldWidth) <<
    "measureElementsList" << " : " <<
    singularOrPlural (
      measureElementsListSize, "element", "elements") <<
    endl;

  if (measureElementsListSize) {
    gIndenter++;

    list<S_msrMeasureElement>::const_iterator
      iBegin = fMeasureElementsList.begin (),
      iEnd   = fMeasureElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i)->asShortString ();
      if (++i == iEnd) break;
      os << endl;
    } // for
    os << endl;

    gIndenter--;
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrMeasure& elt)
{
  elt->print (os);
  return os;
}


}


/*

    // set the harmony's souding whole notes
    if (harmonyWholeNotesOffset.getNumerator () > 0) {
      // decrement the harmony's duration as much
      harmony->
        setHarmonySoundingWholeNotes (
          newNoteSoundingWholeNotes
            -
          harmonyWholeNotesOffset);
    }
    else {
      harmony->
        setHarmonySoundingWholeNotes (
          fCurrentNoteSoundingWholeNotes
            /
          pendingHarmoniesNumber);
    }


    // has the harmony whole notes offset already been used
    // at the same point in time?
    if (harmonyWholeNotesOffset == previousHarmonyWhoseNotesOffset) {
      stringstream s;

      s <<
        "harmonyWholeNotesOffset '" <<
        harmonyWholeNotesOffset <<
        "' already occured in that same point in time, ignoring it";

      msrMusicXMLWarning (
        gGeneralOptions->fInputSourceName,
        inputLineNumber,
//        __FILE__, __LINE__,
        s.str ());
    }

    else {
    }
*/


