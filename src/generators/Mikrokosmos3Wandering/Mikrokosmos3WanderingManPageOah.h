/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___Mikrokosmos3WanderingManPageOah___
#define ___Mikrokosmos3WanderingManPageOah___

#include "oahVisitor.h"

#include "Mikrokosmos3WanderingOah2manPageGenerator.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP Mikrokosmos3WanderingManPageGenerateAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<Mikrokosmos3WanderingManPageGenerateAtom> create (
                            string                 shortName,
                            string                 longName,
                            string                 description,
                            S_oahVisitor           theOahVisitor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          Mikrokosmos3WanderingManPageGenerateAtom (
                            string                 shortName,
                            string                 longName,
                            string                 description,
                            S_oahVisitor           theOahVisitor);

    virtual               ~Mikrokosmos3WanderingManPageGenerateAtom ();

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
typedef SMARTP<Mikrokosmos3WanderingManPageGenerateAtom> S_Mikrokosmos3WanderingManPageGenerateAtom;
EXP ostream& operator<< (ostream& os, const S_Mikrokosmos3WanderingManPageGenerateAtom& elt);

//______________________________________________________________________________
class EXP Mikrokosmos3WanderingManPageOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<Mikrokosmos3WanderingManPageOahGroup> create (
                            S_oahVisitor theOah2manPageGenerator);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMikrokosmos3WanderingManPageOahGroup (
                            S_oahVisitor theOah2manPageGenerator);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          Mikrokosmos3WanderingManPageOahGroup (
                            S_oahVisitor theOah2manPageGenerator);

    virtual               ~Mikrokosmos3WanderingManPageOahGroup ();

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
typedef SMARTP<Mikrokosmos3WanderingManPageOahGroup> S_Mikrokosmos3WanderingManPageOahGroup;
EXP ostream& operator<< (ostream& os, const S_Mikrokosmos3WanderingManPageOahGroup& elt);

EXP extern S_Mikrokosmos3WanderingManPageOahGroup gGlobalMikrokosmos3WanderingManPageOahGroup;

//______________________________________________________________________________
EXP S_Mikrokosmos3WanderingManPageOahGroup createGlobalMikrokosmos3WanderingManPageOahGroupHandling (
  S_oahHandler handler,
  S_oahVisitor theOah2manPageGenerator);


}


#endif
