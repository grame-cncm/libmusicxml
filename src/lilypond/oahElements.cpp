/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iomanip>      // setw, setprecision, ...

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "messagesHandling.h"

#include "oahOah.h"

#include "oahElements.h"

namespace MusicXML2
{

//______________________________________________________________________________
string optionVisibilityKindAsString (
  oahElementVisibilityKind optionVisibilityKind)
{
  string result;

  switch (optionVisibilityKind) {
    case kElementVisibilityAlways:
      result = "elementVisibilityAlways";
      break;

    case kElementVisibilityHiddenByDefault:
      result = "elementVisibilityHiddenByDefault";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
S_oahElement oahElement::create (
  string                   shortName,
  string                   longName,
  string                   description,
  oahElementVisibilityKind optionVisibilityKind)
{
  oahElement* o = new
    oahElement (
      shortName,
      longName,
      description,
      optionVisibilityKind);
  assert(o!=0);
  return o;
}

oahElement::oahElement (
  string                   shortName,
  string                   longName,
  string                   description,
  oahElementVisibilityKind optionVisibilityKind)
{
  fShortName   = shortName;
  fLongName    = longName;
  fDescription = description;

  fElementVisibilityKind = optionVisibilityKind;

  fIsHidden = false;

  fMultipleOccurrencesAllowed = false;
}

oahElement::~oahElement ()
{}

S_oahElement oahElement::fetchOptionByName (
  string name)
{
  S_oahElement result;

  if (
    name == fShortName
     ||
    name == fLongName) {
    result = this;
  }

  return result;
}

string oahElement::fetchNames () const
{
  stringstream s;

  if (
    fShortName.size ()
        &&
    fLongName.size ()
  ) {
      s <<
        "-" << fShortName <<
        ", " <<
        "-" << fLongName;
  }

  else {
    if (fShortName.size ()) {
      s <<
      "-" << fShortName;
    }
    if (fLongName.size ()) {
      s <<
        "-" << fLongName;
    }
  }

  return s.str ();
}

string oahElement::fetchNamesInColumns (
  int subGroupsShortNameFieldWidth) const
{
  stringstream s;

  if (
    fShortName.size ()
        &&
    fLongName.size ()
    ) {
      s << left <<
        setw (subGroupsShortNameFieldWidth) <<
        "-" + fShortName <<
        ", " <<
        "-" << fLongName;
  }

  else {
    if (fShortName.size ()) {
      s << left <<
        setw (subGroupsShortNameFieldWidth) <<
          "-" + fShortName;
    }
    if (fLongName.size ()) {
      s <<
        "-" << fLongName;
    }
  }

  return s.str ();
}

string oahElement::fetchNamesBetweenParentheses () const
{
  stringstream s;

  s <<
    "(" <<
    fetchNames () <<
    ")";

  return s.str ();
}

string oahElement::fetchNamesInColumnsBetweenParentheses (
  int subGroupsShortNameFieldWidth) const
{
  stringstream s;

  s <<
    "(" <<
    fetchNamesInColumns (
      subGroupsShortNameFieldWidth) <<
    ")";

  return s.str ();
}

S_oahValuedAtom oahElement::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
  stringstream s;

  s <<
    "### atom option name " << optionName <<
    " attached to '" <<
    this->asString () <<
    "' is not handled";

  msrInternalError (
    gOahOah->fInputSourceName,
    K_NO_INPUT_LINE_NUMBER,
    __FILE__, __LINE__,
    s.str ());

  // no option value is needed
  return nullptr;
}

void oahElement::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> oahElement::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahElement>*
    p =
      dynamic_cast<visitor<S_oahElement>*> (v)) {
        S_oahElement elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching oahElement::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahElement::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> oahElement::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahElement>*
    p =
      dynamic_cast<visitor<S_oahElement>*> (v)) {
        S_oahElement elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching oahElement::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahElement::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> oahElement::browseData ()" <<
      endl;
  }
#endif
}

string oahElement::asShortNamedOptionString () const
{
  return "-" + fShortName;
}

string oahElement::asActualLongNamedOptionString () const
{
  return "-" + fLongName;
}

string oahElement::asString () const
{
  stringstream s;

  s <<
    "'-" << fLongName << "'"; // JMI

  return s.str ();
}

void oahElement::printOptionHeader (ostream& os) const
{
  os <<
    "-" << fShortName <<
    endl <<
    "-" << fLongName <<
    endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;

    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }
}

void oahElement::printOptionEssentials (
  ostream& os,
  int      fieldWidth) const
{
  os << left <<
    setw (fieldWidth) <<
    "fShortName" << " : " <<
    fShortName <<
    endl <<
    setw (fieldWidth) <<
    "fLongName" << " : " <<
    fLongName <<
    endl <<
    setw (fieldWidth) <<
    "fDescription" << " : " <<
    fDescription <<
    endl <<
    setw (fieldWidth) <<
    "fIsHidden" << " : " <<
    booleanAsString (
      fIsHidden) <<
    endl <<
    setw (fieldWidth) <<
    "fMultipleOccurrencesAllowed" << " : " <<
    booleanAsString (
      fMultipleOccurrencesAllowed) <<
    endl;
}

void oahElement::print (ostream& os) const
{
  os <<
    "??? oahElement ???" <<
    endl;

  printOptionEssentials (os, 40); // JMI
}

void oahElement::printHelp (ostream& os)
{
  os <<
    fetchNames () <<
    endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;

    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }

  // register help print action in options handler upLink JMI ???
//  fHandlerUpLink->setOptionsHandlerFoundAHelpOption ();
}

ostream& operator<< (ostream& os, const S_oahElement& elt)
{
  elt->print (os);
  return os;
}


}
