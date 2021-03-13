/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___oah2manPage___
#define ___oah2manPage___

#include "enableExtraOahIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "extraOah.h"
#endif

#include "oahVisitor.h"


namespace MusicXML2
{

//________________________________________________________________________
class EXP oah2manPage : public oahVisitor
{
  public:

    oah2manPage (
      const S_oahHandler handler,
      ostream&           manPageOutputStream);

    virtual               ~oah2manPage ();

  protected:

    ostream&              fManPageOutputStream;
};
typedef SMARTP<oah2manPage> S_oah2manPage;


}


#endif
