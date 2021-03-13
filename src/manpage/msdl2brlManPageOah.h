/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdl2brlManPageOah___
#define ___msdl2brlManPageOah___

#include "msdl2brlOah2manPage.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msdl2brlManPageGenerateAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2brlManPageGenerateAtom> create (
                            string       shortName,
                            string       longName,
                            string       description,
                            S_oahVisitor theOahVisitor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2brlManPageGenerateAtom (
                            string       shortName,
                            string       longName,
                            string       description,
                            S_oahVisitor theOahVisitor);

    virtual               ~msdl2brlManPageGenerateAtom ();

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
typedef SMARTP<msdl2brlManPageGenerateAtom> S_msdl2brlManPageGenerateAtom;
EXP ostream& operator<< (ostream& os, const S_msdl2brlManPageGenerateAtom& elt);

//______________________________________________________________________________
class EXP msdl2brlManPageOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2brlManPageOahGroup> create (
                            S_oahVisitor theOah2manPage);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdl2brlManPageOahGroup (
                            S_oahVisitor theOah2manPage);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2brlManPageOahGroup (
                            S_oahVisitor theOah2manPage);

    virtual               ~msdl2brlManPageOahGroup ();

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
typedef SMARTP<msdl2brlManPageOahGroup> S_msdl2brlManPageOahGroup;
EXP ostream& operator<< (ostream& os, const S_msdl2brlManPageOahGroup& elt);

EXP extern S_msdl2brlManPageOahGroup gGlobalMsdl2brlManPageOahGroup;

//______________________________________________________________________________
EXP extern S_msdl2brlManPageOahGroup createGlobalMsdl2brlManPageOahHandler (
  S_oahVisitor theOah2manPage);


}


#endif
