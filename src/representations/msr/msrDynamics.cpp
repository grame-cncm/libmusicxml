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

#include "utilities.h"

#include "msrDynamics.h"

#include "oahOah.h"

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
    assert (o != nullptr);
  return o;
}

S_msrDynamics msrDynamics::createDynamicsFromString (
  int              inputLineNumber,
  string           dynamicsString,
  msrPlacementKind dynamicsPlacementKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceDynamicss ()) {
    gLogStream <<
      "Creating dynamics from string \"" <<
      dynamicsString <<
      "\", dynamicsMeasureNumber: '" << dynamicsMeasureNumber <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  return
    msrDynamics::create (
      inputLineNumber,
      dynamicsFromString (dynamicsString),
      dynamicsPlacementKind);
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

void msrDynamics::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrDynamics::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrDynamics>*
    p =
      dynamic_cast<visitor<S_msrDynamics>*> (v)) {
        S_msrDynamics elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrDynamics::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrDynamics::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrDynamics::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrDynamics>*
    p =
      dynamic_cast<visitor<S_msrDynamics>*> (v)) {
        S_msrDynamics elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
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
    msrDynamicsKindAsString (fDynamicsKind) <<
    ", dynamicsPlacementKind: " <<
    msrPlacementKindAsString (fDynamicsPlacementKind) <<
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
    assert (o != nullptr);
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

void msrOtherDynamics::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrOtherDynamics::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrOtherDynamics>*
    p =
      dynamic_cast<visitor<S_msrOtherDynamics>*> (v)) {
        S_msrOtherDynamics elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrOtherDynamics::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrOtherDynamics::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrOtherDynamics::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrOtherDynamics>*
    p =
      dynamic_cast<visitor<S_msrOtherDynamics>*> (v)) {
        S_msrOtherDynamics elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
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
    msrPlacementKindAsString (fOtherDynamicsPlacementKind) <<
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
  assert (o != nullptr);
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
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrWedge::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrWedge>*
    p =
      dynamic_cast<visitor<S_msrWedge>*> (v)) {
        S_msrWedge elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrWedge::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrWedge::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrWedge::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrWedge>*
    p =
      dynamic_cast<visitor<S_msrWedge>*> (v)) {
        S_msrWedge elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
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

  ++gIndenter;

  const unsigned int fieldWidth = 19;

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

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msrWedge& elt)
{
  elt->print (os);
  return os;
}


}
