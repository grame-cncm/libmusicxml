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

//#include <libgen.h>       // pour basename
//#include <string>       // pour basename

#include <iomanip>      // setw, set::precision, ...

#include <fstream>      // ofstream, ofstream::open(), ofstream::close()

#include <regex>

#include "msrVersion.h"
#include "msrUtilities.h"

#include "xml2MsrInterface.h"

#include "msr2LpsrInterface.h"

#include "lpsr2LilyPondInterface.h"


using namespace std;
using namespace MusicXML2;

enum msrHelpKind {
  kAllHelp,
  kGeneralHelp, kMsrHelp, kLpsrHelp};
  
//_______________________________________________________________________________
void printUsage (
  msrHelpKind helpKind,
  int         exitStatus)
{
  cerr <<
    endl <<
    "                Welcome to xml2lilypond v" << currentVersionNumber () << ", " << endl <<
    "              the MusicXML to LilyPond translator" << endl <<
    "          delivered as part of the libmusicxml2 library." << endl <<
    "      https://github.com/grame-cncm/libmusicxml/tree/lilypond" << endl <<
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
    "    There are various options to fine tune the generated LilyPond code." << endl <<
    endl <<
    "    The activity log and warning/error messages go to standard error." << endl <<
    endl;

  // print versions history
  printVersionsHistory (cerr);
    
  cerr <<
    "Options:" <<
    endl <<
    endl;

  // print options help
  switch (helpKind) {
    kAllHelp:
      gGeneralOptions->printGeneralOptionsHelp ();
      gMsrOptions->printMsrOptionsHelp ();
      gLpsrOptions->printLpsrOptionsHelp ();
      break;
      
    kGeneralHelp:
      gGeneralOptions->printGeneralOptionsHelp ();
      break;
      
    kMsrHelp:
      gMsrOptions->printMsrOptionsHelp ();
      break;
      
    kLpsrHelp:
      gLpsrOptions->printLpsrOptionsHelp ();
      break;
  } // switch
  
  cerr <<
    endl;

  // over!
  exit (exitStatus);
}

