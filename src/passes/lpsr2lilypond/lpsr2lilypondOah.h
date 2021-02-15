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
class EXP lilypondScoreOutputKindAtom : public oahAtomWithValue
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

    virtual               ~lilypondScoreOutputKindAtom ();

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
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

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

    lpsrScoreOutputKind&  fLpsrScoreOutputKindVariable;
};
typedef SMARTP<lilypondScoreOutputKindAtom> S_lilypondScoreOutputKindAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondScoreOutputKindAtom& elt);

//______________________________________________________________________________
class EXP lilypondTransposePartNameAtom : public oahAtomWithValue
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
                                               stringToMsrSemiTonesPitchAndOctaveMapVariable);

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
                                               stringToMsrSemiTonesPitchAndOctaveMapVariable);

    virtual               ~lilypondTransposePartNameAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringToMsrSemiTonesPitchAndOctaveMapVariable (
                            string  partName,
                            S_msrSemiTonesPitchAndOctave
                                    semiTonesPitchAndOctave)
                              {
                                fStringToMsrSemiTonesPitchAndOctaveMapVariable [
                                  partName
                                ] =
                                  semiTonesPitchAndOctave;
                              }

    const map<string, S_msrSemiTonesPitchAndOctave>&
                          getStringToMsrSemiTonesPitchAndOctaveMapVariable () const
                              {
                                return
                                  fStringToMsrSemiTonesPitchAndOctaveMapVariable;
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

    map<string, S_msrSemiTonesPitchAndOctave>&
                          fStringToMsrSemiTonesPitchAndOctaveMapVariable;
};
typedef SMARTP<lilypondTransposePartNameAtom> S_lilypondTransposePartNameAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondTransposePartNameAtom& elt);

//______________________________________________________________________________
class EXP lilypondTransposePartIDAtom : public oahAtomWithValue
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
                                               stringToMsrSemiTonesPitchAndOctaveMapVariable);

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
                                               stringToMsrSemiTonesPitchAndOctaveMapVariable);

    virtual               ~lilypondTransposePartIDAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringToMsrSemiTonesPitchAndOctaveMapVariable (
                            string  partName,
                            S_msrSemiTonesPitchAndOctave
                                    semiTonesPitchAndOctave)
                              {
                                fStringToMsrSemiTonesPitchAndOctaveMapVariable [
                                  partName
                                ] =
                                  semiTonesPitchAndOctave;
                              }

    const map<string, S_msrSemiTonesPitchAndOctave>&
                          getStringToMsrSemiTonesPitchAndOctaveMapVariable () const
                              {
                                return
                                  fStringToMsrSemiTonesPitchAndOctaveMapVariable;
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

    map<string, S_msrSemiTonesPitchAndOctave>&
                          fStringToMsrSemiTonesPitchAndOctaveMapVariable;
};
typedef SMARTP<lilypondTransposePartIDAtom> S_lilypondTransposePartIDAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondTransposePartIDAtom& elt);

//______________________________________________________________________________
class EXP lilypondAbsoluteOctaveEntryAtom : public oahAtomWithVariableName
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

    virtual               ~lilypondAbsoluteOctaveEntryAtom ();

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

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    lpsrOctaveEntryKind&  fLpsrOctaveEntryKindVariable;
};
typedef SMARTP<lilypondAbsoluteOctaveEntryAtom> S_lilypondAbsoluteOctaveEntryAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondAbsoluteOctaveEntryAtom& elt);

//______________________________________________________________________________
class EXP lilypondRelativeOctaveEntryAtom : public oahAtomWithValue
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

    virtual               ~lilypondRelativeOctaveEntryAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setRelativeOctaveEntryVariableValue (
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

    lpsrOctaveEntryKind&  fLpsrOctaveEntryKindVariable;

    S_msrSemiTonesPitchAndOctave&
                          fSemiTonesPitchAndOctaveVariable;
};
typedef SMARTP<lilypondRelativeOctaveEntryAtom> S_lilypondRelativeOctaveEntryAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondRelativeOctaveEntryAtom& elt);

