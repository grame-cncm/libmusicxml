/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsrGeometry___
#define ___lpsrGeometry___

#include "lpsrElements.h"

#include "msrGeometry.h"


namespace MusicXML2
{

//______________________________________________________________________________
class lpsrGeometry : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrGeometry> create (
      int           inputLineNumber,
      S_msrGeometry theMsrGeometry);

    SMARTP<lpsrGeometry> createLpsrGeometryNewbornClone ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrGeometry (
      int           inputLineNumber,
      S_msrGeometry theMsrGeometry);

    virtual ~lpsrGeometry ();

  public:

    // set and get
    // ------------------------------------------------------

    // MSR geometry
    S_msrGeometry         getMsrGeometry () const
                              { return fMsrGeometry; }

    // LilyPond geometry ???

    // spaces
    void                  setBetweenSystemSpace (S_msrLength val)
                              { fBetweenSystemSpace = val; }
    S_msrLength           getBetweenSystemSpace () const
                              { return fBetweenSystemSpace; }

    void                  setPageTopSpace (S_msrLength val)
                              { fPageTopSpace = val; }
    S_msrLength           getPageTopSpace () const
                              { return fPageTopSpace; }


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

    // MSR geometry
    S_msrGeometry         fMsrGeometry;

    // LilyPond geometry JMI ???

    // spaces
    S_msrLength           fBetweenSystemSpace;
    S_msrLength           fPageTopSpace;
};
typedef SMARTP<lpsrGeometry> S_lpsrGeometry;
EXP ostream& operator<< (ostream& os, const S_lpsrGeometry& elt);


} // namespace MusicXML2


#endif
