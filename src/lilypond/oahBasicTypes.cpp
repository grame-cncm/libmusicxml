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

#include <climits>      // INT_MIN, INT_MAX
#include <iomanip>      // setw, setprecision, ...

#include <regex>

#include "utilities.h"

#include "oahBasicTypes.h"
#include "oahOah.h"

#include "messagesHandling.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif


namespace MusicXML2
{

//______________________________________________________________________________
S_oahAtom oahAtom::create (
  string shortName,
  string longName,
  string description)
{
  oahAtom* o = new
    oahAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

oahAtom::oahAtom (
  string shortName,
  string longName,
  string description)
  : oahElement (
      shortName,
      longName,
      description,
      kElementVisibilityAlways)
{}

oahAtom::~oahAtom ()
{}

void oahAtom::setSubGroupUpLink (
  S_oahSubGroup subGroup)
{
  // sanity check
  msrAssert (
    subGroup != nullptr,
    "subGroup is null");

  // set the upLink
  fSubGroupUpLink = subGroup;
}

void oahAtom::registerAtomInHandler (
  S_oahHandler handler)
{
  handler->
    registerElementInHandler (this);

  fHandlerUpLink = handler;
}

void oahAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahAtom>*
    p =
      dynamic_cast<visitor<S_oahAtom>*> (v)) {
        S_oahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahAtom>*
    p =
      dynamic_cast<visitor<S_oahAtom>*> (v)) {
        S_oahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahAtom::browseData ()" <<
      endl;
  }
#endif
}

void oahAtom::print (ostream& os) const
{
  const int fieldWidth = 19;

  os <<
    "Atom ???:" <<
      endl;

  gIndenter++;

  oahElement::printOptionEssentials (
    os, fieldWidth);

  gIndenter--;
}

void oahAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
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
  // should go after all non null S_oahAtom values

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
S_oahAtomSynonym oahAtomSynonym::create (
  string    shortName,
  string    longName,
  string    description,
  S_oahAtom originalOahAtom)
{
  oahAtomSynonym* o = new
    oahAtomSynonym (
      shortName,
      longName,
      description,
      originalOahAtom);
  assert(o!=0);
  return o;
}

oahAtomSynonym::oahAtomSynonym (
  string    shortName,
  string    longName,
  string    description,
  S_oahAtom originalOahAtom)
  : oahAtom (
      shortName,
      longName,
      description)
{
  // sanity check
  msrAssert (
    originalOahAtom != nullptr,
    "originalOahAtom is null");

  fOriginalOahAtom = originalOahAtom;
}

oahAtomSynonym::~oahAtomSynonym ()
{}

void oahAtomSynonym::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahAtomSynonym::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahAtomSynonym>*
    p =
      dynamic_cast<visitor<S_oahAtomSynonym>*> (v)) {
        S_oahAtomSynonym elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahAtomSynonym::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahAtomSynonym::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahAtomSynonym::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahAtomSynonym>*
    p =
      dynamic_cast<visitor<S_oahAtomSynonym>*> (v)) {
        S_oahAtomSynonym elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahAtomSynonym::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahAtomSynonym::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahAtomSynonym::browseData ()" <<
      endl;
  }
#endif

  if (fOriginalOahAtom) {
    // browse the original atom
    oahBrowser<oahAtom> browser (v);
    browser.browse (*fOriginalOahAtom);
  }
}

void oahAtomSynonym::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "AtomSynonym:" <<
    endl;

  gIndenter++;

  oahElement::printOptionEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void oahAtomSynonym::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_oahAtomSynonym& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahOptionsUsageAtom oahOptionsUsageAtom::create (
  string shortName,
  string longName,
  string description)
{
  oahOptionsUsageAtom* o = new
    oahOptionsUsageAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

oahOptionsUsageAtom::oahOptionsUsageAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description)
{}

oahOptionsUsageAtom::~oahOptionsUsageAtom ()
{}

S_oahValuedAtom oahOptionsUsageAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahOptionsUsageAtom" <<
      endl;
  }
#endif

  // handle it at once
  os <<
    endl <<
    "Options usage" <<
    endl <<
    "-------------" <<
    endl <<
    endl;

  gIndenter++;

  os <<
    gIndenter.indentMultiLineString (
R"(As an argument, '-' represents standard input.

A number of options exist to fine tune the generated LilyPond code
and limit the need for manually editing the latter.
Most options have a short and a long name for commodity.

The options are organized in a group-subgroup-atom hierarchy.
Help can be obtained for groups or subgroups at will,
as well as for any option with the '-onh, option-name-help' option.

A subgroup can be hidden by default, in which case its description is printed
only when the corresponding short or long names are used.

Both '-' and '--' can be used to introduce options in the command line,
even though the help facility only shows them with '-'.

Command line options and arguments can be placed in any order,
provided atom values immediately follow the corresponding atoms.

Some prefixes allow shortcuts, such as '-t=voices,meas' for '-tvoices, -tmeas')") <<
    endl <<
    endl;

  gIndenter--;

  // no option value is needed
  return nullptr;
}

void oahOptionsUsageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahOptionsUsageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahOptionsUsageAtom>*
    p =
      dynamic_cast<visitor<S_oahOptionsUsageAtom>*> (v)) {
        S_oahOptionsUsageAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahOptionsUsageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahOptionsUsageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahOptionsUsageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahOptionsUsageAtom>*
    p =
      dynamic_cast<visitor<S_oahOptionsUsageAtom>*> (v)) {
        S_oahOptionsUsageAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahOptionsUsageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahOptionsUsageAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahOptionsUsageAtom::browseData ()" <<
      endl;
  }
#endif
}

void oahOptionsUsageAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsUsageAtom:" <<
    endl;

  gIndenter++;

  oahElement::printOptionEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

/*
void oahOptionsUsageAtom::printOptionsUsage (ostream& os) const
{
  os <<
    endl <<
    "Options usage" <<
    endl <<
    "-------------" <<
    endl <<
    endl;

  gIndenter++;

  os <<
    gIndenter.indentMultiLineString (
R"(As an argument, '-' represents standard input.

A number of options exist to fine tune the generated LilyPond code
and limit the need for manually editing the latter.
Most options have a short and a long name for commodity.

The options are organized in a group-subgroup-atom hierarchy.
Help can be obtained for groups or subgroups at will,
as well as for any option with the '-onh, option-name-help' option.

A subgroup can be hidden by default, in which case its description is printed
only when the corresponding short or long names are used.

Both '-' and '--' can be used to introduce options in the command line,
even though the help facility only shows them with '-'.

Command line options and arguments can be placed in any order,
provided atom values immediately follow the corresponding atoms.)") <<
    endl <<
    endl;

  gIndenter--;
}
*/

void oahOptionsUsageAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_oahOptionsUsageAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahOptionsSummaryAtom oahOptionsSummaryAtom::create (
  string shortName,
  string longName,
  string description)
{
  oahOptionsSummaryAtom* o = new
    oahOptionsSummaryAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

oahOptionsSummaryAtom::oahOptionsSummaryAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description)
{}

oahOptionsSummaryAtom::~oahOptionsSummaryAtom ()
{}

S_oahValuedAtom oahOptionsSummaryAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
     "==> option '" << optionName << "' is a oahOptionsSummaryAtom" <<
      endl;
  }
#endif

  print (os); //JMI ???

  // no option value is needed
  return nullptr;
}

void oahOptionsSummaryAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahOptionsSummaryAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahOptionsSummaryAtom>*
    p =
      dynamic_cast<visitor<S_oahOptionsSummaryAtom>*> (v)) {
        S_oahOptionsSummaryAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahOptionsSummaryAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahOptionsSummaryAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahOptionsSummaryAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahOptionsSummaryAtom>*
    p =
      dynamic_cast<visitor<S_oahOptionsSummaryAtom>*> (v)) {
        S_oahOptionsSummaryAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahOptionsSummaryAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahOptionsSummaryAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahOptionsSummaryAtom::browseData ()" <<
      endl;
  }
#endif
}

void oahOptionsSummaryAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsSummaryAtom:" <<
    endl;

  gIndenter++;

  oahElement::printOptionEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void oahOptionsSummaryAtom::printOptionsSummary (ostream& os) const
{
  os <<
    gOahOah->fHandlerExecutableName <<
    endl;
}

void oahOptionsSummaryAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_oahOptionsSummaryAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
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
  assert(o!=0);
  return o;
}

oahAtomWithVariableName::oahAtomWithVariableName (
  string shortName,
  string longName,
  string description,
  string variableName)
  : oahAtom (
      shortName,
      longName,
      description),
    fVariableName (
      variableName)
{}

oahAtomWithVariableName::~oahAtomWithVariableName ()
{}

void oahAtomWithVariableName::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahAtomWithVariableName::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahAtomWithVariableName>*
    p =
      dynamic_cast<visitor<S_oahAtomWithVariableName>*> (v)) {
        S_oahAtomWithVariableName elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahAtomWithVariableName::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahAtomWithVariableName::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahAtomWithVariableName::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahAtomWithVariableName>*
    p =
      dynamic_cast<visitor<S_oahAtomWithVariableName>*> (v)) {
        S_oahAtomWithVariableName elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahAtomWithVariableName::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahAtomWithVariableName::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahAtomWithVariableName::browseData ()" <<
      endl;
  }
#endif
}

void oahAtomWithVariableName::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "AtomWithVariableName:" <<
    endl;

  gIndenter++;

  oahElement::printOptionEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;
  gIndenter--;

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<
    endl;

  gIndenter--;
}

void oahAtomWithVariableName::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
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
S_oahBooleanAtom oahBooleanAtom::create (
  string shortName,
  string longName,
  string description,
  string variableName,
  bool&  booleanVariable)
{
  oahBooleanAtom* o = new
    oahBooleanAtom (
      shortName,
      longName,
      description,
      variableName,
      booleanVariable);
  assert(o!=0);
  return o;
}

oahBooleanAtom::oahBooleanAtom (
  string shortName,
  string longName,
  string description,
  string variableName,
  bool&  booleanVariable)
  : oahAtomWithVariableName (
      shortName,
      longName,
      description,
      variableName),
    fBooleanVariable (
      booleanVariable)
{}

oahBooleanAtom::~oahBooleanAtom ()
{}

S_oahValuedAtom oahBooleanAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
     "==> option '" << optionName << "' is a oahBooleanAtom" <<
      endl;
  }
#endif

  // handle it at once
  setBooleanVariable (true);

  // no option value is needed
  return nullptr;
}

void oahBooleanAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahBooleanAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahBooleanAtom>*
    p =
      dynamic_cast<visitor<S_oahBooleanAtom>*> (v)) {
        S_oahBooleanAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahBooleanAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahBooleanAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahBooleanAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahBooleanAtom>*
    p =
      dynamic_cast<visitor<S_oahBooleanAtom>*> (v)) {
        S_oahBooleanAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahBooleanAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahBooleanAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahBooleanAtom::browseData ()" <<
      endl;
  }
#endif
}

void oahBooleanAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "BooleanAtom:" <<
    endl;

  gIndenter++;

  oahElement::printOptionEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;
  gIndenter--;

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "fBooleanVariable" << " : " <<
    booleanAsString (fBooleanVariable) <<
    endl;

  gIndenter--;
}

void oahBooleanAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    booleanAsString (fBooleanVariable) <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahBooleanAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahTwoBooleansAtom oahTwoBooleansAtom::create (
  string shortName,
  string longName,
  string description,
  string variableName,
  bool&  booleanVariable,
  bool&  booleanSecondaryVariable)
{
  oahTwoBooleansAtom* o = new
    oahTwoBooleansAtom (
      shortName,
      longName,
      description,
      variableName,
      booleanVariable,
      booleanSecondaryVariable);
  assert(o!=0);
  return o;
}

oahTwoBooleansAtom::oahTwoBooleansAtom (
  string shortName,
  string longName,
  string description,
  string variableName,
  bool&  booleanVariable,
  bool&  booleanSecondaryVariable)
  : oahBooleanAtom (
      shortName,
      longName,
      description,
      variableName,
      booleanVariable),
    fBooleanSecondaryVariable (
      booleanSecondaryVariable)
{}

oahTwoBooleansAtom::~oahTwoBooleansAtom ()
{}

S_oahValuedAtom oahTwoBooleansAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
     "==> option '" << optionName << "' is a oahTwoBooleansAtom" <<
      endl;
  }
#endif

  // handle it at once
  setTwoBooleansVariables (true);

  // no option value is needed
  return nullptr;
}

void oahTwoBooleansAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahTwoBooleansAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahTwoBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahTwoBooleansAtom>*> (v)) {
        S_oahTwoBooleansAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahTwoBooleansAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahTwoBooleansAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahTwoBooleansAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahTwoBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahTwoBooleansAtom>*> (v)) {
        S_oahTwoBooleansAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahTwoBooleansAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahTwoBooleansAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahTwoBooleansAtom::browseData ()" <<
      endl;
  }
#endif
}

void oahTwoBooleansAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "TwoBooleansAtom:" <<
    endl;

  gIndenter++;

  oahElement::printOptionEssentials (
    os, fieldWidth);

  os <<
    setw (fieldWidth) <<
    "fDescription" << " : " <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "fBooleanVariable" << " : " <<
    booleanAsString (
      fBooleanVariable) <<
    endl <<
    setw (fieldWidth) <<
    "fBooleanSecondaryVariable" << " : " <<
    booleanAsString (
      fBooleanSecondaryVariable) <<
    endl;

  gIndenter--;
}

void oahTwoBooleansAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    booleanAsString (fBooleanVariable) <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahTwoBooleansAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahThreeBooleansAtom oahThreeBooleansAtom::create (
  string shortName,
  string longName,
  string description,
  string variableName,
  bool&  booleanVariable,
  bool&  booleanSecondaryVariable,
  bool&  booleanTertiaryVariable)
{
  oahThreeBooleansAtom* o = new
    oahThreeBooleansAtom (
      shortName,
      longName,
      description,
      variableName,
      booleanVariable,
      booleanSecondaryVariable,
      booleanTertiaryVariable);
  assert(o!=0);
  return o;
}

oahThreeBooleansAtom::oahThreeBooleansAtom (
  string shortName,
  string longName,
  string description,
  string variableName,
  bool&  booleanVariable,
  bool&  booleanSecondaryVariable,
  bool&  booleanTertiaryVariable)
  : oahBooleanAtom (
      shortName,
      longName,
      description,
      variableName,
      booleanVariable),
    fBooleanSecondaryVariable (
      booleanSecondaryVariable),
    fBooleanTertiaryVariable (
      booleanTertiaryVariable)
{}

oahThreeBooleansAtom::~oahThreeBooleansAtom ()
{}

S_oahValuedAtom oahThreeBooleansAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
     "==> option '" << optionName << "' is a oahThreeBooleansAtom" <<
      endl;
  }
#endif

  // handle it at once
  setThreeBooleansVariables (true);

  // no option value is needed
  return nullptr;
}

void oahThreeBooleansAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahThreeBooleansAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahThreeBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahThreeBooleansAtom>*> (v)) {
        S_oahThreeBooleansAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahThreeBooleansAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahThreeBooleansAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahThreeBooleansAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahThreeBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahThreeBooleansAtom>*> (v)) {
        S_oahThreeBooleansAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahThreeBooleansAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahThreeBooleansAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahThreeBooleansAtom::browseData ()" <<
      endl;
  }
#endif
}

void oahThreeBooleansAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "ThreeBooleansAtom:" <<
    endl;

  gIndenter++;

  oahElement::printOptionEssentials (
    os, fieldWidth);

  os <<
    setw (fieldWidth) <<
    "fDescription" << " : " <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "fBooleanVariable" << " : " <<
    booleanAsString (
      fBooleanVariable) <<
    endl <<
    setw (fieldWidth) <<
    "fBooleanSecondaryVariable" << " : " <<
    booleanAsString (
      fBooleanSecondaryVariable) <<
    endl <<
    setw (fieldWidth) <<
    "fBooleanTertiaryVariable" << " : " <<
    booleanAsString (
      fBooleanTertiaryVariable) <<
    endl;

  gIndenter--;
}

void oahThreeBooleansAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    booleanAsString (
      fBooleanVariable) <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahThreeBooleansAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahCombinedBooleansAtom oahCombinedBooleansAtom::create (
  string shortName,
  string longName,
  string description,
  string variableName,
  bool&  booleanVariable)
{
  oahCombinedBooleansAtom* o = new
    oahCombinedBooleansAtom (
      shortName,
      longName,
      description,
      variableName,
      booleanVariable);
  assert(o!=0);
  return o;
}

oahCombinedBooleansAtom::oahCombinedBooleansAtom (
  string shortName,
  string longName,
  string description,
  string variableName,
  bool&  booleanVariable)
  : oahAtomWithVariableName (
      shortName,
      longName,
      description,
      variableName),
    fBooleanVariable (
      booleanVariable)
{}

oahCombinedBooleansAtom::~oahCombinedBooleansAtom ()
{}

void oahCombinedBooleansAtom::addBooleanAtom (
  S_oahBooleanAtom booleanAtom)
{
  // sanity check
  msrAssert (
    booleanAtom != nullptr,
    "booleanAtom is null");

  fBooleanAtomsList.push_back (
    booleanAtom);
}

void oahCombinedBooleansAtom::addBooleanAtomByName (
  string name)
{
  // get the options handler
  S_oahHandler
    handler =
      getSubGroupUpLink ()->
        getGroupUpLink ()->
          getHandlerUpLink ();

  // sanity check
  msrAssert (
    handler != nullptr,
    "handler is null");

  // is name known in options map?
  S_oahElement
    element =
      handler->
        fetchElementFromMap (name);

  if (! element) {
    // no, name is unknown in the map
    handler->
      printOptionsSummary ();

    stringstream s;

    s <<
      "INTERNAL ERROR: option name '" << name <<
      "' is unknown";

    oahError (s.str ());
  }

  else {
    // name is known, let's handle it

    if (
      // boolean atom?
      S_oahBooleanAtom
        atom =
          dynamic_cast<oahBooleanAtom*>(&(*element))
      ) {
      // handle the atom
      fBooleanAtomsList.push_back (atom);
    }

    else {
      stringstream s;

      s <<
        "option name '" << name << "' is not that of an atom";

      oahError (s.str ());
    }
  }
}

void oahCombinedBooleansAtom::setCombinedBooleanVariables (
  bool value)
{
  // set the combined atoms atom variable
  fBooleanVariable = value;

  // set the value of the atoms in the list
  if (fBooleanAtomsList.size ()) {
    for (
      list<S_oahBooleanAtom>::const_iterator i =
        fBooleanAtomsList.begin ();
      i != fBooleanAtomsList.end ();
      i++
    ) {
      S_oahAtom atom = (*i);

      if (
        // boolean atom?
        S_oahBooleanAtom
          booleanAtom =
            dynamic_cast<oahBooleanAtom*>(&(*atom))
        ) {
        // set the boolean variable
        booleanAtom->
          setBooleanVariable (value);
      }
    } // for
  }
}

S_oahValuedAtom oahCombinedBooleansAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahCombinedBooleansAtom" <<
      endl;
  }
#endif

  // handle it at once
  setCombinedBooleanVariables (true);

  // no option value is needed
  return nullptr;
}

void oahCombinedBooleansAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahCombinedBooleansAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahCombinedBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahCombinedBooleansAtom>*> (v)) {
        S_oahCombinedBooleansAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahCombinedBooleansAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahCombinedBooleansAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahCombinedBooleansAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahCombinedBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahCombinedBooleansAtom>*> (v)) {
        S_oahCombinedBooleansAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahCombinedBooleansAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahCombinedBooleansAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahCombinedBooleansAtom::browseData ()" <<
      endl;
  }
#endif

  // browse the boolean atoms
  if (fBooleanAtomsList.size ()) {
    for (
      list<S_oahBooleanAtom>::const_iterator i = fBooleanAtomsList.begin ();
      i != fBooleanAtomsList.end ();
      i++
    ) {
      S_oahBooleanAtom booleanAtom = (*i);

      // browse the boolean atom
      oahBrowser<oahBooleanAtom> browser (v);
      browser.browse (*(booleanAtom));
    } // for
  }
}

void oahCombinedBooleansAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "CombinedBooleansAtom:" <<
    endl;

  gIndenter++;

  printOptionEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fBooleanAtomsList" << " : ";

  if (! fBooleanAtomsList.size ()) {
    os <<
      "none";
  }

  else {
    os << endl;

    gIndenter++;

    os << "'";

    list<S_oahBooleanAtom>::const_iterator
      iBegin = fBooleanAtomsList.begin (),
      iEnd   = fBooleanAtomsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << " ";
    } // for

    os << "'";

    gIndenter--;
  }

  gIndenter--;

  os << endl;
}

