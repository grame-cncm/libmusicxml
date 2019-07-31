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

#include "exports.h"
#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class xml2lyOahVersionAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyOahVersionAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyOahVersionAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2lyOahVersionAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printVersion (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2lyOahVersionAtom> S_xml2lyOahVersionAtom;
EXP ostream& operator<< (ostream& os, const S_xml2lyOahVersionAtom& elt);

//______________________________________________________________________________
class xml2lyOahAboutAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyOahAboutAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyOahAboutAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2lyOahAboutAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAbout (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2lyOahAboutAtom> S_xml2lyOahAboutAtom;
EXP ostream& operator<< (ostream& os, const S_xml2lyOahAboutAtom& elt);

//______________________________________________________________________________
class xml2lyOahContactAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyOahContactAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyOahContactAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2lyOahContactAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printContact (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2lyOahContactAtom> S_xml2lyOahContactAtom;
EXP ostream& operator<< (ostream& os, const S_xml2lyOahContactAtom& elt);

//_______________________________________________________________________________
class EXP xml2lyOahHandler : public oahHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyOahHandler> create (
      string           executableName,
      indentedOstream& ios);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyOahHandler (
      string           executableName,
      indentedOstream& ios);

    virtual ~xml2lyOahHandler ();

  private:

    // initialization
    // ------------------------------------------------------

    void                  initializeOptionsHandler (
                            string executableName);

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceOahHandlerQuietness ();

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
typedef SMARTP<xml2lyOahHandler> S_xml2lyOahHandler;
EXP ostream& operator<< (ostream& os, const S_xml2lyOahHandler& elt);

//______________________________________________________________________________
class xml2lyOah : public oahGroup
{
  public:

    static SMARTP<xml2lyOah> create (
      S_oahHandler handler);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2lyOah ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyOah (
      S_oahHandler handler);

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

  public:

    // print
    // ------------------------------------------------------

    void                  printXml2lyOahHelp ();

    void                  printXml2lyOahValues (int fieldWidth);

  public:

    // output file
    // --------------------------------------

    string                fOutputFileName;
    bool                  fAutoOutputFileName;
};
typedef SMARTP<xml2lyOah> S_xml2lyOah;
EXP ostream& operator<< (ostream& os, const S_xml2lyOah& elt);

EXP extern S_xml2lyOah gXml2lyOah;

//______________________________________________________________________________
void initializeXml2lyOah (
  S_oahHandler handler);


}


#endif
