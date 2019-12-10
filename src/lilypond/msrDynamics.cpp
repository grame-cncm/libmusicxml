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
#include <iomanip>      // setw, setprecision, ...

#include "msrDynamics.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrDynamics msrDynamics::create (
  int              inputLineNumber,
  msrDynamicsKind  dynamicsKind,
  msrPlacementKind dynamicsPlacementKind)
{
  msrDynamics* o =
    new msrDynamics (
      inputLineNumber,
      dynamicsKind,
      dynamicsPlacementKind);
    assert(o!=0);
  return o;
}

msrDynamics::msrDynamics (
  int              inputLineNumber,
  msrDynamicsKind  dynamicsKind,
  msrPlacementKind dynamicsPlacementKind)
    : msrElement (inputLineNumber)
{
  fDynamicsKind = dynamicsKind;

  fDynamicsPlacementKind = dynamicsPlacementKind;
}

msrDynamics::~msrDynamics ()
{}

string msrDynamics::dynamicsKindAsString (
  msrDynamicsKind dynamicsKind)
{
  string result;

  switch (dynamicsKind) {
    case msrDynamics::kF:
      result = "f";
      break;
    case msrDynamics::kFF:
      result = "ff";
      break;
    case msrDynamics::kFFF:
      result = "fff";
      break;
    case msrDynamics::kFFFF:
      result = "ffff";
      break;
    case msrDynamics::kFFFFF:
      result = "fffff";
      break;
    case msrDynamics::kFFFFFF:
      result = "ffffff";
      break;

    case msrDynamics::kP:
      result = "p";
      break;
    case msrDynamics::kPP:
      result = "pp";
      break;
    case msrDynamics::kPPP:
      result = "ppp";
      break;
    case msrDynamics::kPPPP:
      result = "pppp";
      break;
    case msrDynamics::kPPPPP:
      result = "ppppp";
      break;
    case msrDynamics::kPPPPPP:
      result = "pppppp";
      break;

    case msrDynamics::kMF:
      result = "mf";
      break;
    case msrDynamics::kMP:
      result = "mp";
      break;
    case msrDynamics::kFP:
      result = "fp";
      break;
    case msrDynamics::kFZ:
      result = "fz";
      break;
    case msrDynamics::kRF:
      result = "rf";
      break;
    case msrDynamics::kSF:
      result = "sf";
      break;

    case msrDynamics::kRFZ:
      result = "rfz";
      break;
    case msrDynamics::kSFZ:
      result = "sfz";
      break;
    case msrDynamics::kSFP:
      result = "sfp";
      break;
    case msrDynamics::kSFPP:
      result = "sfpp";
      break;
    case msrDynamics::kSFFZ:
      result = "sffz";
      break;
    case msrDynamics::k_NoDynamics:
      result = "k_NoDynamics???";
  } // switch

  return result;
}

string msrDynamics::dynamicsKindAsString () const
{
  return
    dynamicsKindAsString (fDynamicsKind);
}

string msrDynamics::dynamicsPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fDynamicsPlacementKind);
}

void msrDynamics::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrDynamics::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrDynamics>*
    p =
      dynamic_cast<visitor<S_msrDynamics>*> (v)) {
        S_msrDynamics elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrDynamics::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrDynamics::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrDynamics::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrDynamics>*
    p =
      dynamic_cast<visitor<S_msrDynamics>*> (v)) {
        S_msrDynamics elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrDynamics::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}


void msrDynamics::browseData (basevisitor* v)
{}

void msrDynamics::print (ostream& os) const
{
  os <<
    "Dynamics" " " <<
    dynamicsKindAsString () <<
    ", dynamicsPlacementKind: " <<
    dynamicsPlacementKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrDynamics& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOtherDynamics msrOtherDynamics::create (
  int              inputLineNumber,
  string           otherDynamicsString,
  msrPlacementKind otherDynamicsPlacementKind)
{
  msrOtherDynamics* o =
    new msrOtherDynamics (
      inputLineNumber,
      otherDynamicsString,
      otherDynamicsPlacementKind);
    assert(o!=0);
  return o;
}

msrOtherDynamics::msrOtherDynamics (
  int              inputLineNumber,
  string           otherDynamicsString,
  msrPlacementKind otherDynamicsPlacementKind)
    : msrElement (inputLineNumber)
{
  fOtherDynamicsString = otherDynamicsString;

  fOtherDynamicsPlacementKind = otherDynamicsPlacementKind;
}

msrOtherDynamics::~msrOtherDynamics ()
{}

string msrOtherDynamics::otherDynamicsPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fOtherDynamicsPlacementKind);
}

