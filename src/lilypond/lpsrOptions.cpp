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
#include <string>
#include <regex>

#include "utilities.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "generalOptions.h"
#include "lpsrOptions.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_lpsrScoreOutputKindAtom lpsrScoreOutputKindAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  lpsrScoreOutputKind&
                     lpsrScoreOutputKindKindVariable)
{
  lpsrScoreOutputKindAtom* o = new
    lpsrScoreOutputKindAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      lpsrScoreOutputKindKindVariable);
  assert(o!=0);
  return o;
}

lpsrScoreOutputKindAtom::lpsrScoreOutputKindAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  lpsrScoreOutputKind&
                     lpsrScoreOutputKindKindVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fLpsrScoreOutputKindVariable (
      lpsrScoreOutputKindKindVariable)
{}

lpsrScoreOutputKindAtom::~lpsrScoreOutputKindAtom ()
{}

void lpsrScoreOutputKindAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OPTIONS
  if (gOahBasicOptions->fTraceOptions) {
    os <<
      "==> oahAtom is of type 'lpsrScoreOutputKindAtom'" <<
      endl;
  }
#endif

  // theString contains the score output kind:
  // is it in the score output kinds map?

#ifdef TRACE_OPTIONS
  if (gOahBasicOptions->fTraceOptions) {
    os <<
      "==> oahAtom is of type 'lpsrScoreOutputKindAtom'" <<
      endl;
  }
#endif

  map<string, lpsrScoreOutputKind>::const_iterator
    it =
      gLpsrScoreOutputKindsMap.find (
        theString);

  if (it == gLpsrScoreOutputKindsMap.end ()) {
    // no, score output kind is unknown in the map

    stringstream s;

    s <<
      "LPSR score output kind '" << theString <<
      "' is unknown" <<
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

  setLpsrScoreOutputKindVariable (
    (*it).second);
}

void lpsrScoreOutputKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsLpsrScoreOutputKindAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsLpsrScoreOutputKindVariable" << " : \"" <<
    lpsrScoreOutputKindAsString (
      fLpsrScoreOutputKindVariable) <<
    "\"" <<
    endl;
}

void lpsrScoreOutputKindAtom::printOptionsValues (
  indentedOstream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    lpsrScoreOutputKindAsString (
      fLpsrScoreOutputKindVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_lpsrScoreOutputKindAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrPitchesLanguageAtom lpsrPitchesLanguageAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msrQuarterTonesPitchesLanguageKind&
                     lpsrPitchesLanguageKindVariable)
{
  lpsrPitchesLanguageAtom* o = new
    lpsrPitchesLanguageAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      lpsrPitchesLanguageKindVariable);
  assert(o!=0);
  return o;
}

lpsrPitchesLanguageAtom::lpsrPitchesLanguageAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msrQuarterTonesPitchesLanguageKind&
                     lpsrPitchesLanguageKindVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fMsrQuarterTonesPitchesLanguageKindVariable (
      lpsrPitchesLanguageKindVariable)
{}

lpsrPitchesLanguageAtom::~lpsrPitchesLanguageAtom ()
{}

void lpsrPitchesLanguageAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OPTIONS
  if (gOahBasicOptions->fTraceOptions) {
    os <<
      "==> oahAtom is of type 'lpsrPitchesLanguageAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the pitches languages map?

#ifdef TRACE_OPTIONS
  if (gOahBasicOptions->fTraceOptions) {
    os <<
      "==> oahAtom is of type 'lpsrPitchesLanguageAtom'" <<
      endl;
  }
#endif

  map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator
    it =
      gQuarterTonesPitchesLanguageKindsMap.find (
        theString);

  if (it == gQuarterTonesPitchesLanguageKindsMap.end ()) {
    // no, language is unknown in the map

    stringstream s;

    s <<
      "LPSR pitches language '" << theString <<
      "' is unknown" <<
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

  setMsrQuarterTonesPitchesLanguageKindVariable (
    (*it).second);
}

void lpsrPitchesLanguageAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsLpsrPitchesLanguageAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsLpsrPitchesLanguageVariable" << " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable) <<
    "\"" <<
    endl;
}

