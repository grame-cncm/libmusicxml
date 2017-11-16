/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __xml2lyOptionsHandling__
#define __xml2lyOptionsHandling__

#include "exports.h"
#include "optionsHandling.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class xml2lyOptionsVersionItem : public optionsVersionItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyOptionsVersionItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyOptionsVersionItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
      
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
ostream& operator<< (ostream& os, const S_xml2lyOptionsVersionItem& elt);

//_______________________________________________________________________________
class EXP xml2lyOptionsHandler : public optionsHandler
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyOptionsHandler> create (
      indentedOstream& ios);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyOptionsHandler (
      indentedOstream& ios);
      
    virtual ~xml2lyOptionsHandler();

  private:

    // initialization
    // ------------------------------------------------------

    void                  initializeOptionsHandler ();

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
ostream& operator<< (ostream& os, const S_xml2lyOptionsHandler& elt);

//______________________________________________________________________________
class xml2lyOptions : public optionsGroup
{
  public:

    static SMARTP<xml2lyOptions> create (
      S_optionsHandler optionsHandler);
        
  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2lyOptions ();
        
    void                  printXml2lyOptionsHelp ();

    void                  printXml2lyOptionsValues (int fieldWidth);
    
  protected:
  
    // constructors/destructor
    // ------------------------------------------------------

    xml2lyOptions (
      S_optionsHandler optionsHandler);
  
    virtual ~xml2lyOptions ();

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkXml2lyOptionsConsistency () const;

  public:

    // input
    // --------------------------------------

    string                fInputSourceName;
    
    string                fTranslationDate;


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
  S_optionsHandler optionsHandler);


}


#endif
