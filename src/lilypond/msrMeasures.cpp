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
  S_msrSegment measureSegmentUplink)
{
  msrMeasure* o =
    new msrMeasure (
      inputLineNumber,
      measureNumber,
      measureSegmentUplink);
  assert(o!=0);

  return o;
}

msrMeasure::msrMeasure (
  int          inputLineNumber,
  string       measureNumber,
  S_msrSegment measureSegmentUplink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert(
    measureSegmentUplink != nullptr,
    "measureSegmentUplink is null");

  // set measure's segment uplink
  fMeasureSegmentUplink =
    measureSegmentUplink;

  // set measure numbers
  fMeasureNumber = measureNumber;
  fMeasurePuristNumber = -1; // default value
  fNextMeasureNumber = "";

  // set debug number
  fMeasureDebugNumber = ++gMeasureDebugNumber;
  
  // do other initializations
  initializeMeasure ();
}

void msrMeasure::initializeMeasure ()
{
  S_msrVoice
    voiceUplink =
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ();
      
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Initializing measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in segment '" <<
      fMeasureSegmentUplink->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      voiceUplink->getVoiceName () <<
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
  
        fMeasureSegmentUplink->
          getSegmentVoiceUplink () <<
        
        "=======================" <<
        endl <<
        endl;
  
      abort ();
    }
  }
#endif

  // measure kind
  setMeasureKind (kMeasureUnknown);

  // measure 'first in segment' kind
  fMeasureFirstInSegmentKind = kMeasureFirstInSegmentUnknown;

  // measure 'relative to a repeat' kind
  setMeasureRepeatKind (kMeasureRepeatUnknown);
  
  // measure 'first in voice'
  fMeasureFirstInVoice = false; // default value

  // single-measure rest?
  fMeasureIsASingleMeasureRest = false;
  
  // initialize measure whole notes
  setActualMeasureWholeNotes (
    fInputLineNumber,
    rational (0, 1)); // ready to receive the first note

  // fetch the staff
  S_msrStaff
    staffUplink =
      voiceUplink->
        getVoiceStaffUplink ();

  // get the staff time
  S_msrTime
    staffCurrentTime =
      staffUplink->
        getStaffCurrentTime ();
        
  // set the measure full length if relevant
  if (staffCurrentTime) {
    setFullMeasureWholeNotesFromTime (
      staffCurrentTime);
  }

  // measure doesn't contain music yet
  fMeasureContainsMusic = false;

  // regular measure ends detection
  fMeasureEndRegularKind = kMeasureEndRegularUnknown;

  // measure finalization
  fMeasureHasBeenFinalized = false;
}

msrMeasure::~msrMeasure ()
{}

S_msrPart msrMeasure::fetchMeasurePartUplink () const
{
  return
    fMeasureSegmentUplink->
      fetchSegmentPartUplink ();
}

S_msrVoice msrMeasure::fetchMeasureVoiceUplink () const
{
  return
    fMeasureSegmentUplink->
      getSegmentVoiceUplink ();
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
          getSegmentVoiceUplink ()->
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
  // fActualMeasureWholeNotes and fFullMeasureWholeNotes
  // will be computed on the fly
  // when appending notes to the measure newborn clone

  // measure kind
  newbornClone->fMeasureKind =
    fMeasureKind;
*/
  
  // chords handling

  // elements

  // uplinks

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
          getSegmentVoiceUplink ()->
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
    
  measureDeepCopy->fActualMeasureWholeNotes =
    fActualMeasureWholeNotes;
    
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
            getSegmentVoiceUplink ()->
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
            fetchMeasureVoiceUplink ());

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
          getSegmentVoiceUplink ()->
            getVoiceName () <<
        "\"" <<
        endl;
    }
#endif
  }

  // uplinks

  // fMeasureSegmentUplink JMI ???
  
  return measureDeepCopy;
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
        fMeasureSegmentUplink->asString () <<
        " in voice \"" <<
        fMeasureSegmentUplink->
          getSegmentVoiceUplink ()->
            getVoiceName () <<
        "\"" <<
        endl;
    }
#endif

  fMeasurePuristNumber = measurePuristNumber;
}

void msrMeasure::setMeasureRepeatKind (
  msrMeasureRepeatKind
    measureRepeatKind)
{
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
        "Setting relative to a repeat kind of measure '" <<
        fMeasureNumber <<
        "' to '" <<
        measureRepeatKindAsString (
          measureRepeatKind) <<
        "' in segment " <<
        fMeasureSegmentUplink->asString () <<
        " in voice \"" <<
        fMeasureSegmentUplink->
          getSegmentVoiceUplink ()->
            getVoiceName () <<
        "\"" <<
        endl;
    }
#endif

  fMeasureRepeatKind =
    measureRepeatKind;
}

