/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrPageGeometry___
#define ___msrPageGeometry___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msrPageGeometry : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPageGeometry> create (
      int inputLineNumber);

    SMARTP<msrPageGeometry> createGeometryNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrPageGeometry (
      int inputLineNumber);

    virtual ~msrPageGeometry ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMillimeters (float val)
                              { fMillimeters = val; }
    float                 getMillimeters () const
                              { return fMillimeters; }

    void                  setTenths (int val)
                              { fTenths = val; }
    float                 getTenths () const
                              { return float(fTenths); }

    void                  setPaperWidth (float val)
                              { fPaperWidth = val; }
    float                 getPaperWidth () const
                              { return fPaperWidth; }

    void                  setPaperHeight (float val)
                              { fPaperHeight = val; }
    float                 getPaperHeight () const
                              { return fPaperHeight; }

    void                  setTopMargin (float val)
                              { fTopMargin = val; }
    float                 getTopMargin () const
                              { return fTopMargin; }

    void                  setBottomMargin (float val)
                              { fBottomMargin = val; }
    float                 getBottomMargin () const
                              { return fBottomMargin; }

    void                  setLeftMargin (float val)
                              { fLeftMargin = val; }
    float                 getLeftMargin () const
                              { return fLeftMargin; }

    void                  setRightMargin (float val)
                              { fRightMargin = val; }
    float                 getRightMargin () const
                              { return fRightMargin; }

    void                  setBetweenSystemSpace (float val)
                              { fBetweenSystemSpace = val; }
    float                 getBetweenSystemSpace () const
                              { return fBetweenSystemSpace; }

    void                  setPageTopSpace (float val)
                              { fPageTopSpace = val; }
    float                 getPageTopSpace () const
                              { return fPageTopSpace; }

    // services
    // ------------------------------------------------------

    float                 fetchGlobalStaffSize () const;

  public:

    // visitors
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

    // page height, margins and the like are in centimeters
    float                 fPaperWidth;
    float                 fPaperHeight;

    float                 fTopMargin;
    float                 fBottomMargin;
    float                 fLeftMargin;
    float                 fRightMargin;

    float                 fMillimeters;
    int                   fTenths;

    float                 fBetweenSystemSpace;
    float                 fPageTopSpace;
};
typedef SMARTP<msrPageGeometry> S_msrPageGeometry;
EXP ostream& operator<< (ostream& os, const S_msrPageGeometry& elt);


} // namespace MusicXML2


#endif
