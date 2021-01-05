#ifndef ___msrSegments___
#define ___msrSegments___

#include "msrMeasures.h"
#include "msrVoiceElements.h"
#include "msrVoices.h"

#include "msrRehearsals.h"
#include "msrSegnosAndCodas.h"
#include "msrStaves.h"
#include "msrTablatures.h"
#include "msrTempos.h"
#include "msrVoiceElements.h"
//#include "msrVoices.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msrStaff;
typedef SMARTP<msrStaff> S_msrStaff;

class msrPart;
typedef SMARTP<msrPart> S_msrPart;

class msrPartGroup;
typedef SMARTP<msrPartGroup> S_msrPartGroup;

class msrScore;
typedef SMARTP<msrScore> S_msrScore;

class msrPart;
typedef SMARTP<msrPart> S_msrPart;

class msrPrintLayout;
typedef SMARTP<msrPrintLayout> S_msrPrintLayout;

class msrTime;
typedef SMARTP<msrTime> S_msrTime;

class msrChord;
typedef SMARTP<msrChord> S_msrChord;

class msrTuplet;
typedef SMARTP<msrTuplet> S_msrTuplet;

class msrTranspose;
typedef SMARTP<msrTranspose> S_msrTranspose;

class msrPartNameDisplay;
typedef SMARTP<msrPartNameDisplay> S_msrPartNameDisplay;

class msrPartAbbreviationDisplay;
typedef SMARTP<msrPartAbbreviationDisplay> S_msrPartAbbreviationDisplay;

class msrStaffDetails;
typedef SMARTP<msrStaffDetails> S_msrStaffDetails;

class msrOctaveShift;
typedef SMARTP<msrOctaveShift> S_msrOctaveShift;

class msrHarmony;
typedef SMARTP<msrHarmony> S_msrHarmony;

class msrFiguredBass;
typedef SMARTP<msrFiguredBass> S_msrFiguredBass;

class msrVoiceStaffChange;
typedef SMARTP<msrVoiceStaffChange> S_msrVoiceStaffChange;

class msrVoice;
typedef SMARTP<msrVoice> S_msrVoice;

class msrDoubleTremolo;
typedef SMARTP<msrDoubleTremolo> S_msrDoubleTremolo;

