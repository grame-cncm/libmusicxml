/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "lpsrVariables.h"

#include "lpsrOptions.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
S_lpsrVariableUseCommand lpsrVariableUseCommand::create (
  int    inputLineNumber,
  string variableName)
{
  lpsrVariableUseCommand* o =
    new lpsrVariableUseCommand (
      inputLineNumber, variableName);
  assert(o!=0);
  return o;
}

lpsrVariableUseCommand::lpsrVariableUseCommand (
  int    inputLineNumber,
  string variableName)
    : lpsrElement (inputLineNumber)
{
  fVariableName = variableName;
}

lpsrVariableUseCommand::~lpsrVariableUseCommand()
{}

void lpsrVariableUseCommand::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrVariableUseCommand::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_lpsrVariableUseCommand>*
    p =
      dynamic_cast<visitor<S_lpsrVariableUseCommand>*> (v)) {
        S_lpsrVariableUseCommand elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrVariableUseCommand::visitStart()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrVariableUseCommand::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrVariableUseCommand::acceptOut()" <<
      endl;
  }

  if (visitor<S_lpsrVariableUseCommand>*
    p =
      dynamic_cast<visitor<S_lpsrVariableUseCommand>*> (v)) {
        S_lpsrVariableUseCommand elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrVariableUseCommand::visitEnd()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrVariableUseCommand::browseData (basevisitor* v)
{}

void lpsrVariableUseCommand::print (ostream& os)
{
  os << "VariableUseCommand" << endl;
  gIndenter++;
  os << fVariableName << endl;
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsrVariableUseCommand& nstf)
{
  nstf->print (os);
  return os;
}


}
