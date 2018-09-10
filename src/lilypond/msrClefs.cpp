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

#include "msrClefs.h"

#include "msrOptions.h"


using namespace std;

namespace MusicXML2 {

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
    : msrElement (inputLineNumber)
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
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrClef::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrClef::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrClef::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrClef::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrClef::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrClef::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrClef& elt)
{
  elt->print (os);
  return os;
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
    "Clef" " \"";
   
  switch (fClefKind) {
    case msrClef::k_NoClef:
      s << "none";
      break;
    case msrClef::kTrebleClef:
      s << "treble";
      break;
    case msrClef::kSopranoClef:
      s << "soprano";
      break;
    case msrClef::kMezzoSopranoClef:
      s << "mezzo soprano";
      break;
    case msrClef::kAltoClef:
      s << "alto";
      break;
    case msrClef::kTenorClef:
      s << "tenor";
      break;
    case msrClef::kBaritoneClef:
      s << "baritone";
      break;
    case msrClef::kBassClef:
      s << "bass";
      break;
    case msrClef::kTrebleLine1Clef:
      s << "treble line 1";
      break;
    case msrClef::kTrebleMinus15Clef:
      s << "treble -15";
      break;
    case msrClef::kTrebleMinus8Clef:
      s << "treble -8";
      break;
    case msrClef::kTreblePlus8Clef:
      s << "treble +8";
      break;
    case msrClef::kTreblePlus15Clef:
      s << "treble +15";
      break;
    case msrClef::kBassMinus15Clef:
      s << "bass -15";
      break;
    case msrClef::kBassMinus8Clef:
      s << "bass -8";
      break;
    case msrClef::kBassPlus8Clef:
      s << "bass +8";
      break;
    case msrClef::kBassPlus15Clef:
      s << "bass +15";
      break;
    case msrClef::kVarbaritoneClef:
      s << "varbaritone";
      break;
    case msrClef::kTablature4Clef:
      s << "tablature 4 lines";
      break;
    case msrClef::kTablature5Clef:
      s << "tablature 5 lines";
      break;
    case msrClef::kTablature6Clef:
      s << "tablature 6 lines";
      break;
    case msrClef::kTablature7Clef:
      s << "tablature 7 lines";
      break;
    case msrClef::kPercussionClef:
      s << "percussion";
      break;
    case msrClef::kJianpuClef:
      s << "jianpu";
      break;
  } // switch

  s <<
    "\", line " << fInputLineNumber;

  return s.str ();
}

void msrClef::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}


}
