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

// useful shortcut macros
#define idtr indenter::gIndenter
#define tab  indenter::gIndenter.getSpacer ()

#define TRACE_OPTIONS 0

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

void msrOptionsElement::printHeader (ostream& os) const
{
  os <<
    idtr <<
      "-" << fOptionsElementShortName <<
      endl <<
    idtr <<
      "-" << fOptionsElementLongName <<
      endl;

  idtr++; idtr++; idtr++;
  
  os <<
    idtr <<
      idtr.indentMultiLineString (
        fOptionsElementDescription) <<
      endl;

  idtr--; idtr--; idtr--;
}

void msrOptionsElement::underlineHeader (ostream& os) const
{
  os <<
    idtr;
  for (unsigned int i = 0; i < fOptionsElementDescription.size (); i++) {
    os << "-";
  } // for
  os <<
    endl;
}

void msrOptionsElement::print (ostream& os) const
{
  os << "??? msrOptionsElement ???" << endl;
}

void msrOptionsElement::printHelp (ostream& os) const
{
  if (
    fOptionsElementShortName.size ()
        &&
    fOptionsElementLongName.size ()
    ) {
      os << idtr <<
        "-" << fOptionsElementShortName <<
        ", " <<
        "-" << fOptionsElementLongName;
  }
  
  else {
    if (fOptionsElementShortName.size ()) {
      os << idtr <<
      "-" << fOptionsElementShortName;
    }
    if (fOptionsElementLongName.size ()) {
      os << idtr <<
      "-" << fOptionsElementLongName;
    }
  }

  os <<
    endl;

  // indent a bit more for readability
  idtr++; idtr++; idtr++;
  
  os << idtr <<
    idtr.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;

  idtr--; idtr--; idtr--;
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
  int                optionsItemValuesNumber)
{
  msrOptionsItem* o = new
    msrOptionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsItemValuesNumber);
  assert(o!=0);
  return o;
}

msrOptionsItem::msrOptionsItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  int                optionsItemValuesNumber)
  : msrOptionsElement (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription)
{
  fOptionsItemValuesNumber = optionsItemValuesNumber;
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

void msrOptionsItem::registerOptionsItemInHandler (
  S_msrOptionsHandler optionsHandler)
{
  optionsHandler->
    registerOptionsElementNamesInHandler (this);
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
      "fOptionsElementShortName" << " : " <<
      fOptionsElementShortName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementLongName" << " : " <<
      fOptionsElementLongName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementDescription" << " : " <<
      fOptionsElementDescription <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsItemValuesNumber" << " : " <<
      fOptionsItemValuesNumber <<
      endl;
}

void msrOptionsItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os <<
    idtr <<
      "OptionsItem values ???:" <<
      endl;
}

ostream& operator<< (ostream& os, const S_msrOptionsItem& elt)
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
      optionsItemDescription,
      0),
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
    idtr <<
      "OptionsBooleanItem:" <<
      endl;

  idtr++;

  os << left <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementShortName" << " : " <<
      fOptionsElementShortName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementLongName" << " : " <<
      fOptionsElementLongName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementDescription" << " :" <<
      endl;

  idtr++;
  os <<
    idtr <<
      idtr.indentMultiLineString (
        fOptionsElementDescription) <<
      endl;
  idtr--;

  os << left <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsBooleanItemVariableDisplayName" << " : " <<
      fOptionsBooleanItemVariableDisplayName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsBooleanItemVariable" << " : " <<
      booleanAsString (
        fOptionsBooleanItemVariable) <<
      endl;

  idtr--;
}

void msrOptionsBooleanItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    idtr <<
      setw(valueFieldWidth) <<
      "|" <<
      fOptionsBooleanItemVariableDisplayName <<
      " : " <<
      "|" <<
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
S_msrOptionsValuedItem msrOptionsValuedItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  int                optionsItemValuesNumber)
{
  msrOptionsValuedItem* o = new
    msrOptionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsItemValuesNumber);
  assert(o!=0);
  return o;
}

msrOptionsValuedItem::msrOptionsValuedItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  int                optionsItemValuesNumber)
  : msrOptionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsItemValuesNumber)
{
  fOptionsValueSpecification = optionsValueSpecification;
  
  fOptionsItemValuesNumber = optionsItemValuesNumber;
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
    registerOptionsElementNamesInHandler (this);
}

