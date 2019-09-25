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

#include "musicXMLOah.h"
#include "msrOah.h"


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
  assert(o!=0);

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
  msrAssert(
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceStaves) {
    gLogOstream <<
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
    case msrStaff::kStaffRegular:
      fStaffName =
        fStaffPartUpLink->getPartMsrName () +
        "_Staff_" +
        int2EnglishWord (fStaffNumber);
      break;

    case msrStaff::kStaffTablature:
        fStaffPartUpLink->getPartMsrName () +
        "_Tablature_" +
        int2EnglishWord (fStaffNumber);
      break;

    case msrStaff::kStaffHarmony:
      fStaffName =
        fStaffPartUpLink->getPartMsrName () +
        "_HARMONY_Staff";
      break;

    case msrStaff::kStaffFiguredBass:
      fStaffName =
        fStaffPartUpLink->getPartMsrName () +
        "_FIGURED_BASS_Staff";
      break;

    case msrStaff::kStaffDrum:
      fStaffName =
        fStaffPartUpLink->getPartMsrName () +
        "_DRUM_Staff";
      break;

    case msrStaff::kStaffRythmic:
      fStaffName =
        fStaffPartUpLink->getPartMsrName () +
        "_RYTHMIC_Staff";
      break;
  } // switch

// JMI  gIndenter++;

  // check the staff number
  switch (fStaffKind) {
    case msrStaff::kStaffRegular:
      // the staff number should not be negative
      if (fStaffNumber < 0) {
        stringstream s;

        s <<
          "regular staff number " << fStaffNumber <<
          " is not positive";

        msrAssert (
          false,
          s.str ());
      }
      break;

    case msrStaff::kStaffTablature:
      break;

    case msrStaff::kStaffHarmony:
      break;

    case msrStaff::kStaffFiguredBass:
    /* JMI
      if (fStaffNumber != K_PART_FIGURED_BASS_STAFF_NUMBER) {
        stringstream s;

        s <<
          "figured bass staff number " << fStaffNumber <<
          " is not equal to " << K_PART_FIGURED_BASS_STAFF_NUMBER;

        msrInternalError (
          gOahOah->fInputSourceName,
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      */
      break;

    case msrStaff::kStaffDrum:
      break;

    case msrStaff::kStaffRythmic:
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
#ifdef TRACE_OAH
      if (gTraceOah->fTraceClefs || gTraceOah->fTraceStaves) {
        gLogOstream <<
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
#ifdef TRACE_OAH
      if (gTraceOah->fTraceStaves || gTraceOah->fTraceKeys) {
        gLogOstream <<
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
#ifdef TRACE_OAH
      if (gTraceOah->fTraceStaves /* JMI ||       gTraceOah->fTraceTransposes */) {
        gLogOstream <<
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

  // current position in measure
  fStaffCurrentPositionInMeasure = rational (0,0);

// JMI  gIndenter--;
}

msrStaff::~msrStaff ()
{}

S_msrStaff msrStaff::createStaffNewbornClone (
  S_msrPart containingPart)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceStaves) {
    gLogOstream <<
      "Creating a newborn clone of staff \"" <<
      fStaffName <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
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

string msrStaff::staffNumberAsString ()
{
  string result;

  switch (fStaffNumber) {
    case K_PART_FIGURED_BASS_STAFF_NUMBER:
      result = "K_PART_FIGURED_BASS_STAFF_NUMBER"; // JMI
      break;
    default:
      result = to_string (fStaffNumber);
  } // switch

  return result;
}

/* KEEP JMI
const int msrStaff::getStaffNumberOfMusicVoices () const
{
  int result = 0;

  for (
    map<int, S_msrVoice>::const_iterator i =
      fStaffRegularVoicesMap.begin ();
    i != fStaffRegularVoicesMap.end ();
    i++
  ) {
      S_msrVoice
        voice =
          (*i).second;

      switch (voice->getVoiceKind ()) {
        case msrVoice::kVoiceRegular:
          if (voice->getMusicHasBeenInsertedInVoice ()) {
            result++;
          }
          break;

        case msrVoice::kVoiceHarmony: // JMI
          break;

        case msrVoice::kVoiceFiguredBass: // JMI
          break;
      } // switch

  } // for

  return result;
}
*/

void msrStaff::createMeasureAndAppendItToStaff (
  int    inputLineNumber,
  string measureNumber,
  msrMeasure::msrMeasureImplicitKind
         measureImplicitKind)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures || gTraceOah->fTraceStaves) {
    gLogOstream <<
      "Creating and appending measure '" <<
      measureNumber <<
      ", in staff \"" << getStaffName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  // propagate it to all staves
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++
  ) {
    S_msrVoice
      voice = (*i).second;

    // sanity check
    msrAssert (
      voice != nullptr,
      "voice is null");

    switch (voice->getVoiceKind ()) {
      case msrVoice::kVoiceRegular:
#ifdef TRACE_OAH
        if (gTraceOah->fTraceMeasures || gTraceOah->fTraceStaves) {
          gLogOstream <<
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

      case msrVoice::kVoiceHarmony:
        break;

      case msrVoice::kVoiceFiguredBass:
        break;
    } // switch
  } // for
}

void msrStaff::setNextMeasureNumberInStaff (
  int    inputLineNumber,
  string nextMeasureNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures || gTraceOah->fTraceStaves) {
    gLogOstream <<
      "Setting next measure number to '" <<
      nextMeasureNumber <<
      ", in staff \"" << getStaffName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  // propagate it to all staves
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++
  ) {
    S_msrVoice voice = (*i).second;

    // sanity check
    msrAssert (
      voice != nullptr,
      "voice is null");

#ifdef TRACE_OAH
    if (gTraceOah->fTraceMeasures || gTraceOah->fTraceStaves) {
      gLogOstream <<
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
  msrVoice::msrVoiceKind voiceKind,
  int                    voiceNumber,
  string                 currentMeasureNumber)
{
  // take this new voice into account if relevant
  switch (voiceKind) {
    case msrVoice::kVoiceRegular:
      fStaffRegularVoicesCounter++;

#ifdef TRACE_OAH
      if (gTraceOah->fTraceStaves || gTraceOah->fTraceVoices) {
        gLogOstream <<
          "Creating regular voice number '" <<
          voiceNumber <<
          "', voiceKind '" <<
          msrVoice::voiceKindAsString (voiceKind) <<
          "' as regular voice '" <<
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

    case msrVoice::kVoiceHarmony:
#ifdef TRACE_OAH
      if (gTraceOah->fTraceStaves || gTraceOah->fTraceVoices) {
        gLogOstream <<
          "Creating harmony voice number '" <<
          voiceNumber <<
          "', voiceKind '" <<
          msrVoice::voiceKindAsString (voiceKind) <<
          "' in staff \"" << getStaffName () <<
          "\", line " << inputLineNumber <<
          "\", current measure number: " <<
          currentMeasureNumber <<
     // JMI     " in part " << fStaffPartUpLink->getPartCombinedName () <<
          endl;
      }
#endif
      break;

    case msrVoice::kVoiceFiguredBass:
#ifdef TRACE_OAH
      if (gTraceOah->fTraceStaves || gTraceOah->fTraceVoices) {
        gLogOstream <<
          "Creating figured bass voice number '" <<
          voiceNumber <<
          "', voiceKind '" <<
          msrVoice::voiceKindAsString (voiceKind) <<
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
      gOahOah->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
      */
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
    case msrVoice::kVoiceRegular:
      // register the voice by its relative number
      /* JMI
#ifdef TRACE_OAH
      if (gTraceOah->fTraceVoices) {
        gLogOstream <<
          "Voice " << voiceNumber <<
          " in staff " << getStaffName () <<
          " gets staff regular voice number " <<
          fStaffRegularVoicesCounter <<
          endl;
      }
#endif
*/

      registerVoiceInRegularVoicesMapByItsNumberByItsNumber (
        voiceNumber,
        voice);
      break;

    case msrVoice::kVoiceHarmony:
      break;

    case msrVoice::kVoiceFiguredBass:
      break;
  } // switch

  // register it in staff by its number
  registerVoiceByItsNumber (
    inputLineNumber,
    voiceNumber,
    voice);

  return voice;
}

void msrStaff::registerVoiceByItsNumber (
  int        inputLineNumber,
  int        voiceNumber,
  S_msrVoice voice)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices || gTraceOah->fTraceStaves) {
    gLogOstream <<
      "Registering voice number '" << voiceNumber <<
      "', named \"" << voice->getVoiceName () <<
      "\" in staff " << getStaffName () <<
      endl;
  }
#endif

  // register voice in all voices map
  fStaffAllVoicesMap [voiceNumber] =
    voice;

  // register it in all voices list
  fStaffAllVoicesList.push_back (voice);

  // sort the list if necessary
  switch (voice->getVoiceKind ()) {
    case msrVoice::kVoiceRegular:
      break;

    case msrVoice::kVoiceHarmony:
#ifdef TRACE_OAH
      if (gTraceOah->fTraceStaves || gTraceOah->fTraceVoices) {
        gLogOstream <<
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

    case msrVoice::kVoiceFiguredBass:
#ifdef TRACE_OAH
      if (gTraceOah->fTraceStaves || gTraceOah->fTraceVoices) {
        gLogOstream <<
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

void msrStaff::registerVoiceInRegularVoicesMapByItsNumberByItsNumber (
  int        voiceNumber,
  S_msrVoice voice)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices || gTraceOah->fTraceStaves) {
    gLogOstream <<
      "Registering regular voice number '" << voiceNumber <<
      "', named \"" << voice->getVoiceName () <<
      "\" in staff " << getStaffName () <<
      "'s regular voices map as regular voice with sequential number " <<
      fStaffRegularVoicesCounter <<
      endl;
  }
#endif

  fStaffRegularVoicesList.push_back (voice);

  fStaffRegularVoicesMap [fStaffRegularVoicesCounter] =
    voice;

  // setRegularVoiceStaffSequentialNumber() will be called in msrStaff::finalizeStaff()

/* JMI
  // set voice staff sequential number
  voice->
    setRegularVoiceStaffSequentialNumber (
      fStaffRegularVoicesCounter);
      */
}

void msrStaff::registerVoiceInAllVoicesList (
  int        voiceNumber,
  S_msrVoice voice)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices || gTraceOah->fTraceStaves) {
    gLogOstream <<
      "Registering voice number '" << voiceNumber <<
      "', named \"" << voice->getVoiceName () <<
      "\" in staff " << getStaffName () <<
      "'s all voices list" <<
      endl;
  }
#endif

  fStaffAllVoicesList.push_back (voice);
}

S_msrVoice msrStaff::fetchVoiceFromStaffByItsNumber (
  int inputLineNumber,
  int voiceNumber)
{
  S_msrVoice result; // JMI avoid repetitive messages!

#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices || gTraceOah->fTraceStaves) {
    gLogOstream <<
      "Fetching voice number " <<
      voiceNumber <<
     " in staff \"" << getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i =
      fStaffRegularVoicesMap.begin ();
    i != fStaffRegularVoicesMap.end ();
    i++
  ) {
    if (
      (*i).second->getVoiceNumber ()
        ==
      voiceNumber) {
#ifdef TRACE_OAH
        if (gTraceOah->fTraceVoices || gTraceOah->fTraceStaves) {
          gLogOstream <<
            "Voice " << voiceNumber <<
            " in staff \"" << getStaffName () << "\"" <<
            " has staff relative number " << (*i).first <<
            endl;
        }
#endif

        result = (*i).second;
        break;
    }
  } // for

  return result;
}

void msrStaff::assignSequentialNumbersToRegularVoicesInStaff (
  int inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceStaves || gTraceOah->fTraceVoices) {
    gLogOstream <<
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
      msrAssert (
        voice != nullptr,
        "voice is null");

      voiceSequentialCounter++;

#ifdef TRACE_OAH
      if (gTraceOah->fTraceStaves || gTraceOah->fTraceVoices) {
        gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices || gTraceOah->fTraceStaves) {
    gLogOstream <<
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
      gOahOah->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());

  }
  else {
    result = fStaffRegularVoicesList.front ();

    // sanity check
    msrAssert (
      result->getRegularVoiceStaffSequentialNumber () == 1,
      "result->getRegularVoiceStaffSequentialNumber () is not equal to 1");

#ifdef TRACE_OAH
    if (gTraceOah->fTraceVoices || gTraceOah->fTraceStaves) {
      gLogOstream <<
        "The first regular voice in staff \"" <<
        getStaffName () <<
        "\" is " <<
        result->asShortString () <<
        endl;
    }
#endif
  }

#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices || gTraceOah->fTraceStaves) {
    gLogOstream <<
      "--> result = \"" <<
      result->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  return result;
}

void msrStaff::addAVoiceToStaffIfItHasNone (
  int inputLineNumber)
{
  if (fStaffAllVoicesMap.size () == 0) {
#ifdef TRACE_OAH
    if (gTraceOah->fTraceStaves || gTraceOah->fTraceVoices) {
      gLogOstream <<
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
        msrVoice::kVoiceRegular,
        1,    // voiceNumber,
        "1"); // fCurrentMeasureNumber
  }
}

void msrStaff::registerVoiceInStaff (
  int        inputLineNumber,
  S_msrVoice voice)
{
  // sanity check
  msrAssert (
    voice != nullptr,
    "voice is null");

  // get voice kind
  msrVoice::msrVoiceKind voiceKind =
    voice->getVoiceKind ();

  // take this new voice into account if relevant
  switch (voiceKind) {
    case msrVoice::kVoiceRegular:
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
          gOahOah->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
          */
      }
      break;

    case msrVoice::kVoiceHarmony:
      break;
    case msrVoice::kVoiceFiguredBass:
      break;
  } // switch

  // register voice in this staff
#ifdef TRACE_OAH
  if (gTraceOah->fTraceStaves || gTraceOah->fTraceVoices) {
    gLogOstream <<
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
    case msrVoice::kVoiceRegular:
      {
        int voiceNumber = voice->getVoiceNumber ();

        // register the voice by its number
#ifdef TRACE_OAH
        if (gTraceOah->fTraceVoices) {
          gLogOstream <<
            "Registering regular voice '" << voiceNumber <<
            "' " << voice->getVoiceName () <<
            " with staff regular voice number " <<
            fStaffRegularVoicesCounter <<
            " in staff " << getStaffName () <<
            endl;
        }
#endif

        registerVoiceInRegularVoicesMapByItsNumberByItsNumber (
          voiceNumber,
          voice);
      }
      break;

    case msrVoice::kVoiceHarmony:
      break;

    case msrVoice::kVoiceFiguredBass:
      break;
  } // switch
}

