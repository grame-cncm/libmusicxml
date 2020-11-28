/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsr2lilypondOah___
#define ___lpsr2lilypondOah___

#include "lpsrBasicTypes.h"

#include "oahAtomsCollection.h"


namespace MusicXML2
{

//______________________________________________________________________________
class lilypondScoreOutputKindAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondScoreOutputKindAtom> create (
      string               shortName,
      string               longName,
      string               description,
      string               valueSpecification,
      string               variableName,
      lpsrScoreOutputKind& lilypondScoreOutputKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lilypondScoreOutputKindAtom (
      string               shortName,
      string               longName,
      string               description,
      string               valueSpecification,
      string               variableName,
      lpsrScoreOutputKind& lilypondScoreOutputKindVariable);

    virtual ~lilypondScoreOutputKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setScoreOutputKindVariableValue (
                            lpsrScoreOutputKind value)
                              {  fLpsrScoreOutputKindVariable = value; }

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

    lpsrScoreOutputKind&  fLpsrScoreOutputKindVariable;
};
typedef SMARTP<lilypondScoreOutputKindAtom> S_lilypondScoreOutputKindAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondScoreOutputKindAtom& elt);

//______________________________________________________________________________
class lilypondTransposePartNameAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondTransposePartNameAtom> create (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      map<string, S_msrSemiTonesPitchAndOctave>&
                         stringMsrSemiTonesPitchAndOctaveVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lilypondTransposePartNameAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      map<string, S_msrSemiTonesPitchAndOctave>&
                         stringMsrSemiTonesPitchAndOctaveVariable);

    virtual ~lilypondTransposePartNameAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringMsrSemiTonesPitchAndOctaveVariable (
                            string  partName,
                            S_msrSemiTonesPitchAndOctave
                                    semiTonesPitchAndOctave)
                              {
                                fStringMsrSemiTonesPitchAndOctaveVariable [
                                  partName
                                ] =
                                  semiTonesPitchAndOctave;
                              }

    const map<string, S_msrSemiTonesPitchAndOctave>&
                          getStringMsrSemiTonesPitchAndOctaveVariable ()
                              {
                                return
                                  fStringMsrSemiTonesPitchAndOctaveVariable;
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

    map<string, S_msrSemiTonesPitchAndOctave>&
                          fStringMsrSemiTonesPitchAndOctaveVariable;
};
typedef SMARTP<lilypondTransposePartNameAtom> S_lilypondTransposePartNameAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondTransposePartNameAtom& elt);

//______________________________________________________________________________
class lilypondTransposePartIDAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondTransposePartIDAtom> create (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      map<string, S_msrSemiTonesPitchAndOctave>&
                         stringMsrSemiTonesPitchAndOctaveVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lilypondTransposePartIDAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      map<string, S_msrSemiTonesPitchAndOctave>&
                         stringMsrSemiTonesPitchAndOctaveVariable);

    virtual ~lilypondTransposePartIDAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringMsrSemiTonesPitchAndOctaveVariable (
                            string  partName,
                            S_msrSemiTonesPitchAndOctave
                                    semiTonesPitchAndOctave)
                              {
                                fStringMsrSemiTonesPitchAndOctaveVariable [
                                  partName
                                ] =
                                  semiTonesPitchAndOctave;
                              }

    const map<string, S_msrSemiTonesPitchAndOctave>&
                          getStringMsrSemiTonesPitchAndOctaveVariable ()
                              {
                                return
                                  fStringMsrSemiTonesPitchAndOctaveVariable;
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

    map<string, S_msrSemiTonesPitchAndOctave>&
                          fStringMsrSemiTonesPitchAndOctaveVariable;
};
typedef SMARTP<lilypondTransposePartIDAtom> S_lilypondTransposePartIDAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondTransposePartIDAtom& elt);

//______________________________________________________________________________
class lilypondAbsoluteOctaveEntryAtom : public oahAtomWithVariableName
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondAbsoluteOctaveEntryAtom> create (
      string               shortName,
      string               longName,
      string               description,
      string               variableName,
      lpsrOctaveEntryKind& lpsrOctaveEntryKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lilypondAbsoluteOctaveEntryAtom (
      string               shortName,
      string               longName,
      string               description,
      string               variableName,
      lpsrOctaveEntryKind& lpsrOctaveEntryKindVariable);

    virtual ~lilypondAbsoluteOctaveEntryAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyElement (ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    lpsrOctaveEntryKind&  fLpsrOctaveEntryKindVariable;
};
typedef SMARTP<lilypondAbsoluteOctaveEntryAtom> S_lilypondAbsoluteOctaveEntryAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondAbsoluteOctaveEntryAtom& elt);

