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

#include "messagesHandling.h"

#include "generalOptions.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif


namespace MusicXML2
{

/*
Basics:
  - oah (Options And Help) is supposed to be pronouced something close to "whaaaah!"
    The intonation is left to the speaker, though...

  - options handling is organized as a hierarchical, instrospective set of classes.
    Options and their corresponding help are grouped in a single object.

  - oahElement is the super-class of all options elements.
    It contains a short name and a long name, as well as a decription.
    Short and long names can be used and mixed at will in the command line,
    as well as '-' and '--'.
    The short name is mandatory, but the long name may be empty.

  - prefixes such '-t=' and -help=' allow for a contracted form of options.
    For example, -t=meas,notes is short for '-t-meas, -tnotes'.
    An oahPrefix contains the prefix name, the ersatz by which to replace it,
    and a description.

  - an oahHandler contains oahGroup's, each handled in a pair or .h/.cpp files,
    such as msrOptions.h and msrOptions.cpp, and a list of options prefixes.

  - an oahGroup contains oahSubGroup's and an upLink to the containing oahHandler.

  - an oahSubGroup contains oahAtom's and an upLink to the containing oahGroup.

  - each oahAtom is an atomic option to the executable and its corresponding help,
    and an upLink to the containing oahSubGroup.

Features:
  - partial help to be obtained, i.e. help about any group, subgroup or item,
    showing the path in the hierarchy down to the corresponding element.

  - there are various subclasses of oahAtom such as oahIntegerAtom, oahBooleanAtom
    and oahRationalAtom to control options values of common types.

  - oahThreeBooleansAtom, for example, allows for three boolean settings
    to be controlled at once with a single option.

  - oahValuedAtom describes options for which a value is supplied in the command line.

  - a class such as optionsLpsrPitchesLanguageItem is used
    to supply a string value to be converted into an internal enumerated type.

  - oahCombinedBooleansAtom contains a list of items to manipulate several items as a single one,
    see the 'cubase' combined item in musicXMLOptions.cpp.

  - storing options and the corresponding help in oahGroup's makes it easy to re-use them.
    For example, xml2ly and xml2lbr have their three first passes in common,
    (up to obtaining the MSR description of the score), as well as the corresponding options and help.

Handling:
  - each optionElement must have unique short and long names, for consistency.

  - an executable main() call decipherOptionsAndArguments(), in which:
    - handleElementName() handles the item names
    - handleOptionsItemValueOrArgument() handles the values that may follow an item name
      and the arguments to the executable.

  - contracted forms are expanded in handleElementName() before the resulting,
    uncontracted options are handled.

  - handleElementName() fetches the oahElement corresponding to the name from the map,
    determines the type of the latter,
    and delegates the handling to the corresponding object.

  - handleOptionsItemValueOrArgument() associatiates the value
    to the (preceding) fPendingValuedAtom if not null,
    or appends it fArgumentsVector to otherwise.

  - the printOptionsSummary() methods are used when there are errors in the options used.

  - the printHelp() methods perform the actual help print work

  - options deciphering it done by the handleOptionsItem() methods defined:
      - in oahBasicTypes.h/.cpp for the predefined ones;
      - in the various options groups for those specific to the latter.
    The value following the option name, if any, is taken care of
    by the handleOptionsItemValue() methods.
*/

//______________________________________________________________________________
S_oahElement oahElement::create (
  string shortName,
  string longName,
  string description)
{
  oahElement* o = new
    oahElement (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

oahElement::oahElement (
  string shortName,
  string longName,
  string description)
{
  fShortName   = shortName;
  fLongName    = longName;
  fDescription = description;

  fIsHidden    = false;
}

oahElement::~oahElement ()
{}

S_oahElement oahElement::fetchElementByName (
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

string oahElement::asString () const
{
  stringstream s;

  s <<
    "'"
    "-" << fShortName <<
    "," <<
    "-" << fLongName <<
// JMI    ": " << fDescription <<
    "'";

  return s.str ();
}

void oahElement::printHeader (ostream& os) const
{
  os <<
    "-" << fShortName <<
    endl <<
    "-" << fLongName <<
    endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;

    gIndenter.decrement (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);
  }
}

void oahElement::printElementEssentials (
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
    endl;
}

void oahElement::print (ostream& os) const
{
  os <<
    "??? oahElement ???" <<
    endl;

  printElementEssentials (os, 40); // JMI
}

void oahElement::printHelp (ostream& os) const
{
  os <<
    fetchNames () <<
    endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;

    gIndenter.decrement (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);
  }

  // register help print action in options handler upLink JMI ???
//  fHandlerUpLink->setOptionsHandlerFoundAHelpItem ();
}

ostream& operator<< (ostream& os, const S_oahElement& elt)
{
  elt->print (os);
  return os;
}

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
      description)
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

void oahAtom::registerOptionsItemInHandler (
  S_oahHandler oahHandler)
{
  oahHandler->
    registerElementInHandler (this);
}

void oahAtom::print (ostream& os) const
{
  const int fieldWidth = 19;

  os <<
    "OptionsItem ???:" <<
      endl;

  gIndenter++;

  oahElement::printElementEssentials (
    os, fieldWidth);

  gIndenter--;
}

void oahAtom::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os <<
    "OptionsItem values ???:" <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahAtom& elt)
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

void oahOptionsUsageAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsHelpUsageItem:" <<
    endl;

  gIndenter++;

  oahElement::printElementEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

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

The options are organized in a group-subgroup-item hierarchy.
Help can be obtained for groups or subgroups at will,
as well as for any option with the '-ih, itemHelp' option.

A subgroup can be hidden by default, in which case its description is printed
only when the corresponding item short or long names are used.

Both '-' and '--' can be used to introduce options in the command line,
even though the help facility only shows them with '-'.

Command line options and arguments can be placed in any order,
provided item values immediately follow the corresponding items.)") <<
    endl <<
    endl;

  gIndenter--;
}

void oahOptionsUsageAtom::printOptionsValues (
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

void oahOptionsSummaryAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsHelpSummaryItem:" <<
    endl;

  gIndenter++;

  oahElement::printElementEssentials (
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
    gGeneralOptions->fExecutableName <<
    endl;
}

void oahOptionsSummaryAtom::printOptionsValues (
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

void oahAtomWithVariableName::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsItemWithVariableName:" <<
    endl;

  gIndenter++;

  oahElement::printElementEssentials (
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

void oahAtomWithVariableName::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    "FOO" <<
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

void oahBooleanAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsBooleanItem:" <<
    endl;

  gIndenter++;

  oahElement::printElementEssentials (
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
    booleanAsString (
      fBooleanVariable) <<
    endl;

  gIndenter--;
}

void oahBooleanAtom::printOptionsValues (
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
  : oahAtomWithVariableName (
      shortName,
      longName,
      description,
      variableName),
    fBooleanVariable (
      booleanVariable),
    fBooleanSecondaryVariable (
      booleanSecondaryVariable)
{}

oahTwoBooleansAtom::~oahTwoBooleansAtom ()
{}

void oahTwoBooleansAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsTwoBooleansItem:" <<
    endl;

  gIndenter++;

  oahElement::printElementEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription);
  gIndenter--;

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "fBooleanVariable" << " : " <<
    booleanAsString (
      fBooleanVariable) <<
    endl;

  gIndenter--;
}

void oahTwoBooleansAtom::printOptionsValues (
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
  : oahAtomWithVariableName (
      shortName,
      longName,
      description,
      variableName),
    fBooleanVariable (
      booleanVariable),
    fBooleanSecondaryVariable (
      booleanSecondaryVariable),
    fBooleanTertiaryVariable (
      booleanTertiaryVariable)
{}

oahThreeBooleansAtom::~oahThreeBooleansAtom ()
{}

void oahThreeBooleansAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsThreeBooleansItem:" <<
    endl;

  gIndenter++;

  oahElement::printElementEssentials (
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
    booleanAsString (
      fBooleanVariable) <<
    endl;

  gIndenter--;
}

void oahThreeBooleansAtom::printOptionsValues (
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
          getOptionsHandlerUpLink ();

  // sanity check
  msrAssert (
    handler != nullptr,
    "handler is null");

  // is name known in options elements map?
  S_oahElement
    element =
      handler->
        fetchElementFromMap (
          name);

  if (! element) {
    // no, name is unknown in the map
    handler->
      printOptionsSummary ();

    stringstream s;

    s <<
      "INTERNAL ERROR: option name '" << name <<
      "' is unknown";

    optionError (s.str ());
  }

  else {
    // name is known, let's handle it

    if (
      // boolean atom?
      S_oahBooleanAtom
        item =
          dynamic_cast<oahBooleanAtom*>(&(*element))
      ) {
      // handle the option item
      fBooleanAtomsList.push_back (
        item);
    }

    else {
      stringstream s;

      s <<
        "option name '" << name <<
        "' is not that of an item";

      optionError (s.str ());

      exit (2);
    }
  }
}

void oahCombinedBooleansAtom::setBooleanVariablesValues (
  bool value)
{
  // set the combined items item variable
  fBooleanVariable = value;

  // set the value of the items in the list
  if (fBooleanAtomsList.size ()) {
    for (
      list<S_oahBooleanAtom>::const_iterator i =
        fBooleanAtomsList.begin ();
      i != fBooleanAtomsList.end ();
      i++
    ) {
      S_oahAtom
        item = (*i);

      if (
        // boolean item?
        S_oahBooleanAtom
          booleanItem =
            dynamic_cast<oahBooleanAtom*>(&(*item))
        ) {
        // set the boolean value
        booleanItem->
          setBooleanItemVariableValue (value);
      }
    } // for
  }
}

void oahCombinedBooleansAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsCombinedBooleanItemsItem:" <<
    endl;

  gIndenter++;

  printElementEssentials (
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

void oahCombinedBooleansAtom::printHelp (ostream& os) const
{
  os <<
    fetchNames () <<
    endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);

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
    gIndenter.decrement (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);
  }

  // register help print action in options handler upLink
//  fHandlerUpLink->setOptionsHandlerFoundAHelpItem ();
}

void oahCombinedBooleansAtom::printOptionsValues (
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
        item = (*i);

      if (
        // boolean item?
        S_oahBooleanAtom
          booleanItem =
            dynamic_cast<oahBooleanAtom*>(&(*item))
        ) {
        // print the boolean value
        booleanItem->
          printOptionsValues (
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

string oahValuedAtom::oahAtomKindAsString (
  oahValuedAtom::oahValuedAtomKind oahAtomKind)
{
  string result;

  switch (oahAtomKind) {
    case oahValuedAtom::kOptionsItemHasNoArgument:
      result = "OptionsItemHasNoArgument";
      break;
    case oahValuedAtom::kOptionsItemHasARequiredArgument:
      result = "OptionsItemHasARequiredArgument";
      break;
    case oahValuedAtom::kOptionsItemHasAnOptionsArgument:
      result = "OptionsItemHasAnOptionsArgument";
      break;
  } // switch

  return result;
}

void oahValuedAtom::printValuedAtomEssentials (
  ostream& os,
  int      fieldWidth) const
{
  printElementEssentials (
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
    "OptionsValuedAtom ???:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  gIndenter--;
}

void oahValuedAtom::printHelp (ostream& os) const
{
  os <<
    fetchNames () <<
    " " <<
    fValueSpecification <<
    endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;

    gIndenter.decrement (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);
  }

/* superfluous JMI
  if (fValueIsOptional) {
    os <<
      "option value is optionnal" <<
      endl;
  }
*/

  // register help print action in options handler upLink // JMI
//  fHandlerUpLink->setOptionsHandlerFoundAHelpItem ();
}

void oahValuedAtom::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os <<
    "OptionsItem values ???:" <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahValuedAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahElementHelpAtom oahElementHelpAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName)
{
  oahElementHelpAtom* o = new
    oahElementHelpAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName);
  assert(o!=0);
  return o;
}

oahElementHelpAtom::oahElementHelpAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName)
{}

oahElementHelpAtom::~oahElementHelpAtom ()
{}

void oahElementHelpAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsItemHelpItem:" <<
    endl;

  gIndenter++;

  printElementEssentials (
    os, fieldWidth);

  gIndenter--;
}

void oahElementHelpAtom::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_oahElementHelpAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahIntegerAtom oahIntegerAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  int&               integerVariable)
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
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  int&               integerVariable)
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

void oahIntegerAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsIntegerItem:" <<
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

void oahIntegerAtom::printOptionsValues (
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
S_oahFloatAtom oahFloatAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  float&             floatVariable)
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
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  float&             floatVariable)
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

void oahFloatAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsFloatItem:" <<
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

void oahFloatAtom::printOptionsValues (
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
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  string&            stringVariable)
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
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  string&            stringVariable)
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

void oahStringAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsStringItem:" <<
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

void oahStringAtom::printOptionsValues (
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
    "OptionsStringItem:" <<
    endl;
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahRationalAtom oahRationalAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  rational&          rationalVariable)
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
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  rational&          rationalVariable)
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

void oahRationalAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsRationalItem:" <<
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

void oahRationalAtom::printOptionsValues (
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
S_oahNumbersSetAtom oahNumbersSetAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  set<int>&          numbersSetVariable)
{
  oahNumbersSetAtom* o = new
    oahNumbersSetAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      numbersSetVariable);
  assert(o!=0);
  return o;
}

oahNumbersSetAtom::oahNumbersSetAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  set<int>&          numbersSetVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fNumbersSetVariable (
      numbersSetVariable)
{}

oahNumbersSetAtom::~oahNumbersSetAtom ()
{}

void oahNumbersSetAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsNumbersSetItem:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fNumbersSetVariable" << " : " <<
    endl;

  if (! fNumbersSetVariable.size ()) {
    os <<
      "none";
  }

  else {
    os <<
      "'";

    set<int>::const_iterator
      iBegin = fNumbersSetVariable.begin (),
      iEnd   = fNumbersSetVariable.end (),
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

void oahNumbersSetAtom::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fNumbersSetVariable.size ()) {
    os <<
      "none";
  }

  else {
    os <<
      "'";

    set<int>::const_iterator
      iBegin = fNumbersSetVariable.begin (),
      iEnd   = fNumbersSetVariable.end (),
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

ostream& operator<< (ostream& os, const S_oahNumbersSetAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahSubGroup oahSubGroup::create (
  string                    header,
  string                    shortName,
  string                    longName,
  string                    description,
  oahSubGroupVisibilityKind subGroupVisibilityKind,
  S_oahGroup                groupUpLink)
{
  oahSubGroup* o = new
    oahSubGroup (
      header,
      shortName,
      longName,
      description,
      subGroupVisibilityKind,
      groupUpLink);
  assert(o!=0);
  return o;
}

oahSubGroup::oahSubGroup (
  string                    header,
  string                    shortName,
  string                    longName,
  string                    description,
  oahSubGroupVisibilityKind subGroupVisibilityKind,
  S_oahGroup                groupUpLink)
  : oahElement (
      shortName,
      longName,
      description)
{
  fGroupUpLink = groupUpLink;

  fHeader = header;

  fSubGroupVisibilityKind = subGroupVisibilityKind;
}

oahSubGroup::~oahSubGroup ()
{}

string oahSubGroup::subGroupVisibilityKindAsString (
  oahSubGroupVisibilityKind subGroupVisibilityKind)
{
  string result;

  switch (subGroupVisibilityKind) {
    case kSubGroupVisibilityAlways:
      result = "subGroupVisibilityAlways";
      break;

    case kSubGroupVisibilityHiddenByDefault:
      result = "subGroupVisibilityHiddenByDefault";
      break;
  } // switch

  return result;
}

void oahSubGroup::underlineHeader (ostream& os) const
{
  /* JMI ???
  for (unsigned int i = 0; i < fHeader.size (); i++) {
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

  for (
    list<S_oahAtom>::const_iterator
      i = fSubGroupItemsList.begin ();
    i != fSubGroupItemsList.end ();
    i++
  ) {
    // register the options sub group
    (*i)->
      registerOptionsItemInHandler (
        handler);
  } // for
}

void oahSubGroup::appendAtom (
  S_oahAtom oahAtom)
{
  // sanity check
  msrAssert (
    oahAtom != nullptr,
    "oahAtom is null");

  // append options item
  fSubGroupItemsList.push_back (
    oahAtom);

  // set options item subgroup upLink
  oahAtom->
    setSubGroupUpLink (this);
}

S_oahElement oahSubGroup::fetchElementByName (
  string name)
{
  S_oahElement result;

  for (
    list<S_oahAtom>::const_iterator
      i = fSubGroupItemsList.begin ();
    i != fSubGroupItemsList.end ();
    i++
  ) {
    // search name in the options group
    result =
      (*i)->fetchElementByName (name);

    if (result != 0) {
      break;
    }
  } // for

  return result;
}

void oahSubGroup::print (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
   "SubGroup:" <<
    endl;

  gIndenter++;

  oahElement::printElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fSubGroupVisibilityKind" << " : " <<
      subGroupVisibilityKindAsString (
        fSubGroupVisibilityKind) <<
    endl <<
    endl;

  os <<
    "Options items (" <<
    singularOrPlural (
      fSubGroupItemsList.size (), "element",  "elements") <<
    "):" <<
    endl;

  if (fSubGroupItemsList.size ()) {
    os << endl;

    gIndenter++;

    list<S_oahAtom>::const_iterator
      iBegin = fSubGroupItemsList.begin (),
      iEnd   = fSubGroupItemsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options item
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }

  gIndenter--;
}

void oahSubGroup::printHelp (ostream& os) const
{
  // print the header and option names
  os <<
    fHeader;

  os <<
    " " <<
    fetchNamesBetweenParentheses ();

  switch (fSubGroupVisibilityKind) {
    case kSubGroupVisibilityAlways:
      os <<
        ":";
      break;

    case kSubGroupVisibilityHiddenByDefault:
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
        fDescription);
    gIndenter--;

    os << endl;
  }

  switch (fSubGroupVisibilityKind) {
    case kSubGroupVisibilityAlways:
      if (fSubGroupItemsList.size ()) {
        gIndenter++;

        list<S_oahAtom>::const_iterator
          iBegin = fSubGroupItemsList.begin (),
          iEnd   = fSubGroupItemsList.end (),
          i      = iBegin;
        for ( ; ; ) {
          S_oahAtom
            oahAtom = (*i);
          bool
            oahAtomIsHidden =
              oahAtom->getIsHidden ();

          // print the options item help unless it is hidden
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

    case kSubGroupVisibilityHiddenByDefault:
      break;
  } // switch

  // register help print action in options groups's options handler upLink
  fGroupUpLink->
    getOptionsHandlerUpLink ()->
      setOptionsHandlerFoundAHelpItem ();
}

void oahSubGroup::printSubGroupForcedHelp (ostream& os) const
{
  // print the header and option names
  os <<
    fHeader;

  os <<
    " " <<
    fetchNamesBetweenParentheses ();

  switch (fSubGroupVisibilityKind) {
    case kSubGroupVisibilityAlways:
      os <<
        ":";
      break;

    case kSubGroupVisibilityHiddenByDefault:
      os <<
        " (hidden by default) :";
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

    os << endl;
  }

  if (fSubGroupItemsList.size ()) {
    gIndenter++;

    list<S_oahAtom>::const_iterator
      iBegin = fSubGroupItemsList.begin (),
      iEnd   = fSubGroupItemsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options item help
      (*i)->printHelp (os);
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for

    gIndenter--;
  }

  os << endl;

  // register help print action in options groups's options handler upLink
  fGroupUpLink->
    getOptionsHandlerUpLink ()->
      setOptionsHandlerFoundAHelpItem ();
}

void oahSubGroup::printOptionsSummary (
  ostream& os) const
{
  // fetch maximum subgroups help headers size
  int maximumSubGroupsHelpHeadersSize =
    getGroupUpLink ()->
      getOptionsHandlerUpLink ()->
        getMaximumSubGroupsHelpHeadersSize ();

  // fetch maximum short name width
  int maximumShortNameWidth =
    getGroupUpLink ()->
      getOptionsHandlerUpLink ()->
        getMaximumShortNameWidth ();

  // print the summary
  os << left <<
    setw (maximumSubGroupsHelpHeadersSize) <<
    fHeader <<
    " " <<
    fetchNamesInColumnsBetweenParentheses (
      maximumShortNameWidth);

  switch (fSubGroupVisibilityKind) {
    case kSubGroupVisibilityAlways:
      break;

    case kSubGroupVisibilityHiddenByDefault:
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
    getOptionsHandlerUpLink ()->
      setOptionsHandlerFoundAHelpItem ();
}

void oahSubGroup::printSpecificSubGroupHelp (
  ostream&      os,
  S_oahSubGroup subGroup) const
{
  // print only the summary if this is not the desired subgroup,
  // otherwise print the regular help
  if (subGroup == this) {
    os << endl;
    printSpecificSubGroupHelp (
      os,
      subGroup);
  }
  else {
    printOptionsSummary (os);
  }
 }

void oahSubGroup::printAtomForcedHelp (
  ostream&  os,
  S_oahAtom targetAtom) const
{
  // print the header
  os <<
    fHeader <<
    " " <<
    fetchNamesBetweenParentheses () <<
    ":" <<
    endl <<
    endl;

  // underline the options subgroup header
// JMI  underlineHeader (os);

  // print the options items
  if (fSubGroupItemsList.size ()) {
    gIndenter++;

    list<S_oahAtom>::const_iterator
      iBegin = fSubGroupItemsList.begin (),
      iEnd   = fSubGroupItemsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahAtom
        oahAtom = (*i);

      if (oahAtom == targetAtom) {
        // print the target options item's help
        // target options item's help
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
    getOptionsHandlerUpLink ()->
      setOptionsHandlerFoundAHelpItem ();
}

void oahSubGroup::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // print the header
  os <<
    fHeader <<
    " " <<
    fetchNamesBetweenParentheses () <<
    ":" <<
    endl;

  // underline the options subgroup header
// JMI  underlineHeader (os);

  // print the options items values
  if (fSubGroupItemsList.size ()) {
    gIndenter++;

    list<S_oahAtom>::const_iterator
      iBegin = fSubGroupItemsList.begin (),
      iEnd   = fSubGroupItemsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options item values
      (*i)->
        printOptionsValues (
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
  string           oahGroupHelpHeader,
  string           optionGroupShortName,
  string           optionGroupLongName,
  string           optionGroupDescription,
  S_oahHandler oahHandlerUpLink)
{
  oahGroup* o = new
    oahGroup (
      oahGroupHelpHeader,
      optionGroupShortName,
      optionGroupLongName,
      optionGroupDescription,
      oahHandlerUpLink);
  assert(o!=0);
  return o;
}

oahGroup::oahGroup (
  string           oahGroupHelpHeader,
  string           optionGroupShortName,
  string           optionGroupLongName,
  string           optionGroupDescription,
  S_oahHandler oahHandlerUpLink)
  : oahElement (
      optionGroupShortName,
      optionGroupLongName,
      optionGroupDescription)
{
  fHandlerUpLink = oahHandlerUpLink;

  fOptionsGroupHelpHeader = oahGroupHelpHeader;
}

oahGroup::~oahGroup ()
{}

void oahGroup::setOptionsHandlerUpLink (
  S_oahHandler handler)
{
  // sanity check
  msrAssert (
    handler != nullptr,
    "handler is null");

  // set the upLink
  fHandlerUpLink =
    handler;
}

void oahGroup::underlineHeader (ostream& os) const
{
  /* JMI
  for (unsigned int i = 0; i < fOptionsGroupHelpHeader.size (); i++) {
    os << "-";
  } // for
  os << endl;
  */
  os << "--------------------------" << endl;
}

void oahGroup::registerOptionsGroupInHandler (
  S_oahHandler handler)
{
  // sanity check
  msrAssert (
    handler != nullptr,
    "handler is null");

  // set options handler upLink
  setOptionsHandlerUpLink (handler);

  // register options group in options handler
  handler->
    registerElementInHandler (this);

  for (
    list<S_oahSubGroup>::const_iterator
      i = fOptionsGroupSubGroupsList.begin ();
    i != fOptionsGroupSubGroupsList.end ();
    i++
  ) {
    // register the options sub group
    (*i)->
      registerSubGroupInHandler (
        handler);
  } // for
}

void  oahGroup::appendSubGroup (
  S_oahSubGroup subGroup)
{
  // sanity check
  msrAssert (
    subGroup != nullptr,
    "subGroup is null");

  // append options subgroup
  fOptionsGroupSubGroupsList.push_back (
    subGroup);

  // set options subgroup group upLink
  subGroup->
    setGroupUpLink (this);
}

S_oahElement oahGroup::fetchElementByName (
  string name)
{
  S_oahElement result;

  for (
    list<S_oahSubGroup>::const_iterator
      i = fOptionsGroupSubGroupsList.begin ();
    i != fOptionsGroupSubGroupsList.end ();
    i++
  ) {
    // search name in the options group
    result =
      (*i)->fetchElementByName (name);

    if (result != 0) {
      break;
    }
  } // for

  return result;
}

S_oahValuedAtom oahGroup::handleOptionsItem (
  ostream&      os,
  S_oahAtom item)
{
  S_oahValuedAtom result;

  os <<
    "---> Options item '" <<
    item <<
    "' is not handled" <<
    endl;

  return result;
}

void oahGroup::handleOptionsItemValue (
  ostream&      os,
  S_oahAtom item,
  string        theString)
{
  os <<
    "---> Options item '" <<
    item <<
    "' with value '" <<
    theString <<
    "' is not handled" <<
    endl;
}

void oahGroup::checkOptionsConsistency ()
{}

void oahGroup::print (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "OptionsGroup:" <<
    endl;

  gIndenter++;

  oahElement::printElementEssentials (
    os, fieldWidth);

  os <<
    "Options subgroups (" <<
    singularOrPlural (
      fOptionsGroupSubGroupsList.size (), "element",  "elements") <<
    "):" <<
    endl;

  if (fOptionsGroupSubGroupsList.size ()) {
    os << endl;

    gIndenter++;

    list<S_oahSubGroup>::const_iterator
      iBegin = fOptionsGroupSubGroupsList.begin (),
      iEnd   = fOptionsGroupSubGroupsList.end (),
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

void oahGroup::printHelp (ostream& os) const
{
  // print the header and option names
  os <<
    fOptionsGroupHelpHeader <<
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
  underlineHeader (os);

  // print the options subgroups
  if (fOptionsGroupSubGroupsList.size ()) {
    gIndenter++;

    list<S_oahSubGroup>::const_iterator
      iBegin = fOptionsGroupSubGroupsList.begin (),
      iEnd   = fOptionsGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup help
      (*i)->printHelp (os);
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for

    gIndenter--;
  }

  // register help print action in options handler upLink
  fHandlerUpLink->setOptionsHandlerFoundAHelpItem ();
}

void oahGroup::printSubGroupForcedHelp (
  ostream&      os,
  S_oahSubGroup targetSubGroup) const
{
  // print the header and option names
  os <<
    fOptionsGroupHelpHeader <<
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
  underlineHeader (os);

  // print the target options subgroup
  if (fOptionsGroupSubGroupsList.size ()) {
    gIndenter++;

    list<S_oahSubGroup>::const_iterator
      iBegin = fOptionsGroupSubGroupsList.begin (),
      iEnd   = fOptionsGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahSubGroup
        subGroup = (*i);

      if (subGroup == targetSubGroup) {
        // print the target options subgroup help
        subGroup->
          printSubGroupForcedHelp (
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

void oahGroup::printGroupForcedHelp (
  ostream&      os,
  S_oahSubGroup targetSubGroup,
  S_oahAtom     targetAtom) const
{
  // print the header and option names
  os <<
    fOptionsGroupHelpHeader <<
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
  underlineHeader (os);

  // print the target options subgroup
  if (fOptionsGroupSubGroupsList.size ()) {
    gIndenter++;

    list<S_oahSubGroup>::const_iterator
      iBegin = fOptionsGroupSubGroupsList.begin (),
      iEnd   = fOptionsGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahSubGroup
        subGroup = (*i);

      if (subGroup == targetSubGroup) {
        // print the target options subgroup's
        // target options item's help
        (*i)->
          printAtomForcedHelp (
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
  fHandlerUpLink->setOptionsHandlerFoundAHelpItem ();
}

void oahGroup::printOptionsSummary (ostream& os) const
{
  // the description is the header of the information
  os <<
    fOptionsGroupHelpHeader <<
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
  underlineHeader (os);

  // print the options subgroups
  if (fOptionsGroupSubGroupsList.size ()) {
    gIndenter++;

    list<S_oahSubGroup>::const_iterator
      iBegin = fOptionsGroupSubGroupsList.begin (),
      iEnd   = fOptionsGroupSubGroupsList.end (),
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
  fHandlerUpLink->setOptionsHandlerFoundAHelpItem ();
}

void oahGroup::printSpecificSubGroupHelp (
  ostream&      os,
  S_oahSubGroup subGroup) const
{
  // the description is the header of the information
  os <<
    fOptionsGroupHelpHeader <<
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
  underlineHeader (os);

  // print the options subgroups
  if (fOptionsGroupSubGroupsList.size ()) {
    gIndenter++;

    list<S_oahSubGroup>::const_iterator
      iBegin = fOptionsGroupSubGroupsList.begin (),
      iEnd   = fOptionsGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup specific subgroup help
      (*i)->
        printSpecificSubGroupHelp (
          os,
          subGroup);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }

  // register help print action in options handler upLink
  fHandlerUpLink->setOptionsHandlerFoundAHelpItem ();
}

void oahGroup::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // print the header
  os <<
    fOptionsGroupHelpHeader <<
    " " <<
    fetchNamesBetweenParentheses () <<
    ":" <<
    endl;

  // underline the options group header
  underlineHeader (os);

  // print the options subgroups values
  if (fOptionsGroupSubGroupsList.size ()) {
    gIndenter++;

    list<S_oahSubGroup>::const_iterator
      iBegin = fOptionsGroupSubGroupsList.begin (),
      iEnd   = fOptionsGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup values
      (*i)->
        printOptionsValues (
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
S_oahPrefix oahPrefix::create (
  string oahPrefixName,
  string oahPrefixErsatz,
  string oahPrefixDescription)
{
  oahPrefix* o = new
    oahPrefix (
      oahPrefixName,
      oahPrefixErsatz,
      oahPrefixDescription);
  assert(o!=0);
  return o;
}

oahPrefix::oahPrefix (
  string oahPrefixName,
  string oahPrefixErsatz,
  string oahPrefixDescription)
{
  fOptionsPrefixName        = oahPrefixName;
  fOptionsPrefixErsatz      = oahPrefixErsatz;
  fOptionsPrefixDescription = oahPrefixDescription;
}

oahPrefix::~oahPrefix ()
{}

/* JMI
S_oahPrefix oahPrefix::fetchElementByName (
  string name)
{
  S_oahPrefix result;

  if (name == fOptionsPrefixName) {
    result = this;
  }

  return result;
}
*/

string oahPrefix::oahPrefixNames () const
{
  stringstream s;

  if (fOptionsPrefixName.size ()) {
      s <<
        "-" << fOptionsPrefixName;
  }

  return s.str ();
}

string oahPrefix::oahPrefixNamesInColumns (
  int subGroupsShortNameFieldWidth) const
{
  stringstream s;

  if (fOptionsPrefixName.size ()) {
      s << left <<
        setw (subGroupsShortNameFieldWidth) <<
        "-" + fOptionsPrefixName;
  }

  return s.str ();
}

string oahPrefix::oahPrefixNamesBetweenParentheses () const
{
  stringstream s;

  s <<
    "(" <<
    oahPrefixNames () <<
    ")";

  return s.str ();
}

string oahPrefix::oahPrefixNamesInColumnsBetweenParentheses (
  int subGroupsShortNameFieldWidth) const
{
  stringstream s;

  s <<
    "(" <<
    oahPrefixNamesInColumns (
      subGroupsShortNameFieldWidth) <<
    ")";

  return s.str ();
}

void oahPrefix::printHeader (ostream& os) const
{
  os <<
    "'" << fOptionsPrefixName <<
    "' translates to '" << fOptionsPrefixErsatz <<
    "':" <<
    endl;

  if (fOptionsPrefixDescription.size ()) {
    // indent a bit more for readability
    gIndenter++;

    os <<
      gIndenter.indentMultiLineString (
        fOptionsPrefixDescription) <<
      endl;

    gIndenter--;
  }
}

void oahPrefix::printElementEssentials (
  ostream& os,
  int      fieldWidth) const
{
  os << left <<
    setw (fieldWidth) <<
    "oahPrefixName" << " : " <<
    fOptionsPrefixName <<
    endl <<
    setw (fieldWidth) <<
    "oahPrefixErsatz" << " : " <<
    fOptionsPrefixErsatz <<
    endl <<
    setw (fieldWidth) <<
    "oahPrefixDescription" << " : " <<
    fOptionsPrefixDescription <<
    endl;
}

void oahPrefix::print (ostream& os) const
{
  os <<
    "??? oahPrefix ???" <<
    endl;

  printElementEssentials (os, 40); // JMI
}

void oahPrefix::printHelp (ostream& os) const
{
  os <<
    oahPrefixNames () <<
    endl;

  if (fOptionsPrefixErsatz.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fOptionsPrefixErsatz) <<
      endl;

    gIndenter.decrement (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);
  }

  if (fOptionsPrefixDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fOptionsPrefixDescription) <<
      endl;

    gIndenter.decrement (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);
  }

  // register help print action in options handler upLink
//  fHandlerUpLink->setOptionsHandlerFoundAHelpItem ();
}

ostream& operator<< (ostream& os, const S_oahPrefix& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
/* JMI
S_oahHandler oahHandler::create (
  string           oahHandlerHelpHeader,
  string           oahHandlerValuesHeader,
  string           optionHandlerHelpShortName,
  string           optionHandlerHelpLongName,
  string           optionHandlerHelpSummaryShortName,
  string           optionHandlerHelpSummaryLongName,
  string           optionHandlerPreamble,
  string           optionHandlerDescription,
  indentedOstream& oahHandlerlogIOstream)
{
  oahHandler* o = new
    oahHandler (
      oahHandlerHelpHeader,
      oahHandlerValuesHeader,
      optionHandlerHelpShortName,
      optionHandlerHelpLongName,
      optionHandlerHelpSummaryShortName,
      optionHandlerHelpSummaryLongName,
      optionHandlerPreamble,
      optionHandlerDescription,
      oahHandlerlogIOstream);
  assert(o!=0);
  return o;
}
*/

oahHandler::oahHandler (
  string           oahHandlerHelpHeader,
  string           oahHandlerValuesHeader,
  string           optionHandlerHelpShortName,
  string           optionHandlerHelpLongName,
  string           optionHandlerHelpSummaryShortName,
  string           optionHandlerHelpSummaryLongName,
  string           optionHandlerPreamble,
  string           optionHandlerDescription,
  indentedOstream& oahHandlerlogIOstream)
  : oahElement (
      optionHandlerHelpShortName,
      optionHandlerHelpLongName,
      optionHandlerDescription),
    fHandlerLogIOstream (
      oahHandlerlogIOstream)
{
  fHandlerHelpHeader =
    oahHandlerHelpHeader;

  fHandlerValuesHeader =
    oahHandlerValuesHeader;

  fHandlerHelpSummaryShortName =
    optionHandlerHelpSummaryShortName;
  fHandlerHelpSummaryLongName =
    optionHandlerHelpSummaryLongName;

  fHandlerPreamble =
    optionHandlerPreamble;

  fMaximumSubGroupsHelpHeadersSize = 1;

  fMaximumShortNameWidth   = 1;
  fMaximumLongNameWidth    = 1;

  fMaximumVariableNameWidth = 0;

  fHandlerFoundAHelpItem = false;
}

oahHandler::~oahHandler ()
{}

void oahHandler::registerHandlerInItself ()
{
  this->
    registerElementInHandler (this);

/* JMI ???
  // register the help summary names in handler
  registerNamesInHandler (
    fHandlerHelpSummaryShortName,
    fHandlerHelpSummaryLongName,
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
      registerOptionsGroupInHandler (
        this);
  } // for
 // */
}

S_oahPrefix oahHandler::fetchPrefixFromMap (
  string oahElementName) const
{
  S_oahPrefix result;

  // is oahAtomName known in options elements map?
  map<string, S_oahPrefix>::const_iterator
    it =
      fPrefixesMap.find (
        oahElementName);

  if (it != fPrefixesMap.end ()) {
    // yes, oahAtomName is known in the map
    result = (*it).second;
  }

  return result;
}

S_oahElement oahHandler::fetchElementFromMap (
  string oahElementName) const
{
  S_oahElement result;

  // is oahAtomName known in options elements map?
  map<string, S_oahElement>::const_iterator
    it =
      fElementsMap.find (
        oahElementName);

  if (it != fElementsMap.end ()) {
    // yes, oahAtomName is known in the map
    result = (*it).second;
  }

  return result;
}

string oahHandler::helpNamesBetweenParentheses () const
{
  stringstream s;

  s <<
    "(" <<
    fetchNames () <<
    ", ";

  if (
    fHandlerHelpSummaryShortName.size ()
        &&
    fHandlerHelpSummaryLongName.size ()
    ) {
      s <<
        "-" << fHandlerHelpSummaryShortName <<
        ", " <<
        "-" << fHandlerHelpSummaryLongName;
  }

  else {
    if (fHandlerHelpSummaryShortName.size ()) {
      s <<
      "-" << fHandlerHelpSummaryShortName;
    }
    if (fHandlerHelpSummaryLongName.size ()) {
      s <<
      "-" << fHandlerHelpSummaryLongName;
    }
  }

  s <<
    ")";

  return s.str ();
}

void oahHandler::registerNamesInHandler (
  string       optionShortName,
  string       optionLongName,
  S_oahElement element)
{
  int
    optionShortNameSize =
      optionShortName.size (),
    optionLongNameSize =
      optionLongName.size ();

  if (optionShortNameSize == 0 && optionLongNameSize == 0) {
    stringstream s;

    s <<
      "option long name and short name are both empty";

    optionError (s.str ());
    exit (33);
  }

  if (optionShortName == optionLongName) {
    stringstream s;

    s <<
      "option long name '" << optionLongName << "'" <<
      " is the same as the short name for the same";

    optionError (s.str ());
    exit (33);
  }

  for (
    map<string, S_oahElement>::iterator i =
      fElementsMap.begin ();
    i != fElementsMap.end ();
    i++
  ) {

    // is optionLongName already in the options names map?
    if ((*i).first == optionLongName) {
      stringstream s;

      s <<
        "option long name '" << optionLongName << "'" <<
          " for option short name '" << optionShortName << "'" <<
        " is specified more that once";

      optionError (s.str ());
      exit (33);
    }

    // is optionShortName already in the options names map?
    if ((*i).first == optionShortName) {
      if (optionShortName.size ()) {
        stringstream s;

        s <<
          "option short name '" << optionShortName << "'" <<
          " for option long name '" << optionLongName << "'" <<
          " is specified more that once";

        optionError (s.str ());
        exit (33);
      }
    }
  } // for

  // register element's names
  if (optionLongNameSize) {
    fElementsMap [optionLongName] = element;

    if (optionLongNameSize > fMaximumLongNameWidth) {
      fMaximumLongNameWidth = optionLongNameSize;
    }
  }

  if (optionShortNameSize) {
    fElementsMap [optionShortName] =
      element;

    if (optionShortNameSize > fMaximumShortNameWidth) {
      fMaximumShortNameWidth = optionShortNameSize;
    }
  }

  // take element's display variable name length into account
  int
    oahElementVariableNameLength =
      element->
        fetchVariableNameLength ();

    if (
      oahElementVariableNameLength
        >
      fMaximumVariableNameWidth
    ) {
      fMaximumVariableNameWidth =
        oahElementVariableNameLength;
    }
}

void oahHandler::registerElementInHandler (
  S_oahElement element)
{
  string
    optionShortName =
      element->getShortName (),
    optionLongName =
      element->getLongName ();

  // register the option names in handler
  registerNamesInHandler (
    optionShortName,
    optionLongName,
    element);

  if (
    // subgroup?
    S_oahSubGroup
      subGroup =
        dynamic_cast<oahSubGroup*>(&(*element))
    ) {

    string
      subGroupHeader=
        subGroup-> getHeader ();
    int
      subGroupHeaderSize =
        subGroupHeader.size ();

    // account for subGroup's header size
    if (subGroupHeaderSize > fMaximumSubGroupsHelpHeadersSize) {
      fMaximumSubGroupsHelpHeadersSize =
        subGroupHeaderSize;
    }
  }
}

void oahHandler::print (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "OptionsHandler:" <<
    endl;

  gIndenter++;

  printElementEssentials (os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fHandlerHelpSummaryShortName" << " : " <<
    fHandlerHelpSummaryShortName <<
    endl <<
    setw (fieldWidth) <<
    "fHandlerHelpSummaryLongName" << " : " <<
    fHandlerHelpSummaryLongName <<
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
    "oahHandlerFoundAHelpItem" << " : " <<
    fHandlerFoundAHelpItem <<
    endl;

  // print the options prefixes if any
  if (fPrefixesMap.size ()) {
    printKnownPrefixes ();
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

void oahHandler::printHelp (ostream& os) const
{
  // print the options handler preamble
  os <<
    gIndenter.indentMultiLineString (
      fHandlerPreamble);

  os << endl;

  // print the options handler help header and element names
  os <<
    fHandlerHelpHeader <<
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

  os << endl;

  // print the options groups helps
  if (fHandlerGroupsList.size ()) {
    gIndenter++;

    list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group help
      (*i)->printHelp (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }

  // register help print action in options handler upLink
//  fHandlerUpLink->setOptionsHandlerFoundAHelpItem ();
}

void oahHandler::printOptionsSummary (ostream& os) const
{
  // print the options handler preamble
  os <<
    gIndenter.indentMultiLineString (
      fHandlerPreamble);

  // print the options handler help header and element names
  os <<
    fHandlerHelpHeader <<
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

void oahHandler::printSpecificSubGroupHelp (
  ostream&      os,
  S_oahSubGroup subGroup) const
{
  // print the options handler help header and element names
  os <<
    fHandlerHelpHeader <<
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
      // print the options group specific subgroup help
      (*i)->
        printSpecificSubGroupHelp (
          os,
          subGroup);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }
}

void oahHandler::printSpecificElementHelp (
  ostream& os,
  string   name) const
{
  // is name known in options elements map?
  S_oahElement
    element =
      fetchElementFromMap (
        name);

  if (! element) {
    // name is is not well handled by this options handler
    stringstream s;

    s <<
      "option name '" << name <<
      "' is unknown, cannot deliver item specific help";

    optionError (s.str ());
    exit (33);
  }

  else {
    // name is known, let's handle it
    if (
      // options handler?
      S_oahHandler
        handler =
          dynamic_cast<oahHandler*>(&(*element))
      ) {
      // print the option handler help or help summary
      if (
        name ==
          handler->
            getOptionHandlerHelpSummaryShortName ()
          ||
        name ==
          handler->
            getOptionHandlerHelpSummaryLongName ()
         ) {
        handler->
          printOptionsSummary (
            fHandlerLogIOstream);
      }
      else {
        handler->
          printHelp (
            fHandlerLogIOstream);
      }

      fHandlerLogIOstream << endl;
    }

    else if (
      // options group?
      S_oahGroup
        group =
          dynamic_cast<oahGroup*>(&(*element))
      ) {
      // print the help
      fHandlerLogIOstream <<
        endl <<
        "--- Help for options item name '" <<
        name <<
        "' for group \"" <<
        group->
          getOptionsGroupHelpHeader () <<
        "\" ---" <<
        endl <<
        endl;

      group->
        printHelp (
          fHandlerLogIOstream);

      fHandlerLogIOstream << endl;
    }

    else if (
      // options subgroup?
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
      fHandlerLogIOstream <<
        endl <<
        "--- Help for options item name '" <<
        name <<
        "' for subgroup \"" <<
        subGroup->
          getHeader () <<
        "\"" <<
        " in group \"" <<
        group->
          getOptionsGroupHelpHeader () <<
        "\" ---" <<
        endl <<
        endl;

      group->
        printSubGroupForcedHelp (
          fHandlerLogIOstream,
          subGroup);
    }

    else if (
      // options item?
      S_oahAtom
        item =
          dynamic_cast<oahAtom*>(&(*element))
      ) {
      // get the options subgroup upLink
      S_oahSubGroup
        subGroup =
          item->
            getSubGroupUpLink ();

      // get the options group upLink
      S_oahGroup
        group =
          subGroup->
            getGroupUpLink ();

      // print the help
      fHandlerLogIOstream <<
        endl <<
        "--- Help for options item name '" <<
        name <<
        "' in subgroup \"" <<
        subGroup->
          getHeader () <<
        "\"" <<
        " of group \"" <<
        group->
          getOptionsGroupHelpHeader () <<
        "\" ---" <<
        endl <<
        endl;

      group->
        printGroupForcedHelp (
          fHandlerLogIOstream,
          subGroup,
          item);
    }

    else {
      stringstream s;

      s <<
        "cannot handle specific help about name \"" <<
        name <<
        "\"";

      optionError (s.str ());
      exit (33);
    }
  }
}

void oahHandler::printAllOptionsValues (
  ostream& os) const
{
  // print the options handler values header
  os <<
    fHandlerValuesHeader <<
    " " <<
    fetchNamesBetweenParentheses () <<
    ":" <<
    endl;

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
        printOptionsValues (
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

  string prefixName = prefix->getOptionsPrefixName ();

  S_oahPrefix result;

  // is prefixName already known in options elements map?
  map<string, S_oahPrefix>::const_iterator
    it =
      fPrefixesMap.find (
        prefixName);

  if (it != fPrefixesMap.end ()) {
    // prefixName is already known in the map
    stringstream s;

    s <<
      "option prefix name '" << prefixName <<
      "' is already known";

    optionError (s.str ());
    exit (7);
  }

  // register prefix in the options prefixes map
  fPrefixesMap [
    prefix->getOptionsPrefixName ()
    ] = prefix;
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
    setOptionsHandlerUpLink (this);
}

void oahHandler::printKnownPrefixes () const
{
  int oahHandlerOptionsPrefixesListSize =
    fPrefixesMap.size ();

  fHandlerLogIOstream <<
    "There are " <<
    oahHandlerOptionsPrefixesListSize <<
    " options prefixes:" <<
    endl;

  gIndenter++;

  if (oahHandlerOptionsPrefixesListSize) {
    // indent a bit more for readability
    for (
      map<string, S_oahPrefix>::const_iterator i =
        fPrefixesMap.begin ();
      i != fPrefixesMap.end ();
      i++
    ) {
      S_oahPrefix
        prefix = (*i).second;

      prefix->
        printHeader (
          fHandlerLogIOstream);
    } // for
  }
  else {
    fHandlerLogIOstream <<
      "none" <<
      endl;
  }

  gIndenter--;
}

void oahHandler::printKnownElements () const
{
  int oahElementsMapSize =
    fElementsMap.size ();

  // print the options elements map
  fHandlerLogIOstream <<
    "The " <<
    oahElementsMapSize <<
    " known options elements are:" <<
    endl;

  gIndenter++;

  if (oahElementsMapSize) {
    map<string, S_oahElement>::const_iterator
      iBegin = fElementsMap.begin (),
      iEnd   = fElementsMap.end (),
      i      = iBegin;
    for ( ; ; ) {
      fHandlerLogIOstream <<
        (*i).first << "-->" <<
        endl;

      gIndenter++;

      (*i).second->
        printHeader (
          fHandlerLogIOstream);

      if (++i == iEnd) break;

      gIndenter--;
    } // for
  }
  else {
    fHandlerLogIOstream <<
      "none" <<
      endl;
  }

  gIndenter--;
}

S_oahElement oahHandler::fetchElementByName (
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
      (*i)->fetchElementByName (name);

    if (result != 0) {
      break;
    }
  } // for

  return result;
}

void oahHandler::checkMissingPendingOptionsValuedAtomValue (
  string context)
{
  if (fPendingValuedAtom) {
    if (fPendingValuedAtom->getValueIsOptional ()) {
      // handle the valued item using the default value JMI
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

      optionError (s.str ());
      exit (9);
    }
  }
}

const vector<string> oahHandler::decipherOptionsAndArguments (
  int   argc,
  char* argv[])
{
  // fetch program name
  fExecutableName = string (argv [0]);

  fCommandLineWithShortOptions = fExecutableName;
  fCommandLineWithLongOptions  = fExecutableName;

  // decipher the command options and arguments
  int n = 1;

  while (true) {
    if (argv [n] == 0)
      break;

    string currentElement = string (argv [n]);

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
      // print current element
      fHandlerLogIOstream <<
        "Command line element " << n <<
        ": " << currentElement << " "<<
        endl;
    }
#endif

    // handle current element
    if (currentElement [0] == '-') {
      // stdin or options element?

      if (currentElement.size () == 1) {
        // this is the stdin indicator
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
          fHandlerLogIOstream <<
            "'" << currentElement <<
              "' is the '-' stdin indicator" <<
            endl;
        }
#endif

        fArgumentsVector.push_back (currentElement);

        fCommandLineWithShortOptions +=
          " " + currentElement;
        fCommandLineWithLongOptions +=
          " " + currentElement;
      }

      else {
        // this is an option
        string currentOptionName;

        string elementTrailer =
          currentElement.substr (1, string::npos);

        /* JMI
        fHandlerLogIOstream <<
          "elementTrailer '" << elementTrailer << "' is preceded by a dash" <<
          endl;
        */

        if (elementTrailer.size ()) {
          if (elementTrailer [0] == '-') {
            // it is a double-dashed option
            currentOptionName =
              elementTrailer.substr (1, string::npos);

#ifdef TRACE_OPTIONS
            if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
              fHandlerLogIOstream <<
                "'" << currentOptionName << "' is a double-dashed option" <<
                endl;
            }
#endif
          }
          else {
            // it is a single-dashed option
            currentOptionName =
              elementTrailer; //.substr (1, string::npos);

#ifdef TRACE_OPTIONS
            if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
              fHandlerLogIOstream <<
                "'" << currentOptionName << "' is a single-dashed option" <<
                endl;
            }
#endif
          }
        }

        else {
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
            fHandlerLogIOstream <<
              "'-' is the minimal single-dashed option" <<
              endl;
          }
#endif
        }

        // does currentOptionName contain an equal sign?
        size_t equalsSignPosition =
          currentOptionName.find ("=");

        if (equalsSignPosition != string::npos) {
          // yes

#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
            printKnownPrefixes ();
          }
#endif

          // fetch the prefix name and the string after the equals sign
          string prefixName =
            currentOptionName.substr (0, equalsSignPosition);
          string stringAfterEqualsSign =
            currentOptionName.substr (equalsSignPosition + 1);


#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
            fHandlerLogIOstream <<
              "===> equalsSignPosition = '" << equalsSignPosition <<
              "', " <<
              "===> prefixName = '" << prefixName <<
              "', " <<
              "===> stringAfterEqualsSign = '" << stringAfterEqualsSign <<
              "'" <<
              endl;
          }
#endif

          // split stringAfterEqualsSign into a list of string
          // using the comma as separator
          list<string> chunksList;

          splitStringIntoChunks (
            stringAfterEqualsSign,
            ",",
            chunksList);

          unsigned chunksListSize = chunksList.size ();

#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
            fHandlerLogIOstream <<
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
              fHandlerLogIOstream <<
                "[" << (*i) << "]";
              if (++i == iEnd) break;
              fHandlerLogIOstream <<
                " ";
            } // for

            fHandlerLogIOstream << endl;

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

                // build uncontracted option item name
                string
                  uncontractedOptionName =
                    prefix->getOptionsPrefixErsatz () + singleOptionName;

#ifdef TRACE_OPTIONS
                if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
                  fHandlerLogIOstream <<
                    "Expanding option '" << singleOptionName <<
                    "' to '" << uncontractedOptionName <<
                    "'" <<
                    endl;
                }
#endif

                // handle the uncontracted option item name
                handleElementName (uncontractedOptionName);
              } // for
            }
          }

          else {
            printKnownPrefixes ();

            stringstream s;

            s <<
              "option prefix '" << prefixName <<
              "' is unknown, see help summary below";

            optionError (s.str ());
          }


        }

        else {
          // no
          // handle the options item name
          handleElementName (currentOptionName);
        }
      }
    }

    else {
      // currentElement is no options item,
      // i.e. it is an item value or an argument
      handleOptionsItemValueOrArgument (currentElement);
    }

    // next please
    n++;
  } // while

  // is a pending options valued item value missing?
  checkMissingPendingOptionsValuedAtomValue (
    "last option in command line");

  unsigned int argumentsVectorSize =
    fArgumentsVector.size ();

#ifdef TRACE_OPTIONS
  // display arc and argv only now, to wait for the options to have been handled
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fHandlerLogIOstream <<
      "argc: " << argc <<
      endl <<
      "argv:" <<
      endl;

    gIndenter++;

    for (int i = 0; i < argc; i++) {
      fHandlerLogIOstream <<
        "argv [" << i << "]: " << argv [i] <<
        endl;
    } // for

    gIndenter--;
  }
#endif

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptionsDetails && ! gGeneralOptions->fQuiet) {
    printKnownPrefixes ();
    printKnownElements ();
  }
#endif

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    // print the arguments vector
    fHandlerLogIOstream <<
      "Arguments vector (" <<
      argumentsVectorSize <<
      " elements):" <<
      endl;

    if (argumentsVectorSize) {
      gIndenter++;
      for (unsigned int i = 0; i < argumentsVectorSize; i++) {
        fHandlerLogIOstream <<
          fArgumentsVector [i] <<
          endl;
      } // for
      gIndenter--;
    }
  }
#endif

  // was this run 'help' one?
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fHandlerLogIOstream <<
      "==> fHandlerFoundAHelpItem: " <<
      booleanAsString (fHandlerFoundAHelpItem) <<
      endl;
  }
#endif
  if (fHandlerFoundAHelpItem) {
    exit (0);
  }

  // exit if there are no arguments
  if (argumentsVectorSize == 0) {
// NO JMI ???    exit (1);
  }

  // register option element names in command line strings
  if (argumentsVectorSize) {
    for (unsigned int i = 0; i < argumentsVectorSize; i++) {
      fCommandLineWithShortOptions +=
        " " + fArgumentsVector [i];
      fCommandLineWithLongOptions +=
        " " + fArgumentsVector [i];
    } // for
  }

  // check the options and arguments
  checkOptionsAndArguments ();

  // return arguments vector for handling by caller
  return fArgumentsVector;
}

void oahHandler::handleOptionsHandlerItemName (
  S_oahHandler handler,
  string       oahAtomName)
{
  // print the option handler help or help summary
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fHandlerLogIOstream <<
      "==> element is of type 'oahHandler'" <<
      endl;
  }
#endif

  if (
    oahAtomName ==
      handler->
        getOptionHandlerHelpSummaryShortName ()
      ||
    oahAtomName ==
      handler->
        getOptionHandlerHelpSummaryLongName ()
  ) {
    handler->
      printOptionsSummary (
        fHandlerLogIOstream);
  }
  else {
    handler->
      printHelp (
        fHandlerLogIOstream);
  }

  fHandlerLogIOstream << endl;
}

