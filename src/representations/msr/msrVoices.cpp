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

#include "waeMessagesHandling.h"

#include "msrVoices.h"

#include "msrStaves.h"
#include "msrRepeats.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "mxmlTreeOah.h"
#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
string voiceRepeatPhaseKindAsString (
  msrVoiceRepeatPhaseKind
    afterRepeatComponentPhaseKind)
{
  string result;

  switch (afterRepeatComponentPhaseKind) {
    case msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseNone:
      result = "voiceRepeatPhaseNone";
      break;
    case msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterCommonPart:
      result = "voiceRepeatPhaseAfterCommonPart";
      break;
    case msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterHookedEnding:
      result = "voiceRepeatPhaseAfterHookedEnding";
      break;
    case msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterHooklessEnding:
      result = "voiceRepeatPhaseAfterHooklessEnding";
      break;
  } // switch

  return result;
}

string voiceFinalizationStatusKindAsString (
  msrVoiceFinalizationStatusKind voiceFinalizationStatusKind)
{
  string result;

  switch (voiceFinalizationStatusKind) {
    case msrVoiceFinalizationStatusKind::kKeepVoice:
      result = "keepVoice";
      break;
    case msrVoiceFinalizationStatusKind::kEraseVoice:
      result = "eraseVoice";
      break;
  } // switch

  return result;
}

string voiceFinalizationStatusKindAsString (
  msrVoiceCreateInitialLastSegmentKind voiceCreateInitialLastSegmentKind)
{
  string result;

  switch (voiceCreateInitialLastSegmentKind) {
    case msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes:
      break;
    case msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentNo:
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
S_msrRepeatDescr msrRepeatDescr::create (
  int         repeatDescrStartInputLineNumber,
  S_msrRepeat repeatDescrRepeat)
{
  msrRepeatDescr* o = new
    msrRepeatDescr (
      repeatDescrStartInputLineNumber,
      repeatDescrRepeat);
  assert (o != nullptr);
  return o;
}

msrRepeatDescr::msrRepeatDescr (
  int         repeatDescrStartInputLineNumber,
  S_msrRepeat repeatDescrRepeat)
{
  fRepeatDescrStartInputLineNumber =
    repeatDescrStartInputLineNumber;
  fRepeatDescrRepeat = repeatDescrRepeat;
}

msrRepeatDescr::~msrRepeatDescr ()
{}

string msrRepeatDescr::repeatDescrAsString () const
{
  stringstream s;

  s <<
    "fRepeatDescrRepeat = '" <<
    fRepeatDescrRepeat->asShortString () <<
    "', fRepeatDescrStartInputLineNumber = " <<
    fRepeatDescrStartInputLineNumber;

  return s.str ();
}

void msrRepeatDescr::print (ostream& os) const
{
  const unsigned int fieldWidth = 29;

  os << left <<
    setw (fieldWidth) <<
    "repeatDescrRepeat" << " : " <<
    fRepeatDescrRepeat <<
    endl <<
    setw (fieldWidth) <<
    "repeatDescrStartInputLineNumber" << " : " <<
    fRepeatDescrStartInputLineNumber <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrRepeatDescr& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
// global variable
int msrVoice::gVoicesCounter = 0;

S_msrVoice msrVoice::create (
  int          inputLineNumber,
  msrVoiceKind voiceKind,
  int          voiceNumber,
  msrVoiceCreateInitialLastSegmentKind
               voiceCreateInitialLastSegmentKind,
  S_msrStaff   voiceStaffUpLink)
{
  msrVoice* o =
    new msrVoice (
      inputLineNumber,
      voiceKind,
      voiceNumber,
      voiceCreateInitialLastSegmentKind,
      voiceStaffUpLink);
  assert (o != nullptr);

  return o;
}

S_msrVoice msrVoice::createRegularVoice (
  int        inputLineNumber,
  int        voiceNumber,
  S_msrStaff voiceStaffUpLink)
{
  return
    msrVoice::create (
      inputLineNumber,
      msrVoiceKind::kVoiceRegular,
      voiceNumber,
      msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes,
        // the initial last segment is ready to receive music
      voiceStaffUpLink);
}

S_msrVoice msrVoice::createHarmonyVoice (
  int        inputLineNumber,
  int        voiceNumber,
  S_msrStaff voiceStaffUpLink)
{
  return
    msrVoice::create (
      inputLineNumber,
      msrVoiceKind::kVoiceHarmony,
      voiceNumber,
      msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes,
        // the initial last segment is ready to receive music
      voiceStaffUpLink);
}

S_msrVoice msrVoice::createFiguredBassVoice (
  int        inputLineNumber,
  int        voiceNumber,
  S_msrStaff voiceStaffUpLink)
{
  return
    msrVoice::create (
      inputLineNumber,
      msrVoiceKind::kVoiceFiguredBass,
      voiceNumber,
      msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes,
        // the initial last segment is ready to receive music
      voiceStaffUpLink);
}

msrVoice::msrVoice (
  int          inputLineNumber,
  msrVoiceKind voiceKind,
  int          voiceNumber,
  msrVoiceCreateInitialLastSegmentKind
               voiceCreateInitialLastSegmentKind,
  S_msrStaff   voiceStaffUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    voiceStaffUpLink != nullptr,
    "voiceStaffUpLink is null");

  // set voice staff upLink
  fVoiceStaffUpLink = voiceStaffUpLink;

  // set voice kind
  fVoiceKind = voiceKind;

  // set voice number
  fVoiceNumber = voiceNumber;

  // do other initializations
  initializeVoice (
    voiceCreateInitialLastSegmentKind);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Creating voice \"" << asString () << "\"" <<
      endl;
  }
#endif
}

msrVoice::msrVoice (
  int          inputLineNumber,
  msrVoiceKind voiceKind,
  int          voiceNumber)
    : msrElement (inputLineNumber)
{
  // set voice staff upLink
  fVoiceStaffUpLink = nullptr;

  // set voice kind
  fVoiceKind = voiceKind;

  // set voice number
  fVoiceNumber = voiceNumber;

  // do other initializations
  initializeVoice (
    msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes); // JMI

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Creating voice \"" << asString () << "\"" <<
      endl;
  }
#endif
}

msrVoice::~msrVoice ()
{}

S_msrPart msrVoice::fetchVoicePartUpLink () const
{
  S_msrPart result;

  if (fVoiceStaffUpLink) {
    result =
      fVoiceStaffUpLink->
        getStaffPartUpLink ();
  }

  return result;
}

S_msrPartGroup msrVoice::fetchVoicePartGroupUpLink () const
{
  S_msrPartGroup result;

  if (fVoiceStaffUpLink) {
    result =
      fVoiceStaffUpLink->
        fetchStaffPartGroupUpLink ();
  }

  return result;
}

S_msrScore msrVoice::fetchVoiceScoreUpLink () const
{
  S_msrScore result;

  if (fVoiceStaffUpLink) {
    result =
      fVoiceStaffUpLink->
        fetchStaffScoreUpLink ();
  }

  return result;
}

void msrVoice::setRegularVoiceStaffSequentialNumber (
  int regularVoiceStaffSequentialNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Setting the regular voice staff sequential number of voice \"" <<
      getVoiceName () <<
      "\" to " << regularVoiceStaffSequentialNumber <<
      endl;
  }
#endif

  fRegularVoiceStaffSequentialNumber =
    regularVoiceStaffSequentialNumber;
}

void msrVoice::setVoiceNameFromNumber (
  int inputLineNumber,
  int voiceNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Setting the name of '" <<
      msrVoiceKindAsString (fVoiceKind) <<
      "' voice from voice number '" << voiceNumber <<
      "'" <<
      endl;
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
      fVoiceName =
        fVoiceStaffUpLink->getStaffName () +
        "_Voice_" +
        int2EnglishWord (voiceNumber);
      break;

    case msrVoiceKind::kVoiceHarmony:
      fVoiceName =
        fVoiceStaffUpLink->getStaffName () +
        "_Voice_" +
        int2EnglishWord (
          voiceNumber) + // JMI - K_VOICE_HARMONY_VOICE_BASE_NUMBER) +
          "_HARMONY";
      break;

    case msrVoiceKind::kVoiceFiguredBass:
      fVoiceName =
        fVoiceStaffUpLink->getStaffName () +
        "_Voice_" +
        int2EnglishWord (
          voiceNumber) + // JMI - K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER) +
          "_FIGURED_BASS";
/* JMI
      {
        stringstream s;

        s <<
          "A '" <<
          getVoiceName () <<
          "' voice cannot get its name from its number";

        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      */
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "The resulting voice name is \"" <<
      fVoiceName <<
      "\"" <<
      endl;
  }
#endif
}

void msrVoice::setVoiceLastSegmentInVoiceClone (
  S_msrSegment segment)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Setting segment '" <<
      segment->asShortString () <<
      "' as last segment in voice clone \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << segment->getInputLineNumber () <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    segment != nullptr,
    "segment is null");

  fVoiceLastSegment = segment;

  if (! fVoiceFirstSegment) {
    fVoiceFirstSegment = fVoiceLastSegment;
  }
}

void msrVoice::appendSegmentToVoiceClone ( //JMI VIRER???
  S_msrSegment segment)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Appending segment '" <<
      segment->asString () <<
      "' to voice clone \"" <<
      fVoiceName <<
      "\"" <<
      endl;
  }
#endif

  int inputLineNumber =
    segment->getInputLineNumber ();

  // does segment belong to a repeat?
  if (fVoicePendingRepeatDescrsStack.size ()) {
    // yes

 // JMI   if (fVoiceLastSegment) {

    // append segment to whichever part of the repeat is adequate
    S_msrRepeat
      currentRepeat =
        fVoicePendingRepeatDescrsStack.front ()->
          getRepeatDescrRepeat ();

    currentRepeat->
      appendSegmentToRepeat (
        inputLineNumber,
        segment,
        "appendSegmentToVoiceClone()");
  }

  else {
    // no
  }

  {
/* JMI
    // move fVoiceLastSegment to the initial voice elements list
    moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
      inputLineNumber,
      "appendSegmentToVoiceClone()");
      */

    // segment becomes the fVoiceLastSegment
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceSegments ()) {
      gLogStream <<
        "Segment '" <<
        segment->asShortString () <<
        "' becomes the new last segment in voice clone \"" <<
        fVoiceName <<
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
  // in setRegularVoiceStaffSequentialNumber ()
  fRegularVoiceStaffSequentialNumber = -1;

  ++gIndenter;

  // compute voice number
  int voiceNumber =
    gGlobalMsrOahGroup->getCreateVoicesStaffRelativeNumbers () // JMI use it
      ? fRegularVoiceStaffSequentialNumber
      : fVoiceNumber;

  // set voice name
  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
      setVoiceNameFromNumber (
        fInputLineNumber,
        voiceNumber);
      break;

    case msrVoiceKind::kVoiceHarmony:
      setVoiceNameFromNumber (
        fInputLineNumber,
        voiceNumber);
      break;

    case msrVoiceKind::kVoiceFiguredBass:
      fVoiceName =
        fVoiceStaffUpLink->getStaffName () +
        "_FIGURED_BASS_Voice";
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Initializing voice \"" << fVoiceName <<
      "\" in staff \"" <<
      fVoiceStaffUpLink->getStaffName () <<
      "\"" <<
      endl;
  }
#endif

  // check voice number
  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
      // the voice number should be positive
      if (fVoiceNumber < 1 || fVoiceNumber > 4) {
        stringstream s;

        s <<
          "regular voice number " << fVoiceNumber <<
          " is not in the 1..4 range";

          /* JMI ???
        msrMusicXMLError (
          gGlobalOahOahGroup->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
          */
      }
      break;

    case msrVoiceKind::kVoiceHarmony:
      break;

    case msrVoiceKind::kVoiceFiguredBass:
    /* JMI
      if (fVoiceNumber != K_PART_FIGURED_BASS_VOICE_NUMBER) {
        stringstream s;

        s <<
          "figured bass voice number " << fVoiceNumber <<
          " is not equal to " << K_PART_FIGURED_BASS_VOICE_NUMBER;

        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      */
      break;
  } // switch

  // voice shortest note
  fVoiceShortestNoteDuration =
    rational (INT_MAX, 1);
  fVoiceShortestNoteTupletFactor =
    rational (1, 1);

  // set voice current measure number
  fVoiceCurrentMeasureNumber = // JMI "??";
    fetchVoicePartUpLink ()->
      getPartCurrentMeasureNumber ();

  // set voice  current ordinal measure number
  fVoiceCurrentMeasureOrdinalNumber = 0;

  // set voice current measure purist number
  fVoiceCurrentMeasurePuristNumber = 1; // default value,
                                        // supposing there's no anacrusis

  // music has not been inserted in voice yet
  fMusicHasBeenInsertedInVoice = false;

  // counters
  fVoiceActualNotesCounter         = 0;
  fVoiceRestsCounter               = 0;
  fVoiceSkipsCounter               = 0;
  fVoiceActualHarmoniesCounter     = 0;
  fVoiceActualFiguredBassesCounter = 0;

  // regular measure ends detection
  fWholeNotesSinceLastRegularMeasureEnd = rational (0, 1);

  // set voice current after repeat component phase kind
  setCurrentVoiceRepeatPhaseKind (
    fInputLineNumber,
    msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseNone);

  // rest measures
  fVoiceContainsRestMeasures  = false;
  fVoiceRemainingRestMeasures = 0;

  // measures repests
  fVoiceContainsMeasuresRepeats = false;

    // position in voice
  fCurrentPositionInVoice = rational (0, 1);
  fCurrentMomentInVoice = msrMoment (rational (0,1), rational (0,1));

  // voice finalization
  fVoiceHasBeenFinalized = false;

  // create the initial last segment if requested
  switch (voiceCreateInitialLastSegmentKind) {
    case msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes:
      // sanity check // JMI LAST
      msgAssert (
        __FILE__, __LINE__,
        fVoiceLastSegment == nullptr,
        "fVoiceLastSegment is null");

      // create the last segment
      fVoiceLastSegment =
        msrSegment::create (
          fInputLineNumber,
          this);

      if (! fVoiceFirstSegment) {
        fVoiceFirstSegment = fVoiceLastSegment;
      }
      break;
    case msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentNo:
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Initial contents of voice \"" << fVoiceName <<
      "\" in staff \"" <<
      fVoiceStaffUpLink->getStaffName () <<
      "\":" <<
      endl;

    ++gIndenter;

    this->print (gLogStream);

    --gIndenter;
  }
#endif

  --gIndenter;
}

void msrVoice::changeVoiceIdentity ( // after a deep copy
  int voiceNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Changing identity of voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", number: " << voiceNumber <<
      endl;
  }
#endif

  // make it a regular voice
  setVoiceKind (
    msrVoiceKind::kVoiceRegular);

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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Creating a newborn clone of voice \"" <<
      getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    staffClone != nullptr,
    "staffClone is null");

  S_msrVoice
    newbornClone =
      msrVoice::create (
        fInputLineNumber,
        fVoiceKind,
        fVoiceNumber,
        msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentNo,
          // initial segment will be created upon a later segment visit
        staffClone);

  // voice numbers
  newbornClone->fRegularVoiceStaffSequentialNumber =
    fRegularVoiceStaffSequentialNumber;

  // voice name
  newbornClone->fVoiceName =
    fVoiceName;

  return newbornClone;
}

S_msrVoice msrVoice::createVoiceDeepCopy (
  int          inputLineNumber,
  msrVoiceKind voiceKind,
  int          voiceNumber,
  S_msrStaff   containingStaff)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
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
  msgAssert (
    __FILE__, __LINE__,
    containingStaff != nullptr,
    "containingStaff is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoicesDetails ()) {
    gLogStream <<
      "****" <<
      " BEFORE voiceDeepCopy = " <<
      endl;

    print (gLogStream);

    gLogStream <<
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
        msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentNo,
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

  voiceDeepCopy->fVoiceActualFiguredBassesCounter =
    fVoiceActualFiguredBassesCounter;

  // measures
  voiceDeepCopy->fVoiceCurrentMeasureNumber =
    fVoiceCurrentMeasureNumber;
  voiceDeepCopy->fVoiceCurrentMeasureOrdinalNumber =
    fVoiceCurrentMeasureOrdinalNumber;
  voiceDeepCopy->fVoiceCurrentMeasurePuristNumber =
    fVoiceCurrentMeasurePuristNumber;

  // musically empty voices
  voiceDeepCopy->fMusicHasBeenInsertedInVoice =
    fMusicHasBeenInsertedInVoice;

  // regular measure ends detection
  voiceDeepCopy->fWholeNotesSinceLastRegularMeasureEnd =
    fWholeNotesSinceLastRegularMeasureEnd;

  // incomplete measures after repeats detection
  voiceDeepCopy->fCurrentVoiceRepeatPhaseKind =
    fCurrentVoiceRepeatPhaseKind;

  // initial elements
  int numberOfInitialElements =
    fInitialVoiceElementsList.size ();

  if (numberOfInitialElements) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceVoices ()) {
      gLogStream <<
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
          gGlobalOahOahGroup->getInputSourceName (),
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
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceVoices ()) {
      gLogStream <<
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
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceVoices ()) {
      gLogStream <<
        "There is no last segment in voice \"" <<
        getVoiceName () <<
        "\" to be deep copied" <<
        endl;
    }
#endif
  }

  // rest measures
  voiceDeepCopy->fVoiceContainsRestMeasures =
    fVoiceContainsRestMeasures;

  // measures reeats
  voiceDeepCopy->fVoiceContainsMeasuresRepeats =
    fVoiceContainsMeasuresRepeats;

  // stanzas
  for (
    map<string, S_msrStanza>::const_iterator i = fVoiceStanzasMap.begin ();
    i != fVoiceStanzasMap.end ();
    ++i
  ) {
    S_msrStanza stanza = (*i).second;

    voiceDeepCopy->
      addStanzaToVoiceWithoutCatchUp ( // JMI
        stanza->
          createStanzaDeepCopy (
            voiceDeepCopy));
  } // for

  // upLinks
  voiceDeepCopy->fVoiceStaffUpLink =
    containingStaff;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoicesDetails ()) {
    gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Setting next measure number to '" <<
      nextMeasureNumber <<
      ", in voice \"" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  fVoiceLastSegment->
    setNextMeasureNumberInSegment (
      inputLineNumber,
      nextMeasureNumber);

  // is there a pending rest measures in this voice?
  if (fVoiceRestMeasuresWaitingForItsNextMeasureNumber) {
    // yes
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
      gLogStream <<
        "There is a rest measures waiting for its next measure number" <<
        ", fVoiceRemainingRestMeasures = " <<
        fVoiceRemainingRestMeasures <<
        "' in voice \"" <<
        fVoiceName << "\"" <<
        endl;
    }
#endif

    --fVoiceRemainingRestMeasures;

    // is this the last measure in the row?
    if (fVoiceRemainingRestMeasures == 0) {
      // yes, set waiting rest measures's next measure number
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
        gLogStream <<
          "Setting rest measures next measure number to '" <<
          nextMeasureNumber <<
          "' in voice \"" <<
          fVoiceName << "\"" <<
          endl;
      }
#endif

      fVoiceRestMeasuresWaitingForItsNextMeasureNumber->
        setRestMeasuresNextMeasureNumber (
          nextMeasureNumber);

      // forget about this waiting rest measures
      fVoiceRestMeasuresWaitingForItsNextMeasureNumber = nullptr;
    }
  }

  --gIndenter;
}

void msrVoice::incrementVoiceCurrentMeasurePuristNumber (
  int    inputLineNumber,
  string context)
{
  if (fVoiceCurrentMeasurePuristNumber < 0) {
    fVoiceCurrentMeasurePuristNumber = 0;
    fVoiceFirstMeasurePuristNumber = fVoiceCurrentMeasurePuristNumber;
  }
  else {
    ++fVoiceCurrentMeasurePuristNumber;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Incrementing voice current measure purist number to '" <<
      fVoiceCurrentMeasurePuristNumber <<
      "' (" << context << ")" <<
      " in voice \"" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif
}

void msrVoice::appendMeasureCloneToVoiceClone (
  int          inputLineNumber,
  S_msrMeasure measureClone)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Appending measure clone '" <<
      measureClone->asShortString () <<
      "' to voice '" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // append measureClone to the voice last segment
  fVoiceLastSegment->
    appendMeasureToSegment (
      measureClone);

  // measureClone is the new voice last appended measure
  fVoiceLastAppendedMeasure = measureClone;

  --gIndenter;
}

void msrVoice::setWholeNotesSinceLastRegularMeasureEnd (
  int      inputLineNumber,
  rational value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Setting voice whole notes since last regular measure end to '" <<
      value <<
  // JMI    "' (" << context << ")" <<
      " in voice \"" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  fWholeNotesSinceLastRegularMeasureEnd = value;
}

void msrVoice::setCurrentVoiceRepeatPhaseKind (
  int      inputLineNumber,
  msrVoiceRepeatPhaseKind
           afterRepeatComponentPhaseKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Setting voice current after repeat component phase kind to '" <<
      voiceRepeatPhaseKindAsString (
        afterRepeatComponentPhaseKind) <<
 // JMI     "' (" << context << ")" <<
      " in voice \"" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  fCurrentVoiceRepeatPhaseKind =
    afterRepeatComponentPhaseKind;
}

void msrVoice::createNewLastSegmentForVoice (
  int    inputLineNumber,
  string context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Creating a new last segment for voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (gGlobalTraceOahGroup->getTraceVoicesDetails ()) {
    string
      combinedContext =
        "createNewLastSegmentForVoice() 1 called from " + context;

    displayVoice (
      inputLineNumber,
      combinedContext);
  }
#endif

  // create the last segment
  fVoiceLastSegment =
    msrSegment::create (
      inputLineNumber,
      this);

  if (! fVoiceFirstSegment) {
    fVoiceFirstSegment = fVoiceLastSegment;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoicesDetails ()) {
    string
      combinedContext =
        "createNewLastSegmentForVoice() 2 called from " + context;

    displayVoice (
      inputLineNumber,
      combinedContext);
  }
#endif
}

void msrVoice::createNewLastSegmentFromItsFirstMeasureForVoice (
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Creating a new last segment '" <<
      fVoiceLastSegment->asShortString () <<
      "' from its first measure '" <<
      firstMeasure->getMeasureElementMeasureNumber () <<
      "' for voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // append firstMeasure to fVoiceLastSegment
  fVoiceLastSegment->
    appendMeasureToSegment (firstMeasure);

  // firstMeasure is the new voice last appended measure
  fVoiceLastAppendedMeasure = firstMeasure;

  // is firstMeasure the first one it the voice?
  if (! fVoiceFirstMeasure) {
    // yes, register it as such
    setVoiceFirstMeasure (
      firstMeasure);

    firstMeasure->
      setMeasureFirstInVoice ();
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresDetails ()) {
    displayVoice (
      inputLineNumber,
      "createNewLastSegmentFromItsFirstMeasureForVoice()");
  }
#endif
}

