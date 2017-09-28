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

#include "utilities.h"

#include "lilypondOptions.h"


using namespace std;

namespace MusicXML2 
{

// useful shortcut macros
#define idtr indenter::gIndenter
#define tab  indenter::gIndenter.getSpacer ()

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
  : msrOptionsGroup (
    "hlp", "helpLilypond",
R"(LilyPond)"
    )
{
  initializeLilypondOptions (false);
}

lilypondOptions::~lilypondOptions()
{}

void lilypondOptions::initializeLilypondOptions (
  bool boolOptionsInitialValue)
{
  // time
  // --------------------------------------

  {
    // variables
    
    fNumericalTime = boolOptionsInitialValue;
  
    S_msrOptionsSubGroup timeSubGroup =
      msrOptionsSubGroup::create (
      "hlilyti", "helpLilypondtime",
R"(Time)"
      );
  
    appendOptionsSubGroup (timeSubGroup);

    timeSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "numt", "numericalTime",
R"(Generate numerical time signatures, such as '4/4' instead of 'C'.)",
          "numericalTime",
          fNumericalTime));
  }


  // notes
  // --------------------------------------

  {
    // variables
      
    fAbsoluteOctaves  = boolOptionsInitialValue;
    
    fAllDurations  = boolOptionsInitialValue;
  
    fStems          = boolOptionsInitialValue;
    fNoAutoBeaming  = boolOptionsInitialValue;
    
    fRomanStringNumbers = boolOptionsInitialValue;
    fAvoidOpenStrings    = boolOptionsInitialValue;
    
    fAccidentalStyle = "";
  
    fCompressMultiMeasureRests = boolOptionsInitialValue;
  
    fNoteInputLineNumbers = boolOptionsInitialValue;
  
    S_msrOptionsSubGroup notesSubGroup =
      msrOptionsSubGroup::create (
      "hlilyn", "helpLilypondNotes",
R"(Notes)"
      );
  
    appendOptionsSubGroup (notesSubGroup);

    notesSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "abs", "absolute",
R"(Generate LilyPond absolute note octaves. 
By default, relative octaves are generated.)",
          "absoluteOctaves",
          fAbsoluteOctaves));

    notesSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "alldurs", "allDurations",
R"(Generate all LilyPond durations. 
By default, a duration equal to preceding one met in the current voice
is omitted for code conciseness.)",
          "allDurations",
          fAllDurations));

    notesSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "stems", "",
R"(Generate \stemUp and \stemDown LilyPond commands.
By default, LilyPond will take care of that by itself.)",
          "stems",
          fStems));

    notesSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "noab", "noAutoBeaming",
R"(Generate '\set Voice.autoBeaming = ##f' in each voice 
to prevent LilyPond from handling beams automatically.)",
          "noAutoBeaming",
          fNoAutoBeaming));

    notesSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "rsn", "romanStringNumbers",
R"(Generate '\romanStringNumbers' in each voice 
for LilyPond to generate roman instead of arabic string numbers.)",
          "romanStringNumbers",
          fRomanStringNumbers));

    notesSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "aos", "avoidOpenStrings",
R"(Generate '\set TabStaff.restrainOpenStrings = ##t' in each voice 
to prevent LilyPond from using open strings.)",
          "avoidOpenStrings",
          fAvoidOpenStrings));
/* JMI
    notesSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "as", "accidentalStyle", // JMI
R"(Choose the LilyPond accidental styles among: 
  voice, modern, modern-cautionary, modern-voice, 
  modern-voice-cautionary, piano, piano-cautionary, 
  neo-modern, neo-modern-cautionary, neo-modern-voice,
  neo-modern-voice-cautionary, dodecaphonic, dodecaphonic-no-repeat,
  dodecaphonic-first, teaching, no-reset, forget.
The default is... 'default'.)",
          "accidentalStyle",
          fAccidentalStyle));
          */

    notesSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "cmmr", "compressMultiMeasureRests",
