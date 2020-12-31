#ifndef ___msrNotes___
#define ___msrNotes___

#include <iomanip>

#include "msrArticulations.h"
#include "msrBeams.h"
#include "msrChords.h"
#include "msrFiguredBasses.h"
#include "msrGlissandos.h"
#include "msrGraceNotes.h"
#include "msrHarmonies.h"
#include "msrLigatures.h"
#include "msrLyrics.h"
#include "msrMeasures.h"
#include "msrOrnaments.h"
#include "msrParts.h"
#include "msrSingleTremolos.h"
#include "msrSlashes.h"
#include "msrSlides.h"
#include "msrSlurs.h"
#include "msrSpanners.h"
#include "msrStaves.h"
#include "msrStems.h"
#include "msrTechnicals.h"
#include "msrTies.h"
#include "msrTupletElements.h"
#include "msrTuplets.h"
#include "msrVoices.h"
#include "msrWords.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msrGlissando;
typedef SMARTP<msrGlissando> S_msrGlissando;

/* JMI
class msrHarmony;
typedef SMARTP<msrHarmony> S_msrHarmony;

class msrFiguredBass;
typedef SMARTP<msrFiguredBass> S_msrFiguredBass;

class msrSlide;
typedef SMARTP<msrSlide> S_msrSlide;

class msrSyllable;
typedef SMARTP<msrSyllable> S_msrSyllable;
*/

