/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lilypondOah2ManPageGenerator___
#define ___lilypondOah2ManPageGenerator___

#include "lilypondOah.h"

#include "oah2ManPageGenerators.h"


namespace MusicXML2
{

//________________________________________________________________________
class lilypondOah2ManPageGenerator : virtual public oah2ManPageGenerator
{
  public:

    lilypondOah2ManPageGenerator (
      const S_oahHandler handler,
      indentedOstream&   logOstream,
      ostream&           manPageOutputStream);

    virtual ~lilypondOah2ManPageGenerator ();

  protected:

};
typedef SMARTP<lilypondOah2ManPageGenerator> S_lilypondOah2ManPageGenerator;


}


#endif
