/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2lyWithOptionsVectorOah___
#define ___xml2lyWithOptionsVectorOah___

//#include "exports.h"

//#include "xml2lyWithOptionsVectorManPageOah.h"

#include "xml2lyWithOptionsVectorOah2ManPageGenerator.h"


namespace MusicXML2
{

//_______________________________________________________________________________
class EXP xml2lyWithOptionsVectorOahHandler : public oahHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyWithOptionsVectorOahHandler> create (
      string           executableName,
      indentedOstream& ios);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyWithOptionsVectorOahHandler (
      string           executableName,
      indentedOstream& ios);

    virtual ~xml2lyWithOptionsVectorOahHandler ();

  private:

    // initialization
    // ------------------------------------------------------

    void                  initializeXml2lyWithOptionsVectorOptionsHandler (
                            string executableName,
                            S_xml2lyWithOptionsVectorOah2ManPageGenerator
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
typedef SMARTP<xml2lyWithOptionsVectorOahHandler> S_xml2lyWithOptionsVectorOahHandler;
EXP ostream& operator<< (ostream& os, const S_xml2lyWithOptionsVectorOahHandler& elt);

//______________________________________________________________________________
class xml2lyWithOptionsVectorOah : public oahGroup
{
  public:

    static SMARTP<xml2lyWithOptionsVectorOah> create (
      S_oahHandler handlerUpLink);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2lyWithOptionsVectorOah ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyWithOptionsVectorOah (
      S_oahHandler handlerUpLink);

    virtual ~xml2lyWithOptionsVectorOah ();

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

    void                  initializeXml2lyWithOptionsVectorLoopOptions (
                            bool boolOptionsInitialValue);

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  printXml2lyWithOptionsVectorOahHelp ();

    void                  printXml2lyWithOptionsVectorOahValues (int fieldWidth);

  public:

    // output file
    // --------------------------------------

    string                fLilyPondOutputFileName;
    bool                  fAutoOutputFileName;

    // loop back to MusicXML
    // --------------------------------------
    bool                  fLoopBackToMusicXML;
};
typedef SMARTP<xml2lyWithOptionsVectorOah> S_xml2lyWithOptionsVectorOah;
EXP ostream& operator<< (ostream& os, const S_xml2lyWithOptionsVectorOah& elt);

EXP extern S_xml2lyWithOptionsVectorOah gXml2lyWithOptionsVectorOah;

//______________________________________________________________________________
void initializeXml2lyWithOptionsVectorOah (
  S_oahHandler handler);


}


#endif
