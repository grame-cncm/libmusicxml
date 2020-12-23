/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2gmnManPageOah___
#define ___xml2gmnManPageOah___

#include "oahVisitor.h"

#include "xml2gmnOah2manPageGenerator.h"


namespace MusicXML2
{

//______________________________________________________________________________
class xml2gmnManPageGenerateAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2gmnManPageGenerateAtom> create (
      string                 shortName,
      string                 longName,
      string                 description,
      S_oahVisitor           theOahVisitor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2gmnManPageGenerateAtom (
      string                 shortName,
      string                 longName,
      string                 description,
      S_oahVisitor           theOahVisitor);

    virtual ~xml2gmnManPageGenerateAtom ();

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

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private services
    // ------------------------------------------------------

    void                  generateManPageData (ostream& os) const;

  private:

    // private fields
    // --------------------------------------

    S_oahVisitor          fOahVisitor;
};
typedef SMARTP<xml2gmnManPageGenerateAtom> S_xml2gmnManPageGenerateAtom;
EXP ostream& operator<< (ostream& os, const S_xml2gmnManPageGenerateAtom& elt);

//______________________________________________________________________________
class xml2gmnManPageOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2gmnManPageOahGroup> create (
      S_oahVisitor theOah2manPageGenerator);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2gmnManPageOahGroup (
                            S_oahVisitor theOah2manPageGenerator);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    xml2gmnManPageOahGroup (
      S_oahVisitor theOah2manPageGenerator);

    virtual ~xml2gmnManPageOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    virtual void          checkGroupOptionsConsistency () override;

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

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printManPageOahHelp ();

    void                  printManPageOahValues (int fieldWidth);

  private:

    // private fields
    // --------------------------------------

    S_oahVisitor          fOahVisitor;
};
typedef SMARTP<xml2gmnManPageOahGroup> S_xml2gmnManPageOahGroup;
EXP ostream& operator<< (ostream& os, const S_xml2gmnManPageOahGroup& elt);

EXP extern S_xml2gmnManPageOahGroup gGlobalXml2gmnManPageOahGroup;

//______________________________________________________________________________
EXP S_xml2gmnManPageOahGroup createGlobalXml2gmnManPageOahGroupHandling (
  S_oahHandler handler,
  S_oahVisitor theOah2manPageGenerator);


}


#endif
