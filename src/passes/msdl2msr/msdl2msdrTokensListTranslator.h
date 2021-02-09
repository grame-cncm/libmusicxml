/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdl2msdrTokensListTranslator___
#define ___msdl2msdrTokensListTranslator___

#include "msdrTokens.h"

//#include "msdrTokensList2msrOah.h"


namespace MusicXML2
{

//________________________________________________________________________
class EXP msdl2msdrTokensListTranslator
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2msdrTokensListTranslator ();

    virtual               ~msdl2msdrTokensListTranslator ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msdrTokensList      getMsdrTokensList () const
                              { return fMsdrTokensList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  buildMsdrTokensList ();

  private:

    // private fields
    // ------------------------------------------------------

    S_msdrTokensList      fMsdrTokensList;
};


}

#endif
