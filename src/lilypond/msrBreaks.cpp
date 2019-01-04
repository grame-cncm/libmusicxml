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

#include "msrBreaks.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "msrOptions.h"


using namespace std;

namespace MusicXML2 {

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
    : msrElement (inputLineNumber)
{
  fNextBarNumber = nextBarNumber; 

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Creating a break before measure " << fNextBarNumber <<
      endl;
  }
#endif
}

msrLineBreak::~msrLineBreak ()
{}

string msrLineBreak::asString () const
{
  stringstream s;

  s <<
    "Line break" <<
    ", next bar number = \"" << fNextBarNumber << "\"";

  return s.str ();
}

void msrLineBreak::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrLineBreak::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrLineBreak>*
    p =
      dynamic_cast<visitor<S_msrLineBreak>*> (v)) {
        S_msrLineBreak elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrLineBreak::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrLineBreak::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrLineBreak::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrLineBreak>*
    p =
      dynamic_cast<visitor<S_msrLineBreak>*> (v)) {
        S_msrLineBreak elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrLineBreak::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrLineBreak::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrLineBreak& elt)
{
  elt->print (os);
  return os;
}

void msrLineBreak::print (ostream& os)
{
  os <<
    asString () <<
    endl;
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
    : msrElement (inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Creating a page break" <<
      endl;
  }
#endif
}

msrPageBreak::~msrPageBreak ()
{}

string msrPageBreak::asString () const
{
  stringstream s;

  s <<
    "Page break";

  return s.str ();
}

void msrPageBreak::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPageBreak::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrPageBreak>*
    p =
      dynamic_cast<visitor<S_msrPageBreak>*> (v)) {
        S_msrPageBreak elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPageBreak::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPageBreak::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPageBreak::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrPageBreak>*
    p =
      dynamic_cast<visitor<S_msrPageBreak>*> (v)) {
        S_msrPageBreak elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPageBreak::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrPageBreak::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrPageBreak& elt)
{
  elt->print (os);
  return os;
}

void msrPageBreak::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}


}
