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

#include "msrOptions.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________ 
// global variable
int msrVoice::gVoicesCounter = 0;

S_msrVoice msrVoice::create (
  int          inputLineNumber,
  msrVoiceKind voiceKind,
  int          voiceNumber,
  msrVoiceCreateInitialLastSegmentKind
               voiceCreateInitialLastSegmentKind,
  S_msrStaff   voiceStaffUplink)
{
  msrVoice* o =
    new msrVoice (
      inputLineNumber,
      voiceKind,
      voiceNumber,
      voiceCreateInitialLastSegmentKind,
      voiceStaffUplink);
  assert(o!=0);

  return o;
}

msrVoice::msrVoice (
  int          inputLineNumber,
  msrVoiceKind voiceKind,
  int          voiceNumber,
  msrVoiceCreateInitialLastSegmentKind
               voiceCreateInitialLastSegmentKind,
  S_msrStaff   voiceStaffUplink)
    : msrElement (inputLineNumber)
{  
  // sanity check
  msrAssert(
    voiceStaffUplink != nullptr,
    "voiceStaffUplink is null");

  // set voice staff uplink
  fVoiceStaffUplink = voiceStaffUplink;

  // set voice kind
  fVoiceKind = voiceKind;

  // set voice number
  fVoiceNumber = voiceNumber;
  
  // do other initializations
  initializeVoice (
    voiceCreateInitialLastSegmentKind);
}

msrVoice::~msrVoice ()
{}

S_msrPart msrVoice::fetchVoicePartUplink () const
{
 return
  fVoiceStaffUplink->
    getStaffPartUplink ();
}

void msrVoice::setVoiceNameFromNumber (
  int inputLineNumber,
  int voiceNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Setting the name of '" <<
      voiceKindAsString () <<
      "' voice \"" <<
      "\"" <<
      " from number: " << voiceNumber <<
      endl;
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
      fVoiceName =
        fVoiceStaffUplink->getStaffName () +
        "_Voice_" +
        int2EnglishWord (voiceNumber);
      break;
      
    case msrVoice::kHarmonyVoice:
      fVoiceName =
        fVoiceStaffUplink->getStaffName () +
        "_Voice_" +
        int2EnglishWord (
          voiceNumber - K_VOICE_HARMONY_VOICE_BASE_NUMBER) +
          "_HARMONY";
      break;
      
    case msrVoice::kFiguredBassVoice:
      {
        stringstream s;

        s <<
          "A '" <<
          getVoiceName () <<
          "' voice cannot get its name from its number";

        msrInternalError (
          gGeneralOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch
}

void msrVoice::setVoiceLastSegmentInVoiceClone (
  S_msrSegment segment)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceVoices || gGeneralOptions->fTraceSegments) {
    gLogIOstream <<
      "Setting segment '" <<
      segment->asShortString () <<
      "' as last segment in voice clone \"" <<
      fVoiceName <<
      "\" in staff \"" <<
      fVoiceStaffUplink->getStaffName () <<
      "\"" <<
      ", line " << segment->getInputLineNumber () <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    segment != nullptr,
    "segment is null");
    
  fVoiceLastSegment = segment;

  if (! fVoiceFirstSegment) {
    fVoiceFirstSegment = fVoiceLastSegment;
  }
}

void msrVoice::appendSegmentToVoiceClone (
  S_msrSegment segment)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceVoices || gGeneralOptions->fTraceSegments) {
    gLogIOstream <<
      "Appending segment '" <<
      segment->asShortString () <<
      "' to voice clone \"" <<
      fVoiceName <<
      "\" in staff \"" <<
      fVoiceStaffUplink->getStaffName () <<
      "\"" <<
      endl;
  }
#endif

  int inputLineNumber =
    segment->getInputLineNumber ();
    
  // does segment belong to a repeat?
  if (fVoiceRepeatsStack.size ()) {
    // yes

 // JMI   if (fVoiceLastSegment) {
      
    // move segment to whichever part of the repeat is adequate
    S_msrRepeat
      currentRepeat =
        fVoiceRepeatsStack.front ();

    currentRepeat->
      appendSegmentToRepeat (
        inputLineNumber,
        segment,
        "setVoiceCloneLastSegment()");
  }
  
  else {
    // no
  }

  {
/* JMI
    // move fVoiceLastSegment to the initial voice elements list
    moveVoiceLastSegmentToInitialVoiceElements (
      inputLineNumber,
      "setVoiceCloneLastSegment()");
      */
      
    // segment becomes the fVoiceLastSegment
#ifdef TRACE_OPTIONS
    if (gGeneralOptions->fTraceVoices || gGeneralOptions->fTraceSegments) {
      gLogIOstream <<
        "Segment '" <<
        segment->asShortString () <<
        "' becomes the new last segment in voice clone \"" <<
        fVoiceName <<
        "\" in staff \"" <<
        fVoiceStaffUplink->getStaffName () <<
        "\"" <<
        endl;
    }
#endif

    fVoiceLastSegment = segment;
  
    if (! fVoiceFirstSegment) {
      fVoiceFirstSegment = fVoiceLastSegment;
    }
  }
}

void msrVoice::initializeVoice (
  msrVoiceCreateInitialLastSegmentKind
    voiceCreateInitialLastSegmentKind)
{
  // the voice staff sequential number will be set
  // when regular voices are added to a staff
  // in registerVoiceInRegularVoicesMap ()
  fRegularVoiceStaffSequentialNumber = -1;

  gIndenter++;
  
  // compute voice number
  int voiceNumber =
    gMsrOptions->
      fCreateVoicesStaffRelativeNumbers // JMI use it
        ? fRegularVoiceStaffSequentialNumber
        : fVoiceNumber;
  
  // set voice name
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
      setVoiceNameFromNumber (
        fInputLineNumber,
        voiceNumber);
      break;
      
    case msrVoice::kHarmonyVoice:
      setVoiceNameFromNumber (
        fInputLineNumber,
        voiceNumber);
      break;
      
    case msrVoice::kFiguredBassVoice:
      fVoiceName =
        fVoiceStaffUplink->getStaffName () +
        "_FIGURED_BASS_Voice";
      break;
  } // switch
  
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Initializing voice \"" << fVoiceName <<
      "\" in staff \"" <<
      fVoiceStaffUplink->getStaffName () <<
      "\"" <<
      endl;
  }
#endif

  // check voice number
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
      // the voice number should be positive
      if (fVoiceNumber < 1 || fVoiceNumber > 4) {
        stringstream s;
    
        s <<
          "regular voice number " << fVoiceNumber <<
          " is not in the 1..4 range";

          /* JMI ???
        msrMusicXMLError (
          gGeneralOptions->fInputSourceName,
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
          */
      }
      break;
      
    case msrVoice::kHarmonyVoice:
      break;
      
    case msrVoice::kFiguredBassVoice:
      if (fVoiceNumber != K_PART_FIGURED_BASS_VOICE_NUMBER) {
        stringstream s;
    
        s <<
          "figured bass voice number " << fVoiceNumber <<
          " is not equal to " << K_PART_FIGURED_BASS_VOICE_NUMBER;
          
        msrInternalError (
          gGeneralOptions->fInputSourceName,
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

  // voice shortest note
  fVoiceShortestNoteDuration =
    rational (INT_MAX, 1);
  fVoiceShortestNoteTupletFactor =
    rational (1, 1);
  
  // set voice number
  fVoiceCurrentMeasureNumber = // JMI "??";
    fetchVoicePartUplink ()->
      getPartCurrentMeasureNumber ();

  // music has not been inserted in voice yet
  fMusicHasBeenInsertedInVoice = false;

  // counters
  fVoiceActualNotesCounter     = 0;
  fVoiceRestsCounter           = 0;
  fVoiceSkipsCounter           = 0;
  fVoiceActualHarmoniesCounter = 0;

  // multiple rests
  fVoiceRemainingRestMeasures = 0;

  // get the initial staff details from the staff if any
/*
  S_msrStaffDetails
    staffStaffDetails =
      fVoiceStaffUplink->
        getStaffStaffDetails ();

  if (staffStaffDetails) {
    // append it to the voice
    appendStaffDetailsToVoice (
      staffStaffDetails);
  }
  */

#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Initial contents of voice \"" << fVoiceName <<
      "\" in staff \"" <<
      fVoiceStaffUplink->getStaffName () <<
      "\":" <<
      endl;

    gIndenter++;

    this->print (gLogIOstream);

    gIndenter--;
  }
#endif

  gIndenter--;
}

void msrVoice::changeVoiceIdentity ( // after a deep copy
  int voiceNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Changing identity of voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", number: " << voiceNumber <<
      endl;
  }
#endif

  // make it a regular voice
  setVoiceKind (
    msrVoice::kRegularVoice);

  // set its voice number
  setVoiceNumber (
    voiceNumber);

  // set its name
  setVoiceNameFromNumber (
    fInputLineNumber,
    voiceNumber);
}
    
S_msrVoice msrVoice::createVoiceNewbornClone (
  S_msrStaff staffClone)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Creating a newborn clone of voice \"" <<
      getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    staffClone != nullptr,
    "staffClone is null");
    
  S_msrVoice
    newbornClone =
      msrVoice::create (
        fInputLineNumber,
        fVoiceKind,
        fVoiceNumber,
        msrVoice::kCreateInitialLastSegmentNo,
          // initial segment will be created upon a later segment visit
        staffClone);

  // voice numbers
  newbornClone->fRegularVoiceStaffSequentialNumber =
    fRegularVoiceStaffSequentialNumber;

  // voice name
  newbornClone->fVoiceName =
    fVoiceName;

/* JMI
  // counters
  newbornClone->fVoiceActualNotesCounter =
    fVoiceActualNotesCounter;

  newbornClone->fVoiceRestsCounter =
    fVoiceRestsCounter;

  newbornClone->fVoiceSkipsCounter =
    fVoiceSkipsCounter;

  newbornClone->fVoiceActualHarmoniesCounter =
    fVoiceActualHarmoniesCounter;

  // measures
  newbornClone->fVoiceCurrentMeasureNumber =
    fVoiceCurrentMeasureNumber;

  // musically empty voices
  newbornClone->fMusicHasBeenInsertedInVoice =
    fMusicHasBeenInsertedInVoice;
  
  // multple rests
  newbornClone->fVoiceContainsMultipleRests =
    fVoiceContainsMultipleRests;
    */

  return newbornClone;
}

S_msrVoice msrVoice::createVoiceDeepCopy (
  int          inputLineNumber,
  msrVoiceKind voiceKind,
  int          voiceNumber,
  S_msrStaff   containingStaff)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Creating a deep copy of voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", to be placed in containing staff \"" <<
      containingStaff->getStaffName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingStaff != nullptr,
    "containingStaff is null");
    
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceVoicesDetails) {
    gLogIOstream <<
      "****" <<
      " BEFORE voiceDeepCopy = " <<
      endl;
  
    print (gLogIOstream);
    
    gLogIOstream <<
      endl <<
      "****" <<
      endl;
  }
#endif
    
  S_msrVoice
    voiceDeepCopy =
      msrVoice::create (
        fInputLineNumber,
        voiceKind,
        voiceNumber,
        msrVoice::kCreateInitialLastSegmentNo,
          // the voice initial last segment
          // will be created by deep cloning below
        containingStaff);

  // voice numbers
  voiceDeepCopy->fRegularVoiceStaffSequentialNumber =
    fRegularVoiceStaffSequentialNumber;

  // voice name
  if (false) { // JMI
    voiceDeepCopy->fVoiceName =
      fVoiceName;
  }

/* JMI
  // set voice name // JMI
  voiceDeepCopy->
    setVoiceNameFromNumber (
      fInputLineNumber,
      voiceNumber);
*/

  // counters
  voiceDeepCopy->fVoiceActualNotesCounter =
    fVoiceActualNotesCounter;

  voiceDeepCopy->fVoiceRestsCounter =
    fVoiceRestsCounter;

  voiceDeepCopy->fVoiceSkipsCounter =
    fVoiceSkipsCounter;

  voiceDeepCopy->fVoiceActualHarmoniesCounter =
    fVoiceActualHarmoniesCounter;

  // measures
  voiceDeepCopy->fVoiceCurrentMeasureNumber =
    fVoiceCurrentMeasureNumber;

  // musically empty voices
  voiceDeepCopy->fMusicHasBeenInsertedInVoice =
    fMusicHasBeenInsertedInVoice;

  // initial elements
  int numberOfInitialElements =
    fInitialVoiceElementsList.size ();
    
  if (numberOfInitialElements) {
#ifdef TRACE_OPTIONS
    if (gGeneralOptions->fTraceVoices) {
      gLogIOstream <<
        "There are " <<
        numberOfInitialElements <<
        " initial elements in voice \"" <<
        getVoiceName () <<
        "\"" <<
        endl;
    }
#endif

   list<S_msrVoiceElement>::const_iterator
      iBegin = fInitialVoiceElementsList.begin (),
      iEnd   = fInitialVoiceElementsList.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      // handle the deep copy
      S_msrVoiceElement
        elementDeepCopy;
        
      if (
        S_msrRepeat repeat = dynamic_cast<msrRepeat*>(&(*(*i)))
        ) {
          /* JMI ???
        // create the repeat deep copy
        elementDeepCopy =
          repeat->createRepeatDeepCopy (
            voiceDeepCopy);
            */
      }
    
      else if (
        // create the segment deep copy
        S_msrSegment segment = dynamic_cast<msrSegment*>(&(*(*i)))
        ) {
        elementDeepCopy =
          segment->createSegmentDeepCopy (
            voiceDeepCopy);
      }
      
      else {
        stringstream s;

        s <<
          "voice  \"" <<
          getVoiceName () <<
          "\" initial elements element should be a repeat or a segment";

        msrInternalError (
          gGeneralOptions->fInputSourceName,
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
  
      // append the element deep copy to the voice deep copy
      voiceDeepCopy->
        fInitialVoiceElementsList.push_back (
          elementDeepCopy);
        
      if (++i == iEnd) break;
    } // for
  }

  else {    
#ifdef TRACE_OPTIONS
    if (gGeneralOptions->fTraceVoices) {
      gLogIOstream <<
        "There are no initial elements in voice \"" <<
        getVoiceName () <<
        "\" to be deep copied" <<
        endl;
    }
#endif
  }

  // last segment
  if (fVoiceLastSegment) { // JMI
    voiceDeepCopy->fVoiceLastSegment =
      fVoiceLastSegment->
        createSegmentDeepCopy (
          voiceDeepCopy);

    if (! voiceDeepCopy->fVoiceFirstSegment) {
      voiceDeepCopy->fVoiceFirstSegment =
        voiceDeepCopy->fVoiceLastSegment;
    }
  }
  else {    
#ifdef TRACE_OPTIONS
    if (gGeneralOptions->fTraceVoices) {
      gLogIOstream <<
        "There is no last segment in voice \"" <<
        getVoiceName () <<
        "\" to be deep copied" <<
        endl;
    }
#endif
  }
  
  // multiple rests
  voiceDeepCopy->fVoiceContainsMultipleRests =
    fVoiceContainsMultipleRests;

  for (
    map<string, S_msrStanza>::const_iterator i = fVoiceStanzasMap.begin ();
    i != fVoiceStanzasMap.end ();
    i++) {
    S_msrStanza stanza = (*i).second;

    voiceDeepCopy->
      addStanzaToVoiceWithoutCatchUp ( // JMI
        stanza->
          createStanzaDeepCopy (
            voiceDeepCopy));
  } // for

  // uplinks
  voiceDeepCopy->fVoiceStaffUplink =
    containingStaff;

#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceVoicesDetails) {
    gLogIOstream <<
      "****" <<
      " AFTER voiceDeepCopy = " <<
      endl <<
      voiceDeepCopy <<
      endl <<
      "****" <<
      endl;
  }
#endif
    
  return voiceDeepCopy;
}

void msrVoice::setNextMeasureNumberInVoice (
  int    inputLineNumber,
  string nextMeasureNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Setting next measure number to '" <<
      nextMeasureNumber <<
      ", in voice \"" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  fVoiceLastSegment->
    setNextMeasureNumberInSegment (
      inputLineNumber,
      nextMeasureNumber);

  // is there a pending multiple rest in this voice?
  if (fVoiceMultipleRestWaitingForItsNextMeasureNumber) {      
    // yes
#ifdef TRACE_OPTIONS
    if (gGeneralOptions->fTraceRepeats) {
      gLogIOstream <<
        "There is a multiple rest waiting for its next measure number" <<
        ", fVoiceRemainingRestMeasures = " <<
        fVoiceRemainingRestMeasures <<
        "' in voice \"" <<
        fVoiceName << "\"" <<
        endl;
    }
#endif

    fVoiceRemainingRestMeasures--;

    // is this the last measure in the row?
    if (fVoiceRemainingRestMeasures == 0) {
      // yes, set waiting multiple rest's next measure number
#ifdef TRACE_OPTIONS
      if (gGeneralOptions->fTraceRepeats) {
        gLogIOstream <<
          "Setting multiple rest next measure number to '" <<
          nextMeasureNumber <<
          "' in voice \"" <<
          fVoiceName << "\"" <<
          endl;
      }
#endif

      fVoiceMultipleRestWaitingForItsNextMeasureNumber->
        setMultipleRestNextMeasureNumber (
          nextMeasureNumber);

      // forget about this waiting multiple rest
      fVoiceMultipleRestWaitingForItsNextMeasureNumber = nullptr;
    }
  }

  gIndenter--;
}

void msrVoice::createNewLastSegmentForVoice (
  int    inputLineNumber,
  string context)
{
  // create the last segment
  fVoiceLastSegment =
    msrSegment::create (
      inputLineNumber,
      this);

  if (! fVoiceFirstSegment) {
    fVoiceFirstSegment = fVoiceLastSegment;
  }

#ifdef TRACE_OPTIONS
  if (
    gGeneralOptions->fTraceMeasures
      ||
    gGeneralOptions->fTraceRepeats
      ||
    gGeneralOptions->fTraceSegments
      ||
    gGeneralOptions->fTraceVoices
  ) {
    gLogIOstream <<
      "Creating a new last segment '" <<
      fVoiceLastSegment->asShortString () <<
     "' for voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
}

S_msrMeasure msrVoice::createMeasureAndAppendItToVoice (
  int    inputLineNumber,
  string measureNumber,
  int    measureOrdinalNumber,
  msrMeasure::msrMeasureImplicitKind
         measureImplicitKind)
{
  fVoiceCurrentMeasureNumber = measureNumber;

#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Creating measure '" <<
      measureNumber <<
      "' and appending it to voice \"" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
    createNewLastSegmentForVoice (
      inputLineNumber,
      "createMeasureAndAppendItToVoice()");
  }

  // append new measure with given number
  S_msrMeasure
    result =
      fVoiceLastSegment->
        createMeasureAndAppendItToSegment (
          inputLineNumber,
          measureNumber,
          measureOrdinalNumber,
          measureImplicitKind);

  // handle voice kind
  switch (fVoiceKind) {
    case msrVoice::kHarmonyVoice:
      break;
    case msrVoice::kRegularVoice:
    /* JMI
      // append a new measure with given number
      fVoiceLastSegment->
        createMeasureAndAppendItToSegment (
          inputLineNumber,
          measureNumber,
          measureOrdinalNumber,
          measureImplicitKind);
*/

      // append new measure with given number to voice harmony voice if any
      if (fHarmonyVoiceForRegularVoice) {
        fHarmonyVoiceForRegularVoice->
          createMeasureAndAppendItToVoice (
            inputLineNumber,
            measureNumber,
            measureOrdinalNumber,
            measureImplicitKind);
      }
      break;
    case msrVoice::kFiguredBassVoice:
      break;
  } // switch

  gIndenter--;

  return result;
}

