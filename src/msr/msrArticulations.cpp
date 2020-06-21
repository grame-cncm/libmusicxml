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

#include "msrArticulations.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrArticulation msrArticulation::create (
  int                 inputLineNumber,
  msrArticulationKind articulationKind,
  msrPlacementKind    articulationPlacementKind)
{
  msrArticulation* o =
    new msrArticulation (
      inputLineNumber,
      articulationKind,
      articulationPlacementKind);
  assert (o!=0);
  return o;
}

msrArticulation::msrArticulation (
  int                 inputLineNumber,
  msrArticulationKind articulationKind,
  msrPlacementKind    articulationPlacementKind)
    : msrElement (inputLineNumber)
{
  fArticulationKind = articulationKind;

  fArticulationPlacementKind = articulationPlacementKind;
}

msrArticulation::~msrArticulation ()
{}

string msrArticulation::articulationKindAsString (
  msrArticulationKind articulationKind)
{
  string result;

  switch (articulationKind) {
    case msrArticulation::k_NoArticulation:
      result = "noArticulation";
      break;

    case msrArticulation::kAccent:
      result = "accent";
      break;
    case msrArticulation::kBreathMark:
      result = "breathMark";
      break;
    case msrArticulation::kCaesura:
      result = "caesura";
      break;
    case msrArticulation::kSpiccato:
      result = "spiccato";
      break;
    case msrArticulation::kStaccato:
      result = "staccato";
      break;
    case msrArticulation::kStaccatissimo:
      result = "staccatissimo";
      break;
    case msrArticulation::kStress:
      result = "stress";
      break;
    case msrArticulation::kUnstress:
      result = "unstress";
      break;
    case msrArticulation::kDetachedLegato:
      result = "detachedLegato";
      break;
    case msrArticulation::kStrongAccent:
      result = "strongAccent";
      break;
    case msrArticulation::kTenuto:
      result = "tenuto";
      break;
    case msrArticulation::kFermata:
      result = "fermata";
      break;
    case msrArticulation::kArpeggiato:
      result = "arpeggiato";
      break;
    case msrArticulation::kNonArpeggiato:
      result = "nonArpeggiato";
      break;
    case msrArticulation::kDoit:
      result = "doit";
      break;
    case msrArticulation::kFalloff:
      result = "falloff";
      break;
    case msrArticulation::kPlop:
      result = "plop";
      break;
    case msrArticulation::kScoop:
      result = "scoop";
      break;
  } // switch

  return result;
}

string msrArticulation::articulationKindAsString () const
{
  return
    articulationKindAsString (
      fArticulationKind);
}

string msrArticulation::articulationPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fArticulationPlacementKind);
}

