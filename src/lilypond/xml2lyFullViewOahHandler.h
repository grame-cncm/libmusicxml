/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2lyFullViewOahHandler___
#define ___xml2lyFullViewOahHandler___

#include "xml2lyOahTypes.h"


namespace MusicXML2
{

//_______________________________________________________________________________
class EXP xml2lyFullViewOahHandler : public oahHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyFullViewOahHandler> create (
      string   executableName,
      string   handlerHeader,
      ostream& os);

    static SMARTP<xml2lyFullViewOahHandler> createWithOnlyThePrefixes (
      string   executableName,
      string   handlerHeader,
      ostream& os);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyFullViewOahHandler (
      string   executableName,
      string   handlerHeader,
      ostream& os);

    virtual ~xml2lyFullViewOahHandler ();

  private:

    // initialization
    // ------------------------------------------------------

    void                  createThePrefixes ();

    void                  initializeXml2lyFullViewOahHandler (
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
typedef SMARTP<xml2lyFullViewOahHandler> S_xml2lyFullViewOahHandler;
EXP ostream& operator<< (ostream& os, const S_xml2lyFullViewOahHandler& elt);

//______________________________________________________________________________
class xml2lyOahGroup : public oahGroup
{
  public:

    static SMARTP<xml2lyOahGroup> create (
      S_oahHandler handlerUpLink);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2lyOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyOahGroup (
      S_oahHandler handlerUpLink);

    virtual ~xml2lyOahGroup ();

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

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printXml2lyOahGroupHelp ();

    void                  printXml2lyOahGroupValues (int fieldWidth);

  public:

    // output file
    // --------------------------------------

    // we store the atoms for the needs of checkOptionsAndArguments()
    S_oahBooleanAtom      fAutoOutputFileNameAtom;

    S_oahStringAtom       fOutputFileNameStringAtom;

    // loop back to MusicXML
    // --------------------------------------
    bool                  fLoopBackToMusicXML;

    // exit after some passes
    // --------------------------------------

    bool                  fExit2a;
    bool                  fExit2b;

  private:

    // LilyPond output file name
    // --------------------------------------

    // these private variables are the ones accessible through the atoms
    bool                  fAutoOutputFileName;

    string                fLilyPondOutputFileName;
};
typedef SMARTP<xml2lyOahGroup> S_xml2lyOahGroup;
EXP ostream& operator<< (ostream& os, const S_xml2lyOahGroup& elt);

EXP extern S_xml2lyOahGroup gGlobalXml2lyOahGroup;

//______________________________________________________________________________
void initializeXml2lyOah (
  S_oahHandler handler);


}


#endif
