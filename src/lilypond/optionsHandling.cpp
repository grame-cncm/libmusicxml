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

#include "optionsHandling.h"

#include "utilities.h"


using namespace std;

namespace MusicXML2 
{

// useful shortcut macros
#define idtr indenter::gIndenter
#define tab  indenter::gIndenter.getSpacer ()

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

void msrOptionsElement::print (ostream& os) const
{
  os << "??? msrOptionsElement ???" << endl;
}

void msrOptionsElement::printHelp (ostream& os) const
{
  if (fOptionsElementShortName.size ()) {
    os <<
      idtr <<
       fOptionsElementShortName << " ";
  }
  
  if (fOptionsElementLongName.size ()) {
    os <<
      idtr <<
       fOptionsElementLongName << " ";
  }

  os << endl;

  idtr++;
  
  os << idtr <<
    fOptionsElementDescription <<
    endl;

  idtr--;
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
  string             optionsItemDescription,
  msrOptionsItemKind
                     optionsItemKind)
{
  msrOptionsItem* o = new
    msrOptionsItem (
      optionsItemShortName,
      optionsItemDescription,
      optionsItemDescription,
      optionsItemKind);
  assert(o!=0);
  return o;
}

msrOptionsItem::msrOptionsItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  msrOptionsItem::msrOptionsItemKind
                     optionsItemKind)
  : msrOptionsElement (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription)
{
  fOptionsItemKind = optionsItemKind;
  
  fOptionsItemHasBeenSelected = false;
}
    
msrOptionsItem::~msrOptionsItem()
{}

string msrOptionsItem::optionsItemKindAsString (
  msrOptionsItem::msrOptionsItemKind optionsItemKind)
{
  string result;
  
  switch (optionsItemKind) {
    case msrOptionsItem::kOptionsItemHasNoArgument:
      result = "OptionsItemHasNoArgument";
      break;
    case msrOptionsItem::kOptionsItemHasARequiredArgument:
      result = "OptionsItemHasARequiredArgument";
      break;
    case msrOptionsItem::kOptionsItemHasAnOptionsArgument:
      result = "OptionsItemHasAnOptionsArgument";
      break;
  } // switch

  return result;
}  

void msrOptionsItem::print (ostream& os) const
{
  const int fieldWidth = 19;
  
  os <<
    idtr <<
      "OptionsItem ???:" <<
      endl;

  idtr++;

  os << left <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementShortName" << " : " << fOptionsElementShortName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementLongName" << " : " << fOptionsElementLongName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementDescription" << " : " << fOptionsElementDescription <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsItemKind" << " : " <<
        optionsItemKindAsString (
          fOptionsItemKind) <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsItemHasBeenSelected" << " : " <<
      booleanAsString (
        fOptionsItemHasBeenSelected) <<
      endl;
}

ostream& operator<< (ostream& os, const S_msrOptionsItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOptionsBoolItem msrOptionsBoolItem::create (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription,
  bool&  optionsBoolItemVariable)
{
  msrOptionsBoolItem* o = new
    msrOptionsBoolItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsBoolItemVariable);
  assert(o!=0);
  return o;
}

msrOptionsBoolItem::msrOptionsBoolItem (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription,
  bool&  optionsBoolItemVariable)
  : msrOptionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      msrOptionsItem::kOptionsItemHasNoArgument),
    fOptionsBoolItemVariable (optionsBoolItemVariable)
{}

msrOptionsBoolItem::~msrOptionsBoolItem()
{}

void msrOptionsBoolItem::print (ostream& os) const
{
  const int fieldWidth = 28;
  
  os <<
    idtr <<
      "OptionsBoolItem:" <<
      endl;

  idtr++;

  os << left <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementShortName" << " : " << fOptionsElementShortName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementLongName" << " : " << fOptionsElementLongName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementDescription" << " :" <<
      endl;

  idtr++;
  os << left <<
    idtr <<
      idtr.indentMultiLineString (
        fOptionsElementDescription) <<
      endl;
  idtr--;

  os << left <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsItemHasBeenSelected" << " : " <<
      booleanAsString (
        fOptionsItemHasBeenSelected) <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsBoolItemVariable" << " : " <<
      booleanAsString (
        fOptionsBoolItemVariable) <<
      endl;

  idtr--;
}

