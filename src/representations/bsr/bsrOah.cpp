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

#include <regex>

#include "utilities.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "bsrOah.h"

#include "waeMessagesHandling.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
string bsrFacSimileKindAsString (
  bsrFacSimileKind facSimileKind)
{
  string result;

  switch (facSimileKind) {
    case kFacSimileYes:
      result = "facSimileYes";
      break;
    case kFacSimileNo:
      result = "facSimileNo";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
S_bsrFacSimileKindAtom bsrFacSimileKindAtom::create (
  string           shortName,
  string           longName,
  string           description,
  string           valueSpecification,
  string           variableName,
  bsrFacSimileKind bsrFacSimileKindVariable)
{
  bsrFacSimileKindAtom* o = new
    bsrFacSimileKindAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      bsrFacSimileKindVariable);
  assert (o != nullptr);
  return o;
}

bsrFacSimileKindAtom::bsrFacSimileKindAtom (
  string           shortName,
  string           longName,
  string           description,
  string           valueSpecification,
  string           variableName,
  bsrFacSimileKind bsrFacSimileKindVariable)
  : oahAtomStoringAValueInAVariable (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fBsrFacSimileKindVariable (
      bsrFacSimileKindVariable)
{}

bsrFacSimileKindAtom::~bsrFacSimileKindAtom ()
{}

void bsrFacSimileKindAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
  // JMI ???

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'bsrFacSimileKindAtom'" <<
      endl;
  }
#endif
}

void bsrFacSimileKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> bsrFacSimileKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrFacSimileKindAtom>*
    p =
      dynamic_cast<visitor<S_bsrFacSimileKindAtom>*> (v)) {
        S_bsrFacSimileKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching bsrFacSimileKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrFacSimileKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> bsrFacSimileKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrFacSimileKindAtom>*
    p =
      dynamic_cast<visitor<S_bsrFacSimileKindAtom>*> (v)) {
        S_bsrFacSimileKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching bsrFacSimileKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrFacSimileKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> bsrFacSimileKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string bsrFacSimileKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    bsrFacSimileKindAsString (fBsrFacSimileKindVariable);

  return s.str ();
}

string bsrFacSimileKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    bsrFacSimileKindAsString (fBsrFacSimileKindVariable);

  return s.str ();
}

void bsrFacSimileKindAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "bsrFacSimileKindAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "variableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "bsrFacSimileKindVariable" << " : \"" <<
    bsrFacSimileKindAsString (
      fBsrFacSimileKindVariable) <<
      "\"" <<
    endl;

  --gIndenter;
}

void bsrFacSimileKindAtom::printAtomWithVariableOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    bsrFacSimileKindAsString (
      fBsrFacSimileKindVariable) <<
    "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_bsrFacSimileKindAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_bsrTextsLanguageAtom bsrTextsLanguageAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  bsrTextsLanguageKind&
                     bsrTextsLanguageKindVariable)
{
  bsrTextsLanguageAtom* o = new
    bsrTextsLanguageAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      bsrTextsLanguageKindVariable);
  assert (o != nullptr);
  return o;
}

bsrTextsLanguageAtom::bsrTextsLanguageAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  bsrTextsLanguageKind&
                     bsrTextsLanguageKindVariable)
  : oahAtomStoringAValueInAVariable (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fBsrTextsLanguageKindVariable (
      bsrTextsLanguageKindVariable)
{}

bsrTextsLanguageAtom::~bsrTextsLanguageAtom ()
{}

void bsrTextsLanguageAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
  // JMI ???

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'bsrTextsLanguageAtom'" <<
      endl;
  }
#endif
}

void bsrTextsLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> bsrTextsLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrTextsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_bsrTextsLanguageAtom>*> (v)) {
        S_bsrTextsLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching bsrTextsLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrTextsLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> bsrTextsLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrTextsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_bsrTextsLanguageAtom>*> (v)) {
        S_bsrTextsLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching bsrTextsLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrTextsLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> bsrTextsLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string bsrTextsLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    bsrTextsLanguageKindAsString (fBsrTextsLanguageKindVariable);

  return s.str ();
}

string bsrTextsLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    bsrTextsLanguageKindAsString (fBsrTextsLanguageKindVariable);

  return s.str ();
}

void bsrTextsLanguageAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "bsrTextsLanguageAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "variableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fBsrTextsLanguageKindVariable" << " : \"" <<
    bsrTextsLanguageKindAsString (
      fBsrTextsLanguageKindVariable) <<
    "\"" <<
    endl;

  --gIndenter;
}

void bsrTextsLanguageAtom::printAtomWithVariableOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    bsrTextsLanguageKindAsString (
      fBsrTextsLanguageKindVariable) <<
    "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_bsrTextsLanguageAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_bsrOahGroup gGlobalBsrOahGroup;

S_bsrOahGroup bsrOahGroup::create ()
{
  bsrOahGroup* o = new bsrOahGroup ();
  assert (o != nullptr);
  return o;
}

bsrOahGroup::bsrOahGroup ()
  : oahGroup (
      "BSR",
      "hbsr", "help-bsr",
  R"(These bsr control the way BSR data is handled.)",
      oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeBsrOahGroup ();
}

bsrOahGroup::~bsrOahGroup ()
{}

void bsrOahGroup::initializeBsrDisplayOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Display",
        "hbsrd", "help-bsr-display",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // display BSR

  fDisplayBsr = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dbsr", "display-bsr",
R"(Write the contents of the BSR data to standard error.)",
        "displayBsr",
        fDisplayBsr));

  // display BSR short

  fDisplayBsrShort = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dbsrs", "display-bsr-short",
R"(Write the contents of the BSR data, short version, to standard error.)",
        "displayBsrShort",
        fDisplayBsrShort));

  // display BSR details

  fDisplayBsrDetails = false;

  subGroup->
    appendAtomToSubGroup (
      oahTwoBooleansAtom::create (
        "dbsrd", "display-bsr-details",
R"(Write the contents of the BSR data with more details to standard error.)",
        "displayBsrDetails",
        fDisplayBsrDetails,
        fDisplayBsr));
}

void bsrOahGroup::initializeBsrLanguagesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Languages",
        "hbsrl", "help-bsr-languages",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // texts language

  if (! setBsrTextsLanguage ("english")) {
    stringstream s;

    s <<
      "INTERNAL INITIALIZATION ERROR: "
      "BSR texts language 'english' is unknown" <<
      endl <<
      "The " <<
      gGlobalBsrTextsLanguageKindsMap.size () <<
      " known BSR texts languages are:" <<
      endl;

    ++gIndenter;

    s <<
      existingBsrTextsLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  const bsrTextsLanguageKind
    bsrTextsLanguageKindDefaultValue =
      fBsrTextsLanguageKind;

  subGroup->
    appendAtomToSubGroup (
      bsrTextsLanguageAtom::create (
        "btl", "bsr-texts-language",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use LANGUAGE to transcribe texts in the BSR logs and views,
as well as in the generated braille music.
The NUMBER texts languages available are:
TEXT_LANGUAGES.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (gGlobalBsrTextsLanguageKindsMap.size ())),
            regex ("TEXT_LANGUAGES"),
            existingBsrTextsLanguageKinds (K_NAMES_LIST_MAX_LENGTH)),
          regex ("DEFAULT_VALUE"),
          bsrTextsLanguageKindAsString (
            bsrTextsLanguageKindDefaultValue)),
        "LANGUAGE",
        "bsr-texts-language",
        fBsrTextsLanguageKind));
}

#ifdef TRACING_IS_ENABLED
void bsrOahGroup::initializeBsrTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "BSR trace",
        "hbst", "help-bsr-trace",
R"(  Note: the options in this subgroup imply '-tbsr, -trace-bsr'.)",
// JMI        oahElementVisibilityKind::kElementVisibilityHeaderOnly,
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // BSR

  fTraceBsr = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tbsr", "trace-bsr",
R"(Write a trace of the BSR graphs visiting activity to standard error.)",
        "traceBsr",
        fTraceBsr));

  // pages

  fTracePages = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tpages", "trace-pages",
R"()",
        "tracePages",
        fTracePages));

  // lines

  fTraceLines = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tlines", "trace-lines",
