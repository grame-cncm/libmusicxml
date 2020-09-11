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

#include "bsrElements.h"

#include "bsrOah.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
S_bsrElement bsrElement::create (
  int inputLineNumber)
{
  bsrElement* o =
    new bsrElement (
      inputLineNumber);
  assert(o!=0);
  return o;
}

bsrElement::bsrElement (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{}

bsrElement::~bsrElement ()
{}

void bsrElement::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrElement::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrElement>*
    p =
      dynamic_cast<visitor<S_bsrElement>*> (v)) {
        S_bsrElement elem = this;

#ifdef TRACE_OAH
        if (globalBsrOah->fTraceBsrVisitors) {
          gLogOstream <<
            "% ==> Launching bsrElement::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrElement::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrElement::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrElement>*
    p =
      dynamic_cast<visitor<S_bsrElement>*> (v)) {
        S_bsrElement elem = this;

#ifdef TRACE_OAH
        if (globalBsrOah->fTraceBsrVisitors) {
          gLogOstream <<
            "% ==> Launching bsrElement::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrElement::browseData (basevisitor* v)
{}


string bsrElement::asString () const
{
  // this is overriden all in actual elements
  return "??? bsrElement::asString () ???";
}

string bsrElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString ();
}

void bsrElement::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_bsrElement& elt)
{
  elt->print (os);
  return os;
}


}
