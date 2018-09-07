/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msr2Names__
#define __msr2Names__

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
  
  public visitor<S_msrVoice>,

  // lyrics

  public visitor<S_msrStanza>,
  public visitor<S_msrSyllable>,
  
  public visitor<S_msrClef>,
  public visitor<S_msrKey>,
  public visitor<S_msrTime>,
  public visitor<S_msrTempo>,
  
  public visitor<S_msrSegment>,
    
  public visitor<S_msrArticulation>,

  public visitor<S_msrDynamics>,
  public visitor<S_msrWedge>,
  
  public visitor<S_msrGraceNotes>,
  
  public visitor<S_msrNote>,
  
  public visitor<S_msrStem>,
  public visitor<S_msrBeam>,
    
  public visitor<S_msrChord>,
  
  public visitor<S_msrTuplet>,
  
  public visitor<S_msrSlur>,
  
  public visitor<S_msrBarline>,
  public visitor<S_msrBarNumberCheck>,
  public visitor<S_msrLineBreak>,
  
  public visitor<S_msrRepeat>,
    
  public visitor<S_msrVarValAssoc>,
  public visitor<S_msrVarValsListAssoc>,
  
  public visitor<S_msrPageGeometry>,
  public visitor<S_msrLayout>,
  
  public visitor<S_msrMidi>

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
