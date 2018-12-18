/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrTempos___
#define ___bsrTempos___

#include "bsrLineElements.h"

#include "bsrCellsLists.h"

#include "msrTempos.h"


namespace MusicXML2 
{

const bsrCellKind
  kCellTempoEquals = kDots2356,
  kCellTempoHyphen = kDots36;


//______________________________________________________________________________
class bsrTempo : public bsrLineElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrTempo> create (
      int        inputLineNumber,
      S_msrTempo mTempo);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrTempo (
      int        inputLineNumber,
      S_msrTempo mTempo);
      
    virtual ~bsrTempo ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrTempo            getMsrTempo () const
                              { return fMsrTempo; }
                                    
    S_bsrCellsList        getTempoCellsList () const
                              { return fTempoCellsList; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const
                              { return fTempoCellsList; }
  
    int                   fetchCellsNumber () const;

  private:

    // private services
    // ------------------------------------------------------
                                            
    S_bsrCellsList        noteValueKindAsCellsList () const;
                          
    S_bsrCellsList        noteOctaveKindAsCellsList () const;

    S_bsrCellsList        buildCellsList () const;    

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
    
    S_msrTempo            fMsrTempo;
    
    S_bsrCellsList        fTempoCellsList;
};
typedef SMARTP<bsrTempo> S_bsrTempo;
EXP ostream& operator<< (ostream& os, const S_bsrTempo& elt);


} // namespace MusicXML2


#endif
