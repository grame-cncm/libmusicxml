/*

  Copyright (C) 2003-2008  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the MusicXML Library use.
*/

#include <iomanip>      // setw()), set::precision(), ...
#include <fstream>      // ofstream, ofstream::open(), ofstream::close()

#include "libmusicxml.h"
#include "version.h"

#include "utilities.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "generalOah.h"
#include "musicXMLOah.h"
#include "msrOah.h"
#include "lpsrOah.h"
#include "brailleOah.h"

#include "xml2lyOah.h"

#include "musicXML2MxmlTreeInterface.h"

#include "mxmlTree2MsrSkeletonBuilderInterface.h"
#include "mxmlTree2MsrTranslatorInterface.h"

#include "msr2LpsrInterface.h"

#include "lpsr2LilypondInterface.h"


using namespace std;

using namespace MusicXML2;


//_______________________________________________________________________________
vector<string> handleOptionsAndArguments (
  S_xml2lyOahHandler handler,
  int                argc,
  char*              argv [],
  indentedOstream&   logIndentedOutputStream)
{
  // analyse the options
  vector<string>
    argumentsVector =
      handler->
        decipherOptionsAndArguments (
          argc, argv);

  return argumentsVector;
}

//_______________________________________________________________________________
Sxmlelement convertMusicXMLToMxmlTree_Pass1 (
  string inputSourceName)
{
  Sxmlelement mxmlTree;

  if (inputSourceName == "-") {
    // input comes from standard input
    mxmlTree =
      musicXMLFd2mxmlTree (
        stdin,
        gMusicXMLOah,
        gLogOstream);
  }

  else {
    // input comes from a file

/* JMI
    // has the input file name a ".mxl" suffix?
    size_t
      posInString =
        inputSourceName.rfind (".mxl");

    if (posInString == inputSourceName.size () - 4) {
      stringstream s;

      s <<
        "compressed MusicXML files are not handled by xml2ly currently, exiting";

      msrMusicXMLError (
        inputSourceName,
        0,
        __FILE__, __LINE__,
        s.str ());
    }
*/

    // OK, let's go ahead
    mxmlTree =
      musicXMLFile2mxmlTree (
        inputSourceName.c_str(),
        gMusicXMLOah,
        gLogOstream);
  }

  return mxmlTree;
}