void oahHandler::handleOptionsGroupItemName (
  S_oahGroup group,
  string         oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fHandlerLogIOstream <<
      "==> element is of type 'oahGroup'" <<
      endl;
  }
#endif

  // print the help
  fHandlerLogIOstream <<
    endl <<
    "--- Help for group \"" <<
    group->
      getOptionsGroupHelpHeader () <<
    "\" ---" <<
    endl <<
    endl;

  group->
    printHelp (
      fHandlerLogIOstream);
}

void oahHandler::handleSubGroupItemName (
  S_oahSubGroup subGroup,
  string        oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fHandlerLogIOstream <<
      "==> element is of type 'subGroup'" <<
      endl;
  }
#endif

  // get the options group upLink
  S_oahGroup
    group =
      subGroup->
        getGroupUpLink ();

  // print the help
  fHandlerLogIOstream <<
    endl <<
    "--- Help for subgroup \"" <<
    subGroup->
      getHeader () <<
    "\"" <<
    " in group \"" <<
    group->
      getOptionsGroupHelpHeader () <<
    "\" ---" <<
    endl <<
    endl;

  group->
    printSubGroupForcedHelp (
      fHandlerLogIOstream,
      subGroup);
}

void oahHandler::handleOptionsHelpUsageItemName (
  S_oahOptionsUsageAtom helpUsageItem,
  string                 oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fHandlerLogIOstream <<
      "==> element is of type 'oahOptionsUsageAtom'" <<
      endl;
  }