S_msrMeasure msrVoice::createMeasureAndAppendItToVoice (
  int    inputLineNumber,
  string measureNumber,
  msrMeasureImplicitKind
         measureImplicitKind)
{
  fVoiceCurrentMeasureNumber = measureNumber;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating measure '" <<
      measureNumber <<
      "' and appending it to voice \"" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresDetails ()) {
    displayVoice (
      inputLineNumber,
      "createMeasureAndAppendItToVoice() 1");
  }
#endif

  ++gIndenter;

  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
    createNewLastSegmentForVoice (
      inputLineNumber,
      "createMeasureAndAppendItToVoice() 2");
  }

  // append a new measure with given number to voice last segment
  S_msrMeasure
    result =
      fVoiceLastSegment->
        createMeasureAndAppendItToSegment (
          inputLineNumber,
          measureNumber,
          measureImplicitKind);

  // result is the new voice last appended measure
  fVoiceLastAppendedMeasure = result;

  // handle voice kind
  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
    /* JMI
      // fetch the part
      S_msrPart
        part =
          this->fetchVoicePartUpLink ();

      // fetch the part harmonies voice
      S_msrVoice
        partHarmoniesVoice =
          part->
            getPartHarmoniesVoice ();

      if (partHarmoniesVoice) {
        // append a new measure with given number to part harmonies voice
        partHarmoniesVoice ->
          createMeasureAndAppendItToVoice (
            inputLineNumber,
            measureNumber,
            measureImplicitKind);
      }

      // fetch the part figured bass voice
      S_msrVoice
        partFiguredBassVoice =
          part->
            getPartFiguredBassVoice ();

      if (partFiguredBassVoice) {
        // append a new measure with given number to part figured bass voice
        partFiguredBassVoice ->
          createMeasureAndAppendItToVoice (
            inputLineNumber,
            measureNumber,
            measureImplicitKind);
      }
      */

    /* JMI
      // append new measure with given number to voice harmony voice if any
      if (fRegularVoiceHarmonyVoiceForwardLink) {
        fRegularVoiceHarmonyVoiceForwardLink->
          createMeasureAndAppendItToVoice (
            inputLineNumber,
            measureNumber,
            measureImplicitKind);
      }
      // append new measure with given number to voice figured bass voice if any
      if (fRegularVoiceFiguredBassVoiceForwardLink) {
        fRegularVoiceFiguredBassVoiceForwardLink->
          createMeasureAndAppendItToVoice (
            inputLineNumber,
            measureNumber,
            measureImplicitKind);
      }
      */
      break;
    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:
      // JMI should not occur
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresDetails ()) {
    displayVoice (
      inputLineNumber,
      "createMeasureAndAppendItToVoice() 3");
  }
#endif

  --gIndenter;

  return result;
}

/* JMI
S_msrVoice msrVoice::createRegularVoiceHarmonyVoice (
  int    inputLineNumber,
  string currentMeasureNumber)
{
  if (fRegularVoiceHarmonyVoiceForwardLink) {
    stringstream s;

    s <<
      "Voice \"" <<
      getVoiceName () <<
      "\" already has a harmony voice";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create the voice harmony voice
  int regularVoiceHarmonyVoiceNumber =
    K_VOICE_HARMONY_VOICE_BASE_NUMBER + fVoiceNumber;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Creating harmony voice for regular voice \"" <<
      getVoiceName () <<
      "\" with voice number " <<
      regularVoiceHarmonyVoiceNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fRegularVoiceHarmonyVoiceForwardLink =
    msrVoice::create (
      inputLineNumber,
      msrVoiceKind::kVoiceHarmony,
      regularVoiceHarmonyVoiceNumber,
      msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes,
      fVoiceStaffUpLink);

  // register it in the staff
  fVoiceStaffUpLink->
    registerVoiceInStaff (
      inputLineNumber,
      fRegularVoiceHarmonyVoiceForwardLink);

  // set backward link
  fRegularVoiceHarmonyVoiceForwardLink->
    fHarmonyVoiceRegularVoiceBackwardLink = this;

  return fRegularVoiceHarmonyVoiceForwardLink;
}

S_msrVoice msrVoice::createRegularVoiceFiguredBassVoice (
  int    inputLineNumber,
  string currentMeasureNumber)
{
  if (fRegularVoiceFiguredBassVoiceForwardLink) {
    stringstream s;

    s <<
      "Voice \"" <<
      getVoiceName () <<
      "\" already has a figured bass voice";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create the voice figured bass voice
  int regularVoiceFiguredBassVoiceNumber =
    K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER + fVoiceNumber;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceFiguredBasses ()) {
    gLogStream <<
      "Creating figured bass voice for regular voice \"" <<
      getVoiceName () <<
      "\" with voice number " <<
      regularVoiceFiguredBassVoiceNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fRegularVoiceFiguredBassVoiceForwardLink =
    msrVoice::create (
      inputLineNumber,
      msrVoiceKind::kVoiceFiguredBass,
      regularVoiceFiguredBassVoiceNumber,
      msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes,
      fVoiceStaffUpLink);

  // register it in the staff
  fVoiceStaffUpLink->
    registerVoiceInStaff (
      inputLineNumber,
      fRegularVoiceFiguredBassVoiceForwardLink);

  // set backward link
  fRegularVoiceFiguredBassVoiceForwardLink->
    fFiguredBassVoiceRegularVoiceBackwardLink = this;

  return fRegularVoiceFiguredBassVoiceForwardLink;
}
*/

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
      gGlobalOahOahGroup->getInputSourceName (),
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceLyrics ()) {
    gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceLyrics ()) {
    gLogStream <<
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
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceLyrics ()) {
      gLogStream <<
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

    gLogStream <<
      endl << endl << endl <<
      "VOICEVOICEVOICEVOICEVOICEVOICEVOICEVOICE" <<
      endl;

    print (gLogStream);

    gLogStream <<
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
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return stanza;
}

void msrVoice::setVoiceCurrentClef (S_msrClef clef)
{
  fVoiceCurrentClef = clef;

  if (! fVoiceFirstClef) {
    fVoiceFirstClef = fVoiceCurrentClef;
  }
};

void msrVoice::setVoiceCurrentKey (S_msrKey key)
{
  fVoiceCurrentKey = key;
};

void msrVoice::setVoiceCurrentTime (S_msrTime time)
{
  fVoiceCurrentTime = time;
};

void msrVoice::appendPrintLayoutToVoice (S_msrPrintLayout printLayout)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePrintLayouts ()) {
    gLogStream <<
      "Appending print layout '" << printLayout->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  fVoiceLastSegment->
    appendPrintLayoutToSegment (printLayout);

  --gIndenter;
}

void msrVoice::appendClefToVoice (S_msrClef clef)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceClefs ()) {
    gLogStream <<
      "Appending clef '" << clef->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  // set voice current clef
  this->setVoiceCurrentClef (clef);

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

  --gIndenter;
}

void msrVoice::appendKeyToVoice (S_msrKey key)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Appending key " << key->asString () <<
      " to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  // set voice current clef
  this->setVoiceCurrentKey (key);

  // append key to last segment
  fVoiceLastSegment->
    appendKeyToSegment (key);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceKeys ()) {
    displayVoice ( // JMI TEMP
      key->getInputLineNumber (),
      "appendKeyToVoice()");
  }
#endif

  --gIndenter;
}

void msrVoice::appendTimeToVoice (S_msrTime time)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTimes ()) {
    gLogStream <<
      "Appending time '" << time->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  // set voice current time
  this->setVoiceCurrentTime (time);

  // append time to the last segment
  fVoiceLastSegment->
    appendTimeToSegment (time);

  --gIndenter;
}

void msrVoice::appendTimeToVoiceClone (S_msrTime time) // superflous ??? JMI
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTimes ()) {
    gLogStream <<
      "Appending time '" << time->asString () <<
      "' to voice clone \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  // set voice current time
  this->setVoiceCurrentTime (time);

  // append time to the last segment
  fVoiceLastSegment->
    appendTimeToSegmentClone (time);

  --gIndenter;
}

void msrVoice::insertHiddenMeasureAndBarlineInVoiceClone (
  int      inputLineNumber,
  rational positionInMeasure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceDalSegnos () || gGlobalTraceOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Inserting hidden measure and barline at position " <<
      positionInMeasure <<
      "' to voice clone \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // append time to the last segment
  fVoiceLastSegment->
    insertHiddenMeasureAndBarlineInSegmentClone (
      inputLineNumber,
      positionInMeasure);

  --gIndenter;
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
            // ignore this element and return nullptr JMI ???
            /*
            stringstream s;

            s <<
              "tuplet first element should be a note, a chord or another tuplet, found instead '" <<
              element->asShortString () <<
              "'";

            msrInternalError (
              gGlobalOahOahGroup->getInputSourceName (),
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

void msrVoice::setVoiceShortestNoteDuration (
  rational duration)
{
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceNotes ()) {
      gLogStream <<
        "The new shortest note duration in voice \"" <<
        getVoiceName () <<
        "\" becomes " <<
        duration <<
        endl;
    }
#endif

  fVoiceShortestNoteDuration = duration;
}

void msrVoice::setVoiceShortestNoteTupletFactor (
  const msrTupletFactor& noteTupletFactor)
{
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceNotes ()) {
      gLogStream <<
        "The new shortest note tuplet factor in part \"" <<
        getVoiceName () <<
        "\" becomes " <<
        noteTupletFactor <<
        endl;
    }
#endif

  fVoiceShortestNoteTupletFactor = noteTupletFactor;
}

void msrVoice::registerShortestNoteInVoiceIfRelevant (S_msrNote note)
{
  // is note the shortest one in this voice?
  rational
    noteSoundingWholeNotes =
      note->
        getNoteSoundingWholeNotes ();

      /* JMI
  rational
    noteDisplayWholeNotes =
      note->
        getNoteDisplayWholeNotes ();
        */

  if (noteSoundingWholeNotes < fVoiceShortestNoteDuration) {
    // set the voice shortest note duration
    this->
      setVoiceShortestNoteDuration (
        noteSoundingWholeNotes);

    // propagate this new value to the voice's part
    S_msrPart
      voicePartUplink =
        fetchVoicePartUpLink ();

    voicePartUplink ->
      registerShortestNoteInPartIfRelevant (
        note);

/* JMI
    // set the voice shortest note tuplet factor
    fVoiceShortestNoteTupletFactor =
      note->
        getNoteTupletFactor ();

    // propagate this new value to the voice's part
    voicePartUplink ->
      setPartShortestNoteTupletFactor (
        fVoiceShortestNoteTupletFactor);
*/

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceNotes ()) {
      gLogStream <<
        "The new shortest note in voice \"" << getVoiceName () << "\"" <<
        " becomes " << note->asString () <<
        endl;
    }
#endif
  }

/* JMI
  if (noteDisplayWholeNotes < fVoiceShortestNoteDuration) {
    fVoiceShortestNoteDuration = noteDisplayWholeNotes;
  }
    */
}

void msrVoice::registerNoteAsVoiceLastAppendedNote (S_msrNote note)
{
  fVoiceLastAppendedNote = note;

  // is note the shortest one in this voice?
  rational
    noteSoundingWholeNotes =
      note->
        getNoteSoundingWholeNotes (),
    noteDisplayWholeNotes =
      note->
        getNoteDisplayWholeNotes (); // JMI

  if (noteSoundingWholeNotes < fVoiceShortestNoteDuration) {
    fVoiceShortestNoteDuration = noteSoundingWholeNotes;
  }
  if (noteDisplayWholeNotes < fVoiceShortestNoteDuration) {
    fVoiceShortestNoteDuration = noteDisplayWholeNotes;
  }
}

void msrVoice::appendHarmonyToVoice (
  S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Appending harmony " << harmony->asString () <<
      " to voice \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // sanity check
  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceHarmony:
      appendHarmonyToHarmonyVoice (
        harmony);
      break;

    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceFiguredBass:
      {
        stringstream s;

        s <<
          "cannot append a harmony to " <<
          msrVoiceKindAsString (fVoiceKind) <<
          " voice \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

  --gIndenter;
}

void msrVoice::appendHarmonyToHarmonyVoice (
  S_msrHarmony harmony)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    harmony->getInputLineNumber ();

  if (gGlobalTraceOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Appending harmony " << harmony->asString () <<
      " to harmony voice \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // append the harmony to the voice last segment
  fVoiceLastSegment->
    appendHarmonyToSegment (harmony);

  // register harmony
  ++fVoiceActualHarmoniesCounter;
  fMusicHasBeenInsertedInVoice = true;

  --gIndenter;
}

void msrVoice::appendFiguredBassToFiguredBassVoice (
  S_msrFiguredBass figuredBass)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    figuredBass->getInputLineNumber ();

  if (gGlobalTraceOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Appending figuredBass " << figuredBass->asString () <<
      " to figuredBass voice \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // append the figuredBass to the voice last segment
  fVoiceLastSegment->
    appendFiguredBassToSegment (figuredBass);

  // register harmony
  ++fVoiceActualFiguredBassesCounter;
  fMusicHasBeenInsertedInVoice = true;

  --gIndenter;
}

void msrVoice::appendHarmonyToVoiceClone (S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Appending harmony " << harmony->asString () <<
      " to voice clone \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceHarmony:
      fVoiceLastSegment->
        appendHarmonyToSegmentClone (harmony);

      // register harmony
      ++fVoiceActualHarmoniesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceFiguredBass:
      {
        stringstream s;

        s <<
          "cannot append a harmony to " <<
          msrVoiceKindAsString (fVoiceKind) <<
          " voice clone \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

  --gIndenter;
}

void msrVoice::appendFiguredBassToVoice (
  S_msrFiguredBass figuredBass)
{
  int inputLineNumber =
    figuredBass->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceFiguredBasses ()) {
    gLogStream <<
      "Appending figured bass " << figuredBass->asString () <<
      " to voice \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // sanity check
  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceFiguredBass:
      appendFiguredBassToFiguredBassVoice (
        figuredBass);
      break;

    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
      {
        stringstream s;

        s <<
          "cannot append a figured bass to " <<
          msrVoiceKindAsString (fVoiceKind) <<
          " voice \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

  --gIndenter;
}

void msrVoice::appendFiguredBassToVoiceClone (
  S_msrFiguredBass figuredBass)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    figuredBass->getInputLineNumber ();

  if (gGlobalTraceOahGroup->getTraceFiguredBasses ()) {
    gLogStream <<
      "Appending figured bass " << figuredBass->asString () <<
      " to voice clone \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceFiguredBass:
      fVoiceLastSegment->
        appendFiguredBassToSegmentClone (figuredBass);

      // register figured bass
      ++fVoiceActualFiguredBassesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
      {
        stringstream s;

        s <<
          "cannot append a figured bass to " <<
          msrVoiceKindAsString (fVoiceKind) <<
          " voice clone \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          figuredBass->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

  --gIndenter;
}

void msrVoice::padUpToPositionInMeasureInVoice (
  int      inputLineNumber,
  rational wholeNotesPositionInMeasure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Padding up to position in measure '" <<
      wholeNotesPositionInMeasure <<
      "' whole notes in voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    fVoiceLastSegment != nullptr,
    "fVoiceLastSegment is null");

  // pad up the voice's last segment
  fVoiceLastSegment->
    padUpToPositionInMeasureInSegment (
      inputLineNumber,
      wholeNotesPositionInMeasure);

  // pad up the voice's stanzas // JMI ???
  if (fVoiceStanzasMap.size ()) {
    for (
      map<string, S_msrStanza>::const_iterator i = fVoiceStanzasMap.begin ();
      i != fVoiceStanzasMap.end ();
      ++i
    ) {
      S_msrStanza stanza = (*i).second;

      stanza->
        padUpToCurrentMeasureWholeNotesDurationInStanza (
          inputLineNumber,
          wholeNotesPositionInMeasure);
    } // for
  }

  --gIndenter;
}

void msrVoice::backupByWholeNotesStepLengthInVoice (
  int      inputLineNumber,
  rational backupTargetMeasureElementPositionInMeasure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Backup by a '" <<
      backupTargetMeasureElementPositionInMeasure <<
      "' whole notes step length in voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    fVoiceLastSegment != nullptr,
    "fVoiceLastSegment is null");

  // pad up the voice's last segment
  fVoiceLastSegment->
    backupByWholeNotesStepLengthInSegment (
      inputLineNumber,
      backupTargetMeasureElementPositionInMeasure);

  --gIndenter;
}

void msrVoice::appendPaddingNoteToVoice (
  int      inputLineNumber,
  rational forwardStepLength)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Appending padding note" <<
      ", forwardStepLength: " <<
      forwardStepLength <<
      ", to voice \"" <<
      getVoiceName () <<
      "\",line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // pad up the voice's last segment
  fVoiceLastSegment->
    appendPaddingNoteToSegment (
      inputLineNumber,
      forwardStepLength);

  // account for padding note's duration in staff
  fVoiceStaffUpLink->
    getStaffPartUpLink ()->
      incrementPartCurrentPositionInMeasure (
        inputLineNumber,
        forwardStepLength);

  // pad up the voice's stanzas
  if (fVoiceStanzasMap.size ()) {
    for (
      map<string, S_msrStanza>::const_iterator i = fVoiceStanzasMap.begin ();
      i != fVoiceStanzasMap.end ();
      ++i
  ) {
      S_msrStanza stanza = (*i).second;

      stanza->appendPaddingNoteToStanza (
        inputLineNumber,
        forwardStepLength);
    } // for
  }

  --gIndenter;
}

void msrVoice::appendTransposeToVoice (
  S_msrTranspose transpose)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTranspositions ()) {
    gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceStaves ()) {
    gLogStream <<
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

void msrVoice::appendTempoToVoice (S_msrTempo tempo)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTempos ()) {
    gLogStream <<
      "Appending tempo '" << tempo->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendTempoToSegment (tempo);
}

void msrVoice::appendOctaveShiftToVoice (
  S_msrOctaveShift octaveShift)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOctaveShifts ()) {
    gLogStream <<
      "Appending octave shift '" <<
      octaveShift->octaveShiftKindAsString () <<
      "', size: " << octaveShift->getOctaveShiftSize () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendOctaveShiftToSegment (octaveShift);
}

void msrVoice::appendScordaturaToVoice (
  S_msrScordatura scordatura)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceScordaturas ()) {
    gLogStream <<
      "Appending scordatura '" <<
      scordatura->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendScordaturaToSegment (scordatura);
}

void msrVoice::appendAccordionRegistrationToVoice (
  S_msrAccordionRegistration
    accordionRegistration)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceAccordionRegistrations ()) {
    gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceHarpPedals ()) {
    gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRehearsals ()) {
    gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    voiceStaffChange->getInputLineNumber ();
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceStaffChanges ()) {
    gLogStream <<
      "Appending voice staff change '" <<
      voiceStaffChange->asString () <<
      "' to voice " <<
      " \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // append voice staff change to voice's last segment
  fVoiceLastSegment->
    appendVoiceStaffChangeToSegment (
      voiceStaffChange);

  --gIndenter;
}

void msrVoice::appendNoteToVoice (S_msrNote note) {
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Appending note '" <<
      endl;

    ++gIndenter;

    gLogStream <<
      note <<
        "' to voice \"" << getVoiceName () << "\"" <<
        ", line " << inputLineNumber <<
        endl;

    --gIndenter;
  }
#endif

  // fetch the part
  S_msrPart
    part =
      fetchVoicePartUpLink ();

  // fetch the part current position in measure
  rational
    partCurrentPositionInMeasure =
      part->
        getPartCurrentPositionInMeasure ();

  if (! fVoiceLastSegment) {
    this->displayVoice (
      inputLineNumber,
      "fVoiceLastSegment is null in appendNoteToVoice()");

    msgAssert (
      __FILE__, __LINE__,
      fVoiceLastSegment != nullptr,
      "fVoiceLastSegment is null in appendNoteToVoice()");
  }

  // append the note to the last segment
  fVoiceLastSegment->
    appendNoteToSegment (
      note,
      partCurrentPositionInMeasure);

  // is this note the shortest one in this voice?
  this->
    registerShortestNoteInVoiceIfRelevant (
      note);

  // register note as the last appended one into this voice
  fVoiceLastAppendedNote = note;

  // account for note's duration in staff
  part->
    incrementPartCurrentPositionInMeasure (
      inputLineNumber,
      note->getNoteSoundingWholeNotes ());

  // register whether music (i.e. not just skips)
  // has been inserted into the voice
  switch (note->getNoteKind ()) {
    case msrNoteKind::k_NoNote:
      break;

    case msrNoteKind::kNoteRestInMeasure:
      // register rest
      ++fVoiceRestsCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      // don't account skips as music
      ++fVoiceSkipsCounter;
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      // register actual note
      ++fVoiceActualNotesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      // register actual note
      ++fVoiceActualNotesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      // register actual note
      ++fVoiceActualNotesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      // register actual note
      ++fVoiceActualNotesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNoteKind::kNoteRegularInChord:
      // register actual note
      ++fVoiceActualNotesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      // register actual note
      ++fVoiceActualNotesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;
  } // switch

  // are there harmonies attached to this note? // BLARK
  const list<S_msrHarmony>&
    noteHarmoniesList =
      note->
        getNoteHarmoniesList ();

  if (noteHarmoniesList.size ()) {
    // get the current part's harmony voice
    S_msrVoice
      partHarmonyVoice =
        part->
          getPartHarmoniesVoice ();

    list<S_msrHarmony>::const_iterator i;
    for (
      i=noteHarmoniesList.begin (); i!=noteHarmoniesList.end (); ++i
    ) {
      S_msrHarmony harmony = (*i);

      // append the harmony to the part harmony voice
      partHarmonyVoice->
        appendHarmonyToVoice (
          harmony);
    } // for
  }

  // are there figured basses attached to this note?
  const list<S_msrFiguredBass>&
    noteFiguredBassesList =
      note->
        getNoteFiguredBassesList ();

  if (noteFiguredBassesList.size ()) {
    // get the current part's figured bass voice
    S_msrVoice
      partFiguredBassVoice =
        part->
          getPartFiguredBassVoice ();

    list<S_msrFiguredBass>::const_iterator i;
    for (
      i=noteFiguredBassesList.begin (); i!=noteFiguredBassesList.end (); ++i
    ) {
      S_msrFiguredBass figuredBass = (*i);

      // append the harmony to the part harmony voice
      partFiguredBassVoice->
        appendFiguredBassToVoice (
          figuredBass);
    } // for
  }
}

