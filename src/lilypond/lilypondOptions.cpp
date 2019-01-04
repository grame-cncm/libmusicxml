/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <climits>      // INT_MIN, INT_MAX
#include <iomanip>      // setw, setprecision, ...

#include <regex>

#include "utilities.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "lilypondOptions.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
S_optionsAccidentalStyleItem optionsAccidentalStyleItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsAccidentalStyleKindItemVariableDisplayName,
  lpsrAccidentalStyleKind&
                     optionsAccidentalStyleKindItemVariable)
{
  optionsAccidentalStyleItem* o = new
    optionsAccidentalStyleItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsAccidentalStyleKindItemVariableDisplayName,
      optionsAccidentalStyleKindItemVariable);
  assert(o!=0);
  return o;
}

optionsAccidentalStyleItem::optionsAccidentalStyleItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsAccidentalStyleKindItemVariableDisplayName,
  lpsrAccidentalStyleKind&
                     optionsAccidentalStyleKindItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsAccidentalStyleKindItemVariableDisplayName (
      optionsAccidentalStyleKindItemVariableDisplayName),
    fOptionsAccidentalStyleKindItemVariable (
      optionsAccidentalStyleKindItemVariable)
{}

optionsAccidentalStyleItem::~optionsAccidentalStyleItem ()
{}

void optionsAccidentalStyleItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsAccidentalStyleItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsAccidentalStyleKindItemVariableDisplayName" << " : " <<
    fOptionsAccidentalStyleKindItemVariableDisplayName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsAccidentalStyleKindItemVariable" << " : \"" <<
    lpsrAccidentalStyleKindAsString (
      fOptionsAccidentalStyleKindItemVariable) <<
      "\"" <<
    endl;
}

void optionsAccidentalStyleItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsAccidentalStyleKindItemVariableDisplayName <<
    " : \"" <<
    lpsrAccidentalStyleKindAsString (
      fOptionsAccidentalStyleKindItemVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsAccidentalStyleItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsMidiTempoItem optionsMidiTempoItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsMidiTempoItemVariableDisplayName,
  pair<string, int>&
                     optionsMidiTempoItemVariable)
{
  optionsMidiTempoItem* o = new
    optionsMidiTempoItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsMidiTempoItemVariableDisplayName,
      optionsMidiTempoItemVariable);
  assert(o!=0);
  return o;
}

optionsMidiTempoItem::optionsMidiTempoItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsMidiTempoItemVariableDisplayName,
  pair<string, int>&
                     optionsMidiTempoItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsMidiTempoItemVariableDisplayName (
      optionsMidiTempoItemVariableDisplayName),
    fOptionsMidiTempoItemVariable (
      optionsMidiTempoItemVariable)
{}

optionsMidiTempoItem::~optionsMidiTempoItem ()
{}

void optionsMidiTempoItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsMidiTempoItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsMidiTempoItemVariableDisplayName" << " : " <<
    fOptionsMidiTempoItemVariableDisplayName <<
    setw (fieldWidth) <<
    "fOptionsMidiTempoItemVariable" << " : '" <<
    fOptionsMidiTempoItemVariable.first <<
    " = " <<
    fOptionsMidiTempoItemVariable.second <<
    "'" <<
    endl;
}

void optionsMidiTempoItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsMidiTempoItemVariableDisplayName <<
    " : '" <<
    fOptionsMidiTempoItemVariable.first <<
    " = " <<
    fOptionsMidiTempoItemVariable.second <<
    "'" <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsMidiTempoItem& elt)
{
  elt->print (os);
  return os;
}

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
    "h=lp", "help=lilypond",
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
  // identification
  // --------------------------------------

  {
    // variables

    S_optionsSubGroup
      identificationSubGroup =
        optionsSubGroup::create (
          "Identification",
          "hlpi", "help-lilypond-identification",
R"(These options can be used to enforce values in the generated LilyPond code,
thus overriding the ones that may be present in the MSR data.
)",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (identificationSubGroup);

    // MusicXML informations

    identificationSubGroup->
      appendOptionsItem (
        optionsStringItem::create (
          "rights", "",
R"(Set the 'rights' in the LilyPond code.)",
          "STRING",
          "rights",
          fRights));

    identificationSubGroup->
      appendOptionsItem (
        optionsStringItem::create (
          "composer", "",
R"(Set the 'composer' in the LilyPond code.)",
          "STRING",
          "composer",
          fComposer));

    identificationSubGroup->
      appendOptionsItem (
        optionsStringItem::create (
          "arranger", "",
R"(Set the 'arranger' in the LilyPond code.)",
          "STRING",
          "arranger",
          fArranger));

    identificationSubGroup->
      appendOptionsItem (
        optionsStringItem::create (
          "poet", "",
R"(Set the 'poet' in the LilyPond code.)",
          "STRING",
          "poet",
          fPoet));

    identificationSubGroup->
      appendOptionsItem (
        optionsStringItem::create (
          "lyricist", "",
R"(Set the 'lyricist' in the LilyPond code.)",
          "STRING",
          "lyricist",
          fLyricist));

    identificationSubGroup->
      appendOptionsItem (
        optionsStringItem::create (
          "software", "",
R"(Set the 'software' in the LilyPond code.)",
          "STRING",
          "software",
          fSoftware));

    // LilyPond informations

    identificationSubGroup->
      appendOptionsItem (
        optionsStringItem::create (
          "dedication", "",
R"(Set 'dedication' to STRING in the \header.)",
          "STRING",
          "dedication",
          fDedication));

    identificationSubGroup->
      appendOptionsItem (
        optionsStringItem::create (
          "piece", "",
R"(Set 'piece' to STRING in the \header.)",
          "STRING",
          "piece",
          fPiece));

    identificationSubGroup->
      appendOptionsItem (
        optionsStringItem::create (
          "opus", "",
R"(Set 'opus' to STRING in the \header.)",
          "STRING",
          "opus",
          fOpus));

    identificationSubGroup->
      appendOptionsItem (
        optionsStringItem::create (
          "title", "",
R"(Set 'title' to STRING in the \header.)",
          "STRING",
          "title",
          fTitle));

    identificationSubGroup->
      appendOptionsItem (
        optionsStringItem::create (
          "stitle", "subtitle",
R"(Set 'subtitle' to STRING in the \header.)",
          "STRING",
          "subTitle",
          fSubTitle));

    identificationSubGroup->
      appendOptionsItem (
        optionsStringItem::create (
          "sstitle", "subsubtitle",
R"(Set 'subsubtitle' to STRING in the \header.)",
          "STRING",
          "subSubTitle",
          fSubSubTitle));

    identificationSubGroup->
      appendOptionsItem (
        optionsStringItem::create (
          "instrument", "",
R"(Set 'instrument' to STRING in the \header.)",
          "STRING",
          "instrument",
          fInstrument));

    identificationSubGroup->
      appendOptionsItem (
        optionsStringItem::create (
          "meter", "",
R"(Set 'meter' to STRING in the \header.)",
          "STRING",
          "meter",
          fMeter));

    identificationSubGroup->
      appendOptionsItem (
        optionsStringItem::create (
          "tagline", "",
R"(Set 'tagline' to STRING in the \header.)",
          "STRING",
          "tagline",
          fTagline));

    identificationSubGroup->
      appendOptionsItem (
        optionsStringItem::create (
          "copyright", "",
R"(Set 'copyright' to STRING in the \header.)",
          "STRING",
          "copyright",
          fCopyright));
  }
  

  // time
  // --------------------------------------

  {
    // variables
    
    fNumericalTime = boolOptionsInitialValue;
  
    S_optionsSubGroup
      timeSubGroup =
        optionsSubGroup::create (
          "Time",
          "hlpti", "help-lilypond-time",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (timeSubGroup);

    timeSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "numt", "numerical-time",
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
          "hlpn", "help-lilypond-notes",
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
          "alldurs", "all-durations",
R"(Generate all LilyPond durations. 
By default, a duration equal to preceding one found in the current voice
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
          "noab", "no-auto-beaming",
R"(Generate '\set Voice.autoBeaming = ##f' in each voice 
to prevent LilyPond from handling beams automatically.)",
          "noAutoBeaming",
          fNoAutoBeaming));

    notesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "rsn", "roman-string-numbers",
