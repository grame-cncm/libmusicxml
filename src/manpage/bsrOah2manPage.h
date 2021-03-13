/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrOah2manPage___
#define ___bsrOah2manPage___

#include "bsrOah.h"

#include "oah2manPage.h"


namespace MusicXML2
{

//________________________________________________________________________
class EXP bsrOah2manPage : virtual public oah2manPage,

  public visitor<S_bsrFacSimileKindAtom>,

  public visitor<S_bsrTextsLanguageAtom>

{
  public:

                          bsrOah2manPage (
                            const S_oahHandler handler,
                            ostream&           manPageOutputStream);

    virtual               ~bsrOah2manPage ();

  protected:

    virtual void visitStart (S_bsrFacSimileKindAtom& elt);
    virtual void visitEnd   (S_bsrFacSimileKindAtom& elt);

    virtual void visitStart (S_bsrTextsLanguageAtom& elt);
    virtual void visitEnd   (S_bsrTextsLanguageAtom& elt);
};
typedef SMARTP<bsrOah2manPage> S_bsrOah2manPage;


}


#endif