void msrVoice::appendNoteToVoiceClone (S_msrNote note) {
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    note->getInputLineNumber ();
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Appending note '" <<
      note->asShortString () <<
      "' to voice clone \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // append the note to the last segment
  fVoiceLastSegment->
    appendNoteToSegmentClone (note);

  // is this note the shortest one in this voice?
  this->
    registerShortestNoteInVoiceIfRelevant (
      note);

  // register note as the last appended one into this voice
  fVoiceLastAppendedNote = note;

  // register whether music (i.e. not just skips)
  // has been inserted into the voice
  switch (note->getNoteKind ()) {
    case msrNoteKind::k_NoNote:
      break;

    case msrNoteKind::kNoteRestInMeasure:
      // register rest
      ++fVoiceRestsCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      // don't account skips as music
      ++fVoiceSkipsCounter;
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      // register actual note
      ++fVoiceActualNotesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      // register actual note
      ++fVoiceActualNotesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      // register actual note
      ++fVoiceActualNotesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      // register actual note
      ++fVoiceActualNotesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNoteKind::kNoteRegularInChord:
      // register actual note
      ++fVoiceActualNotesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      // register actual note
      ++fVoiceActualNotesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;
  } // switch
}

void msrVoice::appendDoubleTremoloToVoice (
  S_msrDoubleTremolo doubleTremolo)

{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTremolos ()) {
    gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceChords ()) {
    gLogStream <<
      "Appending chord '" << chord->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // append chord to voice last segment
  fVoiceLastSegment->
    appendChordToSegment (chord);

  // account for chord duration in the part current position in measure
  fVoiceStaffUpLink->
    getStaffPartUpLink ()->
      incrementPartCurrentPositionInMeasure (
        chord->getInputLineNumber (),
        chord->getChordSoundingWholeNotes ());

  // get the chord's notes vector
  const vector<S_msrNote>&
    chordNotesVector =
      chord->
        getChordNotesVector ();

  unsigned int chordNotesVectorSize =
    chordNotesVector.size ();

  if (chordNotesVectorSize) {
    {
      // get the chord's first note
      S_msrNote
        chordFirstNote = chordNotesVector [0];

      // is chordFirstNote the shortest one in this voice?
      this->
        registerShortestNoteInVoiceIfRelevant (
          chordFirstNote);
    }

    {
      // get the chord's last note
      S_msrNote
        chordLastNote =
          chordNotesVector [chordNotesVectorSize - 1];

/* JMI
      // is chordLastNote the shortest one in this voice?
      this->
        registerShortestNoteInVoiceIfRelevant (
          chordLastNote);
*/

      // register chordLastNote as the last appended one into this voice
      fVoiceLastAppendedNote = chordLastNote;
    }
  }

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendTupletToVoice (S_msrTuplet tuplet)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Appending tuplet " << tuplet->asString () <<
      " to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  // append tuplet to voice last segment
  fVoiceLastSegment->
    appendTupletToSegment (tuplet);

  // account for tuplet duration in the part's current position in measure
  fVoiceStaffUpLink->
    getStaffPartUpLink ()->
      incrementPartCurrentPositionInMeasure (
        tuplet->getInputLineNumber (),
        tuplet->getTupletSoundingWholeNotes ());

  --gIndenter;

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::addGraceNotesGroupBeforeAheadOfVoiceIfNeeded (
  S_msrGraceNotesGroup graceNotesGroup)
{
  int inputLineNumber =
    graceNotesGroup->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Adding grace notes '" <<
      graceNotesGroup->asString () <<
      "' ahead of voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

/* JMI
  gLogStream <<
    endl <<
    "======================= prependGraceNotesToVoice" <<
    endl;
  this->print (gLogStream);
  gLogStream <<
    "=======================" <<
    endl << endl;
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
      msrMeasureImplicitKind::kMeasureImplicitKindNo);
  }

  // such grace notes groups should be attached to the voice's first note,
  // or to the first chord if the latter belongs to such

  // fetch the voice's first note
  S_msrNote
    voiceFirstNote =
      fetchVoiceFirstNonGraceNote (); // JMI

  // get the voice first note's chord upLink
  S_msrChord
    firstNoteDirectChordUpLink =
      voiceFirstNote->
        getNoteDirectChordUpLink ();

  if (firstNoteDirectChordUpLink) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
      gLogStream <<
        "Attaching grace notes before '" <<
        graceNotesGroup->asString () <<
        "' to the first chord of voice \"" << getVoiceName () <<
        "\", i.e. '" <<
        firstNoteDirectChordUpLink->asShortString () <<
        "'" <<
        endl;
    }
#endif

/*
    firstNoteDirectChordUpLink->
      setChordGraceNotesGroupBefore (
        graceNotesGroup);
        */

    // create a grace notes group link
    S_msrChordGraceNotesGroupLink
      chordChordGraceNotesGroupLink =
        msrChordGraceNotesGroupLink::create (
          graceNotesGroup->getInputLineNumber (),
          graceNotesGroup,
          firstNoteDirectChordUpLink);

    // register it in the chord
    firstNoteDirectChordUpLink->
      setChordGraceNotesGroupLinkBefore (
        inputLineNumber,
        chordChordGraceNotesGroupLink);

  }

  else {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
      gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceLyrics ()) {
    gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasures ()) {
    gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasures ()) {
    gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasures ()) {
    gLogStream <<
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
      ++i
  ) {
      S_msrStanza stanza = (*i).second;

      stanza->appendLineBreakSyllableToStanza (
        lineBreak->getInputLineNumber (),
        fVoiceCurrentMeasurePuristNumber);
    } // for
  }
}

void msrVoice::appendPageBreakToVoice (S_msrPageBreak pageBreak)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Appending page break '" << pageBreak->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendPageBreakToSegment (pageBreak);
}

void msrVoice::prependOtherElementToVoice (S_msrMeasureElement elem) {
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Prepending other element '" << elem <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    prependOtherElementToSegment (elem);
}

void msrVoice::appendOtherElementToVoice (S_msrMeasureElement elem) {
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
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

  msgAssert (
    __FILE__, __LINE__,
    fVoiceLastSegment != nullptr,
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
      gGlobalOahOahGroup->getInputSourceName (),
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
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

void msrVoice::pushRepeatOntoRepeatDescrsStack (
  int         inputLineNumber,
  S_msrRepeat repeat,
  string      context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Pushing repeat '" <<
      repeat->asShortString () <<
      "' onto the repeats stack in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fVoicePendingRepeatDescrsStack.push_front (
    msrRepeatDescr::create (
      inputLineNumber,
      repeat));

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    string
      combinedContext =
        "pushRepeatOntoRepeatDescrsStack() called from " + context;

    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      combinedContext);
  }
#endif
}

void msrVoice::popRepeatFromRepeatDescrsStack (
  int         inputLineNumber,
  S_msrRepeat repeat,
  string      context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    string
      combinedContext =
        "popRepeatFromRepeatDescrsStack() 1 called from " + context;

    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      combinedContext);
  }
#endif

  if (
    repeat
      !=
    fVoicePendingRepeatDescrsStack.front ()->getRepeatDescrRepeat ()
  ) {
    stringstream s;

    s <<
      "cannot pop repeat '" <<
      repeat->asShortString () <<
      "' from the stack since it is not at the top" <<
      " (" << context << ")" <<
      ", line " << inputLineNumber;

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
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
  fVoicePendingRepeatDescrsStack.pop_front ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    string
      combinedContext =
        "popRepeatFromRepeatDescrsStack() 2 called from " + context;

    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      combinedContext);
  }
#endif
}

void msrVoice::displayVoiceRepeatsStack (
  int    inputLineNumber,
  string context)
{
  unsigned int repeatDescrsStackSize =
    fVoicePendingRepeatDescrsStack.size ();

  gLogStream <<
    endl <<
    ">>++++++++++++++++ Displaying voice repeats stack " << context <<
    endl <<
    "The voice repeats stack contains " <<
    singularOrPlural (repeatDescrsStackSize, "element", "elements") <<
    ", line " << inputLineNumber <<
    ":" <<
    endl;

  if (repeatDescrsStackSize) {
    list<S_msrRepeatDescr>::const_iterator
      iBegin = fVoicePendingRepeatDescrsStack.begin (),
      iEnd   = fVoicePendingRepeatDescrsStack.end (),
      i      = iBegin;

    ++gIndenter;

    int n = repeatDescrsStackSize;
    for ( ; ; ) {
      gLogStream <<
        "v (" << n << ")" <<
        ", repeatDescrStartInputLineNumber: " <<
        (*i)->getRepeatDescrStartInputLineNumber () <<
        endl;

      ++gIndenter;
      (*i)->
        getRepeatDescrRepeat ()->
          printShort (gLogStream);
      --gIndenter;

      --n;

      if (++i == iEnd) break;

      gLogStream << endl;
    } // for

    --gIndenter;
  }

  gLogStream <<
    " <<++++++++++++++++ " <<
    endl << endl;
}

void msrVoice::displayVoiceRepeatsStackSummary (
  int    inputLineNumber,
  string context)
{
  unsigned int repeatDescrsStackSize =
    fVoicePendingRepeatDescrsStack.size ();

  gLogStream <<
    endl <<
    "The voice repeats stack contains " <<
    singularOrPlural (repeatDescrsStackSize, "element", "elements") <<
    " - " << context <<
    ", line " << inputLineNumber <<
    ":" <<
    endl;

  if (repeatDescrsStackSize) {
    list<S_msrRepeatDescr>::const_iterator
      iBegin = fVoicePendingRepeatDescrsStack.begin (),
      iEnd   = fVoicePendingRepeatDescrsStack.end (),
      i      = iBegin;

    ++gIndenter;

    int n = repeatDescrsStackSize;
    for ( ; ; ) {
      gLogStream <<
        "v (" << n << ")" <<
        ", repeatDescrStartInputLineNumber: " <<
        (*i)->getRepeatDescrStartInputLineNumber () <<
        endl;

      ++gIndenter;
      (*i)->
        getRepeatDescrRepeat ()->
          printShort (gLogStream);
      --gIndenter;

      --n;

      if (++i == iEnd) break;

      gLogStream << endl;
    } // for

    gLogStream <<
      "===============" <<
      endl << endl;

    --gIndenter;
  }
}

void msrVoice::displayVoiceRepeatsStackAndVoice (
  int    inputLineNumber,
  string context)
{
  displayVoiceRepeatsStack (
    inputLineNumber,
    context);

  displayVoice (
    inputLineNumber,
    context);
}

void msrVoice::displayVoiceMeasuresRepeat (
  int    inputLineNumber,
  string context)
{
  gLogStream <<
    endl <<
    ">>++++++++++++++++ Displaying voice measures repeats " << context <<
    endl <<
    "The current voice measures repeat contains " <<
    ", line " << inputLineNumber <<
    ":" <<
    endl;

  ++gIndenter;

  if (fVoicePendingMeasuresRepeat) {
    fVoicePendingMeasuresRepeat->
      displayMeasuresRepeat (
        inputLineNumber,
        context);
  }
  else {
    gLogStream <<
      "none" <<
      endl;
  }

  --gIndenter;

  gLogStream <<
    " <<++++++++++++++++ " <<
    endl << endl;
}

void msrVoice::displayVoiceMeasuresRepeatAndVoice (
  int    inputLineNumber,
  string context)
{
  displayVoiceMeasuresRepeat (
    inputLineNumber,
    context);

  displayVoice (
    inputLineNumber,
    context);
}

void msrVoice::displayVoiceRestMeasures (
  int    inputLineNumber,
  string context)
{
  gLogStream <<
    endl <<
    ">>++++++++++++++++ voice rest measures " << context <<
    endl <<
    "The current voice rest measures contains " <<
    ", line " << inputLineNumber <<
    ":" <<
    endl;

  ++gIndenter;

  if (fVoicePendingRestMeasures) {
    fVoicePendingRestMeasures->
      displayRestMeasures (
        inputLineNumber,
        context);
  }
  else {
    gLogStream <<
      "none" <<
      endl;
  }

  --gIndenter;

  gLogStream <<
    " <<++++++++++++++++ " <<
    endl << endl;
}

void msrVoice::displayVoiceRestMeasuresAndVoice (
  int    inputLineNumber,
  string context)
{
  displayVoiceRestMeasures (
    inputLineNumber,
    context);

  displayVoice (
    inputLineNumber,
    context);
}

S_msrRepeat msrVoice::createARepeatAndStackIt (
  int    inputLineNumber,
  string context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
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

  // push it onto the voice's repeat descrs stack
  pushRepeatOntoRepeatDescrsStack (
    inputLineNumber,
    result,
    "createARepeatAndStackIt() 1");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "createARepeatAndStackIt() 2");
  }
#endif

  return result;
}

S_msrRepeat msrVoice::createARepeatCloneAndStackIt (
  int         inputLineNumber,
  S_msrRepeat repeat,
  string      context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
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

  // push it onto the voice's repeat descrs stack
  pushRepeatOntoRepeatDescrsStack (
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
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
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  for (
    list<S_msrVoiceElement>::iterator i = fInitialVoiceElementsList.begin ();
    i != fInitialVoiceElementsList.end ();
    ++i
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Moving the voice last segment '";

    if (fVoiceLastSegment) { // JMI should not be necessary?
      gLogStream <<
        fVoiceLastSegment->asShortString ();
    }
    else {
      gLogStream <<
        "null";
    }

    gLogStream <<
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    string
      combinedContext =
        "moveVoiceLastSegmentToRepeatCommonPart() 1 called from " + context;

    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      combinedContext);
  }
#endif

  if (fVoiceLastSegment) { // JMI should not be necessary?
  /* JMI
    // fetch the last segment's last measure // JMI ???
    string
      combinedContext =
        "moveVoiceLastSegmentToRepeatCommonPart() 2 called from " + context;

    S_msrMeasure
      voiceLastSegmentLastMeasure =
        fVoiceLastSegment->
          fetchLastMeasureFromSegment (
            inputLineNumber,
            combinedContext);
*/

    // set voice current after repeat component phase kind
    setCurrentVoiceRepeatPhaseKind (
      inputLineNumber,
      msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterCommonPart);

    // append fVoiceLastSegment to the repeat common part
    repeatCommonPart->
      appendSegmentToRepeatCommonPart (
        inputLineNumber,
        fVoiceLastSegment,
        context);

    // forget about this voice last segment
    fVoiceLastSegment = nullptr;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    string
      combinedContext =
        "moveVoiceLastSegmentToRepeatCommonPart() 3 called from " + context;

    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      combinedContext);
  }
#endif
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

void msrVoice::moveVoiceLastSegmentToRepeatEnding (
  int               inputLineNumber,
  S_msrRepeatEnding repeatEnding,
  string            context)
{
  // move the voice last segment to repeatEnding
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
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

void msrVoice::appendRepeatToInitialVoiceElements (
  int         inputLineNumber,
  S_msrRepeat repeat,
  string      context)
{
  // append repeat to the list of initial elements
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending repeat '" <<
      repeat->asString () <<
      "' to the initial elements in voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // is the previous element in the voice elements list a repeat?
  S_msrVoiceElement previousElement;

  if (fInitialVoiceElementsList.size ()) {
    previousElement =
      fInitialVoiceElementsList.back ();
  }

  if (previousElement) {
    if (
      S_msrRepeat previousRepeat = dynamic_cast<msrRepeat*>(&(*previousElement))
    ) {
      // set the respective immediately preceding and following repeats
      previousRepeat->
        setImmediatelyFollowingRepeat (
          repeat);
      repeat->
        setImmediatelyPrecedingRepeat (
          previousRepeat);
    }
  }

  // do append the repeat
  fInitialVoiceElementsList.push_back (
    repeat);
}

void msrVoice::appendRestMeasuresToInitialVoiceElements (
  int               inputLineNumber,
  S_msrRestMeasures restMeasures,
  string            context)
{
  // append restMeasures to the list of initial elements
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    gLogStream <<
      "Appending rest measures '" <<
      restMeasures->asString () <<
      "' to the initial elements in voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fInitialVoiceElementsList.push_back (
    restMeasures);
}

void msrVoice::appendMeasuresRepeatToInitialVoiceElements (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeat,
  string              context)
{
  // append measuresRepeat to the list of initial elements
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    gLogStream <<
      "Appending measures repeat '" <<
      measuresRepeat->asString () <<
      "' to the initial elements in voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fInitialVoiceElementsList.push_back (
    measuresRepeat);
}

void msrVoice::appendVoiceLastSegmentToInitialVoiceElements (
  int          inputLineNumber,
  string       context)
{
  // append segment to the list of initial elements
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Appending voice last segment '" <<
      fVoiceLastSegment->asString () <<
      "' to the initial elements in voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fInitialVoiceElementsList.push_back (
    fVoiceLastSegment);
}

void msrVoice::moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
  int          inputLineNumber,
  string       context)
{
  if (fVoiceLastSegment) {
    const list<S_msrMeasure>&
      segmentMeasuresList =
        fVoiceLastSegment->
          getSegmentMeasuresList ();

    if (segmentMeasuresList.size ()) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceSegments ()) {
        gLogStream <<
          "Moving voice last segment '" <<
          fVoiceLastSegment->asString () <<
          "' to the initial elements in voice \"" <<
          getVoiceName () <<
          "\" (" << context << ")" <<
          ", line " << inputLineNumber <<
          endl;
      }

      if (gGlobalTraceOahGroup->getTraceSegmentsDetails ()) {
        displayVoice (
          inputLineNumber,
          "moveVoiceLastSegmentToInitialVoiceElementsIfRelevant() 1");
      }
#endif

      // append segment to the list of initial elements
      fInitialVoiceElementsList.push_back (
        fVoiceLastSegment);

      // forget about this last segment
      fVoiceLastSegment = nullptr;

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceSegmentsDetails ()) {
        displayVoice (
          inputLineNumber,
          "moveVoiceLastSegmentToInitialVoiceElementsIfRelevant() 2");
      }
#endif
    }

    else {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceSegments ()) {
        gLogStream <<
          "Voice last segment '" <<
          fVoiceLastSegment->asString () <<
          "' contains no measure, not moved to the initial elements in voice \"" <<
          getVoiceName () <<
          "\" (" << context << ")" <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif
    }
  }

  else {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceSegments ()) {
      gLogStream <<
        "Voice last segment '" <<
        fVoiceLastSegment->asString () <<
        "' is null, not moved to the initial elements in voice \"" <<
        getVoiceName () <<
        "\" (" << context << ")" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif
  }
}

void msrVoice::appendRepeatCloneToInitialVoiceElements (
  int         inputLineNumber,
  S_msrRepeat repeatCLone,
  string      context)
{
  // append repeatCLone to the list of initial elements
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
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

void msrVoice::handleVoiceLevelRepeatStartInVoice (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a voice-level repeat start in voice \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatStartInVoice() 1");
  }
#endif

  ++gIndenter;

  // is there a voice last segment?
  if (fVoiceLastSegment) {

    // fetch last segment's measures list
    list<S_msrMeasure>
      voiceLastSegmentMeasuresList =
        fVoiceLastSegment->
          getSegmentMeasuresList ();

    // are there measures in the voice last segment?
    if (voiceLastSegmentMeasuresList.size ()) {
      // yes

      // fetch last measure in the last segment
      S_msrMeasure
        lastMeasureInLastSegment =
          voiceLastSegmentMeasuresList.back ();

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceRepeatsDetails ()) {
        lastMeasureInLastSegment->
          displayMeasure (
            inputLineNumber,
            "lastMeasureInLastSegment - handleVoiceLevelRepeatStartInVoice() 2");
      }
#endif

      // let's look at the last measure in detail

      // fetch its elements list
      const list<S_msrMeasureElement>&
        lastMeasureElementsList =
          lastMeasureInLastSegment->
            getMeasureElementsList ();

      if (lastMeasureElementsList.size ()) {
        // the last measure is not empty

        rational
          currentMeasureWholeNotesDuration =
            lastMeasureInLastSegment->getCurrentMeasureWholeNotesDuration ();
        rational
          fullMeasureWholeNotesDuration =
            lastMeasureInLastSegment->getFullMeasureWholeNotesDuration ();

        // is there a measure splitting?
        if ( // JMI better criterion???
          currentMeasureWholeNotesDuration.getNumerator () > 0
            &&
          currentMeasureWholeNotesDuration
            <
          fullMeasureWholeNotesDuration
        ) {
          // yes this measure is not yet complete and should be split
#ifdef TRACING_IS_ENABLED
          if (gGlobalTraceOahGroup->getTraceRepeats ()) {
            gLogStream <<
              "Splitting measure '" <<
              lastMeasureInLastSegment->asShortString () <<
              "' upon a repeat start in voice \"" <<
              getVoiceName () <<
              "\"" <<
              ", currentMeasureWholeNotesDuration: " <<
              currentMeasureWholeNotesDuration <<
              ", fullMeasureWholeNotesDuration: " <<
              fullMeasureWholeNotesDuration <<
              ", line " << inputLineNumber <<
              endl;
          }
#endif

/* JMI
          // finalize lastMeasureInLastSegment
          lastMeasureInLastSegment->
            finalizeMeasure (
              inputLineNumber,
              msrMeasure::kMeasuresRepeatContextKindNone,
              "handleVoiceLevelRepeatStartInVoice() 3");
*/
          // append last segment to initial voice elements list
          appendVoiceLastSegmentToInitialVoiceElements (
            inputLineNumber,
            "handleVoiceLevelRepeatStartInVoice() 4");

          // create a new last segment for the voice
#ifdef TRACING_IS_ENABLED
          if (gGlobalTraceOahGroup->getTraceSegments ()) {
            gLogStream <<
              "Creating a new last segment for voice \"" <<
              fVoiceName << "\"" <<
              ", line " << inputLineNumber <<
              endl;
          }
#endif

          createNewLastSegmentForVoice (
            inputLineNumber,
            "handleVoiceLevelRepeatStartInVoice() 5");

          // create a new measure with the same number as the voice last measure
          // and append it to the voice,
          createMeasureAndAppendItToVoice (
            inputLineNumber,
            lastMeasureInLastSegment->getMeasureElementMeasureNumber (),
            msrMeasureImplicitKind::kMeasureImplicitKindNo);

        /* JMI
          // set it as created before a repeat
          voiceLastMeasure->
            setMeasureCreatedForARepeatKind (
              msrMeasure::kMeasureCreatedForARepeatBefore);
              */
        }
        else {
          // no this measure is complete
          // JMI ???
        }
      }

      else {
        // the last measure elements list is empty,
        // keep it for a new voice last segment

        // remove last measure
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceRepeats ()) {
          gLogStream <<
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
                inputLineNumber,
                "handleVoiceLevelRepeatStartInVoice() 55");

        // append the voice last segment to the initial voice elements list
        appendVoiceLastSegmentToInitialVoiceElements (
          inputLineNumber,
          "handleVoiceLevelRepeatStartInVoice() 555");

        // create a new last segment containing a new measure for the voice
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceRepeats ()) {
          gLogStream <<
            "Creating a new last segment with the first common part measure for voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        createNewLastSegmentFromItsFirstMeasureForVoice (
          inputLineNumber,
          dummyMeasure,
          "handleVoiceLevelRepeatStartInVoice() 5555");
      }
    }
  }

  else {
    // no voice last segment JMI ???
  }

  // create the repeat and stack it
  S_msrRepeat
    newRepeat =
      createARepeatAndStackIt (
        inputLineNumber,
        "handleVoiceLevelRepeatStartInVoice() 10");

  // create the repeat common part
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a repeat common part upon its start in voice \"" <<
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
        newRepeat);

  // register it in newRepeat
  newRepeat->
    setRepeatCommonPart (
      repeatCommonPart);

  // set newRepeat as having an explicit start
  newRepeat->
    setRepeatExplicitStartKind (
      msrRepeat::kRepeatExplicitStartYes);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatStartInVoice() 11");
  }
