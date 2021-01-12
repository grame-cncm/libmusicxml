/*

  Copyright (C) 2003-2008  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the MusicXML Library use.
  It shows how to build a memory representation of a score from scratch.

*/

#include <signal.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <fstream>

#include "libmusicxml.h" // for xmlErr

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "musicxmlOah.h"
#include "mxmlTreeOah.h"
#include "msrOah.h"

#include "lpsrOah.h"
#include "lpsr2lilypondOah.h"

#include "bsrOah.h"
#include "bsr2brailleOah.h"

#include "msr2lilypond.h"
#include "msr2musicxml.h"
#include "msr2braille.h"
#include "msr2guido.h"

#include "Mikrokosmos3WanderingInsiderHandler.h"
#include "Mikrokosmos3WanderingRegularHandler.h"


using namespace MusicXML2;

/*
  ENFORCE_TRACE_OAH can be used to issue trace messages
  before gGlobalOahOahGroup->fTrace has been initialized
*/
#define ENFORCE_TRACE_OAH

//_______________________________________________________________________________
#ifndef WIN32
static void _sigaction (int signal, siginfo_t *si, void *arg)
{
  cerr << "Signal #" << signal << " catched!" << endl;
  exit (-2);
}

static void catchSignals ()
{
	struct sigaction sa;

  memset (&sa, 0, sizeof(struct sigaction));

  sigemptyset (&sa.sa_mask);

  sa.sa_sigaction = _sigaction;
  sa.sa_flags     = SA_SIGINFO;

  sigaction (SIGSEGV, &sa, NULL);
  sigaction (SIGILL,  &sa, NULL);
  sigaction (SIGFPE,  &sa, NULL);
}
#else
static void catchSignals () {}
#endif

//_______________________________________________________________________________
static void argvElements2stringsVector (
  int argc, char *argv[],
  vector<string>& stringsVector)
{
	for (int i=1; i<argc; ++i) {
		stringsVector.push_back (argv [i]);
	} // for
}

//_______________________________________________________________________________
static bool arguments2optionsVector (int argc, char *argv[], optionsVector& theOptionsVector)
{
  // create a strings vector from the elements in argv
	vector<string> stringsVector;

	argvElements2stringsVector (argc, argv, stringsVector);

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  cerr << "arguments2optionsVector: stringsVector size: " << stringsVector.size() << endl;
	cerr << "==> stringsVector:" << endl;
	for (auto str: stringsVector) {
	  cerr << "   " << str << endl;
	} // for
	cerr << endl;
#endif
#endif

  // populate the optionsVector
  // ------------------------------------------------------

	string curOption;

	for (unsigned int i = 0; i < stringsVector.size (); ++i) {
	  string str = stringsVector [i];

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
	  cerr << "--> curOption: " << curOption << endl;
	  cerr << "--> str      : " << str << endl;
#endif
#endif

		if (curOption.empty ()) {	// wait for option
			if (str [0] == '-') {
			  curOption = str;
      }
			else {
			  return false;
      }
		}

		else {
			if (str [0] == '-') {
				// option without value
				theOptionsVector.push_back (make_pair (curOption, ""));
				curOption = str;
			}
			else {
			  // option with value
				theOptionsVector.push_back (make_pair(curOption, str));
				curOption = "";
			}
		}
	} // for

	if (curOption.size())
		theOptionsVector.push_back (make_pair (curOption, ""));

	return true;
}

//------------------------------------------------------------------------
static S_msrScore createTheScore (
  msrGenerationAPIKind generationAPIKind)
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
      "Mikrokosmos III Wandering - " +
        msrGenerationAPIKindAsString (
          generationAPIKind));

  return theMsrScore;
}

//------------------------------------------------------------------------
static S_msrPart createPartInScore (S_msrScore theMsrScore)
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

//------------------------------------------------------------------------
static S_msrStaff createStaffInPart (int staffNumber, S_msrPart part)
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