//_______________________________________________________________________________
S_msrScore convertMxmlTreeToAScoreSkeleton_Pass2a (
  Sxmlelement mxmlTree)
{
  S_msrScore
    mScore =
      buildMsrSkeletonFromElementsTree (
        gMsrOah,
        mxmlTree,
        gLogOstream);

  if (gIndenter != 0) {
    if (! gGeneralOah->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after pass 2a: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gOahOah->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }

  if (! mScore) {
    gLogOstream <<
      "### Conversion from MusicCML to an MSR skeleton failed ###" <<
      endl <<
      endl;

    exit (1);
  }

  return mScore;
}

//_______________________________________________________________________________
void populateScoreSkeletonFromMusicXML_Pass2b (
  Sxmlelement mxmlTree,
  S_msrScore  scoreSkeleton)
{
  populateMsrSkeletonFromMxmlTree (
    gMsrOah,
    mxmlTree,
    scoreSkeleton,
    gLogOstream);

  if (gIndenter != 0) {
    if (! gGeneralOah->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after pass 2b: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gOahOah->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

//_______________________________________________________________________________
void displayMsrScore_OptionalPass (
  S_msrScore   mScore,
  S_msrOah msrOpts)
{
  // display the MSR
  displayMSRPopulatedScore (
    msrOpts,
    mScore,
    gLogOstream);

  if (gIndenter != 0) {
    if (! gGeneralOah->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after MSR score display: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gOahOah->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

//_______________________________________________________________________________
S_lpsrScore convertMsrScoreToLpsrScore_Pass3 (
  S_msrScore mScore)
{
  S_lpsrScore lpScore;

  if (gLilypondOah->fNoLilypondCode) {
    gLogOstream <<
      "Option '-nolpc, -no-lilypond-code' is set, no LPSR is created" <<
      endl <<
      endl;
  }
  else {
    lpScore =
      buildLpsrScoreFromMsrScore (
        mScore,
        gMsrOah,
        gLpsrOah,
        gLogOstream);
  }

  if (gIndenter != 0) {
    if (! gGeneralOah->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after pass 3: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gOahOah->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }

  if (! lpScore && ! gLilypondOah->fNoLilypondCode) {
    gLogOstream <<
      "### Conversion from MSR to LPSR failed ###" <<
      endl <<
      endl;

    exit (2);
  }

  return lpScore;
}

//_______________________________________________________________________________
void displayLpsrScore_OptionalPass (
  S_lpsrScore   lpScore,
  S_msrOah  msrOpts,
  S_lpsrOah lpsrOpts)
{
  // display it
  displayLpsrScore (
    lpScore,
    msrOpts,
    lpsrOpts,
    gLogOstream);

  if (gIndenter != 0) {
    if (! gGeneralOah->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after LPSR score display: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gOahOah->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

//_______________________________________________________________________________
void convertLpsrScoreToLilypondCode_Pass4 (
  string     outputFileName,
  S_lpsrScore lpScore)
{
  int outputFileNameSize = outputFileName.size ();

  if (gLilypondOah->fNoLilypondCode) {
    gLogOstream <<
      "Option '-nolpc -no-lilypond-code' is set, no LilyPond code is created" <<
      endl <<
      endl;
  }
  else {
    // open output file if need be
    // ------------------------------------------------------

    ofstream outFileStream;

    if (outputFileNameSize) {
#ifdef TRACE_OAH
      if (gTraceOah->fTracePasses) {
        gLogOstream <<
          "Opening file '" << outputFileName << "' for writing" <<
          endl;
      }
#endif

      outFileStream.open (
        outputFileName.c_str(),
        ofstream::out);

      if (! outFileStream.is_open ()) {
        gLogOstream <<
          "Could not open LilyPond output file \"" <<
          outputFileName <<
          "\" for writing, exiting" <<
          endl;

        exit (9);
      }

      // create an indented output stream for the LilyPond code
      // to be written to outFileStream
      indentedOstream
        brailleCodeFileOutputStream (
          outFileStream,
          gIndenter);

      // convert the LPSR score to LilyPond code
      generateLilypondCodeFromLpsrScore (
        lpScore,
        gMsrOah,
        gLpsrOah,
        gLogOstream,
        brailleCodeFileOutputStream);
    }

    else {
#ifdef TRACE_OAH
      if (gTraceOah->fTracePasses) {
        gLogOstream <<
          endl <<
          "LilyPond code will be written to standard output" <<
          endl;
      }
#endif

      // create an indented output stream for the LilyPond code
      // to be written to cout
      indentedOstream
        brailleCodeCoutOutputStream (
          cout,
          gIndenter);

      // convert the LPSR score to LilyPond code
      generateLilypondCodeFromLpsrScore (
        lpScore,
        gMsrOah,
        gLpsrOah,
        gLogOstream,
        brailleCodeCoutOutputStream);
    }

    if (outputFileNameSize) {
#ifdef TRACE_OAH
      if (gTraceOah->fTracePasses) {
        gLogOstream <<
          endl <<
          "Closing file '" << outputFileName << "'" <<
          endl;
      }
#endif

      outFileStream.close ();
    }
  }

  if (gIndenter != 0) {
    if (! gGeneralOah->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after pass 4: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gOahOah->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

//_______________________________________________________________________________
void convertMusicXMLToLilypond (
  string     inputSourceName,
  string     outputFileName)
{
  // create the mxmxTree from MusicXML contents (pass 1)
  // ------------------------------------------------------

  Sxmlelement
    mxmlTree =
      convertMusicXMLToMxmlTree_Pass1 (
        inputSourceName);

  // create the MSR skeleton from MusicXML contents (pass 2a)
  // ------------------------------------------------------

  S_msrScore
    mScore =
      convertMxmlTreeToAScoreSkeleton_Pass2a (
        mxmlTree);

  if (gMsrOah->fExit2a) {
    gLogOstream <<
      endl <<
      "Existing after pass 2a as requested" <<
      endl;
    return;
  }

  // populate the MSR from MusicXML contents (pass 2b)
  // ------------------------------------------------------

  populateScoreSkeletonFromMusicXML_Pass2b (
    mxmlTree,
    mScore);

  if (gMsrOah->fExit2b) {
    gLogOstream <<
      endl <<
      "Existing after pass 2b as requested" <<
      endl;
    return;
  }

  // display the MSR score summary if requested
  // ------------------------------------------------------

  if (gMsrOah->fDisplayMsr) {
    displayMsrScore_OptionalPass (
      mScore,
      gMsrOah);
  }

  // display the score summary if requested
  // ------------------------------------------------------

  if (gMsrOah->fDisplayMsrSummary) {
    // display the score summary
    displayMSRPopulatedScoreSummary (
      gMsrOah,
      mScore,
      gLogOstream);

    exit (19);
  }

  // display the score names if requested
  // ------------------------------------------------------

  if (gMsrOah->fDisplayMsrNames) {
    // display the score name
    displayMSRPopulatedScoreNames (
      gMsrOah,
      mScore,
      gLogOstream);

    exit (19);
  }

  // create the LPSR from the MSR (pass 3)
  // ------------------------------------------------------

  S_lpsrScore
    lpScore =
      convertMsrScoreToLpsrScore_Pass3 (
        mScore);

  if (gLpsrOah->fExit3) {
    gLogOstream <<
      endl <<
      "Existing after pass 3 as requested" <<
      endl;
    return;
  }

  // display the LPSR score if requested
  // ------------------------------------------------------

  if (gLpsrOah->fDisplayLpsr) {
    displayLpsrScore_OptionalPass (
      lpScore,
      gMsrOah,
      gLpsrOah);
  }

  // generate LilyPond code from the LPSR (pass 4)
  // ------------------------------------------------------

  convertLpsrScoreToLilypondCode_Pass4 (
    outputFileName,
    lpScore);
}

//_______________________________________________________________________________
int main (int argc, char *argv[])
{
  // create the options handler
  // ------------------------------------------------------

  S_xml2lyOahHandler
    handler =
      xml2lyOahHandler::create (
        argv [0],
        gOutputOstream);

  // analyze the command line options and arguments
  // ------------------------------------------------------

  vector<string>
    argumentsVector =
      handleOptionsAndArguments (
        handler,
        argc, argv,
        gLogOstream);

  string
    inputSourceName =
      gOahOah->fInputSourceName;

  string
    outputFileName =
      gXml2lyOah->fOutputFileName;

  int
    outputFileNameSize =
      outputFileName.size ();

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
      "Time is " << gGeneralOah->fTranslationDate <<
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

