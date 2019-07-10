/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2lyOptionsHandling___
#define ___xml2lyOptionsHandling___

#include "exports.h"
#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class xml2lyOptionsVersionAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyOptionsVersionAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyOptionsVersionAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2lyOptionsVersionAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printVersion (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2lyOptionsVersionAtom> S_xml2lyOptionsVersionAtom;
EXP ostream& operator<< (ostream& os, const S_xml2lyOptionsVersionAtom& elt);

//______________________________________________________________________________
class xml2lyOptionsAboutAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyOptionsAboutAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyOptionsAboutAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2lyOptionsAboutAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAbout (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2lyOptionsAboutAtom> S_xml2lyOptionsAboutAtom;
EXP ostream& operator<< (ostream& os, const S_xml2lyOptionsAboutAtom& elt);

//______________________________________________________________________________
class xml2lyOptionsContactAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyOptionsContactAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyOptionsContactAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2lyOptionsContactAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printContact (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2lyOptionsContactAtom> S_xml2lyOptionsContactAtom;
EXP ostream& operator<< (ostream& os, const S_xml2lyOptionsContactAtom& elt);

//_______________________________________________________________________________
class EXP xml2lyOptionsHandler : public oahHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyOptionsHandler> create (
      string           executableName,
      indentedOstream& ios);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyOptionsHandler (
      string           executableName,
      indentedOstream& ios);

    virtual ~xml2lyOptionsHandler ();

  private:

    // initialization
    // ------------------------------------------------------

    void                  initializeOptionsHandler (
                            string executableName);

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceOptionsHandlerQuietness ();

  public:

    // services
    // ------------------------------------------------------

    void                  checkOptionsAndArguments ();

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2lyOptionsHandler> S_xml2lyOptionsHandler;
EXP ostream& operator<< (ostream& os, const S_xml2lyOptionsHandler& elt);

//______________________________________________________________________________
class xml2lyOptions : public oahGroup
{
  public:

    static SMARTP<xml2lyOptions> create (
      S_oahHandler handler);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2lyOptions ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyOptions (
      S_oahHandler handler);

    virtual ~xml2lyOptions ();

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

    virtual S_oahValuedAtom
                          handleAtom (
                            ostream&  os,
                            S_oahAtom atom);

  public:

    // print
    // ------------------------------------------------------

    void                  printXml2lyOptionsHelp ();

    void                  printXml2lyOptionsValues (int fieldWidth);

  public:

    // output file
    // --------------------------------------

    string                fOutputFileName;
    bool                  fAutoOutputFileName;
};
typedef SMARTP<xml2lyOptions> S_xml2lyOptions;
EXP ostream& operator<< (ostream& os, const S_xml2lyOptions& elt);

EXP extern S_xml2lyOptions gXml2lyOptions;

//______________________________________________________________________________
void initializeXml2lyOptionsHandling (
  S_oahHandler handler);


}


#endif
