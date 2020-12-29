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

#include "bsrDynamics_MUT_DEP.h"

#include "utilities.h"
#include "messagesHandling.h"

#include "oahOah.h"

#include "bsrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_bsrDynamics bsrDynamics::create (
  int                          inputLineNumber,
  msrDynamics::msrDynamicsKind dynamicsKind)
{
  bsrDynamics* o =
    new bsrDynamics (
      inputLineNumber, dynamicsKind);
  assert (o!=0);
  return o;
}

bsrDynamics::bsrDynamics (
  int                          inputLineNumber,
  msrDynamics::msrDynamicsKind dynamicsKind)
    : bsrLineElement (inputLineNumber)
{
  fDynamicsKind  = dynamicsKind;

  fDynamicsCellsList = buildCellsList ();
}

bsrDynamics::~bsrDynamics ()
{}

S_bsrCellsList bsrDynamics::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (
        fInputLineNumber, kCellWordSign);

  switch (fDynamicsKind) {
    case msrDynamics::kF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellF));
      break;
    case msrDynamics::kFF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellF, kCellF));
      break;
    case msrDynamics::kFFF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellF, kCellF, kCellF));
      break;
    case msrDynamics::kFFFF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellF, kCellF, kCellF, kCellF));
      break;
    case msrDynamics::kFFFFF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellF, kCellF, kCellF, kCellF, kCellF));
      break;
    case msrDynamics::kFFFFFF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellF, kCellF, kCellF, kCellF, kCellF, kCellF));
      break;

    case msrDynamics::kP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellP));
      break;
    case msrDynamics::kPP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellP, kCellP));
      break;
    case msrDynamics::kPPP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellP, kCellP, kCellP));
      break;
    case msrDynamics::kPPPP:
      result->
        appendCellsListToCellsList (
         bsrCellsList::create (
            fInputLineNumber, kCellP, kCellP, kCellP, kCellP));
      break;
    case msrDynamics::kPPPPP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellP, kCellP, kCellP, kCellP, kCellP));
      break;
    case msrDynamics::kPPPPPP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellP, kCellP, kCellP, kCellP, kCellP, kCellP));
      break;

    case msrDynamics::kMF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellM, kCellF));
      break;
    case msrDynamics::kMP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellM, kCellP));
      break;
    case msrDynamics::kFP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellM, kCellF));
      break;
    case msrDynamics::kFZ:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellF, kCellZ));
      break;
    case msrDynamics::kRF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellR, kCellF));
      break;
    case msrDynamics::kSF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellS, kCellF));
      break;

    case msrDynamics::kRFZ:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellR, kCellF, kCellZ));
      break;
    case msrDynamics::kSFZ:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellS, kCellF, kCellZ));
      break;
    case msrDynamics::kSFP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellS, kCellF, kCellP));
      break;
    case msrDynamics::kSFPP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellS, kCellF, kCellP, kCellP));
      break;
    case msrDynamics::kSFFZ:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellS, kCellF, kCellF, kCellZ));
      break;
    case msrDynamics::k_NoDynamics:
      result =
        bsrCellsList::create (fInputLineNumber);
  } // switch

  return result;
}

int bsrDynamics::fetchCellsNumber() const
{
  return fDynamicsCellsList->fetchCellsNumber();
}

void bsrDynamics::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrDynamics::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrDynamics>*
    p =
      dynamic_cast<visitor<S_bsrDynamics>*> (v)) {
        S_bsrDynamics elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrDynamics::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrDynamics::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrDynamics::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrDynamics>*
    p =
      dynamic_cast<visitor<S_bsrDynamics>*> (v)) {
        S_bsrDynamics elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrDynamics::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrDynamics::browseData (basevisitor* v)
{}

string bsrDynamics::asString () const
{
  stringstream s;

  s <<
    "Dynamics" <<
    ", dynamicsKind: " <<
    msrDynamics::dynamicsKindAsString (fDynamicsKind) <<
    ", dynamicsCellsList: " <<
    fDynamicsCellsList->asShortString () <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string bsrDynamics::asDebugString () const
{
  stringstream s;

  s <<
    "Dyn" <<
    ", dynamicsKind: " <<
    msrDynamics::dynamicsKindAsString (fDynamicsKind) <<
    ", dynamicsCellsList: " <<
    fDynamicsCellsList->asShortString () <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void bsrDynamics::print (ostream& os) const
{
  os <<
    "Dynamics" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 23;

  os <<
    setw (fieldWidth) <<
    "dynamicsKind" << " : " <<
    msrDynamics::dynamicsKindAsString (fDynamicsKind) <<
    endl <<
    setw (fieldWidth) <<
    "noteCellsList" << " : " <<
    fDynamicsCellsList->asShortString () <<
    endl <<
    setw (fieldWidth) <<
    "spacesBefore" << " : " << fSpacesBefore <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrDynamics& elt)
{
  elt->print (os);
  return os;
}


}