void msrMeasure::appendElementToMeasure (S_msrMeasureElement elem)
{
#ifdef TRACE_OPTIONS
  int inputLineNumber =
    elem->getInputLineNumber ();
#endif
    
  if (fMeasurePendingMeasureElementsList.size ()) {
    // pad measure up to the elements positions in measure,
    // and then append them

#ifdef TRACE_OPTIONS
    // fetch the part measure whole notes high tide
    rational
      partActualMeasureWholeNotesHighTide =
        fetchMeasurePartUplink ()->
          getPartActualMeasureWholeNotesHighTide ();
#endif
    
    list<S_msrMeasureElement>::iterator
      iBegin = fMeasurePendingMeasureElementsList.begin (),
      iEnd   = fMeasurePendingMeasureElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrMeasureElement
        measureElement = (*i);
        
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceBarlines || gTraceOptions->fTraceMeasures) {
        gLogIOstream <<
          "Considering delayed '" <<
          measureElement->asShortString () <<
          "' in measure '" <<
          asShortString () <<
          "' in voice \"" <<
          fMeasureSegmentUplink->
            getSegmentVoiceUplink ()
              ->getVoiceName () <<
          "\", line " << inputLineNumber <<
          ", has position in measure '" <<
          measureElement->getPositionInMeasure () <<
          ", ActualMeasureWholeNotes = " << fActualMeasureWholeNotes <<
          ", partActualMeasureWholeNotesHighTide = " << partActualMeasureWholeNotesHighTide <<
          endl;
      }
#endif

      if (measureElement->getPositionInMeasure () == fActualMeasureWholeNotes) { // JMI
        // this is where measureElement should be appended
        
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceBarlines || gTraceOptions->fTraceMeasures) {
          gLogIOstream <<
            "Appending delayed '" <<
            measureElement->asShortString () <<
            "' in measure '" <<
            asShortString () <<
            "' in voice \"" <<
            fMeasureSegmentUplink->
              getSegmentVoiceUplink ()
                ->getVoiceName () <<
            "\", line " << inputLineNumber <<
            ", has position in measure '" <<
            measureElement->getPositionInMeasure () <<
            ", actualMeasureWholeNotes = " << fActualMeasureWholeNotes <<
            ", partActualMeasureWholeNotesHighTide = " << partActualMeasureWholeNotesHighTide <<
            endl;
        }
#endif

        fMeasureElementsList.push_back (measureElement);

        // remove it from the pending measure elements list
        i = fMeasurePendingMeasureElementsList.erase (i);
      }

      if (++i == iEnd) break;
    } // for    
  }

  fMeasureElementsList.push_back (elem);
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
      fMeasureSegmentUplink-> asString () <<
      /* JMI
      "' in voice \"" <<
      fSegmentVoiceUplink->getVoiceName () <<
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
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      "' in segment '" <<
      fMeasureSegmentUplink-> asString () <<
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

void msrMeasure::setActualMeasureWholeNotes (
  int      inputLineNumber,
  rational wholeNotes)
{
  // rationalise the measure whole notes
  rational
    rationalisedActualMeasureWholeNotes =
      wholeNotes;
  rationalisedActualMeasureWholeNotes.rationalise ();
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gMusicXMLOptions->fTraceDivisions) {
    gLogIOstream <<
      "Setting measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' measure actual whole notes to '"  <<
      rationalisedActualMeasureWholeNotes <<
      "'";

    if (
      rationalisedActualMeasureWholeNotes.getDenominator ()
        !=
      wholeNotes.getDenominator ()
    ) {
      gLogIOstream <<
        " (was '" << wholeNotes << "')";
    }

    gLogIOstream <<
      " in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  // set measure whole notes
  fActualMeasureWholeNotes = rationalisedActualMeasureWholeNotes;
}

string msrMeasure::actualMeasureWholeNotesAsMSRString ()
{
  return
    wholeNotesAsMsrString (
      fInputLineNumber,
      fActualMeasureWholeNotes);
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
        fMeasureSegmentUplink->asString () <<
        " in voice \"" <<
        fMeasureSegmentUplink->
          getSegmentVoiceUplink ()->
            getVoiceName () <<
        "\"" <<
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
      fetchMeasureVoiceUplink ()->getVoiceName () <<
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
      fetchMeasureVoiceUplink ()->getVoiceName () <<
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
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
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
      voiceUplink =
        fMeasureSegmentUplink->
          getSegmentVoiceUplink ();
        
    gLogIOstream <<
      "to measure clone '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in segment clone '" <<
      fMeasureSegmentUplink->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      voiceUplink->getVoiceName () <<
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
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

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
            fMeasureSegmentUplink->
              getSegmentVoiceUplink ()->
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
            fMeasureSegmentUplink->
              getSegmentVoiceUplink ()->
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
          fMeasureSegmentUplink->
            getSegmentVoiceUplink ()->
              getVoiceName () <<
          "\"" <<
          " is senza misura" <<
          endl;
      }
#endif
  
      setMeasureKind (kMeasureCadenza);
      
      fFullMeasureWholeNotes =
        rational (INT_MAX, 1);
      break;
  } // switch
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
    setPositionInMeasure (
      fActualMeasureWholeNotes);

  // is there already a pending barline in this voice?
  if (fMeasurePendingMeasureElementsList.size () > 1) {
    stringstream s;
  
    s <<
      "should not have more than one pending measure elements in measure " << // JMI
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
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
        if (
          gTraceOptions->fTraceRepeatsDetails
            ||
          gTraceOptions->fTraceVoicesDetails
        ) {
          printMeasurePendingMeasureElementsList ();
        }
#endif

  // fetch the part measure whole notes high tide
  rational
    partActualMeasureWholeNotesHighTide =
      fetchMeasurePartUplink ()->
        getPartActualMeasureWholeNotesHighTide ();

  if (true /* JMI */ ||
    fActualMeasureWholeNotes == partActualMeasureWholeNotesHighTide
  ) {
    // append barline to the measure elements list at once
    appendElementToMeasure (barline);
  }
  else {
    // delay barline handling until this measure reaches
    // the part measure whole notes high tide

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceBarlines || gTraceOptions->fTraceMeasures) {
      gLogIOstream <<
        "Delaying insertion of barline '" <<
        barline->asShortString () <<
        "' in measure '" <<
        asShortString () <<
        "' in voice \"" <<
        fMeasureSegmentUplink->
          getSegmentVoiceUplink ()
            ->getVoiceName () <<
        "\", line " << inputLineNumber <<
        ", has position in measure '" <<
        barline->getPositionInMeasure () <<
        ", actualMeasureWholeNotes = " << fActualMeasureWholeNotes <<
        ", partActualMeasureWholeNotesHighTide = " << partActualMeasureWholeNotesHighTide <<
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
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;
  
  // populate measure uplink
  note->
    setNoteMeasureUplink (this);

  // register note position in measure
  rational
    notePositionInMeasure =
      fActualMeasureWholeNotes; // for harmony voice
  
  note->
    setNotePositionInMeasure (
      notePositionInMeasure);
  
  // fetch note sounding whole notes
  rational noteSoundingWholeNotes =
    note->getNoteSoundingWholeNotes ();
    
  string noteSoundingWholeNotesAsMsrString =
    note->noteSoundingWholeNotesAsMsrString ();
    
  // account for note duration in measure whole notes
  setActualMeasureWholeNotes (
    inputLineNumber,
    fActualMeasureWholeNotes + noteSoundingWholeNotes);

  // update part measure whole notes high tide if need be
  fetchMeasurePartUplink ()->
    updatePartActualMeasureWholeNotesHighTide (
      inputLineNumber,
      fActualMeasureWholeNotes);

/* JMI
  // determine whether the note occupies a full measure
  if (noteSoundingWholeNotes == fFullMeasureWholeNotes)
    note->
      setNoteOccupiesAFullMeasure ();
  */
  
  // append the note to the measure elements list
// JMI  // only now to make it possible to remove it afterwards
  // if it happens to be the first note of a chord
  appendElementToMeasure (note);
  
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
      fMeasureSegmentUplink->getSegmentAbsoluteNumber () <<
      "' in voice clone \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
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
    fMeasureSegmentUplink->
      appendNoteToSegment (note);
  }

  else {
  */
    // regular insertion in current measure
    
    // populate measure uplink
    note->setNoteMeasureUplink (
      this);

    // register note measure position
    rational
      notePositionInMeasure =
        fActualMeasureWholeNotes; // for harmony voice
    
    note->
      setNotePositionInMeasure (
        notePositionInMeasure);
    
    // fetch note sounding whole notes
    rational
      noteSoundingWholeNotes =
        note->getNoteSoundingWholeNotes ();

    // account for note duration in measure whole notes
    setActualMeasureWholeNotes (
      inputLineNumber,
      fActualMeasureWholeNotes + noteSoundingWholeNotes);
  
    // update part measure whole notes high tide if need be
    fetchMeasurePartUplink ()->
      updatePartActualMeasureWholeNotesHighTide (
        inputLineNumber,
        fActualMeasureWholeNotes);

  /* JMI
    // determine whether the note occupies a full measure
    if (noteSoundingWholeNotes == fFullMeasureWholeNotes)
      note->
        setNoteOccupiesAFullMeasure ();
      */
      
    // append the note to the measure elements list
  // JMI  // only now to make it possible to remove it afterwards
    // if it happens to be the first note of a chord
    appendElementToMeasure (note);

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
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
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
      fActualMeasureWholeNotes);

  // copy measure number to first note, that was created beforehand
  doubleTremolo->
    setDoubleTremoloMeasureNumber (
      fMeasureNumber);
  
  // copy measure position to first note, that was created beforehand
  doubleTremolo->
    setDoubleTremoloPositionInMeasure (
      fActualMeasureWholeNotes);

  // fetch doubleTremolo sounding whole notes
  rational
    doubleTremoloSoundingWholeNotes =
      doubleTremolo->
        getDoubleTremoloSoundingWholeNotes ();
    
  // account for doubleTremolo duration in measure whole notes
  setActualMeasureWholeNotes (
    inputLineNumber,
    fActualMeasureWholeNotes + doubleTremoloSoundingWholeNotes);

  // update part measure whole notes high tide if need be
  fetchMeasurePartUplink ()->
    updatePartActualMeasureWholeNotesHighTide (
      inputLineNumber,
      fActualMeasureWholeNotes);

  // determine if the doubleTremolo occupies a full measure
