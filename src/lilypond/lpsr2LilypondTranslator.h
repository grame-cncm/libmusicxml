/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsr2LilypondTranslator___
#define ___lpsr2LilypondTranslator___

#include "msrOptions.h"
#include "lpsrOptions.h"

#include "lpsr.h"


namespace MusicXML2
{

//________________________________________________________________________
struct msrRepeatDescr : public smartable
{
/*
 * positions represent the order in which the parts appear in <part-list />
*/
 
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrRepeatDescr> create (
      int repeatEndingsNumber);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRepeatDescr (
      int repeatEndingsNumber);

    virtual ~msrRepeatDescr ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getRepeatEndingsNumber () const
                              { return fRepeatEndingsNumber; }

    void                  incrementRepeatEndingsCounter ()
                              { fRepeatEndingsCounter ++; }

    int                   getRepeatEndingsCounter () const
                              { return fRepeatEndingsCounter; }

    void                  setEndOfRepeatHasBeenGenerated ()
                              { fEndOfRepeatHasBeenGenerated = true; }
                              
    bool                  getEndOfRepeatHasBeenGenerated () const
                              { return fEndOfRepeatHasBeenGenerated; }
    
    // services
    // ------------------------------------------------------

    string                repeatDescrAsString () const;

    // print
    // ------------------------------------------------------
    
    virtual void          print (ostream& os) const;
    
  private:
     
    // fields
    // ------------------------------------------------------

    int                   fRepeatEndingsNumber;
    int                   fRepeatEndingsCounter;
    
    bool                  fEndOfRepeatHasBeenGenerated;
};
typedef SMARTP<msrRepeatDescr> S_msrRepeatDescr;
EXP ostream& operator<< (ostream& os, const S_msrRepeatDescr& elt);

//________________________________________________________________________
class lpsr2LilypondTranslator :

  // LPSR
  
  public visitor<S_lpsrScore>,
  
  // variable-value associations

  public visitor<S_lpsrVarValAssoc>,
  public visitor<S_lpsrVarValsListAssoc>,
  public visitor<S_lpsrSchemeVariable>,

  // header

  public visitor<S_lpsrHeader>,

  // paper

  public visitor<S_lpsrPaper>,

  // layout

  public visitor<S_lpsrLayout>,
  
  // score blocks

  public visitor<S_lpsrParallelMusicBLock>,

  public visitor<S_lpsrScoreBlock>,
  public visitor<S_lpsrPartGroupBlock>,
  public visitor<S_lpsrPartBlock>,
  public visitor<S_lpsrStaffBlock>,

  public visitor<S_lpsrUseVoiceCommand>,
  public visitor<S_lpsrNewLyricsBlock>,
  public visitor<S_lpsrVariableUseCommand>,
  
//  public visitor<S_lpsrContext>, JMI
  public visitor<S_lpsrChordNamesContext>,
  public visitor<S_lpsrFiguredBassContext>,
  
  public visitor<S_lpsrBarCommand>,
  
 // JMI public visitor<S_lpsrMelismaCommand>,

  public visitor<S_lpsrComment>,
  
  public visitor<S_lpsrSchemeFunction>,

  // MSR
  
  public visitor<S_msrScore>,

  // rights

  public visitor<S_msrCredit>,
  public visitor<S_msrCreditWords>,
  
  // parts & part groups

  public visitor<S_msrPartGroup>,
  
  public visitor<S_msrPart>,
  
  // staff details

  public visitor<S_msrStaffTuning>,
  public visitor<S_msrStaffDetails>,

  // staves and voices
  
  public visitor<S_msrStaff>,
  
  public visitor<S_msrVoice>,
  
  public visitor<S_msrVoiceStaffChange>,
  
  // harmonies

  public visitor<S_msrHarmony>,
  
  // frames

  public visitor<S_msrFrame>,
  
  // figured bass

  public visitor<S_msrFiguredBass>,
  public visitor<S_msrFigure>,

  public visitor<S_msrSegment>,
  public visitor<S_msrMeasure>,

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
  public visitor<S_msrTempoRelationshipElements>,
  public visitor<S_msrTempoNote>,
  public visitor<S_msrTempoTuplet>,
      
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
  
  public visitor<S_msrWedge>,
    
  // grace notes

  public visitor<S_msrGraceNotesGroup>,
  
  public visitor<S_msrAfterGraceNotesGroup>,
  public visitor<S_msrAfterGraceNotesGroupContents>,
  
