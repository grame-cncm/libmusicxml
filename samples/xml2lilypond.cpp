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

#include "generalOptions.h"
#include "mxmlOptions.h"
#include "msrOptions.h"
#include "lpsrOptions.h"
#include "lilypondOptions.h"

#include "xml2lilypondOptionsHandling.h"

#include "mxmlTree2MsrSkeletonBuilderInterface.h"
#include "mxmlTree2MsrTranslatorInterface.h"

#include "msr2LpsrInterface.h"

#include "lpsr2LilypondInterface.h"


using namespace std;

using namespace MusicXML2;

#define TRACE_OPTIONS 0

//_______________________________________________________________________________
vector<string> handleOptionsAndArguments (
  S_xml2lilypondOptionsHandler optionsHandler,
  int                          argc,
  char*                        argv [],
  indentedOstream&             logIndentedOutputStream)
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
      printOptionsValues (
        logIndentedOutputStream);
        
    logIndentedOutputStream <<
      endl;

    gIndenter--;
  }
  
  return argumentsVector;
}

S_msrScore runPass2a (
  string inputSourceName,
  string outputFileName)
{
  int outputFileNameSize = outputFileName.size ();
  
  S_msrScore mScore;

  if (inputSourceName == "-") {
    // input comes from standard input
    if (outputFileNameSize)
      mScore =
        mxmlFd2MsrSkeleton (
          stdin,
          gMsrOptions,
          gLogIOstream);
    else
      mScore =
        mxmlFd2MsrSkeleton (
          stdin,
          gMsrOptions,
          gLogIOstream);
  }
  
  else {
    // input comes from a file
    if (outputFileNameSize) {
      mScore =
        mxmlFile2MsrSkeleton (
          inputSourceName.c_str(),
          gMsrOptions,
          gLogIOstream);
    }
    else {
      mScore =
        mxmlFile2MsrSkeleton (
          inputSourceName.c_str(),
          gMsrOptions,
          gLogIOstream);
    }
  }
    
  return mScore;
}

void runPass2b (
  string     inputSourceName,
  string     outputFileName,
  S_msrScore mScore)
{
  int outputFileNameSize = outputFileName.size ();

  if (inputSourceName == "-") {
    // input comes from standard input
    if (outputFileNameSize)
      mScore =
        mxmlFd2Msr (
          stdin,
          gMsrOptions,
          gLogIOstream);
    else
      mScore =
        mxmlFd2Msr (
          stdin,
          gMsrOptions,
          gLogIOstream);
  }
  
  else {
    // input comes from a file
    if (outputFileNameSize) {
      mScore =
        mxmlFile2Msr (
          inputSourceName.c_str(),
          gMsrOptions,
          gLogIOstream);
    }
    else {
      mScore =
        mxmlFile2Msr (
          inputSourceName.c_str(),
          gMsrOptions,
          gLogIOstream);
    }
  }

    /* JMI
  if (! mScore) {
    gLogIOstream <<
      "### Conversion from MusicCML to MSR failed ###" <<
      endl <<
      endl;
// JMI    return false;
  }
*/

// JMI  return true;
}

S_lpsrScore runPass3 (
  string     outputFileName,
  S_msrScore mScore)
{
  int outputFileNameSize = outputFileName.size ();

  S_lpsrScore lpScore;

  if (! gLilypondOptions->fNoLilypondCode) {
    if (outputFileNameSize) {
      lpScore =
        msr2Lpsr (
          mScore,
          gMsrOptions,
          gLpsrOptions,
          gLogIOstream);
    }
    else {
      lpScore =
        msr2Lpsr (
          mScore,
          gMsrOptions,
          gLpsrOptions,
          gLogIOstream);
    }
  }

  return lpScore;
}

void runPass4 (
  string     outputFileName,
  S_msrScore mScore)
{
  int outputFileNameSize = outputFileName.size ();

  if (! gLilypondOptions->fNoLilypondCode) {
    // open output file if need be
    // ------------------------------------------------------
  
    ofstream outFileStream;
        
    if (outputFileNameSize) {
      if (gGeneralOptions->fTraceGeneral)
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
          outFileStream, gIndenter);

      // convert the LPSR score to LilyPond code
      lpsr2Lilypond (
        lpScore,
        gMsrOptions,
        gLpsrOptions,
        gLogIOstream,
        lilypondCodeFileOutputStream);
    }
    
    else {
      // create an indented output stream for the LilyPond code
      // to be written to cout
      indentedOstream
        lilypondCodeCoutOutputStream (
          cout, gIndenter);

      lpsr2Lilypond (
        lpScore,
        gMsrOptions,
        gLpsrOptions,
        gLogIOstream,
        lilypondCodeCoutOutputStream);
    }
    
    if (outputFileNameSize) {
      if (gGeneralOptions->fTraceGeneral)
        gLogIOstream <<
          endl <<
          "Closing file '" << outputFileName << "'" <<
          endl;
          
      outFileStream.close ();
    }
  }
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

  S_xml2lilypondOptionsHandler
    optionsHandler =
      xml2lilypondOptionsHandler::create (
        gLogIOstream);
    
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
      gGeneralOptions->fInputSourceName;
      
  string
    outputFileName =
      gGeneralOptions->fOutputFileName;

  int
    outputFileNameSize =
      outputFileName.size ();

  // welcome message
  // ------------------------------------------------------

  if (gGeneralOptions->fTraceGeneral) {
    gLogIOstream <<
      "This is xml2Lilypond v" << currentVersionNumber () << 
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
      "Time is " << gGeneralOptions->fTranslationDate <<
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

  // print the chosen LilyPond options
  // ------------------------------------------------------

  if (gGeneralOptions->fTraceGeneral) {
    optionsHandler->
      printOptionsValues (
        gLogIOstream);
  }

  // acknoledge end of command line analysis
  // ------------------------------------------------------

  if (gGeneralOptions->fTraceGeneral) {
    gLogIOstream <<
      endl <<
      "The command line options and arguments have been analyzed" <<
      endl;
  }

  // create the MSR skeleton from MusicXML contents (pass 2a)
  // ------------------------------------------------------

  S_msrScore
    mScore =
      runPass2a (
        inputSourceName, outputFileName);

  if (! mScore) {
    gLogIOstream <<
      "### Conversion from MusicCML to an MSR skeleton failed ###" <<
      endl <<
      endl;

    exit (1);
  }

  else if (! gGeneralOptions->fExit2a) {
    
    // create the MSR from MusicXML contents (pass 2b)
    // ------------------------------------------------------

    runPass2b (
      inputSourceName, outputFileName,
      mScore);
    
    if (! gGeneralOptions->fExit2b) {
      
      // create the LPSR from the MSR (pass 3)
      // ------------------------------------------------------

      S_lpsrScore
        lpScore =
          runPass3 (
            outputFileName,
            mScore);

      if (! lpScore) {
        gLogIOstream <<
          "### Conversion from MSR to LPSR failed ###" <<
          endl <<
          endl;
        return 1;
      }

      else if (! gGeneralOptions->fExit3) {
        
        // generate LilyPond code from the LPSR (pass 4)
        // ------------------------------------------------------
    
        runPass4 (
          outputFileName,
          lpScore);
      }
    }
  }

  // print timing information
  // ------------------------------------------------------

  if (gGeneralOptions->fDisplayCPUusage)
    timing::gTiming.print (
      gLogIOstream);
  

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

