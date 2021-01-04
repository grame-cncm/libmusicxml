/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msr2lilypondInsiderOahHandler___
#define ___msr2lilypondInsiderOahHandler___

#include "enableExtraOahIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "extraOah.h"
#endif

#include "oahAtomsCollection.h"

#include "oahRegularHandlers.h"


namespace MusicXML2
{
//_______________________________________________________________________________
EXP string msr2lilypondAboutInformation ();

//_______________________________________________________________________________
class EXP msr2lilypondInsiderOahHandler : public oahHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2lilypondInsiderOahHandler> create (
      string                 executableName,
      string                 executableAboutInformation,
      string                 handlerHeader,
      oahHandlerUsedThruKind handlerUsedThruKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msr2lilypondInsiderOahHandler (
      string                 executableName,
      string                 executableAboutInformation,
      string                 handlerHeader,
      oahHandlerUsedThruKind handlerUsedThruKind);

    virtual ~msr2lilypondInsiderOahHandler ();

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheMsr2lilypondPrefixes ();

    void                  createTheMsr2lilypondOptionGroups (
                            string executableName);

  public:

    // public services
    // ------------------------------------------------------

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
typedef SMARTP<msr2lilypondInsiderOahHandler> S_msr2lilypondInsiderOahHandler;
EXP ostream& operator<< (ostream& os, const S_msr2lilypondInsiderOahHandler& elt);

//______________________________________________________________________________
class EXP msr2lilypondInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<msr2lilypondInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsr2lilypondInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msr2lilypondInsiderOahGroup ();

    virtual ~msr2lilypondInsiderOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // output file
    S_oahBooleanAtom      getAutoOutputFileNameAtom () const
                              { return fAutoOutputFileNameAtom; }

    S_oahStringAtom       getOutputFileNameStringAtom () const
                              { return fOutputFileNameStringAtom; }

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

    void                  printMsr2lilypondInsiderOahGroupHelp ();

    void                  printMsr2lilypondInsiderOahGroupValues (unsigned int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createInsiderOutputSubGroup ();

    void                  createInsiderQuitSubGroup ();

  private:

    // private fields
    // ------------------------------------------------------

    // output file
    // we store the atoms for the needs of fetchOutputFileNameFromTheOptions()
    S_oahBooleanAtom      fAutoOutputFileNameAtom;

    S_oahStringAtom       fOutputFileNameStringAtom;

    // quit after some passes
    bool                  fQuitAfterPass2a;
    bool                  fQuitAfterPass2b;

    // LilyPond output file name
    // these private variables are the ones accessible through the atoms
    bool                  fAutoOutputFileName;

    string                fOutputFileName;
};
typedef SMARTP<msr2lilypondInsiderOahGroup> S_msr2lilypondInsiderOahGroup;
EXP ostream& operator<< (ostream& os, const S_msr2lilypondInsiderOahGroup& elt);

EXP extern S_msr2lilypondInsiderOahGroup gGlobalMsr2lilypondInsiderOahGroup;

//______________________________________________________________________________
S_msr2lilypondInsiderOahGroup createGlobalMsr2lilypondInsiderOahGroup ();


}


#endif
