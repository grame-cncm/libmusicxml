/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msr2Names___
#define ___msr2Names___

#include "msr.h"

#include "msrOptions.h"

namespace MusicXML2
{

//________________________________________________________________________
class msr2NamesVisitor :

  public visitor<S_msrScore>,
  
  // parts & part groups

  public visitor<S_msrPartGroup>,
  
  public visitor<S_msrPart>,
  
  // staves

  public visitor<S_msrStaff>,

  // voices
  
  public visitor<S_msrVoice>,

  // varValAssocs
  
  public visitor<S_msrVarValAssoc>,
  public visitor<S_msrVarValsListAssoc>

{
  public:
  
    msr2NamesVisitor (
      S_msrOptions&    msrOpts,
      indentedOstream& ios);
        
    virtual ~msr2NamesVisitor ();

    void printNamesFromMsrScore (
      const S_msrScore& score);
    
  protected:
      
    virtual void visitStart (S_msrScore& elt);
    virtual void visitEnd   (S_msrScore& elt);

    virtual void visitStart (S_msrPartGroup& elt);
    virtual void visitEnd   (S_msrPartGroup& elt);

    virtual void visitStart (S_msrPart& elt);
    virtual void visitEnd   (S_msrPart& elt);

    virtual void visitStart (S_msrStaff& elt);
    virtual void visitEnd   (S_msrStaff& elt);

    virtual void visitStart (S_msrVoice& elt);
    virtual void visitEnd   (S_msrVoice& elt);

    virtual void visitStart (S_msrVarValAssoc& elt);
    virtual void visitEnd   (S_msrVarValAssoc& elt);
    virtual void visitStart (S_msrVarValsListAssoc& elt);
    virtual void visitEnd   (S_msrVarValsListAssoc& elt);

  private:
                     
    S_msrOptions          fMsrOptions;
    
    indentedOstream&      fMsrNamesOutputStream;

    // score
    // ------------------------------------------------------
    
    // part groups
    // ------------------------------------------------------
    
    int                   fPartGroupsCounter;

    // parts
    // ------------------------------------------------------
    
    int                   fPartsCounter;

    // staves
    // ------------------------------------------------------
    
    int                   fStavesCounter;
    
    // prevent clef, key and time from being handled twice
    bool                  fOnGoingStaff;

    // voices
    // ------------------------------------------------------
    
    int                   fVoicesCounter;
};


}


#endif
