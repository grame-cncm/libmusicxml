/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2brlInsiderOahHandler___
#define ___xml2brlInsiderOahHandler___

#include "exports.h"

#include "oahAtomsCollection.h"

#include "oahRegularHandlers.h"


namespace MusicXML2
{
//_______________________________________________________________________________
EXP string xml2brlAboutInformation ();

//_______________________________________________________________________________
class EXP xml2brlInsiderOahHandler : public oahHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2brlInsiderOahHandler> create (
      string executableName,
      string executableAboutInformation,
      string handlerHeader);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2brlInsiderOahHandler (
      string executableName,
      string executableAboutInformation,
      string handlerHeader);

    virtual ~xml2brlInsiderOahHandler ();

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheXml2brlPrefixes ();

    void                  createTheXml2brlOptionGroups (
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
typedef SMARTP<xml2brlInsiderOahHandler> S_xml2brlInsiderOahHandler;
EXP ostream& operator<< (ostream& os, const S_xml2brlInsiderOahHandler& elt);

//______________________________________________________________________________
class EXP xml2brlInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<xml2brlInsiderOahGroup> create (
      string executableName,
      string handlerHeader);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2brlInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2brlInsiderOahGroup (
      string executableName,
      string handlerHeader);

    virtual ~xml2brlInsiderOahGroup ();

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

    void                  printXml2brlInsiderOahGroupHelp ();

    void                  printXml2brlInsiderOahGroupValues (unsigned int fieldWidth);

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
typedef SMARTP<xml2brlInsiderOahGroup> S_xml2brlInsiderOahGroup;
EXP ostream& operator<< (ostream& os, const S_xml2brlInsiderOahGroup& elt);

EXP extern S_xml2brlInsiderOahGroup gGlobalXml2brlInsiderOahGroup;

//______________________________________________________________________________
S_xml2brlInsiderOahGroup createGlobalXml2brlOahGroup (
  string executableName,
  string handlerHeader);


}


#endif
