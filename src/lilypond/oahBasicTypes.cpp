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
  - options handling is organized as a hierarchical, instrospective set of classes.
    Options and their corresponding help are grouped in a single object.

  - oahElement is the super-class of all options elements.
    It contains a short name and a long name, as well as a decription.
    Short and long names can be used and mixed at will in the command line,
    as well as '-' and '--'.
    The short name is mandatory, but the long name may be empty.

  - prefixes such '-t=' and -help=' allow for a contracted form of options.
    For example, -t=meas,notes is short for '-t-meas, -tnotes'.
    An optionsPrefix contains the prefix name, the ersatz by which to replace it,
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

  - there are various subclasses of oahAtom such as optionsIntegerItem, oahBooleanAtom
    and optionsRationalItem to control options values of common types.

  - oahThreeBooleansAtom, for example, allows for three boolean settings
    to be controlled at once with a single option.

  - optionsValuedItem describes options for which a value is supplied in the command line.

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
    - handleOptionsItemName() handles the item names
    - handleOptionsItemValueOrArgument() handles the values that may follow an item name
      and the arguments to the executable.

  - contracted forms are expanded in handleOptionsItemName() before the resulting,
    uncontracted options are handled.

  - handleOptionsItemName() fetches the oahElement corresponding to the name from the map,
    determines the type of the latter,
    and delegates the handling to the corresponding object.

  - handleOptionsItemValueOrArgument() associatiates the value
    to the (preceding) fPendingOptionsValuedItem if not null,
    or appends it fArgumentsVector to otherwise.

  - the printHelpSummary() methods are used when there are errors in the options used.

  - the printHelp() methods perform the actual help print work

  - options deciphering it done by the handleOptionsItem() methods defined:
      - in oahBasicTypes.h/.cpp for the predefined ones;
      - in the various options groups for those specific to the latter.
    The value following the option name, if any, is taken care of
    by the handleOptionsItemValue() methods.
*/

//______________________________________________________________________________
S_oahElement oahElement::create (
  string oahElementShortName,
  string oahElementLongName,
  string oahElementDescription)
{
  oahElement* o = new
    oahElement (
      oahElementShortName,
      oahElementLongName,
      oahElementDescription);
  assert(o!=0);
  return o;
}

oahElement::oahElement (
  string oahElementShortName,
  string oahElementLongName,
  string oahElementDescription)
{
  fOptionsElementShortName   = oahElementShortName;
  fOptionsElementLongName    = oahElementLongName;
  fOptionsElementDescription = oahElementDescription;

  fOptionsElementIsHidden    = false;
}

oahElement::~oahElement ()
{}

S_oahElement oahElement::fetchOptionElement (
  string optiontElementName)
{
  S_oahElement result;

  if (
    optiontElementName == fOptionsElementShortName
     ||
    optiontElementName == fOptionsElementLongName) {
    result = this;
  }

  return result;
}

string oahElement::oahElementNames () const
{
  stringstream s;

  if (
    fOptionsElementShortName.size ()
        &&
    fOptionsElementLongName.size ()
    ) {
      s <<
        "-" << fOptionsElementShortName <<
        ", " <<
        "-" << fOptionsElementLongName;
  }

  else {
    if (fOptionsElementShortName.size ()) {
      s <<
      "-" << fOptionsElementShortName;
    }
    if (fOptionsElementLongName.size ()) {
      s <<
      "-" << fOptionsElementLongName;
    }
  }

  return s.str ();
}

string oahElement::oahElementNamesInColumns (
  int subGroupsShortNameFieldWidth) const
{
  stringstream s;

  if (
    fOptionsElementShortName.size ()
        &&
    fOptionsElementLongName.size ()
    ) {
      s << left <<
        setw (subGroupsShortNameFieldWidth) <<
        "-" + fOptionsElementShortName <<
        ", " <<
        "-" << fOptionsElementLongName;
  }

  else {
    if (fOptionsElementShortName.size ()) {
      s << left <<
        setw (subGroupsShortNameFieldWidth) <<
          "-" + fOptionsElementShortName;
    }
    if (fOptionsElementLongName.size ()) {
      s <<
        "-" << fOptionsElementLongName;
    }
  }

  return s.str ();
}

string oahElement::oahElementNamesBetweenParentheses () const
{
  stringstream s;

  s <<
    "(" <<
    oahElementNames () <<
    ")";

  return s.str ();
}

string oahElement::oahElementNamesInColumnsBetweenParentheses (
  int subGroupsShortNameFieldWidth) const
{
  stringstream s;

  s <<
    "(" <<
    oahElementNamesInColumns (
      subGroupsShortNameFieldWidth) <<
    ")";

  return s.str ();
}

string oahElement::asString () const
{
  stringstream s;

  s <<
    "'"
    "-" << fOptionsElementShortName <<
    "," <<
    "-" << fOptionsElementLongName <<
// JMI    ": " << fOptionsElementDescription <<
    "'";

  return s.str ();
}

void oahElement::printHeader (ostream& os) const
{
  os <<
    "-" << fOptionsElementShortName <<
    endl <<
    "-" << fOptionsElementLongName <<
    endl;

  if (fOptionsElementDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fOptionsElementDescription) <<
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
    "fOptionsElementShortName" << " : " <<
    fOptionsElementShortName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsElementLongName" << " : " <<
    fOptionsElementLongName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsElementDescription" << " : " <<
    fOptionsElementDescription <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsElementIsHidden" << " : " <<
    booleanAsString (
      fOptionsElementIsHidden) <<
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
    oahElementNames () <<
    endl;

  if (fOptionsElementDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fOptionsElementDescription) <<
      endl;

    gIndenter.decrement (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);
  }

  // register help print action in options handler upLink JMI ???
//  fOptionsHandlerUpLink->setOptionsHandlerFoundAHelpItem ();
}

ostream& operator<< (ostream& os, const S_oahElement& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahAtom oahAtom::create (
  string oahAtomShortName,
  string oahAtomLongName,
  string oahAtomDescription)
{
  oahAtom* o = new
    oahAtom (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription);
  assert(o!=0);
  return o;
}

oahAtom::oahAtom (
  string oahAtomShortName,
  string oahAtomLongName,
  string oahAtomDescription)
  : oahElement (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription)
{}

oahAtom::~oahAtom ()
{}

void oahAtom::setOptionsSubGroupUpLink (
  S_oahSubGroup oahSubGroup)
{
  // sanity check
  msrAssert (
    oahSubGroup != nullptr,
    "oahSubGroup is null");

  // set the upLink
  fOptionsSubGroupUpLink =
    oahSubGroup;
}

void oahAtom::registerOptionsItemInHandler (
  S_oahHandler oahHandler)
{
  oahHandler->
    registerOptionsElementInHandler (this);
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
S_oahHelpUsageAtom oahHelpUsageAtom::create (
  string oahAtomShortName,
  string oahAtomLongName,
  string oahAtomDescription)
{
  oahHelpUsageAtom* o = new
    oahHelpUsageAtom (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription);
  assert(o!=0);
  return o;
}

oahHelpUsageAtom::oahHelpUsageAtom (
  string oahAtomShortName,
  string oahAtomLongName,
  string oahAtomDescription)
  : oahAtom (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription)
{}

oahHelpUsageAtom::~oahHelpUsageAtom ()
{}

void oahHelpUsageAtom::print (ostream& os) const
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
      fOptionsElementDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void oahHelpUsageAtom::printHelpUsage (ostream& os) const
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

void oahHelpUsageAtom::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_oahHelpUsageAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahHelpSummaryAtom oahHelpSummaryAtom::create (
  string oahAtomShortName,
  string oahAtomLongName,
  string oahAtomDescription)
{
  oahHelpSummaryAtom* o = new
    oahHelpSummaryAtom (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription);
  assert(o!=0);
  return o;
}

oahHelpSummaryAtom::oahHelpSummaryAtom (
  string oahAtomShortName,
  string oahAtomLongName,
  string oahAtomDescription)
  : oahAtom (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription)
{}

oahHelpSummaryAtom::~oahHelpSummaryAtom ()
{}

void oahHelpSummaryAtom::print (ostream& os) const
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
      fOptionsElementDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void oahHelpSummaryAtom::printHelpSummary (ostream& os) const
{
  os <<
    gGeneralOptions->fExecutableName <<
    endl;
}

void oahHelpSummaryAtom::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_oahHelpSummaryAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahAtomWithVariableDisplayName oahAtomWithVariableDisplayName::create (
  string oahAtomShortName,
  string oahAtomLongName,
  string oahAtomDescription,
  string oahAtomVariableDisplayName)
{
  oahAtomWithVariableDisplayName* o = new
    oahAtomWithVariableDisplayName (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      oahAtomVariableDisplayName);
  assert(o!=0);
  return o;
}

oahAtomWithVariableDisplayName::oahAtomWithVariableDisplayName (
  string oahAtomShortName,
  string oahAtomLongName,
  string oahAtomDescription,
  string oahAtomVariableDisplayName)
  : oahAtom (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription),
    fOptionsItemVariableDisplayName (
      oahAtomVariableDisplayName)
{}

oahAtomWithVariableDisplayName::~oahAtomWithVariableDisplayName ()
{}

void oahAtomWithVariableDisplayName::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsItemWithVariableDisplayName:" <<
    endl;

  gIndenter++;

  oahElement::printElementEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;
  gIndenter--;

  os << left <<
    setw (fieldWidth) <<
    "fOptionsItemVariableDisplayName" << " : " <<
    fOptionsItemVariableDisplayName <<
    endl <<
    endl;

  gIndenter--;
}

void oahAtomWithVariableDisplayName::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsItemVariableDisplayName <<
    " : " <<
    "FOO" <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahAtomWithVariableDisplayName& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahBooleanAtom oahBooleanAtom::create (
  string oahAtomShortName,
  string oahAtomLongName,
  string oahAtomDescription,
  string oahBooleanAtomVariableDisplayName,
  bool&  oahBooleanAtomVariable)
{
  oahBooleanAtom* o = new
    oahBooleanAtom (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      oahBooleanAtomVariableDisplayName,
      oahBooleanAtomVariable);
  assert(o!=0);
  return o;
}

oahBooleanAtom::oahBooleanAtom (
  string oahAtomShortName,
  string oahAtomLongName,
  string oahAtomDescription,
  string oahBooleanAtomVariableDisplayName,
  bool&  oahBooleanAtomVariable)
  : oahAtomWithVariableDisplayName (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      oahBooleanAtomVariableDisplayName),
    fOptionsBooleanItemVariable (
      oahBooleanAtomVariable)
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
      fOptionsElementDescription) <<
    endl;
  gIndenter--;

  os << left <<
    setw (fieldWidth) <<
    "fOptionsItemVariableDisplayName" << " : " <<
    fOptionsItemVariableDisplayName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsBooleanItemVariable" << " : " <<
    booleanAsString (
      fOptionsBooleanItemVariable) <<
    endl;

  gIndenter--;
}