//______________________________________________________________________________
class EXP msrSegment : public msrVoiceElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSegment> create (
      int        inputLineNumber,
      S_msrVoice segmentVoicekUpLink);

    SMARTP<msrSegment> createSegmentNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrSegment> createSegmentDeepCopy (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSegment (
      int        inputLineNumber,
      S_msrVoice segmentVoicekUpLink);

    virtual ~msrSegment ();

  private:

    // private initialization
    // ------------------------------------------------------

    void                  initializeSegment ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink

    S_msrVoice            getSegmentVoiceUpLink () const
                              { return fSegmentVoiceUpLink; }

    // number

    int                   getSegmentAbsoluteNumber () const
                              { return fSegmentAbsoluteNumber; }

    // measures

    const list<S_msrMeasure>&
                          getSegmentMeasuresList () const
                              { return fSegmentMeasuresList; }

    list<S_msrMeasure>&   getSegmentMeasuresListToModify () // JMI
                              { return fSegmentMeasuresList; }

/* JMI
    // segment shortest note

    void                  setSegmentShortestNoteDuration (
                            rational duration);

    rational              getSegmentShortestNoteDuration () const
                              { return fSegmentShortestNoteDuration; }

    void                  setSegmentShortestNoteTupletFactor (
                            const msrTupletFactor& noteTupletFactor);

    const msrTupletFactor&
                          getSegmentShortestNoteTupletFactor () const
                              { return fSegmentShortestNoteTupletFactor; }
*/

  public:

    // public services
    // ------------------------------------------------------

    // upLinks

    S_msrStaff            fetchSegmentStaffUpLink () const;

    S_msrPart             fetchSegmentPartUpLink () const;

    S_msrPartGroup        fetchSegmentPartGroupUpLink () const;

    S_msrScore            fetchSegmentScoreUpLink () const;

    // print layout MusicXML specific??? JMI

     void                 appendPrintLayoutToSegment (
                            S_msrPrintLayout printLayout);

    // backup and padding

    void                  padUpToPositionInMeasureInSegment (
                            int      inputLineNumber,
                            rational wholeNotes);

    void                  backupByWholeNotesStepLengthInSegment (
                            int      inputLineNumber,
                            rational backupTargetMeasureElementPositionInMeasure);

    void                  appendPaddingNoteToSegment (
                            int      inputLineNumber,
                            rational forwardStepLength);

    // measures

    S_msrMeasure          fetchLastMeasureFromSegment (
                            int    inputLineNumber,
                            string context);

    S_msrMeasure          createMeasureAndAppendItToSegment (
                            int    inputLineNumber,
                            string measureNumber,
                            msrMeasureImplicitKind
                                   measureImplicitKind);

    void                  setNextMeasureNumberInSegment (
                            int    inputLineNumber,
                            string nextMeasureNumber);

    void                  appendMeasureToSegment (
                            S_msrMeasure measure);

    void                  prependMeasureToSegment (
                            S_msrMeasure measure);

    // clef, key, time

    void                  appendClefToSegment  (S_msrClef clef);
    void                  prependClefToSegment (S_msrClef clef);

    void                  appendKeyToSegment (S_msrKey key);

    void                  appendTimeToSegment (S_msrTime time);
    void                  appendTimeToSegmentClone (S_msrTime time);

    // dal segno

    void                  insertHiddenMeasureAndBarlineInSegmentClone (
                            int      inputLineNumber,
                            rational positionInMeasure);

    // transpose

    void                  appendTransposeToSegment (
                            S_msrTranspose transpose);

    // part name display

    void                  appendPartNameDisplayToSegment (
                            S_msrPartNameDisplay partNameDisplay);

    // part abbreviation display

    void                  appendPartAbbreviationDisplayToSegment (
                            S_msrPartAbbreviationDisplay partAbbreviationDisplay);

    // staff details

    void                  appendStaffDetailsToSegment (
                            S_msrStaffDetails staffDetails);

    // tempo

    void                  appendTempoToSegment (S_msrTempo tempo);

    // rehearsals

    void                  appendRehearsalToSegment (
                            S_msrRehearsal rehearsal);

    // octave shifts

    void                  appendOctaveShiftToSegment (
                            S_msrOctaveShift octaveShift);

    // scordaturas

    void                  appendScordaturaToSegment (
                            S_msrScordatura scordatura);

    // accordion registration

    void                  appendAccordionRegistrationToSegment (
                            S_msrAccordionRegistration
                              accordionRegistration);

    // harp pedals tuning

    void                  appendHarpPedalsTuningToSegment (
                            S_msrHarpPedalsTuning
                              harpPedalsTuning);

    // harmonies

    void                  appendHarmonyToSegment (
                            S_msrHarmony harmony);

    void                  appendHarmonyToSegmentClone (
                            S_msrHarmony harmony);

    // frames

    void                  appendFrameToSegment (
                            S_msrFrame frame);

    void                  appendFrameToSegmentClone (
                            S_msrFrame frame);

    // figured bass

    void                  appendFiguredBassToSegment (
                            S_msrFiguredBass figuredBass);

    void                  appendFiguredBassToSegmentClone (
                            S_msrFiguredBass figuredBass);

    // staff change

    void                  appendVoiceStaffChangeToSegment (
                            S_msrVoiceStaffChange voiceStaffChange);

    // notes

    void                  appendNoteToSegment (
                            S_msrNote note,
                            rational  partCurrentPositionInMeasure);

    void                  appendNoteToSegmentClone (S_msrNote note);

    // tremolos

    void                  appendDoubleTremoloToSegment (
                            S_msrDoubleTremolo doubleTremolo);

    // chords

    void                  appendChordToSegment (S_msrChord chord);

    // tuplets

    void                  appendTupletToSegment (S_msrTuplet tuplet);

    // bar lines

    void                  appendBarlineToSegment (
                            S_msrBarline barline);

    void                  prependBarlineToSegment (
                            S_msrBarline barline);

    // bar checks

    void                  appendBarCheckToSegment (
                            S_msrBarCheck barCheck);

    // bar number checks

    void                  appendBarNumberCheckToSegment (
                            S_msrBarNumberCheck barNumberCheck);

    // breaks

    void                  appendLineBreakToSegment (S_msrLineBreak lineBreak);
    void                  appendPageBreakToSegment (S_msrPageBreak pageBreak);

    // segno

    void                  appendSegnoToSegment (S_msrSegno segno);

    // coda

    void                  appendCodaToSegment (S_msrCoda coda);

    // eyeglasses

    void                  appendEyeGlassesToSegment (
                            S_msrEyeGlasses eyeGlasses);

    // pedal

    void                  appendPedalToSegment (S_msrPedal pedal);

    // damp

    void                  appendDampToSegment (S_msrDamp damp);

    // damp all

    void                  appendDampAllToSegment (S_msrDampAll dampAll);

    // grace notes

    /* JMI
    void                  addGraceNotesGroupAheadOfSegmentIfNeeded (
                            S_msrGraceNotesGroup graceNotesGroup);

    void                  appendGraceNotesToSegment (
                            S_msrGraceNotes graceNotes);

    void                  appendAfterGraceNotesToSegment (
                            S_msrAfterGraceNotes afterGraceNotes);

    void                  prependAfterGraceNotesToSegment (
                            S_msrAfterGraceNotes afterGraceNotes);
                            */

    // other elements

    void                  prependOtherElementToSegment (
                            S_msrMeasureElement elem);
    void                  appendOtherElementToSegment (
                            S_msrMeasureElement elem);

    // removing elements

    void                  removeNoteFromSegment (
                            int       inputLineNumber,
                            S_msrNote note);

    void                  removeElementFromSegment (
                            int          inputLineNumber,
                            S_msrElement element);

    S_msrMeasure          removeLastMeasureFromSegment (
                            int    inputLineNumber,
                            string context);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortString () const override;
    string                asString () const override;

    void                  displaySegment (
                            int    inputLineNumber,
                            string context);

    void                  print (ostream& os) const override;

    void                  printShort (ostream& os) const override;

  private:

    // private services
    // ------------------------------------------------------

    // necessary due to the complexity of repeats management
    void                  assertSegmentMeasuresListIsNotEmpty (
                            int inputLineNumber) const;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fSegmentVoiceUpLink;

    // counter
    static int            gSegmentsCounter;

    // absolute number, shared by newborn clones and deep copies
    int                   fSegmentAbsoluteNumber;

    // the measures in the segment contain the mmusic
    list<S_msrMeasure>    fSegmentMeasuresList;

    // debug number, unique for every msrSegment instance
    static int            gSegmentDebugNumber;
    int                   fSegmentDebugNumber;

/* JMI
    // segment shortest note

    // fPartShortestNoteDuration and fPartShortestNoteTupletFactor
    // are used to compute a number of divisions per quarter note
    // if needed, such as when generating MusicXML from MSR
    rational              fSegmentShortestNoteDuration;
    msrTupletFactor       fSegmentShortestNoteTupletFactor;
    */
};
typedef SMARTP<msrSegment> S_msrSegment;
EXP ostream& operator<< (ostream& os, const S_msrSegment& elt);


}


#endif