void oahCombinedBooleansAtom::printHelp (ostream& os)
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
  }

  os <<
    "This combined option is equivalent to: ";

  if (! fBooleanAtomsList.size ()) {
    os <<
      "none" <<
      endl;
  }

  else {
    os << endl;

    gIndenter++;

    list<S_oahBooleanAtom>::const_iterator
      iBegin = fBooleanAtomsList.begin (),
      iEnd   = fBooleanAtomsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os <<
        (*i)-> fetchNames ();
      if (++i == iEnd) break;
      os << endl;
    } // for

    os << endl;

    gIndenter--;
  }

  if (fDescription.size ()) { // ??? JMI
    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }

  // register help print action in options handler upLink
//  fHandlerUpLink->setOptionsHandlerFoundAHelpOption ();
}

void oahCombinedBooleansAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    booleanAsString (
      fBooleanVariable) <<
    endl;

  int fieldWidth =
    valueFieldWidth - gIndenter.getIndent () + 1;

  gIndenter++; // only now

  if (! fBooleanAtomsList.size ()) {
    os <<
      "none" <<
      endl;
  }

  else {
    list<S_oahBooleanAtom>::const_iterator
      iBegin = fBooleanAtomsList.begin (),
      iEnd   = fBooleanAtomsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_oahAtom
        atom = (*i);

      if (
        // boolean atom?
        S_oahBooleanAtom
          booleanAtom =
            dynamic_cast<oahBooleanAtom*>(&(*atom))
        ) {
        // print the boolean value
        booleanAtom->
          printAtomOptionsValues (
            os, fieldWidth);
      }

      if (++i == iEnd) break;

  // JMI    os << endl;
    } // for
  }

  gIndenter--;

}

ostream& operator<< (ostream& os, const S_oahCombinedBooleansAtom& elt)
{
  elt->print (os);
  return os;
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
  assert(o!=0);
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

/* JMI
S_oahPrefix oahPrefix::fetchOptionByName (
  string name)
{
  S_oahPrefix result;

  if (name == fPrefixName) {
    result = this;
  }

  return result;
}
*/

void oahPrefix::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahPrefix::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahPrefix>*
    p =
      dynamic_cast<visitor<S_oahPrefix>*> (v)) {
        S_oahPrefix elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahPrefix::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahPrefix::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahPrefix::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahPrefix>*
    p =
      dynamic_cast<visitor<S_oahPrefix>*> (v)) {
        S_oahPrefix elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahPrefix::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahPrefix::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahPrefix::browseData ()" <<
      endl;
  }
#endif
}

string oahPrefix::prefixNames () const
{
  stringstream s;

  if (fPrefixName.size ()) {
      s <<
        "-" << fPrefixName;
  }

  return s.str ();
}

string oahPrefix::prefixNamesInColumns (
  int subGroupsShortNameFieldWidth) const
{
  stringstream s;

  if (fPrefixName.size ()) {
      s << left <<
        setw (subGroupsShortNameFieldWidth) <<
        "-" + fPrefixName;
  }

  return s.str ();
}

string oahPrefix::prefixNamesBetweenParentheses () const
{
  stringstream s;

  s <<
    "(" <<
    prefixNames () <<
    ")";

  return s.str ();
}

string oahPrefix::prefixNamesInColumnsBetweenParentheses (
  int subGroupsShortNameFieldWidth) const
{
  stringstream s;

  s <<
    "(" <<
    prefixNamesInColumns (
      subGroupsShortNameFieldWidth) <<
    ")";

  return s.str ();
}

void oahPrefix::printPrefixHeader (ostream& os) const
{
/* JMI
  os <<
    "'" << fPrefixName <<
    "' translates to '" << fPrefixErsatz <<
    "':" <<
    endl;
*/

  if (fPrefixDescription.size ()) {
    // indent a bit more for readability
 //   gIndenter++;

    os <<
      gIndenter.indentMultiLineString (
        fPrefixDescription) <<
      endl;

 //   gIndenter--;
  }
}

void oahPrefix::printPrefixEssentials (
  ostream& os,
  int      fieldWidth) const
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

void oahPrefix::printHelp (ostream& os)
{
  os <<
    prefixNames () <<
    endl;

  if (fPrefixErsatz.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fPrefixErsatz) <<
      endl;

    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }

  if (fPrefixDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fPrefixDescription) <<
      endl;

    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }

  // register help print action in options handler upLink
//  fHandlerUpLink->setOptionsHandlerFoundAHelpOption ();
}

ostream& operator<< (ostream& os, const S_oahPrefix& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahMultiplexBooleansAtom oahMultiplexBooleansAtom::create (
  string      description,
  string      shortSuffixDescriptor,
  string      longSuffixDescriptor,
  S_oahPrefix shortNamesPrefix,
  S_oahPrefix longNamesPrefix)
{
  oahMultiplexBooleansAtom* o = new
    oahMultiplexBooleansAtom (
      description,
      shortSuffixDescriptor,
      longSuffixDescriptor,
      shortNamesPrefix,
      longNamesPrefix);
  assert(o!=0);
  return o;
}

oahMultiplexBooleansAtom::oahMultiplexBooleansAtom (
  string      description,
  string      shortSuffixDescriptor,
  string      longSuffixDescriptor,
  S_oahPrefix shortNamesPrefix,
  S_oahPrefix longNamesPrefix)
  : oahAtom (
      "unusedShortName_" + shortSuffixDescriptor + "_" + description,
        // should be a unique shortName
      "unusedLongName_" + longSuffixDescriptor + "_" + description,
        // should be a unique longName
      description),
    fShortNamesPrefix (
      shortNamesPrefix),
    fLongNamesPrefix (
      longNamesPrefix),
    fShortSuffixDescriptor (
      shortSuffixDescriptor),
    fLongSuffixDescriptor (
      longSuffixDescriptor)
{
  // sanity checks
  msrAssert (
    fShortNamesPrefix != nullptr,
    "fShortNamesPrefix is null");
  msrAssert (
    fLongNamesPrefix != nullptr,
    "fLongNamesPrefix is null");

  // get prefixes names
  fShortNamesPrefixName =
    fShortNamesPrefix->getPrefixName ();
  fLongNamesPrefixName =
    fLongNamesPrefix->getPrefixName ();
}

oahMultiplexBooleansAtom::~oahMultiplexBooleansAtom ()
{}

void oahMultiplexBooleansAtom::addBooleanAtom (
  S_oahBooleanAtom booleanAtom)
{
  // sanity check
  msrAssert (
    booleanAtom != nullptr,
    "booleanAtom is null");

  // short name consistency check
  {
    string booleanAtomShortName =
      booleanAtom->getShortName ();

    std::size_t found =
      booleanAtomShortName.find (fShortNamesPrefixName);

    if (found == string::npos) {
      stringstream s;

      s <<
        "INTERNAL ERROR: option short name '" << booleanAtomShortName <<
        "' is different that the short names prefix name '" <<
        fShortNamesPrefixName <<
        "'";

      booleanAtom->print (s);

      oahError (s.str ());
    }
    else if (found != 0) {
      stringstream s;

      s <<
        "INTERNAL ERROR: option short name '" << booleanAtomShortName <<
        "' doesn't start by the short names prefix name '" <<
        fShortNamesPrefixName <<
        "'";

      booleanAtom->print (s);

      oahError (s.str ());
    }
    else {
      string booleanAtomShortNameSuffix =
        booleanAtomShortName.substr (
          fShortNamesPrefixName.size ());

      if (booleanAtomShortNameSuffix.size () == 0) {
        stringstream s;

        s <<
          "INTERNAL ERROR: option short name '" << booleanAtomShortName <<
          "' is nothing more than the short names prefix name '" <<
          fShortNamesPrefixName <<
          "'";

        booleanAtom->print (s);

        oahError (s.str ());
      }
      else {
        // register this boolean atom's suffix in the list
        fShortNamesSuffixes.push_back (booleanAtomShortNameSuffix);
      }
    }
  }

  // long name consistency check
  {
    string booleanAtomLongName =
      booleanAtom->getLongName ();

    if (booleanAtomLongName.size ()) {
      std::size_t found =
        booleanAtomLongName.find (fLongNamesPrefixName);

      if (found == string::npos) {
        stringstream s;

        s <<
          "INTERNAL ERROR: option long name '" << booleanAtomLongName <<
          "' is different that the long names prefix name '" <<
          fLongNamesPrefixName <<
          "'";

        booleanAtom->print (s);

        oahError (s.str ());
      }
      else if (found != 0) {
        stringstream s;

        s <<
          "INTERNAL ERROR: option long name '" << booleanAtomLongName <<
          "' doesn't start by the long names prefix name '" <<
          fLongNamesPrefixName <<
          "'";

        booleanAtom->print (s);

        oahError (s.str ());
      }
      else {
        string booleanAtomLongNameSuffix =
          booleanAtomLongName.substr (
            fLongNamesPrefixName.size ());

        if (booleanAtomLongNameSuffix.size () == 0) {
          stringstream s;

          s <<
            "INTERNAL ERROR: option long name '" << booleanAtomLongName <<
            "' is nothing more than the long names prefix name '" <<
            fLongNamesPrefixName <<
            "'";

          booleanAtom->print (s);

          oahError (s.str ());
        }
        else {
          // register this boolean atom's suffix in the list
          fLongNamesSuffixes.push_back (booleanAtomLongNameSuffix);
        }
      }
    }

    else {
      stringstream s;

      s <<
        "INTERNAL ERROR: option long name '" << booleanAtomLongName <<
        "' is empty, atom '" <<
        fLongNamesPrefixName <<
        "' cannot be used in a multiplex booleans atom";

      booleanAtom->print (s);

      oahError (s.str ());
    }
  }

  // append the boolean atom to the list
  fBooleanAtomsList.push_back (
    booleanAtom);

  // hide it
  booleanAtom->setIsHidden ();
}

void oahMultiplexBooleansAtom::addBooleanAtomByName (
  string name)
{
  // get the options handler
  S_oahHandler
    handler =
      getSubGroupUpLink ()->
        getGroupUpLink ()->
          getHandlerUpLink ();

  // sanity check
  msrAssert (
    handler != nullptr,
    "handler is null");

  // is name known in options map?
  S_oahElement
    element =
      handler->
        fetchElementFromMap (name);

  if (! element) {
    // no, name is unknown in the map
    handler->
      printOptionsSummary ();

    stringstream s;

    s <<
      "INTERNAL ERROR: option name '" << name <<
      "' is unknown";

    oahError (s.str ());
  }

  else {
    // name is known, let's handle it

    if (
      // boolean atom?
      S_oahBooleanAtom
        atom =
          dynamic_cast<oahBooleanAtom*>(&(*element))
      ) {
      // add the boolean atom
      addBooleanAtom (atom);
    }

    else {
      stringstream s;

      s <<
        "option name '" << name << "' is not that of an atom";

      oahError (s.str ());
    }
  }
}

S_oahValuedAtom oahMultiplexBooleansAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahMultiplexBooleansAtom" <<
      endl;
  }
#endif

  // handle it at once JMI ???

  // no option value is needed
  return nullptr;
}

void oahMultiplexBooleansAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahMultiplexBooleansAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahMultiplexBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahMultiplexBooleansAtom>*> (v)) {
        S_oahMultiplexBooleansAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahMultiplexBooleansAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahMultiplexBooleansAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahMultiplexBooleansAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahMultiplexBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahMultiplexBooleansAtom>*> (v)) {
        S_oahMultiplexBooleansAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahMultiplexBooleansAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahMultiplexBooleansAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahMultiplexBooleansAtom::browseData ()" <<
      endl;
  }
#endif

  if (fShortNamesPrefix) {
    // browse the short names prefix
    oahBrowser<oahPrefix> browser (v);
    browser.browse (*(fShortNamesPrefix));
  }

  if (fLongNamesPrefix) {
    // browse the long names prefix
    oahBrowser<oahPrefix> browser (v);
    browser.browse (*(fLongNamesPrefix));
  }

  // browse the boolean atoms
  if (fBooleanAtomsList.size ()) {
    for (
      list<S_oahBooleanAtom>::const_iterator i = fBooleanAtomsList.begin ();
      i != fBooleanAtomsList.end ();
      i++
    ) {
      S_oahBooleanAtom booleanAtom = (*i);

      // browse the boolean atom
      oahBrowser<oahBooleanAtom> browser (v);
      browser.browse (*(booleanAtom));
    } // for
  }
}

void oahMultiplexBooleansAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "MultiplexBooleansAtom:" <<
    endl;

  gIndenter++;

  printOptionEssentials (
    os, fieldWidth);

  os << left <<
    "shortSuffixDescriptor" << " : " <<
    fShortSuffixDescriptor <<
    endl <<
    "longSuffixDescriptor" << " : " <<
    fLongSuffixDescriptor <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "shortNamesPrefix" << " : ";
  if (fShortNamesPrefix) {
    os << fShortNamesPrefix;
  }
  else {
    os << "null" << endl;
  }

  os << left <<
    setw (fieldWidth) <<
    "longNamesPrefix" << " : ";
  if (fLongNamesPrefix) {
    os << fLongNamesPrefix;
  }
  else {
    os << "null" << endl;
  }

  os << left <<
    setw (fieldWidth) <<
    "fBooleanAtomsList" << " : ";

  if (! fBooleanAtomsList.size ()) {
    os << "none";
  }

  else {
    os << endl;

    gIndenter++;

    os << "'";

    list<S_oahBooleanAtom>::const_iterator
      iBegin = fBooleanAtomsList.begin (),
      iEnd   = fBooleanAtomsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << " ";
    } // for

    os << "'";

    gIndenter--;
  }

  gIndenter--;

  os << endl;
}

void oahMultiplexBooleansAtom::printHelp (ostream& os)
{
  os <<
    "-" << fShortNamesPrefixName << "<" << fShortSuffixDescriptor << ">" <<
    ", " <<
    "-" << fLongNamesPrefixName << "<" << fLongSuffixDescriptor << ">" <<
    endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;
  }

  os <<
    "The " <<
    fShortNamesSuffixes.size () <<
    " known " << fShortSuffixDescriptor << "s are: ";

  if (! fShortNamesSuffixes.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os << endl;
    gIndenter++;

    list<string>::const_iterator
      iBegin = fShortNamesSuffixes.begin (),
      iEnd   = fShortNamesSuffixes.end (),
      i      = iBegin;

    int cumulatedLength = 0;

    for ( ; ; ) {
      string suffix = (*i);

      os << suffix;

      cumulatedLength += suffix.size ();
      if (cumulatedLength >= K_NAMES_LIST_MAX_LENGTH) break;

      if (++i == iEnd) break;

      if (next (i) == iEnd) {
        os << " and ";
      }
      else {
        os << ", ";
      }
    } // for

    os << "." << endl;
    gIndenter--;
  }

  if (fLongSuffixDescriptor != fShortSuffixDescriptor) {
    os <<
    "The " <<
    fLongNamesSuffixes.size () <<
    " known " << fLongSuffixDescriptor << "s are: ";

    if (! fLongNamesSuffixes.size ()) {
      os <<
        "none" <<
        endl;
    }
    else {
      os << endl;
      gIndenter++;

      list<string>::const_iterator
        iBegin = fLongNamesSuffixes.begin (),
        iEnd   = fLongNamesSuffixes.end (),
        i      = iBegin;

      int cumulatedLength = 0;

      for ( ; ; ) {
        string suffix = (*i);

        os << suffix;

        cumulatedLength += suffix.size ();
        if (cumulatedLength >= K_NAMES_LIST_MAX_LENGTH) break;

        if (++i == iEnd) break;
        if (next (i) == iEnd) {
          os << " and ";
        }
        else {
          os << ", ";
        }
      } // for

      os << "." << endl;
      gIndenter--;
    }
  }

  if (fDescription.size ()) { // ??? JMI
    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }

  // register help print action in options handler upLink
//  fHandlerUpLink->setOptionsHandlerFoundAHelpOption ();
}

void oahMultiplexBooleansAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to do, these options values will be printed
  // by the boolean atoms in the list
}

ostream& operator<< (ostream& os, const S_oahMultiplexBooleansAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
/* pure virtual class
S_oahValuedAtom oahValuedAtom::create (
  string shortName,
  string longName,
  string description,
  string valueSpecification,
  string variableName)
{
  oahValuedAtom* o = new
    oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName);
  assert(o!=0);
  return o;
}
*/

oahValuedAtom::oahValuedAtom (
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

  fValueIsOptional = false;
}

oahValuedAtom::~oahValuedAtom ()
{}

void oahValuedAtom::setValueIsOptional ()
{
  fValueIsOptional = true;

/* JMI
  // a valued atom with an optional value
  // should not have the same name as a prefix
  // since this would create an ambiguity
  if (fetchPrefixInMapByItsName (fShortName)) {
    stringstream s;

    s <<
      "values atom short name '" << fShortName <<
      "' is already known as a prefix " <<;

    oahError (s.str ());
  }
  if (fetchPrefixInMapByItsName (fLongName)) {
    stringstream s;

    s <<
      "values atom long name '" << fLongName <<
      "' is already known as a prefix " <<;

    oahError (s.str ());
  }
  */
}

void oahValuedAtom::handleDefaultValue ()
{}

void oahValuedAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahValuedAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahValuedAtom>*
    p =
      dynamic_cast<visitor<S_oahValuedAtom>*> (v)) {
        S_oahValuedAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahValuedAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahValuedAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahValuedAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahValuedAtom>*
    p =
      dynamic_cast<visitor<S_oahValuedAtom>*> (v)) {
        S_oahValuedAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahValuedAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahValuedAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahValuedAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahValuedAtom::oahAtomKindAsString (
  oahValuedAtom::oahValuedAtomKind oahAtomKind)
{
  string result;

  switch (oahAtomKind) {
    case oahValuedAtom::kAtomHasNoArgument:
      result = "atomHasNoArgument";
      break;
    case oahValuedAtom::kAtomHasARequiredArgument:
      result = "atomHasARequiredArgument";
      break;
    case oahValuedAtom::kAtomHasAnOptionsArgument:
      result = "atomHasAnOptionsArgument";
      break;
  } // switch

  return result;
}

void oahValuedAtom::printValuedAtomEssentials (
  ostream& os,
  int      fieldWidth) const
{
  printOptionEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fValueSpecification" << " : " <<
    fValueSpecification <<
    endl <<
    setw (fieldWidth) <<
    "fValueIsOptional" << " : " <<
    booleanAsString (fValueIsOptional) <<
    endl;
}

void oahValuedAtom::print (ostream& os) const
{
  const int fieldWidth = 19;

  os <<
    "ValuedAtom ???:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  gIndenter--;
}

void oahValuedAtom::printHelp (ostream& os)
{
  os <<
    fetchNames ();

  oahOptionalValuesStyleKind
    optionalValuesStyleKind =
      fHandlerUpLink->
        getHandlerOptionalValuesStyleKind ();


if (fValueIsOptional) {
    switch (optionalValuesStyleKind) {
      case kOptionalValuesStyleGNU: // default value
        os <<
          "[=" << fValueSpecification << "]";
        break;
      case kOptionalValuesStyleOAH:
        os <<
          " " << fValueSpecification;
    } // switch
  }
  else {
    os <<
      " " << fValueSpecification;
  }

  os << endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;

    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }

  // register help print action in options handler upLink // JMI
//  fHandlerUpLink->setOptionsHandlerFoundAHelpOption ();
}

void oahValuedAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os <<
    "ValuedAtom values ???:" <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahValuedAtom& elt)
{
  elt->print (os);
  return os;
}

// optional values style
//______________________________________________________________________________

map<string, oahOptionalValuesStyleKind>
  gOahOptionalValuesStyleKindsMap;

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

  gOahOptionalValuesStyleKindsMap ["gnu"] = kOptionalValuesStyleGNU;
  gOahOptionalValuesStyleKindsMap ["oah"] = kOptionalValuesStyleOAH;
}