//______________________________________________________________________________
class EXP lilypondFixedOctaveEntryAtom : public oahAtomWithValue
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

    virtual               ~lilypondFixedOctaveEntryAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setFixedOctaveEntryVariableValue (
                            S_msrSemiTonesPitchAndOctave value)
                              { fSemiTonesPitchAndOctaveVariable = value; }

    S_msrSemiTonesPitchAndOctave&
                          getFixedOctaveEntryVariableValue () const
                              { return fSemiTonesPitchAndOctaveVariable; }
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

    lpsrOctaveEntryKind&  fLpsrOctaveEntryKindVariable;

    S_msrSemiTonesPitchAndOctave&
                          fSemiTonesPitchAndOctaveVariable;
};
typedef SMARTP<lilypondFixedOctaveEntryAtom> S_lilypondFixedOctaveEntryAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondFixedOctaveEntryAtom& elt);

//______________________________________________________________________________
class EXP lilypondAccidentalStyleKindAtom : public oahAtomWithValue
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

    virtual               ~lilypondAccidentalStyleKindAtom ();

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

    lpsrAccidentalStyleKind&
                          fLpsrAccidentalStyleKindVariable;
};
typedef SMARTP<lilypondAccidentalStyleKindAtom> S_lilypondAccidentalStyleKindAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondAccidentalStyleKindAtom& elt);

//______________________________________________________________________________
class EXP lilypondChordsDisplayAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondChordsDisplayAtom> create (
                            const string&         shortName,
                            const string&         longName,
                            const string&         description,
                            const string&         valueSpecification,
                            const string&         variableName,
                            list<pair<string, string> >&
                                                  lilypondChordsDisplayVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondChordsDisplayAtom (
                            const string&         shortName,
                            const string&         longName,
                            const string&         description,
                            const string&         valueSpecification,
                            const string&         variableName,
                            list<pair<string, string> >&
                                                  lilypondChordsDisplayVariable);

    virtual               ~lilypondChordsDisplayAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringsPairListVariable (
                            pair<string, string> value)
                              {
                                fStringsPairListVariable.push_back (value);
                              }

    const list<pair<string, string> >&
                          getStringsPairListVariable () const
                              { return fStringsPairListVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    list<pair<string, string> >&
                          fStringsPairListVariable;
};
typedef SMARTP<lilypondChordsDisplayAtom> S_lilypondChordsDisplayAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondChordsDisplayAtom& elt);

//______________________________________________________________________________
class EXP lilypondLyricsDurationsKindAtom : public oahAtomWithValue
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

    virtual               ~lilypondLyricsDurationsKindAtom ();

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

    lpsrLyricsDurationsKind&
                          fLpsrLyricsDurationsKindVariable;
};
typedef SMARTP<lilypondLyricsDurationsKindAtom> S_lilypondLyricsDurationsKindAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondLyricsDurationsKindAtom& elt);

//______________________________________________________________________________
class EXP lpsr2lilypondOahGroup : public oahGroup
{
  public:

    // data types
    // ------------------------------------------------------

    enum scoreNotationKind {
      kWesternNotation,
      kJianpuNotation,
      kABCNotation // not used yet
    };

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

    virtual               ~lpsr2lilypondOahGroup ();

    // set and get
    // ------------------------------------------------------

 // JMI   bool                  setAccidentalStyleKind (
  //                          lpsrAccidentalStyleKind accidentalStyleKind);

    bool                  setScoreOutputKind (
                            string outputKind);

    // LilyPond version
    // --------------------------------------

    string                getLilypondVersion () const
                              { return fLilypondVersion; }
    S_oahStringAtom       getLilypondVersionAtom () const
                              { return fLilypondVersionAtom; }

    string                getLilypondVersionDefaultValue () const
                              { return fLilypondVersionDefaultValue; }
    // score notation
    // --------------------------------------

    void                  setJianpu ()
                              { fJianpu = true; }
    bool                  getJianpu () const
                              { return fJianpu; }

    void                  setLyLuaTexOutput ()
                              { fLyLuaTexOutput = true; }
    bool                  getLyLuaTexOutput () const
                              { return fLyLuaTexOutput; }

    // global staff size
    // --------------------------------------

    float                 getGlobalStaffSize () const
                              { return fGlobalStaffSize; }
    S_oahFloatAtom        getGlobalStaffSizeAtom () const
                              { return fGlobalStaffSizeAtom; }

    float                 getStaffGlobalSizeDefaultValue () const
                              { return fStaffGlobalSizeDefaultValue; }

