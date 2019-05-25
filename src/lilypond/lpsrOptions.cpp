/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <sstream>
#include <iomanip>      // setw, setprecision, ...

#include "utilities.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "lpsrOptions.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_optionsLpsrScoreOutputKindItem optionsLpsrScoreOutputKindItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsLpsrScoreOutputKindKindItemVariableDisplayName,
  lpsrScoreOutputKind&
                     optionsLpsrScoreOutputKindKindItemVariable)
{
  optionsLpsrScoreOutputKindItem* o = new
    optionsLpsrScoreOutputKindItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsLpsrScoreOutputKindKindItemVariableDisplayName,
      optionsLpsrScoreOutputKindKindItemVariable);
  assert(o!=0);
  return o;
}

optionsLpsrScoreOutputKindItem::optionsLpsrScoreOutputKindItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsLpsrScoreOutputKindKindItemVariableDisplayName,
  lpsrScoreOutputKind&
                     optionsLpsrScoreOutputKindKindItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsLpsrScoreOutputKindKindItemVariableDisplayName (
      optionsLpsrScoreOutputKindKindItemVariableDisplayName),
    fOptionsLpsrScoreOutputKindKindItemVariable (
      optionsLpsrScoreOutputKindKindItemVariable)
{}

optionsLpsrScoreOutputKindItem::~optionsLpsrScoreOutputKindItem ()
{}

void optionsLpsrScoreOutputKindItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;

  os <<
    "OptionsLpsrScoreOutputKindItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsLpsrPitchesLanguagKindeItemVariableDisplayName" << " : " <<
    fOptionsLpsrScoreOutputKindKindItemVariableDisplayName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsLpsrScoreOutputKindItemVariable" << " : \"" <<
    lpsrScoreOutputKindAsString (
      fOptionsLpsrScoreOutputKindKindItemVariable) <<
      "\"" <<
    endl;
}

void optionsLpsrScoreOutputKindItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsLpsrScoreOutputKindKindItemVariableDisplayName <<
    " : \"" <<
    lpsrScoreOutputKindAsString (
      fOptionsLpsrScoreOutputKindKindItemVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsLpsrScoreOutputKindItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsLpsrPitchesLanguageItem optionsLpsrPitchesLanguageItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsLpsrPitchesLanguageKindItemVariableDisplayName,
  msrQuarterTonesPitchesLanguageKind&
                     optionsLpsrPitchesLanguageKindItemVariable)
{
  optionsLpsrPitchesLanguageItem* o = new
    optionsLpsrPitchesLanguageItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsLpsrPitchesLanguageKindItemVariableDisplayName,
      optionsLpsrPitchesLanguageKindItemVariable);
  assert(o!=0);
  return o;
}

optionsLpsrPitchesLanguageItem::optionsLpsrPitchesLanguageItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsLpsrPitchesLanguageKindItemVariableDisplayName,
  msrQuarterTonesPitchesLanguageKind&
                     optionsLpsrPitchesLanguageKindItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsLpsrPitchesLanguageKindItemVariableDisplayName (
      optionsLpsrPitchesLanguageKindItemVariableDisplayName),
    fOptionsLpsrPitchesLanguageKindItemVariable (
      optionsLpsrPitchesLanguageKindItemVariable)
{}

optionsLpsrPitchesLanguageItem::~optionsLpsrPitchesLanguageItem ()
{}

void optionsLpsrPitchesLanguageItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;

  os <<
    "OptionsLpsrPitchesLanguageItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsLpsrPitchesLanguagKindeItemVariableDisplayName" << " : " <<
    fOptionsLpsrPitchesLanguageKindItemVariableDisplayName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsLpsrPitchesLanguageItemVariable" << " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fOptionsLpsrPitchesLanguageKindItemVariable) <<
      "\"" <<
    endl;
}

void optionsLpsrPitchesLanguageItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsLpsrPitchesLanguageKindItemVariableDisplayName <<
    " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fOptionsLpsrPitchesLanguageKindItemVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsLpsrPitchesLanguageItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsLpsrChordsLanguageItem optionsLpsrChordsLanguageItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsLpsrChordsLanguageKindItemVariableDisplayName,
  lpsrChordsLanguageKind&
                     optionsLpsrChordsLanguageKindItemVariable)
{
  optionsLpsrChordsLanguageItem* o = new
    optionsLpsrChordsLanguageItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsLpsrChordsLanguageKindItemVariableDisplayName,
      optionsLpsrChordsLanguageKindItemVariable);
  assert(o!=0);
  return o;
}

optionsLpsrChordsLanguageItem::optionsLpsrChordsLanguageItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsLpsrChordsLanguageKindItemVariableDisplayName,
  lpsrChordsLanguageKind&
                     optionsLpsrChordsLanguageKindItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsLpsrChordsLanguageKindItemVariableDisplayName (
      optionsLpsrChordsLanguageKindItemVariableDisplayName),
    fOptionsLpsrChordsLanguageKindItemVariable (
      optionsLpsrChordsLanguageKindItemVariable)
{}

optionsLpsrChordsLanguageItem::~optionsLpsrChordsLanguageItem ()
{}

void optionsLpsrChordsLanguageItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;

  os <<
    "OptionsLpsrChordsLanguageItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsLpsrChordsLanguageKindItemVariableDisplayName" << " : " <<
    fOptionsLpsrChordsLanguageKindItemVariableDisplayName <<
    setw (fieldWidth) <<
    "fOptionsLpsrChordsLanguageKindItemVariable" << " : \"" <<
    lpsrChordsLanguageKindAsString (
      fOptionsLpsrChordsLanguageKindItemVariable) <<
      "\"" <<
    endl;
}

void optionsLpsrChordsLanguageItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsLpsrChordsLanguageKindItemVariableDisplayName <<
    " : \"" <<
    lpsrChordsLanguageKindAsString (
      fOptionsLpsrChordsLanguageKindItemVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsLpsrChordsLanguageItem& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_lpsrOptions gLpsrOptions;
S_lpsrOptions gLpsrOptionsUserChoices;
S_lpsrOptions gLpsrOptionsWithDetailedTrace;

S_lpsrOptions lpsrOptions::create (
  S_optionsHandler optionsHandler)
{
  lpsrOptions* o = new lpsrOptions (
    optionsHandler);
  assert(o!=0);
  return o;
}

lpsrOptions::lpsrOptions (
  S_optionsHandler optionsHandler)
  : optionsGroup (
    "LPSR",
    "hlpsr", "help-lpsr",
R"(These options control the way LPSR data is handled.)",
    optionsHandler)
{
  // append this options group to the options handler
  // if relevant
  if (optionsHandler) {
    optionsHandler->
      appendOptionsGroupToHandler (this);
  }

  // initialize it
  initializeLpsrOptions (false);
}

lpsrOptions::~lpsrOptions ()
{}

void lpsrOptions::initializeLpsrOptions (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OPTIONS
  // trace and display
  // --------------------------------------

  {
    // variables

    fTraceLpsr            = boolOptionsInitialValue;

    fTraceLpsrVisitors    = boolOptionsInitialValue;

    fTraceLpsrBlocks      = boolOptionsInitialValue;

    fDisplayLpsr          = boolOptionsInitialValue;

    fTraceSchemeFunctions = boolOptionsInitialValue;

    // options

    S_optionsSubGroup
      traceAndDisplaySubGroup =
        optionsSubGroup::create (
          "Trace and display",
          "hlpsrtd", "help-lpsr-trace-and-display",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);

    appendOptionsSubGroup (traceAndDisplaySubGroup);

    traceAndDisplaySubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "tlpsr", "trace-lpsr",
R"(Write a trace of the LPSR graphs visiting activity to standard error.)",
          "traceLpsr",
          fTraceLpsr));

    traceAndDisplaySubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "tlpsrv", "trace-lpsr-visitors",
R"(Write a trace of the LPSR tree visiting activity to standard error.)",
          "traceLpsrVisitors",
          fTraceLpsrVisitors));

  traceAndDisplaySubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "tlpsrb", "trace-lpsr-blocks",
R"(Write a trace of the LPSR blocks to standard error.)",
        "traceLpsrBlocks",
        fTraceLpsrBlocks));

    traceAndDisplaySubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "dlpsr", "display-lpsr",