R"(Generate '\compressMMRests' at the beginning of voices.
By default, this command is commented.)",
          "compressMultiMeasureRests",
          fCompressMultiMeasureRests));

    notesSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "niln", "noteInputLineNumbers",
R"(Generate after each note and barline a comment containing
its MusicXML input line number.)",
          "noteInputLineNumbers",
          fNoteInputLineNumbers));
  }

  
  // bars
  // --------------------------------------

  {
    // variables
      
    fShowAllBarNumbers = boolOptionsInitialValue;
  
    // options
  
    S_msrOptionsSubGroup barsSubGroup =
      msrOptionsSubGroup::create (
      "hlilyb", "helpLilypondBars",
R"(Bars)"
      );
  
    appendOptionsSubGroup (barsSubGroup);

    barsSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "sabn", "showAllBarNumbers",
R"(Generate LilyPond code to show all bar numbers.)",
          "showAllBarNumbers",
          fShowAllBarNumbers));
  }   


  // line breaks
  // --------------------------------------

  {
    // variables
      
    fDontKeepLineBreaks                  = boolOptionsInitialValue;
  
    fBreakLinesAtIncompleteRightMeasures = boolOptionsInitialValue;
    
    fSeparatorLineEveryNMeasures         = -1;
    
    // options
  
    S_msrOptionsSubGroup lineBreaksSubGroup =
      msrOptionsSubGroup::create (
      "hlilylb", "helpLilypondLineBreaks",
R"(Line breaks)"
      );
  
    appendOptionsSubGroup (lineBreaksSubGroup);

    lineBreaksSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "dklb", "dontKeepLineBreaks",
R"(Don't keep the line breaks from the MusicXML input
and let LilyPond decide about them.)",
          "dontKeepLineBreaks",
          fDontKeepLineBreaks));

    lineBreaksSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "blairm", "breakLinesAtIncompleteRightMeasures",
