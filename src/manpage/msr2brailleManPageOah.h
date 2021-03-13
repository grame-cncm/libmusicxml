/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msr2brailleManPageOah___
#define ___msr2brailleManPageOah___

#include "brailleGenerationOah2manPage.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msr2brailleManPageGenerateAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2brailleManPageGenerateAtom> create (
                            string                 shortName,
                            string                 longName,
                            string                 description,
                            S_oahVisitor           theOahVisitor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2brailleManPageGenerateAtom (
                            string                 shortName,
                            string                 longName,
                            string                 description,
                            S_oahVisitor           theOahVisitor);

    virtual               ~msr2brailleManPageGenerateAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyElement (ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private services
    // ------------------------------------------------------

    void                  generateManPageData (ostream& os) const;

  private:

    // private fields
    // --------------------------------------

    S_oahVisitor          fOahVisitor;
};
typedef SMARTP<msr2brailleManPageGenerateAtom> S_msr2brailleManPageGenerateAtom;
EXP ostream& operator<< (ostream& os, const S_msr2brailleManPageGenerateAtom& elt);

//______________________________________________________________________________
class EXP msr2brailleManPageOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2brailleManPageOahGroup> create (
                            S_oahVisitor theOah2manPage);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2brailleManPageOahGroup (
                            S_oahVisitor theOah2manPage);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2brailleManPageOahGroup (
                            S_oahVisitor theOah2manPage);

    virtual               ~msr2brailleManPageOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceGroupQuietness () override;

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkGroupOptionsConsistency () override;

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeManPageGenerateOptions (
                            S_oahVisitor
                                 theOah2manPage);

#ifdef TRACING_IS_ENABLED
    void                  initializeManPageTraceOah ();
#endif

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printManPageOahelp ();

    void                  printManPageOahValues (unsigned int fieldWidth);

  private:

    // private fields
    // --------------------------------------

    S_oahVisitor
                          fOah2manPage;
};
typedef SMARTP<msr2brailleManPageOahGroup> S_msr2brailleManPageOahGroup;
EXP ostream& operator<< (ostream& os, const S_msr2brailleManPageOahGroup& elt);

EXP extern S_msr2brailleManPageOahGroup gGlobalXml2brailleManPageOahGroup;

//______________________________________________________________________________
EXP extern S_msr2brailleManPageOahGroup createGlobalXml2brailleManPageOahHandler (
  S_oahVisitor theOah2manPage);


}


#endif