void msrVoice::createNewLastSegmentFromFirstMeasureForVoice (
  int          inputLineNumber,
  S_msrMeasure firstMeasure,
  string       context)
{
  // create the last segment
  fVoiceLastSegment =
    msrSegment::create (
      inputLineNumber,
      this);

  if (! fVoiceFirstSegment) {
    fVoiceFirstSegment = fVoiceLastSegment;
  }
  
#ifdef TRACE_OPTIONS
  if (
    gGeneralOptions->fTraceSegments
      ||
    gGeneralOptions->fTraceMeasures
      ||
    gGeneralOptions->fTraceRepeats
      ||
    gGeneralOptions->fTraceVoices
  ) {
    gLogIOstream <<
      "Creating a new last segment '" <<
      fVoiceLastSegment->asShortString () <<
      "' with first measure '" <<
      firstMeasure->getMeasureNumber () <<
      "' for voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // append firstMeasure to it
  fVoiceLastSegment->
    appendMeasureToSegment (firstMeasure);
}

void msrVoice::createNewLastSegmentAndANewMeasureBeforeARepeat ( // DROP ??? JMI
  int inputLineNumber,
  int fullMeasureWholeNotes)
{
/*
  This method is used for repeats
  It may lead to several measures having the same number,
  in case a bar line cuts them apart
  If this measure remains empty because the end of measure
  follows the barline, it will be removed upon finalizeMeasure ()
*/

/* JMI
  // finalize the current measure
  finalizeCurrentMeasureInVoice (
    inputLineNumber);

  // create the new voice last segment
#ifdef TRACE_OPTIONS
  if (
    gGeneralOptions->fTraceVoices
      ||
    gGeneralOptions->fTraceSegments
      ||
    gGeneralOptions->fTraceMeasures
      ||
    gGeneralOptions->fTraceRepeats
  ) {
    gLogIOstream <<
      "Creating a new last segment containing a new measure '" <<
      fVoiceCurrentMeasureNumber <<
      "' before a repeat for voice \"" <<
      getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fVoiceLastSegment =
    msrSegment::create (
      inputLineNumber,
      this);

  if (! fVoiceFirstSegment) {
    fVoiceFirstSegment = fVoiceLastSegment;
  }

  // create a measure
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceSegments) {
    gLogIOstream <<
      "Creating measure '" << fVoiceCurrentMeasureNumber <<
      "' in segment " << asString () <<
      ", in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create the new measure with number newMeasureMeasureNumber
  S_msrMeasure
    newMeasure =
      msrMeasure::create (
        inputLineNumber,
        fVoiceCurrentMeasureNumber,
        fVoiceLastSegment);

  // set 'first in segment' kind
  newMeasure->
    setMeasureFirstInSegmentKind (
      msrMeasure::kMeasureFirstInSegmentYes);
  
  // set it as being created before a repeat
  newMeasure->
    setMeasureCreatedForARepeatKind (
      msrMeasure::kMeasureCreatedForARepeatBefore);
    
  // set it's full measure whole notes
  newMeasure->setFullMeasureWholeNotes (
    fullMeasureWholeNotes);
    
  // append new measure to new voice last segment
  fVoiceLastSegment->
    appendMeasureToSegment (newMeasure);
    */
}

void msrVoice::createNewLastSegmentAndANewMeasureAfterARepeat ( // DROP ??? JMI
  int inputLineNumber,
  int fullMeasureWholeNotes)
{
/*
  This method is used for repeats
  It may lead to several measures having the same number,
  in case a bar line cuts them apart
  If this measure remains empty because the end of measure
  follows the barline, it will be removed upon finalizeMeasure ()
*/

/*
  // finalize the current measure
  finalizeCurrentMeasureInVoice (
    inputLineNumber);

  // create the segment
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Creating a new last segment containing a new measure '" <<
      fVoiceCurrentMeasureNumber <<
      "' after a repeat for voice \"" <<
      getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create the new voice last segment
  fVoiceLastSegment =
    msrSegment::create (
      inputLineNumber,
      this);

  if (! fVoiceFirstSegment) {
    fVoiceFirstSegment = fVoiceLastSegment;
  }

  // create the new measure with number newMeasureMeasureNumber
#ifdef TRACE_OPTIONS
  if (
    gGeneralOptions->fTraceVoices
      ||
    gGeneralOptions->fTraceSegments
      ||
    gGeneralOptions->fTraceMeasures
      ||
    gGeneralOptions->fTraceRepeats
  ) {
    gLogIOstream <<
      "Creating a new last segment containing a new measure '" <<
      fVoiceCurrentMeasureNumber <<
      "' after a repeat for voice \"" <<
      getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  S_msrMeasure
    newMeasure =
      msrMeasure::create (
        inputLineNumber,
        fVoiceCurrentMeasureNumber,
        fVoiceLastSegment);

  // set 'first in segment' kind
  newMeasure->
    setMeasureFirstInSegmentKind (
      msrMeasure::kMeasureFirstInSegmentYes);
  
  // set it as being created after a repeat
  newMeasure->
    setMeasureCreatedForARepeatKind (
      msrMeasure::kMeasureCreatedForARepeatAfter);
    
  // set it's full measure whole notes
  newMeasure->setFullMeasureWholeNotes (
    fullMeasureWholeNotes);
    
  // append new measure to new voice last segment
  fVoiceLastSegment->
    appendMeasureToSegment (newMeasure);
    */
}

S_msrVoice msrVoice::createHarmonyVoiceForRegularVoice (
  int    inputLineNumber,
  string currentMeasureNumber)
{
  if (fHarmonyVoiceForRegularVoice) {
    stringstream s;

    s <<
      "Voice \"" <<
      getVoiceName () <<
      "\" already has a harmony voice";

    msrInternalError (
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create the voice harmony voice
  int harmonyVoiceForRegularVoiceNumber =
    K_VOICE_HARMONY_VOICE_BASE_NUMBER + fVoiceNumber;
    
#ifdef TRACE_OPTIONS
  if (
    gGeneralOptions->fTraceHarmonies
      ||
    gGeneralOptions->fTraceVoices
      ||
    gGeneralOptions->fTraceStaves) {
    gLogIOstream <<
      "Creating harmony voice for regular voice \"" << getVoiceName () << "\"" <<
      " with voice number " << harmonyVoiceForRegularVoiceNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fHarmonyVoiceForRegularVoice =
    msrVoice::create (
      inputLineNumber,
      msrVoice::kHarmonyVoice,
      harmonyVoiceForRegularVoiceNumber,
      msrVoice::kCreateInitialLastSegmentYes,
      fVoiceStaffUplink);

  // register it in the staff
  fVoiceStaffUplink->
    registerVoiceInStaff (
      inputLineNumber,
      fHarmonyVoiceForRegularVoice);

  // set back link
  fHarmonyVoiceForRegularVoice->
    fRegularVoiceForHarmonyVoice = this;

  return fHarmonyVoiceForRegularVoice;
}

S_msrStanza msrVoice::addStanzaToVoiceByItsNumber (
  int    inputLineNumber,
  string stanzaNumber)
{
  if (fVoiceStanzasMap.count (stanzaNumber)) {
    stringstream s;
    
    s <<
      "stanza " << stanzaNumber <<
      " already exists in voice \"" <<
      getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;

    msrInternalError (
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create the stanza
  S_msrStanza
    stanza =
      msrStanza::create (
        inputLineNumber,
        stanzaNumber,
        this);

  // add the stanza to this voice
  addStanzaToVoice (stanza);

  // return it
  return stanza;
}

void msrVoice::addStanzaToVoice (S_msrStanza stanza)
{
  // get stanza number
  string stanzaNumber =
    stanza->getStanzaNumber ();
    
  // register stanza in this voice
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceLyrics) {
    gLogIOstream <<
      "Adding stanza " << stanza->getStanzaName () <<
      " (" << stanzaNumber <<
      ") to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // add the stanza to this voice
  fVoiceStanzasMap [stanzaNumber] = stanza;
}

void msrVoice::addStanzaToVoiceWithoutCatchUp (S_msrStanza stanza)
{
  // get stanza number
  string stanzaNumber =
    stanza->getStanzaNumber ();
    
  // register stanza in this voice
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceLyrics) {
    gLogIOstream <<
      "Adding stanza " << stanza->getStanzaName () <<
      " (" << stanzaNumber <<
      ") to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // add the stanza to this voice
  fVoiceStanzasMap [stanzaNumber] = stanza;
}

S_msrStanza msrVoice::createStanzaInVoiceIfNotYetDone (
  int    inputLineNumber,
  string stanzaNumber,
  string stanzaName) // JMI
{
  S_msrStanza stanza;

  // is stanzaNumber already known in voice?
  if (fVoiceStanzasMap.count (stanzaNumber)) {
    // yes, use it
    stanza =
      fVoiceStanzasMap [stanzaNumber];
  }
  
  else {
    // no, create it and add it to the voice
#ifdef TRACE_OPTIONS
    if (gGeneralOptions->fTraceVoices || gGeneralOptions->fTraceLyrics) {
      gLogIOstream <<
        "Creating stanza" <<
        " number " << stanzaNumber <<
        ", name \"" << stanzaName << "\"" <<
        ", in voice \"" << getVoiceName () << "\"" <<
        ", line " << inputLineNumber <<
        ", fVoiceStanzasMap.size () = " << fVoiceStanzasMap.size () <<
        endl;
    }
#endif

    stanza =
      addStanzaToVoiceByItsNumber (
        inputLineNumber, stanzaNumber);
  }

  return stanza;
}

S_msrStanza msrVoice::fetchStanzaInVoice (
  int    inputLineNumber,
  string stanzaNumber,
  string stanzaName) // JMI
{
  S_msrStanza stanza;

  // is stanzaNumber known in voice?
  if (fVoiceStanzasMap.count (stanzaNumber)) {
    // yes, use it
    stanza =
      fVoiceStanzasMap [stanzaNumber];
  }
  
  else {
    stringstream s;
    
    gLogIOstream <<
      endl << endl << endl <<
      "VOICEVOICEVOICEVOICEVOICEVOICEVOICEVOICE" <<
      endl;

    print (gLogIOstream);
      
    gLogIOstream <<
      "VOICEVOICEVOICEVOICEVOICEVOICEVOICEVOICE" <<
      endl << endl << endl <<
      endl;


    s <<
      "Stanza" <<
      " number " << stanzaNumber <<
      ", name \"" << stanzaName << "\"" <<
      ", not found in voice \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      ", fVoiceStanzasMap.size () = " << fVoiceStanzasMap.size () <<
      endl;

    msrInternalError (
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return stanza;
}

void msrVoice::appendClefToVoice (S_msrClef clef)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceClefs || gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending clef '" << clef->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  gIndenter++;
  
  if (fMusicHasBeenInsertedInVoice) {
    // append clef to last segment
    fVoiceLastSegment->
      appendClefToSegment (clef);
  }

  else {
    // moving clefs to the left, thus prepend to last segment
    fVoiceLastSegment->
      prependClefToSegment (clef);
  }

  gIndenter--;
}

void msrVoice::displayVoiceContents (
  int    inputLineNumber,
  string context)
{
  gLogIOstream <<
    endl <<
    "*********>> Voice " << context << " \"" <<
    getVoiceName () <<
    "\"" <<
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

void msrVoice::appendKeyToVoice (S_msrKey key)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceKeys || gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending key '" << key->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OPTIONS
  if (
    gGeneralOptions->fTraceRepeats
      ||
    gGeneralOptions->fTraceVoices
  ) {
    displayVoiceContents ( // JMI TEMP
      key->getInputLineNumber (),
      "appendKeyToVoice()");
  }
#endif
  
  // append key to last segment
  fVoiceLastSegment->
    appendKeyToSegment (key);

  gIndenter--;
}

void msrVoice::appendTimeToVoice (S_msrTime time)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceTimes || gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending time '" << time->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  gIndenter++;
  
  // append time to the last segment
  fVoiceLastSegment->
    appendTimeToSegment (time);

  gIndenter--;
}

void msrVoice::appendTimeToVoiceClone (S_msrTime time)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceTimes || gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending time '" << time->asString () <<
      "' to voice clone \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  gIndenter++;
  
  // append time to the last segment
  fVoiceLastSegment->
    appendTimeToSegmentClone (time);

  gIndenter--;
}

S_msrNote msrVoice::fetchVoiceFirstNonGraceNote () const
{
  S_msrNote result;

  if (fVoiceFirstSegment) {
    // get the segment's measures list
    const list<S_msrMeasure>&
      firstSegmentMeasuresList =
        fVoiceFirstSegment->
          getSegmentMeasuresList ();

    if (firstSegmentMeasuresList.size ()) {
      // get the segment's first measure
      S_msrMeasure
        firstMeasure =
          firstSegmentMeasuresList.front ();

      // get the first measure's elements list
      const list<S_msrMeasureElement>&
        firstMeasureElementsList =
          firstMeasure->
            getMeasureElementsList ();

      // fetch the first note in the first measure to which
      // a grace notes group can be attached
      // i.e. one not in a grace notes group itself,
      // possibly inside a chord or tuplet

      if (firstMeasureElementsList.size ()) {
        list<S_msrMeasureElement>::const_iterator
          iBegin = firstMeasureElementsList.begin (),
          iEnd   = firstMeasureElementsList.end (),
          i      = iBegin;
        for ( ; ; ) {
          S_msrMeasureElement element = (*i);
          
          if (
            S_msrNote note = dynamic_cast<msrNote*>(&(*element))
            ) {    
            result = note;
            break;
          }
        
          else if (
            S_msrChord chord = dynamic_cast<msrChord*>(&(*element))
            ) {
            // get the chord's first note
            result = chord->fetchChordFirstNonGraceNote ();
            break;
          }
          
          else if (
            S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(*element))
            ) {
            // get the tuplet's first note
            result = tuplet->fetchTupletFirstNonGraceNote ();
            break;
          }
          
          else if (
            S_msrRepeat repeat = dynamic_cast<msrRepeat*>(&(*element))
            ) {
            // get the repeat's first note
            result = repeat->fetchRepeatFirstNonGraceNote ();
            break;
          }
          
          else if (
            S_msrClef clef = dynamic_cast<msrClef*>(&(*element))
            ) {
            // ignore this clef
          }
          
          else if (
            S_msrKey key = dynamic_cast<msrKey*>(&(*element))
            ) {
            // ignore this key
          }
          
          else if (
            S_msrTime time = dynamic_cast<msrTime*>(&(*element))
            ) {
            // ignore this time
          }
          
          else {
            // ignore this element and return nullptr
            /*
            stringstream s;

            s <<
              "tuplet first element should be a note, a chord or another tuplet, found instead '" <<
              element->asShortString () <<
              "'";
              
            msrInternalError (
              gGeneralOptions->fInputSourceName,
              fInputLineNumber,
              __FILE__, __LINE__,
              s.str ());
              */
          }
      
          if (++i == iEnd) break;
        } // for
      }
    }
  }

  return result;
}

void msrVoice::registerShortestNoteIfRelevant (S_msrNote note)
{
  // is note the shortest one in this voice?
  rational
    noteSoundingWholeNotes =
      note->getNoteSoundingWholeNotes (),
    noteDisplayWholeNotes =
      note->getNoteDisplayWholeNotes (); // JMI
      
  if (noteSoundingWholeNotes < fVoiceShortestNoteDuration) {
    fVoiceShortestNoteDuration = noteSoundingWholeNotes;
  }
  if (noteDisplayWholeNotes < fVoiceShortestNoteDuration) {
    fVoiceShortestNoteDuration = noteDisplayWholeNotes;
  }
}

void msrVoice::registerNoteAsVoiceLastAppendedNote (S_msrNote note)
{
  fVoiceLastAppendedNote = note;
  
  // is note the shortest one in this voice?
  rational
    noteSoundingWholeNotes =
      note->getNoteSoundingWholeNotes (),
    noteDisplayWholeNotes =
      note->getNoteDisplayWholeNotes (); // JMI
      
  if (noteSoundingWholeNotes < fVoiceShortestNoteDuration) {
    fVoiceShortestNoteDuration = noteSoundingWholeNotes;
  }
  if (noteDisplayWholeNotes < fVoiceShortestNoteDuration) {
    fVoiceShortestNoteDuration = noteDisplayWholeNotes;
  }
}

void msrVoice::appendHarmonyToVoice (S_msrHarmony harmony)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending harmony '" << harmony->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  int inputLineNumber =
    harmony->getInputLineNumber ();
    
  switch (fVoiceKind) {
    case msrVoice::kHarmonyVoice:
      // skip to harmony note position in the voice
      padUpToActualMeasureWholeNotesInVoice (
        inputLineNumber,
        harmony->
          getHarmonyNoteUplink ()->
            getNotePositionInMeasure ());

      // append the harmony to the voice last segment
      fVoiceLastSegment->
        appendHarmonyToSegment (harmony);
    
      // register harmony
      fVoiceActualHarmoniesCounter++;
      fMusicHasBeenInsertedInVoice = true;

      break;
      
    case msrVoice::kRegularVoice:
    case msrVoice::kFiguredBassVoice:
      {
        stringstream s;

        s <<
          "cannot append a harmony to " <<
          voiceKindAsString () <<
          " voice \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          gGeneralOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch
}

void msrVoice::appendHarmonyToVoiceClone (S_msrHarmony harmony)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending harmony '" << harmony->asString () <<
      "' to voice clone \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif
      
  int inputLineNumber =
    harmony->getInputLineNumber ();
    
  switch (fVoiceKind) {
    case msrVoice::kHarmonyVoice:
      fVoiceLastSegment->
        appendHarmonyToSegmentClone (harmony);
    
      // register harmony
      fVoiceActualHarmoniesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrVoice::kRegularVoice:
    case msrVoice::kFiguredBassVoice:
      {
        stringstream s;

        s <<
          "cannot append a harmony to " <<
          voiceKindAsString () <<
          " voice clone \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          gGeneralOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch
}

void msrVoice::appendFiguredBassToVoice (
  S_msrFiguredBass figuredBass)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceFiguredBass || gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending figured bass '" << figuredBass->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kFiguredBassVoice:
      fVoiceLastSegment->
        appendFiguredBassToSegment (figuredBass);
    
      // register harmony
      fVoiceActualFiguredBassCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
      {
        stringstream s;

        s <<
          "cannot append a figured bass to " <<
          voiceKindAsString () <<
          " voice \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          gGeneralOptions->fInputSourceName,
          figuredBass->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch
}

void msrVoice::appendFiguredBassToVoiceClone (
  S_msrFiguredBass figuredBass)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceFiguredBass || gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending figured bass '" << figuredBass->asString () <<
      "' to voice clone \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif
      
  switch (fVoiceKind) {
    case msrVoice::kFiguredBassVoice:
      fVoiceLastSegment->
        appendFiguredBassToSegmentClone (figuredBass);
    
      // register figured bass
      fVoiceActualFiguredBassCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
      {
        stringstream s;

        s <<
          "cannot append a figured bass to " <<
          voiceKindAsString () <<
          " voice clone \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          gGeneralOptions->fInputSourceName,
          figuredBass->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch
}

void msrVoice::padUpToActualMeasureWholeNotesInVoice (
  int      inputLineNumber,
  rational wholeNotes)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceVoices || gGeneralOptions->fTraceMeasures) {
    gLogIOstream <<
      "Padding up to actual measure whole notes '" << wholeNotes <<
      "' in voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // pad up the voice's last segment
  fVoiceLastSegment->
    padUpToActualMeasureWholeNotesInSegment (
      inputLineNumber, wholeNotes);
      
  // pad up the voice's stanzas
  if (fVoiceStanzasMap.size ()) {
    for (
      map<string, S_msrStanza>::const_iterator i = fVoiceStanzasMap.begin ();
      i != fVoiceStanzasMap.end ();
      i++) {
      S_msrStanza stanza = (*i).second;

      stanza->padUpToActualMeasureWholeNotesInStanza (
        inputLineNumber, wholeNotes);
    } // for
  }

  gIndenter--;
}

void msrVoice::appendPaddingNoteToVoice (
  int inputLineNumber,
  int divisions,
  int divisionsPerQuarterNote)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceVoices || gGeneralOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending padding note of " << divisions <<
      " divisions to voice \"" <<
      getVoiceName () <<
      "\", divisions = " << divisions <<
      ",line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // pad up the voice's last segment
  fVoiceLastSegment->
    appendPaddingNoteToSegment (
      inputLineNumber,
      divisions,
      divisionsPerQuarterNote);
      
  // pad up the voice's stanzas
  if (fVoiceStanzasMap.size ()) {
    for (
      map<string, S_msrStanza>::const_iterator i = fVoiceStanzasMap.begin ();
      i != fVoiceStanzasMap.end ();
      i++) {
      S_msrStanza stanza = (*i).second;

      stanza->appendPaddingNoteToStanza (
        inputLineNumber,
        divisions,
        divisionsPerQuarterNote);
    } // for
  }

  gIndenter--;
}

void msrVoice::appendTransposeToVoice (
  S_msrTranspose transpose)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceTranspositions || gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending transpose '" <<
      transpose->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendTransposeToSegment (transpose);
}

void msrVoice::appendPartNameDisplayToVoice (
  S_msrPartNameDisplay partNameDisplay)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceTranspositions || gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending part name display '" <<
      partNameDisplay->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendPartNameDisplayToSegment (partNameDisplay);
}

void msrVoice::appendPartAbbreviationDisplayToVoice (
  S_msrPartAbbreviationDisplay partAbbreviationDisplay)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceTranspositions || gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending part abbreviation display '" <<
      partAbbreviationDisplay->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendPartAbbreviationDisplayToSegment (
      partAbbreviationDisplay);
}

void msrVoice::appendStaffDetailsToVoice (
  S_msrStaffDetails staffDetails)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending staff details '" <<
      staffDetails->asShortString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // staff details are attributes in MusicXML,
  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
    createNewLastSegmentForVoice (
      staffDetails->getInputLineNumber (),
      "appendStaffDetailsToVoice()");
  }

  fVoiceLastSegment->
    appendStaffDetailsToSegment (staffDetails);
}

/* JMI
void msrVoice::appendWordsToVoice (S_msrWords words)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceWords) {
    gLogIOstream <<
      "Appending words '" << words->asString () <<
      "' to voice " << getVoiceName () << endl;
  }
#endif

  S_msrElement w = words;
  fVoiceLastSegment->
    appendWordsToSegment (w);
}
*/

void msrVoice::appendTempoToVoice (S_msrTempo tempo)
{
  if (gMsrOptions->fTraceMsr) {
    gLogIOstream <<
      "Appending tempo '" << tempo->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  fVoiceLastSegment->
    appendTempoToSegment (tempo);
}

void msrVoice::appendOctaveShiftToVoice (
  S_msrOctaveShift octaveShift)
{
  if (gMsrOptions->fTraceMsr) {
    gLogIOstream <<
      "Appending octave shift '" <<
      octaveShift->octaveShiftKindAsString () <<
      "', size: " << octaveShift->getOctaveShiftSize () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  fVoiceLastSegment->
    appendOctaveShiftToSegment (octaveShift);
}

void msrVoice::appendScordaturaToVoice (
  S_msrScordatura scordatura)
{
  if (gMsrOptions->fTraceMsr) {
    gLogIOstream <<
      "Appending scordatura '" <<
      scordatura->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  fVoiceLastSegment->
    appendScordaturaToSegment (scordatura);
}

void msrVoice::appendAccordionRegistrationToVoice (
  S_msrAccordionRegistration
    accordionRegistration)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending accordion registration '" <<
      accordionRegistration->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendAccordionRegistrationToSegment (
      accordionRegistration);
}

void msrVoice::appendHarpPedalsTuningToVoice (
  S_msrHarpPedalsTuning
    harpPedalsTuning)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending harp pedals tuning '" <<
      harpPedalsTuning->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendHarpPedalsTuningToSegment (
      harpPedalsTuning);
}

void msrVoice::appendRehearsalToVoice (S_msrRehearsal rehearsal)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending rehearsal '" << rehearsal->getRehearsalText () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendRehearsalToSegment (rehearsal);
}

