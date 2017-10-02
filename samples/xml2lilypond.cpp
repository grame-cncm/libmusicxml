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


// useful shortcut macros
#define idtr indenter::gIndenter
#define tab  indenter::gIndenter.getSpacer ()

#define TRACE_OPTIONS 0

//_______________________________________________________________________________
vector<string> analyzeOptionsAndArguments (
S_xml2lilypondOptionsHandler
                 optionsHandler,
  int            argc,
  char*          argv [])
{
  // analyse the options
  vector<string>
    argumentsVector =
      optionsHandler->
        analyzeOptions (argc, argv);

  if (TRACE_OPTIONS) {
    // print the options values
    cerr <<
      "Options values:" <<
      endl;

    idtr++;
    
    optionsHandler->
      printOptionsValues (cerr);
        
    cerr <<
      endl;

    idtr--;
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

  // initialize the components of MSR that we'll use
  initializeMSR ();
  initializeLPSR ();
  
  // create the options handler
  // ------------------------------------------------------

  S_xml2lilypondOptionsHandler
    optionsHandler =
      xml2lilypondOptionsHandler::create ();

  // analyze the command line options and arguments
  // ------------------------------------------------------

  vector<string>
    argumentsVector =
      analyzeOptionsAndArguments (
        optionsHandler,
        argc, argv);

  // print the resulting options
  if (TRACE_OPTIONS) {
    cerr <<
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
    cerr <<  idtr <<
      "This is xml2Lilypond v" << currentVersionNumber () << 
      " from libmusicxml2 v" << musicxmllibVersionStr () <<
      endl;

    cerr << idtr <<
      "Launching conversion of ";

    if (inputSourceName == "-")
      cerr << "standard input";
    else
      cerr << "\"" << inputSourceName << "\"";

    cerr <<
      " to LilyPond" <<
      endl;

    cerr << idtr <<
      "Time is " << gGeneralOptions->fTranslationDate <<
      endl;      

    cerr << idtr <<
      "LilyPond code will be written to ";
    if (outputFileNameSize) {
      cerr <<
        outputFileName;
    }
    else {
      cerr << "standard output";
    }
    cerr <<
      endl <<
      endl;
    
    cerr << idtr <<
      "The command line is:" <<
      endl;

    idtr++;
    
    cerr <<
      idtr <<
        optionsHandler->
          getCommandLineWithLongOptions () <<
      endl;

    idtr--;
    cerr <<
      idtr <<
        "or:" <<
        endl;
    idtr++;
    
    cerr <<
      idtr <<
        optionsHandler->
          getCommandLineWithShortOptions () <<
        endl <<
      endl;

    idtr--;
  }

  // print the chosen LilyPond options
  // ------------------------------------------------------

  optionsHandler->
    printOptionsValues (cerr);

  // acknoledge end of command line analysis
  // ------------------------------------------------------

  if (gGeneralOptions->fTraceGeneral)
    cerr <<
      endl <<
      idtr <<
        "The command line options and arguments have been analyzed" <<
        endl;

  // open output file if need be
  // ------------------------------------------------------

  ofstream outStream;

  if (outputFileNameSize) {
    if (gGeneralOptions->fTraceGeneral)
      cerr << idtr <<
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
          stdin, gMsrOptions, outStream);
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
          inputSourceName.c_str(), gMsrOptions, outStream);
    }
    else {
      mScore =
        musicxmlFile2Msr (
          inputSourceName.c_str(), gMsrOptions, cout);
    }
  }
    
  if (! mScore) {
    cerr <<
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
          mScore, gMsrOptions, gLpsrOptions, outStream);
    }
    else {
      lpScore =
        msr2Lpsr (
          mScore, gMsrOptions, gLpsrOptions, cout);
    }
    
    if (! lpScore) {
      cerr <<
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
        lpScore, gMsrOptions, gLpsrOptions, outStream);
    else
      lpsr2Lilypond (
        lpScore, gMsrOptions, gLpsrOptions, cout);
    
    if (outputFileNameSize) {
      if (gGeneralOptions->fTraceGeneral)
        cerr <<
          endl <<
          idtr <<
            "Closing file '" << outputFileName << "'" <<
            endl;
          
      outStream.close ();
    }
  }

  // print timing information
  // ------------------------------------------------------

  if (gGeneralOptions->fDisplayCPUusage)
    timing::gTiming.print (cerr);
  

  // over!
  // ------------------------------------------------------

  if (! true) { // JMI
    cerr <<
      "### Conversion from LPSR to LilyPond code failed ###" <<
      endl <<
      endl;
    return 1;
  }

  return 0;
}

