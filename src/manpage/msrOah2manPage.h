/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrOah2manPage___
#define ___msrOah2manPage___

#include "msrOah.h"
#include "msr2msrOah.h"

#include "oah2manPage.h"


namespace MusicXML2
{

//________________________________________________________________________
class EXP msrOah2manPage : virtual public oah2manPage,

  public visitor<S_msrRenamePartAtom>,

  public visitor<S_msrOmitPartAtom>,

  public visitor<S_msrKeepPartAtom>,

  public visitor<S_msrPitchesLanguageAtom>

{
  public:

                          msrOah2manPage (
                            const S_oahHandler handler,
                            ostream&           manPageOutputStream);

    virtual               ~msrOah2manPage ();

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
typedef SMARTP<msrOah2manPage> S_msrOah2manPage;


}


#endif