// XXL  JMI  if (doubleTremoloSoundingWholeNotes == fMeasureDivisionsPerWholeMeasure)
    // doubleTremolo->setDoubleTremoloOccupiesAFullMeasure ();

  // append the doubleTremolo to the measure elements list
  appendElementToMeasure (doubleTremolo);

  // this measure contains music
  fMeasureContainsMusic = true;

  gIndenter--;
}

void msrMeasure::appendChordToMeasure (S_msrChord chord) // JMI XXL
{
  int inputLineNumber =
    chord->getInputLineNumber ();
    
  // populate measure uplink
  chord->setChordMeasureUplink (this);

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending chord '" << chord->asString () <<
      "' to measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // register chord measure number
  chord->
    setChordMeasureNumber (fMeasureNumber);
  
  // register chord measure position in measure
  chord->
    setChordPositionInMeasure (
      fActualMeasureWholeNotes);

  // copy measure position in measure to first note, that was created beforehand
  chord->
    setChordFirstNotePositionInMeasure (
      fActualMeasureWholeNotes);

  // fetch chord sounding whole notes
  rational
    chordSoundingWholeNotes =
      chord->
        getChordSoundingWholeNotes ();
    
  // account for chord duration in measure whole notes
  setActualMeasureWholeNotes (
    inputLineNumber,
    fActualMeasureWholeNotes + chordSoundingWholeNotes);

  // update part measure whole notes high tide if need be
  fetchMeasurePartUplink ()->
    updatePartActualMeasureWholeNotesHighTide (
      inputLineNumber,
      fActualMeasureWholeNotes);

  // determine if the chord occupies a full measure
// XXL  JMI  if (chordSoundingWholeNotes == fMeasureDivisionsPerWholeMeasure)
    // chord->setChordOccupiesAFullMeasure ();

  // append the chord to the measure elements list
  appendElementToMeasure (chord);

  // this measure contains music
  fMeasureContainsMusic = true;
}

void msrMeasure::appendTupletToMeasure (S_msrTuplet tuplet)
{
  int inputLineNumber =
    tuplet->getInputLineNumber ();
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending tuplet '" << tuplet->asString () <<
      "' to measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;

  // populate measure uplink
  tuplet->setTupletMeasureUplink (this);
  
  // fetch tuplet sousnding whole notes
  rational
    tupletSoundingWholeNotes =
      tuplet->
        getTupletSoundingWholeNotes ();
    
  // account for tuplet duration in measure whole notes
  setActualMeasureWholeNotes (
    inputLineNumber,
    fActualMeasureWholeNotes + tupletSoundingWholeNotes);

  // update part measure whole notes high tide if need be
  fetchMeasurePartUplink ()->
    updatePartActualMeasureWholeNotesHighTide (
      inputLineNumber,
      fActualMeasureWholeNotes);

  // append the tuplet to the measure elements list
  appendElementToMeasure (tuplet);

  // this measure contains music
  fMeasureContainsMusic = true;

  gIndenter--;
}

void msrMeasure::appendHarmonyToMeasure (S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending harmony '" << harmony->asString () <<
      "' to measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      ", actualMeasureWholeNotes = " << fActualMeasureWholeNotes <<
      endl;
  }
#endif

  // fetch harmony sounding whole notes
  rational
    harmonySoundingWholeNotes =
      harmony->
        getHarmonySoundingWholeNotes ();
    
  // account for harmony duration in measure whole notes
  setActualMeasureWholeNotes (
    inputLineNumber,
    fActualMeasureWholeNotes + harmonySoundingWholeNotes);

  // update part measure whole notes high tide if need be
  fetchMeasurePartUplink ()->
    updatePartActualMeasureWholeNotesHighTide (
      inputLineNumber,
      fActualMeasureWholeNotes);
  
  // append the harmony to the measure elements list
  appendElementToMeasure (harmony);

  // this measure contains music
  fMeasureContainsMusic = true;
}

