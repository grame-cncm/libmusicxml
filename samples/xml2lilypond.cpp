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

#include "version.h"
#include "utilities.h"

#include "musicXMLTree2MsrInterface.h"

#include "msr2LpsrInterface.h"

#include "lpsr2LilypondInterface.h"

#include "lilypondOptions.h"


using namespace std;

using namespace MusicXML2;


// useful shortcut macros
#define idtr indenter::gIndenter
#define tab  indenter::gIndenter.getSpacer ()

enum msrHelpKind {
  kAllHelp,
  kGeneralHelp, kMusicXMLHelp, kMsrHelp, kLpsrHelp, kLilypondHelp};
  
//_______________________________________________________________________________
void printUsage (
  msrHelpKind helpKind,
  int         exitStatus)
{
  cerr <<
R"(
                Welcome to xml2lilypond v0.1.0, 
              the MusicXML to LilyPond translator
          delivered as part of the libmusicxml2 library.
      https://github.com/grame-cncm/libmusicxml/tree/lilypond

Usage:

    xml2lilypond [options] [MusicXMLFile|-]

What it does:

    This multi-pass translator basically performs 4 passes:
        Pass 1: reads the contents of MusicXMLFile or stdin ('-')
                and converts it to a MusicXML tree;
        Pass 2: converts that tree to a Music Score Representation (MSR);
        Pass 3: converts the MSR to a LilyPond Score Representation (LPSR);
        Pass 4: converts the LPSR to LilyPond source code 
                and writes it to standard output.

    Other passes are performed according to the options, such as
    printing views of the internal data or printing a summary of the score.

    There are various options to fine tune the generated LilyPond code
    and limit the need for manually editing the latter.

    The activity log and warning/error messages go to standard error.
)"
    << endl;

  // print versions history
  printVersionsHistory (cerr);
    
  // print options help
  switch (helpKind) {
    case kAllHelp:
      cerr <<
        "All options:" <<
        endl <<
        endl;
        
      gGeneralOptions->printGeneralOptionsHelp ();
      gMusicXMLOptions->printMusicXMLOptionsHelp ();
      gMsrOptions->printMsrOptionsHelp ();
      gLpsrOptions->printLpsrOptionsHelp ();
      gLilypondOptions->printLilypondOptionsHelp ();
      break;
      
    case kGeneralHelp:
      cerr <<
        "General options (use '--h, --help' to see all the options):" <<
        endl <<
        endl;
        
      gGeneralOptions->printGeneralOptionsHelp ();
      break;
      
    case kMusicXMLHelp:
      cerr <<
        "MusicXML options (use '--h, --help' to see all the options):" <<
        endl <<
        endl;
        
      gMusicXMLOptions->printMusicXMLOptionsHelp ();
      break;
      
    case kMsrHelp:
      cerr <<
        "MSR options (use '--h, --help' to see all the options):" <<
        endl <<
        endl;
        
      gMsrOptions->printMsrOptionsHelp ();
      break;
      
    case kLpsrHelp:
      cerr <<
        "LPSR options (use '--h, --help' to see all the options):" <<
        endl <<
        endl;
        
      gLpsrOptions->printLpsrOptionsHelp ();
      break;
      
    case kLilypondHelp:
      cerr <<
        "LilyPond options (use '--h, --help' to see all the options):" <<
        endl <<
        endl;
        
      gLilypondOptions->printLilypondOptionsHelp ();
      break;
  } // switch
  
  cerr <<
    endl;

  // over!
  exit (exitStatus);
}

//_______________________________________________________________________________
// a private variable
map<string, string> pOptionShortNames;

