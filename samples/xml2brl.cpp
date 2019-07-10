/*

  Copyright (C) 2003-2008  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the MusicXML Library use.
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <iomanip>      // setw()), set::precision(), ...
#include <fstream>      // ofstream, ofstream::open(), ofstream::close()

#include "libmusicxml.h"
#include "version.h"

#include "utilities.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "generalOptions.h"
#include "musicXMLOptions.h"
#include "msrOptions.h"
#include "bsrOptions.h"
#include "brailleOptions.h"

#include "xml2brlOptionsHandling.h"

#include "musicXML2MxmlTreeInterface.h"

#include "mxmlTree2MsrSkeletonBuilderInterface.h"
#include "mxmlTree2MsrTranslatorInterface.h"

#include "msr2BsrInterface.h"
#include "bsr2BsrFinalizerInterface.h"

#include "bsr2BrailleTranslatorInterface.h"


using namespace std;

using namespace MusicXML2;


//_______________________________________________________________________________
vector<string> handleOptionsAndArguments (
  S_xml2brlOptionsHandler handler,
  int                    argc,
  char*                  argv [],
  indentedOstream&       logIndentedOutputStream)
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
        gMusicXMLOptions,
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
        "compressed MusicXML files are not handled by xml2brl currently, exiting";

      msrMusicXMLError (
        inputSourceName,
        0,
        __FILE__, __LINE__,
        s.str ());

      exit (1);
    }
*/

    // OK, let's go ahead
    mxmlTree =
      musicXMLFile2mxmlTree (
        inputSourceName.c_str(),
        gMusicXMLOptions,
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
        gMsrOptions,
        mxmlTree,
        gLogOstream);

  if (gIndenter != 0) {
    if (! gGeneralOptions->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after pass 2a: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gOahBasicOptions->fInputSourceName,
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
    gMsrOptions,
    mxmlTree,
    scoreSkeleton,
    gLogOstream);

  if (gIndenter != 0) {
    if (! gGeneralOptions->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after pass 2b: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gOahBasicOptions->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

//_______________________________________________________________________________
void displayMsrScore_OptionalPass (
  S_msrScore   mScore,
  S_msrOptions msrOpts)
{
  // display the MSR
  displayMSRPopulatedScore (
    msrOpts,
    mScore,
    gLogOstream);

  if (gIndenter != 0) {
    if (! gGeneralOptions->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after MSR score display: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gOahBasicOptions->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

//_______________________________________________________________________________
void displayBsrFirstScore_OptionalPass (
  S_bsrScore   bScore,
  S_msrOptions msrOpts,
  S_bsrOptions bsrOpts)
{
  // display it
  displayFirstBsrScore (
    bScore,
    msrOpts,
    bsrOpts,
    gLogOstream);

  if (gIndenter != 0) {
    if (! gGeneralOptions->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after first BSR score display: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gOahBasicOptions->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

//_______________________________________________________________________________
void displayBsrFinalizedScore_OptionalPass (
  S_bsrScore   bScore,
  S_msrOptions msrOpts,
  S_bsrOptions bsrOpts)
{
  // display it
  displayFinalizedBsrScore (
    bScore,
    msrOpts,
    bsrOpts,
    gLogOstream);

  if (gIndenter != 0) {
    if (! gGeneralOptions->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after finalized BSR score display: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gOahBasicOptions->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

//_______________________________________________________________________________
S_bsrScore convertMsrScoreToBsrScore_Pass3a (
  S_msrScore mScore)
{
  S_bsrScore bScore;

  if (! gBrailleOptions->fNoBrailleCode) {
    bScore =
      buildBsrScoreFromMsrScore (
        mScore,
        gMsrOptions,
        gBsrOptions,
        gLogOstream);
  }

  if (gIndenter != 0) {
    if (! gGeneralOptions->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after pass 3a: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gOahBasicOptions->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }

  if (! bScore) {
    gLogOstream <<
      "### Conversion from MSR to BSR failed ###" <<
      endl <<
      endl;

    exit (2);
  }

  return bScore;
}

//_______________________________________________________________________________
void displayFinalizedBsrScore_OptionalPass (
  S_bsrScore   bScore,
  S_msrOptions msrOpts,
  S_bsrOptions bsrOpts)
{
  // display it
  displayFinalizedBsrScore (
    bScore,
    msrOpts,
    bsrOpts,
    gLogOstream);

  if (gIndenter != 0) {
    if (! gGeneralOptions->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after BSR score display: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gOahBasicOptions->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

//_______________________________________________________________________________
S_bsrScore convertBsrScoreToFinalizedScore_Pass3b (
  S_bsrScore bScore)
{
  S_bsrScore finalizedBsrScore;

  if (! gBrailleOptions->fNoBrailleCode) {
    finalizedBsrScore =
      generateFinalizedBsrScoreFromBsrScore (
        bScore,
        gBsrOptions,
        gLogOstream);
  }

  if (gIndenter != 0) {
    if (! gGeneralOptions->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after pass 3b: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gOahBasicOptions->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }

  if (! finalizedBsrScore) {
    gLogOstream <<
      "### Conversion from first BSR to finalized BSR failed ###" <<
      endl <<
      endl;

    exit (2);
  }

  return finalizedBsrScore;
}

//_______________________________________________________________________________
void convertBsrScoreToBrailleText_Pass4 (
  string     outputFileName,
  S_bsrScore bScore)
{
  int outputFileNameSize = outputFileName.size ();

  if (! gBrailleOptions->fNoBrailleCode) {
    // open output file if need be
    // ------------------------------------------------------

    ofstream brailleCodeFileOutputStream;

    if (outputFileNameSize) {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTracePasses) {
        gLogOstream <<
          "Opening file '" << outputFileName << "' for writing" <<
          endl;
      }
#endif

      brailleCodeFileOutputStream.open (
        outputFileName.c_str(),
        ofstream::out);

      if (! brailleCodeFileOutputStream.is_open ()) {
        gLogOstream <<
          "Could not open braille music output file \"" <<
          outputFileName <<
          "\" for writing, exiting" <<
          endl;

        exit (9);
      }

/*
      // create an output stream for the Braille music
      // to be written to outFileStream
      ostream
        brailleCodeFileOutputStream (
          outFileStream,
          gIndenter);
      */

      // convert the BSR score to Braille music
      generateBrailleCodeFromBsrScore (
        bScore,
        gBsrOptions,
        gLogOstream,
        brailleCodeFileOutputStream);
    }

    else {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTracePasses) {
        gLogOstream <<
          endl <<
          "Braille music will be written to standard output" <<
          endl;
      }
#endif

      // create an indented output stream for the Braille music
      // to be written to cout
      indentedOstream
        brailleCodeCoutOutputStream (
          cout,
          gIndenter);

      // convert the BSR score to Braille music
      generateBrailleCodeFromBsrScore (
        bScore,
        gBsrOptions,
        gLogOstream,
        brailleCodeCoutOutputStream);
    }

    if (outputFileNameSize) {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTracePasses) {
        gLogOstream <<
          endl <<
          "Closing file '" << outputFileName << "'" <<
          endl;
      }
#endif

      brailleCodeFileOutputStream.close ();
    }
  }

  if (gIndenter != 0) {
    if (! gGeneralOptions->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after pass 4: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gOahBasicOptions->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

//_______________________________________________________________________________
void convertMusicXMLToBraille (
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

  if (gMsrOptions->fExit2a) {
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

  if (gMsrOptions->fExit2b) {
    gLogOstream <<
      endl <<
      "Existing after pass 2b as requested" <<
      endl;
    return;
  }


  // display the MSR score summary if requested
  // ------------------------------------------------------

  if (gMsrOptions->fDisplayMsr) {
    displayMsrScore_OptionalPass (
      mScore,
      gMsrOptions);
  }


  // display the score summary if requested
  // ------------------------------------------------------

  if (gMsrOptions->fDisplayMsrSummary) {
    // display the score summary
    displayMSRPopulatedScoreSummary (
      gMsrOptions,
      mScore,
      gLogOstream);

    exit (444);
  }


  // display the score names if requested
  // ------------------------------------------------------

  if (gMsrOptions->fDisplayMsrNames) {
    // display the score name
    displayMSRPopulatedScoreNames (
      gMsrOptions,
      mScore,
      gLogOstream);

    exit (555);
  }


  // create the BSR from the MSR (pass 3a)
  // ------------------------------------------------------

  S_bsrScore
    firstBsrScore =
      convertMsrScoreToBsrScore_Pass3a (
        mScore);

  if (gBsrOptions->fExit3a) {
    gLogOstream <<
      endl <<
      "Existing after pass 3a as requested" <<
      endl;
    return;
  }


  // display the first BSR score if requested
  // ------------------------------------------------------

  if (gBsrOptions->fDisplayBsr) {
    displayBsrFirstScore_OptionalPass (
      firstBsrScore,
      gMsrOptions,
      gBsrOptions);
  }

  // create the finalized BCR from the first BSR (pass 3b)
  // ------------------------------------------------------

  S_bsrScore
    finalizedBsrScore =
      convertBsrScoreToFinalizedScore_Pass3b (
        firstBsrScore);

  if (gBsrOptions->fExit3b) {
    gLogOstream <<
      endl <<
      "Existing after pass 3b as requested" <<
      endl;
    return;
  }


  // display the finalized BCR score if requested
  // ------------------------------------------------------

  if (gBsrOptions->fDisplayBsr) {
    displayFinalizedBsrScore_OptionalPass (
      finalizedBsrScore,
      gMsrOptions,
      gBsrOptions);
  }


  // generate Braille music text from the BCR (pass 4)
  // ------------------------------------------------------

  convertBsrScoreToBrailleText_Pass4 (
    outputFileName,
    finalizedBsrScore);
}

//_______________________________________________________________________________
int main (int argc, char *argv[])
{
  // create the options handler
  // ------------------------------------------------------

  S_xml2brlOptionsHandler
    handler =
      xml2brlOptionsHandler::create (
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

/* JMI
  // print the resulting options
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fDisplayOptionsHandler) {
    gLogOstream <<
      handler <<
      endl <<
      endl;
  }
#endif
*/

  string
    inputSourceName =
      gOahBasicOptions->fInputSourceName;

  string
    outputFileName =
      gXml2brlOptions->fOutputFileName;

  int
    outputFileNameSize =
      outputFileName.size ();

  // has quiet mode been requested?
  // ------------------------------------------------------

  if (gGeneralOptions->fQuiet) {
    // disable all trace and display options
    handler->
      enforceOptionsHandlerQuietness ();
  }

  // welcome message
  // ------------------------------------------------------

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePasses) {
    gLogOstream <<
      "This is xml2brl " << currentVersionNumber () <<
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
      " to Braille music" <<
      endl;

    gLogOstream <<
      "Time is " << gGeneralOptions->fTranslationDate <<
      endl;

    gLogOstream <<
      "Braille music will be written to ";
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

  // print the chosen options if so chosen
  // ------------------------------------------------------

#ifdef TRACE_OPTIONS
  if (gOahBasicOptions->fDisplayOptionsValues) {
    handler->
      printAllOptionsValues (
        gLogOstream);

    gLogOstream << endl;
  }
#endif

  // acknoledge end of command line analysis
  // ------------------------------------------------------

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePasses) {
    gLogOstream <<
      "The command line options and arguments have been analyzed" <<
      endl;
  }
#endif

  // do the translation
  // ------------------------------------------------------

  convertMusicXMLToBraille (
    inputSourceName,
    outputFileName);

  // display the input line numbers for which messages have been issued
  // ------------------------------------------------------

  displayWarningsAndErrorsInputLineNumbers ();

  // print timing information
  // ------------------------------------------------------

  if (gGeneralOptions->fDisplayCPUusage)
    timing::gTiming.print (
      gLogOstream);

  // check indentation
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
      "### Conversion from BSR to Braille music failed ###" <<
      endl <<
      endl;

    return 1;
  }

  return 0;
}

