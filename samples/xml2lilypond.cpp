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
        gLogIos);

  // analyze the command line options and arguments
  // ------------------------------------------------------

  vector<string>
    argumentsVector =
      handleOptionsAndArguments (
        optionsHandler,
        argc, argv,
        gLogIos);

  // print the resulting options
  if (TRACE_OPTIONS) {
    gLogIos <<
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
    gLogIos <<
      "This is xml2Lilypond v" << currentVersionNumber () << 
      " from libmusicxml2 v" << musicxmllibVersionStr () <<
      endl;

    gLogIos <<
      "Launching conversion of ";

    if (inputSourceName == "-")
      gLogIos <<
        "standard input";
    else
      gLogIos <<
        "\"" << inputSourceName << "\"";

    gLogIos <<
      " to LilyPond" <<
      endl;

    gLogIos <<
      "Time is " << gGeneralOptions->fTranslationDate <<
      endl;      

    gLogIos <<
      "LilyPond code will be written to ";
    if (outputFileNameSize) {
      gLogIos <<
        outputFileName;
    }
    else {
      gLogIos <<
        "standard output";
    }
    gLogIos <<
      endl <<
      endl;
    
    gLogIos <<
      "The command line is:" <<
      endl;

    gIndenter++;
    
    gLogIos <<
      optionsHandler->
        getCommandLineWithLongOptions () <<
      endl;

    gIndenter--;
    gLogIos <<
      "or:" <<
      endl;
    gIndenter++;
    
    gLogIos <<
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
        gLogIos);
  }

  // acknoledge end of command line analysis
  // ------------------------------------------------------

  if (gGeneralOptions->fTraceGeneral) {
    gLogIos <<
      endl <<
      "The command line options and arguments have been analyzed" <<
      endl;
  }

  // open output file if need be
  // ------------------------------------------------------

  ofstream outFileStream;

  if (outputFileNameSize) {
    if (gGeneralOptions->fTraceGeneral)
      gLogIos <<
        "Opening file '" << outputFileName << "' for writing" <<
        endl;
        
    outFileStream.open (
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
          outFileStream);
    else
      mScore =
        musicxmlFd2Msr (
          stdin,
          gMsrOptions,
          cout);
  }
  
  else {
    // input comes from a file
    if (outputFileNameSize) {
      mScore =
        musicxmlFile2Msr (
          inputSourceName.c_str(),
          gMsrOptions,
          gLogIos);
    }
    else {
      mScore =
        musicxmlFile2Msr (
          inputSourceName.c_str(),
          gMsrOptions,
          gLogIos);
    }
  }
    
  if (! mScore) {
    gLogIos <<
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
          gLogIos);
    }
    else {
      lpScore =
        msr2Lpsr (
          mScore,
          gMsrOptions,
          gLpsrOptions,
          gLogIos);
    }
    
    if (! lpScore) {
      gLogIos <<
        "### Conversion from MSR to LPSR failed ###" <<
        endl <<
        endl;
      return 1;
    }
  }

  // generate LilyPond code from LPSR
  // ------------------------------------------------------

  if (! gLilypondOptions->fNoLilypondCode) {
    if (outputFileNameSize) {
      // create an indented output stream for the LilyPond code
      // to be written to outFileStream
      indentedOutputStream
        lilypondCodeFileOutputStream (
          outFileStream, gIndenter);

      // convert the LPSR score to LilyPond code
      lpsr2Lilypond (
        lpScore,
        gMsrOptions,
        gLpsrOptions,
        lilypondCodeFileOutputStream);
    }
    
    else {
      // create an indented output stream for the LilyPond code
      // to be written to cout
      indentedOutputStream
        lilypondCodeCoutOutputStream (
          cout, gIndenter);

      lpsr2Lilypond (
        lpScore,
        gMsrOptions,
        gLpsrOptions,
        lilypondCodeCoutOutputStream);
    }
    
    if (outputFileNameSize) {
      if (gGeneralOptions->fTraceGeneral)
        gLogIos <<
          endl <<
          "Closing file '" << outputFileName << "'" <<
          endl;
          
      outFileStream.close ();
    }
  }

  // print timing information
  // ------------------------------------------------------

  if (gGeneralOptions->fDisplayCPUusage)
    timing::gTiming.print (
      gLogIos);
  

  // over!
  // ------------------------------------------------------

  if (! true) { // JMI
    gLogIos <<
      "### Conversion from LPSR to LilyPond code failed ###" <<
      endl <<
      endl;
    return 1;
  }

  return 0;
}

