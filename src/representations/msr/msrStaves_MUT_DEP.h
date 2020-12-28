/*
  This file is to be included only by msr_MUTUAL_DEPENDENCIES.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___msrStaves_MUT_DEP___
#define ___msrStaves_MUT_DEP___

namespace MusicXML2
{

//______________________________________________________________________________
class msrStaff;
typedef SMARTP<msrStaff> S_msrStaff;

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

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    virtual void          print (ostream& os) const override;

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
                          getStaffVoiceNumberToAnyVoiceMap () const
                              { return fStaffVoiceNumberToAnyVoiceMap; }

    const map<int, S_msrVoice>&
                          getStaffVoiceNumberToVoicesMap () const
                              { return fStaffVoiceNumberToVoicesMap; }

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

    S_msrVoice            createVoiceInStaffByItsNumber (
                            int                    inputLineNumber,
                            msrVoiceKind voiceKind,
                            int                    voiceNumber,
                            string                 currentMeasureNumber);

    void                  registerVoiceByItsNumber (
                            int        inputLineNumber,
                            int        voiceNumber,
                            S_msrVoice voice);

    void                  registerVoiceInStaff (
                            int        inputLineNumber,
                            S_msrVoice voice);

    void                  registerRegularVoiceByItsNumber (
                            int        voiceNumber,
                            S_msrVoice voice);

    void                  registerPartLevelVoiceInStaff (
                            int        inputLineNumber,
                            S_msrVoice voice);

    void                  registerVoiceInStaffClone (
                            int        inputLineNumber,
                            S_msrVoice voice);

/* JMI
    void                  registerVoiceInAllVoicesList (
                            int        voiceNumber,
                            S_msrVoice voice);
*/

    S_msrVoice            fetchVoiceFromStaffByItsNumber (
                            int inputLineNumber,
                            int voiceNumber);

    S_msrVoice            fetchFirstRegularVoiceFromStaff (
                            int inputLineNumber);

    void                  assignSequentialNumbersToRegularVoicesInStaff (
                            int inputLineNumber);

/* JMI
    void                  addAVoiceToStaffIfItHasNone (
                            int inputLineNumber);
*/

    // measures

    void                  createMeasureAndAppendItToStaff (
                            int    inputLineNumber,
                            string measureNumber,
                            msrMeasure::msrMeasureImplicitKind
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
                            msrRepeatEnding::msrRepeatEndingKind
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

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    // staff number

    string                staffNumberAsString () const;

    virtual void          print (ostream& os) const override;

    virtual void          printShort (ostream& os) const override;

    virtual void          printSummary (ostream& os) const override;

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
    map<int, S_msrVoice>  fStaffVoiceNumberToAnyVoiceMap;

    // the mapping of voice numbers to voices
    map<int, S_msrVoice>  fStaffVoiceNumberToVoicesMap;

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

    // transpose

    S_msrTranspose        getStaffCurrentTranspose () const
                              { return fStaffCurrentTranspose; }

    void                  registerVoiceInStaffAllVoicesList (
                            S_msrVoice voice);

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
