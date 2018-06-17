/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msrStreamsExampleOptionsHandling__
#define __msrStreamsExampleOptionsHandling__

#include "exports.h"
#include "optionsBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrStreamsExampleOptionsVersionItem : public optionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrStreamsExampleOptionsVersionItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStreamsExampleOptionsVersionItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
      
    virtual ~msrStreamsExampleOptionsVersionItem ();

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
typedef SMARTP<msrStreamsExampleOptionsVersionItem> S_msrStreamsExampleOptionsVersionItem;
ostream& operator<< (ostream& os, const S_msrStreamsExampleOptionsVersionItem& elt);

//______________________________________________________________________________
class msrStreamsExampleOptionsAboutItem : public optionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrStreamsExampleOptionsAboutItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStreamsExampleOptionsAboutItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
      
    virtual ~msrStreamsExampleOptionsAboutItem ();

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
typedef SMARTP<msrStreamsExampleOptionsAboutItem> S_msrStreamsExampleOptionsAboutItem;
ostream& operator<< (ostream& os, const S_msrStreamsExampleOptionsAboutItem& elt);

//______________________________________________________________________________
class msrStreamsExampleOptionsContactItem : public optionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrStreamsExampleOptionsContactItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStreamsExampleOptionsContactItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
      
    virtual ~msrStreamsExampleOptionsContactItem ();

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
typedef SMARTP<msrStreamsExampleOptionsContactItem> S_msrStreamsExampleOptionsContactItem;
ostream& operator<< (ostream& os, const S_msrStreamsExampleOptionsContactItem& elt);

//_______________________________________________________________________________
class EXP msrStreamsExampleOptionsHandler : public optionsHandler
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<msrStreamsExampleOptionsHandler> create (
      indentedOstream& ios);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStreamsExampleOptionsHandler (
      indentedOstream& ios);
      
    virtual ~msrStreamsExampleOptionsHandler();

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
typedef SMARTP<msrStreamsExampleOptionsHandler> S_msrStreamsExampleOptionsHandler;
ostream& operator<< (ostream& os, const S_msrStreamsExampleOptionsHandler& elt);

//______________________________________________________________________________
class msrStreamsExampleOptions : public optionsGroup
{
  public:

    static SMARTP<msrStreamsExampleOptions> create (
      S_optionsHandler optionsHandler);
        
  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsrStreamsExampleOptions ();
        
    void                  printMsrStreamsExampleOptionsHelp ();

    void                  printMsrStreamsExampleOptionsValues (int fieldWidth);
    
  protected:
  
    // constructors/destructor
    // ------------------------------------------------------

    msrStreamsExampleOptions (
      S_optionsHandler optionsHandler);
  
    virtual ~msrStreamsExampleOptions ();

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkmsrStreamsExampleOptionsConsistency () const;

    // services
    // ------------------------------------------------------

    virtual S_optionsItem handleOptionsItem (
                            ostream&      os,
                            S_optionsItem item);
        
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
typedef SMARTP<msrStreamsExampleOptions> S_msrStreamsExampleOptions;
EXP ostream& operator<< (ostream& os, const S_msrStreamsExampleOptions& elt);

EXP extern S_msrStreamsExampleOptions gMsrStreamsExampleOptions;

//______________________________________________________________________________
void initializeMsrStreamsExampleOptionsHandling (
  S_optionsHandler optionsHandler);


}


#endif
