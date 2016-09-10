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

#include "musicxml2lilypond.h"

using namespace std;
using namespace MusicXML2;

void usage(int exitStatus) {
  cerr <<
    endl <<
    "--> Usage:    musicxml2lilypond [options] <MusicXMLFile>" << endl <<
    "    Action:   reads <MusicXMLFile> or stdin if <MusicXMLFile> is '-'" << endl <<
    endl <<
    "    Options: -a,--absolute:    generates absolute code" << endl <<
    "                   (default:   generate relative code)" << endl <<
    "             -c,--nocomments:  don't generate comments" << endl <<
    "             -b,--noautobars:  don't generate barlines" << endl <<
    "             -s,--nostems:     don't generate stems commands" << endl <<
    "             -p,--nopositions: don't generate positions" << endl <<
    "             -t,--notrace:     don't generate a trace of the activity" << endl <<
    "             -d,--debug  :     print debugging information" << endl <<
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
  
  bool generateAbsoluteCode = false;
  bool generateComments =     true;
  bool generateBars =         true;
  bool generateStems =        false;
  bool generatePositions =    false;
  bool trace =                true;
  bool debug =                false;
  
   static struct option long_options [] =
    {
    /* These options set a flag. */
    {"help",      no_argument,       0, 'h'},
    {"absolute",  no_argument,       0, 'a'},
    {"comments",  no_argument,       0, 'c'},
    {"autobars",  no_argument,       0, 'b'},
    {"stems",     no_argument,       0, 's'},
    {"positions", no_argument,       0, 'p'},
    {"trace",     no_argument,       0, 't'},
    {"debug",     no_argument,       0, 'd'},
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
      case 'h' :
        usage (0);
        break;
      case 'a' :
        generateAbsoluteCode = true;
        break;
      case 'b' :
        generateBars = false;
        break;
      case 's' :
        generateStems = true;
        break;
      case 'p' :
        generatePositions = true;
        break;
      case 't' :
        trace = false;
        break;
      case 'd' :
        debug = true;
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
      std::cerr <<
        "--> nonOptionArgs = " << nonOptionArgs << std::endl;
      usage (1);
    } //  switch

  int   remainingArgs = nonOptionArgs;

  translationSwitches sw;
  sw.fGenerateAbsoluteCode = generateAbsoluteCode;
  sw.fGenerateComments =     generateComments;
  sw.fGenerateBars =         generateBars;
  sw.fGenerateStems =        generateStems;
  sw.fGeneratePositions =    generatePositions;
  sw.fTrace =                trace;
  sw.fDebug =                debug;

  if (sw.fTrace)
    cerr << 
      "Launching conversion to LilyPond with libmusicxml2 v" << 
      musicxmllibVersionStr() <<
      " / xml2lilypond v" <<
      musicxml2lilypondVersionStr() << 
      endl;
  
  xmlErr err = kNoErr;
  if (!strcmp(file, "-"))
    err = musicxmlfd2lilypond(stdin, sw, cout);
  else
    err = musicxmlfile2lilypond(file, sw, cout);
  if (err) {
    cout << "conversion failed" << endl;
  }
  return 0;
}
