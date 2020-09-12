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

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class lilypondScoreOutputKindAtom : public oahValuedAtom
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

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os) override;

    void                  handleValue (
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

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------

    lpsrScoreOutputKind&  fLpsrScoreOutputKindVariable;
};
typedef SMARTP<lilypondScoreOutputKindAtom> S_lilypondScoreOutputKindAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondScoreOutputKindAtom& elt);

//______________________________________________________________________________
class lilypondTransposePartNameAtom : public oahValuedAtom
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

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os) override;

    void                  handleValue (
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

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------

    map<string, S_msrSemiTonesPitchAndOctave>&
                          fStringMsrSemiTonesPitchAndOctaveVariable;
};
typedef SMARTP<lilypondTransposePartNameAtom> S_lilypondTransposePartNameAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondTransposePartNameAtom& elt);

//______________________________________________________________________________
class lilypondTransposePartIDAtom : public oahValuedAtom
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

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os) override;

    void                  handleValue (
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

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
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

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
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

    void                  print (ostream& os) const override;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------

    lpsrOctaveEntryKind&  fLpsrOctaveEntryKindVariable;
};
typedef SMARTP<lilypondAbsoluteOctaveEntryAtom> S_lilypondAbsoluteOctaveEntryAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondAbsoluteOctaveEntryAtom& elt);

//______________________________________________________________________________
class lilypondRelativeOctaveEntryAtom : public oahValuedAtom
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

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os) override;

    void                  handleValue (
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

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------

    lpsrOctaveEntryKind&  fLpsrOctaveEntryKindVariable;

    S_msrSemiTonesPitchAndOctave&
                          fMsrSemiTonesPitchAndOctaveVariable;
};
typedef SMARTP<lilypondRelativeOctaveEntryAtom> S_lilypondRelativeOctaveEntryAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondRelativeOctaveEntryAtom& elt);

//______________________________________________________________________________
class lilypondFixedOctaveEntryAtom : public oahValuedAtom
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

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os) override;

    void                  handleValue (
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

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------

    lpsrOctaveEntryKind&  fLpsrOctaveEntryKindVariable;

    S_msrSemiTonesPitchAndOctave&
                          fMsrSemiTonesPitchAndOctaveVariable;
};
typedef SMARTP<lilypondFixedOctaveEntryAtom> S_lilypondFixedOctaveEntryAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondFixedOctaveEntryAtom& elt);

//______________________________________________________________________________
class lilypondAccidentalStyleKindAtom : public oahValuedAtom
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

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os) override;

    void                  handleValue (
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

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------

    lpsrAccidentalStyleKind&
                          fLpsrAccidentalStyleKindVariable;
};
typedef SMARTP<lilypondAccidentalStyleKindAtom> S_lilypondAccidentalStyleKindAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondAccidentalStyleKindAtom& elt);

//______________________________________________________________________________
class lilypondChordsDisplayAtom : public oahValuedAtom
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

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os) override;

    void                  handleValue (
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

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------

    list<pair<string, string> >&
                          fStringsPairListVariable;
};
typedef SMARTP<lilypondChordsDisplayAtom> S_lilypondChordsDisplayAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondChordsDisplayAtom& elt);

//______________________________________________________________________________
class lilypondLyricsDurationsKindAtom : public oahValuedAtom
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

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os) override;

    void                  handleValue (
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

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------

    lpsrLyricsDurationsKind&
                          fLpsrLyricsDurationsKindVariable;
};
typedef SMARTP<lilypondLyricsDurationsKindAtom> S_lilypondLyricsDurationsKindAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondLyricsDurationsKindAtom& elt);

//______________________________________________________________________________
class lpsr2LilypondOah : public oahGroup
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

    static SMARTP<lpsr2LilypondOah> create (
      S_oahHandler handlerUpLink);

    SMARTP<lpsr2LilypondOah>   createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeLpsr2LilypondOah (
                            bool boolOptionsInitialValue);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    lpsr2LilypondOah (
      S_oahHandler handlerUpLink);

    virtual ~lpsr2LilypondOah ();

    // set and get
    // ------------------------------------------------------

 // JMI   bool                  setAccidentalStyleKind (
  //                          lpsrAccidentalStyleKind accidentalStyleKind);

    bool                  setScoreOutputKind (
                            string outputKind);

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceQuietness ();

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkOptionsConsistency () override;

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeIdentificationOptions (
                            bool boolOptionsInitialValue);

    void                  initializePartsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeEngraversOptions (
                            bool boolOptionsInitialValue);

    void                  initializeClefsKeysTimesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeNotesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeBarsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeLineBreaksOptions (
                            bool boolOptionsInitialValue);

    void                  initializePageBreaksOptions (
                            bool boolOptionsInitialValue);

    void                  initializeStavesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeChordsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeTupletsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeRepeatOptions (
                            bool boolOptionsInitialValue);

    void                  initializeOrnamentsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeChordsDisplayOptions (
                            bool boolOptionsInitialValue);

    void                  initializeLyricsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeFontsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeCodeGenerationOptions (
                            bool boolOptionsInitialValue);

    void                  initializeScoreNotationOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMidiOptions (
                            bool boolOptionsInitialValue);

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

    void                  printLpsr2LilypondOahHelp ();

    void                  printLpsr2LilypondOahValues (int fieldWidth);

    virtual void          printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  public:

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


    // bars and measures
    // --------------------------------------

    bool                  fShowAllBarNumbers;

    map<string, int>      fResetMeasureElementMeasureNumberMap;
    map<string, int>      fAddEmptyMeasuresAfterMeasureNumberMap;

    set<int>              fBoxAroundBarNumberSet;


    // line breaks
    // --------------------------------------

    bool                  fIgnoreMusicXMLLineBreaks;

    bool                  fBreakLinesAtIncompleteRightMeasures;

    int                   fSeparatorLineEveryNMeasures;

    set<string>           fBreakLineAfterMeasureNumberSet;

    // page breaks
    // --------------------------------------

    bool                  fIgnoreMusicXMLPageBreaks;

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
};
typedef SMARTP<lpsr2LilypondOah> S_lpsr2LilypondOah;
EXP ostream& operator<< (ostream& os, const S_lpsr2LilypondOah& elt);

EXP extern S_lpsr2LilypondOah gGlobalLpsr2LilypondOah;
EXP extern S_lpsr2LilypondOah gGlobalLpsr2LilypondOahUserChoices;
EXP extern S_lpsr2LilypondOah gGlobalLpsr2LilypondOahWithDetailedTrace;

// JMI typedef void (lpsr2LilypondOah::*lpsr2LilypondOahMethodPtr)(); //MyTypedef is a type!

/*
  It's worth noting that, as of C++11, you could write this expression
  as a more legible using statement:
    using lpsr2LilypondOahMethodPtr = void (lpsr2LilypondOah::*)();
*/

//______________________________________________________________________________
void initializeLpsr2LilypondOahHandler (
  S_oahHandler handler);


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
class lilypondBreakPageAfterMeasureNumberAtom : public oahValuedAtom
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

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os) override;

    void                  handleValue (
                            string   theString,
                            ostream& os) override;

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
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