#endif

  --gIndenter;
}

void msrVoice::handleNestedRepeatStartInVoice (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a nested repeat start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleNestedRepeatStartInVoice() 2");
  }
#endif
}

void msrVoice::handleRepeatStartInVoice (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling repeat start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatStartInVoice() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:
      // analyze this repeat start's context
      switch (fVoicePendingRepeatDescrsStack.size ()) {
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatStartInVoice() 2");
  }
#endif
}

void msrVoice::handleVoiceLevelRepeatEndWithoutStartInVoice (
  int    inputLineNumber,
  string measureNumber,
  int    repeatTimes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a voice-level repeat end without start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndWithoutStartInVoice() 1");
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "This repeat end without a start is at the voice-level" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create the repeat
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
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
    voiceLastSegmentLastMeasure =
      fVoiceLastSegment->
        fetchLastMeasureFromSegment (
          inputLineNumber,
          "handleVoiceLevelRepeatEndWithoutStartInVoice() 2");

  // set voice current after repeat component phase kind
  // before finalizeLastAppendedMeasureInVoice()
  setCurrentVoiceRepeatPhaseKind (
    inputLineNumber,
    msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterCommonPart);
/* JMI
  // finalize current measure in voice
  voiceLastSegmentLastMeasure->
    finalizeMeasure (
      inputLineNumber,
      msrMeasure::kMeasuresRepeatContextKindCommonPartLastMeasure,
      "handleVoiceLevelRepeatEndWithoutStartInVoice() 3"
      );
*/
  // append the voice last segment to the new repeat common part
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
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
      "handleVoiceLevelRepeatEndWithoutStartInVoice() 4");

  // create a new last segment for the voice
  createNewLastSegmentForVoice (
    inputLineNumber,
    "handleVoiceLevelRepeatEndWithoutStartInVoice() 5");

  // set newRepeat's build phase to completed
  newRepeat->
    setCurrentRepeatBuildPhaseKind (
      msrRepeat::kRepeatBuildPhaseCompleted);

  // append newRepeat to the list of initial elements
  appendRepeatToInitialVoiceElements (
    inputLineNumber,
    newRepeat,
    "handleVoiceLevelRepeatEndWithoutStartInVoice() 6");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndWithoutStartInVoice() 7");
  }
#endif

  --gIndenter;
}

void msrVoice::handleVoiceLevelContainingRepeatEndWithoutStartInVoice (
  int    inputLineNumber,
  string measureNumber,
  int    repeatTimes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a voice-level rcontaining epeat end without start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelContainingRepeatEndWithoutStartInVoice() 1");
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "This repeat end without a start is at the voice-level" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create the repeat
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a repeat common part upon its end in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  S_msrRepeatCommonPart
    newRepeatCommonPart =
      msrRepeatCommonPart::create (
        repeatInputLineNumber,
        newRepeat);

  // register it in newRepeat
  newRepeat->
    setRepeatCommonPart (
      newRepeatCommonPart);

  // fetch the last segment's last measure
  S_msrMeasure
    voiceLastSegmentLastMeasure =
      fVoiceLastSegment->
        fetchLastMeasureFromSegment (
          inputLineNumber,
          "handleVoiceLevelContainingRepeatEndWithoutStartInVoice() 1");

  // fetch the top of the repeats stack
  S_msrRepeat
    repeatsStackTopRepeat =
      fVoicePendingRepeatDescrsStack.front ()->
        getRepeatDescrRepeat ();

  // pop it from the repeats stack
  popRepeatFromRepeatDescrsStack (
    inputLineNumber,
    repeatsStackTopRepeat,
    "handleVoiceLevelContainingRepeatEndWithoutStartInVoice() 2");

  // append it to newRepeat's common part
  newRepeatCommonPart->
    appendRepeatToRepeatCommonPart (
      inputLineNumber,
      repeatsStackTopRepeat,
      "handleVoiceLevelContainingRepeatEndWithoutStartInVoice() 3");

  // append newRepeat to the list of initial elements
  appendRepeatToInitialVoiceElements (
    inputLineNumber,
    newRepeat,
    "handleVoiceLevelContainingRepeatEndWithoutStartInVoice() 4");

  // append the voice last segment to the new repeat common part
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending the voice last segment in voice \"" <<
      getVoiceName () <<
      "\" to the new voice-level repeat common part" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  newRepeatCommonPart->
    appendSegmentToRepeatCommonPart (
      inputLineNumber,
      fVoiceLastSegment,
      "handleVoiceLevelContainingRepeatEndWithoutStartInVoice() 5");

/* JMI SURE
  // create a new last segment for the voice
  createNewLastSegmentForVoice (
    inputLineNumber,
    "handleVoiceLevelContainingRepeatEndWithoutStartInVoice()");
    */

  // set newRepeat's build phase to completed
  newRepeat->
    setCurrentRepeatBuildPhaseKind (
      msrRepeat::kRepeatBuildPhaseCompleted);

  // append newRepeat to the list of initial elements
  appendRepeatToInitialVoiceElements (
    inputLineNumber,
    newRepeat,
    "handleVoiceLevelContainingRepeatEndWithoutStartInVoice() 6");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelContainingRepeatEndWithoutStartInVoice() 7");
  }
#endif

  --gIndenter;
}

void msrVoice::handleVoiceLevelRepeatEndWithStartInVoice (
  int    inputLineNumber,
  string measureNumber,
  int    repeatTimes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a voice-level repeat end with start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndWithStartInVoice() 1");
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "This repeat end with a start is at the voice-level" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // fetch the voice's last measure
  S_msrMeasure
    voiceLastMeasure =
      fetchVoiceLastMeasure (
        inputLineNumber);

  // set voice current after repeat component phase kind
  setCurrentVoiceRepeatPhaseKind (
    inputLineNumber,
    msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterCommonPart);

  // grab current repeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatDescrsStack.front ()->
        getRepeatDescrRepeat ();

  // pop it from the repeats stack
  popRepeatFromRepeatDescrsStack (
    inputLineNumber,
    currentRepeat,
    "handleVoiceLevelRepeatEndWithStartInVoice() 2");

  // set its repeat times field
  currentRepeat->
    setRepeatTimes (repeatTimes);

  // create the currentRepeat's common part
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
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
    "handleVoiceLevelRepeatEndWithStartInVoice() 3");

/* JMI SURE ???
  // create a new last segment to collect the remainder of the voice,
  if (false) {
  createNewLastSegmentForVoice ( // JMI
    inputLineNumber,
    "handleVoiceLevelRepeatEndWithStartInVoice() BOF");
  }
    */

  // set currentRepeat's build phase to completed
  currentRepeat->
    setCurrentRepeatBuildPhaseKind (
      msrRepeat::kRepeatBuildPhaseCompleted);

  // append currentRepeat to the list of initial elements
  appendRepeatToInitialVoiceElements (
    inputLineNumber,
    currentRepeat,
    "handleVoiceLevelRepeatEndWithStartInVoice() 4");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndWithStartInVoice() 5");
  }
#endif

  --gIndenter;
}

void msrVoice::handleNestedRepeatEndInVoice (
  int    inputLineNumber,
  string measureNumber,
  int    repeatTimes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a nested repeat end in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleNestedRepeatEndInVoice() 1");
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
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
    voiceLastMeasure->getCurrentMeasureWholeNotesDuration ()
      ==
    voiceLastMeasure->getFullMeasureWholeNotesDuration ()
  ) {
    // this measure is incomplete and should be split
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceRepeats ()) {
      gLogStream <<
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
      msrMeasureImplicitKind::kMeasureImplicitKindNo);

  /* JMI
    // set it as created after a repeat
    voiceLastMeasure->
      setMeasureCreatedForARepeatKind (
        msrMeasure::kMeasureCreatedForARepeatAfter);
        */
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatEndInVoice() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:
      {
        // analyze this repeat end's context
        switch (fVoicePendingRepeatDescrsStack.size ()) {
          case 0:
            // this repeat is at the voice-level and has no start
            // -------------------------------------
            handleVoiceLevelRepeatEndWithoutStartInVoice (
              inputLineNumber,
              measureNumber,
              repeatTimes);
            break;

          case 1:
            {
              // fetch the top of the repeats stack
              S_msrRepeat
                repeatsStackTopRepeat =
                  fVoicePendingRepeatDescrsStack.front ()->
                    getRepeatDescrRepeat ();

              // analyze it
              switch (repeatsStackTopRepeat->getCurrentRepeatBuildPhaseKind ()) {
                case msrRepeat::kRepeatBuildPhaseJustCreated:
                  {
                    stringstream s;

                    s <<
                      "attempting to end repeat '" <<
                      repeatsStackTopRepeat->asShortString () <<
                      "' right after it has been created" <<
                      " in voice '" <<
                      asShortString () <<
                      "' ";

                    msrInternalError (
                      gGlobalOahOahGroup->getInputSourceName (),
                      fInputLineNumber,
                      __FILE__, __LINE__,
                      s.str ());
                  }
                  break;

                case msrRepeat::kRepeatBuildPhaseInCommonPart:
                case msrRepeat::kRepeatBuildPhaseInEndings:
                  // this repeat is at the voice-level and has a start
                  // -------------------------------------
                  handleVoiceLevelRepeatEndWithStartInVoice (
                    inputLineNumber,
                    measureNumber,
                    repeatTimes);
                  break;

                case msrRepeat::kRepeatBuildPhaseCompleted:
                  // this repeat is at the voice-level, has no start
                  // and contains repeatsStackTop
                  // -------------------------------------
                  handleVoiceLevelContainingRepeatEndWithoutStartInVoice (
                    inputLineNumber,
                    measureNumber,
                    repeatTimes);
                  break;
              } // switch
            }
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatEndInVoice() 2");
  }
#endif
}

void msrVoice::handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice (
  int         inputLineNumber,
  S_msrRepeat currentRepeat)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a voice-level repeat ending start without explicit start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice() 1");
  }
#endif

  ++gIndenter;

  S_msrRepeat           repeat;
  S_msrRepeatCommonPart repeatCommonPart;

  if (currentRepeat) {
    repeatCommonPart =
      currentRepeat->
        getRepeatCommonPart ();
  }

  else {
    // create the repeat and stack it
    stringstream s1;

    s1 <<
      "Creating a voice-level repeat upon its first ending in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber;

    int repeatInputLineNumber = 1; // could find first measure's input line number??? JMI

    currentRepeat =
      createARepeatAndStackIt (
        repeatInputLineNumber,
        s1.str ());

    // create the repeat common part
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceRepeats ()) {
      gLogStream <<
        "Creating a repeat common part upon its end in voice \"" <<
        getVoiceName () <<
        "\"" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    repeatCommonPart =
      msrRepeatCommonPart::create (
        repeatInputLineNumber,
        currentRepeat);

    // register it in newRepeat
    currentRepeat->
      setRepeatCommonPart (
        repeatCommonPart);
  }

  // fetch last measure
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
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
          inputLineNumber,
          "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice() 1");

  // fetch lastMeasure's elements list
  const list<S_msrMeasureElement>&
    lastMeasureElementsList =
      lastMeasure->
        getMeasureElementsList ();

  // let's look at lastMeasure in detail
  if (! lastMeasureElementsList.size ()) {

    // it is empty, keep it for a new voice last segment

    // remove last measure
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceRepeats ()) {
      gLogStream <<
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
            inputLineNumber,
            "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice() 2");

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
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceRepeats ()) {
      gLogStream <<
        "Creating a new last segment with the first ending measure for voice \"" <<
        fVoiceName << "\"" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    createNewLastSegmentFromItsFirstMeasureForVoice (
      inputLineNumber,
      lastMeasure,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice() 3");
  }

  else {

    // it is not empty

    // append last segment to initial voice elements list
    appendVoiceLastSegmentToInitialVoiceElements (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice() 4");

    // create a new last segment for the voice
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceSegments ()) {
      gLogStream <<
        "Creating a new last segment for voice \"" <<
        fVoiceName << "\"" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    createNewLastSegmentForVoice (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice( 5)");
  }

  // set currentRepeat's build phase
  currentRepeat->
    setCurrentRepeatBuildPhaseKind (
      msrRepeat::kRepeatBuildPhaseInEndings);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice() 2");
  }
#endif

  --gIndenter;
}

void msrVoice::handleVoiceLevelRepeatEndingStartWithExplicitStartInVoice (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a voice-level repeat ending start with explicit start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithExplicitStartInVoice() 1");
  }
#endif

  ++gIndenter;

  // fetch currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatDescrsStack.front ()->
        getRepeatDescrRepeat ();

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
          inputLineNumber,
          "handleVoiceLevelRepeatEndingStartWithExplicitStartInVoice() 2");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Fetching the last measure of the last segment in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", it is:" <<
      endl <<
      lastMeasure->asShortString () <<
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
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceRepeats ()) {
      gLogStream <<
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
            inputLineNumber,
            "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice() 3");

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
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceRepeats ()) {
      gLogStream <<
        "Creating a new last segment with the first ending measure for voice \"" <<
        fVoiceName << "\"" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    createNewLastSegmentFromItsFirstMeasureForVoice (
      inputLineNumber,
      lastMeasure,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice() 4");
  }

  else {

    // it is not empty

    // append last segment to initial voice elements list
    appendVoiceLastSegmentToInitialVoiceElements (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice() 5");

    // create a new last segment for the voice
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceSegments ()) {
      gLogStream <<
        "Creating a new last segment for voice \"" <<
        fVoiceName << "\"" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    createNewLastSegmentForVoice (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice() 6");
  }

  // set currentRepeat's build phase
  currentRepeat->
    setCurrentRepeatBuildPhaseKind (
      msrRepeat::kRepeatBuildPhaseInEndings);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithExplicitStartInVoice() 7");
  }
#endif

  --gIndenter;
}

void msrVoice::nestContentsIntoNewRepeatInVoice (
  int inputLineNumber)
{
  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:
      // is there a voice last segment?
      if (fVoiceLastSegment) {

        // are there measures in the voice last segment?
        if (fVoiceLastSegment->getSegmentMeasuresList ().size ()) {
          // finalize current measure in voice
          finalizeLastAppendedMeasureInVoice (
            inputLineNumber);

#ifdef TRACING_IS_ENABLED
          if (gGlobalTraceOahGroup->getTraceRepeats ()) {
            displayVoiceRepeatsStackSummary (
              inputLineNumber,
              "nestContentsIntoNewRepeatInVoice() 1");
          }
#endif

          // move voice last segment to the list of initial elements
          moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
            inputLineNumber,
            "nestContentsIntoNewRepeatInVoice() 1");

#ifdef TRACING_IS_ENABLED
          if (gGlobalTraceOahGroup->getTraceRepeats ()) {
            displayVoiceRepeatsStackSummary (
              inputLineNumber,
              "nestContentsIntoNewRepeatInVoice() 2");
          }
#endif
        }
      }
      break;
  } // switch
}

void msrVoice::handleNestedRepeatEndingStartInVoice (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a nested repeat ending start voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
 }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleNestedRepeatEndingStartInVoice() 2");
  }
#endif
}

void msrVoice::handleRepeatEndingStartInVoice (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat ending start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatEndingStartInVoice() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:
      {
        ++gIndenter;

        // analyze this repeat end's context
        switch (fVoicePendingRepeatDescrsStack.size ()) {
          case 0:
            // this the first ending of a voice-level repeat without start
            // -------------------------------------
            handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice (
              inputLineNumber,
              nullptr);
            break;

          case 1:
            {
              // fetch currentRepeat
              S_msrRepeat
                currentRepeat =
                  fVoicePendingRepeatDescrsStack.front ()->
                    getRepeatDescrRepeat ();

              switch (currentRepeat->getRepeatExplicitStartKind ()) {
                case msrRepeat::kRepeatExplicitStartNo:
                  // this the first ending of a voice-level repeat without a start
                  // -------------------------------------
                  handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice (
                    inputLineNumber,
                    currentRepeat);
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

        --gIndenter;
      }
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatEndingStartInVoice() 2");
  }
#endif
}

void msrVoice::handleRepeatEndingStartInVoiceClone (
  int       inputLineNumber,
  msrRepeatEndingKind
            repeatEndingKind,
  string    repeatEndingNumber) // may be "1, 2"
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatEndingStartInVoiceClone() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:
      {
        // handle the repeat ending start
        ++gIndenter;

#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceRepeats ()) {
          gLogStream <<
            "Handling a repeat ending upon its start in voice clone \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        // is there a current repeat?
        switch (fVoicePendingRepeatDescrsStack.size ()) {
          case 0:
            {
              stringstream s;

              s <<
                "repeats stack is empty when attempting to handle a repeat ending start in voice clone '" <<
                asShortString () <<
                "' ";

              msrInternalError (
                gGlobalOahOahGroup->getInputSourceName (),
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
                  fVoicePendingRepeatDescrsStack.front ()->
                    getRepeatDescrRepeat ();

              // create a repeat ending
#ifdef TRACING_IS_ENABLED
              if (gGlobalTraceOahGroup->getTraceRepeats ()) {
                gLogStream <<
                  "Creating a " <<
                  msrRepeatEndingKindAsString (
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

              // add the repeat ending to the voice current repeat
#ifdef TRACING_IS_ENABLED
              if (gGlobalTraceOahGroup->getTraceRepeats ()) {
                gLogStream <<
                  "Appending a " <<
                  msrRepeatEndingKindAsString (
                    repeatEndingKind) <<
                  " repeat ending to current repeat in voice \"" <<
                  fVoiceName <<
                  "\"" <<
                  endl;
              }
#endif

#ifdef TRACING_IS_ENABLED
              if (gGlobalTraceOahGroup->getTraceRepeats ()) {
                displayVoiceRepeatsStackSummary (
                  inputLineNumber,
                  "before adding a hooked repeat ending to current repeat");
              }
#endif

              currentRepeat->
                addRepeatEndingToRepeat (
                  inputLineNumber,
                  repeatEnding);

              // set currentRepeat's build phase
              currentRepeat->
                setCurrentRepeatBuildPhaseKind (
                  msrRepeat::kRepeatBuildPhaseInEndings);
            }
          break;

          default:
            {
              // the current repeat is nested

              // JMI ???

              // move voice last segment into the repeat common part
#ifdef TRACING_IS_ENABLED
              if (gGlobalTraceOahGroup->getTraceRepeats ()) {
                gLogStream <<
                  "Moving the voice last segment to the repeat common part in voice clone \"" <<
                  getVoiceName () <<
                  "\"" <<
                  ", line " << inputLineNumber <<
                  endl;
              }
#endif
            }
        } // switch

        --gIndenter;
      }
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatEndingStartInVoiceClone() 2");
  }
#endif
}

void msrVoice::handleSegmentCloneEndInVoiceClone (
  int          inputLineNumber,
  S_msrSegment segmentClone)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Handling segment '" <<
      segmentClone->asShortString () <<
      "' in voice clone \"" <<
      fVoiceName <<
      "\"" <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceSegmentsDetails ()) {
    displayVoice (
      inputLineNumber,
      "handleSegmentCloneEndInVoiceClone() 1");
  }
#endif

  // analyze segmentClone's context,
  // rest measures and measures repeats first,
  // since they can be nested in repeats
  if (fVoicePendingRestMeasures) {
    // segmentClone belongs to a rest measures

    // get fVoicePendingRestMeasures's contents
    S_msrRestMeasuresContents
      restMeasuresContents =
        fVoicePendingRestMeasures->
          getRestMeasuresContents ();

    // set segmentClone as the contents's segment
    restMeasuresContents->
      setRestMeasuresContentsSegment (
        inputLineNumber,
        segmentClone);
  }

  else if (fVoicePendingMeasuresRepeat) {
    // segmentClone belongs to a measures repeat

    switch (
      fVoicePendingMeasuresRepeat->getCurrentMeasuresRepeatBuildPhaseKind ()
    ) {
      case msrMeasuresRepeat::kMeasuresRepeatBuildPhaseJustCreated:
        {
          stringstream s;

          s <<
            "segment '" <<
            segmentClone->asShortString () <<
            "'cannot be added to a just created measures repeat";

          msrMusicXMLError (
            gGlobalOahOahGroup->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
        break;

      case msrMeasuresRepeat::kMeasuresRepeatBuildPhaseInPattern:
        {
          // get fVoicePendingMeasuresRepeat's pattern
          S_msrMeasuresRepeatPattern
            measuresRepeatPattern =
              fVoicePendingMeasuresRepeat->
                getMeasuresRepeatPattern ();

          // set segmentClone as the pattern's segment
          measuresRepeatPattern->
            setMeasuresRepeatPatternSegment (
        // JMI      inputLineNumber,
              segmentClone);
        }
        break;

      case msrMeasuresRepeat::kMeasuresRepeatBuildPhaseInReplicas:
        {
          // get fVoicePendingMeasuresRepeat's replicas
          S_msrMeasuresRepeatReplicas
            measuresRepeatReplicas =
              fVoicePendingMeasuresRepeat->
                getMeasuresRepeatReplicas ();

          // set segmentClone as the replicas's segment
          measuresRepeatReplicas->
            setMeasuresRepeatReplicasSegment (
        // JMI      inputLineNumber,
              segmentClone);
        }
        break;

      case msrMeasuresRepeat::kMeasuresRepeatBuildPhaseCompleted:
        {
          stringstream s;

          s <<
            "segment '" <<
            segmentClone->asShortString () <<
            "'cannot be added to a completed measures repeat";

          msrMusicXMLError (
            gGlobalOahOahGroup->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
        break;
    } // switch
  }

  else if (fVoicePendingRepeatDescrsStack.size ()) {
    // segmentClone belongs to a repeat

    // append segment to whichever part of the repeat is adequate
    S_msrRepeat
      currentRepeat =
        fVoicePendingRepeatDescrsStack.front ()->
          getRepeatDescrRepeat ();

    currentRepeat->
      appendSegmentToRepeat (
        inputLineNumber,
        segmentClone,
        "handleSegmentCloneEndInVoiceClone() 2");
  }

  else {
    // segmentClone is a voice-level segment

    // move fVoiceLastSegment to the initial voice elements list
    moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
      inputLineNumber,
      "handleSegmentCloneEndInVoiceClone()");
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceSegmentsDetails ()) {
    displayVoice (
      inputLineNumber,
      "handleSegmentCloneEndInVoiceClone() 3");
  }
#endif

  --gIndenter;
}

/* JMI
void msrVoice::finalizeRepeatEndInVoice (
  int    inputLineNumber,
  string measureNumber,
  int    repeatTimes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "finalizeRepeatEndInVoice() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:
      {
        // finalize current measure in voice
        finalizeLastAppendedMeasureInVoice (
          inputLineNumber);

        // fetch the repeat
        if (! fVoicePendingRepeatDescrsStack.size ()) {
          stringstream s;

          s <<
            "repeats stack is empty when attempting to finalize a repeat in voice '" <<
            asShortString () <<
            "' ";

          msrInternalError (
            gGlobalOahOahGroup->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

        S_msrRepeat
          repeat =
            fVoicePendingRepeatDescrsStack.front ()->
              getRepeatDescrRepeat ();

        // get repeat's common part
        S_msrRepeatCommonPart
          repeatCommonPart =
            repeat->
              getRepeatCommonPart ();

        // is there another repeat to nest into this repeat?
        if (! fVoicePendingRepeatDescrsStack.size ()) {
          // yes, this repeat contains a nested repeat

/ * JMI
          // move all the voice contents to the new repeat common part
          moveAllVoiceContentsToRepeatCommonPart (
            inputLineNumber,
            repeatCommonPart,
            "finalize repeat");
          // move the voice initial elements to the new repeat common part
        * /
        }

        else {
          // no, this repeat is at the voice-level

/ * JMI
          // move all the voice contents to the new repeat common part
          moveAllVoiceContentsToRepeatCommonPart ( // JMI
            inputLineNumber,
            repeatCommonPart,
            "finalizing voice-level repeat");
            * /
        }
      }
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "finalizeRepeatEndInVoice() 2");
  }
#endif
}
*/

void msrVoice::createMeasuresRepeatFromItsFirstMeasuresInVoice (
  int inputLineNumber,
  int measuresRepeatMeasuresNumber,
  int measuresRepeatSlashesNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Creating a measures repeat from its first measure in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;

    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "createMeasuresRepeatFromItsFirstMeasuresInVoice() 1");
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
      "attempting to create a measures repeat with " <<
      measuresRepeatMeasuresNumber <<
      " measures while current last segment only has " <<
      availableMeasuresNumber <<
      " available";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
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

  // create the measures repeat repeated segment
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Removing the last " <<
      singularOrPlural (
        measuresRepeatMeasuresNumber, "measure", "measures") <<
      " (to be repeated) from voice \"" <<
      fVoiceName <<
      endl;
  }
#endif

  for (int i = 0; i < measuresRepeatMeasuresNumber; ++i) {
    S_msrMeasure
      lastMeasure =
        removeLastMeasureFromVoice (
          inputLineNumber);

    repeatedSegment->
      prependMeasureToSegment (
        lastMeasure);
  } // for

  // create the measures repeat
  if (fVoicePendingMeasuresRepeat) {
    stringstream s;

    s <<
      "attempting to create a measures repeat while another one is pending";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fVoicePendingMeasuresRepeat =
    msrMeasuresRepeat::create (
      inputLineNumber,
      measuresRepeatMeasuresNumber,
      measuresRepeatSlashesNumber,
      this);

  // create the measures repeat pattern
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Creating a measures repeat pattern in voice \"" <<
      fVoiceName <<
      "\"" <<
      endl;
  }
#endif

  S_msrMeasuresRepeatPattern
    measuresRepeatPattern =
      msrMeasuresRepeatPattern::create (
        inputLineNumber,
        fVoicePendingMeasuresRepeat);

  // set the repeated segment as the measures repeat pattern segment
  measuresRepeatPattern->
    setMeasuresRepeatPatternSegment (
      repeatedSegment);

  // set the measures repeat pattern
  fVoicePendingMeasuresRepeat->
    setMeasuresRepeatPattern (
      measuresRepeatPattern);

  // create a new last segment to collect the measures repeat replicas,
  // containing the first, yet incomplete, replica
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Creating a new last segment with the first replica measure for voice \"" <<
      fVoiceName << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  createNewLastSegmentFromItsFirstMeasureForVoice (
    inputLineNumber,
    firstReplicaMeasure,
    "createMeasuresRepeatFromItsFirstMeasuresInVoice() 2");

  // keep the measures repeat pending

  // print resulting voice contents
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "createMeasuresRepeatFromItsFirstMeasuresInVoice() 3");
  }
#endif
}

