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

#include <climits>      /* INT_MIN, INT_MAX */
#include <iomanip>      // setw, set::precision, ...

#include <regex>

#include "version.h"
#include "utilities.h"

#include "optionsHandling.h"


using namespace std;

namespace MusicXML2 
{

#define TRACE_OPTIONS 0

#define OPTIONS_ELEMENTS_IDTR_OFFSET 3
  // indent a bit more for readability

#define FIELD_WIDTH 40

//______________________________________________________________________________
S_msrOptionsElement msrOptionsElement::create (
  string optionsElementShortName,
  string optionsElementLongName,
  string optionsElementDescription)
{
  msrOptionsElement* o = new
    msrOptionsElement (
      optionsElementShortName,
      optionsElementLongName,
      optionsElementDescription);
  assert(o!=0);
  return o;
}

msrOptionsElement::msrOptionsElement (
  string optionsElementShortName,
  string optionsElementLongName,
  string optionsElementDescription)
{
  fOptionsElementShortName   = optionsElementShortName;  
  fOptionsElementLongName    = optionsElementLongName;  
  fOptionsElementDescription = optionsElementDescription;

  fOptionsElementIsHidden    = false;
}

msrOptionsElement::~msrOptionsElement()
{}

S_msrOptionsElement msrOptionsElement::fetchOptionElement (
  string optiontElementName)
{
  S_msrOptionsElement result;

  if (
    optiontElementName == fOptionsElementShortName
     ||
    optiontElementName == fOptionsElementLongName) {
    result = this;
  }
  
  return result;
}

string msrOptionsElement::optionsElementNames () const
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

string msrOptionsElement::optionsElementNamesInColumns (
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

string msrOptionsElement::optionsElementNamesBetweenParentheses () const
{
  stringstream s;

  s <<
    "(" <<
    optionsElementNames () <<
    ")";
  
  return s.str ();
}

string msrOptionsElement::optionsElementNamesInColumnsBetweenParentheses (
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

void msrOptionsElement::printHeader (ostream& os) const
{
  os <<
    "-" << fOptionsElementShortName <<
    endl <<
    "-" << fOptionsElementLongName <<
    endl;

  // indent a bit more for readability
  gIdtr.increment (OPTIONS_ELEMENTS_IDTR_OFFSET);
  
  os <<
    gIdtr.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;

  gIdtr.decrement (OPTIONS_ELEMENTS_IDTR_OFFSET);
}

void msrOptionsElement::printElementEssentials (
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

void msrOptionsElement::print (ostream& os) const
{
  os <<
    "??? msrOptionsElement ???" <<
    endl;

  printElementEssentials (os, 35);  
}

void msrOptionsElement::printHelp (ostream& os) const
{
  os <<
    optionsElementNames () <<
    endl <<
    endl;

  // indent a bit more for readability
  gIdtr.increment (OPTIONS_ELEMENTS_IDTR_OFFSET);
  
  os <<
    gIdtr.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;

  gIdtr.decrement (OPTIONS_ELEMENTS_IDTR_OFFSET);
}

ostream& operator<< (ostream& os, const S_msrOptionsElement& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOptionsItem msrOptionsItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription)
{
  msrOptionsItem* o = new
    msrOptionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription);
  assert(o!=0);
  return o;
}

msrOptionsItem::msrOptionsItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription)
  : msrOptionsElement (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription)
{
}
    
msrOptionsItem::~msrOptionsItem()
{}

void msrOptionsItem::registerOptionsItemInHandler (
  S_msrOptionsHandler optionsHandler)
{
  optionsHandler->
    registerOptionsElementInHandler (this);
}

void msrOptionsItem::print (ostream& os) const
{
  const int fieldWidth = 19;
  
  os <<
    "OptionsItem ???:" <<
      endl;

  gIdtr++;

  msrOptionsElement::printElementEssentials (
    os, fieldWidth);
}

void msrOptionsItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os <<
    "OptionsItem values ???:" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrOptionsItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOptionsVersionItem msrOptionsVersionItem::create (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
{
  msrOptionsVersionItem* o = new
    msrOptionsVersionItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription);
  assert(o!=0);
  return o;
}

msrOptionsVersionItem::msrOptionsVersionItem (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription)
  : msrOptionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription)
{}

msrOptionsVersionItem::~msrOptionsVersionItem()
{}

void msrOptionsVersionItem::print (ostream& os) const
{
  const int fieldWidth = FIELD_WIDTH;
  
  os <<
    "OptionsVersionItem:" <<
    endl;

  gIdtr++;

  msrOptionsElement::printElementEssentials (
    os, fieldWidth);

  gIdtr++;
  os <<
    gIdtr.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;
  gIdtr--;

  gIdtr--;
}

void msrOptionsVersionItem::printVersion (ostream& os) const
{  
  os <<
    "xml2lilypond" " "<<
    currentVersionNumber () <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrOptionsVersionItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOptionsBooleanItem msrOptionsBooleanItem::create (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription,
  string optionsBooleanItemVariableDisplayName,
  bool&  optionsBooleanItemVariable)
{
  msrOptionsBooleanItem* o = new
    msrOptionsBooleanItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsBooleanItemVariableDisplayName,
      optionsBooleanItemVariable);
  assert(o!=0);
  return o;
}

msrOptionsBooleanItem::msrOptionsBooleanItem (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription,
  string optionsBooleanItemVariableDisplayName,
  bool&  optionsBooleanItemVariable)
  : msrOptionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription),
    fOptionsBooleanItemVariableDisplayName (
      optionsBooleanItemVariableDisplayName),
    fOptionsBooleanItemVariable (
      optionsBooleanItemVariable)
{}

msrOptionsBooleanItem::~msrOptionsBooleanItem()
{}

void msrOptionsBooleanItem::print (ostream& os) const
{
  const int fieldWidth = FIELD_WIDTH;
  
  os <<
    "OptionsBooleanItem:" <<
    endl;

  gIdtr++;

  msrOptionsElement::printElementEssentials (
    os, fieldWidth);

  gIdtr++;
  os <<
    gIdtr.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;
  gIdtr--;

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

  gIdtr--;
}

void msrOptionsBooleanItem::printOptionsValues (
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

ostream& operator<< (ostream& os, const S_msrOptionsBooleanItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOptionsTwoBooleansItem msrOptionsTwoBooleansItem::create (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription,
  string optionsTwoBooleansItemVariableDisplayName,
  bool&  optionsTwoBooleansItemVariable,
  bool&  optionsTwoBooleansItemSecondaryVariable)
{
  msrOptionsTwoBooleansItem* o = new
    msrOptionsTwoBooleansItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsTwoBooleansItemVariableDisplayName,
      optionsTwoBooleansItemVariable,
      optionsTwoBooleansItemSecondaryVariable);
  assert(o!=0);
  return o;
}

msrOptionsTwoBooleansItem::msrOptionsTwoBooleansItem (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription,
  string optionsTwoBooleansItemVariableDisplayName,
  bool&  optionsTwoBooleansItemVariable,
  bool&  optionsTwoBooleansItemSecondaryVariable)
  : msrOptionsItem (
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

msrOptionsTwoBooleansItem::~msrOptionsTwoBooleansItem()
{}

void msrOptionsTwoBooleansItem::print (ostream& os) const
{
  const int fieldWidth = FIELD_WIDTH;
  
  os <<
    "OptionsTwoBooleansItem:" <<
    endl;

  gIdtr++;

  msrOptionsElement::printElementEssentials (
    os, fieldWidth);

  gIdtr++;
  os <<
    gIdtr.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;
  gIdtr--;

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

  gIdtr--;
}

void msrOptionsTwoBooleansItem::printOptionsValues (
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

ostream& operator<< (ostream& os, const S_msrOptionsTwoBooleansItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOptionsValuedItem msrOptionsValuedItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification)
{
  msrOptionsValuedItem* o = new
    msrOptionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification);
  assert(o!=0);
  return o;
}

msrOptionsValuedItem::msrOptionsValuedItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification)
  : msrOptionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription)
{
  fOptionsValueSpecification = optionsValueSpecification;
}
    
msrOptionsValuedItem::~msrOptionsValuedItem()
{}

string msrOptionsValuedItem::optionsItemKindAsString (
  msrOptionsValuedItem::msrOptionsValuedItemKind optionsItemKind)
{
  string result;
  
  switch (optionsItemKind) {
    case msrOptionsValuedItem::kOptionsItemHasNoArgument:
      result = "OptionsItemHasNoArgument";
      break;
    case msrOptionsValuedItem::kOptionsItemHasARequiredArgument:
      result = "OptionsItemHasARequiredArgument";
      break;
    case msrOptionsValuedItem::kOptionsItemHasAnOptionsArgument:
      result = "OptionsItemHasAnOptionsArgument";
      break;
  } // switch

  return result;
}  

void msrOptionsValuedItem::registerOptionsItemInHandler (
  S_msrOptionsHandler optionsHandler)
{
  optionsHandler->
    registerOptionsElementInHandler (this);
}

void msrOptionsValuedItem::printValuedItemEssentials (
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

void msrOptionsValuedItem::print (ostream& os) const
{
  const int fieldWidth = 19;
  
  os <<
    "OptionsValuedItem ???:" <<
    endl;

  gIdtr++;

  printValuedItemEssentials (
    os, fieldWidth);
}

void msrOptionsValuedItem::printHelp (ostream& os) const
{
  os <<
    optionsElementNames () <<
    " " <<
    fOptionsValueSpecification <<
    endl <<
    endl;

  // indent a bit more for readability
  gIdtr.increment (OPTIONS_ELEMENTS_IDTR_OFFSET);
  
  os <<
    gIdtr.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;

  gIdtr.decrement (OPTIONS_ELEMENTS_IDTR_OFFSET);
}

void msrOptionsValuedItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os <<
    "OptionsItem values ???:" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrOptionsValuedItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOptionsItemHelpItem msrOptionsItemHelpItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification)
{
  msrOptionsItemHelpItem* o = new
    msrOptionsItemHelpItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification);
  assert(o!=0);
  return o;
}

