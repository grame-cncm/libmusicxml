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

#include "generalOah.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "msrOah.h"

#include "messagesHandling.h"


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
                     optionsMsrPitchesLanguageKindVariable)
{
  msrPitchesLanguageAtom* o = new
    msrPitchesLanguageAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      optionsMsrPitchesLanguageKindVariable);
  assert(o!=0);
  return o;
}

msrPitchesLanguageAtom::msrPitchesLanguageAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msrQuarterTonesPitchesLanguageKind&
                     optionsMsrPitchesLanguageKindVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fMsrQuarterTonesPitchesLanguageKindVariable (
      optionsMsrPitchesLanguageKindVariable)
{}

msrPitchesLanguageAtom::~msrPitchesLanguageAtom ()
{}

S_oahValuedAtom msrPitchesLanguageAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a msrPitchesLanguageAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void msrPitchesLanguageAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'msrPitchesLanguageAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the pitches languages map?

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'msrPitchesLanguageAtom'" <<
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
      "MSR pitches language '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gQuarterTonesPitchesLanguageKindsMap.size () <<
      " known MSR pitches languages are:" <<
      endl;

    gIndenter++;

    s <<
      existingQuarterTonesPitchesLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    gIndenter--;

    oahError (s.str ());
  }

  setMsrQuarterTonesPitchesLanguageKindVariable (
    (*it).second);
}

void msrPitchesLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrPitchesLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msrPitchesLanguageAtom>*> (v)) {
        S_msrPitchesLanguageAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msrPitchesLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrPitchesLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrPitchesLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msrPitchesLanguageAtom>*> (v)) {
        S_msrPitchesLanguageAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msrPitchesLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrPitchesLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
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
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msrPitchesLanguageAtom:" <<
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
    "fmsrPitchesLanguageVariable" << " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable) <<
      "\"" <<
    endl;
}

void msrPitchesLanguageAtom::printAtomOptionsValues (
  ostream& os,
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
  map<string, string>& stringStringMapVariable)
{
  msrRenamePartAtom* o = new
    msrRenamePartAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringStringMapVariable);
  assert(o!=0);
  return o;
}

msrRenamePartAtom::msrRenamePartAtom (
  string               shortName,
  string               longName,
  string               description,
  string               valueSpecification,
  string               variableName,
  map<string, string>& stringStringMapVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringStringMapVariable (
      stringStringMapVariable)
{
  fMultipleOccurrencesAllowed = true;
}

msrRenamePartAtom::~msrRenamePartAtom ()
{}

S_oahValuedAtom msrRenamePartAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a msrRenamePartAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void msrRenamePartAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'msrRenamePartAtom'" <<
      endl;
  }
#endif

  // theString contains the part rename specification
  // decipher it to extract the old and new part names

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
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

  unsigned smSize = sm.size ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "There are " << smSize << " matches" <<
      " for part rename string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;
  }
#endif

  if (smSize == 3) {
#ifdef TRACE_OAH
    if (gTraceOah->fTraceOah) {
      for (unsigned i = 0; i < smSize; ++i) {
        os <<
          "[" << sm [i] << "] ";
      } // for
      os << endl;
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "--> oldPartName = \"" << oldPartName << "\", " <<
      "--> newPartName = \"" << newPartName << "\"" <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  map<string, string>::iterator
    it =
      fStringStringMapVariable.find (oldPartName);

  if (it != fStringStringMapVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Part \"" << oldPartName << "\" occurs more that once" <<
      "in the '--rename-part' option";

    oahError (s.str ());
  }

  else {
    fStringStringMapVariable [oldPartName] = newPartName;
  }
}

void msrRenamePartAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrRenamePartAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrRenamePartAtom>*
    p =
      dynamic_cast<visitor<S_msrRenamePartAtom>*> (v)) {
        S_msrRenamePartAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msrRenamePartAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrRenamePartAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrRenamePartAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrRenamePartAtom>*
    p =
      dynamic_cast<visitor<S_msrRenamePartAtom>*> (v)) {
        S_msrRenamePartAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msrRenamePartAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrRenamePartAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
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

  if (! fStringStringMapVariable.size ()) {
    s << "none";
  }
  else {
    map<string, string>::const_iterator
      iBegin = fStringStringMapVariable.begin (),
      iEnd   = fStringStringMapVariable.end (),
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

  if (! fStringStringMapVariable.size ()) {
    s << "none";
  }
  else {
    map<string, string>::const_iterator
      iBegin = fStringStringMapVariable.begin (),
      iEnd   = fStringStringMapVariable.end (),
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
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msrRenamePartAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringStringMapVariable" << " : " <<
    endl;

  if (! fStringStringMapVariable.size ()) {
    os << "none";
  }
  else {
    map<string, string>::const_iterator
      iBegin = fStringStringMapVariable.begin (),
      iEnd   = fStringStringMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  os << endl;
}

void msrRenamePartAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringStringMapVariable.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os << endl;
    gIndenter++;

    map<string, string>::const_iterator
      iBegin = fStringStringMapVariable.begin (),
      iEnd   = fStringStringMapVariable.end (),
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

ostream& operator<< (ostream& os, const S_msrRenamePartAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msrOah gMsrOah;
S_msrOah gMsrOahUserChoices;
S_msrOah gMsrOahWithDetailedTrace;

S_msrOah msrOah::create (
  S_oahHandler handlerUpLink)
{
  msrOah* o = new msrOah(
    handlerUpLink);
  assert(o!=0);
  return o;
}

msrOah::msrOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "MSR",
    "hmsr", "help-msr",
R"(These options control the way MSR data is handled.)",
    kElementVisibilityAlways,
    handlerUpLink)
{
  // append this options group to the options handler
  // if relevant // JMI ???
  if (handlerUpLink) {
    handlerUpLink->
      appendGroupToHandler (this);
  }

  // initialize it
  initializeMsrOah (false);
}

msrOah::~msrOah ()
{}

#ifdef TRACE_OAH
void msrOah::initializeMsrTraceOah (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Trace",
      "hmsrt", "help-msr-trace",
R"()",
    kElementVisibilityAlways,
    this);

  appendSubGroupToGroup (subGroup);

  // MSR

  fTraceMsr          = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tmsr", "trace-msr",
R"(Write a trace of the LPSR graphs visiting activity to standard error.)",
        "traceMsr",
        fTraceMsr));

  // MSR visitors

  fTraceMsrVisitors  = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tmsrv", "trace-msr-visitors",
R"(Write a trace of the MSR graphs visiting activity to standard error.)",
        "traceMsrVisitors",
        fTraceMsrVisitors));
}
#endif

