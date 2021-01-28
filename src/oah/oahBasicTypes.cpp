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

#include <regex>

#include "utilities.h"
#include "messagesHandling.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "generalOah.h"

#include "oahOah.h"

#include "oahBasicTypes.h"

#include "version.h"


namespace MusicXML2
{
/*
  ENFORCE_TRACE_OAH can be used to issue trace messages
  before "trace-oah", if present, has been applied
*/

//#define ENFORCE_TRACE_OAH

// handler used thru...?
//______________________________________________________________________________
string oahHandlerUsedThruKindAsString (
  oahHandlerUsedThruKind handlerUsedThruKind)
{
  string result;

  switch (handlerUsedThruKind) {
    case kHandlerUsedThruUnknown: // default value
      result = "handlerUsedThruUnknown";
      break;
    case kHandlerUsedThruArgcAndArgv:
      result = "handlerUsedThruArgcAndArgv";
      break;
    case kHandlerUsedThruOptionsVector:
      result = "handlerUsedThruOptionsVector";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
void displayOptionsVector (
  const optionsVector& theOptionsVector,
  ostream&             os)
{
  os <<
    "The options vector contains " <<
    singularOrPlural (
      theOptionsVector.size (), "element", "elements");

  if (theOptionsVector.size ()) {
    os << ":" << endl;

    ++gIndenter;

    for (unsigned int i = 0; i < theOptionsVector.size (); ++i) {
      string optionName  = theOptionsVector [i].first;
      string optionValue = theOptionsVector [i].second;

      os <<
        right << setw (2) << i <<
        ": \"" <<
        optionName <<
        "\" \"" <<
        optionValue <<
        "\"" <<
        endl;
    } //for

    os << endl;

    --gIndenter;
  }
  else {
    os << endl;
  }
}

//______________________________________________________________________________
S_oahPrefix oahPrefix::create (
  string prefixName,
  string prefixErsatz,
  string prefixDescription)
{
  oahPrefix* o = new
    oahPrefix (
      prefixName,
      prefixErsatz,
      prefixDescription);
  assert (o != nullptr);
  return o;
}

oahPrefix::oahPrefix (
  string prefixName,
  string prefixErsatz,
  string prefixDescription)
{
  fPrefixName        = prefixName;
  fPrefixErsatz      = prefixErsatz;
  fPrefixDescription = prefixDescription;
}

oahPrefix::~oahPrefix ()
{}

void oahPrefix::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahPrefix::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahPrefix>*
    p =
      dynamic_cast<visitor<S_oahPrefix>*> (v)) {
        S_oahPrefix elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahPrefix::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahPrefix::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahPrefix::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahPrefix>*
    p =
      dynamic_cast<visitor<S_oahPrefix>*> (v)) {
        S_oahPrefix elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahPrefix::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahPrefix::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahPrefix::browseData ()" <<
      endl;
  }
#endif
}

string oahPrefix::fetchPrefixNames () const
{
  stringstream s;

  s <<
    "\"" <<
    fPrefixName <<
    "\", \"" <<
   fPrefixErsatz <<
   "\"";

  return s.str ();
}

void oahPrefix::findStringInPrefix (
  string        lowerCaseString,
  list<string>& foundStringsList,
  ostream&      os) const
{
  // does this element's short name match?
  bool prefixNameMatches =
    stringToLowerCase (fPrefixName).find (lowerCaseString) != string::npos;

  // does this element's long name match?
  bool prefixErsatzMatches =
    stringToLowerCase (fPrefixErsatz).find (lowerCaseString) != string::npos;

  // does this element's description match?
  bool prefixDescriptionMatches =
    stringToLowerCase (fPrefixDescription).find (lowerCaseString) != string::npos;

  if (prefixNameMatches || prefixErsatzMatches || prefixDescriptionMatches) {
    stringstream s;

    s <<
      fetchPrefixNames () <<
        endl;

    // indent a bit more for readability
 // JMI    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    s <<
 //     gIndenter.indentMultiLineString ( // JMI
//        fDescription) <<
      fPrefixDescription;

 // JMI    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);

    // append the string
    foundStringsList.push_back (s.str ());
  }
}

void oahPrefix::printPrefixEssentials (
  ostream& os,
  unsigned int fieldWidth) const
{
  os << left <<
    setw (fieldWidth) <<
    "prefixName" << " : " <<
    fPrefixName <<
    endl <<
    setw (fieldWidth) <<
    "prefixErsatz" << " : " <<
    fPrefixErsatz <<
    endl <<
    setw (fieldWidth) <<
    "prefixDescription" << " : " <<
    fPrefixDescription <<
    endl;
}

void oahPrefix::print (ostream& os) const
{
  os <<
    "??? oahPrefix ???" <<
    endl;

  printPrefixEssentials (os, 40); // JMI
}

void oahPrefix::printShort (ostream& os) const
{
  os <<
    "??? oahPrefix ???" ;

  printPrefixEssentials (os, 40); // JMI
}

void oahPrefix::printHelp (ostream& os) const
{
  if (fPrefixDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fPrefixDescription) <<
      endl;

    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }

  os << endl;
}

ostream& operator<< (ostream& os, const S_oahPrefix& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
/* this class is purely virtual
S_oahAtom oahAtom::create (
  string         shortName,
  string         longName,
  string         description,
  oahElementKind atomValueExpectedKind)
{
  oahAtom* o = new
    oahAtom (
      shortName,
      longName,
      description,
      atomValueExpectedKind);
  assert (o != nullptr);
  return o;
}
*/

oahAtom::oahAtom (
  string         shortName,
  string         longName,
  string         description,
  oahElementKind atomValueExpectedKind)
  : oahElement (
      shortName,
      longName,
      description,
      atomValueExpectedKind,
      kElementVisibilityWhole)
{}

oahAtom::~oahAtom ()
{}

void oahAtom::setSubGroupUpLink (
  S_oahSubGroup subGroup)
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    subGroup != nullptr,
    "subGroup is null");

  // set the atom subgroup upLink
  fSubGroupUpLink = subGroup;
}

S_oahGroup oahAtom::fetchAtomGroupUpLink () const
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    fSubGroupUpLink != nullptr,
    "fSubGroupUpLink is null");

  return
    fSubGroupUpLink->
      getGroupUpLink ();
}

S_oahHandler oahAtom::fetchAtomHandlerUpLink () const
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    fSubGroupUpLink != nullptr,
    "fSubGroupUpLink is null");

  return
    fSubGroupUpLink->
      getGroupUpLink ()->
        getHandlerUpLink ();
}

void oahAtom::appendAtomToElementsList (
  S_oahHandler handler)
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
if (false)
  gLogStream <<
    "Registering atom " <<
    fetchNamesBetweenQuotes () <<
    " in handler \"" <<
    handler->getHandlerHeader () <<
    "\"" <<
    endl;
#endif
#endif

  ++gIndenter;

// JMI
  // an atom with an optional value
  // should not have the same name as a prefix,
  // since this would create an ambiguity
  switch (fElementKind) {
    case kElementWithoutValue:
    case kElementWithMandatoryValue:
      break;
    case kElementWithOptionalValue:
      if (handler->fetchNameInPrefixesMap (fShortName)) {
        stringstream s;

        s <<
          "the short name of atom with optional value " <<
          fetchNamesBetweenQuotes () <<
          " is already known as a prefix";

        oahError (s.str ());
      }
      if (handler->fetchNameInPrefixesMap (fLongName)) {
        stringstream s;

        s <<
          "the long name of atom with optional value " <<
          fetchNamesBetweenQuotes () <<
          " is already known as a prefix";

        oahError (s.str ());
      }
      break;
  } // switch

  handler->
    appendElementToElementsList (this);

  --gIndenter;
}

void oahAtom::registerAtomAsBeingUsed ()
{
  fSubGroupUpLink->incrementNumberOfUserChoseAtomsInThisSubGroup ();

  S_oahGroup
    atomGroup =
      fSubGroupUpLink-> getGroupUpLink ();

  atomGroup->incrementNumberOfUserChoseAtomsInThisGroup ();
}

void oahAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahAtom>*
    p =
      dynamic_cast<visitor<S_oahAtom>*> (v)) {
        S_oahAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahAtom>*
    p =
      dynamic_cast<visitor<S_oahAtom>*> (v)) {
        S_oahAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtom::browseData ()" <<
      endl;
  }
#endif
}

void oahAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = 19;

  os <<
    "Atom ???:" <<
      endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) << "elementHelpOnlyKind" << " : " <<
    elementHelpOnlyKindAsString (fElementHelpOnlyKind) <<
    endl;

  --gIndenter;
}

void oahAtom::printShort (ostream& os) const
{
  const unsigned int fieldWidth = 19;

  os <<
    "Atom: ";

  oahElement::printOahElementEssentialsShort (
    os, fieldWidth);
}

void oahAtom::printSummary (ostream& os) const
{
  os <<
    "Atom: " <<
    fetchNames () <<
    endl;
}

void oahAtom::findStringInAtom (
  string        lowerCaseString,
  list<string>& foundStringsList,
  ostream&      os) const
{
  findStringInElement (
    lowerCaseString,
    foundStringsList,
    os);
}

void oahAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os <<
    "Atom values ???:" <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
/* this class is purely virtual
S_oahAtomWithVariableName oahAtomWithVariableName::create (
  string shortName,
  string longName,
  string description,
  string variableName)
{
  oahAtomWithVariableName* o = new
    oahAtomWithVariableName (
      shortName,
      longName,
      description,
      variableName);
  assert (o != nullptr);
  return o;
}
*/

oahAtomWithVariableName::oahAtomWithVariableName (
  string shortName,
  string longName,
  string description,
  string variableName)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementWithoutValue),
    fVariableName (
      variableName)
{}

oahAtomWithVariableName::~oahAtomWithVariableName ()
{}

void oahAtomWithVariableName::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtomWithVariableName::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahAtomWithVariableName>*
    p =
      dynamic_cast<visitor<S_oahAtomWithVariableName>*> (v)) {
        S_oahAtomWithVariableName elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahAtomWithVariableName::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahAtomWithVariableName::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtomWithVariableName::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahAtomWithVariableName>*
    p =
      dynamic_cast<visitor<S_oahAtomWithVariableName>*> (v)) {
        S_oahAtomWithVariableName elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahAtomWithVariableName::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahAtomWithVariableName::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtomWithVariableName::browseData ()" <<
      endl;
  }
#endif
}

void oahAtomWithVariableName::printAtomWithVariableNameEssentials (
  ostream& os,
  unsigned int fieldWidth) const
{
  printOahElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "variableName" << " : " <<
    "\"" << fVariableName << "\"" <<
    endl;
}

void oahAtomWithVariableName::printAtomWithVariableNameEssentialsShort (
  ostream& os,
  unsigned int fieldWidth) const
{
  printOahElementEssentialsShort (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "variableName" << " : " <<
    "\"" << fVariableName << "\"" <<
    endl;
}

void oahAtomWithVariableName::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "AtomWithVariableName:" <<
    endl;

  ++gIndenter;

  printOahElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "variableName" << " : " <<
    "\"" << fVariableName << "\"" <<
    endl << endl;

  --gIndenter;
}

void oahAtomWithVariableName::printShort (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "AtomWithVariableName: ";

  printOahElementEssentialsShort (
    os, fieldWidth);

  os <<
    fDescription <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "variableName" << " : " <<
    "\"" << fVariableName << "\"" <<
    endl << endl;
}

void oahAtomWithVariableName::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    "JMI" << // ???
    endl;
}

ostream& operator<< (ostream& os, const S_oahAtomWithVariableName& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
/* this class is purely virtual
S_oahAtomWithValue oahAtomWithValue::create (
  string shortName,
  string longName,
  string description,
  string valueSpecification,
  string variableName)
{
  oahAtomWithValue* o = new
    oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName);
  assert (o != nullptr);
  return o;
}
*/

oahAtomWithValue::oahAtomWithValue (
  string shortName,
  string longName,
  string description,
  string valueSpecification,
  string variableName)
  : oahAtomWithVariableName (
      shortName,
      longName,
      description,
      variableName)
{
  fValueSpecification = valueSpecification;

  fVariableHasBeenSet = false;

  this->setElementKind (kElementWithMandatoryValue);
}

oahAtomWithValue::~oahAtomWithValue ()
{}

void oahAtomWithValue::applyElement (ostream& os)
{
  stringstream s;

  s <<
    "Applying atom with mandatory value '" <<
    fetchNames () <<
    "' without a value in not possible";

  oahInternalError (s.str ());
}

void oahAtomWithValue::applyAtomWithValueDefaultValue (ostream& os)
{
  stringstream s;

  s <<
    "Applying atom with mandatory value '" <<
    fetchNames () <<
    "' with a default value is possible only in oahAtomWithValue subclasses";

  oahInternalError (s.str ());
}

void oahAtomWithValue::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtomWithValue::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahAtomWithValue>*
    p =
      dynamic_cast<visitor<S_oahAtomWithValue>*> (v)) {
        S_oahAtomWithValue elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahAtomWithValue::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahAtomWithValue::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtomWithValue::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahAtomWithValue>*
    p =
      dynamic_cast<visitor<S_oahAtomWithValue>*> (v)) {
        S_oahAtomWithValue elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahAtomWithValue::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahAtomWithValue::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtomWithValue::browseData ()" <<
      endl;
  }
#endif
}

void oahAtomWithValue::printAtomWithValueEssentials (
  ostream& os,
  unsigned int fieldWidth) const
{
  printAtomWithVariableNameEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "valueSpecification" << " : " <<
    fValueSpecification <<
    endl <<
    setw (fieldWidth) <<
    "variableHasBeenSet" << " : " <<
    booleanAsString (fVariableHasBeenSet) <<
    endl;
}

void oahAtomWithValue::printAtomWithValueEssentialsShort (
  ostream& os,
  unsigned int fieldWidth) const
{
  printAtomWithVariableNameEssentialsShort (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "valueSpecification" << " : " <<
    fValueSpecification <<
    endl <<
    setw (fieldWidth) <<
    "variableHasBeenSet" << " : " <<
    booleanAsString (fVariableHasBeenSet) <<
    endl;
}

void oahAtomWithValue::print (ostream& os) const
{
  const unsigned int fieldWidth = 19;

  os <<
    "AtomWithValue:";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahAtomWithValue::printShort (ostream& os) const
{
  const unsigned int fieldWidth = 19;

  os <<
    "AtomWithValue: ";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }

  printAtomWithValueEssentialsShort (
    os, fieldWidth);
}

void oahAtomWithValue::printHelp (ostream& os) const
{
  os <<
    fetchNames ();

/* JMI
  oahOptionalValuesStyleKind
    optionalValuesStyleKind =
      getHandlerUpLink ()->
        getHandlerOptionalValuesStyleKind ();

  switch (fElementKind) {
    case kElementWithoutValue:
    case kElementWithMandatoryValue:
      os <<
        " " << fValueSpecification;
      break;
    case kElementWithOptionalValue:
      switch (optionalValuesStyleKind) {
        case kOptionalValuesStyleGNU: // default value
          os <<
            "[=" << fValueSpecification << "]";
          break;
        case kOptionalValuesStyleOAH:
          os <<
            " " << fValueSpecification;
          break;
      } // switch
      break;
  } // switch
  os << endl;
*/

  os <<
    " " << fValueSpecification <<
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

void oahAtomWithValue::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os <<
    "AtomWithValue values:" <<
    "???, variableHasBeenSet: " <<
    booleanAsString (fVariableHasBeenSet) <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahAtomWithValue& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahSubGroup oahSubGroup::create (
  string                   subGroupHeader,
  string                   shortName,
  string                   longName,
  string                   description,
  oahElementVisibilityKind optionVisibilityKind,
  S_oahGroup               groupUpLink)
{
  oahSubGroup* o = new
    oahSubGroup (
      subGroupHeader,
      shortName,
      longName,
      description,
      optionVisibilityKind,
      groupUpLink);
  assert (o != nullptr);
  return o;
}

oahSubGroup::oahSubGroup (
  string                   subGroupHeader,
  string                   shortName,
  string                   longName,
  string                   description,
  oahElementVisibilityKind optionVisibilityKind,
  S_oahGroup               groupUpLink)
  : oahElement (
      shortName,
      longName,
      description,
      kElementWithoutValue,
      optionVisibilityKind)
{
  fGroupUpLink = groupUpLink;

  fSubGroupHeader = subGroupHeader;

  fElementHelpOnlyKind = kElementHelpOnlyYes;

  fNumberOfUserChoseAtomsInThisSubGroup = 0;
}

oahSubGroup::~oahSubGroup ()
{}

S_oahHandler oahSubGroup::fetchSubGroupHandlerUpLink () const
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    fGroupUpLink != nullptr,
    "fGroupUpLink is null");

  return
    fGroupUpLink->
      getHandlerUpLink ();
}

