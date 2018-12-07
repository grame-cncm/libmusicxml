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

#include "musicXML2MxmlTreeInterface.h"

#include "mxmlTree2MsrSkeletonBuilderInterface.h"
#include "mxmlTree2MsrTranslatorInterface.h"

#include "msr2BsrInterface.h"
#include "bsr2BsrFinalizerInterface.h"

#include "bsr2BrailleTranslatorInterface.h"

#include "generalOptions.h"
#include "musicXMLOptions.h"
#include "msrOptions.h"
#include "bsrOptions.h"
#include "brailleOptions.h"

#include "xml2brlOptionsHandling.h"


using namespace std;

using namespace MusicXML2;


//_______________________________________________________________________________
vector<string> handleOptionsAndArguments (
  S_xml2brlOptionsHandler optionsHandler,
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
        gGeneralOptions->fInputSourceName,
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
        gGeneralOptions->fInputSourceName,
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
        gGeneralOptions->fInputSourceName,
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
    gLogIOstream);

  if (gIndenter != 0) {
    if (! gGeneralOptions->fQuiet) {
      stringstream s;
      
      s <<
        "gIndenter value after first BSR score display: "<<
        gIndenter.getIndent ();
        
      msrMusicXMLWarning (
        gGeneralOptions->fInputSourceName,
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
    gLogIOstream);

  if (gIndenter != 0) {
    if (! gGeneralOptions->fQuiet) {
      stringstream s;
      
      s <<
        "gIndenter value after finalized BSR score display: "<<
        gIndenter.getIndent ();
        
      msrMusicXMLWarning (
        gGeneralOptions->fInputSourceName,
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
        gLogIOstream);
  }

  if (gIndenter != 0) {
    if (! gGeneralOptions->fQuiet) {
      stringstream s;
      
      s <<
        "gIndenter value after pass 3a: "<<
        gIndenter.getIndent ();
        
      msrMusicXMLWarning (
        gGeneralOptions->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }

  if (! bScore) {
    gLogIOstream <<
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
    gLogIOstream);

  if (gIndenter != 0) {
    if (! gGeneralOptions->fQuiet) {
      stringstream s;
      
      s <<
        "gIndenter value after BSR score display: "<<
        gIndenter.getIndent ();
        
      msrMusicXMLWarning (
        gGeneralOptions->fInputSourceName,
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
        gLogIOstream);
  }

  if (gIndenter != 0) {
    if (! gGeneralOptions->fQuiet) {
      stringstream s;
      
      s <<
        "gIndenter value after pass 3b: "<<
        gIndenter.getIndent ();
        
      msrMusicXMLWarning (
        gGeneralOptions->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }

  if (! finalizedBsrScore) {
    gLogIOstream <<
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
      if (gGeneralOptions->fTracePasses) {
        gLogIOstream <<
          "Opening file '" << outputFileName << "' for writing" <<
          endl;
      }
#endif
          
      brailleCodeFileOutputStream.open (
        outputFileName.c_str(),
        ofstream::out);
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
        gLogIOstream,
        brailleCodeFileOutputStream);
    }
    
    else {
#ifdef TRACE_OPTIONS
      if (gGeneralOptions->fTracePasses) {
        gLogIOstream <<
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
        gLogIOstream,
        brailleCodeCoutOutputStream);
    }

    if (outputFileNameSize) {
#ifdef TRACE_OPTIONS
      if (gGeneralOptions->fTracePasses) {
        gLogIOstream <<
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
        gGeneralOptions->fInputSourceName,
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

  if (gMsrOptions->fExit2a)
    return;

    
  // populate the MSR from MusicXML contents (pass 2b)
  // ------------------------------------------------------

  populateScoreSkeletonFromMusicXML_Pass2b (
    mxmlTree,
    mScore);
  
  if (gMsrOptions->fExit2b)
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


  // create the BSR from the MSR (pass 3a)
  // ------------------------------------------------------

  S_bsrScore
    firstBsrScore =
      convertMsrScoreToBsrScore_Pass3a (
        mScore);

  if (gBsrOptions->fExit3a)
    return;


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

  if (gBsrOptions->fExit3b)
    return;


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

  // initialize the components of MSR that we'll be using
  // ------------------------------------------------------

  initializeMSR ();
  initializeBSR ();
  
  // create the options handler
  // ------------------------------------------------------

  S_xml2brlOptionsHandler
    optionsHandler =
      xml2brlOptionsHandler::create (
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
  if (gGeneralOptions->fDisplayOptionsHandler) {
    gLogIOstream <<
      optionsHandler <<
      endl <<
      endl;
  }
#endif
*/

  string
    inputSourceName =
      gGeneralOptions->fInputSourceName;
      
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
    optionsHandler->
      enforceOptionsHandlerQuietness ();
  }

  // welcome message
  // ------------------------------------------------------

#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTracePasses) {
    gLogIOstream <<
      "This is xml2brl " << currentVersionNumber () << 
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
      " to Braille music" <<
      endl;

    gLogIOstream <<
      "Time is " << gGeneralOptions->fTranslationDate <<
      endl;      

    gLogIOstream <<
      "Braille music will be written to ";
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

  // print the chosen options if so chosen
  // ------------------------------------------------------

#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fDisplayOptionsValues) {
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
  if (gGeneralOptions->fTracePasses) {
    gLogIOstream <<
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
      "### Conversion from BSR to Braille music failed ###" <<
      endl <<
      endl;
      
    return 1;
  }

  return 0;
}

