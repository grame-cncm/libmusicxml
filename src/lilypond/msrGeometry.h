/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrGeometry___
#define ___msrGeometry___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msrGeometry : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrGeometry> create (
      int inputLineNumber);

    SMARTP<msrGeometry> createMsrGeometryNewbornClone ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

    msrGeometry (
      int inputLineNumber);

    virtual ~msrGeometry ();

  public:

    // set and get
    // ------------------------------------------------------

    // relative to absolute lengths conversion
    void                  setMillimeters (float val)
                              { fMillimeters = val; }
    float                 getMillimeters () const
                              { return fMillimeters; }

    void                  setTenths (int val)
                              { fTenths = val; }
    float                 getTenths () const
                              { return float(fTenths); }

    // page size
    void                  setPaperWidth (S_msrLength val)
                              { fPaperWidth = val; }
    S_msrLength           getPaperWidth () const
                              { return fPaperWidth; }

    void                  setPaperHeight (S_msrLength val)
                              { fPaperHeight = val; }
    S_msrLength           getPaperHeight () const
                              { return fPaperHeight; }

    // margins
    void                  setTopMargin (S_msrMargin val)
                              { fTopMargin = val; }
    S_msrMargin           getTopMargin () const
                              { return fTopMargin; }

    void                  setBottomMargin (S_msrMargin val)
                              { fBottomMargin = val; }
    S_msrMargin           getBottomMargin () const
                              { return fBottomMargin; }

    void                  setLeftMargin (S_msrMargin val)
                              { fLeftMargin = val; }
    S_msrMargin           getLeftMargin () const
                              { return fLeftMargin; }

    void                  setRightMargin (S_msrMargin val)
                              { fRightMargin = val; }
    S_msrMargin           getRightMargin () const
                              { return fRightMargin; }

    // layout
    void                  setBetweenSystemSpace (S_msrLength val)
                              { fBetweenSystemSpace = val; }
    S_msrLength           getBetweenSystemSpace () const
                              { return fBetweenSystemSpace; }

    void                  setPageTopSpace (S_msrLength val)
                              { fPageTopSpace = val; }
    S_msrLength           getPageTopSpace () const
                              { return fPageTopSpace; }

  public:

    // services
    // ------------------------------------------------------

    float                 fetchGlobalStaffSize () const;

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

    // relative to absolute lengths conversion
    float                 fMillimeters;
    int                   fTenths;

    // page size
    S_msrLength           fPaperWidth;
    S_msrLength           fPaperHeight;

    // margins
    S_msrMargin           fTopMargin;
    S_msrMargin           fBottomMargin;
    S_msrMargin           fLeftMargin;
    S_msrMargin           fRightMargin;

    // layout
    S_msrLength           fBetweenSystemSpace;
    S_msrLength           fPageTopSpace;
};
typedef SMARTP<msrGeometry> S_msrGeometry;
EXP ostream& operator<< (ostream& os, const S_msrGeometry& elt);


} // namespace MusicXML2


#endif
