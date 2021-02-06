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
#include "msrIdentification.h"

#include "lpsrElements.h"

#include "lilypondOah.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP lpsrHeader : public lpsrElement
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

    virtual               ~lpsrHeader ();

  public:

    // set and get
    // ------------------------------------------------------

    // identification

    void                  setHeaderIdentification (S_msrIdentification value)
                              { fHeaderIdentification = value; }

    S_msrIdentification   getHeaderIdentification () const
                              { return fHeaderIdentification; }

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

    unsigned int          maxLilypondVariablesNamesLength ();

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

    void                  printShort (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // identification

    S_msrIdentification   fHeaderIdentification;

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
