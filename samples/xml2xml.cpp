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

#include <regex>

#include "version.h"

#include "utilities.h"
#include "messagesHandling.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "oahBasicTypes.h"
#include "generalOah.h"

#include "musicxml2musicxml.h"

#include "xml2xmlInsiderOahHandler.h"
#include "xml2xmlRegularOahHandler.h"


using namespace std;

using namespace MusicXML2;

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
    "xml2xml main()" <<
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

  // create the OAH handler, a regular handler is the default
  // ------------------------------------------------------

  S_oahHandler handler;

  if (insiderOptions) {
    // create an insider xml2xml OAH handler
    handler =
      xml2xmlInsiderOahHandler::create (
        executableName,
        "xml2xml with insider options",
        gOutputOstream);

  }
  else {
    // create a regular xml2xml OAH handler
    handler =
      xml2xmlRegularOahHandler::create (
        executableName,
        "xml2xml with regular options",
        gOutputOstream);
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
      gGlobalXml2xmlOahGroup->
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
    /* JMI
    twoViewHandler->
      enforceOahHandlerQuietness ();
      */
  }

  // welcome message
  // ------------------------------------------------------

#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->getTracePasses ()) {
    int
      outputFileNameSize =
        outputFileName.size ();

    gLogOstream <<
      "This is xml2xml " << currentVersionNumber () <<
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
      " back to MusicXML" <<
      endl;

    gLogOstream <<
      "Time is " << gGlobalGeneralOahGroup->fTranslationDateFull <<
      endl;

    gLogOstream <<
      "MusicXML code will be written to ";
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

  // sanity check
  // ------------------------------------------------------

  if (inputSourceName == outputFileName) {
    stringstream s;

    s <<
      "\"" << inputSourceName << "\" is both the input and output file name";;

    oahError (s.str ());
  }

  // do the translation
  // ------------------------------------------------------

  xmlErr err;

  try {
    err =
      convertMusicXMLBackToMusicXML (
        inputSourceName,
        outputFileName);
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
      "### xml2xml gIndenter final value: "<< gIndenter.getIndent () << " ###" <<
      endl << endl;
  }

  // over!
  // ------------------------------------------------------

  if (err != kNoErr) {
    gLogOstream <<
      "### Conversion from MusicXML to MusicXML code failed ###" <<
      endl << endl;

    return 1;
  }

  return 0;
}

