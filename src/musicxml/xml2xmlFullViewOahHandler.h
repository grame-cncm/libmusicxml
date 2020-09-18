/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2xmlOahGroup___
#define ___xml2xmlOahGroup___

#include "oahBasicTypes.h"


namespace MusicXML2
{

//_______________________________________________________________________________
class EXP xml2xmlFullViewOahHandler : public oahHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2xmlFullViewOahHandler> create (
      string   executableName,
      string   handlerHeader,
      ostream& os);

    static SMARTP<xml2xmlFullViewOahHandler> createWithOnlyThePrefixes (
      string   executableName,
      string   handlerHeader,
      ostream& os);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2xmlFullViewOahHandler (
      string   executableName,
      string   handlerHeader,
      ostream& os);

    virtual ~xml2xmlFullViewOahHandler ();

  private:

    // initialization
    // ------------------------------------------------------

    void                  createThePrefixes ();

    void                  initializeXml2xmlFullViewOahHandler (
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
typedef SMARTP<xml2xmlFullViewOahHandler> S_xml2xmlFullViewOahHandler;
EXP ostream& operator<< (ostream& os, const S_xml2xmlFullViewOahHandler& elt);

//______________________________________________________________________________
class xml2xmlOahGroup : public oahGroup
{
  public:

    static SMARTP<xml2xmlOahGroup> create (
      S_oahHandler handlerUpLink);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2xmlOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2xmlOahGroup (
      S_oahHandler handlerUpLink);

    virtual ~xml2xmlOahGroup ();

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

    void                  printXml2xmlOahGroupHelp ();

    void                  printXml2xmlOahGroupValues (int fieldWidth);

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
typedef SMARTP<xml2xmlOahGroup> S_xml2xmlOahGroup;
EXP ostream& operator<< (ostream& os, const S_xml2xmlOahGroup& elt);

EXP extern S_xml2xmlOahGroup gGlobalXml2xmlOahGroup;

//______________________________________________________________________________
void initializeXml2xmlFullViewOahHandling (
  S_oahHandler handler);


}


#endif
