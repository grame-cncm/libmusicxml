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

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"
#include "msrOah.h"

#include "waeMessagesHandling.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrPitchesLanguageAtom msrPitchesLanguageAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msrQuarterTonesPitchesLanguageKind&
                     quarterTonesPitchesLanguageKind)
{
  msrPitchesLanguageAtom* o = new
    msrPitchesLanguageAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      quarterTonesPitchesLanguageKind);
  assert (o != nullptr);
  return o;
}

msrPitchesLanguageAtom::msrPitchesLanguageAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msrQuarterTonesPitchesLanguageKind&
                     quarterTonesPitchesLanguageKind)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fMsrQuarterTonesPitchesLanguageKindVariable (
      quarterTonesPitchesLanguageKind)
{}

msrPitchesLanguageAtom::~msrPitchesLanguageAtom ()
{}

void msrPitchesLanguageAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msrPitchesLanguageAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the pitches languages map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msrPitchesLanguageAtom'" <<
      endl;
  }
#endif

  map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator
    it =
      gGlobalQuarterTonesPitchesLanguageKindsMap.find (
        theString);

  if (it == gGlobalQuarterTonesPitchesLanguageKindsMap.end ()) {
    // no, language is unknown in the map

    stringstream s;

    s <<
      "MSR pitches language '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalQuarterTonesPitchesLanguageKindsMap.size () <<
      " known MSR pitches languages are:" <<
      endl;

    ++gIndenter;

    s <<
      existingQuarterTonesPitchesLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setMsrQuarterTonesPitchesLanguageKindVariable (
    (*it).second);
}

void msrPitchesLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrPitchesLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msrPitchesLanguageAtom>*> (v)) {
        S_msrPitchesLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrPitchesLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrPitchesLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrPitchesLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msrPitchesLanguageAtom>*> (v)) {
        S_msrPitchesLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrPitchesLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrPitchesLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrPitchesLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string msrPitchesLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable);

  return s.str ();
}

string msrPitchesLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable);

  return s.str ();
}

void msrPitchesLanguageAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msrPitchesLanguageAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "fmsrPitchesLanguageVariable" << " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable) <<
      "\"" <<
    endl;

  --gIndenter;
}

void msrPitchesLanguageAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable) <<
    "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_msrPitchesLanguageAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrRenamePartAtom msrRenamePartAtom::create (
  string               shortName,
  string               longName,
  string               description,
  string               valueSpecification,
  string               variableName,
  map<string, string>& stringToStringMapVariable)
{
  msrRenamePartAtom* o = new
    msrRenamePartAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringToStringMapVariable);
  assert (o != nullptr);
  return o;
}

msrRenamePartAtom::msrRenamePartAtom (
  string               shortName,
  string               longName,
  string               description,
  string               valueSpecification,
  string               variableName,
  map<string, string>& stringToStringMapVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringToStringMapVariable (
      stringToStringMapVariable)
{
  fMultipleOccurrencesAllowed = true;
}

msrRenamePartAtom::~msrRenamePartAtom ()
{}

void msrRenamePartAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msrRenamePartAtom'" <<
      endl;
  }
#endif

  // theString contains the part rename specification
  // decipher it to extract the old and new part names

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msrRenamePartAtom'" <<
      endl;
  }
#endif

  string regularExpression (
    "[[:space:]]*([^[:space:]]*)[[:space:]]*"
    "="
    "[[:space:]]*([^[:space:]]*)[[:space:]]*");

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for part rename string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;
  }
#endif

  if (smSize == 3) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      for (unsigned i = 0; i < smSize; ++i) {
        gLogStream <<
          "[" << sm [i] << "] ";
      } // for
      gLogStream << endl;
    }
#endif
  }

  else {
    stringstream s;

    s <<
      "-msrPartRename argument '" << theString <<
      "' is ill-formed";

    oahError (s.str ());
  }

  string
    oldPartName = sm [1],
    newPartName = sm [2];

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "--> oldPartName = \"" << oldPartName << "\", " <<
      "--> newPartName = \"" << newPartName << "\"" <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  map<string, string>::iterator
    it =
      fStringToStringMapVariable.find (oldPartName);

  if (it != fStringToStringMapVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Part \"" << oldPartName << "\" occurs more that once" <<
      "in the '--rename-part' option";

    oahError (s.str ());
  }

  else {
    fStringToStringMapVariable [oldPartName] = newPartName;
  }
}

void msrRenamePartAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrRenamePartAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrRenamePartAtom>*
    p =
      dynamic_cast<visitor<S_msrRenamePartAtom>*> (v)) {
        S_msrRenamePartAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrRenamePartAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrRenamePartAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrRenamePartAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrRenamePartAtom>*
    p =
      dynamic_cast<visitor<S_msrRenamePartAtom>*> (v)) {
        S_msrRenamePartAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrRenamePartAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrRenamePartAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrRenamePartAtom::browseData ()" <<
      endl;
  }
