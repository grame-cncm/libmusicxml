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
      gMsrOptions->printMsrOptionsHelp ();
      gLpsrOptions->printLpsrOptionsHelp ();
      break;
      
    case kGeneralHelp:
      cerr <<
        "General options (use '--h, --help' to see all the options):" <<
        endl <<
        endl;
      gGeneralOptions->printGeneralOptionsHelp ();
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
  
  #define _VERSION_LONG_NAME_  "version"
  #define _VERSION_SHORT_NAME_ "v"
  
  checkOptionUniqueness (
    _VERSION_LONG_NAME_, _VERSION_SHORT_NAME_);
    
  int versionPresent = 0;

  // help
  
  #define _HELP_LONG_NAME_  "help"
  #define _HELP_SHORT_NAME_ "h"
  
  checkOptionUniqueness (
    _HELP_LONG_NAME_, _HELP_SHORT_NAME_);
    
  int helpPresent = 0;
  
  #define _HELP_GENERAL_LONG_NAME_  "helpGeneral"
  #define _HELP_GENERAL_SHORT_NAME_ "hg"
  
  checkOptionUniqueness (
    _HELP_GENERAL_LONG_NAME_, _HELP_GENERAL_SHORT_NAME_);
    
  int helpGeneralPresent = 0;

  #define _HELP_MSR_LONG_NAME_  "helpMsr"
  #define _HELP_MSR_SHORT_NAME_ "hm"
  
  checkOptionUniqueness (
    _HELP_MSR_LONG_NAME_, _HELP_MSR_SHORT_NAME_);
    
  int helpMsrPresent = 0;

  #define _HELP_LPSR_LONG_NAME_  "helpLpsr"
  #define _HELP_LPSR_SHORT_NAME_ "hlp"
  
  checkOptionUniqueness (
    _HELP_LPSR_LONG_NAME_, _HELP_LPSR_SHORT_NAME_);
    
  int helpLpsrPresent = 0;

  // output file
  
  #define _OUTPUT_FILE_LONG_NAME_  "outputFile"
  #define _OUTPUT_FILE_SHORT_NAME_ "of"
  
  checkOptionUniqueness (
    _OUTPUT_FILE_LONG_NAME_, _OUTPUT_FILE_SHORT_NAME_);
    
  int outputFilePresent = 0;

  #define _AUTO_OUTPUT_FILE_LONG_NAME_  "autoOutputFile"
  #define _AUTO_OUTPUT_FILE_SHORT_NAME_ "aof"
  
  checkOptionUniqueness (
    _AUTO_OUTPUT_FILE_LONG_NAME_, _AUTO_OUTPUT_FILE_SHORT_NAME_);
    
  int autoOutputFilePresent = 0;

  // trace and display
  
  #define _TRACE_GENERAL_LONG_NAME_  "traceGeneral"
  #define _TRACE_GENERAL_SHORT_NAME_ "t"
  
  checkOptionUniqueness (
    _TRACE_GENERAL_LONG_NAME_, _TRACE_GENERAL_SHORT_NAME_);
    
  int traceGeneralPresent = 0;
  
  // specific trace    

  #define _TRACE_DIVISIONS_LONG_NAME_  "traceDivisions"
  #define _TRACE_DIVISIONS_SHORT_NAME_ "tdivs"
  
  checkOptionUniqueness (
    _TRACE_DIVISIONS_LONG_NAME_, _TRACE_DIVISIONS_SHORT_NAME_);
    
  int traceDivisionsPresent = 0;
  
  #define _TRACE_PARTS_LONG_NAME_  "traceParts"
  #define _TRACE_PARTS_SHORT_NAME_ "tparts"
  
  checkOptionUniqueness (
    _TRACE_PARTS_LONG_NAME_, _TRACE_PARTS_SHORT_NAME_);
    
  int tracePartsPresent = 0;
  
  #define _TRACE_VOICES_LONG_NAME_  "traceVoices"
  #define _TRACE_VOICES_SHORT_NAME_ "tvoices"
  
  checkOptionUniqueness (
    _TRACE_VOICES_LONG_NAME_, _TRACE_VOICES_SHORT_NAME_);
    
  int traceVoicesPresent = 0;
  
  #define _TRACE_SEGMENTS_LONG_NAME_  "traceSegments"
  #define _TRACE_SEGMENTS_SHORT_NAME_ "tsegs"
  
  checkOptionUniqueness (
    _TRACE_SEGMENTS_LONG_NAME_, _TRACE_SEGMENTS_SHORT_NAME_);
    
  int traceSegmentsPresent = 0;
  
  #define _TRACE_REPEATS_LONG_NAME_  "traceRepeats"
  #define _TRACE_REPEATS_SHORT_NAME_ "treps"
  
  checkOptionUniqueness (
    _TRACE_REPEATS_LONG_NAME_, _TRACE_REPEATS_SHORT_NAME_);
    
  int traceRepeatsPresent = 0;
  
  #define _TRACE_MEASURES_LONG_NAME_  "traceMeasures"
  #define _TRACE_MEASURES_SHORT_NAME_ "tmeas"
  
  checkOptionUniqueness (
    _TRACE_MEASURES_LONG_NAME_, _TRACE_MEASURES_SHORT_NAME_);
    
  int traceMeasuresPresent = 0;
  
  #define _TRACE_NOTES_LONG_NAME_  "traceNotes"
  #define _TRACE_NOTES_SHORT_NAME_ "tnotes"
  
  checkOptionUniqueness (
    _TRACE_NOTES_LONG_NAME_, _TRACE_NOTES_SHORT_NAME_);
    
  int traceNotesPresent = 0;
  
  #define _TRACE_TREMOLOS_LONG_NAME_  "traceTremolos"
  #define _TRACE_TREMOLOS_SHORT_NAME_ "ttrem"
  
  checkOptionUniqueness (
    _TRACE_TREMOLOS_LONG_NAME_, _TRACE_TREMOLOS_SHORT_NAME_);
    
  int traceTremolosPresent = 0;
  
  #define _TRACE_CHORDS_LONG_NAME_  "traceChords"
  #define _TRACE_CHORDS_SHORT_NAME_ "tchords"
  
  checkOptionUniqueness (
    _TRACE_CHORDS_LONG_NAME_, _TRACE_CHORDS_SHORT_NAME_);
    
  int traceChordsPresent = 0;
  
  #define _TRACE_TUPLETS_LONG_NAME_  "traceTuplets"
  #define _TRACE_TUPLETS_SHORT_NAME_ "ttuplets"
  
  checkOptionUniqueness (
    _TRACE_TUPLETS_LONG_NAME_, _TRACE_TUPLETS_SHORT_NAME_);
    
  int traceTupletsPresent = 0;
  
  #define _TRACE_GRACENOTES_LONG_NAME_  "traceGracenotes"
  #define _TRACE_GRACENOTES_SHORT_NAME_ "tgrace"
  
  checkOptionUniqueness (
    _TRACE_GRACENOTES_LONG_NAME_, _TRACE_GRACENOTES_SHORT_NAME_);
    
  int traceGracenotesPresent = 0;
  
  #define _TRACE_LYRICS_LONG_NAME_  "traceLyrics"
  #define _TRACE_LYRICS_SHORT_NAME_ "tlyrics"
  
  checkOptionUniqueness (
    _TRACE_LYRICS_LONG_NAME_, _TRACE_LYRICS_SHORT_NAME_);
    
  int traceLyricsPresent = 0;
  
  #define _TRACE_HARMONIES_LONG_NAME_  "traceHarmonies"
  #define _TRACE_HARMONIES_SHORT_NAME_ "tharms"
  
  checkOptionUniqueness (
    _TRACE_HARMONIES_LONG_NAME_, _TRACE_HARMONIES_SHORT_NAME_);
    
  int traceHarmoniesPresent = 0;
  
  #define _TRACE_DETAILED_LONG_NAME_  "traceDetailed"
  #define _TRACE_DETAILED_SHORT_NAME_ "tdetail"
  
  checkOptionUniqueness (
    _TRACE_DETAILED_LONG_NAME_, _TRACE_DETAILED_SHORT_NAME_);
    
  int traceDetailedPresent = 0;
  
  // CPU usage
  
  #define _CPU_USAGE_LONG_NAME_  "displayCPUusage"
  #define _CPU_USAGE_SHORT_NAME_ "dCPU"
  
  checkOptionUniqueness (
    _CPU_USAGE_LONG_NAME_, _CPU_USAGE_SHORT_NAME_);
    
  int displayCPUusagePresent = 0;

  
  // MSR options
  // -----------

  // trace and display

  #define _TRACE_MSR_LONG_NAME_  "traceMsr"
  #define _TRACE_MSR_SHORT_NAME_ "tmsr"
  
  checkOptionUniqueness (
    _TRACE_MSR_LONG_NAME_, _TRACE_MSR_SHORT_NAME_);
    
  int traceMSRPresent = 0;

  #define _TRACE_MSR_VISITORS_LONG_NAME_  "traceMsrVisitors"
  #define _TRACE_MSR_VISITORS_SHORT_NAME_ "tmvisits"
  
  checkOptionUniqueness (
    _TRACE_MSR_VISITORS_LONG_NAME_, _TRACE_MSR_VISITORS_SHORT_NAME_);
    
  int traceMSRVisitorsPresent = 0;

  #define _DISPLAY_MSR_LONG_NAME_  "displayMsr"
  #define _DISPLAY_MSR_SHORT_NAME_ "msr"
  
  checkOptionUniqueness (
    _DISPLAY_MSR_LONG_NAME_, _DISPLAY_MSR_SHORT_NAME_);
    
  int displayMSRPresent = 0;
  
  #define _DISPLAY_MSR_SUMMARY_LONG_NAME_  "displayMsrSummary"
  #define _DISPLAY_MSR_SUMMARY_SHORT_NAME_ "sum"
  
  checkOptionUniqueness (
    _DISPLAY_MSR_SUMMARY_LONG_NAME_, _DISPLAY_MSR_SUMMARY_SHORT_NAME_);
    
  int displayMSRSummaryPresent = 0;
  
  // languages

  #define _MSR_PITCHES_LANGUAGE_LONG_NAME_  "msrPitchesLanguage"
  #define _MSR_PITCHES_LANGUAGE_SHORT_NAME_ "mpl"
  
  checkOptionUniqueness (
    _MSR_PITCHES_LANGUAGE_LONG_NAME_, _MSR_PITCHES_LANGUAGE_SHORT_NAME_);
    
  int msrPitchesLanguagePresent = 0;

  // parts

  #define _PART_NAME_LONG_NAME_  "partName"
  #define _PART_NAME_SHORT_NAME_ "part"
  
  checkOptionUniqueness (
    _PART_NAME_LONG_NAME_, _PART_NAME_SHORT_NAME_);
    
  int partNamePresent = 0;
  
  // voices

  #define _STAFF_RELATIVE_VOICE_NUMBERS_LONG_NAME_  "staffRelativeVoiceNumbers"
  #define _STAFF_RELATIVE_VOICE_NUMBERS_SHORT_NAME_ "srvn"
  
  checkOptionUniqueness (
    _STAFF_RELATIVE_VOICE_NUMBERS_LONG_NAME_, _STAFF_RELATIVE_VOICE_NUMBERS_SHORT_NAME_);
    
  int staffRelativeVoiceNumbersPresent = 0;

  #define _KEEP_MASTER_VOICES_LONG_NAME_  "keepMasterVoices"
  #define _KEEP_MASTER_VOICES_SHORT_NAME_ "kmv"
  
  checkOptionUniqueness (
    _KEEP_MASTER_VOICES_LONG_NAME_, _KEEP_MASTER_VOICES_SHORT_NAME_);
    
  int keepMasterVoicesPresent = 0;

  // notes

  #define _DELAY_RESTS_DYNAMICS_LONG_NAME_  "delayRestsDynamics"
  #define _DELAY_RESTS_DYNAMICS_SHORT_NAME_ "drdyns"
  
  checkOptionUniqueness (
    _DELAY_RESTS_DYNAMICS_LONG_NAME_, _DELAY_RESTS_DYNAMICS_SHORT_NAME_);
    
  int delayRestsDynamicsPresent = 0;

  #define _DELAY_RESTS_WORDS_LONG_NAME_  "delayRestsWords"
  #define _DELAY_RESTS_WORDS_SHORT_NAME_ "drwords"
  
  checkOptionUniqueness (
    _DELAY_RESTS_WORDS_LONG_NAME_, _DELAY_RESTS_WORDS_SHORT_NAME_);
    
  int delayRestsWordsPresent = 0;

  #define _DELAY_RESTS_SLURS_LONG_NAME_  "delayRestsSlurs"
  #define _DELAY_RESTS_SLURS_SHORT_NAME_ "drslurs"
  
  checkOptionUniqueness (
    _DELAY_RESTS_SLURS_LONG_NAME_, _DELAY_RESTS_SLURS_SHORT_NAME_);
    
  int delayRestsSlursPresent = 0;
  
  #define _DELAY_RESTS_LIGATURES_LONG_NAME_  "delayRestsLigatures"
  #define _DELAY_RESTS_LIGATURES_SHORT_NAME_ "drligs"
  
  checkOptionUniqueness (
    _DELAY_RESTS_LIGATURES_LONG_NAME_, _DELAY_RESTS_LIGATURES_SHORT_NAME_);
    
  int delayRestsLigaturesPresent = 0;

  #define _DELAY_RESTS_WEDGES_LONG_NAME_  "delayRestsWedges"
  #define _DELAY_RESTS_WEDGES_SHORT_NAME_ "drwedges"
  
  checkOptionUniqueness (
    _DELAY_RESTS_WEDGES_LONG_NAME_, _DELAY_RESTS_WEDGES_SHORT_NAME_);
    
  int delayRestsWedgesPresent = 0;
  
  // lyrics

  #define _DONT_DISPLAY_MSR_STANZAS_LONG_NAME_  "dontDisplayMsrStanzas"
  #define _DONT_DISPLAY_MSR_STANZAS_SHORT_NAME_ "noms"
  
  checkOptionUniqueness (
    _DONT_DISPLAY_MSR_STANZAS_LONG_NAME_, _DONT_DISPLAY_MSR_STANZAS_SHORT_NAME_);
    
  int dontDisplayMsrStanzasPresent = 0;
  
  #define _KEEP_MASTER_STANZAS_LONG_NAME_  "keepMasterStanzas"
  #define _KEEP_MASTER_STANZAS_SHORT_NAME_ "kms"
  
  checkOptionUniqueness (
    _KEEP_MASTER_STANZAS_LONG_NAME_, _KEEP_MASTER_STANZAS_SHORT_NAME_);
    
  int keepMasterStanzasPresent = 0;

  // harmonies

  #define _KEEP_EMPTY_HARMONIES_VOICE_LONG_NAME_  "keepEmptyHarmoniesVoice"
  #define _KEEP_EMPTY_HARMONIES_VOICE_SHORT_NAME_ "kehv"
  
  checkOptionUniqueness (
    _KEEP_EMPTY_HARMONIES_VOICE_LONG_NAME_, _KEEP_EMPTY_HARMONIES_VOICE_SHORT_NAME_);
    
  int keepEmptyHarmoniesVoicePresent = 0;

  
  // LPSR options
  // ------------

  // trace and display

  #define _TRACE_LPSR_LONG_NAME_  "traceLpsr "
  #define _TRACE_LPSR_SHORT_NAME_ "tlpsr"
  
  checkOptionUniqueness (
    _TRACE_LPSR_LONG_NAME_, _TRACE_LPSR_SHORT_NAME_);
    
  int traceLpsrPresent = 0;

  #define _TRACE_LPSR_VISITORS_LONG_NAME_  "traceLpsrVisitors "
  #define _TRACE_LPSR_VISITORS_SHORT_NAME_ "tlpvisits"
  
  checkOptionUniqueness (
    _TRACE_LPSR_VISITORS_LONG_NAME_, _TRACE_LPSR_VISITORS_SHORT_NAME_);
    
  int traceLpsrVisitorsPresent = 0;

  #define _DISPLAY_LPSR_LONG_NAME_  "displayLpsr "
  #define _DISPLAY_LPSR_SHORT_NAME_ "lpsr"
  
  checkOptionUniqueness (
    _DISPLAY_LPSR_LONG_NAME_, _DISPLAY_LPSR_SHORT_NAME_);
    
  int displayLPSRPresent  = 0;

  #define _COMMENTS_LONG_NAME_  "comments "
  #define _COMMENTS_SHORT_NAME_ "com"
  
  checkOptionUniqueness (
    _COMMENTS_LONG_NAME_, _COMMENTS_SHORT_NAME_);
    
  int commentsPresent = 0;

  // languages

  #define _LPSR_PITCHES_LANGUAGE_LONG_NAME_  "lpsrPitchesLanguage "
  #define _LPSR_PITCHES_LANGUAGE_SHORT_NAME_ "lppl"
  
  checkOptionUniqueness (
    _LPSR_PITCHES_LANGUAGE_LONG_NAME_, _LPSR_PITCHES_LANGUAGE_SHORT_NAME_);
    
  int lpsrPitchesLanguagePresent = 0;

  #define _LPSR_CHORDS_LANGUAGE_LONG_NAME_  "lpsrChordsLanguage "
  #define _LPSR_CHORDS_LANGUAGE_SHORT_NAME_ "lpcl"
  
  checkOptionUniqueness (
    _LPSR_CHORDS_LANGUAGE_LONG_NAME_, _LPSR_CHORDS_LANGUAGE_SHORT_NAME_);
    
  int lpsrChordsLanguagePresent = 0;
  
  // time

  #define _NUMERICAL_TIME_LONG_NAME_  "numericalTime "
  #define _NUMERICAL_TIME_SHORT_NAME_ "numt"
  
  checkOptionUniqueness (
    _NUMERICAL_TIME_LONG_NAME_, _NUMERICAL_TIME_SHORT_NAME_);
    
  int numericaltimePresent = 0;

  // notes

  #define _ABSOLUTE_LONG_NAME_  "absolute "
  #define _ABSOLUTE_SHORT_NAME_ "abs"
  
  checkOptionUniqueness (
    _ABSOLUTE_LONG_NAME_, _ABSOLUTE_SHORT_NAME_);
    
  int absolutePresent = 0;

  #define _STEMS_LONG_NAME_  "stems "
  #define _STEMS_SHORT_NAME_ ""
  
  checkOptionUniqueness (
    _STEMS_LONG_NAME_, _STEMS_SHORT_NAME_);
    
  int stemsPresent = 0;

  #define _NO_AUTO_BEAMING_LONG_NAME_  "noAutoBeaming "
  #define _NO_AUTO_BEAMING_SHORT_NAME_ "noab"
  
  checkOptionUniqueness (
    _NO_AUTO_BEAMING_LONG_NAME_, _NO_AUTO_BEAMING_SHORT_NAME_);
    
  int noAutoBeamingPresent = 0;

  #define _ACCIDENTAL_STYLE_LONG_NAME_  "accidentalStyle "
  #define _ACCIDENTAL_STYLE_SHORT_NAME_ "as"
  
  checkOptionUniqueness (
    _ACCIDENTAL_STYLE_LONG_NAME_, _ACCIDENTAL_STYLE_SHORT_NAME_);
    
  int accidentalStylePresent = 0;
  
  #define _NOTE_INPUT_LINE_NUMBERS_LONG_NAME_  "noteInputLineNumbers "
  #define _NOTE_INPUT_LINE_NUMBERS_SHORT_NAME_ "niln"
  
  checkOptionUniqueness (
    _NOTE_INPUT_LINE_NUMBERS_LONG_NAME_, _NOTE_INPUT_LINE_NUMBERS_SHORT_NAME_);
    
  int noteInputLineNumbersPresent = 0;

  // bars

  #define _SHOW_ALL_BAR_NUMBERS_LONG_NAME_  "showAllBarNumbers "
  #define _SHOW_ALL_BAR_NUMBERS_SHORT_NAME_ "sabn"
  
  checkOptionUniqueness (
    _SHOW_ALL_BAR_NUMBERS_LONG_NAME_, _SHOW_ALL_BAR_NUMBERS_SHORT_NAME_);
    
  int showAllBarNumbersPresent = 0;

  #define _COMPRESS_FULL_BAR_RESTS_LONG_NAME_  "compressFullBarRests "
  #define _COMPRESS_FULL_BAR_RESTS_SHORT_NAME_ "cfbr"
  
  checkOptionUniqueness (
    _COMPRESS_FULL_BAR_RESTS_LONG_NAME_, _COMPRESS_FULL_BAR_RESTS_SHORT_NAME_);
    
  int compressFullBarRestsPresent = 0;

  // line breaks

  #define _DONT_KEEP_LINE_BREAKS_LONG_NAME_  "dontKeepLineBreaks "
  #define _DONT_KEEP_LINE_BREAKS_SHORT_NAME_ "noBreaks"
  
  checkOptionUniqueness (
    _DONT_KEEP_LINE_BREAKS_LONG_NAME_, _DONT_KEEP_LINE_BREAKS_SHORT_NAME_);
    
  int dontKeepLineBreaksPresent = 0;

  #define _BREAK_LINES_AT_INCOMPLETE_RIGHT_MEASURES_LONG_NAME_  "breakLinesAtIncompleteRightMeasures "
  #define _BREAK_LINES_AT_INCOMPLETE_RIGHT_MEASURES_SHORT_NAME_ "blairm"
  
  checkOptionUniqueness (
    _BREAK_LINES_AT_INCOMPLETE_RIGHT_MEASURES_LONG_NAME_, _BREAK_LINES_AT_INCOMPLETE_RIGHT_MEASURES_SHORT_NAME_);
    
  int breakLinesAtIncompleteRightMeasuresPresent = 0;
  
  #define _SEPARATOR_LINE_EVERY_N_MEASURES_LONG_NAME_  "separatorLineEveryNMeasures "
  #define _SEPARATOR_LINE_EVERY_N_MEASURES_SHORT_NAME_ "slenm"
  
  checkOptionUniqueness (
    _SEPARATOR_LINE_EVERY_N_MEASURES_LONG_NAME_, _SEPARATOR_LINE_EVERY_N_MEASURES_SHORT_NAME_);
    
  int separatorLineEveryNMeasuresPresent = 0;

  // staves

  #define _MODERN_TAB_LONG_NAME_  "modernTab "
  #define _MODERN_TAB_SHORT_NAME_ "mtab"
  
  checkOptionUniqueness (
    _MODERN_TAB_LONG_NAME_, _MODERN_TAB_SHORT_NAME_);
    
  int modernTabPresent = 0;

  // midi

  #define _MIDI_TEMPO_LONG_NAME_  "midiTempo "
  #define _MIDI_TEMPO_SHORT_NAME_ "mt"
  
  checkOptionUniqueness (
    _MIDI_TEMPO_LONG_NAME_, _MIDI_TEMPO_SHORT_NAME_);
    
  int midiTempoPresent = 0;

  #define _NO_MIDI_LONG_NAME_  "noMidi "
  #define _NO_MIDI_SHORT_NAME_ ""
  
  checkOptionUniqueness (
    _NO_MIDI_LONG_NAME_, _NO_MIDI_SHORT_NAME_);
    
  int dontGenerateMidiCommandPresent = 0;

  // LilyPond code generation

  #define _TUPLETS_ON_A_LINE_LONG_NAME_  "tupletsOnALine "
  #define _TUPLETS_ON_A_LINE_SHORT_NAME_ "toal"
  
  checkOptionUniqueness (
    _TUPLETS_ON_A_LINE_LONG_NAME_, _TUPLETS_ON_A_LINE_SHORT_NAME_);
    
  int tupletsOnALinePresent = 0;

  #define _DELAYED_ORNAMENTS_FRACTION_LONG_NAME_  "delayedOrnamentsFraction "
  #define _DELAYED_ORNAMENTS_FRACTION_SHORT_NAME_ "dof"
  
  checkOptionUniqueness (
    _DELAYED_ORNAMENTS_FRACTION_LONG_NAME_, _DELAYED_ORNAMENTS_FRACTION_SHORT_NAME_);
    
  int delayedOrnamentFractionPresent    = 0;

  #define _DONT_GENERATE_LILYPOND_CODE_LONG_NAME_  "dontGenerateLilyPondCode "
  #define _DONT_GENERATE_LILYPOND_CODE_SHORT_NAME_ "nolpc"
  
  checkOptionUniqueness (
    _DONT_GENERATE_LILYPOND_CODE_LONG_NAME_, _DONT_GENERATE_LILYPOND_CODE_SHORT_NAME_);
    
  int dontGenerateLilyPondCodePresent = 0;

  #define _DONT_GENERATE_LILYPOND_LYRICS_LONG_NAME_  "dontGenerateLilyPondLyrics "
  #define _DONT_GENERATE_LILYPOND_LYRICS_SHORT_NAME_ "nolpl"
  
  checkOptionUniqueness (
    _DONT_GENERATE_LILYPOND_LYRICS_LONG_NAME_, _DONT_GENERATE_LILYPOND_LYRICS_SHORT_NAME_);
    
  int dontGenerateLilyPondLyricsPresent = 0;


  // long_options data structure
  static struct option long_options [] =
    {
    /* These options set a flag. */

    // General options
    // ---------------

    // version

    {
      _VERSION_LONG_NAME_,
      no_argument, &versionPresent, 1
    },
    {
      _VERSION_SHORT_NAME_,
      no_argument, &versionPresent, 1
    },

    // help

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

    // output file

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

    {
      _TRACE_GENERAL_LONG_NAME_,
      no_argument, &traceGeneralPresent, 1
    },
    {
      _TRACE_GENERAL_SHORT_NAME_,
      no_argument, &traceGeneralPresent, 1
    },

    // specific trace    

    {
      _TRACE_DIVISIONS_LONG_NAME_,
      no_argument, &traceDivisionsPresent, 1
    },
    {
      _TRACE_DIVISIONS_SHORT_NAME_,
      no_argument, &traceDivisionsPresent, 1
    },
    
    {
      _TRACE_PARTS_LONG_NAME_,
      no_argument, &tracePartsPresent, 1
    },
    {
      _TRACE_PARTS_SHORT_NAME_,
      no_argument, &tracePartsPresent, 1
    },
    
    {
      _TRACE_VOICES_LONG_NAME_,
      no_argument, &traceVoicesPresent, 1
    },
    {
      _TRACE_VOICES_SHORT_NAME_,
      no_argument, &traceVoicesPresent, 1
    },
    
    {
      _TRACE_SEGMENTS_LONG_NAME_,
      no_argument, &traceSegmentsPresent, 1
    },
    {
      _TRACE_SEGMENTS_SHORT_NAME_,
      no_argument, &traceSegmentsPresent, 1
    },
    
    {
      _TRACE_REPEATS_LONG_NAME_,
      no_argument, &traceRepeatsPresent, 1
    },
    {
      _TRACE_REPEATS_SHORT_NAME_,
      no_argument, &traceRepeatsPresent, 1
    },
    
    {
      _TRACE_MEASURES_LONG_NAME_,
      no_argument, &traceMeasuresPresent, 1
    },
    {
      _TRACE_MEASURES_SHORT_NAME_,
      no_argument, &traceMeasuresPresent, 1
    },
    
    {
      _TRACE_NOTES_LONG_NAME_,
      no_argument, &traceNotesPresent, 1
    },
    {
      _TRACE_NOTES_SHORT_NAME_,
      no_argument, &traceNotesPresent, 1
    },
    
    {
      _TRACE_TREMOLOS_LONG_NAME_,
      no_argument, &traceTremolosPresent, 1
    },
    {
      _TRACE_TREMOLOS_SHORT_NAME_,
      no_argument, &traceTremolosPresent, 1
    },
    
    {
      _TRACE_CHORDS_LONG_NAME_,
      no_argument, &traceChordsPresent, 1
    },
    {
      _TRACE_CHORDS_SHORT_NAME_,
      no_argument, &traceChordsPresent, 1
    },
    
    {
      _TRACE_TUPLETS_LONG_NAME_,
      no_argument, &traceTupletsPresent, 1
    },
    {
      _TRACE_TUPLETS_SHORT_NAME_,
      no_argument, &traceTupletsPresent, 1
    },
    
    {
      _TRACE_GRACENOTES_LONG_NAME_,
      no_argument, &traceGracenotesPresent, 1
    },
    {
      _TRACE_GRACENOTES_SHORT_NAME_,
      no_argument, &traceGracenotesPresent, 1
    },
    
    {
      _TRACE_LYRICS_LONG_NAME_,
      no_argument, &traceLyricsPresent, 1
    },
    {
      _TRACE_LYRICS_SHORT_NAME_,
      no_argument, &traceLyricsPresent, 1
    },
    
    {
      _TRACE_HARMONIES_LONG_NAME_,
      no_argument, &traceHarmoniesPresent, 1
    },
    {
      _TRACE_HARMONIES_SHORT_NAME_,
      no_argument, &traceHarmoniesPresent, 1
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

    {
      _CPU_USAGE_LONG_NAME_,
      no_argument, &displayCPUusagePresent, 1
    },
    {
      _CPU_USAGE_SHORT_NAME_,
      no_argument, &displayCPUusagePresent, 1
    },

    // MSR options
    // -----------

    // trace and display

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

    // trace and display

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
      "lpsr",
      no_argument, &displayLPSRPresent, 1},
    {
      "displayLPSR",
      no_argument, &displayLPSRPresent, 1
    },

    // languages

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

        // version
        
        {
        if (versionPresent) {
          idtr++;
          
          cerr <<
            endl <<
            idtr <<
              "This is xml2lilypond version " <<
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
        if (helpMsrPresent) {
          printUsage (kMsrHelp, 0);
          break;
        }
        if (helpLpsrPresent) {
          printUsage (kLpsrHelp, 0);
          break;
        }

        // output file
        
        if (outputFilePresent) {
          outputFileName = optarg;
          gGeneralOptions->fOutputFileName = outputFileName;
          
          {
            stringstream s;
  
            s <<
              "--"_OUTPUT_FILE_LONG_NAME_ << " " << outputFileName << " ";
            gGeneralOptions->fCommandLineLongOptions +=
              s.str();
          }
          
          {
            stringstream s;
  
            s <<
              "--"_OUTPUT_FILE_SHORT_NAME_ << " " << outputFileName << " ";
            gGeneralOptions->fCommandLineShortOptions +=
              s.str();
          }
          
          outputFilePresent = false;
        }
        
        if (autoOutputFilePresent) {
          gGeneralOptions->fAutoOutputFile = true;
          
          gGeneralOptions->fCommandLineLongOptions +=
            "--"_AUTO_OUTPUT_FILE_LONG_NAME_" ";
          gGeneralOptions->fCommandLineShortOptions +=
            "--"_AUTO_OUTPUT_FILE_SHORT_NAME_" ";
            
          autoOutputFilePresent = false;
        }

        // trace and display
        
        if (traceGeneralPresent) {
          gGeneralOptions->fTraceGeneral = true;
          
          gGeneralOptions->fCommandLineLongOptions +=
            "--"_TRACE_GENERAL_LONG_NAME_" ";
          gGeneralOptions->fCommandLineShortOptions +=
            "--"_TRACE_GENERAL_SHORT_NAME_" ";
            
          traceGeneralPresent = false;
        }

        if (traceDetailedPresent) {
          // optarg contains the measure numbers set specification
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceDetailed = true;

          char*        measuresSpec = optarg;

          gGeneralOptions->fTraceAllMeasureNumbersSet =
            decipherNumbersSetSpecification (
              measuresSpec, false); // 'true' to debug it

          {
            stringstream s;
  
            s <<
               "--"_TRACE_DETAILED_LONG_NAME_" " << measuresSpec << " ";
               
            gGeneralOptions->fCommandLineLongOptions +=
              s.str();
          }

          {
            stringstream s;
  
            s <<
               "--"_TRACE_DETAILED_SHORT_NAME_" " << measuresSpec << " ";
               
            gGeneralOptions->fCommandLineShortOptions +=
              s.str();
           }
                  
          traceDetailedPresent = false;
        }

        // CPU usage

        if (displayCPUusagePresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fDisplayCPUusage = true;
          
          gGeneralOptions->fCommandLineLongOptions +=
            "--"_CPU_USAGE_LONG_NAME_" ";
          gGeneralOptions->fCommandLineShortOptions +=
            "--"_CPU_USAGE_SHORT_NAME_" ";
            
          displayCPUusagePresent = false;
        }
        
        // specific trace    

        if (traceDivisionsPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceDivisions = true;
          
          
          gGeneralOptions->fCommandLineLongOptions +=
            "--"_TRACE_DIVISIONS_LONG_NAME_" ";
          gGeneralOptions->fCommandLineShortOptions +=
            "--"_TRACE_DIVISIONS_SHORT_NAME_" ";
            
          gGeneralOptions->fCommandLineLongOptions +=
            "--traceDivisions ";
            
          traceDivisionsPresent = false;
        }
        
        if (tracePartsPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceParts = true;
          
          gGeneralOptions->fCommandLineLongOptions +=
            "--"_TRACE_PARTS_LONG_NAME_" ";
          gGeneralOptions->fCommandLineShortOptions +=
            "--"_TRACE_PARTS_SHORT_NAME_" ";
            
          gGeneralOptions->fCommandLineLongOptions +=
            "--traceparts ";

          tracePartsPresent = false;
        }
        if (traceVoicesPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceVoices = true;
          
          gGeneralOptions->fCommandLineLongOptions +=
            "--"_TRACE_VOICES_LONG_NAME_" ";
          gGeneralOptions->fCommandLineShortOptions +=
            "--"_TRACE_VOICES_SHORT_NAME_" ";
            
          gGeneralOptions->fCommandLineLongOptions +=
            "--traceVoices ";

          traceVoicesPresent = false;
        }
        if (traceSegmentsPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceSegments = true;
          
          gGeneralOptions->fCommandLineLongOptions +=
            "--"_TRACE_SEGMENTS_LONG_NAME_" ";
          gGeneralOptions->fCommandLineShortOptions +=
            "--"_TRACE_SEGMENTS_SHORT_NAME_" ";
            
          gGeneralOptions->fCommandLineLongOptions +=
            "--traceSegments ";

          traceSegmentsPresent = false;
        }
        if (traceRepeatsPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceRepeats = true;
          
          gGeneralOptions->fCommandLineLongOptions +=
            "--"_TRACE_REPEATS_LONG_NAME_" ";
          gGeneralOptions->fCommandLineShortOptions +=
            "--"_TRACE_REPEATS_SHORT_NAME_" ";
            
          gGeneralOptions->fCommandLineLongOptions +=
            "--traceRepeats ";

          traceRepeatsPresent = false;
        }
        if (traceMeasuresPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceMeasures = true;
          
          gGeneralOptions->fCommandLineLongOptions +=
            "--"_TRACE_MEASURES_LONG_NAME_" ";
          gGeneralOptions->fCommandLineShortOptions +=
            "--"_TRACE_MEASURES_SHORT_NAME_" ";

          traceMeasuresPresent = false;
        }
        if (traceNotesPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceNotes = true;
          
          gGeneralOptions->fCommandLineLongOptions +=
            "--"_TRACE_NOTES_LONG_NAME_" ";
          gGeneralOptions->fCommandLineShortOptions +=
            "--"_TRACE_NOTES_SHORT_NAME_" ";
            
          traceNotesPresent = false;
        }
        if (traceTremolosPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceTremolos = true;
          
          gGeneralOptions->fCommandLineLongOptions +=
            "--"_TRACE_TREMOLOS_LONG_NAME_" ";
          gGeneralOptions->fCommandLineShortOptions +=
            "--"_TRACE_TREMOLOS_SHORT_NAME_" ";
            
          traceTremolosPresent = false;
        }
        if (traceChordsPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceChords = true;
          
          gGeneralOptions->fCommandLineLongOptions +=
            "--"_TRACE_CHORDS_LONG_NAME_" ";
          gGeneralOptions->fCommandLineShortOptions +=
            "--"_TRACE_CHORDS_SHORT_NAME_" ";
            
          traceChordsPresent = false;
        }
        if (traceTupletsPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceTuplets = true;
          
          gGeneralOptions->fCommandLineLongOptions +=
            "--"_TRACE_TUPLETS_LONG_NAME_" ";
          gGeneralOptions->fCommandLineShortOptions +=
            "--"_TRACE_TUPLETS_SHORT_NAME_" ";
                        
          traceTupletsPresent = false;
        }
        if (traceGracenotesPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceGracenotes = true;
          
          gGeneralOptions->fCommandLineLongOptions +=
            "--"_TRACE_GRACENOTES_LONG_NAME_" ";
          gGeneralOptions->fCommandLineShortOptions +=
            "--"_TRACE_GRACENOTES_SHORT_NAME_" ";
            
          traceGracenotesPresent = false;
        }

        if (traceLyricsPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceLyrics = true;
          
          gGeneralOptions->fCommandLineLongOptions +=
            "--"_TRACE_LYRICS_LONG_NAME_" ";
          gGeneralOptions->fCommandLineShortOptions +=
            "--"_TRACE_LYRICS_SHORT_NAME_" ";
            
          traceLyricsPresent = false;
        }
        
        if (traceHarmoniesPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gGeneralOptions->fTraceHarmonies = true;
          
          gGeneralOptions->fCommandLineLongOptions +=
            "--"_TRACE_HARMONIES_LONG_NAME_" ";
          gGeneralOptions->fCommandLineShortOptions +=
            "--"_TRACE_HARMONIES_SHORT_NAME_" ";
            
          traceHarmoniesPresent = false;
        }
        
        // MSR options
        // -----------
        
        // trace and display

        if (traceMSRVisitorsPresent) {
          gMsrOptions->fTraceMsrVisitors = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--traceMSRVisitors ";
          traceMSRVisitorsPresent = false;
        }

        // languages

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
          
          gGeneralOptions->fCommandLineLongOptions +=
            "--msrPitchesLanguage " +
            optargAsString +
            " ";
          msrPitchesLanguagePresent = false;
          }
             
        // parts

        if (staffRelativeVoiceNumbersPresent) {
          gMsrOptions->fCreateStaffRelativeVoiceNumbers = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--staffRelativeVoiceNumbers ";
          staffRelativeVoiceNumbersPresent = false;
        }
        
        if (dontDisplayMsrStanzasPresent) {
          gMsrOptions->fDontDisplayMsrStanzas = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--dontGenerateMSRStanzas ";
          dontDisplayMsrStanzasPresent = false;
        }
        
        if (delayRestsDynamicsPresent) {
          gMsrOptions->fDelayRestsDynamics = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--delayRestsDynamics ";
          delayRestsDynamicsPresent = false;
        }
        
        if (delayRestsWordsPresent) {
          gMsrOptions->fDelayRestsWords = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--delayRestsWords ";
          delayRestsWordsPresent = false;
        }
        
        if (delayRestsSlursPresent) {
          gMsrOptions->fDelayRestsSlurs = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--delayRestsSlurs ";
          delayRestsSlursPresent = false;
        }
        
        if (delayRestsLigaturesPresent) {
          gMsrOptions->fDelayRestsLigatures = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--delayRestsLigatures ";
          delayRestsLigaturesPresent = false;
        }
        
        if (delayRestsWedgesPresent) {
          gMsrOptions->fDelayRestsWedges = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--delayRestsWedges ";
          delayRestsWedgesPresent = false;
        }
        
        if (keepMasterVoicesPresent) {
          gMsrOptions->fKeepMasterVoices = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--keepMasterVoices ";
          keepMasterVoicesPresent = false;
        }
        if (keepMasterStanzasPresent) {
          gMsrOptions->fKeepMasterStanzas = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--keepMasterStanzas ";
          keepMasterStanzasPresent = false;
        }

        if (displayMSRPresent) {
          gMsrOptions->fDisplayMsr = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--displayMSR ";
          displayMSRPresent = false;
        }

        if (displayMSRSummaryPresent) {
          gMsrOptions->fDisplayMsrSummary = true;
          gLpsrOptions->fDontGenerateLilyPondCode = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--displayScoreSummary ";
          displayMSRSummaryPresent = false;
        }
        
        if (partNamePresent) {
          // optarg contains the part name
          char*        partNameSpec = optarg;
          stringstream s;

          s <<
            "--partName" << " \"" << partNameSpec << "\" ";

          gGeneralOptions->fCommandLineLongOptions +=
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

        // trace and display

        if (traceLpsrPresent) {
          gGeneralOptions->fTraceGeneral = true;
          gLpsrOptions->fTraceLpsr = true;
          
          gGeneralOptions->fCommandLineLongOptions +=
            "--"_TRACE_LPSR_LONG_NAME_" ";
          gGeneralOptions->fCommandLineShortOptions +=
            "--"_TRACE_LPSR_SHORT_NAME_" ";
            
          traceLpsrPresent = false;
        }

        if (traceLpsrVisitorsPresent) {
          gLpsrOptions->fTraceLpsrVisitors = true;

          gGeneralOptions->fCommandLineLongOptions +=
            "--"_TRACE_LPSR_VISITORS_LONG_NAME_" ";
          gGeneralOptions->fCommandLineShortOptions +=
            "--"_TRACE_LPSR_VISITORS_SHORT_NAME_" ";
            
          traceLpsrVisitorsPresent = false;
        }

        if (commentsPresent) {
          gLpsrOptions->fGenerateComments = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--comments ";
          commentsPresent = false;
        }

        // languages

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
          
          gGeneralOptions->fCommandLineLongOptions +=
            "--"_TRACE_LPSR_LONG_NAME_" ";
          gGeneralOptions->fCommandLineShortOptions +=
            "--"_TRACE_LPSR_SHORT_NAME_" ";

          gGeneralOptions->fCommandLineLongOptions +=
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
          
          gGeneralOptions->fCommandLineLongOptions +=
            "--lpsrChordsLanguage " +
            optargAsString +
            " ";
          lpsrChordsLanguagePresent = false;
          }
             
        if (displayLPSRPresent) {
          gLpsrOptions->fDisplayLpsr = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--displayLpsr ";
          displayLPSRPresent = false;
        }

        if (absolutePresent) {
          gLpsrOptions->fGenerateAbsoluteOctaves = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--absolute ";
          absolutePresent = false;
        }

        if (showAllBarNumbersPresent) {
          gLpsrOptions->fShowAllBarNumbers = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--showAllBarNumbers ";
          showAllBarNumbersPresent = false;
        }

        if (compressFullBarRestsPresent) {
          gLpsrOptions->fCompressFullBarRests = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--compressFullBarRests ";
          compressFullBarRestsPresent = false;
        }

        if (tupletsOnALinePresent) {
          gLpsrOptions->fTupletsOnALine = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--tupletsOnALine ";
          tupletsOnALinePresent = false;
        }

        if (breakLinesAtIncompleteRightMeasuresPresent) {
          gLpsrOptions->fBreakLinesAtIncompleteRightMeasures = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--breakLinesAtIncompleteRightMeasures ";
          breakLinesAtIncompleteRightMeasuresPresent = false;
        }

        if (separatorLineEveryNMeasuresPresent) {
          gLpsrOptions->fSeparatorLineEveryNMeasures = true;
          gLpsrOptions->fSeparatorLineEveryNMeasuresValue =
            atoi (optarg);
          gGeneralOptions->fCommandLineLongOptions +=
            "--separatorLineEveryNMeasures ";
          separatorLineEveryNMeasuresPresent = false;
        }

        if (dontKeepLineBreaksPresent) {
          gLpsrOptions->fDontKeepLineBreaks = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--dontKeepLineBreaks ";
          dontKeepLineBreaksPresent = false;
        }

        if (numericaltimePresent) {
          gLpsrOptions->fGenerateNumericalTime = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--numericalTime ";
          numericaltimePresent = false;
        }
        if (stemsPresent) {
          gLpsrOptions->fGenerateStems = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--stems ";
          stemsPresent = false;
        }
        if (noAutoBeamingPresent) {
          gLpsrOptions->fNoAutoBeaming = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--noAutoBeaming ";
          noAutoBeamingPresent = false;
        }

        if (noteInputLineNumbersPresent) {
          gLpsrOptions->fGenerateInputLineNumbers = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--generateInputLineNumbers ";
          noteInputLineNumbersPresent = false;
        }
        
        if (modernTabPresent) {
          gLpsrOptions->fModernTab = true;
          gGeneralOptions->fCommandLineLongOptions +=
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
          
          gGeneralOptions->fCommandLineLongOptions +=
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
          
          gGeneralOptions->fCommandLineLongOptions +=
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
          
          gGeneralOptions->fCommandLineLongOptions +=
            s.str();
          midiTempoPresent = false;
        }
        
        if (dontGenerateLilyPondLyricsPresent) {
          gLpsrOptions->fDontGenerateLilyPondLyrics = true;
          gGeneralOptions->fCommandLineLongOptions +=
            "--dontGenerateLilyPondLyrics ";
          dontGenerateLilyPondLyricsPresent = false;
        }
        
        if (dontGenerateLilyPondCodePresent) {
          gLpsrOptions->fDontGenerateLilyPondCode = true;
          gGeneralOptions->fCommandLineLongOptions +=
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
    cerr <<
      endl <<
      idtr <<
        "The command line options and arguments have been analyzed" <<
        endl;

  cerr << idtr <<
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

  gGeneralOptionsUserChoices = msrGeneralOptions::create ();
  assert(gGeneralOptionsUserChoices != 0);

  gGeneralOptions =
    gGeneralOptionsUserChoices;

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

  // prepare for measure detailed trace
  // ------------------------------------------------------

  gGeneralOptionsWithDetailedTrace =
    gGeneralOptions->
      createCloneWithDetailedTrace ();

  // welcome message
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
