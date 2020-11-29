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

#include "utilities.h"
#include "messagesHandling.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "oahElements.h"

namespace MusicXML2
{

//______________________________________________________________________________
string elementKindAsString (
  oahElementKind elementKind)
{
  string result;

  switch (elementKind) {
    case kElementWithoutValue:
      result = "elementWithoutValue";
      break;
    case kElementWithMandatoryValue:
      result = "elementWithMandatoryValue";
      break;
    case kElementWithOptionalValue:
      result = "elementWithOptionalValue";
      break;
  } // switch

  return result;
}

string elementVisibilityKindAsString (
  oahElementVisibilityKind elementVisibilityKind)
{
  string result;

  switch (elementVisibilityKind) {
    case kElementVisibilityNone:
      result = "elementVisibilityNone";
      break;

    case kElementVisibilityWhole:
      result = "elementVisibilityWhole";
      break;

    case kElementVisibilityHeaderOnly:
      result = "elementVisibilityHeaderOnly";
      break;

    case kElementVisibilityHidden:
      result = "elementVisibilityHidden";
      break;
  } // switch

  return result;
}

string elementHelpOnlyKindAsString (
  oahElementHelpOnlyKind elementHelpOnlyKind)
{
  string result;

  switch (elementHelpOnlyKind) {
    case kElementHelpOnlyYes:
      result = "elementHelpOnlyYes";
      break;
    case kElementHelpOnlyNo:
      result = "elementHelpOnlyNo";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
/* this class is purely virtual
S_oahElement oahElement::create (
  string                   shortName,
  string                   longName,
  string                   description,
  oahElementKind           elementKind,
  oahElementVisibilityKind elementVisibilityKind)
{
  oahElement* o = new
    oahElement (
      shortName,
      longName,
      description,
      elementKind,
      elementVisibilityKind);
  assert (o!=0);
  return o;
}
*/

oahElement::oahElement (
  string                   shortName,
  string                   longName,
  string                   description,
  oahElementKind           elementKind,
  oahElementVisibilityKind elementVisibilityKind)
{
  fShortName   = shortName;
  fLongName    = longName;
  fDescription = description;

  fElementKind = elementKind;

  fElementHelpOnlyKind = kElementHelpOnlyNo; // default value

  fElementVisibilityKind = elementVisibilityKind;

  fMultipleOccurrencesAllowed = false;
}

oahElement::~oahElement ()
{}

S_oahElement oahElement::thisElementIfItHasName (
  string name)
{
  S_oahElement result;

  if (
    name == fShortName
     ||
    name == fLongName
  ) {
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

string oahElement::fetchNamesBetweenQuotes () const
{
  stringstream s;

  s <<
    "'" <<
    fetchNames () <<
    "'";

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

void oahElement::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahElement::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahElement>*
    p =
      dynamic_cast<visitor<S_oahElement>*> (v)) {
        S_oahElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahElement::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahElement::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahElement::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahElement>*
    p =
      dynamic_cast<visitor<S_oahElement>*> (v)) {
        S_oahElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahElement::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahElement::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
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

string oahElement::asLongNamedOptionString () const
{
  if (fLongName.size ()) {
    return asActualLongNamedOptionString ();
  }
  else {
    return asShortNamedOptionString ();
  }
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

void oahElement::printOahElementEssentials (
  ostream& os,
  int      fieldWidth) const
{
  os << left <<
    setw (fieldWidth) <<
    "shortName" << " : " <<
    "\"" << fShortName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "longName" << " : " <<
    "\"" << fLongName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "description" << " : " <<
    "\"" << fDescription << "\"" <<
    endl <<

/* JMI
  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;
  gIndenter--;
*/

    setw (fieldWidth) <<
    "elementKind" << " : " <<
    elementKindAsString (
      fElementKind) <<
    endl <<

    setw (fieldWidth) <<
    "oahElementHelpOnlyKind" << " : " <<
    elementHelpOnlyKindAsString (
      fElementHelpOnlyKind) <<
    endl <<

    setw (fieldWidth) <<
    "elementVisibilityKind" << " : " <<
    elementVisibilityKindAsString (
      fElementVisibilityKind) <<
    endl <<

    setw (fieldWidth) <<
    "multipleOccurrencesAllowed" << " : " <<
    booleanAsString (
      fMultipleOccurrencesAllowed) <<
    endl;
}

void oahElement::printOahElementEssentialsShort (
  ostream& os,
  int      fieldWidth) const
{
  os << left <<
    setw (fieldWidth) <<
    fetchNames () << " : " <<
    fDescription <<
    endl;
}

void oahElement::print (ostream& os) const
{
  os <<
    "??? oahElement ???" <<
    endl;

  printOahElementEssentials (os, 40); // JMI
}

void oahElement::printShort (ostream& os) const
{
  os <<
    "??? oahElement ???" <<
    endl;

  printOahElementEssentials (os, 40); // JMI
}

void oahElement::printHelp (ostream& os) const
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
}

ostream& operator<< (ostream& os, const S_oahElement& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
bool compareOahElements::operator() (
  const S_oahElement firstElement,
  const S_oahElement secondElement) const
{
/*
  Compare:
  A binary predicate that takes two arguments of the same type as the elements and returns a bool. The expression comp(a,b), where comp is an object of this type and a and b are key values, shall return true if a is considered to go before b in the strict weak ordering the function defines.
  The multiset object uses this expression to determine both the order the elements follow in the container and whether two element keys are equivalent (by comparing them reflexively: they are equivalent if !comp(a,b) && !comp(b,a)).
  This can be a function pointer or a function object (see constructor for an example). This defaults to less<T>, which returns the same as applying the less-than operator (a<b).
  Aliased as member types multiset::key_compare and multiset::value_compare.
*/

  // let's decide that nullptr (which shouldn't occur too often...)
  // should go after all non null S_oahElement values

  bool result;

  if (firstElement) {
    if (secondElement) {
      result =
        firstElement->getShortName () < secondElement->getShortName ();
    }
    else {
      result = true;
    }
  }
  else {
    result = false;
  }

  return result;
}

//______________________________________________________________________________
S_oahElementUse oahElementUse::create (
  S_oahElement elementUsed,
  string       nameUsed,
  string       valueUsed)
{
  oahElementUse* o =
    new oahElementUse (
      elementUsed,
      nameUsed,
      valueUsed);
  assert (o!=0);
  return o;
}

oahElementUse::oahElementUse (
  S_oahElement elementUsed,
  string       nameUsed,
  string       valueUsed)
{
  fElementUsed = elementUsed;
  fNameUsed    = nameUsed;
  fValueUsed   = valueUsed;
}

oahElementUse::~oahElementUse ()
{}

string oahElementUse::asString () const
{
  stringstream s;

  s <<
    "Atom use" <<
    ": " << fElementUsed->fetchNamesBetweenQuotes () <<
    ", nameUsed: \"" << fNameUsed << "\"" <<
    ", valueUsed: \"" << fValueUsed << "\"" <<
    ", elementKind: " <<
    ", oahElementHelpOnlyKind: " <<
    elementHelpOnlyKindAsString (fElementUsed->getElementHelpOnlyKind ()) <<
    elementKindAsString (fElementUsed->getElementKind ()) <<
    ", elementVisibilityKind: " <<
    elementVisibilityKindAsString (fElementUsed->getElementVisibilityKind ()) <<
    ", multipleOccurrencesAllowed: " <<
    booleanAsString (fElementUsed->getMultipleOccurrencesAllowed ());

  return s.str ();
}

void oahElementUse::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahElementUse& elt)
{
  elt->print (os);
  return os;
}


}