msrOptionsItemHelpItem::msrOptionsItemHelpItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification)
  : msrOptionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification)
{}

msrOptionsItemHelpItem::~msrOptionsItemHelpItem()
{}

void msrOptionsItemHelpItem::print (ostream& os) const
{
  const int fieldWidth = FIELD_WIDTH;
  
  os <<
    "OptionsItemHelpItem:" <<
    endl;

  gIdtr++;

  msrOptionsItemHelpItem::printValuedItemEssentials (
    os, fieldWidth);

  gIdtr--;
}

ostream& operator<< (ostream& os, const S_msrOptionsItemHelpItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOptionsIntegerItem msrOptionsIntegerItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsIntegerItemVariableDisplayName,
  int&               optionsIntegerItemVariable)
{
  msrOptionsIntegerItem* o = new
    msrOptionsIntegerItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsIntegerItemVariableDisplayName,
      optionsIntegerItemVariable);
  assert(o!=0);
  return o;
}

msrOptionsIntegerItem::msrOptionsIntegerItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsIntegerItemVariableDisplayName,
  int&               optionsIntegerItemVariable)
  : msrOptionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsIntegerItemVariableDisplayName (
      optionsIntegerItemVariableDisplayName),
    fOptionsIntegerItemVariable (
      optionsIntegerItemVariable)
{}

msrOptionsIntegerItem::~msrOptionsIntegerItem()
{}

void msrOptionsIntegerItem::print (ostream& os) const
{
  const int fieldWidth = FIELD_WIDTH;
  
  os <<
    "OptionsIntegerItem:" <<
    endl;

  gIdtr++;

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

  gIdtr--;
}

void msrOptionsIntegerItem::printOptionsValues (
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

ostream& operator<< (ostream& os, const S_msrOptionsIntegerItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOptionsFloatItem msrOptionsFloatItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsFloatItemVariableDisplayName,
  float&             optionsFloatItemVariable)
{
  msrOptionsFloatItem* o = new
    msrOptionsFloatItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsFloatItemVariableDisplayName,
      optionsFloatItemVariable);
  assert(o!=0);
  return o;
}

msrOptionsFloatItem::msrOptionsFloatItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsFloatItemVariableDisplayName,
  float&             optionsFloatItemVariable)
  : msrOptionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsFloatItemVariableDisplayName (
      optionsFloatItemVariableDisplayName),
    fOptionsFloatItemVariable (
      optionsFloatItemVariable)
{}

msrOptionsFloatItem::~msrOptionsFloatItem()
{}

void msrOptionsFloatItem::print (ostream& os) const
{
  const int fieldWidth = FIELD_WIDTH;
  
  os <<
    "OptionsFloatItem:" <<
    endl;

  gIdtr++;

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

  gIdtr--;
}

void msrOptionsFloatItem::printOptionsValues (
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

ostream& operator<< (ostream& os, const S_msrOptionsFloatItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOptionsStringItem msrOptionsStringItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsStringItemVariableDisplayName,
  string&            optionsStringItemVariable)
{
  msrOptionsStringItem* o = new
    msrOptionsStringItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsStringItemVariableDisplayName,
      optionsStringItemVariable);
  assert(o!=0);
  return o;
}

msrOptionsStringItem::msrOptionsStringItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsStringItemVariableDisplayName,
  string&            optionsStringItemVariable)
  : msrOptionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsStringItemVariableDisplayName (
      optionsStringItemVariableDisplayName),
    fOptionsStringItemVariable (
      optionsStringItemVariable)
{}

msrOptionsStringItem::~msrOptionsStringItem()
{}

void msrOptionsStringItem::print (ostream& os) const
{
  const int fieldWidth = FIELD_WIDTH;
  
  os <<
    "OptionsStringItem:" <<
    endl;

  gIdtr++;

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
}

void msrOptionsStringItem::printOptionsValues (
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

ostream& operator<< (ostream& os, const S_msrOptionsStringItem& elt)
{
  os <<
    "OptionsStringItem:" <<
    endl;
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOptionsRationalItem msrOptionsRationalItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsRationalItemVariableDisplayName,
  rational&          optionsRationalItemVariable)
{
  msrOptionsRationalItem* o = new
    msrOptionsRationalItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsRationalItemVariableDisplayName,
      optionsRationalItemVariable);
  assert(o!=0);
  return o;
}

msrOptionsRationalItem::msrOptionsRationalItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsRationalItemVariableDisplayName,
  rational&          optionsRationalItemVariable)
  : msrOptionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsRationalItemVariableDisplayName (
      optionsRationalItemVariableDisplayName),
    fOptionsRationalItemVariable (
      optionsRationalItemVariable)
{}

msrOptionsRationalItem::~msrOptionsRationalItem()
{}

void msrOptionsRationalItem::print (ostream& os) const
{
  const int fieldWidth = FIELD_WIDTH;
  
  os <<
    "OptionsRationalItem:" <<
    endl;

  gIdtr++;

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
}

void msrOptionsRationalItem::printOptionsValues (
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

ostream& operator<< (ostream& os, const S_msrOptionsRationalItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOptionsNumbersSetItem msrOptionsNumbersSetItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsNumbersSetItemVariableDisplayName,
  set<int>&          optionsNumbersSetItemVariable)
{
  msrOptionsNumbersSetItem* o = new
    msrOptionsNumbersSetItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsNumbersSetItemVariableDisplayName,
      optionsNumbersSetItemVariable);
  assert(o!=0);
  return o;
}

msrOptionsNumbersSetItem::msrOptionsNumbersSetItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsNumbersSetItemVariableDisplayName,
  set<int>&          optionsNumbersSetItemVariable)
  : msrOptionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsNumbersSetItemVariableDisplayName (
      optionsNumbersSetItemVariableDisplayName),
    fOptionsNumbersSetItemVariable (
      optionsNumbersSetItemVariable)
{}

msrOptionsNumbersSetItem::~msrOptionsNumbersSetItem()
{}

void msrOptionsNumbersSetItem::print (ostream& os) const
{
  const int fieldWidth = FIELD_WIDTH;
  
  os <<
    "OptionsNumbersSetItem:" <<
    endl;

  gIdtr++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsNumbersSetItemVariableDisplayName" << " : " <<
    fOptionsNumbersSetItemVariableDisplayName <<
    setw (fieldWidth) <<
    "fOptionsNumbersSetItemVariable" << " : " <<
    endl;

    if (fOptionsNumbersSetItemVariable.empty ()) {
      os <<
        "none";
    }
      
    else {
      os <<
        "'";
        
      set<int>::const_iterator
        iBegin = fOptionsNumbersSetItemVariable.begin(),
        iEnd   = fOptionsNumbersSetItemVariable.end(),
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

void msrOptionsNumbersSetItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsNumbersSetItemVariableDisplayName <<
    " : ";

  if (fOptionsNumbersSetItemVariable.empty ()) {
    os <<
      "none";
  }
    
  else {
    os <<
      "'";
      
    set<int>::const_iterator
      iBegin = fOptionsNumbersSetItemVariable.begin(),
      iEnd   = fOptionsNumbersSetItemVariable.end(),
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

ostream& operator<< (ostream& os, const S_msrOptionsNumbersSetItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOptionsPitchesLanguageItem msrOptionsPitchesLanguageItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsPitchesLanguageItemVariableDisplayName,
  msrQuarterTonesPitchesLanguage&
                     optionsPitchesLanguageItemVariable)
{
  msrOptionsPitchesLanguageItem* o = new
    msrOptionsPitchesLanguageItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsPitchesLanguageItemVariableDisplayName,
      optionsPitchesLanguageItemVariable);
  assert(o!=0);
  return o;
}

msrOptionsPitchesLanguageItem::msrOptionsPitchesLanguageItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsPitchesLanguageItemVariableDisplayName,
  msrQuarterTonesPitchesLanguage&
                     optionsPitchesLanguageItemVariable)
  : msrOptionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsPitchesLanguageItemVariableDisplayName (
      optionsPitchesLanguageItemVariableDisplayName),
    fOptionsPitchesLanguageItemVariable (
      optionsPitchesLanguageItemVariable)
{}

msrOptionsPitchesLanguageItem::~msrOptionsPitchesLanguageItem()
{}

void msrOptionsPitchesLanguageItem::print (ostream& os) const
{
  const int fieldWidth = FIELD_WIDTH;
  
  os <<
    "OptionsPitchesLanguageItem:" <<
    endl;

  gIdtr++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsPitchesLanguageItemVariableDisplayName" << " : " <<
    fOptionsPitchesLanguageItemVariableDisplayName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsPitchesLanguageItemVariable" << " : \"" <<
    msrQuarterTonesPitchesLanguageAsString (
      fOptionsPitchesLanguageItemVariable) <<
      "\"" <<
    endl;
}

void msrOptionsPitchesLanguageItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsPitchesLanguageItemVariableDisplayName <<
    " : \"" <<
    msrQuarterTonesPitchesLanguageAsString (
      fOptionsPitchesLanguageItemVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrOptionsPitchesLanguageItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOptionsAccidentalStyleItem msrOptionsAccidentalStyleItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsAccidentalStyleItemVariableDisplayName,
  lpsrAccidentalStyle&
                     optionsAccidentalStyleItemVariable)
{
  msrOptionsAccidentalStyleItem* o = new
    msrOptionsAccidentalStyleItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsAccidentalStyleItemVariableDisplayName,
      optionsAccidentalStyleItemVariable);
  assert(o!=0);
  return o;
}

