/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "msrMutualDependencies.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "mxmlTreeOah.h"
#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
int msrSegment::gSegmentsCounter = 0;
int msrSegment::gSegmentDebugNumber = 0;

S_msrSegment msrSegment::create (
  int        inputLineNumber,
  S_msrVoice segmentVoicekUpLink)
{
  msrSegment* o =
    new msrSegment (
      inputLineNumber,
      segmentVoicekUpLink);
  assert(o!=0);

  return o;
}

msrSegment::msrSegment (
  int        inputLineNumber,
  S_msrVoice segmentVoicekUpLink)
    : msrVoiceElement (inputLineNumber)
{
  // sanity check
  msrAssert(
    segmentVoicekUpLink != nullptr,
    "segmentVoicekUpLink is null");

  // set segment's voice upLink
  fSegmentVoiceUpLink =
    segmentVoicekUpLink;

  // do other initializations
  initializeSegment ();
}

msrSegment::~msrSegment ()
{}

void msrSegment::initializeSegment ()
{
  fSegmentAbsoluteNumber = ++gSegmentsCounter;
  fSegmentDebugNumber    = ++gSegmentDebugNumber;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceSegments) {
    gLogOstream <<
      "Initializing new segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "', in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\", line " << fInputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceSegments) {
    if (false && fSegmentDebugNumber == 3) { // JMI
      gLogOstream <<
        endl <<
        "======================= initializeSegment()" <<
        endl <<

        fSegmentVoiceUpLink <<

        "=======================" <<
        endl <<
        endl;

      abort ();
    }
  }
#endif
}

S_msrSegment msrSegment::createSegmentNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceSegments) {
    gLogOstream <<
      "Creating a newborn clone of segment " <<
      asString () <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrSegment
    newbornClone =
      msrSegment::create (
        fInputLineNumber,
        containingVoice);

  // absolute number, for coherency between passes
  newbornClone->fSegmentAbsoluteNumber =
    fSegmentAbsoluteNumber;

  // keep debug number fSegmentDebugNumber unchanged

  return newbornClone;
}

S_msrSegment msrSegment::createSegmentDeepCopy (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceSegments) {
    gLogOstream <<
      "Creating a deep copy of segment " <<
      asString () <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrSegment
    segmentDeepCopy =
      msrSegment::create (
        fInputLineNumber,
        containingVoice);

  // absolute number
  segmentDeepCopy->fSegmentAbsoluteNumber =
    fSegmentAbsoluteNumber;

  // keep debug number fSegmentDebugNumber unchanged

  // the measures in the segment contain the mmusic
  int numberOfSegmentMeasures =
   fSegmentMeasuresList.size () ;

  if (numberOfSegmentMeasures) {
#ifdef TRACE_OAH
    if (gTraceOah->fTraceVoices) {
      gLogOstream <<
        "There are " <<
        numberOfSegmentMeasures <<
        " measures in segment to be deep copied" <<
        endl;
    }
#endif

    for (
      list<S_msrMeasure>::const_iterator i = fSegmentMeasuresList.begin ();
      i != fSegmentMeasuresList.end ();
      i++
  ) {
      // append a deep copy of the measure to the deep copy
      segmentDeepCopy->
        appendMeasureToSegment (
          (*i)->
            createMeasureDeepCopy (this));
    } // for
  }

  else {
#ifdef TRACE_OAH
    if (gTraceOah->fTraceSegments) {
      gLogOstream <<
        "There are no measures in segment to be deep copied" <<
        endl;
    }
#endif
  }

  // upLinks
  segmentDeepCopy->fSegmentVoiceUpLink =
    containingVoice;

  return segmentDeepCopy;
}

S_msrPart msrSegment::fetchSegmentPartUpLink () const
{
  return
    fSegmentVoiceUpLink->
      fetchVoicePartUpLink ();
}

void msrSegment::assertSegmentMeasuresListIsNotEmpty (
  int inputLineNumber) const
{
  if (! fSegmentMeasuresList.size ()) {
#ifdef TRACE_OAH
  if (
    gTraceOah->fTraceMeasures
      ||
    gTraceOah->fTraceSegments
      ||
    gTraceOah->fTraceRepeats
  ) {
    fSegmentVoiceUpLink->
      displayVoiceRepeatsStackRestMeasuresMeasuresRepeatAndVoice (
        inputLineNumber,
        "assertSegmentMeasuresListIsNotEmpty()");
  }
#endif

    gLogOstream <<
      "assertSegmentMeasuresListIsNotEmpty()" <<
      ", segmentMeasuresList is empty" <<
      ", segment: " <<
      this->asString () <<
      ", in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      "', line " << inputLineNumber <<
      endl;

    msrAssert (
      false,
      ", segmentMeasuresList is empty");
  }
}

