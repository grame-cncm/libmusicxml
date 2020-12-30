/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "msrStaves.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "msrOah.h"

#include "mxmlTree2msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
int msrStaff::gStaffMaxRegularVoices = 4;

S_msrStaff msrStaff::create (
  int          inputLineNumber,
  msrStaffKind staffKind,
  int          staffNumber,
  S_msrPart    staffPartUpLink)
{
  msrStaff* o =
    new msrStaff (
      inputLineNumber,
      staffKind,
      staffNumber,
      staffPartUpLink);
  assert (o!=0);

  return o;
}

msrStaff::msrStaff (
  int          inputLineNumber,
  msrStaffKind staffKind,
  int          staffNumber,
  S_msrPart    staffPartUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msgAssert(
    staffPartUpLink != nullptr,
    "staffPartUpLink is null");

  // set staff part upLink
  fStaffPartUpLink =
    staffPartUpLink;

  // set staff kind and number
  fStaffKind   = staffKind;
  fStaffNumber = staffNumber;

  // do other initializations
  initializeStaff ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Creating staff \"" << asString () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif
}

void msrStaff::initializeStaff ()
{
  fStaffRegularVoicesCounter = 0;

  // set staff name
  switch (fStaffKind) {
    case kStaffRegular:
      fStaffName =
        fStaffPartUpLink->getPartMsrName () +
        "_Staff_" +
        int2EnglishWord (fStaffNumber);
      break;

    case kStaffTablature:
        fStaffPartUpLink->getPartMsrName () +
        "_Tablature_" +
        int2EnglishWord (fStaffNumber);
      break;

    case kStaffHarmony:
      fStaffName =
        fStaffPartUpLink->getPartMsrName () +
        "_HARMONY_Staff";
      break;

    case kStaffFiguredBass:
      fStaffName =
        fStaffPartUpLink->getPartMsrName () +
        "_FIGURED_BASS_Staff";
      break;

    case kStaffDrum:
      fStaffName =
        fStaffPartUpLink->getPartMsrName () +
        "_DRUM_Staff";
      break;

    case kStaffRythmic:
      fStaffName =
        fStaffPartUpLink->getPartMsrName () +
        "_RYTHMIC_Staff";
      break;
  } // switch

  gIndenter++;

  // check the staff number
  switch (fStaffKind) {
    case kStaffRegular:
      // the staff number should not be negative
      if (fStaffNumber < 0) {
        stringstream s;

        s <<
          "regular staff number " << fStaffNumber <<
          " is not positive";

        msgAssert (
          false,
          s.str ());
      }
      break;

    case kStaffTablature:
      break;

    case kStaffHarmony:
      break;

    case kStaffFiguredBass:
    /* JMI
      if (fStaffNumber != K_PART_FIGURED_BASS_STAFF_NUMBER) {
        stringstream s;

        s <<
          "figured bass staff number " << fStaffNumber <<
          " is not equal to " << K_PART_FIGURED_BASS_STAFF_NUMBER;

        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      */
      break;

    case kStaffDrum:
      break;

    case kStaffRythmic:
      break;
  } // switch

  // get the initial staff details from the part if any
  S_msrStaffDetails
    partStaffDetails =
      fStaffPartUpLink->
        getCurrentPartStaffDetails ();

  if (partStaffDetails) {
    // append it to the staff
    appendStaffDetailsToStaff (partStaffDetails);
  }

  // get the initial clef from the part if any
  {
    S_msrClef
      clef =
        fStaffPartUpLink->
          getPartCurrentClef ();

    if (clef) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceClefs ()) {
        gLogStream <<
          "Appending part clef '" << clef->asString () <<
          "' as initial clef to staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUpLink->getPartCombinedName () <<
          endl;
      }
#endif

      appendClefToStaff (clef); // JMI
    }
  }

  // get the initial key from the part if any
  {
    //* JMI
    S_msrKey
      key =
        fStaffPartUpLink->
          getPartCurrentKey ();

    if (key) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceKeys ()) {
        gLogStream <<
          "Appending part key '" << key->asString () <<
          "' as initial key to staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUpLink->getPartCombinedName () <<
          endl;
      }
#endif

      appendKeyToStaff (key);
    }
  }

  // get the initial transpose from the part if any
  {
    S_msrTranspose
      transpose =
        fStaffPartUpLink->
          getPartCurrentTranspose ();

    if (transpose) {
#ifdef TRACING_IS_ENABLED
      if ( gGlobalTraceOahGroup->getTraceTranspositions ()) {
        gLogStream <<
          "Appending part transpose '" << transpose->asString () <<
          "' as initial transpose to staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUpLink->getPartCombinedName () <<
          endl;
      }
#endif

      fStaffCurrentTranspose = transpose;

      appendTransposeToAllStaffVoices (transpose);
    }
  }

  // set staff instrument names default values // JMI
  fStaffInstrumentName =
    fStaffPartUpLink->
      getPartInstrumentName ();
  fStaffInstrumentAbbreviation =
    fStaffPartUpLink->
      getPartInstrumentAbbreviation ();

  // rest measures
  fStaffContainsRestMeasures = false;

  gIndenter--;
}

msrStaff::~msrStaff ()
{}

S_msrStaff msrStaff::createStaffNewbornClone (
  S_msrPart containingPart)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Creating a newborn clone of staff \"" <<
      fStaffName <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msgAssert(
    containingPart != nullptr,
    "containingPart is null");

  S_msrStaff
    newbornClone =
      msrStaff::create (
        fInputLineNumber,
        fStaffKind,
        fStaffNumber,
        containingPart);

  newbornClone->fStaffName =
    fStaffName;

  newbornClone->fStaffNumber =
    fStaffNumber;

  newbornClone->fStaffInstrumentName =
    fStaffInstrumentName;

  newbornClone->fStaffInstrumentAbbreviation =
    fStaffInstrumentAbbreviation;

  return newbornClone;
}

S_msrPartGroup msrStaff::fetchStaffPartGroupUpLink () const
{
  return
    fStaffPartUpLink->
      getPartPartGroupUpLink ();
}

S_msrScore msrStaff::fetchStaffScoreUpLink () const
{
  return
    fStaffPartUpLink->
      fetchPartScoreUpLink ();
}

void msrStaff::setStaffCurrentClef (S_msrClef clef)
{
  fStaffCurrentClef = clef;
};

void msrStaff::setStaffCurrentKey (S_msrKey key)
{
  fStaffCurrentKey = key;
};

void msrStaff::setStaffCurrentTime (S_msrTime time)
{
  fStaffCurrentTime = time;
};

string msrStaff::staffNumberAsString () const
{
  string result = to_string (fStaffNumber);

  switch (fStaffNumber) {
    case K_PART_HARMONY_STAFF_NUMBER:
      result += " (K_PART_HARMONY_STAFF_NUMBER)"; // JMI
      break;
    case K_PART_FIGURED_BASS_STAFF_NUMBER:
      result += " (K_PART_FIGURED_BASS_STAFF_NUMBER)"; // JMI
      break;
    default:
      ;
  } // switch

  return result;
}

/* KEEP JMI
const int msrStaff::getStaffNumberOfMusicVoices () const
{
  int result = 0;

  for (
    map<int, S_msrVoice>::const_iterator i =
      fStaffVoiceNumberToVoicesMap.begin ();
    i != fStaffVoiceNumberToVoicesMap.end ();
    i++
  ) {
      S_msrVoice
        voice =
          (*i).second;

      switch (voice->getVoiceKind ()) {
        case kVoiceRegular:
          if (voice->getMusicHasBeenInsertedInVoice ()) {
            result++;
          }
          break;

        case kVoiceHarmony: // JMI
          break;

        case kVoiceFiguredBass: // JMI
          break;
      } // switch

  } // for

  return result;
}
*/

void msrStaff::createMeasureAndAppendItToStaff (
  int    inputLineNumber,
  string measureNumber,
  msrMeasureImplicitKind
         measureImplicitKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating measure '" <<
      measureNumber <<
      "' and appending it to staff \"" << getStaffName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  // propagate it to all voices
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    S_msrVoice
      voice = (*i).second;

    // sanity check
    msgAssert (
      voice != nullptr,
      "voice is null");

    switch (voice->getVoiceKind ()) {
      case kVoiceRegular:
      case kVoiceHarmony:
      case kVoiceFiguredBass:
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceMeasures ()) {
          gLogStream <<
            "Propagating the creation and appending of measure '" <<
            measureNumber <<
            "', line " << inputLineNumber <<
            ", to voice \"" << voice->getVoiceName () << "\"" <<
            endl;
        }
#endif

        voice->
          createMeasureAndAppendItToVoice (
            inputLineNumber,
            measureNumber,
            measureImplicitKind);
        break;
/* JMI
      case kVoiceHarmony:
        break;

      case kVoiceFiguredBass:
        break;
        */
    } // switch
  } // for
}

