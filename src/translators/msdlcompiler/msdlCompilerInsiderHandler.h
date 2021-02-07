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

#include "msdl2msrOah.h"


namespace MusicXML2
{

//_______________________________________________________________________________
class EXP msdlCompilerInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlCompilerInsiderHandler> create (
                            const string&       executableName,
                            const string&       handlerHeader,
                            generatorOutputKind generatorOutputKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlCompilerInsiderHandler (
                            const string&       executableName,
                            const string&       handlerHeader,
                            generatorOutputKind generatorOutputKind);

    virtual               ~msdlCompilerInsiderHandler ();

  public:

    // set and get
    // ------------------------------------------------------

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheMsdlCompilerPrefixes ();

    void                  createTheMsdlCompilerOptionGroups (
                            const string&        executableName,
                            generatorOutputKind generatorOutputKind);

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
                            generatorOutputKind generatorOutputKind);

    string                msdlCompilerAboutInformation (
                            generatorOutputKind theGeneratorOutputKind) const;

  private:

    // private fields
    // ------------------------------------------------------

    // generated output kind
    generatorOutputKind  fGeneratorOutputKind;

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

    void                  initializeMsdlCompilerInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlCompilerInsiderOahGroup ();

    virtual               ~msdlCompilerInsiderOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // generation API kind
    void                  setGenerationAPIKind (msrGenerationAPIKind value)
                              { fGenerationAPIKind = value; }
    msrGenerationAPIKind  getGenerationAPIKind () const
                              { return fGenerationAPIKind; }

    // generated output kind
    void                  setGeneratorOutputKind (generatorOutputKind value)
                              { fGeneratorOutputKind = value; }
    generatorOutputKind   getGeneratorOutputKind () const
                              { return fGeneratorOutputKind; }

    // Guido
    void                  setGenerateComments ()
                              { fGenerateComments = true; }
    bool                  getGenerateComments () const
                              { return fGenerateComments; }

    void                  setGenerateStem ()
                              { fGenerateStem = true; }
    bool                  getGenerateStem () const
                              { return fGenerateStem; }

    void                  setGenerateBars ()
                              { fGenerateBars = true; }
    bool                  getGenerateBars () const
                              { return fGenerateBars; }

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

    void                  printMsdlCompilerInsiderOahGroupHelp ();

    void                  printMsdlCompilerInsiderOahGroupValues (
                            unsigned int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createInsiderGuidoSubGroup ();

    void                  createInsiderLilypondSubGroup ();

    void                  createInsiderQuitSubGroup ();

  private:

    // private fields
    // ------------------------------------------------------

    // generation API kind
    msrGenerationAPIKind  fGenerationAPIKind;

    // generated output kind
    generatorOutputKind   fGeneratorOutputKind;

    // Guido
    bool                  fGenerateComments;
    bool                  fGenerateStem;
    bool                  fGenerateBars;

    // quit after some passes
    bool                  fQuitAfterPass2a;
    bool                  fQuitAfterPass2b;
};
typedef SMARTP<msdlCompilerInsiderOahGroup> S_msdlCompilerInsiderOahGroup;
EXP ostream& operator<< (ostream& os, const S_msdlCompilerInsiderOahGroup& elt);

EXP extern S_msdlCompilerInsiderOahGroup gGlobalMsdlCompilerInsiderOahGroup;

//______________________________________________________________________________
S_msdlCompilerInsiderOahGroup createGlobalMsdlCompilerInsiderOahGroup ();


}


#endif
