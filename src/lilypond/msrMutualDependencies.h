/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrMutualDependencies___
#define ___msrMutualDependencies___

#include "typedefs.h"
#include "tree_browser.h"
#include "exports.h"

#include "utilities.h"
#include "messagesHandling.h"

#include "msrElements.h"

#include "msrTupletElements.h"

#include "msrVoiceElements.h"

#include "msrVarValAssocs.h"

#include "msrIdentification.h"
#include "msrPageGeometry.h"
#include "msrLayout.h"

#include "msrCredits.h"

#include "msrDivisions.h"

#include "msrPartNamesDisplays.h"

#include "msrStavesDetails.h"

#include "msrClefs.h"
#include "msrKeys.h"
#include "msrTimes.h"

#include "msrTempos.h"

#include "msrRehearsals.h"

#include "msrWords.h"

#include "msrTranspositions.h"

#include "msrBreaks.h"

#include "msrStems.h"
#include "msrBeams.h"

#include "msrTechnicals.h"

#include "msrBars.h"

#include "msrDynamics.h"
#include "msrArticulations.h"

#include "msrTablatures.h"
#include "msrInstruments.h"
#include "msrOtherElements.h"

#include "msrTies.h"
#include "msrSlurs.h"
#include "msrLigatures.h"

#include "msrSingleTremolos.h"

#include "msrSlashes.h"

#include "msrPartGroups.h"

#include "msrScores.h"

#include "msrMidi.h"

#include "generalOptions.h"


namespace MusicXML2
{

/*
  This file declares classes that cannot be placed in 'individual' headers
  due to mutual dependancies
*/

//______________________________________________________________________________
// PRE-declarations for class mutual dependencies

class msrNote;
typedef SMARTP<msrNote> S_msrNote;

class msrChord;
typedef SMARTP<msrChord> S_msrChord;

class msrTuplet;
typedef SMARTP<msrTuplet> S_msrTuplet;

class msrGraceNotesGroup;
typedef SMARTP<msrGraceNotesGroup> S_msrGraceNotesGroup;

class msrAfterGraceNotesGroup;
typedef SMARTP<msrAfterGraceNotesGroup> S_msrAfterGraceNotesGroup;

class msrHarmony;
typedef SMARTP<msrHarmony> S_msrHarmony;

class msrFiguredBass;
typedef SMARTP<msrFiguredBass> S_msrFiguredBass;

class msrRestMeasures;
typedef SMARTP<msrRestMeasures> S_msrRestMeasures;

class msrMeasuresRepeat;
typedef SMARTP<msrMeasuresRepeat> S_msrMeasuresRepeat;

class msrRepeat;
typedef SMARTP<msrRepeat> S_msrRepeat;

class msrMeasure;
typedef SMARTP<msrMeasure> S_msrMeasure;

class msrSegment;
typedef SMARTP<msrSegment> S_msrSegment;

class msrVoice;
typedef SMARTP<msrVoice> S_msrVoice;

class msrVoiceStaffChange;
typedef SMARTP<msrVoiceStaffChange> S_msrVoiceStaffChange;

class msrPart;
typedef SMARTP<msrPart> S_msrPart;


//______________________________________________________________________________
class msrOrnament : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrOrnamentKind {
        kOrnamentTrill, kOrnamentDashes,
        kOrnamentTurn, kOrnamentInvertedTurn, kOrnamentDelayedTurn,
        kOrnamentDelayedInvertedTurn, kOrnamentVerticalTurn,
        kOrnamentMordent, kOrnamentInvertedMordent,
        kOrnamentSchleifer, kOrnamentShake,
        kOrnamentAccidentalMark };

