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

#include "msrRehearsals.h"

#include "msrOptions.h"


using namespace std;

namespace MusicXML2 {

//______________________________________________________________________________
S_msrRehearsal msrRehearsal::create (
  int              inputLineNumber,
  msrRehearsalKind rehearsalKind,
  string           rehearsalText,
  msrPlacementKind rehearsalPlacementKind)
{
  msrRehearsal* o =
    new msrRehearsal (
      inputLineNumber,
      rehearsalKind,
      rehearsalText,
      rehearsalPlacementKind);
  assert (o!=0);
  return o;
}

msrRehearsal::msrRehearsal (
  int              inputLineNumber,
  msrRehearsalKind rehearsalKind,
  string           rehearsalText,
  msrPlacementKind rehearsalPlacementKind)
    : msrElement (inputLineNumber)
{
  fRehearsalKind = rehearsalKind;

  fRehearsalText = rehearsalText;

  fRehearsalPlacementKind = rehearsalPlacementKind;
}

msrRehearsal::~msrRehearsal ()
{}

void msrRehearsal::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrRehearsal::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrRehearsal>*
    p =
      dynamic_cast<visitor<S_msrRehearsal>*> (v)) {
        S_msrRehearsal elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRehearsal::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRehearsal::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrRehearsal::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrRehearsal>*
    p =
      dynamic_cast<visitor<S_msrRehearsal>*> (v)) {
        S_msrRehearsal elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRehearsal::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrRehearsal::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrRehearsal& elt)
{
  elt->print (os);
  return os;
}

string msrRehearsal::rehearsalKindAsString (
  msrRehearsalKind rehearsalKind)
{
  string result;
  
  switch (rehearsalKind) {
    case msrRehearsal::kNone:
      result = "none";
      break;
    case msrRehearsal::kRectangle:
      result = "rectangle";
      break;
    case msrRehearsal::kOval:
      result = "oval";
      break;
    case msrRehearsal::kCircle:
      result = "circle";
      break;
    case msrRehearsal::kBracket:
      result = "bracket";
      break;
    case msrRehearsal::kTriangle:
      result = "triangle";
      break;
    case msrRehearsal::kDiamond:
      result = "diamond";
      break;
  } // switch

  return result;
}

void msrRehearsal::print (ostream& os)
{
  os <<
    "Rehearsal" << " " << fRehearsalText <<
    " kind: " <<
    rehearsalKindAsString (fRehearsalKind) <<
    " rehearsalPlacementKind: " <<
    msrPlacementKindAsString (fRehearsalPlacementKind) <<
    endl;
}


}
