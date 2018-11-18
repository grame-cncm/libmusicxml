/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrTranscriptionNotes___
#define ___bsrTranscriptionNotes___

#include "bsrElements.h"

#include "bsrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class bsrTranscriptionNotesElement : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrTranscriptionNotesElement> create (
      int    inputLineNumber,
      string transcriptionNoteText);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrTranscriptionNotesElement (
      int    inputLineNumber,
      string transcriptionNoteText);
      
    virtual ~bsrTranscriptionNotesElement ();
  
  public:

    // set and get
    // ------------------------------------------------------

    string                getTranscriptionNoteText () const
                              { return fTranscriptionNoteText; }

  public:

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

    string                fTranscriptionNoteText;
};
typedef SMARTP<bsrTranscriptionNotesElement> S_bsrTranscriptionNotesElement;
EXP ostream& operator<< (ostream& os, const S_bsrTranscriptionNotesElement& elt);

//______________________________________________________________________________
class bsrTranscriptionNotes : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrTranscriptionNotes> create (
      int inputLineNumber);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrTranscriptionNotes (
      int inputLineNumber);
      
    virtual ~bsrTranscriptionNotes ();
  
  public:

    // set and get
    // ------------------------------------------------------

  public:

    // services
    // ------------------------------------------------------

    void                  appendNoteElementToTranscriptionNotes (
                            S_bsrTranscriptionNotesElement transcriptionNotesElement)
                              {
                                fTranscriptionNotesElementsList.push_back (
                                  transcriptionNotesElement);
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

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    list<S_bsrTranscriptionNotesElement>
                          fTranscriptionNotesElementsList;
};
typedef SMARTP<bsrTranscriptionNotes> S_bsrTranscriptionNotes;
EXP ostream& operator<< (ostream& os, const S_bsrTranscriptionNotes& elt);


} // namespace MusicXML2


#endif