R"(Generate '\romanStringNumbers' in each voice 
for LilyPond to generate roman instead of arabic string numbers.)",
          "romanStringNumbers",
          fRomanStringNumbers));

    notesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "aos", "avoid-open-strings",
R"(Generate '\set TabStaff.restrainOpenStrings = ##t' in each voice 
to prevent LilyPond from using open strings.)",
          "avoidOpenStrings",
          fAvoidOpenStrings));

    notesSubGroup->
      appendOptionsItem (
        optionsAccidentalStyleItem::create (
          "as", "accidental-style", // JMI
R"(Choose the LilyPond accidental STYLE among: 
  voice, modern, modern-cautionary, modern-voice, 
  modern-voice-cautionary, piano, piano-cautionary, 
  neo-modern, neo-modern-cautionary, neo-modern-voice,
  neo-modern-voice-cautionary, dodecaphonic, dodecaphonic-no-repeat,
  dodecaphonic-first, teaching, no-reset, forget.
The default is... 'default'.)",
          "STYLE",
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
          "niln", "note-input-line-numbers",
R"(Generate after each note and barline a comment containing
its MusicXML input line number.
This is useful when debugging xml2ly.)",
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
          "hlpb", "help-lilypond-bars",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (barsSubGroup);

    barsSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "abn", "all-bar-numbers",
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
          "hlplb", "help-lilypond-line-breaks",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (lineBreaksSubGroup);

    lineBreaksSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "ilb", "ignore-line-breaks",
R"(Ignore the line breaks from the MusicXML input
and let LilyPond decide about them.)",
          "ignoreLineBreaks",
          fIgnoreLineBreaks));

    lineBreaksSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "blairm", "break-lines-at-incomplete-right-measures",
R"(Generate a '\break' command at the end of incomplete right measures
which is handy in popular folk dances and tunes.)",
          "breakLinesAtIncompleteRightMeasures",
          fBreakLinesAtIncompleteRightMeasures));

    lineBreaksSubGroup->
      appendOptionsItem (
        optionsIntegerItem::create (
          "slenm", "separator-line-every-n-measures",
R"(Generate an additional separator line for readability every N measures,
where N is a positive integer.
Nothing special is done by default.)",
          "N",
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
          "hlppb", "help-lilypond-page-breaks",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (pageBreaksSubGroup);

    pageBreaksSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "ipb", "ignore-page-breaks",
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

  /* JMI ???
    S_optionsSubGroup
      stavesSubGroup =
        optionsSubGroup::create (
          "Staves",
          "hlps", "helpLilypondStaves",
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
          */
  }
  

  // chords
  // --------------------------------------

  {
    // variables  
  
    fConnectArpeggios = boolOptionsInitialValue;
    
    // options
  
    S_optionsSubGroup
      chordsSubGroup =
        optionsSubGroup::create (
          "Chords",
          "hlpc", "help-lilypond-chordss",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (chordsSubGroup);

    chordsSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "carpeg", "connect-arpeggios",
R"(Connect arpeggios across piano staves.)",
          "connectArpeggios",
          fConnectArpeggios));
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
          "hlpt", "help-lilypond-tuplets",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (tupletsSubGroup);

    tupletsSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "itups", "indent-tuplets",
R"(Indent tuplets notes on a line of their own,
instead of keeping the on the same line as the current measure.)",
          "indentTuplets",
          fIndentTuplets));
  }
      

  // repeats
  // --------------------------------------
  
  {
    // variables
    
    fRepeatBrackets      = boolOptionsInitialValue;
    fIgnoreRepeatNumbers = boolOptionsInitialValue;
  
    // options
  
    S_optionsSubGroup
      repeatsSubGroup =
        optionsSubGroup::create (
          "Repeats",
          "hlpr", "help-lilypond-repeats",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (repeatsSubGroup);

    repeatsSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "irn", "ignore-repeat-numbers",
R"(Ignore repeats numbers and let LilyPond determine them.)",
          "ignoreRepeatNumbers",
          fIgnoreRepeatNumbers));

    repeatsSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "rbracks", "repeat-brackets",
