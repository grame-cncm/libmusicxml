/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <climits>      /* INT_MIN, INT_MAX */
#include <iomanip>      // setw, set::precision, ...

#include "lpsrOptions.h"


using namespace std;

namespace MusicXML2 
{

// useful shortcut macros
#define idtr indenter::gIndenter
#define tab  indenter::gIndenter.getSpacer ()

//_______________________________________________________________________________
S_lpsrOptions gLpsrOptions;
S_lpsrOptions gLpsrOptionsUserChoices;
S_lpsrOptions gLpsrOptionsWithDetailedTrace;

S_lpsrOptions lpsrOptions::create ()
{
  lpsrOptions* o = new lpsrOptions();
  assert(o!=0);
  return o;
}

lpsrOptions::lpsrOptions()
{
  initializeLpsrOptions (false);
}

lpsrOptions::~lpsrOptions()
{}

void lpsrOptions::initializeLpsrOptions (
  bool boolOptionsInitialValue)
{
  // trace and display
  
  fTraceLpsr         = boolOptionsInitialValue;
  
  fTraceLpsrVisitors = boolOptionsInitialValue;

  fDisplayLpsr       = boolOptionsInitialValue;
  
  // Scheme functions
  fTraceSchemeFunctions = boolOptionsInitialValue;

  // languages
  
  if (! setLpsrQuarterTonesPitchesLanguage ("nederlands")) {
    stringstream s;

    s <<
      "INTERNAL INITIALIZATION ERROR: "
      "LPSR pitches language 'nederlands' is unknown" <<
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
}

S_lpsrOptions lpsrOptions::createCloneWithDetailedTrace ()
{
  S_lpsrOptions
    clone =
      lpsrOptions::create ();

  // trace and display
  // --------------------------------------
    
  clone->fTraceLpsr =
    true;

  clone->fTraceLpsrVisitors =
    true;

  clone->fDisplayLpsr =
    true;
    
  // Scheme functions
  clone->fTraceSchemeFunctions =
    true;
  
  // languages
  // --------------------------------------

  clone->fLpsrQuarterTonesPitchesLanguage =
    fLpsrQuarterTonesPitchesLanguage;

  clone->fLpsrChordsLanguage =
    fLpsrChordsLanguage;

  return clone;
}

bool lpsrOptions::setLpsrQuarterTonesPitchesLanguage (string language)
{
  // is language in the note names languages map?
  map<string, msrQuarterTonesPitchesLanguage>::const_iterator
    it =
      gQuarterTonesPitchesLanguagesMap.find (language);
        
  if (it == gQuarterTonesPitchesLanguagesMap.end ()) {
    // no, language is unknown in the map    
    return false;
  }

  fLpsrQuarterTonesPitchesLanguage = (*it).second;
  
  return true;
}

bool lpsrOptions::setLpsrChordsLanguage (string language)
{
  // is language in the chords languages map?
  map<string, lpsrChordsLanguage>::const_iterator
    it =
      gLpsrChordsLanguagesMap.find (language);
        
  if (it == gLpsrChordsLanguagesMap.end ()) {
    // no, language is unknown in the map    
    return false;
  }

  fLpsrChordsLanguage = (*it).second;
  
  return true;
}

void lpsrOptions::printLpsrOptionsHelp ()
{
  idtr++;

  cerr <<
    idtr << "LPSR:" <<
    endl <<
    idtr << "----" <<
    endl <<
    endl;

  idtr++;

  // trace and display
  // --------------------------------------
  cerr <<
    idtr << "Trace and display:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _TRACE_LPSR_SHORT_NAME_ ", --" _TRACE_LPSR_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Write a trace of the LPSR graphs visiting activity to standard error." <<
      endl <<
    endl <<
      
    idtr <<
      "--t" _TRACE_LPSR_VISITORS_SHORT_NAME_ ", --" _TRACE_LPSR_VISITORS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Write a trace of the LPSR graphs visiting activity to standard error." <<
      endl <<
    endl;

  cerr <<
    idtr <<
      "--" _DISPLAY_LPSR_SHORT_NAME_ ", --" _DISPLAY_LPSR_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Write the contents of the LPSR data to standard error." << 
      endl <<
    endl;

  idtr--;

  cerr <<
    idtr <<
      "--" _TRACE_SCHEME_FUNCTIONS_SHORT_NAME_ ", --" _TRACE_SCHEME_FUNCTIONS_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Write a trace of the activity regarding Scheme functions to standard error." << 
      endl <<
    endl;

  idtr--;

  // languages
  // --------------------------------------
  cerr <<
    idtr << "Languages:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
   idtr <<
      "--" _LPSR_PITCHES_LANGUAGE_SHORT_NAME_ ", --" _LPSR_PITCHES_LANGUAGE_LONG_NAME_ " language" << 
      endl <<
    idtr << tab << tab << tab <<
      "Use 'language' to display note pitches in the LPSR logs and views," <<
      endl <<
    idtr << tab << tab << tab <<
      "as well as in the generated LilyPond code." << 
      endl <<
    idtr << tab << tab << tab <<
      "The 12 LilyPond pitches languages are available:" << 
      endl <<
    idtr << tab << tab << tab <<
      "nederlands, catalan, deutsch, english, espanol, français, " << 
      endl <<
    idtr << tab << tab << tab <<
      "italiano, norsk, portugues, suomi, svenska and vlaams." << 
      endl <<
    idtr << tab << tab << tab <<
      "The default is to use 'nederlands'." << 
      endl <<
    endl;

  cerr <<
    idtr <<
      "--" _LPSR_CHORDS_LANGUAGE_SHORT_NAME_ " , --" _LPSR_CHORDS_LANGUAGE_LONG_NAME_ " language" << 
      endl <<
    idtr << tab << tab << tab <<
      "Use 'language' to display chord names, their root and bass notes," << 
      endl <<
    idtr << tab << tab << tab <<
      "in the LPSR logs and views and the generated LilyPond code." << 
      endl <<
    idtr << tab << tab << tab <<
      "The 4 LilyPond chords languages are available:" << 
      endl <<
    idtr << tab << tab << tab <<
      "german, semiGerman, italian and french." << 
      endl <<
    idtr << tab << tab << tab <<
      "The default used by LilyPond is Ignatzek's jazz-like, english naming." << 
      endl <<
    endl;

  idtr--;
    

  idtr--;
  
  idtr--;
}

void lpsrOptions::printLpsrOptionsValues (int fieldWidth)
{
  cerr << idtr <<
    "The LPSR options are:" <<
    endl;

  idtr++;
  
  // trace and display
  // --------------------------------------
  cerr <<
    idtr << "Trace and display:" <<
    endl;

  idtr++;
  
  cerr << left <<
    idtr <<
      setw(fieldWidth) << "traceLpsr" << " : " <<
      booleanAsString (fTraceLpsr) <<
      endl <<
    
    idtr <<
      setw(fieldWidth) << "traceLpsrVisitors" << " : " <<
      booleanAsString (fTraceLpsrVisitors) <<
      endl <<

    idtr << setw(fieldWidth) << "displayLpsr" << " : " <<
      booleanAsString (fDisplayLpsr) <<
      endl <<
      
    // Scheme functions
    idtr <<
      setw(fieldWidth) << "traceSchemeFunctions" << " : " <<
      booleanAsString (fTraceSchemeFunctions) <<
      endl;

  idtr--;
  
  // languages
  // --------------------------------------
  cerr <<
    idtr << "Languages:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw(fieldWidth) << "lpsrQuarterTonesPitchesLanguage" << " : \"" <<
      msrQuarterTonesPitchesLanguageAsString (
        fLpsrQuarterTonesPitchesLanguage) <<
        "\"" <<
      endl <<

    idtr << setw(fieldWidth) << "lpsrChordsLanguage" << " : \"" <<
      lpsrChordsLanguageAsString (
        fLpsrChordsLanguage) <<
        "\"" <<
      endl;  

  idtr--;

  
  idtr--;
}

//_______________________________________________________________________________
S_lilypondOptions gLilypondOptions;
S_lilypondOptions gLilypondOptionsUserChoices;
S_lilypondOptions gLilypondOptionsWithDetailedTrace;

S_lilypondOptions lilypondOptions::create ()
{
  lilypondOptions* o = new lilypondOptions();
  assert(o!=0);
  return o;
}

lilypondOptions::lilypondOptions()
{
  initializeLilypondOptions (false);
}

lilypondOptions::~lilypondOptions()
{}

void lilypondOptions::initializeLilypondOptions (
  bool boolOptionsInitialValue)
{
  // time
  
  fNumericalTime = boolOptionsInitialValue;

  // notes
  
  fAbsoluteOctaves  = boolOptionsInitialValue;

  fStems          = boolOptionsInitialValue;
  fNoAutoBeaming  = boolOptionsInitialValue;
  
  fRomanStringNumbers = boolOptionsInitialValue;
  fAvoidOpenString    = boolOptionsInitialValue;
  
  fAccidentalStyle = "";

  fCompressMultiMeasureRests = boolOptionsInitialValue;

  fInputLineNumbers = boolOptionsInitialValue;

  // bars
  
  fShowAllBarNumbers = boolOptionsInitialValue;

  // line breaks
  
  fDontKeepLineBreaks                  = boolOptionsInitialValue;

  fBreakLinesAtIncompleteRightMeasures = boolOptionsInitialValue;
  
  fSeparatorLineEveryNMeasures         = boolOptionsInitialValue;
  fSeparatorLineEveryNMeasuresValue    = INT_MAX;
  
  // staves
  
  fModernTab = boolOptionsInitialValue;

  // tuplets
  
  fTupletsOnALine = boolOptionsInitialValue;
  
  // repeats
  
  fRepeatBrackets = boolOptionsInitialValue;

  // ornaments
  
  fDelayedOrnamentFractionNumerator   = 2;
  fDelayedOrnamentFractionDenominator = 3;

  // code generation

  fComments            = boolOptionsInitialValue;

  fGlobal              = boolOptionsInitialValue;
  
  fDisplayMusic        = boolOptionsInitialValue;
    
  fNoLilypondCode      = boolOptionsInitialValue;
  
  fNoLilypondLyrics    = boolOptionsInitialValue;
  
  fLilypondCompileDate = boolOptionsInitialValue;
    
  // midi

  fMidiTempoDuration  = "4";
  fMidiTempoPerSecond = 100;
  
  fNoMidiCommand = boolOptionsInitialValue;
  
  
  // JMI ???

  fKeepStaffSize = boolOptionsInitialValue;

  fSilentVoices = boolOptionsInitialValue;


  // register the LilyPond accidental styles
  
  fLilypondAccidentalStyles.insert ("voice");
  fLilypondAccidentalStyles.insert ("modern");
  fLilypondAccidentalStyles.insert ("modern-cautionary");
  fLilypondAccidentalStyles.insert ("modern-voice");
  fLilypondAccidentalStyles.insert ("modern-voice-cautionary");
  fLilypondAccidentalStyles.insert ("piano");
  fLilypondAccidentalStyles.insert ("piano-cautionary");
  fLilypondAccidentalStyles.insert ("neo-modern");
  fLilypondAccidentalStyles.insert ("neo-modern-cautionary");
  fLilypondAccidentalStyles.insert ("neo-modern-voice");
  fLilypondAccidentalStyles.insert ("neo-modern-voice-cautionary");
  fLilypondAccidentalStyles.insert ("dodecaphonic");
  fLilypondAccidentalStyles.insert ("dodecaphonic-no-repeat");
  fLilypondAccidentalStyles.insert ("dodecaphonic-first");
  fLilypondAccidentalStyles.insert ("teaching");
  fLilypondAccidentalStyles.insert ("no-reset");
  fLilypondAccidentalStyles.insert ("forget");

  /* JMI
  for (
    set<string>::const_iterator i=fLilypondAccidentalStyles.begin();
    i!=fLilypondAccidentalStyles.end();
    i++) {
      cout << (*i) << endl;
  } // for
  */
}

S_lilypondOptions lilypondOptions::createCloneWithDetailedTrace ()
{
  S_lilypondOptions
    clone =
      lilypondOptions::create ();

  // time
  
  clone->fNumericalTime =
    fNumericalTime;

  // notes
  
  clone->fAbsoluteOctaves =
    fAbsoluteOctaves;

  clone->fStems =
    fStems;

  clone->fNoAutoBeaming =
    fNoAutoBeaming;

  clone->fRomanStringNumbers =
    fRomanStringNumbers;

  clone->fAvoidOpenString =
    fAvoidOpenString;

  clone->fAccidentalStyle =
    fAccidentalStyle;

  clone->fCompressMultiMeasureRests =
    fCompressMultiMeasureRests;

  clone->fInputLineNumbers =
    true;

  // bars
  
  clone->fShowAllBarNumbers =
    true;

  // line breaks

  clone->fDontKeepLineBreaks =
    fDontKeepLineBreaks;

  clone->fBreakLinesAtIncompleteRightMeasures =
    fBreakLinesAtIncompleteRightMeasures;

  clone->fSeparatorLineEveryNMeasures =
    fSeparatorLineEveryNMeasures;
  clone->fSeparatorLineEveryNMeasuresValue =
    fSeparatorLineEveryNMeasuresValue;

  // staves
  
  clone->fModernTab =
    fModernTab;
  
  // tuplets
    
  clone->fTupletsOnALine =
    fTupletsOnALine;
  
  // repeats
  
  clone->fRepeatBrackets =
    fRepeatBrackets;
  
  // ornaments
  
  clone->fDelayedOrnamentFractionNumerator =
    fDelayedOrnamentFractionNumerator;
  clone->fDelayedOrnamentFractionDenominator =
    fDelayedOrnamentFractionDenominator;
  
  // code generation

  clone->fComments =
    true;
  
  clone->fGlobal =
    fGlobal;
  
  clone->fDisplayMusic =
    fDisplayMusic;  
  
  clone->fNoLilypondCode =
    fNoLilypondCode;
    
  clone->fNoLilypondLyrics =
    fNoLilypondLyrics;
    
  clone->fLilypondCompileDate =
    fLilypondCompileDate;
    
  // midi
  
  clone->fMidiTempoDuration =
    fMidiTempoDuration;
  clone->fMidiTempoPerSecond =
    fMidiTempoPerSecond;
    
  clone->fNoMidiCommand =
    fNoMidiCommand;
    
// JMI ???

  clone->fKeepStaffSize =
    fKeepStaffSize;
    
  clone->fSilentVoices =
    fSilentVoices;

  return clone;
}

bool lilypondOptions::setAccidentalStyle (string accidentalStyle)
{
  // is accidentalStyle in the accidental styles set?
  set<string>::const_iterator
    it =
      fLilypondAccidentalStyles.find (accidentalStyle);
        
  if (it == fLilypondAccidentalStyles.end ()) {
    // no, accidentalStyle is unknown
    return false;
  }

  fAccidentalStyle = accidentalStyle;
  
  return true;
} 

void lilypondOptions::printLilypondOptionsHelp ()
{
  idtr++;

  cerr <<
    idtr << "LilyPond:" <<
    endl <<
    idtr << "--------" <<
    endl <<
    endl;

  idtr++;

  // time
  // --------------------------------------
  cerr <<
    idtr << "Time:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _NUMERICAL_TIME_SHORT_NAME_ ", --" _NUMERICAL_TIME_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Generate numerical time signatures, such as '4/4' instead of 'C'." << 
      endl <<
    endl;

  idtr--;

  // notes
  // --------------------------------------
  cerr <<
    idtr << "Notes:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _ABSOLUTE_SHORT_NAME_ ", --" _ABSOLUTE_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Generate LilyPond absolute note octaves. " << 
      endl <<
    idtr << tab << tab << tab <<
      "By default, relative octaves are generated." << 
      endl <<
    endl;

  cerr <<
    idtr <<
      "--" _STEMS_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Generate \\stemUp and \\stemDown LilyPond commands." << 
      endl <<
    idtr << tab << tab << tab <<
      "By default, LilyPond will take care of that by itself." << 
      endl <<
    endl;

  cerr <<
    idtr <<
      "--" _NO_AUTO_BEAMING_SHORT_NAME_ ", --" _NO_AUTO_BEAMING_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Generate '\\set Voice.autoBeaming = ##f' in each voice " << 
      endl <<
    idtr << tab << tab << tab <<
      "to prevent LilyPond from handling beams automatically." << 
      endl <<
    endl;

  cerr <<
    idtr <<
      "--" _ROMAN_STRING_NUMBERS_SHORT_NAME_ ", --" _ROMAN_STRING_NUMBERS_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Generate '\\romanStringNumbers' in each voice " << 
      endl <<
    idtr << tab << tab << tab <<
      "for LilyPond to generate roman instead of arabic string numbers." << 
      endl <<
    endl;

  cerr <<
    idtr <<
      "--" _AVOID_OPEN_STRINGS_SHORT_NAME_ ", --" _AVOID_OPEN_STRINGS_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Generate '\\set TabStaff.restrainOpenStrings = ##t' in each voice " << 
      endl <<
    idtr << tab << tab << tab <<
      "to prevent LilyPond from using open strings." << 
      endl <<
    endl;

  cerr <<
    idtr <<
      "--" _ACCIDENTAL_STYLE_SHORT_NAME_ ", --" _ACCIDENTAL_STYLE_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Choose the LilyPond accidental styles among: " << 
      endl <<
    idtr << tab << tab << tab <<
      "voice, modern, modern-cautionary, modern-voice, " <<  
      endl <<
    idtr << tab << tab << tab <<
      "modern-voice-cautionary, piano, piano-cautionary, " << 
      endl <<
    idtr << tab << tab << tab <<
      "neo-modern, neo-modern-cautionary, neo-modern-voice," << 
      endl <<
    idtr << tab << tab << tab <<
      "neo-modern-voice-cautionary, dodecaphonic, dodecaphonic-no-repeat," << 
      endl <<
    idtr << tab << tab << tab <<
      "dodecaphonic-first, teaching, no-reset, forget." << 
      endl <<
    idtr << tab << tab << tab <<
      "The default is... 'default'." << 
      endl <<
    endl <<
      
    idtr <<
      "--" _COMPRESS_MULTI_MEASURE_RESTS_SHORT_NAME_ ", --" _COMPRESS_MULTI_MEASURE_RESTS_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Generate '\\compressMMRests' at the beginning of voices." << 
      endl <<
    idtr << tab << tab << tab <<
      "By default, this command is commented." << 
      endl <<
    endl;

  cerr <<
    idtr <<
      "--" _NOTE_INPUT_LINE_NUMBERS_SHORT_NAME_ ", --" _NOTE_INPUT_LINE_NUMBERS_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Generate after each note and barline a comment containing" << 
      endl <<
    idtr << tab << tab << tab <<
      "its MusicXML input line number." << 
      endl <<
    endl;
    
  idtr--;

  // bars
  // --------------------------------------
  cerr <<
    idtr << "Bars:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _SHOW_ALL_BAR_NUMBERS_SHORT_NAME_ ", --" _SHOW_ALL_BAR_NUMBERS_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Generate LilyPond code to show all bar numbers." << 
      endl <<
    endl;

  idtr--;

  // line breaks
  // --------------------------------------
  cerr <<
    idtr << "Line breaks:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _DONT_KEEP_LINE_BREAKS_SHORT_NAME_ ", --" _DONT_KEEP_LINE_BREAKS_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Don't keep the line breaks from the MusicXML input" << 
      endl <<
    idtr << tab << tab << tab <<
      "and let LilyPond decide about them." << 
      endl <<
    endl <<
      
    idtr <<
      "--" _BREAK_LINES_AT_INCOMPLETE_RIGHT_MEASURES_SHORT_NAME_ ", --" _BREAK_LINES_AT_INCOMPLETE_RIGHT_MEASURES_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Generate a '\\break' command at the end incomplete right measures" << 
      endl <<
    idtr << tab << tab << tab <<
      "which is handy in popular folk dances and tunes." << 
      endl <<
    endl <<
      
    idtr <<
      "--" _SEPARATOR_LINE_EVERY_N_MEASURES_SHORT_NAME_ ", --" _SEPARATOR_LINE_EVERY_N_MEASURES_LONG_NAME_ " 'n'" << 
      endl <<
    idtr << tab << tab << tab <<
      "Generate an additional separator line for readability every 'n' measures," << 
      endl <<
    idtr << tab << tab << tab <<
      "where 'n' is a positive integer'." << 
      endl <<
    endl;

  idtr--;

  // staves
  // --------------------------------------
  cerr <<
    idtr << "Staves:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _MODERN_TAB_SHORT_NAME_ ", --" _MODERN_TAB_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Generate '\\moderntab' instead of the default '\\tab'." << 
      endl <<
    endl;

  idtr--;

  // tuplets
  // --------------------------------------
  cerr <<
    idtr << "Tuplets:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _TUPLETS_ON_A_LINE_SHORT_NAME_ ", --" _TUPLETS_ON_A_LINE_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Keep tuplets notes on the same line, instead of" << 
      endl <<
    idtr << tab << tab << tab <<
      "'having \\tuplet {' and '}' on separate lines." << 
      endl <<
    endl;
      
  idtr--;

  // repeats
  // --------------------------------------
  cerr <<
    idtr << "Repeats:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _REPEAT_BRACKETS_SHORT_NAME_ ", --" _REPEAT_BRACKETS_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Generate repeats with brackets instead of regular bar lines." << 
      endl <<
    endl;
      
  idtr--;

  // ornaments
  // --------------------------------------
  cerr <<
    idtr << "Ornaments:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _DELAYED_ORNAMENTS_FRACTION_SHORT_NAME_ ", --" _DELAYED_ORNAMENTS_FRACTION_LONG_NAME_ " 'num/denom'" << 
      endl <<
    idtr <<
      "--" _DELAYED_ORNAMENTS_FRACTION_SHORT_NAME_ ", --" _DELAYED_ORNAMENTS_FRACTION_LONG_NAME_ " \"num/denom\"" << 
      endl <<
    idtr << tab << tab << tab <<
      "Place the delayed turn/reverseturn at the given fraction" << 
      endl <<
    idtr << tab << tab << tab <<
      "between the ornemented note and the next one." << 
      endl <<
    idtr << tab << tab << tab <<
      "The default fraction is '2/3'." << 
      endl <<
    endl;
      
  idtr--;

  // code generation
  // --------------------------------------
  cerr <<
    idtr << "Code generation:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _COMMENTS_SHORT_NAME_ ", --" _COMMENTS_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Generate comments showing the structure of the score" << 
      endl <<
    idtr << tab << tab << tab <<
      "such as '% part P_POne (P1)'." << 
      endl <<
    endl;

  cerr <<
    idtr <<
      "--" _GLOBAL_LONG_NAME_ << // _GLOBAL_SHORT_NAME_ is empty
      endl <<
    idtr << tab << tab << tab <<
      "Generate a 'global' empty variable and place a use of it at the beginning of all voices." << 
      endl <<
    endl <<
      
    idtr <<
      "--" _DISPLAY_MUSIC_SHORT_NAME_ ", --" _DISPLAY_MUSIC_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Place the contents of all voices inside a '\\displayMusic' block," << 
      endl <<
    idtr << tab << tab << tab <<
      "'for LilyPond to show its internal representation of the music." << 
      endl <<
    endl <<
      
    idtr <<
      "--" _NO_LILYPOND_CODE_SHORT_NAME_ ", --" _NO_LILYPOND_CODE_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Don't generate any LilyPond code." << 
      endl <<
    idtr << tab << tab << tab <<
      "This can be useful if only a summary of the score is needed." << 
      endl <<
    endl <<
      
    idtr <<
      "--" _NO_LILYPOND_LYRICS_SHORT_NAME_ ", --" _NO_LILYPOND_LYRICS_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Don't generate any lyrics in the LilyPond code." << 
      endl <<
    endl <<
      
    idtr <<
      "--" _LILYPOND_COMPILE_DATE_SHORT_NAME_ << // _LILYPOND_COMPILE_DATE_SHORT_NAME_ is empty 
      endl <<
    idtr << tab << tab << tab <<
      "Generate code to include the date when LilyPond creates the score in the latter." << 
      endl <<
    endl;
    
  idtr--;

  // midi
  // --------------------------------------
  cerr <<
    idtr << "Midi:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _MIDI_TEMPO_SHORT_NAME_ ", --" _MIDI_TEMPO_LONG_NAME_ " 'duration = perSecond'" << 
      endl <<
    idtr <<
      "--" _MIDI_TEMPO_SHORT_NAME_ ", --" _MIDI_TEMPO_LONG_NAME_ " \"duration = perSecond\"" << 
      endl <<
    idtr << tab << tab << tab <<
      "Generate a '\\tempo duration = perSecond' command in the \\midi block." << 
      endl <<
    idtr << tab << tab << tab <<
      "'duration' is a string such as '8.', and 'perSecond' is an integer," << 
      endl <<
    idtr << tab << tab << tab <<
      "and spaces can be used frely in the argument." << 
      endl <<
    idtr << tab << tab << tab <<
      "The default is '4 = 100'." << 
      endl <<      
    endl <<
    
    idtr <<
      "--" _NO_MIDI_LONG_NAME_ << // _NO_MIDI_SHORT_NAME_ is empty
      endl <<
    idtr << tab << tab << tab <<
      "Generate the '\\midi' block as a comment instead of active code." << 
      endl <<
    endl;

  idtr--;

  
  idtr--;
}

void lilypondOptions::printLilypondOptionsValues (int fieldWidth)
{
  cerr << idtr <<
    "The LPSR options are:" <<
    endl;

  idtr++;
  
  // time
  // --------------------------------------
  cerr <<
    idtr << "Time:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw(fieldWidth) << "numericalTime" << " : " <<
      booleanAsString (fNumericalTime) <<
      endl;

  idtr--;
  
  // notes
  // --------------------------------------
  cerr <<
    idtr << "Notes:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw(fieldWidth) << "absoluteOctaves" << " : " <<
      booleanAsString (fAbsoluteOctaves) <<
      endl <<
    
    idtr << setw(fieldWidth) << "stems" << " : " <<
      booleanAsString (fStems) <<
      endl <<

    idtr << setw(fieldWidth) << "noAutoBeaming" << " : " <<
      booleanAsString (fNoAutoBeaming) <<
      endl <<

    idtr << setw(fieldWidth) << "romanStringNumbers" << " : " <<
      booleanAsString (fRomanStringNumbers) <<
      endl <<
      
    idtr << setw(fieldWidth) << "avoidOpenString" << " : " <<
      booleanAsString (fAvoidOpenString) <<
      endl <<

    idtr << setw(fieldWidth) << "accidentalStyle" << " : " <<
      fAccidentalStyle <<
      endl <<
    
    idtr << setw(fieldWidth) << "compressMultiMeasureRests" << " : " <<
      booleanAsString (fCompressMultiMeasureRests) <<
      endl <<

    idtr << setw(fieldWidth) << "inputLineNumbers" << " : " <<
      booleanAsString (fInputLineNumbers) <<
      endl;

  idtr--;
  
  // bars
  // --------------------------------------
  cerr <<
    idtr << "Bars:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw(fieldWidth) << "showAllBarNumbers" << " : " <<
      booleanAsString (fShowAllBarNumbers) <<
      endl;

  idtr--;

  // line breaks
  // --------------------------------------
  cerr <<
    idtr << "Line breaks:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw(fieldWidth) << "dontKeepLineBreaks" << " : " <<
      booleanAsString (fDontKeepLineBreaks) <<
      endl <<

    idtr << setw(fieldWidth) << "breakLinesAtIncompleteRightMeasures" << " : " <<
      booleanAsString (fBreakLinesAtIncompleteRightMeasures) <<
      endl <<
    
    idtr << setw(fieldWidth) << "separatorLineEveryNMeasures" << " : " <<
      booleanAsString (fSeparatorLineEveryNMeasures) <<
      endl <<

    idtr << setw(fieldWidth) << "separatorLineEveryNMeasuresValue" << " : " <<
      fSeparatorLineEveryNMeasuresValue <<
      endl;

  idtr--;
  
  // staves
  // --------------------------------------
  cerr <<
    idtr << "Staves:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw(fieldWidth) << "modernTab" << " : " <<
      booleanAsString (fModernTab) <<
      endl;

  idtr--;
  
  // tuplets
  // --------------------------------------

  cerr <<
    idtr << "Tuplets:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw(fieldWidth) << "tupletsOnALine" << " : " <<
      booleanAsString (fTupletsOnALine) <<
      endl;
    
  idtr--;

  // repeats
  // --------------------------------------

  cerr <<
    idtr << "Repeats:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw(fieldWidth) << "repeatBrackets" << " : " <<
      booleanAsString (fRepeatBrackets) <<
      endl;
    
  idtr--;

  // ornaments
  // --------------------------------------

  cerr <<
    idtr << "Ornaments:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw(fieldWidth) << "delayedOrnamentFractionNumerator" << " : " <<
      fDelayedOrnamentFractionNumerator <<
      endl <<

    idtr << setw(fieldWidth) << "delayedOrnamentFractionDenominator" << " : " <<
      fDelayedOrnamentFractionDenominator <<
      endl;

  idtr--;

  // code generation
  // --------------------------------------

  cerr <<
    idtr << "LilyPond code generation:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw(fieldWidth) << "comments" << " : " <<
      booleanAsString (fComments) <<
      endl <<

    idtr << setw(fieldWidth) << "global" << " : " <<
      booleanAsString (fGlobal) <<
      endl <<
    
    idtr << setw(fieldWidth) << "noLilypondCode" << " : " <<
      booleanAsString (fNoLilypondCode) <<
      endl <<

    idtr << setw(fieldWidth) << "noLilypondLyrics" << " : " <<
      booleanAsString (fNoLilypondLyrics) <<
      endl <<

    idtr << setw(fieldWidth) << "lilypondCompileDate" << " : " <<
      booleanAsString (fLilypondCompileDate) <<
      endl;

  idtr--;
  
  // midi
  // --------------------------------------
  cerr <<
    idtr << "Midi:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw(fieldWidth) << "midiTempoDuration" << " : " <<
      fMidiTempoDuration <<
      endl <<

    idtr << setw(fieldWidth) << "midiTempoPerSecond" << " : " <<
      fMidiTempoPerSecond <<
      endl <<

    idtr << setw(fieldWidth) << "noMidiCommand" << " : " <<
      booleanAsString (fNoMidiCommand) <<
      endl;

  idtr--;
  

  idtr--;
}


}