msrOptionsAccidentalStyleItem::msrOptionsAccidentalStyleItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsAccidentalStyleItemVariableDisplayName,
  lpsrAccidentalStyle&
                     optionsAccidentalStyleItemVariable)
  : msrOptionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsAccidentalStyleItemVariableDisplayName (
      optionsAccidentalStyleItemVariableDisplayName),
    fOptionsAccidentalStyleItemVariable (
      optionsAccidentalStyleItemVariable)
{}

msrOptionsAccidentalStyleItem::~msrOptionsAccidentalStyleItem()
{}

void msrOptionsAccidentalStyleItem::print (ostream& os) const
{
  const int fieldWidth = FIELD_WIDTH;
  
  os <<
    "OptionsAccidentalStyleItem:" <<
    endl;

  gIdtr++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsAccidentalStyleItemVariableDisplayName" << " : " <<
    fOptionsAccidentalStyleItemVariableDisplayName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsAccidentalStyleItemVariable" << " : \"" <<
    lpsrAccidentalStyleAsString (
      fOptionsAccidentalStyleItemVariable) <<
      "\"" <<
    endl;
}

void msrOptionsAccidentalStyleItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsAccidentalStyleItemVariableDisplayName <<
    " : \"" <<
    lpsrAccidentalStyleAsString (
      fOptionsAccidentalStyleItemVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrOptionsAccidentalStyleItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOptionsChordsLanguageItem msrOptionsChordsLanguageItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsChordsLanguageItemVariableDisplayName,
  lpsrChordsLanguage&
                     optionsChordsLanguageItemVariable)
{
  msrOptionsChordsLanguageItem* o = new
    msrOptionsChordsLanguageItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsChordsLanguageItemVariableDisplayName,
      optionsChordsLanguageItemVariable);
  assert(o!=0);
  return o;
}

msrOptionsChordsLanguageItem::msrOptionsChordsLanguageItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsChordsLanguageItemVariableDisplayName,
  lpsrChordsLanguage&
                     optionsChordsLanguageItemVariable)
  : msrOptionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsChordsLanguageItemVariableDisplayName (
      optionsChordsLanguageItemVariableDisplayName),
    fOptionsChordsLanguageItemVariable (
      optionsChordsLanguageItemVariable)
{}

msrOptionsChordsLanguageItem::~msrOptionsChordsLanguageItem()
{}

void msrOptionsChordsLanguageItem::print (ostream& os) const
{
  const int fieldWidth = FIELD_WIDTH;
  
  os <<
    "OptionsChordsLanguageItem:" <<
    endl;

  gIdtr++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsChordsLanguageItemVariableDisplayName" << " : " <<
    fOptionsChordsLanguageItemVariableDisplayName <<
    setw (fieldWidth) <<
    "fOptionsChordsLanguageItemVariable" << " : \"" <<
    lpsrChordsLanguageAsString (
      fOptionsChordsLanguageItemVariable) <<
      "\"" <<
    endl;
}

void msrOptionsChordsLanguageItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsChordsLanguageItemVariableDisplayName <<
    " : \"" <<
    lpsrChordsLanguageAsString (
      fOptionsChordsLanguageItemVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrOptionsChordsLanguageItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOptionsPartRenameItem msrOptionsPartRenameItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsPartRenameItemVariableDisplayName,
  map<string, string>&
                     optionsPartRenameItemVariable)
{
  msrOptionsPartRenameItem* o = new
    msrOptionsPartRenameItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsPartRenameItemVariableDisplayName,
      optionsPartRenameItemVariable);
  assert(o!=0);
  return o;
}

msrOptionsPartRenameItem::msrOptionsPartRenameItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsPartRenameItemVariableDisplayName,
  map<string, string>&
                     optionsPartRenameItemVariable)
  : msrOptionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsPartRenameItemVariableDisplayName (
      optionsPartRenameItemVariableDisplayName),
    fOptionsPartRenameItemVariable (
      optionsPartRenameItemVariable)
{}

msrOptionsPartRenameItem::~msrOptionsPartRenameItem()
{}

void msrOptionsPartRenameItem::print (ostream& os) const
{
  const int fieldWidth = FIELD_WIDTH;
  
  os <<
    "OptionsPartRenameItem:" <<
    endl;

  gIdtr++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsPartRenameItemVariableDisplayName" << " : " <<
    fOptionsPartRenameItemVariableDisplayName <<
    setw (fieldWidth) <<
    "fOptionsPartRenameItemVariable" << " : " <<
    endl;

  if (! fOptionsPartRenameItemVariable.size ()) {
    os << "none";
  }
  else {
    map<string, string>::const_iterator
      iBegin = fOptionsPartRenameItemVariable.begin(),
      iEnd   = fOptionsPartRenameItemVariable.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  
  os <<
    endl;
}

void msrOptionsPartRenameItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsPartRenameItemVariableDisplayName <<
    " : ";
      
  if (! fOptionsPartRenameItemVariable.size ()) {
    os << "none";
  }
  else {
    gIdtr++;
    
    os <<
      endl;

    map<string, string>::const_iterator
      iBegin = fOptionsPartRenameItemVariable.begin(),
      iEnd   = fOptionsPartRenameItemVariable.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << left <<
        setw (valueFieldWidth) <<
        " " <<
        " \"" <<
        (*i).first <<
        "\" --> \"" <<
        (*i).second <<
        "\"";
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIdtr--;
  }
  
  os <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrOptionsPartRenameItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOptionsMidiTempoItem msrOptionsMidiTempoItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsMidiTempoItemVariableDisplayName,
  pair<string, int>&
                     optionsMidiTempoItemVariable)
{
  msrOptionsMidiTempoItem* o = new
    msrOptionsMidiTempoItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsMidiTempoItemVariableDisplayName,
      optionsMidiTempoItemVariable);
  assert(o!=0);
  return o;
}

msrOptionsMidiTempoItem::msrOptionsMidiTempoItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsMidiTempoItemVariableDisplayName,
  pair<string, int>&
                     optionsMidiTempoItemVariable)
  : msrOptionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsMidiTempoItemVariableDisplayName (
      optionsMidiTempoItemVariableDisplayName),
    fOptionsMidiTempoItemVariable (
      optionsMidiTempoItemVariable)
{}

msrOptionsMidiTempoItem::~msrOptionsMidiTempoItem()
{}

void msrOptionsMidiTempoItem::print (ostream& os) const
{
  const int fieldWidth = FIELD_WIDTH;
  
  os <<
    "OptionsMidiTempoItem:" <<
    endl;

  gIdtr++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsMidiTempoItemVariableDisplayName" << " : " <<
    fOptionsMidiTempoItemVariableDisplayName <<
    setw (fieldWidth) <<
    "fOptionsMidiTempoItemVariable" << " : '" <<
    fOptionsMidiTempoItemVariable.first <<
    " = " <<
    fOptionsMidiTempoItemVariable.second <<
    "'" <<
    endl;
}

void msrOptionsMidiTempoItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsMidiTempoItemVariableDisplayName <<
    " : '" <<
    fOptionsMidiTempoItemVariable.first <<
    " = " <<
    fOptionsMidiTempoItemVariable.second <<
    "'" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrOptionsMidiTempoItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOptionsSubGroup msrOptionsSubGroup::create (
  string optionsSubGroupHelpHeader,
  string optionsSubGroupShortName,
  string optionsSubGroupLongName,
  string optionsSubGroupDescription,
  msrOptionsSubGroupDescriptionVisibility
         optionsSubGroupDescriptionVisibility)
{
  msrOptionsSubGroup* o = new
    msrOptionsSubGroup (
      optionsSubGroupHelpHeader,
      optionsSubGroupShortName,
      optionsSubGroupLongName,
      optionsSubGroupDescription,
      optionsSubGroupDescriptionVisibility);
  assert(o!=0);
  return o;
}

msrOptionsSubGroup::msrOptionsSubGroup (
  string optionsSubGroupHelpHeader,
  string optionsSubGroupShortName,
  string optionsSubGroupLongName,
  string optionsSubGroupDescription,
  msrOptionsSubGroupDescriptionVisibility
         optionsSubGroupDescriptionVisibility)
  : msrOptionsElement (
      optionsSubGroupShortName,
      optionsSubGroupLongName,
      optionsSubGroupDescription)
{
  fOptionsSubGroupHelpHeader =
    optionsSubGroupHelpHeader;

  fOptionsSubGroupDescriptionVisibility =
    optionsSubGroupDescriptionVisibility;
}

msrOptionsSubGroup::~msrOptionsSubGroup()
{}

string msrOptionsSubGroup::optionsSubGroupDescriptionVisibilityAsString (
  msrOptionsSubGroupDescriptionVisibility
    optionsSubGroupDescriptionVisibility)
{
  string result;
  
  switch (optionsSubGroupDescriptionVisibility) {
    case kAlwaysShowDescription:
      result = "AlwaysShowDescription";
      break;
      
    case kHideDescriptionByDefault:
      result = "HideDescriptionByDefault";
      break;
  } // switch

  return result;
}

void msrOptionsSubGroup::underlineHeader (ostream& os) const
{
  for (unsigned int i = 0; i < fOptionsSubGroupHelpHeader.size (); i++) {
    os << "-";
  } // for
  os <<
    endl;
}

void msrOptionsSubGroup::registerOptionsSubGroupInHandler (
  S_msrOptionsHandler optionsHandler)
{
  optionsHandler->
    registerOptionsElementInHandler (this);

  for (
    list<S_msrOptionsItem>::const_iterator
      i = fOptionsSubGroupItemsList.begin();
    i != fOptionsSubGroupItemsList.end();
    i++) {
    // register the options sub group
    (*i)->
      registerOptionsItemInHandler (
        optionsHandler);
  } // for
}

void msrOptionsSubGroup::appendOptionsItem (
  S_msrOptionsItem optionsItem)
{
  fOptionsSubGroupItemsList.push_back (
    optionsItem);

  // set options item subgroup uplink
  optionsItem->
    setOptionsSubGroupUplink (this);
}

