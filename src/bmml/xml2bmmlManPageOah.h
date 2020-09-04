/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2bmmlManPageOah___
#define ___xml2bmmlManPageOah___

#include "oahVisitor.h"

#include "xml2bmmlOah2ManPageGenerator.h"


namespace MusicXML2
{

//______________________________________________________________________________
class xml2bmmlManPageGenerateAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2bmmlManPageGenerateAtom> create (
      string                 shortName,
      string                 longName,
      string                 description,
      S_oahVisitor           theOahVisitor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2bmmlManPageGenerateAtom (
      string                 shortName,
      string                 longName,
      string                 description,
      S_oahVisitor           theOahVisitor);

    virtual ~xml2bmmlManPageGenerateAtom ();

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
typedef SMARTP<xml2bmmlManPageGenerateAtom> S_xml2bmmlManPageGenerateAtom;
EXP ostream& operator<< (ostream& os, const S_xml2bmmlManPageGenerateAtom& elt);

//______________________________________________________________________________
class xml2bmmlManPageOah : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2bmmlManPageOah> create (
      S_oahHandler           handlerUpLink,
      S_oahVisitor theOah2ManPageGenerator);

    SMARTP<xml2bmmlManPageOah>        createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2bmmlManPageOah (
                            bool boolOptionsInitialValue,
                            S_oahVisitor
                                 theOah2ManPageGenerator);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    xml2bmmlManPageOah (
      S_oahHandler           handlerUpLink,
      S_oahVisitor theOah2ManPageGenerator);

    virtual ~xml2bmmlManPageOah ();

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
typedef SMARTP<xml2bmmlManPageOah> S_xml2bmmlManPageOah;
EXP ostream& operator<< (ostream& os, const S_xml2bmmlManPageOah& elt);

EXP extern S_xml2bmmlManPageOah gXml2bmmlManPageOah;
EXP extern S_xml2bmmlManPageOah gXml2bmmlManPageOahUserChoices;
EXP extern S_xml2bmmlManPageOah gXml2bmmlManPageOahWithDetailedTrace;

//______________________________________________________________________________
EXP  void initializeXml2bmmlManPageOahHandling (
  S_oahHandler           handler,
  S_oahVisitor theOah2ManPageGenerator);


}


#endif
