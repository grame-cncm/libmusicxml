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

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "oahBasicTypes.h"


namespace MusicXML2
{

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
                            ostream& os) override;

    void                  handleValuedAtomValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printValuedAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

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
                            ostream& os) override;

    void                  handleValuedAtomValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printValuedAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

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
                            ostream& os) override;

    void                  handleValuedAtomValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printValuedAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------

    S_msrSemiTonesPitchAndOctave&
                          fMsrSemiTonesPitchAndOctaveVariable;
};
typedef SMARTP<lpsrTransposeAtom> S_lpsrTransposeAtom;
EXP ostream& operator<< (ostream& os, const S_lpsrTransposeAtom& elt);

/* JMI
//______________________________________________________________________________
class lpsrDalSegnoAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lpsrDalSegnoAtom> create (
      string              shortName,
      string              longName,
      string              description,
      string              valueSpecification,
      string              variableName,
      map<string, msrDalSegno::msrDalSegnoKind>&
                          stringDalSegnoKindMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrDalSegnoAtom (
      string              shortName,
      string              longName,
      string              description,
      string              valueSpecification,
      string              variableName,
      map<string, msrDalSegno::msrDalSegnoKind>&
                          stringDalSegnoKindMapVariable);

    virtual ~lpsrDalSegnoAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<string, msrDalSegno::msrDalSegnoKind>&
                          getStringDalSegnoKindMapVariable ()
                              { return fStringDalSegnoKindMapVariable; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os) override;

    void                  handleValuedAtomValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printValuedAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------

    map<string, msrDalSegno::msrDalSegnoKind>&
                          fStringDalSegnoKindMapVariable;
};
typedef SMARTP<lpsrDalSegnoAtom> S_lpsrDalSegnoAtom;
EXP ostream& operator<< (ostream& os, const S_lpsrDalSegnoAtom& elt);

//______________________________________________________________________________
class lpsrDalSegnoAlFineAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lpsrDalSegnoAlFineAtom> create (
      string              shortName,
      string              longName,
      string              description,
      string              valueSpecification,
      string              variableName,
      map<string, msrDalSegno::msrDalSegnoKind>&
                          stringDalSegnoKindMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrDalSegnoAlFineAtom (
      string              shortName,
      string              longName,
      string              description,
      string              valueSpecification,
      string              variableName,
      map<string, msrDalSegno::msrDalSegnoKind>&
                          stringDalSegnoKindMapVariable);

    virtual ~lpsrDalSegnoAlFineAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<string, msrDalSegno::msrDalSegnoKind>&
                          getStringDalSegnoKindMapVariable ()
                              { return fStringDalSegnoKindMapVariable; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os) override;

    void                  handleValuedAtomValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printValuedAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------

    map<string, msrDalSegno::msrDalSegnoKind>&
                          fStringDalSegnoKindMapVariable;
};
typedef SMARTP<lpsrDalSegnoAlFineAtom> S_lpsrDalSegnoAlFineAtom;
EXP ostream& operator<< (ostream& os, const S_lpsrDalSegnoAlFineAtom& elt);

//______________________________________________________________________________
class lpsrDalSegnoAlCodaAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lpsrDalSegnoAlCodaAtom> create (
      string              shortName,
      string              longName,
      string              description,
      string              valueSpecification,
      string              variableName,
      map<string, msrDalSegno::msrDalSegnoKind>&
                          stringDalSegnoKindMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrDalSegnoAlCodaAtom (
      string              shortName,
      string              longName,
      string              description,
      string              valueSpecification,
      string              variableName,
      map<string, msrDalSegno::msrDalSegnoKind>&
                          stringDalSegnoKindMapVariable);

    virtual ~lpsrDalSegnoAlCodaAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<string, msrDalSegno::msrDalSegnoKind>&
                          getStringDalSegnoKindMapVariable ()
                              { return fStringDalSegnoKindMapVariable; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os) override;

    void                  handleValuedAtomValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printValuedAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------

    map<string, msrDalSegno::msrDalSegnoKind>&
                          fStringDalSegnoKindMapVariable;
};
typedef SMARTP<lpsrDalSegnoAlCodaAtom> S_lpsrDalSegnoAlCodaAtom;
EXP ostream& operator<< (ostream& os, const S_lpsrDalSegnoAlCodaAtom& elt);
*/

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

    // public services
    // ------------------------------------------------------

    void                  crackLilypondVersionNumber (
                            string theString,
                            int&   generationNumber,
                            int&   majorNumber,
                            int&   minorNumber);

    bool                  versionNumberGreaterThanOrEqualTo (
                            string otherVersionNumber);

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    virtual void          checkGroupOptionsConsistency () override;

  private:

    // private
    // ------------------------------------------------------