#endif
}

string msrRenamePartAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " ";

  if (! fStringToStringMapVariable.size ()) {
    s << "none";
  }
  else {
    map<string, string>::const_iterator
      iBegin = fStringToStringMapVariable.begin (),
      iEnd   = fStringToStringMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

string msrRenamePartAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " ";

  if (! fStringToStringMapVariable.size ()) {
    s << "none";
  }
  else {
    map<string, string>::const_iterator
      iBegin = fStringToStringMapVariable.begin (),
      iEnd   = fStringToStringMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void msrRenamePartAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msrRenamePartAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringToStringMapVariable" << " : " <<
    endl;

  if (! fStringToStringMapVariable.size ()) {
    os << "none";
  }
  else {
    map<string, string>::const_iterator
      iBegin = fStringToStringMapVariable.begin (),
      iEnd   = fStringToStringMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  os << endl;

  --gIndenter;
}

void msrRenamePartAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringToStringMapVariable.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os << endl;
    ++gIndenter;

    map<string, string>::const_iterator
      iBegin = fStringToStringMapVariable.begin (),
      iEnd   = fStringToStringMapVariable.end (),
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

    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);

    --gIndenter;
  }
}

ostream& operator<< (ostream& os, const S_msrRenamePartAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msrOahGroup gGlobalMsrOahGroup;

S_msrOahGroup msrOahGroup::create ()
{
  msrOahGroup* o = new msrOahGroup ();
  assert (o != nullptr);
  return o;
}

msrOahGroup::msrOahGroup ()
  : oahGroup (
    "MSR",
    "hmsr", "help-msr",
R"(These options control the way MSR data is handled.)",
    kElementVisibilityWhole)
{
  initializeMsrOahGroup ();
}

msrOahGroup::~msrOahGroup ()
{}

#ifdef TRACING_IS_ENABLED
void msrOahGroup::initializeMsrTraceOah ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "MSR Trace",
      "hmsrt", "help-msr-trace",
R"()",
    kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // MSR

  fTraceMsr          = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tmsr", "trace-msr",
R"(Write a trace of the LPSR graphs visiting activity to standard error.)",
        "traceMsr",
        fTraceMsr));

  // MSR visitors

  fTraceMsrVisitors  = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tmsrv", "trace-msr-visitors",
R"(Write a trace of the MSR graphs visiting activity to standard error.)",
        "traceMsrVisitors",
        fTraceMsrVisitors));

  // MSR durations

  fTraceMsrDurations  = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tmsrd", "trace-msr-durations",
R"(Write a trace of the MSR durations handling activity to standard error.)",
        "traceMsrDurations",
        fTraceMsrDurations));
}
#endif

void msrOahGroup::initializeMsrDisplayOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Display",
      "hmsrtd", "help-msr-trace-and-display",
R"()",
    kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // display partgroups

  fDisplayPartGroups = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dpg", "display-partgroups",
R"(Write the structure of the part groups to standard error.)",
        "displayPartGroups",
        fDisplayPartGroups));

  // display MSR skeleton

  fDisplayMsrSkeleton        = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dmsrsk", "display-msr-skeleton",
R"(Write the contents of the MSR skeleton data to standard error.)",
        "displayMsrSkeleton",
        fDisplayMsrSkeleton));

  // display MSR

  fDisplayMsr        = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dmsr", "display-msr",
R"(Write the contents of the MSR data to standard error.)",
        "displayMsr",
        fDisplayMsr));

  // display MSR short

  fDisplayMsrShort        = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dmsrs", "display-msr-short",
R"(Write the contents of the MSR data, short version, to standard error.)",
        "displayMsrShort",
        fDisplayMsrShort));

  // display MSR details

  fDisplayMsrDetails = false;

  subGroup->
    appendAtomToSubGroup (
      oahTwoBooleansAtom::create (
        "dmsrd", "display-msr-details",
R"(Write the contents of the MSR data with more details to standard error.)",
        "displayMsrDetails",
        fDisplayMsrDetails,
        fDisplayMsr));

  // display MSR names

  fDisplayMsrNames   = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dmnames", "display-msr-names",
R"(Only write a view of the names in the MSR to standard error.
This implies that no LilyPond code is generated.)",
        "displayMsrNames",
        fDisplayMsrNames));

  // display MSR summary

  fDisplayMsrSummary = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dmsum", "display-msr-summary",
R"(Only write a summary of the MSR to standard error.
This implies that no LilyPond code is generated.)",
        "displayMsrSummary",
        fDisplayMsrSummary));
}

