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

#include "lpsrScheme.h"

#include "lpsrVarValAssocs.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
string const lpsrSchemeVariable::g_SchemeVariableNoUnit    = "";
string const lpsrSchemeVariable::g_SchemeVariableNoComment = "";

S_lpsrSchemeVariable lpsrSchemeVariable::create (
  int               inputLineNumber,
  lpsrCommentedKind commentedKind,
  string            variableName,
  string            value, 
  string            comment,
  lpsrEndlKind      endlKind)
{
  lpsrSchemeVariable* o =
    new lpsrSchemeVariable (
      inputLineNumber,
      commentedKind, variableName, value, comment, endlKind);
  assert(o!=0);
  return o;
}

lpsrSchemeVariable::lpsrSchemeVariable (
  int               inputLineNumber,
  lpsrCommentedKind commentedKind,
  string            variableName,
  string            value, 
  string            comment,
  lpsrEndlKind      endlKind)
    : lpsrElement (inputLineNumber)
{
  fCommentedKind = commentedKind;

  fVariableName  = variableName;
  fVariableValue = value;

  fComment       = comment;
  
  fEndlKind      = endlKind;
}

lpsrSchemeVariable::~lpsrSchemeVariable ()
{}

string lpsrSchemeVariable::commentedKindAsString (
  lpsrCommentedKind commentedKind)
{
  string result;

  switch (commentedKind) {
    case lpsrVarValAssoc::kCommented:
      result = "commented";
      break;
    case lpsrVarValAssoc::kUncommented:
      result = "uncommented";
      break;
  } // switch

  return result;
}

string lpsrSchemeVariable::endlKindAsString (
  lpsrEndlKind endlKind)
{
  string result;

  switch (endlKind) {
    case lpsrVarValAssoc::kWithEndl:
      result = "with end line";
      break;
    case lpsrVarValAssoc::kWithEndlTwice:
      result = "with end line twice";
      break;
    case lpsrVarValAssoc::kWithoutEndl:
      result = "without end line";
      break;
  } // switch

  return result;
}

void lpsrSchemeVariable::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrSchemeVariable::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_lpsrSchemeVariable>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeVariable>*> (v)) {
        S_lpsrSchemeVariable elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrSchemeVariable::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrSchemeVariable::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrSchemeVariable::acceptOut ()" <<
      endl;
  }

  if (visitor<S_lpsrSchemeVariable>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeVariable>*> (v)) {
        S_lpsrSchemeVariable elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrSchemeVariable::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrSchemeVariable::browseData (basevisitor* v)
{}

void lpsrSchemeVariable::print (ostream& os)
{
  os <<
    "SchemeVariable" <<
    endl;

  gIndenter++;

  // escape quotes if any
  string variableName;
  string variableValue;

  for_each (
    fVariableName.begin (),
    fVariableName.end (),
    stringQuoteEscaper (variableName));
  for_each (
    fVariableValue.begin (),
    fVariableValue.end (),
    stringQuoteEscaper (variableValue));

  // print resulting strings
  const int fieldWidth = 15;

  os << left <<
    setw (fieldWidth) <<
    "variable name" <<
    " : \"" << variableName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "variable value" <<
    " : \"" << variableValue << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "commented kind" << " : " <<
    commentedKindAsString (fCommentedKind) <<
    endl <<

  // backSlashKindAsString ??? JMI
  // varValSeparatorKindAsString ??? JMI
  // quotesKindAsString ??? JMI
  
    setw (fieldWidth) <<
    "endl kind" << " : " <<
    endlKindAsString (fEndlKind) <<
    endl;
  
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsrSchemeVariable& assoc)
{
  assoc->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrSchemeFunction lpsrSchemeFunction::create (
  int    inputLineNumber,
  string functionName,
  string functionDescription,
  string functionCode)
{
  lpsrSchemeFunction* o =
    new lpsrSchemeFunction (
      inputLineNumber,
      functionName, functionDescription, functionCode);
  assert(o!=0);
  return o;
}

lpsrSchemeFunction::lpsrSchemeFunction (
  int    inputLineNumber,
  string functionName,
  string functionDescription,
  string functionCode)
    : lpsrElement (inputLineNumber)
{
  fFunctionName        = functionName;
  fFunctionDescription = functionDescription;
  fFunctionCode        = functionCode;
  }

lpsrSchemeFunction::~lpsrSchemeFunction ()
{}

void lpsrSchemeFunction::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrSchemeFunction::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_lpsrSchemeFunction>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeFunction>*> (v)) {
        S_lpsrSchemeFunction elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrSchemeFunction::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrSchemeFunction::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrSchemeFunction::acceptOut ()" <<
      endl;
  }

  if (visitor<S_lpsrSchemeFunction>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeFunction>*> (v)) {
        S_lpsrSchemeFunction elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrSchemeFunction::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrSchemeFunction::browseData (basevisitor* v)
{}

void lpsrSchemeFunction::print (ostream& os)
{
  os <<
    "SchemeFunction" <<
    endl;

  gIndenter++;

  // print resulting strings
  os <<
    "function name : \"" << fFunctionName << "\"" <<
    endl <<
    "function description: \"" << fFunctionDescription << "\"" <<
    endl <<
    "function code: \"" << fFunctionCode << "\"" <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsrSchemeFunction& assoc)
{
  assoc->print (os);
  return os;
}


}