string existingOahOptionalValuesStyleKinds (int namesListMaxLength)
{
  stringstream s;

  if (gOahOptionalValuesStyleKindsMap.size ()) {
    map<string, oahOptionalValuesStyleKind>::const_iterator
      iBegin = gOahOptionalValuesStyleKindsMap.begin (),
      iEnd   = gOahOptionalValuesStyleKindsMap.end (),
      i      = iBegin;

    int cumulatedLength = 0;

    for ( ; ; ) {
      string theString = (*i).first;

      s << theString;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= K_NAMES_LIST_MAX_LENGTH) break;

      if (++i == iEnd) break;
      if (next (i) == iEnd) {
        s << " and ";
      }
      else {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

//______________________________________________________________________________
S_oahIntegerAtom oahIntegerAtom::create (
  string shortName,
  string longName,
  string description,
  string valueSpecification,
  string variableName,
  int&   integerVariable)
{
  oahIntegerAtom* o = new
    oahIntegerAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      integerVariable);
  assert(o!=0);
  return o;
}

oahIntegerAtom::oahIntegerAtom (
  string shortName,
  string longName,
  string description,
  string valueSpecification,
  string variableName,
  int&   integerVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fIntegerVariable (
      integerVariable)
{}

oahIntegerAtom::~oahIntegerAtom ()
{}

S_oahValuedAtom oahIntegerAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahIntegerAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void oahIntegerAtom::handleValue (
  string   theString,
  ostream& os)
{
  // theString contains the integer value

  // check whether it is well-formed
  string regularExpression (
    "([[:digit:]]+)");

  regex e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "There are " << smSize << " matches" <<
      " for integer string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;

    for (unsigned i = 0; i < smSize; ++i) {
      os <<
        "[" << sm [i] << "] ";
    } // for

    os << endl;
  }
#endif

  if (smSize) {
    // leave the low level details to the STL...
    int integerValue;
    {
      stringstream s;
      s << theString;
      s >> integerValue;
    }

    fIntegerVariable = integerValue;
  }

  else {
    stringstream s;

    s <<
      "integer value '" << theString <<
      "' for option '" << fetchNames () <<
      "' is ill-formed";

    oahError (s.str ());
  }
}

void oahIntegerAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahIntegerAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahIntegerAtom>*
    p =
      dynamic_cast<visitor<S_oahIntegerAtom>*> (v)) {
        S_oahIntegerAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahIntegerAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahIntegerAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahIntegerAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahIntegerAtom>*
    p =
      dynamic_cast<visitor<S_oahIntegerAtom>*> (v)) {
        S_oahIntegerAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahIntegerAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahIntegerAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahIntegerAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahIntegerAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " << fIntegerVariable;

  return s.str ();
}

string oahIntegerAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " << fIntegerVariable;

  return s.str ();
}

void oahIntegerAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "IntegerAtom:" <<
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
    "fIntegerVariable" << " : " <<
    fIntegerVariable <<
    endl;

  gIndenter--;
}

void oahIntegerAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    fIntegerVariable <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahIntegerAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahTwoIntegersAtom oahTwoIntegersAtom::create (
  string shortName,
  string longName,
  string description,
  string valueSpecification,
  string variableName,
  int&   integerVariable,
  int&   integerSecondaryVariable)
{
  oahTwoIntegersAtom* o = new
    oahTwoIntegersAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      integerVariable,
      integerSecondaryVariable);
  assert(o!=0);
  return o;
}

oahTwoIntegersAtom::oahTwoIntegersAtom (
  string shortName,
  string longName,
  string description,
  string valueSpecification,
  string variableName,
  int&   integerVariable,
  int&   integerSecondaryVariable)
  : oahIntegerAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      integerVariable),
    fIntegerSecondaryVariable (
      integerSecondaryVariable)
{}

oahTwoIntegersAtom::~oahTwoIntegersAtom ()
{}

S_oahValuedAtom oahTwoIntegersAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahTwoIntegersAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void oahTwoIntegersAtom::handleValue (
  string   theString,
  ostream& os)
{
  // theString contains the two integer values

  // check whether it is well-formed
  string regularExpression (
    "([[:digit:]]+)[[:space:]]+([[:digit:]]+)");

  regex e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "There are " << smSize << " matches" <<
      " for integer string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;

    for (unsigned i = 0; i < smSize; ++i) {
      os <<
        "[" << sm [i] << "] ";
    } // for

    os << endl;
  }
#endif

  if (smSize == 3) {
    // leave the low level details to the STL...
    int integerValue;
    {
      stringstream s;
      s << sm [ 1 ];
      s >> integerValue;
    }
    fIntegerVariable = integerValue;

    {
      stringstream s;
      s << sm [ 2 ];
      s >> integerValue;
    }
    fIntegerSecondaryVariable = integerValue;
  }

  else {
    stringstream s;

    s <<
      "integer value '" << theString <<
      "' for option '" << fetchNames () <<
      "' is ill-formed";

    oahError (s.str ());
  }
}

void oahTwoIntegersAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahTwoIntegersAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahTwoIntegersAtom>*
    p =
      dynamic_cast<visitor<S_oahTwoIntegersAtom>*> (v)) {
        S_oahTwoIntegersAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahTwoIntegersAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahTwoIntegersAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahTwoIntegersAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahTwoIntegersAtom>*
    p =
      dynamic_cast<visitor<S_oahTwoIntegersAtom>*> (v)) {
        S_oahTwoIntegersAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahTwoIntegersAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahTwoIntegersAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahTwoIntegersAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahTwoIntegersAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName <<
    " \"" <<
    fIntegerVariable <<
    " " <<
    fIntegerSecondaryVariable <<
    "\"";

  return s.str ();
}

string oahTwoIntegersAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName <<
    " \"" <<
    fIntegerVariable <<
    " " <<
    fIntegerSecondaryVariable <<
    "\"";

  return s.str ();
}

void oahTwoIntegersAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "IntegerAtom:" <<
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
    "fIntegerVariable" << " : " <<
    fIntegerVariable <<
    endl <<
    setw (fieldWidth) <<
    "fIntegerSecondaryVariable" << " : " <<
    fIntegerSecondaryVariable <<
    endl;

  gIndenter--;
}

void oahTwoIntegersAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    "\"" <<
    fIntegerVariable <<
    " " <<
    fIntegerSecondaryVariable <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahTwoIntegersAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahFloatAtom oahFloatAtom::create (
  string shortName,
  string longName,
  string description,
  string valueSpecification,
  string variableName,
  float& floatVariable)
{
  oahFloatAtom* o = new
    oahFloatAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      floatVariable);
  assert(o!=0);
  return o;
}

oahFloatAtom::oahFloatAtom (
  string shortName,
  string longName,
  string description,
  string valueSpecification,
  string variableName,
  float& floatVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fFloatVariable (
      floatVariable)
{}

oahFloatAtom::~oahFloatAtom ()
{}

S_oahValuedAtom oahFloatAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahFloatAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void oahFloatAtom::handleValue (
  string   theString,
  ostream& os)
{
  // theString contains the float value

  // check whether it is well-formed
  string regularExpression (
    // no sign, a '-' would be handled as an option name JMI   "([+|-]?)"
    "([[:digit:]]+)(.[[:digit:]]*)?"
    );

  regex e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "There are " << smSize << " matches" <<
      " for float string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;

    for (unsigned i = 0; i < smSize; ++i) {
      os <<
        "[" << sm [i] << "] ";
    } // for

    os << endl;
  }
#endif

  if (smSize == 3) {
    // leave the low level details to the STL...
    float floatValue;
    {
      stringstream s;

      s << sm [ 0 ];
      s >> floatValue;
    }

    fFloatVariable = floatValue;
  }

  else {
    stringstream s;

    s <<
      "float value '" << theString <<
      "' for option '" << fetchNames () <<
      "' is ill-formed";

    oahError (s.str ());
  }
}

void oahFloatAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahFloatAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahFloatAtom>*
    p =
      dynamic_cast<visitor<S_oahFloatAtom>*> (v)) {
        S_oahFloatAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahFloatAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahFloatAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahFloatAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahFloatAtom>*
    p =
      dynamic_cast<visitor<S_oahFloatAtom>*> (v)) {
        S_oahFloatAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahFloatAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahFloatAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahFloatAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahFloatAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " << fFloatVariable;

  return s.str ();
}

string oahFloatAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " << fFloatVariable;

  return s.str ();
}

void oahFloatAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "FloatAtom:" <<
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
    "fFloatVariable" << " : " <<
    fFloatVariable <<
    endl;

  gIndenter--;
}

void oahFloatAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    fFloatVariable <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahFloatAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahStringAtom oahStringAtom::create (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& stringVariable)
{
  oahStringAtom* o = new
    oahStringAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringVariable);
  assert(o!=0);
  return o;
}

oahStringAtom::oahStringAtom (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& stringVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringVariable (
      stringVariable)
{}

oahStringAtom::~oahStringAtom ()
{}

S_oahValuedAtom oahStringAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahStringAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void oahStringAtom::handleValue (
  string   theString,
  ostream& os)
{
  fStringVariable = theString;
}

void oahStringAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahStringAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringAtom>*
    p =
      dynamic_cast<visitor<S_oahStringAtom>*> (v)) {
        S_oahStringAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahStringAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahStringAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringAtom>*
    p =
      dynamic_cast<visitor<S_oahStringAtom>*> (v)) {
        S_oahStringAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahStringAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahStringAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahStringAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahStringAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " '" << fStringVariable << "'";

  return s.str ();
}

string oahStringAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " '" << fStringVariable << "'";

  return s.str ();
}

void oahStringAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringAtom:" <<
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
    "fStringVariable" << " : " <<
    fStringVariable <<
    endl;

  gIndenter--;
}

void oahStringAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    fStringVariable <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahStringAtom& elt)
{
  os <<
    "StringAtom:" <<
    endl;
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahMonoplexStringAtom oahMonoplexStringAtom::create (
  string      description,
  string      atomNameDescriptor,
  string      stringValueDescriptor)
{
  oahMonoplexStringAtom* o = new
    oahMonoplexStringAtom (
      description,
      atomNameDescriptor,
      stringValueDescriptor);
  assert(o!=0);
  return o;
}

oahMonoplexStringAtom::oahMonoplexStringAtom (
  string      description,
  string      atomNameDescriptor,
  string      stringValueDescriptor)
  : oahAtom (
      "unusedShortName_" + atomNameDescriptor + "_" + description,
        // should be a unique shortName
      "unusedLongName_" + atomNameDescriptor + "_" + description,
        // should be a unique longName
      description),
    fAtomNameDescriptor (
      atomNameDescriptor),
    fStringValueDescriptor (
      stringValueDescriptor)
{
  // sanity checks
  msrAssert (
    stringValueDescriptor.size (),
    "stringValueDescriptor is empty");
  msrAssert (
    stringValueDescriptor.size (),
    "stringValueDescriptor is empty");
}

oahMonoplexStringAtom::~oahMonoplexStringAtom ()
{}

void oahMonoplexStringAtom::addStringAtom (
  S_oahStringAtom stringAtom)
{
  // sanity check
  msrAssert (
    stringAtom != nullptr,
    "stringAtom is null");

  // atom short name consistency check
  {
    string stringAtomShortName =
      stringAtom->getShortName ();

    if (stringAtomShortName.size () == 0) {
      stringstream s;

      s <<
        "INTERNAL ERROR: option short name '" << stringAtomShortName <<
        "' is empty";

      stringAtom->print (s);

      oahError (s.str ());
    }
    else {
      // register this string atom's suffix in the list
      fAtomNamesList.push_back (stringAtomShortName);
    }
  }

  // atom long name consistency check
  {
    string stringAtomLongName =
      stringAtom->getLongName ();

    if (stringAtomLongName.size () != 0) {
      stringstream s;

      s <<
        "INTERNAL ERROR: option long name '" << stringAtomLongName <<
        "' is not empty";

      stringAtom->print (s);

      oahError (s.str ());
    }
  }

  // append the string atom to the list
  fStringAtomsList.push_back (
    stringAtom);

  // hide it
  stringAtom->setIsHidden ();
}

void oahMonoplexStringAtom::addStringAtomByName (
  string name)
{
  // get the options handler
  S_oahHandler
    handler =
      getSubGroupUpLink ()->
        getGroupUpLink ()->
          getHandlerUpLink ();

  // sanity check
  msrAssert (
    handler != nullptr,
    "handler is null");

  // is name known in options map?
  S_oahElement
    element =
      handler->
        fetchElementFromMap (name);

  if (! element) {
    // no, name is unknown in the map
    handler->
      printOptionsSummary ();

    stringstream s;

    s <<
      "INTERNAL ERROR: option name '" << name <<
      "' is unknown";

    oahError (s.str ());
  }

  else {
    // name is known, let's handle it

    if (
      // string atom?
      S_oahStringAtom
        atom =
          dynamic_cast<oahStringAtom*>(&(*element))
      ) {
      // add the string atom
      addStringAtom (atom);
    }

    else {
      stringstream s;

      s <<
        "option name '" << name << "' is not that of an atom";

      oahError (s.str ());
    }
  }
}

S_oahValuedAtom oahMonoplexStringAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahMonoplexStringAtom" <<
      endl;
  }
#endif

  // handle it at once JMI ???

  // no option value is needed
  return nullptr;
}

void oahMonoplexStringAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahMonoplexStringAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahMonoplexStringAtom>*
    p =
      dynamic_cast<visitor<S_oahMonoplexStringAtom>*> (v)) {
        S_oahMonoplexStringAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahMonoplexStringAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahMonoplexStringAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahMonoplexStringAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahMonoplexStringAtom>*
    p =
      dynamic_cast<visitor<S_oahMonoplexStringAtom>*> (v)) {
        S_oahMonoplexStringAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahMonoplexStringAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahMonoplexStringAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahMonoplexStringAtom::browseData ()" <<
      endl;
  }
#endif

  // browse the string atoms
  if (fStringAtomsList.size ()) {
    for (
      list<S_oahStringAtom>::const_iterator i = fStringAtomsList.begin ();
      i != fStringAtomsList.end ();
      i++
    ) {
      S_oahStringAtom stringAtom = (*i);

      // browse the string atom
      oahBrowser<oahStringAtom> browser (v);
      browser.browse (*(stringAtom));
    } // for
  }
}

void oahMonoplexStringAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "MonoplexStringAtom:" <<
    endl;

  gIndenter++;

  printOptionEssentials (
    os, fieldWidth);

  os << left <<
    "atomNameDescriptor" << " : " <<
    fAtomNameDescriptor <<
    endl <<
    "stringValueDescriptor" << " : " <<
    fStringValueDescriptor <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fStringAtomsList" << " : ";

  if (! fStringAtomsList.size ()) {
    os << "none";
  }

  else {
    os << endl;

    gIndenter++;

    os << "'";

    list<S_oahStringAtom>::const_iterator
      iBegin = fStringAtomsList.begin (),
      iEnd   = fStringAtomsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << " ";
    } // for

    os << "'";

    gIndenter--;
  }

  gIndenter--;

  os << endl;
}

void oahMonoplexStringAtom::printHelp (ostream& os)
{
  os <<
    "-" << fAtomNameDescriptor << " " << fStringValueDescriptor <<
    endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;
  }

  os <<
    "The " <<
    fAtomNamesList.size () <<
    " known " << fAtomNameDescriptor << "s are: ";

  if (! fAtomNamesList.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os << endl;
    gIndenter++;

    list<string>::const_iterator
      iBegin = fAtomNamesList.begin (),
      iEnd   = fAtomNamesList.end (),
      i      = iBegin;

    int cumulatedLength = 0;

    for ( ; ; ) {
      string suffix = (*i);

      os << suffix;

      cumulatedLength += suffix.size ();
      if (cumulatedLength >= K_NAMES_LIST_MAX_LENGTH) break;

      if (++i == iEnd) break;
      if (next (i) == iEnd) {
        os << " and ";
      }
      else {
        os << ", ";
      }
    } // for

    os << "." << endl;
    gIndenter--;
  }

  if (fDescription.size ()) { // ??? JMI
    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }

  // register help print action in options handler upLink
//  fHandlerUpLink->setOptionsHandlerFoundAHelpOption ();
}

void oahMonoplexStringAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to do, these options values will be printed
  // by the string atoms in the list
}

ostream& operator<< (ostream& os, const S_oahMonoplexStringAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahStringWithDefaultValueAtom oahStringWithDefaultValueAtom::create (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& stringVariable,
  string  defaultStringValue)
{
  oahStringWithDefaultValueAtom* o = new
    oahStringWithDefaultValueAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringVariable,
      defaultStringValue);
  assert(o!=0);
  return o;
}

oahStringWithDefaultValueAtom::oahStringWithDefaultValueAtom (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& stringVariable,
  string  defaultStringValue)
  : oahStringAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringVariable),
    fDefaultStringValue (
      defaultStringValue)
{
  setValueIsOptional ();
}

oahStringWithDefaultValueAtom::~oahStringWithDefaultValueAtom ()
{}

S_oahValuedAtom oahStringWithDefaultValueAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahStringWithDefaultValueAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void oahStringWithDefaultValueAtom::handleValue (
  string   theString,
  ostream& os)
{
  oahStringAtom::handleValue (theString, os); // JMI ???
}

void oahStringWithDefaultValueAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahStringWithDefaultValueAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringWithDefaultValueAtom>*
    p =
      dynamic_cast<visitor<S_oahStringWithDefaultValueAtom>*> (v)) {
        S_oahStringWithDefaultValueAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahStringWithDefaultValueAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringWithDefaultValueAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahStringWithDefaultValueAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringWithDefaultValueAtom>*
    p =
      dynamic_cast<visitor<S_oahStringWithDefaultValueAtom>*> (v)) {
        S_oahStringWithDefaultValueAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahStringWithDefaultValueAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahStringWithDefaultValueAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahStringWithDefaultValueAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahStringWithDefaultValueAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " << fStringVariable;

  return s.str ();
}

string oahStringWithDefaultValueAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " << fStringVariable;

  return s.str ();
}

void oahStringWithDefaultValueAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringWithDefaultValueAtom:" <<
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
    "fStringVariable" << " : " <<
    fStringVariable <<
    endl;

  gIndenter--;
}

void oahStringWithDefaultValueAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    fStringVariable <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahStringWithDefaultValueAtom& elt)
{
  os <<
    "StringWithDefaultValueAtom:" <<
    endl;
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahRationalAtom oahRationalAtom::create (
  string    shortName,
  string    longName,
  string    description,
  string    valueSpecification,
  string    variableName,
  rational& rationalVariable)
{
  oahRationalAtom* o = new
    oahRationalAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      rationalVariable);
  assert(o!=0);
  return o;
}

oahRationalAtom::oahRationalAtom (
  string    shortName,
  string    longName,
  string    description,
  string    valueSpecification,
  string    variableName,
  rational& rationalVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fRationalVariable (
      rationalVariable)
{}

oahRationalAtom::~oahRationalAtom ()
{}

S_oahValuedAtom oahRationalAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahRationalAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void oahRationalAtom::handleValue (
  string   theString,
  ostream& os)
{

  // theString contains the fraction:
  // decipher it to extract numerator and denominator values

  string regularExpression (
    "[[:space:]]*([[:digit:]]+)[[:space:]]*"
    "/"
    "[[:space:]]*([[:digit:]]+)[[:space:]]*");

  regex e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "There are " << smSize << " matches" <<
      " for rational string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;

    for (unsigned i = 0; i < smSize; ++i) {
      os <<
        "[" << sm [i] << "] ";
    } // for

    os << endl;
  }
#endif

  if (smSize == 3) { // JMI ???
    int
      numerator,
      denominator;

    {
      stringstream s;
      s << sm [1];
      s >> numerator;
    }
    {
      stringstream s;
      s << sm [2];
      s >> denominator;
    }

    rational
      rationalValue =
        rational (numerator, denominator);

#ifdef TRACE_OAH
    if (gTraceOah->fTraceOah) {
      os <<
        "rationalValue = " <<
        rationalValue <<
        endl;
    }
#endif

    fRationalVariable = rationalValue;
  }

  else {
    stringstream s;

    s <<
      "rational atom value '" << theString <<
      "' is ill-formed";

    oahError (s.str ());
  }
}

void oahRationalAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahRationalAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahRationalAtom>*
    p =
      dynamic_cast<visitor<S_oahRationalAtom>*> (v)) {
        S_oahRationalAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahRationalAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahRationalAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahRationalAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahRationalAtom>*
    p =
      dynamic_cast<visitor<S_oahRationalAtom>*> (v)) {
        S_oahRationalAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahRationalAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahRationalAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahRationalAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahRationalAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " << fRationalVariable;

  return s.str ();
}

string oahRationalAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " << fRationalVariable;

  return s.str ();
}

void oahRationalAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "RationalAtom:" <<
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
    "fRationalVariable" << " : " <<
    fRationalVariable <<
    endl;

  gIndenter--;
}

void oahRationalAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    fRationalVariable <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahRationalAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahNaturalNumbersSetElementAtom oahNaturalNumbersSetElementAtom::create (
  string    shortName,
  string    longName,
  string    description,
  string    valueSpecification,
  string    variableName,
  set<int>& naturalNumbersSetVariable)
{
  oahNaturalNumbersSetElementAtom* o = new
    oahNaturalNumbersSetElementAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      naturalNumbersSetVariable);
  assert(o!=0);
  return o;
}

