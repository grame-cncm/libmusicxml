/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdl2xmlInsiderOahGroup___
#define ___msdl2xmlInsiderOahGroup___

#include "oahBasicTypes.h"

#include "oahInsiderHandlers.h"


namespace MusicXML2
{
//_______________________________________________________________________________
class EXP msdl2xmlInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2xmlInsiderHandler> create (
                            string executableName,
                            string handlerHeader);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2xmlInsiderHandler (
                            string executableName,
                            string handlerHeader);

    virtual               ~msdl2xmlInsiderHandler ();

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheMsdl2xmlPrefixes ();

    void                  createTheMsdl2xmlOptionGroups (
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

    // private fields
    // ------------------------------------------------------

    S_oahPrefix           fShortIgnoreRedundantPrefix;
    S_oahPrefix           fLongIgnoreRedundantPrefix;
};
typedef SMARTP<msdl2xmlInsiderHandler> S_msdl2xmlInsiderHandler;
EXP ostream& operator<< (ostream& os, const S_msdl2xmlInsiderHandler& elt);

//______________________________________________________________________________
class EXP msdl2xmlInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<msdl2xmlInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdl2xmlInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2xmlInsiderOahGroup ();

    virtual               ~msdl2xmlInsiderOahGroup ();

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

    void                  printMsdl2xmlInsiderOahGroupHelp ();

    void                  printMsdl2xmlInsiderOahGroupValues (
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
typedef SMARTP<msdl2xmlInsiderOahGroup> S_msdl2xmlInsiderOahGroup;
EXP ostream& operator<< (ostream& os, const S_msdl2xmlInsiderOahGroup& elt);

EXP extern S_msdl2xmlInsiderOahGroup gGlobalMsdl2xmlInsiderOahGroup;

//______________________________________________________________________________
S_msdl2xmlInsiderOahGroup createGlobalMsdl2xmlOahGroup ();


}


#endif