R"(Generate repeats with brackets instead of regular bar lines.)",
          "repeatBrackets",
          fRepeatBrackets));
  }
      
  // ornaments
  // --------------------------------------
  
  {
    // variables
    
    fDelayedOrnamentsFraction = rational (1, 2);
  
    // options
  
    S_optionsSubGroup
      ornamentsSubGroup =
        optionsSubGroup::create (
          "Ornaments",
          "hlpo", "help-lilypond-ornaments",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (ornamentsSubGroup);

    ornamentsSubGroup->
      appendOptionsItem (
        optionsRationalItem::create (
          "dof", "delayed-ornaments-fraction",
R"(Place the delayed turn/reverseturn at the given fraction
between the ornemented note and the next one.
The default value is '1/2'.)",
          "NUM/DENOM",
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
          "hfonts", "help-fonts",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (fontsSubGroup);

    fontsSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "jazz", "jazz-fonts",
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
  
    fXml2lyInfos         = boolOptionsInitialValue;
    
    fComments            = boolOptionsInitialValue;
  
    fGlobal              = boolOptionsInitialValue;
    
    fDisplayMusic        = boolOptionsInitialValue;
      
    fNoLilypondCode      = boolOptionsInitialValue;
    
    fNoLilypondLyrics    = boolOptionsInitialValue;
    
    fLilypondCompileDate = boolOptionsInitialValue;
    
    fPointAndClickOff    = boolOptionsInitialValue;
    
    fWhiteNoteHeads      = boolOptionsInitialValue;
  
    // options
  
    S_optionsSubGroup
      codeGenerationSubGroup =
        optionsSubGroup::create (
          "Code generation",
          "hlpcg", "help-lilypond-code-generation",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (codeGenerationSubGroup);

    codeGenerationSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "xi", "xml2ly-infos",
R"(Generate initial comments showing the compilation date and options.)",
          "xml2lyInfos",
          fXml2lyInfos));

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
          "dm", "display-music",
R"(Place the contents of all voices inside a '\displayMusic' block,
for LilyPond to show its internal representation of the music.)",
          "displayMusic",
          fDisplayMusic));

    codeGenerationSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "nolpc", "no-lilypond-code",
R"(Don't generate any LilyPond code.
That can be useful if only a summary of the score is needed.)",
          "noLilypondCode",
          fNoLilypondCode));

    codeGenerationSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "nolpl", "no-lilypond-lyrics",
R"(Don't generate any lyrics in the LilyPond code.)",
          "noLilypondLyrics",
          fNoLilypondLyrics));

    codeGenerationSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "lpcd", "lilypond-compile-date",
R"(Generate code to include the compilation date
when LilyPond creates the score.)",
          "lilypondCompileDate",
          fLilypondCompileDate));

    codeGenerationSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "paco", "point-and-clic-off",
R"(Generate \pointAndClickOff at the beginning of the LilyPond code
to reduce the size of the resulting PDF file.)",
          "pointAndClickOff",
          fPointAndClickOff));

    codeGenerationSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "wnh", "white-note-heads",
