/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdl2gmnInsiderOahGroup___
#define ___msdl2gmnInsiderOahGroup___

#include "oahBasicTypes.h"

#include "oahInsiderHandlers.h"

#include "oahAtomsCollection.h"


namespace MusicXML2
{
//_______________________________________________________________________________
class EXP msdl2gmnInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2gmnInsiderHandler> create (
                            string executableName,
                            string handlerHeader);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2gmnInsiderHandler (
                            string executableName,
                            string handlerHeader);

    virtual               ~msdl2gmnInsiderHandler ();

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheMsdl2gmnPrefixes ();

    void                  createTheMsdl2gmnOptionGroups (
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
typedef SMARTP<msdl2gmnInsiderHandler> S_msdl2gmnInsiderHandler;
EXP ostream& operator<< (ostream& os, const S_msdl2gmnInsiderHandler& elt);

//______________________________________________________________________________
class EXP msdl2gmnInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<msdl2gmnInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdl2gmnInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2gmnInsiderOahGroup ();

    virtual               ~msdl2gmnInsiderOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // quit after some passes
    void                  setQuitAfterPass2a ()
                              { fQuitAfterPass2a = true; }
    bool                  getQuitAfterPass2a () const
                              { return fQuitAfterPass2a; }

    void                  setQuitAfterPass2b ()
                              { fQuitAfterPass2b = true; }
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

    void                  printMsdl2gmnInsiderOahGroupHelp ();

    void                  printMsdl2gmnInsiderOahGroupValues (
                            unsigned int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

  private:

    // private fields
    // ------------------------------------------------------

    // quit after some passes
    bool                  fQuitAfterPass2a;
    bool                  fQuitAfterPass2b;
};
typedef SMARTP<msdl2gmnInsiderOahGroup> S_msdl2gmnInsiderOahGroup;
EXP ostream& operator<< (ostream& os, const S_msdl2gmnInsiderOahGroup& elt);

EXP extern S_msdl2gmnInsiderOahGroup gGlobalMsdl2gmnInsiderOahGroup;

//______________________________________________________________________________
S_msdl2gmnInsiderOahGroup createGlobalMsdl2gmnInsiderOahGroup ();


}


#endif
