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

#include "msrSingleTremolos.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrSingleTremolo msrSingleTremolo::create (
  int              inputLineNumber,
  int              singleTremoloMarksNumber,
  msrPlacementKind singleTremoloPlacementKind)
{
  msrSingleTremolo* o =
    new msrSingleTremolo (
      inputLineNumber,
      singleTremoloMarksNumber, singleTremoloPlacementKind);
  assert (o!=0);
  return o;
}

msrSingleTremolo::msrSingleTremolo (
  int              inputLineNumber,
  int              singleTremoloMarksNumber,
  msrPlacementKind singleTremoloPlacementKind)
    : msrElement (inputLineNumber)
{
  fSingleTremoloMarksNumber   = singleTremoloMarksNumber;
  fSingleTremoloPlacementKind = singleTremoloPlacementKind;
}

msrSingleTremolo::~msrSingleTremolo ()
{}

string msrSingleTremolo::singleTremoloPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fSingleTremoloPlacementKind);
}

void msrSingleTremolo::acceptIn (basevisitor* v)
{
  if (globalMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrSingleTremolo::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrSingleTremolo>*
    p =
      dynamic_cast<visitor<S_msrSingleTremolo>*> (v)) {
        S_msrSingleTremolo elem = this;

        if (globalMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrSingleTremolo::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSingleTremolo::acceptOut (basevisitor* v)
{
  if (globalMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrSingleTremolo::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrSingleTremolo>*
    p =
      dynamic_cast<visitor<S_msrSingleTremolo>*> (v)) {
        S_msrSingleTremolo elem = this;

        if (globalMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrSingleTremolo::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrSingleTremolo::browseData (basevisitor* v)
{}

string msrSingleTremolo::asString () const
{
  stringstream s;

  s <<
    "SingleTremolo" <<
    ", line " << fInputLineNumber <<
    ", " << fSingleTremoloMarksNumber << " marks" <<
    ", placement" << " = " << singleTremoloPlacementKindAsString ();

  return s.str ();
}

void msrSingleTremolo::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrSingleTremolo& elt)
{
  elt->print (os);
  return os;
}


}