R"(Write the contents of the LPSR data to standard error.)",
          "displayLpsr",
          fDisplayLpsr,
          gTraceOptions->fTracePasses));

    traceAndDisplaySubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "tsf", "trace-scheme-functions",
R"(Write a trace of the activity regarding Scheme functions to standard error.)",
          "traceSchemeFunctions",
          fTraceSchemeFunctions));
  }
#endif

  // LilyPond output kind
  // --------------------------------------

  {
    // variables

    const lpsrScoreOutputKind
      lpsrScoreOutputKindDefaultValue =
        kScoreOnly; // default value

    fScoreOutputKind = lpsrScoreOutputKindDefaultValue;

    // options

    S_optionsSubGroup
      lilypondOutputKindSubGroup =
        optionsSubGroup::create (
          "LilyPond output kind",
          "hlpok", "help-lilypond-output-kind",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);

    appendOptionsSubGroup (lilypondOutputKindSubGroup);

    lilypondOutputKindSubGroup->
      appendOptionsItem (
        optionsLpsrScoreOutputKindItem::create (
          "lpsok", "lpsr-score-output-kind",
          replaceSubstringInString (
            replaceSubstringInString (
              replaceSubstringInString (
R"(Use OUTPUT_KIND to create the LPSR blocks,
as well as in the generated LilyPond code.
The NUMBER LilyPond output kinds available are:
  OUTPUT_KINDS.
'-one-file' means that LilyPond will produce a single file containing all the scores and/or parts.
Otherwise, one file will be generated for each score and/or part.
The default is 'DEFAULT_VALUE'.)",
                "NUMBER",
                to_string (gLpsrScoreOutputKindsMap.size ())),
              "OUTPUT_KINDS",
              existingLpsrScoreOutputKinds ()),
            "DEFAULT_VALUE",
            lpsrScoreOutputKindAsString (
              lpsrScoreOutputKindDefaultValue)),
          "OUTPUT_KIND",
          "scoreOutputKind",
          fScoreOutputKind));
  }


  // lyrics vs words
  // --------------------------------------

  {
    // variables

    fAddWordsFromTheLyrics = boolOptionsInitialValue;

    // options

    S_optionsSubGroup
      traceAndDisplaySubGroup =
        optionsSubGroup::create (
          "Lyrics vs words",
          "hlyrsvswords", "help-lyrics-vs-words",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);

    appendOptionsSubGroup (traceAndDisplaySubGroup);

    traceAndDisplaySubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "awftl", "add-words-from-the-lyrics",
R"(Add words with the lyrics contents, keeping the latter untouched.
This may come in handy when MusicXML data has been obtained from scanned images.)",
          "addWordsFromTheLyrics",
          fAddWordsFromTheLyrics));
  }


  // languages
  // --------------------------------------

  {
    // variables

    if (! setLpsrQuarterTonesPitchesLanguage ("nederlands")) {
      stringstream s;

      s <<
        "INTERNAL INITIALIZATION ERROR: "
        "LPSR pitches language 'nederlands' is unknown" <<
        endl <<
        "The " <<
        gQuarterTonesPitchesLanguageKindsMap.size () <<
        " known LPSR pitches languages are:" <<
        endl;

      gIndenter++;

      s <<
        existingQuarterTonesPitchesLanguageKinds ();

      gIndenter--;

      optionError (s.str ());
    }

    const msrQuarterTonesPitchesLanguageKind
      msrQuarterTonesPitchesLanguageKindDefaultValue =
        fLpsrQuarterTonesPitchesLanguageKind;

    fLpsrQuarterTonesPitchesLanguageKind =
      msrQuarterTonesPitchesLanguageKindDefaultValue;

    const lpsrChordsLanguageKind
      lpsrChordsLanguageKindDefaultValue =
        k_IgnatzekChords; // LilyPond default

    fLpsrChordsLanguageKind =
      lpsrChordsLanguageKindDefaultValue;

    // options

    S_optionsSubGroup
      languagesSubGroup =
        optionsSubGroup::create (
          "Languages",
          "hlpsrl", "help-lpsr-languages",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);

    appendOptionsSubGroup (languagesSubGroup);

    languagesSubGroup->
      appendOptionsItem (
        optionsLpsrPitchesLanguageItem::create (
          "lppl", "lpsr-pitches-language",
          replaceSubstringInString (
            replaceSubstringInString (
              replaceSubstringInString (
R"(Use LANGUAGE to display note pitches in the LPSR logs and views,
as well as in the generated LilyPond code.
The NUMBER LilyPond pitches languages available are:
  PITCHES_LANGUAGES.
The default is 'DEFAULT_VALUE'.)",
                "NUMBER",
                to_string (gQuarterTonesPitchesLanguageKindsMap.size ())),
              "PITCHES_LANGUAGES",
              existingQuarterTonesPitchesLanguageKinds ()),
            "DEFAULT_VALUE",
            msrQuarterTonesPitchesLanguageKindAsString (
              msrQuarterTonesPitchesLanguageKindDefaultValue)),
          "LANGUAGE",
          "lpsrPitchesanguage",
          fLpsrQuarterTonesPitchesLanguageKind));

    languagesSubGroup->
      appendOptionsItem (
        optionsLpsrChordsLanguageItem::create (
          "lpcl", "lpsr-chords-language",
          replaceSubstringInString (
            replaceSubstringInString (
              replaceSubstringInString (
R"(Use LANGUAGE to display chord names, their root and bass notes,
in the LPSR logs and views and the generated LilyPond code.
The NUMBER LilyPond pitches languages available are:
  CHORDS_LANGUAGES.
'ignatzek' is Ignatzek's jazz-like, english naming used by LilyPond by default.
The default is 'DEFAULT_VALUE'.)",
                "NUMBER",
                to_string (gLpsrChordsLanguageKindsMap.size ())),
              "CHORDS_LANGUAGES",
              existingLpsrChordsLanguageKinds ()),
            "DEFAULT_VALUE",
            lpsrChordsLanguageKindAsString (
              lpsrChordsLanguageKindDefaultValue)),
          "LANGUAGE",
          "lpsr-chords-language",
          fLpsrChordsLanguageKind));
  }


  // exit after some passes
  // --------------------------------------

  {
    // variables

    // options

    S_optionsSubGroup
      exitAfterSomePassesSubGroup =
        optionsSubGroup::create (
          "Exit after some passes",
          "hge", "help-general-exit",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);

    appendOptionsSubGroup (exitAfterSomePassesSubGroup);

    // '-exit-3' is hidden...
    S_optionsBooleanItem
      exit3OptionsBooleanItem =
        optionsBooleanItem::create (
          "e3", "exit-3",
R"(Exit after pass 3, i.e. after conversion
of the MSR to LPSR.)",
          "exit3",
          fExit3);
    exit3OptionsBooleanItem->
      setOptionsElementIsHidden ();

    exitAfterSomePassesSubGroup->
      appendOptionsItem (
        exit3OptionsBooleanItem);
  }
}

