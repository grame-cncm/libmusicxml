/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __lpsrOptions__
#define __lpsrOptions__

#include "lpsr.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
// LPSR options

// trace and display

#define _TRACE_LPSR_LONG_NAME_  "traceLpsr"
#define _TRACE_LPSR_SHORT_NAME_ "tlpsr"

#define _TRACE_LPSR_VISITORS_LONG_NAME_  "traceLpsrVisitors"
#define _TRACE_LPSR_VISITORS_SHORT_NAME_ "tlpvisits"

#define _DISPLAY_LPSR_LONG_NAME_  "displayLpsr"
#define _DISPLAY_LPSR_SHORT_NAME_ "lpsr"

// Scheme functions
#define _TRACE_SCHEME_FUNCTIONS_LONG_NAME_  "traceSchemeFunctions"
#define _TRACE_SCHEME_FUNCTIONS_SHORT_NAME_ "tscheme"

// languages

#define _LPSR_PITCHES_LANGUAGE_LONG_NAME_  "lpsrPitchesLanguage"
#define _LPSR_PITCHES_LANGUAGE_SHORT_NAME_ "lppl"

#define _LPSR_CHORDS_LANGUAGE_LONG_NAME_  "lpsrChordsLanguage"
#define _LPSR_CHORDS_LANGUAGE_SHORT_NAME_ "lpcl"

class EXP lpsrOptions : public smartable {
  public:

    static SMARTP<lpsrOptions> create ();
    
    SMARTP<lpsrOptions>        createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeLpsrOptions (
                            bool boolOptionsInitialValue);
    
    void                  printLpsrOptionsHelp ();

    void                  printLpsrOptionsValues (int fieldWidth);
    
  public:
  
    // constructors/destructor
    // ------------------------------------------------------

    lpsrOptions();
    
    virtual ~lpsrOptions();

    // set and get
    // ------------------------------------------------------

    bool                  setLpsrQuarterTonesPitchesLanguage (
                            string language);    
 
    bool                  setLpsrChordsLanguage (
                            string language);    
 
  public:

    // trace and display
    // --------------------------------------
    
    bool                  fTraceLpsr;
    
    bool                  fTraceLpsrVisitors;
    
    bool                  fDisplayLpsr;
    
    // Scheme functions
    bool                  fTraceSchemeFunctions;

    // languages
    // --------------------------------------

    msrQuarterTonesPitchesLanguage
                          fLpsrQuarterTonesPitchesLanguage;
    
    lpsrChordsLanguage    fLpsrChordsLanguage;
};
typedef SMARTP<lpsrOptions> S_lpsrOptions;

extern S_lpsrOptions gLpsrOptions;
extern S_lpsrOptions gLpsrOptionsUserChoices;
extern S_lpsrOptions gLpsrOptionsWithDetailedTrace;

//______________________________________________________________________________
// LilyPond options

// time
// --------------------------------------

#define _NUMERICAL_TIME_LONG_NAME_  "numericalTime"
#define _NUMERICAL_TIME_SHORT_NAME_ "numt"

// notes
// --------------------------------------

#define _ABSOLUTE_LONG_NAME_  "absolute"
#define _ABSOLUTE_SHORT_NAME_ "abs"

#define _STEMS_LONG_NAME_  "stems"
#define _STEMS_SHORT_NAME_ ""

#define _NO_AUTO_BEAMING_LONG_NAME_  "noAutoBeaming"
#define _NO_AUTO_BEAMING_SHORT_NAME_ "noab"

#define _ROMAN_STRING_NUMBERS_LONG_NAME_  "romanStringNumbers"
#define _ROMAN_STRING_NUMBERS_SHORT_NAME_ "rsn"

#define _AVOID_OPEN_STRINGS_LONG_NAME_  "avoidOpenString"
#define _AVOID_OPEN_STRINGS_SHORT_NAME_ "aos"

#define _ACCIDENTAL_STYLE_LONG_NAME_  "accidentalStyle"
#define _ACCIDENTAL_STYLE_SHORT_NAME_ "as"

#define _NOTE_INPUT_LINE_NUMBERS_LONG_NAME_  "noteInputLineNumbers"
#define _NOTE_INPUT_LINE_NUMBERS_SHORT_NAME_ "niln"

// bars
// --------------------------------------

#define _SHOW_ALL_BAR_NUMBERS_LONG_NAME_  "showAllBarNumbers"
#define _SHOW_ALL_BAR_NUMBERS_SHORT_NAME_ "sabn"

#define _COMPRESS_MULTI_MEASURE_RESTS_LONG_NAME_  "compressMultiMeasureRests"
#define _COMPRESS_MULTI_MEASURE_RESTS_SHORT_NAME_ "cmmr"

// line breaks
// --------------------------------------

#define _DONT_KEEP_LINE_BREAKS_LONG_NAME_  "dontKeepLineBreaks"
#define _DONT_KEEP_LINE_BREAKS_SHORT_NAME_ "noBreaks"

#define _BREAK_LINES_AT_INCOMPLETE_RIGHT_MEASURES_LONG_NAME_  "breakLinesAtIncompleteRightMeasures"
#define _BREAK_LINES_AT_INCOMPLETE_RIGHT_MEASURES_SHORT_NAME_ "blairm"

