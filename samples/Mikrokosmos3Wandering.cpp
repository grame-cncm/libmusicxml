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

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "msrOah.h"

#include "musicxmlOah.h"
#include "msr2mxmlTreeOah.h"
#include "mxmlTreeOah.h"

#include "msr2lpsrOah.h"
#include "lpsrOah.h"
#include "lpsr2lilypondOah.h"

#include "msr2bsrOah.h"
#include "bsrOah.h"
#include "bsr2brailleOah.h"

#include "libmusicxml.h" // for xmlErr

#include "msr.h"

#include "msr2mxmlTreeInterface.h"
#include "xml2guidovisitor.h"
#include "mxmlTree2xmlTranlatorInterface.h"

#include "lpsr.h"

#include "msr2lpsrInterface.h"
#include "lpsr2lilypondInterface.h"

#include "bsr.h"

#include "msr2bsrInterface.h"
#include "bsr2bsrFinalizer.h"
#include "bsr2bsrFinalizerInterface.h"
#include "bsr2brailleTranslatorInterface.h"

#include "exNihiloInsiderOahHandler.h"
#include "exNihiloRegularOahHandler.h"


using namespace std;
using namespace MusicXML2;

/*
  ENFORCE_TRACE_OAH can be used to issue trace messages
  before gGlobalOahOahGroup->fTrace has been initialized
*/
//#define ENFORCE_TRACE_OAH

//_______________________________________________________________________________
#ifndef WIN32
static void _sigaction(int signal, siginfo_t *si, void *arg)
{
  cerr << "Signal #" << signal << " catched!" << endl;
  exit (-2);
}

static void catchsigs ()
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
static void catchsigs () {}
#endif

//_______________________________________________________________________________
static void argvElements2stringsVector (
  int argc, char *argv[],
  vector<string>& stringsVector)
{
	for (int i=1; i<argc; i++) {
		stringsVector.push_back (argv [i]);
	} // for
}