void oahSubGroup::appendSubGroupToElementsList (
  S_oahHandler handler)
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Appending subgroup \"" <<
    fSubGroupHeader <<
    "\" in the elements list in handler \"" <<
    handler->getHandlerHeader () <<
    "\"" <<
    endl;
#endif
#endif

  ++gIndenter;

  // append subgroup to elements list
  handler->
    appendElementToElementsList (this);

  fGroupUpLink->
    setHandlerUpLink (handler);

  // register the OAH atoms in the handler
  for (
    list<S_oahAtom>::const_iterator
      i = fSubGroupAtomsList.begin ();
    i != fSubGroupAtomsList.end ();
    ++i
  ) {
    S_oahAtom atom = (*i);
    // register atom in the handler elements list
    atom->
      appendAtomToElementsList (
        handler);
  } // for

  --gIndenter;
}

void oahSubGroup::registerNamesInSubGroupToTheNamesToElementsMap (
  S_oahHandler handler)
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Registering the names in subgroup \"" <<
    fSubGroupHeader <<
    "\" in the names to elements map in handler \"" <<
    handler->getHandlerHeader () <<
    "\"" <<
    endl;
#endif
#endif

  ++gIndenter;

  // register this subgroup's names in this handler
  handler->
    registerElementNamesInHandler (
      this);

  // register this subgroup's names in handler
  for (
    list<S_oahAtom>::const_iterator
      i = fSubGroupAtomsList.begin ();
    i != fSubGroupAtomsList.end ();
    ++i
  ) {
    S_oahAtom atom = (*i);

    // register atom's names in handler
    handler->
      registerElementNamesInHandler (
        atom);
  } // for

  --gIndenter;
}

void oahSubGroup::appendAtomToSubGroup (
  S_oahAtom atom)
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    atom != nullptr,
    "atom is null");

  // append atom to atoms list
  fSubGroupAtomsList.push_back (
    atom);

  // set atom subgroup upLink
  atom->
    setSubGroupUpLink (this);
}

S_oahElement oahSubGroup::fetchOptionByNameInSubGroup (
  string name)
{
  S_oahElement result;

  for (
    list<S_oahAtom>::const_iterator
      i = fSubGroupAtomsList.begin ();
    i != fSubGroupAtomsList.end ();
    ++i
  ) {
    S_oahAtom atom = (*i);

    // has atom this name?
    result =
      atom->thisElementIfItHasName (name);

    if (result) {
      break;
    }
  } // for

  return result;
}

void oahSubGroup::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Applying element " <<
      fetchNames () <<
      ", which is a oahSubGroup" <<
      endl;
  }
#endif

  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  // print the help
  gLogStream <<
    "--- Help for subgroup \"" <<
    fSubGroupHeader <<
    "\" in group \"" <<
    fGroupUpLink->getGroupHeader () <<
    "\" ---" <<
    endl;

  ++gIndenter;

  fGroupUpLink->
    printGroupAndSubGroupHelp (
      os,
      this);

  --gIndenter;

  gIndenter.setIndent (saveIndent);
}

void oahSubGroup::checkSubGroupOptionsConsistency ()
{}

void oahSubGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahSubGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahSubGroup>*
    p =
      dynamic_cast<visitor<S_oahSubGroup>*> (v)) {
        S_oahSubGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahSubGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahSubGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahSubGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahSubGroup>*
    p =
      dynamic_cast<visitor<S_oahSubGroup>*> (v)) {
        S_oahSubGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahSubGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahSubGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahSubGroup::browseData ()" <<
      ", " << fSubGroupHeader <<
      endl;
  }
#endif

  // browse the atoms
  if (fSubGroupAtomsList.size ()) {
    for (
      list<S_oahAtom>::const_iterator i = fSubGroupAtomsList.begin ();
      i != fSubGroupAtomsList.end ();
      ++i
    ) {
      S_oahAtom atom = (*i);

      // browse the atom
      /* JMI ???
      gLogStream <<
        ".\\\" ==> Going to browse atom" <<
        " '" << atom->fetchNames () << "'" <<
        endl;
        */

      oahBrowser<oahAtom> browser (v);
      browser.browse (*(atom));
    } // for
  }
}

void oahSubGroup::print (ostream& os) const
{
  const unsigned int fieldWidth = 27;

  os <<
   "SubGroup:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "elementVisibilityKind" << " : " <<
      elementVisibilityKindAsString (
        fElementVisibilityKind) <<
    endl << endl;

  os <<
    "AtomsList (" <<
    singularOrPlural (
      fSubGroupAtomsList.size (), "atom",  "atoms") <<
    "):" <<
    endl;

  if (fSubGroupAtomsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_oahAtom>::const_iterator
      iBegin = fSubGroupAtomsList.begin (),
      iEnd   = fSubGroupAtomsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the atom
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;
}

void oahSubGroup::printShort (ostream& os) const
{
  const unsigned int fieldWidth = 27;

  os <<
   "SubGroup: " ;

  oahElement::printOahElementEssentialsShort (
    os, fieldWidth);

  if (fSubGroupAtomsList.size ()) {
    ++gIndenter;

    list<S_oahAtom>::const_iterator
      iBegin = fSubGroupAtomsList.begin (),
      iEnd   = fSubGroupAtomsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the atom
      (*i)->printShort (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }
}

void oahSubGroup::printSummary (ostream& os) const
{
  os <<
    "SubGroup: " <<
    fetchNames () <<
    endl;

  if (fSubGroupAtomsList.size ()) {
    ++gIndenter;

    list<S_oahAtom>::const_iterator
      iBegin = fSubGroupAtomsList.begin (),
      iEnd   = fSubGroupAtomsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahAtom atom = (*i);

      // print a summary of the atom
      atom->printSummary (os);
      if (++i == iEnd) break;
//      os << endl;
    } // for

    --gIndenter;
  }
}

void oahSubGroup::underlineSubGroupHeader (ostream& os) const
{
  /* JMI ???
  for (unsigned int i = 0; i < fSubGroupHeader.size (); ++i) {
    os << "-";
  } // for
  os << endl;
  */
  os << "--------------------------" << endl;
}

void oahSubGroup::printSubGroupHeader (ostream& os) const
{
  // print the header and option names
  os <<
    fSubGroupHeader;

  os <<
    " " <<
    fetchNamesBetweenParentheses ();

  switch (fElementVisibilityKind) {
    case kElementVisibilityNone:
      break;

    case kElementVisibilityWhole:
      os <<
        ":";
      break;

    case kElementVisibilityHeaderOnly:
      os <<
        " (use this option to show this subgroup)";
      break;

    case kElementVisibilityHidden:
      break;
  } // switch

  os << endl;
}

void oahSubGroup::printSubGroupHeaderWithHeaderWidth (
  ostream& os,
  int      subGroupHeaderWidth) const
{
  // print the header and option names
  os << left <<
    setw (subGroupHeaderWidth) <<
    fSubGroupHeader <<
    " " <<
    fetchNamesBetweenParentheses ();

  switch (fElementVisibilityKind) {
    case kElementVisibilityNone:
      break;

    case kElementVisibilityWhole:
      os <<
        ":";
      break;

    case kElementVisibilityHeaderOnly:
      os <<
        " (use this option to show this subgroup)";
      break;

    case kElementVisibilityHidden:
      break;
  } // switch

  os << endl;

}

void oahSubGroup::printHelp (ostream& os) const
{
  // print the header and option names
  printSubGroupHeader (os);

  // print the description if any
  if (fDescription.size ()) {
    ++gIndenter;
    os <<
      gIndenter.indentMultiLineString (
        fDescription);
    --gIndenter;

    os << endl;
  }

  // print the options atoms if relevant
  switch (fElementVisibilityKind) {
    case kElementVisibilityNone:
      break;

    case kElementVisibilityWhole:
      if (fSubGroupAtomsList.size ()) {
        ++gIndenter;

        list<S_oahAtom>::const_iterator
          iBegin = fSubGroupAtomsList.begin (),
          iEnd   = fSubGroupAtomsList.end (),
          i      = iBegin;
        for ( ; ; ) {
          S_oahAtom atom = (*i);

          // print the atom help unless it is not visible
          switch (atom->getElementVisibilityKind ()) {
            case kElementVisibilityWhole:
            case kElementVisibilityHeaderOnly:
              atom->printHelp (os);
              break;

            case kElementVisibilityNone:
            case kElementVisibilityHidden:
              break;
          } // switch

          if (++i == iEnd) break;
        } // for

        --gIndenter;
      }
      break;

    case kElementVisibilityHeaderOnly:
      break;

    case kElementVisibilityHidden:
      break;
  } // switch
}

void oahSubGroup::printHelpWithHeaderWidth (
  ostream& os,
  int      subGroupHeaderWidth) const
{
  // print the header and option names
  printSubGroupHeaderWithHeaderWidth (
    os,
    subGroupHeaderWidth);

  // underline the header if the group header is not written
  if (! fGroupUpLink->getGroupHeaderIsToBeWritten ()) {
    underlineSubGroupHeader (os);
  }

  // print the description if any
  if (fDescription.size ()) {
    ++gIndenter;
    os <<
      gIndenter.indentMultiLineString (
        fDescription);
    --gIndenter;

    os << endl;
  }

  // print the options atoms if relevant
  switch (fElementVisibilityKind) {
    case kElementVisibilityWhole:
      if (fSubGroupAtomsList.size ()) {
        ++gIndenter;

        list<S_oahAtom>::const_iterator
          iBegin = fSubGroupAtomsList.begin (),
          iEnd   = fSubGroupAtomsList.end (),
          i      = iBegin;
        for ( ; ; ) {
          S_oahAtom atom = (*i);

          // print the atom help unless it is not visible
          switch (atom->getElementVisibilityKind ()) {
            case kElementVisibilityWhole:
            case kElementVisibilityHeaderOnly:
              atom->printHelp (os);
              break;

            case kElementVisibilityNone:
            case kElementVisibilityHidden:
              break;
          } // switch

          if (++i == iEnd) break;
        } // for

        --gIndenter;
      }
      break;

    case kElementVisibilityHeaderOnly:
      break;

    case kElementVisibilityNone:
    case kElementVisibilityHidden:
      break;
  } // switch
}

void oahSubGroup::printSubGroupHelp (ostream& os) const
{
  // print the header and option names
  printSubGroupHeader (os);

  // print the description if any
  if (fDescription.size ()) {
    ++gIndenter;
    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;
    --gIndenter;
  }

  if (fSubGroupAtomsList.size ()) {
    ++gIndenter;

    list<S_oahAtom>::const_iterator
      iBegin = fSubGroupAtomsList.begin (),
      iEnd   = fSubGroupAtomsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the atom help
      (*i)->printHelp (os);
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for

    --gIndenter;
  }
}

void oahSubGroup::printOptionsSummary (
  ostream& os) const
{
  // fetch maximum subgroups help headers size
  unsigned int maximumSubGroupsHelpHeadersSize =
    getGroupUpLink ()->
      getHandlerUpLink ()->
        getMaximumSubGroupsHeadersSize ();

    /* JMI
  // fetch maximum short name width
  int maximumShortNameWidth =
    getGroupUpLink ()->
      getHandlerUpLink ()->
        getMaximumShortNameWidth ();
*/

  // print the summary
  os << left <<
    setw (maximumSubGroupsHelpHeadersSize) <<
    fSubGroupHeader <<
    " " <<
    /* JMI
    fetchNamesInColumnsBetweenParentheses (
      maximumShortNameWidth);
      */
    fetchNamesBetweenParentheses ();

  switch (fElementVisibilityKind) {
    case kElementVisibilityNone:
      break;

    case kElementVisibilityWhole:
      break;

    case kElementVisibilityHeaderOnly:
      os <<
        " (use this option to show this subgroup)";
      break;

    case kElementVisibilityHidden:
      break;
  } // switch

  os << endl;

  // print the description if any
  if (fDescription.size ()) {
    ++gIndenter;
    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;
    --gIndenter;
  }
}

void oahSubGroup::printSubGroupSpecificHelpOrOptionsSummary (
  ostream&      os,
  S_oahSubGroup subGroup) const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    os << "oahSubGroup::printSubGroupSpecificHelpOrOptionsSummary" << endl;
  }
#endif

  // print only the summary if this is not the desired subgroup,
  // otherwise print the regular help
  if (subGroup == this) {
    os << endl;
    printSubGroupSpecificHelpOrOptionsSummary (
      os,
      subGroup);
  }
  else {
    printOptionsSummary (os); // JMI ???
  }
 }

void oahSubGroup::printSubGroupAndAtomHelp (
  ostream&  os,
  S_oahAtom targetAtom) const
{
  // print the subgroup atoms
  if (fSubGroupAtomsList.size ()) {
    ++gIndenter;

    list<S_oahAtom>::const_iterator
      iBegin = fSubGroupAtomsList.begin (),
      iEnd   = fSubGroupAtomsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahAtom atom = (*i);

      if (atom == targetAtom) {
        // print the target atom's help
        // target options atom's help
        (*i)->printHelp (os);
      }
      if (++i == iEnd) break;
    } // for

    --gIndenter;
  }
}

void oahSubGroup::findStringInSubGroup (
  string        lowerCaseString,
  list<string>& foundStringsList,
  ostream&      os) const
{
  findStringInElement (
    lowerCaseString,
    foundStringsList,
    os);

  // do this subgroups's atoms match?
  if (fSubGroupAtomsList.size ()) {
   ++gIndenter;

   for (
      list<S_oahAtom>::const_iterator i = fSubGroupAtomsList.begin ();
      i != fSubGroupAtomsList.end ();
      ++i
    ) {
      S_oahAtom atom = (*i);

      atom->
        findStringInAtom (
          lowerCaseString,
          foundStringsList,
          os);
    } // for

    --gIndenter;
  }
}

void oahSubGroup::printSubGroupOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  // print the header
  os <<
    fSubGroupHeader <<
    " " <<
    fetchNamesBetweenParentheses ();
  if (fNumberOfUserChoseAtomsInThisSubGroup > 0) {
    os <<
      ", " <<
      singularOrPlural (
        fNumberOfUserChoseAtomsInThisSubGroup, "atom chosen", "atoms chosen");
  }
  os <<
    ":" <<
    endl;

  // underline the options subgroup header
// JMI  underlineSubGroupHeader (os);

  // print the subgroup atoms values
  if (fSubGroupAtomsList.size ()) {
    ++gIndenter;

    list<S_oahAtom>::const_iterator
      iBegin = fSubGroupAtomsList.begin (),
      iEnd   = fSubGroupAtomsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the atom values
      (*i)->
        printAtomWithValueOptionsValues (
          os, valueFieldWidth);
      if (++i == iEnd) break;
  //    os << endl;
    } // for

    --gIndenter;
  }
}

ostream& operator<< (ostream& os, const S_oahSubGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahGroup oahGroup::create (
  string                   header,
  string                   shortName,
  string                   longName,
  string                   description,
  oahElementVisibilityKind optionVisibilityKind)
{
  oahGroup* o = new
    oahGroup (
      header,
      shortName,
      longName,
      description,
      optionVisibilityKind);
  assert (o != nullptr);
  return o;
}

S_oahGroup oahGroup::create (
  string                   header,
  string                   shortName,
  string                   longName,
  string                   description,
  oahElementVisibilityKind optionVisibilityKind,
  S_oahHandler             groupHandlerUpLink)
{
  oahGroup* o = new
    oahGroup (
      header,
      shortName,
      longName,
      description,
      optionVisibilityKind,
      groupHandlerUpLink);
  assert (o != nullptr);
  return o;
}

oahGroup::oahGroup (
  string                   header,
  string                   shortName,
  string                   longName,
  string                   description,
  oahElementVisibilityKind optionVisibilityKind)
  : oahElement (
      shortName,
      longName,
      description,
      kElementWithoutValue,
      optionVisibilityKind)
{
  fGroupHeader = header;

  fGroupHeaderIsToBeWritten = true; // default value

  fElementHelpOnlyKind = kElementHelpOnlyYes;

  fNumberOfUserChoseAtomsInThisGroup = 0;
}

oahGroup::oahGroup (
  string                   header,
  string                   shortName,
  string                   longName,
  string                   description,
  oahElementVisibilityKind optionVisibilityKind,
  S_oahHandler             groupHandlerUpLink)
  : oahElement (
      shortName,
      longName,
      description,
      kElementWithoutValue,
      optionVisibilityKind)
{
  fHandlerUpLink = groupHandlerUpLink;

  fGroupHeader = header;

  fGroupHeaderIsToBeWritten = true; // default value

  fNumberOfUserChoseAtomsInThisGroup = 0;
}

oahGroup::~oahGroup ()
{}

void oahGroup::setHandlerUpLink (
  S_oahHandler handlerUpLink)
{
  fHandlerUpLink = handlerUpLink;
}

void oahGroup::appendGroupToElementsList (
  S_oahHandler handler)
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Appending group \"" <<
    fGroupHeader <<
    "\" to elements list in handler \"" <<
    handler->getHandlerHeader () <<
    "\"" <<
    endl;
#endif
#endif

  ++gIndenter;

  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    handler != nullptr,
    "handler is null");

  // append this group to the elements list
  handler->
    appendElementToElementsList (this);

  // set group handler upLink
  fHandlerUpLink = handler;

  // append subgroups to elements list
  for (
    list<S_oahSubGroup>::const_iterator
      i = fGroupSubGroupsList.begin ();
    i != fGroupSubGroupsList.end ();
    ++i
  ) {
    S_oahSubGroup subGroup = (*i);

    // append subgroup to elements list
    subGroup->
      appendSubGroupToElementsList (
        handler);
  } // for

  --gIndenter;
}

