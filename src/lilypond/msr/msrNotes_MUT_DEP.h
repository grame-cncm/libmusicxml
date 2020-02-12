/*
  This file is to be included only by msrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

//______________________________________________________________________________
class msrNote : public msrTupletElement
{
  public:

    // constants
    // ------------------------------------------------------

    #define K_NO_OCTAVE -1

    // data types
    // ------------------------------------------------------

    enum msrNoteKind {
      k_NoNoteKind,
      kRestNote, kSkipNote,
      kUnpitchedNote,
      kRegularNote,
      kDoubleTremoloMemberNote,
      kGraceNote,
      kChordMemberNote,
      kGraceChordMemberNote,
      kTupletMemberNote,
      kGraceTupletMemberNote,
      kTupletMemberUnpitchedNote };

    static string noteKindAsString (
      msrNoteKind noteKind);

    enum msrNoteAccidentalKind {
      kNoteAccidentalNone,

      kNoteAccidentalSharp, kNoteAccidentalNatural,
      kNoteAccidentalFlat, kNoteAccidentalDoubleSharp,
      kNoteAccidentalSharpSharp,
      kNoteAccidentalFlatFlat, kNoteAccidentalNaturalSharp,
      kNoteAccidentalNaturalFlat, kNoteAccidentalQuarterFlat,
      kNoteAccidentalQuarterSharp,kNoteAccidentalThreeQuartersFlat,
      kNoteAccidentalThreeQuartersSharp,

      kNoteAccidentalSharpDown, kNoteAccidentalSharpUp,
      kNoteAccidentalNaturalDown, kNoteAccidentalNaturalUp,
      kNoteAccidentalFlatDown, kNoteAccidentalFlatUp,
      kNoteAccidentalTripleSharp, kNoteAccidentalTripleFlat,
      kNoteAccidentalSlashQuarterSharp, kNoteAccidentalSlashSharp,
      kNoteAccidentalSlashFlat,kNoteAccidentaldoubleSlashFlat,
      kNoteAccidentalSharp_1, kNoteAccidentalSharp_2,
      kNoteAccidentalSharp_3,  kNoteAccidentalSharp_5,
      kNoteAccidentalFlat_1, kNoteAccidentalFlat_2,
      kNoteAccidentalFlat_3, kNoteAccidentalFlat_4,
      kNoteAccidentalSori, kNoteAccidentalKoron };

    static string noteAccidentalKindAsString (
      msrNoteAccidentalKind noteAccidentalKind);

    enum msrNoteEditorialAccidentalKind {
      kNoteEditorialAccidentalYes, kNoteEditorialAccidentalNo };

    static string noteEditorialAccidentalKindAsString (
      msrNoteEditorialAccidentalKind noteEditorialAccidentalKind);

    enum msrNoteCautionaryAccidentalKind {
      kNoteCautionaryAccidentalYes, kNoteCautionaryAccidentalNo };

    static string noteCautionaryAccidentalKindAsString (
      msrNoteCautionaryAccidentalKind noteCautionaryAccidentalKind);

    enum msrNoteHeadKind {
      kNoteHeadSlash,
      kNoteHeadTriangle, kNoteHeadDiamond, kNoteHeadSquare,
      kNoteHeadCross,kNoteHeadX,
      kNoteHeadCircleX, kNoteHeadInvertedTriangle,
      kNoteHeadArrowDown, kNoteHeadArrowUp,
      kNoteHeadSlashed, kNoteHeadBackSlashed,
      kNoteHeadNormal, kNoteHeadCluster,
      kNoteHeadCircleDot,
      kNoteHeadLeftTriangle,
      kNoteHeadRectangle,
      kNoteHeadNone,
      kNoteHeadDo, kNoteHeadRe, kNoteHeadMi, kNoteHeadFa, kNoteHeadFaUp,
      kNoteHeadSo, kNoteHeadLa, kNoteHeadTi };

    static string noteHeadKindAsString (
      msrNoteHeadKind noteHeadKind);

    enum msrNoteHeadFilledKind {
      kNoteHeadFilledYes, kNoteHeadFilledNo };

    static string noteHeadFilledKindAsString (
      msrNoteHeadFilledKind noteHeadFilledKind);

    enum msrNoteHeadParenthesesKind {
      kNoteHeadParenthesesYes, kNoteHeadParenthesesNo };

    static string noteHeadParenthesesKindAsString (
      msrNoteHeadParenthesesKind noteHeadParenthesesKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrNote> create (
      int                        inputLineNumber,
      string                     noteMeasureNumber,

      msrNoteKind                noteKind,

      msrQuarterTonesPitchKind   noteQuarterTonesPitchKind,

      rational                   noteSoundingWholeNotes,
      rational                   noteDisplayWholeNotes,

      int                        noteDotsNumber,

      msrDurationKind            noteGraphicDurationKind,

      int                        noteOctave,

      msrQuarterTonesPitchKind   noteQuarterTonesDisplayPitchKind,
      int                        noteDisplayOctave,

      bool                       noteIsARest,
      bool                       noteIsUnpitched,

      bool                       noteIsACueNote,
      bool                       noteIsAGraceNote,

      msrPrintObjectKind         notePrintObjectKind,

      msrNoteHeadKind            noteHeadKind,
      msrNoteHeadFilledKind      noteHeadFilledKind,
      msrNoteHeadParenthesesKind noteHeadParenthesesKind);

    SMARTP<msrNote> createNoteNewbornClone (
      S_msrPart containingPart);

    SMARTP<msrNote> createNoteDeepCopy (
      S_msrVoice containingVoice);

    // creation from xml2Msr
    // ------------------------------------------------------

    static SMARTP<msrNote> createRestNote (
      int       inputLineNumber,
      string    noteMeasureNumber,
      rational  soundingWholeNotes,
      rational  displayWholeNotes,
      int       dotsNumber,
      int       staffNumber,
      int       voiceNumber);

    static SMARTP<msrNote> createSkipNote (
      int       inputLineNumber,
      string    noteMeasureNumber,
      rational  soundingWholeNotes,
      rational  displayWholeNotes,
      int       dotsNumber,
      int       staffNumber,
      int       voiceNumber);

    // creation from the options
    // ------------------------------------------------------

    static SMARTP<msrNote> createNoteFromSemiTonesPitchAndOctave (
      int                          inputLineNumber,
      S_msrSemiTonesPitchAndOctave semiTonesPitchAndOctave);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrNote (
      int                        inputLineNumber,
      string                     noteMeasureNumber,

      msrNoteKind                noteKind,

      msrQuarterTonesPitchKind   noteQuarterTonesPitchKind,

      rational                   noteSoundingWholeNotes,
      rational                   noteDisplayWholeNotes,

      int                        noteDotsNumber,

      msrDurationKind            noteGraphicDurationKind,

      int                        noteOctave,

      msrQuarterTonesPitchKind   noteQuarterTonesDisplayPitchKind,
      int                        noteDisplayOctave,

      bool                       noteIsARest,
      bool                       noteIsUnpitched,

      bool                       noteIsACueNote,
      bool                       noteIsAGraceNote,

      msrPrintObjectKind         notePrintObjectKind,

      msrNoteHeadKind            noteHeadKind,
      msrNoteHeadFilledKind      noteHeadFilledKind,
      msrNoteHeadParenthesesKind noteHeadParenthesesKind);

    virtual ~msrNote ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeNote ();

  public:

    // set and get
    // ------------------------------------------------------

    // measure upLink
    void                  setNoteMeasureUpLink (
                            const S_msrMeasure& measure)
                              { fNoteMeasureUpLink = measure; }

    S_msrMeasure          getNoteMeasureUpLink () const
                              { return fNoteMeasureUpLink; }

    // chord upLink
    void                  setNoteChordUpLink (
                            const S_msrChord& chord)
                              { fNoteChordUpLink = chord; }

    S_msrChord            getNoteChordUpLink () const
                              { return fNoteChordUpLink; }

    // grace notes group upLink
    void                  setNoteGraceNotesGroupUpLink (
                            const S_msrGraceNotesGroup& graceNotesGroup)
                              { fNoteGraceNotesGroupUpLink = graceNotesGroup; }

    S_msrGraceNotesGroup  getNoteGraceNotesGroupUpLink () const
                              { return fNoteGraceNotesGroupUpLink; }

    // tuplet upLink
    void                  setNoteTupletUpLink (
                            const S_msrTuplet& tuplet)
                              { fNoteTupletUpLink = tuplet; }

    S_msrTuplet           getNoteTupletUpLink () const
                              { return fNoteTupletUpLink; }

    // note kind

    void                  setNoteKind (msrNoteKind noteKind);

    msrNoteKind           getNoteKind () const
                              { return fNoteKind; }

    // note pitch

    msrQuarterTonesPitchKind
                          getNoteQuarterTonesPitchKind () const
                              { return fNoteQuarterTonesPitchKind; }

    // note print kind

    void                  setNotePrintObjectKind (
                            msrPrintObjectKind notePrintObjectKind)
                              { fNotePrintObjectKind = notePrintObjectKind; }
    msrPrintObjectKind    getNotePrintObjectKind () const
                              { return fNotePrintObjectKind; }

    // note head

    void                  setNoteHeadKind (
                            msrNoteHeadKind noteHeadKind)
                              { fNoteHeadKind = noteHeadKind; }

    msrNoteHeadKind       getNoteHeadKind () const
                              { return fNoteHeadKind; }

    void                  setNoteHeadFilledKind (
                            msrNoteHeadFilledKind
                              noteHeadFilledKind)
                              {
                                fNoteHeadFilledKind =
                                  noteHeadFilledKind;
                              }

    msrNoteHeadFilledKind getNoteHeadFilledKind () const
                              { return fNoteHeadFilledKind; }

    void                  setNoteHeadParenthesesKind (
                            msrNoteHeadParenthesesKind
                              noteHeadParenthesesKind)
                              {
                                fNoteHeadParenthesesKind =
                                  noteHeadParenthesesKind;
                              }

    msrNoteHeadParenthesesKind
                          getNoteHeadParenthesesKind () const
                              { return fNoteHeadParenthesesKind; }

    // accidentals

    void                  setNoteAccidentalKind (
                            msrNoteAccidentalKind
                              noteAccidentalKind)
                              {
                                fNoteAccidentalKind =
                                  noteAccidentalKind;
                              }

    msrNoteAccidentalKind
                          getNoteAccidentalKind ()
                              {
                                return
                                  fNoteAccidentalKind;
                              }

    void                  setNoteEditorialAccidentalKind (
                            msrNoteEditorialAccidentalKind
                              noteEditorialAccidentalKind)
                              {
                                fNoteEditorialAccidentalKind =
                                  noteEditorialAccidentalKind;
                              }

    msrNoteEditorialAccidentalKind
                          getNoteEditorialAccidentalKind ()
                              {
                                return
                                  fNoteEditorialAccidentalKind;
                              }

    void                  setNoteCautionaryAccidentalKind (
                            msrNoteCautionaryAccidentalKind
                              noteCautionaryAccidentalKind)
                              {
                                fNoteCautionaryAccidentalKind =
                                  noteCautionaryAccidentalKind;
                              }

    msrNoteCautionaryAccidentalKind
                          getNoteCautionaryAccidentalKind ()
                              {
                                return
                                  fNoteCautionaryAccidentalKind;
                              }

    // note whole notes

    void                  setNoteSoundingWholeNotes (
                            rational wholeNotes);

    rational              getNoteSoundingWholeNotes ()
                              {
                                return
                                  fMeasureElementSoundingWholeNotes; // JMI fNoteSoundingWholeNotes
                              }

    // note display

    msrQuarterTonesPitchKind
                          getNoteQuarterTonesDisplayPitchKind () const
                              {
                                return
                                  fNoteQuarterTonesDisplayPitchKind;
                              }

    void                  setNoteDisplayWholeNotes (
                            rational wholeNotes);

    rational              getNoteDisplayWholeNotes ()
                              { return fNoteDisplayWholeNotes; }

    // dots

    int                   getNoteDotsNumber () const
                              { return fNoteDotsNumber; }

    msrDurationKind       getNoteGraphicDurationKind () const
                              { return fNoteGraphicDurationKind; }

    int                   getNoteOctave () const
                              { return fNoteOctave; }

    int                   getNoteDisplayOctave () const
                              { return fNoteDisplayOctave; }

    bool                  getNoteIsARest () const
                            // useful for rest tuplet members
                            { return fNoteIsARest; }

    bool                  getNoteIsUnpitched () const
                              { return fNoteIsUnpitched; }

    bool                  getNoteIsACueNote () const
                              { return fNoteIsACueNote; }

    // octave shifts
    void                  setNoteOctaveShift (
                             S_msrOctaveShift octaveShift)
                                { fNoteOctaveShift = octaveShift; }

    S_msrOctaveShift      getNoteOctaveShift () const
                              { return fNoteOctaveShift; }

    // note context
    // -------------------------------

    // chord members
    void                  setNoteBelongsToAChord ();

    bool                  getNoteBelongsToAChord () const
                              { return fNoteBelongsToAChord; }

    // grace notes
    bool                  getNoteIsAGraceNote () const
                              { return fNoteIsAGraceNote; }

    // harmonies
    void                  appendHarmonyToNoteHarmoniesList (
                            S_msrHarmony harmony);

    const list<S_msrHarmony>&
                          getNoteHarmoniesList () const
                              { return fNoteHarmoniesList; }

    // figured bass
    void                  appendFiguredBassToNoteFiguredBassesList (
                            S_msrFiguredBass figuredBass);

    const list<S_msrFiguredBass>&
                          getNoteFiguredBassesList () const
                              { return fNoteFiguredBassesList; }

    // note lyrics
    // -------------------------------

    list<S_msrSyllable>   getNoteSyllables () const
                              { return fNoteSyllables; }

    // elements attached to the note
    // -------------------------------

    // stems
    void                  setNoteStem (S_msrStem stem);

    S_msrStem             getNoteStem () const
                              { return fNoteStem; }

    // beams
    const list<S_msrBeam>&
                          getNoteBeams () const
                              { return fNoteBeams; }

    // articulations
    const list<S_msrArticulation>&
                          getNoteArticulations () const
                              { return fNoteArticulations; }

    list<S_msrArticulation>&
                          getNoteArticulationsToModify ()
                              { return fNoteArticulations; }

    // spanners
    const list<S_msrSpanner>&
                          getNoteSpanners () const
                              { return fNoteSpanners; }

    list<S_msrSpanner>&
                          getNoteSpannersToModify ()
                              { return fNoteSpanners; }

    // technicals
    const list<S_msrTechnical>&
                          getNoteTechnicals () const
                              { return fNoteTechnicals; }

    const list<S_msrTechnicalWithInteger>&
                          getNoteTechnicalWithIntegers () const
                              { return fNoteTechnicalWithIntegers; }

    const list<S_msrTechnicalWithFloat>&
                          getNoteTechnicalWithFloats () const
                              { return fNoteTechnicalWithFloats; }

    const list<S_msrTechnicalWithString>&
                          getNoteTechnicalWithStrings () const
                              { return fNoteTechnicalWithStrings; }

    // ornaments
    const list<S_msrOrnament>&
                          getNoteOrnaments () const
                              { return fNoteOrnaments; }

    // glissandos
    const list<S_msrGlissando>&
                          getNoteGlissandos () const
                              { return fNoteGlissandos; }

    // slides
    const list<S_msrSlide>&
                          getNoteSlides () const
                              { return fNoteSlides; }

    // grace notes
    void                  setNoteGraceNotesGroupBefore (
                            S_msrGraceNotesGroup graceNotesGroupBefore);

    S_msrGraceNotesGroup  getNoteGraceNotesGroupBefore () const
                              { return fNoteGraceNotesGroupBefore; }

    void                  setNoteGraceNotesGroupAfter (
                            S_msrGraceNotesGroup graceNotesGroupAfter);

    S_msrGraceNotesGroup  getNoteGraceNotesGroupAfter () const
                              { return fNoteGraceNotesGroupAfter; }

    // after grace notes
    /* JMI
    void                  setNoteAfterGraceNotesGroup (
                            S_msrAfterGraceNotesGroup afterGraceNotesGroup);

    S_msrAfterGraceNotesGroup
                          getNoteAfterGraceNotesGroup () const
                              { return fNoteAfterGraceNotesGroup; }
                            */

    // singleTremolo
    void                  setNoteSingleTremolo (
                            S_msrSingleTremolo trem);

    S_msrSingleTremolo    getNoteSingleTremolo () const
                              { return fNoteSingleTremolo; }

    // ties
    void                  setNoteTie (S_msrTie tie)
                              { fNoteTie = tie; }
    S_msrTie              getNoteTie () const
                              { return fNoteTie; }

    // dynamics
    const list<S_msrDynamics>&
                          getNoteDynamics () const
                              { return fNoteDynamics; }
    const list<S_msrOtherDynamics>&
                          getNoteOtherDynamics () const
                              { return fNoteOtherDynamics; }

    // words
    const list<S_msrWords>&
                          getNoteWords () const
                              { return fNoteWords; }

    list<S_msrWords>&     getNoteWordsToModify ()
                              { return fNoteWords; }

    // slashes
    const list<S_msrSlash>&
                          getNoteSlashes () const
                              { return fNoteSlashes; }

    // wedges
    const list<S_msrWedge>&
                          getNoteWedges () const
                              { return fNoteWedges; }

    list<S_msrWedge>&
                          getNoteWedgesToModify ()
                              { return fNoteWedges; }

    // segnos
    const list<S_msrSegno>&
                          getNoteSegnos () const
                              { return fNoteSegnos; }

    // coda
    const list<S_msrCoda>&
                          getNoteCodas () const
                              { return fNoteCodas; }

    // eyeglasses
    const list<S_msrEyeGlasses>&
                          getNoteEyeGlasses () const
                              { return fNoteEyeGlasses; }

    // damps
    const list<S_msrDamp>&
                          getNoteDamps () const
                              { return fNoteDamps; }

    // damp alls
    const list<S_msrDampAll>&
                          getNoteDampAlls () const
                              { return fNoteDampAlls; }

    // scordaturas
    const list<S_msrScordatura>&
                          getNoteScordaturas () const
                              { return fNoteScordaturas; }

    // slurs
    const list<S_msrSlur>&
                          getNoteSlurs () const
                              { return fNoteSlurs; }

    // ligatures
    const list<S_msrLigature>&
                          getNoteLigatures () const
                              { return fNoteLigatures; }

    // pedals
    const list<S_msrPedal>&
                          getNotePedals () const
                              { return fNotePedals; }

    // note measure information
    // -------------------------------

    // position in measure
    void                  setNotePositionInMeasure (
                            rational positionInMeasure);

    void                  setNoteOccupiesAFullMeasure ();

    bool                  getNoteOccupiesAFullMeasure () const
                              { return fNoteOccupiesAFullMeasure; }

    // note redundant information (for speed)
    bool                  getNoteIsStemless () const
                              { return fNoteIsStemless; }

    void                  setNoteIsAChordsFirstMemberNote ()
                              {
                                fNoteIsAChordsFirstMemberNote = true;
                              }

    bool                  getNoteIsAChordsFirstMemberNote () const
                              { return fNoteIsAChordsFirstMemberNote; }

    void                  setNoteIsFirstNoteInADoubleTremolo ()
                              { fNoteIsFirstNoteInADoubleTremolo = true; }

    bool                  getNoteIsFirstNoteInADoubleTremolo () const
                              { return fNoteIsFirstNoteInADoubleTremolo; }

    void                  setNoteIsSecondNoteInADoubleTremolo ()
                              { fNoteIsSecondNoteInADoubleTremolo = true; }

    bool                  getNoteIsSecondNoteInADoubleTremolo () const
                              { return fNoteIsSecondNoteInADoubleTremolo; }

    S_msrOrnament         getNoteTrillOrnament () const
                              { return fNoteTrillOrnament; }

    S_msrOrnament         getNoteDashesOrnament () const
                              { return fNoteDashesOrnament; }

    S_msrOrnament         getNoteDelayedTurnOrnament () const
                              { return fNoteDelayedTurnOrnament; }

    S_msrOrnament         getNoteDelayedInvertedTurnOrnament () const
                              { return fNoteDelayedInvertedTurnOrnament; }

    S_msrSpanner          getNoteWavyLineSpannerStart () const
                              { return fNoteWavyLineSpannerStart; }

    S_msrSpanner          getNoteWavyLineSpannerStop () const
                              { return fNoteWavyLineSpannerStop; }

    void                  setNoteIsFollowedByGraceNotesGroup ()
                              { fNoteIsFollowedByGraceNotesGroup = true; }

    bool                  getNoteIsFollowedByGraceNotesGroup () const
                              { return fNoteIsFollowedByGraceNotesGroup; }

    void                  setNoteColor (
                            msrAlphaRGBColor noteColor)
                              { fNoteColor = noteColor; }

    msrAlphaRGBColor              getNoteColor () const
                              { return fNoteColor; }

    // services
    // ------------------------------------------------------

    bool                  noteIsAPitchedRest () const;

    string                asShortStringWithRawWholeNotes () const;

    // diatonic pitch
    msrDiatonicPitchKind  noteDiatonicPitchKind (
                            int inputLineNumber) const;

    // tuplet members
    void                  determineTupletMemberSoundingFromDisplayWholeNotes (
                            int actualNotes, int normalNotes);

    // lyrics
    void                  appendSyllableToNote (S_msrSyllable syllable);

    // beams
    void                  appendBeamToNote (S_msrBeam beam);

    // articulations
    void                  appendArticulationToNote (S_msrArticulation art);

    // spanners
    void                  appendSpannerToNote (S_msrSpanner span);

    // technicals
    void                  appendTechnicalToNote (S_msrTechnical tech);

    void                  appendTechnicalWithIntegerToNote (
                            S_msrTechnicalWithInteger tech);

    void                  appendTechnicalWithFloatToNote (
                            S_msrTechnicalWithFloat tech);

    void                  appendTechnicalWithStringToNote (
                            S_msrTechnicalWithString tech);

    // ornaments
    void                  appendOrnamentToNote (S_msrOrnament orn);

    // glissandos
    void                  appendGlissandoToNote (S_msrGlissando glissando);

    // slides
    void                  appendSlideToNote (S_msrSlide slide);

    // dynamics
    void                  appendDynamicsToNote (
                            S_msrDynamics dynamics);
    void                  appendOtherDynamicsToNote (
                            S_msrOtherDynamics otherDynamics);

    S_msrDynamics         removeFirstDynamics (); // ???

    // words
    void                  appendWordsToNote (S_msrWords words);

    // slurs
    void                  appendSlurToNote (S_msrSlur slur);

    // ligatures
    void                  appendLigatureToNote (S_msrLigature ligature);

    // pedals
    void                  appendPedalToNote (S_msrPedal pedal);

    //  slashes
    void                  appendSlashToNote (S_msrSlash slash);

    //  wedges
    void                  appendWedgeToNote (S_msrWedge wedge);

    S_msrWedge            removeFirstWedge (); // JMI

    // segnos
    void                  appendSegnoToNote (S_msrSegno segno);

    // codas
    void                  appendCodaToNote (S_msrCoda coda);

    // eyeglasses
    void                  appendEyeGlassesToNote (S_msrEyeGlasses eyeGlasses);

    // damps
    void                  appendDampToNote (S_msrDamp damp);

    // damp alls
    void                  appendDampAllToNote (S_msrDampAll dampAll);

    // scordaturas
    void                  appendScordaturaToNote (S_msrScordatura scordatura);


  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    // note kind
    string                noteKindAsString () const;

    string                notePitchAsString () const;
    string                noteDisplayPitchKindAsString () const;

    string                noteDisplayOctaveAsString () const;

    // note print kind
    string                notePrintObjectKindAsString () const;

    // note head
    string                noteHeadKindAsString () const;

    string                noteHeadFilledKindAsString () const;
    string                noteHeadParenthesesKindAsString () const;

    string                noteDiatonicPitchKindAsString (
                            int inputLineNumber) const;

    // whole notes
    string                noteSoundingWholeNotesAsMsrString () const;
    string                noteDisplayWholeNotesAsMsrString () const;

    // graphic duration
    string                noteGraphicDurationAsMsrString () const;
    string                tupletNoteGraphicDurationAsMsrString ( // JMI
                            int actualNotes, int normalNotes) const;

    // note as string
    string                asString () const;

    string                asShortString () const;

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    // ------------------------------------------------------

    S_msrChord            fNoteChordUpLink;

    S_msrGraceNotesGroup  fNoteGraceNotesGroupUpLink;

    S_msrTuplet           fNoteTupletUpLink;

    S_msrMeasure          fNoteMeasureUpLink;

    // basic note description
    // ------------------------------------------------------

    // note kind
    msrNoteKind           fNoteKind;

    // note quarter tones pitch
    msrQuarterTonesPitchKind
                          fNoteQuarterTonesPitchKind;

    // whole notes