void msrVoice::appendVoiceStaffChangeToVoice (
  S_msrVoiceStaffChange voiceStaffChange)
{
  int inputLineNumber =
    voiceStaffChange->getInputLineNumber ();
    
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceVoices || gGeneralOptions->fTraceStaves) {
    gLogIOstream <<
      "Appending voice staff change '" <<
      voiceStaffChange->asString () <<
      "' to voice " <<
      " \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  // append voice staff change to voice's last segment
  fVoiceLastSegment->
    appendVoiceStaffChangeToSegment (
      voiceStaffChange);

  gIndenter--;
}

void msrVoice::appendNoteToVoice (S_msrNote note) {
  int inputLineNumber =
    note->getInputLineNumber ();
    
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending note '" <<
      endl;

    gIndenter++;
    
    gLogIOstream <<
      note <<
        "' to voice \"" << getVoiceName () << "\"" <<
        ", line " << inputLineNumber <<
        endl;

    gIndenter--;
  }
#endif

  // register whether music (i.e. not just skips)
  // has been inserted into the voice
  switch (note->getNoteKind ()) {
    case msrNote::k_NoNoteKind:
      break;

    case msrNote::kRestNote:
      // register rest
      fVoiceRestsCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kSkipNote:
      // don't account skips as music
      fVoiceSkipsCounter++;
      break;
      
    case msrNote::kUnpitchedNote:
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kStandaloneNote:
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kDoubleTremoloMemberNote:
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kGraceNote:
    case msrNote::kGraceChordMemberNote:
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kChordMemberNote:
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kTupletMemberNote:
    case msrNote::kGraceTupletMemberNote:
    case msrNote::kTupletMemberUnpitchedNote:
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
  } // switch
  
  // append the note to the last segment
  fVoiceLastSegment->
    appendNoteToSegment (note);

  // is this note the shortest one in this voice?
  this->
    registerShortestNoteIfRelevant (note);

  // register note as the last appended one into this voice
  fVoiceLastAppendedNote = note;
}

void msrVoice::appendNoteToVoiceClone (S_msrNote note) {
  int inputLineNumber =
    note->getInputLineNumber ();
    
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending note '" <<
      note->asShortString () <<
      "' to voice clone \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // register whether music (i.e. not just skips)
  // has been inserted into the voice
  switch (note->getNoteKind ()) {
    case msrNote::k_NoNoteKind:
      break;

    case msrNote::kRestNote:
      // register rest
      fVoiceRestsCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kSkipNote:
      // don't account skips as music
      fVoiceSkipsCounter++;
      break;
      
    case msrNote::kUnpitchedNote:
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kStandaloneNote:
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kDoubleTremoloMemberNote:
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kGraceNote:
    case msrNote::kGraceChordMemberNote:
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kChordMemberNote:
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kTupletMemberNote:
    case msrNote::kGraceTupletMemberNote:
    case msrNote::kTupletMemberUnpitchedNote:
      // register actual note
      fVoiceActualNotesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
  } // switch
  
  // append the note to the last segment
  fVoiceLastSegment->
    appendNoteToSegmentClone (note);

  // is this note the shortest one in this voice?
  this->
    registerShortestNoteIfRelevant (note);

  // register note as the last appended one into this voice
  fVoiceLastAppendedNote = note;
}

void msrVoice::appendDoubleTremoloToVoice (
  S_msrDoubleTremolo doubleTremolo)

{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceTremolos) {
    gLogIOstream <<
      "Appending double tremolo '" <<
      doubleTremolo->asShortString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendDoubleTremoloToSegment (doubleTremolo);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendChordToVoice (S_msrChord chord)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceChords) {
    gLogIOstream <<
      "Appending chord '" << chord->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendChordToSegment (chord);

  // get the chord's notes vector
  const vector<S_msrNote>&
    chordNotesVector =
      chord->
        getChordNotesVector ();

  int chordNotesVectorSize =
    chordNotesVector.size ();

  if (chordNotesVectorSize > 0) {
    {
      // get the chord's first note
      S_msrNote
        chordFirstNote = chordNotesVector [0];
    
      // is chordFirstNote the shortest one in this voice?
      this->
        registerShortestNoteIfRelevant (chordFirstNote);
    }
    
    {
      // get the chord's last note
      S_msrNote
        chordLastNote =
          chordNotesVector [chordNotesVectorSize - 1];
    
      // is chordLastNote the shortest one in this voice?
      this->
        registerShortestNoteIfRelevant (chordLastNote);

      // register chordLastNote as the last appended one into this voice
      fVoiceLastAppendedNote = chordLastNote;
    }
  }
  
  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendTupletToVoice (S_msrTuplet tuplet)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceTuplets) {
    gLogIOstream <<
      "Appending tuplet '" << tuplet->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  gIndenter++;
  
  fVoiceLastSegment->
    appendTupletToSegment (tuplet);

  gIndenter--;

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::addGraceNotesGroupBeforeAheadOfVoiceIfNeeded (
  S_msrGraceNotesGroup graceNotesGroup)
{
  int inputLineNumber =
    graceNotesGroup->getInputLineNumber ();
  
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceGraceNotes) {
    gLogIOstream <<
      "Adding grace notes '" <<
      graceNotesGroup->asString () <<
      "' ahead of voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

/* JMI
  gLogIOstream <<
    endl <<
    "======================= prependGraceNotesToVoice" <<
    endl;
  this->print (gLogIOstream);
  gLogIOstream <<
    "=======================" <<
    endl <<
    endl;
  */

  if (! fVoiceFirstSegment) {
    // these graceNotesGroup appears at the beginning of the voice:
    // create a first segment
    createNewLastSegmentForVoice (
      inputLineNumber,
      "addGraceNotesGroupBeforeAheadOfVoiceIfNeeded()");
      
    fVoiceFirstSegment = fVoiceLastSegment;

    // then create the first measure
    createMeasureAndAppendItToVoice (
      inputLineNumber,
      graceNotesGroup->
        getGraceNotesGroupMeasureNumber (),
      1, //    measureOrdinalNumber,
      msrMeasure::kMeasureImplicitNo);
  }
  
  // such grace notes groups should be attached to the voice's first note,
  // or to the first chord if the latter belongs to such

  // fetch the voice's first note
  S_msrNote
    voiceFirstNote =
      fetchVoiceFirstNonGraceNote (); // JMI
    
  // get the voice first note's chord uplink
  S_msrChord
    firstNoteChordUplink =
      voiceFirstNote->
        getNoteChordUplink ();
      
  if (firstNoteChordUplink) {
#ifdef TRACE_OPTIONS
    if (gGeneralOptions->fTraceGraceNotes || gGeneralOptions->fTraceChords) {
      gLogIOstream <<
        "Attaching grace notes before '" <<
        graceNotesGroup->asString () <<
        "' to the first chord of voice \"" << getVoiceName () <<
        "\", i.e. '" <<
        firstNoteChordUplink->asShortString () <<
        "'" <<
        endl;
    }
#endif

    firstNoteChordUplink->
      setChordGraceNotesGroupBefore (
        graceNotesGroup);
  }
  
  else {
#ifdef TRACE_OPTIONS
    if (gGeneralOptions->fTraceGraceNotes || gGeneralOptions->fTraceChords) {
      gLogIOstream <<
        "Attaching grace notes before '" <<
        graceNotesGroup->asString () <<
        "' to the first note of voice \"" << getVoiceName () <<
        "\", i.e. '" <<
        voiceFirstNote->asShortString () <<
        "'" <<
        endl;
    }
#endif

    voiceFirstNote->
      setNoteGraceNotesGroupBefore (
        graceNotesGroup);
  }

  fMusicHasBeenInsertedInVoice = true;
}

/* JMI
void msrVoice::appendAfterGraceNotesToVoice (
  S_msrAfterGraceNotes afterGraceNotes)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceGraceNotes) {
    gLogIOstream <<
      "Appending after grace notes " << // JMI AfterGraceNotes <<
      " to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendAfterGraceNotesToSegment (afterGraceNotes);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::prependAfterGraceNotesToVoice (
  S_msrAfterGraceNotes afterGraceNotes)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceGraceNotes) {
    gLogIOstream <<
      "Prepending after grace notes " << // JMI AfterGraceNotes <<
      " to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceFirstSegment->
    prependAfterGraceNotesToSegment (afterGraceNotes);

  fMusicHasBeenInsertedInVoice = true;
}
*/

void msrVoice::appendSyllableToVoice (
  int           inputLineNumber,
  string        stanzaNumber,
  string        stanzaName,
  S_msrSyllable syllable)
{
  // append syllable to this voice
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending syllable '" <<
      syllable->asString () <<
      "' to voice \"" << fVoiceName << "\"" <<
      endl;
  }
#endif

  // fetch stanzaNumber in this voice
  S_msrStanza
    stanza =
      createStanzaInVoiceIfNotYetDone (
        inputLineNumber,
        stanzaNumber,
        stanzaName);

  // add the syllable to the stanza
  stanza->
    appendSyllableToStanza (
      syllable);
}

void msrVoice::appendBarCheckToVoice (S_msrBarCheck barCheck)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending bar check '" << barCheck->asString () <<
      "' to voice \"" << getVoiceName () <<  "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendBarCheckToSegment (barCheck);
}

void msrVoice::appendBarNumberCheckToVoice (
  S_msrBarNumberCheck barNumberCheck)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending barnumber check '" <<
      barNumberCheck->asString () <<
      "' to voice \"" << getVoiceName () <<  "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendBarNumberCheckToSegment (barNumberCheck);
}

void msrVoice::appendLineBreakToVoice (S_msrLineBreak lineBreak)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending line break '" << lineBreak->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendLineBreakToSegment (lineBreak);

  // propagate this lineBreak to the voice stanzas if any
  if (fVoiceStanzasMap.size ()) {
    for (
      map<string, S_msrStanza>::const_iterator i = fVoiceStanzasMap.begin ();
      i != fVoiceStanzasMap.end ();
      i++) {
      S_msrStanza stanza = (*i).second;

      stanza->appendLineBreakSyllableToStanza (
        lineBreak->getInputLineNumber ());
    } // for
  }
}

void msrVoice::appendPageBreakToVoice (S_msrPageBreak pageBreak)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceMeasures) {
    gLogIOstream <<
      "Appending page break '" << pageBreak->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendPageBreakToSegment (pageBreak);
}

void msrVoice::prependOtherElementToVoice (S_msrMeasureElement elem) {
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Prepending other element '" << elem <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    prependOtherElementToSegment (elem);
}

void msrVoice::appendOtherElementToVoice (S_msrMeasureElement elem) {
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending other element '" << elem <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendOtherElementToSegment (elem);
}

S_msrMeasure msrVoice::fetchVoiceLastMeasure (
  int inputLineNumber) const
{
  S_msrMeasure result;
  
  msrAssert (
    fVoiceLastSegment != 0,
    "fVoiceLastSegment is null");

  const list<S_msrMeasure>&
    lastSegmentMeasuresList =
      fVoiceLastSegment->getSegmentMeasuresList ();
      
  if (lastSegmentMeasuresList.size ()) {
    result = lastSegmentMeasuresList.back ();
  }
  else {
    stringstream s;

    s <<
      "attempting to fetch voice last measure in an empty measures list";

    msrInternalError (
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

S_msrMeasureElement msrVoice::fetchVoiceLastElement (
  int inputLineNumber) const
{
  S_msrMeasureElement result;
  
  S_msrMeasure
    lastMeasure =
      fetchVoiceLastMeasure (inputLineNumber);

  const list<S_msrMeasureElement>&
    lastMeasureElementsList =
      lastMeasure->getMeasureElementsList ();
      
  if (lastMeasureElementsList.size ()) {
    result = lastMeasureElementsList.back ();
  }
  
  else {
    stringstream s;

    s <<
      "attempting to fetch voice last element in an empty elements list";

    msrInternalError (
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

void msrVoice::displayVoiceRepeatsStack (string context)
{
  int repeatsStackSize = fVoiceRepeatsStack.size ();
  
  gLogIOstream <<
    endl <<
    ">>++++++++++++++++ " << context <<
    endl <<
    "The repeats stack contains " <<
    singularOrPlural (repeatsStackSize, "element", "elements") <<
    ":" <<
    endl;

  if (repeatsStackSize) {  
    list<S_msrRepeat>::const_iterator
      iBegin = fVoiceRepeatsStack.begin (),
      iEnd   = fVoiceRepeatsStack.end (),
      i      = iBegin;
        
    gIndenter++;

    int n = repeatsStackSize;
    for ( ; ; ) {
      gLogIOstream <<
        "v (" << n << ")" <<
        endl;

      gIndenter++;
      (*i)->shortPrint (gLogIOstream);
      gIndenter--;

      n--;
      
      if (++i == iEnd) break;
      
      gLogIOstream <<
        endl;
    } // for
  
    gIndenter--;
  }
  
  gLogIOstream <<
    "<<++++++++++++++++ " <<
    endl <<
    endl;
}

S_msrRepeat msrVoice::createARepeatAndStackIt (
  int    inputLineNumber,
  string context)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a repeat and stacking it in voice \"" <<
      getVoiceName () <<
      "\"" <<
      " (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
  
  S_msrRepeat
    result =
      msrRepeat::create (
        inputLineNumber,
        2, // repeatTimes, default value JMI
        this);

  // push it onto the voice's repeats stack
  pushRepeatOntoRepeatsStack (
    inputLineNumber,
    result,
    "createARepeatAndStackIt()");

#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    displayVoiceRepeatsStack ("createARepeatAndStackIt()");
  }
#endif
  return result;
}

S_msrRepeat msrVoice::createARepeatCloneAndStackIt (
  int         inputLineNumber,
  S_msrRepeat repeat,
  string      context)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a repeat clone and stacking it in voice clone \"" <<
      getVoiceName () <<
      "\"" <<
      " (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
  
  S_msrRepeat
    result =
      repeat->
        createRepeatNewbornClone (this);

  // push it onto the voice's repeats stack
  pushRepeatOntoRepeatsStack (
    inputLineNumber,
    result,
    "createARepeatCloneAndStackIt()");

  return result;
}

void msrVoice::moveVoiceInitialElementsToRepeatCommonPart (
  int                   inputLineNumber,
  S_msrRepeatCommonPart repeatCommonPart,
  string                context)
{
  // move the voice initial elements to the new repeat common part
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Moving the " <<
      singularOrPlural (
        fInitialVoiceElementsList.size (),
        "initial element",
        "initial elements") <<
      " in voice \"" <<
      getVoiceName () <<
      "\" to repeat common part '" <<
      repeatCommonPart->asShortString () <<
      "' (" << context << ")" <<
      "' in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
  
  for (
    list<S_msrVoiceElement>::iterator i = fInitialVoiceElementsList.begin ();
    i != fInitialVoiceElementsList.end ();
    i++
  ) {
    S_msrVoiceElement element = (*i);

    // append the element to the new segment
    repeatCommonPart->
      appendVoiceElementToRepeatCommonPart (
        inputLineNumber,
        element,
        "moveVoiceInitialElementsToRepeatCommonPart()");

    // remove it from the voice initial elements
    i = fInitialVoiceElementsList.erase (i);
  } // for
}

void msrVoice::moveVoiceLastSegmentToRepeatCommonPart (
  int                   inputLineNumber,
  S_msrRepeatCommonPart repeatCommonPart,
  string                context)
{
  // move the voice last segment to repeatCommonPart
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Moving the voice last segment '";

    if (fVoiceLastSegment) { // JMI should not be necessary?
      gLogIOstream <<
        fVoiceLastSegment->asShortString ();
    }
    else {
      gLogIOstream <<
        "null";
    }
    
    gLogIOstream <<
      "' to repeat common part '" <<
      repeatCommonPart->asShortString () <<
      "' (" << context << ")" <<
      "' in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (fVoiceLastSegment) { // JMI should not be necessary?
    repeatCommonPart->
      appendSegmentToRepeatCommonPart (
        inputLineNumber,
        fVoiceLastSegment,
        context);

    // forget about this voice last segment
    fVoiceLastSegment = nullptr;
  }
}

void msrVoice::moveAllVoiceContentsToRepeatCommonPart (
  int                   inputLineNumber,
  S_msrRepeatCommonPart repeatCommonPart,
  string                context)
{
  moveVoiceInitialElementsToRepeatCommonPart (
    inputLineNumber,
    repeatCommonPart,
    context);
    
  moveVoiceLastSegmentToRepeatCommonPart (
    inputLineNumber,
    repeatCommonPart,
    context);
}

void msrVoice::splitMeasureIfItIsIncompleteInVoice (
  int          inputLineNumber,
  S_msrMeasure measure,
  string       context)
{
  // get the measure number
  string
    measureNumber =
      measure->getMeasureNumber ();

  // get the staff current time
  S_msrTime
    time =
      fVoiceStaffUplink->
        getStaffCurrentTime ();

#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Time for measure '" <<
      measure->asString () <<
      "' is '" <<
      time->asString () <<
      "'" <<
      endl;

    gIndenter++;
    
    measure->print (gLogIOstream);

    gIndenter++;

    gLogIOstream <<
      "upon a repeat end in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (! time) {
#ifdef TRACE_OPTIONS
    if (gGeneralOptions->fTraceRepeats) {
      gLogIOstream <<
        "*** no time signature known, no measure splitting ***" <<
        endl;
    }
#endif
  }
  
  else {
    if (
      measure->getActualMeasureWholeNotes ()
        <
      measure->getFullMeasureWholeNotes ()
    ) {
        // this measure is incomplete and should be split
  
#ifdef TRACE_OPTIONS
      if (gGeneralOptions->fTraceRepeats) {
        gLogIOstream <<
          "Splitting measure:" <<
          endl;
    
        gIndenter++;
        
        measure->
          print (gLogIOstream);
    
        gIndenter++;
    
        gLogIOstream <<
          ", (" << context << ") in voice \"" <<
          getVoiceName () <<
          "\"" <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif
      
      // create a new measure with the same number as the voice last measure
      // and append it to the voice
      S_msrMeasure
        meassureSecondPart =
          createMeasureAndAppendItToVoice (
            inputLineNumber,
            measureNumber,
            999, // measure ordinal number JMI
            msrMeasure::kMeasureImplicitNo);
  
      // set it as created after a repeat
      meassureSecondPart->
        setMeasureCreatedForARepeatKind (
          msrMeasure::kMeasureCreatedForARepeatAfter);
    }
  }
}

void msrVoice::moveVoiceLastSegmentToRepeatEnding (
  int               inputLineNumber,
  S_msrRepeatEnding repeatEnding,
  string            context)
{
  // move the voice last segment to repeatEnding
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Moving the voice last segment to repeat ending '" <<
      repeatEnding->asShortString () <<
      "' in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (fVoiceLastSegment) { // JMI should not be necessary?
    repeatEnding->
      appendSegmentToRepeatEnding (
        inputLineNumber,
        fVoiceLastSegment,
        context);

    // forget about this voice last segment
    fVoiceLastSegment = nullptr;
  }
}

void msrVoice::pushRepeatOntoRepeatsStack (
  int         inputLineNumber,
  S_msrRepeat repeat,
  string      context)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Pushing repeat '" <<
      repeat->asShortString () <<
      "' onto the repeats stack in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
                
  fVoiceRepeatsStack.push_front (repeat);

#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    displayVoiceRepeatsStack ("context");
  }
#endif
}

void msrVoice::appendRepeatToInitialVoiceElements (
  int         inputLineNumber,
  S_msrRepeat repeat,
  string      context)
{
  // append repeat to the list of initial elements
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending repeat '" <<
      repeat->asString () <<
      "' to the initial elements in voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
          
  fInitialVoiceElementsList.push_back (
    repeat);
}
    
void msrVoice::appendVoiceLastSegmentToInitialVoiceElements (
  int          inputLineNumber,
  string       context)
{
  // append segment to the list of initial elements
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending voice last segment '" <<
      fVoiceLastSegment->asString () <<
      "' to the initial elements in voice clone \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
          
  fInitialVoiceElementsList.push_back (
    fVoiceLastSegment);
}
    
void msrVoice::moveVoiceLastSegmentToInitialVoiceElements (
  int          inputLineNumber,
  string       context)
{
  // append segment to the list of initial elements
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Moving voice last segment '" <<
      fVoiceLastSegment->asString () <<
      "' to the initial elements in voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
          
  if (fVoiceLastSegment) { // JMI should not be necessary?
    fInitialVoiceElementsList.push_back (
      fVoiceLastSegment);
  
    // forget about this last segment
    fVoiceLastSegment = nullptr;
  }
}
    
void msrVoice::appendRepeatCloneToInitialVoiceElements (
  int         inputLineNumber,
  S_msrRepeat repeatCLone,
  string      context)
{
  // append repeatCLone to the list of initial elements
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending repeat cLone '" <<
      repeatCLone->asString () <<
      "' to the initial elements in voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
          
  fInitialVoiceElementsList.push_back (
    repeatCLone);
}
    