//______________________________________________________________________________
class lilypondRelativeOctaveEntryAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondRelativeOctaveEntryAtom> create (
      string                        shortName,
      string                        longName,
      string                        description,
      string                        valueSpecification,
      string                        variableName,
      lpsrOctaveEntryKind&          lpsrOctaveEntryKindVariable,
      S_msrSemiTonesPitchAndOctave& lilypondRelativeOctaveEntryVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lilypondRelativeOctaveEntryAtom (
      string                        shortName,
      string                        longName,
      string                        description,
      string                        valueSpecification,
      string                        variableName,
      lpsrOctaveEntryKind&          lpsrOctaveEntryKindVariable,
      S_msrSemiTonesPitchAndOctave& lilypondRelativeOctaveEntryVariable);

    virtual ~lilypondRelativeOctaveEntryAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setRelativeOctaveEntryVariableValue (
                            S_msrSemiTonesPitchAndOctave value)
                              {
                                fMsrSemiTonesPitchAndOctaveVariable = value;
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

    lpsrOctaveEntryKind&  fLpsrOctaveEntryKindVariable;

    S_msrSemiTonesPitchAndOctave&
                          fMsrSemiTonesPitchAndOctaveVariable;
};
typedef SMARTP<lilypondRelativeOctaveEntryAtom> S_lilypondRelativeOctaveEntryAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondRelativeOctaveEntryAtom& elt);

//______________________________________________________________________________
class lilypondFixedOctaveEntryAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondFixedOctaveEntryAtom> create (
      string                        shortName,
      string                        longName,
      string                        description,
      string                        valueSpecification,
      string                        variableName,
      lpsrOctaveEntryKind&          lpsrOctaveEntryKindVariable,
      S_msrSemiTonesPitchAndOctave& lilypondFixedOctaveEntryVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lilypondFixedOctaveEntryAtom (
      string                        shortName,
      string                        longName,
      string                        description,
      string                        valueSpecification,
      string                        variableName,
      lpsrOctaveEntryKind&          lpsrOctaveEntryKindVariable,
      S_msrSemiTonesPitchAndOctave& lilypondFixedOctaveEntryVariable);

    virtual ~lilypondFixedOctaveEntryAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setFixedOctaveEntryVariableValue (
                            S_msrSemiTonesPitchAndOctave value)
                              {
                                fMsrSemiTonesPitchAndOctaveVariable = value;
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

    lpsrOctaveEntryKind&  fLpsrOctaveEntryKindVariable;

    S_msrSemiTonesPitchAndOctave&
                          fMsrSemiTonesPitchAndOctaveVariable;
};
typedef SMARTP<lilypondFixedOctaveEntryAtom> S_lilypondFixedOctaveEntryAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondFixedOctaveEntryAtom& elt);

//______________________________________________________________________________
class lilypondAccidentalStyleKindAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondAccidentalStyleKindAtom> create (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      lpsrAccidentalStyleKind&
                         lilypondAccidentalStyleKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lilypondAccidentalStyleKindAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      lpsrAccidentalStyleKind&
                         lilypondAccidentalStyleKindVariable);

    virtual ~lilypondAccidentalStyleKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setLpsrAccidentalStyleKindVariable (
                            lpsrAccidentalStyleKind value)
                              {
                                fLpsrAccidentalStyleKindVariable = value;
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

    lpsrAccidentalStyleKind&
                          fLpsrAccidentalStyleKindVariable;
};
typedef SMARTP<lilypondAccidentalStyleKindAtom> S_lilypondAccidentalStyleKindAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondAccidentalStyleKindAtom& elt);

