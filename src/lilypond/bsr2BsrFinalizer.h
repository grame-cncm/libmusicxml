/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsr2BsrFinalizer___
#define ___bsr2BsrFinalizer___

#include "bsr.h"

#include "bsrOptions.h"


namespace MusicXML2
{

//________________________________________________________________________
class bsr2BsrFinalizer :
  
  public visitor<S_bsrScore>,
  
  public visitor<S_bsrTranscriptionNotes>,
  public visitor<S_bsrTranscriptionNotesElement>,
  
  public visitor<S_bsrPage>,
  
  public visitor<S_bsrPageHeading>,
  
  public visitor<S_bsrLine>,

  public visitor<S_bsrNumber>,
  
  public visitor<S_bsrClef>,
  public visitor<S_bsrKey>,
  public visitor<S_bsrTime>,
  
  public visitor<S_bsrNote>

{
  public:
  
    bsr2BsrFinalizer (
      S_bsrOptions&    bsrOpts,
      indentedOstream& logIOstream,
      S_bsrScore       bScore);
        
    virtual ~bsr2BsrFinalizer ();

    void generateFinalizedBsrScoreFromBsrScore ();

    S_bsrScore getBsrScore () const
        { return fBsrScore; };

  protected:
        
    virtual void visitStart (S_bsrScore& elt);
    virtual void visitEnd   (S_bsrScore& elt);

    virtual void visitStart (S_bsrSpaces& elt);
    virtual void visitEnd   (S_bsrSpaces& elt);

    virtual void visitStart (S_bsrTranscriptionNotes& elt);
    virtual void visitEnd   (S_bsrTranscriptionNotes& elt);

    virtual void visitStart (S_bsrTranscriptionNotesElement& elt);
    virtual void visitEnd   (S_bsrTranscriptionNotesElement& elt);

    virtual void visitStart (S_bsrPage& elt);
    virtual void visitEnd   (S_bsrPage& elt);

    virtual void visitStart (S_bsrPageHeading& elt);
    virtual void visitEnd   (S_bsrPageHeading& elt);

    virtual void visitStart (S_bsrLine& elt);
    virtual void visitEnd   (S_bsrLine& elt);

    virtual void visitStart (S_bsrNumber& elt);
    virtual void visitEnd   (S_bsrNumber& elt);

    virtual void visitStart (S_bsrClef& elt);
    virtual void visitEnd   (S_bsrClef& elt);

    virtual void visitStart (S_bsrKey& elt);
    virtual void visitEnd   (S_bsrKey& elt);

    virtual void visitStart (S_bsrTime& elt);
    virtual void visitEnd   (S_bsrTime& elt);

    virtual void visitStart (S_bsrNote& elt);
    virtual void visitEnd   (S_bsrNote& elt);

  private:

    // the MSR score we're relying on
    // ------------------------------------------------------
    S_msrScore            fBaseMsrScore;
                  
    // the BSR score we're visiting
    // ------------------------------------------------------
    S_bsrScore            fVisitedBsrScore;
                  
    // the BSR score we're building
    // ------------------------------------------------------
    S_bsrScore            fBsrScore;

    // pages & lines
    // ------------------------------------------------------

    S_bsrPage             fCurrentPage;
    S_bsrLine             fCurrentLine;
    

    S_bsrOptions          fBsrOptions;

    indentedOstream&      fLogOutputStream;
};


}


#endif
