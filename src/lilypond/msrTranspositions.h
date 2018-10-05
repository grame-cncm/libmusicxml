/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrTranspositions___
#define ___msrTranspositions___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrOctaveShift : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrOctaveShiftKind {
      kOctaveShiftNone,
      kOctaveShiftUp, kOctaveShiftDown,
      kOctaveShiftStop, kOctaveShiftContinue };

    static string octaveShiftKindAsString (
      msrOctaveShiftKind octaveShiftKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrOctaveShift> create (
      int                inputLineNumber,
      msrOctaveShiftKind octaveShiftKind,
      int                octaveShiftSize);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOctaveShift (
      int                inputLineNumber,
      msrOctaveShiftKind octaveShiftKind,
      int                octaveShiftSize);
      
    virtual ~msrOctaveShift ();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrOctaveShiftKind    getOctaveShiftKind () const
                              { return fOctaveShiftKind; }

    int                   getOctaveShiftSize () const
                              { return fOctaveShiftSize; }

    // services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                octaveShiftKindAsString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    msrOctaveShiftKind    fOctaveShiftKind;

    int                   fOctaveShiftSize;
};
typedef SMARTP<msrOctaveShift> S_msrOctaveShift;
EXP ostream& operator<< (ostream& os, const S_msrOctaveShift& elt);

//______________________________________________________________________________
class msrTranspose;
typedef SMARTP<msrTranspose> S_msrTranspose;

class msrTranspose : public msrElement
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTranspose> create (
      int  inputLineNumber,
      int  transposeDiatonic,
      int  transposeChromatic,
      int  transposeOctaveChange,
      bool transposeDouble);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTranspose (
      int  inputLineNumber,
      int  transposeDiatonic,
      int  transposeChromatic,
      int  transposeOctaveChange,
      bool transposeDouble);
      
    virtual ~msrTranspose ();
  
  public:

    // set and get
    // ------------------------------------------------------

    int                   getTransposeDiatonic () const
                              { return fTransposeDiatonic; }
                  
    int                   getTransposeChromatic () const
                              { return fTransposeChromatic; }
                  
    int                   getTransposeOctaveChange () const
                              { return fTransposeOctaveChange; }
                  
    int                   getTransposeDouble () const
                              { return fTransposeDouble; }
                  
    // services
    // ------------------------------------------------------

    bool                  isEqualTo (S_msrTranspose otherTranspose) const;
                            
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

    int                   fTransposeDiatonic;
    int                   fTransposeChromatic;
    int                   fTransposeOctaveChange;
    bool                  fTransposeDouble;
};
typedef SMARTP<msrTranspose> S_msrTranspose;
EXP ostream& operator<< (ostream& os, const S_msrTranspose& elt);


} // namespace MusicXML2


#endif
