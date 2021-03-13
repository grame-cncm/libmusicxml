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

#include "Mikrokosmos3WanderingGenerator.h"

#include "oahOah.h"
#include "generalOah.h"

#include "msrGenerationOah.h"

#include "Mikrokosmos3WanderingInsiderHandler.h"
#include "Mikrokosmos3WanderingRegularHandler.h"

#include "msr2lilypond.h"
#include "msr2musicxml.h"
#include "msr2braille.h"
#include "msr2guido.h"


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
static bool arguments2optionsVector (
  int argc, char *argv[], optionsVector& theOptionsVector)
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

	for (unsigned int i = 0; i < stringsVector.size (); ++i) {
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
#ifdef TRACING_IS_ENABLED
        if (getTraceOah ()) {
          cerr << "--> str contains an argument: " << str << endl;
        }
#endif

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
void enforceSomeOptions (
  multiGeneratorOutputKind theGeneratorOutputKind)
//------------------------------------------------------------------------
{
  /*
    This is a way to enforce options 'permanently'
    independently of the options chosen by the user
  */

  // trace
  // ------------------------------------------------------

/*
  gGlobalTraceOahGroup->setTraceScores ();
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
    case multiGeneratorOutputKind::k_NoOutput:
      // should not occur
      break;

    case multiGeneratorOutputKind::kGuidoOutput:
/*
     gGlobalMxmltreeGenerationOahGroup->setTraceDivisions ();
*/
     break;

    case multiGeneratorOutputKind::kLilyPondOutput:
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
      gGlobalLilypondGenerationOahGroup->setLilypondCompileDate (); // JMI NOT OK
      gGlobalLilypondGenerationOahGroup->setInputLineNumbers ();
*/
      break;

    case multiGeneratorOutputKind::kBrailleOutput:
      // BSR
      // ------------------------------------------------------

/*
      gGlobalBsrOahGroup->setTraceBsr ();
      //  gGlobalBsrOahGroup->setTraceBsrVisitors ();
*/

      // bsr2braille
      // ------------------------------------------------------

/*
      gGlobalBrailleGenerationOahGroup->
        setBrailleOutputKind (kBrailleOutputUTF8Debug);
*/
      break;

    case multiGeneratorOutputKind::kMusicXMLOutput:
      // MusicXML
      // ------------------------------------------------------

/*
      gGlobalMxmltreeGenerationOahGroup->setTraceDivisions ();

      gGlobalMxmlTreeOahGroup->setTraceMusicXMLTreeVisitors ();
*/
      break;

    case multiGeneratorOutputKind::kMidiOutput:
/*
*/
      break;
  } // switch
}

//------------------------------------------------------------------------
int main (int argc, char * argv[])
//------------------------------------------------------------------------
{
  // setup signals catching
  // ------------------------------------------------------

	catchSignals ();

  // the executable name
  // ------------------------------------------------------

  string executableName = argv [0];

  // are there insider/regular or trace OAH options present?
  // ------------------------------------------------------

  bool insiderOptions = false;
  bool regularOptions = false;

	for (int i = 1; i < argc; ++i) {
	  string argumentAsString = string (argv [i]);

    string argumentWithoutDash = argumentAsString.substr (1);

#ifdef TRACING_IS_ENABLED
    if (
      argumentWithoutDash == K_TRACE_OAH_SHORT_OPTION_NAME
        ||
      argumentWithoutDash == K_TRACE_OAH_LONG_OPTION_NAME
    ) {
      setTraceOah ();
    }
#endif

		if (argumentWithoutDash == K_INSIDER_OPTION_NAME) {
		  insiderOptions = true;
		}
		if (argumentWithoutDash == K_REGULAR_OPTION_NAME) {
		  regularOptions = true;
		}
	} // for

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    cerr <<
      executableName << " main()" <<
      ", insiderOptions: " << booleanAsString (insiderOptions) <<
      ", regularOptions: " << booleanAsString (regularOptions) <<
      endl;
  }
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
	  // arguments2optionsVector() returned false
    cerr <<
      executableName <<
      " does not take arguments, quitting" <<
      endl;

    return 1;
	}

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    displayOptionsVector (theOptionsVector, cerr);
  }
