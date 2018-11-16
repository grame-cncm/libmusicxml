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
  #include "bsrTraceOptions.h"
#endif

#include "bsrMutuallyDependent.h"

#include "bsrOptions.h"
#include "brailleOptions.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
S_bsrLine bsrLine::create (
  int inputLineNumber)
{
  bsrLine* o =
    new bsrLine (
      inputLineNumber);
  assert(o!=0);
  return o;
}

bsrLine::bsrLine (
  int inputLineNumber)
    : bsrElement (inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gBsrTraceOptions->fTraceLines) {
    gLogIOstream <<
      "Creating bsrLine '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      endl;
  }
#endif
}

bsrLine::~bsrLine ()
{}

void bsrLine::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrLine::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_bsrLine>*
    p =
      dynamic_cast<visitor<S_bsrLine>*> (v)) {
        S_bsrLine elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrLine::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrLine::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrLine::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrLine>*
    p =
      dynamic_cast<visitor<S_bsrLine>*> (v)) {
        S_bsrLine elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrLine::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void bsrLine::browseData (basevisitor* v)
{
  for (
    list<S_bsrElement>::const_iterator i = fLineElementsList.begin ();
    i != fLineElementsList.end ();
    i++ ) {
    // browse the element
    bsrBrowser<bsrElement> browser (v);
    browser.browse (*(*i));
  } // for

}

void bsrLine::print (ostream& os)
{
  os <<
    "Line" <<
    endl;
  
  gIndenter++;

  // print the line numbers
  const int fieldWidth = 18;
  
  os << left <<
    setw (fieldWidth) <<
    "printLineNumber" << " : " << fPrintLineNumber <<
    endl <<
    setw (fieldWidth) <<
    "brailleLineNumber" << " : " << fBrailleLineNumber <<
    endl;
  
  // print the line elements if any
  int lineElementsListSize = fLineElementsList.size ();
  
  if (lineElementsListSize || gBsrOptions->fDisplayBsrDetails) {
    os <<
      setw (fieldWidth) <<
      "fLineElementsList";
    if (lineElementsListSize) {
      os <<
        endl;
      gIndenter++;
  
      list<S_bsrElement>::const_iterator
        iBegin = fLineElementsList.begin (),
        iEnd   = fLineElementsList.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        os << endl;
      } // for
  
      os <<
        endl;
        
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

ostream& operator<< (ostream& os, const S_bsrLine& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_bsrLineElement bsrLineElement::create (
  int inputLineNumber)
{
  bsrLineElement* o =
    new bsrLineElement (
      inputLineNumber);
  assert(o!=0);
  return o;
}

bsrLineElement::bsrLineElement (
  int inputLineNumber)
    : bsrElement (inputLineNumber)
{}

bsrLineElement::~bsrLineElement ()
{}

/*
void bsrLineElement::setBarLineUplink (
S_bsrLine barlineUplink)
{
  fBarLineUplink = barlineUplink;
}

S_bsrLine bsrLineElement::getBarLineUplink () const
{
  return fBarLineUplink;
}
*/

void bsrLineElement::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrLineElement::acceptIn ()" <<
      endl;
  }
  
  if (visitor<S_bsrLineElement>*
    p =
      dynamic_cast<visitor<S_bsrLineElement>*> (v)) {
        S_bsrLineElement elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrLineElement::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrLineElement::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrLineElement::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrLineElement>*
    p =
      dynamic_cast<visitor<S_bsrLineElement>*> (v)) {
        S_bsrLineElement elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrLineElement::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void bsrLineElement::browseData (basevisitor* v)
{}


string bsrLineElement::asString () const
{
  // this is overriden all in actual elements
  return "??? bsrLineElement::asString () ???";
}

string bsrLineElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString ();
}

void bsrLineElement::print (ostream& os)
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_bsrLineElement& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_bsrPage bsrPage::create (
  int inputLineNumber)
{
  bsrPage* o =
    new bsrPage (
      inputLineNumber);
  assert(o!=0);
  return o;
}

bsrPage::bsrPage (
  int inputLineNumber)
    : bsrElement (inputLineNumber)
{
}

bsrPage::~bsrPage ()
{
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

void bsrPage::print (ostream& os)
{
  os <<
    "Page" <<
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
    endl;
  
  // print the page elements if any
  int pageElementsListSize = fPageElementsList.size ();
  
  if (pageElementsListSize || gBsrOptions->fDisplayBsrDetails) {
    os <<
      setw (fieldWidth) <<
      "fPageElementsList";
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
  
      os <<
        endl;
        
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
