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

#include "bsrPages.h"

#include "oahOah.h"

#include "bsrOah.h"
#include "brailleOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_bsrPage bsrPage::create (
  int inputLineNumber,
  int printPageNumber,
  int linesPerPage)
{
  bsrPage* o =
    new bsrPage (
      inputLineNumber, printPageNumber, linesPerPage);
  assert (o!=0);
  return o;
}

bsrPage::bsrPage (
  int inputLineNumber,
  int printPageNumber,
  int linesPerPage)
    : bsrElement (inputLineNumber)
{
  fPrintPageNumber   = printPageNumber;
  fBraillePageNumber = fPrintPageNumber; // this will be set by BSR finalizer

  fLinesPerPage = linesPerPage;

#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTracePages ()) {
    gLogStream <<
      "Creating bsrPage '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      endl;
  }
#endif
}

bsrPage::~bsrPage ()
{}

S_bsrPage bsrPage::createPageNewbornClone ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTracePages ()) {
    gLogStream <<
      "Creating a newborn clone of page " <<
      asString () <<
      endl;
  }
#endif

  S_bsrPage
    newbornClone =
      bsrPage::create (
        fInputLineNumber,
        fPrintPageNumber,
        fLinesPerPage);

  // braille line number
  newbornClone->fBraillePageNumber =
    fBraillePageNumber;

  return newbornClone;
}

int bsrPage::fetchLineContentsNumber () const
{
  int result = 0;

  for (
    list<S_bsrPageElement>::const_iterator i = fPageElementsList.begin ();
    i != fPageElementsList.end ();
    i++
  ) {
    result += (*i)->fetchLineContentsNumber ();
  } // for

  return result;
}

void bsrPage::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrPage::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrPage>*
    p =
      dynamic_cast<visitor<S_bsrPage>*> (v)) {
        S_bsrPage elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrPage::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrPage::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrPage::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrPage>*
    p =
      dynamic_cast<visitor<S_bsrPage>*> (v)) {
        S_bsrPage elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrPage::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrPage::browseData (basevisitor* v)
{
  for (
    list<S_bsrPageElement>::const_iterator i = fPageElementsList.begin ();
    i != fPageElementsList.end ();
    i++ ) {
    // browse the element
    bsrBrowser<bsrElement> browser (v);
    browser.browse (*(*i));
  } // for
}

string bsrPage::asString () const
{
  stringstream s;

  s <<
    "Page" <<
    ", printPageNumber" << " : " << fPrintPageNumber <<
    ", braillePageNumber" << " : " << fBraillePageNumber <<
    ", linesPerPage" << " : " << fLinesPerPage <<
    ", elements: " << fPageElementsList.size () <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void bsrPage::print (ostream& os) const
{
  os <<
    "Page" <<
    ", printPageNumber: " << fPrintPageNumber <<
    ", lineContentsNumber: " << fetchLineContentsNumber () <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  // print the page numbers
  const unsigned int fieldWidth = 18;

  os << left <<
  /* JMI
    setw (fieldWidth) <<
    "printPageNumber" << " : " << fPrintPageNumber <<
    endl <<
    */
    setw (fieldWidth) <<
    "braillePageNumber" << " : " << fBraillePageNumber <<
    endl <<
    setw (fieldWidth) <<
    "linesPerPage" << " : " << fLinesPerPage <<
    endl;
  os << endl;

  // print the page elements if any
  unsigned int pageElementsListSize = fPageElementsList.size ();

  if (pageElementsListSize || gGlobalBsrOahGroup->getDisplayBsrDetails ()) {
    os <<
//      setw (fieldWidth) <<
      "PageElementsList" <<
    ", " <<
    singularOrPlural (
      pageElementsListSize, "pageElement", "pageElements");
    if (pageElementsListSize) {
      os << endl;
      gIndenter++;

      list<S_bsrPageElement>::const_iterator
        iBegin = fPageElementsList.begin (),
        iEnd   = fPageElementsList.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        os << endl;
      } // for

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

ostream& operator<< (ostream& os, const S_bsrPage& elt)
{
  elt->print (os);
  return os;
}


}
