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
#include <iomanip>      // setw, setprecision, ...

#include "utilities.h"

#include "msrSlashes.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrSlash msrSlash::create (
  int                  inputLineNumber,
  msrSlashTypeKind     slashTypeKind,
  msrUseDotsKind  useDotsKind,
  msrSlashUseStemsKind slashUseStemsKind)
{
  msrSlash* o =
    new msrSlash (
      inputLineNumber,
      slashTypeKind,
      useDotsKind,
      slashUseStemsKind);
  assert (o!=0);
  return o;
}

msrSlash::msrSlash (
  int                  inputLineNumber,
  msrSlashTypeKind     slashTypeKind,
  msrUseDotsKind  useDotsKind,
  msrSlashUseStemsKind slashUseStemsKind)
    : msrElement (inputLineNumber)
{
  fSlashTypeKind     = slashTypeKind;
  fUseDotsKind  = useDotsKind;
  fSlashUseStemsKind = slashUseStemsKind;
}

msrSlash::~msrSlash ()
{}

void msrSlash::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSlash::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrSlash>*
    p =
      dynamic_cast<visitor<S_msrSlash>*> (v)) {
        S_msrSlash elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSlash::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSlash::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSlash::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrSlash>*
    p =
      dynamic_cast<visitor<S_msrSlash>*> (v)) {
        S_msrSlash elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
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
    ", useDotsKind:" <<
    msrUseDotsKindAsString (
      fUseDotsKind) <<
    ", slashUseStemsKind:" <<
    msrSlashUseStemsKindAsString (
      fSlashUseStemsKind);

  return s.str ();
}

void msrSlash::print (ostream& os) const
{
  os <<
    "Slash" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 18;

  // print the voice upLink
  os << left <<
    setw (fieldWidth) <<
    "slashTypeKind" << " : " <<
    msrSlashTypeKindAsString (
      fSlashTypeKind) <<
    endl <<
    setw (fieldWidth) <<
    "useDotsKind" << " : " <<
    msrUseDotsKindAsString (
      fUseDotsKind) <<
    endl <<
    setw (fieldWidth) <<
    "slashUseStemsKind" << " : " <<
    msrSlashUseStemsKindAsString (
      fSlashUseStemsKind) <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrSlash& elt)
{
  elt->print (os);
  return os;
}


}
