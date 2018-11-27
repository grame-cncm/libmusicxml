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

#include "bsrKeys.h"

#include "bsrNumbers.h"

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
S_bsrKey bsrKey::create (
  int        inputLineNumber,
  bsrKeyKind keyKind,
  int        numberOfAlterations)
{
  bsrKey* o =
    new bsrKey (
      inputLineNumber, keyKind, numberOfAlterations);
  assert (o!=0);

  return o;
}

bsrKey::bsrKey (
  int        inputLineNumber,
  bsrKeyKind keyKind,
  int        numberOfAlterations)
    : bsrBrailleElement (inputLineNumber)
{
  fKeyKind             = keyKind;
  fNumberOfAlterations = numberOfAlterations;

  // consistency check
  if (fKeyKind == kKeyKindNaturals && fNumberOfAlterations != 0) {
    stringstream s;

    s <<
      "BSR key inconsistency:" <<
      "keyKind: " << keyKindAsString (fKeyKind) <<
      "numberOfAlterations: " << fNumberOfAlterations;
       
    bsrInternalError (
      gXml2brlOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fKeyBrailleSign = asBrailleSign ();
}

bsrKey::~bsrKey ()
{}


S_bsrBrailleSign bsrKey::keyKindAsBrailleSign ()
{
  S_bsrBrailleSign result;

  switch (fKeyKind) {
    case bsrKey::kKeyKindFlats:
      result =
        bsrBrailleSign::create (
          fInputLineNumber,
          kCellFlat);
      break;
    case bsrKey::kKeyKindNaturals:
      result =
        bsrBrailleSign::create (
          fInputLineNumber,
          kCellNatural);
      break;
    case bsrKey::kKeyKindSharps:
      result =
        bsrBrailleSign::create (
          fInputLineNumber,
          kCellSharp);
      break;
  } // switch

  return result;
}

S_bsrBrailleSign bsrKey::asBrailleSign ()
{
  S_bsrBrailleSign
    result =
      bsrBrailleSign::create (
        fInputLineNumber);

  switch (fNumberOfAlterations) {
    case 0:
      break;
      
    case 1:
    case 2:
    case 3:
      // create as many flat or sharp signs as needed
      for (int i = 1; i <= fNumberOfAlterations; i++) {
        result->appendBrailleSignToBrailleSign (
          keyKindAsBrailleSign ());
      } // for
      break;
      
    default:
      {
        // create the number of alterations
        S_bsrNumber
          number =
            bsrNumber::create (
              fInputLineNumber,
              fNumberOfAlterations,
              bsrNumber::kNumberSignIsNeededYes);

        // append it to result
        result->appendBrailleSignToBrailleSign (
          number->asBrailleSign ());
  
        // append the flat or sharp sign to result
        result->appendBrailleSignToBrailleSign (
          keyKindAsBrailleSign ());
      }
  } // switch
  
  return result;
}

void bsrKey::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrKey::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_bsrKey>*
    p =
      dynamic_cast<visitor<S_bsrKey>*> (v)) {
        S_bsrKey elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrKey::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrKey::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrKey::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrKey>*
    p =
      dynamic_cast<visitor<S_bsrKey>*> (v)) {
        S_bsrKey elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrKey::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void bsrKey::browseData (basevisitor* v)
{}

string bsrKey::keyKindAsString (
  bsrKeyKind keyKind)
{
  string result;
 
  switch (keyKind) {
    case bsrKey::kKeyKindFlats:
      result = "keyKindFlats";
      break;
    case bsrKey::kKeyKindNaturals:
      result = "keyKindNaturals";
      break;
    case bsrKey::kKeyKindSharps:
      result = "keyKindSharps";
      break;
  } // switch

  return result;
}

string bsrKey::asString () const
{
  stringstream s;

  s <<
    "Key" <<
    ", " << keyKindAsString (fKeyKind) <<
    ", numberOfAlterations: " << fNumberOfAlterations <<
    ", keyBrailleSign: " << fKeyBrailleSign->asShortString () <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void bsrKey::print (ostream& os)
{
  os <<
    "Key" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;
  
  const int fieldWidth = 20;

  os <<
    setw (fieldWidth) <<
    "keyKind" << " : " << keyKindAsString (fKeyKind) <<
    endl <<
    setw (fieldWidth) <<
    "numberOfAlterations" << " : " << fNumberOfAlterations <<
    endl <<
    setw (fieldWidth) <<
    "keyBrailleSign" << " : " << fKeyBrailleSign->asShortString () <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrKey& elt)
{
  elt->print (os);
  return os;
}


}