#endif

  // handle it at once
  helpUsageItem->
    printOptionsUsage (
      fHandlerLogIOstream);

  // exit
  exit (0);
}

void oahHandler::handleOptionsHelpSummaryItemName (
  S_oahOptionsSummaryAtom helpSummaryItem,
  string                   oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fHandlerLogIOstream <<
      "==> element is of type 'oahOptionsSummaryAtom'" <<
      endl;
  }
#endif

  // handle it at once
  printOptionsSummary (
    fHandlerLogIOstream);

  // exit
  exit (0);
}

void oahHandler::handleOptionsCombinedBooleanItemsItemName (
  S_oahCombinedBooleansAtom combinedBooleanItemsItem,
  string                     oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fHandlerLogIOstream <<
      "==> element is of type 'oahCombinedBooleansAtom'" <<
      endl;
  }
#endif

  // handle it at once
  combinedBooleanItemsItem->
    setBooleanVariablesValues (true);
}

void oahHandler::handleOptionsBooleanItemItemName (
  S_oahBooleanAtom booleanItem,
  string               oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fHandlerLogIOstream <<
      "==> element is of type 'oahBooleanAtom'" <<
      endl;
  }
#endif

  // handle it at once
  booleanItem->
    setBooleanItemVariableValue (true);
}

