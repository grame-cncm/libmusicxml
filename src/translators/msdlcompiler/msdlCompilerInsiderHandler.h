/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdlCompilerInsiderHandler___
#define ___msdlCompilerInsiderHandler___

#include "msrBasicTypes.h"

#include "oahBasicTypes.h"

#include "oahAtomsCollection.h"

#include "oahInsiderHandlers.h"

#include "multiGenerationOah.h"

#include "msdlCompilerOah.h"


namespace MusicXML2
{

//_______________________________________________________________________________
class EXP msdlCompilerInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlCompilerInsiderHandler> create (
                            const string&            executableName,
                            const string&            handlerHeader,
                            multiGeneratorOutputKind generatorOutputKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlCompilerInsiderHandler (
                            const string&            executableName,
                            const string&            handlerHeader,
                            multiGeneratorOutputKind generatorOutputKind);

    virtual               ~msdlCompilerInsiderHandler ();

  public:

    // set and get
    // ------------------------------------------------------

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheMsdlCompilerPrefixes ();

    void                  createTheMsdlCompilerOptionGroups (
                            const string&            executableName,
                            multiGeneratorOutputKind generatorOutputKind);

  public:

    // public services
    // ------------------------------------------------------

    void                  handlerOahError (const string& errorMessage) override;

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

  protected:

    // protected services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

    string                usageInformation (
                            multiGeneratorOutputKind generatorOutputKind);

    string                msdlCompilerAboutInformation (
                            multiGeneratorOutputKind generatorOutputKind) const;

  private:

    // private fields
    // ------------------------------------------------------

    // generated output kind
    multiGeneratorOutputKind   fGeneratorOutputKind;

};
typedef SMARTP<msdlCompilerInsiderHandler> S_msdlCompilerInsiderHandler;
EXP ostream& operator<< (ostream& os, const S_msdlCompilerInsiderHandler& elt);

//______________________________________________________________________________
class EXP msdlCompilerInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<msdlCompilerInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializemsdlCompilerInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlCompilerInsiderOahGroup ();

    virtual               ~msdlCompilerInsiderOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // generated output kind
    void                  setGeneratorOutputKind (multiGeneratorOutputKind value)
                              { fGeneratorOutputKind = value; }
    multiGeneratorOutputKind   getGeneratorOutputKind () const
                              { return fGeneratorOutputKind; }

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

    void                  printmsdlCompilerInsiderOahGroupHelp ();

    void                  printmsdlCompilerInsiderOahGroupValues (
                            unsigned int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createInsiderLilypondSubGroup ();

  private:

    // private fields
    // ------------------------------------------------------

    // generated output kind
    multiGeneratorOutputKind
                          fGeneratorOutputKind;
};
typedef SMARTP<msdlCompilerInsiderOahGroup> S_msdlCompilerInsiderOahGroup;
EXP ostream& operator<< (ostream& os, const S_msdlCompilerInsiderOahGroup& elt);

EXP extern S_msdlCompilerInsiderOahGroup gGlobalmsdlCompilerInsiderOahGroup;

//______________________________________________________________________________
S_msdlCompilerInsiderOahGroup createGlobalmsdlCompilerInsiderOahGroup ();


}


#endif
