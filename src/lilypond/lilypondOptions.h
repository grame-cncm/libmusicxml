/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lilypondOptions___
#define ___lilypondOptions___

//#include <set>

#include "optionsBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class optionsScoreOutputKindItem : public optionsValuedItem
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsScoreOutputKindItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsScoreOutputKindItemVariableDisplayName,
      lpsrScoreOutputKind&
                         optionsScoreOutputKindItemVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsScoreOutputKindItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsScoreOutputKindItemVariableDisplayName,
      lpsrScoreOutputKind&
                         optionsScoreOutputKindItemVariable);

    virtual ~optionsScoreOutputKindItem ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setScoreOutputKindItemVariableValue (
                            lpsrScoreOutputKind value)
                              {
                                fOptionsScoreOutputKindItemVariable = value;
                              }

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    lpsrScoreOutputKind&  fOptionsScoreOutputKindItemVariable;
};
typedef SMARTP<optionsScoreOutputKindItem> S_optionsScoreOutputKindItem;
EXP ostream& operator<< (ostream& os, const S_optionsScoreOutputKindItem& elt);

//______________________________________________________________________________
class optionsAbsoluteOctaveEntryItem : public optionsItemWithVariableDisplayName
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsAbsoluteOctaveEntryItem> create (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription,
      string optionsAbsoluteOctaveEntryItemVariableDisplayName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsAbsoluteOctaveEntryItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription,
      string optionsAbsoluteOctaveEntryItemVariableDisplayName);

    virtual ~optionsAbsoluteOctaveEntryItem ();

  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------
    string                fOptionsAbsoluteOctaveEntryItemVariableDisplayName;
                            // in order for it to be displayed,
                            // even if there's no associated variable
};
typedef SMARTP<optionsAbsoluteOctaveEntryItem> S_optionsAbsoluteOctaveEntryItem;
EXP ostream& operator<< (ostream& os, const S_optionsAbsoluteOctaveEntryItem& elt);

//______________________________________________________________________________
class optionsRelativeOctaveEntryItem : public optionsValuedItem
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsRelativeOctaveEntryItem> create (
      string  optionsItemShortName,
      string  optionsItemLongName,
      string  optionsItemDescription,
      string  optionsValueSpecification,
      string  optionsRelativeOctaveEntryItemVariableDisplayName,
      S_msrSemiTonesPitchAndOctave&
              optionsRelativeOctaveEntryItemVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsRelativeOctaveEntryItem (
      string  optionsItemShortName,
      string  optionsItemLongName,
      string  optionsItemDescription,
      string  optionsValueSpecification,
      string  optionsRelativeOctaveEntryItemVariableDisplayName,
      S_msrSemiTonesPitchAndOctave&
              optionsRelativeOctaveEntryItemVariable);

    virtual ~optionsRelativeOctaveEntryItem ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setRelativeOctaveEntryItemVariableValue (
                            S_msrSemiTonesPitchAndOctave value)
                              {
                                fOptionsRelativeOctaveEntryItemVariable = value;
                              }

    // services
    // ------------------------------------------------------

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
                          fOptionsRelativeOctaveEntryItemVariable;
};
typedef SMARTP<optionsRelativeOctaveEntryItem> S_optionsRelativeOctaveEntryItem;
EXP ostream& operator<< (ostream& os, const S_optionsRelativeOctaveEntryItem& elt);

//______________________________________________________________________________
class optionsFixedOctaveEntryItem : public optionsValuedItem
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsFixedOctaveEntryItem> create (
      string  optionsItemShortName,
      string  optionsItemLongName,
      string  optionsItemDescription,
      string  optionsValueSpecification,
      string  optionsFixedOctaveEntryItemVariableDisplayName,
      S_msrSemiTonesPitchAndOctave&
              optionsFixedOctaveEntryItemVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsFixedOctaveEntryItem (
      string  optionsItemShortName,
      string  optionsItemLongName,
      string  optionsItemDescription,
      string  optionsValueSpecification,
      string  optionsFixedOctaveEntryItemVariableDisplayName,
      S_msrSemiTonesPitchAndOctave&
              optionsFixedOctaveEntryItemVariable);

    virtual ~optionsFixedOctaveEntryItem ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setFixedOctaveEntryItemVariableValue (
                            S_msrSemiTonesPitchAndOctave value)
                              {
                                fOptionsFixedOctaveEntryItemVariable = value;
                              }

    // services
    // ------------------------------------------------------

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
                          fOptionsFixedOctaveEntryItemVariable;
};
typedef SMARTP<optionsFixedOctaveEntryItem> S_optionsFixedOctaveEntryItem;
EXP ostream& operator<< (ostream& os, const S_optionsFixedOctaveEntryItem& elt);

//______________________________________________________________________________
class optionsAccidentalStyleKindItem : public optionsValuedItem
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsAccidentalStyleKindItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsAccidentalStyleKindItemVariableDisplayName,
      lpsrAccidentalStyleKind&
                         optionsAccidentalStyleKindItemVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsAccidentalStyleKindItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsAccidentalStyleKindItemVariableDisplayName,
      lpsrAccidentalStyleKind&
                         optionsAccidentalStyleKindItemVariable);

    virtual ~optionsAccidentalStyleKindItem ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setAccidentalStyleKindItemVariableValue (
                            lpsrAccidentalStyleKind value)
                              {
                                fOptionsAccidentalStyleKindItemVariable = value;
                              }

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    lpsrAccidentalStyleKind&
                          fOptionsAccidentalStyleKindItemVariable;
};
typedef SMARTP<optionsAccidentalStyleKindItem> S_optionsAccidentalStyleKindItem;
EXP ostream& operator<< (ostream& os, const S_optionsAccidentalStyleKindItem& elt);

