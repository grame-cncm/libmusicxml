/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msr2LpsrTranslator__
#define __msr2LpsrTranslator__

#include <map>
#include <vector>

#include "lpsr.h"


namespace MusicXML2
{

/*!
\addtogroup visitors
@{
*/

class msr2LpsrTranslator :

  public visitor<S_msrScore>,
  
  // rights

  public visitor<S_msrIdentification>,
  
  public visitor<S_msrCredit>,
  public visitor<S_msrCreditWords>,
  
  // variable-value associations
  
  public visitor<S_msrVarValAssoc>,
  
  // geometry

  public visitor<S_msrPageGeometry>,

  // layout

  public visitor<S_msrLayout>,
  
  // parts & part groups

  public visitor<S_msrPartGroup>,
  
  public visitor<S_msrPart>,
  
  // staff details

  public visitor<S_msrStaffLinesNumber>,
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
  
  // words

  public visitor<S_msrWords>,
  
  // tempo

  public visitor<S_msrTempo>,
  
  public visitor<S_msrSegment>,
  
  // harmonies

  public visitor<S_msrHarmony>,
  
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
  public visitor<S_msrTechnicalWithString>,
  
  // ornaments

  public visitor<S_msrOrnament>,
  
  // tremolos

  public visitor<S_msrSingleTremolo>,
  
  public visitor<S_msrDoubleTremolo>,

  // dynamics

  public visitor<S_msrDynamics>,
  
  public visitor<S_msrOtherDynamics>,
  
  public visitor<S_msrSlur>,
  public visitor<S_msrLigature>,
  public visitor<S_msrWedge>,
  
  // grace notes

  public visitor<S_msrGraceNotes>,
  
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
    
  // ?

  public visitor<S_msrBarline>,
  
  public visitor<S_msrSegno>,
  public visitor<S_msrCoda>,
  
  public visitor<S_msrEyeGlasses>,
  public visitor<S_msrPedal>,

  public visitor<S_msrBarCheck>,
  public visitor<S_msrBarNumberCheck>,
  
  public visitor<S_msrLineBreak>,

  public visitor<S_msrPageBreak>,
  
  // repeats

  public visitor<S_msrRepeat>,
  public visitor<S_msrRepeatCommonPart>,
  public visitor<S_msrRepeatEnding>,
  
  public visitor<S_msrMeasureRepeat>,
  public visitor<S_msrMeasureRepeatPattern>,
  public visitor<S_msrMeasureRepeatReplicas>,
  
  public visitor<S_msrMultipleRest>,
  public visitor<S_msrMultipleRestContents>,
  
  // rehearsal

  public visitor<S_msrRehearsal>,
  
  // midi

