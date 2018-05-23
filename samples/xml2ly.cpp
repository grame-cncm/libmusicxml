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

#include "traceOptions.h"
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

#define TRACE_OPTIONS 0

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

  if (TRACE_OPTIONS) {
    // print the options values
    logIndentedOutputStream <<
      "Options values:" <<
      endl;

    gIndenter++;
    
    optionsHandler->
      printAllOptionsValues (
        logIndentedOutputStream);
        
    logIndentedOutputStream <<
      endl;

    gIndenter--;
  }
  
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
      if (gTraceOptions->fTraceBasic)
        gLogIOstream <<
          endl <<
          "Opening file '" << outputFileName << "' for writing" <<
          endl;
          
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
      if (gTraceOptions->fTraceBasic)
        gLogIOstream <<
          endl <<
          "LilyPond code will be written to standard output" <<
          endl;
          
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
      if (gTraceOptions->fTraceBasic)
        gLogIOstream <<
          endl <<
          "Closing file '" << outputFileName << "'" <<
          endl;
          
      outFileStream.close ();
    }
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
    
  // create the LPSR from the MSR (pass 3)
  // ------------------------------------------------------

  S_lpsrScore
    lpScore =
      convertMsrScoreToLpsrScore_Pass3 (
        mScore);

  if (gGeneralOptions->fExit3)
    return;
    
  // generate LilyPond code from the LPSR (pass 4)
  // ------------------------------------------------------

  convertLpsrScoreToLilypondCode_Pass4 (
    outputFileName,
    lpScore);
}

//_______________________________________________________________________________
int main (int argc, char *argv[]) 
{
  /* JMI
  cerr << "argc = " << argc << endl;
  for (int i = 0; i < argc ; i++ ) {
    cerr << "argv[ " << i << "] = " << argv[i] << endl;
  }
  */

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

  // print the resulting options
  if (TRACE_OPTIONS) {
    gLogIOstream <<
      optionsHandler <<
      endl <<
      endl;
  }

  string
    inputSourceName =
      gXml2lyOptions->fInputSourceName;
      
  string
    outputFileName =
      gXml2lyOptions->fOutputFileName;

  int
    outputFileNameSize =
      outputFileName.size ();

  // welcome message
  // ------------------------------------------------------

  if (gTraceOptions->fTraceBasic) {
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

  // print the chosen LilyPond options if so chosen
  // ------------------------------------------------------

  if (gTraceOptions->fDisplayOptionsValues) {
    optionsHandler->
      printAllOptionsValues (
        gLogIOstream);

    gLogIOstream <<
      endl;
  }

  // acknoledge end of command line analysis
  // ------------------------------------------------------

  if (gTraceOptions->fTraceBasic) {
    gLogIOstream <<
      "The command line options and arguments have been analyzed" <<
      endl;
  }

  
  // do the translation
  // ------------------------------------------------------

  convertMusicXMLToLilypond (
    inputSourceName,
    outputFileName);

  // print timing information
  // ------------------------------------------------------

  if (gGeneralOptions->fDisplayCPUusage)
    timing::gTiming.print (
      gLogIOstream);

  // check indentation
  if (gIndenter != 0) {
    gLogIOstream <<
      "### gIndenter final value: "<< gIndenter << " ###" <<
      endl <<
      endl;

    abort ();
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

