/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsrHeaders___
#define ___lpsrHeaders___

#include "msrBasicTypes.h"

#include "lpsrElements.h"

#include "lilypondOah.h"


namespace MusicXML2
{

//______________________________________________________________________________
class lpsrHeader : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrHeader> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrHeader (
      int inputLineNumber);

    virtual ~lpsrHeader ();

  public:

    // set and get
    // ------------------------------------------------------

    // MusicXML informations

    // work number
    void                  setWorkNumber (string value)
                              { fWorkNumber = value; }

    string                getWorkNumber () const
                              { return fWorkNumber; }

    // work title
    void                  setWorkTitle (string value)
                              { fWorkTitle = value; }

    string                getWorkTitle () const
                              { return fWorkTitle; }

    // opus
    void                  setOpus (string value)
                              { fOpus = value; }

    string                getOpus () const
                              { return fOpus; }

    // movement number
    void                  setMovementNumber (string value)
                              { fMovementNumber = value; }

    string                getMovementNumber () const
                              { return fMovementNumber; }

    // movement title
    void                  setMovementTitle (string value)
                              { fMovementTitle = value; }

    string                getMovementTitle () const
                              { return fMovementTitle; }

    // encoding date
    void                  setEncodingDate (string value)
                              { fEncodingDate = value; }

    string                getEncodingDate () const
                              { return fEncodingDate; }

    // miscellaneous field
    void                  setMiscellaneousField (string value)
                              { fMiscellaneousField = value; }

    string                getMiscellaneousField () const
                              { return fMiscellaneousField; }

    // score instrument
    void                  setScoreInstrument (string value)
                              { fScoreInstrument = value; }

    string                getScoreInstrument () const
                              { return fScoreInstrument; }

    // rights
    void                  setRights (
                            int               inputLineNumber,
                            string            val,
                            msrFontStyleKind  fontStyleKind,
                            msrFontWeightKind fontWeightKind);

    const list<string>&   getRightsList () const
                              { return fRightsList; }

    // composers
    void                  setComposers (
                            int               inputLineNumber,
                            string            val,
                            msrFontStyleKind  fontStyleKind,
                            msrFontWeightKind fontWeightKind);

    const list<string>&   getComposersList () const
                              { return fComposersList; }

    // arrangers
    void                  setArrangers (
                            int               inputLineNumber,
                            string            val,
                            msrFontStyleKind  fontStyleKind,
                            msrFontWeightKind fontWeightKind);

    const list<string>&   getArrangersList () const
                              { return fArrangersList; }

     // lyricists
   void                  setLyricists (
                            int               inputLineNumber,
                            string            val,
                            msrFontStyleKind  fontStyleKind,
                            msrFontWeightKind fontWeightKind);

    const list<string>&   getLyricistsList () const
                              { return fLyricistsList; }

    // poets
    void                  setPoets ( // not defined JMI
                            int               inputLineNumber,
                            string            val,
                            msrFontStyleKind  fontStyleKind,
                            msrFontWeightKind fontWeightKind);

    const list<string>&   getPoetsList () const
                              { return fPoetsList; }

    // translators
    void                  setTranslators (
                            int               inputLineNumber,
                            string            val,
                            msrFontStyleKind  fontStyleKind,
                            msrFontWeightKind fontWeightKind);

    const list<string>&   getTranslatorsList () const
                              { return fTranslatorsList; }

    // artists
    void                  setArtists (
                            int               inputLineNumber,
                            string            val,
                            msrFontStyleKind  fontStyleKind,
                            msrFontWeightKind fontWeightKind);

    const list<string>&   getArtistsList () const
                              { return fArtistsList; }

    // softwares
    void                  setSoftwares (
                            int               inputLineNumber,
                            string            val,
                            msrFontStyleKind  fontStyleKind,
                            msrFontWeightKind fontWeightKind);

    const list<string>&   getSoftwaresList () const
                              { return fSoftwaresList; }