//_______________________________________________________________________________
static bool args2Options (int argc, char *argv[], optionsVector& theOptionsVector)
{
  // create a strings vector from the elements in argv
	vector<string> stringsVector;

	argvElements2stringsVector (argc, argv, stringsVector);

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  cerr << "args2Options: stringsVector size: " << stringsVector.size() << endl;
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

	for (int i = 0; i < stringsVector.size (); i++) {
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

//_______________________________________________________________________________
enum generatedCodeKind {
  kNoGeneratedCode,
  kGuido, kLilyPond, kBrailleMusic, kMusicXML };

string generatedCodeKindAsString (generatedCodeKind kind)
{
  string result;

  switch (kind) {
    case kNoGeneratedCode:
      result = "*NoGeneratedCode*";
      break;
    case kGuido:
      result = "Guido";
      break;
    case kLilyPond:
      result = "LilyPond";
      break;
    case kBrailleMusic:
      result = "BrailleMusic";
      break;
    case kMusicXML:
      result = "MusicXML";
      break;
  } // switch

  return result;
}

//_______________________________________________________________________________
generatedCodeKind gGeneratedCodeKind = kNoGeneratedCode;

void registerGeneratedCodeKind (generatedCodeKind kind)
{
  if (gGeneratedCodeKind != kNoGeneratedCode) {
    cerr << "only one of '-guido', '-lilypond', '-braille' and '-musicxml' can be used" << endl;
    exit (2);
  }
  else {
    gGeneratedCodeKind = kind;
  }
}

//------------------------------------------------------------------------
enum msplGenerationKind {
  kRegularAPIKind,
  kStringsAPIKind
};

string msplGenerationKindAsString (
  msplGenerationKind generationKind)
{
  string result;

  switch (generationKind) {
    case kRegularAPIKind:
      result = "regular API";
      break;
    case kStringsAPIKind:
      result = "strings API";
      break;
  } // switch

  return result;
}

//------------------------------------------------------------------------
EXP S_msrScore createTheScore (
  msplGenerationKind generationKind)
{
  // create the score
  S_msrScore
    score =
      msrScore::create (__LINE__);

  // create its identification
  S_msrIdentification
    identification =
      msrIdentification::create (
        __LINE__);

  score->
    setIdentification (
      identification);

  // set the identification's work title
  identification->
    setWorkTitle (
      __LINE__,
      "Mikrokosmos III Wandering - MSPL, " +
        msplGenerationKindAsString (generationKind));

  return score;
}

//------------------------------------------------------------------------
EXP S_msrPart createPartInScore (S_msrScore score)
{
  // create the part group
  S_msrPartGroup
    partGroup =
      msrPartGroup::create (
        __LINE__, 1, 1, "OnlyPartGroup", nullptr, score);

  // add it to the score
  score->
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

  return part;
}

//------------------------------------------------------------------------
S_msrStaff createStaffInPart (int staffNumber, S_msrPart part)
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
S_msrVoice createVoiceInStaff (
  int          inputLineNumber,
  msrVoiceKind voiceKind,
  int          voiceNumber,
  S_msrStaff   staff)
{
  // create the voice
  S_msrVoice
    voice =
      msrVoice::create (
        inputLineNumber,
        kVoiceRegular,
        voiceNumber,
        msrVoice::kCreateInitialLastSegmentYes,
          // the initial last segment is ready to receive music
        staff);

  // append it to the staff
  staff->
//    registerVoiceByItsNumber (
    registerRegularVoiceByItsNumber (
      inputLineNumber,
//      voiceNumber,
      voice);

  return voice;
}

//------------------------------------------------------------------------
// upperVoice1 - Regular API
//------------------------------------------------------------------------
void populateUpperVoice1WithTheRegularAPI (
  S_msrVoice upperVoice1)
{
  // measure 1
  // ----------------------------------

  {
    // create it
    string measure1number = "1"; // the measure number is a string;

    S_msrMeasure
      measure1 =
        upperVoice1->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure1number,
            msrMeasure::kMeasureImplicitKindNo);

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


  // measure 2
  // ----------------------------------

  {
    // create it
    string measure2number = "2"; // the measure number is a string;

    S_msrMeasure
      measure2 =
        upperVoice1->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure2number,
            msrMeasure::kMeasureImplicitKindNo);

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
        upperVoice1->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure3number,
            msrMeasure::kMeasureImplicitKindNo);

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
        upperVoice1->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure4number,
            msrMeasure::kMeasureImplicitKindNo);

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
        upperVoice1->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure5number,
            msrMeasure::kMeasureImplicitKindNo);

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
void populateUpperVoice1WithTheStringsAPI (
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
        upperVoice1->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure1number,
            msrMeasure::kMeasureImplicitKindNo);

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


  // measure 2
  // ----------------------------------

  {
    // create it
    string measure2number = "2"; // the measure number is a string;

    S_msrMeasure
      measure2 =
        upperVoice1->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure2number,
            msrMeasure::kMeasureImplicitKindNo);

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
        upperVoice1->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure3number,
            msrMeasure::kMeasureImplicitKindNo);

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
        upperVoice1->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure4number,
            msrMeasure::kMeasureImplicitKindNo);

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
        upperVoice1->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure5number,
            msrMeasure::kMeasureImplicitKindNo);

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
void populateLowerVoice1WithTheRegularAPI (
  S_msrVoice lowerVoice1)
{
  // measure 1
  // ----------------------------------

  {
    // create it
    string measure1number = "1"; // the measure number is a string;

    S_msrMeasure
      measure1 =
        lowerVoice1->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure1number,
            msrMeasure::kMeasureImplicitKindNo);

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

  // measure 2
  // ----------------------------------

  {
    // create it
    string measure2number = "2"; // the measure number is a string;

    S_msrMeasure
      measure2 =
        lowerVoice1->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure2number,
            msrMeasure::kMeasureImplicitKindNo);

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
        lowerVoice1->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure3number,
            msrMeasure::kMeasureImplicitKindNo);

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
        lowerVoice1->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure4number,
            msrMeasure::kMeasureImplicitKindNo);

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
        lowerVoice1->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure5number,
            msrMeasure::kMeasureImplicitKindNo);

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
void populateLowerVoice1WithTheStringsAPI (
  S_msrVoice lowerVoice1)
{
  // measure 1
  // ----------------------------------

  {
    // create it
    string measure1number = "1"; // the measure number is a string;

    S_msrMeasure
      measure1 =
        lowerVoice1->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure1number,
            msrMeasure::kMeasureImplicitKindNo);

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

  // measure 2
  // ----------------------------------

  {
    // create it
    string measure2number = "2"; // the measure number is a string;

    S_msrMeasure
      measure2 =
        lowerVoice1->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure2number,
            msrMeasure::kMeasureImplicitKindNo);

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
        lowerVoice1->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure3number,
            msrMeasure::kMeasureImplicitKindNo);

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
        lowerVoice1->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure4number,
            msrMeasure::kMeasureImplicitKindNo);

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
        lowerVoice1->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure5number,
            msrMeasure::kMeasureImplicitKindNo);

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
void populateLowerVoice2WithTheRegularAPI (
  S_msrVoice lowerVoice2)
{
  // measure 1
  // ----------------------------------

  {
    // create it
    string measure1number = "1"; // the measure number is a string;

    S_msrMeasure
      measure1 =
        lowerVoice2->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure1number,
            msrMeasure::kMeasureImplicitKindNo);

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

  // measure 2
  // ----------------------------------

  {
    // create it
    string measure2number = "2"; // the measure number is a string;

    S_msrMeasure
      measure2 =
        lowerVoice2->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure2number,
            msrMeasure::kMeasureImplicitKindNo);

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
        lowerVoice2->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure3number,
            msrMeasure::kMeasureImplicitKindNo);

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
        lowerVoice2->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure4number,
            msrMeasure::kMeasureImplicitKindNo);

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
        lowerVoice2->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure5number,
            msrMeasure::kMeasureImplicitKindNo);

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
void populateLowerVoice2WithTheStringsAPI (
  S_msrVoice lowerVoice2)
{
  // measure 1
  // ----------------------------------

  {
    // create it
    string measure1number = "1"; // the measure number is a string;

    S_msrMeasure
      measure1 =
        lowerVoice2->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure1number,
            msrMeasure::kMeasureImplicitKindNo);

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

  // measure 2
  // ----------------------------------

  {
    // create it
    string measure2number = "2"; // the measure number is a string;

    S_msrMeasure
      measure2 =
        lowerVoice2->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure2number,
            msrMeasure::kMeasureImplicitKindNo);

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
        lowerVoice2->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure3number,
            msrMeasure::kMeasureImplicitKindNo);

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
        lowerVoice2->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure4number,
            msrMeasure::kMeasureImplicitKindNo);

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
        lowerVoice2->
          createMeasureAndAppendItToVoice (
            __LINE__,
            measure5number,
            msrMeasure::kMeasureImplicitKindNo);

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
S_msrStaff createAndPopulateUpperStaffInPart (
  S_msrPart          part,
  msplGenerationKind generationKind)
{
  // create the upper staff
  S_msrStaff
    upperStaff =
      createStaffInPart (1, part);

  // create voice 1 in upperStaff
  S_msrVoice
    upperVoice1 =
      createVoiceInStaff (
        __LINE__,
        kVoiceRegular,
        1,
       upperStaff);

  // populate its voice(s)
  switch (generationKind) {
    case kRegularAPIKind:
      populateUpperVoice1WithTheRegularAPI (
        upperVoice1);
      break;

    case kStringsAPIKind:
      populateUpperVoice1WithTheStringsAPI (
        upperVoice1);
      break;
  } // switch

  return upperStaff;
}

