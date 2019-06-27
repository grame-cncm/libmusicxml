/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "lpsrElements.h"

#include "lpsrOptions.h"


using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
S_lpsrElement lpsrElement::create (
  int inputLineNumber)
{
  lpsrElement* o =
    new lpsrElement (
      inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrElement::lpsrElement (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{}

lpsrElement::~lpsrElement ()
{}

void lpsrElement::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrElement::acceptIn ()" <<
      endl;
  }
  
  if (visitor<S_lpsrElement>*
    p =
      dynamic_cast<visitor<S_lpsrElement>*> (v)) {
        S_lpsrElement elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrElement::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrElement::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrElement::acceptOut ()" <<
      endl;
  }

  if (visitor<S_lpsrElement>*
    p =
      dynamic_cast<visitor<S_lpsrElement>*> (v)) {
        S_lpsrElement elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrElement::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrElement::browseData (basevisitor* v)
{}


}