ostream& operator<< (ostream& os, const S_msrOptionsBoolItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOptionsIntItem msrOptionsIntItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  int&               optionsIntItemVariable,
  msrOptionsItemKind optionsItemKind)
{
  msrOptionsIntItem* o = new
    msrOptionsIntItem (
      optionsItemShortName,
      optionsItemDescription,
      optionsItemDescription,
      optionsIntItemVariable,
      optionsItemKind);
  assert(o!=0);
  return o;
}

msrOptionsIntItem::msrOptionsIntItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  int&               optionsIntItemVariable,
  msrOptionsItemKind optionsItemKind)
  : msrOptionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsItemKind),
    fOptionsIntItemVariable (optionsIntItemVariable)
{}

msrOptionsIntItem::~msrOptionsIntItem()
{}

void msrOptionsIntItem::print (ostream& os) const
{
  const int fieldWidth = 28;
  
  os <<
    idtr <<
      "OptionsIntItem:" <<
      endl;

  idtr++;

  os << left <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementShortName" << " : " << fOptionsElementShortName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementLongName" << " : " << fOptionsElementLongName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementDescription" << " : " << fOptionsElementDescription <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsItemHasBeenSelected" << " : " <<
      booleanAsString (
        fOptionsItemHasBeenSelected) <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsIntItemVariable" << " : " << fOptionsIntItemVariable <<
      endl;

  idtr--;
}

ostream& operator<< (ostream& os, const S_msrOptionsIntItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOptionsFloatItem msrOptionsFloatItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  float&             optionsFloatItemVariable,
  msrOptionsItemKind optionsItemKind)
{
  msrOptionsFloatItem* o = new
    msrOptionsFloatItem (
      optionsItemShortName,
      optionsItemDescription,
      optionsItemDescription,
      optionsFloatItemVariable,
      optionsItemKind);
  assert(o!=0);
  return o;
}

msrOptionsFloatItem::msrOptionsFloatItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  float&             optionsFloatItemVariable,
  msrOptionsItemKind optionsItemKind)
  : msrOptionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsItemKind),
    fOptionsFloatItemVariable (optionsFloatItemVariable)
{}

msrOptionsFloatItem::~msrOptionsFloatItem()
{}

void msrOptionsFloatItem::print (ostream& os) const
{
  const int fieldWidth = 28;
  
  os <<
    idtr <<
      "OptionsFloatItem:" <<
      endl;

  idtr++;

  os << left <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementShortName" << " : " << fOptionsElementShortName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementLongName" << " : " << fOptionsElementLongName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementDescription" << " : " << fOptionsElementDescription <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsItemHasBeenSelected" << " : " <<
      booleanAsString (
        fOptionsItemHasBeenSelected) <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsFloatItemVariable" << " : " << fOptionsFloatItemVariable <<
      endl;

  idtr--;
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
  string&            optionsStringItemVariable,
  msrOptionsItemKind optionsItemKind)
{
  msrOptionsStringItem* o = new
    msrOptionsStringItem (
      optionsItemShortName,
      optionsItemDescription,
      optionsItemDescription,
      optionsStringItemVariable,
      optionsItemKind);
  assert(o!=0);
  return o;
}

msrOptionsStringItem::msrOptionsStringItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string&            optionsStringItemVariable,
  msrOptionsItemKind optionsItemKind)
  : msrOptionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsItemKind),
    fOptionsStringItemVariable (optionsStringItemVariable)
{}

msrOptionsStringItem::~msrOptionsStringItem()
{}

