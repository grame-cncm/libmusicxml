/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___brailleOah___
#define ___brailleOah___

#include <set>

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class brailleOah : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<brailleOah> create (
      S_oahHandler handlerUpLink);

    SMARTP<brailleOah>    createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeBrailleOah (
                            bool boolOptionsInitialValue);

    void                  printBrailleOahHelp ();

    void                  printBrailleOahValues (int fieldWidth);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    brailleOah (
      S_oahHandler handlerUpLink);

    virtual ~brailleOah ();

    // set and get
    // ------------------------------------------------------

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceQuietness ();

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkOptionsConsistency () override;

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  private:

    // private services
    // ------------------------------------------------------

  public:

    // fields
    // --------------------------------------
};
typedef SMARTP<brailleOah> S_brailleOah;
EXP ostream& operator<< (ostream& os, const S_brailleOah& elt);

EXP extern S_brailleOah gGlobalBrailleOah;
EXP extern S_brailleOah gGlobalBrailleOahUserChoices;
EXP extern S_brailleOah gGlobalBrailleOahWithDetailedTrace;

//______________________________________________________________________________
void initializeBrailleOahHandler (
  S_oahHandler handler);


}


#endif
