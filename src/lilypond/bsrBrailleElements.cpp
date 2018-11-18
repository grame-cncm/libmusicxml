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

#include "bsrBrailleElements.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "bsrTraceOptions.h"
#endif

#include "bsrOptions.h"


using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
S_bsrBrailleElement bsrBrailleElement::create (
  int inputLineNumber)
{
  bsrBrailleElement* o =
    new bsrBrailleElement (
      inputLineNumber);
  assert(o!=0);
  return o;
}

bsrBrailleElement::bsrBrailleElement (
  int inputLineNumber)
    : bsrElement (inputLineNumber)
{}

bsrBrailleElement::~bsrBrailleElement ()
{}

/*
void bsrBrailleElement::setBarLineUplink (
S_bsrBraille barlineUplink)
{
  fBarLineUplink = barlineUplink;
}

S_bsrBraille bsrBrailleElement::getBarLineUplink () const
{
  return fBarLineUplink;
}
*/

void bsrBrailleElement::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrBrailleElement::acceptIn ()" <<
      endl;
  }
  
  if (visitor<S_bsrBrailleElement>*
    p =
      dynamic_cast<visitor<S_bsrBrailleElement>*> (v)) {
        S_bsrBrailleElement elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrBrailleElement::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrBrailleElement::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrBrailleElement::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrBrailleElement>*
    p =
      dynamic_cast<visitor<S_bsrBrailleElement>*> (v)) {
        S_bsrBrailleElement elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrBrailleElement::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void bsrBrailleElement::browseData (basevisitor* v)
{}


string bsrBrailleElement::asString () const
{
  // this is overriden all in actual elements
  return "??? bsrBrailleElement::asString () ???";
}

string bsrBrailleElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString ();
}

void bsrBrailleElement::print (ostream& os)
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_bsrBrailleElement& elt)
{
  elt->print (os);
  return os;
}


}
