/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iostream>
#include <fstream>

#include "versions.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "msdlParser.h"

#include "msdl2msdrOah.h"

#include "msrInterface.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
S_msrScore msdlParser::createBook ()
{
  // create the book
  S_msrBook
    theMsrBook =
      msrBook::create (__LINE__);

  return theMsrBook;
}

//_______________________________________________________________________________
S_msrScore msdlParser::createScore ()
{
  // create the score
  S_msrScore
    theMsrScore =
      msrScore::create (__LINE__);

  // create its identification
  S_msrIdentification
    identification =
      msrIdentification::create (
        __LINE__);

  theMsrScore->
    setIdentification (
      identification);

  // set the identification's work title
  identification->
    setWorkTitle (
      __LINE__,
      "Mikrokosmos III Wandering");

  // append the identification's composer
  identification->
    appendComposer (
      __LINE__,
      "Zoltan Kodaly");

  // set the identification's software
  identification->
    appendSoftware (
      __LINE__,
      "MSDL compiler"); // JMI

  return theMsrScore;
}

//_______________________________________________________________________________
S_msrPart msdlParser::createPartInScore (
  S_msrScore theMsrScore)
{
  // create the part group
  S_msrPartGroup
    partGroup =
      msrPartGroup::create (
        __LINE__, 1, 1, "OnlyPartGroup", nullptr, theMsrScore);

  // add it to the score
  theMsrScore->
    addPartGroupToScore (
      partGroup);

  // create the part
  S_msrPart
    part =
      msrPart::create (
        __LINE__, "OnlyPart", partGroup);

  // append it to the part group
  partGroup->
    appendPartToPartGroup (
      part);

if (false) { // JMI TEMP
  part->
    setPartShortestNoteDuration (
      rational (1, 16));
}

  return part;
}

//_______________________________________________________________________________
S_msrStaff msdlParser::createStaffInPart (
  int staffNumber, S_msrPart part)
{
  // create the staff
  S_msrStaff
    staff =
      msrStaff::create (
        __LINE__, kStaffRegular, staffNumber, part);

  // append it to the part
  part ->
    addStaffToPartCloneByItsNumber ( // JMI NOT clone???
      staff);

  return staff;
}

//_______________________________________________________________________________
S_msrVoice msdlParser::createRegularVoiceInStaff (
  int          inputLineNumber,
  int          voiceNumber,
  S_msrStaff   staff)
{
  // create the functions voice
  S_msrVoice
    voice =
      msrVoice::createRegularVoice (
        inputLineNumber,
        voiceNumber,
        staff);

  // append it to the staff
  staff->
    registerVoiceInStaff (
      inputLineNumber,
      voice);

  return voice;
}

//_______________________________________________________________________________
S_msrMeasure msdlParser::createMeasureInVoice (
  int        inputLineNumber,
  string     measureNumber,
  S_msrVoice voice)
{
  return
    voice->
      createMeasureAndAppendItToVoice (
        inputLineNumber,
        measureNumber,
        kMeasureImplicitKindNo);
}

/*
  // measure 1
  // ----------------------------------

  {
    // create the measure

    string measure1number = "1"; // the measure number is a string;

    S_msrMeasure
      measure1 =
        createMeasureInVoice (
          __LINE__,
          measure1number,
          upperVoice1);

    // append the clef

    measure1->
      appendClefToMeasure (
        msrClef::create (
          __LINE__,
          kTrebleClef,
          1));

    // append the key

    measure1->
      appendKeyToMeasure (
        msrKey::createTraditional (
          __LINE__,
          kC_Natural_QTP,
          kMajorMode,
          0)); // keyCancel JMI

    // append the time

    measure1->
      appendTimeToMeasure (
        msrTime::createTwoQuartersTime (
          __LINE__));

    // append the notes

    {
      S_msrNote
        note1 =
          msrNote::createRegularNote (
            __LINE__,
            measure1number,
            kA_Natural_QTP,
            kOctave4,
            rational (1, 8), // soundingWholeNotes
            rational (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the dynamics to the note
      note1->
        appendDynamicsToNote (
          msrDynamics::create (
            __LINE__,
            kDynamicsPP,
            kPlacementBelow));

      // append the slur start to the note
      note1->
        appendSlurToNote (
          msrSlur::create (
            __LINE__,
            1, // slurNumber
            kRegularSlurStart,
            kLineTypeSolid,
            kPlacementBelow));

      // append the first beam begin to the note
      note1->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            kBeamBegin));

      // append the note to the measure
      measure1->
        appendNoteOrPaddingToMeasure (note1);
    }
*/




}
