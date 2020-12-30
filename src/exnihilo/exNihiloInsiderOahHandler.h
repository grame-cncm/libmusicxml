/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___exNihiloInsiderOahGroup___
#define ___exNihiloInsiderOahGroup___

#include "oahBasicTypes.h"

#include "oahAtomsCollection.h"

#include "exNihiloOahTypes.h"


namespace MusicXML2
{
//_______________________________________________________________________________
#define K_REGULAR_OPTION_NAME "regular"
#define K_INSIDER_OPTION_NAME "insider"

//_______________________________________________________________________________
EXP string exNihiloAboutInformation ();

//_______________________________________________________________________________
class EXP exNihiloInsiderOahHandler : public oahHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<exNihiloInsiderOahHandler> create (
      string executableName,
      string executableAboutInformation,
      string handlerHeader);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    exNihiloInsiderOahHandler (
      string executableName,
      string executableAboutInformation,
      string handlerHeader);

    virtual ~exNihiloInsiderOahHandler ();

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheExNihiloPrefixes ();

    void                  createTheExNihiloOptionGroups (
                            string executableName);

  public:

    // public services
    // ------------------------------------------------------

    virtual void          checkOptionsAndArgumentsFromArgcAndArgv () const override;

    virtual string        fetchOutputFileNameFromTheOptions () const override;

    // quiet mode
    virtual void          enforceHandlerQuietness () override;

    // consistency check
    virtual void          checkHandlerOptionsConsistency () override;

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

    S_oahPrefix           fShortTracePrefix;
    S_oahPrefix           fLongTracePrefix;

    S_oahPrefix           fShortIgnoreRedundantPrefix;
    S_oahPrefix           fLongIgnoreRedundantPrefix;

    S_oahPrefix           fShortDelayRestsPrefix;
    S_oahPrefix           fLongDelayRestsPrefix;
};
typedef SMARTP<exNihiloInsiderOahHandler> S_exNihiloInsiderOahHandler;
EXP ostream& operator<< (ostream& os, const S_exNihiloInsiderOahHandler& elt);

//______________________________________________________________________________
class EXP exNihiloInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<exNihiloInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeExNihiloInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    exNihiloInsiderOahGroup ();

    virtual ~exNihiloInsiderOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // generated code kind
    void                  setGgeneratedCodeKind (generatedCodeKind value)
                              { fGgeneratedCodeKind = value; }
    generatedCodeKind     getGgeneratedCodeKind () const
                              { return fGgeneratedCodeKind; }

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

    // output file
    S_oahBooleanAtom      getAutoOutputFileNameAtom () const
                              { return fAutoOutputFileNameAtom; }

    S_oahStringAtom       getOutputFileNameStringAtom () const
                              { return fOutputFileNameStringAtom; }

    void                  setAutoOutputFileName ()
                              { fAutoOutputFileName = true; }
    bool                  getAutoOutputFileName () const
                              { return fAutoOutputFileName; }

    void                  setOutputFileName (string value)
                              { fOutputFileName = value; }
    string                getOutputFileName () const
                              { return fOutputFileName; }

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
    virtual void          checkGroupOptionsConsistency () override;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printExNihiloInsiderOahGroupHelp ();

    void                  printExNihiloInsiderOahGroupValues (unsigned int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createInsiderGuidoSubGroup ();

    void                  createInsiderOutputSubGroup ();

    void                  createInsiderQuitSubGroup ();

  private:

    // private fields
    // ------------------------------------------------------

    // generated code kind
    generatedCodeKind     fGgeneratedCodeKind;

    // Guido
    bool                  fGenerateComments;
    bool                  fGenerateStem;
    bool                  fGenerateBars;

    // output file
    // we store the atoms for the needs of fetchOutputFileNameFromTheOptions()
    S_oahBooleanAtom      fAutoOutputFileNameAtom;

    S_oahStringAtom       fOutputFileNameStringAtom;

    // quit after some passes
    bool                  fQuitAfterPass2a;
    bool                  fQuitAfterPass2b;

    // MusicXML output file name
    // these private variables are the ones accessible through the atoms
    bool                  fAutoOutputFileName;

    string                fOutputFileName;
};
typedef SMARTP<exNihiloInsiderOahGroup> S_exNihiloInsiderOahGroup;
EXP ostream& operator<< (ostream& os, const S_exNihiloInsiderOahGroup& elt);

EXP extern S_exNihiloInsiderOahGroup gGlobalExNihiloInsiderOahGroup;

//______________________________________________________________________________
S_exNihiloInsiderOahGroup createGlobalExNihiloOahGroup ();


}


#endif
