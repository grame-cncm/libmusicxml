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

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "musicXMLOah.h"
#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrRepeatDescr msrRepeatDescr::create (
  int         repeatDescrStartInputLineNumber,
  S_msrRepeat repeatDescrRepeat)
{
  msrRepeatDescr* o = new
    msrRepeatDescr (
      repeatDescrStartInputLineNumber,
      repeatDescrRepeat);
  assert(o!=0);
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
  const int fieldWidth = 29;

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
  assert(o!=0);

  return o;
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
  msrAssert(
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
      "Creating voice \"" << asString () << "\"" <<
      endl;
  }
#endif
}

msrVoice::~msrVoice ()
{}

S_msrPart msrVoice::fetchVoicePartUpLink () const
{
 return
  fVoiceStaffUpLink->
    getStaffPartUpLink ();
}

void msrVoice::setRegularVoiceStaffSequentialNumber (
  int regularVoiceStaffSequentialNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
      "Setting the name of '" <<
      voiceKindAsString () <<
      "' voice from number " << voiceNumber <<
      endl;
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kVoiceRegular:
      fVoiceName =
        fVoiceStaffUpLink->getStaffName () +
        "_Voice_" +
        int2EnglishWord (voiceNumber);
      break;

    case msrVoice::kVoiceHarmony:
      fVoiceName =
        fVoiceStaffUpLink->getStaffName () +
        "_Voice_" +
        int2EnglishWord (
          voiceNumber - K_VOICE_HARMONY_VOICE_BASE_NUMBER) +
          "_HARMONY";
      break;

    case msrVoice::kVoiceFiguredBass:
      fVoiceName =
        fVoiceStaffUpLink->getStaffName () +
        "_Voice_" +
        int2EnglishWord (
          voiceNumber - K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER) +
          "_FIGURED_BASS";
/* JMI
      {
        stringstream s;

        s <<
          "A '" <<
          getVoiceName () <<
          "' voice cannot get its name from its number";

        msrInternalError (
          gOahOah->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      */
      break;
  } // switch