void oahHandler::handleOptionsTwoBooleansItemItemName (
  S_oahTwoBooleansAtom twoBooleansItem,
  string                   oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fHandlerLogIOstream <<
      "==> element is of type 'oahTwoBooleansAtom'" <<
      endl;
  }
#endif

  // handle it at once
  twoBooleansItem->
    setTwoBooleansItemVariableValue (true);
}

void oahHandler::handleOptionsThreeBooleansItemItemName (
  S_oahThreeBooleansAtom threeBooleansItem,
  string                     oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fHandlerLogIOstream <<
      "==> element is of type 'oahThreeBooleansAtom'" <<
      endl;
  }
#endif

  // handle it at once
  threeBooleansItem->
    setThreeBooleansItemVariableValue (true);
}

void oahHandler::handleOptionsItemHelpItemName (
  S_oahElementHelpAtom itemHelpItem,
  string                oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fHandlerLogIOstream <<
      "==> element is of type 'oahElementHelpAtom'" <<
      endl;
  }
#endif

  // wait until the value is met
  fPendingValuedAtom = itemHelpItem;
}

void oahHandler::handleOptionsIntegerItemItemName (
  S_oahIntegerAtom integerItem,
  string               oahAtomName)
{
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
      fHandlerLogIOstream <<
        "==> element is of type 'oahIntegerAtom'" <<
        endl;
    }