S_msrMeasure msrSegment::createMeasureAndAppendItToSegment (
  int    inputLineNumber,
  string measureNumber,
  msrMeasure::msrMeasureImplicitKind
         measureImplicitKind)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
      "Creating and appending a measure '" << measureNumber <<
      "', to segment '" << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  // determine new measure 'first in segment' kind
  msrMeasure::msrMeasureFirstInSegmentKind
    measureFirstInSegmentKind;

  if (fSegmentMeasuresList.size () == 0) {
    // this is the first measure in the segment
    measureFirstInSegmentKind =
      msrMeasure::kMeasureFirstInSegmentKindYes;
  }
  else {
    // this is not the first measure in the segment
    measureFirstInSegmentKind =
      msrMeasure::kMeasureFirstInSegmentKindNo;
  }

  // create a measure
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
      "Creating measure '" << measureNumber <<
      "' in segment " << asString () <<
      ", in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      ", measureFirstInSegmentKind: " <<
      msrMeasure::measureFirstInSegmentKindAsString (
        measureFirstInSegmentKind) <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  S_msrMeasure
    result =
      msrMeasure::create (
        inputLineNumber,
        measureNumber,
        this);

  // append it to the segment
  appendMeasureToSegment (result);

  gIndenter--;

  return result;
}

void msrSegment::setNextMeasureNumberInSegment (
  int    inputLineNumber,
  string nextMeasureNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
      "Setting next measure number to '" << nextMeasureNumber <<
      "' in segment '" << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  if (fSegmentMeasuresList.size ()) { // JMI ???
#ifdef TRACE_OAH
    if (gTraceOah->fTraceMeasures) {
      gLogOstream <<
        "Setting next measure number to '" << nextMeasureNumber <<
        "' in segment '" << asString () <<
        "'s last measure " <<
        "' in voice \"" <<
        fSegmentVoiceUpLink->getVoiceName () <<
        "\"" <<
        "', line " << inputLineNumber <<
        endl;
    }
#endif

    fSegmentMeasuresList.back ()->
      setNextMeasureNumber (
        nextMeasureNumber);
  }

  gIndenter--;
}

void msrSegment::appendClefToSegment (S_msrClef clef)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceClefs) {
    gLogOstream <<
      "Appending clef '" << clef->asString () <<
      "' to segment " << asString () <<
      ", in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;

  // sanity check
  if (fSegmentMeasuresList.size () == 0) {
    stringstream s;

    s <<
      "SegmentMeasuresList is empty"  <<
      " in segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"";

    gLogOstream <<
      "SegmentVoiceUpLink:" <<
      endl;
    gIndenter++;
    gLogOstream <<
      fSegmentVoiceUpLink <<
      endl;
    gIndenter--;

    msrInternalError (
      gOahOah->fInputSourceName,
      clef->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // register clef in segments's current measure
  fSegmentMeasuresList.back ()->
    appendClefToMeasure (clef);

  gIndenter--;
}

void msrSegment::prependClefToSegment (S_msrClef clef) // JMI
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceClefs) {
    gLogOstream <<
      "Prepending clef '" << clef->asString () <<
      "' to segment " << asString () <<
      ", in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;

  // sanity check
  if (fSegmentMeasuresList.size () == 0) {
    stringstream s;

    s <<
      "SegmentMeasuresList is empty"  <<
      " in segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"";

    gLogOstream <<
      "SegmentVoiceUpLink:" <<
      endl;
    gIndenter++;
    gLogOstream <<
      fSegmentVoiceUpLink <<
      endl;
    gIndenter--;

    msrInternalError (
      gOahOah->fInputSourceName,
      clef->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // register clef in segments's current measure
  fSegmentMeasuresList.front ()->
    appendClefToMeasure (clef);

  gIndenter--;
}

void msrSegment::appendKeyToSegment (S_msrKey key)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceKeys) {
    gLogOstream <<
      "Appending key '" << key->asString () <<
      "' to segment " << asString () <<
    ", in voice \"" <<
    fSegmentVoiceUpLink->getVoiceName () <<
    "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    key->getInputLineNumber ());

  gIndenter++;

  // register key in segments's current measure
  fSegmentMeasuresList.back ()->
    appendKeyToMeasure (key);

  gIndenter--;
}

void msrSegment::appendTimeToSegment (S_msrTime time)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTimes) {
    gLogOstream <<
      "Appending time:" <<
      endl;

    gIndenter++;

    gLogOstream <<
      time;

    gIndenter--;

    gLogOstream <<
      "to segment " << asString () <<
      ", in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    time->getInputLineNumber ());

  gIndenter++;

  // append time to segments's current measure
  fSegmentMeasuresList.back ()->
    appendTimeToMeasure (time);

  gIndenter--;
}

void msrSegment::appendTimeToSegmentClone (S_msrTime time)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTimes) {
    gLogOstream <<
      "Appending time:" <<
      endl;

    gIndenter++;

    gLogOstream <<
      time;

    gIndenter--;

    gLogOstream <<
      "to segment clone " << asString () <<
      ", in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    time->getInputLineNumber ());

  // append time to segments's current measure
  fSegmentMeasuresList.back ()->
    appendTimeToMeasureClone (time);

  gIndenter--;
}

