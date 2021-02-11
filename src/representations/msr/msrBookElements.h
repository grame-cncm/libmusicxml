/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrBookElements___
#define ___msrBookElements___

#include "msrElements.h"


namespace MusicXML2
{

//______________________________________________________________________________
/*
  Parts and part groups can be found in part groups,
  hence class msrBookElement
*/

class EXP msrBookElement : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    // cloning
    // ------------------------------------------------------

  protected:

                          msrBookElement (
                            int inputLineNumber);

    virtual               ~msrBookElement ();

  /*
    The part group uplink is declared in the sub-classes,
    to allow for separate *.h files, C++ constraint
  */
};
typedef SMARTP<msrBookElement> S_msrBookElement;
EXP ostream& operator<< (ostream& os, const S_msrBookElement& elt);


} // namespace MusicXML2


#endif
