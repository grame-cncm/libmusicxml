/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msr2brailleInsiderOahHandler___
#define ___msr2brailleInsiderOahHandler___

#include "exports.h"

#include "oahAtomsCollection.h"

#include "oahRegularHandlers.h"


namespace MusicXML2
{
//_______________________________________________________________________________
EXP string msr2brailleAboutInformation ();

//_______________________________________________________________________________
class EXP msr2brailleInsiderOahHandler : public oahHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2brailleInsiderOahHandler> create (
      string executableName,
      string executableAboutInformation,
      string handlerHeader);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msr2brailleInsiderOahHandler (
      string executableName,
      string executableAboutInformation,
      string handlerHeader);

    virtual ~msr2brailleInsiderOahHandler ();

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheXml2braillePrefixes ();

    void                  createTheXml2brailleOptionGroups (
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

    // private fields
    // ------------------------------------------------------

    S_oahPrefix           fShortIgnoreRedundantPrefix;
    S_oahPrefix           fLongIgnoreRedundantPrefix;
};
typedef SMARTP<msr2brailleInsiderOahHandler> S_msr2brailleInsiderOahHandler;
EXP ostream& operator<< (ostream& os, const S_msr2brailleInsiderOahHandler& elt);

//______________________________________________________________________________
class EXP msr2brailleInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<msr2brailleInsiderOahGroup> create (
      string executableName,
      string handlerHeader);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2brailleInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msr2brailleInsiderOahGroup (
      string executableName,
      string handlerHeader);

    virtual ~msr2brailleInsiderOahGroup ();

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

    void                  printXml2brailleInsiderOahGroupHelp ();

    void                  printXml2brailleInsiderOahGroupValues (unsigned int fieldWidth);

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
typedef SMARTP<msr2brailleInsiderOahGroup> S_msr2brailleInsiderOahGroup;
EXP ostream& operator<< (ostream& os, const S_msr2brailleInsiderOahGroup& elt);

EXP extern S_msr2brailleInsiderOahGroup gGlobalXml2brailleInsiderOahGroup;

//______________________________________________________________________________
S_msr2brailleInsiderOahGroup createGlobalXml2brailleOahGroup (
  string executableName,
  string handlerHeader);


}


#endif