void msrSegment::appendHarmonyToSegment (S_msrHarmony harmony)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceHarmonies) {
    gLogOstream <<
      "Appending harmony " <<
      harmony->asString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  int inputLineNumber =
    harmony->getInputLineNumber ();

  gIndenter++;

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    inputLineNumber);

  // append harmony to this segment
  fSegmentMeasuresList.back ()->
    appendHarmonyToMeasure (harmony);

  gIndenter--;
}

void msrSegment::appendHarmonyToSegmentClone (S_msrHarmony harmony)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceHarmonies) {
    gLogOstream <<
      "Appending harmony " << harmony->asString () <<
      " to segment clone " << asString () <<
      "' in voice clone \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    harmony->getInputLineNumber ());

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendHarmonyToMeasureClone (harmony);

  gIndenter--;
}

void msrSegment::appendFiguredBassToSegment (
  S_msrFiguredBass figuredBass)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceFiguredBasses) {
    gLogOstream <<
      "Appending figured bass " <<
      figuredBass->asString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  int inputLineNumber =
    figuredBass->getInputLineNumber ();

  gIndenter++;

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    inputLineNumber);

  // append figuredBass to this segment
  fSegmentMeasuresList.back ()->
    appendFiguredBassToMeasure (figuredBass);

  gIndenter--;
}

void msrSegment::appendFiguredBassToSegmentClone (
  S_msrFiguredBass figuredBass)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceFiguredBasses) {
    gLogOstream <<
      "Appending figured bass " <<
      figuredBass->asString () <<
      " to segment clone " << asString () <<
      "' in voice clone \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    figuredBass->getInputLineNumber ());

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendFiguredBassToMeasure (figuredBass);

  gIndenter--;
}

void msrSegment::appendSegnoToSegment (S_msrSegno segno)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceSegnos) {
    gLogOstream <<
      "Appending segno " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    segno->getInputLineNumber ());

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendSegnoToMeasure (segno);

  gIndenter--;
}

void msrSegment::appendCodaToSegment (S_msrCoda coda)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceCodas) {
    gLogOstream <<
      "Appending coda " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    coda->getInputLineNumber ());

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendCodaToMeasure (coda);

  gIndenter--;
}

void msrSegment::appendEyeGlassesToSegment (
  S_msrEyeGlasses eyeGlasses)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceEyeGlasses) {
    gLogOstream <<
      "Appending eyeGlasses " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    eyeGlasses->getInputLineNumber ());

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendEyeGlassesToMeasure (eyeGlasses);

  gIndenter--;
}

void msrSegment::appendPedalToSegment (S_msrPedal pedal)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTracePedals) {
    gLogOstream <<
      "Appending pedal " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }

#endif
  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    pedal->getInputLineNumber ());

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendPedalToMeasure (pedal);

  gIndenter--;
}

void msrSegment::appendDampToSegment (S_msrDamp damp)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceDamps) {
    gLogOstream <<
      "Appending damp " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    damp->getInputLineNumber ());

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendDampToMeasure (damp);

  gIndenter--;
}

void msrSegment::appendDampAllToSegment (S_msrDampAll dampAll)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceDampAlls) {
    gLogOstream <<
      "Appending damp all " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    dampAll->getInputLineNumber ());

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendDampAllToMeasure (dampAll);

  gIndenter--;
}

void msrSegment::appendTransposeToSegment (
  S_msrTranspose transpose)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTranspositions) {
    gLogOstream <<
      "Appending transpose " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    transpose->getInputLineNumber ());

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendTransposeToMeasure (transpose);

  gIndenter--;
}

void msrSegment::appendPartNameDisplayToSegment (
  S_msrPartNameDisplay partNameDisplay)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceSegments) {
    gLogOstream <<
      "Appending part name display " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    partNameDisplay->getInputLineNumber ());

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendPartNameDisplayToMeasure (partNameDisplay);

  gIndenter--;
}

void msrSegment::appendPartAbbreviationDisplayToSegment (
  S_msrPartAbbreviationDisplay partAbbreviationDisplay)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceSegments) {
    gLogOstream <<
      "Appending part abbreviation display " <<

      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    partAbbreviationDisplay->getInputLineNumber ());

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendPartAbbreviationDisplayToMeasure (
      partAbbreviationDisplay);

  gIndenter--;
}

