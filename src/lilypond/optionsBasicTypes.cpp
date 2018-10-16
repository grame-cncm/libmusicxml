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

#include "messagesHandling.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
  #include "generalOptions.h"
#endif


#include "optionsBasicTypes.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
S_optionsElement optionsElement::create (
  string optionsElementShortName,
  string optionsElementLongName,
  string optionsElementDescription)
{
  optionsElement* o = new
    optionsElement (
      optionsElementShortName,
      optionsElementLongName,
      optionsElementDescription);
  assert(o!=0);
  return o;
}

optionsElement::optionsElement (
  string optionsElementShortName,
  string optionsElementLongName,
  string optionsElementDescription)
{
  fOptionsElementShortName   = optionsElementShortName;  
  fOptionsElementLongName    = optionsElementLongName;  
  fOptionsElementDescription = optionsElementDescription;

  fOptionsElementIsHidden    = false;
}

optionsElement::~optionsElement ()
{}

S_optionsElement optionsElement::fetchOptionElement (
  string optiontElementName)
{
  S_optionsElement result;

  if (
    optiontElementName == fOptionsElementShortName
     ||
    optiontElementName == fOptionsElementLongName) {
    result = this;
  }
  
  return result;
}

string optionsElement::optionsElementNames () const
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

string optionsElement::optionsElementNamesInColumns (
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

string optionsElement::optionsElementNamesBetweenParentheses () const
{
  stringstream s;

  s <<
    "(" <<
    optionsElementNames () <<
    ")";
  
  return s.str ();
}

string optionsElement::optionsElementNamesInColumnsBetweenParentheses (
  int subGroupsShortNameFieldWidth) const
{
  stringstream s;

  s <<
    "(" <<
    optionsElementNamesInColumns (
      subGroupsShortNameFieldWidth) <<
    ")";
  
  return s.str ();
}

void optionsElement::printHeader (ostream& os) const
{
  os <<
    "-" << fOptionsElementShortName <<
    endl <<
    "-" << fOptionsElementLongName <<
    endl;

  // indent a bit more for readability
  gIndenter.increment (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);
  
  os <<
    gIndenter.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;

  gIndenter.decrement (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);
}

void optionsElement::printElementEssentials (
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

void optionsElement::print (ostream& os) const
{
  os <<
    "??? optionsElement ???" <<
    endl;

  printElementEssentials (os, 35);  
}

void optionsElement::printHelp (ostream& os) const
{
  os <<
    optionsElementNames () <<
    endl <<
    endl;

  // indent a bit more for readability
  gIndenter.increment (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);
  
  os <<
    gIndenter.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;

  gIndenter.decrement (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);
}

ostream& operator<< (ostream& os, const S_optionsElement& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsItem optionsItem::create (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
{
  optionsItem* o = new
    optionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription);
  assert(o!=0);
  return o;
}

optionsItem::optionsItem (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
  : optionsElement (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription)
{
}
    
optionsItem::~optionsItem ()
{}

void optionsItem::setOptionsSubGroupUplink (
  S_optionsSubGroup optionsSubGroup)
{
  // sanity check
  msrAssert (
    optionsSubGroup != nullptr,
    "optionsSubGroup is null");

  // set the uplink
  fOptionsSubGroupUplink =
    optionsSubGroup;
}

void optionsItem::registerOptionsItemInHandler (
  S_optionsHandler optionsHandler)
{
  optionsHandler->
    registerOptionsElementInHandler (this);
}

void optionsItem::print (ostream& os) const
{
  const int fieldWidth = 19;
  
  os <<
    "OptionsItem ???:" <<
      endl;

  gIndenter++;

  optionsElement::printElementEssentials (
    os, fieldWidth);

  gIndenter--;
}

void optionsItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os <<
    "OptionsItem values ???:" <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsHelpUsageItem optionsHelpUsageItem::create (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
{
  optionsHelpUsageItem* o = new
    optionsHelpUsageItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription);
  assert(o!=0);
  return o;
}

optionsHelpUsageItem::optionsHelpUsageItem (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
  : optionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription)
{}

optionsHelpUsageItem::~optionsHelpUsageItem ()
{}

void optionsHelpUsageItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsHelpUsageItem:" <<
    endl;

  gIndenter++;

  optionsElement::printElementEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void optionsHelpUsageItem::printHelpUsage (ostream& os) const
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

A subgroup displayed with '***' has its description printed
only when the corresponding item short or long names are used.

Both '-' and '--' can be used to introduce options in the command line,
even though the help facility only shows them with '-'.

Command line options and arguments can be placed in any order,
provided item values immediately follow the corresponding items.)") <<
    endl <<
    endl;

  gIndenter--;
}

void optionsHelpUsageItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}
                            
ostream& operator<< (ostream& os, const S_optionsHelpUsageItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsHelpSummaryItem optionsHelpSummaryItem::create (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
{
  optionsHelpSummaryItem* o = new
    optionsHelpSummaryItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription);
  assert(o!=0);
  return o;
}

optionsHelpSummaryItem::optionsHelpSummaryItem (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
  : optionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription)
{}

optionsHelpSummaryItem::~optionsHelpSummaryItem ()
{}

void optionsHelpSummaryItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsHelpSummaryItem:" <<
    endl;

  gIndenter++;

  optionsElement::printElementEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void optionsHelpSummaryItem::printHelpSummary (ostream& os) const
{  
  os <<
    "xml2ly"<<
    endl;
}

void optionsHelpSummaryItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}
                            
ostream& operator<< (ostream& os, const S_optionsHelpSummaryItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsCombinedItemsItem optionsCombinedItemsItem::create (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription,
  string optionsCombinedItemsItemVariableDisplayName,
  bool&  optionsCombinedItemsItemVariable)
{
  optionsCombinedItemsItem* o = new
    optionsCombinedItemsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsCombinedItemsItemVariableDisplayName,
      optionsCombinedItemsItemVariable);
  assert(o!=0);
  return o;
}

optionsCombinedItemsItem::optionsCombinedItemsItem (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription,
  string optionsCombinedItemsItemVariableDisplayName,
  bool&  optionsCombinedItemsItemVariable)
  : optionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription),
    fOptionsCombinedItemsItemVariableDisplayName (
      optionsCombinedItemsItemVariableDisplayName),
    fOptionsCombinedItemsItemVariable (
      optionsCombinedItemsItemVariable)
{}

optionsCombinedItemsItem::~optionsCombinedItemsItem ()
{}

void optionsCombinedItemsItem::appendOptionsItemToCombinedItemsList (
  S_optionsItem optionsItem)
{
  // sanity check
  msrAssert (
    optionsItem != nullptr,
    "optionsItem is null");
    
  fOptionsCombinedItemsList.push_back (
    optionsItem);
}

void optionsCombinedItemsItem::appendOptionsItemToCombinedItemsList (
  string optionsItemName)
{
  // get the options handler
  S_optionsHandler
    optionsHandler =
      getOptionsSubGroupUplink ()->
        getOptionsGroupUplink ()->
          getOptionsHandlerUplink ();

  // sanity check
  msrAssert (
    optionsHandler != nullptr,
    "optionsHandler is null");
    
  // is optionsItemName known in options elements map?
  S_optionsElement
    optionsElement =
      optionsHandler->
        fetchOptionsElementFromMap (
          optionsItemName);

  if (! optionsElement) {
    // no, optionsItemName is unknown in the map
    optionsHandler->
      printHelpSummary ();

    stringstream s;

    s <<
      "INTERNAL ERROR: option name '" << optionsItemName <<
      "' is unknown";
      
    optionError (s.str ());
  }

  else {
    // optionsItemName is known, let's handle it

    if (
      // options item?
      S_optionsItem
        item =
          dynamic_cast<optionsItem*>(&(*optionsElement))
      ) {
      // handle the option item
      fOptionsCombinedItemsList.push_back (
        item);
    }

    else {
      stringstream s;
  
      s <<
        "option name '" << optionsItemName <<
        "' is not that of an item";
        
      optionError (s.str ());
  
      exit (2);
    }
  }
}

void optionsCombinedItemsItem::setCombinedItemsVariablesValue (
  bool value)
{
  // set the combined items item variable
  fOptionsCombinedItemsItemVariable = value;

  // set the value of the items in the list
  if (fOptionsCombinedItemsList.size ()) {      
    for (
      list<S_optionsItem>::const_iterator i =
        fOptionsCombinedItemsList.begin ();
      i != fOptionsCombinedItemsList.end ();
      i++) {
      S_optionsItem
        item = (*i);

      if (
        // boolean item?
        S_optionsBooleanItem
          booleanItem =
            dynamic_cast<optionsBooleanItem*>(&(*item))
        ) {
        // set the boolean value
        booleanItem->
          setBooleanItemVariableValue (value);
      }      
    } // for
  }
}

void optionsCombinedItemsItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsCombinedItemsItem:" <<
    endl;

  gIndenter++;

  printElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsCombinedItemsList" << " : ";

  if (! fOptionsCombinedItemsList.size ()) {
    os <<
      "none";
  }
    
  else {
    os <<
      endl;

    gIndenter++;

    os <<
        "'";
      
    list<S_optionsItem>::const_iterator
      iBegin = fOptionsCombinedItemsList.begin (),
      iEnd   = fOptionsCombinedItemsList.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << " ";
    } // for
  
    os <<
      "'";

    gIndenter--;
  }

  gIndenter--;
  
  os <<
    endl;
}

void optionsCombinedItemsItem::printHelp (ostream& os) const
{
  os <<
    optionsElementNames () <<
    endl <<
    endl;

  // indent a bit more for readability
  gIndenter.increment (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);
  
  os <<
    gIndenter.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;

  os <<
    "This combined option is equivalent to: ";

  if (! fOptionsCombinedItemsList.size ()) {
    os <<
      "none" <<
      endl;
  }
    
  else {
    os <<
      endl;

    gIndenter++;
          
    list<S_optionsItem>::const_iterator
      iBegin = fOptionsCombinedItemsList.begin (),
      iEnd   = fOptionsCombinedItemsList.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      os <<
        (*i)-> optionsElementNames ();
      if (++i == iEnd) break;
      os << endl;
    } // for
  
    os <<
      endl;
  
    gIndenter--;
  }

  gIndenter.decrement (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);
    
  os <<
    endl;
}

void optionsCombinedItemsItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsCombinedItemsItemVariableDisplayName <<
    " : " <<
    booleanAsString (
      fOptionsCombinedItemsItemVariable) <<
    endl;

  int fieldWidth =
    valueFieldWidth - gIndenter.getIndent () + 1;
    
  gIndenter++; // only now

  if (! fOptionsCombinedItemsList.size ()) {
    os <<
      "none" <<
      endl;
  }
    
  else {      
    list<S_optionsItem>::const_iterator
      iBegin = fOptionsCombinedItemsList.begin (),
      iEnd   = fOptionsCombinedItemsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_optionsItem
        item = (*i);

      if (
        // boolean item?
        S_optionsBooleanItem
          booleanItem =
            dynamic_cast<optionsBooleanItem*>(&(*item))
        ) {
        // set the boolean value
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

ostream& operator<< (ostream& os, const S_optionsCombinedItemsItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsBooleanItem optionsBooleanItem::create (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription,
  string optionsBooleanItemVariableDisplayName,
  bool&  optionsBooleanItemVariable)
{
  optionsBooleanItem* o = new
    optionsBooleanItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsBooleanItemVariableDisplayName,
      optionsBooleanItemVariable);
  assert(o!=0);
  return o;
}

optionsBooleanItem::optionsBooleanItem (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription,
  string optionsBooleanItemVariableDisplayName,
  bool&  optionsBooleanItemVariable)
  : optionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription),
    fOptionsBooleanItemVariableDisplayName (
      optionsBooleanItemVariableDisplayName),
    fOptionsBooleanItemVariable (
      optionsBooleanItemVariable)
{}

optionsBooleanItem::~optionsBooleanItem ()
{}

void optionsBooleanItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsBooleanItem:" <<
    endl;

  gIndenter++;

  optionsElement::printElementEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;
  gIndenter--;

  os << left <<
    setw (fieldWidth) <<
    "fOptionsBooleanItemVariableDisplayName" << " : " <<
    fOptionsBooleanItemVariableDisplayName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsBooleanItemVariable" << " : " <<
    booleanAsString (
      fOptionsBooleanItemVariable) <<
    endl;

  gIndenter--;
}

void optionsBooleanItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsBooleanItemVariableDisplayName <<
    " : " <<
    booleanAsString (
      fOptionsBooleanItemVariable) <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsBooleanItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsTwoBooleansItem optionsTwoBooleansItem::create (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription,
  string optionsTwoBooleansItemVariableDisplayName,
  bool&  optionsTwoBooleansItemVariable,
  bool&  optionsTwoBooleansItemSecondaryVariable)
{
  optionsTwoBooleansItem* o = new
    optionsTwoBooleansItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsTwoBooleansItemVariableDisplayName,
      optionsTwoBooleansItemVariable,
      optionsTwoBooleansItemSecondaryVariable);
  assert(o!=0);
  return o;
}

optionsTwoBooleansItem::optionsTwoBooleansItem (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription,
  string optionsTwoBooleansItemVariableDisplayName,
  bool&  optionsTwoBooleansItemVariable,
  bool&  optionsTwoBooleansItemSecondaryVariable)
  : optionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription),
    fOptionsTwoBooleansItemVariableDisplayName (
      optionsTwoBooleansItemVariableDisplayName),
    fOptionsTwoBooleansItemVariable (
      optionsTwoBooleansItemVariable),
    fOptionsTwoBooleansItemSecondaryVariable (
      optionsTwoBooleansItemSecondaryVariable)
{}

optionsTwoBooleansItem::~optionsTwoBooleansItem ()
{}

void optionsTwoBooleansItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsTwoBooleansItem:" <<
    endl;

  gIndenter++;

  optionsElement::printElementEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;
  gIndenter--;

  os << left <<
    setw (fieldWidth) <<
    "fOptionsTwoBooleansItemVariableDisplayName" << " : " <<
    fOptionsTwoBooleansItemVariableDisplayName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsTwoBooleansItemVariable" << " : " <<
    booleanAsString (
      fOptionsTwoBooleansItemVariable) <<
    endl;

  gIndenter--;
}

void optionsTwoBooleansItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsTwoBooleansItemVariableDisplayName <<
    " : " <<
    booleanAsString (
      fOptionsTwoBooleansItemVariable) <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsTwoBooleansItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsThreeBooleansItem optionsThreeBooleansItem::create (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription,
  string optionsThreeBooleansItemVariableDisplayName,
  bool&  optionsThreeBooleansItemVariable,
  bool&  optionsThreeBooleansItemSecondaryVariable,
  bool&  optionsThreeBooleansItemTertiaryVariable)
{
  optionsThreeBooleansItem* o = new
    optionsThreeBooleansItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsThreeBooleansItemVariableDisplayName,
      optionsThreeBooleansItemVariable,
      optionsThreeBooleansItemSecondaryVariable,
      optionsThreeBooleansItemTertiaryVariable);
  assert(o!=0);
  return o;
}

optionsThreeBooleansItem::optionsThreeBooleansItem (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription,
  string optionsThreeBooleansItemVariableDisplayName,
  bool&  optionsThreeBooleansItemVariable,
  bool&  optionsThreeBooleansItemSecondaryVariable,
  bool&  optionsThreeBooleansItemTertiaryVariable)
  : optionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription),
    fOptionsThreeBooleansItemVariableDisplayName (
      optionsThreeBooleansItemVariableDisplayName),
    fOptionsThreeBooleansItemVariable (
      optionsThreeBooleansItemVariable),
    fOptionsThreeBooleansItemSecondaryVariable (
      optionsThreeBooleansItemSecondaryVariable),
    fOptionsThreeBooleansItemTertiaryVariable (
      optionsThreeBooleansItemTertiaryVariable)
{}

optionsThreeBooleansItem::~optionsThreeBooleansItem ()
{}

void optionsThreeBooleansItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsThreeBooleansItem:" <<
    endl;

  gIndenter++;

  optionsElement::printElementEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;
  gIndenter--;

  os << left <<
    setw (fieldWidth) <<
    "fOptionsThreeBooleansItemVariableDisplayName" << " : " <<
    fOptionsThreeBooleansItemVariableDisplayName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsThreeBooleansItemVariable" << " : " <<
    booleanAsString (
      fOptionsThreeBooleansItemVariable) <<
    endl;

  gIndenter--;
}

void optionsThreeBooleansItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsThreeBooleansItemVariableDisplayName <<
    " : " <<
    booleanAsString (
      fOptionsThreeBooleansItemVariable) <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsThreeBooleansItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsValuedItem optionsValuedItem::create (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription,
  string optionsValueSpecification)
{
  optionsValuedItem* o = new
    optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification);
  assert(o!=0);
  return o;
}

optionsValuedItem::optionsValuedItem (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription,
  string optionsValueSpecification)
  : optionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription)
{
  fOptionsValueSpecification = optionsValueSpecification;
}
    
optionsValuedItem::~optionsValuedItem ()
{}

string optionsValuedItem::optionsItemKindAsString (
  optionsValuedItem::optionsValuedItemKind optionsItemKind)
{
  string result;
  
  switch (optionsItemKind) {
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

void optionsValuedItem::registerOptionsItemInHandler (
  S_optionsHandler optionsHandler)
{
  optionsHandler->
    registerOptionsElementInHandler (this);
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
    optionsElementNames () <<
    " " <<
    fOptionsValueSpecification <<
    endl <<
    endl;

  // indent a bit more for readability
  gIndenter.increment (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);
  
  os <<
    gIndenter.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;

  gIndenter.decrement (K_OPTIONS_ELEMENTS_INDENTER_OFFSET);
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
S_optionsItemHelpItem optionsItemHelpItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification)
{
  optionsItemHelpItem* o = new
    optionsItemHelpItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification);
  assert(o!=0);
  return o;
}

optionsItemHelpItem::optionsItemHelpItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification)
{}

