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

#include "bsrMutualDependencies.h"

#include "utilities.h"
#include "messagesHandling.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "bsrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_bsrClef bsrClef::create (
  int         inputLineNumber,
  bsrClefKind clefKind)
{
  bsrClef* o =
    new bsrClef (
      inputLineNumber, clefKind);
  assert (o!=0);
  return o;
}

bsrClef::bsrClef (
  int         inputLineNumber,
  bsrClefKind clefKind)
    : bsrLineElement (inputLineNumber)
{
  fClefKind = clefKind;

  fClefCellsList = buildCellsList ();

#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->fTraceClefs) {
    gLogOstream <<
      "Creating bsrClef '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      endl;
  }
#endif
}

bsrClef::~bsrClef ()
{}

S_bsrCellsList bsrClef::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (
        fInputLineNumber);

  switch (fClefKind) {
    case kClefKindNone:
      result =
        bsrCellsList::create (
          fInputLineNumber);
      break;

    case kClefKindGTreble:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          kDots345, kDots34, kDots123);
      break;
    case kClefKindFBass:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          kDots345, kDots3456, kDots123);
      break;
    case kClefKindCAlto:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          kDots345, kDots346, kDots123);
      break;

    case kClefKindGSoprano:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          kDots345, kDots34, kDots4, kDots123);
      break;
    case kClefKindCBaritone:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          kDots345, kDots3456, kDots456, kDots123);
      break;
    case kClefKindCTenor:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          kDots345, kDots346, kDots5, kDots123);
      break;

    case kClefKindGOttavaAlta:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          kDots345, kDots34, kDots123, kDots3456, kDots125);
      break;
    case kClefKindGOttavaBassa:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          kDots345, kDots34, kDots123, kDots3456, kDots236);
      break;

    case kClefKindModifiedBassForRightHandPart:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          kDots345, kDots3456, kDots13);
      break;
    case kClefKindModifiedTrebleForLeftHandPart:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          kDots345, kDots34, kDots13);
      break;
  } // switch

  return result;
}

int bsrClef::fetchCellsNumber() const
{
  return fClefCellsList->fetchCellsNumber();
}

void bsrClef::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrClef::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrClef>*
    p =
      dynamic_cast<visitor<S_bsrClef>*> (v)) {
        S_bsrClef elem = this;

#ifdef TRACE_OAH
        if (gGlobalBsrOah->fTraceBsrVisitors) {
          gLogOstream <<
            "% ==> Launching bsrClef::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrClef::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrClef::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrClef>*
    p =
      dynamic_cast<visitor<S_bsrClef>*> (v)) {
        S_bsrClef elem = this;

#ifdef TRACE_OAH
        if (gGlobalBsrOah->fTraceBsrVisitors) {
          gLogOstream <<
            "% ==> Launching bsrClef::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrClef::browseData (basevisitor* v)
{}


string bsrClef::clefKindAsString (
  bsrClefKind clefKind)
{
  string result;

  switch (clefKind) {
    case bsrClef::kClefKindNone:
      result = "***clefNone***";
      break;
    case bsrClef::kClefKindGTreble:
      result = "clefGTreble";
      break;
    case bsrClef::kClefKindFBass:
      result = "clefFBass";
      break;
    case bsrClef::kClefKindCAlto:
      result = "clefCAlto";
      break;
    case bsrClef::kClefKindGSoprano:
      result = "clefGSoprano";
      break;
    case bsrClef::kClefKindCBaritone:
      result = "clefFBaritone";
      break;
    case bsrClef::kClefKindCTenor:
      result = "clefCTenor";
      break;
    case bsrClef::kClefKindGOttavaAlta:
      result = "clefGOttavaAlta";
      break;
    case bsrClef::kClefKindGOttavaBassa:
      result = "clefGOttavaBassa";
      break;
    case bsrClef::kClefKindModifiedBassForRightHandPart:
      result = "clefModifiedBassForRightHandPart";
      break;
    case bsrClef::kClefKindModifiedTrebleForLeftHandPart:
      result = "clefModifiedTrebleForLeftHandPart";
      break;
  } // switch

  return result;
}

string bsrClef::clefKindAsDebugString (
  bsrClefKind clefKind)
{
  string result;

  switch (clefKind) {
    case bsrClef::kClefKindNone:
      result = "_";
      break;
    case bsrClef::kClefKindGTreble:
      result = "g2";
      break;
    case bsrClef::kClefKindFBass:
      result = "f4";
      break;
    case bsrClef::kClefKindCAlto:
      result = "u3";
      break;
    case bsrClef::kClefKindGSoprano:
      result = "g1";
      break;
    case bsrClef::kClefKindCBaritone:
      result = "u5";
      break;
    case bsrClef::kClefKindCTenor:
      result = "u4";
      break;
    case bsrClef::kClefKindGOttavaAlta:
      result = "g8a";
      break;
    case bsrClef::kClefKindGOttavaBassa:
      result = "g8b";
      break;
    case bsrClef::kClefKindModifiedBassForRightHandPart:
      result = "rh";
      break;
    case bsrClef::kClefKindModifiedTrebleForLeftHandPart:
      result = "lh";
      break;
  } // switch

  return result;
}

string bsrClef::asString () const
{
  stringstream s;

  s <<
    "Clef" <<
    ", clefKind: " <<
    clefKindAsString (fClefKind) <<
    "clefCellsList: " << fClefCellsList <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string bsrClef::asDebugString () const
{
  stringstream s;

  s <<
    "CLEF " <<
    clefKindAsDebugString (fClefKind) <<
    "]";

  return s.str ();
}

void bsrClef::print (ostream& os) const
{
  os <<
    "Clef" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 16;

  os <<
    setw (fieldWidth) <<
    "clefKind" << " : " <<
    clefKindAsString (fClefKind) <<
    endl <<
    setw (fieldWidth) <<
    "clefCellsList" << " : " << fClefCellsList <<
    endl <<
    setw (fieldWidth) <<
    "spacesBefore" << " : " << fSpacesBefore <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrClef& elt)
{
  elt->print (os);
  return os;
}


}
