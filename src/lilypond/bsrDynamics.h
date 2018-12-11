/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrDynamics___
#define ___bsrDynamics___

#include "bsrLineElements.h"

#include "bsrCellsLists.h"

#include "msrDynamics.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class bsrDynamics : public bsrLineElement
{
  public:

    // data types
    // ------------------------------------------------------

// JMI ???

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrDynamics> create (
      int                          inputLineNumber,
      msrDynamics::msrDynamicsKind dynamicsKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrDynamics (
      int                          inputLineNumber,
      msrDynamics::msrDynamicsKind dynamicsKind);
      
    virtual ~bsrDynamics ();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrDynamics::msrDynamicsKind
                          getDynamicsKind () const
                              { return fDynamicsKind; }
                                    
    S_bsrCellsList        getDynamicsCellsList () const
                              { return fDynamicsCellsList; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        asCellsList () const;    
  
    int                   fetchCellsNumber () const;

  private:

    // private services
    // ------------------------------------------------------
                                            
    S_bsrCellsList        noteValueKindAsCellsList () const;
                          
    S_bsrCellsList        noteOctaveKindAsCellsList () const;

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
    
    msrDynamics::msrDynamicsKind
                          fDynamicsKind;
    
    S_bsrCellsList        fDynamicsCellsList;
};
typedef SMARTP<bsrDynamics> S_bsrDynamics;
EXP ostream& operator<< (ostream& os, const S_bsrDynamics& elt);


} // namespace MusicXML2


#endif
