/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <iostream>
#include <sstream>
#include <climits>      // INT_MIN, INT_MAX

#include "msrElements.h"

#include "msrOptions.h"


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
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrElement::msrElement ()" <<
      endl;
  }
      
  if (visitor<S_msrElement>*
    p =
      dynamic_cast<visitor<S_msrElement>*> (v)) {
        S_msrElement elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrElement::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrElement::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrElement::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrElement>*
    p =
      dynamic_cast<visitor<S_msrElement>*> (v)) {
        S_msrElement elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
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

void msrElement::print (ostream& os)
{
  os << asString () << endl;
}

void msrElement::shortPrint (ostream& os)
{
  print (os);
}

ostream& operator<< (ostream& os, const S_msrElement& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
msrMeasureElement::msrMeasureElement (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
  fMeasureNumber = "???";
  fPositionInMeasure = rational (INT_MIN, 1);
}

msrMeasureElement::~msrMeasureElement ()
{}

void msrMeasureElement::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasureElement::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrMeasureElement>*
    p =
      dynamic_cast<visitor<S_msrMeasureElement>*> (v)) {
        S_msrMeasureElement elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasureElement::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasureElement::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasureElement::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMeasureElement>*
    p =
      dynamic_cast<visitor<S_msrMeasureElement>*> (v)) {
        S_msrMeasureElement elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasureElement::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

string msrMeasureElement::asString () const
{
  // this is overriden all in actual elements
  return "??? msrMeasureElement::asString () ???";
}

string msrMeasureElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString ();
}

void msrMeasureElement::print (ostream& os)
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_msrMeasureElement& elt)
{
  elt->print (os);
  return os;
}


}
