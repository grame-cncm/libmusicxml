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

#include "msprOah.h"

#include "messagesHandling.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msprPitchesLanguageAtom msprPitchesLanguageAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msrQuarterTonesPitchesLanguageKind&
                     msprPitchesLanguageKindVariable)
{
  msprPitchesLanguageAtom* o = new
    msprPitchesLanguageAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      msprPitchesLanguageKindVariable);
  assert (o != nullptr);
  return o;
}

msprPitchesLanguageAtom::msprPitchesLanguageAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msrQuarterTonesPitchesLanguageKind&
                     msprPitchesLanguageKindVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fMsrQuarterTonesPitchesLanguageKindVariable (
      msprPitchesLanguageKindVariable)
{}

msprPitchesLanguageAtom::~msprPitchesLanguageAtom ()
{}

void msprPitchesLanguageAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msprPitchesLanguageAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the pitches languages map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msprPitchesLanguageAtom'" <<
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

    gIndenter++;

    s <<
      existingQuarterTonesPitchesLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    gIndenter--;

    oahError (s.str ());
  }

  setMsrQuarterTonesPitchesLanguageKindVariable (
    (*it).second);
}

void msprPitchesLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprPitchesLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msprPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msprPitchesLanguageAtom>*> (v)) {
        S_msprPitchesLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprPitchesLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msprPitchesLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprPitchesLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msprPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msprPitchesLanguageAtom>*> (v)) {
        S_msprPitchesLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprPitchesLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msprPitchesLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprPitchesLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string msprPitchesLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable);

  return s.str ();
}

string msprPitchesLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable);

  return s.str ();
}

void msprPitchesLanguageAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msprPitchesLanguageAtom:" <<
    endl;

  gIndenter++;

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

  gIndenter--;
}

void msprPitchesLanguageAtom::printAtomWithValueOptionsValues (
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

ostream& operator<< (ostream& os, const S_msprPitchesLanguageAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msprChordsLanguageAtom msprChordsLanguageAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msprChordsLanguageKind&
                     msprChordsLanguageKindVariable)
{
  msprChordsLanguageAtom* o = new
    msprChordsLanguageAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      msprChordsLanguageKindVariable);
  assert (o != nullptr);
  return o;
}

msprChordsLanguageAtom::msprChordsLanguageAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msprChordsLanguageKind&
                     msprChordsLanguageKindVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fMsdrChordsLanguageKindVariable (
      msprChordsLanguageKindVariable)
{}

msprChordsLanguageAtom::~msprChordsLanguageAtom ()
{}

void msprChordsLanguageAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msprChordsLanguageAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the chords languages map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msprChordsLanguageAtom'" <<
      endl;
  }
#endif

  map<string, msprChordsLanguageKind>::const_iterator
    it =
      gGlobalMsdrChordsLanguageKindsMap.find (theString);

  if (it == gGlobalMsdrChordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    stringstream s;

    s <<
      "MSDR chords language '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalMsdrChordsLanguageKindsMap.size () - 1 <<
      " known MSDR chords languages apart from the default Ignatzek are:" <<
      endl;

    gIndenter++;

    s <<
      existingMsdrChordsLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    gIndenter--;

    oahError (s.str ());
  }

  setMsdrChordsLanguageKindVariable (
    (*it).second);
}

void msprChordsLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprChordsLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msprChordsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msprChordsLanguageAtom>*> (v)) {
        S_msprChordsLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprChordsLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msprChordsLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprChordsLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msprChordsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msprChordsLanguageAtom>*> (v)) {
        S_msprChordsLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprChordsLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msprChordsLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprChordsLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string msprChordsLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    msprChordsLanguageKindAsString (fMsdrChordsLanguageKindVariable);

  return s.str ();
}

string msprChordsLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    msprChordsLanguageKindAsString (fMsdrChordsLanguageKindVariable);

  return s.str ();
}

void msprChordsLanguageAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msprChordsLanguageAtom:" <<
    endl;

  gIndenter++;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fMsdrChordsLanguageKindVariable" << " : \"" <<
    msprChordsLanguageKindAsString (
      fMsdrChordsLanguageKindVariable) <<
    "\"" <<
    endl;

  gIndenter--;
}

void msprChordsLanguageAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    msprChordsLanguageKindAsString (
      fMsdrChordsLanguageKindVariable) <<
    "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_msprChordsLanguageAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msprOahGroup gGlobalMsprOahGroup;

S_msprOahGroup msprOahGroup::create ()
{
  msprOahGroup* o = new msprOahGroup ();
  assert (o != nullptr);
  return o;
}

msprOahGroup::msprOahGroup ()
  : oahGroup (
    "MSDR",
    "hmspr", "help-mspr",
R"(These options control the way MSDR data is handled.)",
    kElementVisibilityWhole)
{
  initializeMsdrOahGroup ();
}

msprOahGroup::~msprOahGroup ()
{}

#ifdef TRACING_IS_ENABLED
void msprOahGroup::initializeMsdrTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "MSDR Trace",
        "hmsprtrace", "help-mspr-trace",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // trace MSPR

  fTraceMsdr            = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tmspr", "trace-mspr",
R"(Write a trace of the MSDR graphs visiting activity to standard error.)",
        "traceMsdr",
        fTraceMsdr));
}
#endif

void msprOahGroup::initializeMsdrDisplayOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Display",
        "hmsprd", "help-mspr-display",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // display MSDR

  fDisplayMsdr = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dmspr", "display-mspr",
R"(Write the contents of the MSDR data to standard error.)",
        "displayMsdr",
        fDisplayMsdr));

  // display MSDR short

  fDisplayMsdrShort = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dmsprs", "display-mspr-short",
R"(Write the contents of the MSDR data, short version, to standard error.)",
        "displayMsdrShort",
        fDisplayMsdrShort));
}


void msprOahGroup::initializeMsdrLanguagesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Languages",
        "hmsprl", "help-mspr-languages",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // mspr pitches language

  if (! setMsdrQuarterTonesPitchesLanguage ("nederlands")) {
    stringstream s;

    s <<
      "INTERNAL INITIALIZATION ERROR: "
      "MSDR pitches language 'nederlands' is unknown" <<
      endl <<
      "The " <<
      gGlobalQuarterTonesPitchesLanguageKindsMap.size () <<
      " known MSDR pitches languages are:" <<
      endl;

    gIndenter++;

    s <<
      existingQuarterTonesPitchesLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    gIndenter--;

    oahError (s.str ());
  }

  const msrQuarterTonesPitchesLanguageKind
    msrQuarterTonesPitchesLanguageKindDefaultValue =
      fMsdrQuarterTonesPitchesLanguageKind;

  fMsdrQuarterTonesPitchesLanguageKind =
    msrQuarterTonesPitchesLanguageKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      msprPitchesLanguageAtom::create (
        "lppl", "mspr-pitches-language",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use LANGUAGE to display note pitches in the MSDR logs and views,
as well as in the generated LilyPond code.
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
        "msprPitchesLanguage",
        fMsdrQuarterTonesPitchesLanguageKind));

  // mspr chords language

  const msprChordsLanguageKind
    msprChordsLanguageKindDefaultValue =
      k_IgnatzekChords; // LilyPond default

  fMsdrChordsLanguageKind =
    msprChordsLanguageKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      msprChordsLanguageAtom::create (
        "lpcl", "mspr-chords-language",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use LANGUAGE to display chord names, their root and bass notes,
in the MSDR logs and views and the generated LilyPond code.
The NUMBER MSDR chords pitches languages available are:
CHORDS_LANGUAGES.
'ignatzek' is Ignatzek's jazz-like, english naming used by LilyPond by default.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (gGlobalMsdrChordsLanguageKindsMap.size ())),
            regex ("CHORDS_LANGUAGES"),
            gIndenter.indentMultiLineString (
              existingMsdrChordsLanguageKinds (K_NAMES_LIST_MAX_LENGTH))),
          regex ("DEFAULT_VALUE"),
          msprChordsLanguageKindAsString (
            msprChordsLanguageKindDefaultValue)),
        "LANGUAGE",
        "mspr-chords-language",
        fMsdrChordsLanguageKind));
}