#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceSegments) {
    gLogOstream <<
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
  msrAssert (
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceSegments) {
    gLogOstream <<
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
#ifdef TRACE_OAH
    if (gTraceOah->fTraceSegments) {
      gLogOstream <<
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
  // in registerVoiceInRegularVoicesMapByItsNumberByItsNumber ()
  fRegularVoiceStaffSequentialNumber = -1;

  gIndenter++;

  // compute voice number
  int voiceNumber =
    gMsrOah->
      fCreateVoicesStaffRelativeNumbers // JMI use it
        ? fRegularVoiceStaffSequentialNumber
        : fVoiceNumber;

  // set voice name
  switch (fVoiceKind) {
    case msrVoice::kVoiceRegular:
      setVoiceNameFromNumber (
        fInputLineNumber,
        voiceNumber);
      break;

    case msrVoice::kVoiceHarmony:
      setVoiceNameFromNumber (
        fInputLineNumber,
        voiceNumber);
      break;

    case msrVoice::kVoiceFiguredBass:
      fVoiceName =
        fVoiceStaffUpLink->getStaffName () +
        "_FIGURED_BASS_Voice";
      break;
  } // switch

#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
      "Initializing voice \"" << fVoiceName <<
      "\" in staff \"" <<
      fVoiceStaffUpLink->getStaffName () <<
      "\"" <<
      endl;
  }
#endif

  // check voice number
  switch (fVoiceKind) {
    case msrVoice::kVoiceRegular:
      // the voice number should be positive
      if (fVoiceNumber < 1 || fVoiceNumber > 4) {
        stringstream s;

        s <<
          "regular voice number " << fVoiceNumber <<
          " is not in the 1..4 range";

          /* JMI ???
        msrMusicXMLError (
          gOahOah->fInputSourceName,
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
          */
      }
      break;

    case msrVoice::kVoiceHarmony:
      break;

    case msrVoice::kVoiceFiguredBass:
    /* JMI
      if (fVoiceNumber != K_PART_FIGURED_BASS_VOICE_NUMBER) {
        stringstream s;

        s <<
          "figured bass voice number " << fVoiceNumber <<
          " is not equal to " << K_PART_FIGURED_BASS_VOICE_NUMBER;

        msrInternalError (
          gOahOah->fInputSourceName,
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

  // set voice number
  fVoiceCurrentMeasureNumber = // JMI "??";
    fetchVoicePartUpLink ()->
      getPartCurrentMeasureNumber ();

  // set voice current measure purist number
  fVoiceCurrentMeasurePuristNumber = 1; // default value,
                                        // supposing there's no anacrusis

  // music has not been inserted in voice yet
  fMusicHasBeenInsertedInVoice = false;

  // counters
  fVoiceActualNotesCounter     = 0;
  fVoiceRestsCounter           = 0;
  fVoiceSkipsCounter           = 0;
  fVoiceActualHarmoniesCounter = 0;

  // regular measure ends detection
  fWholeNotesSinceLastRegularMeasureEnd = rational (0, 1);

  // set voice current after repeat component phase kind
  setCurrentVoiceRepeatPhaseKind (
    fInputLineNumber,
    kVoiceRepeatPhaseNone);

  // rest measures
  fVoiceContainsRestMeasures  = false;
  fVoiceRemainingRestMeasures = 0;

  // measures repests
  fVoiceContainsMeasuresRepeats = false;

  // voice finalization
  fVoiceHasBeenFinalized = false;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
      "Initial contents of voice \"" << fVoiceName <<
      "\" in staff \"" <<
      fVoiceStaffUpLink->getStaffName () <<
      "\":" <<
      endl;

    gIndenter++;

    this->print (gLogOstream);

    gIndenter--;
  }
#endif

  gIndenter--;
}

void msrVoice::changeVoiceIdentity ( // after a deep copy
  int voiceNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
      "Changing identity of voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", number: " << voiceNumber <<
      endl;
  }
#endif

  // make it a regular voice
  setVoiceKind (
    msrVoice::kVoiceRegular);

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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
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

  return newbornClone;
}

S_msrVoice msrVoice::createVoiceDeepCopy (
  int          inputLineNumber,
  msrVoiceKind voiceKind,
  int          voiceNumber,
  S_msrStaff   containingStaff)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoicesDetails) {
    gLogOstream <<
      "****" <<
      " BEFORE voiceDeepCopy = " <<
      endl;

    print (gLogOstream);

    gLogOstream <<
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
#ifdef TRACE_OAH
    if (gTraceOah->fTraceVoices) {
      gLogOstream <<
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
          gOahOah->fInputSourceName,
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
#ifdef TRACE_OAH
    if (gTraceOah->fTraceVoices) {
      gLogOstream <<
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
#ifdef TRACE_OAH
    if (gTraceOah->fTraceVoices) {
      gLogOstream <<
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
    i++
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoicesDetails) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
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

  // is there a pending rest measures in this voice?
  if (fVoiceRestMeasuresWaitingForItsNextMeasureNumber) {
    // yes
#ifdef TRACE_OAH
    if (gTraceOah->fTraceRestMeasures) {
      gLogOstream <<
        "There is a rest measures waiting for its next measure number" <<
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
      // yes, set waiting rest measures's next measure number
#ifdef TRACE_OAH
      if (gTraceOah->fTraceRestMeasures) {
        gLogOstream <<
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

  gIndenter--;
}

void msrVoice::incrementVoiceCurrentMeasurePuristNumber (
  int    inputLineNumber,
  string context)
{
  fVoiceCurrentMeasurePuristNumber++;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
      "Appending measure clone '" <<
      measureClone->asShortString () <<
      "' to voice '" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  // append measureClone to the voice last segment
  fVoiceLastSegment->
    appendMeasureToSegment (
      measureClone);

  // measureClone is the new voice current measure
  fVoiceCurrentMeasure = measureClone;
}

void msrVoice::setWholeNotesSinceLastRegularMeasureEnd (
  int      inputLineNumber,
  rational value)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceSegments) {
    gLogOstream <<
      "Creating a new last segment for voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (gTraceOah->fTraceVoicesDetails) {
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoicesDetails) {
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceSegments) {
    gLogOstream <<
      "Creating a new last segment '" <<
      fVoiceLastSegment->asShortString () <<
      "' from its first measure '" <<
      firstMeasure->getMeasureNumber () <<
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

  // firstMeasure is the new voice current measure
  fVoiceCurrentMeasure = firstMeasure;

  // is firstMeasure the first one it the voice?
  if (! fVoiceFirstMeasure) {
    // yes, register it as such
    setVoiceFirstMeasure (
      firstMeasure);

    firstMeasure->
      setMeasureFirstInVoice ();
  }

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresDetails) {
    displayVoice (
      inputLineNumber,
      "createNewLastSegmentFromItsFirstMeasureForVoice()");
  }
#endif
}

S_msrMeasure msrVoice::createMeasureAndAppendItToVoice (
  int    inputLineNumber,
  string measureNumber,
  msrMeasure::msrMeasureImplicitKind
         measureImplicitKind)
{
  fVoiceCurrentMeasureNumber = measureNumber;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
      "Creating measure '" <<
      measureNumber <<
      "' and appending it to voice \"" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresDetails) {
    displayVoice (
      inputLineNumber,
      "createMeasureAndAppendItToVoice() 1");
  }
#endif

  gIndenter++;

  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
    createNewLastSegmentForVoice (
      inputLineNumber,
      "createMeasureAndAppendItToVoice() 2");
  }

  // append new measure with given number to voice last segment
  S_msrMeasure
    result =
      fVoiceLastSegment->
        createMeasureAndAppendItToSegment (
          inputLineNumber,
          measureNumber,
          measureImplicitKind);

  // result is the new voice current measure
  fVoiceCurrentMeasure = result;

  // handle voice kind
  switch (fVoiceKind) {
    case msrVoice::kVoiceHarmony:
      break;
    case msrVoice::kVoiceRegular:
      // append new measure with given number to voice harmony voice if any
      if (fHarmonyVoiceForRegularVoiceForwardLink) {
        fHarmonyVoiceForRegularVoiceForwardLink->
          createMeasureAndAppendItToVoice (
            inputLineNumber,
            measureNumber,
            measureImplicitKind);
      }
      // append new measure with given number to voice figured bass voice if any
      if (fFiguredBassVoiceForRegularVoiceForwardLink) {
        fFiguredBassVoiceForRegularVoiceForwardLink->
          createMeasureAndAppendItToVoice (
            inputLineNumber,
            measureNumber,
            measureImplicitKind);
      }
      break;
    case msrVoice::kVoiceFiguredBass:
      break;
  } // switch

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresDetails) {
    displayVoice (
      inputLineNumber,
      "createMeasureAndAppendItToVoice() 3");
  }
#endif

  gIndenter--;

  return result;
}

S_msrVoice msrVoice::createHarmonyVoiceForRegularVoice (
  int    inputLineNumber,
  string currentMeasureNumber)
{
  if (fHarmonyVoiceForRegularVoiceForwardLink) {
    stringstream s;

    s <<
      "Voice \"" <<
      getVoiceName () <<
      "\" already has a harmony voice";

    msrInternalError (
      gOahOah->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create the voice harmony voice
  int harmonyVoiceForRegularVoiceNumber =
    K_VOICE_HARMONY_VOICE_BASE_NUMBER + fVoiceNumber;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceHarmonies) {
    gLogOstream <<
      "Creating harmony voice for regular voice \"" <<
      getVoiceName () <<
      "\" with voice number " <<
      harmonyVoiceForRegularVoiceNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fHarmonyVoiceForRegularVoiceForwardLink =
    msrVoice::create (
      inputLineNumber,
      msrVoice::kVoiceHarmony,
      harmonyVoiceForRegularVoiceNumber,
      msrVoice::kCreateInitialLastSegmentYes,
      fVoiceStaffUpLink);

  // register it in the staff
  fVoiceStaffUpLink->
    registerVoiceInStaff (
      inputLineNumber,
      fHarmonyVoiceForRegularVoiceForwardLink);

  // set backward link
  fHarmonyVoiceForRegularVoiceForwardLink->
    fRegularVoiceForHarmonyVoiceBackwardLink = this;

  return fHarmonyVoiceForRegularVoiceForwardLink;
}

S_msrVoice msrVoice::createFiguredBassVoiceForRegularVoice (
  int    inputLineNumber,
  string currentMeasureNumber)
{
  if (fFiguredBassVoiceForRegularVoiceForwardLink) {
    stringstream s;

    s <<
      "Voice \"" <<
      getVoiceName () <<
      "\" already has a figured bass voice";

    msrInternalError (
      gOahOah->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create the voice figured bass voice
  int figuredBassVoiceForRegularVoiceNumber =
    K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER + fVoiceNumber;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceFiguredBasses) {
    gLogOstream <<
      "Creating figured bass voice for regular voice \"" <<
      getVoiceName () <<
      "\" with voice number " <<
      figuredBassVoiceForRegularVoiceNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fFiguredBassVoiceForRegularVoiceForwardLink =
    msrVoice::create (
      inputLineNumber,
      msrVoice::kVoiceFiguredBass,
      figuredBassVoiceForRegularVoiceNumber,
      msrVoice::kCreateInitialLastSegmentYes,
      fVoiceStaffUpLink);

  // register it in the staff
  fVoiceStaffUpLink->
    registerVoiceInStaff (
      inputLineNumber,
      fFiguredBassVoiceForRegularVoiceForwardLink);

  // set backward link
  fFiguredBassVoiceForRegularVoiceForwardLink->
    fRegularVoiceForFiguredBassVoiceBackwardLink = this;

  return fFiguredBassVoiceForRegularVoiceForwardLink;
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
      gOahOah->fInputSourceName,
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceLyrics) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceLyrics) {
    gLogOstream <<
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
#ifdef TRACE_OAH
    if (gTraceOah->fTraceLyrics) {
      gLogOstream <<
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

    gLogOstream <<
      endl << endl << endl <<
      "VOICEVOICEVOICEVOICEVOICEVOICEVOICEVOICE" <<
      endl;

    print (gLogOstream);

    gLogOstream <<
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
      gOahOah->fInputSourceName,
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

void msrVoice::appendClefToVoice (S_msrClef clef)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceClefs) {
    gLogOstream <<
      "Appending clef '" << clef->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  gIndenter++;

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

  gIndenter--;
}

void msrVoice::appendKeyToVoice (S_msrKey key)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceKeys) {
    gLogOstream <<
      "Appending key '" << key->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  gIndenter++;

  // set voice current clef
  this->setVoiceCurrentKey (key);

  // append key to last segment
  fVoiceLastSegment->
    appendKeyToSegment (key);

#ifdef TRACE_OAH
  if (gTraceOah->fTraceKeys) {
    displayVoice ( // JMI TEMP
      key->getInputLineNumber (),
      "appendKeyToVoice()");
  }
#endif

  gIndenter--;
}

void msrVoice::appendTimeToVoice (S_msrTime time)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTimes) {
    gLogOstream <<
      "Appending time '" << time->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  gIndenter++;

  // set voice current time
  this->setVoiceCurrentTime (time);

  // append time to the last segment
  fVoiceLastSegment->
    appendTimeToSegment (time);

  gIndenter--;
}

void msrVoice::appendTimeToVoiceClone (S_msrTime time) // superflous ??? JMI
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTimes) {
    gLogOstream <<
      "Appending time '" << time->asString () <<
      "' to voice clone \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  gIndenter++;

  // set voice current time
  this->setVoiceCurrentTime (time);

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
            // ignore this element and return nullptr JMI ???
            /*
            stringstream s;

            s <<
              "tuplet first element should be a note, a chord or another tuplet, found instead '" <<
              element->asShortString () <<
              "'";

            msrInternalError (
              gOahOah->fInputSourceName,
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

void msrVoice::appendHarmonyToVoice (
  S_msrHarmony harmony)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceHarmonies) {
    gLogOstream <<
      "Appending harmony " << harmony->asString () <<
      " to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  int inputLineNumber =
    harmony->getInputLineNumber ();

  // sanity check
  switch (fVoiceKind) {
    case msrVoice::kVoiceHarmony:
      appendHarmonyToHarmonyVoice (
        harmony);
      break;

    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceFiguredBass:
      {
        stringstream s;

        s <<
          "cannot append a harmony to " <<
          voiceKindAsString () <<
          " voice \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          gOahOah->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch
}

void msrVoice::appendHarmonyToHarmonyVoice (
  S_msrHarmony harmony)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceHarmonies) {
    gLogOstream <<
      "Appending harmony " << harmony->asString () <<
      " to harmony voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // append the harmony to the voice last segment
  fVoiceLastSegment->
    appendHarmonyToSegment (harmony);

  // register harmony
  fVoiceActualHarmoniesCounter++;
  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendHarmonyToVoiceClone (S_msrHarmony harmony)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceHarmonies) {
    gLogOstream <<
      "Appending harmony " << harmony->asString () <<
      " to voice clone \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  int inputLineNumber =
    harmony->getInputLineNumber ();

  switch (fVoiceKind) {
    case msrVoice::kVoiceHarmony:
      fVoiceLastSegment->
        appendHarmonyToSegmentClone (harmony);

      // register harmony
      fVoiceActualHarmoniesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceFiguredBass:
      {
        stringstream s;

        s <<
          "cannot append a harmony to " <<
          voiceKindAsString () <<
          " voice clone \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          gOahOah->fInputSourceName,
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceFiguredBasses) {
    gLogOstream <<
      "Appending figured bass " << figuredBass->asString () <<
      " to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  int inputLineNumber =
    figuredBass->getInputLineNumber ();

  switch (fVoiceKind) {
    case msrVoice::kVoiceFiguredBass:
      // skip to figured bass note position in the voice
      padUpToPositionInMeasureInVoice (
        inputLineNumber,
        figuredBass->
          getFiguredBassNoteUpLink ()->
            getPositionInMeasure ());

      // append the figured bass to the voice last segment
      fVoiceLastSegment->
        appendFiguredBassToSegment (figuredBass);

      // register figured bass
      fVoiceActualFiguredBassesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
      {
        stringstream s;

        s <<
          "cannot append a figured bass to " <<
          voiceKindAsString () <<
          " voice \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          gOahOah->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch
}

void msrVoice::appendFiguredBassToVoiceClone (
  S_msrFiguredBass figuredBass)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceFiguredBasses) {
    gLogOstream <<
      "Appending figured bass " << figuredBass->asString () <<
      " to voice clone \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kVoiceFiguredBass:
      fVoiceLastSegment->
        appendFiguredBassToSegmentClone (figuredBass);

      // register figured bass
      fVoiceActualFiguredBassesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
      {
        stringstream s;

        s <<
          "cannot append a figured bass to " <<
          voiceKindAsString () <<
          " voice clone \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          gOahOah->fInputSourceName,
          figuredBass->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch
}

void msrVoice::padUpToPositionInMeasureInVoice (
  int      inputLineNumber,
  rational wholeNotesPositionInMeasure)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTracePositionsInMeasures) {
    gLogOstream <<
      "Padding up to position in measure '" <<
      wholeNotesPositionInMeasure <<
      "' whole notes in voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  // sanity check
  msrAssert (
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
      i++
  ) {
      S_msrStanza stanza = (*i).second;

      stanza->
        padUpToCurrentMeasureWholeNotesDurationInStanza (
          inputLineNumber,
          wholeNotesPositionInMeasure);
    } // for
  }

  gIndenter--;
}

void msrVoice::backupByWholeNotesStepLengthInVoice (
  int      inputLineNumber,
  rational backupTargetPositionInMeasure)
{
#ifdef TRACE_OAH
  if (gMusicXMLOah->fTraceBackup) {
    gLogOstream <<
      "Backup by a '" <<
      backupTargetPositionInMeasure <<
      "' whole notes step length in voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  // sanity check
  msrAssert (
    fVoiceLastSegment != nullptr,
    "fVoiceLastSegment is null");

  // pad up the voice's last segment
  fVoiceLastSegment->
    backupByWholeNotesStepLengthInSegment (
      inputLineNumber,
      backupTargetPositionInMeasure);

  gIndenter--;
}

void msrVoice::appendPaddingNoteToVoice (
  int      inputLineNumber,
  rational forwardStepLength)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotes) {
    gLogOstream <<
      "Appending padding note" <<
      ", forwardStepLength: " <<
      forwardStepLength <<
      ", to voice \"" <<
      getVoiceName () <<
      "\",line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

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
      i++
  ) {
      S_msrStanza stanza = (*i).second;

      stanza->appendPaddingNoteToStanza (
        inputLineNumber,
        forwardStepLength);
    } // for
  }

  gIndenter--;
}

void msrVoice::appendTransposeToVoice (
  S_msrTranspose transpose)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTranspositions) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceStaves) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTempos) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOctaveShifts) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceScordaturas) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceAccordionRegistrations) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceHarpPedals) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRehearsals) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  int inputLineNumber =
    voiceStaffChange->getInputLineNumber ();
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceStaffChanges) {
    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotes) {
    gLogOstream <<
      "Appending note '" <<
      endl;

    gIndenter++;

    gLogOstream <<
      note <<
        "' to voice \"" << getVoiceName () << "\"" <<
        ", line " << inputLineNumber <<
        endl;

    gIndenter--;
  }
#endif

  // fetch the part current position in measure
  rational
    partCurrentPositionInMeasure =
      fVoiceStaffUpLink->
        getStaffPartUpLink ()->
          getPartCurrentPositionInMeasure ();

  // append the note to the last segment
  fVoiceLastSegment->
    appendNoteToSegment (
      note,
      partCurrentPositionInMeasure);

  // is this note the shortest one in this voice?
  this->
    registerShortestNoteIfRelevant (note);

  // register note as the last appended one into this voice
  fVoiceLastAppendedNote = note;

  // account for note's duration in staff
  fVoiceStaffUpLink->
    getStaffPartUpLink ()->
      incrementPartCurrentPositionInMeasure (
        inputLineNumber,
        note->getNoteSoundingWholeNotes ());

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
}

void msrVoice::appendNoteToVoiceClone (S_msrNote note) {
#ifdef TRACE_OAH
  int inputLineNumber =
    note->getInputLineNumber ();
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotes) {
    gLogOstream <<
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
    registerShortestNoteIfRelevant (note);

  // register note as the last appended one into this voice
  fVoiceLastAppendedNote = note;

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
}

void msrVoice::appendDoubleTremoloToVoice (
  S_msrDoubleTremolo doubleTremolo)

{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTremolos) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceChords) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTuplets) {
    gLogOstream <<
      "Appending tuplet " << tuplet->asString () <<
      " to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  gIndenter++;

  // append tuplet to voice last segment
  fVoiceLastSegment->
    appendTupletToSegment (tuplet);

  // account for tuplet duration in the part's current position in measure
  fVoiceStaffUpLink->
    getStaffPartUpLink ()->
      incrementPartCurrentPositionInMeasure (
        tuplet->getInputLineNumber (),
        tuplet->getTupletSoundingWholeNotes ());

  gIndenter--;

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::addGraceNotesGroupBeforeAheadOfVoiceIfNeeded (
  S_msrGraceNotesGroup graceNotesGroup)
{
  int inputLineNumber =
    graceNotesGroup->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceGraceNotes) {
    gLogOstream <<
      "Adding grace notes '" <<
      graceNotesGroup->asString () <<
      "' ahead of voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

/* JMI
  gLogOstream <<
    endl <<
    "======================= prependGraceNotesToVoice" <<
    endl;
  this->print (gLogOstream);
  gLogOstream <<
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
      msrMeasure::kMeasureImplicitKindNo);
  }

  // such grace notes groups should be attached to the voice's first note,
  // or to the first chord if the latter belongs to such

  // fetch the voice's first note
  S_msrNote
    voiceFirstNote =
      fetchVoiceFirstNonGraceNote (); // JMI

  // get the voice first note's chord upLink
  S_msrChord
    firstNoteChordUpLink =
      voiceFirstNote->
        getNoteChordUpLink ();

  if (firstNoteChordUpLink) {
#ifdef TRACE_OAH
    if (gTraceOah->fTraceGraceNotes) {
      gLogOstream <<
        "Attaching grace notes before '" <<
        graceNotesGroup->asString () <<
        "' to the first chord of voice \"" << getVoiceName () <<
        "\", i.e. '" <<
        firstNoteChordUpLink->asShortString () <<
        "'" <<
        endl;
    }
#endif

    firstNoteChordUpLink->
      setChordGraceNotesGroupBefore (
        graceNotesGroup);
  }

  else {
#ifdef TRACE_OAH
    if (gTraceOah->fTraceGraceNotes) {
      gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceGraceNotes) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceGraceNotes) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceLyrics) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
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
      i++
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
      "Appending page break '" << pageBreak->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendPageBreakToSegment (pageBreak);
}

void msrVoice::prependOtherElementToVoice (S_msrMeasureElement elem) {
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
      "Prepending other element '" << elem <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    prependOtherElementToSegment (elem);
}

void msrVoice::appendOtherElementToVoice (S_msrMeasureElement elem) {
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
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
      gOahOah->fInputSourceName,
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
      gOahOah->fInputSourceName,
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
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
      gOahOah->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
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
  int repeatDescrsStackSize =
    fVoicePendingRepeatDescrsStack.size ();

  gLogOstream <<
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

    gIndenter++;

    int n = repeatDescrsStackSize;
    for ( ; ; ) {
      gLogOstream <<
        "v (" << n << ")" <<
        ", repeatDescrStartInputLineNumber: " <<
        (*i)->getRepeatDescrStartInputLineNumber () <<
        endl;

      gIndenter++;
      (*i)->
        getRepeatDescrRepeat ()->
          shortPrint (gLogOstream);
      gIndenter--;

      n--;

      if (++i == iEnd) break;

      gLogOstream << endl;
    } // for

    gIndenter--;
  }

  gLogOstream <<
    "<<++++++++++++++++ " <<
    endl <<
    endl;
}

void msrVoice::displayVoiceRepeatsStackSummary (
  int    inputLineNumber,
  string context)
{
  int repeatDescrsStackSize =
    fVoicePendingRepeatDescrsStack.size ();

  gLogOstream <<
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

    gIndenter++;

    int n = repeatDescrsStackSize;
    for ( ; ; ) {
      gLogOstream <<
        "v (" << n << ")" <<
        ", repeatDescrStartInputLineNumber: " <<
        (*i)->getRepeatDescrStartInputLineNumber () <<
        endl;

      gIndenter++;
      (*i)->
        getRepeatDescrRepeat ()->
          shortPrint (gLogOstream);
      gIndenter--;

      n--;

      if (++i == iEnd) break;

      gLogOstream << endl;
    } // for

    gLogOstream <<
      "===============" <<
      endl <<
      endl;

    gIndenter--;
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
  gLogOstream <<
    endl <<
    ">>++++++++++++++++ Displaying voice measures repeats " << context <<
    endl <<
    "The current voice measures repeat contains " <<
    ", line " << inputLineNumber <<
    ":" <<
    endl;

  gIndenter++;

  if (fVoicePendingMeasuresRepeat) {
    fVoicePendingMeasuresRepeat->
      displayMeasuresRepeat (
        inputLineNumber,
        context);
  }
  else {
    gLogOstream <<
      "none" <<
      endl;
  }

  gIndenter--;

  gLogOstream <<
    "<<++++++++++++++++ " <<
    endl <<
    endl;
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
  gLogOstream <<
    endl <<
    ">>++++++++++++++++ voice rest measures " << context <<
    endl <<
    "The current voice rest measures contains " <<
    ", line " << inputLineNumber <<
    ":" <<
    endl;

  gIndenter++;

  if (fVoicePendingRestMeasures) {
    fVoicePendingRestMeasures->
      displayRestMeasures (
        inputLineNumber,
        context);
  }
  else {
    gLogOstream <<
      "none" <<
      endl;
  }

  gIndenter--;

  gLogOstream <<
    "<<++++++++++++++++ " <<
    endl <<
    endl;
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Moving the voice last segment '";

    if (fVoiceLastSegment) { // JMI should not be necessary?
      gLogOstream <<
        fVoiceLastSegment->asShortString ();
    }
    else {
      gLogOstream <<
        "null";
    }

    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
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
      kVoiceRepeatPhaseAfterCommonPart);

    // append fVoiceLastSegment to the repeat common part
    repeatCommonPart->
      appendSegmentToRepeatCommonPart (
        inputLineNumber,
        fVoiceLastSegment,
        context);

    // forget about this voice last segment
    fVoiceLastSegment = nullptr;
  }

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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

void msrVoice::appendRestMeasuresToInitialVoiceElements (
  int               inputLineNumber,
  S_msrRestMeasures restMeasures,
  string            context)
{
  // append restMeasures to the list of initial elements
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceSegments) {
    gLogOstream <<
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
#ifdef TRACE_OAH
      if (gTraceOah->fTraceSegments) {
        gLogOstream <<
          "Moving voice last segment '" <<
          fVoiceLastSegment->asString () <<
          "' to the initial elements in voice \"" <<
          getVoiceName () <<
          "\" (" << context << ")" <<
          ", line " << inputLineNumber <<
          endl;
      }

      if (gTraceOah->fTraceSegmentsDetails) {
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

#ifdef TRACE_OAH
      if (gTraceOah->fTraceSegmentsDetails) {
        displayVoice (
          inputLineNumber,
          "moveVoiceLastSegmentToInitialVoiceElementsIfRelevant() 2");
      }
#endif
    }

    else {
#ifdef TRACE_OAH
      if (gTraceOah->fTraceSegments) {
        gLogOstream <<
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
#ifdef TRACE_OAH
    if (gTraceOah->fTraceSegments) {
      gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Handling a voice-level repeat start in voice \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatStartInVoice() 1");
  }
#endif

  gIndenter++;

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

#ifdef TRACE_OAH
      if (gTraceOah->fTraceRepeatsDetails) {
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

        // is there a measure splitting?
        if (
          lastMeasureInLastSegment->getCurrentMeasureWholeNotesDuration ()
            == // JMI SURE ???
          lastMeasureInLastSegment->getFullMeasureWholeNotesDuration ()
        ) {
          // yes this measure is not yet complete and should be split
#ifdef TRACE_OAH
          if (gTraceOah->fTraceRepeats) {
            gLogOstream <<
              "Splitting measure '" <<
              lastMeasureInLastSegment->asShortString () <<
              "' upon a repeat end in voice \"" <<
              getVoiceName () <<
              "\"" <<
              ", line " << inputLineNumber <<
              endl;
          }
#endif

          // finalize lastMeasureInLastSegment
          lastMeasureInLastSegment->
            finalizeMeasure (
              inputLineNumber,
              msrMeasure::kMeasuresRepeatContextKindNone,
              "handleVoiceLevelRepeatStartInVoice() 3");

          // append last segment to initial voice elements list
          appendVoiceLastSegmentToInitialVoiceElements (
            inputLineNumber,
            "handleVoiceLevelRepeatStartInVoice() 4");

          // create a new last segment for the voice
#ifdef TRACE_OAH
          if (gTraceOah->fTraceSegments) {
            gLogOstream <<
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
            lastMeasureInLastSegment->getMeasureNumber (),
            msrMeasure::kMeasureImplicitKindNo);

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
#ifdef TRACE_OAH
        if (gTraceOah->fTraceRepeats) {
          gLogOstream <<
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
#ifdef TRACE_OAH
        if (gTraceOah->fTraceRepeats) {
          gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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
        newRepeat);

  // register it in newRepeat
  newRepeat->
    setRepeatCommonPart (
      repeatCommonPart);

  // set newRepeat as having an explicit start
  newRepeat->
    setRepeatExplicitStartKind (
      msrRepeat::kRepeatExplicitStartYes);

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatStartInVoice() 11");
  }
#endif

  gIndenter--;
}

void msrVoice::handleNestedRepeatStartInVoice (
  int inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Handling a nested repeat start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleNestedRepeatStartInVoice() 2");
  }
#endif
}

void msrVoice::handleRepeatStartInVoice (
  int inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Handling repeat start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatStartInVoice() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Handling a voice-level repeat end without start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndWithoutStartInVoice() 1");
  }
#endif

  gIndenter++;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "This repeat end without a start is at the voice-level" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create the repeat
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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
  // before finalizeCurrentMeasureInVoice()
  setCurrentVoiceRepeatPhaseKind (
    inputLineNumber,
    kVoiceRepeatPhaseAfterCommonPart);

  // finalize current measure in voice
  voiceLastSegmentLastMeasure->
    finalizeMeasure (
      inputLineNumber,
      msrMeasure::kMeasuresRepeatContextKindCommonPartLastMeasure,
      "handleVoiceLevelRepeatEndWithoutStartInVoice() 3"
      );

  // append the voice last segment to the new repeat common part
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndWithoutStartInVoice() 7");
  }
#endif

  gIndenter--;
}

void msrVoice::handleVoiceLevelContainingRepeatEndWithoutStartInVoice (
  int    inputLineNumber,
  string measureNumber,
  int    repeatTimes)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Handling a voice-level rcontaining epeat end without start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelContainingRepeatEndWithoutStartInVoice() 1");
  }
#endif

  gIndenter++;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "This repeat end without a start is at the voice-level" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create the repeat
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelContainingRepeatEndWithoutStartInVoice() 7");
  }
#endif

  gIndenter--;
}

void msrVoice::handleVoiceLevelRepeatEndWithStartInVoice (
  int    inputLineNumber,
  string measureNumber,
  int    repeatTimes)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Handling a voice-level repeat end with start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndWithStartInVoice() 1");
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "This repeat end with a start is at the voice-level" <<
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

  // set voice current after repeat component phase kind
  setCurrentVoiceRepeatPhaseKind (
    inputLineNumber,
    kVoiceRepeatPhaseAfterCommonPart);

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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndWithStartInVoice() 5");
  }
#endif

  gIndenter--;
}

void msrVoice::handleNestedRepeatEndInVoice (
  int    inputLineNumber,
  string measureNumber,
  int    repeatTimes)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Handling a nested repeat end in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleNestedRepeatEndInVoice() 1");
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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
#ifdef TRACE_OAH
    if (gTraceOah->fTraceRepeats) {
      gLogOstream <<
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
      msrMeasure::kMeasureImplicitKindNo);

  /* JMI
    // set it as created after a repeat
    voiceLastMeasure->
      setMeasureCreatedForARepeatKind (
        msrMeasure::kMeasureCreatedForARepeatAfter);
        */
  }

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatEndInVoice() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
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
                      gOahOah->fInputSourceName,
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Handling a voice-level repeat ending start without explicit start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice() 1");
  }
