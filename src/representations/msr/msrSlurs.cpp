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

#include "msrSlurs.h"

#include "oahOah.h"

#include "msrOah.h"

#include "messagesHandling.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrSlur msrSlur::create (
  int              inputLineNumber,
  int              slurNumber,
  msrSlurTypeKind  slurTypeKind,
  msrLineTypeKind  slurLineTypeKind,
  msrPlacementKind slurPlacementKind)
{
  msrSlur* o =
    new msrSlur (
      inputLineNumber,
      slurNumber,
      slurTypeKind,
      slurLineTypeKind,
      slurPlacementKind);
  assert (o != nullptr);
  return o;
}

msrSlur::msrSlur (
  int              inputLineNumber,
  int              slurNumber,
  msrSlurTypeKind  slurTypeKind,
  msrLineTypeKind  slurLineTypeKind,
  msrPlacementKind slurPlacementKind)
    : msrElement (inputLineNumber)
{
  fSlurNumber        = slurNumber;
  fSlurTypeKind      = slurTypeKind;
  fSlurLineTypeKind  = slurLineTypeKind;
  fSlurPlacementKind = slurPlacementKind;
}

msrSlur::~msrSlur ()
{}

void msrSlur::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSlur::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrSlur>*
    p =
      dynamic_cast<visitor<S_msrSlur>*> (v)) {
        S_msrSlur elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSlur::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSlur::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSlur::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrSlur>*
    p =
      dynamic_cast<visitor<S_msrSlur>*> (v)) {
        S_msrSlur elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSlur::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}


void msrSlur::browseData (basevisitor* v)
{}

string msrSlur::slurTypeKindAsString (
  msrSlurTypeKind slurTypeKind)
{
  stringstream s;

  switch (slurTypeKind) {
    case msrSlur::k_NoSlur:
      s << "Slur" << slurTypeKind << "???";
      break;
    case msrSlur::kRegularSlurStart:
      s << "regularSlurStart";
      break;
    case msrSlur::kPhrasingSlurStart:
      s << "phrasingSlurStart";
      break;
    case msrSlur::kSlurContinue:
      s << "slurContinue";
      break;
    case msrSlur::kRegularSlurStop:
      s << "regularSlurStop";
      break;
    case msrSlur::kPhrasingSlurStop:
      s << "phrasingSlurStop";
      break;
  } // switch

  return s.str ();
}

string msrSlur::slurTypeKindAsString () const
{
  return slurTypeKindAsString (fSlurTypeKind);
}

string msrSlur::asString () const
{
  stringstream s;

  s <<
    "[Slur " <<
    ", slurNumber: " << fSlurNumber <<
    slurTypeKindAsString () <<
    ", slurLineTypeKind: " <<
    msrLineTypeKindAsString (
      fSlurLineTypeKind) <<
    ", slurLineTypeKind: " <<
    msrPlacementKindAsString (
      fSlurPlacementKind) <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrSlur::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrSlur& elt)
{
  elt->print (os);
  return os;
}


}