void msrMeasure::appendHarmonyToMeasureClone (S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();
    
  // regular insertion in current measure
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending harmony '" << harmony->asString () <<
      "' to measure clone '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in segment clone '" <<
      fMeasureSegmentUplink->getSegmentAbsoluteNumber () <<
      "' in voice clone \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      ", actualMeasureWholeNotes = " << fActualMeasureWholeNotes <<
      endl;
  }
#endif
      
  // fetch harmony sounding whole notes
  rational
    harmonySoundingWholeNotes =
      harmony->
        getHarmonySoundingWholeNotes ();
    
  // account for harmony duration in measure whole notes
  setActualMeasureWholeNotes (
    inputLineNumber,
    fActualMeasureWholeNotes + harmonySoundingWholeNotes);

  // update part measure whole notes high tide if need be
  fetchMeasurePartUplink ()->
    updatePartActualMeasureWholeNotesHighTide (
      inputLineNumber,
      fActualMeasureWholeNotes);
  
  // append the harmony to the measure elements list
  appendElementToMeasure (harmony);

  // this measure contains music
  fMeasureContainsMusic = true;
}

void msrMeasure::appendFiguredBassToMeasure (
  S_msrFiguredBass figuredBass)
{
  int inputLineNumber =
    figuredBass->getInputLineNumber ();
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending figured bass '" << figuredBass->asString () <<
      "' to measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      ", actualMeasureWholeNotes = " << fActualMeasureWholeNotes <<
      endl;
  }
#endif

  // fetch harmony sounding whole notes
  rational
    figuredBassSoundingWholeNotes =
      figuredBass->
        getFiguredBassSoundingWholeNotes ();
    
  // account for harmony duration in measure whole notes
  setActualMeasureWholeNotes (
    inputLineNumber,
    fActualMeasureWholeNotes + figuredBassSoundingWholeNotes);

  // update part measure whole notes high tide if need be
  fetchMeasurePartUplink ()->
    updatePartActualMeasureWholeNotesHighTide (
      inputLineNumber,
      fActualMeasureWholeNotes);
  
  // append the harmony to the measure elements list
  appendElementToMeasure (figuredBass);

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
  if (gTraceOptions->fTraceHarmonies || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending figured bass '" << figuredBass->asString () <<
      "' to measure clone '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in segment clone '" <<
      fMeasureSegmentUplink->getSegmentAbsoluteNumber () <<
      "' in voice clone \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      ", actualMeasureWholeNotes = " << fActualMeasureWholeNotes <<
      endl;
  }
#endif
      
  // fetch harmony sounding whole notes
  rational
    figuredBassSoundingWholeNotes =
      figuredBass->
        getFiguredBassSoundingWholeNotes ();
    
  // account for harmony duration in measure whole notes
  setActualMeasureWholeNotes (
    inputLineNumber,
    fActualMeasureWholeNotes + figuredBassSoundingWholeNotes);

  // update part measure whole notes high tide if need be
  fetchMeasurePartUplink ()->
    updatePartActualMeasureWholeNotesHighTide (
      inputLineNumber,
      fActualMeasureWholeNotes);
  
  // append the harmony to the measure elements list
  appendElementToMeasure (figuredBass);

  // this measure contains music
  fMeasureContainsMusic = true;
}

S_msrNote msrMeasure::createPaddingNoteForVoice (
  int        inputLineNumber,
  rational   duration,
  S_msrVoice voice)
{
#ifdef TRACE_OPTIONS
  if (
    gTraceOptions->fTraceNotes
      ||
    gTraceOptions->fTraceMeasures
      ||
    gTraceOptions->fTraceRepeats
      ||
    gTraceOptions->fTraceVoices
  ) {
    gLogIOstream <<
      "Creating a padding note for voice \"" <<
      voice->getVoiceName () <<
      /*
      "\" in measure '"
      asShortString () <<
      */
      "\" in measure '";

    print (gLogIOstream);
    
    gLogIOstream <<

      "', duration = '" <<
      duration <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif
  
  // create a rest or a skip depending on measureVoice kind
  S_msrNote paddingNote;

  switch (voice->getVoiceKind ()) {
    case msrVoice::kRegularVoice:
      paddingNote =
      /* JMI ???
        msrNote::createRestNote (
          inputLineNumber,
          fMeasureNumber,
          duration,
          duration,
          0, // dots number JMI ???
          voice->
            getVoiceStaffUplink ()->getStaffNumber (),
          voice->
            getVoiceNumber ());
            */
        msrNote::createSkipNote (
          inputLineNumber,
          fMeasureNumber,
          duration,
          duration,
          0, // dots number JMI ???
          voice->
            getVoiceStaffUplink ()->getStaffNumber (),
          voice->
            getVoiceNumber ());
      break;
      
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      paddingNote =
        msrNote::createSkipNote (
          inputLineNumber,
          fMeasureNumber,
          duration,
          duration,
          0, // dots number JMI ???
          voice->
            getVoiceStaffUplink ()->getStaffNumber (),
          voice->
            getVoiceNumber ());
      break;
  } // switch

  return paddingNote;
}

void msrMeasure::padUpToActualMeasureWholeNotesInMeasure (
  int      inputLineNumber,
  rational wholeNotes)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Padding from actual measure whole notes '" << fActualMeasureWholeNotes <<
      "' to '" << wholeNotes <<
      "' in measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in segment " <<
      fMeasureSegmentUplink->getSegmentAbsoluteNumber () << 
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  if (fActualMeasureWholeNotes < wholeNotes) {
    // appending a padding rest or skip to this measure to reach wholeNotes
    rational
      missingDuration =
        wholeNotes - fActualMeasureWholeNotes;
    
    // fetch the measure voice
    S_msrVoice
      measureVoice =
        fMeasureSegmentUplink->
          getSegmentVoiceUplink ();
      
    // create a rest or a skip depending on measureVoice kind
    S_msrNote
      paddingNote =
        createPaddingNoteForVoice (
          inputLineNumber,
          missingDuration,
          measureVoice);
  
    // register rest's measure whole notes
    paddingNote->
      setNotePositionInMeasure (
        fActualMeasureWholeNotes);
           
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceMeasures || gMusicXMLOptions->fTraceDivisions) {
      gLogIOstream <<
        "Appending rest" << paddingNote->asString () <<
        " (missingDuration " << missingDuration <<
        " whole notes) to skip from length '" << fActualMeasureWholeNotes <<
        " to length '" << wholeNotes << "'"
        " in measure '" <<
        fMeasureNumber <<
        ", measureDebugNumber: '" <<
        fMeasureDebugNumber <<
        "in voice \"" << measureVoice->getVoiceName () <<
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
        msrMeasure::kMeasureCreatedForARepeatPadded);
    */
    
    // this measure contains music
    fMeasureContainsMusic = true;
  }

  gIndenter--;
}

