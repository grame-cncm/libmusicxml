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

//#include "bsrLines.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
  #include "bsrTraceOptions.h"
#endif

#include "bsrPages.h"

#include "bsrOptions.h"
#include "brailleOptions.h"


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
  assert(o!=0);
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

#ifdef TRACE_OPTIONS
  if (gBsrTraceOptions->fTracePages) {
    gLogIOstream <<
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
#ifdef TRACE_OPTIONS
  if (gBsrTraceOptions->fTracePages) {
    gLogIOstream <<
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

void bsrPage::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrPage::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_bsrPage>*
    p =
      dynamic_cast<visitor<S_bsrPage>*> (v)) {
        S_bsrPage elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrPage::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrPage::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrPage::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrPage>*
    p =
      dynamic_cast<visitor<S_bsrPage>*> (v)) {
        S_bsrPage elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrPage::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void bsrPage::browseData (basevisitor* v)
{
  for (
    list<S_bsrElement>::const_iterator i = fPageElementsList.begin ();
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

void bsrPage::print (ostream& os)
{
  os <<
    "Page" <<
    ", line " << fInputLineNumber <<
    endl;
  
  gIndenter++;

  // print the page numbers
  const int fieldWidth = 18;
  
  os << left <<
    setw (fieldWidth) <<
    "printPageNumber" << " : " << fPrintPageNumber <<
    endl <<
    setw (fieldWidth) <<
    "braillePageNumber" << " : " << fBraillePageNumber <<
    endl <<
    setw (fieldWidth) <<
    "linesPerPage" << " : " << fLinesPerPage <<
    endl;
  os <<
    endl;
    
  // print the page elements if any
  int pageElementsListSize = fPageElementsList.size ();
  
  if (pageElementsListSize || gBsrOptions->fDisplayBsrDetails) {
    os <<
      setw (fieldWidth) <<
      "PageElementsList";
    if (pageElementsListSize) {
      os <<
        endl;
      gIndenter++;
  
      list<S_bsrElement>::const_iterator
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

//_______________________________________________________________________________
S_bsrPageElement bsrPageElement::create (
  int inputLineNumber)
{
  bsrPageElement* o =
    new bsrPageElement (
      inputLineNumber);
  assert(o!=0);
  return o;
}

bsrPageElement::bsrPageElement (
  int inputLineNumber)
    : bsrElement (inputLineNumber)
{}

bsrPageElement::~bsrPageElement ()
{}

void bsrPageElement::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrPageElement::acceptIn ()" <<
      endl;
  }
  
  if (visitor<S_bsrPageElement>*
    p =
      dynamic_cast<visitor<S_bsrPageElement>*> (v)) {
        S_bsrPageElement elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrPageElement::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrPageElement::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrPageElement::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrPageElement>*
    p =
      dynamic_cast<visitor<S_bsrPageElement>*> (v)) {
        S_bsrPageElement elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrPageElement::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void bsrPageElement::browseData (basevisitor* v)
{}


string bsrPageElement::asString () const
{
  // this is overriden all in actual elements
  return "??? bsrPageElement::asString () ???";
}

string bsrPageElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString ();
}

void bsrPageElement::print (ostream& os)
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_bsrPageElement& elt)
{
  elt->print (os);
  return os;
}


}
