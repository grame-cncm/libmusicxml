/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrOptions___
#define ___bsrOptions___

#include "bsrBasicTypes.h"
#include "optionsBasicTypes.h"
#include "exports.h"

namespace MusicXML2 
{
/*
//______________________________________________________________________________
class optionsBsrPitchesLanguageItem : public optionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsBsrPitchesLanguageItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsBsrPitchesLanguageKindItemVariableDisplayName,
      msrQuarterTonesPitchesLanguageKind&
                         optionsBsrPitchesLanguageKindItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsBsrPitchesLanguageItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsBsrPitchesLanguageKindItemVariableDisplayName,
      msrQuarterTonesPitchesLanguageKind&
                         optionsBsrPitchesLanguageKindItemVariable);
      
    virtual ~optionsBsrPitchesLanguageItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsBsrPitchesLanguageKindItemVariableDisplayName () const
                              {
                                return
                                  fOptionsBsrPitchesLanguageKindItemVariableDisplayName;
                              }
                              
    void                  setPitchesLanguageKindItemVariableValue (
                            msrQuarterTonesPitchesLanguageKind value)
                              {
                                fOptionsBsrPitchesLanguageKindItemVariable = value;
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

    string                fOptionsBsrPitchesLanguageKindItemVariableDisplayName;
    msrQuarterTonesPitchesLanguageKind&
                          fOptionsBsrPitchesLanguageKindItemVariable;
};
typedef SMARTP<optionsBsrPitchesLanguageItem> S_optionsBsrPitchesLanguageItem;
ostream& operator<< (ostream& os, const S_optionsBsrPitchesLanguageItem& elt);

//______________________________________________________________________________
class optionsBsrChordsLanguageItem : public optionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsBsrChordsLanguageItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsBsrChordsLanguageKindItemVariableDisplayName,
      bsrChordsLanguageKind&
                         optionsBsrChordsLanguageKindItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsBsrChordsLanguageItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsBsrChordsLanguageKindItemVariableDisplayName,
      bsrChordsLanguageKind&
                         optionsBsrChordsLanguageKindItemVariable);
      
    virtual ~optionsBsrChordsLanguageItem ();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsBsrChordsLanguageKindItemVariableDisplayName () const
                              {
                                return
                                  fOptionsBsrChordsLanguageKindItemVariableDisplayName;
                              }
                              
    void                  setBsrChordsLanguageKindItemVariableValue (
                            bsrChordsLanguageKind value)
                              {
                                fOptionsBsrChordsLanguageKindItemVariable = value;
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

    string                fOptionsBsrChordsLanguageKindItemVariableDisplayName;
    bsrChordsLanguageKind&
                          fOptionsBsrChordsLanguageKindItemVariable;
};
typedef SMARTP<optionsBsrChordsLanguageItem> S_optionsBsrChordsLanguageItem;
ostream& operator<< (ostream& os, const S_optionsBsrChordsLanguageItem& elt);
*/

//______________________________________________________________________________
class bsrOptions : public optionsGroup
{
  public:

    static SMARTP<bsrOptions> create (
      S_optionsHandler optionsHandler);
    
    SMARTP<bsrOptions>        createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeBsrOptions (
                            bool boolOptionsInitialValue);
    
    void                  printBsrOptionsHelp ();

    void                  printBsrOptionsValues (int fieldWidth);
    
  public:
  
    // constructors/destructor
    // ------------------------------------------------------

    bsrOptions (
      S_optionsHandler optionsHandler);
    
    virtual ~bsrOptions ();

    // set and get
    // ------------------------------------------------------
/*
    bool                  setBsrQuarterTonesPitchesLanguage (
                            string language);    
 
    bool                  setBsrChordsLanguage (
                            string language);    
 */
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
    
    bool                  fTraceBsr;
    
    bool                  fTraceBsrVisitors;
    
    bool                  fDisplayBsr;
    
    // lyrics vs words
    // --------------------------------------

    bool                  fAddWordsFromTheLyrics;

    // languages
    // --------------------------------------
/*
    msrQuarterTonesPitchesLanguageKind
                          fBsrQuarterTonesPitchesLanguageKind;
    
    bsrChordsLanguageKind
                          fBsrChordsLanguageKind;
                          */
};
typedef SMARTP<bsrOptions> S_bsrOptions;
EXP ostream& operator<< (ostream& os, const S_bsrOptions& elt);

EXP extern S_bsrOptions gBsrOptions;
EXP extern S_bsrOptions gBsrOptionsUserChoices;
EXP extern S_bsrOptions gBsrOptionsWithDetailedTrace;

//______________________________________________________________________________
EXP  void initializeBsrOptionsHandling (
  S_optionsHandler optionsHandler);


}


#endif