    bool                  getNoSetGlobalStaffSize () const
                              { return fNoSetGlobalStaffSize; }
    S_oahBooleanAtom      getNoSetGlobalStaffSizeAtom () const
                              { return fNoSetGlobalStaffSizeAtom; }

    // languages
    // --------------------------------------

    bool                  getUseLilypondDefaultLanguages () const
                              { return fUseLilypondDefaultLanguages; }
    S_oahBooleanAtom      getUseLilypondDefaultLanguagesAtom () const
                              { return fUseLilypondDefaultLanguagesAtom; }

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

    // Lilypond informations
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

    // book block
    // --------------------------------------

    bool                  getNoBookBlock () const
                              { return fNoBookBlock; }
    S_oahBooleanAtom      getNoBookBlockAtom () const
                              { return fNoBookBlockAtom; }

    // header block
    // --------------------------------------

    bool                  getNoHeaderBlock () const
                              { return fNoHeaderBlock; }
    S_oahBooleanAtom      getNoHeaderBlockAtom () const
                              { return fNoHeaderBlockAtom; }

    // layout block
    // --------------------------------------

    bool                  getNoLayoutScoreContext () const
                              { return fNoLayoutScoreContext; }
    S_oahBooleanAtom      getNoLayoutScoreContextAtom () const
                              { return fNoLayoutScoreContextAtom; }

    bool                  getNoLayoutVoiceContext () const
                              { return fNoLayoutVoiceContext; }
    S_oahBooleanAtom      getNoLayoutVoiceContextAtom () const
                              { return fNoLayoutVoiceContextAtom; }

    bool                  getNoTopLevelLayoutBlock () const
                              { return fNoTopLevelLayoutBlock; }

    // paper block
    // --------------------------------------

    bool                  getNoPaperBlock () const
                              { return fNoPaperBlock; }
    S_oahBooleanAtom      getNoPaperBlockAtom () const
                              { return fNoPaperBlockAtom; }

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

    // clefs
    // --------------------------------------

    bool                  getNoInitialTrebleClef () const
                              { return fNoInitialTrebleClef; }

    bool                  getCommentClefChanges () const
                              { return fCommentClefChanges; }

    // keys
    // --------------------------------------

    bool                  getNoInitialCMajorKey () const
                              { return fNoInitialCMajorKey; }

    // times
    // --------------------------------------

    bool                  getNoInitialCommonTime () const
                              { return fNoInitialCommonTime; }

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


    void                  setAllDurations ()
                              { fAllDurations = true; }
    bool                  getAllDurations () const
                              { return fAllDurations; }

    void                  setNoAutoBeaming ()
                              { fNoAutoBeaming = true; }
    bool                  getNoAutoBeaming () const
                              { return fNoAutoBeaming; }

    void                  setNoBeams ()
                              { fNoBeams = true; }
    bool                  getNoBeams () const
                              { return fNoBeams; }

    void                  setRomanStringNumbers ()
                              { fRomanStringNumbers = true; }
    bool                  getRomanStringNumbers () const
                              { return fRomanStringNumbers; }

    void                  setAvoidOpenStrings ()
                              { fAvoidOpenStrings = true; }
    bool                  getAvoidOpenStrings () const
                              { return fAvoidOpenStrings; }

    lpsrAccidentalStyleKind
                          getAccidentalStyleKind () const
                              { return fAccidentalStyleKind; }

    void                  setCompressFullMeasureRests ()
                              { fCompressFullMeasureRests = true; }
    bool                  getCompressFullMeasureRests () const
                              { return fCompressFullMeasureRests; }

    void                  setMergeRests ()
                              { fMergeRests = true; }
    bool                  getMergeRests () const
                              { return fMergeRests; }

    void                  setInputLineNumbers ()
                              { fInputLineNumbers = true; }
    bool                  getInputLineNumbers () const
                              { return fInputLineNumbers; }

    void                  setOriginalMeasureNumbers ()
                              { fOriginalMeasureNumbers = true; }
    bool                  getOriginalMeasureNumbers () const
                              { return fOriginalMeasureNumbers; }

    void                  setGeneratePositionsInMeasures ()
                              { fGeneratePositionsInMeasures = true; }
    bool                  getGeneratePositionsInMeasures () const
                              { return fGeneratePositionsInMeasures; }

