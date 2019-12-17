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

#include "lpsrGeometry.h"

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
      int            inputLineNumber,
      S_lpsrGeometry theLpsrGeometry);

    SMARTP<lpsrPaper> createPaperNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrPaper (
      int            inputLineNumber,
      S_lpsrGeometry theLpsrGeometry);

    virtual ~lpsrPaper ();

  public:

    // set and get
    // ------------------------------------------------------

    // LPSR geometry
    void                  setLpsrGeometry (S_lpsrGeometry value)
                              { fLpsrGeometry = value; }
    S_lpsrGeometry        getLpsrGeometry () const
                            { return fLpsrGeometry; }

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
    void                  setPageCount (S_msrLength value)
                              { fPageCount = value; }
    S_msrLength           getPageCount () const
                              { return fPageCount; }

    void                  setSystemCount (S_msrLength value)
                              { fSystemCount = value; }
    S_msrLength           getSystemCount () const
                              { return fSystemCount; }

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

    // LPSR geometry
    S_lpsrGeometry        fLpsrGeometry;

    // indents
    S_msrLength           fHorizontalShift;
    S_msrLength           fIndent;
    S_msrLength           fShortIndent;

    // counts
    S_msrLength           fPageCount;
    S_msrLength           fSystemCount;

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
