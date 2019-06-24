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

    map<string, string>&  fOptionsPartRenameItemVariable;
};
typedef SMARTP<optionsPartRenameItem> S_optionsPartRenameItem;
EXP ostream& operator<< (ostream& os, const S_optionsPartRenameItem& elt);

//______________________________________________________________________________
class optionsPartTransposeItem : public optionsValuedItem
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsPartTransposeItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsPartTransposeItemVariableDisplayName,
      map<string, S_msrSemiTonesPitchAndOctave>&
                         optionsPartTransposeItemVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsPartTransposeItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsPartTransposeItemVariableDisplayName,
      map<string, S_msrSemiTonesPitchAndOctave>&
                         optionsPartTransposeItemVariable);

    virtual ~optionsPartTransposeItem ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setPartTransposeItemVariableValue (
                            string  partName,
                            S_msrSemiTonesPitchAndOctave
                                    semiTonesPitchAndOctave)
                              {
                                fOptionsPartTransposeItemVariable [partName] =
                                  semiTonesPitchAndOctave;
                              }

    const map<string, S_msrSemiTonesPitchAndOctave>&
                          getOptionsPartTransposeItemVariable ()
                              {
                                return
                                  fOptionsPartTransposeItemVariable;
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

    map<string, S_msrSemiTonesPitchAndOctave>&
                          fOptionsPartTransposeItemVariable;
};
typedef SMARTP<optionsPartTransposeItem> S_optionsPartTransposeItem;
EXP ostream& operator<< (ostream& os, const S_optionsPartTransposeItem& elt);

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

    msrQuarterTonesPitchesLanguageKind&
                          fOptionsMsrPitchesLanguageKindItemVariable;
};
typedef SMARTP<optionsMsrPitchesLanguageItem> S_optionsMsrPitchesLanguageItem;
EXP ostream& operator<< (ostream& os, const S_optionsMsrPitchesLanguageItem& elt);

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

    // public services
    // ------------------------------------------------------

    virtual S_optionsItem handleOptionsItem (
                            ostream&      os,
                            S_optionsItem item);

    virtual void          handleOptionsItemValue (
                            ostream&      os,
                            S_optionsItem item,
                            string        theString);

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACE_OPTIONS
    void                  initializeMsrTraceOptions (
                            bool boolOptionsInitialValue);
#endif

    void                  initializeMsrDisplayOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsrLanguagesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsrPartsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsrStavesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsrVoicesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsrRepeatsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsrNotesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsrLyricsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsrHarmoniesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsrFiguredBassOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsrExitAfterSomePassesOptions (
                            bool boolOptionsInitialValue);

    virtual void          handleOptionsPartRenameItemValue (
                            ostream&                os,
                            S_optionsPartRenameItem partRenameItem,
                            string                  theString);

    virtual void          handleOptionsPartTransposeItemValue (
                            ostream&                   os,
                            S_optionsPartTransposeItem partTransposeItem,
                            string                     theString);

    virtual void          handleOptionsMsrPitchesLanguageItemValue (
                            ostream&                        os,
                            S_optionsMsrPitchesLanguageItem pitchesLanguageKindItem,
                            string                          theString);

  public:

    // print
    // ------------------------------------------------------

    void                  printMsrOptionsHelp ();

    void                  printMsrOptionsValues (int fieldWidth);

  public:

    // trace
    // --------------------------------------

    bool                  fTraceMsr;

    bool                  fTraceMsrVisitors;

    // display
    // --------------------------------------

    bool                  fDisplayPartGroups;

    bool                  fDisplayMsr;
    bool                  fDisplayMsrDetails;

    bool                  fDisplayMsrNames;
    bool                  fDisplayMsrSummary;


    // languages
    // --------------------------------------

    msrQuarterTonesPitchesLanguageKind
                          fMsrQuarterTonesPitchesLanguageKind;

    // parts
    // --------------------------------------

    map<string, string>   fPartsRenamingMap;
    map<string, S_msrSemiTonesPitchAndOctave>
                          fPartsTranspositionMap;


    // staves
    // --------------------------------------

    bool                  fCreateSingleLineStavesAsRythmic;


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

    bool                  fAddStanzasNumbers; // LPSR ??? JMI


    // harmonies
    // --------------------------------------

    bool                  fShowHarmonyVoices;


    // figured bass
    // --------------------------------------

    bool                  fShowFiguredBassVoices;


    // exit after some passes
    // --------------------------------------

    bool                  fExit2a;
    bool                  fExit2b;
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
