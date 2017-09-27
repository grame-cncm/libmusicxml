/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __lilypondOptions__
#define __lilypondOptions__

#include <set>

#include "optionsHandling.h"


using namespace std;

namespace MusicXML2 
{

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

#define _ALL_DURATIONS_LONG_NAME_  "allDurations"
#define _ALL_DURATIONS_SHORT_NAME_ "alldurs"

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

// page breaks
// --------------------------------------

#define _DONT_KEEP_PAGE_BREAKS_LONG_NAME_  "dontKeepPageBreaks"
#define _DONT_KEEP_PAGE_BREAKS_SHORT_NAME_ "noPageBreaks"

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

// fonts
// --------------------------------------

#define _JAZZ_FONTS_LONG_NAME_  "jazzFonts"
#define _JAZZ_FONTS_SHORT_NAME_ "jazz"

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

// score notation
// --------------------------------------

#define _JIANPU_LONG_NAME_  "jianpu"
#define _JIANPU_SHORT_NAME_ ""

#define _ABC_LONG_NAME_  "abc"
#define _ABC_SHORT_NAME_ ""

// midi
// --------------------------------------

#define _MIDI_TEMPO_LONG_NAME_  "midiTempo"
#define _MIDI_TEMPO_SHORT_NAME_ "mt"

#define _NO_MIDI_LONG_NAME_  "noMidi"
#define _NO_MIDI_SHORT_NAME_ ""


class EXP lilypondOptions : public msrOptionsGroup
{
  public:

    // data types
    // ------------------------------------------------------

    enum scoreNotationKind {
      kWesternNotation, kJianpuNotation, kABCNotation };

    static string scoreNotationKindAsString (
      scoreNotationKind notationKind);

  public:

    // creation
    // ------------------------------------------------------

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
    
    bool                  fAllDurations;
    
    bool                  fStems;
    bool                  fNoAutoBeaming;
    
    bool                  fRomanStringNumbers;
    bool                  fAvoidOpenStrings;
    
    string                fAccidentalStyle;
    
    bool                  fCompressMultiMeasureRests;

    bool                  fNoteInputLineNumbers;

    // bars
    // --------------------------------------
    
    bool                  fShowAllBarNumbers;
    
    // line breaks
    // --------------------------------------
    
    bool                  fDontKeepLineBreaks;
    
    bool                  fBreakLinesAtIncompleteRightMeasures;
    
    int                   fSeparatorLineEveryNMeasures;

    // page breaks
    // --------------------------------------
    
    bool                  fDontKeepPageBreaks;
    
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

    rational              fDelayedOrnamentsFraction;

    // fonts
    // --------------------------------------

    bool                  fJazzFonts;

    // code generation
    // --------------------------------------

    bool                  fComments;

    bool                  fGlobal;
    
    bool                  fDisplayMusic;

    bool                  fNoLilypondCode;

    bool                  fNoLilypondLyrics;

    bool                  fLilypondCompileDate;

    // score notation
    // --------------------------------------

    scoreNotationKind     fScoreNotationKind;
    
    // midi
    // --------------------------------------

    pair<string, int>     fMidiTempo;
    
    bool                  fNoMidi;


// JMI ???

    bool                  fKeepStaffSize; // JMI

    bool                  fSilentVoices; // JMI

  private:

    set<string>           fLilypondAccidentalStyles;
};
typedef SMARTP<lilypondOptions> S_lilypondOptions;
EXP ostream& operator<< (ostream& os, const S_lilypondOptions& elt);

extern S_lilypondOptions gLilypondOptions;
extern S_lilypondOptions gLilypondOptionsUserChoices;
extern S_lilypondOptions gLilypondOptionsWithDetailedTrace;


/*! @} */

}


#endif