optionsItemHelpItem::~optionsItemHelpItem ()
{}

void optionsItemHelpItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsItemHelpItem:" <<
    endl;

  gIndenter++;

  optionsItemHelpItem::printValuedItemEssentials (
    os, fieldWidth);

  gIndenter--;
}

void optionsItemHelpItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}
                            
ostream& operator<< (ostream& os, const S_optionsItemHelpItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsIntegerItem optionsIntegerItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsIntegerItemVariableDisplayName,
  int&               optionsIntegerItemVariable)
{
  optionsIntegerItem* o = new
    optionsIntegerItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsIntegerItemVariableDisplayName,
      optionsIntegerItemVariable);
  assert(o!=0);
  return o;
}

optionsIntegerItem::optionsIntegerItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsIntegerItemVariableDisplayName,
  int&               optionsIntegerItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsIntegerItemVariableDisplayName (
      optionsIntegerItemVariableDisplayName),
    fOptionsIntegerItemVariable (
      optionsIntegerItemVariable)
{}

optionsIntegerItem::~optionsIntegerItem ()
{}

void optionsIntegerItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsIntegerItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsIntegerItemVariableDisplayName" << " : " <<
    fOptionsIntegerItemVariableDisplayName <<
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
    fOptionsIntegerItemVariableDisplayName <<
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
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsFloatItemVariableDisplayName,
  float&             optionsFloatItemVariable)
{
  optionsFloatItem* o = new
    optionsFloatItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsFloatItemVariableDisplayName,
      optionsFloatItemVariable);
  assert(o!=0);
  return o;
}

optionsFloatItem::optionsFloatItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsFloatItemVariableDisplayName,
  float&             optionsFloatItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsFloatItemVariableDisplayName (
      optionsFloatItemVariableDisplayName),
    fOptionsFloatItemVariable (
      optionsFloatItemVariable)
{}

optionsFloatItem::~optionsFloatItem ()
{}

void optionsFloatItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsFloatItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsFloatItemVariableDisplayName" << " : " <<
    fOptionsFloatItemVariableDisplayName <<
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
    fOptionsFloatItemVariableDisplayName <<
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
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsStringItemVariableDisplayName,
  string&            optionsStringItemVariable)
{
  optionsStringItem* o = new
    optionsStringItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsStringItemVariableDisplayName,
      optionsStringItemVariable);
  assert(o!=0);
  return o;
}

optionsStringItem::optionsStringItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsStringItemVariableDisplayName,
  string&            optionsStringItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsStringItemVariableDisplayName (
      optionsStringItemVariableDisplayName),
    fOptionsStringItemVariable (
      optionsStringItemVariable)
{}

optionsStringItem::~optionsStringItem ()
{}

void optionsStringItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsStringItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsStringItemVariableDisplayName" << " : " <<
    fOptionsStringItemVariableDisplayName <<
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
    fOptionsStringItemVariableDisplayName <<
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
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsRationalItemVariableDisplayName,
  rational&          optionsRationalItemVariable)
{
  optionsRationalItem* o = new
    optionsRationalItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsRationalItemVariableDisplayName,
      optionsRationalItemVariable);
  assert(o!=0);
  return o;
}

optionsRationalItem::optionsRationalItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsRationalItemVariableDisplayName,
  rational&          optionsRationalItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsRationalItemVariableDisplayName (
      optionsRationalItemVariableDisplayName),
    fOptionsRationalItemVariable (
      optionsRationalItemVariable)
{}

optionsRationalItem::~optionsRationalItem ()
{}

void optionsRationalItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsRationalItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsRationalItemVariableDisplayName" << " : " <<
    fOptionsRationalItemVariableDisplayName <<
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
    fOptionsRationalItemVariableDisplayName <<
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
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsNumbersSetItemVariableDisplayName,
  set<int>&          optionsNumbersSetItemVariable)
{
  optionsNumbersSetItem* o = new
    optionsNumbersSetItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsNumbersSetItemVariableDisplayName,
      optionsNumbersSetItemVariable);
  assert(o!=0);
  return o;
}

optionsNumbersSetItem::optionsNumbersSetItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsNumbersSetItemVariableDisplayName,
  set<int>&          optionsNumbersSetItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsNumbersSetItemVariableDisplayName (
      optionsNumbersSetItemVariableDisplayName),
    fOptionsNumbersSetItemVariable (
      optionsNumbersSetItemVariable)
{}

optionsNumbersSetItem::~optionsNumbersSetItem ()
{}

void optionsNumbersSetItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsNumbersSetItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsNumbersSetItemVariableDisplayName" << " : " <<
    fOptionsNumbersSetItemVariableDisplayName <<
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

  os <<
    endl;

  gIndenter--;
}

void optionsNumbersSetItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsNumbersSetItemVariableDisplayName <<
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

  os <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsNumbersSetItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsSubGroup optionsSubGroup::create (
  string optionsSubGroupHelpHeader,
  string optionsSubGroupShortName,
  string optionsSubGroupLongName,
  string optionsSubGroupDescription,
  optionsSubGroupDescriptionVisibilityKind
         optionsSubGroupDescriptionVisibilityKind,
  S_optionsGroup
         optionsGroupUplink)
{
  optionsSubGroup* o = new
    optionsSubGroup (
      optionsSubGroupHelpHeader,
      optionsSubGroupShortName,
      optionsSubGroupLongName,
      optionsSubGroupDescription,
      optionsSubGroupDescriptionVisibilityKind,
      optionsGroupUplink);
  assert(o!=0);
  return o;
}

optionsSubGroup::optionsSubGroup (
  string optionsSubGroupHelpHeader,
  string optionsSubGroupShortName,
  string optionsSubGroupLongName,
  string optionsSubGroupDescription,
  optionsSubGroupDescriptionVisibilityKind
         optionsSubGroupDescriptionVisibilityKind,
  S_optionsGroup
         optionsGroupUplink)
  : optionsElement (
      optionsSubGroupShortName,
      optionsSubGroupLongName,
      optionsSubGroupDescription)
{
  fOptionsGroupUplink =
    optionsGroupUplink;
  
  fOptionsSubGroupHelpHeader =
    optionsSubGroupHelpHeader;

  fOptionsSubGroupDescriptionVisibilityKind =
    optionsSubGroupDescriptionVisibilityKind;
}

optionsSubGroup::~optionsSubGroup ()
{}

string optionsSubGroup::optionsSubGroupDescriptionVisibilityKindAsString (
  optionsSubGroupDescriptionVisibilityKind
    optionsSubGroupDescriptionVisibilityKind)
{
  string result;
  
  switch (optionsSubGroupDescriptionVisibilityKind) {
    case kAlwaysShowDescription:
      result = "AlwaysShowDescription";
      break;
      
    case kHideDescriptionByDefault:
      result = "HideDescriptionByDefault";
      break;
  } // switch

  return result;
}

void optionsSubGroup::underlineHeader (ostream& os) const
{
  for (unsigned int i = 0; i < fOptionsSubGroupHelpHeader.size (); i++) {
    os << "-";
  } // for
  os <<
    endl;
}

void optionsSubGroup::registerOptionsSubGroupInHandler (
  S_optionsHandler optionsHandler)
{
  optionsHandler->
    registerOptionsElementInHandler (this);

  for (
    list<S_optionsItem>::const_iterator
      i = fOptionsSubGroupItemsList.begin ();
    i != fOptionsSubGroupItemsList.end ();
    i++) {
    // register the options sub group
    (*i)->
      registerOptionsItemInHandler (
        optionsHandler);
  } // for
}

void optionsSubGroup::appendOptionsItem (
  S_optionsItem optionsItem)
{
  // sanity check
  msrAssert (
    optionsItem != nullptr,
    "optionsItem is null");

  // append options item
  fOptionsSubGroupItemsList.push_back (
    optionsItem);

  // set options item subgroup uplink
  optionsItem->
    setOptionsSubGroupUplink (this);
}

S_optionsElement optionsSubGroup::fetchOptionElement (
  string optiontElementName)
{
  S_optionsElement result;
  
  for (
    list<S_optionsItem>::const_iterator
      i = fOptionsSubGroupItemsList.begin ();
    i != fOptionsSubGroupItemsList.end ();
    i++) {
    // search optiontElementName in the options group
    result =
      (*i)->fetchOptionElement (optiontElementName);
      
    if (result != 0) {
      break;
    }
  } // for

  return result;
}