void msrSegment::appendStaffDetailsToSegment (
  S_msrStaffDetails staffDetails)
{
  int inputLineNumber =
    staffDetails->getInputLineNumber ();

#ifdef TRACE_OAH
  if (
    gTraceOah->fTraceStaffDetails
      ||
    gTraceOah->fTraceSegments
  ) {
    gLogOstream <<
      "Appending staff details '" <<
      staffDetails->asShortString () <<
      "' to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\" line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (
    gTraceOah->fTraceStaffDetails
      ||
    gTraceOah->fTraceSegments
  ) {
    fSegmentVoiceUpLink->
      displayVoice (
        inputLineNumber,
        "appendStaffDetailsToSegment() 1");
  }
#endif

  gIndenter++;

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    staffDetails->getInputLineNumber ());

  // append staffDetails to this segment
  fSegmentMeasuresList.back ()->
    appendStaffDetailsToMeasure (staffDetails);

#ifdef TRACE_OAH
  if (
    gTraceOah->fTraceStaffDetails
      ||
    gTraceOah->fTraceSegments
  ) {
    fSegmentVoiceUpLink->
      displayVoice (
        inputLineNumber,
        "appendStaffDetailsToSegment() 2");
  }
#endif

  gIndenter--;
}

void msrSegment::appendLineBreakToSegment (S_msrLineBreak lineBreak)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceLineBreaks) {
    gLogOstream <<
      "Appending break '" <<
      lineBreak->asShortString () <<
      "' to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    lineBreak->getInputLineNumber ());

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendLineBreakToMeasure (lineBreak);

  gIndenter--;
}

void msrSegment::appendPageBreakToSegment (S_msrPageBreak pageBreak)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTracePageBreaks) {
    gLogOstream <<
      "Appending break " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    pageBreak->getInputLineNumber ());

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendPageBreakToMeasure (pageBreak);

  gIndenter--;
}

void msrSegment::appendBarNumberCheckToSegment (
  S_msrBarNumberCheck barNumberCheck)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceBarNumberChecks) {
    gLogOstream <<
      "Appending bar number check " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    barNumberCheck->getInputLineNumber ());

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendBarNumberCheckToMeasure (barNumberCheck);

  gIndenter--;
}

void msrSegment::appendTempoToSegment (
  S_msrTempo tempo)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTempos) {
    gLogOstream <<
      "Appending tempo " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    tempo->getInputLineNumber ());

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendTempoToMeasure (tempo);

  gIndenter--;
}

void msrSegment::appendRehearsalToSegment (
  S_msrRehearsal rehearsal)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRehearsals) {
    gLogOstream <<
      "Appending rehearsal " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    rehearsal->getInputLineNumber ());

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendRehearsalToMeasure (rehearsal);

  gIndenter--;
}

void msrSegment::appendOctaveShiftToSegment (
  S_msrOctaveShift octaveShift)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOctaveShifts) {
    gLogOstream <<
      "Appending octave shift '" <<
      octaveShift->octaveShiftKindAsString () <<
      "' to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    octaveShift->getInputLineNumber ());

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendOctaveShiftToMeasure (octaveShift);

  gIndenter--;
}

void msrSegment::appendScordaturaToSegment (
  S_msrScordatura scordatura)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceScordaturas) {
    gLogOstream <<
      "Appending scordatura '" <<
  // JMI ???    scordatura->octaveShiftKindAsString () <<
      "' to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    scordatura->getInputLineNumber ());

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendScordaturaToMeasure (scordatura);

  gIndenter--;
}

void msrSegment::appendAccordionRegistrationToSegment (
  S_msrAccordionRegistration
    accordionRegistration)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceSegments) {
    gLogOstream <<
      "Appending accordion registration '" <<
      accordionRegistration->asString () <<
      "' to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    accordionRegistration->getInputLineNumber ());

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendAccordionRegistrationToMeasure (
      accordionRegistration);

  gIndenter--;
}

void msrSegment::appendHarpPedalsTuningToSegment (
  S_msrHarpPedalsTuning
    harpPedalsTuning)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceSegments) {
    gLogOstream <<
      "Appending staff pedals tuning '" <<
      harpPedalsTuning->asString () <<
      "' to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    harpPedalsTuning->getInputLineNumber ());

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendHarpPedalsTuningToMeasure (
      harpPedalsTuning);

  gIndenter--;
}

