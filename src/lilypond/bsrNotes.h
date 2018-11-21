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

#include "bsrBrailleSigns.h"

#include "bsrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class bsrNote : public bsrBrailleElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum bsrNoteValueKind {
        kNoteCEighthKind,
        kNoteCQuarterKind,
        kNoteCHalfKind,
        kNoteCWholeKind,
        
        kNoteDEighthKind,
        kNoteDQuarterKind,
        kNoteDHalfKind,
        kNoteDWholeKind,
        
        kNoteEEighthKind,
        kNoteEQuarterKind,
        kNoteEHalf4Kind,
        kNoteEWholeKind,
        
        kNoteFEighthKind,
        kNoteFQuarterKind,
        kNoteFHalfKind,
        kNoteFWholeKind,
        
        kNoteGEighthKind,
        kNoteGQuarterKind,
        kNoteGHalfKind,
        kNoteGWholeKind,
        
        kNoteAEighthKind,
        kNoteAQuarterKind,
        kNoteAHalfKind,
        kNoteAWholeKind,
        
        kNoteBEighthKind,
        kNoteBQuarterKind,
        kNoteBHalfKind,
        kNoteBWholeKind };

    static string noteValueKindAsString (
      bsrNoteValueKind noteValueKind);

    static bsrSixDotsKind noteValueKindAsSixDotsKind (
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

    static bsrSixDotsKind noteOctaveKindAsSixDotsKind (
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
      bsrNoteOctaveKind noteOctaveKind,
      bsrNoteOctaveIsNeededKind
                        noteOctaveIsNeededKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrNote (
      int               inputLineNumber,
      bsrNoteValueKind  noteValueKind,
      bsrNoteOctaveKind noteOctaveKind,
      bsrNoteOctaveIsNeededKind
                        noteOctaveIsNeededKind);
      
    virtual ~bsrNote ();
  
  public:

    // set and get
    // ------------------------------------------------------

    bsrNoteValueKind      getNoteValueKind () const
                              { return fNoteValueKind; }
                  
    bsrNoteOctaveKind     getNoteOctaveKind () const
                              { return fNoteOctaveKind; }
                  
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
                  
    // services
    // ------------------------------------------------------

    bsrSixDotsKind        noteValueKindAsSixDotsKind ()
                              {
                                return
                                  noteValueKindAsSixDotsKind  (
                                    fNoteValueKind);
                              }
                          
    bsrSixDotsKind        noteOctaveKindAsSixDotsKind ()
                              {
                                return
                                  noteOctaveKindAsSixDotsKind  (
                                    fNoteOctaveKind);
                              }

    S_bsrBrailleSign      asBrailleSign ();
    
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
    bsrNoteOctaveKind     fNoteOctaveKind;

    bsrNoteOctaveIsNeededKind
                          fNoteOctaveIsNeededKind;
};
typedef SMARTP<bsrNote> S_bsrNote;
EXP ostream& operator<< (ostream& os, const S_bsrNote& elt);


} // namespace MusicXML2


#endif
