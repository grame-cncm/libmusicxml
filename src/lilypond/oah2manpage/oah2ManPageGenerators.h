/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___oah2ManPageGenerator___
#define ___oah2ManPageGenerator___

#include "extraOah.h"

#include "oahVisitor.h"


namespace MusicXML2
{

//________________________________________________________________________
class oah2ManPageGenerator : public oahVisitor
{
  public:

    oah2ManPageGenerator (
      const S_oahHandler handler,
      indentedOstream&   logOstream,
      ostream&           manPageOutputStream);

    virtual ~oah2ManPageGenerator ();

  protected:

  protected:

    indentedOstream&      fOah2ManPageLogOstream;
    ostream&              fOah2ManPageOutputStream;
};
typedef SMARTP<oah2ManPageGenerator> S_oah2ManPageGenerator;


}


#endif
