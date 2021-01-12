/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msprOah___
#define ___msprOah___

#include "msprBasicTypes.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahAtomsCollection.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msprPitchesLanguageAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msprPitchesLanguageAtom> create (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      msrQuarterTonesPitchesLanguageKind&
                         msprPitchesLanguageKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msprPitchesLanguageAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      msrQuarterTonesPitchesLanguageKind&
                         msprPitchesLanguageKindVariable);

    virtual ~msprPitchesLanguageAtom ();

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

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrQuarterTonesPitchesLanguageKind&
                          fMsrQuarterTonesPitchesLanguageKindVariable;
};
typedef SMARTP<msprPitchesLanguageAtom> S_msprPitchesLanguageAtom;
EXP ostream& operator<< (ostream& os, const S_msprPitchesLanguageAtom& elt);

//______________________________________________________________________________
class EXP msprChordsLanguageAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msprChordsLanguageAtom> create (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      msprChordsLanguageKind&
                         msprChordsLanguageKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msprChordsLanguageAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      msprChordsLanguageKind&
                         msprChordsLanguageKindVariable);

    virtual ~msprChordsLanguageAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setmsprChordsLanguageKindVariable (
                            msprChordsLanguageKind value)
                              {
                                fmsprChordsLanguageKindVariable = value;
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

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msprChordsLanguageKind&
                          fmsprChordsLanguageKindVariable;
};
typedef SMARTP<msprChordsLanguageAtom> S_msprChordsLanguageAtom;
EXP ostream& operator<< (ostream& os, const S_msprChordsLanguageAtom& elt);

//______________________________________________________________________________
class EXP msprTransposeAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msprTransposeAtom> create (
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

    msprTransposeAtom (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      S_msrSemiTonesPitchAndOctave&
              semiTonesPitchAndOctaveVariable);

    virtual ~msprTransposeAtom ();

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

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrSemiTonesPitchAndOctave&
                          fSemiTonesPitchAndOctaveVariable;
};
typedef SMARTP<msprTransposeAtom> S_msprTransposeAtom;
EXP ostream& operator<< (ostream& os, const S_msprTransposeAtom& elt);

//______________________________________________________________________________
class EXP msprOahGroup : public oahGroup
{
  public:

    static SMARTP<msprOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializemsprOahGroup ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

    msprOahGroup ();

    virtual ~msprOahGroup ();

    // set and get
    // ------------------------------------------------------

    // trace
    // --------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  setTracemspr ()
                              { fTracemspr = true; }
    bool                  getTracemspr () const
                              { return fTracemspr; }

    void                  setTraceLilypondVersion ()
                              { fTraceLilypondVersion = true; }
    bool                  getTraceLilypondVersion () const
                              { return fTraceLilypondVersion; }

    void                  setTracemsprVisitors ()
                              { fTracemsprVisitors = true; }
    bool                  getTracemsprVisitors () const
                              { return fTracemsprVisitors; }

    void                  setTracemsprBlocks ()
                              { fTracemsprBlocks = true; }
    bool                  getTracemsprBlocks () const
                              { return fTracemsprBlocks; }

    void                  setTraceSchemeFunctions ()
                              { fTraceSchemeFunctions = true; }
    bool                  getTraceSchemeFunctions () const
                              { return fTraceSchemeFunctions; }
#endif

    // display
    // --------------------------------------

    void                  setDisplaymspr ()
                              { fDisplaymspr = true; }
    bool                  getDisplaymspr () const
                              { return fDisplaymspr; }

    void                  setDisplaymsprShort ()
                              { fDisplaymsprShort = true; }
    bool                  getDisplaymsprShort () const
                              { return fDisplaymsprShort; }

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

    msrQuarterTonesPitchesLanguageKind
                          getmsprQuarterTonesPitchesLanguageKind () const
                              { return fmsprQuarterTonesPitchesLanguageKind; }

    msprChordsLanguageKind
                          getmsprChordsLanguageKind () const
                              { return fmsprChordsLanguageKind; }

    // transpose
    // --------------------------------------

    S_msrSemiTonesPitchAndOctave
                          getTransposeSemiTonesPitchAndOctave () const
                              { return fTransposeSemiTonesPitchAndOctave; }

    // quit after some passes
    // --------------------------------------

    void                  setQuitAfterPass3 ()
                              { fQuitAfterPass3 = true; }
    bool                  getQuitAfterPass3 () const
                              { return fQuitAfterPass3; }

