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

#include "xml2xmlOah2ManPageGenerator.h"


namespace MusicXML2
{

//______________________________________________________________________________
class xml2xmlManPageGenerateAtom : public oahAtom
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

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // private services
    // ------------------------------------------------------

    void                  generateManPageData (ostream& os) const;

  private:

    // fields
    // --------------------------------------

    S_oahVisitor          fOahVisitor;
};
typedef SMARTP<xml2xmlManPageGenerateAtom> S_xml2xmlManPageGenerateAtom;
EXP ostream& operator<< (ostream& os, const S_xml2xmlManPageGenerateAtom& elt);

//______________________________________________________________________________
class xml2xmlManPageOah : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2xmlManPageOah> create (
      S_oahHandler           handlerUpLink,
      S_oahVisitor theOah2ManPageGenerator);

    SMARTP<xml2xmlManPageOah>        createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2xmlManPageOah (
                            bool boolOptionsInitialValue,
                            S_oahVisitor
                                 theOah2ManPageGenerator);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    xml2xmlManPageOah (
      S_oahHandler           handlerUpLink,
      S_oahVisitor theOah2ManPageGenerator);

    virtual ~xml2xmlManPageOah ();

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

    virtual void          checkOptionsConsistency ();

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

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

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
typedef SMARTP<xml2xmlManPageOah> S_xml2xmlManPageOah;
EXP ostream& operator<< (ostream& os, const S_xml2xmlManPageOah& elt);

EXP extern S_xml2xmlManPageOah gxml2xmlManPageOah;
EXP extern S_xml2xmlManPageOah gxml2xmlManPageOahUserChoices;
EXP extern S_xml2xmlManPageOah gxml2xmlManPageOahWithDetailedTrace;

//______________________________________________________________________________
EXP  void initializeXml2xmlManPageOahHandling (
  S_oahHandler           handler,
  S_oahVisitor theOah2ManPageGenerator);


}


#endif
