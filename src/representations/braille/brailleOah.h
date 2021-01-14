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

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP brailleOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<brailleOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeBrailleOahGroup ();

    void                  printBrailleOahHelp ();

    void                  printBrailleOahValues (unsigned int fieldWidth);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    brailleOahGroup ();

    virtual               ~brailleOahGroup ();

    // set and get
    // ------------------------------------------------------

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceGroupQuietness () override;

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkGroupOptionsConsistency () override;

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  private:

    // private services
    // ------------------------------------------------------

  private:

    // private fields
    // --------------------------------------
};
typedef SMARTP<brailleOahGroup> S_brailleOahGroup;
EXP ostream& operator<< (ostream& os, const S_brailleOahGroup& elt);

EXP extern S_brailleOahGroup gGlobalBrailleOahGroup;

//______________________________________________________________________________
S_brailleOahGroup createGlobalBrailleOahGroup ();


}


#endif
