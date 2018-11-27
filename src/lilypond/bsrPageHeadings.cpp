/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iomanip>      // setw, setprecision, ...

#include "bsrPageHeadings.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "bsrOptions.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
S_bsrPageHeading bsrPageHeading::create (
  int inputLineNumber)
{
  bsrPageHeading* o =
    new bsrPageHeading (
      inputLineNumber);
  assert(o!=0);
  return o;
}

bsrPageHeading::bsrPageHeading (
  int inputLineNumber)
    : bsrElement (inputLineNumber)
{
  fPageHeadingBrailleSign =
    bsrBrailleSign::create  (inputLineNumber);
}

bsrPageHeading::~bsrPageHeading ()
{}

void bsrPageHeading::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrPageHeading::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_bsrPageHeading>*
    p =
      dynamic_cast<visitor<S_bsrPageHeading>*> (v)) {
        S_bsrPageHeading elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrPageHeading::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrPageHeading::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrPageHeading::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrPageHeading>*
    p =
      dynamic_cast<visitor<S_bsrPageHeading>*> (v)) {
        S_bsrPageHeading elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrPageHeading::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void bsrPageHeading::browseData (basevisitor* v)
{
}

void bsrPageHeading::print (ostream& os)
{
  os <<
    "PageHeading" <<
    endl;
  
  gIndenter++;

  const int fieldWidth = 16;

  os << left <<
    setw (fieldWidth) <<
    "foo" << " : " << "FOO" <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrPageHeading& elt)
{
  elt->print (os);
  return os;
}


}
