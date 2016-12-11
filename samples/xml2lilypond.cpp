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

#include <fstream>      // ofstream, ofstream::open(), ofstream::close()

#include "msrUtilities.h"

#include "xml2Msr.h"

#include "lpsr.h"
#include "msr2Lpsr.h"
#include "lpsr2LilyPond.h"


using namespace std;
using namespace MusicXML2;

//_______________________________________________________________________________
void printUsage (int exitStatus)
{
  cerr <<
    endl <<
    "                   Welcome to xml2lilypond, " << endl <<
    "              the MusicXML to LilyPond translator" << endl <<
    "          delivered as part of the libmusicxml2 library." << endl <<
    "        https://github.com/dfober/libmusicxml/tree/master" << endl <<
    endl <<
    "Usage:" << endl <<
    endl <<
    "    xml2lilypond [options] [MusicXMLFile|-]" << endl <<
    endl <<
    "What it does:" << endl <<
    endl <<
    "    This multi-pass translator basically performs 4 passes:" << endl <<
    "        Pass 1: reads the contents of MusicXMLFile or stdin ('-')" << endl <<
    "                and converts it to a MusicXML tree;" << endl <<
    "        Pass 2: converts that tree to a Music Score Representation (MSR);" << endl <<
    "        Pass 3: converts the MSR to a LilyPond Score Representation (LPSR);" << endl <<
    "        Pass 4: writes the LPSR as LilyPond source code to standard output." << endl <<
    endl <<
    "    Other passes are performed according to the options, such as" << endl <<
    "    printing views of the internal data or printing a summary of the score." << endl <<
    endl <<
    "    The activity log and warning/error messages go to standard error." << endl <<
    endl <<

    "Options:" << endl <<
    endl <<

    "  General:" << endl <<
    endl <<
    "    --help" << endl <<
    "          Display this help." << endl <<
    endl <<

    "    --of, --outputFile fileName" << endl <<
    "          Write LilyPond code to file 'fileName' instead of standard output." << endl <<
    endl <<

//    "    --i, --interactive" << endl <<
//    "          Don't create and LilyPond output file, " << endl <<
//    "          but print the code to standard output instead." << endl <<
//    endl <<

    "    --nt, --noTrace" << endl <<
    "          Don't generate a trace of the activity to standard error." << endl <<
    endl <<
    "    --d, --debug " << endl <<
    "          Generate a trace of the activity and print additional" << endl <<
    "          debugging information to standard error." << endl <<
    "    --dd, --debugDebug " << endl <<
    "          Same as above, but print even more debugging information." << endl <<
    endl <<
    "    --fd, --forceDebug " << endl <<
    "          Force debugging information to be printed at some places." << endl <<
    "          This is less invasive that debugging everything." << endl <<
    endl <<
    "    --dm, --debugMeasures measureNumbersSet" << endl <<
    "          'measureNumbersSet' has a form such as '0,2-14,^8-10'," << endl <<
    "          where '^' excludes the corresponding numbers interval" << endl <<
    "          and 0 applies to the '<part-list>' and anacrusis if present." <<endl <<
    "          Generate a trace of the activity and print additional" << endl <<
    "          debugging information to standard error for the specified measures." << endl <<
    "    --ddm, --debugDebugMeasures <measuresSpec>" << endl <<
    "          Same as above, but print even more debugging information." << endl <<
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

    "    --part, --partName 'originalName = newName'" << endl <<
    "    --part, --partName \"originalName = newName\"" << endl <<
    "          Rename part 'original' to 'newName', for example after " << endl <<
    "          displaying a summary of the score in a first xml2lilypond run." << endl <<
    "          There can be several occurrences of this option." << endl <<
    endl <<

    "  LPSR:" << endl <<
    endl <<

    "    --lpsr, --displayLPSR" << endl <<
    "          Write the contents of the LPSR data to standard error." << endl <<
    endl <<

    "    --abs, --absolute" << endl <<
    "          Generate LilyPond absolute note octaves. " << endl <<
    "          By default, relative octaves are generated." << endl <<
    endl <<

    "    --noBreaks, --dontKeepLineBreaks" << endl <<
    "          Don't keep the line breaks from the MusicXML input" << endl <<
    "          and let LilyPond decide about them." << endl <<
    endl <<

    "    --numericalTime" << endl <<
    "          Don't generate non-numerical time signatures such as 'C'." << endl <<
    "    --comments" << endl <<
    "          Generate comments showing the structure of the score" << endl <<
    "          such as '% part P_POne (P1)'." << endl <<
    "    --noStems" << endl <<
    "          Don't generate \\stemUp nor \\stemDown LilyPond commands." << endl <<
    "    --noAutoBeaming" << endl <<
    "          Generate '\\set Voice.autoBeaming = ##f' in each voice " << endl <<
    "          to prevent LilyPond from handling beams automatically." << endl <<
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
void analyzeOptions (
  int            argc,
  char*          argv[],
  S_msrOptions&  msrOpts, 
  S_lpsrOptions& lpsrOpts,
  string&        inputFileName,
  string&        outputFileName)
{
  // General options
  // ---------------
  
  msrOpts->fInteractive                       = false;
  
  msrOpts->fTrace                             = true;
  
  msrOpts->fDebug                             = false;
  msrOpts->fDebugDebug                        = false;
  
  msrOpts->fForceDebug                        = false;

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

  lpsrOpts->fDisplayLPSR                      = false;

  lpsrOpts->fDontKeepLineBreaks               = false;
  lpsrOpts->fKeepStaffSize                    = false;
    
  lpsrOpts->fGenerateAbsoluteOctaves          = false;

  lpsrOpts->fGenerateNumericalTime            = false;
  lpsrOpts->fGenerateComments                 = false;
  lpsrOpts->fGenerateStems                    = false;
  lpsrOpts->fNoAutoBeaming                    = false;
  lpsrOpts->fGeneratePositions                = false;

  lpsrOpts->fDontDisplayLilyPondCode          = false;


  // General options
  // ---------------

  int helpPresent                       = 0;

  int noTracePresent                    = 0;
  
  int outputFilePresent                 = 0;
  int interactivePresent                = 0;
  
  int debugPresent                      = 0;
  int debugDebugPresent                 = 0;
  int forceDebugPresent                 = 0;
  
  int debugMeasuresPresent              = 0;
  int debugdebugMeasuresPresent         = 0;
  
  // MSR options
  // -----------

  int languagePresent                   = 0;

  int staffRelativeVoiceNumbersPresent  = 0;
  
  int dontDisplayMSRLyricsPresent       = 0;

  int delayRestsDynamicsPresent         = 0;
  
  int displayMSRPresent                 = 0;
  
  int displayMSRScoreSummaryPresent     = 0;
  
  int partNamePresent                   = 0;
  
  // LPSR options
  // ------------

  int displayLPSRPresent                = 0;

  int fDontKeepLineBreaksPresent        = 0;
//  int fKeepStaffSizePresent             = 0; JMI
  
  int absolutePresent                   = 0;
  
  int numericaltimePresent              = 0;
  int commentsPresent                   = 0;
  int stemsPresent                      = 0;
  int noAutoBeamingPresent              = 0;
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
      "of",
      required_argument,
      &outputFilePresent, 1
    },
    {
      "outputFile",
      required_argument,
      &outputFilePresent, 1
    },
    {
      "i",
      no_argument,
      &interactivePresent, 1
    },
    {
      "interactive",
      no_argument,
      &interactivePresent, 1
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
      "df",
      no_argument,
      &forceDebugPresent, 1
    },
    {
      "forceDebug",
      no_argument,
      &forceDebugPresent, 1
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
    {
      "ddm",
      required_argument,
      &debugdebugMeasuresPresent, 1
    },
    {
      "debugDebugMeasures",
      required_argument,
      &debugdebugMeasuresPresent, 1
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

    {
      "part",
      required_argument,
      &partNamePresent, 1
    },
    {
      "partName",
      required_argument,
      &partNamePresent, 1
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
      "noBreaks",
      no_argument,
      &fDontKeepLineBreaksPresent, 1
    },
    {
      "fDontKeepLineBreaks",
      no_argument,
      &fDontKeepLineBreaksPresent, 1
    },
    
    {
      "numericalTime",
      no_argument,
      &numericaltimePresent, 1
    },
    {
      "comments",
      no_argument,
      &commentsPresent, 1
    },
    {
      "stems",
      no_argument,
      &stemsPresent, 1
    },
    {
      "noAutoBeaming",
      no_argument,
      &noAutoBeamingPresent, 1
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
          printUsage (0);
          break;
        }

        if (outputFilePresent) {
          outputFileName = optarg;

          stringstream s;

          s <<
            "--outputFile" << " " << outputFileName;
          msrOpts->fCommandLineOptions += s.str();
          outputFilePresent = false;
        }

        if (interactivePresent) {
          msrOpts->fInteractive = false;
          msrOpts->fCommandLineOptions +=
            "--interactive ";
          interactivePresent = false;
        }
        
        if (noTracePresent) {
          msrOpts->fTrace = false;
          msrOpts->fCommandLineOptions +=
            "--noTrace ";
          noTracePresent = false;
        }
        
        if (debugPresent) {
          msrOpts->fTrace = true;
          msrOpts->fDebug = true;
          msrOpts->fCommandLineOptions +=
            "--debug ";
          debugPresent = false;
        }
        if (debugDebugPresent) {
          msrOpts->fTrace = true;
          msrOpts->fDebugDebug = true;
          msrOpts->fCommandLineOptions +=
            "--debugDebug ";
          debugDebugPresent = false;
        }
        if (forceDebugPresent) {
          msrOpts->fTrace = true;
          msrOpts->fForceDebug = true;
          msrOpts->fCommandLineOptions +=
            "--forceDebug ";
          forceDebugPresent = false;
        }
        
        if (debugMeasuresPresent) {
          msrOpts->fTrace = true;
          msrOpts->fDebug = true;
          
          char*        measuresSpec = optarg;
          stringstream s;

          s <<
            "--debugMeasures" << " " << measuresSpec << " ";
          msrOpts->fCommandLineOptions += s.str();
            
          msrOpts->fDebugMeasureNumbersSet =
            decipherNumbersSetSpecification (
              measuresSpec, false); // 'true' to debug it
          debugMeasuresPresent = false;
        }
        if (debugdebugMeasuresPresent) {
          msrOpts->fTrace = true;
          msrOpts->fDebugDebug = true;
          
          char*        measuresSpec = optarg;
          stringstream s;

          s <<
            "--debugDebugMeasures" << " " << measuresSpec;
          msrOpts->fCommandLineOptions += s.str();
            
          msrOpts->fDebugMeasureNumbersSet =
            decipherNumbersSetSpecification (
              measuresSpec, false); // 'true' to debug it
          debugdebugMeasuresPresent = false;
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
              "\", using \"dutch\" instead" << endl;
            msrOpts->fMsrNoteNamesLanguageAsString = "dutch";
            msrOpts->fMsrNoteNamesLanguage = kNederlands;
          }
          msrOpts->fCommandLineOptions +=
            "--language " +
            msrOpts->fMsrNoteNamesLanguageAsString +
            " ";
          languagePresent = false;
          }
             
        if (staffRelativeVoiceNumbersPresent) {
          msrOpts->fCreateStaffRelativeVoiceNumbers = true;
          msrOpts->fCommandLineOptions +=
            "--staffRelativeVoiceNumbers ";
          staffRelativeVoiceNumbersPresent = false;
        }
        
        if (dontDisplayMSRLyricsPresent) {
          msrOpts->fDontDisplayMSRLyrics = true;
          msrOpts->fCommandLineOptions +=
            "--dontGenerateLyrics ";
          dontDisplayMSRLyricsPresent = false;
        }
        
        if (delayRestsDynamicsPresent) {
          msrOpts->fDelayRestsDynamics = true;
          msrOpts->fCommandLineOptions +=
            "--delayRestsDynamics ";
          delayRestsDynamicsPresent = false;
        }
        
        if (displayMSRPresent) {
          msrOpts->fDisplayMSR = true;
          msrOpts->fCommandLineOptions +=
            "--displayMSR ";
          displayMSRPresent = false;
        }

        if (displayMSRScoreSummaryPresent) {
          msrOpts->fDisplayMSRScoreSummary = true;
          msrOpts->fCommandLineOptions +=
            "--displayScoreSummary ";
          displayMSRScoreSummaryPresent = false;
        }
        
        if (partNamePresent) {
          char*        partNameSpec = optarg;
          stringstream s;

          s <<
            "--partName" << " \"" << partNameSpec << "\" ";
          msrOpts->fCommandLineOptions += s.str();

          std::pair<string, string>
            pair =
              extractNamesPairFromString (
                partNameSpec,
                '=',
                false); // 'true' to debug it
          /*
          cout <<
            "--> pair.first = \"" << pair.first << "\", " <<
            "--> pair.second = \"" << pair.second << "\"" <<
            endl;
          */

          // is this part name in the part renaming map?
          map<string, string>::iterator
            it =
              msrOpts->fPartsRenaming.find (pair.first);
                
          if (it != msrOpts->fPartsRenaming.end ()) {
            // yes, issue error message
          }
          else
            msrOpts->fPartsRenaming [pair.first] =
              pair.second;

          partNamePresent = false;
        }
        
        // LPSR options
        // ------------

        if (displayLPSRPresent) {
          lpsrOpts->fDisplayLPSR = true;
          msrOpts->fCommandLineOptions +=
            "--displayLPSR ";
          displayLPSRPresent = false;
        }

        if (absolutePresent) {
          lpsrOpts->fGenerateAbsoluteOctaves = true;
          msrOpts->fCommandLineOptions +=
            "--absolute ";
          absolutePresent = false;
        }

        if (fDontKeepLineBreaksPresent) {
          lpsrOpts->fDontKeepLineBreaks = true;
          msrOpts->fCommandLineOptions +=
            "--dontKeepLineBreaks ";
          fDontKeepLineBreaksPresent = false;
        }

        if (numericaltimePresent) {
          lpsrOpts->fGenerateNumericalTime = true;
          msrOpts->fCommandLineOptions +=
            "--numericalTime ";
        }
        if (commentsPresent) {
          lpsrOpts->fGenerateComments = true;
          msrOpts->fCommandLineOptions +=
            "--comments ";
          numericaltimePresent = false;
        }
        if (stemsPresent) {
          lpsrOpts->fGenerateStems = true;
          msrOpts->fCommandLineOptions +=
            "--stems ";
        }
        if (noAutoBeamingPresent) {
          lpsrOpts->fNoAutoBeaming = true;
          msrOpts->fCommandLineOptions +=
            "--noAutoBeaming ";
        }
        if (positionsPresent) {
          lpsrOpts->fGeneratePositions = true;
          msrOpts->fCommandLineOptions +=
            "--positions ";
          positionsPresent = false;
        }
        
        if (dontGenerateLilyPondLyricsPresent) {
          lpsrOpts->fDontGenerateLilyPondLyrics = true;
          msrOpts->fCommandLineOptions +=
            "--dontGenerateLyrics ";
          dontGenerateLilyPondLyricsPresent = false;
        }
        
        if (dontGenerateLilyPondLyricsPresent) {
          lpsrOpts->fDontDisplayLilyPondCode = true;
          msrOpts->fCommandLineOptions +=
            "--dontDisplayLilyPondCode ";
          dontGenerateLilyPondLyricsPresent = false;
        }

        }
        break;
        
      default:
        printUsage (1);
        break;
      } // switch
    } // while
 
  int nonOptionArgsNumber = argc-optind;

  switch (nonOptionArgsNumber)
    {
    case 1 :
      inputFileName = argv [optind];
      break;

    default:
      printUsage (1);
      break;
    } //  switch
}

