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

#include "bsrDynamics.h"

#include "utilities.h"
#include "messagesHandling.h"

#include "oahOah.h"

#include "bsrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_bsrDynamics bsrDynamics::create (
  int             inputLineNumber,
  msrDynamicsKind dynamicsKind)
{
  bsrDynamics* o =
    new bsrDynamics (
      inputLineNumber, dynamicsKind);
  assert (o != nullptr);
  return o;
}

bsrDynamics::bsrDynamics (
  int             inputLineNumber,
  msrDynamicsKind dynamicsKind)
    : bsrLineContentsElement (inputLineNumber)
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
    case kDynamicsF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellF));
      break;
    case kDynamicsFF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellF, kCellF));
      break;
    case kDynamicsFFF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellF, kCellF, kCellF));
      break;
    case kDynamicsFFFF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellF, kCellF, kCellF, kCellF));
      break;
    case kDynamicsFFFFF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellF, kCellF, kCellF, kCellF, kCellF));
      break;
    case kDynamicsFFFFFF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellF, kCellF, kCellF, kCellF, kCellF, kCellF));
      break;

    case kDynamicsP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellP));
      break;
    case kDynamicsPP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellP, kCellP));
      break;
    case kDynamicsPPP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellP, kCellP, kCellP));
      break;
    case kDynamicsPPPP:
      result->
        appendCellsListToCellsList (
         bsrCellsList::create (
            fInputLineNumber, kCellP, kCellP, kCellP, kCellP));
      break;
    case kDynamicsPPPPP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellP, kCellP, kCellP, kCellP, kCellP));
      break;
    case kDynamicsPPPPPP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellP, kCellP, kCellP, kCellP, kCellP, kCellP));
      break;

    case kDynamicsMF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellM, kCellF));
      break;
    case kDynamicsMP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellM, kCellP));
      break;
    case kDynamicsFP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellM, kCellF));
      break;
    case kDynamicsFZ:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellF, kCellZ));
      break;
    case kDynamicsRF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellR, kCellF));
      break;
    case kDynamicsSF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellS, kCellF));
      break;

    case kDynamicsRFZ:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellR, kCellF, kCellZ));
      break;
    case kDynamicsSFZ:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellS, kCellF, kCellZ));
      break;
    case kDynamicsSFP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellS, kCellF, kCellP));
      break;
    case kDynamicsSFPP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellS, kCellF, kCellP, kCellP));
      break;
    case kDynamicsSFFZ:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellS, kCellF, kCellF, kCellZ));
      break;
    case k_NoDynamics:
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
    dynamicsKindAsString (fDynamicsKind) <<
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
    dynamicsKindAsString (fDynamicsKind) <<
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

  ++gIndenter;

  const unsigned int fieldWidth = 23;

  os <<
    setw (fieldWidth) <<
    "dynamicsKind" << " : " <<
    dynamicsKindAsString (fDynamicsKind) <<
    endl <<
    setw (fieldWidth) <<
    "noteCellsList" << " : " <<
    fDynamicsCellsList->asShortString () <<
    endl <<
    setw (fieldWidth) <<
    "spacesBefore" << " : " << fSpacesBefore <<
    endl;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_bsrDynamics& elt)
{
  elt->print (os);
  return os;
}


}