void msprOahGroup::initializeMsdrQuitAfterSomePassesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Quit after some passes",
        "hlquit", "help-mspr-quit",
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

void msprOahGroup::initializeMsdrOahGroup ()
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
bool msprOahGroup::setMsdrQuarterTonesPitchesLanguage (string language)
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
bool msprOahGroup::setMsdrChordsLanguage (string language)
{
  // is language in the chords languages map?
  map<string, msprChordsLanguageKind>::const_iterator
    it =
      gGlobalMsdrChordsLanguageKindsMap.find (language);

  if (it == gGlobalMsdrChordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    return false;
  }

  fMsdrChordsLanguageKind = (*it).second;

  return true;
}

//______________________________________________________________________________
void msprOahGroup::enforceGroupQuietness ()
{
#ifdef TRACING_IS_ENABLED
  fTraceMsdr = false;
#endif

  fDisplayMsdr = false;
  fDisplayMsdrShort = false;
}

//______________________________________________________________________________
void msprOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void msprOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msprOahGroup>*
    p =
      dynamic_cast<visitor<S_msprOahGroup>*> (v)) {
        S_msprOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msprOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msprOahGroup>*
    p =
      dynamic_cast<visitor<S_msprOahGroup>*> (v)) {
        S_msprOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msprOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msprOahGroup::printMsdrOahValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The MSDR options are:" <<
    endl;

  gIndenter++;

#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  gLogStream <<
    "Trace:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (fieldWidth) << "traceMsdr" << " : " <<
    booleanAsString (fTraceMsdr) <<
    endl <<

  gIndenter--;
#endif

  // display
  // --------------------------------------
  gLogStream <<
    "Display:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (fieldWidth) << "displayMsdr" << " : " <<
    booleanAsString (fDisplayMsdr) <<
    endl <<
    setw (fieldWidth) << "displayMsdrShort" << " : " <<
    booleanAsString (fDisplayMsdrShort) <<
    endl;

  gIndenter--;

  // languages
  // --------------------------------------

  gLogStream <<
    "Languages:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (fieldWidth) << "msprQuarterTonesPitchesLanguage" << " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsdrQuarterTonesPitchesLanguageKind) <<
    "\"" <<
    endl <<

    setw (fieldWidth) << "msprChordsLanguage" << " : \"" <<
    msprChordsLanguageKindAsString (
      fMsdrChordsLanguageKind) <<
    "\"" <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msprOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msprOahGroup createGlobalMsdrOahGroup ()
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
      msprOahGroup::create ();
    assert (gGlobalMsdrOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsdrOahGroup;
}


}