void msrVoice::popARepeatFromStack (
  int         inputLineNumber,
  S_msrRepeat repeat,
  string      context)
{

  if (repeat != fVoiceRepeatsStack.front ()) {
    stringstream s;

    s <<
      "cannot pop repeat '" <<
      repeat->asShortString () <<
      "' from the stack since it is not at the top" <<
      " (" << context << ")" <<
      ", line " << inputLineNumber;

    msrInternalError (
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
  
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Popping repeat '" <<
      repeat->asString () <<
      "' from the repeat stack in voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
          
  // pop it from repeats stack
  fVoiceRepeatsStack.pop_front ();

#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    displayVoiceRepeatsStack ("popARepeatFromStack()");
  }
#endif
}

void msrVoice::nestContentsIntoNewRepeatInVoice (
  int inputLineNumber)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      // is there a voice last segment?
      if (fVoiceLastSegment) {
        
        // are there measures in the voice last segment?
        if (fVoiceLastSegment->getSegmentMeasuresList ().size ()) {
          
          // fetch last measure's full measure whole notes
          /* JMI
          int fullMeasureWholeNotes =
            fVoiceLastSegment->
              getSegmentMeasuresList ().back ()->
                  getFullMeasureWholeNotes ();
                  */
                
          // finalize current measure in voice
          finalizeCurrentMeasureInVoice (
            inputLineNumber);
  
#ifdef TRACE_OPTIONS
          if (
            gGeneralOptions->fTraceRepeats
              ||
            gGeneralOptions->fTraceVoices
          ) {
            displayVoiceContents (
              inputLineNumber,
              "nestContentsIntoNewRepeatInVoice() 1");
          }
#endif
  
          // move voice last segment to the list of initial elements
          moveVoiceLastSegmentToInitialVoiceElements (
            inputLineNumber,
            "nestContentsIntoNewRepeatInVoice()");

  /* JMI
          // create a new last segment containing a new measure for the voice
#ifdef TRACE_OPTIONS
          if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices) {
            gLogIOstream <<
              "Creating a new last segment containing a new measure for voice \"" <<
              fVoiceName << "\"" <<
              ", line " << inputLineNumber <<
              endl;
          }
#endif
  
          createNewLastSegmentAndANewMeasureAfterARepeat (
            inputLineNumber,
            fullMeasureWholeNotes);
  */
  
#ifdef TRACE_OPTIONS
          if (
            gGeneralOptions->fTraceRepeats
              ||
            gGeneralOptions->fTraceVoicesDetails
          ) {
            displayVoiceContents (
              inputLineNumber,
              "nestContentsIntoNewRepeatInVoice() 2");
          }
#endif
        }
      }
      break;
  } // switch
}

void msrVoice::handleVoiceLevelRepeatStartInVoice (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Handling a voice-level repeat start in voice \"" <<
      getVoiceName () <<
      /* JMI
      "\" in staff \"" <<
      fVoiceStaffUplink->getStaffName () <<
      "\" in part " <<
      fVoiceStaffUplink->
        getStaffPartUplink ()->getPartCombinedName () <<
        */
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OPTIONS
  if (
    gGeneralOptions->fTraceRepeats
      ||
    gGeneralOptions->fTraceVoicesDetails
  ) {
    displayVoiceRepeatsStack ("handleVoiceLevelRepeatStartInVoice()");

    displayVoiceContents (
      inputLineNumber,
      "handleVoiceLevelRepeatStartInVoice() 1");
  }
#endif

  gIndenter++;

  // is there a voice last segment?
  if (fVoiceLastSegment) {
    
    // fetch last segment's measures list
    list<S_msrMeasure>&
    
      voiceLastSegmentMeasuresList =
        fVoiceLastSegment->
          getSegmentMeasuresListToModify ();
          
    // are there measures in the voice last segment?
    if (voiceLastSegmentMeasuresList.size ()) {
  
      // yes
      
      // fetch last measure in the last segment
      S_msrMeasure
        lastMeasureInLastSegment =
          voiceLastSegmentMeasuresList.back ();
   
      // finalize lastMeasureInLastSegment
      lastMeasureInLastSegment->
        finalizeMeasure (
          inputLineNumber);
  
      // let's look at the last measure in detail
    
      // fetch its elements list
      const list<S_msrMeasureElement>&
        lastMeasureElementsList =
          lastMeasureInLastSegment->
            getMeasureElementsList ();
    
    // JMI     if (lastMeasureElementsList.size ()) {
      switch (lastMeasureInLastSegment->getMeasureKind ()) {
        case msrMeasure::kEmptyMeasureKind:
          {
            // the last measure is empty:
            // keep it for a new voice last segment
    
#ifdef TRACE_OPTIONS
            if (
              gGeneralOptions->fTraceMeasures
                ||
              gGeneralOptions->fTraceSegments
                ||
              gGeneralOptions->fTraceRepeats
            ) {
              stringstream s;
          
              gLogIOstream <<
                "Removing empty last measure '" <<
                lastMeasureInLastSegment->getMeasureNumber () <<
                "' from last segment '" <<
                asString () <<
                "' in voice \"" <<
                getVoiceName () <<
                "\"";
            }
#endif
            fVoiceLastSegment->
              removeLastMeasureFromSegment (
                inputLineNumber);
    
            // append last segment to initial voice elements list
            appendVoiceLastSegmentToInitialVoiceElements (
              inputLineNumber,
              "handleVoiceLevelRepeatStartInVoice()");
    
            // create a new last segment containing a new measure for the voice
#ifdef TRACE_OPTIONS
            if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices) {
              gLogIOstream <<
                "Creating a new last segment with the empty measure '" <<
                lastMeasureInLastSegment->asShortString () <<
                "' in voice \"" <<
                fVoiceName << "\"" <<
                ", line " << inputLineNumber <<
                endl;
            }
#endif
    
            createNewLastSegmentFromFirstMeasureForVoice (
              inputLineNumber,
              lastMeasureInLastSegment,
              "handleVoiceLevelRepeatStartInVoice()");
          }
          break;
    
        default:
          {
            // the last measure it not empty:
    
            // split lastMeasureInLastSegment if it is incomplete
            splitMeasureIfItIsIncompleteInVoice (
              inputLineNumber,
              lastMeasureInLastSegment,
              "handleVoiceLevelRepeatStartInVoice()");
    
            // append last segment to initial voice elements list
            appendVoiceLastSegmentToInitialVoiceElements (
              inputLineNumber,
              "handleVoiceLevelRepeatStartInVoice()");
    
            // create a new last segment for the voice
#ifdef TRACE_OPTIONS
            if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices) {
              gLogIOstream <<
                "Creating a new last segment for voice \"" <<
                fVoiceName << "\"" <<
                ", line " << inputLineNumber <<
                endl;
            }
#endif
    
            createNewLastSegmentForVoice (
              inputLineNumber,
              "handleVoiceLevelRepeatStartInVoice()");
          }
          break;
      } // switch
    }
  }
  
  else {
    // no JMI ???
  }
      
  // create the repeat and stack it
  S_msrRepeat
    newRepeat =
      createARepeatAndStackIt (
        inputLineNumber,
        "handleVoiceLevelRepeatStartInVoice()");

  // set it as having an explicit start
  newRepeat->
    setRepeatExplicitStartKind (
      msrRepeat::kRepeatExplicitStartYes);
  
#ifdef TRACE_OPTIONS
  if (
    gGeneralOptions->fTraceRepeats
      ||
    gGeneralOptions->fTraceVoicesDetails
  ) {
    displayVoiceContents (
      inputLineNumber,
      "handleVoiceLevelRepeatStartInVoice() 2");
  }
#endif

  gIndenter--;
}

void msrVoice::handleNestedRepeatStartInVoice (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Handling a nested repeat start in voice \"" <<
      getVoiceName () <<
      "\" in staff \"" <<
      fVoiceStaffUplink->getStaffName () <<
      "\" in part " <<
      fVoiceStaffUplink->
        getStaffPartUplink ()->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;

    displayVoiceRepeatsStack ("handleNestedRepeatStartInVoice()");
    
    displayVoiceContents (
      inputLineNumber,
      "handleNestedRepeatStartInVoice()");
  }
#endif
}

void msrVoice::handleRepeatStartInVoice (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Handling repeat start in voice \"" <<
      getVoiceName () <<
      "\" in staff \"" <<
      fVoiceStaffUplink->getStaffName () <<
      "\" in part " <<
      fVoiceStaffUplink->
        getStaffPartUplink ()->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;

    displayVoiceRepeatsStack ("handleRepeatStartInVoice()");
    
    displayVoiceContents (
      inputLineNumber,
      "handleRepeatStartInVoice()");
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      // analyze this repeat start's context
      switch (fVoiceRepeatsStack.size ()) {
        case 0:
          // this repeat start is at the voice-level
          // -------------------------------------
          handleVoiceLevelRepeatStartInVoice (
            inputLineNumber);
          break;

        default:
          // this repeat start belongs to a nested repeat
          // ---------------------------------------
          handleNestedRepeatStartInVoice (
            inputLineNumber);
      } // switch
      break;
  } // switch
}

void msrVoice::handleVoiceLevelRepeatEndWithImplicitStartInVoice (
  int    inputLineNumber,
  string measureNumber,
  int    repeatTimes)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Handling a voice-level repeat end with implicit start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;

    displayVoiceRepeatsStack ("handleVoiceLevelRepeatEndWithImplicitStartInVoice()");
    
    displayVoiceContents (
      inputLineNumber,
      "handleVoiceLevelRepeatEndWithImplicitStartInVoice()");
  }
#endif

  gIndenter++;
  
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "This repeat end  without a start is at the voice-level" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create the repeat
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a repeat upon its end in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
  
  int repeatInputLineNumber = 1; // could find first measure's input line number??? JMI

  S_msrRepeat
    newRepeat =
      msrRepeat::create (
        repeatInputLineNumber,
        repeatTimes,
        this);

  // create the repeat common part
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a repeat common part upon its end in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  S_msrRepeatCommonPart
    repeatCommonPart =
      msrRepeatCommonPart::create (
        repeatInputLineNumber,
        newRepeat);

  // register it in newRepeat
  newRepeat->
    setRepeatCommonPart (
      repeatCommonPart);
      
  // fetch the last segment's last measure
  S_msrMeasure
    previousLastSegmentLastMeasure =
      fVoiceLastSegment->
        fetchLastMeasureFromSegment (
          inputLineNumber);

  // split previousLastSegmentLastMeasure if it is incomplete
  splitMeasureIfItIsIncompleteInVoice (
    inputLineNumber,
    previousLastSegmentLastMeasure,
    "handleVoiceLevelRepeatEndWithImplicitStartInVoice()");

  // append the voice last segment to the new repeat common part
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending the voice last segment in voice \"" <<
      getVoiceName () <<
      "\" to the new voice-level repeat common part" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  repeatCommonPart->
    appendSegmentToRepeatCommonPart (
      inputLineNumber,
      fVoiceLastSegment,
      "handleVoiceLevelRepeatEndWithImplicitStartInVoice()");

  // create a new last segment for the voice
  createNewLastSegmentForVoice (
    inputLineNumber,
    "handleVoiceLevelRepeatEndWithImplicitStartInVoice()");
  
  // set newRepeat's build phase to completed
  newRepeat->
    setCurrentRepeatBuildPhaseKind (
      msrRepeat::kRepeatBuildPhaseCompleted);

  // append newRepeat to the list of initial elements
  appendRepeatToInitialVoiceElements (
    inputLineNumber,
    newRepeat,
    "handleVoiceLevelRepeatEndWithImplicitStartInVoice()");

  gIndenter--;
}

void msrVoice::handleVoiceLevelRepeatEndWithExplicitStartInVoice (
  int    inputLineNumber,
  string measureNumber,
  int    repeatTimes)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Handling a voice-level repeat end without implicit start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;

    displayVoiceRepeatsStack ("handleVoiceLevelRepeatEndWithExplicitStartInVoice()");
    
    displayVoiceContents (
      inputLineNumber,
      "handleVoiceLevelRepeatEndWithExplicitStartInVoice()");
  }
#endif

#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "This repeat end  with a start is at the voice-level" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // fetch the voice's last measure
  S_msrMeasure
    voiceLastMeasure =
      fetchVoiceLastMeasure (
        inputLineNumber);

  // split previousLastSegmentLastMeasure if it is incomplete
  splitMeasureIfItIsIncompleteInVoice (
    inputLineNumber,
    voiceLastMeasure,
    "handleVoiceLevelRepeatEndWithExplicitStartInVoice()");

  // grab current repeat
  S_msrRepeat
    currentRepeat =
      fVoiceRepeatsStack.front ();

  // set its repeat times field
  currentRepeat->
    setRepeatTimes (repeatTimes);

/* JMI
  // get repeat's common part
  S_msrRepeatCommonPart
    repeatCommonPart =
      currentRepeat->
        getRepeatCommonPart ();
        */
  // create the currentRepeat's common part
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a repeat common part upon repeat end in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  S_msrRepeatCommonPart
    repeatCommonPart =
      msrRepeatCommonPart::create (
        inputLineNumber,
        currentRepeat);

  // register it in newRepeat
  currentRepeat->
    setRepeatCommonPart (
      repeatCommonPart);
      
  // move the voice last segment to the new repeat common part
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Moving the voice last segment in voice \"" <<
      getVoiceName () <<
      "\" to the current voice-level repeat common part" <<
      " and forget about it" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  moveVoiceLastSegmentToRepeatCommonPart (
    inputLineNumber,
    repeatCommonPart,
    "handleVoiceLevelRepeatEndWithExplicitStartInVoice()");

  // create a new last segment to collect the remainder of the voice,
  createNewLastSegmentForVoice ( // JMI
    inputLineNumber,
    "handleVoiceLevelRepeatEndWithExplicitStartInVoice()");

  // set currentRepeat's build phase to completed
  currentRepeat->
    setCurrentRepeatBuildPhaseKind (
      msrRepeat::kRepeatBuildPhaseCompleted);

  // append currentRepeat to the list of initial elements
  appendRepeatToInitialVoiceElements (
    inputLineNumber,
    currentRepeat,
    "handleVoiceLevelRepeatEndWithExplicitStartInVoice()");

  gIndenter--;
}

void msrVoice::handleNestedRepeatEndInVoice (
  int    inputLineNumber,
  string measureNumber,
  int    repeatTimes)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Handling a nested repeat end in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;

    displayVoiceRepeatsStack ("handleNestedRepeatEndInVoice()");
  }
#endif

#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "This repeat is nested" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

// fetch the voice's last measure
  S_msrMeasure
    voiceLastMeasure =
      fetchVoiceLastMeasure (
        inputLineNumber);
  
  // is there a measure splitting?
  if (
    voiceLastMeasure->getActualMeasureWholeNotes ()
      ==
    voiceLastMeasure->getFullMeasureWholeNotes ()
  ) {
    // this measure is incomplete and should be split
#ifdef TRACE_OPTIONS
    if (gGeneralOptions->fTraceRepeats) {
      gLogIOstream <<
        "Splitting measure '" <<
        voiceLastMeasure->asShortString () <<
        "' upon a repeat end in voice \"" <<
        getVoiceName () <<
        "\"" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif
  
    // create a new measure with the same number as the voice last measure
    // and append it to the voice,
    createMeasureAndAppendItToVoice (
      inputLineNumber,
      measureNumber,
      997, // measure ordinal number JMI
      msrMeasure::kMeasureImplicitNo);
  
    // set it as created after a repeat
    voiceLastMeasure->
      setMeasureCreatedForARepeatKind (
        msrMeasure::kMeasureCreatedForARepeatAfter);
  }

#ifdef TRACE_OPTIONS
  if (
    gGeneralOptions->fTraceRepeats
      ||
    gGeneralOptions->fTraceVoicesDetails
  ) {
    displayVoiceContents (
      inputLineNumber,
      "handleNestedRepeatEndInVoice() 2");
  }
#endif
}

void msrVoice::handleRepeatEndInVoice (
  int    inputLineNumber,
  string measureNumber,
  int    repeatTimes)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Handling a repeat end in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;

    displayVoiceRepeatsStack ("handleRepeatEndInVoice()");
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
        // finalize current measure in voice
        finalizeCurrentMeasureInVoice (
          inputLineNumber);
          
#ifdef TRACE_OPTIONS
        if (
          gGeneralOptions->fTraceRepeats
            ||
          gGeneralOptions->fTraceVoicesDetails
        ) {
          displayVoiceContents (
            inputLineNumber,
            "handleRepeatEndInVoice() 1");
        }
#endif

        // analyze this repeat end's context
        switch (fVoiceRepeatsStack.size ()) {
          case 0:
            // this repeat is at the voice-level and has an implicit start
            // -------------------------------------
            handleVoiceLevelRepeatEndWithImplicitStartInVoice (
              inputLineNumber,
              measureNumber,
              repeatTimes);
            break;

          case 1:
            // this repeat is at the voice-level and has an explicit start
            // -------------------------------------
            handleVoiceLevelRepeatEndWithExplicitStartInVoice (
              inputLineNumber,
              measureNumber,
              repeatTimes);
            break;

          default:
            // newRepeat is a nested repeat
            // ---------------------------------------
            handleNestedRepeatEndInVoice (
              inputLineNumber,
              measureNumber,
              repeatTimes);
        } // switch
      }
      break;
  } // switch
}

