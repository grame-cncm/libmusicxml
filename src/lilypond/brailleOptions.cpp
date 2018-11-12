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

#include "brailleOptions.h"


using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
S_brailleOptions gBrailleOptions;
S_brailleOptions gBrailleOptionsUserChoices;
S_brailleOptions gBrailleOptionsWithDetailedTrace;

S_brailleOptions brailleOptions::create (
  S_optionsHandler optionsHandler)
{
  brailleOptions* o = new brailleOptions (
    optionsHandler);
  assert(o!=0);
  return o;
}

brailleOptions::brailleOptions (
  S_optionsHandler optionsHandler)
  : optionsGroup (
    "Braille",
    "h=lp", "help=braille",
R"(These options control which Braille code is generated.)",
    optionsHandler)
{
  // append this options group to the options handler
  // if relevant
  if (optionsHandler) {
    optionsHandler->
      appendOptionsGroup (this);
  }

  // initialize it
  initializeBrailleOptions (false);
}

brailleOptions::~brailleOptions ()
{}

void brailleOptions::initializeBrailleOptions (
  bool boolOptionsInitialValue)
{
  /*
  // identification
  // --------------------------------------

  {
    // variables

    S_optionsSubGroup
      identificationSubGroup =
        optionsSubGroup::create (
          "Identification",
          "hlpi", "help-braille-identification",
R"(These options can be used to enforce values in the generated Braille code,
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
R"(Set the 'rights' to STRING in the Braille code.)",
          "STRING",
          "rights",
          fRights));

    identificationSubGroup->
      appendOptionsItem (
        optionsStringItem::create (
          "composer", "",
R"(Set the 'composer' to STRING in the Braille code.)",
          "STRING",
          "composer",
          fComposer));

    identificationSubGroup->
      appendOptionsItem (
        optionsStringItem::create (
          "arranger", "",
R"(Set the 'arranger' to STRING in the Braille code.)",
          "STRING",
          "arranger",
          fArranger));

    identificationSubGroup->
      appendOptionsItem (
        optionsStringItem::create (
          "poet", "",
R"(Set the 'poet' to STRING in the Braille code.)",
          "STRING",
          "poet",
          fPoet));

    identificationSubGroup->
      appendOptionsItem (
        optionsStringItem::create (
          "lyricist", "",
R"(Set the 'lyricist' to STRING in the Braille code.)",
          "STRING",
          "lyricist",
          fLyricist));

    identificationSubGroup->
      appendOptionsItem (
        optionsStringItem::create (
          "software", "",
R"(Set the 'software' to STRING in the Braille code.)",
          "STRING",
          "software",
          fSoftware));

    // Braille informations

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
  

  // notes
  // --------------------------------------

  {
    // variables
      
    fAbsoluteOctaves  = boolOptionsInitialValue;
    
    fAllDurations  = boolOptionsInitialValue;
      
    fRomanStringNumbers = boolOptionsInitialValue;
    fAvoidOpenStrings    = boolOptionsInitialValue;
      
    fCompressMultiMeasureRests = boolOptionsInitialValue;
  
    fNoteInputLineNumbers = boolOptionsInitialValue;
  
    S_optionsSubGroup
      notesSubGroup =
        optionsSubGroup::create (
          "Notes",
          "hlpn", "help-braille-notes",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (notesSubGroup);

    notesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "abs", "absolute",
R"(Generate Braille absolute note octaves. 
By default, relative octaves are generated.)",
          "absoluteOctaves",
          fAbsoluteOctaves));

    notesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "alldurs", "all-durations",
R"(Generate all Braille durations. 
By default, a duration equal to preceding one found in the current voice
is omitted for code conciseness.)",
          "allDurations",
          fAllDurations));

    notesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "rsn", "roman-string-numbers",
R"(Generate '\romanStringNumbers' in each voice 
for Braille to generate roman instead of arabic string numbers.)",
          "romanStringNumbers",
          fRomanStringNumbers));

    notesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "aos", "avoid-open-strings",
R"(Generate '\set TabStaff.restrainOpenStrings = ##t' in each voice 
to prevent Braille from using open strings.)",
          "avoidOpenStrings",
          fAvoidOpenStrings));

    notesSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "niln", "note-input-line-numbers",
R"(Generate after each note and barline a comment containing
its MusicXML input line number.
This is useful when debugging xml2brl.)",
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
          "hlpb", "help-braille-bars",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (barsSubGroup);

    barsSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "abn", "all-bar-numbers",
R"(Generate Braille code to show all bar numbers.)",
          "showAllBarNumbers",
          fShowAllBarNumbers));
  }   


  // line breaks
  // --------------------------------------

  {
    // variables
      
    fIgnoreLineBreaks                    = boolOptionsInitialValue;
  
    fBreakLinesAtIncompleteRightMeasures = boolOptionsInitialValue;
        
    // options
  
    S_optionsSubGroup
      lineBreaksSubGroup =
        optionsSubGroup::create (
          "Line breaks",
          "hlplb", "help-braille-line-breaks",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (lineBreaksSubGroup);

    lineBreaksSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "ilb", "ignore-line-breaks",
R"(Ignore the line breaks from the MusicXML input
and let Braille decide about them.)",
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
          "hlppb", "help-braille-page-breaks",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (pageBreaksSubGroup);

    pageBreaksSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "ipb", "ignore-page-breaks",
R"(Ignore the page breaks from the MusicXML input
and let Braille decide about them.)",
          "ignorePageBreaks",
          fIgnorePageBreaks));
  }
        */  
     
  // code generation
  // --------------------------------------
  
  {
    // variables  
  
    fXml2brlInfos         = boolOptionsInitialValue;
                
    fNoBrailleCode       = boolOptionsInitialValue;
    
    fNoBrailleLyrics     = boolOptionsInitialValue;
    
    fBrailleCompileDate  = boolOptionsInitialValue;
    
    // options
  
    S_optionsSubGroup
      codeGenerationSubGroup =
        optionsSubGroup::create (
          "Code generation",
          "hlpcg", "help-braille-code-generation",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (codeGenerationSubGroup);

    codeGenerationSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "xi", "xml2brl-infos",
R"(Generate initial comments showing the compilation date and options.)",
          "xml2brlInfos",
          fXml2brlInfos));

    codeGenerationSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "nolpc", "no-braille-code",
R"(Don't generate any Braille code.
That can be useful if only a summary of the score is needed.)",
          "noBrailleCode",
          fNoBrailleCode));

    codeGenerationSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "nolpl", "no-braille-lyrics",