#endif

    // wait until the value is met
    fPendingValuedAtom = integerItem;
}

void oahHandler::handleOptionsFloatItemItemName (
  S_oahFloatAtom floatItem,
  string             oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fHandlerLogIOstream <<
      "==> element is of type 'oahFloatAtom'" <<
      endl;
  }
#endif

  // wait until the value is met
  fPendingValuedAtom = floatItem;
}

void oahHandler::handleOptionsStringItemItemName (
  S_oahStringAtom stringItem,
  string              oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fHandlerLogIOstream <<
      "==> element is of type 'oahStringAtom'" <<
      endl;
  }
#endif

  // wait until the value is met
  fPendingValuedAtom = stringItem;
}

void oahHandler::handleOptionsRationalItemItemName (
  S_oahRationalAtom rationalItem,
  string              oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fHandlerLogIOstream <<
      "==> element is of type 'oahRationalAtom'" <<
      endl;
  }
#endif

  // wait until the value is met
  fPendingValuedAtom = rationalItem;
}

void oahHandler::handleOptionsNumbersSetItemItemName (
  S_oahNumbersSetAtom numbersSetItem,
  string                  oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fHandlerLogIOstream <<
      "==> element is of type 'oahNumbersSetAtom'" <<
      endl;
  }
#endif

  // wait until the value is met
  fPendingValuedAtom = numbersSetItem;
}

