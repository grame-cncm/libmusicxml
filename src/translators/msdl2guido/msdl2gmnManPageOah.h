/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdl2gmnManPageOah___
#define ___msdl2gmnManPageOah___

#include "oahVisitor.h"

#include "msdl2gmnOah2manPageGenerator.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msdl2gmnManPageGenerateAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2gmnManPageGenerateAtom> create (
                            string                 shortName,
                            string                 longName,
                            string                 description,
                            S_oahVisitor           theOahVisitor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2gmnManPageGenerateAtom (
                            string                 shortName,
                            string                 longName,
                            string                 description,
                            S_oahVisitor           theOahVisitor);

    virtual               ~msdl2gmnManPageGenerateAtom ();

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
typedef SMARTP<msdl2gmnManPageGenerateAtom> S_msdl2gmnManPageGenerateAtom;
EXP ostream& operator<< (ostream& os, const S_msdl2gmnManPageGenerateAtom& elt);

//______________________________________________________________________________
class EXP msdl2gmnManPageOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2gmnManPageOahGroup> create (
                            S_oahVisitor theOah2manPageGenerator);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdl2gmnManPageOahGroup (
                            S_oahVisitor theOah2manPageGenerator);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2gmnManPageOahGroup (
                            S_oahVisitor theOah2manPageGenerator);

    virtual               ~msdl2gmnManPageOahGroup ();

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
typedef SMARTP<msdl2gmnManPageOahGroup> S_msdl2gmnManPageOahGroup;
EXP ostream& operator<< (ostream& os, const S_msdl2gmnManPageOahGroup& elt);

EXP extern S_msdl2gmnManPageOahGroup gGlobalMsdl2gmnManPageOahGroup;

//______________________________________________________________________________
EXP S_msdl2gmnManPageOahGroup createGlobalMsdl2gmnManPageOahGroupHandling (
  S_oahHandler handler,
  S_oahVisitor theOah2manPageGenerator);


}


#endif
