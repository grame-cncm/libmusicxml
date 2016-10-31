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

#include "musicxml2guido.h"

using namespace std;
using namespace MusicXML2;

void usage(int exitStatus) {
  cerr <<
    endl <<
    "--> Usage:    musicxml2guido [options] <MusicXMLFile>" << endl <<
    "    Action:   reads <MusicXMLFile> or stdin if <MusicXMLFile> is '-'" << endl <<
    endl <<
    "             -b,--autobars: don't generates barlines" << endl <<
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
  
  bool generateBars = true;

  static struct option long_options [] =
    {
    /* These options set a flag. */
    {"help",     no_argument,       0, 'h'},
    {"autobars", no_argument,       0, 'b'},
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
      case 'b' :
        generateBars = false;
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

  // int   remainingArgs = nonOptionArgs;

  xmlErr err = kNoErr;
  if (!strcmp(file, "-"))
    err = musicxmlfd2guido(stdin, generateBars, cout);
  else
    err = musicxmlfile2guido(file, generateBars, cout);
  if (err) {
    cout << "conversion failed" << endl;
  }
  return 0;
}
