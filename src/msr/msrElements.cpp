/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iostream>
#include <sstream>
#include <climits>      // INT_MIN, INT_MAX

#include "utilities.h"

#include "msrElements.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
msrElement::msrElement (
  int inputLineNumber)
{
  fInputLineNumber = inputLineNumber;
}

msrElement::~msrElement ()
{}

void msrElement::acceptIn (basevisitor* v)
{
  if (globalMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrElement::msrElement ()" <<
      endl;
  }

  if (visitor<S_msrElement>*
    p =
      dynamic_cast<visitor<S_msrElement>*> (v)) {
        S_msrElement elem = this;

        if (globalMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrElement::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrElement::acceptOut (basevisitor* v)
{
  if (globalMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrElement::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrElement>*
    p =
      dynamic_cast<visitor<S_msrElement>*> (v)) {
        S_msrElement elem = this;

        if (globalMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrElement::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

string msrElement::asString () const
{
  // this is overriden all in actual elements
  return "??? msrElement::asString () ???";
}

string msrElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString ();
}

void msrElement::print (ostream& os) const
{
  os << asString () << endl;
}

void msrElement::printShort (ostream& os) const
{
  print (os);
}

ostream& operator<< (ostream& os, const S_msrElement& elt)
{
  elt->print (os);
  return os;
}


}