    static string ornamentKindAsString (
      msrOrnamentKind ornamentKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrOrnament> create (
      int              inputLineNumber,
      msrOrnamentKind  ornamentKind,
      msrPlacementKind ornamentPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOrnament (
      int              inputLineNumber,
      msrOrnamentKind  ornamentKind,
      msrPlacementKind ornamentPlacementKind);

    virtual ~msrOrnament ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setOrnamentNoteUpLink (S_msrNote note)
                              { fOrnamentNoteUpLink = note; }

    S_msrNote             getOrnamentNoteUpLink () const
                              { return fOrnamentNoteUpLink; }

    msrOrnamentKind       getOrnamentKind () const
                              { return fOrnamentKind; }

    void                  setOrnamentPlacementKind (
                            msrPlacementKind
                              ornamentPlacementKind)
                              {
                                fOrnamentPlacementKind =
                                  ornamentPlacementKind;
                              }

    msrPlacementKind      getOrnamentPlacementKind () const
                              { return fOrnamentPlacementKind; }

    void                  setOrnamentAccidentalMarkKind (
                            msrAlterationKind
                              ornamentAccidentalMark)
                              {
                                fOrnamentAccidentalMark =
                                  ornamentAccidentalMark;
                              }

    msrAlterationKind     getOrnamentAccidentalMark () const
                              { return fOrnamentAccidentalMark; }

    // services
    // ------------------------------------------------------

    string                ornamentKindAsString () const;

    string                ornamentPlacementKindAsString () const;

    string                ornamentAccidentalMarkAsString () const;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    S_msrNote             fOrnamentNoteUpLink;

    msrOrnamentKind       fOrnamentKind;

    msrPlacementKind      fOrnamentPlacementKind;

    msrAlterationKind     fOrnamentAccidentalMark;
};
typedef SMARTP<msrOrnament> S_msrOrnament;
EXP ostream& operator<< (ostream& os, const S_msrOrnament& elt);

//______________________________________________________________________________
class msrDoubleTremolo : public msrMeasureElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrDoubleTremoloKind {
      kNotesDoubleTremolo, kChordsDoubleTremolo};

    static string msrDoubleTremoloKindAsString (
      msrDoubleTremoloKind doubleTremolotKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrDoubleTremolo> create (
      int                  inputLineNumber,
      msrDoubleTremoloKind doubleTremoloKind,
      msrTremoloTypeKind   doubleTremoloTypeKind,
      int                  doubleTremoloMarksNumber,
      msrPlacementKind     doubleTremoloPlacementKind);


    SMARTP<msrDoubleTremolo> createDoubleTremoloNewbornClone (
      S_msrElement doubleTremoloFirstElement,
      S_msrElement doubleTremoloSecondElement);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrDoubleTremolo (
      int                  inputLineNumber,
      msrDoubleTremoloKind doubleTremoloKind,
      msrTremoloTypeKind   doubleTremoloTypeKind,
      int                  doubleTremoloMarksNumber,
      msrPlacementKind     doubleTremoloPlacementKind);

    virtual ~msrDoubleTremolo ();

  public:

    // set and get
    // ------------------------------------------------------

    // double tremolo kind

    void                  setDoubleTremoloKind (
                            msrDoubleTremoloKind doubleTremoloKind)
                              { fDoubleTremoloKind = doubleTremoloKind; }

    msrDoubleTremoloKind  getDoubleTremoloKind () const
                              { return fDoubleTremoloKind; }

    // double tremolo type kind

    void                  setDoubleTremoloTypeKind (
                            msrTremoloTypeKind doubleTremoloTypeKind)
                              { fDoubleTremoloTypeKind = doubleTremoloTypeKind; }

    msrTremoloTypeKind    getDoubleTremoloTypeKind () const
                              { return fDoubleTremoloTypeKind; }

    // double tremolo placement

    void                  setDoubleTremoloPlacementKind (
                            msrPlacementKind
                              doubleTremoloPlacementKind)
                              {
                                fDoubleTremoloPlacementKind =
                                  doubleTremoloPlacementKind;
                              }

    msrPlacementKind      getDoubleTremoloPlacementKind () const
                              { return fDoubleTremoloPlacementKind; }

    // double tremolo marks number

    int                   getDoubleTremoloMarksNumber () const
                              { return fDoubleTremoloMarksNumber; }

    // double tremolo elements duration

    rational              getDoubleTremoloElementsDuration () const
                              { return fDoubleTremoloElementsDuration; }

    // double tremolo number of repeats

    void                  setDoubleTremoloNumberOfRepeats (
                            int doubleTremoloNumberOfRepeats)
                              {
                                fDoubleTremoloNumberOfRepeats =
                                  doubleTremoloNumberOfRepeats;
                              }

    int                   getDoubleTremoloNumberOfRepeats () const
                              {
                                return
                                  fDoubleTremoloNumberOfRepeats;
                              }


    // double tremolo placement

    void                  setDoubleTremoloNoteFirstElement (
                            S_msrNote note);

    void                  setDoubleTremoloChordFirstElement (
                            S_msrChord chord);

    // double tremolo first element

    S_msrElement          getDoubleTremoloFirstElement () const
                              { return fDoubleTremoloFirstElement; }

    // double tremolo second element

    void                  setDoubleTremoloNoteSecondElement (
                            S_msrNote note);

    void                  setDoubleTremoloChordSecondElement (
                            S_msrChord chord);

    S_msrElement          getDoubleTremoloSecondElement () const
                              { return fDoubleTremoloSecondElement; }

    // sounding whole notes

    void                  setDoubleTremoloSoundingWholeNotes (
                            rational wholeNotes)
                              {
                                fDoubleTremoloSoundingWholeNotes =
                                  wholeNotes;
                              }

    rational              getDoubleTremoloSoundingWholeNotes () const
                              {
                                return
                                  fDoubleTremoloSoundingWholeNotes;
                              }

    // measure number

    void                  setDoubleTremoloMeasureNumber (
                            string measureNumber)
                              {
                                fMeasureNumber =  measureNumber;
                              }

    // position in measure

    void                  setDoubleTremoloPositionInMeasure (
                            rational positionInMeasure);

    // services
    // ------------------------------------------------------

    // tremolo first note

    void                  setDoubleTremoloFirstNotePositionInMeasure (
                            rational positionInMeasure);

    void                  setDoubleTremoloFirstNoteMeasureNumber (
                            string measureNumber);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                doubleTremoloPlacementKindAsString () const;

    string                asShortString () const;
    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // sounding divisions
    // the same as the displayed divisions of both members
    rational              fDoubleTremoloSoundingWholeNotes;

    msrDoubleTremoloKind  fDoubleTremoloKind;

    msrTremoloTypeKind    fDoubleTremoloTypeKind;

    int                   fDoubleTremoloMarksNumber;

    rational              fDoubleTremoloElementsDuration;

    int                   fDoubleTremoloNumberOfRepeats;

    msrPlacementKind      fDoubleTremoloPlacementKind;

    // the two elements of a double tremole are notes or chords
    S_msrElement          fDoubleTremoloFirstElement;
    S_msrElement          fDoubleTremoloSecondElement;
};
typedef SMARTP<msrDoubleTremolo> S_msrDoubleTremolo;
EXP ostream& operator<< (ostream& os, const S_msrDoubleTremolo& elt);

//______________________________________________________________________________
// pre-declaration for two-way sideLinks
class msrSpanner;
typedef SMARTP<msrSpanner> S_msrSpanner;

class msrSpanner : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrSpannerKind {
        kSpannerDashes, kSpannerWavyLine };

    static string spannerKindAsString (
      msrSpannerKind spannerKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSpanner> create (
      int                inputLineNumber,
      int                spannerNumber,
      msrSpannerKind     spannerKind,
      msrSpannerTypeKind spannerTypeKind,
      msrPlacementKind   spannerPlacementKind,
      S_msrNote          spannerNoteUpLink);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSpanner (
      int                inputLineNumber,
      int                spannerNumber,
      msrSpannerKind     spannerKind,
      msrSpannerTypeKind spannerTypeKind,
      msrPlacementKind   spannerPlacementKind,
      S_msrNote          spannerNoteUpLink);

    virtual ~msrSpanner ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    void                  setSpannerNoteUpLink (
                            S_msrNote spannerNoteUpLink)
                              { fSpannerNoteUpLink = spannerNoteUpLink; }

    S_msrNote             getSpannerNoteUpLink () const
                              { return fSpannerNoteUpLink; }

    // sideLinks
    void                  setSpannerOtherEndSideLink (
                            S_msrSpanner otherEndSideLink);

    S_msrSpanner          getSpannerOtherEndSideLink () const
                              { return fSpannerOtherEndSideLink; }

    int                   getSpannerNumber () const
                              { return fSpannerNumber; }

    msrSpannerKind        getSpannerKind () const
                              { return fSpannerKind; }

    msrSpannerTypeKind    getSpannerTypeKind () const
                              { return fSpannerTypeKind; }

    msrPlacementKind      getSpannerPlacementKind () const
                              { return fSpannerPlacementKind; }

    // services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                spannerKindAsString () const;

    string                spannerTypeKindAsString () const;

    string                spannerPlacementKindAsString () const;

    string                asShortString () const;

    virtual void          print (ostream& os);

  protected:

    // upLinks
    // ------------------------------------------------------
    S_msrNote             fSpannerNoteUpLink;

    // sideLinks
    // ------------------------------------------------------
    S_msrSpanner          fSpannerOtherEndSideLink; // two-way

    // fields
    // ------------------------------------------------------

    int                   fSpannerNumber;

    msrSpannerKind        fSpannerKind;

    msrSpannerTypeKind    fSpannerTypeKind;

    msrPlacementKind      fSpannerPlacementKind;
};
typedef SMARTP<msrSpanner> S_msrSpanner;
EXP ostream& operator<< (ostream& os, const S_msrSpanner& elt);

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

    enum msrMeasureRepeatContextKind {
        kMeasureRepeatContextKindUnknown,
        kMeasureRepeatContextKindNone,
        kMeasureRepeatContextKindCommonPartLastMeasure,
        kMeasureRepeatContextKindHookedEndingLastMeasure,
        kMeasureRepeatContextKindHooklessEndingLastMeasure,
        kMeasureRepeatContextKindNextMeasureAfterCommonPart,
        kMeasureRepeatContextKindNextMeasureAfterHookedEnding,
        kMeasureRepeatContextKindNextMeasureAfterHooklessEnding };

    static string measureRepeatContextKindAsString (
      msrMeasureRepeatContextKind measureRepeatContextKind);

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

    void                  setFullMeasureWholeNotes (
                            rational wholeNotes)
                              { fFullMeasureWholeNotes = wholeNotes; }

    rational              getFullMeasureWholeNotes () const
                              {  return fFullMeasureWholeNotes; }

    void                  setActualMeasureWholeNotes (
                            int      inputLineNumber,
                            rational wholeNotes);

    rational              getActualMeasureWholeNotes () const
                              { return fActualMeasureWholeNotes; }

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


    void                  setMeasureIsASingleMeasureRest ()
                              { fMeasureIsASingleMeasureRest = true; }

    bool                  getMeasureIsASingleMeasureRest ()
                             { return fMeasureIsASingleMeasureRest; }

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

    void                  setMeasureRepeatContextKind (
                            msrMeasureRepeatContextKind measureRepeatContextKind);

    msrMeasureRepeatContextKind
                          getMeasureRepeatContextKind () const
                              { return fMeasureRepeatContextKind; }

  public:

    // public services
    // ------------------------------------------------------

    // upLinks

    S_msrPart             fetchMeasurePartUpLink () const;

    S_msrVoice            fetchMeasureVoiceUpLink () const;

    // lengthes

    string                fullMeasureWholeNotesAsMSRString ();

    string                actualMeasureWholeNotesAsMSRString ();

    S_msrNote             createPaddingNoteForVoice (
                            int        inputLineNumber,
                            rational   duration,
                            S_msrVoice voice);

    void                  padUpToActualMeasureWholeNotesInMeasure (
                            int      inputLineNumber,
                            rational wholeNotes);

    void                  appendPaddingNoteToMeasure (
                            int inputLineNumber,
                            int divisions,
                            int divisionsPerQuarterNote);

    // clefs

    void                  appendClefToMeasure (S_msrClef clef);

    // keys

    void                  appendKeyToMeasure (S_msrKey key);

    // times

    void                  appendTimeToMeasure (S_msrTime time);

    void                  setFullMeasureWholeNotesFromTime (
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

    void                  appendNoteToMeasure (S_msrNote note);
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
                            msrMeasureRepeatContextKind
                                    measureRepeatContextKind);

    void                  padUpToPositionInMeasure (
                            int      inputLineNumber,
                            rational positionInMeasureToPadUpTo);

    void                  padUpToPositionAtTheEndOfMeasure (
                            int      inputLineNumber,
                            rational positionInMeasureToPadUpTo);

    void                  finalizeMeasure (
                            int                         inputLineNumber,
                            msrMeasureRepeatContextKind measureRepeatContextKind,
                            string                      context);

    void                  finalizeRegularMeasure (
                            int                         inputLineNumber,
                            msrMeasureRepeatContextKind measureRepeatContextKind,
                            string                      context);

    void                  finalizeHarmonyMeasure (
                            int                         inputLineNumber,
                            msrMeasureRepeatContextKind measureRepeatContextKind,
                            string                      context);

    void                  finalizeFiguredBassMeasure (
                            int                         inputLineNumber,
                            msrMeasureRepeatContextKind measureRepeatContextKind,
                            string                      context);

    void                  handleHarmoniesInHarmonyMeasureFinalization (
                            int      inputLineNumber,
                            rational partActualMeasureWholeNotesHighTide,
                            string   context);

    void                  finalizeMeasureClone (
                            int          inputLineNumber,
                            S_msrMeasure originalMeasure,
                            S_msrVoice   voiceClone);

  private:

    // private services
    // ------------------------------------------------------

    void                  appendElementToMeasure (
                            S_msrMeasureElement elem);

    void                  appendElementAtTheEndOfMeasure (
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

    rational              fFullMeasureWholeNotes;
                            // meaningfull only
                            // when there is a time signature,
                            // but not for cadenzas

    rational              fActualMeasureWholeNotes;

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

    bool                  fMeasureIsASingleMeasureRest;

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
    msrMeasureRepeatContextKind
                          fMeasureRepeatContextKind;

  public:

    // public work services
    // ------------------------------------------------------

  private:

    // private work services
    // ------------------------------------------------------

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

//______________________________________________________________________________
class msrSegment : public msrVoiceElement
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

    // initialization
    // ------------------------------------------------------

    void                  initializeSegment ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks

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

    // services
    // ------------------------------------------------------

    // upLinks

    S_msrPart             fetchSegmentPartUpLink () const;

    // backup and padding

    void                  padUpToActualMeasureWholeNotesInSegment (
                            int      inputLineNumber,
                            rational wholeNotes);

    void                  appendPaddingNoteToSegment (
                            int inputLineNumber,
                            int divisions,
                            int divisionsPerQuarterNote);

    // measures

    S_msrMeasure          fetchLastMeasureFromSegment (
                            int    inputLineNumber,
                            string context);

    S_msrMeasure          createMeasureAndAppendItToSegment (
                            int    inputLineNumber,
                            string measureNumber,
                            msrMeasure::msrMeasureImplicitKind
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

    void                  appendNoteToSegment      (S_msrNote note);
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

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortString () const;
    string                asString () const;

    void                  displaySegment (
                            int    inputLineNumber,
                            string context);

    virtual void          print (ostream& os);

    virtual void          shortPrint (ostream& os);

  private:

    // fields
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
};
typedef SMARTP<msrSegment> S_msrSegment;
EXP ostream& operator<< (ostream& os, const S_msrSegment& elt);

//______________________________________________________________________________
class msrGraceNotesGroup : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrGraceNotesGroupKind {
      kGraceNotesGroupBefore,
      kGraceNotesGroupAfter };

    static string graceNotesGroupKindAsString (
      msrGraceNotesGroupKind graceNotesGroupKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrGraceNotesGroup> create (
      int                    inputLineNumber,
      msrGraceNotesGroupKind graceNotesGroupKind,
      bool                   graceNotesGroupIsSlashed,
      S_msrVoice             graceNotesGroupVoiceUpLink);

    SMARTP<msrGraceNotesGroup> createGraceNotesGroupNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrGraceNotesGroup> createSkipGraceNotesGroupClone (
      S_msrVoice containingVoice);

    SMARTP<msrGraceNotesGroup> createGraceNotesGroupDeepCopy (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrGraceNotesGroup (
      int                    inputLineNumber,
      msrGraceNotesGroupKind graceNotesGroupKind,
      bool                   graceNotesGroupIsSlashed,
      S_msrVoice             graceNotesGroupVoiceUpLink);

    virtual ~msrGraceNotesGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrVoice            getGraceNotesGroupVoiceUpLink () const
                              { return fGraceNotesGroupVoiceUpLink; }

    void                  setGraceNotesGroupNoteUpLink (
                            S_msrNote note)
                              { fGraceNotesGroupNoteUpLink = note; }

    S_msrNote             getGraceNotesGroupNoteUpLink () const
                              { return fGraceNotesGroupNoteUpLink; }

    msrGraceNotesGroupKind
                          getGraceNotesGroupKind () const
                              { return fGraceNotesGroupKind; }

    void                  setGraceNotesGroupKind (
                            msrGraceNotesGroupKind graceNotesGroupKind)
                              { fGraceNotesGroupKind = graceNotesGroupKind; }

    list<S_msrMeasureElement>&
                          getGraceNotesGroupElementsList ()
                              { return fGraceNotesGroupElementsList; }

    bool                  getGraceNotesGroupIsSlashed () const
                              { return fGraceNotesGroupIsSlashed; }

    bool                  getGraceNotesGroupIsTied () const
                              { return fGraceNotesGroupIsTied; }

    void                  setGraceNotesGroupIsFollowedByNotes (bool value)
                              { fGraceNotesGroupIsFollowedByNotes = value; }

    bool                  getGraceNotesGroupIsFollowedByNotes () const
                              { return fGraceNotesGroupIsFollowedByNotes; }

    void                  setGraceNotesGroupMeasureNumber (
                            string graceNotesGroupMeasureNumber)
                              {
                                fGraceNotesGroupMeasureNumber =
                                  graceNotesGroupMeasureNumber;
                              }

    string                getGraceNotesGroupMeasureNumber () const
                              { return fGraceNotesGroupMeasureNumber; }

    // services
    // ------------------------------------------------------

    S_msrPart             fetchGraceNotesGroupPartUpLink () const;

    void                  appendNoteToGraceNotesGroup (S_msrNote note);
    void                  appendChordToGraceNotesGroup (S_msrChord chord);

    S_msrNote             removeLastNoteFromGraceNotesGroup (
                            int inputLineNumber);


  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortString () const;
    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fGraceNotesGroupVoiceUpLink;
    S_msrNote             fGraceNotesGroupNoteUpLink;

    msrGraceNotesGroupKind
                          fGraceNotesGroupKind;

    list<S_msrMeasureElement>
                          fGraceNotesGroupElementsList;

    bool                  fGraceNotesGroupIsSlashed;
    bool                  fGraceNotesGroupIsTied;

    bool                  fGraceNotesGroupIsFollowedByNotes;

    // LilyPond issue 34 may lead to add skip grace notes to voices
    // other than the one containing these grace notes:
    // the measure number is needed to create the first measure
    // in case the grace notes are at the beginning of the voice
    string                fGraceNotesGroupMeasureNumber; // JMI ???
};
typedef SMARTP<msrGraceNotesGroup> S_msrGraceNotesGroup;
EXP ostream& operator<< (ostream& os, const S_msrGraceNotesGroup& elt);

//______________________________________________________________________________
class msrAfterGraceNotesGroupContents : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrAfterGraceNotesGroupContents> create (
      int        inputLineNumber,
      S_msrVoice afterGraceNotesGroupContentsVoiceUpLink);

    SMARTP<msrAfterGraceNotesGroupContents> createAfterGraceNotesGroupContentsNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrVoice> createAfterGraceNotesGroupContentsDeepCopy (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrAfterGraceNotesGroupContents (
      int        inputLineNumber,
      S_msrVoice afterGraceNotesGroupContentsVoiceUpLink);

    virtual ~msrAfterGraceNotesGroupContents ();

  public:

    // set and get
    // ------------------------------------------------------

    const list<S_msrMeasureElement>&
                          getAfterGraceNotesGroupContentsNotesList ()
                              {
                                return
                                  fAfterGraceNotesGroupContentsNotesList;
                              }

    // services
    // ------------------------------------------------------

    // upLinks
    S_msrPart             fetchAfterGraceNotesGroupContentsPartUpLink () const;

    // notes
    void                  appendNoteToAfterGraceNotesGroupContents (
                            S_msrNote note);
    void                  appendNoteToAfterGraceNotesGroupContents (
                            S_msrChord chord);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fAfterGraceNotesGroupContentsVoiceUpLink;

    // the notes list
    list<S_msrMeasureElement>
                          fAfterGraceNotesGroupContentsNotesList;
};
typedef SMARTP<msrAfterGraceNotesGroupContents> S_msrAfterGraceNotesGroupContents;
EXP ostream& operator<< (ostream& os, const S_msrAfterGraceNotesGroupContents& elt);

//______________________________________________________________________________
class msrAfterGraceNotesGroup : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrAfterGraceNotesGroup> create (
      int          inputLineNumber,
      S_msrElement afterGraceNotesGroupElement,
      bool         afterGraceNotesGroupIsSlashed,
      S_msrVoice   afterGraceNotesGroupVoiceUpLink);

    SMARTP<msrAfterGraceNotesGroup> createAfterGraceNotesGroupNewbornClone (
      S_msrNote  noteClone,
      S_msrVoice containingVoice);

    SMARTP<msrVoice> createAfterGraceNotesGroupDeepCopy (
      S_msrNote  noteClone,
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrAfterGraceNotesGroup (
      int          inputLineNumber,
      S_msrElement afterGraceNotesGroupElement,
      bool         afterGraceNotesGroupIsSlashed,
      S_msrVoice   afterGraceNotesGroupVoiceUpLink);

    virtual ~msrAfterGraceNotesGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrElement          getAfterGraceNotesGroupElement () const
                              { return fAfterGraceNotesGroupElement; }

    S_msrAfterGraceNotesGroupContents
                          getAfterGraceNotesGroupContents () const
                              { return fAfterGraceNotesGroupContents; }

    // services
    // ------------------------------------------------------

    // upLinks
    S_msrPart             fetchAfterGraceNotesGroupPartUpLink () const;

    // notes
    void                  appendNoteToAfterGraceNotesGroupContents (
                            S_msrNote note);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fAfterGraceNotesGroupVoiceUpLink;

    S_msrElement          fAfterGraceNotesGroupElement;

    bool                  fAfterGraceNotesGroupIsSlashed;

    S_msrAfterGraceNotesGroupContents
                          fAfterGraceNotesGroupContents;
};
typedef SMARTP<msrAfterGraceNotesGroup> S_msrAfterGraceNotesGroup;
EXP ostream& operator<< (ostream& os, const S_msrAfterGraceNotesGroup& elt);

//______________________________________________________________________________
class msrStanza;
typedef SMARTP<msrStanza> S_msrStanza;

//______________________________________________________________________________
class msrSyllable : public msrMeasureElement
{
  public:

    // data types
    // ------------------------------------------------------

    // we want to end the line in the LilyPond code at a break
    enum msrSyllableKind {
      kSyllableNone,
      kSyllableSingle,
      kSyllableBegin, kSyllableMiddle, kSyllableEnd,
      kSyllableSkip,
      kSyllableMeasureEnd,
      kSyllableLineBreak, kSyllablePageBreak};

    static string syllableKindAsString (
      msrSyllableKind syllableKind);

    enum msrSyllableExtendKind {
      kSyllableExtendNone,
      kSyllableExtendSingle,
      kSyllableExtendStart, kSyllableExtendContinue, kSyllableExtendStop };

    static string syllableExtendKindAsString (
      msrSyllableExtendKind syllableExtendKind);


    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSyllable> create (
      int                   inputLineNumber,
      msrSyllableKind       syllableKind,
      msrSyllableExtendKind syllableExtendKind,
      rational              syllableWholeNotes,
      msrTupletFactor       syllableTupletFactor,
      S_msrStanza           syllableStanzaUpLink);

    SMARTP<msrSyllable> createSyllableNewbornClone (
      S_msrPart containingPart); // JMI

    SMARTP<msrSyllable> createSyllableDeepCopy (
      S_msrPart containingPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSyllable (
      int                   inputLineNumber,
      msrSyllableKind       syllableKind,
      msrSyllableExtendKind syllableExtendKind,
      rational              syllableWholeNotes,
      msrTupletFactor       syllableTupletFactor,
      S_msrStanza           syllableStanzaUpLink);

    virtual ~msrSyllable ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrNote             getSyllableNoteUpLink () const
                              { return fSyllableNoteUpLink; }

    S_msrStanza           getSyllableStanzaUpLink () const
                              { return fSyllableStanzaUpLink; }

    // syllable kind and contents
    msrSyllableKind       getSyllableKind () const
                              { return fSyllableKind; }

    const list<string>&   getSyllableTextsList ()
                              { return fSyllableTextsList; }

    msrSyllableExtendKind getSyllableExtendKind () const
                              { return fSyllableExtendKind; }

    // syllable whole notes
    rational              getSyllableWholeNotes () const
                              { return fSyllableWholeNotes; }

    // syllable tuplet factor
    msrTupletFactor       getSyllableTupletFactor () const
                              { return fSyllableTupletFactor; }

    // services
    // ------------------------------------------------------

    void                  appendSyllableToNoteAndSetItsNoteUpLink (
                            S_msrNote note);

    void                  appendLyricTextToSyllable (string text);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                syllableNoteUpLinkAsString () const;

    string                syllableWholeNotesAsMsrString () const;

    string                syllableKindAsString () const;

    string                syllableExtendKindAsString () const;

    string                syllableTextsListAsString () const;

    static void           writeTextsList (
                            const list<string>& textsList,
                            ostream&            os);

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrNote             fSyllableNoteUpLink;
    S_msrStanza           fSyllableStanzaUpLink;

    // syllable kind and contents
    msrSyllableKind       fSyllableKind;
    list<string>          fSyllableTextsList;
    msrSyllableExtendKind fSyllableExtendKind;

    // syllable whole notes
    rational              fSyllableWholeNotes;

    // syllable tuplet factor
    msrTupletFactor       fSyllableTupletFactor;
};
typedef SMARTP<msrSyllable> S_msrSyllable;
EXP ostream& operator<< (ostream& os, const S_msrSyllable& elt);

//______________________________________________________________________________
class msrHarmonyDegree : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

/*
Degree elements
  can then add, subtract, or alter from these
  starting points.

      <harmony>
        <root>
          <root-step>B</root-step>
        </root>
        <kind>dominant</kind>
        <degree>
          <degree-value>5</degree-value>
          <degree-alter>1</degree-alter>
          <degree-type>alter</degree-type>
        </degree>
        <degree>
          <degree-value>9</degree-value>
          <degree-alter>1</degree-alter>
          <degree-type>add</degree-type>
        </degree>
      </harmony>
*/

    enum msrHarmonyDegreeTypeKind {
      kHarmonyDegreeTypeAdd,
      kHarmonyDegreeTypeAlter,
      kHarmonyDegreeTypeSubstract };

    static string harmonyDegreeTypeKindAsString (
      msrHarmonyDegreeTypeKind harmonyDegreeTypeKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrHarmonyDegree> create (
      int                      inputLineNumber,
      int                      harmonyDegreeValue,
      msrAlterationKind        harmonyDegreeAlterationKind,
      msrHarmonyDegreeTypeKind harmonyDegreeTypeKind);

    /* JMI
    SMARTP<msrHarmonyDegree> createHarmonyDegreeNewbornClone (
      S_msrPart containingPart);

    SMARTP<msrHarmonyDegree> createHarmonyDegreeDeepCopy ( // JMI ???
      S_msrPart containingPart);
      */

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrHarmonyDegree (
      int                      inputLineNumber,
      int                      harmonyDegreeValue,
      msrAlterationKind        harmonyDegreeAlterationKind,
      msrHarmonyDegreeTypeKind harmonyDegreeTypeKind);

    virtual ~msrHarmonyDegree ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setHarmonyDegreeHarmonyUpLink (
                            S_msrHarmony harmonyUpLink);

    S_msrHarmony          getHarmonyDegreeHarmonyUpLink () const
                              { return fHarmonyDegreeHarmonyUpLink; }

    int                   getHarmonyDegreeValue () const
                              { return fHarmonyDegreeValue; }

    msrAlterationKind     getHarmonyDegreeAlterationKind () const
                              { return fHarmonyDegreeAlterationKind; }

    msrHarmonyDegreeTypeKind
                          getHarmonyDegreeTypeKind () const
                              { return fHarmonyDegreeTypeKind; }

    // services
    // ------------------------------------------------------

    int                   harmonyDegreeAsSemitones () const;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                harmonyDegreeKindAsString () const;
    string                harmonyDegreeKindAsShortString () const;

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrHarmony          fHarmonyDegreeHarmonyUpLink;

    int                   fHarmonyDegreeValue;
    msrAlterationKind     fHarmonyDegreeAlterationKind;
    msrHarmonyDegreeTypeKind
                          fHarmonyDegreeTypeKind;
};
typedef SMARTP<msrHarmonyDegree> S_msrHarmonyDegree;
EXP ostream& operator<< (ostream& os, const S_msrHarmonyDegree& elt);

//______________________________________________________________________________
class msrHarmony : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrHarmony> create (
      int                      inputLineNumber,
      // no harmonyVoiceUpLink yet
      msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
      msrHarmonyKind           harmonyKind,
      string                   harmonyKindText,
      int                      harmonyInversion,
      msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
      rational                 harmonySoundingWholeNotes,
      rational                 harmonyDisplayWholeNotes,
      int                      harmonyStaffNumber,
      msrTupletFactor          harmonyTupletFactor,
      rational                 harmonyWholeNotesOffset);

    SMARTP<msrHarmony> createHarmonyNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrHarmony> createHarmonyDeepCopy ( // JMI ???
      S_msrVoice containingVoice);

    // applications API
    // ------------------------------------------------------

    static SMARTP<msrHarmony> create (
      int                      inputLineNumber,
      S_msrVoice               harmonyVoiceUpLink,
      msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
      msrHarmonyKind           harmonyKind,
      string                   harmonyKindText,
      int                      harmonyInversion,
      msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
      rational                 harmonySoundingWholeNotes,
      rational                 harmonyDisplayWholeNotes,
      int                      harmonyStaffNumber,
      msrTupletFactor          harmonyTupletFactor,
      rational                 harmonyWholeNotesOffset);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrHarmony (
      int                      inputLineNumber,
      S_msrVoice               harmonyVoiceUpLink,
      msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
      msrHarmonyKind           harmonyKind,
      string                   harmonyKindText,
      int                      harmonyInversion,
      msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
      rational                 harmonySoundingWholeNotes,
      rational                 harmonyDisplayWholeNotes,
      int                      harmonyStaffNumber,
      msrTupletFactor          harmonyTupletFactor,
      rational                 harmonyWholeNotesOffset);

    virtual ~msrHarmony ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplinks
    void                  setHarmonyNoteUpLink (
                            S_msrNote note)
                              { fHarmonyNoteUpLink = note; }

    S_msrNote             getHarmonyNoteUpLink () const
                             { return fHarmonyNoteUpLink; }

    void                  setHarmonyVoiceUpLink (
                            S_msrVoice voice)
                              { fHarmonyVoiceUpLink = voice; }

    S_msrVoice            getHarmonyVoiceUpLink () const
                             { return fHarmonyVoiceUpLink; }

    void                  setHarmonySoundingWholeNotes (
                            rational wholeNotes)
                              { fHarmonySoundingWholeNotes = wholeNotes; }

    rational              getHarmonySoundingWholeNotes () const
                              { return fHarmonySoundingWholeNotes; }

    void                  setHarmonyDisplayWholeNotes (
                            rational wholeNotes)
                              { fHarmonyDisplayWholeNotes = wholeNotes; }

    rational              getHarmonyDisplayWholeNotes () const
                              { return fHarmonyDisplayWholeNotes; }

    msrQuarterTonesPitchKind
                          getHarmonyRootQuarterTonesPitchKind () const
                              { return fHarmonyRootQuarterTonesPitchKind; }

    msrHarmonyKind        getHarmonyKind () const
                              { return fHarmonyKind; }

    string                getHarmonyKindText () const
                              { return fHarmonyKindText; }

    const list<S_msrHarmonyDegree>&
                          getHarmonyDegreesList ()
                              { return fHarmonyDegreesList; }

    int                   getHarmonyInversion () const
                              { return fHarmonyInversion; }

    msrQuarterTonesPitchKind
                          getHarmonyBassQuarterTonesPitchKind () const
                              { return fHarmonyBassQuarterTonesPitchKind; }

    void                  setHarmonyStaffNumber (
                            int staffNumber)
                              { fHarmonyStaffNumber = staffNumber; }

    int                   getHarmonyStaffNumber () const
                             { return fHarmonyStaffNumber; }

    void                  setHarmonyTupletFactor (
                            msrTupletFactor tupletFactor)
                              { fHarmonyTupletFactor = tupletFactor; }

    msrTupletFactor       getHarmonyTupletFactor () const
                              { return fHarmonyTupletFactor; }

    rational              getHarmonyWholeNotesOffset () const
                              { return fHarmonyWholeNotesOffset; }

    void                  setHarmonyPositionInMeasure (
                            rational positionInMeasure);

    // services
    // ------------------------------------------------------

    void                  appendHarmonyDegreeToHarmony (
                            S_msrHarmonyDegree harmonyDegree)
                              {
                                fHarmonyDegreesList.push_back (
                                  harmonyDegree);
                              }

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrNote             fHarmonyNoteUpLink;
    S_msrVoice            fHarmonyVoiceUpLink;

    rational              fHarmonySoundingWholeNotes;
    rational              fHarmonyDisplayWholeNotes;

    msrQuarterTonesPitchKind
                          fHarmonyRootQuarterTonesPitchKind;

    msrHarmonyKind        fHarmonyKind;
    string                fHarmonyKindText;

    int                   fHarmonyInversion;

    msrQuarterTonesPitchKind
                          fHarmonyBassQuarterTonesPitchKind;

    list<S_msrHarmonyDegree>
                          fHarmonyDegreesList;

    int                   fHarmonyStaffNumber;

    msrTupletFactor       fHarmonyTupletFactor;

    rational              fHarmonyWholeNotesOffset;
};
typedef SMARTP<msrHarmony> S_msrHarmony;
EXP ostream& operator<< (ostream& os, const S_msrHarmony& elt);

//______________________________________________________________________________
class msrFigure : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrFigurePrefixKind {
      k_NoFigurePrefix,
      kDoubleFlatPrefix, kFlatPrefix, kFlatFlatPrefix,
      kNaturalPrefix,
      kSharpSharpPrefix, kSharpPrefix, kDoubleSharpPrefix };

    static string figurePrefixKindAsString (
      msrFigurePrefixKind figurePrefixKind);

    enum msrFigureSuffixKind {
      k_NoFigureSuffix,
      kDoubleFlatSuffix, kFlatSuffix, kFlatFlatSuffix,
      kNaturalSuffix,
      kSharpSharpSuffix, kSharpSuffix, kDoubleSharpSuffix,
      kSlashSuffix };

    static string figureSuffixKindAsString (
      msrFigureSuffixKind figureSuffixKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFigure> create (
      int                 inputLineNumber,
      S_msrPart           figurePartUpLink,
      msrFigurePrefixKind figurePrefixKind,
      int                 figureNumber,
      msrFigureSuffixKind figureSuffixKind);

    SMARTP<msrFigure> createFigureNewbornClone (
      S_msrPart containingPart);

    SMARTP<msrFigure> createFigureDeepCopy ( // JMI ???
      S_msrPart containingPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrFigure (
      int                 inputLineNumber,
      S_msrPart           figurePartUpLink,
      msrFigurePrefixKind figurePrefixKind,
      int                 figureNumber,
      msrFigureSuffixKind figureSuffixKind);

    virtual ~msrFigure ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrPart             getFigurePartUpLink () const
                              { return fFigurePartUpLink; }

    msrFigurePrefixKind
                          getFigurePrefixKind () const
                              { return fFigurePrefixKind; }

    int                   getFigureNumber () const
                              { return fFigureNumber; }

    msrFigureSuffixKind
                          getFigureSuffixKind () const
                              { return fFigureSuffixKind; }

    // services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                figurePrefixKindAsString () const;
    string                figureSuffixKindAsShortString () const;

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrPart             fFigurePartUpLink;

    msrFigurePrefixKind
                          fFigurePrefixKind;
    int                   fFigureNumber;
    msrFigureSuffixKind
                          fFigureSuffixKind;
};
typedef SMARTP<msrFigure> S_msrFigure;
EXP ostream& operator<< (ostream& os, const S_msrFigure& elt);

//______________________________________________________________________________
class msrFiguredBass : public msrMeasureElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrFiguredBassParenthesesKind {
      kFiguredBassParenthesesYes, kFiguredBassParenthesesNo };

    static string figuredBassParenthesesKindAsString (
      msrFiguredBassParenthesesKind figuredBassParenthesesKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFiguredBass> create (
      int       inputLineNumber); // JMI ???

    static SMARTP<msrFiguredBass> create (
      int                           inputLineNumber,
 //   S_msrPart                     figuredBassPartUpLink,
      rational                      figuredBassSoundingWholeNotes,
      rational                      figuredBassDisplayWholeNotes,
      msrFiguredBassParenthesesKind figuredBassParenthesesKind,
      msrTupletFactor               figuredBassTupletFactor);

    SMARTP<msrFiguredBass> createFiguredBassNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrFiguredBass> createFiguredBassDeepCopy (); // JMI ???

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrFiguredBass (
      int                           inputLineNumber,
 //   S_msrPart                     figuredBassPartUpLink,
      rational                      figuredBassSoundingWholeNotes,
      rational                      figuredBassDisplayWholeNotes,
      msrFiguredBassParenthesesKind figuredBassParenthesesKind,
      msrTupletFactor               figuredBassTupletFactor);

    virtual ~msrFiguredBass ();

  public:

    // set and get
    // ------------------------------------------------------
    void                  setFiguredBassNoteUpLink (
                            S_msrNote note)
                              { fFiguredBassNoteUpLink = note; }

    S_msrNote             getFiguredBassNoteUpLink () const
                             { return fFiguredBassNoteUpLink; }

    void                  setFiguredBassVoiceUpLink (
                            S_msrVoice voice)
                              { fFiguredBassVoiceUpLink = voice; }

    S_msrVoice            getFiguredBassVoiceUpLink () const
                             { return fFiguredBassVoiceUpLink; }

/* JMI
    S_msrPart             getFiguredBassPartUpLink () const
                              { return fFiguredBassPartUpLink; }
*/

    void                  setFiguredBassSoundingWholeNotes ( // JMI
                            rational figuredBassSoundingWholeNotes)
                              {
                                fFiguredBassSoundingWholeNotes =
                                  figuredBassSoundingWholeNotes;
                              }

    rational              getFiguredBassSoundingWholeNotes () const
                              { return fFiguredBassSoundingWholeNotes; }

    void                  setFiguredBassDisplayWholeNotes (
                            rational wholeNotes)
                              { fFiguredBassDisplayWholeNotes = wholeNotes; }

    rational              getFiguredBassDisplayWholeNotes () const
                              { return fFiguredBassDisplayWholeNotes; }

    msrFiguredBassParenthesesKind
                          getFiguredBassParenthesesKind () const
                              { return fFiguredBassParenthesesKind; }

    const list<S_msrFigure>&
                          getFiguredBassFiguresList ()
                              { return fFiguredBassFiguresList;  }

    void                  setFiguredBassTupletFactor (
                            msrTupletFactor tupletFactor)
                              { fFiguredBassTupletFactor = tupletFactor; }

    msrTupletFactor       getFiguredBassTupletFactor () const
                              { return fFiguredBassTupletFactor; }

    // services
    // ------------------------------------------------------

    void                  appendFigureToFiguredBass (
                            S_msrFigure figure);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
 // JMI   S_msrPart             fFiguredBassPartUpLink;
    S_msrNote             fFiguredBassNoteUpLink;
    S_msrVoice            fFiguredBassVoiceUpLink;

    rational              fFiguredBassSoundingWholeNotes;
    rational              fFiguredBassDisplayWholeNotes;

    msrFiguredBassParenthesesKind
                          fFiguredBassParenthesesKind;

    list<S_msrFigure>     fFiguredBassFiguresList;

    msrTupletFactor       fFiguredBassTupletFactor;
};
typedef SMARTP<msrFiguredBass> S_msrFiguredBass;
EXP ostream& operator<< (ostream& os, const S_msrFiguredBass& elt);

//______________________________________________________________________________
class msrGlissando : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrGlissandoTypeKind {
      kGlissandoTypeNone,
      kGlissandoTypeStart, kGlissandoTypeStop };

    static string glissandoTypeKindAsString (
      msrGlissandoTypeKind glissandoTypeKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrGlissando> create (
      int                  inputLineNumber,
      int                  glissandoNumber,
      msrGlissandoTypeKind glissandoTypeKind,
      msrLineTypeKind      glissandoLineTypeKind,
      string               glissandoTextValue);

    SMARTP<msrGlissando> createGlissandoNewbornClone ();

    SMARTP<msrGlissando> createGlissandoDeepCopy ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrGlissando (
      int                  inputLineNumber,
      int                  glissandoNumber,
      msrGlissandoTypeKind glissandoTypeKind,
      msrLineTypeKind      glissandoLineTypeKind,
      string               glissandoTextValue);

    virtual ~msrGlissando ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getGlissandoNumber () const
                              { return fGlissandoNumber; }

    msrGlissandoTypeKind  getGlissandoTypeKind () const
                              { return fGlissandoTypeKind; }

    msrLineTypeKind       getGlissandoLineTypeKind () const
                              { return fGlissandoLineTypeKind; }

    string                getGlissandoTextValue () const
                              { return fGlissandoTextValue; }

    // measure upLink
    void                  setGlissandoMeasureUpLink (
                            const S_msrMeasure& measure)
                              { fGlissandoMeasureUpLink = measure; }

    S_msrMeasure          getGlissandoMeasureUpLink () const
                              { return fGlissandoMeasureUpLink; }

    // services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // upLink
    S_msrMeasure          fGlissandoMeasureUpLink;

    int                   fGlissandoNumber;

    msrGlissandoTypeKind  fGlissandoTypeKind;

    msrLineTypeKind       fGlissandoLineTypeKind;

    string                fGlissandoTextValue;
};
typedef SMARTP<msrGlissando> S_msrGlissando;
EXP ostream& operator<< (ostream& os, const S_msrGlissando& elt);

//______________________________________________________________________________
class msrSlide : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrSlideTypeKind {
      kSlideTypeNone,
      kSlideTypeStart, kSlideTypeStop };

    static string slideTypeKindAsString (
      msrSlideTypeKind slideTypeKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSlide> create (
      int              inputLineNumber,
      int              slideNumber,
      msrSlideTypeKind slideTypeKind,
      msrLineTypeKind  slideLineTypeKind,
      string           slideTextValue);

    SMARTP<msrSlide> createSlideNewbornClone ();

    SMARTP<msrSlide> createSlideDeepCopy ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSlide (
      int              inputLineNumber,
      int              slideNumber,
      msrSlideTypeKind slideTypeKind,
      msrLineTypeKind  slideLineTypeKind,
      string           slideTextValue);

    virtual ~msrSlide ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getSlideNumber () const
                              { return fSlideNumber; }

    msrSlideTypeKind      getSlideTypeKind () const
                              { return fSlideTypeKind; }

    msrLineTypeKind       getSlideLineTypeKind () const
                              { return fSlideLineTypeKind; }

    string                getSlideTextValue () const
                              { return fSlideTextValue; }

    // measure upLink
    void                  setSlideMeasureUpLink (
                            const S_msrMeasure& measure)
                              { fSlideMeasureUpLink = measure; }

    S_msrMeasure          getSlideMeasureUpLink () const
                              { return fSlideMeasureUpLink; }

    // services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // upLink
    S_msrMeasure          fSlideMeasureUpLink;

    int                   fSlideNumber;

    msrSlideTypeKind      fSlideTypeKind;

    msrLineTypeKind       fSlideLineTypeKind;

    string                fSlideTextValue;
};
typedef SMARTP<msrSlide> S_msrSlide;
EXP ostream& operator<< (ostream& os, const S_msrSlide& elt);

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
      kStandaloneNote,
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

      kNoteAccidentalSharp, kNoteAccidentalNatural, kNoteAccidentalFlat, kNoteAccidentalDoubleSharp, kNoteAccidentalSharpSharp,
      kNoteAccidentalFlatFlat, kNoteAccidentalNaturalSharp,
      kNoteAccidentalNaturalFlat, kNoteAccidentalQuarterFlat, kNoteAccidentalQuarterSharp,kNoteAccidentalThreeQuartersFlat, kNoteAccidentalThreeQuartersSharp,

      kNoteAccidentalSharpDown, kNoteAccidentalSharpUp,
      kNoteAccidentalNaturalDown, kNoteAccidentalNaturalUp,
      kNoteAccidentalFlatDown, kNoteAccidentalFlatUp,kNoteAccidentalTripleSharp, kNoteAccidentalTripleFlat,
      kNoteAccidentalSlashQuarterSharp, kNoteAccidentalSlashSharp, kNoteAccidentalSlashFlat,kNoteAccidentaldoubleSlashFlat,
      kNoteAccidentalSharp_1, kNoteAccidentalSharp_2, kNoteAccidentalSharp_3, kNoteAccidentalSharp_5, kNoteAccidentalFlat_1, kNoteAccidentalFlat_2, kNoteAccidentalFlat_3, kNoteAccidentalFlat_4, kNoteAccidentalSori, kNoteAccidentalKoron };

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
                                  fNoteSoundingWholeNotes;
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

    // harmony
    void                  setNoteHarmony (
                            S_msrHarmony harmony);

    const S_msrHarmony&   getNoteHarmony () const
                              { return fNoteHarmony; };

    // frame
    void                  setNoteFrame (
                            S_msrFrame frame);

    const S_msrFrame&     getNoteFrame () const
                              { return fNoteFrame; };

    // figured bass
    void                  setNoteFiguredBass (
                            S_msrFiguredBass figuredBass);

    const S_msrFiguredBass&
                          getNoteFiguredBass () const
                              { return fNoteFiguredBass; };

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

    // eyeglasses
    const list<S_msrEyeGlasses>&
                          getNoteEyeGlasses () const
                              { return fNoteEyeGlasses; }

    // damps
    const list<S_msrDamp>&
                          getNoteDamps () const
                              { return fNoteDamps; };

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
                            msrColor noteColor)
                              { fNoteColor = noteColor; }

    msrColor              getNoteColor () const
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

    virtual void          print (ostream& os);

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
    rational              fNoteSoundingWholeNotes;
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

    // harmony
    // ------------------------------------------------------

    S_msrHarmony          fNoteHarmony;

    // frame
    // ------------------------------------------------------

    S_msrFrame            fNoteFrame;

    // figured bass
    // ------------------------------------------------------

    S_msrFiguredBass      fNoteFiguredBass;

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
    msrColor              fNoteColor;
};
typedef SMARTP<msrNote> S_msrNote;
EXP ostream& operator<< (ostream& os, const S_msrNote& elt);

//______________________________________________________________________________
class msrChord : public msrTupletElement
{
  public:

    static SMARTP<msrChord> create (
      int             inputLineNumber,
      rational        chordSoundingWholeNotes,
      rational        chordDisplayWholeNotes,
      msrDurationKind chordGraphicDurationKind);

    // creation from MusicXML
    // ------------------------------------------------------

    SMARTP<msrChord> createChordNewbornClone (
      S_msrPart containingPart);

    SMARTP<msrChord> createChordDeepCopy (
      S_msrPart containingPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrChord (
      int             inputLineNumber,
      rational        chordSoundingWholeNotes,
      rational        chordDisplayWholeNotes,
      msrDurationKind chordGraphicDurationKind);

    virtual ~msrChord ();

  public:

    // set and get
    // ------------------------------------------------------

     // whole notes
    void                  setChordSoundingWholeNotes (
                            rational divisions);

    rational              getChordSoundingWholeNotes () const
                              { return fChordSoundingWholeNotes; }

    void                  setChordDisplayWholeNotes (
                            rational divisions);

    rational              getChordDisplayWholeNotes () const
                              { return fChordDisplayWholeNotes; }

    // graphic duration
    msrDurationKind       getChordGraphicDurationKind () const
                              { return fChordGraphicDurationKind; }

    // chord notes
    const vector<S_msrNote>&
                          getChordNotesVector () const
                              { return fChordNotesVector; }

    // stems
    const list<S_msrStem>&
                          getChordStems () const
                              { return fChordStems; }

    // beams
    const list<S_msrBeam>&
                          getChordBeams () const
                              { return fChordBeams; }

    // articulations
    const list<S_msrArticulation>&
                          getChordArticulations () const
                              { return fChordArticulations; }

    // spanners
    const list<S_msrSpanner>&
                          getChordSpanners () const
                              { return fChordSpanners; }

    // technicals
    const list<S_msrTechnical>&
                          getChordTechnicals () const
                              { return fChordTechnicals; }

    const list<S_msrTechnicalWithInteger>&
                          getChordTechnicalWithIntegers () const
                              { return fChordTechnicalWithIntegers; }

    const list<S_msrTechnicalWithFloat>&
                          getChordTechnicalWithFloats () const
                              { return fChordTechnicalWithFloats; }

    const list<S_msrTechnicalWithString>&
                          getChordTechnicalWithStrings () const
                              { return fChordTechnicalWithStrings; }

    // ornaments
    const list<S_msrOrnament>&
                          getChordOrnaments () const
                              { return fChordOrnaments; }

    // glissandos
    const list<S_msrGlissando>&
                          getChordGlissandos () const
                              { return fChordGlissandos; }

    // slides
    const list<S_msrSlide>&
                          getChordSlides () const
                              { return fChordSlides; }

    // singleTremolo
    void                  setChordSingleTremolo (
                            S_msrSingleTremolo trem);

    S_msrSingleTremolo    getChordSingleTremolo () const
                              { return fChordSingleTremolo; }

    // dynamics
    const list<S_msrDynamics>&
                          getChordDynamics () const
                              { return fChordDynamics; }
    const list<S_msrOtherDynamics>&
                          getChordOtherDynamics () const
                              { return fChordOtherDynamics; }

    const list<S_msrWedge>&
                          getChordWedges () const
                              { return fChordWedges; }

    // words
    const list<S_msrWords>&
                          getChordWords () const
                              { return fChordWords; }

    // ties
    const list<S_msrTie>&
                          getChordTies () const
                              { return fChordTies; }

    // slurs
    const list<S_msrSlur>&
                          getChordSlurs () const
                              { return fChordSlurs; }

    // ligatures
    const list<S_msrLigature>&
                          getChordLigatures () const
                              { return fChordLigatures; }

    // pedals
    const list<S_msrPedal>&
                          getChordPedals () const
                              { return fChordPedals; }

    // double tremolo
    void                  setChordIsFirstChordInADoubleTremolo ()
                              { fChordIsFirstChordInADoubleTremolo = true; }

    bool                  getChordIsFirstChordInADoubleTremolo () const
                              { return fChordIsFirstChordInADoubleTremolo; }

    void                  setChordIsSecondChordInADoubleTremolo ()
                              { fChordIsSecondChordInADoubleTremolo = true; }

    bool                  getChordIsSecondChordInADoubleTremolo () const
                              { return fChordIsSecondChordInADoubleTremolo; }

    // grace notes
    void                  setChordGraceNotesGroupBefore (
                            S_msrGraceNotesGroup graceNotesGroupBefore)
                              {
                                fChordGraceNotesGroupBefore =
                                  graceNotesGroupBefore;
                              }

    S_msrGraceNotesGroup  getChordGraceNotesGroupBefore () const
                              { return fChordGraceNotesGroupBefore; }

    void                  setChordGraceNotesGroupAfter (
                            S_msrGraceNotesGroup graceNotesGroupAfter)
                              {
                                fChordGraceNotesGroupAfter =
                                  graceNotesGroupAfter;
                              }

    S_msrGraceNotesGroup  getChordGraceNotesGroupAfter () const
                              { return fChordGraceNotesGroupAfter; }


    // harmony
    void                  setChordHarmony (
                            S_msrHarmony harmony)
                              { fChordHarmony = harmony; }

    // figured bass
    void                  setChordFiguredBass (
                            S_msrFiguredBass figuredBass)
                              { fChordFiguredBass = figuredBass; }

    const S_msrFiguredBass&
                          getChordFiguredBass () const
                              { return fChordFiguredBass; };

    // octave shift
    void                  setChordOctaveShift (
                            S_msrOctaveShift octaveShift)
                              { fChordOctaveShift = octaveShift; }

    S_msrOctaveShift      getChordOctaveShift () const
                            { return fChordOctaveShift; }

    // measure upLink
    void                  setChordMeasureUpLink (
                            const S_msrMeasure& measure)
                              { fChordMeasureUpLink = measure; }

    S_msrMeasure          getChordMeasureUpLink () const
                            { return fChordMeasureUpLink; }

    // positions in measures
    void                  setChordMembersPositionInMeasure (
                            rational positionInMeasure);

    // services
    // ------------------------------------------------------

    // notes
    void                  addFirstNoteToChord (
                            S_msrNote  note,
                            S_msrVoice voice);

    void                  addAnotherNoteToChord (
                            S_msrNote note,
                            S_msrVoice voice);

    S_msrNote             fetchChordFirstNonGraceNote () const;

    // articulations
    void                  appendArticulationToChord (S_msrArticulation art);

    // spanners
    void                  appendSpannerToChord (S_msrSpanner span);

    // technicals
    void                  appendTechnicalToChord (S_msrTechnical tech);

    void                  appendTechnicalWithIntegerToChord (
                            S_msrTechnicalWithInteger tech);

    void                  appendTechnicalWithFloatToChord (
                            S_msrTechnicalWithFloat tech);

    void                  appendTechnicalWithStringToChord (
                            S_msrTechnicalWithString tech);

    // ornaments
    void                  appendOrnamentToChord (S_msrOrnament orn);

    // glissandos
    void                  appendGlissandoToChord (S_msrGlissando gliss);

    // slides
    void                  appendSlideToChord (S_msrSlide slide);

    // dynamics
    void                  appendDynamicsToChord (S_msrDynamics dynamic)
                              {
                                fChordDynamics.push_back (dynamic);
                              }
    void                  appendOtherDynamicsToChord (
                            S_msrOtherDynamics otherDynamic)
                              {
                                fChordOtherDynamics.push_back (otherDynamic);
                              }

    // slashes
    void                  appendSlashToChord (S_msrSlash slash)
                              {
                                fChordSlashes.push_back (slash);
                              }

    // wedges
    void                  appendWedgeToChord (S_msrWedge wedge)
                              {
                                fChordWedges.push_back (wedge);
                              }

    // words
    void                  appendWordsToChord (S_msrWords dynamic)
                              {
                                fChordWords.push_back (dynamic);
                              }

    // ties
    void                  appendTieToChord (S_msrTie tie)
                              {
                                fChordTies.push_back (tie);
                              }

    // slurs
    void                  appendSlurToChord (S_msrSlur slur)
                              {
                                fChordSlurs.push_back (slur);
                              }

    // stems
    void                  appendStemToChord (S_msrStem stem)
                              {
                                fChordStems.push_back (stem);
                              }

    // beams
    void                  appendBeamToChord (S_msrBeam beam);

    // ligatures
    void                  appendLigatureToChord (S_msrLigature ligature)
                              {
                                fChordLigatures.push_back (ligature);
                              }

    // pedals
    void                  appendPedalToChord (S_msrPedal pedal)
                              {
                                fChordPedals.push_back (pedal);
                              }

    // tuplet members
    /* JMI
    void                  applyTupletMemberDisplayedFactorToChordMembers (
                            int actualNotes, int normalNotes);
*/

    // position in measure
    void                  setChordFirstNotePositionInMeasure (
                            rational position);

    // finalization
    void                  finalizeChord (
                            int inputLineNumber);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    // as MSR string
    string                chordSoundingWholeNotesAsMsrString ();
    string                chordDisplayWholeNotesAsMsrString ();
 // JMI   string                chordGraphicDurationAsMsrString () const;

    string                asStringwithRawDivisions () const;

    string                asString () const;

    string                asShortString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // measure upLink
    S_msrMeasure          fChordMeasureUpLink;

    // sounding divisions
    rational              fChordSoundingWholeNotes;

    // display divisions
    rational              fChordDisplayWholeNotes;

    // graphic duration is needed for grace notes,
    // since they don't have any note (sounding) duration
    msrDurationKind       fChordGraphicDurationKind;

    vector<S_msrNote>     fChordNotesVector;

    // stems
    list<S_msrStem>       fChordStems;

    // beams
    list<S_msrBeam>       fChordBeams;

    // articulations
    list<S_msrArticulation>
                          fChordArticulations;

    // spanners
    list<S_msrSpanner>    fChordSpanners;

    // single tremolo
    S_msrSingleTremolo    fChordSingleTremolo;

    // double tremolo
    bool                  fChordIsFirstChordInADoubleTremolo;
    bool                  fChordIsSecondChordInADoubleTremolo;

    // technicals
    list<S_msrTechnical>  fChordTechnicals;

    list<S_msrTechnicalWithInteger>
                          fChordTechnicalWithIntegers;

    list<S_msrTechnicalWithFloat>
                          fChordTechnicalWithFloats;

    list<S_msrTechnicalWithString>
                          fChordTechnicalWithStrings;

    // ornaments
    list<S_msrOrnament>   fChordOrnaments;

    // glissandos
    list<S_msrGlissando>  fChordGlissandos;

    // slides
    list<S_msrSlide>      fChordSlides;

    // dynamics
    list<S_msrDynamics>   fChordDynamics;
    list<S_msrOtherDynamics>
                          fChordOtherDynamics;

    // slashes
    list<S_msrSlash>      fChordSlashes;

    // wedges
    list<S_msrWedge>      fChordWedges;

    // octave shift
    S_msrOctaveShift      fChordOctaveShift;

    // words
    list<S_msrWords>      fChordWords;

    // ties
    list<S_msrTie>        fChordTies;

    // slurs
    list<S_msrSlur>       fChordSlurs;

    // ligatures
    list<S_msrLigature>   fChordLigatures;

    // pedals
    list<S_msrPedal>      fChordPedals;

    // grace notes
    S_msrGraceNotesGroup  fChordGraceNotesGroupBefore;
    S_msrGraceNotesGroup  fChordGraceNotesGroupAfter;

    // harmony
    S_msrHarmony          fChordHarmony;

    // figured bass
    S_msrFiguredBass      fChordFiguredBass;
};
typedef SMARTP<msrChord> S_msrChord;
EXP ostream& operator<< (ostream& os, const S_msrChord& elt);

//______________________________________________________________________________
class msrTuplet : public msrTupletElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrTupletTypeKind {
      kTupletTypeNone,
      kTupletTypeStart, kTupletTypeContinue, kTupletTypeStop,
      kTupletTypeStartAndStopInARow };

    static string tupletTypeKindAsString (
      msrTupletTypeKind tupletTypeKind);

    enum msrTupletBracketKind {
      kTupletBracketYes, kTupletBracketNo};

    static string tupletBracketKindAsString (
      msrTupletBracketKind tupletBracketKind);

    enum msrTupletLineShapeKind {
      kTupletLineShapeStraight, kTupletLineShapeCurved};

    static string tupletLineShapeKindAsString (
      msrTupletLineShapeKind tupletLineShapeKind);

    enum msrTupletShowNumberKind {
      kTupletShowNumberActual, kTupletShowNumberBoth, kTupletShowNumberNone };

    static string tupletShowNumberKindAsString (
      msrTupletShowNumberKind tupletShowNumberKind);

    enum msrTupletShowTypeKind {
      kTupletShowTypeActual, kTupletShowTypeBoth, kTupletShowTypeNone };

    static string tupletShowTypeKindAsString (
      msrTupletShowTypeKind tupletShowTypeKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTuplet> create (
      int                     inputLineNumber,
      string                  tupletMeasureNumber,
      int                     tupletNumber,
      msrTupletBracketKind    tupletBracketKind,
      msrTupletLineShapeKind  tupletLineShapeKind,
      msrTupletShowNumberKind tupletShowNumberKind,
      msrTupletShowTypeKind   tupletShowTypeKind,
      msrTupletFactor         tupletFactor,
      rational                memberNotesSoundingWholeNotes,
      rational                memberNotesDisplayWholeNotes);

    SMARTP<msrTuplet> createTupletNewbornClone ();

    SMARTP<msrTuplet> createTupletDeepCopy ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTuplet (
      int                     inputLineNumber,
      string                  tupletMeasureNumber,
      int                     tupletNumber,
      msrTupletBracketKind    tupletBracketKind,
      msrTupletLineShapeKind  tupletLineShapeKind,
      msrTupletShowNumberKind tupletShowNumberKind,
      msrTupletShowTypeKind   tupletShowTypeKind,
      msrTupletFactor         tupletFactor,
      rational                memberNotesSoundingWholeNotes,
      rational                memberNotesDisplayWholeNotes);

    virtual ~msrTuplet ();

  public:

    // set and get
    // ------------------------------------------------------

    // tuplet uplink
    void                  setTupletTupletUpLink (
                            const S_msrTuplet& tuplet)
                              { fTupletTupletUpLink = tuplet; }

    S_msrTuplet           getTupletTupletUpLink () const
                              { return fTupletTupletUpLink; }

    // measure upLink
    void                  setTupletMeasureUpLink (
                            const S_msrMeasure& measure)
                              { fTupletMeasureUpLink = measure; }

    S_msrMeasure          getTupletMeasureUpLink () const
                              { return fTupletMeasureUpLink; }

    // positions in measures
    rational              setTupletMembersPositionInMeasure (
                            rational position);
                              // returns the position after the tuplet JMI ???

    int                   getTupletNumber () const
                              { return fTupletNumber; }

    msrTupletBracketKind  getTupletBracketKind () const
                              { return fTupletBracketKind; }

    msrTupletLineShapeKind
                          getTupletLineShapeKind () const
                              { return fTupletLineShapeKind; }

    msrTupletShowNumberKind
                          getTupletShowNumberKind () const
                              { return fTupletShowNumberKind; }

    msrTupletShowTypeKind getTupletShowTypeKind () const
                              { return fTupletShowTypeKind; }

    const msrTupletFactor&
                          getTupletFactor ()
                              { return fTupletFactor; }

    rational              getMemberNotesSoundingWholeNotes () const
                              { return fMemberNotesSoundingWholeNotes; }
    rational              getMemberNotesDisplayWholeNotes () const
                              { return fMemberNotesDisplayWholeNotes; }

/* JMI
    const list<S_msrTupletElement>&
                          getTupletElementsList () const
                              { return fTupletElementsList; }
                              */

    rational              getTupletSoundingWholeNotes () const
                              { return fTupletSoundingWholeNotes; }

    rational              getTupletDisplayWholeNotes () const
                              { return fTupletDisplayWholeNotes; }

    // services
    // ------------------------------------------------------

    void                  addNoteToTuplet (
                            S_msrNote  note,
                            S_msrVoice voice);

    void                  addChordToTuplet (S_msrChord chord);

    void                  addTupletToTuplet (S_msrTuplet tuplet);

    void                  addTupletToTupletClone (S_msrTuplet tuplet);

    S_msrNote             fetchTupletFirstNonGraceNote () const;

    S_msrNote             removeFirstNoteFromTuplet ( // JMI
                            int inputLineNumber);
    S_msrNote             removeLastNoteFromTuplet (
                            int inputLineNumber);

 // JMI   void                  applyDisplayFactorToTupletMembers ();

    void                  unapplySoundingFactorToTupletMembers (
                            const msrTupletFactor& containingTupletFactor);

    // finalization
    void                  finalizeTuplet (
                            int inputLineNumber);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

    virtual void          printShort (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // upLink
    S_msrMeasure          fTupletMeasureUpLink;
    S_msrTuplet           fTupletTupletUpLink;

    int                   fTupletNumber;

    msrTupletBracketKind  fTupletBracketKind;

    msrTupletLineShapeKind
                          fTupletLineShapeKind;

    msrTupletShowNumberKind
                          fTupletShowNumberKind;

    msrTupletShowTypeKind fTupletShowTypeKind;

    msrTupletFactor       fTupletFactor;

    rational              fMemberNotesSoundingWholeNotes;
    rational              fMemberNotesDisplayWholeNotes;

    rational              fTupletSoundingWholeNotes;
    rational              fTupletDisplayWholeNotes;

    list<S_msrTupletElement>
                          fTupletElementsList;
};
typedef SMARTP<msrTuplet> S_msrTuplet;
EXP ostream& operator<< (ostream& os, const S_msrTuplet& elt);

//______________________________________________________________________________
class msrStanza : public msrElement
{
  public:

    // constants
    // ------------------------------------------------------

    #define K_NO_STANZA_NUMBER "-1"
    #define K_NO_STANZA_NAME   "Unknown stanza"

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStanza> create (
      int           inputLineNumber,
      string        stanzaNumber,
      S_msrVoice    stanzaVoiceUpLink);

    SMARTP<msrStanza> createStanzaNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrStanza> createStanzaDeepCopy (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStanza (
      int           inputLineNumber,
      string        stanzaNumber,
      S_msrVoice    stanzaVoiceUpLink);

  public:

    virtual ~msrStanza ();

  private:

    // initialization
    // ------------------------------------------------------

    void                  initializeStanza ();

  public:

    // set and get
    // ------------------------------------------------------

    // number
    string                getStanzaNumber () const
                              { return fStanzaNumber; }

    // name
    string                getStanzaName () const
                              { return fStanzaName; }

    // contents
    const vector<S_msrSyllable>&
                          getSyllables () const
                              { return fSyllables; }

    void                  setStanzaTextPresent ()
                              { fStanzaTextPresent = true; }

    bool                  getStanzaTextPresent () const
                              { return fStanzaTextPresent; }

    // upLinks
    S_msrVoice            getStanzaVoiceUpLink () const
                              { return fStanzaVoiceUpLink; }

    // services
    // ------------------------------------------------------

    void                  appendSyllableToStanza (
                            S_msrSyllable syllable);

    S_msrSyllable         appendRestSyllableToStanza (
                            int      inputLineNumber,
                            rational wholeNotes);

    S_msrSyllable         appendSkipSyllableToStanza (
                            int      inputLineNumber,
                            rational wholeNotes);

    S_msrSyllable         appendMeasureEndSyllableToStanza (
                            int inputLineNumber);

    S_msrSyllable         appendMelismaSyllableToStanza (
                            int             inputLineNumber,
                            msrSyllable::msrSyllableKind
                                            syllableKind,
                            rational        wholeNote);

    S_msrSyllable         appendLineBreakSyllableToStanza (
                            int inputLineNumber);

/* JMI
    S_msrSyllable         appendTiedSyllableToStanza (
                            int      inputLineNumber,
                            rational wholeNotes);

    S_msrSyllable         appendSlurSyllableToStanza (
                            int      inputLineNumber,
                            rational wholeNotes);

    S_msrSyllable         appendSlurBeyondEndSyllableToStanza (
                            int      inputLineNumber,
                            rational wholeNotes);

    S_msrSyllable         appendLigatureSyllableToStanza (
                            int      inputLineNumber,
                            rational wholeNotes);

    S_msrSyllable         appendLigatureBeyondEndSyllableToStanza (
                            int      inputLineNumber,
                            rational wholeNotes);

    S_msrSyllable         appendBarNumberCheckSyllableToStanza (
                            int    inputLineNumber,
                            string nextMeasureNumber);

    S_msrSyllable         appendBarcheckSyllableToStanza (
                            int    inputLineNumber,
                            string nextMeasureNumber);
*/

    S_msrSyllable         appendLineBreakSyllableToStanza ( // JMI ???
                            int    inputLineNumber,
                            string nextMeasureNumber);

    S_msrSyllable         appendPageBreakSyllableToStanza ( // JMI ???
                            int    inputLineNumber,
                            string nextMeasureNumber);

    void                  padUpToActualMeasureWholeNotesInStanza ( // JMI
                            int      inputLineNumber,
                            rational wholeNotes);

    void                  appendPaddingNoteToStanza (
                            int inputLineNumber,
                            int divisions,
                            int divisionsPerQuarterNote);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fStanzaVoiceUpLink;

    // number
    // The lyric number indicates multiple lines,
    // though a name can be used as well (as in Finale's verse/chorus/section specification)
    string                fStanzaNumber;

    // name
    string                fStanzaName;

    // contents
    vector<S_msrSyllable> fSyllables;

    bool                  fStanzaTextPresent;

    // actual measure whole notes
    rational              fStanzaActualMeasureWholeNotes;
};
typedef SMARTP<msrStanza> S_msrStanza;
EXP ostream& operator<< (ostream& os, const S_msrStanza& elt);

//______________________________________________________________________________
/* JMI
class msrRepeatElement : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeatElement> create (
      int          inputLineNumber,
      S_msrRepeat  repeatUpLink);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRepeatElement (
      int          inputLineNumber,
      S_msrRepeat  repeatUpLink);

    virtual ~msrRepeatElement ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrRepeat           getRepeatElementRepeatUpLink () const
                              { return fRepeatElementRepeatUpLink; }

    // elements
    const list<S_msrVoiceElement>&
                          getRepeatElementElementsList ()
                              { return fRepeatElementElementsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendSegmentToRepeatElementsList (
                            int          inputLineNumber,
                            S_msrSegment segment,
                            string       context);

    void                  appendRepeatToRepeatElementsList (
                            int          inputLineNumber,
                            S_msrRepeat  repeat,
                            string       context);

    void                  appendMeasuresRepeatToRepeatElementsList (
                            int                 inputLineNumber,
                            S_msrMeasuresRepeat measuresRepeat,
                            string              context);

    void                  appendRestMeasuresToRepeatElementsList (
                            int                 inputLineNumber,
                            S_msrMeasuresRepeat measuresRepeat,
                            string              context);

    S_msrNote             fetchRepeatElementFirstNonGraceNote () const;

    void                  collectRepeatElementMeasuresIntoFlatList (
                            int inputLineNumber);

  private:

    // private services
    // ------------------------------------------------------

    void                  appendVoiceElementToRepeatElementsList ( // JMI
                            int               inputLineNumber,
                            S_msrVoiceElement voiceElement,
                            string            context);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

    virtual void          shortPrint (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrRepeat           fRepeatElementRepeatUpLink;

    // elements list
    list<S_msrVoiceElement>
                          fRepeatElementElementsList;
};
typedef SMARTP<msrRepeatElement> S_msrRepeatElement;
EXP ostream& operator<< (ostream& os, const S_msrRepeatElement& elt);
*/

//______________________________________________________________________________
class msrRepeatCommonPart : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeatCommonPart> create (
      int          inputLineNumber,
      S_msrRepeat  repeatUpLink);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRepeatCommonPart (
      int          inputLineNumber,
      S_msrRepeat  repeatUpLink);

    virtual ~msrRepeatCommonPart ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrRepeat           getRepeatCommonPartRepeatUpLink () const
                              { return fRepeatCommonPartRepeatUpLink; }

    // elements
    const list<S_msrVoiceElement>&
                          getRepeatCommonPartElementsList ()
                              { return fRepeatCommonPartElementsList; }

  public:

    // services
    // ------------------------------------------------------

    void                  appendVoiceElementToRepeatCommonPart (
                            int               inputLineNumber,
                            S_msrVoiceElement voiceElement,
                            string            context);

    void                  appendSegmentToRepeatCommonPart (
                            int          inputLineNumber,
                            S_msrSegment segment,
                            string       context);

    void                  appendRepeatToRepeatCommonPart (
                            int          inputLineNumber,
                            S_msrRepeat  repeat,
                            string       context);

    void                  appendMeasuresRepeatToRepeatCommonPart (
                            int                 inputLineNumber,
                            S_msrMeasuresRepeat measuresRepeat,
                            string              context);

    void                  appendRestMeasuresToRepeatCommonPart (
                            int               inputLineNumber,
                            S_msrRestMeasures restMeasures,
                            string            context);

    S_msrNote             fetchRepeatCommonPartFirstNonGraceNote () const;

    void                  collectRepeatCommonPartMeasuresIntoFlatList (
                            int inputLineNumber);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

    virtual void          shortPrint (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrRepeat           fRepeatCommonPartRepeatUpLink;

    // elements list
    list<S_msrVoiceElement>
                          fRepeatCommonPartElementsList;
};
typedef SMARTP<msrRepeatCommonPart> S_msrRepeatCommonPart;
EXP ostream& operator<< (ostream& os, const S_msrRepeatCommonPart& elt);

//______________________________________________________________________________
class msrRepeatEnding : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrRepeatEndingKind {
      kHookedEnding,
      kHooklessEnding};

    static string repeatEndingKindAsString (
      msrRepeatEndingKind repeatEndingKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeatEnding> create (
      int                 inputLineNumber,
      string              repeatEndingNumber, // may be "1, 2"
      msrRepeatEndingKind repeatEndingKind,
      S_msrRepeat         repeatUpLink);

    /* JMI
    SMARTP<msrRepeatEnding> createRepeatEndingNewbornClone (
      S_msrRepeat containingRepeat);
      */

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRepeatEnding (
      int                 inputLineNumber,
      string              repeatEndingNumber, // may be "1, 2"
      msrRepeatEndingKind repeatEndingKind,
      S_msrRepeat         repeatUpLink);

    virtual ~msrRepeatEnding ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrRepeat           getRepeatEndingRepeatUpLink () const
                              { return fRepeatEndingRepeatUpLink; }

    // numbers
    string                getRepeatEndingNumber () const
                              { return fRepeatEndingNumber; }

    void                  setRepeatEndingNumber (int repeatEndingNumber)
                              { fRepeatEndingNumber = repeatEndingNumber; }

    void                  setRepeatEndingInternalNumber (
                            int repeatEndingInternalNumber)
                              {
                                fRepeatEndingInternalNumber =
                                  repeatEndingInternalNumber;
                              }

    int                   getRepeatEndingInternalNumber () const
                              { return fRepeatEndingInternalNumber; }

    // kind
    msrRepeatEndingKind   getRepeatEndingKind () const
                              { return fRepeatEndingKind; }

    // elements
    const list<S_msrVoiceElement>&
                          getRepeatEndingElementsList ()
                              { return fRepeatEndingElementsList; }

    // services
    // ------------------------------------------------------

    void                  appendVoiceElementToRepeatEnding ( // JMI
                            int               inputLineNumber,
                            S_msrVoiceElement voiceElement,
                            string            context);

    void                  appendSegmentToRepeatEnding (
                            int          inputLineNumber,
                            S_msrSegment segment,
                            string       context);

    void                  appendRepeatToRepeatEnding (
                            int          inputLineNumber,
                            S_msrRepeat  repeat,
                            string       context);

    void                  appendMeasuresRepeatToRepeatEnding (
                            int                 inputLineNumber,
                            S_msrMeasuresRepeat measuresRepeat,
                            string              context);

    void                  appendRestMeasuresToRepeatEnding (
                            int                       inputLineNumber,
                            S_msrRestMeasures restMeasures,
                            string                    context);

    void                  collectRepeatEndingMeasuresIntoFlatList (
                            int inputLineNumber);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

    virtual void          shortPrint (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrRepeat           fRepeatEndingRepeatUpLink;

    // numbers
    string                fRepeatEndingNumber; // may be "1, 2"
    int                   fRepeatEndingInternalNumber; // internally assigned

    // kind
    msrRepeatEndingKind   fRepeatEndingKind;

    // elements list
    list<S_msrVoiceElement>
                          fRepeatEndingElementsList;
};
typedef SMARTP<msrRepeatEnding> S_msrRepeatEnding;
EXP ostream& operator<< (ostream& os, const S_msrRepeatEnding& elt);

//______________________________________________________________________________
class msrRepeat : public msrVoiceElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrRepeatExplicitStartKind {
      kRepeatExplicitStartNo,
      kRepeatExplicitStartYes };

    static string repeatExplicitStartKindAsString (
      msrRepeatExplicitStartKind repeatExplicitStartKind);

    enum msrRepeatBuildPhaseKind {
      kRepeatBuildPhaseJustCreated,
      kRepeatBuildPhaseInCommonPart,
      kRepeatBuildPhaseInEndings,
      kRepeatBuildPhaseCompleted};

    static string repeatBuildPhaseKindAsString (
      msrRepeatBuildPhaseKind repeatBuildPhaseKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeat> create (
      int        inputLineNumber,
      int        repeatTimes,
      S_msrVoice voiceUpLink);

    SMARTP<msrRepeat> createRepeatNewbornClone (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRepeat (
      int        inputLineNumber,
      int        repeatTimes,
      S_msrVoice voiceUpLink);

    virtual ~msrRepeat ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            getRepeatVoiceUpLink () const
                              { return fRepeatVoiceUpLink; }
    // times
    int                   getRepeatTimes () const
                              { return fRepeatTimes; }

    void                  setRepeatTimes (int repeatTimes) // JMI
                              { fRepeatTimes = repeatTimes; }

    // implicit start?
    void                  setRepeatExplicitStartKind (
                            msrRepeatExplicitStartKind repeatExplicitStartKind)
                              {
                                fRepeatExplicitStartKind =
                                  repeatExplicitStartKind;
                              }

    msrRepeatExplicitStartKind
                          getRepeatExplicitStartKind () const
                              { return fRepeatExplicitStartKind; }

    // common part
    void                  setRepeatCommonPart (
                            S_msrRepeatCommonPart repeatCommonPart);

    S_msrRepeatCommonPart getRepeatCommonPart () const
                              { return fRepeatCommonPart; }

    // endings
    const vector<S_msrRepeatEnding>&
                          getRepeatEndings () const
                              { return fRepeatEndings; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addRepeatEndingToRepeat (
                            int               inputLineNumber,
                            S_msrRepeatEnding repeatEnding);

    void                  appendSegmentToRepeat (
                            int          inputLineNumber,
                            S_msrSegment segment,
                            string       context);

    void                  appendRepeatToRepeat (
                            int         inputLineNumber,
                            S_msrRepeat repeat,
                            string      context);

    void                  appendMeasuresRepeatToRepeat (
                            int                 inputLineNumber,
                            S_msrMeasuresRepeat measuresRepeat,
                            string              context);

    void                  appendRestMeasuresToRepeat (
                            int                       inputLineNumber,
                            S_msrRestMeasures restMeasures,
                            string                    context);

    S_msrNote             fetchRepeatFirstNonGraceNote () const;

    void                  collectRepeatMeasuresIntoFlatList (
                            int inputLineNumber);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortString () const;
    string                asString () const;

    void                  displayRepeat (
                            int    inputLineNumber,
                            string context);

    virtual void          print (ostream& os);

    virtual void          shortPrint (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fRepeatVoiceUpLink;

    // number of repetitions
    int                   fRepeatTimes;

    // explicit start?
    msrRepeatExplicitStartKind
                          fRepeatExplicitStartKind;

    // common part
    S_msrRepeatCommonPart fRepeatCommonPart;

    // repeat endings
    vector<S_msrRepeatEnding>
                          fRepeatEndings;
    int                   fRepeatEndingsInternalCounter;

  public:

    // public work services
    // ------------------------------------------------------

    // repeat build phase
    void                  setCurrentRepeatBuildPhaseKind (
                            msrRepeatBuildPhaseKind repeatBuildPhaseKind)
                              {
                                fCurrentRepeatBuildPhaseKind =
                                  repeatBuildPhaseKind;
                              }

    msrRepeatBuildPhaseKind
                          getCurrentRepeatBuildPhaseKind () const
                            { return fCurrentRepeatBuildPhaseKind; }

  private:

    // private work services
    // ------------------------------------------------------

    void                  printMeasurePendingMeasureElementsList ();

  private:

    // work fields
    // ------------------------------------------------------

    // repeat build phase, used when building the repeat
    msrRepeatBuildPhaseKind
                          fCurrentRepeatBuildPhaseKind;
};
typedef SMARTP<msrRepeat> S_msrRepeat;
EXP ostream& operator<< (ostream& os, const S_msrRepeat& elt);

//______________________________________________________________________________
/* JMI
class msrMeasuresRepeatElement : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasuresRepeatElement> create (
      int                 inputLineNumber,
      S_msrMeasuresRepeat measuresRepeatUpLink);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMeasuresRepeatElement (
      int                 inputLineNumber,
      S_msrMeasuresRepeat measuresRepeatUpLink);

    virtual ~msrMeasuresRepeatElement ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrMeasuresRepeat   getMeasuresRepeatElementMeasuresRepeatUpLink () const
                              { return fMeasuresRepeatElementMeasuresRepeatUpLink; }

    // elements
    const list<S_msrVoiceElement>&
                          getMeasuresRepeatElementElementsList ()
                              { return fMeasuresRepeatElementElementsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendSegmentToMeasuresRepeatElementsList (
                            int          inputLineNumber,
                            S_msrSegment segment,
                            string       context);

    void                  appendMeasuresRepeatToMeasuresRepeatElementsList (
                            int                  inputLineNumber,
                            S_msrMeasuresRepeat  measuresRepeat,
                            string               context);


    void                  appendRestMeasuresToMeasuresRepeatElementsList (
                            int                         inputLineNumber,
                            S_msrMeasuresRepeat measuresRepeat,
                            string                      context);

    S_msrNote             fetchMeasuresRepeatElementFirstNonGraceNote () const;

    void                  collectMeasuresRepeatElementMeasuresIntoFlatList (
                            int inputLineNumber);

  private:

    // private services
    // ------------------------------------------------------

    void                  appendVoiceElementToMeasuresRepeatElementsList ( // JMI
                            int               inputLineNumber,
                            S_msrVoiceElement voiceElement,
                            string            context);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

    virtual void          shortPrint (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrMeasuresRepeat   fMeasuresRepeatElementMeasuresRepeatUpLink;

    // elements list
    list<S_msrVoiceElement>
                          fMeasuresRepeatElementElementsList;
};
typedef SMARTP<msrMeasuresRepeatElement> S_msrMeasuresRepeatElement;
EXP ostream& operator<< (ostream& os, const S_msrMeasuresRepeatElement& elt);
*/

//______________________________________________________________________________
class msrMeasuresRepeatPattern : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasuresRepeatPattern> create (
      int                 inputLineNumber,
      S_msrMeasuresRepeat measuresRepeatUpLink);

    /* JMI
    SMARTP<msrMeasuresRepeatPattern> createMeasuresRepeatPatternNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrMeasuresRepeatPattern> createMeasuresRepeatPatternDeepCopy (
      S_msrVoice containingVoice);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMeasuresRepeatPattern (
      int                 inputLineNumber,
      S_msrMeasuresRepeat measuresRepeatUpLink);

    virtual ~msrMeasuresRepeatPattern ();

  public:

    // set and get
    // ------------------------------------------------------

    // segment
    void                  setMeasuresRepeatPatternSegment (
                            S_msrSegment measuresRepeatPatternSegment);

    S_msrSegment          getMeasuresRepeatPatternSegment () const
                              { return fMeasuresRepeatPatternSegment; }

    // upLinks
    S_msrMeasuresRepeat   getMeasuresRepeatUpLink () const
                            { return fMeasuresRepeatUpLink; }

    // services
    // ------------------------------------------------------

    int                   measuresRepeatPatternMeasuresNumber () const;

    void                  collectRepeatPatternMeasuresIntoFlatList (
                            int inputLineNumber);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrMeasuresRepeat   fMeasuresRepeatUpLink;

    // segment
    S_msrSegment          fMeasuresRepeatPatternSegment;
};
typedef SMARTP<msrMeasuresRepeatPattern> S_msrMeasuresRepeatPattern;
EXP ostream& operator<< (ostream& os, const S_msrMeasuresRepeatPattern& elt);

//______________________________________________________________________________
class msrMeasuresRepeatReplicas : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasuresRepeatReplicas> create (
      int                 inputLineNumber,
      S_msrMeasuresRepeat measuresRepeatUpLink);

    /* JMI
    SMARTP<msrMeasuresRepeatReplicas> createMeasuresRepeatReplicasNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrMeasuresRepeatReplicas> createMeasuresRepeatReplicasDeepCopy (
      S_msrVoice containingVoice);
      */

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMeasuresRepeatReplicas (
      int                 inputLineNumber,
      S_msrMeasuresRepeat measuresRepeatUpLink);

    virtual ~msrMeasuresRepeatReplicas ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrMeasuresRepeat   getMeasuresRepeatUpLink () const
                            { return fMeasuresRepeatUpLink; }
    // segment
    void                  setMeasuresRepeatReplicasSegment (
                            S_msrSegment measuresRepeatReplicasSegment);

    S_msrSegment          getMeasuresRepeatReplicasSegment () const
                              { return fMeasuresRepeatReplicasSegment; }

    // services
    // ------------------------------------------------------

    int                   measuresRepeatReplicasMeasuresNumber () const;

    int                   measuresRepeatReplicasNumber () const;

    void                  collectRepeatReplicasMeasuresIntoFlatList (
                            int inputLineNumber);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrMeasuresRepeat   fMeasuresRepeatUpLink;

    // segment
    S_msrSegment          fMeasuresRepeatReplicasSegment;
};
typedef SMARTP<msrMeasuresRepeatReplicas> S_msrMeasuresRepeatReplicas;
EXP ostream& operator<< (ostream& os, const S_msrMeasuresRepeatReplicas& elt);

//______________________________________________________________________________
class msrMeasuresRepeat : public msrVoiceElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrMeasuresRepeatKind {
      k_NoMeasuresRepeat,
      kStartMeasuresRepeat, kStopMeasuresRepeat };

    static string measuresRepeatKindAsString (
      msrMeasuresRepeatKind measuresRepeatKind);

    enum msrMeasuresRepeatBuildPhaseKind {
      kMeasuresRepeatBuildPhaseJustCreated,
      kMeasuresRepeatBuildPhaseInPattern,
      kMeasuresRepeatBuildPhaseInReplicas,
      kMeasuresRepeatBuildPhaseCompleted};

    static string measuresRepeatBuildPhaseKindAsString (
      msrMeasuresRepeatBuildPhaseKind measuresRepeatBuildPhaseKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasuresRepeat> create (
      int        inputLineNumber,
      int        measuresRepeatMeasuresNumber,
      int        measuresRepeatSlashesNumber,
      S_msrVoice voiceUpLink);

    SMARTP<msrMeasuresRepeat> createMeasuresRepeatNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrMeasuresRepeat> createMeasuresRepeatDeepCopy (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMeasuresRepeat (
      int        inputLineNumber,
      int        measuresRepeatMeasuresNumber,
      int        measuresRepeatSlashesNumber,
      S_msrVoice voiceUpLink);

    virtual ~msrMeasuresRepeat ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            getMeasuresRepeatVoiceUpLink () const
                            { return fMeasuresRepeatVoiceUpLink; }

    // numbers
    int                   getMeasuresRepeatMeasuresNumber () const
                              { return fMeasuresRepeatMeasuresNumber; }

    int                   getMeasuresRepeatSlashesNumber () const
                              { return fMeasuresRepeatSlashesNumber; }

    // measuresRepeat pattern
    void                  setMeasuresRepeatPattern (
                            S_msrMeasuresRepeatPattern
                              measuresRepeatPattern);

    S_msrMeasuresRepeatPattern
                          getMeasuresRepeatPattern () const
                              { return fMeasuresRepeatPattern; }

    // measures repeat replicas
    void                  setMeasuresRepeatReplicas (
                            S_msrMeasuresRepeatReplicas
                              measuresRepeatReplicas);

    S_msrMeasuresRepeatReplicas
                          getMeasuresRepeatReplicas () const
                              { return fMeasuresRepeatReplicas; }

    // measures repeat build phase
    void                  setCurrentMeasuresRepeatBuildPhaseKind (
                            msrMeasuresRepeatBuildPhaseKind
                              measuresRepeatBuildPhaseKind)
                              {
                                fCurrentMeasuresRepeatBuildPhaseKind =
                                  measuresRepeatBuildPhaseKind;
                              }

    msrMeasuresRepeatBuildPhaseKind
                          getCurrentMeasuresRepeatBuildPhaseKind () const
                            { return fCurrentMeasuresRepeatBuildPhaseKind; }

    // services
    // ------------------------------------------------------

    int                   measuresRepeatPatternMeasuresNumber () const
                            {
                              return
                                fMeasuresRepeatPattern->
                                  measuresRepeatPatternMeasuresNumber ();
                            }

    int                   measuresRepeatReplicasMeasuresNumber () const
                            {
                              return
                                fMeasuresRepeatReplicas->
                                  measuresRepeatReplicasMeasuresNumber ();
                            }

    int                   measuresRepeatReplicasNumber () const;

    void                  collectMeasuresRepeatMeasuresIntoFlatList (
                            int inputLineNumber);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  displayMeasuresRepeat (
                            int    inputLineNumber,
                            string context);

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fMeasuresRepeatVoiceUpLink;

    // numbers
    int                   fMeasuresRepeatMeasuresNumber;
    int                   fMeasuresRepeatSlashesNumber;

    // measures repeat pattern
    S_msrMeasuresRepeatPattern
                          fMeasuresRepeatPattern;

    // measures repeat replicas
    S_msrMeasuresRepeatReplicas
                          fMeasuresRepeatReplicas;

    // measures repeat build phase, used when building the measures repeat
    msrMeasuresRepeatBuildPhaseKind
                          fCurrentMeasuresRepeatBuildPhaseKind;
};
typedef SMARTP<msrMeasuresRepeat> S_msrMeasuresRepeat;
EXP ostream& operator<< (ostream& os, const S_msrMeasuresRepeat& elt);

//______________________________________________________________________________
class msrRestMeasuresContents : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRestMeasuresContents> create (
      int               inputLineNumber,
      S_msrRestMeasures restMeasures);

    SMARTP<msrRestMeasuresContents> createRestMeasuresContentsNewbornClone (
      S_msrRestMeasures restMeasures);

    SMARTP<msrRestMeasuresContents> createRestMeasuresContentsDeepCopy (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRestMeasuresContents (
      int               inputLineNumber,
      S_msrRestMeasures restMeasures);

    virtual ~msrRestMeasuresContents ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLink
    S_msrRestMeasures
                          getRestMeasuresContentsRestMeasuresUpLink () const
                            {
                              return fRestMeasuresContentsRestMeasuresUpLink;
                            }

    void                  setRestMeasuresContentsSegment (
                            int          inputLineNumber,
                            S_msrSegment restMeasuresContentsSegment);

    S_msrSegment          getRestMeasuresContentsSegment () const
                              { return fRestMeasuresContentsSegment; }

    // services
    // ------------------------------------------------------

    int                   restMeasuresContentsMeasuresNumber () const;

    void                  collectRestMeasuresContentsMeasuresIntoFlatList (
                            int inputLineNumber);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // upLink
    S_msrRestMeasures     fRestMeasuresContentsRestMeasuresUpLink;

    S_msrSegment          fRestMeasuresContentsSegment;
};
typedef SMARTP<msrRestMeasuresContents> S_msrRestMeasuresContents;
EXP ostream& operator<< (ostream& os, const S_msrRestMeasuresContents& elt);

//______________________________________________________________________________
class msrRestMeasures : public msrVoiceElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrRestMeasuresKind {
      kStartRestMeasures, kStopRestMeasures,
      k_NoRestMeasures };

    static string restMeasuresKindAsString (
      msrRestMeasuresKind restMeasuresKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRestMeasures> create (
      int        inputLineNumber,
      rational   restMeasuresMeasureSoundingNotes,
      int        restMeasuresNumber,
      S_msrVoice voiceUpLink);

    SMARTP<msrRestMeasures> createRestMeasuresNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrRestMeasures> createRestMeasuresDeepCopy (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRestMeasures (
      int        inputLineNumber,
      rational   restMeasuresMeasureSoundingNotes,
      int        restMeasuresNumber,
      S_msrVoice voiceUpLink);

    virtual ~msrRestMeasures ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrVoice            getRestMeasuresVoiceUpLink () const
                            { return fRestMeasuresVoiceUpLink; }

    int                   getRestMeasuresNumber () const
                              { return fRestMeasuresNumber; }

    void                  setRestMeasuresContents (
                            S_msrRestMeasuresContents
                              restMeasuresContents);

    S_msrRestMeasuresContents
                          getRestMeasuresContents () const
                              { return fRestMeasuresContents; }

    void                  setRestMeasuresNextMeasureNumber (
                            string nextMeasureNumber);

    string                getRestMeasuresNextMeasureNumber () const
                              { return fRestMeasuresNextMeasureNumber; }

    void                  setRestMeasuresLastMeasurePuristMeasureNumber (
                            int inputLineNumber);

    int                   getRestMeasuresLastMeasurePuristMeasureNumber () const
                              { return fRestMeasuresLastMeasurePuristNumber; }

    rational              getRestMeasuresMeasureSoundingNotes () const
                              { return fRestMeasuresMeasureSoundingNotes; }

    // services
    // ------------------------------------------------------

    int                   restMeasuresContentsMeasuresNumber () const
                            {
                              return
                                fRestMeasuresContents->
                                  restMeasuresContentsMeasuresNumber ();
                            }

    void                  collectRestMeasuresIntoFlatList (
                            int inputLineNumber);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  displayRestMeasures (
                            int    inputLineNumber,
                            string context);

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    S_msrVoice            fRestMeasuresVoiceUpLink;

    int                   fRestMeasuresNumber;

    S_msrRestMeasuresContents
                          fRestMeasuresContents;

    string                fRestMeasuresNextMeasureNumber;
    int                   fRestMeasuresLastMeasurePuristNumber;

    // shortcut for efficiency
    rational              fRestMeasuresMeasureSoundingNotes;
};
typedef SMARTP<msrRestMeasures> S_msrRestMeasures;
EXP ostream& operator<< (ostream& os, const S_msrRestMeasures& elt);

//______________________________________________________________________________
class msrRepeatCoda : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeatCoda> create (
      int          inputLineNumber,
      S_msrSegment segment,
      S_msrRepeat  repeatUpLink);

    SMARTP<msrRepeatCoda> createRepeatCodaNewbornClone (
      S_msrRepeat containingRepeat);

    SMARTP<msrRepeatCoda> createRepeatCodaDeepCopy (
      S_msrRepeat containingRepeat);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRepeatCoda (
      int          inputLineNumber,
      S_msrSegment segment,
      S_msrRepeat  repeatUpLink);

    virtual ~msrRepeatCoda ();

  public:

    // set and get
    // ------------------------------------------------------

    // segment
    S_msrSegment          getRepeatCodaSegment () const
                              { return fRepeatCodaSegment; }

    // upLinks
    S_msrRepeat           getRepeatCodaRepeatUpLink () const
                              { return fRepeatCodaRepeatUpLink; }

    // services
    // ------------------------------------------------------

 //  JMI void                  appendElementToRepeatCoda (S_msrElement elem);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // segment
    S_msrSegment          fRepeatCodaSegment;

    // upLinks
    S_msrRepeat           fRepeatCodaRepeatUpLink;
};
typedef SMARTP<msrRepeatCoda> S_msrRepeatCoda;
EXP ostream& operator<< (ostream& os, const S_msrRepeatCoda& elt);

//________________________________________________________________________
struct msrRepeatDescr : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrRepeatDescr> create (
      int         repeatDescrStartInputLineNumber,
      S_msrRepeat repeatDescrRepeat);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRepeatDescr (
      int         repeatDescrStartInputLineNumber,
      S_msrRepeat fRepeatDescrRepeat);

    virtual ~msrRepeatDescr ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrRepeat           getRepeatDescrRepeat () const
                              { return fRepeatDescrRepeat; }

    void                  setRepeatDescrStartInputLineNumber (
                            int inputLineNumber)
                              {
                                fRepeatDescrStartInputLineNumber =
                                  inputLineNumber;
                              }

    int                   getRepeatDescrStartInputLineNumber () const
                              {
                                return
                                  fRepeatDescrStartInputLineNumber;
                              }

    // services
    // ------------------------------------------------------

    string                repeatDescrAsString () const;

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    // the repeat
    S_msrRepeat           fRepeatDescrRepeat;

    // its start input line number
    int                   fRepeatDescrStartInputLineNumber;
};
typedef SMARTP<msrRepeatDescr> S_msrRepeatDescr;
EXP ostream& operator<< (ostream& os, const S_msrRepeatDescr& elt);

//______________________________________________________________________________
class msrStaff;
typedef SMARTP<msrStaff> S_msrStaff;

//______________________________________________________________________________
class msrVoice : public msrElement
{
  public:

    // constants
    // ------------------------------------------------------

    #define K_NO_VOICE_NUMBER                     -99
    #define K_VOICE_HARMONY_VOICE_BASE_NUMBER      20
    #define K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER 40

    // data types
    // ------------------------------------------------------

    enum msrVoiceKind {
      kVoiceRegular,
      kVoiceHarmony,       // for MusicXML <harmony/>, LilyPond ChordNames
      kVoiceFiguredBass }; // for MusicXML <figured-bass/>, LilyPond FiguredBass

    static string voiceKindAsString (
      msrVoiceKind voiceKind);

    enum msrVoiceRepeatPhaseKind {
        kVoiceRepeatPhaseNone,
        kVoiceRepeatPhaseAfterCommonPart,
        kVoiceRepeatPhaseAfterHookedEnding,
        kVoiceRepeatPhaseAfterHooklessEnding };

    static string voiceRepeatPhaseKindAsString (
      msrVoiceRepeatPhaseKind
        afterRepeatComponentPhaseKind);

    enum msrVoiceFinalizationStatusKind { // JMI ???
      kKeepVoice,
      kEraseVoice };

    static string voiceFinalizationStatusKindAsString (
      msrVoiceFinalizationStatusKind voiceFinalizationStatusKind);

    enum msrVoiceCreateInitialLastSegmentKind {
      kCreateInitialLastSegmentYes, kCreateInitialLastSegmentNo };

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrVoice> create (
      int          inputLineNumber,
      msrVoiceKind voiceKind,
      int          voiceNumber,
      msrVoiceCreateInitialLastSegmentKind
                   voiceCreateInitialLastSegmentKind,
      S_msrStaff   voiceStaffUpLink);

    SMARTP<msrVoice> createVoiceNewbornClone (
      S_msrStaff containingStaff);

    SMARTP<msrVoice> createVoiceDeepCopy (
      int          inputLineNumber,
      msrVoiceKind voiceKind,
      int          voiceNumber,
      S_msrStaff   containingStaff);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    // for regular voices
    msrVoice (
      int          inputLineNumber,
      msrVoiceKind voiceKind,
      int          voiceNumber,
      msrVoiceCreateInitialLastSegmentKind
                   voiceCreateInitialLastSegmentKind,
      S_msrStaff   voiceStaffUpLink);

    // destructor
    virtual ~msrVoice ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeVoice (
                            msrVoiceCreateInitialLastSegmentKind
                             voiceCreateInitialLastSegmentKind);

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks

    S_msrStaff            getVoiceStaffUpLink () const
                              { return fVoiceStaffUpLink; }

    // voice kind

    void                  setVoiceKind (msrVoiceKind voiceKind)
                            { fVoiceKind = voiceKind; }

    msrVoiceKind          getVoiceKind () const
                              { return fVoiceKind; }

    // voice numbers

    void                  setVoiceNumber (int voiceNumber)
                              { fVoiceNumber = voiceNumber; }

    int                   getVoiceNumber () const
                              { return fVoiceNumber; }

    void                  setRegularVoiceStaffSequentialNumber (
                            int regularVoiceStaffSequentialNumber)
                              {
                                fRegularVoiceStaffSequentialNumber =
                                  regularVoiceStaffSequentialNumber;
                              }

    int                   getRegularVoiceStaffSequentialNumber () const
                              { return fRegularVoiceStaffSequentialNumber; }

    // voice name

    void                  setVoiceNameFromNumber (
                            int inputLineNumber,
                            int voiceNumber);

    string                getVoiceName () const
                              { return fVoiceName; }

    // harmonies

    S_msrVoice            getHarmonyVoiceForRegularVoiceForwardLink () const
                              { return fHarmonyVoiceForRegularVoiceForwardLink; }

    S_msrVoice            getRegularVoiceForHarmonyVoiceBackwardLink () const
                              { return fRegularVoiceForHarmonyVoiceBackwardLink; }

    // figured bass

    S_msrVoice            getFiguredBassVoiceForRegularVoiceForwardLink () const
                              { return fFiguredBassVoiceForRegularVoiceForwardLink; }

    S_msrVoice            getRegularVoiceForFiguredBassVoiceBackwardLink () const
                              { return fRegularVoiceForFiguredBassVoiceBackwardLink; }

    // stanzas

    const map<string, S_msrStanza>&
                          getVoiceStanzasMap () const
                              { return fVoiceStanzasMap; }

    // voice last segment

    void                  setVoiceLastSegmentInVoiceClone (
                            S_msrSegment segment);

    void                  appendSegmentToVoiceClone (
                            S_msrSegment segment);

    S_msrSegment          getVoiceLastSegment () const
                              { return fVoiceLastSegment; }

    // voice last appended note

    S_msrNote             getVoiceLastAppendedNote () const
                              { return fVoiceLastAppendedNote; }

    // voice shortest note

    rational              getVoiceShortestNoteDuration () const
                              { return fVoiceShortestNoteDuration; }

    rational              getVoiceShortestNoteTupletFactor () const
                              { return fVoiceShortestNoteTupletFactor; }

    // counters

    int                   getVoiceActualNotesCounter () const
                              { return fVoiceActualNotesCounter; }

    int                   getVoiceRestsCounter () const
                              { return fVoiceRestsCounter; }

    int                   getVoiceSkipsCounter () const
                              { return fVoiceSkipsCounter; }

    int                   getVoiceActualHarmoniesCounter () const
                              { return fVoiceActualHarmoniesCounter; }

    int                   getVoiceActualFiguredBassesCounter () const
                              { return fVoiceActualFiguredBassesCounter; }

    // has music been inserted in the voice?

    void                  setMusicHasBeenInsertedInVoice ()
                              { fMusicHasBeenInsertedInVoice = true; }

    bool                  getMusicHasBeenInsertedInVoice () const
                              { return fMusicHasBeenInsertedInVoice; }

    // regular measure ends detection

    void                  setWholeNotesSinceLastRegularMeasureEnd (
                            int      inputLineNumber,
                            rational value);

    rational              getWholeNotesSinceLastRegularMeasureEnd () const
                              { return fWholeNotesSinceLastRegularMeasureEnd; }

    // incomplete measures after repeats detection

    void                  setCurrentVoiceRepeatPhaseKind (
                            int      inputLineNumber,
                            msrVoiceRepeatPhaseKind
                                     afterRepeatComponentPhaseKind);

    msrVoiceRepeatPhaseKind
                          getCurrentVoiceRepeatPhaseKind () const
                              { return fCurrentVoiceRepeatPhaseKind; }


    // rests measures

    void                  setVoiceContainsRestMeasures (
                            int inputLineNumber);

    bool                  getVoiceContainsRestMeasures () const
                              { return fVoiceContainsRestMeasures; }

    // measures repeats

    void                  setVoiceContainsMeasuresRepeats (
                            int inputLineNumber);

    bool                  getVoiceContainsMeasuresRepeats () const
                              { return fVoiceContainsMeasuresRepeats; }

    // measures flat list
    const list<S_msrMeasure>&
                          getVoiceMeasuresFlatList () const
                              { return fVoiceMeasuresFlatList; }

  public:

    // public services
    // ------------------------------------------------------

    // upLinks

    S_msrPart             fetchVoicePartUpLink () const;

    // identity

    void                  changeVoiceIdentity ( // after a deep copy
                            int voiceNumber);

    // measures

    S_msrMeasure          createMeasureAndAppendItToVoice (
                            int    inputLineNumber,
                            string measureNumber,
                            msrMeasure::msrMeasureImplicitKind
                                   measureImplicitKind);

    void                  setNextMeasureNumberInVoice (
                            int    inputLineNumber,
                            string nextMeasureNumber);

    void                  padUpToActualMeasureWholeNotesInVoice (
                            int      inputLineNumber,
                            rational wholeNotes);

    void                  appendPaddingNoteToVoice ( // for <forward />
                            int inputLineNumber,
                            int divisions,
                            int divisionsPerQuarterNote);

    // backup

    void                  handleBackup (
                            int inputLineNumber,
                            int divisions,
                            int divisionsPerQuarterNote);

    // clef, key, time

    void                  appendClefToVoice (S_msrClef clef);

    void                  appendKeyToVoice  (S_msrKey  key);

    void                  appendTimeToVoice (S_msrTime time);
    void                  appendTimeToVoiceClone (S_msrTime time);

    // notes

    S_msrNote             fetchVoiceFirstNonGraceNote () const;

    void                  registerShortestNoteIfRelevant (S_msrNote note);

    void                  registerNoteAsVoiceLastAppendedNote (S_msrNote note);

    // harmonies

    S_msrVoice            createHarmonyVoiceForRegularVoice (
                            int    inputLineNumber,
                            string currentMeasureNumber);

    // figured bass

    S_msrVoice            createFiguredBassVoiceForRegularVoice (
                            int    inputLineNumber,
                            string currentMeasureNumber);

    // transpose

    void                  appendTransposeToVoice (
                            S_msrTranspose transpose);

    // part name display

    void                  appendPartNameDisplayToVoice (
                            S_msrPartNameDisplay partNameDisplay);

    // part abbreviation display

    void                  appendPartAbbreviationDisplayToVoice (
                            S_msrPartAbbreviationDisplay partAbbreviationDisplay);

    // staff details

    void                  appendStaffDetailsToVoice (
                            S_msrStaffDetails staffDetails);

    // tempo

    void                  appendTempoToVoice (S_msrTempo tempo);

    // rehearsals

    void                  appendRehearsalToVoice (
                            S_msrRehearsal rehearsal);

    // octave shifts

    void                  appendOctaveShiftToVoice (
                            S_msrOctaveShift octaveShift);

    // scordaturas

    void                  appendScordaturaToVoice (
                            S_msrScordatura scordatura);

    // accordion registration

    void                  appendAccordionRegistrationToVoice (
                            S_msrAccordionRegistration
                              accordionRegistration);

    // harp pedals tuning

    void                  appendHarpPedalsTuningToVoice (
                            S_msrHarpPedalsTuning
                              harpPedalsTuning);

    // staff change

    void                  appendVoiceStaffChangeToVoice (
                            S_msrVoiceStaffChange voiceStaffChange);

    // notes

    void                  appendNoteToVoice      (S_msrNote note);
    void                  appendNoteToVoiceClone (S_msrNote note);

    // tremolos

    void                  appendDoubleTremoloToVoice (
                            S_msrDoubleTremolo doubleTremolo);

    // chords

    void                  appendChordToVoice  (S_msrChord chord);

    // tuplets

    void                  appendTupletToVoice (S_msrTuplet tuplet);

    // harmonies

    void                  appendHarmonyToVoice (
                            S_msrHarmony harmony);

    void                  appendHarmonyToVoiceClone (
                            S_msrHarmony harmony);

    // figured bass

    void                  appendFiguredBassToVoice (
                            S_msrFiguredBass figuredBass);

    void                  appendFiguredBassToVoiceClone (
                            S_msrFiguredBass figuredBass);

    // frames

    void                  appendFrameToVoice (
                            S_msrFrame frame);

    void                  appendFrameToVoiceClone (
                            S_msrFrame frame);

    // grace notes

    void                  addGraceNotesGroupBeforeAheadOfVoiceIfNeeded (
                            S_msrGraceNotesGroup graceNotesGroup);

    /*
    void                  appendGraceNotesToVoice (
                            S_msrGraceNotes graceNotes);

    void                  appendAfterGraceNotesToVoice (
                            S_msrAfterGraceNotes afterGraceNotes);

    void                  prependAfterGraceNotesToVoice (
                            S_msrAfterGraceNotes afterGraceNotes);
                            */

    // lyrics

    void                  appendSyllableToVoice ( // JMI
                            int           inputLineNumber,
                            string        stanzaNumber,
                            string        stanzaName,
                            S_msrSyllable syllable);

    // bar checks

    void                  appendBarCheckToVoice (
                            S_msrBarCheck barCheck);

    // bar number checks

    void                  appendBarNumberCheckToVoice (
                            S_msrBarNumberCheck barNumberCheck);

    // breaks

    void                  appendLineBreakToVoice (S_msrLineBreak lineBreak);
    void                  appendPageBreakToVoice (S_msrPageBreak pageBreak);

    // bar lines

    void                  prependBarlineToVoice (S_msrBarline barline);
    void                  appendBarlineToVoice (S_msrBarline barline);

    // segno

    void                  appendSegnoToVoice (S_msrSegno segno);

    // coda

    void                  appendCodaToVoice (S_msrCoda coda);

    // eyeglasses

    void                  appendEyeGlassesToVoice (
                            S_msrEyeGlasses eyeGlasses);

    // pedal

    void                  appendPedalToVoice (S_msrPedal pedal);

    // damp

    void                  appendDampToVoice (S_msrDamp damp);

    // damp all

    void                  appendDampAllToVoice (S_msrDampAll dampAll);

    // other elements

    void                  prependOtherElementToVoice (
                            S_msrMeasureElement elem);
    void                  appendOtherElementToVoice (
                            S_msrMeasureElement elem);
                            // for other types of elements not known
                            // in this header file, such as LPSR elements

    // last measure in voice

    S_msrMeasure          fetchVoiceLastMeasure (
                            int inputLineNumber) const;

    // last element in voice

    S_msrMeasureElement   fetchVoiceLastElement ( // JMI ???
                            int inputLineNumber) const;

    // removing elements from voice

    void                  removeNoteFromVoice (
                            int       inputLineNumber,
                            S_msrNote note);

    void                  removeElementFromVoice (
                            int          inputLineNumber,
                            S_msrElement element);

    S_msrMeasure          removeLastMeasureFromVoice (
                            int inputLineNumber);

    // segments

    void                  createNewLastSegmentForVoice (
                            int    inputLineNumber,
                            string context);

    void                  createNewLastSegmentFromItsFirstMeasureForVoice (
                            int          inputLineNumber,
                            S_msrMeasure firstMeasure,
                            string       context);

    // repeats

    void                  handleRepeatEndInVoice (
                            int    inputLineNumber,
                            string measureNumber,
                            int    repeatTimes);

    void                  handleRepeatEndingStartInVoice (
                            int inputLineNumber);

    void                  handleRepeatEndingEndInVoice (
                            int       inputLineNumber,
                            string    repeatEndingNumber, // may be "1, 2"
                            msrRepeatEnding::msrRepeatEndingKind
                                      repeatEndingKind);

    void                  handleRepeatCommonPartStartInVoiceClone (
                            int inputLineNumber);

    void                  handleRepeatCommonPartEndInVoiceClone (
                            int inputLineNumber);

    void                  handleRepeatEndingStartInVoiceClone (
                            int       inputLineNumber,
                            msrRepeatEnding::msrRepeatEndingKind
                                      repeatEndingKind,
                            string    repeatEndingNumber); // may be "1, 2"

    void                  handleRepeatEndingEndInVoiceClone (
                            int       inputLineNumber,
                            string    repeatEndingNumber, // may be "1, 2"
                            msrRepeatEnding::msrRepeatEndingKind
                                      repeatEndingKind);

    void                  handleSegmentCloneEndInVoiceClone (
                            int          inputLineNumber,
                            S_msrSegment segmentClone);

/* JMI
    void                  finalizeRepeatEndInVoice (
                            int    inputLineNumber,
                            string measureNumber,
                            int    repeatTimes);
                            */

    void                  handleRepeatStartInVoice (
                            int inputLineNumber);

    void                  handleRepeatStartInVoiceClone (
                            int         inputLineNumber,
                            S_msrRepeat repeat);

    void                  handleRepeatEndInVoiceClone (
                            int inputLineNumber);

    void                  appendRepeatCloneToVoiceClone (
                            int         inputLineNumber,
                            S_msrRepeat repeatCLone);

    void                  appendRepeatEndingCloneToVoice (
                            S_msrRepeatEnding repeatEndingClone);

    // rest measures

    void                  appendRestMeasuresToVoiceElementsList (
                            S_msrRestMeasures restMeasures);

    void                  createRestMeasuresInVoice (
                            int inputLineNumber,
                            int restMeasuresNumber);

    void                  appendRestMeasuresToVoice (
                            int               inputLineNumber,
                            S_msrRestMeasures restMeasures);

    void                  appendPendingRestMeasuresToVoice (
                            int inputLineNumber);

    void                  handleRestMeasuresStartInVoiceClone (
                            int               inputLineNumber,
                            S_msrRestMeasures restMeasures);

    void                  handleRestMeasuresEndInVoiceClone (
                            int inputLineNumber);

    void                  handleRestMeasuresContentsStartInVoiceClone (
                            int inputLineNumber);

    void                  handleRestMeasuresContentsEndInVoiceClone (
                            int inputLineNumber);

    void                  appendRestMeasuresCloneToVoiceClone ( // JMI ???
                            int               inputLineNumber,
                            S_msrRestMeasures restMeasuresClone);

    // measures repeats

    void                  appendMeasuresRepeatToVoiceElementsList (
                            S_msrMeasuresRepeat measuresRepeat);

    void                  appendMeasuresRepeatReplicaToVoice (
                            int inputLineNumber);

    void                  createMeasuresRepeatFromItsFirstMeasuresInVoice (
                            int inputLineNumber,
                            int measuresRepeatMeasuresNumber,
                            int measuresRepeatSlashesNumber);

    void                  appendMeasuresRepeatToVoice (
                            int                 inputLineNumber,
                            S_msrMeasuresRepeat measuresRepeat);

    void                  appendPendingMeasuresRepeatToVoice (
                            int inputLineNumber);

    void                  createMeasuresRepeatAndAppendItToVoiceClone (
                            int inputLineNumber,
                            int measuresRepeatMeasuresNumber,
                            int measuresRepeatSlashesNumber);

    void                  handleMeasuresRepeatStartInVoiceClone (
                            int                 inputLineNumber,
                            S_msrMeasuresRepeat measuresRepeat);

    void                  handleMeasuresRepeatEndInVoiceClone (
                            int inputLineNumber);

    void                  handleMeasuresRepeatPatternStartInVoiceClone (
                            int inputLineNumber);

    void                  handleMeasuresRepeatPatternEndInVoiceClone (
                            int inputLineNumber);

    void                  handleMeasuresRepeatReplicasStartInVoiceClone (
                            int inputLineNumber);

    void                  handleMeasuresRepeatReplicasEndInVoiceClone (
                            int inputLineNumber);

    void                  appendMeasuresRepeatCloneToVoiceClone ( // JMI ???
                            int                 inputLineNumber,
                            S_msrMeasuresRepeat measuresRepeatClone);

    // stanzas

    S_msrStanza           addStanzaToVoiceByItsNumber (
                            int    inputLineNumber,
                            string stanzaNumber);

    void                  addStanzaToVoiceWithoutCatchUp ( // JMI
                            S_msrStanza stanza);

    void                  addStanzaToVoice (
                            S_msrStanza stanza);

    S_msrStanza           createStanzaInVoiceIfNotYetDone (
                            int    inputLineNumber,
                            string stanzaNumber,
                            string stanzaName);

    S_msrStanza           fetchStanzaInVoice (
                            int    inputLineNumber,
                            string stanzaNumber,
                            string stanzaName);

    // strings

    // finalization

    void                  finalizeCurrentMeasureInVoice (
                            int inputLineNumber);

    void                  finalizeVoice (
                            int inputLineNumber);

    void                  collectVoiceMeasuresIntoFlatList (
                            int inputLineNumber);

  private:

    // private services
    // ------------------------------------------------------

    // voice last segment

    void                  appendVoiceLastSegmentToInitialVoiceElements (
                            int          inputLineNumber,
                            string       context);

    void                  moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
                            int          inputLineNumber,
                            string       context);

    // harmonies

    void                  appendHarmonyToHarmonyVoice (
                            S_msrHarmony harmony);

    // repeats

    S_msrRepeat           createARepeatAndStackIt (
                            int    inputLineNumber,
                            string context);

    S_msrRepeat           createARepeatCloneAndStackIt (
                            int         inputLineNumber,
                            S_msrRepeat repeat,
                            string      context);

    void                  popRepeatFromRepeatDescrsStack (
                            int         inputLineNumber,
                            S_msrRepeat repeat,
                            string      context);

    void                  pushRepeatOntoRepeatDescrsStack (
                            int         inputLineNumber,
                            S_msrRepeat repeat,
                            string      context);

    void                  appendRepeatToInitialVoiceElements (
                            int         inputLineNumber,
                            S_msrRepeat repeat,
                            string      context);

    void                  appendRepeatCloneToInitialVoiceElements (
                            int         inputLineNumber,
                            S_msrRepeat repeatCLone,
                            string      context);

    void                  handleVoiceLevelRepeatStartInVoice (
                            int inputLineNumber);

    void                  handleNestedRepeatStartInVoice (
                            int inputLineNumber);

    void                  handleVoiceLevelRepeatEndWithoutStartInVoice (
                            int    inputLineNumber,
                            string measureNumber,
                            int    repeatTimes);

    void                  handleVoiceLevelContainingRepeatEndWithoutStartInVoice (
                            int    inputLineNumber,
                            string measureNumber,
                            int    repeatTimes);

    void                  handleVoiceLevelRepeatEndWithStartInVoice (
                            int    inputLineNumber,
                            string measureNumber,
                            int    repeatTimes);

    void                  handleVoiceLevelRepeatEndingStartWithoutExplicitStartInVoice (
                            int         inputLineNumber,
                            S_msrRepeat currentRepeat);

    void                  handleVoiceLevelRepeatEndingStartWithExplicitStartInVoice (
                            int inputLineNumber);

    void                  nestContentsIntoNewRepeatInVoice (
                            int inputLineNumber);

    void                  handleNestedRepeatEndingStartInVoice (
                            int inputLineNumber);

    void                  handleNestedRepeatEndInVoice (
                            int    inputLineNumber,
                            string measureNumber,
                            int    repeatTimes);

    void                  moveVoiceInitialElementsToRepeatCommonPart (
                            int                   inputLineNumber,
                            S_msrRepeatCommonPart repeatCommonPart,
                            string                context);

    void                  moveVoiceLastSegmentToRepeatCommonPart (
                            int                   inputLineNumber,
                            S_msrRepeatCommonPart repeatCommonPart,
                            string                context);

    void                  moveAllVoiceContentsToRepeatCommonPart (
                            int                   inputLineNumber,
                            S_msrRepeatCommonPart repeatCommonPart,
                            string                context);

    void                  moveVoiceLastSegmentToRepeatEnding (
                            int               inputLineNumber,
                            S_msrRepeatEnding repeatEnding,
                            string            context);

    void                  handleHookedRepeatEndingEndInVoice (
                            int       inputLineNumber,
                            string    repeatEndingNumber); // may be "1, 2"

    void                  handleHooklessRepeatEndingEndInVoice (
                            int       inputLineNumber,
                            string    repeatEndingNumber); // may be "1, 2"

    void                  handleHookedRepeatEndingEndInVoiceClone (
                            int       inputLineNumber,
                            string    repeatEndingNumber); // may be "1, 2"

    void                  handleHooklessRepeatEndingEndInVoiceClone (
                            int       inputLineNumber,
                            string    repeatEndingNumber); // may be "1, 2"

    // rest measures

    void                  appendRestMeasuresToInitialVoiceElements (
                            int               inputLineNumber,
                            S_msrRestMeasures restMeasures,
                            string            context);

    // measures repeats

    void                  appendMeasuresRepeatToInitialVoiceElements (
                            int                 inputLineNumber,
                            S_msrMeasuresRepeat measuresRepeat,
                            string              context);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                voiceKindAsString () const;

    string                voiceNumberAsString () const;

    string                regularVoiceStaffSequentialNumberAsString () const;

    string                asShortString () const;
    string                asString () const;

    void                  displayVoice (
                            int    inputLineNumber,
                            string context);

    void                  displayVoiceRepeatsStackRestMeasuresMeasuresRepeatAndVoice (
                            int    inputLineNumber,
                            string context);

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // upLinks

    S_msrStaff            fVoiceStaffUpLink;

    // two-way links
    S_msrVoice            fHarmonyVoiceForRegularVoiceForwardLink;
    S_msrVoice            fRegularVoiceForHarmonyVoiceBackwardLink;

    S_msrVoice            fFiguredBassVoiceForRegularVoiceForwardLink;
    S_msrVoice            fRegularVoiceForFiguredBassVoiceBackwardLink;

    // voice kind

    msrVoiceKind          fVoiceKind;

    // voice numbers

    // voice numbers in MusicXML may be greater than 4
    // and there can be holes
    int                   fVoiceNumber;

    // there can only be 4 regular voices in a staff
    // (those that can contain beamed notes)
    // and we need a number for the orientation of beams
    int                   fRegularVoiceStaffSequentialNumber;

    // voice name

    string                fVoiceName;


    // counters

    static int            gVoicesCounter;

    int                   fVoiceActualNotesCounter;
    int                   fVoiceRestsCounter;
    int                   fVoiceSkipsCounter;
    int                   fVoiceActualHarmoniesCounter;
    int                   fVoiceActualFiguredBassesCounter;

    // musically empty voices

    bool                  fMusicHasBeenInsertedInVoice;

    // regular measure ends detection

    rational              fWholeNotesSinceLastRegularMeasureEnd;

    // incomplete measures after repeats detection
    msrVoiceRepeatPhaseKind
                          fCurrentVoiceRepeatPhaseKind;

    // voice internal handling

    list<S_msrVoiceElement>
                          fInitialVoiceElementsList;

     // stanzas

    map<string, S_msrStanza>
                          fVoiceStanzasMap;

   // segments

    // fVoiceLastSegment contains the music
    // not yet stored in fVoiceInitialElementsList,
    // it is thus logically the end of the latter,
    // and is created implicitly for every voice.
    // Is is needed 'outside' of the 'list<S_msrElement>'
    // because it is not a mere S_msrElement, but a S_msrSegment
    S_msrSegment          fVoiceLastSegment;

    // fVoiceFirstSegment is used to work around LilyPond issue 34
    S_msrSegment          fVoiceFirstSegment;

    // measures

    // fVoiceFirstMeasure is used to number voice upbeats as measure 0
    S_msrMeasure          fVoiceFirstMeasure;

    // measures flat list
    // i.e. without segments nor repeats,
    // gathered from fVoiceInitialElementsList and fVoiceLastSegment
    // by finalizeMeasure()
    list<S_msrMeasure>    fVoiceMeasuresFlatList;

    // notes

    // fVoiceLastAppendedNote is used to build chords upon their second note
    S_msrNote             fVoiceLastAppendedNote;
    rational              fVoiceShortestNoteDuration;

    // fVoiceShortestNoteDuration and fVoiceShortestNoteTupletFactor
    // are used to compute a number of divisions per quarter note
    // if needed, such as when generating MusicXML from MSR
    rational              fVoiceShorftestNoteDuration;
    rational              fVoiceShortestNoteTupletFactor;

    // repeats

  public:

    // public work services
    // ------------------------------------------------------

    // clef, key, time

    void                  setVoiceCurrentClef (S_msrClef clef);

    S_msrClef             getVoiceCurrentClef () const
                              { return fVoiceCurrentClef; };

    S_msrClef             getVoiceFirstClef () const
                              { return fVoiceFirstClef; };

    void                  setVoiceCurrentKey (S_msrKey key);

    S_msrKey              getVoiceCurrentKey  () const
                              { return fVoiceCurrentKey; };

    void                  setVoiceCurrentTime (S_msrTime time);

    S_msrTime             getVoiceCurrentTime () const
                              { return fVoiceCurrentTime; };

     // measures

    const string          getVoiceCurrentMeasureNumber () const
                              { return fVoiceCurrentMeasureNumber; }

    void                  incrementVoiceCurrentMeasurePuristNumber (
                            int    inputLineNumber,
                            string context);

    const int             getVoiceCurrentMeasurePuristNumber () const
                              { return fVoiceCurrentMeasurePuristNumber; }

    void                  setVoiceFirstMeasure (
                            S_msrMeasure measure)
                              { fVoiceFirstMeasure = measure; }

    const S_msrMeasure    getVoiceFirstMeasure () const
                              { return fVoiceFirstMeasure; }

    void                  appendMeasureCloneToVoiceClone (
                            int          inputLineNumber,
                            S_msrMeasure measureClone);

  private:

    // private work services
    // ------------------------------------------------------

    void                  displayVoiceRepeatsStack (
                            int    inputLineNumber,
                            string context);

    void                  displayVoiceRepeatsStackAndVoice (
                            int    inputLineNumber,
                            string context);

    void                  displayVoiceRestMeasures (
                            int    inputLineNumber,
                            string context);

    void                  displayVoiceRestMeasuresAndVoice (
                            int    inputLineNumber,
                            string context);

    void                  displayVoiceMeasuresRepeat (
                            int    inputLineNumber,
                            string context);

    void                  displayVoiceMeasuresRepeatAndVoice (
                            int    inputLineNumber,
                            string context);

  private:

    // work fields
    // ------------------------------------------------------

    // clef, key, time
    // needed at the voice level, because adding such to voice clones
    // is done directly, without going down the part-staff-voice hierarchy
    S_msrClef             fVoiceCurrentClef;

    S_msrKey              fVoiceCurrentKey;

    S_msrTime             fVoiceCurrentTime;

    // the first in the voice is handled specifically for LilyPond code generation
    S_msrClef             fVoiceFirstClef;

    // measures

    string                fVoiceCurrentMeasureNumber;

    int                   fVoiceCurrentMeasurePuristNumber;
                            // this is a 'purist' measure number,
                            // that starts at 0 if there is an anacrusis,
                            // and 1 otherwise,
                            // and is shared by incomplete (sub)measure parts

    // fVoiceCurrentMeasure contains the last measure append to the voice
    S_msrMeasure          fVoiceCurrentMeasure;

    // a stack is needed to handle pending repeats, which can be nested
    list<S_msrRepeatDescr>
                          fVoicePendingRepeatDescrsStack;

    // rest measures

    // fVoicePendingRestMeasures is either null
    // or the last msrRestMeasures created,
    // but not yet appended to the voice
    S_msrRestMeasures     fVoicePendingRestMeasures;

    // fVoiceRestMeasuresWaitingForItsNextMeasureNumber is either null
    // or the last msrRestMeasures created and appended to the voice,
    // but with its next measure number not yet set
    S_msrRestMeasures     fVoiceRestMeasuresWaitingForItsNextMeasureNumber;
    int                   fVoiceRemainingRestMeasures;

    bool                  fVoiceContainsRestMeasures;

    // measure repeats

    // fVoicePendingMeasuresRepeat is either null
    // or the last msrMeasuresRepeat created,
    // but not yet appended to the voice
    S_msrMeasuresRepeat   fVoicePendingMeasuresRepeat;

    bool                  fVoiceContainsMeasuresRepeats;

    // voice finalization

    bool                  fVoiceHasBeenFinalized;
};
EXP ostream& operator<< (ostream& os, const S_msrVoice& elt);

//______________________________________________________________________________
class msrStaff : public msrElement
{
  public:

    // constants
    // ------------------------------------------------------

    #define K_NO_STAFF_NUMBER                 -79

    // data types
    // ------------------------------------------------------

    enum msrStaffKind {
      kStaffRegular,
      kStaffTablature,
      kStaffHarmony,
      kStaffFiguredBass,
      kStaffDrum,
      kStaffRythmic };

    static string staffKindAsString (
      msrStaffKind staffKind);

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

    // initialization
    // ------------------------------------------------------

    void                  initializeStaff ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks

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
                          getStaffAllVoicesMap () const
                              { return fStaffAllVoicesMap; }

    const map<int, S_msrVoice>&
                          getStaffRegularVoicesMap () const
                              {
                                return
                                  fStaffRegularVoicesMap;
                              }

    const list<S_msrVoice>&
                          getStaffAllVoicesList () const
                              {
                                return
                                  fStaffAllVoicesList;
                              }

    // services
    // ------------------------------------------------------

    // staff details

    void                  appendStaffDetailsToStaff (
                            S_msrStaffDetails staffDetails);

    // measures

    void                  padUpToActualMeasureWholeNotesInStaff (
                            int      inputLineNumber,
                            rational wholeNotes);

    // clef, key, time

    void                  appendClefToStaff (S_msrClef clef);

    void                  appendKeyToStaff (S_msrKey  key);

    void                  appendTimeToStaff (S_msrTime time);
    void                  appendTimeToStaffClone (S_msrTime time);

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
                            msrVoice::msrVoiceKind voiceKind,
                            int                    voiceNumber,
                            string                 currentMeasureNumber);

    void                  registerVoiceInStaff (
                            int        inputLineNumber,
                            S_msrVoice voice);

    void                  registerVoiceByItsNumber (
                            int        inputLineNumber,
                            int        voiceNumber,
                            S_msrVoice voice);

    void                  registerVoiceInRegularVoicesMap (
                            int        voiceNumber,
                            S_msrVoice voice);

    void                  registerVoiceInAllVoicesList (
                            int        voiceNumber,
                            S_msrVoice voice);

    S_msrVoice            fetchVoiceFromStaffByItsNumber (
                            int inputLineNumber,
                            int voiceNumber);

    void                  addAVoiceToStaffIfItHasNone (
                            int inputLineNumber);

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

    void                  appendRestMeasuresCloneToStaff (
                            int               inputLineNumber,
                            S_msrRestMeasures restMeasures);

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

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    // staff number

    string                staffNumberAsString ();

    string                staffKindAsString () const;

    virtual void          print (ostream& os);

    virtual void          printSummary (ostream& os);

  private:

    // fields
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

    // staff voices

    static int            gStaffMaxRegularVoices;

    // the dictionary of all the voices in the staff
    map<int, S_msrVoice>  fStaffAllVoicesMap;

    // we need to handle the regular voice specifically
    // to assign them sequencing numbers from 1 to gMaxStaffVoices,
    // needed to set the beams orientation (up or down)
    int                   fStaffRegularVoicesCounter;
    map<int, S_msrVoice>  fStaffRegularVoicesMap;

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
                              { return fStaffCurrentClef; };

    void                  setStaffCurrentKey (S_msrKey key);

    S_msrKey              getStaffCurrentKey  () const
                              { return fStaffCurrentKey; };

    void                  setStaffCurrentTime (S_msrTime time);

    S_msrTime             getStaffCurrentTime () const
                              { return fStaffCurrentTime; };

    // finalization

    void                  finalizeCurrentMeasureInStaff (
                            int inputLineNumber);

    void                  finalizeStaff (
                            int inputLineNumber);

  private:

    // private work services
    // ------------------------------------------------------

    // transpose

    S_msrTranspose        getStaffCurrentTranspose () const
                              { return fStaffCurrentTranspose; };

    void                  printMeasurePendingMeasureElementsList ();

    // voices ordering in staves

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

//______________________________________________________________________________
class msrVoiceStaffChange : public msrMeasureElement
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

    // services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    S_msrStaff            fStaffToChangeTo;
};
typedef SMARTP<msrVoiceStaffChange> S_msrVoiceStaffChange;
EXP ostream& operator<< (ostream& os, const S_msrVoiceStaffChange& elt);

//______________________________________________________________________________
class msrPartGroup;
typedef SMARTP<msrPartGroup> S_msrPartGroup;

class msrScore;
typedef SMARTP<msrScore> S_msrScore;

//______________________________________________________________________________
class msrPart : public msrPartGroupElement
{
  public:

    // constants
    // ------------------------------------------------------

    #define K_PART_FIGURED_BASS_STAFF_NUMBER -219
    #define K_PART_FIGURED_BASS_VOICE_NUMBER -227

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

    virtual ~msrPart ();

  private:

    // initialization
    // ------------------------------------------------------

    void                  initializePart ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks

    void                  setPartPartGroupUpLink (
                            S_msrPartGroup partGroup)
                              { fPartPartGroupUpLink = partGroup; };

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

    void                  setPartActualMeasureWholeNotesHighTide (
                            int      inputLineNumber,
                            rational wholeNotes);

    void                  updatePartActualMeasureWholeNotesHighTide (
                            int      inputLineNumber,
                            rational wholeNotes);

    rational              getPartActualMeasureWholeNotesHighTide () const
                              {
                                return
                                  fPartActualMeasureWholeNotesHighTide;
                              }

    void                  setPartNumberOfMeasures (
                            int partNumberOfMeasures)
                              {
                                fPartNumberOfMeasures =
                                  partNumberOfMeasures;
                              }

    const int             getPartNumberOfMeasures () const
                              { return fPartNumberOfMeasures; }
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
                          getPartStavesMap ()
                              { return fPartStavesMap; }

    // services
    // ------------------------------------------------------

    void                  addAVoiceToStavesThatHaveNone (
                            int inputLineNumber);

    // measures

    void                  padUpToActualMeasureWholeNotesInPart (
                            int      inputLineNumber,
                            rational wholeNotes);

    // part name display

    void                  appendPartNameDisplayToPart (
                            S_msrPartNameDisplay partNameDisplay);

    // part abbreviation display

    void                  appendPartAbbreviationDisplayToPart (
                            S_msrPartAbbreviationDisplay partAbbreviationDisplay);

/* JMI
    // figured bass staff and voice

    void                  createPartFiguredBassStaffAndVoiceIfNotYetDone (
                            int inputLineNumber);
        */

    // measures

    void                  createMeasureAndAppendItToPart (
                            int    inputLineNumber,
                            string measureNumber,
                            msrMeasure::msrMeasureImplicitKind
                                   measureImplicitKind);

    void                  setNextMeasureNumberInPart (
                            int    inputLineNumber,
                            string nextMeasureNumber);

    // clef, key, time

    void                  appendClefToPart (S_msrClef clef);

    void                  appendKeyToPart (S_msrKey  key);

    void                  appendTimeToPart (S_msrTime time);
    void                  appendTimeToPartClone (S_msrTime time);

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
                            msrRepeatEnding::msrRepeatEndingKind
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

    void                  createRestMeasuresInPart (
                            int inputLineNumber,
                            int restMeasuresNumber);

    void                  appendPendingRestMeasuresToPart (
                            int inputLineNumber);

    void                  appendRestMeasuresCloneToPart (
                            int               inputLineNumber,
                            S_msrRestMeasures restMeasures);

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
                            int                    inputLineNumber,
                            msrStaff::msrStaffKind staffKind,
                            int                    staffNumber);

    void                  addStaffToPartCloneByItsNumber (
                            S_msrStaff staff);

    S_msrStaff            fetchStaffFromPart (int staffNumber);

    // voices

    void                  removePartEmptyVoices ();

    // harmonies

    // JMI ???

    // frames

    // JMI ???

    // figured bass

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

/* JMI
    // backup

    void                  handleBackup (
                            int inputLineNumber,
                            int divisions,
                            int divisionsPerQuarterNote);
*/

    // LilyPond issue 34

    void                  addSkipGraceNotesGroupBeforeAheadOfVoicesClonesIfNeeded (
                            S_msrVoice           graceNotesGroupOriginVoice,
                            S_msrGraceNotesGroup skipGraceNotesGroup);

/*
    void                  appendSkipGraceNotesToVoicesClones ( // JMI ???
                            S_msrVoice      graceNotesOriginVoice,
                            S_msrGraceNotes skipGraceNotes);

                            */

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual string        asString () const;

    virtual void          print (ostream& os);

    virtual void          printSummary (ostream& os);

  private:

    // fields
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

    // measures

    string                fPartCurrentMeasureNumber;

    int                   fPartNumberOfMeasures;

    rational              fPartActualMeasureWholeNotesHighTide;

    // clef, key, time

    S_msrClef             fPartCurrentClef;

    S_msrKey              fPartCurrentKey;

    S_msrTime             fPartCurrentTime;

    // transpose

    S_msrTranspose        fPartCurrentTranspose;

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
                              { return fPartCurrentClef; };

    S_msrKey              getPartCurrentKey  () const
                              { return fPartCurrentKey; };

    void                  setPartCurrentTime (S_msrTime time)
                              { fPartCurrentTime = time; };

    S_msrTime             getPartCurrentTime () const
                              { return fPartCurrentTime; };

    // transpose

    S_msrTranspose        getPartCurrentTranspose () const
                              { return fPartCurrentTranspose; };

    // staff details

    S_msrStaffDetails     getCurrentPartStaffDetails () const
                              { return fCurrentPartStaffDetails; }

    // finalization

    void                  finalizeCurrentMeasureInPart (
                            int inputLineNumber);

    void                  finalizePart (
                            int inputLineNumber);

    void                  finalizePartClone (
                            int inputLineNumber);

  private:

    // private work services
    // ------------------------------------------------------

    // measure elements

    void                  printMeasurePendingMeasureElementsList ();

  private:

    // work fields
    // ------------------------------------------------------

    // staff details

    S_msrStaffDetails     fCurrentPartStaffDetails;
};
typedef SMARTP<msrPart> S_msrPart;
EXP ostream& operator<< (ostream& os, const S_msrPart& elt);


}

#endif