oahNaturalNumbersSetElementAtom::oahNaturalNumbersSetElementAtom (
  string    shortName,
  string    longName,
  string    description,
  string    valueSpecification,
  string    variableName,
  set<int>& naturalNumbersSetVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fNaturalNumbersSetVariable (
      naturalNumbersSetVariable)
{}

oahNaturalNumbersSetElementAtom::~oahNaturalNumbersSetElementAtom ()
{}

S_oahValuedAtom oahNaturalNumbersSetElementAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahNaturalNumbersSetElementAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void oahNaturalNumbersSetElementAtom::handleValue (
  string   theString,
  ostream& os)
{
  // theString contains the integer value

  // check whether it is well-formed
  string regularExpression (
    "([[:digit:]]+)");

  regex e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "There are " << smSize << " matches" <<
      " for integer string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;

    for (unsigned i = 0; i < smSize; ++i) {
      os <<
        "[" << sm [i] << "] ";
    } // for

    os << endl;
  }
#endif

  if (smSize) {
    // leave the low level details to the STL...
    int integerValue;
    {
      stringstream s;
      s << theString;
      s >> integerValue;
    }

    fNaturalNumbersSetVariable.insert (integerValue);
  }

  else {
    stringstream s;

    s <<
      "integer value '" << theString <<
      "' for option '" << fetchNames () <<
      "' is ill-formed";

    oahError (s.str ());
  }
}

void oahNaturalNumbersSetElementAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahNaturalNumbersSetElementAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahNaturalNumbersSetElementAtom>*
    p =
      dynamic_cast<visitor<S_oahNaturalNumbersSetElementAtom>*> (v)) {
        S_oahNaturalNumbersSetElementAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahNaturalNumbersSetElementAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahNaturalNumbersSetElementAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahNaturalNumbersSetElementAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahNaturalNumbersSetElementAtom>*
    p =
      dynamic_cast<visitor<S_oahNaturalNumbersSetElementAtom>*> (v)) {
        S_oahNaturalNumbersSetElementAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahNaturalNumbersSetElementAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahNaturalNumbersSetElementAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahNaturalNumbersSetElementAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahNaturalNumbersSetElementAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    "[";

  set<int>::const_iterator
    iBegin = fNaturalNumbersSetVariable.begin (),
    iEnd   = fNaturalNumbersSetVariable.end (),
    i      = iBegin;

  for ( ; ; ) {
    s << (*i);
    if (++i == iEnd) break;
    s << " ";
  } // for

  s <<
    "]";

  return s.str ();
}

string oahNaturalNumbersSetElementAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    "[";

  set<int>::const_iterator
    iBegin = fNaturalNumbersSetVariable.begin (),
    iEnd   = fNaturalNumbersSetVariable.end (),
    i      = iBegin;

  for ( ; ; ) {
    s << (*i);
    if (++i == iEnd) break;
    s << " ";
  } // for

  s <<
    "]";

  return s.str ();
}

void oahNaturalNumbersSetElementAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "NaturalNumbersSetElementAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fNaturalNumbersSetVariable" << " : " <<
    endl;

  if (! fNaturalNumbersSetVariable.size ()) {
    os <<
      "none";
  }

  else {
    os <<
      "'";

    set<int>::const_iterator
      iBegin = fNaturalNumbersSetVariable.begin (),
      iEnd   = fNaturalNumbersSetVariable.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << " ";
    } // for

    os <<
      "'";
  }

  os << endl;

  gIndenter--;
}

void oahNaturalNumbersSetElementAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fNaturalNumbersSetVariable.size ()) {
    os <<
      "none";
  }

  else {
    os <<
      "'";

    set<int>::const_iterator
      iBegin = fNaturalNumbersSetVariable.begin (),
      iEnd   = fNaturalNumbersSetVariable.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << " ";
    } // for

    os <<
      "'";
  }

  os << endl;
}

ostream& operator<< (ostream& os, const S_oahNaturalNumbersSetElementAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahNaturalNumbersSetAtom oahNaturalNumbersSetAtom::create (
  string    shortName,
  string    longName,
  string    description,
  string    valueSpecification,
  string    variableName,
  set<int>& naturalNumbersSetVariable)
{
  oahNaturalNumbersSetAtom* o = new
    oahNaturalNumbersSetAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      naturalNumbersSetVariable);
  assert(o!=0);
  return o;
}

oahNaturalNumbersSetAtom::oahNaturalNumbersSetAtom (
  string    shortName,
  string    longName,
  string    description,
  string    valueSpecification,
  string    variableName,
  set<int>& naturalNumbersSetVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fNaturalNumbersSetVariable (
      naturalNumbersSetVariable)
{}

oahNaturalNumbersSetAtom::~oahNaturalNumbersSetAtom ()
{}

S_oahValuedAtom oahNaturalNumbersSetAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahNaturalNumbersSetAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void oahNaturalNumbersSetAtom::handleValue (
  string   theString,
  ostream& os)
{
  fNaturalNumbersSetVariable =
    decipherNaturalNumbersSetSpecification (
      theString,
      false); // 'true' to debug it
}

void oahNaturalNumbersSetAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahNaturalNumbersSetAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahNaturalNumbersSetAtom>*
    p =
      dynamic_cast<visitor<S_oahNaturalNumbersSetAtom>*> (v)) {
        S_oahNaturalNumbersSetAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahNaturalNumbersSetAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahNaturalNumbersSetAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahNaturalNumbersSetAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahNaturalNumbersSetAtom>*
    p =
      dynamic_cast<visitor<S_oahNaturalNumbersSetAtom>*> (v)) {
        S_oahNaturalNumbersSetAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahNaturalNumbersSetAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahNaturalNumbersSetAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahNaturalNumbersSetAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahNaturalNumbersSetAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    "[";

  set<int>::const_iterator
    iBegin = fNaturalNumbersSetVariable.begin (),
    iEnd   = fNaturalNumbersSetVariable.end (),
    i      = iBegin;

  for ( ; ; ) {
    s << (*i);
    if (++i == iEnd) break;
    s << " ";
  } // for

  s <<
    "]";

  return s.str ();
}

string oahNaturalNumbersSetAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    "[";

  set<int>::const_iterator
    iBegin = fNaturalNumbersSetVariable.begin (),
    iEnd   = fNaturalNumbersSetVariable.end (),
    i      = iBegin;

  for ( ; ; ) {
    s << (*i);
    if (++i == iEnd) break;
    s << " ";
  } // for

  s <<
    "]";

  return s.str ();
}

void oahNaturalNumbersSetAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "NaturalNumbersSetAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fNaturalNumbersSetVariable" << " : " <<
    endl;

  if (! fNaturalNumbersSetVariable.size ()) {
    os <<
      "none";
  }

  else {
    os <<
      "'";

    set<int>::const_iterator
      iBegin = fNaturalNumbersSetVariable.begin (),
      iEnd   = fNaturalNumbersSetVariable.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << " ";
    } // for

    os <<
      "'";
  }

  os << endl;

  gIndenter--;
}

void oahNaturalNumbersSetAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fNaturalNumbersSetVariable.size ()) {
    os <<
      "none";
  }

  else {
    os <<
      "'";

    set<int>::const_iterator
      iBegin = fNaturalNumbersSetVariable.begin (),
      iEnd   = fNaturalNumbersSetVariable.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << " ";
    } // for

    os <<
      "'";
  }

  os << endl;
}

ostream& operator<< (ostream& os, const S_oahNaturalNumbersSetAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahStringsSetElementAtom oahStringsSetElementAtom::create (
  string       shortName,
  string       longName,
  string       description,
  string       valueSpecification,
  string       variableName,
  set<string>& stringsSetVariable)
{
  oahStringsSetElementAtom* o = new
    oahStringsSetElementAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringsSetVariable);
  assert(o!=0);
  return o;
}

oahStringsSetElementAtom::oahStringsSetElementAtom (
  string       shortName,
  string       longName,
  string       description,
  string       valueSpecification,
  string       variableName,
  set<string>& stringsSetVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringsSetVariable (
      stringsSetVariable)
{
  fMultipleOccurrencesAllowed = true;
}

oahStringsSetElementAtom::~oahStringsSetElementAtom ()
{}

S_oahValuedAtom oahStringsSetElementAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahStringsSetElementAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void oahStringsSetElementAtom::handleValue (
  string   theString,
  ostream& os)
{
  fStringsSetVariable.insert (theString);
}

void oahStringsSetElementAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahStringsSetElementAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringsSetElementAtom>*
    p =
      dynamic_cast<visitor<S_oahStringsSetElementAtom>*> (v)) {
        S_oahStringsSetElementAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahStringsSetElementAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringsSetElementAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahStringsSetElementAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringsSetElementAtom>*
    p =
      dynamic_cast<visitor<S_oahStringsSetElementAtom>*> (v)) {
        S_oahStringsSetElementAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahStringsSetElementAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahStringsSetElementAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahStringsSetElementAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahStringsSetElementAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    "[";

  set<string>::const_iterator
    iBegin = fStringsSetVariable.begin (),
    iEnd   = fStringsSetVariable.end (),
    i      = iBegin;

  string currentValue; // JMI do better

  for ( ; ; ) {
    // write the value only once
    string theString = (*i);

    if (true || theString != currentValue) {
      s << theString;
    }
    currentValue = theString;
    if (++i == iEnd) break;
    s << " ";
  } // for

  s <<
    "]";

  return s.str ();
}

string oahStringsSetElementAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    "[";

  set<string>::const_iterator
    iBegin = fStringsSetVariable.begin (),
    iEnd   = fStringsSetVariable.end (),
    i      = iBegin;

  for ( ; ; ) {
    s << (*i);
    if (++i == iEnd) break;
    s << " ";
  } // for

  s <<
    "]";

  return s.str ();
}

void oahStringsSetElementAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringsSetElementAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringsSetVariable" << " : " <<
    endl;

  if (! fStringsSetVariable.size ()) {
    os <<
      "none";
  }

  else {
    os <<
      "'";

    set<string>::const_iterator
      iBegin = fStringsSetVariable.begin (),
      iEnd   = fStringsSetVariable.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << " ";
    } // for

    os <<
      "'";
  }

  os << endl;

  gIndenter--;
}

void oahStringsSetElementAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringsSetVariable.size ()) {
    os <<
      "none";
  }

  else {
    os <<
      "'";

    set<string>::const_iterator
      iBegin = fStringsSetVariable.begin (),
      iEnd   = fStringsSetVariable.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << " ";
    } // for

    os <<
      "'";
  }

  os << endl;
}

ostream& operator<< (ostream& os, const S_oahStringsSetElementAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahStringsSetAtom oahStringsSetAtom::create (
  string       shortName,
  string       longName,
  string       description,
  string       valueSpecification,
  string       variableName,
  set<string>& stringsSetVariable)
{
  oahStringsSetAtom* o = new
    oahStringsSetAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringsSetVariable);
  assert(o!=0);
  return o;
}

oahStringsSetAtom::oahStringsSetAtom (
  string       shortName,
  string       longName,
  string       description,
  string       valueSpecification,
  string       variableName,
  set<string>& stringsSetVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringsSetVariable (
      stringsSetVariable)
{}

oahStringsSetAtom::~oahStringsSetAtom ()
{}

S_oahValuedAtom oahStringsSetAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahStringsSetAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void oahStringsSetAtom::handleValue (
  string   theString,
  ostream& os)
{
  fStringsSetVariable =
    decipherStringsSetSpecification (
      theString,
      true); // 'true' to debug it
}

void oahStringsSetAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahStringsSetAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringsSetAtom>*
    p =
      dynamic_cast<visitor<S_oahStringsSetAtom>*> (v)) {
        S_oahStringsSetAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahStringsSetAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringsSetAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahStringsSetAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringsSetAtom>*
    p =
      dynamic_cast<visitor<S_oahStringsSetAtom>*> (v)) {
        S_oahStringsSetAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahStringsSetAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahStringsSetAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahStringsSetAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahStringsSetAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    "[";

  set<string>::const_iterator
    iBegin = fStringsSetVariable.begin (),
    iEnd   = fStringsSetVariable.end (),
    i      = iBegin;

  for ( ; ; ) {
    s << (*i);
    if (++i == iEnd) break;
    s << " ";
  } // for

  s <<
    "]";

  return s.str ();
}

string oahStringsSetAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    "[";

  set<string>::const_iterator
    iBegin = fStringsSetVariable.begin (),
    iEnd   = fStringsSetVariable.end (),
    i      = iBegin;

  for ( ; ; ) {
    s << (*i);
    if (++i == iEnd) break;
    s << " ";
  } // for

  s <<
    "]";

  return s.str ();
}

void oahStringsSetAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringsSetAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringsSetVariable" << " : " <<
    endl;

  if (! fStringsSetVariable.size ()) {
    os <<
      "none";
  }

  else {
    os <<
      "'";

    set<string>::const_iterator
      iBegin = fStringsSetVariable.begin (),
      iEnd   = fStringsSetVariable.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << " ";
    } // for

    os <<
      "'";
  }

  os << endl;

  gIndenter--;
}

void oahStringsSetAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringsSetVariable.size ()) {
    os <<
      "none";
  }

  else {
    os <<
      "'";

    set<string>::const_iterator
      iBegin = fStringsSetVariable.begin (),
      iEnd   = fStringsSetVariable.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << " ";
    } // for

    os <<
      "'";
  }

  os << endl;
}

ostream& operator<< (ostream& os, const S_oahStringsSetAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahRGBColorAtom oahRGBColorAtom::create (
  string       shortName,
  string       longName,
  string       description,
  string       valueSpecification,
  string       variableName,
  msrRGBColor& RGBColorVariable,
  bool&        hasBeenSetVariable)
{
  oahRGBColorAtom* o = new
    oahRGBColorAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      RGBColorVariable,
      hasBeenSetVariable);
  assert(o!=0);
  return o;
}

oahRGBColorAtom::oahRGBColorAtom (
  string       shortName,
  string       longName,
  string       description,
  string       valueSpecification,
  string       variableName,
  msrRGBColor& RGBColorVariable,
  bool&        hasBeenSetVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fRGBColorVariable (
      RGBColorVariable),
    fHasBeenSetVariable (
      hasBeenSetVariable)
{
  fHasBeenSetVariable = false;

  fMultipleOccurrencesAllowed = true;
}

oahRGBColorAtom::~oahRGBColorAtom ()
{}

S_oahValuedAtom oahRGBColorAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahRGBColorAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void oahRGBColorAtom::handleValue (
  string   theString,
  ostream& os)
{
  fRGBColorVariable = msrRGBColor (theString);
  fHasBeenSetVariable = true;
}

void oahRGBColorAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahRGBColorAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahRGBColorAtom>*
    p =
      dynamic_cast<visitor<S_oahRGBColorAtom>*> (v)) {
        S_oahRGBColorAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahRGBColorAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahRGBColorAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahRGBColorAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahRGBColorAtom>*
    p =
      dynamic_cast<visitor<S_oahRGBColorAtom>*> (v)) {
        S_oahRGBColorAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahRGBColorAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahRGBColorAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahRGBColorAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahRGBColorAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    "[" <<
    fRGBColorVariable.asString () <<
    "]";

  return s.str ();
}

string oahRGBColorAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    "[" <<
    fRGBColorVariable.asString () <<
    "]";

  return s.str ();
}

void oahRGBColorAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "RGBColorAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fRGBColorVariable" << " : " <<
    fRGBColorVariable.asString () <<
    "fHasBeenSetVariable" << " : " <<
    booleanAsString (fHasBeenSetVariable) <<
    endl;

  gIndenter--;
}

void oahRGBColorAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    fRGBColorVariable.asString () <<
    endl <<
    setw (valueFieldWidth) <<
    "hasBeenSetVariable" <<
    " : " <<
    fRGBColorVariable.asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahRGBColorAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahIntSetAtom oahIntSetAtom::create (
  string    shortName,
  string    longName,
  string    description,
  string    valueSpecification,
  string    variableName,
  set<int>& intSetVariable)
{
  oahIntSetAtom* o = new
    oahIntSetAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      intSetVariable);
  assert(o!=0);
  return o;
}

oahIntSetAtom::oahIntSetAtom (
  string    shortName,
  string    longName,
  string    description,
  string    valueSpecification,
  string    variableName,
  set<int>& intSetVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fIntSetVariable (
      intSetVariable)
{
  fMultipleOccurrencesAllowed = true;
}

oahIntSetAtom::~oahIntSetAtom ()
{}

S_oahValuedAtom oahIntSetAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahIntSetAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void oahIntSetAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'oahIntSetAtom'" <<
      endl;
  }
#endif

  // theString contains the bar number specification
  // decipher it to extract duration and perSecond values

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'oahIntSetAtom'" <<
      endl;
  }
#endif

  string regularExpression (
    "([[:digit:]]+)");

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "There are " << smSize << " matches" <<
      " for reset measure number string '" << theString <<
      "' with regex '" << regularExpression <<
      "':" <<
      endl;

    gIndenter++;

    for (unsigned i = 0; i < smSize; ++i) {
      os <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    os << endl;

    gIndenter--;
  }
#endif

  if (smSize != 2) {
    stringstream s;

    s <<
      "-boxAroundBarNumber argument '" << theString <<
      "' is ill-formed";

    oahError (s.str ());
  }

  int lilypondMeasureNumber;
  {
    stringstream s;
    s << sm [1];
    s >> lilypondMeasureNumber;
  }

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "lilypondMeasureNumber = " <<
      lilypondMeasureNumber <<
      endl;
  }
#endif

  fIntSetVariable.insert (lilypondMeasureNumber);
}

void oahIntSetAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahIntSetAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahIntSetAtom>*
    p =
      dynamic_cast<visitor<S_oahIntSetAtom>*> (v)) {
        S_oahIntSetAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahIntSetAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahIntSetAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahIntSetAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahIntSetAtom>*
    p =
      dynamic_cast<visitor<S_oahIntSetAtom>*> (v)) {
        S_oahIntSetAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahIntSetAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahIntSetAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahIntSetAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahIntSetAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " ";

  if (! fIntSetVariable.size ()) {
    s << "empty";
  }
  else {
    set<int>::const_iterator
      iBegin = fIntSetVariable.begin (),
      iEnd   = fIntSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

string oahIntSetAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " ";

  if (! fIntSetVariable.size ()) {
    s << "empty";
  }
  else {
    set<int>::const_iterator
      iBegin = fIntSetVariable.begin (),
      iEnd   = fIntSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void oahIntSetAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahIntSetAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fIntSetVariable" << " : '" <<
    endl;

  if (! fIntSetVariable.size ()) {
    os << "empty";
  }
  else {
    set<int>::const_iterator
      iBegin = fIntSetVariable.begin (),
      iEnd   = fIntSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  os << endl;

  gIndenter--;
}

void oahIntSetAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fIntSetVariable.size ()) {
    os <<
      "empty" <<
      endl;
  }
  else {
    os << endl;
    gIndenter++;

    set<int>::const_iterator
      iBegin = fIntSetVariable.begin (),
      iEnd   = fIntSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i) << endl;
      if (++i == iEnd) break;
    } // for

    gIndenter--;
  }
}

ostream& operator<< (ostream& os, const S_oahIntSetAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahStringSetAtom oahStringSetAtom::create (
    string       shortName,
    string       longName,
    string       description,
    string       valueSpecification,
    string       variableName,
    set<string>& stringSetVariable)
{
  oahStringSetAtom* o = new
    oahStringSetAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringSetVariable);
  assert(o!=0);
  return o;
}

oahStringSetAtom::oahStringSetAtom (
    string       shortName,
    string       longName,
    string       description,
    string       valueSpecification,
    string       variableName,
    set<string>& stringSetVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringSetVariable (
      stringSetVariable)
{
  fMultipleOccurrencesAllowed = true;
}

oahStringSetAtom::~oahStringSetAtom ()
{}

S_oahValuedAtom oahStringSetAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahStringSetAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void oahStringSetAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'oahStringSetAtom'" <<
      endl;
  }
#endif

  // theString contains the bar number specification
  // decipher it to extract duration and perSecond values

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'oahStringSetAtom'" <<
      endl;
  }
#endif

  string regularExpression (
    "(.+)");

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "There are " << smSize << " matches" <<
      " for string '" << theString <<
      "' with regex '" << regularExpression <<
      "':" <<
      endl;

    gIndenter++;

    for (unsigned i = 0; i < smSize; ++i) {
      os <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    os << endl;

    gIndenter--;
  }
#endif

  if (smSize != 2) {
    stringstream s;

    s <<
      "-oahStringSetAtom argument '" << theString <<
      "' is ill-formed";

    oahError (s.str ());
  }

  string stringValue = sm [ 1 ];

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "stringValue = " <<
      stringValue <<
      endl;
  }
#endif

  fStringSetVariable.insert (stringValue);
}

void oahStringSetAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahStringSetAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringSetAtom>*
    p =
      dynamic_cast<visitor<S_oahStringSetAtom>*> (v)) {
        S_oahStringSetAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahStringSetAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringSetAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahStringSetAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringSetAtom>*
    p =
      dynamic_cast<visitor<S_oahStringSetAtom>*> (v)) {
        S_oahStringSetAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahStringSetAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahStringSetAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahStringSetAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahStringSetAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " ";

  if (! fStringSetVariable.size ()) {
    s << "empty";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

string oahStringSetAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " ";

  if (! fStringSetVariable.size ()) {
    s << "empty";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void oahStringSetAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahStringSetAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringSetVariable" << " : '" <<
    endl;

  if (! fStringSetVariable.size ()) {
    os << "empty";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  os << endl;

  gIndenter--;
}

void oahStringSetAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringSetVariable.size ()) {
    os <<
      "empty" <<
      endl;
  }
  else {
    os << endl;
    gIndenter++;

    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i) << endl;
      if (++i == iEnd) break;
    } // for

    gIndenter--;
  }
}

