/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2brlOah___
#define ___xml2brlOah___

//#include "exports.h"

//#include "xml2lyManPageOah.h"

#include "xml2brlOah2ManPageGenerator.h"


namespace MusicXML2
{

//_______________________________________________________________________________
class EXP xml2brlOahHandler : public oahHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2brlOahHandler> create (
      string   executableName,
      ostream& ios);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2brlOahHandler (
      string   executableName,
      ostream& ios);

    virtual ~xml2brlOahHandler ();

  private:

    // initialization
    // ------------------------------------------------------

    void                  initializeXml2brlOptionsHandler (
                            string executableName,
                            S_xml2brlOah2ManPageGenerator
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

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2brlOahHandler> S_xml2brlOahHandler;
EXP ostream& operator<< (ostream& os, const S_xml2brlOahHandler& elt);

//______________________________________________________________________________
class xml2brlOah : public oahGroup
{
  public:

    static SMARTP<xml2brlOah> create (
      S_oahHandler handlerUpLink);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2brlOah ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2brlOah (
      S_oahHandler handlerUpLink);

    virtual ~xml2brlOah ();

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

    void                  printXml2brlOahHelp ();

    void                  printXml2brlOahValues (int fieldWidth);

  public:

    // input
    // --------------------------------------

    // output file
    // --------------------------------------

    string                fBrailleMusicOutputFileName;
    bool                  fAutoOutputFileName;

    // exit after some passes
    // --------------------------------------

    bool                  fExit2a;
    bool                  fExit2b;
};
typedef SMARTP<xml2brlOah> S_xml2brlOah;
EXP ostream& operator<< (ostream& os, const S_xml2brlOah& elt);

EXP extern S_xml2brlOah gXml2brlOah;

//______________________________________________________________________________
void initializeXml2brlOah (
  S_oahHandler handler);


}


#endif
