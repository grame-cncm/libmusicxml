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

#include "msrPartNamesDisplays.h"

#include "msrOptions.h"


using namespace std;

namespace MusicXML2 {

//______________________________________________________________________________
S_msrPartNameDisplay msrPartNameDisplay::create (
  int    inputLineNumber,
  string partNameDisplayValue)
{
  msrPartNameDisplay* o =
    new msrPartNameDisplay (
      inputLineNumber, partNameDisplayValue);
  assert(o!=0);
  return o;
}

msrPartNameDisplay::msrPartNameDisplay (
  int    inputLineNumber,
  string partNameDisplayValue)
    : msrElement (inputLineNumber)
{
  fPartNameDisplayValue = partNameDisplayValue;
}

msrPartNameDisplay::~msrPartNameDisplay ()
{}

void msrPartNameDisplay::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPartNameDisplay::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrPartNameDisplay>*
    p =
      dynamic_cast<visitor<S_msrPartNameDisplay>*> (v)) {
        S_msrPartNameDisplay elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPartNameDisplay::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPartNameDisplay::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPartNameDisplay::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrPartNameDisplay>*
    p =
      dynamic_cast<visitor<S_msrPartNameDisplay>*> (v)) {
        S_msrPartNameDisplay elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPartNameDisplay::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrPartNameDisplay::browseData (basevisitor* v)
{}

string msrPartNameDisplay::asString () const
{
  stringstream s;

  s <<
    "PartNameDisplay" <<
    ", line " << fInputLineNumber <<
    ", partNameDisplayValue: " << fPartNameDisplayValue;
    
  return s.str ();
}

ostream& operator<< (ostream& os, const S_msrPartNameDisplay& elt)
{
  elt->print (os);
  return os;
}

void msrPartNameDisplay::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

//______________________________________________________________________________
S_msrPartAbbreviationDisplay msrPartAbbreviationDisplay::create (
  int    inputLineNumber,
  string partAbbreviationDisplayValue)
{
  msrPartAbbreviationDisplay* o =
    new msrPartAbbreviationDisplay (
      inputLineNumber, partAbbreviationDisplayValue);
  assert(o!=0);
  return o;
}

msrPartAbbreviationDisplay::msrPartAbbreviationDisplay (
  int    inputLineNumber,
  string partAbbreviationDisplayValue)
    : msrElement (inputLineNumber)
{
  fPartAbbreviationDisplayValue = partAbbreviationDisplayValue;
}

msrPartAbbreviationDisplay::~msrPartAbbreviationDisplay ()
{}

void msrPartAbbreviationDisplay::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPartAbbreviationDisplay::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrPartAbbreviationDisplay>*
    p =
      dynamic_cast<visitor<S_msrPartAbbreviationDisplay>*> (v)) {
        S_msrPartAbbreviationDisplay elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPartAbbreviationDisplay::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPartAbbreviationDisplay::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPartAbbreviationDisplay::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrPartAbbreviationDisplay>*
    p =
      dynamic_cast<visitor<S_msrPartAbbreviationDisplay>*> (v)) {
        S_msrPartAbbreviationDisplay elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPartAbbreviationDisplay::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrPartAbbreviationDisplay::browseData (basevisitor* v)
{}

string msrPartAbbreviationDisplay::asString () const
{
  stringstream s;

  s <<
    "PartAbbreviationDisplay" <<
    ", line " << fInputLineNumber <<
    ", " <<
    "partAbbreviationDisplayValue: " << fPartAbbreviationDisplayValue;
    
  return s.str ();
}

ostream& operator<< (ostream& os, const S_msrPartAbbreviationDisplay& elt)
{
  elt->print (os);
  return os;
}

void msrPartAbbreviationDisplay::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}


}