void msrSegment::padUpToPositionInMeasureInSegment (
  int      inputLineNumber,
  rational wholeNotes)
{
#ifdef TRACE_OAH
  if (
    gTraceOah->fTraceSegments
      ||
    gTraceOah->fTraceMeasures
      ||
    gMusicXMLOah->fTraceBackup
  ) {
    gLogOstream <<
      "Padding up to current whole notes '" << wholeNotes <<
      "' in segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  if (false && fSegmentMeasuresList.size () == 0) { // JMI
    stringstream s;

    s <<
      "SegmentMeasuresList is empty"  <<
      " in segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"";

    gLogOstream <<
      "SegmentVoiceUpLink:" <<
      endl;
    gIndenter++;
    gLogOstream <<
      fSegmentVoiceUpLink <<
      endl;
    gIndenter--;

    gLogOstream <<
      "Part:" <<
      endl;
    gIndenter++;
    gLogOstream <<
      fSegmentVoiceUpLink->fetchVoicePartUpLink () <<
      endl;
    gIndenter--;

    msrInternalError (
      gOahOah->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  if (fSegmentMeasuresList.size ()) { // JMI BOFBOF
    // pad last measure up to to this actual wholes notes
    fSegmentMeasuresList.back ()->
      padUpToPositionInMeasureInMeasure (
        inputLineNumber,
        wholeNotes);
  }
}

void msrSegment::backupByWholeNotesStepLengthInSegment (
  int      inputLineNumber,
  rational backupTargetMeasureElementPositionInMeasure)
{
#ifdef TRACE_OAH
  if (
    gTraceOah->fTraceSegments
      ||
    gTraceOah->fTraceMeasures
      ||
    gMusicXMLOah->fTraceBackup
  ) {
    gLogOstream <<
      "Backup by a '" <<
      backupTargetMeasureElementPositionInMeasure <<
      "' whole notes step length in segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  if (fSegmentMeasuresList.size ()) { // JMI BOFBOF
    // pad last measure up to to this actual wholes notes
    fSegmentMeasuresList.back ()->
      backupByWholeNotesStepLengthInMeasure (
        inputLineNumber,
        backupTargetMeasureElementPositionInMeasure);
  }
}

void msrSegment::appendPaddingNoteToSegment (
  int      inputLineNumber,
  rational forwardStepLength)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotes) {
    gLogOstream <<
      "Appending padding note" <<
      ", forwardStepLength: " <<
      forwardStepLength <<
      ", to segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  if (fSegmentMeasuresList.size ()) { // JMI BOFBOF
    // append a padding note to the segment's last measure
    fSegmentMeasuresList.back ()->
      appendPaddingNoteToMeasure (
        inputLineNumber,
        forwardStepLength);
  }

  gIndenter--;
}

void msrSegment::appendMeasureToSegment (S_msrMeasure measure)
{
  int inputLineNumber =
    measure->getInputLineNumber ();

  string measureNumber =
    measure->getMeasureElementMeasureNumber ();

  int segmentMeasuresListSize =
    fSegmentMeasuresList.size ();

  string currentMeasureNumber =
    segmentMeasuresListSize == 0
      ? ""
      : fSegmentMeasuresList.back ()->getMeasureElementMeasureNumber ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
      "Appending measure '" << measureNumber <<
      "' to segment " << asString ();

    if (fSegmentMeasuresList.size () == 0)
      gLogOstream <<
        ", as first measure";
    else
      gLogOstream <<
      ", after measure number '" << currentMeasureNumber << "'";

    gLogOstream <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      ", line " << measure->getInputLineNumber () <<
      endl;
  }
#endif

  if (measureNumber == currentMeasureNumber) {
    stringstream s;

    s <<
      "measure number '" << measureNumber <<
      "' occurs twice in a row";

    msrInternalError (
      gOahOah->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // is measure the first one this segment?
  if (segmentMeasuresListSize == 0) {
    measure->
      setMeasureFirstInSegmentKind (
        msrMeasure::kMeasureFirstInSegmentKindYes);
  }
  else {
    measure->
      setMeasureFirstInSegmentKind (
        msrMeasure::kMeasureFirstInSegmentKindNo);
  }

  // is measure the first one it the voice?
  // this is necessary for voice clones,
  // which don't go down the part-staff-voice-segment hierarchy
  if (! fSegmentVoiceUpLink->getVoiceFirstMeasure ()) {
    // yes, register it as such
    fSegmentVoiceUpLink->
      setVoiceFirstMeasure (measure);

    measure->
      setMeasureFirstInVoice ();
  }

  // append measure to the segment
  fSegmentMeasuresList.push_back (measure);
}

void msrSegment::prependMeasureToSegment (S_msrMeasure measure)
{
  int inputLineNumber =
    measure->getInputLineNumber ();

  string measureNumber =
    measure->getMeasureElementMeasureNumber ();

  int segmentMeasuresListSize =
    fSegmentMeasuresList.size ();

  string currentMeasureNumber =
    segmentMeasuresListSize == 0
      ? ""
      : fSegmentMeasuresList.back ()->getMeasureElementMeasureNumber ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
      "Prepending measure " << measureNumber <<
      " to segment " << asString ();

    if (segmentMeasuresListSize == 0) {
      gLogOstream <<
        ", as first measure FOO"; // JMI
    }

/* JMI
    else
      gLogOstream <<
      ", after measure number '" << currentMeasureNumber << "'";
*/

    gLogOstream <<
      " in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"," <<
      ", line " << measure->getInputLineNumber () <<
      endl;
  }
#endif

  if (measureNumber == currentMeasureNumber) {
    stringstream s;

    s <<
      "measure number '" << measureNumber <<
      "' occurs twice in a row";

  // JMI  msrInternalError (
    msrInternalWarning (
      gOahOah->fInputSourceName,
      inputLineNumber,
      s.str ());
  }

  measure->
    setMeasureFirstInVoice ();

  // prepend measure to the segment
  fSegmentMeasuresList.push_front (measure);
}

