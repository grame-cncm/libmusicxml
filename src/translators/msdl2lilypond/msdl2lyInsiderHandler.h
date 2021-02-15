/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdl2lyInsiderHandler___
#define ___msdl2lyInsiderHandler___

#include "enableExtraOahIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "extraOah.h"
#endif

#include "oahAtomsCollection.h"

#include "oahInsiderHandlers.h"


namespace MusicXML2
{
//_______________________________________________________________________________
class EXP msdl2lyInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2lyInsiderHandler> create (
                            string                 executableName,
                            string                 handlerHeader,
                            oahHandlerUsedThruKind handlerUsedThruKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2lyInsiderHandler (
                            string                 executableName,
                            string                 handlerHeader,
                            oahHandlerUsedThruKind handlerUsedThruKind);

    virtual               ~msdl2lyInsiderHandler ();

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheMsdl2lyPrefixes ();

    void                  createTheMsdl2lyOptionGroups (
                            string executableName);

  public:

    // public services
    // ------------------------------------------------------

    string                handlerExecutableAboutInformation () const override;

    void                  checkOptionsAndArgumentsFromArgcAndArgv () const override;

    string                fetchOutputFileNameFromTheOptions () const override;

    // quiet mode
    void                  enforceHandlerQuietness () override;

    // consistency check
    void                  checkHandlerOptionsConsistency () override;

  public:

    // visitors
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

  private:

    // private methods
    // ------------------------------------------------------

    string               usageFromUsedThruKind (
                           oahHandlerUsedThruKind handlerUsedThruKind) const;
  private:

    // private fields
    // ------------------------------------------------------

    S_oahPrefix           fShortIgnoreRedundantPrefix;
    S_oahPrefix           fLongIgnoreRedundantPrefix;
};
typedef SMARTP<msdl2lyInsiderHandler> S_msdl2lyInsiderHandler;
EXP ostream& operator<< (ostream& os, const S_msdl2lyInsiderHandler& elt);

//______________________________________________________________________________
class EXP msdl2lyInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<msdl2lyInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdl2lyInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2lyInsiderOahGroup ();

    virtual               ~msdl2lyInsiderOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // quit after some passes
    bool                  getQuitAfterPass2a () const
                              { return fQuitAfterPass2a; }
    bool                  getQuitAfterPass2b () const
                              { return fQuitAfterPass2b; }

  public:

    // public services
    // ------------------------------------------------------

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    void                  checkGroupOptionsConsistency () override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsdl2lyInsiderOahGroupHelp ();

    void                  printMsdl2lyInsiderOahGroupValues (
                            unsigned int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createInsiderQuitSubGroup ();

  private:

    // private fields
    // ------------------------------------------------------

    // quit after some passes
    bool                  fQuitAfterPass2a;
    bool                  fQuitAfterPass2b;
};
typedef SMARTP<msdl2lyInsiderOahGroup> S_msdl2lyInsiderOahGroup;
EXP ostream& operator<< (ostream& os, const S_msdl2lyInsiderOahGroup& elt);

EXP extern S_msdl2lyInsiderOahGroup gGlobalMsdl2lyInsiderOahGroup;

//______________________________________________________________________________
S_msdl2lyInsiderOahGroup createGlobalMsdl2lyInsiderOahGroup ();


}


#endif
