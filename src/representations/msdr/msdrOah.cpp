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

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "msdrOah.h"

#include "waeMessagesHandling.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msdrKeywordsLanguageAtom msdrKeywordsLanguageAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msdrKeywordsLanguageKind&
                     msdrKeywordsLanguageKindVariable)
{
  msdrKeywordsLanguageAtom* o = new
    msdrKeywordsLanguageAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      msdrKeywordsLanguageKindVariable);
  assert (o != nullptr);
  return o;
}

msdrKeywordsLanguageAtom::msdrKeywordsLanguageAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msdrKeywordsLanguageKind&
                     msdrKeywordsLanguageKindVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fMsdrKeywordsLanguageKindVariable (
      msdrKeywordsLanguageKindVariable)
{}

msdrKeywordsLanguageAtom::~msdrKeywordsLanguageAtom ()
{}

void msdrKeywordsLanguageAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdrKeywordsLanguageAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the keywords languages map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdrKeywordsLanguageAtom'" <<
      endl;
  }
#endif

  map<string, msdrKeywordsLanguageKind>::const_iterator
    it =
      gGlobalMsdrKeywordsLanguageKindsMap.find (theString);

  if (it == gGlobalMsdrKeywordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    stringstream s;

    s <<
      "MSDR keywords language '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalMsdrKeywordsLanguageKindsMap.size () - 1 <<
      " known MSDR keywords languages apart from the default Ignatzek are:" <<
      endl;

    ++gIndenter;

    s <<
      existingMsdrKeywordsLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setMsdrKeywordsLanguageKindVariable (
    (*it).second);
}

void msdrKeywordsLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdrKeywordsLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdrKeywordsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdrKeywordsLanguageAtom>*> (v)) {
        S_msdrKeywordsLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdrKeywordsLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdrKeywordsLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdrKeywordsLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdrKeywordsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdrKeywordsLanguageAtom>*> (v)) {
        S_msdrKeywordsLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdrKeywordsLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdrKeywordsLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdrKeywordsLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string msdrKeywordsLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    msdrKeywordsLanguageKindAsString (fMsdrKeywordsLanguageKindVariable);

  return s.str ();
}

string msdrKeywordsLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    msdrKeywordsLanguageKindAsString (fMsdrKeywordsLanguageKindVariable);

  return s.str ();
}

void msdrKeywordsLanguageAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msdrKeywordsLanguageAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fMsdrKeywordsLanguageKindVariable" << " : \"" <<
    msdrKeywordsLanguageKindAsString (
      fMsdrKeywordsLanguageKindVariable) <<
    "\"" <<
    endl;

  --gIndenter;
}

void msdrKeywordsLanguageAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    msdrKeywordsLanguageKindAsString (
      fMsdrKeywordsLanguageKindVariable) <<
    "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_msdrKeywordsLanguageAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msdrPitchesLanguageAtom msdrPitchesLanguageAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msrQuarterTonesPitchesLanguageKind&
                     msdrPitchesLanguageKindVariable)
{
  msdrPitchesLanguageAtom* o = new
    msdrPitchesLanguageAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      msdrPitchesLanguageKindVariable);
  assert (o != nullptr);
  return o;
}

msdrPitchesLanguageAtom::msdrPitchesLanguageAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msrQuarterTonesPitchesLanguageKind&
                     msdrPitchesLanguageKindVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fMsrQuarterTonesPitchesLanguageKindVariable (
      msdrPitchesLanguageKindVariable)
{}

msdrPitchesLanguageAtom::~msdrPitchesLanguageAtom ()
{}

void msdrPitchesLanguageAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdrPitchesLanguageAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the pitches languages map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdrPitchesLanguageAtom'" <<
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
      "MSDR pitches language '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalQuarterTonesPitchesLanguageKindsMap.size () <<
      " known MSDR pitches languages are:" <<
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

void msdrPitchesLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdrPitchesLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdrPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdrPitchesLanguageAtom>*> (v)) {
        S_msdrPitchesLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdrPitchesLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdrPitchesLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdrPitchesLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdrPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdrPitchesLanguageAtom>*> (v)) {
        S_msdrPitchesLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdrPitchesLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdrPitchesLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdrPitchesLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string msdrPitchesLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable);

  return s.str ();
}

string msdrPitchesLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable);

  return s.str ();
}

void msdrPitchesLanguageAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msdrPitchesLanguageAtom:" <<
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
    "fOptionsMsdrPitchesLanguageVariable" << " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable) <<
    "\"" <<
    endl;

  --gIndenter;
}

void msdrPitchesLanguageAtom::printAtomWithValueOptionsValues (
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

ostream& operator<< (ostream& os, const S_msdrPitchesLanguageAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msdrOahGroup gGlobalMsdrOahGroup;

S_msdrOahGroup msdrOahGroup::create ()
{
  msdrOahGroup* o = new msdrOahGroup ();
  assert (o != nullptr);
  return o;
}

msdrOahGroup::msdrOahGroup ()
  : oahGroup (
    "MSDR",
    "hmsdr", "help-msdr",
R"(These options control the way MSDR data is handled.)",
    kElementVisibilityWhole)
{
  initializeMsdrOahGroup ();
}

msdrOahGroup::~msdrOahGroup ()
{}

#ifdef TRACING_IS_ENABLED
void msdrOahGroup::initializeMsdrTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "MSDR Trace",
        "hmsdrtrace", "help-msdr-trace",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // trace MSDR

  fTraceMsdr            = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tmsdr", "trace-msdr",
R"(Write a trace of the MSDR graphs visiting activity to standard error.)",
        "traceMsdr",
        fTraceMsdr));
}
#endif

void msdrOahGroup::initializeMsdrDisplayOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Display",
        "hmsdrd", "help-msdr-display",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // display MSDR

  fDisplayMsdr = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dmsdr", "display-msdr",
R"(Write the contents of the MSDR data to standard error.)",
        "displayMsdr",
        fDisplayMsdr));

  // display MSDR short

  fDisplayMsdrShort = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dmsdrs", "display-msdr-short",
R"(Write the contents of the MSDR data, short version, to standard error.)",
        "displayMsdrShort",
        fDisplayMsdrShort));
}


void msdrOahGroup::initializeMsdrLanguagesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Languages",
        "hmsdrl", "help-msdr-languages",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // msdr pitches language

  if (! setMsdrQuarterTonesPitchesLanguage ("english")) {
    stringstream s;

    s <<
      "INTERNAL INITIALIZATION ERROR: "
      "MSDR pitches language 'nederlands' is unknown" <<
      endl <<
      "The " <<
      gGlobalQuarterTonesPitchesLanguageKindsMap.size () <<
      " known MSDR pitches languages are:" <<
      endl;

    ++gIndenter;

    s <<
      existingQuarterTonesPitchesLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  const msrQuarterTonesPitchesLanguageKind
    msrQuarterTonesPitchesLanguageKindDefaultValue =
      fMsdrQuarterTonesPitchesLanguageKind;

  fMsdrQuarterTonesPitchesLanguageKind =
    msrQuarterTonesPitchesLanguageKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      msdrPitchesLanguageAtom::create (
        "msdpl", "msdr-pitches-language",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use LANGUAGE to input note pitches.
The NUMBER MSDR pitches languages available are:
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
        "msdrPitchesLanguage",
        fMsdrQuarterTonesPitchesLanguageKind));

  // msdr keywords language

  const msdrKeywordsLanguageKind
    msdrKeywordsLanguageKindDefaultValue =
      kKeywordsEnglish; // MSDL default

  fMsdrKeywordsLanguageKind =
    msdrKeywordsLanguageKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      msdrKeywordsLanguageAtom::create (
        "msdkl", "msdr-keywords-language",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use LANGUAGE to input keyword names.
The NUMBER MSDR keywords pitches languages available are:
KEYWORDS_LANGUAGES.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (gGlobalMsdrKeywordsLanguageKindsMap.size ())),
            regex ("KEYWORDS_LANGUAGES"),
            gIndenter.indentMultiLineString (
              existingMsdrKeywordsLanguageKinds (K_NAMES_LIST_MAX_LENGTH))),
          regex ("DEFAULT_VALUE"),
          msdrKeywordsLanguageKindAsString (
            msdrKeywordsLanguageKindDefaultValue)),
        "LANGUAGE",
        "msdr-keywords-language",
        fMsdrKeywordsLanguageKind));
}

