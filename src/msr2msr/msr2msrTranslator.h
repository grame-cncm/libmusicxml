/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msr2msrTranslator___
#define ___msr2msrTranslator___

#include <map>
#include <vector>

#include "msr.h"


namespace MusicXML2
{

//________________________________________________________________________
struct msrHiddenMeasureAndBarlineDescr : public smartable
{
/*
 * positions represent the order in which the parts appear in <part-list />
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrHiddenMeasureAndBarlineDescr> create (
      int           inputLineNumber,
      S_msrDalSegno dalSegno);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrHiddenMeasureAndBarlineDescr (
      int           inputLineNumber,
      S_msrDalSegno dalSegno);

    virtual ~msrHiddenMeasureAndBarlineDescr ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getInputLineNumber () const
                              { return fInputLineNumber; }

    S_msrDalSegno         getDalSegno () const
                              { return fDalSegno; }

  public:

    // public services
    // ------------------------------------------------------

    string                hiddenMeasureAndBarlineDescrAsString () const;

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fInputLineNumber;
    S_msrDalSegno         fDalSegno;
};
typedef SMARTP<msrHiddenMeasureAndBarlineDescr> S_msrHiddenMeasureAndBarlineDescr;
EXP ostream& operator<< (ostream& os, const S_msrHiddenMeasureAndBarlineDescr& elt);

//________________________________________________________________________
class msr2msrTranslator :

  public visitor<S_msrScore>,

  // rights

  public visitor<S_msrIdentification>,

  public visitor<S_msrCredit>,
  public visitor<S_msrCreditWords>,

  // variable-value associations

  public visitor<S_msrVarValAssoc>,
  public visitor<S_msrVarValsListAssoc>,

  // scaling

  public visitor<S_msrScaling>,

  // layout

  public visitor<S_msrPageLayout>,
  public visitor<S_msrSystemLayout>,
  public visitor<S_msrStaffLayout>,

  // appearance

  public visitor<S_msrAppearance>,

  // parts & part groups

  public visitor<S_msrPartGroup>,

  public visitor<S_msrPart>,

  // staff details

  public visitor<S_msrStaffTuning>,
  public visitor<S_msrStaffDetails>,

  public visitor<S_msrStaff>,

  public visitor<S_msrVoice>,

  public visitor<S_msrVoiceStaffChange>,

  // lyrics

  public visitor<S_msrStanza>,
  public visitor<S_msrSyllable>,

  // clefs

  public visitor<S_msrClef>,

  // keys

  public visitor<S_msrKey>,

  // times

  public visitor<S_msrTime>,

  // transpose

  public visitor<S_msrTranspose>,

  // print

  public visitor<S_msrPartNameDisplay>,
  public visitor<S_msrPartAbbreviationDisplay>,

  // words

  public visitor<S_msrWords>,

  // tempo

  public visitor<S_msrTempo>,

  public visitor<S_msrSegment>,

  // rehearsal

  public visitor<S_msrRehearsal>,

  // harmonies

  public visitor<S_msrHarmony>,
  public visitor<S_msrHarmonyDegree>,

  // frames

  public visitor<S_msrFrame>,

  // figured bass

  public visitor<S_msrFiguredBass>,
  public visitor<S_msrFigure>,

  // measures

  public visitor<S_msrMeasure>,

  // articulations

  public visitor<S_msrArticulation>,

  public visitor<S_msrFermata>,

  public visitor<S_msrArpeggiato>,
  public visitor<S_msrNonArpeggiato>,

  // technicals

  public visitor<S_msrTechnical>,
  public visitor<S_msrTechnicalWithInteger>,
  public visitor<S_msrTechnicalWithFloat>,
  public visitor<S_msrTechnicalWithString>,

  // ornaments

  public visitor<S_msrOrnament>,

  // spanners

  public visitor<S_msrSpanner>,

  // glissandos

  public visitor<S_msrGlissando>,

  // slides

  public visitor<S_msrSlide>,

  // tremolos

  public visitor<S_msrSingleTremolo>,

  public visitor<S_msrDoubleTremolo>,

  // dynamics

  public visitor<S_msrDynamics>,

  public visitor<S_msrOtherDynamics>,

  // slashes

  public visitor<S_msrSlash>,

  // wedges

  public visitor<S_msrWedge>,

  // slurs

  public visitor<S_msrSlur>,
  public visitor<S_msrChordSlurLink>,

  // ligatures

  public visitor<S_msrLigature>,

  // grace note groups

  public visitor<S_msrGraceNotesGroup>,
  public visitor<S_msrChordGraceNotesGroupLink>,

  // notes

  public visitor<S_msrNote>,
  public visitor<S_msrOctaveShift>,

  // accordion registration

  public visitor<S_msrAccordionRegistration>,

  // harp pedals tuning

  public visitor<S_msrHarpPedalsTuning>,

  public visitor<S_msrStem>,

  // chords

  public visitor<S_msrChord>,

  // tuplets

  public visitor<S_msrTuplet>,

  // ties, slurs, brackets & beams

  public visitor<S_msrTie>,

  public visitor<S_msrBeam>,
  public visitor<S_msrChordBeamLink>,

  // barlines

  public visitor<S_msrBarline>,

  // staff-level elements

  public visitor<S_msrSegno>,
  public visitor<S_msrDalSegno>,
  public visitor<S_msrCoda>,

  public visitor<S_msrEyeGlasses>,
  public visitor<S_msrPedal>,
  public visitor<S_msrDamp>,
  public visitor<S_msrDampAll>,
  public visitor<S_msrScordatura>,

  // bars

  public visitor<S_msrBarCheck>,
  public visitor<S_msrBarNumberCheck>,

  // breaks

  public visitor<S_msrLineBreak>,

  public visitor<S_msrPageBreak>,

  // repeats

  public visitor<S_msrRepeat>,
  public visitor<S_msrRepeatCommonPart>,
  public visitor<S_msrRepeatEnding>,

  public visitor<S_msrMeasuresRepeat>,
  public visitor<S_msrMeasuresRepeatPattern>,
  public visitor<S_msrMeasuresRepeatReplicas>,

  public visitor<S_msrRestMeasures>,
  public visitor<S_msrRestMeasuresContents>,

  // midi

  public visitor<S_msrMidiTempo>

{
  public:

    msr2msrTranslator (
      S_msrScore mScore);

    virtual ~msr2msrTranslator ();

    void buildMsrScoreFromMsrScore ();

    S_msrScore getResultingMsrScore () const
        { return fResultingMsrScore; }

  protected:

    virtual void visitStart (S_msrScore& elt);
    virtual void visitEnd   (S_msrScore& elt);

    virtual void visitStart (S_msrIdentification& elt);
    virtual void visitEnd   (S_msrIdentification& elt);

    virtual void visitStart (S_msrCredit& elt);
    virtual void visitEnd   (S_msrCredit& elt);
    virtual void visitStart (S_msrCreditWords& elt);
    virtual void visitEnd   (S_msrCreditWords& elt);

    // parts & part groups

    virtual void visitStart (S_msrPartGroup& elt);
    virtual void visitEnd   (S_msrPartGroup& elt);

    virtual void visitStart (S_msrPart& elt);
    virtual void visitEnd   (S_msrPart& elt);

    virtual void visitStart (S_msrStaffTuning& elt);
    virtual void visitStart (S_msrStaffDetails& elt);
    virtual void visitEnd   (S_msrStaffDetails& elt);

    virtual void visitStart (S_msrStaff& elt);
    virtual void visitEnd   (S_msrStaff& elt);

    virtual void visitStart (S_msrVoice& elt);
    virtual void visitEnd   (S_msrVoice& elt);

    virtual void visitStart (S_msrVoiceStaffChange& elt);

    virtual void visitStart (S_msrStanza& elt);
    virtual void visitEnd   (S_msrStanza& elt);

    virtual void visitStart (S_msrSyllable& elt);
    virtual void visitEnd   (S_msrSyllable& elt);

    virtual void visitStart (S_msrClef& elt);
    virtual void visitEnd   (S_msrClef& elt);

    virtual void visitStart (S_msrKey& elt);
    virtual void visitEnd   (S_msrKey& elt);

    virtual void visitStart (S_msrTime& elt);
    virtual void visitEnd   (S_msrTime& elt);

    virtual void visitStart (S_msrTranspose& elt);
    virtual void visitEnd   (S_msrTranspose& elt);

    virtual void visitStart (S_msrPartNameDisplay& elt);
    virtual void visitStart (S_msrPartAbbreviationDisplay& elt);

    virtual void visitStart (S_msrWords& elt);
    virtual void visitEnd   (S_msrWords& elt);

    virtual void visitStart (S_msrTempo& elt);
    virtual void visitEnd   (S_msrTempo& elt);

    virtual void visitStart (S_msrRehearsal& elt);
    virtual void visitEnd   (S_msrRehearsal& elt);

    virtual void visitStart (S_msrSegment& elt);
    virtual void visitEnd   (S_msrSegment& elt);

    virtual void visitStart (S_msrHarmony& elt);
    virtual void visitEnd   (S_msrHarmony& elt);
    virtual void visitStart (S_msrHarmonyDegree& elt);

    virtual void visitStart (S_msrFrame& elt);

    virtual void visitStart (S_msrFiguredBass& elt);
    virtual void visitEnd   (S_msrFiguredBass& elt);
    virtual void visitStart (S_msrFigure& elt);

    virtual void visitStart (S_msrMeasure& elt);
    virtual void visitEnd   (S_msrMeasure& elt);

    virtual void visitStart (S_msrArticulation& elt);
    virtual void visitEnd   (S_msrArticulation& elt);

    virtual void visitStart (S_msrFermata& elt);

    virtual void visitStart (S_msrArpeggiato& elt);
    virtual void visitStart (S_msrNonArpeggiato& elt);

    virtual void visitStart (S_msrTechnical& elt);
    virtual void visitEnd   (S_msrTechnical& elt);

    virtual void visitStart (S_msrTechnicalWithInteger& elt);
    virtual void visitEnd   (S_msrTechnicalWithInteger& elt);

    virtual void visitStart (S_msrTechnicalWithFloat& elt);
    virtual void visitEnd   (S_msrTechnicalWithFloat& elt);

    virtual void visitStart (S_msrTechnicalWithString& elt);
    virtual void visitEnd   (S_msrTechnicalWithString& elt);

    virtual void visitStart (S_msrOrnament& elt);
    virtual void visitEnd   (S_msrOrnament& elt);

    virtual void visitStart (S_msrSpanner& elt);
    virtual void visitEnd   (S_msrSpanner& elt);

    virtual void visitStart (S_msrGlissando& elt);
    virtual void visitEnd   (S_msrGlissando& elt);

    virtual void visitStart (S_msrSlide& elt);
    virtual void visitEnd   (S_msrSlide& elt);

    // tremolos
    virtual void visitStart (S_msrSingleTremolo& elt);
    virtual void visitEnd   (S_msrSingleTremolo& elt);

    virtual void visitStart (S_msrDoubleTremolo& elt);
    virtual void visitEnd   (S_msrDoubleTremolo& elt);

    // dynamics
    virtual void visitStart (S_msrDynamics& elt);
    virtual void visitEnd   (S_msrDynamics& elt);

    virtual void visitStart (S_msrOtherDynamics& elt);
    virtual void visitEnd   (S_msrOtherDynamics& elt);

    // wedges
    virtual void visitStart (S_msrWedge& elt);
    virtual void visitEnd   (S_msrWedge& elt);

    // slashes
    virtual void visitStart (S_msrSlash& elt);

    // grace notes groups
    virtual void visitStart (S_msrGraceNotesGroup& elt);
    virtual void visitEnd   (S_msrGraceNotesGroup& elt);
    virtual void visitStart (S_msrChordGraceNotesGroupLink& elt);
    virtual void visitEnd   (S_msrChordGraceNotesGroupLink& elt);

    // notes
    virtual void visitStart (S_msrNote& elt);
    virtual void visitEnd   (S_msrNote& elt);

    // octave shifts
    virtual void visitStart (S_msrOctaveShift& elt);
    virtual void visitEnd   (S_msrOctaveShift& elt);

    // instruments
    virtual void visitStart (S_msrAccordionRegistration& elt);

    virtual void visitStart (S_msrHarpPedalsTuning& elt);

    // stems
    virtual void visitStart (S_msrStem& elt);
    virtual void visitEnd   (S_msrStem& elt);

    // beams
    virtual void visitStart (S_msrBeam& elt);
    virtual void visitEnd   (S_msrBeam& elt);
    virtual void visitStart (S_msrChordBeamLink& elt);
    virtual void visitEnd   (S_msrChordBeamLink& elt);

    // chords
    virtual void visitStart (S_msrChord& elt);
    virtual void visitEnd   (S_msrChord& elt);

    // tuplets
    virtual void visitStart (S_msrTuplet& elt);
    virtual void visitEnd   (S_msrTuplet& elt);

    // ties
    virtual void visitStart (S_msrTie& elt);
    virtual void visitEnd   (S_msrTie& elt);

    // slurs
    virtual void visitStart (S_msrSlur& elt);
    virtual void visitEnd   (S_msrSlur& elt);
    virtual void visitStart (S_msrChordSlurLink& elt);
    virtual void visitEnd   (S_msrChordSlurLink& elt);

    // ligatures
    virtual void visitStart (S_msrLigature& elt);
    virtual void visitEnd   (S_msrLigature& elt);

    // barlines
    virtual void visitStart (S_msrBarline& elt);
    virtual void visitEnd   (S_msrBarline& elt);

    virtual void visitStart (S_msrSegno& elt);
    virtual void visitStart (S_msrDalSegno& elt);
    virtual void visitStart (S_msrCoda& elt);

    virtual void visitStart (S_msrEyeGlasses& elt);
    virtual void visitStart (S_msrPedal& elt);
    virtual void visitStart (S_msrDamp& elt);
    virtual void visitStart (S_msrDampAll& elt);
    virtual void visitStart (S_msrScordatura& elt);

    // bar checks
    virtual void visitStart (S_msrBarCheck& elt);
    virtual void visitEnd   (S_msrBarCheck& elt);

    // bar number checks
    virtual void visitStart (S_msrBarNumberCheck& elt);
    virtual void visitEnd   (S_msrBarNumberCheck& elt);

    // breaks
    virtual void visitStart (S_msrLineBreak& elt);
    virtual void visitEnd   (S_msrLineBreak& elt);

    virtual void visitStart (S_msrPageBreak& elt);
    virtual void visitEnd   (S_msrPageBreak& elt);

    // repeats
    virtual void visitStart (S_msrRepeat& elt);
    virtual void visitEnd   (S_msrRepeat& elt);
    virtual void visitStart (S_msrRepeatCommonPart& elt);
    virtual void visitEnd   (S_msrRepeatCommonPart& elt);
    virtual void visitStart (S_msrRepeatEnding& elt);
    virtual void visitEnd   (S_msrRepeatEnding& elt);

    // measure repeats
    virtual void visitStart (S_msrMeasuresRepeat& elt);
    virtual void visitEnd   (S_msrMeasuresRepeat& elt);
    virtual void visitStart (S_msrMeasuresRepeatPattern& elt);
    virtual void visitEnd   (S_msrMeasuresRepeatPattern& elt);
    virtual void visitStart (S_msrMeasuresRepeatReplicas& elt);
    virtual void visitEnd   (S_msrMeasuresRepeatReplicas& elt);

    // rest measures
    virtual void visitStart (S_msrRestMeasures& elt);
    virtual void visitEnd   (S_msrRestMeasures& elt);
    virtual void visitStart (S_msrRestMeasuresContents& elt);
    virtual void visitEnd   (S_msrRestMeasuresContents& elt);

    virtual void visitStart (S_msrVarValAssoc& elt);
    virtual void visitEnd   (S_msrVarValAssoc& elt);
    virtual void visitStart (S_msrVarValsListAssoc& elt);
    virtual void visitEnd   (S_msrVarValsListAssoc& elt);

    // scaling
    virtual void visitStart (S_msrScaling& elt);
    virtual void visitEnd   (S_msrScaling& elt);

    // layout
    virtual void visitStart (S_msrPageLayout& elt);
    virtual void visitEnd   (S_msrPageLayout& elt);

    virtual void visitStart (S_msrSystemLayout& elt);
    virtual void visitEnd   (S_msrSystemLayout& elt);

    virtual void visitStart (S_msrStaffLayout& elt);
    virtual void visitEnd   (S_msrStaffLayout& elt);

    // appearance
    virtual void visitStart (S_msrAppearance& elt);
    virtual void visitEnd   (S_msrAppearance& elt);

    // midi
    virtual void visitStart (S_msrMidiTempo& elt);
    virtual void visitEnd   (S_msrMidiTempo& elt);

  private:

    // the MSR score we're visiting
    // ------------------------------------------------------
    S_msrScore                fVisitedMsrScore;


    // the MSR score we're building (it is always built)
    // ------------------------------------------------------
    S_msrScore                fResultingMsrScore;


    // score
    // ------------------------------------------------------
    S_msrScore                fCurrentMsrScoreClone;


    // identification
    // ------------------------------------------------------
    bool                      fOnGoingIdentification;
    S_msrIdentification       fCurrentIdentification;


    // credits
    // ------------------------------------------------------
    S_msrCredit               fCurrentCredit;


    // part groups
    // ------------------------------------------------------
  //  S_msrPartGroup            fCurrentPartGroupClone; JMI

    // partGroup's can be nested, hence this stack
    // the current partGroup is the top of the stack
    stack<S_msrPartGroup>     fPartGroupsStack;


    // parts
    // ------------------------------------------------------
    S_msrPart                 fCurrentPartClone;


    // staff details
    // ------------------------------------------------------

    S_msrStaffTuning          fCurrentStaffTuningClone;


    // staves
    // ------------------------------------------------------
    S_msrStaff                fCurrentStaffClone;
    // prevent clef, key and time from being handled twice
    bool                      fOnGoingStaff;


    // voices
    // ------------------------------------------------------
    S_msrVoice                fCurrentVoiceClone;
    S_msrVoice                fCurrentVoiceOriginal;
    map<S_msrNote, S_msrNote> fVoiceNotesMap; // JMI

    // dal segnos
    // ------------------------------------------------------

    list<S_msrHiddenMeasureAndBarlineDescr>
                              fPartHiddenMeasureAndBarlineDescrList;

    void                      displayPartHiddenMeasureAndBarlineDescrList ();

    void                      handlePartHiddenMeasureAndBarlineDescrList ();


    // harmonies
    // ------------------------------------------------------
    bool                      fOnGoingHarmonyVoice;

    bool                      fOnGoingHarmony;
    S_msrHarmony              fCurrentHarmonyClone;

    list<S_msrHarmony>        fPendingHarmoniesList;


    // figured bass
    // ------------------------------------------------------
    bool                      fOnGoingFiguredBassVoice;
    S_msrFiguredBass          fCurrentFiguredBassClone;


    // segments
    // ------------------------------------------------------
    // the current segment clone is the one at the top of the stack
    S_msrSegment              fCurrentSegmentClone;


    // measures
    // ------------------------------------------------------
    S_msrMeasure              fCurrentMeasureClone;

    // full measure rests compression
    S_msrMeasure              fCurrentRestMeasure;
    S_msrRestMeasures         fCurrentRestMeasures;

    // bar checks
    // ------------------------------------------------------
    S_msrBarCheck             fLastBarCheck;


    // notes
    // ------------------------------------------------------

    S_msrNote                 fCurrentNoteClone;

    // browsing grace notes groups leads to several notes
    // being ongoing simultaneously,
    // since such groups are attached to a note, hence:
    list<S_msrNote>           fOnGoingNotesStack;
    void                      displayOnGoingNotesStack (string context);

    bool                      fOnGoingNonGraceNote;

    // fCurrentNonGraceNoteClone is not used for grace notes,
    // which are visited while the note they're attached to
    // is being visited too
    S_msrNote                 fCurrentNonGraceNoteClone;

    // to help workaround LilyPond issue #34 JMI ???
    S_msrNote                 fFirstNoteCloneInVoice;

    S_msrGraceNotesGroup      fCurrentSkipGraceNotesGroup;


    // beams
    // ------------------------------------------------------
    bool                      fOnGoingChordBeamLink;

    // glissandos
    // ------------------------------------------------------


    // slides
    // ------------------------------------------------------


    // double tremolos
    // ------------------------------------------------------
    S_msrDoubleTremolo        fCurrentDoubleTremoloClone;
    bool                      fOnGoingDoubleTremolo;


    // stems
    // ------------------------------------------------------
    S_msrStem                 fCurrentStem;


    // grace notes
    // ------------------------------------------------------
    S_msrGraceNotesGroup      fCurrentGraceNotesGroupClone;
    S_msrNote                 fCurrentGraceNoteClone;
    bool                      fOnGoingGraceNotesGroup;

    bool                      fOnGoingChordGraceNotesGroupLink;

    // afterGraceNotes optimisation
    S_msrAfterGraceNotesGroup fPendingAfterGraceNotesGroup;
    S_msrElement              fCurrentAfterGraceNotesGroupElement;

    /* JMI
    void                      prependSkipGraceNotesGroupToPartOtherVoices (
                                S_msrPart            partClone,
                                S_msrVoice           voiceClone,
                                S_msrGraceNotesGroup skipGraceNotesGroup);
                                */

// JMI    bool                      fOnGoingNote;
// JMI    S_msrChordGraceNotesGroupLink
//                          fCurrentChordGraceNotesGroupLink;

    // chords
    // ------------------------------------------------------
    bool                      fOnGoingChord;
    S_msrChord                fCurrentChordClone;


    // tuplets
    // ------------------------------------------------------
//    S_msrTuplet             fCurrentTupletClone;
 //   bool                      fOnGoingTuplet;
    stack<S_msrTuplet>        fTupletClonesStack;


    // slurs
    // ------------------------------------------------------
    bool                      fOnGoingChordSlurLink;

    // stanzas
    // ------------------------------------------------------
    S_msrStanza               fCurrentStanzaClone;
    bool                      fOnGoingStanza;


    // syllables
    // ------------------------------------------------------
    S_msrSyllable             fCurrentSyllableClone;
    bool                      fOnGoingSyllableExtend;


    // current ongoing values display
    // ------------------------------------------------------
    void                      displayCurrentOnGoingValues ();
};


}


#endif
