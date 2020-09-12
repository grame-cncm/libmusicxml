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
#include "messagesHandling.h"

#include "msrLigatures.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrLigature msrLigature::create (
  int                    inputLineNumber,
  int                    ligatureNumber,
  msrLigatureKind        ligatureKind,
  msrLigatureLineEndKind ligatureLineEndKind,
  msrLineTypeKind        ligatureLineTypeKind,
  msrPlacementKind       ligaturePlacementKind)
{
  msrLigature* o =
    new msrLigature (
      inputLineNumber,
      ligatureNumber,
      ligatureKind,
      ligatureLineEndKind,
      ligatureLineTypeKind,
      ligaturePlacementKind);
  assert(o!=0);
  return o;
}

msrLigature::msrLigature (
  int                    inputLineNumber,
  int                    ligatureNumber,
  msrLigatureKind        ligatureKind,
  msrLigatureLineEndKind ligatureLineEndKind,
  msrLineTypeKind        ligatureLineTypeKind,
  msrPlacementKind       ligaturePlacementKind)
    : msrElement (inputLineNumber)
{
  fLigatureNumber        = ligatureNumber;
  fLigatureKind          = ligatureKind;
  fLigatureLineEndKind   = ligatureLineEndKind;
  fLigatureLineTypeKind  = ligatureLineTypeKind;
  fLigaturePlacementKind = ligaturePlacementKind;
}

msrLigature::~msrLigature ()
{}

void msrLigature::setLigatureOtherEndSideLink (
  S_msrLigature otherEndSideLink)
{
  // sanity check
  msgAssert (
    otherEndSideLink != nullptr,
    "otherEndSideLink is null");

  // set the two-way sideLink between both ends of the spanner
  fLigatureOtherEndSideLink =
    otherEndSideLink;

  otherEndSideLink->fLigatureOtherEndSideLink =
    this;
}

void msrLigature::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrLigature::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrLigature>*
    p =
      dynamic_cast<visitor<S_msrLigature>*> (v)) {
        S_msrLigature elem = this;

        if (gGlobalMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrLigature::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrLigature::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrLigature::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrLigature>*
    p =
      dynamic_cast<visitor<S_msrLigature>*> (v)) {
        S_msrLigature elem = this;

        if (gGlobalMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrLigature::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}


void msrLigature::browseData (basevisitor* v)
{}

string msrLigature::ligatureKindAsString (
  msrLigatureKind ligatureKind)
{
  stringstream s;

  switch (ligatureKind) {
    case msrLigature::kLigatureStart:
      s << "ligatureStart";
      break;
    case msrLigature::kLigatureContinue:
      s << "ligatureContinue";
      break;
    case msrLigature::kLigatureStop:
      s << "ligatureStop";
      break;
    case kLigatureNone:
      s << "ligatureNone";
  } // switch

  return s.str ();

}

string msrLigature::ligatureLineEndKindAsString (
  msrLigatureLineEndKind ligatureLineEndKind)
{
  stringstream s;

  switch (ligatureLineEndKind) {
    case msrLigature::kLigatureLineEndUp:
      s << "ligatureLineEndUp";
      break;
    case msrLigature::kLigatureLineEndDown:
      s << "ligatureLineEndDown";
      break;
    case msrLigature::kLigatureLineEndBoth:
      s << "ligatureLineEndBoth";
      break;
    case msrLigature::kLigatureLineEndArrow:
      s << "ligatureLineEndArrow";
      break;
    case msrLigature::kLigatureLineEndNone:
      s << "ligatureLineEndNone";
      break;
  } // switch

  return s.str ();

}

string msrLigature::ligatureKindAsString () const
{
  return ligatureKindAsString (fLigatureKind);
}

void msrLigature::print (ostream& os) const
{
  os <<
    "Ligature " << ligatureKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 22;

  os << left <<
    setw (fieldWidth) <<
    "ligatureLineEndKind" << " : " <<
    ligatureLineEndKindAsString (
      fLigatureLineEndKind) <<
    endl <<
    setw (fieldWidth) <<
    "ligatureLineTypeKind" << " : " <<
    msrLineTypeKindAsString (
      fLigatureLineTypeKind) <<
    endl <<
    setw (fieldWidth) <<
    "ligaturePlacementKind" << " : " <<
    msrPlacementKindAsString (
      fLigaturePlacementKind) <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrLigature& elt)
{
  elt->print (os);
  return os;
}


}
