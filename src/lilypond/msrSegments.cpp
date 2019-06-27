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

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSegments) {
    gLogIOstream <<
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

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSegments) {
    if (false && fSegmentDebugNumber == 3) { // JMI
      gLogIOstream <<
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

S_msrPart msrSegment::fetchSegmentPartUpLink () const
{
  return
    fSegmentVoiceUpLink->
      fetchVoicePartUpLink ();
}

S_msrSegment msrSegment::createSegmentNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSegments) {
    gLogIOstream <<
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
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSegments) {
    gLogIOstream <<
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
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceVoices) {
      gLogIOstream <<
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
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceSegments) {
      gLogIOstream <<
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

S_msrMeasure msrSegment::createMeasureAndAppendItToSegment (
  int    inputLineNumber,
  string measureNumber,
  msrMeasure::msrMeasureImplicitKind
         measureImplicitKind)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
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
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
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
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
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
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
      gLogIOstream <<
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
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceClefs || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
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

    gLogIOstream <<
      "SegmentVoiceUpLink:" <<
      endl;
    gIndenter++;
    gLogIOstream <<
      fSegmentVoiceUpLink <<
      endl;
    gIndenter--;

    msrInternalError (
      gGeneralOptions->fInputSourceName,
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
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceClefs || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
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

    gLogIOstream <<
      "SegmentVoiceUpLink:" <<
      endl;
    gIndenter++;
    gLogIOstream <<
      fSegmentVoiceUpLink <<
      endl;
    gIndenter--;

    msrInternalError (
      gGeneralOptions->fInputSourceName,
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
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceKeys || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending key '" << key->asString () <<
      "' to segment " << asString () <<
    ", in voice \"" <<
    fSegmentVoiceUpLink->getVoiceName () <<
    "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;

  // register key in segments's current measure
  fSegmentMeasuresList.back ()->
    appendKeyToMeasure (key);

  gIndenter--;
}

void msrSegment::appendTimeToSegment (S_msrTime time)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTimes || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending time:" <<
      endl;

    gIndenter++;

    gLogIOstream <<
      time;

    gIndenter--;

    gLogIOstream <<
      "to segment " << asString () <<
      ", in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;

  // append time to segments's current measure
  fSegmentMeasuresList.back ()->
    appendTimeToMeasure (time);

  gIndenter--;
}

void msrSegment::appendTimeToSegmentClone (S_msrTime time)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTimes || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending time:" <<
      endl;

    gIndenter++;

    gLogIOstream <<
      time;

    gIndenter--;

    gLogIOstream <<
      "to segment clone " << asString () <<
      ", in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  // append time to segments's current measure
  fSegmentMeasuresList.back ()->
    appendTimeToMeasureClone (time);

  gIndenter--;
}

void msrSegment::appendHarmonyToSegment (S_msrHarmony harmony)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending harmony " << harmony->asString () <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  // append it to this segment
  fSegmentMeasuresList.back ()-> // JMI ???
    appendHarmonyToMeasure (harmony);

  gIndenter--;
}

void msrSegment::appendHarmonyToSegmentClone (S_msrHarmony harmony)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending harmony " << harmony->asString () <<
      " to segment clone " << asString () <<
      "' in voice clone \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendHarmonyToMeasureClone (harmony);

  gIndenter--;
}

void msrSegment::appendFiguredBassToSegment (
  S_msrFiguredBass figuredBass)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceFiguredBasses || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending figured bass " << figuredBass->asString () <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()-> // JMI ???
    appendFiguredBassToMeasure (figuredBass);

  gIndenter--;
}

void msrSegment::appendFiguredBassToSegmentClone (
  S_msrFiguredBass figuredBass)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceFiguredBasses || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending figured bass " << figuredBass->asString () <<
      " to segment clone " << asString () <<
      "' in voice clone \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendFiguredBassToMeasure (figuredBass);

  gIndenter--;
}

void msrSegment::appendSegnoToSegment (S_msrSegno segno)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSegnos || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending segno " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendSegnoToMeasure (segno);

  gIndenter--;
}

void msrSegment::appendCodaToSegment (S_msrCoda coda)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceCodas || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending coda " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendCodaToMeasure (coda);

  gIndenter--;
}