void msrStaff::setNextMeasureNumberInStaff (
  int    inputLineNumber,
  string nextMeasureNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Setting next measure number to '" <<
      nextMeasureNumber <<
      ", in staff \"" << getStaffName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  // propagate it to all voices
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    S_msrVoice voice = (*i).second;

    // sanity check
    msgAssert (
      voice != nullptr,
      "voice is null");

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "Propagating the setting of next measure number '" <<
        nextMeasureNumber <<
        "', line " << inputLineNumber <<
        ", in voice \"" << voice->getVoiceName () << "\"" <<
        endl;
    }
#endif

    voice->
      setNextMeasureNumberInVoice (
        inputLineNumber,
        nextMeasureNumber);
  } // for

  gIndenter--;
}

S_msrVoice msrStaff::createVoiceInStaffByItsNumber (
  int                    inputLineNumber,
  msrVoiceKind voiceKind,
  int                    voiceNumber,
  string                 currentMeasureNumber)
{
  // take this new voice into account if relevant
  switch (voiceKind) {
    case kVoiceRegular:
      fStaffRegularVoicesCounter++;

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceVoices ()) {
        gLogStream <<
          "Creating regular voice number '" <<
          voiceNumber <<
          "', voiceKind '" <<
          voiceKindAsString (voiceKind) <<
          "' as regular voice number '" <<
          fStaffRegularVoicesCounter <<
          "' of staff \"" << getStaffName () <<
          "\", line " << inputLineNumber <<
          "\", current measure number: " <<
          currentMeasureNumber <<
     // JMI     " in part " << fStaffPartUpLink->getPartCombinedName () <<
          endl;
      }
#endif
      break;

    case kVoiceHarmony:
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceVoices ()) {
        gLogStream <<
          "Creating harmony voice number '" <<
          voiceNumber <<
          "', voiceKind '" <<
          voiceKindAsString (voiceKind) <<
          "' in staff \"" << getStaffName () <<
          "\", line " << inputLineNumber <<
          "\", current measure number: " <<
          currentMeasureNumber <<
     // JMI     " in part " << fStaffPartUpLink->getPartCombinedName () <<
          endl;
      }
#endif
      break;

    case kVoiceFiguredBass:
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceVoices ()) {
        gLogStream <<
          "Creating figured bass voice number '" <<
          voiceNumber <<
          "', voiceKind '" <<
          voiceKindAsString (voiceKind) <<
          "' in staff \"" << getStaffName () <<
          "\", line " << inputLineNumber <<
          "\", current measure number: " <<
          currentMeasureNumber <<
     // JMI     " in part " << fStaffPartUpLink->getPartCombinedName () <<
          endl;
      }
#endif
      break;
  } // switch


  // are there too many regular voices in this staff?
  if (fStaffRegularVoicesCounter > msrStaff::gStaffMaxRegularVoices) {
    stringstream s;

    s <<
      "staff \"" << getStaffName () <<
      "\" is already filled up with " <<
      msrStaff::gStaffMaxRegularVoices << " regular voices" <<
      endl <<
      ". voice number " <<
      voiceNumber <<
      " overflows it" <<
      endl <<
      ", fStaffRegularVoicesCounter = " <<
      fStaffRegularVoicesCounter <<
      ", msrStaff::gStaffMaxRegularVoices = " <<
      msrStaff::gStaffMaxRegularVoices <<
      endl;

      /* JMI ???
    msrMusicXMLError (
// JMI    msrMusicXMLWarning ( JMI
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
      */
  }

  // is this voice number in the regular voices map?
  map<int, S_msrVoice>::const_iterator
    it =
      fStaffVoiceNumberToVoicesMap.find (voiceNumber);

  if (it != fStaffVoiceNumberToVoicesMap.end ()) {
    // yes
    S_msrVoice
      olderVoice = (*it).second;

// JMI    this->print (gLogStream); // JMI

    stringstream s;

    s <<
      "attempting to create a voice anew by number" <<
      ", voiceNumber: " << voiceNumber <<
      ", currentMeasureNumber: " << currentMeasureNumber <<
      " ' in staff \"" <<
      fStaffName <<
      "\", " <<
      olderVoice->asShortString () <<
      " already exists with this same voiceNumber" <<
      ", line " << inputLineNumber;

    msrInternalError ( // JMI ???
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create the voice
  S_msrVoice
    voice =
      msrVoice::create (
        inputLineNumber,
        voiceKind,
        voiceNumber,
        msrVoice::kCreateInitialLastSegmentYes,
        this);

  // take this new voice into account if relevant
  switch (voiceKind) {
    case kVoiceRegular:
      // register the voice by its number
      registerRegularVoiceByItsNumber (
        voiceNumber,
        voice);
      break;

    case kVoiceHarmony:
    case kVoiceFiguredBass:
      // register it in staff by its number
      registerPartLevelVoiceInStaff (
        inputLineNumber,
        voice);
      break;
  } // switch

  return voice;
}

void msrStaff::registerVoiceInStaffAllVoicesList (
  S_msrVoice voice)
{
  // register voice in this staff
  fStaffAllVoicesList.push_back (voice);

  // register it in the part uplink
  fStaffPartUpLink->
    registerVoiceInPartAllVoicesList (voice);
}

void msrStaff::registerVoiceByItsNumber (
  int        inputLineNumber,
  int        voiceNumber,
  S_msrVoice voice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Registering voice number '" << voiceNumber <<
      "', named \"" << voice->getVoiceName () <<
      "\" in staff " << getStaffName () <<
      endl;
  }
#endif

  // register voice in all voices map
  fStaffVoiceNumberToAnyVoiceMap [voiceNumber] = voice;

  // register it in all voices list
  registerVoiceInStaffAllVoicesList (voice);

  // sort the list if necessary
  switch (voice->getVoiceKind ()) {
    case kVoiceRegular:
      break;

    case kVoiceHarmony:
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceVoices ()) {
        gLogStream <<
          "Sorting the voices in staff \"" <<
          getStaffName () << "\"" <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif

      // sort fStaffAllVoicesList, to have harmonies just before
      // the corresponding voice
      if (fStaffAllVoicesList.size ()) {
        fStaffAllVoicesList.sort (
          compareVoicesToHaveHarmoniesAboveCorrespondingVoice);
      }
      break;

    case kVoiceFiguredBass:
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceVoices ()) {
        gLogStream <<
          "Sorting the voices in staff \"" <<
          getStaffName () << "\"" <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif

      // sort fStaffAllVoicesList, to have figured basses just after
      // the corresponding voice
      if (fStaffAllVoicesList.size ()) {
        fStaffAllVoicesList.sort (
          compareVoicesToHaveFiguredBassesBelowCorrespondingVoice);
      }
      break;
  } // switch
}

void msrStaff::registerRegularVoiceByItsNumber (
  int        voiceNumber,
  S_msrVoice regularVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Registering regular voice " <<
      regularVoice->asShortString () <<
      " by it's number '" << voiceNumber <<
      "\" in staff " << getStaffName () <<
      "'s regular voices list with sequential number '" <<
      fStaffRegularVoicesCounter <<
      "'" <<
      endl;
  }
#endif

  // register voice in the 'Any' voices map
  fStaffVoiceNumberToAnyVoiceMap [voiceNumber] = regularVoice;

  // register voice in the all voices map
  fStaffVoiceNumberToVoicesMap [voiceNumber] = regularVoice;

  // register it in all regular voices list
  fStaffRegularVoicesList.push_back (regularVoice);

  // register it in all voices list
  registerVoiceInStaffAllVoicesList (regularVoice);

  // set regularVoice staff sequential number
  regularVoice->
    setRegularVoiceStaffSequentialNumber (
      fStaffRegularVoicesCounter);
}

