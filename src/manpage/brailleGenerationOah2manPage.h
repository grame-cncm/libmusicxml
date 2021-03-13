/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___brailleGenerationOah2manPage___
#define ___brailleGenerationOah2manPage___

#include "oah2manPage.h"

#include "brailleGenerationOah.h"


namespace MusicXML2
{

//________________________________________________________________________
class EXP brailleGenerationOah2manPage : virtual public oah2manPage,

  public visitor<S_brailleOutputKindAtom>,

  public visitor<S_brailleUTFKindAtom>,

  public visitor<S_brailleByteOrderingKindAtom>

{
  public:

                          brailleGenerationOah2manPage (
                            const S_oahHandler handler,
                            ostream&           manPageOutputStream);

    virtual               ~brailleGenerationOah2manPage ();

  protected:

    virtual void visitStart (S_brailleOutputKindAtom& elt);
    virtual void visitEnd   (S_brailleOutputKindAtom& elt);

    virtual void visitStart (S_brailleUTFKindAtom& elt);
    virtual void visitEnd   (S_brailleUTFKindAtom& elt);

    virtual void visitStart (S_brailleByteOrderingKindAtom& elt);
    virtual void visitEnd   (S_brailleByteOrderingKindAtom& elt);
};
typedef SMARTP<brailleGenerationOah2manPage> S_brailleGenerationOah2manPage;


}


#endif
