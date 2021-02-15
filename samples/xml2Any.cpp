/*

  Copyright (C) 2003-2008  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the MusicXML Library use.

*/

#include <fstream>      // ifstream, ...
#include <sstream>

#ifndef WIN32
#include <signal.h>
#endif

#include "libxml2ly.h"
#include "libxml2brl.h"
#include "libxml2xml.h"
#include "libxml2gmn.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "generalOah.h"

#include "waeMessagesHandling.h"

#include "generatorsBasicTypes.h"


using namespace std;
using namespace MusicXML2;


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
  sigaction (SIGILL, &sa, NULL);
  sigaction (SIGFPE, &sa, NULL);
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
  if (getTraceOah ()) {
    cerr << "arguments2optionsVector: stringsVector size: " << stringsVector.size() << endl;
    cerr << "==> stringsVector:" << endl;
    for (auto str: stringsVector) {
      cerr << "   " << str << endl;
    } // for
    cerr << endl;
  }
#endif

  // populate the optionsVector
  // ------------------------------------------------------

	string curOption;

	for (unsigned int i = 0; i < stringsVector.size () - 1; ++i) {
	  // -1 because the last argument is the input file name
	  string str = stringsVector [i];

#ifdef TRACING_IS_ENABLED
    if (getTraceOah ()) {
      cerr << "--> curOption: " << curOption << endl;
      cerr << "--> str      : " << str << endl;
    }
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
static generatorOutputKind gGeneratorOutputKind = k_NoOutput;

void registerGeneratorOutputKind (generatorOutputKind kind)
{
  if (gGeneratorOutputKind != k_NoOutput) {
    cerr << "only one of '-lilypond', '-braille' and '-musicxml' can be used" << endl;
    exit (2);
  }
  else {
    gGeneratorOutputKind = kind;
  }
}

//------------------------------------------------------------------------
// the main function
//------------------------------------------------------------------------
int main (int argc, char *argv[])
{
  // setup signals catching
  // ------------------------------------------------------

	catchSignals ();

  // the executable name
  // ------------------------------------------------------

  string executableName = argv [0];

  // fetch the input filename
  // ------------------------------------------------------

	const char* inputFileName = 0;

	if (argc > 1) {
	  // the file name is the last argument
		inputFileName = argv [argc - 1];
  }
	else {
		cerr <<
		  "usage: " <<
		  argv [0] <<
		  "[-lilypond | -braille | -musicxml] [options] -|inputFileName " <<
		  endl;
		return -1;
	}

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
  if (getTraceOah ()) {
    displayOptionsVector (theOptionsVector, cerr);
  }
#endif

  // take generatorOutputKind options into account if any
  // ------------------------------------------------------

	optionsVector keptOptions;

	for (auto option: theOptionsVector) {
	  if (option.first      == "-guido") {
	    registerGeneratorOutputKind (kGuidoOutput);
	  }
	  else if (option.first == "-lilypond") {
	    registerGeneratorOutputKind (kLilyPondOutput);
	  }
	  else if (option.first == "-braille") {
	    registerGeneratorOutputKind (kBrailleOutput);
	  }
	  else if (option.first == "-musicxml") {
	    registerGeneratorOutputKind (kMusicXMLOutput);
	  }
	  else if (option.first == "-midi") {
	    registerGeneratorOutputKind (kMidiOutput);
	  }
	  else {
	    keptOptions.push_back (option);
	  }
	} // for

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    displayOptionsVector (theOptionsVector, cerr);
  }
#endif

  // the default is '-lilypond'
  if (gGeneratorOutputKind == k_NoOutput) {
    gGeneratorOutputKind = kLilyPondOutput;
  }

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    cerr <<
      "==> generatorOutputKind: " <<
      generatorOutputKindAsString (gGeneratorOutputKind) <<
      endl;
  }
#endif

  // should we generate LilyPond, braille music or MusicXML?
  // ------------------------------------------------------

  xmlErr err = kNoErr;

  if (string (inputFileName) == "-") {

    // MusicXML data comes from standard input
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    cerr << "Reading standard input" << endl;
  }
#endif

    switch (gGeneratorOutputKind) {
      case k_NoOutput:
        // should not occur
        break;

      case kGuidoOutput:
        err =
          musicxmlfd2guido (
            stdin, keptOptions, cout, cerr);
        break;

      case kLilyPondOutput:
        err =
          musicxmlfd2lilypond (
            stdin, keptOptions, cout, cerr);
        break;

      case kBrailleOutput:
        err =
          musicxmlfd2braille (
            stdin, keptOptions, cout, cerr);
        break;

      case kMusicXMLOutput:
        err =
          musicxmlfd2musicxml (
            stdin, keptOptions, cout, cerr);
        break;

      case kMidiOutput:
        gLogStream <<
          "MIDI output is not implemented yet 2" <<
          endl;

        return 0;
        break;
    } // switch

#ifdef TRACING_IS_ENABLED
    if (getTraceOah ()) {
      if (err != 0) {
        cerr <<
          executableName <<
          ", " <<
          generatorOutputKindAsString (gGeneratorOutputKind) <<
          ", from stdin, err = " <<
          err <<
          endl;
      }
    }
#endif
  }

  else {
    // MusicXML data comes from a file
#ifdef TRACING_IS_ENABLED
    if (getTraceOah ()) {
      cerr << "Reading file '" << inputFileName << "'" << endl;
    }
#endif

    switch (gGeneratorOutputKind) {
      case k_NoOutput:
        // should not occur
        break;

      case kGuidoOutput:
        err =
          musicxmlfile2guido (
            inputFileName, keptOptions, cout, cerr);
        break;

      case kLilyPondOutput:
        err =
          musicxmlfile2lilypond (
            inputFileName, keptOptions, cout, cerr);
        break;

      case kBrailleOutput:
        err =
          musicxmlfile2braille (
            inputFileName, keptOptions, cout, cerr);
        break;

      case kMusicXMLOutput:
        err =
          musicxmlfile2musicxml (
            inputFileName, keptOptions, cout, cerr);
        break;

      case kMidiOutput:
        gLogStream <<
          "MIDI output is not implemented yet 3" <<
          endl;

        return 0;
        break;
    } // switch

#ifdef TRACING_IS_ENABLED
    if (getTraceOah ()) {
      if (err != 0) {
        cerr <<
          executableName <<
          ", " <<
          generatorOutputKindAsString (gGeneratorOutputKind) <<
          ", from a file \"" << inputFileName << "\", err = " <<
          err <<
          endl;
      }
    }
#endif
  }

  // display the input line numbers for which messages have been issued
  // ------------------------------------------------------

  displayWarningsAndErrorsInputLineNumbers ();

  // print timing information
  // ------------------------------------------------------


  // necessary test, in case this run terminates before any OAH handler is created
  if (gGlobalGeneralOahGroup) {
    if (gGlobalGeneralOahGroup->getDisplayCPUusage ()) {
      timing::gGlobalTiming.print (cerr);
    }
  }

  // check indentation
  // ------------------------------------------------------

  if (gIndenter != 0) {
    cerr <<
      "### " <<
      executableName <<
      " gIndenter final value: " <<
      gIndenter.getIndent () <<
      " ###" <<
      endl;

    gIndenter.resetToZero ();
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
