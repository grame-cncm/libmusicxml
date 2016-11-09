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

#include "lpsr.h"

#include "musicxml2msr.h"

using namespace std;
using namespace MusicXML2;

void usage (int exitStatus) {
  cerr <<
    endl <<
    "                   Welcome to xml2lilypond, " << endl <<
    "              the MusicXML to LilyPond translator" << endl <<
    "          delivered as part of the libmusicxml2 library." << endl <<
    endl <<
    "Usage:" << endl <<
    endl <<
    "    xml2lilypond [options] [MusicXMLFile|-]" << endl <<
    endl <<
    "What it does:" << endl <<
    endl <<
    "    This multi-pass translator features:" << endl <<
    "        Pass 1: read the contents of MusicXMLFile or stdin ('-')" << endl <<
    "                and convert it to a MusicXML tree;" << endl <<
    "        Pass 2: convert that tree to Music Score Representation (MSR);" << endl <<
    "        Pass 3: augment the MSR to obtain a LilyPond Score Representation (LPSR);" << endl <<
    "        Pass 4: write the LPSR as LilyPond source code to standard output." << endl <<
    "    The activity log and warning/error messages go to standard error." << endl <<
    endl <<

    "Options:" << endl <<
    endl <<

    "  General:" << endl <<
    endl <<
    "    --help" << endl <<
    "          Display this help." << endl <<
    endl <<

    "    --nt, --noTrace" << endl <<
    "          Don't generate a trace of the activity to standard error." << endl <<
    "    --d, --debug  " << endl <<
    "          Generate a trace of the activity and print additional" << endl <<
    "          debugging information to standard error." << endl <<
    endl <<

    "  MSR:" << endl <<
    endl <<

    "    --srvn, --staffRelativeVoiceNumbers" << endl <<
    "          Generate voices names with numbers relative to their staff." << endl <<
    "          By default, the voice numbers found are used, " << endl <<
    "          which may be global to the score." << endl <<
    endl <<
    
    "    --noml, --dontDisplayMSRLyrics" << endl <<
    "          Don't display MSR lyrics while displaying MSR data." << endl <<
    endl <<

    "    --drd, --delayRestsDynamics" << endl <<
    "          Don't attach dynamics and wedges to rests," << endl <<
    "          but delay them until the next actual note instead." << endl <<
    endl <<

    "    --msr, --displayMSR" << endl <<
    "          Write the contents of the MSR data to standard error." << endl <<
    endl <<

    "    --sum, --displayMSRScoreSummary" << endl <<
    "          Write a summary of the MSR score to standard error." << endl <<
    endl <<

    "  LPSR:" << endl <<
    endl <<

    "    --abs, --absolute" << endl <<
    "          Generate LilyPond absolute code. " << endl <<
    "          By default, relative code is generated." << endl <<
    endl <<

    "    --numericalTime" << endl <<
    "          Don't generate non-numerical time signatures such as 'C'." << endl <<
    "    --noComments" << endl <<
    "          Don't generate comments." << endl <<
    "    --noStems" << endl <<
    "          Don't generate stems commands." << endl <<
    "    --noPositions" << endl <<
    "          Don't generate positions." << endl <<
    endl <<
    "    --nolpl, --dontGenerateLilyPondLyrics" << endl <<
    "          Don't generate lyrics in the LilyPond code." << endl <<
    endl <<
    "    --nolpc, --dontDisplayLilyPondCode" << endl <<
    "          Don't write the LilyPond code to standard output." << endl <<
    "          This can be useful if only a summary of the score is needed." << endl <<
    endl <<

    endl <<

    endl;
  exit(exitStatus);
}