void msrSegment::prependBarlineToSegment (S_msrBarline barline)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceBarlines) {
    gLogOstream <<
      "Prepending barline '" << barline->asString () <<
      "' to segment '" << asString () << "'" <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"," <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    barline->getInputLineNumber ());

  // prepend barline to this segment
  gIndenter++;

  fSegmentMeasuresList.front ()->
    prependBarlineToMeasure (barline);

  gIndenter--;
}

void msrSegment::appendBarlineToSegment (S_msrBarline barline)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceBarlines) {
    gLogOstream <<
      "Appending barline " << barline->asString () <<
      " to segment " << asString () << "'" <<
      " in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"," <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    barline->getInputLineNumber ());

  gIndenter++;

  // append barline to this segment
  fSegmentMeasuresList.back ()->
    appendBarlineToMeasure (barline);

  gIndenter--;
}

void msrSegment::appendBarCheckToSegment (S_msrBarCheck barCheck)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceBarChecks) {
    gLogOstream <<
      "Appending bar check '" << barCheck->asString () <<
      "' to segment '" << asString () << "'" <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"," <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    barCheck->getInputLineNumber ());

  fSegmentMeasuresList.back ()->
    appendBarCheckToMeasure (barCheck);
}

void msrSegment::appendVoiceStaffChangeToSegment (
  S_msrVoiceStaffChange voiceStaffChange)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceStaffChanges) {
    gLogOstream <<
      "Appending voice staff change '" <<
      voiceStaffChange->asString () <<
      "' to segment \"" << asString () << "\"" <<
      ", line " << voiceStaffChange->getInputLineNumber () <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    voiceStaffChange->getInputLineNumber ());

  gIndenter++;

  fSegmentMeasuresList.back ()->
    appendVoiceStaffChangeToMeasure (
      voiceStaffChange);

  gIndenter--;
}

void msrSegment::appendNoteToSegment (
  S_msrNote note,
  rational  partCurrentPositionInMeasure)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotes) {
    if (! fSegmentMeasuresList.size ()) { // JMI
      displaySegment (
        note->getInputLineNumber (),
        "appendNoteToSegment()");
    }
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    note->getInputLineNumber ());

  fSegmentMeasuresList.back ()->
    appendNoteToMeasure (
      note,
      partCurrentPositionInMeasure);
}

void msrSegment::appendNoteToSegmentClone (S_msrNote note)
{
  fSegmentMeasuresList.back ()->
    appendNoteToMeasureClone (note);
}

void msrSegment::appendDoubleTremoloToSegment ( // JMI
  S_msrDoubleTremolo doubleTremolo)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTremolos) {
    gLogOstream <<
      "Appending double tremolo " <<
      doubleTremolo->asShortString () <<
      " to segment '" << asString () << "'" <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"," <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    doubleTremolo->getInputLineNumber ());

  fSegmentMeasuresList.back ()->
    appendDoubleTremoloToMeasure (doubleTremolo);
}

void msrSegment::appendChordToSegment (S_msrChord chord) // JMI
{
  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    chord->getInputLineNumber ());

  fSegmentMeasuresList.back ()->
    appendChordToMeasure (chord);
}

void msrSegment::appendTupletToSegment (S_msrTuplet tuplet) // JMI
{
  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    tuplet->getInputLineNumber ());

  fSegmentMeasuresList.back ()->
    appendTupletToMeasure (tuplet);
}

/* JMI
void msrSegment::addGraceNotesGroupAheadOfSegmentIfNeeded (
  S_msrGraceNotesGroup graceNotesGroup)

{
  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    graceNotesGroup->getInputLineNumber ());

  fSegmentMeasuresList.front ()->
    addGraceNotesGroupAheadOfMeasure (graceNotesGroup);
}

void msrSegment::appendGraceNotesToSegment (
  S_msrGraceNotes graceNotes)
{
  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    graceNotes->getInputLineNumber ());

  fSegmentMeasuresList.back ()->
    appendGraceNotesToMeasure (graceNotes);
}

void msrSegment::appendAfterGraceNotesToSegment (
  S_msrAfterGraceNotes afterGraceNotes)
{
  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    afterGraceNotes->getInputLineNumber ());

  fSegmentMeasuresList.back ()->
    appendAfterGraceNotesToMeasure (afterGraceNotes);
}

void msrSegment::prependAfterGraceNotesToSegment (
  S_msrAfterGraceNotes afterGraceNotes)

{
  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    afterGraceNotes->getInputLineNumber ());

  fSegmentMeasuresList.front ()->
    prependAfterGraceNotesToMeasure (afterGraceNotes); // JMI
}
*/