S_lpsrOptions lpsrOptions::createCloneWithDetailedTrace ()
{
  S_lpsrOptions
    clone =
      lpsrOptions::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler uplink
  clone->
    setOptionsHandlerUplink (
      fOptionsHandlerUplink);


  // trace and display
  // --------------------------------------

  clone->fTraceLpsr =
    true;

  clone->fTraceLpsrVisitors =
    true;

  clone->fTraceLpsrBlocks =
    true;

  clone->fDisplayLpsr =
    true;

  clone->fTraceSchemeFunctions =
    true;


  // LilyPond output kind
  // --------------------------------------

  clone->fScoreOutputKind =
    fScoreOutputKind;


  // lyrics vs words
  // --------------------------------------

  clone->fAddWordsFromTheLyrics =
    true;


  // languages
  // --------------------------------------

  clone->fLpsrQuarterTonesPitchesLanguageKind =
    fLpsrQuarterTonesPitchesLanguageKind;

  clone->fLpsrChordsLanguageKind =
    fLpsrChordsLanguageKind;

  return clone;
}

//______________________________________________________________________________
bool lpsrOptions::setLpsrQuarterTonesPitchesLanguage (string language)
{
  // is language in the note names languages map?
  map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator
    it =
      gQuarterTonesPitchesLanguageKindsMap.find (language);

  if (it == gQuarterTonesPitchesLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    return false;
  }

  fLpsrQuarterTonesPitchesLanguageKind = (*it).second;

  return true;
}

