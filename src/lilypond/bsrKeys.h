/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrKeys___
#define ___bsrKeys___

//#include <list>

#include "bsrLineElements.h"

#include "bsrCellsLists.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
class bsrKey;
typedef SMARTP<bsrKey> S_bsrKey;

class bsrKey : public bsrLineElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum bsrKeyKind {
        kKeyKindNone,
        kKeyKindFlats, kKeyKindNaturals, kKeyKindSharps };
        
    static string keyKindAsString (
      bsrKeyKind keyKind);
      
    // creation
    // ------------------------------------------------------

    static SMARTP<bsrKey> create (
      int        inputLineNumber,
      bsrKeyKind keyKind,
      int        numberOfAlterations);
      
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrKey (
      int        inputLineNumber,
      bsrKeyKind keyKind,
      int        numberOfAlterations);
            
    virtual ~bsrKey ();

  public:

    // set and get
    // ------------------------------------------------------

    bsrKeyKind            getKeyKind () const
                              { return fKeyKind; }
    
    int                   getNumberOfAlterations () const
                              { return fNumberOfAlterations; }
    
    S_bsrCellsList        getKeyCellsList () const
                              { return fKeyCellsList; }

  public:

    // public services
    // ------------------------------------------------------
                  
  private:
  
    // private services
    // ------------------------------------------------------
                                            
    S_bsrCellsList        keyKindAsCellsList ();
                          
    S_bsrCellsList        asCellsList ();

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

    bsrKeyKind            fKeyKind;
    int                   fNumberOfAlterations;

    S_bsrCellsList        fKeyCellsList;
};
typedef SMARTP<bsrKey> S_bsrKey;
EXP ostream& operator<< (ostream& os, const S_bsrKey& elt);


} // namespace MusicXML2


#endif