//______________________________________________________________________________
class lilypondChordsDisplayAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondChordsDisplayAtom> create (
      string                shortName,
      string                longName,
      string                description,
      string                valueSpecification,
      string                variableName,
      list<pair<string, string> >&
                            lilypondChordsDisplayVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lilypondChordsDisplayAtom (
      string                shortName,
      string                longName,
      string                description,
      string                valueSpecification,
      string                variableName,
      list<pair<string, string> >&
                            lilypondChordsDisplayVariable);

    virtual ~lilypondChordsDisplayAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringsPairListVariable (
                            pair<string, string> value)
                              {
                                fStringsPairListVariable.push_back (value);
                              }

    const list<pair<string, string> >&
                          getStringsPairListVariable ()
                              { return fStringsPairListVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string  theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    list<pair<string, string> >&
                          fStringsPairListVariable;
};
typedef SMARTP<lilypondChordsDisplayAtom> S_lilypondChordsDisplayAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondChordsDisplayAtom& elt);

//______________________________________________________________________________
class lilypondLyricsDurationsKindAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondLyricsDurationsKindAtom> create (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      lpsrLyricsDurationsKind&
                         lilypondLyricsDurationsKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lilypondLyricsDurationsKindAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      lpsrLyricsDurationsKind&
                         lilypondLyricsDurationsKindVariable);

    virtual ~lilypondLyricsDurationsKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setLpsrLyricsDurationsKindVariable (
                            lpsrLyricsDurationsKind value)
                              {
                                fLpsrLyricsDurationsKindVariable = value;
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

    lpsrLyricsDurationsKind&
                          fLpsrLyricsDurationsKindVariable;
};
typedef SMARTP<lilypondLyricsDurationsKindAtom> S_lilypondLyricsDurationsKindAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondLyricsDurationsKindAtom& elt);

//______________________________________________________________________________
class lpsr2lilypondOahGroup : public oahGroup
{
  public:

    // data types
    // ------------------------------------------------------

    enum scoreNotationKind {
      kWesternNotation, kJianpuNotation, kABCNotation };

    static string scoreNotationKindAsString (
      scoreNotationKind notationKind);

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lpsr2lilypondOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeLpsr2lilypondOahGroup ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

    lpsr2lilypondOahGroup ();

    virtual ~lpsr2lilypondOahGroup ();

    // set and get
    // ------------------------------------------------------

 // JMI   bool                  setAccidentalStyleKind (
  //                          lpsrAccidentalStyleKind accidentalStyleKind);

    bool                  setScoreOutputKind (
                            string outputKind);

    // identification
    // --------------------------------------

    // MusicXML informations
    string                getRights () const
                              { return fRights; }
    string                getComposer () const
                              { return fComposer; }
    string                getArranger () const
                              { return fArranger; }

    S_oahStringAtom       getPoetAtom () const
                              { return fPoetAtom; }
    string                getPoet () const
                              { return fPoet; }
    string                getLyricist () const
                              { return fLyricist; }
    string                getSoftware () const
                              { return fSoftware; }

    // LilyPond informations
    string                getDedication () const
                              { return fDedication; }
    string                getPiece () const
                              { return fPiece; }
    string                getOpus () const
                              { return fOpus; }
    string                getTitle () const
                              { return fTitle; }
    string                getSubTitle () const
                              { return fSubTitle; }
    string                getSubSubTitle () const
                              { return fSubSubTitle; }
    string                getInstrument () const
                              { return fInstrument; }
    string                getMeter () const
                              { return fMeter; }
    string                getTagline () const
                              { return fTagline; }
    string                getCopyright () const
                              { return fCopyright; }


    // paper block
    // --------------------------------------

    bool                  getNoPaperBlock () const
                              { return fNoPaperBlock; }

    // parts
    // --------------------------------------

    const map<string, S_msrSemiTonesPitchAndOctave>&
                          getPartNamesTranspositionMap () const
                              { return fPartNamesTranspositionMap; }
    const map<string, S_msrSemiTonesPitchAndOctave>&
                          getPartIDsTranspositionMap () const
                              { return fPartIDsTranspositionMap; }


    // names
    // --------------------------------------


    // engravers
    // --------------------------------------

    bool                  getAmbitusEngraver () const
                              { return fAmbitusEngraver; }
    bool                  getCustosEngraver () const
                              { return fCustosEngraver; }


    // clefs, keys, times
    // --------------------------------------

    bool                  getCommentClefChanges () const
                              { return fCommentClefChanges; }

