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
#include <sstream>

#include <regex>

#include "utilities.h"

#include "generalOptions.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "msrOptions.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_optionsPartRenameItem optionsPartRenameItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsPartRenameItemVariableDisplayName,
  map<string, string>&
                     optionsPartRenameItemVariable)
{
  optionsPartRenameItem* o = new
    optionsPartRenameItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsPartRenameItemVariableDisplayName,
      optionsPartRenameItemVariable);
  assert(o!=0);
  return o;
}

optionsPartRenameItem::optionsPartRenameItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsPartRenameItemVariableDisplayName,
  map<string, string>&
                     optionsPartRenameItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsPartRenameItemVariableDisplayName (
      optionsPartRenameItemVariableDisplayName),
    fOptionsPartRenameItemVariable (
      optionsPartRenameItemVariable)
{}

optionsPartRenameItem::~optionsPartRenameItem ()
{}

void optionsPartRenameItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;

  os <<
    "OptionsPartRenameItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsPartRenameItemVariableDisplayName" << " : " <<
    fOptionsPartRenameItemVariableDisplayName <<
    setw (fieldWidth) <<
    "fOptionsPartRenameItemVariable" << " : " <<
    endl;

  if (! fOptionsPartRenameItemVariable.size ()) {
    os << "none";
  }
  else {
    map<string, string>::const_iterator
      iBegin = fOptionsPartRenameItemVariable.begin (),
      iEnd   = fOptionsPartRenameItemVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  os <<
    endl;
}

void optionsPartRenameItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsPartRenameItemVariableDisplayName <<
    " : ";

  if (! fOptionsPartRenameItemVariable.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os <<
      endl;

    gIndenter++;

    map<string, string>::const_iterator
      iBegin = fOptionsPartRenameItemVariable.begin (),
      iEnd   = fOptionsPartRenameItemVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" <<
        (*i).first <<
        "\" --> \"" <<
        (*i).second <<
        "\"" <<
        endl;
      if (++i == iEnd) break;
    } // for

    gIndenter--;
  }
}

ostream& operator<< (ostream& os, const S_optionsPartRenameItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsPartTransposeItem optionsPartTransposeItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsPartTransposeItemVariableDisplayName,
  map<string, S_msrSemiTonesPitchAndRelativeOctave>&
                     optionsPartTransposeItemVariable)
{
  optionsPartTransposeItem* o = new
    optionsPartTransposeItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsPartTransposeItemVariableDisplayName,
      optionsPartTransposeItemVariable);
  assert(o!=0);
  return o;
}

optionsPartTransposeItem::optionsPartTransposeItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsPartTransposeItemVariableDisplayName,
  map<string, S_msrSemiTonesPitchAndRelativeOctave>&
                     optionsPartTransposeItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsPartTransposeItemVariableDisplayName (
      optionsPartTransposeItemVariableDisplayName),
    fOptionsPartTransposeItemVariable (
      optionsPartTransposeItemVariable)
{}

optionsPartTransposeItem::~optionsPartTransposeItem ()
{}

void optionsPartTransposeItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;

  os <<
    "OptionsPartTransposeItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsPartTransposeItemVariableDisplayName" << " : " <<
    fOptionsPartTransposeItemVariableDisplayName <<
    setw (fieldWidth) <<
    "fOptionsPartTransposeItemVariable" << " : " <<
    endl;

  if (! fOptionsPartTransposeItemVariable.size ()) {
    os << "none";
  }
  else {
    map<string, S_msrSemiTonesPitchAndRelativeOctave>::const_iterator
      iBegin = fOptionsPartTransposeItemVariable.begin (),
      iEnd   = fOptionsPartTransposeItemVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  os <<
    endl;
}

void optionsPartTransposeItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsPartTransposeItemVariableDisplayName <<
    " : ";

  if (! fOptionsPartTransposeItemVariable.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os <<
      endl;

    gIndenter++;

    map<string, S_msrSemiTonesPitchAndRelativeOctave>::const_iterator
      iBegin = fOptionsPartTransposeItemVariable.begin (),
      iEnd   = fOptionsPartTransposeItemVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" <<
        (*i).first <<
        "\" --> \"" <<
        (*i).second <<
        "\"" <<
        endl;
      if (++i == iEnd) break;
    } // for

    gIndenter--;
  }
}

ostream& operator<< (ostream& os, const S_optionsPartTransposeItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsMsrPitchesLanguageItem optionsMsrPitchesLanguageItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsMsrPitchesLanguageKindItemVariableDisplayName,
  msrQuarterTonesPitchesLanguageKind&
                     optionsMsrPitchesLanguageKindItemVariable)
{
  optionsMsrPitchesLanguageItem* o = new
    optionsMsrPitchesLanguageItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsMsrPitchesLanguageKindItemVariableDisplayName,
      optionsMsrPitchesLanguageKindItemVariable);
  assert(o!=0);
  return o;
}