//------------------------------------------------------------------------
static S_msrVoice createRegularVoiceInStaff (
  int          inputLineNumber,
  int          voiceNumber,
  S_msrStaff   staff)
{
  // create the regular voice
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

//------------------------------------------------------------------------
static S_msrMeasure createMeasureInVoice (
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

//------------------------------------------------------------------------
// upperVoice1 - Regular API
//------------------------------------------------------------------------
static void populateUpperVoice1WithTheRegularAPI (
  S_msrVoice upperVoice1)
{
  // measure 1
  // ----------------------------------

  {
    // create it
    string measure1number = "1"; // the measure number is a string;

    S_msrMeasure
      measure1 =
        createMeasureInVoice (
          __LINE__,
          measure1number,
          upperVoice1);

    // clef
    measure1->
      appendClefToMeasure (
        msrClef::create (
          __LINE__,
          kTrebleClef,
          1));

    // key
    measure1->
      appendKeyToMeasure (
        msrKey::createTraditional (
          __LINE__,
          kC_Natural_QTP,
          kMajorMode,
          0)); // keyCancel JMI

    // time
    measure1->
      appendTimeToMeasure (
        msrTime::createTwoQuartersTime (
          __LINE__));

    // notes
    measure1->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure1number,
          kA_Natural_QTP,
          kOctave4,
          rational (1, 8), // soundingWholeNotes
          rational (1, 8), // displayWholeNotes
          0));             // dotsNumber

    measure1->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure1number,
          kG_Natural_QTP,
          kOctave4,
          rational (1, 8), // soundingWholeNotes
          rational (1, 8), // displayWholeNotes
          0));             // dotsNumber

    measure1->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure1number,
          kF_Natural_QTP,
          kOctave4,
          rational (1, 8), // soundingWholeNotes
          rational (1, 8), // displayWholeNotes
          0));             // dotsNumber

    measure1->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure1number,
          kE_Natural_QTP,
          kOctave4,
          rational (1, 8), // soundingWholeNotes
          rational (1, 8), // displayWholeNotes
          0));             // dotsNumber
  }

 return; // QUIT

  // measure 2
  // ----------------------------------

  {
    // create it
    string measure2number = "2"; // the measure number is a string;

    S_msrMeasure
      measure2 =
        createMeasureInVoice (
          __LINE__,
          measure2number,
          upperVoice1);

    // time
    measure2->
      appendTimeToMeasure (
        msrTime::createThreeQuartersTime (
          __LINE__));

    // notes
    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure2number,
          kD_Natural_QTP,
          kOctave4,
          rational (1, 4), // soundingWholeNotes
          rational (1, 4), // displayWholeNotes
          0));             // dotsNumber

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure2number,
          kG_Natural_QTP,
          kOctave4,
          rational (1, 8), // soundingWholeNotes
          rational (1, 8), // displayWholeNotes
          0));             // dotsNumber

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure2number,
          kF_Natural_QTP,
          kOctave4,
          rational (1, 8), // soundingWholeNotes
          rational (1, 8), // displayWholeNotes
          0));             // dotsNumber

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure2number,
          kE_Natural_QTP,
          kOctave4,
          rational (1, 8), // soundingWholeNotes
          rational (1, 8), // displayWholeNotes
          0));             // dotsNumber

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure2number,
          kD_Natural_QTP,
          kOctave4,
          rational (1, 8), // soundingWholeNotes
          rational (1, 8), // displayWholeNotes
          0));             // dotsNumber
  }


  // measure 3
  // ----------------------------------

  {
    // create it
    string measure3number = "3"; // the measure number is a string;

    S_msrMeasure
      measure3 =
        createMeasureInVoice (
          __LINE__,
          measure3number,
          upperVoice1);

    // time
    measure3->
      appendTimeToMeasure (
        msrTime::createTwoQuartersTime (
          __LINE__));

    // notes
    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure3number,
          kC_Natural_QTP,
          kOctave4,
          rational (1, 8), // soundingWholeNotes
          rational (1, 8), // displayWholeNotes
          0));             // dotsNumber

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure3number,
          kD_Natural_QTP,
          kOctave4,
          rational (1, 16), // soundingWholeNotes
          rational (1, 16), // displayWholeNotes
          0));              // dotsNumber

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure3number,
          kE_Natural_QTP,
          kOctave4,
          rational (1, 16), // soundingWholeNotes
          rational (1, 16), // displayWholeNotes
          0));              // dotsNumber

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure3number,
          kF_Natural_QTP,
          kOctave4,
          rational (1, 8), // soundingWholeNotes
          rational (1, 8), // displayWholeNotes
          0));             // dotsNumber

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure3number,
          kE_Natural_QTP,
          kOctave4,
          rational (1, 8), // soundingWholeNotes
          rational (1, 8), // displayWholeNotes
          0));             // dotsNumber
  }


  // measure 4
  // ----------------------------------

  {
    // create it
    string measure4number = "4"; // the measure number is a string;

    S_msrMeasure
      measure4 =
          createMeasureInVoice (
            __LINE__,
            measure4number,
            upperVoice1);

    // notes
    measure4->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure4number,
          kD_Natural_QTP,
          kOctave4,
          rational (1, 2), // soundingWholeNotes
          rational (1, 2), // displayWholeNotes
          0));             // dotsNumber
  }

  // measure 5
  // ----------------------------------

  {
    // create it
    string measure5number = "5"; // the measure number is a string;

    S_msrMeasure
      measure5 =
        createMeasureInVoice (
          __LINE__,
          measure5number,
          upperVoice1);

    // notes
    measure5->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure5number,
          kD_Natural_QTP,
          kOctave4,
          rational (1, 2), // soundingWholeNotes
          rational (1, 2), // displayWholeNotes
          0));             // dotsNumber

    // final barline
    measure5->
      appendBarlineToMeasure (
        msrBarline::createFinalBarline (
          __LINE__));
  }
}

//------------------------------------------------------------------------
// upperVoice1 - Strings API
//------------------------------------------------------------------------
static void populateUpperVoice1WithTheStringsAPI (
  S_msrVoice upperVoice1)
{
/*
void msrVoice::appendSegmentToVoice ( //JMI VIRER???
  S_msrSegment segment)
*/

  // measure 1
  // ----------------------------------

  {
    // create it
    string measure1number = "1"; // the measure number is a string;

    S_msrMeasure
      measure1 =
        createMeasureInVoice (
          __LINE__,
          measure1number,
          upperVoice1);

    // clef
    measure1->
      appendClefToMeasure (
        msrClef::createClefFromString (
          __LINE__,
          "treble",
          1));

    // key
    measure1->
      appendKeyToMeasure (
        msrKey::createTraditional (
          __LINE__,
          kC_Natural_QTP,
          kMajorMode,
          0)); // keyCancel JMI

    // time
    measure1->
      appendTimeToMeasure (
        msrTime::createTimeFromString (
          __LINE__,
          "2/4"));

    // notes
    measure1->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "a'8",
          measure1number));

    measure1->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "g'8",
          measure1number));

    measure1->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "f'8",
          measure1number));

    measure1->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "e'8",
          measure1number));
  }


 return; // QUIT

  // measure 2
  // ----------------------------------

  {
    // create it
    string measure2number = "2"; // the measure number is a string;

    S_msrMeasure
      measure2 =
        createMeasureInVoice (
          __LINE__,
          measure2number,
          upperVoice1);

    // time
    measure2->
      appendTimeToMeasure (
        msrTime::createTimeFromString (
          __LINE__,
          "3/4"));

    // notes
    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "d'4",
          measure2number));

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "g'8",
          measure2number));

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "f'8",
          measure2number));

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "e'8",
          measure2number));

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "d'8",
          measure2number));
  }


  // measure 3
  // ----------------------------------

  {
    // create it
    string measure3number = "3"; // the measure number is a string;

    S_msrMeasure
      measure3 =
        createMeasureInVoice (
          __LINE__,
          measure3number,
          upperVoice1);

    // time
    measure3->
      appendTimeToMeasure (
        msrTime::createTimeFromString (
          __LINE__,
          "2/4"));

    // notes
    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "c'8",
          measure3number));

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "d'16",
          measure3number));

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "e'16",
          measure3number));

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "f'8",
          measure3number));

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "e'8",
          measure3number));
  }


  // measure 4
  // ----------------------------------

  {
    // create it
    string measure4number = "4"; // the measure number is a string;

    S_msrMeasure
      measure4 =
        createMeasureInVoice (
          __LINE__,
          measure4number,
          upperVoice1);

    // notes
    measure4->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "d'2",
          measure4number));
  }


  // measure 5
  // ----------------------------------

  {
    // create it
    string measure5number = "5"; // the measure number is a string;

    S_msrMeasure
      measure5 =
        createMeasureInVoice (
          __LINE__,
          measure5number,
          upperVoice1);

    // notes
    measure5->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "d'2",
          measure5number));

    // final barline
    measure5->
      appendBarlineToMeasure (
        msrBarline::createFinalBarline (
          __LINE__));
  }
}

