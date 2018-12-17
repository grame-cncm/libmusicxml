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
        kNoteValueNone,
        
        kNoteValueRestBreve,
        kNoteValueRestWhole,
        kNoteValueRestHalf,
        kNoteValueRestQuarter,
        kNoteValueRest8th,
        kNoteValueRest16th,
        kNoteValueRest32nd,
        kNoteValueRest64th,
        kNoteValueRest128th,
        kNoteValueRest256th,
        
        kNoteValueCBreve,
        kNoteValueCWhole,
        kNoteValueCHalf,
        kNoteValueCQuarter,
        kNoteValueC8th,
        kNoteValueC16th,
        kNoteValueC32nd,
        kNoteValueC64th,
        kNoteValueC128th,
        kNoteValueC256th,
        
        kNoteValueDBreve,
        kNoteValueDWhole,
        kNoteValueDHalf,
        kNoteValueDQuarter,
        kNoteValueD8th,
        kNoteValueD16th,
        kNoteValueD32nd,
        kNoteValueD64th,
        kNoteValueD128th,
        kNoteValueD256th,
        
        kNoteValueEBreve,
        kNoteValueEWhole,
        kNoteValueEHalf,
        kNoteValueEQuarter,
        kNoteValueE8th,
        kNoteValueE16th,
        kNoteValueE32nd,
        kNoteValueE64th,
        kNoteValueE128th,
        kNoteValueE256th,
        
        kNoteValueFBreve,
        kNoteValueFWhole,
        kNoteValueFHalf,
        kNoteValueFQuarter,
        kNoteValueF8th,
        kNoteValueF16th,
        kNoteValueF32nd,
        kNoteValueF64th,
        kNoteValueF128th,
        kNoteValueF256th,
        
        kNoteValueGBreve,
        kNoteValueGWhole,
        kNoteValueGHalf,
        kNoteValueGQuarter,
        kNoteValueG8th,
        kNoteValueG16th,
        kNoteValueG32nd,
        kNoteValueG64th,
        kNoteValueG128th,
        kNoteValueG256th,
        
        kNoteValueABreve,
        kNoteValueAWhole,
        kNoteValueAHalf,
        kNoteValueAQuarter,
        kNoteValueA8th,
        kNoteValueA16th,
        kNoteValueA32nd,
        kNoteValueA64th,
        kNoteValueA128th,
        kNoteValueA256th,
        
        kNoteValueBBreve,
        kNoteValueBWhole,
        kNoteValueBHalf,
        kNoteValueBQuarter,
        kNoteValueB8th,
        kNoteValueB16th,
        kNoteValueB32nd,
        kNoteValueB64th,
        kNoteValueB128th,
        kNoteValueB256th };

    static string noteValueKindAsString (
      bsrNoteValueKind noteValueKind);

    static S_bsrCellsList noteValueKindAsCellsList (
      int              inputLineNumber,
      bsrNoteValueKind noteValueKind);

    enum bsrNoteOctaveKind {
        kNoteOctaveNone,
        kNoteOctaveBelow1,
        kNoteOctave1,
        kNoteOctave2,
        kNoteOctave3,
        kNoteOctave4,
        kNoteOctave5,
        kNoteOctave6,
        kNoteOctave7,
        kNoteOctaveAbove7 };

    static string noteOctaveKindAsString (
      bsrNoteOctaveKind noteOctaveKind);

    static S_bsrCellsList noteOctaveKindAsCellsList (
      int               inputLineNumber,
      bsrNoteOctaveKind noteOctaveKind);

    enum bsrNoteOctaveIsNeeded {
        kNoteOctaveIsNeededYes, kNoteOctaveIsNeededNo};

    static string noteOctaveIsNeededAsString (
      bsrNoteOctaveIsNeeded noteOctaveIsNeeded);

    enum bsrNoteAccidentalKind {
      kNoteAccidentalNone,
      kNoteAccidentalSharp, kNoteAccidentalNatural, kNoteAccidentalFlat,
      kNoteAccidentalDoubleFlat, kNoteAccidentalDoubleSharp,
      kNoteAccidentalQuarterSharp, kNoteAccidentalQuarterFlat,
      kNoteAccidentalThreeQuarterSharp, kNoteAccidentalThreeQuarterFlat };
  
    static string noteAccidentalKindAsString (
      bsrNoteAccidentalKind noteAccidentalKind);
  
    static S_bsrCellsList noteAccidentalKindAsCellsList (
      int                   inputLineNumber,
      bsrNoteAccidentalKind noteAccidentalKind);

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrNote> create (
      int                   inputLineNumber,
      bsrNoteValueKind      noteValueKind,
      int                   noteDotsNumber,
      bsrNoteOctaveKind     noteOctaveKind,
      bsrNoteOctaveIsNeeded noteOctaveIsNeeded,
      bsrNoteAccidentalKind noteAccidentalKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrNote (
      int                   inputLineNumber,
      bsrNoteValueKind      noteValueKind,
      int                   noteDotsNumber,
      bsrNoteOctaveKind     noteOctaveKind,
      bsrNoteOctaveIsNeeded noteOctaveIsNeeded,
      bsrNoteAccidentalKind noteAccidentalKind);
      
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
                  
    bsrNoteAccidentalKind getNoteAccidentalKind () const
                              { return fNoteAccidentalKind; }
                  
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
    
    S_bsrCellsList        noteAccidentalKindAsCellsList () const;

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

    bsrNoteAccidentalKind fNoteAccidentalKind;

    S_bsrCellsList        fNoteCellsList;
};
typedef SMARTP<bsrNote> S_bsrNote;
EXP ostream& operator<< (ostream& os, const S_bsrNote& elt);


} // namespace MusicXML2


#endif
