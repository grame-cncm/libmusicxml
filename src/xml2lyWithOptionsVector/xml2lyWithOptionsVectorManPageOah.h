/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2lyWithOptionsVectorManPageOah___
#define ___xml2lyWithOptionsVectorManPageOah___

#include "oahVisitor.h"

#include "xml2lyWithOptionsVectorOah2ManPageGenerator.h"


namespace MusicXML2
{

//______________________________________________________________________________
class xml2lyWithOptionsVectorManPageGenerateAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyWithOptionsVectorManPageGenerateAtom> create (
      string                 shortName,
      string                 longName,
      string                 description,
      S_oahVisitor           theOahVisitor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyWithOptionsVectorManPageGenerateAtom (
      string                 shortName,
      string                 longName,
      string                 description,
      S_oahVisitor           theOahVisitor);

    virtual ~xml2lyWithOptionsVectorManPageGenerateAtom ();

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
typedef SMARTP<xml2lyWithOptionsVectorManPageGenerateAtom> S_xml2lyWithOptionsVectorManPageGenerateAtom;
EXP ostream& operator<< (ostream& os, const S_xml2lyWithOptionsVectorManPageGenerateAtom& elt);

//______________________________________________________________________________
class xml2lyWithOptionsVectorManPageOah : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyWithOptionsVectorManPageOah> create (
      S_oahHandler           handlerUpLink,
      S_oahVisitor theOah2ManPageGenerator);

    SMARTP<xml2lyWithOptionsVectorManPageOah>        createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2lyWithOptionsVectorManPageOah (
                            bool boolOptionsInitialValue,
                            S_oahVisitor
                                 theOah2ManPageGenerator);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyWithOptionsVectorManPageOah (
      S_oahHandler           handlerUpLink,
      S_oahVisitor theOah2ManPageGenerator);

    virtual ~xml2lyWithOptionsVectorManPageOah ();

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
typedef SMARTP<xml2lyWithOptionsVectorManPageOah> S_xml2lyWithOptionsVectorManPageOah;
EXP ostream& operator<< (ostream& os, const S_xml2lyWithOptionsVectorManPageOah& elt);

EXP extern S_xml2lyWithOptionsVectorManPageOah gxml2lyWithOptionsVectorManPageOah;
EXP extern S_xml2lyWithOptionsVectorManPageOah gxml2lyWithOptionsVectorManPageOahUserChoices;
EXP extern S_xml2lyWithOptionsVectorManPageOah gxml2lyWithOptionsVectorManPageOahWithDetailedTrace;

//______________________________________________________________________________
EXP  void initializeXml2lyWithOptionsVectorManPageOahHandling (
  S_oahHandler           handler,
  S_oahVisitor theOah2ManPageGenerator);


}


#endif