#endif

  gIndenter++;

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
#ifdef TRACE_OAH
    if (gTraceOah->fTraceRepeats) {
      gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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
#ifdef TRACE_OAH
    if (gTraceOah->fTraceRepeats) {
      gLogOstream <<
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
#ifdef TRACE_OAH
    if (gTraceOah->fTraceRepeats) {
      gLogOstream <<
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
#ifdef TRACE_OAH
    if (gTraceOah->fTraceSegments) {
      gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice() 2");
  }
#endif

  gIndenter--;
}

void msrVoice::handleVoiceLevelRepeatEndingStartWithExplicitStartInVoice (
  int inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Handling a voice-level repeat ending start with explicit start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithExplicitStartInVoice() 1");
  }
#endif

  gIndenter++;

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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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
#ifdef TRACE_OAH
    if (gTraceOah->fTraceRepeats) {
      gLogOstream <<
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
#ifdef TRACE_OAH
    if (gTraceOah->fTraceRepeats) {
      gLogOstream <<
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
#ifdef TRACE_OAH
    if (gTraceOah->fTraceSegments) {
      gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithExplicitStartInVoice() 7");
  }
#endif

  gIndenter--;
}

void msrVoice::nestContentsIntoNewRepeatInVoice (
  int inputLineNumber)
{
  switch (fVoiceKind) {
    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
      // is there a voice last segment?
      if (fVoiceLastSegment) {

        // are there measures in the voice last segment?
        if (fVoiceLastSegment->getSegmentMeasuresList ().size ()) {
          // finalize current measure in voice
          finalizeCurrentMeasureInVoice (
            inputLineNumber);

#ifdef TRACE_OAH
          if (gTraceOah->fTraceRepeats) {
            displayVoiceRepeatsStackSummary (
              inputLineNumber,
              "nestContentsIntoNewRepeatInVoice() 1");
          }
#endif

          // move voice last segment to the list of initial elements
          moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
            inputLineNumber,
            "nestContentsIntoNewRepeatInVoice() 1");

#ifdef TRACE_OAH
          if (gTraceOah->fTraceRepeats) {
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Handling a nested repeat ending start voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
 }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleNestedRepeatEndingStartInVoice() 2");
  }
#endif
}

void msrVoice::handleRepeatEndingStartInVoice (
  int inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Handling a repeat ending start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatEndingStartInVoice() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
      {
        gIndenter++;

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

        gIndenter--;
      }
      break;
  } // switch

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatEndingStartInVoice() 2");
  }
#endif
}

void msrVoice::handleRepeatEndingStartInVoiceClone (
  int       inputLineNumber,
  msrRepeatEnding::msrRepeatEndingKind
            repeatEndingKind,
  string    repeatEndingNumber) // may be "1, 2"
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatEndingStartInVoiceClone() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
      {
        // handle the repeat ending start
        gIndenter++;

#ifdef TRACE_OAH
        if (gTraceOah->fTraceRepeats) {
          gLogOstream <<
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
                gOahOah->fInputSourceName,
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
#ifdef TRACE_OAH
              if (gTraceOah->fTraceRepeats) {
                gLogOstream <<
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

              // add the repeat ending to the voice current repeat
#ifdef TRACE_OAH
              if (gTraceOah->fTraceRepeats) {
                gLogOstream <<
                  "Appending a " <<
                  msrRepeatEnding::repeatEndingKindAsString (
                    repeatEndingKind) <<
                  " repeat ending to current repeat in voice \"" <<
                  fVoiceName <<
                  "\"" <<
                  endl;
              }
#endif

#ifdef TRACE_OAH
              if (gTraceOah->fTraceRepeats) {
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
#ifdef TRACE_OAH
              if (gTraceOah->fTraceRepeats) {
                gLogOstream <<
                  "Moving the voice last segment to the repeat common part in voice clone \"" <<
                  getVoiceName () <<
                  "\"" <<
                  ", line " << inputLineNumber <<
                  endl;
              }
#endif
            }
        } // switch

        gIndenter--;
      }
      break;
  } // switch

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceSegments) {
    gLogOstream <<
      "Handling segment '" <<
      segmentClone->asShortString () <<
      "' in voice clone \"" <<
      fVoiceName <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceSegmentsDetails) {
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
            gOahOah->fInputSourceName,
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
            gOahOah->fInputSourceName,
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceSegmentsDetails) {
    displayVoice (
      inputLineNumber,
      "handleSegmentCloneEndInVoiceClone() 3");
  }
#endif

  gIndenter--;
}