void msrSegment::appendEyeGlassesToSegment (
  S_msrEyeGlasses eyeGlasses)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceEyeGlasses || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending eyeGlasses " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendEyeGlassesToMeasure (eyeGlasses);

  gIndenter--;
}

void msrSegment::appendPedalToSegment (S_msrPedal pedal)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePedals || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending pedal " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }

#endif
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendPedalToMeasure (pedal);

  gIndenter--;
}

void msrSegment::appendDampToSegment (S_msrDamp damp)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceDamps || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending damp " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendDampToMeasure (damp);

  gIndenter--;
}

void msrSegment::appendDampAllToSegment (S_msrDampAll dampAll)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceDampAlls || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending damp all " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendDampAllToMeasure (dampAll);

  gIndenter--;
}

void msrSegment::appendTransposeToSegment (
  S_msrTranspose transpose)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending transpose " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendTransposeToMeasure (transpose);

  gIndenter--;
}

void msrSegment::appendPartNameDisplayToSegment (
  S_msrPartNameDisplay partNameDisplay)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending part name display " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendPartNameDisplayToMeasure (partNameDisplay);

  gIndenter--;
}

void msrSegment::appendPartAbbreviationDisplayToSegment (
  S_msrPartAbbreviationDisplay partAbbreviationDisplay)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTranspositions || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending part abbreviation display " <<

      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

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

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceStaffDetails
      ||
    gTraceOptions->fTraceSegments
  ) {
    gLogIOstream <<
      "Appending staff details '" <<
      staffDetails->asShortString () <<
      "' to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\" line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceStaffDetails
      ||
    gTraceOptions->fTraceSegments
  ) {
    fSegmentVoiceUpLink->
      displayVoice (
        inputLineNumber,
        "appendStaffDetailsToSegment() 1");
  }
#endif

  gIndenter++;

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  // append staffDetails to this segment
  fSegmentMeasuresList.back ()->
    appendStaffDetailsToMeasure (staffDetails);

#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceStaffDetails
      ||
    gTraceOptions->fTraceSegments
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
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLineBreaks || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
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
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendLineBreakToMeasure (lineBreak);

  gIndenter--;
}

void msrSegment::appendPageBreakToSegment (S_msrPageBreak pageBreak)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePageBreaks || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending break " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendPageBreakToMeasure (pageBreak);

  gIndenter--;
}

void msrSegment::appendBarNumberCheckToSegment (
  S_msrBarNumberCheck barNumberCheck)
{
#ifdef TRACE_OPTIONS
  if (/* JMI gTraceOptions->fTraceBarNumberChecks || */ gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending bar number check " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendBarNumberCheckToMeasure (barNumberCheck);

  gIndenter--;
}

void msrSegment::appendTempoToSegment (
  S_msrTempo tempo)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTempos || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending tempo " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendTempoToMeasure (tempo);

  gIndenter--;
}

void msrSegment::appendRehearsalToSegment (
  S_msrRehearsal rehearsal)
{
#ifdef TRACE_OPTIONS
  if (/* JMI gTraceOptions->fTraceRehearsals || */ gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending rehearsal " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendRehearsalToMeasure (rehearsal);

  gIndenter--;
}

void msrSegment::appendOctaveShiftToSegment (
  S_msrOctaveShift octaveShift)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOctaveShifts || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
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
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendOctaveShiftToMeasure (octaveShift);

  gIndenter--;
}

void msrSegment::appendScordaturaToSegment (
  S_msrScordatura scordatura)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceScordaturas || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
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
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

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
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSegments) {
    gLogIOstream <<
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
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

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
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSegments) {
    gLogIOstream <<
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
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendHarpPedalsTuningToMeasure (
      harpPedalsTuning);

  gIndenter--;
}