  // notes

  public visitor<S_msrNote>,
  public visitor<S_msrOctaveShift>,
  
  // accordion registration

  public visitor<S_msrAccordionRegistration>,
  
  // harp pedals tuning

  public visitor<S_msrHarpPedalsTuning>,
  
  public visitor<S_msrStem>,
  public visitor<S_msrBeam>,
    
  // chords

  public visitor<S_msrChord>,
  
  public visitor<S_msrTuplet>,
  
  public visitor<S_msrTie>,
  
  public visitor<S_msrSlur>,
  
  public visitor<S_msrLigature>,
  
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
      
  // rehearsal

  public visitor<S_msrRehearsal>,
  
  // midi

  public visitor<S_msrMidi>

{
  public:
  
    lpsr2LilypondTranslator (
      S_msrOptions&    msrOpts,
      S_lpsrOptions&   lpsrOpts,
      indentedOstream& logIOstream,
      indentedOstream& lilypondCodeIOstream,
      S_lpsrScore      lpsrScore);
        
    virtual ~lpsr2LilypondTranslator ();

    void generateLilypondCodeFromLpsrScore ();
    
  protected:
      
    // LPSR
  
    virtual void visitStart (S_lpsrScore& elt);
    virtual void visitEnd   (S_lpsrScore& elt);

    virtual void visitStart (S_lpsrVarValAssoc& elt);
    virtual void visitEnd   (S_lpsrVarValAssoc& elt);

    virtual void visitStart (S_lpsrVarValsListAssoc& elt);
    virtual void visitEnd   (S_lpsrVarValsListAssoc& elt);

    virtual void visitStart (S_lpsrSchemeVariable& elt);
    virtual void visitEnd   (S_lpsrSchemeVariable& elt);

    virtual void visitStart (S_lpsrHeader& elt);
    virtual void visitEnd   (S_lpsrHeader& elt);

    virtual void visitStart (S_lpsrPaper& elt);
    virtual void visitEnd   (S_lpsrPaper& elt);

    virtual void visitStart (S_lpsrLayout& elt);
    virtual void visitEnd   (S_lpsrLayout& elt);

    virtual void visitStart (S_lpsrScoreBlock& elt);
    virtual void visitEnd   (S_lpsrScoreBlock& elt);

    virtual void visitStart (S_lpsrParallelMusicBLock& elt);
    virtual void visitEnd   (S_lpsrParallelMusicBLock& elt);

    virtual void visitStart (S_lpsrPartGroupBlock& elt);
    virtual void visitEnd   (S_lpsrPartGroupBlock& elt);

    virtual void visitStart (S_lpsrPartBlock& elt);
    virtual void visitEnd   (S_lpsrPartBlock& elt);

    virtual void visitStart (S_lpsrStaffBlock& elt);
    virtual void visitEnd   (S_lpsrStaffBlock& elt);

    virtual void visitStart (S_lpsrUseVoiceCommand& elt);
    virtual void visitEnd   (S_lpsrUseVoiceCommand& elt);
  
    virtual void visitStart (S_lpsrNewLyricsBlock& elt);
    virtual void visitEnd   (S_lpsrNewLyricsBlock& elt);
    
    virtual void visitStart (S_lpsrVariableUseCommand& elt);
    virtual void visitEnd   (S_lpsrVariableUseCommand& elt);
    
 //   virtual void visitStart (S_lpsrContext& elt); JMI
//    virtual void visitEnd   (S_lpsrContext& elt);
    virtual void visitStart (S_lpsrChordNamesContext& elt);
    virtual void visitEnd   (S_lpsrChordNamesContext& elt);
    virtual void visitStart (S_lpsrFiguredBassContext& elt);
    virtual void visitEnd   (S_lpsrFiguredBassContext& elt);

    virtual void visitStart (S_lpsrBarCommand& elt);
    virtual void visitEnd   (S_lpsrBarCommand& elt);

// JMI    virtual void visitStart (S_lpsrMelismaCommand& elt);
 //   virtual void visitEnd   (S_lpsrMelismaCommand& elt);

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

    virtual void visitStart (S_msrStaffTuning& elt);
    virtual void visitStart (S_msrStaffDetails& elt);

    virtual void visitStart (S_msrStaff& elt);
    virtual void visitEnd   (S_msrStaff& elt);

    virtual void visitStart (S_msrVoice& elt);
    virtual void visitEnd   (S_msrVoice& elt);

    virtual void visitStart (S_msrVoiceStaffChange& elt);

    virtual void visitStart (S_msrHarmony& elt);
    
    virtual void visitStart (S_msrFrame& elt);
    
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
    
    virtual void visitStart (S_msrPartNameDisplay& elt);
    virtual void visitStart (S_msrPartAbbreviationDisplay& elt);

    virtual void visitStart (S_msrWords& elt);
    virtual void visitEnd   (S_msrWords& elt);

    virtual void visitStart (S_msrTempo& elt);
    virtual void visitEnd   (S_msrTempo& elt);
    virtual void visitStart (S_msrTempoRelationshipElements& elt);
    virtual void visitEnd   (S_msrTempoRelationshipElements& elt);
    virtual void visitStart (S_msrTempoNote& elt);
    virtual void visitStart (S_msrTempoTuplet& elt);
    virtual void visitEnd   (S_msrTempoTuplet& elt);

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

    virtual void visitStart (S_msrTechnicalWithFloat& elt);
    virtual void visitEnd   (S_msrTechnicalWithFloat& elt);

    virtual void visitStart (S_msrTechnicalWithString& elt);
    virtual void visitEnd   (S_msrTechnicalWithString& elt);

    virtual void visitStart (S_msrOrnament& elt);
    virtual void visitEnd   (S_msrOrnament& elt);

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

    virtual void visitStart (S_msrWedge& elt);
    virtual void visitEnd   (S_msrWedge& elt);

    virtual void visitStart (S_msrGraceNotesGroup& elt);
    virtual void visitEnd   (S_msrGraceNotesGroup& elt);

    virtual void visitStart (S_msrAfterGraceNotesGroup& elt);
    virtual void visitEnd   (S_msrAfterGraceNotesGroup& elt);
    virtual void visitStart (S_msrAfterGraceNotesGroupContents& elt);
    virtual void visitEnd   (S_msrAfterGraceNotesGroupContents& elt);

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
    
    virtual void visitStart (S_msrRehearsal& elt);
    virtual void visitEnd   (S_msrRehearsal& elt);

    virtual void visitStart (S_msrMidi& elt);
    virtual void visitEnd   (S_msrMidi& elt);

    // names

    string                nameAsLilypondString (
                            string name);
                              
    // variable-value associations

    string                lpsrVarValAssocKindAsLilypondString (
                            lpsrVarValAssoc::lpsrVarValAssocKind
                              lilyPondVarValAssocKind);

    string                lpsrVarValsListAssocKindAsLilypondString (
                            lpsrVarValsListAssoc::lpsrVarValsListAssocKind
                              lilyPondVarValsListAssocKind);

    void                  generateLpsrVarValsListAssocValues (
                            S_lpsrVarValsListAssoc varValsListAssoc);

    string                lpsrVarValAssocAsLilypondString (
                            S_lpsrVarValAssoc lpsrVarValAssoc,
                            int               fieldNameWidth);

    // whole notes
    // JMI
                            
    // names

    string                generateMultilineName (string theString);

    // octaves
    
    string                absoluteOctaveAsLilypondString (
                            int absoluteOctave);

    // alterations
    
    string                alterationKindAsLilypondString (
                            msrAlterationKind alterationKind);

    // durations
    
    rational              fLastMetWholeNotes;
    
    string                durationAsLilypondString (
                            int      inputLineNumber,
                            rational wholeNotes);

         /* JMI                   
    string                durationAsExplicitLilypondString ( // JMI
                            int      inputLineNumber,
                            rational wholeNotes);
*/

    // notes
    
    msrNote::msrNotePrintKind
                          fCurrentNotePrintKind;

    string                lilypondRelativeOctave (
                            S_msrNote note);

    string                notePitchAsLilypondString (S_msrNote note);
    
    string                pitchedRestAsLilypondString (S_msrNote note);

    void                  generateNoteBeams (S_msrNote note);
    void                  generateNoteSlurs (S_msrNote note);

    void                  generateNote (S_msrNote note);

    // articulations

    msrDirectionKind      fCurrentArpeggioDirectionKind;
    
    void                  generateNoteArticulation (
                            S_msrArticulation articulation);

    void                  generateChordArticulation (
                            S_msrArticulation articulation);

    // technicals
    
    string                technicalAsLilypondString (
                            S_msrTechnical technical);

    string                technicalWithIntegerAsLilypondString (
                            S_msrTechnicalWithInteger technicalWithInteger);

    string                technicalWithFloatAsLilypondString (
                            S_msrTechnicalWithFloat technicalWithFloat);

    string                technicalWithStringAsLilypondString (
                            S_msrTechnicalWithString technicalWithString);

    // ornaments
    
    void                  generateOrnament (
                            S_msrOrnament ornament);

    // trills

    bool                  fOnGoingTrillSpanner; // JMI
    
    // spanners

    msrPlacementKind      fCurrentSpannerPlacementKind;
    
    void                  generateCodeForSpannerBeforeNote (
                            S_msrSpanner spanner);

    void                  generateCodeForSpannerAfterNote (
                            S_msrSpanner spanner);

    // grace notes

    bool                  fOnGoingGraceNotesGroup;
    void                  generateGraceNotesGroup (
                            S_msrGraceNotesGroup graceNotesGroup);

    // tremolos
    
    string                singleTremoloDurationAsLilypondString (
                            S_msrSingleTremolo singleTremolo);

    // dynamics
    
    string                dynamicsAsLilypondString (
                            S_msrDynamics dynamics);

    // string tuning
    string                stringTuningAsLilypondString (
                            int               inputLineNumber,
                            S_msrStringTuning stringTuning);

    // harp pedals tuning
    
    string                harpPedalTuningAsLilypondString (
                            msrAlterationKind alterationKind);
   
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

    string                harmonyDegreeAlterationKindAsLilypondString (
                            msrAlterationKind harmonyDegreeAlterationKind);

    // frames

    string                frameAsLilypondString (
                            S_msrFrame frame);

    // figured bass
    
    S_msrFiguredBass      fCurrentFiguredBass;
    unsigned int          fCurrentFiguredBassFiguresCounter;

  private:
                     
    S_msrOptions          fMsrOptions;
    S_lpsrOptions         fLpsrOptions;
    
    indentedOstream&      fLogOutputStream;
    indentedOstream&      fLilypondCodeIOstream;
//    indentedOstream&      fNullIOstream; // immuably empty
    
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
    bool                  fVoiceIsCurrentlySenzaMisura; // JMI
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
    list<S_msrRepeatDescr>
                          fRepeatsDescrStack;

    // multiple rest measures
    // ------------------------------------------------------
    int                   fRemainingMultipleRestMeasuresNumber;
    bool                  fOnGoingMultipleRestMeasures;
    
    // segments
    // ------------------------------------------------------
    // we need to handle nested msrSegment's to indent notes
    // in a consistent way
    stack<int>            fSegmentNotesAndChordsCountersStack;

    // measures
    // ------------------------------------------------------
    // we need to count the measures
    // for option fSeparatorLineEveryNMeasures and bar number checks,
    // since measure numbers can actually be any strings
    int                   fCurrentVoiceMeasuresCounter;
    
    // notes
    // ------------------------------------------------------
    bool                  fOnGoingNote;

    // stems
    msrStem::msrStemKind  fCurrentStemKind;

    // double tremolos
    // ------------------------------------------------------
    
    // chords
    // ------------------------------------------------------
    list<int>             fPendingChordMemberNotesStringNumbers;
    bool                  fOnGoingChord;
    
    void                  generateChord (S_msrChord chord);

    // tuplets
    // ------------------------------------------------------
// JMI     S_msrTuplet          fCurrentMsrTupletClone;
    stack<S_msrTuplet>    fTupletsStack;

    // stanzas
    // ------------------------------------------------------
    S_msrStanza           fCurrentStanzaClone;
    bool                  fGenerateCodeForOngoingNonEmptyStanza;
    
    // syllables
    // ------------------------------------------------------
    S_msrSyllable         fCurrentMsrSyllableClone;

    // score blocks
    // ------------------------------------------------------
    bool                  fOnGoingScoreBlock; // JMI

    // parallel music
    // ------------------------------------------------------
    S_lpsrParallelMusicBLock
                          fCurrentParallelMusicBLock;
    
    // part group blocks
    // ------------------------------------------------------
    int                   fNumberOfPartGroupBlocks;
    int                   fPartGroupBlocksCounter;

    // part blocks
    // ------------------------------------------------------
    int                   fNumberOfPartGroupBlockElements;
    int                   fPartGroupBlockElementsCounter;

    // staff blocks
    // ------------------------------------------------------
    int                   fNumberOfStaffBlocksElements;
    int                   fStaffBlocksCounter;
};


}


#endif
