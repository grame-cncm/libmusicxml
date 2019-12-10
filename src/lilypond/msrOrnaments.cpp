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

  fOrnamentAccidentalMark = k_NoAlteration;
}

msrOrnament::~msrOrnament ()
{}

string msrOrnament::ornamentKindAsString () const
{
  string result;

  switch (fOrnamentKind) {
    case msrOrnament::kOrnamentTrill:
      result = "ornamentTrill";
      break;
    case msrOrnament::kOrnamentDashes:
      result = "ornamentDashes";
      break;
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
    case msrOrnament::kOrnamentAccidentalMark:
      result = "ornamentAccidentalMark";
      break;
  } // switch

  return result;
}

string msrOrnament::ornamentPlacementKindAsString () const
{
   return
    msrPlacementKindAsString (
      fOrnamentPlacementKind);
}

string msrOrnament::ornamentAccidentalMarkAsString () const
{
  string result;

  switch (fOrnamentAccidentalMark) {
    case kTripleFlat:
      result = "tripleFlat";
      break;
    case kDoubleFlat:
      result = "doubleFlat";
      break;
    case kSesquiFlat:
      result = "sesquiFlat";
      break;
    case kFlat:
      result = "flat";
      break;
    case kSemiFlat:
      result = "semiFlat";
      break;
    case kNatural:
      result = "natural";
      break;
    case kSemiSharp:
      result = "semiSharp";
      break;
    case kSharp:
      result = "sharp";
      break;
    case kSesquiSharp:
      result = "sesquiSharp";
      break;
    case kDoubleSharp:
      result = "doubleSharp";
      break;
    case kTripleSharp:
      result = "tripleSharp";
      break;
    case k_NoAlteration:
      result = "noAlteration";
      break;
  } // switch

  return result;
}

void msrOrnament::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrOrnament::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrOrnament>*
    p =
      dynamic_cast<visitor<S_msrOrnament>*> (v)) {
        S_msrOrnament elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrOrnament::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrOrnament::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrOrnament::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrOrnament>*
    p =
      dynamic_cast<visitor<S_msrOrnament>*> (v)) {
        S_msrOrnament elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrOrnament::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrOrnament::browseData (basevisitor* v)
{}

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
    ornamentAccidentalMarkAsString () <<
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