void oahBooleanAtom::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsItemVariableDisplayName <<
    " : " <<
    booleanAsString (
      fOptionsBooleanItemVariable) <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahBooleanAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahTwoBooleansAtom oahTwoBooleansAtom::create (
  string oahAtomShortName,
  string oahAtomLongName,
  string oahAtomDescription,
  string oahTwoBooleansAtomVariableDisplayName,
  bool&  oahTwoBooleansAtomVariable,
  bool&  oahTwoBooleansAtomSecondaryVariable)
{
  oahTwoBooleansAtom* o = new
    oahTwoBooleansAtom (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      oahTwoBooleansAtomVariableDisplayName,
      oahTwoBooleansAtomVariable,
      oahTwoBooleansAtomSecondaryVariable);
  assert(o!=0);
  return o;
}

oahTwoBooleansAtom::oahTwoBooleansAtom (
  string oahAtomShortName,
  string oahAtomLongName,
  string oahAtomDescription,
  string oahTwoBooleansAtomVariableDisplayName,
  bool&  oahTwoBooleansAtomVariable,
  bool&  oahTwoBooleansAtomSecondaryVariable)
  : oahAtomWithVariableDisplayName (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      oahTwoBooleansAtomVariableDisplayName),
    fOptionsTwoBooleansItemVariable (
      oahTwoBooleansAtomVariable),
    fOptionsTwoBooleansItemSecondaryVariable (
      oahTwoBooleansAtomSecondaryVariable)
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
      fOptionsElementDescription);
  gIndenter--;

  os << left <<
    setw (fieldWidth) <<
    "fOptionsItemVariableDisplayName" << " : " <<
    fOptionsItemVariableDisplayName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsTwoBooleansItemVariable" << " : " <<
    booleanAsString (
      fOptionsTwoBooleansItemVariable) <<
    endl;

  gIndenter--;
}

void oahTwoBooleansAtom::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsItemVariableDisplayName <<
    " : " <<
    booleanAsString (
      fOptionsTwoBooleansItemVariable) <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahTwoBooleansAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahThreeBooleansAtom oahThreeBooleansAtom::create (
  string oahAtomShortName,
  string oahAtomLongName,
  string oahAtomDescription,
  string oahThreeBooleansAtomVariableDisplayName,
  bool&  oahThreeBooleansAtomVariable,
  bool&  oahThreeBooleansAtomSecondaryVariable,
  bool&  oahThreeBooleansAtomTertiaryVariable)
{
  oahThreeBooleansAtom* o = new
    oahThreeBooleansAtom (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      oahThreeBooleansAtomVariableDisplayName,
      oahThreeBooleansAtomVariable,
      oahThreeBooleansAtomSecondaryVariable,
      oahThreeBooleansAtomTertiaryVariable);
  assert(o!=0);
  return o;
}

oahThreeBooleansAtom::oahThreeBooleansAtom (
  string oahAtomShortName,
  string oahAtomLongName,
  string oahAtomDescription,
  string oahThreeBooleansAtomVariableDisplayName,
  bool&  oahThreeBooleansAtomVariable,
  bool&  oahThreeBooleansAtomSecondaryVariable,
  bool&  oahThreeBooleansAtomTertiaryVariable)
  : oahAtomWithVariableDisplayName (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      oahThreeBooleansAtomVariableDisplayName),
    fOptionsThreeBooleansItemVariable (
      oahThreeBooleansAtomVariable),
    fOptionsThreeBooleansItemSecondaryVariable (
      oahThreeBooleansAtomSecondaryVariable),
    fOptionsThreeBooleansItemTertiaryVariable (
      oahThreeBooleansAtomTertiaryVariable)
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
      fOptionsElementDescription) <<
    endl;
  gIndenter--;

  os << left <<
    setw (fieldWidth) <<
    "fOptionsItemVariableDisplayName" << " : " <<
    fOptionsItemVariableDisplayName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsThreeBooleansItemVariable" << " : " <<
    booleanAsString (
      fOptionsThreeBooleansItemVariable) <<
    endl;

  gIndenter--;
}

void oahThreeBooleansAtom::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsItemVariableDisplayName <<
    " : " <<
    booleanAsString (
      fOptionsThreeBooleansItemVariable) <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahThreeBooleansAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahCombinedBooleansAtom oahCombinedBooleansAtom::create (
  string oahAtomShortName,
  string oahAtomLongName,
  string oahAtomDescription,
  string oahCombinedBooleansAtomVariableDisplayName,
  bool&  oahCombinedBooleansAtomVariable)
{
  oahCombinedBooleansAtom* o = new
    oahCombinedBooleansAtom (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      oahCombinedBooleansAtomVariableDisplayName,
      oahCombinedBooleansAtomVariable);
  assert(o!=0);
  return o;
}

oahCombinedBooleansAtom::oahCombinedBooleansAtom (
  string oahAtomShortName,
  string oahAtomLongName,
  string oahAtomDescription,
  string oahCombinedBooleansAtomVariableDisplayName,
  bool&  oahCombinedBooleansAtomVariable)
  : oahAtomWithVariableDisplayName (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      oahCombinedBooleansAtomVariableDisplayName),
    fOptionsCombinedBooleanItemsItemVariable (
      oahCombinedBooleansAtomVariable)
{}

oahCombinedBooleansAtom::~oahCombinedBooleansAtom ()
{}

void oahCombinedBooleansAtom::appendOptionsItemToCombinedBooleanItemsList (
  S_oahAtom oahAtom)
{
  // sanity check
  msrAssert (
    oahAtom != nullptr,
    "oahAtom is null");

  fOptionsCombinedBooleanItemsList.push_back (
    oahAtom);
}

void oahCombinedBooleansAtom::appendOptionsItemToCombinedBooleanItemsList (
  string oahAtomName)
{
  // get the options handler
  S_oahHandler
    oahHandler =
      getOptionsSubGroupUpLink ()->
        getOptionsGroupUpLink ()->
          getOptionsHandlerUpLink ();

  // sanity check
  msrAssert (
    oahHandler != nullptr,
    "oahHandler is null");

  // is oahAtomName known in options elements map?
  S_oahElement
    oahElement =
      oahHandler->
        fetchOptionsElementFromMap (
          oahAtomName);

  if (! oahElement) {
    // no, oahAtomName is unknown in the map
    oahHandler->
      printHelpSummary ();

    stringstream s;

    s <<
      "INTERNAL ERROR: option name '" << oahAtomName <<
      "' is unknown";

    optionError (s.str ());
  }

  else {
    // oahAtomName is known, let's handle it

    if (
      // options item?
      S_oahAtom
        item =
          dynamic_cast<oahAtom*>(&(*oahElement))
      ) {
      // handle the option item
      fOptionsCombinedBooleanItemsList.push_back (
        item);
    }

    else {
      stringstream s;

      s <<
        "option name '" << oahAtomName <<
        "' is not that of an item";

      optionError (s.str ());

      exit (2);
    }
  }
}