    bool                  getNumericalTime () const
                              { return fNumericalTime; }


    // notes
    // --------------------------------------

    lpsrOctaveEntryKind   getOctaveEntryKind () const
                              { return fOctaveEntryKind; }

    // this is relevant only for relative octave entry kinds
    S_msrSemiTonesPitchAndOctave
                          getRelativeOctaveEntrySemiTonesPitchAndOctave () const
                              { return fRelativeOctaveEntrySemiTonesPitchAndOctave; }
    // this is relevant only for fixed octave entry kinds
    S_msrSemiTonesPitchAndOctave
                          getFixedOctaveEntrySemiTonesPitchAndOctave () const
                              { return fFixedOctaveEntrySemiTonesPitchAndOctave; }

    S_msrSemiTonesPitchAndOctave
                          getSemiTonesPitchAndOctaveDefaultValue () const
                              { return fSemiTonesPitchAndOctaveDefaultValue; }


    bool                  getAllDurations () const
                              { return fAllDurations; }

    bool                  getNoAutoBeaming () const
                              { return fNoAutoBeaming; }
    bool                  getNoBeams () const
                              { return fNoBeams; }

    bool                  getRomanStringNumbers () const
                              { return fRomanStringNumbers; }
    bool                  getAvoidOpenStrings () const
                              { return fAvoidOpenStrings; }

    lpsrAccidentalStyleKind
                          getAccidentalStyleKind () const
                              { return fAccidentalStyleKind; }

    bool                  getCompressFullMeasureRests () const
                              { return fCompressFullMeasureRests; }
    bool                  getMergeRests () const
                              { return fMergeRests; }

    bool                  getInputLineNumbers () const
                              { return fInputLineNumbers; }
    bool                  getOriginalMeasureNumbers () const
                              { return fOriginalMeasureNumbers; }
    bool                  getPositionsInMeasures () const
                              { return fPositionsInMeasures; }


    // measure numbers
    // --------------------------------------

    const map<string, int>&
                          getResetMeasureElementMeasureNumberMap () const
                              { return fResetMeasureElementMeasureNumberMap; }
    const map<string, int>&
                          getAddEmptyMeasuresAfterMeasureNumberMap () const
                              { return fAddEmptyMeasuresAfterMeasureNumberMap; }

    // bar numbers
    // --------------------------------------

    bool                  getShowAllBarNumbers () const
                              { return fShowAllBarNumbers; }

    const set<int>&       getBoxAroundBarNumberSet () const
                              { return fBoxAroundBarNumberSet; }

    // bar number checks
    // --------------------------------------

    bool                  getNoBarNumberChecks () const
                              { return fNoBarNumberChecks; }

    // line breaks
    // --------------------------------------

    bool                  getIgnoreMusicXMLLineBreaks () const
                              { return fIgnoreMusicXMLLineBreaks; }
    bool                  getSuppressMusicXMLLineBreaks () const
                              { return fSuppressMusicXMLLineBreaks; }

    bool                  getBreakLinesAtIncompleteRightMeasures () const
                              { return fBreakLinesAtIncompleteRightMeasures; }

    int                   getSeparatorLineEveryNMeasures () const
                              { return fSeparatorLineEveryNMeasures; }

    const set<string>&    getBreakLineAfterMeasureNumberSet () const
                              { return fBreakLineAfterMeasureNumberSet; }

    // page breaks
    // --------------------------------------

    bool                  getIgnoreMusicXMLPageBreaks () const
                              { return fIgnoreMusicXMLPageBreaks; }
    bool                  getSuppressMusicXMLPageBreaks () const
                              { return fSuppressMusicXMLPageBreaks; }

    const set<string>&    getBreakPageAfterMeasureNumberSet () const
                              { return fBreakPageAfterMeasureNumberSet; }


    // staves
    // --------------------------------------

    bool                  getModernTab () const
                              { return fModernTab; }
    bool                  getTabFullNotation () const
                              { return fTabFullNotation; }

    bool                  getKeepStaffSize () const
                              { return fKeepStaffSize; }

    msrRGBColor           getLedgerLinesRGBColor () const
                              { return fLedgerLinesRGBColor; }
    bool                  getLedgerLinesRGBColorHasBeenSet () const // JMI ???
                              { return fLedgerLinesRGBColorHasBeenSet; }