optionsMsrPitchesLanguageItem::optionsMsrPitchesLanguageItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsMsrPitchesLanguageKindItemVariableDisplayName,
  msrQuarterTonesPitchesLanguageKind&
                     optionsMsrPitchesLanguageKindItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsMsrPitchesLanguageKindItemVariableDisplayName (
      optionsMsrPitchesLanguageKindItemVariableDisplayName),
    fOptionsMsrPitchesLanguageKindItemVariable (
      optionsMsrPitchesLanguageKindItemVariable)
{}

optionsMsrPitchesLanguageItem::~optionsMsrPitchesLanguageItem ()
{}

void optionsMsrPitchesLanguageItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;

  os <<
    "OptionsMsrPitchesLanguageItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsMsrPitchesLanguagKindeItemVariableDisplayName" << " : " <<
    fOptionsMsrPitchesLanguageKindItemVariableDisplayName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsMsrPitchesLanguageItemVariable" << " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fOptionsMsrPitchesLanguageKindItemVariable) <<
      "\"" <<
    endl;
}

void optionsMsrPitchesLanguageItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsMsrPitchesLanguageKindItemVariableDisplayName <<
    " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fOptionsMsrPitchesLanguageKindItemVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsMsrPitchesLanguageItem& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msrOptions gMsrOptions;
S_msrOptions gMsrOptionsUserChoices;
S_msrOptions gMsrOptionsWithDetailedTrace;

S_msrOptions msrOptions::create (
  S_optionsHandler optionsHandler)
{
  msrOptions* o = new msrOptions(
    optionsHandler);
  assert(o!=0);
  return o;
}

msrOptions::msrOptions (
  S_optionsHandler optionsHandler)
  : optionsGroup (
    "MSR",
    "hmsr", "help-msr",
R"(These options control the way MSR data is handled.)",
    optionsHandler)
{
  // append this options group to the options handler
  // if relevant
  if (optionsHandler) {
    optionsHandler->
      appendOptionsGroupToHandler (this);
  }

  // initialize it
  initializeMsrOptions (false);
}

msrOptions::~msrOptions ()
{}

#ifdef TRACE_OPTIONS
void msrOptions::initializeMsrTraceOptions (
  bool boolOptionsInitialValue)
{
  // variables

  fTraceMsr          = boolOptionsInitialValue;

  fTraceMsrVisitors  = boolOptionsInitialValue;

  // options

  S_optionsSubGroup traceSubGroup =
    optionsSubGroup::create (
      "Trace",
      "hmsrt", "help-msr-trace",
R"()",
    optionsSubGroup::kAlwaysShowDescription,
    this);

  appendOptionsSubGroup (traceSubGroup);

  traceSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "tmsr", "trace-msr",
R"(Write a trace of the LPSR graphs visiting activity to standard error.)",
        "traceMsr",
        fTraceMsr));

  traceSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "tmsrv", "trace-msr-visitors",
R"(Write a trace of the MSR graphs visiting activity to standard error.)",
        "traceMsrVisitors",
        fTraceMsrVisitors));
}
#endif

void msrOptions::initializeMsrDisplayOptions (
  bool boolOptionsInitialValue)
{
  // variables

  fDisplayPartGroups = boolOptionsInitialValue;

  fDisplayMsr        = boolOptionsInitialValue;
  fDisplayMsrDetails = boolOptionsInitialValue;

  fDisplayMsrNames   = boolOptionsInitialValue;
  fDisplayMsrSummary = boolOptionsInitialValue;

  // options

  S_optionsSubGroup displaySubGroup =
    optionsSubGroup::create (
      "Display",
      "hmsrtd", "help-msr-trace-and-display",
R"()",
    optionsSubGroup::kAlwaysShowDescription,
    this);

  appendOptionsSubGroup (displaySubGroup);

  displaySubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "dpg", "display-partgroups",
R"(Write the structure of the part groups to standard error.)",
        "displayPartGroups",
        fDisplayPartGroups));

  displaySubGroup->
    appendOptionsItem (
      optionsTwoBooleansItem::create (
        "dmsr", "display-msr",
R"(Write the contents of the MSR data to standard error.)",
        "displayMsr",
        fDisplayMsr,
        gGeneralOptions->fTracePasses));

  displaySubGroup->
    appendOptionsItem (
      optionsThreeBooleansItem::create (
        "dmsrd", "display-msr-details",
R"(Write the contents of the MSR data with more details to standard error.)",
        "displayMsrDetails",
        fDisplayMsrDetails,
        fDisplayMsr,
        gGeneralOptions->fTracePasses));

  displaySubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "dmnames", "display-msr-names",
R"(Only write a view of the names in the MSR to standard error.
This implies that no LilyPond code is generated.)",
        "displayMsrNames",
        fDisplayMsrNames));

  displaySubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "dmsum", "display-msr-summary",