void msrOahGroup::initializeMsrLanguagesOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Languages",
      "hmsrlang", "help-msr-languages",
R"()",
    kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // notes pitches

  if (! setMsrQuarterTonesPitchesLanguage ("nederlands")) {
    stringstream s;

    s <<
      "INTERNAL INITIALIZATION ERROR: "
      "MSR pitches language 'nederlands' is unknown" <<
      endl <<
      "The " <<
      gGlobalQuarterTonesPitchesLanguageKindsMap.size () <<
      " known MSR pitches languages are:" <<
      endl;

    ++gIndenter;

    s <<
      existingQuarterTonesPitchesLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  const msrQuarterTonesPitchesLanguageKind
    msrQuarterTonesPitchesLanguageKindDefaultValue =
      kNederlands; //LilyPond default value

  subGroup->
    appendAtomToSubGroup (
      msrPitchesLanguageAtom::create (
        "mplang", "msr-pitches-language",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use LANGUAGE to display note pitches in the MSR logs and text views.
The NUMBER MSR pitches languages available are:
PITCHES_LANGUAGES.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (gGlobalQuarterTonesPitchesLanguageKindsMap.size ())),
            regex ("PITCHES_LANGUAGES"),
            gIndenter.indentMultiLineString (
              existingQuarterTonesPitchesLanguageKinds (K_NAMES_LIST_MAX_LENGTH))),
          regex ("DEFAULT_VALUE"),
          msrQuarterTonesPitchesLanguageKindAsString (
            msrQuarterTonesPitchesLanguageKindDefaultValue)),
        "LANGUAGE",
        "msrPitchesLanguage",
        fMsrQuarterTonesPitchesLanguageKind));
}

void msrOahGroup::initializeMsrPartsOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Parts",
      "hmsrp", "help-msr-parts",
R"()",
    kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // MSR rename part

  subGroup->
    appendAtomToSubGroup (
      msrRenamePartAtom::create (
        "mrp", "msr-rename-part",
        regex_replace (
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
EXECUTABLE -msr-rename-part "P1 = ${DESSUS}" .
There can be several occurrences of this option.)",
         regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getHandlerExecutableName ()),
        "PART_RENAME_SPEC",
        "partsRenamingMap",
        fPartsRenamingMap));
}

void msrOahGroup::initializeMsrStavesOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Staves",
      "hmsrs", "help-msr-staves",
R"()",
    kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // single line staves

  fCreateSingleLineStavesAsRythmic = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "cslsar", "create-single-line-staves-as-rythmic",
R"(Create staves with a single line as rythmic staves.
By default, drum staves are created in this case.)",
        "createSingleLineStavesAsRythmic",
        fCreateSingleLineStavesAsRythmic));
}

void msrOahGroup::initializeMsrVoicesOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Voices",
      "hmsrv", "help-msr-voices",
R"()",
    kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // staff relative numbers

  fCreateVoicesStaffRelativeNumbers = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "cvsrvn", "create-voices-staff-relative-numbers",
R"(Generate voices names with numbers relative to their staff.
By default, the voice numbers found are used,
which may be global to the score.)",
        "createVoicesStaffRelativeNumbers",
        fCreateVoicesStaffRelativeNumbers));
}

void msrOahGroup::initializeMsrHarmoniesOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Harmonies",
      "hmsrh", "help-msr-harmonies",
R"()",
    kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // show harmony voices
  // --------------------------------------

  fShowHarmonyVoices = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "shv", "show-harmony-voices",
R"(Show the parts harmony voices in the MSR data
even though it does not contain music.)",
        "showHarmonyVoices",
        fShowHarmonyVoices));
}

void msrOahGroup::initializeMsrFiguredBassOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Figured bass",
      "hmsrfb", "help-msr-figured-bass",
R"()",
    kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // show figured bass voices
  // --------------------------------------

  fShowFiguredBassVoices = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "sfbv", "show-figured-bass-voices",
R"(Show the figured bass voices in the MSR data
even though they do not contain music.)",
        "showFiguredBassVoices",
        fShowFiguredBassVoices));
}

void msrOahGroup::initializeMsrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  initializeMsrTraceOah ();
#endif

  // display
  // --------------------------------------
  initializeMsrDisplayOptions ();

  // languages
  // --------------------------------------
  initializeMsrLanguagesOptions ();

  // parts
  // --------------------------------------

  initializeMsrPartsOptions ();

  // staves
  // --------------------------------------
  initializeMsrStavesOptions ();

  // voices
  // --------------------------------------
  initializeMsrVoicesOptions ();

  // harmonies
  // --------------------------------------

  initializeMsrHarmoniesOptions ();

  // figured bass
  // --------------------------------------

  initializeMsrFiguredBassOptions ();
}