//------------------------------------------------------------------------
// lowerVoice1 - Regular API
//------------------------------------------------------------------------
static void populateLowerVoice1WithTheRegularAPI (
  S_msrVoice lowerVoice1)
{
  // measure 1
  // ----------------------------------

  {
    // create it
    string measure1number = "1"; // the measure number is a string;

    S_msrMeasure
      measure1 =
        createMeasureInVoice (
          __LINE__,
          measure1number,
          lowerVoice1);

    // clef
    measure1->
      appendClefToMeasure (
        msrClef::create (
          __LINE__,
          kTrebleClef,
          2));

    // key
    measure1->
      appendKeyToMeasure (
        msrKey::createTraditional (
          __LINE__,
          kC_Natural_QTP,
          kMajorMode,
          0)); // keyCancel JMI

    // time
    measure1->
      appendTimeToMeasure (
        msrTime::createTwoQuartersTime (
          __LINE__));

    // notes
    measure1->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure1number,
          kB_Natural_QTP,
          kOctave3,
          rational (1, 4), // soundingWholeNotes
          rational (1, 4), // displayWholeNotes
          0));             // dotsNumber

    measure1->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure1number,
          kD_Natural_QTP,
          kOctave4,
          rational (1, 8), // soundingWholeNotes
          rational (1, 8), // displayWholeNotes
          0));             // dotsNumber
    measure1->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure1number,
          kC_Natural_QTP,
          kOctave4,
          rational (1, 8), // soundingWholeNotes
          rational (1, 8), // displayWholeNotes
          0));             // dotsNumber
  }

 return; // QUIT

  // measure 2
  // ----------------------------------

  {
    // create it
    string measure2number = "2"; // the measure number is a string;

    S_msrMeasure
      measure2 =
        createMeasureInVoice (
          __LINE__,
          measure2number,
          lowerVoice1);

    // time
    measure2->
      appendTimeToMeasure (
        msrTime::createThreeQuartersTime (
          __LINE__));

    // notes
    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure2number,
          kB_Natural_QTP,
          kOctave3,
          rational (1, 8), // soundingWholeNotes
          rational (1, 8), // displayWholeNotes
          0));             // dotsNumber

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure2number,
          kA_Natural_QTP,
          kOctave3,
          rational (1, 8), // soundingWholeNotes
          rational (1, 8), // displayWholeNotes
          0));             // dotsNumber

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure2number,
          kA_Flat_QTP,
          kOctave3,
          rational (1, 4), // soundingWholeNotes
          rational (1, 4), // displayWholeNotes
          0));             // dotsNumber

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure2number,
          kC_Natural_QTP,
          kOctave4,
          rational (1, 8), // soundingWholeNotes
          rational (1, 8), // displayWholeNotes
          0));             // dotsNumber

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure2number,
          kB_Flat_QTP,
          kOctave3,
          rational (1, 8), // soundingWholeNotes
          rational (1, 8), // displayWholeNotes
          0));             // dotsNumber
  }

  // measure 3
  // ----------------------------------

  {
    // create it
    string measure3number = "3"; // the measure number is a string;

    S_msrMeasure
      measure3 =
        createMeasureInVoice (
          __LINE__,
          measure3number,
          lowerVoice1);

    // time
    measure3->
      appendTimeToMeasure (
        msrTime::createTwoQuartersTime (
          __LINE__));

    // notes
    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure3number,
          kA_Natural_QTP,
          kOctave3,
          rational (1, 8), // soundingWholeNotes
          rational (1, 8), // displayWholeNotes
          0));             // dotsNumber

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure3number,
          kG_Natural_QTP,
          kOctave3,
          rational (1, 8), // soundingWholeNotes
          rational (1, 8), // displayWholeNotes
          0));             // dotsNumber

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure3number,
          kF_Sharp_QTP,
          kOctave3,
          rational (1, 8), // soundingWholeNotes
          rational (1, 8), // displayWholeNotes
          0));             // dotsNumber

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure3number,
          kG_Natural_QTP,
          kOctave3,
          rational (1, 16), // soundingWholeNotes
          rational (1, 16), // displayWholeNotes
          0));              // dotsNumber

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure3number,
          kA_Natural_QTP,
          kOctave3,
          rational (1, 16), // soundingWholeNotes
          rational (1, 16), // displayWholeNotes
          0));              // dotsNumber
  }

  // measure 4
  // ----------------------------------

  {
    // create it
    string measure4number = "4"; // the measure number is a string;

    S_msrMeasure
      measure4 =
        createMeasureInVoice (
          __LINE__,
          measure4number,
          lowerVoice1);

    // notes
    measure4->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure4number,
          kB_Natural_QTP,
          kOctave3,
          rational (1, 2), // soundingWholeNotes
          rational (1, 2), // displayWholeNotes
          0));             // dotsNumber
  }

  // measure 5
  // ----------------------------------

  {
    // create it
    string measure5number = "5"; // the measure number is a string;

    S_msrMeasure
      measure5 =
        createMeasureInVoice (
          __LINE__,
          measure5number,
          lowerVoice1);

    // notes
    measure5->
      appendNoteOrPaddingToMeasure (
        msrNote::createRegularNote (
          __LINE__,
          measure5number,
          kB_Natural_QTP,
          kOctave3,
          rational (1, 2), // soundingWholeNotes
          rational (1, 2), // displayWholeNotes
          0));             // dotsNumber

    // final barline
    measure5->
      appendBarlineToMeasure (
        msrBarline::createFinalBarline (
          __LINE__));
  }
}