//______________________________________________________________________________
bool lpsrOptions::setLpsrChordsLanguage (string language)
{
  // is language in the chords languages map?
  map<string, lpsrChordsLanguageKind>::const_iterator
    it =
      gLpsrChordsLanguageKindsMap.find (language);

  if (it == gLpsrChordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    return false;
  }

  fLpsrChordsLanguageKind = (*it).second;

  return true;
}

//______________________________________________________________________________
void lpsrOptions::enforceQuietness ()
{
  fTraceLpsr = false;
  fTraceLpsrVisitors = false;
  fTraceLpsrBlocks = false;
  fDisplayLpsr = false;
  fTraceSchemeFunctions = false;
}

//______________________________________________________________________________
void lpsrOptions::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void lpsrOptions::printLpsrOptionsValues (int fieldWidth)
{
  gLogIOstream <<
    "The LPSR options are:" <<
    endl;

  gIndenter++;

  // trace and display
  // --------------------------------------
  gLogIOstream <<
    "Trace and display:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "traceLpsr" << " : " <<
    booleanAsString (fTraceLpsr) <<
    endl <<

    setw (fieldWidth) << "traceLpsrVisitors" << " : " <<
    booleanAsString (fTraceLpsrVisitors) <<
    endl <<

    setw (fieldWidth) << "traceLpsrBlocks" << " : " <<
    booleanAsString (fTraceLpsrBlocks) <<
    endl <<

    setw (fieldWidth) << "displayLpsr" << " : " <<
    booleanAsString (fDisplayLpsr) <<
    endl <<

    setw (fieldWidth) << "traceSchemeFunctions" << " : " <<
    booleanAsString (fTraceSchemeFunctions) <<
    endl;

  gIndenter--;

  // LilyPond output kind
  // --------------------------------------

  gLogIOstream <<
    "LilyPond output kind:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "scoreOutputKind" << " : " <<
    lpsrScoreOutputKindAsString (fScoreOutputKind) <<
    endl;

  gIndenter--;

  // lyrics vs words
  // --------------------------------------

  gLogIOstream <<
    "Lyrics vs words:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "addWordsFromTheLyrics" << " : " <<
    booleanAsString (fAddWordsFromTheLyrics) <<
    endl;

  gIndenter--;

  // languages
  // --------------------------------------

  gLogIOstream <<
    "Languages:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "lpsrQuarterTonesPitchesLanguage" << " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fLpsrQuarterTonesPitchesLanguageKind) <<
    "\"" <<
    endl <<

    setw (fieldWidth) << "lpsrChordsLanguage" << " : \"" <<
    lpsrChordsLanguageKindAsString (
      fLpsrChordsLanguageKind) <<
    "\"" <<
    endl;

  gIndenter--;


  gIndenter--;
}

S_optionsItem lpsrOptions::handleOptionsItem (
  ostream&      os,
  S_optionsItem item)
{
  S_optionsItem result;

  if (
    // LPSR score output kind item?
    S_optionsLpsrScoreOutputKindItem
      scoreOutputKindItem =
        dynamic_cast<optionsLpsrScoreOutputKindItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsLpsrScoreOutputKindItem'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = scoreOutputKindItem;
  }

  else if (
    // LPSR pitches language item?
    S_optionsLpsrPitchesLanguageItem
      pitchesLanguageItem =
        dynamic_cast<optionsLpsrPitchesLanguageItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsLpsrPitchesLanguageItem'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = pitchesLanguageItem;
  }

  else if (
    // chords language item?
    S_optionsLpsrChordsLanguageItem
      LpsrChordsLanguageItem =
        dynamic_cast<optionsLpsrChordsLanguageItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsLpsrChordsLanguageItem'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = LpsrChordsLanguageItem;
  }

  return result;
}

