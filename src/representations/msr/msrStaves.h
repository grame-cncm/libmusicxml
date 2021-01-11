#ifndef ___msrStaves___
#define ___msrStaves___

#include "msrBars.h"
#include "msrClefs.h"
#include "msrInstruments.h"
#include "msrKeys.h"
#include "msrMeasures.h"
#include "msrMeasureElements.h"
#include "msrParts.h"
#include "msrPartGroups.h"
#include "msrPartNamesDisplays.h"
#include "msrRestMeasures.h"
#include "msrRepeats.h"
#include "msrScores.h"
#include "msrStavesDetails.h"
#include "msrTimes.h"
#include "msrTranspositions.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msrPartGroup;
typedef SMARTP<msrPartGroup> S_msrPartGroup;

class msrPart;
typedef SMARTP<msrPart> S_msrPart;

class msrStaff;
typedef SMARTP<msrStaff> S_msrStaff;

class msrRepeat;
typedef SMARTP<msrRepeat> S_msrRepeat;

class msrRepeatEnding;
typedef SMARTP<msrRepeatEnding> S_msrRepeatEnding;

//______________________________________________________________________________
class EXP msrVoiceStaffChange : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrVoiceStaffChange> create (
      int        inputLineNumber,
      S_msrStaff staffToChangeTo);

    SMARTP<msrVoiceStaffChange> createStaffChangeNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrVoiceStaffChange (
      int        inputLineNumber,
      S_msrStaff staffToChangeTo);

    virtual ~msrVoiceStaffChange ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrStaff            getStaffToChangeTo () const
                              { return fStaffToChangeTo; }

  public:

    // public services
    // ------------------------------------------------------

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

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrStaff            fStaffToChangeTo;
};
typedef SMARTP<msrVoiceStaffChange> S_msrVoiceStaffChange;
EXP ostream& operator<< (ostream& os, const S_msrVoiceStaffChange& elt);

//______________________________________________________________________________
class EXP msrStaff : public msrElement
{
  public:

    // constants
    // ------------------------------------------------------

    #define K_NO_STAFF_NUMBER                 -777

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStaff> create (
      int          inputLineNumber,
      msrStaffKind staffKind,
      int          staffNumber,
      S_msrPart    staffPartUpLink);

