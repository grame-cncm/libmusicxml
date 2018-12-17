/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrBarlines___
#define ___bsrBarlines___

//#include <list>

#include "bsrLineElements.h"

#include "bsrCellsLists.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
class bsrBarline;
typedef SMARTP<bsrBarline> S_bsrBarline;

class bsrBarline : public bsrLineElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum bsrBarlineKind {
        kBarlineKindNone,
        kBarlineKindSpecial, // for dotted or dashed barlines
        kBarlineKindUnusual, // irregular spacing, ...
        kBarlineKindFinalDouble, kBarlineKindSectionalDouble };
        
    static string barlineKindAsString (
      bsrBarlineKind barlineKind);
      
    // creation
    // ------------------------------------------------------

    static SMARTP<bsrBarline> create (
      int            inputLineNumber,
      bsrBarlineKind barlineKind);
      
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrBarline (
      int            inputLineNumber,
      bsrBarlineKind barlineKin);
            
    virtual ~bsrBarline ();

  public:

    // set and get
    // ------------------------------------------------------

    bsrBarlineKind        getBarlineKind () const
                              { return fBarlineKind; }
    
    S_bsrCellsList        getBarlineCellsList () const
                              { return fBarlineCellsList; }

  public:

    // public services
    // ------------------------------------------------------
                  
    int                   fetchCellsNumber () const;

  private:
  
    // private services
    // ------------------------------------------------------
                                            
    S_bsrCellsList        barlineKindAsCellsList ();
                          
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

    bsrBarlineKind        fBarlineKind;

    S_bsrCellsList        fBarlineCellsList;
};
typedef SMARTP<bsrBarline> S_bsrBarline;
EXP ostream& operator<< (ostream& os, const S_bsrBarline& elt);


} // namespace MusicXML2


#endif
