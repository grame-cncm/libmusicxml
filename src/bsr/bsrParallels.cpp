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

#include "bsrParallels.h"

#include "bsrOah.h"
#include "brailleOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_bsrParallel bsrParallel::create (
  int inputLineNumber)
{
  bsrParallel* o =
    new bsrParallel (
      inputLineNumber);
  assert(o!=0);
  return o;
}

bsrParallel::bsrParallel (
  int inputLineNumber)
    : bsrElement (inputLineNumber)
{
}

bsrParallel::~bsrParallel ()
{
#ifdef TRACE_OAH
  if (gBsrOah->fTraceParallels) {
    gLogOstream <<
      "Creating bsrParallel '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      endl;
  }
#endif
}

void bsrParallel::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrParallel::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrParallel>*
    p =
      dynamic_cast<visitor<S_bsrParallel>*> (v)) {
        S_bsrParallel elem = this;

#ifdef TRACE_OAH
        if (gBsrOah->fTraceBsrVisitors) {
          gLogOstream <<
            "% ==> Launching bsrParallel::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrParallel::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrParallel::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrParallel>*
    p =
      dynamic_cast<visitor<S_bsrParallel>*> (v)) {
        S_bsrParallel elem = this;

#ifdef TRACE_OAH
        if (gBsrOah->fTraceBsrVisitors) {
          gLogOstream <<
            "% ==> Launching bsrParallel::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrParallel::browseData (basevisitor* v)
{
  for (
    list<S_bsrElement>::const_iterator i = fParallelElementsList.begin ();
    i != fParallelElementsList.end ();
    i++ ) {
    // browse the element
    bsrBrowser<bsrElement> browser (v);
    browser.browse (*(*i));
  } // for

}

string bsrParallel::parallelLayoutKindAsString (
  bsrParallelLayoutKind parallelLayoutKind)
{
  string result;

  switch (parallelLayoutKind) {
    case bsrParallel::kParallelLayoutBarOverBarKind:
      result = "parallelLayoutBarOverBarKind";
      break;
    case bsrParallel::kParallelLayoutLineOverLineKind:
      result = "parallelLayoutLineOverLineKind";
      break;
  } // switch

  return result;
}

void bsrParallel::print (ostream& os) const
{
  os <<
    "Parallel" <<
    endl;

  gIndenter++;

  // print the parallel numbers
  const int fieldWidth = 18;

  os << left <<
    setw (fieldWidth) <<
    "printParallelNumber" << " : " << fPrintParallelNumber <<
    endl <<
    setw (fieldWidth) <<
    "brailleParallelNumber" << " : " << fBrailleParallelNumber <<
    endl;

  // print the parallel elements if any
  int parallelElementsListSize = fParallelElementsList.size ();

  if (parallelElementsListSize || gBsrOah->fDisplayBsrDetails) {
    os <<
      setw (fieldWidth) <<
      "fParallelElementsList";
    if (parallelElementsListSize) {
      os << endl;
      gIndenter++;

      list<S_bsrElement>::const_iterator
        iBegin = fParallelElementsList.begin (),
        iEnd   = fParallelElementsList.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        os << endl;
      } // for

      os << endl;

      gIndenter--;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrParallel& elt)
{
  elt->print (os);
  return os;
}


}