    void                  setGenerateMomentsInMeasures ()
                              { fGenerateMomentsInMeasures = true; }
    bool                  getGenerateMomentsInMeasures () const
                              { return fGenerateMomentsInMeasures; }

    const msrRGBColor&    getNonPrintNotesHeadRGBColor () const
                              { return fNonPrintNotesHeadRGBColor; }
    S_oahRGBColorAtom     getNonPrintNotesHeadRGBColorAtom () const
                              { return fNonPrintNotesHeadRGBColorAtom; }

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

    void                  setShowAllBarNumbers ()
                              { fShowAllBarNumbers = true; }
    bool                  getShowAllBarNumbers () const
                              { return fShowAllBarNumbers; }

    const set<int>&       getBoxAroundBarNumberSet () const
                              { return fBoxAroundBarNumberSet; }

    // bar number checks
    // --------------------------------------

    void                  setNoBarNumberChecks ()
                              { fNoBarNumberChecks = true; }
    bool                  getNoBarNumberChecks () const
                              { return fNoBarNumberChecks; }

    // line breaks
    // --------------------------------------

    void                  setIgnoreMusicXMLLineBreaks ()
                              { fIgnoreMusicXMLLineBreaks = true; }
    bool                  getIgnoreMusicXMLLineBreaks () const
                              { return fIgnoreMusicXMLLineBreaks; }

    void                  setSuppressMusicXMLLineBreaks ()
                              { fSuppressMusicXMLLineBreaks = true; }
    bool                  getSuppressMusicXMLLineBreaks () const
                              { return fSuppressMusicXMLLineBreaks; }

    void                  setBreakLinesAtIncompleteRightMeasures ()
                              { fBreakLinesAtIncompleteRightMeasures = true; }
    bool                  getBreakLinesAtIncompleteRightMeasures () const
                              { return fBreakLinesAtIncompleteRightMeasures; }

    void                  setSeparatorLineEveryNMeasures (int value)
                              { fSeparatorLineEveryNMeasures = value; }
    int                   getSeparatorLineEveryNMeasures () const
                              { return fSeparatorLineEveryNMeasures; }

    const set<string>&    getBreakLineAfterMeasureNumberSet () const
                              { return fBreakLineAfterMeasureNumberSet; }

    // page breaks
    // --------------------------------------

    void                  setIgnoreMusicXMLPageBreaks ()
                              { fIgnoreMusicXMLPageBreaks = true; }
    bool                  getIgnoreMusicXMLPageBreaks () const
                              { return fIgnoreMusicXMLPageBreaks; }

    void                  setSuppressMusicXMLPageBreaks ()
                              { fSuppressMusicXMLPageBreaks = true; }
    bool                  getSuppressMusicXMLPageBreaks () const
                              { return fSuppressMusicXMLPageBreaks; }

    const set<string>&    getBreakPageAfterMeasureNumberSet () const
                              { return fBreakPageAfterMeasureNumberSet; }


    // staves
    // --------------------------------------

    void                  setModernTab ()
                              { fModernTab = true; }
    bool                  getModernTab () const
                              { return fModernTab; }

    void                  setTabFullNotation ()
                              { fTabFullNotation = true; }
    bool                  getTabFullNotation () const
                              { return fTabFullNotation; }

    void                  setKeepStaffSize ()
                              { fKeepStaffSize = true; }
    bool                  getKeepStaffSize () const
                              { return fKeepStaffSize; }

    const msrRGBColor&    getLedgerLinesRGBColor () const
                              { return fLedgerLinesRGBColor; }
    S_oahRGBColorAtom     getLedgerLinesRGBColorAtom () const
                              { return fLedgerLinesRGBColorAtom; }

    // chords
    // --------------------------------------

    void                  setConnectArpeggios ()
                              { fConnectArpeggios = true; }
    bool                  getConnectArpeggios () const
                              { return fConnectArpeggios; }


    // tuplets
    // --------------------------------------

    void                  setIndentTuplets ()
                              { fIndentTuplets = true; }
    bool                  getIndentTuplets () const
                              { return fIndentTuplets; }


    // repeats
    // --------------------------------------

