/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msr2lilypondManPageOah___
#define ___msr2lilypondManPageOah___

#include "oahVisitor.h"

#include "msr2lilypondOah2manPageGenerator.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msr2lilypondManPageGenerateAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2lilypondManPageGenerateAtom> create (
                            string                 shortName,
                            string                 longName,
                            string                 description,
                            S_oahVisitor           theOahVisitor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2lilypondManPageGenerateAtom (
                            string                 shortName,
                            string                 longName,
                            string                 description,
                            S_oahVisitor           theOahVisitor);

    virtual               ~msr2lilypondManPageGenerateAtom ();

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

    void                  printAtomWithValueOptionsValues (
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
typedef SMARTP<msr2lilypondManPageGenerateAtom> S_msr2lilypondManPageGenerateAtom;
EXP ostream& operator<< (ostream& os, const S_msr2lilypondManPageGenerateAtom& elt);

//______________________________________________________________________________
class EXP msr2lilypondManPageOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2lilypondManPageOahGroup> create (
                            S_oahVisitor theOah2manPageGenerator);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsr2lilypondManPageOahGroup (
                            S_oahVisitor theOah2manPageGenerator);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2lilypondManPageOahGroup (
                            S_oahVisitor theOah2manPageGenerator);

    virtual               ~msr2lilypondManPageOahGroup ();

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
typedef SMARTP<msr2lilypondManPageOahGroup> S_msr2lilypondManPageOahGroup;
EXP ostream& operator<< (ostream& os, const S_msr2lilypondManPageOahGroup& elt);

EXP extern S_msr2lilypondManPageOahGroup gGlobalMsr2lilypondManPageOahGroup;

//______________________________________________________________________________
EXP S_msr2lilypondManPageOahGroup createGlobalMsr2lilypondManPageOahGroup (
  S_oahVisitor theOah2manPageGenerator);


}


#endif
