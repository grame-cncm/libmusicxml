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

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahAtomsCollection.h"


namespace MusicXML2
{

//______________________________________________________________________________
class lpsrPitchesLanguageAtom : public oahAtomWithValue
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

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
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

    void                  printAtomWithValueOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrQuarterTonesPitchesLanguageKind&
                          fMsrQuarterTonesPitchesLanguageKindVariable;
};
typedef SMARTP<lpsrPitchesLanguageAtom> S_lpsrPitchesLanguageAtom;
EXP ostream& operator<< (ostream& os, const S_lpsrPitchesLanguageAtom& elt);

//______________________________________________________________________________
class lpsrChordsLanguageAtom : public oahAtomWithValue
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

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
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

    void                  printAtomWithValueOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    lpsrChordsLanguageKind&
                          fLpsrChordsLanguageKindVariable;
};
typedef SMARTP<lpsrChordsLanguageAtom> S_lpsrChordsLanguageAtom;
EXP ostream& operator<< (ostream& os, const S_lpsrChordsLanguageAtom& elt);

//______________________________________________________________________________
class lpsrTransposeAtom : public oahAtomWithValue
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
              semiTonesPitchAndOctaveVariable);

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
              semiTonesPitchAndOctaveVariable);

    virtual ~lpsrTransposeAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setSemiTonesPitchAndOctaveVariable (
                            S_msrSemiTonesPitchAndOctave value)
                              {
                                fSemiTonesPitchAndOctaveVariable = value;
                              }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
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

    void                  printAtomWithValueOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrSemiTonesPitchAndOctave&
                          fSemiTonesPitchAndOctaveVariable;
};
typedef SMARTP<lpsrTransposeAtom> S_lpsrTransposeAtom;
EXP ostream& operator<< (ostream& os, const S_lpsrTransposeAtom& elt);

//______________________________________________________________________________
class lpsrOahGroup : public oahGroup
{
  public:

    static SMARTP<lpsrOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeLpsrOahGroup ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrOahGroup ();

    virtual ~lpsrOahGroup ();

    // set and get
    // ------------------------------------------------------

    // trace
    // --------------------------------------

#ifdef TRACING_IS_ENABLED
    bool                  getTraceLpsr () const
                              { return fTraceLpsr; }

    bool                  getTraceLilypondVersion () const
                              { return fTraceLilypondVersion; }

    bool                  getTraceLpsrVisitors () const
                              { return fTraceLpsrVisitors; }

    bool                  getTraceLpsrBlocks () const
                              { return fTraceLpsrBlocks; }

    bool                  getTraceSchemeFunctions () const
                              { return fTraceSchemeFunctions; }
#endif

    // display
    // --------------------------------------

    bool                  getDisplayLpsr () const
                              { return fDisplayLpsr; }
    bool                  getDisplayLpsrShort () const
                              { return fDisplayLpsrShort; }

    // LilyPond version
    // --------------------------------------

    string                getLilyPondVersion () const
                              { return fLilyPondVersion; }

    // global staff size
    // --------------------------------------

    float                 getGlobalStaffSize () const
                              { return fGlobalStaffSize; }

    float                 getStaffGlobalSizeDefaultValue () const
                              { return fStaffGlobalSizeDefaultValue; }

    bool                  getStaffGlobalSizeHasBeenSet () const // JMI ???
                              { return fStaffGlobalSizeHasBeenSet; }

    // paper
    // --------------------------------------

    bool                  getAllPaperVariables () const
                              { return fAllPaperVariables; }

    msrLengthUnitKind     getLengthUnitKind () const
                              { return fLengthUnitKind; }
    msrLengthUnitKind     getLengthUnitKindDefaultValue () const
                              { return fLengthUnitKindDefaultValue; }

    msrLength             getPaperHeight () const
                              { return fPaperHeight; }
    S_oahLengthAtom       getPaperHeightAtom () const
                              { return fPaperHeightAtom; }

    msrLength             getPaperWidth () const
                              { return fPaperWidth; }
    S_oahLengthAtom       getPaperWidthAtom () const
                              { return fPaperWidthAtom; }

    // indents
    msrLength             getPaperHorizontalShift () const
                              { return fPaperHorizontalShift; }
    msrLength             getPaperIndent () const
                              { return fPaperIndent; }
    msrLength             getPaperShortIndent () const
                              { return fPaperShortIndent; }

    // spaces
    msrLength             getMarkupSystemSpacingPadding () const
                              { return fMarkupSystemSpacingPadding; }
    msrLength             getBetweenSystemSpace () const
                              { return fBetweenSystemSpace; }
    msrLength             getPageTopSpace () const
                              { return fPageTopSpace; }

    // counts
    int                   getPageCount () const
                              { return fPageCount; }
    int                   getSystemCount () const
                              { return fSystemCount; }

    bool                  getRaggedBottom () const
                              { return fRaggedBottom; }
    bool                  getRaggedLast () const
                              { return fRaggedLast; }
    bool                  getRaggedLastBottom () const
                              { return fRaggedLastBottom; }
    bool                  getRaggedRight () const
                              { return fRaggedRight; }

    bool                  getTagline () const
                              { return fTagline; }

    // measures
    // --------------------------------------

    // replicate empty measure JMI ???
    string                getReplicateEmptyMeasureNumber () const
                              { return fReplicateEmptyMeasureNumber; }
    int                   getReplicateEmptyMeasureReplicas () const
                              { return fReplicateEmptyMeasureReplicas; }

