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

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "generalOah.h"
#include "mxmlTreeOah.h"
#include "msr2LpsrOah.h"

#include "xml2lyWithOptionsVectorOah.h"

#include "musicXML2MxmlTreeInterface.h"

#include "mxmlTree2MsrSkeletonBuilderInterface.h"
#include "mxmlTree2MsrTranslatorInterface.h"

#include "msr2LpsrInterface.h"

#include "lpsr2LilypondInterface.h"

#include "msr2MxmltreeInterface.h"

#include "mxmlTree.h"


using namespace std;

using namespace MusicXML2;


//_______________________________________________________________________________
#ifndef WIN32

static void _sigaction(int signal, siginfo_t *si, void *arg)
{
    cerr << "Signal #" << signal << " catched!" << endl;
    exit(-2);
}

static void catchsigs()
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
const string kGlobalStaffSize = "33.5";

//_______________________________________________________________________________
void showUsageAndHelp (string executableName)
{
  gOutputOstream <<
    "About: " <<
      endl <<
    gTab << executableName << " demonstrates the use of optionsVector." <<
      endl <<
    gTab << "It converts MusicXML data to LilyPond code, with a global staff size of "  <<
      kGlobalStaffSize <<
      endl <<
    "Usage: " <<
      endl <<
    gTab << executableName <<
      " (-h | -help)" <<
      endl <<
    gTab << executableName <<
      " <inputFileName> [<outputFileName>]" <<
      endl <<
    gTab << gTab <<
      "('-' as input file name means standard input)" <<
      endl <<
    gTab << gTab <<
      "(by default, the LilyPond code goes to standard output)" <<
      endl;
}

//_______________________________________________________________________________
int main (int argc, char *argv[])
{
  // create the signals
  // ------------------------------------------------------

	catchsigs();

  // create and populate theOptionsVector
  // ------------------------------------------------------

  optionsVector theOptionsVector = {
    make_pair ("-global-staff-size", kGlobalStaffSize)
  };

  // analyze the command line options and arguments
  // ------------------------------------------------------

  string executableName = argv [0];

  string inputSourceName;
  string outputFileName;

  switch (argc) {
    case 1:
      showUsageAndHelp (executableName);
      exit (1);
      break;

    case 2:
      {
        string argument = argv [1];

        if (argument == "-h" || argument == "-help") {
          showUsageAndHelp (executableName);
          exit (1);
        }
        else {
          // set the input file name
          inputSourceName = argument;
          // an empty outputFileName means the LilyPond code goes to standard output
        }
      }
      break;

    case 3:
      // set the input file name
      inputSourceName = argv [1];
      // set the output file name
      outputFileName = argv [2];
      break;

    default:
    /* JMI
      // accept a limited set of options
      for (int i = 3; i < argc; i++) {
        string element = argv [i];

        if (element == "-aofn" or element == "-auto-output-file-name") {
          theOptionsVector.push_back (
            make_pair (element, ""));
        }
        else {
          showUsage (executableName);
        }
      } // for
      */
      showUsageAndHelp (executableName);
      exit (1);
  } // switch

  // create the options handler
  // ------------------------------------------------------

  S_xml2lyWithOptionsVectorOahHandler
    handler =
      xml2lyWithOptionsVectorOahHandler::create (
        executableName,
        gOutputOstream);

  gOahOah->fInputSourceName = inputSourceName;

  int
    outputFileNameSize =
      outputFileName.size ();

  gXml2lyWithOptionsVectorOah->fLilyPondOutputFileName = outputFileName;

#ifdef TRACE_OAH
  // only now, so that gTraceOah has been set
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> executableName: '" << executableName << "'" <<
      "==> inputSourceName: '" << inputSourceName << "'" <<
      "==> outputFileName: '" << outputFileName << "'" <<
      endl;
  }
#endif

  // handle theOptionsVector
  // ------------------------------------------------------

  vector<string>
    argumentsVector =
      handler->
        hangleOptionsFromOptionsVector (
          executableName,
          theOptionsVector);

  // has quiet mode been requested?
  // ------------------------------------------------------

  if (gGeneralOah->fQuiet) {
    // disable all trace and display options
    handler->
      enforceOahHandlerQuietness ();
  }

  // welcome message
  // ------------------------------------------------------

#ifdef TRACE_OAH
  if (gTraceOah->fTracePasses) {
    gLogOstream <<
      "This is xml2lyWithOptionsVector " << currentVersionNumber () <<
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
      "Time is " << gGeneralOah->fTranslationDateFull <<
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
      endl <<
      endl;

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
      endl <<
      endl;

    gIndenter--;
  }
#endif

  // acknoledge end of command line analysis
  // ------------------------------------------------------

#ifdef TRACE_OAH
  if (gTraceOah->fTracePasses) {
    gLogOstream <<
      "The command line options and arguments have been analyzed" <<
      endl;
  }
#endif

  // do the translation
  // ------------------------------------------------------

  convertMusicXMLToLilypond (
    inputSourceName,
    outputFileName);

  // display the input line numbers for which messages have been issued
  // ------------------------------------------------------

  displayWarningsAndErrorsInputLineNumbers ();

  // print timing information
  // ------------------------------------------------------

  if (gGeneralOah->fDisplayCPUusage)
    timing::gTiming.print (
      gLogOstream);

  // check indentation
  // ------------------------------------------------------
  if (gIndenter != 0) {
    gLogOstream <<
      "### gIndenter final value: "<< gIndenter.getIndent () << " ###" <<
      endl <<
      endl;

    // JMI abort ();
  }

  // over!
  // ------------------------------------------------------

  if (! true) { // JMI
    gLogOstream <<
      "### Conversion from LPSR to LilyPond code failed ###" <<
      endl <<
      endl;

    return 1;
  }

  return 0;
}

