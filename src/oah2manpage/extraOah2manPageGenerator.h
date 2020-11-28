/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___extraOah2manPageGenerator___
#define ___extraOah2manPageGenerator___

#include "enableExtraOahIfDesired.h"

#ifdef EXTRA_OAH_IS_ENABLED

#include "extraOah.h"

#include "oah2manPageGenerators.h"


namespace MusicXML2
{

//________________________________________________________________________
class extraOah2manPageGenerator : virtual public oah2manPageGenerator,

  public visitor<S_extraShowAllHarmoniesStructuresAtom>,

  public visitor<S_extraShowAllHarmoniesContentsAtom>,

  public visitor<S_extraShowHarmonyDetailsAtom>,

  public visitor<S_extraShowHarmonyAnalysisAtom>

{
  public:

    extraOah2manPageGenerator (
      const S_oahHandler handler,
      ostream&           manPageOutputStream);

    virtual ~extraOah2manPageGenerator ();

  protected:

    virtual void visitStart (S_extraShowAllHarmoniesStructuresAtom& elt);
    virtual void visitEnd   (S_extraShowAllHarmoniesStructuresAtom& elt);

    virtual void visitStart (S_extraShowAllHarmoniesContentsAtom& elt);
    virtual void visitEnd   (S_extraShowAllHarmoniesContentsAtom& elt);

    virtual void visitStart (S_extraShowHarmonyDetailsAtom& elt);
    virtual void visitEnd   (S_extraShowHarmonyDetailsAtom& elt);

    virtual void visitStart (S_extraShowHarmonyAnalysisAtom& elt);
    virtual void visitEnd   (S_extraShowHarmonyAnalysisAtom& elt);
};
typedef SMARTP<extraOah2manPageGenerator> S_extraOah2manPageGenerator;


}


#endif

#endif
