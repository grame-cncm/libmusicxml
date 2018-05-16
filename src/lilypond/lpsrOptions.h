/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __lpsrOptions__
#define __lpsrOptions__

#include "lpsrBasicTypes.h"
#include "optionsBasicTypes.h"
#include "exports.h"

namespace MusicXML2 
{

//______________________________________________________________________________
class optionsChordsLanguageItem : public optionsValuedItem
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<optionsChordsLanguageItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsChordsLanguageKindItemVariableDisplayName,
      lpsrChordsLanguageKind&
                         optionsChordsLanguageKindItemVariable);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsChordsLanguageItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsChordsLanguageKindItemVariableDisplayName,
      lpsrChordsLanguageKind&
                         optionsChordsLanguageKindItemVariable);
      
    virtual ~optionsChordsLanguageItem();

  public:
  
    // set and get
    // ------------------------------------------------------

    string                getOptionsChordsLanguageKindItemVariableDisplayName () const
                              {
                                return
                                  fOptionsChordsLanguageKindItemVariableDisplayName;
                              }
                              
    void                  setChordsLanguageKindItemVariableValue (
                            lpsrChordsLanguageKind value)
                              {
                                fOptionsChordsLanguageKindItemVariable = value;
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

    string                fOptionsChordsLanguageKindItemVariableDisplayName;
    lpsrChordsLanguageKind&
                          fOptionsChordsLanguageKindItemVariable;
};
typedef SMARTP<optionsChordsLanguageItem> S_optionsChordsLanguageItem;
ostream& operator<< (ostream& os, const S_optionsChordsLanguageItem& elt);

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

    // consistency check
    // ------------------------------------------------------

    void                  checkLpsrOptionsConsistency () const;

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

    // trace and display
    // --------------------------------------
    
    bool                  fTraceLpsr;
    
    bool                  fTraceLpsrVisitors;
    
    bool                  fDisplayLpsr;
    
    // Scheme functions
    bool                  fTraceSchemeFunctions;

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
