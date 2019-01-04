/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "lpsrBarNumbers.h"

#include "lpsrOptions.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
S_lpsrBarNumberCheck lpsrBarNumberCheck::create (
  int inputLineNumber,
  int nextBarNumber)
{
  lpsrBarNumberCheck* o =
    new lpsrBarNumberCheck (
      inputLineNumber, nextBarNumber);
  assert(o!=0);
  return o;
}

lpsrBarNumberCheck::lpsrBarNumberCheck (
  int inputLineNumber,
  int nextBarNumber)
    : lpsrElement (inputLineNumber)
{
  fNextBarNumber=nextBarNumber; 
}

lpsrBarNumberCheck::~lpsrBarNumberCheck ()
{}

void lpsrBarNumberCheck::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrBarNumberCheck::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_lpsrBarNumberCheck>*
    p =
      dynamic_cast<visitor<S_lpsrBarNumberCheck>*> (v)) {
        S_lpsrBarNumberCheck elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrBarNumberCheck::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrBarNumberCheck::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrBarNumberCheck::acceptOut ()" <<
      endl;
  }

  if (visitor<S_lpsrBarNumberCheck>*
    p =
      dynamic_cast<visitor<S_lpsrBarNumberCheck>*> (v)) {
        S_lpsrBarNumberCheck elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrBarNumberCheck::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrBarNumberCheck::browseData (basevisitor* v)
{}

void lpsrBarNumberCheck::print (ostream& os)
{
  os <<
    "BarNumberCheck" << " " << fNextBarNumber <<
    endl;
}

ostream& operator<< (ostream& os, const S_lpsrBarNumberCheck& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrBarCommand lpsrBarCommand::create (
  int inputLineNumber)
{
  lpsrBarCommand* o =
    new lpsrBarCommand (
      inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrBarCommand::lpsrBarCommand (
  int inputLineNumber)
    : lpsrElement (inputLineNumber)
{}

lpsrBarCommand::~lpsrBarCommand ()
{}

void lpsrBarCommand::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrBarCommand::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_lpsrBarCommand>*
    p =
      dynamic_cast<visitor<S_lpsrBarCommand>*> (v)) {
        S_lpsrBarCommand elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrBarCommand::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrBarCommand::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrBarCommand::acceptOut ()" <<
      endl;
  }

  if (visitor<S_lpsrBarCommand>*
    p =
      dynamic_cast<visitor<S_lpsrBarCommand>*> (v)) {
        S_lpsrBarCommand elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrBarCommand::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrBarCommand::browseData (basevisitor* v)
{}

void lpsrBarCommand::print (ostream& os)
{
  os <<
    "BarCommand" << " " << "\"|.\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_lpsrBarCommand& nstf)
{
  nstf->print (os);
  return os;
}


}