R"(Only write a summary of the MSR to standard error.
This implies that no LilyPond code is generated.)",
        "displayMsrSummary",
        fDisplayMsrSummary));
}

void msrOptions::initializeMsrLanguagesOptions (
  bool boolOptionsInitialValue)
{
  // variables

  if (! setMsrQuarterTonesPitchesLanguage ("nederlands")) {
    stringstream s;

    s <<
      "INTERNAL INITIALIZATION ERROR: "
      "MSR pitches language 'nederlands' is unknown" <<
      endl <<
      "The " <<
      gQuarterTonesPitchesLanguageKindsMap.size () <<
      " known MSR pitches languages are:" <<
      endl;

    gIndenter++;

    s <<
      existingQuarterTonesPitchesLanguageKinds ();

    gIndenter--;

    optionError (s.str ());
  }

  const msrQuarterTonesPitchesLanguageKind
    msrQuarterTonesPitchesLanguageKindDefaultValue =
      kNederlands; //LilyPond default value

  // options

  S_optionsSubGroup languagesSubGroup =
    optionsSubGroup::create (
      "Languages",
      "hmsrlang", "help-msr-languages",
R"()",
    optionsSubGroup::kAlwaysShowDescription,
    this);

  appendOptionsSubGroup (languagesSubGroup);

  languagesSubGroup->
    appendOptionsItem (
      optionsMsrPitchesLanguageItem::create (
        "mplang", "msr-pitches-language",
        replaceSubstringInString (
          replaceSubstringInString (
            replaceSubstringInString (
R"(Use LANGUAGE to display note pitches in the MSR logs and text views.
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
        "msrPitchesLanguage",
        fMsrQuarterTonesPitchesLanguageKind));
}

void msrOptions::initializeMsrPartsOptions (
  bool boolOptionsInitialValue)
{
  // variables

  // options

  S_optionsSubGroup partsSubGroup =
    optionsSubGroup::create (
      "Parts",
      "hmsrp", "help-msr-parts",
R"()",
    optionsSubGroup::kAlwaysShowDescription,
    this);

  appendOptionsSubGroup (partsSubGroup);

  partsSubGroup->
    appendOptionsItem (
      optionsPartRenameItem::create (
        "mpr", "msr-part-rename",
        replaceSubstringInString (
R"(Rename part ORIGINAL_NAME to NEW_NAME, for example after displaying
the names in the score or a summary of the latter in a first run with options
'-dmnames, -display-msr-names' or 'dmsum, -display-msr-summary'.
PART_RENAME_SPEC can be:
'ORIGINAL_NAME = NEW_NAME'
or
"ORIGINAL_NAME = NEW_NAME"
The single or double quotes are used to allow spaces in the names
and around the '=' sign, otherwise they can be dispensed with.
Using double quotes allows for shell variables substitutions, as in:
DESSUS="Cor anglais"
EXECUTABLE -msr-part-rename "P1 = ${DESSUS}" .
There can be several occurrences of this option.)",
         "EXECUTABLE",
          gGeneralOptions->fExecutableName),
        "PART_RENAME_SPEC",
        "partRename",
        fPartsRenamingMap));

  partsSubGroup->
    appendOptionsItem (
      optionsPartTransposeItem::create (
        "mpt", "msr-part-transpose",
R"(Transpose part ORIGINAL_NAME using TRANSPOSITION to tranpose in the MSR data.
PART_TRANSPOSITION_SPEC can be:
'ORIGINAL_NAME = TRANSPOSITION'
or
"ORIGINAL_NAME = TRANSPOSITION"
The single or double quotes are used to allow spaces in the names
and around the '=' sign, otherwise they can be dispensed with.
TRANSPOSITION should contain a diatonic pitch, followed if needed
by a sequence of ',' or '\'' octave indications.
Such indications cannot be mixed, and they are relative to c\', i.e. middle C.
For example, 'a', 'f' and 'bes,' can be used respectively
for instruments in 'a', 'f' and B flat respectively.
Using double quotes allows for shell variables substitutions, as in:
SAXOPHONE="bes,"
EXECUTABLE -msr-part-transpose "P1 ${SAXOPHONE}" .
There can be several occurrences of this option.)",
        "PART_TRANSPOSITION_SPEC",
        "partTranspose",
        fPartsTranspositionMap));
}

void msrOptions::initializeMsrStavesOptions (
  bool boolOptionsInitialValue)
{
  // variables

  fCreateVoicesStaffRelativeNumbers = boolOptionsInitialValue;

  // options

  S_optionsSubGroup stavesSubGroup =
    optionsSubGroup::create (
      "Staves",
      "hmsrs", "help-msr-staves",
R"()",
    optionsSubGroup::kAlwaysShowDescription,
    this);

  appendOptionsSubGroup (stavesSubGroup);

  stavesSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "cslsar", "create-single-line-staves-as-rythmic",
R"(Create staves with a single line as rythmic staves.
By default, drum staves are created in this case.)",
        "createSingleLineStavesAsRythmic",
        fCreateSingleLineStavesAsRythmic));
}

