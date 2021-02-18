/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdrScannerHandler___
#define ___msdrScannerHandler___

#include "waeHandlers.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
class EXP msdlScannerWaeHandler : public waeHandler
{
/* this class is purely virtual
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlScannerWaeHandler> create ();
*/

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlScannerWaeHandler ();

    virtual               ~msdlScannerWaeHandler ();

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
typedef SMARTP<msdlScannerWaeHandler> S_msdlScannerWaeHandler;
EXP ostream& operator<< (ostream& os, const S_msdlScannerWaeHandler& elt);

//________________________________________________________________________
class EXP msdlScannerEnglishWaeHandler : public msdlScannerWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlScannerEnglishWaeHandler> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlScannerEnglishWaeHandler ();

    virtual               ~msdlScannerEnglishWaeHandler ();

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
typedef SMARTP<msdlScannerEnglishWaeHandler> S_msdlScannerEnglishWaeHandler;
EXP ostream& operator<< (ostream& os, const S_msdlScannerEnglishWaeHandler& elt);

//________________________________________________________________________
class EXP msdlScannerFrenchWaeHandler : public msdlScannerWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlScannerFrenchWaeHandler> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlScannerFrenchWaeHandler ();

    virtual               ~msdlScannerFrenchWaeHandler ();

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
typedef SMARTP<msdlScannerFrenchWaeHandler> S_msdlScannerFrenchWaeHandler;
EXP ostream& operator<< (ostream& os, const S_msdlScannerFrenchWaeHandler& elt);


}


#endif