    bool                  setmsprQuarterTonesPitchesLanguage (
                            string language);

    bool                  setmsprChordsLanguage (
                            string language);

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

    void                  printmsprOahHelp ();

    void                  printmsprOahValues (unsigned int fieldWidth);

  private:

    // private methods
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  initializemsprTraceOah ();
#endif

    void                  initializemsprDisplayOptions ();

    void                  initializemsprPaperOptions ();

    void                  initializemsprMeasuresOptions ();

    void                  initializemsprTemposOptions ();

    void                  initializemsprWordsOptions ();

    void                  initializemsprLanguagesOptions ();

    void                  initializemsprTransposeOptions ();

    void                  initializemsprQuitAfterSomePassesOptions ();

  private:

    // private fields
    // --------------------------------------

    // trace
    // --------------------------------------

#ifdef TRACING_IS_ENABLED
    bool                  fTracemspr;

    bool                  fTraceLilypondVersion;

    bool                  fTracemsprVisitors;

    bool                  fTracemsprBlocks;

    bool                  fTraceSchemeFunctions;
#endif

    // display
    // --------------------------------------

    bool                  fDisplaymspr;
    bool                  fDisplaymsprShort;

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
                          fmsprQuarterTonesPitchesLanguageKind;

    msprChordsLanguageKind
                          fmsprChordsLanguageKind;

    // transpose
    // --------------------------------------

    S_msrSemiTonesPitchAndOctave
                          fTransposeSemiTonesPitchAndOctave;

    // quit after some passes
    // --------------------------------------

    bool                  fQuitAfterPass3;
};
typedef SMARTP<msprOahGroup> S_msprOahGroup;
EXP ostream& operator<< (ostream& os, const S_msprOahGroup& elt);

EXP extern S_msprOahGroup gGlobalmsprOahGroup;

//______________________________________________________________________________
EXP S_msprOahGroup createGlobalmsprOahGroup ();


}


#endif

/* JMI
//______________________________________________________________________________
class EXP msprDalSegnoAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msprDalSegnoAtom> create (
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

    msprDalSegnoAtom (
      string              shortName,
      string              longName,
      string              description,
      string              valueSpecification,
      string              variableName,
      map<string, msrDalSegno::msrDalSegnoKind>&
                          stringToDalSegnoKindMapVariable);

    virtual ~msprDalSegnoAtom ();

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

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    map<string, msrDalSegno::msrDalSegnoKind>&
                          fStringToDalSegnoKindMapVariable;
};
typedef SMARTP<msprDalSegnoAtom> S_msprDalSegnoAtom;
EXP ostream& operator<< (ostream& os, const S_msprDalSegnoAtom& elt);

//______________________________________________________________________________
class EXP msprDalSegnoAlFineAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msprDalSegnoAlFineAtom> create (
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

    msprDalSegnoAlFineAtom (
      string              shortName,
      string              longName,
      string              description,
      string              valueSpecification,
      string              variableName,
      map<string, msrDalSegno::msrDalSegnoKind>&
                          stringToDalSegnoKindMapVariable);

    virtual ~msprDalSegnoAlFineAtom ();

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

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    map<string, msrDalSegno::msrDalSegnoKind>&
                          fStringToDalSegnoKindMapVariable;
};
typedef SMARTP<msprDalSegnoAlFineAtom> S_msprDalSegnoAlFineAtom;
EXP ostream& operator<< (ostream& os, const S_msprDalSegnoAlFineAtom& elt);

//______________________________________________________________________________
class EXP msprDalSegnoAlCodaAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msprDalSegnoAlCodaAtom> create (
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

    msprDalSegnoAlCodaAtom (
      string              shortName,
      string              longName,
      string              description,
      string              valueSpecification,
      string              variableName,
      map<string, msrDalSegno::msrDalSegnoKind>&
                          stringToDalSegnoKindMapVariable);

    virtual ~msprDalSegnoAlCodaAtom ();

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

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    map<string, msrDalSegno::msrDalSegnoKind>&
                          fStringToDalSegnoKindMapVariable;
};
typedef SMARTP<msprDalSegnoAlCodaAtom> S_msprDalSegnoAlCodaAtom;
EXP ostream& operator<< (ostream& os, const S_msprDalSegnoAlCodaAtom& elt);
*/

