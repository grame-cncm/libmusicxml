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

#include "msrUtilities.h"

#include "musicxml2msr.h"

using namespace std;
using namespace MusicXML2;

void usage(int exitStatus) {
  cerr <<
    endl <<
    "                   Welcome to xml2lilypond, " << endl <<
    "              the MusicXML to LilyPond translator" << endl <<
    "          delivered as part of the libmusicxml2 library." << endl <<
    endl <<
    "Usage:" << endl <<
    "    xml2lilypond [options] [MusicXMLFile|-]" << endl <<
    endl <<
    "What it does:" << endl <<
    "    This multi-pass translator features:" << endl <<
    "        Pass 1: read the contents of MusicXMLFile or stdin ('-')" << endl <<
    "                and convert it to a MusicXML tree;" << endl <<
    "        Pass 2: convert that tree to Music Score Representation (MSR);" << endl <<
    "        Pass 3: convert the MSR to LilyPond Score Representation (LPSR);" << endl <<
    "        Pass 4: write the LPSR as LilyPond source code to standard output." << endl <<
    "        The activity log and warning/error messages go to standard error." << endl <<
    endl <<
    "Options:" << endl <<
    "  --help'" << endl <<
    "        Display this help." << endl <<
    endl <<
    "  --abs, --absolute" << endl <<
    "        Generate LilyPond absolute code. " << endl <<
    "        By default, relative code is generated." << endl <<
    endl <<
    "  --numericalTime" << endl <<
    "        Don't generate non-numerical time signatures such as 'C'." << endl <<
    "  --noComments" << endl <<
    "        Don't generate comments." << endl <<
    "  --noAutobars" << endl <<
    "        Don't generate barlines." << endl <<
    "  --noStems" << endl <<
    "        Don't generate stems commands." << endl <<
    "  --noPositions" << endl <<
    "        Don't generate positions." << endl <<
    endl <<
    "  --drd, --delayRestsDynamics" << endl <<
    "        Don't generate dynamics and wedges on rests," << endl <<
    "        but delay them until the next actual note instead." << endl <<
    endl <<
    "  --nt, --noTrace" << endl <<
    "        Don't generate a trace of the activity to standard error." << endl <<
    "  --d, --debug  " << endl <<
    "        Generate a trace of the activity and print additional" << endl <<
    "        debugging information to standard error." << endl <<
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
  
  // create the translation switches
  S_translationSettings ts = translationSettings::create();
  assert(ts != 0);
  
  ts->fSelectedOptions                   = "";
  
  ts->fMsrNoteNamesLanguageAsString      = "dutch";
  ts->fMsrNoteNamesLanguage              = kNederlands;
  
  ts->fGenerateStaffRelativeVoiceNumbers = false;
  
  ts->fGenerateAbsoluteCode              = true;

  ts->fGenerateNumericalTime             = false;
  ts->fGenerateComments                  = true;
  ts->fGenerateStems                     = false;
  ts->fGeneratePositions                 = false;

  ts->fDelayRestsDynamics                = false;
  
  ts->fDisplayMSR                        = false;

  ts->fTrace                             = true;
  ts->fDebug                             = false;

  // to detect supplied options
  int helpPresent                      = 0;
  
  int languagePresent                  = 0;

  int staffRelativeVoiceNumbersPresent = 0;
  
  int absolutePresent                  = 0;
  int numericaltimePresent             = 0;
  int noCommentsPresent                = 0;
  int stemsPresent                     = 0;
  int positionsPresent                 = 0;

  int delayRestsDynamicsPresent        = 0;
  
  int displayMSRPresent                = 0;

  int noTracePresent                   = 0;
  int debugPresent                     = 0;

  static struct option long_options [] =
    {
    /* These options set a flag. */
    {"help",                      no_argument,       &helpPresent, 1},
    
    {"language",                  required_argument, &languagePresent, 1},
    
    {"srvn",                      no_argument,       &staffRelativeVoiceNumbersPresent, 1},
    {"staffRelativeVoiceNumbers", no_argument,       &staffRelativeVoiceNumbersPresent, 1},
    
    {"abs",                       no_argument,       &absolutePresent, 1},
    {"absolute",                  no_argument,       &absolutePresent, 1},
    
    {"numericalTime",             no_argument,       &numericaltimePresent, 1},
    {"noComments",                no_argument,       &noCommentsPresent, 1},
    {"stems",                     no_argument,       &stemsPresent, 1},
    {"positions",                 no_argument,       &positionsPresent, 1},

    {"drd",                       no_argument,       &delayRestsDynamicsPresent, 1},
    {"delayRestsDynamics",        no_argument,       &delayRestsDynamicsPresent, 1},
   
    {"msr",                       no_argument,       &displayMSRPresent, 1},
    {"displayMSR",                no_argument,       &displayMSRPresent, 1},

    {"nt",                        no_argument,       &noTracePresent, 1},
    {"noTrace",                   no_argument,       &noTracePresent, 1},
    {"d",                         no_argument,       &debugPresent, 1},
    {"debug",                     no_argument,       &debugPresent, 1},
    
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
          if (gMsrNoteNamesLanguageMap.count(optarg)) {
            ts->fMsrNoteNamesLanguageAsString = optarg;
          } else {
            cerr <<
              "--> Unknown language name \"" << optarg <<
              "\", using \"dutch\" instead" << std::endl;
            ts->fMsrNoteNamesLanguageAsString = "dutch";
            ts->fMsrNoteNamesLanguage = kNederlands;
          }
          ts->fSelectedOptions +=
            "--language "+ts->fMsrNoteNamesLanguageAsString+" ";
          }
          break;
             
        if (staffRelativeVoiceNumbersPresent) {
          ts->fGenerateStaffRelativeVoiceNumbers = true;
          ts->fSelectedOptions += "--staffRelativeVoiceNumbers ";
          break;
        }
        
        if (absolutePresent) {
          ts->fGenerateAbsoluteCode = true;
          ts->fSelectedOptions += "--absolute ";
          break;
        }
        
        if (numericaltimePresent) {
          ts->fGenerateNumericalTime = true;
          ts->fSelectedOptions += "--numericalTime ";
          break;
        }
        if (noCommentsPresent) {
          ts->fGenerateComments = false;
          ts->fSelectedOptions += "--noComments ";
          break;
        }
        if (stemsPresent) {
          ts->fGenerateStems = true;
          ts->fSelectedOptions += "--stems ";
          break;
        }
        if (positionsPresent) {
          ts->fGeneratePositions = true;
          ts->fSelectedOptions += "--positions ";
          break;
        }
        
        if (delayRestsDynamicsPresent) {
          ts->fDelayRestsDynamics = true;
          ts->fSelectedOptions += "--delayRestsDynamics ";
          break;
        }
        
        if (displayMSRPresent) {
          ts->fDisplayMSR = true;
          ts->fSelectedOptions += "--noAutobars ";
          break;
        }

        if (noTracePresent) {
          ts->fTrace = false;
          ts->fSelectedOptions += "--noTrace ";
          break;
        }
        if (debugPresent) {
          ts->fTrace = true;
          ts->fDebug = true;
          ts->fSelectedOptions += "--debug ";
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

  // for TESTS
  ts->fDisplayMSR                        = true;
  //ts->fTrace = true;
  //ts->fDebug = true;
  
  if (ts->fTrace)
    cerr << 
      "Launching conversion to LilyPond with libmusicxml2 v" << 
      musicxmllibVersionStr() <<
      " & xml2Lilypond v" <<
      musicxml2MsrVersionStr() << 
      endl <<
      "The settings are:" << endl <<
      "  noteNamesLanguageName: \"" << ts->fMsrNoteNamesLanguageAsString << "\"" << endl <<
      
      "  displayMSR:            " << string(ts->fDisplayMSR ? "true" : "false") << endl <<

      "  generateAbsoluteCode:  " << string(ts->fGenerateAbsoluteCode ? "true" : "false") << endl <<
      
      "  generateNumericalTime: " << string(ts->fGenerateNumericalTime ? "true" : "false") << endl <<
      "  generateComments:      " << string(ts->fGenerateComments ? "true" : "false") << endl <<
      "  generateStems:         " << string(ts->fGenerateStems ? "true" : "false") << endl <<
      "  generatePositions:     " << string(ts->fGeneratePositions ? "true" : "false") << endl <<

      "  delayRestsDynamics:    " << string(ts->fDelayRestsDynamics ? "true" : "false") << endl <<
    
      "  trace:                 " << string(ts->fTrace ? "true" : "false") << endl <<
      "  debug:                 " << string(ts->fDebug ? "true" : "false") << endl;
  
  xmlErr err = kNoErr;
  
  if (!strcmp (file, "-"))
    err = musicxmlFd2Msr (stdin, ts, cout);
  else
    err = musicxmlFile2Msr (file, ts, cout);
  if (err) {
    cout << "### Conversion from MusicCML to LilyPond failed ###" << endl;
  }
  
  return 0;
}
