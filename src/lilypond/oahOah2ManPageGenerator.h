/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___oahOah2ManPageGenerator___
#define ___oahOah2ManPageGenerator___

#include "oahOah.h"

#include "oahVisitor.h"


namespace MusicXML2
{

//________________________________________________________________________
class oahOah2ManPageGenerator :

  public visitor<S_oahOptionalValuesStyleKindAtom>

{
  public:

    oahOah2ManPageGenerator (
      indentedOstream&   logOstream,
      ostream&           manPageOutputStream);

    virtual ~oahOah2ManPageGenerator ();

  protected:

    virtual void visitStart (S_oahOptionalValuesStyleKindAtom& elt);
    virtual void visitEnd   (S_oahOptionalValuesStyleKindAtom& elt);

  protected:

    indentedOstream&      fOahOah2ManPageLogOstream;
    ostream&              fOahOah2ManPageOutputStream;
};
typedef SMARTP<oahOah2ManPageGenerator> S_oahOah2ManPageGenerator;


}


#endif