void oahCombinedBooleansAtom::setCombinedBooleanItemsVariablesValue (
  bool value)
{
  // set the combined items item variable
  fOptionsCombinedBooleanItemsItemVariable = value;

  // set the value of the items in the list
  if (fOptionsCombinedBooleanItemsList.size ()) {
    for (
      list<S_oahAtom>::const_iterator i =
        fOptionsCombinedBooleanItemsList.begin ();
      i != fOptionsCombinedBooleanItemsList.end ();
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
    "fOptionsCombinedBooleanItemsList" << " : ";

  if (! fOptionsCombinedBooleanItemsList.size ()) {
    os <<
      "none";
  }

  else {
    os << endl;

    gIndenter++;

    os << "'";

    list<S_oahAtom>::const_iterator
      iBegin = fOptionsCombinedBooleanItemsList.begin (),
      iEnd   = fOptionsCombinedBooleanItemsList.end (),
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
    oahElementNames () <<
    endl;

  if (fOptionsElementDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fOptionsElementDescription) <<
      endl;
  }

  os <<
    "This combined option is equivalent to: ";

  if (! fOptionsCombinedBooleanItemsList.size ()) {
    os <<
      "none" <<
      endl;
  }

  else {
    os << endl;

    gIndenter++;

    list<S_oahAtom>::const_iterator
      iBegin = fOptionsCombinedBooleanItemsList.begin (),
      iEnd   = fOptionsCombinedBooleanItemsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os <<
        (*i)-> oahElementNames ();
      if (++i == iEnd) break;
      os << endl;
    } // for

    os << endl;

    gIndenter--;
  }

  if (fOptionsElementDescription.size ()) { // ??? JMI
    gIndenter.decrement (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);
  }

  // register help print action in options handler upLink
//  fOptionsHandlerUpLink->setOptionsHandlerFoundAHelpItem ();
}

void oahCombinedBooleansAtom::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsItemVariableDisplayName <<
    " : " <<
    booleanAsString (
      fOptionsCombinedBooleanItemsItemVariable) <<
    endl;

  int fieldWidth =
    valueFieldWidth - gIndenter.getIndent () + 1;

  gIndenter++; // only now

  if (! fOptionsCombinedBooleanItemsList.size ()) {
    os <<
      "none" <<
      endl;
  }

  else {
    list<S_oahAtom>::const_iterator
      iBegin = fOptionsCombinedBooleanItemsList.begin (),
      iEnd   = fOptionsCombinedBooleanItemsList.end (),
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
S_optionsValuedItem optionsValuedItem::create (
  string oahAtomShortName,
  string oahAtomLongName,
  string oahAtomDescription,
  string optionsValueSpecification,
  string optionsValuedItemVariableDisplayName)
{
  optionsValuedItem* o = new
    optionsValuedItem (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      optionsValueSpecification,
      optionsValuedItemVariableDisplayName);
  assert(o!=0);
  return o;
}

optionsValuedItem::optionsValuedItem (
  string oahAtomShortName,
  string oahAtomLongName,
  string oahAtomDescription,
  string optionsValueSpecification,
  string optionsValuedItemVariableDisplayName)
  : oahAtomWithVariableDisplayName (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      optionsValuedItemVariableDisplayName)
{
  fOptionsValueSpecification = optionsValueSpecification;

  fOptionsValueIsOptional = false;
}

optionsValuedItem::~optionsValuedItem ()
{}

string optionsValuedItem::oahAtomKindAsString (
  optionsValuedItem::optionsValuedItemKind oahAtomKind)
{
  string result;

  switch (oahAtomKind) {
    case optionsValuedItem::kOptionsItemHasNoArgument:
      result = "OptionsItemHasNoArgument";
      break;
    case optionsValuedItem::kOptionsItemHasARequiredArgument:
      result = "OptionsItemHasARequiredArgument";
      break;
    case optionsValuedItem::kOptionsItemHasAnOptionsArgument:
      result = "OptionsItemHasAnOptionsArgument";
      break;
  } // switch

  return result;
}

void optionsValuedItem::printValuedItemEssentials (
  ostream& os,
  int      fieldWidth) const
{
  printElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsValueSpecification" << " : " <<
    fOptionsValueSpecification <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsValueIsOptional" << " : " <<
    booleanAsString (fOptionsValueIsOptional) <<
    endl;
}

void optionsValuedItem::print (ostream& os) const
{
  const int fieldWidth = 19;

  os <<
    "OptionsValuedItem ???:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  gIndenter--;
}

void optionsValuedItem::printHelp (ostream& os) const
{
  os <<
    oahElementNames () <<
    " " <<
    fOptionsValueSpecification <<
    endl;

  if (fOptionsElementDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fOptionsElementDescription) <<
      endl;

    gIndenter.decrement (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);
  }

/* superfluous JMI
  if (fOptionsValueIsOptional) {
    os <<
      "option value is optionnal" <<
      endl;
  }
*/

  // register help print action in options handler upLink // JMI
//  fOptionsHandlerUpLink->setOptionsHandlerFoundAHelpItem ();
}

void optionsValuedItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os <<
    "OptionsItem values ???:" <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsValuedItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahAtomHelpItem oahAtomHelpItem::create (
  string             oahAtomShortName,
  string             oahAtomLongName,
  string             oahAtomDescription,
  string             optionsValueSpecification,
  string             optionsHelpItemVariableDisplayName)
{
  oahAtomHelpItem* o = new
    oahAtomHelpItem (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      optionsValueSpecification,
      optionsHelpItemVariableDisplayName);
  assert(o!=0);
  return o;
}

oahAtomHelpItem::oahAtomHelpItem (
  string             oahAtomShortName,
  string             oahAtomLongName,
  string             oahAtomDescription,
  string             optionsValueSpecification,
  string             optionsHelpItemVariableDisplayName)
  : optionsValuedItem (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      optionsValueSpecification,
      optionsHelpItemVariableDisplayName)
{}

oahAtomHelpItem::~oahAtomHelpItem ()
{}

void oahAtomHelpItem::print (ostream& os) const
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

void oahAtomHelpItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_oahAtomHelpItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsIntegerItem optionsIntegerItem::create (
  string             oahAtomShortName,
  string             oahAtomLongName,
  string             oahAtomDescription,
  string             optionsValueSpecification,
  string             optionsIntegerItemVariableDisplayName,
  int&               optionsIntegerItemVariable)
{
  optionsIntegerItem* o = new
    optionsIntegerItem (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      optionsValueSpecification,
      optionsIntegerItemVariableDisplayName,
      optionsIntegerItemVariable);
  assert(o!=0);
  return o;
}

optionsIntegerItem::optionsIntegerItem (
  string             oahAtomShortName,
  string             oahAtomLongName,
  string             oahAtomDescription,
  string             optionsValueSpecification,
  string             optionsIntegerItemVariableDisplayName,
  int&               optionsIntegerItemVariable)
  : optionsValuedItem (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      optionsValueSpecification,
      optionsIntegerItemVariableDisplayName),
    fOptionsIntegerItemVariable (
      optionsIntegerItemVariable)
{}

optionsIntegerItem::~optionsIntegerItem ()
{}

void optionsIntegerItem::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsIntegerItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsItemVariableDisplayName" << " : " <<
    fOptionsItemVariableDisplayName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsIntegerItemVariable" << " : " <<
    fOptionsIntegerItemVariable <<
    endl;

  gIndenter--;
}

void optionsIntegerItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsItemVariableDisplayName <<
    " : " <<
    fOptionsIntegerItemVariable <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsIntegerItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsFloatItem optionsFloatItem::create (
  string             oahAtomShortName,
  string             oahAtomLongName,
  string             oahAtomDescription,
  string             optionsValueSpecification,
  string             optionsFloatItemVariableDisplayName,
  float&             optionsFloatItemVariable)
{
  optionsFloatItem* o = new
    optionsFloatItem (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      optionsValueSpecification,
      optionsFloatItemVariableDisplayName,
      optionsFloatItemVariable);
  assert(o!=0);
  return o;
}

optionsFloatItem::optionsFloatItem (
  string             oahAtomShortName,
  string             oahAtomLongName,
  string             oahAtomDescription,
  string             optionsValueSpecification,
  string             optionsFloatItemVariableDisplayName,
  float&             optionsFloatItemVariable)
  : optionsValuedItem (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      optionsValueSpecification,
      optionsFloatItemVariableDisplayName),
    fOptionsFloatItemVariable (
      optionsFloatItemVariable)
{}

optionsFloatItem::~optionsFloatItem ()
{}

void optionsFloatItem::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsFloatItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsItemVariableDisplayName" << " : " <<
    fOptionsItemVariableDisplayName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsFloatItemVariable" << " : " <<
    fOptionsFloatItemVariable <<
    endl;

  gIndenter--;
}

void optionsFloatItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsItemVariableDisplayName <<
    " : " <<
    fOptionsFloatItemVariable <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsFloatItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsStringItem optionsStringItem::create (
  string             oahAtomShortName,
  string             oahAtomLongName,
  string             oahAtomDescription,
  string             optionsValueSpecification,
  string             optionsStringItemVariableDisplayName,
  string&            optionsStringItemVariable)
{
  optionsStringItem* o = new
    optionsStringItem (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      optionsValueSpecification,
      optionsStringItemVariableDisplayName,
      optionsStringItemVariable);
  assert(o!=0);
  return o;
}

optionsStringItem::optionsStringItem (
  string             oahAtomShortName,
  string             oahAtomLongName,
  string             oahAtomDescription,
  string             optionsValueSpecification,
  string             optionsStringItemVariableDisplayName,
  string&            optionsStringItemVariable)
  : optionsValuedItem (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      optionsValueSpecification,
      optionsStringItemVariableDisplayName),
    fOptionsStringItemVariable (
      optionsStringItemVariable)
{}

optionsStringItem::~optionsStringItem ()
{}

void optionsStringItem::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsStringItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsItemVariableDisplayName" << " : " <<
    fOptionsItemVariableDisplayName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsStringItemVariable" << " : " <<
    fOptionsStringItemVariable <<
    endl;

  gIndenter--;
}

void optionsStringItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsItemVariableDisplayName <<
    " : \"" <<
    fOptionsStringItemVariable <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsStringItem& elt)
{
  os <<
    "OptionsStringItem:" <<
    endl;
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsRationalItem optionsRationalItem::create (
  string             oahAtomShortName,
  string             oahAtomLongName,
  string             oahAtomDescription,
  string             optionsValueSpecification,
  string             optionsRationalItemVariableDisplayName,
  rational&          optionsRationalItemVariable)
{
  optionsRationalItem* o = new
    optionsRationalItem (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      optionsValueSpecification,
      optionsRationalItemVariableDisplayName,
      optionsRationalItemVariable);
  assert(o!=0);
  return o;
}

optionsRationalItem::optionsRationalItem (
  string             oahAtomShortName,
  string             oahAtomLongName,
  string             oahAtomDescription,
  string             optionsValueSpecification,
  string             optionsRationalItemVariableDisplayName,
  rational&          optionsRationalItemVariable)
  : optionsValuedItem (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      optionsValueSpecification,
      optionsRationalItemVariableDisplayName),
    fOptionsRationalItemVariable (
      optionsRationalItemVariable)
{}

optionsRationalItem::~optionsRationalItem ()
{}

void optionsRationalItem::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsRationalItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsItemVariableDisplayName" << " : " <<
    fOptionsItemVariableDisplayName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsRationalItemVariable" << " : " <<
    fOptionsRationalItemVariable <<
    endl;

  gIndenter--;
}

void optionsRationalItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsItemVariableDisplayName <<
    " : " <<
    fOptionsRationalItemVariable <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsRationalItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsNumbersSetItem optionsNumbersSetItem::create (
  string             oahAtomShortName,
  string             oahAtomLongName,
  string             oahAtomDescription,
  string             optionsValueSpecification,
  string             optionsNumbersSetItemVariableDisplayName,
  set<int>&          optionsNumbersSetItemVariable)
{
  optionsNumbersSetItem* o = new
    optionsNumbersSetItem (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      optionsValueSpecification,
      optionsNumbersSetItemVariableDisplayName,
      optionsNumbersSetItemVariable);
  assert(o!=0);
  return o;
}

optionsNumbersSetItem::optionsNumbersSetItem (
  string             oahAtomShortName,
  string             oahAtomLongName,
  string             oahAtomDescription,
  string             optionsValueSpecification,
  string             optionsNumbersSetItemVariableDisplayName,
  set<int>&          optionsNumbersSetItemVariable)
  : optionsValuedItem (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      optionsValueSpecification,
      optionsNumbersSetItemVariableDisplayName),
    fOptionsNumbersSetItemVariable (
      optionsNumbersSetItemVariable)
{}

optionsNumbersSetItem::~optionsNumbersSetItem ()
{}