    // chords
    // --------------------------------------

    bool                  getConnectArpeggios () const
                              { return fConnectArpeggios; }


    // tuplets
    // --------------------------------------

    bool                  getIndentTuplets () const
                              { return fIndentTuplets; }


    // repeats
    // --------------------------------------

    bool                  getKeepRepeatBarlines () const
                              { return fKeepRepeatBarlines; }
    bool                  getRepeatBrackets () const
                              { return fRepeatBrackets; }
    bool                  getIgnoreRepeatNumbers () const
                              { return fIgnoreRepeatNumbers; }


    // ornaments
    // --------------------------------------

    rational              getDelayedOrnamentsFraction () const
                              { return fDelayedOrnamentsFraction; }


    // chords display
    // --------------------------------------

    const list<pair<string, string> >&
                          getChordsDisplayList () const
                              { return fChordsDisplayList; }
    bool                  getJazzChordsDisplay () const
                              { return fJazzChordsDisplay; }
    string                getJazzChordsDisplayLilypondcode () const
                              { return fJazzChordsDisplayLilypondcode; }


    // fonts
    // --------------------------------------

    bool                  getJazzFonts () const
                              { return fJazzFonts; }


    // code generation
    // --------------------------------------

    bool                  getXml2lyInfos () const
                              { return fXml2lyInfos; }

    bool                  getLilyPondComments () const
                              { return fLilyPondComments; }

    bool                  getGlobal () const
                              { return fGlobal; }

    bool                  getDisplayMusic () const
                              { return fDisplayMusic; }

    bool                  getNoLilypondCode () const
                              { return fNoLilypondCode; }

    bool                  getNoLilypondLyrics () const
                              { return fNoLilypondLyrics; }

    bool                  getLilypondCompileDate () const
                              { return fLilypondCompileDate; }

    bool                  getPointAndClickOff () const
                              { return fPointAndClickOff; }

    bool                  getDraftMode () const
                              { return fDraftMode; }

    bool                  getWhiteNoteHeads () const
                              { return fWhiteNoteHeads; }


    // score notation
    // --------------------------------------

    // scoreNotationKind     getScoreNotationKind; JMI ???
    bool                  getJianpu () const
                              { return fJianpu; }



    // lyrics
    // --------------------------------------

    bool                  getAddStanzasNumbers () const
                              { return fAddStanzasNumbers; }

    lpsrLyricsDurationsKind
                          getLyricsDurationsKind () const
                              { return fLyricsDurationsKind; }

    // midi
    // --------------------------------------

    msrMidiTempo          getMidiTempo () const
                              { return fMidiTempo; }

    bool                  getNoMidi () const
                              { return fNoMidi; }

    // combined options, minimal
    // --------------------------------------

    bool                  getMinimal () const
                              { return fMinimal; }

  public:

    // public services
    // ------------------------------------------------------

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    virtual void          checkGroupOptionsConsistency () override;

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeIdentificationOptions ();

    void                  initializePartsOptions ();

    void                  initializeEngraversOptions ();

    void                  initializeClefsKeysTimesOptions ();

    void                  initializeNotesOptions ();

    void                  initializeBarsOptions ();

    void                  initializeLineBreaksOptions ();

    void                  initializePageBreaksOptions ();

    void                  initializeStavesOptions ();

    void                  initializeChordsOptions ();

    void                  initializeTupletsOptions ();

    void                  initializeRepeatOptions ();

    void                  initializeOrnamentsOptions ();

    void                  initializeChordsDisplayOptions ();

    void                  initializeLyricsOptions ();

    void                  initializeFontsOptions ();

    void                  initializeCodeGenerationOptions ();

    void                  initializeScoreNotationOptions ();

    void                  initializeMidiOptions ();

    void                  initializeCombinedOptionsOptions ();

    void                  setOctaveEntryKindToAbsolute () // JMI ???
                              {
                                fOctaveEntryKind =
                                  kOctaveEntryAbsolute;
                              }

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printLpsr2lilypondOahHelp ();

    void                  printLpsr2lilypondOahValues (int fieldWidth);

    virtual void          printAtomWithValueOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // private fields
    // --------------------------------------

    // identification
    // --------------------------------------

    // MusicXML informations
    string                fRights;
    string                fComposer;
    string                fArranger;

