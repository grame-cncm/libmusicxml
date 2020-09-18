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

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "generalOah.h"
#include "bsrOah.h"

#include "messagesHandling.h"


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
  assert(o!=0);
  return o;
}

bsrFacSimileKindAtom::bsrFacSimileKindAtom (
  string           shortName,
  string           longName,
  string           description,
  string           valueSpecification,
  string           variableName,
  bsrFacSimileKind bsrFacSimileKindVariable)
  : oahValuedAtom (
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

void bsrFacSimileKindAtom::handleValuedAtomValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    os <<
      "==> oahAtom is of type 'bsrFacSimileKindAtom'" <<
      endl;
  }
#endif

}

void bsrFacSimileKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> bsrFacSimileKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrFacSimileKindAtom>*
    p =
      dynamic_cast<visitor<S_bsrFacSimileKindAtom>*> (v)) {
        S_bsrFacSimileKindAtom elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOahGroup->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching bsrFacSimileKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrFacSimileKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> bsrFacSimileKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrFacSimileKindAtom>*
    p =
      dynamic_cast<visitor<S_bsrFacSimileKindAtom>*> (v)) {
        S_bsrFacSimileKindAtom elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOahGroup->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching bsrFacSimileKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrFacSimileKindAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
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
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsFacSimileKindAtom:" <<
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
    "bsrFacSimileKindVariable" << " : \"" <<
    bsrFacSimileKindAsString (
      fBsrFacSimileKindVariable) <<
      "\"" <<
    endl;

  gIndenter--;
}

void bsrFacSimileKindAtom::printValuedAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
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
  assert(o!=0);
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
  : oahValuedAtom (
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

void bsrTextsLanguageAtom::handleValuedAtomValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    os <<
      "==> oahAtom is of type 'bsrTextsLanguageAtom'" <<
      endl;
  }
#endif

}

void bsrTextsLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> bsrTextsLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrTextsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_bsrTextsLanguageAtom>*> (v)) {
        S_bsrTextsLanguageAtom elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOahGroup->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching bsrTextsLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrTextsLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> bsrTextsLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrTextsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_bsrTextsLanguageAtom>*> (v)) {
        S_bsrTextsLanguageAtom elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOahGroup->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching bsrTextsLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrTextsLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
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
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsBsrTextsLanguageAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fBsrTextsLanguageKindVariable" << " : \"" <<
    bsrTextsLanguageKindAsString (
      fBsrTextsLanguageKindVariable) <<
    "\"" <<
    endl;

  gIndenter--;
}

void bsrTextsLanguageAtom::printValuedAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
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
S_bsrOah gGlobalBsrOah;
S_bsrOah gGlobalBsrOahUserChoices;
S_bsrOah gGlobalBsrOahWithDetailedTrace;

S_bsrOah bsrOah::create (
  S_oahHandler handlerUpLink)
{
  bsrOah* o = new bsrOah (
    handlerUpLink);
  assert(o!=0);
  return o;
}

bsrOah::bsrOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "BSR",
    "hbsr", "help-bsr",
R"(These bsr control the way BSR data is handled.)",
    kElementVisibilityWhole,
    handlerUpLink)
{
  // append this bsr group to the bsr handler
  // if relevant
  if (handlerUpLink) {
    handlerUpLink->
      appendGroupToHandler (this);
  }

  // initialize it
  initializeBsrOah (false);
}

bsrOah::~bsrOah ()
{}

void bsrOah::initializeBsrDisplayOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Display",
        "hbsrd", "help-bsr-display",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // display BSR

  fDisplayBsr = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dbsr", "display-bsr",
R"(Write the contents of the BSR data to standard error.)",
        "displayBsr",
        fDisplayBsr));

  // display BSR short

  fDisplayBsrShort = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dbsrs", "display-bsr-short",
R"(Write the contents of the BSR data, short version, to standard error.)",
        "displayBsrShort",
        fDisplayBsrShort));

  // display BSR details

  fDisplayBsrDetails = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahTwoBooleansAtom::create (
        "dbsrd", "display-bsr-details",
R"(Write the contents of the BSR data with more details to standard error.)",
        "displayBsrDetails",
        fDisplayBsrDetails,
        fDisplayBsr));
}

void bsrOah::initializeBsrLanguagesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Languages",
        "hbsrl", "help-bsr-languages",
R"()",
      kElementVisibilityWhole,
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
      gBsrTextsLanguageKindsMap.size () <<
      " known BSR texts languages are:" <<
      endl;

    gIndenter++;

    s <<
      existingBsrTextsLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    gIndenter--;

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
              to_string (gBsrTextsLanguageKindsMap.size ())),
            regex ("TEXT_LANGUAGES"),
            existingBsrTextsLanguageKinds (K_NAMES_LIST_MAX_LENGTH)),
          regex ("DEFAULT_VALUE"),
          bsrTextsLanguageKindAsString (
            bsrTextsLanguageKindDefaultValue)),
        "LANGUAGE",
        "bsr-texts-language",
        fBsrTextsLanguageKind));
}