//______________________________________________________________________________
bool msrOahGroup::setMsrQuarterTonesPitchesLanguage (string language)
{
  // is language in the pitches languages map?
  map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator
    it =
      gGlobalQuarterTonesPitchesLanguageKindsMap.find (language);

  if (it == gGlobalQuarterTonesPitchesLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    return false;
  }

  fMsrQuarterTonesPitchesLanguageKind = (*it).second;

  return true;
}

//______________________________________________________________________________
void msrOahGroup::enforceGroupQuietness ()
{
  fTraceMsr = false;
  fTraceMsrVisitors = false;
  fDisplayPartGroups = false;
  fDisplayMsr = false;
  fDisplayMsrShort = false;
  fDisplayMsrDetails = false;
  fDisplayMsrNames = false;
  fDisplayMsrSummary = false;
}

//______________________________________________________________________________
void msrOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void msrOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrOahGroup>*
    p =
      dynamic_cast<visitor<S_msrOahGroup>*> (v)) {
        S_msrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrOahGroup>*
    p =
      dynamic_cast<visitor<S_msrOahGroup>*> (v)) {
        S_msrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msrOahGroup::printMsrOahValues (int valueFieldWidth)
{
  gLogStream <<
    "The MSR options are:" <<
    endl;

  ++gIndenter;

  // trace and display
  // --------------------------------------

  gLogStream <<
    "Trace and display:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "traceMsr" << " : " <<
    booleanAsString (fTraceMsr) <<
    endl <<

    setw (valueFieldWidth) << "traceMsrVisitors" << " : " <<
    booleanAsString (fTraceMsrVisitors) <<
    endl <<

    setw (valueFieldWidth) << "displayPartGroups" << " : " <<
    booleanAsString (fDisplayPartGroups) <<
    endl <<

    setw (valueFieldWidth) << "displayMsr" << " : " <<
    booleanAsString (fDisplayMsr) <<
    endl <<
    setw (valueFieldWidth) << "displayMsrShort" << " : " <<
    booleanAsString (fDisplayMsrShort) <<
    endl <<
    setw (valueFieldWidth) << "displayMsrDetails" << " : " <<
    booleanAsString (fDisplayMsrDetails) <<
    endl <<

    setw (valueFieldWidth) << "displayMsrNames" << " : " <<
    booleanAsString (fDisplayMsrNames) <<
    endl <<

    setw (valueFieldWidth) << "displayMsrSummary" << " : " <<
    booleanAsString (fDisplayMsrSummary) <<
    endl;

  --gIndenter;

  // languages
  // --------------------------------------

  gLogStream <<
     "Languages:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "msrPitchesLanguage" << " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKind) <<
      "\"" <<
    endl;

  --gIndenter;

  // parts
  // --------------------------------------

  gLogStream <<
     "Parts:" <<
    endl;

  ++gIndenter;

  // parts renaming

  gLogStream << left <<
    setw (valueFieldWidth) << "parts renaming" << " : ";

  if (! fPartsRenamingMap.size ()) {
    gLogStream <<
      "none";
  }
  else {
    for (
      map<string, string>::const_iterator i =
        fPartsRenamingMap.begin ();
      i != fPartsRenamingMap.end ();
      ++i
  ) {
        gLogStream <<
          "\"" << ((*i).first) << "\" -> \"" << ((*i).second) << "\"";
    } // for
  }

  gLogStream << endl;

  --gIndenter;

  // staves
  // --------------------------------------

  gLogStream <<
    "Staves:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) <<
    "createSingleLineStavesAsRythmic" << " : " <<
    booleanAsString (fCreateSingleLineStavesAsRythmic) <<
    endl;

  --gIndenter;

  // voices
  // --------------------------------------

  gLogStream <<
    "Voices:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) <<
    "createVoicesStaffRelativeNumbers" << " : " <<
    booleanAsString (fCreateVoicesStaffRelativeNumbers) <<
    endl;

  --gIndenter;

  // harmonies
  // --------------------------------------

  gLogStream <<
    "Harmonies:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "showHarmonyVoices" << " : " <<
    booleanAsString (fShowHarmonyVoices) <<
    endl;

  --gIndenter;

  // figured bass
  // --------------------------------------

  gLogStream <<
    "Figured bass:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "showFiguredBassVoices" << " : " <<
    booleanAsString (fShowFiguredBassVoices) <<
    endl;

  --gIndenter;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msrOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOahGroup createGlobalMsrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global MSR OAH group" <<
    endl;
#endif
#endif

  // protect library against multiple initializations
  if (! gGlobalMsrOahGroup) {
    // create the global options group
    gGlobalMsrOahGroup =
      msrOahGroup::create ();
    assert (gGlobalMsrOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsrOahGroup;
}


}
