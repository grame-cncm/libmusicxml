/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2brlInsiderOahHandler___
#define ___xml2brlInsiderOahHandler___

#include "oahBasicTypes.h"


namespace MusicXML2
{

//_______________________________________________________________________________
class EXP xml2brlInsiderOahHandler : public oahHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2brlInsiderOahHandler> create (
      string   executableName,
      string   handlerHeader,
      ostream& os);

    static SMARTP<xml2brlInsiderOahHandler> createWithOnlyThePrefixes (
      string   executableName,
      string   handlerHeader,
      ostream& os);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2brlInsiderOahHandler (
      string   executableName,
      string   handlerHeader,
      ostream& os);

    virtual ~xml2brlInsiderOahHandler ();

  private:

    // initialization
    // ------------------------------------------------------

    void                  createThePrefixes ();

    void                  initializeXml2brlInsiderOahHandler (
                            string executableName);

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceHandlerQuietness () override;

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkHandlerOptionsConsistency () override;

  public:

    // services
    // ------------------------------------------------------

    void                  createThePrefixesAndInitializeOahHandler (
                            string executableName);

    void                  checkOptionsAndArguments () override;

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
typedef SMARTP<xml2brlInsiderOahHandler> S_xml2brlInsiderOahHandler;
EXP ostream& operator<< (ostream& os, const S_xml2brlInsiderOahHandler& elt);

//______________________________________________________________________________
class xml2brlOahGroup : public oahGroup
{
  public:

    static SMARTP<xml2brlOahGroup> create (
      S_oahHandler handlerUpLink);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2brlOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2brlOahGroup (
      S_oahHandler handlerUpLink);

    virtual ~xml2brlOahGroup ();

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

    void                  enforceGroupQuietness () override;

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkOptionsAndArguments ();

    void                  checkGroupOptionsConsistency () override;

  public:

    // services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    void                  printXml2brlOahHelp ();

    void                  printXml2brlOahValues (int fieldWidth);

  public:

    // output file
    // --------------------------------------

    // we store the atoms for the needs of checkOptionsAndArguments()
    S_oahBooleanAtom      fAutoOutputFileNameAtom;

    S_oahStringAtom       fOutputFileNameStringAtom;

    // exit after some passes
    // --------------------------------------

    bool                  fExit2a;
    bool                  fExit2b;

  private:

    // Braille music output file name
    // --------------------------------------

    // these private variables are the ones accessible through the atoms

    bool                  fAutoOutputFileName;

    string                fBrailleMusicOutputFileName;
};
typedef SMARTP<xml2brlOahGroup> S_xml2brlOahGroup;
EXP ostream& operator<< (ostream& os, const S_xml2brlOahGroup& elt);

EXP extern S_xml2brlOahGroup gGlobalXml2brlOahGroup;

//______________________________________________________________________________
void initializeXml2brlOahHandling (
  S_oahHandler handler);


}


#endif