void optionsSubGroup::print (ostream& os) const
{
  const int fieldWidth = 27;
  
  os <<
   "OptionsSubGroup:" <<
    endl;

  gIndenter++;

  optionsElement::printElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsSubGroupDescriptionVisibility" << " : " <<
      optionsSubGroupDescriptionVisibilityKindAsString (
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
    
    list<S_optionsItem>::const_iterator
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

void optionsSubGroup::printHelp (ostream& os) const
{
  
  // print the header and option names
  os <<
    fOptionsSubGroupHelpHeader;
    
  os <<
    " " <<
    optionsElementNamesBetweenParentheses ();

  switch (fOptionsSubGroupDescriptionVisibilityKind) {
    case kAlwaysShowDescription:
      os <<
        ":";
      break;
      
    case kHideDescriptionByDefault:
      os <<
        " ***";
      break;
  } // switch

  os <<
    endl <<
    endl;

  // print the description if any
  if (fOptionsElementDescription.size ()) {
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fOptionsElementDescription) <<
      endl;
    gIndenter--;

    os <<
      endl;  
  }
  
  switch (fOptionsSubGroupDescriptionVisibilityKind) {
    case kAlwaysShowDescription:
      if (fOptionsSubGroupItemsList.size ()) {    
        gIndenter++;
    
        list<S_optionsItem>::const_iterator
          iBegin = fOptionsSubGroupItemsList.begin (),
          iEnd   = fOptionsSubGroupItemsList.end (),
          i      = iBegin;
        for ( ; ; ) {
          S_optionsItem
            optionsItem = (*i);
          bool
            optionsItemIsHidden =
              optionsItem->getOptionsElementIsHidden ();
              
          // print the options item help unless it is hidden
          if (! optionsItemIsHidden) {
            optionsItem->
              printHelp (os);
          }
          if (++i == iEnd) break;
          if (! optionsItemIsHidden) {
            os <<
              endl;
          }
        } // for
          
        gIndenter--;
      }
      break;
      
    case kHideDescriptionByDefault:
      break;
  } // switch
}

void optionsSubGroup::printOptionsSubGroupForcedHelp (ostream& os) const
{
  // print the header and option names
  os <<
    fOptionsSubGroupHelpHeader;
    
  os <<
    " " <<
    optionsElementNamesBetweenParentheses ();

  switch (fOptionsSubGroupDescriptionVisibilityKind) {
    case kAlwaysShowDescription:
      os <<
        ":";
      break;
      
    case kHideDescriptionByDefault:
      os <<
        " *** :";
      break;
  } // switch

  os <<
    endl <<
    endl;

  // print the description if any
  if (fOptionsElementDescription.size ()) {
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fOptionsElementDescription) <<
      endl;
    gIndenter--;

    os <<
      endl;  
  }
      
  if (fOptionsSubGroupItemsList.size ()) {    
    gIndenter++;

    list<S_optionsItem>::const_iterator
      iBegin = fOptionsSubGroupItemsList.begin (),
      iEnd   = fOptionsSubGroupItemsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options item help
      (*i)->printHelp (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }

  os <<
    endl;
}

void optionsSubGroup::printHelpSummary (
  ostream& os) const
{
  // fetch maximum subgroups help headers size
  int maximumSubGroupsHelpHeadersSize =
    getOptionsGroupUplink ()->
      getOptionsHandlerUplink ()->
        getMaximumSubGroupsHelpHeadersSize ();
    
  // fetch maximum short name width
  int maximumShortNameWidth =
    getOptionsGroupUplink ()->
      getOptionsHandlerUplink ()->
        getMaximumShortNameWidth ();

  // print the summary
  os << left <<
    setw (maximumSubGroupsHelpHeadersSize) <<
    fOptionsSubGroupHelpHeader <<
    " " <<
    optionsElementNamesInColumnsBetweenParentheses (
      maximumShortNameWidth);

  switch (fOptionsSubGroupDescriptionVisibilityKind) {
    case kAlwaysShowDescription:
      break;
      
    case kHideDescriptionByDefault:
      os <<
        " ***";
      break;
  } // switch

  os <<
    endl <<
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
}

void optionsSubGroup::printSpecificSubGroupHelp (
  ostream& os,
  S_optionsSubGroup
           optionsSubGroup) const
{
  // print only the summary if this is not the desired subgroup,
  // otherwise print the regular help
  if (optionsSubGroup == this) {
    os <<
      endl;
    printSpecificSubGroupHelp (
      os, optionsSubGroup);
  }
  else {
    printHelpSummary (os);
  }
 }

void optionsSubGroup::printOptionsItemForcedHelp (
  ostream&         os,
  S_optionsItem targetOptionsItem) const
{
  // print the header
  os <<
    fOptionsSubGroupHelpHeader <<
    " " <<
    optionsElementNamesBetweenParentheses () <<
    ":" <<
    endl <<
    endl;

  // underline the options subgroup header
// JMI  underlineHeader (os);

  // print the options items
  if (fOptionsSubGroupItemsList.size ()) {    
    gIndenter++;

    list<S_optionsItem>::const_iterator
      iBegin = fOptionsSubGroupItemsList.begin (),
      iEnd   = fOptionsSubGroupItemsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_optionsItem
        optionsItem = (*i);
        
      if (optionsItem == targetOptionsItem) {
        // print the target options item's help
        // target options item's help
        (*i)->
          printHelp (
            os);
      }
      if (++i == iEnd) break;
      if (optionsItem == targetOptionsItem) {
        os <<
          endl;
      }
    } // for
    
    gIndenter--;
  }
}

void optionsSubGroup::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // print the header
  os <<
    fOptionsSubGroupHelpHeader <<
    " " <<
    optionsElementNamesBetweenParentheses () <<
    ":" <<
    endl;

  // underline the options subgroup header