//______________________________________________________________________________
class EXP msrNote : public msrTupletElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrNoteKind {
      k_NoNoteKind,
      kRestNote, kSkipNote,
      kUnpitchedNote,
      kRegularNote,
      kDoubleTremoloMemberNote,
      kGraceNote, kGraceSkipNote,
      kChordMemberNote,
      kGraceChordMemberNote,
      kTupletMemberNote,
      kTupletRestMemberNote,
      kTupletUnpitchedMemberNote,
      kGraceTupletMemberNote
    };

    static string noteKindAsString (
      msrNoteKind noteKind);

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
      kNoteHeadSo, kNoteHeadLa, kNoteHeadTi
    };

    static string noteHeadKindAsString (
      msrNoteHeadKind noteHeadKind);

    enum msrNoteHeadFilledKind {
      kNoteHeadFilledYes, kNoteHeadFilledNo
    };

    static string noteHeadFilledKindAsString (
      msrNoteHeadFilledKind noteHeadFilledKind);

    enum msrNoteHeadParenthesesKind {
      kNoteHeadParenthesesYes, kNoteHeadParenthesesNo
    };

    static string noteHeadParenthesesKindAsString (
      msrNoteHeadParenthesesKind noteHeadParenthesesKind);

    enum msrNoteIsACueNoteKind {
      kNoteIsACueNoteYes, kNoteIsACueNoteNo
    };

    static string noteIsACueNoteKindAsString (
      msrNoteIsACueNoteKind noteIsACueNoteKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrNote> create (
      int                        inputLineNumber,
      string                     noteMeasureNumber,

      msrNoteKind                noteKind,

      msrQuarterTonesPitchKind   noteQuarterTonesPitchKind,
      msrOctaveKind              noteOctaveKind,

      rational                   noteSoundingWholeNotes,
      rational                   noteDisplayWholeNotes,

      int                        noteDotsNumber,

      msrDurationKind            noteGraphicDurationKind,

      msrQuarterTonesPitchKind   noteQuarterTonesDisplayPitchKind,
      msrOctaveKind              noteDisplayOctaveKind,

      msrNoteIsACueNoteKind      noteIsACueNoteKind,

      msrPrintObjectKind         notePrintObjectKind,

      msrNoteHeadKind            noteHeadKind,
      msrNoteHeadFilledKind      noteHeadFilledKind,
      msrNoteHeadParenthesesKind noteHeadParenthesesKind);

    SMARTP<msrNote> createNoteNewbornClone (
      S_msrPart containingPart);

    SMARTP<msrNote> createNoteDeepCopy (
      S_msrVoice containingVoice);

    // creation from the applications
    // ------------------------------------------------------

    static SMARTP<msrNote> createRestNote (
      int       inputLineNumber,
      string    noteMeasureNumber,
      rational  soundingWholeNotes,
      rational  displayWholeNotes,
      int       dotsNumber);

    static SMARTP<msrNote> createRestNoteWithOctave (
      int           inputLineNumber,
      string        noteMeasureNumber,
      msrOctaveKind noteOctave,
      rational      soundingWholeNotes,
      rational      displayWholeNotes,
      int           dotsNumber);

    static SMARTP<msrNote> createSkipNote (
      int         inputLineNumber,
      string      noteMeasureNumber,
      rational    soundingWholeNotes,
      rational    displayWholeNotes,
      int         dotsNumberr);

    static SMARTP<msrNote> createSkipNoteWithOctave (
      int           inputLineNumber,
      string        noteMeasureNumber,
      msrOctaveKind noteOctave,
      rational      soundingWholeNotes,
      rational      displayWholeNotes,
      int           dotsNumber);

    static SMARTP<msrNote> createGraceSkipNote ( // above with MusicXML??? JMI
      int      inputLineNumber,
      string   noteMeasureNumber,
      rational soundingWholeNotes,
      rational displayWholeNotes,
      int      dotsNumber);

    static SMARTP<msrNote> createRegularNote (
      int                      inputLineNumber,
      string                   noteMeasureNumber,
      msrQuarterTonesPitchKind quarterTonesPitchKind,
      msrOctaveKind            noteOctaveKind,
      rational                 soundingWholeNotes,
      rational                 displayWholeNotes,
      int                      dotsNumber);

    static SMARTP<msrNote> createRestFromString (
      int    inputLineNumber,
      string restString,
      string restMeasureNumber);

    static SMARTP<msrNote> createSkipFromString (
      int    inputLineNumber,
      string skipString,
      string skipMeasureNumber);

    static SMARTP<msrNote> createNoteFromString (
      int    inputLineNumber,
      string noteString,
      string noteMeasureNumber);

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
      msrOctaveKind              noteOctaveKind,

      rational                   noteSoundingWholeNotes,
      rational                   noteDisplayWholeNotes,

      int                        noteDotsNumber,

      msrDurationKind            noteGraphicDurationKind,

      msrQuarterTonesPitchKind   noteQuarterTonesDisplayPitchKind,
      msrOctaveKind              noteDisplayOctaveKind,

      msrNoteIsACueNoteKind      noteIsACueNoteKind,

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

    // note pitch kind

    msrQuarterTonesPitchKind
                          getNoteQuarterTonesPitchKind () const
                              { return fNoteQuarterTonesPitchKind; }

    // note octave kind
    msrOctaveKind         getNoteOctaveKind () const
                              { return fNoteOctaveKind; }

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
                              { fNoteHeadFilledKind = noteHeadFilledKind; }

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
                            msrAccidentalKind accidentalKind)
                              { fNoteAccidentalKind = accidentalKind; }

    msrAccidentalKind     getNoteAccidentalKind () const
                              { return fNoteAccidentalKind; }

    // editorial accidentals

    void                  setNoteEditorialAccidentalKind (
                            msrEditorialAccidentalKind editorialAccidentalKind)
                              {
                                fNoteEditorialAccidentalKind =
                                  editorialAccidentalKind;
                              }

    msrEditorialAccidentalKind
                          getNoteEditorialAccidentalKind () const
                              { return fNoteEditorialAccidentalKind; }

    // cautionary accidentals

    void                  setNoteCautionaryAccidentalKind (
                            msrCautionaryAccidentalKind cautionaryAccidentalKind)
                              {
                                fNoteCautionaryAccidentalKind =
                                  cautionaryAccidentalKind;
                              }

    msrCautionaryAccidentalKind
                          getNoteCautionaryAccidentalKind () const
                              { return fNoteCautionaryAccidentalKind; }

    // note whole notes

    void                  setNoteSoundingWholeNotes (
                            rational wholeNotes)
                              { fMeasureElementSoundingWholeNotes = wholeNotes; }

    rational              getNoteSoundingWholeNotes () const
                              { return fMeasureElementSoundingWholeNotes; }

    // note display

    msrQuarterTonesPitchKind
                          getNoteQuarterTonesDisplayPitchKind () const
                              { return fNoteQuarterTonesDisplayPitchKind; }

    void                  setNoteDisplayWholeNotes (
                            rational wholeNotes)
                              { fNoteDisplayWholeNotes = wholeNotes; }

    rational              getNoteDisplayWholeNotes () const
                              { return fNoteDisplayWholeNotes; }

    // dots

    int                   getNoteDotsNumber () const
                              { return fNoteDotsNumber; }

    msrDurationKind       getNoteGraphicDurationKind () const
                              { return fNoteGraphicDurationKind; }

    // tuplet factor

    void                  setNoteTupletFactor (
                            const msrTupletFactor& tupletFactor)
                              { fNoteTupletFactor = tupletFactor; }

    const msrTupletFactor&
                          getNoteTupletFactor () const
                              { return fNoteTupletFactor; }

    // display octave

    msrOctaveKind         getNoteDisplayOctaveKind () const
                              { return fNoteDisplayOctaveKind; }

    // rest?
    bool                  getNoteIsARest () const
                            // shortcut for efficiency
                              {
                                return
                                  fNoteKind == msrNote::kRestNote
                                    ||
                                  fNoteKind == msrNote::kTupletRestMemberNote;
                              }

    // unpitched?
    bool                  getNoteIsUnpitched () const
                              {
                                return
                                  fNoteKind ==
                                    msrNote::kUnpitchedNote
                                    ||
                                  fNoteKind ==
                                    msrNote::kTupletUnpitchedMemberNote;
                              }

    // cue note?
    void                  setNoteIsACueNoteKind (
                            msrNoteIsACueNoteKind noteIsACueNoteKind)
                              { fNoteIsACueNoteKind = noteIsACueNoteKind; }

    msrNoteIsACueNoteKind getNoteIsACueNoteKind () const
                              { return fNoteIsACueNoteKind; }

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
                              {
                                return
                                  fNoteKind == msrNote::kGraceNote
                                    ||
                                  fNoteKind == msrNote::kGraceSkipNote
                                    ||
                                  fNoteKind == msrNote::kGraceChordMemberNote
                                    ||
                                  fNoteKind == msrNote::kGraceTupletMemberNote;
                              }

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

    // dal segnos
    const list<S_msrDalSegno>&
                          getNoteDalSegnos () const
                              { return fNoteDalSegnos; }

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
                              { fNoteIsAChordsFirstMemberNote = true; }

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

    void                  setNoteAlphaRGBColor (
                            msrAlphaRGBColor& noteAlphaRGBColor);

    const msrAlphaRGBColor&
                          getNoteAlphaRGBColor () const
                              { return fNoteAlphaRGBColor; }
    bool                  getNoteAlphaRGBColorHasBenSet () const
                              { return fNoteAlphaRGBColorHasBenSet; }

  public:

    // public services
    // ------------------------------------------------------

    // uplinks
    S_msrVoice            fetchNoteVoiceUpLink () const;

    S_msrStaff            fetchNoteStaffUpLink () const;

    S_msrPart             fetchNotePartUpLink () const;

    S_msrPartGroup        fetchNotePartGroupUpLink () const;

    S_msrScore            fetchNoteScoreUpLink () const;

    // a pitched rest?
    bool                  noteIsAPitchedRest () const;

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

    // dal segnos
    void                  appendDalSegnoToNote (S_msrDalSegno dalSegno);

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

    virtual void          assignMeasureElementPositionInVoice (
                            rational& positionInVoice,
                            string    context) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    // note kind
    string                noteKindAsString () const;

    // pitch kind
    string                notePitchAsString () const;

    string                noteDisplayPitchKindAsString () const;

    // octave kind
    string                noteOctaveKindAsString () const
                              {
                                return
                                  msrOctaveKindAsString (fNoteOctaveKind);
                              }

    string                noteDisplayOctaveKindAsString () const
                              {
                                return
                                  msrOctaveKindAsString (fNoteDisplayOctaveKind);
                              }

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
    string                notePitchAndSoundingWholeNotesAsString () const
                              {
                                return
                                  notePitchAsString ()
                                    +
                                noteSoundingWholeNotesAsMsrString ();
                              }

    string                noteEssentialsAsString () const;

    string                asString () const override;

    string                asShortString () const override;

    string                asShortStringWithRawWholeNotes () const;

    virtual void          printNoteEssentials (ostream& os) const;

    void                  print (ostream& os) const override;

    void                  printShort (ostream& os) const override;

  private:

    // private fields
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

    // note quarter tones pitch kind
    msrQuarterTonesPitchKind
                          fNoteQuarterTonesPitchKind;

    // note octave kind
    msrOctaveKind         fNoteOctaveKind;

    // whole notes
    rational              fNoteDisplayWholeNotes;

    int                   fNoteDotsNumber;

    msrDurationKind       fNoteGraphicDurationKind;

    msrTupletFactor       fNoteTupletFactor;

    msrQuarterTonesPitchKind
                          fNoteQuarterTonesDisplayPitchKind;
    msrOctaveKind         fNoteDisplayOctaveKind;
                                // for unpitched notes
                                // and pitched rests

    // cue note?
    msrNoteIsACueNoteKind fNoteIsACueNoteKind;

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

    msrAccidentalKind     fNoteAccidentalKind;

    msrEditorialAccidentalKind
                          fNoteEditorialAccidentalKind;
    msrCautionaryAccidentalKind
                          fNoteCautionaryAccidentalKind;

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

    // dal segnos
    // ------------------------------------------------------

    list<S_msrDalSegno>   fNoteDalSegnos;

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
    msrAlphaRGBColor      fNoteAlphaRGBColor;
    bool                  fNoteAlphaRGBColorHasBenSet;
};
typedef SMARTP<msrNote> S_msrNote;
EXP ostream& operator<< (ostream& os, const S_msrNote& elt);


}


#endif