void msrSegment::prependOtherElementToSegment (
  S_msrMeasureElement elem)
{
  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    elem->getInputLineNumber ());

  fSegmentMeasuresList.back ()->
    prependOtherElementToMeasure (elem);
}

void msrSegment::appendOtherElementToSegment (
  S_msrMeasureElement elem)
{
  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    elem->getInputLineNumber ());

  fSegmentMeasuresList.back ()->
    appendOtherElementToMeasure (elem);
}

/* JMI
S_msrElement msrSegment::removeLastElementFromSegment (
  int inputLineNumber)
{
  // this last element can be a note or a tuplet,
  // this method is used when the seconde note of a chord is mest

  if (fSegmentMeasuresList.size ()) {
    return
      fSegmentMeasuresList.back ()->
        removeLastElementFromMeasure (
          inputLineNumber);
  }

  else {
    msrInternalError (
      inputLineNumber,
      "cannot removeLastElementFromSegment () " <<
      asString () <<
      " since it is empty");
  }
}
*/

void msrSegment::removeNoteFromSegment (
  int       inputLineNumber,
  S_msrNote note)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotes) {
    gLogOstream <<
      "Removing note '" <<
      note->asString () <<
      "'from segment '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  gIndenter++;

  if (fSegmentMeasuresList.size ()) {
    fSegmentMeasuresList.back ()->
      removeNoteFromMeasure (
        inputLineNumber,
        note);
  }

  else {
    stringstream s;

    s <<
      "cannot remove note from segment " <<
      asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"," <<
      " since it is empty";

    msrInternalError (
      gOahOah->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  gIndenter--;
}

void msrSegment::removeElementFromSegment (
  int          inputLineNumber,
  S_msrElement element)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceSegments) {
    gLogOstream <<
      "Removing element '" <<
      element->asString () <<
      "'from segment '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

 if (fSegmentMeasuresList.size ()) {
    fSegmentMeasuresList.back ()->
      removeElementFromMeasure (
        inputLineNumber,
        element);
  }

  else {
    stringstream s;

    s <<
      "cannot remove note from segment " <<
      asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"," <<
      " since it is empty";

    msrInternalError (
      gOahOah->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

S_msrMeasure msrSegment::fetchLastMeasureFromSegment (
  int    inputLineNumber,
  string context)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
      "Fetching last measure from segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      " (" << context << ")" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  if (! fSegmentMeasuresList.size ()) {
    stringstream s;

    s <<
      "cannot fetch last measure from segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"," <<
      "' since it is empty";

    msrInternalError (
      gOahOah->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  S_msrMeasure
    result =
      fSegmentMeasuresList.back ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
      endl <<
      "The fetched measure contains:" <<
      endl;

    gIndenter++;

    gLogOstream <<
      result <<
      endl;

    gIndenter--;
  }
#endif

  return result;
}

S_msrMeasure msrSegment::removeLastMeasureFromSegment (
  int    inputLineNumber,
  string context)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
      "Removing last measure from segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      " (" << context << ")" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresDetails) {
    fSegmentVoiceUpLink->
      displayVoiceRepeatsStackRestMeasuresMeasuresRepeatAndVoice (
        inputLineNumber,
        "removeLastMeasureFromSegment() 1");
  }
#endif

  if (! fSegmentMeasuresList.size ()) {
    stringstream s;

    s <<
      "cannot remove last measure from segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"," <<
      "' since it is empty";

    msrInternalError (
      gOahOah->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  S_msrMeasure
    result =
      fSegmentMeasuresList.back ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
      endl <<
      "The removed measure contains:" <<
      endl;

    gIndenter++;

    gLogOstream <<
      result <<
      endl;

    gIndenter--;
  }
#endif

  // JMI for tests abort ();

  fSegmentMeasuresList.pop_back ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresDetails) {
    fSegmentVoiceUpLink->
      displayVoiceRepeatsStackRestMeasuresMeasuresRepeatAndVoice (
        inputLineNumber,
        "removeLastMeasureFromSegment() 2");
  }
#endif

  return result;
}

void msrSegment::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrSegment::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrSegment>*
    p =
      dynamic_cast<visitor<S_msrSegment>*> (v)) {
        S_msrSegment elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrSegment::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSegment::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrSegment::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrSegment>*
    p =
      dynamic_cast<visitor<S_msrSegment>*> (v)) {
        S_msrSegment elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrSegment::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrSegment::browseData (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrSegment::browseData ()" <<
      endl;
  }

/* JMI
  gLogOstream <<
    gTab <<
    "==========>>> " <<
    fSegmentMeasuresList.size () << " measures" << endl;
    */

  for (
    list<S_msrMeasure>::const_iterator i = fSegmentMeasuresList.begin ();
    i != fSegmentMeasuresList.end ();
    i++
  ) {
    // browse the element
    msrBrowser<msrMeasure> browser (v);
    browser.browse (*(*i));
  } // for

  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% <== msrSegment::browseData ()" <<
      endl;
  }
}