void msdrOahGroup::initializeMsdrQuitAfterSomePassesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Quit after some passes",
        "hlquit", "help-msdr-quit",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // quit after pass 3

  fQuitAfterPass3 = false;

  S_oahBooleanAtom
    quit3OahBooleanAtom =
      oahBooleanAtom::create (
        "q3", "quitAfterPass-3",
R"(Quit after pass 3, i.e. after conversion
of the MSR to MSDR.)",
        "quitAfterPass3",
        fQuitAfterPass3);

  subGroup->
    appendAtomToSubGroup (
      quit3OahBooleanAtom);
}

void msdrOahGroup::initializeMsdrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace and display
  // --------------------------------------
  initializeMsdrTraceOah ();
#endif

  // display
  // --------------------------------------
  initializeMsdrDisplayOptions ();

  // languages
  // --------------------------------------
  initializeMsdrLanguagesOptions ();

  // quit after some passes
  // --------------------------------------
  initializeMsdrQuitAfterSomePassesOptions ();
}

//______________________________________________________________________________
bool msdrOahGroup::setMsdrKeywordsLanguage (string language)
{
  // is language in the keywords languages map?
  map<string, msdrKeywordsLanguageKind>::const_iterator
    it =
      gGlobalMsdrKeywordsLanguageKindsMap.find (language);

  if (it == gGlobalMsdrKeywordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    return false;
  }

  fMsdrKeywordsLanguageKind = (*it).second;

  return true;
}

//______________________________________________________________________________
bool msdrOahGroup::setMsdrQuarterTonesPitchesLanguage (string language)
{
  // is language in the note names languages map?
  map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator
    it =
      gGlobalQuarterTonesPitchesLanguageKindsMap.find (language);

  if (it == gGlobalQuarterTonesPitchesLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    return false;
  }

  fMsdrQuarterTonesPitchesLanguageKind = (*it).second;

  return true;
}

//______________________________________________________________________________
void msdrOahGroup::enforceGroupQuietness ()
{
#ifdef TRACING_IS_ENABLED
  fTraceMsdr = false;
#endif

  fDisplayMsdr = false;
  fDisplayMsdrShort = false;
}

//______________________________________________________________________________
void msdrOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void msdrOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdrOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdrOahGroup>*
    p =
      dynamic_cast<visitor<S_msdrOahGroup>*> (v)) {
        S_msdrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdrOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdrOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdrOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdrOahGroup>*
    p =
      dynamic_cast<visitor<S_msdrOahGroup>*> (v)) {
        S_msdrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdrOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdrOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdrOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msdrOahGroup::printMsdrOahValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The MSDR options are:" <<
    endl;

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  gLogStream <<
    "Trace:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "traceMsdr" << " : " <<
    booleanAsString (fTraceMsdr) <<
    endl <<

  --gIndenter;
#endif

  // display
  // --------------------------------------
  gLogStream <<
    "Display:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "displayMsdr" << " : " <<
    booleanAsString (fDisplayMsdr) <<
    endl <<
    setw (fieldWidth) << "displayMsdrShort" << " : " <<
    booleanAsString (fDisplayMsdrShort) <<
    endl;

  --gIndenter;

  // languages
  // --------------------------------------

  gLogStream <<
    "Languages:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "msdrQuarterTonesPitchesLanguage" << " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsdrQuarterTonesPitchesLanguageKind) <<
    "\"" <<
    endl <<

    setw (fieldWidth) << "msdrKeywordsLanguage" << " : \"" <<
    msdrKeywordsLanguageKindAsString (
      fMsdrKeywordsLanguageKind) <<
    "\"" <<
    endl;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msdrOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msdrOahGroup createGlobalMsdrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global MSDR OAH group" <<
    endl;
#endif
#endif

  // protect library against multiple initializations
  if (! gGlobalMsdrOahGroup) {
    // create the global options group
    gGlobalMsdrOahGroup =
      msdrOahGroup::create ();
    assert (gGlobalMsdrOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsdrOahGroup;
}


}
