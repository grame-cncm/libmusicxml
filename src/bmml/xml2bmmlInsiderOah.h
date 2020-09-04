/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2bmmlOah___
#define ___xml2bmmlOah___

#include "oahBasicTypes.h"


namespace MusicXML2
{

//_______________________________________________________________________________
class EXP xml2bmmlInsiderOahHandler : public oahHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2bmmlInsiderOahHandler> create (
      string   executableName,
      string   handlerHeader,
      ostream& os);

    static SMARTP<xml2bmmlInsiderOahHandler> createWithOnlyThePrefixes (
      string   executableName,
      string   handlerHeader,
      ostream& os);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2bmmlInsiderOahHandler (
      string   executableName,
      string   handlerHeader,
      ostream& os);

    virtual ~xml2bmmlInsiderOahHandler ();

  private:

    // initialization
    // ------------------------------------------------------

    void                  createThePrefixes ();

    void                  initializeXml2bmmlInsiderOahHandling (
                            string executableName);

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceOahHandlerQuietness ();

  public:

    // public services
    // ------------------------------------------------------

    void                  createThePrefixesAndInitializeOahHandling (
                            string executableName);

    void                  checkOptionsAndArguments () override;

    void                  checkOptionsConsistency () override;

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

    void                  print (ostream& os) const override;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2bmmlInsiderOahHandler> S_xml2bmmlInsiderOahHandler;
EXP ostream& operator<< (ostream& os, const S_xml2bmmlInsiderOahHandler& elt);

//______________________________________________________________________________
class xml2bmmlOah : public oahGroup
{
  public:

    static SMARTP<xml2bmmlOah> create (
      S_oahHandler handlerUpLink);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2bmmlOah ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2bmmlOah (
      S_oahHandler handlerUpLink);

    virtual ~xml2bmmlOah ();

  public:

    // set and get
    // ------------------------------------------------------

    S_oahBooleanAtom      getAutoOutputFileNameAtom () const
                              { return fAutoOutputFileNameAtom; }

    S_oahStringAtom       getOutputFileNameStringAtom () const
                              { return fOutputFileNameStringAtom; }

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceQuietness ();

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkOptionsAndArguments ();

    void                  checkOptionsConsistency () override;

  public:

    // services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    void                  printXml2bmmlOahHelp ();

    void                  printXml2bmmlOahValues (int fieldWidth);

  public:

    // we store the atoms for the needs of checkOptionsAndArguments()
    S_oahBooleanAtom      fAutoOutputFileNameAtom;

    S_oahStringAtom       fOutputFileNameStringAtom;

    // exit after some passes
    // --------------------------------------

    bool                  fExit2a;
    bool                  fExit2b;

  private:

    // LilyPond output file name
    // --------------------------------------

    // these private variables are the ones accessible through the atoms
    bool                  fAutoOutputFileName;

    string                fMusicXMLOutputFileName;
};
typedef SMARTP<xml2bmmlOah> S_xml2bmmlOah;
EXP ostream& operator<< (ostream& os, const S_xml2bmmlOah& elt);

EXP extern S_xml2bmmlOah gXml2bmmlOah;

//______________________________________________________________________________
void initializeXml2bmmlOah (
  S_oahHandler handler);


}


#endif