S_msrOptionsElement msrOptionsSubGroup::fetchOptionElement (
  string optiontElementName)
{
  S_msrOptionsElement result;
  
  for (
    list<S_msrOptionsItem>::const_iterator
      i = fOptionsSubGroupItemsList.begin();
    i != fOptionsSubGroupItemsList.end();
    i++) {
    // search optiontElementName in the options group
    result =
      (*i)->fetchOptionElement (optiontElementName);
      
    if (result != 0)
      break;
  } // for

  return result;
}

void msrOptionsSubGroup::print (ostream& os) const
{
  const int fieldWidth = 27;
  
  os <<
   "OptionsSubGroup:" <<
    endl;

  gIdtr++;

  msrOptionsElement::printElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsSubGroupDescriptionVisibility" << " : " <<
      optionsSubGroupDescriptionVisibilityAsString (
        fOptionsSubGroupDescriptionVisibility) <<
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
    
    gIdtr++;
    
    list<S_msrOptionsItem>::const_iterator
      iBegin = fOptionsSubGroupItemsList.begin(),
      iEnd   = fOptionsSubGroupItemsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      // print the options item
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIdtr--;
  }

  gIdtr--;
}

void msrOptionsSubGroup::printHelp (ostream& os) const
{
  
  // print the header and option names
  os <<
    fOptionsSubGroupHelpHeader;
    
  os <<
    " " <<
    optionsElementNamesBetweenParentheses ();

  switch (fOptionsSubGroupDescriptionVisibility) {
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
    gIdtr++;
    os <<
      gIdtr.indentMultiLineString (
        fOptionsElementDescription) <<
      endl;
    gIdtr--;

    os <<
      endl;  
  }
  
  switch (fOptionsSubGroupDescriptionVisibility) {
    case kAlwaysShowDescription:
      if (fOptionsSubGroupItemsList.size ()) {    
        gIdtr++;
    
        list<S_msrOptionsItem>::const_iterator
          iBegin = fOptionsSubGroupItemsList.begin(),
          iEnd   = fOptionsSubGroupItemsList.end(),
          i      = iBegin;
        for ( ; ; ) {
          S_msrOptionsItem
            optionsItem = (*i);
          bool
            optionsItemIsHidden =
              optionsItem->getOptionsElementIsHidden ();
              
          // print the options item help unless it is hidden
          if (! optionsItemIsHidden) {
            optionsItem->printHelp (os);
          }
          if (++i == iEnd) break;
          if (! optionsItemIsHidden) {
            os <<
              endl;
          }
        } // for
          
        gIdtr--;
      }
      break;
      
    case kHideDescriptionByDefault:
      break;
  } // switch
}

void msrOptionsSubGroup::printOptionsSubGroupForcedHelp (ostream& os) const
{
  // print the header and option names
  os <<
    fOptionsSubGroupHelpHeader;
    
  os <<
    " " <<
    optionsElementNamesBetweenParentheses ();

  switch (fOptionsSubGroupDescriptionVisibility) {
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
    endl;

  // print the description if any
  if (fOptionsElementDescription.size ()) {
    gIdtr++;
    os <<
      gIdtr.indentMultiLineString (
        fOptionsElementDescription) <<
      endl;
    gIdtr--;

    os <<
      endl;  
  }
      
  if (fOptionsSubGroupItemsList.size ()) {    
    gIdtr++;

    list<S_msrOptionsItem>::const_iterator
      iBegin = fOptionsSubGroupItemsList.begin(),
      iEnd   = fOptionsSubGroupItemsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      // print the options item help
      (*i)->printHelp (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
      
    gIdtr--;
  }
}

void msrOptionsSubGroup::printHelpSummary (
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

  switch (fOptionsSubGroupDescriptionVisibility) {
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
    gIdtr++;
    os <<
      gIdtr.indentMultiLineString (
        fOptionsElementDescription) <<
      endl;
    gIdtr--;
  }
}

void msrOptionsSubGroup::printSpecificSubGroupHelp (
  ostream& os,
  S_msrOptionsSubGroup
           optionsSubGroup) const
{
  // print only the summary if this is not the desired subgroup,
  // otherwise print the regular help
  if (optionsSubGroup == this) {
    os <<
      endl;
    printHelp (os);
  }
  else {
    printHelpSummary (os);
  }
 }

void msrOptionsSubGroup::printOptionsItemForcedHelp (
  ostream&             os,
  S_msrOptionsItem     targetOptionsItem) const
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

  // print the options items
  if (fOptionsSubGroupItemsList.size ()) {    
    gIdtr++;

    list<S_msrOptionsItem>::const_iterator
      iBegin = fOptionsSubGroupItemsList.begin(),
      iEnd   = fOptionsSubGroupItemsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      S_msrOptionsItem
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
    
    gIdtr--;
  }
}

void msrOptionsSubGroup::printOptionsValues (
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
    gIdtr++;

    list<S_msrOptionsItem>::const_iterator
      iBegin = fOptionsSubGroupItemsList.begin(),
      iEnd   = fOptionsSubGroupItemsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      // print the options item values
      (*i)->
        printOptionsValues (
          os, valueFieldWidth);
      if (++i == iEnd) break;
  //    os << endl;
    } // for

    gIdtr--;
  }
}

ostream& operator<< (ostream& os, const S_msrOptionsSubGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOptionsGroup msrOptionsGroup::create (
  string optionsGroupHelpHeader,
  string optionGroupShortName,
  string optionGroupLongName,
  string optionGroupDescription)
{
  msrOptionsGroup* o = new
    msrOptionsGroup (
      optionsGroupHelpHeader,
      optionGroupShortName,
      optionGroupLongName,
      optionGroupDescription);
  assert(o!=0);
  return o;
}

msrOptionsGroup::msrOptionsGroup (
  string optionsGroupHelpHeader,
  string optionGroupShortName,
  string optionGroupLongName,
  string optionGroupDescription)
  : msrOptionsElement (
      optionGroupShortName,
      optionGroupLongName,
      optionGroupDescription)
{
  fOptionsGroupHelpHeader = optionsGroupHelpHeader;
}

msrOptionsGroup::~msrOptionsGroup()
{}

void msrOptionsGroup::underlineHeader (ostream& os) const
{
  for (unsigned int i = 0; i < fOptionsGroupHelpHeader.size (); i++) {
    os << "-";
  } // for
  os <<
    endl;
}

void msrOptionsGroup::registerOptionsGroupInHandler (
  S_msrOptionsHandler optionsHandler)
{
  // set options handler uplink
  setOptionsHandlerUplink (optionsHandler);

  // register options group in options handler
  optionsHandler->
    registerOptionsElementInHandler (this);

  for (
    list<S_msrOptionsSubGroup>::const_iterator
      i = fOptionsGroupSubGroupsList.begin();
    i != fOptionsGroupSubGroupsList.end();
    i++) {
    // register the options sub group
    (*i)->
      registerOptionsSubGroupInHandler (
        optionsHandler);
  } // for
}

void  msrOptionsGroup::appendOptionsSubGroup (
  S_msrOptionsSubGroup optionsSubGroup)
{
  fOptionsGroupSubGroupsList.push_back (
    optionsSubGroup);

  // set options subgroup group uplink
  optionsSubGroup->
    setOptionsGroupUplink (this);
}

S_msrOptionsElement msrOptionsGroup::fetchOptionElement (
  string optiontElementName)
{
  S_msrOptionsElement result;
  
  for (
    list<S_msrOptionsSubGroup>::const_iterator
      i = fOptionsGroupSubGroupsList.begin();
    i != fOptionsGroupSubGroupsList.end();
    i++) {
    // search optiontElementName in the options group
    result =
      (*i)->fetchOptionElement (optiontElementName);
      
    if (result != 0)
      break;
  } // for

  return result;
}

void msrOptionsGroup::print (ostream& os) const
{
  const int fieldWidth = 27;
  
  os <<
    "OptionsGroup:" <<
    endl;

  gIdtr++;

  msrOptionsElement::printElementEssentials (
    os, fieldWidth);

  os <<
    "Options subgroups (" <<
    singularOrPlural (
      fOptionsGroupSubGroupsList.size (), "element",  "elements") <<
    "):" <<
    endl;

  if (fOptionsGroupSubGroupsList.size ()) {
    os << endl;
    
    gIdtr++;
    
    list<S_msrOptionsSubGroup>::const_iterator
      iBegin = fOptionsGroupSubGroupsList.begin(),
      iEnd   = fOptionsGroupSubGroupsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIdtr--;
  }

  gIdtr--;
}

void msrOptionsGroup::printHelp (ostream& os) const
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
    gIdtr++;
    os <<
      gIdtr.indentMultiLineString (
        fOptionsElementDescription) <<
      endl;
    gIdtr--;

    os <<
      endl;  
  }
    
  // print the options subgroups
  if (fOptionsGroupSubGroupsList.size ()) {    
    gIdtr++;

    list<S_msrOptionsSubGroup>::const_iterator
      iBegin = fOptionsGroupSubGroupsList.begin(),
      iEnd   = fOptionsGroupSubGroupsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup help
      (*i)->printHelp (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
      
    gIdtr--;
  }
}

void msrOptionsGroup::printOptionsSubGroupForcedHelp (
  ostream&             os,
  S_msrOptionsSubGroup targetOptionsSubGroup) const
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
    gIdtr++;
    os <<
      gIdtr.indentMultiLineString (
        fOptionsElementDescription) <<
      endl;
    gIdtr--;

    os <<
      endl;  
  }
    
  // print the target options subgroup
  if (fOptionsGroupSubGroupsList.size ()) {    
    gIdtr++;

    list<S_msrOptionsSubGroup>::const_iterator
      iBegin = fOptionsGroupSubGroupsList.begin(),
      iEnd   = fOptionsGroupSubGroupsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      S_msrOptionsSubGroup
        optionsSubGroup = (*i);
        
      if (optionsSubGroup == targetOptionsSubGroup) {
        // print the target options subgroup help
        (*i)->printHelp (os);
      }
      if (++i == iEnd) break;
      if (optionsSubGroup == targetOptionsSubGroup) {
        os <<
          endl;
      }
    } // for
      
    gIdtr--;
  }
}

