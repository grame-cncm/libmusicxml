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

#include "bsrPaginations.h"

#include "bsrNumbers.h"

#include "utilities.h"
#include "messagesHandling.h"

#include "oahOah.h"

#include "bsrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_bsrPagination bsrPagination::create (
  int inputLineNumber,
  int printPageNumber,
  int braillePageNumber)
{
  bsrPagination* o =
    new bsrPagination (
      inputLineNumber, printPageNumber, braillePageNumber);
  assert (o!=0);
  return o;
}

bsrPagination::bsrPagination (
  int inputLineNumber,
  int printPageNumber,
  int braillePageNumber)
    : bsrLineContentsElement (inputLineNumber)
{
  fPrintPageNumber   = printPageNumber;
  fBraillePageNumber = braillePageNumber;

  fPaginationCellsList = buildCellsList ();
}

bsrPagination::~bsrPagination ()
{}

S_bsrCellsList bsrPagination::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  // create the pagination sign
  S_bsrCellsList
    paginationSign =
      bsrCellsList::create (
        fInputLineNumber,
        kDots5, kDots25 );

  // append it to result
  result->appendCellsListToCellsList (
    paginationSign);

  // create the print page number
  S_bsrNumber
    printPageNumber =
      bsrNumber::create (
        fInputLineNumber,
        fPrintPageNumber,
        bsrNumber::kNumberSignIsNeededYes);

  // append it to result
  result->appendCellsListToCellsList (
    printPageNumber->fetchCellsList ());

  // create the braille page number
  S_bsrNumber
    braillePageNumber =
      bsrNumber::create (
        fInputLineNumber,
        fPrintPageNumber,
        bsrNumber::kNumberSignIsNeededYes);

  // append it to result
  result->appendCellsListToCellsList (
    braillePageNumber->fetchCellsList ());
  return result;
}

int bsrPagination::fetchCellsNumber () const
{
  return fPaginationCellsList->fetchCellsNumber ();
}

void bsrPagination::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrPagination::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrPagination>*
    p =
      dynamic_cast<visitor<S_bsrPagination>*> (v)) {
        S_bsrPagination elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrPagination::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrPagination::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrPagination::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrPagination>*
    p =
      dynamic_cast<visitor<S_bsrPagination>*> (v)) {
        S_bsrPagination elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrPagination::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrPagination::browseData (basevisitor* v)
{}

string bsrPagination::asString () const
{
  stringstream s;

  s <<
    "Pagination" <<
    ", printPageNumber: " << fPrintPageNumber <<
    ", braillePageNumber: " << fBraillePageNumber <<
    ", spacesCellsList: " << fPaginationCellsList <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string bsrPagination::asDebugString () const
{
  stringstream s;

  s <<
    "PGNT" <<
    ", printPageNumber: " << fPrintPageNumber <<
    ", braillePageNumber: " << fBraillePageNumber;

  return s.str ();
}

void bsrPagination::print (ostream& os) const
{
  os <<
    "Pagination" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const unsigned int fieldWidth = 18;

  os <<
    setw (fieldWidth) <<
    "printPageNumber" << " : " << fPrintPageNumber <<
    endl <<
    setw (fieldWidth) <<
    "braillePageNumber" << " : " << fBraillePageNumber <<
    endl <<
    setw (fieldWidth) <<
    "paginationCellsList" << " : " << fPaginationCellsList <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrPagination& elt)
{
  elt->print (os);
  return os;
}


}