//------------------------------------------------------------------------
// lowerVoice1 - Strings API
//------------------------------------------------------------------------
static void populateLowerVoice1WithTheStringsAPI (
  S_msrVoice lowerVoice1)
{
  // measure 1
  // ----------------------------------

  {
    // create it
    string measure1number = "1"; // the measure number is a string;

    S_msrMeasure
      measure1 =
        createMeasureInVoice (
          __LINE__,
          measure1number,
          lowerVoice1);

    // clef
    measure1->
      appendClefToMeasure (
        msrClef::createClefFromString (
          __LINE__,
          "bass",
          2));

    // key
    measure1->
      appendKeyToMeasure (
        msrKey::createTraditional (
          __LINE__,
          kC_Natural_QTP,
          kMajorMode,
          0)); // keyCancel JMI

    // time
    measure1->
      appendTimeToMeasure (
        msrTime::createTimeFromString (
          __LINE__,
          "2/4"));

    // notes
    measure1->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "b4",
          measure1number));

    measure1->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "d'8",
          measure1number));

    measure1->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "c'8",
          measure1number));
  }

 return; // QUIT

  // measure 2
  // ----------------------------------

  {
    // create it
    string measure2number = "2"; // the measure number is a string;

    S_msrMeasure
      measure2 =
        createMeasureInVoice (
          __LINE__,
          measure2number,
          lowerVoice1);

    // time
    measure2->
      appendTimeToMeasure (
        msrTime::createTimeFromString (
          __LINE__,
          "3/4"));

    // notes
    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "b8",
          measure2number));

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "a8",
          measure2number));

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "aes4",
          measure2number));

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "c'8",
          measure2number));

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "bes8",
          measure2number));
  }

  // measure 3
  // ----------------------------------

  {
    // create it
    string measure3number = "3"; // the measure number is a string;

    S_msrMeasure
      measure3 =
        createMeasureInVoice (
          __LINE__,
          measure3number,
          lowerVoice1);

    // time
    measure3->
      appendTimeToMeasure (
        msrTime::createTimeFromString (
          __LINE__,
          "2/4"));

    // notes
    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "a8",
          measure3number));

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "g8",
          measure3number));

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "fis8",
          measure3number));

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "g16",
          measure3number));

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "a16",
          measure3number));
  }

  // measure 4
  // ----------------------------------

  {
    // create it
    string measure4number = "4"; // the measure number is a string;

    S_msrMeasure
      measure4 =
        createMeasureInVoice (
          __LINE__,
          measure4number,
          lowerVoice1);

    // notes
    measure4->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "b2",
          measure4number));
  }

  // measure 5
  // ----------------------------------

  {
    // create it
    string measure5number = "5"; // the measure number is a string;

    S_msrMeasure
      measure5 =
        createMeasureInVoice (
          __LINE__,
          measure5number,
          lowerVoice1);

    // notes
    measure5->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "b2",
          measure5number));

    // final barline
    measure5->
      appendBarlineToMeasure (
        msrBarline::createFinalBarline (
          __LINE__));
  }
}