    // add empty measures
// JMI    map<string,int>       getAddEmptyMeasuresStringToIntMap () const;

    // tempos
    // --------------------------------------

    // convert tempos to rehearsal marks
    bool                  getConvertTemposToRehearsalMarks () const
                              { return fConvertTemposToRehearsalMarks; }

    // words
    // --------------------------------------

    // convert words to tempo
    bool                  getConvertWordsToTempo () const
                              { return fConvertWordsToTempo; }
    // add words from the lyrics
    bool                  getAddWordsFromTheLyrics () const
                              { return fAddWordsFromTheLyrics; }

    // convert words to rehearsal marks
    bool                  getConvertWordsToRehearsalMarks () const
                              { return fConvertWordsToRehearsalMarks; }

    // languages
    // --------------------------------------

    msrQuarterTonesPitchesLanguageKind
                          getLpsrQuarterTonesPitchesLanguageKind () const
                              { return fLpsrQuarterTonesPitchesLanguageKind; }

    lpsrChordsLanguageKind
                          getLpsrChordsLanguageKind () const
                              { return fLpsrChordsLanguageKind; }

    // transpose
    // --------------------------------------

    S_msrSemiTonesPitchAndOctave
                          getTransposeSemiTonesPitchAndOctave () const
                              { return fTransposeSemiTonesPitchAndOctave; }

    // quit after some passes
    // --------------------------------------

    bool                  getQuit3 () const
                              { return fQuitAfterPass3; }

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

#ifdef TRACING_IS_ENABLED
    void                  initializeLpsrTraceOah ();
#endif

    void                  initializeLpsrDisplayOptions ();

    void                  initializeLpsrLilypondVersionOptions ();

    void                  initializeLpsrGlobalStaffSizeOptions ();

    void                  initializeLpsrPaperOptions ();

    void                  initializeLpsrMeasuresOptions ();

    void                  initializeLpsrTemposOptions ();

    void                  initializeLpsrWordsOptions ();

    void                  initializeLpsrLanguagesOptions ();

    void                  initializeLpsrTransposeOptions ();

    void                  initializeLpsrQuitAfterSomePassesOptions ();

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

  private:

    // private fields
    // --------------------------------------

    // trace
    // --------------------------------------

#ifdef TRACING_IS_ENABLED
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

    // quit after some passes
    // --------------------------------------

    bool                  fQuitAfterPass3;
};
typedef SMARTP<lpsrOahGroup> S_lpsrOahGroup;
EXP ostream& operator<< (ostream& os, const S_lpsrOahGroup& elt);

EXP extern S_lpsrOahGroup gGlobalLpsrOahGroup;

//______________________________________________________________________________
EXP S_lpsrOahGroup createGlobalLpsrOahGroup ();


}


#endif

/* JMI
//______________________________________________________________________________
class lpsrDalSegnoAtom : public oahAtomWithValue
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
                          stringToDalSegnoKindMapVariable);

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
                          stringToDalSegnoKindMapVariable);

    virtual ~lpsrDalSegnoAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<string, msrDalSegno::msrDalSegnoKind>&
                          getStringToDalSegnoKindMapVariable () const
                              { return fStringToDalSegnoKindMapVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
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

    void                  printAtomWithValueOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    map<string, msrDalSegno::msrDalSegnoKind>&
                          fStringToDalSegnoKindMapVariable;
};
typedef SMARTP<lpsrDalSegnoAtom> S_lpsrDalSegnoAtom;
EXP ostream& operator<< (ostream& os, const S_lpsrDalSegnoAtom& elt);

//______________________________________________________________________________
class lpsrDalSegnoAlFineAtom : public oahAtomWithValue
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
                          stringToDalSegnoKindMapVariable);

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
                          stringToDalSegnoKindMapVariable);

    virtual ~lpsrDalSegnoAlFineAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<string, msrDalSegno::msrDalSegnoKind>&
                          getStringToDalSegnoKindMapVariable () const
                              { return fStringToDalSegnoKindMapVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
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

    void                  printAtomWithValueOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    map<string, msrDalSegno::msrDalSegnoKind>&
                          fStringToDalSegnoKindMapVariable;
};
typedef SMARTP<lpsrDalSegnoAlFineAtom> S_lpsrDalSegnoAlFineAtom;
EXP ostream& operator<< (ostream& os, const S_lpsrDalSegnoAlFineAtom& elt);

//______________________________________________________________________________
class lpsrDalSegnoAlCodaAtom : public oahAtomWithValue
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
                          stringToDalSegnoKindMapVariable);

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
                          stringToDalSegnoKindMapVariable);

    virtual ~lpsrDalSegnoAlCodaAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<string, msrDalSegno::msrDalSegnoKind>&
                          getStringToDalSegnoKindMapVariable () const
                              { return fStringToDalSegnoKindMapVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
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

    void                  printAtomWithValueOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    map<string, msrDalSegno::msrDalSegnoKind>&
                          fStringToDalSegnoKindMapVariable;
};
typedef SMARTP<lpsrDalSegnoAlCodaAtom> S_lpsrDalSegnoAlCodaAtom;
EXP ostream& operator<< (ostream& os, const S_lpsrDalSegnoAlCodaAtom& elt);
*/