    void                  setKeepRepeatBarlines ()
                              { fKeepRepeatBarlines = true; }
    bool                  getKeepRepeatBarlines () const
                              { return fKeepRepeatBarlines; }

    void                  setRepeatBrackets ()
                              { fRepeatBrackets = true; }
    bool                  getRepeatBrackets () const
                              { return fRepeatBrackets; }

    void                  setIgnoreRepeatNumbers ()
                              { fIgnoreRepeatNumbers = true; }
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

    void                  setJazzChordsDisplay ()
                              { fJazzChordsDisplay = true; }
    bool                  getJazzChordsDisplay () const
                              { return fJazzChordsDisplay; }

    string                getJazzChordsDisplayLilypondcode () const
                              { return fJazzChordsDisplayLilypondcode; }


    // fonts
    // --------------------------------------

    void                  setJazzFonts ()
                              { fJazzFonts = true; }
    bool                  getJazzFonts () const
                              { return fJazzFonts; }


    // code generation
    // --------------------------------------

    void                  setXml2lyInfos ()
                              { fXml2lyInfos = true; }
    bool                  getXml2lyInfos () const
                              { return fXml2lyInfos; }

    void                  setLilypondComments ()
                              { fLilypondComments = true; }
    bool                  getLilypondComments () const
                              { return fLilypondComments; }

    void                  setGlobal ()
                              { fGlobal = true; }
    bool                  getGlobal () const
                              { return fGlobal; }

    void                  setDisplayMusic ()
                              { fDisplayMusic = true; }
    bool                  getDisplayMusic () const
                              { return fDisplayMusic; }

    void                  setNoLilypondCode ()
                              { fNoLilypondCode = true; }
    bool                  getNoLilypondCode () const
                              { return fNoLilypondCode; }

    void                  setNoLilypondLyrics ()
                              { fNoLilypondLyrics = true; }
    bool                  getNoLilypondLyrics () const
                              { return fNoLilypondLyrics; }

    void                  setLilypondCompileDate ()
                              { fLilypondCompileDate = true; }
    bool                  getLilypondCompileDate () const
                              { return fLilypondCompileDate; }

    void                  setPointAndClickOff ()
                              { fPointAndClickOff = true; }
    bool                  getPointAndClickOff () const
                              { return fPointAndClickOff; }

    void                  setDraftMode ()
                              { fDraftMode = true; }
    bool                  getDraftMode () const
                              { return fDraftMode; }

    void                  setWhiteNoteHeads ()
                              { fWhiteNoteHeads = true; }
    bool                  getWhiteNoteHeads () const
                              { return fWhiteNoteHeads; }

    void                  setGenerateCommentedOutVariables ()
                              { fGenerateCommentedOutVariables = true; }
    bool                  getGenerateCommentedOutVariables () const
                              { return fGenerateCommentedOutVariables; }


    // lyrics
    // --------------------------------------

    void                  setAddStanzasNumbers ()
                              { fAddStanzasNumbers = true; }
    bool                  getAddStanzasNumbers () const
                              { return fAddStanzasNumbers; }

    void                  setLyricsDurationsKind (
                            lpsrLyricsDurationsKind value)
                              { fLyricsDurationsKind = value; }
    lpsrLyricsDurationsKind
                          getLyricsDurationsKind () const
                              { return fLyricsDurationsKind; }

    // midi
    // --------------------------------------

    void                  getMidiTempo (msrMidiTempo value)
                              { fMidiTempo = value; }
    msrMidiTempo          getMidiTempo () const
                              { return fMidiTempo; }

    void                  setNoMidi ()
                              { fNoMidi = true; }
    bool                  getNoMidi () const
                              { return fNoMidi; }

/* JMI ???
    bool                  getCreateScoreBlockMidiBlock () const
                              { return fetCreateScoreBlockMidiBlock; }
*/

    // combined options, minimal
    // --------------------------------------

    void                  setMinimal ()
                              { fMinimal = true; }
    bool                  getMinimal () const
                              { return fMinimal; }

  public:

    // public services
    // ------------------------------------------------------

    // LilyPond version
    bool                  versionNumberGreaterThanOrEqualTo (
                            string otherVersionNumber);

    void                  setOctaveEntryKindToAbsolute () // JMI ???
                              {
                                fOctaveEntryKind =
                                  kOctaveEntryAbsolute;
                              }

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    void                  checkGroupOptionsConsistency () override;

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeLilypondVersionOptions ();