ostream& operator<< (ostream& os, const S_oahStringSetAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahStringToIntMapAtom oahStringToIntMapAtom::create (
  string            shortName,
  string            longName,
  string            description,
  string            valueSpecification,
  string            variableName,
  map<string, int>& stringToIntMapVariable)
{
  oahStringToIntMapAtom* o = new
    oahStringToIntMapAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringToIntMapVariable);
  assert(o!=0);
  return o;
}

oahStringToIntMapAtom::oahStringToIntMapAtom (
  string            shortName,
  string            longName,
  string            description,
  string            valueSpecification,
  string            variableName,
  map<string, int>& stringToIntMapVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringToIntMapVariable (
      stringToIntMapVariable)
{
  fMultipleOccurrencesAllowed = true;
}

oahStringToIntMapAtom::~oahStringToIntMapAtom ()
{}

S_oahValuedAtom oahStringToIntMapAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahStringToIntMapAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void oahStringToIntMapAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'oahStringToIntMapAtom'" <<
      endl;
  }
#endif

  // theString contains the string int map specification
  // decipher it to extract duration and perSecond values

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'oahStringToIntMapAtom'" <<
      endl;
  }
#endif

  string regularExpression (
    "[[:space:]]*"
    "([[:w:]]+)"
    "[[:space:]]*"
//    "=" JMI
//    "[[:space:]]*"
    "([[:digit:]]+)"
    "[[:space:]]*");

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "There are " << smSize << " matches" <<
      " for reset measure number string '" << theString <<
      "' with regex '" << regularExpression <<
      "':" <<
      endl;

    gIndenter++;

    for (unsigned i = 0; i < smSize; ++i) {
      os <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    os << endl;

    gIndenter--;
  }
#endif

  if (smSize != 3) {
    stringstream s;

    s <<
      "-resetMeasureNumber argument '" << theString <<
      "' is ill-formed";

    oahError (s.str ());
  }

  string musicXMLMeasureNumber = sm [1];

  int lilypondMeasureNumber;
  {
    stringstream s;
    s << sm [2];
    s >> lilypondMeasureNumber;
  }

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "musicXMLMeasureNumber = " <<
      musicXMLMeasureNumber <<
      endl <<
      "lilypondMeasureNumber = " <<
      lilypondMeasureNumber <<
      endl;
  }
#endif

  fStringToIntMapVariable [musicXMLMeasureNumber] = lilypondMeasureNumber;
}

void oahStringToIntMapAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahStringToIntMapAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringToIntMapAtom>*
    p =
      dynamic_cast<visitor<S_oahStringToIntMapAtom>*> (v)) {
        S_oahStringToIntMapAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahStringToIntMapAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringToIntMapAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahStringToIntMapAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringToIntMapAtom>*
    p =
      dynamic_cast<visitor<S_oahStringToIntMapAtom>*> (v)) {
        S_oahStringToIntMapAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahStringToIntMapAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahStringToIntMapAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahStringToIntMapAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahStringToIntMapAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " ";

  if (! fStringToIntMapVariable.size ()) {
    s << "empty";
  }
  else {
    map<string, int>::const_iterator
      iBegin = fStringToIntMapVariable.begin (),
      iEnd   = fStringToIntMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

string oahStringToIntMapAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " ";

  if (! fStringToIntMapVariable.size ()) {
    s << "empty";
  }
  else {
    map<string, int>::const_iterator
      iBegin = fStringToIntMapVariable.begin (),
      iEnd   = fStringToIntMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void oahStringToIntMapAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsResetMeasureNumberAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringSetVariable" << " : '" <<
    endl;

  if (! fStringToIntMapVariable.size ()) {
    os << "empty";
  }
  else {
    map<string, int>::const_iterator
      iBegin = fStringToIntMapVariable.begin (),
      iEnd   = fStringToIntMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  os << endl;

  gIndenter--;
}

void oahStringToIntMapAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringToIntMapVariable.size ()) {
    os <<
      "empty" <<
      endl;
  }
  else {
    os << endl;
    gIndenter++;

    map<string, int>::const_iterator
      iBegin = fStringToIntMapVariable.begin (),
      iEnd   = fStringToIntMapVariable.end (),
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

ostream& operator<< (ostream& os, const S_oahStringToIntMapAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahStringAndIntegerAtom oahStringAndIntegerAtom::create (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  stringVariableName,
  string& stringVariable,
  string  integerVariableName,
  int&    integerVariable)
{
  oahStringAndIntegerAtom* o = new
    oahStringAndIntegerAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      stringVariableName,
      stringVariable,
      integerVariableName,
      integerVariable);
  assert(o!=0);
  return o;
}

oahStringAndIntegerAtom::oahStringAndIntegerAtom (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  stringVariableName,
  string& stringVariable,
  string  integerVariableName,
  int&    integerVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      stringVariableName),
    fStringVariableName (
      stringVariableName),
    fStringVariable (
      stringVariable),
    fIntegerVariableName (
      integerVariableName),
    fIntegerVariable (
      integerVariable)
{
  fMultipleOccurrencesAllowed = true;
}

oahStringAndIntegerAtom::~oahStringAndIntegerAtom ()
{}

S_oahValuedAtom oahStringAndIntegerAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahStringAndIntegerAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void oahStringAndIntegerAtom::handleValue (
  string   theString,
  ostream& os)
{
  // theString contains the string and integer values

  // check whether it is well-formed
  string regularExpression (
    "(.+)[[:space:]]+([[:digit:]]+)");

  regex e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "There are " << smSize << " matches" <<
      " for integer string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;

    for (unsigned i = 0; i < smSize; ++i) {
      os <<
        "[" << sm [i] << "] ";
    } // for

    os << endl;
  }
#endif

  if (smSize == 3) {
    fStringVariable = sm [ 1 ];

    // leave the low level details to the STL...
    int integerValue;
    {
      stringstream s;
      s << sm [ 2 ];
      s >> integerValue;
    }
    fIntegerVariable = integerValue;
  }

  else {
    stringstream s;

    s <<
      "integer value '" << theString <<
      "' for option '" << fetchNames () <<
      "' is ill-formed";

    oahError (s.str ());
  }
}

void oahStringAndIntegerAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahStringAndIntegerAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringAndIntegerAtom>*
    p =
      dynamic_cast<visitor<S_oahStringAndIntegerAtom>*> (v)) {
        S_oahStringAndIntegerAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahStringAndIntegerAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringAndIntegerAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahStringAndIntegerAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringAndIntegerAtom>*
    p =
      dynamic_cast<visitor<S_oahStringAndIntegerAtom>*> (v)) {
        S_oahStringAndIntegerAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahStringAndIntegerAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahStringAndIntegerAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahStringAndIntegerAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahStringAndIntegerAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    "\"" <<
    fStringVariable <<
    "\" " <<
    fIntegerVariable;

  return s.str ();
}

string oahStringAndIntegerAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    "\"" <<
    fStringVariable <<
    "\" " <<
    fIntegerVariable;

  return s.str ();
}

void oahStringAndIntegerAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringAndIntegerAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "stringVariableName" << " : " <<
    fStringVariableName <<
    endl <<
    setw (fieldWidth) <<
    "stringVariable" << " : " <<
    "\"" << fStringVariable << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "integerVariableName" << " : " <<
    fIntegerVariableName <<
    endl <<
    setw (fieldWidth) <<
    "integerVariable" << " : " <<
    fIntegerVariable <<
    endl;

  gIndenter--;
}

void oahStringAndIntegerAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fStringVariableName <<
    " : " <<
    "\"" << fStringVariable << "\"" <<
    endl <<

    setw (valueFieldWidth) <<
    fIntegerVariableName <<
    " : " <<
    "\"" << fIntegerVariable << "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahStringAndIntegerAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahStringAndTwoIntegersAtom oahStringAndTwoIntegersAtom::create (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& stringVariable,
  int&    primaryIntegerVariable,
  int&    secondaryIntegerVariable)
{
  oahStringAndTwoIntegersAtom* o = new
    oahStringAndTwoIntegersAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringVariable,
      primaryIntegerVariable,
      secondaryIntegerVariable);
  assert(o!=0);
  return o;
}

oahStringAndTwoIntegersAtom::oahStringAndTwoIntegersAtom (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& stringVariable,
  int&    primaryIntegerVariable,
  int&    secondaryIntegerVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringVariable (
      stringVariable),
    fPrimaryIntegerVariable (
      primaryIntegerVariable),
    fSecondaryIntegerVariable (
      secondaryIntegerVariable)
{
  fMultipleOccurrencesAllowed = true;
}

oahStringAndTwoIntegersAtom::~oahStringAndTwoIntegersAtom ()
{}

S_oahValuedAtom oahStringAndTwoIntegersAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahStringAndTwoIntegersAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void oahStringAndTwoIntegersAtom::handleValue (
  string   theString,
  ostream& os)
{
  // theString contains the string and integer values

  // check whether it is well-formed
  string regularExpression (
    "([[:alpha:]]+)[[:space:]]+([[:digit:]]+)[[:space:]]+([[:digit:]]+)");

  regex e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "There are " << smSize << " matches" <<
      " for integer string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;

    for (unsigned i = 0; i < smSize; ++i) {
      os <<
        "[" << sm [i] << "] ";
    } // for

    os << endl;
  }
#endif

  if (smSize == 4) {
    fStringVariable = sm [ 1 ];

    // leave the low level details to the STL...
    int integerValue;
    {
      stringstream s;
      s << sm [ 2 ];
      s >> integerValue;
    }
    fPrimaryIntegerVariable = integerValue; // JMI

    {
      stringstream s;
      s << sm [ 3 ];
      s >> integerValue;
    }
    fSecondaryIntegerVariable = integerValue;
  }

  else {
    stringstream s;

    s <<
      "integer value '" << theString <<
      "' for option '" << fetchNames () <<
      "' is ill-formed";

    oahError (s.str ());
  }
}

void oahStringAndTwoIntegersAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahStringAndTwoIntegersAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringAndTwoIntegersAtom>*
    p =
      dynamic_cast<visitor<S_oahStringAndTwoIntegersAtom>*> (v)) {
        S_oahStringAndTwoIntegersAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahStringAndTwoIntegersAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringAndTwoIntegersAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahStringAndTwoIntegersAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringAndTwoIntegersAtom>*
    p =
      dynamic_cast<visitor<S_oahStringAndTwoIntegersAtom>*> (v)) {
        S_oahStringAndTwoIntegersAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahStringAndTwoIntegersAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahStringAndTwoIntegersAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahStringAndTwoIntegersAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahStringAndTwoIntegersAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    "\"" <<
    fStringVariable <<
    "\" " <<
    fPrimaryIntegerVariable <<
    " " <<
    fSecondaryIntegerVariable;

  return s.str ();
}

string oahStringAndTwoIntegersAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    "\"" <<
    fStringVariable <<
    "\" " <<
    fPrimaryIntegerVariable <<
    " " <<
    fSecondaryIntegerVariable;

  return s.str ();
}

void oahStringAndTwoIntegersAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringAndIntegerAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringVariable" << " : " <<
    "\"" << fStringVariable << "\"" <<
    "fPrimaryIntegerVariable" << " : " <<
    fPrimaryIntegerVariable <<
    "fSecondaryIntegerVariable" << " : " <<
    fSecondaryIntegerVariable <<
    endl;

  gIndenter--;
}

void oahStringAndTwoIntegersAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    "\"" << fStringVariable << "\"" <<
    endl <<
    setw (valueFieldWidth) <<
    "stringVariable" <<
    " : " <<
    "\"" << fStringVariable << "\"" <<
    endl <<
    setw (valueFieldWidth) <<
    "primaryIntegerVariable" <<
    " : " <<
    fPrimaryIntegerVariable <<
    endl <<
    setw (valueFieldWidth) <<
    "secondaryIntegerVariable" <<
    " : " <<
    fSecondaryIntegerVariable <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahStringAndTwoIntegersAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahLengthUnitKindAtom oahLengthUnitKindAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msrLengthUnitKind& lengthUnitKindVariable)
{
  oahLengthUnitKindAtom* o = new
    oahLengthUnitKindAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      lengthUnitKindVariable);
  assert(o!=0);
  return o;
}

oahLengthUnitKindAtom::oahLengthUnitKindAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msrLengthUnitKind& lengthUnitKindVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fLengthUnitKindVariable (
      lengthUnitKindVariable)
{}

oahLengthUnitKindAtom::~oahLengthUnitKindAtom ()
{}

S_oahValuedAtom oahLengthUnitKindAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahLengthUnitKindAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void oahLengthUnitKindAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'oahLengthUnitKindAtom'" <<
      endl;
  }
#endif

  // theString contains the score output kind:
  // is it in the score output kinds map?

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'oahLengthUnitKindAtom'" <<
      endl;
  }
#endif

  map<string, msrLengthUnitKind>::const_iterator
    it =
      gMsrLengthUnitKindsMap.find (
        theString);

  if (it == gMsrLengthUnitKindsMap.end ()) {
    // no, score output kind is unknown in the map

    stringstream s;

    s <<
      "LPSR score output kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gMsrLengthUnitKindsMap.size () <<
      " known LPSR score output kinds are:" <<
      endl;

    gIndenter++;

    s <<
      existingMsrLengthUnitKinds (K_NAMES_LIST_MAX_LENGTH);

    gIndenter--;

    oahError (s.str ());
  }

  setLengthUnitKindVariable (
    (*it).second);
}

void oahLengthUnitKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahLengthUnitKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahLengthUnitKindAtom>*
    p =
      dynamic_cast<visitor<S_oahLengthUnitKindAtom>*> (v)) {
        S_oahLengthUnitKindAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahLengthUnitKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahLengthUnitKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahLengthUnitKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahLengthUnitKindAtom>*
    p =
      dynamic_cast<visitor<S_oahLengthUnitKindAtom>*> (v)) {
        S_oahLengthUnitKindAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahLengthUnitKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahLengthUnitKindAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahLengthUnitKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahLengthUnitKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    msrLengthUnitKindAsString (fLengthUnitKindVariable);

  return s.str ();
}

string oahLengthUnitKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    msrLengthUnitKindAsString (fLengthUnitKindVariable);

  return s.str ();
}

void oahLengthUnitKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsMsrLengthUnitKindAtom:" <<
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
    "fOptionsLengthUnitKindVariable" << " : \"" <<
    msrLengthUnitKindAsString (
      fLengthUnitKindVariable) <<
    "\"" <<
    endl;
}

void oahLengthUnitKindAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    msrLengthUnitKindAsString (
      fLengthUnitKindVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahLengthUnitKindAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahLengthAtom oahLengthAtom::create (
  string     shortName,
  string     longName,
  string     description,
  string     valueSpecification,
  string     variableName,
  msrLength& lengthVariable)
{
  oahLengthAtom* o = new
    oahLengthAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      lengthVariable);
  assert(o!=0);
  return o;
}

oahLengthAtom::oahLengthAtom (
  string     shortName,
  string     longName,
  string     description,
  string     valueSpecification,
  string     variableName,
  msrLength& lengthVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fLengthVariable (
      lengthVariable)
{}

oahLengthAtom::~oahLengthAtom ()
{}

S_oahValuedAtom oahLengthAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahLengthAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void oahLengthAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'oahLengthAtom'" <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'oahLengthAtom'" <<
      endl;
  }
#endif

  // theString contains the score output kind:

  // check whether it is well-formed
   // no sign, a '-' would be handled as an option name JMI   "([+|-]?)"
  string regularExpression (
    "([[:digit:]]+)(.[[:digit:]]*)?([[:alpha:]]{2,})"
    );

  regex e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "There are " << smSize << " matches" <<
      " for integer string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;

    for (unsigned i = 0; i < smSize; ++i) {
      os <<
        "[" << sm [i] << "] ";
    } // for

    os << endl;
  }
#endif

  if (smSize == 4) {
    // leave the low level details to the STL...
    float floatValue;
    {
      stringstream s;
      // concatenate the integer and decimal parts
      s << sm [ 1 ] << sm [ 2 ];
      s >> floatValue;
    }

    string lengthUnitName = sm [ 3 ];

    // is lengthUnitName known in the the length unit names map?
    map<string, msrLengthUnitKind>::const_iterator
      it =
        gMsrLengthUnitKindsMap.find (
          lengthUnitName);

    if (it == gMsrLengthUnitKindsMap.end ()) {
      // no, length unit name is unknown in the map

      stringstream s;

      s <<
        "length unit name '" << lengthUnitName <<
        "' is unknown" <<
        endl <<
        "The " <<
        gMsrLengthUnitKindsMap.size () <<
        " known length unit names are:" <<
        endl;

      gIndenter++;

      s <<
        existingMsrLengthUnitKinds (K_NAMES_LIST_MAX_LENGTH);

      gIndenter--;

      oahError (s.str ());
    }

    setLengthVariable (
      msrLength (
        (*it).second,
        floatValue));
  }

  else {
    stringstream s;

    s <<
      "length value '" << theString <<
      "' for option '" << fetchNames () <<
      "' is ill-formed";

    oahError (s.str ());
  }
}

void oahLengthAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahLengthAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahLengthAtom>*
    p =
      dynamic_cast<visitor<S_oahLengthAtom>*> (v)) {
        S_oahLengthAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahLengthAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahLengthAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahLengthAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahLengthAtom>*
    p =
      dynamic_cast<visitor<S_oahLengthAtom>*> (v)) {
        S_oahLengthAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahLengthAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahLengthAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahLengthAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahLengthAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    fLengthVariable.asString ();

  return s.str ();
}

string oahLengthAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    fLengthVariable.asString ();

  return s.str ();
}

void oahLengthAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsMsrLengthAtom:" <<
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
    "fOptionsLengthVariable" << " : " <<
    fLengthVariable.asString () <<
    endl;
}

void oahLengthAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    fLengthVariable.asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahLengthAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahOptionNameHelpAtom oahOptionNameHelpAtom::create (
  string shortName,
  string longName,
  string description,
  string valueSpecification,
  string variableName,
  string& stringVariable,
  string  defaultOptionName)
{
  oahOptionNameHelpAtom* o = new
    oahOptionNameHelpAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringVariable,
      defaultOptionName);
  assert(o!=0);
  return o;
}

oahOptionNameHelpAtom::oahOptionNameHelpAtom (
  string shortName,
  string longName,
  string description,
  string valueSpecification,
  string variableName,
  string& stringVariable,
  string  defaultOptionName)
  : oahStringWithDefaultValueAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringVariable,
      defaultOptionName)
{}

oahOptionNameHelpAtom::~oahOptionNameHelpAtom ()
{}

S_oahValuedAtom oahOptionNameHelpAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahOptionNameHelpAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void oahOptionNameHelpAtom::handleValue (
  string   theString,
  ostream& os)
{
  // delegate this to the handler
  fHandlerUpLink->
    printOptionNameIntrospectiveHelp (
      os,
      theString);

  // register 'option name help' action in options groups's options handler upLink
  fSubGroupUpLink->
    getGroupUpLink ()->
      getHandlerUpLink ()->
        setOptionsHandlerFoundAHelpOption ();
}

void oahOptionNameHelpAtom::handleDefaultValue ()
{
  // delegate this to the handler
  fHandlerUpLink->
    printOptionNameIntrospectiveHelp (
      fHandlerUpLink->getHandlerLogOstream (),
      fDefaultStringValue);
}

void oahOptionNameHelpAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahOptionNameHelpAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahOptionNameHelpAtom>*
    p =
      dynamic_cast<visitor<S_oahOptionNameHelpAtom>*> (v)) {
        S_oahOptionNameHelpAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahOptionNameHelpAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahOptionNameHelpAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahOptionNameHelpAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahOptionNameHelpAtom>*
    p =
      dynamic_cast<visitor<S_oahOptionNameHelpAtom>*> (v)) {
        S_oahOptionNameHelpAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahOptionNameHelpAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahOptionNameHelpAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahOptionNameHelpAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahOptionNameHelpAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " << fVariableName;

  return s.str ();
}

string oahOptionNameHelpAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " << fVariableName;

  return s.str ();
}

void oahOptionNameHelpAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionNameHelpAtom:" <<
    endl;

  gIndenter++;

  printOptionEssentials (
    os, fieldWidth);

  gIndenter--;
}

void oahOptionNameHelpAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_oahOptionNameHelpAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahSubGroup oahSubGroup::create (
  string                  subGroupHeader,
  string                  shortName,
  string                  longName,
  string                  description,
  oahElementVisibilityKind optionVisibilityKind,
  S_oahGroup              groupUpLink)
{
  oahSubGroup* o = new
    oahSubGroup (
      subGroupHeader,
      shortName,
      longName,
      description,
      optionVisibilityKind,
      groupUpLink);
  assert(o!=0);
  return o;
}

oahSubGroup::oahSubGroup (
  string                  subGroupHeader,
  string                  shortName,
  string                  longName,
  string                  description,
  oahElementVisibilityKind optionVisibilityKind,
  S_oahGroup              groupUpLink)
  : oahElement (
      shortName,
      longName,
      description,
      optionVisibilityKind)
{
  fGroupUpLink = groupUpLink;

  fSubGroupHeader = subGroupHeader;
}

oahSubGroup::~oahSubGroup ()
{}

void oahSubGroup::underlineSubGroupHeader (ostream& os) const
{
  /* JMI ???
  for (unsigned int i = 0; i < fSubGroupHeader.size (); i++) {
    os << "-";
  } // for
  os << endl;
  */
  os << "--------------------------" << endl;
}

void oahSubGroup::registerSubGroupInHandler (
  S_oahHandler handler)
{
  handler->
    registerElementInHandler (this);

  fHandlerUpLink = handler;

  for (
    list<S_oahAtom>::const_iterator
      i = fAtomsList.begin ();
    i != fAtomsList.end ();
    i++
  ) {
    // register the options sub group
    (*i)->
      registerAtomInHandler (
        handler);
  } // for
}

void oahSubGroup::appendAtomToSubGroup (
  S_oahAtom oahAtom)
{
  // sanity check
  msrAssert (
    oahAtom != nullptr,
    "oahAtom is null");

  // append atom
  fAtomsList.push_back (
    oahAtom);

  // set atom subgroup upLink
  oahAtom->
    setSubGroupUpLink (this);
}

S_oahElement oahSubGroup::fetchOptionByName (
  string name)
{
  S_oahElement result;

  for (
    list<S_oahAtom>::const_iterator
      i = fAtomsList.begin ();
    i != fAtomsList.end ();
    i++
  ) {
    // search name in the options group
    result =
      (*i)->fetchOptionByName (name);

    if (result) {
      break;
    }
  } // for

  return result;
}

void oahSubGroup::checkOptionsConsistency ()
{}

void oahSubGroup::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahSubGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahSubGroup>*
    p =
      dynamic_cast<visitor<S_oahSubGroup>*> (v)) {
        S_oahSubGroup elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahSubGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahSubGroup::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahSubGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahSubGroup>*
    p =
      dynamic_cast<visitor<S_oahSubGroup>*> (v)) {
        S_oahSubGroup elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahSubGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahSubGroup::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahSubGroup::browseData ()" <<
      ", " << fSubGroupHeader <<
      endl;
  }
#endif

  // browse the atoms
  if (fAtomsList.size ()) {
    for (
      list<S_oahAtom>::const_iterator i = fAtomsList.begin ();
      i != fAtomsList.end ();
      i++
    ) {
      S_oahAtom atom = (*i);

      // browse the atom
      /* JMI ???
      gOutputOstream <<
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
  const int fieldWidth = 27;

  os <<
   "SubGroup:" <<
    endl;

  gIndenter++;

  oahElement::printOptionEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fElementVisibilityKind" << " : " <<
      optionVisibilityKindAsString (
        fElementVisibilityKind) <<
    endl <<
    endl;

  os <<
    "AtomsList (" <<
    singularOrPlural (
      fAtomsList.size (), "atom",  "atoms") <<
    "):" <<
    endl;

  if (fAtomsList.size ()) {
    os << endl;

    gIndenter++;

    list<S_oahAtom>::const_iterator
      iBegin = fAtomsList.begin (),
      iEnd   = fAtomsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the atom
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }

  gIndenter--;
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
    case kElementVisibilityAlways:
      os <<
        ":";
      break;

    case kElementVisibilityHiddenByDefault:
      os <<
        " (hidden by default)";
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
    case kElementVisibilityAlways:
      os <<
        ":";
      break;

    case kElementVisibilityHiddenByDefault:
      os <<
        " (hidden by default)";
      break;
  } // switch

  os << endl;

}

void oahSubGroup::printHelp (ostream& os)
{
  // print the header and option names
  printSubGroupHeader (os);

  // print the description if any
  if (fDescription.size ()) {
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fDescription);
    gIndenter--;

    os << endl;
  }

  // print the options atoms if relevant
  switch (fElementVisibilityKind) {
    case kElementVisibilityAlways:
      if (fAtomsList.size ()) {
        gIndenter++;

        list<S_oahAtom>::const_iterator
          iBegin = fAtomsList.begin (),
          iEnd   = fAtomsList.end (),
          i      = iBegin;
        for ( ; ; ) {
          S_oahAtom oahAtom = (*i);
          bool
            oahAtomIsHidden =
              oahAtom->getIsHidden ();

          // print the atom help unless it is hidden
          if (! oahAtomIsHidden) {
            oahAtom->
              printHelp (os);
          }
          if (++i == iEnd) break;
          if (! oahAtomIsHidden) {
   // JMI         os << endl;
          }
        } // for

        gIndenter--;
      }
      break;

    case kElementVisibilityHiddenByDefault:
      break;
  } // switch

  // register help print action in options groups's options handler upLink
  fGroupUpLink->
    getHandlerUpLink ()->
      setOptionsHandlerFoundAHelpOption ();
}

void oahSubGroup::printHelpWithHeaderWidth (
  ostream& os,
  int      subGroupHeaderWidth)
{
  // print the header and option names
  printSubGroupHeaderWithHeaderWidth (
    os,
    subGroupHeaderWidth);

  // print the description if any
  if (fDescription.size ()) {
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fDescription);
    gIndenter--;

    os << endl;
  }

  // print the options atoms if relevant
  switch (fElementVisibilityKind) {
    case kElementVisibilityAlways:
      if (fAtomsList.size ()) {
        gIndenter++;

        list<S_oahAtom>::const_iterator
          iBegin = fAtomsList.begin (),
          iEnd   = fAtomsList.end (),
          i      = iBegin;
        for ( ; ; ) {
          S_oahAtom oahAtom = (*i);
          bool
            oahAtomIsHidden =
              oahAtom->getIsHidden ();

          // print the atom help unless it is hidden
          if (! oahAtomIsHidden) {
            oahAtom->
              printHelp (os);
          }
          if (++i == iEnd) break;
          if (! oahAtomIsHidden) {
   // JMI         os << endl;
          }
        } // for

        gIndenter--;
      }
      break;

    case kElementVisibilityHiddenByDefault:
      break;
  } // switch

  // register help print action in options groups's options handler upLink
  fGroupUpLink->
    getHandlerUpLink ()->
      setOptionsHandlerFoundAHelpOption ();
}

void oahSubGroup::printSubGroupHelp (ostream& os) const
{
  // print the header and option names
  printSubGroupHeader (os);

  // print the description if any
  if (fDescription.size ()) {
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;
    gIndenter--;

    os << endl;
  }

  if (fAtomsList.size ()) {
    gIndenter++;

    list<S_oahAtom>::const_iterator
      iBegin = fAtomsList.begin (),
      iEnd   = fAtomsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the atom help
      (*i)->printHelp (os);
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for

    gIndenter--;
  }

  os << endl;

  // register help print action in options groups's options handler upLink
  fGroupUpLink->
    getHandlerUpLink ()->
      setOptionsHandlerFoundAHelpOption ();

}

S_oahValuedAtom oahSubGroup::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahSubGroup" <<
      endl;
  }
#endif

  printSubGroupHelp (os);

  // no option value is needed
  return nullptr;
}

void oahSubGroup::printOptionsSummary (
  ostream& os) const
{
  // fetch maximum subgroups help headers size
  int maximumSubGroupsHelpHeadersSize =
    getGroupUpLink ()->
      getHandlerUpLink ()->
        getMaximumSubGroupsHeadersSize ();

  // fetch maximum short name width
  int maximumShortNameWidth =
    getGroupUpLink ()->
      getHandlerUpLink ()->
        getMaximumShortNameWidth ();

  // print the summary
  os << left <<
    setw (maximumSubGroupsHelpHeadersSize) <<
    fSubGroupHeader <<
    " " <<
    fetchNamesInColumnsBetweenParentheses (
      maximumShortNameWidth);

  switch (fElementVisibilityKind) {
    case kElementVisibilityAlways:
      break;

    case kElementVisibilityHiddenByDefault:
      os <<
        " (hidden by default)";
      break;
  } // switch

  os << endl;

  // print the description if any
  if (fDescription.size ()) {
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;
    gIndenter--;
  }

  // register help print action in options groups's options handler upLink
  fGroupUpLink->
    getHandlerUpLink ()->
      setOptionsHandlerFoundAHelpOption ();
}

void oahSubGroup::printSubGroupSpecificHelpOrOptionsSummary (
  ostream&      os,
  S_oahSubGroup subGroup) const
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
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
  // print the header
  os <<
    fSubGroupHeader <<
    " " <<
    fetchNamesBetweenParentheses () <<
    ":" <<
    endl <<
    endl;

  // underline the options subgroup header
// JMI  underlineSubGroupHeader (os);

  // print the subgroup atoms
  if (fAtomsList.size ()) {
    gIndenter++;

    list<S_oahAtom>::const_iterator
      iBegin = fAtomsList.begin (),
      iEnd   = fAtomsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahAtom oahAtom = (*i);

      if (oahAtom == targetAtom) {
        // print the target atom's help
        // target options atom's help
        (*i)->
          printHelp (
            os);
      }
      if (++i == iEnd) break;
      if (oahAtom == targetAtom) {
        os << endl;
      }
    } // for

    gIndenter--;
  }

  // register help print action in options groups's options handler upLink
  fGroupUpLink->
    getHandlerUpLink ()->
      setOptionsHandlerFoundAHelpOption ();
}

void oahSubGroup::printSubGroupOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // print the header
  os <<
    fSubGroupHeader <<
    " " <<
    fetchNamesBetweenParentheses () <<
    ":" <<
    endl;

  // underline the options subgroup header
// JMI  underlineSubGroupHeader (os);

  // print the subgroup atoms values
  if (fAtomsList.size ()) {
    gIndenter++;

    list<S_oahAtom>::const_iterator
      iBegin = fAtomsList.begin (),
      iEnd   = fAtomsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the atom values
      (*i)->
        printAtomOptionsValues (
          os, valueFieldWidth);
      if (++i == iEnd) break;
  //    os << endl;
    } // for

    gIndenter--;
  }
}

ostream& operator<< (ostream& os, const S_oahSubGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahGroup oahGroup::create (
  string                  header,
  string                  shortName,
  string                  longName,
  string                  description,
  oahElementVisibilityKind optionVisibilityKind,
  S_oahHandler            groupHandlerUpLink)
{
  oahGroup* o = new
    oahGroup (
      header,
      shortName,
      longName,
      description,
      optionVisibilityKind,
      groupHandlerUpLink);
  assert(o!=0);
  return o;
}

oahGroup::oahGroup (
  string                  header,
  string                  shortName,
  string                  longName,
  string                  description,
  oahElementVisibilityKind optionVisibilityKind,
  S_oahHandler            groupHandlerUpLink)
  : oahElement (
      shortName,
      longName,
      description,
      optionVisibilityKind)
{
  fHandlerUpLink = groupHandlerUpLink;

  fGroupHeader = header;
}

oahGroup::~oahGroup ()
{}

void oahGroup::underlineGroupHeader (ostream& os) const
{
  /* JMI
  for (unsigned int i = 0; i < fGroupHeader.size (); i++) {
    os << "-";
  } // for
  os << endl;
  */
  os << "--------------------------" << endl;
}

void oahGroup::registerGroupInHandler (
  S_oahHandler handler)
{
  // sanity check
  msrAssert (
    handler != nullptr,
    "handler is null");

  // set options handler upLink
  fHandlerUpLink = handler;

  // register options group in options handler
  handler->
    registerElementInHandler (this);

  for (
    list<S_oahSubGroup>::const_iterator
      i = fSubGroupsList.begin ();
    i != fSubGroupsList.end ();
    i++
  ) {
    // register the options sub group
    (*i)->
      registerSubGroupInHandler (
        handler);
  } // for
}

void  oahGroup::appendSubGroupToGroup (
  S_oahSubGroup subGroup)
{
  // sanity check
  msrAssert (
    subGroup != nullptr,
    "subGroup is null");

  // append options subgroup
  fSubGroupsList.push_back (
    subGroup);

  // set options subgroup group upLink
  subGroup->
    setGroupUpLink (this);
}

S_oahElement oahGroup::fetchOptionByName (
  string name)
{
  S_oahElement result;

  for (
    list<S_oahSubGroup>::const_iterator
      i = fSubGroupsList.begin ();
    i != fSubGroupsList.end ();
    i++
  ) {
    // search name in the options group
    result =
      (*i)->fetchOptionByName (name);

    if (result) {
      break;
    }
  } // for

  return result;
}

void oahGroup::handleAtomValue (
  ostream&  os,
  S_oahAtom atom,
  string    theString)
{
  os <<
    endl <<
    "-==> Options atom '" <<
    atom <<
    "' with value '" <<
    theString <<
    "' is not handled" <<
    endl <<
    endl;
}

void oahGroup::checkOptionsConsistency ()
{}

void oahGroup::checkGroupSubGroupsOptionsConsistency ()
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "Checking the consistency of OAH group \"" <<
      fGroupHeader <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;

  // check the subgroups options consistency
  if (fSubGroupsList.size ()) {
    for (
      list<S_oahSubGroup>::const_iterator i = fSubGroupsList.begin ();
      i != fSubGroupsList.end ();
      i++
    ) {
      S_oahSubGroup subgroup = (*i);

      // check the subgroup
      subgroup->
        checkOptionsConsistency ();
    } // for
  }

  gIndenter--;

  // check the group's own consistency
  this->checkOptionsConsistency ();
}

void oahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahGroup>*
    p =
      dynamic_cast<visitor<S_oahGroup>*> (v)) {
        S_oahGroup elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahGroup>*
    p =
      dynamic_cast<visitor<S_oahGroup>*> (v)) {
        S_oahGroup elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahGroup::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahGroup::browseData ()" <<
      ", " << fGroupHeader <<
      endl;
  }
#endif

  // browse the subGroups
  if (fSubGroupsList.size ()) {
    for (
      list<S_oahSubGroup>::const_iterator i = fSubGroupsList.begin ();
      i != fSubGroupsList.end ();
      i++
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
  const int fieldWidth = 27;

  os <<
    "Group:" <<
    endl;

  gIndenter++;

  oahElement::printOptionEssentials (
    os, fieldWidth);

  os <<
    "SubgroupsList (" <<
    singularOrPlural (
      fSubGroupsList.size (), "element",  "elements") <<
    "):" <<
    endl;

  if (fSubGroupsList.size ()) {
    os << endl;

    gIndenter++;

    list<S_oahSubGroup>::const_iterator
      iBegin = fSubGroupsList.begin (),
      iEnd   = fSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }

  gIndenter--;
}

void oahGroup::printGroupHeader (ostream& os) const
{
  // print the header and option names
  os <<
    fGroupHeader <<
    " " <<
    fetchNamesBetweenParentheses ();

  switch (fElementVisibilityKind) {
    case kElementVisibilityAlways:
      os <<
        ":";
      break;

    case kElementVisibilityHiddenByDefault:
      os <<
        " (hidden by default)";
      break;
  } // switch

  os << endl;
}

void oahGroup::printHelp (ostream& os)
{
  // print the header and option names
  printGroupHeader (os);

  // print the description if any
  if (fDescription.size ()) {
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;
    gIndenter--;
  }

  // underline the options group header
  underlineGroupHeader (os);

/* JMI
  // print the options subgroups
  if (fSubGroupsList.size ()) {
    gIndenter++;

    list<S_oahSubGroup>::const_iterator
      iBegin = fSubGroupsList.begin (),
      iEnd   = fSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup help
      (*i)->printHelp (os);
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for

    gIndenter--;
  }
*/

  // print the options subgroups
  if (fSubGroupsList.size ()) {
    // compute the maximun sub group header length
    int maximumSubGroupHeaderLength = 0;

    for (
      list<S_oahSubGroup>::const_iterator i = fSubGroupsList.begin ();
      i != fSubGroupsList.end ();
      i++
    ) {
      string subGroupHeader  = (*i)->getSubGroupHeader ();
      int subGroupHeaderSize = subGroupHeader.size ();

      if (subGroupHeaderSize > maximumSubGroupHeaderLength) {
        maximumSubGroupHeaderLength = subGroupHeaderSize;
      }
    } // for

    maximumSubGroupHeaderLength += 3; // to have some more spaces

    gIndenter++;

    list<S_oahSubGroup>::const_iterator
      iBegin = fSubGroupsList.begin (),
      iEnd   = fSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahSubGroup subGroup = (*i);

      // print the options subgroup help
      subGroup->printHelpWithHeaderWidth (
        os,
        maximumSubGroupHeaderLength);

      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for

    gIndenter--;
  }

  // register help print action in options handler upLink
  fHandlerUpLink->setOptionsHandlerFoundAHelpOption ();
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
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;
    gIndenter--;

    os << endl;
  }

  // underline the options group header
  underlineGroupHeader (os);

  // print the target options subgroup
  if (fSubGroupsList.size ()) {
    gIndenter++;

    list<S_oahSubGroup>::const_iterator
      iBegin = fSubGroupsList.begin (),
      iEnd   = fSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahSubGroup
        subGroup = (*i);

      if (subGroup == targetSubGroup) {
        // print the target options subgroup help
        subGroup->
          printSubGroupHelp (
            os);
      }
      if (++i == iEnd) break;
      if (subGroup == targetSubGroup) {
        os << endl;
      }
    } // for

    gIndenter--;
  }
}

void oahGroup::printGroupAndSubGroupAndAtomHelp (
  ostream&      os,
  S_oahSubGroup targetSubGroup,
  S_oahAtom     targetAtom) const
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
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;
    gIndenter--;

    os << endl;
  }

  // underline the options group header
  underlineGroupHeader (os);

  // print the target options subgroup
  if (fSubGroupsList.size ()) {
    gIndenter++;

    list<S_oahSubGroup>::const_iterator
      iBegin = fSubGroupsList.begin (),
      iEnd   = fSubGroupsList.end (),
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
      if (subGroup == targetSubGroup) {
        os << endl;
      }
    } // for

    os << endl;

    gIndenter--;
  }

  // register help print action in options handler upLink
  fHandlerUpLink->setOptionsHandlerFoundAHelpOption ();
}

S_oahValuedAtom oahGroup::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << optionName << "' is a oahGroup" <<
      endl;
  }
#endif

  printHelp (os);

  // no option value is needed
  return nullptr;
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
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;
    gIndenter--;
  }

  // underline the options group header
  underlineGroupHeader (os);

  // print the options subgroups
  if (fSubGroupsList.size ()) {
    gIndenter++;

    list<S_oahSubGroup>::const_iterator
      iBegin = fSubGroupsList.begin (),
      iEnd   = fSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup description
      (*i)->
        printOptionsSummary (os);
      if (++i == iEnd) break;
 //     os << endl;
    } // for

    gIndenter--;
  }

  // register help print action in options handler upLink
  fHandlerUpLink->setOptionsHandlerFoundAHelpOption ();
}

void oahGroup::printGroupAndSubGroupSpecificHelp (
  ostream&      os,
  S_oahSubGroup subGroup) const
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os << "oahGroup::printGroupAndSubGroupSpecificHelp" << endl;
  }
#endif

  // the description is the header of the information
  os <<
    fGroupHeader <<
    " " <<
    fetchNamesBetweenParentheses () <<
    ":" <<
    endl;

  // print the description if any
  if (fDescription.size ()) {
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;
    gIndenter--;
  }

  // underline the options group header
  underlineGroupHeader (os);

  // print the options subgroups
  if (fSubGroupsList.size ()) {
    gIndenter++;

    list<S_oahSubGroup>::const_iterator
      iBegin = fSubGroupsList.begin (),
      iEnd   = fSubGroupsList.end (),
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

    gIndenter--;
  }

  // register help print action in options handler upLink
  fHandlerUpLink->setOptionsHandlerFoundAHelpOption ();
}

void oahGroup::printGroupOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // print the header
  os <<
    fGroupHeader <<
    " " <<
    fetchNamesBetweenParentheses () <<
    ":" <<
    endl;

  // underline the options group header
  underlineGroupHeader (os);

  // print the options subgroups values
  if (fSubGroupsList.size ()) {
    gIndenter++;

    list<S_oahSubGroup>::const_iterator
      iBegin = fSubGroupsList.begin (),
      iEnd   = fSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup values
      (*i)->
        printSubGroupOptionsValues (
          os, valueFieldWidth);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }
}

