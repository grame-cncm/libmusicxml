/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msr2LpsrTranslator___
#define ___msr2LpsrTranslator___

#include <map>
#include <vector>

#include "lpsr.h"


namespace MusicXML2
{

//________________________________________________________________________
class msr2LpsrTranslator :

  public visitor<S_msrScore>,
  
  // rights

  public visitor<S_msrIdentification>,
  
  public visitor<S_msrCredit>,
  public visitor<S_msrCreditWords>,
  
  // variable-value associations
  
  public visitor<S_msrVarValAssoc>,
  public visitor<S_msrVarValsListAssoc>,
  
  // geometry

  public visitor<S_msrPageGeometry>,

  // layout

  public visitor<S_msrLayout>,
  
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

  public visitor<S_msrSlash>,

  public visitor<S_msrSlur>,
  public visitor<S_msrLigature>,
  public visitor<S_msrWedge>,
  
  // grace notes

  public visitor<S_msrGraceNotesGroup>,
  
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
  public visitor<S_msrDamp>,
  public visitor<S_msrDampAll>,
  public visitor<S_msrScordatura>,

  public visitor<S_msrBarCheck>,
  public visitor<S_msrBarNumberCheck>,
  
  public visitor<S_msrLineBreak>,

  public visitor<S_msrPageBreak>,
  
  // repeats

  public visitor<S_msrRepeat>,
  public visitor<S_msrRepeatCommonPart>,
  public visitor<S_msrRepeatEnding>,
  
  public visitor<S_msrMeasuresRepeat>,
  public visitor<S_msrMeasuresRepeatPattern>,
  public visitor<S_msrMeasuresRepeatReplicas>,
  
  public visitor<S_msrMultipleRest>,
  public visitor<S_msrMultipleRestContents>,
  
  // midi

  public visitor<S_msrMidi>

{
  public:
  
    msr2LpsrTranslator (
      indentedOstream& ios,
      S_msrScore       mScore);
        
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

    virtual void visitStart (S_msrSingleTremolo& elt);
    virtual void visitEnd   (S_msrSingleTremolo& elt);

    virtual void visitStart (S_msrDoubleTremolo& elt);
    virtual void visitEnd   (S_msrDoubleTremolo& elt);

    virtual void visitStart (S_msrDynamics& elt);
    virtual void visitEnd   (S_msrDynamics& elt);
    
    virtual void visitStart (S_msrOtherDynamics& elt);
    virtual void visitEnd   (S_msrOtherDynamics& elt);

    virtual void visitStart (S_msrSlash& elt);
    
    virtual void visitStart (S_msrWedge& elt);
    virtual void visitEnd   (S_msrWedge& elt);

    virtual void visitStart (S_msrGraceNotesGroup& elt);
    virtual void visitEnd   (S_msrGraceNotesGroup& elt);

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
    virtual void visitStart (S_msrDamp& elt);
    virtual void visitStart (S_msrDampAll& elt);
    virtual void visitStart (S_msrScordatura& elt);

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

    virtual void visitStart (S_msrMeasuresRepeat& elt);
    virtual void visitEnd   (S_msrMeasuresRepeat& elt);
    virtual void visitStart (S_msrMeasuresRepeatPattern& elt);
    virtual void visitEnd   (S_msrMeasuresRepeatPattern& elt);
    virtual void visitStart (S_msrMeasuresRepeatReplicas& elt);
    virtual void visitEnd   (S_msrMeasuresRepeatReplicas& elt);

    virtual void visitStart (S_msrMultipleRest& elt);
    virtual void visitEnd   (S_msrMultipleRest& elt);
    virtual void visitStart (S_msrMultipleRestContents& elt);
    virtual void visitEnd   (S_msrMultipleRestContents& elt);

    virtual void visitStart (S_msrVarValAssoc& elt);
    virtual void visitEnd   (S_msrVarValAssoc& elt);
    virtual void visitStart (S_msrVarValsListAssoc& elt);
    virtual void visitEnd   (S_msrVarValsListAssoc& elt);

    virtual void visitStart (S_msrPageGeometry& elt);
    virtual void visitEnd   (S_msrPageGeometry& elt);

    virtual void visitStart (S_msrLayout& elt);
    virtual void visitEnd   (S_msrLayout& elt);

    virtual void visitStart (S_msrMidi& elt);
    virtual void visitEnd   (S_msrMidi& elt);

  private:
                     
    indentedOstream&          fLogOutputStream;


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
    S_msrIdentification       fCurrentIdentification;


    // header
    // ------------------------------------------------------
    bool                      fWorkNumberKnown;
    bool                      fWorkTitleKnown;
    bool                      fMovementNumberKnown;
    bool                      fMovementTitleKnown;
    

    // paper
    // ------------------------------------------------------
    void                      setPaperIndentsIfNeeded (
                                S_msrPageGeometry pageGeometry);

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
    // ------------------------------------------------------

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
    S_msrVoice                fCurrentVoiceOriginal;
    map<S_msrNote, S_msrNote> fVoiceNotesMap;


    // harmonies
    // ------------------------------------------------------    
    bool                      fOnGoingHarmonyVoice;

    S_msrHarmony              fCurrentHarmonyClone;

    list<S_msrHarmony>        fPendingHarmoniesList;


    // frames
    // ------------------------------------------------------    
 //   bool                      fOnGoingFramesVoice; JMI
    
 //   list<S_msrFrame>          fPendingFramesList; // JMI


    // figured bass
    // ------------------------------------------------------    
    bool                      fOnGoingFiguredBassVoice;
    S_msrFiguredBass          fCurrentFiguredBass;

    
    // repeats
    // ------------------------------------------------------

    bool                      fOnGoingRepeat;
    S_msrRepeatCommonPart     fCurrentRepeatCommonPartClone;
    S_msrRepeatEnding         fCurrentRepeatEndingClone;


    // measure repeats
    // ------------------------------------------------------

    S_msrMeasuresRepeatPattern
                              fCurrentMeasuresRepeatPatternClone;
    S_msrMeasuresRepeatReplicas
                              fCurrentMeasuresRepeatReplicasClone;

    // multiple rests
    // ------------------------------------------------------

    S_msrMultipleRest         fCurrentMultipleRestClone; // JMI
    S_msrMultipleRestContents fCurrentMultipleRestContentsClone;


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

    // fCurrentNonGraceNoteClone is not used for grace notes,
    // which are visited while the note they're attached to
    // is being visited too
    S_msrNote                 fCurrentNonGraceNoteClone;
    
    // to help workaround LilyPond issue 34
    S_msrNote                 fFirstNoteCloneInVoice;

    S_msrGraceNotesGroup      fCurrentSkipGraceNotesGroup;


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

    // afterGraceNotes optimisation
    S_msrAfterGraceNotesGroup fPendingAfterGraceNotesGroup;
    S_msrElement              fCurrentAfterGraceNotesGroupElement;

    /* JMI
    void                      prependSkipGraceNotesGroupToPartOtherVoices (
                                S_msrPart            partClone,
                                S_msrVoice           voiceClone,
                                S_msrGraceNotesGroup skipGraceNotesGroup);
                                */

    
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


}


#endif