void msrVoice::handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Handling a voice-level repeat ending start without explicit start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OPTIONS
  if (
    gGeneralOptions->fTraceRepeats
      ||
    gGeneralOptions->fTraceVoicesDetails
  ) {
    displayVoiceRepeatsStack ("handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice() 1");

    displayVoiceContents (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice() 1");
  }
#endif

  gIndenter++;

  // create the repeat and stack it
  stringstream s1;
  
  s1 <<
    "Creating a voice-level repeat upon its first ending in voice \"" <<
    getVoiceName () <<
    "\"" <<
    ", line " << inputLineNumber;

  int repeatInputLineNumber = 1; // could find first measure's input line number??? JMI

  S_msrRepeat
    newRepeat =
      createARepeatAndStackIt (
        repeatInputLineNumber,
        s1.str ());

  // create the repeat common part
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a repeat common part upon its end in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  S_msrRepeatCommonPart
    repeatCommonPart =
      msrRepeatCommonPart::create (
        repeatInputLineNumber,
        newRepeat);

  // register it in newRepeat
  newRepeat->
    setRepeatCommonPart (
      repeatCommonPart);

  // fetch last measure
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Fetching the last measure in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
        
  S_msrMeasure
    lastMeasure =
      fVoiceLastSegment->
        fetchLastMeasureFromSegment (
          inputLineNumber);

  // fetch lastMeasure's elements list
  const list<S_msrMeasureElement>&
    lastMeasureElementsList =
      lastMeasure->
        getMeasureElementsList ();

  // let's look at lastMeasure in detail
  if (! lastMeasureElementsList.size ()) {
    
    // it is empty, keep it for a new voice last segment

    // remove last measure
#ifdef TRACE_OPTIONS
    if (
      gGeneralOptions->fTraceMeasures
        ||
      gGeneralOptions->fTraceSegments
        ||
      gGeneralOptions->fTraceRepeats
        ||
      gGeneralOptions->fTraceVoices
    ) {
      gLogIOstream <<
        "Removing the last measure in voice \"" <<
        getVoiceName () <<
        "\"" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    S_msrMeasure
      dummyMeasure =
        fVoiceLastSegment->
          removeLastMeasureFromSegment (
            inputLineNumber);
          
    // move voice last segment into the repeat common part
    stringstream s2;
  
    s2 <<
      "Moving the voice last segment to the repeat common part in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber;
    
    moveVoiceLastSegmentToRepeatCommonPart (
      inputLineNumber,
      repeatCommonPart,
      s2.str ());
        
    // create a new last segment containing a new measure for the voice
#ifdef TRACE_OPTIONS
    if (
      gGeneralOptions->fTraceMeasures
        ||
      gGeneralOptions->fTraceSegments
        ||
      gGeneralOptions->fTraceRepeats
        ||
      gGeneralOptions->fTraceVoices
    ) {
      gLogIOstream <<
        "Creating a new last segment with the first ending measure for voice \"" <<
        fVoiceName << "\"" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif
    
    createNewLastSegmentFromFirstMeasureForVoice (
      inputLineNumber,
      lastMeasure,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice()");
  }
    
  else {
    
    // it is not empty
    
    // split lastMeasureInLastSegment if it is incomplete
    splitMeasureIfItIsIncompleteInVoice (
      inputLineNumber,
      lastMeasure,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice()");

    // append last segment to initial voice elements list
    appendVoiceLastSegmentToInitialVoiceElements (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice()");

    // create a new last segment for the voice
#ifdef TRACE_OPTIONS
    if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices) {
      gLogIOstream <<
        "Creating a new last segment for voice \"" <<
        fVoiceName << "\"" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    createNewLastSegmentForVoice (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice()");
  }

#ifdef TRACE_OPTIONS
  if (
    gGeneralOptions->fTraceRepeats
      ||
    gGeneralOptions->fTraceVoicesDetails
  ) {
    displayVoiceRepeatsStack ("handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice() 2");

    displayVoiceContents (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice() 2");
  }
#endif

  gIndenter--;
}

void msrVoice::handleVoiceLevelRepeatEndingStartWithExplicitStartInVoice (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Handling a voice-level repeat ending start with explicit start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OPTIONS
  if (
    gGeneralOptions->fTraceRepeats
      ||
    gGeneralOptions->fTraceVoicesDetails
  ) {
    displayVoiceRepeatsStack ("handleVoiceLevelRepeatEndingStartWithExplicitStartInVoice() 1");

    displayVoiceContents (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithExplicitStartInVoice() 1");
  }
#endif
  
  gIndenter++;

  // fetch currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoiceRepeatsStack.front ();
      
  // get currentRepeat's common part
  S_msrRepeatCommonPart
    repeatCommonPart =
      currentRepeat->
        getRepeatCommonPart ();
      
  // fetch last measure of last segment
  S_msrMeasure
    lastMeasure =
      fVoiceLastSegment->
        fetchLastMeasureFromSegment (
          inputLineNumber);

#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Fetching the last measure of the last segment in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", it is:" <<
      endl <<
      lastMeasure <<
      endl;
  }
#endif
        
  // fetch lastMeasure's elements list
  const list<S_msrMeasureElement>&
    lastMeasureElementsList =
      lastMeasure->
        getMeasureElementsList ();

  // let's look at lastMeasure in detail
  if (! lastMeasureElementsList.size ()) {
    
    // it is empty, keep it for a new voice last segment

    // remove last measure
#ifdef TRACE_OPTIONS
    if (
      gGeneralOptions->fTraceMeasures
        ||
      gGeneralOptions->fTraceSegments
        ||
      gGeneralOptions->fTraceRepeats
        ||
      gGeneralOptions->fTraceVoices
    ) {
      gLogIOstream <<
        "Removing the last measure in voice \"" <<
        getVoiceName () <<
        "\"" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    S_msrMeasure
      dummyMeasure =
        fVoiceLastSegment->
          removeLastMeasureFromSegment (
            inputLineNumber);
          
    // move voice last segment into the repeat common part
    stringstream s2;
  
    s2 <<
      "Moving the voice last segment to the repeat common part in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber;
    
    moveVoiceLastSegmentToRepeatCommonPart (
      inputLineNumber,
      repeatCommonPart,
      s2.str ());
        
    // create a new last segment containing a new measure for the voice
#ifdef TRACE_OPTIONS
    if (
      gGeneralOptions->fTraceMeasures
        ||
      gGeneralOptions->fTraceSegments
        ||
      gGeneralOptions->fTraceRepeats
        ||
      gGeneralOptions->fTraceVoices
    ) {
      gLogIOstream <<
        "Creating a new last segment with the first ending measure for voice \"" <<
        fVoiceName << "\"" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif
    
    createNewLastSegmentFromFirstMeasureForVoice (
      inputLineNumber,
      lastMeasure,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice()");
  }
    
  else {
    
    // it is not empty
    
    // split lastMeasureInLastSegment if it is incomplete
    splitMeasureIfItIsIncompleteInVoice (
      inputLineNumber,
      lastMeasure,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice()");

    // append last segment to initial voice elements list
    appendVoiceLastSegmentToInitialVoiceElements (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice()");

    // create a new last segment for the voice
#ifdef TRACE_OPTIONS
    if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices) {
      gLogIOstream <<
        "Creating a new last segment for voice \"" <<
        fVoiceName << "\"" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    createNewLastSegmentForVoice (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice()");
  }

#ifdef TRACE_OPTIONS
  if (
    gGeneralOptions->fTraceRepeats
      ||
    gGeneralOptions->fTraceVoicesDetails
  ) {
    displayVoiceRepeatsStack ("handleVoiceLevelRepeatEndingStartWithExplicitStartInVoice() 2");

    displayVoiceContents (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithExplicitStartInVoice() 2");
  }
#endif

  gIndenter--;
}

void msrVoice::handleNestedRepeatEndingStartInVoice (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Handling a nested repeat ending start voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;

    displayVoiceRepeatsStack ("handleNestedRepeatEndingStartInVoice() 1");
  }
#endif
}

void msrVoice::handleRepeatEndingStartInVoice (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Handling a repeat ending start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
#ifdef TRACE_OPTIONS
        if (
          gGeneralOptions->fTraceRepeats
            ||
          gGeneralOptions->fTraceVoicesDetails
        ) {

          displayVoiceRepeatsStack ("handleRepeatEndingStartInVoice() 1");

          displayVoiceContents (
            inputLineNumber,
            "handleRepeatEndingStartInVoice() 1");
        }
#endif
          
        gIndenter++;

        // analyze this repeat end's context
        switch (fVoiceRepeatsStack.size ()) {
          case 0:
            // this the first ending of a voice-level repeat without start
            // -------------------------------------
            handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice (
              inputLineNumber);
            break;

          case 1:
            {
              // fetch currentRepeat
              S_msrRepeat
                currentRepeat =
                  fVoiceRepeatsStack.front ();
                  
              switch (currentRepeat->getRepeatExplicitStartKind ()) {
                case msrRepeat::kRepeatExplicitStartNo:
                  // this the first ending of a voice-level repeat without a start
                  // -------------------------------------
                  handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice (
                    inputLineNumber);
                  break;
                case msrRepeat::kRepeatExplicitStartYes:
                  // this the first ending of a voice-level repeat with a start
                  // -------------------------------------
                  handleVoiceLevelRepeatEndingStartWithExplicitStartInVoice (
                    inputLineNumber);
                  break;
              } // switch
            }
            break;

          default:
            // newRepeat is a nested repeat
            // ---------------------------------------
            handleNestedRepeatEndingStartInVoice (
              inputLineNumber);
        } // switch
        
#ifdef TRACE_OPTIONS
        if (
          gGeneralOptions->fTraceRepeats
            ||
          gGeneralOptions->fTraceVoicesDetails
        ) {
          displayVoiceContents (
            inputLineNumber,
            "handleRepeatEndingStartInVoice() 2");
        }
#endif

        gIndenter--;
      }
      break;
  } // switch
}

void msrVoice::handleRepeatEndingStartInVoiceClone (
  int       inputLineNumber,
  msrRepeatEnding::msrRepeatEndingKind
            repeatEndingKind,
  string    repeatEndingNumber) // may be "1, 2"
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Handling a repeat ending start voice clone \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;

    displayVoiceRepeatsStack ("handleRepeatEndingStartInVoiceClone() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
#ifdef TRACE_OPTIONS
        if (
          gGeneralOptions->fTraceRepeats
            ||
          gGeneralOptions->fTraceVoicesDetails
        ) {
          displayVoiceContents (
            inputLineNumber,
            "handleRepeatEndingStartInVoiceClone() 1");
        }
#endif
         
        // handle the repeat ending start
        gIndenter++;

#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceRepeats) {
          gLogIOstream <<
            "Handling a repeat ending upon its start in voice clone \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
        
        // is there a current repeat?
        switch (fVoiceRepeatsStack.size ()) {
          case 0:
            {
              stringstream s;
        
              s <<
                "repeats stack is empty when attempting to handle a repeat ending start in voice clone '" <<
                asShortString () <<
                "' ";
                
              msrInternalError (
                gGeneralOptions->fInputSourceName,
                fInputLineNumber,
                __FILE__, __LINE__,
                s.str ());
            }
          break;
  
          case 1:
            {
              // this repeat ending is part of a voice-level repeat

              // fetch currentRepeat
              S_msrRepeat
                currentRepeat =
                  fVoiceRepeatsStack.front ();
                  
              // create a repeat ending
#ifdef TRACE_OPTIONS
              if (gGeneralOptions->fTraceRepeats) {
                gLogIOstream <<
                  "Creating a " <<
                  msrRepeatEnding::repeatEndingKindAsString (
                    repeatEndingKind) <<
                  " repeat ending in current repeat in voice clone \"" <<
                  fVoiceName <<
                  "\"" <<
                  endl;
              }
#endif
                
              S_msrRepeatEnding
                repeatEnding =
                  msrRepeatEnding::create (
                    inputLineNumber,
                    repeatEndingNumber,
                    repeatEndingKind,
                    currentRepeat);
            }
          break;
          
          default:
            {
              // the current repeat is nested
              
              // JMI ???

              // move voice last segment into the repeat common part
#ifdef TRACE_OPTIONS
              if (gGeneralOptions->fTraceRepeats) {
                gLogIOstream <<
                  "Moving the voice last segment to the repeat common part in voice clone \"" <<
                  getVoiceName () <<
                  "\"" <<
                  ", line " << inputLineNumber <<
                  endl;
              }
#endif

            /* JMI
              moveVoiceLastSegmentToRepeatCommonPart (
                inputLineNumber,
                fVoiceRepeatsStack.front ()->getRepeatCommonPart (),
                "handleRepeatEndingStartInVoiceClone()");
      */
      
 /* JMI
              // append newRepeat to the list of initial elements
              appendRepeatCloneToInitialVoiceElements (
                inputLineNumber,
                newRepeat,
                "handleRepeatEndInVoice()");
            
             // push newRepeat onto the voice's repeats stack
         pushRepeatOntoRepeatsStack (
          inputLineNumber,
          newRepeat,
          "handleRepeatEndInVoice()");
      
      */

            }
        } // switch
        
#ifdef TRACE_OPTIONS
        if (
          gGeneralOptions->fTraceRepeats
            ||
          gGeneralOptions->fTraceVoicesDetails
        ) {
          displayVoiceContents (
            inputLineNumber,
            "handleRepeatEndingStartInVoiceClone() 2");
        }
#endif

        gIndenter--;
      }
      break;
  } // switch
}

void msrVoice::finalizeRepeatEndInVoice (
  int    inputLineNumber,
  string measureNumber,
  int    repeatTimes)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Finalizing a repeat upon its end in voice \"" <<
      getVoiceName () <<
      "\" in part " <<
      fVoiceStaffUplink->
        getStaffPartUplink ()->
          getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;

  displayVoiceRepeatsStack ("finalizeRepeatEndInVoice()");
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
        // finalize current measure in voice
        finalizeCurrentMeasureInVoice (
          inputLineNumber);
          
#ifdef TRACE_OPTIONS
        if (
          gGeneralOptions->fTraceRepeats
            ||
          gGeneralOptions->fTraceVoicesDetails
        ) {
          displayVoiceContents (
            inputLineNumber,
            "finalizeRepeatEndInVoice() 1");
        }
#endif

        // fetch the repeat
        if (! fVoiceRepeatsStack.size ()) {
          stringstream s;
      
          s <<
            "repeats stack is empty when attempting to finalize a repeat in voice '" <<
            asShortString () <<
            "' ";
            
          msrInternalError (
            gGeneralOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
        
        S_msrRepeat
          repeat =
            fVoiceRepeatsStack.front ();

        // get repeat's common part
        S_msrRepeatCommonPart
          repeatCommonPart =
            repeat->
              getRepeatCommonPart ();

        // is there another repeat to nest into this repeat?
        if (! fVoiceRepeatsStack.size ()) {
          // yes, this repeat contains a nested repeat

/* JMI
          // move all the voice contents to the new repeat common part
          moveAllVoiceContentsToRepeatCommonPart (
            inputLineNumber,
            repeatCommonPart,
            "finalize repeat");
          // move the voice initial elements to the new repeat common part
        */
        }
        
        else {
          // no, this repeat is at the voice-level

/* JMI
          // move all the voice contents to the new repeat common part
          moveAllVoiceContentsToRepeatCommonPart ( // JMI
            inputLineNumber,
            repeatCommonPart,
            "finalizing voice-level repeat");
            */
        }

#ifdef TRACE_OPTIONS
        if (
          gGeneralOptions->fTraceRepeats
            ||
          gGeneralOptions->fTraceVoicesDetails
        ) {
          displayVoiceContents (
            inputLineNumber,
            "finalizeRepeatEndInVoice() 2");
        }
#endif

      }
      break;
  } // switch
}

void msrVoice::createMeasuresRepeatFromItsFirstMeasuresInVoice (
  int inputLineNumber,
  int measuresRepeatMeasuresNumber,
  int measuresRepeatSlashes)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a measures repeat from its first measure in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;

 // JMI   displayVoiceRepeatsStack ("createMeasuresRepeatFromItsFirstMeasuresInVoice()");
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
        // create a measure repeat
#ifdef TRACE_OPTIONS
        if (
          gGeneralOptions->fTraceMeasures
            ||
          gGeneralOptions->fTraceRepeats
            ||
          gGeneralOptions->fTraceVoices
          ) {
          gLogIOstream <<
            "Creating a " <<
            measuresRepeatMeasuresNumber <<
            " measure, " <<
            measuresRepeatSlashes <<
            " slashes repeat from it's first measure in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
      
        // print current voice contents
#ifdef TRACE_OPTIONS
        if (
          gGeneralOptions->fTraceMeasures
            ||
          gGeneralOptions->fTraceRepeats
            ||
          gGeneralOptions->fTraceSegments
            ||
          gGeneralOptions->fTraceVoices
          ) {
          gLogIOstream <<
            endl <<
            "The current contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;

          gIndenter++;
          print (gLogIOstream);
          gIndenter--;
        }
#endif
  
        // this occurs after an empty measure has just been created,
        // hence the repeated measure/measures is/are the
        // measuresRepeatMeasuresNumber measures preceding the last one

        int
          lastSegmentMeasuresNumber =
            fVoiceLastSegment->
              getSegmentMeasuresList ().size (),
          availableMeasuresNumber =
            lastSegmentMeasuresNumber - 1;
            
        if (
          availableMeasuresNumber < measuresRepeatMeasuresNumber) {
          stringstream s;

          s <<
            "attempting to create a measure repeat with " <<
            measuresRepeatMeasuresNumber <<
            " measures while current last segment only has " <<
            availableMeasuresNumber <<
            " available";

          msrInternalError (
            gGeneralOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

        // grab the just created last measure from the voice,
        // (i.e. the one containing:
        //   <measure-repeat ... type="start">2</measure-repeat>)
        // which is the first replica measure
        S_msrMeasure
          firstReplicaMeasure =
            removeLastMeasureFromVoice (
              inputLineNumber);

        // create the measure repeat repeated segment
        S_msrSegment
          repeatedSegment =
            msrSegment::create (
              inputLineNumber,
              this);

        if (! fVoiceFirstSegment) {
          fVoiceFirstSegment = fVoiceLastSegment;
        }

        // remove the repeated measure(s) for the last segment
        // and prepend them to the repeated segment
#ifdef TRACE_OPTIONS
        if (
          gGeneralOptions->fTraceMeasures
            ||
          gGeneralOptions->fTraceRepeats
            ||
          gGeneralOptions->fTraceSegments
            ||
          gGeneralOptions->fTraceVoices
          ) {
          gLogIOstream <<
            "Removing the last " <<
            measuresRepeatMeasuresNumber <<
            " measures (to be repeated) from voice \"" <<
            fVoiceName <<
            endl;
        }
#endif

        for (int i = 0; i< measuresRepeatMeasuresNumber; i++) {
          S_msrMeasure
            lastMeasure =
              removeLastMeasureFromVoice (
                inputLineNumber);

          repeatedSegment->
            prependMeasureToSegment (
              lastMeasure);
        } // for

        // create the measure repeat
        if (fVoiceCurrentMeasuresRepeat) {
          stringstream s;

          s <<
            "attempting to create a measure repeat while another one is pending";

          msrInternalError (
            gGeneralOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
        
        fVoiceCurrentMeasuresRepeat =
          msrMeasuresRepeat::create (
            inputLineNumber,
            measuresRepeatMeasuresNumber,
            measuresRepeatSlashes,
            this);

        // create the measures repeat pattern
#ifdef TRACE_OPTIONS
        if (
          gGeneralOptions->fTraceMeasures
            ||
          gGeneralOptions->fTraceRepeats
            ||
          gGeneralOptions->fTraceSegments
            ||
          gGeneralOptions->fTraceVoices
          ) {
          gLogIOstream <<
            "Creating a measure repeat pattern for voice \"" <<
            fVoiceName << "\" is:" <<
            endl;
        }
#endif
        
        S_msrMeasuresRepeatPattern
          measuresRepeatPattern =
            msrMeasuresRepeatPattern::create (
              inputLineNumber,
              fVoiceCurrentMeasuresRepeat);

        // set the repeated segment as the measure repeat pattern segment
        measuresRepeatPattern->
          setMeasuresRepeatPatternSegment (
            repeatedSegment);
            
        // set the measure repeat pattern
        fVoiceCurrentMeasuresRepeat->
          setMeasuresRepeatPattern (
            measuresRepeatPattern);
        
        // create a new last segment to collect the measure repeat replicas,
        // containing the first, yet incomplete, replica
#ifdef TRACE_OPTIONS
        if (
          gGeneralOptions->fTraceMeasures
            ||
          gGeneralOptions->fTraceRepeats
            ||
          gGeneralOptions->fTraceSegments
            ||
          gGeneralOptions->fTraceVoices
          ) {
          gLogIOstream <<
            "Creating a new last segment with the first replica measure for voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
            
        createNewLastSegmentFromFirstMeasureForVoice (
          inputLineNumber,
          firstReplicaMeasure,
          "()");

        // print resulting voice contents
#ifdef TRACE_OPTIONS
        if (
          gGeneralOptions->fTraceMeasures
            ||
          gGeneralOptions->fTraceRepeats
            ||
          gGeneralOptions->fTraceSegments
            ||
          gGeneralOptions->fTraceVoices
          ) {
          gLogIOstream <<
            "The contents of voice \"" <<
            fVoiceName << "\" after createMeasuresRepeatFromItsFirstMeasuresInVoice () is:" <<
            endl;

          gIndenter++;
          print (gLogIOstream);
          gIndenter--;
        }
#endif

        // keep the measure repeat pending
      }
      break;
  } // switch
}

void msrVoice::appendPendingMeasuresRepeatToVoice (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending pending measures repeat to voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;

 // JMI   displayVoiceRepeatsStack ("appendPendingMeasuresRepeatToVoice()");
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
        // does the current measures repeat exist?
        if (! fVoiceCurrentMeasuresRepeat) {
          stringstream s;

          s <<
            "attempting to append a pending measure repeat which doesn't exist";

          msrInternalError (
            gGeneralOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

        // fetch the last segment's measure list
        list<S_msrMeasure>&
          voiceLastSegmentMeasureList =
            fVoiceLastSegment->
              getSegmentMeasuresListToModify ();
       
        // grab the just created last measure in the last segment's measure list,
        // (i.e. the one containing:
        //   <measure-repeat type="stop"/>)
        // which is the next measure after the measure repeat
        if (! voiceLastSegmentMeasureList.size ()) {
          stringstream s;

          s <<
            "attempting to grab first measure of voice last segment, that contains none";

          msrInternalError (
            gGeneralOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

        S_msrMeasure
          nextMeasureAfterMeasuresRepeat =
            voiceLastSegmentMeasureList.back ();

// BOFBOFBOF JMI
        // remove the next measure from the last segment's measure list
#ifdef TRACE_OPTIONS
        if (
          gGeneralOptions->fTraceMeasures
            ||
          gGeneralOptions->fTraceRepeats
            ||
          gGeneralOptions->fTraceSegments
            ||
          gGeneralOptions->fTraceVoices
          ) {
          gLogIOstream <<
            "Removing last measure in last segment in voice \"" <<
            fVoiceName << "\" is:" <<
            endl;
        }
#endif

        voiceLastSegmentMeasureList.pop_back ();

        // create the measure repeat replicas contents
#ifdef TRACE_OPTIONS
        if (
          gGeneralOptions->fTraceMeasures
            ||
          gGeneralOptions->fTraceRepeats
            ||
          gGeneralOptions->fTraceSegments
            ||
          gGeneralOptions->fTraceVoices
          ) {
          gLogIOstream <<
            "Creating a measure repeat replicas FAA contents for voice \"" <<
            fVoiceName << "\" is:" <<
            endl;
        }
#endif

        S_msrMeasuresRepeatReplicas
          measuresRepeatReplicas =
            msrMeasuresRepeatReplicas::create (
              inputLineNumber,
              fVoiceCurrentMeasuresRepeat);

        // set the voice last segment as the measure repeat replicas segment
        measuresRepeatReplicas->
          setMeasuresRepeatReplicasSegment (
            fVoiceLastSegment);

        // forget about this voice last segment
        fVoiceLastSegment = nullptr; // JMI

        fVoiceCurrentMeasuresRepeat->
          setMeasuresRepeatReplicas (
            measuresRepeatReplicas);

#ifdef TRACE_OPTIONS
        if (
          gGeneralOptions->fTraceMeasures
            ||
          gGeneralOptions->fTraceRepeats
            ||
          gGeneralOptions->fTraceSegments
            ||
          gGeneralOptions->fTraceVoices
          ) {
          gLogIOstream <<
            "Setting pending measure repeat replicas segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
#endif
        
        fVoiceCurrentMeasuresRepeat->
          setMeasuresRepeatReplicas (
            measuresRepeatReplicas);

#ifdef TRACE_OPTIONS
        if (
          gGeneralOptions->fTraceMeasures
            ||
          gGeneralOptions->fTraceRepeats
            ||
          gGeneralOptions->fTraceSegments
            ||
          gGeneralOptions->fTraceVoices
          ) {
          gLogIOstream <<
            "Setting measures repeat segment to voice last segment CCC for voice \"" <<
            fVoiceName << "\"" <<
            endl;
        }
#endif

        fVoiceCurrentMeasuresRepeat->
          getMeasuresRepeatReplicas ()->
            setMeasuresRepeatReplicasSegment (
              fVoiceLastSegment);

        // forget about this voice last segment
        fVoiceLastSegment = nullptr; // JMI

        // append pending measure repeat to the last segment
        fVoiceLastSegment->appendMeasuresRepeatToSegment (
          fVoiceCurrentMeasuresRepeat);

        // create a new last segment to collect the remainder of the voice,
        // containing the next, yet incomplete, measure
#ifdef TRACE_OPTIONS
        if (
          gGeneralOptions->fTraceMeasures
            ||
          gGeneralOptions->fTraceRepeats
            ||
          gGeneralOptions->fTraceSegments
            ||
          gGeneralOptions->fTraceVoices
          ) {
          gLogIOstream <<
            "Creating a new last segment with the AAA measures repeat next measure for voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
            
        createNewLastSegmentFromFirstMeasureForVoice (
          inputLineNumber,
          nextMeasureAfterMeasuresRepeat,
          "()");
      }
      break;
  } // switch
}

void msrVoice::createMeasuresRepeatAndAppendItToVoiceClone (
  int inputLineNumber,
  int measuresRepeatMeasuresNumber,
  int measuresRepeatSlashesNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating measures repeat and appending it to voice clone \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;

 // JMI   displayVoiceRepeatsStack ("createMeasuresRepeatAndAppendItToVoiceClone()");
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
        // fetch last measure's full measure whole notes
        /* JMI
        int fullMeasureWholeNotes =
          fVoiceLastSegment->
            getSegmentMeasuresList ().back ()->
                getFullMeasureWholeNotes ();
                */

        // does the pending measure repeat exist?
        if (fVoiceCurrentMeasuresRepeat) {
          stringstream s;

          s <<
            "attempting to create a measures repeat while another one is pending";

          msrInternalError (
            gGeneralOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

        // create the measures repeat
#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceRepeats) {
          gLogIOstream <<
            "Creating and appending a measures repeat in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
      
        fVoiceCurrentMeasuresRepeat =
          msrMeasuresRepeat::create (
            inputLineNumber,
            measuresRepeatMeasuresNumber,
            measuresRepeatSlashesNumber,
            this);

        // create a measures repeat pattern from current last segment
#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceRepeats) {
          gLogIOstream <<
            "Creating a measures repeat pattern from current last segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
      
        S_msrMeasuresRepeatPattern
          measuresRepeatPattern =
            msrMeasuresRepeatPattern::create (
              inputLineNumber,
              fVoiceCurrentMeasuresRepeat);

        // set current last segment as the measures repeat pattern segment
        measuresRepeatPattern->
          setMeasuresRepeatPatternSegment (
            fVoiceLastSegment);

        // forget about this voice last segment
        fVoiceLastSegment = nullptr; // JMI        

        // set the measures repeat pattern
#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceRepeats) {
          gLogIOstream <<
            "Setting repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
#endif

        fVoiceCurrentMeasuresRepeat->
          setMeasuresRepeatPattern (
            measuresRepeatPattern);

/*
        // create a new last segment containing a new measure for the voice
#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices) {
          gLogIOstream <<
            "Creating a new last segment containing a new measure for voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        createNewLastSegmentAndANewMeasureAfterARepeat (
          inputLineNumber,
          fullMeasureWholeNotes);
          */

        // append the measures repeat to the list of initial elements
#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending measures repeat to the last segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
#endif

        fVoiceLastSegment->appendMeasuresRepeatToSegment (
          fVoiceCurrentMeasuresRepeat);
      }
      break;
  } // switch
}

void msrVoice::setVoiceContainsMultipleRests (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains multiple rests" <<
      endl;
  }
#endif

  fVoiceContainsMultipleRests = true;
}

void msrVoice::createMultipleRestInVoice (
  int inputLineNumber,
  int multipleRestMeasuresNumber)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
        // create a multiple rest
#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceRepeats) {
          gLogIOstream <<
            "Creating a multiple rest in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            ", " <<
            singularOrPlural (
              multipleRestMeasuresNumber, "measure", "measures") <<
            endl;
        }
#endif
      
        // grab the just created last measure from the voice,
        // (i.e. the one containing:
        //   <multiple-rest ... type="start">2</multiple-rest>)
        // which is the first rest measure
        S_msrMeasure
          firstRestMeasure =
            removeLastMeasureFromVoice (
              inputLineNumber);

        // move the current voice last segment to the initial elements list
        moveVoiceLastSegmentToInitialVoiceElements ( //JMI
          inputLineNumber,
          "createMultipleRestInVoice()");
          
/* JMI
        // create the multiple rest rests segment
        S_msrSegment
          restsSegment =
            msrSegment::create (
              inputLineNumber,
              this);

  if (! fVoiceFirstSegment) {
    fVoiceFirstSegment = fVoiceLastSegment;
  }
*/

        // create the multiple rest
        if (fVoicePendingMultipleRest) {
          stringstream s;

          s <<
            "attempting to create a multiple rest while another one is pending";

          msrInternalError (
            gGeneralOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
        
        fVoicePendingMultipleRest =
          msrMultipleRest::create (
            inputLineNumber,
            firstRestMeasure->getFullMeasureWholeNotes (),
            multipleRestMeasuresNumber,
            this);

         // remember fVoicePendingMultipleRest for later next measure number setting
#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceRepeats) {
          gLogIOstream <<
            "Registering multiple rest as waiting for its next measure number" <<
            ", multipleRestMeasuresNumber = " <<
            multipleRestMeasuresNumber <<
            " in voice \"" <<
            fVoiceName << "\"" <<
            endl;
        }
#endif
        
        fVoiceMultipleRestWaitingForItsNextMeasureNumber =
          fVoicePendingMultipleRest;

        fVoiceRemainingRestMeasures =
          multipleRestMeasuresNumber;

#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceRepeats) {
          gLogIOstream <<
            "Setting fVoiceRemainingRestMeasures to '" <<
            fVoiceRemainingRestMeasures <<
            "' in voice \"" <<
            fVoiceName << "\"" <<
            endl;
        }
#endif

        // create a new segment to collect the multiple rest measures,
        // containing the first, rest measure
#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices) {
          gLogIOstream <<
            "Creating a new last segment containing the first rest measure in voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
            
        createNewLastSegmentFromFirstMeasureForVoice (
          inputLineNumber,
          firstRestMeasure,
          "()");

/* JMI
        // append the first rest measure to the new last segment
        fVoiceLastSegment->
          appendMeasureToSegment (
            firstRestMeasure);
*/

        // force multiple measure rests compression JMI ???
        this->setVoiceContainsMultipleRests (
          inputLineNumber);
            
        // print resulting voice contents
#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices) {
          gLogIOstream <<
            "The contents of voice \"" <<
            fVoiceName <<
            "\" after createMultipleRestInVoice() is:" <<
            endl;
  
          gIndenter++;
          print (gLogIOstream);
          gIndenter--;
        }
#endif

        // keep the multiple rest pending
      }
      break;
  } // switch
}

void msrVoice::appendPendingMultipleRestToVoice (
  int inputLineNumber)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending the pending multiple rest to voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        // does the pending multiple rest exist?
        if (! fVoicePendingMultipleRest) {
          stringstream s;

          s <<
            "attempting to append a pending multiple rest which doesn't exist";

          msrInternalError (
            gGeneralOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

        /* JMI
        // fetch the last segment's measure list
        list<S_msrMeasure>&
          voiceLastSegmentMeasureList =
            fVoiceLastSegment->
              getSegmentMeasuresListToModify ();
       
        // grab the just created last measure in the last segment's measure list,
        // which is the next measure after the multiple rest
        if (! voiceLastSegmentMeasureList.size ()) {
          stringstream s;

          s <<
            "attempting to grab first measure of voice last segment, that contains none";

          msrInternalError (
            gGeneralOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

        S_msrMeasure
          nextMeasureAfterMultipleRest =
            voiceLastSegmentMeasureList.back ();

        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices) {
          gLogIOstream <<
            endl <<
            "==========> nextMeasureAfterMultipleRest:" <<
            endl;
  
            
          nextMeasureAfterMultipleRest->
            print (gLogIOstream);
            
          gLogIOstream <<
            endl;
        }
        */
          
        // remove the next measure from the last segment's measure list
    // JMI    voiceLastSegmentMeasureList.pop_back ();

        // create the multiple rest contents
#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices) {
          gLogIOstream <<
            "Creating a measure pattern for voice \"" <<
            fVoiceName << "\" is:" <<
            endl;
        }
#endif

        S_msrMultipleRestContents
          multipleRestContents =
            msrMultipleRestContents::create (
              inputLineNumber,
              this);

        // set voice last segment as the multiple rest segment
#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceRepeats) {
          gLogIOstream <<
            "Setting current last segment as multiple rest segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
#endif
      
        multipleRestContents->
          setMultipleRestContentsSegment (
            fVoiceLastSegment);

        // forget about this voice last segment
        fVoiceLastSegment = nullptr;

        // set multipleRestContents as the multiple rest contents
#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceRepeats) {
          gLogIOstream <<
            "Setting multiple rest contents in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
#endif
      
        fVoicePendingMultipleRest->
          setMultipleRestContents (
            multipleRestContents);

        // append pending multiple rest to the last segment
        fVoiceLastSegment->appendMultipleRestToSegment (
          fVoicePendingMultipleRest);

            /* JMI
        // create a new segment to collect the remainder of the voice,
        // containing the next, yet incomplete, measure
#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices) {
          gLogIOstream <<
            "Creating a new last segment with the measure after the multiple rests for voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        createNewLastSegmentForVoice (
          inputLineNumber);
          */

/* JMI
        // append the next measure after the multiple rest
        // to the new last segment
        fVoiceLastSegment->
          appendMeasureToSegment (
            nextMeasureAfterMultipleRest);
*/
        // print resulting voice contents
#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices) {
          gLogIOstream <<
            "The contents of voice \"" <<
            fVoiceName <<
            "\" after appendPendingMultipleRestToVoice () is:" <<
            endl;

          gIndenter++;
          print (gLogIOstream);
          gIndenter--;
        }
#endif

        // forget about this pending multiple rest
        fVoicePendingMultipleRest = nullptr;
      }
      break;
  } // switch
}

void msrVoice::handleMultipleRestInVoiceClone (
  int inputLineNumber)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      // is there a voice last segment?
      if (fVoiceLastSegment) {
        
        // are there measures in the voice last segment?
        if (fVoiceLastSegment->getSegmentMeasuresList ().size ()) {
          
          // fetch last measure's full measure whole notes
          /* JMI
          int fullMeasureWholeNotes =
            fVoiceLastSegment->
              getSegmentMeasuresList ().back ()->
                  getFullMeasureWholeNotes ();
                  */
                
          // finalize current measure in voice
          finalizeCurrentMeasureInVoice (
            inputLineNumber);
  
#ifdef TRACE_OPTIONS
          if (
            gGeneralOptions->fTraceMultipleRests
              ||
            gGeneralOptions->fTraceVoicesDetails) {
            gLogIOstream <<
              endl <<
              "Before handleMultipleRestInVoiceClone(), voice \"" <<
              getVoiceName () <<
              "\"" <<
              ", line " << inputLineNumber <<
              ", contains:" <<
              endl;
  
            print (gLogIOstream);
          }
#endif

          // is this multiple rest nested in a repeat?
          if (fVoiceRepeatsStack.size ()) {
            // yes

            // fVoiceLastSegment is cumulating elements for the repeat common part:
            // this should be set aside, and later re-installed as the voice last segment
#ifdef TRACE_OPTIONS
            if (
              gGeneralOptions->fTraceMultipleRests
                ||
              gGeneralOptions->fTraceSegments
                ||
              gGeneralOptions->fTraceVoices
            ) {
              gLogIOstream <<
                "Putting voice last segment " <<
                fVoiceLastSegment->asString () <<
                "' aside for multiple rest in voice \"" <<
                fVoiceName << "\"" <<
                ", line " << inputLineNumber <<
                endl;
            }
#endif

            // move fVoiceLastSegment to the list of initial elements
            moveVoiceLastSegmentToRepeatCommonPart (
              inputLineNumber,
              fVoiceRepeatsStack.front ()->getRepeatCommonPart (), // JMI
              "handleMultipleRestInVoiceClone()");
  
#ifdef TRACE_OPTIONS
            if (
              gGeneralOptions->fTraceMultipleRests
                ||
              gGeneralOptions->fTraceVoicesDetails
            ) {
              displayVoiceContents (
                inputLineNumber,
                "handleMultipleRestInVoiceClone()");
            }
#endif
          }
          
          else {
            // no
          
            // move current last segment to the list of initial elements
            moveVoiceLastSegmentToInitialVoiceElements ( // JMI
              inputLineNumber,
              "handleMultipleRestInVoiceClone()");

  /* JMI
            // create a new last segment containing a new measure for the voice
#ifdef TRACE_OPTIONS
            if (
              gGeneralOptions->fTraceMultipleRests
                ||
              gGeneralOptions->fTraceSegments
                ||
              gGeneralOptions->fTraceVoices
            ) {
              gLogIOstream <<
                "Creating a new last segment containing a new measure for voice \"" <<
                fVoiceName << "\"" <<
                ", line " << inputLineNumber <<
                endl;
            }
#endif
  
            createNewLastSegmentAndANewMeasureAfterARepeat (
              inputLineNumber,
              fullMeasureWholeNotes);
              */
  
#ifdef TRACE_OPTIONS
            if (
              gGeneralOptions->fTraceMultipleRests
                ||
              gGeneralOptions->fTraceVoicesDetails
            ) {
              displayVoiceContents (
                inputLineNumber,
                "handleMultipleRestInVoiceClone()");
            }
#endif
          }

#ifdef TRACE_OPTIONS
          if (
            gGeneralOptions->fTraceMultipleRests
              ||
            gGeneralOptions->fTraceVoicesDetails) {
            gLogIOstream <<
              endl <<
              "After handleMultipleRestInVoiceClone(), voice \"" <<
              getVoiceName () <<
              "\"" <<
              ", line " << inputLineNumber <<
              ", contains:" <<
              endl;
  
            print (gLogIOstream);
          }
#endif
        }
      }
      break;
  } // switch
}

void msrVoice::appendMultipleRestCloneToVoice (
  int               inputLineNumber,
  S_msrMultipleRest multipleRestClone)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceMultipleRests) {
          gLogIOstream <<
            "Appending multiple rest clone '" <<
            multipleRestClone->asString () <<
            " to voice clone \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        // print beforehand voice contents
#ifdef TRACE_OPTIONS
        if (
          gGeneralOptions->fTraceMultipleRests
            ||
          gGeneralOptions->fTraceSegments
            ||
          gGeneralOptions->fTraceVoices
        ) {
          gLogIOstream <<
            "The contents of voice \"" <<
            fVoiceName <<
            "\" before appendMultipleRestCloneToVoice () is:" <<
            endl;

          gIndenter++;
          print (gLogIOstream);
          gIndenter--;
        }
#endif

        // is this multiple rest nested in a repeat?
        if (fVoiceRepeatsStack.size ()) {
          // yes

          S_msrRepeat
            currentRepeat =
              fVoiceRepeatsStack.front ();
              
          // grab the multiple rest segment, i.e. the voice's last segment
          S_msrSegment
            multipleRestSegment =
              fVoiceLastSegment;

          // forget about this voice last segment
          fVoiceLastSegment = nullptr;

          // append it to the current repeat's common part
          currentRepeat->
            getRepeatCommonPart ()->
              appendSegmentToRepeatCommonPart ( // NO !!!
                inputLineNumber,
                multipleRestSegment,
                "appendMultipleRestCloneToVoice()");
        }
        
        else {
          // no
          // JMI ???
        }

        // append the multiple rest clone to the last segment
        fVoiceLastSegment->appendMultipleRestToSegment (
          multipleRestClone);

        // print resulting voice contents
#ifdef TRACE_OPTIONS
        if (
          gGeneralOptions->fTraceMultipleRests
            ||
          gGeneralOptions->fTraceSegments
            ||
          gGeneralOptions->fTraceVoices
        ) {
          gLogIOstream <<
            "The contents of voice \"" <<
            fVoiceName <<
            "\" after appendMultipleRestCloneToVoice () is:" <<
            endl;

          gIndenter++;
          print (gLogIOstream);
          gIndenter--;
        }
#endif
      }
      break;
  } // switch
}

void msrVoice::appendRepeatCloneToVoice (
  int         inputLineNumber,
  S_msrRepeat repeatCLone)
{
  // sanity check
  msrAssert(
    repeatCLone != nullptr,
    "repeatCLone is null");
      
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending repeat clone to voice clone \"" <<
            getVoiceName () <<  "\"" <<
            endl;

          displayVoiceRepeatsStack ("appendRepeatCloneToVoice()");
        }
#endif

      /* JMI
        // get repeatCLone's common part
        S_msrRepeatCommonPart
          repeatCommonPart =
            repeatCLone->
              getRepeatCommonPart ();

        // move voice last segment into the repeat common part
#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceRepeats) {
          gLogIOstream <<
            "Append the segment to use to the repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
      
        repeatCommonPart->
          appendSegmentToRepeatCommonPart (
            inputLineNumber,
            fVoiceLastSegment,
            "appendRepeatCloneToVoice()");
*/

        // register repeat clone as the (new) current repeat
#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceRepeats) {
          gLogIOstream <<
            "Registering repeat clone as the new current repeat in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
#endif
      
        // push the repeat clone onto the voice's repeats stack
        pushRepeatOntoRepeatsStack (
          inputLineNumber,
          repeatCLone,
          "appendRepeatCloneToVoice()");

        // append it to the list of initial elements
         appendRepeatCloneToInitialVoiceElements (
          inputLineNumber,
          repeatCLone,
          "handleRepeatEndInVoice()");

      /* JMI
        // create a new segment for the voice
        createNewLastSegmentForVoice (
          inputLineNumber);
          */
        }
      break;
  } // switch
}

