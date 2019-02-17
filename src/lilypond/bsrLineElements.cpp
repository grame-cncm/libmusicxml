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

#include "bsrMutualDependencies.h"

#include "bsrOptions.h"
#include "brailleOptions.h"


using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
/*
S_bsrLineElement bsrLineElement::create (
  int inputLineNumber)
{
  bsrLineElement* o =
    new bsrLineElement (
      inputLineNumber);
  assert(o!=0);
  return o;
}
*/

bsrLineElement::bsrLineElement (
  int inputLineNumber)
    : bsrElement (inputLineNumber)
{
  fSpacesBefore = 0;
}

bsrLineElement::~bsrLineElement ()
{}

void bsrLineElement::setBsrLineUplink (
S_bsrLine bsrLineUplink)
{
  fBsrLineUplink = bsrLineUplink;
}

S_bsrLine bsrLineElement::getBsrLineUplink () const
{
  return fBsrLineUplink;
}

void bsrLineElement::acceptIn (basevisitor* v)
{
#ifdef TRACE_OPTIONS
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrLineElement::acceptIn ()" <<
      endl;
  }
#endif
  
  if (visitor<S_bsrLineElement>*
    p =
      dynamic_cast<visitor<S_bsrLineElement>*> (v)) {
        S_bsrLineElement elem = this;
        
#ifdef TRACE_OPTIONS
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrLineElement::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrLineElement::acceptOut (basevisitor* v)
{
#ifdef TRACE_OPTIONS
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrLineElement::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrLineElement>*
    p =
      dynamic_cast<visitor<S_bsrLineElement>*> (v)) {
        S_bsrLineElement elem = this;
      
#ifdef TRACE_OPTIONS
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrLineElement::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrLineElement::browseData (basevisitor* v)
{}


string bsrLineElement::asString () const
{
  // this is overriden all in actual elements
  return "??? bsrLineElement::asString () ???";
}

string bsrLineElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString ();
}

void bsrLineElement::print (ostream& os)
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_bsrLineElement& elt)
{
  elt->print (os);
  return os;
}


}
