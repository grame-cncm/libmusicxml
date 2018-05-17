/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __extraOptions__
#define __extraOptions__

#include "optionsBasicTypes.h"
#include "exports.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class optionsShowAllChordIntervalsItem : public optionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsShowAllChordIntervalsItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsShowAllChordIntervalsItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
      
    virtual ~optionsShowAllChordIntervalsItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAllChordIntervals (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;
  
  private:
  
    // fields
    // ------------------------------------------------------
};
typedef SMARTP<optionsShowAllChordIntervalsItem> S_optionsShowAllChordIntervalsItem;
ostream& operator<< (ostream& os, const S_optionsShowAllChordIntervalsItem& elt);

//______________________________________________________________________________
class optionsShowAllChordNotesItem : public optionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsShowAllChordNotesItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsShowAllChordNotesItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
      
    virtual ~optionsShowAllChordNotesItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAllChordNotes (
                            ostream&              os,
                            msrSemiTonesPitchKind semiTonesPitchKind) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;
  
  private:
  
    // fields
    // ------------------------------------------------------
};
typedef SMARTP<optionsShowAllChordNotesItem> S_optionsShowAllChordNotesItem;
ostream& operator<< (ostream& os, const S_optionsShowAllChordNotesItem& elt);

//______________________________________________________________________________
class extraOptions : public optionsGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraOptions> create (
      S_optionsHandler optionsHandler);
    
    SMARTP<extraOptions>        createCloneWithDetailedTrace ();
    
  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeExtraOptions (
                            bool boolOptionsInitialValue);
        
  protected:
  
    // constructors/destructor
    // ------------------------------------------------------

    extraOptions (
      S_optionsHandler optionsHandler);
  
    virtual ~extraOptions ();
 
  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkExtraOptionsConsistency () const;

    // services
    // ------------------------------------------------------

    virtual S_optionsItem handleOptionsItem (
                            ostream&      os,
                            S_optionsItem item);

    virtual void          handleValuedOptionsItem (
                            ostream&      os,
                            S_optionsItem item,
                            string        theString);
  public:

    // print
    // ------------------------------------------------------

    void                  printExtraOptionsHelp ();

    void                  printExtraOptionsValues (int fieldWidth);
    
  public:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<extraOptions> S_extraOptions;
EXP ostream& operator<< (ostream& os, const S_extraOptions& elt);

EXP extern S_extraOptions gExtraOptions;
EXP extern S_extraOptions gExtraOptionsUserChoices;
EXP extern S_extraOptions gExtraOptionsWithDetailedTrace;

//______________________________________________________________________________
void initializeExtraOptionsHandling (
  S_optionsHandler optionsHandler);


}


#endif
