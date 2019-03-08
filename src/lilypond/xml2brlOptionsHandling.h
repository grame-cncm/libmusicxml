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
#include "optionsBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class xml2brlOptionsVersionItem : public optionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<xml2brlOptionsVersionItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2brlOptionsVersionItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
      
    virtual ~xml2brlOptionsVersionItem ();

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
typedef SMARTP<xml2brlOptionsVersionItem> S_xml2brlOptionsVersionItem;
EXP ostream& operator<< (ostream& os, const S_xml2brlOptionsVersionItem& elt);

//______________________________________________________________________________
class xml2brlOptionsAboutItem : public optionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<xml2brlOptionsAboutItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2brlOptionsAboutItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
      
    virtual ~xml2brlOptionsAboutItem ();

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
typedef SMARTP<xml2brlOptionsAboutItem> S_xml2brlOptionsAboutItem;
EXP ostream& operator<< (ostream& os, const S_xml2brlOptionsAboutItem& elt);

//______________________________________________________________________________
class xml2brlOptionsContactItem : public optionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<xml2brlOptionsContactItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2brlOptionsContactItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
      
    virtual ~xml2brlOptionsContactItem ();

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
typedef SMARTP<xml2brlOptionsContactItem> S_xml2brlOptionsContactItem;
EXP ostream& operator<< (ostream& os, const S_xml2brlOptionsContactItem& elt);

//_______________________________________________________________________________
class EXP xml2brlOptionsHandler : public optionsHandler
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
class xml2brlOptions : public optionsGroup
{
  public:

    static SMARTP<xml2brlOptions> create (
      S_optionsHandler optionsHandler);
        
  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2brlOptions ();
        
  protected:
  
    // constructors/destructor
    // ------------------------------------------------------

    xml2brlOptions (
      S_optionsHandler optionsHandler);
  
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

    virtual S_optionsItem handleOptionsItem (
                            ostream&      os,
                            S_optionsItem item);
        
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
  S_optionsHandler optionsHandler);


}


#endif