void optionsNumbersSetItem::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsNumbersSetItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsItemVariableDisplayName" << " : " <<
    fOptionsItemVariableDisplayName <<
    setw (fieldWidth) <<
    "fOptionsNumbersSetItemVariable" << " : " <<
    endl;

  if (! fOptionsNumbersSetItemVariable.size ()) {
    os <<
      "none";
  }

  else {
    os <<
      "'";

    set<int>::const_iterator
      iBegin = fOptionsNumbersSetItemVariable.begin (),
      iEnd   = fOptionsNumbersSetItemVariable.end (),
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

void optionsNumbersSetItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsItemVariableDisplayName <<
    " : ";

  if (! fOptionsNumbersSetItemVariable.size ()) {
    os <<
      "none";
  }

  else {
    os <<
      "'";

    set<int>::const_iterator
      iBegin = fOptionsNumbersSetItemVariable.begin (),
      iEnd   = fOptionsNumbersSetItemVariable.end (),
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

ostream& operator<< (ostream& os, const S_optionsNumbersSetItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahSubGroup oahSubGroup::create (
  string oahSubGroupHelpHeader,
  string oahSubGroupShortName,
  string oahSubGroupLongName,
  string oahSubGroupDescription,
  oahSubGroupDescriptionVisibilityKind
         oahSubGroupDescriptionVisibilityKind,
  S_oahGroup
         oahGroupUpLink)
{
  oahSubGroup* o = new
    oahSubGroup (
      oahSubGroupHelpHeader,
      oahSubGroupShortName,
      oahSubGroupLongName,
      oahSubGroupDescription,
      oahSubGroupDescriptionVisibilityKind,
      oahGroupUpLink);
  assert(o!=0);
  return o;
}

oahSubGroup::oahSubGroup (
  string oahSubGroupHelpHeader,
  string oahSubGroupShortName,
  string oahSubGroupLongName,
  string oahSubGroupDescription,
  oahSubGroupDescriptionVisibilityKind
         oahSubGroupDescriptionVisibilityKind,
  S_oahGroup
         oahGroupUpLink)
  : oahElement (
      oahSubGroupShortName,
      oahSubGroupLongName,
      oahSubGroupDescription)
{
  fOptionsGroupUpLink =
    oahGroupUpLink;

  fOptionsSubGroupHelpHeader =
    oahSubGroupHelpHeader;

  fOptionsSubGroupDescriptionVisibilityKind =
    oahSubGroupDescriptionVisibilityKind;
}

oahSubGroup::~oahSubGroup ()
{}

string oahSubGroup::oahSubGroupDescriptionVisibilityKindAsString (
  oahSubGroupDescriptionVisibilityKind
    oahSubGroupDescriptionVisibilityKind)
{
  string result;

  switch (oahSubGroupDescriptionVisibilityKind) {
    case kAlwaysShowDescription:
      result = "AlwaysShowDescription";
      break;

    case kHideDescriptionByDefault:
      result = "HideDescriptionByDefault";
      break;
  } // switch

  return result;
}

void oahSubGroup::underlineHeader (ostream& os) const
{
  /* JMI ???
  for (unsigned int i = 0; i < fOptionsSubGroupHelpHeader.size (); i++) {
    os << "-";
  } // for
  os << endl;
  */
  os << "--------------------------" << endl;
}

void oahSubGroup::registerOptionsSubGroupInHandler (
  S_oahHandler oahHandler)
{
  oahHandler->
    registerOptionsElementInHandler (this);

  for (
    list<S_oahAtom>::const_iterator
      i = fOptionsSubGroupItemsList.begin ();
    i != fOptionsSubGroupItemsList.end ();
    i++
  ) {
    // register the options sub group
    (*i)->
      registerOptionsItemInHandler (
        oahHandler);
  } // for
}

void oahSubGroup::appendOptionsItem (
  S_oahAtom oahAtom)
{
  // sanity check
  msrAssert (
    oahAtom != nullptr,
    "oahAtom is null");

  // append options item
  fOptionsSubGroupItemsList.push_back (
    oahAtom);

  // set options item subgroup upLink
  oahAtom->
    setOptionsSubGroupUpLink (this);
}

S_oahElement oahSubGroup::fetchOptionElement (
  string optiontElementName)
{
  S_oahElement result;

  for (
    list<S_oahAtom>::const_iterator
      i = fOptionsSubGroupItemsList.begin ();
    i != fOptionsSubGroupItemsList.end ();
    i++
  ) {
    // search optiontElementName in the options group
    result =
      (*i)->fetchOptionElement (optiontElementName);

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
   "OptionsSubGroup:" <<
    endl;

  gIndenter++;

  oahElement::printElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsSubGroupDescriptionVisibility" << " : " <<
      oahSubGroupDescriptionVisibilityKindAsString (
        fOptionsSubGroupDescriptionVisibilityKind) <<
    endl <<
    endl;

  os <<
    "Options items (" <<
    singularOrPlural (
      fOptionsSubGroupItemsList.size (), "element",  "elements") <<
    "):" <<
    endl;

  if (fOptionsSubGroupItemsList.size ()) {
    os << endl;

    gIndenter++;

    list<S_oahAtom>::const_iterator
      iBegin = fOptionsSubGroupItemsList.begin (),
      iEnd   = fOptionsSubGroupItemsList.end (),
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
    fOptionsSubGroupHelpHeader;

  os <<
    " " <<
    oahElementNamesBetweenParentheses ();

  switch (fOptionsSubGroupDescriptionVisibilityKind) {
    case kAlwaysShowDescription:
      os <<
        ":";
      break;

    case kHideDescriptionByDefault:
      os <<
        " (hidden by default)";
      break;
  } // switch

  os << endl;

  // print the description if any
  if (fOptionsElementDescription.size ()) {
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fOptionsElementDescription);
    gIndenter--;

    os << endl;
  }

  switch (fOptionsSubGroupDescriptionVisibilityKind) {
    case kAlwaysShowDescription:
      if (fOptionsSubGroupItemsList.size ()) {
        gIndenter++;

        list<S_oahAtom>::const_iterator
          iBegin = fOptionsSubGroupItemsList.begin (),
          iEnd   = fOptionsSubGroupItemsList.end (),
          i      = iBegin;
        for ( ; ; ) {
          S_oahAtom
            oahAtom = (*i);
          bool
            oahAtomIsHidden =
              oahAtom->getOptionsElementIsHidden ();

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

    case kHideDescriptionByDefault:
      break;
  } // switch

  // register help print action in options groups's options handler upLink
  fOptionsGroupUpLink->
    getOptionsHandlerUpLink ()->
      setOptionsHandlerFoundAHelpItem ();
}

void oahSubGroup::printOptionsSubGroupForcedHelp (ostream& os) const
{
  // print the header and option names
  os <<
    fOptionsSubGroupHelpHeader;

  os <<
    " " <<
    oahElementNamesBetweenParentheses ();

  switch (fOptionsSubGroupDescriptionVisibilityKind) {
    case kAlwaysShowDescription:
      os <<
        ":";
      break;

    case kHideDescriptionByDefault:
      os <<
        " (hidden by default) :";
      break;
  } // switch

  os << endl;

  // print the description if any
  if (fOptionsElementDescription.size ()) {
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fOptionsElementDescription) <<
      endl;
    gIndenter--;

    os << endl;
  }

  if (fOptionsSubGroupItemsList.size ()) {
    gIndenter++;

    list<S_oahAtom>::const_iterator
      iBegin = fOptionsSubGroupItemsList.begin (),
      iEnd   = fOptionsSubGroupItemsList.end (),
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
  fOptionsGroupUpLink->
    getOptionsHandlerUpLink ()->
      setOptionsHandlerFoundAHelpItem ();
}

void oahSubGroup::printHelpSummary (
  ostream& os) const
{
  // fetch maximum subgroups help headers size
  int maximumSubGroupsHelpHeadersSize =
    getOptionsGroupUpLink ()->
      getOptionsHandlerUpLink ()->
        getMaximumSubGroupsHelpHeadersSize ();

  // fetch maximum short name width
  int maximumShortNameWidth =
    getOptionsGroupUpLink ()->
      getOptionsHandlerUpLink ()->
        getMaximumShortNameWidth ();

  // print the summary
  os << left <<
    setw (maximumSubGroupsHelpHeadersSize) <<
    fOptionsSubGroupHelpHeader <<
    " " <<
    oahElementNamesInColumnsBetweenParentheses (
      maximumShortNameWidth);

  switch (fOptionsSubGroupDescriptionVisibilityKind) {
    case kAlwaysShowDescription:
      break;

    case kHideDescriptionByDefault:
      os <<
        " (hidden by default)";
      break;
  } // switch

  os << endl;

  // print the description if any
  if (fOptionsElementDescription.size ()) {
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fOptionsElementDescription) <<
      endl;
    gIndenter--;
  }

  // register help print action in options groups's options handler upLink
  fOptionsGroupUpLink->
    getOptionsHandlerUpLink ()->
      setOptionsHandlerFoundAHelpItem ();
}

void oahSubGroup::printSpecificSubGroupHelp (
  ostream& os,
  S_oahSubGroup
           oahSubGroup) const
{
  // print only the summary if this is not the desired subgroup,
  // otherwise print the regular help
  if (oahSubGroup == this) {
    os << endl;
    printSpecificSubGroupHelp (
      os, oahSubGroup);
  }
  else {
    printHelpSummary (os);
  }
 }

void oahSubGroup::printOptionsItemForcedHelp (
  ostream&         os,
  S_oahAtom targetOptionsItem) const
{
  // print the header
  os <<
    fOptionsSubGroupHelpHeader <<
    " " <<
    oahElementNamesBetweenParentheses () <<
    ":" <<
    endl <<
    endl;

  // underline the options subgroup header
// JMI  underlineHeader (os);

  // print the options items
  if (fOptionsSubGroupItemsList.size ()) {
    gIndenter++;

    list<S_oahAtom>::const_iterator
      iBegin = fOptionsSubGroupItemsList.begin (),
      iEnd   = fOptionsSubGroupItemsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahAtom
        oahAtom = (*i);

      if (oahAtom == targetOptionsItem) {
        // print the target options item's help
        // target options item's help
        (*i)->
          printHelp (
            os);
      }
      if (++i == iEnd) break;
      if (oahAtom == targetOptionsItem) {
        os << endl;
      }
    } // for

    gIndenter--;
  }

  // register help print action in options groups's options handler upLink
  fOptionsGroupUpLink->
    getOptionsHandlerUpLink ()->
      setOptionsHandlerFoundAHelpItem ();
}

void oahSubGroup::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // print the header
  os <<
    fOptionsSubGroupHelpHeader <<
    " " <<
    oahElementNamesBetweenParentheses () <<
    ":" <<
    endl;

  // underline the options subgroup header
// JMI  underlineHeader (os);

  // print the options items values
  if (fOptionsSubGroupItemsList.size ()) {
    gIndenter++;

    list<S_oahAtom>::const_iterator
      iBegin = fOptionsSubGroupItemsList.begin (),
      iEnd   = fOptionsSubGroupItemsList.end (),
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
  fOptionsHandlerUpLink = oahHandlerUpLink;

  fOptionsGroupHelpHeader = oahGroupHelpHeader;
}

oahGroup::~oahGroup ()
{}

void oahGroup::setOptionsHandlerUpLink (
  S_oahHandler oahHandler)
{
  // sanity check
  msrAssert (
    oahHandler != nullptr,
    "oahHandler is null");

  // set the upLink
  fOptionsHandlerUpLink =
    oahHandler;
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
  S_oahHandler oahHandler)
{
  // sanity check
  msrAssert (
    oahHandler != nullptr,
    "oahHandler is null");

  // set options handler upLink
  setOptionsHandlerUpLink (oahHandler);

  // register options group in options handler
  oahHandler->
    registerOptionsElementInHandler (this);

  for (
    list<S_oahSubGroup>::const_iterator
      i = fOptionsGroupSubGroupsList.begin ();
    i != fOptionsGroupSubGroupsList.end ();
    i++
  ) {
    // register the options sub group
    (*i)->
      registerOptionsSubGroupInHandler (
        oahHandler);
  } // for
}

void  oahGroup::appendOptionsSubGroup (
  S_oahSubGroup oahSubGroup)
{
  // sanity check
  msrAssert (
    oahSubGroup != nullptr,
    "oahSubGroup is null");

  // append options subgroup
  fOptionsGroupSubGroupsList.push_back (
    oahSubGroup);

  // set options subgroup group upLink
  oahSubGroup->
    setOptionsGroupUpLink (this);
}

S_oahElement oahGroup::fetchOptionElement (
  string optiontElementName)
{
  S_oahElement result;

  for (
    list<S_oahSubGroup>::const_iterator
      i = fOptionsGroupSubGroupsList.begin ();
    i != fOptionsGroupSubGroupsList.end ();
    i++
  ) {
    // search optiontElementName in the options group
    result =
      (*i)->fetchOptionElement (optiontElementName);

    if (result != 0) {
      break;
    }
  } // for

  return result;
}

S_optionsValuedItem oahGroup::handleOptionsItem (
  ostream&      os,
  S_oahAtom item)
{
  S_optionsValuedItem result;

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
    oahElementNamesBetweenParentheses () <<
    ":" <<
    endl;

  // print the description if any
  if (fOptionsElementDescription.size ()) {
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fOptionsElementDescription) <<
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
  fOptionsHandlerUpLink->setOptionsHandlerFoundAHelpItem ();
}

void oahGroup::printOptionsSubGroupForcedHelp (
  ostream&             os,
  S_oahSubGroup targetOptionsSubGroup) const
{
  // print the header and option names
  os <<
    fOptionsGroupHelpHeader <<
    " " <<
    oahElementNamesBetweenParentheses () <<
    ":" <<
    endl;

  // print the description if any
  if (fOptionsElementDescription.size ()) {
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fOptionsElementDescription) <<
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
        oahSubGroup = (*i);

      if (oahSubGroup == targetOptionsSubGroup) {
        // print the target options subgroup help
        oahSubGroup->
          printOptionsSubGroupForcedHelp (
            os);
      }
      if (++i == iEnd) break;
      if (oahSubGroup == targetOptionsSubGroup) {
        os << endl;
      }
    } // for

    gIndenter--;
  }
}

void oahGroup::printOptionsItemForcedHelp (
  ostream&             os,
  S_oahSubGroup targetOptionsSubGroup,
  S_oahAtom     targetOptionsItem) const
{
  // print the header and option names
  os <<
    fOptionsGroupHelpHeader <<
    " " <<
    oahElementNamesBetweenParentheses () <<
    ":" <<
    endl;

  // print the description if any
  if (fOptionsElementDescription.size ()) {
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fOptionsElementDescription) <<
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
        oahSubGroup = (*i);

      if (oahSubGroup == targetOptionsSubGroup) {
        // print the target options subgroup's
        // target options item's help
        (*i)->
          printOptionsItemForcedHelp (
            os,
            targetOptionsItem);
      }
      if (++i == iEnd) break;
      if (oahSubGroup == targetOptionsSubGroup) {
        os << endl;
      }
    } // for

    os << endl;

    gIndenter--;
  }

  // register help print action in options handler upLink
  fOptionsHandlerUpLink->setOptionsHandlerFoundAHelpItem ();
}