//_______________________________________________________________________________
void analyzeOptions (
  int            argc,
  char*          argv[],
  string&        inputFileName,
  string&        outputFileName)
{
  // General options
  // ---------------

  int versionPresent                    = 0;

  int helpPresent                       = 0;
  int helpGeneralPresent                = 0;
  int helpMsrPresent                    = 0;
  int helpLpsrPresent                   = 0;

  int outputFilePresent                 = 0;
  int autoOutputFilePresent             = 0;
    
  int traceGeneralPresent               = 0;
  
  int traceDivisionsPresent             = 0;
  
  int tracePartsPresent                 = 0;
  
  int traceVoicesPresent                = 0;
  int traceSegmentsPresent              = 0;
  int traceRepeatsPresent               = 0;
  int traceMeasuresPresent              = 0;
  int traceNotesPresent                 = 0;
  int traceChordsPresent                = 0;
  int traceTupletsPresent               = 0;
  
  int traceGracenotesPresent            = 0;
  
  int traceLyricsPresent                = 0;
  
  int traceHarmoniesPresent             = 0;
  
  int debugMeasuresPresent              = 0;
  int debugdebugMeasuresPresent         = 0;
  
  int displayCPUusagePresent            = 0;
  
  // MSR options
  // -----------

  int traceMSRVisitorsPresent           = 0;

  int msrPitchesLanguagePresent         = 0;

  int staffRelativeVoiceNumbersPresent  = 0;
  
  int dontDisplayMsrStanzasPresent      = 0;

  int delayRestsDynamicsPresent         = 0;
  int delayRestsWordsPresent            = 0;
  int delayRestsSlursPresent            = 0;
  int delayRestsLigaturesPresent        = 0;
  int delayRestsWedgesPresent           = 0;
  
  int keepMasterVoicesPresent           = 0;
  int keepMasterStanzasPresent          = 0;
  
  int displayMSRPresent                 = 0;
  
  int displayMSRSummaryPresent          = 0;
  
  int partNamePresent                   = 0;
  
  // LPSR options
  // ------------

  int traceLPSRVisitorsPresent          = 0;

  int lpsrPitchesLanguagePresent        = 0;
  int lpsrChordsLanguagePresent         = 0;
  
  int displayLPSRPresent                = 0;

  int dontKeepLineBreaksPresent         = 0;
  int showAllBarNumbersPresent          = 0;
  int compressFullBarRestsPresent       = 0;

  int tupletsOnALinePresent             = 0;
  
  int breakLinesAtIncompleteRightMeasuresPresent  = 0;
  int separatorLineEveryNMeasuresPresent = 0;
//  int fKeepStaffSizePresent             = 0; JMI
  
  int absolutePresent                   = 0;
  
  int numericaltimePresent              = 0;
  int commentsPresent                   = 0;
  int stemsPresent                      = 0;
  int noAutoBeamingPresent              = 0;
  int noteInputLineNumbersPresent       = 0;
  
  int modernTabPresent                  = 0;
  
  int accidentalStylePresent            = 0;
  
  int delayedOrnamentFractionPresent    = 0;
  
  int midiTempoPresent                  = 0;

  int dontGenerateLilyPondLyricsPresent = 0;

  int dontGenerateLilyPondCodePresent   = 0;


  // long_options data structure
  static struct option long_options [] =
    {
    /* These options set a flag. */

    // General options
    // ---------------

    {
      "v",
      no_argument, &versionPresent, 1
    },
    {
      "version",
      no_argument, &versionPresent, 1
    },
    
    {
      "h",
      no_argument, &helpPresent, 1
    },
    {
      "help",
      no_argument, &helpPresent, 1
    },
    
    {
      "hg",
      no_argument, &helpGeneralPresent, 1
    },
    {
      "helpGeneral",
      no_argument, &helpGeneralPresent, 1
    },
    {
      "hm",
      no_argument, &helpMsrPresent, 1
    },
    {
      "helpMsr",
      no_argument, &helpMsrPresent, 1
    },
    {
      "hlp",
      no_argument, &helpLpsrPresent, 1
    },
    {
      "helpLpsr",
      no_argument, &helpLpsrPresent, 1
    },
    
    {
      "of",
      required_argument, &outputFilePresent, 1
    },
    {
      "outputFile",
      required_argument, &outputFilePresent, 1
    },
    
    {
      "aof",
      no_argument, &autoOutputFilePresent, 1
    },
    {
      "autoOutputFile",
      no_argument, &autoOutputFilePresent, 1
    },
    
    {
      "t",
      no_argument, &traceGeneralPresent, 1
    },
    {
      "traceGeneral",
      no_argument, &traceGeneralPresent, 1
    },
    
    {
      "tdivs",
      no_argument, &traceDivisionsPresent, 1
    },
    {
      "traceDivisions",
      no_argument, &traceDivisionsPresent, 1
    },
    
    {
      "tparts",
      no_argument, &tracePartsPresent, 1
    },
    {
      "traceparts",
      no_argument, &tracePartsPresent, 1
    },
    
    {
      "tvoices",
      no_argument, &traceVoicesPresent, 1
    },
    {
      "traceVoices",
      no_argument, &traceVoicesPresent, 1
    },
    
    {
      "tsegs",
      no_argument, &traceSegmentsPresent, 1
    },
    {
      "traceSegments",
      no_argument, &traceSegmentsPresent, 1
    },
    
    {
      "trepeats",
      no_argument, &traceRepeatsPresent, 1
    },
    {
      "traceRepeats",
      no_argument, &traceRepeatsPresent, 1
    },
    
    {
      "tmeas",
      no_argument, &traceMeasuresPresent, 1
    },
    {
      "traceMeasures",
      no_argument, &traceMeasuresPresent, 1
    },
    
    {
      "tnotes",
      no_argument, &traceNotesPresent, 1
    },
    {
      "traceNotes",
      no_argument, &traceNotesPresent, 1
    },
    
    {
      "tchords",
      no_argument, &traceChordsPresent, 1
    },
    {
      "traceChords",
      no_argument, &traceChordsPresent, 1
    },
    
    {
      "ttuplets",
      no_argument, &traceTupletsPresent, 1
    },
    {
      "traceTuplets",
      no_argument, &traceTupletsPresent, 1
    },
    
    {
      "tgrace",
      no_argument, &traceGracenotesPresent, 1
    },
    {
      "traceGracenotes",
      no_argument, &traceGracenotesPresent, 1
    },
    
    {
      "tlyrics",
      no_argument, &traceLyricsPresent, 1
    },
    {
      "traceLyrics",
      no_argument, &traceLyricsPresent, 1
    },
    
    {
      "tharm",
      no_argument, &traceHarmoniesPresent, 1
    },
    {
      "traceHarmonies",
      no_argument, &traceHarmoniesPresent, 1
    },
    
    {
      "dm",
      required_argument, &debugMeasuresPresent, 1
    },
    {
      "debugMeasures",
      required_argument, &debugMeasuresPresent, 1
    },
    {
      "ddm",
      required_argument, &debugdebugMeasuresPresent, 1
    },
    {
      "debugDebugMeasures",
      required_argument, &debugdebugMeasuresPresent, 1
    },
    
    {
      "dCPU",
      required_argument, &displayCPUusagePresent, 1
    },
    {
      "displayCPUusage",
      required_argument, &displayCPUusagePresent, 1
    },

    // MSR options
    // -----------

    {
      "tmvisit",
      no_argument, &traceMSRVisitorsPresent, 1
    },
    {
      "traceMSRVisitors",
      no_argument, &traceMSRVisitorsPresent, 1
    },

    {
      "mpl",
      required_argument, &msrPitchesLanguagePresent, 1
    },
    {
      "msrPitchesLanguage",
      required_argument, &msrPitchesLanguagePresent, 1
    },
    
    {
      "srvn",
      no_argument,
      &staffRelativeVoiceNumbersPresent, 1
    },
    {
      "staffRelativeVoiceNumbers",
      no_argument, &staffRelativeVoiceNumbersPresent, 1
    },
    
    {
      "noms",
      no_argument, &dontDisplayMsrStanzasPresent, 1
    },
    {
      "dontDisplayMsrStanzas",
      no_argument, &dontDisplayMsrStanzasPresent, 1
    },

    {
      "drdyn",
      no_argument, &delayRestsDynamicsPresent, 1
    },
    {
      "delayRestsDynamics",
      no_argument, &delayRestsDynamicsPresent, 1
    },
    {
      "drwords",
      no_argument, &delayRestsWordsPresent, 1
    },
    {
      "delayRestsWords",
      no_argument, &delayRestsWordsPresent, 1
    },
    {
      "drslurs",
      no_argument, &delayRestsSlursPresent, 1
    },
    {
      "delayRestsSlurs",
      no_argument, &delayRestsSlursPresent, 1
    },
    {
      "drligs",
      no_argument, &delayRestsLigaturesPresent, 1
    },
    {
      "delayRestsLigatures",
      no_argument, &delayRestsLigaturesPresent, 1
    },
    {
      "drwedges",
      no_argument, &delayRestsWedgesPresent, 1
    },
    {
      "delayRestsWedges",
      no_argument, &delayRestsWedgesPresent, 1
    },
   
    {
      "msr",
      no_argument, &displayMSRPresent, 1},
    {
      "displayMSR",
      no_argument, &displayMSRPresent, 1
    },

    {
      "kmv",
      no_argument, &keepMasterVoicesPresent, 1},
    {
      "keepMasterVoices",
      no_argument, &keepMasterVoicesPresent, 1
    },

    {
      "kms",
      no_argument, &keepMasterStanzasPresent, 1},
    {
      "keepMasterStanzas",
      no_argument, &keepMasterStanzasPresent, 1
    },

    {
      "sum",
      no_argument, &displayMSRSummaryPresent, 1
    },
    {
      "displayMSRSummary",
      no_argument, &displayMSRSummaryPresent, 1
    },

    {
      "part",
      required_argument, &partNamePresent, 1
    },
    {
      "partName",
      required_argument, &partNamePresent, 1
    },

    // LPSR options
    // ------------

    {
      "tlpvisit",
      no_argument, &traceLPSRVisitorsPresent, 1
    },
    {
      "traceLPSRVisitors",
      no_argument, &traceLPSRVisitorsPresent, 1
    },

    {
      "lppl",
      required_argument, &lpsrPitchesLanguagePresent, 1
    },
    {
      "lpsrPitchesLanguage",
      required_argument, &lpsrPitchesLanguagePresent, 1
    },
    
    {
      "lpcl",
      required_argument, &lpsrChordsLanguagePresent, 1
    },
    {
      "lpsrChordsLanguage",
      required_argument, &lpsrChordsLanguagePresent, 1
    },
    
    {
      "lpsr",
      no_argument, &displayLPSRPresent, 1},
    {
      "displayLPSR",
      no_argument, &displayLPSRPresent, 1
    },

    {
      "abs",
      no_argument, &absolutePresent, 1
    },
    {
      "absolute",
      no_argument, &absolutePresent, 1
    },
    
    {
      "sabn",
      no_argument, &showAllBarNumbersPresent, 1
    },
    {
      "showAllBarNumbers",
      no_argument, &showAllBarNumbersPresent, 1
    },
    
    {
      "cfbr",
      no_argument, &compressFullBarRestsPresent, 1
    },
    {
      "compressFullBarRests",
      no_argument, &compressFullBarRestsPresent, 1
    },
    
    {
      "toal",
      no_argument, &tupletsOnALinePresent, 1
    },
    {
      "tupletsOnALine",
      no_argument, &tupletsOnALinePresent, 1
    },
    
    {
      "blairm",
      no_argument, &breakLinesAtIncompleteRightMeasuresPresent, 1
    },
    {
      "breakLinesAtIncompleteRightMeasures",
      no_argument, &breakLinesAtIncompleteRightMeasuresPresent, 1
    },
    
    {
      "slenm",
      required_argument, &separatorLineEveryNMeasuresPresent, 1
    },
    {
      "separatorLineEveryNMeasures",
      required_argument, &separatorLineEveryNMeasuresPresent, 1
    },
    
    {
      "noBreaks",
      no_argument, &dontKeepLineBreaksPresent, 1
    },
    {
      "dontKeepLineBreaks",
      no_argument, &dontKeepLineBreaksPresent, 1
    },
    
    {
      "numericalTime",
      no_argument, &numericaltimePresent, 1
    },
    
    {
      "com",
      no_argument, &commentsPresent, 1
    },
    {
      "comments",
      no_argument, &commentsPresent, 1
    },
    
    {
      "stems",
      no_argument, &stemsPresent, 1
    },
    
    {
      "noab",
      no_argument, &noAutoBeamingPresent, 1
    },
    {
      "noAutoBeaming",
      no_argument, &noAutoBeamingPresent, 1
    },
    
    {
      "iln",
      no_argument, &noteInputLineNumbersPresent, 1
    },
    {
      "generateInputLineNumbers",
      no_argument, &noteInputLineNumbersPresent, 1
    },    

    {
      "mtab",
      no_argument, &modernTabPresent, 1
    },
    {
      "modernTab",
      no_argument, &modernTabPresent, 1
    },
    
    {
      "as",
      required_argument, &accidentalStylePresent, 1
    },
    {
      "accidentalStyle",
      required_argument, &accidentalStylePresent, 1
    },
    
    {
      "dof",
      required_argument, &delayedOrnamentFractionPresent, 1
    },
    {
      "delayedOrnamentFraction",
      required_argument, &delayedOrnamentFractionPresent, 1
    },
    
    {
      "mt",
      required_argument, &midiTempoPresent, 1
    },
    {
      "midiTempo",
      required_argument, &midiTempoPresent, 1
    },
    
    {
      "nolpc",
      no_argument, &dontGenerateLilyPondCodePresent, 1
    },
    {
      "dontGenerateLilyPondCode",
      no_argument, &dontGenerateLilyPondCodePresent, 1
    },

    {
      "nolpl",
      no_argument, &dontGenerateLilyPondLyricsPresent, 1
    },
    {
      "dontGenerateLilyPondLyrics",
      no_argument, &dontGenerateLilyPondLyricsPresent, 1
    },

    {0, 0, 0, 0}
    };

  // initialize output file name
  outputFileName = "";
  
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
        if (versionPresent) {
          idtr++;
          
          cerr <<
            endl <<
            idtr <<
              "This is xml2lilypond version " <<
              currentVersionNumber () << "," <<
              endl <<
            idtr <<
              "an open translator of MusicXML into LilyPond." <<
              endl <<
              endl <<
            idtr <<
              "See https://github.com/dfober/libmusicxml/tree/lilypond" <<
              endl <<
            idtr <<
              "for more information and access to the source code." <<
              endl <<
            endl;

          idtr--;
          
          exit (0);
          break;
        }

        if (helpPresent) {
          printUsage (kAllHelp, 0);
          break;
        }

        if (helpGeneralPresent) {
          printUsage (kGeneralHelp, 0);
          break;
        }
        if (helpMsrPresent) {
          printUsage (kMsrHelp, 0);
          break;
        }
        if (helpLpsrPresent) {
          printUsage (kLpsrHelp, 0);
          break;
        }

        if (outputFilePresent) {
          outputFileName = optarg;
          gGeneralOptions->fOutputFileName = outputFileName;

          stringstream s;

          s <<
            "--outputFile" << " " << outputFileName << " ";
          gGeneralOptions->fCommandLineOptions +=
            s.str();
          outputFilePresent = false;
        }
        
        if (autoOutputFilePresent) {
          gGeneralOptions->fAutoOutputFile = true;
          gGeneralOptions->fCommandLineOptions +=
            "--autoOutputFile ";
          autoOutputFilePresent = false;
        }
        
        if (traceGeneralPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fCommandLineOptions +=
            "--traceGeneral ";
          traceGeneralPresent = false;
        }
        
        if (traceDivisionsPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceDivisions = true;
          gGeneralOptions->fCommandLineOptions +=
            "--traceDivisions ";
          traceDivisionsPresent = false;
        }
        
        if (tracePartsPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceParts = true;
          gGeneralOptions->fCommandLineOptions +=
            "--traceparts ";
          tracePartsPresent = false;
        }
        if (traceVoicesPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceVoices = true;
          gGeneralOptions->fCommandLineOptions +=
            "--traceVoices ";
          traceVoicesPresent = false;
        }
        if (traceSegmentsPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceSegments = true;
          gGeneralOptions->fCommandLineOptions +=
            "--traceSegments ";
          traceSegmentsPresent = false;
        }
        if (traceRepeatsPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceRepeats = true;
          gGeneralOptions->fCommandLineOptions +=
            "--traceRepeats ";
          traceRepeatsPresent = false;
        }
        if (traceMeasuresPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceMeasures = true;
          gGeneralOptions->fCommandLineOptions +=
            "--traceMeasures ";
          traceMeasuresPresent = false;
        }
        if (traceNotesPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceNotes = true;
          gGeneralOptions->fCommandLineOptions +=
            "--traceNotes ";
          traceNotesPresent = false;
        }
        if (traceChordsPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceChords = true;
          gGeneralOptions->fCommandLineOptions +=
            "--traceChords ";
          traceChordsPresent = false;
        }
        if (traceTupletsPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceTuplets = true;
          gGeneralOptions->fCommandLineOptions +=
            "--traceTuplets ";
          traceTupletsPresent = false;
        }
        if (traceGracenotesPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceGracenotes = true;
          gGeneralOptions->fCommandLineOptions +=
            "--traceGracenotes ";
          traceGracenotesPresent = false;
        }

        if (traceLyricsPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceLyrics = true;
          gGeneralOptions->fCommandLineOptions +=
            "--traceLyrics ";
          traceLyricsPresent = false;
        }
        
        if (traceHarmoniesPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceHarmonies = true;
          gGeneralOptions->fCommandLineOptions +=
            "--traceHarmonies ";
          traceHarmoniesPresent = false;
        }
        
        if (debugMeasuresPresent) {
          // optarg contains the measure numbers set specification
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceMeasures = true;
          
          char*        measuresSpec = optarg;
          stringstream s;

          s <<
            "--debugMeasures" << " " << measuresSpec << " ";
          gGeneralOptions->fCommandLineOptions +=
            s.str();
            
          gGeneralOptions->fDebugMeasureNumbersSet =
            decipherNumbersSetSpecification (
              measuresSpec, false); // 'true' to debug it
          debugMeasuresPresent = false;
        }
        if (debugdebugMeasuresPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fDebugDebug = true;
          
          char*        measuresSpec = optarg;
          stringstream s;

          s <<
            "--debugDebugMeasures" << " " << measuresSpec;
          gGeneralOptions->fCommandLineOptions +=
            s.str();
            
          gGeneralOptions->fDebugMeasureNumbersSet =
            decipherNumbersSetSpecification (
              measuresSpec, false); // 'true' to debug it
          debugdebugMeasuresPresent = false;
        }

        if (displayCPUusagePresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fDisplayCPUusage = true;
          gGeneralOptions->fCommandLineOptions +=
            "--displayCPUusage ";
          displayCPUusagePresent = false;
        }
        

        // MSR options
        // -----------
        
        if (traceMSRVisitorsPresent) {
          gMsrOptions->fTraceMsrVisitors = true;
          gGeneralOptions->fCommandLineOptions +=
            "--traceMSRVisitors ";
          traceMSRVisitorsPresent = false;
        }

        if (msrPitchesLanguagePresent) {
          // optarg contains the language name
          string optargAsString;
          {
            stringstream s;
            s << optarg;
            optargAsString = s.str();
          }
          
          if (! gMsrOptions->setMsrQuartertonesPitchesLanguage (
            optargAsString)) {
            stringstream s;
        
            s <<
              "MSR pitches language " << optargAsString <<
              " is unknown" <<
              endl <<
              "The " <<
              gQuatertonesPitchesLanguagesMap.size () <<
              " known MSR pitches languages are:" <<
              endl;
        
            idtr++;
          
            s <<
              existingQuartertonesPitchesLanguages ();
        
            idtr--;
        
            optionError (s.str());
          }
          
          gGeneralOptions->fCommandLineOptions +=
            "--msrPitchesLanguage " +
            optargAsString +
            " ";
          msrPitchesLanguagePresent = false;
          }
             
        if (staffRelativeVoiceNumbersPresent) {
          gMsrOptions->fCreateStaffRelativeVoiceNumbers = true;
          gGeneralOptions->fCommandLineOptions +=
            "--staffRelativeVoiceNumbers ";
          staffRelativeVoiceNumbersPresent = false;
        }
        
        if (dontDisplayMsrStanzasPresent) {
          gMsrOptions->fDontDisplayMsrStanzas = true;
          gGeneralOptions->fCommandLineOptions +=
            "--dontGenerateMSRStanzas ";
          dontDisplayMsrStanzasPresent = false;
        }
        
        if (delayRestsDynamicsPresent) {
          gMsrOptions->fDelayRestsDynamics = true;
          gGeneralOptions->fCommandLineOptions +=
            "--delayRestsDynamics ";
          delayRestsDynamicsPresent = false;
        }
        
        if (delayRestsWordsPresent) {
          gMsrOptions->fDelayRestsWords = true;
          gGeneralOptions->fCommandLineOptions +=
            "--delayRestsWords ";
          delayRestsWordsPresent = false;
        }
        
        if (delayRestsSlursPresent) {
          gMsrOptions->fDelayRestsSlurs = true;
          gGeneralOptions->fCommandLineOptions +=
            "--delayRestsSlurs ";
          delayRestsSlursPresent = false;
        }
        
        if (delayRestsLigaturesPresent) {
          gMsrOptions->fDelayRestsLigatures = true;
          gGeneralOptions->fCommandLineOptions +=
            "--delayRestsLigatures ";
          delayRestsLigaturesPresent = false;
        }
        
        if (delayRestsWedgesPresent) {
          gMsrOptions->fDelayRestsWedges = true;
          gGeneralOptions->fCommandLineOptions +=
            "--delayRestsWedges ";
          delayRestsWedgesPresent = false;
        }
        
        if (keepMasterVoicesPresent) {
          gMsrOptions->fKeepMasterVoices = true;
          gGeneralOptions->fCommandLineOptions +=
            "--keepMasterVoices ";
          keepMasterVoicesPresent = false;
        }
        if (keepMasterStanzasPresent) {
          gMsrOptions->fKeepMasterStanzas = true;
          gGeneralOptions->fCommandLineOptions +=
            "--keepMasterStanzas ";
          keepMasterStanzasPresent = false;
        }

        if (displayMSRPresent) {
          gMsrOptions->fDisplayMsr = true;
          gGeneralOptions->fCommandLineOptions +=
            "--displayMSR ";
          displayMSRPresent = false;
        }

        if (displayMSRSummaryPresent) {
          gMsrOptions->fDisplayMsrSummary = true;
          gLpsrOptions->fDontGenerateLilyPondCode = true;
          gGeneralOptions->fCommandLineOptions +=
            "--displayScoreSummary ";
          displayMSRSummaryPresent = false;
        }
        
        if (partNamePresent) {
          // optarg contains the part name
          char*        partNameSpec = optarg;
          stringstream s;

          s <<
            "--partName" << " \"" << partNameSpec << "\" ";

          gGeneralOptions->fCommandLineOptions +=
            s.str();

          std::pair<string, string>
            pair =
              extractNamesPairFromString (
                partNameSpec,
                '=',
                false); // 'true' to debug it
          /*
          cerr <<
            "--> pair.first = \"" << pair.first << "\", " <<
            "--> pair.second = \"" << pair.second << "\"" <<
            endl;
          */

          // is this part name in the part renaming map?
          map<string, string>::iterator
            it =
              gMsrOptions->fPartsRenaming.find (pair.first);
                
          if (it != gMsrOptions->fPartsRenaming.end ()) {
            // yes, issue error message
            stringstream s;

            s <<
              "Part \"" << pair.first << "\" occurs more that one" <<
              "in the '--partName' option";
              
            optionError (s.str());
          }
          else
            gMsrOptions->fPartsRenaming [pair.first] =
              pair.second;

          partNamePresent = false;
        }
        
        // LPSR options
        // ------------

        if (traceLPSRVisitorsPresent) {
          gLpsrOptions->fTraceLpsrVisitors = true;
          gGeneralOptions->fCommandLineOptions +=
            "--traceLPSRVisitors ";
          traceLPSRVisitorsPresent = false;
        }

        if (lpsrPitchesLanguagePresent) {
          // optarg contains the language name
          string optargAsString;
          {
            stringstream s;
            s << optarg;
            optargAsString = s.str();
          }
          
          if (! gLpsrOptions->setLpsrQuartertonesPitchesLanguage (
            optargAsString)) {
            stringstream s;

            s <<
              "LPSR pitches language name '" << optargAsString <<
              "' is unknown" <<
              endl <<
              "The " <<
              gQuatertonesPitchesLanguagesMap.size () <<
              " known LPSR pitches languages are:" <<
              endl;
        
            idtr++;
          
            s <<
              existingQuartertonesPitchesLanguages ();
        
            idtr--;
        
            optionError (s.str());
          }
          
          gGeneralOptions->fCommandLineOptions +=
            "--lpsrPitchesLanguage " +
            optargAsString +
            " ";
          lpsrPitchesLanguagePresent = false;
          }
             
        if (lpsrChordsLanguagePresent) {
          // optarg contains the language name
          string optargAsString;
          {
            stringstream s;
            s << optarg;
            optargAsString = s.str();
          }
          
          if (! gLpsrOptions->setLpsrChordsLanguage (
            optargAsString)) {
            stringstream s;

            s <<
              "LPSR chords language name '" << optargAsString <<
              "' is unknown" <<
              endl <<
              "The " <<
              gQuatertonesPitchesLanguagesMap.size () <<
              " known LPSR chords languages are:" <<
              endl;
        
            idtr++;
          
            s <<
              existingLpsrChordsLanguages ();
        
            idtr--;
              
            optionError (s.str());
          }
          
          gGeneralOptions->fCommandLineOptions +=
            "--lpsrChordsLanguage " +
            optargAsString +
            " ";
          lpsrChordsLanguagePresent = false;
          }
             
        if (displayLPSRPresent) {
          gLpsrOptions->fDisplayLpsr = true;
          gGeneralOptions->fCommandLineOptions +=
            "--displayLpsr ";
          displayLPSRPresent = false;
        }

        if (absolutePresent) {
          gLpsrOptions->fGenerateAbsoluteOctaves = true;
          gGeneralOptions->fCommandLineOptions +=
            "--absolute ";
          absolutePresent = false;
        }

        if (showAllBarNumbersPresent) {
          gLpsrOptions->fShowAllBarNumbers = true;
          gGeneralOptions->fCommandLineOptions +=
            "--showAllBarNumbers ";
          showAllBarNumbersPresent = false;
        }

        if (compressFullBarRestsPresent) {
          gLpsrOptions->fCompressFullBarRests = true;
          gGeneralOptions->fCommandLineOptions +=
            "--compressFullBarRests ";
          compressFullBarRestsPresent = false;
        }

        if (tupletsOnALinePresent) {
          gLpsrOptions->fTupletsOnALine = true;
          gGeneralOptions->fCommandLineOptions +=
            "--tupletsOnALine ";
          tupletsOnALinePresent = false;
        }

        if (breakLinesAtIncompleteRightMeasuresPresent) {
          gLpsrOptions->fBreakLinesAtIncompleteRightMeasures = true;
          gGeneralOptions->fCommandLineOptions +=
            "--breakLinesAtIncompleteRightMeasures ";
          breakLinesAtIncompleteRightMeasuresPresent = false;
        }

        if (separatorLineEveryNMeasuresPresent) {
          gLpsrOptions->fSeparatorLineEveryNMeasures = true;
          gLpsrOptions->fSeparatorLineEveryNMeasuresValue =
            atoi (optarg);
          gGeneralOptions->fCommandLineOptions +=
            "--separatorLineEveryNMeasures ";
          separatorLineEveryNMeasuresPresent = false;
        }

        if (dontKeepLineBreaksPresent) {
          gLpsrOptions->fDontKeepLineBreaks = true;
          gGeneralOptions->fCommandLineOptions +=
            "--dontKeepLineBreaks ";
          dontKeepLineBreaksPresent = false;
        }

        if (numericaltimePresent) {
          gLpsrOptions->fGenerateNumericalTime = true;
          gGeneralOptions->fCommandLineOptions +=
            "--numericalTime ";
          numericaltimePresent = false;
        }
        if (commentsPresent) {
          gLpsrOptions->fGenerateComments = true;
          gGeneralOptions->fCommandLineOptions +=
            "--comments ";
          commentsPresent = false;
        }
        if (stemsPresent) {
          gLpsrOptions->fGenerateStems = true;
          gGeneralOptions->fCommandLineOptions +=
            "--stems ";
          stemsPresent = false;
        }
        if (noAutoBeamingPresent) {
          gLpsrOptions->fNoAutoBeaming = true;
          gGeneralOptions->fCommandLineOptions +=
            "--noAutoBeaming ";
          noAutoBeamingPresent = false;
        }

        if (noteInputLineNumbersPresent) {
          gLpsrOptions->fGenerateInputLineNumbers = true;
          gGeneralOptions->fCommandLineOptions +=
            "--generateInputLineNumbers ";
          noteInputLineNumbersPresent = false;
        }
        
        if (modernTabPresent) {
          gLpsrOptions->fModernTab = true;
          gGeneralOptions->fCommandLineOptions +=
            "--modernTab ";
          modernTabPresent = false;
        }
        
        if (accidentalStylePresent) {
          // optarg contains the accidental style name
          stringstream s;

          if (! gLpsrOptions->setAccidentalStyle (optarg)) {
            s <<
              "--accidentalStyle argument '" << optarg <<
              "' is not known";
              
            optionError (s.str());
          }

          s <<
            "--accidentalStyle " << optarg << " ";
          
          gGeneralOptions->fCommandLineOptions +=
            s.str();
          accidentalStylePresent = false;
        }

        if (delayedOrnamentFractionPresent) {
          // optarg contains the midi tempo specification
          // decipher it to extract numerator and denominator values
          string optargAsString;
          {
            stringstream s;
            s << optarg;
            optargAsString = s.str();
          }
          
          string regularExpression (
            "[[:space:]]*([[:digit:]]+)[[:space:]]*"
            "/"
            "[[:space:]]*([[:digit:]]+)[[:space:]]*");
            
          regex  e (regularExpression);
          smatch sm;

          regex_match (optargAsString, sm, e);

          /*
          cout <<
            "There are " << sm.size() << " matches" <<
            " for string '" << optargAsString <<
            "' with regex '" << regularExpression <<
            "'" <<
            endl;
          */
        
          if (sm.size ()) {
            for (unsigned i = 0; i < sm.size (); ++i) {
              cout << "[" << sm [i] << "] ";
            } // for
            cout << endl;
          }
          
          else {
            stringstream s;

            s <<
              "--delayedOrnamentFraction argument '" << optarg <<
              "' is ill-formed";
              
            optionError (s.str());
          }
          
          {
            stringstream s;
            s << sm [1];
            s >> gLpsrOptions->fDelayedOrnamentFractionNumerator;
          }
          {
            stringstream s;
            s << sm [2];
            s >> gLpsrOptions->fDelayedOrnamentFractionDenominator;
          }
          /*
          cerr << JMI
            "gLpsrOptions->fDelayedOrnamentFractionNumerator = " <<
            gLpsrOptions->fDelayedOrnamentFractionNumerator <<
            endl <<
            "gLpsrOptions->fDelayedOrnamentFractionDenominator = " <<
            gLpsrOptions->fDelayedOrnamentFractionDenominator <<
            endl;
          */

          stringstream s;

          s <<
            "--delayedOrnamentFraction '" << optargAsString << "' ";
          
          gGeneralOptions->fCommandLineOptions +=
            s.str();
          midiTempoPresent = false;
        }

        if (midiTempoPresent) {
          // optarg contains the midi tempo specification
          // decipher it to extract duration and perSecond values
          string optargAsString;
          {
            stringstream s;
            s << optarg;
            optargAsString = s.str();
          }
          
          string regularExpression (
            "[[:space:]]*([[:digit:]]+\\.*)[[:space:]]*"
            "="
            "[[:space:]]*([[:digit:]]+)[[:space:]]*");
            
          regex  e (regularExpression);
          smatch sm;

          regex_match (optargAsString, sm, e);

          /*
          cout <<
            "There are " << sm.size() << " matches" <<
            " for string '" << optargAsString <<
            "' with regex '" << regularExpression <<
            "'" <<
            endl;
          */
        
          if (sm.size ()) {
            for (unsigned i = 0; i < sm.size (); ++i) {
              cout << "[" << sm [i] << "] ";
            } // for
            cout << endl;
          }
          
          else {
            stringstream s;

            s <<
              "--midiTempo argument '" << optarg <<
              "' is ill-formed";
              
            optionError (s.str());
          }
          
          gLpsrOptions->fMidiTempoDuration =  sm [1];

          {
            stringstream s;
            s << sm [2];
            s >> gLpsrOptions->fMidiTempoPerSecond;
          }
          /*
          cerr <<
            "gLpsrOptions->fMidiTempoDuration = " <<
            gLpsrOptions->fMidiTempoDuration <<
            endl <<
            "gLpsrOptions->fMidiTempoPerSecond = " <<
            gLpsrOptions->fMidiTempoPerSecond <<
            endl;
          */

          stringstream s;

          s <<
            "--midiTempo '" << optargAsString << "' ";
          
          gGeneralOptions->fCommandLineOptions +=
            s.str();
          midiTempoPresent = false;
        }
        
        if (dontGenerateLilyPondLyricsPresent) {
          gLpsrOptions->fDontGenerateLilyPondLyrics = true;
          gGeneralOptions->fCommandLineOptions +=
            "--dontGenerateLilyPondLyrics ";
          dontGenerateLilyPondLyricsPresent = false;
        }
        
        if (dontGenerateLilyPondCodePresent) {
          gLpsrOptions->fDontGenerateLilyPondCode = true;
          gGeneralOptions->fCommandLineOptions +=
            "--dontGenerateLilyPondCode ";
          dontGenerateLilyPondCodePresent = false;
        }

        }
        break;
        
      default:
        printUsage (kAllHelp, 1);
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
      printUsage (kAllHelp, 1);
      break;
    } //  switch

  // handle auto output file option
  if (gGeneralOptions->fAutoOutputFile) {
    if (outputFileName.size ()) {
      stringstream s;
  
      s <<
        "options '--aof, --autoOutputFile' and '--of, --outputFile'"  <<
        endl <<
        "cannot be used simultaneously";
        
      optionError (s.str ());
    }
  
    else if (inputFileName == "-") {
      stringstream s;
  
      s <<
        "option '--aof, --autoOutputFile'"  <<
        endl <<
        "cannot be used when reading from standard input";
        
      optionError (s.str ());
    }

    // build output file name
    string
      inputFileBasename = basename (inputFileName.c_str());
    
    outputFileName =
      inputFileBasename;
    
    size_t posInString =
      outputFileName.rfind ('.');
      
    if (posInString != string::npos)
      outputFileName.replace (
        posInString, outputFileName.size () - posInString, ".ly");
  }
}

