/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "messagesHandling.h"

#include "msrSpanners.h"

#include "msrNotes.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrSpanner msrSpanner::create (
  int                inputLineNumber,
  int                spannerNumber,
  msrSpannerKind     spannerKind,
  msrSpannerTypeKind spannerTypeKind,
  msrPlacementKind   spannerPlacementKind,
  S_msrNote          spannerNoteUpLink)
{
  msrSpanner* o =
    new msrSpanner (
      inputLineNumber,
      spannerNumber,
      spannerKind,
      spannerTypeKind,
      spannerPlacementKind,
      spannerNoteUpLink);
  assert (o != nullptr);
  return o;
}

msrSpanner::msrSpanner (
  int                inputLineNumber,
  int                spannerNumber,
  msrSpannerKind     spannerKind,
  msrSpannerTypeKind spannerTypeKind,
  msrPlacementKind   spannerPlacementKind,
  S_msrNote          spannerNoteUpLink)
    : msrElement (inputLineNumber)
{
  fSpannerNoteUpLink = spannerNoteUpLink;

  fSpannerNumber = spannerNumber;

  fSpannerKind = spannerKind;

  fSpannerTypeKind = spannerTypeKind;

  fSpannerPlacementKind = spannerPlacementKind;
}

msrSpanner::~msrSpanner ()
{}

void msrSpanner::setSpannerOtherEndSideLink (
  S_msrSpanner otherEndSideLink)
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    otherEndSideLink != nullptr,
    "otherEndSideLink is null");

  // set the two-way sideLink between both ends of the spanner
  fSpannerOtherEndSideLink =
    otherEndSideLink;

  otherEndSideLink->fSpannerOtherEndSideLink =
    this;
}

void msrSpanner::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSpanner::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrSpanner>*
    p =
      dynamic_cast<visitor<S_msrSpanner>*> (v)) {
        S_msrSpanner elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSpanner::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSpanner::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSpanner::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrSpanner>*
    p =
      dynamic_cast<visitor<S_msrSpanner>*> (v)) {
        S_msrSpanner elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSpanner::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrSpanner::browseData (basevisitor* v)
{}

string msrSpanner::spannerKindAsString (
  msrSpannerKind spannerKind)
{
  string result;

  switch (spannerKind) {
    case msrSpanner::kSpannerDashes:
      result = "spannerDashes";
      break;
    case msrSpanner::kSpannerWavyLine:
      result = "spannerWavyLine";
      break;
  } // switch

  return result;
}

string msrSpanner::spannerTypeKindAsString () const
{
  return
    msrSpannerTypeKindAsString (
      fSpannerTypeKind);
}

string msrSpanner::spannerKindAsString () const
{
  return
    spannerKindAsString (
      fSpannerKind);
}

string msrSpanner::spannerPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fSpannerPlacementKind);
}

string msrSpanner::asShortString () const
{
  stringstream s;

  s <<
    "Spanner" <<
    ", spannerNumber = " << fSpannerNumber <<
    ", " << spannerKindAsString () <<
    ", line " << fInputLineNumber <<
    ", spannerPlacementKind" << " : " <<
    spannerPlacementKindAsString () <<
    ", spannerTypeKind" << " : " <<
    spannerTypeKindAsString ();

  return s.str ();
}

void msrSpanner::print (ostream& os) const
{
  os <<
    "Spanner" <<
    ", spannerNumber = " << fSpannerNumber <<
    ", " << spannerKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const unsigned int fieldWidth = 24;

  os << left <<
    setw (fieldWidth) <<
    "spannerPlacementKind" << " : " <<
    spannerPlacementKindAsString () <<
    endl <<
    setw (fieldWidth) <<
    "spannerTypeKind" << " : " <<
    spannerTypeKindAsString () <<
    endl <<
    setw (fieldWidth) <<
    "spannerNoteUpLink" << " : " <<
    fSpannerNoteUpLink->asShortString () <<
    endl <<
    setw (fieldWidth) <<
    "spannerBeginText" << " : \"" <<
    fSpannerBeginText <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "spannerMiddleText" << " : \"" <<
    fSpannerMiddleText <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "spannerEndText" << " : \"" <<
    fSpannerEndText <<
    "\"" <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "spannerNoteUpLink" << " : " <<
    spannerPlacementKindAsString () <<
    endl;
  ++gIndenter;
    os <<
      fSpannerNoteUpLink->asString () <<
      endl;
  --gIndenter;

  os << left <<
    setw (fieldWidth) <<
    "spannerOtherEndSideLink";
  if (fSpannerOtherEndSideLink) {
    os <<
      endl <<
      fSpannerOtherEndSideLink->asShortString ();
  }
  else {
    os <<
    " : " << "***none***" <<
    endl;
  }

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msrSpanner& elt)
{
  elt->print (os);
  return os;
}


}
