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

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "lpsrOptions.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
S_lpsrPaper lpsrPaper::create (
  int inputLineNumber)
{
  lpsrPaper* o =
    new lpsrPaper (
      inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrPaper::lpsrPaper (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
  fPaperWidth   = -1.0;
  fPaperHeight  = -1.0;
  
  fTopMargin    = -1.0;
  fBottomMargin = -1.0;
  fLeftMargin   = -1.0;
  fRightMargin  = -1.0;
    
  fIndent       = -1.0;
  fShortIndent  = -1.0;

  fBetweenSystemSpace = -1.0;
  fPageTopSpace = -1.0;
}

lpsrPaper::~lpsrPaper ()
{}

void lpsrPaper::setIndent (float val)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceGeometry) {
    gLogIOstream <<
      "Setting paper indent to " << val <<
      endl;
  }
#endif

  fIndent = val;
}

void lpsrPaper::setShortIndent (float val)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceGeometry) {
    gLogIOstream <<
      "Setting paper short indent to " << val <<
      endl;
  }
#endif

  fShortIndent = val;
}

void lpsrPaper::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrPaper::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_lpsrPaper>*
    p =
      dynamic_cast<visitor<S_lpsrPaper>*> (v)) {
        S_lpsrPaper elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrPaper::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrPaper::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrPaper::acceptOut ()" <<
      endl;
  }

  if (visitor<S_lpsrPaper>*
    p =
      dynamic_cast<visitor<S_lpsrPaper>*> (v)) {
        S_lpsrPaper elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrPaper::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrPaper::browseData (basevisitor* v)
{}

void lpsrPaper::print (ostream& os) {
  os <<
    "Paper" <<
    endl;

  bool emptyPaper = true;

  gIndenter++;

  const int fieldWidth = 20;
  
  // page width, height and margins

  if (fPaperWidth > 0) {
    os << left <<
      setw (fieldWidth) <<
      "paper-width" << " : " <<
      setprecision (2) << fPaperWidth << "\\cm" <<
      endl;
      
    emptyPaper = false;
  }
  
  if (fPaperHeight > 0) {
    os << left <<
      setw (fieldWidth) <<
      "paper-height" << " : " <<
      setprecision (2) << fPaperHeight << "\\cm" <<
      endl;
      
    emptyPaper = false;
  }
  
  if (fTopMargin > 0) {
    os << left <<
      setw (fieldWidth) <<
      "top-margin" << " : " <<
      setprecision (2) << fTopMargin << "\\cm" <<
      endl;
      
    emptyPaper = false;
  }
  
  if (fBottomMargin > 0) {
    os << left <<
      setw (fieldWidth) <<
      "bottom-margin" << " : " <<
      setprecision (2) << fBottomMargin << "\\cm" <<
      endl;
      
    emptyPaper = false;
  }
  
  if (fLeftMargin > 0) {
    os << left <<
      setw (fieldWidth) <<
      "left-margin" << " : " <<
      setprecision (2) << fLeftMargin << "\\cm" <<
      endl;
      
    emptyPaper = false;
  }
  
  if (fRightMargin > 0) {
    os << left <<
      setw (fieldWidth) <<
      "right-margin" << " : " <<
      setprecision (2) << fRightMargin << "\\cm" <<
      endl;
      
    emptyPaper = false;
  }

  if (fIndent > 0) {
    os << left <<
      setw (fieldWidth) <<
      "indent" << " : " <<
      setprecision (2) << fIndent << "\\cm" <<
      endl;
      
    emptyPaper = false;
  }

  if (fShortIndent > 0) {
    os << left <<
      setw (fieldWidth) <<
      "short-indent" << " : " <<
      setprecision (2) << fShortIndent << "\\cm" <<
      endl;
      
    emptyPaper = false;
  }

  // spaces

  if (fBetweenSystemSpace > 0) {
    os << left <<
      setw (fieldWidth) <<
      "between-system-space" << " : " <<
      setprecision (2) << fBetweenSystemSpace << "\\cm" <<
      endl;
      
    emptyPaper = false;
  }

  if (fPageTopSpace > 0) {
    os << left <<
      setw (fieldWidth) <<
      "page-top-space" << " : " <<
      setprecision (2) << fPageTopSpace << "\\cm" <<
      endl;
      
    emptyPaper = false;
  }

  // headers and footers

  if (fOddHeaderMarkup.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "oddHeaderMarkup" << " : " <<
      fOddHeaderMarkup <<
      endl;
      
    emptyPaper = false;
  }

  if (fEvenHeaderMarkup.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "evenHeaderMarkup" << " : " <<
      fEvenHeaderMarkup <<
      endl;
      
    emptyPaper = false;
  }

  if (fOddFooterMarkup.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "oddFooterMarkup" << " : " <<
      fOddFooterMarkup <<
      endl;
      
    emptyPaper = false;
  }

  if (fEvenFooterMarkup.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "evenFooterMarkup" << " : " <<
      fEvenFooterMarkup <<
      endl;
      
    emptyPaper = false;
  }

  // otherwise
  
  if (emptyPaper) {
    os <<
      " " << "nothing specified" <<
      endl <<
      endl;
  }
  
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsrPaper& pap) {
  pap->print (os);
  return os;
}


}