void msrOah::initializeMsrDisplayOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Display",
      "hmsrtd", "help-msr-trace-and-display",
R"()",
    kElementVisibilityAlways,
    this);

  appendSubGroupToGroup (subGroup);

  // partgroups

  fDisplayPartGroups = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dpg", "display-partgroups",
R"(Write the structure of the part groups to standard error.)",
        "displayPartGroups",
        fDisplayPartGroups));

  // MSR

  fDisplayMsr        = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dmsr", "display-msr",
R"(Write the contents of the MSR data to standard error.)",
        "displayMsr",
        fDisplayMsr));

  // MSR details

  fDisplayMsrDetails = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahTwoBooleansAtom::create (
        "dmsrd", "display-msr-details",
R"(Write the contents of the MSR data with more details to standard error.)",
        "displayMsrDetails",
        fDisplayMsrDetails,
        fDisplayMsr));

  // MSR names

  fDisplayMsrNames   = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "names", "display-msr-names",
R"(Only write a view of the names in the MSR to standard error.
This implies that no LilyPond code is generated.)",
        "displayMsrNames",
        fDisplayMsrNames));

  // MSR summary

  fDisplayMsrSummary = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "sum", "display-msr-summary",
R"(Only write a summary of the MSR to standard error.
This implies that no LilyPond code is generated.)",
        "displayMsrSummary",
        fDisplayMsrSummary));
}

void msrOah::initializeMsrLanguagesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Languages",
      "hmsrlang", "help-msr-languages",
R"()",
    kElementVisibilityAlways,
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
      gQuarterTonesPitchesLanguageKindsMap.size () <<
      " known MSR pitches languages are:" <<
      endl;

    gIndenter++;

    s <<
      existingQuarterTonesPitchesLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    gIndenter--;

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
              to_string (gQuarterTonesPitchesLanguageKindsMap.size ())),
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

void msrOah::initializeMsrPartsOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Parts",
      "hmsrp", "help-msr-parts",
R"()",
    kElementVisibilityAlways,
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
          gOahOah->fHandlerExecutableName),
        "PART_RENAME_SPEC",
        "partsRenamingMap",
        fPartsRenamingMap));
}

void msrOah::initializeMsrStavesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Staves",
      "hmsrs", "help-msr-staves",
R"()",
    kElementVisibilityAlways,
    this);

  appendSubGroupToGroup (subGroup);

  // single line staves

  fCreateSingleLineStavesAsRythmic = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "cslsar", "create-single-line-staves-as-rythmic",
R"(Create staves with a single line as rythmic staves.
By default, drum staves are created in this case.)",
        "createSingleLineStavesAsRythmic",
        fCreateSingleLineStavesAsRythmic));
}

void msrOah::initializeMsrVoicesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Voices",
      "hmsrv", "help-msr-voices",
R"()",
    kElementVisibilityAlways,
    this);

  appendSubGroupToGroup (subGroup);

  // staff relative numbers

  fCreateVoicesStaffRelativeNumbers = boolOptionsInitialValue;

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

void msrOah::initializeMsrHarmoniesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Harmonies",
      "hmsrh", "help-msr-harmonies",
R"()",
    kElementVisibilityAlways,
    this);

  appendSubGroupToGroup (subGroup);

  // show harmony voices
  // --------------------------------------

  fShowHarmonyVoices = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "shv", "show-harmony-voices",