void msrMeasure::appendPaddingNoteToMeasure (
  int inputLineNumber,
  int divisions,
  int divisionsPerQuarterNote)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending padding note of " << divisions <<
      " divisions to measure " <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in segment " <<
      fMeasureSegmentUplink->getSegmentAbsoluteNumber () << 
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // fetch the measure voice
  S_msrVoice
    measureVoice =
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ();
    
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

  // register rest's measure whole notes
  paddingNote->
    setNotePositionInMeasure (
      fActualMeasureWholeNotes);

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
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
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
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
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
      setActualMeasureWholeNotes (
        inputLineNumber,
        fActualMeasureWholeNotes
          -
        fMeasureLastHandledNote->getNoteSoundingWholeNotes ());

      // return from function
      return;
    }
  } // for

  S_msrVoice
    segmentVoiceUplink =
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ();

  S_msrPart
    segmentVoicePart =
      segmentVoiceUplink->
        getVoiceStaffUplink ()->
        getStaffPartUplink ();
      
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
    "@@@@@@@@@@@@@@@@@ segmentVoiceUplink" <<
    endl <<
    segmentVoiceUplink <<
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
    segmentVoiceUplink->getVoiceName () <<
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
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
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
      setActualMeasureWholeNotes (
        inputLineNumber,
        fActualMeasureWholeNotes
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
    fMeasureSegmentUplink->
      getSegmentVoiceUplink ()->
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
  msrMeasure::msrMeasureRepeatKind
          measureRepeatKind)
{
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ();
    
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
      fActualMeasureWholeNotes;
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

  if (fActualMeasureWholeNotes.getNumerator () == 0) {
    // empty measure
    
    stringstream s;

    voice->
      displayVoiceRepeatsStackRestMeasuresMeasuresRepeatAndVoice (
        inputLineNumber,
        "determineMeasureKindAndPuristNumber() measure has 0 fActualMeasureWholeNotes");
      
    s <<
      "measure '" <<
      fMeasureNumber <<
      "' has 0 fActualMeasureWholeNotes" <<
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

    // set it's measure kind
    setMeasureKind (kMeasureMusicallyEmpty);

    // set it's 'relative to a repeat' kind
    setMeasureRepeatKind (
      kMeasureRepeatNone);
  }
  
  else if (fActualMeasureWholeNotes == fFullMeasureWholeNotes) {
    // regular measure

    // this is a regular measure end
    fMeasureEndRegularKind = kMeasureEndRegularYes;

    // reset voice whole notes since last regular measure end
    voice->
      setWholeNotesSinceLastRegularMeasureEnd (
        inputLineNumber,
        0);

    // set it's measure kind
    setMeasureKind (kMeasureRegular);

    // set it's 'relative to a repeat' kind
    setMeasureRepeatKind (
      kMeasureRepeatNone);

    // increment voice current measure purist number
    voice->
      incrementVoiceCurrentMeasurePuristNumber (
        inputLineNumber,
        "determineMeasureKindAndPuristNumber() kMeasureRegular");
  }
  
  else {
    // this is no regular measure end
    fMeasureEndRegularKind = kMeasureEndRegularNo;

    // increment voice whole notes since last regular measure end
    voice->
      setWholeNotesSinceLastRegularMeasureEnd (
        inputLineNumber,
        newWholeNotesSinceLastRegularMeasureEnd);

    if (fActualMeasureWholeNotes < fFullMeasureWholeNotes) {
      //  incomplete measure
      
      if (fMeasureFirstInVoice) {
        // this is an anacrusis
  
        // sanity check
        msrAssert (
          voice->getVoiceCurrentMeasurePuristNumber () == 0,
          "voiceCurrentMeasurePuristNumber is not 0 upon anacrusis");
        
        // set it's measure kind
        setMeasureKind (kMeasureAnacrusis);

        // set it's 'relative to a repeat' kind
        setMeasureRepeatKind (
          kMeasureRepeatNone);
      }
  
      else {
        // this is an incomplete measure within the voice
  
        // set measure's 'relative to a repeat' kind
        // according to the current voice after repeat component phase kind
        switch (currentVoiceRepeatPhaseKind) {
          case msrVoice::kVoiceRepeatPhaseNone:
            // leave it unchanged
            {
              stringstream s;
          
              displayMeasure (
                inputLineNumber,
                "determineMeasureKindAndPuristNumber() kVoiceRepeatPhaseNone");
                
              s <<
                "measure '" <<
                fMeasureNumber <<
                "' keeps it's measureRepeatKind '" <<
                msrMeasure::measureRepeatKindAsString (
                  fMeasureRepeatKind) <<
                "', line " << inputLineNumber;
                
        // JMI      msrInternalError (
              msrInternalWarning (
                gGeneralOptions->fInputSourceName,
                inputLineNumber,
         //       __FILE__, __LINE__,
                s.str ());
            }
          /* JMI
            setMeasureRepeatKind (
              kMeasureRepeatNone);
              */
            break;
            
          case msrVoice::kVoiceRepeatPhaseAfterCommonPart:
            setMeasureRepeatKind (
              kMeasureRepeatNextMeasureAfterCommonPart);
            break;
      
          case msrVoice::kVoiceRepeatPhaseAfterHookedEnding:
            setMeasureRepeatKind (
              msrMeasure::kMeasureRepeatNextMeasureAfterHookedEnding);
            break;
      
          case msrVoice::kVoiceRepeatPhaseAfterHooklessEnding:
            setMeasureRepeatKind (
              msrMeasure::kMeasureRepeatNextMeasureAfterHooklessEnding);
            break;
        } // switch

        // set measure's kind if it is the next measure after a repeat component
        // (it is already set otherwise) // JMI ???
        switch (fMeasureRepeatKind) {
          case msrMeasure::kMeasureRepeatUnknown: // JMI ???
            {
              stringstream s;
          
              displayMeasure (
                inputLineNumber,
                "determineMeasureKindAndPuristNumber() kMeasureRepeatUnknown");
                
              s <<
                "measure '" <<
                fMeasureNumber <<
                "' is kMeasureRepeatUnknown " <<
                asShortString () <<
                ", line " << inputLineNumber;
                
        // JMI      msrInternalError (
              msrInternalWarning (
                gGeneralOptions->fInputSourceName,
                inputLineNumber,
         //       __FILE__, __LINE__,
                s.str ());
            }
            break;
            
          case msrMeasure::kMeasureRepeatNone:
            // set it's measure kind
            setMeasureKind (kMeasureIncompleteStandalone);

            // update the voice current measure purist number if relevant
            switch (fMeasureEndRegularKind) {
              case msrMeasure::kMeasureEndRegularUnknown:
                // JMI ???
                break;
                
              case msrMeasure::kMeasureEndRegularYes:
                // don't increment the voice current measure purist number,
                // this has already been done for the 'first part' of the measure
                break;
                
              case msrMeasure::kMeasureEndRegularNo:
                // increment voice current measure purist number
                voice->
                  incrementVoiceCurrentMeasurePuristNumber (
                    inputLineNumber,
                    "determineMeasureKindAndPuristNumber() kMeasureEndRegularNo");
                break;
            } // switch
            break;
            
          case msrMeasure::kMeasureRepeatCommonPartLastMeasure:
            gLogIOstream <<
              "BLARK" <<
              endl;
            setMeasureKind (
              kMeasureIncompleteLastInRepeatCommonPart);
            break;
          case msrMeasure::kMeasureRepeatHookedEndingLastMeasure:
            setMeasureKind (
              kMeasureIncompleteLastInRepeatHookedEnding);
            break;
          case msrMeasure::kMeasureRepeatHooklessEndingLastMeasure:
            setMeasureKind (
              kMeasureIncompleteLastInRepeatHooklessEnding);
            break;
            
          case msrMeasure::kMeasureRepeatNextMeasureAfterCommonPart:
            setMeasureKind (
              kMeasureIncompleteNextMeasureAfterCommonPart);
            break;
          case msrMeasure::kMeasureRepeatNextMeasureAfterHookedEnding:
            setMeasureKind (
              kMeasureIncompleteNextMeasureAfterHookedEnding);
            break;
          case msrMeasure::kMeasureRepeatNextMeasureAfterHooklessEnding:
            setMeasureKind (
              kMeasureIncompleteNextMeasureAfterHooklessEnding);
            break;
        } // switch
      }
    }
  
    else if (fActualMeasureWholeNotes > fFullMeasureWholeNotes) {
      // this is an overfull measure
  
      // set it's measure kind
      setMeasureKind (kMeasureOvercomplete);
      
      // set it's 'relative to a repeat' kind
      setMeasureRepeatKind (
        kMeasureRepeatNone);

      // set it's measure purist number
      voice->
        incrementVoiceCurrentMeasurePuristNumber (
          inputLineNumber,
          "determineMeasureKindAndPuristNumber() kMeasureOvercomplete");
      
      setMeasurePuristNumber (
        voice->
          getVoiceCurrentMeasurePuristNumber ());
    }

    /* JMI
    else {
      // strange measure...

      // set it's 'relative to a repeat' kind
      setMeasureRepeatKind (
        kMeasureRepeatNone);

      stringstream s;
  
      displayMeasure (
        inputLineNumber,
        "determineMeasureKindAndPuristNumber() strange measure...");
        
      s <<
        "measure '" <<
        fMeasureNumber <<
        "' is in a strange state" <<
        asShortString () <<
        ", line " << inputLineNumber;
        
      msrInternalError (
        gGeneralOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());  
    }
    */
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

  // set measure purist number
  setMeasurePuristNumber (
    voice->
      getVoiceCurrentMeasurePuristNumber ());

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    displayMeasure (
      inputLineNumber,
      "determineMeasureKindAndPuristNumber() 2");
  }
#endif

  gIndenter--;
}