R"()",
        "traceLines",
        fTraceLines));

  // spaces

  fTraceSpaces = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tspaces", "trace-spaces",
R"(Write a trace of the BSR spaces activity to standard error.)",
        "traceSpaces",
        fTraceSpaces));

  // numbers

  fTraceNumbers = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tnums", "trace-numbers",
R"(Write a trace of the BSR numbers activity to standard error.)",
        "traceNumbers",
        fTraceNumbers));

  // parallels

  fTraceParallels = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tpars", "trace-parallels",
R"()",
        "traceParallels",
        fTraceParallels));

  // BSR visitors

  fTraceBsrVisitors = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tbsrv", "trace-bsr-visitors",
R"(Write a trace of the BSR tree visiting activity to standard error.)",
        "traceBsrVisitors",
        fTraceBsrVisitors));
}
#endif

void bsrOahGroup::initializeBsrOahGroup ()
{
  // display
  // --------------------------------------
  initializeBsrDisplayOptions ();

  // languages
  // --------------------------------------
  initializeBsrLanguagesOptions ();

#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  initializeBsrTraceOah ();
#endif
}

//______________________________________________________________________________
bool bsrOahGroup::setBsrTextsLanguage (string language)
{
  // is language in the chords languages map?
  map<string, bsrTextsLanguageKind>::const_iterator
    it =
      gGlobalBsrTextsLanguageKindsMap.find (language);

  if (it == gGlobalBsrTextsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    return false;
  }

  fBsrTextsLanguageKind = (*it).second;

  return true;
}

//______________________________________________________________________________
void bsrOahGroup::enforceGroupQuietness ()
{
  fDisplayBsr = false;
  fDisplayBsrShort = false;
  fDisplayBsrDetails = false;
}

//______________________________________________________________________________
void bsrOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void bsrOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> bsrOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrOahGroup>*
    p =
      dynamic_cast<visitor<S_bsrOahGroup>*> (v)) {
        S_bsrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching bsrOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> bsrOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrOahGroup>*
    p =
      dynamic_cast<visitor<S_bsrOahGroup>*> (v)) {
        S_bsrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching bsrOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> bsrOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void bsrOahGroup::printBsrOahValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The BSR bsr are:" <<
    endl;

  ++gIndenter;

  // display
  // --------------------------------------
  gLogStream <<
    "Display:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "displayBsr" << " : " <<
    booleanAsString (fDisplayBsr) <<
    endl <<

    setw (fieldWidth) << "displayBsrShort" << " : " <<
    booleanAsString (fDisplayBsrShort) <<
    endl <<

    setw (fieldWidth) << "displayBsrDetails" << " : " <<
    booleanAsString (fDisplayBsrDetails) <<
    endl;

  --gIndenter;

  // trace
  // --------------------------------------

#ifdef TRACING_IS_ENABLED
  gLogStream <<
    "Trace:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "traceBsr" << " : " <<
    booleanAsString (fTraceBsr) <<
    endl <<
    setw (fieldWidth) << "tracePages" << " : " <<
    booleanAsString (fTracePages) <<
    endl <<
    setw (fieldWidth) << "traceLines" << " : " <<
    booleanAsString (fTraceLines) <<
    endl <<
    setw (fieldWidth) << "traceSpaces" << " : " <<
    booleanAsString (fTraceSpaces) <<
    endl <<
    setw (fieldWidth) << "traceNumbers" << " : " <<
    booleanAsString (fTraceNumbers) <<
    endl <<
    setw (fieldWidth) << "traceParallels" << " : " <<
    booleanAsString (fTraceParallels) <<
    endl <<
    setw (fieldWidth) << "traceBsrVisitors" << " : " <<
    booleanAsString (fTraceBsrVisitors) <<
    endl;

  --gIndenter;
#endif

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_bsrOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_bsrOahGroup createGlobalBsrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating global BSR OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalBsrOahGroup) {
    // create the bsr variables
    gGlobalBsrOahGroup =
      bsrOahGroup::create ();
    assert (gGlobalBsrOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalBsrOahGroup;
}


}
