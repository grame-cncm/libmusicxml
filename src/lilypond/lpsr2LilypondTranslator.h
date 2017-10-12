/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __lpsr2LilypondTranslator__
#define __lpsr2LilypondTranslator__

#include "msrOptions.h"
#include "lpsrOptions.h"

#include "lpsr.h"


namespace MusicXML2
{

/*!
\addtogroup lpsr2lilypond
@{
*/

class lpsr2LilypondTranslator :

  // LPSR
  
  public visitor<S_lpsrScore>,
  
  public visitor<S_lpsrLilypondVarValAssoc>,
  public visitor<S_lpsrSchemeVarValAssoc>,

  public visitor<S_lpsrHeader>,
  public visitor<S_lpsrPaper>,
  public visitor<S_lpsrLayout>,
  
  public visitor<S_lpsrParallelMusic>,

  public visitor<S_lpsrScoreBlock>,
  public visitor<S_lpsrPartGroupBlock>,
  public visitor<S_lpsrPartBlock>,
  public visitor<S_lpsrStaffBlock>,
//  public visitor<S_lpsrNewStaffBlock>, JMI
//  public visitor<S_lpsrNewStaffgroupBlock>,
  public visitor<S_lpsrUseVoiceCommand>,
  public visitor<S_lpsrNewLyricsBlock>,
  public visitor<S_lpsrVariableUseCommand>,
  
  public visitor<S_lpsrContext>,
  
  public visitor<S_lpsrBarCommand>,
  
  public visitor<S_lpsrMelismaCommand>,

  public visitor<S_lpsrComment>,
  
  public visitor<S_lpsrSchemeFunction>,

  // MSR
  
  public visitor<S_msrScore>,
  
  public visitor<S_msrCredit>,
  public visitor<S_msrCreditWords>,
  
  public visitor<S_msrPartGroup>,
  
  public visitor<S_msrPart>,
  
  public visitor<S_msrStaffLinesNumber>,
  public visitor<S_msrStaffTuning>,
  public visitor<S_msrStaffDetails>,
  
  public visitor<S_msrStaff>,
  
  public visitor<S_msrVoice>,
  
  public visitor<S_msrVoiceStaffChange>,
  
  public visitor<S_msrHarmony>,
  
  public visitor<S_msrFiguredBass>,
  public visitor<S_msrFigure>,

  public visitor<S_msrSegment>,
  public visitor<S_msrMeasure>,

  public visitor<S_msrStanza>,
  public visitor<S_msrSyllable>,
  
  public visitor<S_msrClef>,
  public visitor<S_msrKey>,
  public visitor<S_msrTime>,
  
  public visitor<S_msrTranspose>,
  
  public visitor<S_msrWords>,
      
  public visitor<S_msrTempo>,
      
  public visitor<S_msrArticulation>,
  
  public visitor<S_msrFermata>,
  
  public visitor<S_msrArpeggiato>,
  public visitor<S_msrNonArpeggiato>,
  
  public visitor<S_msrTechnical>,
  public visitor<S_msrTechnicalWithInteger>,
  public visitor<S_msrTechnicalWithString>,
  
  public visitor<S_msrOrnament>,
  
  public visitor<S_msrSingleTremolo>,
  
  public visitor<S_msrDoubleTremolo>,

  public visitor<S_msrDynamics>,
  
  public visitor<S_msrOtherDynamics>,
  
  public visitor<S_msrWedge>,
    
  public visitor<S_msrGraceNotes>,
  public visitor<S_msrAfterGraceNotes>,
  
  public visitor<S_msrNote>,
  public visitor<S_msrOctaveShift>,
  
  public visitor<S_msrAccordionRegistration>,
  
  public visitor<S_msrHarpPedalsTuning>,
  
  public visitor<S_msrStem>,
  public visitor<S_msrBeam>,
    
  public visitor<S_msrChord>,
  
  public visitor<S_msrTuplet>,
  
  public visitor<S_msrTie>,
  
  public visitor<S_msrSlur>,
  
  public visitor<S_msrLigature>,
  
  public visitor<S_msrBarline>,
  