void msrOptionsGroup::printOptionsItemForcedHelp (
  ostream&             os,
  S_msrOptionsSubGroup targetOptionsSubGroup,
  S_msrOptionsItem     targetOptionsItem) const
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
    gIdtr++;
    os <<
      gIdtr.indentMultiLineString (
        fOptionsElementDescription) <<
      endl;
    gIdtr--;

    os <<
      endl;  
  }
    
  // print the target options subgroup
  if (fOptionsGroupSubGroupsList.size ()) {    
    gIdtr++;

    list<S_msrOptionsSubGroup>::const_iterator
      iBegin = fOptionsGroupSubGroupsList.begin(),
      iEnd   = fOptionsGroupSubGroupsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      S_msrOptionsSubGroup
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
      
    gIdtr--;
  }
}

void msrOptionsGroup::printHelpSummary (ostream& os) const
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
    gIdtr++;
    os <<
      gIdtr.indentMultiLineString (
        fOptionsElementDescription) <<
      endl;
    gIdtr--;

    os <<
      endl;  
  }
    
  // print the options subgroups
  if (fOptionsGroupSubGroupsList.size ()) {    
    gIdtr++;

    list<S_msrOptionsSubGroup>::const_iterator
      iBegin = fOptionsGroupSubGroupsList.begin(),
      iEnd   = fOptionsGroupSubGroupsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup description
      (*i)->
        printHelpSummary (os);
      if (++i == iEnd) break;
 //     os << endl;
    } // for
      
    gIdtr--;
  }
}

void msrOptionsGroup::printSpecificSubGroupHelp (
  ostream& os,
  S_msrOptionsSubGroup
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
    gIdtr++;
    os <<
      gIdtr.indentMultiLineString (
        fOptionsElementDescription) <<
      endl;
    gIdtr--;

    os <<
      endl;  
  }
    
  // print the options subgroups
  if (fOptionsGroupSubGroupsList.size ()) {    
    gIdtr++;

    list<S_msrOptionsSubGroup>::const_iterator
      iBegin = fOptionsGroupSubGroupsList.begin(),
      iEnd   = fOptionsGroupSubGroupsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup specific subgroup help
      (*i)->
        printSpecificSubGroupHelp (
          os, optionsSubGroup);
      if (++i == iEnd) break;
      os << endl;
    } // for

    os <<
      endl;

    gIdtr--;
  }
}

void msrOptionsGroup::printOptionsValues (
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
    gIdtr++;

    list<S_msrOptionsSubGroup>::const_iterator
      iBegin = fOptionsGroupSubGroupsList.begin(),
      iEnd   = fOptionsGroupSubGroupsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup values
      (*i)->
        printOptionsValues (
          os, valueFieldWidth);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIdtr--;
  }
}

ostream& operator<< (ostream& os, const S_msrOptionsGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
/* JMI
S_msrOptionsHandler msrOptionsHandler::create (
  string optionsHandlerHelpHeader,
  string optionsHandlerValuesHeader,
  string optionHandlerHelpShortName,
  string optionHandlerHelpLongName,
  string optionHandlerHelpSummaryShortName,
  string optionHandlerHelpSummaryLongName,
  string optionHandlerPreamble,
  string optionHandlerDescription,
  indentedOutputStream& ios)
{
  msrOptionsHandler* o = new
    msrOptionsHandler (
      optionsHandlerHelpHeader,
      optionsHandlerValuesHeader,
      optionHandlerHelpShortName,
      optionHandlerHelpLongName,
      optionHandlerHelpSummaryShortName,
      optionHandlerHelpSummaryLongName,
      optionHandlerPreamble,
      optionHandlerDescription,
      ios);
  assert(o!=0);
  return o;
}
*/

msrOptionsHandler::msrOptionsHandler (
  string optionsHandlerHelpHeader,
  string optionsHandlerValuesHeader,
  string optionHandlerHelpShortName,
  string optionHandlerHelpLongName,
  string optionHandlerHelpSummaryShortName,
  string optionHandlerHelpSummaryLongName,
  string optionHandlerPreamble,
  string optionHandlerDescription,
  indentedOutputStream& ios)
  : msrOptionsElement (
      optionHandlerHelpShortName,
      optionHandlerHelpLongName,
      optionHandlerDescription),
    fLogOutputStream (ios)
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

msrOptionsHandler::~msrOptionsHandler()
{}

void msrOptionsHandler::registerOptionsHandlerInItself ()
{
  this->
    registerOptionsElementInHandler (this);

  // register the help summary names in handler
  registerOptionsNamesInHandler (
    fOptionHandlerHelpSummaryShortName,
    fOptionHandlerHelpSummaryLongName,
    this);

  for (
    list<S_msrOptionsGroup>::const_iterator
      i = fOptionsHandlerOptionsGroupsList.begin();
    i != fOptionsHandlerOptionsGroupsList.end();
    i++) {
    // register the options group
    (*i)->
      registerOptionsGroupInHandler (
        this);
  } // for
}

string msrOptionsHandler::helpNamesBetweenParentheses () const
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

void msrOptionsHandler::registerOptionsNamesInHandler (
  string              optionShortName,
  string              optionLongName,
  S_msrOptionsElement optionsElement)
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
    abort ();
  }
  
  if (optionShortName == optionLongName) {
    stringstream s;

    s <<
      "option long name '" << optionLongName << "'" <<
      " is the same as the short name for the same";
      
    optionError (s.str ());
    abort ();
  }
  
  for (
    map<string, S_msrOptionsElement>::iterator i =
      fOptionsElementsMap.begin();
    i != fOptionsElementsMap.end();
    i++) {
      
    // is optionLongName already in the options names map?
    if ((*i).first == optionLongName) {
      stringstream s;
  
      s <<
        "option long name '" << optionLongName << "'" <<
          " for option short name '" << optionShortName << "'" <<
        " is specified more that once";
        
      optionError (s.str ());
      abort ();
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
        abort ();
      }
    }
  } // for
    
  // register optionsElement's names size
  if (optionLongNameSize) {
    fOptionsElementsMap [optionLongName] =
      optionsElement;
    
    if (optionLongNameSize > fMaximumLongNameWidth)
      fMaximumLongNameWidth = optionLongNameSize;
    
    if (optionLongNameSize > fMaximumDisplayNameWidth)
      fMaximumDisplayNameWidth = optionLongNameSize;
  }
  
  if (optionShortNameSize) {
    fOptionsElementsMap [optionShortName] =
      optionsElement;
    
    if (optionShortNameSize > fMaximumShortNameWidth)
      fMaximumShortNameWidth = optionShortNameSize;
    
    if (optionShortNameSize > fMaximumDisplayNameWidth)
      fMaximumDisplayNameWidth = optionShortNameSize;
  }
}

void msrOptionsHandler::registerOptionsElementInHandler (
  S_msrOptionsElement optionsElement)
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
    S_msrOptionsSubGroup
      optionsSubGroup =
        dynamic_cast<msrOptionsSubGroup*>(&(*optionsElement))
    ) {    

    string
      optionHelpHeader=
        optionsSubGroup->getOptionsSubGroupHelpHeader ();
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

void msrOptionsHandler::print (ostream& os) const
{
  const int fieldWidth = 27;
  
  os <<
    "OptionsHandler:" <<
    endl;

  gIdtr++;
  
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
    
    gIdtr++;
    
    list<S_msrOptionsGroup>::const_iterator
      iBegin = fOptionsHandlerOptionsGroupsList.begin(),
      iEnd   = fOptionsHandlerOptionsGroupsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIdtr--;
  }

  gIdtr--;
}

