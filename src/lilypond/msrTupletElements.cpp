/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <climits>      // INT_MIN, INT_MAX

#include "msrTupletElements.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
msrTupletElement::msrTupletElement (
  int inputLineNumber)
    : msrMeasureElement (inputLineNumber)
{
// fTupletNumber = "???";
//  fPositionInTuplet = rational (INT_MIN, 1);
}

msrTupletElement::~msrTupletElement ()
{}

/* JMI
void msrTupletElement::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrTupletElement::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrTupletElement>*
    p =
      dynamic_cast<visitor<S_msrTupletElement>*> (v)) {
        S_msrTupletElement elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrTupletElement::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTupletElement::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrTupletElement::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrTupletElement>*
    p =
      dynamic_cast<visitor<S_msrTupletElement>*> (v)) {
        S_msrTupletElement elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrTupletElement::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

string msrTupletElement::asString () const
{
  // this is overriden all in actual elements
  return "??? msrTupletElement::asString () ???";
}

string msrTupletElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString () const;
}

void msrTupletElement::print (ostream& os)
{
  os << asString () << endl;
}
*/

ostream& operator<< (ostream& os, const S_msrTupletElement& elt)
{
  elt->print (os);
  return os;
}


}
