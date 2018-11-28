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

#include "bsrBrailleElements.h"

#include "bsrCellsLists.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class bsrNote : public bsrBrailleElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum bsrNoteValueKind {
        kNoteRestBreveKind,
        kNoteRestWholeKind,
        kNoteRestHalfKind,
        kNoteRestQuarterKind,
        kNoteRest8thKind,
        kNoteRest16thKind,
        kNoteRest32ndKind,
        kNoteRest64thKind,
        kNoteRest128thKind,
        kNoteRest256thKind,
        
        kNoteCBreveKind,
        kNoteCWholeKind,
        kNoteCHalfKind,
        kNoteCQuarterKind,
        kNoteC8thKind,
        kNoteC16thKind,
        kNoteC32ndKind,
        kNoteC64thKind,
        kNoteC128thKind,
        kNoteC256thKind,
        
        kNoteDBreveKind,
        kNoteDWholeKind,
        kNoteDHalfKind,
        kNoteDQuarterKind,
        kNoteD8thKind,
        kNoteD16thKind,
        kNoteD32ndKind,
        kNoteD64thKind,
        kNoteD128thKind,
        kNoteD256thKind,
        
        kNoteEBreveKind,
        kNoteEWholeKind,
        kNoteEHalfKind,
        kNoteEQuarterKind,
        kNoteE8thKind,
        kNoteE16thKind,
        kNoteE32ndKind,
        kNoteE64thKind,
        kNoteE128thKind,
        kNoteE256thKind,
        
        kNoteFBreveKind,
        kNoteFWholeKind,
        kNoteFHalfKind,
        kNoteFQuarterKind,
        kNoteF8thKind,
        kNoteF16thKind,
        kNoteF32ndKind,
        kNoteF64thKind,
        kNoteF128thKind,
        kNoteF256thKind,
        
        kNoteGBreveKind,
        kNoteGWholeKind,
        kNoteGHalfKind,
        kNoteGQuarterKind,
        kNoteG8thKind,
        kNoteG16thKind,
        kNoteG32ndKind,
        kNoteG64thKind,
        kNoteG128thKind,
        kNoteG256thKind,
        
        kNoteABreveKind,
        kNoteAWholeKind,
        kNoteAHalfKind,
        kNoteAQuarterKind,
        kNoteA8thKind,
        kNoteA16thKind,
        kNoteA32ndKind,
        kNoteA64thKind,
        kNoteA128thKind,
        kNoteA256thKind,
        
        kNoteBBreveKind,
        kNoteBWholeKind,
        kNoteBHalfKind,
        kNoteBQuarterKind,
        kNoteB8thKind,
        kNoteB16thKind,
        kNoteB32ndKind,
        kNoteB64thKind,
        kNoteB128thKind,
        kNoteB256thKind };

    static string noteValueKindAsString (
      bsrNoteValueKind noteValueKind);

    static S_bsrCellsList noteValueKindAsCellsList (
      int              inputLineNumber,
      bsrNoteValueKind noteValueKind);

    enum bsrNoteOctaveKind {
        kNoteOctaveBelow1Kind,
        kNoteOctave1Kind,
        kNoteOctave2Kind,
        kNoteOctave3Kind,
        kNoteOctave4Kind,
        kNoteOctave5Kind,
        kNoteOctave6Kind,
        kNoteOctave7Kind,
        kNoteOctaveAbove7Kind };

    static string noteOctaveKindAsString (
      bsrNoteOctaveKind noteOctaveKind);

    static S_bsrCellsList noteOctaveKindAsCellsList (
      int               inputLineNumber,
      bsrNoteOctaveKind noteOctaveKind);

    enum bsrNoteOctaveIsNeededKind {
        kNoteOctaveIsNeededYes, kNoteOctaveIsNeededNo};

    static string noteOctaveIsNeededKindAsString (
      bsrNoteOctaveIsNeededKind noteOctaveIsNeededKind);

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrNote> create (
      int               inputLineNumber,
      bsrNoteValueKind  noteValueKind,
      int               noteDotsNumber,
      bsrNoteOctaveKind noteOctaveKind,
      bsrNoteOctaveIsNeededKind
                        noteOctaveIsNeededKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrNote (
      int               inputLineNumber,
      bsrNoteValueKind  noteValueKind,
      int               noteDotsNumber,
      bsrNoteOctaveKind noteOctaveKind,
      bsrNoteOctaveIsNeededKind
                        noteOctaveIsNeededKind);
      
    virtual ~bsrNote ();
  
  public:

    // set and get
    // ------------------------------------------------------

    bsrNoteValueKind      getNoteValueKind () const
                              { return fNoteValueKind; }
                  
    int                   getNoteDotsNumber () const
                              { return fNoteDotsNumber; }
                  
    void                  setNoteOctaveIsNeededKind (
                            bsrNoteOctaveIsNeededKind
                              noteOctaveIsNeededKindvalue)
                              {
                                fNoteOctaveIsNeededKind =
                                  noteOctaveIsNeededKindvalue;
                              }
                  
    bsrNoteOctaveIsNeededKind
                          getNoteOctaveIsNeededKind () const
                              { return fNoteOctaveIsNeededKind; }
                  
    S_bsrCellsList      getNoteCellsList () const
                              { return fNoteCellsList; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList      asCellsList ();
    
  private:
  
    // private services
    // ------------------------------------------------------
                                            
    S_bsrCellsList      noteValueKindAsCellsList ();
                          
    S_bsrCellsList      noteOctaveKindAsCellsList ();

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

    bsrNoteOctaveIsNeededKind
                          fNoteOctaveIsNeededKind;

    S_bsrCellsList      fNoteCellsList;
};
typedef SMARTP<bsrNote> S_bsrNote;
EXP ostream& operator<< (ostream& os, const S_bsrNote& elt);


} // namespace MusicXML2


#endif
