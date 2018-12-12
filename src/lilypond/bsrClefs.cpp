/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <iostream>
#include <sstream>

#include "bsrClefs.h"

#include "messagesHandling.h"

#include "generalOptions.h"
#include "bsrOptions.h"


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

  fClefCellsList = asCellsList ();
    
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceClefs) {
    gLogIOstream <<
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

S_bsrCellsList bsrClef::asCellsList () const
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
    case kClefKindFBaritone:
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
  return asCellsList ()->fetchCellsNumber();
}

void bsrClef::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrClef::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_bsrClef>*
    p =
      dynamic_cast<visitor<S_bsrClef>*> (v)) {
        S_bsrClef elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrClef::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrClef::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrClef::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrClef>*
    p =
      dynamic_cast<visitor<S_bsrClef>*> (v)) {
        S_bsrClef elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrClef::visitEnd ()" <<
            endl;
        }
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
    case bsrClef::kClefKindFBaritone:
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

void bsrClef::print (ostream& os)
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
