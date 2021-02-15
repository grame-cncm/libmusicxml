/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdrLexicalWaeHandler___
#define ___msdrLexicalWaeHandler___

#include <list>

#include "waeHandlers.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
class EXP msdlLexicalWaeHandler : public waeHandler
{
/* this class is purely virtual
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlLexicalWaeHandler> create ();
*/

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlLexicalWaeHandler ();

    virtual               ~msdlLexicalWaeHandler ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    virtual void          illegalCharacter (char theChar) = 0;
    virtual void          illegalCharacters (string theString) = 0;

    virtual void          digitExpectedAfterDecimalDot (char theChar) = 0;
    virtual void          digitExpectedInExponent (char theChar) = 0;

    virtual void          illegalEscapedCharacterInString (char theChar) = 0;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<msdlLexicalWaeHandler> S_msdlLexicalWaeHandler;
EXP ostream& operator<< (ostream& os, const S_msdlLexicalWaeHandler& elt);


}


#endif