#ifdef TRACE_OAH
    void                  initializeLpsrTraceOah (
                            bool boolOptionsInitialValue);
#endif

    void                  initializeLpsrDisplayOptions (
                            bool boolOptionsInitialValue);

    void                  initializeLpsrLilypondVersionOptions (
                            bool boolOptionsInitialValue);

    void                  initializeLpsrGlobalStaffSizeOptions (
                            bool boolOptionsInitialValue);

    void                  initializeLpsrPaperOptions (
                            bool boolOptionsInitialValue);

    void                  initializeLpsrMeasuresOptions (
                            bool boolOptionsInitialValue);

    void                  initializeLpsrTemposOptions (
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

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

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
    bool                  fDisplayLpsrShort;

    // LilyPond version
    // --------------------------------------

    string                fLilyPondVersion;

    // global staff size
    // --------------------------------------

    float                 fGlobalStaffSize;
    float                 fStaffGlobalSizeDefaultValue;
    bool                  fStaffGlobalSizeHasBeenSet;

    // paper
    // --------------------------------------

    bool                  fAllPaperVariables;

    msrLengthUnitKind     fLengthUnitKind;
    msrLengthUnitKind     fLengthUnitKindDefaultValue;

    msrLength             fPaperHeight;
    S_oahLengthAtom       fPaperHeightAtom;

    msrLength             fPaperWidth;
    S_oahLengthAtom       fPaperWidthAtom;

    // indents
    msrLength             fPaperHorizontalShift;
    msrLength             fPaperIndent;
    msrLength             fPaperShortIndent;

    // spaces
    msrLength             fMarkupSystemSpacingPadding;
    msrLength             fBetweenSystemSpace;
    msrLength             fPageTopSpace;

    // counts
    int                   fPageCount;   // null if not specified
    int                   fSystemCount; // null if not specified

    bool                  fRaggedBottom;
    bool                  fRaggedLast;
    bool                  fRaggedLastBottom;
    bool                  fRaggedRight;

    bool                  fTagline;

    // measures
    // --------------------------------------

    // replicate empty measure JMI ???
    string                fReplicateEmptyMeasureNumber;
    int                   fReplicateEmptyMeasureReplicas;

    // add empty measures
// JMI    map<string,int>       fAddEmptyMeasuresStringToIntMap;

    // tempos
    // --------------------------------------

    // convert tempos to rehearsal marks
    bool                  fConvertTemposToRehearsalMarks;

    // words
    // --------------------------------------

    // convert words to tempo
    bool                  fConvertWordsToTempo;
    // add words from the lyrics
    bool                  fAddWordsFromTheLyrics;

    // convert words to rehearsal marks
    bool                  fConvertWordsToRehearsalMarks;

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

EXP extern S_lpsrOah gGlobalLpsrOah;
EXP extern S_lpsrOah gGlobalLpsrOahUserChoices;
EXP extern S_lpsrOah gGlobalLpsrOahWithDetailedTrace;

//______________________________________________________________________________
EXP  void initializeLpsrOahHandling (
  S_oahHandler handler);


}


#endif