void checkOptionUniqueness (
  string optionLongName, string optionShortName)
{
  if (optionShortName == optionLongName) {
    stringstream s;

    s <<
      "option long name '" << optionLongName << "'" <<
      " is also used as short name";
      
    optionError (s.str());
  }
  
  for (
    map<string, string>::iterator i = pOptionShortNames.begin();
    i != pOptionShortNames.end();
    i++) {
      
    // is optionLongName in the options names map?
    if ((*i).first == optionLongName) {
      stringstream s;
  
      s <<
        "option long name '" << optionLongName << "'" <<
        " is specified more that once";
        
      optionError (s.str());
    }

    // is optionShortName in the options names map?
    if ((*i).second == optionShortName) {
      if (optionShortName.size ()) {
        stringstream s;
    
        s <<
          "option short name '" << optionShortName << "'" <<
          " for option long name '" << optionLongName << "'" <<
          " is specified more that once";
          
        optionError (s.str());
      }
    }
  } // for

  // everything OK, register the option names
  pOptionShortNames [optionLongName] = optionShortName;
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

  // version
  // --------------------------------------
  
  checkOptionUniqueness (
    _VERSION_LONG_NAME_, _VERSION_SHORT_NAME_);
    
  int versionPresent = 0;

  // help
  // --------------------------------------
  
  checkOptionUniqueness (
    _HELP_LONG_NAME_, _HELP_SHORT_NAME_);
    
  int helpPresent = 0;

  
  checkOptionUniqueness (
    _HELP_GENERAL_LONG_NAME_, _HELP_GENERAL_SHORT_NAME_);
    
  int helpGeneralPresent = 0;


  checkOptionUniqueness (
    _HELP_MUSICXML_LONG_NAME_, _HELP_MUSICXML_SHORT_NAME_);
    
  int helpMusicXMLPresent = 0;


  checkOptionUniqueness (
    _HELP_MSR_LONG_NAME_, _HELP_MSR_SHORT_NAME_);
    
  int helpMsrPresent = 0;


  checkOptionUniqueness (
    _HELP_LPSR_LONG_NAME_, _HELP_LPSR_SHORT_NAME_);
    
  int helpLpsrPresent = 0;


  checkOptionUniqueness (
    _HELP_LILYPOND_LONG_NAME_, _HELP_LILYPOND_SHORT_NAME_);
    
  int helpLilypondPresent = 0;

  // output file
  // --------------------------------------
  
  checkOptionUniqueness (
    _OUTPUT_FILE_LONG_NAME_, _OUTPUT_FILE_SHORT_NAME_);
    
  int outputFilePresent = 0;


  checkOptionUniqueness (
    _AUTO_OUTPUT_FILE_LONG_NAME_, _AUTO_OUTPUT_FILE_SHORT_NAME_);
    
  int autoOutputFilePresent = 0;

  // trace and display
  // --------------------------------------
  
  checkOptionUniqueness (
    _TRACE_GENERAL_LONG_NAME_, _TRACE_GENERAL_SHORT_NAME_);
    
  int traceGeneralPresent = 0;
  
  checkOptionUniqueness (
    _TRACE_DETAILED_LONG_NAME_, _TRACE_DETAILED_SHORT_NAME_);
    
  int traceDetailedPresent = 0;
  
  // CPU usage
  // --------------------------------------
  
  checkOptionUniqueness (
    _CPU_USAGE_LONG_NAME_, _CPU_USAGE_SHORT_NAME_);
    
  int displayCPUusagePresent = 0;

  // specific trace    
  // --------------------------------------

  // divisions
  checkOptionUniqueness (
    _TRACE_DIVISIONS_LONG_NAME_, _TRACE_DIVISIONS_SHORT_NAME_);
    
  int traceDivisionsPresent = 0;

  // geometry
  checkOptionUniqueness (
    _TRACE_GEOMETRY_LONG_NAME_, _TRACE_GEOMETRY_SHORT_NAME_);
    
  int traceGeometryPresent = 0;

  // part groups
  checkOptionUniqueness (
    _TRACE_PARTGROUPS_LONG_NAME_, _TRACE_PARTGROUPS_SHORT_NAME_);
    
  int tracePartGroupsPresent = 0;

  // parts
  checkOptionUniqueness (
    _TRACE_PARTS_LONG_NAME_, _TRACE_PARTS_SHORT_NAME_);
    
  int tracePartsPresent = 0;

  // staves
  checkOptionUniqueness (
    _TRACE_STAVES_LONG_NAME_, _TRACE_STAVES_SHORT_NAME_);
    
  int traceStavesPresent = 0;

  // voices
  checkOptionUniqueness (
    _TRACE_VOICES_LONG_NAME_, _TRACE_VOICES_SHORT_NAME_);
    
  int traceVoicesPresent = 0;

  checkOptionUniqueness (
    _TRACE_VOICES_DETAILS_LONG_NAME_, _TRACE_VOICES_DETAILS_SHORT_NAME_);
    
  int traceVoicesDetailsPresent = 0;

  // clefs
  checkOptionUniqueness (
    _TRACE_CLEFS_LONG_NAME_, _TRACE_CLEFS_SHORT_NAME_);
    
  int traceClefsPresent = 0;

  // keys
  checkOptionUniqueness (
    _TRACE_KEYS_LONG_NAME_, _TRACE_KEYS_SHORT_NAME_);
    
  int traceKeysPresent = 0;

  // times
  checkOptionUniqueness (
    _TRACE_TIMES_LONG_NAME_, _TRACE_TIMES_SHORT_NAME_);
    
  int traceTimesPresent = 0;

  // transpositions
  checkOptionUniqueness (
    _TRACE_TRANSPOSITIONS_LONG_NAME_, _TRACE_TRANSPOSITIONS_SHORT_NAME_);
    
  int traceTranspositionsPresent = 0;

  // segments
  checkOptionUniqueness (
    _TRACE_SEGMENTS_LONG_NAME_, _TRACE_SEGMENTS_SHORT_NAME_);
    
  int traceSegmentsPresent = 0;

  // repeats
  checkOptionUniqueness (
    _TRACE_REPEATS_LONG_NAME_, _TRACE_REPEATS_SHORT_NAME_);
    
  int traceRepeatsPresent = 0;

  // measures
  checkOptionUniqueness (
    _TRACE_MEASURES_LONG_NAME_, _TRACE_MEASURES_SHORT_NAME_);
    
  int traceMeasuresPresent = 0;

  // notes
  checkOptionUniqueness (
    _TRACE_NOTES_LONG_NAME_, _TRACE_NOTES_SHORT_NAME_);
    
  int traceNotesPresent = 0;

  checkOptionUniqueness (
    _TRACE_NOTES_DETAILS_LONG_NAME_, _TRACE_NOTES_DETAILS_SHORT_NAME_);
    
  int traceNotesDetailsPresent = 0;

  // breams
  checkOptionUniqueness (
    _TRACE_BEAMS_LONG_NAME_, _TRACE_BEAMS_SHORT_NAME_);
    
  int traceBeamsPresent = 0;

  // technicals
  checkOptionUniqueness (
    _TRACE_TECHNICALS_LONG_NAME_, _TRACE_TECHNICALS_SHORT_NAME_);
    
  int traceTechnicalsPresent = 0;
  
  // words
  checkOptionUniqueness (
    _TRACE_WORDS_LONG_NAME_, _TRACE_WORDS_SHORT_NAME_);
    
  int traceWordsPresent = 0;

  // tremolos
  checkOptionUniqueness (
    _TRACE_TREMOLOS_LONG_NAME_, _TRACE_TREMOLOS_SHORT_NAME_);
    
  int traceTremolosPresent = 0;

  // chords
  checkOptionUniqueness (
    _TRACE_CHORDS_LONG_NAME_, _TRACE_CHORDS_SHORT_NAME_);
    
  int traceChordsPresent = 0;

  // tuplets
  checkOptionUniqueness (
    _TRACE_TUPLETS_LONG_NAME_, _TRACE_TUPLETS_SHORT_NAME_);
    
  int traceTupletsPresent = 0;

  // grace notes
  checkOptionUniqueness (
    _TRACE_GRACENOTES_LONG_NAME_, _TRACE_GRACENOTES_SHORT_NAME_);
    
  int traceGracenotesPresent = 0;

  // lyrics
  checkOptionUniqueness (
    _TRACE_LYRICS_LONG_NAME_, _TRACE_LYRICS_SHORT_NAME_);
    
  int traceLyricsPresent = 0;

  // harmonies
  checkOptionUniqueness (
    _TRACE_HARMONIES_LONG_NAME_, _TRACE_HARMONIES_SHORT_NAME_);
    
  int traceHarmoniesPresent = 0;

  // figured bass
  checkOptionUniqueness (
    _TRACE_FIGURED_BASS_LONG_NAME_, _TRACE_FIGURED_BASS_SHORT_NAME_);
    
  int traceFiguredBassPresent = 0;

  // credits
  checkOptionUniqueness (
    _TRACE_CREDITS_LONG_NAME_, _TRACE_CREDITS_SHORT_NAME_);
    
  int traceCreditsPresent = 0;

  // Scheme functions
  checkOptionUniqueness (
    _TRACE_SCHEME_FUNCTIONS_LONG_NAME_, _TRACE_SCHEME_FUNCTIONS_SHORT_NAME_);
    
  int traceSchemeFunctionsPresent = 0;

  
  // MusicXML options
  // ----------------

  // trace and display
  // --------------------------------------

  checkOptionUniqueness (
    _TRACE_MUSICXML_TREE_VISITORS_LONG_NAME_, _TRACE_MUSICXML_TREE_VISITORS_SHORT_NAME_);
    
  int traceMusicXMLTreeVisitorsPresent = 0;

  // other
  // --------------------------------------

  checkOptionUniqueness (
    _INGORE_MUSICXML_ERRORS_LONG_NAME_, _INGORE_MUSICXML_ERRORS_SHORT_NAME_);
    
  int ignoreMusicXMLErrorsPresent = 0;


  checkOptionUniqueness (
    _LOOP_TO_MUSICXML_LONG_NAME_, _LOOP_TO_MUSICXML_SHORT_NAME_);
    
  int loopToMusicXMLPresent = 0;

  // MSR options
  // -----------

  // trace and display
  // --------------------------------------

  checkOptionUniqueness (
    _TRACE_MSR_LONG_NAME_, _TRACE_MSR_SHORT_NAME_);
    
  int traceMsrPresent = 0;


  checkOptionUniqueness (
    _TRACE_MSR_VISITORS_LONG_NAME_, _TRACE_MSR_VISITORS_SHORT_NAME_);
    
  int traceMsrVisitorsPresent = 0;


  checkOptionUniqueness (
    _DISPLAY_MSR_LONG_NAME_, _DISPLAY_MSR_SHORT_NAME_);
    
  int displayMsrPresent = 0;

  
  checkOptionUniqueness (
    _DISPLAY_MSR_SUMMARY_LONG_NAME_, _DISPLAY_MSR_SUMMARY_SHORT_NAME_);
    
  int displayMsrSummaryPresent = 0;
  
  // languages
  // --------------------------------------

  checkOptionUniqueness (
    _MSR_PITCHES_LANGUAGE_LONG_NAME_, _MSR_PITCHES_LANGUAGE_SHORT_NAME_);
    
  int msrPitchesLanguagePresent = 0;

  // parts
  // --------------------------------------

  checkOptionUniqueness (
    _PART_NAME_LONG_NAME_, _PART_NAME_SHORT_NAME_);
    
  int partNamePresent = 0;
  
  // voices
  // --------------------------------------

  checkOptionUniqueness (
    _STAFF_RELATIVE_VOICE_NUMBERS_LONG_NAME_, _STAFF_RELATIVE_VOICE_NUMBERS_SHORT_NAME_);
    
  int staffRelativeVoiceNumbersPresent = 0;


  checkOptionUniqueness (
    _SHOW_SILENT_VOICES_LONG_NAME_, _SHOW_SILENT_VOICES_SHORT_NAME_);
    
  int showSilentVoicesPresent = 0;

  checkOptionUniqueness (
    _KEEP_SILENT_VOICES_LONG_NAME_, _KEEP_SILENT_VOICES_SHORT_NAME_);
    
  int keepSilentVoicesPresent = 0;

  // notes
  // --------------------------------------

  checkOptionUniqueness (
    _DELAY_RESTS_DYNAMICS_LONG_NAME_, _DELAY_RESTS_DYNAMICS_SHORT_NAME_);
    
  int delayRestsDynamicsPresent = 0;


  checkOptionUniqueness (
    _DELAY_RESTS_WORDS_LONG_NAME_, _DELAY_RESTS_WORDS_SHORT_NAME_);
    
  int delayRestsWordsPresent = 0;


  checkOptionUniqueness (
    _DELAY_RESTS_SLURS_LONG_NAME_, _DELAY_RESTS_SLURS_SHORT_NAME_);
    
  int delayRestsSlursPresent = 0;

  
  checkOptionUniqueness (
    _DELAY_RESTS_LIGATURES_LONG_NAME_, _DELAY_RESTS_LIGATURES_SHORT_NAME_);
    
  int delayRestsLigaturesPresent = 0;


  checkOptionUniqueness (
    _DELAY_RESTS_WEDGES_LONG_NAME_, _DELAY_RESTS_WEDGES_SHORT_NAME_);
    
  int delayRestsWedgesPresent = 0;
  
  // lyrics
  // --------------------------------------

  checkOptionUniqueness (
    _SHOW_MSR_STANZAS_LONG_NAME_, _SHOW_MSR_STANZAS_SHORT_NAME_);
    
  int showMsrStanzasPresent = 0;

  
  checkOptionUniqueness (
    _KEEP_MUTE_STANZAS_LONG_NAME_, _KEEP_MUTE_STANZAS_SHORT_NAME_);
    
  int keepMuteStanzasPresent = 0;

  // harmonies
  // --------------------------------------

  checkOptionUniqueness (
    _SHOW_HARMONY_VOICES_LONG_NAME_, _SHOW_HARMONY_VOICES_SHORT_NAME_);
    
  int showHarmonyVoicesPresent = 0;

  checkOptionUniqueness (
    _KEEP_EMPTY_HARMONIES_VOICE_LONG_NAME_, _KEEP_EMPTY_HARMONIES_VOICE_SHORT_NAME_);
    
  int keepEmptyHarmonyVoicesPresent = 0;

  
  // LPSR options
  // ------------

  // trace and display
  // --------------------------------------

  checkOptionUniqueness (
    _TRACE_LPSR_LONG_NAME_, _TRACE_LPSR_SHORT_NAME_);
    
  int traceLpsrPresent = 0;


  checkOptionUniqueness (
    _TRACE_LPSR_VISITORS_LONG_NAME_, _TRACE_LPSR_VISITORS_SHORT_NAME_);
    
  int traceLpsrVisitorsPresent = 0;


  checkOptionUniqueness (
    _DISPLAY_LPSR_LONG_NAME_, _DISPLAY_LPSR_SHORT_NAME_);
    
  int displayLPSRPresent  = 0;

  // languages
  // --------------------------------------
  
  checkOptionUniqueness (
    _LPSR_PITCHES_LANGUAGE_LONG_NAME_, _LPSR_PITCHES_LANGUAGE_SHORT_NAME_);
    
  int lpsrPitchesLanguagePresent = 0;


  checkOptionUniqueness (
    _LPSR_CHORDS_LANGUAGE_LONG_NAME_, _LPSR_CHORDS_LANGUAGE_SHORT_NAME_);
    
  int lpsrChordsLanguagePresent = 0;
  
  // time
  // --------------------------------------
  
  checkOptionUniqueness (
    _NUMERICAL_TIME_LONG_NAME_, _NUMERICAL_TIME_SHORT_NAME_);
    
  int numericaltimePresent = 0;

  // notes
  // --------------------------------------
  
  checkOptionUniqueness (
    _ABSOLUTE_LONG_NAME_, _ABSOLUTE_SHORT_NAME_);
    
  int absolutePresent = 0;


  checkOptionUniqueness (
    _ALL_DURATIONS_LONG_NAME_, _ALL_DURATIONS_SHORT_NAME_);
    
  int allDurationsPresent = 0;


  checkOptionUniqueness (
    _STEMS_LONG_NAME_, _STEMS_SHORT_NAME_);
    
  int stemsPresent = 0;


  checkOptionUniqueness (
    _NO_AUTO_BEAMING_LONG_NAME_, _NO_AUTO_BEAMING_SHORT_NAME_);
    
  int noAutoBeamingPresent = 0;


  checkOptionUniqueness (
    _ROMAN_STRING_NUMBERS_LONG_NAME_, _ROMAN_STRING_NUMBERS_SHORT_NAME_);
    
  int romanStringNumbersPresent = 0;


  checkOptionUniqueness (
    _AVOID_OPEN_STRINGS_LONG_NAME_, _AVOID_OPEN_STRINGS_SHORT_NAME_);
    
  int avoidOpenStringPresent = 0;


  checkOptionUniqueness (
    _ACCIDENTAL_STYLE_LONG_NAME_, _ACCIDENTAL_STYLE_SHORT_NAME_);
    
  int accidentalStylePresent = 0;

  
  checkOptionUniqueness (
    _COMPRESS_MULTI_MEASURE_RESTS_LONG_NAME_, _COMPRESS_MULTI_MEASURE_RESTS_SHORT_NAME_);
    
  int compressMultiMeasureRestsPresent = 0;


  checkOptionUniqueness (
    _NOTE_INPUT_LINE_NUMBERS_LONG_NAME_, _NOTE_INPUT_LINE_NUMBERS_SHORT_NAME_);
    
  int noteInputLineNumbersPresent = 0;

  // bars
  // --------------------------------------
  
  checkOptionUniqueness (
    _SHOW_ALL_BAR_NUMBERS_LONG_NAME_, _SHOW_ALL_BAR_NUMBERS_SHORT_NAME_);
    
  int showAllBarNumbersPresent = 0;

  // line breaks
  // --------------------------------------
  
  checkOptionUniqueness (
    _DONT_KEEP_LINE_BREAKS_LONG_NAME_, _DONT_KEEP_LINE_BREAKS_SHORT_NAME_);
    
  int dontKeepLineBreaksPresent = 0;


  checkOptionUniqueness (
    _BREAK_LINES_AT_INCOMPLETE_RIGHT_MEASURES_LONG_NAME_, _BREAK_LINES_AT_INCOMPLETE_RIGHT_MEASURES_SHORT_NAME_);
    
  int breakLinesAtIncompleteRightMeasuresPresent = 0;

  
  checkOptionUniqueness (
    _SEPARATOR_LINE_EVERY_N_MEASURES_LONG_NAME_, _SEPARATOR_LINE_EVERY_N_MEASURES_SHORT_NAME_);
    
  int separatorLineEveryNMeasuresPresent = 0;

  // staves
  // --------------------------------------
  
  checkOptionUniqueness (
    _MODERN_TAB_LONG_NAME_, _MODERN_TAB_SHORT_NAME_);
    
  int modernTabPresent = 0;

  // fonts
  // --------------------------------------
  
  checkOptionUniqueness (
    _JAZZ_FONTS_LONG_NAME_, _JAZZ_FONTS_SHORT_NAME_);
    
  int jazzFontsPresent = 0;

  // code generation
  // --------------------------------------
  
  checkOptionUniqueness (
    _COMMENTS_LONG_NAME_, _COMMENTS_SHORT_NAME_);
    
  int commentsPresent = 0;


  checkOptionUniqueness (
    _GLOBAL_LONG_NAME_, _GLOBAL_SHORT_NAME_);
    
  int globalPresent = 0;


  checkOptionUniqueness (
    _TUPLETS_ON_A_LINE_LONG_NAME_, _TUPLETS_ON_A_LINE_SHORT_NAME_);
    
  int tupletsOnALinePresent = 0;

  checkOptionUniqueness (
    _REPEAT_BRACKETS_LONG_NAME_, _REPEAT_BRACKETS_SHORT_NAME_);
    
  int repeatBracketsPresent = 0;


  // ornaments

  checkOptionUniqueness (
    _DELAYED_ORNAMENTS_FRACTION_LONG_NAME_, _DELAYED_ORNAMENTS_FRACTION_SHORT_NAME_);
    
  int delayedOrnamentFractionPresent = 0;


  checkOptionUniqueness (
    _DISPLAY_MUSIC_LONG_NAME_, _DISPLAY_MUSIC_SHORT_NAME_);
    
  int displayMusicPresent = 0;


  checkOptionUniqueness (
    _NO_LILYPOND_CODE_LONG_NAME_, _NO_LILYPOND_CODE_SHORT_NAME_);
    
  int noLilypondCodePresent = 0;


  checkOptionUniqueness (
    _NO_LILYPOND_LYRICS_LONG_NAME_, _NO_LILYPOND_LYRICS_SHORT_NAME_);
    
  int noLilypondLyricsPresent = 0;


  checkOptionUniqueness (
    _LILYPOND_COMPILE_DATE_LONG_NAME_, _LILYPOND_COMPILE_DATE_SHORT_NAME_);
    
  int lilypondCompileDatePresent = 0;

  // score notation

  checkOptionUniqueness (
    _JIANPU_LONG_NAME_, _JIANPU_SHORT_NAME_);
    
  int jianpuPresent = 0;

  checkOptionUniqueness (
    _ABC_LONG_NAME_, _ABC_SHORT_NAME_);
    
  int abcPresent = 0;

  // midi
  // --------------------------------------
  
  checkOptionUniqueness (
    _MIDI_TEMPO_LONG_NAME_, _MIDI_TEMPO_SHORT_NAME_);
    
  int midiTempoPresent = 0;


  checkOptionUniqueness (
    _NO_MIDI_LONG_NAME_, _NO_MIDI_SHORT_NAME_);
    
  int noMidiCommandPresent = 0;


  // long_options data structure
  static struct option long_options [] =
    {
    /* These options set a flag. */

    // General options
    // ---------------

    // version
    // --------------------------------------

    {
      _VERSION_LONG_NAME_,
      no_argument, &versionPresent, 1
    },
    {
      _VERSION_SHORT_NAME_,
      no_argument, &versionPresent, 1
    },

    // help
    // --------------------------------------

    {
      _HELP_LONG_NAME_,
      no_argument, &helpPresent, 1
    },
    {
      _HELP_SHORT_NAME_,
      no_argument, &helpPresent, 1
    },

    
    {
      _HELP_GENERAL_LONG_NAME_,
      no_argument, &helpGeneralPresent, 1
    },

    {
      _HELP_GENERAL_SHORT_NAME_,
      no_argument, &helpGeneralPresent, 1
    },

    {
      _HELP_MUSICXML_LONG_NAME_,
      no_argument, &helpMusicXMLPresent, 1
    },

    {
      _HELP_MUSICXML_SHORT_NAME_,
      no_argument, &helpMusicXMLPresent, 1
    },
    {
      _HELP_MSR_LONG_NAME_,
      no_argument, &helpMsrPresent, 1
    },

    {
      _HELP_MSR_SHORT_NAME_,
      no_argument, &helpMsrPresent, 1
    },

    {
      _HELP_LPSR_LONG_NAME_,
      no_argument, &helpLpsrPresent, 1
    },

    {
      _HELP_LPSR_SHORT_NAME_,
      no_argument, &helpLpsrPresent, 1
    },

    {
      _HELP_LILYPOND_LONG_NAME_,
      no_argument, &helpLilypondPresent, 1
    },

    {
      _HELP_LILYPOND_SHORT_NAME_,
      no_argument, &helpLilypondPresent, 1
    },

    // output file
    // --------------------------------------

    {
      _OUTPUT_FILE_LONG_NAME_,
      required_argument, &outputFilePresent, 1
    },

    {
      _OUTPUT_FILE_SHORT_NAME_,
      required_argument, &outputFilePresent, 1
    },

    
    {
      _AUTO_OUTPUT_FILE_LONG_NAME_,
      no_argument, &autoOutputFilePresent, 1
    },

    {
      _AUTO_OUTPUT_FILE_SHORT_NAME_,
      no_argument, &autoOutputFilePresent, 1
    },

    // trace and display
    // --------------------------------------

    {
      _TRACE_GENERAL_LONG_NAME_,
      no_argument, &traceGeneralPresent, 1
    },

    {
      _TRACE_GENERAL_SHORT_NAME_,
      no_argument, &traceGeneralPresent, 1
    },


    {
      _TRACE_DETAILED_LONG_NAME_,
      required_argument, &traceDetailedPresent, 1
    },

    {
      _TRACE_DETAILED_SHORT_NAME_,
      required_argument, &traceDetailedPresent, 1
    },
    
    // CPU usage
    // --------------------------------------

    {
      _CPU_USAGE_LONG_NAME_,
      no_argument, &displayCPUusagePresent, 1
    },

    {
      _CPU_USAGE_SHORT_NAME_,
      no_argument, &displayCPUusagePresent, 1
    },

    // specific trace    
    // --------------------------------------

    // divisions
    {
      _TRACE_DIVISIONS_LONG_NAME_,
      no_argument, &traceDivisionsPresent, 1
    },

    {
      _TRACE_DIVISIONS_SHORT_NAME_,
      no_argument, &traceDivisionsPresent, 1
    },
    
    // geometry
    {
      _TRACE_GEOMETRY_LONG_NAME_,
      no_argument, &traceGeometryPresent, 1
    },

    {
      _TRACE_GEOMETRY_SHORT_NAME_,
      no_argument, &traceGeometryPresent, 1
    },
    
    // part groups
    {
      _TRACE_PARTGROUPS_LONG_NAME_,
      no_argument, &tracePartGroupsPresent, 1
    },

    {
      _TRACE_PARTGROUPS_SHORT_NAME_,
      no_argument, &tracePartGroupsPresent, 1
    },
    
    // parts
    {
      _TRACE_PARTS_LONG_NAME_,
      no_argument, &tracePartsPresent, 1
    },

    {
      _TRACE_PARTS_SHORT_NAME_,
      no_argument, &tracePartsPresent, 1
    },
    
    // staves
    {
      _TRACE_STAVES_LONG_NAME_,
      no_argument, &traceStavesPresent, 1
    },

    {
      _TRACE_STAVES_SHORT_NAME_,
      no_argument, &traceStavesPresent, 1
    },
    
    // voices
    {
      _TRACE_VOICES_LONG_NAME_,
      no_argument, &traceVoicesPresent, 1
    },

    {
      _TRACE_VOICES_SHORT_NAME_,
      no_argument, &traceVoicesPresent, 1
    },
    
    {
      _TRACE_VOICES_DETAILS_LONG_NAME_,
      no_argument, &traceVoicesDetailsPresent, 1
    },

    {
      _TRACE_VOICES_DETAILS_SHORT_NAME_,
      no_argument, &traceVoicesDetailsPresent, 1
    },
    
    // clefs
    {
      _TRACE_CLEFS_LONG_NAME_,
      no_argument, &traceClefsPresent, 1
    },

    {
      _TRACE_CLEFS_SHORT_NAME_,
      no_argument, &traceClefsPresent, 1
    },
    
    // keys
    {
      _TRACE_KEYS_LONG_NAME_,
      no_argument, &traceKeysPresent, 1
    },

    {
      _TRACE_KEYS_SHORT_NAME_,
      no_argument, &traceKeysPresent, 1
    },
    
    // times
    {
      _TRACE_TIMES_LONG_NAME_,
      no_argument, &traceTimesPresent, 1
    },

    {
      _TRACE_TIMES_SHORT_NAME_,
      no_argument, &traceTimesPresent, 1
    },
    
    // transpositions
    {
      _TRACE_TRANSPOSITIONS_LONG_NAME_,
      no_argument, &traceTranspositionsPresent, 1
    },

    {
      _TRACE_TRANSPOSITIONS_SHORT_NAME_,
      no_argument, &traceTranspositionsPresent, 1
    },
    
    // segments
    {
      _TRACE_SEGMENTS_LONG_NAME_,
      no_argument, &traceSegmentsPresent, 1
    },

    {
      _TRACE_SEGMENTS_SHORT_NAME_,
      no_argument, &traceSegmentsPresent, 1
    },
    
    // repeats
    {
      _TRACE_REPEATS_LONG_NAME_,
      no_argument, &traceRepeatsPresent, 1
    },

    {
      _TRACE_REPEATS_SHORT_NAME_,
      no_argument, &traceRepeatsPresent, 1
    },
    
    // measures
    {
      _TRACE_MEASURES_LONG_NAME_,
      no_argument, &traceMeasuresPresent, 1
    },

    {
      _TRACE_MEASURES_SHORT_NAME_,
      no_argument, &traceMeasuresPresent, 1
    },
    
    // notes
    {
      _TRACE_NOTES_LONG_NAME_,
      no_argument, &traceNotesPresent, 1
    },

    {
      _TRACE_NOTES_SHORT_NAME_,
      no_argument, &traceNotesPresent, 1
    },
    
    {
      _TRACE_NOTES_DETAILS_LONG_NAME_,
      no_argument, &traceNotesDetailsPresent, 1
    },

    {
      _TRACE_NOTES_DETAILS_SHORT_NAME_,
      no_argument, &traceNotesDetailsPresent, 1
    },
    
    // beams
    {
      _TRACE_BEAMS_LONG_NAME_,
      no_argument, &traceBeamsPresent, 1
    },

    {
      _TRACE_BEAMS_SHORT_NAME_,
      no_argument, &traceBeamsPresent, 1
    },
    
    // technicals
    {
      _TRACE_TECHNICALS_LONG_NAME_,
      no_argument, &traceTechnicalsPresent, 1
    },

    {
      _TRACE_TECHNICALS_SHORT_NAME_,
      no_argument, &traceTechnicalsPresent, 1
    },
    
    // words
    {
      _TRACE_WORDS_LONG_NAME_,
      no_argument, &traceWordsPresent, 1
    },

    {
      _TRACE_WORDS_SHORT_NAME_,
      no_argument, &traceWordsPresent, 1
    },
    
    // tremolos
    {
      _TRACE_TREMOLOS_LONG_NAME_,
      no_argument, &traceTremolosPresent, 1
    },

    {
      _TRACE_TREMOLOS_SHORT_NAME_,
      no_argument, &traceTremolosPresent, 1
    },
    
    // chords
    {
      _TRACE_CHORDS_LONG_NAME_,
      no_argument, &traceChordsPresent, 1
    },

    {
      _TRACE_CHORDS_SHORT_NAME_,
      no_argument, &traceChordsPresent, 1
    },
    
    // tuplets
    {
      _TRACE_TUPLETS_LONG_NAME_,
      no_argument, &traceTupletsPresent, 1
    },

    {
      _TRACE_TUPLETS_SHORT_NAME_,
      no_argument, &traceTupletsPresent, 1
    },
    
    // grace notes
    {
      _TRACE_GRACENOTES_LONG_NAME_,
      no_argument, &traceGracenotesPresent, 1
    },

    {
      _TRACE_GRACENOTES_SHORT_NAME_,
      no_argument, &traceGracenotesPresent, 1
    },
    
    // lyrics
    {
      _TRACE_LYRICS_LONG_NAME_,
      no_argument, &traceLyricsPresent, 1
    },

    {
      _TRACE_LYRICS_SHORT_NAME_,
      no_argument, &traceLyricsPresent, 1
    },
    
    // harmonies
    {
      _TRACE_HARMONIES_LONG_NAME_,
      no_argument, &traceHarmoniesPresent, 1
    },

    {
      _TRACE_HARMONIES_SHORT_NAME_,
      no_argument, &traceHarmoniesPresent, 1
    },
    
    // figured bass
    {
      _TRACE_FIGURED_BASS_LONG_NAME_,
      no_argument, &traceFiguredBassPresent, 1
    },

    {
      _TRACE_FIGURED_BASS_SHORT_NAME_,
      no_argument, &traceFiguredBassPresent, 1
    },
    
    // credits
    {
      _TRACE_CREDITS_LONG_NAME_,
      no_argument, &traceCreditsPresent, 1
    },

    {
      _TRACE_CREDITS_SHORT_NAME_,
      no_argument, &traceCreditsPresent, 1
    },
    
    // Scheme functions
    {
      _TRACE_SCHEME_FUNCTIONS_LONG_NAME_,
      no_argument, &traceSchemeFunctionsPresent, 1
    },

    {
      _TRACE_SCHEME_FUNCTIONS_SHORT_NAME_,
      no_argument, &traceSchemeFunctionsPresent, 1
    },
    
    // MusicXML options
    // ----------------

    // trace and display
    // --------------------------------------

    {
      _TRACE_MUSICXML_TREE_VISITORS_LONG_NAME_,
      no_argument, &traceMusicXMLTreeVisitorsPresent, 1
    },

    {
      _TRACE_MUSICXML_TREE_VISITORS_SHORT_NAME_,
      no_argument, &traceMusicXMLTreeVisitorsPresent, 1
    },

    // other
    // --------------------------------------

    {
      _INGORE_MUSICXML_ERRORS_LONG_NAME_,
      no_argument, &ignoreMusicXMLErrorsPresent, 1
    },

    {
      _INGORE_MUSICXML_ERRORS_SHORT_NAME_,
      no_argument, &ignoreMusicXMLErrorsPresent, 1
    },


    {
      _LOOP_TO_MUSICXML_LONG_NAME_,
      no_argument, &loopToMusicXMLPresent, 1
    },

    {
      _LOOP_TO_MUSICXML_SHORT_NAME_,
      no_argument, &loopToMusicXMLPresent, 1
    },

    // MSR options
    // -----------

    // trace and display
    // --------------------------------------

    {
      _TRACE_MSR_LONG_NAME_,
      no_argument, &traceMsrPresent, 1
    },

    {
      _TRACE_MSR_SHORT_NAME_,
      no_argument, &traceMsrPresent, 1
    },


    {
      _TRACE_MSR_VISITORS_LONG_NAME_,
      no_argument, &traceMsrVisitorsPresent, 1
    },

    {
      _TRACE_MSR_VISITORS_SHORT_NAME_,
      no_argument, &traceMsrVisitorsPresent, 1
    },

    {
      _DISPLAY_MSR_LONG_NAME_,
      no_argument, &displayMsrPresent, 1
    },
    
    {
      _DISPLAY_MSR_SHORT_NAME_,
      no_argument, &displayMsrPresent, 1
    },


    {
      _DISPLAY_MSR_SUMMARY_LONG_NAME_,
      no_argument, &displayMsrSummaryPresent, 1
    },

    {
      _DISPLAY_MSR_SUMMARY_SHORT_NAME_,
      no_argument, &displayMsrSummaryPresent, 1
    },

    // languages
    // --------------------------------------

    {
      _MSR_PITCHES_LANGUAGE_LONG_NAME_,
      required_argument, &msrPitchesLanguagePresent, 1
    },

    {
      _MSR_PITCHES_LANGUAGE_SHORT_NAME_,
      required_argument, &msrPitchesLanguagePresent, 1
    },

    // parts
    // --------------------------------------

    {
      _PART_NAME_LONG_NAME_,
      required_argument, &partNamePresent, 1
    },

    {
     _PART_NAME_SHORT_NAME_,
      required_argument, &partNamePresent, 1
    },

    // voices
    // --------------------------------------

    {
      _STAFF_RELATIVE_VOICE_NUMBERS_LONG_NAME_,
      no_argument,
      &staffRelativeVoiceNumbersPresent, 1
    },

    {
      _STAFF_RELATIVE_VOICE_NUMBERS_SHORT_NAME_,
      no_argument, &staffRelativeVoiceNumbersPresent, 1
    },

    
    {
      _SHOW_SILENT_VOICES_LONG_NAME_,
      no_argument, &showSilentVoicesPresent, 1
    },
    
    {
      _SHOW_SILENT_VOICES_SHORT_NAME_,
      no_argument, &showSilentVoicesPresent, 1
    },

    {
      _KEEP_SILENT_VOICES_LONG_NAME_,
      no_argument, &keepSilentVoicesPresent, 1
    },
    
    {
      _KEEP_SILENT_VOICES_SHORT_NAME_,
      no_argument, &keepSilentVoicesPresent, 1
    },

    // notes
    // --------------------------------------

    {
      _DELAY_RESTS_DYNAMICS_LONG_NAME_,
      no_argument, &delayRestsDynamicsPresent, 1
    },

    {
      _DELAY_RESTS_DYNAMICS_SHORT_NAME_,
      no_argument, &delayRestsDynamicsPresent, 1
    },

    {
      _DELAY_RESTS_WORDS_LONG_NAME_,
      no_argument, &delayRestsWordsPresent, 1
    },

    {
      _DELAY_RESTS_WORDS_SHORT_NAME_,
      no_argument, &delayRestsWordsPresent, 1
    },

    {
      _DELAY_RESTS_SLURS_LONG_NAME_,
      no_argument, &delayRestsSlursPresent, 1
    },

    {
      _DELAY_RESTS_SLURS_SHORT_NAME_,
      no_argument, &delayRestsSlursPresent, 1
    },

    {
      _DELAY_RESTS_LIGATURES_LONG_NAME_,
      no_argument, &delayRestsLigaturesPresent, 1
    },

    {
      _DELAY_RESTS_LIGATURES_SHORT_NAME_,
      no_argument, &delayRestsLigaturesPresent, 1
    },

    {
      _DELAY_RESTS_WEDGES_LONG_NAME_,
      no_argument, &delayRestsWedgesPresent, 1
    },

    {
     _DELAY_RESTS_WEDGES_SHORT_NAME_,
      no_argument, &delayRestsWedgesPresent, 1
    },
   
    // lyrics
    // --------------------------------------

    {
      _SHOW_MSR_STANZAS_LONG_NAME_,
      no_argument, &showMsrStanzasPresent, 1
    },

    {
      _SHOW_MSR_STANZAS_SHORT_NAME_,
      no_argument, &showMsrStanzasPresent, 1
    },


    {
      _KEEP_MUTE_STANZAS_LONG_NAME_,
      no_argument, &keepMuteStanzasPresent, 1
    },

    {
      _KEEP_MUTE_STANZAS_SHORT_NAME_,
      no_argument, &keepMuteStanzasPresent, 1
    },

    // harmonies
    // --------------------------------------

    {
      _SHOW_HARMONY_VOICES_LONG_NAME_,
      no_argument, &showHarmonyVoicesPresent, 1
    },
    
    {
      _SHOW_HARMONY_VOICES_SHORT_NAME_,
      no_argument, &showHarmonyVoicesPresent, 1
    },

    {
      _KEEP_EMPTY_HARMONIES_VOICE_LONG_NAME_,
      no_argument, &keepEmptyHarmonyVoicesPresent, 1
    },
    
    {
      _KEEP_EMPTY_HARMONIES_VOICE_SHORT_NAME_,
      no_argument, &keepEmptyHarmonyVoicesPresent, 1
    },

    // figured bass
    // --------------------------------------

    {
      _SHOW_HARMONY_VOICES_LONG_NAME_,
      no_argument, &showHarmonyVoicesPresent, 1
    },
    
    {
      _SHOW_HARMONY_VOICES_SHORT_NAME_,
      no_argument, &showHarmonyVoicesPresent, 1
    },

    // credits
    // --------------------------------------

    {
      _SHOW_HARMONY_VOICES_LONG_NAME_,
      no_argument, &showHarmonyVoicesPresent, 1
    },
    
    {
      _SHOW_HARMONY_VOICES_SHORT_NAME_,
      no_argument, &showHarmonyVoicesPresent, 1
    },

    // LPSR options
    // ------------

    // trace and display
    // --------------------------------------

    {
      _TRACE_LPSR_LONG_NAME_,
      no_argument, &traceLpsrPresent, 1
    },
    
    {
      _TRACE_LPSR_SHORT_NAME_,
      no_argument, &traceLpsrPresent, 1
    },


    {
      _TRACE_LPSR_VISITORS_LONG_NAME_,
      no_argument, &traceLpsrVisitorsPresent, 1
    },

    {
      _TRACE_LPSR_VISITORS_SHORT_NAME_,
      no_argument, &traceLpsrVisitorsPresent, 1
    },


    {
      _DISPLAY_LPSR_LONG_NAME_,
      no_argument, &displayLPSRPresent, 1
    },

    {
      _DISPLAY_LPSR_SHORT_NAME_,
      no_argument, &displayLPSRPresent, 1
    },

    // languages
    // --------------------------------------

    {
      _LPSR_PITCHES_LANGUAGE_LONG_NAME_,
      required_argument, &lpsrPitchesLanguagePresent, 1
    },
    
    {
      _LPSR_PITCHES_LANGUAGE_SHORT_NAME_,
      required_argument, &lpsrPitchesLanguagePresent, 1
    },
    

    {
      _LPSR_CHORDS_LANGUAGE_LONG_NAME_,
      required_argument, &lpsrChordsLanguagePresent, 1
    },

    {
      _LPSR_CHORDS_LANGUAGE_SHORT_NAME_,
      required_argument, &lpsrChordsLanguagePresent, 1
    },
    
    // time
    // --------------------------------------

    {
      _NUMERICAL_TIME_LONG_NAME_,
      no_argument, &numericaltimePresent, 1
    },

    {
      _NUMERICAL_TIME_SHORT_NAME_,
      no_argument, &numericaltimePresent, 1
    },
    
    // notes
    // --------------------------------------

    {
      _ABSOLUTE_LONG_NAME_,
      no_argument, &absolutePresent, 1
    },

    {
      _ABSOLUTE_SHORT_NAME_,
      no_argument, &absolutePresent, 1
    },

    
    {
      _ALL_DURATIONS_LONG_NAME_,
      no_argument, &allDurationsPresent, 1
    },

    {
      _ALL_DURATIONS_SHORT_NAME_,
      no_argument, &allDurationsPresent, 1
    },

    
    {
      _STEMS_LONG_NAME_,
      no_argument, &stemsPresent, 1
    },
    // _STEMS_SHORT_NAME_ is empty
    

    {
      _NO_AUTO_BEAMING_LONG_NAME_,
      no_argument, &noAutoBeamingPresent, 1
    },

    {
      _NO_AUTO_BEAMING_SHORT_NAME_,
      no_argument, &noAutoBeamingPresent, 1
    },

    
    {
      _ROMAN_STRING_NUMBERS_LONG_NAME_,
      no_argument, &romanStringNumbersPresent, 1
    },

    {
      _ROMAN_STRING_NUMBERS_SHORT_NAME_,
      no_argument, &romanStringNumbersPresent, 1
    },

    
    {
      _AVOID_OPEN_STRINGS_LONG_NAME_,
      no_argument, &avoidOpenStringPresent, 1
    },

    {
      _AVOID_OPEN_STRINGS_SHORT_NAME_,
      no_argument, &avoidOpenStringPresent, 1
    },

    
    {
      _ACCIDENTAL_STYLE_LONG_NAME_,
      required_argument, &accidentalStylePresent, 1
    },

    {
      _ACCIDENTAL_STYLE_SHORT_NAME_,
      required_argument, &accidentalStylePresent, 1
    },

    
    {
      _COMPRESS_MULTI_MEASURE_RESTS_LONG_NAME_,
      no_argument, &compressMultiMeasureRestsPresent, 1
    },

    {
      _COMPRESS_MULTI_MEASURE_RESTS_SHORT_NAME_,
      no_argument, &compressMultiMeasureRestsPresent, 1
    },

    
    {
      _NOTE_INPUT_LINE_NUMBERS_LONG_NAME_,
      no_argument, &noteInputLineNumbersPresent, 1
    },

    {
      _NOTE_INPUT_LINE_NUMBERS_SHORT_NAME_,
      no_argument, &noteInputLineNumbersPresent, 1
    },    

    // bars
    // --------------------------------------

    {
      _SHOW_ALL_BAR_NUMBERS_LONG_NAME_,
      no_argument, &showAllBarNumbersPresent, 1
    },

    {
      _SHOW_ALL_BAR_NUMBERS_SHORT_NAME_,
      no_argument, &showAllBarNumbersPresent, 1
    },
    
    // line breaks
    // --------------------------------------

    {
      _DONT_KEEP_LINE_BREAKS_LONG_NAME_,
      no_argument, &dontKeepLineBreaksPresent, 1
    },

    {
      _DONT_KEEP_LINE_BREAKS_SHORT_NAME_,
      no_argument, &dontKeepLineBreaksPresent, 1
    },

    
    {
      _BREAK_LINES_AT_INCOMPLETE_RIGHT_MEASURES_LONG_NAME_,
      no_argument, &breakLinesAtIncompleteRightMeasuresPresent, 1
    },

    {
      _BREAK_LINES_AT_INCOMPLETE_RIGHT_MEASURES_SHORT_NAME_,
      no_argument, &breakLinesAtIncompleteRightMeasuresPresent, 1
    },

    
    {
      _SEPARATOR_LINE_EVERY_N_MEASURES_LONG_NAME_,
      required_argument, &separatorLineEveryNMeasuresPresent, 1
    },

    {
      _SEPARATOR_LINE_EVERY_N_MEASURES_SHORT_NAME_,
      required_argument, &separatorLineEveryNMeasuresPresent, 1
    },
    
    // staves
    // --------------------------------------

    {
      _MODERN_TAB_LONG_NAME_,
      no_argument, &modernTabPresent, 1
    },

    {
      _MODERN_TAB_SHORT_NAME_,
      no_argument, &modernTabPresent, 1
    },
    
    // fonts
    // --------------------------------------

    {
      _JAZZ_FONTS_LONG_NAME_,
      no_argument, &jazzFontsPresent, 1
    },

    {
      _JAZZ_FONTS_SHORT_NAME_,
      no_argument, &jazzFontsPresent, 1
    },

    // code generation
    // --------------------------------------

    {
      _COMMENTS_LONG_NAME_,
      no_argument, &commentsPresent, 1
    },

    {
      _COMMENTS_SHORT_NAME_,
      no_argument, &commentsPresent, 1
    },

    
    {
      _GLOBAL_LONG_NAME_,
      no_argument, &globalPresent, 1
    },
    // _GLOBAL_SHORT_NAME_ is empty

    
    {
      _TUPLETS_ON_A_LINE_LONG_NAME_,
      no_argument, &tupletsOnALinePresent, 1
    },

    {
      _TUPLETS_ON_A_LINE_SHORT_NAME_,
      no_argument, &tupletsOnALinePresent, 1
    },
    
    {
      _REPEAT_BRACKETS_LONG_NAME_,
      no_argument, &repeatBracketsPresent, 1
    },

    {
      _REPEAT_BRACKETS_SHORT_NAME_,
      no_argument, &repeatBracketsPresent, 1
    },

    
    {
      _DELAYED_ORNAMENTS_FRACTION_LONG_NAME_,
      required_argument, &delayedOrnamentFractionPresent, 1
    },

    {
      _DELAYED_ORNAMENTS_FRACTION_SHORT_NAME_,
      required_argument, &delayedOrnamentFractionPresent, 1
    },

    
    {
      _DISPLAY_MUSIC_LONG_NAME_,
      no_argument, &displayMusicPresent, 1
    },

    {
      _DISPLAY_MUSIC_SHORT_NAME_,
      no_argument, &displayMusicPresent, 1
    },

    
    {
      _NO_LILYPOND_CODE_LONG_NAME_,
      no_argument, &noLilypondCodePresent, 1
    },

    {
      _NO_LILYPOND_CODE_SHORT_NAME_,
      no_argument, &noLilypondCodePresent, 1
    },


    {
      _NO_LILYPOND_LYRICS_LONG_NAME_,
      no_argument, &noLilypondLyricsPresent, 1
    },

    {
      _NO_LILYPOND_LYRICS_SHORT_NAME_,
      no_argument, &noLilypondLyricsPresent, 1
    },


    {
      _LILYPOND_COMPILE_DATE_LONG_NAME_,
      no_argument, &lilypondCompileDatePresent, 1
    },

    {
      _LILYPOND_COMPILE_DATE_SHORT_NAME_,
      no_argument, &lilypondCompileDatePresent, 1
    },

    // score notation
    // --------------------------------------

    {
      _JIANPU_LONG_NAME_,
      no_argument, &jianpuPresent, 1
    },

    {
      _JIANPU_SHORT_NAME_,
      no_argument, &jianpuPresent, 1
    },


    {
      _ABC_LONG_NAME_,
      no_argument, &abcPresent, 1
    },

    {
      _ABC_SHORT_NAME_,
      no_argument, &abcPresent, 1
    },

    // midi
    // --------------------------------------

    {
      _MIDI_TEMPO_LONG_NAME_,
      required_argument, &midiTempoPresent, 1
    },

    {
      _MIDI_TEMPO_SHORT_NAME_,
      required_argument, &midiTempoPresent, 1
    },

    
    {
      _NO_MIDI_LONG_NAME_,
      no_argument, &noMidiCommandPresent, 1
    },
    // _NO_MIDI_SHORT_NAME_ is empty
    

    {0, 0, 0, 0}
    };

/*
struct option {
    const char *name;
    int         has_arg;
    int        *flag;
    int         val;
*/

  vector<struct option> myLongOptions;

  struct option option0;

  option0.name =
    _VERSION_SHORT_NAME_;
  option0.has_arg =
    no_argument;
  option0.flag =
    &versionPresent;
  option0.val =
    1;

  struct option option1;

  option1.name =
    _LILYPOND_COMPILE_DATE_LONG_NAME_;
  option1.has_arg =
    no_argument;
  option1.flag =
    &lilypondCompileDatePresent;
  option1.val =
    1;

  struct option option2;

  option2.name =
    _HELP_LONG_NAME_;
  option2.has_arg =
    no_argument;
  option2.flag =
    &helpPresent;
  option2.val =
    1;

  struct option option3;

  option3.name =
    _HELP_SHORT_NAME_;
  option3.has_arg =
    no_argument;
  option3.flag =
    &helpPresent;
  option3.val =
    1;

  struct option optionsTrailer;

  optionsTrailer.name =
    0;
  optionsTrailer.has_arg =
    0;
  optionsTrailer.flag =
    0;
  optionsTrailer.val =
    0;

  myLongOptions.push_back (
    option0);
    
  myLongOptions.push_back (
    option1);
    
  myLongOptions.push_back (
    option2);
    
  myLongOptions.push_back (
    option3);
    
  myLongOptions.push_back (
    optionsTrailer);
    
  // initialize output file name
  outputFileName = "";
  
  /* getopt_long stores the option index here. */
  int option_index = 0;

  int c;

  cerr <<
    endl;
  
  while (
    (c = getopt_long (
      argc, argv,
      "hab",
      long_options,
// JMI      myLongOptions.data (),
      & option_index ))
      !=
    -1
    ) {
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
    
    switch (c) {
      case 0 :
        {

          {
            // General options
            // ------------
    
            // version
            // --------------------------------------
            
            if (versionPresent) {
              idtr++;
              
              cerr <<
                idtr <<
                  "This is " << argv [0] << " version " <<
                  currentVersionNumber () << "," <<
R"(
  an open translator of MusicXML into LilyPond.

  See https://github.com/grame-cncm/libmusicxml/tree/lilypond
  for more information and access to the source code.
)" <<
                endl;
    
              idtr--;
              
              exit (0);
              break;
            }
    
            // help
            
            if (helpPresent) {
              printUsage (kAllHelp, 0);
              break;
            }
    
            if (helpGeneralPresent) {
              printUsage (kGeneralHelp, 0);
              break;
            }
            if (helpMusicXMLPresent) {
              printUsage (kMusicXMLHelp, 0);
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
            if (helpLilypondPresent) {
              printUsage (kLilypondHelp, 0);
              break;
            }
    
            // output file
            // --------------------------------------
            
            if (outputFilePresent) {
              outputFileName = optarg;
              gGeneralOptions->fOutputFileName = outputFileName;
              
              {
                stringstream s;
      
                s <<
                  "--" _OUTPUT_FILE_LONG_NAME_ << " " << outputFileName << " ";
                  
                gGeneralOptions->fCommandLineLongOptions +=
                  s.str();
              }
              
              {
                stringstream s;
      
                s <<
                  "--" _OUTPUT_FILE_SHORT_NAME_ << " " << outputFileName << " ";
                  
                gGeneralOptions->fCommandLineShortOptions +=
                  s.str();
              }
              
              outputFilePresent = false;
            }
            
            if (autoOutputFilePresent) {
              gGeneralOptions->fAutoOutputFile = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _AUTO_OUTPUT_FILE_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _AUTO_OUTPUT_FILE_SHORT_NAME_ " ";
                
              autoOutputFilePresent = false;
            }
    
            // trace and display
            // --------------------------------------
            
            if (traceGeneralPresent) {
              gGeneralOptions->fTraceGeneral = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_GENERAL_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_GENERAL_SHORT_NAME_ " ";
                
              traceGeneralPresent = false;
            }
    
            if (traceDetailedPresent) {
              // optarg contains the measure numbers set specification
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceDetailed = true;
    
              char*        measuresSpec = optarg;
    
              gGeneralOptions->
                fTraceDetailedMeasureNumbersSet =
                  decipherNumbersSetSpecification (
                    measuresSpec, false); // 'true' to debug it
    
              {
                stringstream s;
      
                s <<
                   "--" _TRACE_DETAILED_LONG_NAME_ " " << measuresSpec << " ";
                   
                gGeneralOptions->fCommandLineLongOptions +=
                  s.str();
              }
    
              {
                stringstream s;
      
                s <<
                   "--" _TRACE_DETAILED_SHORT_NAME_ " " << measuresSpec << " ";
                   
                gGeneralOptions->fCommandLineShortOptions +=
                  s.str();
               }
                      
              traceDetailedPresent = false;
            }
    
            // CPU usage
            // --------------------------------------
    
            if (displayCPUusagePresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fDisplayCPUusage = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _CPU_USAGE_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _CPU_USAGE_SHORT_NAME_ " ";
                
              displayCPUusagePresent = false;
            }
            
            // specific trace
            // --------------------------------------
    
            // divisions
            if (traceDivisionsPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceDivisions = true;
              
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_DIVISIONS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_DIVISIONS_SHORT_NAME_ " ";
                
              traceDivisionsPresent = false;
            }
            
            // geometry
            if (traceGeometryPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceGeometry = true;
                        
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_GEOMETRY_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_GEOMETRY_SHORT_NAME_ " ";
                
              traceGeometryPresent = false;
            }
            
            // part groupss
            if (tracePartGroupsPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceParts = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_PARTGROUPS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_PARTGROUPS_SHORT_NAME_ " ";
                
              tracePartGroupsPresent = false;
            }
            
            // parts
            if (tracePartsPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceParts = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_PARTS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_PARTS_SHORT_NAME_ " ";
                
              tracePartsPresent = false;
            }
            
            // staves
            if (traceStavesPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceStaves = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_STAVES_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_STAVES_SHORT_NAME_ " ";
    
              traceStavesPresent = false;
            }
            
            // voices
            if (traceVoicesPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceVoices = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_VOICES_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_VOICES_SHORT_NAME_ " ";
    
              traceVoicesPresent = false;
            }
            if (traceVoicesDetailsPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceVoices = true;
              gGeneralOptions->fTraceVoicesDetails = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_VOICES_DETAILS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_VOICES_DETAILS_SHORT_NAME_ " ";
    
              traceVoicesDetailsPresent = false;
            }
    
            // clefs
            if (traceClefsPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceClefs = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_CLEFS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_CLEFS_SHORT_NAME_ " ";
    
              traceClefsPresent = false;
            }
    
            // keys
            if (traceKeysPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceKeys = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_KEYS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_KEYS_SHORT_NAME_ " ";
    
              traceKeysPresent = false;
            }
    
            // times
            if (traceTimesPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceTimes = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_TIMES_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_TIMES_SHORT_NAME_ " ";
    
              traceTimesPresent = false;
            }
    
            // transpositions
            if (traceTranspositionsPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceTranspositions = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_TRANSPOSITIONS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_TRANSPOSITIONS_SHORT_NAME_ " ";
    
              traceTranspositionsPresent = false;
            }
    
            // segments
            if (traceSegmentsPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceSegments = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_SEGMENTS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_SEGMENTS_SHORT_NAME_ " ";
                
              traceSegmentsPresent = false;
            }
    
            // repeats
            if (traceRepeatsPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceRepeats = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_REPEATS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_REPEATS_SHORT_NAME_ " ";
                
              traceRepeatsPresent = false;
            }
    
            // measures
            if (traceMeasuresPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceMeasures = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_MEASURES_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_MEASURES_SHORT_NAME_ " ";
    
              traceMeasuresPresent = false;
            }
    
            // notes
            if (traceNotesPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceNotes = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_NOTES_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_NOTES_SHORT_NAME_ " ";
                
              traceNotesPresent = false;
            }
            if (traceNotesDetailsPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceDivisions = true;
              gGeneralOptions->fTraceNotes = true;
              gGeneralOptions->fTraceNotesDetails = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_NOTES_DETAILS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_NOTES_DETAILS_SHORT_NAME_ " ";
                
              traceNotesDetailsPresent = false;
            }
    
            // beams
            if (traceBeamsPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceBeams = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_BEAMS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_BEAMS_SHORT_NAME_ " ";
                
              traceBeamsPresent = false;
            }
            
            // technicals
            if (traceTechnicalsPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceTechnicals = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_TECHNICALS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_TECHNICALS_SHORT_NAME_ " ";
                
              traceTechnicalsPresent = false;
            }
            
            // words
            if (traceWordsPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceWords = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_WORDS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_WORDS_SHORT_NAME_ " ";
                
              traceWordsPresent = false;
            }
            
            // tremolos
            if (traceTremolosPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceTremolos = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_TREMOLOS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_TREMOLOS_SHORT_NAME_ " ";
                
              traceTremolosPresent = false;
            }
    
            // chords
            if (traceChordsPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceChords = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_CHORDS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_CHORDS_SHORT_NAME_ " ";
                
              traceChordsPresent = false;
            }
    
            // tuplets
            if (traceTupletsPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceTuplets = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_TUPLETS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_TUPLETS_SHORT_NAME_ " ";
                            
              traceTupletsPresent = false;
            }
            
            // grace notes
            if (traceGracenotesPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceGraceNotes = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_GRACENOTES_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_GRACENOTES_SHORT_NAME_ " ";
                
              traceGracenotesPresent = false;
            }
    
            // lyrics
            if (traceLyricsPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceLyrics = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_LYRICS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_LYRICS_SHORT_NAME_ " ";
                
              traceLyricsPresent = false;
            }
            
            // harmonies
            if (traceHarmoniesPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceHarmonies = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_HARMONIES_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_HARMONIES_SHORT_NAME_ " ";
                
              traceHarmoniesPresent = false;
            }
            
            // figured bass
            if (traceFiguredBassPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceFiguredBass = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_FIGURED_BASS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_FIGURED_BASS_SHORT_NAME_ " ";
                
              traceFiguredBassPresent = false;
            }
            
            // credits
            if (traceCreditsPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gGeneralOptions->fTraceCredits = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_CREDITS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_CREDITS_SHORT_NAME_ " ";
                
              traceCreditsPresent = false;
            }
          }

          {
            // MusicXML options
            // ----------------
    
            // trace and display
            // --------------------------------------
    
            if (traceMusicXMLTreeVisitorsPresent) {
              gMusicXMLOptions->fTraceMusicXMLTreeVisitors = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_MUSICXML_TREE_VISITORS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_MUSICXML_TREE_VISITORS_SHORT_NAME_ " ";
                
              traceMusicXMLTreeVisitorsPresent = false;
            }
            
            // other
            // --------------------------------------
    
            if (ignoreMusicXMLErrorsPresent) {
              gMusicXMLOptions->fIgnoreMusicXMLErrors = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _INGORE_MUSICXML_ERRORS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _INGORE_MUSICXML_ERRORS_SHORT_NAME_ " ";
                
              ignoreMusicXMLErrorsPresent = false;
            }
            
            if (loopToMusicXMLPresent) {
              gMusicXMLOptions->fLoopToMusicXML = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _LOOP_TO_MUSICXML_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _LOOP_TO_MUSICXML_SHORT_NAME_ " ";
                
              loopToMusicXMLPresent = false;
            }
          }
  
          {
            // MSR options
            // -----------
            
            // trace and display
            // --------------------------------------
    
            if (traceMsrPresent) {
              gMsrOptions->fTraceMsr = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_MSR_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_MSR_SHORT_NAME_ " ";
                
              traceMsrPresent = false;
            }
    
            if (traceMsrVisitorsPresent) {
              gMsrOptions->fTraceMsrVisitors = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_MSR_VISITORS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_MSR_VISITORS_SHORT_NAME_ " ";
                
              traceMsrVisitorsPresent = false;
            }
    
            if (displayMsrPresent) {
              gMsrOptions->fDisplayMsr = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _DISPLAY_MSR_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _DISPLAY_MSR_SHORT_NAME_ " ";
                
              displayMsrPresent = false;
            }
    
            if (displayMsrSummaryPresent) {
              gMsrOptions->fDisplayMsrSummary = true;
              gLilypondOptions->fNoLilypondCode = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _DISPLAY_MSR_SUMMARY_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _DISPLAY_MSR_SUMMARY_SHORT_NAME_ " ";
                
              displayMsrSummaryPresent = false;
            }
            
            // languages
            // --------------------------------------
    
            if (msrPitchesLanguagePresent) {
              // optarg contains the language name
              string optargAsString;
              {
                stringstream s;
                s << optarg;
                optargAsString = s.str();
              }
              
              if (! gMsrOptions->setMsrQuarterTonesPitchesLanguage (
                optargAsString)) {
                stringstream s;
            
                s <<
                  "MSR pitches language " << optargAsString <<
                  " is unknown" <<
                  endl <<
                  "The " <<
                  gQuarterTonesPitchesLanguagesMap.size () <<
                  " known MSR pitches languages are:" <<
                  endl;
            
                idtr++;
              
                s <<
                  existingQuarterTonesPitchesLanguages ();
            
                idtr--;
            
                optionError (s.str());
              }
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _MSR_PITCHES_LANGUAGE_LONG_NAME_ " " +
                optargAsString +
                " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _MSR_PITCHES_LANGUAGE_SHORT_NAME_ " " +
                optargAsString +
                " ";
                
              msrPitchesLanguagePresent = false;
              }
                 
            // parts
            // --------------------------------------
    
            if (partNamePresent) {
              // optarg contains the part name
              char*        partNameSpec = optarg;
    
              {
                stringstream s;
      
                s <<
                  "--" _PART_NAME_LONG_NAME_ << " " << outputFileName << " ";
                  
                gGeneralOptions->fCommandLineLongOptions +=
                  s.str();
              }
              
              {
                stringstream s;
      
                s <<
                  "--" _PART_NAME_SHORT_NAME_ << " " << outputFileName << " ";
                  
                gGeneralOptions->fCommandLineShortOptions +=
                  s.str();
              }
              
              std::pair<string, string>
                pair =
                  extractNamesPairFromString (
                    partNameSpec,
                    '=',
                    false); // 'true' to debug it

              if (false) {
                cerr <<
                  "--> pair.first = \"" << pair.first << "\", " <<
                  "--> pair.second = \"" << pair.second << "\"" <<
                  endl;
              }
    
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
            
            // voices
            // --------------------------------------
            
            if (staffRelativeVoiceNumbersPresent) {
              gMsrOptions->fCreateVoicesStaffRelativeNumbers = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _STAFF_RELATIVE_VOICE_NUMBERS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _STAFF_RELATIVE_VOICE_NUMBERS_SHORT_NAME_ " ";
                
              staffRelativeVoiceNumbersPresent = false;
            }
            
            if (showSilentVoicesPresent) {
              gMsrOptions->fShowSilentVoices = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _SHOW_SILENT_VOICES_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _SHOW_SILENT_VOICES_SHORT_NAME_ " ";
                
              showSilentVoicesPresent = false;
            }
    
            if (keepSilentVoicesPresent) {
              gMsrOptions->fKeepSilentVoices = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _KEEP_SILENT_VOICES_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _KEEP_SILENT_VOICES_SHORT_NAME_ " ";
                
              keepSilentVoicesPresent = false;
            }
    
            // notes
            // --------------------------------------
            
            if (delayRestsDynamicsPresent) {
              gMsrOptions->fDelayRestsDynamics = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _DELAY_RESTS_DYNAMICS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _DELAY_RESTS_DYNAMICS_SHORT_NAME_ " ";
                
              delayRestsDynamicsPresent = false;
            }
            
            if (delayRestsWordsPresent) {
              gMsrOptions->fDelayRestsWords = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _DELAY_RESTS_WORDS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _DELAY_RESTS_WORDS_SHORT_NAME_ " ";
                
              delayRestsWordsPresent = false;
            }
            
            if (delayRestsSlursPresent) {
              gMsrOptions->fDelayRestsSlurs = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _DELAY_RESTS_SLURS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _DELAY_RESTS_SLURS_SHORT_NAME_ " ";
                
              delayRestsSlursPresent = false;
            }
            
            if (delayRestsLigaturesPresent) {
              gMsrOptions->fDelayRestsLigatures = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _DELAY_RESTS_LIGATURES_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _DELAY_RESTS_LIGATURES_SHORT_NAME_ " ";
                
              delayRestsLigaturesPresent = false;
            }
            
            if (delayRestsWedgesPresent) {
              gMsrOptions->fDelayRestsWedges = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _DELAY_RESTS_WEDGES_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _DELAY_RESTS_WEDGES_SHORT_NAME_ " ";
                
              delayRestsWedgesPresent = false;
            }
            
            // lyrics
            
            if (showMsrStanzasPresent) {
              gMsrOptions->fShowMsrStanzas = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _SHOW_MSR_STANZAS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _SHOW_MSR_STANZAS_SHORT_NAME_ " ";
                
              showMsrStanzasPresent = false;
            }
            
            if (keepMuteStanzasPresent) {
              gMsrOptions->fKeepMuteStanzas = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _KEEP_MUTE_STANZAS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _KEEP_MUTE_STANZAS_SHORT_NAME_ " ";
                
              keepMuteStanzasPresent = false;
            }
    
            // harmonies
    
            if (showHarmonyVoicesPresent) {
              gMsrOptions->fShowHarmonyVoices = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _SHOW_HARMONY_VOICES_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _SHOW_HARMONY_VOICES_SHORT_NAME_ " ";
                
              showHarmonyVoicesPresent = false;
            }
          
            if (keepEmptyHarmonyVoicesPresent) {
              gMsrOptions->fKeepEmptyHarmonyVoices = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _KEEP_EMPTY_HARMONIES_VOICE_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _KEEP_EMPTY_HARMONIES_VOICE_SHORT_NAME_ " ";
                
              keepEmptyHarmonyVoicesPresent = false;
            }
          }
  
          {
            // LPSR options
            // ------------
    
            // trace and display
            // --------------------------------------
    
            if (traceLpsrPresent) {
              gLpsrOptions->fTraceLpsr = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_LPSR_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_LPSR_SHORT_NAME_ " ";
                
              traceLpsrPresent = false;
            }
    
            if (traceLpsrVisitorsPresent) {
              gLpsrOptions->fTraceLpsrVisitors = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_LPSR_VISITORS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_LPSR_VISITORS_SHORT_NAME_ " ";
                
              traceLpsrVisitorsPresent = false;
            }
    
            if (displayLPSRPresent) {
              gLpsrOptions->fDisplayLpsr = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _DISPLAY_LPSR_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _DISPLAY_LPSR_SHORT_NAME_ " ";
                
              displayLPSRPresent = false;
            }
            
            // Scheme functions
            if (traceSchemeFunctionsPresent) {
              gGeneralOptions->fTraceGeneral = true;
              gLpsrOptions->fTraceSchemeFunctions = true;
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TRACE_SCHEME_FUNCTIONS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TRACE_SCHEME_FUNCTIONS_SHORT_NAME_ " ";
                
              traceSchemeFunctionsPresent = false;
            }
  
            // languages
            // --------------------------------------
    
            if (lpsrPitchesLanguagePresent) {
              // optarg contains the language name
              string optargAsString;
              {
                stringstream s;
                s << optarg;
                optargAsString = s.str();
              }
              
              if (! gLpsrOptions->setLpsrQuarterTonesPitchesLanguage (
                optargAsString)) {
                stringstream s;
    
                s <<
                  "LPSR pitches language name '" << optargAsString <<
                  "' is unknown" <<
                  endl <<
                  "The " <<
                  gQuarterTonesPitchesLanguagesMap.size () <<
                  " known LPSR pitches languages are:" <<
                  endl;
            
                idtr++;
              
                s <<
                  existingQuarterTonesPitchesLanguages ();
            
                idtr--;
            
                optionError (s.str());
              }
              
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _LPSR_PITCHES_LANGUAGE_LONG_NAME_ " " +
                optargAsString +
                " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _LPSR_PITCHES_LANGUAGE_SHORT_NAME_ " " +
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
                  gQuarterTonesPitchesLanguagesMap.size () <<
                  " known LPSR chords languages are:" <<
                  endl;
            
                idtr++;
              
                s <<
                  existingLpsrChordsLanguages ();
            
                idtr--;
                  
                optionError (s.str());
              }
              
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _LPSR_CHORDS_LANGUAGE_LONG_NAME_ " " +
              optargAsString +
              " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _LPSR_CHORDS_LANGUAGE_SHORT_NAME_ " " +
                optargAsString +
                " ";
    
              lpsrChordsLanguagePresent = false;
            }
          }
  
          {
            // LilyPond options
            // ----------------
            
            // time
            // --------------------------------------
    
            if (numericaltimePresent) {
              gLilypondOptions->fNumericalTime = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _NUMERICAL_TIME_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _NUMERICAL_TIME_SHORT_NAME_ " ";
                
              numericaltimePresent = false;
            }
    
            // notes
            // --------------------------------------
            
            if (absolutePresent) {
              gLilypondOptions->fAbsoluteOctaves = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _ABSOLUTE_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _ABSOLUTE_SHORT_NAME_ " ";
                
              absolutePresent = false;
            }
    
            if (allDurationsPresent) {
              gLilypondOptions->fAllDurations = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _ALL_DURATIONS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _ALL_DURATIONS_SHORT_NAME_ " ";
                
              allDurationsPresent = false;
            }
    
            if (stemsPresent) {
              gLilypondOptions->fStems = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _STEMS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _STEMS_LONG_NAME_ " "; // _STEMS_SHORT_NAME_ is empty
       
              stemsPresent = false;
            }
            
            if (noAutoBeamingPresent) {
              gLilypondOptions->fNoAutoBeaming = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _NO_AUTO_BEAMING_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _NO_AUTO_BEAMING_SHORT_NAME_ " ";
                
              noAutoBeamingPresent = false;
            }
    
            if (romanStringNumbersPresent) {
              gLilypondOptions->fRomanStringNumbers = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _ROMAN_STRING_NUMBERS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _ROMAN_STRING_NUMBERS_SHORT_NAME_ " ";
                
              romanStringNumbersPresent = false;
            }
    
            if (avoidOpenStringPresent) {
              gLilypondOptions->fAvoidOpenString = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _AVOID_OPEN_STRINGS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _AVOID_OPEN_STRINGS_SHORT_NAME_ " ";
                
              avoidOpenStringPresent = false;
            }
    
            if (accidentalStylePresent) {
              // optarg contains the accidental style name
              if (! gLilypondOptions->setAccidentalStyle (optarg)) {
                stringstream s;
      
                s <<
                  "--accidentalStyle argument '" << optarg <<
                  "' is not known";
                  
                optionError (s.str());
              }
    
              {
                stringstream s;
    
                s <<
                  "--" _ACCIDENTAL_STYLE_LONG_NAME_ " "  <<
                  optarg << " ";
      
                gGeneralOptions->fCommandLineLongOptions +=
                  s.str();
              }
    
              {
                stringstream s;
    
                s <<
                  "--" _ACCIDENTAL_STYLE_SHORT_NAME_ " "  <<
                  optarg << " ";
      
                gGeneralOptions->fCommandLineShortOptions +=
                  s.str();
              }
                
              accidentalStylePresent = false;
            }
    
            if (noteInputLineNumbersPresent) {
              gLilypondOptions->fInputLineNumbers = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _NOTE_INPUT_LINE_NUMBERS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _NOTE_INPUT_LINE_NUMBERS_SHORT_NAME_ " ";
                
              noteInputLineNumbersPresent = false;
            }
            
            // bars
            // --------------------------------------
            
            if (showAllBarNumbersPresent) {
              gLilypondOptions->fShowAllBarNumbers = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _SHOW_ALL_BAR_NUMBERS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _SHOW_ALL_BAR_NUMBERS_SHORT_NAME_ " ";
                
              showAllBarNumbersPresent = false;
            }
    
            if (compressMultiMeasureRestsPresent) {
              gLilypondOptions->fCompressMultiMeasureRests = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _COMPRESS_MULTI_MEASURE_RESTS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _COMPRESS_MULTI_MEASURE_RESTS_SHORT_NAME_ " ";
                
              compressMultiMeasureRestsPresent = false;
            }
    
            // line breaks
            // --------------------------------------
            
            if (dontKeepLineBreaksPresent) {
              gLilypondOptions->fDontKeepLineBreaks = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _DONT_KEEP_LINE_BREAKS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _DONT_KEEP_LINE_BREAKS_SHORT_NAME_ " ";
                
              dontKeepLineBreaksPresent = false;
            }
    
            if (breakLinesAtIncompleteRightMeasuresPresent) {
              gLilypondOptions->fBreakLinesAtIncompleteRightMeasures = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _BREAK_LINES_AT_INCOMPLETE_RIGHT_MEASURES_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _BREAK_LINES_AT_INCOMPLETE_RIGHT_MEASURES_SHORT_NAME_ " ";
                
              breakLinesAtIncompleteRightMeasuresPresent = false;
            }
    
            if (separatorLineEveryNMeasuresPresent) {
              gLilypondOptions->fSeparatorLineEveryNMeasures = true;
              gLilypondOptions->fSeparatorLineEveryNMeasuresValue =
                atoi (optarg);
    
              {
                stringstream s;
    
                s <<
                  "--" _SEPARATOR_LINE_EVERY_N_MEASURES_LONG_NAME_ " "  <<
                  optarg << " ";
      
                gGeneralOptions->fCommandLineLongOptions +=
                  s.str();
              }
    
              {
                stringstream s;
    
                s <<
                  "--" _SEPARATOR_LINE_EVERY_N_MEASURES_SHORT_NAME_ " "  <<
                  optarg << " ";
      
                gGeneralOptions->fCommandLineShortOptions +=
                  s.str();
              }
                
              separatorLineEveryNMeasuresPresent = false;
            }
    
            // staves
            // --------------------------------------
            
            if (modernTabPresent) {
              gLilypondOptions->fModernTab = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _MODERN_TAB_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _MODERN_TAB_SHORT_NAME_ " ";
                
              modernTabPresent = false;
            }
            
            // fonts
            // --------------------------------------

            if (jazzFontsPresent) {
              gLilypondOptions->fJazzFonts = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _JAZZ_FONTS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _JAZZ_FONTS_SHORT_NAME_ " ";
                
              jazzFontsPresent = false;
            }
    
            // LilyPond code generation
            // --------------------------------------
            
            if (commentsPresent) {
              gLilypondOptions->fComments = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _COMMENTS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _COMMENTS_SHORT_NAME_ " ";
                
              commentsPresent = false;
            }
    
            if (globalPresent) {
              gLilypondOptions->fGlobal = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _GLOBAL_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _GLOBAL_LONG_NAME_ " "; // _GLOBAL_SHORT_NAME_ is empty
                
              globalPresent = false;
            }
    
            if (tupletsOnALinePresent) {
              gLilypondOptions->fTupletsOnALine = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _TUPLETS_ON_A_LINE_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _TUPLETS_ON_A_LINE_SHORT_NAME_ " ";
                
              tupletsOnALinePresent = false;
            }
    
            if (repeatBracketsPresent) {
              gLilypondOptions->fRepeatBrackets = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _REPEAT_BRACKETS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _REPEAT_BRACKETS_SHORT_NAME_ " ";
                
              repeatBracketsPresent = false;
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
    
              if (false) {
                cout <<
                  "There are " << sm.size() << " matches" <<
                  " for string '" << optargAsString <<
                  "' with regex '" << regularExpression <<
                  "'" <<
                  endl;
              }
            
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
                s >> gLilypondOptions->fDelayedOrnamentFractionNumerator;
              }
              {
                stringstream s;
                s << sm [2];
                s >> gLilypondOptions->fDelayedOrnamentFractionDenominator;
              }

              if (false) {
                cerr << // JMI
                  "gLilypondOptions->fDelayedOrnamentFractionNumerator = " <<
                  gLilypondOptions->fDelayedOrnamentFractionNumerator <<
                  endl <<
                  "gLilypondOptions->fDelayedOrnamentFractionDenominator = " <<
                  gLilypondOptions->fDelayedOrnamentFractionDenominator <<
                  endl;
              }
    
              stringstream s;
    
              s <<
                "--delayedOrnamentFraction '" << optargAsString << "' ";
              
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _DELAYED_ORNAMENTS_FRACTION_LONG_NAME_ " " +
                optargAsString + "' ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _DELAYED_ORNAMENTS_FRACTION_SHORT_NAME_ " " +
                optargAsString + "' ";
                
              delayedOrnamentFractionPresent = false;
            }
    
            if (displayMusicPresent) {
              gLilypondOptions->fDisplayMusic = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _DISPLAY_MUSIC_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _DISPLAY_MUSIC_SHORT_NAME_ " ";
                
              displayMusicPresent = false;
            }
    
            if (noLilypondCodePresent) {
              gLilypondOptions->fNoLilypondCode = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _NO_LILYPOND_CODE_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _NO_LILYPOND_CODE_SHORT_NAME_ " ";
                
              noLilypondCodePresent = false;
            }
    
            if (noLilypondLyricsPresent) {
              gLilypondOptions->fNoLilypondLyrics = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _NO_LILYPOND_LYRICS_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _NO_LILYPOND_LYRICS_SHORT_NAME_ " ";
                
              noLilypondLyricsPresent = false;
            }
            
            if (lilypondCompileDatePresent) {
              gLilypondOptions->fLilypondCompileDate = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _LILYPOND_COMPILE_DATE_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _LILYPOND_COMPILE_DATE_SHORT_NAME_ " ";
                
              lilypondCompileDatePresent = false;
            }
          
            // score notation
            // --------------------------------------
    
            if (jianpuPresent) {
              gLilypondOptions->fScoreNotationKind =
                lilypondOptions::kJianpuNotation;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _JIANPU_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _JIANPU_LONG_NAME_ " ";
                // _JIANPU_SHORT_NAME_ is empty
                
              jianpuPresent = false;
            }
            
            if (abcPresent) {
              gLilypondOptions->fScoreNotationKind =
                lilypondOptions::kABCNotation;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _ABC_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _ABC_LONG_NAME_ " ";
                // _ABC_SHORT_NAME_ is empty
      
              abcPresent = false;
            }
    
            // Midi
            // --------------------------------------
            
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
    
              if (false) {
                cout <<
                  "There are " << sm.size() << " matches" <<
                  " for string '" << optargAsString <<
                  "' with regex '" << regularExpression <<
                  "'" <<
                  endl;
              }
            
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
              
              gLilypondOptions->fMidiTempoDuration =  sm [1];
    
              {
                stringstream s;
                s << sm [2];
                s >> gLilypondOptions->fMidiTempoPerSecond;
              }

              if (false) {
                cerr <<
                  "gLilypondOptions->fMidiTempoDuration = " <<
                  gLilypondOptions->fMidiTempoDuration <<
                  endl <<
                  "gLilypondOptions->fMidiTempoPerSecond = " <<
                  gLilypondOptions->fMidiTempoPerSecond <<
                  endl;
              }
    
              {
                stringstream s;
    
                s <<
                  "--" _MIDI_TEMPO_LONG_NAME_ " '"  <<
                  optarg << "' ";
      
                gGeneralOptions->fCommandLineLongOptions +=
                  s.str();
              }
    
              {
                stringstream s;
    
                s <<
                  "--" _MIDI_TEMPO_SHORT_NAME_ " '"  <<
                  optarg << "' ";
      
                gGeneralOptions->fCommandLineShortOptions +=
                  s.str();
              }
                            
              midiTempoPresent = false;
            }
    
            if (noMidiCommandPresent) {
              gLilypondOptions->fNoMidiCommand = true;
    
              gGeneralOptions->fCommandLineLongOptions +=
                "--" _NO_MIDI_LONG_NAME_ " ";
              gGeneralOptions->fCommandLineShortOptions +=
                "--" _NO_MIDI_LONG_NAME_ " "; // _NO_MIDI_SHORT_NAME_ is empty
                
              noMidiCommandPresent = false;
            }
          }
        }
        break;
          

      // otherwise
      // --------------------------------------

      default:
        cerr <<
          endl <<
          idtr <<
          "Use:" <<
          endl <<
          tab <<
            argv [0] << " '--help'" <<
            endl <<
          "to know more about the options" <<
          endl <<
          endl;

        exit (1);
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
  cerr <<
    endl <<
    idtr <<
    "The command line is:" <<
    endl;

  cerr <<
    idtr << tab <<
      gGeneralOptions->fProgramName << " " <<
      gGeneralOptions->fCommandLineLongOptions <<
      gGeneralOptions->fInputSourceName <<
      endl <<
    idtr <<
      "or:" <<
      endl <<
    idtr << tab <<
      gGeneralOptions->fProgramName << " " <<
      gGeneralOptions->fCommandLineShortOptions <<
      gGeneralOptions->fInputSourceName <<
      endl <<
    endl;

  // the option name field width
  int const fieldWidth = 35;
  
  // General options
  // ---------------

  // print the chosen general options
  gGeneralOptions->
    printGeneralOptionsValues (fieldWidth);
  cerr <<
    endl;
    
  // MusicXML options
  // -----------

  // print the chosen MusicXML options
  gMusicXMLOptions->
    printMusicXMLOptionsValues (fieldWidth);
  cerr <<
    endl;
    
  // MSR options
  // -----------

  // print the chosen MSR options
  gMsrOptions->
    printMsrOptionsValues (fieldWidth);
  cerr <<
    endl;
    
  // LPSR options
  // ------------

  // print the chosen LPSR options
  gLpsrOptions->
    printLpsrOptionsValues (fieldWidth);
  cerr <<
    endl;
        
  // LilyPond options
  // ------------

  // print the chosen LilyPond options
  gLilypondOptions->
    printLilypondOptionsValues (fieldWidth);
  cerr <<
    endl;

  // acknoledge end of command line analysis
  // ------------
  if (gGeneralOptions->fTraceGeneral)
    cerr <<
      endl <<
      idtr <<
        "The command line options and arguments have been analyzed" <<
        endl;
}