//_______________________________________________________________________________
void printOptions ()
{
  if (gGeneralOptions->fTraceGeneral)
    cerr << idtr <<
      "The command line options and arguments have been analyzed" <<
      endl;

  cerr << idtr <<
    "The command line is:" <<
    endl;

  idtr++;
  cerr <<
    idtr <<
      gGeneralOptions->fProgramName << " " <<
      gGeneralOptions->fCommandLineOptions <<
      gGeneralOptions->fInputSourceName <<
      endl;
  idtr--;

  // the option name field width
  int const fieldWidth = 35;
  
  // General options
  // ---------------

  // print the chosen general options
  gGeneralOptions->
    printGeneralOptionsValues (fieldWidth);
    
  // MSR options
  // -----------

  // print the chosen MSR options
  gMsrOptions->
    printMsrOptionsValues (fieldWidth);
    
  // LPSR options
  // ------------

  // print the chosen LPSR options
  gLpsrOptions->
    printLpsrOptionsValues (fieldWidth);
}

//_______________________________________________________________________________
int main (int argc, char *argv[]) 
{
 /*
  cerr << "argc = " << argc << endl;
  for (int i = 0; i < argc ; i++ ) {
    cerr << "argv[ " << i << "] = " << argv[i] << endl;
  }
  */

  // enlist versions information
  enlistVersion (
    "0.0.0", "somewhere in 2016",
    "Start as a clone of xml2guido");
  enlistVersion (
    "0.1.0", "01-APR-2017",
    "Tentative version");
  
  // analyze the pitches and chords languages variables
  // ------------------------------------------------------

  initializePitchesLanguages ();
  initializeLpsrChordsLanguages ();
  
  // analyze the command line options
  // ------------------------------------------------------

  gGeneralOptions = msrGeneralOptions::create ();
  assert(gGeneralOptions != 0);

  gMsrOptions = msrOptions::create ();
  assert(gMsrOptions != 0);

  gLpsrOptions = lpsrOptions::create();
  assert(gLpsrOptions != 0);
  
  string    inputFileName;
  string    outputFileName;
  
  analyzeOptions (
    argc, argv,
    inputFileName, outputFileName);

  // program name
  // ------------------------------------------------------
  
  gGeneralOptions->fProgramName = argv [0];
  
  // input source name
  // ------------------------------------------------------

  gGeneralOptions->fInputSourceName = inputFileName;
  
  // translation date
  // ------------------------------------------------------

  time_t      translationRawtime;
  struct tm*  translationTimeinfo;
  char buffer [80];

  time (&translationRawtime);
  translationTimeinfo = localtime (&translationRawtime);

  strftime (buffer, 80, "%A %F @ %T %Z", translationTimeinfo);
  gGeneralOptions->fTranslationDate = buffer;
  
  // trace
  // ------------------------------------------------------

  if (gGeneralOptions->fTraceGeneral) {
    cerr <<  idtr <<
      "This is xml2Lilypond v" << currentVersionNumber () << 
      " from libmusicxml2 v" << musicxmllibVersionStr () <<
      endl;

    cerr <<  idtr <<
      "Launching conversion of ";

    if (gGeneralOptions->fInputSourceName == "-")
      cerr << "standard input";
    else
      cerr << "\"" << gGeneralOptions->fInputSourceName << "\"";

    cerr <<
      " to LilyPond" <<
      endl;

    cerr << idtr <<
      "LilyPond code will be written to ";
    if (outputFileName.size ())
      cerr << outputFileName;
    else
      cerr << "standard output";
    cerr << endl;
    
    printOptions ();
  }

  // open output file if need be
  // ------------------------------------------------------

  ofstream outStream;

  if (outputFileName.size ()) {
    if (gGeneralOptions->fTraceGeneral)
      cerr << idtr <<
        "Opening file '" << outputFileName << "' for writing" <<
        endl;
        
    outStream.open (outputFileName.c_str(), ofstream::out);
  }
      
  // create MSR from MusicXML contents
  // ------------------------------------------------------

  S_msrScore mScore;

  if (inputFileName == "-") {
    // input comes from standard input
    if (outputFileName.size ())
      mScore =
        musicxmlFd2Msr (stdin, gMsrOptions, outStream);
    else
      mScore =
        musicxmlFd2Msr (stdin, gMsrOptions, cout);
  }
  
  else {
    // input comes from a file
    if (outputFileName.size ())
      mScore =
        musicxmlFile2Msr (
          inputFileName.c_str(), gMsrOptions, outStream);
    else
      mScore =
        musicxmlFile2Msr (
          inputFileName.c_str(), gMsrOptions, cout);
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
        
  if (! gLpsrOptions->fDontGenerateLilyPondCode) {
    if (outputFileName.size()) // ??? JMI
      lpScore =
        msr2Lpsr (mScore, gMsrOptions, gLpsrOptions, outStream);
    else
      lpScore =
        msr2Lpsr (mScore, gMsrOptions, gLpsrOptions, cout);
    
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

  if (! gLpsrOptions->fDontGenerateLilyPondCode) {
    if (outputFileName.size ())
      lpsr2LilyPond (
        lpScore, gMsrOptions, gLpsrOptions, outStream);
    else
      lpsr2LilyPond (
        lpScore, gMsrOptions, gLpsrOptions, cout);
    
    if (outputFileName.size ()) {
      if (gGeneralOptions->fTraceGeneral)
        cerr << idtr <<
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