//------------------------------------------------------------------------
S_msrStaff createAndPopulateLowerStaffInPart (
  S_msrPart          part,
  msplGenerationKind generationKind)
{
  // create the lower staff
  S_msrStaff
    lowerStaff =
      createStaffInPart (2, part);

  // create voice 1 in lowerStaff
  S_msrVoice
    lowerVoice1 =
      createVoiceInStaff (
        __LINE__,
        kVoiceRegular,
        1,
       lowerStaff);

  // create voice 2 in lowerStaff
  S_msrVoice
    lowerVoice2 =
      createVoiceInStaff (
        __LINE__,
        kVoiceRegular,
        2,
       lowerStaff);

  // populate its voice(s)
  // populate its voice(s)
  switch (generationKind) {
    case kRegularAPIKind:
      populateLowerVoice1WithTheRegularAPI (
        lowerVoice1);
      populateLowerVoice2WithTheRegularAPI (
        lowerVoice2);
      break;

    case kStringsAPIKind:
      populateLowerVoice1WithTheStringsAPI (
        lowerVoice1);
      populateLowerVoice2WithTheStringsAPI (
        lowerVoice2);
      break;
  } // switch

  return lowerStaff;
}

//------------------------------------------------------------------------
S_msrScore createAndPopulateTheScore (
  msplGenerationKind generationKind)
{
  S_msrScore
    score =
      createTheScore (
        generationKind);

  S_msrPart
    part =
      createPartInScore (score);

  // create and populate the two staves in part
  S_msrStaff
    upperStaff =
      createAndPopulateUpperStaffInPart (
        part,
        generationKind);

  S_msrStaff
    lowerStaff =
      createAndPopulateLowerStaffInPart (
        part,
        generationKind);
  // finalize the part
  part->
    finalizePart (__LINE__); // JMI

  if (gGlobalMsrOahGroup->getDisplayMsr ()) {
    // print the score
    gOutputStream <<
      "The MSR score contains:" <<
      endl <<
      "----------------------" <<
      endl;

    gIndenter++;
    gOutputStream <<
      score <<
      endl;
    gIndenter--;
  }

  return score;
}