//_______________________________________________________________________________
int main (int argc, char *argv[]) 
{
  msrHarmony::initializeHarmoniesChordItemsVector (); // JMI
  msrHarmony::printHarmoniesChordItemsVector ();

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

  if (false) {
    cerr <<
      "7/16: " << wholeNotesAsMsrString (1, rational (7, 16)) <<
      endl <<
      "3/4:  " << wholeNotesAsMsrString (1, rational (3, 4)) <<
      endl <<
      "14/4: " << wholeNotesAsMsrString (1, rational (14, 4)) <<
      endl <<
      endl;
      
    exit (0);
  }
  
  // create the options variables
  // ------------------------------------------------------

  // MusicXML options
  
  gMusicXMLOptionsUserChoices = msrMusicXMLOptions::create ();
  assert(gMusicXMLOptionsUserChoices != 0);

  gMusicXMLOptions =
    gMusicXMLOptionsUserChoices;

  // general options
  
  gGeneralOptionsUserChoices = msrGeneralOptions::create ();
  assert(gGeneralOptionsUserChoices != 0);

  gGeneralOptions =
    gGeneralOptionsUserChoices;

  // MSR options
  
  gMsrOptionsUserChoices = msrOptions::create ();
  assert(gMsrOptionsUserChoices != 0);

  gMsrOptions =
    gMsrOptionsUserChoices;

  // LPSR options
  
  gLpsrOptionsUserChoices = lpsrOptions::create();
  assert(gLpsrOptionsUserChoices != 0);
  
  gLpsrOptions =
    gLpsrOptionsUserChoices;

  // LilyPond options
  
  gLilypondOptionsUserChoices = lilypondOptions::create();
  assert(gLilypondOptionsUserChoices != 0);
  
  gLilypondOptions =
    gLilypondOptionsUserChoices;

  // analyze the command line options
  // ------------------------------------------------------

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

  // prepare for measure detailed trace
  // ------------------------------------------------------

  gMusicXMLOptionsWithDetailedTrace =
    gMusicXMLOptions->
      createCloneWithDetailedTrace ();

  gGeneralOptionsWithDetailedTrace =
    gGeneralOptions->
      createCloneWithDetailedTrace ();

  gMsrOptionsWithDetailedTrace =
    gMsrOptions->
      createCloneWithDetailedTrace ();

  gLpsrOptionsWithDetailedTrace =
    gLpsrOptions->
      createCloneWithDetailedTrace ();

  gLilypondOptionsWithDetailedTrace =
    gLilypondOptions->
      createCloneWithDetailedTrace ();

  // welcome message
  // ------------------------------------------------------

  if (gGeneralOptions->fTraceGeneral) {
    cerr <<  idtr <<
      "This is xml2Lilypond v" << currentVersionNumber () << 
      " from libmusicxml2 v" << musicxmllibVersionStr () <<
      endl;

    cerr << idtr <<
      "Launching conversion of ";

    if (gGeneralOptions->fInputSourceName == "-")
      cerr << "standard input";
    else
      cerr << "\"" << gGeneralOptions->fInputSourceName << "\"";

    cerr <<
      " to LilyPond" <<
      endl;

    cerr << idtr <<
      "Time is " << gGeneralOptions->fTranslationDate <<
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
        
  if (! gLilypondOptions->fNoLilypondCode) {
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

  if (! gLilypondOptions->fNoLilypondCode) {
    if (outputFileName.size ())
      lpsr2Lilypond (
        lpScore, gMsrOptions, gLpsrOptions, outStream);
    else
      lpsr2Lilypond (
        lpScore, gMsrOptions, gLpsrOptions, cout);
    
    if (outputFileName.size ()) {
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

