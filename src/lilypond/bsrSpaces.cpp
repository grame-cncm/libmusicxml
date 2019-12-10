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

#include "bsrMutualDependencies.h"

#include "messagesHandling.h"

#include "bsrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_bsrSpaces bsrSpaces::create (
  int inputLineNumber,
  int numberOfSpaces)
{
  bsrSpaces* o =
    new bsrSpaces (
      inputLineNumber, numberOfSpaces);
  assert (o!=0);
  return o;
}

bsrSpaces::bsrSpaces (
  int inputLineNumber,
  int numberOfSpaces)
    : bsrLineElement (inputLineNumber)
{
  fNumberOfSpaces = numberOfSpaces;

  fSpacesCellsList =
    bsrCellsList::create (inputLineNumber);

  for (int i = 1; i <= fNumberOfSpaces; i++) {
    fSpacesCellsList->
      appendCellKindToCellsList (kCellSpace);
  } // for
}

bsrSpaces::~bsrSpaces ()
{}

void bsrSpaces::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrSpaces::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrSpaces>*
    p =
      dynamic_cast<visitor<S_bsrSpaces>*> (v)) {
        S_bsrSpaces elem = this;

#ifdef TRACE_OAH
        if (gBsrOah->fTraceBsrVisitors) {
          gLogOstream <<
            "% ==> Launching bsrSpaces::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

int bsrSpaces::fetchCellsNumber () const
{
  return fNumberOfSpaces;
}

void bsrSpaces::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrSpaces::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrSpaces>*
    p =
      dynamic_cast<visitor<S_bsrSpaces>*> (v)) {
        S_bsrSpaces elem = this;

#ifdef TRACE_OAH
        if (gBsrOah->fTraceBsrVisitors) {
          gLogOstream <<
            "% ==> Launching bsrSpaces::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrSpaces::browseData (basevisitor* v)
{}

string bsrSpaces::asString () const
{
  stringstream s;

  s <<
    "Spaces" <<
    ", numberOfSpaces: " << fNumberOfSpaces <<
    ", spacesCellsList: " << fSpacesCellsList <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void bsrSpaces::print (ostream& os) const
{
  os <<
    "Spaces" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 16;

  os <<
    setw (fieldWidth) <<
    "numberOfSpaces" << " : " << fNumberOfSpaces <<
    endl <<
    setw (fieldWidth) <<
    "spacesCellsList" << " : " << fSpacesCellsList <<
    endl <<
    setw (fieldWidth) <<
    "spacesBefore" << " : " << fSpacesBefore <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrSpaces& elt)
{
  elt->print (os);
  return os;
}


}