#ifdef TRACE_OAH
void bsrOah::initializeBsrTraceOah (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Trace",
        "hbst", "help-bsr-trace",
R"(  Note: the options in this subgroup imply '-tbsr, -trace-bsr'.)",
// JMI        kElementVisibilityHeaderOnly,
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // BSR

  fTraceBsr = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tbsr", "trace-bsr",
R"(Write a trace of the BSR graphs visiting activity to standard error.)",
        "traceBsr",
        fTraceBsr));

  // pages

  fTracePages = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tpages", "trace-pages",
R"()",
        "tracePages",
        fTracePages));

  // lines

  fTraceLines = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tlines", "trace-lines",
R"()",
        "traceLines",
        fTraceLines));

  // spaces

  fTraceSpaces = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tspaces", "trace-spaces",
R"(Write a trace of the BSR spaces activity to standard error.)",
        "traceSpaces",
        fTraceSpaces));

  // numbers

  fTraceNumbers = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tnums", "trace-numbers",
R"(Write a trace of the BSR numbers activity to standard error.)",
        "traceNumbers",
        fTraceNumbers));

  // parallels

  fTraceParallels = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tpars", "trace-parallels",
R"()",
        "traceParallels",
        fTraceParallels));

  // BSR visitors

  fTraceBsrVisitors = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tbsrv", "trace-bsr-visitors",
R"(Write a trace of the BSR tree visiting activity to standard error.)",
        "traceBsrVisitors",
        fTraceBsrVisitors));
}
#endif

void bsrOah::initializeBsrOah (
  bool boolOptionsInitialValue)
{
  // display
  // --------------------------------------
  initializeBsrDisplayOptions (
    boolOptionsInitialValue);

  // languages
  // --------------------------------------
  initializeBsrLanguagesOptions (
    boolOptionsInitialValue);

#ifdef TRACE_OAH
  // trace
  // --------------------------------------
  initializeBsrTraceOah (
    boolOptionsInitialValue);
#endif
}

S_bsrOah bsrOah::createCloneWithDetailedTrace ()
{
  S_bsrOah
    clone =
      bsrOah::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the bsr handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;


  // display
  // --------------------------------------

  clone->fDisplayBsr =
    true;
  clone->fDisplayBsrShort =
    true;
  clone->fDisplayBsrDetails =
    true;


  // miscellaneous
  // --------------------------------------


  // exit after some passes
  // --------------------------------------


  return clone;
}

//______________________________________________________________________________
bool bsrOah::setBsrTextsLanguage (string language)
{
  // is language in the chords languages map?
  map<string, bsrTextsLanguageKind>::const_iterator
    it =
      gBsrTextsLanguageKindsMap.find (language);

  if (it == gBsrTextsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    return false;
  }

  fBsrTextsLanguageKind = (*it).second;

  return true;
}

//______________________________________________________________________________
void bsrOah::enforceGroupQuietness ()
{
  fDisplayBsr = false;
  fDisplayBsrShort = false;
  fDisplayBsrDetails = false;
}

//______________________________________________________________________________
void bsrOah::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void bsrOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> bsrOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrOah>*
    p =
      dynamic_cast<visitor<S_bsrOah>*> (v)) {
        S_bsrOah elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOahGroup->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching bsrOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> bsrOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrOah>*
    p =
      dynamic_cast<visitor<S_bsrOah>*> (v)) {
        S_bsrOah elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOahGroup->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching bsrOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> bsrOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void bsrOah::printBsrOahValues (int fieldWidth)
{
  gLogOstream <<
    "The BSR bsr are:" <<
    endl;

  gIndenter++;

  // display
  // --------------------------------------
  gLogOstream <<
    "Display:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "displayBsr" << " : " <<
    booleanAsString (fDisplayBsr) <<
    endl <<

    setw (fieldWidth) << "displayBsrShort" << " : " <<
    booleanAsString (fDisplayBsrShort) <<
    endl <<

    setw (fieldWidth) << "displayBsrDetails" << " : " <<
    booleanAsString (fDisplayBsrDetails) <<
    endl;

  gIndenter--;

  // trace
  // --------------------------------------

#ifdef TRACE_OAH
  gLogOstream <<
    "Trace:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
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

  gIndenter--;
#endif

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeBsrOahHandling (
  S_oahHandler handler)
{
#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->getTraceOah () && ! gGlobalGeneralOahGroup->fQuiet) {
    gLogOstream <<
      "Initializing BSR bsr handling" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
    // create the bsr variables
    // ------------------------------------------------------

    gGlobalBsrOahUserChoices = bsrOah::create (
      handler);
    assert(gGlobalBsrOahUserChoices != 0);

    gGlobalBsrOah =
      gGlobalBsrOahUserChoices;

    // prepare for measure detailed trace
    // ------------------------------------------------------
  /* JMI
    gGlobalBsrOahWithDetailedTrace =
      gGlobalBsrOah->
        createCloneWithDetailedTrace ();
        */

// JMI    pThisMethodHasBeenRun = true;
  }
}


}