void oahGroup::registerNamesInGroupToTheNamesToElementsMap (
  S_oahHandler handler)
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Registering the names in group \"" <<
    fGroupHeader <<
    "\" in the names to elements map in handler \"" <<
    handler->getHandlerHeader () <<
    "\"" <<
    endl;
#endif
#endif

  ++gIndenter;

  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    handler != nullptr,
    "handler is null");

  // register this group' names in handler
  handler->
    registerElementNamesInHandler (
      this);

  // append subgroups to elements list
  for (
    list<S_oahSubGroup>::const_iterator
      i = fGroupSubGroupsList.begin ();
    i != fGroupSubGroupsList.end ();
    ++i
  ) {
    S_oahSubGroup subGroup = (*i);

    // append subgroup to elements list
    subGroup->
      registerNamesInSubGroupToTheNamesToElementsMap (
        handler);
  } // for

  --gIndenter;
}

void  oahGroup::appendSubGroupToGroup (
  S_oahSubGroup subGroup)
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    subGroup != nullptr,
    "subGroup is null");

  // append options subgroup
  fGroupSubGroupsList.push_back (
    subGroup);

  // set options subgroup group upLink
  subGroup->
    setGroupUpLink (this);
}

S_oahElement oahGroup::fetchOptionByNameInGroup (
  string name)
{
  S_oahElement result;

  for (
    list<S_oahSubGroup>::const_iterator
      i = fGroupSubGroupsList.begin ();
    i != fGroupSubGroupsList.end ();
    ++i
  ) {
    // search name in the options group
    result =
      (*i)->fetchOptionByNameInSubGroup (name);

    if (result) {
      break;
    }
  } // for

  return result;
}

void oahGroup::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Applying element '" <<
      fetchNames () <<
      "', which is a oahGroup" <<
      endl;
  }
#endif

  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  // print the help
  gLogStream <<
    "--- Help for group \"" <<
    fGroupHeader <<
    "\" ---" <<
    endl;

  ++gIndenter;

  printHelp (os);

  --gIndenter;

  gIndenter.setIndent (saveIndent);
}

void oahGroup::handleAtomValue (
  ostream&  os,
  S_oahAtom atom,
  string    theString)
{
  os <<
    "Handling option name atom '" <<
    atom <<
    "' with value \"" <<
    theString <<
    "\" is not handled" <<
    endl;
}

void oahGroup::enforceGroupQuietness ()
{}

void oahGroup::checkGroupOptionsConsistency ()
{}

void oahGroup::checkGroupSubGroupsOptionsConsistency ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Checking the consistency of OAH group \"" <<
      fGroupHeader <<
      "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  // check the subgroups options consistency
  if (fGroupSubGroupsList.size ()) {
    for (
      list<S_oahSubGroup>::const_iterator i = fGroupSubGroupsList.begin ();
      i != fGroupSubGroupsList.end ();
      ++i
    ) {
      S_oahSubGroup subgroup = (*i);

      // check the subgroup
      subgroup->
        checkSubGroupOptionsConsistency ();
    } // for
  }

  --gIndenter;

  // check the group's own consistency
  this->checkGroupOptionsConsistency ();
}

void oahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahGroup>*
    p =
      dynamic_cast<visitor<S_oahGroup>*> (v)) {
        S_oahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahGroup>*
    p =
      dynamic_cast<visitor<S_oahGroup>*> (v)) {
        S_oahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahGroup::browseData ()" <<
      ", " << fGroupHeader <<
      endl;
  }
#endif

  // browse the subGroups
  if (fGroupSubGroupsList.size ()) {
    for (
      list<S_oahSubGroup>::const_iterator i = fGroupSubGroupsList.begin ();
      i != fGroupSubGroupsList.end ();
      ++i
    ) {
      S_oahSubGroup subGroup = (*i);

      // browse the subGroup
      oahBrowser<oahSubGroup> browser (v);
      browser.browse (*(subGroup));
    } // for
  }
}

void oahGroup::print (ostream& os) const
{
  const unsigned int fieldWidth = 27;

  os <<
    "Group:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os <<
    "SubGroupsList (" <<
    singularOrPlural (
      fGroupSubGroupsList.size (), "element",  "elements") <<
    "):" <<
    endl;

  if (fGroupSubGroupsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_oahSubGroup>::const_iterator
      iBegin = fGroupSubGroupsList.begin (),
      iEnd   = fGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;
}

void oahGroup::printShort (ostream& os) const
{
  const unsigned int fieldWidth = 27;

  os <<
    "Group: ";

  oahElement::printOahElementEssentialsShort (
    os, fieldWidth);

  if (fGroupSubGroupsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_oahSubGroup>::const_iterator
      iBegin = fGroupSubGroupsList.begin (),
      iEnd   = fGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup
      (*i)->printShort (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }
}

void oahGroup::printSummary (ostream& os) const
{
  os <<
    "Group: " <<
    fetchNames () <<
    endl;

  if (fGroupSubGroupsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_oahSubGroup>::const_iterator
      iBegin = fGroupSubGroupsList.begin (),
      iEnd   = fGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahSubGroup subGroup = (*i);

      // print a summary of the options subgroup
      subGroup->printSummary (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }
}

void oahGroup::underlineGroupHeader (ostream& os) const
{
  /* JMI
  for (unsigned int i = 0; i < fGroupHeader.size (); ++i) {
    os << "-";
  } // for
  os << endl;
  */
  os << "--------------------------" << endl;
}

void oahGroup::printGroupHeader (ostream& os) const
{
  // print the header and option names
  os <<
    fGroupHeader <<
    " " <<
    fetchNamesBetweenParentheses ();

  switch (fElementVisibilityKind) {
    case kElementVisibilityNone:
      break;

    case kElementVisibilityWhole:
      os <<
        ":";
      break;

    case kElementVisibilityHeaderOnly:
      os <<
        " (use this option to show this group)";
      break;

    case kElementVisibilityHidden:
      break;
  } // switch

  os << endl;
}

void oahGroup::printHelp (ostream& os) const
{
  if (fGroupHeaderIsToBeWritten) {
    // print the header and option names
    printGroupHeader (os);

    // print the description if any
    if (fDescription.size ()) {
      ++gIndenter;
      os <<
        gIndenter.indentMultiLineString (
          fDescription) <<
        endl;
      --gIndenter;
    }

    // underline the options group header
    underlineGroupHeader (os);
  }

  // print the options subgroups
  if (fGroupSubGroupsList.size ()) {
    // compute the maximun sub group header length
    unsigned int maximumSubGroupHeaderLength = 0;

    for (
      list<S_oahSubGroup>::const_iterator i = fGroupSubGroupsList.begin ();
      i != fGroupSubGroupsList.end ();
      ++i
    ) {
      string subGroupHeader  = (*i)->getSubGroupHeader ();
      unsigned int subGroupHeaderSize = subGroupHeader.size ();

      if (subGroupHeaderSize > maximumSubGroupHeaderLength) {
        maximumSubGroupHeaderLength = subGroupHeaderSize;
      }
    } // for

    maximumSubGroupHeaderLength += 3; // to have some more spaces

    if (fGroupHeaderIsToBeWritten) {
      ++gIndenter;
    }

    list<S_oahSubGroup>::const_iterator
      iBegin = fGroupSubGroupsList.begin (),
      iEnd   = fGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahSubGroup subGroup = (*i);

      // print the options subgroup help
      subGroup->
        printHelpWithHeaderWidth (
          os,
          maximumSubGroupHeaderLength);

      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for

    if (fGroupHeaderIsToBeWritten) {
      --gIndenter;
    }
  }
}

void oahGroup::printGroupAndSubGroupHelp (
  ostream&      os,
  S_oahSubGroup targetSubGroup) const
{
  // print the header and option names
  os <<
    fGroupHeader <<
    " " <<
    fetchNamesBetweenParentheses () <<
    ":" <<
    endl;

  // print the description if any
  if (fDescription.size ()) {
    ++gIndenter;
    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;
    --gIndenter;
  }

  // underline the options group header
  underlineGroupHeader (os);

  // print the target options subgroup
  if (fGroupSubGroupsList.size ()) {
    ++gIndenter;

    list<S_oahSubGroup>::const_iterator
      iBegin = fGroupSubGroupsList.begin (),
      iEnd   = fGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahSubGroup
        subGroup = (*i);

      if (subGroup == targetSubGroup) {
        // print the target options subgroup help
        subGroup->printSubGroupHelp (os);
      }
      if (++i == iEnd) break;
    } // for

    --gIndenter;
  }
}

void oahGroup::printGroupAndSubGroupAndAtomHelp (
  ostream&      os,
  S_oahSubGroup targetSubGroup,
  S_oahAtom     targetAtom) const
{
  // print the target options subgroup
  if (fGroupSubGroupsList.size ()) {
    list<S_oahSubGroup>::const_iterator
      iBegin = fGroupSubGroupsList.begin (),
      iEnd   = fGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahSubGroup subGroup = (*i);

      if (subGroup == targetSubGroup) {
        // print the target options subgroup's
        // target options targetAtom's help
        subGroup->
          printSubGroupAndAtomHelp (
            os,
            targetAtom);
      }

      if (++i == iEnd) break;
    } // for
  }
}

void oahGroup::printOptionsSummary (ostream& os) const
{
  // the description is the header of the information
  os <<
    fGroupHeader <<
    " " <<
    fetchNamesBetweenParentheses () <<
    ":" <<
    endl;

  // print the description if any
  if (fDescription.size ()) {
    ++gIndenter;
    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;
    --gIndenter;
  }

  // underline the options group header
  underlineGroupHeader (os);

  // print the options subgroups
  if (fGroupSubGroupsList.size ()) {
    ++gIndenter;

    list<S_oahSubGroup>::const_iterator
      iBegin = fGroupSubGroupsList.begin (),
      iEnd   = fGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup description
      (*i)->
        printOptionsSummary (os);
      if (++i == iEnd) break;
 //     os << endl;
    } // for

    --gIndenter;
  }
}

void oahGroup::printGroupAndSubGroupSpecificHelp (
  ostream&      os,
  S_oahSubGroup subGroup) const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    os << "oahGroup::printGroupAndSubGroupSpecificHelp" << endl;
  }
#endif

  switch (fElementVisibilityKind) {
    case kElementVisibilityNone:
    case kElementVisibilityWhole:
    case kElementVisibilityHeaderOnly:
      break;

    case kElementVisibilityHidden:
      // don't print specific help for this group
      return;
      break;
  } // switch

  if (fGroupHeaderIsToBeWritten) {
    // the description is the header of the information
    os <<
      fGroupHeader <<
      " " <<
      fetchNamesBetweenParentheses () <<
      ":" <<
      endl;

    // print the description if any
    if (fDescription.size ()) {
      ++gIndenter;
      os <<
        gIndenter.indentMultiLineString (
          fDescription) <<
        endl;
      --gIndenter;
    }

    // underline the options group header
    underlineGroupHeader (os);
  }

  // print the options subgroups
  if (fGroupSubGroupsList.size ()) {
    ++gIndenter;

    list<S_oahSubGroup>::const_iterator
      iBegin = fGroupSubGroupsList.begin (),
      iEnd   = fGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahSubGroup subGroup = (*i);

      // print the options subgroup specific subgroup help
      subGroup->
        printSubGroupSpecificHelpOrOptionsSummary (
          os,
          subGroup);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }
}

void oahGroup::printGroupOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  // print the header
  os <<
    fGroupHeader <<
    " " <<
    fetchNamesBetweenParentheses ();
  if (fNumberOfUserChoseAtomsInThisGroup > 0) {
    os <<
      ", " <<
      singularOrPlural (
        fNumberOfUserChoseAtomsInThisGroup, "atom chosen", "atoms chosen");
  }
  os <<
    ":" <<
    endl;

  // underline the options group header
  underlineGroupHeader (os);

  // print the options subgroups values
  if (fGroupSubGroupsList.size ()) {
    ++gIndenter;

    list<S_oahSubGroup>::const_iterator
      iBegin = fGroupSubGroupsList.begin (),
      iEnd   = fGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup values
      (*i)->
        printSubGroupOptionsValues (
          os, valueFieldWidth);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }
}

void oahGroup::findStringInGroup (
  string        lowerCaseString,
  list<string>& foundStringsList,
  ostream&      os) const
{
  switch (fElementVisibilityKind) { // JMI remove???
    case kElementVisibilityNone:
    case kElementVisibilityWhole:
    case kElementVisibilityHeaderOnly:
      break;

    case kElementVisibilityHidden:
      // don't use this groups's data to find the string
      return;
      break;
  } // switch

  findStringInElement (
    lowerCaseString,
    foundStringsList,
    os);

  // do this groups's subgroups match?
  if (fGroupSubGroupsList.size ()) {
    ++gIndenter;

    for (
      list<S_oahSubGroup>::const_iterator i = fGroupSubGroupsList.begin ();
      i != fGroupSubGroupsList.end ();
      ++i
    ) {
      S_oahSubGroup subGroup = (*i);

      subGroup->
        findStringInSubGroup (
          lowerCaseString,
          foundStringsList,
          os);
    } // for

    --gIndenter;
  }
}

ostream& operator<< (ostream& os, const S_oahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
/* this class is purely virtual
S_oahHandler oahHandler::create (
  string   executableName,
  string   handlerHeader,
  string   handlerDescription,
  string   handlerUsage)
{
  oahHandler* o = new
    oahHandler (
      executableName,
      handlerHeader,
      handlerDescription,
      handlerUsage);
  assert (o != nullptr);
  return o;
}
*/

oahHandler::oahHandler (
  string   executableName,
  string   handlerHeader,
  string   handlerDescription,
  string   handlerUsage)
{
  fHandlerExecutableName = executableName;

  fHandlerHeader = handlerHeader;

  fHandlerDescription = handlerDescription;

  fHandlerUsage = handlerUsage;

  initializeHandler ();
}

void oahHandler::initializeHandler ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Initializing handler \"" <<
    fHandlerHeader <<
    "\" for \"" <<
    fHandlerExecutableName <<
    "\"" <<
    endl;
#endif
#endif

  // arguments handling
  fNowEverythingIsAnArgument = false;

  // default values style // JMI NOT USED as of 21/11/2020
  fOahOptionsDefaultValuesStyle = kOAHStyle;

  // handler used thru...?
  fHandlerUsedThruKind = kHandlerUsedThruUnknown;

  // sizes and widths
  fMaximumSubGroupsHeadersSize = 1;

  fMaximumShortNameWidth   = 1;
  fMaximumLongNameWidth    = 1;

  fMaximumVariableNameWidth = 0;

  // has help been requested?
  fOahHandlerFoundAHelpOption = false;

/* JMI
  // initialize the optional values style kinds map
  initializeOahOptionalValuesStyleKindsMap ();

  // initialize the optional values style kind
  fHandlerOptionalValuesStyleKind = kOptionalValuesStyleGNU; // default value
  */
}

oahHandler::~oahHandler ()
{}

S_oahPrefix oahHandler::fetchNameInHandlerPrefixesMap (
  string name) const
{
  S_oahPrefix result;

  // is name known in prefixes map?
  map<string, S_oahPrefix>::const_iterator
    it =
      fHandlerPrefixesMap.find (
        name);

  if (it != fHandlerPrefixesMap.end ()) {
    // yes, name is known in the map
    result = (*it).second;
  }

  return result;
}

S_oahElement oahHandler::fetchNameInNamesToElementsMap (
  string name) const
{
  S_oahElement result;

  // is name known in the names to elements map?
  map<string, S_oahElement>::const_iterator
    it =
      fHandlerNamesToElementsMap.find (
        name);

  if (it != fHandlerNamesToElementsMap.end ()) {
    // yes, name is known in the map
    result = (*it).second;
  }

  return result;
}