void lpsrPitchesLanguageAtom::printOptionsValues (
  indentedOstream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_lpsrPitchesLanguageAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrChordsLanguageAtom lpsrChordsLanguageAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  lpsrChordsLanguageKind&
                     lpsrChordsLanguageKindVariable)
{
  lpsrChordsLanguageAtom* o = new
    lpsrChordsLanguageAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      lpsrChordsLanguageKindVariable);
  assert(o!=0);
  return o;
}

lpsrChordsLanguageAtom::lpsrChordsLanguageAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  lpsrChordsLanguageKind&
                     lpsrChordsLanguageKindVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fLpsrChordsLanguageKindVariable (
      lpsrChordsLanguageKindVariable)
{}

lpsrChordsLanguageAtom::~lpsrChordsLanguageAtom ()
{}

void lpsrChordsLanguageAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OPTIONS
  if (gOahBasicOptions->fTraceOptions) {
    os <<
      "==> oahAtom is of type 'lpsrChordsLanguageAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the chords languages map?

#ifdef TRACE_OPTIONS
  if (gOahBasicOptions->fTraceOptions) {
    os <<
      "==> oahAtom is of type 'lpsrChordsLanguageAtom'" <<
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
      "LPSR chords language '" << theString <<
      "' is unknown" <<
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
    exit (4);
  }

  setLpsrChordsLanguageKindVariable (
    (*it).second);
}

void lpsrChordsLanguageAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsLpsrChordsLanguageAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fLpsrChordsLanguageKindVariable" << " : \"" <<
    lpsrChordsLanguageKindAsString (
      fLpsrChordsLanguageKindVariable) <<
    "\"" <<
    endl;
}

void lpsrChordsLanguageAtom::printOptionsValues (
  indentedOstream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    lpsrChordsLanguageKindAsString (
      fLpsrChordsLanguageKindVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_lpsrChordsLanguageAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrTransposeAtom lpsrTransposeAtom::create (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  S_msrSemiTonesPitchAndOctave&
          lpsrTransposeVariable)
{
  lpsrTransposeAtom* o = new
    lpsrTransposeAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      lpsrTransposeVariable);
  assert(o!=0);
  return o;
}

lpsrTransposeAtom::lpsrTransposeAtom (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  S_msrSemiTonesPitchAndOctave&
          lpsrTransposeVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fMsrSemiTonesPitchAndOctaveVariable (
      lpsrTransposeVariable)
{}

lpsrTransposeAtom::~lpsrTransposeAtom ()
{}

void lpsrTransposeAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OPTIONS
  if (gOahBasicOptions->fTraceOptions) {
    os <<
      "==> oahAtom is of type 'lpsrTransposeAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the chords languages map?

#ifdef TRACE_OPTIONS
  if (gOahBasicOptions->fTraceOptions) {
    os <<
      "==> oahAtom is of type 'lpsrTransposeAtom'" <<
      endl;
  }
#endif

  // create the semitones pitch and octave from theString
  S_msrSemiTonesPitchAndOctave
    semiTonesPitchAndOctave =
      msrSemiTonesPitchAndOctave::createFromString (
        K_NO_INPUT_LINE_NUMBER,
        theString);

  // set the transpose atom variable value
  setMsrSemiTonesPitchAndOctaveVariable (
    semiTonesPitchAndOctave);
}

void lpsrTransposeAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "lpsrTransposeAtom:" <<
    endl;

  gIndenter++;

  oahOption::printOptionEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void lpsrTransposeAtom::printOptionsValues (
  indentedOstream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";
  if (fMsrSemiTonesPitchAndOctaveVariable) {
    os << endl;
    gIndenter++;
    os <<
      fMsrSemiTonesPitchAndOctaveVariable;
    gIndenter--;
  }
  else {
    os <<
      "none" <<
      endl;
  }
}

ostream& operator<< (ostream& os, const S_lpsrTransposeAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_lpsrOptions gLpsrOptions;
S_lpsrOptions gLpsrOptionsUserChoices;
S_lpsrOptions gLpsrOptionsWithDetailedTrace;

S_lpsrOptions lpsrOptions::create (
  S_oahHandler handler)
{
  lpsrOptions* o = new lpsrOptions (
    handler);
  assert(o!=0);
  return o;
}

lpsrOptions::lpsrOptions (
  S_oahHandler handler)
  : oahGroup (
    "LPSR",
    "hlpsr", "help-lpsr",
R"(These options control the way LPSR data is handled.)",
    kElementVisibilityAlways,
    handler)
{
  // append this options group to the options handler
  // if relevant
  if (handler) {
    handler->
      appendGroupToHandler (this);
  }

  // initialize it
  initializeLpsrOptions (false);
}

lpsrOptions::~lpsrOptions ()
{}

#ifdef TRACE_OPTIONS
void lpsrOptions::initializeLpsrTraceOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    traceSubGroup =
      oahSubGroup::create (
        "Trace",
        "hlpsrtrace", "help-lpsr-trace",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (traceSubGroup);

  // trace- lpsr

  fTraceLpsr            = boolOptionsInitialValue;

  traceSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "tlpsr", "trace-lpsr",
R"(Write a trace of the LPSR graphs visiting activity to standard error.)",
        "traceLpsr",
        fTraceLpsr));

  // trace lilypond version

  fTraceLilypondVersion = boolOptionsInitialValue;

  traceSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "tlpv", "trace-lilypond-version",
R"(Write a trace of the LPSR graphs visiting activity to standard error.)",
        "traceLilypondVersion",
        fTraceLilypondVersion));

  // trace lpsr visitors

  fTraceLpsrVisitors    = boolOptionsInitialValue;

  traceSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "tlpsrv", "trace-lpsr-visitors",