ostream& operator<< (ostream& os, const S_oahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
/* JMI
S_oahHandler oahHandler::create (
  string           handlerHeader,
  string           handlerValuesHeader,
  string           handlerShortName,
  string           handlerLongName,
  string           handlerSummaryShortName,
  string           handlerSummaryLongName,
  string           handlerPreamble,
  string           handlerUsage,
  string           handlerDescription,
  indentedOstream& handlerLogOstream)
{
  oahHandler* o = new
    oahHandler (
      handlerHeader,
      handlerValuesHeader,
      handlerShortName,
      handlerLongName,
      handlerSummaryShortName,
      handlerSummaryLongName,
      handlerPreamble,
      handlerUsage,
      handlerDescription,
      handlerLogOstream);
  assert(o!=0);
  return o;
}
*/

oahHandler::oahHandler (
  string           handlerHeader,
  string           handlerValuesHeader,
  string           handlerShortName,
  string           handlerLongName,
  string           handlerSummaryShortName,
  string           handlerSummaryLongName,
  string           handlerPreamble,
  string           handlerUsage,
  string           handlerDescription,
  indentedOstream& handlerLogOstream)
  : oahElement (
      handlerShortName,
      handlerLongName,
      handlerDescription,
      kElementVisibilityAlways),
    fHandlerLogOstream (
      handlerLogOstream)
{
  fHandlerHeader =
    handlerHeader;

  fHandlerValuesHeader =
    handlerValuesHeader;

  fHandlerSummaryShortName =
    handlerSummaryShortName;
  fHandlerSummaryLongName =
    handlerSummaryLongName;

  fHandlerPreamble =
    handlerPreamble;

  fHandlerUsage =
    handlerUsage;

  fNowEverythingIsAnArgument = false;

  fOahOptionsDefaultValuesStyle = kOAHStyle;

  fMaximumSubGroupsHeadersSize = 1;

  fMaximumShortNameWidth   = 1;
  fMaximumLongNameWidth    = 1;

  fMaximumVariableNameWidth = 0;

  fHandlerFoundAHelpOption = false;

  // initialize the optional values style kinds map
  initializeOahOptionalValuesStyleKindsMap ();

  // initialize the optional values style kind
  fHandlerOptionalValuesStyleKind = kOptionalValuesStyleGNU; // default value
}

oahHandler::~oahHandler ()
{}

void oahHandler::registerHandlerInItself ()
{
  this->
    registerElementInHandler (this);

/* JMI ???
  // register the help summary names in handler
  registerElementNamesInHandler (
    fHandlerSummaryShortName,
    fHandlerSummaryLongName,
    this);
*/

//* JMI
  for (
    list<S_oahGroup>::const_iterator
      i = fHandlerGroupsList.begin ();
    i != fHandlerGroupsList.end ();
    i++
  ) {
    // register the options group
    (*i)->
      registerGroupInHandler (
        this);
  } // for
 // */
}

S_oahPrefix oahHandler::fetchPrefixFromMap (
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

S_oahElement oahHandler::fetchElementFromMap (
  string name) const
{
  S_oahElement result;

  // is name known in options map?
  map<string, S_oahElement>::const_iterator
    it =
      fHandlerElementsMap.find (
        name);

  if (it != fHandlerElementsMap.end ()) {
    // yes, name is known in the map
    result = (*it).second;
  }

  return result;
}

string oahHandler::handlerOptionNamesBetweenParentheses () const
{
  stringstream s;

  s <<
    "(" <<
    fetchNames () <<
    ", ";

  if (
    fHandlerSummaryShortName.size ()
        &&
    fHandlerSummaryLongName.size ()
    ) {
      s <<
        "-" << fHandlerSummaryShortName <<
        ", " <<
        "-" << fHandlerSummaryLongName;
  }

  else {
    if (fHandlerSummaryShortName.size ()) {
      s <<
      "-" << fHandlerSummaryShortName;
    }
    if (fHandlerSummaryLongName.size ()) {
      s <<
      "-" << fHandlerSummaryLongName;
    }
  }

  s <<
    ")";

  return s.str ();
}

void oahHandler::registerElementNamesInHandler (
  S_oahElement element)
{
  string
    elementShortName =
      element->getShortName (),
    elementLongName =
      element->getLongName ();

  int
    elementShortNameSize =
      elementShortName.size (),
    elementLongNameSize =
      elementLongName.size ();

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
      "element long name '" << elementLongName << "'" <<
      " is the same as the short name for the same";

    oahError (s.str ());
  }

  if (elementLongNameSize == 1) {
    stringstream s;

    s <<
      "element long name '" << elementLongName << "'" <<
      " has only one character";

    oahWarning (s.str ());
  }

  for (
    map<string, S_oahElement>::iterator i =
      fHandlerElementsMap.begin ();
    i != fHandlerElementsMap.end ();
    i++
  ) {

    // is elementLongName already in the elements names map?
    if ((*i).first == elementLongName) {
      stringstream s;

      s <<
        "element long name '" << elementLongName << "'" <<
          " for element short name '" << elementShortName << "'" <<
        " is defined more that once";

      oahError (s.str ());
    }

    // is elementShortName already in the elements names map?
    if ((*i).first == elementShortName) {
      if (elementShortName.size ()) {
        stringstream s;

        s <<
          "element short name '" << elementShortName << "'" <<
          " for element long name '" << elementLongName << "'" <<
          " is defined more that once";

        oahError (s.str ());
      }
    }
  } // for

  // register element's names
  if (elementShortNameSize == 1) {
    fSingleCharacterShortNamesSet.insert (elementShortName);
  }

  if (elementLongNameSize) {
    fHandlerElementsMap [elementLongName] =
      element;

    if (elementLongNameSize > fMaximumLongNameWidth) {
      fMaximumLongNameWidth = elementLongNameSize;
    }
  }

  if (elementShortNameSize) {
    fHandlerElementsMap [elementShortName] =
      element;

    if (elementShortNameSize > fMaximumShortNameWidth) {
      fMaximumShortNameWidth = elementShortNameSize;
    }
  }

  // take element's display variable name length into account
  int
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
}

void oahHandler::registerElementInHandler (
  S_oahElement element)
{
  // register the element names in handler
  registerElementNamesInHandler (
    element);

  // insert element into the registered elements list
  fHandlerRegisteredElementsList.push_back (element);

  if (
    // subgroup?
    S_oahSubGroup
      subGroup =
        dynamic_cast<oahSubGroup*>(&(*element))
    ) {

    string
      subHeader=
        subGroup-> getSubGroupHeader ();
    int
      subHeaderSize =
        subHeader.size ();

    // account for subGroup's header size
    if (subHeaderSize > fMaximumSubGroupsHeadersSize) {
      fMaximumSubGroupsHeadersSize =
        subHeaderSize;
    }
  }
}

void oahHandler::checkOptionsConsistency ()
{}

void oahHandler::checkHandlerGroupsOptionsConsistency ()
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "Checking the consistency of OAH handler \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  gIndenter++;

  // check the handler groups options consistency
  if (fHandlerGroupsList.size ()) {
    for (
      list<S_oahGroup>::const_iterator i = fHandlerGroupsList.begin ();
      i != fHandlerGroupsList.end ();
      i++
    ) {
      S_oahGroup group = (*i);

      // check the group
      group->
        checkGroupSubGroupsOptionsConsistency ();
    } // for
  }

  gIndenter--;

  // the the handler's own consistency
  this->checkOptionsConsistency ();
}

void oahHandler::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahHandler::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahHandler>*
    p =
      dynamic_cast<visitor<S_oahHandler>*> (v)) {
        S_oahHandler elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahHandler::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahHandler::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahHandler::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahHandler>*
    p =
      dynamic_cast<visitor<S_oahHandler>*> (v)) {
        S_oahHandler elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gOutputOstream <<
            ".\\\" ==> Launching oahHandler::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahHandler::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gOutputOstream <<
      ".\\\" ==> oahHandler::browseData ()" <<
      endl;
  }
#endif

  // browse the prefixes
  if (fHandlerPrefixesMap.size ()) {
    for (
      map<string, S_oahPrefix>::const_iterator i = fHandlerPrefixesMap.begin ();
      i != fHandlerPrefixesMap.end ();
      i++
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
      i++
    ) {
      S_oahGroup group = (*i);

      // browse the group
      oahBrowser<oahGroup> browser (v);
      browser.browse (*(group));
    } // for
  }
}

void oahHandler::print (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "Handler:" <<
    endl;

  gIndenter++;

  printOptionEssentials (os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fHandlerSummaryShortName" << " : " <<
    fHandlerSummaryShortName <<
    endl <<
    setw (fieldWidth) <<
    "fHandlerSummaryLongName" << " : " <<
    fHandlerSummaryLongName <<
    endl <<

    setw (fieldWidth) <<
    "fShortName" << " : " <<
    fShortName <<
    endl <<
    setw (fieldWidth) <<
    "fLongName" << " : " <<
    fLongName <<
    endl <<

    setw (fieldWidth) <<
    "oahHandlerFoundAHelpOption" << " : " <<
    fHandlerFoundAHelpOption <<
    endl <<

    setw (fieldWidth) <<
    "oahOptionsDefaultValuesStyle" << " : " <<
    optionsDefaultValuesStyleAsString (
      fOahOptionsDefaultValuesStyle) <<
    endl;

  // print the options prefixes if any
  if (fHandlerPrefixesMap.size ()) {
    printKnownPrefixes ();
  }

  // print the single-character options if any
  if (fSingleCharacterShortNamesSet.size ()) {
    printKnownSingleCharacterOptions ();
  }

  // print the options groups if any
  if (fHandlerGroupsList.size ()) {
    os << endl;

    gIndenter++;

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

    gIndenter--;
  }

  gIndenter--;
}

void oahHandler::printHelp (ostream& os)
{
  // print the options handler preamble
  os <<
    gIndenter.indentMultiLineString (
      fHandlerPreamble);

  os << endl;

  // print the options handler usage
  os <<
    "Usage:" <<
    endl;

   gIndenter.indentMultiLineString (
      fHandlerUsage);

  os << endl;

  // print the options handler help header and option names
  os <<
    fHandlerHeader <<
    " " <<
    fetchNamesBetweenParentheses () <<
    ":" <<
    endl;

  // print the options handler description
  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription);
  gIndenter--;

  os <<
    endl <<
    endl;

  // print the known options prefixes
  gIndenter++;
  printKnownPrefixes ();
  gIndenter--;

  // print the single-character options
  gIndenter++;
  printKnownSingleCharacterOptions ();
  gIndenter--;

  // print information about options default values
  gIndenter++;
  printOptionsDefaultValuesInformation ();
  gIndenter--;

  os << endl;

  // print the options groups help
  if (fHandlerGroupsList.size ()) {
    gIndenter++;

    list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahGroup group = (*i);

      // print the options group help
//      group->printHelp (os);

      // print the options subgroups if relevant
      switch (group->getElementVisibilityKind ()) {
        case kElementVisibilityAlways:
          group->printHelp (os);
          break;

        case kElementVisibilityHiddenByDefault:
          group->printGroupHeader (os);
          group->underlineGroupHeader(os);
          break;
      } // switch

      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }

  // register help print action in options handler upLink
//  fHandlerUpLink->setOptionsHandlerFoundAHelpOption ();
}

void oahHandler::printOptionsSummary (ostream& os) const
{
  // print the options handler preamble
  os <<
    gIndenter.indentMultiLineString (
      fHandlerPreamble);

  // print the options handler help header and option names
  os <<
    fHandlerHeader <<
    " " <<
    fetchNamesBetweenParentheses () <<
    ":" <<
    endl;

  // print the options handler description
  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;
  gIndenter--;

  // print the options groups help summaries
  if (fHandlerGroupsList.size ()) {
    gIndenter++;

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

    gIndenter--;
  }
}

void oahHandler::printHandlerAndGroupAndSubGroupSpecificHelp (
  ostream&      os,
  S_oahSubGroup subGroup) const
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os << "oahHandler::printHandlerAndGroupAndSubGroupSpecificHelp" << endl;
  }
#endif

  // print the options handler help header and option names
  os <<
    fHandlerHeader <<
    " " <<
    fetchNamesBetweenParentheses () <<
    ":" <<
    endl;

  // print the optons group subgroups specific help
  if (fHandlerGroupsList.size ()) {
    gIndenter++;

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

    gIndenter--;
  }
}

void oahHandler::printOptionNameIntrospectiveHelp (
  ostream& os,
  string   name) const
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os << "oahHandler::printOptionNameIntrospectiveHelp" << endl;
  }
#endif

  // is name known in options map?
  S_oahElement
    element =
      fetchElementFromMap (name);

  if (! element) {
    // name is not well handled by this options handler
    stringstream s;

    s <<
      "option name '" << name << "' is unknown, cannot deliver specific help";

    oahError (s.str ());
  }

  else {
    // name is known, let's handle it
    if (
      // handler?
      S_oahHandler
        handler =
          dynamic_cast<oahHandler*>(&(*element))
    ) {
      // print the option handler help or help summary
      if (
        name == handler->getHandlerSummaryShortName ()
          ||
        name == handler->getHandlerSummaryLongName ()
     ) {
        handler->
          printOptionsSummary (
            gOutputOstream);
      }
      else {
        handler->
          printHelp (
            gOutputOstream);
      }

      gOutputOstream << endl;
    }

    else if (
      // group?
      S_oahGroup
        group =
          dynamic_cast<oahGroup*>(&(*element))
    ) {
      // print the help
      gOutputOstream <<
        endl <<
        "--- Help for option '" <<
        name <<
        "' at help top level ---" <<
        endl <<
        endl;

      group->
        printHelp (
          gOutputOstream);

      gOutputOstream << endl;
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

      // print the help
      gOutputOstream <<
        endl <<
        "--- Help for option '" <<
        name <<
        "' in group \"" <<
        group->getGroupHeader () <<
        "\" ---" <<
        endl <<
        endl;

      group->
        printGroupAndSubGroupHelp (
          gOutputOstream,
          subGroup);
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

      // print the help
      gOutputOstream <<
        endl <<
        "--- Help for option '" <<
        name <<
        "' in subgroup \"" <<
        subGroup->
          getSubGroupHeader () <<
        "\"" <<
        " of group \"" <<
        group->
          getGroupHeader () <<
        "\" ---" <<
        endl <<
        endl;

      group->
        printGroupAndSubGroupAndAtomHelp (
          gOutputOstream,
          subGroup,
          atom);
    }

    else {
      stringstream s;

      s <<
        "cannot provide specific help about option name \"" <<
        name <<
        "\"";

      oahError (s.str ());
    }
  }
}

void oahHandler::printAllOahCommandLineValues (
  ostream& os) const
{
  // print the options handler values header
  os <<
    fHandlerValuesHeader <<
    " " <<
    fetchNamesBetweenParentheses () <<
    ":" <<
    endl;

  int
    handlerCommandLineElementsMultisetSize =
      fHandlerCommandLineElementsMultiset.size ();

  os <<
    "There are " <<
    fHandlerElementsMap.size () <<
    " known options names for " <<
    fHandlerRegisteredElementsList.size () <<
    " registered elements, " <<
    handlerCommandLineElementsMultisetSize <<
    " of which " <<
    singularOrPluralWithoutNumber (
      handlerCommandLineElementsMultisetSize, "occurs", "occur") <<
    " in the command line" <<
    endl;

  if (fSingleCharacterShortNamesSet.size ()) {
    os <<
      "The single character short option names are: ";

    set<string>::const_iterator
      iBegin = fSingleCharacterShortNamesSet.begin (),
      iEnd   = fSingleCharacterShortNamesSet.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group values
      os << (*i);
      if (++i == iEnd) break;
      os << ",";
    } // for

    os << endl;
  }

  // print the options groups values
  if (fHandlerGroupsList.size ()) {
    os << endl;

    gIndenter++;

    list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group values
      (*i)->
        printGroupOptionsValues (
          os, fMaximumVariableNameWidth);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }
}

ostream& operator<< (ostream& os, const S_oahHandler& elt)
{
  elt->print (os);
  return os;
}

void oahHandler::appendPrefixToHandler (
  S_oahPrefix prefix)
{
  // sanity check
  msrAssert (
    prefix != nullptr,
    "prefix is null");

  string prefixName = prefix->getPrefixName ();

  // is prefixName already known in options map?
  map<string, S_oahPrefix>::const_iterator
    it =
      fHandlerPrefixesMap.find (
        prefixName);

  if (it != fHandlerPrefixesMap.end ()) {
    // prefixName is already known in the map
    stringstream s;

    s <<
      "option prefix name '" << prefixName <<
      "' is already known";

    oahError (s.str ());
  }

  // register prefix in the options prefixes map
  fHandlerPrefixesMap [prefix->getPrefixName ()] =
    prefix;
}

S_oahPrefix oahHandler::fetchPrefixInMapByItsName (
  string prefixName)
{
  S_oahPrefix result;

  // is prefixName already known in options map?
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
  S_oahGroup oahGroup)
{
  // sanity check
  msrAssert (
    oahGroup != nullptr,
    "oahGroup is null");

  // append the options group
  fHandlerGroupsList.push_back (
    oahGroup);

  // set the upLink
  oahGroup->
    setHandlerUpLink (this);
}

void oahHandler::prependGroupToHandler (
  S_oahGroup oahGroup)
{
  // sanity check
  msrAssert (
    oahGroup != nullptr,
    "oahGroup is null");

  // prepend the options group
  fHandlerGroupsList.push_front (
    oahGroup);

  // set the upLink
  oahGroup->
    setHandlerUpLink (this);
}

void oahHandler::printKnownPrefixes () const
{
  int oahHandlerPrefixesListSize =
    fHandlerPrefixesMap.size ();

  gOutputOstream <<
    "There are " <<
    oahHandlerPrefixesListSize <<
    " options prefixes:" <<
    endl;

  // indent a bit more for readability
  gIndenter++;

  if (oahHandlerPrefixesListSize) {
    for (
      map<string, S_oahPrefix>::const_iterator i =
        fHandlerPrefixesMap.begin ();
      i != fHandlerPrefixesMap.end ();
      i++
    ) {
      S_oahPrefix
        prefix = (*i).second;

      prefix->
        printPrefixHeader (
          gOutputOstream);
    } // for

    gOutputOstream << endl;
  }
  else {
    gOutputOstream <<
      "none" <<
      endl;
  }

  gIndenter--;
}

void oahHandler::printKnownSingleCharacterOptions () const
{
  int oahHandlerPrefixesListSize =
    fSingleCharacterShortNamesSet.size ();

  gOutputOstream <<
    "There are " <<
    oahHandlerPrefixesListSize <<
    " single-character options:" <<
    endl;

  // indent a bit more for readability
  gIndenter++;

  if (oahHandlerPrefixesListSize) {
    set<string>::const_iterator
      iBegin = fSingleCharacterShortNamesSet.begin (),
      iEnd   = fSingleCharacterShortNamesSet.end (),
      i      = iBegin;

    int cumulatedLength = 0;

    for ( ; ; ) {
      string theString = (*i);

      gOutputOstream << "-" << theString;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= K_NAMES_LIST_MAX_LENGTH) break;

      if (++i == iEnd) break;

      if (next (i) == iEnd) {
        gOutputOstream << " and ";
      }
      else {
        gOutputOstream << ", ";
      }
    } // for

    gOutputOstream << endl;
  }
  else {
    gOutputOstream <<
      "none" <<
      endl;
  }

  gIndenter--;

  gOutputOstream <<
    "They can be clustered, such as:" <<
    endl <<
    gTab << "'-vac'" <<
    endl <<
    "which is equivalent to:" <<
    endl <<
    gTab << "'-v, -a, -c'" <<
    endl;
}

string oahHandler::optionsDefaultValuesStyleAsString (
  oahOptionsDefaultValuesStyle optionsDefaultValuesStyle)
{
  string result;

  switch (optionsDefaultValuesStyle) {
    case oahHandler::kGNUStyle:
      result = "GNUStyle";
      break;
    case oahHandler::kOAHStyle:
      result = "OAHStyle";
      break;
  } // switch

  return result;
}

