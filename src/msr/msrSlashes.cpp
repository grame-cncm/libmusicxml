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
#include <iomanip>      // setw, setprecision, ...

#include "msrSlashes.h"

#include "msrOptions.h"


using namespace std;

namespace MusicXML2 {

//______________________________________________________________________________
S_msrSlash msrSlash::create (
  int                  inputLineNumber,
  msrSlashTypeKind     slashTypeKind,
  msrSlashUseDotsKind  slashUseDotsKind,
  msrSlashUseStemsKind slashUseStemsKind)
{
  msrSlash* o =
    new msrSlash (
      inputLineNumber,
      slashTypeKind,
      slashUseDotsKind,
      slashUseStemsKind);
  assert(o!=0);
  return o;
}

msrSlash::msrSlash (
  int                  inputLineNumber,
  msrSlashTypeKind     slashTypeKind,
  msrSlashUseDotsKind  slashUseDotsKind,
  msrSlashUseStemsKind slashUseStemsKind)
    : msrElement (inputLineNumber)
{
  fSlashTypeKind     = slashTypeKind;
  fSlashUseDotsKind  = slashUseDotsKind;
  fSlashUseStemsKind = slashUseStemsKind;
}

msrSlash::~msrSlash ()
{}

void msrSlash::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSlash::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrSlash>*
    p =
      dynamic_cast<visitor<S_msrSlash>*> (v)) {
        S_msrSlash elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSlash::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSlash::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrSlash::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrSlash>*
    p =
      dynamic_cast<visitor<S_msrSlash>*> (v)) {
        S_msrSlash elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrSlash::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrSlash::browseData (basevisitor* v)
{}

string msrSlash::asString () const
{
  stringstream s;

  s <<
    "Slash" <<
    ", line " << fInputLineNumber <<
    ", slashTypeKind:" <<
    msrSlashTypeKindAsString (
      fSlashTypeKind) <<
    ", slashUseDotsKind:" <<
    msrSlashUseDotsKindAsString (
      fSlashUseDotsKind) <<
    ", slashUseStemsKind:" <<
    msrSlashUseStemsKindAsString (
      fSlashUseStemsKind);
    
  return s.str ();
}

ostream& operator<< (ostream& os, const S_msrSlash& elt)
{
  elt->print (os);
  return os;
}

void msrSlash::print (ostream& os)
{
  os <<
    "Slash" <<
    ", line " << fInputLineNumber <<
    endl;
  
  gIndenter++;

  const int fieldWidth = 18;

  // print the voice uplink
  os << left <<
    setw (fieldWidth) <<
    "slashTypeKind" << " : " <<
    msrSlashTypeKindAsString (
      fSlashTypeKind) <<
    endl <<
    setw (fieldWidth) <<
    "slashUseDotsKind" << " : " <<
    msrSlashUseDotsKindAsString (
      fSlashUseDotsKind) <<
    endl <<
    setw (fieldWidth) <<
    "slashUseStemsKind" << " : " <<
    msrSlashUseStemsKindAsString (
      fSlashUseStemsKind) <<
    endl;
      
  gIndenter--;
}


}
