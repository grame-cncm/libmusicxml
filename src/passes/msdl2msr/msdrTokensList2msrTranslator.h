/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdrTokensList2msrTranslator___
#define ___msdrTokensList2msrTranslator___

#include "msdlTokens.h"


namespace MusicXML2
{

//________________________________________________________________________
class EXP msdrTokensList2msrTranslator
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdrTokensList2msrTranslator (
                            S_msdrTokensList tokensList);

    virtual               ~msdrTokensList2msrTranslator ();

    // set and get
    // ------------------------------------------------------

    S_msdrTokensList      getMsrTokensList () const
                              { return fMsrTokensList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  createMsrFromMsdl ();

    // visitors
    // ------------------------------------------------------

  private:


    // the MSR score we are populating
    // ------------------------------------------------------

    S_msdrTokensList      fMsrTokensList;
};


}


#endif
