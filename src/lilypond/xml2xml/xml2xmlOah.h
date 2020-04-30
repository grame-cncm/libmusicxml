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

//#include "exports.h"

//#include "xml2xmlManPageOah.h"

#include "xml2xmlOah2ManPageGenerator.h"


namespace MusicXML2
{

//_______________________________________________________________________________
class EXP xml2xmlOahHandler : public oahHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2xmlOahHandler> create (
      string           executableName,
      indentedOstream& ios);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2xmlOahHandler (
      string           executableName,
      indentedOstream& ios);

    virtual ~xml2xmlOahHandler ();

  private:

    // initialization
    // ------------------------------------------------------

    void                  initializeXml2xmlOptionsHandler (
                            string executableName,
                            S_xml2xmlOah2ManPageGenerator
                                   theOah2ManPageGenerator);

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceOahHandlerQuietness ();

  public:

    // services
    // ------------------------------------------------------

    void                  checkOptionsAndArguments ();

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
typedef SMARTP<xml2xmlOahHandler> S_xml2xmlOahHandler;
EXP ostream& operator<< (ostream& os, const S_xml2xmlOahHandler& elt);

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

    string                fOutputFileName;
    bool                  fAutoOutputFileName;
};
typedef SMARTP<xml2xmlOah> S_xml2xmlOah;
EXP ostream& operator<< (ostream& os, const S_xml2xmlOah& elt);

EXP extern S_xml2xmlOah gXml2xmlOah;

//______________________________________________________________________________
void initializeXml2xmlOah (
  S_oahHandler handler);


}


#endif
