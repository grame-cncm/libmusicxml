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
class optionsLpsrScoreOutputKindItem : public optionsValuedItem
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsLpsrScoreOutputKindItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsLpsrScoreOutputKindKindItemVariableDisplayName,
      lpsrScoreOutputKind&
                         optionsLpsrScoreOutputKindKindItemVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsLpsrScoreOutputKindItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsLpsrScoreOutputKindKindItemVariableDisplayName,
      lpsrScoreOutputKind&
                         optionsLpsrScoreOutputKindKindItemVariable);

    virtual ~optionsLpsrScoreOutputKindItem ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getOptionsLpsrScoreOutputKindKindItemVariableDisplayName () const
                              {
                                return
                                  fOptionsLpsrScoreOutputKindKindItemVariableDisplayName;
                              }

    void                  setScoreOutputKindKindItemVariableValue (
                            lpsrScoreOutputKind value)
                              {
                                fOptionsLpsrScoreOutputKindKindItemVariable = value;
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

    string                fOptionsLpsrScoreOutputKindKindItemVariableDisplayName;
    lpsrScoreOutputKind&  fOptionsLpsrScoreOutputKindKindItemVariable;
};
typedef SMARTP<optionsLpsrScoreOutputKindItem> S_optionsLpsrScoreOutputKindItem;
EXP ostream& operator<< (ostream& os, const S_optionsLpsrScoreOutputKindItem& elt);

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
EXP ostream& operator<< (ostream& os, const S_optionsLpsrPitchesLanguageItem& elt);

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
EXP ostream& operator<< (ostream& os, const S_optionsLpsrChordsLanguageItem& elt);

//______________________________________________________________________________
class optionsLpsrTransposeItem : public optionsValuedItem
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsLpsrTransposeItem> create (
      string  optionsItemShortName,
      string  optionsItemLongName,
      string  optionsItemDescription,
      string  optionsValueSpecification,
      string  optionsStringItemVariableDisplayName,
      string& optionsStringItemVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsLpsrTransposeItem (
      string  optionsItemShortName,
      string  optionsItemLongName,
      string  optionsItemDescription,
      string  optionsValueSpecification,
      string  optionsStringItemVariableDisplayName,
      string& optionsStringItemVariable);

    virtual ~optionsLpsrTransposeItem ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getOptionsTransposeItemVariableDisplayName () const
                              {
                                return
                                  fOptionsTransposeItemVariableDisplayName;
                              }

    void                  setTransposeItemVariableValue (
                            string value)
                              {
                                fOptionsTransposeItemVariable = value;
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

    string                fOptionsTransposeItemVariableDisplayName;
    string&               fOptionsTransposeItemVariable;
};
typedef SMARTP<optionsLpsrTransposeItem> S_optionsLpsrTransposeItem;
EXP ostream& operator<< (ostream& os, const S_optionsLpsrTransposeItem& elt);

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

    // public services
    // ------------------------------------------------------

    virtual S_optionsItem handleOptionsItem (
                            ostream&      os,
                            S_optionsItem item);

    virtual void          handleOptionsItemValue (
                            ostream&      os,
                            S_optionsItem item,
                            string        theString);

    void                  handleOptionsLpsrScoreOutputKindItemValue (
                            ostream&                         os,
                            S_optionsLpsrScoreOutputKindItem scoreOutputKindItem,
                            string                           theString);

    void                  handleOptionsLpsrPitchesLanguageItemValue (
                            ostream&                         os,
                            S_optionsLpsrPitchesLanguageItem pitchesLanguageKindItem,
                            string                           theString);

    void                  handleOptionsLpsrChordsLanguageItemValue (
                            ostream&                        os,
                            S_optionsLpsrChordsLanguageItem chordsLanguageItem,
                            string                          theString);

    void                  handleOptionsLpsrTransposeItemValue (
                            ostream&                   os,
                            S_optionsLpsrTransposeItem transposeItem,
                            string                     theString);

    void                  crackVersionNumber (
                            string theString,
                            int&   generationNumber,
                            int&   majorNumber,
                            int&   minorNumber);

    bool                  versionNumberGreaterThanOrEqualTo (
                            string otherVersionNumber);

  private:

    // private
    // ------------------------------------------------------

#ifdef TRACE_OPTIONS
    void                  initializeLpsrTraceOptions (
                            bool boolOptionsInitialValue);
#endif

    void                  initializeLpsrScoreOutputKindOptions (
                            bool boolOptionsInitialValue);

    void                  initializeLpsrLyricsVersusWordsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeLpsrLanguagesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeLpsrTransposeOptions (
                            bool boolOptionsInitialValue);

    void                  initializeLpsrExitAfterSomePassesOptions (
                            bool boolOptionsInitialValue);

  public:

    // print
    // ------------------------------------------------------

    void                  printLpsrOptionsHelp ();

    void                  printLpsrOptionsValues (int fieldWidth);

  public:

    // trace and display
    // --------------------------------------

    bool                  fTraceLpsr;

    bool                  fTraceLpsrVisitors;

    bool                  fTraceLpsrBlocks;

    bool                  fDisplayLpsr;

    bool                  fTraceSchemeFunctions;

    // LilyPond version
    // --------------------------------------

    string                fLilyPondVersion;

    // LilyPond output kind
    // --------------------------------------

    lpsrScoreOutputKind   fScoreOutputKind;

    // lyrics vs words
    // --------------------------------------

    bool                  fAddWordsFromTheLyrics;

    // languages
    // --------------------------------------

    msrQuarterTonesPitchesLanguageKind
                          fLpsrQuarterTonesPitchesLanguageKind;

    lpsrChordsLanguageKind
                          fLpsrChordsLanguageKind;

    // transpose
    // --------------------------------------

    // exit after some passes
    // --------------------------------------

    bool                  fExit3;
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