R"(Generate a '\break' command at the end incomplete right measures
which is handy in popular folk dances and tuness.)",
          "breakLinesAtIncompleteRightMeasures",
          fBreakLinesAtIncompleteRightMeasures));

    lineBreaksSubGroup->
      appendOptionsItem (
        msrOptionsIntegerItem::create (
          "slenm", "separatorLineEveryNMeasures",
R"(Generate an additional separator line for readability every 'n' measures,
where 'n' is a positive integer.
Nothing special is done by default.)",
          "n",
          "separatorLineEveryNMeasures",
          fSeparatorLineEveryNMeasures));
  }
  

  // page breaks
  // --------------------------------------

  {
    // variables  
   
    fDontKeepPageBreaks                  = boolOptionsInitialValue;
  
    // options
  
    S_msrOptionsSubGroup pageBreaksSubGroup =
      msrOptionsSubGroup::create (
      "hlilypb", "helpLilypondPageBreaks",
R"(Page breaks)"
      );
  
    appendOptionsSubGroup (pageBreaksSubGroup);

    pageBreaksSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "dkpb", "dontKeepPageBreaks",
R"(Don't keep the page breaks from the MusicXML input
and let LilyPond decide about them.)",
          "dontKeepPageBreaks",
          fDontKeepPageBreaks));
  }
    

  // staves
  // --------------------------------------

  {
    // variables  
    
    fModernTab = boolOptionsInitialValue;
  
    // options
  
    S_msrOptionsSubGroup stavesSubGroup =
      msrOptionsSubGroup::create (
      "hlilys", "helpLilypondStaves",
R"(Staves)"
      );
  
    appendOptionsSubGroup (stavesSubGroup);

    stavesSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "mtab", "modernTab",
R"(Generate '\moderntab' instead of the default '\tab'.)",
          "modernTab",
          fModernTab));
  }
  

  // tuplets
  // --------------------------------------

  {
    // variables  
  
    fTupletsOnALine = boolOptionsInitialValue;
    
    // options
  
    S_msrOptionsSubGroup tupletsSubGroup =
      msrOptionsSubGroup::create (
      "hlilytu", "helpLilypondTuplets",
R"(Tuplets)"
      );
  
    appendOptionsSubGroup (tupletsSubGroup);

    tupletsSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "toal", "tupletsOnALine",
R"(Keep tuplets notes on the same line, instead of
'having \tuplet {' and '}' on separate lines.)",
          "tupletsOnALine",
          fTupletsOnALine));
  }
      

  // repeats
  // --------------------------------------
  
  {
    // variables
    
    fRepeatBrackets = boolOptionsInitialValue;
  
    // options
  
    S_msrOptionsSubGroup repeatsSubGroup =
      msrOptionsSubGroup::create (
      "hlilyr", "helpLilypondRepeats",
R"(Repeats)"
      );
  
    appendOptionsSubGroup (repeatsSubGroup);

    repeatsSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "rbracks", "repeatBrackets",
R"(Generate repeats with brackets instead of regular bar lines)",
          "repeatBrackets",
          fRepeatBrackets));
  }
      
  // ornaments
  // --------------------------------------
  
  {
    // variables
    
    fDelayedOrnamentsFraction = rational (2, 3);
  
    // options
  
    S_msrOptionsSubGroup ornamentsSubGroup =
      msrOptionsSubGroup::create (
      "hlilyo", "helpLilypondOrnaments",
R"(Ornaments)"
      );
  
    appendOptionsSubGroup (ornamentsSubGroup);

    ornamentsSubGroup->
      appendOptionsItem (
        msrOptionsRationalItem::create (
          "dof", "delayedOrnamentsFraction",
R"(Place the delayed turn/reverseturn at the given fraction
between the ornemented note and the next one.
The default value is '2/3'.)",
          "num/denom",
          "delayedOrnamentsFraction",
          fDelayedOrnamentsFraction));
  }

      
  // fonts
  // --------------------------------------

  {
    // variables
    
    fJazzFonts   = boolOptionsInitialValue;
  
    // options
  
    S_msrOptionsSubGroup fontsSubGroup =
      msrOptionsSubGroup::create (
      "hfonts", "helpFonts",
R"(Fonts)"
      );
  
    appendOptionsSubGroup (fontsSubGroup);

    fontsSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "jazz", "jazzFonts",
R"(Use the LilyJazz font for notes, braces, text and chords.
This font should be installed so that LilyPond can use it.)",
          "jazzFonts",
          fJazzFonts));
  }


  // code generation
  // --------------------------------------
  
  {
    // variables  
  
    fComments            = boolOptionsInitialValue;
  
    fGlobal              = boolOptionsInitialValue;
    
    fDisplayMusic        = boolOptionsInitialValue;
      
    fNoLilypondCode      = boolOptionsInitialValue;
    
    fNoLilypondLyrics    = boolOptionsInitialValue;
    
    fLilypondCompileDate = boolOptionsInitialValue;
  
    // options
  
    S_msrOptionsSubGroup codeGenerationSubGroup =
      msrOptionsSubGroup::create (
      "hlilycg", "helpLilypondCodeGeneration",
  R"(Code generation)"
      );
  
    appendOptionsSubGroup (codeGenerationSubGroup);

    codeGenerationSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "com", "comments",
R"(Generate comments showing the structure of the score
such as '% part P_POne (P1).)",
          "comments",
          fComments));

    codeGenerationSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "global", "",
R"(Generate a 'global' empty variable and place a use of it
at the beginning of all voices.)",
          "global",
          fGlobal));

    codeGenerationSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "dm", "displayMusic",
R"(Place the contents of all voices inside a '\displayMusic' block,
for LilyPond to show its internal representation of the music.)",
          "displayMusic",
          fDisplayMusic));

    codeGenerationSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "nolpc", "noLilypondCode",
R"(Don't generate any LilyPond code.
That can be useful if only a summary of the score is needed.)",
          "noLilypondCode",
          fNoLilypondCode));

    codeGenerationSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "nolpl", "noLilypondLyrics",
R"(Don't generate any lyrics in the LilyPond code.)",
          "noLilypondLyrics",
          fNoLilypondLyrics));

    codeGenerationSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "lpcd", "lilypondCompileDate",
