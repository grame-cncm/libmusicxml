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
#include <iomanip>      // setw, setprecision, ...

#include "utilities.h"

#include "lilypondOptions.h"


using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
S_lilypondOptions gLilypondOptions;
S_lilypondOptions gLilypondOptionsUserChoices;
S_lilypondOptions gLilypondOptionsWithDetailedTrace;

S_lilypondOptions lilypondOptions::create (
  S_optionsHandler optionsHandler)
{
  lilypondOptions* o = new lilypondOptions (
    optionsHandler);
  assert(o!=0);
  return o;
}

lilypondOptions::lilypondOptions (
  S_optionsHandler optionsHandler)
  : optionsGroup (
    "LilyPond",
    "hlp", "helpLilypond",
R"(These options control which LilyPond code is generated.)",
    optionsHandler)
{
  // append this options group to the options handler
  // if relevant
  if (optionsHandler) {
    optionsHandler->
      appendOptionsGroup (this);
  }

  // initialize it
  initializeLilypondOptions (false);
}

lilypondOptions::~lilypondOptions ()
{}

void lilypondOptions::initializeLilypondOptions (
  bool boolOptionsInitialValue)
{
  // time
  // --------------------------------------

  {
    // variables
    
    fNumericalTime = boolOptionsInitialValue;
  
    S_optionsSubGroup
      timeSubGroup =
        optionsSubGroup::create (
          "Time",
          "hlilyti", "helpLilypondtime",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (timeSubGroup);

    timeSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
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
    
    fAccidentalStyleKind = kDefaultStyle;
  
    fCompressMultiMeasureRests = boolOptionsInitialValue;
  
    fNoteInputLineNumbers = boolOptionsInitialValue;
  
    S_optionsSubGroup
      notesSubGroup =
        optionsSubGroup::create (
          "Notes",
          "hlilyn", "helpLilypondNotes",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (notesSubGroup);

    notesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "abs", "absolute",
R"(Generate LilyPond absolute note octaves. 
By default, relative octaves are generated.)",
          "absoluteOctaves",
          fAbsoluteOctaves));

    notesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "alldurs", "allDurations",
R"(Generate all LilyPond durations. 
By default, a duration equal to preceding one met in the current voice
is omitted for code conciseness.)",
          "allDurations",
          fAllDurations));

    notesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "stems", "",
R"(Generate \stemUp and \stemDown LilyPond commands.
By default, LilyPond will take care of that by itself.)",
          "stems",
          fStems));

    notesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "noab", "noAutoBeaming",
R"(Generate '\set Voice.autoBeaming = ##f' in each voice 
to prevent LilyPond from handling beams automatically.)",
          "noAutoBeaming",
          fNoAutoBeaming));

    notesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "rsn", "romanStringNumbers",
R"(Generate '\romanStringNumbers' in each voice 
for LilyPond to generate roman instead of arabic string numbers.)",
          "romanStringNumbers",
          fRomanStringNumbers));

    notesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "aos", "avoidOpenStrings",
R"(Generate '\set TabStaff.restrainOpenStrings = ##t' in each voice 
to prevent LilyPond from using open strings.)",
          "avoidOpenStrings",
          fAvoidOpenStrings));

    notesSubGroup->
      appendOptionsItem (
        optionsAccidentalStyleItem::create (
          "as", "accidentalStyle", // JMI
R"(Choose the LilyPond accidental 'style' among: 
  voice, modern, modern-cautionary, modern-voice, 
  modern-voice-cautionary, piano, piano-cautionary, 
  neo-modern, neo-modern-cautionary, neo-modern-voice,
  neo-modern-voice-cautionary, dodecaphonic, dodecaphonic-no-repeat,
  dodecaphonic-first, teaching, no-reset, forget.
The default is... 'default'.)",
          "style",
          "accidentalStyle",
          fAccidentalStyleKind));

