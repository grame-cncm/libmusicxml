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

#include "msrPartGroupElements.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
msrPartGroupElement::msrPartGroupElement (
  int inputLineNumber)
    : msrMeasureElement (inputLineNumber)
{
//  fPartGroupNumber = "???";
//  fPositionInPartGroup = rational (INT_MIN, 1);
}

msrPartGroupElement::~msrPartGroupElement ()
{}

/* JMI
void msrPartGroupElement::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrPartGroupElement::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrPartGroupElement>*
    p =
      dynamic_cast<visitor<S_msrPartGroupElement>*> (v)) {
        S_msrPartGroupElement elem = this;
        
        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrPartGroupElement::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPartGroupElement::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrPartGroupElement::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrPartGroupElement>*
    p =
      dynamic_cast<visitor<S_msrPartGroupElement>*> (v)) {
        S_msrPartGroupElement elem = this;
      
        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrPartGroupElement::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

string msrPartGroupElement::asString () const
{
  // this is overriden all in actual elements
  return "??? msrPartGroupElement::asString () ???";
}

string msrPartGroupElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString () const;
}

void msrPartGroupElement::print (ostream& os)
{
  os << asString () << endl;
}
*/

ostream& operator<< (ostream& os, const S_msrPartGroupElement& elt)
{
  elt->print (os);
  return os;
}


}
