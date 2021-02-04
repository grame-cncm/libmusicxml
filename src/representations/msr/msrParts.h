#ifndef ___msrParts___
#define ___msrParts___

#include "msrClefs.h"
#include "msrKeys.h"
#include "msrPartGroups.h"
#include "msrPartGroupElements.h"
#include "msrRepeats.h"
#include "msrRestMeasures.h"
#include "msrStaves.h"
#include "msrStavesDetails.h"
#include "msrTimes.h"
#include "msrTranspositions.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msrPartNameDisplay;
typedef SMARTP<msrPartNameDisplay> S_msrPartNameDisplay;

class msrPartAbbreviationDisplay;
typedef SMARTP<msrPartAbbreviationDisplay> S_msrPartAbbreviationDisplay;

class msrStaff;
typedef SMARTP<msrStaff> S_msrStaff;

class msrStaffDetails;
typedef SMARTP<msrStaffDetails> S_msrStaffDetails;

class msrHarmony;
typedef SMARTP<msrHarmony> S_msrHarmony;

class msrFiguredBass;
typedef SMARTP<msrFiguredBass> S_msrFiguredBass;

class msrGraceNotesGroup;
typedef SMARTP<msrGraceNotesGroup> S_msrGraceNotesGroup;

//______________________________________________________________________________
class EXP msrPart : public msrPartGroupElement
{
  public:

    // constants
    // ------------------------------------------------------

    #define K_PART_HARMONY_STAFF_NUMBER       10
    #define K_PART_HARMONY_VOICE_NUMBER       11

    #define K_PART_FIGURED_BASS_STAFF_NUMBER  20
    #define K_PART_FIGURED_BASS_VOICE_NUMBER  21

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPart> create (
                            int            inputLineNumber,
                            string         partID,
                            S_msrPartGroup partPartGroupUpLink);

