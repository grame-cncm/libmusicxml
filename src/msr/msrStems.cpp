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

#include "msrStems.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrStem msrStem::create (
  int         inputLineNumber,
  msrStemKind stemKind)
{
  msrStem* o =
    new msrStem (
      inputLineNumber, stemKind);
  assert (o!=0);
  return o;
}

msrStem::msrStem (
  int         inputLineNumber,
  msrStemKind stemKind)
    : msrElement (inputLineNumber)
{
  fStemKind   = stemKind;
}

msrStem::~msrStem ()
{}

void msrStem::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStem::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrStem>*
    p =
      dynamic_cast<visitor<S_msrStem>*> (v)) {
        S_msrStem elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStem::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrStem::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStem::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrStem>*
    p =
      dynamic_cast<visitor<S_msrStem>*> (v)) {
        S_msrStem elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStem::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrStem::browseData (basevisitor* v)
{}

string msrStem::stemKindAsString (
  msrStemKind stemKind)
{
  string result;

  switch (stemKind) {
    case msrStem::kStemNone:
      result = "stemNone";
      break;
    case msrStem::kStemUp:
      result = "stemUp";
      break;
    case msrStem::kStemDown:
      result = "stemDown";
      break;
    case msrStem::kStemDouble:
      result = "stemDouble";
      break;
  } // switch

  return result;
}

string msrStem::asString () const
{
  stringstream s;

  s <<
    "Stem" <<
    ", stemKind: " <<
    stemKindAsString (fStemKind) <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrStem::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrStem& elt)
{
  elt->print (os);
  return os;
}


}
