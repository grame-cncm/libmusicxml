/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___brailleOah2ManPageGenerator___
#define ___brailleOah2ManPageGenerator___

#include "brailleOah.h"

#include "oah2ManPageGenerators.h"


namespace MusicXML2
{

//________________________________________________________________________
class brailleOah2ManPageGenerator : virtual public oah2ManPageGenerator,

  public visitor<S_brailleOutputKindAtom>,

  public visitor<S_brailleUTFKindAtom>,

  public visitor<S_brailleByteOrderingKindAtom>

{
  public:

    brailleOah2ManPageGenerator (
      const S_oahHandler handler,
      indentedOstream&   logOstream,
      ostream&           manPageOutputStream);

    virtual ~brailleOah2ManPageGenerator ();

  protected:

    virtual void visitStart (S_brailleOutputKindAtom& elt);
    virtual void visitEnd   (S_brailleOutputKindAtom& elt);

    virtual void visitStart (S_brailleUTFKindAtom& elt);
    virtual void visitEnd   (S_brailleUTFKindAtom& elt);

    virtual void visitStart (S_brailleByteOrderingKindAtom& elt);
    virtual void visitEnd   (S_brailleByteOrderingKindAtom& elt);
};
typedef SMARTP<brailleOah2ManPageGenerator> S_brailleOah2ManPageGenerator;


}


#endif