// JMI  underlineHeader (os);

  // print the options items values
  if (fOptionsSubGroupItemsList.size ()) {    
    gIndenter++;

    list<S_optionsItem>::const_iterator
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

ostream& operator<< (ostream& os, const S_optionsSubGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsGroup optionsGroup::create (
  string optionsGroupHelpHeader,
  string optionGroupShortName,
  string optionGroupLongName,
  string optionGroupDescription,
  S_optionsHandler
         optionsHandlerUplink)
{
  optionsGroup* o = new
    optionsGroup (
      optionsGroupHelpHeader,
      optionGroupShortName,
      optionGroupLongName,
      optionGroupDescription,
      optionsHandlerUplink);
  assert(o!=0);
  return o;
}

optionsGroup::optionsGroup (
  string optionsGroupHelpHeader,
  string optionGroupShortName,
  string optionGroupLongName,
  string optionGroupDescription,
  S_optionsHandler
         optionsHandlerUplink)
  : optionsElement (
      optionGroupShortName,
      optionGroupLongName,
      optionGroupDescription)
{
  fOptionsHandlerUplink = optionsHandlerUplink;
  
  fOptionsGroupHelpHeader = optionsGroupHelpHeader;
}

optionsGroup::~optionsGroup ()
{}

void optionsGroup::setOptionsHandlerUplink (
  S_optionsHandler optionsHandler)
{
  // sanity check
  msrAssert (
    optionsHandler != nullptr,
    "optionsHandler is null");

  // set the uplink
  fOptionsHandlerUplink =
    optionsHandler;
}

void optionsGroup::underlineHeader (ostream& os) const
{
  for (unsigned int i = 0; i < fOptionsGroupHelpHeader.size (); i++) {
    os << "-";
  } // for
  os <<
    endl;
}

void optionsGroup::registerOptionsGroupInHandler (
  S_optionsHandler optionsHandler)
{
  // sanity check
  msrAssert (
    optionsHandler != nullptr,
    "optionsHandler is null");

  // set options handler uplink
  setOptionsHandlerUplink (optionsHandler);

  // register options group in options handler
  optionsHandler->
    registerOptionsElementInHandler (this);

  for (
    list<S_optionsSubGroup>::const_iterator
      i = fOptionsGroupSubGroupsList.begin ();
    i != fOptionsGroupSubGroupsList.end ();
    i++) {
    // register the options sub group
    (*i)->
      registerOptionsSubGroupInHandler (
        optionsHandler);
  } // for
}

void  optionsGroup::appendOptionsSubGroup (
  S_optionsSubGroup optionsSubGroup)
{
  // sanity check
  msrAssert (
    optionsSubGroup != nullptr,
    "optionsSubGroup is null");

  // append options subgroup
  fOptionsGroupSubGroupsList.push_back (
    optionsSubGroup);

  // set options subgroup group uplink
  optionsSubGroup->
    setOptionsGroupUplink (this);
}

S_optionsElement optionsGroup::fetchOptionElement (
  string optiontElementName)
{
  S_optionsElement result;
  
  for (
    list<S_optionsSubGroup>::const_iterator
      i = fOptionsGroupSubGroupsList.begin ();
    i != fOptionsGroupSubGroupsList.end ();
    i++) {
    // search optiontElementName in the options group
    result =
      (*i)->fetchOptionElement (optiontElementName);
      
    if (result != 0) {
      break;
    }
  } // for

  return result;
}

S_optionsItem optionsGroup::handleOptionsItem (
  ostream&      os,
  S_optionsItem item)
{
  S_optionsItem result;
  
  os <<
    "---> Options item '" <<
    item <<
    "' is not handled" <<
    endl;

  return result;
}

void optionsGroup::handleOptionsItemValue (
  ostream&      os,
  S_optionsItem item,
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

void optionsGroup::checkOptionsConsistency ()
{}

void optionsGroup::print (ostream& os) const
{
  const int fieldWidth = 27;
  
  os <<
    "OptionsGroup:" <<
    endl;

  gIndenter++;

  optionsElement::printElementEssentials (
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
    
    list<S_optionsSubGroup>::const_iterator
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

void optionsGroup::printHelp (ostream& os) const
{
  // print the header and option names
  os <<
    fOptionsGroupHelpHeader <<
    " " <<
    optionsElementNamesBetweenParentheses () <<
    ":" <<
    endl;

  // underline the options group header
  underlineHeader (os);
  os <<
    endl;

  // print the description if any
  if (fOptionsElementDescription.size ()) {
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fOptionsElementDescription) <<
      endl;
    gIndenter--;

    os <<
      endl;  
  }
    
  // print the options subgroups
  if (fOptionsGroupSubGroupsList.size ()) {    
    gIndenter++;

    list<S_optionsSubGroup>::const_iterator
      iBegin = fOptionsGroupSubGroupsList.begin (),
      iEnd   = fOptionsGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup help
      (*i)->printHelp (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
      
    gIndenter--;
  }
}

void optionsGroup::printOptionsSubGroupForcedHelp (
  ostream&             os,
  S_optionsSubGroup targetOptionsSubGroup) const
{
  // print the header and option names
  os <<
    fOptionsGroupHelpHeader <<
    " " <<
    optionsElementNamesBetweenParentheses () <<
    ":" <<
    endl;

  // underline the options group header
  underlineHeader (os);
  os <<
    endl;

  // print the description if any
  if (fOptionsElementDescription.size ()) {
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fOptionsElementDescription) <<
      endl;
    gIndenter--;

    os <<
      endl;  
  }
    
  // print the target options subgroup
  if (fOptionsGroupSubGroupsList.size ()) {    
    gIndenter++;

    list<S_optionsSubGroup>::const_iterator
      iBegin = fOptionsGroupSubGroupsList.begin (),
      iEnd   = fOptionsGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_optionsSubGroup
        optionsSubGroup = (*i);
        
      if (optionsSubGroup == targetOptionsSubGroup) {
        // print the target options subgroup help
        optionsSubGroup->
          printOptionsSubGroupForcedHelp (
            os);
      }
      if (++i == iEnd) break;
      if (optionsSubGroup == targetOptionsSubGroup) {
        os <<
          endl;
      }
    } // for
      
    gIndenter--;
  }
}

void optionsGroup::printOptionsItemForcedHelp (
  ostream&             os,
  S_optionsSubGroup targetOptionsSubGroup,
  S_optionsItem     targetOptionsItem) const
{
  // print the header and option names
  os <<
    fOptionsGroupHelpHeader <<
    " " <<
    optionsElementNamesBetweenParentheses () <<
    ":" <<
    endl;

  // underline the options group header
  underlineHeader (os);
  os <<
    endl;

  // print the description if any
  if (fOptionsElementDescription.size ()) {
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fOptionsElementDescription) <<
      endl;
    gIndenter--;

    os <<
      endl;  
  }
    
  // print the target options subgroup
  if (fOptionsGroupSubGroupsList.size ()) {    
    gIndenter++;

    list<S_optionsSubGroup>::const_iterator
      iBegin = fOptionsGroupSubGroupsList.begin (),
      iEnd   = fOptionsGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_optionsSubGroup
        optionsSubGroup = (*i);
        
      if (optionsSubGroup == targetOptionsSubGroup) {
        // print the target options subgroup's
        // target options item's help
        (*i)->
          printOptionsItemForcedHelp (
            os,
            targetOptionsItem);
      }
      if (++i == iEnd) break;
      if (optionsSubGroup == targetOptionsSubGroup) {
        os <<
          endl;
      }
    } // for

    os <<
      endl;  
      
    gIndenter--;
  }
}

void optionsGroup::printHelpSummary (ostream& os) const
{
  // the description is the header of the information
  os <<
    fOptionsGroupHelpHeader <<
    " " <<
    optionsElementNamesBetweenParentheses () <<
    ":" <<
    endl;

  // underline the options group header
  underlineHeader (os);
  os <<
    endl;

  // print the description if any
  if (fOptionsElementDescription.size ()) {
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fOptionsElementDescription) <<
      endl;
    gIndenter--;

    os <<
      endl;  
  }
    
  // print the options subgroups
  if (fOptionsGroupSubGroupsList.size ()) {    
    gIndenter++;

    list<S_optionsSubGroup>::const_iterator
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
}

void optionsGroup::printSpecificSubGroupHelp (
  ostream& os,
  S_optionsSubGroup
           optionsSubGroup) const
{
  // the description is the header of the information
  os <<
    fOptionsGroupHelpHeader <<
    " " <<
    optionsElementNamesBetweenParentheses () <<
    ":" <<
    endl;

  // underline the options group header
  underlineHeader (os);
  os <<
    endl <<
    endl;

  // print the description if any
  if (fOptionsElementDescription.size ()) {
    gIndenter++;
    os <<
      gIndenter.indentMultiLineString (
        fOptionsElementDescription) <<
      endl;
    gIndenter--;

    os <<
      endl;  
  }
    
  // print the options subgroups
  if (fOptionsGroupSubGroupsList.size ()) {    
    gIndenter++;

    list<S_optionsSubGroup>::const_iterator
      iBegin = fOptionsGroupSubGroupsList.begin (),
      iEnd   = fOptionsGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup specific subgroup help
      (*i)->
        printSpecificSubGroupHelp (
          os, optionsSubGroup);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }
}

void optionsGroup::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // print the header
  os <<
    fOptionsGroupHelpHeader <<
    " " <<
    optionsElementNamesBetweenParentheses () <<
    ":" <<
    endl;

  // underline the options group header
  underlineHeader (os);
  os <<
    endl;

  // print the options subgroups values
  if (fOptionsGroupSubGroupsList.size ()) {    
    gIndenter++;

    list<S_optionsSubGroup>::const_iterator
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

ostream& operator<< (ostream& os, const S_optionsGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
/* JMI
S_optionsHandler optionsHandler::create (
  string           optionsHandlerHelpHeader,
  string           optionsHandlerValuesHeader,
  string           optionHandlerHelpShortName,
  string           optionHandlerHelpLongName,
  string           optionHandlerHelpSummaryShortName,
  string           optionHandlerHelpSummaryLongName,
  string           optionHandlerPreamble,
  string           optionHandlerDescription,
  indentedOstream& optionsHandlerLogIOstream)
{
  optionsHandler* o = new
    optionsHandler (
      optionsHandlerHelpHeader,
      optionsHandlerValuesHeader,
      optionHandlerHelpShortName,
      optionHandlerHelpLongName,
      optionHandlerHelpSummaryShortName,
      optionHandlerHelpSummaryLongName,
      optionHandlerPreamble,
      optionHandlerDescription,
      optionsHandlerLogIOstream);
  assert(o!=0);
  return o;
}
*/

optionsHandler::optionsHandler (
  string           optionsHandlerHelpHeader,
  string           optionsHandlerValuesHeader,
  string           optionHandlerHelpShortName,
  string           optionHandlerHelpLongName,
  string           optionHandlerHelpSummaryShortName,
  string           optionHandlerHelpSummaryLongName,
  string           optionHandlerPreamble,
  string           optionHandlerDescription,
  indentedOstream& optionsHandlerLogIOstream)
  : optionsElement (
      optionHandlerHelpShortName,
      optionHandlerHelpLongName,
      optionHandlerDescription),
    fOptionsHandlerLogIOstream (
      optionsHandlerLogIOstream)
{
  fOptionsHandlerHelpHeader =
    optionsHandlerHelpHeader;
    
  fOptionsHandlerValuesHeader =
    optionsHandlerValuesHeader;

  fOptionHandlerHelpSummaryShortName =
    optionHandlerHelpSummaryShortName;
  fOptionHandlerHelpSummaryLongName =
    optionHandlerHelpSummaryLongName;

  fOptionHandlerPreamble =
    optionHandlerPreamble;
    
  fMaximumSubGroupsHelpHeadersSize = 1;

  fMaximumShortNameWidth   = 1;
  fMaximumLongNameWidth    = 1;
  
  fMaximumDisplayNameWidth = 1;
}

optionsHandler::~optionsHandler ()
{}

void optionsHandler::registerOptionsHandlerInItself ()
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
    list<S_optionsGroup>::const_iterator
      i = fOptionsHandlerOptionsGroupsList.begin ();
    i != fOptionsHandlerOptionsGroupsList.end ();
    i++) {
    // register the options group
    (*i)->
      registerOptionsGroupInHandler (
        this);
  } // for
 // */
}

S_optionsElement optionsHandler::fetchOptionsElementFromMap (
  string optionsElementName) const
{
  S_optionsElement result;
  
  // is optionsItemName known in options elements map?
  map<string, S_optionsElement>::const_iterator
    it =
      fOptionsElementsMap.find (
        optionsElementName);
        
  if (it != fOptionsElementsMap.end ()) {
    // yes, optionsItemName it is unknown in the map    
    result = (*it).second;
  }    

  return result;
}

string optionsHandler::helpNamesBetweenParentheses () const
{
  stringstream s;
  
  s <<
    "(" <<
    optionsElementNames () <<
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

void optionsHandler::registerOptionsNamesInHandler (
  string              optionShortName,
  string              optionLongName,
  S_optionsElement optionsElement)
{
  int
    optionShortNameSize =
      optionShortName.size (),
    optionLongNameSize =
      optionLongName.size ();

  if (
    optionShortNameSize == 0
      &&
    optionLongNameSize == 0) {
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
    map<string, S_optionsElement>::iterator i =
      fOptionsElementsMap.begin ();
    i != fOptionsElementsMap.end ();
    i++) {
      
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
    
  // register optionsElement's names size
  if (optionLongNameSize) {
    fOptionsElementsMap [optionLongName] =
      optionsElement;
    
    if (optionLongNameSize > fMaximumLongNameWidth) {
      fMaximumLongNameWidth = optionLongNameSize;
    }
    
    if (optionLongNameSize > fMaximumDisplayNameWidth) {
      fMaximumDisplayNameWidth = optionLongNameSize;
    }
  }
  
  if (optionShortNameSize) {
    fOptionsElementsMap [optionShortName] =
      optionsElement;
    
    if (optionShortNameSize > fMaximumShortNameWidth) {
      fMaximumShortNameWidth = optionShortNameSize;
    }
    
    if (optionShortNameSize > fMaximumDisplayNameWidth) {
      fMaximumDisplayNameWidth = optionShortNameSize;
    }
  }
}

void optionsHandler::registerOptionsElementInHandler (
  S_optionsElement optionsElement)
{
  string
    optionShortName =
      optionsElement->getOptionsElementShortName (),
    optionLongName =
      optionsElement->getOptionsElementLongName ();

  // register the option names in handler
  registerOptionsNamesInHandler (
    optionShortName,
    optionLongName,
    optionsElement);
    
  if (
    // options subgroup?
    S_optionsSubGroup
      subGroup =
        dynamic_cast<optionsSubGroup*>(&(*optionsElement))
    ) {    

    string
      optionHelpHeader=
        subGroup->
          getOptionsSubGroupHelpHeader ();
    int
      optionHelpHeaderSize =
        optionHelpHeader.size ();
          
    // account for optionsSubGroup's header size
    if (optionHelpHeaderSize > fMaximumSubGroupsHelpHeadersSize) {
      fMaximumSubGroupsHelpHeadersSize =
        optionHelpHeaderSize;
    }
  }
}

void optionsHandler::print (ostream& os) const
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
    endl;

  if (fOptionsHandlerOptionsGroupsList.size ()) {
    os <<
      endl;
    
    gIndenter++;
    
    list<S_optionsGroup>::const_iterator
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

void optionsHandler::printHelp (ostream& os) const
{
  // print the options handler preamble
  os <<
    gIndenter.indentMultiLineString (
      fOptionHandlerPreamble) <<
      endl;

  // print the options handler help header and element names
  os <<
    fOptionsHandlerHelpHeader <<
    " " <<
    optionsElementNamesBetweenParentheses () <<
    ":" <<
    endl;
  
  // print the options handler description
  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fOptionsElementDescription) <<
      endl <<
    endl;
  gIndenter--;
    
  // print the options groups helps
  if (fOptionsHandlerOptionsGroupsList.size ()) {    
    gIndenter++;

    list<S_optionsGroup>::const_iterator
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
}

void optionsHandler::printHelpSummary (ostream& os) const
{
  // print the options handler preamble
  os <<
    gIndenter.indentMultiLineString (
      fOptionHandlerPreamble) <<
      endl;
    
  // print the options handler help header and element names
  os <<
    fOptionsHandlerHelpHeader <<
    " " <<
    optionsElementNamesBetweenParentheses () <<
    ":" <<
    endl;

  // print the options handler description
  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fOptionsElementDescription) <<
      endl <<
    endl;
  gIndenter--;
    
  // print the options groups help summaries
  if (fOptionsHandlerOptionsGroupsList.size ()) {    
    gIndenter++;

    list<S_optionsGroup>::const_iterator
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

void optionsHandler::printSpecificSubGroupHelp (
  ostream& os,
  S_optionsSubGroup
           optionsSubGroup) const
{
  // print the options handler help header and element names
  os <<
    fOptionsHandlerHelpHeader <<
    " " <<
    optionsElementNamesBetweenParentheses () <<
    ":" <<
    endl;

  // print the optons group subgroups specific help
  if (fOptionsHandlerOptionsGroupsList.size ()) {    
    gIndenter++;

    list<S_optionsGroup>::const_iterator
      iBegin = fOptionsHandlerOptionsGroupsList.begin (),
      iEnd   = fOptionsHandlerOptionsGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group specific subgroup help
      (*i)->
        printSpecificSubGroupHelp (
          os,
          optionsSubGroup);
      if (++i == iEnd) break;
      os << endl;
    } // for
      
    gIndenter--;
  }
}

void optionsHandler::printSpecificItemHelp (
  ostream& os,
  string   optionsItemName) const
{  
  // is optionsItemName known in options elements map?
  S_optionsElement
    optionsElement =
      fetchOptionsElementFromMap (
        optionsItemName);
                      
  if (! optionsElement) {
    // optionsItemName is is not well handled by this options handler
    stringstream s;

    s <<
      "option name '" << optionsItemName <<
      "' is not well handled in optionsHandler::printSpecificItemHelp ()";
      
    optionError (s.str ());
    exit (33);
  }

  else {
    // optionsItemName is known, let's handle it
    if (
      // options handler?
      S_optionsHandler
        handler =
          dynamic_cast<optionsHandler*>(&(*optionsElement))
      ) {
      // print the option handler help or help summary
      if (
        optionsItemName ==
          handler->
            getOptionHandlerHelpSummaryShortName ()
          ||
        optionsItemName ==
          handler->
            getOptionHandlerHelpSummaryLongName ()
         ) {
        handler->
          printHelpSummary (
            fOptionsHandlerLogIOstream);
      }
      else {
        handler->
          printHelp (
            fOptionsHandlerLogIOstream);
      }
      
      fOptionsHandlerLogIOstream <<
        endl;
    }
    
    else if (
      // options group?
      S_optionsGroup
        group =
          dynamic_cast<optionsGroup*>(&(*optionsElement))
      ) {
      // print the help
      fOptionsHandlerLogIOstream <<
        endl <<
        "--- Help for options item name '" <<
        optionsItemName <<
        "' for group \"" <<
        group->
          getOptionsGroupHelpHeader () <<
        "\" ---" <<
        endl <<
        endl;
        
      group->
        printHelp (
          fOptionsHandlerLogIOstream);
      
      fOptionsHandlerLogIOstream <<
        endl;
    }
    
    else if (
      // options subgroup?
      S_optionsSubGroup
        subGroup =
          dynamic_cast<optionsSubGroup*>(&(*optionsElement))
      ) {
      // get the options group uplink
      S_optionsGroup
        group =
          subGroup->
            getOptionsGroupUplink ();
          
      // print the help
      fOptionsHandlerLogIOstream <<
        endl <<
        "--- Help for options item name '" <<
        optionsItemName <<
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
          fOptionsHandlerLogIOstream,
          subGroup);
    }
    
    else if (
      // options item?
      S_optionsItem
        item =
          dynamic_cast<optionsItem*>(&(*optionsElement))
      ) {
      // get the options subgroup uplink
      S_optionsSubGroup
        subGroup =
          item->
            getOptionsSubGroupUplink ();
          
      // get the options group uplink
      S_optionsGroup
        group =
          subGroup->
            getOptionsGroupUplink ();

      // print the help
      fOptionsHandlerLogIOstream <<
        endl <<
        "--- Help for options item name '" <<
        optionsItemName <<
        "' in subgroup \"" <<
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
        printOptionsItemForcedHelp (
          fOptionsHandlerLogIOstream,
          subGroup,
          item);
    }
    
    else {
      stringstream s;

      s <<
        "cannot handle specific help about optionsItemName \"" <<
        optionsItemName <<
        "\"";
        
      optionError (s.str ());
      exit (33);
    }
  }
}

void optionsHandler::printAllOptionsValues (
  ostream& os) const
{
  // print the options handler values header
  os <<
    fOptionsHandlerValuesHeader <<
    " " <<
    optionsElementNamesBetweenParentheses () <<
    ":" <<
    endl;

  // print the options groups values
  if (fOptionsHandlerOptionsGroupsList.size ()) {
    os << endl;
    
    gIndenter++;

    list<S_optionsGroup>::const_iterator
      iBegin = fOptionsHandlerOptionsGroupsList.begin (),
      iEnd   = fOptionsHandlerOptionsGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group values
      (*i)->
        printOptionsValues (
          os, fMaximumDisplayNameWidth);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }
}

ostream& operator<< (ostream& os, const S_optionsHandler& elt)
{
  elt->print (os);
  return os;
}

void optionsHandler::appendOptionsGroup (
  S_optionsGroup optionsGroup)
{
  // sanity check
  msrAssert (
    optionsGroup != nullptr,
    "optionsGroup is null");

  // append the options group
  fOptionsHandlerOptionsGroupsList.push_back (
    optionsGroup);

  // set the uplink
  optionsGroup->
    setOptionsHandlerUplink (this);
  }

S_optionsElement optionsHandler::fetchOptionElement (
  string optiontElementName)
{
  S_optionsElement result;
  
  for (
    list<S_optionsGroup>::const_iterator
      i = fOptionsHandlerOptionsGroupsList.begin ();
    i != fOptionsHandlerOptionsGroupsList.end ();
    i++) {
    // search optiontElementName in the options group
    result =
      (*i)->fetchOptionElement (optiontElementName);
      
    if (result != 0) {
      break;
    }
  } // for

  return result;
}

const vector<string> optionsHandler::decipherOptionsAndArguments (
  int   argc,
  char* argv[])
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    // print the options elements map  
    fOptionsHandlerLogIOstream <<
      "Options elements map (" <<
      fOptionsElementsMap.size () <<
      " elements):" <<
      endl;
    if (fOptionsElementsMap.size ()) {
      gIndenter++;
      
      map<string, S_optionsElement>::const_iterator
        iBegin = fOptionsElementsMap.begin (),
        iEnd   = fOptionsElementsMap.end (),
        i      = iBegin;
      for ( ; ; ) {
        fOptionsHandlerLogIOstream <<
          (*i).first << "-->" <<
          endl;
          
        gIndenter++;
        
        (*i).second->
          printHeader (
            fOptionsHandlerLogIOstream);
            
        if (++i == iEnd) break;
        
        fOptionsHandlerLogIOstream <<
          endl;
          
        gIndenter--;
      } // for
      
      gIndenter--;
    }
    
    fOptionsHandlerLogIOstream <<
      endl;
  }
#endif

  // fetch program name
  fProgramName = string (argv [0]);
  
  fCommandLineWithShortOptions = fProgramName;
  fCommandLineWithLongOptions  = fProgramName;
  
  // decipher the command options and arguments
  int n = 1;
  
  while (true) { 
    if (argv [n] == 0)
      break;

    string currentElement = string (argv [n]);
    
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
      // print current element
      fOptionsHandlerLogIOstream <<
        "Command line element " << n <<
        ": " <<currentElement << " "<<
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
          fOptionsHandlerLogIOstream <<
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
        fOptionsHandlerLogIOstream <<
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
              fOptionsHandlerLogIOstream <<
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
              fOptionsHandlerLogIOstream <<
                "'" << currentOptionName << "' is a single-dashed option" <<
                endl;
            }
#endif
          }
        }
        
        else {
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
            fOptionsHandlerLogIOstream <<
              "'-' is the minimal single-dashed option" <<
              endl;
          }
