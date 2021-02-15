/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdl2brlInsiderHandler___
#define ___msdl2brlInsiderHandler___

#include "exports.h"

#include "oahAtomsCollection.h"

#include "oahInsiderHandlers.h"


namespace MusicXML2
{
//_______________________________________________________________________________
class EXP msdl2brlInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2brlInsiderHandler> create (
                            string executableName,
                            string handlerHeader);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2brlInsiderHandler (
                            string executableName,
                            string handlerHeader);

    virtual               ~msdl2brlInsiderHandler ();

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheMsdl2brlPrefixes ();

    void                  createTheMsdl2brlOptionGroups (
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
typedef SMARTP<msdl2brlInsiderHandler> S_msdl2brlInsiderHandler;
EXP ostream& operator<< (ostream& os, const S_msdl2brlInsiderHandler& elt);

//______________________________________________________________________________
class EXP msdl2brlInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<msdl2brlInsiderOahGroup> create (
                            string executableName,
                            string handlerHeader);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdl2brlInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2brlInsiderOahGroup (
                            string executableName,
                            string handlerHeader);

    virtual               ~msdl2brlInsiderOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // quit after some passes
    void                  setQuit2a ()
                              { fQuitAfterPass2a = true; }
    bool                  getQuitAfterPass2a () const
                              { return fQuitAfterPass2a; }

    void                  setQuit2b ()
                              { fQuitAfterPass2b = true; }
    bool                  getQuitAfterPass2b () const
                              { return fQuitAfterPass2a; }

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

    void                  printMsdl2brlInsiderOahGroupHelp ();

    void                  printMsdl2brlInsiderOahGroupValues (
                            unsigned int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createInsiderQuitSubGroup ();

  private:

    // private fields
    // --------------------------------------

    // quit after some passes
    bool                  fQuitAfterPass2a;
    bool                  fQuitAfterPass2b;
};
typedef SMARTP<msdl2brlInsiderOahGroup> S_msdl2brlInsiderOahGroup;
EXP ostream& operator<< (ostream& os, const S_msdl2brlInsiderOahGroup& elt);

EXP extern S_msdl2brlInsiderOahGroup gGlobalMsdl2brlInsiderOahGroup;

//______________________________________________________________________________
S_msdl2brlInsiderOahGroup createGlobalMsdl2brlOahGroup (
  string executableName,
  string handlerHeader);


}


#endif
