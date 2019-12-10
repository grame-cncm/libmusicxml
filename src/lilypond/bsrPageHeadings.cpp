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

#include "bsrOah.h"
#include "brailleOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_bsrPageHeading bsrPageHeading::create (
  int             inputLineNumber,
  string          pageHeadingTitle,
  S_bsrPagination pageHeadingPagination,
  int             pageHeadingNumber)
{
  bsrPageHeading* o =
    new bsrPageHeading (
      inputLineNumber,
      pageHeadingTitle, pageHeadingPagination, pageHeadingNumber);
  assert(o!=0);
  return o;
}

bsrPageHeading::bsrPageHeading (
  int             inputLineNumber,
  string          pageHeadingTitle,
  S_bsrPagination pageHeadingPagination,
  int             pageHeadingNumber)
    : bsrLine (
        inputLineNumber,
        0, // JMI ???
        gBrailleOah->fCellsPerLine)
{
  fPageHeadingTitle = pageHeadingTitle;

  fPageHeadingPagination = pageHeadingPagination;
  fPageHeadingNumber     = pageHeadingNumber;
}

bsrPageHeading::~bsrPageHeading ()
{}

S_bsrCellsList bsrPageHeading::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  // append the pagination to result
  result->appendCellsListToCellsList (
    fPageHeadingPagination->fetchCellsList ());

  // append 3 spaces to result
  result->appendCellsListToCellsList (
    bsrSpaces::create (
      fInputLineNumber, 3)->
        fetchCellsList ());

  // append the title to result
  result->appendCellsListToCellsList (
    bsrWords::create (
      fInputLineNumber, fPageHeadingTitle)->
        fetchCellsList ());

  // append 3 spaces to result
  result->appendCellsListToCellsList (
    bsrSpaces::create (
      fInputLineNumber, 3)->
        fetchCellsList ());

  // append the number to result
  result->appendCellsListToCellsList (
    bsrNumber::create (
      fInputLineNumber,
      fPageHeadingNumber,
      bsrNumber::kNumberSignIsNeededYes)->
        fetchCellsList ());

  // append 1 space to result
  result->appendCellsListToCellsList (
    bsrSpaces::create (
      fInputLineNumber, 1)->
        fetchCellsList ());

  return result;
}

void bsrPageHeading::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrPageHeading::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrPageHeading>*
    p =
      dynamic_cast<visitor<S_bsrPageHeading>*> (v)) {
        S_bsrPageHeading elem = this;

#ifdef TRACE_OAH
        if (gBsrOah->fTraceBsrVisitors) {
          gLogOstream <<
            "% ==> Launching bsrPageHeading::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrPageHeading::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrPageHeading::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrPageHeading>*
    p =
      dynamic_cast<visitor<S_bsrPageHeading>*> (v)) {
        S_bsrPageHeading elem = this;

#ifdef TRACE_OAH
        if (gBsrOah->fTraceBsrVisitors) {
          gLogOstream <<
            "% ==> Launching bsrPageHeading::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrPageHeading::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrScore::browseData ()" <<
      endl;
  }
#endif

  if (fPageHeadingPagination) {
    // browse the pagination
    msrBrowser<bsrPagination> browser (v);
    browser.browse (*fPageHeadingPagination);
  }

#ifdef TRACE_OAH
  if (gBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% <== bsrScore::browseData ()" <<
      endl;
  }
#endif
}

string bsrPageHeading::asString () const
{
  stringstream s;

  s <<
    "PageHeading" <<
    ", pageHeadingTitle: \"" << fPageHeadingTitle << "\"" <<
      ", pageHeadingPagination: ";

  if (fPageHeadingPagination) {
    s <<
      fPageHeadingPagination->asShortString ();
  }
  else {
    s <<
      "none";
  }

  s <<
    ", pageHeadingNumber: " << fPageHeadingNumber <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void bsrPageHeading::print (ostream& os) const
{
  os <<
    "PageHeading" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 22;

  os << left <<
    setw (fieldWidth) <<
    "pageHeadingTitle" << " : \"" << fPageHeadingTitle << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "pageHeadingPagination" <<
    endl;

  if (fPageHeadingPagination) {
    gIndenter++;

    os <<
      fPageHeadingPagination;

    gIndenter--;
  }
  else {
    os <<
      " : " << "none" <<
    endl;
  }

  os << left <<
    setw (fieldWidth) <<
    "pageHeadingNumber" << " : " << fPageHeadingNumber <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrPageHeading& elt)
{
  elt->print (os);
  return os;
}


}