void msrVoice::appendRestMeasuresToVoice (
  int               inputLineNumber,
  S_msrRestMeasures restMeasures)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    gLogStream <<
      "Appending rest measures '" <<
      restMeasures->asShortString () <<
      "' to voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    displayVoiceRestMeasuresAndVoice (
      inputLineNumber,
      "appendRestMeasuresToVoice() 1");
  }
#endif

  // analyze this rest measures's context
  switch (fVoicePendingRepeatDescrsStack.size ()) {
    case 0:
      // this rest measures is at the voice-level
      // -------------------------------------
      appendRestMeasuresToVoiceElementsList (
        restMeasures);
      break;

    default:
      // this rest measures is inside a repeat
      // -------------------------------------
      S_msrRepeat
        currentRepeat =
          fVoicePendingRepeatDescrsStack.front ()->
            getRepeatDescrRepeat ();

      currentRepeat->
        appendRestMeasuresToRepeat (
          inputLineNumber,
          restMeasures,
          "appendRestMeasuresToVoice() 2");
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    displayVoiceRestMeasuresAndVoice (
      inputLineNumber,
      "appendRestMeasuresToVoice() 3");
  }
#endif
}

void msrVoice::appendMeasuresRepeatToVoice (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeat)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Appending measures repeat '" <<
      measuresRepeat->asShortString () <<
      "' to voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "appendMeasuresRepeatToVoice() 1");
  }
#endif

  // analyze this measures repeats's context
  switch (fVoicePendingRepeatDescrsStack.size ()) {
    case 0:
      // this measures repeat is at the voice-level
      // -------------------------------------
      appendMeasuresRepeatToVoiceElementsList (
        measuresRepeat);
      break;

    default:
      // this measures repeat is inside a repeat
      // -------------------------------------
      S_msrRepeat
        currentRepeat =
          fVoicePendingRepeatDescrsStack.front ()->
            getRepeatDescrRepeat ();

      currentRepeat->
        appendMeasuresRepeatToRepeat (
          inputLineNumber,
          measuresRepeat,
          "appendMeasuresRepeatToVoice() 2");
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "appendMeasuresRepeatToVoice() 3");
  }
#endif
}

void msrVoice::appendPendingMeasuresRepeatToVoice (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Appending pending measures repeat to voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;

    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "appendPendingMeasuresRepeatToVoice() 1");
  }
#endif

  // does the current measures repeat exist?
  if (! fVoicePendingMeasuresRepeat) {
    stringstream s;

    s <<
      "attempting to append a pending measures repeat which doesn't exist";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // fetch the last segment's measure list
  list<S_msrMeasure>
    voiceLastSegmentMeasureList =
      fVoiceLastSegment->
        getSegmentMeasuresList ();

  // grab the just created last measure
  // in the last segment's measure list,
  // (i.e. the one containing:
  //   <measure-repeat type="stop"/>)
  // which is the next measure after the measures repeat
  if (! voiceLastSegmentMeasureList.size ()) {
    stringstream s;

    s <<
      "attempting to grab first measure of voice last segment, that contains none";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  S_msrMeasure
    nextMeasureAfterMeasuresRepeat =
      voiceLastSegmentMeasureList.back ();

// BOFBOFBOF JMI
  // remove the next measure from the last segment's measure list
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Removing last measure in last segment" <<
      "in voice \"" <<
      fVoiceName <<
      "\":" <<
      endl;
    ++gIndenter;
    gLogStream <<
      nextMeasureAfterMeasuresRepeat;
    gLogStream << endl;
    --gIndenter;
}
#endif

  voiceLastSegmentMeasureList.pop_back ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "appendPendingMeasuresRepeatToVoice() 2");
  }
#endif

  // create the measures repeat replicas contents
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Creating a measures repeat replicas contents for voice \"" <<
      fVoiceName << "\" is:" <<
      endl;
  }
#endif

  S_msrMeasuresRepeatReplicas
    measuresRepeatReplicas =
      msrMeasuresRepeatReplicas::create (
        inputLineNumber,
        fVoicePendingMeasuresRepeat);

  // set the voice last segment as the measures repeat replicas segment
  measuresRepeatReplicas->
    setMeasuresRepeatReplicasSegment (
      fVoiceLastSegment);

  fVoicePendingMeasuresRepeat->
    setMeasuresRepeatReplicas (
      measuresRepeatReplicas);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Setting pending measures repeat replicas segment in voice \"" <<
      getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fVoicePendingMeasuresRepeat->
    setMeasuresRepeatReplicas (
      measuresRepeatReplicas);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Setting measures repeat segment to voice last segment for voice \"" <<
      fVoiceName << "\"" <<
      endl;
  }
#endif

  fVoicePendingMeasuresRepeat->
    getMeasuresRepeatReplicas ()->
      setMeasuresRepeatReplicasSegment (
        fVoiceLastSegment);

  // forget about this voice last segment
  fVoiceLastSegment = nullptr; // JMI

  // append pending measures repeat to the voice
  appendMeasuresRepeatToVoice (
    inputLineNumber,
    fVoicePendingMeasuresRepeat);

  // create a new last segment to collect the remainder of the voice,
  // containing the next, yet incomplete, measure
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Creating a new last segment with the AAA measures repeat next measure for voice \"" <<
      fVoiceName << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  createNewLastSegmentFromItsFirstMeasureForVoice (
    inputLineNumber,
    nextMeasureAfterMeasuresRepeat,
    "appendPendingMeasuresRepeatToVoice() 3");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Appending pending measures repeat to voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;

    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "appendPendingMeasuresRepeatToVoice() 4");
  }
#endif
}

void msrVoice::createMeasuresRepeatAndAppendItToVoiceClone (
  int inputLineNumber,
  int measuresRepeatMeasuresNumber,
  int measuresRepeatSlashesNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
            "Creating measures repeat and appending it to voice clone \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;

    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "createMeasuresRepeatAndAppendItToVoiceClone() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:
      {
        // does the pending measures repeat exist?
        if (fVoicePendingMeasuresRepeat) {
          stringstream s;

          s <<
            "attempting to create a measures repeat while another one is pending";

          msrInternalError (
            gGlobalOahOahGroup->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

        // create the measures repeat
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
          gLogStream <<
            "Creating and appending a measures repeat in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        fVoicePendingMeasuresRepeat =
          msrMeasuresRepeat::create (
            inputLineNumber,
            measuresRepeatMeasuresNumber,
            measuresRepeatSlashesNumber,
            this);

        // create a measures repeat pattern from current last segment
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
          gLogStream <<
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
              fVoicePendingMeasuresRepeat);

        // set current last segment as the measures repeat pattern segment
        measuresRepeatPattern->
          setMeasuresRepeatPatternSegment (
            fVoiceLastSegment);

        // forget about this voice last segment
        fVoiceLastSegment = nullptr; // JMI

        // set the measures repeat pattern
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
          gLogStream <<
            "Setting repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
#endif

        fVoicePendingMeasuresRepeat->
          setMeasuresRepeatPattern (
            measuresRepeatPattern);

        // append the measures repeat to the list of initial elements
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
          gLogStream <<
            "Appending measures repeat to voice \"" <<
            fVoiceName <<
            "\"" <<
            endl;
        }
#endif

        // append pending measures repeat to the voice
        appendMeasuresRepeatToVoice (
          inputLineNumber,
          fVoicePendingMeasuresRepeat);
      }
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
            "Creating measures repeat and appending it to voice clone \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;

    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "createMeasuresRepeatAndAppendItToVoiceClone() 2");
  }
#endif
}

void msrVoice::setVoiceContainsRestMeasures (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    gLogStream <<
      "Voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains rest measures" <<
      endl;
  }
#endif

  fVoiceContainsRestMeasures = true;
}

void msrVoice::setVoiceContainsMeasuresRepeats (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains measures repeats" <<
      endl;
  }
#endif

  fVoiceContainsMeasuresRepeats = true;
}

void msrVoice::createRestMeasuresInVoice (
  int inputLineNumber,
  int restMeasuresNumber)
{
  // create a rest measures
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    gLogStream <<
      "Creating multiple rest measures in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", " <<
      singularOrPlural (
        restMeasuresNumber, "measure", "measures") <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    displayVoiceRestMeasuresAndVoice (
      inputLineNumber,
      "createRestMeasuresInVoice() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:
      {
        ++gIndenter;

        // grab the just created last measure from the voice,
        // (i.e. the one containing:
        //   <multiple-rest ... type="start">2</multiple-rest>)
        // which is the first rest measure
        S_msrMeasure
          firstRestMeasure =
            removeLastMeasureFromVoice (
              inputLineNumber);

        // move the current voice last segment to the initial elements list
        moveVoiceLastSegmentToInitialVoiceElementsIfRelevant ( //JMI
          inputLineNumber,
          "createRestMeasuresInVoice() 2");

        // create the rest measures
        if (fVoicePendingRestMeasures) {
          stringstream s;

          s <<
            "attempting to create a rest measures while another one is pending";

          msrInternalError (
            gGlobalOahOahGroup->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

        fVoicePendingRestMeasures =
          msrRestMeasures::create (
            inputLineNumber,
            firstRestMeasure->getFullMeasureWholeNotesDuration (),
            restMeasuresNumber,
            this);

         // remember fVoicePendingRestMeasures for later next measure number setting
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
          gLogStream <<
            "Registering rest measures as waiting for its next measure number" <<
            ", restMeasuresNumber = " <<
            restMeasuresNumber <<
            " in voice \"" <<
            fVoiceName << "\"" <<
            endl;
        }
#endif

        fVoiceRestMeasuresWaitingForItsNextMeasureNumber =
          fVoicePendingRestMeasures;

        fVoiceRemainingRestMeasures =
          restMeasuresNumber;

#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
          gLogStream <<
            "Setting fVoiceRemainingRestMeasures to '" <<
            fVoiceRemainingRestMeasures <<
            "' in voice \"" <<
            fVoiceName << "\"" <<
            endl;
        }
#endif

        // create a new segment to collect the rest measures,
        // containing the first, rest measure
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
          gLogStream <<
            "Creating a new last segment containing the first rest measure in voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        createNewLastSegmentFromItsFirstMeasureForVoice (
          inputLineNumber,
          firstRestMeasure,
          "createRestMeasuresInVoice() 3");

        // this voice contails rest measures
        this->setVoiceContainsRestMeasures (
          inputLineNumber);

        // keep the rest measures pending

        --gIndenter;
      }
      break;
  } // switch

  // print resulting voice contents
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    displayVoiceRestMeasuresAndVoice (
      inputLineNumber,
      "createRestMeasuresInVoice() 4");
  }
#endif
}

void msrVoice::addRestMeasuresToVoice (
  int    inputLineNumber,
  string previousMeasureNumber,
  int    restMeasuresNumber)
{
  // create a rest measures
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    gLogStream <<
      "Adding multiple rest measures in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", " <<
      singularOrPlural (
        restMeasuresNumber, "measure", "measures") <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    displayVoiceRestMeasuresAndVoice (
      inputLineNumber,
      "addRestMeasuresToVoice() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:
      {
        ++gIndenter;

        // move the current voice last segment to the initial elements list
        moveVoiceLastSegmentToInitialVoiceElementsIfRelevant ( //JMI
          inputLineNumber,
          "addRestMeasuresToVoice() 2");

        // create the rest measures
        if (fVoicePendingRestMeasures) {
          stringstream s;

          s <<
            "attempting to create a rest measures while another one is pending";

          msrInternalError (
            gGlobalOahOahGroup->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

/* JMI
        fVoicePendingRestMeasures =
          msrRestMeasures::create (
            inputLineNumber,
            firstRestMeasure->getFullMeasureWholeNotesDuration (),
            restMeasuresNumber,
            this);
*/

         // remember fVoicePendingRestMeasures for later next measure number setting JMI ???
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
          gLogStream <<
            "Registering rest measures as waiting for its next measure number" <<
            ", restMeasuresNumber = " <<
            restMeasuresNumber <<
            " in voice \"" <<
            fVoiceName << "\"" <<
            endl;
        }
#endif

        fVoiceRestMeasuresWaitingForItsNextMeasureNumber =
          fVoicePendingRestMeasures;

        fVoiceRemainingRestMeasures =
          restMeasuresNumber;

#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
          gLogStream <<
            "Setting fVoiceRemainingRestMeasures to '" <<
            fVoiceRemainingRestMeasures <<
            "' in voice \"" <<
            fVoiceName << "\"" <<
            endl;
        }
#endif

        // create a new segment to collect the rest measures,
        // containing the first, rest measure
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
          gLogStream <<
            "Creating a new last segment containing the first rest measure in voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

/* JMI
        createNewLastSegmentFromItsFirstMeasureForVoice (
          inputLineNumber,
          firstRestMeasure,
          "addRestMeasuresToVoice() 3");
*/

        // this voice contails rest measures
        this->setVoiceContainsRestMeasures (
          inputLineNumber);

        // keep the rest measures pending

        --gIndenter;
      }
      break;
  } // switch

  // print resulting voice contents
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    displayVoiceRestMeasuresAndVoice (
      inputLineNumber,
      "addRestMeasuresToVoice() 4");
  }
#endif
}

void msrVoice::appendPendingRestMeasuresToVoice (
    int inputLineNumber)
{
  // a rest measures is a voice element,
  // and can be voice-level as well as part of a repeat

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    displayVoiceRestMeasuresAndVoice (
      inputLineNumber,
      "appendPendingRestMeasuresToVoice() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:
      {
        // does the pending rest measures exist?
        if (! fVoicePendingRestMeasures) {
          stringstream s;

          s <<
            "attempting to append a pending rest measures which doesn't exist";

          msrInternalError (
            gGlobalOahOahGroup->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
          gLogStream <<
            "Appending pending rest measures '" <<
            fVoicePendingRestMeasures->asShortString () <<
            "' to voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        // create the rest measures contents
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
          gLogStream <<
            "Creating a rest measures contents for voice \"" <<
            fVoiceName << "\" is:" <<
            endl;
        }
#endif

        S_msrRestMeasuresContents
          restMeasuresContents =
            msrRestMeasuresContents::create (
              inputLineNumber,
              fVoicePendingRestMeasures);

        // set voice last segment as the rest measures contents segment
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
          gLogStream <<
            "Setting current last segment as rest measures contents segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
#endif

        restMeasuresContents->
          setRestMeasuresContentsSegment (
            inputLineNumber,
            fVoiceLastSegment);

        // set restMeasuresContents as the rest measures contents
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
          gLogStream <<
            "Setting rest measures contents in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
#endif

        fVoicePendingRestMeasures->
          setRestMeasuresContents (
            restMeasuresContents);

        // set pending rest measures last measure purist number
        fVoicePendingRestMeasures->
          setRestMeasuresLastMeasurePuristMeasureNumber (
            inputLineNumber);

        // forget about this voice last segment
        fVoiceLastSegment = nullptr;

        // append pending rest measures to the voice
        appendRestMeasuresToVoice (
          inputLineNumber,
          fVoicePendingRestMeasures);

        // forget about this pending rest measures
        fVoicePendingRestMeasures = nullptr;
      }
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    displayVoiceRestMeasuresAndVoice (
      inputLineNumber,
      "appendPendingRestMeasuresToVoice() 2");
  }
#endif
}

void msrVoice::handleRestMeasuresStartInVoiceClone (
  int               inputLineNumber,
  S_msrRestMeasures restMeasures)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    gLogStream <<
      "Handling rest measures start in voice clone \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    displayVoiceRestMeasuresAndVoice (
      inputLineNumber,
      "handleRestMeasuresStartInVoiceClone() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:

      // is there a voice last segment?
      if (fVoiceLastSegment) {

        // are there measures in the voice last segment?
        if (fVoiceLastSegment->getSegmentMeasuresList ().size ()) {

          // finalize current measure in voice
          finalizeLastAppendedMeasureInVoice (
            inputLineNumber);

          // move voice last segment to the list of initial elements
          moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
            inputLineNumber,
            "handleRestMeasuresStartInVoiceClone() 2");

          // forget about fVoicePendingRestMeasures
          fVoicePendingRestMeasures = nullptr;

          // create a new last segment containing a new measure for the voice
          createNewLastSegmentForVoice (
            inputLineNumber,
            "handleRestMeasuresStartInVoiceClone()");

#ifdef TRACING_IS_ENABLED
          if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
            displayVoiceRestMeasuresAndVoice (
              inputLineNumber,
              "handleRestMeasuresStartInVoiceClone() 3");
          }
#endif
        }
      }

      // is there already a current rest measures in this voice?
      if (fVoicePendingRestMeasures) {
        stringstream s;

        s <<
          "current voice rest measures is not null when attempting to handle rest measures start '" <<
          "' in voice clone '" <<
          asShortString () <<
          "' ";

        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

      // create the rest measures clone and register it
      fVoicePendingRestMeasures =
        restMeasures->
          createRestMeasuresNewbornClone (
            this);

      // this voice contails rest measures
      this->setVoiceContainsRestMeasures (
        inputLineNumber);

      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    displayVoiceRestMeasuresAndVoice (
      inputLineNumber,
      "handleRestMeasuresStartInVoiceClone() 4");
  }