R"(Generate Scheme function 'whiteNoteHeads'
at the beginning of the LilyPond code.)",
          "whiteNoteHeads",
          fWhiteNoteHeads));
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
          "hlpsn", "help-lilypond-score-notation",
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
(https://github.com/nybbs2003/lilypond-Jianpu/jianpu10a.ly).)",
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
          "hlpm", "help-lilypond-midi",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (midiSubGroup);

    midiSubGroup->
      appendOptionsItem (
        optionsMidiTempoItem::create (
          "midi-tempo", "",
R"(Generate a '\tempo' command in the \midi block.
MIDI_TEMPO_SPEC can be:
  'DURATION = PER_SECOND'
or
  "DURATION = PER_SECOND" .
DURATION is a string such as '8.', and PER_SECOND is an integer.
The single or double quotes are used to allow spaces around the '=' sign,
otherwise they can be dispensed with.
Using double quotes allows for shell variables substitutions, as in:
  PER_SECOND=66
  xml2ly -midiTempo "8. ${PER_SECOND}" .
The default midi tempo is '4 = 90'.)",
          "MIDI_TEMPO_SPEC",
          "midiTempo",
          fMidiTempo));

    midiSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "no-midi", "",
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
      // 0 not to have it inserted twice in the option handler

  // set the options handler uplink
  clone->
    setOptionsHandlerUplink (
      fOptionsHandlerUplink);


  // identification
  // --------------------------------------

  // MusicXML informations
  clone->fRights =
    fRights;
  clone->fComposer =
    fComposer;
  clone->fArranger =
    fArranger;
  clone->fPoet =
    fPoet;
  clone->fLyricist =
    fLyricist;
  clone->fSoftware =
    fSoftware;
    
  // LilyPond informations
  clone->fDedication =
    fDedication;
  clone->fPiece =
    fPiece;
  clone->fOpus =
    fOpus;
  clone->fTitle =
    fTitle;
  clone->fSubTitle =
    fSubTitle;
  clone->fSubSubTitle =
    fSubSubTitle;
  clone->fInstrument =
    fInstrument;
  clone->fMeter =
    fMeter;
  clone->fTagline =
    fTagline;
  clone->fCopyright =
    fCopyright;


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

  
  // chords
  // --------------------------------------
    
  clone->fConnectArpeggios =
    fConnectArpeggios;

  
  // tuplets
  // --------------------------------------
    
  clone->fIndentTuplets =
    fIndentTuplets;

  
  // repeats
  // --------------------------------------
  
  clone->fRepeatBrackets =
    fRepeatBrackets;
  clone->fIgnoreRepeatNumbers =
    fIgnoreRepeatNumbers;

  
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
    
  clone->fPointAndClickOff =
    fPointAndClickOff;

  clone->fWhiteNoteHeads =
    fWhiteNoteHeads;

    
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

//______________________________________________________________________________
void lilypondOptions::enforceQuietness ()
{}

//______________________________________________________________________________
void lilypondOptions::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
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
  
  // identification
  // --------------------------------------
  gLogIOstream <<
    "Time:" <<
    endl;

  gIndenter++;

  // MusicXML informations
  gLogIOstream << left <<
    setw (fieldWidth) << "rights" << " : " <<
      fRights <<
      endl <<

    setw (fieldWidth) << "composer" << " : " <<
      fComposer <<
      endl <<
    setw (fieldWidth) << "arranger" << " : " <<
      fArranger <<
      endl <<
    setw (fieldWidth) << "poet" << " : " <<
      fPoet <<
      endl <<
    setw (fieldWidth) << "lyricist" << " : " <<
      fLyricist <<
      endl <<

    setw (fieldWidth) << "software" << " : " <<
      fSoftware <<
      endl <<

  // LilyPond informations

    setw (fieldWidth) << "dedication" << " : " <<
      fDedication <<
      endl <<

    setw (fieldWidth) << "piece" << " : " <<
      fPiece <<
      endl <<
    setw (fieldWidth) << "opus" << " : " <<
      fOpus <<
      endl <<

    setw (fieldWidth) << "title" << " : " <<
      fTitle <<
      endl <<
    setw (fieldWidth) << "subTitle" << " : " <<
      fSubTitle <<
      endl <<
    setw (fieldWidth) << "subSubTitle" << " : " <<
      fSubSubTitle <<
      endl <<
    setw (fieldWidth) << "instrument" << " : " <<
      fInstrument <<
      endl <<
    setw (fieldWidth) << "meter" << " : " <<
      fMeter <<
      endl <<

    setw (fieldWidth) << "tagline" << " : " <<
      fTagline <<
      endl <<

    setw (fieldWidth) << "copyright" << " : " <<
      fCopyright <<
      endl;

  gIndenter--;


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

  
  // chords
  // --------------------------------------

  gLogIOstream <<
    "Chords:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "connectArpeggios" << " : " <<
    booleanAsString (fConnectArpeggios) <<
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
    setw (fieldWidth) << "ignoreRepeatNumbers" << " : " <<
    booleanAsString (fIgnoreRepeatNumbers) <<
    endl <<
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
    
    setw (fieldWidth) << "displayMusic" << " : " <<
      booleanAsString (fDisplayMusic) <<
      endl <<
    
    setw (fieldWidth) << "noLilypondCode" << " : " <<
      booleanAsString (fNoLilypondCode) <<
      endl <<
    
    setw (fieldWidth) << "noLilypondLyrics" << " : " <<
      booleanAsString (fNoLilypondLyrics) <<
      endl <<
    
    setw (fieldWidth) << "lilypondCompileDate" << " : " <<
      booleanAsString (fLilypondCompileDate) <<
      endl <<
    
    setw (fieldWidth) << "pointAndClickOff" << " : " <<
      booleanAsString (fPointAndClickOff) <<
      endl <<
    
    setw (fieldWidth) << "whiteNoteHeads" << " : " <<
      booleanAsString (fWhiteNoteHeads) <<
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

