/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msr2guidoInsiderOahGroup___
#define ___msr2guidoInsiderOahGroup___

#include "oahBasicTypes.h"

#include "oahAtomsCollection.h"


namespace MusicXML2
{
//_______________________________________________________________________________
EXP string msr2guidoAboutInformation ();

//_______________________________________________________________________________
class EXP msr2guidoInsiderOahHandler : public oahHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2guidoInsiderOahHandler> create (
      string executableName,
      string executableAboutInformation,
      string handlerHeader);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msr2guidoInsiderOahHandler (
      string executableName,
      string executableAboutInformation,
      string handlerHeader);

    virtual ~msr2guidoInsiderOahHandler ();

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheMsr2guidoPrefixes ();

    void                  createTheMsr2guidoOptionGroups (
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
typedef SMARTP<msr2guidoInsiderOahHandler> S_msr2guidoInsiderOahHandler;
EXP ostream& operator<< (ostream& os, const S_msr2guidoInsiderOahHandler& elt);

//______________________________________________________________________________
class EXP msr2guidoInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<msr2guidoInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsr2guidoInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msr2guidoInsiderOahGroup ();

    virtual ~msr2guidoInsiderOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

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

    void                  printMsr2guidoInsiderOahGroupHelp ();

    void                  printMsr2guidoInsiderOahGroupValues (unsigned int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createInsiderGuidoSubGroup ();

    void                  createInsiderOutputSubGroup ();

    void                  createInsiderQuitSubGroup ();

  private:

    // private fields
    // ------------------------------------------------------

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
typedef SMARTP<msr2guidoInsiderOahGroup> S_msr2guidoInsiderOahGroup;
EXP ostream& operator<< (ostream& os, const S_msr2guidoInsiderOahGroup& elt);

EXP extern S_msr2guidoInsiderOahGroup gGlobalMsr2guidoInsiderOahGroup;

//______________________________________________________________________________
S_msr2guidoInsiderOahGroup createGlobalMsr2guidoOahGroup ();


}


#endif
