/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrOptions___
#define ___msrOptions___

#include "msrBasicTypes.h"

#include "optionsBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class optionsPartRenameItem : public optionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsPartRenameItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsPartRenameItemVariableDisplayName,
      map<string, string>&
                         optionsPartRenameItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsPartRenameItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsPartRenameItemVariableDisplayName,
      map<string, string>&
                         optionsPartRenameItemVariable);
      
    virtual ~optionsPartRenameItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsPartRenameItemVariableDisplayName () const
                              {
                                return
                                  fOptionsPartRenameItemVariableDisplayName;
                              }
                              
    void                  setPartRenameItemVariableValue (
                            string oldPartName,
                            string newPartName)
                              {
                                fOptionsPartRenameItemVariable [oldPartName] =
                                  newPartName;
                              }

    const map<string, string>&
                          getOptionsPartRenameItemVariable ()
                              {
                                return
                                  fOptionsPartRenameItemVariable;
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

    string                fOptionsPartRenameItemVariableDisplayName;
    map<string, string>&  fOptionsPartRenameItemVariable;
};
typedef SMARTP<optionsPartRenameItem> S_optionsPartRenameItem;
ostream& operator<< (ostream& os, const S_optionsPartRenameItem& elt);

//______________________________________________________________________________
class optionsMsrPitchesLanguageItem : public optionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsMsrPitchesLanguageItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsMsrPitchesLanguageKindItemVariableDisplayName,
      msrQuarterTonesPitchesLanguageKind&
                         optionsMsrPitchesLanguageKindItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsMsrPitchesLanguageItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsMsrPitchesLanguageKindItemVariableDisplayName,
      msrQuarterTonesPitchesLanguageKind&
                         optionsMsrPitchesLanguageKindItemVariable);
      
    virtual ~optionsMsrPitchesLanguageItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsMsrPitchesLanguageKindItemVariableDisplayName () const
                              {
                                return
                                  fOptionsMsrPitchesLanguageKindItemVariableDisplayName;
                              }
                              
    void                  setPitchesLanguageKindItemVariableValue (
                            msrQuarterTonesPitchesLanguageKind value)
                              {
                                fOptionsMsrPitchesLanguageKindItemVariable = value;
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

    string                fOptionsMsrPitchesLanguageKindItemVariableDisplayName;
    msrQuarterTonesPitchesLanguageKind&
                          fOptionsMsrPitchesLanguageKindItemVariable;
};
typedef SMARTP<optionsMsrPitchesLanguageItem> S_optionsMsrPitchesLanguageItem;
ostream& operator<< (ostream& os, const S_optionsMsrPitchesLanguageItem& elt);

//______________________________________________________________________________
class msrOptions : public optionsGroup
{
  public:

    static SMARTP<msrOptions> create (
      S_optionsHandler optionsHandler);
      
    SMARTP<msrOptions>        createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsrOptions (
                            bool boolOptionsInitialValue);
    
    void                  printMsrOptionsHelp ();

    void                  printMsrOptionsValues (int fieldWidth);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptions (
      S_optionsHandler optionsHandler);
  
    virtual ~msrOptions ();
 
    // set and get
    // ------------------------------------------------------

    bool                  setMsrQuarterTonesPitchesLanguage (
                            string language);    
 
  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceQuietness ();

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkOptionsConsistency ();

  public:

    // services
    // ------------------------------------------------------

    virtual S_optionsItem handleOptionsItem (
                            ostream&      os,
                            S_optionsItem item);
                            
    virtual void          handleOptionsItemValue (
                            ostream&      os,
                            S_optionsItem item,
                            string        theString);
  
  public:

    // trace and display
    // --------------------------------------
    
    bool                  fTraceMsr;
    
    bool                  fTraceMsrVisitors;
    
    bool                  fDisplayPartGroups;
    
    bool                  fDisplayMsr;
    bool                  fDisplayMsrDetails;
    
    bool                  fDisplayMsrSummary;
    bool                  fDisplayMsrNames;

    
    // languages
    // --------------------------------------
    
    msrQuarterTonesPitchesLanguageKind
                          fMsrQuarterTonesPitchesLanguageKind;

    
    // parts
    // --------------------------------------
    
    map<string, string>   fPartsRenamingMap;


    // voices
    // --------------------------------------
    
    bool                  fCreateVoicesStaffRelativeNumbers;

    
    // repeats
    // --------------------------------------
    
    bool                  fCreateImplicitInitialRepeatBarline;

    
    // notes
    // --------------------------------------
    
    bool                  fDelayRestsDynamics;
    bool                  fDelayRestsWords; // JMI
    bool                  fDelayRestsBeams; // JMI
    bool                  fDelayRestsSlurs; // JMI
    bool                  fDelayRestsLigatures; // JMI
    bool                  fDelayRestsPedals; // JMI
    bool                  fDelayRestsSlashes; // JMI
    bool                  fDelayRestsWedges; // JMI


    // lyrics
    // --------------------------------------
    
    bool                  fAddStanzasNumbers;

    
    // harmonies
    // --------------------------------------
    
    bool                  fShowHarmonyVoices;

    
    // figured bass
    // --------------------------------------
    
    bool                  fShowFiguredBassVoices;
};
typedef SMARTP<msrOptions> S_msrOptions;
EXP ostream& operator<< (ostream& os, const S_msrOptions& elt);

EXP extern S_msrOptions gMsrOptions;
EXP extern S_msrOptions gMsrOptionsUserChoices;
EXP extern S_msrOptions gMsrOptionsWithDetailedTrace;

//______________________________________________________________________________
void initializeMsrOptionsHandling (
  S_optionsHandler optionsHandler);


}


#endif