/* JMI
void msrStaff::registerVoiceInAllVoicesList (
  int        voiceNumber,
  S_msrVoice voice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Registering voice number '" << voiceNumber <<
      "', named \"" << voice->getVoiceName () <<
      "\" in staff " << getStaffName () <<
      "'s all voices list" <<
      endl;
  }
#endif

  registerVoiceInStaffAllVoicesList (voice);
}
*/

S_msrVoice msrStaff::fetchVoiceFromStaffByItsNumber (
  int inputLineNumber,
  int voiceNumber)
{
  S_msrVoice result; // JMI avoid repetitive messages!

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Fetching voice number '" <<
      voiceNumber <<
     "' in staff \"" << getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i =
      fStaffVoiceNumberToVoicesMap.begin ();
    i != fStaffVoiceNumberToVoicesMap.end ();
    i++
  ) {
    int        number = (*i).first;
    S_msrVoice voice  = (*i).second;

    if (
      voice->getVoiceNumber ()
        ==
      voiceNumber
    ) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceVoices ()) {
        gLogStream <<
          "Voice number '" << voiceNumber <<
          "' in staff \"" << getStaffName () << "\"" <<
          " has staff relative number '" << number <<
          "'" <<
          endl;
      }
#endif

      result = voice;
      break;
    }
  } // for

  return result;
}

void msrStaff::assignSequentialNumbersToRegularVoicesInStaff (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Assigning sequential numbers to the regular voices in staff \"" <<
      fStaffName <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // assign sequential numbers to the regular voices,
  // needed to know about voices 1, 2, 3 and 4
  fStaffRegularVoicesList.sort (
    compareVoicesByIncreasingNumber);

  if (fStaffRegularVoicesList.size ()) {
    int voiceSequentialCounter = 0;

    list<S_msrVoice>::const_iterator
      iBegin = fStaffRegularVoicesList.begin (),
      iEnd   = fStaffRegularVoicesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      if (i == iEnd) break;

      S_msrVoice voice = (*i);

      // sanity check
      msgAssert (
        voice != nullptr,
        "voice is null");

      voiceSequentialCounter++;

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceVoices ()) {
        gLogStream <<
          "Voice \"" <<
          voice->getVoiceName () <<
          "\" gets sequential number " <<
          voiceSequentialCounter <<
          " in staff \"" <<
          fStaffName <<
          "\"" <<
          endl;
      }
#endif

      voice->
        setRegularVoiceStaffSequentialNumber (
          voiceSequentialCounter);

      if (++i == iEnd) break;
    } // for
  }
}

S_msrVoice msrStaff::fetchFirstRegularVoiceFromStaff (
  int inputLineNumber)
{
  S_msrVoice result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Fetching first regular voice in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (! fStaffRegularVoicesList.size ()) {
    stringstream s;

    s <<
      "staff " << fStaffNumber <<
      " in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      " doesn't contain any regular voices, cannot fetch the first one" <<
      ", line " << inputLineNumber;

    msrInternalError ( // JMI ???
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  else {
    result = fStaffRegularVoicesList.front ();

    // sanity check
    msgAssert (
      result->getRegularVoiceStaffSequentialNumber () == 1,
      "result->getRegularVoiceStaffSequentialNumber () is not equal to 1");

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceVoices ()) {
      gLogStream <<
        "The first regular voice in staff \"" <<
        getStaffName () <<
        "\" is " <<
        result->asShortString () <<
        endl;
    }
#endif
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "--> result = \"" <<
      result->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  return result;
}

/* JMI
void msrStaff::addAVoiceToStaffIfItHasNone (
  int inputLineNumber)
{
  if (fStaffVoiceNumberToAnyVoiceMap.size () == 0) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceVoices ()) {
      gLogStream <<
        "Staff \"" <<
        getStaffName () <<
        "\" doesn't contain any voice, adding one" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    this->
      createVoiceInStaffByItsNumber (
        inputLineNumber,
        kVoiceRegular,
        1,    // voiceNumber,
        "1"); // fCurrentMeasureNumber
  }
}
*/

void msrStaff::registerVoiceInStaff (
  int        inputLineNumber,
  S_msrVoice voice)
{
  // sanity check
  msgAssert (
    voice != nullptr,
    "voice is null");

  // get voice kind
  msrVoiceKind voiceKind =
    voice->getVoiceKind ();

  // take this new voice into account if relevant
  switch (voiceKind) {
    case kVoiceRegular:
      // take that regular voice into account
      fStaffRegularVoicesCounter++;

      // are there too many voices in this staff?
      if (fStaffRegularVoicesCounter > msrStaff::gStaffMaxRegularVoices) {
        stringstream s;

        s <<
          "staff \"" << getStaffName () <<
          "\" is already filled up with " <<
          msrStaff::gStaffMaxRegularVoices << " regular voices," <<
          endl <<
          "the voice named \"" << voice->getVoiceName () << "\" overflows it" <<
          endl <<
          ", fStaffRegularVoicesCounter = " <<
          fStaffRegularVoicesCounter <<
          ", msrStaff::gStaffMaxRegularVoices = " <<
          msrStaff::gStaffMaxRegularVoices <<
          endl;

          /* JMI ???
        msrMusicXMLError (
    // JMI    msrMusicXMLWarning ( JMI
          gGlobalOahOahGroup->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
          */
      }
      break;

    case kVoiceHarmony:
      break;
    case kVoiceFiguredBass:
      break;
  } // switch

  // register voice in this staff
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Registering voice \"" << voice->getVoiceName () <<
      "\" as relative voice " <<
      fStaffRegularVoicesCounter <<
      " of staff \"" << getStaffName () <<
      "\", line " << inputLineNumber <<
// JMI       " in part " << fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  // register it in staff by its number
  registerVoiceByItsNumber (
    inputLineNumber,
    voice->getVoiceNumber (),
    voice);

  // is voice a regular voice???
  switch (voiceKind) {
    case kVoiceRegular:
      {
        int voiceNumber = voice->getVoiceNumber ();

        registerRegularVoiceByItsNumber (
          voiceNumber,
          voice);
      }
      break;
    case kVoiceHarmony:
      break;

    case kVoiceFiguredBass:
      break;
  } // switch
}

void msrStaff::registerPartLevelVoiceInStaff (
  int        inputLineNumber,
  S_msrVoice voice)
{
  // sanity check
  msgAssert (
    voice != nullptr,
    "voice is null");

  // get voice kind
  msrVoiceKind voiceKind =
    voice->getVoiceKind ();

  // take this new voice into account if relevant
  switch (voiceKind) {
    case kVoiceRegular:
      break;

    case kVoiceHarmony:
    case kVoiceFiguredBass:
      // take that regular voice into account
      fStaffRegularVoicesCounter++;

      // are there too many voices in this staff?
      if (fStaffRegularVoicesCounter > msrStaff::gStaffMaxRegularVoices) {
        stringstream s;

        s <<
          "staff \"" << getStaffName () <<
          "\" is already filled up with " <<
          msrStaff::gStaffMaxRegularVoices << " regular voices," <<
          endl <<
          "the voice named \"" << voice->getVoiceName () << "\" overflows it" <<
          endl <<
          ", fStaffRegularVoicesCounter = " <<
          fStaffRegularVoicesCounter <<
          ", msrStaff::gStaffMaxRegularVoices = " <<
          msrStaff::gStaffMaxRegularVoices <<
          endl;

          /* JMI ???
        msrMusicXMLError (
    // JMI    msrMusicXMLWarning ( JMI
          gGlobalOahOahGroup->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
          */
      }
      break;
  } // switch

  // register voice in this staff
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Registering voice \"" << voice->getVoiceName () <<
      "\" as relative voice " <<
      fStaffRegularVoicesCounter <<
      " of staff \"" << getStaffName () <<
      "\", line " << inputLineNumber <<
// JMI       " in part " << fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  // register it in staff by its number
  registerVoiceByItsNumber (
    inputLineNumber,
    voice->getVoiceNumber (),
    voice);

  // is voice a regular voice???
  switch (voiceKind) {
    case kVoiceRegular:
      {
        int voiceNumber = voice->getVoiceNumber ();

        // register the voice by its number
        registerRegularVoiceByItsNumber (
          voiceNumber,
          voice);
      }
      break;

    case kVoiceHarmony:
      break;

    case kVoiceFiguredBass:
      break;
  } // switch
}

void msrStaff::registerVoiceInStaffClone (
  int        inputLineNumber,
  S_msrVoice voice)
{
  // sanity check
  msgAssert (
    voice != nullptr,
    "voice is null");

  // get voice kind
  msrVoiceKind voiceKind =
    voice->getVoiceKind ();

  // take this new voice into account if relevant
  switch (voiceKind) {
    case kVoiceRegular:
      // take that regular voice into account
      fStaffRegularVoicesCounter++;

      // are there too many voices in this staff?
      if (fStaffRegularVoicesCounter > msrStaff::gStaffMaxRegularVoices) {
        stringstream s;

        s <<
          "staff clone \"" << getStaffName () <<
          "\" is already filled up with " <<
          msrStaff::gStaffMaxRegularVoices << " regular voices," <<
          endl <<
          "the voice named \"" << voice->getVoiceName () << "\" overflows it" <<
          endl <<
          ", fStaffRegularVoicesCounter = " <<
          fStaffRegularVoicesCounter <<
          ", msrStaff::gStaffMaxRegularVoices = " <<
          msrStaff::gStaffMaxRegularVoices <<
          endl;

          /* JMI ???
        msrMusicXMLError (
    // JMI    msrMusicXMLWarning ( JMI
          gGlobalOahOahGroup->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
          */
      }
      break;

    case kVoiceHarmony:
      break;
    case kVoiceFiguredBass:
      break;
  } // switch

  // register voice in this staff
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Registering voice \"" << voice->getVoiceName () <<
      "\" as relative voice " <<
      fStaffRegularVoicesCounter <<
      " of staff clone \"" << getStaffName () <<
      "\", line " << inputLineNumber <<
// JMI       " in part " << fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  // register it in staff by its number
  registerVoiceByItsNumber (
    inputLineNumber,
    voice->getVoiceNumber (),
    voice);

  // is voice a regular voice???
  switch (voiceKind) {
    case kVoiceRegular:
      {
        int voiceNumber = voice->getVoiceNumber ();

        // register the voice by its number
        registerRegularVoiceByItsNumber (
          voiceNumber,
          voice);
      }
      break;

    case kVoiceHarmony:
      break;

    case kVoiceFiguredBass:
      break;
  } // switch
}

void msrStaff::appendClefToStaff (S_msrClef clef)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceClefs ()) {
    gLogStream <<
      "Appending clef '" << clef->asString () <<
      "' to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  gIndenter++;

  // append clef to the staff,
  // unless we should ignore redundant clefs
  // and a clef equal to the current clef is found
  bool doAppendClefToStaff = true;

  if (fStaffCurrentClef) {
    if (
      gGlobalMxmlTree2msrOahGroup->getIgnoreRedundantClefs ()
        &&
      clef->isEqualTo (fStaffCurrentClef)
    ) {
      doAppendClefToStaff = false;
    }
  }

  if (doAppendClefToStaff) {
    // register clef as current staff clef
    fStaffCurrentClef = clef;

    // propagate clef to all voices
    for (
      map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
      i != fStaffVoiceNumberToAnyVoiceMap.end ();
      i++
    ) {
      (*i).second-> // JMI msgAssert???
        appendClefToVoice (clef);
    } // for
  }

  else {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceClefs ()) {
      gLogStream <<
        "Clef '" <<
        clef->asString () <<
        "' ignored because it is already present in staff " <<
        getStaffName () <<
        "\" in part " <<
        fStaffPartUpLink->getPartCombinedName () <<
        endl;
    }
#endif
  }

  gIndenter--;
}

