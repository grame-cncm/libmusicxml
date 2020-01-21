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
  int             inputLineNumber,
  S_msrScaling    scaling,
  S_msrPageLayout pageLayout)
{
  lpsrPaper* o =
    new lpsrPaper (
      inputLineNumber,
      scaling,
      pageLayout);
  assert(o!=0);
  return o;
}

lpsrPaper::lpsrPaper (
  int             inputLineNumber,
  S_msrScaling    scaling,
  S_msrPageLayout pageLayout)
    : lpsrElement (inputLineNumber)
{
  // sanity checks
  msrAssert (
    scaling != nullptr,
    "scaling is null");
  msrAssert (
    pageLayout != nullptr,
    "pageLayout is null");

  fScaling = scaling;
  fPageLayout = pageLayout;

  fPageCount = -1;
  fSystemCount = -1;
}

S_lpsrPaper lpsrPaper::createPaperNewbornClone ()
{
  S_lpsrPaper
    newbornClone =
      lpsrPaper::create (
        fInputLineNumber,
        fScaling,
        fPageLayout);

  // indents
  newbornClone->fHorizontalShift =
    fHorizontalShift;
  newbornClone->fIndent =
    fIndent;
  newbornClone->fShortIndent =
    fShortIndent;

  // spaces
  newbornClone->fMarkupSystemSpacingPadding =
    fMarkupSystemSpacingPadding;
  newbornClone->fBetweenSystemSpace =
    fBetweenSystemSpace;
  newbornClone->fPageTopSpace =
    fPageTopSpace;

  // counts
  newbornClone->fPageCount =
    fPageCount;
  newbornClone->fSystemCount =
    fSystemCount;

  // headers and footers
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

  // browse the scaling
  if (fScaling) {
    msrBrowser<msrScaling> browser (v);
    browser.browse (*fScaling);
  }

  // browse the page layout
  if (fPageLayout) {
    msrBrowser<msrPageLayout> browser (v);
    browser.browse (*fPageLayout);
  }

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

  // scaling

  os << left <<
    setw (fieldWidth) <<
    "scaling" << " : ";
  if (fScaling) {
    os << fScaling;
  }
  else {
    os << "none";
  }
  os << endl;

  // page layout

  os << left <<
    setw (fieldWidth) <<
    "pageLayout" << " : ";
  if (fPageLayout) {
    os << fPageLayout;
  }
  else {
    os << "none";
  }
  os << endl;

  // indents

  os << left <<
    setw (fieldWidth) <<
    "horizontalShift" << " : ";
  if (fHorizontalShift) {
    os << fHorizontalShift;
  }
  else {
    os << "none";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "indent" << " : ";
  if (fIndent) {
    os << fIndent;
  }
  else {
    os << "none";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "shortIndent" << " : ";
  if (fShortIndent) {
    os << fShortIndent;
  }
  else {
    os << "none";
  }
  os << endl;

  // spaces

  os << left <<
    setw (fieldWidth) <<
    "markupSystemSpacingPadding" << " : ";
  if (fMarkupSystemSpacingPadding) {
    os << fMarkupSystemSpacingPadding;
  }
  else {
    os << "none";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "betweenSystemSpace" << " : ";
  if (fBetweenSystemSpace) {
    os << fBetweenSystemSpace;
  }
  else {
    os << "none";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "pageTopSpace" << " : ";
  if (fPageTopSpace) {
    os << fPageTopSpace;
  }
  else {
    os << "none";
  }
  os << endl;

  // counts

  os << left <<
    setw (fieldWidth) <<
    "pageCount" << " : ";
  if (fPageCount) {
    os << fPageCount;
  }
  else {
    os << "none";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "systemCount" << " : ";
  if (fSystemCount) {
    os << fSystemCount;
  }
  else {
    os << "none";
  }
  os << endl;

  // headers and footers

  os << left <<
    setw (fieldWidth) <<
    "oddHeaderMarkup" << " : \"" <<
    fOddHeaderMarkup <<
    "\"" <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "evenHeaderMarkup" << " : " <<
    fEvenHeaderMarkup <<
    "\"" <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "oddFooterMarkup" << " : " <<
    fOddFooterMarkup <<
    "\"" <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "evenFooterMarkup" << " : " <<
    fEvenFooterMarkup <<
    "\"" <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsrPaper& pap) {
  pap->print (os);
  return os;
}


}
