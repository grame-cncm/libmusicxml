/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsrOah___
#define ___lpsrOah___

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

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
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

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
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

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
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

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
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
class lpsrOah : public oahGroup
{
  public:

    static SMARTP<lpsrOah> create (
      S_oahHandler handlerUpLink);

    SMARTP<lpsrOah>       createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeLpsrOah (
                            bool boolOptionsInitialValue);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrOah (
      S_oahHandler handlerUpLink);

    virtual ~lpsrOah ();

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

#ifdef TRACE_OAH
    void                  initializeLpsrTraceOah (
                            bool boolOptionsInitialValue);
#endif

    void                  initializeLpsrDisplayOptions (
                            bool boolOptionsInitialValue);

    void                  initializeLilypondScoreOutputOptions (
                            bool boolOptionsInitialValue);

    void                  initializeLpsrMeasuresOptions (
                            bool boolOptionsInitialValue);

    void                  initializeLpsrWordsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeLpsrLanguagesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeLpsrTransposeOptions (
                            bool boolOptionsInitialValue);

    void                  initializeLpsrExitAfterSomePassesOptions (
                            bool boolOptionsInitialValue);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  printLpsrOahHelp ();

    void                  printLpsrOahValues (int fieldWidth);

  public:

    // trace
    // --------------------------------------

#ifdef TRACE_OAH
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

    // measures
    // --------------------------------------

    // replicate empty measure
    string                fReplicateEmptyMeasureNumber;
    int                   fReplicateEmptyMeasureReplicas;

    // words
    // --------------------------------------

    // convert words to tempo
    bool                  fConvertWordsToTempo;
    // add words from the lyrics
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
typedef SMARTP<lpsrOah> S_lpsrOah;
EXP ostream& operator<< (ostream& os, const S_lpsrOah& elt);

EXP extern S_lpsrOah gLpsrOah;
EXP extern S_lpsrOah gLpsrOahUserChoices;
EXP extern S_lpsrOah gLpsrOahWithDetailedTrace;

//______________________________________________________________________________
EXP  void initializeLpsrOahHandling (
  S_oahHandler handler);


}


#endif