void msrStaff::appendKeyToStaff (S_msrKey  key)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Appending key " << key->asString () <<
      " to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  gIndenter++;

  // append key to staff?
  bool doAppendKeyToStaff = true;

  if (fStaffCurrentKey) {
    if (
      gGlobalMxmlTree2msrOahGroup->getIgnoreRedundantKeys ()
        &&
      fStaffCurrentKey->isEqualTo (key)
    ) {
      doAppendKeyToStaff = false;
    }

    else {
      if (key->isEqualTo (fStaffCurrentKey)) {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceKeys ()) {
          gLogStream <<
            "Key '" <<
            key->asString () <<
            "' ignored because it is already present in staff " <<
            getStaffName () <<
            "\" in part " <<
            fStaffPartUpLink->getPartCombinedName () <<
            endl;
        }
#endif

        doAppendKeyToStaff = false;
      }
    }
  }

  if (doAppendKeyToStaff) {
    // register key as current staff key
    fStaffCurrentKey = key;

    // propagate it to all voices
    for (
      map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
      i != fStaffVoiceNumberToAnyVoiceMap.end ();
      i++
    ) {
      (*i).second-> // JMI msgAssert???
        appendKeyToVoice (key);
    } // for
  }

  gIndenter--;
}

void msrStaff::appendTimeToStaff (S_msrTime time)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTimes ()) {
    gLogStream <<
      "Appending time '" << time->asString () <<
      "' to staff \"" <<
      getStaffName () <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;

  // append time to staff?
  bool doAppendTimeToStaff = true;

  if (fStaffCurrentTime) {
    if (
      gGlobalMxmlTree2msrOahGroup->getIgnoreRedundantTimes ()
        &&
      fStaffCurrentTime->isEqualTo (time)
    ) {
      doAppendTimeToStaff = false;
    }

    else {
      if (time->isEqualTo (fStaffCurrentTime)) {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceTimes ()) {
          gLogStream <<
            "Time '" <<
            time->asString () <<
            "' ignored because it is already present in staff " <<
            getStaffName () <<
            "\" in part " <<
            fStaffPartUpLink->getPartCombinedName () <<
            endl;
        }
#endif

        doAppendTimeToStaff = false;
      }
    }
  }

  if (doAppendTimeToStaff) {
    // register time as current staff time
    fStaffCurrentTime = time;

    // propagate it to all voices
    for (
      map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
      i != fStaffVoiceNumberToAnyVoiceMap.end ();
      i++
    ) {
      (*i).second-> // JMI msgAssert???
        appendTimeToVoice (time);
    } // for
  }

  gIndenter--;
}

void msrStaff::appendTimeToStaffClone (S_msrTime time)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTimes ()) {
    gLogStream <<
      "Appending time '" << time->asString () <<
      "' to staff clone \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  gIndenter++;

  // set staff time
  fStaffCurrentTime = time;

  // propagate it to all voices
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    (*i).second-> // JMI msgAssert???
      appendTimeToVoiceClone (time);
  } // for

  gIndenter--;
}

void msrStaff::insertHiddenMeasureAndBarlineInStaffClone (
  int      inputLineNumber,
  rational positionInMeasure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceDalSegnos () || gGlobalTraceOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Inserting hidden measure and barline at position " <<
      positionInMeasure <<
      "' in staff clone \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  // propagate it to all voices
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    (*i).second-> // JMI msgAssert???
      insertHiddenMeasureAndBarlineInVoiceClone (
        inputLineNumber,
        positionInMeasure);
  } // for

  gIndenter--;
}

/* JMI
void msrStaff::nestContentsIntoNewRepeatInStaff (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Nesting contents into new repeat in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    (*i).second->
      nestContentsIntoNewRepeatInVoice (
        inputLineNumber);
  } // for
}
*/

void msrStaff::handleRepeatStartInStaff (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling repeat start in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    (*i).second->
      handleRepeatStartInVoice (
        inputLineNumber);
  } // for

  gIndenter--;
}

void msrStaff::handleRepeatEndInStaff (
  int    inputLineNumber,
  string measureNumber,
  int    repeatTimes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat end in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    (*i).second->
      handleRepeatEndInVoice (
        inputLineNumber,
        measureNumber,
        repeatTimes);
  } // for

  gIndenter--;
}

void msrStaff::handleRepeatEndingStartInStaff (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat ending start in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    (*i).second->
      handleRepeatEndingStartInVoice (
        inputLineNumber);
  } // for

  gIndenter--;
}

void msrStaff::handleRepeatEndingEndInStaff (
  int       inputLineNumber,
  string    repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind
            repeatEndingKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a " <<
      repeatEndingKindAsString (
        repeatEndingKind) <<
      " repeat ending end in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    (*i).second->
      handleRepeatEndingEndInVoice (
        inputLineNumber,
        repeatEndingNumber,
        repeatEndingKind);
  } // for

  gIndenter--;
}

