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

#include "msrClefs.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrClef msrClef::create (
  int         inputLineNumber,
  msrClefKind clefKind)
{
  msrClef* o =
    new msrClef (
      inputLineNumber, clefKind);
  assert(o!=0);
  return o;
}

msrClef::msrClef (
  int         inputLineNumber,
  msrClefKind clefKind)
    : msrMeasureElement (inputLineNumber)
{
  fClefKind = clefKind;
}

msrClef::~msrClef ()
{}

bool msrClef::clefIsATablatureClef () const
{
  switch (fClefKind) {
    case msrClef::kTablature4Clef:
    case msrClef::kTablature5Clef:
    case msrClef::kTablature6Clef:
    case msrClef::kTablature7Clef:
      return true;
      break;
    default:
      return false;
  } // switch
}

bool msrClef::clefIsAPercussionClef () const
{
  switch (fClefKind) {
    case msrClef::kPercussionClef:
      return true;
      break;
    default:
      return false;
  } // switch
}

bool msrClef::isEqualTo (S_msrClef otherClef) const
{
  if (! otherClef) {
    return false;
  }

  return fClefKind == otherClef->fClefKind;
}

void msrClef::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrClef::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrClef::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrClef::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrClef::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrClef::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrClef::browseData (basevisitor* v)
{}

string msrClef::clefKindAsString (
  msrClefKind clefKind)
{
  string result;

  switch (clefKind) {
    case msrClef::k_NoClef:
      result = "none";
      break;
    case msrClef::kTrebleClef:
      result = "treble";
      break;
    case msrClef::kSopranoClef:
      result = "soprano";
      break;
    case msrClef::kMezzoSopranoClef:
      result = "mezzo soprano";
      break;
    case msrClef::kAltoClef:
      result = "alto";
      break;
    case msrClef::kTenorClef:
      result = "tenor";
      break;
    case msrClef::kBaritoneClef:
      result = "baritone";
      break;
    case msrClef::kBassClef:
      result = "bass";
      break;
    case msrClef::kTrebleLine1Clef:
      result = "treble line 1";
      break;
    case msrClef::kTrebleMinus15Clef:
      result = "treble -15";
      break;
    case msrClef::kTrebleMinus8Clef:
      result = "treble -8";
      break;
    case msrClef::kTreblePlus8Clef:
      result = "treble +8";
      break;
    case msrClef::kTreblePlus15Clef:
      result = "treble +15";
      break;
    case msrClef::kBassMinus15Clef:
      result = "bass -15";
      break;
    case msrClef::kBassMinus8Clef:
      result = "bass -8";
      break;
    case msrClef::kBassPlus8Clef:
      result = "bass +8";
      break;
    case msrClef::kBassPlus15Clef:
      result = "bass +15";
      break;
    case msrClef::kVarbaritoneClef:
      result = "varbaritone";
      break;
    case msrClef::kTablature4Clef:
      result = "tablature 4 lines";
      break;
    case msrClef::kTablature5Clef:
      result = "tablature 5 lines";
      break;
    case msrClef::kTablature6Clef:
      result = "tablature 6 lines";
      break;
    case msrClef::kTablature7Clef:
      result = "tablature 7 lines";
      break;
    case msrClef::kPercussionClef:
      result = "percussion";
      break;
    case msrClef::kJianpuClef:
      result = "jianpu";
      break;
  } // switch

  return result;
}

msrClef::msrClefKind msrClef::clefKindFromString (
  string theString)
{
  msrClef::msrClefKind result = msrClef::k_NoClef;

  if      (theString == "treble")
    result = msrClef::kTrebleClef;
  else if (theString == "bass")
    result = msrClef::kBassClef;
  else if (theString == "alto")
    result = msrClef::kAltoClef;
  else if (theString == "tenor")
    result = msrClef::kTenorClef;
  else if (theString == "soprano")
    result = msrClef::kSopranoClef;
  else if (theString == "mezzosoprano")
    result = msrClef::kMezzoSopranoClef;
  else if (theString == "baritone")
    result = msrClef::kBaritoneClef;
  else if (theString == "treble^8")
    result = msrClef::kTreblePlus8Clef;
  else if (theString == "treble_8")
    result = msrClef::kTrebleMinus8Clef;
  else if (theString == "bass_8")
    result = msrClef::kBassMinus8Clef;
  else if (theString == "bass^8")
    result = msrClef::kBassPlus8Clef;
  else if (theString == "varbaritone")
    result = msrClef::kVarbaritoneClef;
  else if (theString == "treble^15")
    result = msrClef::kTreblePlus15Clef;
  else if (theString == "treble_15")
    result = msrClef::kTrebleMinus15Clef;
  else if (theString == "bass^15")
    result = msrClef::kBassPlus15Clef;
  else if (theString == "bass_15")
    result = msrClef::kBassMinus15Clef;
  else if (theString == "tab4")
    result = msrClef::kTablature4Clef;
  else if (theString == "tab5")
    result = msrClef::kTablature5Clef;
  else if (theString == "tab6")
    result = msrClef::kTablature6Clef;
  else if (theString == "tab7")
    result = msrClef::kTablature7Clef;
  else if (theString == "percussion")
    result = msrClef::kPercussionClef;
  else if (theString == "jianpu")
    result = msrClef::kJianpuClef;

  return result;
}

string msrClef::asString () const
{
  stringstream s;

  s <<
    "[Clef " <<
    clefKindAsString (fClefKind) <<
    "], line " << fInputLineNumber;

  return s.str ();
}

void msrClef::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrClef& elt)
{
  elt->print (os);
  return os;
}


}
