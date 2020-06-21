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

#include "utilities.h"

#include "msrClefs.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrClef msrClef::create (
  int         inputLineNumber,
  msrClefKind clefKind)
{
  msrClef* o =
    new msrClef (
      inputLineNumber, clefKind);
  assert(o!=0);
  return o;
}

msrClef::msrClef (
  int         inputLineNumber,
  msrClefKind clefKind)
    : msrMeasureElement (inputLineNumber)
{
  fClefKind = clefKind;
}

msrClef::~msrClef ()
{}

bool msrClef::clefIsATablatureClef () const
{
  switch (fClefKind) {
    case kTablature4Clef:
    case kTablature5Clef:
    case kTablature6Clef:
    case kTablature7Clef:
      return true;
      break;
    default:
      return false;
  } // switch
}

bool msrClef::clefIsAPercussionClef () const
{
  switch (fClefKind) {
    case kPercussionClef:
      return true;
      break;
    default:
      return false;
  } // switch
}

bool msrClef::isEqualTo (S_msrClef otherClef) const
{
  if (! otherClef) {
    return false;
  }

  return fClefKind == otherClef->fClefKind;
}

void msrClef::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrClef::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrClef::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrClef::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrClef::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrClef::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrClef::browseData (basevisitor* v)
{}

string msrClef::asString () const
{
  stringstream s;

  s <<
    "[Clef " <<
    clefKindAsString (fClefKind) <<
    "], line " << fInputLineNumber;

  return s.str ();
}

void msrClef::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrClef& elt)
{
  elt->print (os);
  return os;
}


}
