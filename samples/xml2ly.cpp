/*

  Copyright (C) 2003-2008  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the MusicXML Library use.
*/

#include <iomanip>      // setw()), set::precision(), ...

#ifndef WIN32
#include <signal.h>
#endif

#include "libmusicxml.h"
#include "version.h"

#include "utilities.h"
#include "messagesHandling.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "oahBasicTypes.h"
#include "generalOah.h"

#include "xml2lyInsiderOahHandler.h"
#include "xml2lyRegularOahHandler.h"

#include "musicxml2lilypond.h"
#include "musicxml2musicxml.h"


using namespace std;

using namespace MusicXML2;

/*
  OAH_TRACE can be used to issue trace messages
  before gGlobalOahOahGroup->fTrace has been initialized
*/
//#define OAH_TRACE

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
  // create the signals
  // ------------------------------------------------------

	catchsigs();

  string executableName = argv [0];

  // are insider and/or regular options present?
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

#ifdef TRACE_OAH
#ifdef OAH_TRACE
  gLogOstream <<
    "xml2ly main()" <<
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

  // create the xml2ly OAH handler
  // ------------------------------------------------------

  // an xml2ly insider OAH handler is needed in all cases, create it
  S_xml2lyInsiderOahHandler insiderOahHandler;

  try {
    insiderOahHandler =
      xml2lyInsiderOahHandler::create (
        executableName,
        "xml2ly - insider OAH handler",
        gOutputOstream);
  }
  catch (msrOahException& e) {
    return kInvalidOption;
  }
  catch (std::exception& e) {
    return kInvalidFile;
  }

  // the OAH handler, a regular OAH handler is the default
  S_oahHandler handler;

  if (insiderOptions) {
    handler = insiderOahHandler;
  }

  else {
    // create an xml2ly regular OAH handler
    try {
      handler =
        xml2lyRegularOahHandler::createFromInsiderHandler (
          executableName,
          "xml2ly - regular OAH handler",
          insiderOahHandler,
          gOutputOstream);
    }
    catch (msrOahException& e) {
      return kInvalidOption;
    }
    catch (std::exception& e) {
      return kInvalidFile;
    }
  }

  // analyze the command line options and arguments
  // ------------------------------------------------------

  try {
    oahHandler::oahHelpOptionsHaveBeenUsedKind
      helpOptionsHaveBeenUsedKind =
        handler->
          applyOptionsAndArgumentsFromArgcAndArgv (
            argc, argv);

    switch (helpOptionsHaveBeenUsedKind) {
      case oahHandler::kHelpOptionsHaveBeenUsedYes:
        return kNoErr;
        break;
      case oahHandler::kHelpOptionsHaveBeenUsedNo:
        // let's go ahead!
        break;
    } // switch
  }
  catch (msrOahException& e) {
    return kInvalidOption;
  }
  catch (std::exception& e) {
    return kInvalidFile;
  }

  string
    inputSourceName =
      gGlobalOahOahGroup->fInputSourceName;

  string
    outputFileName =
      gGlobalXml2lyInsiderOahGroup->
        getOutputFileNameStringAtom ()->
          getStringVariable ();

#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->getTracePasses ()) {
    string separator =
      "%--------------------------------------------------------------";

    gLogOstream <<
      executableName << ": " <<
      "inputSourceName = \"" << inputSourceName << "\"" <<
      ", outputFileName = \"" << outputFileName << "\"" <<
      endl <<
      separator <<
      endl;
  }
#endif

  // has quiet mode been requested?
  // ------------------------------------------------------

  if (gGlobalGeneralOahGroup->fQuiet) {
    // disable all trace and display options
    handler->
      enforceHandlerQuietness ();
  }

  // welcome message
  // ------------------------------------------------------

#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->getTracePasses ()) {
    int
      outputFileNameSize =
        outputFileName.size ();

    gLogOstream <<
      "This is xml2ly " << currentVersionNumber () <<
      " from libmusicxml2 v" << musicxmllibVersionStr () <<
      endl;

    gLogOstream <<
      "Launching conversion of ";

    if (inputSourceName == "-")
      gLogOstream <<
        "standard input";
    else
      gLogOstream <<
        "\"" << inputSourceName << "\"";

    gLogOstream <<
      " to LilyPond" <<
      endl;

    gLogOstream <<
      "Time is " << gGlobalGeneralOahGroup->fTranslationDateFull <<
      endl;

    gLogOstream <<
      "LilyPond code will be written to ";
    if (outputFileNameSize) {
      gLogOstream <<
        outputFileName;
    }
    else {
      gLogOstream <<
        "standard output";
    }
    gLogOstream <<
      endl << endl;

    gLogOstream <<
      "The command line is:" <<
      endl;

    gIndenter++;
    gLogOstream <<
      handler->
        commandLineWithShortNamesAsString () <<
      endl;
    gIndenter--;

    gLogOstream <<
      "or:" <<
      endl;
    gIndenter++;

    gLogOstream <<
      handler->
        commandLineWithLongNamesAsString () <<
      endl << endl;

    gIndenter--;
  }
#endif

  // acknoledge end of command line analysis
  // ------------------------------------------------------

#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->getTracePasses ()) {
    gLogOstream <<
      "The command line options and arguments have been analyzed" <<
      endl;
  }
#endif

  // do the translation, creating MusicXML back from the MSR if requested
  // ------------------------------------------------------

  xmlErr err;

  try {
    err =
      convertMusicXMLToLilypond (
        inputSourceName,
        outputFileName,
        gGlobalXml2lyInsiderOahGroup->fLoopBackToMusicXML); // loopBackToMusicXML is used by 'xml2ly -loop'
  }
  catch (std::exception& e) {
    return kInvalidFile;
  }

  // display the input line numbers for which messages have been issued
  // ------------------------------------------------------

  displayWarningsAndErrorsInputLineNumbers ();

  // print timing information
  // ------------------------------------------------------

  if (gGlobalGeneralOahGroup->fDisplayCPUusage)
    timing::gTiming.print (
      gLogOstream);

  // check indentation
  // ------------------------------------------------------

  if (gIndenter != 0) {
    gLogOstream <<
      "### xml2ly gIndenter final value: "<< gIndenter.getIndent () << " ###" <<
      endl <<
      endl;
  }

  // over!
  // ------------------------------------------------------

  if (err != kNoErr) {
    gLogOstream <<
      "### Conversion from MusicXML to LilyPond code failed ###" <<
      endl <<
      endl;

    return 1;
  }

  return 0;
}