void msrStaff::appendClefToStaff (S_msrClef clef)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceClefs || gTraceOah->fTraceStaves) {
    gLogOstream <<
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
  // and a clef equal to the current clef is met
  bool doAppendClefToStaff = true;

  if (fStaffCurrentClef) {
    if (
      gMusicXMLOah->fIgnoreRedundantClefs
        &&
      clef->isEqualTo (fStaffCurrentClef)
    ) {
      doAppendClefToStaff = false;
    }
  }

  if (doAppendClefToStaff) {
    // register clef as current staff clef
    fStaffCurrentClef = clef;

// JMI ??? should there be a staff lines number change for 00f-Basics-Clefs.xml???

    // set staff kind accordingly if relevant
    switch (clef->getClefKind ()) {
      case msrClef::kPercussionClef:
        fStaffKind = msrStaff::kStaffDrum; // JMI ???
        break;
      case msrClef::kTablature4Clef:
      case msrClef::kTablature5Clef:
      case msrClef::kTablature6Clef:
      case msrClef::kTablature7Clef:
        fStaffKind = msrStaff::kStaffTablature;
        break;
      default:
        ;
    } // switch

    // propagate clef to all voices
    for (
      map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
      i != fStaffAllVoicesMap.end ();
      i++
    ) {
      (*i).second-> // JMI msrAssert???
        appendClefToVoice (clef);
    } // for
  }

  else {
#ifdef TRACE_OAH
    if (gTraceOah->fTraceClefs || gTraceOah->fTraceStaves) {
      gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceKeys || gTraceOah->fTraceStaves) {
    gLogOstream <<
      "Appending key '" << key->asString () <<
      "' to staff \"" <<
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
      gMusicXMLOah->fIgnoreRedundantKeys
        &&
      fStaffCurrentKey->isEqualTo (key)
    ) {
      doAppendKeyToStaff = false;
    }

    else {
      if (key->isEqualTo (fStaffCurrentKey)) {
#ifdef TRACE_OAH
        if (gTraceOah->fTraceKeys || gTraceOah->fTraceStaves) {
          gLogOstream <<
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
      map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
      i != fStaffAllVoicesMap.end ();
      i++
    ) {
      (*i).second-> // JMI msrAssert???
        appendKeyToVoice (key);
    } // for
  }

  gIndenter--;
}

void msrStaff::appendTimeToStaff (S_msrTime time)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTimes || gTraceOah->fTraceStaves) {
    gLogOstream <<
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
      gMusicXMLOah->fIgnoreRedundantTimes
        &&
      fStaffCurrentTime->isEqualTo (time)
    ) {
      doAppendTimeToStaff = false;
    }

    else {
      if (time->isEqualTo (fStaffCurrentTime)) {
#ifdef TRACE_OAH
        if (gTraceOah->fTraceTimes || gTraceOah->fTraceStaves) {
          gLogOstream <<
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
      map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
      i != fStaffAllVoicesMap.end ();
      i++
    ) {
      (*i).second-> // JMI msrAssert???
        appendTimeToVoice (time);
    } // for
  }

  gIndenter--;
}

void msrStaff::appendTimeToStaffClone (S_msrTime time)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTimes || gTraceOah->fTraceStaves) {
    gLogOstream <<
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
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++
  ) {
    (*i).second-> // JMI msrAssert???
      appendTimeToVoiceClone (time);
  } // for

  gIndenter--;
}

