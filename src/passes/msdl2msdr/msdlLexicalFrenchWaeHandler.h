/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdrLexicalFrenchHandler___
#define ___msdrLexicalFrenchHandler___

#include <list>

#include "smartpointer.h"

#include "msdrKeywords.h"

#include "msdlLexicalWaeHandler.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
class EXP msdlLexicalFrenchWaeHandler : public msdlLexicalWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlLexicalFrenchWaeHandler> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlLexicalFrenchWaeHandler ();

    virtual               ~msdlLexicalFrenchWaeHandler ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  illegalCharacter (char theChar) override;
    void                  illegalCharacters (string theString) override;

    void                  digitExpectedAfterDecimalDot (char theChar) override;
    void                  digitExpectedInExponent (char theChar) override;

    void                  illegalEscapedCharacterInString (char theChar) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<msdlLexicalFrenchWaeHandler> S_msdlLexicalFrenchWaeHandler;
EXP ostream& operator<< (ostream& os, const S_msdlLexicalFrenchWaeHandler& elt);


}


#endif
