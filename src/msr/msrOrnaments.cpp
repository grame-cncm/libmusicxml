/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "msrMutualDependencies.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrOrnament msrOrnament::create (
  int              inputLineNumber,
  msrOrnamentKind  ornamentKind,
  msrPlacementKind ornamentPlacementKind)
{
  msrOrnament* o =
    new msrOrnament (
      inputLineNumber, ornamentKind, ornamentPlacementKind);
  assert (o!=0);
  return o;
}

msrOrnament::msrOrnament (
  int              inputLineNumber,
  msrOrnamentKind  ornamentKind,
  msrPlacementKind ornamentPlacementKind)
    : msrElement (inputLineNumber)
{
  fOrnamentKind = ornamentKind;

  fOrnamentPlacementKind = ornamentPlacementKind;

  fOrnamentAccidentalKind = kAccidentalNone;
}

msrOrnament::~msrOrnament ()
{}

void msrOrnament::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrOrnament::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrOrnament>*
    p =
      dynamic_cast<visitor<S_msrOrnament>*> (v)) {
        S_msrOrnament elem = this;

        if (gGlobalMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrOrnament::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrOrnament::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrOrnament::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrOrnament>*
    p =
      dynamic_cast<visitor<S_msrOrnament>*> (v)) {
        S_msrOrnament elem = this;

        if (gGlobalMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrOrnament::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrOrnament::browseData (basevisitor* v)
{}

string msrOrnament::ornamentKindAsString (
  msrOrnamentKind ornamentKind)
{
  string result;

  switch (ornamentKind) {
    case msrOrnament::kOrnamentTrill:
      result = "ornamentTrill";
      break;
      /* JMI
    case msrOrnament::kOrnamentDashes:
      result = "ornamentDashes";
      break;
      */
    case msrOrnament::kOrnamentTurn:
      result = "ornamentTurn";
      break;
    case msrOrnament::kOrnamentInvertedTurn:
      result = "ornamentInvertedTurn";
      break;
    case msrOrnament::kOrnamentDelayedTurn:
      result = "ornamentDelayedTurn";
      break;
    case msrOrnament::kOrnamentDelayedInvertedTurn:
      result = "ornamentDelayedInvertedTurn";
      break;
    case msrOrnament::kOrnamentVerticalTurn:
      result = "ornamentVerticalTurn";
      break;
    case msrOrnament::kOrnamentMordent:
      result = "ornamentMordent";
      break;
    case msrOrnament::kOrnamentInvertedMordent:
      result = "ornamentInvertedMordent";
      break;
    case msrOrnament::kOrnamentSchleifer:
      result = "ornamentSchleifer";
      break;
    case msrOrnament::kOrnamentShake:
      result = "ornamentShake";
      break;
    case msrOrnament::kOrnamentAccidentalKind:
      result = "ornamentAccidentalKind";
      break;
  } // switch

  return result;
}

string msrOrnament::ornamentKindAsString () const
{
  return ornamentKindAsString (fOrnamentKind);
}

string msrOrnament::ornamentPlacementKindAsString () const
{
   return
    msrPlacementKindAsString (
      fOrnamentPlacementKind);
}

string msrOrnament::ornamentAccidentalKindAsString () const
{
  return
    accidentalKindAsString (
      fOrnamentAccidentalKind);
}

string msrOrnament::asString () const
{
  stringstream s;

  s <<
    "[Ornament" <<
    ", ornamentKind: " << ornamentKindAsString () <<
    ", ornamentPlacementKind: " << msrPlacementKindAsString (fOrnamentPlacementKind) <<
    ", ornamentAccidentalKind: " << accidentalKindAsString (fOrnamentAccidentalKind) <<
    "]";

  return s.str ();
}


void msrOrnament::print (ostream& os) const
{
  os <<
    "Ornament" <<
    ", " << ornamentKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 16;

  os <<
    setw (fieldWidth) <<
    "placement" << " : " <<
    ornamentPlacementKindAsString () <<
    endl <<
    setw (fieldWidth) <<
    "accidental mark" << " : " <<
    ornamentAccidentalKindAsString () <<
    endl <<
    setw (fieldWidth) <<
    "noteUpLink" << " : " <<
    fOrnamentNoteUpLink->asShortString () <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrOrnament& elt)
{
  elt->print (os);
  return os;
}


}
