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
#include "lpsrOptions.h"
#include "lilypondOptions.h"

#include "xml2lyOptionsHandling.h"

#include "musicXML2mxmlTreeInterface.h"

#include "mxmlTree2MsrSkeletonBuilderInterface.h"
#include "mxmlTree2MsrTranslatorInterface.h"

#include "msr2LpsrInterface.h"

#include "lpsr2LilypondInterface.h"


using namespace std;

using namespace MusicXML2;


//_______________________________________________________________________________
vector<string> handleOptionsAndArguments (
  S_xml2lyOptionsHandler optionsHandler,
  int                    argc,
  char*                  argv [],
  indentedOstream&       logIndentedOutputStream)
{
  // analyse the options
  vector<string>
    argumentsVector =
      optionsHandler->
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
        gLogIOstream);
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

      exit (1);
    }
*/

    // OK, let's go ahead
    mxmlTree =
      musicXMLFile2mxmlTree (
        inputSourceName.c_str(),
        gMusicXMLOptions,
        gLogIOstream);
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
        gLogIOstream);

  if (gIndenter != 0) {
    if (! gGeneralOptions->fQuiet) {
      stringstream s;
      
      s <<
        "gIndenter value after pass 2a: "<<
        gIndenter.getIndent ();
        
      msrMusicXMLWarning (
        gXml2lyOptions->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }

  if (! mScore) {
    gLogIOstream <<
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
    gLogIOstream);

  if (gIndenter != 0) {
    if (! gGeneralOptions->fQuiet) {
      stringstream s;
      
      s <<
        "gIndenter value after pass 2b: "<<
        gIndenter.getIndent ();
        
      msrMusicXMLWarning (
        gXml2lyOptions->fInputSourceName,
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
    gLogIOstream);

  if (gIndenter != 0) {
    if (! gGeneralOptions->fQuiet) {
      stringstream s;
      
      s <<
        "gIndenter value after MSR score display: "<<
        gIndenter.getIndent ();
        
      msrMusicXMLWarning (
        gXml2lyOptions->fInputSourceName,
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

  if (! gLilypondOptions->fNoLilypondCode) {
    lpScore =
      buildLpsrScoreFromMsrScore (
        mScore,
        gMsrOptions,
        gLpsrOptions,
        gLogIOstream);
  }

  if (gIndenter != 0) {
    if (! gGeneralOptions->fQuiet) {
      stringstream s;
      
      s <<
        "gIndenter value after pass 3: "<<
        gIndenter.getIndent ();
        
      msrMusicXMLWarning (
        gXml2lyOptions->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }

  if (! lpScore) {
    gLogIOstream <<
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
  S_msrOptions  msrOpts,
  S_lpsrOptions lpsrOpts)
{
  // display it
  displayLpsrScore (
    lpScore,
    msrOpts,
    lpsrOpts,
    gLogIOstream);

  if (gIndenter != 0) {
    if (! gGeneralOptions->fQuiet) {
      stringstream s;
      
      s <<
        "gIndenter value after LPSR score display: "<<
        gIndenter.getIndent ();
        
      msrMusicXMLWarning (
        gXml2lyOptions->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

//_______________________________________________________________________________
void convertLpsrScoreToLilypondCode_Pass4 (
  string      outputFileName,
  S_lpsrScore lpScore)
{  
  int outputFileNameSize = outputFileName.size ();

  if (! gLilypondOptions->fNoLilypondCode) {
    // open output file if need be
    // ------------------------------------------------------
  
    ofstream outFileStream;
        
    if (outputFileNameSize) {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTracePasses) {
        gLogIOstream <<
          "Opening file '" << outputFileName << "' for writing" <<
          endl;
      }
#endif
          
      outFileStream.open (
        outputFileName.c_str(),
        ofstream::out);

      // create an indented output stream for the LilyPond code
      // to be written to outFileStream
      indentedOstream
        lilypondCodeFileOutputStream (
          outFileStream,
          gIndenter);
      
      // convert the LPSR score to LilyPond code
      generateLilypondCodeFromLpsrScore (
        lpScore,
        gMsrOptions,
        gLpsrOptions,
        gLogIOstream,
        lilypondCodeFileOutputStream);
    }
    
    else {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTracePasses) {
        gLogIOstream <<
          endl <<
          "LilyPond code will be written to standard output" <<
          endl;
      }
#endif
          
      // create an indented output stream for the LilyPond code
      // to be written to cout
      indentedOstream
        lilypondCodeCoutOutputStream (
          cout,
          gIndenter);
      
      // convert the LPSR score to LilyPond code
      generateLilypondCodeFromLpsrScore (
        lpScore,
        gMsrOptions,
        gLpsrOptions,
        gLogIOstream,
        lilypondCodeCoutOutputStream);
    }

    if (outputFileNameSize) {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTracePasses) {
        gLogIOstream <<
          endl <<
          "Closing file '" << outputFileName << "'" <<
          endl;
      }
#endif
          
      outFileStream.close ();
    }
  }

  if (gIndenter != 0) {
    if (! gGeneralOptions->fQuiet) {
      stringstream s;
      
      s <<
        "gIndenter value after pass 4: "<<
        gIndenter.getIndent ();
        
      msrMusicXMLWarning (
        gXml2lyOptions->fInputSourceName,
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

  if (gGeneralOptions->fExit2a)
    return;

    
  // populate the MSR from MusicXML contents (pass 2b)
  // ------------------------------------------------------

  populateScoreSkeletonFromMusicXML_Pass2b (
    mxmlTree,
    mScore);
  
  if (gGeneralOptions->fExit2b)
    return;
    

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
      gLogIOstream);

    exit (444);
  }


  // display the score names if requested
  // ------------------------------------------------------

  if (gMsrOptions->fDisplayMsrNames) {
    // display the score name
    displayMSRPopulatedScoreNames (
      gMsrOptions,
      mScore,
      gLogIOstream);

    exit (555);
  }


  // create the LPSR from the MSR (pass 3)
  // ------------------------------------------------------

  S_lpsrScore
    lpScore =
      convertMsrScoreToLpsrScore_Pass3 (
        mScore);

  if (gGeneralOptions->fExit3)
    return;


  // display the LPSR score if requested
  // ------------------------------------------------------

  if (gLpsrOptions->fDisplayLpsr) {
    displayLpsrScore_OptionalPass (
      lpScore,
      gMsrOptions,
      gLpsrOptions);
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

  // initialize the components of MSR that we'll be using
  // ------------------------------------------------------

  initializeMSR ();
  initializeLPSR ();
  
  // create the options handler
  // ------------------------------------------------------

  S_xml2lyOptionsHandler
    optionsHandler =
      xml2lyOptionsHandler::create (
        gOutputIOstream);
    
  // analyze the command line options and arguments
  // ------------------------------------------------------

  vector<string>
    argumentsVector =
      handleOptionsAndArguments (
        optionsHandler,
        argc, argv,
        gLogIOstream);

/* JMI
  // print the resulting options
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fDisplayOptionsHandler) {
    gLogIOstream <<
      optionsHandler <<
      endl <<
      endl;
  }
#endif
*/

  string
    inputSourceName =
      gXml2lyOptions->fInputSourceName;
      
  string
    outputFileName =
      gXml2lyOptions->fOutputFileName;

  int
    outputFileNameSize =
      outputFileName.size ();

  // has quiet mode been requested?
  // ------------------------------------------------------

  if (gGeneralOptions->fQuiet) {
    // disable all trace and display options
    optionsHandler->
      enforceOptionsHandlerQuietness ();
  }

  // welcome message
  // ------------------------------------------------------

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePasses) {
    gLogIOstream <<
      "This is xml2ly " << currentVersionNumber () << 
      " from libmusicxml2 v" << musicxmllibVersionStr () <<
      endl;

    gLogIOstream <<
      "Launching conversion of ";

    if (inputSourceName == "-")
      gLogIOstream <<
        "standard input";
    else
      gLogIOstream <<
        "\"" << inputSourceName << "\"";

    gLogIOstream <<
      " to LilyPond" <<
      endl;

    gLogIOstream <<
      "Time is " << gXml2lyOptions->fTranslationDate <<
      endl;      

    gLogIOstream <<
      "LilyPond code will be written to ";
    if (outputFileNameSize) {
      gLogIOstream <<
        outputFileName;
    }
    else {
      gLogIOstream <<
        "standard output";
    }
    gLogIOstream <<
      endl <<
      endl;
    
    gLogIOstream <<
      "The command line is:" <<
      endl;

    gIndenter++;
    
    gLogIOstream <<
      optionsHandler->
        getCommandLineWithLongOptions () <<
      endl;

    gIndenter--;
    gLogIOstream <<
      "or:" <<
      endl;
    gIndenter++;
    
    gLogIOstream <<
      optionsHandler->
        getCommandLineWithShortOptions () <<
      endl <<
      endl;

    gIndenter--;
  }
#endif

  // print the chosen LilyPond options if so chosen
  // ------------------------------------------------------

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fDisplayOptionsValues) {
    optionsHandler->
      printAllOptionsValues (
        gLogIOstream);

    gLogIOstream <<
      endl;
  }
#endif


  // acknoledge end of command line analysis
  // ------------------------------------------------------

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePasses) {
    gLogIOstream <<
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

  if (gGeneralOptions->fDisplayCPUusage)
    timing::gTiming.print (
      gLogIOstream);

  // check indentation
  if (gIndenter != 0) {
    gLogIOstream <<
      "### gIndenter final value: "<< gIndenter.getIndent () << " ###" <<
      endl <<
      endl;

    // JMI abort ();
  }

  // over!
  // ------------------------------------------------------

  if (! true) { // JMI
    gLogIOstream <<
      "### Conversion from LPSR to LilyPond code failed ###" <<
      endl <<
      endl;
      
    return 1;
  }

  return 0;
}