//    rational              fNoteSoundingWholeNotes;
    rational              fNoteDisplayWholeNotes;

    int                   fNoteDotsNumber;

    msrDurationKind       fNoteGraphicDurationKind;

    int                   fNoteOctave;

    msrQuarterTonesPitchKind
                          fNoteQuarterTonesDisplayPitchKind;
    int                   fNoteDisplayOctave;
                                // for unpitched notes
                                // and pitched rests

    // short cuts for efficiency
    bool                  fNoteIsARest;
    bool                  fNoteIsUnpitched;

    bool                  fNoteIsACueNote;
    bool                  fNoteIsAGraceNote;

    // note octave shift
    S_msrOctaveShift      fNoteOctaveShift;

    // note print kind
    msrPrintObjectKind    fNotePrintObjectKind;

    // note head
    msrNoteHeadKind       fNoteHeadKind;
    msrNoteHeadFilledKind fNoteHeadFilledKind;
    msrNoteHeadParenthesesKind
                          fNoteHeadParenthesesKind;

    // accidentals
    // ------------------------------------------------------

    msrNoteAccidentalKind fNoteAccidentalKind;

    msrNoteEditorialAccidentalKind
                          fNoteEditorialAccidentalKind;
    msrNoteCautionaryAccidentalKind
                          fNoteCautionaryAccidentalKind;

    // staff and voice context
    // ------------------------------------------------------

    int                   fNoteStaffNumber;
    int                   fNoteVoiceNumber;

    // chord member?
    // ------------------------------------------------------

    bool                  fNoteBelongsToAChord;

    // tuplet member?
    // ------------------------------------------------------

    bool                  fNoteBelongsToATuplet;

    // note measure information
    // ------------------------------------------------------

    bool                  fNoteOccupiesAFullMeasure;

    // multiple rest member?
    // ------------------------------------------------------

    bool                  fNoteBelongsToARestMeasures; // JMI
    int                   fNoteRestMeasuresSequenceNumber; // JMI

    // note lyrics
    // ------------------------------------------------------

    list<S_msrSyllable>   fNoteSyllables;

    // stem
    // ------------------------------------------------------

    S_msrStem             fNoteStem;

    // beams
    // ------------------------------------------------------

    list<S_msrBeam>       fNoteBeams;

    // articulations
    // ------------------------------------------------------

    list<S_msrArticulation>
                          fNoteArticulations;

    // spanners
    // ------------------------------------------------------

    list<S_msrSpanner>    fNoteSpanners;

    // technicals
    // ------------------------------------------------------

    list<S_msrTechnical>  fNoteTechnicals;

    list<S_msrTechnicalWithInteger>
                          fNoteTechnicalWithIntegers;

    list<S_msrTechnicalWithFloat>
                          fNoteTechnicalWithFloats;

    list<S_msrTechnicalWithString>
                          fNoteTechnicalWithStrings;

    // ornaments
    // ------------------------------------------------------

    list<S_msrOrnament>   fNoteOrnaments;

    // glissandos
    // ------------------------------------------------------

    list<S_msrGlissando>  fNoteGlissandos;

    // slides
    // ------------------------------------------------------

    list<S_msrSlide>      fNoteSlides;

    // grace notes
    // ------------------------------------------------------

    S_msrGraceNotesGroup  fNoteGraceNotesGroupBefore;
  // JMI  S_msrAfterGraceNotesGroup
    S_msrGraceNotesGroup
                          fNoteGraceNotesGroupAfter;

    // single tremolo
    // ------------------------------------------------------

    S_msrSingleTremolo    fNoteSingleTremolo;

    // tie
    // ------------------------------------------------------

    S_msrTie              fNoteTie;

    // dynamics
    // ------------------------------------------------------

    list<S_msrDynamics>   fNoteDynamics;
    list<S_msrOtherDynamics>
                          fNoteOtherDynamics;

    // slashes
    // ------------------------------------------------------

    list<S_msrSlash>      fNoteSlashes;

    // wedges
    // ------------------------------------------------------

    list<S_msrWedge>      fNoteWedges;

    // segnos
    // ------------------------------------------------------

    list<S_msrSegno>      fNoteSegnos;

    // coda
    // ------------------------------------------------------

    list<S_msrCoda>       fNoteCodas;

    // eyeglasses
    // ------------------------------------------------------

    list<S_msrEyeGlasses> fNoteEyeGlasses;

    // damps
    // ------------------------------------------------------

    list<S_msrDamp>       fNoteDamps;

    // damp alls
    // ------------------------------------------------------

    list<S_msrDampAll>    fNoteDampAlls;

    // scordaturas
    // ------------------------------------------------------

    list<S_msrScordatura> fNoteScordaturas;

    // words
    // ------------------------------------------------------

    list<S_msrWords>      fNoteWords;

    // slurs
    // ------------------------------------------------------

    list<S_msrSlur>       fNoteSlurs;

    // ligatures
    // ------------------------------------------------------

    list<S_msrLigature>   fNoteLigatures;

    // pedals
    // ------------------------------------------------------

    list<S_msrPedal>      fNotePedals;

    // harmonies
    // ------------------------------------------------------

    list<S_msrHarmony>    fNoteHarmoniesList;

    // figured bass
    // ------------------------------------------------------

    list<S_msrFiguredBass>
                          fNoteFiguredBassesList;

    // note redundant information (for speed)
    // ------------------------------------------------------

    // this is needed to handle stemless notes,
    // because the <stem> is visited after 'visitorStart ( S_msrNote )'
    bool                  fNoteIsStemless;

    // this is to distinguish the first chord member note
    // from the other ones, since only it should get the slur if any
    bool                  fNoteIsAChordsFirstMemberNote;

    // this is needed to handle double tremolos
    bool                  fNoteIsFirstNoteInADoubleTremolo;
    bool                  fNoteIsSecondNoteInADoubleTremolo;

    // this is useful
    // as well as to produce a nice \aftergrace in LilyPond
    S_msrOrnament         fNoteTrillOrnament;
    bool                  fNoteIsFollowedByGraceNotesGroup;

    // this is useful to produce a text spanner in LilyPond
    S_msrOrnament         fNoteDashesOrnament;

    // this is needed to produce a delayed turn/inverted-turn in LilyPond
    S_msrOrnament         fNoteDelayedTurnOrnament;
    S_msrOrnament         fNoteDelayedInvertedTurnOrnament;

    // this is useful to  to combine a trill and a wavy line in LilyPond
    S_msrSpanner          fNoteWavyLineSpannerStart;
    S_msrSpanner          fNoteWavyLineSpannerStop;

    // color
    msrAlphaRGBColor              fNoteColor;
};
typedef SMARTP<msrNote> S_msrNote;
EXP ostream& operator<< (ostream& os, const S_msrNote& elt);

