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

#include "oahBasicTypes.h"

#include "oahAtomsCollection.h"

#include "Mikrokosmos3WanderingOah.h"


namespace MusicXML2
{
//_______________________________________________________________________________
#define K_GENERATED_CODE_KIND_SHORT_NAME "gen"
#define K_GENERATED_CODE_KIND_LONG_NAME  "generated-code-kind"

//_______________________________________________________________________________
EXP string Mikrokosmos3WanderingAboutInformation ();

//_______________________________________________________________________________
class EXP Mikrokosmos3WanderingInsiderOahHandler : public oahHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<Mikrokosmos3WanderingInsiderOahHandler> create (
      string            executableName,
      string            executableAboutInformation,
      string            handlerHeader,
      generatedCodeKind theGeneratedCodeKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    Mikrokosmos3WanderingInsiderOahHandler (
      string            executableName,
      string            executableAboutInformation,
      string            handlerHeader,
      generatedCodeKind theGeneratedCodeKind);

    virtual ~Mikrokosmos3WanderingInsiderOahHandler ();

  public:

    // set and get
    // ------------------------------------------------------

    // generated code kind
    void                  setGeneratedCodeKind (generatedCodeKind value)
                              { fGeneratedCodeKind = value; }
    generatedCodeKind     getGeneratedCodeKind () const
                              { return fGeneratedCodeKind; }

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheMikrokosmos3WanderingPrefixes ();

    void                  createTheMikrokosmos3WanderingOptionGroups (
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

    // generated code kind
    generatedCodeKind     fGeneratedCodeKind;
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

    // generated code kind
    void                  setGeneratedCodeKind (generatedCodeKind value)
                              { fGeneratedCodeKind = value; }
    generatedCodeKind     getGeneratedCodeKind () const
                              { return fGeneratedCodeKind; }

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

    // generated code kind
    generatedCodeKind     fGeneratedCodeKind;

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
