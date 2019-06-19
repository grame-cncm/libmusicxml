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

    string                getOptionsScoreOutputKindItemVariableDisplayName () const
                              {
                                return
                                  fOptionsScoreOutputKindItemVariableDisplayName;
                              }

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

    string                fOptionsScoreOutputKindItemVariableDisplayName;
    lpsrScoreOutputKind&
                          fOptionsScoreOutputKindItemVariable;
};
typedef SMARTP<optionsScoreOutputKindItem> S_optionsScoreOutputKindItem;
EXP ostream& operator<< (ostream& os, const S_optionsScoreOutputKindItem& elt);

//______________________________________________________________________________
class optionsOctaveEntryKindItem : public optionsValuedItem
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsOctaveEntryKindItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsOctaveEntryKindItemVariableDisplayName,
      lpsrOctaveEntryKind&
                         optionsOctaveEntryKindItemVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsOctaveEntryKindItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsOctaveEntryKindItemVariableDisplayName,
      lpsrOctaveEntryKind&
                         optionsOctaveEntryKindItemVariable);

    virtual ~optionsOctaveEntryKindItem ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getOptionsOctaveEntryKindItemVariableDisplayName () const
                              {
                                return
                                  fOptionsOctaveEntryKindItemVariableDisplayName;
                              }

    void                  setOctaveEntryKindItemVariableValue (
                            lpsrOctaveEntryKind value)
                              {
                                fOptionsOctaveEntryKindItemVariable = value;
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

    string                fOptionsOctaveEntryKindItemVariableDisplayName;
    lpsrOctaveEntryKind&  fOptionsOctaveEntryKindItemVariable;
};
typedef SMARTP<optionsOctaveEntryKindItem> S_optionsOctaveEntryKindItem;
EXP ostream& operator<< (ostream& os, const S_optionsOctaveEntryKindItem& elt);

//______________________________________________________________________________
class optionsOctaveEntryReferenceItem : public optionsValuedItem
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsOctaveEntryReferenceItem> create (
      string  optionsItemShortName,
      string  optionsItemLongName,
      string  optionsItemDescription,
      string  optionsValueSpecification,
      string  optionsStringItemVariableDisplayName,
      S_msrSemiTonesPitchAndOctave&
              optionsOctaveEntryReferenceItemVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsOctaveEntryReferenceItem (
      string  optionsItemShortName,
      string  optionsItemLongName,
      string  optionsItemDescription,
      string  optionsValueSpecification,
      string  optionsStringItemVariableDisplayName,
      S_msrSemiTonesPitchAndOctave&
              optionsOctaveEntryReferenceItemVariable);

    virtual ~optionsOctaveEntryReferenceItem ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getOptionsTransposeItemVariableDisplayName () const
                              {
                                return
                                  fOptionsTransposeItemVariableDisplayName;
                              }

    void                  setTransposeItemVariableValue (
                            S_msrSemiTonesPitchAndOctave value)
                              {
                                fOptionsTransposeItemVariable = value;
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

    string                fOptionsTransposeItemVariableDisplayName;
    S_msrSemiTonesPitchAndOctave&
                          fOptionsTransposeItemVariable;
};
typedef SMARTP<optionsOctaveEntryReferenceItem> S_optionsOctaveEntryReferenceItem;
EXP ostream& operator<< (ostream& os, const S_optionsOctaveEntryReferenceItem& elt);

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

    string                getOptionsAccidentalStyleKindItemVariableDisplayName () const
                              {
                                return
                                  fOptionsAccidentalStyleKindItemVariableDisplayName;
                              }

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

    string                fOptionsAccidentalStyleKindItemVariableDisplayName;
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

    string                getOptionsChordsDisplayItemVariableDisplayName () const
                              {
                                return
                                  foptionsChordsDisplayItemVariableDisplayName;
                              }

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

    string                foptionsChordsDisplayItemVariableDisplayName;
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

    string                getOptionsMidiTempoItemVariableDisplayName () const
                              {
                                return
                                  fOptionsMidiTempoItemVariableDisplayName;
                              }

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

    string                fOptionsMidiTempoItemVariableDisplayName;
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

    void                  handleOptionsOctaveEntryKindItemValue (
                            ostream&      os,
                            S_optionsOctaveEntryKindItem
                                          octaveEntryKindItem,
                            string        theString);

    void                  handleOptionsAccidentalStyleKindItemValue (
                            ostream&      os,
                            S_optionsAccidentalStyleKindItem
                                          accidentalStyleKindItem,
                            string        theString);

    void                  handleOptionsChordsDisplayItemValue (
                            ostream&      os,
                            S_optionsChordsDisplayItem
                                          ChordsDisplayItem,
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

    // engravers
    // --------------------------------------

    bool                  fAmbitusEngraver;

    // time
    // --------------------------------------

    bool                  fNumericalTime;

    // notes
    // --------------------------------------

    lpsrOctaveEntryKind   fOctaveEntryKind;
    // this is relevant only for relative and fixed octave entry kinds
    S_msrSemiTonesPitchAndOctave
                          fSemiTonesPitchAndOctave;
//    function<void(void)>  setOctaveEntryKindToAbsolute ();

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

//______________________________________________________________________________
void initializeLilypondOptionsHandling (
  S_optionsHandler optionsHandler);


}


#endif