void msrArticulation::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrArticulation::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrArticulation>*
    p =
      dynamic_cast<visitor<S_msrArticulation>*> (v)) {
        S_msrArticulation elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrArticulation::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrArticulation::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrArticulation::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrArticulation>*
    p =
      dynamic_cast<visitor<S_msrArticulation>*> (v)) {
        S_msrArticulation elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrArticulation::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrArticulation::browseData (basevisitor* v)
{}

void msrArticulation::print (ostream& os) const
{
  os <<
    "Articulation" " " <<
    articulationKindAsString () <<
    ", " <<
    articulationPlacementKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrArticulation& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrFermata msrFermata::create (
  int                inputLineNumber,
  msrFermataKind     fermataKind,
  msrFermataTypeKind fermataTypeKind)
{
  msrFermata* o =
    new msrFermata (
      inputLineNumber,
      fermataKind,
      fermataTypeKind);
  assert (o!=0);
  return o;
}

msrFermata::msrFermata (
    int                inputLineNumber,
    msrFermataKind     fermataKind,
    msrFermataTypeKind fermataTypeKind)
    : msrArticulation (
      inputLineNumber,
      msrArticulation::kFermata,
      kPlacementNone) // temporary, JMI TEMP
{
  fFermataKind = fermataKind;
  fFermataTypeKind = fermataTypeKind;
}

msrFermata::~msrFermata ()
{}

void msrFermata::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrFermata::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrFermata>*
    p =
      dynamic_cast<visitor<S_msrFermata>*> (v)) {
        S_msrFermata elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrFermata::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrFermata::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrFermata::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrFermata>*
    p =
      dynamic_cast<visitor<S_msrFermata>*> (v)) {
        S_msrFermata elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrFermata::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrFermata::browseData (basevisitor* v)
{}

string msrFermata::fermataKindAsString (
  msrFermataKind fermataKind)
{
  string result;

  switch (fermataKind) {
    case msrFermata::kNormalFermataKind:
      result = "fermata kind: normal";
      break;
    case msrFermata::kAngledFermataKind:
      result = "fermata kind: angled";
      break;
    case msrFermata::kSquareFermataKind:
      result = "fermata kind: square";
      break;
  } // switch

  return result;
}

string msrFermata::fermataTypeKindAsString (
  msrFermataTypeKind fermataTypeKind)
{
  string result;

  switch (fermataTypeKind) {
    case msrFermata::kFermataTypeNone:
      result = "fermataTypeNone";
      break;
    case msrFermata::kFermataTypeUpright:
      result = "fermataTypeUpright";
      break;
    case msrFermata::kFermataTypeInverted:
      result = "fermataTypeInverted";
      break;
  } // switch

  return result;
}

string msrFermata::asString () const
{
  stringstream s;

  s <<
    "Fermata" <<
    ", " <<
    fermataKindAsString (fFermataKind) <<
    ", " <<
    fermataTypeKindAsString (fFermataTypeKind) <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrFermata::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrFermata& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrArpeggiato msrArpeggiato::create (
  int              inputLineNumber,
  msrPlacementKind arpeggiatoPlacementKind,
  msrDirectionKind arpeggiatoDirectionKind,
  int              arpeggiatoNumber)
{
  msrArpeggiato* o =
    new msrArpeggiato (
      inputLineNumber,
      arpeggiatoPlacementKind,
      arpeggiatoDirectionKind,
      arpeggiatoNumber);
  assert (o!=0);
  return o;
}

msrArpeggiato::msrArpeggiato (
  int              inputLineNumber,
  msrPlacementKind arpeggiatoPlacementKind,
  msrDirectionKind arpeggiatoDirectionKind,
  int              arpeggiatoNumber)
    : msrArticulation (
      inputLineNumber,
      kArpeggiato,
      arpeggiatoPlacementKind)
{
  fArpeggiatoDirectionKind = arpeggiatoDirectionKind;

  fArpeggiatoNumber = arpeggiatoNumber;
}

msrArpeggiato::~msrArpeggiato ()
{}

string msrArpeggiato::arpeggiatoDirectionKindAsString () const
{
  return
    msrDirectionKindAsString (fArpeggiatoDirectionKind);
}

void msrArpeggiato::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrArpeggiato::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrArpeggiato>*
    p =
      dynamic_cast<visitor<S_msrArpeggiato>*> (v)) {
        S_msrArpeggiato elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrArpeggiato::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrArpeggiato::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrArpeggiato::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrArpeggiato>*
    p =
      dynamic_cast<visitor<S_msrArpeggiato>*> (v)) {
        S_msrArpeggiato elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrArpeggiato::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrArpeggiato::browseData (basevisitor* v)
{}

void msrArpeggiato::print (ostream& os) const
{
  os <<
    "Arpeggiato" " " <<
    articulationKindAsString () <<
    ", " <<
    articulationPlacementKindAsString () <<
    ", " <<
    arpeggiatoDirectionKindAsString () <<
    ", number: " <<
    fArpeggiatoNumber <<
    ", line " << fInputLineNumber <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrArpeggiato& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrNonArpeggiato msrNonArpeggiato::create (
  int                      inputLineNumber,
  msrPlacementKind         nonArpeggiatoPlacementKind,
  msrNonArpeggiatoTypeKind nonArpeggiatoTypeKind,
  int                      nonArpeggiatoNumber)
{
  msrNonArpeggiato* o =
    new msrNonArpeggiato (
      inputLineNumber,
      nonArpeggiatoPlacementKind,
      nonArpeggiatoTypeKind,
      nonArpeggiatoNumber);
  assert (o!=0);
  return o;
}

msrNonArpeggiato::msrNonArpeggiato (
  int                      inputLineNumber,
  msrPlacementKind         nonArpeggiatoPlacementKind,
  msrNonArpeggiatoTypeKind nonArpeggiatoTypeKind,
  int                      nonArpeggiatoNumber)
    : msrArticulation (
      inputLineNumber,
      kNonArpeggiato,
      nonArpeggiatoPlacementKind)
{
  fNonArpeggiatoTypeKind = nonArpeggiatoTypeKind;

  fNonArpeggiatoNumber = nonArpeggiatoNumber;
}

msrNonArpeggiato::~msrNonArpeggiato ()
{}

string msrNonArpeggiato::nonArpeggiatoTypeKindAsString (
  msrNonArpeggiatoTypeKind nonArpeggiatoTypeKind)
{
  string result;

  switch (nonArpeggiatoTypeKind) {
    case msrNonArpeggiato::kNonArpeggiatoTypeNone:
      result = "nonArpeggiatoTypeNone";
      break;
    case msrNonArpeggiato::kNonArpeggiatoTypeTop:
      result = "nonArpeggiatoTypeTop";
      break;
    case msrNonArpeggiato::kNonArpeggiatoTypeBottom:
      result = "nonArpeggiatoTypeBottom";
      break;
  } // switch

  return result;
}

string msrNonArpeggiato::nonArpeggiatoTypeKindAsString () const
{
  return
    nonArpeggiatoTypeKindAsString (fNonArpeggiatoTypeKind);
}

void msrNonArpeggiato::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrNonArpeggiato::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrNonArpeggiato>*
    p =
      dynamic_cast<visitor<S_msrNonArpeggiato>*> (v)) {
        S_msrNonArpeggiato elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrNonArpeggiato::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrNonArpeggiato::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrNonArpeggiato::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrNonArpeggiato>*
    p =
      dynamic_cast<visitor<S_msrNonArpeggiato>*> (v)) {
        S_msrNonArpeggiato elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrNonArpeggiato::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrNonArpeggiato::browseData (basevisitor* v)
{}

void msrNonArpeggiato::print (ostream& os) const
{
  os <<
    "NonArpeggiato" " " <<
    articulationKindAsString () <<
    ", " <<
    articulationPlacementKindAsString () <<
    ", " <<
    nonArpeggiatoTypeKindAsString () <<
    ", number: " <<
    fNonArpeggiatoNumber <<
    ", line " << fInputLineNumber <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrNonArpeggiato& elt)
{
  elt->print (os);
  return os;
}


}
