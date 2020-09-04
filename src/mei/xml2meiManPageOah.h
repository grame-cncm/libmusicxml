/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2meiManPageOah___
#define ___xml2meiManPageOah___

#include "oahVisitor.h"

#include "xml2meiOah2ManPageGenerator.h"


namespace MusicXML2
{

//______________________________________________________________________________
class xml2meiManPageGenerateAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2meiManPageGenerateAtom> create (
      string                 shortName,
      string                 longName,
      string                 description,
      S_oahVisitor           theOahVisitor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2meiManPageGenerateAtom (
      string                 shortName,
      string                 longName,
      string                 description,
      S_oahVisitor           theOahVisitor);

    virtual ~xml2meiManPageGenerateAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os) override;

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

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private services
    // ------------------------------------------------------

    void                  generateManPageData (ostream& os) const;

  private:

    // fields
    // --------------------------------------

    S_oahVisitor          fOahVisitor;
};
typedef SMARTP<xml2meiManPageGenerateAtom> S_xml2meiManPageGenerateAtom;
EXP ostream& operator<< (ostream& os, const S_xml2meiManPageGenerateAtom& elt);

//______________________________________________________________________________
class xml2meiManPageOah : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2meiManPageOah> create (
      S_oahHandler           handlerUpLink,
      S_oahVisitor theOah2ManPageGenerator);

    SMARTP<xml2meiManPageOah>        createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2meiManPageOah (
                            bool boolOptionsInitialValue,
                            S_oahVisitor
                                 theOah2ManPageGenerator);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    xml2meiManPageOah (
      S_oahHandler           handlerUpLink,
      S_oahVisitor theOah2ManPageGenerator);

    virtual ~xml2meiManPageOah ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceQuietness ();

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkOptionsConsistency () override;

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeManPageGenerateOptions (
                            bool boolOptionsInitialValue,
                            S_oahVisitor
                                 theOah2ManPageGenerator);

#ifdef TRACE_OAH
    void                  initializeManPageTraceOah (
                            bool boolOptionsInitialValue);
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

    // fields
    // --------------------------------------

    S_oahVisitor          fOahVisitor;
};
typedef SMARTP<xml2meiManPageOah> S_xml2meiManPageOah;
EXP ostream& operator<< (ostream& os, const S_xml2meiManPageOah& elt);

EXP extern S_xml2meiManPageOah gXml2meiManPageOah;
EXP extern S_xml2meiManPageOah gXml2meiManPageOahUserChoices;
EXP extern S_xml2meiManPageOah gXml2meiManPageOahWithDetailedTrace;

//______________________________________________________________________________
EXP  void initializeXml2meiManPageOahHandling (
  S_oahHandler           handler,
  S_oahVisitor theOah2ManPageGenerator);


}


#endif