//_______________________________________________________________________________
void printOptions (
  S_msrOptions&  msrOpts, 
  S_lpsrOptions& lpsrOpts )
{
  if (msrOpts->fTrace)
    cerr << idtr <<
      "The command line options and arguments have been analyzed" <<
      endl;

  idtr++;
  
  cerr << idtr <<
    "The command line options are: ";
    
  if (msrOpts->fCommandLineOptions.size()) {
    idtr++;
    cerr <<
      endl <<
      idtr << msrOpts->fCommandLineOptions;
    idtr--;
  }
  else
    cerr << "none";
  cerr << endl;

  // the option name field width
  int const fieldWidth = 31;
  
  // General options
  // ---------------

  cerr << idtr <<
    "The general options are:" <<
    endl;

  idtr++;

  cerr << left <<
    idtr << setw(fieldWidth) << "input source name" << " : " <<
      msrOpts->fInputSourceName << endl <<
      
    idtr << setw(fieldWidth) << "translation date" << " : " <<
      msrOpts->fTranslationDate << endl <<
      
    idtr << setw(fieldWidth) << "interactive" << " : " <<
      string(msrOpts->fInteractive
        ? "true" : "false") << endl <<
        
    idtr << setw(fieldWidth) << "trace" << " : " <<
      string(msrOpts->fTrace
        ? "true" : "false") << endl <<
        
    idtr << setw(fieldWidth) << "debug" << " : " <<
      string(msrOpts->fDebug
        ? "true" : "false") << endl <<
    idtr << setw(fieldWidth) << "debugDebug" << " : " <<
      string(msrOpts->fDebugDebug
        ? "true" : "false") << endl <<
    idtr << setw(fieldWidth) << "forceDebug" << " : " <<
      string(msrOpts->fForceDebug
        ? "true" : "false") << endl <<
    idtr << setw(fieldWidth) << "debugMeasureNumbersSet" << " : ";

  if (msrOpts->fDebugMeasureNumbersSet.empty ())
    cerr << "none";
  else
    for (
      set<int>::const_iterator i =
        msrOpts->fDebugMeasureNumbersSet.begin();
      i != msrOpts->fDebugMeasureNumbersSet.end();
      i++) {
        cerr << (*i) << " ";
    } // for
  
  cerr << endl;
  
  idtr--;

  // MSR options
  // -----------

  cerr << idtr <<
    "The MSR options are:" <<
    endl;

  idtr++;
  
  cerr << left <<
    idtr << setw(fieldWidth) << "noteNamesLanguageName" << " : \"" <<
      msrOpts->fMsrNoteNamesLanguageAsString << "\"" << endl <<
    
    idtr << setw(fieldWidth) << "createStaffRelativeVoiceNumbers" << " : " <<
      string(msrOpts->fCreateStaffRelativeVoiceNumbers
        ? "true" : "false") << endl <<

    idtr << setw(fieldWidth) << "dontDisplayMSRLyrics" << " : " <<
      string(msrOpts->fDontDisplayMSRLyrics
        ? "true" : "false") << endl <<

    idtr << setw(fieldWidth) << "delayRestsDynamics" << " : " <<
      string(msrOpts->fDelayRestsDynamics
        ? "true" : "false") << endl <<

    idtr << setw(fieldWidth) << "displayMSR" << " : " <<
      string(msrOpts->fDisplayMSR
        ? "true" : "false") << endl <<
    
    idtr << setw(fieldWidth) << "displayMSRScoreSummary" << " : " <<
      string(msrOpts->fDisplayMSRScoreSummary
        ? "true" : "false") << endl <<
    
    idtr << setw(fieldWidth) << "partRenamingSpecifications" << " : ";
    
  if (msrOpts->fPartsRenaming.empty ())
    cerr << "none";
  else
    for (
      map<string, string>::const_iterator i =
        msrOpts->fPartsRenaming.begin();
      i != msrOpts->fPartsRenaming.end();
      i++) {
        cerr << "\"" << ((*i).first) << " = " << ((*i).second) << "\" ";
    } // for
  
  cerr << endl;

  idtr--;

  // LPSR options
  // ------------

  cerr << idtr <<
    "The LPSR options are:" <<
    endl;

  idtr++;
  
  cerr << left <<
    idtr << setw(fieldWidth) << "displayLPSR" << " : " <<
      string(lpsrOpts->fDisplayLPSR
        ? "true" : "false") << endl <<

    idtr << setw(fieldWidth) << "generateAbsoluteOctaves" << " : " <<
      string(lpsrOpts->fGenerateAbsoluteOctaves
        ? "true" : "false") << endl <<
    
    idtr << setw(fieldWidth) << "dontKeepLineBreaks" << " : " <<
      string(lpsrOpts->fDontKeepLineBreaks
        ? "true" : "false") << endl <<
    
    idtr << setw(fieldWidth) << "generateNumericalTime" << " : " <<
      string(lpsrOpts->fGenerateNumericalTime
        ? "true" : "false") << endl <<
    idtr << setw(fieldWidth) << "generateComments" << " : " <<
      string(lpsrOpts->fGenerateComments
      ? "true" : "false") << endl <<
    idtr << setw(fieldWidth) << "generateStems" << " : " <<
      string(lpsrOpts->fGenerateStems
      ? "true" : "false") << endl <<
    idtr << setw(fieldWidth) << "noAutoBeaming" << " : " <<
      string(lpsrOpts->fNoAutoBeaming
      ? "true" : "false") << endl <<
//      setw(fieldWidth) << "generatePositions" << " : " <<
//        string(lpsrOpts->fGeneratePositions
//        ? "true" : "false") << endl <<

    idtr << setw(fieldWidth) << "dontGenerateLilyPondLyrics" << " : " <<
      string(lpsrOpts->fDontGenerateLilyPondLyrics
        ? "true" : "false") << endl <<

    idtr << setw(fieldWidth) << "dontDisplayLilyPondCode" << " : " <<
      string(lpsrOpts->fDontDisplayLilyPondCode
      ? "true" : "false") << endl;

  idtr--;

  idtr--;
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

  // analyze the command line options
  // ------------------------------------------------------

  S_msrOptions msrOpts = msrOptions::create ();
  assert(msrOpts != 0);

  S_lpsrOptions lpsrOpts = lpsrOptions::create();
  assert(lpsrOpts != 0);
  
  string    inputFileName;
  string    outputFileName;
  
  analyzeOptions (
    argc, argv,
    msrOpts, lpsrOpts,
    inputFileName, outputFileName);

  // input source name
  // ------------------------------------------------------

  if (inputFileName == "-")
    msrOpts->fInputSourceName = "standard input";
  else
    msrOpts->fInputSourceName = "\"" + inputFileName + "\"";
  
  // translation date
  // ------------------------------------------------------

  time_t      rawtime;
  struct tm*  timeinfo;
  char buffer [80];

  time (&rawtime);
  timeinfo = localtime (&rawtime);

  strftime (buffer, 80, "%A %F @ %T %Z", timeinfo);
  msrOpts->fTranslationDate = buffer;
  
  // trace
  // ------------------------------------------------------

  if (msrOpts->fTrace) {
    cerr <<  idtr <<
      "This is xml2Lilypond v" << musicxml2MsrVersionStr() << 
      " from libmusicxml2 v" << musicxmllibVersionStr() <<
      endl;

    cerr <<  idtr <<
      "Launching conversion of " <<
      msrOpts->fInputSourceName <<
      " to LilyPond" <<
      endl;

    cerr << idtr <<
      "LilyPond code will be written to ";
    if (outputFileName.size())
      cerr << outputFileName;
    else
      cerr << "standard output";
    cerr << endl;
    
    printOptions (msrOpts, lpsrOpts);
  }
    
  // open output file if need be
  // ------------------------------------------------------

  ofstream outStream;

  if (outputFileName.size()) {
    if (msrOpts->fDebug)
      cerr << idtr <<
        "Opening file '" << outputFileName << "' for writing" << endl;
        
    outStream.open (outputFileName.c_str(), ofstream::out);
  }
      
  // create MSR from MusicXML contents
  // ------------------------------------------------------

  S_msrScore mScore;

  if (inputFileName == "-") {
    // input comes from standard input
    if (outputFileName.size())
      mScore =
        musicxmlFd2Msr (stdin, msrOpts, outStream);
    else
      mScore =
        musicxmlFd2Msr (stdin, msrOpts, cout);
  }
  
  else {
    // input comes from a file
    if (outputFileName.size())
      mScore =
        musicxmlFile2Msr (
          inputFileName.c_str(), msrOpts, outStream);
    else
      mScore =
        musicxmlFile2Msr (
          inputFileName.c_str(), msrOpts, cout);
  }
    
  if (! mScore) {
    cerr <<
      "### Conversion from MusicCML to MSR failed ###" << endl <<
      endl;
    return 1;
  }

  // create LPSR from MSR
  // ------------------------------------------------------

  S_lpsrScore lpScore;
        
  if (outputFileName.size())
    lpScore =
      msr2Lpsr (mScore, msrOpts, lpsrOpts, outStream);
  else
    lpScore =
      msr2Lpsr (mScore, msrOpts, lpsrOpts, cout);
  
  if (! lpScore) {
    cerr <<
      "### Conversion from MSR to LPSR failed ###" << endl <<
      endl;
    return 1;
  }

  // generate LilyPond code from LPSR
  // ------------------------------------------------------

  if (outputFileName.size())
    lpsr2LilyPond (lpScore, msrOpts, lpsrOpts, outStream);
  else
    lpsr2LilyPond (lpScore, msrOpts, lpsrOpts, cout);
  
  if (outputFileName.size()) {
    if (msrOpts->fDebug)
      cerr << idtr <<
        "Closing file '" << outputFileName << "'" << endl;
        
    outStream.close ();
  }
  
  if (! true) { // JMI
    cerr <<
      "### Conversion from LPSR to LilyPond code failed ###" << endl <<
      endl;
    return 1;
  }

  return 0;
}