/* JMI
    notesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "cmmr", "compressMultiMeasureRests",
R"(Generate '\compressMMRests' at the beginning of voices.
By default, this command is commented.)",
          "compressMultiMeasureRests",
          fCompressMultiMeasureRests));
*/

    notesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
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
  
    S_optionsSubGroup
      barsSubGroup =
        optionsSubGroup::create (
          "Bars",
          "hlilyb", "helpLilypondBars",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (barsSubGroup);

    barsSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "sabn", "showAllBarNumbers",
R"(Generate LilyPond code to show all bar numbers.)",
          "showAllBarNumbers",
          fShowAllBarNumbers));
  }   


  // line breaks
  // --------------------------------------

  {
    // variables
      
    fIgnoreLineBreaks                    = boolOptionsInitialValue;
  
    fBreakLinesAtIncompleteRightMeasures = boolOptionsInitialValue;
    
    fSeparatorLineEveryNMeasures         = -1;
    
    // options
  
    S_optionsSubGroup
      lineBreaksSubGroup =
        optionsSubGroup::create (
          "Line breaks",
          "hlilylb", "helpLilypondLineBreaks",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (lineBreaksSubGroup);

    lineBreaksSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "ilb", "ignoreLineBreaks",
R"(Ignore the line breaks from the MusicXML input
and let LilyPond decide about them.)",
          "ignoreLineBreaks",
          fIgnoreLineBreaks));

    lineBreaksSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "blairm", "breakLinesAtIncompleteRightMeasures",
R"(Generate a '\break' command at the end of incomplete right measures
which is handy in popular folk dances and tunes.)",
          "breakLinesAtIncompleteRightMeasures",
          fBreakLinesAtIncompleteRightMeasures));

    lineBreaksSubGroup->
      appendOptionsItem (
        optionsIntegerItem::create (
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
   
    fIgnorePageBreaks = boolOptionsInitialValue;
  
    // options
  
    S_optionsSubGroup
      pageBreaksSubGroup =
        optionsSubGroup::create (
          "Page breaks",
          "hlilypb", "helpLilypondPageBreaks",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (pageBreaksSubGroup);

    pageBreaksSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "ipb", "ignorePageBreaks",
R"(Ignore the page breaks from the MusicXML input
and let LilyPond decide about them.)",
          "ignorePageBreaks",
          fIgnorePageBreaks));
  }
    

  // staves
  // --------------------------------------

  {
    // variables  
    
    fModernTab = boolOptionsInitialValue;
  
    // options
  
    S_optionsSubGroup
      stavesSubGroup =
        optionsSubGroup::create (
          "Staves",
          "hlilys", "helpLilypondStaves",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (stavesSubGroup);

    stavesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "mtab", "modernTab",
R"(Generate '\moderntab' instead of the default '\tab'.)",
          "modernTab",
          fModernTab));
  }
  

  // tuplets
  // --------------------------------------

  {
    // variables  
  
    fIndentTuplets = boolOptionsInitialValue;
    
    // options
  
    S_optionsSubGroup
      tupletsSubGroup =
        optionsSubGroup::create (
          "Tuplets",
          "hlilytu", "helpLilypondTuplets",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (tupletsSubGroup);

    tupletsSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "itups", "indentTuplets",
R"(Indent tuplets notes on a line of their own,
instead of keeping the on the same line as the current measure.)",
          "indentTuplets",
          fIndentTuplets));
  }
      

  // repeats
  // --------------------------------------
  
  {
    // variables
    
    fRepeatBrackets = boolOptionsInitialValue;
  
    // options
  
    S_optionsSubGroup
      repeatsSubGroup =
        optionsSubGroup::create (
          "Repeats",
          "hlilyr", "helpLilypondRepeats",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (repeatsSubGroup);

    repeatsSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
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
  
    S_optionsSubGroup
      ornamentsSubGroup =
        optionsSubGroup::create (
          "Ornaments",
          "hlilyo", "helpLilypondOrnaments",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (ornamentsSubGroup);

    ornamentsSubGroup->
      appendOptionsItem (
        optionsRationalItem::create (
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
  
    S_optionsSubGroup
      fontsSubGroup =
        optionsSubGroup::create (
          "Fonts",
          "hfonts", "helpFonts",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (fontsSubGroup);

    fontsSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "jazz", "jazzFonts",
R"(Use the LilyJazz font for notes, braces, text and chords.
It can be downloaded from https://github.com/OpenLilyPondFonts/lilyjazz.
This font should be installed so that LilyPond can use it, see:
https://github.com/OpenLilyPondFonts/lilyjazz/blob/master/LilyPond-Fonts-Installation-And-Usage.txt.)",
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
  
    S_optionsSubGroup
      codeGenerationSubGroup =
        optionsSubGroup::create (
          "Code generation",
          "hlilycg", "helpLilypondCodeGeneration",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (codeGenerationSubGroup);

    codeGenerationSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "com", "comments",
R"(Generate comments showing the structure of the score
such as '% part P_POne (P1).)",
          "comments",
          fComments));

    codeGenerationSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "global", "",
R"(Generate a 'global' empty variable and place a use of it
at the beginning of all voices.)",
          "global",
          fGlobal));

    codeGenerationSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "dm", "displayMusic",
R"(Place the contents of all voices inside a '\displayMusic' block,
for LilyPond to show its internal representation of the music.)",
          "displayMusic",
          fDisplayMusic));

    codeGenerationSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "nolpc", "noLilypondCode",