void msrVoice::handleHookedRepeatEndingEndInVoice (
  int       inputLineNumber,
  string    repeatEndingNumber) // may be "1, 2"
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Handling a hooked repeat ending in voice \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      endl;

    displayVoiceRepeatsStack ("handleHookedRepeatEndingEndInVoice()");
  }
#endif

  gIndenter++;

/* JMI
        int voiceRepeatsStackSize =
          fVoiceRepeatsStack.size ();
*/

  if (! fVoiceRepeatsStack.size ()) {
    stringstream s;

    s <<
      "repeats stack is empty when attempting to handle a hooked repeat ending end in voice '" <<
      asShortString () <<
      "' ";
      
    msrInternalError (
      gGeneralOptions->fInputSourceName,
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // finalize current measure in voice JMI lastMeasureInLastSegment ???
  finalizeCurrentMeasureInVoice (
    inputLineNumber);

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoiceRepeatsStack.front ();

  // create a hooked repeat ending
  msrRepeatEnding::msrRepeatEndingKind
    repeatEndingKind =
      msrRepeatEnding::kHookedEnding;
    
  S_msrRepeatEnding
    repeatEnding =
      msrRepeatEnding::create (
        inputLineNumber,
        repeatEndingNumber,
        repeatEndingKind,
        currentRepeat);

/* JMI
  // move the voice last segment to repeatEnding
  moveVoiceLastSegmentToRepeatEnding (
    inputLineNumber,
    repeatEnding,
    "handleHookedRepeatEndingEndInVoice()");
    */

  // add the repeat ending to the voice current repeat
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending a " <<
      msrRepeatEnding::repeatEndingKindAsString (
        repeatEndingKind) <<
      " repeat ending to current repeat in voice \"" <<
      fVoiceName <<
      "\"" <<
      endl;
  }
#endif

  displayVoiceRepeatsStack ("before adding a hooked repeat ending to current repeat");

  currentRepeat->
    addRepeatEndingToRepeat (
      inputLineNumber,
      repeatEnding);

  displayVoiceRepeatsStack ("after adding a hooked repeat ending to current repeat");

  gIndenter--;
}

void msrVoice::handleHooklessRepeatEndingEndInVoice (
  int       inputLineNumber,
  string    repeatEndingNumber) // may be "1, 2"
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Handling a hookless repeat ending in voice \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      endl;

    displayVoiceRepeatsStack ("handleHooklessRepeatEndingEndInVoice()");
  }
#endif

  gIndenter++;

  if (! fVoiceRepeatsStack.size ()) {
    stringstream s;

    s <<
      "repeats stack is empty when attempting to handle a hookless repeat ending end in voice '" <<
      asShortString () <<
      "' ";
      
    msrInternalError (
      gGeneralOptions->fInputSourceName,
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // finalize current measure in voice JMI lastMeasureInLastSegment ???
  finalizeCurrentMeasureInVoice (
    inputLineNumber);

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoiceRepeatsStack.front ();

  // create a hookless repeat ending
  msrRepeatEnding::msrRepeatEndingKind
    repeatEndingKind =
      msrRepeatEnding::kHooklessEnding;
    
  S_msrRepeatEnding
    repeatEnding =
      msrRepeatEnding::create (
        inputLineNumber,
        repeatEndingNumber,
        repeatEndingKind,
        currentRepeat);

  // move the voice last segment to repeatEnding
  moveVoiceLastSegmentToRepeatEnding (
    inputLineNumber,
    repeatEnding,
    "handleHookedRepeatEndingEndInVoice()");

  // add the repeat ending it to the voice current repeat
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending a " <<
      msrRepeatEnding::repeatEndingKindAsString (
        repeatEndingKind) <<
      " repeat ending to current repeat in voice \"" <<
      fVoiceName <<
      "\"" <<
      endl;
  }
#endif

  displayVoiceRepeatsStack ("before adding a hookless repeat ending to current repeat");

  currentRepeat->
    addRepeatEndingToRepeat (
      inputLineNumber,
      repeatEnding);

  displayVoiceRepeatsStack ("after adding a hookless repeat ending to current repeat");

  // append currentRepeat to the list of initial elements
  appendRepeatCloneToInitialVoiceElements (
    inputLineNumber,
    currentRepeat,
    "handleHooklessRepeatEndingEndInVoice()");

  // pop it from the voice's repeats stack
  popARepeatFromStack (
    inputLineNumber,
    currentRepeat,
    "handleHooklessRepeatEndingEndInVoice");

  // create a new last segment containing a new measure for the voice
  createNewLastSegmentForVoice (
    inputLineNumber,
    "handleHooklessRepeatEndingEndInVoice()");

  gIndenter--;
}

void msrVoice::handleRepeatEndingEndInVoice (
  int       inputLineNumber,
  string    repeatEndingNumber, // may be "1, 2"
  msrRepeatEnding::msrRepeatEndingKind
            repeatEndingKind)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
        switch (repeatEndingKind) {
          case msrRepeatEnding::kHookedEnding:
            handleHookedRepeatEndingEndInVoice (
              inputLineNumber,
              repeatEndingNumber);
            break;
            
          case msrRepeatEnding::kHooklessEnding:
            handleHooklessRepeatEndingEndInVoice (
              inputLineNumber,
              repeatEndingNumber);
            break;
        } // switch
      break;
    }
  } // switch

