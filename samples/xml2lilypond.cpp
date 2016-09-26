/*

  Copyright (C) 2003-2008  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the MusicXML Library use.
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "musicxml2lpsr.h"

using namespace std;
using namespace MusicXML2;

void usage(int exitStatus) {
  cerr <<
    endl <<
    "--> Usage:    musicxml2lpsr [options] <MusicXMLFile>" << endl <<
    endl <<
    "    Effect:   reads the contents of <MusicXMLFile>, or stdin if <MusicXMLFile> is '-'," << endl <<
    "              and converts it to LilyPond source code written to standard output" << endl <<
    endl <<
    "    Options: --help:          display this help" << endl <<
    "             --absolute:      generate absolute code (default: relative code)" << endl <<
    "             --numericaltime  don't generate 'C' or such numerical time signatures" << endl <<
    "             --nocomments:    don't generate comments" << endl <<
    "             --noautobars:    don't generate barlines" << endl <<
    "             --nostems:       don't generate stems commands" << endl <<
    "             --nopositions:   don't generate positions" << endl <<
    "             --notrace:       don't generate a trace of the activity to standard error" << endl <<
    "             --debug  :       print debugging information" << endl <<
    endl;
  exit(exitStatus);
}

//_______________________________________________________________________________
int main(int argc, char *argv[]) 
{
 /*
  cout << "argc = " << argc << endl;
  for (int i = 0; i < argc ; i++ ) {
    cout << "argv[ " << i << "] = " << argv[i] << endl;
  }
  */
  
  int helpPresent =          0;
  int absolutePresent =      0;
  int numericaltimePresent = 0;
  int nocommentsPresent =    0;
  int noautobarsPresent =    0;
  int stemsPresent =         0;
  int positionsPresent =     0;
  int notracePresent =       0;
  int debugPresent =         0;
  
  std::string selectedOptions = "";
  
  bool generateAbsoluteCode =  false;
  bool generateNumericalTime = false;
  bool generateComments =      true;
  bool generateBars =          true;
  bool generateStems =         false;
  bool generatePositions =     false;
  bool trace =                 true;
  bool debug =                 false;
  
   static struct option long_options [] =
    {
    /* These options set a flag. */
    {"help",          no_argument,   &helpPresent, 1},
    {"absolute",      no_argument,   &absolutePresent, 1},
    {"numericaltime", no_argument,   &numericaltimePresent, 1},
    {"nocomments",    no_argument,   &nocommentsPresent, 1},
    {"noautobars",    no_argument,   &noautobarsPresent, 1},
    {"stems",         no_argument,   &stemsPresent, 1},
    {"positions",     no_argument,   &positionsPresent, 1},
    {"notrace",       no_argument,   &notracePresent, 1},
    {"debug",         no_argument,   &debugPresent, 1},
    {0, 0, 0, 0}
    };

  /* getopt_long stores the option index here. */
  int option_index = 0;

  int c;
  while (
    (c = getopt_long (
      argc, argv,
      "hab",
      long_options, & option_index ))
      !=
    -1
    )
    {
    switch (c)
      {
      case 0 :
        if (helpPresent) {
          usage (0);
          break;
        }
        if (absolutePresent) {
          generateAbsoluteCode = true;
          selectedOptions += "--absolute ";
          break;
        }
        if (numericaltimePresent) {
          generateNumericalTime = true;
          selectedOptions += "--numericaltime ";
          break;
        }
        if (nocommentsPresent) {
          generateComments = false;
          selectedOptions += "--nocomments ";
          break;
        }
        if (noautobarsPresent) {
          generateBars = false;
          selectedOptions += "--noautobars ";
          break;
        }
        if (stemsPresent) {
          generateStems = true;
          selectedOptions += "--stems ";
          break;
        }
        if (positionsPresent) {
          generatePositions = true;
          selectedOptions += "--positions ";
          break;
        }
        if (notracePresent) {
          trace = false;
          selectedOptions += "--notrace ";
          break;
        }
        if (debugPresent) {
          debug = true;
          selectedOptions += "--debug ";
          break;
        }
        break;
        
       default:
        usage (1);
        break;
      } // switch
    } // while
 
  int nonOptionArgs = argc-optind;

  char * file = "";

  switch (nonOptionArgs)
    {
    case 1 :
      file = argv [optind];
      break;

    default:
      usage (1);
      break;
    } //  switch

  // int   remainingArgs = nonOptionArgs;

  translationSwitches  ts;
  ts.fGenerateAbsoluteCode =  generateAbsoluteCode;
  ts.fGenerateNumericalTime = generateNumericalTime;
  ts.fGenerateComments =      generateComments;
  ts.fGenerateBars =          generateBars;
  ts.fGenerateStems =         generateStems;
  ts.fGeneratePositions =     generatePositions;
  ts.fTrace =                 trace;
  ts.fDebug =                 debug;
  ts.fSelectedOptions =       selectedOptions;

  if (ts.fTrace)
    cerr << 
      "Launching conversion to LilyPond with libmusicxml2 v" << 
      musicxmllibVersionStr() <<
      " / xml2Lpsr v" <<
      musicxml2LpsrVersionStr() << 
      endl <<
      "The options are:" << endl <<
      "  generateAbsoluteCode:  " << string(generateAbsoluteCode ? "true" : "false") << endl <<
      "  generateNumericalTime: " << string(generateNumericalTime ? "true" : "false") << endl <<
      "  generateComments:      " << string(generateComments ? "true" : "false") << endl <<
      "  generateBars:          " << string(generateBars ? "true" : "false") << endl <<
      "  generateStems:         " << string(generateStems ? "true" : "false") << endl <<
      "  generatePositions:     " << string(generatePositions ? "true" : "false") << endl <<
      "  trace:                 " << string(trace ? "true" : "false") << endl <<
      "  debug:                 " << string(debug ? "true" : "false") << endl;
  
  xmlErr err = kNoErr;
  if (!strcmp(file, "-"))
    err = musicxmlFd2Lpsr (stdin, ts, cout);
  else
    err = musicxmlFile2Lpsr (file, ts, cout);
  if (err) {
    cout << "conversion failed" << endl;
  }
  return 0;
}
