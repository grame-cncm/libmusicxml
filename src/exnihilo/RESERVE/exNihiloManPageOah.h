/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___exNihiloManPageOah___
#define ___exNihiloManPageOah___

#include "oahVisitor.h"

#include "exNihiloOah2manPageGenerator.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP exNihiloManPageGenerateAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<exNihiloManPageGenerateAtom> create (
      string                 shortName,
      string                 longName,
      string                 description,
      S_oahVisitor           theOahVisitor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    exNihiloManPageGenerateAtom (
      string                 shortName,
      string                 longName,
      string                 description,
      S_oahVisitor           theOahVisitor);

    virtual ~exNihiloManPageGenerateAtom ();

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
typedef SMARTP<exNihiloManPageGenerateAtom> S_exNihiloManPageGenerateAtom;
EXP ostream& operator<< (ostream& os, const S_exNihiloManPageGenerateAtom& elt);

//______________________________________________________________________________
class EXP exNihiloManPageOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<exNihiloManPageOahGroup> create (
      S_oahVisitor theOah2manPageGenerator);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2gmnManPageOahGroup (
                            S_oahVisitor theOah2manPageGenerator);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    exNihiloManPageOahGroup (
      S_oahVisitor theOah2manPageGenerator);

    virtual ~exNihiloManPageOahGroup ();

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
typedef SMARTP<exNihiloManPageOahGroup> S_exNihiloManPageOahGroup;
EXP ostream& operator<< (ostream& os, const S_exNihiloManPageOahGroup& elt);

EXP extern S_exNihiloManPageOahGroup gGlobalXml2gmnManPageOahGroup;

//______________________________________________________________________________
EXP S_exNihiloManPageOahGroup createGlobalXml2gmnManPageOahGroupHandling (
  S_oahHandler handler,
  S_oahVisitor theOah2manPageGenerator);


}


#endif
