/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___oah2ManPageGenerator___
#define ___oah2ManPageGenerator___

#include "bsr.h"

#include "manPageOah.h"


namespace MusicXML2
{

//________________________________________________________________________
class oah2ManPageGenerator :

  public visitor<S_oahHandler>,

  public visitor<S_oahGroup>,

  public visitor<S_oahSubGroup>,

  public visitor<S_oahAtom>,
  public visitor<S_oahAtomWithVariableName>,

  public visitor<S_bsrPage>,

  public visitor<S_bsrPagination>,

  public visitor<S_bsrPageHeading>,
  public visitor<S_bsrMusicHeading>,

  public visitor<S_bsrFootNotes>,

  public visitor<S_bsrLine>,
  public visitor<S_bsrLineContents>,

  public visitor<S_bsrMeasure>,

  public visitor<S_bsrNumber>,
  public visitor<S_bsrWords>,

  public visitor<S_bsrClef>,
  public visitor<S_bsrKey>,
  public visitor<S_bsrTime>,

  public visitor<S_bsrTempo>,

  public visitor<S_bsrNote>

{
  public:

    oah2ManPageGenerator (
      const S_oahHandler handler,
      S_manPageOah&      manPageOpts,
      indentedOstream&   logOstream,
      ostream&            manPageOutputStream);

    virtual ~oah2ManPageGenerator ();

    void generateManPageFromOahHandler ();

  protected:

    virtual void visitStart (S_oahHandler& elt);
    virtual void visitEnd   (S_oahHandler& elt);

    virtual void visitStart (S_oahGroup& elt);
    virtual void visitEnd   (S_oahGroup& elt);

    virtual void visitStart (S_oahSubGroup& elt);

    virtual void visitStart (S_oahAtom& elt);
    virtual void visitEnd   (S_oahAtom& elt);

    virtual void visitStart (S_oahAtomWithVariableName& elt);
    virtual void visitEnd   (S_oahAtomWithVariableName& elt);

    virtual void visitStart (S_oahStringWithDefaultValueAtom& elt);
    virtual void visitEnd   (S_oahStringWithDefaultValueAtom& elt);

  private:

    S_oahHandler          fVisitedOahHandler;

    S_manPageOah          fManPageOpts;

    indentedOstream&      fLogOutputStream;
};


}


#endif
