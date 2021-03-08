/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdl2lyManPageOah___
#define ___msdl2lyManPageOah___

#include "oahVisitor.h"

#include "msdl2lyOah2manPageGenerator.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msdl2lyManPageGenerateAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2lyManPageGenerateAtom> create (
                            string                 shortName,
                            string                 longName,
                            string                 description,
                            S_oahVisitor           theOahVisitor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2lyManPageGenerateAtom (
                            string                 shortName,
                            string                 longName,
                            string                 description,
                            S_oahVisitor           theOahVisitor);

    virtual               ~msdl2lyManPageGenerateAtom ();

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
typedef SMARTP<msdl2lyManPageGenerateAtom> S_msdl2lyManPageGenerateAtom;
EXP ostream& operator<< (ostream& os, const S_msdl2lyManPageGenerateAtom& elt);

//______________________________________________________________________________
class EXP msdl2lyManPageOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2lyManPageOahGroup> create (
                            S_oahVisitor theOah2manPageGenerator);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdl2lyManPageOahGroup (
                            S_oahVisitor theOah2manPageGenerator);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2lyManPageOahGroup (
                            S_oahVisitor theOah2manPageGenerator);

    virtual               ~msdl2lyManPageOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    void                  checkGroupOptionsConsistency () override;

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeManPageGenerateOptions (
                            S_oahVisitor
                                 theOah2manPageGenerator);

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

    void                  printManPageOahHelp ();

    void                  printManPageOahValues (unsigned int fieldWidth);

  private:

    // private fields
    // --------------------------------------

    S_oahVisitor          fOahVisitor;
};
typedef SMARTP<msdl2lyManPageOahGroup> S_msdl2lyManPageOahGroup;
EXP ostream& operator<< (ostream& os, const S_msdl2lyManPageOahGroup& elt);

EXP extern S_msdl2lyManPageOahGroup gGlobalMsdl2lyManPageOahGroup;

//______________________________________________________________________________
EXP S_msdl2lyManPageOahGroup createGlobalMsdl2lyManPageOahGroup (
  S_oahVisitor theOah2manPageGenerator);


}


#endif
