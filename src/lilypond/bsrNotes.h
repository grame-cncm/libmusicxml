/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrNotes___
#define ___bsrNotes___

#include "bsrLineElements.h"

#include "bsrCellsLists.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class bsrNote : public bsrLineElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum bsrNoteValueKind {
        kNoteKindNone,
        
        kNoteKindRestBreve,
        kNoteKindRestWhole,
        kNoteKindRestHalf,
        kNoteKindRestQuarter,
        kNoteKindRest8th,
        kNoteKindRest16th,
        kNoteKindRest32nd,
        kNoteKindRest64th,
        kNoteKindRest128th,
        kNoteKindRest256th,
        
        kNoteKindCBreve,
        kNoteKindCWhole,
        kNoteKindCHalf,
        kNoteKindCQuarter,
        kNoteKindC8th,
        kNoteKindC16th,
        kNoteKindC32nd,
        kNoteKindC64th,
        kNoteKindC128th,
        kNoteKindC256th,
        
        kNoteKindDBreve,
        kNoteKindDWhole,
        kNoteKindDHalf,
        kNoteKindDQuarter,
        kNoteKindD8th,
        kNoteKindD16th,
        kNoteKindD32nd,
        kNoteKindD64th,
        kNoteKindD128th,
        kNoteKindD256th,
        
        kNoteKindEBreve,
        kNoteKindEWhole,
        kNoteKindEHalf,
        kNoteKindEQuarter,
        kNoteKindE8th,
        kNoteKindE16th,
        kNoteKindE32nd,
        kNoteKindE64th,
        kNoteKindE128th,
        kNoteKindE256th,
        
        kNoteKindFBreve,
        kNoteKindFWhole,
        kNoteKindFHalf,
        kNoteKindFQuarter,
        kNoteKindF8th,
        kNoteKindF16th,
        kNoteKindF32nd,
        kNoteKindF64th,
        kNoteKindF128th,
        kNoteKindF256th,
        
        kNoteKindGBreve,
        kNoteKindGWhole,
        kNoteKindGHalf,
        kNoteKindGQuarter,
        kNoteKindG8th,
        kNoteKindG16th,
        kNoteKindG32nd,
        kNoteKindG64th,
        kNoteKindG128th,
        kNoteKindG256th,
        
        kNoteKindABreve,
        kNoteKindAWhole,
        kNoteKindAHalf,
        kNoteKindAQuarter,
        kNoteKindA8th,
        kNoteKindA16th,
        kNoteKindA32nd,
        kNoteKindA64th,
        kNoteKindA128th,
        kNoteKindA256th,
        
        kNoteKindBBreve,
        kNoteKindBWhole,
        kNoteKindBHalf,
        kNoteKindBQuarter,
        kNoteKindB8th,
        kNoteKindB16th,
        kNoteKindB32nd,
        kNoteKindB64th,
        kNoteKindB128th,
        kNoteKindB256th };

    static string noteValueKindAsString (
      bsrNoteValueKind noteValueKind);

    static S_bsrCellsList noteValueKindAsCellsList (
      int              inputLineNumber,
      bsrNoteValueKind noteValueKind);

    enum bsrNoteOctaveKind {
        kNoteOctaveKindBelow1,
        kNoteOctaveKind1,
        kNoteOctaveKind2,
        kNoteOctaveKind3,
        kNoteOctaveKind4,
        kNoteOctaveKind5,
        kNoteOctaveKind6,
        kNoteOctaveKind7,
        kNoteOctaveKindAbove7 };

    static string noteOctaveKindAsString (
      bsrNoteOctaveKind noteOctaveKind);

    static S_bsrCellsList noteOctaveKindAsCellsList (
      int               inputLineNumber,
      bsrNoteOctaveKind noteOctaveKind);

    enum bsrNoteOctaveIsNeeded {
        kNoteOctaveIsNeededYes, kNoteOctaveIsNeededNo};

    static string noteOctaveIsNeededAsString (
      bsrNoteOctaveIsNeeded noteOctaveIsNeeded);

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrNote> create (
      int                   inputLineNumber,
      bsrNoteValueKind      noteValueKind,
      int                   noteDotsNumber,
      bsrNoteOctaveKind     noteOctaveKind,
      bsrNoteOctaveIsNeeded noteOctaveIsNeeded);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrNote (
      int                   inputLineNumber,
      bsrNoteValueKind      noteValueKind,
      int                   noteDotsNumber,
      bsrNoteOctaveKind     noteOctaveKind,
      bsrNoteOctaveIsNeeded noteOctaveIsNeeded);
      
    virtual ~bsrNote ();
  
  public:

    // set and get
    // ------------------------------------------------------

    bsrNoteValueKind      getNoteValueKind () const
                              { return fNoteValueKind; }
                  
    int                   getNoteDotsNumber () const
                              { return fNoteDotsNumber; }
                  
    void                  setNoteOctaveIsNeeded (
                            bsrNoteOctaveIsNeeded
                              noteOctaveIsNeededvalue)
                              {
                                fNoteOctaveIsNeeded =
                                  noteOctaveIsNeededvalue;
                              }
                  
    bsrNoteOctaveIsNeeded getNoteOctaveIsNeeded () const
                              { return fNoteOctaveIsNeeded; }
                  
    S_bsrCellsList        getNoteCellsList () const
                              { return fNoteCellsList; }

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

    
    bsrNoteValueKind      fNoteValueKind;
    int                   fNoteDotsNumber;
    
    bsrNoteOctaveKind     fNoteOctaveKind;

    bsrNoteOctaveIsNeeded fNoteOctaveIsNeeded;

    S_bsrCellsList        fNoteCellsList;
};
typedef SMARTP<bsrNote> S_bsrNote;
EXP ostream& operator<< (ostream& os, const S_bsrNote& elt);


} // namespace MusicXML2


#endif