void msrOptionsValuedItem::print (ostream& os) const
{
  const int fieldWidth = 19;
  
  os <<
    idtr <<
      "OptionsValuedItem ???:" <<
      endl;

  idtr++;

  os << left <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementShortName" << " : " <<
      fOptionsElementShortName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementLongName" << " : " <<
      fOptionsElementLongName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementDescription" << " : " <<
      fOptionsElementDescription <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsValueSpecification" << " : " <<
      fOptionsValueSpecification <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsItemValuesNumber" << " : " <<
      fOptionsItemValuesNumber <<
      endl;
}

void msrOptionsValuedItem::printHelp (ostream& os) const
{
  if (
    fOptionsElementShortName.size ()
        &&
    fOptionsElementLongName.size ()
    ) {
      os << idtr <<
        "-" << fOptionsElementShortName <<
        ", " <<
        "-" << fOptionsElementLongName;
  }
  
  else {
    if (fOptionsElementShortName.size ()) {
      os << idtr <<
      "-" << fOptionsElementShortName;
    }
    if (fOptionsElementLongName.size ()) {
      os << idtr <<
      "-" << fOptionsElementLongName;
    }
  }

  os <<
    " " <<
    fOptionsValueSpecification;

  os <<
    endl;

  // indent a bit more for readability
  idtr++; idtr++; idtr++;
  
  os << idtr <<
    idtr.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;

  idtr--; idtr--; idtr--;
}

void msrOptionsValuedItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os <<
    idtr <<
      "OptionsItem values ???:" <<
      endl;
}

ostream& operator<< (ostream& os, const S_msrOptionsValuedItem& elt)
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
      optionsValueSpecification,
      1),
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
    idtr <<
      "OptionsIntegerItem:" <<
      endl;

  idtr++;

  os << left <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementShortName" << " : " <<
      fOptionsElementShortName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementLongName" << " : " <<
      fOptionsElementLongName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementDescription" << " : " <<
      fOptionsElementDescription <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsIntegerItemVariableDisplayName" << " : " <<
      fOptionsIntegerItemVariableDisplayName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsIntegerItemVariable" << " : " <<
      fOptionsIntegerItemVariable <<
      endl;

  idtr--;
}

void msrOptionsIntegerItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    idtr <<
      setw(valueFieldWidth) <<
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
      optionsValueSpecification,
      1),
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
    idtr <<
      "OptionsFloatItem:" <<
      endl;

  idtr++;

  os << left <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementShortName" << " : " <<
      fOptionsElementShortName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementLongName" << " : " <<
      fOptionsElementLongName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementDescription" << " : " <<
      fOptionsElementDescription <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsFloatItemVariableDisplayName" << " : " <<
      fOptionsFloatItemVariableDisplayName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsFloatItemVariable" << " : " <<
      fOptionsFloatItemVariable <<
      endl;

  idtr--;
}

void msrOptionsFloatItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    idtr <<
      setw(valueFieldWidth) <<
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
      optionsValueSpecification,
      1),
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
    idtr <<
      "OptionsStringItem:" <<
      endl;

  idtr++;

  os << left <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementShortName" << " : " <<
      fOptionsElementShortName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementLongName" << " : " <<
      fOptionsElementLongName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementDescription" << " : " <<
      fOptionsElementDescription <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsValueSpecification" << " : " <<
      fOptionsValueSpecification <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsStringItemVariableDisplayName" << " : " <<
      fOptionsStringItemVariableDisplayName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsStringItemVariable" << " : " <<
      fOptionsStringItemVariable <<
      endl;
}

void msrOptionsStringItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    idtr <<
      setw(valueFieldWidth) <<
      fOptionsStringItemVariableDisplayName <<
      " : " <<
      fOptionsStringItemVariable <<
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
      optionsValueSpecification,
      1),
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
    idtr <<
      "OptionsRationalItem:" <<
      endl;

  idtr++;

  os << left <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementShortName" << " : " <<
      fOptionsElementShortName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementLongName" << " : " <<
      fOptionsElementLongName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementDescription" << " : " <<
      fOptionsElementDescription <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsRationalItemVariableDisplayName" << " : " <<
      fOptionsRationalItemVariableDisplayName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsRationalItemVariable" << " : " <<
      fOptionsRationalItemVariable <<
      endl;
}

void msrOptionsRationalItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    idtr <<
      setw(valueFieldWidth) <<
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
      optionsValueSpecification,
      1),
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
    idtr <<
      "OptionsNumbersSetItem:" <<
      endl;

  idtr++;

  os << left <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementShortName" << " : " <<
      fOptionsElementShortName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementLongName" << " : " <<
      fOptionsElementLongName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementDescription" << " : " <<
      fOptionsElementDescription <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsNumbersSetItemVariableDisplayName" << " : " <<
      fOptionsNumbersSetItemVariableDisplayName <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsNumbersSetItemVariable" << " : " <<
      endl;

    if (fOptionsNumbersSetItemVariable.empty ()) {
      cerr << "none";
    }
      
    else {
      cerr <<
        "'";
        
      set<int>::const_iterator
        iBegin = fOptionsNumbersSetItemVariable.begin(),
        iEnd   = fOptionsNumbersSetItemVariable.end(),
        i      = iBegin;
        
      for ( ; ; ) {
        cerr << (*i);
        if (++i == iEnd) break;
        cerr << " ";
      } // for
    
      cerr <<
        "'";
    }

    cerr <<
      endl;
}

void msrOptionsNumbersSetItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    idtr <<
      setw(valueFieldWidth) <<
      fOptionsNumbersSetItemVariableDisplayName <<
      " : ";

  if (fOptionsNumbersSetItemVariable.empty ()) {
    cerr << "none";
  }
    
  else {
    cerr <<
      "'";
      
    set<int>::const_iterator
      iBegin = fOptionsNumbersSetItemVariable.begin(),
      iEnd   = fOptionsNumbersSetItemVariable.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      cerr << (*i);
      if (++i == iEnd) break;
      cerr << " ";
    } // for
  
    cerr <<
      "'";
  }

  cerr <<
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
      optionsValueSpecification,
      1),
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
    idtr <<
      "OptionsPitchesLanguageItem:" <<
      endl;

  idtr++;

  os << left <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementShortName" << " : " <<
      fOptionsElementShortName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementLongName" << " : " <<
      fOptionsElementLongName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementDescription" << " : " <<
      fOptionsElementDescription <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsPitchesLanguageItemVariableDisplayName" << " : " <<
      fOptionsPitchesLanguageItemVariableDisplayName <<
    idtr <<
      setw(fieldWidth) <<
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
    idtr <<
      setw(valueFieldWidth) <<
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
      optionsValueSpecification,
      1),
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
    idtr <<
      "OptionsChordsLanguageItem:" <<
      endl;

  idtr++;

  os << left <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementShortName" << " : " <<
      fOptionsElementShortName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementLongName" << " : " <<
      fOptionsElementLongName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementDescription" << " : " <<
      fOptionsElementDescription <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsChordsLanguageItemVariableDisplayName" << " : " <<
      fOptionsChordsLanguageItemVariableDisplayName <<
    idtr <<
      setw(fieldWidth) <<
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
    idtr <<
      setw(valueFieldWidth) <<
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
S_msrOptionsStringsMapItem msrOptionsStringsMapItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsStringsMapItemVariableDisplayName,
  map<string, string>&
                     optionsStringsMapItemVariable)
{
  msrOptionsStringsMapItem* o = new
    msrOptionsStringsMapItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsStringsMapItemVariableDisplayName,
      optionsStringsMapItemVariable);
  assert(o!=0);
  return o;
}

msrOptionsStringsMapItem::msrOptionsStringsMapItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsStringsMapItemVariableDisplayName,
  map<string, string>&
                     optionsStringsMapItemVariable)
  : msrOptionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      1),
    fOptionsStringsMapItemVariableDisplayName (
      optionsStringsMapItemVariableDisplayName),
    fOptionsStringsMapItemVariable (
      optionsStringsMapItemVariable)
{}

msrOptionsStringsMapItem::~msrOptionsStringsMapItem()
{}

void msrOptionsStringsMapItem::print (ostream& os) const
{
  const int fieldWidth = FIELD_WIDTH;
  
  os <<
    idtr <<
      "OptionsStringsMapItem:" <<
      endl;

  idtr++;

  os << left <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementShortName" << " : " <<
      fOptionsElementShortName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementLongName" << " : " <<
      fOptionsElementLongName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementDescription" << " : " <<
      fOptionsElementDescription <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsStringsMapItemVariableDisplayName" << " : " <<
      fOptionsStringsMapItemVariableDisplayName <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsStringsMapItemVariable" << " : \"" <<
      map<string, string>AsString (
        fOptionsStringsMapItemVariable) <<
        "\"" <<
      endl;
}

void msrOptionsStringsMapItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    idtr <<
      setw(valueFieldWidth) <<
      fOptionsStringsMapItemVariableDisplayName <<
      " : \"" <<
      map<string, string>AsString (
        fOptionsStringsMapItemVariable) <<
      "\"" <<
      endl;
}

