/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsrOptions___
#define ___lpsrOptions___

#include "lpsrBasicTypes.h"
#include "optionsBasicTypes.h"
#include "exports.h"

namespace MusicXML2 
{

//______________________________________________________________________________
class optionsLpsrPitchesLanguageItem : public optionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsLpsrPitchesLanguageItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsLpsrPitchesLanguageKindItemVariableDisplayName,
      msrQuarterTonesPitchesLanguageKind&
                         optionsLpsrPitchesLanguageKindItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsLpsrPitchesLanguageItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsLpsrPitchesLanguageKindItemVariableDisplayName,
      msrQuarterTonesPitchesLanguageKind&
                         optionsLpsrPitchesLanguageKindItemVariable);
      
    virtual ~optionsLpsrPitchesLanguageItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsLpsrPitchesLanguageKindItemVariableDisplayName () const
                              {
                                return
                                  fOptionsLpsrPitchesLanguageKindItemVariableDisplayName;
                              }
                              
    void                  setPitchesLanguageKindItemVariableValue (
                            msrQuarterTonesPitchesLanguageKind value)
                              {
                                fOptionsLpsrPitchesLanguageKindItemVariable = value;
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

    string                fOptionsLpsrPitchesLanguageKindItemVariableDisplayName;
    msrQuarterTonesPitchesLanguageKind&
                          fOptionsLpsrPitchesLanguageKindItemVariable;
};
typedef SMARTP<optionsLpsrPitchesLanguageItem> S_optionsLpsrPitchesLanguageItem;
ostream& operator<< (ostream& os, const S_optionsLpsrPitchesLanguageItem& elt);

//______________________________________________________________________________
class optionsLpsrChordsLanguageItem : public optionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsLpsrChordsLanguageItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsLpsrChordsLanguageKindItemVariableDisplayName,
      lpsrChordsLanguageKind&
                         optionsLpsrChordsLanguageKindItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsLpsrChordsLanguageItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsLpsrChordsLanguageKindItemVariableDisplayName,
      lpsrChordsLanguageKind&
                         optionsLpsrChordsLanguageKindItemVariable);
      
    virtual ~optionsLpsrChordsLanguageItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsLpsrChordsLanguageKindItemVariableDisplayName () const
                              {
                                return
                                  fOptionsLpsrChordsLanguageKindItemVariableDisplayName;
                              }
                              
    void                  setLpsrChordsLanguageKindItemVariableValue (
                            lpsrChordsLanguageKind value)
                              {
                                fOptionsLpsrChordsLanguageKindItemVariable = value;
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

    string                fOptionsLpsrChordsLanguageKindItemVariableDisplayName;
    lpsrChordsLanguageKind&
                          fOptionsLpsrChordsLanguageKindItemVariable;
};
typedef SMARTP<optionsLpsrChordsLanguageItem> S_optionsLpsrChordsLanguageItem;
ostream& operator<< (ostream& os, const S_optionsLpsrChordsLanguageItem& elt);

//______________________________________________________________________________
class lpsrOptions : public optionsGroup
{
  public:

    static SMARTP<lpsrOptions> create (
      S_optionsHandler optionsHandler);
    
    SMARTP<lpsrOptions>        createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeLpsrOptions (
                            bool boolOptionsInitialValue);
    
    void                  printLpsrOptionsHelp ();

    void                  printLpsrOptionsValues (int fieldWidth);
    
  public:
  
    // constructors/destructor
    // ------------------------------------------------------

    lpsrOptions (
      S_optionsHandler optionsHandler);
    
    virtual ~lpsrOptions ();

    // set and get
    // ------------------------------------------------------

    bool                  setLpsrQuarterTonesPitchesLanguage (
                            string language);    
 
    bool                  setLpsrChordsLanguage (
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
    
    bool                  fTraceLpsr;
    
    bool                  fTraceLpsrVisitors;
    
    bool                  fDisplayLpsr;
    
    bool                  fTraceSchemeFunctions;

    // lyrics vs words
    // --------------------------------------

    bool                  fAddWordsFromTheLyrics;

    // languages
    // --------------------------------------

    msrQuarterTonesPitchesLanguageKind
                          fLpsrQuarterTonesPitchesLanguageKind;
    
    lpsrChordsLanguageKind
                          fLpsrChordsLanguageKind;
};
typedef SMARTP<lpsrOptions> S_lpsrOptions;
EXP ostream& operator<< (ostream& os, const S_lpsrOptions& elt);

EXP extern S_lpsrOptions gLpsrOptions;
EXP extern S_lpsrOptions gLpsrOptionsUserChoices;
EXP extern S_lpsrOptions gLpsrOptionsWithDetailedTrace;

//______________________________________________________________________________
EXP  void initializeLpsrOptionsHandling (
  S_optionsHandler optionsHandler);


}


#endif