/* JMI
void msrVoice::finalizeRepeatEndInVoice (
  int    inputLineNumber,
  string measureNumber,
  int    repeatTimes)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "finalizeRepeatEndInVoice() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
      {
        // finalize current measure in voice
        finalizeCurrentMeasureInVoice (
          inputLineNumber);

        // fetch the repeat
        if (! fVoicePendingRepeatDescrsStack.size ()) {
          stringstream s;

          s <<
            "repeats stack is empty when attempting to finalize a repeat in voice '" <<
            asShortString () <<
            "' ";

          msrInternalError (
            gOahOah->fInputSourceName,
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
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
      gOahOah->fInputSourceName,
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
      "Removing the last " <<
      singularOrPlural (
        measuresRepeatMeasuresNumber, "measure", "measures") <<
      " (to be repeated) from voice \"" <<
      fVoiceName <<
      endl;
  }
#endif

  for (int i = 0; i < measuresRepeatMeasuresNumber; i++) {
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
      gOahOah->fInputSourceName,
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    gLogOstream <<
      "Appending rest measures '" <<
      restMeasures->asShortString () <<
      "' to voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
      "Appending measures repeat '" <<
      measuresRepeat->asShortString () <<
      "' to voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "appendMeasuresRepeatToVoice() 3");
  }
#endif
}

void msrVoice::appendPendingMeasuresRepeatToVoice (
  int inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
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
      gOahOah->fInputSourceName,
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
      gOahOah->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  S_msrMeasure
    nextMeasureAfterMeasuresRepeat =
      voiceLastSegmentMeasureList.back ();

// BOFBOFBOF JMI
  // remove the next measure from the last segment's measure list
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
      "Removing last measure in last segment" <<
      "in voice \"" <<
      fVoiceName <<
      "\":" <<
      endl;
    gIndenter++;
    gLogOstream <<
      nextMeasureAfterMeasuresRepeat;
    gLogOstream << endl;
  }
#endif

  voiceLastSegmentMeasureList.pop_back ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "appendPendingMeasuresRepeatToVoice() 2");
  }
#endif

  // create the measures repeat replicas contents
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
      "Setting pending measures repeat replicas segment in voice \"" <<
      getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fVoicePendingMeasuresRepeat->
    setMeasuresRepeatReplicas (
      measuresRepeatReplicas);

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
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
    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
      {
        // does the pending measures repeat exist?
        if (fVoicePendingMeasuresRepeat) {
          stringstream s;

          s <<
            "attempting to create a measures repeat while another one is pending";

          msrInternalError (
            gOahOah->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

        // create the measures repeat
#ifdef TRACE_OAH
        if (gTraceOah->fTraceMeasuresRepeats) {
          gLogOstream <<
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
#ifdef TRACE_OAH
        if (gTraceOah->fTraceMeasuresRepeats) {
          gLogOstream <<
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
#ifdef TRACE_OAH
        if (gTraceOah->fTraceMeasuresRepeats) {
          gLogOstream <<
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
#ifdef TRACE_OAH
        if (gTraceOah->fTraceMeasuresRepeats) {
          gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    gLogOstream <<
      "Creating a rest measures in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", " <<
      singularOrPlural (
        restMeasuresNumber, "measure", "measures") <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    displayVoiceRestMeasuresAndVoice (
      inputLineNumber,
      "createRestMeasuresInVoice() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
      {
        gIndenter++;

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
            gOahOah->fInputSourceName,
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
#ifdef TRACE_OAH
        if (gTraceOah->fTraceRestMeasures) {
          gLogOstream <<
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

#ifdef TRACE_OAH
        if (gTraceOah->fTraceRestMeasures) {
          gLogOstream <<
            "Setting fVoiceRemainingRestMeasures to '" <<
            fVoiceRemainingRestMeasures <<
            "' in voice \"" <<
            fVoiceName << "\"" <<
            endl;
        }
#endif

        // create a new segment to collect the rest measures,
        // containing the first, rest measure
#ifdef TRACE_OAH
        if (gTraceOah->fTraceRestMeasures) {
          gLogOstream <<
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

        gIndenter--;
      }
      break;
  } // switch

  // print resulting voice contents
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    displayVoiceRestMeasuresAndVoice (
      inputLineNumber,
      "createRestMeasuresInVoice() 4");
  }
#endif
}

void msrVoice::appendPendingRestMeasuresToVoice (
    int inputLineNumber)
{
  // a rest measures is a voice element,
  // and can be voice-level as well as part of a repeat

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    displayVoiceRestMeasuresAndVoice (
      inputLineNumber,
      "appendPendingRestMeasuresToVoice() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
      {
        // does the pending rest measures exist?
        if (! fVoicePendingRestMeasures) {
          stringstream s;

          s <<
            "attempting to append a pending rest measures which doesn't exist";

          msrInternalError (
            gOahOah->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

#ifdef TRACE_OAH
        if (gTraceOah->fTraceRestMeasures) {
          gLogOstream <<
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
#ifdef TRACE_OAH
        if (gTraceOah->fTraceRestMeasures) {
          gLogOstream <<
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
#ifdef TRACE_OAH
        if (gTraceOah->fTraceRestMeasures) {
          gLogOstream <<
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
#ifdef TRACE_OAH
        if (gTraceOah->fTraceRestMeasures) {
          gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    gLogOstream <<
      "Handling rest measures start in voice clone \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    displayVoiceRestMeasuresAndVoice (
      inputLineNumber,
      "handleRestMeasuresStartInVoiceClone() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:

      // is there a voice last segment?
      if (fVoiceLastSegment) {

        // are there measures in the voice last segment?
        if (fVoiceLastSegment->getSegmentMeasuresList ().size ()) {

          // finalize current measure in voice
          finalizeCurrentMeasureInVoice (
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

#ifdef TRACE_OAH
          if (gTraceOah->fTraceRestMeasures) {
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
          gOahOah->fInputSourceName,
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    displayVoiceRestMeasuresAndVoice (
      inputLineNumber,
      "handleRestMeasuresStartInVoiceClone() 4");
  }
#endif

  gIndenter--;
}

void msrVoice::handleRestMeasuresEndInVoiceClone (
  int inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    gLogOstream <<
      "Handling rest measures end in voice clone \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    displayVoiceRestMeasuresAndVoice (
      inputLineNumber,
      "handleRestMeasuresEndInVoiceClone() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:

      // is there a current rest measures in this voice?
      if (! fVoicePendingRestMeasures) {
        stringstream s;

        s <<
          "current voice rest measures is null when attempting to handle rest measures end '" <<
          "' in voice clone '" <<
          asShortString () <<
          "' ";

        msrInternalError (
          gOahOah->fInputSourceName,
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

#ifdef TRACE_OAH
      if (gTraceOah->fTraceRestMeasures) {
        displayVoice (
          inputLineNumber,
          "handleRestMeasuresEndInVoiceClone() 3");
      }
#endif
    break;
  } // switch

  gIndenter--;
}

void msrVoice::handleRestMeasuresContentsStartInVoiceClone (
  int inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    gLogOstream <<
      "Handling rest measures contents start in voice clone \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    displayVoiceRestMeasuresAndVoice (
      inputLineNumber,
      "handleRestMeasuresContentsStartInVoiceClone() 1");
  }
#endif

  gIndenter++;

  if (! fVoicePendingRestMeasures) {
    stringstream s;

    s <<
      "current voice rest measures is null when attempting to handle rest measures contents start '" <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gOahOah->fInputSourceName,
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create fVoicePendingRestMeasures' rest contents
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    displayVoiceRestMeasuresAndVoice (
      inputLineNumber,
      "handleRestMeasuresContentsStartInVoiceClone() 2");
  }
#endif

  gIndenter--;
}

void msrVoice::handleRestMeasuresContentsEndInVoiceClone (
  int inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    gLogOstream <<
      "Handling rest measures contents end in voice clone \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
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
      gOahOah->fInputSourceName,
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    displayVoiceRestMeasuresAndVoice (
      inputLineNumber,
      "handleRestMeasuresContentsEndInVoiceClone() 2");
  }
#endif

  gIndenter--;
}

void msrVoice::appendRestMeasuresCloneToVoiceClone (
  int               inputLineNumber,
  S_msrRestMeasures restMeasuresClone)
{
  gIndenter++;

  // sanity check
  msrAssert(
    restMeasuresClone != nullptr,
    "restMeasuresClone is null");

  switch (fVoiceKind) {
    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
      {
#ifdef TRACE_OAH
        if (gTraceOah->fTraceRestMeasures) {
          gLogOstream <<
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
#ifdef TRACE_OAH
        if (gTraceOah->fTraceRestMeasures) {
          displayVoice (
            inputLineNumber,
            "appendRestMeasuresCloneToVoiceClone() 3");
        }
#endif
      }
      break;
  } // switch

  gIndenter--;
}

void msrVoice::appendRepeatCloneToVoiceClone (
  int         inputLineNumber,
  S_msrRepeat repeatCLone)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Appending repeat clone '" <<
      repeatCLone->asString () <<
      "' to voice clone \"" <<
      getVoiceName () <<  "\"" <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "appendRepeatCloneToVoiceClone() 1");
  }
#endif

  gIndenter++;

  // sanity check
  msrAssert(
    repeatCLone != nullptr,
    "repeatCLone is null");

  switch (fVoiceKind) {
    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
      {
        // pushing repeat clone as the (new) current repeat
#ifdef TRACE_OAH
        if (gTraceOah->fTraceRepeats) {
          gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "appendRepeatCloneToVoiceClone() 4");
  }
#endif

  gIndenter--;
}

void msrVoice::handleMeasuresRepeatStartInVoiceClone (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeat)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
      "Handling measures repeat start in voice clone \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "handleMeasuresRepeatStartInVoiceClone() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
      // is there already a current measures repeat in this voice?
      if (fVoicePendingMeasuresRepeat) {
        stringstream s;

        s <<
          "current voice measures repeat is not null when attempting to handle measures repeat start '" <<
          "' in voice clone '" <<
          asShortString () <<
          "' ";

        msrInternalError (
          gOahOah->fInputSourceName,
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

#ifdef TRACE_OAH
      if (gTraceOah->fTraceMeasuresRepeats) {
        displayVoiceMeasuresRepeatAndVoice (
          inputLineNumber,
          "handleMeasuresRepeatStartInVoiceClone() 2");
      }
#endif
      break;
  } // switch

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "handleMeasuresRepeatStartInVoiceClone() 3");
  }
#endif

  gIndenter--;
}

void msrVoice::handleMeasuresRepeatEndInVoiceClone (
  int inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
      "Handling measures repeat end in voice clone \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "handleMeasuresRepeatEndInVoiceClone() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
      // is there a current measures repeat in this voice?
      if (! fVoicePendingMeasuresRepeat) {
        stringstream s;

        s <<
          "current voice measures repeat is null when attempting to handle measures repeat end '" <<
          "' in voice clone '" <<
          asShortString () <<
          "' ";

        msrInternalError (
          gOahOah->fInputSourceName,
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "handleMeasuresRepeatEndInVoiceClone() 3");
  }
#endif

  gIndenter--;
}

void msrVoice::handleMeasuresRepeatPatternStartInVoiceClone (
  int inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
      "Handling measures repeat start in voice clone \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
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
      gOahOah->fInputSourceName,
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create fVoicePendingMeasuresRepeat' rest pattern
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "handleMeasuresRepeatPatternStartInVoiceClone() 2");
  }
#endif

  gIndenter--;
}

void msrVoice::handleMeasuresRepeatPatternEndInVoiceClone (
  int inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
      "Handling measures repeat end in voice clone \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
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
      gOahOah->fInputSourceName,
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "handleMeasuresRepeatPatternEndInVoiceClone() 2");
  }
#endif

  gIndenter--;
}

void msrVoice::handleMeasuresRepeatReplicasStartInVoiceClone (
  int inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
      "Handling measures repeat start in voice clone \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
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
      gOahOah->fInputSourceName,
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create fVoicePendingMeasuresRepeat' replicas
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "handleMeasuresRepeatReplicasStartInVoiceClone() 2");
  }
#endif

  gIndenter--;
}

void msrVoice::handleMeasuresRepeatReplicasEndInVoiceClone (
  int inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
      "Handling measures repeat end in voice clone \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
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
      gOahOah->fInputSourceName,
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    displayVoiceMeasuresRepeatAndVoice (
      inputLineNumber,
      "handleMeasuresRepeatReplicasEndInVoiceClone() 2");
  }
#endif

  gIndenter--;
}

void msrVoice::appendMeasuresRepeatCloneToVoiceClone (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeatClone)
{
  gIndenter++;

  // sanity check
  msrAssert(
    measuresRepeatClone != nullptr,
    "measuresRepeatClone is null");

  switch (fVoiceKind) { // superflous JMI ???
    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
      {
#ifdef TRACE_OAH
        if (gTraceOah->fTraceMeasuresRepeats) {
          gLogOstream <<
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
#ifdef TRACE_OAH
        if (gTraceOah->fTraceMeasuresRepeats) {
          displayVoiceMeasuresRepeatAndVoice (
            inputLineNumber,
            "appendMeasuresRepeatCloneToVoiceClone() 3");
        }
#endif
      }
      break;
  } // switch

  gIndenter--;
}

void msrVoice::handleHookedRepeatEndingEndInVoice (
  int       inputLineNumber,
  string    repeatEndingNumber) // may be "1, 2"
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Handling a hooked repeat ending in voice \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHookedRepeatEndingEndInVoice() 1");
  }
#endif

  gIndenter++;

/* JMI
        int voicerepeatDescrsStackSize =
          fVoicePendingRepeatDescrsStack.size ();
*/

  if (! fVoicePendingRepeatDescrsStack.size ()) {
    stringstream s;

    s <<
      "repeats stack is empty when attempting to handle a hooked repeat ending end in voice '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gOahOah->fInputSourceName,
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // finalize current measure in voice
  finalizeCurrentMeasureInVoice (
    inputLineNumber);

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatDescrsStack.front ()->
        getRepeatDescrRepeat ();

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

  // move the voice last segment to repeatEnding
  moveVoiceLastSegmentToRepeatEnding (
    inputLineNumber,
    repeatEnding,
    "handleHookedRepeatEndingEndInVoice() 2");

  // add the repeat ending to the voice current repeat
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Appending a " <<
      msrRepeatEnding::repeatEndingKindAsString (
        repeatEndingKind) <<
      " repeat ending to current repeat in voice \"" <<
      fVoiceName <<
      "\"" <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "before adding a hooked repeat ending to current repeat");
  }