//------------------------------------------------------------------------
xmlErr generateGuidoCodeFromScore (S_msrScore score)
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  cerr <<
    "==> generateGuidoCodeFromScore" <<
    endl;
#endif
#endif

  // convert the score into an mxmlTree (pass 2)
  // ------------------------------------------------------

  Sxmlelement mxmlTree;

  try {
    mxmlTree =
      convertMsrScoreToMxmltree (
        score,
        gGlobalMsrOahGroup,
        "Pass 2",
        timingItem::kMandatory);
  }
  catch (mxmlTreeToMsrException& e) {
    return kInvalidFile;
  }
  catch (std::exception& e) {
    return kInvalidFile;
  }

  // generate MusicXML from the mxmlTree (pass 3)
  // ------------------------------------------------------

  string
    outputFileName = "Mikrokosmos3Wandering.gmn";

  bool
    generateComments = true,
    generateStem     = true,
    generateBars     = true;

  int
    partNum = 0;

  try {
    // create the xml2guidovisitor
		xml2guidovisitor v (
		  generateComments, generateStem, generateBars, partNum);

		// do the conversion to Guido
		Sguidoelement gmn = v.convert (mxmlTree);

    // write the result to the output
		cout << gmn << endl;
  }
  catch (mxmlTreeToMsrException& e) {
    return kInvalidFile;
  }
  catch (std::exception& e) {
    return kInvalidFile;
  }

  return kNoErr;
}

//------------------------------------------------------------------------
xmlErr generateLilypondCodeFromScore (S_msrScore score)
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  cerr <<
    "==> generateLilypondCodeFromScore" <<
    endl;
#endif
#endif

  // the LPSR score
  S_lpsrScore theLpsrScore;

  {
    // create the LPSR from the MSR (pass 2)
    // ------------------------------------------------------

    const string passNumber = "Pass 2";

    try {
      theLpsrScore =
        convertMsrScoreToLpsrScore (
          score,
          gGlobalMsrOahGroup,
          gGlobalLpsrOahGroup,
          passNumber);
    }
    catch (msrScoreToLpsrScoreException& e) {
      return kInvalidFile;
    }
    catch (std::exception& e) {
      return kInvalidFile;
    }

    // display the LPSR score if requested
    // ------------------------------------------------------

    if (gGlobalLpsrOahGroup->getDisplayLpsr ()) {
      displayLpsrScore_OptionalPass (
        theLpsrScore,
        gGlobalMsrOahGroup,
        gGlobalLpsrOahGroup);
    }

    if (gGlobalLpsrOahGroup->getDisplayLpsrShort ()) {
      displayLpsrScoreShort_OptionalPass (
        theLpsrScore,
        gGlobalMsrOahGroup,
        gGlobalLpsrOahGroup);
    }

    // should we return now?
    // ------------------------------------------------------

    if (gGlobalLpsrOahGroup->getQuitAfterPass3 ()) {
      cerr <<
        endl <<
        "Quitting after pass 3 as requested" <<
        endl;

      return kNoErr;
    }
  }

  {
    // generate LilyPond code from the LPSR (pass 3)
    // ------------------------------------------------------

    const string passNumber = "Pass 3";

    string
      outputFileName = "Mikrokosmos3Wandering.ly";

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      cerr <<
        "xmlFile2lilypond() outputFileName = \"" <<
        outputFileName <<
        "\"" <<
        endl;
    }
#endif

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      cerr <<
        "xmlFile2lilypond() output goes to standard output" <<
        endl;
    }
#endif

    // create an indented output stream for the LilyPond code
    // to be written to outputFileStream
    indentedOstream
      lilypondStandardOutputStream (
        cout,
        gIndenter);

    // convert the LPSR score to LilyPond code
    try {
      convertLpsrScoreToLilypondCode (
        theLpsrScore,
        gGlobalMsrOahGroup,
        gGlobalLpsrOahGroup,
        passNumber,
        lilypondStandardOutputStream);
    }
    catch (lpsrScoreToLilypondException& e) {
      return kInvalidFile;
    }
    catch (std::exception& e) {
      return kInvalidFile;
    }

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      cerr <<
        "xmlFile2lilypond() output goes to file \"" <<
        outputFileName <<
        "\"" <<
        endl;
    }
#endif

    // open output file
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTracePasses ()) {
      cerr <<
        "Opening file '" << outputFileName << "' for writing" <<
        endl;
    }
