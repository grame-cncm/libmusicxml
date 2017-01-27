/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __lpsr2LilyPondTranslator__
#define __lpsr2LilyPondTranslator__

#include <map>
#include <vector>

#include "lpsr.h"

//#include "musicxml2msr.h"

namespace MusicXML2
{

/*!
\addtogroup lpsr2lilypond
@{
*/

/*!
\brief Produces a summary of a MusicXML part.

  A part summary consists in 
  - a number of staves
  - a map that associates each stave with the corresponding number of notes
  - a map that associates each stave with the corresponding voices and notes
  - a map that associates each stave with the correcponding lyricss
*/
class lpsr2LilyPondTranslator :

  // LPSR
  
  public visitor<S_lpsrScore>,
  
  public visitor<S_lpsrLilypondVarValAssoc>,
  public visitor<S_lpsrSchemeVarValAssoc>,

  public visitor<S_lpsrHeader>,
  public visitor<S_lpsrPaper>,
  public visitor<S_lpsrLayout>,
  
  public visitor<S_lpsrParallelMusic>,

  public visitor<S_lpsrScoreBlock>,
  public visitor<S_lpsrPartgroupBlock>,
  public visitor<S_lpsrPartBlock>,
  public visitor<S_lpsrStaffBlock>,
//  public visitor<S_lpsrNewStaffBlock>,
//  public visitor<S_lpsrNewStaffgroupBlock>,
  public visitor<S_lpsrUseVoiceCommand>,
  public visitor<S_lpsrNewLyricsBlock>,
  public visitor<S_lpsrVariableUseCommand>,
  
  public visitor<S_lpsrContext>,
  
  public visitor<S_lpsrBarCommand>,

  public visitor<S_lpsrComment>,

  // MSR
  
  public visitor<S_msrScore>,
  
  public visitor<S_msrPartgroup>,
  
  public visitor<S_msrPart>,
  
  public visitor<S_msrStaff>,
  
  public visitor<S_msrVoice>,
  
  public visitor<S_msrSegment>,
  public visitor<S_msrMeasure>,

  public visitor<S_msrLyrics>,
  public visitor<S_msrLyricschunk>,
  
  public visitor<S_msrClef>,
  public visitor<S_msrKey>,
  public visitor<S_msrTime>,
  
  public visitor<S_msrWords>,
      
  public visitor<S_msrTempo>,
      
  public visitor<S_msrArticulation>,
  
  public visitor<S_msrOrnament>,

  public visitor<S_msrDynamics>,
  public visitor<S_msrWedge>,
    
  public visitor<S_msrGracenotes>,
  
  public visitor<S_msrNote>,
  public visitor<S_msrOctaveShift>,
  
  public visitor<S_msrStem>,
  public visitor<S_msrBeam>,
    
  public visitor<S_msrChord>,
  
  public visitor<S_msrTuplet>,
  
  public visitor<S_msrTie>,
  
  public visitor<S_msrSlur>,
  
  public visitor<S_msrBarline>,
  
  public visitor<S_msrSegno>,
  public visitor<S_msrCoda>,
  
  public visitor<S_msrEyeglasses>,
  public visitor<S_msrPedal>,

  public visitor<S_msrBarCheck>,
  
  public visitor<S_msrBarnumberCheck>,
  
  public visitor<S_msrBreak>,
   
  public visitor<S_msrRepeat>,
  public visitor<S_msrRepeatending>,
      
  public visitor<S_msrRehearsal>,
  