#endif

  currentRepeat->
    addRepeatEndingToRepeat (
      inputLineNumber,
      repeatEnding);

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHookedRepeatEndingEndInVoice() 3");
  }
#endif

  gIndenter--;
}

void msrVoice::handleHooklessRepeatEndingEndInVoice (
  int       inputLineNumber,
  string    repeatEndingNumber) // may be "1, 2"
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Handling a hookless repeat ending in voice \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      endl;  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHooklessRepeatEndingEndInVoice() 1");
  }
#endif

  gIndenter++;

  if (! fVoicePendingRepeatDescrsStack.size ()) {
    stringstream s;

    s <<
      "repeats stack is empty when attempting to handle a hookless repeat ending end in voice '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gOahOah->fInputSourceName,
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // finalize current measure in voice
  finalizeCurrentMeasureInVoice (
    inputLineNumber);

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatDescrsStack.front ()->
        getRepeatDescrRepeat ();

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
    "handleHookedRepeatEndingEndInVoice() 2");

  // add the repeat ending it to the voice current repeat
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Appending a " <<
      msrRepeatEnding::repeatEndingKindAsString (
        repeatEndingKind) <<
      " repeat ending to current repeat in voice \"" <<
      fVoiceName <<
      "\"" <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "before adding a hookless repeat ending to current repeat");
  }