//------------------------------------------------------------------------
// lowerVoice2 - Regular API
//------------------------------------------------------------------------
static void populateLowerVoice2WithTheRegularAPI (
  S_msrVoice lowerVoice2)
{
  // measure 1
  // ----------------------------------

  {
    // create it
    string measure1number = "1"; // the measure number is a string;

    S_msrMeasure
      measure1 =
        createMeasureInVoice (
          __LINE__,
          measure1number,
          lowerVoice2);

    // clef
    measure1->
      appendClefToMeasure (
        msrClef::create (
          __LINE__,
          kBassClef,
          2));

    // key
    measure1->
      appendKeyToMeasure (
        msrKey::createTraditional (
          __LINE__,
          kC_Natural_QTP,
          kMajorMode,
          0)); // keyCancel JMI

    // time
    measure1->
      appendTimeToMeasure (
        msrTime::createTwoQuartersTime (
          __LINE__));

    // notes
    measure1->
      appendNoteOrPaddingToMeasure (
        msrNote::createSkipNote (
          __LINE__,
          measure1number,
          rational (1, 2), // soundingWholeNotes
          rational (1, 2), // displayWholeNotes
          0));             // dotsNumber
  }

 return; // QUIT

  // measure 2
  // ----------------------------------

  {
    // create it
    string measure2number = "2"; // the measure number is a string;

    S_msrMeasure
      measure2 =
        createMeasureInVoice (
          __LINE__,
          measure2number,
          lowerVoice2);

    // time
    measure2->
      appendTimeToMeasure (
        msrTime::createThreeQuartersTime (
          __LINE__));

    // notes
    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createSkipNote (
          __LINE__,
          measure2number,
          rational (3, 4), // soundingWholeNotes
          rational (2, 4), // displayWholeNotes
          0));             // dotsNumber
  }

  // measure 3
  // ----------------------------------

  {
    // create it
    string measure3number = "3"; // the measure number is a string;

    S_msrMeasure
      measure3 =
        createMeasureInVoice (
          __LINE__,
          measure3number,
          lowerVoice2);

    // time
    measure3->
      appendTimeToMeasure (
        msrTime::createTwoQuartersTime (
          __LINE__));

    // notes
    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createSkipNote (
          __LINE__,
          measure3number,
          rational (1, 2), // soundingWholeNotes
          rational (1, 2), // displayWholeNotes
          0));             // dotsNumber
  }

  // measure 4
  // ----------------------------------

  {
    // create it
    string measure4number = "4"; // the measure number is a string;

    S_msrMeasure
      measure4 =
        createMeasureInVoice (
          __LINE__,
          measure4number,
          lowerVoice2);

    // notes
    measure4->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "b4",
          measure4number));

    measure4->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "a4",
          measure4number));
  }

  // measure 5
  // ----------------------------------

  {
    // create it
    string measure5number = "5"; // the measure number is a string;

    S_msrMeasure
      measure5 =
        createMeasureInVoice (
          __LINE__,
          measure5number,
          lowerVoice2);

    // notes
    measure5->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "g2",
          measure5number));

    // final barline
    measure5->
      appendBarlineToMeasure (
        msrBarline::createFinalBarline (
          __LINE__));
  }
}

//------------------------------------------------------------------------
// lowerVoice2 - Strings API
//------------------------------------------------------------------------
static void populateLowerVoice2WithTheStringsAPI (
  S_msrVoice lowerVoice2)
{
  // measure 1
  // ----------------------------------

  {
    // create it
    string measure1number = "1"; // the measure number is a string;

    S_msrMeasure
      measure1 =
        createMeasureInVoice (
          __LINE__,
          measure1number,
          lowerVoice2);

    // clef
    measure1->
      appendClefToMeasure (
        msrClef::createClefFromString (
          __LINE__,
          "bass",
          2));

    // key
    measure1->
      appendKeyToMeasure (
        msrKey::createTraditional (
          __LINE__,
          kC_Natural_QTP,
          kMajorMode,
          0)); // keyCancel JMI

    // time
    measure1->
      appendTimeToMeasure (
        msrTime::createTimeFromString (
          __LINE__,
          "2/4"));

    // notes
    measure1->
      appendNoteOrPaddingToMeasure (
        msrNote::createSkipFromString (
          __LINE__,
          "s2",
          measure1number));
  }

 return; // QUIT

  // measure 2
  // ----------------------------------

  {
    // create it
    string measure2number = "2"; // the measure number is a string;

    S_msrMeasure
      measure2 =
        createMeasureInVoice (
          __LINE__,
          measure2number,
          lowerVoice2);

    // time
    measure2->
      appendTimeToMeasure (
        msrTime::createTimeFromString (
          __LINE__,
          "3/4"));

    // notes
    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createSkipFromString (
          __LINE__,
          "s2.",
          measure2number));
  }

  // measure 3
  // ----------------------------------

  {
    // create it
    string measure3number = "3"; // the measure number is a string;

    S_msrMeasure
      measure3 =
        createMeasureInVoice (
          __LINE__,
          measure3number,
          lowerVoice2);

    // time
    measure3->
      appendTimeToMeasure (
        msrTime::createTimeFromString (
          __LINE__,
          "2/4"));

    // notes
    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createSkipFromString (
          __LINE__,
          "s2",
          measure3number));
  }

  // measure 4
  // ----------------------------------

  {
    // create it
    string measure4number = "4"; // the measure number is a string;

    S_msrMeasure
      measure4 =
        createMeasureInVoice (
          __LINE__,
          measure4number,
          lowerVoice2);

    // notes
    measure4->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "b4",
          measure4number));

    measure4->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "a4",
          measure4number));
  }

  // measure 5
  // ----------------------------------

  {
    // create it
    string measure5number = "5"; // the measure number is a string;

    S_msrMeasure
      measure5 =
        createMeasureInVoice (
          __LINE__,
          measure5number,
          lowerVoice2);

    // notes
    measure5->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "g2",
          measure5number));

    // final barline
    measure5->
      appendBarlineToMeasure (
        msrBarline::createFinalBarline (
          __LINE__));
  }
}

//------------------------------------------------------------------------
static S_msrStaff createAndPopulateUpperStaffInPart (
  S_msrPart            part,
  msrGenerationAPIKind generationAPIKind)
{
  // create the upper staff
  S_msrStaff
    upperStaff =
      createStaffInPart (
        1, part);

  // create voice 1 in upperStaff
  S_msrVoice
    upperVoice1 =
      createRegularVoiceInStaff (
        __LINE__,
        1,
       upperStaff);

  // populate its voice(s)
  switch (generationAPIKind) {
    case kMsrFunctionsAPIKind:
      populateUpperVoice1WithTheRegularAPI (
        upperVoice1);
      break;

    case kMsrStringsAPIKind:
      populateUpperVoice1WithTheStringsAPI (
        upperVoice1);
      break;
  } // switch

  return upperStaff;
}

