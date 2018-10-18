/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrTablatures___
#define ___msrTablatures___

#include <list>

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrFrameNote : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrBarreTypeKind {
      kBarreTypeNone,
      kBarreTypeStart, kBarreTypeStop };

    static string barreTypeKindAsString (
      msrBarreTypeKind barreTypeKind);
      
/*
          <frame-note>
            <string>6</string>
            <fret>0</fret>
          </frame-note>
*/

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFrameNote> create (
      int              inputLineNumber,
      int              frameNoteStringNumber,
      int              frameNoteFretNumber,
      int              frameNoteFingering,
      msrBarreTypeKind frameNoteBarreTypeKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrFrameNote (
      int              inputLineNumber,
      int              frameNoteStringNumber,
      int              frameNoteFretNumber,
      int              frameNoteFingering,
      msrBarreTypeKind frameNoteBarreTypeKind);

    virtual ~msrFrameNote ();
  
  public:

    // set and get
    // ------------------------------------------------------

    int                   getFrameNoteStringNumber () const
                              { return fFrameNoteStringNumber; }
                              
    int                   getFrameNoteFretNumber () const
                              { return fFrameNoteFretNumber; }
                              
    int                   getFrameNoteFingering () const
                              { return fFrameNoteFingering; }
                              
    msrBarreTypeKind      getFrameNoteBarreTypeKind () const
                              { return fFrameNoteBarreTypeKind; }
                              
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

    string                asString () const;
   
    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    int                   fFrameNoteStringNumber;
    int                   fFrameNoteFretNumber;

    int                   fFrameNoteFingering;

    msrBarreTypeKind      fFrameNoteBarreTypeKind;
};
typedef SMARTP<msrFrameNote> S_msrFrameNote;
EXP ostream& operator<< (ostream& os, const S_msrFrameNote& elt);

//______________________________________________________________________________
struct msrBarre {
  public:

    // constructors/destructor
    // ------------------------------------------------------

    msrBarre (
      int barreStartString,
      int barreStopString,
      int barreFretNumber)
    {
      fBarreStartString = barreStartString;
      fBarreStopString  = barreStopString;
      fBarreFretNumber  = barreFretNumber;
    }

    // fields
    // ------------------------------------------------------
    
    int                   fBarreStartString;
    int                   fBarreStopString;
    int                   fBarreFretNumber;
};

//______________________________________________________________________________
class msrFrame : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------
      
    static SMARTP<msrFrame> create (
      int inputLineNumber,
      int frameStringsNumber,
      int frameFretsNumber,
      int frameFirstFretNumber);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrFrame (
      int inputLineNumber,
      int frameStringsNumber,
      int frameFretsNumber,
      int frameFirstFretNumber);

    virtual ~msrFrame ();
  
  public:

    // set and get
    // ------------------------------------------------------

                
    int                   getFrameStringsNumber () const
                              { return fFrameStringsNumber; }

    int                   getFrameFretsNumber () const
                              { return fFrameFretsNumber; }

    int                   getFrameFirstFretNumber () const
                              { return fFrameFirstFretNumber; }

    const list<S_msrFrameNote>&
                          getFrameFrameNotesList ()
                              { return fFrameFrameNotesList; }
                                              
    const list<msrBarre>& getFrameBarresList ()
                              { return fFrameBarresList; }
                                              
    bool                  getFrameContainsFingerings () const
                              { return fFrameContainsFingerings; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendFrameNoteToFrame (
                            S_msrFrameNote frameNote);
    
  private:

    // private services
    // ------------------------------------------------------

    void                  appendBarreToFrame ( // JMI
                            msrBarre barre)
                              {
                                fFrameBarresList.push_back (barre);
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

    int                   fFrameStringsNumber;
    int                   fFrameFretsNumber;
    int                   fFrameFirstFretNumber;

    list<S_msrFrameNote>  fFrameFrameNotesList;

    list<msrBarre >       fFrameBarresList;
    
    // a barre start remains pending
    // until the matching stop is appended to the frame
    stack<S_msrFrameNote> fPendingBarreStartFrameNotes;
                              
    // optimizing computation
    bool                  fFrameContainsFingerings;
};
typedef SMARTP<msrFrame> S_msrFrame;
EXP ostream& operator<< (ostream& os, const S_msrFrame& elt);


} // namespace MusicXML2


#endif
