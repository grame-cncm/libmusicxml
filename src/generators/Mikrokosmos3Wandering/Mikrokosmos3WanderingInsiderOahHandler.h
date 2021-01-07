/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___Mikrokosmos3WanderingInsiderOahGroup___
#define ___Mikrokosmos3WanderingInsiderOahGroup___

#include "msrBasicTypes.h"

#include "oahBasicTypes.h"

#include "oahAtomsCollection.h"

#include "Mikrokosmos3WanderingOah.h"


namespace MusicXML2
{

//_______________________________________________________________________________
EXP string Mikrokosmos3WanderingAboutInformation ();

//_______________________________________________________________________________
class EXP Mikrokosmos3WanderingInsiderOahHandler : public oahHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<Mikrokosmos3WanderingInsiderOahHandler> create (
      const string&        executableName,
      const string&        executableAboutInformation,
      const string&        handlerHeader,
      generatorOutputKind generatorOutputKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    Mikrokosmos3WanderingInsiderOahHandler (
      const string&        executableName,
      const string&        executableAboutInformation,
      const string&        handlerHeader,
      generatorOutputKind generatorOutputKind);

    virtual ~Mikrokosmos3WanderingInsiderOahHandler ();

  public:

    // set and get
    // ------------------------------------------------------

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheMikrokosmos3WanderingPrefixes ();

    void                  createTheMikrokosmos3WanderingOptionGroups (
                            const string&        executableName,
                            generatorOutputKind generatorOutputKind);

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
};
typedef SMARTP<Mikrokosmos3WanderingInsiderOahHandler> S_Mikrokosmos3WanderingInsiderOahHandler;
EXP ostream& operator<< (ostream& os, const S_Mikrokosmos3WanderingInsiderOahHandler& elt);

//______________________________________________________________________________
class EXP Mikrokosmos3WanderingInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<Mikrokosmos3WanderingInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMikrokosmos3WanderingInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    Mikrokosmos3WanderingInsiderOahGroup ();

    virtual ~Mikrokosmos3WanderingInsiderOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // generation API kind
    void                  setGenerationAPIKind (msrGenerationAPIKind value)
                              { fGenerationAPIKind = value; }
    msrGenerationAPIKind  getGenerationAPIKind () const
                              { return fGenerationAPIKind; }

    // generate code kind
    void                  setGeneratorOutputKind (generatorOutputKind value)
                              { fGeneratorOutputKind = value; }
    generatorOutputKind  getGeneratorOutputKind () const
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

    void                  printMikrokosmos3WanderingInsiderOahGroupHelp ();

    void                  printMikrokosmos3WanderingInsiderOahGroupValues (unsigned int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createInsiderGuidoSubGroup ();

    void                  createInsiderQuitSubGroup ();

  private:

    // private fields
    // ------------------------------------------------------

    // generation API kind
    msrGenerationAPIKind  fGenerationAPIKind;

    // generate code kind
    generatorOutputKind  fGeneratorOutputKind;

    // Guido
    bool                  fGenerateComments;
    bool                  fGenerateStem;
    bool                  fGenerateBars;

    // quit after some passes
    bool                  fQuitAfterPass2a;
    bool                  fQuitAfterPass2b;
};
typedef SMARTP<Mikrokosmos3WanderingInsiderOahGroup> S_Mikrokosmos3WanderingInsiderOahGroup;
EXP ostream& operator<< (ostream& os, const S_Mikrokosmos3WanderingInsiderOahGroup& elt);

EXP extern S_Mikrokosmos3WanderingInsiderOahGroup gGlobalMikrokosmos3WanderingInsiderOahGroup;

//______________________________________________________________________________
S_Mikrokosmos3WanderingInsiderOahGroup createGlobalMikrokosmos3WanderingInsiderOahGroup ();


}


#endif