/* JMI
//______________________________________________________________________________
S_msprDalSegnoAtom msprDalSegnoAtom::create (
  string              shortName,
  string              longName,
  string              description,
  string              valueSpecification,
  string              variableName,
  map<string, msrDalSegno::msrDalSegnoKind>&
                      stringToDalSegnoKindMapVariable)
{
  msprDalSegnoAtom* o = new
    msprDalSegnoAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringToDalSegnoKindMapVariable);
  assert (o != nullptr);
  return o;
}

msprDalSegnoAtom::msprDalSegnoAtom (
  string              shortName,
  string              longName,
  string              description,
  string              valueSpecification,
  string              variableName,
  map<string, msrDalSegno::msrDalSegnoKind>&
                      stringToDalSegnoKindMapVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringToDalSegnoKindMapVariable (
      stringToDalSegnoKindMapVariable)
{
  fMultipleOccurrencesAllowed = true;
}

msprDalSegnoAtom::~msprDalSegnoAtom ()
{}

void msprDalSegnoAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msprDalSegnoAtom'" <<
      endl;
  }
#endif

  // theString contains the dal segno specification

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msprDalSegnoAtom'" <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "--> theString = \"" << theString << "\", " <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  map<string, msrDalSegno::msrDalSegnoKind>::iterator
    it =
      fStringToDalSegnoKindMapVariable.find (theString);

  if (it != fStringToDalSegnoKindMapVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Dal segno \"" << theString << "\" occurs more that once" <<
      "in a '--dal-segno' option";

    oahError (s.str ());
  }

  else {
    fStringToDalSegnoKindMapVariable [theString] = msrDalSegno::kDalSegno;
  }
}

void msprDalSegnoAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprDalSegnoAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msprDalSegnoAtom>*
    p =
      dynamic_cast<visitor<S_msprDalSegnoAtom>*> (v)) {
        S_msprDalSegnoAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprDalSegnoAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msprDalSegnoAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprDalSegnoAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msprDalSegnoAtom>*
    p =
      dynamic_cast<visitor<S_msprDalSegnoAtom>*> (v)) {
        S_msprDalSegnoAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprDalSegnoAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msprDalSegnoAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprDalSegnoAtom::browseData ()" <<
      endl;
  }
#endif
}

string msprDalSegnoAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " ";

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    s << "none";
  }
  else {
    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

string msprDalSegnoAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " ";

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    s << "none";
  }
  else {
    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void msprDalSegnoAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msprDalSegnoAtom:" <<
    endl;

  gIndenter++;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringToDalSegnoKindMapVariable" << " : " <<
    endl;

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    os << "none";
  }
  else {
    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  gIndenter--;

  os << endl;
}

void msprDalSegnoAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os << endl;
    gIndenter++;

    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
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

    gIndenter--;
  }
}

ostream& operator<< (ostream& os, const S_msprDalSegnoAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msprDalSegnoAlFineAtom msprDalSegnoAlFineAtom::create (
  string              shortName,
  string              longName,
  string              description,
  string              valueSpecification,
  string              variableName,
  map<string, msrDalSegno::msrDalSegnoKind>&
                      stringToDalSegnoKindMapVariable)
{
  msprDalSegnoAlFineAtom* o = new
    msprDalSegnoAlFineAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringToDalSegnoKindMapVariable);
  assert (o != nullptr);
  return o;
}

msprDalSegnoAlFineAtom::msprDalSegnoAlFineAtom (
  string              shortName,
  string              longName,
  string              description,
  string              valueSpecification,
  string              variableName,
  map<string, msrDalSegno::msrDalSegnoKind>&
                      stringToDalSegnoKindMapVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringToDalSegnoKindMapVariable (
      stringToDalSegnoKindMapVariable)
{
  fMultipleOccurrencesAllowed = true;
}

msprDalSegnoAlFineAtom::~msprDalSegnoAlFineAtom ()
{}

void msprDalSegnoAlFineAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msprDalSegnoAlFineAtom'" <<
      endl;
  }
#endif

  // theString contains the dal segno al fine specification

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msprDalSegnoAlFineAtom'" <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "--> theString = \"" << theString << "\", " <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  map<string, msrDalSegno::msrDalSegnoKind>::iterator
    it =
      fStringToDalSegnoKindMapVariable.find (theString);

  if (it != fStringToDalSegnoKindMapVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Dal segno \"" << theString << "\" occurs more that once" <<
      "in a '--dal-segno-al-fine' option";

    oahError (s.str ());
  }

  else {
    fStringToDalSegnoKindMapVariable [theString] = msrDalSegno::kDalSegno;
  }
}

void msprDalSegnoAlFineAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprDalSegnoAlFineAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msprDalSegnoAlFineAtom>*
    p =
      dynamic_cast<visitor<S_msprDalSegnoAlFineAtom>*> (v)) {
        S_msprDalSegnoAlFineAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprDalSegnoAlFineAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msprDalSegnoAlFineAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprDalSegnoAlFineAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msprDalSegnoAlFineAtom>*
    p =
      dynamic_cast<visitor<S_msprDalSegnoAlFineAtom>*> (v)) {
        S_msprDalSegnoAlFineAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprDalSegnoAlFineAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msprDalSegnoAlFineAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprDalSegnoAlFineAtom::browseData ()" <<
      endl;
  }
#endif
}

string msprDalSegnoAlFineAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " ";

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    s << "none";
  }
  else {
    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

string msprDalSegnoAlFineAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " ";

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    s << "none";
  }
  else {
    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void msprDalSegnoAlFineAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msprDalSegnoAlFineAtom:" <<
    endl;

  gIndenter++;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringToDalSegnoKindMapVariable" << " : " <<
    endl;

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    os << "none";
  }
  else {
    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  gIndenter--;

  os << endl;
}

void msprDalSegnoAlFineAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os << endl;
    gIndenter++;

    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
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

    gIndenter--;
  }
}

ostream& operator<< (ostream& os, const S_msprDalSegnoAlFineAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msprDalSegnoAlCodaAtom msprDalSegnoAlCodaAtom::create (
  string              shortName,
  string              longName,
  string              description,
  string              valueSpecification,
  string              variableName,
  map<string, msrDalSegno::msrDalSegnoKind>&
                      stringToDalSegnoKindMapVariable)
{
  msprDalSegnoAlCodaAtom* o = new
    msprDalSegnoAlCodaAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringToDalSegnoKindMapVariable);
  assert (o != nullptr);
  return o;
}

msprDalSegnoAlCodaAtom::msprDalSegnoAlCodaAtom (
  string              shortName,
  string              longName,
  string              description,
  string              valueSpecification,
  string              variableName,
  map<string, msrDalSegno::msrDalSegnoKind>&
                      stringToDalSegnoKindMapVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringToDalSegnoKindMapVariable (
      stringToDalSegnoKindMapVariable)
{
  fMultipleOccurrencesAllowed = true;
}

msprDalSegnoAlCodaAtom::~msprDalSegnoAlCodaAtom ()
{}

void msprDalSegnoAlCodaAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msprDalSegnoAlCodaAtom'" <<
      endl;
  }
#endif

  // theString contains the dal segno specification
  // decipher it to extract the old and new part names

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msprDalSegnoAlCodaAtom'" <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "--> theString = \"" << theString << "\", " <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  map<string, msrDalSegno::msrDalSegnoKind>::iterator
    it =
      fStringToDalSegnoKindMapVariable.find (theString);

  if (it != fStringToDalSegnoKindMapVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Dal segno al coda value \"" << theString << "\" occurs more that once" <<
      "in a '--dal-segno' option";

    oahError (s.str ());
  }

  else {
    fStringToDalSegnoKindMapVariable [theString] = msrDalSegno::kDalSegno;
  }
}

void msprDalSegnoAlCodaAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprDalSegnoAlCodaAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msprDalSegnoAlCodaAtom>*
    p =
      dynamic_cast<visitor<S_msprDalSegnoAlCodaAtom>*> (v)) {
        S_msprDalSegnoAlCodaAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprDalSegnoAlCodaAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msprDalSegnoAlCodaAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprDalSegnoAlCodaAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msprDalSegnoAlCodaAtom>*
    p =
      dynamic_cast<visitor<S_msprDalSegnoAlCodaAtom>*> (v)) {
        S_msprDalSegnoAlCodaAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msprDalSegnoAlCodaAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msprDalSegnoAlCodaAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msprDalSegnoAlCodaAtom::browseData ()" <<
      endl;
  }
#endif
}

string msprDalSegnoAlCodaAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " ";

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    s << "none";
  }
  else {
    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

string msprDalSegnoAlCodaAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " ";

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    s << "none";
  }
  else {
    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void msprDalSegnoAlCodaAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msprDalSegnoAlCodaAtom:" <<
    endl;

  gIndenter++;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringToDalSegnoKindMapVariable" << " : " <<
    endl;

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    os << "none";
  }
  else {
    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  gIndenter--;

  os << endl;
}

void msprDalSegnoAlCodaAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os << endl;
    gIndenter++;

    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
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

    gIndenter--;
  }
}

ostream& operator<< (ostream& os, const S_msprDalSegnoAlCodaAtom& elt)
{
  elt->print (os);
  return os;
}
*/

