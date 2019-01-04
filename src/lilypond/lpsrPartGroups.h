/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsrPartGroups___
#define ___lpsrPartGroups___

#include "lpsrElements.h"

#include "msr.h"


namespace MusicXML2
{

//______________________________________________________________________________
class lpsrPartGroupBlock;
typedef SMARTP<lpsrPartGroupBlock> S_lpsrPartGroupBlock;

//______________________________________________________________________________
class lpsrPartGroupBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrPartGroupBlock> create (
      S_msrPartGroup partGroup);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrPartGroupBlock (
      S_msrPartGroup partGroup);
      
    virtual ~lpsrPartGroupBlock ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrPartGroup        getPartGroup () const
                              { return fPartGroup; }

    const list<S_msrElement>&
                          getPartGroupBlockElements () const
                              { return fPartGroupBlockElements; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendElementToPartGroupBlock (
                            S_msrElement elem)
                              { fPartGroupBlockElements.push_back (elem); }

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

    S_msrPartGroup        fPartGroup;
    
    list<S_msrElement>    fPartGroupBlockElements;
};
EXP ostream& operator<< (ostream& os, const S_lpsrPartGroupBlock& elt);


}


#endif
