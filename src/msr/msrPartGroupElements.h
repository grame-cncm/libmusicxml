/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrPartGroupElements___
#define ___msrPartGroupElements___

#include "msrSegnosAndCodas.h"


namespace MusicXML2
{

//______________________________________________________________________________
/*
  Parts and part groups can be found in part groups,
  hence class msrPartGroupElement
*/

class msrPartGroupElement : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    // cloning
    // ------------------------------------------------------

  protected:

    msrPartGroupElement (
      int inputLineNumber);

    virtual ~msrPartGroupElement ();

  /*
    The part group uplink is declared in the sub-classes,
    to allow for separate *.h files, C++ constraint
  */
};
typedef SMARTP<msrPartGroupElement> S_msrPartGroupElement;
EXP ostream& operator<< (ostream& os, const S_msrPartGroupElement& elt);


} // namespace MusicXML2


#endif