void msrOtherDynamics::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrOtherDynamics::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrOtherDynamics>*
    p =
      dynamic_cast<visitor<S_msrOtherDynamics>*> (v)) {
        S_msrOtherDynamics elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrOtherDynamics::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrOtherDynamics::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrOtherDynamics::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrOtherDynamics>*
    p =
      dynamic_cast<visitor<S_msrOtherDynamics>*> (v)) {
        S_msrOtherDynamics elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrOtherDynamics::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}


void msrOtherDynamics::browseData (basevisitor* v)
{}

string msrOtherDynamics::asString () const
{
  stringstream s;

  s <<
    "OtherDynamics '" <<
    ", otherDynamicsString: " <<
    fOtherDynamicsString <<
    ", otherDynamicsPlacementKind: " <<
    otherDynamicsPlacementKindAsString () <<
    "', line " << fInputLineNumber;

  return s.str ();
}

void msrOtherDynamics::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrOtherDynamics& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrWedge msrWedge::create (
  int                inputLineNumber,
  msrWedgeKind       wedgeKind,
  msrWedgeNienteKind wedgeNienteKind,
  msrLineTypeKind    wedgeLineTypeKind,
  msrPlacementKind   wedgePlacementKind)
{
  msrWedge* o =
    new msrWedge (
      inputLineNumber,
      wedgeKind,
      wedgeNienteKind,
      wedgeLineTypeKind,
      wedgePlacementKind);
  assert(o!=0);
  return o;
}

msrWedge::msrWedge (
  int                inputLineNumber,
  msrWedgeKind       wedgeKind,
  msrWedgeNienteKind wedgeNienteKind,
  msrLineTypeKind    wedgeLineTypeKind,
  msrPlacementKind   wedgePlacementKind)
    : msrElement (inputLineNumber)
{
  fWedgeKind          = wedgeKind;
  fWedgeNienteKind    = wedgeNienteKind;
  fWedgeLineTypeKind  = wedgeLineTypeKind;
  fWedgePlacementKind = wedgePlacementKind;
}

msrWedge::~msrWedge ()
{}

string msrWedge::wedgeKindAsString () const
{
  string result;

  switch (fWedgeKind) {
    case msrWedge::kWedgeKindNone:
      result = "wedgeKindNone";
      break;
    case msrWedge::kWedgeCrescendo:
      result = "wedgeCrescendo";
      break;
    case msrWedge::kWedgeDecrescendo:
      result = "wedgeDecrescendo";
      break;
    case msrWedge::kWedgeStop:
      result = "wedgeStop";
      break;
  } // switch

  return result;
}

string msrWedge::wedgeNienteKindAsString (
  msrWedgeNienteKind wedgeNienteKind)
{
  string result;

  switch (wedgeNienteKind) {
    case msrWedge::kWedgeNienteYes:
      result = "kWedgeNienteYes";
      break;
    case msrWedge::kWedgeNienteNo:
      result = "kWedgeNienteNo";
      break;
  } // switch

  return result;
}

void msrWedge::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrWedge::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrWedge>*
    p =
      dynamic_cast<visitor<S_msrWedge>*> (v)) {
        S_msrWedge elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrWedge::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrWedge::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrWedge::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrWedge>*
    p =
      dynamic_cast<visitor<S_msrWedge>*> (v)) {
        S_msrWedge elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrWedge::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrWedge::browseData (basevisitor* v)
{}

void msrWedge::print (ostream& os) const
{
  os <<
    "Wedge" " " << wedgeKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 19;

  os << left <<
    setw (fieldWidth) <<
    "wedgeNienteKind" << " : " <<
    wedgeNienteKindAsString (
      fWedgeNienteKind) <<
    endl <<
    setw (fieldWidth) <<
    "wedgeLineTypeKind" << " : " <<
    msrLineTypeKindAsString (
      fWedgeLineTypeKind) <<
    endl <<
    setw (fieldWidth) <<
    "wedgePlacementKind" << " : " <<
    msrPlacementKindAsString (
      fWedgePlacementKind) <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrWedge& elt)
{
  elt->print (os);
  return os;
}


}