#endif

  --gIndenter;
}

void msrVoice::handleRestMeasuresEndInVoiceClone (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    gLogStream <<
      "Handling rest measures end in voice clone \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    displayVoiceRestMeasuresAndVoice (
      inputLineNumber,
      "handleRestMeasuresEndInVoiceClone() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:

      // is there a current rest measures in this voice?
      if (! fVoicePendingRestMeasures) {
        stringstream s;

        s <<
          "current voice rest measures is null when attempting to handle rest measures end '" <<
          "' in voice clone '" <<
          asShortString () <<
          "' ";

        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

      // set pending rest measures last measure purist number
      fVoicePendingRestMeasures->
        setRestMeasuresLastMeasurePuristMeasureNumber (
          inputLineNumber);

      // append current voice rest measure to the initial voice elements list
      appendRestMeasuresToInitialVoiceElements (
        inputLineNumber,
        fVoicePendingRestMeasures,
        "handleRestMeasuresEndInVoiceClone() 2");

      // forget about fVoicePendingRestMeasures
      fVoicePendingRestMeasures = nullptr;

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
        displayVoice (
          inputLineNumber,
          "handleRestMeasuresEndInVoiceClone() 3");
      }
#endif
    break;
  } // switch

  --gIndenter;
}

void msrVoice::handleRestMeasuresContentsStartInVoiceClone (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    gLogStream <<
      "Handling rest measures contents start in voice clone \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    displayVoiceRestMeasuresAndVoice (
      inputLineNumber,
      "handleRestMeasuresContentsStartInVoiceClone() 1");
  }
#endif

  ++gIndenter;

  if (! fVoicePendingRestMeasures) {
    stringstream s;

    s <<
      "current voice rest measures is null when attempting to handle rest measures contents start '" <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create fVoicePendingRestMeasures' rest contents
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    gLogStream <<
      "Creating a rest measures contents upon its start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  S_msrRestMeasuresContents
    restMeasuresContents =
      msrRestMeasuresContents::create (
        inputLineNumber,
        fVoicePendingRestMeasures);

  // register it in fVoicePendingRestMeasures
  fVoicePendingRestMeasures->
    setRestMeasuresContents (
      restMeasuresContents);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    displayVoiceRestMeasuresAndVoice (
      inputLineNumber,
      "handleRestMeasuresContentsStartInVoiceClone() 2");
  }
#endif

  --gIndenter;
}

void msrVoice::handleRestMeasuresContentsEndInVoiceClone (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    gLogStream <<
      "Handling rest measures contents end in voice clone \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    displayVoiceRestMeasuresAndVoice (
      inputLineNumber,
      "handleRestMeasuresContentsEndInVoiceClone() 1");
  }
#endif

  if (! fVoicePendingRestMeasures) {
    stringstream s;

    s <<
      "current voice rest measures is null when attempting to handle rest measures contents end '" <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // get fVoicePendingRestMeasures's contents
  S_msrRestMeasuresContents
    restMeasuresContents =
      fVoicePendingRestMeasures->
        getRestMeasuresContents ();

  // set fVoiceLastSegment as restMeasuresContents' segment
  restMeasuresContents->
    setRestMeasuresContentsSegment (
      inputLineNumber,
      fVoiceLastSegment);

  // forget about fVoiceLastSegment
 // fVoiceLastSegment = nullptr;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    displayVoiceRestMeasuresAndVoice (
      inputLineNumber,
      "handleRestMeasuresContentsEndInVoiceClone() 2");
  }
#endif

  --gIndenter;
}

void msrVoice::appendRestMeasuresCloneToVoiceClone (
  int               inputLineNumber,
  S_msrRestMeasures restMeasuresClone)
{
  ++gIndenter;

  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    restMeasuresClone != nullptr,
    "restMeasuresClone is null");

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:
      {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
          gLogStream <<
            "Appending rest measures clone '" <<
            restMeasuresClone->asString () <<
            "' to voice clone \"" <<
            getVoiceName () << "\"" <<
            ", line " << inputLineNumber <<
            endl;

          displayVoiceRestMeasuresAndVoice (
            inputLineNumber,
            "appendRestMeasuresCloneToVoiceClone() 1");
        }
#endif

        // is rest measures nested in a repeat?
        if (fVoicePendingRepeatDescrsStack.size ()) {
          // yes

          S_msrRepeat
            currentRepeat =
              fVoicePendingRepeatDescrsStack.front ()->
                getRepeatDescrRepeat ();

          // grab the rest measures segment, i.e. the voice's last segment JMI ???
          S_msrSegment
            restMeasuresSegment =
              fVoiceLastSegment;

          // append it to the current repeat's common part
          currentRepeat->
            getRepeatCommonPart ()->
              appendSegmentToRepeatCommonPart ( // NO !!!
                inputLineNumber,
                restMeasuresSegment,
                "appendRestMeasuresCloneToVoiceClone() 2");

          // forget about this voice last segment
          fVoiceLastSegment = nullptr;
        }

        else {
          // no
          // JMI ???
        }

        // append the rest measures clone to the voice
        appendRestMeasuresToVoice (
          inputLineNumber,
          restMeasuresClone);

        // print resulting voice contents
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
          displayVoice (
            inputLineNumber,
            "appendRestMeasuresCloneToVoiceClone() 3");
        }
#endif
      }
      break;
  } // switch

  --gIndenter;
}

void msrVoice::appendRepeatCloneToVoiceClone (
  int         inputLineNumber,
  S_msrRepeat repeatCLone)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending repeat clone '" <<
      repeatCLone->asString () <<
      "' to voice clone \"" <<
      getVoiceName () <<  "\"" <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "appendRepeatCloneToVoiceClone() 1");
  }
#endif

  ++gIndenter;

  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    repeatCLone != nullptr,
    "repeatCLone is null");

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:
      {
        // pushing repeat clone as the (new) current repeat
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceRepeats ()) {
          gLogStream <<
            "Pushing repeat clone as the new current repeat in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
#endif

        // push the repeat clone onto the voice's repeat descrs stack
        pushRepeatOntoRepeatDescrsStack (
          inputLineNumber,
          repeatCLone,
          "appendRepeatCloneToVoiceClone() 2");

        // append it to the list of initial elements
        appendRepeatCloneToInitialVoiceElements (
          inputLineNumber,
          repeatCLone,
          "appendRepeatCloneToVoiceClone() 3");
      }
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "appendRepeatCloneToVoiceClone() 4");
  }
#endif

  --gIndenter;
}

void msrVoice::handleMeasuresRepeatStartInVoiceClone (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeat)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Handling measures repeat start in voice clone \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "handleMeasuresRepeatStartInVoiceClone() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:
      // is there already a current measures repeat in this voice?
      if (fVoicePendingMeasuresRepeat) {
        stringstream s;

        s <<
          "current voice measures repeat is not null when attempting to handle measures repeat start '" <<
          "' in voice clone '" <<
          asShortString () <<
          "' ";

        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

      // create the measures repeat clone and register it
      fVoicePendingMeasuresRepeat =
        measuresRepeat->
          createMeasuresRepeatNewbornClone (
            this);

      // this voice contails measures repeats
      this->setVoiceContainsMeasuresRepeats (
        inputLineNumber);

      // set fVoicePendingMeasuresRepeat's build phase to completed
      fVoicePendingMeasuresRepeat->
        setCurrentMeasuresRepeatBuildPhaseKind (
          msrMeasuresRepeat::kMeasuresRepeatBuildPhaseJustCreated);

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
        displayVoiceMeasuresRepeatAndVoice (
          inputLineNumber,
          "handleMeasuresRepeatStartInVoiceClone() 2");
      }
#endif
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "handleMeasuresRepeatStartInVoiceClone() 3");
  }
#endif

  --gIndenter;
}

void msrVoice::handleMeasuresRepeatEndInVoiceClone (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Handling measures repeat end in voice clone \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "handleMeasuresRepeatEndInVoiceClone() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:
      // is there a current measures repeat in this voice?
      if (! fVoicePendingMeasuresRepeat) {
        stringstream s;

        s <<
          "current voice measures repeat is null when attempting to handle measures repeat end '" <<
          "' in voice clone '" <<
          asShortString () <<
          "' ";

        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

      // append current voice rest measure to the initial voice elements list
      appendMeasuresRepeatToInitialVoiceElements (
        inputLineNumber,
        fVoicePendingMeasuresRepeat,
        "handleMeasuresRepeatEndInVoiceClone() 2");

      // forget about fVoicePendingMeasuresRepeat
      fVoicePendingMeasuresRepeat = nullptr;

    break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "handleMeasuresRepeatEndInVoiceClone() 3");
  }
#endif

  --gIndenter;
}

void msrVoice::handleMeasuresRepeatPatternStartInVoiceClone (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Handling measures repeat start in voice clone \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "handleMeasuresRepeatPatternStartInVoiceClone() 1");
  }
#endif

  if (! fVoicePendingMeasuresRepeat) {
    stringstream s;

    s <<
      "current voice measures repeat is null when attempting to handle measures repeat pattern start '" <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create fVoicePendingMeasuresRepeat' rest pattern
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Creating a measures repeat pattern upon its start in voice \"" <<
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
        fVoicePendingMeasuresRepeat);

  // register it in fVoicePendingMeasuresRepeat
  fVoicePendingMeasuresRepeat->
    setMeasuresRepeatPattern (
      measuresRepeatPattern);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "handleMeasuresRepeatPatternStartInVoiceClone() 2");
  }
#endif

  --gIndenter;
}

void msrVoice::handleMeasuresRepeatPatternEndInVoiceClone (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Handling measures repeat end in voice clone \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "handleMeasuresRepeatPatternEndInVoiceClone() 1");
  }
#endif

  if (! fVoicePendingMeasuresRepeat) {
    stringstream s;

    s <<
      "current voice measures repeat is null when attempting to handle measures repeat pattern end '" <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // get fVoicePendingMeasuresRepeat's pattern
  S_msrMeasuresRepeatPattern
    measuresRepeatPattern =
      fVoicePendingMeasuresRepeat->
        getMeasuresRepeatPattern ();

  // set fVoiceLastSegment as measuresRepeatPattern' segment
  measuresRepeatPattern->
    setMeasuresRepeatPatternSegment (
      fVoiceLastSegment);

  // forget about fVoiceLastSegment
 // fVoiceLastSegment = nullptr;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "handleMeasuresRepeatPatternEndInVoiceClone() 2");
  }
#endif

  --gIndenter;
}

void msrVoice::handleMeasuresRepeatReplicasStartInVoiceClone (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Handling measures repeat start in voice clone \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "handleMeasuresRepeatReplicasStartInVoiceClone() 1");
  }
#endif

  if (! fVoicePendingMeasuresRepeat) {
    stringstream s;

    s <<
      "current voice measures repeat is null when attempting to handle measures repeat replicas start '" <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create fVoicePendingMeasuresRepeat' replicas
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Creating a measures repeat replicas upon its start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  S_msrMeasuresRepeatReplicas
    measuresRepeatReplicas =
      msrMeasuresRepeatReplicas::create (
        inputLineNumber,
        fVoicePendingMeasuresRepeat);

  // register it in fVoicePendingMeasuresRepeat
  fVoicePendingMeasuresRepeat->
    setMeasuresRepeatReplicas (
      measuresRepeatReplicas);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "handleMeasuresRepeatReplicasStartInVoiceClone() 2");
  }
#endif

  --gIndenter;
}

void msrVoice::handleMeasuresRepeatReplicasEndInVoiceClone (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Handling measures repeat end in voice clone \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "handleMeasuresRepeatReplicasEndInVoiceClone() 1");
  }
#endif

  if (! fVoicePendingMeasuresRepeat) {
    stringstream s;

    s <<
      "current voice measures repeat is null when attempting to handle measures repeat replicas end '" <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // get fVoicePendingMeasuresRepeat's replicas
  S_msrMeasuresRepeatReplicas
    measuresRepeatReplicas =
      fVoicePendingMeasuresRepeat->
        getMeasuresRepeatReplicas ();

  // set fVoiceLastSegment as measuresRepeatReplicas' segment
  measuresRepeatReplicas->
    setMeasuresRepeatReplicasSegment (
      fVoiceLastSegment);

  // forget about fVoiceLastSegment
 // fVoiceLastSegment = nullptr;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "handleMeasuresRepeatReplicasEndInVoiceClone() 2");
  }
#endif

  --gIndenter;
}

void msrVoice::appendMeasuresRepeatCloneToVoiceClone (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeatClone)
{
  ++gIndenter;

  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    measuresRepeatClone != nullptr,
    "measuresRepeatClone is null");

  switch (fVoiceKind) { // superflous JMI ???
    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:
      {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
          gLogStream <<
            "Appending measures repeat clone '" <<
            measuresRepeatClone->asString () <<
            "' to voice clone \"" <<
            getVoiceName () << "\"" <<
            ", line " << inputLineNumber <<
            endl;

          displayVoiceMeasuresRepeatAndVoice (
            inputLineNumber,
            "appendMeasuresRepeatCloneToVoiceClone() 1");
        }
#endif

        // is measures repeat nested in a repeat?
        if (fVoicePendingRepeatDescrsStack.size ()) {
          // yes

          S_msrRepeat
            currentRepeat =
              fVoicePendingRepeatDescrsStack.front ()->
                getRepeatDescrRepeat ();

          // grab the measures repeat segment, i.e. the voice's last segment JMI ???
          S_msrSegment
            measuresRepeatSegment =
              fVoiceLastSegment;

          // append it to the current repeat's common part
          currentRepeat->
            getRepeatCommonPart ()->
              appendSegmentToRepeatCommonPart ( // NO !!!
                inputLineNumber,
                measuresRepeatSegment,
                "appendMeasuresRepeatCloneToVoiceClone() 2");

          // forget about this voice last segment
          fVoiceLastSegment = nullptr;
        }

        else {
          // no
          // JMI ???
        }

        // append the measures repeat clone to the voice
        appendMeasuresRepeatToVoice (
          inputLineNumber,
          measuresRepeatClone);

        // print resulting voice contents
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
          displayVoiceMeasuresRepeatAndVoice (
            inputLineNumber,
            "appendMeasuresRepeatCloneToVoiceClone() 3");
        }
#endif
      }
      break;
  } // switch

  --gIndenter;
}

void msrVoice::handleHookedRepeatEndingEndInVoice (
  int       inputLineNumber,
  string    repeatEndingNumber) // may be "1, 2"
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a hooked repeat ending in voice \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHookedRepeatEndingEndInVoice() 1");
  }
#endif

  ++gIndenter;

/* JMI
        unsigned int voicerepeatDescrsStackSize =
          fVoicePendingRepeatDescrsStack.size ();
*/

  if (! fVoicePendingRepeatDescrsStack.size ()) {
    stringstream s;

    s <<
      "repeats stack is empty when attempting to handle a hooked repeat ending end in voice '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // finalize current measure in voice
  finalizeLastAppendedMeasureInVoice (
    inputLineNumber);

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatDescrsStack.front ()->
        getRepeatDescrRepeat ();

  // create a hooked repeat ending
  msrRepeatEndingKind
    repeatEndingKind =
      msrRepeatEndingKind::kEndingHooked;

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
    "handleHookedRepeatEndingEndInVoice() 2");

  // add the repeat ending to the voice current repeat
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending a " <<
      msrRepeatEndingKindAsString (
        repeatEndingKind) <<
      " repeat ending to current repeat in voice \"" <<
      fVoiceName <<
      "\"" <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "before adding a hooked repeat ending to current repeat");
  }
#endif

  currentRepeat->
    addRepeatEndingToRepeat (
      inputLineNumber,
      repeatEnding);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHookedRepeatEndingEndInVoice() 3");
  }
#endif

  --gIndenter;
}

void msrVoice::handleHooklessRepeatEndingEndInVoice (
  int       inputLineNumber,
  string    repeatEndingNumber) // may be "1, 2"
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a hookless repeat ending in voice \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      endl;  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHooklessRepeatEndingEndInVoice() 1");
  }
#endif

  ++gIndenter;

  if (! fVoicePendingRepeatDescrsStack.size ()) {
    stringstream s;

    s <<
      "repeats stack is empty when attempting to handle a hookless repeat ending end in voice '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // finalize current measure in voice
  finalizeLastAppendedMeasureInVoice (
    inputLineNumber);

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatDescrsStack.front ()->
        getRepeatDescrRepeat ();

  // create a hookless repeat ending
  msrRepeatEndingKind
    repeatEndingKind =
      msrRepeatEndingKind::kEndingHookless;

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
    "handleHookedRepeatEndingEndInVoice() 2");

  // add the repeat ending it to the voice current repeat
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending a " <<
      msrRepeatEndingKindAsString (
        repeatEndingKind) <<
      " repeat ending to current repeat in voice \"" <<
      fVoiceName <<
      "\"" <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "before adding a hookless repeat ending to current repeat");
  }
#endif

  currentRepeat->
    addRepeatEndingToRepeat (
      inputLineNumber,
      repeatEnding);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "after adding a hookless repeat ending to current repeat");
  }
#endif

  // append currentRepeat to the list of initial elements
  appendRepeatCloneToInitialVoiceElements (
    inputLineNumber,
    currentRepeat,
    "handleHooklessRepeatEndingEndInVoice() 3");

  // pop it from the voice's repeat descrs stack
  popRepeatFromRepeatDescrsStack (
    inputLineNumber,
    currentRepeat,
    "handleHooklessRepeatEndingEndInVoice");

  // create a new last segment containing a new measure for the voice
  createNewLastSegmentForVoice (
    inputLineNumber,
    "handleHooklessRepeatEndingEndInVoice() 4");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHooklessRepeatEndingEndInVoice() 5");
  }
#endif

  --gIndenter;
}

void msrVoice::handleRepeatEndingEndInVoice (
  int       inputLineNumber,
  string    repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind
            repeatEndingKind)
{
  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:
      {
        switch (repeatEndingKind) {
          case msrRepeatEndingKind::kEndingHooked:
            handleHookedRepeatEndingEndInVoice (
              inputLineNumber,
              repeatEndingNumber);
            break;

          case msrRepeatEndingKind::kEndingHookless:
            handleHooklessRepeatEndingEndInVoice (
              inputLineNumber,
              repeatEndingNumber);
            break;
        } // switch
      break;
    }
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "appendRepeatEndingToVoice() 0");
  }
#endif
}

void msrVoice::handleRepeatCommonPartStartInVoiceClone (
  int inputLineNumber) // may be "1, 2"
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat common part start in voice clone \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatCommonPartStartInVoiceClone() 1");
  }
#endif

  ++gIndenter;

  if (! fVoicePendingRepeatDescrsStack.size ()) {
    stringstream s;

    s <<
      "repeats stack is empty when attempting to handle repeat common part start '" <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatDescrsStack.front ()->
        getRepeatDescrRepeat ();

  // create currentRepeat's common part
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a repeat common part upon its start in voice \"" <<
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatCommonPartStartInVoiceClone() 2");
  }
#endif

  --gIndenter;
}

void msrVoice::handleRepeatCommonPartEndInVoiceClone (
  int inputLineNumber) // may be "1, 2"
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat common part end in voice clone \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatCommonPartEndInVoiceClone() 1");
  }
#endif

  ++gIndenter;

  if (! fVoicePendingRepeatDescrsStack.size ()) {
    stringstream s;

    s <<
      "repeats stack is empty when attempting to handle repeat ending '" <<
 //     repeatEnding->asShortString () <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatDescrsStack.front ()->
        getRepeatDescrRepeat ();

  // get currentRepeat's common part
  S_msrRepeatCommonPart
    repeatCommonPart =
      currentRepeat->
        getRepeatCommonPart ();

/* JMI
  // fetch the last segment's last measure
  S_msrMeasure
    voiceLastSegmentLastMeasure =
      fVoiceLastSegment->
        fetchLastMeasureFromSegment (
          inputLineNumber,
          "handleRepeatCommonPartEndInVoiceClone() 2");
*/

  // set voice current after repeat component phase kind
  setCurrentVoiceRepeatPhaseKind (
    inputLineNumber,
    msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterCommonPart);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatCommonPartEndInVoiceClone() 3");
  }
#endif

  --gIndenter;
}

void msrVoice::handleHookedRepeatEndingEndInVoiceClone (
  int    inputLineNumber,
  string repeatEndingNumber) // may be "1, 2"
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a hooked repeat ending in voice clone \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHookedRepeatEndingEndInVoiceClone() 1");
  }
#endif

  ++gIndenter;

  if (! fVoicePendingRepeatDescrsStack.size ()) {
    stringstream s;

    s <<
      "repeats stack is empty when attempting to handle hooked repeat ending '" <<
 //     repeatEnding->asShortString () <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatDescrsStack.front ()->
        getRepeatDescrRepeat ();

  // create a hooked repeat ending
  msrRepeatEndingKind
    repeatEndingKind =
      msrRepeatEndingKind::kEndingHooked;

  S_msrRepeatEnding
    repeatEnding =
      msrRepeatEnding::create (
        inputLineNumber,
        repeatEndingNumber,
        repeatEndingKind,
        currentRepeat);

  // fetch the last segment's last measure
  S_msrMeasure
    voiceLastSegmentLastMeasure =
      fVoiceLastSegment->
        fetchLastMeasureFromSegment (
          inputLineNumber,
          "handleHookedRepeatEndingEndInVoiceClone() 2");

  // set voice current after repeat component phase kind
  setCurrentVoiceRepeatPhaseKind (
    inputLineNumber,
    msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterHookedEnding);

  // move the voice last segment to repeatEnding
  moveVoiceLastSegmentToRepeatEnding (
    inputLineNumber,
    repeatEnding,
    "handleHookedRepeatEndingEndInVoiceClone()");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHookedRepeatEndingEndInVoiceClone() 3");
  }
#endif

  --gIndenter;
}

void msrVoice::handleHooklessRepeatEndingEndInVoiceClone (
  int    inputLineNumber,
  string repeatEndingNumber) // may be "1, 2"
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a hookless repeat ending in voice clone \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHooklessRepeatEndingEndInVoiceClone() 1");
  }