void msrSegment::padUpToActualMeasureWholeNotesInSegment (
  int      inputLineNumber,
  rational wholeNotes)
{
#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceSegments
      ||
    gTraceOptions->fTraceMeasures
      ||
    gMusicXMLOptions->fTraceBackup
  ) {
    gLogIOstream <<
      "Padding up to actual whole notes '" << wholeNotes <<
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

    gLogIOstream <<
      "SegmentVoiceUpLink:" <<
      endl;
    gIndenter++;
    gLogIOstream <<
      fSegmentVoiceUpLink <<
      endl;
    gIndenter--;

    gLogIOstream <<
      "Part:" <<
      endl;
    gIndenter++;
    gLogIOstream <<
      fSegmentVoiceUpLink->fetchVoicePartUpLink () <<
      endl;
    gIndenter--;

    msrInternalError (
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  if (fSegmentMeasuresList.size ()) { // JMI BOFBOF
    // pad last measure up to to this actual wholes notes
    fSegmentMeasuresList.back ()->
      padUpToActualMeasureWholeNotesInMeasure (
        inputLineNumber,
        wholeNotes);
  }
}

void msrSegment::appendPaddingNoteToSegment (
  int inputLineNumber,
  int divisions,
  int divisionsPerQuarterNote)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSegments || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appendding padding tote of " << divisions <<
      " divisions to segment '" <<
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
        divisions,
        divisionsPerQuarterNote);
  }

  gIndenter--;
}

void msrSegment::appendMeasureToSegment (S_msrMeasure measure)
{
  int inputLineNumber =
    measure->getInputLineNumber ();

  string measureNumber =
    measure->getMeasureNumber ();

  int segmentMeasuresListSize =
    fSegmentMeasuresList.size ();

  string currentMeasureNumber =
    segmentMeasuresListSize == 0
      ? ""
      : fSegmentMeasuresList.back ()->getMeasureNumber ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending measure '" << measureNumber <<
      "' to segment " << asString ();

    if (fSegmentMeasuresList.size () == 0)
      gLogIOstream <<
        ", as first measure";
    else
      gLogIOstream <<
      ", after measure number '" << currentMeasureNumber << "'";

    gLogIOstream <<
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
      gGeneralOptions->fInputSourceName,
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
    measure->getMeasureNumber ();

  int segmentMeasuresListSize =
    fSegmentMeasuresList.size ();

  string currentMeasureNumber =
    segmentMeasuresListSize == 0
      ? ""
      : fSegmentMeasuresList.back ()->getMeasureNumber ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Prepending measure " << measureNumber <<
      " to segment " << asString ();

    if (segmentMeasuresListSize == 0) {
      gLogIOstream <<
        ", as first measure FOO"; // JMI
    }

/* JMI
    else
      gLogIOstream <<
      ", after measure number '" << currentMeasureNumber << "'";
*/

    gLogIOstream <<
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
      gGeneralOptions->fInputSourceName,
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
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Prepending barline '" << barline->asString () <<
      "' to segment '" << asString () << "'" <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"," <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  // prepend barline to this segment
  gIndenter++;

  fSegmentMeasuresList.front ()->
    prependBarlineToMeasure (barline);

  gIndenter--;
}

void msrSegment::appendBarlineToSegment (S_msrBarline barline)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending barline '" << barline->asString () <<
      "' to segment '" << asString () << "'" <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"," <<
      endl;
  }
#endif

  if (! fSegmentMeasuresList.size ()) { // JMI
#ifdef TRACE_OPTIONS
    if (
      gTraceOptions->fTraceBarLinesDetails
        ||
      gTraceOptions->fTraceSegmentsDetails
    ) { // JMI
      fSegmentVoiceUpLink->
        displayVoice (
          barline->getInputLineNumber (),
          "appendBarlineToSegment (S_msrBarline)");
    }
#endif
  }

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;

  // append barline to this segment
  fSegmentMeasuresList.back ()->
    appendBarlineToMeasure (barline);

  gIndenter--;
}

void msrSegment::appendBarCheckToSegment (S_msrBarCheck barCheck)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending bar check '" << barCheck->asString () <<
      "' to segment '" << asString () << "'" <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"," <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  fSegmentMeasuresList.back ()->
    appendBarCheckToMeasure (barCheck);
}