void oahHandler::registerElementNamesInHandler (
  S_oahElement element)
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
if (true)
  gLogStream <<
    "Registering the names of " <<
    element->fetchNamesBetweenQuotes () <<
    " in handler \"" <<
    fHandlerHeader <<
    "\"" <<
    endl;
#endif
#endif

  ++gIndenter;

  string
    elementShortName =
      element->getShortName (),
    elementLongName =
      element->getLongName ();

  unsigned int
    elementShortNameSize =
      elementShortName.size (),
    elementLongNameSize =
      elementLongName.size ();

  // sanity checks
  if (elementShortNameSize == 0) {
    stringstream s;

    s <<
      "element short name is empty";

    oahError (s.str ());
  }

  if (elementShortNameSize == 0 && elementLongNameSize == 0) {
    stringstream s;

    s <<
      "element long name and short name are both empty";

    oahError (s.str ());
  }

  if (elementShortName == elementLongName) {
    stringstream s;

    s <<
      "element long name \"" << elementLongName << "\"" <<
      " is the same as the short name for the same";

    oahError (s.str ());
  }

  if (elementLongNameSize == 1) {
    stringstream s;

    s <<
      "element long name \"" << elementLongName << "\"" <<
      " has only one character";

    oahWarning (s.str ());
  }

  for (
    map<string, S_oahElement>::iterator i =
      fHandlerNamesToElementsMap.begin ();
    i != fHandlerNamesToElementsMap.end ();
    ++i
  ) {
    // is elementLongName already in the elements names map?
    if ((*i).first == elementLongName) {
      stringstream s;

      s <<
        "element long name \"" << elementLongName << "\"" <<
          " for element short name \"" << elementShortName << "\"" <<
          " is defined more than once in handler \"" <<
          fHandlerHeader <<
          "\"";

      oahInternalError (s.str ());
    }

    // is elementShortName already in the elements names map?
    if ((*i).first == elementShortName) {
      if (elementShortName.size ()) {
        stringstream s;

        s <<
          "element short name \"" << elementShortName << "\"" <<
          " for element long name \"" << elementLongName << "\"" <<
          " is defined more than once in handler \"" <<
          fHandlerHeader <<
          "\"";

        oahInternalError (s.str ());
      }
    }
  } // for

  if (elementShortNameSize == 1) {
    // register element's short name in single character short names set
    fSingleCharacterShortNamesSet.insert (elementShortName);
  }

  // register element's names in names to elements map
  if (elementLongNameSize) {
    fHandlerNamesToElementsMap [elementLongName] = element;

    if (elementLongNameSize > fMaximumLongNameWidth) {
      fMaximumLongNameWidth = elementLongNameSize;
    }
  }

  if (elementShortNameSize) {
    fHandlerNamesToElementsMap [elementShortName] = element;

    if (elementShortNameSize > fMaximumShortNameWidth) {
      fMaximumShortNameWidth = elementShortNameSize;
    }
  }

  // take element's display variable name length into account
  unsigned int
    elementVariableNameLength =
      element->
        fetchVariableNameLength ();

    if (
      elementVariableNameLength
        >
      fMaximumVariableNameWidth
    ) {
      fMaximumVariableNameWidth =
        elementVariableNameLength;
    }

  --gIndenter;
}

void oahHandler::registerNamesInHandlerToTheNamesToElementsMap ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Registering the names in handler \"" <<
    fHandlerHeader <<
    "\" in its names to elements map" <<
    endl;
#endif
#endif

  ++gIndenter;

  // check the handler groups options consistency
  if (fHandlerGroupsList.size ()) {
    for (
      list<S_oahGroup>::const_iterator i = fHandlerGroupsList.begin ();
      i != fHandlerGroupsList.end ();
      ++i
    ) {
      S_oahGroup group = (*i);

      // check the group
      group->
        registerNamesInGroupToTheNamesToElementsMap (
          this);
    } // for
  }


#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
if (false) // JMI
  displayNamesToElementsMapShort (gLogStream);

  gLogStream <<
    "All the names in handler \"" <<
    fHandlerHeader <<
    "\" have been registered in the names to elements map" <<
    endl;
#endif
#endif

  --gIndenter;
}

void oahHandler::appendElementToElementsList (
  S_oahElement element)
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
if (true) // JMI
  gLogStream <<
    "Registering element " <<
    element->fetchNamesBetweenQuotes () <<
    " in \"" <<
    fHandlerExecutableName <<
    "\" handler \"" <<
    fHandlerHeader <<
    "\"" <<
    endl;
#endif
#endif

  // insert element into the registered elements list
  fHandlerElementsList.push_back (element);

  // compute subgroup's header size
  if (
    // subgroup?
    S_oahSubGroup
      subGroup =
        dynamic_cast<oahSubGroup*>(&(*element))
    ) {

    string
      subHeader=
        subGroup-> getSubGroupHeader ();
    unsigned int
      subHeaderSize =
        subHeader.size ();

    // account for subgroup's header size
    if (subHeaderSize > fMaximumSubGroupsHeadersSize) {
      fMaximumSubGroupsHeadersSize =
        subHeaderSize;
    }
  }
}

void oahHandler::checkOptionsAndArgumentsFromOptionsVector ()
{
  unsigned int argumentsNumber =
    fHandlerArgumentsVector.size ();

  if (argumentsNumber > 0) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      gLogStream <<
        singularOrPluralWithoutNumber (
          argumentsNumber, "There is", "There are") <<
        " " <<
        argumentsNumber <<
        " " <<
        singularOrPluralWithoutNumber (
          argumentsNumber, "argument", "arguments") <<
        ":" <<
        endl;

      ++gIndenter;

      for (unsigned int i = 0; i < argumentsNumber; ++i) {
        gLogStream <<
          i << " : " << fHandlerArgumentsVector [i] <<
            endl;
      } // for

      gLogStream << endl;

      --gIndenter;
    }
#endif

    oahInternalError (
      "the options vector should be empty with optionsVector");
  }

  else {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      gLogStream <<
        "There are no arguments to " <<
        fHandlerExecutableName <<
        endl;
    }
#endif
  }
}

//______________________________________________________________________________
void oahHandler::createTheCommonPrefixes ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating the common prefixes in \"" <<
    fHandlerHeader <<
    "\"" <<
    endl;
#endif
#endif

  ++gIndenter;

  // the 'help' prefixes
  // --------------------------------------

  registerPrefixInHandler (
    oahPrefix::create (
      "help", "help-",
      "'-help=abc,yz' is equivalent to '-help-abc, -help-yz'"));

  registerPrefixInHandler (
    oahPrefix::create (
      "h", "h",
      "'-h=abc,wxyz' is equivalent to '-habc, -hwxyz'"));

  // the 'display' prefixes
  // --------------------------------------

  registerPrefixInHandler (
    oahPrefix::create (
      "display", "display-",
      "'-display=abc,yz' is equivalent to '-display-abc, -display-yz'"));

  registerPrefixInHandler (
    oahPrefix::create (
      "d", "d",
      "'-d=abc,wxyz' is equivalent to '-dabc, -dwxyz'"));

  // the 'omit' prefixes
  // --------------------------------------

  registerPrefixInHandler (
    oahPrefix::create (
      "omit", "omit-",
      "'-omit=abc,yz' is equivalent to '-omit-abc, -omit-yz'"));

  registerPrefixInHandler (
    oahPrefix::create (
      "o", "o",
      "'-o=abc,wxyz' is equivalent to '-oabc, -owxyz'"));

  --gIndenter;
}

//______________________________________________________________________________
void oahHandler::checkNoInputSourceInArgumentsVector () const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "checking no input source in argument vector in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  unsigned int argumentsNumber =
    fHandlerArgumentsVector.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah () && ! gGlobalGeneralOahGroup->getQuiet ()) {
    if (argumentsNumber > 0) {
      gLogStream <<
        singularOrPluralWithoutNumber (
          argumentsNumber, "There is", "There are") <<
        " " <<
        argumentsNumber <<
        " " <<
        singularOrPluralWithoutNumber (
          argumentsNumber, "argument", "arguments") <<
        " in handler arguments vector for " <<
        fHandlerExecutableName <<
        ":" <<
        endl;

      ++gIndenter;

      for (unsigned int i = 0; i < argumentsNumber; ++i) {
        gLogStream <<
          i << " : " << fHandlerArgumentsVector [i] <<
            endl;
      } // for

      gLogStream << endl;

      --gIndenter;
    }
    else {
      gLogStream <<
        "There are no arguments to " <<
        fHandlerExecutableName <<
        endl;
    }
  }
#endif

  // input source name
  // ------------------------------------------------------

  switch (argumentsNumber) {
    case 0:
      // fine, do nothing
      break;

    default:
      {
        stringstream s;

        s <<
          fHandlerExecutableName <<
          " doesn't expect arguments, only options can be used";

        string message = s.str ();

        gLogStream <<
          message <<
          endl;

        throw msgOahException (message);
      }
  } //  switch
}

//______________________________________________________________________________
void oahHandler::checkSingleInputSourceInArgumentsVector () const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "checking single input source in argument vector in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  unsigned int argumentsNumber =
    fHandlerArgumentsVector.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah () && ! gGlobalGeneralOahGroup->getQuiet ()) {
    if (argumentsNumber > 0) {
      gLogStream <<
        singularOrPluralWithoutNumber (
          argumentsNumber, "There is", "There are") <<
        " " <<
        argumentsNumber <<
        " " <<
        singularOrPluralWithoutNumber (
          argumentsNumber, "argument", "arguments") <<
        " in handler arguments vector for " <<
        fHandlerExecutableName <<
        ":" <<
        endl;

      ++gIndenter;

      for (unsigned int i = 0; i < argumentsNumber; ++i) {
        gLogStream <<
          i << " : " << fHandlerArgumentsVector [i] <<
            endl;
      } // for

      gLogStream << endl;

      --gIndenter;
    }
    else {
      gLogStream <<
        "There are no arguments to " <<
        fHandlerExecutableName <<
        endl;
    }
  }
#endif

  // input source name
  // ------------------------------------------------------

  switch (argumentsNumber) {
    case 0:
      if (! fOahHandlerFoundAHelpOption) {
        string message =
          "Input file name or '-' for standard input expected";

        gLogStream <<
          message <<
          endl;

        throw msgOahException (message);
      }
      break;

    case 1:
      // register intput file name
      gGlobalOahOahGroup->setInputSourceName (
        fHandlerArgumentsVector [0]);
      break;

    default:
      gLogStream <<
        endl <<
        "Several input file name supplied, only the first one, \"" <<
        fHandlerArgumentsVector [0] <<
        "\", will be translated" <<
        endl << endl;

      // register intput file name
      gGlobalOahOahGroup->setInputSourceName (
        fHandlerArgumentsVector [0]);
      break;
  } //  switch
}

void oahHandler::setOahHandlerFoundAHelpOption (
  string context)
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Setting oahHandlerFoundAHelpOption in handler \"" <<
    fHandlerHeader <<
    "\", context: " <<
    context <<
    endl;
#endif
#endif

  fOahHandlerFoundAHelpOption = true;
}

void oahHandler::checkHandlerOwnOptionsConsistency ()
{}

void oahHandler::checkHandlerOptionsConsistency ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePasses () || gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "oahHandler: checking the consistency of the options in \"" <<
      fHandlerExecutableName <<
      "\" handler \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  // check the handler groups options consistency
  if (fHandlerGroupsList.size ()) {
    for (
      list<S_oahGroup>::const_iterator i = fHandlerGroupsList.begin ();
      i != fHandlerGroupsList.end ();
      ++i
    ) {
      S_oahGroup group = (*i);

      // check the group
      group->
        checkGroupSubGroupsOptionsConsistency ();
    } // for
  }

  --gIndenter;

  // the the handler's own consistency
  this->checkHandlerOwnOptionsConsistency ();
}

void oahHandler::enforceHandlerQuietness ()
{}

void oahHandler::displayArgumentsVector (
  const vector<string>& theArgumentsVector)
{
  gLogStream <<
    "The arguments vector contains " <<
    singularOrPlural (
      theArgumentsVector.size (), "element", "elements");

  if (theArgumentsVector.size ()) {
    gLogStream << ":" << endl;

    ++gIndenter;

    for (unsigned int i = 0; i < theArgumentsVector.size (); ++i) {
      string theArgument  = theArgumentsVector [i];

      gLogStream <<
        right << setw (2) << i <<
        ": \"" <<
        theArgument <<
        "\"" <<
        endl;
    } //for

    gLogStream << endl;

    --gIndenter;
  }
  else {
    gLogStream << endl;
  }
}

void oahHandler::displayNamesToElementsMap ()
{
  gLogStream <<
    "The names to elements map contains " <<
    singularOrPlural (
      fHandlerNamesToElementsMap.size (), "element", "elements");

  if (fHandlerNamesToElementsMap.size ()) {
    gLogStream << ":" << endl;
    ++gIndenter;

    for (
      map<string, S_oahElement>::iterator i =
        fHandlerNamesToElementsMap.begin ();
      i != fHandlerNamesToElementsMap.end ();
      ++i
    ) {
      string name          = (*i).first;
      S_oahElement element = (*i).second;

      gLogStream <<
        "\"" <<
        name <<
        "\":" <<
        endl;

      ++gIndenter;

      gLogStream <<
        element <<
        endl;

      --gIndenter;
    } // for

    --gIndenter;
  }
  else {
    gLogStream << endl;
  }
}

void oahHandler::displayNamesToElementsMapShort (ostream& os) const
{
  gLogStream <<
    "The names to elements map contains (short version) " <<
    singularOrPlural (
      fHandlerNamesToElementsMap.size (), "element", "elements");

  if (fHandlerNamesToElementsMap.size ()) {
    gLogStream <<
      ":" <<
      endl;
    ++gIndenter;

    const unsigned int fieldWidth = 40;

    for (
      map<string, S_oahElement>::const_iterator i =
        fHandlerNamesToElementsMap.begin ();
      i != fHandlerNamesToElementsMap.end ();
      ++i
    ) {
      string       name    = (*i).first;
      S_oahElement element = (*i).second;

      gLogStream << left <<
        setw (fieldWidth) <<
        string ("\"") + name + "\"" <<
        ": " <<
        element->fetchNamesBetweenQuotes () <<
        endl;
    } // for

    --gIndenter;
  }
  else {
    gLogStream << endl;
  }
}

void oahHandler::displayElementUsesList ()
{
  gLogStream <<
    "The elements uses list contains " <<
    singularOrPlural (
      fElementUsesList.size (), "element", "elements");

  if (fElementUsesList.size ()) {
    gLogStream << ":" << endl;
    ++gIndenter;

    int counter = 0;

    for (
      list<S_oahElementUse >::const_iterator i =
        fElementUsesList.begin ();
      i != fElementUsesList.end ();
      ++i
    ) {
      S_oahElementUse elementUse = (*i);

      // sanity check
      if (false) // JMI
      msgAssert (
        __FILE__, __LINE__,
        elementUse != nullptr,
        "elementUse is null");

      ++counter;

      gLogStream <<
        right << setw (2) << counter <<
        ": ";

      if (elementUse) { // JMI
        gLogStream <<
          elementUse->asString () <<
          endl;
      }
      else {
        gLogStream <<
          "null" <<
          endl;
      }
    } // for

    gLogStream << endl;

    --gIndenter;
  }
  else {
    gLogStream << endl;
  }
}

void oahHandler::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahHandler::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahHandler>*
    p =
      dynamic_cast<visitor<S_oahHandler>*> (v)) {
        S_oahHandler elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahHandler::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahHandler::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahHandler::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahHandler>*
    p =
      dynamic_cast<visitor<S_oahHandler>*> (v)) {
        S_oahHandler elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahHandler::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahHandler::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahHandler::browseData ()" <<
      endl;
  }
#endif

  // browse the prefixes
  if (fHandlerPrefixesMap.size ()) {
    for (
      map<string, S_oahPrefix>::const_iterator i = fHandlerPrefixesMap.begin ();
      i != fHandlerPrefixesMap.end ();
      ++i
    ) {
      S_oahPrefix prefix = (*i).second;

      // browse the prefix
      oahBrowser<oahPrefix> browser (v);
      browser.browse (*(prefix));
    } // for
  }

  // browse the groups
  if (fHandlerGroupsList.size ()) {
    for (
      list<S_oahGroup>::const_iterator i = fHandlerGroupsList.begin ();
      i != fHandlerGroupsList.end ();
      ++i
    ) {
      S_oahGroup group = (*i);

      // browse the group
      oahBrowser<oahGroup> browser (v);
      browser.browse (*(group));
    } // for
  }
}