void msrOptions::initializeMsrVoicesOptions (
  bool boolOptionsInitialValue)
{
  // variables

  fCreateVoicesStaffRelativeNumbers = boolOptionsInitialValue;

  // options

  S_optionsSubGroup voicesSubGroup =
    optionsSubGroup::create (
      "Voices",
      "hmsrv", "help-msr-voices",
R"()",
    optionsSubGroup::kAlwaysShowDescription,
    this);

  appendOptionsSubGroup (voicesSubGroup);

  voicesSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "cvsrvn", "create-voices-staff-relative-numbers",
R"(Generate voices names with numbers relative to their staff.
By default, the voice numbers found are used,
which may be global to the score.)",
        "createVoicesStaffRelativeNumbers",
        fCreateVoicesStaffRelativeNumbers));
}

void msrOptions::initializeMsrRepeatsOptions (
  bool boolOptionsInitialValue)
{
  // variables

  fCreateImplicitInitialRepeatBarline = boolOptionsInitialValue;

  // options

  S_optionsSubGroup repeatsSubGroup =
    optionsSubGroup::create (
      "Repeats",
      "hmsrr", "help-msr-repeats",
R"()",
    optionsSubGroup::kAlwaysShowDescription,
    this);

  appendOptionsSubGroup (repeatsSubGroup);

  repeatsSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "ciirb", "create-implicit-initial-repeat-barline",
R"(Create an implicit repeat barline at the beginning of the stave
in case there is none, as is usual in scores.
By default, no such barline is added.)",
        "createImplicitInitialRepeatBarline",
        fCreateImplicitInitialRepeatBarline));
}

void msrOptions::initializeMsrNotesOptions (
  bool boolOptionsInitialValue)
{
  // variables

  fDelayRestsDynamics  = boolOptionsInitialValue;
  fDelayRestsWords     = boolOptionsInitialValue; // JMI
  fDelayRestsBeams     = boolOptionsInitialValue; // JMI
  fDelayRestsSlurs     = boolOptionsInitialValue; // JMI
  fDelayRestsLigatures = boolOptionsInitialValue; // JMI
  fDelayRestsPedals    = boolOptionsInitialValue; // JMI
  fDelayRestsSlashes   = boolOptionsInitialValue; // JMI
  fDelayRestsWedges    = boolOptionsInitialValue; // JMI

  // options

  S_optionsSubGroup notesSubGroup =
    optionsSubGroup::create (
      "Notes",
      "hmsrn", "help-msr-notes",
R"()",
    optionsSubGroup::kAlwaysShowDescription,
    this);

  appendOptionsSubGroup (notesSubGroup);

  notesSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "drdyns", "delay-rests-dynamics",
R"()",
        "delayRestsDynamics",
        fDelayRestsDynamics));

  notesSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "drwords", "delay-rests-words",
R"()",
        "delayRestsWords",
        fDelayRestsWords));

  notesSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "drbeams", "delay-rests-beams",
R"()",
        "delayRestsBeams",
        fDelayRestsBeams));

  notesSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "drslurs", "delay-rests-slurs",
R"()",
        "delayRestsSlurs",
        fDelayRestsSlurs));

  notesSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "drligs", "delay-rests-ligatures",
R"(<bracket/> in MusicXML, '\[... \}' in LilyPond)",
        "delayRestsLigatures",
        fDelayRestsLigatures));

  notesSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "drpeds", "delay-rests-pedals",
R"()",
        "delayRestsPedals",
        fDelayRestsPedals));

  notesSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "drslashes", "delay-rests-slashes",
R"('<slash/>' in MusicXML)",
        "delayRestsSlashes",
        fDelayRestsSlashes));

  notesSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "drwedges", "delay-rests-wedges",
R"('<wedge/>' in MusicXML, '<!' in LilyPond)",
        "delayRestsWedges",
        fDelayRestsWedges));
}

void msrOptions::initializeMsrLyricsOptions (
  bool boolOptionsInitialValue)
{
  // variables

  fAddStanzasNumbers  = false;

  // options

  S_optionsSubGroup lyricsSubGroup =
    optionsSubGroup::create (
      "Lyrics",
      "hmsrlyrd", "help-msr-lyrics",
R"()",
    optionsSubGroup::kAlwaysShowDescription,
    this);

  appendOptionsSubGroup (lyricsSubGroup);

  lyricsSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "asn", "add-stanzas-numbers",
R"(Add stanzas numbers to lyrics.)",
        "addStanzasNumbers",
        fAddStanzasNumbers));
}