void oahHandler::handleOptionsItemItemName (
  S_oahAtom item,
  string        oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fHandlerLogIOstream <<
      "==> item is of type 'oahAtom'" <<
      endl;
  }
#endif

  // is a pending options valued item value missing?
  string context =
    "before option " + item->asString ();

  checkMissingPendingOptionsValuedAtomValue (
    context);

  if (
    // help usage item?
    S_oahOptionsUsageAtom
      helpUsageItem =
        dynamic_cast<oahOptionsUsageAtom*>(&(*item))
  ) {
    handleOptionsHelpUsageItemName (
      helpUsageItem,
      oahAtomName);
  }

  else if (
    // help summary item?
    S_oahOptionsSummaryAtom
      helpSummaryItem =
        dynamic_cast<oahOptionsSummaryAtom*>(&(*item))
  ) {
    handleOptionsHelpSummaryItemName (
      helpSummaryItem,
      oahAtomName);
  }

  else if (
    // combined items item?
    S_oahCombinedBooleansAtom
      combinedBooleanItemsItem =
        dynamic_cast<oahCombinedBooleansAtom*>(&(*item))
  ) {
    handleOptionsCombinedBooleanItemsItemName (
      combinedBooleanItemsItem,
      oahAtomName);
  }

  else if (
    // boolean item?
    S_oahBooleanAtom
      booleanItem =
        dynamic_cast<oahBooleanAtom*>(&(*item))
  ) {
    handleOptionsBooleanItemItemName (
      booleanItem,
      oahAtomName);
  }

  else if (
    // two booleans item?
    S_oahTwoBooleansAtom
      twoBooleansItem =
        dynamic_cast<oahTwoBooleansAtom*>(&(*item))
  ) {
    handleOptionsTwoBooleansItemItemName (
      twoBooleansItem,
      oahAtomName);
  }

  else if (
    // three booleans item?
    S_oahThreeBooleansAtom
      threeBooleansItem =
        dynamic_cast<oahThreeBooleansAtom*>(&(*item))
  ) {
    handleOptionsThreeBooleansItemItemName (
      threeBooleansItem,
      oahAtomName);
  }

  else if (
    // item help item?
    S_oahElementHelpAtom
      itemHelpItem =
        dynamic_cast<oahElementHelpAtom*>(&(*item))
  ) {
    handleOptionsItemHelpItemName (
      itemHelpItem,
      oahAtomName);
  }

  else if (
    // integer item?
    S_oahIntegerAtom
      integerItem =
        dynamic_cast<oahIntegerAtom*>(&(*item))
  ) {
    handleOptionsIntegerItemItemName (
      integerItem,
      oahAtomName);
  }

  else if (
    // float item?
    S_oahFloatAtom
      floatItem =
        dynamic_cast<oahFloatAtom*>(&(*item))
  ) {
    handleOptionsFloatItemItemName (
      floatItem,
      oahAtomName);
  }

  else if (
    // string item?
    S_oahStringAtom
      stringItem =
        dynamic_cast<oahStringAtom*>(&(*item))
  ) {
    handleOptionsStringItemItemName (
      stringItem,
      oahAtomName);
  }

  else if (
    // rational item?
    S_oahRationalAtom
      rationalItem =
        dynamic_cast<oahRationalAtom*>(&(*item))
  ) {
    handleOptionsRationalItemItemName (
      rationalItem,
      oahAtomName);
  }

  else if (
    // numbers set item?
    S_oahNumbersSetAtom
      numbersSetItem =
        dynamic_cast<oahNumbersSetAtom*>(&(*item))
  ) {
    handleOptionsNumbersSetItemItemName (
      numbersSetItem,
      oahAtomName);
  }

  else {
    // item is of another type,
    // let the oahGroup handle it

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
      fHandlerLogIOstream <<
        "==> item is of another type" <<
        ", let the oahGroup handle it" <<
        endl;
    }
