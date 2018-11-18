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
        kBsrCEighthKind,
        kBsrCQuarterKind,
        kBsrCHalfKind,
        kBsrCWholeKind,
        
        kBsrDEighthKind,
        kBsrDQuarterKind,
        kBrlDHalfKind,
        kBsrDWholeKind,
        
        kBsrEEighthKind,
        kBsrEQuarterKind,
        kBsrEHalf4Kind,
        kBsrEWholeKind,
        
        kBsrFEighthKind,
        kBsrFQuarterKind,
        kBsrFHalfKind,
        kBsrFWholeKind,
        
        kBsrGEighthKind,
        kBsrGQuarterKind,
        kBsrGHalfKind,
        kBsrGWholeKind,
        
        kBsrAEighthKind,
        kBsrAQuarterKind,
        kBsrAHalfKind,
        kBsrAWholeKind,
        
        kBsrBEighthKind,
        kBsrBQuarterKind,
        kBsrBHalfKind,
        kBsrBWholeKind };

    static wchar_t noteValueKindAsBraille (
      bsrNoteValueKind noteValueKind);

    static string noteValueKindAsString (
      bsrNoteValueKind noteValueKind);

    enum bsrNoteOctaveKind {
        kBrlOctaveBelow1Kind,
        kBrlOctave1Kind,
        kBrlOctave2Kind,
        kBrlOctave3Kind,
        kBrlOctave4Kind,
        kBrlOctave5Kind,
        kBrlOctave6Kind,
        kBrlOctave7Kind,
        kBrlOctaveAbove7Kind };

    wchar_t noteOctaveKindAsBraille (
      bsrNoteOctaveKind noteOctaveKind);

    static string noteOctaveKindAsString (
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

    wchar_t               noteValueKindAsBraille ()
                              {
                                return
                                  noteValueKindAsBraille (
                                    fNoteValueKind);
                              }
                          
    wchar_t               noteOctaveKindAsBraille ()
                              {
                                return
                                  noteOctaveKindAsBraille (
                                    fNoteOctaveKind);
                              }
                          
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