void msrOptions::initializeMsrHarmoniesOptions (
  bool boolOptionsInitialValue)
{
  // variables

  fShowHarmonyVoices      = boolOptionsInitialValue;

  // options

  S_optionsSubGroup harmoniesSubGroup =
    optionsSubGroup::create (
      "Harmonies",
      "hmsrh", "help-msr-harmonies",
R"()",
    optionsSubGroup::kAlwaysShowDescription,
    this);

  appendOptionsSubGroup (harmoniesSubGroup);

  harmoniesSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "shv", "show-harmony-voices",
R"(Show the parts harmony voices in the MSR data
even though it does not contain music.)",
        "showHarmonyVoices",
        fShowHarmonyVoices));
}

void msrOptions::initializeMsrFiguredBassOptions (
  bool boolOptionsInitialValue)
{
  // variables

  fShowFiguredBassVoices      = boolOptionsInitialValue;

  // options

  S_optionsSubGroup figuredBassSubGroup =
    optionsSubGroup::create (
      "Figured bass",
      "hmsrfb", "help-msr-figured-bass",
R"()",
    optionsSubGroup::kAlwaysShowDescription,
    this);

  appendOptionsSubGroup (figuredBassSubGroup);

  figuredBassSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "sfbv", "show-figured-bass-voices",
R"(Show the figured bass harmony voices in the MSR data
even though they do not contain music.)",
        "showFiguredBassVoices",
        fShowFiguredBassVoices));
}

void msrOptions::initializeMsrExitAfterSomePassesOptions (
  bool boolOptionsInitialValue)
{
  // variables

  fExit2a = boolOptionsInitialValue;
  fExit2b = boolOptionsInitialValue;

  // options

  S_optionsSubGroup
    exitAfterSomePassesSubGroup =
      optionsSubGroup::create (
        "Exit after some passes",
        "hme", "help-msr-exit",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);

  appendOptionsSubGroup (exitAfterSomePassesSubGroup);

  // '-exit-2a' is hidden...
  S_optionsBooleanItem
    exit2aOptionsBooleanItem =
      optionsBooleanItem::create (
        "e2a", "exit-2a",
R"(Exit after pass 2a, i.e. after conversion
of the MusicXML tree to an MSR skeleton.)",
        "exit2a",
        fExit2a);
        /* JMI
  exit2aOptionsBooleanItem->
    setOptionsElementIsHidden ();
    */

  exitAfterSomePassesSubGroup->
    appendOptionsItem (
      exit2aOptionsBooleanItem);

  // '-exit-2b' is hidden...
  S_optionsBooleanItem
    exit2bOptionsBooleanItem =
      optionsBooleanItem::create (
        "e2b", "exit-2b",
R"(Exit after pass 2b, i.e. after conversion
of the MusicXML tree to MSR.)",
        "exit2b",
        fExit2b);
        /* JMI
  exit2bOptionsBooleanItem->
    setOptionsElementIsHidden ();
    */

  exitAfterSomePassesSubGroup->
    appendOptionsItem (
      exit2bOptionsBooleanItem);
}

void msrOptions::initializeMsrOptions (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OPTIONS
  // trace
  // --------------------------------------
  initializeMsrTraceOptions (
    boolOptionsInitialValue);
#endif

  // display
  // --------------------------------------
  initializeMsrDisplayOptions (
    boolOptionsInitialValue);

  // languages
  // --------------------------------------
  initializeMsrLanguagesOptions (
    boolOptionsInitialValue);

  // parts
  // --------------------------------------
  initializeMsrPartsOptions (
    boolOptionsInitialValue);

  // staves
  // --------------------------------------
  initializeMsrStavesOptions (
    boolOptionsInitialValue);

  // voices
  // --------------------------------------
  initializeMsrVoicesOptions (
    boolOptionsInitialValue);

  // repeats
  // --------------------------------------
  initializeMsrRepeatsOptions (
    boolOptionsInitialValue);

  // notes
  // --------------------------------------
  initializeMsrNotesOptions (
    boolOptionsInitialValue);

  // lyrics
  // --------------------------------------
  initializeMsrLyricsOptions (
    boolOptionsInitialValue);

  // harmonies
  // --------------------------------------
  initializeMsrHarmoniesOptions (
    boolOptionsInitialValue);

  // figured bass
  // --------------------------------------
  initializeMsrFiguredBassOptions (
    boolOptionsInitialValue);

  // exit after some passes
  // --------------------------------------
  initializeMsrExitAfterSomePassesOptions (
    boolOptionsInitialValue);
}