#ifdef TRACE_OPTIONS
  if (
    gGeneralOptions->fTraceRepeats
      ||
    gGeneralOptions->fTraceVoicesDetails
  ) {
    displayVoiceContents (
      inputLineNumber,
      "appendRepeatEndingToVoice() 1");
  }
#endif
}

void msrVoice::handleRepeatCommonPartStartInVoiceClone (
  int inputLineNumber) // may be "1, 2"
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Handling a repeat common part start in voice clone \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OPTIONS
  if (
    gGeneralOptions->fTraceRepeats
      ||
    gGeneralOptions->fTraceVoicesDetails
  ) {
    displayVoiceRepeatsStack ("handleRepeatCommonPartStartInVoiceClone() 0");

    displayVoiceContents (
      inputLineNumber,
      "handleRepeatCommonPartStartInVoiceClone() 0");
  }
#endif

  if (! fVoiceRepeatsStack.size ()) {
    stringstream s;

    s <<
      "repeats stack is empty when attempting to handle repeat ending '" <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";
      
    msrInternalError (
      gGeneralOptions->fInputSourceName,
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoiceRepeatsStack.front ();

  // create the currentRepeat's common part
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a repeat common part upon its end in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  S_msrRepeatCommonPart
    repeatCommonPart =
      msrRepeatCommonPart::create (
        inputLineNumber,
        currentRepeat);

  // register it in currentRepeat
  currentRepeat->
    setRepeatCommonPart (
      repeatCommonPart);      
}

void msrVoice::handleRepeatCommonPartEndInVoiceClone (
  int inputLineNumber) // may be "1, 2"
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Handling a repeat common part end in voice clone \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OPTIONS
  if (
    gGeneralOptions->fTraceRepeats
      ||
    gGeneralOptions->fTraceVoicesDetails
  ) {
    displayVoiceRepeatsStack ("handleRepeatCommonPartEndInVoiceClone() 1");

    displayVoiceContents (
      inputLineNumber,
      "handleRepeatCommonPartEndInVoiceClone() 1");
  }
#endif

  gIndenter++;

  if (! fVoiceRepeatsStack.size ()) {
    stringstream s;

    s <<
      "repeats stack is empty when attempting to handle repeat ending '" <<
 //     repeatEnding->asShortString () <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";
      
    msrInternalError (
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoiceRepeatsStack.front ();

  // get currentRepeat's common part
  S_msrRepeatCommonPart
    repeatCommonPart =
      currentRepeat->
        getRepeatCommonPart ();

/* JMI
  // move voice last segment into the repeat common part
  stringstream s2;

  s2 <<
    "Moving the voice last segment to the repeat common part in voice clone \"" <<
    getVoiceName () <<
    "\"" <<
    ", line " << inputLineNumber;
  
  moveVoiceLastSegmentToRepeatCommonPart (
    inputLineNumber,
    repeatCommonPart,
    s2.str ());
         */
                  
  gIndenter--;

#ifdef TRACE_OPTIONS
  if (
    gGeneralOptions->fTraceRepeats
      ||
    gGeneralOptions->fTraceVoicesDetails
  ) {
    displayVoiceRepeatsStack ("handleRepeatCommonPartEndInVoiceClone() 2");

    displayVoiceContents (
      inputLineNumber,
      "handleRepeatCommonPartEndInVoiceClone() 2");
  }
#endif
}

void msrVoice::handleHookedRepeatEndingEndInVoiceClone (
  int    inputLineNumber,
  string repeatEndingNumber) // may be "1, 2"
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Handling a hooked repeat ending in voice clone \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OPTIONS
  if (
    gGeneralOptions->fTraceRepeats
      ||
    gGeneralOptions->fTraceVoicesDetails
  ) {
    displayVoiceRepeatsStack ("handleHookedRepeatEndingEndInVoiceClone()");

    displayVoiceContents (
      inputLineNumber,
      "handleHookedRepeatEndingEndInVoiceClone() 1");
  }
#endif

  gIndenter++;

  if (! fVoiceRepeatsStack.size ()) {
    stringstream s;

    s <<
      "repeats stack is empty when attempting to handle hooked repeat ending '" <<
 //     repeatEnding->asShortString () <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";
      
    msrInternalError (
      gGeneralOptions->fInputSourceName,
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoiceRepeatsStack.front ();

  // create a hooked repeat ending
  msrRepeatEnding::msrRepeatEndingKind
    repeatEndingKind =
      msrRepeatEnding::kHookedEnding;
    
  S_msrRepeatEnding
    repeatEnding =
      msrRepeatEnding::create (
        inputLineNumber,
        repeatEndingNumber,
        repeatEndingKind,
        currentRepeat);

//* JMI
  // move the voice last segment to repeatEnding
  moveVoiceLastSegmentToRepeatEnding (
    inputLineNumber,
    repeatEnding,
    "handleHookedRepeatEndingEndInVoiceClone()");
  //  */

  // add the repeat ending it to currentRepeat
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending a " <<
      msrRepeatEnding::repeatEndingKindAsString (
        repeatEndingKind) <<
      " repeat ending to current repeat in voice clone \"" <<
      fVoiceName <<
      "\"" <<
      endl;
  }
#endif

  displayVoiceRepeatsStack ("before adding a hooked repeat ending to current repeat");

  currentRepeat->
    addRepeatEndingToRepeat (
      inputLineNumber,
      repeatEnding);

  displayVoiceRepeatsStack ("after adding a hooked repeat ending to current repeat");

  gIndenter--;

#ifdef TRACE_OPTIONS
  if (
    gGeneralOptions->fTraceRepeats
      ||
    gGeneralOptions->fTraceVoicesDetails
  ) {
    displayVoiceRepeatsStack ("handleHookedRepeatEndingEndInVoiceClone()");

    displayVoiceContents (
      inputLineNumber,
      "handleHookedRepeatEndingEndInVoiceClone() 2");
  }
#endif
}

void msrVoice::handleHooklessRepeatEndingEndInVoiceClone (
  int    inputLineNumber,
  string repeatEndingNumber) // may be "1, 2"
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Handling a hookless repeat ending in voice clone \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OPTIONS
  if (
    gGeneralOptions->fTraceRepeats
      ||
    gGeneralOptions->fTraceVoicesDetails
  ) {
    displayVoiceRepeatsStack ("handleHooklessRepeatEndingEndInVoiceClone()");

    displayVoiceContents (
      inputLineNumber,
      "handleHooklessRepeatEndingEndInVoiceClone() 1");
  }
#endif

  gIndenter++;

  if (! fVoiceRepeatsStack.size ()) {
    stringstream s;

    s <<
      "repeats stack is empty when attempting to handle hookless repeat ending '" <<
 //     repeatEnding->asShortString () <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";
      
    msrInternalError (
      gGeneralOptions->fInputSourceName,
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoiceRepeatsStack.front ();

  // create a hookless repeat ending
  msrRepeatEnding::msrRepeatEndingKind
    repeatEndingKind =
      msrRepeatEnding::kHooklessEnding;
    
  S_msrRepeatEnding
    repeatEnding =
      msrRepeatEnding::create (
        inputLineNumber,
        repeatEndingNumber,
        repeatEndingKind,
        currentRepeat);

/* JMI
  // move the voice last segment to repeatEnding
  moveVoiceLastSegmentToRepeatEnding (
    inputLineNumber,
    repeatEnding,
    "handleHooklessRepeatEndingEndInVoiceClone()");
    */

  // add the repeat ending to the voice current repeat
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending a " <<
      msrRepeatEnding::repeatEndingKindAsString (
        repeatEndingKind) <<
      " repeat ending to current repeat in voice clone \"" <<
      fVoiceName <<
      "\"" <<
      endl;
  }
#endif

  displayVoiceRepeatsStack ("before adding a hookless repeat ending to current repeat");

  currentRepeat->
    addRepeatEndingToRepeat (
      inputLineNumber,
      repeatEnding);

  gIndenter--;

#ifdef TRACE_OPTIONS
  if (
    gGeneralOptions->fTraceRepeats
      ||
    gGeneralOptions->fTraceVoicesDetails
  ) {
    displayVoiceRepeatsStack ("handleHooklessRepeatEndingEndInVoiceClone()");

    displayVoiceContents (
      inputLineNumber,
      "handleHooklessRepeatEndingEndInVoiceClone() 2");
  }
#endif
}

void msrVoice::handleRepeatEndingEndInVoiceClone (
  int       inputLineNumber,
  string    repeatEndingNumber, // may be "1, 2"
  msrRepeatEnding::msrRepeatEndingKind
            repeatEndingKind)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
        switch (repeatEndingKind) {
          case msrRepeatEnding::kHookedEnding:
            handleHookedRepeatEndingEndInVoiceClone (
              inputLineNumber,
              repeatEndingNumber);
            break;
            
          case msrRepeatEnding::kHooklessEnding:
            handleHooklessRepeatEndingEndInVoiceClone (
              inputLineNumber,
              repeatEndingNumber);
            break;
        } // switch
      break;
    }
  } // switch

#ifdef TRACE_OPTIONS
  if (
    gGeneralOptions->fTraceRepeats
      ||
    gGeneralOptions->fTraceVoicesDetails
  ) {
    displayVoiceContents (
      inputLineNumber,
      "appendRepeatEndingToVoiceClone() 1");
  }
#endif
}

void msrVoice::handleRepeatStartInVoiceClone (
  int         inputLineNumber,
  S_msrRepeat repeat)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Handling repeat start in voice clone \"" <<
      getVoiceName () <<
      "\" in staff \"" <<
      fVoiceStaffUplink->getStaffName () <<
      "\" in part " <<
      fVoiceStaffUplink->
        getStaffPartUplink ()->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OPTIONS
  if (
    gGeneralOptions->fTraceRepeats
      ||
    gGeneralOptions->fTraceVoicesDetails
  ) {
    displayVoiceRepeatsStack ("handleRepeatStartInVoiceClone()");

    displayVoiceContents (
      inputLineNumber,
      "handleRepeatStartInVoiceClone() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      gIndenter++;

      // is there a voice last segment?
      if (fVoiceLastSegment) {
        
        // fetch last segment's measures list
        list<S_msrMeasure>&
          voiceLastSegmentMeasuresList =
            fVoiceLastSegment->
              getSegmentMeasuresListToModify ();
              
        // are there measures in the voice last segment?
        if (voiceLastSegmentMeasuresList.size ()) {

          // fetch last measure in the last segment
          S_msrMeasure
            lastMeasureInLastSegment =
              voiceLastSegmentMeasuresList.back ();
       
          // finalize current measure in voice JMI lastMeasureInLastSegment ???
          // this may remove it if it is empty JMI ???
          finalizeCurrentMeasureInVoice (
            inputLineNumber);
  
#ifdef TRACE_OPTIONS
          if (
            gGeneralOptions->fTraceRepeats
              ||
            gGeneralOptions->fTraceVoicesDetails
          ) {
            displayVoiceRepeatsStack ("handleRepeatStartInVoiceClone() 1");

            displayVoiceContents (
              inputLineNumber,
              "handleRepeatStartInVoiceClone() 1");
          }
#endif

          // move current last segment to the list of initial elements
          moveVoiceLastSegmentToInitialVoiceElements (
            inputLineNumber,
            "handleRepeatStartInVoiceClone()");
        }
      }

      // create the repeat clone and stack it
#ifdef TRACE_OPTIONS
      if (gGeneralOptions->fTraceRepeats) {
        gLogIOstream <<
          "Creating a repeat upon its start in voice clone \"" <<
          getVoiceName () <<
          "\"" <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif

      S_msrRepeat
        repeatClone =
          createARepeatCloneAndStackIt (
            inputLineNumber,
            repeat,
            "handleRepeatStartInVoiceClone()");

#ifdef TRACE_OPTIONS
      if (
        gGeneralOptions->fTraceRepeats
          ||
        gGeneralOptions->fTraceVoicesDetails
      ) {
        displayVoiceRepeatsStack ("handleRepeatStartInVoiceClone() 2");

        displayVoiceContents (
          inputLineNumber,
          "handleRepeatStartInVoiceClone() 2");
      }
#endif

      gIndenter--;

      break;
  } // switch
}

void msrVoice::handleRepeatEndInVoiceClone (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Handling a repeat end in voice clone \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
        gIndenter++;

        // finalize current measure in voice
        finalizeCurrentMeasureInVoice (
          inputLineNumber);
          
#ifdef TRACE_OPTIONS
        if (
          gGeneralOptions->fTraceRepeats
            ||
          gGeneralOptions->fTraceVoicesDetails
        ) {
          displayVoiceRepeatsStack ("handleRepeatEndInVoiceClone() 1");

          gLogIOstream <<
            endl <<
            "*********>> handleRepeatEndInVoiceClone() 1 \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            " contains:" <<
            endl;

          print (gLogIOstream);

          gLogIOstream <<
            "<<*********" <<
            endl <<
            endl;
        }
#endif

        // is there a current repeat?
        switch (fVoiceRepeatsStack.size ()) {
          case 0:
            {
              stringstream s;
          
              s <<
                "repeats stack is empty when attempting to handle a repeat end in voice clone '" <<
                asShortString () <<
                "' ";
                
              msrInternalError (
                gGeneralOptions->fInputSourceName,
                fInputLineNumber,
                __FILE__, __LINE__,
                s.str ());
            }
            break;

          case 1:
            {
              // this is a voice-level repeat
              
              // fetch currentRepeat
              S_msrRepeat
                currentRepeat =
                  fVoiceRepeatsStack.front ();
                  
              // get currentRepeat's common part
              S_msrRepeatCommonPart
                repeatCommonPart =
                  currentRepeat->
                    getRepeatCommonPart ();

            /* JMI
              // move all the voice contents to the new repeat common part
              moveAllVoiceContentsToRepeatCommonPart (
                inputLineNumber,
                repeatCommonPart,
                "handleRepeatEndInVoiceClone");
                */

              // append currentRepeat to the list of initial elements
              appendRepeatToInitialVoiceElements (
                inputLineNumber,
                currentRepeat,
                "handleVoiceLevelRepeatEndWithImplicitStartInVoice()");

              // pop currentRepeat from the voice's repeats stack
              popARepeatFromStack (
                inputLineNumber,
                currentRepeat,
                "handleRepeatEndInVoiceClone");
            }
            break;
        
          default:
            {
              // this is a nested repeat
    
              // fetch currentRepeat
              S_msrRepeat
                currentRepeat =
                  fVoiceRepeatsStack.front ();
                  
              // get currentRepeat's common part
              S_msrRepeatCommonPart
                repeatCommonPart =
                  currentRepeat->
                    getRepeatCommonPart ();
            
              // append currentRepeat to the list of initial elements
              appendRepeatCloneToInitialVoiceElements (
                inputLineNumber,
                currentRepeat,
                "handleRepeatEndInVoiceClone()");

              // pop currentRepeat from the voice's repeats stack
              popARepeatFromStack (
                inputLineNumber,
                currentRepeat,
                "handleRepeatEndInVoiceClone");
            }
        } // switch


#ifdef TRACE_OPTIONS
        if (
          gGeneralOptions->fTraceRepeats
            ||
          gGeneralOptions->fTraceVoicesDetails
        ) {
          displayVoiceRepeatsStack ("handleRepeatEndInVoiceClone() 2");
      
          displayVoiceContents (
            inputLineNumber,
            "handleRepeatEndInVoiceClone() 2");
        }
#endif

        gIndenter--;
      }
      break;
  } // switch
}

void msrVoice::appendMeasuresRepeatReplicaToVoice (
  int inputLineNumber)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending a measures repeat replica to voice \"" <<
            getVoiceName () <<  "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
      
        // fetch last measure's full measure whole notes
        /* JMI
        int fullMeasureWholeNotes =
          fVoiceLastSegment->
            getSegmentMeasuresList ().back ()->
              getFullMeasureWholeNotes ();
              */

#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceRepeats) { // JMI
          gLogIOstream <<
            endl <<
            "***********" <<
            endl <<
            endl;
          print (gLogIOstream);
          gLogIOstream <<
            "***********" <<
            endl <<
            endl;
        }
#endif
        
        // sanity check
        msrAssert (
          fVoiceCurrentMeasuresRepeat != nullptr,
          "fVoiceCurrentMeasuresRepeat is null");
                
        // create a measures repeat replicas
        S_msrMeasuresRepeatReplicas
          measuresRepeatReplicas =
            msrMeasuresRepeatReplicas::create (
              inputLineNumber,
              fVoiceCurrentMeasuresRepeat);

        // set the voice last segment as the measure repeat replicas segment
        measuresRepeatReplicas->
          setMeasuresRepeatReplicasSegment (
            fVoiceLastSegment);

        // forget about this voice last segment
        fVoiceLastSegment = nullptr;
        
        // set the measures repeat replicas in the voice current measures repeat
#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceRepeats) {
          gLogIOstream <<
            "Setting the measures repeat replica to current measures repeat BBB in voice \"" <<
            fVoiceName <<
            "\"" <<
            endl;
        }
#endif
            
        fVoiceCurrentMeasuresRepeat->
          setMeasuresRepeatReplicas (
            measuresRepeatReplicas);

/* JMI
        // create a new last segment containing a new measure for the voice
#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices) {
          gLogIOstream <<
            "Creating a new last segment containing a new measure for voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
            
        createNewLastSegmentAndANewMeasureAfterARepeat (
          inputLineNumber,
          fullMeasureWholeNotes);
          */
      }
      break;
  } // switch
}

void msrVoice:: appendRepeatEndingCloneToVoice ( // JMI
  S_msrRepeatEnding repeatEndingClone)
{
  gIndenter++;
  
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
    case msrVoice::kFiguredBassVoice:
      {
        // add the repeat ending it to the voice current repeat
#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceRepeats) {
          gLogIOstream <<
            "Appending a " <<
            msrRepeatEnding::repeatEndingKindAsString (
              repeatEndingClone->getRepeatEndingKind ()) <<
            " repeat ending clone to current repeat in voice \"" <<
            fVoiceName <<
            "\"" <<
            endl;

          displayVoiceRepeatsStack ("appendRepeatEndingCloneToVoice()");
        }
#endif
            
        // sanity check
        if (! fVoiceRepeatsStack.size ()) {
          stringstream s;
      
          s <<
            "repeats stack is empty when attempting to append a "<<
            msrRepeatEnding::repeatEndingKindAsString (
              repeatEndingClone->getRepeatEndingKind ()) <<
            " repeat ending to voice '" <<
            asShortString () <<
            "' ";
            
          msrInternalError (
            gGeneralOptions->fInputSourceName,
            fInputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

        S_msrRepeat
          currentRepeat =
            fVoiceRepeatsStack.front ();
          
        currentRepeat->
          addRepeatEndingToRepeat (
            repeatEndingClone->getInputLineNumber (),
            repeatEndingClone);
      }
      break;
  } // switch

  gIndenter--;
}

void msrVoice::prependBarlineToVoice (S_msrBarline barline)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Prepending barline '" <<
      barline->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      ":" <<
      endl;
  }
#endif
      
  gIndenter++;
  
  fVoiceLastSegment->
    prependBarlineToSegment (barline);

  gIndenter--;
}

void msrVoice::appendBarlineToVoice (S_msrBarline barline)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending barline '" <<
      barline->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      ":" <<
      endl;
  }
#endif
      
  gIndenter++;
  
  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
    createNewLastSegmentForVoice (
      barline->getInputLineNumber (),
      "appendBarlineToVoice()");
  }

  fVoiceLastSegment->
    appendBarlineToSegment (barline);

  gIndenter--;
}

void msrVoice::appendSegnoToVoice (S_msrSegno segno)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending a segno to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
    createNewLastSegmentForVoice (
      segno->getInputLineNumber (),
      "appendSegnoToVoice()");
  }

  fVoiceLastSegment->
    appendSegnoToSegment (segno);
}

void msrVoice::appendCodaToVoice (S_msrCoda coda)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Appending a coda to voice \"" << getVoiceName () << "\"" <<
      ":" <<
      endl;
  }
#endif

  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
    createNewLastSegmentForVoice (
      coda->getInputLineNumber (),
      "appendSegnoToVoice()");
  }

  fVoiceLastSegment->
    appendCodaToSegment (coda);
}

void msrVoice::appendEyeGlassesToVoice (S_msrEyeGlasses eyeGlasses)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceNotes) {
    gLogIOstream <<
      "Appending a eyeGlasses to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
    createNewLastSegmentForVoice (
      eyeGlasses->getInputLineNumber (),
      "appendSegnoToVoice()");
  }

  fVoiceLastSegment->
    appendEyeGlassesToSegment (eyeGlasses);
}

void msrVoice::appendPedalToVoice (S_msrPedal pedal)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceNotes) {
    gLogIOstream <<
      "Appending a pedal to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
    createNewLastSegmentForVoice (
      pedal->getInputLineNumber (),
      "appendSegnoToVoice()");
  }

  fVoiceLastSegment->
    appendPedalToSegment (pedal);
}

void msrVoice::appendDampToVoice (S_msrDamp damp)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceNotes) {
    gLogIOstream <<
      "Appending a damp to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
    createNewLastSegmentForVoice (
      damp->getInputLineNumber (),
      "appendSegnoToVoice()");
  }

  fVoiceLastSegment->
    appendDampToSegment (damp);
}

void msrVoice::appendDampAllToVoice (S_msrDampAll dampAll)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceNotes) {
    gLogIOstream <<
      "Appending a damp all to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
    createNewLastSegmentForVoice (
      dampAll->getInputLineNumber (),
      "appendSegnoToVoice()");
  }

  fVoiceLastSegment->
    appendDampAllToSegment (dampAll);
}

/*
S_msrElement msrVoice::removeLastElementFromVoice (  // JMI
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Removing last note" <<
      " from voice " << getVoiceName () <<
      endl;
  }
#endif

  return
    fVoiceLastSegment->
      removeLastElementFromSegment (inputLineNumber);
}
*/

