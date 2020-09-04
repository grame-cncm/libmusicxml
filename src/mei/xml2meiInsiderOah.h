/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2meiOah___
#define ___xml2meiOah___

#include "oahBasicTypes.h"


namespace MusicXML2
{

//_______________________________________________________________________________
class EXP xml2meiInsiderOahHandler : public oahHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2meiInsiderOahHandler> create (
      string   executableName,
      string   handlerHeader,
      ostream& os);

    static SMARTP<xml2meiInsiderOahHandler> createWithOnlyThePrefixes (
      string   executableName,
      string   handlerHeader,
      ostream& os);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2meiInsiderOahHandler (
      string   executableName,
      string   handlerHeader,
      ostream& os);

    virtual ~xml2meiInsiderOahHandler ();

  private:

    // initialization
    // ------------------------------------------------------

    void                  createThePrefixes ();

    void                  initializeXml2meiInsiderOahHandling (
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
typedef SMARTP<xml2meiInsiderOahHandler> S_xml2meiInsiderOahHandler;
EXP ostream& operator<< (ostream& os, const S_xml2meiInsiderOahHandler& elt);

//______________________________________________________________________________
class xml2meiOah : public oahGroup
{
  public:

    static SMARTP<xml2meiOah> create (
      S_oahHandler handlerUpLink);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2meiOah ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2meiOah (
      S_oahHandler handlerUpLink);

    virtual ~xml2meiOah ();

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

    void                  printXml2meiOahHelp ();

    void                  printXml2meiOahValues (int fieldWidth);

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
typedef SMARTP<xml2meiOah> S_xml2meiOah;
EXP ostream& operator<< (ostream& os, const S_xml2meiOah& elt);

EXP extern S_xml2meiOah gXml2meiOah;

//______________________________________________________________________________
void initializeXml2meiOah (
  S_oahHandler handler);


}


#endif