S_msrOptions msrOptions::createCloneWithDetailedTrace ()
{
  S_msrOptions
    clone =
      msrOptions::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler uplink
  clone->
    setOptionsHandlerUplink (
      fOptionsHandlerUplink);


  // trace and display
  // --------------------------------------

  clone->fTraceMsr =
    true;

  clone->fTraceMsrVisitors =
    true;

  clone->fDisplayPartGroups =
    true;

  clone->fDisplayMsr =
    fDisplayMsr;
  clone->fDisplayMsrDetails =
    fDisplayMsrDetails;

  clone->fDisplayMsrNames =
    fDisplayMsrNames;
  clone->fDisplayMsrSummary =
    fDisplayMsrSummary;


  // languages
  // --------------------------------------

  clone->fMsrQuarterTonesPitchesLanguageKind =
    fMsrQuarterTonesPitchesLanguageKind;


  // parts
  // --------------------------------------

  clone->fPartsRenamingMap =
    fPartsRenamingMap;


  // staves
  // --------------------------------------

  clone->fCreateSingleLineStavesAsRythmic =
    fCreateSingleLineStavesAsRythmic;


  // voices
  // --------------------------------------

  clone->fCreateVoicesStaffRelativeNumbers =
    fCreateVoicesStaffRelativeNumbers;


  // repeats
  // --------------------------------------

  clone->fCreateImplicitInitialRepeatBarline =
    fCreateImplicitInitialRepeatBarline;


  // notes
  // --------------------------------------

  clone->fDelayRestsDynamics =
    fDelayRestsDynamics;
  clone->fDelayRestsWords =
    fDelayRestsWords;
  clone->fDelayRestsSlurs =
    fDelayRestsSlurs;
  clone->fDelayRestsLigatures =
    fDelayRestsLigatures;
  clone->fDelayRestsPedals =
    fDelayRestsPedals;
  clone->fDelayRestsSlashes =
    fDelayRestsSlashes;
  clone->fDelayRestsWedges =
    fDelayRestsWedges;


  // lyrics
  // --------------------------------------

  clone->fAddStanzasNumbers =
    fAddStanzasNumbers;

  // harmonies
  // --------------------------------------

  clone->fShowHarmonyVoices =
    fShowHarmonyVoices;


  // figured bass
  // --------------------------------------

  clone->fShowFiguredBassVoices =
    fShowFiguredBassVoices;

  return clone;
}

//______________________________________________________________________________
bool msrOptions::setMsrQuarterTonesPitchesLanguage (string language)
{
  // is language in the pitches languages map?
  map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator
    it =
      gQuarterTonesPitchesLanguageKindsMap.find (language);

  if (it == gQuarterTonesPitchesLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    return false;
  }

  fMsrQuarterTonesPitchesLanguageKind = (*it).second;

  return true;
}

//______________________________________________________________________________
void msrOptions::enforceQuietness ()
{
  fTraceMsr = false;
  fTraceMsrVisitors = false;
  fDisplayPartGroups = false;
  fDisplayMsr = false;
  fDisplayMsrDetails = false;
  fDisplayMsrNames = false;
  fDisplayMsrSummary = false;
}