void msrSegment::appendVoiceStaffChangeToSegment (
  S_msrVoiceStaffChange voiceStaffChange)
{
#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceVoices
      ||
    gTraceOptions->fTraceStaves
      ||
    gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending voice staff change '" <<
      voiceStaffChange->asString () <<
      "' to segment \"" << asString () << "\"" <<
      ", line " << voiceStaffChange->getInputLineNumber () <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  gIndenter++;

  fSegmentMeasuresList.back ()->
    appendVoiceStaffChangeToMeasure (
      voiceStaffChange);

  gIndenter--;
}

void msrSegment::appendNoteToSegment (S_msrNote note)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceSegments) {
    if (! fSegmentMeasuresList.size ()) { // JMI
      displaySegment (
        note->getInputLineNumber (),
        "appendNoteToSegment()");
    }
  }
#endif

  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  fSegmentMeasuresList.back ()->
    appendNoteToMeasure (note);
}

void msrSegment::appendNoteToSegmentClone (S_msrNote note)
{
  fSegmentMeasuresList.back ()->
    appendNoteToMeasureClone (note);
}

void msrSegment::appendDoubleTremoloToSegment ( // JMI
  S_msrDoubleTremolo doubleTremolo)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTremolos || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
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
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  fSegmentMeasuresList.back ()->
    appendDoubleTremoloToMeasure (doubleTremolo);
}

void msrSegment::appendChordToSegment (S_msrChord chord) // JMI
{
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  fSegmentMeasuresList.back ()->
    appendChordToMeasure (chord);
}

void msrSegment::appendTupletToSegment (S_msrTuplet tuplet) // JMI
{
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  fSegmentMeasuresList.back ()->
    appendTupletToMeasure (tuplet);
}

/* JMI
void msrSegment::addGraceNotesGroupAheadOfSegmentIfNeeded (
  S_msrGraceNotesGroup graceNotesGroup)

{
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  fSegmentMeasuresList.front ()->
    addGraceNotesGroupAheadOfMeasure (graceNotesGroup);
}

void msrSegment::appendGraceNotesToSegment (
  S_msrGraceNotes graceNotes)
{
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  fSegmentMeasuresList.back ()->
    appendGraceNotesToMeasure (graceNotes);
}

void msrSegment::appendAfterGraceNotesToSegment (
  S_msrAfterGraceNotes afterGraceNotes)
{
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  fSegmentMeasuresList.back ()->
    appendAfterGraceNotesToMeasure (afterGraceNotes);
}

void msrSegment::prependAfterGraceNotesToSegment (
  S_msrAfterGraceNotes afterGraceNotes)

{
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  fSegmentMeasuresList.front ()->
    prependAfterGraceNotesToMeasure (afterGraceNotes); // JMI
}
*/

void msrSegment::prependOtherElementToSegment (
  S_msrMeasureElement elem)
{
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

  fSegmentMeasuresList.back ()->
    prependOtherElementToMeasure (elem);
}

void msrSegment::appendOtherElementToSegment (
  S_msrMeasureElement elem)
{
  // sanity check
  msrAssert (
    fSegmentMeasuresList.size () > 0,
    "fSegmentMeasuresList is empty");

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
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
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
      gGeneralOptions->fInputSourceName,
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
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSegments) {
    gLogIOstream <<
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
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

S_msrMeasure msrSegment::fetchLastMeasureFromSegment (
  int    inputLineNumber,
  string context)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
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
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  S_msrMeasure
    result =
      fSegmentMeasuresList.back ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      endl <<
      "The fetched measure contains:" <<
      endl;

    gIndenter++;

    gLogIOstream <<
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
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      "Removing last measure from segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      " (" << context << ")" <<
      "', line " << inputLineNumber <<
      endl;

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
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  S_msrMeasure
    result =
      fSegmentMeasuresList.back ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
    gLogIOstream <<
      endl <<
      "The removed measure contains:" <<
      endl;

    gIndenter++;

    gLogIOstream <<
      result <<
      endl;

    gIndenter--;
  }
#endif

  // JMI for tests abort ();

  fSegmentMeasuresList.pop_back ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceSegments) {
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
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSegment::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrSegment>*
    p =
      dynamic_cast<visitor<S_msrSegment>*> (v)) {
        S_msrSegment elem = this;

        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSegment::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSegment::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSegment::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrSegment>*
    p =
      dynamic_cast<visitor<S_msrSegment>*> (v)) {
        S_msrSegment elem = this;

        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSegment::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrSegment::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSegment::browseData ()" <<
      endl;
  }