#endif
        }

        // handle the options item name
        handleOptionsItemName (currentOptionName);
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

  unsigned int argumentsVectorSize =
    fArgumentsVector.size ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    // print the arguments vector
    fOptionsHandlerLogIOstream <<
      "Arguments vector (" <<
      argumentsVectorSize <<
      " elements):" <<
      endl;
  
    if (argumentsVectorSize) {
      gIndenter++;
      for (unsigned int i = 0; i < argumentsVectorSize; i++) {
        fOptionsHandlerLogIOstream <<
          fArgumentsVector [i] <<
          endl;
      } // for
      gIndenter--;
    }
  }
#endif

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

void optionsHandler::handleOptionsItemName (
  string optionsItemName)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fOptionsHandlerLogIOstream <<
      "==> handleOptionsItemName (), optionsItemName = \"" <<
      optionsItemName <<
      "\"" <<
      endl;
  }
#endif

  // is optionsItemName known in options elements map?
  S_optionsElement
    optionsElement =
      fetchOptionsElementFromMap (
        optionsItemName);

  if (! optionsElement) {
    // optionsItemName is is not well handled by this options handler
    printHelpSummary ();

    stringstream s;

    s <<
      "option name '" << optionsItemName <<
      "' is unknown, see help summary above";      

    optionError (s.str ());
    exit (6);
  }
  
  else {
    // optionsItemName is known, let's handle it
    fCommandOptionsElements.push_back (
      optionsElement);

    // determine option element names to be used,
    // in case one of them (short or long) is empty
    string
      shortName =
        optionsElement->getOptionsElementShortName (),
      longName =
        optionsElement->getOptionsElementLongName ();

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
      S_optionsHandler
        handler =
          dynamic_cast<optionsHandler*>(&(*optionsElement))
      ) {
      // print the option handler help or help summary
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
        fOptionsHandlerLogIOstream <<
          "==> optionsElement is of type 'optionsHandler'" <<
          endl;
      }
