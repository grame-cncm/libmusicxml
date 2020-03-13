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

#include "msrTies.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrTie msrTie::create (
  int        inputLineNumber,
  msrTieKind tieKind)
{
  msrTie* o =
    new msrTie (
      inputLineNumber, tieKind);
  assert(o!=0);
  return o;
}

msrTie::msrTie (
  int        inputLineNumber,
  msrTieKind tieKind)
    : msrElement (inputLineNumber)
{
  fTieKind = tieKind;
}

msrTie::~msrTie ()
{}

void msrTie::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrTie::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrTie>*
    p =
      dynamic_cast<visitor<S_msrTie>*> (v)) {
        S_msrTie elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrTie::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTie::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrTie::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrTie>*
    p =
      dynamic_cast<visitor<S_msrTie>*> (v)) {
        S_msrTie elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrTie::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTie::browseData (basevisitor* v)
{}

string msrTie::tieKindAsString (msrTieKind tieKind)
{
  stringstream s;

  switch (tieKind) {
    case msrTie::kTieNone:
      s << "tieNone";
      break;
    case msrTie::kTieStart:
      s << "tieStart";
      break;
    case msrTie::kTieContinue:
      s << "tieContinue";
      break;
    case msrTie::kTieStop:
      s << "tieStop";
      break;
  } // switch

  return s.str ();
}

string msrTie::tieKindAsString () const
{
  return tieKindAsString (fTieKind);
}

string msrTie::asString () const
{
  stringstream s;

  s <<
    "Tie" << " " << tieKindAsString () <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrTie::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrTie& elt)
{
  elt->print (os);
  return os;
}


}