/* JMI
void msrStaff::finalizeRepeatEndInStaff (
  int    inputLineNumber,
  string measureNumber,
  int    repeatTimes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Finalizing a repeat upon its end in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    (*i).second->
      finalizeRepeatEndInVoice (
        inputLineNumber,
        measureNumber,
        repeatTimes);
  } // for

  gIndenter--;
}
*/

void msrStaff::createMeasuresRepeatFromItsFirstMeasuresInStaff (
  int inputLineNumber,
  int measuresRepeatMeasuresNumber,
  int measuresRepeatSlashes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a measures repeat from it's first measure in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    (*i).second->
      createMeasuresRepeatFromItsFirstMeasuresInVoice (
        inputLineNumber,
        measuresRepeatMeasuresNumber,
        measuresRepeatSlashes);
  } // for
}

void msrStaff::appendPendingMeasuresRepeatToStaff (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending the pending measures repeat to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    (*i).second->
      appendPendingMeasuresRepeatToVoice (
        inputLineNumber);
  } // for
}

void msrStaff::createRestMeasuresInStaff (
  int inputLineNumber,
  int restMeasuresMeasuresNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    gLogStream <<
      "Creating multiple rest measures in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", " <<
      singularOrPlural (
        restMeasuresMeasuresNumber, "measure", "measures") <<
      endl;
  }
#endif

  fStaffContainsRestMeasures = true;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    (*i).second->
      createRestMeasuresInVoice (
        inputLineNumber,
        restMeasuresMeasuresNumber);
  } // for
}

void msrStaff::addRestMeasuresToStaff (
  int    inputLineNumber,
  string previousMeasureNumber,
  int    restMeasuresNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    gLogStream <<
      "Adding multiple rest measures to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", " <<
      singularOrPlural (
        restMeasuresNumber, "measure", "measures") <<
      endl;
  }
#endif

  fStaffContainsRestMeasures = true;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    (*i).second->
      addRestMeasuresToVoice (
        inputLineNumber,
        previousMeasureNumber,
        restMeasuresNumber);
  } // for
}

void msrStaff::appendPendingRestMeasuresToStaff (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    gLogStream <<
      "Appending the pending multiple rest to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    (*i).second->
      appendPendingRestMeasuresToVoice (
        inputLineNumber);
  } // for
}

void msrStaff::appendRestMeasuresCloneToStaff (
  int               inputLineNumber,
  S_msrRestMeasures restMeasures)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestMeasures ()) {
    gLogStream <<
      "Appending multiple rest '" <<
      restMeasures->asString () <<
      "' to staff clone \"" <<
      getStaffName () <<
      "\"" <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    (*i).second-> // JMI msgAssert???
      appendRestMeasuresCloneToVoiceClone (
        inputLineNumber,
        restMeasures);
  } // for
}

void msrStaff::appendRepeatCloneToStaff (
  int         inputLineNumber,
  S_msrRepeat repeatCLone)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending repeat clone to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    (*i).second-> // JMI msgAssert???
      appendRepeatCloneToVoiceClone (
        inputLineNumber, repeatCLone);
  } // for
}

void msrStaff::appendRepeatEndingCloneToStaff (
  S_msrRepeatEnding repeatEndingClone)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending a repeat ending clone to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", line " << repeatEndingClone->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter++;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    (*i).second-> // JMI msgAssert???
      appendRepeatEndingCloneToVoice (
        repeatEndingClone);
  } // for

  gIndenter--;
}

void msrStaff::appendBarlineToStaff (S_msrBarline barline)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceBarlines ()) {
    gLogStream <<
      "Appending barline " << barline->asString () <<
      " to staff " <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  gIndenter++;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    (*i).second->
      appendBarlineToVoice (barline);
  } // for

  gIndenter--;
}

void msrStaff::appendTransposeToStaff (
  S_msrTranspose transpose)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTranspositions ()) {
    gLogStream <<
      "Setting transpose '" <<
      transpose->asString () <<
      "' in staff " <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  // set staff transpose
  bool doAppendTransposeToStaff = true;

  if (! fStaffCurrentTranspose) {
    doAppendTransposeToStaff = true;
  }

  else {
    if (transpose->isEqualTo (fStaffCurrentTranspose)) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceTranspositions ()) {
        gLogStream <<
          "Transpose '" <<
          transpose->asString () <<
          "' ignored because it is already present in staff " <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUpLink->getPartCombinedName () <<
          endl;
      }
#endif

      doAppendTransposeToStaff = false;
    }
  }

  if (doAppendTransposeToStaff) {
    // register transpose as current staff transpose
    fStaffCurrentTranspose = transpose;

    // propagate it to all voices
    appendTransposeToAllStaffVoices (transpose);
  }
}

void msrStaff::appendPartNameDisplayToStaff (
  S_msrPartNameDisplay partNameDisplay)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Setting part name display '" <<
      partNameDisplay->asString () <<
      "' in staff " <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  // set staff transpose
  bool doAppendPartNameDisplayToStaff = true;

  /* JMI ???
  if (! fStaffCurrentTranspose) {
    doAppendPartNameDisplayToStaff = true;
  }

  else {
    if (partNameDisplay->isEqualTo (fStaffCurrentTranspose)) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceTranspositions ()) {
        gLogStream <<
          "Transpose '" <<
          partNameDisplay->asString () <<
          "' ignored because it is already present in staff " <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUpLink->getPartCombinedName () <<
          endl;
      }
#endif

      doAppendPartNameDisplayToStaff = false;
    }
  }
  */

  if (doAppendPartNameDisplayToStaff) {
    // register transpose as current staff transpose
 // JMI   fStaffCurrentTranspose = partNameDisplay;

    // propagate it to all voices
    appendPartNameDisplayToAllStaffVoices (partNameDisplay);
  }
}

void msrStaff::appendPartAbbreviationDisplayToStaff (
  S_msrPartAbbreviationDisplay partAbbreviationDisplay)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Setting part abbreviation display '" <<
      partAbbreviationDisplay->asString () <<
      "' in staff " <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  // set staff transpose
  bool doAppendPartAbbreviationDisplayToStaff = true;

/* JMI ???
  if (! fStaffCurrentTranspose) {
    doAppendPartAbbreviationDisplayToStaff = true;
  }

  else {
    if (partAbbreviationDisplay->isEqualTo (fStaffCurrentTranspose)) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceTranspositions ()) {
        gLogStream <<
          "Transpose '" <<
          transpose->asString () <<
          "' ignored because it is already present in staff " <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUpLink->getPartCombinedName () <<
          endl;
      }
#endif

      doAppendPartAbbreviationDisplayToStaff = false;
    }
  }
  */

  if (doAppendPartAbbreviationDisplayToStaff) {
    // register transpose as current staff transpose
 // JMI   fStaffCurrentTranspose = partAbbreviationDisplay;

    // propagate it to all voices
    appendPartAbbreviationDisplayToAllStaffVoices (partAbbreviationDisplay);
  }
}

void msrStaff::appendStaffDetailsToStaff (
  S_msrStaffDetails staffDetails)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceStaffDetails ()) {
    gLogStream <<
      "Appending staff details '" <<
      staffDetails->asShortString () <<
      "' to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    staffDetails != nullptr,
    "staffDetails is null");

  // register staff details in staff
  fCurrentStaffStaffDetails = staffDetails;

  // set staff kind accordingly if relevant
  switch (staffDetails->getStaffLinesNumber ()) {
    case 1:
      if (gGlobalMsrOahGroup->getCreateSingleLineStavesAsRythmic ()) {
        fStaffKind = kStaffRythmic;
      }
      else {
        fStaffKind = kStaffDrum;
      }
      break;
    default:
      ;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Setting staff kind to '" <<
      staffKindAsString (fStaffKind) <<
      "' in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    (*i).second->
      appendStaffDetailsToVoice (staffDetails);
  } // for
}

void msrStaff::appendTransposeToAllStaffVoices (
  S_msrTranspose transpose)
{
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    (*i).second->
      appendTransposeToVoice (transpose);
  } // for
}

void msrStaff::appendPartNameDisplayToAllStaffVoices (
  S_msrPartNameDisplay partNameDisplay)
{
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    (*i).second->
      appendPartNameDisplayToVoice (partNameDisplay);
  } // for
}

