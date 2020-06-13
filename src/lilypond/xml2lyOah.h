/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2lyOah___
#define ___xml2lyOah___

//#include "exports.h"

//#include "xml2lyManPageOah.h"

#include "xml2lyOah2ManPageGenerator.h"


namespace MusicXML2
{

//_______________________________________________________________________________
class EXP xml2lyOahHandler : public oahHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyOahHandler> create (
      string   executableName,
      ostream& os);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyOahHandler (
      string   executableName,
      ostream& os);

    virtual ~xml2lyOahHandler ();

  private:

    // initialization
    // ------------------------------------------------------

    void                  initializeXml2lyOahHandler (
                            string executableName,
                            S_xml2lyOah2ManPageGenerator
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
typedef SMARTP<xml2lyOahHandler> S_xml2lyOahHandler;
EXP ostream& operator<< (ostream& os, const S_xml2lyOahHandler& elt);

//______________________________________________________________________________
class xml2lyOah : public oahGroup
{
  public:

    static SMARTP<xml2lyOah> create (
      S_oahHandler handlerUpLink);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2lyOah ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyOah (
      S_oahHandler handlerUpLink);

    virtual ~xml2lyOah ();

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

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  printXml2lyOahHelp ();

    void                  printXml2lyOahValues (int fieldWidth);

  public:

    // output file
    // --------------------------------------

    string                fLilyPondOutputFileName;
    bool                  fAutoOutputFileName;

    // loop back to MusicXML
    // --------------------------------------
    bool                  fLoopBackToMusicXML;

    // exit after some passes
    // --------------------------------------

    bool                  fExit2a;
    bool                  fExit2b;
};
typedef SMARTP<xml2lyOah> S_xml2lyOah;
EXP ostream& operator<< (ostream& os, const S_xml2lyOah& elt);

EXP extern S_xml2lyOah gXml2lyOah;

//______________________________________________________________________________
void initializeXml2lyOah (
  S_oahHandler handler);


}


#endif
