/*
  This file is to be included only by msrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

//______________________________________________________________________________
class msrMeasure : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrMeasureKind {
      kMeasureKindUnknown,
      kMeasureKindRegular,
      kMeasureKindAnacrusis,
      kMeasureKindIncompleteStandalone,
      kMeasureKindIncompleteLastInRepeatCommonPart,
      kMeasureKindIncompleteLastInRepeatHookedEnding,
      kMeasureKindIncompleteLastInRepeatHooklessEnding,
      kMeasureKindIncompleteNextMeasureAfterCommonPart,
      kMeasureKindIncompleteNextMeasureAfterHookedEnding,
      kMeasureKindIncompleteNextMeasureAfterHooklessEnding,
      kMeasureKindOvercomplete,
      kMeasureKindCadenza,
      kMeasureKindMusicallyEmpty};

    static string measureKindAsString (
      msrMeasureKind measureKind);

    enum msrMeasureImplicitKind {
      kMeasureImplicitKindYes,
      kMeasureImplicitKindNo };

    static string measureImplicitKindAsString (
      msrMeasureImplicitKind measureImplicitKind);

    enum msrMeasureFirstInSegmentKind {
      kMeasureFirstInSegmentKindUnknown,
      kMeasureFirstInSegmentKindYes,
      kMeasureFirstInSegmentKindNo };

    static string measureFirstInSegmentKindAsString (
      msrMeasureFirstInSegmentKind measureFirstInSegmentKind);

    enum msrMeasuresRepeatContextKind {
      kMeasuresRepeatContextKindUnknown,
      kMeasuresRepeatContextKindNone,
      kMeasuresRepeatContextKindCommonPartLastMeasure,
      kMeasuresRepeatContextKindHookedEndingLastMeasure,
      kMeasuresRepeatContextKindHooklessEndingLastMeasure,
      kMeasuresRepeatContextKindNextMeasureAfterCommonPart,
      kMeasuresRepeatContextKindNextMeasureAfterHookedEnding,
      kMeasuresRepeatContextKindNextMeasureAfterHooklessEnding };

    static string measuresRepeatContextKindAsString (
      msrMeasuresRepeatContextKind measuresRepeatContextKind);

    enum msrMeasureEndRegularKind {
      kMeasureEndRegularKindUnknown,
      kMeasureEndRegularKindYes,
      kMeasureEndRegularKindNo };

    static string measureEndRegularKindAsString (
      msrMeasureEndRegularKind measureEndRegularKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasure> create (
      int          inputLineNumber,
      string       measureNumber,
      S_msrSegment measureSegmentUpLink);

    SMARTP<msrMeasure> createMeasureNewbornClone (
      S_msrSegment containingSegment);

    SMARTP<msrMeasure> createMeasureDeepCopy (
      S_msrSegment containingSegment);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMeasure (
      int          inputLineNumber,
      string       measureNumber,
      S_msrSegment measureSegmentUpLink);

    virtual ~msrMeasure ();

  private:

    // initialization
    // ------------------------------------------------------

    void                  initializeMeasure ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks

    S_msrSegment          getMeasureSegmentUpLink () const
                              { return fMeasureSegmentUpLink; }

    // measure numbers

    void                  setMeasureNumber (string measureNumber)
                              { fMeasureNumber = measureNumber; }

    string                getMeasureNumber () const
                              { return fMeasureNumber; }

    void                  setMeasurePuristNumber (
                            int measurePuristNumber);

    int                   getMeasurePuristNumber () const
                              { return fMeasurePuristNumber; }

    void                  setNextMeasureNumber (string nextMeasureNumber);

    string                getNextMeasureNumber () const
                              { return fNextMeasureNumber; }

    void                  setMeasureFirstInVoice ();

    bool                  getMeasureFirstInVoice () const
                              { return fMeasureFirstInVoice; }

    // measure lengthes, in whole notes

    void                  setFullMeasureWholeNotesDuration (
                            rational wholeNotes)
                              { fFullMeasureWholeNotesDuration = wholeNotes; }

    rational              getFullMeasureWholeNotesDuration () const
                              {  return fFullMeasureWholeNotesDuration; }

    void                  setCurrentMeasureWholeNotesDuration (
                            int      inputLineNumber,
                            rational wholeNotes);

    rational              getCurrentMeasureWholeNotesDuration () const
                              { return fCurrentMeasureWholeNotesDuration; }

    // measure kind

    void                  setMeasureKind (
                            msrMeasureKind measureKind);

    msrMeasureKind
                          getMeasureKind () const
                              { return fMeasureKind; }

    // measure implicit kind

    void                  setMeasureImplicitKind (
                            msrMeasureImplicitKind measureImplicitKind)
                              {
                                fMeasureImplicitKind =
                                  measureImplicitKind;
                              }

    msrMeasureImplicitKind
                          getMeasureImplicitKind () const
                              { return fMeasureImplicitKind; }


    // measure 'first in segment' kind

    void                  setMeasureFirstInSegmentKind (
                            msrMeasureFirstInSegmentKind
                              measureFirstInSegmentKind)
                              {
                                fMeasureFirstInSegmentKind =
                                  measureFirstInSegmentKind;
                              }

    msrMeasureFirstInSegmentKind
                          getMeasureFirstInSegmentKind () const
                              { return fMeasureFirstInSegmentKind; }

    // single-measure rest?


    void                  setMeasureIsAFullMeasureRest ()
                              { fMeasureIsAFullMeasureRest = true; }

    bool                  getMeasureIsAFullMeasureRest () const
                             { return fMeasureIsAFullMeasureRest; }

    // measure longest note

    S_msrNote             getMeasureLongestNote () const
                              { return fMeasureLongestNote; }

    // chords handling

    S_msrNote             getMeasureLastHandledNote () const
                              { return fMeasureLastHandledNote; }

    // elements list

    const list<S_msrMeasureElement>&
                          getMeasureElementsList () const
                              { return fMeasureElementsList; }

    bool                  getMeasureContainsMusic () const
                              { return fMeasureContainsMusic; }

    // regular measure ends detection

    void                  setMeasureEndRegularKind (
                            msrMeasureEndRegularKind measureEndRegularKind);

    msrMeasureEndRegularKind
                          getMeasureEndRegularKind () const
                              { return fMeasureEndRegularKind; }

    // repeat context

    void                  setMeasuresRepeatContextKind (
                            msrMeasuresRepeatContextKind measuresRepeatContextKind);

    msrMeasuresRepeatContextKind
                          getMeasuresRepeatContextKind () const
                              { return fMeasuresRepeatContextKind; }

  public:

    // public services
    // ------------------------------------------------------

    // upLinks

    S_msrPart             fetchMeasurePartUpLink () const;

    S_msrVoice            fetchMeasureVoiceUpLink () const;

    // lengthes

    string                fullMeasureWholeNotesDurationAsMSRString ();

    string                currentMeasureWholeNotesDurationAsMSRString ();

    // backup and padding

    S_msrNote             createPaddingNoteForVoice (
                            int        inputLineNumber,
                            rational   duration,
                            S_msrVoice voice);

    void                  padUpToPositionInMeasureInMeasure (
                            int      inputLineNumber,
                            rational wholeNotes);

    void                  backupByWholeNotesStepLengthInMeasure (
                            int      inputLineNumber,
                            rational backupTargetPositionInMeasure);

    void                  appendPaddingNoteToMeasure (
                            int      inputLineNumber,
                            rational forwardStepLength);

    // rest measures

    bool                  measureIsARestMeasure () const;

    // clefs

    void                  appendClefToMeasure (S_msrClef clef);

    // keys

    void                  appendKeyToMeasure (S_msrKey key);

    // times

    void                  appendTimeToMeasure (S_msrTime time);

    void                  setFullMeasureWholeNotesDurationFromTime (
                            S_msrTime time);

    void                  appendTimeToMeasureClone (S_msrTime time);

    // transpose

    void                  appendTransposeToMeasure (
                            S_msrTranspose transpose);

    // part name display

    void                  appendPartNameDisplayToMeasure (
                            S_msrPartNameDisplay partNameDisplay);

    // part abbreviation display

    void                  appendPartAbbreviationDisplayToMeasure (
                            S_msrPartAbbreviationDisplay partAbbreviationDisplay);

    // staff details

    void                  appendStaffDetailsToMeasure (
                            S_msrStaffDetails staffDetails);

    // bar number checks

    void                  appendBarNumberCheckToMeasure (
                            S_msrBarNumberCheck barNumberCheck);

    // breaks

    void                  appendLineBreakToMeasure (S_msrLineBreak lineBreak);
    void                  appendPageBreakToMeasure (S_msrPageBreak pageBreak);

    // tempo

    void                  appendTempoToMeasure (S_msrTempo tempo);

    // rehearsals

    void                  appendRehearsalToMeasure (
                            S_msrRehearsal rehearsal);

    // octave shifts

    void                  appendOctaveShiftToMeasure (
                            S_msrOctaveShift octaveShift);

    // scordaturas

    void                  appendScordaturaToMeasure (
                            S_msrScordatura scordatura);

    // accordion registration

    void                  appendAccordionRegistrationToMeasure (
                            S_msrAccordionRegistration
                              accordionRegistration);

    // harp pedals tuning

    void                  appendHarpPedalsTuningToMeasure (
                            S_msrHarpPedalsTuning
                              harpPedalsTuning);

    // bar lines

    void                  appendBarlineToMeasure (S_msrBarline barline);
    void                  prependBarlineToMeasure (S_msrBarline barline);

    // bar checks

    void                  appendBarCheckToMeasure (S_msrBarCheck barCheck);

    // staff change

    void                  appendVoiceStaffChangeToMeasure (
                            S_msrVoiceStaffChange voiceStaffChange);

    // notes

    void                  appendNoteToMeasure (
                            S_msrNote note,
                            rational  staffCurrentPositionInMeasure);

    void                  appendNoteOrPaddingToMeasure (
                            S_msrNote note);

    void                  accountForTupletMemberNoteDurationInMeasure (
                            S_msrNote note);

    void                  appendPaddingNoteAtTheEndOfMeasure (S_msrNote note);

    void                  appendNoteToMeasureClone (S_msrNote note);

    // tremolos

    void                  appendDoubleTremoloToMeasure (
                            S_msrDoubleTremolo doubleTremolo);

     // chords

    void                  appendChordToMeasure (
                            S_msrChord chord);

    // tuplets

    void                  appendTupletToMeasure (
                            S_msrTuplet tuplet);

    // harmonies

    void                  appendHarmonyToMeasure (
                            S_msrHarmony harmony);

    void                  appendHarmonyToMeasureClone (
                            S_msrHarmony harmony);

    // frames

    void                  appendFrameToMeasure (
                            S_msrFrame frame);

    void                  appendFrameToMeasureClone (
                            S_msrFrame frame);

    // figured bass

    void                  appendFiguredBassToMeasure (
                            S_msrFiguredBass figuredBass);

    void                  appendFiguredBassToMeasureClone (
                            S_msrFiguredBass figuredBass);

    // grace notes

    /* JMI
    void                  addGraceNotesGroupAheadOfMeasure (
                            S_msrGraceNotesGroup graceNotesGroup);

    void                  appendGraceNotesToMeasure (
                            S_msrGraceNotes graceNotes);

    void                  appendAfterGraceNotesToMeasure (
                            S_msrAfterGraceNotes afterGraceNotes);

    void                  prependAfterGraceNotesToMeasure (
                            S_msrAfterGraceNotes afterGraceNotes);
                            */

    // segno

    void                  appendSegnoToMeasure (S_msrSegno segno);

    // coda

    void                  appendCodaToMeasure (S_msrCoda coda);

    // eyeglasses

    void                  appendEyeGlassesToMeasure (
                            S_msrEyeGlasses eyeGlasses);

    // pedal

    void                  appendPedalToMeasure (S_msrPedal pedal);

    // damp

    void                  appendDampToMeasure (S_msrDamp damp);

    // damp all

    void                  appendDampAllToMeasure (S_msrDampAll dampAll);

    // other elements

    void                  prependOtherElementToMeasure (
                            S_msrMeasureElement elem);
    void                  appendOtherElementToMeasure (
                            S_msrMeasureElement elem);

    // last element of measure

    S_msrElement          getLastElementOfMeasure () const
                              { return fMeasureElementsList.back (); }

     /* JMI
    S_msrElement          removeLastElementFromMeasure (
                            int inputLineNumber);
*/

    // removing elements from measure

    void                  removeNoteFromMeasure (
                            int       inputLineNumber,
                            S_msrNote note);

    void                  removeElementFromMeasure (
                            int          inputLineNumber,
                            S_msrElement elem);

    // finalization

    void                  determineMeasureKindAndPuristNumber (
                            int     inputLineNumber,
                            msrMeasuresRepeatContextKind
                                    measuresRepeatContextKind);

    void                  padUpToPositionInMeasure (
                            int      inputLineNumber,
                            rational positionInMeasureToPadUpTo);

    void                  padUpToPositionAtTheEndOfTheMeasure (
                            int      inputLineNumber,
                            rational positionInMeasureToPadUpTo);

    void                  finalizeMeasure (
                            int                          inputLineNumber,
                            msrMeasuresRepeatContextKind measuresRepeatContextKind,
                            string                       context);

    void                  finalizeRegularMeasure (
                            int                          inputLineNumber,
                            msrMeasuresRepeatContextKind measuresRepeatContextKind,
                            string                       context);

    void                  finalizeHarmonyMeasure (
                            int                          inputLineNumber,
                            msrMeasuresRepeatContextKind measuresRepeatContextKind,
                            string                       context);

    void                  finalizeFiguredBassMeasure (
                            int                          inputLineNumber,
                            msrMeasuresRepeatContextKind measuresRepeatContextKind,
                            string                       context);

    void                  finalizeMeasureClone (
                            int          inputLineNumber,
                            S_msrMeasure originalMeasure,
                            S_msrVoice   voiceClone);

  private:

    // private services
    // ------------------------------------------------------

    void                  appendElementToMeasure (
                            S_msrMeasureElement elem);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                measureKindAsString () const;

    string                asShortString () const;
    string                asString () const;

    void                  displayMeasure (
                            int    inputLineNumber,
                            string context);

    virtual void          print (ostream& os);

    virtual void          shortPrint (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // upLinks

    S_msrSegment          fMeasureSegmentUpLink;

    // measure lengthes, in whole notes

    rational              fFullMeasureWholeNotesDuration;
                            // meaningfull only
                            // when there is a time signature,
                            // but not for cadenzas

    rational              fCurrentMeasureWholeNotesDuration;
                            // this increases when musical elements
                            // are appended to the measure

    // measure numbers, shared by newborn clones and deep copies

    string                fMeasureNumber;
    string                fNextMeasureNumber;

    // first measure in voice?

    bool                  fMeasureFirstInVoice;

    // measure longest note

    S_msrNote             fMeasureLongestNote;

    // measure kind

    msrMeasureKind        fMeasureKind;

    // measure implicit kind

    msrMeasureImplicitKind
                          fMeasureImplicitKind;

    // measure 'first in segment' kind

    msrMeasureFirstInSegmentKind
                          fMeasureFirstInSegmentKind;

    // single-measure rest?

    bool                  fMeasureIsAFullMeasureRest;

    // chords handling

    S_msrNote             fMeasureLastHandledNote;

    // elements

    list<S_msrMeasureElement>
                          fMeasureElementsList;

    bool                  fMeasureContainsMusic;

    // purist measure number, forcing anacruses to start at '0' if it's not the case
    // and not shared among repeats components
    int                   fMeasurePuristNumber;

    // debug number, unique for every msrMeasure instance
    static int            gMeasureDebugNumber;
    int                   fMeasureDebugNumber;

    // regular measure ends detection
    msrMeasureEndRegularKind
                          fMeasureEndRegularKind;

    // repeat context
    msrMeasuresRepeatContextKind
                          fMeasuresRepeatContextKind;

  public:

    // public work services
    // ------------------------------------------------------

  private:

    // private work services
    // ------------------------------------------------------

    void                  handleHarmoniesInHarmonyMeasureFinalization (
                            int    inputLineNumber,
                            string context);

    void                  insertElementInMeasureBeforeIterator (
                            int                                 inputLineNumber,
                            list<S_msrMeasureElement>::iterator iter,
                            S_msrMeasureElement                 elem);

    void                  appendElementAtTheEndOfMeasure (
                            S_msrMeasureElement elem);

    void                  printMeasurePendingMeasureElementsList ();

  private:

    // work fields
    // ------------------------------------------------------

    // pending measure elements, which may have to be delayed
    // because of <backup />

    list<S_msrMeasureElement>
                          fMeasurePendingMeasureElementsList;

    // measure finalization
    bool                  fMeasureHasBeenFinalized;
    bool                  fMeasureKindAndPuristNumberHaveBeenDetermined;
};
typedef SMARTP<msrMeasure> S_msrMeasure;
EXP ostream& operator<< (ostream& os, const S_msrMeasure& elt);

