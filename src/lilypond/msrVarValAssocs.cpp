/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <iostream>
#include <sstream>
#include <iomanip>      // setw, setprecision, ...

#include "msrVarValAssocs.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "msrOptions.h"


using namespace std;

namespace MusicXML2 {

//______________________________________________________________________________
S_msrVarValAssoc msrVarValAssoc::create (
  int                inputLineNumber,
  msrVarValAssocKind varValAssocKind,
  string             value)
{
  msrVarValAssoc* o =
    new msrVarValAssoc(
      inputLineNumber,
      varValAssocKind, value);
  assert(o!=0);
  return o;
}

msrVarValAssoc::msrVarValAssoc (
  int                inputLineNumber,
  msrVarValAssocKind varValAssocKind,
  string             value)
    : msrElement (inputLineNumber)
{
  fVarValAssocKind = varValAssocKind;
  fVariableValue   = value;
}

msrVarValAssoc::~msrVarValAssoc ()
{}

void msrVarValAssoc::setVariableValue (string value)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVarValAssocs) {
    gLogIOstream <<
      "Setting the value of marVarValAssoc \"" <<
      varValAssocKindAsString (fVarValAssocKind) <<
      "\" to '" <<
      value <<
      "', line " << fInputLineNumber <<
      endl;
  }
#endif

  fVariableValue = value;
}

void msrVarValAssoc::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrVarValAssoc::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrVarValAssoc>*
    p =
      dynamic_cast<visitor<S_msrVarValAssoc>*> (v)) {
        S_msrVarValAssoc elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrVarValAssoc::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrVarValAssoc::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrVarValAssoc::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrVarValAssoc>*
    p =
      dynamic_cast<visitor<S_msrVarValAssoc>*> (v)) {
        S_msrVarValAssoc elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrVarValAssoc::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrVarValAssoc::browseData (basevisitor* v)
{}

string msrVarValAssoc::varValAssocKindAsString (
  msrVarValAssocKind varValAssocKind)
{
  string result;

  switch (varValAssocKind) {
    case msrVarValAssoc::kWorkNumber:
      result = "workNumber";
      break;
    case msrVarValAssoc::kWorkTitle:
      result = "workTitle";
      break;
    case msrVarValAssoc::kMovementNumber:
      result = "movementNumber";
      break;
    case msrVarValAssoc::kMovementTitle:
      result = "movementTitle";
      break;
    case msrVarValAssoc::kEncodingDate:
      result = "encodingDate";
      break;
    case msrVarValAssoc::kScoreInstrument:
      result = "scoreInstrument";
      break;
    case msrVarValAssoc::kMiscellaneousField:
      result = "miscellaneousField";
      break;
  } // switch
  
  return result;
}

void msrVarValAssoc::print (ostream& os)
{
  os <<
    "MSR VarValAssoc" <<
    endl;
  
  gIndenter++;

  string variableValue;

  for_each (
    fVariableValue.begin (),
    fVariableValue.end (),
    stringQuoteEscaper (variableValue));

  // print resulting strings
  const int fieldWidth = 16;
  
  os << left <<
    setw (fieldWidth) <<
    "variable kind" << " : " <<
    varValAssocKindAsString () <<
    endl <<
    setw (fieldWidth) <<
    "variable value" << " : " <<
    "\"" << variableValue << "\"" <<
    endl;
  
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrVarValAssoc& elt) {
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrVarValsListAssoc msrVarValsListAssoc::create (
  int                inputLineNumber,
  msrVarValsListAssocKind varValsListAssocKind)
{
  msrVarValsListAssoc* o =
    new msrVarValsListAssoc(
      inputLineNumber,
      varValsListAssocKind);
  assert(o!=0);
  return o;
}

msrVarValsListAssoc::msrVarValsListAssoc (
  int                inputLineNumber,
  msrVarValsListAssocKind varValsListAssocKind)
    : msrElement (inputLineNumber)
{
  fVarValsListAssocKind = varValsListAssocKind;
}

msrVarValsListAssoc::~msrVarValsListAssoc ()
{}

void msrVarValsListAssoc::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrVarValsListAssoc::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrVarValsListAssoc>*
    p =
      dynamic_cast<visitor<S_msrVarValsListAssoc>*> (v)) {
        S_msrVarValsListAssoc elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrVarValsListAssoc::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrVarValsListAssoc::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrVarValsListAssoc::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrVarValsListAssoc>*
    p =
      dynamic_cast<visitor<S_msrVarValsListAssoc>*> (v)) {
        S_msrVarValsListAssoc elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrVarValsListAssoc::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrVarValsListAssoc::browseData (basevisitor* v)
{}

string msrVarValsListAssoc::varValsListAssocKindAsString (
  msrVarValsListAssocKind varValsListAssocKind)
{
  string result;

  switch (varValsListAssocKind) {
    case msrVarValsListAssoc::kRights:
      result = "rights";
      break;
    case msrVarValsListAssoc::kComposer:
      result = "composer";
      break;
    case msrVarValsListAssoc::kArranger:
      result = "arranger";
      break;
    case msrVarValsListAssoc::kLyricist:
      result = "lyricist";
      break;
    case msrVarValsListAssoc::kPoet:
      result = "poet";
      break;
    case msrVarValsListAssoc::kTranslator:
      result = "translator";
      break;
    case msrVarValsListAssoc::kSoftware:
      result = "software";
      break;
  } // switch
  
  return result;
}

string msrVarValsListAssoc::varValsListAssocValuesAsString () const
{
  stringstream s;
  
  list<string>::const_iterator
    iBegin = fVariableValuesList.begin (),
    iEnd   = fVariableValuesList.end (),
    i      = iBegin;

  s << "[";
  if (fVariableValuesList.size ()) {
    for ( ; ; ) {
      s << "\"" << (*i) << "\""; 
      if (++i == iEnd) break;
      s << ", ";
    } // for
  }
  s << "]";
  
  return s.str ();
}

void msrVarValsListAssoc::print (ostream& os)
{
  os <<
    "MSR VarValsListAssoc" <<
    endl;
  
  gIndenter++;

  const int fieldWidth = 16;

  os << left <<
    setw (fieldWidth) <<
    "assoc kind" << " : " <<
    varValsListAssocKindAsString () <<
    endl <<
    setw (fieldWidth) <<
    
    "variable values" << " : " <<
    endl;
    
  gIndenter++;

  if (fVariableValuesList.size ()) {
    list<string>::const_iterator
      iBegin = fVariableValuesList.begin (),
      iEnd   = fVariableValuesList.end (),
      i      = iBegin;
  
    for ( ; ; ) {
      os << "\"" << (*i) << "\""; 
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  
  os <<
    endl;
  
  gIndenter--;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrVarValsListAssoc& elt) {
  elt->print (os);
  return os;
}


}