void msrStaff::appendPartAbbreviationDisplayToAllStaffVoices (
  S_msrPartAbbreviationDisplay partAbbreviationDisplay)
{
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    (*i).second->
      appendPartAbbreviationDisplayToVoice (partAbbreviationDisplay);
  } // for
}

void msrStaff::appendScordaturaToStaff (
  S_msrScordatura scordatura)
{
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    (*i).second->
      appendScordaturaToVoice (
        scordatura);
  } // for
}

void msrStaff::appendAccordionRegistrationToStaff (
  S_msrAccordionRegistration
    accordionRegistration)
{
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    (*i).second->
      appendAccordionRegistrationToVoice (
        accordionRegistration);
  } // for
}

void msrStaff::appendHarpPedalsTuningToStaff (
  S_msrHarpPedalsTuning
    harpPedalsTuning)
{
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    (*i).second->
      appendHarpPedalsTuningToVoice (
        harpPedalsTuning);
  } // for
}

void msrStaff::finalizeLastAppendedMeasureInStaff (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Finalizing last appended measure in staff \"" <<
      getStaffName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  // finalize all the voices in the staff
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    S_msrVoice
      voice =
        (*i).second;

    switch (voice->getVoiceKind ()) {
      case kVoiceRegular:
      case kVoiceHarmony:
      case kVoiceFiguredBass:
        {
          // handle the regular voice
          voice->
            finalizeLastAppendedMeasureInVoice (
              inputLineNumber);

/* JMI this will be done later
          // handle the harmony voice if any
          S_msrVoice
            regularVoiceHarmonyVoice =
              voice->
                getRegularVoiceHarmonyVoiceForwardLink ();

          if (regularVoiceHarmonyVoice) {
            regularVoiceHarmonyVoice->
              finalizeLastAppendedMeasureInVoice (
                inputLineNumber);
          }

          // handle the figuredBass voice if any
          S_msrVoice
            regularVoiceFiguredBassVoice =
              voice->
                getFiguredBassVoiceRegularVoiceBackwardLink ();

          if (regularVoiceFiguredBassVoice) {
            regularVoiceFiguredBassVoice->
              finalizeLastAppendedMeasureInVoice (
                inputLineNumber);
          }
*/
        }
        break;

/* JMI
      case kVoiceHarmony:
      case kVoiceFiguredBass:
        break;
        */
    } // switch
  } // for

  gIndenter--;
}

bool msrStaff::compareVoicesByIncreasingNumber (
  const S_msrVoice& first,
  const S_msrVoice& second)
{
  return
    first->getVoiceNumber ()
      <
    second->getVoiceNumber ();
}

bool msrStaff::compareVoicesToHaveHarmoniesAboveCorrespondingVoice (
  const S_msrVoice& first,
  const S_msrVoice& second)
{
  int
    firstVoiceNumber =
      first->getVoiceNumber (),
    secondVoiceNumber =
      second->getVoiceNumber ();

  if (firstVoiceNumber > K_VOICE_HARMONY_VOICE_BASE_NUMBER) {
    firstVoiceNumber -= K_VOICE_HARMONY_VOICE_BASE_NUMBER + 1;
  }
  if (secondVoiceNumber > K_VOICE_HARMONY_VOICE_BASE_NUMBER) {
    secondVoiceNumber -= K_VOICE_HARMONY_VOICE_BASE_NUMBER + 1;
  }

  bool result =
    firstVoiceNumber < secondVoiceNumber;

  return result;

  /* JMI
  switch (firstVoiceNumber) {
    case kVoiceRegular:
      switch (secondVoiceNumber) {
        case kVoiceRegular:
          break;

        case kVoiceHarmony:
          result =
            secondVoiceNumber - K_VOICE_HARMONY_VOICE_BASE_NUMBER
              >
            firstVoiceNumber;
          break;

        case kVoiceFiguredBass:
          break;
      } // switch
      break;

    case kVoiceHarmony:
      switch (secondVoiceNumber) {
        case kVoiceRegular:
          result =
            firstVoiceNumber - K_VOICE_HARMONY_VOICE_BASE_NUMBER
              >
            secondVoiceNumber;
          break;

        case kVoiceHarmony:
          break;

        case kVoiceFiguredBass:
          break;
      } // switch
      break;

    case kVoiceFiguredBass:
      switch (secondVoiceNumber) {
        case kVoiceRegular:
          break;

        case kVoiceHarmony:
          break;

        case kVoiceFiguredBass:
          break;
      } // switch
      break;
  } // switch

  return result;
  */
}

bool msrStaff::compareVoicesToHaveFiguredBassesBelowCorrespondingVoice (
  const S_msrVoice& first,
  const S_msrVoice& second)
{
  int
    firstVoiceNumber =
      first->getVoiceNumber (),
    secondVoiceNumber =
      second->getVoiceNumber ();

  if (firstVoiceNumber > K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER) {
    firstVoiceNumber -= K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER + 1;
  }
  if (secondVoiceNumber > K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER) {
    secondVoiceNumber -= K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER + 1;
  }

  bool result =
    firstVoiceNumber > secondVoiceNumber;

  return result;
}

void msrStaff::finalizeStaff (int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Finalizing staff \"" <<
      getStaffName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  // finalize the voices
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Finalizing the voices in staff \"" <<
      getStaffName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
    i != fStaffVoiceNumberToAnyVoiceMap.end ();
    i++
  ) {
    S_msrVoice
      voice = (*i).second;

    voice->
      finalizeVoice (
        inputLineNumber);
  } // for

  gIndenter--;
}

void msrStaff::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStaff::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrStaff>*
    p =
      dynamic_cast<visitor<S_msrStaff>*> (v)) {
        S_msrStaff elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStaff::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrStaff::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStaff::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrStaff>*
    p =
      dynamic_cast<visitor<S_msrStaff>*> (v)) {
        S_msrStaff elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStaff::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrStaff::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStaff::browseData ()" <<
      endl;
  }

  /*
    fPartCurrentClef, fPartCurrentKey and fPartCurrentTime are used
    to populate newly created voices, not to create music proper:
    they're thus not browsed
  */

  /*
    fCurrentPartStaffDetails is used
    to populate newly created voices, not to create music proper:
    it is thus not browsed
  */

/*
  if (fStaffTuningsList.size ()) {
    for (
      list<S_msrStaffTuning>::const_iterator i = fStaffTuningsList.begin ();
      i != fStaffTuningsList.end ();
      i++
  ) {
      // browse the voice
      msrBrowser<msrStaffTuning> browser (v);
      browser.browse (*(*i));
    } // for
 //   gInfgdenter--;
  }
*/

/* JMI may be useful???
  if (fStaffVoiceNumberToAnyVoiceMap.size ()) {
    for (
      map<int, S_msrVoice>::const_iterator i = fStaffVoiceNumberToAnyVoiceMap.begin ();
      i != fStaffVoiceNumberToAnyVoiceMap.end ();
      i++
  ) {
        msrBrowser<msrVoice> browser (v);
        browser.browse (*((*i).second));
    } // for
  }
  */

  if (fStaffAllVoicesList.size ()) {
    for (
      list<S_msrVoice>::const_iterator i = fStaffAllVoicesList.begin ();
      i != fStaffAllVoicesList.end ();
      i++
    ) {
        msrBrowser<msrVoice> browser (v);
        browser.browse (*(*i));
    } // for
  }

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% <== msrStaff::browseData ()" <<
      endl;
  }
}

void msrStaff::print (ostream& os) const
{
  os <<
    "Staff number '" <<
    fStaffNumber <<
    "', \"" << getStaffName () <<
    "\", " << staffKindAsString (fStaffKind) <<
    ", " <<
    singularOrPlural (
      fStaffVoiceNumberToAnyVoiceMap.size (), "voice", "voices") <<
    ", " <<
    singularOrPlural (
      fStaffRegularVoicesCounter,
      "regular voice",
      "regular voices") << // JMI
    ")" <<
    endl;

  gIndenter++;

  const unsigned int fieldWidth = 28;

  os << left <<
    setw (fieldWidth) <<
    "staffNumber" << " : " <<
    staffNumberAsString () <<
    endl <<
    setw (fieldWidth) <<
    "staffPartUpLink" << " : " <<
    fStaffPartUpLink->getPartCombinedName () <<
    endl <<
    setw (fieldWidth) <<
    "staffInstrumentName" << " : \"" <<
    fStaffInstrumentName <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "staffInstrumentAbbreviation" << " : \"" <<
    fStaffInstrumentAbbreviation <<
    endl;

  // print current the staff clef if any
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceClefs ()) {
    os << left <<
      setw (fieldWidth) <<
      "staffCurrentClef" << " : ";

    if (fStaffCurrentClef) {
      os <<
        "'" <<
        fStaffCurrentClef->asShortString () <<
        "'";
    }
    else {
      os <<
        "none";
    }

    os << endl;
  }
