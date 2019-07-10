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
class lpsrScoreOutputKindAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lpsrScoreOutputKindAtom> create (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      lpsrScoreOutputKind&
                         lpsrScoreOutputKindKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrScoreOutputKindAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      lpsrScoreOutputKind&
                         lpsrScoreOutputKindKindVariable);

    virtual ~lpsrScoreOutputKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setLpsrScoreOutputKindVariable (
                            lpsrScoreOutputKind value)
                              { fLpsrScoreOutputKindVariable = value; }

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string           theString,
                            indentedOstream& os);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    lpsrScoreOutputKind&  fLpsrScoreOutputKindVariable;
};
typedef SMARTP<lpsrScoreOutputKindAtom> S_lpsrScoreOutputKindAtom;
EXP ostream& operator<< (ostream& os, const S_lpsrScoreOutputKindAtom& elt);

//______________________________________________________________________________
class lpsrPitchesLanguageAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lpsrPitchesLanguageAtom> create (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      msrQuarterTonesPitchesLanguageKind&
                         lpsrPitchesLanguageKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrPitchesLanguageAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      msrQuarterTonesPitchesLanguageKind&
                         lpsrPitchesLanguageKindVariable);

    virtual ~lpsrPitchesLanguageAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMsrQuarterTonesPitchesLanguageKindVariable (
                            msrQuarterTonesPitchesLanguageKind value)
                              {
                                fMsrQuarterTonesPitchesLanguageKindVariable = value;
                              }

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string           theString,
                            indentedOstream& os);

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
                          fMsrQuarterTonesPitchesLanguageKindVariable;
};
typedef SMARTP<lpsrPitchesLanguageAtom> S_lpsrPitchesLanguageAtom;
EXP ostream& operator<< (ostream& os, const S_lpsrPitchesLanguageAtom& elt);

//______________________________________________________________________________
class lpsrChordsLanguageAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lpsrChordsLanguageAtom> create (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      lpsrChordsLanguageKind&
                         lpsrChordsLanguageKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrChordsLanguageAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      lpsrChordsLanguageKind&
                         lpsrChordsLanguageKindVariable);

    virtual ~lpsrChordsLanguageAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setLpsrChordsLanguageKindVariable (
                            lpsrChordsLanguageKind value)
                              {
                                fLpsrChordsLanguageKindVariable = value;
                              }

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string           theString,
                            indentedOstream& os);

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
                          fLpsrChordsLanguageKindVariable;
};
typedef SMARTP<lpsrChordsLanguageAtom> S_lpsrChordsLanguageAtom;
EXP ostream& operator<< (ostream& os, const S_lpsrChordsLanguageAtom& elt);

//______________________________________________________________________________
class lpsrTransposeAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lpsrTransposeAtom> create (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      S_msrSemiTonesPitchAndOctave&
              lpsrTransposeVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrTransposeAtom (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      S_msrSemiTonesPitchAndOctave&
              lpsrTransposeVariable);

    virtual ~lpsrTransposeAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMsrSemiTonesPitchAndOctaveVariable (
                            S_msrSemiTonesPitchAndOctave value)
                              {
                                fMsrSemiTonesPitchAndOctaveVariable = value;
                              }

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string           theString,
                            indentedOstream& os);

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
                          fMsrSemiTonesPitchAndOctaveVariable;
};
typedef SMARTP<lpsrTransposeAtom> S_lpsrTransposeAtom;
EXP ostream& operator<< (ostream& os, const S_lpsrTransposeAtom& elt);

//______________________________________________________________________________
class lpsrOptions : public oahGroup
{
  public:

    static SMARTP<lpsrOptions> create (
      S_oahHandler handler);

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
      S_oahHandler handler);

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
                          handleAtom (
                            ostream&  os,
                            S_oahAtom atom);

    virtual void          handleValuedAtomValue (
                            indentedOstream& os,
                            S_oahAtom        atom,
                            string           theString);

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
  S_oahHandler handler);


}


#endif