void msrMeasure::padUpToPositionInMeasure (
  int      inputLineNumber,
  rational positionInMeasure)
{
  gIndenter++;
  
  // fetch the part measure whole notes high tide
  rational
    partActualMeasureWholeNotesHighTide =
      fetchMeasurePartUplink ()->
        getPartActualMeasureWholeNotesHighTide ();
    
  rational
    lengthToReach =
      partActualMeasureWholeNotesHighTide;

  if (fActualMeasureWholeNotes < lengthToReach) {
    // appending a rest to this measure to reach lengthToReach 
    rational
      missingDuration =
        lengthToReach - fActualMeasureWholeNotes;
      
    // fetch the voice
    S_msrVoice
      measureVoice =
        fMeasureSegmentUplink->
          getSegmentVoiceUplink ();
    
    // create a rest or a skip depending on measureVoice kind
    S_msrNote
      paddingNote =
        createPaddingNoteForVoice (
          inputLineNumber,
          missingDuration,
          measureVoice);

    // register rest's position in measure
    paddingNote->
      setNotePositionInMeasure (fActualMeasureWholeNotes);
           
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
       ":" <<
       fActualMeasureWholeNotes <<
       " % --> @" << fMeasureNumber << // JMI
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
       ":" << partActualMeasureWholeNotesHighTide <<
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

  gIndenter--;
}

void msrMeasure::finalizeMeasure (
  int                  inputLineNumber,
  msrMeasureRepeatKind measureRepeatKind,
  string               context)
{
  if (fMeasureHasBeenFinalized) {
    stringstream s;

    s <<
      "Attempting to finalize  measure \"" <<
      asShortString () <<
      "\" more than once";
      
 //   msrInternalError (
    msrInternalWarning (
      gGeneralOptions->fInputSourceName,
      fInputLineNumber,
  //    __FILE__, __LINE__,
      s.str ());  
  }
  
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ();

  // fetch the part measure whole notes high tide
#ifdef TRACE_OPTIONS
  rational
    partActualMeasureWholeNotesHighTide =
      fetchMeasurePartUplink ()->
        getPartActualMeasureWholeNotesHighTide ();
#endif
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceBarlines || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Finalizing measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in segment '" <<
      fMeasureSegmentUplink->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      voice->getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    displayMeasure (
      inputLineNumber,
      "finalizeMeasure() 1");
  }
#endif

  // sanity check
  switch (fMeasureKind) {
    case msrMeasure::kMeasureMusicallyEmpty:
      {
        stringstream s;
      
        s <<
          "measure '" <<
          fMeasureNumber <<
          ", measureDebugNumber: '" <<
          fMeasureDebugNumber <<
          "' in segment '" <<
          fMeasureSegmentUplink->getSegmentAbsoluteNumber () <<
          "' in voice \"" <<
          voice->getVoiceName () <<
          "\" (" << context << context << ")" <<
          ", line " << inputLineNumber <<
          " IS EMPTY";

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
      break;
    default:
      ;
  } // switch

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

  if (fActualMeasureWholeNotes.getNumerator () == 0) {
    stringstream s;
    
    s <<
      "Measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in segment '" <<
      fMeasureSegmentUplink->getSegmentAbsoluteNumber () <<
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
      if (gTraceOptions->fTraceBarlines || gTraceOptions->fTraceMeasures) {
        gLogIOstream <<
          "Considering delayed '" <<
          measureElement->asShortString () <<
          "' in measure '" <<
          asShortString () <<
          "' in voice \"" <<
          fMeasureSegmentUplink->
            getSegmentVoiceUplink ()
              ->getVoiceName () <<
          "\", line " << inputLineNumber <<
          ", has position in measure '" <<
          measureElement->getPositionInMeasure () <<
          ", actualMeasureWholeNotes = " << fActualMeasureWholeNotes <<
          ", partActualMeasureWholeNotesHighTide = " << partActualMeasureWholeNotesHighTide <<
          endl;
      }
#endif

      if (
        measureElement->getPositionInMeasure () <= fActualMeasureWholeNotes
      ) { // JMI
        // this is where measureElement should be appended at last
        
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceBarlines || gTraceOptions->fTraceMeasures) {
          gLogIOstream <<
            "Appending delayed '" <<
            measureElement->asShortString () <<
            "' in measure '" <<
            asShortString () <<
            "' in voice \"" <<
            fMeasureSegmentUplink->
              getSegmentVoiceUplink ()
                ->getVoiceName () <<
            "\", line " << inputLineNumber <<
            ", has position in measure '" <<
            measureElement->getPositionInMeasure () <<
            ", actualMeasureWholeNotes = " << fActualMeasureWholeNotes <<
            ", partActualMeasureWholeNotesHighTide = " << partActualMeasureWholeNotesHighTide <<
            endl;
        }
#endif

  // JMI      appendElementToMeasure (measureElement);
        fMeasureElementsList.push_back (measureElement);

        // remove it from the pending measure elements list
        i = fMeasurePendingMeasureElementsList.erase (i);
      }

      if (++i == iEnd) break;
    } // for    
  }
  
  // pad measure up to part measure whole notes high tide
  // and determine its measure kind if relevant
  switch (fMeasureKind) {
    case msrMeasure::kMeasureCadenza:
      break;

    case msrMeasure::kMeasureOvercomplete:
    case msrMeasure::kMeasureAnacrusis:
    case msrMeasure::kMeasureRegular:
    case msrMeasure::kMeasureIncompleteStandalone: // JMI
    case msrMeasure::kMeasureIncompleteLastInRepeatCommonPart: // JMI
    case msrMeasure::kMeasureIncompleteLastInRepeatHookedEnding: // JMI
    case msrMeasure::kMeasureIncompleteLastInRepeatHooklessEnding: // JMI
    case msrMeasure::kMeasureIncompleteNextMeasureAfterCommonPart: // JMI
    case msrMeasure::kMeasureIncompleteNextMeasureAfterHookedEnding: // JMI
    case msrMeasure::kMeasureIncompleteNextMeasureAfterHooklessEnding: // JMI
      break;

    case msrMeasure::kMeasureUnknown:
      // JMI ???
      break;
      
    case msrMeasure::kMeasureMusicallyEmpty:
      // fetch the part measure whole notes high tide
      rational
        partActualMeasureWholeNotesHighTide =
          fetchMeasurePartUplink ()->
            getPartActualMeasureWholeNotesHighTide ();
        
      padUpToPositionInMeasure ( // JMI ???
        inputLineNumber,
        partActualMeasureWholeNotesHighTide);

        /* JMI
      switch (fMeasureCreatedForARepeatKind) {
        case msrMeasure::kMeasureCreatedForARepeatNo:
          {
            // fetch the part measure whole notes high tide
            rational
              partActualMeasureWholeNotesHighTide =
                fetchMeasurePartUplink ()->
                  getPartActualMeasureWholeNotesHighTide ();
              
            padUpToPositionInMeasure (
              inputLineNumber,
              partActualMeasureWholeNotesHighTide);
          }
          break;

        case msrMeasure::kMeasureCreatedForARepeatBefore:
        case msrMeasure::kMeasureCreatedForARepeatAfter:
          // such a measure should not be padded with a rest
          break;

        case msrMeasure::kMeasureCreatedForARepeatPadded:
          // should not occur
          break;
      } // switch
      */
      break;
  } // switch

  // determine the measure kind and purist number
  determineMeasureKindAndPuristNumber (
    inputLineNumber,
    measureRepeatKind);

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
          fMeasureSegmentUplink->getSegmentAbsoluteNumber () <<
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

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    displayMeasure (
      inputLineNumber,
      "finalizeMeasure() 2");
  }
