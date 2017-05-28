/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iostream>
#include <list>
#include <algorithm>
#include <iomanip>      // setw, set::precision, ...
#include <climits>      /* INT_MIN, INT_MAX */

#include "msrVersion.h"

//#include "xml2Msr.h"

#include "lpsr.h"

using namespace std;

namespace MusicXML2 
{

// chords languages
//______________________________________________________________________________

map<string, lpsrChordsLanguage>
  gLpsrChordsLanguagesMap;

void initializeLpsrChordsLanguages ()
{
  gLpsrChordsLanguagesMap ["Ignatzek"]   = k_IgnatzekChords; // default
  gLpsrChordsLanguagesMap ["german"]     = k_GermanChords;
  gLpsrChordsLanguagesMap ["semiGerman"] = k_SemiGermanChords;
  gLpsrChordsLanguagesMap ["italian"]    = k_ItalianChords;
  gLpsrChordsLanguagesMap ["french"]     = k_FrenchChords;
}

string lpsrChordsLanguageAsString (
  lpsrChordsLanguage language)
{
  string result;
  
  switch (language) {
    case k_IgnatzekChords: // default value
      result = "IgnatzekChords";
      break;
    case k_GermanChords:
      result = "german";
      break;
    case k_SemiGermanChords:
      result = "semiGerman";
      break;
    case k_ItalianChords:
      result = "italian";
      break;
    case k_FrenchChords:
      result = "french";
      break;
  } // switch

  return result;
}

string existingLpsrChordsLanguages ()
{
  stringstream s;
  
  map<string, lpsrChordsLanguage>::const_iterator
    iBegin = gLpsrChordsLanguagesMap.begin(),
    iEnd   = gLpsrChordsLanguagesMap.end(),
    i      = iBegin;
  for ( ; ; ) {
    if ((*i).second != k_IgnatzekChords)
      s << (*i).first;
    if (++i == iEnd) break;
    if ((*i).second != k_IgnatzekChords)
      s << " ";
  } // for

  return s.str();
}

//_______________________________________________________________________________
S_lpsrOptions gLpsrOptions;

S_lpsrOptions lpsrOptions::create()
{
  lpsrOptions* o = new lpsrOptions();
  assert(o!=0);
  return o;
}

lpsrOptions::lpsrOptions()
{
  initializeLpsrOptions (false);
}

lpsrOptions::~lpsrOptions() {}

void lpsrOptions::initializeLpsrOptions (
  bool boolOptionsInitialValue)
{
  // trace and display
  
  fTraceLpsr         = boolOptionsInitialValue;
  
  fTraceLpsrVisitors = boolOptionsInitialValue;

  fDisplayLpsr       = boolOptionsInitialValue;
  
  // languages
  
  if (! setLpsrQuartertonesPitchesLanguage ("nederlands")) {
    stringstream s;

    s <<
      "INTERNAL INITIALIZATION ERROR: "
      "LPSR pitches language 'nederlands' is unknown" <<
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
}

bool lpsrOptions::setLpsrQuartertonesPitchesLanguage (string language)
{
  // is language in the note names languages map?
  map<string, msrQuatertonesPitchesLanguage>::const_iterator
    it =
      gQuatertonesPitchesLanguagesMap.find (language);
        
  if (it == gQuatertonesPitchesLanguagesMap.end ()) {
    // no, language is unknown in the map    
    return false;
  }

  fLpsrQuatertonesPitchesLanguage = (*it).second;
  
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
      endl;

  idtr--;
  
  // languages
  // --------------------------------------
  cerr <<
    idtr << "Languages:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw(fieldWidth) << "lpsrQuaterTonesPitchesLanguage" << " : \"" <<
      msrQuatertonesPitchesLanguageAsString (
        fLpsrQuatertonesPitchesLanguage) <<
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

S_lilypondOptions lilypondOptions::create()
{
  lilypondOptions* o = new lilypondOptions();
  assert(o!=0);
  return o;
}

lilypondOptions::lilypondOptions()
{
  initializeLilypondOptions (false);
}

lilypondOptions::~lilypondOptions() {}

void lilypondOptions::initializeLilypondOptions (
  bool boolOptionsInitialValue)
{
  // time
  
  fGenerateNumericalTime = boolOptionsInitialValue;

  // notes
  
  fGenerateAbsoluteOctaves  = boolOptionsInitialValue;

  fGenerateStems            = boolOptionsInitialValue;
  fNoAutoBeaming            = boolOptionsInitialValue;
  
  fRomanStringNumbers       = boolOptionsInitialValue;
  fAvoidOpenString          = boolOptionsInitialValue;
  
  fAccidentalStyle          = "";

  fCompressMultiMeasureRests = boolOptionsInitialValue;

  fGenerateInputLineNumbers = boolOptionsInitialValue;

  // bars
  
  fShowAllBarNumbers    = boolOptionsInitialValue;

  // line breaks
  
  fDontKeepLineBreaks                  = boolOptionsInitialValue;

  fBreakLinesAtIncompleteRightMeasures = boolOptionsInitialValue;
  
  fSeparatorLineEveryNMeasures         = boolOptionsInitialValue;
  fSeparatorLineEveryNMeasuresValue    = INT_MAX;
  
  // staves
  
  fModernTab = boolOptionsInitialValue;

  // midi

  fMidiTempoDuration       = "4";
  fMidiTempoPerSecond      = 100;
  
  fDontGenerateMidiCommand = boolOptionsInitialValue;
  
  // LilyPond code generation

  fGenerateComments                   = boolOptionsInitialValue;

  fGenerateGlobal                     = boolOptionsInitialValue;
  
  fTupletsOnALine                     = boolOptionsInitialValue;
  
  fRepeatBrackets                     = boolOptionsInitialValue;

  fDelayedOrnamentFractionNumerator   = 2;
  fDelayedOrnamentFractionDenominator = 3;

  fDisplayMusic                       = boolOptionsInitialValue;
  
  fDontGenerateLilyPondCode           = boolOptionsInitialValue;
  
  fDontGenerateLilyPondLyrics         = boolOptionsInitialValue;
  
  fGenerateLilyPondCompileDate        = boolOptionsInitialValue;
    
  
  // JMI ???

  fKeepStaffSize        = boolOptionsInitialValue;

  fGenerateMasterVoices = boolOptionsInitialValue;


  // register the LilyPond accidental styles
  
  fLilyPondAccidentalStyles.insert ("voice");
  fLilyPondAccidentalStyles.insert ("modern");
  fLilyPondAccidentalStyles.insert ("modern-cautionary");
  fLilyPondAccidentalStyles.insert ("modern-voice");
  fLilyPondAccidentalStyles.insert ("modern-voice-cautionary");
  fLilyPondAccidentalStyles.insert ("piano");
  fLilyPondAccidentalStyles.insert ("piano-cautionary");
  fLilyPondAccidentalStyles.insert ("neo-modern");
  fLilyPondAccidentalStyles.insert ("neo-modern-cautionary");
  fLilyPondAccidentalStyles.insert ("neo-modern-voice");
  fLilyPondAccidentalStyles.insert ("neo-modern-voice-cautionary");
  fLilyPondAccidentalStyles.insert ("dodecaphonic");
  fLilyPondAccidentalStyles.insert ("dodecaphonic-no-repeat");
  fLilyPondAccidentalStyles.insert ("dodecaphonic-first");
  fLilyPondAccidentalStyles.insert ("teaching");
  fLilyPondAccidentalStyles.insert ("no-reset");
  fLilyPondAccidentalStyles.insert ("forget");

  /* JMI
  for (
    set<string>::const_iterator i=fLilyPondAccidentalStyles.begin();
    i!=fLilyPondAccidentalStyles.end();
    i++) {
      cout << (*i) << endl;
  } // for
  */
}

bool lilypondOptions::setAccidentalStyle (string accidentalStyle)
{
  // is accidentalStyle in the accidental styles set?
  set<string>::const_iterator
    it =
      fLilyPondAccidentalStyles.find (accidentalStyle);
        
  if (it == fLilyPondAccidentalStyles.end ()) {
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
      "Don't generate non-numerical time signatures such as 'C'." << 
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
      "--" _GENERATE_GLOBAL_SHORT_NAME_ ", --" _GENERATE_GLOBAL_LONG_NAME_ << 
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
      "--" _DONT_GENERATE_LILYPOND_CODE_SHORT_NAME_ ", --" _DONT_GENERATE_LILYPOND_CODE_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Don't generate LilyPond code." << 
      endl <<
    idtr << tab << tab << tab <<
      "This can be useful if only a summary of the score is needed." << 
      endl <<
    endl <<
      
    idtr <<
      "--" _DONT_GENERATE_LILYPOND_LYRICS_SHORT_NAME_ ", --" _DONT_GENERATE_LILYPOND_LYRICS_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Don't generate lyrics in the LilyPond code." << 
      endl <<
    endl <<
      
    idtr <<
      "--" _GENERATE_LILYPOND_COMPILE_DATE_SHORT_NAME_ ", --" _GENERATE_LILYPOND_COMPILE_DATE_LONG_NAME_ << 
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
      "--" _NO_MIDI_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Generate the '\\midi' block as a comment." << 
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
    idtr << setw(fieldWidth) << "generateNumericalTime" << " : " <<
      booleanAsString (fGenerateNumericalTime) <<
      endl;

  idtr--;
  
  // notes
  // --------------------------------------
  cerr <<
    idtr << "Notes:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw(fieldWidth) << "generateAbsoluteOctaves" << " : " <<
      booleanAsString (fGenerateAbsoluteOctaves) <<
      endl <<
    
    idtr << setw(fieldWidth) << "generateStems" << " : " <<
      booleanAsString (fGenerateStems) <<
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

    idtr << setw(fieldWidth) << "generateInputLineNumbers" << " : " <<
      booleanAsString (fGenerateInputLineNumbers) <<
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
    idtr << setw(fieldWidth) << "generateComments" << " : " <<
      booleanAsString (fGenerateComments) <<
      endl <<

    idtr << setw(fieldWidth) << "generateGlobal" << " : " <<
      booleanAsString (fGenerateGlobal) <<
      endl <<
    
    idtr << setw(fieldWidth) << "dontGenerateLilyPondCode" << " : " <<
      booleanAsString (fDontGenerateLilyPondCode) <<
      endl <<

    idtr << setw(fieldWidth) << "dontGenerateLilyPondLyrics" << " : " <<
      booleanAsString (fDontGenerateLilyPondLyrics) <<
      endl <<

    idtr << setw(fieldWidth) << "generateLilyPondCompileDate" << " : " <<
      booleanAsString (fGenerateLilyPondCompileDate) <<
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

    idtr << setw(fieldWidth) << "dontGenerateMidiCommand" << " : " <<
      booleanAsString (fDontGenerateMidiCommand) <<
      endl;

  idtr--;
  

  idtr--;
}

//_______________________________________________________________________________
S_lpsrElement lpsrElement::create (
  int            inputLineNumber)
{
  lpsrElement* o =
    new lpsrElement (
      inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrElement::lpsrElement (
  int            inputLineNumber)
    : msrElement (inputLineNumber)
{}

lpsrElement::~lpsrElement() {}

void lpsrElement::acceptIn (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrElement::acceptIn()" <<
      endl;
      
  if (visitor<S_lpsrElement>*
    p =
      dynamic_cast<visitor<S_lpsrElement>*> (v)) {
        S_lpsrElement elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrElement::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void lpsrElement::acceptOut (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrElement::acceptOut()" <<
      endl;

  if (visitor<S_lpsrElement>*
    p =
      dynamic_cast<visitor<S_lpsrElement>*> (v)) {
        S_lpsrElement elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrElement::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void lpsrElement::browseData (basevisitor* v)
{}

//______________________________________________________________________________
S_lpsrParallelMusic lpsrParallelMusic::create (
  int                   inputLineNumber,
  lpsrElementsSeparator elementsSeparator)
{
  lpsrParallelMusic* o =
    new lpsrParallelMusic (
      inputLineNumber, elementsSeparator);
  assert(o!=0);
  return o;
}

lpsrParallelMusic::lpsrParallelMusic (
  int                   inputLineNumber,
  lpsrElementsSeparator elementsSeparator)
    : lpsrElement (inputLineNumber)
{
  fElementsSeparator=elementsSeparator;
}

lpsrParallelMusic::~lpsrParallelMusic() {}

void lpsrParallelMusic::acceptIn (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrParallelMusic::acceptIn()" <<
      endl;
      
  if (visitor<S_lpsrParallelMusic>*
    p =
      dynamic_cast<visitor<S_lpsrParallelMusic>*> (v)) {
        S_lpsrParallelMusic elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrParallelMusic::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void lpsrParallelMusic::acceptOut (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrParallelMusic::acceptOut()" <<
      endl;

  if (visitor<S_lpsrParallelMusic>*
    p =
      dynamic_cast<visitor<S_lpsrParallelMusic>*> (v)) {
        S_lpsrParallelMusic elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrParallelMusic::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void lpsrParallelMusic::browseData (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrParallelMusic::browseData()" <<
      endl;
  
  for (
    vector<S_msrElement>::iterator i = fParallelMusicElements.begin();
    i != fParallelMusicElements.end();
    i++) {
    // browse the element browser
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% <== lpsrParallelMusic::browseData()" <<
      endl;
}

ostream& operator<< (ostream& os, const S_lpsrParallelMusic& elt)
{
  elt->print (os);
  return os;
}

void lpsrParallelMusic::print (ostream& os)
{
  os <<
    "ParallelMusic" <<
    ", " <<
    singularOrPlural (
      fParallelMusicElements.size(), "element", "elements") <<
    endl;
  
  idtr++;
  
  for (
    vector<S_msrElement>::const_iterator i = fParallelMusicElements.begin();
    i != fParallelMusicElements.end();
    i++) {
    os << idtr << (*i);
  } // for
/* JMI
  int size = fParallelMusicElements.size();
  
  for (int i = 0; i < size; i++ ) {
    os << idtr << fParallelMusicElements [i];
  } // for
*/ 
  idtr--;
}

//______________________________________________________________________________
S_lpsrComment lpsrComment::create (
  int                inputLineNumber,
  string             contents,
  lpsrCommentGapKind commentGapKind)
{
  lpsrComment* o = new
    lpsrComment (
      inputLineNumber, contents, commentGapKind);
  assert(o!=0);
  return o;
}

lpsrComment::lpsrComment (
  int                inputLineNumber,
  string             contents,
  lpsrCommentGapKind commentGapKind)
    : lpsrElement (inputLineNumber)
{
  fContents       = contents;
  fCommentGapKind = commentGapKind;
}

lpsrComment::~lpsrComment() {}

void lpsrComment::acceptIn (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrComment::acceptIn()" <<
      endl;
      
  if (visitor<S_lpsrComment>*
    p =
      dynamic_cast<visitor<S_lpsrComment>*> (v)) {
        S_lpsrComment elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrComment::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void lpsrComment::acceptOut (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrComment::acceptOut()" <<
      endl;

  if (visitor<S_lpsrComment>*
    p =
      dynamic_cast<visitor<S_lpsrComment>*> (v)) {
        S_lpsrComment elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrComment::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void lpsrComment::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrComment& elt)
{
  elt->print (os);
  return os;
}

void lpsrComment::print (ostream& os)
{
  os <<
    "Comment" <<
    endl;
  
  idtr++;
  
  os << idtr <<
    "% " << fContents <<
    endl;

  if (fCommentGapKind == kGapAfterwards)
    os <<
      endl;
  
  idtr--;
}

//______________________________________________________________________________
S_lpsrBarNumberCheck lpsrBarNumberCheck::create (
  int                    inputLineNumber,
  int                    nextBarNumber)
{
  lpsrBarNumberCheck* o =
    new lpsrBarNumberCheck (
      inputLineNumber, nextBarNumber);
  assert(o!=0);
  return o;
}

lpsrBarNumberCheck::lpsrBarNumberCheck (
  int                    inputLineNumber,
  int                    nextBarNumber)
    : lpsrElement (inputLineNumber)
{
  fNextBarNumber=nextBarNumber; 
}
lpsrBarNumberCheck::~lpsrBarNumberCheck() {}

void lpsrBarNumberCheck::acceptIn (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrBarNumberCheck::acceptIn()" <<
      endl;
      
  if (visitor<S_lpsrBarNumberCheck>*
    p =
      dynamic_cast<visitor<S_lpsrBarNumberCheck>*> (v)) {
        S_lpsrBarNumberCheck elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrBarNumberCheck::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void lpsrBarNumberCheck::acceptOut (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrBarNumberCheck::acceptOut()" <<
      endl;

  if (visitor<S_lpsrBarNumberCheck>*
    p =
      dynamic_cast<visitor<S_lpsrBarNumberCheck>*> (v)) {
        S_lpsrBarNumberCheck elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrBarNumberCheck::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void lpsrBarNumberCheck::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrBarNumberCheck& elt)
{
  elt->print (os);
  return os;
}

void lpsrBarNumberCheck::print (ostream& os)
{
  os <<
    "BarNumberCheck" << " " << fNextBarNumber <<
    endl;
}

//______________________________________________________________________________
S_lpsrUseVoiceCommand lpsrUseVoiceCommand::create (
  int            inputLineNumber,
  S_msrVoice     voice)
{
  lpsrUseVoiceCommand* o =
    new lpsrUseVoiceCommand (
      inputLineNumber,
      voice);
  assert(o!=0);
  return o;
}

lpsrUseVoiceCommand::lpsrUseVoiceCommand (
  int            inputLineNumber,
  S_msrVoice     voice)
    : lpsrElement (inputLineNumber)
{
  fVoice  = voice;
}

lpsrUseVoiceCommand::~lpsrUseVoiceCommand() {}

void lpsrUseVoiceCommand::acceptIn (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrUseVoiceCommand::acceptIn()" <<
      endl;
      
  if (visitor<S_lpsrUseVoiceCommand>*
    p =
      dynamic_cast<visitor<S_lpsrUseVoiceCommand>*> (v)) {
        S_lpsrUseVoiceCommand elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrUseVoiceCommand::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void lpsrUseVoiceCommand::acceptOut (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrUseVoiceCommand::acceptOut()" <<
      endl;

  if (visitor<S_lpsrUseVoiceCommand>*
    p =
      dynamic_cast<visitor<S_lpsrUseVoiceCommand>*> (v)) {
        S_lpsrUseVoiceCommand elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrUseVoiceCommand::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void lpsrUseVoiceCommand::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrUseVoiceCommand& nlc)
{
  nlc->print (os);
  return os;
}

void lpsrUseVoiceCommand::print (ostream& os)
{
  os <<
    "UseVoiceCommand" << " \"" <<
    fVoice->getVoiceName () <<
    "\", " <<
    singularOrPlural (
      fVoice->getVoiceStanzasMap ().size(), "stanza", "stanzas") <<
    endl;
}

//______________________________________________________________________________
S_lpsrNewLyricsBlock lpsrNewLyricsBlock::create (
  int            inputLineNumber,
  S_msrStanza    stanza,
  S_msrVoice     voice)
{
  lpsrNewLyricsBlock* o =
    new lpsrNewLyricsBlock (
      inputLineNumber,
      stanza, voice);
  assert(o!=0);
  return o;
}

lpsrNewLyricsBlock::lpsrNewLyricsBlock (
  int            inputLineNumber,
  S_msrStanza    stanza,
  S_msrVoice     voice)
    : lpsrElement (inputLineNumber)
{
  fStanza = stanza;
  fVoice  = voice;
}

lpsrNewLyricsBlock::~lpsrNewLyricsBlock() {}

void lpsrNewLyricsBlock::acceptIn (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrNewLyricsBlock::acceptIn()" <<
      endl;
      
  if (visitor<S_lpsrNewLyricsBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewLyricsBlock>*> (v)) {
        S_lpsrNewLyricsBlock elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrNewLyricsBlock::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void lpsrNewLyricsBlock::acceptOut (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrNewLyricsBlock::acceptOut()" <<
      endl;

  if (visitor<S_lpsrNewLyricsBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewLyricsBlock>*> (v)) {
        S_lpsrNewLyricsBlock elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrNewLyricsBlock::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void lpsrNewLyricsBlock::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrNewLyricsBlock& nlc)
{
  nlc->print (os);
  return os;
}

void lpsrNewLyricsBlock::print (ostream& os)
{
  os <<
    "NewLyricsBlock" << " " <<
    fStanza->getStanzaName () << " " <<
    fVoice->getVoiceName () <<
    endl;
}

//______________________________________________________________________________
S_lpsrLilypondVarValAssoc lpsrLilypondVarValAssoc::create (
  int                 inputLineNumber,
  lpsrCommentedKind   commentedKind,
  lpsrBackslashKind   backslashKind,
  string              variableName,
  lpsrVarValSeparator varValSeparator,
  lpsrQuotesKind      quotesKind,
  string              value, 
  string              unit,
  string              comment,
  lpsrEndlKind        endlKind)
{
  lpsrLilypondVarValAssoc* o =
    new lpsrLilypondVarValAssoc(
      inputLineNumber,
      commentedKind,
      backslashKind,
      variableName,
      varValSeparator, 
      quotesKind,
      value,
      unit,
      comment,
      endlKind);
  assert(o!=0);
  return o;
}

lpsrLilypondVarValAssoc::lpsrLilypondVarValAssoc (
  int                 inputLineNumber,
  lpsrCommentedKind   commentedKind,
  lpsrBackslashKind   backslashKind,
  string              variableName,
  lpsrVarValSeparator varValSeparator,
  lpsrQuotesKind      quotesKind,
  string              value, 
  string              unit,
  string              comment,
  lpsrEndlKind        endlKind)
    : lpsrElement (inputLineNumber)
{
  fBackslashKind   = backslashKind;
  fVariableName    = variableName;
  fVarValSeparator = varValSeparator;
  fQuotesKind      = quotesKind;
  fVariableValue   = value;
  fCommentedKind   = commentedKind;
  fUnit            = unit;
  fComment         = comment;
  fEndlKind        = endlKind;
}

lpsrLilypondVarValAssoc::~lpsrLilypondVarValAssoc() {}

string const lpsrLilypondVarValAssoc::g_VarValAssocNoUnit    = "";
string const lpsrLilypondVarValAssoc::g_VarValAssocNoComment = "";

void lpsrLilypondVarValAssoc::acceptIn (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrLilypondVarValAssoc::acceptIn()" <<
      endl;
      
  if (visitor<S_lpsrLilypondVarValAssoc>*
    p =
      dynamic_cast<visitor<S_lpsrLilypondVarValAssoc>*> (v)) {
        S_lpsrLilypondVarValAssoc elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrLilypondVarValAssoc::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void lpsrLilypondVarValAssoc::acceptOut (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrLilypondVarValAssoc::acceptOut()" <<
      endl;

  if (visitor<S_lpsrLilypondVarValAssoc>*
    p =
      dynamic_cast<visitor<S_lpsrLilypondVarValAssoc>*> (v)) {
        S_lpsrLilypondVarValAssoc elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrLilypondVarValAssoc::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void lpsrLilypondVarValAssoc::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrLilypondVarValAssoc& assoc) {
  assoc->print (os);
  return os;
}

void lpsrLilypondVarValAssoc::print (ostream& os)
{
  os <<
    "LilypondVarValAssoc" <<
    endl;
  
  idtr++;

  // escape quotes if any
  string variableName;
  string variableValue;

  std::for_each (
    fVariableName.begin(),
    fVariableName.end(),
    stringQuoteEscaper (variableName));
  std::for_each (
    fVariableValue.begin(),
    fVariableValue.end(),
    stringQuoteEscaper (variableValue));

  os << idtr;
  switch (fCommentedKind) {
    case kCommented:
      os << "commented";
      break;
    case kUncommented:
      os << "uncommented";
      break;
  } // switch
  os << endl;

  os << idtr;
  switch (fBackslashKind) {
    case kWithBackslash:
      os << "with backslash";
      break;
    case kWithoutBackslash:
      os << "without backslash";
      break;
  } // switch
  os << " before variable name";
  os << endl;
  
  os <<
    idtr << "variable name: \"" << variableName << "\"" <<
    endl;
  
  os << idtr;
  switch (fVarValSeparator) {
    case kSpace:
      os << "space";
      break;
    case kEqualSign:
      os << "equal";
      break;
  } // switch
  os << " separator";
  os << idtr << endl;

  os << idtr;
  switch (fQuotesKind) {
    case kQuotesAroundValue:
      os << "quotes";
      break;
    case kNoQuotesAroundValue:
      os << "noQuotes";
      break;
  } // switch
  os << " around value";
  os << idtr << endl;

  os <<
    idtr << "variable value: \"" << variableValue << "\"" <<
    endl;

  os <<
    idtr << "unit: ";
  if (fUnit.size())
    os << "\"" << fUnit << "\"";
  else
    os << "none";
  os << endl;
  
  os <<
    idtr << "comment: ";
  if (fComment.size())
    os << "\"" << fComment << "\"";
  else
    os << "none";
  os << endl;

  os << idtr;
  switch (fEndlKind) {
    case kWithEndl:
      os << "with end line";
      break;
    case kWithEndlTwice:
      os << "with end line twice";
      break;
    case kWithoutEndl:
      os << "without end line";
      break;
  } // switch
  os << " afterwards";
  os << endl;
  
// JMI  os << endl;

  idtr--;
}

//______________________________________________________________________________
S_lpsrSchemeVarValAssoc lpsrSchemeVarValAssoc::create (
  int               inputLineNumber,
  lpsrCommentedKind commentedKind,
  string            variableName,
  string            value, 
  string            comment,
  lpsrEndlKind      endlKind)
{
  lpsrSchemeVarValAssoc* o =
    new lpsrSchemeVarValAssoc (
      inputLineNumber,
      commentedKind, variableName, value, comment, endlKind);
  assert(o!=0);
  return o;
}

lpsrSchemeVarValAssoc::lpsrSchemeVarValAssoc (
  int               inputLineNumber,
  lpsrCommentedKind commentedKind,
  string            variableName,
  string            value, 
  string            comment,
  lpsrEndlKind      endlKind)
    : lpsrElement (inputLineNumber)
{
  fCommentedKind = commentedKind;

  fVariableName  = variableName;
  fVariableValue = value;

  fComment       = comment;
  
  fEndlKind      = endlKind;
}

lpsrSchemeVarValAssoc::~lpsrSchemeVarValAssoc() {}

string const lpsrSchemeVarValAssoc::g_SchemeVarValAssocNoUnit    = "";
string const lpsrSchemeVarValAssoc::g_SchemeVarValAssocNoComment = "";

void lpsrSchemeVarValAssoc::acceptIn (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrSchemeVarValAssoc::acceptIn()" <<
      endl;
      
  if (visitor<S_lpsrSchemeVarValAssoc>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeVarValAssoc>*> (v)) {
        S_lpsrSchemeVarValAssoc elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrSchemeVarValAssoc::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void lpsrSchemeVarValAssoc::acceptOut (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrSchemeVarValAssoc::acceptOut()" <<
      endl;

  if (visitor<S_lpsrSchemeVarValAssoc>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeVarValAssoc>*> (v)) {
        S_lpsrSchemeVarValAssoc elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrSchemeVarValAssoc::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void lpsrSchemeVarValAssoc::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrSchemeVarValAssoc& assoc)
{
  assoc->print (os);
  return os;
}

void lpsrSchemeVarValAssoc::print (ostream& os)
{
  os <<
    "SchemeVarValAssoc" <<
    endl;

  idtr++;

  // escape quotes if any
  string variableName;
  string variableValue;

  std::for_each (
    fVariableName.begin(),
    fVariableName.end(),
    stringQuoteEscaper (variableName));
  std::for_each (
    fVariableValue.begin(),
    fVariableValue.end(),
    stringQuoteEscaper (variableValue));

  os << idtr;
  switch (fCommentedKind) {
    case kCommented:
      os << "commented";
      break;
    case kUncommented:
      os << "uncommented";
      break;
  } // switch
  os << endl;

  // print resulting strings
  os <<
    idtr << "variable name : \"" << variableName << "\"" <<
    endl <<
    idtr << "variable value: \"" << variableValue << "\"" <<
    endl;

  os <<
    idtr << "comment: ";
  if (fComment.size())
    os << "\"" << fComment << "\"";
  else
    os << "none";
  os << endl;

  os << idtr;
  switch (fEndlKind) {
    case kWithEndl:
      os << "with end line";
      break;
    case kWithEndlTwice:
      os << "with end line twice";
      break;
    case kWithoutEndl:
      os << "without end line";
      break;
  } // switch
  os << " afterwards";
  os << endl;
  
 // JMI os << endl;

  idtr--;
}

//______________________________________________________________________________
S_lpsrSchemeFunction lpsrSchemeFunction::create (
  int    inputLineNumber,
  string functionName,
  string functionDescription,
  string functionCode)
{
  lpsrSchemeFunction* o =
    new lpsrSchemeFunction (
      inputLineNumber,
      functionName, functionDescription, functionCode);
  assert(o!=0);
  return o;
}

lpsrSchemeFunction::lpsrSchemeFunction (
  int    inputLineNumber,
  string functionName,
  string functionDescription,
  string functionCode)
    : lpsrElement (inputLineNumber)
{
  fFunctionName        = functionName;
  fFunctionDescription = functionDescription;
  fFunctionCode        = functionCode;
  }

lpsrSchemeFunction::~lpsrSchemeFunction() {}

void lpsrSchemeFunction::acceptIn (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrSchemeFunction::acceptIn()" <<
      endl;
      
  if (visitor<S_lpsrSchemeFunction>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeFunction>*> (v)) {
        S_lpsrSchemeFunction elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrSchemeFunction::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void lpsrSchemeFunction::acceptOut (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrSchemeFunction::acceptOut()" <<
      endl;

  if (visitor<S_lpsrSchemeFunction>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeFunction>*> (v)) {
        S_lpsrSchemeFunction elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrSchemeFunction::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void lpsrSchemeFunction::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrSchemeFunction& assoc)
{
  assoc->print (os);
  return os;
}

void lpsrSchemeFunction::print (ostream& os)
{
  os <<
    "SchemeFunction" <<
    endl;

  idtr++;

  // print resulting strings
  os <<
    idtr <<
      "function name : \"" << fFunctionName << "\"" <<
      endl <<
    idtr <<
      "function description: \"" << fFunctionDescription << "\"" <<
      endl <<
    idtr <<
      "function code: \"" << fFunctionCode << "\"" <<
      endl;

  idtr--;
}

//______________________________________________________________________________
S_lpsrNewStaffgroupBlock lpsrNewStaffgroupBlock::create (
  int            inputLineNumber)
{
  lpsrNewStaffgroupBlock* o =
    new lpsrNewStaffgroupBlock (
      inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrNewStaffgroupBlock::lpsrNewStaffgroupBlock (
  int            inputLineNumber)
    : lpsrElement (inputLineNumber)
{}

lpsrNewStaffgroupBlock::~lpsrNewStaffgroupBlock() {}

void lpsrNewStaffgroupBlock::acceptIn (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrNewStaffgroupBlock::acceptIn()" <<
      endl;
      
  if (visitor<S_lpsrNewStaffgroupBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffgroupBlock>*> (v)) {
        S_lpsrNewStaffgroupBlock elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrNewStaffgroupBlock::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void lpsrNewStaffgroupBlock::acceptOut (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrNewStaffgroupBlock::acceptOut()" <<
      endl;

  if (visitor<S_lpsrNewStaffgroupBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffgroupBlock>*> (v)) {
        S_lpsrNewStaffgroupBlock elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrNewStaffgroupBlock::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void lpsrNewStaffgroupBlock::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrNewStaffgroupBlock& nstf)
{
  nstf->print (os);
  return os;
}

void lpsrNewStaffgroupBlock::print (ostream& os)
{
  os << "NewStaffgroupBlock" << endl;

  idtr++;
  
  int size = fNewStaffgroupElements.size();

  for (int i = 0; i < size; i++ ) {
    os << idtr << fNewStaffgroupElements[i];
  } // for
  
  idtr--;
}

//______________________________________________________________________________
S_lpsrNewStafftuningBlock lpsrNewStafftuningBlock::create (
  int              inputLineNumber,
  S_msrStafftuning stafftuning)
{
  lpsrNewStafftuningBlock* o =
    new lpsrNewStafftuningBlock (
      inputLineNumber, stafftuning);
  assert(o!=0);
  return o;
}

lpsrNewStafftuningBlock::lpsrNewStafftuningBlock (
  int              inputLineNumber,
  S_msrStafftuning stafftuning)
    : lpsrElement (inputLineNumber)
{
  fStafftuning = stafftuning;
}

lpsrNewStafftuningBlock::~lpsrNewStafftuningBlock() {}

void lpsrNewStafftuningBlock::acceptIn (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrNewStafftuningBlock::acceptIn()" <<
      endl;
      
  if (visitor<S_lpsrNewStafftuningBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewStafftuningBlock>*> (v)) {
        S_lpsrNewStafftuningBlock elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrNewStafftuningBlock::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void lpsrNewStafftuningBlock::acceptOut (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrNewStafftuningBlock::acceptOut()" <<
      endl;

  if (visitor<S_lpsrNewStafftuningBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewStafftuningBlock>*> (v)) {
        S_lpsrNewStafftuningBlock elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrNewStafftuningBlock::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void lpsrNewStafftuningBlock::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrNewStafftuningBlock& nstf)
{
  nstf->print (os);
  return os;
}

void lpsrNewStafftuningBlock::print (ostream& os)
{
  os << "NewStafftuningBlock" << endl;

  idtr++;
  
  os <<
    idtr << fStafftuning;
  
  idtr--;
}

//______________________________________________________________________________
S_lpsrNewStaffBlock lpsrNewStaffBlock::create (
  int            inputLineNumber)
{
  lpsrNewStaffBlock* o =
    new lpsrNewStaffBlock (
      inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrNewStaffBlock::lpsrNewStaffBlock (
  int            inputLineNumber)
    : lpsrElement (inputLineNumber)
{}

lpsrNewStaffBlock::~lpsrNewStaffBlock() {}

void lpsrNewStaffBlock::acceptIn (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrNewStaffBlock::acceptIn()" <<
      endl;
      
  if (visitor<S_lpsrNewStaffBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffBlock>*> (v)) {
        S_lpsrNewStaffBlock elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrNewStaffBlock::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void lpsrNewStaffBlock::acceptOut (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrNewStaffBlock::acceptOut()" <<
      endl;

  if (visitor<S_lpsrNewStaffBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffBlock>*> (v)) {
        S_lpsrNewStaffBlock elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrNewStaffBlock::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void lpsrNewStaffBlock::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrNewStaffBlock& nstf)
{
  nstf->print (os);
  return os;
}

void lpsrNewStaffBlock::print (ostream& os)
{
  os << "NewStaffBlock" << endl;

  idtr++;
  
  int size = fNewStaffElements.size();

  for (int i = 0; i < size; i++ ) {
    os << idtr << fNewStaffElements[i];
  } // for
  
  idtr--;
}

//______________________________________________________________________________
S_lpsrVariableUseCommand lpsrVariableUseCommand::create (
  int            inputLineNumber,
  string         variableName)
{
  lpsrVariableUseCommand* o =
    new lpsrVariableUseCommand (
      inputLineNumber, variableName);
  assert(o!=0);
  return o;
}

lpsrVariableUseCommand::lpsrVariableUseCommand (
  int            inputLineNumber,
  string         variableName)
    : lpsrElement (inputLineNumber)
{
  fVariableName = variableName;
}

lpsrVariableUseCommand::~lpsrVariableUseCommand() {}

void lpsrVariableUseCommand::acceptIn (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrVariableUseCommand::acceptIn()" <<
      endl;
      
  if (visitor<S_lpsrVariableUseCommand>*
    p =
      dynamic_cast<visitor<S_lpsrVariableUseCommand>*> (v)) {
        S_lpsrVariableUseCommand elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrVariableUseCommand::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void lpsrVariableUseCommand::acceptOut (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrVariableUseCommand::acceptOut()" <<
      endl;

  if (visitor<S_lpsrVariableUseCommand>*
    p =
      dynamic_cast<visitor<S_lpsrVariableUseCommand>*> (v)) {
        S_lpsrVariableUseCommand elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrVariableUseCommand::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void lpsrVariableUseCommand::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrVariableUseCommand& nstf)
{
  nstf->print (os);
  return os;
}

void lpsrVariableUseCommand::print (ostream& os)
{
  os << "VariableUseCommand" << endl;
  idtr++;
  os << idtr << fVariableName << endl;
  idtr--;
}

//______________________________________________________________________________
S_lpsrContext lpsrContext::create (
  int             inputLineNumber,
  lpsrContextKind contextKind,
  string          contextType,
  string          contextName)
{
  lpsrContext* o =
    new lpsrContext (
      inputLineNumber,
      contextKind, contextType, contextName);
  assert(o!=0);
  return o;
}

lpsrContext::lpsrContext (
  int             inputLineNumber,
  lpsrContextKind contextKind,
  string          contextType,
  string          contextName)
    : lpsrElement (inputLineNumber)
{
  fContextKind = contextKind;
  fContextType = contextType;
  fContextName = contextName; 
}
lpsrContext::~lpsrContext() {}

void lpsrContext::acceptIn (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrContext::acceptIn()" <<
      endl;
      
  if (visitor<S_lpsrContext>*
    p =
      dynamic_cast<visitor<S_lpsrContext>*> (v)) {
        S_lpsrContext elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrContext::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void lpsrContext::acceptOut (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrContext::acceptOut()" <<
      endl;

  if (visitor<S_lpsrContext>*
    p =
      dynamic_cast<visitor<S_lpsrContext>*> (v)) {
        S_lpsrContext elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrContext::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void lpsrContext::browseData (basevisitor* v)
{}

void lpsrContext::print (ostream& os)
{  
  os << "Context" ", ";
  
  switch (fContextKind) {
    case kExistingContext:
      os << "existing";
      break;
      
    case kNewContext:
      os << "new";
      break;
  } // switch
  
  os << ", \"" << fContextType << "\"";
  if (fContextName.size())
    os << " " << fContextName;
  os << endl;
  
  idtr++;

  int size = fContextElements.size();

  for (int i = 0; i < size; i++ ) {
    os << idtr << fContextElements[i];
  } // for

  idtr--;
}

//______________________________________________________________________________
S_lpsrBarCommand lpsrBarCommand::create (
  int            inputLineNumber)
{
  lpsrBarCommand* o =
    new lpsrBarCommand (
      inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrBarCommand::lpsrBarCommand (
  int            inputLineNumber)
    : lpsrElement (inputLineNumber)
{}

lpsrBarCommand::~lpsrBarCommand() {}

void lpsrBarCommand::acceptIn (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrBarCommand::acceptIn()" <<
      endl;
      
  if (visitor<S_lpsrBarCommand>*
    p =
      dynamic_cast<visitor<S_lpsrBarCommand>*> (v)) {
        S_lpsrBarCommand elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrBarCommand::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void lpsrBarCommand::acceptOut (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrBarCommand::acceptOut()" <<
      endl;

  if (visitor<S_lpsrBarCommand>*
    p =
      dynamic_cast<visitor<S_lpsrBarCommand>*> (v)) {
        S_lpsrBarCommand elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrBarCommand::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void lpsrBarCommand::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrBarCommand& nstf)
{
  nstf->print (os);
  return os;
}

void lpsrBarCommand::print (ostream& os)
{
  os <<
    "BarCommand" << " " << "\"|.\"" <<
    endl;
}

//______________________________________________________________________________
S_lpsrMelismaCommand lpsrMelismaCommand::create (
  int             inputLineNumber,
  lpsrMelismaKind melismaKind)
{
  lpsrMelismaCommand* o =
    new lpsrMelismaCommand (
      inputLineNumber, melismaKind);
  assert(o!=0);
  return o;
}

lpsrMelismaCommand::lpsrMelismaCommand (
  int             inputLineNumber,
  lpsrMelismaKind melismaKind)
    : lpsrElement (inputLineNumber)
{
  if (gGeneralOptions->fTraceLyrics)
    cerr << idtr <<
      "--> creating lpsrMelismaCommand " <<
      melismaKindAsString (melismaKind) <<
      endl;
  
  fMelismaKind = melismaKind;
}

lpsrMelismaCommand::~lpsrMelismaCommand() {}

void lpsrMelismaCommand::acceptIn (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrMelismaCommand::acceptIn()" <<
      endl;
      
  if (visitor<S_lpsrMelismaCommand>*
    p =
      dynamic_cast<visitor<S_lpsrMelismaCommand>*> (v)) {
        S_lpsrMelismaCommand elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrMelismaCommand::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void lpsrMelismaCommand::acceptOut (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrMelismaCommand::acceptOut()" <<
      endl;

  if (visitor<S_lpsrMelismaCommand>*
    p =
      dynamic_cast<visitor<S_lpsrMelismaCommand>*> (v)) {
        S_lpsrMelismaCommand elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrMelismaCommand::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void lpsrMelismaCommand::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrMelismaCommand& nstf)
{
  nstf->print (os);
  return os;
}

string lpsrMelismaCommand::melismaKindAsString (
  lpsrMelismaKind melismaKind)
{
  string result;

  switch (melismaKind) {
    case lpsrMelismaCommand::kMelismaStart:
      result = "Start";
      break;
    case lpsrMelismaCommand::kMelismaEnd:
      result = "End";
      break;
  } // switch

  return result;
}

void lpsrMelismaCommand::print (ostream& os)
{
  os <<
    "MelismaCommand" << " " <<
    melismaKindAsString (fMelismaKind) <<
    endl;
}

//______________________________________________________________________________
S_lpsrHeader lpsrHeader::create (
  int            inputLineNumber)
{
  lpsrHeader* o =
    new lpsrHeader (
      inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrHeader::lpsrHeader (
  int            inputLineNumber)
    : lpsrElement (inputLineNumber)
{}

lpsrHeader::~lpsrHeader() {}

void lpsrHeader::setWorkNumber (
  int    inputLineNumber,
  string val)
{
 fWorkNumber =
    lpsrLilypondVarValAssoc::create (
      inputLineNumber,
      lpsrLilypondVarValAssoc::kUncommented,
      lpsrLilypondVarValAssoc::kWithoutBackslash,
      "work-number",
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kQuotesAroundValue,
      val,
      lpsrLilypondVarValAssoc::g_VarValAssocNoUnit,
      lpsrLilypondVarValAssoc::g_VarValAssocNoComment,
      lpsrLilypondVarValAssoc::kWithoutEndl);
}

void lpsrHeader::setWorkTitle (
  int    inputLineNumber,
  string val)
  {
  fWorkTitle =
    lpsrLilypondVarValAssoc::create (
      inputLineNumber,
      lpsrLilypondVarValAssoc::kUncommented,
      lpsrLilypondVarValAssoc::kWithoutBackslash,
      "work-title",
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kQuotesAroundValue,
      val,
      lpsrLilypondVarValAssoc::g_VarValAssocNoUnit,
      lpsrLilypondVarValAssoc::g_VarValAssocNoComment,
      lpsrLilypondVarValAssoc::kWithoutEndl);
  }

void lpsrHeader::setMovementNumber (
  int    inputLineNumber,
  string val)
  {
  fMovementNumber =
    lpsrLilypondVarValAssoc::create (
      inputLineNumber,
      lpsrLilypondVarValAssoc::kUncommented,
      lpsrLilypondVarValAssoc::kWithoutBackslash,
      "movement-number",
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kQuotesAroundValue,
      val,
      lpsrLilypondVarValAssoc::g_VarValAssocNoUnit,
      lpsrLilypondVarValAssoc::g_VarValAssocNoComment,
      lpsrLilypondVarValAssoc::kWithoutEndl);
  }

void lpsrHeader::setMovementTitle (
  int    inputLineNumber,
  string val)
{
  fMovementTitle =
    lpsrLilypondVarValAssoc::create (
      inputLineNumber,
      lpsrLilypondVarValAssoc::kUncommented,
      lpsrLilypondVarValAssoc::kWithoutBackslash,
      "movement-title",
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kQuotesAroundValue,
      val,
      lpsrLilypondVarValAssoc::g_VarValAssocNoUnit,
      lpsrLilypondVarValAssoc::g_VarValAssocNoComment,
      lpsrLilypondVarValAssoc::kWithoutEndl);
}

S_lpsrLilypondVarValAssoc lpsrHeader::addComposer (
  int    inputLineNumber,
  string type,
  string val)
{
  S_lpsrLilypondVarValAssoc
    result =
      lpsrLilypondVarValAssoc::create (
        inputLineNumber,
        lpsrLilypondVarValAssoc::kUncommented,
        lpsrLilypondVarValAssoc::kWithoutBackslash,
        type,
        lpsrLilypondVarValAssoc::kEqualSign,
        lpsrLilypondVarValAssoc::kQuotesAroundValue,
        val,
        lpsrLilypondVarValAssoc::g_VarValAssocNoUnit,
        lpsrLilypondVarValAssoc::g_VarValAssocNoComment,
      lpsrLilypondVarValAssoc::kWithoutEndl);

  fComposers.push_back (result);

  return result;
}

S_lpsrLilypondVarValAssoc lpsrHeader::addArranger (
  int    inputLineNumber,
  string type,
  string val)
{
  S_lpsrLilypondVarValAssoc
    result =
      lpsrLilypondVarValAssoc::create (
        inputLineNumber,
        lpsrLilypondVarValAssoc::kUncommented,
        lpsrLilypondVarValAssoc::kWithoutBackslash,
        type,
        lpsrLilypondVarValAssoc::kEqualSign,
        lpsrLilypondVarValAssoc::kQuotesAroundValue,
        val,
        lpsrLilypondVarValAssoc::g_VarValAssocNoUnit,
        lpsrLilypondVarValAssoc::g_VarValAssocNoComment,
      lpsrLilypondVarValAssoc::kWithoutEndl);

  fArrangers.push_back (result);

  return result;
}

S_lpsrLilypondVarValAssoc lpsrHeader::addLyricist (
  int    inputLineNumber,
  string type,
  string val)
{
  S_lpsrLilypondVarValAssoc
    result =
      lpsrLilypondVarValAssoc::create (
        inputLineNumber,
        lpsrLilypondVarValAssoc::kUncommented,
        lpsrLilypondVarValAssoc::kWithoutBackslash,
        type,
        lpsrLilypondVarValAssoc::kEqualSign,
        lpsrLilypondVarValAssoc::kQuotesAroundValue,
        val,
        lpsrLilypondVarValAssoc::g_VarValAssocNoUnit,
        lpsrLilypondVarValAssoc::g_VarValAssocNoComment,
      lpsrLilypondVarValAssoc::kWithoutEndl);

  fLyricists.push_back (result);

  return result;
}

void lpsrHeader::setRights (
  int    inputLineNumber,
  string val)
  {
  fRights =
    lpsrLilypondVarValAssoc::create (
      inputLineNumber,
      lpsrLilypondVarValAssoc::kUncommented,
      lpsrLilypondVarValAssoc::kWithoutBackslash,
      "rights",
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kQuotesAroundValue,
      val,
      lpsrLilypondVarValAssoc::g_VarValAssocNoUnit,
      lpsrLilypondVarValAssoc::g_VarValAssocNoComment,
      lpsrLilypondVarValAssoc::kWithoutEndl);
  }

void lpsrHeader::addSoftware (
  int    inputLineNumber,
  string val)
{
  fSoftwares.push_back(
    lpsrLilypondVarValAssoc::create (
      inputLineNumber,
      lpsrLilypondVarValAssoc::kUncommented,
      lpsrLilypondVarValAssoc::kWithoutBackslash,
      "software",
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kQuotesAroundValue,
      val,
      lpsrLilypondVarValAssoc::g_VarValAssocNoUnit,
      lpsrLilypondVarValAssoc::g_VarValAssocNoComment,
      lpsrLilypondVarValAssoc::kWithoutEndl)
  );
}

void lpsrHeader::setEncodingDate (
  int    inputLineNumber,
  string val)
{
  fEncodingDate =
    lpsrLilypondVarValAssoc::create (
      inputLineNumber,
      lpsrLilypondVarValAssoc::kUncommented,
      lpsrLilypondVarValAssoc::kWithoutBackslash,
      "encoding-date",
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kQuotesAroundValue,
      val,
      lpsrLilypondVarValAssoc::g_VarValAssocNoUnit,
      lpsrLilypondVarValAssoc::g_VarValAssocNoComment,
      lpsrLilypondVarValAssoc::kWithoutEndl);
}

void lpsrHeader::setScoreInstrument (
  int    inputLineNumber,
  string val)
{
  fScoreInstrument =
    lpsrLilypondVarValAssoc::create (
      inputLineNumber,
      lpsrLilypondVarValAssoc::kUncommented,
      lpsrLilypondVarValAssoc::kWithoutBackslash,
      "score-instrument",
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kQuotesAroundValue,
      val,
      lpsrLilypondVarValAssoc::g_VarValAssocNoUnit,
      lpsrLilypondVarValAssoc::g_VarValAssocNoComment,
      lpsrLilypondVarValAssoc::kWithoutEndl);
}

void lpsrHeader::changeWorkTitleVariableName (string name)
{
  fWorkTitle->changeAssocVariableName (name);
}

void lpsrHeader::changeMovementTitleVariableName (string name)
{
  fMovementTitle->changeAssocVariableName (name);
}

void lpsrHeader::changeWorkNumberVariableName (string name)
{
  fWorkNumber->changeAssocVariableName (name);
}

void lpsrHeader::changeMovementNumberVariableName (string name)
{
  fMovementNumber->changeAssocVariableName (name);
}

void lpsrHeader::changeRightsTitleVariableName (string name)
{
  fRights->changeAssocVariableName (name);
}

/* JMI
void lpsrHeader::changeCreatorVariableName (
  string variableName, string newName)
{
  vector<S_lpsrLilypondVarValAssoc>::const_iterator i;
  for (i=fCreators.begin(); i!=fCreators.end(); i++) {
    if ((*i)->getVariableName () == variableName)
      (*i)->changeAssocVariableName (newName);
  } // for
}
*/

int lpsrHeader::maxLilyPondVariablesNamesLength ()
{
  int result = 0;

  if (fWorkNumber) {
    int length = fWorkNumber->getVariableName ().size();
    if (length > result) result = length;
  }
  
  if (fWorkTitle) {
    int length = fWorkTitle->getVariableName ().size();
    if (length > result) result = length;
  }
    
  if (fMovementNumber) {
    int length = fMovementNumber->getVariableName ().size();
    if (length > result) result = length;
  }
    
  if (fMovementTitle) {
    int length = fMovementTitle->getVariableName ().size();
    if (length > result) result = length;
  }
    
  if (! fComposers.empty()) {
    vector<S_lpsrLilypondVarValAssoc>::const_iterator i;
    for (i=fComposers.begin(); i!=fComposers.end(); i++) {
      int length = (*i)->getVariableName ().size();
      if (length > result) result = length;
    } // for
  }
    
  if (! fArrangers.empty()) {
    vector<S_lpsrLilypondVarValAssoc>::const_iterator i;
    for (i=fArrangers.begin(); i!=fArrangers.end(); i++) {
      int length = (*i)->getVariableName ().size();
      if (length > result) result = length;
    } // for
  }
    
  if (! fLyricists.empty()) {
    vector<S_lpsrLilypondVarValAssoc>::const_iterator i;
    for (i=fLyricists.begin(); i!=fLyricists.end(); i++) {
      int length = (*i)->getVariableName ().size();
      if (length > result) result = length;
    } // for
  }
    
  if (fRights) {
    int length = fRights->getVariableName ().size();
    if (length > result) result = length;
  }
    
  if (! fSoftwares.empty()) {
    vector<S_lpsrLilypondVarValAssoc>::const_iterator i;
    for (i=fSoftwares.begin(); i!=fSoftwares.end(); i++) {
      int length = (*i)->getVariableName ().size();
      if (length > result) result = length;
    } // for
  }
    
  if (fEncodingDate) {
    int length = fEncodingDate->getVariableName ().size();
    if (length > result) result = length;
  }
  
  return result;
}

void lpsrHeader::acceptIn (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrHeader::acceptIn()" <<
      endl;
      
  if (visitor<S_lpsrHeader>*
    p =
      dynamic_cast<visitor<S_lpsrHeader>*> (v)) {
        S_lpsrHeader elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrHeader::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void lpsrHeader::acceptOut (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrHeader::acceptOut()" <<
      endl;

  if (visitor<S_lpsrHeader>*
    p =
      dynamic_cast<visitor<S_lpsrHeader>*> (v)) {
        S_lpsrHeader elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrHeader::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void lpsrHeader::browseData (basevisitor* v)
{
  if (fWorkNumber) {
    // browse fWorkNumber
    msrBrowser<lpsrLilypondVarValAssoc> browser (v);
    browser.browse (*fWorkNumber);
  }

  if (fWorkTitle) {
    // browse fWorkTitle
    msrBrowser<lpsrLilypondVarValAssoc> browser (v);
    browser.browse (*fWorkTitle);
  }

  if (fMovementNumber) {
    // browse fMovementNumber
    msrBrowser<lpsrLilypondVarValAssoc> browser (v);
    browser.browse (*fMovementNumber);
  }

  if (fMovementTitle) {
    // browse fMovementTitle
    msrBrowser<lpsrLilypondVarValAssoc> browser (v);
    browser.browse (*fMovementTitle);
  }

  if (! fComposers.empty()) {
    vector<S_lpsrLilypondVarValAssoc>::const_iterator i;
    for (i=fComposers.begin(); i!=fComposers.end(); i++) {
      // browse creator
      msrBrowser<lpsrLilypondVarValAssoc> browser (v);
      browser.browse (*(*i));
    } // for
  }
    
  if (! fArrangers.empty()) {
    vector<S_lpsrLilypondVarValAssoc>::const_iterator i;
    for (i=fArrangers.begin(); i!=fArrangers.end(); i++) {
      // browse creator
      msrBrowser<lpsrLilypondVarValAssoc> browser (v);
      browser.browse (*(*i));
    } // for
  }
    
  if (! fLyricists.empty()) {
    vector<S_lpsrLilypondVarValAssoc>::const_iterator i;
    for (i=fLyricists.begin(); i!=fLyricists.end(); i++) {
      // browse creator
      msrBrowser<lpsrLilypondVarValAssoc> browser (v);
      browser.browse (*(*i));
    } // for
  }
    
  if (fRights) {
    // browse fRights
    msrBrowser<lpsrLilypondVarValAssoc> browser (v);
    browser.browse (*fRights);
  }

  if (! fSoftwares.empty()) {
    vector<S_lpsrLilypondVarValAssoc>::const_iterator i;
    for (i=fSoftwares.begin(); i!=fSoftwares.end(); i++) {
      // browse software
      msrBrowser<lpsrLilypondVarValAssoc> browser (v);
      browser.browse (*(*i));
    } // for
  }

  if (fEncodingDate) {
    // browse fEncodingDate
    msrBrowser<lpsrLilypondVarValAssoc> browser (v);
    browser.browse (*fEncodingDate);
  }

  if (fScoreInstrument) {
    // browse fScoreInstrument
    msrBrowser<lpsrLilypondVarValAssoc> browser (v);
    browser.browse (*fScoreInstrument);
  }
}

ostream& operator<< (ostream& os, const S_lpsrHeader& elt)
{
  elt->print (os);
  return os;
}

void lpsrHeader::print (ostream& os)
{
  os <<
    "Header" <<
    endl;

  bool emptyHeader = true;
  
  idtr++;
  
  if (fWorkNumber) {
    os << idtr << fWorkNumber;
    emptyHeader = false;
  }
  
  if (fWorkTitle) {
    os << idtr << fWorkTitle;
    emptyHeader = false;
  }
    
  if (fMovementNumber) {
    os << idtr << fMovementNumber;
    emptyHeader = false;
  }
    
  if (fMovementTitle) {
    os << idtr << fMovementTitle;
    emptyHeader = false;
  }
    
  if (! fComposers.empty()) {
    vector<S_lpsrLilypondVarValAssoc>::const_iterator
      iBegin = fComposers.begin(),
      iEnd   = fComposers.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    emptyHeader = false;
  }
    
  if (! fArrangers.empty()) {
    vector<S_lpsrLilypondVarValAssoc>::const_iterator
      iBegin = fArrangers.begin(),
      iEnd   = fArrangers.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    emptyHeader = false;
  }
    
  if (! fLyricists.empty()) {
    vector<S_lpsrLilypondVarValAssoc>::const_iterator
      iBegin = fLyricists.begin(),
      iEnd   = fLyricists.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    emptyHeader = false;
  }
    
  if (fRights) {
    os << idtr << fRights;
    emptyHeader = false;
  }
    
  if (! fSoftwares.empty()) {
    vector<S_lpsrLilypondVarValAssoc>::const_iterator
      iBegin = fSoftwares.begin(),
      iEnd   = fSoftwares.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
    os << endl;
    
    emptyHeader = false;
  }
    
  if (fEncodingDate) {
    os << idtr << fEncodingDate;
    emptyHeader = false;
  }
  
  if (emptyHeader)
    os << idtr <<
      " " << "nothing specified" << endl; // JMI
  
  idtr--;
}

//______________________________________________________________________________
S_lpsrPaper lpsrPaper::create (
  int           inputLineNumber)
{
  lpsrPaper* o =
    new lpsrPaper (
      inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrPaper::lpsrPaper (
  int           inputLineNumber)
    : msrElement (inputLineNumber)
{
  fPaperWidth   = -1.0;
  fPaperWidth   = -1.0;
  fPaperHeight  = -1.0;
  fTopMargin    = -1.0;
  fBottomMargin = -1.0;
  fLeftMargin   = -1.0;
  fRightMargin  = -1.0;
    
  fBetweenSystemSpace = -1.0;
  
  fPageTopSpace = -1.0;
}

lpsrPaper::~lpsrPaper() {}

void lpsrPaper::acceptIn (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrPaper::acceptIn()" <<
      endl;
      
  if (visitor<S_lpsrPaper>*
    p =
      dynamic_cast<visitor<S_lpsrPaper>*> (v)) {
        S_lpsrPaper elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrPaper::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void lpsrPaper::acceptOut (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrPaper::acceptOut()" <<
      endl;

  if (visitor<S_lpsrPaper>*
    p =
      dynamic_cast<visitor<S_lpsrPaper>*> (v)) {
        S_lpsrPaper elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrPaper::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void lpsrPaper::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_lpsrPaper& pap) {
  pap->print (os);
  return os;
}

void lpsrPaper::print (ostream& os) {
  os << "Paper" << endl;

  bool emptyPaper = true;

  idtr++;

  const int fieldWidth = 13;
  
  if (fPaperWidth > 0) {
    os << idtr << left <<
      setw(fieldWidth) <<
      "paper-width" << " = " <<
      setprecision(4) << fPaperWidth << "\\cm" <<
      endl;
    emptyPaper = false;
  }
  if (fPaperHeight > 0) {
    os << idtr << left <<
      setw(fieldWidth) <<
      "paper-height" << " = " <<
      setprecision(4) << fPaperHeight << "\\cm" <<
      endl;
    emptyPaper = false;
  }
  
  if (fTopMargin > 0) {
    os << idtr <<  left <<
      setw(fieldWidth) <<
      "top-margin" << " = " <<
      setprecision(4) << fTopMargin << "\\cm" <<
      endl;
    emptyPaper = false;
  }
  if (fBottomMargin > 0) {
    os << idtr <<  left <<
      setw(fieldWidth) <<
      "bottom-margin" << " = " <<
      setprecision(4) << fBottomMargin << "\\cm" <<
      endl;
    emptyPaper = false;
  }
  if (fLeftMargin > 0) {
    os << idtr <<  left <<
      setw(fieldWidth) <<
      "left-margin" << " = " <<
      setprecision(4) << fLeftMargin << "\\cm" <<
      endl;
    emptyPaper = false;
  }
  if (fRightMargin > 0) {
    os << idtr <<  left <<
      setw(fieldWidth) <<
      "right-margin" << " = " <<
      setprecision(4) << fRightMargin << "\\cm" <<
      endl;
    emptyPaper = false;
  }

  if (emptyPaper)
    os << idtr <<
      " " << "nothing specified" <<
      endl;
  
  idtr--;
}

/* JMI
 * 
  if (fBetweenSystemSpace > 0) {
    os << idtr << "between-system-space = " << setprecision(4) << fBetweenSystemSpace << "\\cm" << endl;
  }

  if (fPageTopSpace > 0) {
    os << idtr << "page-top-space = " << setprecision(4) << fPageTopSpace << "\\cm" << endl;
  }
*/

//______________________________________________________________________________
S_lpsrLayout lpsrLayout::create (
  int            inputLineNumber)
{
  lpsrLayout* o =
    new lpsrLayout (
      inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrLayout::lpsrLayout (
  int            inputLineNumber)
    : lpsrElement (inputLineNumber)
{
  fStaffSize = 20; // LilyPond default // JMI
}

lpsrLayout::~lpsrLayout() {}

void lpsrLayout::acceptIn (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrLayout::acceptIn()" <<
      endl;
      
  if (visitor<S_lpsrLayout>*
    p =
      dynamic_cast<visitor<S_lpsrLayout>*> (v)) {
        S_lpsrLayout elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrLayout::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void lpsrLayout::acceptOut (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrLayout::acceptOut()" <<
      endl;

  if (visitor<S_lpsrLayout>*
    p =
      dynamic_cast<visitor<S_lpsrLayout>*> (v)) {
        S_lpsrLayout elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrLayout::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void lpsrLayout::browseData (basevisitor* v)
{

  int lilypondAssocs = flpsrLilypondVarValAssocs.size();

  for (int i = 0; i < lilypondAssocs; i++ ) {
    // browse the variable/value association
    msrBrowser<lpsrLilypondVarValAssoc> browser (v);
    browser.browse (*flpsrLilypondVarValAssocs [i]);
  } // for
  
  int schemeAssocs = fLpsrSchemeVarValAssocs.size();
  
  for (int i = 0; i < schemeAssocs; i++ ) {
    // browse the Scheme variable/value association
    msrBrowser<lpsrSchemeVarValAssoc> browser (v);
    browser.browse (*fLpsrSchemeVarValAssocs [i]);
  } // for
}

ostream& operator<< (ostream& os, const S_lpsrLayout& lay)
{
  lay->print (os);
  return os;
}

void lpsrLayout::print (ostream& os)
{
  os << "Layout" << endl;

  idtr++;

  os << idtr <<
    "StaffSize: " << fStaffSize <<
    endl;
    
  int lilypondAssocs = flpsrLilypondVarValAssocs.size();
  
  for (int i = 0; i < lilypondAssocs; i++ ) {
    os << idtr << flpsrLilypondVarValAssocs [i];
  } // for

  int schemeAssocs = fLpsrSchemeVarValAssocs.size();
  
  for (int i = 0; i < schemeAssocs; i++ ) {
    os << idtr << fLpsrSchemeVarValAssocs[i];
  } // for
  
  idtr--;
}

//______________________________________________________________________________
S_lpsrStaffBlock lpsrStaffBlock::create (
  S_msrStaff     staff)
{
  lpsrStaffBlock* o = new lpsrStaffBlock (
    staff);
  assert(o!=0);
  return o;
}

lpsrStaffBlock::lpsrStaffBlock (
  S_msrStaff     staff)
    : lpsrElement (0) // JMI
{
  fStaff = staff;
}

lpsrStaffBlock::~lpsrStaffBlock() {}

void lpsrStaffBlock::appendVoiceUseToStaffBlock (S_msrVoice voice)
{
  S_lpsrUseVoiceCommand
    useVoiceCommand =
      lpsrUseVoiceCommand::create (
        fInputLineNumber,
        voice);
  
  fStaffBlockElements.push_back (useVoiceCommand);
}

void lpsrStaffBlock::appendLyricsUseToStaffBlock (S_msrStanza stanza)
{
  S_lpsrNewLyricsBlock
    newLyricsCommand =
      lpsrNewLyricsBlock::create (
        fInputLineNumber,
        stanza,
        stanza->getStanzaVoiceUplink ());
  
  fStaffBlockElements.push_back (newLyricsCommand);
}

void lpsrStaffBlock::acceptIn (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrStaffBlock::acceptIn()" <<
      endl;
      
  if (visitor<S_lpsrStaffBlock>*
    p =
      dynamic_cast<visitor<S_lpsrStaffBlock>*> (v)) {
        S_lpsrStaffBlock elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrStaffBlock::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void lpsrStaffBlock::acceptOut (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrStaffBlock::acceptOut()" <<
      endl;

  if (visitor<S_lpsrStaffBlock>*
    p =
      dynamic_cast<visitor<S_lpsrStaffBlock>*> (v)) {
        S_lpsrStaffBlock elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrStaffBlock::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void lpsrStaffBlock::browseData (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrStaffBlock::browseData()" <<
      endl;

  for (
    list<S_msrElement>::iterator i = fStaffBlockElements.begin();
    i != fStaffBlockElements.end();
    i++) {
    // browse the element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% <== lpsrStaffBlock::browseData()" <<
      endl;
}

ostream& operator<< (ostream& os, const S_lpsrStaffBlock& scr)
{
  scr->print (os);
  return os;
}

void lpsrStaffBlock::print (ostream& os)
{
  os <<
    "StaffBlock for staff \"" <<
    fStaff->getStaffName () <<
    "\" (" << fStaff->staffKindAsString () <<
    "), " <<
    singularOrPlural (
      fStaffBlockElements.size(), "element", "elements") <<
    endl;

  idtr++;

  if (fStaffBlockElements.size()) {  
    list<S_msrElement>::const_iterator
      iBegin = fStaffBlockElements.begin(),
      iEnd   = fStaffBlockElements.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
  }

  idtr--;
}

//______________________________________________________________________________
S_lpsrPartBlock lpsrPartBlock::create (
  S_msrPart      part)
{
  lpsrPartBlock* o = new lpsrPartBlock (
    part);
  assert(o!=0);
  return o;
}

lpsrPartBlock::lpsrPartBlock (
  S_msrPart      part)
    : lpsrElement (0) // JMI 
{
  fPart = part;
}

lpsrPartBlock::~lpsrPartBlock() {}

void lpsrPartBlock::acceptIn (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrPartBlock::acceptIn()" <<
      endl;
      
  if (visitor<S_lpsrPartBlock>*
    p =
      dynamic_cast<visitor<S_lpsrPartBlock>*> (v)) {
        S_lpsrPartBlock elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrPartBlock::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void lpsrPartBlock::acceptOut (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrPartBlock::acceptOut()" <<
      endl;

  if (visitor<S_lpsrPartBlock>*
    p =
      dynamic_cast<visitor<S_lpsrPartBlock>*> (v)) {
        S_lpsrPartBlock elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrPartBlock::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void lpsrPartBlock::browseData (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrPartBlock::browseData()" <<
      endl;

  for (
    list<S_msrElement>::iterator i = fPartBlockElements.begin();
    i != fPartBlockElements.end();
    i++) {
    // browse the element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% <== lpsrPartBlock::browseData()" <<
      endl;
}

ostream& operator<< (ostream& os, const S_lpsrPartBlock& scr)
{
  scr->print (os);
  return os;
}

void lpsrPartBlock::print (ostream& os)
{
  os <<
    "PartBlock" << " " <<
    "for part " << fPart->getPartCombinedName () <<
    ", " <<
    singularOrPlural (
      fPartBlockElements.size(), "element", "elements") <<
    endl;

  idtr++;

  const int fieldWidth = 28;

  os << left <<
    idtr <<
      setw(fieldWidth) << string ("(") + "PartName" << " = \"" <<
      fPart->getPartName () << "\")" <<
      endl <<
    idtr <<
      setw(fieldWidth) << string ("(") + "PartAbbreviation" << " = \"" <<
      fPart->getPartAbbreviation () << "\")" <<
      endl <<

    idtr <<
      setw(fieldWidth) << "PartBlockInstrumentName" << " = \"" <<
      fPartBlockInstrumentName <<
      endl <<
    idtr <<
      setw(fieldWidth) << "PartBlockShortInstrumentName" << " = \"" <<
      fPartBlockShortInstrumentName <<
      endl;

  os << endl;

  if (fPartBlockElements.size()) {  
    list<S_msrElement>::const_iterator
      iBegin = fPartBlockElements.begin(),
      iEnd   = fPartBlockElements.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  idtr--;
}

//______________________________________________________________________________
S_lpsrPartgroupBlock lpsrPartgroupBlock::create (
  S_msrPartgroup partgroup)
{
  lpsrPartgroupBlock* o = new lpsrPartgroupBlock (
    partgroup);
  assert(o!=0);
  return o;
}

lpsrPartgroupBlock::lpsrPartgroupBlock (
  S_msrPartgroup partgroup)
    : lpsrElement (0) // JMI
{
  fPartgroup = partgroup;
}

lpsrPartgroupBlock::~lpsrPartgroupBlock() {}

void lpsrPartgroupBlock::acceptIn (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrPartgroupBlock::acceptIn()" <<
      endl;
      
  if (visitor<S_lpsrPartgroupBlock>*
    p =
      dynamic_cast<visitor<S_lpsrPartgroupBlock>*> (v)) {
        S_lpsrPartgroupBlock elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrPartgroupBlock::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void lpsrPartgroupBlock::acceptOut (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrPartgroupBlock::acceptOut()" <<
      endl;

  if (visitor<S_lpsrPartgroupBlock>*
    p =
      dynamic_cast<visitor<S_lpsrPartgroupBlock>*> (v)) {
        S_lpsrPartgroupBlock elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrPartgroupBlock::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void lpsrPartgroupBlock::browseData (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrPartgroupBlock::browseData()" <<
      endl;

  for (
    list<S_msrElement>::iterator i = fPartgroupBlockElements.begin();
    i != fPartgroupBlockElements.end();
    i++) {
    // browse the element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% <== lpsrPartgroupBlock::browseData()" <<
      endl;
}

ostream& operator<< (ostream& os, const S_lpsrPartgroupBlock& scr)
{
  scr->print (os);
  return os;
}

void lpsrPartgroupBlock::print (ostream& os)
{
  os <<
    endl <<
    idtr <<
    "PartgroupBlock" << " " <<
    "for partgroup \"" <<
    fPartgroup->getPartgroupCombinedName () <<
    "\", " << fPartgroup->partgroupSymbolKindAsString () <<
    ", " <<
    singularOrPlural (
      fPartgroupBlockElements.size(), "element", "elements") <<
    endl << endl;

  idtr++;

  if (fPartgroupBlockElements.size()) {  
    list<S_msrElement>::const_iterator
      iBegin = fPartgroupBlockElements.begin(),
      iEnd   = fPartgroupBlockElements.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  idtr--;
}

//______________________________________________________________________________
S_lpsrScoreBlock lpsrScoreBlock::create (
  int            inputLineNumber)
{
  lpsrScoreBlock* o = new lpsrScoreBlock (
    inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrScoreBlock::lpsrScoreBlock (
  int            inputLineNumber)
    : lpsrElement (inputLineNumber)
{
  // create the score command parallel music
  fScoreBlockParallelMusic =
    lpsrParallelMusic::create (
      inputLineNumber,
      lpsrParallelMusic::kEndOfLine);
  
  // create the score command layout
  fScoreBlockLayout =
    lpsrLayout::create (
      inputLineNumber);
  
  // create the score command midi
  fScoreBlockMidi =
    msrMidi::create (
      inputLineNumber,
      gLilypondOptions->fMidiTempoDuration,
      gLilypondOptions->fMidiTempoPerSecond);
}

lpsrScoreBlock::~lpsrScoreBlock() {}

void lpsrScoreBlock::appendPartgroupBlockToParallelMusic (
  S_lpsrPartgroupBlock partgroupBlock)
{
  if (gGeneralOptions->fTracePartgroups)
    cerr << idtr <<
      "Appending part group block " <<
       partgroupBlock-> getPartgroup ()-> getPartgroupCombinedName() <<
       " to LPSR score" <<
       endl;

  fScoreBlockParallelMusic->
    addElementToParallelMusic (partgroupBlock);
    
//               fScoreBlockElements.push_back(partgroupBlock);
}

void lpsrScoreBlock::appendVoiceUseToParallelMusic (
  S_lpsrUseVoiceCommand voiceUse)
{
  if (gGeneralOptions->fTraceVoices)
    cerr << idtr <<
      "Appending the use of voice \"" <<
       voiceUse-> getVoice ()-> getVoiceName() <<
       "\" to LPSR score" <<
       endl;

  fScoreBlockParallelMusic->
    addElementToParallelMusic (voiceUse);
    
//     JMI             fScoreBlockElements.push_back(voiceUse);
}
                  
void lpsrScoreBlock::appendLyricsUseToParallelMusic (
  S_lpsrNewLyricsBlock lyricsUse)
{
  if (gGeneralOptions->fTraceLyrics)
    cerr << idtr <<
      "Appending the use of stanza " <<
       lyricsUse-> getStanza ()-> getStanzaName() <<
       " to LPSR score" <<
       endl;

  fScoreBlockParallelMusic->
    addElementToParallelMusic (lyricsUse);
}

void lpsrScoreBlock::acceptIn (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrScoreBlock::acceptIn()" <<
      endl;
      
  if (visitor<S_lpsrScoreBlock>*
    p =
      dynamic_cast<visitor<S_lpsrScoreBlock>*> (v)) {
        S_lpsrScoreBlock elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrScoreBlock::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void lpsrScoreBlock::acceptOut (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrScoreBlock::acceptOut()" <<
      endl;

  if (visitor<S_lpsrScoreBlock>*
    p =
      dynamic_cast<visitor<S_lpsrScoreBlock>*> (v)) {
        S_lpsrScoreBlock elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrScoreBlock::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void lpsrScoreBlock::browseData (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrScoreBlock::browseData()" <<
      endl;

  {
    // browse the score command parallel music
    msrBrowser<lpsrParallelMusic> browser (v);    
    browser.browse (*fScoreBlockParallelMusic);
  }

/* JMI
  for (
    vector<S_msrElement>::iterator i = fScoreBlockElements.begin();
    i != fScoreBlockElements.end();
    i++) {
    // browse the element
 //   msrBrowser<msrElement> browser (v);
 //   browser.browse (*(*i));
  } // for
*/
  {
    // browse the score command layout
    msrBrowser<lpsrLayout> browser (v);    
    browser.browse (*fScoreBlockLayout);
  }

  {
    // browse the score command midi
    msrBrowser<msrMidi> browser (v);    
    browser.browse (*fScoreBlockMidi);
  }

  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% <== lpsrScoreBlock::browseData()" <<
      endl;
}

ostream& operator<< (ostream& os, const S_lpsrScoreBlock& scr)
{
  scr->print (os);
  return os;
}

void lpsrScoreBlock::print (ostream& os)
{
  os << "ScoreBlock" << endl << endl;

  idtr++;

  os << idtr <<
    fScoreBlockParallelMusic <<
    endl;
    
  os << idtr <<
    fScoreBlockLayout <<
    endl;
  
  os << idtr <<
    fScoreBlockMidi <<
    endl;

  idtr--;
}

//______________________________________________________________________________
S_lpsrScore lpsrScore::create (
  int            inputLineNumber,
  S_msrScore     mScore)
{
  lpsrScore* o = new lpsrScore (
    inputLineNumber, mScore);
  assert(o!=0);
  return o;
}

lpsrScore::lpsrScore (
  int            inputLineNumber,
  S_msrScore     mScore)
    : lpsrElement (inputLineNumber)
{
  fMsrScore = mScore;

  // create the LilyPond version assoc
  fLilyPondVersion =
    lpsrLilypondVarValAssoc::create (
      inputLineNumber,
      lpsrLilypondVarValAssoc::kUncommented,
      lpsrLilypondVarValAssoc::kWithBackslash,
      "version",
      lpsrLilypondVarValAssoc::kSpace,
      lpsrLilypondVarValAssoc::kQuotesAroundValue,
      "2.19",
      lpsrLilypondVarValAssoc::g_VarValAssocNoUnit,
      lpsrLilypondVarValAssoc::g_VarValAssocNoComment,
      lpsrLilypondVarValAssoc::kWithEndl);

  // create the input source name comment
  {
  stringstream s;

  s <<
    "Generated by xml2lilypond v" << currentVersionNumber () <<
    " from ";

  if (gGeneralOptions->fInputSourceName == "-")
    s << "standard input";
  else
    s << "\"" << gGeneralOptions->fInputSourceName << "\"";

  fInputSourceNameComment =
    lpsrComment::create (
      inputLineNumber,
      s.str(),
      lpsrComment::kNoGapAfterwards);
  }

  // create the translation date comment
  {
  string
    comment =
      "on " + gGeneralOptions->fTranslationDate +
      ", through:";
      
  fTranslationDateComment =
    lpsrComment::create (
      inputLineNumber,
      comment,
      lpsrComment::kNoGapAfterwards);
  }

  // do the command line long and short options differ?
  bool
    longAndShortOptionsDiffer =
      gGeneralOptions->fCommandLineShortOptions
        !=
      gGeneralOptions->fCommandLineLongOptions;

    
  // create the command line long options comment
  {
  stringstream s;
  
  s <<
    "  " <<
    gGeneralOptions->fProgramName << " " <<
    gGeneralOptions->fCommandLineLongOptions <<
    gGeneralOptions->fInputSourceName;

  if (longAndShortOptionsDiffer)
    s <<
      endl <<
      "% or:";
  
  fCommandLineLongOptionsComment =
    lpsrComment::create (
      inputLineNumber,
      s.str (),
      longAndShortOptionsDiffer
        ? lpsrComment::kNoGapAfterwards
        : lpsrComment::kGapAfterwards);
  }

  if (longAndShortOptionsDiffer) {
    // create the command line short options comment
    stringstream s;
    
    s <<
      "  " <<
      gGeneralOptions->fProgramName << " " <<
      gGeneralOptions->fCommandLineShortOptions <<
      gGeneralOptions->fInputSourceName;
    
    fCommandLineShortOptionsComment =
      lpsrComment::create (
        inputLineNumber,
        s.str (),
        lpsrComment::kGapAfterwards);
  }

  // create the global staff size assoc
  fGlobalStaffSizeAssoc =
    lpsrSchemeVarValAssoc::create (
      inputLineNumber,
      lpsrSchemeVarValAssoc::kCommented,
      "set-global-staff-size",
      "20", // the LilyPond default
      "Uncomment and adapt next line as needed (default is 20)",
      lpsrSchemeVarValAssoc::kWithEndlTwice);

  // initialize Scheme functions informations
  fTongueSchemeFunctionNeeded = false;
  
  // create the header
  fHeader =
    lpsrHeader::create (
      inputLineNumber);
  
  // create the paper
  fPaper =
    lpsrPaper::create (
      inputLineNumber);

  // create the score layout
  fScoreLayout =
    lpsrLayout::create (
      inputLineNumber);

  // create the 'myBreakIsBreak' assoc
  {
  lpsrLilypondVarValAssoc::lpsrCommentedKind
    commentedKind =
      gLilypondOptions->fDontKeepLineBreaks
        ? lpsrLilypondVarValAssoc::kCommented
        : lpsrLilypondVarValAssoc::kUncommented;

  fMyBreakIsBreakAssoc =
    lpsrLilypondVarValAssoc::create (
      inputLineNumber,
      commentedKind,
      lpsrLilypondVarValAssoc::kWithoutBackslash,
      "myBreak",
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kNoQuotesAroundValue,
      "{ \\break }",
      lpsrLilypondVarValAssoc::g_VarValAssocNoUnit,
      "Pick your choice from the next two lines as needed",
      lpsrLilypondVarValAssoc::kWithoutEndl);
  }
  
  // create the 'myBreakIsEmpty' assoc
  {
  lpsrLilypondVarValAssoc::lpsrCommentedKind
    commentedKind =
      gLilypondOptions->fDontKeepLineBreaks
        ? lpsrLilypondVarValAssoc::kUncommented
        : lpsrLilypondVarValAssoc::kCommented;

  fMyBreakIsEmptyAssoc =
    lpsrLilypondVarValAssoc::create (
      inputLineNumber,
      commentedKind,
      lpsrLilypondVarValAssoc::kWithoutBackslash,
      "myBreak",
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kNoQuotesAroundValue,
      "{ }",
      lpsrLilypondVarValAssoc::g_VarValAssocNoUnit,
      lpsrLilypondVarValAssoc::g_VarValAssocNoComment,
      lpsrLilypondVarValAssoc::kWithEndl);
  }

  if (gLilypondOptions->fGenerateGlobal) {
    // create the 'global' assoc
    fGlobalAssoc =
    lpsrLilypondVarValAssoc::create (
      inputLineNumber,
      lpsrLilypondVarValAssoc::kUncommented,
      lpsrLilypondVarValAssoc::kWithoutBackslash,
      "global",
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kNoQuotesAroundValue,
      "{ }",
      lpsrLilypondVarValAssoc::g_VarValAssocNoUnit,
      "Place whatever you need in the following variable",
      lpsrLilypondVarValAssoc::kWithEndl);
  }
  
  // create the score command
  fScoreBlock =
    lpsrScoreBlock::create (
      inputLineNumber);
}

lpsrScore::~lpsrScore() {}

void lpsrScore::setTongueSchemeFunctionNeeded ()
{
  if (! fTongueSchemeFunctionNeeded) {
    string
      schemeFunctionName =
        "tongue",
        
      schemeFunctionDescription =
  R"(
  % Creates multiple tongue technicals, argument is a number.
  % Example: 'c4 -\tongue #3' creates a triple tongue.
  )",
  
      schemeFunctionCode =
  R"(
  tongue =
  #(define-music-function (parser location dots) (integer?)
     (let ((script (make-music 'ArticulationEvent
                     'articulation-type "staccato")))
       (set! (ly:music-property script 'tweaks)
             (acons 'stencil
               (lambda (grob)
                 (let ((stil (ly:script-interface::print grob)))
                   (let loop ((count (1- dots)) (new-stil stil))
                     (if (> count 0)
                         (loop (1- count)
                           (ly:stencil-combine-at-edge new-stil X RIGHT stil 0.2))
                         (ly:stencil-aligned-to new-stil X CENTER)))))
               (ly:music-property script 'tweaks)))
       script))
  )";

    if (true || gGeneralOptions->fTraceGeneral) { // JMI
      cerr << idtr <<
        "Creating Scheme funcction '" << schemeFunctionName << "'" <<
        endl;
    }
  
    // create the Scheme function
    S_lpsrSchemeFunction
      tongueSchemeFunction =
        lpsrSchemeFunction::create (
          1, // inputLineNumber, JMI ???
          schemeFunctionName,
          schemeFunctionDescription,
          schemeFunctionCode);
  
    // register it in the Scheme functions map
    fScoreSchemeFunctionsMap [schemeFunctionName] =
      tongueSchemeFunction;

    fTongueSchemeFunctionNeeded = true;    
  }
}

/* JMI
void lpsrScore::appendPartgroupToStoreCommand (S_msrVoice voice)
{
  S_lpsrPartgroupBlock
    partgroupBlock =
      lpsrPartgroupBlock::create (
        fInputLineNumber,
        voice);
  
  fScoreBlock->
    appendVoiceUseToParallelMusic (useVoiceCommand);
}
*/

void lpsrScore::appendVoiceUseToStoreCommand (S_msrVoice voice)
{
  S_lpsrUseVoiceCommand
    useVoiceCommand =
      lpsrUseVoiceCommand::create (
        fInputLineNumber,
        voice);
  
  fScoreBlock->
    appendVoiceUseToParallelMusic (useVoiceCommand);
}

void lpsrScore::appendLyricsUseToStoreCommand (S_msrStanza stanza)
{
  S_lpsrNewLyricsBlock
    newLyricsCommand =
      lpsrNewLyricsBlock::create (
        fInputLineNumber,
        stanza,
        stanza->getStanzaVoiceUplink ());
  
  fScoreBlock->
    appendLyricsUseToParallelMusic (newLyricsCommand);
}

void lpsrScore::acceptIn (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrScore::acceptIn()" <<
      endl;
      
  if (visitor<S_lpsrScore>*
    p =
      dynamic_cast<visitor<S_lpsrScore>*> (v)) {
        S_lpsrScore elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrScore::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void lpsrScore::acceptOut (basevisitor* v) {
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrScore::acceptOut()" <<
      endl;

  if (visitor<S_lpsrScore>*
    p =
      dynamic_cast<visitor<S_lpsrScore>*> (v)) {
        S_lpsrScore elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors)
          cerr << idtr <<
            "% ==> Launching lpsrScore::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void lpsrScore::browseData (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% ==> lpsrScore::browseData()" <<
      endl;

  {
    // browse the score LilyPond version
    msrBrowser<lpsrLilypondVarValAssoc> browser (v);
    browser.browse (*fLilyPondVersion);
  }

  {
    // browse the input source name comment
    msrBrowser<lpsrComment> browser (v);
    browser.browse (*fInputSourceNameComment);
  }

  {
    // browse the translation date comment
    msrBrowser<lpsrComment> browser (v);
    browser.browse (*fTranslationDateComment);
  }

  {
    // browse the command line long options comment
    msrBrowser<lpsrComment> browser (v);
    browser.browse (*fCommandLineLongOptionsComment);
  }

  if (fCommandLineShortOptionsComment) {
    // browse the command line short options comment
    msrBrowser<lpsrComment> browser (v);
    browser.browse (*fCommandLineShortOptionsComment);
  }

  {
    // browse the score global size
    msrBrowser<lpsrSchemeVarValAssoc> browser (v);
    browser.browse (*fGlobalStaffSizeAssoc);
  }

  {
    // browse the Scheme function map
    for (
      map<string, S_lpsrSchemeFunction>::const_iterator i =
        fScoreSchemeFunctionsMap.begin();
      i != fScoreSchemeFunctionsMap.end();
      i++) {
      // browse the Scheme function
      msrBrowser<lpsrSchemeFunction> browser (v);
      browser.browse (*(*i).second);
    } // for
  }

  {
    // browse the score header
    msrBrowser<lpsrHeader> browser (v);
    browser.browse (*fHeader);
  }

  {
    // browse the score paper
    msrBrowser<lpsrPaper> browser (v);
    browser.browse (*fPaper);
  }

  {
    // browse the score layout
    msrBrowser<lpsrLayout> browser (v);
    browser.browse (*fScoreLayout);
  }

  {
    // browse the myBreakIsBreak assoc
    msrBrowser<lpsrLilypondVarValAssoc> browser (v);
    browser.browse (*fMyBreakIsBreakAssoc);
  }
  {
    // browse the myBreakIsEmpty assoc
    msrBrowser<lpsrLilypondVarValAssoc> browser (v);
    browser.browse (*fMyBreakIsEmptyAssoc);
  }

  if (fGlobalAssoc) {
    // browse the 'global' assoc
    msrBrowser<lpsrLilypondVarValAssoc> browser (v);
    browser.browse (*fGlobalAssoc);
  }

  {
    // browse the voices and stanzas list
    for (
      list<S_msrElement>::iterator i = fScoreElements.begin();
      i != fScoreElements.end();
      i++) {
      // browse the element
      msrBrowser<msrElement> browser (v);
      browser.browse (*(*i));
    } // for
  }

  {
    // browse the score command
    msrBrowser<lpsrScoreBlock> browser (v);    
    browser.browse (*fScoreBlock);
  }

  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "% <== lpsrScore::browseData()" <<
      endl;
}

ostream& operator<< (ostream& os, const S_lpsrScore& scr)
{
  scr->print (os);
  return os;
}

void lpsrScore::print (ostream& os)
{
  os << idtr <<
    "LPSR Structure" <<
    endl << endl;

  idtr++;

  // print the MSR structure (without the voices)
  os << idtr;
  fMsrScore->printStructure (os);
  os << endl;

  // are some Scheme functions needed?
  os <<
    idtr <<
      "TongueSchemeFunctionNeeded: " <<
      booleanAsString (fTongueSchemeFunctionNeeded) <<
      endl <<
      endl;

  // print LPSR basic information
  os <<
    idtr << fLilyPondVersion <<
    endl;
  os <<
    idtr << fGlobalStaffSizeAssoc <<
    endl;
  os <<
    idtr << fHeader <<
    endl;
  os <<
    idtr << fPaper <<
    endl;
  os <<
    idtr << fScoreLayout <<
    endl;

// myBreakAssoc, globalAssoc? JMI

  // print the voices
  if (fScoreElements.size()) {  
    list<S_msrElement>::const_iterator
      iBegin = fScoreElements.begin(),
      iEnd   = fScoreElements.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
    os << endl;
  }

  // print the score block
  os <<
    idtr << fScoreBlock <<
    endl;

  idtr--;
}


}
