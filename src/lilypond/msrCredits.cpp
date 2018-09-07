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
#include <iomanip>      // setw, setprecision, ...

#include "msrCredits.h"

#include "msrOptions.h"


using namespace std;

namespace MusicXML2 {

//______________________________________________________________________________
S_msrCreditWords msrCreditWords::create (
  int         inputLineNumber,
  string      creditWordsContents,
  string      creditWordsFontFamily,
  float       creditWordsFontSize,
  string      creditWordsFontWeight,
  string      creditWordsFontJustify,
  string      creditWordsFontHAlign,
  string      creditWordsFontVAlign,
  string      creditWordsFontXMLLanguage)
{
  msrCreditWords* o =
    new msrCreditWords (
      inputLineNumber,
      creditWordsContents,
      creditWordsFontFamily,
      creditWordsFontSize,
      creditWordsFontWeight,
      creditWordsFontJustify,
      creditWordsFontHAlign,
      creditWordsFontVAlign,
      creditWordsFontXMLLanguage);
  assert(o!=0);
  return o;
}

msrCreditWords::msrCreditWords (
  int         inputLineNumber,
  string      creditWordsContents,
  string      creditWordsFontFamily,
  float       creditWordsFontSize,
  string      creditWordsFontWeight,
  string      creditWordsFontJustify,
  string      creditWordsFontHAlign,
  string      creditWordsFontVAlign,
  string      creditWordsFontXMLLanguage)
    : msrElement (inputLineNumber)
{
  fCreditWordsContents        = creditWordsContents;

  fCreditWordsFontFamily      = creditWordsFontFamily;
  fCreditWordsFontSize        = creditWordsFontSize;
  fCreditWordsFontWeight      = creditWordsFontWeight;
  fCreditWordsFontJustify     = creditWordsFontJustify;
  fCreditWordsFontHAlign      = creditWordsFontHAlign;
  fCreditWordsFontVAlign      = creditWordsFontVAlign;
  fCreditWordsFontXMLLanguage = creditWordsFontXMLLanguage;
}

msrCreditWords::~msrCreditWords ()
{}

void msrCreditWords::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrCreditWords::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrCreditWords>*
    p =
      dynamic_cast<visitor<S_msrCreditWords>*> (v)) {
        S_msrCreditWords elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrCreditWords::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrCreditWords::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrCreditWords::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrCreditWords>*
    p =
      dynamic_cast<visitor<S_msrCreditWords>*> (v)) {
        S_msrCreditWords elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrCreditWords::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrCreditWords::browseData (basevisitor* v)
{}

void msrCreditWords::print (ostream& os)
{
  os <<
    "CreditWords" <<
    endl;

  gIndenter++;

  const int fieldWidth = 27;
  
  os <<
    setw (fieldWidth) <<
    "creditWordsContents" << " : \"" <<
    fCreditWordsContents <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "creditWordsFontFamily" << " : \"" <<
    fCreditWordsFontFamily <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "creditWordsFontSize" << " : " <<
    fCreditWordsFontSize <<
    endl <<
    setw (fieldWidth) <<
    "creditWordsFontWeight" << " : \"" <<
    fCreditWordsFontWeight <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "creditWordsFontJustify" << " : \"" <<
    fCreditWordsFontJustify <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "creditWordsFontHAlign" << " : \"" <<
    fCreditWordsFontHAlign <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "creditWordsFontVAlign" << " : \"" <<
    fCreditWordsFontVAlign <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "creditWordsFontXMLLanguage" << " : \"" <<
    fCreditWordsFontXMLLanguage <<
    "\"" <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrCreditWords& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrCredit msrCredit::create (
  int inputLineNumber,
  int creditPageNumber)
{
  msrCredit* o =
    new msrCredit (
      inputLineNumber, creditPageNumber);
  assert(o!=0);
  return o;
}

msrCredit::msrCredit (
  int inputLineNumber,
  int creditPageNumber)
    : msrElement (inputLineNumber)
{
  fCreditPageNumber = creditPageNumber;
}

msrCredit::~msrCredit ()
{}

void msrCredit::appendCreditWordsToCredit (
  S_msrCreditWords creditWords)
{
  fCreditWordsList.push_back (creditWords);
}

void msrCredit::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrCredit::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrCredit>*
    p =
      dynamic_cast<visitor<S_msrCredit>*> (v)) {
        S_msrCredit elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrCredit::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrCredit::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrCredit::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrCredit>*
    p =
      dynamic_cast<visitor<S_msrCredit>*> (v)) {
        S_msrCredit elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrCredit::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrCredit::browseData (basevisitor* v)
{
  int n1 = fCreditWordsList.size ();
  
  for (int i = 0; i < n1; i++ ) {
    // browse the credit words
    msrBrowser<msrCreditWords> browser (v);
    browser.browse (*fCreditWordsList [i]);
  } // for
}

string msrCredit::asString () const
{
  stringstream s;

  s <<
    "Credit" <<
    ", fCreditPageNumber" << " = " << fCreditPageNumber << ", ";

  if (fCreditWordsList.size ()) {
    s << "[";
    
    vector<S_msrCreditWords>::const_iterator
      iBegin = fCreditWordsList.begin (),
      iEnd   = fCreditWordsList.end (),
      i      = iBegin;
  
    for ( ; ; ) {
      s << "\"" << (*i)->getCreditWordsContents () << "\"";
      if (++i == iEnd) break;
      s << ", ";
    } // for

    s << "]";
  }
  
  else
    s << "no credit words";
    
  return s.str ();
}

void msrCredit::print (ostream& os)
{
  os <<
    "Credit" <<
    endl;

  gIndenter++;
  
  os <<
    "fCreditPageNumber" << " : " << fCreditPageNumber <<
    endl;

  if (fCreditWordsList.size ()) {
    vector<S_msrCreditWords>::const_iterator
      iBegin = fCreditWordsList.begin (),
      iEnd   = fCreditWordsList.end (),
      i      = iBegin;
  
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for
  }
  
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrCredit& elt)
{
  elt->print (os);
  return os;
}


}