R"(Generate code to include the compilation date
when LilyPond creates the score.)",
          "lilypondCompileDate",
          fLilypondCompileDate));
  } 
      
    
  // score notation
  // --------------------------------------

  {
    // variables  
  
  //  fScoreNotationKind =  lilypondOptions::kWesternNotation;
      // default value

    fJianpu = false;
    
    // options
  
    S_msrOptionsSubGroup scoreNotationSubGroup =
      msrOptionsSubGroup::create (
      "hlilysn", "helpLilypondScoreNotation",
R"(Score notation)"
      );
  
    appendOptionsSubGroup (scoreNotationSubGroup);

    scoreNotationSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "jianpu", "",
R"(Generate the score using jianpu (numbered) notation
instead of the default western notation.
That option needs lilypond-Jianpu to be accessible to LilyPond
(https://github.com/nybbs2003/lilypond-Jianpu/blob/master/jianpu10a.ly).)",
          "jianpu",
          fJianpu));

  } 
      

  // midi
  // --------------------------------------
  
  {
    // variables
    string midiTempoDuration  = "4";
    int    midiTempoPerSecond = 90;
    
    fMidiTempo.first  = midiTempoDuration;
    fMidiTempo.second = midiTempoPerSecond;
    
    fNoMidi = boolOptionsInitialValue;
    
    // options
  
    S_msrOptionsSubGroup midiSubGroup =
      msrOptionsSubGroup::create (
      "hlilym", "helpLilypondMidi",
R"(Midi)"
      );
  
    appendOptionsSubGroup (midiSubGroup);

    midiSubGroup->
      appendOptionsItem (
        msrOptionsMidiTempoItem::create (
          "midiTempo", "",
R"(Generate a '\tempo' command in the \midi block.
'midiTempoSpecification' can be:
  'duration = perSecond'
or
  "duration = perSecond" .
'duration' is a string such as '8.', and 'perSecond' is an integer.
The single or double quotes are used to allow spaces around the '=' sign,
otherwise they can be dispensed with.
Using double quotes allows for shell variables substitutions, as in:
  PER_SECOND=66
  xml2lilypond -midiTempo "8. ${PER_SECOND}" .
The default midi tempo is '4 = 90'.)",
          "midiTempoSpecification",
          "midiTempo",
          fMidiTempo));

    midiSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "noMidi", "",