#endif

  gIndenter--;
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
      "Attempting to finalize  measure \"" <<
      asShortString () <<
      "\" more than once";
      
    msrInternalError (
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());  
  }

  gIndenter++;

/* JMI
  // get voiceClone
  msrMeasureRepeatKind
    measureRepeatKind =
      voiceClone->
        getMeasureRepeatKind ();
  */
  
  // determine the measure kind and purist number
  determineMeasureKindAndPuristNumber (
    inputLineNumber,
    kMeasureRepeatNextMeasureAfterHooklessEnding); // JMI

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
    i++) {
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
    case msrMeasure::kMeasureUnknown:
      result = "***measureUnknown***";
      break;
    case msrMeasure::kMeasureRegular:
      result = "measureRegular";
      break;
    case msrMeasure::kMeasureAnacrusis:
      result = "measureAnacrusis";
      break;
    case msrMeasure::kMeasureIncompleteStandalone:
      result = "measureIncompleteStandalone";
      break;
    case msrMeasure::kMeasureIncompleteLastInRepeatCommonPart:
      result = "measureIncompleteLastInRepeatCommonPart";
      break;
    case msrMeasure::kMeasureIncompleteLastInRepeatHookedEnding:
      result = "measureIncompleteLastInRepeatHookedEnding";
      break;
    case msrMeasure::kMeasureIncompleteLastInRepeatHooklessEnding:
      result = "measureIncompleteLastInRepeatHooklessEnding";
      break;
    case msrMeasure::kMeasureIncompleteNextMeasureAfterCommonPart:
      result = "measureIncompleteNextMeasureAfterCommonPart";
      break;
    case msrMeasure::kMeasureIncompleteNextMeasureAfterHookedEnding:
      result = "measureIncompleteNextMeasureAfterHookedEnding";
      break;
    case msrMeasure::kMeasureIncompleteNextMeasureAfterHooklessEnding:
      result = "measureIncompleteNextMeasureAfterHooklessEnding";
      break;
    case msrMeasure::kMeasureOvercomplete:
      result = "measureOvercomplete";
      break;
    case msrMeasure::kMeasureCadenza:
      result = "measureCadenza";
      break;
    case msrMeasure::kMeasureMusicallyEmpty:
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
    case msrMeasure::kMeasureImplicitYes:
      result = "measureImplicitYes";
      break;
    case msrMeasure::kMeasureImplicitNo:
      result = "measureImplicitNo";
      break;
  } // switch

  return result;
}

