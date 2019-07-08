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
class xml2lyOptionsVersionItem : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyOptionsVersionItem> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyOptionsVersionItem (
      string shortName,
      string longName,
      string description);

    virtual ~xml2lyOptionsVersionItem ();

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
typedef SMARTP<xml2lyOptionsVersionItem> S_xml2lyOptionsVersionItem;
EXP ostream& operator<< (ostream& os, const S_xml2lyOptionsVersionItem& elt);

//______________________________________________________________________________
class xml2lyOptionsAboutItem : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyOptionsAboutItem> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyOptionsAboutItem (
      string shortName,
      string longName,
      string description);

    virtual ~xml2lyOptionsAboutItem ();

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
typedef SMARTP<xml2lyOptionsAboutItem> S_xml2lyOptionsAboutItem;
EXP ostream& operator<< (ostream& os, const S_xml2lyOptionsAboutItem& elt);

//______________________________________________________________________________
class xml2lyOptionsContactItem : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyOptionsContactItem> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyOptionsContactItem (
      string shortName,
      string longName,
      string description);

    virtual ~xml2lyOptionsContactItem ();

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
typedef SMARTP<xml2lyOptionsContactItem> S_xml2lyOptionsContactItem;
EXP ostream& operator<< (ostream& os, const S_xml2lyOptionsContactItem& elt);

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
                            ostream&      os,
                            S_oahAtom item);

  public:

    // print
    // ------------------------------------------------------

    void                  printXml2lyOptionsHelp ();

    void                  printXml2lyOptionsValues (int fieldWidth);

  public:

    // output file
    // --------------------------------------

    string                fOutputFileName;
    bool                  fAutoOutputFile;
};
typedef SMARTP<xml2lyOptions> S_xml2lyOptions;
EXP ostream& operator<< (ostream& os, const S_xml2lyOptions& elt);

EXP extern S_xml2lyOptions gXml2lyOptions;

//______________________________________________________________________________
void initializeXml2lyOptionsHandling (
  S_oahHandler handler);


}


#endif