/* JMI
void msrStaff::nestContentsIntoNewRepeatInStaff (
  int inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Nesting contents into new repeat in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
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
  msrRepeatEnding::msrRepeatEndingKind
            repeatEndingKind)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Handling a " <<
      msrRepeatEnding::repeatEndingKindAsString (
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
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Creating a measures repeat from it's first measure in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Appending the pending measures repeat to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Creating a multiple rest in staff \"" <<
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
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++
  ) {
    (*i).second->
      createRestMeasuresInVoice (
        inputLineNumber,
        restMeasuresMeasuresNumber);
  } // for
}

void msrStaff::appendPendingRestMeasuresToStaff (
  int inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Appending the pending multiple rest to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Appending multiple rest '" <<
      restMeasures->asString () <<
      "' to staff clone \"" <<
      getStaffName () <<
      "\"" <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++
  ) {
    (*i).second-> // JMI msrAssert???
      appendRestMeasuresCloneToVoiceClone (
        inputLineNumber,
        restMeasures);
  } // for
}

void msrStaff::appendRepeatCloneToStaff (
  int         inputLineNumber,
  S_msrRepeat repeatCLone)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Appending repeat clone to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++
  ) {
    (*i).second-> // JMI msrAssert???
      appendRepeatCloneToVoiceClone (
        inputLineNumber, repeatCLone);
  } // for
}

void msrStaff::appendRepeatEndingCloneToStaff (
  S_msrRepeatEnding repeatEndingClone)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
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
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++
  ) {
    (*i).second-> // JMI msrAssert???
      appendRepeatEndingCloneToVoice (
        repeatEndingClone);
  } // for

  gIndenter--;
}

void msrStaff::appendBarlineToStaff (S_msrBarline barline)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    gLogOstream <<
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
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTranspositions || gTraceOah->fTraceStaves) {
    gLogOstream <<
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
#ifdef TRACE_OAH
      if (gTraceOah->fTraceTranspositions || gTraceOah->fTraceStaves) {
        gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTranspositions || gTraceOah->fTraceStaves) {
    gLogOstream <<
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
#ifdef TRACE_OAH
      if (gTraceOah->fTraceTranspositions || gTraceOah->fTraceStaves) {
        gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTranspositions || gTraceOah->fTraceStaves) {
    gLogOstream <<
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
#ifdef TRACE_OAH
      if (gTraceOah->fTraceTranspositions || gTraceOah->fTraceStaves) {
        gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceStaves) {
    gLogOstream <<
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
  msrAssert (
    staffDetails != nullptr,
    "staffDetails is null");

  // register staff details in staff
  fCurrentStaffStaffDetails = staffDetails;

  // set staff kind accordingly if relevant
  switch (staffDetails->getStaffLinesNumber ()) {
    case 1:
      if (gMsrOah->fCreateSingleLineStavesAsRythmic) {
        fStaffKind = msrStaff::kStaffRythmic;
      }
      else {
        fStaffKind = msrStaff::kStaffDrum;
      }
      break;
    default:
      ;
  } // switch

#ifdef TRACE_OAH
  if (gTraceOah->fTraceStaves) {
    gLogOstream <<
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
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
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
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
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
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
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
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
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
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
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
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
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
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
    i++
  ) {
    (*i).second->
      appendHarpPedalsTuningToVoice (
        harpPedalsTuning);
  } // for
}

void msrStaff::finalizeCurrentMeasureInStaff (
  int inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures || gTraceOah->fTraceStaves) {
    gLogOstream <<
      "Finalizing current measure in staff \"" <<
      getStaffName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  // finalize all the regular voices
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffRegularVoicesMap.begin ();
    i != fStaffRegularVoicesMap.end ();
    i++
  ) {
    S_msrVoice
      voice =
        (*i).second;

    switch (voice->getVoiceKind ()) {
      case msrVoice::kVoiceRegular:
        {
          // handle the regular voice
          voice->
            finalizeCurrentMeasureInVoice (
              inputLineNumber);

/* JMI this will be done later
          // handle the harmony voice if any
          S_msrVoice
            harmonyVoiceForRegularVoice =
              voice->
                getHarmonyVoiceForRegularVoiceForwardLink ();

          if (harmonyVoiceForRegularVoice) {
            harmonyVoiceForRegularVoice->
              finalizeCurrentMeasureInVoice (
                inputLineNumber);
          }
*/

          // handle the figuredBass voice if any
          S_msrVoice
            figuredBassVoiceForRegularVoice =
              voice->
                getFiguredBassVoiceForRegularVoiceForwardLink ();

          if (figuredBassVoiceForRegularVoice) {
            figuredBassVoiceForRegularVoice->
              finalizeCurrentMeasureInVoice (
                inputLineNumber);
          }
        }
        break;

      case msrVoice::kVoiceHarmony:
      case msrVoice::kVoiceFiguredBass:
        break;
    } // switch
  } // for

  // reset current position in measure
  fStaffCurrentPositionInMeasure = rational (0,0);

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
    case msrVoice::kVoiceRegular:
      switch (secondVoiceNumber) {
        case msrVoice::kVoiceRegular:
          break;

        case msrVoice::kVoiceHarmony:
          result =
            secondVoiceNumber - K_VOICE_HARMONY_VOICE_BASE_NUMBER
              >
            firstVoiceNumber;
          break;

        case msrVoice::kVoiceFiguredBass:
          break;
      } // switch
      break;

    case msrVoice::kVoiceHarmony:
      switch (secondVoiceNumber) {
        case msrVoice::kVoiceRegular:
          result =
            firstVoiceNumber - K_VOICE_HARMONY_VOICE_BASE_NUMBER
              >
            secondVoiceNumber;
          break;

        case msrVoice::kVoiceHarmony:
          break;

        case msrVoice::kVoiceFiguredBass:
          break;
      } // switch
      break;

    case msrVoice::kVoiceFiguredBass:
      switch (secondVoiceNumber) {
        case msrVoice::kVoiceRegular:
          break;

        case msrVoice::kVoiceHarmony:
          break;

        case msrVoice::kVoiceFiguredBass:
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceStaves) {
    gLogOstream <<
      "Finalizing staff \"" <<
      getStaffName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  // finalize the voices
#ifdef TRACE_OAH
  if (gTraceOah->fTraceStaves || gTraceOah->fTraceVoices) {
    gLogOstream <<
      "Finalizing the voices in staff \"" <<
      getStaffName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
    i != fStaffAllVoicesMap.end ();
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
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrStaff::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrStaff>*
    p =
      dynamic_cast<visitor<S_msrStaff>*> (v)) {
        S_msrStaff elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrStaff::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrStaff::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrStaff::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrStaff>*
    p =
      dynamic_cast<visitor<S_msrStaff>*> (v)) {
        S_msrStaff elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrStaff::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrStaff::browseData (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
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
  if (fStaffAllVoicesMap.size ()) {
    for (
      map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin ();
      i != fStaffAllVoicesMap.end ();
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

  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% <== msrStaff::browseData ()" <<
      endl;
  }
}

string msrStaff::staffKindAsString (
  msrStaffKind staffKind)
{
  string result;

  switch (staffKind) {
    case msrStaff::kStaffRegular:
      result = "staffRegular";
      break;
    case msrStaff::kStaffTablature:
      result = "staffTablature";
      break;
    case msrStaff::kStaffHarmony:
      result = "staffHarmony";
      break;
    case msrStaff::kStaffFiguredBass:
      result = "staffFiguredBass bass";
      break;
    case msrStaff::kStaffDrum:
      result = "staffDrum";
      break;
    case msrStaff::kStaffRythmic:
      result = "staffRythmic";
      break;
  } // switch

  return result;
}

string msrStaff::staffKindAsString () const
{
  return staffKindAsString (fStaffKind);
}

void msrStaff::print (ostream& os)
{
  os <<
    "Staff " << getStaffName () <<
    ", " << staffKindAsString () <<
    ", " <<
    singularOrPlural (
      fStaffAllVoicesMap.size (), "voice", "voices") <<
    ", " <<
    singularOrPlural (
      fStaffRegularVoicesCounter,
      "regular voice",
      "regular voices") << // JMI
    ")" <<
    endl;

  gIndenter++;

  const int fieldWidth = 28;

  os <<
    setw (fieldWidth) <<
    "staffNumber" << " : " <<
    fStaffNumber <<
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
    "staffCurrentPositionInMeasure" << " : " <<
    fStaffCurrentPositionInMeasure <<
    endl;

  // print current the staff clef if any
#ifdef TRACE_OAH
  if (gTraceOah->fTraceClefs) {
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceKeys) {
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTimes) {
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceStaffDetails) {
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

  // print the all voices map
  if (fStaffAllVoicesMap.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "staffAllVoicesMap" << " : " <<
      endl;

    gIndenter++;

    map<int, S_msrVoice>::const_iterator
      iBegin = fStaffAllVoicesMap.begin (),
      iEnd   = fStaffAllVoicesMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      int        voiceNumber = (*i).first;
      S_msrVoice voice       = (*i).second;

        os <<
          voiceNumber << " : " <<
          "regularVoiceStaffSequentialNumber = " <<
          voice->getRegularVoiceStaffSequentialNumber () <<
          ", " <<
          voice->asShortString ();
        if (++i == iEnd) break;
        os << endl;
    } // for
    os << endl;

    gIndenter--;
  }

  // print the regular voices map
  if (fStaffAllVoicesMap.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "staffRegularVoicesMap" << " : " <<
      endl;

    gIndenter++;

    map<int, S_msrVoice>::const_iterator
      iBegin = fStaffRegularVoicesMap.begin (),
      iEnd   = fStaffRegularVoicesMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      if (i == iEnd) break; // JMI ???

      int        voiceNumber = (*i).first;
      S_msrVoice voice       = (*i).second;

      // sanity check
      msrAssert (
        voice != nullptr,
        "voice is null");

      os <<
        voiceNumber << " : " <<
        "regularVoiceStaffSequentialNumber = " <<
        voice->getRegularVoiceStaffSequentialNumber () <<
        ", " <<
        voice->asShortString ();
      if (++i == iEnd) break;
      os << endl;
    } // for
    os << endl;

    gIndenter--;
  }

  // print the regular voices list
  if (fStaffRegularVoicesList.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "staffRegularVoicesList" << " : " <<
      endl;

    gIndenter++;

    list<S_msrVoice>::const_iterator
      iBegin = fStaffRegularVoicesList.begin (),
      iEnd   = fStaffRegularVoicesList.end (),
      i      = iBegin;

    int voiceNumber = 0;

    for ( ; ; ) {
      if (i == iEnd) break; // JMI ???

      S_msrVoice voice = (*i);

      // sanity check
      msrAssert (
        voice != nullptr,
        "voice is null");

      voiceNumber++;

      os <<
        voiceNumber << " : " <<
        "regularVoiceStaffSequentialNumber = " <<
        voice->getRegularVoiceStaffSequentialNumber () <<
        ", " <<
        voice->asShortString ();
      if (++i == iEnd) break;
      os << endl;
    } // for
    os << endl;

    gIndenter--;
  }

  // print the  voices
  if (fStaffAllVoicesMap.size ()) {
    os << endl;

    map<int, S_msrVoice>::const_iterator
      iBegin = fStaffAllVoicesMap.begin (),
      iEnd   = fStaffAllVoicesMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrVoice voice = (*i).second;

/* JMI
os <<
  endl <<
  "================= voice :" <<
  endl <<
  voice <<
  endl <<
  endl;
*/

      os <<
        voice;

        /* JMI
      switch (voice->getVoiceKind ()) {
        case msrVoice::kVoiceRegular:
          os <<
            voice;
          break;

        case msrVoice::kVoiceHarmony:
          if (
            gMsrOah->fShowHarmonyVoices
              ||
            voice->getMusicHasBeenInsertedInVoice ())
            os <<
              voice;
          break;

        case msrVoice::kVoiceFiguredBass:
          if (
            gMsrOah->fShowFiguredBassVoices
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

void msrStaff::printSummary (ostream& os)
{
  os <<
    "Staff" " " << getStaffName () <<
    ", " << staffKindAsString () <<
    " (" <<
    singularOrPlural (
      fStaffAllVoicesMap.size (), "voice", "voices") <<
    ")" <<
    endl;

  gIndenter++;

  os <<
    "StaffInstrumentName: \"" <<
    fStaffInstrumentName << "\"" <<
    "staffCurrentPositionInMeasure: " <<
    fStaffCurrentPositionInMeasure <<
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
*/

  // print the voices names
  if (fStaffAllVoicesMap.size ()) {
    os <<
      "Voices:" <<
      endl;

    gIndenter++;

    map<int, S_msrVoice>::const_iterator
      iBegin = fStaffAllVoicesMap.begin (),
      iEnd   = fStaffAllVoicesMap.end (),
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

    gIndenter --;
  }

  gIndenter--;

  os << endl;
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
  assert(o!=0);
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceStaves || gTraceOah->fTraceVoices) {
    gLogOstream <<
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
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrVoiceStaffChange::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrVoiceStaffChange>*
    p =
      dynamic_cast<visitor<S_msrVoiceStaffChange>*> (v)) {
        S_msrVoiceStaffChange elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrVoiceStaffChange::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrVoiceStaffChange::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrVoiceStaffChange::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrVoiceStaffChange>*
    p =
      dynamic_cast<visitor<S_msrVoiceStaffChange>*> (v)) {
        S_msrVoiceStaffChange elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
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

void msrVoiceStaffChange::print (ostream& os)
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
