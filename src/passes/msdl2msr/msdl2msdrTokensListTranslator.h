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

#include "msdlTokens.h"

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

    S_msdrTokensList      getMsdlTokensList () const
                              { return fMsdlTokensList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  buildMsdlTokensList ();

  private:

    // private fields
    // ------------------------------------------------------

    S_msdrTokensList      fMsdlTokensList;
};


}

#endif