void oahHandler::printHandlerEssentials (
  ostream& os,
  unsigned int fieldWidth) const
{
  os << left <<
    "handlerExecutableName:" <<
    endl;
  ++gIndenter;
  os <<
    gIndenter.indentMultiLineString (fHandlerExecutableName) <<
    endl;
  --gIndenter;

  os << left <<
    "handlerHeader:" <<
    endl;
  ++gIndenter;
  os <<
    gIndenter.indentMultiLineString (fHandlerHeader) <<
    endl;
  --gIndenter;

  os << left <<
    "handlerDescription:" <<
    endl;
  ++gIndenter;
  os <<
    gIndenter.indentMultiLineString (fHandlerDescription) <<
    endl;
  --gIndenter;

  os << left <<
    "handlerUsage:" <<
    endl;
  ++gIndenter;
  os <<
    gIndenter.indentMultiLineString (fHandlerUsage) <<
    endl;
}

void oahHandler::print (ostream& os) const
{
  const unsigned int fieldWidth = 27;

  os <<
    "Handler:" <<
    endl;

  ++gIndenter;

  printHandlerEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "oahHandlerFoundAHelpOption" << " : " <<
    fOahHandlerFoundAHelpOption <<
    endl <<

    setw (fieldWidth) <<
    "optionsDefaultValuesStyle" << " : " <<
    optionsDefaultValuesStyleAsString (
      fOahOptionsDefaultValuesStyle) <<
    endl <<

    setw (fieldWidth) <<
    "fHandlerUsedThruKind" << " : " <<
    oahHandlerUsedThruKindAsString (
      fHandlerUsedThruKind) <<
    endl;

  // print the options prefixes if any
  if (fHandlerPrefixesMap.size ()) {
    printKnownPrefixes (os);
  }

  // print the single-character options if any
  if (fSingleCharacterShortNamesSet.size ()) {
    printKnownSingleCharacterOptions (os);
  }

  // print the options groups if any
  if (fHandlerGroupsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;
}

void oahHandler::printShort (ostream& os) const
{
  const unsigned int fieldWidth = 27;

  os <<
    "Handler:" <<
    endl;

  ++gIndenter;

  printHandlerEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "oahHandlerFoundAHelpOption" << " : " <<
    fOahHandlerFoundAHelpOption <<
    endl <<

    setw (fieldWidth) <<
    "optionsDefaultValuesStyle" << " : " <<
    optionsDefaultValuesStyleAsString (
      fOahOptionsDefaultValuesStyle) <<
    endl <<

    setw (fieldWidth) <<
    "fHandlerUsedThruKind" << " : " <<
    oahHandlerUsedThruKindAsString (
      fHandlerUsedThruKind) <<
    endl;

/* JMI
  // print the options prefixes if any
  if (fHandlerPrefixesMap.size ()) {
    printKnownPrefixes (os);
  }

  // print the single-character options if any
  if (fSingleCharacterShortNamesSet.size ()) {
    printKnownSingleCharacterOptions (os);
  }
*/

  // print the options groups if any
  if (fHandlerGroupsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group
      (*i)->printShort (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

  // print the known options
if (false) { // JMI
  os <<
    "oahHandler known options" <<
    endl;
  displayNamesToElementsMap (os);
}

  --gIndenter;
}

void oahHandler::printSummary (ostream& os) const
{
  const unsigned int fieldWidth = 27;

  os <<
    "Handler:" <<
    endl;

  ++gIndenter;

  printHandlerEssentials (
    os, fieldWidth);

  // print a summary of the options groups if any
  if (fHandlerGroupsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahGroup group = (*i);

      // print the options group
      group->printSummary (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;
}

void oahHandler::printHelp (ostream& os) const
{
  // print the options handler description
  ++gIndenter;
  os <<
    gIndenter.indentMultiLineString (
      fHandlerDescription);
  --gIndenter;

  // print the options handler usage
  os <<
    endl <<
    fHandlerUsage <<
    endl;

  os << endl;

  // print the options groups help
  if (fHandlerGroupsList.size ()) {
// JMI    ++gIndenter;

    list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_oahGroup group = (*i);

      // don't print help for empty groups,
      // temporary may 2020 until more is known JMI
      if (group->getGroupSubGroupsList ().size ()) {
        // print the options subgroups if relevant
        switch (group->getElementVisibilityKind ()) {
          case kElementVisibilityNone:
            break;

          case kElementVisibilityWhole:
            group->printHelp (os);
            break;

          case kElementVisibilityHeaderOnly:
            group->printGroupHeader (os);
            group->underlineGroupHeader(os);
            break;

          case kElementVisibilityHidden:
            break;
        } // switch
      }

      if (++i == iEnd) break;
      os << endl;
    } // for

 // JMI   --gIndenter;
  }
}

void oahHandler::printOptionsSummary (ostream& os) const
{
  // print the options handler description
  os <<
    gIndenter.indentMultiLineString (
      fHandlerDescription);

  // print the options handler help header and option names
  os <<
    fHandlerHeader <<
    ":" <<
    endl;

  // print the options handler description
  // print the options groups help summaries
  if (fHandlerGroupsList.size ()) {
    ++gIndenter;

    list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group summary
      (*i)->printOptionsSummary (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }
}

void oahHandler::printHandlerAndGroupAndSubGroupSpecificHelp (
  ostream&      os,
  S_oahSubGroup subGroup) const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    os << "oahHandler::printHandlerAndGroupAndSubGroupSpecificHelp" << endl;
  }
#endif

  // print the options handler help header and option names
  os <<
    fHandlerHeader <<
    ":" <<
    endl;

  // print the optons group subgroups specific help
  if (fHandlerGroupsList.size ()) {
    ++gIndenter;

    list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahGroup group = (*i);

      // print the options group specific subgroup help
      group->
        printGroupAndSubGroupSpecificHelp (
          os,
          subGroup);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }
}

void oahHandler::printNameIntrospectiveHelp (
  ostream& os,
  string   name)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    os << "Printing name \"" <<
      name <<
      "\" introspective help in handler \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  // a given name can be both that of a suffix and that of an option,
  // such as "h" and "help", hence:
  bool suffixHelpHasBeenProvided = false;

  if (
    // prefix?
    S_oahPrefix
      prefix =
        fetchNameInPrefixesMap (name)
  ) {
    // print the help
    int saveIndent = gIndenter.getIndent ();

    gIndenter.resetToZero ();

    gLogStream <<
      "--- Help for prefix \"" <<
      name <<
      "\" ---" <<
      endl;

    ++gIndenter;

    prefix->printHelp (os);

    --gIndenter;

    gIndenter.setIndent (saveIndent);

    suffixHelpHasBeenProvided = true;
  }

  // is name known in the elements map?
  S_oahElement
    element =
      fetchNameInNamesToElementsMap (name);

  if (element) {
    // name is known by this OAH handler

    if (
      // group?
      S_oahGroup
        group =
          dynamic_cast<oahGroup*>(&(*element))
    ) {
      int saveIndent = gIndenter.getIndent ();

      gIndenter.resetToZero ();

      // print the help
      gLogStream <<
        "--- Help for group \"" <<
        name <<
        "\" at help top level ---" <<
        endl;

      ++gIndenter;

      group->printHelp (os);

      --gIndenter;

      gIndenter.setIndent (saveIndent);
    }

    else if (
      // subgroup?
      S_oahSubGroup
        subGroup =
          dynamic_cast<oahSubGroup*>(&(*element))
    ) {
      // get the options group upLink
      S_oahGroup
        group =
          subGroup->
            getGroupUpLink ();

      int saveIndent = gIndenter.getIndent ();

      gIndenter.resetToZero ();

      // print the help
      gLogStream <<
        "--- Help for subgroup \"" <<
        name <<
        "\" in group \"" <<
        group->getGroupHeader () <<
        "\" ---" <<
        endl;

      ++gIndenter;

      group->
        printGroupAndSubGroupHelp (
          gLogStream,
          subGroup);

      --gIndenter;

      gIndenter.setIndent (saveIndent);
    }

    else if (
      // atom?
      S_oahAtom
        atom =
          dynamic_cast<oahAtom*>(&(*element))
    ) {
      // get the subgroup upLink
      S_oahSubGroup
        subGroup =
          atom->
            getSubGroupUpLink ();

      // get the group upLink
      S_oahGroup
        group =
          subGroup->
            getGroupUpLink ();

      int saveIndent = gIndenter.getIndent ();

      gIndenter.resetToZero ();

      // print the help
      gLogStream <<
        "--- Help for atom \"" <<
        name <<
        "\" in subgroup \"" <<
        subGroup->
          getSubGroupHeader () <<
        "\"";
      if (group->getGroupHeaderIsToBeWritten ()) {
        gLogStream <<
          " of group \"" <<
          group->getGroupHeader () <<
          "\" ---";
      }
      gLogStream << endl;

      ++gIndenter;

      group->
        printGroupAndSubGroupAndAtomHelp (
          gLogStream,
          subGroup,
          atom);

      --gIndenter;

      gIndenter.setIndent (saveIndent);
    }

    else {
      if (! suffixHelpHasBeenProvided) { // JMI ONH
        stringstream s;

        s <<
          "cannot provide specific help about option name \"" <<
          name <<
          "\"";

        oahInternalError (s.str ());
      }
    }
  }

  else {
    // element is not known by this OAH handler
    stringstream s;

    s <<
      "option name \"" <<
      name <<
      "\" is unknown in handler \"" <<
      fHandlerHeader <<
      "\" cannot deliver specific help";

    oahError (s.str ());
  }
}

void oahHandler::findStringInHandler (
  string        lowerCaseString,
  list<string>& foundStringsList,
  ostream&      os) const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    os << "Finding string \"" <<
      lowerCaseString <<
      "\" in handler \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  // does this handler's header match?
  bool headerMatches =
    stringToLowerCase (fHandlerHeader).find (lowerCaseString) != string::npos;

  // does this handler's description match?
  bool descriptionMatches =
    stringToLowerCase (fHandlerDescription).find (lowerCaseString) != string::npos;

  // does this handler's usage match?
  bool usageMatches =
    stringToLowerCase (fHandlerUsage).find (lowerCaseString) != string::npos;

  if (headerMatches || descriptionMatches || usageMatches) {
    stringstream s;

    s <<
      fHandlerHeader <<
      " " <<
      fHandlerDescription <<
      " " <<
      fHandlerUsage;

    // append the string
    foundStringsList.push_back (s.str ());
  }

  // do this handler's prefixes match?
  if (fHandlerPrefixesMap.size ()) {
    ++gIndenter;

    for (
      map<string, S_oahPrefix>::const_iterator i =
        fHandlerPrefixesMap.begin ();
      i != fHandlerPrefixesMap.end ();
      ++i
    ) {
      S_oahPrefix
        prefix = (*i).second;

      // does the prefix match?
      prefix->
        findStringInPrefix (
          lowerCaseString,
          foundStringsList,
          os);
    } // for

    --gIndenter;
  }

  // do this handler's groups match?
  if (fHandlerGroupsList.size ()) {
    ++gIndenter;

    for (
      list<S_oahGroup>::const_iterator i = fHandlerGroupsList.begin ();
      i != fHandlerGroupsList.end ();
      ++i
    ) {
      S_oahGroup group = (*i);

      group->
        findStringInGroup (
          lowerCaseString,
          foundStringsList,
          os);
    } // for

    --gIndenter;
  }
}

void oahHandler::printHandlerOptionsValues (
  ostream& os) const
{
  // print the options handler values header
  unsigned int
    handlerCommandLineElementsMultisetSize =
      fHandlerCommandLineElementsMultiset.size ();

  os <<
    "There are " <<
    fHandlerNamesToElementsMap.size () <<
    " known names for " <<
    fHandlerElementsList.size () <<
    " elements in \"" <<
    /* JMI
    fHandlerExecutableName <<
    "\" " <<
    */
    fHandlerHeader <<
    "\", " <<
    handlerCommandLineElementsMultisetSize <<
    " of which " <<
    singularOrPluralWithoutNumber (
      handlerCommandLineElementsMultisetSize, "occurs", "occur") <<
    " in the command line" <<
    endl;

  os <<
    "The options values for \"" <<
    fHandlerHeader <<
    "\" are:" <<
    endl;

  // print the options groups values
  if (fHandlerGroupsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahGroup group = (*i);

      // print the options group values if relevant
      switch (group->getElementVisibilityKind ()) {
        case kElementVisibilityNone:
          break;

        case kElementVisibilityWhole:
        case kElementVisibilityHeaderOnly:
          group->
            printGroupOptionsValues (
              os, fMaximumVariableNameWidth);
          break;

        case kElementVisibilityHidden:
          break;
      } // switch

      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }
}

ostream& operator<< (ostream& os, const S_oahHandler& elt)
{
  elt->print (os);
  return os;
}

void oahHandler::registerPrefixInHandler (
  S_oahPrefix prefix)
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    prefix != nullptr,
    "prefix is null");

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Registering prefix " <<
    prefix->fetchPrefixNames () <<
    " in handler \"" <<
    fHandlerHeader <<
    "\"" <<
    endl;
#endif
#endif

  string prefixName = prefix->getPrefixName ();

  // is prefixName already known in prefixes map?
  map<string, S_oahPrefix>::const_iterator
    it =
      fHandlerPrefixesMap.find (
        prefixName);

  if (it != fHandlerPrefixesMap.end ()) {
    // prefixName is already known in the map
    stringstream s;

    s <<
      " prefix name \"" <<
      prefixName <<
      "\" is already known in OAH handler \"" <<
      fHandlerHeader <<
      "\"";

    oahError (s.str ());
  }

  // register prefix in the options prefixes map
  fHandlerPrefixesMap [prefix->getPrefixName ()] =
    prefix;
}

S_oahPrefix oahHandler::fetchNameInPrefixesMap (
  string prefixName) const
{
  S_oahPrefix result;

  // is prefixName already known in prefixes map?
  map<string, S_oahPrefix>::const_iterator
    it =
      fHandlerPrefixesMap.find (
        prefixName);

  if (it != fHandlerPrefixesMap.end ()) {
    // prefixName is already known in the map
    result = (*it).second;
  }

  return result;
}

void oahHandler::appendGroupToHandler (
  S_oahGroup group)
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    group != nullptr,
    "group is null");

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Appending group \"" <<
    group->getGroupHeader () <<
    "\" to handler \"" <<
    fHandlerHeader <<
    "\"" <<
    endl;
#endif
#endif

  // append group to elements list
  group->
    appendGroupToElementsList (this);

  // append group to groups list
  fHandlerGroupsList.push_back (group);

  // set the group upLink to this handler
  group->
    setHandlerUpLink (this);
}

void oahHandler::prependGroupToHandler (
  S_oahGroup group)
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    group != nullptr,
    "group is null");

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Prepending group \"" <<
    group->getGroupHeader () <<
    "\" to handler \"" <<
    fHandlerHeader <<
    "\"" <<
    endl;
#endif
#endif

  // prepend group  to elements list
  group->
    appendGroupToElementsList (this);

  // prepend group to the groups list
  fHandlerGroupsList.push_front (group);

  // set the group upLink to this handler
  group->
    setHandlerUpLink (this);
}

void oahHandler::printKnownPrefixes (ostream& os) const
{
  unsigned int oahHandlerPrefixesListSize =
    fHandlerPrefixesMap.size ();

  os <<
    "There are " <<
    oahHandlerPrefixesListSize <<
    " options prefixes in \"" <<
    fHandlerExecutableName <<
    "\" handler \"" <<
    fHandlerHeader <<
    "\"";

  if (oahHandlerPrefixesListSize) {
    os << endl;

    ++gIndenter;

    for (
      map<string, S_oahPrefix>::const_iterator i =
        fHandlerPrefixesMap.begin ();
      i != fHandlerPrefixesMap.end ();
      ++i
    ) {
      S_oahPrefix
        prefix = (*i).second;

      os <<
        prefix->fetchPrefixNames () <<
        ":" <<
        endl <<
        gTab <<
          prefix->getPrefixDescription () <<
        endl;
    } // for

    os << endl;

    --gIndenter;
  }
  else {
    os << endl;
  }
}

void oahHandler::printKnownSingleCharacterOptions (ostream& os) const
{
  unsigned int oahHandlerPrefixesListSize =
    fSingleCharacterShortNamesSet.size ();

  os <<
    "There are " <<
    oahHandlerPrefixesListSize <<
    " single-character options in \"" <<
    fHandlerExecutableName <<
    "\" handler" <<
    fHandlerHeader <<
    ":" <<
    endl;

  // indent a bit more for readability
  ++gIndenter;

  if (oahHandlerPrefixesListSize) {
    set<string>::const_iterator
      iBegin = fSingleCharacterShortNamesSet.begin (),
      iEnd   = fSingleCharacterShortNamesSet.end (),
      i      = iBegin;

    int cumulatedLength = 0;

    for ( ; ; ) {
      string theString = (*i);

      cumulatedLength += theString.size ();
      if (cumulatedLength >= K_NAMES_LIST_MAX_LENGTH) {
        os << endl;
        cumulatedLength = 0;
        break;
      }

      os << "-" << theString;

      if (++i == iEnd) break;

      if (next (i) == iEnd) {
        os << " and ";
      }
      else {
        os << ", ";
      }
    } // for

    os << endl;
  }
  else {
    os <<
      "none" <<
      endl;
  }

  --gIndenter;

  os <<
    "They can be clustered, such as:" <<
    endl <<
    gTab << "'-vac'" <<
    endl <<
    "which is equivalent to:" <<
    endl <<
    gTab << "'-v, -a, -c'" <<
    endl;
}