//______________________________________________________________________________
void msrOptions::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void msrOptions::printMsrOptionsValues (int fieldWidth)
{
  gLogIOstream <<
    "The MSR options are:" <<
    endl;

  gIndenter++;


  // trace and display
  // --------------------------------------

  gLogIOstream <<
    "Trace and display:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "traceMsr" << " : " <<
    booleanAsString (fTraceMsr) <<
    endl <<

    setw (fieldWidth) << "traceMsrVisitors" << " : " <<
    booleanAsString (fTraceMsrVisitors) <<
    endl <<

    setw (fieldWidth) << "displayPartGroups" << " : " <<
    booleanAsString (fDisplayPartGroups) <<
    endl <<

    setw (fieldWidth) << "displayMsr" << " : " <<
    booleanAsString (fDisplayMsr) <<
    endl <<
    setw (fieldWidth) << "displayMsrDetails" << " : " <<
    booleanAsString (fDisplayMsrDetails) <<
    endl <<

    setw (fieldWidth) << "displayMsrNames" << " : " <<
    booleanAsString (fDisplayMsrNames) <<
    endl <<

    setw (fieldWidth) << "displayMsrSummary" << " : " <<
    booleanAsString (fDisplayMsrSummary) <<
    endl;

  gIndenter--;


  // languages
  // --------------------------------------

  gLogIOstream <<
     "Languages:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "msrPitchesLanguage" << " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKind) <<
      "\"" <<
    endl;

  gIndenter--;


  // parts
  // --------------------------------------

  gLogIOstream <<
     "Parts:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "parts renaming" << " : ";

  if (! fPartsRenamingMap.size ()) {
    gLogIOstream <<
      "none";
  }

  else {
    for (
      map<string, string>::const_iterator i =
        fPartsRenamingMap.begin ();
      i != fPartsRenamingMap.end ();
      i++
  ) {
        gLogIOstream <<
          "\"" << ((*i).first) << " = " << ((*i).second) << "\" ";
    } // for
  }

  gLogIOstream <<
    endl;

  gIndenter--;


  // staves
  // --------------------------------------

  gLogIOstream <<
    "Staves:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) <<
    "createSingleLineStavesAsRythmic" << " : " <<
    booleanAsString (fCreateSingleLineStavesAsRythmic) <<
    endl;

  gIndenter--;


  // voices
  // --------------------------------------

  gLogIOstream <<
    "Voices:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) <<
    "createVoicesStaffRelativeNumbers" << " : " <<
    booleanAsString (fCreateVoicesStaffRelativeNumbers) <<
    endl;

  gIndenter--;


  // repeats
  // --------------------------------------

  gLogIOstream <<
    "Repeats:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) <<
    "createImplicitInitialRepeatBarline" << " : " <<
    booleanAsString (fCreateImplicitInitialRepeatBarline) <<
    endl;

  gIndenter--;


  // notes
  // --------------------------------------

  gLogIOstream <<
     "Notes:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "delayRestsDynamics" << " : " <<
    booleanAsString (fDelayRestsDynamics) <<
    endl <<

    setw (fieldWidth) << "delayRestsWords" << " : " <<
    booleanAsString (fDelayRestsWords) <<
    endl <<

    setw (fieldWidth) << "delayRestsSlurs" << " : " <<
    booleanAsString (fDelayRestsSlurs) <<
    endl <<

    setw (fieldWidth) << "delayRestsLigatures" << " : " <<
    booleanAsString (fDelayRestsLigatures) <<
    endl <<

    setw (fieldWidth) << "delayRestsPedals" << " : " <<
    booleanAsString (fDelayRestsPedals) <<
    endl <<

    setw (fieldWidth) << "delayRestsSlashes" << " : " <<
    booleanAsString (fDelayRestsSlashes) <<
    endl <<

    setw (fieldWidth) << "delayRestsWedges" << " : " <<
    booleanAsString (fDelayRestsWedges) <<
    endl;

  gIndenter--;


  // lyrics
  // --------------------------------------

  gLogIOstream <<
    "Lyrics:" <<
    endl;

  gIndenter++;

  gLogIOstream <<
    setw (fieldWidth) << "addStanzasNumbers" << " : " <<
    booleanAsString (fAddStanzasNumbers) <<
    endl;

  gIndenter--;


  // harmonies
  // --------------------------------------

  gLogIOstream <<
    "Harmonies:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "showHarmonyVoices" << " : " <<
    booleanAsString (fShowHarmonyVoices) <<
    endl;

  gIndenter--;


  // figured bass
  // --------------------------------------

  gLogIOstream <<
    "Figured bass:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "showFiguredBassVoices" << " : " <<
    booleanAsString (fShowFiguredBassVoices) <<
    endl;

  gIndenter--;

  gIndenter--;


  // exit after some passes
  // --------------------------------------

  gLogIOstream <<
    "Exit after some passes:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "exit2a" << " : " <<
    booleanAsString (fExit2a) <<
    endl <<
    setw (fieldWidth) << "exit2b" << " : " <<
    booleanAsString (fExit2b) <<
    endl;

  gIndenter--;
}

S_optionsItem msrOptions::handleOptionsItem (
  ostream&      os,
  S_optionsItem item)
{
  S_optionsItem result;

  if (
    // part rename item?
    S_optionsPartRenameItem
      partRenameItem =
        dynamic_cast<optionsPartRenameItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsPartRenameItem'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = partRenameItem;
  }

  else if (
    // part transpose item?
    S_optionsPartTransposeItem
      partTransposeItem =
        dynamic_cast<optionsPartTransposeItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsPartTransposeItem'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = partTransposeItem;
  }

  else if (
    // pitches language item?
    S_optionsMsrPitchesLanguageItem
      pitchesLanguageItem =
        dynamic_cast<optionsMsrPitchesLanguageItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsMsrPitchesLanguageItem'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = pitchesLanguageItem;
  }

  return result;
}

void msrOptions::handleOptionsPartRenameItemValue (
  ostream&                os,
  S_optionsPartRenameItem partRenameItem,
  string                  theString)
{
  // theString contains the part rename specification
  // decipher it to extract the old and new part names

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions) {
    os <<
      "==> optionsItem is of type 'optionsPartRenameItem'" <<
      endl;
  }
#endif

  string regularExpression (
    "[[:space:]]*(.*)[[:space:]]*"
    "="
    "[[:space:]]*(.*)[[:space:]]*");

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions) {
    os <<
      "There are " << smSize << " matches" <<
      " for part rename string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;
  }
#endif

  if (smSize) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      for (unsigned i = 0; i < smSize; ++i) {
        os <<
          "[" << sm [i] << "] ";
      } // for
      os <<
        endl;
    }
#endif
  }

  else {
    stringstream s;

    s <<
      "-msrPartRename argument '" << theString <<
      "' is ill-formed";

    optionError (s.str ());

    printSpecificSubGroupHelp (
      os,
      partRenameItem->
        getOptionsSubGroupUplink ());

    exit (4);
  }

  string
    oldPartName = sm [1],
    newPartName = sm [2];

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions) {
    os <<
      "--> oldPartName = \"" << oldPartName << "\", " <<
      "--> newPartName = \"" << newPartName << "\"" <<
      endl;
  }
