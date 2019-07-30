/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lilypondOah___
#define ___lilypondOah___

//#include <set>

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

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string   theString,
                            ostream& os);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    lpsrScoreOutputKind&  fLpsrScoreOutputKindVariable;
};
typedef SMARTP<lilypondScoreOutputKindAtom> S_lilypondScoreOutputKindAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondScoreOutputKindAtom& elt);

//______________________________________________________________________________
class lilypondAbsoluteOctaveEntryAtom : public oahAtomWithVariableName
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondAbsoluteOctaveEntryAtom> create (
      string shortName,
      string longName,
      string description,
      string lilypondAbsoluteOctaveEntryVariableName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lilypondAbsoluteOctaveEntryAtom (
      string shortName,
      string longName,
      string description,
      string lilypondAbsoluteOctaveEntryVariableName);

    virtual ~lilypondAbsoluteOctaveEntryAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------
    string                fOptionsAbsoluteOctaveEntryVariableName;
                            // in order for it to be displayed,
                            // even if there's no associated variable
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

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string   theString,
                            ostream& os);

    // print
    // ------------------------------------------------------

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
typedef SMARTP<lilypondRelativeOctaveEntryAtom> S_lilypondRelativeOctaveEntryAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondRelativeOctaveEntryAtom& elt);

//______________________________________________________________________________
class lilypondFixedOctaveEntryAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondFixedOctaveEntryAtom> create (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      S_msrSemiTonesPitchAndOctave&
              lilypondFixedOctaveEntryVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lilypondFixedOctaveEntryAtom (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      S_msrSemiTonesPitchAndOctave&
              lilypondFixedOctaveEntryVariable);

    virtual ~lilypondFixedOctaveEntryAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setFixedOctaveEntryVariableValue (
                            S_msrSemiTonesPitchAndOctave value)
                              {
                                fMsrSemiTonesPitchAndOctaveVariable = value;
                              }

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string   theString,
                            ostream& os);

    // print
    // ------------------------------------------------------

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
typedef SMARTP<lilypondFixedOctaveEntryAtom> S_lilypondFixedOctaveEntryAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondFixedOctaveEntryAtom& elt);

//______________________________________________________________________________
class lilypondResetMeasureNumberAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondResetMeasureNumberAtom> create (
      string         shortName,
      string         longName,
      string         description,
      string         valueSpecification,
      string         variableName,
      map<int, int>& lilypondResetMeasureNumberVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lilypondResetMeasureNumberAtom (
      string         shortName,
      string         longName,
      string         description,
      string         valueSpecification,
      string         variableName,
      map<int, int>& lilypondResetMeasureNumberVariable);

    virtual ~lilypondResetMeasureNumberAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<int, int>&  getIntIntMapVariable ()
                              { return fIntIntMapVariable; }

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string   theString,
                            ostream& os);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    map<int, int>&        fIntIntMapVariable;
};
typedef SMARTP<lilypondResetMeasureNumberAtom> S_lilypondResetMeasureNumberAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondResetMeasureNumberAtom& elt);

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

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string   theString,
                            ostream& os);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

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

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string  theString,
                            ostream& os);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    list<pair<string, string> >&
                          fStringsPairListVariable;
};
typedef SMARTP<lilypondChordsDisplayAtom> S_lilypondChordsDisplayAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondChordsDisplayAtom& elt);

//______________________________________________________________________________
class lilypondMidiTempoAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondMidiTempoAtom> create (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      pair<string, int>& lilypondMidiTempoVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lilypondMidiTempoAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      pair<string, int>& lilypondMidiTempoVariable);

    virtual ~lilypondMidiTempoAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringIntPairVariable (
                            pair<string, int> value)
                              { fStringIntPairVariable = value; }

    const pair<string, int>&
                          getStringIntPairVariable ()
                              { return fStringIntPairVariable; }

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string   theString,
                            ostream& os);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    pair<string, int>&    fStringIntPairVariable;
};
typedef SMARTP<lilypondMidiTempoAtom> S_lilypondMidiTempoAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondMidiTempoAtom& elt);

//______________________________________________________________________________
class lilypondOah : public oahGroup
{
  public:

    // data types
    // ------------------------------------------------------

    enum scoreNotationKind {
      kWesternNotation, kJianpuNotation, kABCNotation }; // JMI

    static string scoreNotationKindAsString (
      scoreNotationKind notationKind);

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

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondOah> create (
      S_oahHandler handler);

    SMARTP<lilypondOah>        createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeLilypondOah (
                            bool boolOptionsInitialValue);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    lilypondOah (
      S_oahHandler handler);

    virtual ~lilypondOah ();

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

    virtual void          checkOptionsConsistency ();

  public:

    // public services
    // ------------------------------------------------------

    virtual S_oahValuedAtom
                          handleAtom (
                            ostream&  os,
                            S_oahAtom atom);

    virtual void          handleValuedAtomValue (
                            ostream&  os,
                            S_oahAtom atom,
                            string    theString);

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeIdentificationOptions (
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

    // print
    // ------------------------------------------------------

    void                  printLilypondOahHelp ();

    void                  printLilypondOahValues (int fieldWidth);

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


    // names
    // --------------------------------------


    // engravers
    // --------------------------------------

    bool                  fAmbitusEngraver;


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

    bool                  fStems;
    bool                  fNoAutoBeaming;

    bool                  fRomanStringNumbers;
    bool                  fAvoidOpenStrings;

    lpsrAccidentalStyleKind
                          fAccidentalStyleKind;

    bool                  fCompressFullMeasureRests;

    bool                  fInputLineNumbers;
    bool                  fPositionsInMeasures;


    // bars and measures
    // --------------------------------------

    bool                  fShowAllBarNumbers;

    map<int, int>         fResetMeasureNumberMap;


    // line breaks
    // --------------------------------------

    bool                  fIgnoreLineBreaks;

    bool                  fBreakLinesAtIncompleteRightMeasures;

    int                   fSeparatorLineEveryNMeasures;


    // page breaks
    // --------------------------------------

    bool                  fIgnorePageBreaks;


    // staves
    // --------------------------------------

    bool                  fModernTab;

    bool                  fKeepStaffSize;


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

    bool                  fComments;

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


    // midi
    // --------------------------------------

    pair<string, int>     fMidiTempo;

    bool                  fNoMidi;
};
typedef SMARTP<lilypondOah> S_lilypondOah;
EXP ostream& operator<< (ostream& os, const S_lilypondOah& elt);

EXP extern S_lilypondOah gLilypondOah;
EXP extern S_lilypondOah gLilypondOahUserChoices;
EXP extern S_lilypondOah gLilypondOahWithDetailedTrace;

// JMI typedef void (lilypondOah::*lilypondOahMethodPtr)(); //MyTypedef is a type!

/*
  It's worth noting that, as of C++11, you could write this expression
  as a more legible using statement:
    using lilypondOahMethodPtr = void (lilypondOah::*)();
*/

//______________________________________________________________________________
void initializeLilypondOahHandling (
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
