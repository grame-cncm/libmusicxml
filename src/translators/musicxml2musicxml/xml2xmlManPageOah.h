/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2xmlManPageOah___
#define ___xml2xmlManPageOah___

#include "oahVisitor.h"

#include "xml2xmlOah2manPageGenerator.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP xml2xmlManPageGenerateAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2xmlManPageGenerateAtom> create (
      string                 shortName,
      string                 longName,
      string                 description,
      S_oahVisitor           theOahVisitor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2xmlManPageGenerateAtom (
      string                 shortName,
      string                 longName,
      string                 description,
      S_oahVisitor           theOahVisitor);

    virtual ~xml2xmlManPageGenerateAtom ();

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
typedef SMARTP<xml2xmlManPageGenerateAtom> S_xml2xmlManPageGenerateAtom;
EXP ostream& operator<< (ostream& os, const S_xml2xmlManPageGenerateAtom& elt);

//______________________________________________________________________________
class EXP xml2xmlManPageOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2xmlManPageOahGroup> create (
      S_oahVisitor theOah2manPageGenerator);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2xmlManPageOahGroup (
                            S_oahVisitor theOah2manPageGenerator);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    xml2xmlManPageOahGroup (
      S_oahVisitor theOah2manPageGenerator);

    virtual ~xml2xmlManPageOahGroup ();

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
typedef SMARTP<xml2xmlManPageOahGroup> S_xml2xmlManPageOahGroup;
EXP ostream& operator<< (ostream& os, const S_xml2xmlManPageOahGroup& elt);

EXP extern S_xml2xmlManPageOahGroup gGlobalXml2xmlManPageOahGroup;

//______________________________________________________________________________
EXP S_xml2xmlManPageOahGroup createGlobalXml2xmlManPageOahGroupHandling (
  S_oahHandler handler,
  S_oahVisitor theOah2manPageGenerator);


}


#endif