#endif

    ofstream
      outputFileStream (
        outputFileName.c_str (),
        ofstream::out);

    if (! outputFileStream.is_open ()) {
      stringstream s;

      s <<
        "Could not open LilyPond output file \"" <<
        outputFileName <<
        "\" for writing, quitting";

      string message = s.str ();

      cerr <<
        message <<
        endl;

      throw lpsrScoreToLilypondException (message);
    }

    // create an indented output stream for the LilyPond code
    // to be written to outputFileStream
    indentedOstream
      lilypondFileOutputStream (
        outputFileStream,
        gIndenter);

    // convert the LPSR score to LilyPond code
    try {
      convertLpsrScoreToLilypondCode (
        theLpsrScore,
        gGlobalMsrOahGroup,
        gGlobalLpsrOahGroup,
        passNumber,
        lilypondFileOutputStream);
    }
    catch (lpsrScoreToLilypondException& e) {
      return kInvalidFile;
    }
    catch (std::exception& e) {
      return kInvalidFile;
    }

    // close output file
#ifdef TRACE_OAH
    if (gTraceOah->fTracePasses) {
      gLogOstream <<
        endl <<
        "Closing file \"" << outputFileName << "\"" <<
        endl;
    }
#endif

    outputFileStream.close ();
  }

  return kNoErr;
}

//------------------------------------------------------------------------
xmlErr generateBrailleMusicFromScore (S_msrScore score)
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  cerr <<
    "==> generateBrailleMusicFromScore" <<
    endl;
#endif
#endif

  // the first BSR score
  S_bsrScore firstBsrScore;

  {
    // create the first BSR from the MSR (pass 2a)
    // ------------------------------------------------------

    // start the clock
    clock_t startClock = clock ();
    const string passNumber = "Pass 2a";

    try {
      firstBsrScore =
        convertMsrScoreToBsrScore (
          score,
          gGlobalMsrOahGroup,
          gGlobalBsrOahGroup,
          passNumber);
    }
    catch (msrScoreToBsrScoreException& e) {
      return kInvalidFile;
    }
    catch (std::exception& e) {
      return kInvalidFile;
    }

    clock_t endClock = clock ();

    // register time spent
    timing::gGlobalTiming.appendTimingItem (
      passNumber,
      "Build the first BSR",
      timingItem::kMandatory,
      startClock,
      endClock);

    // display the first BSR score if requested
    // ------------------------------------------------------

    if (gGlobalBsrOahGroup->getDisplayBsr ()) {
      displayBsrFirstScore_OptionalPass (
        firstBsrScore,
        gGlobalMsrOahGroup,
        gGlobalBsrOahGroup);
    }

    if (gGlobalBsrOahGroup->getDisplayBsrShort ()) {
      displayBsrFirstScoreShort_OptionalPass (
        firstBsrScore,
        gGlobalMsrOahGroup,
        gGlobalBsrOahGroup);
    }

    // should we return now?
    // ------------------------------------------------------

    if (gGlobalMsr2bsrOahGroup->getQuitAfterPass3a ()) {
      cerr <<
        endl <<
        "Quitting after pass 3a as requested" <<
        endl;

      return kNoErr;
    }
  }

  // the finalized BSR score
  S_bsrScore finalizedBsrScore;

  {
    // create the finalized BSR from the first BSR (pass 2b)
    // ------------------------------------------------------

    // start the clock
    clock_t startClock = clock ();
    const string passNumber = "Pass 2b";

    try {
      finalizedBsrScore =
        convertBsrFirstScoreToFinalizedBsrScore (
          firstBsrScore,
          gGlobalBsrOahGroup,
          passNumber);
    }
    catch (bsrScoreToFinalizedBsrScoreException& e) {
      return kInvalidFile;
    }
    catch (std::exception& e) {
      return kInvalidFile;
    }

    clock_t endClock = clock ();

    // register time spent
    timing::gGlobalTiming.appendTimingItem (
      passNumber,
      "Build the finalized BSR",
      timingItem::kMandatory,
      startClock,
      endClock);

    // display the finalized BSR score if requested
    // ------------------------------------------------------

    if (gGlobalBsrOahGroup->getDisplayBsr ()) {
      displayFinalizedBsrScore_OptionalPass (
        finalizedBsrScore,
        gGlobalMsrOahGroup,
        gGlobalBsrOahGroup);
    }

    if (gGlobalBsrOahGroup->getDisplayBsrShort ()) {
      displayFinalizedBsrScoreShort_OptionalPass (
        finalizedBsrScore,
        gGlobalMsrOahGroup,
        gGlobalBsrOahGroup);
    }

    // should we return now?
    // ------------------------------------------------------

    if (gGlobalMsr2bsrOahGroup->getQuitAfterPass3b ()) {
      cerr <<
        endl <<
        "Quitting after pass 3b as requested" <<
        endl;

      return kNoErr;
    }
  }

  {
    // generate Braille music text from the BSR (pass 3)
    // ------------------------------------------------------

    // start the clock
    clock_t startClock = clock ();
    const string passNumber = "Pass 3";

    string
      outputFileName = "Mikrokosmos3Wandering.brf";

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceOah ()) {
        cerr <<
          "xmlFile2braille() outputFileName = \"" <<
          outputFileName <<
          "\"" <<
          endl;
      }