#endif
      
      if (
        optionsItemName ==
          handler->
            getOptionHandlerHelpSummaryShortName ()
          ||
        optionsItemName ==
          handler->
            getOptionHandlerHelpSummaryLongName ()
         ) {
        handler->
          printHelpSummary (
            fOptionsHandlerLogIOstream);
      }
      else {
        handler->
          printHelp (
            fOptionsHandlerLogIOstream);
      }
      
      fOptionsHandlerLogIOstream <<
        endl;
    }
    
    else if (
      // options group?
      S_optionsGroup
        group =
          dynamic_cast<optionsGroup*>(&(*optionsElement))
      ) {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
        fOptionsHandlerLogIOstream <<
          "==> optionsElement is of type 'optionsGroup'" <<
          endl;
      }
#endif

      // print the help
      fOptionsHandlerLogIOstream <<
        endl <<
        "--- Help for group \"" <<
        group->
          getOptionsGroupHelpHeader () <<
        "\" ---" <<
        endl <<
        endl;
        
      group->
        printHelp (
          fOptionsHandlerLogIOstream);
    }
    
    else if (
      // options subgroup?
      S_optionsSubGroup
        subGroup =
          dynamic_cast<optionsSubGroup*>(&(*optionsElement))
      ) {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
        fOptionsHandlerLogIOstream <<
          "==> optionsElement is of type 'optionsSubGroup'" <<
          endl;
      }
#endif

      // get the options group uplink
      S_optionsGroup
        group =
          subGroup->
            getOptionsGroupUplink ();
          
      // print the help
      fOptionsHandlerLogIOstream <<
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
          fOptionsHandlerLogIOstream,
          subGroup);
    }
    
    else if (
      // options item?
      S_optionsItem
        item =
          dynamic_cast<optionsItem*>(&(*optionsElement))
      ) {
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
        fOptionsHandlerLogIOstream <<
          "==> optionsElement is of type 'optionsItem'" <<
          endl;
      }
#endif

      if (
        // help usage item?
        S_optionsHelpUsageItem
          helpUsageItem =
            dynamic_cast<optionsHelpUsageItem*>(&(*optionsElement))
        ) {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
          fOptionsHandlerLogIOstream <<
            "==> optionsElement is of type 'optionsHelpUsageItem'" <<
            endl;
        }
#endif
  
        // handle it at once
        helpUsageItem->
          printHelpUsage (
            fOptionsHandlerLogIOstream);

        // exit
        exit (0);
      }
      
      else if (
        // help summary item?
        S_optionsHelpSummaryItem
          helpSummaryItem =
            dynamic_cast<optionsHelpSummaryItem*>(&(*optionsElement))
        ) {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
          fOptionsHandlerLogIOstream <<
            "==> optionsElement is of type 'optionsHelpSummaryItem'" <<
            endl;
        }
#endif
  
        // handle it at once
        printHelpSummary (
          fOptionsHandlerLogIOstream);

        // exit
        exit (0);
      }
      
      else if (
        // combined items item?
        S_optionsCombinedItemsItem
          combinedItemsItem =
            dynamic_cast<optionsCombinedItemsItem*>(&(*optionsElement))
        ) {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
          fOptionsHandlerLogIOstream <<
            "==> optionsElement is of type 'optionsCombinedItemsItem'" <<
            endl;
        }
#endif
  
        // handle it at once
        combinedItemsItem->
          setCombinedItemsVariablesValue (true);
      }
      
      else if (
        // boolean item?
        S_optionsBooleanItem
          booleanItem =
            dynamic_cast<optionsBooleanItem*>(&(*optionsElement))
        ) {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
          fOptionsHandlerLogIOstream <<
            "==> optionsElement is of type 'optionsBooleanItem'" <<
            endl;
        }