    SMARTP<msrStaff> createStaffNewbornClone (
      S_msrPart containingPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStaff (
      int          inputLineNumber,
      msrStaffKind staffKind,
      int          staffNumber,
      S_msrPart    staffPartUpLink);

    virtual ~msrStaff ();

  private:

    // private initialization
    // ------------------------------------------------------

    void                  initializeStaff ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLink

    S_msrPart             getStaffPartUpLink () const
                              { return fStaffPartUpLink; }

    // staff kind

    msrStaffKind          getStaffKind () const
                              { return fStaffKind; }

    // staff number and name

    int                   getStaffNumber () const
                              { return fStaffNumber; }

    string                getStaffName () const
                              { return fStaffName; }

    string                getStaffInstrumentName () const
                              { return fStaffInstrumentName; }

    string                getStaffInstrumentAbbreviation () const
                              { return fStaffInstrumentAbbreviation; }

    // voices

    int                   getStaffRegularVoicesCounter () const
                              { return fStaffRegularVoicesCounter; }

    // staff details

    S_msrStaffDetails     getCurrentStaffStaffDetails () const
                            { return fCurrentStaffStaffDetails; }

    // staff voices

    const map<int, S_msrVoice>&
                          getStaffVoiceNumbersToAllVoicesMap () const
                              { return fStaffVoiceNumbersToAllVoicesMap; }

    const map<int, S_msrVoice>&
                          getStaffVoiceNumbersToRegularVoicesMap () const
                              { return fStaffVoiceNumbersToRegularVoicesMap; }

    const list<S_msrVoice>&
                          getStaffAllVoicesVector () const
                              { return fStaffAllVoicesList; }

  public:

    // public services
    // ------------------------------------------------------

    // upLinks

    S_msrPartGroup        fetchStaffPartGroupUpLink () const;

    S_msrScore            fetchStaffScoreUpLink () const;

    // staff details

    void                  appendStaffDetailsToStaff (
                            S_msrStaffDetails staffDetails);

    // clef, key, time

    void                  appendClefToStaff (S_msrClef clef);

    void                  appendKeyToStaff (S_msrKey  key);

    void                  appendTimeToStaff (S_msrTime time);
    void                  appendTimeToStaffClone (S_msrTime time);

    // dal segno

    void                  insertHiddenMeasureAndBarlineInStaffClone (
                            int      inputLineNumber,
                            rational positionInMeasure);

    // transpose

    void                  appendTransposeToStaff (
                            S_msrTranspose transpose);

    // part name display

    void                  appendPartNameDisplayToStaff (
                            S_msrPartNameDisplay partNameDisplay);

    // part abbreviation display

    void                  appendPartAbbreviationDisplayToStaff (
                            S_msrPartAbbreviationDisplay partAbbreviationDisplay);

    // voices

    S_msrVoice            createRegularVoiceInStaffByItsNumber (
                            int          inputLineNumber,
                            int          voiceNumber,
                            string       currentMeasureNumber);

    void                  registerVoiceInStaff (
                            int        inputLineNumber,
                            S_msrVoice voice);

    void                  registerVoiceInStaffClone (
                            int        inputLineNumber,
                            S_msrVoice voice);

    S_msrVoice            fetchVoiceFromStaffByItsNumber (
                            int inputLineNumber,
                            int voiceNumber);

    S_msrVoice            fetchFirstRegularVoiceFromStaff (
                            int inputLineNumber);

    void                  assignSequentialNumbersToRegularVoicesInStaff (
                            int inputLineNumber);

    // measures

    void                  createMeasureAndAppendItToStaff (
                            int    inputLineNumber,
                            string measureNumber,
                            msrMeasureImplicitKind
                                   measureImplicitKind);

    void                  setNextMeasureNumberInStaff (
                            int    inputLineNumber,
                            string nextMeasureNumber);

    // repeats

    void                  handleRepeatStartInStaff (
                            int inputLineNumber);

    void                  handleRepeatEndInStaff (
                            int    inputLineNumber,
                            string measureNumber,
                            int    repeatTimes);

    void                  handleRepeatEndingStartInStaff (
                            int inputLineNumber);

    void                  handleRepeatEndingEndInStaff (
                            int       inputLineNumber,
                            string    repeatEndingNumber, // may be "1, 2"
                            msrRepeatEndingKind
                                      repeatEndingKind);

/* JMI
    void                  finalizeRepeatEndInStaff (
                            int    inputLineNumber,
                            string measureNumber,
                            int    repeatTimes);
    */

    void                  createMeasuresRepeatFromItsFirstMeasuresInStaff (
                            int inputLineNumber,
                            int measuresRepeatMeasuresNumber,
                            int measuresRepeatSlashesNumber);

    void                  appendPendingMeasuresRepeatToStaff (
                            int inputLineNumber);

    void                  createRestMeasuresInStaff (
                            int inputLineNumber,
                            int restMeasuresNumber);

    void                  appendPendingRestMeasuresToStaff (
                            int inputLineNumber);

    void                  addRestMeasuresToStaff (
                            int    inputLineNumber,
                            string previousMeasureNumber,
                            int    restMeasuresNumber);

    void                  appendRestMeasuresCloneToStaff (
                            int               inputLineNumber,
                            S_msrRestMeasures restMeasures);

    void                  createBeatsRepeatFromItsFirstMeasuresInStaff (
                            int inputLineNumber,
                            int beatsRepeatMeasuresNumber,
                            int beatsRepeatSlashesNumber);

    void                  appendRepeatCloneToStaff (
                            int         inputLineNumber,
                            S_msrRepeat repeatCLone);

    void                  appendRepeatEndingCloneToStaff (
                            S_msrRepeatEnding repeatEndingClone);

    // barlines

    void                  appendBarlineToStaff (S_msrBarline barline);

    // transpose

    void                  appendTransposeToAllStaffVoices ( // JMI
                            S_msrTranspose transpose);

    // part name display

    void                  appendPartNameDisplayToAllStaffVoices ( // JMI
                            S_msrPartNameDisplay partNameDisplay);

    // part abbreviation display

    void                  appendPartAbbreviationDisplayToAllStaffVoices ( // JMI
                            S_msrPartAbbreviationDisplay partAbbreviationDisplay);

    // scordaturas

    void                  appendScordaturaToStaff (
                            S_msrScordatura scordatura);

    // accordion registration

    void                  appendAccordionRegistrationToStaff (
                            S_msrAccordionRegistration
                              accordionRegistration);

    // harp pedals tuning

    void                  appendHarpPedalsTuningToStaff (
                            S_msrHarpPedalsTuning
                              harpPedalsTuning);

    // strings

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    // staff number

    string                staffNumberAsString () const;

    void                  print (ostream& os) const override;

    void                  printShort (ostream& os) const override;

    void                  printSummary (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks

    S_msrPart             fStaffPartUpLink;

    // staff name
    string                fStaffName;

    // staff kind
    msrStaffKind          fStaffKind;

    // staff number
    int                   fStaffNumber;

    // staff instrument name
    string                fStaffInstrumentName;
    string                fStaffInstrumentAbbreviation;

    /*
      There are 'regular', harmonyes and figured voices.
      A voice in and 'All' or 'Any' data structure can be any of these.
    */

    // the staff regular (i.e. not harmonies nor figured bass) voices
    static int            gStaffMaxRegularVoices;

    // the mapping of all the voices in the staff,
    // including harmonies and figured bass voices
    map<int, S_msrVoice>  fStaffVoiceNumbersToAllVoicesMap;

    // the mapping of voice numbers to regular voices
    map<int, S_msrVoice>  fStaffVoiceNumbersToRegularVoicesMap;

    // we need to handle the regular voice specifically
    // to assign them sequencing numbers from 1 to gMaxStaffVoices,
    // needed to set the beams orientation (up or down)
    int                   fStaffRegularVoicesCounter;

    // harmonies and figured basses should be placed
    // in the first regular voice of the staff, hence:
    list<S_msrVoice>      fStaffRegularVoicesList;

    // we need to sort the voices by increasing voice numbers,
    // but with harmony voices right before the corresponding regular voices
    list<S_msrVoice>      fStaffAllVoicesList;

    // staff details

    S_msrStaffDetails     fCurrentStaffStaffDetails;

    // rest measures

    bool                  fStaffContainsRestMeasures;

  public:

    // public work services
    // ------------------------------------------------------

    // clef, key, time

    void                  setStaffCurrentClef (S_msrClef clef);

    S_msrClef             getStaffCurrentClef () const
                              { return fStaffCurrentClef; }

    void                  setStaffCurrentKey (S_msrKey key);

    S_msrKey              getStaffCurrentKey  () const
                              { return fStaffCurrentKey; }

    void                  setStaffCurrentTime (S_msrTime time);

    S_msrTime             getStaffCurrentTime () const
                              { return fStaffCurrentTime; }

    // finalization

    void                  finalizeLastAppendedMeasureInStaff (
                            int inputLineNumber);

    void                  finalizeStaff (
                            int inputLineNumber);

  private:

    // private work services
    // ------------------------------------------------------

    // voice registration

    void                  registerVoiceInMapsAndLists (
                            S_msrVoice voice);

    void                  registerVoiceInStaffAllVoicesList (
                            S_msrVoice voice);

    void                  registerVoiceByItsNumber (
                            int        inputLineNumber,
                            S_msrVoice voice);

    void                  registerRegularVoiceByItsNumber (
                            S_msrVoice regularVoice,
                            int        voiceNumber);

    void                  registerHarmonyVoiceByItsNumber (
                            S_msrVoice harmonyVoice,
                            int        voiceNumber);

    void                  registerFiguredBassVoiceByItsNumber (
                            S_msrVoice figuredBassVoice,
                            int        voiceNumber);

    void                  registerPartLevelVoiceInStaff ( // JMI rename for harmonies and figured bass ???
                            int        inputLineNumber,
                            S_msrVoice voice);

    // transpose

    S_msrTranspose        getStaffCurrentTranspose () const
                              { return fStaffCurrentTranspose; }

    // voices ordering in staves

    static bool           compareVoicesByIncreasingNumber (
                            const S_msrVoice& first,
                            const S_msrVoice& second);

    static bool           compareVoicesToHaveHarmoniesAboveCorrespondingVoice (
                            const S_msrVoice& first,
                            const S_msrVoice& second);

    static bool           compareVoicesToHaveFiguredBassesBelowCorrespondingVoice (
                            const S_msrVoice& first,
                            const S_msrVoice& second);

  private:

    // work fields
    // ------------------------------------------------------

    // clef, key, time

    S_msrClef             fStaffCurrentClef;

    S_msrKey              fStaffCurrentKey;

    S_msrTime             fStaffCurrentTime;

    // transpose

    S_msrTranspose        fStaffCurrentTranspose;
};
typedef SMARTP<msrStaff> S_msrStaff;
EXP ostream& operator<< (ostream& os, const S_msrStaff& elt);


}


#endif
