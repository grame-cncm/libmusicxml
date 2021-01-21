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

#include "msrTies.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrTie msrTie::create (
  int              inputLineNumber,
  msrTieKind       tieKind,
  msrPlacementKind placementKind)
{
  msrTie* o =
    new msrTie (
      inputLineNumber,
      tieKind,
      placementKind);
  assert (o != nullptr);
  return o;
}

S_msrTie msrTie::create (
  int              inputLineNumber,
  msrTieKind       tieKind)
{
  return
    msrTie::create (
      inputLineNumber,
      tieKind,
      k_NoPlacement);
}

msrTie::msrTie (
  int              inputLineNumber,
  msrTieKind       tieKind,
  msrPlacementKind placementKind)
    : msrElement (inputLineNumber)
{
  fTieKind = tieKind;

  fTiePlacementKind = placementKind;
}

msrTie::~msrTie ()
{}

void msrTie::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTie::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrTie>*
    p =
      dynamic_cast<visitor<S_msrTie>*> (v)) {
        S_msrTie elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTie::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTie::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTie::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrTie>*
    p =
      dynamic_cast<visitor<S_msrTie>*> (v)) {
        S_msrTie elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTie::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTie::browseData (basevisitor* v)
{}

string msrTie::asString () const
{
  stringstream s;

  s <<
    "Tie" << " " << msrTieKindAsString (fTieKind) <<
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