string optionsDefaultValuesStyleAsString (
  oahOptionsDefaultValuesStyle optionsDefaultValuesStyle)
{
  string result;

  switch (optionsDefaultValuesStyle) {
    case kGNUStyle:
      result = "GNUStyle";
      break;
    case kOAHStyle:
      result = "OAHStyle";
      break;
  } // switch

  return result;
}

void oahHandler::printOptionsDefaultValuesInformation (ostream& os) const
{
  os <<
    endl <<
    "Some options needing a value can use a default value:" <<
    endl;

  ++gIndenter;

  os  <<
    fHandlerExecutableName <<
    gIndenter.indentMultiLineString (
R"( supports two styles for this, see '-ovs, -optional-values-style' option.)") <<
    endl;

  --gIndenter;
}

string oahHandler::commandLineAsSuppliedAsString () const
{
  return fCommandLineAsSupplied;
}

string oahHandler::commandLineWithShortNamesAsString () const
{
  stringstream s;

  s <<
    fHandlerExecutableName;

  if (fHandlerArgumentsVector.size ()) {
    s << " ";

    vector<string>::const_iterator
      iBegin = fHandlerArgumentsVector.begin (),
      iEnd   = fHandlerArgumentsVector.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  if (fHandlerCommandLineElementsList.size ()) {
    s << " ";

    list<S_oahElement>::const_iterator
      iBegin = fHandlerCommandLineElementsList.begin (),
      iEnd   = fHandlerCommandLineElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahElement element = (*i);

      s << element-> asShortNamedOptionString ();
      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  return s.str ();
}

string oahHandler::commandLineWithLongNamesAsString () const
{
  stringstream s;

  s <<
    fHandlerExecutableName;

  if (fHandlerArgumentsVector.size ()) {
    s << " ";

    vector<string>::const_iterator
      iBegin = fHandlerArgumentsVector.begin (),
      iEnd   = fHandlerArgumentsVector.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  if (fHandlerCommandLineElementsList.size ()) {
    s << " ";

    list<S_oahElement>::const_iterator
      iBegin = fHandlerCommandLineElementsList.begin (),
      iEnd   = fHandlerCommandLineElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahElement option = (*i);

      s << option-> asLongNamedOptionString ();
      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  return s.str ();
}

void oahHandler::displayNamesToElementsMap (ostream& os) const
{
  unsigned int handlerElementsMapSize =
    fHandlerNamesToElementsMap.size ();

  // print the names to elements map
  os <<
    "The " <<
    handlerElementsMapSize <<
    " known names for the " <<
    handlerElementsMapSize << // JMI
    " elements in \"" <<
    fHandlerExecutableName <<
    "\" handler \"" <<
    fHandlerHeader <<
    "\" are:" <<
    endl;

  ++gIndenter;

  if (handlerElementsMapSize) {
    map<string, S_oahElement>::const_iterator
      iBegin = fHandlerNamesToElementsMap.begin (),
      iEnd   = fHandlerNamesToElementsMap.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << (*i).first << "\" ==>" <<
        endl;

      ++gIndenter;

      (*i).second->
        printOptionHeader (
          os);

      --gIndenter;

      if (++i == iEnd) break;
    } // for
  }
  else {
    os <<
      "none" <<
      endl;
  }

  --gIndenter;

  // create a list of the options map elements names
  list<string> optionsMapElementsNamesList;

  unsigned int optionsMapElementsNamesListSize =
    optionsMapElementsNamesList.size ();

  if (optionsMapElementsNamesListSize) {
    for (
      map<string, S_oahElement>::const_iterator i = fHandlerNamesToElementsMap.begin ();
      i!= fHandlerNamesToElementsMap.end ();
      ++i
    ) {
      optionsMapElementsNamesList.push_back ((*i).first);
    } // for
  }

  // sort it
  optionsMapElementsNamesList.sort ();

  // print it
  os <<
    "The " <<
    optionsMapElementsNamesListSize <<
    " known options names for the " <<
    optionsMapElementsNamesListSize << // JMI
    " registered elements in \"" <<
    fHandlerExecutableName <<
    "\" handler " <<
    fHandlerHeader <<
    " are:" <<
    endl;

  ++gIndenter;

  if (optionsMapElementsNamesListSize) {
    list<string>::const_iterator
      iBegin = optionsMapElementsNamesList.begin (),
      iEnd   = optionsMapElementsNamesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << (*i) << "\" ==>" <<
        endl;

      if (++i == iEnd) break;
    } // for
  }
  else {
    os <<
      "none" <<
      endl;
  }

  --gIndenter;
}

S_oahElement oahHandler::fetchElementByNameInHandler (
  string name)
{
  S_oahElement result;

  for (
    list<S_oahGroup>::const_iterator i = fHandlerGroupsList.begin ();
    i != fHandlerGroupsList.end ();
    ++i
  ) {
    // search name in the options group
    result =
      (*i)->fetchOptionByNameInGroup (name);

    if (result) {
      break;
    }
  } // for

  return result;
}

void oahHandler::checkMissingPendingArgvAtomWithValueValue (
  string lastOptionNameFound,
  string context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    ++gIndenter;

    gLogStream <<
      "Checking missing pending argv atom with mandatory value \"" <<
      lastOptionNameFound <<
      "\", the pending atom with mandatory value is:";

    if (fPendingArgvAtomWithValue) {
      gLogStream << endl;
      ++gIndenter;

      gLogStream <<
        fPendingArgvAtomWithValue;

      --gIndenter;
    }
    else {
      gLogStream <<
        " null" <<
        endl;
    }

    gLogStream << endl;
    --gIndenter;
  }

/* JMI
  if (gGlobalTraceOahGroup->getTraceOahDetails ()) {
    gLogStream <<
      ", fHandlerOptionalValuesStyleKind: " <<
      oahOptionalValuesStyleKindAsString (
        fHandlerOptionalValuesStyleKind) <<
      ", fElementKind: " <<
      elementKindAsString (
        fElementKind) <<
      endl;
  }
*/
#endif

  if (fPendingArgvAtomWithValue) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      gLogStream <<
        "Checking missing value for pending atom with mandatory value \"" <<
        lastOptionNameFound <<
        "\", the name pending atom with mandatory value is: " <<
        "\"" << fNameUsedForPendingArgvAtomWithValue << "\"" <<
        endl;
    }
#endif

/* JMI BLARK
    switch (fHandlerOptionalValuesStyleKind) {
      case kOptionalValuesStyleGNU:
        // handle the valued atom using the default value
        switch (fElementValueExpectedKind) {
          case kElementValueExpectedYes:
          case kElementValueExpectedNo:
            / * JMI
              stringstream s;

              s <<
                "option name '" << atomName <<
                "' takes an optional value: it should be used with a '=' in GNU optional values style";

              oahError (s.str ());
             //   oahWarning (s.str ()); // JMI
            * /
            break;
          case kElementValueExpectedOptional:
            fPendingValuedAtom->
              handleDefaultValue ();
            break;
        } // switch
        break;

      case kOptionalValuesStyleOAH:
        // handle the valued atom using the default value
        switch (fElementValueExpectedKind) {
          case kElementValueExpectedYes:
            {
              // an option requiring a value is expecting it,
              // but another option, an argument or the end of the command line
              // has been found instead
              stringstream s;

              s <<
                "checkMissingPendingValuedAtomValue(): option " <<
               fPendingValuedAtom->asString () <<
               " expects a value" <<
               " (" << context << ")";

              oahError (s.str ());
            }
            break;
          case kElementValueExpectedNo:
            // JMI
            break;
          case kElementValueExpectedOptional:
            fPendingValuedAtom->
              handleDefaultValue ();
            break;
        } // switch
        break;
    } // switch
*/

    // forget about this pending atom with mandatory value
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      gLogStream <<
        "Forgetting about the pending argv atom with mandatory value when handling atom name \"" <<
        lastOptionNameFound <<
        "\"" <<
        endl;
    }
#endif

    fPendingArgvAtomWithValue = nullptr;
    fNameUsedForPendingArgvAtomWithValue = "";
  }

  else {
    // no atom with mandatory value is pending
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      gLogStream <<
        "No argv atom with mandatory value is pending when handling atom name \"" <<
        lastOptionNameFound <<
        "\"" <<
        endl;
    }
#endif
  }
}

void oahHandler::registerElementUse (
  S_oahElement elementUsed,
  string       nameUsed,
  string       valueUsed)
{
  // sanity check
  if (false) // JMI
  msgAssert (
    __FILE__, __LINE__,
    elementUsed != nullptr,
    "elementUsed is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Registering element use " <<
      elementUsed->fetchNamesBetweenQuotes () <<
      ", nameUsed \"" << nameUsed << "\"" <<
      ", valueUsed \"" << valueUsed << "\"" <<
      endl;
  }
#endif

  fElementUsesList.push_back (
    oahElementUse::create (
      elementUsed,
      nameUsed,
      valueUsed));

  // should we apply '-trace-oah' right now?
#ifdef TRACING_IS_ENABLED
  if (elementUsed == gGlobalTraceOahGroup->getTraceOahAtom ()) {
    gGlobalTraceOahGroup->setTraceOah ();
  }
#endif
}

void oahHandler::handleOptionPrefixName (
  string prefixName,
  size_t equalsSignPosition,
  string stringAfterEqualsSign)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Handling option prefix name \"" <<
      prefixName <<
      ", equalsSignPosition = \"" << equalsSignPosition << "\"" <<
      ", stringAfterEqualsSign = \"" << stringAfterEqualsSign << "\"" <<
      endl;
  }
#endif

  // split stringAfterEqualsSign into a list of strings
  // using the comma as separator
  list<string> chunksList;

  splitStringIntoChunks (
    stringAfterEqualsSign,
    ",",
    chunksList);

  unsigned int chunksListSize = chunksList.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "There are " << chunksListSize << " chunk[s]" <<
      " in '" << stringAfterEqualsSign <<
      "':" <<
      endl;

    ++gIndenter;

    list<string>::const_iterator
      iBegin = chunksList.begin (),
      iEnd   = chunksList.end (),
      i      = iBegin;

    for ( ; ; ) {
      gLogStream <<
        "[" << (*i) << "]";
      if (++i == iEnd) break;
      gLogStream <<
        " ";
    } // for

    gLogStream << endl;

    --gIndenter;
  }
#endif

  S_oahPrefix
    prefix =
      fetchNameInHandlerPrefixesMap (prefixName);

  if (prefix) {
    if (chunksListSize) {
      // expand the option names contained in chunksList
      for (
        list<string>::const_iterator i =
          chunksList.begin ();
        i != chunksList.end ();
        ++i
      ) {
        string singleOptionName = (*i);

        // build uncontracted option name
        string
          uncontractedOptionName =
            prefix->getPrefixErsatz () + singleOptionName;

#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceOah ()) {
          gLogStream <<
            "Expanding option \"" << singleOptionName <<
            "\" to \"" << uncontractedOptionName <<
            "\"" <<
            endl;
        }
#endif

        // handle the uncontracted option name
        handleOptionNameCommon (
          uncontractedOptionName);
      } // for
    }
  }

  else {
    stringstream s;

    s <<
      "option prefix \"" << prefixName <<
      "\" is unknown to \"" <<
        fHandlerHeader <<
        "\", see help summary below";

    printKnownPrefixes (s);

 // JMI   oahError (s.str ());
    oahWarning (s.str ());
  }
}

bool oahHandler::nameIsASingleCharacterOptionsCluster (
  string optionName)
{
  bool result = true; // until the contrary is known

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Checking whether optionName \"" <<
      optionName <<
      "\" is a single-character options cluster" <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    optionName.size () != 0,
    "optionName.size () == 0");

  ++gIndenter;

  list<S_oahElement> clusterElementsList;

  // collect single-character elements in clusterElementsList
  for (
    string::const_iterator i = optionName.begin ();
    i != optionName.end ();
    ++i
  ) {
    string singleCharacterString (1, (*i));

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOahDetails ()) {
      gLogStream <<
        "Considering single-character '" << singleCharacterString << "'" <<
        endl;
    }
    else if (gGlobalTraceOahGroup->getTraceOah ()) {
      gLogStream <<
        "'" << singleCharacterString << "' ";
    }
#endif

    set<string>::const_iterator
      it =
        fSingleCharacterShortNamesSet.find (
          singleCharacterString);

    if (it != fSingleCharacterShortNamesSet.end ()) {
      // yes, singleCharacterString is known in the set
      clusterElementsList.push_back (
        fetchElementByNameInHandler (
          singleCharacterString));
    }
    else {
      // no, singleCharacterString is not known in the set,
      // optionName is not a cluster
      result = false; // necessary if others are actually atoms short names JMI
    }
  } // for

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      gLogStream << endl;
    }
#endif

  unsigned int clusterElementsListSize =
    clusterElementsList.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "clusterElementsListSize = " << clusterElementsListSize <<
      endl;

    gLogStream <<
      "clusterElementsList:" <<
      endl;

    ++gIndenter;

    for (
      list<S_oahElement>::const_iterator i = clusterElementsList.begin ();
      i != clusterElementsList.end ();
      ++i
    ) {
      S_oahElement element = (*i);

      if (element) { // JMI should not be necessary...
        gLogStream <<
          element->
            fetchNamesBetweenQuotes () <<
          endl;
      }
    } // for

    --gIndenter;

    gLogStream <<
      "fSingleCharacterShortNamesSet:" <<
      endl;
    for (
      set<string>::const_iterator i = fSingleCharacterShortNamesSet.begin ();
      i != fSingleCharacterShortNamesSet.end ();
      ++i
    ) {
      string singleCharacterElementName = (*i);

      gLogStream <<
        "\"" <<
        singleCharacterElementName <<
        "\" ";
    } // for
    gLogStream << endl;
  }
#endif

  unsigned int optionNameSize =
    optionName.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "optionNameSize = " << optionNameSize <<
      endl;
  }
#endif

  if (
    clusterElementsListSize
      &&
    clusterElementsListSize == optionNameSize
  ) {
    // all the elements in clusterElementsList are single character options,
    // handle them
    for (
      list<S_oahElement>::const_iterator i = clusterElementsList.begin ();
      i != clusterElementsList.end ();
      ++i
    ) {
      S_oahElement element = (*i);

      // handle element name
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      if (element) { // JMI should not be necessary...
        gLogStream <<
          "handling single-character options cluster element " <<
          element->asString () <<
          endl;
      }
    }
#endif

    handleOptionNameCommon (
      element->getShortName ());
    } // for
  }

  else {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      gLogStream <<
        "OptionName \"" <<
        optionName <<
        "\" is no single-character options cluster" <<
        endl;
    }
#endif
    result = false;
  }

  --gIndenter;

  return result;
}

void oahHandler::handleNameContainingEqualSign (
  string optionName,
  size_t equalsSignPosition)
{
  // fetch the strings before and after the equals sign
  string
    stringBeforeEqualsSign =
      optionName.substr (0, equalsSignPosition);
  string
    stringAfterEqualsSign =
      optionName.substr (equalsSignPosition + 1);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Deciphering option name \"" <<
      optionName <<
      "\" that contains an  equal sign at position " <<
      equalsSignPosition <<
      ", stringBeforeEqualsSign = \"" << stringBeforeEqualsSign <<
      ", stringAfterEqualsSign = \"" << stringAfterEqualsSign << "\"" <<
      endl;
  }
#endif

  // prefixes have precedence over options with optional values
  S_oahPrefix
    prefix =
      fetchNameInHandlerPrefixesMap (
        stringBeforeEqualsSign);

  if (prefix) {
    // handle prefix name
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      gLogStream <<
        "Option name \"" <<
        stringBeforeEqualsSign <<
        "\" is a prefix" <<
        endl;
    }