    SMARTP<msrPart> createPartNewbornClone (
                            S_msrPartGroup partGroupClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrPart (
                            int            inputLineNumber,
                            string         partID,
                            S_msrPartGroup partPartGroupUpLink);

    virtual               ~msrPart ();

  private:

    // private initialization
    // ------------------------------------------------------

    void                  initializePart ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks

    void                  setPartPartGroupUpLink (
                            S_msrPartGroup partGroup)
                              { fPartPartGroupUpLink = partGroup; }

    S_msrPartGroup        getPartPartGroupUpLink () const
                              { return fPartPartGroupUpLink; }

    // part absolute number

    int                   getPartAbsoluteNumber () const
                              { return fPartAbsoluteNumber; }

    // ID and name

    void                  setPartID (string partID)
                              { fPartID = partID; }

    string                getPartID () const
                              { return fPartID; }

    void                  setPartMsrName (string partMsrName);

    string                getPartMsrName () const
                              { return fPartMsrName; }

    void                  setPartName (string partName)
                              { fPartName = partName; }

    string                getPartName () const
                              { return fPartName; }

    void                  setPartNameDisplayText (
                            string partNameDisplayText)
                              {
                                fPartNameDisplayText =
                                  partNameDisplayText;
                              }

    string                getPartNameDisplayText () const
                              { return fPartNameDisplayText; }

    void                  setPartAbbreviation (
                            string partAbbreviation)
                              {
                                fPartAbbreviation =
                                  partAbbreviation;
                              }

    string                getPartAbbreviation () const
                              { return fPartAbbreviation; }

    void                  setPartAbbreviationDisplayText (
                            string partAbbreviationDisplayText)
                              {
                                fPartAbbreviationDisplayText =
                                  partAbbreviationDisplayText;
                              }

    string                getPartAbbreviationDisplayText () const
                              { return fPartAbbreviationDisplayText; }

    string                getPartCombinedName () const;

    void                  setPartInstrumentNamesMaxLengthes ();

    // measures

    void                  setPartNumberOfMeasures (
                            unsigned int partNumberOfMeasures);

    const unsigned int    getPartNumberOfMeasures () const
                              { return fPartNumberOfMeasures; }

    const vector<rational>&
                          getPartMeasuresWholeNotesDurationsVector () const
                              { return fPartMeasuresWholeNotesDurationsVector; }

    // instrument name

    void                  setPartInstrumentName (
                            string partInstrumentName)
                              {
                                fPartInstrumentName =
                                  partInstrumentName;
                              }

    string                getPartInstrumentName () const
                              { return fPartInstrumentName; }

    void                  setPartInstrumentAbbreviation (
                            string partInstrumentAbbreviation)
                              {
                                fPartInstrumentAbbreviation =
                                  partInstrumentAbbreviation;
                              }

    string                getPartInstrumentAbbreviation () const
                              { return fPartInstrumentAbbreviation; }

    // harmonies staff and voice

    void                  setPartHarmoniesStaff (
                            S_msrStaff harmoniesStaff)
                              {
                                fPartHarmoniesStaff =
                                  harmoniesStaff;
                              }

    S_msrStaff            getPartHarmoniesStaff () const
                              { return fPartHarmoniesStaff; }

    void                  setPartHarmoniesVoice (
                            S_msrVoice harmoniesVoice)
                              {
                                fPartHarmoniesVoice =
                                  harmoniesVoice;
                              }

    S_msrVoice            getPartHarmoniesVoice () const
                              { return fPartHarmoniesVoice; }

    // figured bass staff and voice

    void                  setPartFiguredBassStaff (
                            S_msrStaff figuredBassStaff)
                              {
                                fPartFiguredBassStaff =
                                  figuredBassStaff;
                              }

    S_msrStaff            getPartFiguredBassStaff () const
                              { return fPartFiguredBassStaff; }

    void                  setPartFiguredBassVoice (
                            S_msrVoice figuredBassVoice)
                              {
                                fPartFiguredBassVoice =
                                  figuredBassVoice;
                              }

    S_msrVoice            getPartFiguredBassVoice () const
                              { return fPartFiguredBassVoice; }

    // staves map

    const map<int, S_msrStaff>&
                          getPartStavesMap () const
                              { return fPartStavesMap; }

    // staff position in measure

    void                  setPartCurrentPositionInMeasure (
                            int      inputLineNumber,
                            rational positionInMeasure);

    void                  incrementPartCurrentPositionInMeasure (
                            int      inputLineNumber,
                            rational duration);
    void                  decrementPartCurrentPositionInMeasure (
                            int      inputLineNumber,
                            rational duration);

    rational              getPartCurrentPositionInMeasure () const
                              { return fPartCurrentPositionInMeasure; }

    // part shortest note

    void                  setPartShortestNoteDuration (
                            rational duration);

    rational              getPartShortestNoteDuration () const
                              { return fPartShortestNoteDuration; }

    void                  setPartShortestNoteTupletFactor (
                            const msrTupletFactor& noteTupletFactor);

    const msrTupletFactor&
                          getPartShortestNoteTupletFactor () const
                              { return fPartShortestNoteTupletFactor; }

  public:

    // public services
    // ------------------------------------------------------

    // uplinks
    S_msrScore            fetchPartScoreUpLink () const;

    void                  assignSequentialNumbersToRegularVoicesInPart (
                            int inputLineNumber);

/* JMI
    void                  addAVoiceToStavesThatHaveNone (
                            int inputLineNumber);
*/

    // path shortest note

    void                  registerShortestNoteInPartIfRelevant (S_msrNote note);

    // part name display

    void                  appendPartNameDisplayToPart (
                            S_msrPartNameDisplay partNameDisplay);

    // part abbreviation display

    void                  appendPartAbbreviationDisplayToPart (
                            S_msrPartAbbreviationDisplay partAbbreviationDisplay);

    // measures

    void                  createMeasureAndAppendItToPart (
                            int    inputLineNumber,
                            string measureNumber,
                            msrMeasureImplicitKind
                                   measureImplicitKind);

    void                  setNextMeasureNumberInPart (
                            int    inputLineNumber,
                            string nextMeasureNumber);

    void                  registerOrdinalMeasureNumberWholeNotesDuration (
                            int      inputLineNumber,
                            int      measureOrdinalNumber,
                            rational wholeNotesDuration);

    // clef, key, time

    void                  appendClefToPart (S_msrClef clef);

    void                  appendKeyToPart (S_msrKey  key);

    void                  appendTimeToPart (S_msrTime time);
    void                  appendTimeToPartClone (S_msrTime time);

    // dal segno

    void                  insertHiddenMeasureAndBarlineInPartClone (
                            int      inputLineNumber,
                            rational positionInMeasure);

    // transpose

    void                  appendTransposeToPart (
                            S_msrTranspose transpose);

    // staff details

    void                  appendStaffDetailsToPart (
                            S_msrStaffDetails staffDetails);

    // tuplets JMI ???

    // barlines

    void                  appendBarlineToPart (S_msrBarline barline);

    // repeats

    void                  handleRepeatStartInPart (
                            int inputLineNumber);

    void                  handleRepeatEndInPart (
                            int    inputLineNumber,
                            string measureNumber,
                            int    repeatTimes);

    void                  handleRepeatEndingStartInPart (
                            int inputLineNumber);

    void                  handleRepeatEndingEndInPart (
                            int       inputLineNumber,
                            string    repeatEndingNumber, // may be "1, 2"
                            msrRepeatEndingKind
                                      repeatEndingKind);

    /* JMI ???
    void                  finalizeRepeatEndInPart (
                            int    inputLineNumber,
                            string measureNumber,
                            int    repeatTimes);
                            */

    void                  appendRepeatCloneToPart (
                            int         inputLineNumber,
                            S_msrRepeat repeatCLone);

    void                  appendRepeatEndingCloneToPart (
                            S_msrRepeatEnding repeatEndingCLone);

    // rest measures

    void                  createRestMeasuresInPart (
                            int inputLineNumber,
                            int restMeasuresNumber);

    void                  appendPendingRestMeasuresToPart (
                            int inputLineNumber);

    void                  addRestMeasuresToPart (
                            int    inputLineNumber,
                            string previousMeasureNumber,
                            int    restMeasuresNumber);

    void                  appendRestMeasuresCloneToPart (
                            int               inputLineNumber,
                            S_msrRestMeasures restMeasures);

    // measures repeats

    void                  createMeasuresRepeatFromItsFirstMeasuresInPart (
                            int inputLineNumber,
                            int measuresRepeatMeasuresNumber,
                            int measuresRepeatSlashesNumber);

    void                  appendPendingMeasuresRepeatToPart (
                            int inputLineNumber);

    void                  appendMeasuresRepeatCloneToPart (
                            int               inputLineNumber,
                            S_msrRestMeasures restMeasures);

    // staves

    S_msrStaff            addStaffToPartByItsNumber (
                            int          inputLineNumber,
                            msrStaffKind staffKind,
                            int          staffNumber);

    S_msrStaff            addPartLevelStaffToPart (
                            int          inputLineNumber,
                            msrStaffKind staffKind,
                            int          staffNumber);

    void                  addStaffToPartCloneByItsNumber (
                            S_msrStaff staff);

    S_msrStaff            fetchStaffFromPart (int staffNumber);

    // voices

    void                  registerVoiceInPartAllVoicesList (
                            S_msrVoice voice);

    // frames

    // JMI ???

    // harmonies

    S_msrVoice            createPartHarmonyVoice (
                            int    inputLineNumber,
                            string currentMeasureNumber);

    void                  appendHarmonyToPart (
                            S_msrVoice  harmonySupplierVoice,
                           S_msrHarmony harmony);

    void                  appendHarmonyToPartClone (
                            S_msrVoice   harmonySupplierVoice,
                            S_msrHarmony harmony);

    // figured bass

    S_msrVoice            createPartFiguredBassVoice (
                            int    inputLineNumber,
                            string currentMeasureNumber);

    void                  appendFiguredBassToPart (
                            S_msrVoice      figuredBassSupplierVoice,
                           S_msrFiguredBass figuredBass);

    void                  appendFiguredBassToPartClone (
                            S_msrVoice       figuredBassSupplierVoice,
                            S_msrFiguredBass figuredBass);

    // scordaturas

    void                  appendScordaturaToPart (
                            S_msrScordatura scordatura);

    // accordion registration

    void                  appendAccordionRegistrationToPart (
                            S_msrAccordionRegistration
                              accordionRegistration);

    // harp pedals tuning

    void                  appendHarpPedalsTuningToPart (
                            S_msrHarpPedalsTuning
                              harpPedalsTuning);

    // work around LilyPond issue #34

    void                  addSkipGraceNotesGroupBeforeAheadOfVoicesClonesIfNeeded (
                            S_msrVoice           graceNotesGroupOriginVoice,
                            S_msrGraceNotesGroup skipGraceNotesGroup);

/*
    void                  appendSkipGraceNotesToVoicesClones ( // JMI ???
                            S_msrVoice      graceNotesOriginVoice,
                            S_msrGraceNotes skipGraceNotes);

                            */

    // backup

    void                  handleBackupInPart (
                            int      inputLineNumber,
                            rational backupStepLength);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    void                  printPartMeasuresWholeNotesDurationsVector (
                            ostream&      os,
                            unsigned int fieldWidth) const;

    void                  print (ostream& os) const override;

    void                  printShort (ostream& os) const override;

    void                  printSummary (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks

    S_msrPartGroup        fPartPartGroupUpLink;

    // part ID and name

    string                fPartID; // native

    string                fPartMsrName;
                            // may be different than fPartID
                            // if renamed,
                            // coined in constructor

    string                fPartName; // from '<part-name/>'
    string                fPartNameDisplayText;

    string                fPartAbbreviation;
    string                fPartAbbreviationDisplayText;

    // part absolute number

    int                   fPartAbsoluteNumber;

    // part instrument names

    string                fPartInstrumentName;
    string                fPartInstrumentAbbreviation;

    // voices

    list<S_msrVoice>      fPartAllVoicesList;

    // measures

    string                fPartCurrentMeasureNumber;

    unsigned int          fPartNumberOfMeasures;

    vector<rational>      fPartMeasuresWholeNotesDurationsVector;

    // clef, key, time

    S_msrClef             fPartCurrentClef;

    S_msrKey              fPartCurrentKey;

    S_msrTime             fPartCurrentTime;

    // part shortest note

    // fPartShortestNoteDuration and fPartShortestNoteTupletFactor
    // are used to compute a number of divisions per quarter note
    // if needed, such as when generating MusicXML from MSR
    rational              fPartShortestNoteDuration;
    msrTupletFactor       fPartShortestNoteTupletFactor;

    // transpose

    S_msrTranspose        fPartCurrentTranspose;

    // harmonies

    S_msrStaff            fPartHarmoniesStaff;
    S_msrVoice            fPartHarmoniesVoice;

    // figured bass

    S_msrStaff            fPartFiguredBassStaff;
    S_msrVoice            fPartFiguredBassVoice;

    // the registered staves map

    map<int, S_msrStaff>  fPartStavesMap;

    // counter

    static int            gPartsCounter;

    // rest measures

    bool                  fPartContainsRestMeasures;

  public:

    // public work services
    // ------------------------------------------------------

    // measure number

    void                  setPartCurrentMeasureNumber (
                            string measureNumber)
                              {
                                fPartCurrentMeasureNumber =
                                  measureNumber;
                              }

    const string          getPartCurrentMeasureNumber () const
                              { return fPartCurrentMeasureNumber; }

    // clef, key, time

    S_msrClef             getPartCurrentClef () const
                              { return fPartCurrentClef; }

    S_msrKey              getPartCurrentKey  () const
                              { return fPartCurrentKey; }

    void                  setPartCurrentTime (S_msrTime time)
                              { fPartCurrentTime = time; }

    S_msrTime             getPartCurrentTime () const
                              { return fPartCurrentTime; }

    // transpose

    S_msrTranspose        getPartCurrentTranspose () const
                              { return fPartCurrentTranspose; }

    // staff details

    S_msrStaffDetails     getCurrentPartStaffDetails () const
                              { return fCurrentPartStaffDetails; }

    // finalization

    void                  finalizeLastAppendedMeasureInPart (
                            int inputLineNumber);

    void                  finalizePart (
                            int inputLineNumber);

    void                  finalizePartClone (
                            int inputLineNumber);

    void                  finalizePartAndAllItsMeasures (
                            int inputLineNumber);

  private:

    // private work services
    // ------------------------------------------------------

    // harmonies

    void                  setPartHarmonyVoiceForwardLink (
                            S_msrVoice voice)
                              { fPartHarmonyVoiceForwardLink = voice; }

    S_msrVoice            getPartHarmonyVoiceForwardLink () const
                              { return fPartHarmonyVoiceForwardLink; }

    void                  setHarmonyVoicePartBackwardLink (
                            S_msrVoice voice)
                              { fHarmonyVoicePartBackwardLink = voice; }

    S_msrVoice            getHarmonyVoicePartBackwardLink () const
                              { return fHarmonyVoicePartBackwardLink; }

    // figured bass

    void                  setPartFiguredBassVoiceForwardLink (
                            S_msrVoice voice)
                              { fPartFiguredBassVoiceForwardLink = voice; }

    S_msrVoice            getPartFiguredBassVoiceForwardLink () const
                              { return fPartFiguredBassVoiceForwardLink; }

    void                  setFiguredBassVoicePartBackwardLink (
                            S_msrVoice voice)
                              { fFiguredBassVoicePartBackwardLink = voice; }

    S_msrVoice            getFiguredBassVoicePartBackwardLink () const
                              { return fFiguredBassVoicePartBackwardLink; }

  private:

    // work fields
    // ------------------------------------------------------

    // staff details

    S_msrStaffDetails     fCurrentPartStaffDetails;

    // position in measure

    rational              fPartCurrentPositionInMeasure;

    // two-way links
    S_msrVoice            fPartHarmonyVoiceForwardLink;
    S_msrVoice            fHarmonyVoicePartBackwardLink;

    S_msrVoice            fPartFiguredBassVoiceForwardLink;
    S_msrVoice            fFiguredBassVoicePartBackwardLink;
};
typedef SMARTP<msrPart> S_msrPart;
EXP ostream& operator<< (ostream& os, const S_msrPart& elt);


}


#endif