#endif

  // print the current staff key if any
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceKeys ()) {
    os << left <<
      setw (fieldWidth) <<
      "staffCurrentKey" << " : ";

    if (fStaffCurrentKey) {
      os <<
        "'" <<
        fStaffCurrentKey->asShortString () <<
        "'";
    }
    else {
      os <<
        "none";
    }

    os << endl;
  }
#endif

  // print the current staff time if any
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTimes ()) {
    os << left <<
      setw (fieldWidth) <<
      "staffCurrentTime" << " : ";

    if (fStaffCurrentTime) {
      os <<
        "'" <<
        fStaffCurrentTime->asShortString () <<
        "'";
    }
    else {
      os <<
        "none";
    }

    os << endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceStaffDetails ()) {
    // print the staff details if any
    if (fCurrentStaffStaffDetails) {
      os <<
        fCurrentStaffStaffDetails;
    }
    else {
      os << left <<
        setw (fieldWidth) <<
        "currentStaffStaffDetails" << " : " << "none";
    }
    os << endl;
  }
#endif

  // print the staff all voices map
  os << left <<
    setw (fieldWidth) <<
    "staffVoiceNumberToAnyVoiceMap" << " : " <<
    endl;

  if (fStaffVoiceNumberToAnyVoiceMap.size ()) {
    gIndenter++;

    map<int, S_msrVoice>::const_iterator
      iBegin = fStaffVoiceNumberToAnyVoiceMap.begin (),
      iEnd   = fStaffVoiceNumberToAnyVoiceMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      int        voiceNumber = (*i).first;
      S_msrVoice voice       = (*i).second;

      // sanity check
      msgAssert (
        voice != nullptr,
        "voice is null");

      os <<
        voiceNumber <<
        " : " <<
        voice->asShortString () <<
        endl;

      gIndenter.increment (3);

      os <<
        "voiceKind: " <<
        voiceKindAsString (voice->getVoiceKind ()) <<
        ", regularVoiceStaffSequentialNumber: " <<
        voice->getRegularVoiceStaffSequentialNumber () <<
        endl;

      gIndenter.decrement (3);

      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
 //   os << endl;

    gIndenter--;
  }
  else {
    os << "empty" << endl;
  }

  // print the staff regular voices map
  os << left <<
    setw (fieldWidth) <<
    "staffVoiceNumberToVoicesMap" << " : " <<
    endl;

  if (fStaffVoiceNumberToVoicesMap.size ()) {
    gIndenter++;

    map<int, S_msrVoice>::const_iterator
      iBegin = fStaffVoiceNumberToVoicesMap.begin (),
      iEnd   = fStaffVoiceNumberToVoicesMap.end (),
      i      = iBegin;

    for ( ; ; ) {
//      if (i == iEnd) break; // JMI ???

      int        voiceNumber = (*i).first;
      S_msrVoice voice       = (*i).second;

      // sanity check
      msgAssert (
        voice != nullptr,
        "voice is null");

      os <<
        voiceNumber <<
        " : " <<
        "Voice \"" << voice->getVoiceName () <<
        "\", line " << voice->getInputLineNumber () <<
        endl;

      gIndenter.increment (3);

      os <<
        "voiceKind: " <<
        voiceKindAsString (voice->getVoiceKind ()) <<
        ", regularVoiceStaffSequentialNumber: " <<
        voice->getRegularVoiceStaffSequentialNumber () <<
        endl;

      gIndenter.decrement (3);

      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for
//    os << endl;

    gIndenter--;
  }
  else {
    os << "empty" << endl;
  }

  // print the regular voices list
  os << left <<
    setw (fieldWidth) <<
    "staffRegularVoicesList" << " : " <<
    endl;

  if (fStaffRegularVoicesList.size ()) {
    gIndenter++;

    list<S_msrVoice>::const_iterator
      iBegin = fStaffRegularVoicesList.begin (),
      iEnd   = fStaffRegularVoicesList.end (),
      i      = iBegin;

    int voiceNumber = 0;

    for ( ; ; ) {
 //     if (i == iEnd) break; // JMI ???

      S_msrVoice voice = (*i);

      // sanity check
      msgAssert (
        voice != nullptr,
        "voice is null");

      voiceNumber++;

      os <<
        voiceNumber <<
        " : " <<
        voice->asShortString () <<
        endl;

      gIndenter.increment (3);

      os <<
        "voiceKind: " <<
        voiceKindAsString (voice->getVoiceKind ()) <<
        ", regularVoiceStaffSequentialNumber: " <<
        voice->getRegularVoiceStaffSequentialNumber () <<
        endl;

      gIndenter.decrement (3);

      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
//    os << endl;

    gIndenter--;
  }
  else {
    os << "empty" << endl;
  }

  // print the  voices
  if (fStaffVoiceNumberToAnyVoiceMap.size ()) {
    os << endl;

    map<int, S_msrVoice>::const_iterator
      iBegin = fStaffVoiceNumberToAnyVoiceMap.begin (),
      iEnd   = fStaffVoiceNumberToAnyVoiceMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrVoice voice = (*i).second;

/* JMI
os <<
  endl <<
  "================= voice :" <<
  endl <<
  voice <<
  endl << endl;
*/

      os <<
        voice;

        /* JMI
      switch (voice->getVoiceKind ()) {
        case kVoiceRegular:
          os <<
            voice;
          break;

        case kVoiceHarmony:
          if (
            gGlobalMsrOahGroup->getShowHarmonyVoices ()
              ||
            voice->getMusicHasBeenInsertedInVoice ())
            os <<
              voice;
          break;

        case kVoiceFiguredBass:
          if (
            gGlobalMsrOahGroup->getShowFiguredBassVoices ()
              ||
            voice->getMusicHasBeenInsertedInVoice ())
            os <<
              voice;
          break;
      } // switch
        */

      if (++i == iEnd) break;

      os << endl;
    } // for
  }

  gIndenter--;
}