ostream& operator<< (ostream& os, const S_msrOptionsStringsMapItem& elt)
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

void msrOptionsSubGroup::registerOptionsSubGroupInHandler (
  S_msrOptionsHandler optionsHandler)
{
  optionsHandler->
    registerOptionsElementNamesInHandler (this);

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
      "fOptionsElementDescription" << " : " << fOptionsElementDescription <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementShortName" << " : " << fOptionsElementShortName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementLongName" << " : " << fOptionsElementLongName <<
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
      // print the options item
      os << (*i);
      if (++i == iEnd) break;
      cerr << endl;
    } // for

    idtr--;
  }

  idtr--;
}

void msrOptionsSubGroup::printHelp (ostream& os) const
{
  
  // the description is the header of the information
  os << idtr <<
    fOptionsElementDescription;

  if (
    fOptionsElementShortName.size ()
        &&
    fOptionsElementLongName.size ()
    ) {
      os <<
        " (" <<
        "-" << fOptionsElementShortName <<
        ", " <<
        "-" << fOptionsElementLongName <<
        ")";
  }
  
  else {
    if (fOptionsElementShortName.size ()) {
      os <<
      " (" <<
      "-" << fOptionsElementShortName <<
      ")";
    }
    if (fOptionsElementLongName.size ()) {
      os <<
      " (" <<
      "-" << fOptionsElementLongName <<
      ")";
    }
  }

  os <<
    ":" <<
    endl;

  if (fOptionsSubGroupItemsList.size ()) {
    os << endl;
    
    idtr++;

    list<S_msrOptionsItem>::const_iterator
      iBegin = fOptionsSubGroupItemsList.begin(),
      iEnd   = fOptionsSubGroupItemsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      // print the options item help
      (*i)->printHelp (os);
      if (++i == iEnd) break;
      cerr << endl;
    } // for

    idtr--;
  }
}

void msrOptionsSubGroup::printHelpSummary (ostream& os) const
{
  // the description is the header of the information
  os << idtr <<
    fOptionsElementDescription;

  if (
    fOptionsElementShortName.size ()
        &&
    fOptionsElementLongName.size ()
    ) {
      os <<
        " (" <<
        "-" << fOptionsElementShortName <<
        ", " <<
        "-" << fOptionsElementLongName <<
        ")";
  }
  
  else {
    if (fOptionsElementShortName.size ()) {
      os <<
      " (" <<
      "-" << fOptionsElementShortName <<
      ")";
    }
    if (fOptionsElementLongName.size ()) {
      os <<
      " (" <<
      "-" << fOptionsElementLongName <<
      ")";
    }
  }

  // underline the options subgroup header
  underlineHeader (os);
}

void msrOptionsSubGroup::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // the description is the header of the information
  os << idtr <<
    fOptionsElementDescription;

/* JMI
  if (
    fOptionsElementShortName.size ()
        &&
    fOptionsElementLongName.size ()
    ) {
      os <<
        " (" <<
        "-" << fOptionsElementShortName <<
        ", " <<
        "-" << fOptionsElementLongName <<
        ")";
  }
  
  else {
    if (fOptionsElementShortName.size ()) {
      os <<
      " (" <<
      "-" << fOptionsElementShortName <<
      ")";
    }
    if (fOptionsElementLongName.size ()) {
      os <<
      " (" <<
      "-" << fOptionsElementLongName <<
      ")";
    }
  }
  */

  os <<
    ":" <<
    endl;

  // underline the options subgroup header
 // JMI underlineHeader (os);

  // print the options items values
  if (fOptionsSubGroupItemsList.size ()) {
  // JMI  os << endl;
    
    idtr++;

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
  //    cerr << endl;
    } // for

    idtr--;
  }
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

void msrOptionsGroup::registerOptionsGroupInHandler (
  S_msrOptionsHandler optionsHandler)
{
  optionsHandler->
    registerOptionsElementNamesInHandler (this);

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
      "fOptionsElementDescription" << " : " << fOptionsElementDescription <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementShortName" << " : " << fOptionsElementShortName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementLongName" << " : " << fOptionsElementLongName <<
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
      // print the options subgroup
      os << (*i);
      if (++i == iEnd) break;
      cerr << endl;
    } // for

    idtr--;
  }

  idtr--;
}