S_optionsItem lilypondOptions::handleOptionsItem (
  ostream&      os,
  S_optionsItem item)
{
  S_optionsItem result;
  
  if (
    // acccidentals style item?
    S_optionsAccidentalStyleItem
      accidentalStyleItem =
        dynamic_cast<optionsAccidentalStyleItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsAccidentalStyleItem'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = accidentalStyleItem;
  }

  else if (
    // midi tempo item?
    S_optionsMidiTempoItem
      midiTempoItem =
        dynamic_cast<optionsMidiTempoItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsMidiTempoItem'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = midiTempoItem;
  }

  return result;
}

void lilypondOptions::handleOptionsItemValue (
  ostream&      os,
  S_optionsItem item,
  string        theString)
{
  if (
    // accidental style item?
    S_optionsAccidentalStyleItem
      accidentalStyleKindItem =
        dynamic_cast<optionsAccidentalStyleItem*>(&(*item))
    ) {
    // theString contains the language name:     
    // is it in the accidental styles map?
    
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsAccidentalStyleItem'" <<
        endl;
    }
#endif

    map<string, lpsrAccidentalStyleKind>::const_iterator
      it =
        gLpsrAccidentalStyleKindsMap.find (
          theString);
          
    if (it == gLpsrAccidentalStyleKindsMap.end ()) {
      // no, accidental style is unknown in the map
      stringstream s;
  
      s <<
        "LPSR accidental style " << theString <<
        " is unknown" <<
        endl <<
        "The " <<
        gLpsrAccidentalStyleKindsMap.size () - 1 <<
        " known LPSR accidental styles are:" <<
        endl;
  
      gIndenter++;
    
      s <<
        existingLpsrAccidentalStyleKinds ();
  
      gIndenter--;
  
      optionError (s.str ());
      
      printHelpSummary (os);
      
      exit (4);
    }
  
    accidentalStyleKindItem->
      setAccidentalStyleKindItemVariableValue (
        (*it).second);
  }

  else if (
    // midi tempo item?
    S_optionsMidiTempoItem
      midiTempoItem =
        dynamic_cast<optionsMidiTempoItem*>(&(*item))
    ) {
    // theString contains the midi tempo specification
    // decipher it to extract duration and perSecond values

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsMidiTempoItem'" <<
        endl;
    }
#endif

    string regularExpression (
      "[[:space:]]*([[:digit:]]+\\.*)[[:space:]]*"
      "="
      "[[:space:]]*([[:digit:]]+)[[:space:]]*");
      
    regex  e (regularExpression);
    smatch sm;

    regex_match (theString, sm, e);

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "There are " << sm.size () << " matches" <<
        " for MIDI tempo string '" << theString <<
        "' with regex '" << regularExpression <<
        "'" <<
        endl;
    }
#endif
  
    if (sm.size ()) {
      for (unsigned i = 0; i < sm.size (); ++i) {
        os <<
          "[" << sm [i] << "] ";
      } // for
      os <<
        endl;
    }
    
    else {
      stringstream s;

      s <<
        "-midiTempo argument '" << theString <<
        "' is ill-formed";
        
      optionError (s.str ());
      
      printSpecificSubGroupHelp (
        os,
        midiTempoItem->
          getOptionsSubGroupUplink ());
          
      exit (4);
    }

    string midiTempoDuration  = sm [1];

    int    midiTempoPerSecond;
    {
      stringstream s;
      s << sm [2];
      s >> midiTempoPerSecond;
    }
    
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "midiTempoDuration  = " <<
        midiTempoDuration <<
        endl <<
        "midiTempoPerSecond = " <<
        midiTempoPerSecond <<
        endl;

    midiTempoItem->
      setMidiTempoItemVariableValue (
        pair<string, int> (
          midiTempoDuration, midiTempoPerSecond));
    }
#endif
  }
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