void msrStaff::printShort (ostream& os) const
{
  os <<
    "Staff number '" <<
    fStaffNumber <<
    "', \"" << getStaffName () <<
    "\", " << staffKindAsString (fStaffKind) <<
    ", " <<
    singularOrPlural (
      fStaffVoiceNumberToAnyVoiceMap.size (), "voice", "voices") <<
    ", " <<
    singularOrPlural (
      fStaffRegularVoicesCounter,
      "regular voice",
      "regular voices") << // JMI
    ")" <<
    endl;

  gIndenter++;

/*
  const unsigned int fieldWidth = 28;

  os << left <<
    setw (fieldWidth) <<
    "staffPartUpLink" << " : " <<
    fStaffPartUpLink->getPartCombinedName () <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "staffInstrumentName" << " : \"" <<
    fStaffInstrumentName <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "staffInstrumentAbbreviation" << " : \"" <<
    fStaffInstrumentAbbreviation <<
    endl;

  // print current the staff clef if any
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceClefs ()) {
    os << left <<
      setw (fieldWidth) <<
      "staffCurrentClef" << " : ";

    if (fStaffCurrentClef) {
      os <<
        "'" <<
        fStaffCurrentClef->asShortString () <<
        "'";
    }
    else {
      os <<
        "none";
    }

    os << endl;
  }
#endif

  // print the current staff key if any
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceKeys ()) {
    os << left <<
      setw (fieldWidth) <<
      "staffCurrentKey" << " : ";

    if (fStaffCurrentKey) {
      os <<
        "'" <<
        fStaffCurrentKey->asShortString () <<
        "'";
    }
    else {
      os <<
        "none";
    }

    os << endl;
  }
#endif

  // print the current staff time if any
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTimes ()) {
    os << left <<
      setw (fieldWidth) <<
      "staffCurrentTime" << " : ";

    if (fStaffCurrentTime) {
      os <<
        "'" <<
        fStaffCurrentTime->asShortString () <<
        "'";
    }
    else {
      os <<
        "none";
    }

    os << endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceStaffDetails ()) {
    // print the staff details if any
    if (fCurrentStaffStaffDetails) {
      os <<
        fCurrentStaffStaffDetails;
    }
    else {
      os << left <<
        setw (fieldWidth) <<
        "currentStaffStaffDetails" << " : " << "none";
    }
    os << endl;
  }
#endif

  // print the staff all voices map
  os << left <<
    setw (fieldWidth) <<
    "staffAllVoicesMap" << " : " <<
    endl;

  if (fStaffVoiceNumberToAnyVoiceMap.size ()) {
    gIndenter++;

    map<int, S_msrVoice>::const_iterator
      iBegin = fStaffVoiceNumberToAnyVoiceMap.begin (),
      iEnd   = fStaffVoiceNumberToAnyVoiceMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      int        voiceNumber = (*i).first;
      S_msrVoice voice       = (*i).second;

      // sanity check
      msgAssert (
        voice != nullptr,
        "voice is null");

      os <<
        voiceNumber << " : " <<
        "regularVoiceStaffSequentialNumber = " <<
        voice->getRegularVoiceStaffSequentialNumber () <<
        ", " <<
        voice->asShortString () <<
        endl;
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
 //   os << endl;

    gIndenter--;
  }
  else {
    os << "empty" << endl;
  }

  // print the staff regular voices map
  os << left <<
    setw (fieldWidth) <<
    "staffRegularVoicesMap" << " : ";

  if (fStaffVoiceNumberToVoicesMap.size ()) {
    gIndenter++;

    map<int, S_msrVoice>::const_iterator
      iBegin = fStaffVoiceNumberToVoicesMap.begin (),
      iEnd   = fStaffVoiceNumberToVoicesMap.end (),
      i      = iBegin;

    for ( ; ; ) {
//      if (i == iEnd) break; // JMI ???

      int        voiceNumber = (*i).first;
      S_msrVoice voice       = (*i).second;

      // sanity check
      msgAssert (
        voice != nullptr,
        "voice is null");

      os <<
        voiceNumber << " : " <<
        "regularVoiceStaffSequentialNumber = " <<
        voice->getRegularVoiceStaffSequentialNumber () <<
        ", " <<
        voice->asShortString () <<
        endl;
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for
//    os << endl;

    gIndenter--;
  }
  else {
    os << "empty" << endl;
  }

  // print the regular voices list
  os << left <<
    setw (fieldWidth) <<
    "staffRegularVoicesList" << " : " <<
    endl;

  if (fStaffRegularVoicesList.size ()) {
    gIndenter++;

    list<S_msrVoice>::const_iterator
      iBegin = fStaffRegularVoicesList.begin (),
      iEnd   = fStaffRegularVoicesList.end (),
      i      = iBegin;

    int voiceNumber = 0;

    for ( ; ; ) {
 //     if (i == iEnd) break; // JMI ???

      S_msrVoice voice = (*i);

      // sanity check
      msgAssert (
        voice != nullptr,
        "voice is null");

      voiceNumber++;

      os <<
        voiceNumber << " : " <<
        "regularVoiceStaffSequentialNumber = " <<
        voice->getRegularVoiceStaffSequentialNumber () <<
        ", " <<
        voice->asShortString () <<
        endl;
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
//    os << endl;

    gIndenter--;
  }
  else {
    os << "empty" << endl;
  }
*/

  // print the  voices
  if (fStaffVoiceNumberToAnyVoiceMap.size ()) {
    os << endl;

    map<int, S_msrVoice>::const_iterator
      iBegin = fStaffVoiceNumberToAnyVoiceMap.begin (),
      iEnd   = fStaffVoiceNumberToAnyVoiceMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrVoice voice = (*i).second;

/* JMI
os <<
  endl <<
  "================= voice :" <<
  endl <<
  voice <<
  endl << endl;
*/

      voice->printShort (os);

        /* JMI
      switch (voice->getVoiceKind ()) {
        case kVoiceRegular:
          os <<
            voice;
          break;

        case kVoiceHarmony:
          if (
            gGlobalMsrOahGroup->getShowHarmonyVoices ()
              ||
            voice->getMusicHasBeenInsertedInVoice ())
            os <<
              voice;
          break;

        case kVoiceFiguredBass:
          if (
            gGlobalMsrOahGroup->getShowFiguredBassVoices ()
              ||
            voice->getMusicHasBeenInsertedInVoice ())
            os <<
              voice;
          break;
      } // switch
        */

      if (++i == iEnd) break;

      os << endl;
    } // for
  }

  gIndenter--;
}

void msrStaff::printSummary (ostream& os) const
{
  os <<
    "Staff number '" <<
    fStaffNumber <<
    "', \"" << getStaffName () <<
    "\", " << staffKindAsString (fStaffKind) <<
    " (" <<
    singularOrPlural (
      fStaffVoiceNumberToAnyVoiceMap.size (), "voice", "voices") <<
    ")" <<
    endl;

  gIndenter++;

  os <<
    "StaffInstrumentName: \"" <<
    fStaffInstrumentName << "\"" <<
    endl;

/* JMI
  if (fStaffTuningsList.size ()) {
    os <<
      "Staff tunings:" <<
      endl;

    list<S_msrStaffTuning>::const_iterator
      iBegin = fStaffTuningsList.begin (),
      iEnd   = fStaffTuningsList.end (),
      i      = iBegin;

    gIndenter++;
    for ( ; ; ) {
      os << (*i)->asString ();
      if (++i == iEnd) break;
      os << endl;
    } // for
    os << endl;
    gIndenter--;
  }

  // print the voices names
  if (fStaffVoiceNumberToAnyVoiceMap.size ()) {
    os <<
      "Voices:" <<
      endl;

    gIndenter++;

    map<int, S_msrVoice>::const_iterator
      iBegin = fStaffVoiceNumberToAnyVoiceMap.begin (),
      iEnd   = fStaffVoiceNumberToAnyVoiceMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrVoice
        voice =
          (*i).second;

      os <<
        left <<
          voice->getVoiceName () <<
          " (" <<
          singularOrPlural (
            voice->getVoiceActualNotesCounter (),
            "actual note",
            "actual notes") <<
          ", " <<
          singularOrPlural (
            voice->getVoiceStanzasMap ().size (),
            "stanza",
            "stanzas") <<
          ")";
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }
*/

  os << endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrStaff& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrVoiceStaffChange msrVoiceStaffChange::create (
  int        inputLineNumber,
  S_msrStaff staffToChangeTo)
{
  msrVoiceStaffChange* o =
    new msrVoiceStaffChange (
      inputLineNumber, staffToChangeTo);
  assert (o!=0);
  return o;
}

msrVoiceStaffChange::msrVoiceStaffChange (
  int        inputLineNumber,
  S_msrStaff staffToChangeTo)
    : msrMeasureElement (inputLineNumber)
{
  fStaffToChangeTo = staffToChangeTo;
}

msrVoiceStaffChange::~msrVoiceStaffChange ()
{}

S_msrVoiceStaffChange msrVoiceStaffChange::createStaffChangeNewbornClone ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceStaffChanges ()) {
    gLogStream <<
      "Creating a newborn clone of voice staff change '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

 S_msrVoiceStaffChange
    newbornClone =
      msrVoiceStaffChange::create (
        fInputLineNumber,
        fStaffToChangeTo);

  return newbornClone;
}

void msrVoiceStaffChange::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrVoiceStaffChange::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrVoiceStaffChange>*
    p =
      dynamic_cast<visitor<S_msrVoiceStaffChange>*> (v)) {
        S_msrVoiceStaffChange elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrVoiceStaffChange::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrVoiceStaffChange::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrVoiceStaffChange::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrVoiceStaffChange>*
    p =
      dynamic_cast<visitor<S_msrVoiceStaffChange>*> (v)) {
        S_msrVoiceStaffChange elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrVoiceStaffChange::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrVoiceStaffChange::browseData (basevisitor* v)
{}

string msrVoiceStaffChange::asString () const
{
  stringstream s;

  s <<
    "VoiceStaffChange" <<
    ", line " << fInputLineNumber <<
    ", " <<
    "staffToChangeTo: \"" << fStaffToChangeTo->getStaffName () << "\"";

  return s.str ();
}

void msrVoiceStaffChange::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrVoiceStaffChange& elt)
{
  elt->print (os);
  return os;
}


}