void msrOptionsGroup::printHelp (ostream& os) const
{
  // the description is the header of the information
  os << idtr <<
    fOptionsElementDescription;

  if (
    fOptionsElementShortName.size ()
        &&
    fOptionsElementLongName.size ()
    ) {
      os <<
        " (" <<
        "-" << fOptionsElementShortName <<
        ", " <<
        "-" << fOptionsElementLongName <<
        ")";
  }
  
  else {
    if (fOptionsElementShortName.size ()) {
      os <<
      " (" <<
      "-" << fOptionsElementShortName <<
      ")";
    }
    if (fOptionsElementLongName.size ()) {
      os <<
      " (" <<
      "-" << fOptionsElementLongName <<
      ")";
    }
  }

  os <<
    ":" <<
    endl;

  // underline the options group header
  underlineHeader (os);

  // print the options subgroups
  if (fOptionsGroupSubGroupsList.size ()) {
    os << endl;
    
    idtr++;

    list<S_msrOptionsSubGroup>::const_iterator
      iBegin = fOptionsGroupSubGroupsList.begin(),
      iEnd   = fOptionsGroupSubGroupsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup help
      (*i)->printHelp (os);
      if (++i == iEnd) break;
      cerr << endl;
    } // for

    idtr--;
  }
}

void msrOptionsGroup::printHelpSummary (ostream& os) const
{
  // the description is the header of the information
  os << idtr <<
    fOptionsElementDescription;

  if (
    fOptionsElementShortName.size ()
        &&
    fOptionsElementLongName.size ()
    ) {
      os <<
        " (" <<
        "-" << fOptionsElementShortName <<
        ", " <<
        "-" << fOptionsElementLongName <<
        ")";
  }
  
  else {
    if (fOptionsElementShortName.size ()) {
      os <<
      " (" <<
      "-" << fOptionsElementShortName <<
      ")";
    }
    if (fOptionsElementLongName.size ()) {
      os <<
      " (" <<
      "-" << fOptionsElementLongName <<
      ")";
    }
  }

  os <<
    ":" <<
    endl;

  // underline the options group header
  underlineHeader (os);

  // print the options subgroups
  if (fOptionsGroupSubGroupsList.size ()) {
    os << endl;
    
    idtr++;

    list<S_msrOptionsSubGroup>::const_iterator
      iBegin = fOptionsGroupSubGroupsList.begin(),
      iEnd   = fOptionsGroupSubGroupsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup description
      os << idtr;
      (*i)->printHelpSummary (os);
      if (++i == iEnd) break;
      cerr << endl;
    } // for

    idtr--;
  }
}

void msrOptionsGroup::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // the description is the header of the information
  os << idtr <<
    fOptionsElementDescription;

  os <<
    ":" <<
    endl;

  // underline the options group header
  underlineHeader (os);

  // print the options subgroups values
  if (fOptionsGroupSubGroupsList.size ()) {
 // JMI   os << endl;
    
    idtr++;

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
// JMI      cerr << endl;
    } // for

    idtr--;
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
  string optionsHandlerHelpHeader,
  string optionHandlerShortName,
  string optionHandlerLongName,
  string optionHandlerDescription)
  : msrOptionsElement (
      optionHandlerShortName,
      optionHandlerLongName,
      optionHandlerDescription)
{
  fOptionsHandlerHelpHeader = optionsHandlerHelpHeader;

  fExpectedValuesNumber = 0;
  
  fMaximumDisplayNameWidth = 1;
}

msrOptionsHandler::~msrOptionsHandler()
{}

void msrOptionsHandler::registerOptionsHandlerInSelf ()
{
  this->
    registerOptionsElementNamesInHandler (this);

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

void msrOptionsHandler::registerOptionsElementNamesInHandler (
  S_msrOptionsElement optionsElement)
{
  string
    optionLongName =
      optionsElement->getOptionsElementLongName (),
    optionShortName =
      optionsElement->getOptionsElementShortName ();

  int
    optionLongNameSize =
      optionLongName.size (),
    optionShortNameSize =
      optionShortName.size ();

  if (
    optionShortNameSize == 0
      &&
    optionLongNameSize == 0) {
    stringstream s;

    s <<
      "option long name and short name are both empty";
      
    optionError (s.str());
    abort ();
  }
  
  if (optionShortName == optionLongName) {
    stringstream s;

    s <<
      "option long name '" << optionLongName << "'" <<
      " is the same as the short name for the same";
      
    optionError (s.str());
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
        
      optionError (s.str());
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
          
        optionError (s.str());
        abort ();
      }
    }
  } // for

  // everything OK, register optionsElement in the options element map
  if (optionLongNameSize) {
    fOptionsElementsMap [optionLongName] =
      optionsElement;
    
    if (optionLongNameSize > fMaximumDisplayNameWidth)
      fMaximumDisplayNameWidth = optionLongNameSize;
  }
  
  if (optionShortNameSize) {
    fOptionsElementsMap [optionShortName] =
      optionsElement;
    
    if (optionShortNameSize > fMaximumDisplayNameWidth)
      fMaximumDisplayNameWidth = optionShortNameSize;
  }
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
      "fOptionsElementDescription" << " : " << fOptionsElementDescription <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementShortName" << " : " << fOptionsElementShortName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementLongName" << " : " << fOptionsElementLongName <<
      endl;

  if (fOptionsHandlerOptionsGroupsList.size ()) {
    os << endl;
    
    idtr++;
    
    list<S_msrOptionsGroup>::const_iterator
      iBegin = fOptionsHandlerOptionsGroupsList.begin(),
      iEnd   = fOptionsHandlerOptionsGroupsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group
      os << (*i);
      if (++i == iEnd) break;
      cerr << endl;
    } // for

    idtr--;
  }

  idtr--;
}