    void                  initializeScoreNotationOptions ();

    void                  initializeGlobalStaffSizeOptions ();

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

    void                  initializeMidiOptions ();

    void                  initializeCombinedOptionsOptions ();

    void                  crackLilypondVersionNumber (
                            string theString,
                            int&   generationNumber,
                            int&   majorNumber,
                            int&   minorNumber);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printLpsr2lilypondOahHelp ();

    void                  printLpsr2lilypondOahValues (unsigned int fieldWidth);

    virtual void          printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const;

  private:

    // private fields
    // --------------------------------------

    // LilyPond version
    // --------------------------------------

    string                fLilypondVersion;
    S_oahStringAtom       fLilypondVersionAtom;

    string                fLilypondVersionDefaultValue;

    // score notation
    // --------------------------------------

    // scoreNotationKind     fScoreNotationKind; JMI ???
    bool                  fJianpu;

    // lyluatex output
    // --------------------------------------

    // to embed LilyPond code in LyLuaTex code
    bool                  fLyLuaTexOutput;

    // global staff size
    // --------------------------------------

    float                 fGlobalStaffSize;
    S_oahFloatAtom        fGlobalStaffSizeAtom;

    float                 fStaffGlobalSizeDefaultValue;

    bool                  fNoSetGlobalStaffSize;
    S_oahBooleanAtom      fNoSetGlobalStaffSizeAtom;

    // languages
    // --------------------------------------

    bool                  fUseLilypondDefaultLanguages;
    S_oahBooleanAtom      fUseLilypondDefaultLanguagesAtom;

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

    // book block
    // --------------------------------------

    bool                  fNoBookBlock;
    // this atom is used by the '-minimal' combined option
    S_oahBooleanAtom      fNoBookBlockAtom;

    // header block
    // --------------------------------------

    bool                  fNoHeaderBlock;
    // this atom is used by the '-minimal' combined option
    S_oahBooleanAtom      fNoHeaderBlockAtom;

    // layout block
    // --------------------------------------

    bool                  fNoLayoutScoreContext;
    S_oahBooleanAtom      fNoLayoutScoreContextAtom;

    bool                  fNoLayoutVoiceContext;
    S_oahBooleanAtom      fNoLayoutVoiceContextAtom;

    bool                  fNoTopLevelLayoutBlock;

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

    // clefs
    // --------------------------------------

    bool                  fNoInitialTrebleClef;

    bool                  fCommentClefChanges;

    // keys
    // --------------------------------------

    bool                  fNoInitialCMajorKey;

    // times
    // --------------------------------------

    bool                  fNoInitialCommonTime;

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

    bool                  fGeneratePositionsInMeasures;
    bool                  fGenerateMomentsInMeasures;

    // for MusicXML's <note print-object="no">
    msrRGBColor           fNonPrintNotesHeadRGBColor; // #(rgb-color 1.0 0.9 0.5)
    S_oahRGBColorAtom     fNonPrintNotesHeadRGBColorAtom;

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
    S_oahRGBColorAtom     fLedgerLinesRGBColorAtom;

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

    bool                  fLilypondComments;

    bool                  fGlobal;

    bool                  fDisplayMusic;

    bool                  fNoLilypondCode;

    bool                  fNoLilypondLyrics;

    bool                  fLilypondCompileDate;

    bool                  fPointAndClickOff;

    bool                  fDraftMode;

    bool                  fWhiteNoteHeads;

    bool                  fGenerateCommentedOutVariables;
                            // this is to avoid having to add them by hand

    // lyrics
    // --------------------------------------

    bool                  fAddStanzasNumbers;

    lpsrLyricsDurationsKind
                          fLyricsDurationsKind;

    // midi
    // --------------------------------------

    msrMidiTempo          fMidiTempo;

    bool                  fNoMidi;

    // combined options
    // --------------------------------------

    // minimal
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
EXP S_lpsr2lilypondOahGroup createGlobalLpsr2lilypondOahGroup ();
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
class EXP lilypondBreakPageAfterMeasureNumberAtom : public oahAtomWithValue
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

    virtual               ~lilypondBreakPageAfterMeasureNumberAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const set<string>&    getStringSetVariable ()
                              { return fStringSetVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

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

