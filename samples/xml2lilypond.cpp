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
#include <iomanip>      // setw, set::precision, ...

#include "msrUtilities.h"

#include "musicxml2msr.h"

#include "lpsr.h"
#include "msr2lpsr.h"
#include "lpsr2LilyPond.h"


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
    "        Pass 1: reads the contents of MusicXMLFile or stdin ('-')" << endl <<
    "                and converts it to a MusicXML tree;" << endl <<
    "        Pass 2: converts that tree to a Music Score Representation (MSR);" << endl <<
    "        Pass 3: converts the MSR to a LilyPond Score Representation (LPSR);" << endl <<
    "        Pass 4: writes the LPSR as LilyPond source code to standard output." << endl <<
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
    "    --d, --debug " << endl <<
    "          Generate a trace of the activity and print additional" << endl <<
    "          debugging information to standard error." << endl <<
    "    --dd, --debugDebug " << endl <<
    "          Same as above, but print even more debugging information." << endl <<
    "    --dm, --debugMeasures <measuresSpec>" << endl <<
    "          '<measuresSpec>' has a form such as 0,2-14,^8-10 ," << endl <<
    "          where '^' excludes the corresponding numbers interval" << endl <<
    "          and 0 applies to the '<part-list>' and anacrusis if present." <<endl <<
    "          Generate a trace of the activity and print additional" << endl <<
    "          debugging information to standard error for the specified measures." << endl <<
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
    "          but delay them until the next actual note or chord instead." << endl <<
    endl <<

    "    --msr, --displayMSR" << endl <<
    "          Write the contents of the MSR data to standard error." << endl <<
    endl <<

    "    --sum, --displayMSRScoreSummary" << endl <<
    "          Write a summary of the MSR score to standard error." << endl <<
    endl <<

    "  LPSR:" << endl <<
    endl <<

    "    --lpsr, --displayLPSR" << endl <<
    "          Write the contents of the LPSR data to standard error." << endl <<
    endl <<

    "    --abs, --absolute" << endl <<
    "          Generate LilyPond absolute octaves. " << endl <<
    "          By default, relative octaves are generated." << endl <<
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
  
  S_msrOptions msrOpts = msrOptions::create ();
  assert(msrOpts != 0);
    
  // General options
  // ---------------

  msrOpts->fTrace                             = true;
  msrOpts->fDebug                             = false;
  msrOpts->fDebugDebug                        = false;

  // MSR options
  // -----------

  msrOpts->fMsrNoteNamesLanguageAsString      = "dutch";
  msrOpts->fMsrNoteNamesLanguage              = kNederlands;
  
  msrOpts->fCreateStaffRelativeVoiceNumbers   = false;
  
  msrOpts->fDontDisplayMSRLyrics              = false;

  msrOpts->fDelayRestsDynamics                = false;
  
  msrOpts->fDisplayMSR                        = false;

  msrOpts->fDisplayMSRScoreSummary            = false;

  // LPSR options
  // ------------

  S_lpsrOptions lpsrOpts = lpsrOptions::create();
  assert(lpsrOpts != 0);
  
  lpsrOpts->fDisplayLPSR                      = false;

  lpsrOpts->fDontKeepLineBreaks               = false;
  lpsrOpts->fKeepStaffSize                    = false;
    
  lpsrOpts->fGenerateAbsoluteOctaves          = true;

  lpsrOpts->fGenerateNumericalTime            = false;
  lpsrOpts->fGenerateComments                 = true;
  lpsrOpts->fGenerateStems                    = false;
  lpsrOpts->fGeneratePositions                = false;

  lpsrOpts->fDontDisplayLilyPondCode          = false;


  // General options
  // ---------------

  int helpPresent                       = 0;

  int noTracePresent                    = 0;
  int debugPresent                      = 0;
  int debugDebugPresent                 = 0;
  int debugMeasuresPresent              = 0;
  
  // MSR options
  // -----------

  int languagePresent                   = 0;

  int staffRelativeVoiceNumbersPresent  = 0;
  
  int dontDisplayMSRLyricsPresent       = 0;

  int delayRestsDynamicsPresent         = 0;
  
  int displayMSRPresent                 = 0;
  
  int displayMSRScoreSummaryPresent     = 0;
  
  // LPSR options
  // ------------

  int displayLPSRPresent                = 0;

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

    // General options
    // ---------------

    {
      "help",
      no_argument,
      &helpPresent, 1
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
    
    {
      "dd",
      no_argument,
      &debugDebugPresent, 1
    },
    {
      "debugDebug",
      no_argument,
      &debugDebugPresent, 1
    },
    
    {
      "dm",
      required_argument,
      &debugMeasuresPresent, 1
    },
    {
      "debugMeasures",
      required_argument,
      &debugMeasuresPresent, 1
    },

    // MSR options
    // -----------

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

    // LPSR options
    // ------------

    {
      "lpsr",
      no_argument,
      &displayLPSRPresent, 1},
    {
      "displayLPSR",
      no_argument,
      &displayLPSRPresent, 1
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

    {0, 0, 0, 0}
    };

  /* getopt_long stores the option index here. */
  int option_index = 0;

  string commandLineOptions;

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

        // General options
        // ------------

        {
        if (helpPresent) {
          usage (0);
          break;
        }

        if (noTracePresent) {
          msrOpts->fTrace = false;
          commandLineOptions +=
            "--noTrace ";
        }
        if (debugPresent) {
          msrOpts->fTrace = true;
          msrOpts->fDebug = true;
          commandLineOptions +=
            "--debug ";
        }
        if (debugDebugPresent) {
          msrOpts->fTrace = true;
          msrOpts->fDebugDebug = true;
          commandLineOptions +=
            "--debugDebug ";
        }
        if (debugMeasuresPresent) {
          msrOpts->fTrace = true;
          msrOpts->fDebugDebug = true;
          
          char*        measuresSpec = optarg;
          stringstream s;

          s <<
            "--debugMeasures" << " " << measuresSpec;
          commandLineOptions +=
            s.str();
            
          msrOpts->fDebugMeasureNumbersSet =
            decipherNumbersSpecification (measuresSpec);
        }

        // MSR options
        // -----------
        
        if (languagePresent) {
          // optarg contains the language name
          if (gMsrNoteNamesLanguageMap.count(optarg)) {
            msrOpts->fMsrNoteNamesLanguageAsString = optarg;
          }
          else {
            cerr <<
              "--> Unknown language name \"" << optarg <<
              "\", using \"dutch\" instead" << std::endl;
            msrOpts->fMsrNoteNamesLanguageAsString = "dutch";
            msrOpts->fMsrNoteNamesLanguage = kNederlands;
          }
          commandLineOptions +=
            "--language "+msrOpts->fMsrNoteNamesLanguageAsString+" ";
          }
             
        if (staffRelativeVoiceNumbersPresent) {
          msrOpts->fCreateStaffRelativeVoiceNumbers = true;
          commandLineOptions +=
            "--staffRelativeVoiceNumbers ";
        }
        
        if (dontDisplayMSRLyricsPresent) {
          msrOpts->fDontDisplayMSRLyrics = true;
          commandLineOptions +=
            "--dontGenerateLyrics ";
        }
        
        if (delayRestsDynamicsPresent) {
          msrOpts->fDelayRestsDynamics = true;
          commandLineOptions +=
            "--delayRestsDynamics ";
        }
        
        if (displayMSRPresent) {
          msrOpts->fDisplayMSR = true;
          commandLineOptions +=
            "--displayMSR ";
        }

        if (displayMSRScoreSummaryPresent) {
          msrOpts->fDisplayMSRScoreSummary = true;
          commandLineOptions +=
            "--displayScoreSummary ";
        }
        
        // LPSR options
        // ------------

        if (displayLPSRPresent) {
          lpsrOpts->fDisplayLPSR = true;
          commandLineOptions +=
            "--displayLPSR ";
        }

        if (absolutePresent) {
          lpsrOpts->fGenerateAbsoluteOctaves = true;
          commandLineOptions +=
            "--absolute ";
        }
        
        if (numericaltimePresent) {
          lpsrOpts->fGenerateNumericalTime = true;
          commandLineOptions +=
            "--numericalTime ";
        }
        if (noCommentsPresent) {
          lpsrOpts->fGenerateComments = false;
          commandLineOptions +=
            "--noComments ";
        }
        if (stemsPresent) {
          lpsrOpts->fGenerateStems = true;
          commandLineOptions +=
            "--stems ";
        }
        if (positionsPresent) {
          lpsrOpts->fGeneratePositions = true;
          commandLineOptions +=
            "--positions ";
        }
        
        if (dontGenerateLilyPondLyricsPresent) {
          lpsrOpts->fDontGenerateLilyPondLyrics = true;
          commandLineOptions +=
            "--dontGenerateLyrics ";
        }
        
        if (dontDisplayLilyPondCodePresent) {
          lpsrOpts->fDontDisplayLilyPondCode = true;
          commandLineOptions +=
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
      endl;

    cerr <<
      "The command line options are: ";
    if (commandLineOptions.size())
      cerr <<
        endl <<
        commandLineOptions;
    else
      cerr << "none";
    cerr << endl;
      
    // General options
    // ---------------

    cerr << idtr <<
      left <<
      
      "The general options are:" << endl <<
      "  " << setw(31) << "trace" << " : " <<
        string(msrOpts->fTrace
          ? "true" : "false") << endl <<
      "  " << setw(31) << "debug" << " : " <<
        string(msrOpts->fDebug
          ? "true" : "false") << endl <<
      "  " << setw(31) << "debugDebug" << " : " <<
        string(msrOpts->fDebugDebug
          ? "true" : "false") << endl <<
      "  " << setw(31) << "debugMeasureNumbersSet" << " : ";
      for (
        set<int>::const_iterator i =
          msrOpts->fDebugMeasureNumbersSet.begin();
        i != msrOpts->fDebugMeasureNumbersSet.end();
        i++) {
          cout << (*i) << " ";
      } // for
    cerr << endl;

    // MSR options
    // -----------

    cerr << idtr <<
      left <<
      
      "The MSR options are:" << endl <<
      "  " << setw(31) << "noteNamesLanguageName" << " : \"" <<
        msrOpts->fMsrNoteNamesLanguageAsString << "\"" << endl <<
      
      "  " << setw(31) << "createStaffRelativeVoiceNumbers" << " : " <<
        string(msrOpts->fCreateStaffRelativeVoiceNumbers
          ? "true" : "false") << endl <<
 
      "  " << setw(31) << "dontDisplayMSRLyrics" << " : " <<
        string(msrOpts->fDontDisplayMSRLyrics
          ? "true" : "false") << endl <<

      "  " << setw(31) << "delayRestsDynamics" << " : " <<
        string(msrOpts->fDelayRestsDynamics
          ? "true" : "false") << endl <<

      "  " << setw(31) << "displayMSR" << " : " <<
        string(msrOpts->fDisplayMSR
          ? "true" : "false") << endl <<
      
      "  " << setw(31) << "displayMSRScoreSummary" << " : " <<
        string(msrOpts->fDisplayMSRScoreSummary
          ? "true" : "false") << endl;
      
    // LPSR options
    // ------------

    cerr << idtr <<
      left <<
      
      "The LPSR options are:" << endl <<
      "  " << setw(31) << "displayLPSR" << " : " <<
        string(lpsrOpts->fDisplayLPSR
          ? "true" : "false") << endl <<

      "  " << setw(31) << "generateAbsoluteOctaves" << " : " <<
        string(lpsrOpts->fGenerateAbsoluteOctaves
          ? "true" : "false") << endl <<
      
      "  " << setw(31) << "generateNumericalTime" << " : " <<
        string(lpsrOpts->fGenerateNumericalTime
          ? "true" : "false") << endl <<
      "  " << setw(31) << "generateComments" << " : " <<
        string(lpsrOpts->fGenerateComments
        ? "true" : "false") << endl <<
      "  " << setw(31) << "generateStems" << " : " <<
        string(lpsrOpts->fGenerateStems
        ? "true" : "false") << endl <<
      "  " << setw(31) << "generatePositions" << " : " <<
        string(lpsrOpts->fGeneratePositions
        ? "true" : "false") << endl <<

      "  " << setw(31) << "dontGenerateLilyPondLyrics" << " : " <<
        string(lpsrOpts->fDontGenerateLilyPondLyrics
          ? "true" : "false") << endl <<

      "  " << setw(31) << "dontDisplayLilyPondCode" << " : " <<
        string(lpsrOpts->fDontDisplayLilyPondCode
        ? "true" : "false") << endl;
    
  
  S_msrScore mScore;

  // create MSR score from file contents
  if (! strcmp (fileName, "-"))
    mScore = musicxmlFd2Msr (stdin, msrOpts, cout);
  else
    mScore = musicxmlFile2Msr (fileName, msrOpts, cout);
    
  if (! mScore) {
    cout <<
      "### Conversion from MusicCML to MSR failed ###" << endl <<
      endl;
    return 1;
  }

  // create LPSR score from MSR score
  S_lpsrScore
    lpScore =
      msr2lpsr (mScore, msrOpts, lpsrOpts, cout);
  
  if (! lpScore) {
    cout <<
      "### Conversion from MSR to LPSR failed ###" << endl <<
      endl;
    return 1;
  }

  // generate LilyPond code from LPSR score
  lpsr2LilyPond (lpScore, msrOpts, lpsrOpts, cout);
  
  if (! true) { // JMI
    cout <<
      "### Conversion from LPSR to LilyPond code failed ###" << endl <<
      endl;
    return 1;
  }

  return 0;
}
