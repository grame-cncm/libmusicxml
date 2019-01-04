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

    void    setMillimeters        (float val) { fMillimeters = val; }
    void    setTenths             (int val)   { fTenths = val; }

    float   getMillimeters        () const    { return fMillimeters; }
    float   getTenths             () const    { return float(fTenths); }

    void    setPaperWidth         (float val) { fPaperWidth = val; }
    void    setPaperHeight        (float val) { fPaperHeight = val; }
    
    void    setTopMargin          (float val) { fTopMargin = val; }
    void    setBottomMargin       (float val) { fBottomMargin = val; }
    void    setLeftMargin         (float val) { fLeftMargin = val; }
    void    setRightMargin        (float val) { fRightMargin = val; }
    
    void    setBetweenSystemSpace (float val) { fBetweenSystemSpace = val; }
    void    setPageTopSpace       (float val) { fPageTopSpace = val; }
    
    float   getPaperWidth         () const    { return fPaperWidth; }
    float   getPaperHeight        () const    { return fPaperHeight; }
    
    float   getTopMargin          () const    { return fTopMargin; }
    float   getBottomMargin       () const    { return fBottomMargin; }
    float   getLeftMargin         () const    { return fLeftMargin; }
    float   getRightMargin        () const    { return fRightMargin; }
    
    float   getBetweenSystemSpace () const    { return fBetweenSystemSpace; }
    float   getPageTopSpace       () const    { return fPageTopSpace; }
    
    // services
    // ------------------------------------------------------

    float   globalStaffSize       () const;

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

    // page height, margins and the like in centimeters are in centimeters
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