  public visitor<S_msrSegno>,
  public visitor<S_msrCoda>,
  
  public visitor<S_msrEyeGlasses>,
  public visitor<S_msrPedal>,

  public visitor<S_msrBarCheck>,
  
  public visitor<S_msrBarNumberCheck>,
  
  public visitor<S_msrLineBreak>,
  
  public visitor<S_msrPageBreak>,
   
  public visitor<S_msrRepeat>,
  public visitor<S_msrRepeatCommonPart>,
  public visitor<S_msrRepeatEnding>,
  
  public visitor<S_msrMeasureRepeat>,
  public visitor<S_msrMeasureRepeatPattern>,
  public visitor<S_msrMeasureRepeatReplicas>,
  
  public visitor<S_msrMultipleRest>,
  public visitor<S_msrMultipleRestContents>,
      
  public visitor<S_msrRehearsal>,
  
  public visitor<S_msrMidi>

{
  public:
  
    lpsr2LilypondTranslator (
      S_msrOptions&  msrOpts,
      S_lpsrOptions& lpsrOpts,
      ostream&       os,
      S_lpsrScore    lpsrScore);
        
    virtual ~lpsr2LilypondTranslator ();

    void generateLilypondCodeFromLpsrScore ();
    
  protected:
      
    // LPSR
  
    virtual void visitStart (S_lpsrScore& elt);
    virtual void visitEnd   (S_lpsrScore& elt);

    virtual void visitStart (S_lpsrLilypondVarValAssoc& elt);
    virtual void visitEnd   (S_lpsrLilypondVarValAssoc& elt);

    virtual void visitStart (S_lpsrSchemeVarValAssoc& elt);
    virtual void visitEnd   (S_lpsrSchemeVarValAssoc& elt);

    virtual void visitStart (S_lpsrHeader& elt);
    virtual void visitEnd   (S_lpsrHeader& elt);

    virtual void visitStart (S_lpsrPaper& elt);
    virtual void visitEnd   (S_lpsrPaper& elt);

    virtual void visitStart (S_lpsrLayout& elt);
    virtual void visitEnd   (S_lpsrLayout& elt);

    virtual void visitStart (S_lpsrScoreBlock& elt);
    virtual void visitEnd   (S_lpsrScoreBlock& elt);

    virtual void visitStart (S_lpsrParallelMusic& elt);
    virtual void visitEnd   (S_lpsrParallelMusic& elt);

    virtual void visitStart (S_lpsrPartGroupBlock& elt);
    virtual void visitEnd   (S_lpsrPartGroupBlock& elt);

    virtual void visitStart (S_lpsrPartBlock& elt);
    virtual void visitEnd   (S_lpsrPartBlock& elt);

    virtual void visitStart (S_lpsrStaffBlock& elt);
    virtual void visitEnd   (S_lpsrStaffBlock& elt);

//    virtual void visitStart (S_lpsrNewStaffgroupBlock& elt); JMI
//    virtual void visitEnd   (S_lpsrNewStaffgroupBlock& elt);
    
//    virtual void visitStart (S_lpsrNewStaffBlock& elt);
//    virtual void visitEnd   (S_lpsrNewStaffBlock& elt);
    
    virtual void visitStart (S_lpsrUseVoiceCommand& elt);
    virtual void visitEnd   (S_lpsrUseVoiceCommand& elt);
  
    virtual void visitStart (S_lpsrNewLyricsBlock& elt);
    virtual void visitEnd   (S_lpsrNewLyricsBlock& elt);
    
    virtual void visitStart (S_lpsrVariableUseCommand& elt);
    virtual void visitEnd   (S_lpsrVariableUseCommand& elt);
    
    virtual void visitStart (S_lpsrContext& elt);
    virtual void visitEnd   (S_lpsrContext& elt);
  
    virtual void visitStart (S_lpsrBarCommand& elt);
    virtual void visitEnd   (S_lpsrBarCommand& elt);

    virtual void visitStart (S_lpsrMelismaCommand& elt);
    virtual void visitEnd   (S_lpsrMelismaCommand& elt);

    virtual void visitStart (S_lpsrComment& elt);
    virtual void visitEnd   (S_lpsrComment& elt);

    virtual void visitStart (S_lpsrSchemeFunction& elt);
    virtual void visitEnd   (S_lpsrSchemeFunction& elt);

    // MSR

    virtual void visitStart (S_msrScore& elt);
    virtual void visitEnd   (S_msrScore& elt);

    virtual void visitStart (S_msrCredit& elt);
    virtual void visitEnd   (S_msrCredit& elt);
    virtual void visitStart (S_msrCreditWords& elt);
    virtual void visitEnd   (S_msrCreditWords& elt);

    virtual void visitStart (S_msrPartGroup& elt);
    virtual void visitEnd   (S_msrPartGroup& elt);

    virtual void visitStart (S_msrPart& elt);
    virtual void visitEnd   (S_msrPart& elt);

    virtual void visitStart (S_msrStaffLinesNumber& elt);
    virtual void visitStart (S_msrStaffTuning& elt);
    virtual void visitStart (S_msrStaffDetails& elt);

    virtual void visitStart (S_msrStaff& elt);
    virtual void visitEnd   (S_msrStaff& elt);

    virtual void visitStart (S_msrVoice& elt);
    virtual void visitEnd   (S_msrVoice& elt);

    virtual void visitStart (S_msrVoiceStaffChange& elt);

    virtual void visitStart (S_msrHarmony& elt);
    
    virtual void visitStart (S_msrFiguredBass& elt);
    virtual void visitEnd   (S_msrFiguredBass& elt);
    virtual void visitStart (S_msrFigure& elt);
    
    virtual void visitStart (S_msrSegment& elt);
    virtual void visitEnd   (S_msrSegment& elt);

    virtual void visitStart (S_msrMeasure& elt);
    virtual void visitEnd   (S_msrMeasure& elt);

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

    virtual void visitStart (S_msrArticulation& elt);
    virtual void visitEnd   (S_msrArticulation& elt);

    virtual void visitStart (S_msrFermata& elt);
    virtual void visitEnd   (S_msrFermata& elt);
    
    virtual void visitStart (S_msrArpeggiato& elt);
    virtual void visitEnd   (S_msrArpeggiato& elt);
    virtual void visitStart (S_msrNonArpeggiato& elt);
    virtual void visitEnd   (S_msrNonArpeggiato& elt);
    
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

    virtual void visitStart (S_msrAfterGraceNotes& elt);
    virtual void visitEnd   (S_msrAfterGraceNotes& elt);

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
    
    virtual void visitStart (S_msrRehearsal& elt);
    virtual void visitEnd   (S_msrRehearsal& elt);

    virtual void visitStart (S_msrMidi& elt);
    virtual void visitEnd   (S_msrMidi& elt);

    // whole notes
    // JMI
                            
    // octaves
    
    string                absoluteOctaveAsLilypondString (
                            int absoluteOctave);

    // alterations
    
    string                alterationAsLilypondString (
                            msrAlteration alteration);

    string                alterationAsLilypondAccidentalMark (
                            msrAlteration alteration);

    // durations
    
    rational              fLastMetWholeNotes;
    
    string                durationAsLilypondString (
                            int      inputLineNumber,
                            rational wholeNotes);
    
    // notes
    
    string                lilypondRelativeOctave (
                            S_msrNote note);

    string                notePitchAsLilypondString (S_msrNote note);
    
    string                pitchedRestAsLilypondString (S_msrNote note);

    void                  printNoteAsLilypondString (S_msrNote note);

    // articulations
    
    string                articulationAsLilyponString (
                            S_msrArticulation articulation);

    // technicals
    
    string                technicalAsLilypondString (
                            S_msrTechnical technical);

    string                technicalWithIntegerAsLilypondString (
                            S_msrTechnicalWithInteger technicalWithInteger);

    string                technicalWithStringAsLilypondString (
                            S_msrTechnicalWithString technicalWithString);

    // ornaments
    
    string                ornamentAsLilypondString (
                            S_msrOrnament ornament);

    // tremolos
    
    string                singleTremoloDurationAsLilypondString (
                            S_msrSingleTremolo singleTremolo);

    // dynamics
    
    string                dynamicsAsLilypondString (
                            S_msrDynamics dynamics);

    // harp pedals tuning
    
    string                harpPedalTuningAsLilypondString (
                            msrAlteration alteration);
   
    // transpose
    
    void                  transposeDiatonicError (
                            int inputLineNumber,
                            int transposeDiatonic,
                            int transposeChromatic);

    // harmonies

    bool                  fPowerChordHaveAlreadyBeenGenerated;
                            // to generate it only once
    
    string                harmonyAsLilypondString (
                            S_msrHarmony harmony);

    string                harmonyDegreeAlterationAsLilypondString (
                            msrAlteration harmonyDegreeAlteration);

    // figured bass
    
    S_msrFiguredBass      fCurrentFiguredBass;
    unsigned int          fCurrentFiguredBassFiguresCounter;

  private:
                     
    S_msrOptions          fMsrOptions;
    S_lpsrOptions         fLpsrOptions;
    
    ostream&              fOstream;
    
    // the LPSR score we're visiting
    // ------------------------------------------------------
    S_lpsrScore           fVisitedLpsrScore;

    // header handling
    // ------------------------------------------------------
    bool                  fOnGoingHeader;

    // score
    // ------------------------------------------------------
    S_msrScore            fCurrentMsrScoreClone;

    // part groups
    // ------------------------------------------------------
 // JMI   S_msrPartGroup        fCurrentMsrPartGroupClone;
    
    // parts
    // ------------------------------------------------------
    S_msrPart             fCurrentPart; // JMI jamais visitee

    // time
    // ------------------------------------------------------
    bool                  fVoiceIsCurrentlySenzaMisura;
    bool                  fOnGoingVoiceCadenza;
    
    // staves
    // ------------------------------------------------------
    // prevent clef, key and time from being handled twice
    bool                  fOnGoingStaff;

    // voices
    // ------------------------------------------------------
    bool                  fOnGoingVoice;
    S_msrVoice            fCurrentVoice;
    
    bool                  fOnGoingHarmonyVoice;
    bool                  fOnGoingFiguredBassVoice;
    
    // relative octaves
    // ------------------------------------------------------
    S_msrNote             fRelativeOctaveReference;
                            // contains absolute octave
    
    // repeats
    // ------------------------------------------------------
    int                   fCurrentRepeatEndingsNumber;

    // segments
    // ------------------------------------------------------
    // we need to handle nested msrSegment's to indent notes
    // in a consistent way
    stack<int>            fSegmentNotesAndChordsCountersStack;

    // measures
    // ------------------------------------------------------
    // we need to count the measures for option fSeparatorLineEveryNMeasures,
    // since measure numbers are actually strings
    int                   fMeasuresCounter;
    
    // notes
    // ------------------------------------------------------
    bool                  fOnGoingNote;

    // stems
    msrStem::msrStemKind  fCurrentStemKind;
    S_msrStem             fCurrentStem;
    bool                  fOnGoingStemNone;

    // double tremolos
    // ------------------------------------------------------
    int                   fCurrentDoubleTremoloElementsLpsrDuration;
    
    // chords
    // ------------------------------------------------------
    bool                  fOnGoingChord; // JMI
    
    // tuplets
    // ------------------------------------------------------
// JMI     S_msrTuplet          fCurrentMsrTupletClone;
    stack<S_msrTuplet>    fTupletsStack;

    // stanzas
    // ------------------------------------------------------
    S_msrStanza           fCurrentStanzaClone;
    bool                  fOngoingNonEmptyStanza;
    
    // syllables
    // ------------------------------------------------------
    S_msrSyllable         fCurrentMsrSyllableClone;

    // score blocks
    // ------------------------------------------------------
    bool                  fOnGoingScoreBlock; // JMI
    
    // limiting output line size
    outputLineElementsCounter
                          fMusicOlec;
    outputLineElementsCounter
                          fStanzaOlec;
};


/*! @} */

}

#endif
