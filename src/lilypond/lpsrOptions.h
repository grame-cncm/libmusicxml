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
#include "oahBasicTypes.h"
#include "exports.h"

namespace MusicXML2
{

//______________________________________________________________________________
class optionsLpsrScoreOutputKindItem : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsLpsrScoreOutputKindItem> create (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             optionsLpsrScoreOutputKindKindItemVariableDisplayName,
      lpsrScoreOutputKind&
                         optionsLpsrScoreOutputKindKindItemVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsLpsrScoreOutputKindItem (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             optionsLpsrScoreOutputKindKindItemVariableDisplayName,
      lpsrScoreOutputKind&
                         optionsLpsrScoreOutputKindKindItemVariable);

    virtual ~optionsLpsrScoreOutputKindItem ();

  public:

    // set and get
    // ------------------------------------------------------

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

    lpsrScoreOutputKind&  fOptionsLpsrScoreOutputKindKindItemVariable;
};
typedef SMARTP<optionsLpsrScoreOutputKindItem> S_optionsLpsrScoreOutputKindItem;
EXP ostream& operator<< (ostream& os, const S_optionsLpsrScoreOutputKindItem& elt);

//______________________________________________________________________________
class optionsLpsrPitchesLanguageItem : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsLpsrPitchesLanguageItem> create (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             optionsLpsrPitchesLanguageKindItemVariableDisplayName,
      msrQuarterTonesPitchesLanguageKind&
                         optionsLpsrPitchesLanguageKindItemVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsLpsrPitchesLanguageItem (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             optionsLpsrPitchesLanguageKindItemVariableDisplayName,
      msrQuarterTonesPitchesLanguageKind&
                         optionsLpsrPitchesLanguageKindItemVariable);

    virtual ~optionsLpsrPitchesLanguageItem ();

  public:

    // set and get
    // ------------------------------------------------------

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

    msrQuarterTonesPitchesLanguageKind&
                          fOptionsLpsrPitchesLanguageKindItemVariable;
};
typedef SMARTP<optionsLpsrPitchesLanguageItem> S_optionsLpsrPitchesLanguageItem;
EXP ostream& operator<< (ostream& os, const S_optionsLpsrPitchesLanguageItem& elt);

//______________________________________________________________________________
class optionsLpsrChordsLanguageItem : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsLpsrChordsLanguageItem> create (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             optionsLpsrChordsLanguageKindItemVariableDisplayName,
      lpsrChordsLanguageKind&
                         optionsLpsrChordsLanguageKindItemVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsLpsrChordsLanguageItem (
      string             oahAtomShortName,
      string             oahAtomLongName,
      string             oahAtomDescription,
      string             optionsValueSpecification,
      string             optionsLpsrChordsLanguageKindItemVariableDisplayName,
      lpsrChordsLanguageKind&
                         optionsLpsrChordsLanguageKindItemVariable);

    virtual ~optionsLpsrChordsLanguageItem ();

  public:

    // set and get
    // ------------------------------------------------------

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

    lpsrChordsLanguageKind&
                          fOptionsLpsrChordsLanguageKindItemVariable;
};
typedef SMARTP<optionsLpsrChordsLanguageItem> S_optionsLpsrChordsLanguageItem;
EXP ostream& operator<< (ostream& os, const S_optionsLpsrChordsLanguageItem& elt);

//______________________________________________________________________________
class optionsLpsrTransposeItem : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsLpsrTransposeItem> create (
      string  oahAtomShortName,
      string  oahAtomLongName,
      string  oahAtomDescription,
      string  optionsValueSpecification,
      string  oahStringAtomVariableDisplayName,
      S_msrSemiTonesPitchAndOctave&
              optionsLpsrTransposeItemVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsLpsrTransposeItem (
      string  oahAtomShortName,
      string  oahAtomLongName,
      string  oahAtomDescription,
      string  optionsValueSpecification,
      string  oahStringAtomVariableDisplayName,
      S_msrSemiTonesPitchAndOctave&
              optionsLpsrTransposeItemVariable);

    virtual ~optionsLpsrTransposeItem ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setTransposeItemVariableValue (
                            S_msrSemiTonesPitchAndOctave value)
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

    S_msrSemiTonesPitchAndOctave&
                          fOptionsTransposeItemVariable;
};
typedef SMARTP<optionsLpsrTransposeItem> S_optionsLpsrTransposeItem;
EXP ostream& operator<< (ostream& os, const S_optionsLpsrTransposeItem& elt);

//______________________________________________________________________________
class lpsrOptions : public oahGroup
{
  public:

    static SMARTP<lpsrOptions> create (
      S_oahHandler oahHandler);

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
      S_oahHandler oahHandler);

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

    virtual S_oahValuedAtom
                          handleOptionsItem (
                            ostream&      os,
                            S_oahAtom item);

    virtual void          handleOptionsItemValue (
                            ostream&      os,
                            S_oahAtom item,
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

    void                  crackLilypondVersionNumber (
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

    void                  initializeLpsrDisplayOptions (
                            bool boolOptionsInitialValue);

    void                  initializeLilypondScoreOutputOptions (
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

    // trace
    // --------------------------------------

#ifdef TRACE_OPTIONS
    bool                  fTraceLpsr;

    bool                  fTraceLilypondVersion;

    bool                  fTraceLpsrVisitors;

    bool                  fTraceLpsrBlocks;

    bool                  fTraceSchemeFunctions;
#endif

    // display
    // --------------------------------------

    bool                  fDisplayLpsr;

    // LilyPond version
    // --------------------------------------

    string                fLilyPondVersion;

    // score output kind
    // --------------------------------------

    lpsrScoreOutputKind   fScoreOutputKind;

    // global staff size
    // --------------------------------------

    float                 fGlobalStaffSize;
    float                 fStaffGlobalSizeDefaultValue;

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

    S_msrSemiTonesPitchAndOctave
                          fTransposeSemiTonesPitchAndOctave;

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
  S_oahHandler oahHandler);


}


#endif