R"(Write a trace of the LPSR tree visiting activity to standard error.)",
        "traceLpsrVisitors",
        fTraceLpsrVisitors));

  // trace lpsr blocks

  fTraceLpsrBlocks      = boolOptionsInitialValue;

  traceSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "tlpsrb", "trace-lpsr-blocks",
R"(Write a trace of the LPSR blocks to standard error.)",
        "traceLpsrBlocks",
        fTraceLpsrBlocks));

  // trace scheme functions

  fTraceSchemeFunctions = boolOptionsInitialValue;

  traceSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "tsf", "trace-scheme-functions",
R"(Write a trace of the activity regarding Scheme functions to standard error.)",
        "traceSchemeFunctions",
        fTraceSchemeFunctions));
}
#endif

void lpsrOptions::initializeLpsrDisplayOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    displaySubGroup =
      oahSubGroup::create (
        "Display",
        "hlpsrd", "help-lpsr-display",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (displaySubGroup);

  // display lpsr

  fDisplayLpsr = boolOptionsInitialValue;

  displaySubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "dlpsr", "display-lpsr",
R"(Write the contents of the LPSR data to standard error.)",
        "displayLpsr",
        fDisplayLpsr));
}

void lpsrOptions::initializeLilypondScoreOutputOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    lilypondOutputKindGroup =
      oahSubGroup::create (
        "LilyPond score  output",
        "hlpso", "help-lilypond-score-output",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (lilypondOutputKindGroup);

  // lilypond version

  string lilyPondVersionDefaultValue = "2.19.83";

  fLilyPondVersion = lilyPondVersionDefaultValue;

  lilypondOutputKindGroup->
    appendAtom (
      oahStringAtom::create (
        "lpv", "lilypond-version",
        replaceSubstringInString (
R"(Set the LilyPond '\version' to STRING in the LilyPond code.
The default is 'DEFAULT_VALUE'.)",
          "DEFAULT_VALUE",
          lilyPondVersionDefaultValue),
        "STRING",
        "lilyPondVersion",
        fLilyPondVersion));

  // lpsr score output kind

  const lpsrScoreOutputKind
    lpsrScoreOutputKindDefaultValue =
      kScoreOnly; // default value

  fScoreOutputKind = lpsrScoreOutputKindDefaultValue;

  lilypondOutputKindGroup->
    appendAtom (
      lpsrScoreOutputKindAtom::create (
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

  // global staff size

  fStaffGlobalSizeDefaultValue = 20; // LilyPond default
  fGlobalStaffSize = fStaffGlobalSizeDefaultValue;

  lilypondOutputKindGroup->
    appendAtom (
      oahFloatAtom::create (
        "gss", "global-staff-size",
        replaceSubstringInString (
R"(Set the LilyPond '#(set-global-staff-size ...)' to FLOAT in the LilyPond code.
FLOAT should be a decimal number with at least one digit before the dot,
without any sign.
The default is 'DEFAULT_VALUE'.)",
          "DEFAULT_VALUE",
          to_string (fStaffGlobalSizeDefaultValue)),
        "FLOAT",
        "globalStaffSize",
        fGlobalStaffSize));
}