#endif

  // fetch the generated output kind from theOptionsVector,
  // since the OAH handler should only use the OAH groups needed for it
  // ------------------------------------------------------

  multiGeneratorOutputKind
    theGeneratorOutputKind = multiGeneratorOutputKind::k_NoOutput;

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
        multiGeneratorOutputKindFromString (optionValue);
    }
    else if (optionNameWithoutDash == K_GENERATED_OUTPUT_KIND_GUIDO_NAME) {
      theGeneratorOutputKind = multiGeneratorOutputKind::kGuidoOutput;
    }
    else if (optionNameWithoutDash == K_GENERATED_OUTPUT_KIND_LIlYPOND_NAME) {
      theGeneratorOutputKind = multiGeneratorOutputKind::kLilyPondOutput;
    }
    else if (optionNameWithoutDash == K_GENERATED_OUTPUT_KIND_BRAILLE_NAME) {
      theGeneratorOutputKind = multiGeneratorOutputKind::kBrailleOutput;
    }
    else if (optionNameWithoutDash == K_GENERATED_OUTPUT_KIND_MUSICXML_NAME) {
      theGeneratorOutputKind = multiGeneratorOutputKind::kMusicXMLOutput;
    }
    else if (optionNameWithoutDash == K_GENERATED_OUTPUT_KIND_MIDI_NAME) {
      theGeneratorOutputKind = multiGeneratorOutputKind::kMidiOutput;
    }
  } //for


#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    cerr <<
      "==> multiGeneratorOutputKind: " <<
      multiGeneratorOutputKindAsString (theGeneratorOutputKind) <<
      endl;
  }
#endif

  switch (theGeneratorOutputKind) {
    case multiGeneratorOutputKind::k_NoOutput:
      // wait until after help options have been handled
      // before issuing an error message
      break;

    case multiGeneratorOutputKind::kGuidoOutput:
    case multiGeneratorOutputKind::kLilyPondOutput:
    case multiGeneratorOutputKind::kBrailleOutput:
    case multiGeneratorOutputKind::kMusicXMLOutput:
      break;

    case multiGeneratorOutputKind::kMidiOutput:
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
    // create a generators insider OAH handler
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
      // use the insider generators OAH handler
      handler = insiderOahHandler;
    }
    else {
      // create a regular generators OAH handler
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
      case oahElementHelpOnlyKind::kElementHelpOnlyYes:
        return 0; // quit now
        break;
      case oahElementHelpOnlyKind::kElementHelpOnlyNo:
        // go ahead
        break;
    } // switch
  }
  catch (msgOahException& e) {
    displayException (e, gOutputStream);
    return kInvalidOption;
  }
  catch (exception& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }

  // has the output kind been chosen?
  // ------------------------------------------------------

  switch (theGeneratorOutputKind) {
    case multiGeneratorOutputKind::k_NoOutput:
      {
        stringstream s;

        s <<
          executableName <<
          " needs the output kind to be suplied thru one of the options " <<
          "'-" << K_GENERATED_OUTPUT_KIND_SHORT_NAME << "'" <<
          ", -'" << K_GENERATED_OUTPUT_KIND_LONG_NAME << "'" <<
          ", -'" << K_GENERATED_OUTPUT_KIND_GUIDO_NAME << "'" <<
          ", -'" << K_GENERATED_OUTPUT_KIND_LIlYPOND_NAME << "'" <<
          ", -'" << K_GENERATED_OUTPUT_KIND_BRAILLE_NAME << "'" <<
          ", -'" << K_GENERATED_OUTPUT_KIND_MUSICXML_NAME << "'" <<
          " or '" << K_GENERATED_OUTPUT_KIND_MIDI_NAME << "'" <<
          ", otherwise the run is a pure help one";

        oahWarning (s.str ());
      }
      break;

    default:
      ;
  } // switch

  // check indentation
  // ------------------------------------------------------

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

  // display the OAH handler if needed
  // ------------------------------------------------------

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
      gGlobalMsrGeneratorsOahGroup->
        getGenerationAPIKind ();

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    cerr <<
      "==> generationAPIKind: " <<
      msrGenerationAPIKindAsString (theGenerationAPIKind) <<
      endl;
  }
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

  // the generatorsGenerator
  S_Mikrokosmos3WanderingGenerator
    generator =
      Mikrokosmos3WanderingGenerator::create ();

  // create and populate the score
  S_msrScore
    theMsrScore =
      generator->
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
    case multiGeneratorOutputKind::k_NoOutput:
      // should not occur, unless the run is a pure help one
      break;

    case multiGeneratorOutputKind::kGuidoOutput:
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

    case multiGeneratorOutputKind::kLilyPondOutput:
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

    case multiGeneratorOutputKind::kBrailleOutput:
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

    case multiGeneratorOutputKind::kMusicXMLOutput:
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

    case multiGeneratorOutputKind::kMidiOutput:
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    if (err != 0) {
      cerr <<
        executableName << ", " <<
        multiGeneratorOutputKindAsString (theGeneratorOutputKind) <<
        ", err = " <<
        err <<
        endl;
    }
 }
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
      multiGeneratorOutputKindAsString (theGeneratorOutputKind) <<
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