#endif

  ++gIndenter;

  if (! fVoicePendingRepeatDescrsStack.size ()) {
    stringstream s;

    s <<
      "repeats stack is empty when attempting to handle hookless repeat ending '" <<
 //     repeatEnding->asShortString () <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatDescrsStack.front ()->
        getRepeatDescrRepeat ();

  // create a hookless repeat ending
  msrRepeatEndingKind
    repeatEndingKind =
      msrRepeatEndingKind::kEndingHookless;

  S_msrRepeatEnding
    repeatEnding =
      msrRepeatEnding::create (
        inputLineNumber,
        repeatEndingNumber,
        repeatEndingKind,
        currentRepeat);

  // fetch the last segment's last measure
  S_msrMeasure
    voiceLastSegmentLastMeasure =
      fVoiceLastSegment->
        fetchLastMeasureFromSegment (
          inputLineNumber,
          "handleHookedRepeatEndingEndInVoiceClone() 2");

  // set voice current after repeat component phase kind
  setCurrentVoiceRepeatPhaseKind (
    inputLineNumber,
    msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterHooklessEnding);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHooklessRepeatEndingEndInVoiceClone() 3");
  }
#endif

  --gIndenter;
}

void msrVoice::handleRepeatEndingEndInVoiceClone (
  int       inputLineNumber,
  string    repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind
            repeatEndingKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "appendRepeatEndingToVoiceClone() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:
      {
        switch (repeatEndingKind) {
          case msrRepeatEndingKind::kEndingHooked:
            handleHookedRepeatEndingEndInVoiceClone (
              inputLineNumber,
              repeatEndingNumber);
            break;

          case msrRepeatEndingKind::kEndingHookless:
            handleHooklessRepeatEndingEndInVoiceClone (
              inputLineNumber,
              repeatEndingNumber);
            break;
        } // switch
      break;
    }
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "appendRepeatEndingToVoiceClone() 2");
  }
#endif
}

void msrVoice::handleRepeatStartInVoiceClone (
  int         inputLineNumber,
  S_msrRepeat repeat)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling repeat start in voice clone \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatStartInVoiceClone() 1");
  }
#endif

  ++gIndenter;

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:
      // is there a voice last segment?
      if (fVoiceLastSegment) {

        // fetch last segment's measures list
        list<S_msrMeasure>
          voiceLastSegmentMeasuresList =
            fVoiceLastSegment->
              getSegmentMeasuresList ();

        // are there measures in the voice last segment?
        if (voiceLastSegmentMeasuresList.size ()) {
          // finalize current measure in voice
          finalizeLastAppendedMeasureInVoice (
            inputLineNumber);

          // move current last segment to the list of initial elements
          moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
            inputLineNumber,
            "handleRepeatStartInVoiceClone() 2");

#ifdef TRACING_IS_ENABLED
          if (gGlobalTraceOahGroup->getTraceRepeats ()) {
            displayVoiceRepeatsStackSummary (
              inputLineNumber,
              "handleRepeatStartInVoiceClone() 3");
          }
#endif
        }
      }

      // create the repeat clone and stack it
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceRepeats ()) {
        gLogStream <<
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
            "handleRepeatStartInVoiceClone() 4");

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceRepeats ()) {
        displayVoiceRepeatsStackSummary (
          inputLineNumber,
          "handleRepeatStartInVoiceClone() 5");
      }
#endif
      break;
  } // switch

  --gIndenter;
}

void msrVoice::handleRepeatEndInVoiceClone (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat end in voice clone \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:
      {
      /* JMI
        // finalize current measure in voice
        finalizeLastAppendedMeasureInVoice (
          inputLineNumber);
          */

#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceRepeats ()) {
          displayVoiceRepeatsStackSummary (
            inputLineNumber,
            "handleRepeatEndInVoiceClone() 1");
        }
#endif

        // is there a current repeat?
        switch (fVoicePendingRepeatDescrsStack.size ()) {
          case 0:
            {
              stringstream s;

              s <<
                "repeats stack is empty when attempting to handle a repeat end in voice clone '" <<
                asShortString () <<
                "' ";

              msrInternalError (
                gGlobalOahOahGroup->getInputSourceName (),
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
                  fVoicePendingRepeatDescrsStack.front ()->
                    getRepeatDescrRepeat ();

              // get currentRepeat's common part
              S_msrRepeatCommonPart
                repeatCommonPart =
                  currentRepeat->
                    getRepeatCommonPart ();

              // append currentRepeat to the list of initial elements
              appendRepeatToInitialVoiceElements (
                inputLineNumber,
                currentRepeat,
                "handleRepeatEndInVoiceClone() 2");

              // pop currentRepeat from the voice's repeat descrs stack
              popRepeatFromRepeatDescrsStack (
                inputLineNumber,
                currentRepeat,
                "handleRepeatEndInVoiceClone() 3");

              // forget about the voice last segment JMI ??? BOF
              fVoiceLastSegment = nullptr;
            }
            break;

          default:
            {
              // this is a nested repeat

              // fetch currentRepeat
              S_msrRepeat
                currentRepeat =
                  fVoicePendingRepeatDescrsStack.front ()->
                    getRepeatDescrRepeat ();

              // get currentRepeat's common part
              S_msrRepeatCommonPart
                repeatCommonPart =
                  currentRepeat->
                    getRepeatCommonPart ();

              // append currentRepeat to the list of initial elements
              appendRepeatCloneToInitialVoiceElements (
                inputLineNumber,
                currentRepeat,
                "handleRepeatEndInVoiceClone() 4");

              // pop currentRepeat from the voice's repeat descrs stack
              popRepeatFromRepeatDescrsStack (
                inputLineNumber,
                currentRepeat,
                "handleRepeatEndInVoiceClone() 5");
            }
        } // switch


#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceRepeats ()) {
          displayVoiceRepeatsStackSummary (
            inputLineNumber,
            "handleRepeatEndInVoiceClone() 6");
        }
#endif
      }
      break;
  } // switch

  --gIndenter;
}

void msrVoice::appendMeasuresRepeatReplicaToVoice (
  int inputLineNumber)
{
  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:
      {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
          gLogStream <<
            "Appending a measures repeat replica to voice \"" <<
            getVoiceName () <<  "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        // fetch last measure's full measure whole notes
        /* JMI
        int fullMeasureWholeNotesDuration =
          fVoiceLastSegment->
            getSegmentMeasuresList ().back ()->
              getFullMeasureWholeNotesDuration ();
              */

#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) { // JMI
          gLogStream <<
            endl <<
            "***********" <<
            endl << endl;
          print (gLogStream);
          gLogStream <<
            "***********" <<
            endl << endl;
        }
#endif

        // sanity check
        msgAssert (
          __FILE__, __LINE__,
          fVoicePendingMeasuresRepeat != nullptr,
          "fVoicePendingMeasuresRepeat is null");

        // create a measures repeat replicas
        S_msrMeasuresRepeatReplicas
          measuresRepeatReplicas =
            msrMeasuresRepeatReplicas::create (
              inputLineNumber,
              fVoicePendingMeasuresRepeat);

        // set the voice last segment as the measures repeat replicas segment
        measuresRepeatReplicas->
          setMeasuresRepeatReplicasSegment (
            fVoiceLastSegment);

        // forget about this voice last segment
        fVoiceLastSegment = nullptr;

        // set the measures repeat replicas in the voice current measures repeat
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) { // JMI
          gLogStream <<
            "Setting the measures repeat replica to current measures repeat BBB in voice \"" <<
            fVoiceName <<
            "\"" <<
            endl;
        }
#endif

        fVoicePendingMeasuresRepeat->
          setMeasuresRepeatReplicas (
            measuresRepeatReplicas);
      }
      break;
  } // switch
}

void msrVoice::appendRestMeasuresToVoiceElementsList (
  S_msrRestMeasures restMeasures)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    gLogStream <<
      "Appending rest measures '" <<
      restMeasures->asString () <<
      "' to voice \"" <<
      fVoiceName <<
      "\"," <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    restMeasures != nullptr,
    "restMeasures is null");

  fInitialVoiceElementsList.push_back (restMeasures);
}

void msrVoice::appendMeasuresRepeatToVoiceElementsList (
  S_msrMeasuresRepeat measuresRepeat)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Appending measures repeat '" <<
      measuresRepeat->asString () <<
      "' to voice \"" <<
      fVoiceName <<
      "\"," <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    measuresRepeat != nullptr,
    "measuresRepeat is null");

  fInitialVoiceElementsList.push_back (measuresRepeat);
}

void msrVoice:: appendRepeatEndingCloneToVoice ( // JMI
  S_msrRepeatEnding repeatEndingClone)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    repeatEndingClone->getInputLineNumber (); // JMI
#endif

  ++gIndenter;

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:
      {
        // add the repeat ending it to the voice current repeat
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceRepeats ()) {
          gLogStream <<
            "Appending a " <<
            msrRepeatEndingKindAsString (
              repeatEndingClone->getRepeatEndingKind ()) <<
            " repeat ending clone to current repeat in voice \"" <<
            fVoiceName <<
            "\"" <<
            endl;

          displayVoiceRepeatsStackSummary (
            inputLineNumber,
            "appendRepeatEndingCloneToVoice() 1");
        }
#endif

        // sanity check
        if (! fVoicePendingRepeatDescrsStack.size ()) {
          stringstream s;

          s <<
            "repeats stack is empty when attempting to append a " <<
            msrRepeatEndingKindAsString (
              repeatEndingClone->getRepeatEndingKind ()) <<
            " repeat ending to voice '" <<
            asShortString () <<
            "' ";

          msrInternalError (
            gGlobalOahOahGroup->getInputSourceName (),
            fInputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

        S_msrRepeat
          currentRepeat =
            fVoicePendingRepeatDescrsStack.front ()->
              getRepeatDescrRepeat ();

        currentRepeat->
          addRepeatEndingToRepeat (
            repeatEndingClone->getInputLineNumber (),
            repeatEndingClone);

#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceRepeats ()) {
          displayVoiceRepeatsStackSummary (
            inputLineNumber,
            "appendRepeatEndingCloneToVoice() 2");
        }
#endif
      }
      break;
  } // switch

  --gIndenter;
}

void msrVoice::prependBarlineToVoice (S_msrBarline barline)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceBarLines ()) {
    gLogStream <<
      "Prepending barline '" <<
      barline->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      ":" <<
      endl;
  }
#endif

  ++gIndenter;

  fVoiceLastSegment->
    prependBarlineToSegment (barline);

  --gIndenter;
}

void msrVoice::appendBarlineToVoice (S_msrBarline barline)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceBarLines ()) {
    gLogStream <<
      "Appending barline " <<
      barline->asString () <<
      " to voice \"" << getVoiceName () << "\"" <<
      ":" <<
      endl;
  }
#endif

  ++gIndenter;

  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
    createNewLastSegmentForVoice (
      barline->getInputLineNumber (),
      "appendBarlineToVoice()");
  }

  fVoiceLastSegment->
    appendBarlineToSegment (barline);

  --gIndenter;
}

void msrVoice::appendSegnoToVoice (S_msrSegno segno)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceSegnos ()) {
    gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceCodas ()) {
    gLogStream <<
      "Appending a coda to voice \"" << getVoiceName () << "\"" <<
      ":" <<
      endl;
  }
#endif

  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
    createNewLastSegmentForVoice (
      coda->getInputLineNumber (),
      "appendCodaToVoice()");
  }

  fVoiceLastSegment->
    appendCodaToSegment (coda);
}

void msrVoice::appendEyeGlassesToVoice (S_msrEyeGlasses eyeGlasses)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceEyeGlasses ()) {
    gLogStream <<
      "Appending a eyeGlasses to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
    createNewLastSegmentForVoice (
      eyeGlasses->getInputLineNumber (),
      "appendEyeGlassesToVoice()");
  }

  fVoiceLastSegment->
    appendEyeGlassesToSegment (eyeGlasses);
}

void msrVoice::appendPedalToVoice (S_msrPedal pedal)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePedals ()) {
    gLogStream <<
      "Appending a pedal to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
    createNewLastSegmentForVoice (
      pedal->getInputLineNumber (),
      "appendPedalToVoice()");
  }

  fVoiceLastSegment->
    appendPedalToSegment (pedal);
}

void msrVoice::appendDampToVoice (S_msrDamp damp)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceDamps ()) {
    gLogStream <<
      "Appending a damp to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
    createNewLastSegmentForVoice (
      damp->getInputLineNumber (),
      "appendDampToVoice()");
  }

  fVoiceLastSegment->
    appendDampToSegment (damp);
}

void msrVoice::appendDampAllToVoice (S_msrDampAll dampAll)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceDampAlls ()) {
    gLogStream <<
      "Appending a damp all to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
    createNewLastSegmentForVoice (
      dampAll->getInputLineNumber (),
      "appendDampAllToVoice()");
  }

  fVoiceLastSegment->
    appendDampAllToSegment (dampAll);
}

/*
S_msrElement msrVoice::removeLastElementFromVoice (  // JMI
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceChords ()) {
    gLogStream <<
      "Removing note '" <<
      note->asShortString () <<
      "' from voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  // remove note from voice last segment
  fVoiceLastSegment->
    removeNoteFromSegment (
      inputLineNumber,
      note);

  // update the part current position in measure
  fVoiceStaffUpLink->
    getStaffPartUpLink ()->
      decrementPartCurrentPositionInMeasure (
        inputLineNumber,
        note->
          getNoteSoundingWholeNotes ());

  --gIndenter;
}

void msrVoice::removeElementFromVoice (
  int          inputLineNumber,
  S_msrElement element)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Removing element '" <<
      element->asShortString () <<
      "' from voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  fVoiceLastSegment->
    removeElementFromSegment (
      inputLineNumber,
      element);

  --gIndenter;
}

S_msrMeasure msrVoice::removeLastMeasureFromVoice (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Removing last measure from voice \"" <<
      getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  // remove (new) last measure
  S_msrMeasure result =
    fVoiceLastSegment->
      removeLastMeasureFromSegment (
        inputLineNumber,
        "removeLastMeasureFromVoice()");

  --gIndenter;

  // return it
  return result;
}

void msrVoice::finalizeLastAppendedMeasureInVoice (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Finalizing last appended measure in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (gGlobalTraceOahGroup->getTraceMeasuresDetails ()) {
    displayVoice (
      inputLineNumber,
      "finalizeLastAppendedMeasureInVoice() 1");
  }
#endif

  ++gIndenter;

  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    fVoiceLastAppendedMeasure != nullptr,
    "fVoiceLastAppendedMeasure is null");

  // finalize fVoiceLastAppendedMeasure
  msrMeasure::msrMeasuresRepeatContextKind
    measuresRepeatContextKind =
      msrMeasure::kMeasuresRepeatContextKindNone;

  fVoiceLastAppendedMeasure->
    finalizeMeasure (
      inputLineNumber,
      measuresRepeatContextKind,
      "finalizeLastAppendedMeasureInVoice() 2");

/* JMI NO
  // forget about fVoiceLastAppendedMeasure
  fVoiceLastAppendedMeasure = nullptr;
*/

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceRegular:
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

/* JMI
      // handle the harmony voice if any
      if (fRegularVoiceHarmonyVoiceForwardLink) {
        fRegularVoiceHarmonyVoiceForwardLink->
          finalizeLastAppendedMeasureInVoice (
            inputLineNumber);
      }

      // handle the figuredBass voice if any
      if (fRegularVoiceFiguredBassVoiceForwardLink) {
        fRegularVoiceFiguredBassVoiceForwardLink->
          finalizeLastAppendedMeasureInVoice (
            inputLineNumber);
      }
            */
      break;

    case msrVoiceKind::kVoiceHarmony:
    case msrVoiceKind::kVoiceFiguredBass:
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresDetails ()) {
    displayVoice (
      inputLineNumber,
      "finalizeLastAppendedMeasureInVoice() 3");
  }
#endif

  --gIndenter;
}

void msrVoice:: collectVoiceMeasuresIntoFlatList (
  int inputLineNumber)
{
  // collect measures from the initial elements if any
  if (fInitialVoiceElementsList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceVoices ()) {
      gLogStream <<
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
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceVoices ()) {
      gLogStream <<
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

void msrVoice::finalizeVoice (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Finalizing voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }

  if (gGlobalTraceOahGroup->getTraceVoicesDetails ()) {
    displayVoice (
      inputLineNumber,
      "finalizeVoice() 1");
  }
#endif

  if (fVoiceHasBeenFinalized) {
    stringstream s;

    s <<
      "Attempting to finalize  voice \"" <<
      asShortString () <<
      "\" more than once";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // set part shortest note duration if relevant
  S_msrPart
    voicePart =
      fetchVoicePartUpLink ();

  rational
    partShortestNoteDuration =
      voicePart->
        getPartShortestNoteDuration ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "--> in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", fVoiceShortestNoteDuration: " << fVoiceShortestNoteDuration <<
      ", partShortestNoteDuration: " << partShortestNoteDuration <<
      endl;
  }
#endif

  if (fVoiceShortestNoteDuration < partShortestNoteDuration) {
    // set the voice part shortest note duration
    voicePart->
      setPartShortestNoteDuration (
        fVoiceShortestNoteDuration);

    // set the voice part shortest note tuplet factor // JMI
    voicePart->
      setPartShortestNoteTupletFactor (
        fVoiceShortestNoteTupletFactor);
  }

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
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      s.str ());
  }

  // are there pending repeats in the voice repeats stack???
  unsigned int voicePendingRepeatDescrsStackSize =
    fVoicePendingRepeatDescrsStack.size ();

  if (voicePendingRepeatDescrsStackSize) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceMeasures ()) {
        displayVoiceRepeatsStackSummary (
          inputLineNumber,
          "finalizeVoice() 2");
      }
#endif

    stringstream s;

    s <<
      singularOrPluralWithoutNumber (
        voicePendingRepeatDescrsStackSize, "There is", "There are") <<
      " " <<
      voicePendingRepeatDescrsStackSize <<
      " " <<
      singularOrPluralWithoutNumber (
        voicePendingRepeatDescrsStackSize, "repeat", "repeats") <<
      " pending in the voice repeats stack in voice \"" <<
      asShortString () <<
      "\" ";

    msrMusicXMLWarning (
      gGlobalOahOahGroup->getInputSourceName (),
      fInputLineNumber,
      s.str ());
  }

  // collect the voice measures into the flat list
  collectVoiceMeasuresIntoFlatList (
    inputLineNumber);

  fVoiceHasBeenFinalized = true;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoicesDetails ()) {
    displayVoiceRepeatsStackRestMeasuresMeasuresRepeatAndVoice (
      inputLineNumber,
      "finalizeVoice() 3");
  }
#endif

/* JMI
  // finalize the harmony voice if any
  if (fRegularVoiceHarmonyVoiceForwardLink) {
    fRegularVoiceHarmonyVoiceForwardLink->finalizeVoice (
      inputLineNumber);
  }

  // finalize the figured bass voice if any
  if (fRegularVoiceFiguredBassVoiceForwardLink) {
    fRegularVoiceFiguredBassVoiceForwardLink->finalizeVoice (
      inputLineNumber);
  }
  */
}

void msrVoice::finalizeVoiceAndAllItsMeasures (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Finalizing voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }

  if (gGlobalTraceOahGroup->getTraceVoicesDetails ()) {
    displayVoice (
      inputLineNumber,
      "finalizeVoice() 1");
  }
#endif

  if (fVoiceHasBeenFinalized) {
    stringstream s;

    s <<
      "Attempting to finalize  voice \"" <<
      asShortString () <<
      "\" more than once";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // set part shortest note duration if relevant
  S_msrPart
    voicePart =
      fetchVoicePartUpLink ();

  rational
    partShortestNoteDuration =
      voicePart->
        getPartShortestNoteDuration ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "--> in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", fVoiceShortestNoteDuration: " << fVoiceShortestNoteDuration <<
      ", partShortestNoteDuration: " << partShortestNoteDuration <<
      endl;
  }
#endif

  if (fVoiceShortestNoteDuration < partShortestNoteDuration) {
    // set the voice part shortest note duration
    voicePart->
      setPartShortestNoteDuration (
        fVoiceShortestNoteDuration);

    // set the voice part shortest note tuplet factor // JMI
    voicePart->
      setPartShortestNoteTupletFactor (
        fVoiceShortestNoteTupletFactor);
  }

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
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      s.str ());
  }

  // are there pending repeats in the voice repeats stack???
  unsigned int voicePendingRepeatDescrsStackSize =
    fVoicePendingRepeatDescrsStack.size ();

  if (voicePendingRepeatDescrsStackSize) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceMeasures ()) {
        displayVoiceRepeatsStackSummary (
          inputLineNumber,
          "finalizeVoice() 2");
      }
#endif

    stringstream s;

    s <<
      singularOrPluralWithoutNumber (
        voicePendingRepeatDescrsStackSize, "There is", "There are") <<
      " " <<
      voicePendingRepeatDescrsStackSize <<
      " " <<
      singularOrPluralWithoutNumber (
        voicePendingRepeatDescrsStackSize, "repeat", "repeats") <<
      " pending in the voice repeats stack in voice \"" <<
      asShortString () <<
      "\" ";

    msrMusicXMLWarning (
      gGlobalOahOahGroup->getInputSourceName (),
      fInputLineNumber,
      s.str ());
  }

  // collect the voice measures into the flat list
  collectVoiceMeasuresIntoFlatList (
    inputLineNumber);

  fVoiceHasBeenFinalized = true;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoicesDetails ()) {
    displayVoiceRepeatsStackRestMeasuresMeasuresRepeatAndVoice (
      inputLineNumber,
      "finalizeVoice() 3");
  }
#endif

/* JMI
  // finalize the harmony voice if any
  if (fRegularVoiceHarmonyVoiceForwardLink) {
    fRegularVoiceHarmonyVoiceForwardLink->finalizeVoice (
      inputLineNumber);
  }

  // finalize the figured bass voice if any
  if (fRegularVoiceFiguredBassVoiceForwardLink) {
    fRegularVoiceFiguredBassVoiceForwardLink->finalizeVoice (
      inputLineNumber);
  }
  */

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Finalizing all the measures of voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  for (
    list<S_msrMeasure>::const_iterator i = fVoiceMeasuresFlatList.begin ();
    i != fVoiceMeasuresFlatList.end ();
    ++i
  ) {
    S_msrMeasure measure = (*i);

    measure->
      finalizeMeasure_BIS (
        inputLineNumber,
        msrMeasure::kMeasuresRepeatContextKindNone,
        "finalizeVoiceAndAllItsMeasures()");
  } // for
}

