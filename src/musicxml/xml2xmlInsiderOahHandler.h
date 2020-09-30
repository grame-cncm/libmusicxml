/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2xmlInsiderOahGroup___
#define ___xml2xmlInsiderOahGroup___

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
      string   handlerHeader,
      ostream& os);

    static SMARTP<xml2xmlInsiderOahHandler> createWithOnlyThePrefixes (
      string   executableName,
      string   handlerHeader,
      ostream& os);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2xmlInsiderOahHandler (
      string   executableName,
      string   handlerHeader,
      ostream& os);

    virtual ~xml2xmlInsiderOahHandler ();

  private:

    // initialization
    // ------------------------------------------------------

    void                  createThePrefixes ();

    void                  initializeXml2xmlInsiderOahHandler (
                            string executableName);

  public:

    // public services
    // ------------------------------------------------------

    void                  createThePrefixesAndInitializeOahHandler (
                            string executableName);

    void                  checkOptionsAndArguments () override;

    // quiet mode
    void                  enforceHandlerQuietness () override;

    // consistency check
    virtual void          checkHandlerOptionsConsistency () override;

  private:

    // private services
    // ------------------------------------------------------

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
typedef SMARTP<xml2xmlInsiderOahHandler> S_xml2xmlInsiderOahHandler;
EXP ostream& operator<< (ostream& os, const S_xml2xmlInsiderOahHandler& elt);

//______________________________________________________________________________
class xml2xmlInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<xml2xmlInsiderOahGroup> create (
      S_oahHandler handlerUpLink);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2xmlInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2xmlInsiderOahGroup (
      S_oahHandler handlerUpLink);

    virtual ~xml2xmlInsiderOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    S_oahBooleanAtom      getAutoOutputFileNameAtom () const
                              { return fAutoOutputFileNameAtom; }

    S_oahStringAtom       getOutputFileNameStringAtom () const
                              { return fOutputFileNameStringAtom; }

  public:

    // public services
    // ------------------------------------------------------

    void                  checkOptionsAndArguments ();

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    virtual void          checkGroupOptionsConsistency () override;

  public:

    // visitors
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    void                  printXml2xmlInsiderOahGroupHelp ();

    void                  printXml2xmlInsiderOahGroupValues (int fieldWidth);

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
typedef SMARTP<xml2xmlInsiderOahGroup> S_xml2xmlInsiderOahGroup;
EXP ostream& operator<< (ostream& os, const S_xml2xmlInsiderOahGroup& elt);

EXP extern S_xml2xmlInsiderOahGroup gGlobalXml2xmlOahGroup;

//______________________________________________________________________________
void initializeXml2xmlInsiderOahHandling (
  S_oahHandler handler);


}


#endif