void msrOptionsStringItem::print (ostream& os) const
{
  const int fieldWidth = 28;
  
  os <<
    idtr <<
      "OptionsStringItem:" <<
      endl;

  idtr++;

  os << left <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementShortName" << " : " << fOptionsElementShortName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementLongName" << " : " << fOptionsElementLongName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementDescription" << " : " << fOptionsElementDescription <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsItemHasBeenSelected" << " : " <<
      booleanAsString (
        fOptionsItemHasBeenSelected) <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsStringItemVariable" << " : " << fOptionsStringItemVariable <<
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
  rational&          optionsRationalItemVariable,
  msrOptionsItemKind optionsItemKind)
{
  msrOptionsRationalItem* o = new
    msrOptionsRationalItem (
      optionsItemShortName,
      optionsItemDescription,
      optionsItemDescription,
      optionsRationalItemVariable,
      optionsItemKind);
  assert(o!=0);
  return o;
}

msrOptionsRationalItem::msrOptionsRationalItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  rational&          optionsRationalItemVariable,
  msrOptionsItemKind optionsItemKind)
  : msrOptionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsItemKind),
    fOptionsRationalItemVariable (optionsRationalItemVariable)
{}

msrOptionsRationalItem::~msrOptionsRationalItem()
{}

void msrOptionsRationalItem::print (ostream& os) const
{
  const int fieldWidth = 28;
  
  os <<
    idtr <<
      "OptionsRationalItem:" <<
      endl;

  idtr++;

  os << left <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementShortName" << " : " << fOptionsElementShortName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementLongName" << " : " << fOptionsElementLongName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementDescription" << " : " << fOptionsElementDescription <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsItemHasBeenSelected" << " : " <<
      booleanAsString (
        fOptionsItemHasBeenSelected) <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsRationalItemVariable" << " : " << fOptionsRationalItemVariable <<
      endl;
}

ostream& operator<< (ostream& os, const S_msrOptionsRationalItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOptionsSubGroup msrOptionsSubGroup::create (
  string optionsSubGroupShortName,
  string optionsSubGroupLongName,
  string optionsSubGroupDescription)
{
  msrOptionsSubGroup* o = new
    msrOptionsSubGroup (
      optionsSubGroupShortName,
      optionsSubGroupLongName,
      optionsSubGroupDescription);
  assert(o!=0);
  return o;
}

msrOptionsSubGroup::msrOptionsSubGroup (
  string optionsSubGroupShortName,
  string optionsSubGroupLongName,
  string optionsSubGroupDescription)
  : msrOptionsElement (
      optionsSubGroupShortName,
      optionsSubGroupLongName,
      optionsSubGroupDescription)
{}

msrOptionsSubGroup::~msrOptionsSubGroup()
{}

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
      idtr <<
     "OptionsSubGroup:" <<
      endl;

  idtr++;

  os << left <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementShortName" << " : " << fOptionsElementShortName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementLongName" << " : " << fOptionsElementLongName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementDescription" << " : " << fOptionsElementDescription <<
      endl <<
    endl;

  os << idtr <<
    "Options items (" <<
    singularOrPlural (
      fOptionsSubGroupItemsList.size (), "element",  "elements") <<
    "):" <<
    endl;

  if (fOptionsSubGroupItemsList.size ()) {
    os << endl;
    
    idtr++;
    
    list<S_msrOptionsItem>::const_iterator
      iBegin = fOptionsSubGroupItemsList.begin(),
      iEnd   = fOptionsSubGroupItemsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      // print the item
      os << (*i);
      if (++i == iEnd) break;
      cerr << endl;
    } // for

    idtr--;
  }

  idtr--;
  
  os << endl;
}

