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

#include <sstream>

#include "bsrStrings.h"

#include "messagesHandling.h"

#include "bsrOptions.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_bsrString bsrString::create (
  int    inputLineNumber,
  string stringContents)
{
  bsrString* o =
    new bsrString (
      inputLineNumber, stringContents);
  assert (o!=0);
  return o;
}

bsrString::bsrString (
  int    inputLineNumber,
  string stringContents)
    : bsrLineElement (inputLineNumber)
{
  fStringContents = stringContents;

  fStringCellsList =
    bsrCellsList::create (inputLineNumber);

  for (int i = 1; i <= 7; i++) { // JMI
    fStringCellsList->
      appendCellKindToCellsList (kCellA);
  } // for
}

bsrString::~bsrString ()
{}

void bsrString::acceptIn (basevisitor* v)
{
#ifdef TRACE_OPTIONS
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrString::acceptIn ()" <<
      endl;
  }
#endif
      
  if (visitor<S_bsrString>*
    p =
      dynamic_cast<visitor<S_bsrString>*> (v)) {
        S_bsrString elem = this;
        
#ifdef TRACE_OPTIONS
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrString::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

int bsrString::fetchCellsNumber () const
{
  return fStringCellsList->fetchCellsNumber ();
}

void bsrString::acceptOut (basevisitor* v)
{
#ifdef TRACE_OPTIONS
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrString::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrString>*
    p =
      dynamic_cast<visitor<S_bsrString>*> (v)) {
        S_bsrString elem = this;
      
#ifdef TRACE_OPTIONS
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrString::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrString::browseData (basevisitor* v)
{}

string bsrString::asString () const
{
  stringstream s;

  s <<
    "String" <<
    ", stringContents: " << fStringContents <<
    ", stringCellsList: " << fStringCellsList <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void bsrString::print (ostream& os)
{
  os <<
    "String" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 16;

  os <<
    setw (fieldWidth) <<
    "stringContents" << " : " << fStringContents <<
    endl <<
    setw (fieldWidth) <<
    "stringCellsList" << " : " << fStringCellsList <<
    endl <<
    setw (fieldWidth) <<
    "spacesBefore" << " : " << fSpacesBefore <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrString& elt)
{
  elt->print (os);
  return os;
}


}
