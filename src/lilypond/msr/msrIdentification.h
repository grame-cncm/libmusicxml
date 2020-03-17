/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrIdentification___
#define ___msrIdentification___

#include "msrElements.h"

#include "msrBasicTypes.h"

#include "msrVarValAssocs.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msrIdentification : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrIdentification> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrIdentification (
      int inputLineNumber);

    virtual ~msrIdentification ();

  public:

    // set and get
    // ------------------------------------------------------

    // work number
    void                  setWorkNumber (
                            int    inputLineNumber,
                            string val);

    S_msrVarValAssoc      getWorkNumber () const
                              { return fWorkNumber; }

    // work title
    void                  setWorkTitle (
                            int    inputLineNumber,
                            string val);

    S_msrVarValAssoc      getWorkTitle () const
                              { return fWorkTitle; }

    // opus
    void                  setOpus (
                            int    inputLineNumber,
                            string val);

    S_msrVarValAssoc      getOpus () const
                              { return fOpus; }

    // movement number
    void                  setMovementNumber (
                            int    inputLineNumber,
                            string val);

    S_msrVarValAssoc      getMovementNumber () const
                              { return fMovementNumber; }

    // movement title
    void                  setMovementTitle (
                            int    inputLineNumber,
                            string val);

    S_msrVarValAssoc      getMovementTitle () const
                              { return fMovementTitle; }

    // encoding date
    void                  setEncodingDate (
                            int    inputLineNumber,
                            string val);

    S_msrVarValAssoc      getEncodingDate () const
                              { return fEncodingDate; }

    // miscellaneous field
    void                  setMiscellaneousField (
                            int    inputLineNumber,
                            string val);

    S_msrVarValAssoc      getMiscellaneousField () const
                              { return fMiscellaneousField; }

    // score instrument
    void                  setScoreInstrument (
                            int    inputLineNumber,
                            string val);

    S_msrVarValAssoc      getScoreInstrument () const
                              { return fScoreInstrument; }

    // rights
    S_msrVarValsListAssoc getRights () const
                              { return fRights; }

    // composers
    S_msrVarValsListAssoc getComposers () const
                              { return fComposers; }

    // arrangers
    S_msrVarValsListAssoc getArrangers () const
                              { return fArrangers; }

    // lyricists
    S_msrVarValsListAssoc getLyricists () const
                              { return fLyricists; }

    // poets
    S_msrVarValsListAssoc getPoets () const
                              { return fPoets; }

    // translators
    S_msrVarValsListAssoc getTranslators () const
                              { return fTranslators; }

    // artists
    S_msrVarValsListAssoc getArtists () const
                              { return fArtists; }

    // softwares
    S_msrVarValsListAssoc getSoftwares () const
                              { return fSoftwares; }

    // services
    // ------------------------------------------------------

    // rights
    void                  addRights (
                            int    inputLineNumber,
                            string value);

    // composers
    void                  addComposer (
                            int    inputLineNumber,
                            string value);

    // arrangers
    void                  addArranger (
                            int    inputLineNumber,
                            string value);

    // lyricists
    void                  addLyricist (
                            int    inputLineNumber,
                            string value);

    // poets
    void                  addPoet (
                            int    inputLineNumber,
                            string value);

    // translators
    void                  addTranslator (
                            int    inputLineNumber,
                            string value);

    // artists
    void                  addArtist (
                            int    inputLineNumber,
                            string value);

    // softwares
    void                  addSoftware (
                            int    inputLineNumber,
                            string value);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    S_msrVarValsListAssoc fRights;

    S_msrVarValAssoc      fWorkNumber;
    S_msrVarValAssoc      fWorkTitle;
    S_msrVarValAssoc      fOpus;

    S_msrVarValAssoc      fMovementNumber;
    S_msrVarValAssoc      fMovementTitle;

    S_msrVarValsListAssoc fComposers;
    S_msrVarValsListAssoc fArrangers;
    S_msrVarValsListAssoc fLyricists;
    S_msrVarValsListAssoc fPoets;
    S_msrVarValsListAssoc fTranslators;
    S_msrVarValsListAssoc fArtists;

    S_msrVarValsListAssoc fSoftwares;

    S_msrVarValAssoc      fEncodingDate;

    S_msrVarValAssoc      fMiscellaneousField;

    S_msrVarValAssoc      fScoreInstrument;
};
typedef SMARTP<msrIdentification> S_msrIdentification;
EXP ostream& operator<< (ostream& os, const S_msrIdentification& elt);


} // namespace MusicXML2


#endif