#endif

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      cerr <<
        "xmlFile2braille() output goes to standard output" <<
        endl;
    }
#endif

    // convert the BSR score to braille text
    try {
      convertBsrScoreToBrailleText (
        finalizedBsrScore,
        gGlobalBsrOahGroup,
        passNumber,
        cout);
    }
    catch (lpsrScoreToLilypondException& e) {
      return kInvalidFile;
    }
    catch (std::exception& e) {
      return kInvalidFile;
    }

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      cerr <<
        "xmlFile2braille() output goes to file \"" <<
        outputFileName <<
        "\"" <<
        endl;
    }
#endif

    // open output file
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTracePasses ()) {
      cerr <<
        "Opening file \"" << outputFileName << "\" for writing" <<
        endl;
    }
#endif

    ofstream
      brailleCodeFileOutputStream (
        outputFileName.c_str (),
        ofstream::out);

    if (! brailleCodeFileOutputStream.is_open ()) {
      stringstream s;

      s <<
        "Could not open Braille music output file \"" <<
        outputFileName <<
        "\" for writing, quitting";

      string message = s.str ();

      cerr <<
        message <<
        endl;

      throw bsrScoreToBrailleTextException (message);
    }

    // convert the finalized BSR score to braille text
    try {
      convertBsrScoreToBrailleText (
        finalizedBsrScore,
        gGlobalBsrOahGroup,
        passNumber,
        brailleCodeFileOutputStream);
    }
    catch (lpsrScoreToLilypondException& e) {
      return kInvalidFile;
    }
    catch (std::exception& e) {
      return kInvalidFile;
    }

    // close output file
#ifdef TRACE_OAH
    if (gTraceOah->fTracePasses) {
      gLogOstream <<
        endl <<
        "Closing file \"" << outputFileName << "\"" <<
        endl;
    }
#endif

    brailleCodeFileOutputStream.close ();

    // register time spent
    clock_t endClock = clock ();

    timing::gGlobalTiming.appendTimingItem (
      passNumber,
      "Generate braille music",
      timingItem::kMandatory,
      startClock,
      endClock);
  }

  return kNoErr;
}

//------------------------------------------------------------------------
xmlErr generateMusicXMLFromScore (S_msrScore score)
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  cerr <<
    "==> generateMusicXMLFromScore" <<
    endl;
#endif
#endif

  // convert the score into an mxmlTree (pass 2)
  // ------------------------------------------------------

  Sxmlelement mxmlTree;

  try {
    mxmlTree =
      convertMsrScoreToMxmltree (
        score,
        gGlobalMsrOahGroup,
        "Pass 2",
        timingItem::kMandatory);
  }
  catch (mxmlTreeToMsrException& e) {
    return kInvalidFile;
  }
  catch (std::exception& e) {
    return kInvalidFile;
  }

  // generate MusicXML from the mxmlTree (pass 3)
  // ------------------------------------------------------

  string
    outputFileName = "Mikrokosmos3Wandering.xml";

  try {
    generateMusicXMLFromMxmlTree (
      mxmlTree,
      outputFileName,
      cerr,
      "Pass 3");
  }
  catch (mxmlTreeToMsrException& e) {
    return kInvalidFile;
  }
  catch (std::exception& e) {
    return kInvalidFile;
  }

  return kNoErr;
}