void msrOptionsHandler::printHelp (ostream& os) const
{
  // the description is the header of the information
  os << idtr <<
    fOptionsElementDescription;

  if (
    fOptionsElementShortName.size ()
        &&
    fOptionsElementLongName.size ()
    ) {
      os <<
        " (" <<
        "-" << fOptionsElementShortName <<
        ", " <<
        "-" << fOptionsElementLongName <<
        ")";
  }
  
  else {
    if (fOptionsElementShortName.size ()) {
      os <<
      " (" <<
      "-" << fOptionsElementShortName <<
      ")";
    }
    if (fOptionsElementLongName.size ()) {
      os <<
      " (" <<
      "-" << fOptionsElementLongName <<
      ")";
    }
  }

  os <<
    ":" <<
    endl;

  if (fOptionsHandlerOptionsGroupsList.size ()) {
  // JMI  os << endl;
    
    idtr++;

    list<S_msrOptionsGroup>::const_iterator
      iBegin = fOptionsHandlerOptionsGroupsList.begin(),
      iEnd   = fOptionsHandlerOptionsGroupsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group help
      (*i)->printHelp (os);
      if (++i == iEnd) break;
      cerr << endl;
    } // for

    idtr--;
  }
  
  os <<
    endl;
}

void msrOptionsHandler::printHelpSummary (ostream& os) const
{
  // the description is the header of the information
  os << idtr <<
    fOptionsElementDescription;

  if (
    fOptionsElementShortName.size ()
        &&
    fOptionsElementLongName.size ()
    ) {
      os <<
        " (" <<
        "-" << fOptionsElementShortName <<
        ", " <<
        "-" << fOptionsElementLongName <<
        ")";
  }
  
  else {
    if (fOptionsElementShortName.size ()) {
      os <<
      " (" <<
      "-" << fOptionsElementShortName <<
      ")";
    }
    if (fOptionsElementLongName.size ()) {
      os <<
      " (" <<
      "-" << fOptionsElementLongName <<
      ")";
    }
  }

  os <<
    ":" <<
    endl;

  if (fOptionsHandlerOptionsGroupsList.size ()) {
  // JMI  os << endl;
    
    idtr++;

    list<S_msrOptionsGroup>::const_iterator
      iBegin = fOptionsHandlerOptionsGroupsList.begin(),
      iEnd   = fOptionsHandlerOptionsGroupsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group summary
      os << idtr;
      (*i)->printHelpSummary (os);
      if (++i == iEnd) break;
      cerr << endl;
    } // for

    idtr--;
  }
  
  os <<
    endl;
}

void msrOptionsHandler::printOptionsValues (
  ostream& os) const
{
  // the description is the header of the information
  os << idtr <<
    fOptionsElementDescription <<
    ":" <<
    endl;

  // print the options groups values
  if (fOptionsHandlerOptionsGroupsList.size ()) {
    os << endl;
    
    idtr++;

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
      cerr << endl;
    } // for

    idtr--;
  }
  
  os <<
    endl;
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