/* JMI
  gLogIOstream <<
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

  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% <== msrSegment::browseData ()" <<
      endl;
  }
}

string msrSegment::asShortString () const
{
  stringstream s;

  s <<
   "Segment '" <<
    fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
    "', voice: \"" <<
    fSegmentVoiceUpLink->getVoiceName () <<
    "\"";

  return s.str ();
}

string msrSegment::asString () const
{
  stringstream s;

  s <<
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
      s << (*i)->getMeasureNumber ();
      if (++i == iEnd) break;
      s << ", ";
    } // for

    s << "]";
  }

  return s.str ();
}

void msrSegment::displaySegment (
  int    inputLineNumber,
  string context)
{
  gLogIOstream <<
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
  print (gLogIOstream);
  gIndenter--;

  gLogIOstream <<
    "<<*********" <<
    endl <<
    endl;
}

void msrSegment::print (ostream& os)
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

void msrSegment::shortPrint (ostream& os)
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


/* JMI
    // is last measure an empty measure that was created for a repeat?
    msrMeasure::msrMeasureCreatedForARepeatKind
      lastMeasureCreatedForARepeatKind =
        lastMeasure->
          getMeasureCreatedForARepeatKind ();

    switch (lastMeasureCreatedForARepeatKind) {
      case msrMeasure::kMeasureCreatedForARepeatNo:
        // is the last measure empty?
 //       if (lastMeasure->getActualMeasureWholeNotes ().getNumerator () == 0) { // JMI ???
   //     if (false && lastMeasure->getMeasureElementsList ().size () == 0) { // JMI ALWAYS FINALIZE ???
        if (
          false && // JMI
          lastMeasure->getMeasureElementsList ().size () == 0
        ) { // JMI ALWAYS FINALIZE ???
          // yes, remove it
#ifdef TRACE_OPTIONS
          if (
            gTraceOptions->fTraceMeasures
              ||
            gTraceOptions->fTraceSegments
              ||
            gTraceOptions->fTraceRepeats) {
            stringstream s;

            gLogIOstream <<
              "Removing empty measure '" <<
              lastMeasure->getMeasureNumber () <<
              "' in segment '" <<
              asString () <<
              "'";
          }
#endif

          fSegmentMeasuresList.pop_back ();
        }

        else {
          // no, finalize it
          lastMeasure->
            finalizeMeasure (
              inputLineNumber,
              "finalizeCurrentMeasureInSegment() kMeasureCreatedForARepeatNo");
        }
        break;

      case msrMeasure::kMeasureCreatedForARepeatBefore:
        if (
          false && // JMI
          lastMeasure->getActualMeasureWholeNotes ().getNumerator () == 0
        ) {
          // yes, remove it
#ifdef TRACE_OPTIONS
          if (
            gTraceOptions->fTraceMeasures
              ||
            gTraceOptions->fTraceSegments
              ||
            gTraceOptions->fTraceRepeats) {
            stringstream s;

            gLogIOstream <<
              "Removing empty measure '" <<
              lastMeasure->getMeasureNumber () <<
              "' that was created for a repeat (" <<
                msrMeasure::measureCreatedForARepeatKindAsString (
                  lastMeasureCreatedForARepeatKind) <<
              ") in segment '" <<
              asString () <<
              "'";
          }
#endif

          fSegmentMeasuresList.pop_back ();
        }
        else {
          // no, finalize it
          lastMeasure->
            finalizeMeasure (
              inputLineNumber,
              "finalizeCurrentMeasureInSegment() kMeasureCreatedForARepeatBefore");
        }
        break;

      case msrMeasure::kMeasureCreatedForARepeatAfter:
        // finalize it
        lastMeasure->
          finalizeMeasure (
            inputLineNumber,
              "finalizeCurrentMeasureInSegment() kMeasureCreatedForARepeatAfter");
        break;

      case msrMeasure::kMeasureCreatedForARepeatPadded:
          // finalize it JMI ???
          lastMeasure->
            finalizeMeasure (
              inputLineNumber,
              "finalizeCurrentMeasureInSegment() kMeasureCreatedForARepeatPadded");
        break;
    } // switch
    */