#endif

  map<string, string>
    partRenameItemVariable =
      partRenameItem->
        getOptionsPartRenameItemVariable ();

  // is this part name in the part renaming map?
  map<string, string>::iterator
    it =
      partRenameItemVariable.find (oldPartName);

  if (it != partRenameItemVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Part \"" << oldPartName << "\" occurs more that one" <<
      "in the '--partName' option";

    optionError (s.str ());
    exit (4);
  }

  else {
    partRenameItem->
      setPartRenameItemVariableValue (
        oldPartName, newPartName);
  }
}

void msrOptions::handleOptionsPartTransposeItemValue (
  ostream&                   os,
  S_optionsPartTransposeItem partTransposeItem,
  string                     theString)
{
  // theString contains the part transpose specification
  // decipher it to extract the old and new part names

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions) {
    os <<
      "==> optionsItem is of type 'S_optionsPartTransposeItem'" <<
      endl;
  }
#endif

/* JMI
  string regularExpression (
    "[[:space:]]*(.*)[[:space:]]*"
    "="
    "[[:space:]]*(.*)[[:space:]]*");

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions) {
    os <<
      "There are " << smSize << " matches" <<
      " for part transpose string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;
  }
#endif

  if (smSize) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      for (unsigned i = 0; i < smSize; ++i) {
        os <<
          "[" << sm [i] << "] ";
      } // for
      os <<
        endl;
    }
#endif
  }

  else {
    stringstream s;

    s <<
      "-msrPartTranspose argument '" << theString <<
      "' is ill-formed";

    optionError (s.str ());

    printSpecificSubGroupHelp (
      os,
      partTransposeItem->
        getOptionsSubGroupUplink ());

    exit (4);
  }

  string
    oldPartName = sm [1],
    newPartName = sm [2];

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions) {
    os <<
      "--> oldPartName = \"" << oldPartName << "\", " <<
      "--> newPartName = \"" << newPartName << "\"" <<
      endl;
  }
#endif

  map<string, string>
    partTransposeItemVariable =
      partTransposeItem->
        getOptionsPartTransposeItemVariable ();

  // is this part name in the part renaming map?
  map<string, string>::iterator
    it =
      partTransposeItemVariable.find (oldPartName);

  if (it != partTransposeItemVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Part \"" << oldPartName << "\" occurs more that one" <<
      "in the '--partName' option";

    optionError (s.str ());
    exit (4);
  }

  else {
    partTransposeItem->
      setPartTransposeItemVariableValue (
        oldPartName, newPartName);
  }
  */
}

void msrOptions::handleOptionsMsrPitchesLanguageItemValue (
  ostream&                        os,
  S_optionsMsrPitchesLanguageItem pitchesLanguageKindItem,
  string                          theString)
{
  // theString contains the language name:
  // is it in the pitches languages map?

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions) {
    os <<
      "==> optionsItem is of type 'optionsMsrPitchesLanguageItem'" <<
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
      "MSR pitches language '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gQuarterTonesPitchesLanguageKindsMap.size () <<
      " known MSR pitches languages are:" <<
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

void msrOptions::handleOptionsItemValue (
  ostream&      os,
  S_optionsItem item,
  string        theString)
{
  if (
    // part rename item?
    S_optionsPartRenameItem
      partRenameItem =
        dynamic_cast<optionsPartRenameItem*>(&(*item))
  ) {
    handleOptionsPartRenameItemValue (
      os,
      partRenameItem,
      theString);
  }

  else if (
    // part transpose item?
    S_optionsPartTransposeItem
      partTransposeItem =
        dynamic_cast<optionsPartTransposeItem*>(&(*item))
  ) {
    handleOptionsPartTransposeItemValue (
      os,
      partTransposeItem,
      theString);
  }

  else if (
    // MSR pitches language item?
    S_optionsMsrPitchesLanguageItem
      pitchesLanguageKindItem =
        dynamic_cast<optionsMsrPitchesLanguageItem*>(&(*item))
    ) {
    handleOptionsMsrPitchesLanguageItemValue (
      os,
      pitchesLanguageKindItem,
      theString);
  }
}

ostream& operator<< (ostream& os, const S_msrOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeMsrOptionsHandling (
  S_optionsHandler optionsHandler)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    gLogIOstream <<
      "Initializing MSR options handling" <<
      endl;
  }
#endif

  // MSR options
  // ------------------------------------------------------

  gMsrOptionsUserChoices = msrOptions::create (
    optionsHandler);
  assert(gMsrOptionsUserChoices != 0);

  gMsrOptions =
    gMsrOptionsUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

  gMsrOptionsWithDetailedTrace =
    gMsrOptions->
      createCloneWithDetailedTrace ();
}


}