#endif

  currentRepeat->
    addRepeatEndingToRepeat (
      inputLineNumber,
      repeatEnding);

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHooklessRepeatEndingEndInVoice() 5");
  }
#endif

  gIndenter--;
}

void msrVoice::handleRepeatEndingEndInVoice (
  int       inputLineNumber,
  string    repeatEndingNumber, // may be "1, 2"
  msrRepeatEnding::msrRepeatEndingKind
            repeatEndingKind)
{
  switch (fVoiceKind) {
    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "appendRepeatEndingToVoice() 0");
  }
#endif
}

void msrVoice::handleRepeatCommonPartStartInVoiceClone (
  int inputLineNumber) // may be "1, 2"
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Handling a repeat common part start in voice clone \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatCommonPartStartInVoiceClone() 1");
  }
#endif

  gIndenter++;

  if (! fVoicePendingRepeatDescrsStack.size ()) {
    stringstream s;

    s <<
      "repeats stack is empty when attempting to handle repeat common part start '" <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gOahOah->fInputSourceName,
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatCommonPartStartInVoiceClone() 2");
  }
#endif

  gIndenter--;
}

void msrVoice::handleRepeatCommonPartEndInVoiceClone (
  int inputLineNumber) // may be "1, 2"
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Handling a repeat common part end in voice clone \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatCommonPartEndInVoiceClone() 1");
  }
