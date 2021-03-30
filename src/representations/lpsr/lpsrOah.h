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
class EXP lpsrPitchesLanguageAtom : public oahAtomStoringAValueInAVariable
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

    virtual               ~lpsrPitchesLanguageAtom ();

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
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrQuarterTonesPitchesLanguageKind&
                          fMsrQuarterTonesPitchesLanguageKindVariable;
};
typedef SMARTP<lpsrPitchesLanguageAtom> S_lpsrPitchesLanguageAtom;
EXP ostream& operator<< (ostream& os, const S_lpsrPitchesLanguageAtom& elt);

//______________________________________________________________________________
class EXP lpsrChordsLanguageAtom : public oahAtomStoringAValueInAVariable
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

    virtual               ~lpsrChordsLanguageAtom ();

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
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    lpsrChordsLanguageKind&
                          fLpsrChordsLanguageKindVariable;
};
typedef SMARTP<lpsrChordsLanguageAtom> S_lpsrChordsLanguageAtom;
EXP ostream& operator<< (ostream& os, const S_lpsrChordsLanguageAtom& elt);

//______________________________________________________________________________
class EXP lpsrTransposeAtom : public oahAtomStoringAValueInAVariable
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

    virtual               ~lpsrTransposeAtom ();

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
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrSemiTonesPitchAndOctave&
                          fSemiTonesPitchAndOctaveVariable;
};
typedef SMARTP<lpsrTransposeAtom> S_lpsrTransposeAtom;
EXP ostream& operator<< (ostream& os, const S_lpsrTransposeAtom& elt);

//______________________________________________________________________________
class EXP lpsrOahGroup : public oahGroup
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

    virtual               ~lpsrOahGroup ();

    // set and get
    // ------------------------------------------------------

    // trace
    // --------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  setTraceLpsr ()
                              { fTraceLpsr = true; }
    bool                  getTraceLpsr () const
                              { return fTraceLpsr; }

    void                  setTraceLilypondVersion ()
                              { fTraceLilypondVersion = true; }
    bool                  getTraceLilypondVersion () const
                              { return fTraceLilypondVersion; }

    void                  setTraceLpsrVisitors ()
                              { fTraceLpsrVisitors = true; }
    bool                  getTraceLpsrVisitors () const
                              { return fTraceLpsrVisitors; }

    void                  setTraceLpsrBlocks ()
                              { fTraceLpsrBlocks = true; }
    bool                  getTraceLpsrBlocks () const
                              { return fTraceLpsrBlocks; }

    void                  setTraceSchemeFunctions ()
                              { fTraceSchemeFunctions = true; }
    bool                  getTraceSchemeFunctions () const
                              { return fTraceSchemeFunctions; }