R"(Generate the '\midi' block as a comment instead of active code.)",
          "noMidi",
          fNoMidi));
  } 

  
  // JMI ???

  fKeepStaffSize = boolOptionsInitialValue;

  fSilentVoices = boolOptionsInitialValue;


  // register the LilyPond accidental styles
  // --------------------------------------
  
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

  clone->fAllDurations =
    fAllDurations;

  clone->fStems =
    fStems;

  clone->fNoAutoBeaming =
    fNoAutoBeaming;

  clone->fRomanStringNumbers =
    fRomanStringNumbers;

  clone->fAvoidOpenStrings =
    fAvoidOpenStrings;

  clone->fAccidentalStyle =
    fAccidentalStyle;

  clone->fCompressMultiMeasureRests =
    fCompressMultiMeasureRests;

  clone->fNoteInputLineNumbers =
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

  // page breaks

  clone->fDontKeepPageBreaks =
    fDontKeepPageBreaks;

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
  
  clone->fDelayedOrnamentsFraction =
    fDelayedOrnamentsFraction;
  
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
    
  // score notation

  clone->fJianpu =
    fJianpu;
        
  // midi
  
  clone->fMidiTempo =
    fMidiTempo;
    
  clone->fNoMidi =
    fNoMidi;
    
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
      "--" _ALL_DURATIONS_SHORT_NAME_ ", --" _ALL_DURATIONS_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Generate all LilyPond durations. " << 
      endl <<
    idtr << tab << tab << tab <<
      "By default, a duration equal to last one met in the current voice" <<
      endl <<
    idtr << tab << tab << tab <<
      "is omitted for code conciseness." << 
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

  // page breaks
  // --------------------------------------
  cerr <<
    idtr << "Page breaks:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _DONT_KEEP_PAGE_BREAKS_SHORT_NAME_ ", --" _DONT_KEEP_PAGE_BREAKS_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Don't keep the page breaks from the MusicXML input" << 
      endl <<
    idtr << tab << tab << tab <<
      "and let LilyPond decide about them." << 
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

  // fonts
  // --------------------------------------
  
  cerr <<
    idtr << "Fonts:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _JAZZ_FONTS_SHORT_NAME_ ", --" _JAZZ_FONTS_LONG_NAME_ << 
      endl <<
    idtr << tab << tab << tab <<
      "Use the LilyJazz font for notes, braces, text and chords." << 
      endl <<
    idtr << tab << tab << tab <<
      "This font should be installed so that LilyPond can use it." << 
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
      "--" _LILYPOND_COMPILE_DATE_SHORT_NAME_ << _LILYPOND_COMPILE_DATE_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Generate code to include the date when LilyPond creates the score in the latter." << 
      endl <<
    endl;
    
  idtr--;

  // score notation
  // --------------------------------------

  cerr <<
    idtr << "Score notation:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _JIANPU_LONG_NAME_ << // _JIANPU_SHORT_NAME_ is empty
      endl <<
    idtr << tab << tab << tab <<
      "Generate the score using jianpu (numbered) notation instead of the default western notation." <<
      endl <<
    idtr << tab << tab << tab <<
      "This option needs lilypond-Jianpu to be accessible to LilyPond" <<
      endl <<
    idtr << tab << tab << tab <<
      "(https://github.com/nybbs2003/lilypond-Jianpu/blob/master/jianpu10a.ly)." <<
      endl <<
/* JMI
    idtr << tab << tab << tab <<
      "This option and '--" _ABC_LONG_NAME_ "' are mutually exclusive." <<
      endl <<
    endl <<
    
    idtr <<
      "--" _ABC_LONG_NAME_ << // _ABC_SHORT_NAME_ is empty
      endl <<
    idtr << tab << tab << tab <<
      "Generate the score using ABC (lettered) notation instead of the default western notation." <<
      endl <<
    idtr << tab << tab << tab <<
      "This option and '--" _JIANPU_LONG_NAME_ "' are mutually exclusive." <<
      endl <<
*/
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

string lilypondOptions::scoreNotationKindAsString (
  scoreNotationKind notationKind)
{
  string result;

  switch (notationKind) {
    case lilypondOptions::kWesternNotation:
      result = "western";
      break;
    case lilypondOptions::kJianpuNotation:
      result = "jianpu";
      break;
    case lilypondOptions::kABCNotation:
      result = "abc";
      break;
  } // switch

  return result;
}