void lpsrOptions::initializeLpsrWordsOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Words",
        "hlpsrwords", "help-lpsr-words",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (subGroup);

  // convert words to tempo

  fConvertWordsToTempo = boolOptionsInitialValue;

  subGroup->
    appendAtom (
      oahBooleanAtom::create (
        "cwtt", "convert-words-to-tempo",
R"(Convert words to tempo.
This may come in handy when MusicXML data has been obtained from scanned instrumental music  images.)",
        "convertWordsToTempo",
        fConvertWordsToTempo));

  // add words from the lyrics

  fAddWordsFromTheLyrics = boolOptionsInitialValue;

  subGroup->
    appendAtom (
      oahBooleanAtom::create (
        "awftl", "add-words-from-the-lyrics",
R"(Add words with the lyrics contents, keeping the latter untouched.
This may come in handy when MusicXML data has been obtained from scanned images.)",
        "addWordsFromTheLyrics",
        fAddWordsFromTheLyrics));
}

void lpsrOptions::initializeLpsrLanguagesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    languagesSubGroup =
      oahSubGroup::create (
        "Languages",
        "hlpsrl", "help-lpsr-languages",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (languagesSubGroup);

  // lpsr pitches language

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

  languagesSubGroup->
    appendAtom (
      lpsrPitchesLanguageAtom::create (
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
        "lpsrPitchesLanguage",
        fLpsrQuarterTonesPitchesLanguageKind));

  // lpsr chords language

  const lpsrChordsLanguageKind
    lpsrChordsLanguageKindDefaultValue =
      k_IgnatzekChords; // LilyPond default

  fLpsrChordsLanguageKind =
    lpsrChordsLanguageKindDefaultValue;

  languagesSubGroup->
    appendAtom (
      lpsrChordsLanguageAtom::create (
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

void lpsrOptions::initializeLpsrTransposeOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    languagesSubGroup =
      oahSubGroup::create (
        "Transpose",
        "hlpsrt", "help-lpsr-transpose",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (languagesSubGroup);

  // lpsr transpose

  languagesSubGroup->
    appendAtom (
      lpsrTransposeAtom::create (
        "lpt", "lpsr-transpose",
R"(Use TRANSPOSITION to tranpose in the LPSR data,
thus as in the generated LilyPond code as well.
TRANSPOSITION should contain a diatonic pitch, followed if needed
by a sequence of ',' or '\'' octave indications.
Such indications cannot be mixed.
For example, 'a', 'f' and 'bes,' can be used respectively
for instruments in 'a', 'f' and B flat respectively)",
        "TRANSPOSITION",
        "lpsrTranspose",
        fTransposeSemiTonesPitchAndOctave));
}

void lpsrOptions::initializeLpsrExitAfterSomePassesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    exitAfterSomePassesSubGroup =
      oahSubGroup::create (
        "Exit after some passes",
        "hlpexit", "help-lpsr-exit",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (exitAfterSomePassesSubGroup);

  // exit after pass 3

  fExit3 = boolOptionsInitialValue;

  S_oahBooleanAtom
    exit3OptionsBooleanAtom =
      oahBooleanAtom::create (
        "e3", "exit-3",
R"(Exit after pass 3, i.e. after conversion
of the MSR to LPSR.)",
        "exit3",
        fExit3);

  exitAfterSomePassesSubGroup->
    appendAtom (
      exit3OptionsBooleanAtom);
}

void lpsrOptions::initializeLpsrOptions (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OPTIONS
  // trace and display
  // --------------------------------------
  initializeLpsrTraceOptions (
    boolOptionsInitialValue);
#endif

  // display
  // --------------------------------------
  initializeLpsrDisplayOptions (
    boolOptionsInitialValue);

  // LilyPond score output
  // --------------------------------------
  initializeLilypondScoreOutputOptions (
    boolOptionsInitialValue);

  // words
  // --------------------------------------
  initializeLpsrWordsOptions (
    boolOptionsInitialValue);

  // languages
  // --------------------------------------
  initializeLpsrLanguagesOptions (
    boolOptionsInitialValue);

  // transpose
  // --------------------------------------
  initializeLpsrTransposeOptions (
    boolOptionsInitialValue);

  // exit after some passes
  // --------------------------------------
  initializeLpsrExitAfterSomePassesOptions (
    boolOptionsInitialValue);
}

S_lpsrOptions lpsrOptions::createCloneWithDetailedTrace ()
{
  S_lpsrOptions
    clone =
      lpsrOptions::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;

  // trace
  // --------------------------------------

#ifdef TRACE_OPTIONS
  clone->fTraceLpsr =
    true;

  clone->fTraceLilypondVersion =
    true;

  clone->fTraceLpsrVisitors =
    true;

  clone->fTraceLpsrBlocks =
    true;

  clone->fTraceSchemeFunctions =
    true;
#endif

  // display
  // --------------------------------------

  clone->fDisplayLpsr =
    true;

  // LilyPond version
  // --------------------------------------

  clone->fLilyPondVersion =
    fLilyPondVersion;

  // LilyPond output kind
  // --------------------------------------

  clone->fScoreOutputKind =
    fScoreOutputKind;

  // global staff size
  // --------------------------------------

  clone->fGlobalStaffSize =
    fGlobalStaffSize;

  // words
  // --------------------------------------

  clone->fConvertWordsToTempo =
    true;
  clone->fAddWordsFromTheLyrics =
    true;

  // languages
  // --------------------------------------

  clone->fLpsrQuarterTonesPitchesLanguageKind =
    fLpsrQuarterTonesPitchesLanguageKind;

  clone->fLpsrChordsLanguageKind =
    fLpsrChordsLanguageKind;

  // transpose
  // --------------------------------------

  clone->fTransposeSemiTonesPitchAndOctave =
    fTransposeSemiTonesPitchAndOctave;

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
#ifdef TRACE_OPTIONS
  fTraceLpsr = false;
  fTraceLilypondVersion = false;
  fTraceLpsrVisitors = false;
  fTraceLpsrBlocks = false;
  fTraceSchemeFunctions = false;
#endif

  fDisplayLpsr = false;
}

//______________________________________________________________________________
void lpsrOptions::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void lpsrOptions::printLpsrOptionsValues (int fieldWidth)
{
  gLogOstream <<
    "The LPSR options are:" <<
    endl;

  gIndenter++;

#ifdef TRACE_OPTIONS
  // trace
  // --------------------------------------
  gLogOstream <<
    "Trace:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "traceLpsr" << " : " <<
    booleanAsString (fTraceLpsr) <<
    endl <<

    setw (fieldWidth) << "traceLilypondVersion" << " : " <<
    booleanAsString (fTraceLilypondVersion) <<
    endl <<

    setw (fieldWidth) << "traceLpsrVisitors" << " : " <<
    booleanAsString (fTraceLpsrVisitors) <<
    endl <<

    setw (fieldWidth) << "traceLpsrBlocks" << " : " <<
    booleanAsString (fTraceLpsrBlocks) <<
    endl <<

    setw (fieldWidth) << "traceSchemeFunctions" << " : " <<
    booleanAsString (fTraceSchemeFunctions) <<
    endl;

  gIndenter--;
#endif

  // display
  // --------------------------------------
  gLogOstream <<
    "Display:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "displayLpsr" << " : " <<
    booleanAsString (fDisplayLpsr) <<
    endl;

  gIndenter--;

  // LilyPond version
  // --------------------------------------

  gLogOstream <<
    "LilyPond version:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "lilyPondVersion" << " : " <<
    fLilyPondVersion <<
    endl;

  gIndenter--;

  // LilyPond output kind
  // --------------------------------------

  gLogOstream <<
    "LilyPond output kind:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "scoreOutputKind" << " : " <<
    lpsrScoreOutputKindAsString (fScoreOutputKind) <<
    endl;

  gIndenter--;

  // global staff size
  // --------------------------------------

  gLogOstream <<
    "global staff size:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "fGlobalStaffSize" << " : " <<
    fGlobalStaffSize <<
    endl;

  gIndenter--;

  // words
  // --------------------------------------

  gLogOstream <<
    "Words:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "convertWordsToTempo" << " : " <<
    booleanAsString (fConvertWordsToTempo) <<
    endl <<
    setw (fieldWidth) << "addWordsFromTheLyrics" << " : " <<
    booleanAsString (fAddWordsFromTheLyrics) <<
    endl;

  gIndenter--;

  // languages
  // --------------------------------------

  gLogOstream <<
    "Languages:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
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

  // transpose
  // --------------------------------------

  gLogOstream <<
    "Transpose:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "transposeSemiTonesPitchAndOctave" << " : ";

    if (fTransposeSemiTonesPitchAndOctave) {
      gLogOstream <<
        fTransposeSemiTonesPitchAndOctave->asString ();
    }
    else {
      gLogOstream <<
        "none";
    }
  gLogOstream << endl;

  gIndenter--;


  gIndenter--;
}

S_oahValuedAtom lpsrOptions::handleAtom (
  indentedOstream&  os,
  S_oahAtom atom)
{
  S_oahValuedAtom result;

  if (
    // LPSR score output kind atom?
    S_lpsrScoreOutputKindAtom
      scoreOutputKindAtom =
        dynamic_cast<lpsrScoreOutputKindAtom*>(&(*atom))
    ) {
#ifdef TRACE_OPTIONS
    if (gOahBasicOptions->fTraceOptions) {
      os <<
        "==> oahAtom is of type 'lpsrScoreOutputKindAtom'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = scoreOutputKindAtom;
  }

  else if (
    // LPSR pitches language atom?
    S_lpsrPitchesLanguageAtom
      pitchesLanguageAtom =
        dynamic_cast<lpsrPitchesLanguageAtom*>(&(*atom))
    ) {
#ifdef TRACE_OPTIONS
    if (gOahBasicOptions->fTraceOptions) {
      os <<
        "==> oahAtom is of type 'lpsrPitchesLanguageAtom'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = pitchesLanguageAtom;
  }

  else if (
    // chords language atom?
    S_lpsrChordsLanguageAtom
      LpsrChordsLanguageAtom =
        dynamic_cast<lpsrChordsLanguageAtom*>(&(*atom))
    ) {
#ifdef TRACE_OPTIONS
    if (gOahBasicOptions->fTraceOptions) {
      os <<
        "==> oahAtom is of type 'lpsrChordsLanguageAtom'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = LpsrChordsLanguageAtom;
  }

  else if (
    // transpose atom?
    S_lpsrTransposeAtom
      transposeAtom =
        dynamic_cast<lpsrTransposeAtom*>(&(*atom))
    ) {
#ifdef TRACE_OPTIONS
    if (gOahBasicOptions->fTraceOptions) {
      os <<
        "==> oahAtom is of type 'lpsrTransposeAtom'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = transposeAtom;
  }

  else {
    stringstream s;

    s <<
      "INTERNAL OPTION ERROR: "
      "lpsrOptions::handleAtom() cannot handle option atom" <<
      endl <<
      atom <<
      endl;

    optionError (s.str ());

    exit (5);
  }

  return result;
}

void lpsrOptions::handleValuedAtomValue (
  indentedOstream& os,
  S_oahAtom        atom,
  string           theString)
{
  if (
    // LPSR score output kind atom?
    S_lpsrScoreOutputKindAtom
      scoreOutputKindAtom =
        dynamic_cast<lpsrScoreOutputKindAtom*>(&(*atom))
  ) {
    scoreOutputKindAtom->handleValue (
      theString,
      os);
  }

  else if (
    // LPSR pitches language atom?
    S_lpsrPitchesLanguageAtom
      pitchesLanguageKindAtom =
        dynamic_cast<lpsrPitchesLanguageAtom*>(&(*atom))
  ) {
    pitchesLanguageKindAtom->handleValue (
      theString,
      os);
  }

  else if (
    // chords language atom?
    S_lpsrChordsLanguageAtom
      chordsLanguageAtom =
        dynamic_cast<lpsrChordsLanguageAtom*>(&(*atom))
  ) {
    chordsLanguageAtom->handleValue (
      theString,
      os);
  }

  else if (
    // transpose atom?
    S_lpsrTransposeAtom
      transposeAtom =
        dynamic_cast<lpsrTransposeAtom*>(&(*atom))
  ) {
    transposeAtom->handleValue (
      theString,
      os);
  }

  else {
    stringstream s;

    s <<
      "INTERNAL OPTION ERROR: "
      "lpsrOptions::handleValuedAtomValue() cannot handle option atom" <<
      endl <<
      atom <<
      endl;

    optionError (s.str ());

    exit (5);
  }
}

void lpsrOptions::crackLilypondVersionNumber (
  string theString,
  int&   generationNumber,
  int&   majorNumber,
  int&   minorNumber)
{
  // obtains the three numbers in "2.19.83" or "2.20" for example

  // decipher theString with a three-number regular expression
  string regularExpression (
    "([[:digit:]]+)"
    "."
    "([[:digit:]]+)"
    "."
    "([[:digit:]]+)");

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACE_OPTIONS
  if (gLpsrOptions->fTraceLilypondVersion) {
    gLogOstream <<
      "There are " << smSize << " matches" <<
      " for version string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl <<
      smSize << " elements: ";

      for (unsigned i = 0; i < smSize; ++i) {
        gLogOstream <<
          "[" << sm [i] << "] ";
      } // for

      gLogOstream << endl;
    }
#endif

  if (smSize == 4) {
    // found an n.x.y specification
    string
      generationNumberValue = sm [1],
      majorNumberValue      = sm [2],
      minorNumberValue      = sm [3];

#ifdef TRACE_OPTIONS
    if (gLpsrOptions->fTraceLilypondVersion) {
      gLogOstream <<
        "--> generationNumberValue = \"" << generationNumberValue << "\", " <<
        "--> majorNumberValue = \"" << majorNumberValue << "\", " <<
        "--> minorNumberValue = \"" << minorNumberValue << "\"" <<
        endl;
    }
#endif

    generationNumber = stoi (generationNumberValue);
    majorNumber      = stoi (majorNumberValue);
    minorNumber      = stoi (minorNumberValue);
  }

  else {
    // decipher theString with a two-number regular expression
    string regularExpression (
      "([[:digit:]]+)"
      "."
      "([[:digit:]]+)");

    regex  e (regularExpression);
    smatch sm;

    regex_match (theString, sm, e);

    unsigned smSize = sm.size ();

#ifdef TRACE_OPTIONS
    if (gLpsrOptions->fTraceLilypondVersion) {
      gLogOstream <<
        "There are " << smSize << " matches" <<
        " for chord details string '" << theString <<
        "' with regex '" << regularExpression <<
        "'" <<
        endl <<
        smSize << " elements: ";

        for (unsigned i = 0; i < smSize; ++i) {
          gLogOstream <<
            "[" << sm [i] << "] ";
        } // for

        gLogOstream << endl;
      }
#endif

    if (smSize == 3) {
      // found an n.x specification
      // assume implicit 0 minor number
      string
        generationNumberValue = sm [1],
        majorNumberValue      = sm [2];

#ifdef TRACE_OPTIONS
      if (gLpsrOptions->fTraceLilypondVersion) {
        gLogOstream <<
          "--> generationNumberValue = \"" << generationNumberValue << "\", " <<
          "--> majorNumberValue = \"" << majorNumberValue << "\", " <<
          endl;
      }
#endif

      generationNumber = stoi (generationNumberValue);
      majorNumber      = stoi (majorNumberValue);
      minorNumber      = 0;
    }

    else {
      stringstream s;

      s <<
        "version number argument '" << theString <<
        "' is ill-formed";

      optionError (s.str ());

      exit (4);
    }
  }
}

bool lpsrOptions::versionNumberGreaterThanOrEqualTo (
  string otherVersionNumber)
{
  bool result = false;

  int
    lilyPondVersionGenerationNumber,
    lilyPondVersionMajorNumber,
    lilyPondVersionMinorNumber;

  crackLilypondVersionNumber (
    fLilyPondVersion,
    lilyPondVersionGenerationNumber,
    lilyPondVersionMajorNumber,
    lilyPondVersionMinorNumber);

  int
    otherVersionNumbeGenerationNumber,
    otherVersionNumbeMajorNumber,
    otherVersionNumbeMinorNumber;

  crackLilypondVersionNumber (
    otherVersionNumber,
    otherVersionNumbeGenerationNumber,
    otherVersionNumbeMajorNumber,
    otherVersionNumbeMinorNumber);

  if (otherVersionNumbeGenerationNumber != 2) {
    gLogOstream <<
      "Using verstion \"" <<
      otherVersionNumbeGenerationNumber << ".x.y\" " <<
      "is probably not such a good idea" <<
      endl;
  }

  if (otherVersionNumbeMajorNumber < 19) {
    gLogOstream <<
      "Using a verstion older than \"" <<
      otherVersionNumbeGenerationNumber << ".19.y\" " <<
      "is not a good idea: the generated LilyPond code uses features introduced in the latter" <<
      endl;
  }

  if (lilyPondVersionGenerationNumber < otherVersionNumbeGenerationNumber) {
    result = false;
  }
  else if (lilyPondVersionGenerationNumber > otherVersionNumbeGenerationNumber) {
    result = true;
  }
  else {
    // same generation number
    if (lilyPondVersionMajorNumber < otherVersionNumbeMajorNumber) {
      result = false;
    }
    else if (lilyPondVersionMajorNumber > otherVersionNumbeMajorNumber) {
      result = true;
    }
    else {
      // same major number
      result =
        lilyPondVersionMinorNumber >= otherVersionNumbeMinorNumber;
    }
  }

  return result;
}

ostream& operator<< (ostream& os, const S_lpsrOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeLpsrOptionsHandling (
  S_oahHandler handler)
{
#ifdef TRACE_OPTIONS
  if (gOahBasicOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    gLogOstream <<
      "Initializing LPSR options handling" <<
      endl;
  }
#endif

  // create the options variables
  // ------------------------------------------------------

  gLpsrOptionsUserChoices = lpsrOptions::create (
    handler);
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
