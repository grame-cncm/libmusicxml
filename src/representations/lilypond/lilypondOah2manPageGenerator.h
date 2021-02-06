/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lilypondOah2manPageGenerator___
#define ___lilypondOah2manPageGenerator___

#include "lilypondOah.h"

#include "oah2manPageGenerators.h"


namespace MusicXML2
{

//________________________________________________________________________
class EXP lilypondOah2manPageGenerator : virtual public oah2manPageGenerator

// JMI  public visitor<S_lilypondTransposePartNameAtom>

{
  public:

    lilypondOah2manPageGenerator (
                            const S_oahHandler handler,
                            ostream&           manPageOutputStream);

    virtual               ~lilypondOah2manPageGenerator ();

  protected:

// JMI    virtual void visitStart (S_lilypondTransposePartNameAtom& elt);
//    virtual void visitEnd   (S_lilypondTransposePartNameAtom& elt);

};
typedef SMARTP<lilypondOah2manPageGenerator> S_lilypondOah2manPageGenerator;


}


#endif