//------------------------------------------------------------------------
static S_msrStaff createAndPopulateLowerStaffInPart (
  S_msrPart            part,
  msrGenerationAPIKind generationAPIKind)
{
  // create the lower staff
  S_msrStaff
    lowerStaff =
      createStaffInPart (
        2, part);

  // create voice 1 in lowerStaff
  S_msrVoice
    lowerVoice1 =
      createRegularVoiceInStaff (
        __LINE__,
        1,
       lowerStaff);

  // create voice 2 in lowerStaff
  S_msrVoice
    lowerVoice2 =
      createRegularVoiceInStaff (
        __LINE__,
        2,
       lowerStaff);

  // populate its voice(s)
  // populate its voice(s)
  switch (generationAPIKind) {
    case kMsrFunctionsAPIKind:
      populateLowerVoice1WithTheRegularAPI (
        lowerVoice1);
      populateLowerVoice2WithTheRegularAPI (
        lowerVoice2);
      break;

    case kMsrStringsAPIKind:
      populateLowerVoice1WithTheStringsAPI (
        lowerVoice1);
      populateLowerVoice2WithTheStringsAPI (
        lowerVoice2);
      break;
  } // switch

  return lowerStaff;
}

//------------------------------------------------------------------------
static S_msrScore createAndPopulateTheScore (
  msrGenerationAPIKind generationAPIKind)
{
  S_msrScore
    theMsrScore =
      createTheScore (
        generationAPIKind);

  S_msrPart
    part =
      createPartInScore (theMsrScore);

  // create and populate the two staves in part
  S_msrStaff
    upperStaff =
      createAndPopulateUpperStaffInPart (
        part,
        generationAPIKind);

if (true) // QUIT
  S_msrStaff
    lowerStaff =
      createAndPopulateLowerStaffInPart (
        part,
        generationAPIKind);

  // finalize the part

  part->
    finalizePartAndAllItsMeasures (
      __LINE__);

if (false) // QUIT
  gLogStream <<
    "___PART___" <<
    part <<
    endl;

  if (gGlobalMsrOahGroup->getDisplayMsr ()) {
    // print the score
    gOutputStream <<
      endl <<
      "%--------------------------------------------------------------" <<
      endl <<
      "The Mikrokosmos III Wandering MSR score contains:" <<
      endl <<
      "%--------------------------------------------------------------" <<
      endl << endl;

    ++gIndenter;
    gOutputStream <<
      theMsrScore <<
      endl;
    --gIndenter;
  }

  return theMsrScore;
}

//------------------------------------------------------------------------
static void enforceSomeOptions (generatorOutputKind theGeneratorOutputKind)
{
  /*
    This is a way to enforce options 'permanently'
    independently of the options chosen by the user
  */

  // trace
  // ------------------------------------------------------

/*
  gGlobalTraceOahGroup->setTraceScore ();
  gGlobalTraceOahGroup->setTracePartGroups ();
  gGlobalTraceOahGroup->setTraceParts ();
  gGlobalTraceOahGroup->setTraceStaves ();
  gGlobalTraceOahGroup->setTraceVoices ();
  gGlobalTraceOahGroup->setTraceSegments ();
  gGlobalTraceOahGroup->setTraceMeasures ();
  gGlobalTraceOahGroup->setTraceNotes ();
*/

  // MSR
  // ------------------------------------------------------

/*
  gGlobalMsrOahGroup->setTraceMsr ();
  gGlobalMsrOahGroup->setTraceMsrVisitors ();
  gGlobalMsrOahGroup->setDisplayMsr ();

  gGlobalMsrOahGroup->setTraceMsrDurations ();
*/

  // generate code dependant specific options
  // ------------------------------------------------------

  switch (theGeneratorOutputKind) {
    case k_NoOutput:
      // should not occur
      break;

    case kGuidoOutput:
/*
     gGlobalMusicxmlOahGroup->setTraceDivisions ();
*/
     break;

    case kLilyPondOutput:
      // LPSR
      // ------------------------------------------------------

/*
      gGlobalLpsrOahGroup->setTraceLpsr ();
      gGlobalLpsrOahGroup->setTraceLpsrVisitors ();
      gGlobalLpsrOahGroup->setDisplayLpsr ();
*/

      // lpsr2lilypond
      // ------------------------------------------------------

/*
      gGlobalLpsr2lilypondOahGroup->setLilypondCompileDate (); // JMI NOT OK
      gGlobalLpsr2lilypondOahGroup->setInputLineNumbers ();
*/
      break;

    case kBrailleOutput:
      // BSR
      // ------------------------------------------------------

/*
      gGlobalBsrOahGroup->setTraceBsr ();
      //  gGlobalBsrOahGroup->setTraceBsrVisitors ();
*/

      // bsr2braille
      // ------------------------------------------------------

/*
      gGlobalBsr2brailleOahGroup->
        setBrailleOutputKind (kBrailleOutputUTF8Debug);
*/
      break;

    case kMusicXMLOutput:
      // MusicXML
      // ------------------------------------------------------

/*
      gGlobalMusicxmlOahGroup->setTraceDivisions ();

      gGlobalMxmlTreeOahGroup->setTraceMusicXMLTreeVisitors ();
*/
      break;

    case kMidiOutput:
/*
*/
      break;
  } // switch
}

