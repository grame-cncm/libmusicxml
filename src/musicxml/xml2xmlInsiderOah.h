/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2xmlOah___
#define ___xml2xmlOah___

#include "oahBasicTypes.h"


namespace MusicXML2
{

//_______________________________________________________________________________
class EXP xml2xmlInsiderOahHandler : public oahHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2xmlInsiderOahHandler> create (
      string   executableName,
      ostream& os);

    virtual SMARTP<xml2xmlInsiderOahHandler> createHandlerNewbornCloneWithoutGroups ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2xmlInsiderOahHandler (
      string   executableName,
      ostream& os);

    virtual ~xml2xmlInsiderOahHandler ();

  private:

    // initialization
    // ------------------------------------------------------

    void                  createThePrefixes ();

    void                  initializeXml2xmlInsiderOahHandling (
                            string executableName);

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceOahHandlerQuietness ();

  public:

    // public services
    // ------------------------------------------------------

    void                  createThePrefixesAndInitialize (
                            string executableName);

    void                  checkOptionsAndArguments ();

    void                  checkOptionsConsistency ();

  private:

    // private services
    // ------------------------------------------------------

    void                  determineOutputFileNameFromInputFileName ();

  public:

    // visitors
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2xmlInsiderOahHandler> S_xml2xmlInsiderOahHandler;
EXP ostream& operator<< (ostream& os, const S_xml2xmlInsiderOahHandler& elt);

//______________________________________________________________________________
class xml2xmlOah : public oahGroup
{
  public:

    static SMARTP<xml2xmlOah> create (
      S_oahHandler handlerUpLink);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2xmlOah ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2xmlOah (
      S_oahHandler handlerUpLink);

    virtual ~xml2xmlOah ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceQuietness ();

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkOptionsAndArguments ();

    void                  checkOptionsConsistency ();

  public:

    // services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  printXml2xmlOahHelp ();

    void                  printXml2xmlOahValues (int fieldWidth);

  public:

    // output file
    // --------------------------------------

    string                fMusicXMLOutputFileName;

    // exit after some passes
    // --------------------------------------

    bool                  fExit2a;
    bool                  fExit2b;
};
typedef SMARTP<xml2xmlOah> S_xml2xmlOah;
EXP ostream& operator<< (ostream& os, const S_xml2xmlOah& elt);

EXP extern S_xml2xmlOah gXml2xmlOah;

//______________________________________________________________________________
void initializeXml2xmlOah (
  S_oahHandler handler);


}


#endif
