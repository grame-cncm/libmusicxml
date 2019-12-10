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

#include "lpsrGeometry.h"

#include "generalOah.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "msrOah.h"
#include "lpsrOah.h"

#include "messagesHandling.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_lpsrGeometry lpsrGeometry::create (
  int           inputLineNumber,
  S_msrGeometry theMsrGeometry)
{
  lpsrGeometry* o =
    new lpsrGeometry (
      inputLineNumber,
      theMsrGeometry);
  assert(o!=0);
  return o;
}

lpsrGeometry::lpsrGeometry (
  int           inputLineNumber,
  S_msrGeometry theMsrGeometry)
    : lpsrElement (inputLineNumber)
{
  // sanity check
  msrAssert (
    theMsrGeometry != nullptr,
    "theMsrGeometry is null");

  fMsrGeometry = theMsrGeometry;
}

lpsrGeometry::~lpsrGeometry ()
{}

S_lpsrGeometry lpsrGeometry::createLpsrGeometryNewbornClone ()
{
  S_lpsrGeometry
    newbornClone =
      lpsrGeometry::create (
        fInputLineNumber,
        fMsrGeometry);

  return newbornClone;
}

void lpsrGeometry::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> lpsrGeometry::acceptIn ()" <<
      endl;
  }

  if (visitor<S_lpsrGeometry>*
    p =
      dynamic_cast<visitor<S_lpsrGeometry>*> (v)) {
        S_lpsrGeometry elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching lpsrGeometry::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrGeometry::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> lpsrGeometry::acceptOut ()" <<
      endl;
  }

  if (visitor<S_lpsrGeometry>*
    p =
      dynamic_cast<visitor<S_lpsrGeometry>*> (v)) {
        S_lpsrGeometry elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching lpsrGeometry::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrGeometry::browseData (basevisitor* v)
{}

void lpsrGeometry::print (ostream& os)  const
{
  os <<
    "Geometry" <<
    endl;

  gIndenter++;

  const int fieldWidth = 13;

  // MSR geometry
  os << left <<
    fMsrGeometry <<
    endl;

  // LilyPond geometry
  // ???

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsrGeometry& elt) {
  elt->print (os);
  return os;
}


}