#endif

  gIndenter++;

  if (! fVoicePendingRepeatDescrsStack.size ()) {
    stringstream s;

    s <<
      "repeats stack is empty when attempting to handle repeat ending '" <<
 //     repeatEnding->asShortString () <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gOahOah->fInputSourceName,
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
    kVoiceRepeatPhaseAfterCommonPart);

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatCommonPartEndInVoiceClone() 3");
  }
#endif

  gIndenter--;
}

void msrVoice::handleHookedRepeatEndingEndInVoiceClone (
  int    inputLineNumber,
  string repeatEndingNumber) // may be "1, 2"
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Handling a hooked repeat ending in voice clone \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHookedRepeatEndingEndInVoiceClone() 1");
  }
#endif

  gIndenter++;

  if (! fVoicePendingRepeatDescrsStack.size ()) {
    stringstream s;

    s <<
      "repeats stack is empty when attempting to handle hooked repeat ending '" <<
 //     repeatEnding->asShortString () <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gOahOah->fInputSourceName,
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
    kVoiceRepeatPhaseAfterHookedEnding);

  // move the voice last segment to repeatEnding
  moveVoiceLastSegmentToRepeatEnding (
    inputLineNumber,
    repeatEnding,
    "handleHookedRepeatEndingEndInVoiceClone()");

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHookedRepeatEndingEndInVoiceClone() 3");
  }
#endif

  gIndenter--;
}

void msrVoice::handleHooklessRepeatEndingEndInVoiceClone (
  int    inputLineNumber,
  string repeatEndingNumber) // may be "1, 2"
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Handling a hookless repeat ending in voice clone \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHooklessRepeatEndingEndInVoiceClone() 1");
  }
#endif

  gIndenter++;

  if (! fVoicePendingRepeatDescrsStack.size ()) {
    stringstream s;

    s <<
      "repeats stack is empty when attempting to handle hookless repeat ending '" <<
 //     repeatEnding->asShortString () <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gOahOah->fInputSourceName,
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
    kVoiceRepeatPhaseAfterHooklessEnding);

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHooklessRepeatEndingEndInVoiceClone() 3");
  }
#endif

  gIndenter--;
}

void msrVoice::handleRepeatEndingEndInVoiceClone (
  int       inputLineNumber,
  string    repeatEndingNumber, // may be "1, 2"
  msrRepeatEnding::msrRepeatEndingKind
            repeatEndingKind)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "appendRepeatEndingToVoiceClone() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Handling repeat start in voice clone \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatStartInVoiceClone() 1");
  }
#endif

  gIndenter++;

  switch (fVoiceKind) {
    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
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
          finalizeCurrentMeasureInVoice (
            inputLineNumber);

          // move current last segment to the list of initial elements
          moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
            inputLineNumber,
            "handleRepeatStartInVoiceClone() 2");

#ifdef TRACE_OAH
          if (gTraceOah->fTraceRepeats) {
            displayVoiceRepeatsStackSummary (
              inputLineNumber,
              "handleRepeatStartInVoiceClone() 3");
          }
#endif
        }
      }

      // create the repeat clone and stack it
#ifdef TRACE_OAH
      if (gTraceOah->fTraceRepeats) {
        gLogOstream <<
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

#ifdef TRACE_OAH
      if (gTraceOah->fTraceRepeats) {
        displayVoiceRepeatsStackSummary (
          inputLineNumber,
          "handleRepeatStartInVoiceClone() 5");
      }
#endif
      break;
  } // switch

  gIndenter--;
}

void msrVoice::handleRepeatEndInVoiceClone (
  int inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Handling a repeat end in voice clone \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  switch (fVoiceKind) {
    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
      {
        // finalize current measure in voice
        finalizeCurrentMeasureInVoice (
          inputLineNumber);

#ifdef TRACE_OAH
        if (gTraceOah->fTraceRepeats) {
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
                gOahOah->fInputSourceName,
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


#ifdef TRACE_OAH
        if (gTraceOah->fTraceRepeats) {
          displayVoiceRepeatsStackSummary (
            inputLineNumber,
            "handleRepeatEndInVoiceClone() 6");
        }
#endif
      }
      break;
  } // switch

  gIndenter--;
}

void msrVoice::appendMeasuresRepeatReplicaToVoice (
  int inputLineNumber)
{
  switch (fVoiceKind) {
    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
      {
#ifdef TRACE_OAH
        if (gTraceOah->fTraceMeasuresRepeats) {
          gLogOstream <<
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

#ifdef TRACE_OAH
        if (gTraceOah->fTraceMeasuresRepeats) { // JMI
          gLogOstream <<
            endl <<
            "***********" <<
            endl <<
            endl;
          print (gLogOstream);
          gLogOstream <<
            "***********" <<
            endl <<
            endl;
        }
#endif

        // sanity check
        msrAssert (
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
#ifdef TRACE_OAH
        if (gTraceOah->fTraceMeasuresRepeats) { // JMI
          gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    gLogOstream <<
      "Appending rest measures '" <<
      restMeasures->asString () <<
      "' to voice \"" <<
      fVoiceName <<
      "\"," <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    restMeasures != nullptr,
    "restMeasures is null");

  fInitialVoiceElementsList.push_back (restMeasures);
}

void msrVoice::appendMeasuresRepeatToVoiceElementsList (
  S_msrMeasuresRepeat measuresRepeat)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
      "Appending measures repeat '" <<
      measuresRepeat->asString () <<
      "' to voice \"" <<
      fVoiceName <<
      "\"," <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    measuresRepeat != nullptr,
    "measuresRepeat is null");

  fInitialVoiceElementsList.push_back (measuresRepeat);
}

void msrVoice:: appendRepeatEndingCloneToVoice ( // JMI
  S_msrRepeatEnding repeatEndingClone)
{
#ifdef TRACE_OAH
  int inputLineNumber =
    repeatEndingClone->getInputLineNumber (); // JMI
#endif

  gIndenter++;

  switch (fVoiceKind) {
    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
      {
        // add the repeat ending it to the voice current repeat
#ifdef TRACE_OAH
        if (gTraceOah->fTraceRepeats) {
          gLogOstream <<
            "Appending a " <<
            msrRepeatEnding::repeatEndingKindAsString (
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
            "repeats stack is empty when attempting to append a "<<
            msrRepeatEnding::repeatEndingKindAsString (
              repeatEndingClone->getRepeatEndingKind ()) <<
            " repeat ending to voice '" <<
            asShortString () <<
            "' ";

          msrInternalError (
            gOahOah->fInputSourceName,
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

#ifdef TRACE_OAH
        if (gTraceOah->fTraceRepeats) {
          displayVoiceRepeatsStackSummary (
            inputLineNumber,
            "appendRepeatEndingCloneToVoice() 2");
        }
#endif
      }
      break;
  } // switch

  gIndenter--;
}

void msrVoice::prependBarlineToVoice (S_msrBarline barline)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceBarlines) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceBarlines) {
    gLogOstream <<
      "Appending barline " <<
      barline->asString () <<
      " to voice \"" << getVoiceName () << "\"" <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceSegnos) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceCodas) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceEyeGlasses) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTracePedals) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceDamps) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceDampAlls) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceChords) {
    gLogOstream <<
      "Removing note '" <<
      note->asShortString () <<
      "' from voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  gIndenter++;

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

  gIndenter--;
}