//------------------------------------------------------------------------
// the main function
//------------------------------------------------------------------------
int main (int argc, char * argv[])
{
  // setup signals catching
  // ------------------------------------------------------

	catchSignals ();

  // the executable name
  // ------------------------------------------------------

  string executableName = argv [0];

  // are there insider and/or regular options present?
  // ------------------------------------------------------

  bool insiderOptions = false;
  bool regularOptions = false;

	for (int i = 1; i < argc; ++i) {
	  string argumentAsString = string (argv [i]);

    string argumentWithoutDash = argumentAsString.substr (1);

		if (argumentWithoutDash == K_INSIDER_OPTION_NAME) {
		  insiderOptions = true;
		}
		if (argumentWithoutDash == K_REGULAR_OPTION_NAME) {
		  regularOptions = true;
		}
	} // for

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  cerr <<
    executableName << " main()" <<
    ", insiderOptions: " << booleanAsString (insiderOptions) <<
    ", regularOptions: " << booleanAsString (regularOptions) <<
    endl;
#endif
#endif

  if (insiderOptions && regularOptions) {
    stringstream s;

    s <<
      "options '-insider' and '-regular' cannot be used together";

    oahError (s.str ());
  }

  // here, at most one of insiderOptions and regularOptions is true

  // create the global log indented output stream
  // ------------------------------------------------------

  createTheGlobalIndentedOstreams (cout, cerr);

  // fetch the theOptionsVector from argc/argv
  // ------------------------------------------------------

	optionsVector theOptionsVector;

	if (! arguments2optionsVector (argc, argv, theOptionsVector)) {
    cerr <<
      executableName <<
      ": arguments2optionsVector() returned false" <<
      endl;

    return 1;
	}

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  displayOptionsVector (theOptionsVector, cerr);
#endif
#endif

  // fetch the generated output kind from theOptionsVector,
  // since the OAH handler should only use the OAH groups needed for it
  // ------------------------------------------------------

  generatorOutputKind
    theGeneratorOutputKind = k_NoOutput;

  for (unsigned int i = 0; i < theOptionsVector.size (); ++i) {
    string optionName  = theOptionsVector [i].first;
    string optionValue = theOptionsVector [i].second;

    string optionNameWithoutDash = optionName.substr (1);

    if (
      optionNameWithoutDash == K_GENERATED_OUTPUT_KIND_SHORT_NAME
        ||
      optionNameWithoutDash == K_GENERATED_OUTPUT_KIND_LONG_NAME
    ) {
      theGeneratorOutputKind =
        generatorOutputKindFromString (optionValue);
    }
    else if (optionNameWithoutDash == K_GENERATED_OUTPUT_KIND_GUIDO_NAME) {
      theGeneratorOutputKind = kGuidoOutput;
    }
    else if (optionNameWithoutDash == K_GENERATED_OUTPUT_KIND_LIlYPOND_NAME) {
      theGeneratorOutputKind = kLilyPondOutput;
    }
    else if (optionNameWithoutDash == K_GENERATED_OUTPUT_KIND_BRAILLE_NAME) {
      theGeneratorOutputKind = kBrailleOutput;
    }
    else if (optionNameWithoutDash == K_GENERATED_OUTPUT_KIND_MUSICXML_NAME) {
      theGeneratorOutputKind = kMusicXMLOutput;
    }
    else if (optionNameWithoutDash == K_GENERATED_OUTPUT_KIND_MIDI_NAME) {
      theGeneratorOutputKind = kMidiOutput;
    }
  } //for


#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  cerr <<
    "==> generatorOutputKind: " <<
    generatorOutputKindAsString (theGeneratorOutputKind) <<
    endl;
#endif
#endif

  switch (theGeneratorOutputKind) {
    case k_NoOutput:
      // wait until after help options have been handled
      // before issuing an error message
      break;

    case kGuidoOutput:
    case kLilyPondOutput:
    case kBrailleOutput:
    case kMusicXMLOutput:
      break;

    case kMidiOutput:
      gLogStream <<
        "MIDI output is not implemented yet, sorry" <<
        endl;

      return 0;
      break;
  } // switch

  // the oahHandler, set below
  // ------------------------------------------------------

  S_oahHandler handler;

  try {
    // create an Mikrokosmos3Wandering insider OAH handler
    // ------------------------------------------------------

    S_Mikrokosmos3WanderingInsiderHandler
      insiderOahHandler =
        Mikrokosmos3WanderingInsiderHandler::create (
          executableName,
          executableName + " insider OAH handler with argc/argv",
          theGeneratorOutputKind);

    // the OAH handler to be used, a regular handler is the default
    // ------------------------------------------------------

    if (insiderOptions) {
      // use the insider Mikrokosmos3Wandering OAH handler
      handler = insiderOahHandler;
    }
    else {
      // create a regular Mikrokosmos3Wandering OAH handler
      handler =
        Mikrokosmos3WanderingRegularHandler::create (
          executableName,
          executableName + " regular OAH handler with argc/argv",
          insiderOahHandler,
          theGeneratorOutputKind);
    }

    // handle the command line options and arguments
    // ------------------------------------------------------

    // handle the options and arguments from argc/argv
    oahElementHelpOnlyKind
      helpOnlyKind =
        handler->
          handleOptionsAndArgumentsFromArgcAndArgv (
            argc, argv);

    // have help options been used?
    switch (helpOnlyKind) {
      case kElementHelpOnlyYes:
        return 0; // quit now
        break;
      case kElementHelpOnlyNo:
        // go ahead
        break;
    } // switch
  }
  catch (msrOahException& e) {
    displayException (e, gOutputStream);
    return kInvalidOption;
  }
  catch (exception& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }

  switch (theGeneratorOutputKind) {
    case k_NoOutput:
      {
        stringstream s;

        s <<
          executableName <<
          " requires the output kind to be suplied thru one of the options " <<
          "'-" << K_GENERATED_OUTPUT_KIND_SHORT_NAME << "'" <<
          ", -'" << K_GENERATED_OUTPUT_KIND_LONG_NAME << "'" <<
          ", -'" << K_GENERATED_OUTPUT_KIND_GUIDO_NAME << "'" <<
          ", -'" << K_GENERATED_OUTPUT_KIND_LIlYPOND_NAME << "'" <<
          ", -'" << K_GENERATED_OUTPUT_KIND_BRAILLE_NAME << "'" <<
          ", -'" << K_GENERATED_OUTPUT_KIND_MUSICXML_NAME << "'" <<
          " or '" << K_GENERATED_OUTPUT_KIND_MIDI_NAME << "'" <<
          ", unless the run is a pure help one";

        oahError (s.str ());
      }
      break;

    default:
      ;
  } // switch

  // check indentation
  if (gIndenter != 0) {
    gLogStream <<
      "### " <<
      executableName <<
      " gIndenter value after options ands arguments checking: " <<
      gIndenter.getIndent () <<
      " ###" <<
      endl;

    gIndenter.resetToZero ();
  }

  if (gGlobalOahOahGroup->getDisplayOahHandler ()) {
    gLogStream <<
      "The OAH handler contains:" <<
      endl;

    ++gIndenter;
    handler->print (gLogStream);
    --gIndenter;
  }
  if (gGlobalOahOahGroup->getDisplayOahHandlerSummary ()) {
    gLogStream <<
      "The summary of the OAH handler contains:" <<
      endl;

    ++gIndenter;
    handler->printSummary (gLogStream);
    --gIndenter;
  }
  if (gGlobalOahOahGroup->getDisplayOahHandlerEssentials ()) {
    gLogStream <<
      "The essentials of the OAH handler contains:" <<
      endl;

    ++gIndenter;
    handler->printHandlerEssentials (
      gLogStream,
      30); // fieldWidth
    --gIndenter;
  }

  // let's go ahead
  // ------------------------------------------------------

  // get the generation API kind from the options
  // ------------------------------------------------------

  msrGenerationAPIKind
    theGenerationAPIKind =
      gGlobalMikrokosmos3WanderingOahGroup->
        getGenerationAPIKind ();

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  cerr <<
    "==> generationAPIKind: " <<
    msrGenerationAPIKindAsString (theGenerationAPIKind) <<
    endl;
#endif
#endif

  // create and populate the theMsrScore
  // ------------------------------------------------------

  // start the clock
  clock_t startClock = clock ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePasses ()) {
    string separator =
      "%--------------------------------------------------------------";

    cerr <<
      endl <<
      separator <<
      endl <<
      gTab <<
      "Pass 1: Creating the MSR score with the " <<
      msrGenerationAPIKindAsString (theGenerationAPIKind) <<
      endl <<
      separator <<
      endl;
  }