void oahGroup::printHelpSummary (ostream& os) const
{
  // the description is the header of the information
  os <<
    fOptionsGroupHelpHeader <<
    " " <<
    oahElementNamesBetweenParentheses () <<
    ":" <<
    endl;

  // print the description if any
  if (fOptionsElementDescription.size ()) {
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fOptionsElementDescription) <<
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
        printHelpSummary (os);
      if (++i == iEnd) break;
 //     os << endl;
    } // for

    gIndenter--;
  }

  // register help print action in options handler upLink
  fOptionsHandlerUpLink->setOptionsHandlerFoundAHelpItem ();
}

void oahGroup::printSpecificSubGroupHelp (
  ostream&          os,
  S_oahSubGroup oahSubGroup) const
{
  // the description is the header of the information
  os <<
    fOptionsGroupHelpHeader <<
    " " <<
    oahElementNamesBetweenParentheses () <<
    ":" <<
    endl;

  // print the description if any
  if (fOptionsElementDescription.size ()) {
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fOptionsElementDescription) <<
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
          os, oahSubGroup);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }

  // register help print action in options handler upLink
  fOptionsHandlerUpLink->setOptionsHandlerFoundAHelpItem ();
}

void oahGroup::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // print the header
  os <<
    fOptionsGroupHelpHeader <<
    " " <<
    oahElementNamesBetweenParentheses () <<
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
S_optionsPrefix optionsPrefix::create (
  string optionsPrefixName,
  string optionsPrefixErsatz,
  string optionsPrefixDescription)
{
  optionsPrefix* o = new
    optionsPrefix (
      optionsPrefixName,
      optionsPrefixErsatz,
      optionsPrefixDescription);
  assert(o!=0);
  return o;
}

optionsPrefix::optionsPrefix (
  string optionsPrefixName,
  string optionsPrefixErsatz,
  string optionsPrefixDescription)
{
  fOptionsPrefixName        = optionsPrefixName;
  fOptionsPrefixErsatz      = optionsPrefixErsatz;
  fOptionsPrefixDescription = optionsPrefixDescription;
}

optionsPrefix::~optionsPrefix ()
{}

/* JMI
S_optionsPrefix optionsPrefix::fetchOptionElement (
  string optiontElementName)
{
  S_optionsPrefix result;

  if (optiontElementName == fOptionsPrefixName) {
    result = this;
  }

  return result;
}
*/

string optionsPrefix::optionsPrefixNames () const
{
  stringstream s;

  if (fOptionsPrefixName.size ()) {
      s <<
        "-" << fOptionsPrefixName;
  }

  return s.str ();
}

