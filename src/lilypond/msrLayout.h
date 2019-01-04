/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrLayout___
#define ___msrLayout___

#include "msrElements.h"

#include "msrBasicTypes.h"

#include "msrVarValAssocs.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrLayout : public msrElement // JMI
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrLayout> create (
      int           inputLineNumber);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrLayout (
      int           inputLineNumber);
      
    virtual ~msrLayout ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    void                  addMsrVarValAssoc (S_msrVarValAssoc assoc)
                              { fVarValAssocs.push_back(assoc); } // JMI ???
      
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

    vector<S_msrVarValAssoc> fVarValAssocs;
};
typedef SMARTP<msrLayout> S_msrLayout;
EXP ostream& operator<< (ostream& os, const S_msrLayout& elt);


} // namespace MusicXML2


#endif