void lilypondOptions::printLilypondOptionsValues (int fieldWidth)
{
  cerr << idtr <<
    "The LilyPond options are:" <<
    endl;

  idtr++;
  
  // time
  // --------------------------------------
  cerr <<
    idtr << "Time:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw (fieldWidth) << "numericalTime" << " : " <<
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
    idtr << setw (fieldWidth) << "absoluteOctaves" << " : " <<
      booleanAsString (fAbsoluteOctaves) <<
      endl <<
    
    idtr << setw (fieldWidth) << "allDurations" << " : " <<
      booleanAsString (fAllDurations) <<
      endl <<
    
    idtr << setw (fieldWidth) << "stems" << " : " <<
      booleanAsString (fStems) <<
      endl <<

    idtr << setw (fieldWidth) << "noAutoBeaming" << " : " <<
      booleanAsString (fNoAutoBeaming) <<
      endl <<

    idtr << setw (fieldWidth) << "romanStringNumbers" << " : " <<
      booleanAsString (fRomanStringNumbers) <<
      endl <<
      
    idtr << setw (fieldWidth) << "avoidOpenString" << " : " <<
      booleanAsString (fAvoidOpenStrings) <<
      endl <<

    idtr << setw (fieldWidth) << "accidentalStyle" << " : " <<
      fAccidentalStyle <<
      endl <<
    
    idtr << setw (fieldWidth) << "compressMultiMeasureRests" << " : " <<
      booleanAsString (fCompressMultiMeasureRests) <<
      endl <<

    idtr << setw (fieldWidth) << "inputLineNumbers" << " : " <<
      booleanAsString (fNoteInputLineNumbers) <<
      endl;

  idtr--;
  
  // bars
  // --------------------------------------
  cerr <<
    idtr << "Bars:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw (fieldWidth) << "showAllBarNumbers" << " : " <<
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
    idtr << setw (fieldWidth) << "dontKeepLineBreaks" << " : " <<
      booleanAsString (fDontKeepLineBreaks) <<
      endl <<

    idtr << setw (fieldWidth) << "breakLinesAtIncompleteRightMeasures" << " : " <<
      booleanAsString (fBreakLinesAtIncompleteRightMeasures) <<
      endl <<
    
    idtr << setw (fieldWidth) << "separatorLineEveryNMeasures" << " : " <<
      booleanAsString (fSeparatorLineEveryNMeasures) <<
      endl;

  idtr--;
  
  // page breaks
  // --------------------------------------
  cerr <<
    idtr << "Page breaks:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw (fieldWidth) << "dontKeepPageBreaks" << " : " <<
      booleanAsString (fDontKeepPageBreaks) <<
      endl;

  idtr--;
  
  // staves
  // --------------------------------------
  cerr <<
    idtr << "Staves:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw (fieldWidth) << "modernTab" << " : " <<
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
    idtr << setw (fieldWidth) << "tupletsOnALine" << " : " <<
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
    idtr << setw (fieldWidth) << "repeatBrackets" << " : " <<
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
    idtr << setw (fieldWidth) << "fDelayedOrnamentsFraction" << " : " <<
      fDelayedOrnamentsFraction <<
      endl;

  idtr--;

  // fonts
  // --------------------------------------

  cerr <<
    idtr << "Fonts:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw (fieldWidth) << "jazzFonts" << " : " <<
      booleanAsString (fJazzFonts) <<
      endl;

  idtr--;

  // code generation
  // --------------------------------------

  cerr <<
    idtr << "LilyPond code generation:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw (fieldWidth) << "comments" << " : " <<
      booleanAsString (fComments) <<
      endl <<

    idtr << setw (fieldWidth) << "global" << " : " <<
      booleanAsString (fGlobal) <<
      endl <<
    
    idtr << setw (fieldWidth) << "noLilypondCode" << " : " <<
      booleanAsString (fNoLilypondCode) <<
      endl <<

    idtr << setw (fieldWidth) << "noLilypondLyrics" << " : " <<
      booleanAsString (fNoLilypondLyrics) <<
      endl <<

    idtr << setw (fieldWidth) << "lilypondCompileDate" << " : " <<
      booleanAsString (fLilypondCompileDate) <<
      endl;

  idtr--;

  // score notation
  // --------------------------------------
    
  cerr <<
    idtr << "Score notation:" <<
    endl;

  idtr++;

/*
  cerr <<
    idtr << setw (fieldWidth) << "scoreNotationKind" << " : " <<
      scoreNotationKindAsString (
        fScoreNotationKind) <<
      endl;
*/
  idtr--;
  
  // midi
  // --------------------------------------
  cerr <<
    idtr << "Midi:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw (fieldWidth) << "midiTempoDuration" << " : " <<
 //     fMidiTempoDuration <<
      endl <<

    idtr << setw (fieldWidth) << "midiTempoPerSecond" << " : " <<
  //    fMidiTempoPerSecond <<
      endl <<

    idtr << setw (fieldWidth) << "noMidiCommand" << " : " <<
      booleanAsString (fNoMidi) <<
      endl;

  idtr--;
  

  idtr--;
}

ostream& operator<< (ostream& os, const S_lilypondOptions& elt)
{
  elt->print (os);
  return os;
}


}