string optionsPrefix::optionsPrefixNamesInColumns (
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

string optionsPrefix::optionsPrefixNamesBetweenParentheses () const
{
  stringstream s;

  s <<
    "(" <<
    optionsPrefixNames () <<
    ")";

  return s.str ();
}

string optionsPrefix::optionsPrefixNamesInColumnsBetweenParentheses (
  int subGroupsShortNameFieldWidth) const
{
  stringstream s;

  s <<
    "(" <<
    optionsPrefixNamesInColumns (
      subGroupsShortNameFieldWidth) <<
    ")";

  return s.str ();
}

void optionsPrefix::printHeader (ostream& os) const
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

void optionsPrefix::printElementEssentials (
  ostream& os,
  int      fieldWidth) const
{
  os << left <<
    setw (fieldWidth) <<
    "optionsPrefixName" << " : " <<
    fOptionsPrefixName <<
    endl <<
    setw (fieldWidth) <<
    "optionsPrefixErsatz" << " : " <<
    fOptionsPrefixErsatz <<
    endl <<
    setw (fieldWidth) <<
    "optionsPrefixDescription" << " : " <<
    fOptionsPrefixDescription <<
    endl;
}

void optionsPrefix::print (ostream& os) const
{
  os <<
    "??? optionsPrefix ???" <<
    endl;

  printElementEssentials (os, 40); // JMI
}

void optionsPrefix::printHelp (ostream& os) const
{
  os <<
    optionsPrefixNames () <<
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
//  fOptionsHandlerUpLink->setOptionsHandlerFoundAHelpItem ();
}

ostream& operator<< (ostream& os, const S_optionsPrefix& elt)
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
    fOptionsHandlerlogIOstream (
      oahHandlerlogIOstream)
{
  fOptionsHandlerHelpHeader =
    oahHandlerHelpHeader;

  fOptionsHandlerValuesHeader =
    oahHandlerValuesHeader;

  fOptionHandlerHelpSummaryShortName =
    optionHandlerHelpSummaryShortName;
  fOptionHandlerHelpSummaryLongName =
    optionHandlerHelpSummaryLongName;

  fOptionHandlerPreamble =
    optionHandlerPreamble;

  fMaximumSubGroupsHelpHeadersSize = 1;

  fMaximumShortNameWidth   = 1;
  fMaximumLongNameWidth    = 1;

  fMaximumVariableDisplayNameWidth = 0;

  fOptionsHandlerFoundAHelpItem = false;
}

oahHandler::~oahHandler ()
{}

void oahHandler::registerOptionsHandlerInItself ()
{
  this->
    registerOptionsElementInHandler (this);

/* JMI ???
  // register the help summary names in handler
  registerOptionsNamesInHandler (
    fOptionHandlerHelpSummaryShortName,
    fOptionHandlerHelpSummaryLongName,
    this);
*/

//* JMI
  for (
    list<S_oahGroup>::const_iterator
      i = fOptionsHandlerOptionsGroupsList.begin ();
    i != fOptionsHandlerOptionsGroupsList.end ();
    i++
  ) {
    // register the options group
    (*i)->
      registerOptionsGroupInHandler (
        this);
  } // for
 // */
}

S_optionsPrefix oahHandler::fetchOptionsPrefixFromMap (
  string oahElementName) const
{
  S_optionsPrefix result;

  // is oahAtomName known in options elements map?
  map<string, S_optionsPrefix>::const_iterator
    it =
      fOptionsPrefixesMap.find (
        oahElementName);

  if (it != fOptionsPrefixesMap.end ()) {
    // yes, oahAtomName is known in the map
    result = (*it).second;
  }

  return result;
}

S_oahElement oahHandler::fetchOptionsElementFromMap (
  string oahElementName) const
{
  S_oahElement result;

  // is oahAtomName known in options elements map?
  map<string, S_oahElement>::const_iterator
    it =
      fOptionsElementsMap.find (
        oahElementName);

  if (it != fOptionsElementsMap.end ()) {
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
    oahElementNames () <<
    ", ";

  if (
    fOptionHandlerHelpSummaryShortName.size ()
        &&
    fOptionHandlerHelpSummaryLongName.size ()
    ) {
      s <<
        "-" << fOptionHandlerHelpSummaryShortName <<
        ", " <<
        "-" << fOptionHandlerHelpSummaryLongName;
  }

  else {
    if (fOptionHandlerHelpSummaryShortName.size ()) {
      s <<
      "-" << fOptionHandlerHelpSummaryShortName;
    }
    if (fOptionHandlerHelpSummaryLongName.size ()) {
      s <<
      "-" << fOptionHandlerHelpSummaryLongName;
    }
  }

  s <<
    ")";

  return s.str ();
}

void oahHandler::registerOptionsNamesInHandler (
  string           optionShortName,
  string           optionLongName,
  S_oahElement oahElement)
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
      fOptionsElementsMap.begin ();
    i != fOptionsElementsMap.end ();
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

  // register oahElement's names
  if (optionLongNameSize) {
    fOptionsElementsMap [optionLongName] =
      oahElement;

    if (optionLongNameSize > fMaximumLongNameWidth) {
      fMaximumLongNameWidth = optionLongNameSize;
    }
  }

  if (optionShortNameSize) {
    fOptionsElementsMap [optionShortName] =
      oahElement;

    if (optionShortNameSize > fMaximumShortNameWidth) {
      fMaximumShortNameWidth = optionShortNameSize;
    }
  }

  // take oahElement's display variable name length into account
  int
    oahElementVariableDisplayNameLength =
      oahElement->
        fetchOptionsElementVariableDisplayNameLength ();

    if (
      oahElementVariableDisplayNameLength
        >
      fMaximumVariableDisplayNameWidth
    ) {
      fMaximumVariableDisplayNameWidth =
        oahElementVariableDisplayNameLength;
    }
}

void oahHandler::registerOptionsElementInHandler (
  S_oahElement oahElement)
{
  string
    optionShortName =
      oahElement->getOptionsElementShortName (),
    optionLongName =
      oahElement->getOptionsElementLongName ();

  // register the option names in handler
  registerOptionsNamesInHandler (
    optionShortName,
    optionLongName,
    oahElement);

  if (
    // options subgroup?
    S_oahSubGroup
      subGroup =
        dynamic_cast<oahSubGroup*>(&(*oahElement))
    ) {

    string
      optionHelpHeader=
        subGroup->
          getOptionsSubGroupHelpHeader ();
    int
      optionHelpHeaderSize =
        optionHelpHeader.size ();

    // account for oahSubGroup's header size
    if (optionHelpHeaderSize > fMaximumSubGroupsHelpHeadersSize) {
      fMaximumSubGroupsHelpHeadersSize =
        optionHelpHeaderSize;
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
    "fOptionHandlerHelpSummaryShortName" << " : " <<
    fOptionHandlerHelpSummaryShortName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionHandlerHelpSummaryLongName" << " : " <<
    fOptionHandlerHelpSummaryLongName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsElementShortName" << " : " <<
    fOptionsElementShortName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsElementLongName" << " : " <<
    fOptionsElementLongName <<
    endl <<
    setw (fieldWidth) <<
    "oahHandlerFoundAHelpItem" << " : " <<
    fOptionsHandlerFoundAHelpItem <<
    endl;

  // print the options prefixes if any
  if (fOptionsPrefixesMap.size ()) {
    printKnownOptionsPrefixes ();
  }

  // print the options groups if any
  if (fOptionsHandlerOptionsGroupsList.size ()) {
    os << endl;

    gIndenter++;

    list<S_oahGroup>::const_iterator
      iBegin = fOptionsHandlerOptionsGroupsList.begin (),
      iEnd   = fOptionsHandlerOptionsGroupsList.end (),
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
      fOptionHandlerPreamble);

  os << endl;

  // print the options handler help header and element names
  os <<
    fOptionsHandlerHelpHeader <<
    " " <<
    oahElementNamesBetweenParentheses () <<
    ":" <<
    endl;

  // print the options handler description
  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fOptionsElementDescription);
  gIndenter--;

  os <<
    endl <<
    endl;

  // print the known options prefixes
  gIndenter++;
  printKnownOptionsPrefixes ();
  gIndenter--;

  os << endl;

  // print the options groups helps
  if (fOptionsHandlerOptionsGroupsList.size ()) {
    gIndenter++;

    list<S_oahGroup>::const_iterator
      iBegin = fOptionsHandlerOptionsGroupsList.begin (),
      iEnd   = fOptionsHandlerOptionsGroupsList.end (),
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
//  fOptionsHandlerUpLink->setOptionsHandlerFoundAHelpItem ();
}

void oahHandler::printHelpSummary (ostream& os) const
{
  // print the options handler preamble
  os <<
    gIndenter.indentMultiLineString (
      fOptionHandlerPreamble);

  // print the options handler help header and element names
  os <<
    fOptionsHandlerHelpHeader <<
    " " <<
    oahElementNamesBetweenParentheses () <<
    ":" <<
    endl;

  // print the options handler description
  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;
  gIndenter--;

  // print the options groups help summaries
  if (fOptionsHandlerOptionsGroupsList.size ()) {
    gIndenter++;

    list<S_oahGroup>::const_iterator
      iBegin = fOptionsHandlerOptionsGroupsList.begin (),
      iEnd   = fOptionsHandlerOptionsGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group summary
      (*i)->printHelpSummary (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }
}

void oahHandler::printSpecificSubGroupHelp (
  ostream& os,
  S_oahSubGroup
           oahSubGroup) const
{
  // print the options handler help header and element names
  os <<
    fOptionsHandlerHelpHeader <<
    " " <<
    oahElementNamesBetweenParentheses () <<
    ":" <<
    endl;

  // print the optons group subgroups specific help
  if (fOptionsHandlerOptionsGroupsList.size ()) {
    gIndenter++;

    list<S_oahGroup>::const_iterator
      iBegin = fOptionsHandlerOptionsGroupsList.begin (),
      iEnd   = fOptionsHandlerOptionsGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group specific subgroup help
      (*i)->
        printSpecificSubGroupHelp (
          os,
          oahSubGroup);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }
}

void oahHandler::printSpecificItemHelp (
  ostream& os,
  string   oahAtomName) const
{
  // is oahAtomName known in options elements map?
  S_oahElement
    oahElement =
      fetchOptionsElementFromMap (
        oahAtomName);

  if (! oahElement) {
    // oahAtomName is is not well handled by this options handler
    stringstream s;

    s <<
      "option name '" << oahAtomName <<
      "' is unknown, cannot deliver item specific help";

    optionError (s.str ());
    exit (33);
  }

  else {
    // oahAtomName is known, let's handle it
    if (
      // options handler?
      S_oahHandler
        handler =
          dynamic_cast<oahHandler*>(&(*oahElement))
      ) {
      // print the option handler help or help summary
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
          printHelpSummary (
            fOptionsHandlerlogIOstream);
      }
      else {
        handler->
          printHelp (
            fOptionsHandlerlogIOstream);
      }

      fOptionsHandlerlogIOstream << endl;
    }

    else if (
      // options group?
      S_oahGroup
        group =
          dynamic_cast<oahGroup*>(&(*oahElement))
      ) {
      // print the help
      fOptionsHandlerlogIOstream <<
        endl <<
        "--- Help for options item name '" <<
        oahAtomName <<
        "' for group \"" <<
        group->
          getOptionsGroupHelpHeader () <<
        "\" ---" <<
        endl <<
        endl;

      group->
        printHelp (
          fOptionsHandlerlogIOstream);

      fOptionsHandlerlogIOstream << endl;
    }

    else if (
      // options subgroup?
      S_oahSubGroup
        subGroup =
          dynamic_cast<oahSubGroup*>(&(*oahElement))
      ) {
      // get the options group upLink
      S_oahGroup
        group =
          subGroup->
            getOptionsGroupUpLink ();

      // print the help
      fOptionsHandlerlogIOstream <<
        endl <<
        "--- Help for options item name '" <<
        oahAtomName <<
        "' for subgroup \"" <<
        subGroup->
          getOptionsSubGroupHelpHeader () <<
        "\"" <<
        " in group \"" <<
        group->
          getOptionsGroupHelpHeader () <<
        "\" ---" <<
        endl <<
        endl;

      group->
        printOptionsSubGroupForcedHelp (
          fOptionsHandlerlogIOstream,
          subGroup);
    }

    else if (
      // options item?
      S_oahAtom
        item =
          dynamic_cast<oahAtom*>(&(*oahElement))
      ) {
      // get the options subgroup upLink
      S_oahSubGroup
        subGroup =
          item->
            getOptionsSubGroupUpLink ();

      // get the options group upLink
      S_oahGroup
        group =
          subGroup->
            getOptionsGroupUpLink ();

      // print the help
      fOptionsHandlerlogIOstream <<
        endl <<
        "--- Help for options item name '" <<
        oahAtomName <<
        "' in subgroup \"" <<
        subGroup->
          getOptionsSubGroupHelpHeader () <<
        "\"" <<
        " of group \"" <<
        group->
          getOptionsGroupHelpHeader () <<
        "\" ---" <<
        endl <<
        endl;

      group->
        printOptionsItemForcedHelp (
          fOptionsHandlerlogIOstream,
          subGroup,
          item);
    }

    else {
      stringstream s;

      s <<
        "cannot handle specific help about oahAtomName \"" <<
        oahAtomName <<
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
    fOptionsHandlerValuesHeader <<
    " " <<
    oahElementNamesBetweenParentheses () <<
    ":" <<
    endl;

  // print the options groups values
  if (fOptionsHandlerOptionsGroupsList.size ()) {
    os << endl;

    gIndenter++;

    list<S_oahGroup>::const_iterator
      iBegin = fOptionsHandlerOptionsGroupsList.begin (),
      iEnd   = fOptionsHandlerOptionsGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group values
      (*i)->
        printOptionsValues (
          os, fMaximumVariableDisplayNameWidth);
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

void oahHandler::appendOptionsPrefixToHandler (
  S_optionsPrefix prefix)
{
  // sanity check
  msrAssert (
    prefix != nullptr,
    "prefix is null");

  string prefixName = prefix->getOptionsPrefixName ();

  S_optionsPrefix result;

  // is oahAtomName already known in options elements map?
  map<string, S_optionsPrefix>::const_iterator
    it =
      fOptionsPrefixesMap.find (
        prefixName);

  if (it != fOptionsPrefixesMap.end ()) {
    // prefixName is already known in the map
    stringstream s;

    s <<
      "option prefix name '" << prefixName <<
      "' is already known";

    optionError (s.str ());
    exit (7);
  }

  // register prefix in the options prefixes map
  fOptionsPrefixesMap [
    prefix->getOptionsPrefixName ()
    ] = prefix;
}

void oahHandler::appendOptionsGroupToHandler (
  S_oahGroup oahGroup)
{
  // sanity check
  msrAssert (
    oahGroup != nullptr,
    "oahGroup is null");

  // append the options group
  fOptionsHandlerOptionsGroupsList.push_back (
    oahGroup);

  // set the upLink
  oahGroup->
    setOptionsHandlerUpLink (this);
}

void oahHandler::printKnownOptionsPrefixes () const
{
  int oahHandlerOptionsPrefixesListSize =
    fOptionsPrefixesMap.size ();

  fOptionsHandlerlogIOstream <<
    "There are " <<
    oahHandlerOptionsPrefixesListSize <<
    " options prefixes:" <<
    endl;

  gIndenter++;

  if (oahHandlerOptionsPrefixesListSize) {
    // indent a bit more for readability
    for (
      map<string, S_optionsPrefix>::const_iterator i =
        fOptionsPrefixesMap.begin ();
      i != fOptionsPrefixesMap.end ();
      i++
    ) {
      S_optionsPrefix
        prefix = (*i).second;

      prefix->
        printHeader (
          fOptionsHandlerlogIOstream);
    } // for
  }
  else {
    fOptionsHandlerlogIOstream <<
      "none" <<
      endl;
  }

  gIndenter--;
}

void oahHandler::printKnownOptionsElements () const
{
  int oahElementsMapSize =
    fOptionsElementsMap.size ();

  // print the options elements map
  fOptionsHandlerlogIOstream <<
    "The " <<
    oahElementsMapSize <<
    " known options elements are:" <<
    endl;

  gIndenter++;

  if (oahElementsMapSize) {
    map<string, S_oahElement>::const_iterator
      iBegin = fOptionsElementsMap.begin (),
      iEnd   = fOptionsElementsMap.end (),
      i      = iBegin;
    for ( ; ; ) {
      fOptionsHandlerlogIOstream <<
        (*i).first << "-->" <<
        endl;

      gIndenter++;

      (*i).second->
        printHeader (
          fOptionsHandlerlogIOstream);

      if (++i == iEnd) break;

      gIndenter--;
    } // for
  }
  else {
    fOptionsHandlerlogIOstream <<
      "none" <<
      endl;
  }

  gIndenter--;
}

S_oahElement oahHandler::fetchOptionElement (
  string optiontElementName)
{
  S_oahElement result;

  for (
    list<S_oahGroup>::const_iterator
      i = fOptionsHandlerOptionsGroupsList.begin ();
    i != fOptionsHandlerOptionsGroupsList.end ();
    i++
  ) {
    // search optiontElementName in the options group
    result =
      (*i)->fetchOptionElement (optiontElementName);

    if (result != 0) {
      break;
    }
  } // for

  return result;
}

void oahHandler::checkMissingPendingOptionsValuedItemValue (
  string context)
{
  if (fPendingOptionsValuedItem) {
    if (fPendingOptionsValuedItem->getOptionsValueIsOptional ()) {
      // handle the valued item using the default value JMI
    }
    else {
      // an option requiring a value is expecting it,
      // but another option, an argument or the end of the command line
      // has been found instead
      stringstream s;

      s <<
        "option " <<
       fPendingOptionsValuedItem->asString () <<
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
      fOptionsHandlerlogIOstream <<
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
          fOptionsHandlerlogIOstream <<
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
        fOptionsHandlerlogIOstream <<
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
              fOptionsHandlerlogIOstream <<
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
              fOptionsHandlerlogIOstream <<
                "'" << currentOptionName << "' is a single-dashed option" <<
                endl;
            }
#endif
          }
        }

        else {
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
            fOptionsHandlerlogIOstream <<
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
            printKnownOptionsPrefixes ();
          }
#endif

          // fetch the prefix name and the string after the equals sign
          string prefixName =
            currentOptionName.substr (0, equalsSignPosition);
          string stringAfterEqualsSign =
            currentOptionName.substr (equalsSignPosition + 1);


#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
            fOptionsHandlerlogIOstream <<
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
            fOptionsHandlerlogIOstream <<
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
              fOptionsHandlerlogIOstream <<
                "[" << (*i) << "]";
              if (++i == iEnd) break;
              fOptionsHandlerlogIOstream <<
                " ";
            } // for

            fOptionsHandlerlogIOstream << endl;

            gIndenter--;
          }
#endif

          S_optionsPrefix
            prefix =
              fetchOptionsPrefixFromMap (prefixName);

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
                  fOptionsHandlerlogIOstream <<
                    "Expanding option '" << singleOptionName <<
                    "' to '" << uncontractedOptionName <<
                    "'" <<
                    endl;
                }
#endif

                // handle the uncontracted option item name
                handleOptionsItemName (uncontractedOptionName);
              } // for
            }
          }

          else {
            printKnownOptionsPrefixes ();

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
          handleOptionsItemName (currentOptionName);
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
  checkMissingPendingOptionsValuedItemValue (
    "last option in command line");

  unsigned int argumentsVectorSize =
    fArgumentsVector.size ();

#ifdef TRACE_OPTIONS
  // display arc and argv only now, to wait for the options to have been handled
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fOptionsHandlerlogIOstream <<
      "argc: " << argc <<
      endl <<
      "argv:" <<
      endl;

    gIndenter++;

    for (int i = 0; i < argc; i++) {
      fOptionsHandlerlogIOstream <<
        "argv [" << i << "]: " << argv [i] <<
        endl;
    } // for

    gIndenter--;
  }
#endif

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptionsDetails && ! gGeneralOptions->fQuiet) {
    printKnownOptionsPrefixes ();
    printKnownOptionsElements ();
  }
