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
class optionsShowChordsNotesItem : public optionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsShowChordsNotesItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsShowChordsNotesItemVariableDisplayName,
      msrSemiTonesPitchKind&
                         optionsShowChordsNotesKindItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsShowChordsNotesItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsShowChordsNotesItemVariableDisplayName,
      msrSemiTonesPitchKind&
                         optionsShowChordsNotesKindItemVariable);
      
    virtual ~optionsShowChordsNotesItem();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsShowChordsNotesKindItemVariableDisplayName () const
                              {
                                return
                                  fOptionsShowChordsNotesKindItemVariableDisplayName;
                              }
                              
    void                  setShowChordNotesKindItemVariableValue (
                            msrSemiTonesPitchKind value)
                              {
                                fOptionsShowChordsNotesKindItemVariable = value;
                              }

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:
  
    // fields
    // ------------------------------------------------------

    string                fOptionsShowChordsNotesKindItemVariableDisplayName;
    msrSemiTonesPitchKind&
                          fOptionsShowChordsNotesKindItemVariable;
};
typedef SMARTP<optionsShowChordsNotesItem> S_optionsShowChordsNotesItem;
ostream& operator<< (ostream& os, const S_optionsShowChordsNotesItem& elt);

//______________________________________________________________________________
class optionsShowChordNotesItem : public optionsItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsShowChordNotesItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsShowChordNotesItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription);
      
    virtual ~optionsShowChordNotesItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printChordNotes (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;
  
  private:
  
    // fields
    // ------------------------------------------------------
};
typedef SMARTP<optionsShowChordNotesItem> S_optionsShowChordNotesItem;
ostream& operator<< (ostream& os, const S_optionsShowChordNotesItem& elt);

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

  public:

    // print
    // ------------------------------------------------------

    void                  printExtraOptionsHelp ();

    void                  printExtraOptionsValues (int fieldWidth);
    
  public:

    // fields
    // ------------------------------------------------------

    // chord intervals
    
    bool                  fShowAllChordIntervals;


    // chord notes

    bool                  fShowChordNotes;
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