R"(Don't generate any lyrics in the Braille code.)",
          "noBrailleLyrics",
          fNoBrailleLyrics));

    codeGenerationSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "lpcd", "braille-compile-date",
R"(Generate code to include the compilation date
when Braille creates the score.)",
          "brailleCompileDate",
          fBrailleCompileDate));
  }
}

S_brailleOptions brailleOptions::createCloneWithDetailedTrace ()
{
  S_brailleOptions
    clone =
      brailleOptions::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler uplink
  clone->
    setOptionsHandlerUplink (
      fOptionsHandlerUplink);

/*
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
    
  // Braille informations
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


  // notes
  // --------------------------------------
  
  clone->fAbsoluteOctaves =
    fAbsoluteOctaves;

  clone->fAllDurations =
    fAllDurations;

  clone->fRomanStringNumbers =
    fRomanStringNumbers;

  clone->fAvoidOpenStrings =
    fAvoidOpenStrings;

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

  // page breaks
  // --------------------------------------

  clone->fIgnorePageBreaks =
    fIgnorePageBreaks;
*/

  // code generation
  // --------------------------------------

  clone->fNoBrailleCode =
    fNoBrailleCode;
    
  clone->fNoBrailleLyrics =
    fNoBrailleLyrics;
    
  return clone;
}

//______________________________________________________________________________
void brailleOptions::enforceQuietness ()
{}

//______________________________________________________________________________
void brailleOptions::checkOptionsConsistency ()
{
  // JMI
}

void brailleOptions::printBrailleOptionsValues (int fieldWidth)
{
  gLogIOstream <<
    "The Braille options are:" <<
    endl;

  gIndenter++;
  
/*
  // identification
  // --------------------------------------
  gLogIOstream <<
    "Identification:" <<
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

  // Braille informations

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
    
    setw (fieldWidth) << "romanStringNumbers" << " : " <<
      booleanAsString (fRomanStringNumbers) <<
      endl <<
      
    setw (fieldWidth) << "avoidOpenString" << " : " <<
      booleanAsString (fAvoidOpenStrings) <<
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
*/
  
  // code generation
  // --------------------------------------

  gLogIOstream <<
    "Braille code generation:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "noBrailleCode" << " : " <<
      booleanAsString (fNoBrailleCode) <<
      endl <<
    
    setw (fieldWidth) << "noBrailleLyrics" << " : " <<
      booleanAsString (fNoBrailleLyrics) <<
      endl <<
    
    setw (fieldWidth) << "brailleCompileDate" << " : " <<
      booleanAsString (fBrailleCompileDate) <<
      endl;

  gIndenter--;
  

  gIndenter--;
}

S_optionsItem brailleOptions::handleOptionsItem (
  ostream&      os,
  S_optionsItem item)
{
  S_optionsItem result;

  /*
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
  */

  return result;
}

void brailleOptions::handleOptionsItemValue (
  ostream&      os,
  S_optionsItem item,
  string        theString)
{
  /*
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
        "BSR accidental style " << theString <<
        " is unknown" <<
        endl <<
        "The " <<
        gLpsrAccidentalStyleKindsMap.size () - 1 <<
        " known BSR accidental styles are:" <<
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
  */
}
        
string brailleOptions::byteOrderingKindAsString (
  bsrByteOrderingKind byteOrderingKind)
{
  string result;
  
  switch (byteOrderingKind) {
    case brailleOptions::kByteOrderingBigEndian:
      result = "byteOrderingBigEndian";
      break;
    case brailleOptions::kByteOrderingSmallEndian:
      result = "byteOrderingSmallEndian/Scot";
      break;
  } // switch

  return result;
}

ostream& operator<< (ostream& os, const S_brailleOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeBrailleOptionsHandling (
  S_optionsHandler optionsHandler)
{
  // create the options variables
  // ------------------------------------------------------
  
  gBrailleOptionsUserChoices = brailleOptions::create (
    optionsHandler);
  assert(gBrailleOptionsUserChoices != 0);
  
  gBrailleOptions =
    gBrailleOptionsUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

  gBrailleOptionsWithDetailedTrace =
    gBrailleOptions->
      createCloneWithDetailedTrace ();
}


}