#endif

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    // print the arguments vector
    fOptionsHandlerlogIOstream <<
      "Arguments vector (" <<
      argumentsVectorSize <<
      " elements):" <<
      endl;

    if (argumentsVectorSize) {
      gIndenter++;
      for (unsigned int i = 0; i < argumentsVectorSize; i++) {
        fOptionsHandlerlogIOstream <<
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
    fOptionsHandlerlogIOstream <<
      "==> fOptionsHandlerFoundAHelpItem: " <<
      booleanAsString (fOptionsHandlerFoundAHelpItem) <<
      endl;
  }
#endif
  if (fOptionsHandlerFoundAHelpItem) {
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
  string           oahAtomName)
{
  // print the option handler help or help summary
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fOptionsHandlerlogIOstream <<
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
      printHelpSummary (
        fOptionsHandlerlogIOstream);
  }
  else {
    handler->
      printHelp (
        fOptionsHandlerlogIOstream);
  }

  fOptionsHandlerlogIOstream << endl;
}

void oahHandler::handleOptionsGroupItemName (
  S_oahGroup group,
  string         oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fOptionsHandlerlogIOstream <<
      "==> element is of type 'oahGroup'" <<
      endl;
  }
#endif

  // print the help
  fOptionsHandlerlogIOstream <<
    endl <<
    "--- Help for group \"" <<
    group->
      getOptionsGroupHelpHeader () <<
    "\" ---" <<
    endl <<
    endl;

  group->
    printHelp (
      fOptionsHandlerlogIOstream);
}

void oahHandler::handleOptionsSubGroupItemName (
  S_oahSubGroup subGroup,
  string            oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fOptionsHandlerlogIOstream <<
      "==> element is of type 'oahSubGroup'" <<
      endl;
  }
#endif

  // get the options group upLink
  S_oahGroup
    group =
      subGroup->
        getOptionsGroupUpLink ();

  // print the help
  fOptionsHandlerlogIOstream <<
    endl <<
    "--- Help for subgroup \"" <<
    subGroup->
      getOptionsSubGroupHelpHeader () <<
    "\"" <<
    " in group \"" <<
    group->
      getOptionsGroupHelpHeader () <<
    "\" ---" <<
    endl <<
    endl;

  group->
    printOptionsSubGroupForcedHelp (
      fOptionsHandlerlogIOstream,
      subGroup);
}

void oahHandler::handleOptionsHelpUsageItemName (
  S_oahHelpUsageAtom helpUsageItem,
  string                 oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fOptionsHandlerlogIOstream <<
      "==> element is of type 'oahHelpUsageAtom'" <<
      endl;
  }
#endif

  // handle it at once
  helpUsageItem->
    printHelpUsage (
      fOptionsHandlerlogIOstream);

  // exit
  exit (0);
}

void oahHandler::handleOptionsHelpSummaryItemName (
  S_oahHelpSummaryAtom helpSummaryItem,
  string                   oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fOptionsHandlerlogIOstream <<
      "==> element is of type 'oahHelpSummaryAtom'" <<
      endl;
  }
#endif

  // handle it at once
  printHelpSummary (
    fOptionsHandlerlogIOstream);

  // exit
  exit (0);
}

void oahHandler::handleOptionsCombinedBooleanItemsItemName (
  S_oahCombinedBooleansAtom combinedBooleanItemsItem,
  string                     oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fOptionsHandlerlogIOstream <<
      "==> element is of type 'oahCombinedBooleansAtom'" <<
      endl;
  }
#endif

  // handle it at once
  combinedBooleanItemsItem->
    setCombinedBooleanItemsVariablesValue (true);
}

void oahHandler::handleOptionsBooleanItemItemName (
  S_oahBooleanAtom booleanItem,
  string               oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fOptionsHandlerlogIOstream <<
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
    fOptionsHandlerlogIOstream <<
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
    fOptionsHandlerlogIOstream <<
      "==> element is of type 'oahThreeBooleansAtom'" <<
      endl;
  }
#endif

  // handle it at once
  threeBooleansItem->
    setThreeBooleansItemVariableValue (true);
}

void oahHandler::handleOptionsItemHelpItemName (
  S_oahAtomHelpItem itemHelpItem,
  string                oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fOptionsHandlerlogIOstream <<
      "==> element is of type 'oahAtomHelpItem'" <<
      endl;
  }
#endif

  // wait until the value is met
  fPendingOptionsValuedItem = itemHelpItem;
}

void oahHandler::handleOptionsIntegerItemItemName (
  S_optionsIntegerItem integerItem,
  string               oahAtomName)
{
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
      fOptionsHandlerlogIOstream <<
        "==> element is of type 'optionsIntegerItem'" <<
        endl;
    }
#endif

    // wait until the value is met
    fPendingOptionsValuedItem = integerItem;
}

void oahHandler::handleOptionsFloatItemItemName (
  S_optionsFloatItem floatItem,
  string             oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fOptionsHandlerlogIOstream <<
      "==> element is of type 'optionsFloatItem'" <<
      endl;
  }