void msrVoice::removeElementFromVoice (
  int          inputLineNumber,
  S_msrElement element)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
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
        inputLineNumber,
        "removeLastMeasureFromVoice()");

  gIndenter--;

  // return it
  return result;
}

void msrVoice::finalizeCurrentMeasureInVoice (
  int inputLineNumber)
{
  msrMeasure::msrMeasuresRepeatContextKind
    measuresRepeatContextKind =
      msrMeasure::kMeasuresRepeatContextKindNone;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
      "Finalizing current measure in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (gTraceOah->fTraceMeasuresDetails) {
    displayVoice (
      inputLineNumber,
      "finalizeCurrentMeasureInVoice() 1");
  }
#endif

  gIndenter++;

  // sanity check
  msrAssert (
    fVoiceCurrentMeasure != nullptr,
    "fVoiceCurrentMeasure is null");

  // finalize fVoiceCurrentMeasure
  fVoiceCurrentMeasure->
    finalizeMeasure (
      inputLineNumber,
      measuresRepeatContextKind,
      "finalizeCurrentMeasureInVoice() 2");


  switch (fVoiceKind) {
    case msrVoice::kVoiceRegular:
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

      // handle the harmony voice if any
      if (fHarmonyVoiceForRegularVoiceForwardLink) {
        fHarmonyVoiceForRegularVoiceForwardLink->
          finalizeCurrentMeasureInVoice (
            inputLineNumber);
      }

      // handle the figuredBass voice if any
      if (fFiguredBassVoiceForRegularVoiceForwardLink) {
        fFiguredBassVoiceForRegularVoiceForwardLink->
          finalizeCurrentMeasureInVoice (
            inputLineNumber);
      }
      break;

    case msrVoice::kVoiceHarmony:
    case msrVoice::kVoiceFiguredBass:
      break;
  } // switch

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresDetails) {
    displayVoice (
      inputLineNumber,
      "finalizeCurrentMeasureInVoice() 3");
  }
#endif

  gIndenter--;
}

void msrVoice:: collectVoiceMeasuresIntoFlatList (
  int inputLineNumber)
{
  // collect measures from the initial elements if any
  if (fInitialVoiceElementsList.size ()) {
#ifdef TRACE_OAH
    if (gTraceOah->fTraceVoices) {
      gLogOstream <<
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
#ifdef TRACE_OAH
    if (gTraceOah->fTraceVoices) {
      gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
      "Finalizing voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }

  if (gTraceOah->fTraceVoicesDetails) {
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
      gOahOah->fInputSourceName,
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
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
      gOahOah->fInputSourceName,
      inputLineNumber,
      s.str ());
  }

  // are there pending repeats in the voice repeats stack???
  int voicePendingRepeatDescrsStackSize =
    fVoicePendingRepeatDescrsStack.size ();

  if (voicePendingRepeatDescrsStackSize) {
#ifdef TRACE_OAH
    if (gTraceOah->fTraceMeasures) {
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
      gOahOah->fInputSourceName,
      fInputLineNumber,
      s.str ());
  }

  // collect the voice measures into the flat list
  collectVoiceMeasuresIntoFlatList (
    inputLineNumber);

  fVoiceHasBeenFinalized = true;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
      "Finalizing voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;

    displayVoiceRepeatsStackRestMeasuresMeasuresRepeatAndVoice (
      inputLineNumber,
      "finalizeVoice() 3");
  }
#endif

/* JMI
  // finalize the harmony voice if any
  if (fHarmonyVoiceForRegularVoiceForwardLink) {
    fHarmonyVoiceForRegularVoiceForwardLink->finalizeVoice (
      inputLineNumber);
  }

  // finalize the figured bass voice if any
  if (fFiguredBassVoiceForRegularVoiceForwardLink) {
    fFiguredBassVoiceForRegularVoiceForwardLink->finalizeVoice (
      inputLineNumber);
  }
  */
}

void msrVoice::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrVoice::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrVoice::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrVoice::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrVoice::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrVoice::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrVoice::browseData (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
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

  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% <== msrVoice::browseData ()" <<
      endl;
  }
}

string msrVoice::voiceKindAsString (
  msrVoiceKind voiceKind)
{
  string result;

  switch (voiceKind) {
    case msrVoice::kVoiceRegular:
      result = "voiceRegular";
      break;
    case msrVoice::kVoiceHarmony:
      result = "voiceHarmony";
      break;
    case msrVoice::kVoiceFiguredBass:
      result = "voiceFiguredBass";
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

string msrVoice::voiceRepeatPhaseKindAsString (
  msrVoiceRepeatPhaseKind
    afterRepeatComponentPhaseKind)
{
  string result;

  switch (afterRepeatComponentPhaseKind) {
    case msrVoice::kVoiceRepeatPhaseNone:
      result = "voiceRepeatPhaseNone";
      break;
    case msrVoice::kVoiceRepeatPhaseAfterCommonPart:
      result = "voiceRepeatPhaseAfterCommonPart";
      break;
    case msrVoice::kVoiceRepeatPhaseAfterHookedEnding:
      result = "voiceRepeatPhaseAfterHookedEnding";
      break;
    case msrVoice::kVoiceRepeatPhaseAfterHooklessEnding:
      result = "voiceRepeatPhaseAfterHooklessEnding";
      break;
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

void msrVoice::displayVoice (
  int    inputLineNumber,
  string context)
{
  gLogOstream <<
    endl <<
    "*********>> Displaying voice \"" <<
    getVoiceName () <<
    "\" (" << context << ")" <<
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

  const int fieldWidth = 41;

  os << left <<
    setw (fieldWidth) <<
    "staffUpLink" << " : " <<
    fVoiceStaffUpLink->getStaffName () <<
    endl <<

    setw (fieldWidth) <<
    "voiceNumber" << " : " <<
    voiceNumberAsString () <<
    endl <<

    setw (fieldWidth) <<
    "voiceCurrentMeasurePuristNumber" << " : " <<
    fVoiceCurrentMeasurePuristNumber <<
    endl <<

    setw (fieldWidth) <<
    "regularVoiceStaffSequentialNumber" << " : " <<
    regularVoiceStaffSequentialNumberAsString () <<
    endl;

#ifdef TRACE_OAH
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
      fVoiceCurrentTime;
  }
  else {
    os << "null" << endl;
  }
#endif

  // print the harmony voice name if any
  os << left <<
    setw (fieldWidth) << "harmonyVoiceForRegularVoice" << " : ";
  if (fHarmonyVoiceForRegularVoiceForwardLink) {
    os <<
      fHarmonyVoiceForRegularVoiceForwardLink->getVoiceName ();
  }
  else {
    os <<
      "none";
  }
  os << endl;

  // print the figured bass voice name if any
  os << left <<
    setw (fieldWidth) << "figuredBassVoiceForRegularVoice" << " : ";
  if (fFiguredBassVoiceForRegularVoiceForwardLink) {
    os <<
      fFiguredBassVoiceForRegularVoiceForwardLink->getVoiceName ();
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
    fVoiceShortestNoteTupletFactor <<
    endl;

  os << left <<
    setw (fieldWidth) << "voiceHasBeenFinalized" << " : " <<
    booleanAsString (fVoiceHasBeenFinalized) <<
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

  // print the voice current measure if any
  os <<
    setw (fieldWidth) << "voiceCurrentMeasure" << " : ";
  if (fVoiceCurrentMeasure) {
    os <<
      "'" <<
      fVoiceCurrentMeasure->asShortString () <<
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

#ifdef TRACE_OAH
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
  os << endl;

  if (voiceMeasuresFlatListSize) {
    gIndenter++;

    list<S_msrMeasure>::const_iterator
      iBegin = fVoiceMeasuresFlatList.begin (),
      iEnd   = fVoiceMeasuresFlatList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the measure
      if (gTraceOah->fTraceMeasures) {
        os << (*i)->asShortString ();
      }
      else {
        os << (*i)->getMeasureNumber ();
      }
      if (++i == iEnd) break;
      os << ' ';
    } // for
    os << endl;

    gIndenter--;
  }
#endif

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
  os << endl;

  if (voiceInitialElementsListSize) {
    os << endl;

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

        /* JMI
      switch (lastMeasureInLastSegment->getMeasureKind ()) {
        case msrMeasure::kMeasureKindMusicallyEmpty:
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
#ifdef TRACE_OAH
            if (gTraceOah->fTraceSegments) {
              gLogOstream <<
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
#ifdef TRACE_OAH
          if (gTraceOah->fTraceSegments) {
            gLogOstream <<
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
            lastMeasureInLastSegment->getMeasureNumber (),
            msrMeasure::kMeasureImplicitKindNo);
            */