R"(Show the parts harmony voices in the MSR data
even though it does not contain music.)",
        "showHarmonyVoices",
        fShowHarmonyVoices));
}

void msrOah::initializeMsrFiguredBassOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Figured bass",
      "hmsrfb", "help-msr-figured-bass",
R"()",
    kElementVisibilityAlways,
    this);

  appendSubGroupToGroup (subGroup);

  // show figured bass voices
  // --------------------------------------

  fShowFiguredBassVoices = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "sfbv", "show-figured-bass-voices",
R"(Show the figured bass harmony voices in the MSR data
even though they do not contain music.)",
        "showFiguredBassVoices",
        fShowFiguredBassVoices));
}

void msrOah::initializeMsrOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // trace
  // --------------------------------------
  initializeMsrTraceOah (
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

  // harmonies
  // --------------------------------------

  initializeMsrHarmoniesOptions (
    boolOptionsInitialValue);

  // figured bass
  // --------------------------------------

  initializeMsrFiguredBassOptions (
    boolOptionsInitialValue);
}

S_msrOah msrOah::createCloneWithDetailedTrace ()
{
  S_msrOah
    clone =
      msrOah::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;

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
bool msrOah::setMsrQuarterTonesPitchesLanguage (string language)
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
void msrOah::enforceQuietness ()
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
void msrOah::checkOptionsConsistency ()
{}

//______________________________________________________________________________
void msrOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrOah>*
    p =
      dynamic_cast<visitor<S_msrOah>*> (v)) {
        S_msrOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msrOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrOah>*
    p =
      dynamic_cast<visitor<S_msrOah>*> (v)) {
        S_msrOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msrOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msrOah::printMsrOahValues (int valueFieldWidth)
{
  gLogOstream <<
    "The MSR options are:" <<
    endl;

  gIndenter++;

  // trace and display
  // --------------------------------------

  gLogOstream <<
    "Trace and display:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
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
    setw (valueFieldWidth) << "displayMsrDetails" << " : " <<
    booleanAsString (fDisplayMsrDetails) <<
    endl <<

    setw (valueFieldWidth) << "displayMsrNames" << " : " <<
    booleanAsString (fDisplayMsrNames) <<
    endl <<

    setw (valueFieldWidth) << "displayMsrSummary" << " : " <<
    booleanAsString (fDisplayMsrSummary) <<
    endl;

  gIndenter--;

  // languages
  // --------------------------------------

  gLogOstream <<
     "Languages:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (valueFieldWidth) << "msrPitchesLanguage" << " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKind) <<
      "\"" <<
    endl;

  gIndenter--;

  // parts
  // --------------------------------------

  gLogOstream <<
     "Parts:" <<
    endl;

  gIndenter++;

  // parts renaming

  gLogOstream << left <<
    setw (valueFieldWidth) << "parts renaming" << " : ";

  if (! fPartsRenamingMap.size ()) {
    gLogOstream <<
      "none";
  }
  else {
    for (
      map<string, string>::const_iterator i =
        fPartsRenamingMap.begin ();
      i != fPartsRenamingMap.end ();
      i++
  ) {
        gLogOstream <<
          "\"" << ((*i).first) << "\" -> \"" << ((*i).second) << "\"";
    } // for
  }

  gLogOstream << endl;

  gIndenter--;

  // staves
  // --------------------------------------

  gLogOstream <<
    "Staves:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (valueFieldWidth) <<
    "createSingleLineStavesAsRythmic" << " : " <<
    booleanAsString (fCreateSingleLineStavesAsRythmic) <<
    endl;

  gIndenter--;

  // voices
  // --------------------------------------

  gLogOstream <<
    "Voices:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (valueFieldWidth) <<
    "createVoicesStaffRelativeNumbers" << " : " <<
    booleanAsString (fCreateVoicesStaffRelativeNumbers) <<
    endl;

  gIndenter--;

  // harmonies
  // --------------------------------------

  gLogOstream <<
    "Harmonies:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (valueFieldWidth) << "showHarmonyVoices" << " : " <<
    booleanAsString (fShowHarmonyVoices) <<
    endl;

  gIndenter--;

  // figured bass
  // --------------------------------------

  gLogOstream <<
    "Figured bass:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (valueFieldWidth) << "showFiguredBassVoices" << " : " <<
    booleanAsString (fShowFiguredBassVoices) <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeMsrOahHandling (
  S_oahHandler handler)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
    gLogOstream <<
      "Initializing MSR options handling" <<
      endl;
  }
#endif

  // MSR options
  // ------------------------------------------------------

  gMsrOahUserChoices = msrOah::create (
    handler);
  assert(gMsrOahUserChoices != 0);

  gMsrOah =
    gMsrOahUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

/* JMI
  gMsrOahWithDetailedTrace =
    gMsrOah->
      createCloneWithDetailedTrace ();
      */
}


}