R"(Don't generate any LilyPond code.
That can be useful if only a summary of the score is needed.)",
          "noLilypondCode",
          fNoLilypondCode));

    codeGenerationSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "nolpl", "noLilypondLyrics",
R"(Don't generate any lyrics in the LilyPond code.)",
          "noLilypondLyrics",
          fNoLilypondLyrics));

    codeGenerationSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
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
  
    S_optionsSubGroup
      scoreNotationSubGroup =
        optionsSubGroup::create (
          "Score notation",
          "hlilysn", "helpLilypondScoreNotation",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (scoreNotationSubGroup);

    scoreNotationSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
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
  
    S_optionsSubGroup
      midiSubGroup =
        optionsSubGroup::create (
          "Midi",
          "hlilym", "helpLilypondMidi",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (midiSubGroup);

    midiSubGroup->
      appendOptionsItem (
        optionsMidiTempoItem::create (
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
  xml2ly -midiTempo "8. ${PER_SECOND}" .
The default midi tempo is '4 = 90'.)",
          "midiTempoSpecification",
          "midiTempo",
          fMidiTempo));

    midiSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "noMidi", "",
R"(Generate the '\midi' block as a comment instead of active code.)",
          "noMidi",
          fNoMidi));
  } 

  
  // JMI ???

  fKeepStaffSize = boolOptionsInitialValue;


  /* JMI
  for (
    set<string>::const_iterator i=fLilypondAccidentalStyles.begin ();
    i!=fLilypondAccidentalStyles.end ();
    i++) {
      gLogIOstream << (*i) << endl;
  } // for
  */
}