string msrSegment::asShortString () const
{
  stringstream s;

  s <<
    "[" <<
    "Segment '" <<
    fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
    "', voice: \"" <<
    fSegmentVoiceUpLink->getVoiceName () <<
    "\"" <<
    "]";

  return s.str ();
}

string msrSegment::asString () const
{
  stringstream s;

  s <<
    "[" <<
    "Segment '" <<
    fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
    "' in voice \"" <<
    fSegmentVoiceUpLink->getVoiceName () <<
    "\"";

  if (! fSegmentMeasuresList.size ()) {
    s <<
      " (0 measures)";
  }
  else {
    s <<
      " (" <<
      singularOrPlural (
        fSegmentMeasuresList.size (), "measure", " measures") <<
      "), i.e. [";

    list<S_msrMeasure>::const_iterator
      iBegin = fSegmentMeasuresList.begin (),
      iEnd   = fSegmentMeasuresList.end (),
      i      = iBegin;

    for ( ; ; ) {
      s << (*i)->getMeasureElementMeasureNumber ();
      if (++i == iEnd) break;
      s << ", ";
    } // for

    s << "]";
  }

  s << "]";

  return s.str ();
}

void msrSegment::displaySegment (
  int    inputLineNumber,
  string context)
{
  gLogOstream <<
    endl <<
    "*********>> Segment '" <<
    fSegmentAbsoluteNumber <<
    ", segmentDebugNumber: '" <<
    fSegmentDebugNumber <<
    /* JMI
    "', score:" <<
    endl <<
    fSegmentVoiceUpLink->
      getVoiceStaffUpLink ()->
        getStaffPartUpLink ()->
          getPartPartGroupUpLink ()->
            getPartGroupScoreUpLink () <<
            */
    "', voice:" <<
    endl <<
    fSegmentVoiceUpLink->getVoiceName () <<
    " (" << context << ")" <<
    ", line " << inputLineNumber <<
    " contains:" <<
    endl;

  gIndenter++;
  print (gLogOstream);
  gIndenter--;

  gLogOstream <<
    "<<*********" <<
    endl <<
    endl;
}

void msrSegment::print (ostream& os) const
{
  os <<
    "Segment '" <<
    fSegmentAbsoluteNumber <<
    ", segmentDebugNumber: '" <<
    fSegmentDebugNumber <<
    "', " <<
    singularOrPlural (
      fSegmentMeasuresList.size (), "measure", "measures") <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 20;

  os <<
    setw (fieldWidth) <<
    "voiceUpLink" << " : " <<
    "\"" <<
    fSegmentVoiceUpLink->getVoiceName () <<
    "\"" <<
    endl;

  if (! fSegmentMeasuresList.size ()) {
    os <<
      setw (fieldWidth) <<
      "segmentMeasuresList" << " : " << "none" <<
      endl;
  }

  else {
    os << endl;

    list<S_msrMeasure>::const_iterator
      iBegin = fSegmentMeasuresList.begin (),
      iEnd   = fSegmentMeasuresList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  gIndenter--;
}

void msrSegment::shortPrint (ostream& os) const
{
  os <<
    "Segment '" <<
    fSegmentAbsoluteNumber <<
    ", segmentDebugNumber: '" <<
    fSegmentDebugNumber <<
    "', " <<
    singularOrPlural (
      fSegmentMeasuresList.size (), "measure", "measures") <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 20;

  /* JMI
  os <<
    setw (fieldWidth) << "voiceUpLink" << " : " <<
    "\"" <<
    fSegmentVoiceUpLink->getVoiceName () <<
    "\"" <<
    endl;
    */

  if (! fSegmentMeasuresList.size ()) {
    os <<
      setw (fieldWidth) <<
      "segmentMeasuresList" << " : " << "none" <<
      endl;
  }

  else {
    list<S_msrMeasure>::const_iterator
      iBegin = fSegmentMeasuresList.begin (),
      iEnd   = fSegmentMeasuresList.end (),
      i      = iBegin;

    for ( ; ; ) {
      (*i)->shortPrint (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  os << endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrSegment& elt)
{
  elt->print (os);
  return os;
}


}