#endif

  // wait until the value is met
  fPendingOptionsValuedItem = floatItem;
}

void oahHandler::handleOptionsStringItemItemName (
  S_optionsStringItem stringItem,
  string              oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fOptionsHandlerlogIOstream <<
      "==> element is of type 'optionsStringItem'" <<
      endl;
  }
#endif

  // wait until the value is met
  fPendingOptionsValuedItem = stringItem;
}

void oahHandler::handleOptionsRationalItemItemName (
  S_optionsRationalItem rationalItem,
  string              oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fOptionsHandlerlogIOstream <<
      "==> element is of type 'optionsRationalItem'" <<
      endl;
  }
#endif

  // wait until the value is met
  fPendingOptionsValuedItem = rationalItem;
}

void oahHandler::handleOptionsNumbersSetItemItemName (
  S_optionsNumbersSetItem numbersSetItem,
  string                  oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fOptionsHandlerlogIOstream <<
      "==> element is of type 'optionsNumbersSetItem'" <<
      endl;
  }
#endif

  // wait until the value is met
  fPendingOptionsValuedItem = numbersSetItem;
}

void oahHandler::handleOptionsItemItemName (
  S_oahAtom item,
  string        oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fOptionsHandlerlogIOstream <<
      "==> item is of type 'oahAtom'" <<
      endl;
  }
#endif

  // is a pending options valued item value missing?
  string context =
    "before option " + item->asString ();

  checkMissingPendingOptionsValuedItemValue (
    context);

  if (
    // help usage item?
    S_oahHelpUsageAtom
      helpUsageItem =
        dynamic_cast<oahHelpUsageAtom*>(&(*item))
  ) {
    handleOptionsHelpUsageItemName (
      helpUsageItem,
      oahAtomName);
  }

  else if (
    // help summary item?
    S_oahHelpSummaryAtom
      helpSummaryItem =
        dynamic_cast<oahHelpSummaryAtom*>(&(*item))
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
    S_oahAtomHelpItem
      itemHelpItem =
        dynamic_cast<oahAtomHelpItem*>(&(*item))
  ) {
    handleOptionsItemHelpItemName (
      itemHelpItem,
      oahAtomName);
  }

  else if (
    // integer item?
    S_optionsIntegerItem
      integerItem =
        dynamic_cast<optionsIntegerItem*>(&(*item))
  ) {
    handleOptionsIntegerItemItemName (
      integerItem,
      oahAtomName);
  }

  else if (
    // float item?
    S_optionsFloatItem
      floatItem =
        dynamic_cast<optionsFloatItem*>(&(*item))
  ) {
    handleOptionsFloatItemItemName (
      floatItem,
      oahAtomName);
  }

  else if (
    // string item?
    S_optionsStringItem
      stringItem =
        dynamic_cast<optionsStringItem*>(&(*item))
  ) {
    handleOptionsStringItemItemName (
      stringItem,
      oahAtomName);
  }

  else if (
    // rational item?
    S_optionsRationalItem
      rationalItem =
        dynamic_cast<optionsRationalItem*>(&(*item))
  ) {
    handleOptionsRationalItemItemName (
      rationalItem,
      oahAtomName);
  }

  else if (
    // numbers set item?
    S_optionsNumbersSetItem
      numbersSetItem =
        dynamic_cast<optionsNumbersSetItem*>(&(*item))
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
      fOptionsHandlerlogIOstream <<
        "==> item is of another type" <<
        ", let the oahGroup handle it" <<
        endl;
    }
#endif

    S_oahGroup
      group =
        item->
          getOptionsSubGroupUpLink ()->
            getOptionsGroupUpLink ();

    fPendingOptionsValuedItem =
      group->
        handleOptionsItem (
          fOptionsHandlerlogIOstream,
          item);
  }
}

void oahHandler::handleOptionsItemName (
  string oahAtomName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fOptionsHandlerlogIOstream <<
      "==> handleOptionsItemName (), oahAtomName = \"" <<
      oahAtomName <<
      "\"" <<
      endl;
  }
#endif

  // is oahAtomName known in options elements map?
  S_oahElement
    element =
      fetchOptionsElementFromMap (
        oahAtomName);

  if (! element) {
    // oahAtomName is is not well handled by this options handler
    printHelpSummary ();

    stringstream s;

    s <<
      "option name '" << oahAtomName <<
      "' is unknown, see help summary above";

    optionError (s.str ());
    exit (6);
  }

  else {
    // oahAtomName is known, let's handle it
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fOptionsHandlerlogIOstream <<
      "==> handleOptionsItemName (), oahAtomName = \"" <<
      oahAtomName <<
      "\" is described by oahElement:" <<
      endl;
    gIndenter++;
    element->print (fOptionsHandlerlogIOstream);
    gIndenter--;
  }
#endif

    fCommandOptionsElements.push_back (element);

    // determine option element short and long names to be used,
    // in case one of them (short or long) is empty
    string
      shortName =
        element->getOptionsElementShortName (),
      longName =
        element->getOptionsElementLongName ();

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
          oahAtomName);
    }

    else if (
      // options group?
      S_oahGroup
        group =
          dynamic_cast<oahGroup*>(&(*element))
    ) {
      handleOptionsGroupItemName (
        group,
        oahAtomName);
    }

    else if (
      // options subgroup?
      S_oahSubGroup
        subGroup =
          dynamic_cast<oahSubGroup*>(&(*element))
    ) {
      handleOptionsSubGroupItemName (
        subGroup,
        oahAtomName);
    }

    else if (
      // options item?
      S_oahAtom
        item =
          dynamic_cast<oahAtom*>(&(*element))
    ) {

      handleOptionsItemItemName (
        item,
        oahAtomName);
    }

    else {
      stringstream s;

      s <<
        "INTERNAL ERROR: option name '" << oahAtomName <<
        "' cannot be handled";

      optionError (s.str ());
      exit (7);
    }
  }
}

void oahHandler::handleOptionsItemHelpValue (
  S_oahAtomHelpItem itemHelpItem,
  string                theString)
{
  // handle the option item
  printSpecificItemHelp (
    fOptionsHandlerlogIOstream,
    theString);

  // exit
  exit (23);
}

void oahHandler::handleOptionsItemIntegerValue (
  S_optionsIntegerItem integerItem,
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
    setIntegerItemVariableValue (
      integerValue);
}

void oahHandler::handleOptionsItemFloatValue (
  S_optionsFloatItem floatItem,
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
  S_optionsStringItem stringItem,
  string              theString)
{
  // handle the option item
  stringItem->
    setStringItemVariableValue (
      theString);
}

void oahHandler::handleOptionsItemRationalValue (
  S_optionsRationalItem rationalItem,
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
      fOptionsHandlerlogIOstream <<
        "There are " << smSize << " matches" <<
        " for rational string '" << theString <<
        "' with regex '" << regularExpression <<
        "'" <<
        endl;

      for (unsigned i = 0; i < smSize; ++i) {
        fOptionsHandlerlogIOstream <<
          "[" << sm [i] << "] ";
      } // for

      fOptionsHandlerlogIOstream << endl;
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
      fOptionsHandlerlogIOstream,
      rationalItem->
        getOptionsSubGroupUpLink ());

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
    fOptionsHandlerlogIOstream << // JMI
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
  S_optionsNumbersSetItem numbersSetItem,
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
    fOptionsHandlerlogIOstream <<
      "==> handleOptionsItemValueOrArgument ()" <<
      endl;

    gIndenter++;

    fOptionsHandlerlogIOstream <<
      "fPendingOptionsValuedItem:" <<
      endl;

    gIndenter++;
    if (fPendingOptionsValuedItem) {
      fOptionsHandlerlogIOstream <<
        fPendingOptionsValuedItem;
    }
    else {
      fOptionsHandlerlogIOstream <<
        "null" <<
        endl;
    }
    gIndenter--;

    fOptionsHandlerlogIOstream <<
      "theString:" <<
      endl;

    gIndenter++;
    fOptionsHandlerlogIOstream <<
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
  if (fPendingOptionsValuedItem) {
    // theString is the value for the pending options item

    if (
      // item help item?
      S_oahAtomHelpItem
        itemHelpItem =
          dynamic_cast<oahAtomHelpItem*>(&(*fPendingOptionsValuedItem))
    ) {
      handleOptionsItemHelpValue (
        itemHelpItem,
        theString);
    }

    else if (
      // integer item?
      S_optionsIntegerItem
        integerItem =
          dynamic_cast<optionsIntegerItem*>(&(*fPendingOptionsValuedItem))
    ) {
      handleOptionsItemIntegerValue (
        integerItem,
        theString);
    }

    else if (
      // float item?
      S_optionsFloatItem
        floatItem =
          dynamic_cast<optionsFloatItem*>(&(*fPendingOptionsValuedItem))
    ) {
      handleOptionsItemFloatValue (
        floatItem,
        theString);
    }

    else if (
      // string item?
      S_optionsStringItem
        stringItem =
          dynamic_cast<optionsStringItem*>(&(*fPendingOptionsValuedItem))
    ) {
      handleOptionsItemStringValue (
        stringItem,
        theString);
    }

    else if (
      // rational item?
      S_optionsRationalItem
        rationalItem =
          dynamic_cast<optionsRationalItem*>(&(*fPendingOptionsValuedItem))
    ) {
      handleOptionsItemRationalValue (
        rationalItem,
        theString);
    }

    else if (
      // numbers set item?
      S_optionsNumbersSetItem
        numbersSetItem =
          dynamic_cast<optionsNumbersSetItem*>(&(*fPendingOptionsValuedItem))
    ) {
      handleOptionsItemNumbersSetValue (
        numbersSetItem,
        theString);
    }

    else {
      // fPendingOptionsValuedItem is of another type,
      // let the oahGroup handle it

#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
        fOptionsHandlerlogIOstream <<
          "==> fPendingOptionsValuedItem is of another type" <<
          ", let the oahGroup handle it" <<
          endl;
      }
#endif

      S_oahGroup
        group =
          fPendingOptionsValuedItem->
            getOptionsSubGroupUpLink ()->
              getOptionsGroupUpLink ();

      group->
        handleOptionsItemValue (
          fOptionsHandlerlogIOstream,
          fPendingOptionsValuedItem,
          theString);
    }

    fPendingOptionsValuedItem = nullptr;
  }

  else {
    // theString is an argument

#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
        fOptionsHandlerlogIOstream <<
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
      checkMissingPendingOptionsValuedItemValue (
        "FOO");
*/
