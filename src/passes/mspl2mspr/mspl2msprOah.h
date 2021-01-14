/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___mspl2msprOah___
#define ___mspl2msprOah___

#include "msrSegnosAndCodas.h"

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP mspl2msprOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mspl2msprOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMusicxml2mspr ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    mspl2msprOahGroup ();

    virtual               ~mspl2msprOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    void                  checkGroupOptionsConsistency () override;

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  initializeMusicxml2msprTraceOah ();
#endif

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMusicxml2msprHelp ();

    void                  printMusicxml2msprValues (unsigned int fieldWidth);

  public:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<mspl2msprOahGroup> S_mspl2msprOahGroup;
EXP ostream& operator<< (ostream& os, const S_mspl2msprOahGroup& elt);

EXP extern S_mspl2msprOahGroup gGlobalMusicxml2msprOahGroup;

//______________________________________________________________________________
EXP S_mspl2msprOahGroup createGlobalMusicxml2msprOahGroup ();


}


#endif