#endif

    // display
    // --------------------------------------

    void                  setDisplayLpsr ()
                              { fDisplayLpsr = true; }
    bool                  getDisplayLpsr () const
                              { return fDisplayLpsr; }

    void                  setDisplayLpsrFull ()
                              { fDisplayLpsrFull = true; }
    bool                  getDisplayLpsrFull () const
                              { return fDisplayLpsrFull; }

    void                  setDisplayLpsrShort ()
                              { fDisplayLpsrShort = true; }
    bool                  getDisplayLpsrShort () const
                              { return fDisplayLpsrShort; }

    // paper
    // --------------------------------------

    void                  setAllPaperVariables ()
                              { fAllPaperVariables = true; }
    bool                  getAllPaperVariables () const
                              { return fAllPaperVariables; }

    msrLengthUnitKind     getLengthUnitKindDefaultValue () const
                              { return fLengthUnitKindDefaultValue; }

    // size
    msrLength             getPaperHeight () const
                              { return fPaperHeight; }
    S_oahLengthAtom       getPaperHeightAtom () const
                              { return fPaperHeightAtom; }

    msrLength             getPaperWidth () const
                              { return fPaperWidth; }
    S_oahLengthAtom       getPaperWidthAtom () const
                              { return fPaperWidthAtom; }

    // margins
    msrLength             getPaperLeftMargin () const
                              { return fPaperLeftMargin; }
    S_oahLengthAtom       getPaperLeftMarginAtom () const
                              { return fPaperLeftMarginAtom; }

    msrLength             getPaperRightMargin () const
                              { return fPaperRightMargin; }
    S_oahLengthAtom       getPaperRightMarginAtom () const
                              { return fPaperRightMarginAtom; }

    msrLength             getPaperTopMargin () const
                              { return fPaperTopMargin; }
    S_oahLengthAtom       getPaperTopMarginAtom () const
                              { return fPaperTopMarginAtom; }

    msrLength             getPaperBottomMargin () const
                              { return fPaperBottomMargin; }
    S_oahLengthAtom       getPaperBottomMarginAtom () const
                              { return fPaperBottomMarginAtom; }

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
    void                  setPageCount (int value)
                              { fPageCount = value; }
    int                   getPageCount () const
                              { return fPageCount; }

    void                  setSystemCount (int value)
                              { fSystemCount = value; }
    int                   getSystemCount () const
                              { return fSystemCount; }

    void                  setRaggedBottom ()
                              { fRaggedBottom = true; }
    bool                  getRaggedBottom () const
                              { return fRaggedBottom; }

    void                  setRaggedLast ()
                              { fRaggedLast = true; }
    bool                  getRaggedLast () const
                              { return fRaggedLast; }

    void                  setRaggedLastBottom ()
                              { fRaggedLastBottom = true; }
    bool                  getRaggedLastBottom () const
                              { return fRaggedLastBottom; }

    void                  setRaggedRight ()
                              { fRaggedRight = true; }
    bool                  getRaggedRight () const
                              { return fRaggedRight; }

    void                  setTagline ()
                              { fTagline = true; }
    bool                  getTagline () const
                              { return fTagline; }

    // measures
    // --------------------------------------

    // replicate empty measure JMI ???
    string                getReplicateEmptyMeasureNumber () const
                              { return fReplicateEmptyMeasureNumber; }

    void                  setReplicateEmptyMeasureReplicas ()
                              { fReplicateEmptyMeasureReplicas = true; }
    int                   getReplicateEmptyMeasureReplicas () const
                              { return fReplicateEmptyMeasureReplicas; }

    // add empty measures