void msrVoice::removeNoteFromVoice (
  int       inputLineNumber,
  S_msrNote note)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceChords) {
    gLogIOstream <<
      "Removing note '" <<
      note->asShortString () <<
      "' from voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  gIndenter++;
  
  fVoiceLastSegment->
    removeNoteFromSegment (
      inputLineNumber,
      note);

  gIndenter--;
}

void msrVoice::removeElementFromVoice (
  int          inputLineNumber,
  S_msrElement element)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceChords || gGeneralOptions->fTraceGraceNotes) {
    gLogIOstream <<
      "Removing element '" <<
      element->asShortString () <<
      "' from voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  gIndenter++;
  
  fVoiceLastSegment->
    removeElementFromSegment (
      inputLineNumber,
      element);

  gIndenter--;
}

S_msrMeasure msrVoice::removeLastMeasureFromVoice (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Removing last measure from voice \"" <<
      getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;

  // remove (new) last measure
  S_msrMeasure result =
    fVoiceLastSegment->
      removeLastMeasureFromSegment (
        inputLineNumber);

  gIndenter--;

  // return it
  return result;
}

void msrVoice::finalizeCurrentMeasureInVoice (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Finalizing current measure in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
      
    gIndenter++;

    const int fieldWidth = 26;
    
    gLogIOstream << left <<
      setw (fieldWidth) <<
      "partActualMeasureWholeNotesHighTide" << " = " <<
      fetchVoicePartUplink ()->
        getPartActualMeasureWholeNotesHighTide () <<
      endl;
        
    gIndenter--;
  }
#endif

  gIndenter++;
  
  // finalize last segment's current measure
  if (fVoiceLastSegment) {
    fVoiceLastSegment->
      finalizeCurrentMeasureInSegment (
        inputLineNumber);
  }
  else {
    stringstream s;

    s <<
      "Cannot finalize current measure in voice \"" <<
      asShortString () <<
      "\": fVoiceLastSegment is null" ;
      
// JMI    msrInternalError (
    msrInternalWarning (
      gGeneralOptions->fInputSourceName,
      fInputLineNumber,
 //     __FILE__, __LINE__,
      s.str ());
  }

  // append a measure end syllable to the voice stanzas if any
  if (fVoiceStanzasMap.size ()) {
    map<string, S_msrStanza>::const_iterator
      iBegin = fVoiceStanzasMap.begin (),
      iEnd   = fVoiceStanzasMap.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      S_msrStanza
        stanza = (*i).second;

      stanza->
        appendMeasureEndSyllableToStanza (
          inputLineNumber);

      if (++i == iEnd) break;
    } // for
  }

  gIndenter--;
}


void msrVoice:: collectVoiceMeasuresIntoFlatList (
  int inputLineNumber)
{
  // collect measures from the initial elements if any
  if (fInitialVoiceElementsList.size ()) {
#ifdef TRACE_OPTIONS
    if (gGeneralOptions->fTraceVoices) {
      gLogIOstream <<
        "Collecting measures from the initial elements into voice \"" <<
        getVoiceName () <<
        "s measures flat list" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

  }

  // collect measures from the last segment if any
  if (fVoiceLastSegment) {
#ifdef TRACE_OPTIONS
    if (gGeneralOptions->fTraceVoices) {
      gLogIOstream <<
        "Collecting measures from the last segment into voice \"" <<
        getVoiceName () <<
        "s measures flat list" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    list<S_msrMeasure> lastSegmentMeasuresFlatList;

    const list<S_msrMeasure>&
      voiceLastSegmentMeasuresList =
        fVoiceLastSegment->
          getSegmentMeasuresList ();

    if (voiceLastSegmentMeasuresList.size ()) {
      list<S_msrMeasure>::const_iterator
        iBegin = voiceLastSegmentMeasuresList.begin (),
        iEnd   = voiceLastSegmentMeasuresList.end (),
        i      = iBegin;
        
      for ( ; ; ) {
        fVoiceMeasuresFlatList.push_back ((*i));
        if (++i == iEnd) break;
      } // for
    }
  }
}

void msrVoice::finalizeVoice ( // JMI ???
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceVoices) {
    gLogIOstream <<
      "Finalizing voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  // is this voice totally empty? this should be rare...
  if (
    fInitialVoiceElementsList.size () == 0
      &&
    fVoiceLastSegment->getSegmentMeasuresList ().size () == 0
  ) {
    stringstream s;
    
    s <<
      "Voice \"" <<
      getVoiceName () <<
      "\" is totally empty, no contents ever specified for it" <<
      endl;

    msrMusicXMLWarning (
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
      s.str ());
  }

  // are there pending repeats in the voice repeats stack???
  if (fVoiceRepeatsStack.size ()) {
    displayVoiceRepeatsStack ("FINALIZING VOICE");
    
    stringstream s;

    s <<
      "There are pending repeats in the voice repeats stack in voice \"" <<
      asShortString () <<
      "\" ";
      
// JMI    msrInternalError (
    msrInternalWarning (
      gGeneralOptions->fInputSourceName,
      fInputLineNumber,
 //     __FILE__, __LINE__,
      s.str ());
  }

  // collect the voice measures into the flat list
  collectVoiceMeasuresIntoFlatList (
    inputLineNumber);
}

void msrVoice::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrVoice::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrVoice::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrVoice::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrVoice::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrVoice::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrVoice::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrVoice::browseData ()" <<
      endl;
  }

  // browse the voice initial elements
  if (fInitialVoiceElementsList.size ()) {
    for (
      list<S_msrVoiceElement>::const_iterator i = fInitialVoiceElementsList.begin ();
      i != fInitialVoiceElementsList.end ();
      i++
    ) {
      // browse the element
      msrBrowser<msrVoiceElement> browser (v);
      browser.browse (*(*i));
    } // for
  }

  if (fVoiceLastSegment) {
    // browse the voice last segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fVoiceLastSegment);
  }

  // browse the voice stanzas
  if (fVoiceStanzasMap.size ()) {
    for (
      map<string, S_msrStanza>::const_iterator i = fVoiceStanzasMap.begin ();
      i != fVoiceStanzasMap.end ();
      i++
    ) {
      S_msrStanza stanza = (*i).second;

      if (stanza->getStanzaTextPresent ()) {
        // browse the stanza
        msrBrowser<msrStanza> browser (v);
        browser.browse (*(stanza));
      }
    } // for
  }

  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% <== msrVoice::browseData ()" <<
      endl;
  }
}

string msrVoice::voiceKindAsString (
  msrVoiceKind voiceKind)
{
  string result;
  
  switch (voiceKind) {
    case msrVoice::kRegularVoice:
      result = "regularVoice";
      break;
    case msrVoice::kHarmonyVoice:
      result = "harmonyVoice";
      break;
    case msrVoice::kFiguredBassVoice:
      result = "figured bassVoice";
      break;
  } // switch

  return result;
}

string msrVoice::voiceNumberAsString () const
{
  string result;
  
  switch (fVoiceNumber) {
    case K_PART_FIGURED_BASS_VOICE_NUMBER:
      result = "K_PART_FIGURED_BASS_VOICE_NUMBER";
      break;
    default:
      result = to_string (fVoiceNumber);
  } // switch
  
  return result;
}

string msrVoice::regularVoiceStaffSequentialNumberAsString () const
{
  string result;
  
  switch (fRegularVoiceStaffSequentialNumber) {
    case K_PART_FIGURED_BASS_VOICE_NUMBER:
      result = "K_PART_FIGURED_BASS_VOICE_NUMBER";
      break;
    default:
      result = to_string (fRegularVoiceStaffSequentialNumber);
  } // switch
  
  return result;
}
         
string msrVoice::voiceFinalizationStatusKindAsString (
  msrVoiceFinalizationStatusKind voiceFinalizationStatusKind)
{
  string result;
  
  switch (voiceFinalizationStatusKind) {
    case msrVoice::kKeepVoice:
      result = "keepVoice";
      break;
    case msrVoice::kEraseVoice:
      result = "eraseVoice";
      break;
  } // switch

  return result;
}  

string msrVoice::voiceKindAsString () const
{
  return voiceKindAsString (fVoiceKind);
}
      
string msrVoice::asShortString () const
{
  stringstream s;

  s <<
    "Voice \"" << getVoiceName () << "\", " <<
    voiceKindAsString (fVoiceKind) <<
    ", line " << fInputLineNumber;

  return s.str ();
}
         
string msrVoice::asString () const
{
  stringstream s;

  s <<
    "Voice \"" << getVoiceName () << "\", " <<
    voiceKindAsString (fVoiceKind) <<
     ", " <<
    singularOrPlural (
      fVoiceActualNotesCounter, "actual note", "actual notes") <<
     ", " <<
    singularOrPlural (
      fVoiceStanzasMap.size (), "stanza", "stanzas") <<
    ", line " << fInputLineNumber;

  return s.str ();
}
         
void msrVoice::print (ostream& os)
{
  os <<
    "Voice \"" << getVoiceName () << "\", " <<
    voiceKindAsString (fVoiceKind) <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  os << left <<
    "(" <<
    singularOrPlural (
      fVoiceActualHarmoniesCounter, "harmony", "harmonies") <<
     ", " <<
    singularOrPlural (
      fVoiceActualNotesCounter, "actual note", "actual notes") <<
     ", " <<
    singularOrPlural (
      fVoiceRestsCounter, "rest", "rests") <<
     ", " <<
    singularOrPlural (
      fVoiceSkipsCounter, "skip", "skips") <<
     ", " <<
    singularOrPlural (
      fVoiceStanzasMap.size (), "stanza", "stanzas") <<
    ")" <<
    endl;

  const int fieldWidth = 34;

  os << left <<
    setw (fieldWidth) << "staffUplink" << " : " <<
    fVoiceStaffUplink->getStaffName () <<
    endl <<
    setw (fieldWidth) << "voiceNumber" << " : " <<
    voiceNumberAsString () <<
    endl <<
    setw (fieldWidth) << "regularVoiceStaffSequentialNumber" << " : " <<
    regularVoiceStaffSequentialNumberAsString () <<
    endl;

  // print the harmony voice name if any
  os << left <<
    setw (fieldWidth) << "harmonyVoiceForRegularVoice" << " : ";
  if (fHarmonyVoiceForRegularVoice) {    
    os <<
      fHarmonyVoiceForRegularVoice->getVoiceName ();
  }
  else {
    os <<
      "none";
  }
  os <<
    endl;

  os << left <<
    setw (fieldWidth) << "voiceShortestNoteDuration" << " : " <<
    fVoiceShortestNoteDuration <<
    endl <<
    setw (fieldWidth) << "voiceShortestNoteTupletFactor" << " : " <<
    fVoiceShortestNoteTupletFactor <<
    endl;
    
  os << left <<
    setw (fieldWidth) << "musicHasBeenInsertedInVoice" << " : " <<
    booleanAsString (fMusicHasBeenInsertedInVoice) <<
    endl <<
    setw (fieldWidth) << "voiceContainsMultipleRests" << " : " <<
    booleanAsString (fVoiceContainsMultipleRests) <<
    endl;
    
  // print the voice first segment if any
  os <<
    setw (fieldWidth) << "voiceFirstSegment" << " : ";
  if (fVoiceFirstSegment) {
    os <<
      "'" <<
      fVoiceFirstSegment->getSegmentAbsoluteNumber () <<
      "'";
    }
  else {
    os <<
      "none";
  }
  os <<
    endl;

  // print the voice first measure if any
  os <<
    setw (fieldWidth) << "voiceFirstMeasure" << " : ";
  if (fVoiceFirstMeasure) {
    os <<
      "'" <<
      fVoiceFirstMeasure->asShortString () <<
      "'";
    }
  else {
    os <<
      "none";
  }
  os <<
    endl;

  // print this voice's first non-grace note
  {
    S_msrNote
      voiceFirstNonGraceNote =
        this->
          fetchVoiceFirstNonGraceNote ();
        
    os <<
      setw (fieldWidth) <<
      "voiceFirstNonGraceNote" << " : ";
    if (voiceFirstNonGraceNote) {
      os <<
        endl;
  
      gIndenter++;
  
      os << gTab <<
        voiceFirstNonGraceNote->asString ();
  
      gIndenter--;
    }
    else {
      os <<
        "none" <<
        endl;
    }
    os <<
      endl;
  }
  
  // print the voice last appended note
  os <<
    setw (fieldWidth) <<
    "voiceLastAppendedNote" << " : ";
  if (fVoiceLastAppendedNote) {
    os <<
      endl;

    gIndenter++;

    os << gTab <<
      fVoiceLastAppendedNote->asString ();

    gIndenter--;
  }
  else {
    os <<
      "none";
  }
  os <<
    endl;

  // print the voice measures flat list
  int voiceMeasuresFlatListSize =
    fVoiceMeasuresFlatList.size ();
    
  os <<
    setw (fieldWidth) <<
    "voiceMeasuresFlatList";
  if (voiceMeasuresFlatListSize) {
    os <<
      " : " <<  voiceMeasuresFlatListSize << " elements";
  }
  else {
    os <<
      " : " << "empty";
  }
  os <<
    endl;
    
  if (voiceMeasuresFlatListSize) {
    gIndenter++;

    list<S_msrMeasure>::const_iterator
      iBegin = fVoiceMeasuresFlatList.begin (),
      iEnd   = fVoiceMeasuresFlatList.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      // print the measure
      os <<
        (*i)->asShortString () <<
        endl;
      if (++i == iEnd) break;
   //   os << endl;
    } // for

    gIndenter--;
  }
  
  // print the voice initial elements
  int voiceInitialElementsListSize =
    fInitialVoiceElementsList.size ();
    
  os <<
    endl <<
    setw (fieldWidth) <<
    "voiceInitialElementsList";
  if (voiceInitialElementsListSize) {
    os << " : " <<  voiceInitialElementsListSize << " elements";
  }
  else {
    os << " : " << "none";
  }
  os <<
    endl;
    
  if (voiceInitialElementsListSize) {
    os <<
      endl;
      
    gIndenter++;

    list<S_msrVoiceElement>::const_iterator
      iBegin = fInitialVoiceElementsList.begin (),
      iEnd   = fInitialVoiceElementsList.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      // print the element
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }
  
  /* JMI
  // sanity check
  msrAssert (
    fVoiceLastSegment != nullptr,
    "fVoiceLastSegment is null");
    */

  // print the last segment
  if (fVoiceLastSegment) {
    os <<
      endl <<
      setw (fieldWidth) <<
      "voiceLastSegment" <<
      endl;
      
    gIndenter++;
    os <<
      fVoiceLastSegment <<
      endl;
    gIndenter--;
  }
  else {
    os <<
      "*** voiceLastSegment is null ***" << // JMI
      endl;
  }
  
  // print the stanzas if any
  if (fVoiceStanzasMap.size ()) {
    os <<
      "Stanzas:" <<
      endl;
      
    gIndenter++;
    
    map<string, S_msrStanza>::const_iterator
      iBegin = fVoiceStanzasMap.begin (),
      iEnd   = fVoiceStanzasMap.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      os << (*i).second;
      if (++i == iEnd) break;
      // no endl here
    } // for

    gIndenter--;
  }
  
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrVoice& elt)
{
  elt->print (os);
  return os;
}


}


    /*
  if (fOnGoingRepeat) {
    msrMusicXMLError (
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      "unterminated repeat in MusicXML data, exiting");

    msrMusicXMLWarning (
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
      "unterminated repeat in MusicXML data, ignoring the repeat altogether");

    // let's recover from this error

    // create an extra barline
    S_msrBarline
      barline =
        msrBarline::create (
          inputLineNumber,
          msrBarline::kBarlineCategoryRepeatEnd, // JMI
          msrBarline::kBarlineHasSegnoNo,
          msrBarline::kBarlineHasCodaNo,
          msrBarline::kBarlineLocationRight,
          msrBarline::kBarlineStyleNone,
          msrBarline::kBarlineEndingTypeStop,
          "0", // JMI
          msrBarline::kBarlineRepeatDirectionBackward,
          msrBarline::kBarlineRepeatWingedNone,
          2); // JMI
  
#ifdef TRACE_OPTIONS
    if (gGeneralOptions->fTraceBarlines) {
      fLogOutputStream <<
        "Creating an extra barline in part " <<
        fCurrentPart->getPartCombinedName () << ":" <<
        endl;
        
      gIndenter++;
      
      fLogOutputStream <<
        barline;
        
      gIndenter--;
    }
#endif
*/

/* JMI
  // prepend an implicit bar line to the part if needed
  if (
    ! fOnGoingRepeat
      &&
    gMsrOptions->fCreateImplicitInitialRepeatBarline
  ) {
    createAndPrependImplicitBarLine (
      inputLineNumber);
  }
*/


  /* JMI
#ifdef TRACE_OPTIONS
        if (gGeneralOptions->fTraceRepeats) {
          gLogIOstream <<
            "Fetching the last measure in voice clone \"" <<
            getVoiceName () <<
            "\" and removing it" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif
        
        S_msrMeasure
          endingFirstMeasure =
            removeLastMeasureFromVoice (
              inputLineNumber);
*/

/* JMI
        // fetch last measure's full measure whole notes
        int fullMeasureWholeNotes =
          fVoiceLastSegment->
            getSegmentMeasuresList ().back ()->
                getFullMeasureWholeNotes ();
              
        // finalize current measure in voice
        finalizeCurrentMeasureInVoice (
          inputLineNumber);
          */
 

/* JMI
    void                      createAndPrependImplicitBarLine (
                                int inputLineNumber);

void mxmlTree2MsrTranslator::createAndPrependImplicitBarLine (
  int inputLineNumber)
{     
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceBarlines || gGeneralOptions->fTraceRepeats) {
    fLogOutputStream <<
      "Prepending an implicit repeat start barline at the beginning of part" <<
      fCurrentPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // fetch current voice
  S_msrVoice
    currentVoice =
      fetchVoiceFromPart (
        inputLineNumber,
        fCurrentMusicXMLStaffNumber,
        fCurrentMusicXMLVoiceNumber);

  // create the implicit barline
  S_msrBarline
    implicitBarline =
      msrBarline::create (
        inputLineNumber,
        msrBarline::kBarlineCategoryRepeatStart,
        msrBarline::kBarlineHasSegnoNo,
        msrBarline::kBarlineHasCodaNo,
        msrBarline::kBarlineLocationLeft,
        msrBarline::kBarlineStyleHeavyLight,
        msrBarline::kBarlineEndingTypeStart,
        fCurrentBarlineEndingNumber,
        msrBarline::kBarlineRepeatDirectionForward,
        fCurrentBarlineRepeatWingedKind,
        fCurrentBarlineTimes);

  // prepend the implicit barline to the voice
  gIndenter++;
  
  currentVoice->
    prependBarlineToVoice (implicitBarline);

  gIndenter--;
}
 */
 
/*
  else {
    // no, there is an implicit repeat starting at the beginning of the part,
    // that encloses everything from the beginning on
*/

/*
    // append an implicit repeat to the current part
#ifdef TRACE_OPTIONS
    if (gGeneralOptions->fTraceRepeats) {
      fLogOutputStream <<
        "Prepending an implicit barline ahead of part " <<
        fCurrentPart->getPartCombinedName () <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    createAndPrependImplicitBarLine (
      inputLineNumber);
}
*/

  
  /* JMI
stringstream s;

s <<
  "repeats stack is empty when attempting to handle a repeat ending start in voice '" <<
  asShortString () <<
  "' ";
  
msrInternalError (
  gGeneralOptions->fInputSourceName,
  fInputLineNumber,
  __FILE__, __LINE__,
  s.str ());
  */



    /*
      // fetch last measure in the last segment
      S_msrMeasure
        lastMeasureInLastSegment =
          voiceLastSegmentMeasuresList.back ();
   
      // finalize lastMeasureInLastSegment
      lastMeasureInLastSegment->
        finalizeMeasure (
          inputLineNumber);
  
      // let's look at the last measure in detail
    
      // fetch its elements list
      const list<S_msrMeasureElement>&
        lastMeasureElementsList =
          lastMeasureInLastSegment->
            getMeasureElementsList ();

    // JMI     if (lastMeasureElementsList.size ()) {
      switch (lastMeasureInLastSegment->getMeasureKind ()) {
        case msrMeasure::kEmptyMeasureKind:
          {
            // the last measure is empty:
            // keep it for a new voice last segment
    
#ifdef TRACE_OPTIONS
            if (
              gGeneralOptions->fTraceMeasures
                ||
              gGeneralOptions->fTraceSegments
                ||
              gGeneralOptions->fTraceRepeats
            ) {
              stringstream s;
          
              gLogIOstream <<
                "Removing empty last measure '" <<
                lastMeasureInLastSegment->getMeasureNumber () <<
                "' from last segment '" <<
                asString () <<
                "' in voice \"" <<
                getVoiceName () <<
                "\"";
            }
#endif
            fVoiceLastSegment->
              removeLastMeasureFromSegment (
                inputLineNumber);
                */

    /*
            // append last segment to repeatCommonPart
            repeatCommonPart->
              appendSegmentToRepeatCommonPart (
                inputLineNumber,
                fVoiceLastSegment,
                "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice()");
    
            // create a new last segment containing a new measure for the voice
#ifdef TRACE_OPTIONS
            if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices) {
              gLogIOstream <<
                "Creating a new last segment with the empty measure '" <<
                lastMeasureInLastSegment->asShortString () <<
                "' in voice \"" <<
                fVoiceName << "\"" <<
                ", line " << inputLineNumber <<
                endl;
            }
#endif
    
            createNewLastSegmentFromFirstMeasureForVoice (
              inputLineNumber,
              lastMeasureInLastSegment,
              "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice()");
*/

