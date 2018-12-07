/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrPaginations___
#define ___bsrPaginations___

#include "bsrLineElements.h"

#include "bsrCellsLists.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
class bsrPagination : public bsrLineElement
{
  public:
          
    // creation
    // ------------------------------------------------------

    static SMARTP<bsrPagination> create (
      int inputLineNumber,
      int printPageNumber,
      int braillePageNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrPagination (
      int inputLineNumber,
      int printPageNumber,
      int braillePageNumber);
      
    virtual ~bsrPagination ();
  
  public:

    // set and get
    // ------------------------------------------------------
                  
    int                   getPrintPageNumber () const
                              { return fPrintPageNumber; }

    int                   getBraillePageNumber () const
                              { return fBraillePageNumber; }
                              
    S_bsrCellsList        getPaginationCellsList () const
                              { return fPaginationCellsList; }

    // services
    // ------------------------------------------------------
                  
    S_bsrCellsList        asCellsList () const;    
  
    int                   fetchCellsNumber () const;

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

    int                   fPrintPageNumber;
    int                   fBraillePageNumber;

    S_bsrCellsList        fPaginationCellsList;
};
typedef SMARTP<bsrPagination> S_bsrPagination;
EXP ostream& operator<< (ostream& os, const S_bsrPagination& elt);


} // namespace MusicXML2


#endif