// JMI    map<string,int>       getAddEmptyMeasuresStringToIntMap () const;

    // tempos
    // --------------------------------------

    // convert tempos to rehearsal marks
    void                  setConvertTemposToRehearsalMarks ()
                              { fConvertTemposToRehearsalMarks = true; }
    bool                  getConvertTemposToRehearsalMarks () const
                              { return fConvertTemposToRehearsalMarks; }

    // words
    // --------------------------------------

    // convert words to tempo
    void                  setConvertWordsToTempo ()
                              { fConvertWordsToTempo = true; }
    bool                  getConvertWordsToTempo () const
                              { return fConvertWordsToTempo; }

    // add words from the lyrics
    void                  setAddWordsFromTheLyrics ()
                              { fAddWordsFromTheLyrics = true; }
    bool                  getAddWordsFromTheLyrics () const
                              { return fAddWordsFromTheLyrics; }

    // convert words to rehearsal marks
    void                  setConvertWordsToRehearsalMarks ()
                              { fConvertWordsToRehearsalMarks = true; }
    bool                  getConvertWordsToRehearsalMarks () const
                              { return fConvertWordsToRehearsalMarks; }

    // languages
    // --------------------------------------

    bool                  setLpsrQuarterTonesPitchesLanguage (string language);

    msrQuarterTonesPitchesLanguageKind
                          getLpsrQuarterTonesPitchesLanguageKind () const
                              { return fLpsrQuarterTonesPitchesLanguageKind; }

    bool                  setLpsrChordsLanguageKind (string language);

    lpsrChordsLanguageKind
                          getLpsrChordsLanguageKind () const
                              { return fLpsrChordsLanguageKind; }

    // transpose
    // --------------------------------------

    S_msrSemiTonesPitchAndOctave
                          getTransposeSemiTonesPitchAndOctave () const
                              { return fTransposeSemiTonesPitchAndOctave; }

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceQuietness ();

  public:

    // public services
    // ------------------------------------------------------

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    void                  checkGroupOptionsConsistency () override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printLpsrOahHelp ();

    void                  printLpsrOahValues (unsigned int fieldWidth);

  private:

    // private methods
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  initializeLpsrTraceOah ();
#endif

    void                  initializeLpsrDisplayOptions ();

    void                  initializeLpsrPaperOptions ();

    void                  initializeLpsrMeasuresOptions ();

    void                  initializeLpsrTemposOptions ();

    void                  initializeLpsrWordsOptions ();

    void                  initializeLpsrLanguagesOptions ();

    void                  initializeLpsrTransposeOptions ();

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
    bool                  fDisplayLpsrFull;
    bool                  fDisplayLpsrShort;

    // paper
    // --------------------------------------

    bool                  fAllPaperVariables;

    msrLengthUnitKind     fLengthUnitKind;
    msrLengthUnitKind     fLengthUnitKindDefaultValue;

    // size
    msrLength             fPaperHeight;
    S_oahLengthAtom       fPaperHeightAtom;

    msrLength             fPaperWidth;
    S_oahLengthAtom       fPaperWidthAtom;

    // margins
    msrLength             fPaperLeftMargin;
    S_oahLengthAtom       fPaperLeftMarginAtom;

    msrLength             fPaperRightMargin;
    S_oahLengthAtom       fPaperRightMarginAtom;

    msrLength             fPaperTopMargin;
    S_oahLengthAtom       fPaperTopMarginAtom;

    msrLength             fPaperBottomMargin;
    S_oahLengthAtom       fPaperBottomMarginAtom;

    // indents
    msrLength             fPaperHorizontalShift;

    msrLength             fPaperIndent;
    msrLength             fPaperShortIndent;

    // spaces
    msrLength             fMarkupSystemSpacingPadding;

    msrLength             fBetweenSystemSpace;
    msrLength             fPageTopSpace;

    // counts
    int                   fPageCount;   // 0 if not specified
    int                   fSystemCount; // 0 if not specified

    // ragging
    bool                  fRaggedBottom;
    bool                  fRaggedLast;
    bool                  fRaggedLastBottom;
    bool                  fRaggedRight;

    // tag line
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
class EXP lpsrDalSegnoAtom : public oahAtomStoringAValueInAVariable
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

    virtual               ~lpsrDalSegnoAtom ();

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
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    map<string, msrDalSegno::msrDalSegnoKind>&
                          fStringToDalSegnoKindMapVariable;
};
typedef SMARTP<lpsrDalSegnoAtom> S_lpsrDalSegnoAtom;
EXP ostream& operator<< (ostream& os, const S_lpsrDalSegnoAtom& elt);

//______________________________________________________________________________
class EXP lpsrDalSegnoAlFineAtom : public oahAtomStoringAValueInAVariable
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

    virtual               ~lpsrDalSegnoAlFineAtom ();

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
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    map<string, msrDalSegno::msrDalSegnoKind>&
                          fStringToDalSegnoKindMapVariable;
};
typedef SMARTP<lpsrDalSegnoAlFineAtom> S_lpsrDalSegnoAlFineAtom;
EXP ostream& operator<< (ostream& os, const S_lpsrDalSegnoAlFineAtom& elt);

//______________________________________________________________________________
class EXP lpsrDalSegnoAlCodaAtom : public oahAtomStoringAValueInAVariable
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

    virtual               ~lpsrDalSegnoAlCodaAtom ();

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
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    map<string, msrDalSegno::msrDalSegnoKind>&
                          fStringToDalSegnoKindMapVariable;
};
typedef SMARTP<lpsrDalSegnoAlCodaAtom> S_lpsrDalSegnoAlCodaAtom;
EXP ostream& operator<< (ostream& os, const S_lpsrDalSegnoAlCodaAtom& elt);
*/