    S_oahStringAtom       fPoetAtom;
    string                fPoet;
    string                fLyricist;
    string                fSoftware;

    // LilyPond informations
    string                fDedication;
    string                fPiece;
    string                fOpus;
    string                fTitle;
    string                fSubTitle;
    string                fSubSubTitle;
    string                fInstrument;
    string                fMeter;
    string                fTagline;
    string                fCopyright;


    // paper block
    // --------------------------------------

    bool                  fNoPaperBlock;
    // this atom is used by the '-minimal' combined option
    S_oahBooleanAtom      fNoPaperBlockAtom;

    // parts
    // --------------------------------------

    map<string, S_msrSemiTonesPitchAndOctave>
                          fPartNamesTranspositionMap;
    map<string, S_msrSemiTonesPitchAndOctave>
                          fPartIDsTranspositionMap;


    // names
    // --------------------------------------


    // engravers
    // --------------------------------------

    bool                  fAmbitusEngraver;
    bool                  fCustosEngraver;


    // clefs, keys, times
    // --------------------------------------

    bool                  fCommentClefChanges;

    bool                  fNumericalTime;


    // notes
    // --------------------------------------

    lpsrOctaveEntryKind   fOctaveEntryKind;

    // this is relevant only for relative octave entry kinds
    S_msrSemiTonesPitchAndOctave
                          fRelativeOctaveEntrySemiTonesPitchAndOctave;
    // this is relevant only for fixed octave entry kinds
    S_msrSemiTonesPitchAndOctave
                          fFixedOctaveEntrySemiTonesPitchAndOctave;

    S_msrSemiTonesPitchAndOctave
                          fSemiTonesPitchAndOctaveDefaultValue;


    bool                  fAllDurations;

    bool                  fNoAutoBeaming;
    bool                  fNoBeams;

    bool                  fRomanStringNumbers;
    bool                  fAvoidOpenStrings;

    lpsrAccidentalStyleKind
                          fAccidentalStyleKind;

    bool                  fCompressFullMeasureRests;
    bool                  fMergeRests;

    bool                  fInputLineNumbers;
    bool                  fOriginalMeasureNumbers;
    bool                  fPositionsInMeasures;


    // measure numbers
    // --------------------------------------

    map<string, int>      fResetMeasureElementMeasureNumberMap;
    map<string, int>      fAddEmptyMeasuresAfterMeasureNumberMap;

    // bar numbers
    // --------------------------------------

    bool                  fShowAllBarNumbers;

    set<int>              fBoxAroundBarNumberSet;

    // bar number checks
    // --------------------------------------

    bool                  fNoBarNumberChecks;
    // this atom is used by the '-minimal' combined option
    S_oahBooleanAtom      fNoBarNumberChecksAtom;

    // line breaks
    // --------------------------------------

    bool                  fIgnoreMusicXMLLineBreaks;

    bool                  fSuppressMusicXMLLineBreaks;
    // this atom is used by the '-minimal' combined option
    S_oahBooleanAtom      fSuppressMusicXMLLineBreaksAtom;

    bool                  fBreakLinesAtIncompleteRightMeasures;

    int                   fSeparatorLineEveryNMeasures;

    set<string>           fBreakLineAfterMeasureNumberSet;

    // page breaks
    // --------------------------------------

    bool                  fIgnoreMusicXMLPageBreaks;

    bool                  fSuppressMusicXMLPageBreaks;
    // this atom is used by the '-minimal' combined option
    S_oahBooleanAtom      fSuppressMusicXMLPageBreaksAtom;

    set<string>           fBreakPageAfterMeasureNumberSet;


    // staves
    // --------------------------------------

    bool                  fModernTab;
    bool                  fTabFullNotation;

    bool                  fKeepStaffSize;

    msrRGBColor           fLedgerLinesRGBColor; // #(rgb-color 1.0 0.9 0.5)
    bool                  fLedgerLinesRGBColorHasBeenSet;

    // chords
    // --------------------------------------

    bool                  fConnectArpeggios;


    // tuplets
    // --------------------------------------

    bool                  fIndentTuplets;


    // repeats
    // --------------------------------------

    bool                  fKeepRepeatBarlines;  // otherwise let LilyPond do the job
    bool                  fRepeatBrackets;
    bool                  fIgnoreRepeatNumbers; // and let LilyPond do the job


