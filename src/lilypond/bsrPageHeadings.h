/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrPageHeadings___
#define ___bsrPageHeadings___


#include "bsrElements.h"

#include "bsrCellsLists.h"

#include "bsrPaginations.h"
#include "bsrNumbers.h"

#include "brailleOptions.h"


namespace MusicXML2
{

//______________________________________________________________________________
class bsrPageHeading : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrPageHeading> create (
      int             inputLineNumber,
      string          pageHeadingTitle,
      S_bsrPagination pageHeadingPagination,
      int             pageHeadingNumber);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrPageHeading (
      int             inputLineNumber,
      string          pageHeadingTitle,
      S_bsrPagination pageHeadingPagination,
      int             pageHeadingNumber);
      
    virtual ~bsrPageHeading ();
  
  public:

    // set and get
    // ------------------------------------------------------
                              
    string                getPageHeadingTitle () const
                              { return fPageHeadingTitle ;}

    S_bsrPagination       getPageHeadingPagination () const
                              { return fPageHeadingPagination ;}

    int                   getPageHeadingNumber () const
                              { return fPageHeadingNumber ;}

    S_bsrCellsList        getPageHeadingCellsList () const
                              { return fPageHeadingCellsList ;}

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        asCellsList () const;    
  
  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    string                fPageHeadingTitle;

    S_bsrPagination       fPageHeadingPagination; // before title
    int                   fPageHeadingNumber;     // after title ??? JMI

    S_bsrCellsList        fPageHeadingCellsList;
};
typedef SMARTP<bsrPageHeading> S_bsrPageHeading;
EXP ostream& operator<< (ostream& os, const S_bsrPageHeading& elt);


}


#endif