ostream& operator<< (ostream& os, const S_msrOptionsSubGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOptionsGroup msrOptionsGroup::create (
  string optionGroupShortName,
  string optionGroupLongName,
  string optionGroupDescription)
{
  msrOptionsGroup* o = new
    msrOptionsGroup (
      optionGroupShortName,
      optionGroupLongName,
      optionGroupDescription);
  assert(o!=0);
  return o;
}

msrOptionsGroup::msrOptionsGroup (
  string optionGroupShortName,
  string optionGroupLongName,
  string optionGroupDescription)
  : msrOptionsElement (
      optionGroupShortName,
      optionGroupLongName,
      optionGroupDescription)
{}

msrOptionsGroup::~msrOptionsGroup()
{}

void  msrOptionsGroup::appendOptionsSubGroup (
  S_msrOptionsSubGroup optionsSubGroup)
{
  fOptionsGroupSubGroupsList.push_back (
    optionsSubGroup);
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
    idtr <<
      "OptionsGroup:" <<
      endl;

  idtr++;

  os <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementShortName" << " : " << fOptionsElementShortName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementLongName" << " : " << fOptionsElementLongName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementDescription" << " : " << fOptionsElementDescription <<
      endl <<
    endl;

  os << idtr <<
    "Options subgroups (" <<
    singularOrPlural (
      fOptionsGroupSubGroupsList.size (), "element",  "elements") <<
    "):" <<
    endl;

  if (fOptionsGroupSubGroupsList.size ()) {
    os << endl;
    
    idtr++;
    
    list<S_msrOptionsSubGroup>::const_iterator
      iBegin = fOptionsGroupSubGroupsList.begin(),
      iEnd   = fOptionsGroupSubGroupsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      // print the subgroup
      os << (*i);
      if (++i == iEnd) break;
      cerr << endl;
    } // for

    idtr--;
  }

  idtr--;
  
  os << endl;
}

ostream& operator<< (ostream& os, const S_msrOptionsGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOptionsGroupsList msrOptionsGroupsList::create (
  string optionGroupsListShortName,
  string optionGroupsListLongName,
  string optionGroupsListDescription)
{
  msrOptionsGroupsList* o = new
    msrOptionsGroupsList (
      optionGroupsListShortName,
      optionGroupsListLongName,
      optionGroupsListDescription);
  assert(o!=0);
  return o;
}

msrOptionsGroupsList::msrOptionsGroupsList (
  string optionGroupsListShortName,
  string optionGroupsListLongName,
  string optionGroupsListDescription)
  : msrOptionsElement (
      optionGroupsListShortName,
      optionGroupsListLongName,
      optionGroupsListDescription)
{}

msrOptionsGroupsList::~msrOptionsGroupsList()
{}

void msrOptionsGroupsList::appendOptionsGroup (
  S_msrOptionsGroup optionsGroup)
{
  fOptionsGroupGroupsList.push_back (
    optionsGroup);
}

void msrOptionsGroupsList::print (ostream& os) const
{
  const int fieldWidth = 19;
  
  os <<
    idtr <<
    "OptionsGroupsList:" <<
    endl <<
    setw(fieldWidth) <<
    "fOptionsElementShortName" << " : " << fOptionsElementShortName <<
    endl <<
    setw(fieldWidth) <<
    "fOptionsElementLongName" << " : " << fOptionsElementLongName <<
    endl <<
    setw(fieldWidth) <<
    "fOptionsElementDescription" << " : " << fOptionsElementDescription <<
    endl;

  for (
    list<S_msrOptionsGroup>::const_iterator
      i = fOptionsGroupGroupsList.begin();
    i != fOptionsGroupGroupsList.end();
    i++) {
    // print the element
    os << (*i);
  } // for
}

ostream& operator<< (ostream& os, const S_msrOptionsGroupsList& elt)
{
  os <<
    "OptionsGroupsList:" <<
    endl;
  elt->print (os);
  return os;
}

//______________________________________________________________________________
/* JMI
S_msrOptionsHandler msrOptionsHandler::create (
  string optionHandlerShortName,
  string optionHandlerLongName,
  string optionHandlerDescription)
{
  msrOptionsHandler* o = new
    msrOptionsHandler (
      optionHandlerShortName,
      optionHandlerLongName,
      optionHandlerDescription);
  assert(o!=0);
  return o;
}
*/

msrOptionsHandler::msrOptionsHandler (
  string optionHandlerShortName,
  string optionHandlerLongName,
  string optionHandlerDescription)
  : msrOptionsElement (
      optionHandlerShortName,
      optionHandlerLongName,
      optionHandlerDescription)
{}

msrOptionsHandler::~msrOptionsHandler()
{}

void msrOptionsHandler::registerOptionsElement (
  S_msrOptionsElement optionsElement)
{
  string
    optionLongName =
      optionsElement->getOptionsElementLongName (),
    optionShortName =
      optionsElement->getOptionsElementLongName ();

  if (
    optionShortName.size () == 0
      &&
    optionLongName.size () == 0) {
    stringstream s;

    s <<
      "option long name and short name are both empty";
      
    optionError (s.str());
  }
  
  if (optionShortName == optionLongName) {
    stringstream s;

    s <<
      "option long name '" << optionLongName << "'" <<
      " is the same as the short name";
      
    optionError (s.str());
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
        
      optionError (s.str());
    }

    // is optionShortName already in the options names map?
    if ((*i).first == optionShortName) {
      if (optionShortName.size ()) {
        stringstream s;
    
        s <<
          "option short name '" << optionShortName << "'" <<
          " for option long name '" << optionLongName << "'" <<
          " is specified more that once";
          
        optionError (s.str());
      }
    }
  } // for

  // everything OK, register optionsElement in the options element map
  if (optionLongName.size ())
    fOptionsElementsMap [optionLongName] = optionsElement;
  if (optionShortName.size ())
    fOptionsElementsMap [optionShortName] = optionsElement;
}