S_lilypondOptions lilypondOptions::createCloneWithDetailedTrace ()
{
  S_lilypondOptions
    clone =
      lilypondOptions::create (0);
      // 0 not to have it inserted twice in teh option handler

  // set the options handler uplink
  clone->
    setOptionsHandlerUplink (
      fOptionsHandlerUplink);


  // time
  // --------------------------------------
  
  clone->fNumericalTime =
    fNumericalTime;


  // notes
  // --------------------------------------
  
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

  clone->fAccidentalStyleKind =
    fAccidentalStyleKind;

  clone->fCompressMultiMeasureRests =
    fCompressMultiMeasureRests;

  clone->fNoteInputLineNumbers =
    true;


  // bars
  // --------------------------------------
  
  clone->fShowAllBarNumbers =
    true;


  // line breaks
  // --------------------------------------

  clone->fIgnoreLineBreaks =
    fIgnoreLineBreaks;

  clone->fBreakLinesAtIncompleteRightMeasures =
    fBreakLinesAtIncompleteRightMeasures;

  clone->fSeparatorLineEveryNMeasures =
    fSeparatorLineEveryNMeasures;


  // page breaks
  // --------------------------------------

  clone->fIgnorePageBreaks =
    fIgnorePageBreaks;


  // staves
  // --------------------------------------
  
  clone->fModernTab =
    fModernTab;

  
  // tuplets
  // --------------------------------------
    
  clone->fIndentTuplets =
    fIndentTuplets;

  
  // repeats
  // --------------------------------------
  
  clone->fRepeatBrackets =
    fRepeatBrackets;

  
  // ornaments
  // --------------------------------------
  
  clone->fDelayedOrnamentsFraction =
    fDelayedOrnamentsFraction;

  
  // code generation
  // --------------------------------------

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
  // --------------------------------------

  clone->fJianpu =
    fJianpu;

        
  // midi
  // --------------------------------------
  
  clone->fMidiTempo =
    fMidiTempo;
    
  clone->fNoMidi =
    fNoMidi;
    
// JMI ???

  clone->fKeepStaffSize =
    fKeepStaffSize;
    
  return clone;
}

