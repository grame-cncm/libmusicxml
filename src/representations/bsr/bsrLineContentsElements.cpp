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

#include "bsrLineContentsElements.h"

#include "oahOah.h"

#include "bsrOah.h"
#include "brailleOah.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
/* this class is purely virtual
S_bsrLineContentsElement bsrLineContentsElement::create (
  int inputLineNumber)
{
  bsrLineContentsElement* o =
    new bsrLineContentsElement (
      inputLineNumber);
  assert (o!=0);
  return o;
}
*/

bsrLineContentsElement::bsrLineContentsElement (
  int inputLineNumber)
    : bsrElement (inputLineNumber)
{
  fSpacesBefore = 0;
}

bsrLineContentsElement::~bsrLineContentsElement ()
{}

/*
void bsrLineContentsElement::setBsrLineUpLink (
  S_bsrLine bsrLineUpLink)
{
  fBsrLineUpLink = bsrLineUpLink;
}

S_bsrLine bsrLineContentsElement::getBsrLineUpLink () const
{
  return fBsrLineUpLink;
}
*/

void bsrLineContentsElement::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrLineContentsElement::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrLineContentsElement>*
    p =
      dynamic_cast<visitor<S_bsrLineContentsElement>*> (v)) {
        S_bsrLineContentsElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrLineContentsElement::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrLineContentsElement::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrLineContentsElement::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrLineContentsElement>*
    p =
      dynamic_cast<visitor<S_bsrLineContentsElement>*> (v)) {
        S_bsrLineContentsElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrLineContentsElement::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrLineContentsElement::browseData (basevisitor* v)
{}


string bsrLineContentsElement::asString () const
{
  // this is overriden all in actual elements
  return "??? bsrLineContentsElement::asString () ???";
}

string bsrLineContentsElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString ();
}

/* JMI
string bsrLineContentsElement::asDebugString () const
{
  // this can be overriden in actual elements
  return asShortString ();
}
*/

void bsrLineContentsElement::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_bsrLineContentsElement& elt)
{
  elt->print (os);
  return os;
}


}