#define _SEPARATOR_LINE_EVERY_N_MEASURES_LONG_NAME_  "separatorLineEveryNMeasures"
#define _SEPARATOR_LINE_EVERY_N_MEASURES_SHORT_NAME_ "slenm"

// staves
// --------------------------------------

#define _MODERN_TAB_LONG_NAME_  "modernTab"
#define _MODERN_TAB_SHORT_NAME_ "mtab"

// tuplets
// --------------------------------------

#define _TUPLETS_ON_A_LINE_LONG_NAME_  "tupletsOnALine"
#define _TUPLETS_ON_A_LINE_SHORT_NAME_ "toal"

// repeats
// --------------------------------------

#define _REPEAT_BRACKETS_LONG_NAME_  "repeatBrackets"
#define _REPEAT_BRACKETS_SHORT_NAME_ "rbracks"

// ornaments
// --------------------------------------

#define _DELAYED_ORNAMENTS_FRACTION_LONG_NAME_  "delayedOrnamentsFraction"
#define _DELAYED_ORNAMENTS_FRACTION_SHORT_NAME_ "dof"

// code generation
// --------------------------------------

#define _COMMENTS_LONG_NAME_  "comments"
#define _COMMENTS_SHORT_NAME_ "com"

#define _GLOBAL_LONG_NAME_  "global"
#define _GLOBAL_SHORT_NAME_ ""

#define _DISPLAY_MUSIC_LONG_NAME_  "displayMusic"
#define _DISPLAY_MUSIC_SHORT_NAME_ "dm"

#define _NO_LILYPOND_CODE_LONG_NAME_  "noLilypondCode"
#define _NO_LILYPOND_CODE_SHORT_NAME_ "nolpc"

#define _NO_LILYPOND_LYRICS_LONG_NAME_  "noLilypondLyrics"
#define _NO_LILYPOND_LYRICS_SHORT_NAME_ "nolpl"

#define _LILYPOND_COMPILE_DATE_LONG_NAME_  "lilypondCompileDate"
#define _LILYPOND_COMPILE_DATE_SHORT_NAME_ "lpcd"

// midi
// --------------------------------------

#define _MIDI_TEMPO_LONG_NAME_  "midiTempo"
#define _MIDI_TEMPO_SHORT_NAME_ "mt"

#define _NO_MIDI_LONG_NAME_  "noMidi"
#define _NO_MIDI_SHORT_NAME_ ""


class EXP lilypondOptions : public smartable {
  public:

    static SMARTP<lilypondOptions> create ();
    
    SMARTP<lilypondOptions>        createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeLilypondOptions (
                            bool boolOptionsInitialValue);
    
    void                  printLilypondOptionsHelp ();

    void                  printLilypondOptionsValues (int fieldWidth);
    
  public:
  
    // constructors/destructor
    // ------------------------------------------------------

    lilypondOptions();
    
    virtual ~lilypondOptions();

    // set and get
    // ------------------------------------------------------

    bool                  setAccidentalStyle (
                            string accidentalStyle);    
 
  public:

    // time
    // --------------------------------------
    
    bool                  fNumericalTime;

    // notes
    // --------------------------------------
    
    bool                  fAbsoluteOctaves;
    
    bool                  fStems;
    bool                  fNoAutoBeaming;
    
    bool                  fRomanStringNumbers;
    bool                  fAvoidOpenString;
    
    string                fAccidentalStyle;
    
    bool                  fCompressMultiMeasureRests;

    bool                  fInputLineNumbers;

    // bars
    // --------------------------------------
    
    bool                  fShowAllBarNumbers;
    
    // line breaks
    // --------------------------------------
    
    bool                  fDontKeepLineBreaks;
    
    bool                  fBreakLinesAtIncompleteRightMeasures;
    
    bool                  fSeparatorLineEveryNMeasures;
    int                   fSeparatorLineEveryNMeasuresValue;

    // staves
    // --------------------------------------
    
    bool                  fModernTab;

    // tuplets
    // --------------------------------------
    
    bool                  fTupletsOnALine;

    // repeats
    // --------------------------------------
    
    bool                  fRepeatBrackets;

    // ornaments
    // --------------------------------------

    int                   fDelayedOrnamentFractionNumerator;
    int                   fDelayedOrnamentFractionDenominator;

    // code generation
    // --------------------------------------

    bool                  fComments;

    bool                  fGlobal;
    
    bool                  fDisplayMusic;

    bool                  fNoLilypondCode;

    bool                  fNoLilypondLyrics;

    bool                  fLilypondCompileDate;

    // midi
    // --------------------------------------
    
    string                fMidiTempoDuration;
    int                   fMidiTempoPerSecond;
    
    bool                  fNoMidiCommand;


// JMI ???

    bool                  fKeepStaffSize; // JMI

    bool                  fSilentVoices; // JMI

  private:

    set<string>           fLilypondAccidentalStyles;
};
typedef SMARTP<lilypondOptions> S_lilypondOptions;

extern S_lilypondOptions gLilypondOptions;
extern S_lilypondOptions gLilypondOptionsUserChoices;
extern S_lilypondOptions gLilypondOptionsWithDetailedTrace;


/*! @} */

}


#endif
