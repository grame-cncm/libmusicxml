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
#include <algorithm>    // for_each

#include "utilities.h"

#include "lpsrScheme.h"

#include "lpsrVarValAssocs.h"

#include "oahOah.h"


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
  assert (o!=0);
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

void lpsrSchemeVariable::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrSchemeVariable::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrSchemeVariable>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeVariable>*> (v)) {
        S_lpsrSchemeVariable elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrSchemeVariable::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrSchemeVariable::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrSchemeVariable::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrSchemeVariable>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeVariable>*> (v)) {
        S_lpsrSchemeVariable elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrSchemeVariable::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrSchemeVariable::browseData (basevisitor* v)
{}

string lpsrSchemeVariable::commentedKindAsString (
  lpsrCommentedKind commentedKind)
{
  string result;

  switch (commentedKind) {
    case lpsrSchemeVariable::kCommentedYes:
      result = "commentedYes";
      break;
    case lpsrSchemeVariable::kCommentedNo:
      result = "commentedNo";
      break;
  } // switch

  return result;
}

string lpsrSchemeVariable::endlKindAsString (
  lpsrEndlKind endlKind)
{
  string result;

  switch (endlKind) {
    case lpsrSchemeVariable::kEndlOnce:
      result = "endlOnce";
      break;
    case lpsrSchemeVariable::kEndlTwice:
      result = "endlTwice";
      break;
    case lpsrSchemeVariable::kEndlNone:
      result = "endlNone";
      break;
  } // switch

  return result;
}

void lpsrSchemeVariable::print (ostream& os) const
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
    "variableName" << " : " <<
    "\"" << variableName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "variableValue" << " : " <<
    "\"" << variableValue << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "commentedKind" << " : " <<
    commentedKindAsString (fCommentedKind) <<
    endl <<

  // backSlashKindAsString ??? JMI
  // varValSeparatorKindAsString ??? JMI
  // quotesKindAsString ??? JMI

    setw (fieldWidth) <<
    "endlKind" << " : " <<
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
  assert (o!=0);
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrSchemeFunction::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrSchemeFunction>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeFunction>*> (v)) {
        S_lpsrSchemeFunction elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrSchemeFunction::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrSchemeFunction::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrSchemeFunction::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrSchemeFunction>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeFunction>*> (v)) {
        S_lpsrSchemeFunction elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrSchemeFunction::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrSchemeFunction::browseData (basevisitor* v)
{}

void lpsrSchemeFunction::print (ostream& os) const
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