  public visitor<S_msrMidi>

{
  public:
  
    msr2LpsrTranslator (
      indentedOutputStream& ios,
      S_msrScore            mScore);
        
    virtual ~msr2LpsrTranslator ();

    void buildLpsrScoreFromMsrScore ();

    S_lpsrScore getLpsrScore () const
        { return fLpsrScore; };
    
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

    virtual void visitStart (S_msrStaffLinesNumber& elt);
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

    virtual void visitStart (S_msrWords& elt);
    virtual void visitEnd   (S_msrWords& elt);

    virtual void visitStart (S_msrTempo& elt);
    virtual void visitEnd   (S_msrTempo& elt);

    virtual void visitStart (S_msrSegment& elt);
    virtual void visitEnd   (S_msrSegment& elt);

    virtual void visitStart (S_msrHarmony& elt);
    virtual void visitEnd   (S_msrHarmony& elt);
    
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

    virtual void visitStart (S_msrTechnicalWithString& elt);
    virtual void visitEnd   (S_msrTechnicalWithString& elt);

    virtual void visitStart (S_msrOrnament& elt);
    virtual void visitEnd   (S_msrOrnament& elt);

    virtual void visitStart (S_msrSingleTremolo& elt);
    virtual void visitEnd   (S_msrSingleTremolo& elt);

    virtual void visitStart (S_msrDoubleTremolo& elt);
    virtual void visitEnd   (S_msrDoubleTremolo& elt);

    virtual void visitStart (S_msrDynamics& elt);
    virtual void visitEnd   (S_msrDynamics& elt);
    
    virtual void visitStart (S_msrOtherDynamics& elt);
    virtual void visitEnd   (S_msrOtherDynamics& elt);

    virtual void visitStart (S_msrWedge& elt);
    virtual void visitEnd   (S_msrWedge& elt);

    virtual void visitStart (S_msrGraceNotes& elt);
    virtual void visitEnd   (S_msrGraceNotes& elt);

    virtual void visitStart (S_msrNote& elt);
    virtual void visitEnd   (S_msrNote& elt);

    virtual void visitStart (S_msrOctaveShift& elt);
    virtual void visitEnd   (S_msrOctaveShift& elt);

    virtual void visitStart (S_msrAccordionRegistration& elt);
    
    virtual void visitStart (S_msrHarpPedalsTuning& elt);

    virtual void visitStart (S_msrStem& elt);
    virtual void visitEnd   (S_msrStem& elt);

    virtual void visitStart (S_msrBeam& elt);
    virtual void visitEnd   (S_msrBeam& elt);

    virtual void visitStart (S_msrChord& elt);
    virtual void visitEnd   (S_msrChord& elt);

    virtual void visitStart (S_msrTuplet& elt);
    virtual void visitEnd   (S_msrTuplet& elt);

    virtual void visitStart (S_msrTie& elt);
    virtual void visitEnd   (S_msrTie& elt);

    virtual void visitStart (S_msrSlur& elt);
    virtual void visitEnd   (S_msrSlur& elt);

    virtual void visitStart (S_msrLigature& elt);
    virtual void visitEnd   (S_msrLigature& elt);

    virtual void visitStart (S_msrBarline& elt);
    virtual void visitEnd   (S_msrBarline& elt);

    virtual void visitStart (S_msrSegno& elt);
    virtual void visitStart (S_msrCoda& elt);

    virtual void visitStart (S_msrEyeGlasses& elt);
    virtual void visitStart (S_msrPedal& elt);

    virtual void visitStart (S_msrBarCheck& elt);
    virtual void visitEnd   (S_msrBarCheck& elt);
    virtual void visitStart (S_msrBarNumberCheck& elt);
    virtual void visitEnd   (S_msrBarNumberCheck& elt);

    virtual void visitStart (S_msrLineBreak& elt);
    virtual void visitEnd   (S_msrLineBreak& elt);

    virtual void visitStart (S_msrPageBreak& elt);
    virtual void visitEnd   (S_msrPageBreak& elt);

    virtual void visitStart (S_msrRepeat& elt);
    virtual void visitEnd   (S_msrRepeat& elt);
    virtual void visitStart (S_msrRepeatCommonPart& elt);
    virtual void visitEnd   (S_msrRepeatCommonPart& elt);
    virtual void visitStart (S_msrRepeatEnding& elt);
    virtual void visitEnd   (S_msrRepeatEnding& elt);

    virtual void visitStart (S_msrMeasureRepeat& elt);
    virtual void visitEnd   (S_msrMeasureRepeat& elt);
    virtual void visitStart (S_msrMeasureRepeatPattern& elt);
    virtual void visitEnd   (S_msrMeasureRepeatPattern& elt);
    virtual void visitStart (S_msrMeasureRepeatReplicas& elt);
    virtual void visitEnd   (S_msrMeasureRepeatReplicas& elt);

    virtual void visitStart (S_msrMultipleRest& elt);
    virtual void visitEnd   (S_msrMultipleRest& elt);
    virtual void visitStart (S_msrMultipleRestContents& elt);
    virtual void visitEnd   (S_msrMultipleRestContents& elt);

    virtual void visitStart (S_msrVarValAssoc& elt);
    virtual void visitEnd   (S_msrVarValAssoc& elt);

    virtual void visitStart (S_msrPageGeometry& elt);
    virtual void visitEnd   (S_msrPageGeometry& elt);

    virtual void visitStart (S_msrLayout& elt);
    virtual void visitEnd   (S_msrLayout& elt);

    virtual void visitStart (S_msrRehearsal& elt);
    virtual void visitEnd   (S_msrRehearsal& elt);

    virtual void visitStart (S_msrMidi& elt);
    virtual void visitEnd   (S_msrMidi& elt);

  private:
                     
    indentedOutputStream&     fLogOutputStream;

    // the MSR score we're visiting
    // ------------------------------------------------------
    S_msrScore                fVisitedMsrScore;

    // the LPSR score we're building
    // ------------------------------------------------------
    S_lpsrScore               fLpsrScore;

    // it's header
    // ------------------------------------------------------
    S_lpsrHeader              fLpsrScoreHeader;

    // score
    // ------------------------------------------------------
    S_msrScore                fCurrentMsrScoreClone;
    
    // identification
    // ------------------------------------------------------
    bool                      fOnGoingIdentification;

    // header
    // ------------------------------------------------------
    bool                      fWorkNumberKnown;
    bool                      fWorkTitleKnown;
    bool                      fMovementNumberKnown;
    bool                      fMovementTitleKnown;
    

    // page geometry
    // ------------------------------------------------------

    // credits
    // ------------------------------------------------------
    S_msrCredit               fCurrentCredit;
    
    // part groups
    // ------------------------------------------------------
  //  S_msrPartGroup            fCurrentPartGroupClone; JMI

    // the current partGroup is the top of the stack
    stack<S_msrPartGroup>     fPartGroupsStack;
    
    // parts
    // ------------------------------------------------------
    S_msrPart                 fCurrentPartClone;
    S_lpsrPartBlock           fCurrentPartBlock;

    // staff details

    S_msrStaffLinesNumber     fCurrentStaffLinesNumberClone;
    S_msrStaffTuning          fCurrentStaffTuningClone;
    
    // staves
    // ------------------------------------------------------
    S_msrStaff                fCurrentStaffClone;
    S_lpsrStaffBlock          fCurrentStaffBlock;
    // prevent clef, key and time from being handled twice
    bool                      fOnGoingStaff;

    // voices
    // ------------------------------------------------------    
    S_msrVoice                fCurrentVoiceClone;
    map<S_msrNote, S_msrNote> fVoiceNotesMap;

    // harmonies
    // ------------------------------------------------------    
    bool                      fOnGoingHarmonyVoice;

    // figured bass
    // ------------------------------------------------------    
    bool                      fOnGoingFiguredBassVoice;
    S_msrFiguredBass          fCurrentFiguredBass;
    
    // repeats
    // ------------------------------------------------------
// JMI    S_msrRepeat               fCurrentRepeatClone;
// JMI    bool                      fRepeatHasBeenCreatedForCurrentPartClone;
    bool                      fOnGoingRepeat;
    S_msrRepeatCommonPart     fCurrentRepeatCommonPartClone;
    S_msrRepeatEnding         fCurrentRepeatEndingClone;
    int                       fCurrentRepeatEndingsNumber;
 //   S_lpsrRepeatalternative fCurrentLpsrRepeatalternative; // JMI

    // measure repeats
    // ------------------------------------------------------

    S_msrMeasureRepeatPattern   fCurrentMeasureRepeatPatternClone;
    S_msrMeasureRepeatReplicas  fCurrentMeasureRepeatReplicasClone;

    // multiple rests
    // ------------------------------------------------------

    S_msrMultipleRestContents   fCurrentMultipleRestContentsClone;

    // segments
    // ------------------------------------------------------
    // segments can be imbedded in others,
    // the current segment clone is the one at the top of the stack
    stack<S_msrSegment>       fCurrentSegmentClonesStack;
    
    // measures
    // ------------------------------------------------------
    // we need to count the measures for option fSeparatorLineEveryNMeasures,
    // since measure numbers are actually strings
    int                       fMeasuresCounter;
    
    S_msrMeasure              fCurrentMeasureClone;

    void                      finalizeCurrentMeasureClone (
                                int          inputLineNumber,
                                S_msrMeasure originalMeasure);

    // bar checks
    // ------------------------------------------------------
    S_msrBarCheck             fLastBarCheck;

    // notes
    // ------------------------------------------------------
    bool                      fOnGoingNote;
    S_msrNote                 fCurrentNoteClone;
    S_msrNote                 fFirstNoteCloneInVoice;
                                // to help workaround LilyPond issue 34
    S_msrNote                 fCurrentAfterGraceNotesNote;
                                // to help optimise after grace notes

    // double tremolos
    // ------------------------------------------------------
    S_msrDoubleTremolo        fCurrentDoubleTremoloClone;
    bool                      fOnGoingDoubleTremolo;
    
    // stems
    // ------------------------------------------------------
    S_msrStem                 fCurrentStem;
    
    // grace notes
    // ------------------------------------------------------
    S_msrGraceNotes           fCurrentGraceNotesClone;

    // afterGraceNotes optimisation
    S_msrAfterGraceNotes      fPendingAfterGraceNotes;
    
    void                      prependSkipGraceNotesToPartOtherVoices (
                                S_msrPart       fCurrentPartClone,
                                S_msrVoice      fCurrentVoiceClone,
                                S_msrGraceNotes skipGraceNotes);
    
    // chords
    // ------------------------------------------------------
    bool                      fOnGoingChord;
    S_msrChord                fCurrentChordClone;
    
    // tuplets
    // ------------------------------------------------------
//    S_msrTuplet             fCurrentTupletClone;
 //   bool                      fOnGoingTuplet;
    stack<S_msrTuplet>        fTupletClonesStack;

    // stanzas
    // ------------------------------------------------------
    S_msrStanza               fCurrentStanzaClone;
    bool                      fOnGoingStanza;

    // syllables
    // ------------------------------------------------------
    S_msrSyllable             fCurrentSyllableClone;
    bool                      fOnGoingSyllableExtend;

    // part groups block
    // the current partGroup command is the top of the stack
    stack<S_lpsrPartGroupBlock>
                              fPartGroupBlocksStack;
};


/*! @} */

}

#endif
