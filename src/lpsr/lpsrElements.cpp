/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "utilities.h"

#include "lpsrElements.h"

#include "lpsrOah.h"


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
#ifdef TRACE_OAH
  if (gGlobalLpsrOah->fTraceLpsrVisitors) {
    gLogOstream <<
      "% ==> lpsrElement::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrElement>*
    p =
      dynamic_cast<visitor<S_lpsrElement>*> (v)) {
        S_lpsrElement elem = this;

#ifdef TRACE_OAH
        if (gGlobalLpsrOah->fTraceLpsrVisitors) {
          gLogOstream <<
            "% ==> Launching lpsrElement::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrElement::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalLpsrOah->fTraceLpsrVisitors) {
    gLogOstream <<
      "% ==> lpsrElement::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrElement>*
    p =
      dynamic_cast<visitor<S_lpsrElement>*> (v)) {
        S_lpsrElement elem = this;

#ifdef TRACE_OAH
        if (gGlobalLpsrOah->fTraceLpsrVisitors) {
          gLogOstream <<
            "% ==> Launching lpsrElement::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrElement::browseData (basevisitor* v)
{}


}
