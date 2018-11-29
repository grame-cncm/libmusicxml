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

#include "bsrSpaces.h"

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
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrSpaces::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_bsrSpaces>*
    p =
      dynamic_cast<visitor<S_bsrSpaces>*> (v)) {
        S_bsrSpaces elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrSpaces::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrSpaces::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrSpaces::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrSpaces>*
    p =
      dynamic_cast<visitor<S_bsrSpaces>*> (v)) {
        S_bsrSpaces elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrSpaces::visitEnd ()" <<
            endl;
        }
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
    ", fNumberOfSpaces: " << fNumberOfSpaces <<
    ", spacesCellsList: " << fSpacesCellsList <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void bsrSpaces::print (ostream& os)
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
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrSpaces& elt)
{
  elt->print (os);
  return os;
}


}
