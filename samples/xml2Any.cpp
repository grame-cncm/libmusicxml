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

#include "messagesHandling.h"

using namespace std;
using namespace MusicXML2;

/*
  ENFORCE_TRACE_OAH can be used to issue trace messages
  before gGlobalOahOahGroup->fTrace has been initialized
*/
//#define ENFORCE_TRACE_OAH

//_______________________________________________________________________________
#ifndef WIN32

static void _sigaction (int signal, siginfo_t *si, void *arg)
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
  sigaction (SIGILL, &sa, NULL);
  sigaction (SIGFPE, &sa, NULL);
}

#else
static void catchsigs () {}
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

	for (unsigned int i = 0; i < stringsVector.size () - 1; ++i) {
	  // -1 because the last argument is the input file name
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

enum generatedCodeKind {
  k_NoGeneratedCode,
  kGuido, kLilyPond, kBrailleMusic, kMusicXML };

static string generatedCodeKindAsString (generatedCodeKind kind)
{
  string result;

  switch (kind) {
    case k_NoGeneratedCode:
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
static generatedCodeKind gGeneratedCodeKind = k_NoGeneratedCode;

void registerGeneratedCodeKind (generatedCodeKind kind)
{
  if (gGeneratedCodeKind != k_NoGeneratedCode) {
    cerr << "only one of '-lilypond', '-braille' and '-musicxml' can be used" << endl;
    exit (2);
  }
  else {
    gGeneratedCodeKind = kind;
  }
}

//------------------------------------------------------------------------
// the main function
//------------------------------------------------------------------------
int main (int argc, char *argv[])
{
  // setup signals catching
  // ------------------------------------------------------

	catchsigs ();

  // the executable name
  // ------------------------------------------------------

  string executableName = argv [0];

  // fetch the input filename
  // ------------------------------------------------------

	const char* fileName = 0;

	if (argc > 1) {
		fileName = argv [argc - 1];
  }
	else {
		cerr <<
		  "usage: " <<
		  argv [0] <<
		  "[-lilypond | -braille | -musicxml] [options] -|fileName " <<
		  endl;
		return -1;
	}

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
	  }
	  else if (option.first == "-lilypond") {
	    registerGeneratedCodeKind (kLilyPond);
	  }
	  else if (option.first == "-braille") {
	    registerGeneratedCodeKind (kBrailleMusic);
	  }
	  else if (option.first == "-musicxml") {
	    registerGeneratedCodeKind (kMusicXML);
	  }
	  else {
	    keptOptions.push_back (option);
	  }
	} // for

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  displayOptionsVector (theOptionsVector, cerr);
#endif
#endif

  // the default is '-lilypond'
  if (gGeneratedCodeKind == k_NoGeneratedCode) {
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

  // should we generate LilyPond, braille music or MusicXML?
  // ------------------------------------------------------

  xmlErr err = kNoErr;

  if (string (fileName) == "-") {

    // MusicXML data comes from standard input
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
    cerr << "Reading standard input" << endl;
#endif
#endif

    switch (gGeneratedCodeKind) {
      case k_NoGeneratedCode:
        // should not occur
        break;
      case kGuido:
        err =
          musicxmlfd2guido (
            stdin, keptOptions, cout, cerr);
        break;
      case kLilyPond:
        err =
          musicxmlfd2lilypond (
            stdin, keptOptions, cout, cerr);
        break;
      case kBrailleMusic:
        err =
          musicxmlfd2braille (
            stdin, keptOptions, cout, cerr);
        break;
      case kMusicXML:
        err =
          musicxmlfd2musicxml (
            stdin, keptOptions, cout, cerr);
        break;
    } // switch

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
    if (err != 0) {
      cerr <<
        executableName <<
        ", " <<
        generatedCodeKindAsString (gGeneratedCodeKind) <<
        ", from stdin, err = " <<
        err <<
        endl;
    }
#endif
#endif
  }

  else {
    // MusicXML data comes from a file
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
    cerr << "Reading file '" << fileName << "'" << endl;
#endif
#endif

    switch (gGeneratedCodeKind) {
      case k_NoGeneratedCode:
        // should not occur
        break;
      case kGuido:
        err =
          musicxmlfile2guido (
            fileName, keptOptions, cout, cerr);
        break;
      case kLilyPond:
        err =
          musicxmlfile2lilypond (
            fileName, keptOptions, cout, cerr);
        break;
      case kBrailleMusic:
        err =
          musicxmlfile2braille (
            fileName, keptOptions, cout, cerr);
        break;
      case kMusicXML:
        err =
          musicxmlfile2musicxml (
            fileName, keptOptions, cout, cerr);
        break;
    } // switch

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
    if (err != 0) {
      cerr <<
        executableName <<
        ", " <<
        generatedCodeKindAsString (gGeneratedCodeKind) <<
        ", from a file \"" << fileName << "\", err = " <<
        err <<
        endl;
    }
#endif
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