#endif

    handleOptionPrefixName (
      stringBeforeEqualsSign,
      equalsSignPosition,
      stringAfterEqualsSign);
  }

  else {
    // stringBeforeEqualsSign is not the name of prefix

    // is it the name of an option?
    S_oahElement
      element =
        fetchElementByNameInHandler (
          stringBeforeEqualsSign);

    if (element) {
      // stringBeforeEqualsSign is the name of an option

      if (
        // oahStringWithDefaultValueAtom?
        S_oahStringWithDefaultValueAtom
          stringWithDefaultValueAtom =
            dynamic_cast<oahStringWithDefaultValueAtom*>(&(*element))
      ) {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTraceOah ()) {
          gLogStream <<
            "Option \"" <<
            stringBeforeEqualsSign <<
            "\" is a stringWithDefaultValueAtom" <<
            endl;
        }
#endif

        registerElementUse (
          stringWithDefaultValueAtom,
          stringBeforeEqualsSign,
          stringAfterEqualsSign);
/* JMI
        // handle the stringWithDefaultValueAtom
        switch (fHandlerOptionalValuesStyleKind) {
          case kOptionalValuesStyleGNU: // default value
            registerElementUse (
              stringWithDefaultValueAtom,
              stringBeforeEqualsSign,
              stringAfterEqualsSign);
            break;
          case kOptionalValuesStyleOAH:
            {
              stringstream s;

              s <<
                "option name \"" << stringBeforeEqualsSign <<
                "\" cannot be used with a '=' in OAH optional values style";

              oahError (s.str ());
            }
            break;
        } // switch
        */
      }

      else {
        // stringBeforeEqualsSign is not the name an a stringWithDefaultValueAtom
        stringstream s;

        s <<
          "option name \"" <<
          stringBeforeEqualsSign <<
          "\" doesn't have a default value and thus cannot be used with a '='";

        oahError (s.str ());
      }
    }

    else {
      // stringBeforeEqualsSign is not the name of an option
      stringstream s;

      s <<
        "name \"" <<
        stringBeforeEqualsSign <<
        "\" is not the name of an option";

      oahError (s.str ());
    }
  }
}

void oahHandler::handleOptionsVectorOptionName (
  string optionName,
  string valueUsed)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Handling options vector option name \"" <<
      optionName <<
      "\"" <<
      endl;
  }
#endif

  string
    trailerAfterFirstDash =
      optionName.substr (1, string::npos);

  // is optionName known in options map?
  S_oahElement
    element =
      fetchNameInNamesToElementsMap (optionName);

  if (element) {
    // optionName is known, let's handle it
#ifdef TRACING_IS_ENABLED
if (gGlobalTraceOahGroup->getTraceOah ()) {
  gLogStream <<
    "Handling option name \"" <<
    optionName <<
    "\" which is known to handler \"" <<
    fHandlerHeader <<
    "\"" <<
    endl;
}
#endif

    handleKnownOptionsVectorElementUnderName (
      element,
      optionName,
      valueUsed);
  }

  else {
    // optionName is unknown
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      if (false) // JMI
        this->printHelp (gOutputStream);

      stringstream s;

      s <<
        "option name \"" <<
        optionName <<
        "\" is unknown to handler 2 \"" <<
        fHandlerHeader <<
        "\"" <<
        endl;

//      oahError (s.str ());
      oahWarning (s.str ());
    }
#endif

    handleUnknownOptionName (
      optionName);
  }
}

string oahHandler::handleArgvOptionName (
  const string& optionName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Handling argv option name \"" <<
      optionName <<
      "\"" <<
      endl;
  }
#endif

  string currentOptionName;

  string
    trailerAfterFirstDash =
      optionName.substr (1, string::npos);

  /* JMI
  gLogStream <<
    "trailerAfterFirstDash \"" << trailerAfterFirstDash << "\" is preceded by a dash" <<
    endl;
  */

  // here, trailerAfterFirstDash.size () >= 1

  if (trailerAfterFirstDash [0] == '-') {
    // '--' has been found, optionName is a double-dashed option

    if (trailerAfterFirstDash.size () == 1) {
      // optionName is '--' alone, that marks the end of the options
      fNowEverythingIsAnArgument = true; // JMI ???

      return "";
    }

    else {
      // optionName is a double-dashed option
      currentOptionName =
        trailerAfterFirstDash.substr (1, string::npos);

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceOah ()) {
        gLogStream <<
          "\"" << currentOptionName << "\" is a double-dashed option" <<
          endl;
      }
#endif
    }
  }

  else {
    // optionName is a single-dashed option
    currentOptionName = trailerAfterFirstDash;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      gLogStream <<
        "\"" << currentOptionName << "\" is a single-dashed option" <<
        endl;
    }
#endif
  }

  // handle the current option name
  handleOptionNameCommon (
    currentOptionName);

  return currentOptionName;
}

void oahHandler::handleOptionsVectorElement (
  string optionName,
  string optionValue)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      endl <<
      "----------------------------------------------------------" <<
      endl <<
      "Handling options vector element \"" <<
      optionName <<
      "\" \"" <<
      optionValue << "\"" <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    optionName.size () > 0,
    "optionName is empty");

  // does optionName start with a dash?
  if (optionName.front () != '-') {
    stringstream s;

    s <<
      "option name \"" <<
      optionName <<
      "\" does not start with a dash";

    oahError (s.str ());
  }

  // extract the non-dash part of optionName
  size_t
    firstNonDashCharacterPosition =
      optionName.find_first_not_of ("-");

  if (firstNonDashCharacterPosition == string::npos) {
    stringstream s;

    s <<
      "option name \"" << optionName << "\" contains only dashes";

    oahError (s.str ());
  }

  string
    optionNameNonDashPart =
      optionName.substr (
        firstNonDashCharacterPosition);

  // does optionNameNonDashPart contain an equal sign?
  size_t equalsSignPosition =
    optionNameNonDashPart.find ("=");

  if (equalsSignPosition != string::npos) {
    // yes, there's an equal sign
    handleNameContainingEqualSign ( // JMI NOT HERE BECAUSE IT'S DONE AFTERWARDS IN handleOptionNameCommon() ???
      optionNameNonDashPart,
      equalsSignPosition);
  }

  else {
    // no, there's no equal sign
    // handle the current option name
      handleOptionsVectorOptionName (
        optionNameNonDashPart,
        optionValue);
  }
}

void oahHandler::handleArgvOptionValueOrArgument (
  const string& theString)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Handling argv option value or argument \"" <<
      theString <<
      "\"" <<
      endl;

    ++gIndenter;

    gLogStream <<
      "The pending atom with mandatory value is:";

    if (fPendingArgvAtomWithValue) {
      gLogStream << endl;
      ++gIndenter;

      gLogStream <<
        fPendingArgvAtomWithValue;

      --gIndenter;
    }
    else {
      gLogStream <<
        " null" <<
        endl;
    }

    gLogStream << endl;
    --gIndenter;
  }
#endif

  // options are handled at once, unless they are valued,
  // in which case the handling of the option and its value
  // are postponed until the latter is available

  if (fPendingArgvAtomWithValue) {
    // theString is the value for the pending atom with mandatory value

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
    gLogStream <<
      "The value associated to atom \"" <<
      fNameUsedForPendingArgvAtomWithValue <<
      "\" under the name \"" <<
      fNameUsedForPendingArgvAtomWithValue <<
      "\" in argv is \"" << theString << "\"" <<
      endl;
#endif
#endif

    registerElementUse (
      fPendingArgvAtomWithValue,
      fNameUsedForPendingArgvAtomWithValue,
      theString);

    // forget about this pending atom with mandatory value
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      gLogStream <<
        "handleArgvOptionValueOrArgument() Forgetting about the pending atom with mandatory value" <<
        endl;
    }
#endif

    fPendingArgvAtomWithValue = nullptr;
    fNameUsedForPendingArgvAtomWithValue = "";
  }

  else {
    // theString is an argument

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceOah ()) {
        gLogStream <<
          "\"" << theString << "\" is an argument, not an option" <<
          endl;
      }
#endif

    fHandlerArgumentsVector.push_back (theString);
  }
}

oahElementHelpOnlyKind oahHandler::handleOptionsFromOptionsVector (
  string               fakeExecutableName,
  const optionsVector& theOptionsVector)
{
  // start the clock
  clock_t startClock = clock ();

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Forcing OAH trace of vector options handling in " <<
    "\"" << fHandlerHeader << "\"" <<
    " for \"" << fHandlerExecutableName << "\"" <<
    endl;

    gGlobalTraceOahGroup->setTraceOah ();
#endif
#endif

  oahElementHelpOnlyKind result;

  // register all this handler's names in the names to elements map
  registerNamesInHandlerToTheNamesToElementsMap ();

  // create the elements uses list from the options vector
  createElementUsesListFromOptionsVector (
    fakeExecutableName,
    theOptionsVector);

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  // display the arguments vector
  displayArgumentsVector (fHandlerArgumentsVector);
#endif
#endif

  // apply the options in the elements uses list
  result =
    applyOptionsFromElementUsesList ();

  // check the options and arguments
  checkOptionsAndArgumentsFromOptionsVector ();

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "result oahElementHelpOnlyKind: " <<
    elementHelpOnlyKindAsString (result) <<
    endl;
#endif
#endif

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    "",
    "Handle the options from the options vector",
    timingItem::kMandatory,
    startClock,
    endClock);

  return result;
}

oahElementHelpOnlyKind oahHandler::handleOptionsAndArgumentsFromArgcAndArgv (
  int   argc,
  char* argv[])
{
  // start the clock
  clock_t startClock = clock ();

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Forcing OAH trace of argc/argv elements handling in \"" <<
    fHandlerHeader <<
    "\" for \"" <<
    fHandlerExecutableName <<
    "\"" <<
    endl;

    gGlobalTraceOahGroup->setTraceOah ();
#endif
#endif

  oahElementHelpOnlyKind result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "The contents of argv is:" <<
      endl;

    ++gIndenter;

    for (int i = 0; i < argc; ++i) {
      gLogStream <<
        "argv [" <<
        right << setw (2) <<
        i <<
        "]: " << argv [i] <<
        endl;
    } // for

    --gIndenter;
  }
#endif

  // register executable name
  fHandlerExecutableName = string (argv [0]);

  fCommandLineAsSupplied = fHandlerExecutableName;

  // register all this handler's names in the names to elements map
  registerNamesInHandlerToTheNamesToElementsMap ();

  // create the elements uses list from argc/argv
  createElementUsesListFromArgcAndArgv (
    argc, argv);

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  // display the arguments vector
  displayArgumentsVector (fHandlerArgumentsVector);
#endif
#endif

  // apply the options in the elements uses list
  result =
    applyOptionsFromElementUsesList ();

  // check the options and arguments
  checkOptionsAndArgumentsFromArgcAndArgv ();

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "result oahElementHelpOnlyKind: " <<
    elementHelpOnlyKindAsString (result) <<
    endl;
#endif
#endif

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    "",
    "Handle the options and arguments from argc/argv",
    timingItem::kMandatory,
    startClock,
    endClock);

  return result;
}

void oahHandler::createElementUsesListFromOptionsVector (
  string               fakeExecutableName,
  const optionsVector& theOptionsVector)
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating atom uses list from options vector in \"" <<
    fHandlerHeader <<
    "\"" <<
    endl;
#endif
#endif

  // sanity check
  switch (fHandlerUsedThruKind) {
    case kHandlerUsedThruUnknown: // default value
        fHandlerUsedThruKind =
          kHandlerUsedThruOptionsVector;
      break;
    case kHandlerUsedThruArgcAndArgv:
      {
        stringstream s;

        s <<
          "handler \"" <<
          fHandlerHeader <<
          "\" has been handled from argc/argv, cannot handle it from optionsVector";

        oahError (s.str ());
      }
      break;
    case kHandlerUsedThruOptionsVector:
      {
        stringstream s;

        s <<
          "handler \"" <<
          fHandlerHeader <<
          "\" cannot be handled twice from argc/argv";

        oahError (s.str ());
      }
      break;
  } // switch

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  displayOptionsVector (
    theOptionsVector,
    gLogStream);
#endif
#endif

  // register executable name
  fHandlerExecutableName = fakeExecutableName;

  fCommandLineAsSupplied = fHandlerExecutableName;

  // analyse the options in theOptionsVector
  if (theOptionsVector.size ()) {
    for (unsigned int i = 0; i < theOptionsVector.size (); ++i) {
      string optionName  = theOptionsVector [i].first;
      string optionValue = theOptionsVector [i].second;

      handleOptionsVectorElement (
        optionName,
        optionValue);
    } // for
  } // theOptionsVector.size ()

  else {
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
    gLogStream <<
      "The options vector is empty in \"" << fHandlerHeader << "\"" <<
      endl;
#endif
#endif
  }

  gLogStream << endl;

  --gIndenter;

  // check the options and arguments
  checkOptionsAndArgumentsFromOptionsVector ();

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  displayElementUsesList ();
#endif
#endif
}

void oahHandler::createElementUsesListFromArgcAndArgv (
  int   argc,
  char* argv[])
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
if (false) // JMI
  this->printHelp (gOutputStream);

  gLogStream <<
    "Creating atom uses list from argc/argv in \"" <<
    fHandlerHeader <<
    "\"" <<
    endl;
#endif
#endif

  // sanity check
  switch (fHandlerUsedThruKind) {
    case kHandlerUsedThruUnknown: // default value
        fHandlerUsedThruKind = kHandlerUsedThruArgcAndArgv;
      break;
    case kHandlerUsedThruArgcAndArgv:
      {
        stringstream s;

        s <<
          "handler \"" <<
          fHandlerHeader <<
          "\" cannot be handled twice from argc/argv";

        oahError (s.str ());
      }
      break;
    case kHandlerUsedThruOptionsVector:
      {
        stringstream s;

        s <<
          "handler \"" <<
          fHandlerHeader <<
          "\" has been handled from optionsVector, cannot handle it from argc/argv";

        oahError (s.str ());
      }
      break;
  } // switch

  ++gIndenter;

  // sort out the command options and the arguments
  int n = 1;

  string lastOptionNameFound;

  // loop over the contents of argv:
  while (true) {
    if (argv [n] == 0)
      break;

    string currentString = string (argv [n]);

    fCommandLineAsSupplied +=
      " " + quoteStringIfNonAlpha (currentString);


#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
    // print current option
    gLogStream <<
      endl <<
      "----------------------------------------------------------" <<
      endl <<
      "Handling argv element " <<
      n <<
      ": \"" <<
      currentString <<
      "\"" <<
      endl;
#endif
#endif

    ++gIndenter;

    // handle currentString
    if (currentString [0] == '-') {
      // currentString start with a dash: stdin or option?

      if (currentString.size () == 1) {
        // currentString is the stdin indicator, thus an argument
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
        gLogStream <<
          "\"" << currentString << "\" is the '-' stdin indicator" <<
          endl;
#endif
#endif

        // append it the the arguments vector
        fHandlerArgumentsVector.push_back (currentString);
      }

      else {
        // currentString is an option name,
        // first '-' has been found and currentString.size () >= 2

        lastOptionNameFound =
          handleArgvOptionName (
            currentString);
      }
    }

    else {
      // currentString is no option name:
      // it is an atom value or an argument

      handleArgvOptionValueOrArgument (
        currentString);
    }

    --gIndenter;

    // next please
    ++n;
  } // while

  // is a pending atom with mandatory value value missing?
  checkMissingPendingArgvAtomWithValueValue (
    lastOptionNameFound,
    "last option in command line");

  --gIndenter;

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  displayElementUsesList ();
#endif
#endif
}

oahElementHelpOnlyKind oahHandler::applyOptionsFromElementUsesList ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Applying options from elements uses list in \"" <<
    fHandlerHeader << "\"" <<
    endl;