    // ornaments
    // --------------------------------------

    rational              fDelayedOrnamentsFraction;


    // chords display
    // --------------------------------------

    list<pair<string, string> >
                          fChordsDisplayList;
    bool                  fJazzChordsDisplay;
    string                fJazzChordsDisplayLilypondcode;


    // fonts
    // --------------------------------------

    bool                  fJazzFonts;


    // code generation
    // --------------------------------------

    bool                  fXml2lyInfos;

    bool                  fLilyPondComments;

    bool                  fGlobal;

    bool                  fDisplayMusic;

    bool                  fNoLilypondCode;

    bool                  fNoLilypondLyrics;

    bool                  fLilypondCompileDate;

    bool                  fPointAndClickOff;

    bool                  fDraftMode;

    bool                  fWhiteNoteHeads;


    // score notation
    // --------------------------------------

    // scoreNotationKind     fScoreNotationKind; JMI ???
    bool                  fJianpu;



    // lyrics
    // --------------------------------------

    bool                  fAddStanzasNumbers;

    lpsrLyricsDurationsKind
                          fLyricsDurationsKind;

    // midi
    // --------------------------------------

    msrMidiTempo          fMidiTempo;

    bool                  fNoMidi;

    // combined options, minimal
    // --------------------------------------

    bool                  fMinimal;
    S_oahCombinedBooleansAtom
                          fMinimalCombinedBooleansAtom;
};
typedef SMARTP<lpsr2lilypondOahGroup> S_lpsr2lilypondOahGroup;
EXP ostream& operator<< (ostream& os, const S_lpsr2lilypondOahGroup& elt);

EXP extern S_lpsr2lilypondOahGroup gGlobalLpsr2lilypondOahGroup;

// JMI typedef void (lpsr2lilypondOahGroup::*lpsr2lilypondOahMethodPtr)(); //MyTypedef is a type!

/*
  It's worth noting that, as of C++11, you could write this expression
  as a more legible using statement:
    using lpsr2lilypondOahMethodPtr = void (lpsr2lilypondOahGroup::*)();
*/

//______________________________________________________________________________
S_lpsr2lilypondOahGroup createGlobalLpsr2lilypondOahGroup ();
  // JMI ???
}


#endif

    /* JMI ???
      workNumber
      workTitle
      movementNumber,
      movementTitle,
      scoreInstrument
      miscellaneousField

      partGroupName
      partGroupNameDisplayText
      partGroupAbbrevation
      partGroupInstrumentName

      partID
      partMsrName
      partName
      partAbbrevation
      partInstrumentName
      partInstrumentAbbreviation

      staffInstrumentName
      staffInstrumentAbbreviation

      pickDedication
      pickTitle
      pickSubTitle
      pickSubSubTitle

      pickInstrument

      pickTagline
      pickCopyright
    */


/* JMI
//______________________________________________________________________________
class lilypondBreakPageAfterMeasureNumberAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondBreakPageAfterMeasureNumberAtom> create (
      string       shortName,
      string       longName,
      string       description,
      string       valueSpecification,
      string       variableName,
      set<string>& S_lilypondAbsoluteOctaveEntryAtomtringSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lilypondBreakPageAfterMeasureNumberAtom (
      string       shortName,
      string       longName,
      string       description,
      string       valueSpecification,
      string       variableName,
      set<string>& fStringSetVariable);

    virtual ~lilypondBreakPageAfterMeasureNumberAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const set<string>&    getStringSetVariable ()
                              { return fStringSetVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    set<string>&          fStringSetVariable;
};
typedef SMARTP<lilypondBreakPageAfterMeasureNumberAtom> S_lilypondBreakPageAfterMeasureNumberAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondBreakPageAfterMeasureNumberAtom& elt);
*/

/*
    enum {
      workNumber
      workTitle
      movementNumber,
      movementTitle,
      scoreInstrument
      miscellaneousField

      partGroupName
      partGroupNameDisplayText
      partGroupAbbrevation
      partGroupInstrumentName

      partID
      partMsrName
      partName
      partAbbrevation
      partInstrumentName
      partInstrumentAbbreviation

      staffInstrumentName
      staffInstrumentAbbreviation
      };
*/

