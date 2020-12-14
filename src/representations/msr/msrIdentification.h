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

    string                getWorkNumber () const
                              { return fWorkNumber; }

    // work title
    void                  setWorkTitle (
                            int    inputLineNumber,
                            string val);

    string                getWorkTitle () const
                              { return fWorkTitle; }

    // opus
    void                  setOpus (
                            int    inputLineNumber,
                            string val);

    string                getOpus () const
                              { return fOpus; }

    // movement number
    void                  setMovementNumber (
                            int    inputLineNumber,
                            string val);

    string                getMovementNumber () const
                              { return fMovementNumber; }

    // movement title
    void                  setMovementTitle (
                            int    inputLineNumber,
                            string val);

    string                getMovementTitle () const
                              { return fMovementTitle; }

    // encoding date
    void                  setEncodingDate (
                            int    inputLineNumber,
                            string val);

    string                getEncodingDate () const
                              { return fEncodingDate; }

    // miscellaneous field
    void                  setMiscellaneousField (
                            int    inputLineNumber,
                            string val);

    string                getMiscellaneousField () const
                              { return fMiscellaneousField; }

    // score instrument
    void                  setScoreInstrument (
                            int    inputLineNumber,
                            string val);

    string                getScoreInstrument () const
                              { return fScoreInstrument; }

    // rights
    const list<string>&   getRightsList () const
                              { return fRightsList; }

    // composers
    const list<string>&   getComposersList () const
                              { return fComposersList; }

    // arrangers
    const list<string>&   getArrangersList () const
                              { return fArrangersList; }

    // lyricists
    const list<string>&   getLyricistsList () const
                              { return fLyricistsList; }

    // poets
    const list<string>&   getPoetsList () const
                              { return fPoetsList; }

    // translators
    const list<string>&   getTranslatorsList () const
                              { return fTranslatorsList; }

    // artists
    const list<string>&   getArtistsList () const
                              { return fArtistsList; }

    // softwares
    const list<string>&   getSoftwaresList () const
                              { return fSoftwaresList; }

  public:

    // public services
    // ------------------------------------------------------

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

    // rights
    void                  addRights (
                            int    inputLineNumber,
                            string value);

    // softwares
    void                  addSoftware (
                            int    inputLineNumber,
                            string value);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    virtual void          print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // identification work
    string                fWorkNumber;
    string                fWorkTitle;
    string                fOpus;

    string                fMovementNumber;
    string                fMovementTitle;

    // identification encoding
    string                fEncodingDate;

    // identification miscellaneous
    string                fMiscellaneousField;

    // score instrument
    string                fScoreInstrument;

    // identification creators
    list<string>          fComposersList;
    list<string>          fLyricistsList;
    list<string>          fArrangersList;

    list<string>          fPoetsList;
    list<string>          fTranslatorsList;
    list<string>          fArtistsList;

    list<string>          fSoftwaresList;

    // identification rights
    list<string>          fRightsList;
    // identification sources
    list<string>          fSourcesList;

    // identification relations
    list<string>          fRelationsList;
};
typedef SMARTP<msrIdentification> S_msrIdentification;
EXP ostream& operator<< (ostream& os, const S_msrIdentification& elt);


} // namespace MusicXML2


#endif
