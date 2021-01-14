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
class EXP msrIdentification : public msrElement
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

    virtual               ~msrIdentification ();

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

    // composers
    const list<string>&   getComposersList () const
                              { return fComposersList; }

    // lyricists
    const list<string>&   getLyricistsList () const
                              { return fLyricistsList; }

    // arrangers
    const list<string>&   getArrangersList () const
                              { return fArrangersList; }

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

    // rights
    const list<string>&   getRightsList () const
                              { return fRightsList; }

    // sources
    const list<string>&   getSourcesList () const
                              { return fSourcesList; }

    // relations
    const list<string>&   getRelationsList () const
                              { return fRelationsList; }

  public:

    // public services
    // ------------------------------------------------------

    // composers
    void                  appendComposer (
                            int    inputLineNumber,
                            string value);

    // lyricists
    void                  appendLyricist (
                            int    inputLineNumber,
                            string value);

    // arrangers
    void                  appendArranger (
                            int    inputLineNumber,
                            string value);

    // poets
    void                  appendPoet (
                            int    inputLineNumber,
                            string value);

    // translators
    void                  appendTranslator (
                            int    inputLineNumber,
                            string value);

    // artists
    void                  appendArtist (
                            int    inputLineNumber,
                            string value);

    // softwares
    void                  appendSoftware (
                            int    inputLineNumber,
                            string value);

    // rights
    void                  appendRight (
                            int    inputLineNumber,
                            string value);

    // rources
    void                  appendSource (
                            int    inputLineNumber,
                            string value);

    // relations
    void                  appendRelation (
                            int    inputLineNumber,
                            string value);

    unsigned int          maxIdentificationNamesLength ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // work
    string                fWorkNumber;
    string                fWorkTitle;
    string                fOpus;

    string                fMovementNumber;
    string                fMovementTitle;

    // encoding date
    string                fEncodingDate;

    // miscellaneous field
    string                fMiscellaneousField;

    // score instrument
    string                fScoreInstrument;

    // creators
    list<string>          fComposersList;
    list<string>          fLyricistsList;
    list<string>          fArrangersList;

    list<string>          fPoetsList;
    list<string>          fTranslatorsList;
    list<string>          fArtistsList;

    list<string>          fSoftwaresList;

    // rights
    list<string>          fRightsList;
    // sources
    list<string>          fSourcesList;

    // relations
    list<string>          fRelationsList;
};
typedef SMARTP<msrIdentification> S_msrIdentification;
EXP ostream& operator<< (ostream& os, const S_msrIdentification& elt);


} // namespace MusicXML2


#endif
