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

#include "bsrNumbers.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "bsrOptions.h"
#include "xml2brlOptionsHandling.h"

#include "messagesHandling.h"


using namespace std;

namespace MusicXML2 {

//______________________________________________________________________________
S_bsrNumber bsrNumber::create (
  int       inputLineNumber,
  int       numberValue,
  bsrNumberSignIsNeededKind
            numberSignIsNeededKind)
{
  bsrNumber* o =
    new bsrNumber (
      inputLineNumber, numberValue, numberSignIsNeededKind);
  assert (o!=0);
  return o;
}

bsrNumber::bsrNumber (
  int       inputLineNumber,
  int       numberValue,
  bsrNumberSignIsNeededKind
            numberSignIsNeededKind)
    : bsrBrailleElement (inputLineNumber)
{
  fNumberValue = numberValue;
    
  fNumberSignIsNeededKind = numberSignIsNeededKind;
}

bsrNumber::~bsrNumber ()
{}

void bsrNumber::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrNumber::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_bsrNumber>*
    p =
      dynamic_cast<visitor<S_bsrNumber>*> (v)) {
        S_bsrNumber elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrNumber::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrNumber::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrNumber::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrNumber>*
    p =
      dynamic_cast<visitor<S_bsrNumber>*> (v)) {
        S_bsrNumber elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrNumber::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void bsrNumber::browseData (basevisitor* v)
{}

string bsrNumber::numberSignIsNeededKindAsString (
  bsrNumberSignIsNeededKind numberSignIsNeededKind)
{
  string result;
  
  switch (numberSignIsNeededKind) {
    case bsrNumber::kNumberSignIsNeededYes:
      result = "numberSignIsNeededYes";
      break;
    case bsrNumber::kNumberSignIsNeededNo:
      result = "numberSignIsNeededNo";
      break;
  } // switch

  return result;
}

string bsrNumber::asString () const
{
  stringstream s;

  s <<
    "Number, " << 
    ", numberValue: " << fNumberValue <<
    ", numberSignIsNeeded: " <<
    numberSignIsNeededKindAsString (
      fNumberSignIsNeededKind) <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void bsrNumber::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_bsrNumber& elt)
{
  elt->print (os);
  return os;
}


}
