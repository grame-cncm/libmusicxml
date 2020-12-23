/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrVoiceElements___
#define ___msrVoiceElements___

#include "msrElements.h"


namespace MusicXML2
{

//______________________________________________________________________________
/*
  Various elements can found in voices,
  hence class msrVoiceElement
*/

class EXP msrVoiceElement : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    // cloning
    // ------------------------------------------------------

  protected:

    msrVoiceElement (
      int inputLineNumber);

    virtual ~msrVoiceElement ();

  /*
    The voice uplink is declared in the sub-classes,
    to allow for separate *.h files, C++ constraint
  */
};
typedef SMARTP<msrVoiceElement> S_msrVoiceElement;
EXP ostream& operator<< (ostream& os, const S_msrVoiceElement& elt);


} // namespace MusicXML2


#endif
