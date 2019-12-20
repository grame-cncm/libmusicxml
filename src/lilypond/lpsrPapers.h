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

#include "msrScaling.h"

#include "lpsrElements.h"

#include "messagesHandling.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
class lpsrPaper : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrPaper> create (
      int           inputLineNumber,
      S_msrScaling theMsrScaling);

    SMARTP<lpsrPaper> createPaperNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrPaper (
      int           inputLineNumber,
      S_msrScaling theMsrScaling);

    virtual ~lpsrPaper ();

  public:

    // set and get
    // ------------------------------------------------------

    // LPSR scaling
    void                  setMsrScaling (S_msrScaling value)
                              { fMsrScaling = value; }
    S_msrScaling         getMsrScaling () const
                            { return fMsrScaling; }

    // indents
    void                  setHorizontalShift (S_msrLength value)
                              { fHorizontalShift = value; }
    S_msrLength           getHorizontalShift () const
                              { return fHorizontalShift; }

    void                  setIndent (S_msrLength value)
                              { fIndent = value; }
    S_msrLength           getIndent () const
                              { return fIndent; }

    void                  setShortIndent (S_msrLength value)
                              { fShortIndent = value; }
    S_msrLength           getShortIndent () const
                              { return fShortIndent; }

    // counts
    void                  setPageCount (int value)
                              { fPageCount = value; }
    int                   getPageCount () const
                              { return fPageCount; }

    void                  setSystemCount (int value)
                              { fSystemCount = value; }
    int                   getSystemCount () const
                              { return fSystemCount; }

    // spaces
    void                  setMarkupSystemSpacingPadding (S_msrLength val)
                              { fMarkupSystemSpacingPadding = val; }
    S_msrLength           getMarkupSystemSpacingPadding () const
                              { return fMarkupSystemSpacingPadding; }

    void                  setBetweenSystemSpace (S_msrLength val)
                              { fBetweenSystemSpace = val; }
    S_msrLength           getBetweenSystemSpace () const
                              { return fBetweenSystemSpace; }

    void                  setPageTopSpace (S_msrLength val)
                              { fPageTopSpace = val; }
    S_msrLength           getPageTopSpace () const
                              { return fPageTopSpace; }


    // headers and footers
    void                  setOddHeaderMarkup (string value)
                              { fOddHeaderMarkup = value; }
    string                getOddHeaderMarkup () const
                              { return fOddHeaderMarkup; }

    void                  setEvenHeaderMarkup (string value)
                              { fEvenHeaderMarkup = value; }
    string                getEvenHeaderMarkup () const
                              { return fEvenHeaderMarkup; }

    void                  setOddFooterMarkup (string value)
                              { fOddFooterMarkup = value; }
    string                getOddFooterMarkup () const
                              { return fOddFooterMarkup; }

    void                  setEvenFooterMarkup (string value)
                              { fEvenFooterMarkup = value; }
    string                getEvenFooterMarkup () const
                              { return fEvenFooterMarkup; }

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

    virtual void          print (ostream& os) const;


  private:

    // fields
    // ------------------------------------------------------

    // LPSR scaling
    S_msrScaling         fMsrScaling;

    // indents
    S_msrLength           fHorizontalShift;
    S_msrLength           fIndent;
    S_msrLength           fShortIndent;

    // spaces
    S_msrLength           fMarkupSystemSpacingPadding;
    S_msrLength           fBetweenSystemSpace;
    S_msrLength           fPageTopSpace;

    // counts
    int                   fPageCount;
    int                   fSystemCount;

    // headers and footers
    string                fOddHeaderMarkup;
    string                fEvenHeaderMarkup;
    string                fOddFooterMarkup;
    string                fEvenFooterMarkup;
};
typedef SMARTP<lpsrPaper> S_lpsrPaper;
EXP ostream& operator<< (ostream& os, const S_lpsrPaper& elt);


}


#endif
