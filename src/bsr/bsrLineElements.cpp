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

#include "utilities.h"

#include "bsrMutualDependencies.h"

#include "bsrOah.h"
#include "brailleOah.h"


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

void bsrLineElement::setBsrLineUpLink (
S_bsrLine bsrLineUpLink)
{
  fBsrLineUpLink = bsrLineUpLink;
}

S_bsrLine bsrLineElement::getBsrLineUpLink () const
{
  return fBsrLineUpLink;
}

void bsrLineElement::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrLineElement::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrLineElement>*
    p =
      dynamic_cast<visitor<S_bsrLineElement>*> (v)) {
        S_bsrLineElement elem = this;

#ifdef TRACE_OAH
        if (gGlobalBsrOah->fTraceBsrVisitors) {
          gLogOstream <<
            "% ==> Launching bsrLineElement::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrLineElement::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrLineElement::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrLineElement>*
    p =
      dynamic_cast<visitor<S_bsrLineElement>*> (v)) {
        S_bsrLineElement elem = this;

#ifdef TRACE_OAH
        if (gGlobalBsrOah->fTraceBsrVisitors) {
          gLogOstream <<
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

/* JMI
string bsrLineElement::asDebugString () const
{
  // this can be overriden in actual elements
  return asShortString ();
}
*/

void bsrLineElement::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_bsrLineElement& elt)
{
  elt->print (os);
  return os;
}


}
