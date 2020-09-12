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

#include "utilities.h"

#include "msrBreaks.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrLineBreak msrLineBreak::create (
  int    inputLineNumber,
  string nextBarNumber)
{
  msrLineBreak* o =
    new msrLineBreak (
      inputLineNumber, nextBarNumber);
  assert(o!=0);
  return o;
}

msrLineBreak::msrLineBreak (
  int    inputLineNumber,
  string nextBarNumber)
    : msrMeasureElement (inputLineNumber)
{
  fNextBarNumber = nextBarNumber;

#ifdef TRACE_OAH
  if (gGlobalTraceOah->fTraceMeasures) {
    gLogOstream <<
      "Creating a break before measure " << fNextBarNumber <<
      endl;
  }
#endif
}

msrLineBreak::~msrLineBreak ()
{}

void msrLineBreak::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrLineBreak::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrLineBreak>*
    p =
      dynamic_cast<visitor<S_msrLineBreak>*> (v)) {
        S_msrLineBreak elem = this;

        if (gGlobalMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrLineBreak::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrLineBreak::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrLineBreak::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrLineBreak>*
    p =
      dynamic_cast<visitor<S_msrLineBreak>*> (v)) {
        S_msrLineBreak elem = this;

        if (gGlobalMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrLineBreak::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrLineBreak::browseData (basevisitor* v)
{}

string msrLineBreak::asString () const
{
  stringstream s;

  s <<
    "Line break" <<
    ", nextBarNumber = \"" << fNextBarNumber << "\"";

  return s.str ();
}

void msrLineBreak::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrLineBreak& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrPageBreak msrPageBreak::create (
  int inputLineNumber)
{
  msrPageBreak* o =
    new msrPageBreak (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrPageBreak::msrPageBreak (
  int inputLineNumber)
    : msrMeasureElement (inputLineNumber)
{
#ifdef TRACE_OAH
  if (gGlobalTraceOah->fTraceMeasures) {
    gLogOstream <<
      "Creating a page break" <<
      endl;
  }
#endif
}

msrPageBreak::~msrPageBreak ()
{}

void msrPageBreak::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrPageBreak::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrPageBreak>*
    p =
      dynamic_cast<visitor<S_msrPageBreak>*> (v)) {
        S_msrPageBreak elem = this;

        if (gGlobalMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrPageBreak::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPageBreak::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrPageBreak::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrPageBreak>*
    p =
      dynamic_cast<visitor<S_msrPageBreak>*> (v)) {
        S_msrPageBreak elem = this;

        if (gGlobalMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrPageBreak::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrPageBreak::browseData (basevisitor* v)
{}

string msrPageBreak::asString () const
{
  stringstream s;

  s <<
    "Page break";

  return s.str ();
}

void msrPageBreak::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrPageBreak& elt)
{
  elt->print (os);
  return os;
}


}