  public visitor<S_msrMidi>

{
  public:
  
    lpsr2LilyPondTranslator (
      S_msrOptions&  msrOpts,
      S_lpsrOptions& lpsrOpts,
      ostream&       os,
      S_lpsrScore    lpScore);
        
    virtual ~lpsr2LilyPondTranslator ();

    void generateLilyPondCodeFromLpsrScore ();

    S_lpsrScore getLpsrScore () const
        { return fLpsrScore; };
    
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

    virtual void visitStart (S_lpsrPartgroupBlock& elt);
    virtual void visitEnd   (S_lpsrPartgroupBlock& elt);

    virtual void visitStart (S_lpsrPartBlock& elt);
    virtual void visitEnd   (S_lpsrPartBlock& elt);

    virtual void visitStart (S_lpsrStaffBlock& elt);
    virtual void visitEnd   (S_lpsrStaffBlock& elt);

//    virtual void visitStart (S_lpsrNewStaffgroupBlock& elt);
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

    virtual void visitStart (S_lpsrComment& elt);
    virtual void visitEnd   (S_lpsrComment& elt);

    // MSR

    virtual void visitStart (S_msrScore& elt);
    virtual void visitEnd   (S_msrScore& elt);

    virtual void visitStart (S_msrPartgroup& elt);
    virtual void visitEnd   (S_msrPartgroup& elt);

    virtual void visitStart (S_msrPart& elt);
    virtual void visitEnd   (S_msrPart& elt);

    virtual void visitStart (S_msrStaff& elt);
    virtual void visitEnd   (S_msrStaff& elt);

    virtual void visitStart (S_msrVoice& elt);
    virtual void visitEnd   (S_msrVoice& elt);

    virtual void visitStart (S_msrSegment& elt);
    virtual void visitEnd   (S_msrSegment& elt);

    virtual void visitStart (S_msrMeasure& elt);
    virtual void visitEnd   (S_msrMeasure& elt);

    virtual void visitStart (S_msrLyrics& elt);
    virtual void visitEnd   (S_msrLyrics& elt);

    virtual void visitStart (S_msrLyricschunk& elt);
    virtual void visitEnd   (S_msrLyricschunk& elt);

    virtual void visitStart (S_msrClef& elt);
    virtual void visitEnd   (S_msrClef& elt);

    virtual void visitStart (S_msrKey& elt);
    virtual void visitEnd   (S_msrKey& elt);

    virtual void visitStart (S_msrTime& elt);
    virtual void visitEnd   (S_msrTime& elt);

    virtual void visitStart (S_msrWords& elt);
    virtual void visitEnd   (S_msrWords& elt);

    virtual void visitStart (S_msrTempo& elt);
    virtual void visitEnd   (S_msrTempo& elt);

    virtual void visitStart (S_msrArticulation& elt);
    virtual void visitEnd   (S_msrArticulation& elt);

    virtual void visitStart (S_msrOrnament& elt);
    virtual void visitEnd   (S_msrOrnament& elt);

    virtual void visitStart (S_msrDynamics& elt);
    virtual void visitEnd   (S_msrDynamics& elt);

    virtual void visitStart (S_msrWedge& elt);
    virtual void visitEnd   (S_msrWedge& elt);

    virtual void visitStart (S_msrGracenotes& elt);
    virtual void visitEnd   (S_msrGracenotes& elt);

    virtual void visitStart (S_msrNote& elt);
    virtual void visitEnd   (S_msrNote& elt);

    virtual void visitStart (S_msrOctaveShift& elt);
    virtual void visitEnd   (S_msrOctaveShift& elt);

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

    virtual void visitStart (S_msrBarline& elt);
    virtual void visitEnd   (S_msrBarline& elt);

    virtual void visitStart (S_msrSegno& elt);
    virtual void visitStart (S_msrCoda& elt);

    virtual void visitStart (S_msrEyeglasses& elt);
    virtual void visitStart (S_msrPedal& elt);

    virtual void visitStart (S_msrBarCheck& elt);
    virtual void visitEnd   (S_msrBarCheck& elt);
    
    virtual void visitStart (S_msrBarnumberCheck& elt);
    virtual void visitEnd   (S_msrBarnumberCheck& elt);

    virtual void visitStart (S_msrBreak& elt);
    virtual void visitEnd   (S_msrBreak& elt);

    virtual void visitStart (S_msrRepeat& elt);
    virtual void visitEnd   (S_msrRepeat& elt);
    virtual void visitStart (S_msrRepeatending& elt);
    virtual void visitEnd   (S_msrRepeatending& elt);

    virtual void visitStart (S_msrRehearsal& elt);
    virtual void visitEnd   (S_msrRehearsal& elt);

    virtual void visitStart (S_msrMidi& elt);
    virtual void visitEnd   (S_msrMidi& elt);

    string      absoluteOctaveAsLilypondString (
                  int absoluteOctave);

    string      noteMsrPitchAsLilyPondString (S_msrNote& note);

    string      ornamentKindAsLilyPondString (
                  int                          inputLineNumber,
                  msrOrnament::msrOrnamentKind ornamentKind);

  private:
                     
    S_msrOptions          fMsrOptions;
    S_lpsrOptions         fLpsrOptions;
    ostream&              fOstream;
    S_lpsrScore           fVisitedLpsrScore;

    // the LPSR score we're building
    // ------------------------------------------------------
    S_lpsrScore           fLpsrScore;

    // score
    // ------------------------------------------------------
    S_msrScore            fCurrentMsrScoreClone;
    bool                  fOnGoingScoreBlock;
    
    // part groups
    // ------------------------------------------------------
    S_msrPartgroup        fCurrentMsrPartgroupClone;
    
    // parts
    // ------------------------------------------------------
    S_msrPart             fCurrentMsrPartClone;
    
    // staves
    // ------------------------------------------------------
   // JMI S_msrStaff            fCurrentMsrStaffClone;
    // prevent clef, key and time from being handled twice
    bool                  fOnGoingStaff;

    // voices
    // ------------------------------------------------------
 // JMI   S_msrVoice           fCurrentMsrVoiceClone;
    
    // relative octaves
    // ------------------------------------------------------
    S_msrNote             fRelativeOctaveReference;
                            // contains absolute octave
    
    // repeats
    // ------------------------------------------------------

    // segments
    // ------------------------------------------------------
    // we need to handle embedded msrSegment's to indent notes
    // in a consistent way
    stack<int>            fSegmentNotesAndChordsCountersStack;

    // lyrics
    // ------------------------------------------------------
    S_msrLyrics           fCurrentMsrLyricsClone;
    bool                  fOngoingNonEmptyLyrics;
    int                   fLyricschunksCounter;
    
    // lyrics chunks
    // ------------------------------------------------------
    S_msrLyricschunk      fCurrentMsrLyricschunkClone;

    // stems
    msrStem::msrStemKind  fCurrentStemKind;
    S_msrStem             fCurrentStem;

    // chords
    // ------------------------------------------------------
    
    // tuplets
    // ------------------------------------------------------
// JMI     S_msrTuplet          fCurrentMsrTupletClone;

    // header handling
    // ------------------------------------------------------
    bool                  fOnGoingHeader;

    // limiting line size
    outputLineElementsCounter
                          fMusicOlec;
    outputLineElementsCounter
                          fLyricsOlec;
};


/*! @} */

}

#endif