//_______________________________________________________________________________
int main (int argc, char *argv[]) 
{
 /*
  cout << "argc = " << argc << endl;
  for (int i = 0; i < argc ; i++ ) {
    cout << "argv[ " << i << "] = " << argv[i] << endl;
  }
  */
  
  // create the MSR options
  S_msrOptions msrOpts = msrOptions::create();
  assert(msrOpts != 0);
  
  msrOpts->fMSRCommandLineOptions             = "";
  
  msrOpts->fMsrNoteNamesLanguageAsString      = "dutch";
  msrOpts->fMsrNoteNamesLanguage              = kNederlands;
  
  msrOpts->fCreateStaffRelativeVoiceNumbers   = false;
  
  msrOpts->fDontDisplayMSRLyrics              = false;

  msrOpts->fDelayRestsDynamics                = false;
  
  msrOpts->fDisplayMSR                        = false;

  msrOpts->fDisplayMSRScoreSummary            = false;

  msrOpts->fTrace                             = true;
  msrOpts->fDebug                             = false;

  // create the LPSR options
  S_lpsrOptions lpsrOpts = lpsrOptions::create();
  assert(lpsrOpts != 0);
  
  lpsrOpts->fLPSRCommandLineOptions           = "";

  lpsrOpts->fDontKeepLineBreaks               = false;
  lpsrOpts->fKeepStaffSize                    = false;
    
  lpsrOpts->fGenerateAbsoluteCode             = true;

  lpsrOpts->fGenerateNumericalTime            = false;
  lpsrOpts->fGenerateComments                 = true;
  lpsrOpts->fGenerateStems                    = false;
  lpsrOpts->fGeneratePositions                = false;

  lpsrOpts->fDontDisplayLilyPondCode          = false;


  // to detect supplied MSR options
  int helpPresent                       = 0;

  int noTracePresent                    = 0;
  int debugPresent                      = 0;
  
  int languagePresent                   = 0;

  int staffRelativeVoiceNumbersPresent  = 0;
  
  int dontDisplayMSRLyricsPresent       = 0;

  int delayRestsDynamicsPresent         = 0;
  
  int displayMSRPresent                 = 0;
  
  int displayMSRScoreSummaryPresent     = 0;
  
  // to detect supplied LPSR options
  int fDontKeepLineBreaksPresent        = 0;
  int fKeepStaffSizePresent             = 0;
  
  int absolutePresent                   = 0;
  
  int numericaltimePresent              = 0;
  int noCommentsPresent                 = 0;
  int stemsPresent                      = 0;
  int positionsPresent                  = 0;
  
  int dontGenerateLilyPondLyricsPresent = 0;

  int dontDisplayLilyPondCodePresent    = 0;


  static struct option long_options [] =
    {
    /* These options set a flag. */
    {
      "help",
      no_argument,
      &helpPresent, 1
    },
    
    {
      "language",
      required_argument,
      &languagePresent, 1
    },
    
    {
      "srvn",
      no_argument,
      &staffRelativeVoiceNumbersPresent, 1
    },
    {
      "staffRelativeVoiceNumbers",
      no_argument,
      &staffRelativeVoiceNumbersPresent, 1
    },
    
    {
      "abs",
      no_argument,
      &absolutePresent, 1
    },
    {
      "absolute",
      no_argument,
      &absolutePresent, 1
    },
    
    {
      "numericalTime",
      no_argument,
      &numericaltimePresent, 1
    },
    {
      "noComments",
      no_argument,
      &noCommentsPresent, 1
    },
    {
      "stems",
      no_argument,
      &stemsPresent, 1
    },
    {
      "positions",
      no_argument,
      &positionsPresent, 1
    },
    
    {
      "noml",
      no_argument,
      &dontDisplayMSRLyricsPresent, 1
    },
    {
      "dontDisplayMSRLyrics",
      no_argument,
      &dontDisplayMSRLyricsPresent, 1
    },

    {
      "drd",
      no_argument,
      &delayRestsDynamicsPresent, 1
    },
    {
      "delayRestsDynamics",
      no_argument,
      &delayRestsDynamicsPresent, 1
    },
   
    {
      "msr",
      no_argument,
      &displayMSRPresent, 1},
    {
      "displayMSR",
      no_argument,
      &displayMSRPresent, 1
    },

    {
      "sum",
      no_argument,
      &displayMSRScoreSummaryPresent, 1
    },
    {
      "displayMSRScoreSummary",
      no_argument,
      &displayMSRScoreSummaryPresent, 1
    },

    {
      "nolpl",
      no_argument,
      &dontGenerateLilyPondLyricsPresent, 1
    },
    {
      "dontGenerateLilyPondLyrics",
      no_argument,
      &dontGenerateLilyPondLyricsPresent, 1
    },

    {
      "nolpc",
      no_argument,
      &dontDisplayLilyPondCodePresent, 1
    },
    {
      "dontDisplayLilyPondCode",
      no_argument,
      &dontDisplayLilyPondCodePresent, 1
    },

    {
      "nt",
      no_argument,
      &noTracePresent, 1
    },
    {
      "noTrace",
      no_argument,
      &noTracePresent, 1
    },
    {
      "d",
      no_argument,
      &debugPresent, 1
    },
    {
      "debug",
      no_argument,
      &debugPresent, 1
    },
    
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
    /*
    cerr << "c = " << c << endl;
    cerr << "option_index = " << option_index << endl;
    
    cerr <<
      "option : " << long_options[option_index].name <<
      " with flag " << long_options[option_index].flag;
    if (optarg)
      cerr << " and arg " << optarg << "\"";
    cerr << endl;

    //cerr << "debugPresent = " << debugPresent << endl;
    */
    
    switch (c)
      {
      case 0 :
        {
        if (helpPresent) {
          usage (0);
          break;
        }

        // MSR options
        // -----------
        
        if (languagePresent) {
          // optarg contains the language name
          if (gMsrNoteNamesLanguageMap.count(optarg)) {
            msrOpts->fMsrNoteNamesLanguageAsString = optarg;
          } else {
            cerr <<
              "--> Unknown language name \"" << optarg <<
              "\", using \"dutch\" instead" << std::endl;
            msrOpts->fMsrNoteNamesLanguageAsString = "dutch";
            msrOpts->fMsrNoteNamesLanguage = kNederlands;
          }
          msrOpts->fMSRCommandLineOptions +=
            "--language "+msrOpts->fMsrNoteNamesLanguageAsString+" ";
          }
             
        if (staffRelativeVoiceNumbersPresent) {
          msrOpts->fCreateStaffRelativeVoiceNumbers = true;
          msrOpts->fMSRCommandLineOptions +=
            "--staffRelativeVoiceNumbers ";
        }
        
        if (dontDisplayMSRLyricsPresent) {
          msrOpts->fDontDisplayMSRLyrics = true;
          msrOpts->fMSRCommandLineOptions +=
            "--dontGenerateLyrics ";
        }
        
        if (delayRestsDynamicsPresent) {
          msrOpts->fDelayRestsDynamics = true;
          msrOpts->fMSRCommandLineOptions +=
            "--delayRestsDynamics ";
        }
        
        if (displayMSRPresent) {
          msrOpts->fDisplayMSR = true;
          msrOpts->fMSRCommandLineOptions +=
            "--displayMSR ";
        }

        if (displayMSRScoreSummaryPresent) {
          msrOpts->fDisplayMSRScoreSummary = true;
          msrOpts->fMSRCommandLineOptions +=
            "--displayScoreSummary ";
        }
        
        if (noTracePresent) {
          msrOpts->fTrace = false;
          msrOpts->fMSRCommandLineOptions +=
            "--noTrace ";
        }
        if (debugPresent) {
          msrOpts->fTrace = true;
          msrOpts->fDebug = true;
          msrOpts->fMSRCommandLineOptions +=
            "--debug ";
        }

        // LPSR options
        // ------------

        if (absolutePresent) {
          lpsrOpts->fGenerateAbsoluteCode = true;
          lpsrOpts->fLPSRCommandLineOptions +=
            "--absolute ";
        }
        
        if (numericaltimePresent) {
          lpsrOpts->fGenerateNumericalTime = true;
          lpsrOpts->fLPSRCommandLineOptions +=
            "--numericalTime ";
        }
        if (noCommentsPresent) {
          lpsrOpts->fGenerateComments = false;
          lpsrOpts->fLPSRCommandLineOptions +=
            "--noComments ";
        }
        if (stemsPresent) {
          lpsrOpts->fGenerateStems = true;
          lpsrOpts->fLPSRCommandLineOptions +=
            "--stems ";
        }
        if (positionsPresent) {
          lpsrOpts->fGeneratePositions = true;
          lpsrOpts->fLPSRCommandLineOptions +=
            "--positions ";
        }
        
        if (dontGenerateLilyPondLyricsPresent) {
          lpsrOpts->fDontGenerateLilyPondLyrics = true;
          lpsrOpts->fLPSRCommandLineOptions +=
            "--dontGenerateLyrics ";
        }
        
        if (dontDisplayLilyPondCodePresent) {
          lpsrOpts->fDontDisplayLilyPondCode = true;
          lpsrOpts->fLPSRCommandLineOptions +=
            "--dontDisplayLilyPondCode ";
        }

        }
        break;
        
      default:
        usage (1);
        break;
      } // switch
    } // while
 
  int nonOptionArgs = argc-optind;

  char * fileName = "";

  switch (nonOptionArgs)
    {
    case 1 :
      fileName = argv [optind];
      break;

    default:
      usage (1);
      break;
    } //  switch

  // int   remainingArgs = nonOptionArgs;

  // for TESTS
  //msrOpts->fDisplayMSR                        = true;
  //msrOpts->fTrace = true;
  //msrOpts->fDebug = true;
  
  if (msrOpts->fTrace)
    cerr << 
      "Launching conversion of ";
    if (fileName == "")
      cerr << "standard input";
    else
      cerr << fileName;
    cerr << idtr <<
      " to LilyPond " <<
      endl <<
      "with libmusicxml2 v" << musicxmllibVersionStr() <<
      " & xml2Lilypond v" << musicxml2MsrVersionStr() << 
      endl <<
      "The MSR oprions are:" << endl <<
      "  noteNamesLanguageName      : \"" <<
        msrOpts->fMsrNoteNamesLanguageAsString << "\"" << endl <<
      
      "  displayMSR                 : " <<
        string(msrOpts->fDisplayMSR
          ? "true" : "false") << endl <<

      "  dontDisplayMSRLyrics       : " <<
        string(msrOpts->fDontDisplayMSRLyrics
          ? "true" : "false") << endl <<

      "  displayMSR                 : " <<
        string(msrOpts->fDisplayMSR
          ? "true" : "false") << endl <<
      
      "  displayMSRScoreSummary     : " <<
        string(msrOpts->fDisplayMSRScoreSummary
          ? "true" : "false") << endl <<
      
      "  trace                      : " <<
        string(msrOpts->fTrace
          ? "true" : "false") << endl <<
      "  debug                      : " <<
        string(msrOpts->fDebug
          ? "true" : "false") << endl <<

      "The LPSR oprions are:" << endl <<
      "  generateAbsoluteCode       : " <<
        string(lpsrOpts->fGenerateAbsoluteCode
          ? "true" : "false") << endl <<
      
      "  generateNumericalTime      : " <<
        string(lpsrOpts->fGenerateNumericalTime
          ? "true" : "false") << endl <<
      "  generateComments           : " <<
        string(lpsrOpts->fGenerateComments
        ? "true" : "false") << endl <<
      "  generateStems              : " <<
        string(lpsrOpts->fGenerateStems
        ? "true" : "false") << endl <<
      "  generatePositions          : " <<
        string(lpsrOpts->fGeneratePositions
        ? "true" : "false") << endl <<

      "  dontGenerateLilyPondLyrics : " <<
        string(lpsrOpts->fDontGenerateLilyPondLyrics
          ? "true" : "false") << endl <<

      "  dontDisplayLilyPondCode    : " <<
        string(lpsrOpts->fDontDisplayLilyPondCode
        ? "true" : "false") << endl;
    
  
  xmlErr err = kNoErr;
  
  if (!strcmp (fileName, "-"))
  
    err = musicxmlFd2Msr (stdin, msrOpts, cout);
    
  else
  
    err = musicxmlFile2Msr (fileName, msrOpts, cout);
    
  if (err) {
    cout << "### Conversion from MusicCML to LilyPond failed ###" << endl <<
    endl;
  }
  
  return 0;
}
