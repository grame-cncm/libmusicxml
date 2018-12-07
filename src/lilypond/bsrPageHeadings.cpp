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

#include "bsrPageHeadings.h"

#include "bsrSpaces.h"
#include "bsrStrings.h"

#include "bsrOptions.h"


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
    : bsrElement (inputLineNumber)
{
  fPageHeadingTitle = pageHeadingTitle;
  
  fPageHeadingPagination = pageHeadingPagination;
  fPageHeadingNumber     = pageHeadingNumber;
  
  fPageHeadingCellsList =
    bsrCellsList::create  (inputLineNumber);
}

bsrPageHeading::~bsrPageHeading ()
{}

S_bsrCellsList bsrPageHeading::asCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  // append the pagination to result
  result->appendCellsListToCellsList (
    fPageHeadingPagination->asCellsList ());

  // append 3 spaces to result
  result->appendCellsListToCellsList (
    bsrSpaces::create (
      fInputLineNumber, 3)->
        getSpacesCellsList ());

  // append the title to result
  result->appendCellsListToCellsList (
    bsrString::create (
      fInputLineNumber, fPageHeadingTitle)->
        getStringCellsList ());

  // append 3 spaces to result
  result->appendCellsListToCellsList (
    bsrSpaces::create (
      fInputLineNumber, 3)->
        getSpacesCellsList ());

  // append the number to result
  result->appendCellsListToCellsList (
    bsrNumber::create (
      fInputLineNumber,
      fPageHeadingNumber,
      bsrNumber::kNumberSignIsNeededYes)->
        asCellsList ());

  return result;
}

void bsrPageHeading::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrPageHeading::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_bsrPageHeading>*
    p =
      dynamic_cast<visitor<S_bsrPageHeading>*> (v)) {
        S_bsrPageHeading elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrPageHeading::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrPageHeading::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrPageHeading::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrPageHeading>*
    p =
      dynamic_cast<visitor<S_bsrPageHeading>*> (v)) {
        S_bsrPageHeading elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrPageHeading::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void bsrPageHeading::browseData (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrScore::browseData ()" <<
      endl;
  }

  if (fPageHeadingPagination) {
    // browse the pagination
    msrBrowser<bsrPagination> browser (v);
    browser.browse (*fPageHeadingPagination);
  }

  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% <== bsrScore::browseData ()" <<
      endl;
  }
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

void bsrPageHeading::print (ostream& os)
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
