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

#include "lpsrUtilities.h"

#include "musicxml2lpsr.h"

using namespace std;
using namespace MusicXML2;

void usage(int exitStatus) {
  cerr <<
    endl <<
    "This is xml2lilypond, the MusicXML to LilyPond translator" << endl <<
    "embedded in the libmusicxml2 library." << endl <<
    endl <<
    "Usage:" << endl <<
    "    xml2lilypond [options] [MusicXMLFile|-]" << endl <<
    endl <<
    "What it does:" << endl <<
    "    Read the contents of MusicXMLFile or stdin ('-')," << endl <<
    "    convert it to LilyPond Semantic Representation (LPSR) internally" << endl <<
    "    and write LilyPond source code to standard output." << endl <<
    "    The activity log and warning/error messages go to standard error." << endl <<
    endl <<
    "Options:" << endl <<
    "  --help'" << endl <<
    "        Display this help" << endl <<
    endl <<
    "  --abs, --absolute" << endl <<
    "        Generate LilyPond absolute code. " << endl <<
    "        By default relative code is generated" << endl <<
    endl <<
    "  --numericalTime" << endl <<
    "        Don't generate non-numerical time signatures such as 'C'" << endl <<
    "  --noComments" << endl <<
    "        Don't generate comments" << endl <<
    "  --noAutobars" << endl <<
    "        Don't generate barlines" << endl <<
    "  --noStems" << endl <<
    "        Don't generate stems commands" << endl <<
    "  --noPositions" << endl <<
    "        Don't generate positions" << endl <<
    endl <<
    "  --drd, --delayRestsDynamics" << endl <<
    "        Don't generate dynamics and wedges on rests," << endl <<
    "        but delay them until the next actual note instead" << endl <<
    endl <<
    "  --nt, --noTrace" << endl <<
    "        Don't generate a trace of the activity to standard error" << endl <<
    "  --d, --debug  " << endl <<
    "        Print debugging information to standard error" << endl <<
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
  
  int helpPresent =                 0;
  
  int languagePresent =           0;
  
  int absolutePresent =           0;
  int numericaltimePresent =      0;
  int nocommentsPresent =         0;
  int noautobarsPresent =         0;
  int stemsPresent =              0;
  int positionsPresent =          0;

  int delayRestsDynamicsPresent = 0;
  
  int notracePresent =            0;
  int debugPresent =              0;
  
  std::string selectedOptions = "";
  
  std::string           noteNamesLanguageName = "dutch";
  LpsrNoteNamesLanguage noteNamesLanguage =     kNederlands;
  
  bool                  generateAbsoluteCode =  true;
  
  bool                  generateNumericalTime = false;
  bool                  generateComments =      true;
  bool                  generateBars =          true;
  bool                  generateStems =         false;
  bool                  generatePositions =     false;

  bool                  delayRestsDynamics =    false;
  
  bool                  trace =                 true;
  bool                  debug =                 false;
  
  static struct option long_options [] =
    {
    /* These options set a flag. */
    {"help",               no_argument,       &helpPresent, 1},
    
    {"language",           required_argument, &languagePresent, 1},
    
    {"abs",                no_argument,       &absolutePresent, 1},
    {"absolute",           no_argument,       &absolutePresent, 1},
    
    {"numericalTime",      no_argument,       &numericaltimePresent, 1},
    {"noComments",         no_argument,       &nocommentsPresent, 1},
    {"noAutobars",         no_argument,       &noautobarsPresent, 1},
    {"stems",              no_argument,       &stemsPresent, 1},
    {"positions",          no_argument,       &positionsPresent, 1},

    {"drd",                no_argument,       &notracePresent, 1},
    {"delayRestsDynamics", no_argument,       &notracePresent, 1},
   
    {"nt",                 no_argument,       &notracePresent, 1},
    {"noTrace",            no_argument,       &notracePresent, 1},
    {"d",                  no_argument,       &debugPresent, 1},
    {"debug",              no_argument,       &debugPresent, 1},
    
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
        
        if (languagePresent) {
          // optarg contains the language name
          if (gLpsrNoteNamesLanguageMap.count(optarg)) {
            noteNamesLanguageName = optarg;
          } else {
            cerr
              << "--> Unknown language name \"" << optarg <<
              "\", using \"dutch\" instead" << std::endl;
            noteNamesLanguageName = "dutch";
            noteNamesLanguage = kNederlands;
          }
          selectedOptions += "--language "+noteNamesLanguageName;
          }
          break;
             
        if (absolutePresent) {
          generateAbsoluteCode = true;
          selectedOptions += "--absolute ";
          break;
        }
        
        if (numericaltimePresent) {
          generateNumericalTime = true;
          selectedOptions += "--numericalTime ";
          break;
        }
        if (nocommentsPresent) {
          generateComments = false;
          selectedOptions += "--noComments ";
          break;
        }
        if (noautobarsPresent) {
          generateBars = false;
          selectedOptions += "--noAutobars ";
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
        
        if (delayRestsDynamicsPresent) {
          delayRestsDynamics = true;
          selectedOptions += "--delayRestsDynamics ";
          break;
        }
        
        if (notracePresent) {
          trace = false;
          selectedOptions += "--noTrace ";
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

  // create the translation switches
  S_translationSettings ts = translationSettings::create();
  assert(ts != 0);
  
  // populate them
  ts->fLpsrNoteNamesLanguageAsString = noteNamesLanguageName;
  ts->fLpsrNoteNamesLanguage =         noteNamesLanguage;
  
  ts->fGenerateAbsoluteCode =          generateAbsoluteCode;
  
  ts->fGenerateNumericalTime =         generateNumericalTime;
  ts->fGenerateComments =              generateComments;
  ts->fGenerateBars =                  generateBars;
  ts->fGenerateStems =                 generateStems;
  ts->fGeneratePositions =             generatePositions;

  ts->fDelayRestsDynamics  =           delayRestsDynamics;
  
  ts->fTrace =                         trace;
  ts->fDebug =                         debug;
  
  ts->fSelectedOptions =               selectedOptions;

  if (ts->fTrace)
    cerr << 
      "Launching conversion to LilyPond with libmusicxml2 v" << 
      musicxmllibVersionStr() <<
      " & xml2Lilypond v" <<
      musicxml2LpsrVersionStr() << 
      endl <<
      "The settings are:" << endl <<
      "  noteNamesLanguageName: \"" << noteNamesLanguageName << "\"" << endl <<
      "  generateAbsoluteCode:  " << string(generateAbsoluteCode ? "true" : "false") << endl <<
      "  generateNumericalTime: " << string(generateNumericalTime ? "true" : "false") << endl <<
      "  generateComments:      " << string(generateComments ? "true" : "false") << endl <<
      "  generateBars:          " << string(generateBars ? "true" : "false") << endl <<
      "  generateStems:         " << string(generateStems ? "true" : "false") << endl <<
      "  generatePositions:     " << string(generatePositions ? "true" : "false") << endl <<
      "  trace:                 " << string(trace ? "true" : "false") << endl <<
      "  debug:                 " << string(debug ? "true" : "false") << endl;
  
  xmlErr err = kNoErr;
  
  if (!strcmp (file, "-"))
    err = musicxmlFd2Lpsr (stdin, ts, cout);
  else
    err = musicxmlFile2Lpsr (file, ts, cout);
  if (err) {
    cout << "### Conversion from MusicCML to LilyPond failed ###" << endl;
  }
  
  return 0;
}
