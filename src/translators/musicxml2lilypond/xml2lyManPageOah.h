/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2lyManPageOah___
#define ___xml2lyManPageOah___

#include "oahVisitor.h"

#include "xml2lyOah2manPageGenerator.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP xml2lyManPageGenerateAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyManPageGenerateAtom> create (
      string                 shortName,
      string                 longName,
      string                 description,
      S_oahVisitor           theOahVisitor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyManPageGenerateAtom (
      string                 shortName,
      string                 longName,
      string                 description,
      S_oahVisitor           theOahVisitor);

    virtual ~xml2lyManPageGenerateAtom ();

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
typedef SMARTP<xml2lyManPageGenerateAtom> S_xml2lyManPageGenerateAtom;
EXP ostream& operator<< (ostream& os, const S_xml2lyManPageGenerateAtom& elt);

//______________________________________________________________________________
class EXP xml2lyManPageOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyManPageOahGroup> create (
      S_oahVisitor theOah2manPageGenerator);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2lyManPageOahGroup (
                            S_oahVisitor theOah2manPageGenerator);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyManPageOahGroup (
      S_oahVisitor theOah2manPageGenerator);

    virtual ~xml2lyManPageOahGroup ();

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

    void                  printManPageOahValues (unsigned int fieldWidth);

  private:

    // private fields
    // --------------------------------------

    S_oahVisitor          fOahVisitor;
};
typedef SMARTP<xml2lyManPageOahGroup> S_xml2lyManPageOahGroup;
EXP ostream& operator<< (ostream& os, const S_xml2lyManPageOahGroup& elt);

EXP extern S_xml2lyManPageOahGroup gGlobalXml2lyManPageOahGroup;

//______________________________________________________________________________
EXP S_xml2lyManPageOahGroup createGlobalXml2lyManPageOahGroup (
  S_oahVisitor theOah2manPageGenerator);


}


#endif