void msrOptionsHandler::printHelp (ostream& os) const
{
  // print the options handler preamble
  os <<
    gIdtr.indentMultiLineString (
      fOptionHandlerPreamble) <<
      endl;
    
  // print versions history
  printVersionsHistory (os);

  // print the options handler help header and element names
  os <<
    fOptionsHandlerHelpHeader <<
    " " <<
    helpNamesBetweenParentheses () <<
    ":" <<
    endl;
  
  // print the options handler description
  gIdtr++;
  os <<
    gIdtr.indentMultiLineString (
      fOptionsElementDescription) <<
      endl <<
    endl;
  gIdtr--;
    
  // print the options groups helps
  if (fOptionsHandlerOptionsGroupsList.size ()) {    
    gIdtr++;

    list<S_msrOptionsGroup>::const_iterator
      iBegin = fOptionsHandlerOptionsGroupsList.begin(),
      iEnd   = fOptionsHandlerOptionsGroupsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group help
      (*i)->printHelp (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
    
    gIdtr--;
  }
}

void msrOptionsHandler::printHelpSummary (ostream& os) const
{
  // print the options handler preamble
  os <<
    gIdtr.indentMultiLineString (
      fOptionHandlerPreamble) <<
      endl;
    
  // print the options handler help header and element names
  os <<
    fOptionsHandlerHelpHeader <<
    " " <<
    helpNamesBetweenParentheses () <<
    ":" <<
    endl;

  // print the options handler description
  gIdtr++;
  os <<
    gIdtr.indentMultiLineString (
      fOptionsElementDescription) <<
      endl <<
    endl;
  gIdtr--;
    
  // print the options groups help summaries
  if (fOptionsHandlerOptionsGroupsList.size ()) {    
    gIdtr++;

    list<S_msrOptionsGroup>::const_iterator
      iBegin = fOptionsHandlerOptionsGroupsList.begin(),
      iEnd   = fOptionsHandlerOptionsGroupsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group summary
      (*i)->printHelpSummary (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIdtr--;
  }
}

void msrOptionsHandler::printSpecificSubGroupHelp (
  ostream& os,
  S_msrOptionsSubGroup
           optionsSubGroup) const
{
  // print the options handler help header and element names
  os <<
    fOptionsHandlerHelpHeader <<
    " " <<
    helpNamesBetweenParentheses () <<
    ":" <<
    endl;

  // print the optons group subgroups specific help
  if (fOptionsHandlerOptionsGroupsList.size ()) {    
    gIdtr++;

    list<S_msrOptionsGroup>::const_iterator
      iBegin = fOptionsHandlerOptionsGroupsList.begin(),
      iEnd   = fOptionsHandlerOptionsGroupsList.end(),
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

    os <<
      endl;
      
    gIdtr--;
  }
}

void msrOptionsHandler::printSpecificItemHelp (
  ostream& os,
  string   optionsItemName) const
{  
 // is optionsItemName known in options elements map?
  map<string, S_msrOptionsElement>::const_iterator
    it =
      fOptionsElementsMap.find (optionsItemName);
        
  if (it == fOptionsElementsMap.end ()) {
    // no, optionsItemName is unknown in the map    
    stringstream s;

    s <<
      "option name '" << optionsItemName <<
      "' is unknown";
      
    optionError (s.str ());

    printHelpSummary (fLogOutputStream);

    exit (2);
  }

  S_msrOptionsElement
    optionsElement = (*it).second;
      
  if (! optionsElement) {
    // optionsItemName is is not well handled by this options handler
    stringstream s;

    s <<
      "option name '" << optionsItemName <<
      "' is not well handled";
      
    optionError (s.str ());
    abort ();
  }

  else {
    // optionsItemName is known, let's handle it
    if (
      // options handler?
      S_msrOptionsHandler
        optionsHandler =
          dynamic_cast<msrOptionsHandler*>(&(*optionsElement))
      ) {
      // print the option handler help or help summary
      if (
        optionsItemName ==
          optionsHandler->
            getOptionHandlerHelpSummaryShortName ()
          ||
        optionsItemName ==
          optionsHandler->
            getOptionHandlerHelpSummaryLongName ()
         ) {
        optionsHandler->
          printHelpSummary (
            fLogOutputStream);
      }
      else {
        optionsHandler->
          printHelp (
            fLogOutputStream);
      }
      
      fLogOutputStream <<
        endl;
    }
    
    else if (
      // options group?
      S_msrOptionsGroup
        optionsGroup =
          dynamic_cast<msrOptionsGroup*>(&(*optionsElement))
      ) {
      // print the help
      fLogOutputStream <<
        "--- Help for options item name '" <<
        optionsItemName <<
        "' for group \"" <<
        optionsGroup->
          getOptionsGroupHelpHeader () <<
        "\" ---" <<
        endl <<
        endl;
        
      optionsGroup->
        printHelp (
          fLogOutputStream);
      
      fLogOutputStream <<
        endl;
    }
    
    else if (
      // options subgroup?
      S_msrOptionsSubGroup
        optionsSubGroup =
          dynamic_cast<msrOptionsSubGroup*>(&(*optionsElement))
      ) {
      // get the options group uplink
      S_msrOptionsGroup
        optionsGroup =
          optionsSubGroup-> getOptionsGroupUplink ();
          
      // print the help
      fLogOutputStream <<
        "--- Help for options item name '" <<
        optionsItemName <<
        "' for subgroup \"" <<
        optionsSubGroup->
          getOptionsSubGroupHelpHeader () <<
        "\"" <<
        " in group \"" <<
        optionsGroup->
          getOptionsGroupHelpHeader () <<
        "\" ---" <<
        endl <<
        endl;

      optionsGroup->
        printOptionsSubGroupForcedHelp (
          fLogOutputStream,
          optionsSubGroup);
    }
    
    else if (
      // options item?
      S_msrOptionsItem
        optionsItem =
          dynamic_cast<msrOptionsItem*>(&(*optionsElement))
      ) {
      // get the options subgroup uplink
      S_msrOptionsSubGroup
        optionsSubGroup =
          optionsItem-> getOptionsSubGroupUplink ();
          
      // get the options group uplink
      S_msrOptionsGroup
        optionsGroup =
          optionsSubGroup-> getOptionsGroupUplink ();

      // print the help
      fLogOutputStream <<
        "--- Help for options item name '" <<
        optionsItemName <<
        "' in subgroup \"" <<
        optionsSubGroup->
          getOptionsSubGroupHelpHeader () <<
        "\"" <<
        " in group \"" <<
        optionsGroup->
          getOptionsGroupHelpHeader () <<
        "\" ---" <<
        endl <<
        endl;

      optionsGroup->
        printOptionsItemForcedHelp (
          fLogOutputStream,
          optionsSubGroup,
          optionsItem);
    }
    
    else {
      stringstream s;

      s <<
        "cannot handle specific help about optionsItemName \"" <<
        optionsItemName <<
        "\"";
        
      optionError (s.str ());
      abort ();
    }
  }
}

void msrOptionsHandler::printOptionsValues (
  ostream& os) const
{
  // print the options handler values header
  os <<
    fOptionsHandlerValuesHeader <<
    " " <<
    helpNamesBetweenParentheses () <<
    ":" <<
    endl;

  // print the options groups values
  if (fOptionsHandlerOptionsGroupsList.size ()) {
    os << endl;
    
    gIdtr++;

    list<S_msrOptionsGroup>::const_iterator
      iBegin = fOptionsHandlerOptionsGroupsList.begin(),
      iEnd   = fOptionsHandlerOptionsGroupsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group values
      (*i)->
        printOptionsValues (
          os, fMaximumDisplayNameWidth);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIdtr--;
  }
}

ostream& operator<< (ostream& os, const S_msrOptionsHandler& elt)
{
  elt->print (os);
  return os;
}

void msrOptionsHandler::appendOptionsGroup (
  S_msrOptionsGroup optionsGroup)
{
  fOptionsHandlerOptionsGroupsList.push_back (
    optionsGroup);
}

S_msrOptionsElement msrOptionsHandler::fetchOptionElement (
  string optiontElementName)
{
  S_msrOptionsElement result;
  
  for (
    list<S_msrOptionsGroup>::const_iterator
      i = fOptionsHandlerOptionsGroupsList.begin();
    i != fOptionsHandlerOptionsGroupsList.end();
    i++) {
    // search optiontElementName in the options group
    result =
      (*i)->fetchOptionElement (optiontElementName);
      
    if (result != 0)
      break;
  } // for

  return result;
}

const vector<string> msrOptionsHandler::decipherOptionsAndArguments (
  int   argc,
  char* argv[])
{
  if (TRACE_OPTIONS) {
    // print the options elements map  
    fLogOutputStream <<
      "Options elements map (" <<
      fOptionsElementsMap.size () <<
      " elements):" <<
      endl;
    if (fOptionsElementsMap.size ()) {
      gIdtr++;
      
      map<string, S_msrOptionsElement>::const_iterator
        iBegin = fOptionsElementsMap.begin(),
        iEnd   = fOptionsElementsMap.end(),
        i      = iBegin;
      for ( ; ; ) {
        fLogOutputStream <<
          (*i).first << "-->" <<
          endl;
        gIdtr++;
        (*i).second->
          printHeader (fLogOutputStream);
        if (++i == iEnd) break;
        fLogOutputStream << endl;
        gIdtr--;
      } // for
      
      gIdtr--;
    }
    fLogOutputStream <<
      endl;
  }

  // fetch program name
  fProgramName = string (argv [0]);
  
  fCommandLineWithShortOptions = fProgramName;
  fCommandLineWithLongOptions  = fProgramName;
  
  // decipher the command options and arguments
  int n = 1;
  
  fPureHelpRun = true;

  while (true) { 
    if (argv [n] == 0)
      break;

    string currentElement = string (argv [n]);
    
    if (TRACE_OPTIONS) {
      // print current element
      fLogOutputStream <<
        "Command line element " << n <<
        ": " <<currentElement << " "<<
        endl;
    }

    // handle current element
    if (currentElement [0] == '-') {
      // stdin or options element?
      
      if (currentElement.size () == 1) {
        // this is the stdin indicator
        if (TRACE_OPTIONS) {
          fLogOutputStream <<
            "'" << currentElement <<
              "' is the '-' stdin indicator" <<
            endl;
        }
        
        fArgumentsVector.push_back (currentElement);
        fPureHelpRun = false;

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
        fLogOutputStream <<
          "elementTrailer '" << elementTrailer << "' is preceded by a dash" <<
          endl;
        */
  
        if (elementTrailer.size ()) {
          if (elementTrailer [0] == '-') {
            // it is a double-dashed option
            currentOptionName =
              elementTrailer.substr (1, string::npos);
            
            if (TRACE_OPTIONS) {
              fLogOutputStream <<
                "'" << currentOptionName << "' is a double-dashed option" <<
                endl;
            }
          }
          else {
            // it is a single-dashed option
            currentOptionName =
              elementTrailer; //.substr (1, string::npos);
            
            if (TRACE_OPTIONS) {
              fLogOutputStream <<
                "'" << currentOptionName << "' is a single-dashed option" <<
                endl;
            }
          }
        }
        
        else {
          if (TRACE_OPTIONS) {
            fLogOutputStream <<
              "'-' is the minimal single-dashed option" <<
              endl;
          }
        }

        // handle the options item name
        handleOptionsItemName (currentOptionName);
      }
    }

    else {
      // currentElement is no options item,
      // i.e. it is an item value or an argument
      handleOptionsItemValueOrArgument (currentElement);

      fPureHelpRun = false;
    }
    
    // next please
    n++;
  } // while

  // exit if this is a pure help run
  if (fPureHelpRun) {
    fLogOutputStream <<
      "--- This is a pure help run, exiting. ---" <<
      endl;

    exit (1);
  }

  unsigned int argumentsVectorSize =
    fArgumentsVector.size ();

  if (TRACE_OPTIONS) {
    // print the arguments vector
    fLogOutputStream <<
      "Arguments vector (" <<
      argumentsVectorSize <<
      " elements):" <<
      endl;
  
    if (argumentsVectorSize) {
      gIdtr++;
      for (unsigned int i = 0; i < argumentsVectorSize; i++) {
        fLogOutputStream <<
          fArgumentsVector [i] <<
          endl;
      } // for
      gIdtr--;
    }
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

void msrOptionsHandler::handleOptionsItemName (
  string optionsItemName)
{
  // is optionsItemName known in options elements map?
  map<string, S_msrOptionsElement>::const_iterator
    it =
      fOptionsElementsMap.find (optionsItemName);
        
  if (it == fOptionsElementsMap.end ()) {
    // no, optionsItemName is unknown in the map    
    stringstream s;

    s <<
      "option name '" << optionsItemName <<
      "' is unknown";
      
    optionError (s.str ());

    printHelpSummary (fLogOutputStream);

    exit (2);
  }

  S_msrOptionsElement
    optionsElement = (*it).second;
      
  if (! optionsElement) {
    // optionsItemName is is not well handled by this options handler
    stringstream s;

    s <<
      "option name '" << optionsItemName <<
      "' is not well handled";
      
    optionError (s.str ());
    abort ();
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
    if (! shortNameToBeUsed.size ())
      shortNameToBeUsed = longNameToBeUsed;
    if (! longNameToBeUsed.size ())
      longNameToBeUsed = shortNameToBeUsed;

    // register option element names in command line strings
    fCommandLineWithShortOptions +=
      " -" + shortNameToBeUsed;
    fCommandLineWithLongOptions +=
      " -" + longNameToBeUsed;

    // handle the option element
    if (
      // options handler?
      S_msrOptionsHandler
        optionsHandler =
          dynamic_cast<msrOptionsHandler*>(&(*optionsElement))
      ) {
      // print the option handler help or help summary
      if (
        optionsItemName ==
          optionsHandler->
            getOptionHandlerHelpSummaryShortName ()
          ||
        optionsItemName ==
          optionsHandler->
            getOptionHandlerHelpSummaryLongName ()
         ) {
        optionsHandler->
          printHelpSummary (
            fLogOutputStream);
      }
      else {
        optionsHandler->
          printHelp (
            fLogOutputStream);
      }
      
      fLogOutputStream <<
        endl;
    }
    
    else if (
      // options group?
      S_msrOptionsGroup
        optionsGroup =
          dynamic_cast<msrOptionsGroup*>(&(*optionsElement))
      ) {
      // print the help
      fLogOutputStream <<
        "--- Help for group \"" <<
        optionsGroup->
          getOptionsGroupHelpHeader () <<
        "\" ---" <<
        endl <<
        endl;
        
      optionsGroup->
        printHelp (
          fLogOutputStream);
      
      fLogOutputStream <<
        endl;
    }
    
    else if (
      // options subgroup?
      S_msrOptionsSubGroup
        optionsSubGroup =
          dynamic_cast<msrOptionsSubGroup*>(&(*optionsElement))
      ) {
      // get the options group uplink
      S_msrOptionsGroup
        optionsGroup =
          optionsSubGroup-> getOptionsGroupUplink ();
          
      // print the help
      fLogOutputStream <<
        "--- Help for subgroup \"" <<
        optionsSubGroup->
          getOptionsSubGroupHelpHeader () <<
        "\"" <<
        " in group \"" <<
        optionsGroup->
          getOptionsGroupHelpHeader () <<
        "\" ---" <<
        endl <<
        endl;

      optionsGroup->
        printOptionsSubGroupForcedHelp (
          fLogOutputStream,
          optionsSubGroup);
    }
    
    else {
      // this is an options item, handle it
      
      if (
        // version item?
        S_msrOptionsVersionItem
          optionsVersionItem =
            dynamic_cast<msrOptionsVersionItem*>(&(*optionsElement))
        ) {
        // handle it at once
        optionsVersionItem->
          printVersion (
            fLogOutputStream);

        // exit
        exit (0);
      }
      
      else if (
        // boolean item?
        S_msrOptionsBooleanItem
          optionsBooleanItem =
            dynamic_cast<msrOptionsBooleanItem*>(&(*optionsElement))
        ) {
        // handle it at once
        optionsBooleanItem->
          setBooleanItemVariableValue (true);              
      }
      
      else if (
        // two booleans item?
        S_msrOptionsTwoBooleansItem
          optionsTwoBooleansItem =
            dynamic_cast<msrOptionsTwoBooleansItem*>(&(*optionsElement))
        ) {
        // handle it at once
        optionsTwoBooleansItem->
          setTwoBooleansItemVariableValue (true);              
      }
      
      else if (
        // item help item?
        S_msrOptionsItemHelpItem
          optionsItemHelpItem =
            dynamic_cast<msrOptionsItemHelpItem*>(&(*optionsElement))
        ) {
        // wait until the value is met
        fPendingOptionsItem = optionsItemHelpItem;
      }
      
      else if (
        // integer item?
        S_msrOptionsIntegerItem
          optionsIntegerItem =
            dynamic_cast<msrOptionsIntegerItem*>(&(*optionsElement))
        ) {
        // wait until the value is met
        fPendingOptionsItem = optionsIntegerItem;
      }
      
      else if (
        // float item?
        S_msrOptionsFloatItem
          optionsFloatItem =
            dynamic_cast<msrOptionsFloatItem*>(&(*optionsElement))
        ) {              
        // wait until the value is met
        fPendingOptionsItem = optionsFloatItem;
      }
      
      else if (
        // string item?
        S_msrOptionsStringItem
          optionsStringItem =
            dynamic_cast<msrOptionsStringItem*>(&(*optionsElement))
        ) {
        // wait until the value is met
        fPendingOptionsItem = optionsStringItem;
      }
      
      else if (
        // rational item?
        S_msrOptionsRationalItem
          optionsRationalItem =
            dynamic_cast<msrOptionsRationalItem*>(&(*optionsElement))
        ) {
        // wait until the value is met
        fPendingOptionsItem = optionsRationalItem;
      }

      else if (
        // numbers set item?
        S_msrOptionsNumbersSetItem
          optionsNumbersSetItem =
            dynamic_cast<msrOptionsNumbersSetItem*>(&(*optionsElement))
        ) {
        // wait until the value is met
        fPendingOptionsItem = optionsNumbersSetItem;
      }

      else if (
        // pitches language item?
        S_msrOptionsPitchesLanguageItem
          optionsPitchesLanguageItem =
            dynamic_cast<msrOptionsPitchesLanguageItem*>(&(*optionsElement))
        ) {
        // wait until the value is met
        fPendingOptionsItem = optionsPitchesLanguageItem;
      }

      else if (
        // acccidentals style item?
        S_msrOptionsAccidentalStyleItem
          optionsAccidentalStyleItem =
            dynamic_cast<msrOptionsAccidentalStyleItem*>(&(*optionsElement))
        ) {
        // wait until the value is met
        fPendingOptionsItem = optionsAccidentalStyleItem;
      }

      else if (
        // chords language item?
        S_msrOptionsChordsLanguageItem
          optionsChordsLanguageItem =
            dynamic_cast<msrOptionsChordsLanguageItem*>(&(*optionsElement))
        ) {
        // wait until the value is met
        fPendingOptionsItem = optionsChordsLanguageItem;
      }

      else if (
        // part rename item?
        S_msrOptionsPartRenameItem
          optionsPartRenameItem =
            dynamic_cast<msrOptionsPartRenameItem*>(&(*optionsElement))
        ) {
        // wait until the value is met
        fPendingOptionsItem = optionsPartRenameItem;
      }

      else if (
        // midi tempo item?
        S_msrOptionsMidiTempoItem
          optionsMidiTempoItem =
            dynamic_cast<msrOptionsMidiTempoItem*>(&(*optionsElement))
        ) {
        // wait until the value is met
        fPendingOptionsItem = optionsMidiTempoItem;
      }

      else {
        stringstream s;
    
        s <<
          "option item is of unknown type";
          
        optionError (s.str ());
        abort ();
      }
    }
  }
}

void msrOptionsHandler::handleOptionsItemValueOrArgument (
  string theString)
{
  if (fPendingOptionsItem) {
    // theString is the value for the pending options item

    if (
      // item help item?
      S_msrOptionsItemHelpItem
        optionsItemHelpItem =
          dynamic_cast<msrOptionsItemHelpItem*>(&(*fPendingOptionsItem))
      ) {
      // handle the option item

      printSpecificItemHelp (
        fLogOutputStream,
        theString);

      fPendingOptionsItem = 0;
      }
    
    else if (
      // integer item?
      S_msrOptionsIntegerItem
        optionsIntegerItem =
          dynamic_cast<msrOptionsIntegerItem*>(&(*fPendingOptionsItem))
      ) {
      // handle the option item

      int integerValue;
      {
        stringstream s;
        s << theString;
        s >> integerValue;
      }
      
      optionsIntegerItem->
        setIntegerItemVariableValue (
          integerValue);

      fPendingOptionsItem = 0;
      }
    
    else if (
      // float item?
      S_msrOptionsFloatItem
        optionsFloatItem =
          dynamic_cast<msrOptionsFloatItem*>(&(*fPendingOptionsItem))
      ) {              
      // handle the option item
      float floatValue;
      {
        stringstream s;
        s << theString;
        s >> floatValue;
      }
      
      optionsFloatItem->
        setFloatItemVariableValue (
          floatValue);

      fPendingOptionsItem = 0;
    }
    
    else if (
      // string item?
      S_msrOptionsStringItem
        optionsStringItem =
          dynamic_cast<msrOptionsStringItem*>(&(*fPendingOptionsItem))
      ) {
      // handle the option item
      optionsStringItem->
        setStringItemVariableValue (
          theString);

      fPendingOptionsItem = 0;
    }
    
    else if (
      // rational item?
      S_msrOptionsRationalItem
        optionsRationalItem =
          dynamic_cast<msrOptionsRationalItem*>(&(*fPendingOptionsItem))
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
        if (TRACE_OPTIONS) {
          fLogOutputStream <<
            "There are " << sm.size() << " matches" <<
            " for rational string '" << theString <<
            "' with regex '" << regularExpression <<
            "'" <<
            endl;
       
          for (unsigned i = 0; i < sm.size (); ++i) {
            fLogOutputStream <<
              "[" << sm [i] << "] ";
          } // for
          
          fLogOutputStream <<
            endl;
        }
      }
     
      else {
        stringstream s;

        s <<
          "-delayedOrnamentFraction argument '" << theString <<
          "' is ill-formed";
          
        optionError (s.str ());
        
        printSpecificSubGroupHelp (
          fLogOutputStream,
          optionsRationalItem->
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

      if (TRACE_OPTIONS) {
        fLogOutputStream << // JMI
          "rationalValue = " <<
          rationalValue <<
          endl;
      }

      optionsRationalItem->
        setRationalItemVariableValue (
          rationalValue);

      fPendingOptionsItem = 0;
    }

    else if (
      // numbers set item?
      S_msrOptionsNumbersSetItem
        optionsNumbersSetItem =
          dynamic_cast<msrOptionsNumbersSetItem*>(&(*fPendingOptionsItem))
      ) {
      // theString contains the set specification,
      // decipher it
      optionsNumbersSetItem->
        setNumbersSetItemVariableValue (
          decipherNumbersSetSpecification (
            theString, false) // 'true' to debug it
          );

      fPendingOptionsItem = 0;
    }
    
    else if (
      // pitches language item?
      S_msrOptionsPitchesLanguageItem
        optionsPitchesLanguageItem =
          dynamic_cast<msrOptionsPitchesLanguageItem*>(&(*fPendingOptionsItem))
      ) {
      // theString contains the language name:     
      // is it in the pitches languages map?
      map<string, msrQuarterTonesPitchesLanguage>::const_iterator
        it =
          gQuarterTonesPitchesLanguagesMap.find (
            theString);
            
      if (it == gQuarterTonesPitchesLanguagesMap.end ()) {
        // no, language is unknown in the map
        stringstream s;
    
        s <<
          "MSR pitches language " << theString <<
          " is unknown" <<
          endl <<
          "The " <<
          gQuarterTonesPitchesLanguagesMap.size () <<
          " known MSR pitches languages are:" <<
          endl;
    
        gIdtr++;
      
        s <<
          existingQuarterTonesPitchesLanguages ();
    
        gIdtr--;
    
        optionError (s.str ());
        
        printHelpSummary (
          fLogOutputStream);
        
        exit (4);
      }
    
      optionsPitchesLanguageItem->
        setPitchesLanguageItemVariableValue (
          (*it).second);

      fPendingOptionsItem = 0;
    }
    
    else if (
      // accidental style item?
      S_msrOptionsAccidentalStyleItem
        optionsAccidentalStyleItem =
          dynamic_cast<msrOptionsAccidentalStyleItem*>(&(*fPendingOptionsItem))
      ) {
      // theString contains the language name:     
      // is it in the accidental styles map?
      map<string, lpsrAccidentalStyle>::const_iterator
        it =
          gLpsrAccidentalStylesMap.find (
            theString);
            
      if (it == gLpsrAccidentalStylesMap.end ()) {
        // no, accidental style is unknown in the map
        stringstream s;
    
        s <<
          "LPSR accidental style " << theString <<
          " is unknown" <<
          endl <<
          "The " <<
          gLpsrAccidentalStylesMap.size () - 1 <<
          " known LPSR accidental styles are:" <<
          endl;
    
        gIdtr++;
      
        s <<
          existingLpsrAccidentalStyles ();
    
        gIdtr--;
    
        optionError (s.str ());
        
        printHelpSummary (
          fLogOutputStream);
        
        exit (4);
      }
    
      optionsAccidentalStyleItem->
        setAccidentalStyleItemVariableValue (
          (*it).second);

      fPendingOptionsItem = 0;
    }
    
    else if (
      // chords language item?
      S_msrOptionsChordsLanguageItem
        optionsChordsLanguageItem =
          dynamic_cast<msrOptionsChordsLanguageItem*>(&(*fPendingOptionsItem))
      ) {
      // theString contains the language name:     
      // is it in the chords languages map?
      map<string, lpsrChordsLanguage>::const_iterator
        it =
          gLpsrChordsLanguagesMap.find (theString);
            
      if (it == gLpsrChordsLanguagesMap.end ()) {
        // no, language is unknown in the map    
        stringstream s;
    
        s <<
          "LPSR chords language " << theString <<
          " is unknown" <<
          endl <<
          "The " <<
          gLpsrChordsLanguagesMap.size () - 1 <<
          " known LPSR chords languages apart from the default Ignatzek are:" <<
          endl;
    
        gIdtr++;
      
        s <<
          existingLpsrChordsLanguages ();
    
        gIdtr--;
    
        optionError (s.str ());
        
        printHelpSummary (
          fLogOutputStream);
        
        exit (4);
      }
    
      optionsChordsLanguageItem->
        setChordsLanguageItemVariableValue (
          (*it).second);

      fPendingOptionsItem = 0;
    }
    
    else if (
      // part rename item?
      S_msrOptionsPartRenameItem
        optionsPartRenameItem =
          dynamic_cast<msrOptionsPartRenameItem*>(&(*fPendingOptionsItem))
      ) {
      // theString contains the part rename specification
      // decipher it to extract the old and new part names
      string regularExpression (
        "[[:space:]]*(.*)[[:space:]]*"
        "="
        "[[:space:]]*(.*)[[:space:]]*");
        
      regex  e (regularExpression);
      smatch sm;

      regex_match (theString, sm, e);

      if (TRACE_OPTIONS) {
        fLogOutputStream <<
          "There are " << sm.size() << " matches" <<
          " for MIDI tempo string '" << theString <<
          "' with regex '" << regularExpression <<
          "'" <<
          endl;
      }
    
      if (sm.size ()) {
        for (unsigned i = 0; i < sm.size (); ++i) {
          fLogOutputStream <<
            "[" << sm [i] << "] ";
        } // for
        fLogOutputStream <<
          endl;
      }
      
      else {
        stringstream s;

        s <<
          "-msrPartRename argument '" << theString <<
          "' is ill-formed";
          
        optionError (s.str ());
        
        printSpecificSubGroupHelp (
          fLogOutputStream,
          optionsPartRenameItem->
            getOptionsSubGroupUplink ());
            
        exit (4);
      }

      string
        oldPartName = sm [1],
        newPartName = sm [2];
        
      if (TRACE_OPTIONS) {
        fLogOutputStream <<
          "--> oldPartName = \"" << oldPartName << "\", " <<
          "--> newPartName = \"" << newPartName << "\"" <<
          endl;
      }

      map<string, string>
        optionsPartRenameItemVariable =
          optionsPartRenameItem->
            getOptionsPartRenameItemVariable ();
            
      // is this part name in the part renaming map?
      map<string, string>::iterator
        it =
          optionsPartRenameItemVariable.find (oldPartName);
            
      if (it != optionsPartRenameItemVariable.end ()) {
        // yes, issue error message
        stringstream s;

        s <<
          "Part \"" << oldPartName << "\" occurs more that one" <<
          "in the '--partName' option";
          
        optionError (s.str ());
        exit (4);
      }
      
      else {
        optionsPartRenameItem->
          setPartRenameItemVariableValue (
            oldPartName, newPartName);
      }

      fPendingOptionsItem = 0;
    }

    else if (
      // midi tempo item?
      S_msrOptionsMidiTempoItem
        optionsMidiTempoItem =
          dynamic_cast<msrOptionsMidiTempoItem*>(&(*fPendingOptionsItem))
      ) {
      // theString contains the midi tempo specification
      // decipher it to extract duration and perSecond values
      string regularExpression (
        "[[:space:]]*([[:digit:]]+\\.*)[[:space:]]*"
        "="
        "[[:space:]]*([[:digit:]]+)[[:space:]]*");
        
      regex  e (regularExpression);
      smatch sm;

      regex_match (theString, sm, e);

      if (TRACE_OPTIONS) {
        fLogOutputStream <<
          "There are " << sm.size() << " matches" <<
          " for MIDI tempo string '" << theString <<
          "' with regex '" << regularExpression <<
          "'" <<
          endl;
      }
    
      if (sm.size ()) {
        for (unsigned i = 0; i < sm.size (); ++i) {
          fLogOutputStream <<
            "[" << sm [i] << "] ";
        } // for
        fLogOutputStream <<
          endl;
      }
      
      else {
        stringstream s;

        s <<
          "-midiTempo argument '" << theString <<
          "' is ill-formed";
          
        optionError (s.str ());
        
        printSpecificSubGroupHelp (
          fLogOutputStream,
          optionsMidiTempoItem->
            getOptionsSubGroupUplink ());
            
        exit (4);
      }

      string midiTempoDuration  = sm [1];

      int    midiTempoPerSecond;
      {
        stringstream s;
        s << sm [2];
        s >> midiTempoPerSecond;
      }
      
      if (true || TRACE_OPTIONS) {
        fLogOutputStream <<
          "midiTempoDuration  = " <<
          midiTempoDuration <<
          endl <<
          "midiTempoPerSecond = " <<
          midiTempoPerSecond <<
          endl;

      optionsMidiTempoItem->
        setMidiTempoItemVariableValue (
          pair<string, int> (
            midiTempoDuration, midiTempoPerSecond));
      }

      fPendingOptionsItem = 0;
    }
  }

  else {
    // theString is an argument
    fArgumentsVector.push_back (theString);
    fPureHelpRun = false;
  }
}


}