#endif

  S_msrScore
    theMsrScore =
      createAndPopulateTheScore (
        theGenerationAPIKind);

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    "Pass 1",
    "Create the MSR score",
    timingItem::kMandatory,
    startClock,
    endClock);

  // set the desired options
  // ------------------------------------------------------

  enforceSomeOptions (theGeneratorOutputKind);

  // should we generate Guido, LilyPond, braille music or MusicXML?
  // ------------------------------------------------------

  xmlErr err = kNoErr;

  switch (theGeneratorOutputKind) {
    case k_NoOutput:
      // should not occur, unless the run is a pure help one
      break;

    case kGuidoOutput:
      err =
        msrScore2guidoWithHandler (
          theMsrScore,
          "Pass 2",
          "Convert the MSR score into a second MSR",
          "Pass 3",
          "Convert the second MSR into an mxmlTree",
          "Pass 4",
          "Convert the mxmlTree into Guido text",
          cout,
          cerr,
          handler);
      break;

    case kLilyPondOutput:
      err =
        msrScore2lilypondWithHandler (
          theMsrScore,
          "Pass 2",
          "Convert the MSR into an LPSR",
          "Pass 3",
          "Convert the LPSR into LilyPond code",
          cout,
          cerr,
          handler);
      break;

    case kBrailleOutput:
      err =
        msrScore2brailleWithHandler (
          theMsrScore,
          "Pass 2a",
          "Create the first BSR from the MSR",
          "Pass 2b",
          "Create the finalized BSR from the first BSR",
          "Pass 3",
          "Convert the BSR into Braille music text",
          cout,
          cerr,
          handler);
      break;

    case kMusicXMLOutput:
      err =
        msrScore2musicxmlWithHandler (
          theMsrScore,
          "Pass 2",
          "Convert the MSR score into a second MSR",
          "Pass 3",
          "Convert the second MSR into an mxmlTree",
          "Pass 4",
          "Convert the mxmlTree into MusicXML text",
          cout,
          cerr,
          handler);
      break;

    case kMidiOutput:
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  if (err != 0) {
    cerr <<
      executableName << ", " <<
      generatorOutputKindAsString (theGeneratorOutputKind) <<
      ", err = " <<
      err <<
      endl;
  }
#endif
#endif

  // display the input line numbers for which messages have been issued
  // ------------------------------------------------------

  displayWarningsAndErrorsInputLineNumbers ();

  // print timing information
  // ------------------------------------------------------

  if (gGlobalGeneralOahGroup->getDisplayCPUusage ())
    timing::gGlobalTiming.print (gLogStream);

  // check indentation
  // ------------------------------------------------------

  if (gIndenter != 0) {
    gLogStream <<
      "### " << executableName << " gIndenter final value: " <<
      gIndenter.getIndent () <<
      " ###" <<
      endl;

    gIndenter.resetToZero ();
  }

  // over!
  // ------------------------------------------------------

  if (err != kNoErr) {
    gLogStream <<
      "### The generation of " <<
      generatorOutputKindAsString (theGeneratorOutputKind) <<
      " thru the " <<
      msrGenerationAPIKindAsString (
        theGenerationAPIKind) <<
      " output failed ###" <<
      endl;
  }

  switch (err) {
    case kNoErr:
      return 0;
      break;
    case kInvalidFile:
      return 1;
      break;
    case kInvalidOption:
      return 2;
      break;
    case kUnsupported:
      return 3;
      break;
  } // switch
}

/*
    static unsigned int k_note_counter = 0;

    ++k_note_counter;

    if (k_note_counter > 1) {
      abort ();
    }

*/