#endif

    S_oahGroup
      group =
        item->
          getSubGroupUpLink ()->
            getGroupUpLink ();

    fPendingValuedAtom =
      group->
        handleOptionsItem (
          fHandlerLogIOstream,
          item);
  }
}

void oahHandler::handleElementName (
  string name)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fHandlerLogIOstream <<
      "==> handleElementName (), name = \"" <<
      name <<
      "\"" <<
      endl;
  }
#endif

  // is name known in options elements map?
  S_oahElement
    element =
      fetchElementFromMap (
        name);

  if (! element) {
    // name is is not well handled by this options handler
    printOptionsSummary ();

    stringstream s;

    s <<
      "option name '" << name <<
      "' is unknown, see help summary above";

    optionError (s.str ());
    exit (6);
  }

  else {
    // name is known, let's handle it
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fHandlerLogIOstream <<
      "==> handleElementName (), name = \"" <<
      name <<
      "\" is described by element:" <<
      endl;
    gIndenter++;
    element->print (fHandlerLogIOstream);
    gIndenter--;
  }
#endif

    fCommandOptionsElements.push_back (element);

    // determine option element short and long names to be used,
    // in case one of them (short or long) is empty
    string
      shortName =
        element->getShortName (),
      longName =
        element->getLongName ();

    string
      shortNameToBeUsed = shortName,
      longNameToBeUsed = longName;

    // replace empty option element name if any by the other one,
    // since they can't both be empty
    if (! shortNameToBeUsed.size ()) {
      shortNameToBeUsed = longNameToBeUsed;
    }
    if (! longNameToBeUsed.size ()) {
      longNameToBeUsed = shortNameToBeUsed;
    }

    // register option element names in command line strings
    fCommandLineWithShortOptions +=
      " -" + shortNameToBeUsed;
    fCommandLineWithLongOptions +=
      " -" + longNameToBeUsed;

    // handle the option element
    if (
      // options handler?
      S_oahHandler
        handler =
          dynamic_cast<oahHandler*>(&(*element))
    ) {
        handleOptionsHandlerItemName (
          handler,
          name);
    }

    else if (
      // options group?
      S_oahGroup
        group =
          dynamic_cast<oahGroup*>(&(*element))
    ) {
      handleOptionsGroupItemName (
        group,
        name);
    }

    else if (
      // options subgroup?
      S_oahSubGroup
        subGroup =
          dynamic_cast<oahSubGroup*>(&(*element))
    ) {
      handleSubGroupItemName (
        subGroup,
        name);
    }

    else if (
      // options item?
      S_oahAtom
        item =
          dynamic_cast<oahAtom*>(&(*element))
    ) {

      handleOptionsItemItemName (
        item,
        name);
    }

    else {
      stringstream s;

      s <<
        "INTERNAL ERROR: option name '" << name <<
        "' cannot be handled";

      optionError (s.str ());
      exit (7);
    }
  }
}

void oahHandler::handleOptionsItemHelpValue (
  S_oahElementHelpAtom itemHelpItem,
  string                theString)
{
  // handle the option item
  printSpecificElementHelp (
    fHandlerLogIOstream,
    theString);

  // exit
  exit (23);
}

void oahHandler::handleOptionsItemIntegerValue (
  S_oahIntegerAtom integerItem,
  string               theString)
{
  // handle the option item

  int integerValue;
  {
    stringstream s;
    s << theString;
    s >> integerValue;
  }

  integerItem->
    setIntegerVariableValue (
      integerValue);
}

void oahHandler::handleOptionsItemFloatValue (
  S_oahFloatAtom floatItem,
  string             theString)
{
  // handle the option item
  float floatValue;
  {
    stringstream s;

    s << setprecision (2) << theString;
    s >> floatValue;
  }

  floatItem->
    setFloatItemVariableValue (
      floatValue);
}

void oahHandler::handleOptionsItemStringValue (
  S_oahStringAtom stringItem,
  string              theString)
{
  // handle the option item
  stringItem->
    setStringItemVariableValue (
      theString);
}

void oahHandler::handleOptionsItemRationalValue (
  S_oahRationalAtom rationalItem,
  string                theString)
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

  if (smSize) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
      fHandlerLogIOstream <<
        "There are " << smSize << " matches" <<
        " for rational string '" << theString <<
        "' with regex '" << regularExpression <<
        "'" <<
        endl;

      for (unsigned i = 0; i < smSize; ++i) {
        fHandlerLogIOstream <<
          "[" << sm [i] << "] ";
      } // for

      fHandlerLogIOstream << endl;
    }
#endif
  }

  else {
    stringstream s;

    s <<
      "-delayedOrnamentFraction argument '" << theString <<
      "' is ill-formed";

    optionError (s.str ());

    printSpecificSubGroupHelp (
      fHandlerLogIOstream,
      rationalItem->getSubGroupUpLink ());

    exit (4);
  }

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

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fHandlerLogIOstream << // JMI
      "rationalValue = " <<
      rationalValue <<
      endl;
  }
#endif

  rationalItem->
    setRationalItemVariableValue (
      rationalValue);
}

void oahHandler::handleOptionsItemNumbersSetValue (
  S_oahNumbersSetAtom numbersSetItem,
  string                  theString)
{
  // theString contains the set specification,
  // decipher it
  numbersSetItem->
    setNumbersSetItemVariableValue (
      decipherNumbersSetSpecification (
        theString, false) // 'true' to debug it
      );
}

void oahHandler::handleOptionsItemValueOrArgument (
  string theString)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fHandlerLogIOstream <<
      "==> handleOptionsItemValueOrArgument ()" <<
      endl;

    gIndenter++;

    fHandlerLogIOstream <<
      "fPendingValuedAtom:" <<
      endl;

    gIndenter++;
    if (fPendingValuedAtom) {
      fHandlerLogIOstream <<
        fPendingValuedAtom;
    }
    else {
      fHandlerLogIOstream <<
        "null" <<
        endl;
    }
    gIndenter--;

    fHandlerLogIOstream <<
      "theString:" <<
      endl;

    gIndenter++;
    fHandlerLogIOstream <<
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
    // theString is the value for the pending options item

    if (
      // item help item?
      S_oahElementHelpAtom
        itemHelpItem =
          dynamic_cast<oahElementHelpAtom*>(&(*fPendingValuedAtom))
    ) {
      handleOptionsItemHelpValue (
        itemHelpItem,
        theString);
    }

    else if (
      // integer item?
      S_oahIntegerAtom
        integerItem =
          dynamic_cast<oahIntegerAtom*>(&(*fPendingValuedAtom))
    ) {
      handleOptionsItemIntegerValue (
        integerItem,
        theString);
    }

    else if (
      // float item?
      S_oahFloatAtom
        floatItem =
          dynamic_cast<oahFloatAtom*>(&(*fPendingValuedAtom))
    ) {
      handleOptionsItemFloatValue (
        floatItem,
        theString);
    }

    else if (
      // string item?
      S_oahStringAtom
        stringItem =
          dynamic_cast<oahStringAtom*>(&(*fPendingValuedAtom))
    ) {
      handleOptionsItemStringValue (
        stringItem,
        theString);
    }

    else if (
      // rational item?
      S_oahRationalAtom
        rationalItem =
          dynamic_cast<oahRationalAtom*>(&(*fPendingValuedAtom))
    ) {
      handleOptionsItemRationalValue (
        rationalItem,
        theString);
    }

    else if (
      // numbers set item?
      S_oahNumbersSetAtom
        numbersSetItem =
          dynamic_cast<oahNumbersSetAtom*>(&(*fPendingValuedAtom))
    ) {
      handleOptionsItemNumbersSetValue (
        numbersSetItem,
        theString);
    }

    else {
      // fPendingValuedAtom is of another type,
      // let the oahGroup handle it

#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
        fHandlerLogIOstream <<
          "==> fPendingValuedAtom is of another type" <<
          ", let the oahGroup handle it" <<
          endl;
      }
#endif

      S_oahGroup
        group =
          fPendingValuedAtom->
            getSubGroupUpLink ()->
              getGroupUpLink ();

      group->
        handleOptionsItemValue (
          fHandlerLogIOstream,
          fPendingValuedAtom,
          theString);
    }

    fPendingValuedAtom = nullptr;
  }

  else {
    // theString is an argument

#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
        fHandlerLogIOstream <<
          "'" << theString << "'" <<
          " is an argument, not an option" <<
          endl;
      }
#endif

    fArgumentsVector.push_back (theString);
  }
}


}

/*
      // is a pending options valued item value missing?
      checkMissingPendingOptionsValuedAtomValue (
        "FOO");
*/