const vector<string> msrOptionsHandler::analyzeOptions (
  int   argc,
  char* argv[])
{
  if (TRACE_OPTIONS) {
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
        cerr << idtr <<
          (*i).first << "-->" <<
          endl;
        idtr++;
        (*i).second->printHeader (cerr);
        if (++i == iEnd) break;
        cerr << endl;
        idtr--;
      } // for
      
      idtr--;
    }
    cerr <<
      endl;
  }

  // fetch program name
  fProgramName = string (argv [0]);
  
  fCommandLineWithShortOptions = fProgramName;
  fCommandLineWithLongOptions  = fProgramName;
  
  // decipher the command options and arguments
  int n = 1;

  fExpectedValuesNumber = 0;
  
  bool pureHelpRun = true;

  while (true) { 
    if (argv [n] == 0)
      break;

    string currentElement = string (argv [n]);
    
    if (TRACE_OPTIONS) {
      // print current element
      cerr <<
        "Command line elemnt " << n <<
        ": " <<currentElement << " "<<
        endl;
    }

    // handle current element
    if (currentElement [0] == '-') {
      // stdin or options element?
      
      if (currentElement.size () == 1) {
        // this is the stdin indicator
        if (TRACE_OPTIONS) {
          cerr <<
            "'" << currentElement << "' is the '-' stdin indicator" <<
            endl;
        }
        
        fArgumentsVector.push_back (currentElement);

        fCommandLineWithShortOptions +=
          " " + currentElement;
        fCommandLineWithLongOptions +=
          " " + currentElement;
        
        pureHelpRun = false;
      }
      
      else {
        // this is an option
        string currentOptionName;

        string elementTrailer =
          currentElement.substr (1, string::npos);
  
        /* JMI
        cerr <<
          "elementTrailer '" << elementTrailer << "' is preceded by a dash" <<
          endl;
        */
  
        if (elementTrailer.size ()) {
          if (elementTrailer [0] == '-') {
            // it is a double-dashed option
            currentOptionName =
              elementTrailer.substr (1, string::npos);
            
            if (TRACE_OPTIONS) {
              cerr <<
                "'" << currentOptionName << "' is a double-dashed option" <<
                endl;
            }
          }
          else {
            // it is a single-dashed option
            currentOptionName =
              elementTrailer; //.substr (1, string::npos);
            
            if (TRACE_OPTIONS) {
              cerr <<
                "'" << currentOptionName << "' is a single-dashed option" <<
                endl;
            }
          }
        }
        
        else {
          if (TRACE_OPTIONS) {
            cerr <<
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

      pureHelpRun = false;
    }
    
    // next please
    n++;
  } // while

  if (TRACE_OPTIONS) {
    // print the arguments vector
    int argumentsVectorSize =
      fArgumentsVector.size ();
      
    cerr << idtr <<
      "Arguments vector (" <<
      argumentsVectorSize <<
      " elements):" <<
      endl;
  
    if (argumentsVectorSize) {
      idtr++;
      for (unsigned int i = 0; i < argumentsVectorSize; i++) {
        cerr << idtr <<
          fArgumentsVector [i] <<
          endl;
      } // for
      idtr--;
    }
  }

  // exit if this is a pure help run
  if (pureHelpRun) {
    cerr << idtr <<
      "This is a pure help run, exiting." <<
      endl;

    exit (1);
  }

  // return arguements vector for handling by caller
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
      
    optionError (s.str());
    cerr <<
      fOptionsHandlerHelpHeader <<
      endl;
    printHelpSummary (cerr);
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
      
    optionError (s.str());
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
      " " + shortNameToBeUsed;
    fCommandLineWithLongOptions +=
      " " + longNameToBeUsed;

    // handle the option element
    if (
      S_msrOptionsHandler
        optionsHandler =
          dynamic_cast<msrOptionsHandler*>(&(*optionsElement))
      ) {
      // print the help header
      cerr <<
        getOptionsHandlerHelpHeader () <<
        endl;
        
      // print versions history
      printVersionsHistory (cerr);

      // print the help
      optionsHandler->printHelp (cerr);
      cerr <<
        endl;
    }
    
    else if (
      // options group?
      S_msrOptionsGroup
        optionsGroup =
          dynamic_cast<msrOptionsGroup*>(&(*optionsElement))
      ) {
      // print the help
      cerr << idtr <<
        "--- Help for group \"" <<
        optionsElement->
          getOptionsElementDescription () <<
        "\" ---" <<
        endl <<
        endl;
        
      optionsGroup->printHelp (cerr);
      
      cerr <<
        endl;
    }
    
    else if (
      // options subgroup?
      S_msrOptionsSubGroup
        optionsSubGroup =
          dynamic_cast<msrOptionsSubGroup*>(&(*optionsElement))
      ) {    
      // print the help
      cerr << idtr <<
        "--- Help for subgroup \"" <<
        optionsElement->
          getOptionsElementDescription () <<
        "\" ---" <<
        endl <<
        endl;
        
      optionsSubGroup->printHelp (cerr);

      cerr <<
        endl;
    }
    
    else {
      // this is an options item, handle it
      
      if (
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
        // integer item?
        S_msrOptionsIntegerItem
          optionsIntegerItem =
            dynamic_cast<msrOptionsIntegerItem*>(&(*optionsElement))
        ) {
        // wait until the value is met
        fPendingOptionsItem = optionsIntegerItem;
        fExpectedValuesNumber = 1;
      }
      
      else if (
        // float item?
        S_msrOptionsFloatItem
          optionsFloatItem =
            dynamic_cast<msrOptionsFloatItem*>(&(*optionsElement))
        ) {              
        // wait until the value is met
        fPendingOptionsItem = optionsFloatItem;
        fExpectedValuesNumber = 1;
      }
      
      else if (
        // string item?
        S_msrOptionsStringItem
          optionsStringItem =
            dynamic_cast<msrOptionsStringItem*>(&(*optionsElement))
        ) {
        // wait until the value is met
        fPendingOptionsItem = optionsStringItem;
        fExpectedValuesNumber = 1;
      }
      
      else if (
        // rational item?
        S_msrOptionsRationalItem
          optionsRationalItem =
            dynamic_cast<msrOptionsRationalItem*>(&(*optionsElement))
        ) {
        // wait until the value is met
        fPendingOptionsItem = optionsRationalItem;
        fExpectedValuesNumber = 1;
      }

      else if (
        // numbers set item?
        S_msrOptionsNumbersSetItem
          optionsNumbersSetItem =
            dynamic_cast<msrOptionsNumbersSetItem*>(&(*optionsElement))
        ) {
        // wait until the value is met
        fPendingOptionsItem = optionsNumbersSetItem;
        fExpectedValuesNumber = 1;
      }

      else if (
        // pitches language item?
        S_msrOptionsPitchesLanguageItem
          optionsPitchesLanguageItem =
            dynamic_cast<msrOptionsPitchesLanguageItem*>(&(*optionsElement))
        ) {
        // wait until the value is met
        fPendingOptionsItem = optionsPitchesLanguageItem;
        fExpectedValuesNumber = 1;
      }

      else if (
        // chords language item?
        S_msrOptionsChordsLanguageItem
          optionsChordsLanguageItem =
            dynamic_cast<msrOptionsChordsLanguageItem*>(&(*optionsElement))
        ) {
        // wait until the value is met
        fPendingOptionsItem = optionsChordsLanguageItem;
        fExpectedValuesNumber = 1;
      }

      else {
        stringstream s;
    
        s <<
          "option item type is unknown";
          
        optionError (s.str());
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
      fExpectedValuesNumber = 0;

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
      fExpectedValuesNumber = 0;
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
      fExpectedValuesNumber = 0;
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
          cout <<
            "There are " << sm.size() << " matches" <<
            " for string '" << theString <<
            "' with regex '" << regularExpression <<
            "'" <<
            endl;
       
          for (unsigned i = 0; i < sm.size (); ++i) {
            cout << "[" << sm [i] << "] ";
          } // for
          
          cout <<
            endl;
        }
      }
     
      else {
        stringstream s;

        s <<
          "--delayedOrnamentFraction argument '" << theString <<
          "' is ill-formed";
          
        optionError (s.str());
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
        cerr << // JMI
          "rationalValue = " <<
          rationalValue <<
          endl;
      }

      optionsRationalItem->
        setRationalItemVariableValue (
          rationalValue);

      fPendingOptionsItem = 0;
      fExpectedValuesNumber = 0;
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
      fExpectedValuesNumber = 0;
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
    
        idtr++;
      
        s <<
          existingQuarterTonesPitchesLanguages ();
    
        idtr--;
    
        optionError (s.str());
      }
    
      optionsPitchesLanguageItem->
        setPitchesLanguageItemVariableValue (
          (*it).second);

      fPendingOptionsItem = 0;
      fExpectedValuesNumber = 0;
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
          gLpsrChordsLanguagesMap.size () /* JMI ??? */ -1 <<
          " known LPSR chords languages apart from the default Ignatzek are:" <<
          endl;
    
        idtr++;
      
        s <<
          existingLpsrChordsLanguages ();
    
        idtr--;
    
        optionError (s.str());
      }
    
      optionsChordsLanguageItem->
        setChordsLanguageItemVariableValue (
          (*it).second);

      fPendingOptionsItem = 0;
      fExpectedValuesNumber = 0;
    }
    
  }

  else {
    // theString is an argument
    fArgumentsVector.push_back (theString);
  }
}

}