void oahHandler::printOptionsDefaultValuesInformation () const
{
  gOutputOstream <<
    endl <<
    "Some options needing a value can use a default value:" <<
    endl;

  gIndenter++;

  gOutputOstream  <<
    fHandlerExecutableName <<
    gIndenter.indentMultiLineString (
R"( supports two styles for this, see '-ovs, -optional-values-style' option.)") <<
    endl;

  gIndenter--;
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

void oahHandler::printKnownOptions () const
{
  int handlerElementsMapSize =
    fHandlerElementsMap.size ();

  // print the options map
  gOutputOstream <<
    "The " <<
    handlerElementsMapSize <<
    " known options for the " <<
    handlerElementsMapSize <<
    " registered elements are:" <<
    endl;

  gIndenter++;

  if (handlerElementsMapSize) {
    map<string, S_oahElement>::const_iterator
      iBegin = fHandlerElementsMap.begin (),
      iEnd   = fHandlerElementsMap.end (),
      i      = iBegin;
    for ( ; ; ) {
      gOutputOstream <<
        (*i).first << "==>" <<
        endl;

      gIndenter++;

      (*i).second->
        printOptionHeader (
          gOutputOstream);

      if (++i == iEnd) break;

      gIndenter--;
    } // for
  }
  else {
    gOutputOstream <<
      "none" <<
      endl;
  }

  gIndenter--;
}

S_oahElement oahHandler::fetchOptionByName (
  string name)
{
  S_oahElement result;

  for (
    list<S_oahGroup>::const_iterator
      i = fHandlerGroupsList.begin ();
    i != fHandlerGroupsList.end ();
    i++
  ) {
    // search name in the options group
    result =
      (*i)->fetchOptionByName (name);

    if (result) {
      break;
    }
  } // for

  return result;
}

void oahHandler::checkMissingPendingValuedAtomValue (
  string atomName,
  string context)
{
  if (fPendingValuedAtom) {
    switch (fHandlerOptionalValuesStyleKind) {
      case kOptionalValuesStyleGNU:
        // handle the valued atom using the default value
        if (fPendingValuedAtom->getValueIsOptional ()) {
          fPendingValuedAtom->
            handleDefaultValue ();
        }

        else {
        /* JMI
          stringstream s;

          s <<
            "option name '" << atomName <<
            "' takes an optional value: it should be used with a '=' in GNU optional values style";

          oahError (s.str ());
       //   oahWarning (s.str ()); // JMI
       */
        }
        break;

      case kOptionalValuesStyleOAH:
        // handle the valued atom using the default value
        if (fPendingValuedAtom->getValueIsOptional ()) {
          fPendingValuedAtom->
            handleDefaultValue ();
        }

        else {
          // an option requiring a value is expecting it,
          // but another option, an argument or the end of the command line
          // has been found instead
          stringstream s;

          s <<
            "option " <<
           fPendingValuedAtom->asString () <<
           " expects a value" <<
           " (" << context << ")";

          oahError (s.str ());
        }
        break;
    } // switch

    fPendingValuedAtom = nullptr;
  }
}

void oahHandler::handlePrefixName (
  string prefixName,
  size_t equalsSignPosition,
  string stringAfterEqualsSign)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "===> equalsSignPosition = '" << equalsSignPosition <<
      "', " <<
      "===> prefixName = '" << prefixName <<
      "', " <<
      "===> stringAfterEqualsSign = '" << stringAfterEqualsSign <<
      "'" <<
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

  unsigned chunksListSize = chunksList.size ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "There are " << chunksListSize << " chunks" <<
      " in '" << stringAfterEqualsSign <<
      "'" <<
      endl;

    gIndenter++;

    list<string>::const_iterator
      iBegin = chunksList.begin (),
      iEnd   = chunksList.end (),
      i      = iBegin;

    for ( ; ; ) {
      gOutputOstream <<
        "[" << (*i) << "]";
      if (++i == iEnd) break;
      gOutputOstream <<
        " ";
    } // for

    gOutputOstream << endl;

    gIndenter--;
  }
#endif

  S_oahPrefix
    prefix =
      fetchPrefixFromMap (prefixName);

  if (prefix) {
    if (chunksListSize) {
      // expand the option names contained in chunksList
      for (
        list<string>::const_iterator i =
          chunksList.begin ();
        i != chunksList.end ();
        i++
      ) {
        string singleOptionName = (*i);

        // build uncontracted option name
        string
          uncontractedOptionName =
            prefix->getPrefixErsatz () + singleOptionName;

#ifdef TRACE_OAH
        if (gTraceOah->fTraceOah) {
          gOutputOstream <<
            "Expanding option '" << singleOptionName <<
            "' to '" << uncontractedOptionName <<
            "'" <<
            endl;
        }
#endif

        // handle the uncontracted option name
        handleOptionName (uncontractedOptionName);
      } // for
    }
  }

  else {
    printKnownPrefixes ();

    stringstream s;

    s <<
      "option prefix '" << prefixName <<
      "' is unknown, see help summary below";

    oahError (s.str ());
  }
}

bool oahHandler::optionNameIsASingleCharacterOptionsCluster (
  string optionName)
{
  bool result = true; // until the contrary is known

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "Checking whether optionName '" << optionName << "' is a single-character options cluster" <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    optionName.size () != 0,
    "optionName.size () == 0");

  list<S_oahElement> cluserElementsList;

  // collect single-character elements in cluserElementsList
  for (
    string::const_iterator i = optionName.begin ();
    i != optionName.end ();
    i++
  ) {
    string singleCharacterString (1, (*i));

#ifdef TRACE_OAH
    if (gTraceOah->fTraceOah) {
      gOutputOstream <<
        "Considering single-character '" << singleCharacterString << "'" <<
        endl;
    }
#endif

    set<string>::const_iterator
      it =
        fSingleCharacterShortNamesSet.find (
          singleCharacterString);

    if (it != fSingleCharacterShortNamesSet.end ()) {
      // yes, singleCharacterString is known in the set
      cluserElementsList.push_back (
        fetchOptionByName (
          singleCharacterString));
    }
    else {
      // no, singleCharacterString is not known in the set,
      // optionName is not a cluster
      result = false; // necessary if others are actually atoms short names JMI
    }
  } // for

  if (cluserElementsList.size () == fSingleCharacterShortNamesSet.size ()) {
    // all the elements in cluserElementsList are single character options,
    // handle them
    for (
      list<S_oahElement>::const_iterator i = cluserElementsList.begin ();
      i != cluserElementsList.end ();
      i++
    ) {
      S_oahElement element = (*i);

      // handle element name
#ifdef TRACE_OAH
    if (true || gTraceOah->fTraceOah) {
      gOutputOstream <<
        "handling single-character options cluster element " <<
        element->asString () <<
        endl;
    }
#endif

      handleOptionName (
        element->getShortName ());
    } // for
  }

  else {
#ifdef TRACE_OAH
    if (gTraceOah->fTraceOah) {
      gOutputOstream <<
        "optionName '" << optionName << "' is no single-character options cluster" <<
        endl;
    }
#endif
    result = false;
  }

  return result;
}

void oahHandler::decipherOptionContainingEqualSign (
  string currentOptionName,
  size_t equalsSignPosition)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "deciphering option containing equal sign" <<
      ", currentOptionName: '" << currentOptionName << "'" <<
      ", equalsSignPosition:  '" << equalsSignPosition << "'" <<
      endl;
  }
#endif

  // fetch the option name and the string after the equals sign
  string name =
    currentOptionName.substr (0, equalsSignPosition);
  string stringAfterEqualsSign =
    currentOptionName.substr (equalsSignPosition + 1);

  // prefixes have precedence over options with optional values
  S_oahPrefix
    prefix =
      fetchPrefixFromMap (name);

  if (prefix) {
    // handle prefix name
#ifdef TRACE_OAH
    if (gTraceOah->fTraceOah) {
      printKnownPrefixes ();
    }
#endif

    handlePrefixName (
      name,
      equalsSignPosition,
      stringAfterEqualsSign);
  }

  else {
    // name is not the name of prefix

    // is it the name of an option?
    S_oahElement
      element =
        fetchOptionByName (name);

    if (element) {
      // name is the name of an option
      if (
        // oahStringWithDefaultValueAtom?
        S_oahStringWithDefaultValueAtom
          stringWithDefaultValueAtom =
            dynamic_cast<oahStringWithDefaultValueAtom*>(&(*element))
      ) {
#ifdef TRACE_OAH
        if (gTraceOah->fTraceOah) {
          gOutputOstream <<
            "==> option '" <<
            name <<
            "' is a stringWithDefaultValueAtom" <<
            endl;
        }
#endif

        // handle the stringWithDefaultValueAtom
        switch (fHandlerOptionalValuesStyleKind) {
          case kOptionalValuesStyleGNU: // default value
            stringWithDefaultValueAtom->
              handleValue (
                stringAfterEqualsSign,
                gOutputOstream);
            break;
          case kOptionalValuesStyleOAH:
            {
              stringstream s;

              s <<
                "option name '" << name <<
                "' cannot be used with a '=' in OAH optional values style";

              oahError (s.str ());
            }
            break;
        } // switch
      }
      else {
        // name is not the name an a stringWithDefaultValueAtom
        stringstream s;

        s <<
          "option name '" << name <<
          "' doesn't have a default value and thus cannot be used with a '='";

        oahError (s.str ());
      }
    }

    else {
      // name is not the name of an option
      stringstream s;

      s <<
        "option name '" << name << "' is not the name of an option";

      oahError (s.str ());
    }
  }
}

string oahHandler::decipherOption (
  string currentString)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "deciphering currentString '" << currentString << "'" <<
      endl;
  }
#endif

  string currentOptionName;

  string optionTrailer =
    currentString.substr (1, string::npos);

  /* JMI
  gOutputOstream <<
    "optionTrailer '" << optionTrailer << "' is preceded by a dash" <<
    endl;
  */

  // here, optionTrailer.size () >= 1

  if (optionTrailer [0] == '-') {
    // this is a double-dashed option, '--' has been found

    if (optionTrailer.size () == 1) {
      // optionTrailer is '--' alone, that marks the end of the options
      fNowEverythingIsAnArgument = true;

      return "";
    }

    else {
      // optionTrailer is a double-dashed option
      currentOptionName =
        optionTrailer.substr (1, string::npos);

#ifdef TRACE_OAH
      if (gTraceOah->fTraceOah) {
        gOutputOstream <<
          "'" << currentOptionName << "' is a double-dashed option" <<
          endl;
      }
#endif
    }
  }

  else {
    // it is a single-dashed option
    currentOptionName =
      optionTrailer;

#ifdef TRACE_OAH
    if (gTraceOah->fTraceOah) {
      gOutputOstream <<
        "'" << currentOptionName << "' is a single-dashed option" <<
        endl;
    }
#endif
  }

  // does currentOptionName contain an equal sign?
  size_t equalsSignPosition =
    currentOptionName.find ("=");

  if (equalsSignPosition != string::npos) {
    // yes, there's an equal sign
    decipherOptionContainingEqualSign (
      currentOptionName,
      equalsSignPosition);
  }

  else {
    // no, there's no equal sign
    // handle the current option name
    handleOptionName (currentOptionName);
  }

  return currentOptionName;
}

const vector<string> oahHandler::decipherOptionsAndArguments (
  int   argc,
  char* argv[])
{
  // fetch program name
  fHandlerExecutableName = string (argv [0]);

  fCommandLineAsSupplied = fHandlerExecutableName;

  /* JMI ???
  // print the number of option names
  int handlerElementsMapSize =
    fHandlerElementsMap.size ();

  gOutputOstream <<
    fHandlerExecutableName <<
    " features " <<
    handlerElementsMapSize <<
    " options names for " <<
    fHandlerRegisteredElementsMultiset.size () <<
    " registered elements" <<
    endl;
*/

  // decipher the command options and arguments
  int n = 1;

  string lastOptionNameFound;

  while (true) {
    if (argv [n] == 0)
      break;

    string currentString = string (argv [n]);

    fCommandLineAsSupplied +=
      " " + quoteStringIfNonAlpha (currentString);


#ifdef TRACE_OAH
    if (gTraceOah->fTraceOah) {
      // print current option
      gOutputOstream <<
        "Command line option " << n <<
        ": " << currentString << " "<<
        endl;
    }
#endif

    // handle current option
    if (currentString [0] == '-') {
      // stdin or option?

      if (currentString.size () == 1) {
        // this is the stdin indicator
#ifdef TRACE_OAH
          if (gTraceOah->fTraceOah) {
          gOutputOstream <<
            "'" << currentString <<
              "' is the '-' stdin indicator" <<
            endl;
        }
#endif

        fHandlerArgumentsVector.push_back (currentString);
      }

      else {
        // this is an option, first '-' has been found
        // and currentString.size () >= 2

        lastOptionNameFound =
          decipherOption (
            currentString);
      }
    }

    else {
      // currentString is no oahElement:
      // it is an atom value or an argument
      handleOptionValueOrArgument (currentString);
    }

    // next please
    n++;
  } // while

  // is a pending valued atom value missing?
  checkMissingPendingValuedAtomValue (
    lastOptionNameFound,
    "last option in command line");

  unsigned int argumentsVectorSize =
    fHandlerArgumentsVector.size ();

#ifdef TRACE_OAH
  // display arc and argv only now, to wait for the options to have been handled
  if (
    gTraceOah->fTraceOah
      ||
    gOahOah->fShowOptionsAndArguments
  ) {
    gOutputOstream <<
      "argc: " << argc <<
      endl <<
      "argv:" <<
      endl;

    gIndenter++;

    for (int i = 0; i < argc; i++) {
      gOutputOstream <<
        "argv [" << i << "]: " << argv [i] <<
        endl;
    } // for

    gIndenter--;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOahDetails) {
    printKnownPrefixes ();
    printKnownSingleCharacterOptions ();
    printKnownOptions ();
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    // print the arguments vector
    gOutputOstream <<
      "Arguments vector (" <<
      argumentsVectorSize <<
      " elements):" <<
      endl;

    if (argumentsVectorSize) {
      gIndenter++;
      for (unsigned int i = 0; i < argumentsVectorSize; i++) {
        gOutputOstream <<
          fHandlerArgumentsVector [i] <<
          endl;
      } // for
      gIndenter--;
    }
  }
#endif

  // print the chosen options if so chosen
  // ------------------------------------------------------

#ifdef TRACE_OAH
  if (gOahOah->fDisplayOahValues) {
    printAllOahCommandLineValues (
      gOutputOstream);

    gOutputOstream << endl;
  }
#endif

  // was this run a 'pure help' one?
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "The value of fHandlerFoundAHelpOption is: " <<
      booleanAsString (fHandlerFoundAHelpOption) <<
      endl;
  }
#endif

  if (fHandlerFoundAHelpOption) {
#ifdef TRACE_OAH
    if (gTraceOah->fTraceOah) {
      gOutputOstream <<
        "==> fHandlerFoundAHelpOption is true, exiting" <<
        endl;
    }
#endif

    exit (0);
  }

  // check the consistency of the options
  checkHandlerGroupsOptionsConsistency ();

  // check the options and arguments
  checkOptionsAndArguments ();

  // store the command line with options in gOahOah
  // for whoever need them
  gOahOah->fCommandLineAsSupplied =
      commandLineAsSuppliedAsString ();
  gOahOah->fCommandLineWithShortOptionsNames =
      commandLineWithShortNamesAsString ();
  gOahOah->fCommandLineWithLongOptionsNames =
      commandLineWithLongNamesAsString ();

  // return arguments vector for handling by caller
  return fHandlerArgumentsVector;
}

void oahHandler::handleHandlerName (
  S_oahHandler handler,
  string       name)
{
  // print the handler help or help summary
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << name << "' is of type 'oahHandler'" <<
      endl;
  }
#endif

  if (
    name == handler->getHandlerSummaryShortName ()
      ||
    name == handler->getHandlerSummaryLongName ()
  ) {
    handler->
      printOptionsSummary (
        gOutputOstream);
  }
  else {
    handler->
      printHelp (
        gOutputOstream);
  }

  gOutputOstream << endl;
}

void oahHandler::handleGroupName (
  S_oahGroup group,
  string     groupName)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << groupName << "' is of type 'oahGroup'" <<
      endl;
  }
#endif

  // print the help
  gOutputOstream <<
    endl <<
    "--- Help for group \"" <<
    group->
      getGroupHeader () <<
    "\" ---" <<
    endl <<
    endl;

  group->
    printHelp (
      gOutputOstream);
}

void oahHandler::handleSubGroupName (
  S_oahSubGroup subGroup,
  string        subGroupName)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << subGroupName << "' is of type 'subGroup'" <<
      endl;
  }
#endif

  // get the options group upLink
  S_oahGroup
    group =
      subGroup->
        getGroupUpLink ();

  // print the help
  gOutputOstream <<
    endl <<
    "--- Help for subgroup \"" <<
    subGroup->
      getSubGroupHeader () <<
    "\"" <<
    " in group \"" <<
    group->
      getGroupHeader () <<
    "\" ---" <<
    endl <<
    endl;

  group->
    printGroupAndSubGroupHelp (
      gOutputOstream,
      subGroup);
}

void oahHandler::handleAtomName (
  S_oahAtom atom,
  string    atomName)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> oahHandler: handling atomName '" << atomName <<
      "', atom: "<<
      atom->asString () <<
      "'" <<
      endl;
  }
#endif

  if (
    // atom synonym?
    S_oahAtomSynonym
      atomSynonym =
        dynamic_cast<oahAtomSynonym*>(&(*atom))
  ) {
    // yes, use the original atom instead

    S_oahAtom
      originalOahAtom =
        atomSynonym->getOriginalOahAtom ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << atomName << "' is a synonym for '" <<
      originalOahAtom->asString () <<
      "'" <<
      endl;
  }
#endif

    this->handleAtomName (
      originalOahAtom,
      atomSynonym->getShortName ());
  }

  else {
    // atom is a plain atom

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> option '" << atomName << "' is a plain atom '" <<
      "'" <<
      endl;
  }
#endif

    // is the value for a pending valued atom missing?
    string context =
      "before option " + atom->asString ();

    checkMissingPendingValuedAtomValue (
      atomName,
      context);

    // delegate the handling to the atom
    fPendingValuedAtom =
      atom->
        handleOptionUnderName (
          atomName,
          gOutputOstream);
  }
}

void oahHandler::handleOptionName (
  string name)
{
#ifdef TRACE_OAH
    if (gTraceOah->fTraceOah) {
      gOutputOstream <<
        endl <<
        "==> oahHandler::handleOptionName(), handling name \"" <<
        name <<
        "\"" <<
        endl;
    }
#endif

  // is name known in options map?
  S_oahElement
    element =
      fetchElementFromMap (name);

  if (! element) {
    // name is unknown
#ifdef TRACE_OAH
    if (gTraceOah->fTraceOah) {
      gOutputOstream <<
        endl <<
        "==> oahHandler::handleOptionName (), name = \"" <<
        name <<
        "\" is not known" <<
        endl;
    }
#endif

    if (
      // is it a single-character options cluster?
      name.size () > 1
        &&
      optionNameIsASingleCharacterOptionsCluster (name)
    ) {
      // the options contained in name have just been handled,
      // do nothing more
    }

    else {
      // name is unknown to this OAH handler

      stringstream s;

      s <<
        "option name '" << name << "' is unknown";

      oahError (s.str ());
    }
  }

  else {
    // name is a known element, let's handle it
#ifdef TRACE_OAH
    if (gTraceOah->fTraceOah) {
      gOutputOstream <<
        endl <<
        "==> oahHandler::handleOptionName (), name = \"" <<
        name <<
        "\" is described by option:" <<
        endl;
      gIndenter++;
      element->print (gOutputOstream);
      gIndenter--;
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
          "element '" <<
          element->fetchNames () <<
          "' is already present in the command line";

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
      shortNameToBeUsed = shortName,
      longNameToBeUsed = longName;

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
      // options handler?
      S_oahHandler
        handler =
          dynamic_cast<oahHandler*>(&(*element))
    ) {
        handleHandlerName (
          handler,
          name);
    }

    else if (
      // options group?
      S_oahGroup
        group =
          dynamic_cast<oahGroup*>(&(*element))
    ) {
      handleGroupName (
        group,
        name);
    }

    else if (
      // options subgroup?
      S_oahSubGroup
        subGroup =
          dynamic_cast<oahSubGroup*>(&(*element))
    ) {
      handleSubGroupName (
        subGroup,
        name);
    }

    else if (
      // atom?
      S_oahAtom
        atom =
          dynamic_cast<oahAtom*>(&(*element))
    ) {
      handleAtomName (
        atom,
        name);
    }

    else {
      stringstream s;

      s <<
        "INTERNAL ERROR: option name '" << name << "' cannot be handled";

      oahError (s.str ());
    }
  }
}

void oahHandler::handleOptionValueOrArgument (
  string theString)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gOutputOstream <<
      "==> handleOptionValueOrArgument ()" <<
      endl;

    gIndenter++;

    gOutputOstream <<
      "fPendingValuedAtom:" <<
      endl;

    gIndenter++;
    if (fPendingValuedAtom) {
      gOutputOstream <<
        fPendingValuedAtom;
    }
    else {
      gOutputOstream <<
        "null" <<
        endl;
    }
    gIndenter--;

    gOutputOstream <<
      "theString:" <<
      endl;

    gIndenter++;
    gOutputOstream <<
      " \"" <<
      theString <<
      "\"" <<
      endl;
    gIndenter--;

    gIndenter--;
  }
#endif

  // options are handled at once, unless they are valued,
  // in which case the handling of the option and its value
  // are postponed until the latter is available
  if (fPendingValuedAtom) {
    // theString is the value for the pending valued atom
    fPendingValuedAtom->handleValue (
      theString,
      gOutputOstream);

    fPendingValuedAtom = nullptr;
  }

  else {
    // theString is an argument

#ifdef TRACE_OAH
      if (gTraceOah->fTraceOah) {
        gOutputOstream <<
          "'" << theString << "'" <<
          " is an argument, not an option" <<
          endl;
      }
#endif

    fHandlerArgumentsVector.push_back (theString);
  }
}


}
