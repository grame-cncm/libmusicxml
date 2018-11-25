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

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "bsrOptions.h"
#include "xml2brlOptionsHandling.h"

#include "messagesHandling.h"


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
    : bsrBrailleElement (inputLineNumber)
{
  fClefKind = clefKind;

  fClefBrailleSign =
    bsrBrailleSign::create (inputLineNumber);
}

bsrClef::~bsrClef ()
{}

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
    case kClefNone:
      result = "***clefNone***";
      break;
    case kClefGTrebleKind:
      result = "clefGTrebleKind";
      break;
    case kClefFBassKind:
      result = "clefFBassKind";
      break;
    case kClefGSopranoKind:
      result = "clefGSopranoKind";
      break;
    case kClefFBaritoneKind:
      result = "clefFBaritoneKind";
      break;
    case kClefCTenorKind:
      result = "clefCTenorKind";
      break;
    case kClefGOttavaAltaKind:
      result = "clefGOttavaAltaKind";
      break;
    case kClefGOttavaBassaKind:
      result = "clefGOttavaBassaKind";
      break;
    case kClefModifiedBassForRightHandPartKind:
      result = "clefModifiedBassForRightHandPartKind";
      break;
    case kClefModifiedTrebleForLeftHandPartKind:
      result = "clefModifiedTrebleForLeftHandPartKind";
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
    ", line " << fInputLineNumber;

  return s.str ();
}

void bsrClef::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_bsrClef& elt)
{
  elt->print (os);
  return os;
}


}
