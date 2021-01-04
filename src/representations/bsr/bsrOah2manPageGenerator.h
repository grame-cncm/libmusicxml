/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrOah2manPageGenerator___
#define ___bsrOah2manPageGenerator___

#include "bsrOah.h"

#include "oah2manPageGenerators.h"


namespace MusicXML2
{

//________________________________________________________________________
class EXP bsrOah2manPageGenerator : virtual public oah2manPageGenerator,

  public visitor<S_bsrFacSimileKindAtom>,

  public visitor<S_bsrTextsLanguageAtom>

{
  public:

    bsrOah2manPageGenerator (
      const S_oahHandler handler,
      ostream&           manPageOutputStream);

    virtual ~bsrOah2manPageGenerator ();

  protected:

    virtual void visitStart (S_bsrFacSimileKindAtom& elt);
    virtual void visitEnd   (S_bsrFacSimileKindAtom& elt);

    virtual void visitStart (S_bsrTextsLanguageAtom& elt);
    virtual void visitEnd   (S_bsrTextsLanguageAtom& elt);
};
typedef SMARTP<bsrOah2manPageGenerator> S_bsrOah2manPageGenerator;


}


#endif