//------------------------------------------------------------------------
void initializeTheLibraryAndOAH (string executableName)
{
  // initialize options handling, phase 1
  // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
  // the 'trace' prefixes
  // --------------------------------------


  S_oahPrefix
    shortTracePrefix =
      oahPrefix::create (
        "t", "t",
        "'-t=abc,wxyz' is equivalent to '-tabc, -twxyz'");
//  registerPrefixInHandler (
//    fShortTracePrefix);

  S_oahPrefix
    longTracePrefix =
      oahPrefix::create (
        "trace", "trace-",
        "'-trace=abc,yz' is equivalent to '-trace-abc, -trace-yz'");
//  registerPrefixInHandler (
//    fLongTracePrefix);

  // create the trace OAH group
  // --------------------------------------

  S_traceOahGroup
    traceOahGroup =
      createGlobalTraceOahGroup (
        shortTracePrefix,
        longTracePrefix);
#endif

  // initialize the library
  // ------------------------------------------------------

  initializeMSR ();
  initializeLPSR ();
  initializeBSR ();

  // create the OAH OAH group
  S_oahOahGroup
    oahOahGroup =
      createGlobalOahOahGroup (
        executableName);

  // create the general OAH group
  S_generalOahGroup
    generalOahGroup =
      createGlobalGeneralOahGroup ();

  // create the MSR OAH group
  S_msrOahGroup
    msrOahGroup =
      createGlobalMsrOahGroup ();

  // create the msr2lpsr OAH group
  S_msr2lpsrOahGroup
    msr2lpsrOahGroup =
      createGlobalMsr2lpsrOahGroup ();

  // create the LPSR OAH group
  S_lpsrOahGroup
    lpsrOahGroup =
      createGlobalLpsrOahGroup ();

  // create the lpsr2lilypond OAH group
  S_lpsr2lilypondOahGroup
    lpsr2lilypondOahGroup =
      createGlobalLpsr2lilypondOahGroup ();

  // create the LilyPond OAH group
  S_lilypondOahGroup
    lilypondOahGroup =
      createGlobalLilypondOahGroup ();

  // create the msr2bsr OAH group
  S_msr2bsrOahGroup
    msr2bsrOahGroup =
      createGlobalMsr2bsrOahGroup ();

  // create the BSR OAH group
  S_bsrOahGroup
    bsrOahGroup =
      createGlobalBsrOahGroup ();

  // create the bsr2braille OAH group
  S_bsr2brailleOahGroup
    bsr2brailleOahGroup =
      createGlobalBsr2brailleOahGroup ();

  // create the msr2mxmlTree OAH group
  S_msr2mxmlTreeOahGroup
    msr2mxmlTreeOahGroup =
      createGlobalMsr2mxmlTreeOahGroup ();

  // create the mxmlTree OAH group
  S_mxmlTreeOahGroup
    mxmlTreeOahGroup =
      createGlobalMxmlTreeOahGroup ();

  // create the MusicXML OAH group
  S_musicxmlOahGroup
    musicxmlOahGroup =
      createGlobalMusicxmlOahGroup ();

/* JMI ???
  // create the xml2brl OAH group
  S_xml2brlInsiderOahGroup
    xml2brlInsiderOahGroup =
      createGlobalXml2brlOahGroup ();
      */
}

//------------------------------------------------------------------------
void setTheDesiredOptions ()
{
  /*
    This is a way to enforce options 'permanently'
    independently of the options chosen by the user
  */

  // trace
//  gGlobalTraceOahGroup->setTraceScore ();
//  gGlobalTraceOahGroup->setTracePartGroups ();
//  gGlobalTraceOahGroup->setTraceParts ();
  gGlobalTraceOahGroup->setTraceStaves ();
  gGlobalTraceOahGroup->setTraceVoices ();
//  gGlobalTraceOahGroup->setTraceSegments ();
//  gGlobalTraceOahGroup->setTraceMeasures ();
//  gGlobalTraceOahGroup->setTraceNotes ();

  // MSR
//  gGlobalMsrOahGroup->setTraceMsr ();
//  gGlobalMsrOahGroup->setTraceMsrVisitors ();

  gGlobalMsrOahGroup->setDisplayMsr ();

  gGlobalMsrOahGroup->setTraceMsrDurations ();

  // LPSR
//  gGlobalLpsrOahGroup->setTraceLpsr ();
//  gGlobalLpsrOahGroup->setTraceLpsrVisitors ();

  gGlobalLpsrOahGroup->setDisplayLpsr ();

  // lpsr2lilypond
  gGlobalLpsr2lilypondOahGroup->setLilypondCompileDate (); // JMI NOT OK
  gGlobalLpsr2lilypondOahGroup->setInputLineNumbers ();

  // BSR
  gGlobalBsrOahGroup->setTraceBsr ();
//  gGlobalBsrOahGroup->setTraceBsrVisitors ();

  // bsr2braille

  gGlobalBsr2brailleOahGroup->
    setBrailleOutputKind (kBrailleOutputUTF8Debug);

    /* JMI
  gGlobalXml2brlInsiderOahGroup->
    setAutoOutputFileName ();
*/

  // MusicXML
  gGlobalMxmlTreeOahGroup->setTraceMusicXMLTreeVisitors ();
}

