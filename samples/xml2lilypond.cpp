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

#include "musicXMLOptions.h"
#include "lilypondOptions.h"

#include "xml2lilypondOptionsHandling.h"

#include "musicXMLTree2MsrInterface.h"
#include "msr2LpsrInterface.h"
#include "lpsr2LilypondInterface.h"


using namespace std;

using namespace MusicXML2;

#define TRACE_OPTIONS 0

//_______________________________________________________________________________
vector<string> handleOptionsAndArguments (
S_xml2lilypondOptionsHandler optionsHandler,
  int                        argc,
  char*                      argv [],
  indentedOutputStream&      logIndentedOutputStream)
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

    gIdtr++;
    
    optionsHandler->
      printOptionsValues (
        logIndentedOutputStream);
        
    logIndentedOutputStream <<
      endl;

    gIdtr--;
  }
  
  return argumentsVector;
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

  // create an indented output stream for the log
  indentedOutputStream
    logIndentedOutputStream (
      cerr, gIdtr);
    
  // initialize the components of MSR that we'll be using
  // ------------------------------------------------------

  initializeMSR ();
  initializeLPSR ();
  
  // create the options handler
  // ------------------------------------------------------

  S_xml2lilypondOptionsHandler
    optionsHandler =
      xml2lilypondOptionsHandler::create (
        logIndentedOutputStream);

  // analyze the command line options and arguments
  // ------------------------------------------------------

  vector<string>
    argumentsVector =
      handleOptionsAndArguments (
        optionsHandler,
        argc, argv,
        logIndentedOutputStream);

  // print the resulting options
  if (TRACE_OPTIONS) {
    logIndentedOutputStream <<
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
    logIndentedOutputStream <<
      "This is xml2Lilypond v" << currentVersionNumber () << 
      " from libmusicxml2 v" << musicxmllibVersionStr () <<
      endl;

    logIndentedOutputStream <<
      "Launching conversion of ";

    if (inputSourceName == "-")
      logIndentedOutputStream <<
        "standard input";
    else
      logIndentedOutputStream <<
        "\"" << inputSourceName << "\"";

    logIndentedOutputStream <<
      " to LilyPond" <<
      endl;

    logIndentedOutputStream <<
      "Time is " << gGeneralOptions->fTranslationDate <<
      endl;      

    logIndentedOutputStream <<
      "LilyPond code will be written to ";
    if (outputFileNameSize) {
      logIndentedOutputStream <<
        outputFileName;
    }
    else {
      logIndentedOutputStream <<
        "standard output";
    }
    logIndentedOutputStream <<
      endl <<
      endl;
    
    logIndentedOutputStream <<
      "The command line is:" <<
      endl;

    gIdtr++;
    
    logIndentedOutputStream <<
      optionsHandler->
        getCommandLineWithLongOptions () <<
      endl;

    gIdtr--;
    logIndentedOutputStream <<
    "or:" <<
    endl;
    gIdtr++;
    
    logIndentedOutputStream <<
      optionsHandler->
        getCommandLineWithShortOptions () <<
      endl <<
      endl;

    gIdtr--;
  }

  // print the chosen LilyPond options
  // ------------------------------------------------------

  if (gGeneralOptions->fTraceGeneral) {
    optionsHandler->
      printOptionsValues (
        logIndentedOutputStream);
  }

  // acknoledge end of command line analysis
  // ------------------------------------------------------

  if (gGeneralOptions->fTraceGeneral) {
    logIndentedOutputStream <<
      endl <<
      "The command line options and arguments have been analyzed" <<
      endl;
  }

  // open output file if need be
  // ------------------------------------------------------

  ofstream outStream;

  if (outputFileNameSize) {
    if (gGeneralOptions->fTraceGeneral)
      logIndentedOutputStream <<
        "Opening file '" << outputFileName << "' for writing" <<
        endl;
        
    outStream.open (
      outputFileName.c_str(),
      ofstream::out);
  }
      
  // create MSR from MusicXML contents
  // ------------------------------------------------------

  S_msrScore mScore;

  if (inputSourceName == "-") {
    // input comes from standard input
    if (outputFileNameSize)
      mScore =
        musicxmlFd2Msr (
          stdin,
          gMsrOptions,
          outStream);
    else
      mScore =
        musicxmlFd2Msr (
          stdin, gMsrOptions, cout);
  }
  
  else {
    // input comes from a file
    if (outputFileNameSize) {
      mScore =
        musicxmlFile2Msr (
          inputSourceName.c_str(),
          gMsrOptions,
          outStream);
    }
    else {
      mScore =
        musicxmlFile2Msr (
          inputSourceName.c_str(),
          gMsrOptions,
          cout);
    }
  }
    
  if (! mScore) {
    logIndentedOutputStream <<
      "### Conversion from MusicCML to MSR failed ###" <<
      endl <<
      endl;
    return 1;
  }

  // create LPSR from MSR
  // ------------------------------------------------------

  S_lpsrScore lpScore;
        
  if (! gLilypondOptions->fNoLilypondCode) {
    if (outputFileNameSize) {
      lpScore =
        msr2Lpsr (
          mScore,
          gMsrOptions,
          gLpsrOptions,
          outStream);
    }
    else {
      lpScore =
        msr2Lpsr (
          mScore,
          gMsrOptions,
          gLpsrOptions,
          cout);
    }
    
    if (! lpScore) {
      logIndentedOutputStream <<
        "### Conversion from MSR to LPSR failed ###" <<
        endl <<
        endl;
      return 1;
    }
  }

  // generate LilyPond code from LPSR
  // ------------------------------------------------------

  if (! gLilypondOptions->fNoLilypondCode) {
    if (outputFileNameSize)
      lpsr2Lilypond (
        lpScore,
        gMsrOptions,
        gLpsrOptions,
        outStream);
    else
      lpsr2Lilypond (
        lpScore,
        gMsrOptions,
        gLpsrOptions,
        cout);
    
    if (outputFileNameSize) {
      if (gGeneralOptions->fTraceGeneral)
        logIndentedOutputStream <<
          endl <<
          "Closing file '" << outputFileName << "'" <<
          endl;
          
      outStream.close ();
    }
  }

  // print timing information
  // ------------------------------------------------------

  if (gGeneralOptions->fDisplayCPUusage)
    timing::gTiming.print (
      logIndentedOutputStream);
  

  // over!
  // ------------------------------------------------------

  if (! true) { // JMI
    logIndentedOutputStream <<
      "### Conversion from LPSR to LilyPond code failed ###" <<
      endl <<
      endl;
    return 1;
  }

  return 0;
}