#endif
  
        // handle it at once
        booleanItem->
          setBooleanItemVariableValue (true);              
      }
      
      else if (
        // two booleans item?
        S_optionsTwoBooleansItem
          twoBooleansItem =
            dynamic_cast<optionsTwoBooleansItem*>(&(*optionsElement))
        ) {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
          fOptionsHandlerLogIOstream <<
            "==> optionsElement is of type 'optionsTwoBooleansItem'" <<
            endl;
        }
#endif
  
        // handle it at once
        twoBooleansItem->
          setTwoBooleansItemVariableValue (true);              
      }
      
      else if (
        // three booleans item?
        S_optionsThreeBooleansItem
          threeBooleansItem =
            dynamic_cast<optionsThreeBooleansItem*>(&(*optionsElement))
        ) {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
          fOptionsHandlerLogIOstream <<
            "==> optionsElement is of type 'optionsThreeBooleansItem'" <<
            endl;
        }
#endif
  
        // handle it at once
        threeBooleansItem->
          setThreeBooleansItemVariableValue (true);              
      }
      
      else if (
        // item help item?
        S_optionsItemHelpItem
          itemHelpItem =
            dynamic_cast<optionsItemHelpItem*>(&(*optionsElement))
        ) {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
          fOptionsHandlerLogIOstream <<
            "==> optionsElement is of type 'optionsItemHelpItem'" <<
            endl;
        }
#endif
  
        // wait until the value is met
        fPendingOptionsItem = itemHelpItem;
      }
      
      else if (
        // integer item?
        S_optionsIntegerItem
          integerItem =
            dynamic_cast<optionsIntegerItem*>(&(*optionsElement))
        ) {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
          fOptionsHandlerLogIOstream <<
            "==> optionsElement is of type 'optionsIntegerItem'" <<
            endl;
        }
#endif
  
        // wait until the value is met
        fPendingOptionsItem = integerItem;
      }
      
      else if (
        // float item?
        S_optionsFloatItem
          floatItem =
            dynamic_cast<optionsFloatItem*>(&(*optionsElement))
        ) {              
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
          fOptionsHandlerLogIOstream <<
            "==> optionsElement is of type 'optionsFloatItem'" <<
            endl;
        }
#endif
  
        // wait until the value is met
        fPendingOptionsItem = floatItem;
      }
      
      else if (
        // string item?
        S_optionsStringItem
          stringItem =
            dynamic_cast<optionsStringItem*>(&(*optionsElement))
        ) {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
          fOptionsHandlerLogIOstream <<
            "==> optionsElement is of type 'optionsStringItem'" <<
            endl;
        }
#endif
  
        // wait until the value is met
        fPendingOptionsItem = stringItem;
      }
      
      else if (
        // rational item?
        S_optionsRationalItem
          rationalItem =
            dynamic_cast<optionsRationalItem*>(&(*optionsElement))
        ) {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
          fOptionsHandlerLogIOstream <<
            "==> optionsElement is of type 'optionsRationalItem'" <<
            endl;
        }
#endif
  
        // wait until the value is met
        fPendingOptionsItem = rationalItem;
      }

      else if (
        // numbers set item?
        S_optionsNumbersSetItem
          numbersSetItem =
            dynamic_cast<optionsNumbersSetItem*>(&(*optionsElement))
        ) {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
          fOptionsHandlerLogIOstream <<
            "==> optionsElement is of type 'optionsNumbersSetItem'" <<
            endl;
        }
#endif
  
        // wait until the value is met
        fPendingOptionsItem = numbersSetItem;
      }

      else {
        // optionsElement is of another type,
        // let the optionsGroup handle it

#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
          fOptionsHandlerLogIOstream <<
            "==> fPendingOptionsItem is of another type" <<
            ", let the optionsGroup handle it" <<
            endl;
        }
#endif
      
        S_optionsGroup
          group =
            item->
              getOptionsSubGroupUplink ()->
                getOptionsGroupUplink ();

        fPendingOptionsItem =
          group->
            handleOptionsItem (
              fOptionsHandlerLogIOstream,
              item);
      }
    }

    else {
      stringstream s;
  
      s <<
        "INTERNAL ERROR: option name '" << optionsItemName <<
        "' cannot be handled";      
  
      optionError (s.str ());
      exit (7);
    }
  }
}

void optionsHandler::handleOptionsItemValueOrArgument (
  string theString)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    fOptionsHandlerLogIOstream <<
      "==> handleOptionsItemValueOrArgument ()" <<
      endl;
      
    gIndenter++;
    
    fOptionsHandlerLogIOstream <<
      "fPendingOptionsItem:" <<
      endl;
      
    gIndenter++;
    if (fPendingOptionsItem) {
      fOptionsHandlerLogIOstream <<
        fPendingOptionsItem;
    }
    else {
      fOptionsHandlerLogIOstream <<
        "null" <<
        endl;
    }
    gIndenter--;
    
    fOptionsHandlerLogIOstream <<
      "theString:" <<
      endl;
      
    gIndenter++;
    fOptionsHandlerLogIOstream <<
      " \"" <<
      theString <<
      "\"" <<
      endl;
    gIndenter--;

    gIndenter--;
  }
#endif

  if (fPendingOptionsItem) {
    // theString is the value for the pending options item

    if (
      // item help item?
      S_optionsItemHelpItem
        itemHelpItem =
          dynamic_cast<optionsItemHelpItem*>(&(*fPendingOptionsItem))
      ) {
      // handle the option item
      printSpecificItemHelp (
        fOptionsHandlerLogIOstream,
        theString);

      fPendingOptionsItem = nullptr;

      // exit
      exit (23);
      }
    
    else if (
      // integer item?
      S_optionsIntegerItem
        integerItem =
          dynamic_cast<optionsIntegerItem*>(&(*fPendingOptionsItem))
      ) {
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

      fPendingOptionsItem = nullptr;
      }
    
    else if (
      // float item?
      S_optionsFloatItem
        floatItem =
          dynamic_cast<optionsFloatItem*>(&(*fPendingOptionsItem))
      ) {              
      // handle the option item
      float floatValue;
      {
        stringstream s;
        s << theString;
        s >> floatValue;
      }
      
      floatItem->
        setFloatItemVariableValue (
          floatValue);

      fPendingOptionsItem = nullptr;
    }
    
    else if (
      // string item?
      S_optionsStringItem
        stringItem =
          dynamic_cast<optionsStringItem*>(&(*fPendingOptionsItem))
      ) {
      // handle the option item
      stringItem->
        setStringItemVariableValue (
          theString);

      fPendingOptionsItem = nullptr;
    }
    
    else if (
      // rational item?
      S_optionsRationalItem
        rationalItem =
          dynamic_cast<optionsRationalItem*>(&(*fPendingOptionsItem))
      ) {
      // theString contains the fraction:
      // decipher it to extract numerator and denominator values
      
      string regularExpression (
        "[[:space:]]*([[:digit:]]+)[[:space:]]*"
        "/"
        "[[:space:]]*([[:digit:]]+)[[:space:]]*");
        
      regex  e (regularExpression);
      smatch sm;

      regex_match (theString, sm, e);

      if (sm.size ()) {
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
          fOptionsHandlerLogIOstream <<
            "There are " << sm.size () << " matches" <<
            " for rational string '" << theString <<
            "' with regex '" << regularExpression <<
            "'" <<
            endl;
       
          for (unsigned i = 0; i < sm.size (); ++i) {
            fOptionsHandlerLogIOstream <<
              "[" << sm [i] << "] ";
          } // for
          
          fOptionsHandlerLogIOstream <<
            endl;
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
          fOptionsHandlerLogIOstream,
          rationalItem->
            getOptionsSubGroupUplink ());
            
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
        fOptionsHandlerLogIOstream << // JMI
          "rationalValue = " <<
          rationalValue <<
          endl;
      }
#endif

      rationalItem->
        setRationalItemVariableValue (
          rationalValue);

      fPendingOptionsItem = nullptr;
    }

    else if (
      // numbers set item?
      S_optionsNumbersSetItem
        numbersSetItem =
          dynamic_cast<optionsNumbersSetItem*>(&(*fPendingOptionsItem))
      ) {
      // theString contains the set specification,
      // decipher it
      numbersSetItem->
        setNumbersSetItemVariableValue (
          decipherNumbersSetSpecification (
            theString, false) // 'true' to debug it
          );

      fPendingOptionsItem = nullptr;
    }
    
    else {
      // fPendingOptionsItem is of another type,
      // let the optionsGroup handle it

#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
        fOptionsHandlerLogIOstream <<
          "==> fPendingOptionsItem is of another type" <<
          ", let the optionsGroup handle it" <<
          endl;
      }
#endif
      
      S_optionsGroup
        group =
          fPendingOptionsItem->
            getOptionsSubGroupUplink ()->
              getOptionsGroupUplink ();

      group->
        handleOptionsItemValue (
          fOptionsHandlerLogIOstream,
          fPendingOptionsItem,
          theString);

      fPendingOptionsItem = nullptr;
    }
  }

  else {
    // theString is an argument
    fArgumentsVector.push_back (theString);
  }
}


}