void msrOptionsHandler::print (ostream& os) const
{
  const int fieldWidth = 27;
  
  os <<
    idtr <<
    "OptionsHandler:" <<
    endl;

  idtr++;
  
  os << left <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementShortName" << " : " << fOptionsElementShortName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementLongName" << " : " << fOptionsElementLongName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementDescription" << " : " << fOptionsElementDescription <<
      endl;

  for (
    list<S_msrOptionsGroup>::const_iterator
      i = fOptionsHandlerOptionsGroupsList.begin();
    i != fOptionsHandlerOptionsGroupsList.end();
    i++) {
    // print the element
    os << (*i);
  } // for

  idtr--;
}

ostream& operator<< (ostream& os, const S_msrOptionsHandler& elt)
{
  os <<
    "OptionsHandler:" <<
    endl;
  elt->print (os);
  return os;
}

/* JMI
  S_msrOptionsGroupsList optionsGroupsList =
    new msrOptionsGroupsList (
    "h", "help", " help for OptionsGroupsList");

  optionsGroupsList->
    appendOptionsGroup (optionsGroup);
    
  cerr <<
    optionsGroupsList <<
    endl;
    */

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

const vector<string> msrOptionsHandler::analyzeOptions (
  int   argc,
  char* argv[])
{
  // print the options elements map  
  cerr << idtr <<
    "Options elements map (" <<
    fOptionsElementsMap.size () <<
    " elements):" <<
    endl;
  if (fOptionsElementsMap.size ()) {
    idtr++;
    
    map<string, S_msrOptionsElement>::const_iterator
      iBegin = fOptionsElementsMap.begin(),
      iEnd   = fOptionsElementsMap.end(),
      i      = iBegin;
    for ( ; ; ) {
      cerr << (*i).second;
      if (++i == iEnd) break;
      cerr << endl;
    } // for
    
    idtr--;
  }
  cerr << endl;
  
  vector<string> argumentsVector;
  
  int n = 0;

  while (true) { 
    if (argv [n] == 0)
      break;

    string currentElement = string (argv [n]);
    
    // print current element
    cout <<
      "Command line elemnt " << n <<
      ": " <<currentElement << " "<<
      endl;

    // handle current element
    if (currentElement [0] == '-') {
      if (currentElement.size () == 1) {
        // this is 
          cout <<
            "'" << currentElement << "' is the '-' stdin indicator" <<
            endl;
        argumentsVector.push_back (currentElement);
      }
      
      else {
        // this is an option
        string currentOptionName;

        string elementTrailer =
          currentElement.substr (1, string::npos);
  
        /* JMI
        cout <<
          "elementTrailer '" << elementTrailer << "' is preceded by a dash" <<
          endl;
        */
  
        if (elementTrailer.size ()) {
          if (elementTrailer [0] == '-') {
            // it is a double-dashed option
            currentOptionName =
              elementTrailer.substr (1, string::npos);
            
            cout <<
              "'" << currentOptionName << "' is a double-dashed option" <<
              endl;
          }
          else {
            // it is a single-dashed option
            currentOptionName =
              elementTrailer; //.substr (1, string::npos);
            
            cout <<
              "'" << currentOptionName << "' is a single-dashed option" <<
              endl;
          }
        }
        
        else {
          cout <<
            "'-' is the minimal single-dashed option" <<
            endl;
        }

      // is currentOptionName known in options elements map?
      /*
      map<string, S_msrOptionsElement>::const_iterator
        it =
          fOptionsElementsMap.find (currentOptionName);
            
      if (it == fOptionsElementsMap.end ()) {
        // no, currentOptionName is unknown in the map    
        stringstream s;
    
        s <<
          "option name '" << currentOptionName <<
          "' is unknown";
          
        optionError (s.str());
      }
      */

      S_msrOptionsElement
        optionsElement =
          fetchOptionElement (currentOptionName);

      if (! optionsElement) {
        // currentOptionName is unknown to this options handler 
        stringstream s;
    
        s <<
          "option name '" << currentOptionName <<
          "' is unknown";
          
        optionError (s.str());
      }
      
      else {
        // currentOptionName is known, let's handle it
/*
        S_msrOptionsElement
          optionsElement = (*it).second;
*/
        if (
          S_msrOptionsGroup
            optionsGroup =
              dynamic_cast<msrOptionsGroup*>(&(*optionsElement))
          ) {    
          optionsGroup->printHelp (cerr);
        }
        
        else if (
          S_msrOptionsSubGroup
            optionsSubGroup =
              dynamic_cast<msrOptionsSubGroup*>(&(*optionsElement))
          ) {    
          optionsSubGroup->printHelp (cerr);
        }
        
        else if (
          S_msrOptionsBoolItem
            optionsBoolItem =
              dynamic_cast<msrOptionsBoolItem*>(&(*optionsElement))
          ) {
          optionsBoolItem->
            setBoolItemVariableValue (true);
        }
        
        else if (
          S_msrOptionsIntItem
            optionsIntItem =
              dynamic_cast<msrOptionsIntItem*>(&(*optionsElement))
          ) {
          optionsIntItem->
            setIntItemVariableValue (1999);
        }
        
        else if (
          S_msrOptionsFloatItem
            optionsFloatItem =
              dynamic_cast<msrOptionsFloatItem*>(&(*optionsElement))
          ) {
          optionsFloatItem->
            setFloatItemVariableValue (2017.9);
        }
        
        else if (
          S_msrOptionsStringItem
            optionsStringItem =
              dynamic_cast<msrOptionsStringItem*>(&(*optionsElement))
          ) {
          optionsStringItem->
            setStringItemVariableValue ("september");
        }
        
        else if (
          S_msrOptionsRationalItem
            optionsRationalItem =
              dynamic_cast<msrOptionsRationalItem*>(&(*optionsElement))
          ) {
          optionsRationalItem->
            setRationalItemVariableValue (rational (3, 4));
        }
        
        else {
          stringstream s;
      
          s <<
            "option name '" << currentOptionName <<
            "' is known but cannot be handled";
            
          optionError (s.str());
        }
      }
    }
  } // JMI ???

    // next please
    n++;
  } // while

  // print the arguments vector
  cerr << idtr <<
    "Arguments vector (" <<
    argumentsVector.size () <<
    " elements):" <<
    endl;

  if (argumentsVector.size ()) {
    idtr++;
    for (unsigned int i = 0; i < argumentsVector.size (); i++) {
      cerr << idtr <<
        argumentsVector [i] <<
        endl;
    } // for
    idtr--;
  }
  
  return argumentsVector;
}


}
