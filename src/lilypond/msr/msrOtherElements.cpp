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

#include "msrOtherElements.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrSegno msrSegno::create (
  int inputLineNumber,
  int staffNumber)
{
  msrSegno* o =
    new msrSegno (
      inputLineNumber,
      staffNumber);
  assert(o!=0);
  return o;
}

msrSegno::msrSegno (
  int inputLineNumber,
  int staffNumber)
    : msrMeasureElement (inputLineNumber)
{
  fStaffNumber = staffNumber;
}

msrSegno::~msrSegno ()
{}

void msrSegno::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrSegno::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrSegno>*
    p =
      dynamic_cast<visitor<S_msrSegno>*> (v)) {
        S_msrSegno elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrSegno::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSegno::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrSegno::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrSegno>*
    p =
      dynamic_cast<visitor<S_msrSegno>*> (v)) {
        S_msrSegno elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrSegno::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrSegno::browseData (basevisitor* v)
{}

string msrSegno::asString () const
{
  stringstream s;

  s <<
    "Segno" <<
    ", staffNumber :" << fStaffNumber <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrSegno::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrSegno& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrCoda msrCoda::create (
  int inputLineNumber,
  int staffNumber)
{
  msrCoda* o =
    new msrCoda (
      inputLineNumber,
      staffNumber);
  assert(o!=0);
  return o;
}

msrCoda::msrCoda (
  int inputLineNumber,
  int staffNumber)
    : msrMeasureElement (inputLineNumber)
{
  fStaffNumber = staffNumber;
}

msrCoda::~msrCoda ()
{}

void msrCoda::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrCoda::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrCoda>*
    p =
      dynamic_cast<visitor<S_msrCoda>*> (v)) {
        S_msrCoda elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrCoda::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrCoda::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrCoda::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrCoda>*
    p =
      dynamic_cast<visitor<S_msrCoda>*> (v)) {
        S_msrCoda elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrCoda::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrCoda::browseData (basevisitor* v)
{}

string msrCoda::asString () const
{
  stringstream s;

  s <<
    "Coda" <<
    ", staffNumber :" << fStaffNumber <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrCoda::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrCoda& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrEyeGlasses msrEyeGlasses::create (
  int inputLineNumber)
{
  msrEyeGlasses* o =
    new msrEyeGlasses (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrEyeGlasses::msrEyeGlasses (
  int inputLineNumber)
    : msrMeasureElement (inputLineNumber)
{}

msrEyeGlasses::~msrEyeGlasses ()
{}

void msrEyeGlasses::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrEyeGlasses::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrEyeGlasses>*
    p =
      dynamic_cast<visitor<S_msrEyeGlasses>*> (v)) {
        S_msrEyeGlasses elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrEyeGlasses::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrEyeGlasses::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrEyeGlasses::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrEyeGlasses>*
    p =
      dynamic_cast<visitor<S_msrEyeGlasses>*> (v)) {
        S_msrEyeGlasses elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrEyeGlasses::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrEyeGlasses::browseData (basevisitor* v)
{}

string msrEyeGlasses::asString () const
{
  stringstream s;

  s <<
    "EyeGlasses" <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrEyeGlasses::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrEyeGlasses& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
/*
void msrPolyphony::print (ostream& os) const
{
  const int fieldWidth = 9;

  os << left <<
    setw (fieldWidth) <<
    "fBeatUnit" << " = " << fBeatUnit <<
    endl <<
    setw (fieldWidth) <<
    "fDots" << " = " << fDots <<
    endl;
};
*/


}