    // LilyPond informations

    // centered

    void                  setLilypondDedication (string value)
                              { fLilypondDedication = value; }

    string                getLilypondDedication () const
                              { return fLilypondDedication; }

    void                  setLilypondPiece (string value)
                              { fLilypondPiece = value; }

    string                getLilypondPiece () const
                              { return fLilypondPiece; }

    void                  setLilypondOpus (string value)
                              { fLilypondOpus = value; }

    string                getLilypondOpus () const
                              { return fLilypondOpus; }

    void                  setLilypondTitle (string value)
                              { fLilypondTitle = value; }

    string                getLilypondTitle () const
                              { return fLilypondTitle; }

    void                  setLilypondSubTitle (string value)
                              { fLilypondSubTitle = value; }

    string                getLilypondSubTitle () const
                              { return fLilypondSubTitle; }

    void                  setLilypondSubSubTitle (string value)
                              { fLilypondSubSubTitle = value; }

    string                getLilypondSubSubTitle () const
                              { return fLilypondSubSubTitle; }

    // evenly spread on one line
    // "instrument" also appears on following pages

    void                  setLilypondInstrument (string value)
                              { fLilypondInstrument = value; }

    string                getLilypondInstrument () const
                              { return fLilypondInstrument; }

    // at opposite ends of the same line

    void                  setLilypondMeter (string value)
                              { fLilypondMeter = value; }

    string                getLilypondMeter () const
                              { return fLilypondMeter; }

    // centered at the bottom of the first page

    void                  setLilypondCopyright (string value)
                              { fLilypondCopyright = value; }

    string                getLilypondCopyright () const
                              { return fLilypondCopyright; }

    // centered at the bottom of the last page

    void                  setLilypondTagline (string value)
                              { fLilypondTagline = value; }

    string                getLilypondTagline () const
                              { return fLilypondTagline; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    void                  appendRights (
                            int    inputLineNumber,
                            string value);

    void                  appendComposer (
                            int    inputLineNumber,
                            string value);

    void                  appendArranger (
                            int    inputLineNumber,
                            string value);

    void                  appendLyricist (
                            int    inputLineNumber,
                            string value);

    void                  appendPoet (
                            int    inputLineNumber,
                            string value);
    void                  removeAllPoets (
                            int    inputLineNumber);

    void                  appendTranslator (
                            int    inputLineNumber,
                            string value);

    void                  appendArtist (
                            int    inputLineNumber,
                            string value);

    void                  appendSoftware (
                            int    inputLineNumber,
                            string value);

    int                   maxLilypondVariablesNamesLength ();

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

    virtual void          printShort (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // MusicXML informations

    string                fWorkNumber;
    string                fWorkTitle;
    string                fOpus;

    string                fMovementNumber;
    string                fMovementTitle;

    list<string>          fRightsList;

    list<string>          fComposersList;
    list<string>          fArrangersList;
    list<string>          fLyricistsList;
    list<string>          fPoetsList;
    list<string>          fTranslatorsList;
    list<string>          fArtistsList;

    list<string>          fSoftwaresList;

    string                fEncodingDate;

    string                fScoreInstrument;

    string                fMiscellaneousField;

    // Lilypond informations

    // centered
    string                fLilypondDedication;

    string                fLilypondPiece;
    string                fLilypondOpus;

    string                fLilypondTitle;
    string                fLilypondSubTitle;
    string                fLilypondSubSubTitle;

    // evenly spread on one line
    // "instrument" also appears on following pages
    string                fLilypondInstrument;

    // at opposite ends of the same line
    string                fLilypondMeter;

    // centered at the bottom of the first page
    string                fLilypondCopyright;

    // centered at the bottom of the last page
    string                fLilypondTagline;
};
typedef SMARTP<lpsrHeader> S_lpsrHeader;
EXP ostream& operator<< (ostream& os, const S_lpsrHeader& elt);


}


#endif
