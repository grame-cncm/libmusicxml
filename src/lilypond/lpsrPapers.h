/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsrPapers___
#define ___lpsrPapers___

#include <string>
#include <iostream>

#include "lpsrElements.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
class lpsrPaper : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrPaper> create (
      int inputLineNumber);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrPaper (
      int inputLineNumber);
      
    virtual ~lpsrPaper ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // page width, height, margins and indents

    void    setPaperWidth         (float val) { fPaperWidth = val; }
    void    setPaperHeight        (float val) { fPaperHeight = val; }
    
    void    setTopMargin          (float val) { fTopMargin = val; }
    void    setBottomMargin       (float val) { fBottomMargin = val; }
    void    setLeftMargin         (float val) { fLeftMargin = val; }
    void    setRightMargin        (float val) { fRightMargin = val; }
    
    void    setIndent             (float val);
    void    setShortIndent        (float val);

    float   getPaperWidth         () const    { return fPaperWidth; }
    float   getPaperHeight        () const    { return fPaperHeight; }
    float   getTopMargin          () const    { return fTopMargin; }
    float   getBottomMargin       () const    { return fBottomMargin; }
    float   getLeftMargin         () const    { return fLeftMargin; }
    float   getRightMargin        () const    { return fRightMargin; }
    
    float   getIndent             () const    { return fIndent; }
    float   getShortIndent        () const    { return fShortIndent; }

    // spaces

    void    setBetweenSystemSpace (float val) { fBetweenSystemSpace = val; }
    void    setPageTopSpace       (float val) { fPageTopSpace = val; }

    float   getBetweenSystemSpace () const    { return fBetweenSystemSpace; }
    float   getPageTopSpace       () const    { return fPageTopSpace; }

    // headers and footers

    void    setOddHeaderMarkup   (string val) { fOddHeaderMarkup = val; }
    void    setEvenHeaderMarkup  (string val) { fEvenHeaderMarkup = val; }
    void    setOddFooterMarkup   (string val) { fOddFooterMarkup = val; }
    void    setEvenFooterMarkup  (string val) { fEvenFooterMarkup = val; }

    string  getOddHeaderMarkup    () const    { return fOddHeaderMarkup; }
    string  getEvenHeaderMarkup   () const    { return fEvenHeaderMarkup; }
    string  getOddFooterMarkup    () const    { return fOddFooterMarkup; }
    string  getEvenFooterMarkup   () const    { return fEvenFooterMarkup; }

    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);


  private:

    // fields
    // ------------------------------------------------------

    // page width, height, margins and indents (centimeters)
    
    float             fPaperWidth;
    float             fPaperHeight;
    
    float             fTopMargin;
    float             fBottomMargin;
    float             fLeftMargin;
    float             fRightMargin;
    
    float             fIndent;
    float             fShortIndent;

    // spaces (centimeters)
    
    float             fBetweenSystemSpace;
    float             fPageTopSpace;

    // headers and footers
    
    string            fOddHeaderMarkup;
    string            fEvenHeaderMarkup;
    string            fOddFooterMarkup;
    string            fEvenFooterMarkup;
};
typedef SMARTP<lpsrPaper> S_lpsrPaper;
EXP ostream& operator<< (ostream& os, const S_lpsrPaper& elt);


}


#endif
