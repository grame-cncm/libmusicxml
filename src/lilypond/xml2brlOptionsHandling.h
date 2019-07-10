/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2brlOptionsHandling___
#define ___xml2brlOptionsHandling___

#include "exports.h"
#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class xml2brlOptionsVersionAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2brlOptionsVersionAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2brlOptionsVersionAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2brlOptionsVersionAtom ();

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
typedef SMARTP<xml2brlOptionsVersionAtom> S_xml2brlOptionsVersionAtom;
EXP ostream& operator<< (ostream& os, const S_xml2brlOptionsVersionAtom& elt);

//______________________________________________________________________________
class xml2brlOptionsAboutAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2brlOptionsAboutAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2brlOptionsAboutAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2brlOptionsAboutAtom ();

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
typedef SMARTP<xml2brlOptionsAboutAtom> S_xml2brlOptionsAboutAtom;
EXP ostream& operator<< (ostream& os, const S_xml2brlOptionsAboutAtom& elt);

//______________________________________________________________________________
class xml2brlOptionsContactAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2brlOptionsContactAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2brlOptionsContactAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2brlOptionsContactAtom ();

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
typedef SMARTP<xml2brlOptionsContactAtom> S_xml2brlOptionsContactAtom;
EXP ostream& operator<< (ostream& os, const S_xml2brlOptionsContactAtom& elt);

//_______________________________________________________________________________
class EXP xml2brlOptionsHandler : public oahHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2brlOptionsHandler> create (
      string           executableName,
      indentedOstream& ios);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2brlOptionsHandler (
      string           executableName,
      indentedOstream& ios);

    virtual ~xml2brlOptionsHandler ();

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
typedef SMARTP<xml2brlOptionsHandler> S_xml2brlOptionsHandler;
EXP ostream& operator<< (ostream& os, const S_xml2brlOptionsHandler& elt);

//______________________________________________________________________________
class xml2brlOptions : public oahGroup
{
  public:

    static SMARTP<xml2brlOptions> create (
      S_oahHandler handler);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2brlOptions ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2brlOptions (
      S_oahHandler handler);

    virtual ~xml2brlOptions ();

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

    void                  printXml2brlOptionsHelp ();

    void                  printXml2brlOptionsValues (int fieldWidth);

  public:

    // input
    // --------------------------------------


    // output file
    // --------------------------------------

    string                fOutputFileName;
    bool                  fAutoOutputFile;


};
typedef SMARTP<xml2brlOptions> S_xml2brlOptions;
EXP ostream& operator<< (ostream& os, const S_xml2brlOptions& elt);

EXP extern S_xml2brlOptions gXml2brlOptions;

//______________________________________________________________________________
void initializeXml2brlOptionsHandling (
  S_oahHandler handler);


}


#endif