/* JMI
bool lilypondOptions::setAccidentalStyle (lpsrAccidentalStyle accidentalStyle)
{
  // is accidentalStyle in the accidental styles set?
  map<string, lpsrAccidentalStyle>::const_iterator
    it =
      gLpsrAccidentalStylesMap.find (accidentalStyle);
        
  if (it == gLpsrAccidentalStylesMap.end ()) {
    // no, accidentalStyle is unknown
    return false;
  }

  fAccidentalStyle = it.second;
  
  return true;
} 
*/

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
  gLogIOstream <<
    "The LilyPond options are:" <<
    endl;

  gIndenter++;
  
  // time
  // --------------------------------------
  gLogIOstream <<
    "Time:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "numericalTime" << " : " <<
      booleanAsString (fNumericalTime) <<
      endl;

  gIndenter--;
  
  // notes
  // --------------------------------------
  gLogIOstream <<
    "Notes:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "absoluteOctaves" << " : " <<
      booleanAsString (fAbsoluteOctaves) <<
      endl <<
    
    setw (fieldWidth) << "allDurations" << " : " <<
      booleanAsString (fAllDurations) <<
      endl <<
    
    setw (fieldWidth) << "stems" << " : " <<
      booleanAsString (fStems) <<
      endl <<
    
    setw (fieldWidth) << "noAutoBeaming" << " : " <<
      booleanAsString (fNoAutoBeaming) <<
      endl <<
    
    setw (fieldWidth) << "romanStringNumbers" << " : " <<
      booleanAsString (fRomanStringNumbers) <<
      endl <<
      
    setw (fieldWidth) << "avoidOpenString" << " : " <<
      booleanAsString (fAvoidOpenStrings) <<
      endl <<
    
    setw (fieldWidth) << "accidentalStyle" << " : " <<
      fAccidentalStyleKind <<
      endl <<
    
    setw (fieldWidth) << "compressMultiMeasureRests" << " : " <<
      booleanAsString (fCompressMultiMeasureRests) <<
      endl <<
    
    setw (fieldWidth) << "inputLineNumbers" << " : " <<
      booleanAsString (fNoteInputLineNumbers) <<
      endl;

  gIndenter--;
  
  // bars
  // --------------------------------------
  gLogIOstream <<
    "Bars:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "showAllBarNumbers" << " : " <<
    booleanAsString (fShowAllBarNumbers) <<
    endl;

  gIndenter--;

  // line breaks
  // --------------------------------------
  gLogIOstream <<
    "Line breaks:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "ignoreLineBreaks" << " : " <<
      booleanAsString (fIgnoreLineBreaks) <<
      endl <<
    
    setw (fieldWidth) << "breakLinesAtIncompleteRightMeasures" << " : " <<
      booleanAsString (fBreakLinesAtIncompleteRightMeasures) <<
      endl <<
    
    setw (fieldWidth) << "separatorLineEveryNMeasures" << " : " <<
      booleanAsString (fSeparatorLineEveryNMeasures) <<
      endl;

  gIndenter--;
  
  // page breaks
  // --------------------------------------
  gLogIOstream <<
    "Page breaks:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "ignorePageBreaks" << " : " <<
    booleanAsString (fIgnorePageBreaks) <<
    endl;

  gIndenter--;
  
  // staves
  // --------------------------------------
  gLogIOstream <<
    "Staves:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "modernTab" << " : " <<
    booleanAsString (fModernTab) <<
    endl;

  gIndenter--;
  
  // tuplets
  // --------------------------------------

  gLogIOstream <<
    "Tuplets:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "indentTuplets" << " : " <<
    booleanAsString (fIndentTuplets) <<
    endl;
    
  gIndenter--;

  // repeats
  // --------------------------------------

  gLogIOstream <<
    "Repeats:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "repeatBrackets" << " : " <<
    booleanAsString (fRepeatBrackets) <<
    endl;
    
  gIndenter--;

  // ornaments
  // --------------------------------------

  gLogIOstream <<
    "Ornaments:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "fDelayedOrnamentsFraction" << " : " <<
    fDelayedOrnamentsFraction <<
    endl;

  gIndenter--;

  // fonts
  // --------------------------------------

  gLogIOstream <<
    "Fonts:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "jazzFonts" << " : " <<
    booleanAsString (fJazzFonts) <<
    endl;

  gIndenter--;

  // code generation
  // --------------------------------------

  gLogIOstream <<
    "LilyPond code generation:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "comments" << " : " <<
      booleanAsString (fComments) <<
      endl <<
    
    setw (fieldWidth) << "global" << " : " <<
      booleanAsString (fGlobal) <<
      endl <<
    
    setw (fieldWidth) << "noLilypondCode" << " : " <<
      booleanAsString (fNoLilypondCode) <<
      endl <<
    
    setw (fieldWidth) << "noLilypondLyrics" << " : " <<
      booleanAsString (fNoLilypondLyrics) <<
      endl <<
    
    setw (fieldWidth) << "lilypondCompileDate" << " : " <<
      booleanAsString (fLilypondCompileDate) <<
      endl;

  gIndenter--;

  // score notation
  // --------------------------------------
    
  gLogIOstream <<
    "Score notation:" <<
    endl;

  gIndenter++;

/*
  gLogIOstream << left <<
    setw (fieldWidth) << "scoreNotationKind" << " : " <<
      scoreNotationKindAsString (
        fScoreNotationKind) <<
      endl;
*/
  gIndenter--;
  
  // midi
  // --------------------------------------
  gLogIOstream <<
    "Midi:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "midiTempoDuration" << " : " <<
    //     fMidiTempoDuration <<
      endl <<
    
    setw (fieldWidth) << "midiTempoPerSecond" << " : " <<
    //    fMidiTempoPerSecond <<
      endl <<
    
    setw (fieldWidth) << "noMidiCommand" << " : " <<
      booleanAsString (fNoMidi) <<
      endl;

  gIndenter--;
  

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lilypondOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeLilypondOptionsHandling (
  S_optionsHandler optionsHandler)
{
  // create the options variables
  // ------------------------------------------------------
  
  gLilypondOptionsUserChoices = lilypondOptions::create (
    optionsHandler);
  assert(gLilypondOptionsUserChoices != 0);
  
  gLilypondOptions =
    gLilypondOptionsUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

  gLilypondOptionsWithDetailedTrace =
    gLilypondOptions->
      createCloneWithDetailedTrace ();
}


}