#endif
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  displayElementUsesList ();
#endif
#endif

  if (fElementUsesList.size ()) {
    ++gIndenter;

    int counter = 0;

    for (
      list<S_oahElementUse >::const_iterator i =
        fElementUsesList.begin ();
      i != fElementUsesList.end ();
      ++i
    ) {
      S_oahElementUse elementUse = (*i);

      S_oahElement elementUsed  = elementUse->getElementUsed ();
      string       nameUsed     = elementUse->getNameUsed ();
      string       valueUsed    = elementUse->getValueUsed ();

      ++counter;

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
      gLogStream <<
        endl <<
        "----------------------------------------------------------" <<
        endl <<
        "Applying option " <<
        counter <<
        ": " <<
        elementUsed->fetchNamesBetweenParentheses () <<
        " used under name \"" <<
        nameUsed  <<
        "\" with value \"" <<
        valueUsed << "\"" <<
        endl;
#endif
#endif

      if (elementUsed) {
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
        oahElementKind
          atomValueExpectedKind =
            elementUsed->getElementKind ();

        gLogStream <<
          "Its atomValueExpectedKind is '" <<
          elementKindAsString (atomValueExpectedKind) <<
          "'" <<
          endl;
#endif
#endif

        // the output of the help should go to standard output,
        // so that the user can pipe it into a more/less command
        if (
          // group?
          S_oahGroup
            group =
              dynamic_cast<oahGroup*>(&(*elementUsed))
        ) {
          group->
            applyElement (
              gOutputStream);
        }

        else if (
          // subgroup?
          S_oahSubGroup
            subGroup =
              dynamic_cast<oahSubGroup*>(&(*elementUsed))
        ) {
          subGroup->
            applyElement (
              gOutputStream);
        }

        else if (
          // atom with value?
          S_oahAtomWithValue
            atomWithValue =
              dynamic_cast<oahAtomWithValue*>(&(*elementUsed))
        ) {
          switch (atomWithValue->getElementKind ()) {
            case kElementWithoutValue:
              {
                stringstream s;

                s <<
                  "Atom with value " <<
                  atomWithValue->fetchNamesBetweenQuotes () <<
                  " has been registered as without value";

                oahInternalError (s.str ());
              }
              break;

            case kElementWithMandatoryValue:
              if (valueUsed.size ()) {
                atomWithValue->
                  applyAtomWithValue (
                    valueUsed,
                    gOutputStream);
              }
              else {
                stringstream s;

                s <<
                  "Atom with mandatory value " <<
                  atomWithValue->fetchNamesBetweenQuotes () <<
                  " needs a non-empty value";

                oahInternalError (s.str ());
              }
              break;

            case kElementWithOptionalValue:
              if (valueUsed.size ()) {
                atomWithValue->
                  applyAtomWithValue (
                    valueUsed,
                    gOutputStream);
              }
              else {
                atomWithValue->
                  applyAtomWithValueDefaultValue (
                    gOutputStream);
              }
              break;
          } // switch
        }

        else if (
          // atom?
          S_oahAtom
            atom =
              dynamic_cast<oahAtom*>(&(*elementUsed))
        ) {
          atom->
            applyElement (
              gOutputStream);
        }

        else {
          stringstream s;

          s <<
            "cannot apply option name used \"" <<
            nameUsed <<
            "\"";

          oahError (s.str ());
        }

        // has a help-only been applied?
        switch (elementUsed->getElementHelpOnlyKind ()) {
          case kElementHelpOnlyYes:
            // a help option has been applied
            this->
              setOahHandlerFoundAHelpOption (
                elementUsed->
                  fetchNamesBetweenQuotes ());
            break;
          case kElementHelpOnlyNo:
            break;
        } // switch
      }

      else {
        stringstream s;

        s <<
          "Element from the from the atom uses list for \"" <<
          nameUsed <<
          "\" is null";

        oahInternalError (s.str ());
      }
    } // for

    --gIndenter;
  }

  // print the chosen options if so chosen
  // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getDisplayOahValues ()) {
    printHandlerOptionsValues (
      gLogStream);

    gLogStream << endl;
  }
#endif

  // check the consistency of the options
  // ------------------------------------------------------

  checkHandlerOptionsConsistency ();

  // was this run a 'help-only' one?
  // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    if (fOahHandlerFoundAHelpOption) {
      gLogStream <<
        "This is a help-only run " <<
        endl;

      return kElementHelpOnlyYes;
    }
    else {
      gLogStream <<
        "This is an actual translation run " <<
        endl;

      // let's go ahead
    }
  }
#endif

  // store the command line with options in gGlobalOahOahGroup
  // for whoever need them
  // ------------------------------------------------------

  gGlobalOahOahGroup->getCommandLineAsSupplied () =
    commandLineAsSuppliedAsString ();
  gGlobalOahOahGroup->getCommandLineWithShortOptionsNames () =
    commandLineWithShortNamesAsString ();
  gGlobalOahOahGroup->getCommandLineWithLongOptionsNames () =
    commandLineWithLongNamesAsString ();

  --gIndenter;

  return
    fOahHandlerFoundAHelpOption
      ? kElementHelpOnlyYes
      : kElementHelpOnlyNo;
}

void oahHandler::handleKnownOptionsVectorElementUnderName (
  S_oahElement element,
  string       optionNameUsed,
  string       valueUsed)
{

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Handling argv known element " <<
      element->fetchNamesBetweenQuotes () <<
      " under name \"" <<
      optionNameUsed <<
      "\"" <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Element \"" <<
      element->fetchNamesBetweenQuotes () <<
      "\" is described by: " <<
      endl <<
      gTab <<
      element->getDescription () <<
      endl;
  }
#endif

  // is this element already present in the commande line?
  multiset<S_oahElement, compareOahElements>::const_iterator
    it =
      fHandlerCommandLineElementsMultiset.find (
        element);

  if (it != fHandlerCommandLineElementsMultiset.end ()) {
    // yes, element is known in the list
    if (! element->getMultipleOccurrencesAllowed ()) {
      stringstream s;

      s <<
        "element " <<
        element->fetchNamesBetweenQuotes () <<
        " is already present in the command line";

      oahWarning (s.str ());
    }
  }

  // remember this element as occurring in the command line
  fHandlerCommandLineElementsList.push_back (element);
  fHandlerCommandLineElementsMultiset.insert (element);

  // determine element short and long names to be used,
  // in case one of them (short or long) is empty
  string
    shortName =
      element->getShortName (),
    longName =
      element->getLongName ();

  string
    shortNameToBeUsed =
      shortName,
    longNameToBeUsed =
      longName;

  // replace empty element name if any by the other one,
  // since they can't both be empty
  if (! shortNameToBeUsed.size ()) {
    shortNameToBeUsed = longNameToBeUsed;
  }
  if (! longNameToBeUsed.size ()) {
    longNameToBeUsed = shortNameToBeUsed;
  }

  // handle the option
  if (
    // options group?
    S_oahGroup
      group =
        dynamic_cast<oahGroup*>(&(*element))
  ) {
    registerElementUse (
      group, optionNameUsed, ""); // "===group==="); // JMI to debug
  }

  else if (
    // options subgroup?
    S_oahSubGroup
      subGroup =
        dynamic_cast<oahSubGroup*>(&(*element))
  ) {
    registerElementUse (
      subGroup, optionNameUsed, ""); // "===subGroup==="); // JMI to debug
  }

  else if (
    // atom synonym?
    S_oahAtomSynonym
      atomSynonym =
        dynamic_cast<oahAtomSynonym*>(&(*element))
  ) {
    // use the original atom instead
    S_oahAtom
      originalOahAtom =
        atomSynonym->getOriginalOahAtom ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      gLogStream <<
        "Atom name \"" << optionNameUsed << "\" is a synonym for \"" <<
        originalOahAtom->asString () <<
        "\", handling the latter" <<
        endl;
    }
#endif

    handleKnownOptionsVectorAtomUnderName (
      originalOahAtom,
      optionNameUsed,
      valueUsed);
  }

  else if (
    // atom?
    S_oahAtom
      atom =
        dynamic_cast<oahAtom*>(&(*element))
  ) {
    handleKnownOptionsVectorAtomUnderName (
      atom,
      optionNameUsed,
      valueUsed);
  }

  else {
    stringstream s;

    s <<
      "Known option name \"" <<
      optionNameUsed <<
      "\" with value \"" <<
      valueUsed <<
      "\" cannot be handled";

    oahInternalError (s.str ());
  }
}

void oahHandler::handleKnownArgvElementUnderName (
  S_oahElement element,
  string       optionNameUsed)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Handling argv known element " <<
      element->fetchNamesBetweenQuotes () <<
      " under name \"" <<
      optionNameUsed <<
      "\"" <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Element " <<
      element->fetchNamesBetweenQuotes () <<
      " is described by: " <<
      endl <<
      gTab <<
      element->getDescription () <<
      endl;
  }
#endif

  // is this element already present in the commande line?
  multiset<S_oahElement, compareOahElements>::const_iterator
    it =
      fHandlerCommandLineElementsMultiset.find (
        element);

  if (it != fHandlerCommandLineElementsMultiset.end ()) {
    // yes, element is known in the list
    if (! element->getMultipleOccurrencesAllowed ()) {
      stringstream s;

      s <<
        "element " <<
        element->fetchNamesBetweenQuotes () <<
        " is already present in the command line";

      oahWarning (s.str ());
    }
  }

  // remember this element as occurring in the command line
  fHandlerCommandLineElementsList.push_back (element);
  fHandlerCommandLineElementsMultiset.insert (element);

  // determine element short and long names to be used,
  // in case one of them (short or long) is empty
  string
    shortName =
      element->getShortName (),
    longName =
      element->getLongName ();

  string
    shortNameToBeUsed =
      shortName,
    longNameToBeUsed =
      longName;

  // replace empty element name if any by the other one,
  // since they can't both be empty
  if (! shortNameToBeUsed.size ()) {
    shortNameToBeUsed = longNameToBeUsed;
  }
  if (! longNameToBeUsed.size ()) {
    longNameToBeUsed = shortNameToBeUsed;
  }

  // handle the option
  if (
    // options group?
    S_oahGroup
      group =
        dynamic_cast<oahGroup*>(&(*element))
  ) {
    registerElementUse (
      group, optionNameUsed, "group FYY");
  }

  else if (
    // options subgroup?
    S_oahSubGroup
      subGroup =
        dynamic_cast<oahSubGroup*>(&(*element))
  ) {
    registerElementUse (
      subGroup, optionNameUsed, "subGroup FII");
  }

  else if (
    // atom synonym?
    S_oahAtomSynonym
      atomSynonym =
        dynamic_cast<oahAtomSynonym*>(&(*element))
  ) {
    // use the original atom instead
    S_oahAtom
      originalOahAtom =
        atomSynonym->getOriginalOahAtom ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      gLogStream <<
        "Atom name \"" << optionNameUsed << "\" is a synonym for \"" <<
        originalOahAtom->asString () <<
        "\", handling the latter" <<
        endl;
    }
#endif

    handleKnownArgvAtomUnderName (
      originalOahAtom,
      optionNameUsed);
  }

  else if (
    // atom?
    S_oahAtom
      atom =
        dynamic_cast<oahAtom*>(&(*element))
  ) {
    handleKnownArgvAtomUnderName (
      atom, optionNameUsed);
  }

  else {
    stringstream s;

    s <<
      "Known option name \"" << optionNameUsed << "\" cannot be handled";

    oahInternalError (s.str ());
  }
}

void oahHandler::handleKnownOptionsVectorAtomUnderName (
  S_oahAtom atom,
  string    optionNameUsed,
  string    valueUsed)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Handling known options vector atom " <<
      atom->fetchNamesBetweenQuotes () <<
      " under name \"" <<
      optionNameUsed <<
      "\"" <<
      endl;
  }
#endif

  switch (atom->getElementKind ()) {
    case kElementWithoutValue:
      registerElementUse (
        atom, optionNameUsed, ""); // "===options vector atom without value==="); // JMI to debug
      break;

    case kElementWithMandatoryValue:
      if (
        // argv atom with mandatory value?
        S_oahAtomWithValue
          atomWithValue =
            dynamic_cast<oahAtomWithValue*>(&(*atom))
      ) {
        registerElementUse (
          atom, optionNameUsed, valueUsed);
      }

      else {
        stringstream s;

        s <<
          "argv atom " <<
          atom->fetchNamesBetweenQuotes () <<
          " is not a atom with mandatory value though its kind is kElementWithMandatoryValue";

        oahInternalError (s.str ());
      }
      break;

    case kElementWithOptionalValue:
      registerElementUse (
        atom, optionNameUsed, ""); // "===atom with optional value===" JMI to debug
      break;
  } // switch
}

void oahHandler::handleKnownArgvAtomUnderName (
  S_oahAtom atom,
  string    optionNameUsed)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Handling known argv atom " <<
      atom->fetchNamesBetweenQuotes () <<
      " under name \"" <<
      optionNameUsed <<
      "\"" <<
      endl;
  }
#endif

  if (fPendingArgvAtomWithValue) {
    stringstream s;

    s <<
      "Pending argv atom with mandatory value " <<
      fPendingArgvAtomWithValue->fetchNamesBetweenQuotes () <<
      " used under name \"" <<
      fNameUsedForPendingArgvAtomWithValue <<
      "\" expects a value" <<
      endl;

    oahError (s.str ());
  }

  switch (atom->getElementKind ()) {
    case kElementWithoutValue:
      registerElementUse (
        atom, optionNameUsed, ""); // "===argv atom without value==="); // JMI to debug

      fPendingArgvAtomWithValue = nullptr;
      fNameUsedForPendingArgvAtomWithValue = "";
      break;

    case kElementWithMandatoryValue:
      if (
        // argv atom with mandatory value?
        S_oahAtomWithValue
          atomWithValue =
            dynamic_cast<oahAtomWithValue*>(&(*atom))
      ) {
        // delay this argv atom's handling until the value is handled
        fPendingArgvAtomWithValue =
          atomWithValue;
        fNameUsedForPendingArgvAtomWithValue =
          optionNameUsed;
      }

      else {
        stringstream s;

        s <<
          "argv atom " <<
          atom->fetchNamesBetweenQuotes () <<
          " is not a atom with mandatory value though its kind is kElementWithMandatoryValue";

        oahInternalError (s.str ());
      }
      break;

    case kElementWithOptionalValue:
      registerElementUse (
        atom, optionNameUsed, ""); // "===use default value===" // JMI to debug

      fPendingArgvAtomWithValue = nullptr;
      fNameUsedForPendingArgvAtomWithValue = "";
      break;
  } // switch
}

void oahHandler::handleUnknownOptionName (
  string optionName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Handling unknown option name \"" <<
      optionName <<
      "\"" <<
      endl;
  }
#endif

  // is optionName an options cluster?
  bool nameIsAnOptionsCluster = false;

  if (optionName.size () > 1) {
    // is optionName a single-character options cluster?
    // if so, the options contained in it should be handled
    nameIsAnOptionsCluster =
      nameIsASingleCharacterOptionsCluster (
        optionName);
  }

  if (nameIsAnOptionsCluster) {
    // the cluster has just been handled, nothing more here
  }
  else {
    // optionName is unknown to this handler
    stringstream s;

    s <<
      "option name \"" <<
      optionName <<
      "\" is unknown to \"" <<
      fHandlerHeader <<
      "\"";

    handlerOahError (s.str ());
  }
}

void oahHandler::handleOptionNameCommon (
  const string& optionName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Handling valueless option name \"" <<
      optionName <<
      "\"" <<
      endl;
  }
#endif

  // does optionName contain an equal sign?
  size_t equalsSignPosition =
    optionName.find ("=");

  if (equalsSignPosition != string::npos) {
    // yes, there's an equal sign
    handleNameContainingEqualSign (
      optionName,
      equalsSignPosition);
  }

  else {
    // is optionName known in options map?
    S_oahElement
      element =
        fetchNameInNamesToElementsMap (optionName);

    if (element) {
      // optionName is known, let's handle it
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Handling option name \"" <<
      optionName <<
      "\" is known to handler \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

      handleKnownArgvElementUnderName (
        element,
        optionName);
    }

    else {
      // optionName is unknown
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceOahDetails ()) {
        if (false) // JMI
          this->printHelp (gOutputStream);

        stringstream s;

        s <<
          "option name \"" <<
          optionName <<
          "\" is unknown to handler 3 \"" <<
          fHandlerHeader <<
          "\"" <<
          endl;

  //      oahError (s.str ());
        oahWarning (s.str ());
      }
#endif

      handleUnknownOptionName (
        optionName);
    }
  }
}


}

/* JMI
// optional values style
//______________________________________________________________________________

map<string, oahOptionalValuesStyleKind>
  gGlobalOahOptionalValuesStyleKindsMap;

string oahOptionalValuesStyleKindAsString (
  oahOptionalValuesStyleKind optionalValuesStyleKind)
{
  string result;

  // no CamelCase here, these strings are used in the command line options

  switch (optionalValuesStyleKind) {
    case kOptionalValuesStyleGNU: // default value
      result = "gnu";
      break;
    case kOptionalValuesStyleOAH:
      result = "oah";
      break;
  } // switch

  return result;
}

void initializeOahOptionalValuesStyleKindsMap ()
{
  // register the optional values style kinds
  // --------------------------------------

  // no CamelCase here, these strings are used in the command line options

  gGlobalOahOptionalValuesStyleKindsMap ["gnu"] = kOptionalValuesStyleGNU;
  gGlobalOahOptionalValuesStyleKindsMap ["oah"] = kOptionalValuesStyleOAH;
}

string existingOahOptionalValuesStyleKinds (unsigned int namesListMaxLength)
{
  stringstream s;

  unsigned int
    optionalValuesStyleKindsMapSize =
      gGlobalOahOptionalValuesStyleKindsMap.size ();

  if (optionalValuesStyleKindsMapSize) {
    int
      nextToLast =
        optionalValuesStyleKindsMapSize - 1;

    int count = 0;
    int cumulatedLength = 0;

    for (
      map<string, oahOptionalValuesStyleKind>::const_iterator i =
        gGlobalOahOptionalValuesStyleKindsMap.begin ();
      i != gGlobalOahOptionalValuesStyleKindsMap.end ();
      ++i
    ) {
      string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
      }

      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != optionalValuesStyleKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}
*/