void msrVoice::checkBeamNumber (S_msrBeam beam, S_msrNote note)
{
  int inputLineNumber =
    beam->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceBeams ()) {
    gLogStream <<
      "Checking the order of the beams begins, continues and ends" <<
      " upon beam '" <<
      beam->asShortString () <<
      "' and note '" <<
      note->asString () <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  msrBeamKind beamKind   = beam->getBeamKind ();
  int         beamNumber = beam->getBeamNumber ();

  unsigned int
    noteBeamNumbersStackSize =
      fVoiceBeamNumbersStack.size ();

  switch (beamKind) {
    case msrBeamKind::k_NoBeam:
      {
        stringstream s;

        s <<
          "appending unknown beam " <<
          beam->asShortString () <<
          " to note " <<
          note->asShortString ();

        msrError (
          gGlobalOahOahGroup->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;

    case msrBeamKind::kBeamBegin:
      if (noteBeamNumbersStackSize) {
        // the stack is not empty
        int voiceBeamNumbersStackTop =
          fVoiceBeamNumbersStack.top ();

        if (beamNumber != voiceBeamNumbersStackTop + 1) {
          stringstream s;

          s <<
            "appending beam begin " <<
            beam->asShortString () <<
            " to note " <<
            note->asShortString () <<
            "whilst the current beam began with number '" <<
            voiceBeamNumbersStackTop <<
            "'";

          msrError (
            gGlobalOahOahGroup->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
      }

      fVoiceBeamNumbersStack.push (beamNumber);
      break;

    case msrBeamKind::kBeamContinue:
      if (! noteBeamNumbersStackSize) {
        // the stack is empty
        stringstream s;

        s <<
          "appending beam continue " <<
          beam->asShortString () <<
          " to note " <<
          note->asShortString () <<
          "whilst there is no preceding beam begin with number '" <<
          beamNumber <<
          "'";

        msrError (
          gGlobalOahOahGroup->getInputSourceName (),
          beam->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }
      else {
        // the stack is not empty
        int voiceBeamNumbersStackTop =
          fVoiceBeamNumbersStack.top ();

        if (beamNumber != voiceBeamNumbersStackTop) {
          stringstream s;

          s <<
            "appending beam continue " <<
            beam->asShortString () <<
            " to note " <<
            note->asShortString () <<
            "whilst the current beam began with number '" <<
            voiceBeamNumbersStackTop <<
            "'";

          msrError (
            gGlobalOahOahGroup->getInputSourceName (),
            beam->getInputLineNumber (),
            __FILE__, __LINE__,
            s.str ());
        }
      }
      break;

    case msrBeamKind::kBeamEnd:
      if (noteBeamNumbersStackSize) {
        // the stack is not empty
        int voiceBeamNumbersStackTop =
          fVoiceBeamNumbersStack.top ();

        if (beamNumber != voiceBeamNumbersStackTop) {
          stringstream s;

          s <<
            "appending beam end " <<
            beam->asShortString () <<
            " to note " <<
            note->asShortString () <<
            "whilst the current beam began with number '" <<
            voiceBeamNumbersStackTop <<
            "'";

          msrError (
            gGlobalOahOahGroup->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
      }

      fVoiceBeamNumbersStack.pop ();
      break;

    case msrBeamKind::kBeamForwardHook:
      // JMI ???
      break;
    case msrBeamKind::kBeamBackwardHook:
      // JMI ???
      break;
  } // switch
}

void msrVoice::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrVoice::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrVoice::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrVoice::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrVoice::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrVoice::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrVoice::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrVoice::browseData ()" <<
      endl;
  }

  // browse the voice initial elements
  if (fInitialVoiceElementsList.size ()) {
    for (
      list<S_msrVoiceElement>::const_iterator i = fInitialVoiceElementsList.begin ();
      i != fInitialVoiceElementsList.end ();
      ++i
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
      ++i
    ) {
      S_msrStanza stanza = (*i).second;

      if (stanza->getStanzaTextPresent ()) {
        // browse the stanza
        msrBrowser<msrStanza> browser (v);
        browser.browse (*(stanza));
      }
    } // for
  }

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% <== msrVoice::browseData ()" <<
      endl;
  }
}

string msrVoice::voiceNumberAsString () const
{
  string result = to_string (fVoiceNumber);

  switch (fVoiceNumber) {
    case K_PART_HARMONY_VOICE_NUMBER:
      result += " (K_PART_HARMONY_VOICE_NUMBER)";
      break;
    case K_PART_FIGURED_BASS_VOICE_NUMBER:
      result += " (K_PART_FIGURED_BASS_VOICE_NUMBER)";
      break;
    default:
      ;
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

string msrVoice::asShortString () const
{
  stringstream s;

  s <<
    "Voice \"" << getVoiceName () << "\", " <<
    msrVoiceKindAsString (fVoiceKind) <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string msrVoice::asString () const
{
  stringstream s;

  s <<
    "Voice \"" << getVoiceName () << "\", " <<
    msrVoiceKindAsString (fVoiceKind) <<
     ", " <<
    singularOrPlural (
      fVoiceActualNotesCounter, "actual note", "actual notes") <<
     ", " <<
    singularOrPlural (
      fVoiceStanzasMap.size (), "stanza", "stanzas") <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrVoice::displayVoice (
  int    inputLineNumber,
  string context) const
{
  gLogStream <<
    endl <<
    "*********>> Displaying voice \"" <<
    getVoiceName () <<
    "\" (" << context << ")" <<
    ", line " << inputLineNumber <<
    " contains:" <<
    endl;

  ++gIndenter;
  print (gLogStream);
  --gIndenter;

  gLogStream <<
    " <<*********" <<
    endl << endl;
}

void msrVoice::displayVoiceRepeatsStackRestMeasuresMeasuresRepeatAndVoice (
  int    inputLineNumber,
  string context)
{
  displayVoice (
    inputLineNumber,
    context);

  displayVoiceRepeatsStack (
    inputLineNumber,
    context);

  displayVoiceRestMeasures (
    inputLineNumber,
    context);

  displayVoiceMeasuresRepeat (
    inputLineNumber,
    context);
}

void msrVoice::print (ostream& os) const
{
  os <<
    "Voice number '" <<
    fVoiceNumber <<
    "', \"" << getVoiceName () <<
    "\", " << msrVoiceKindAsString (fVoiceKind) <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  os << left <<
    "(" <<
    singularOrPlural (
      fVoiceActualHarmoniesCounter, "harmony", "harmonies") <<
     ", " <<
    singularOrPlural (
      fVoiceActualFiguredBassesCounter, "figured bass", "figured basses") <<
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

  const unsigned int fieldWidth = 41;

  os << left <<
    setw (fieldWidth) <<
    "staffUpLink" << " : " <<
    fVoiceStaffUpLink->getStaffName () <<
    endl <<

    setw (fieldWidth) <<
    "voiceCurrentMeasureNumber" << " : \"" <<
    fVoiceCurrentMeasureNumber <<
    "\"" <<
    endl <<

    setw (fieldWidth) <<
    "voiceCurrentMeasureOrdinalNumber" << " : " <<
    fVoiceCurrentMeasureOrdinalNumber <<
    endl <<

    setw (fieldWidth) <<
    "voiceCurrentMeasurePuristNumber" << " : " <<
    fVoiceCurrentMeasurePuristNumber <<
    endl <<

    setw (fieldWidth) <<
    "regularVoiceStaffSequentialNumber" << " : " <<
    regularVoiceStaffSequentialNumberAsString () <<
    endl;

#ifdef TRACING_IS_ENABLED
  // regular measure ends detection
  os << left <<
    setw (fieldWidth) <<
    "wholeNotesSinceLastRegularMeasureEnd" << " : " <<
    fWholeNotesSinceLastRegularMeasureEnd <<
    endl;

  // incomplete measures after repeats detection
  os << left <<
    setw (fieldWidth) <<
    "currentVoiceRepeatPhaseKind" << " : " <<
    voiceRepeatPhaseKindAsString (
      fCurrentVoiceRepeatPhaseKind) <<
    endl;

  // print the voice first clef, and the current clef, key and time
  os << left <<
    setw (fieldWidth) << "voiceFirstClef" << " : ";
  if (fVoiceFirstClef) {
    os <<
      fVoiceFirstClef;
  }
  else {
    os << "null" << endl;
  }
  os << left <<
    setw (fieldWidth) << "voiceCurrentClef" << " : ";
  if (fVoiceCurrentClef) {
    os <<
      fVoiceCurrentClef;
  }
  else {
    os << "null" << endl;
  }

  os << left <<
    setw (fieldWidth) << "voiceCurrentKey" << " : ";
  if (fVoiceCurrentKey) {
    os <<
      fVoiceCurrentKey;
  }
  else {
    os << "null" << endl;
  }

  os << left <<
    setw (fieldWidth) << "voiceCurrentTime" << " : ";
  if (fVoiceCurrentTime) {
    ++gIndenter;
    os <<
      endl <<
      fVoiceCurrentTime;
    --gIndenter;
  }
  else {
    os << "null" << endl;
  }
#endif

/* JMI
  // print the harmony voice name if any
  os << left <<
    setw (fieldWidth) << "regularVoiceHarmonyVoice" << " : ";
  if (fRegularVoiceHarmonyVoiceForwardLink) {
    os <<
      fRegularVoiceHarmonyVoiceForwardLink->getVoiceName ();
  }
  else {
    os <<
      "none";
  }
  os << endl;

  // print the figured bass voice name if any
  os << left <<
    setw (fieldWidth) << "regularVoiceFiguredBassVoice" << " : ";
  if (fRegularVoiceFiguredBassVoiceForwardLink) {
    os <<
      fRegularVoiceFiguredBassVoiceForwardLink->getVoiceName ();
  }
  else {
    os <<
      "none";
  }
  os << endl;
*/

  os << left <<
    setw (fieldWidth) << "voiceShortestNoteDuration" << " : " <<
    fVoiceShortestNoteDuration <<
    endl <<
    setw (fieldWidth) << "voiceShortestNoteTupletFactor" << " : " <<
    endl;

  ++gIndenter;
  os <<
    fVoiceShortestNoteTupletFactor;
  --gIndenter;

  os << left <<
    setw (fieldWidth) << "voiceHasBeenFinalized" << " : " <<
    booleanAsString (fVoiceHasBeenFinalized) <<
    endl;

  os << left <<
    setw (fieldWidth) << "currentPositionInVoice" << " : " <<
    fCurrentPositionInVoice <<
    endl <<
    setw (fieldWidth) << "currentMomentInVoice" << " : " <<
    fCurrentMomentInVoice <<
    endl;

  os << left <<
    setw (fieldWidth) << "musicHasBeenInsertedInVoice" << " : " <<
    booleanAsString (fMusicHasBeenInsertedInVoice) <<
    endl <<
    setw (fieldWidth) << "voiceContainsRestMeasures" << " : " <<
    booleanAsString (fVoiceContainsRestMeasures) <<
    endl <<
    setw (fieldWidth) << "voiceContainsMeasuresRepeats" << " : " <<
    booleanAsString (fVoiceContainsMeasuresRepeats) <<
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
  os << endl;

  // print the voice last appended measure if any
  os <<
    setw (fieldWidth) << "voiceLastAppendedMeasure" << " : ";
  if (fVoiceLastAppendedMeasure) {
    os <<
      "'" <<
      fVoiceLastAppendedMeasure->asShortString () <<
      "'";
    }
  else {
    os <<
      "none";
  }
  os << endl;

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
  os << endl;

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
        voiceFirstNonGraceNote->asShortString ();
    }
    else {
      os <<
        "none";
    }
    os << endl;
  }

  // print the voice last appended note
  os <<
    setw (fieldWidth) <<
    "voiceLastAppendedNote" << " : ";
  if (fVoiceLastAppendedNote) {
    os << gTab <<
      fVoiceLastAppendedNote->asShortString ();
  }
  else {
    os <<
      "none";
  }
  os << endl;

#ifdef TRACING_IS_ENABLED
  // print the voice measures flat list
  unsigned int voiceMeasuresFlatListSize =
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
  os << endl;

  if (voiceMeasuresFlatListSize) {
    ++gIndenter;

    list<S_msrMeasure>::const_iterator
      iBegin = fVoiceMeasuresFlatList.begin (),
      iEnd   = fVoiceMeasuresFlatList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the measure
      if (gGlobalTraceOahGroup->getTraceMeasures ()) {
        os << (*i)->asShortString ();
      }
      else {
        os << (*i)->getMeasureElementMeasureNumber ();
      }
      if (++i == iEnd) break;
      os << ' ';
    } // for
    os << endl;

    --gIndenter;
  }
#endif

  // print the voice initial elements
  unsigned int voiceInitialElementsListSize =
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

  if (voiceInitialElementsListSize) {
    os << endl;

    ++gIndenter;

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

    --gIndenter;
  }
  os << endl;

  /* JMI
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
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

    ++gIndenter;
    os <<
      fVoiceLastSegment <<
      endl;
    --gIndenter;
  }
  else {
    os <<
      "*** voiceLastSegment is null ***" << // JMI
      endl;
  }
  os << endl;

  // print the stanzas if any
  if (fVoiceStanzasMap.size ()) {
    os <<
      "Stanzas:" <<
      endl;

    ++gIndenter;

    map<string, S_msrStanza>::const_iterator
      iBegin = fVoiceStanzasMap.begin (),
      iEnd   = fVoiceStanzasMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i).second;
      if (++i == iEnd) break;
      // no endl here
    } // for

    --gIndenter;
  }

  --gIndenter;
}

void msrVoice::printShort (ostream& os) const
{
  os <<
    "Voice number '" <<
    fVoiceNumber <<
    "', \"" << getVoiceName () <<
    "\", " << msrVoiceKindAsString (fVoiceKind) <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  os << left <<
    "(" <<
    singularOrPlural (
      fVoiceActualHarmoniesCounter, "harmony", "harmonies") <<
     ", " <<
    singularOrPlural (
      fVoiceActualFiguredBassesCounter, "figured bass", "figured basses") <<
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

  const unsigned int fieldWidth = 41;

/*
  os << left <<
    setw (fieldWidth) <<
    "staffUpLink" << " : " <<
    fVoiceStaffUpLink->getStaffName () <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "voiceNumber" << " : " <<
    voiceNumberAsString () <<
    endl <<

    setw (fieldWidth) <<
    "voiceCurrentMeasureNumber" << " : \"" <<
    fVoiceCurrentMeasureNumber <<
    "\"" <<
    endl <<

    setw (fieldWidth) <<
    "voiceCurrentMeasureOrdinalNumber" << " : " <<
    fVoiceCurrentMeasureOrdinalNumber <<
    endl <<

    setw (fieldWidth) <<
    "voiceCurrentMeasurePuristNumber" << " : " <<
    fVoiceCurrentMeasurePuristNumber <<
    endl <<

  os << left <<
    setw (fieldWidth) <<
    "regularVoiceStaffSequentialNumber" << " : " <<
    regularVoiceStaffSequentialNumberAsString () <<
    endl;

#ifdef TRACING_IS_ENABLED
  // regular measure ends detection
  os << left <<
    setw (fieldWidth) <<
    "wholeNotesSinceLastRegularMeasureEnd" << " : " <<
    fWholeNotesSinceLastRegularMeasureEnd <<
    endl;

  // incomplete measures after repeats detection
  os << left <<
    setw (fieldWidth) <<
    "currentVoiceRepeatPhaseKind" << " : " <<
    voiceRepeatPhaseKindAsString (
      fCurrentVoiceRepeatPhaseKind) <<
    endl;

  // print the voice first clef, and the current clef, key and time
  os << left <<
    setw (fieldWidth) << "voiceFirstClef" << " : ";
  if (fVoiceFirstClef) {
    os <<
      fVoiceFirstClef;
  }
  else {
    os << "null" << endl;
  }
  os << left <<
    setw (fieldWidth) << "voiceCurrentClef" << " : ";
  if (fVoiceCurrentClef) {
    os <<
      fVoiceCurrentClef;
  }
  else {
    os << "null" << endl;
  }

  os << left <<
    setw (fieldWidth) << "voiceCurrentKey" << " : ";
  if (fVoiceCurrentKey) {
    os <<
      fVoiceCurrentKey;
  }
  else {
    os << "null" << endl;
  }

  os << left <<
    setw (fieldWidth) << "voiceCurrentTime" << " : ";
  if (fVoiceCurrentTime) {
    os <<
      endl <<
      fVoiceCurrentTime;
  }
  else {
    os << "null" << endl;
  }
#endif

  // print the harmony voice name if any
  os << left <<
    setw (fieldWidth) << "regularVoiceHarmonyVoice" << " : ";
  if (fRegularVoiceHarmonyVoiceForwardLink) {
    os <<
      fRegularVoiceHarmonyVoiceForwardLink->getVoiceName ();
  }
  else {
    os <<
      "none";
  }
  os << endl;

  // print the figured bass voice name if any
  os << left <<
    setw (fieldWidth) << "regularVoiceFiguredBassVoice" << " : ";
  if (fRegularVoiceFiguredBassVoiceForwardLink) {
    os <<
      fRegularVoiceFiguredBassVoiceForwardLink->getVoiceName ();
  }
  else {
    os <<
      "none";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) << "voiceShortestNoteDuration" << " : " <<
    fVoiceShortestNoteDuration <<
    endl <<
    setw (fieldWidth) << "voiceShortestNoteTupletFactor" << " : " <<
    endl;

  ++gIndenter;
  os <<
    fVoiceShortestNoteTupletFactor;
  --gIndenter;

  os << left <<
    setw (fieldWidth) << "voiceHasBeenFinalized" << " : " <<
    booleanAsString (fVoiceHasBeenFinalized) <<
    endl;

  os << left <<
    setw (fieldWidth) << "currentPositionInVoice" << " : " <<
    fCurrentPositionInVoice <<
    endl <<
    setw (fieldWidth) << "currentMomentInVoice" << " : " <<
    fCurrentMomentInVoice <<
    endl;

  os << left <<
    setw (fieldWidth) << "musicHasBeenInsertedInVoice" << " : " <<
    booleanAsString (fMusicHasBeenInsertedInVoice) <<
    endl <<
    setw (fieldWidth) << "voiceContainsRestMeasures" << " : " <<
    booleanAsString (fVoiceContainsRestMeasures) <<
    endl <<
    setw (fieldWidth) << "voiceContainsMeasuresRepeats" << " : " <<
    booleanAsString (fVoiceContainsMeasuresRepeats) <<
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
  os << endl;

  // print the voice last appended measure if any
  os <<
    setw (fieldWidth) << "voiceLastAppendedMeasure" << " : ";
  if (fVoiceLastAppendedMeasure) {
    os <<
      "'" <<
      fVoiceLastAppendedMeasure->asShortString () <<
      "'";
    }
  else {
    os <<
      "none";
  }
  os << endl;

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
  os << endl;

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
      os << gTab <<
        voiceFirstNonGraceNote->asShortString ();
    }
    else {
      os <<
        "none";
    }
    os << endl;
  }

  // print the voice last appended note
  os <<
    setw (fieldWidth) <<
    "voiceLastAppendedNote" << " : ";
  if (fVoiceLastAppendedNote) {
    os << gTab <<
      fVoiceLastAppendedNote->asShortString ();
  }
  else {
    os <<
      "none";
  }
  os << endl;
*/

#ifdef TRACING_IS_ENABLED
  // print the voice measures flat list
  unsigned int voiceMeasuresFlatListSize =
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
  os << endl;

  if (voiceMeasuresFlatListSize) {
    ++gIndenter;

    list<S_msrMeasure>::const_iterator
      iBegin = fVoiceMeasuresFlatList.begin (),
      iEnd   = fVoiceMeasuresFlatList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the measure
      if (gGlobalTraceOahGroup->getTraceMeasures ()) {
        os << (*i)->asShortString ();
      }
      else {
        os << (*i)->getMeasureElementMeasureNumber ();
      }
      if (++i == iEnd) break;
      os << ' ';
    } // for
    os << endl;

    --gIndenter;
  }
#endif

  // print the voice initial elements
  unsigned int voiceInitialElementsListSize =
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

  if (voiceInitialElementsListSize) {
    os << endl;

    ++gIndenter;

    list<S_msrVoiceElement>::const_iterator
      iBegin = fInitialVoiceElementsList.begin (),
      iEnd   = fInitialVoiceElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the element
      (*i)->printShort (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }
  os << endl;

  /* JMI
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
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

    ++gIndenter;
    fVoiceLastSegment->printShort (os);
    os <<
      endl;
    --gIndenter;
  }
  else {
    os <<
      "*** voiceLastSegment is null ***" << // JMI
      endl;
  }
  os << endl;

  // print the stanzas if any
  if (fVoiceStanzasMap.size ()) {
    os <<
      "Stanzas:" <<
      endl;

    ++gIndenter;

    map<string, S_msrStanza>::const_iterator
      iBegin = fVoiceStanzasMap.begin (),
      iEnd   = fVoiceStanzasMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      (*i).second->printShort (os);
      if (++i == iEnd) break;
      // no endl here
    } // for

    --gIndenter;
  }

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msrVoice& elt)
{
  elt->print (os);
  return os;
}


}

        /* JMI
      switch (lastMeasureInLastSegment->getMeasureKind ()) {
        case msrMeasureKind::kMeasureKindMusicallyEmpty:
          {
            // the last measure is musically empty:
            // keep it as it is
          }
          break;

        default:
          {
            // the last measure it not musically empty:

            // append last segment to initial voice elements list
            appendVoiceLastSegmentToInitialVoiceElements ( // JMI ???
              inputLineNumber,
              "handleVoiceLevelRepeatStartInVoice() 9");

            // create a new last segment for the voice
#ifdef TRACING_IS_ENABLED
            if (gGlobalTraceOahGroup->getTraceSegments ()) {
              gLogStream <<
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
          break;
      } // switch
      */

/*
          // finalize lastMeasureInLastSegment
          lastMeasureInLastSegment->
            finalizeMeasure (
              inputLineNumber,
              msrMeasure::kMeasuresRepeatContextKindNone,
              "handleVoiceLevelRepeatStartInVoice() 6");

          // append last segment to initial voice elements list
          appendVoiceLastSegmentToInitialVoiceElements (
            inputLineNumber,
            "handleVoiceLevelRepeatStartInVoice() 7");

          // create a new last segment for the voice
#ifdef TRACING_IS_ENABLED
          if (gGlobalTraceOahGroup->getTraceSegments ()) {
            gLogStream <<
              "Creating a new last segment for voice \"" <<
              fVoiceName << "\"" <<
              ", line " << inputLineNumber <<
              endl;
          }
#endif

          createNewLastSegmentForVoice (
            inputLineNumber,
            "handleVoiceLevelRepeatStartInVoice() 8");

          // create a new measure with the same number as the voice last measure
          // and append it to the voice,
          createMeasureAndAppendItToVoice (
            inputLineNumber,
            lastMeasureInLastSegment->getMeasureElementMeasureNumber (),
            msrMeasureImplicitKind::kMeasureImplicitKindNo);
            */