//______________________________________________________________________________
class optionsChordsDisplayItem : public optionsValuedItem
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsChordsDisplayItem> create (
      string                optionsItemShortName,
      string                optionsItemLongName,
      string                optionsItemDescription,
      string                optionsValueSpecification,
      string                optionsChordsDisplayItemVariableDisplayName,
      list<pair<string, string> >&
                            optionsChordsDisplayItemVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsChordsDisplayItem (
      string                optionsItemShortName,
      string                optionsItemLongName,
      string                optionsItemDescription,
      string                optionsValueSpecification,
      string                optionsChordsDisplayItemVariableDisplayName,
      list<pair<string, string> >&
                            optionsChordsDisplayItemVariable);

    virtual ~optionsChordsDisplayItem ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setOptionsChordsDisplayItemVariableDisplayName (
                            pair<string, string> value)
                              {
                                foptionsChordsDisplayItemVariable.push_back (value);
                              }

    const list<pair<string, string> >&
                          getOptionsChordsDisplayItemVariable ()
                              {
                                return
                                  foptionsChordsDisplayItemVariable;
                              }

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    list<pair<string, string> >&
                          foptionsChordsDisplayItemVariable;
};
typedef SMARTP<optionsChordsDisplayItem> S_optionsChordsDisplayItem;
EXP ostream& operator<< (ostream& os, const S_optionsChordsDisplayItem& elt);

//______________________________________________________________________________
class optionsMidiTempoItem : public optionsValuedItem
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsMidiTempoItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsMidiTempoItemVariableDisplayName,
      pair<string, int>& optionsMidiTempoItemVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsMidiTempoItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsMidiTempoItemVariableDisplayName,
      pair<string, int>& optionsMidiTempoItemVariable);

    virtual ~optionsMidiTempoItem ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMidiTempoItemVariableValue (
                            pair<string, int> value)
                              {
                                fOptionsMidiTempoItemVariable = value;
                              }

    const pair<string, int>&
                          getOptionsMidiTempoItemVariable ()
                              {
                                return
                                  fOptionsMidiTempoItemVariable;
                              }

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    pair<string, int>&    fOptionsMidiTempoItemVariable;
};
typedef SMARTP<optionsMidiTempoItem> S_optionsMidiTempoItem;
EXP ostream& operator<< (ostream& os, const S_optionsMidiTempoItem& elt);

//______________________________________________________________________________
class lilypondOptions : public optionsGroup
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

    static SMARTP<lilypondOptions> create (
      S_optionsHandler optionsHandler);

    SMARTP<lilypondOptions>        createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeLilypondOptions (
                            bool boolOptionsInitialValue);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    lilypondOptions (
      S_optionsHandler optionsHandler);

    virtual ~lilypondOptions ();

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

    virtual S_optionsItem handleOptionsItem (
                            ostream&      os,
                            S_optionsItem item);

    virtual void          handleOptionsItemValue (
                            ostream&      os,
                            S_optionsItem item,
                            string        theString);

    void                  handleOptionsRelativeOctaveEntryItemValue (
                            ostream&      os,
                            S_optionsRelativeOctaveEntryItem
                                          relativeOctaveEntryItem,
                            string        theString);

    void                  handleOptionsFixedOctaveEntryItemValue (
                            ostream&      os,
                            S_optionsFixedOctaveEntryItem
                                          fixedOctaveEntryItem,
                            string        theString);

    void                  handleOptionsAccidentalStyleKindItemValue (
                            ostream&      os,
                            S_optionsAccidentalStyleKindItem
                                          accidentalStyleKindItem,
                            string        theString);

    void                  handleOptionsChordsDisplayItemValue (
                            ostream&      os,
                            S_optionsChordsDisplayItem
                                          chordsDisplayItem,
                            string        theString);

    void                  handleOptionsMidiTempoItemValue (
                            ostream&      os,
                            S_optionsMidiTempoItem
                                          midiTempoItem,
                            string        theString);

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeIdentificationOptions (
                            bool boolOptionsInitialValue);

    void                  initializeEngraversOptions (
                            bool boolOptionsInitialValue);

    void                  initializeTimeOptions (
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

    void                  printLilypondOptionsHelp ();

    void                  printLilypondOptionsValues (int fieldWidth);

    virtual void          printOptionsValues (
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

    // time
    // --------------------------------------

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

    bool                  fCompressRestMeasures;

    bool                  fNoteInputLineNumbers;


    // bars
    // --------------------------------------

    bool                  fShowAllBarNumbers;


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
typedef SMARTP<lilypondOptions> S_lilypondOptions;
EXP ostream& operator<< (ostream& os, const S_lilypondOptions& elt);

EXP extern S_lilypondOptions gLilypondOptions;
EXP extern S_lilypondOptions gLilypondOptionsUserChoices;
EXP extern S_lilypondOptions gLilypondOptionsWithDetailedTrace;

// JMI typedef void (lilypondOptions::*lilypondOptionsMethodPtr)(); //MyTypedef is a type!

/*
  It's worth noting that, as of C++11, you could write this expression
  as a more legible using statement:
    using lilypondOptionsMethodPtr = void (lilypondOptions::*)();
*/

//______________________________________________________________________________
void initializeLilypondOptionsHandling (
  S_optionsHandler optionsHandler);


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