void lpsrOptions::handleOptionsItemValue (
  ostream&      os,
  S_optionsItem item,
  string        theString)
{
  if (
    // LPSR score output kind item?
    S_optionsLpsrScoreOutputKindItem
      scoreOutputKindItem =
        dynamic_cast<optionsLpsrScoreOutputKindItem*>(&(*item))
  ) {
    // theString contains the score output kind:
    // is it in the score output kinds map?

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsLpsrScoreOutputKindItem'" <<
        endl;
    }
#endif

    map<string, lpsrScoreOutputKind>::const_iterator
      it =
        gLpsrScoreOutputKindsMap.find (
          theString);

    if (it == gLpsrScoreOutputKindsMap.end ()) {
      // no, language is unknown in the map

      printHelpSummary (os);

      stringstream s;

      s <<
        "LPSR score output kind " << theString <<
        " is unknown" <<
        endl <<
        "The " <<
        gLpsrScoreOutputKindsMap.size () <<
        " known LPSR score output kinds are:" <<
        endl;

      gIndenter++;

      s <<
        existingLpsrScoreOutputKinds ();

      gIndenter--;

      optionError (s.str ());

 //     exit (4); // JMI
      abort ();
    }

    scoreOutputKindItem->
      setScoreOutputKindKindItemVariableValue (
        (*it).second);
  }

  else if (
    // LPSR pitches language item?
    S_optionsLpsrPitchesLanguageItem
      pitchesLanguageKindItem =
        dynamic_cast<optionsLpsrPitchesLanguageItem*>(&(*item))
  ) {
    // theString contains the language name:
    // is it in the pitches languages map?

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsLpsrPitchesLanguageItem'" <<
        endl;
    }
#endif

    map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator
      it =
        gQuarterTonesPitchesLanguageKindsMap.find (
          theString);

    if (it == gQuarterTonesPitchesLanguageKindsMap.end ()) {
      // no, language is unknown in the map

      printHelpSummary (os);

      stringstream s;

      s <<
        "LPSR pitches language " << theString <<
        " is unknown" <<
        endl <<
        "The " <<
        gQuarterTonesPitchesLanguageKindsMap.size () <<
        " known LPSR pitches languages are:" <<
        endl;

      gIndenter++;

      s <<
        existingQuarterTonesPitchesLanguageKinds ();

      gIndenter--;

      optionError (s.str ());

 //     exit (4); // JMI
      abort ();
    }

    pitchesLanguageKindItem->
      setPitchesLanguageKindItemVariableValue (
        (*it).second);
  }

  else if (
    // chords language item?
    S_optionsLpsrChordsLanguageItem
      LpsrChordsLanguageItem =
        dynamic_cast<optionsLpsrChordsLanguageItem*>(&(*item))
  ) {
    // theString contains the language name:
    // is it in the chords languages map?

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsLpsrChordsLanguageItem'" <<
        endl;
    }
#endif

    map<string, lpsrChordsLanguageKind>::const_iterator
      it =
        gLpsrChordsLanguageKindsMap.find (theString);

    if (it == gLpsrChordsLanguageKindsMap.end ()) {
      // no, language is unknown in the map
      stringstream s;

      s <<
        "LPSR chords language " << theString <<
        " is unknown" <<
        endl <<
        "The " <<
        gLpsrChordsLanguageKindsMap.size () - 1 <<
        " known LPSR chords languages apart from the default Ignatzek are:" <<
        endl;

      gIndenter++;

      s <<
        existingLpsrChordsLanguageKinds ();

      gIndenter--;

      optionError (s.str ());

      printHelpSummary (os);

      exit (4);
    }

    LpsrChordsLanguageItem->
      setLpsrChordsLanguageKindItemVariableValue (
        (*it).second);
  }
}

ostream& operator<< (ostream& os, const S_lpsrOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeLpsrOptionsHandling (
  S_optionsHandler optionsHandler)
{
  // create the options variables
  // ------------------------------------------------------

  gLpsrOptionsUserChoices = lpsrOptions::create (
    optionsHandler);
  assert(gLpsrOptionsUserChoices != 0);

  gLpsrOptions =
    gLpsrOptionsUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------
  gLpsrOptionsWithDetailedTrace =
    gLpsrOptions->
      createCloneWithDetailedTrace ();
}


}
