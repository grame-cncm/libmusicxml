/*

  Copyright (C) 2003-2008  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the MusicXML Library use.
*/

#include <iomanip>      // setw()), set::precision(), ...
#include <fstream>      // ifstream, ...

#ifndef WIN32
#include <signal.h>
#endif

#include "version.h"

#include "utilities.h"
#include "messagesHandling.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahBasicTypes.h"

#include "oahOah.h"
#include "generalOah.h"

#include "xml2xmlInsiderOahHandler.h"
#include "xml2xmlRegularOahHandler.h"

#include "musicxml2musicxml.h"


using namespace std;

using namespace MusicXML2;

/*
  ENFORCE_TRACE_OAH can be used to issue trace messages
  before gLogStream has been initialized
*/
//#define ENFORCE_TRACE_OAH

//_______________________________________________________________________________
#ifndef WIN32

static void _sigaction(int signal, siginfo_t *si, void *arg)
{
  cerr << "Signal #" << signal << " catched!" << endl;
  exit(-2);
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
static void catchsigs()	{}
#endif

//_______________________________________________________________________________
int main (int argc, char *argv[])
{
  // setup signals catching
  // ------------------------------------------------------

	catchsigs();

  // fetch executable name
  string executableName = argv [0];

  // are there insider and/or regular options present?
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

  // the oahHandler, set below
  // ------------------------------------------------------

  S_oahHandler handler;

  try {
    // create an xml2xml insider OAH handler
    // ------------------------------------------------------

    S_xml2xmlInsiderOahHandler
      insiderOahHandler =
        xml2xmlInsiderOahHandler::create (
          executableName,
          executableName + " insider OAH handler with argc/argv");

    // the OAH handler to be used, a regular handler is the default
    // ------------------------------------------------------

    if (insiderOptions) {
      // use the insider xml2xml OAH handler
      handler = insiderOahHandler;
    }
    else {
      // create a regular xml2xml OAH handler
      handler =
        xml2xmlRegularOahHandler::create (
          executableName,
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
      endl << endl;

    gIndenter.resetToZero ();
  }

  // let's go ahead
  // ------------------------------------------------------

  string
    inputSourceName =
      gGlobalOahOahGroup->getInputSourceName ();

  string
    outputFileName =
      handler->
        fetchOutputFileNameFromTheOptions ();

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    executableName << ": " <<
    "inputSourceName = \"" << inputSourceName << "\"" <<
    ", outputFileName = \"" << outputFileName << "\"" <<
    endl <<
    separator <<
    endl;
#endif
#endif

  // what if no input source name has been supplied?
  if (! inputSourceName.size ()) {
    if (handler->getOahHandlerFoundAHelpOption ()) {
      return 0; // pure help run
    }
    else {
      stringstream s;

      s <<
        "this is not a pure help run, \"" <<
        executableName <<
        " needs an input file name: " <<
        handler->getHandlerUsage ();

      oahError (s.str ());
    }
  }

  // has quiet mode been requested?
  // ------------------------------------------------------

  if (gGlobalGeneralOahGroup->getQuiet ()) {
    // disable all trace and display options
    handler->
      enforceHandlerQuietness ();
  }

  // welcome message
  // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePasses ()) {
    int
      outputFileNameSize =
        outputFileName.size ();

    gLogStream <<
      "This is \"" << executableName << "\" " << currentVersionNumber () <<
      " from libmusicxml2 v" << musicxmllibVersionStr () <<
      endl;

    gLogStream <<
      "Launching the conversion of ";

    if (inputSourceName == "-") {
      gLogStream <<
        "standard input";
    }
    else {
      gLogStream <<
        "\"" << inputSourceName << "\"";
    }

    gLogStream <<
      " back to MusicXML" <<
      endl;

    gLogStream <<
      "Time is " <<
      gGlobalGeneralOahGroup->getTranslationDateFull () <<
      endl;

    gLogStream <<
      "MusicXML code will be written to ";
    if (outputFileNameSize) {
      gLogStream <<
        outputFileName;
    }
    else {
      gLogStream <<
        "standard output";
    }
    gLogStream <<
      endl << endl;

    gLogStream <<
      "The command line is:" <<
      endl;

    gIndenter++;
    gLogStream <<
      handler->
        commandLineWithShortNamesAsString () <<
      endl;
    gIndenter--;

    gLogStream <<
      "or:" <<
      endl;

    gIndenter++;
    gLogStream <<
      handler->
        commandLineWithLongNamesAsString () <<
      endl << endl;
    gIndenter--;
  }
#endif

  // acknoledge end of command line analysis
  // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePasses ()) {
    gLogStream <<
      "The command line options and arguments have been analyzed" <<
      endl;
  }
#endif

  // sanity check // in xml2xmlInsiderOahGroup ??? JMI
  // ------------------------------------------------------

  if (inputSourceName.size () > 0 && inputSourceName == outputFileName) { // JMI
    stringstream s;

    s <<
      "\"" << inputSourceName << "\" is both the input and output file name";;

    oahError (s.str ());
  }

  // do the translation
  // ------------------------------------------------------

  xmlErr err;

  try {
    if (inputSourceName == "-") {
      // MusicXML data comes from standard input
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTracePasses ()) {
        cerr << "Reading standard input" << endl;
      }
#endif

      err =
        musicxmlFd2musicxmlWithHandler (
          stdin, cout, cerr, handler);
    }

    else {
      // MusicXML data comes from a file
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTracePasses ()) {
        cerr << "Reading file \"" << inputSourceName << "\"" << endl;
      }
#endif

      err =
        musicxmlFile2musicxmlWithHandler (
          inputSourceName.c_str(), cout, cerr, handler);
    }
  }
  catch (std::exception& e) {
    return kInvalidFile;
  }

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
      endl << endl;

    gIndenter.resetToZero ();
  }

  // over!
  // ------------------------------------------------------

  if (err != kNoErr) {
    gLogStream <<
      "### Conversion from MusicXML back to MusicXML code failed ###" <<
      endl << endl;

    return 1;
  }

  return 0;
}