//------------------------------------------------------------------------
// the main function
//------------------------------------------------------------------------
int main (int argc, char * argv[])
{
  // setup signals catching
  // ------------------------------------------------------

//	catchsigs ();

  // the executable name
  // ------------------------------------------------------

  string executableName = argv [0];

  // are there insider and/or regular options present?
  // ------------------------------------------------------

  bool insiderOptions = false;
  bool regularOptions = false;

	for (int i = 1; i < argc; i++) {
	  string argumentAsString = string (argv [i]);

		if (argumentAsString == "-insider") {
		  insiderOptions = true;
		}
		if (argumentAsString == "-regular") {
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

  // the about information
  // ------------------------------------------------------

  string
    aboutInformation =
      exNihiloAboutInformation ();

  // the oahHandler, set below
  // ------------------------------------------------------

  S_oahHandler handler;

  try {
    // create an exNihilo insider OAH handler
    // ------------------------------------------------------

    S_exNihiloInsiderOahHandler
      insiderOahHandler =
        exNihiloInsiderOahHandler::create (
          executableName,
          aboutInformation,
          executableName + " insider OAH handler with argc/argv");

    // the OAH handler to be used, a regular handler is the default
    // ------------------------------------------------------

    if (insiderOptions) {
      // use the insider exNihilo OAH handler
      handler = insiderOahHandler;
    }
    else {
      // create a regular exNihilo OAH handler
      handler =
        exNihiloRegularOahHandler::create (
          executableName,
          aboutInformation,
          executableName + " regular OAH handler with argc/argv",
          insiderOahHandler);
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
    return kInvalidOption;
  }
  catch (std::exception& e) {
    return kInvalidFile;
  }

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

  // let's go ahead
  // ------------------------------------------------------

/* JMI
  // fetch the theOptionsVector from argc/argv
  // ------------------------------------------------------

	optionsVector theOptionsVector;

	if (! args2Options (argc, argv, theOptionsVector)) {
    cerr <<
      executableName <<
      ": args2Options() returned false" <<
      endl;

    return 1;
	}

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  displayOptionsVector (theOptionsVector, cerr);
#endif
#endif

  // take generatedCodeKind options into account if any
  // ------------------------------------------------------

	optionsVector keptOptions;

	for (auto option: theOptionsVector) {
	  if (option.first      == "-guido") {
	    registerGeneratedCodeKind (kGuido);
      keptOptions.push_back (option);
    }
	  else if (option.first == "-lilypond") {
	    registerGeneratedCodeKind (kLilyPond);
      keptOptions.push_back (option);
	  }
	  else if (option.first == "-braille") {
	    registerGeneratedCodeKind (kBrailleMusic);
      keptOptions.push_back (option);
	  }
	  else if (option.first == "-musicxml") {
	    registerGeneratedCodeKind (kMusicXML);
      keptOptions.push_back (option);
	  }
	  else if (option.first == "-all") { // JMI ???
	    registerGeneratedCodeKind (kMusicXML);
      keptOptions.push_back (option);
	  }
	  else {
	    // FUTRURE JMI keptOptions.push_back (option);
	    cerr <<
	      executableName <<
	      ": option '" <<
	      option.first <<
	      "' is unknown" <<
	      endl;

	    return 1;
	  }
	} // for

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  displayOptionsVector (theOptionsVector, cerr);
#endif
#endif

  // the default is '-lilypond'
  if (gGeneratedCodeKind == kNoGeneratedCode) {
    gGeneratedCodeKind = kLilyPond;
  }

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  cerr <<
    "==> generatedCodeKind: " <<
    generatedCodeKindAsString (gGeneratedCodeKind) <<
    endl;
#endif
#endif
*/

  // initialize the library and OAH
  // ------------------------------------------------------

  initializeTheLibraryAndOAH (executableName);

  // create and populate the score
  // ------------------------------------------------------

  msplGenerationKind
    generationKind = // JMI option???
     kRegularAPIKind;
   //
   kStringsAPIKind;

  S_msrScore
    score =
      createAndPopulateTheScore (
        generationKind);

  // set the desired options
  // ------------------------------------------------------

  setTheDesiredOptions ();

  // should we generate Guido, LilyPond, braille music or MusicXML?
  // ------------------------------------------------------

  cerr <<
    "Converting the MSR score to " <<
    generatedCodeKindAsString (gGeneratedCodeKind) <<
    endl;

  xmlErr err = kNoErr;

  switch (gGeneratedCodeKind) {
    case kNoGeneratedCode:
      // should not occur
      break;
    case kGuido:
      err =
        generateGuidoCodeFromScore (
          score);
      break;
    case kLilyPond:
      err =
        generateLilypondCodeFromScore (
          score);
      break;
    case kBrailleMusic:
      err =
        generateBrailleMusicFromScore (
          score);
      break;
    case kMusicXML:
      err =
        generateMusicXMLFromScore (
          score);
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  if (err != 0) {
    cerr <<
      executableName << ", " <<
      generatedCodeKindAsString (gGeneratedCodeKind) <<
      ", err = " <<
      err <<
      endl;
  }
#endif
#endif

  return 0;
}