string msrMeasure::measureFirstInSegmentKindAsString (
    msrMeasureFirstInSegmentKind measureFirstInSegmentKind)
{
  string result;

  switch (measureFirstInSegmentKind) {
    case msrMeasure::kMeasureFirstInSegmentUnknown:
      result = "***measureFirstInSegmentUnknown***";
      break;
    case msrMeasure::kMeasureFirstInSegmentYes:
      result = "measureFirstInSegmentYes";
      break;
    case msrMeasure::kMeasureFirstInSegmentNo:
      result = "measureFirstInSegmentNo";
      break;
  } // switch

  return result;
}

string msrMeasure::measureRepeatKindAsString (
  msrMeasureRepeatKind measureRepeatKind)
{
  string result;

  switch (measureRepeatKind) {
    case msrMeasure::kMeasureRepeatUnknown:
      result = "***measureRepeatUnknown***";
      break;
    case msrMeasure::kMeasureRepeatNone:
      result = "measureRepeatNone";
      break;
    case msrMeasure::kMeasureRepeatCommonPartLastMeasure:
      result = "measureRepeatCommonPartLastMeasure";
      break;
    case msrMeasure::kMeasureRepeatHookedEndingLastMeasure:
      result = "measureRepeatHookedEndingLastMeasure";
      break;
    case msrMeasure::kMeasureRepeatHooklessEndingLastMeasure:
      result = "measureRepeatHooklessEndingLastMeasure";
      break;
    case msrMeasure::kMeasureRepeatNextMeasureAfterCommonPart:
      result = "measureRepeatNextMeasureAfterCommonPart";
      break;
    case msrMeasure::kMeasureRepeatNextMeasureAfterHookedEnding:
      result = "measureRepeatNextMeasureAfterHookedEnding";
      break;
    case msrMeasure::kMeasureRepeatNextMeasureAfterHooklessEnding:
      result = "measureRepeatNextMeasureAfterHooklessEnding";
      break;
  } // switch

  return result;
}

string msrMeasure::measureEndRegularKindAsString (
    msrMeasureEndRegularKind measureEndRegularKind)
{
  string result;

  switch (measureEndRegularKind) {
    case msrMeasure::kMeasureEndRegularUnknown:
      result = "***measureEndRegularUnknown***";
      break;
    case msrMeasure::kMeasureEndRegularYes:
      result = "measureEndRegularYes";
      break;
    case msrMeasure::kMeasureEndRegularNo:
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
    "Measure '" <<
    fMeasureNumber <<
    ", measurePuristNumber: " <<
    fMeasurePuristNumber <<
    ", measureDebugNumber: '" <<
    fMeasureDebugNumber <<
    "', " << measureKindAsString () <<
/* JMI
    ", actualMeasureWholeNotes: " << fActualMeasureWholeNotes <<
    ", fullMeasureWholeNotes: " << fFullMeasureWholeNotes <<
    */
    ", " <<
    singularOrPlural (
      fMeasureElementsList.size (), "element", "elements") <<
    ", line " << fInputLineNumber;

  return s.str ();
}
      
string msrMeasure::asString () const
{
  stringstream s;
  
  s <<
    "Measure '" <<
    fMeasureNumber <<
    ", measurePuristNumber: " <<
    fMeasurePuristNumber <<
    ", measureDebugNumber: '" <<
    fMeasureDebugNumber <<
    "', " << measureKindAsString () <<
    ", actualMeasureWholeNotes: " << fActualMeasureWholeNotes <<
    ", fullMeasureWholeNotes: " << fFullMeasureWholeNotes <<
    ", " <<
    singularOrPlural (
      fMeasureElementsList.size (), "element", "elements") <<
    ", line " << fInputLineNumber;

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
    "', measurePuristNumber: " <<
    fMeasurePuristNumber <<
    ", measureDebugNumber: '" <<
    fMeasureDebugNumber <<
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
    ", actualMeasureWholeNotes: " << fActualMeasureWholeNotes <<
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
    "measureRepeatKind" << " : " <<
    msrMeasure::measureRepeatKindAsString (
      fMeasureRepeatKind) << 
    endl <<
    
    setw (fieldWidth) <<
    "segmentUplink" << " : " <<
    fMeasureSegmentUplink->asShortString () <<
    endl;
    
    
#ifdef TRACE_OPTIONS
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ();

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
    "actualMeasureWholeNotesAsMSRString" << " : " <<
    actualMeasureWholeNotesAsMSRString () <<
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
  os <<
    endl;
    
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
    os <<
      endl;
    
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
    "measureRepeatKind" << " : " <<
    msrMeasure::measureRepeatKindAsString (
      fMeasureRepeatKind) << 

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
    "actualMeasureWholeNotes" << " : " <<
    fActualMeasureWholeNotes <<
    endl <<
    
    setw (fieldWidth) <<
    "fullMeasureWholeNotes" << " : " <<
    fActualMeasureWholeNotes <<
    endl;
    
#ifdef TRACE_OPTIONS
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ();
        
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
