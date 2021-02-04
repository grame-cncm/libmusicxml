/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrOah2manPageGenerator___
#define ___msrOah2manPageGenerator___

#include "msrOah.h"
#include "msr2msrOah.h"

#include "oah2manPageGenerators.h"


namespace MusicXML2
{

//________________________________________________________________________
class EXP msrOah2manPageGenerator : virtual public oah2manPageGenerator,

  public visitor<S_msrRenamePartAtom>,

  public visitor<S_msrOmitPartAtom>,

  public visitor<S_msrKeepPartAtom>,

  public visitor<S_msrPitchesLanguageAtom>

{
  public:

                          msrOah2manPageGenerator (
                            const S_oahHandler handler,
                            ostream&           manPageOutputStream);

    virtual               ~msrOah2manPageGenerator ();

  protected:

    virtual void visitStart (S_msrRenamePartAtom& elt);
    virtual void visitEnd   (S_msrRenamePartAtom& elt);

    virtual void visitStart (S_msrOmitPartAtom& elt);
    virtual void visitEnd   (S_msrOmitPartAtom& elt);

    virtual void visitStart (S_msrKeepPartAtom& elt);
    virtual void visitEnd   (S_msrKeepPartAtom& elt);

    virtual void visitStart (S_msrPitchesLanguageAtom& elt);
    virtual void visitEnd   (S_msrPitchesLanguageAtom& elt);
};
typedef SMARTP<msrOah2manPageGenerator> S_msrOah2manPageGenerator;


}


#endif
