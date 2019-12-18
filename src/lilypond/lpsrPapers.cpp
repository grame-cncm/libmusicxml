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

#include "lpsrPapers.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "lpsrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_lpsrPaper lpsrPaper::create (
  int            inputLineNumber,
  S_lpsrGeometry theLpsrGeometry)
{
  lpsrPaper* o =
    new lpsrPaper (
      inputLineNumber,
      theLpsrGeometry);
  assert(o!=0);
  return o;
}

lpsrPaper::lpsrPaper (
  int            inputLineNumber,
  S_lpsrGeometry theLpsrGeometry)
    : lpsrElement (inputLineNumber)
{
  // sanity check
  msrAssert (
    theLpsrGeometry != nullptr,
    "theLpsrGeometry is null");

  fLpsrGeometry = theLpsrGeometry;
}

S_lpsrPaper lpsrPaper::createPaperNewbornClone ()
{
  S_lpsrPaper
    newbornClone =
      lpsrPaper::create (
        fInputLineNumber,
        fLpsrGeometry);

  newbornClone->fHorizontalShift =
    fHorizontalShift;
  newbornClone->fIndent =
    fIndent;
  newbornClone->fShortIndent =
    fShortIndent;

  newbornClone->fOddHeaderMarkup =
    fOddHeaderMarkup;
  newbornClone->fEvenHeaderMarkup =
    fEvenHeaderMarkup;
  newbornClone->fOddFooterMarkup =
    fOddFooterMarkup;
  newbornClone->fEvenFooterMarkup =
    fEvenFooterMarkup;

  return newbornClone;
}

lpsrPaper::~lpsrPaper ()
{}

void lpsrPaper::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    gLogOstream <<
      "% ==> lpsrPaper::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrPaper>*
    p =
      dynamic_cast<visitor<S_lpsrPaper>*> (v)) {
        S_lpsrPaper elem = this;

#ifdef TRACE_OAH
        if (gLpsrOah->fTraceLpsrVisitors) {
          gLogOstream <<
            "% ==> Launching lpsrPaper::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrPaper::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    gLogOstream <<
      "% ==> lpsrPaper::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrPaper>*
    p =
      dynamic_cast<visitor<S_lpsrPaper>*> (v)) {
        S_lpsrPaper elem = this;

#ifdef TRACE_OAH
        if (gLpsrOah->fTraceLpsrVisitors) {
          gLogOstream <<
            "% ==> Launching lpsrPaper::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrPaper::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    gLogOstream <<
      "% ==> lpsrPaper::browseData ()" <<
      endl;
  }
#endif

  // browse the LPSR geometry
  if (fLpsrGeometry) {
    msrBrowser<lpsrGeometry> browser (v);
    browser.browse (*fLpsrGeometry);
  }

/* JMI ???
  // browse the indents
  if (fHorizontalShift) {
    msrBrowser<msrLength> browser (v);
    browser.browse (*fHorizontalShift);
  }
  if (fIndent) {
    msrBrowser<msrLength> browser (v);
    browser.browse (*fIndent);
  }
  if (fShortIndent) {
    msrBrowser<msrLength> browser (v);
    browser.browse (*fShortIndent);
  }

  // browse the counts
  if (fPageCount) {
    msrBrowser<msrLength> browser (v);
    browser.browse (*fPageCount);
  }
  if (fSystemCount) {
    msrBrowser<msrLength> browser (v);
    browser.browse (*fSystemCount);
  }
  */

#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    gLogOstream <<
      "% <== lpsrPaper::browseData ()" <<
      endl;
  }
#endif
}

void lpsrPaper::print (ostream& os) const
{
  os <<
    "Paper" <<
    endl;

  gIndenter++;

  const int fieldWidth = 20;

  // indents
  if (fHorizontalShift) {
    os << left <<
      setw (fieldWidth) <<
      "horizontal-shift" << " : " <<
      fHorizontalShift <<
      endl;
  }
  if (fIndent) {
    os << left <<
      setw (fieldWidth) <<
      "indent" << " : " <<
      fIndent <<
      endl;
  }
  if (fShortIndent) {
    os << left <<
      setw (fieldWidth) <<
      "shortIndent" << " : " <<
      fShortIndent <<
      endl;
  }

  // counts
  if (fPageCount) {
    os << left <<
      setw (fieldWidth) <<
      "page-count" << " : " <<
      fPageCount <<
      endl;
  }
  if (fSystemCount) {
    os << left <<
      setw (fieldWidth) <<
      "system-count" << " : " <<
      fSystemCount <<
      endl;
  }

  // headers and footers
  if (fOddHeaderMarkup.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "oddHeaderMarkup" << " : " <<
      fOddHeaderMarkup <<
      endl;
  }

  if (fEvenHeaderMarkup.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "evenHeaderMarkup" << " : " <<
      fEvenHeaderMarkup <<
      endl;
  }

  if (fOddFooterMarkup.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "oddFooterMarkup" << " : " <<
      fOddFooterMarkup <<
      endl;
  }

  if (fEvenFooterMarkup.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "evenFooterMarkup" << " : " <<
      fEvenFooterMarkup <<
      endl;
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsrPaper& pap) {
  pap->print (os);
  return os;
}


}
