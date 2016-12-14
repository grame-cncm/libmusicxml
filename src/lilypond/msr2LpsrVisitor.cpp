/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <sstream>
#include <stdlib.h>     /* abort, NULL */
#include <climits>      /* INT_MIN */
#include <algorithm>    /* for_each */

#include "conversions.h"

#include "msr2LpsrVisitor.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
msr2LpsrVisitor::msr2LpsrVisitor (
  S_msrOptions&  msrOpts,
  S_lpsrOptions& lpsrOpts,
  ostream&       os,
  S_msrScore     mScore)
    : fOstream (os)
{
  fMsrOptions      = msrOpts;
  fLpsrOptions     = lpsrOpts;
  fVisitedMsrScore = mScore;

// JMI  gCurrentLocation.fMeasureNumber     = 0; // in case of an anacrusis
 // gCurrentLocation.fPositionInMeasure = 1;

  fOnGoingIdentification = false;

  fCurrentStemDirection  = kStemNeutral;
  
  fOnGoingStaff          = false;

  fOnGoingRepeat         = false;
};
  
msr2LpsrVisitor::~msr2LpsrVisitor ()
{}

//________________________________________________________________________
void msr2LpsrVisitor::buildLpsrScoreFromMsrScore ()
{
  if (fVisitedMsrScore) {    
    // create a msrScore browser
    msrBrowser<msrScore> browser (this);

    // browse the score with the browser
    browser.browse (*fVisitedMsrScore);
  }
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrScore& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrScore" << endl;

  // create an empty clone of fVisitedMsrScore for use by the LPSR score
  // not sharing the visitiged MSR score allows cleaner data handling
  // and optimisations of the LPSR data
  fCurrentScoreClone =
    fVisitedMsrScore->createEmptyClone ();

  // create the LPSR score
  fLpsrScore =
    lpsrScore::create (
      fMsrOptions, fLpsrOptions, 0, fCurrentScoreClone);
/*
  // push it onto this visitors's stack,
  // making it the current partgroup command
  fPartgroupBlocksStack.push (
    partgroupCommand);
    */
}

void msr2LpsrVisitor::visitEnd (S_msrScore& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrScore" << endl;

/* JMI
  // get top level pargroup command from the stack
  S_lpsrPartgroupBlock
    partgroupCommand =
      fPartgroupBlocksStack.top ();

  // pop it from the stack
  fPartgroupBlocksStack.top ();

  // the stack should now be empty
  if (fPartgroupBlocksStack.size())
    msrInternalError (
      1,
      "the partgroup command stack is not exmpty at the end of the visit");
   */ 
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrIdentification& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrIdentification" << endl;

  idtr++;

  fOnGoingIdentification = true;
}

void msr2LpsrVisitor::visitEnd (S_msrIdentification& elt)
{
  fOnGoingIdentification = false;
  
  idtr--;

  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrIdentification" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrPageGeometry& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrPageGeometry" << endl;

  idtr++;

  // get LPSR score paper
  S_lpsrPaper
    paper =
      fLpsrScore->getPaper ();

  // populate paper  
  paper ->
    setPaperWidth (elt->getPaperWidth ());
  paper->
    setPaperHeight (elt->getPaperHeight ());
    
  paper->
    setTopMargin (elt->getTopMargin ());
  paper->
    setBottomMargin (elt->getBottomMargin ());
  paper->
    setLeftMargin (elt->getLeftMargin ());
  paper->
    setRightMargin (elt->getRightMargin ());

  // get LPSR score layout
  S_lpsrLayout
    scoreLayout =
      fLpsrScore->getScoreLayout ();

  // get LPSR score global staff size
  float
    globalStaffSize =
      elt->globalStaffSize ();

  // populate layout
  /*
  scoreLayout->
    setMillimeters (elt->getMillimeters ());
  scoreLayout->
    setTenths (elt->getTenths ());
    */

  // populate score global staff size
  fLpsrScore->
    setGlobalStaffSize (globalStaffSize);

  // get LPSR score command layout
  S_lpsrLayout
    scoreCommandLayout =
      fLpsrScore->getScoreBlock ()->getScoreBlockLayout ();

  // create the score command layout staff-size Scheme assoc
  stringstream s;
  s << globalStaffSize;
  S_lpsrSchemeVarValAssoc
    assoc =
      lpsrSchemeVarValAssoc::create (
        fMsrOptions,
        fLpsrOptions,
        0, // JMI
        lpsrSchemeVarValAssoc::kCommented,
        "layout-set-staff-size",
        s.str(),
        "Uncomment and adapt next line as needed (default is 20)",
        lpsrSchemeVarValAssoc::kWithEndl);

  // populate score command layout
  scoreCommandLayout->
    addSchemeVarValAssoc (assoc);

/* JMI
    void    setBetweenSystemSpace (float val) { fBetweenSystemSpace = val; }
    float   getBetweenSystemSpace () const    { return fBetweenSystemSpace; }

    void    setPageTopSpace       (float val) { fPageTopSpace = val; }
   */
}

void msr2LpsrVisitor::visitEnd (S_msrPageGeometry& elt)
{  
  idtr--;

  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrPageGeometry" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrPartgroup& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrPartgroup" << endl;

  // create a partgroup clone
  fCurrentPartgroupClone =
    elt->createEmptyClone (fCurrentPartgroupClone);

  // add it to the MSR score clone
  fCurrentScoreClone->
    addPartgroupToScore (fCurrentPartgroupClone);

  // create a partgroup command
  S_lpsrPartgroupBlock
    partgroupCommand =
      lpsrPartgroupBlock::create (
        fMsrOptions, fLpsrOptions, fCurrentPartgroupClone);

  // push it onto this visitors's stack,
  // making it the current partgroup command
  fPartgroupBlocksStack.push (
    partgroupCommand);
  
  // get the LPSR store command
  S_lpsrScoreBlock
    scoreCommand =
      fLpsrScore->getScoreBlock ();

  // append the pargroup clone to the score command
  scoreCommand->
    appendPartgroupBlockToParallelMusic (
      partgroupCommand);

  // append the partgroup command to the LPSR score command

/*
 *   fCurrentVoiceClone =
    elt->createEmptyClone (fCurrentStaffClone);
    
  fCurrentStaffClone->
    addVoiceToStaff (fCurrentVoiceClone);

  // append the voice to the LPSR score elements list
  fLpsrScore ->
    appendVoiceToScoreElements (fCurrentVoiceClone);

  // append the voice use to the LPSR score command
  fLpsrScore ->
    appendVoiceUseToStoreCommand (fCurrentVoiceClone);
*/

  idtr++;
}

void msr2LpsrVisitor::visitEnd (S_msrPartgroup& elt)
{
  idtr--;

  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrPartgroup" << endl;

  // pop current partgroup from this visitors's stack
  fPartgroupBlocksStack.pop ();
      
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrPart& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrPart" << endl;

  idtr++;

  // create a part clone
  fCurrentPartClone =
    elt->createEmptyClone (fCurrentPartgroupClone);

  // add it to the partgroup clone
  fCurrentPartgroupClone->
    addPartToPartgroup (fCurrentPartClone);

  // create a part command
  fCurrentPartBlock =
    lpsrPartBlock::create (
      fMsrOptions, fLpsrOptions, fCurrentPartClone);

  // append it to the current partgroup command
  fPartgroupBlocksStack.top ()->
    appendElementToPartgroupBlock (fCurrentPartBlock);
}

void msr2LpsrVisitor::visitEnd (S_msrPart& elt)
{
  idtr--;

  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrPart" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrStaff& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrStaff" << endl;

  idtr++;
  
  // create a staff clone
  fCurrentStaffClone =
    elt->createEmptyClone (fCurrentPartClone);
    
  // add it to the part clone
  fCurrentPartClone->
    addStaffToPart (fCurrentStaffClone);

  // create a staff command
  fCurrentStaffBlock =
    lpsrStaffBlock::create (
      fMsrOptions, fLpsrOptions, fCurrentStaffClone);

  // append it to the current part command
  fCurrentPartBlock->
    appendElementToPartBlock (fCurrentStaffBlock);

  fOnGoingStaff = true;
}

void msr2LpsrVisitor::visitEnd (S_msrStaff& elt)
{
  idtr--;

  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrStaff" << endl;

  fOnGoingStaff = false;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrVoice& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrVoice" << endl;

  idtr++;

  // create a voice clone
  fCurrentVoiceClone =
    elt->createEmptyClone (fCurrentStaffClone);
    
  // add it to the staff clone
  fCurrentStaffClone->
    addVoiceToStaff (fCurrentVoiceClone);

  // append the voice to the LPSR score elements list
  fLpsrScore ->
    appendVoiceToScoreElements (fCurrentVoiceClone);

  // append a use of the voice to the current staff command
  fCurrentStaffBlock->
    appendVoiceUseToStaffBlock (fCurrentVoiceClone);
}

void msr2LpsrVisitor::visitEnd (S_msrVoice& elt)
{
  idtr--;

  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrVoice" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrUpbeat& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrUpbeat" << endl;

  idtr++;

  // create a upbeat clone
  S_msrUpbeat
    upbeat =
      elt->createEmptyClone (fCurrentVoiceClone);
    
  // add it to the current voice clone
  fCurrentVoiceClone->
    setVoiceAnacrusis (upbeat);
}

void msr2LpsrVisitor::visitEnd (S_msrUpbeat& elt)
{
  idtr--;

  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrUpbeat" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrVoicechunk& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrVoicechunk" << endl;

  // create a clone of the voice chunk
  fCurrentVoicechunkClone =
    elt->createEmptyClone ();
/* JMI
  // append it to the current voice
  fCurrentVoiceClone->
    appendVoicechunkToVoice (
      fCurrentVoicechunkClone);
      */
}

void msr2LpsrVisitor::visitEnd (S_msrVoicechunk& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrVoicechunk" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrLyrics& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrLyrics" << endl;

  idtr++;

//  if (elt->getLyricsTextPresent ()) { // JMI
    fCurrentLyricsClone =
      elt->createEmptyClone (fCurrentVoiceClone);
  
    // don't add the lyrics to fCurrentVoiceClone
  
    // append the lyrics to the LPSR score elements list
    fLpsrScore ->
      appendLyricsToScoreElements (fCurrentLyricsClone);
  
    // append a use of the lyrics to the current staff block
    fCurrentStaffBlock ->
      appendLyricsUseToStaffBlock (fCurrentLyricsClone);
//  }
//  else
  //  fCurrentLyricsClone = 0; // JMI
}

void msr2LpsrVisitor::visitEnd (S_msrLyrics& elt)
{
  idtr--;
  
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrLyrics" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrLyricschunk& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrLyricschunk" << endl;

  fCurrentLyricschunkClone =
    elt->createEmptyClone ();
    
// JMI  fCurrentLyricsClone->
    //addChunkToLyrics (fCurrentLyricschunkClone);
  fCurrentLyricsClone->
    addChunkToLyrics (elt);
}

void msr2LpsrVisitor::visitEnd (S_msrLyricschunk& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrLyricschunk" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrClef& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrClef" << endl;

  fCurrentVoiceClone->
    appendClefToVoice (elt);
}

void msr2LpsrVisitor::visitEnd (S_msrClef& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrClef" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrKey& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrKey" << endl;

  fCurrentVoiceClone->
    appendKeyToVoice (elt);
}

void msr2LpsrVisitor::visitEnd (S_msrKey& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrKey" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrTime& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrTime" << endl;

  fCurrentVoiceClone->
    appendTimeToVoice (elt);
}

void msr2LpsrVisitor::visitEnd (S_msrTime& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrTime" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrWords& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrWords" << endl;

  fCurrentVoiceClone->
    appendWordsToVoice (elt);
}

void msr2LpsrVisitor::visitEnd (S_msrWords& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrWords" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrTempo& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrTempo" << endl;

  fCurrentVoiceClone->
    appendTempoToVoice (elt);
}

void msr2LpsrVisitor::visitEnd (S_msrTempo& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrTempo" << endl;
}

//________________________________________________________________________
/*
void msr2LpsrVisitor::visitStart (S_msrDuration& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrDuration" << endl;
}

void msr2LpsrVisitor::visitEnd (S_msrDuration& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrDuration" << endl;
}
*/

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrArticulation& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrArticulation" << endl;
}

void msr2LpsrVisitor::visitEnd (S_msrArticulation& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrArticulation" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrDynamics& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrDynamics" << endl;
}

void msr2LpsrVisitor::visitEnd (S_msrDynamics& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrDynamics" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrWedge& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrWedge" << endl;
}

void msr2LpsrVisitor::visitEnd (S_msrWedge& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrWedge" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrNote& elt)
{
  if (fMsrOptions->fDebug) {
    fOstream << idtr <<
      "--> Start visiting ";
    switch (elt->getNoteKind ()) {
      case msrNote::kStandaloneNote:
        fOstream << "standalone";
        break;
      case msrNote::kGraceNote:
        fOstream << "grace";
        break;
      case msrNote::kRestNote:
        fOstream << "rest";
        break;
      case msrNote::kChordMemberNote:
        fOstream << "chord member";
        break;
      case msrNote::kTupletMemberNote:
        fOstream << "tuplet member";
        break;
    } // switch
    fOstream << " msrNote" << endl;
  }

  switch (elt->getNoteKind ()) {
    
    case msrNote::kStandaloneNote:
      if (fMsrOptions->fDebug)
        cerr << idtr <<
          "--> appending " <<
          elt->noteAsString () << " to voice " <<
          fCurrentVoiceClone->getVoiceName () << endl;
          
      fCurrentVoiceClone->
        appendNoteToVoice (elt);
      break;
      
    case msrNote::kGraceNote:
      if (fMsrOptions->fDebug)
        cerr << idtr <<
          "--> appending " <<
          elt->noteAsString () << " to voice " <<
          fCurrentVoiceClone->getVoiceName () << endl;
          
      fCurrentVoiceClone->
        appendNoteToVoice (elt);
      break;
      
    case msrNote::kRestNote:
      fCurrentVoiceClone->
        appendNoteToVoice (elt);
      break;
      
    case msrNote::kChordMemberNote:
      fCurrentChordClone->
        addNoteToChord (elt);
      break;
      
    case msrNote::kTupletMemberNote:
      fTupletClonesStack.top ()->
        addElementToTuplet (elt);
      break;
  } // switch
}

void msr2LpsrVisitor::visitEnd (S_msrNote& elt)
{
  if (fMsrOptions->fDebug) {
    fOstream << idtr <<
      "--> Start visiting ";
    switch (elt->getNoteKind ()) {
      case msrNote::kStandaloneNote:
        fOstream << "standalone";
        break;
      case msrNote::kGraceNote:
        fOstream << "grace";
        break;
      case msrNote::kRestNote:
        fOstream << "rest";
        break;
      case msrNote::kChordMemberNote:
        fOstream << "chord member";
        break;
      case msrNote::kTupletMemberNote:
        fOstream << "tuplet member";
        break;
    } // switch
    fOstream << " msrNote" << endl;
  }

  switch (elt->getNoteKind ()) {
    case msrNote::kStandaloneNote:
      break;
    case msrNote::kGraceNote:
      break;
    case msrNote::kRestNote:
      break;
    case msrNote::kChordMemberNote:
      break;
    case msrNote::kTupletMemberNote:
      break;
  } // switch
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrBeam& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrBeam" << endl;
}

void msr2LpsrVisitor::visitEnd (S_msrBeam& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrBeam" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrChord& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrChord" << endl;

  fCurrentChordClone =
    elt->createEmptyClone ();

  // appending the chord to the voice at once
  fCurrentVoiceClone->
    appendChordToVoice (fCurrentChordClone);
}

void msr2LpsrVisitor::visitEnd (S_msrChord& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrChord" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrTuplet& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrTuplet" << endl;

  // create the tuplet clone
  S_msrTuplet
    tupletClone =
      elt->createEmptyClone ();

  // register it in this visitor
//  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "++> pushing tuplet " <<
      tupletClone->getActualNotes () <<
      "/" <<
      tupletClone->getNormalNotes () <<
      " to tuplets stack" << endl;
      
  fTupletClonesStack.push (tupletClone);
}

void msr2LpsrVisitor::visitEnd (S_msrTuplet& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrTuplet" << endl;

//  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> popping tuplet " <<
      elt->getActualNotes () <<
       "/" <<
      elt->getNormalNotes () <<
      " from tuplets stack" << endl;
      
    fTupletClonesStack.pop ();

  if (fTupletClonesStack.size ()) {
    // tuplet is an embedded tuplet
//    if (fMsrOptions->fDebug)
      cerr << idtr <<
        "=== adding embedded tuplet " <<
      elt->getActualNotes () <<
       "/" <<
      elt->getNormalNotes () <<
        " to " <<
      fTupletClonesStack.top ()->getActualNotes () <<
       "/" <<
      fTupletClonesStack.top ()->getNormalNotes () <<
      " current stack top tuplet" << endl;
    
    fTupletClonesStack.top ()->
      addElementToTuplet (elt);
  }
  else {
    // tup is a top level tuplet
//    if (fMsrOptions->fDebug)
      cerr << idtr <<
        "=== adding top level tuplet " <<
      elt->getActualNotes () <<
       "/" <<
      elt->getNormalNotes () <<
      " to voice" <<
      fCurrentVoiceClone->getVoiceName () <<
      endl;
      
    fCurrentVoiceClone->
      appendTupletToVoice (elt);
  }  
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrSlur& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrSlur" << endl;
}

void msr2LpsrVisitor::visitEnd (S_msrSlur& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrSlur" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrBarline& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrBarline" << endl;
  
  switch (elt->getBarlineCategory ()) {
    
    case msrBarline::kStandaloneBar:
      fCurrentVoiceClone->
        appendBarlineToVoice (elt);
      break;
      
    case msrBarline::kRepeatStart:
      {
 //     if (fMsrOptions->fDebug)
        cerr << idtr <<
          "--> handling kRepeatStart in voice " <<
          fCurrentVoiceClone->getVoiceName () << endl;

      // get the current voice chunk
      S_msrVoicechunk
        currentVoicechunk =
          fCurrentVoiceClone->
            getVoicechunk ();


      // create an LPSR repeat if not yet done
      /* JMI
      if (! fCurrentRepeatClone)
        fCurrentRepeatClone =
          elt->createEmptyClone (
            fCurrentVoiceClone);
*/

      // set the current voice chunk as the repeat's common part
      fCurrentRepeatClone->
        setRepeatCommonPart (currentVoicechunk);

      // create a new voice chunk for the voice
      if (fMsrOptions->fDebug)
        cerr << idtr <<
          "--> setting new voice chunk for voice " <<
          fCurrentVoiceClone->getVoiceName () << endl;
          
      fCurrentVoiceClone->
        setNewVoicechunkForVoice (
          elt->getInputLineNumber ());

      fCurrentVoiceClone->
        appendBarlineToVoice (elt);
      }
      break;

    case msrBarline::kRepeatEnd:
      {
 //     if (fMsrOptions->fDebug)
        cerr << idtr <<
          "--> handling kRepeatEnd in voice " <<
          fCurrentVoiceClone->getVoiceName () << endl;

      fCurrentVoiceClone->
        appendBarlineToVoice (elt);

      // get the current voice chunk
      S_msrVoicechunk
        currentVoicechunk =
          fCurrentVoiceClone->
            getVoicechunk ();

      // set the current voice chunk as the repeat's common part
      fCurrentRepeatClone->
        setRepeatCommonPart (currentVoicechunk);
      
      // create a new voice chunk for the voice
      if (fMsrOptions->fDebug)
        cerr << idtr <<
          "--> setting new voice chunk for voice " <<
          fCurrentVoiceClone->getVoiceName () << endl;
          
      fCurrentVoiceClone->
        setNewVoicechunkForVoice (
          elt->getInputLineNumber ());

      if (fOnGoingRepeat) {
        // add the repeat to the new voice chunk
   //     if (fMsrOptions->fDebug)
          cerr << idtr <<
            "--> appending the repeat to voice " <<
            fCurrentVoiceClone->getVoiceName () << endl;
  
        fCurrentVoiceClone->
          appendRepeatToVoice (fCurrentRepeatClone);
      }
      }
      break;
            
    case msrBarline::kEndingStart:
      {
 //     if (fMsrOptions->fDebug)
        cerr << idtr <<
          "--> handling kEndingStart in voice " <<
          fCurrentVoiceClone->getVoiceName () << endl;

      // get the current voice chunk
      S_msrVoicechunk
        currentVoicechunk =
          fCurrentVoiceClone->
            getVoicechunk ();

/*
      // set the current voice chunk as the repeat's common part
      fCurrentRepeatClone->
        setRepeatCommonPart (currentVoicechunk);
  */    
      // create a new voice chunk for the voice
      if (fMsrOptions->fDebug)
        cerr << idtr <<
          "--> setting new voice chunk for voice " <<
          fCurrentVoiceClone->getVoiceName () << endl;
          
      fCurrentVoiceClone->
        setNewVoicechunkForVoice (
          elt->getInputLineNumber ());

      fCurrentVoiceClone->
        appendBarlineToVoice (elt);

      if (fOnGoingRepeat) {
      }
      }
      break;
      
    case msrBarline::kHookedEndingEnd:
      {
 //     if (fMsrOptions->fDebug)
        cerr << idtr <<
          "--> handling kHookedEndingEnd in voice " <<
          fCurrentVoiceClone->getVoiceName () << endl;

      fCurrentVoiceClone->
        appendBarlineToVoice (elt);

      // get the current voice chunk
      S_msrVoicechunk
        currentVoicechunk =
          fCurrentVoiceClone->
            getVoicechunk ();

      // create a new voice chunk for the voice
//      if (fMsrOptions->fDebug)
        cerr << idtr <<
          "--> setting new voice chunk for voice " <<
          fCurrentVoiceClone->getVoiceName () << endl;
          
      fCurrentVoiceClone->
        setNewVoicechunkForVoice (
          elt->getInputLineNumber ());

      // create a repeat ending from the current voice chunk
//      if (fMsrOptions->fDebug)
        cerr << idtr <<
          "--> creating a new hooked ending for voice " <<
          fCurrentVoiceClone->getVoiceName () << endl;
          
      S_msrRepeatending
        repeatEnding =
          msrRepeatending::create (
            fMsrOptions, elt->getInputLineNumber (),
            elt->getEndingNumber (),
            msrRepeatending::kHookedEnding,
            currentVoicechunk,
            fCurrentRepeatClone);

      // add the repeat ending to the current repeat
//      if (fMsrOptions->fDebug)
        cerr << idtr <<
          "--> adding hooked ending to current repeat in voice " <<
          fCurrentVoiceClone->getVoiceName () << endl;
          
      fCurrentRepeatClone->
        addRepeatending (repeatEnding);

/*
      if (fOnGoingRepeat) {
        // add the repeat to the new voice chunk
   //     if (fMsrOptions->fDebug)
          cerr << idtr <<
            "--> appending the repeat to voice " <<
            fCurrentVoiceClone->getVoiceName () << endl;
  
        fCurrentVoiceClone->
          appendRepeatToVoice (fCurrentRepeatClone);
      }
      */
      }
      break;
      
    case msrBarline::kHooklessEndingEnd:
      {
 //     if (fMsrOptions->fDebug)
        cerr << idtr <<
          "--> handling kEndOfAHooklessEnding in voice " <<
          fCurrentVoiceClone->getVoiceName () << endl;

      // no need to keep that barline in the MSR,
      // LilyPond will take care of the repeat display
      fCurrentVoiceClone->
        appendBarlineToVoice (elt);

      // get the current voice chunk
      S_msrVoicechunk
        currentVoicechunk =
          fCurrentVoiceClone->
            getVoicechunk ();

      // create a new voice chunk for the voice
//      if (fMsrOptions->fDebug)
        cerr << idtr <<
          "--> setting new voice chunk for voice " <<
          fCurrentVoiceClone->getVoiceName () << endl;
          
      fCurrentVoiceClone->
        setNewVoicechunkForVoice (
          elt->getInputLineNumber ());

      // create a repeat ending from the current voice chunk
//      if (fMsrOptions->fDebug)
        cerr << idtr <<
          "--> creating a new hookless ending for voice " <<
          fCurrentVoiceClone->getVoiceName () << endl;
          
      S_msrRepeatending
        repeatEnding =
          msrRepeatending::create (
            fMsrOptions, elt->getInputLineNumber (),
            elt->getEndingNumber (),
            msrRepeatending::kHooklessEnding,
            currentVoicechunk,
            fCurrentRepeatClone);

      // add the repeat ending to the current repeat
//      if (fMsrOptions->fDebug)
        cerr << idtr <<
          "--> adding hookless ending to current repeat in voice " <<
          fCurrentVoiceClone->getVoiceName () << endl;
          
      fCurrentRepeatClone->
        addRepeatending (repeatEnding);

      if (fOnGoingRepeat) {
        // add the repeat to the new voice chunk
   //     if (fMsrOptions->fDebug)
          cerr << idtr <<
            "--> appending the repeat to voice " <<
            fCurrentVoiceClone->getVoiceName () << endl;
  
        fCurrentVoiceClone->
          appendRepeatToVoice (fCurrentRepeatClone);
      }
      }
      break;
  } // switch
}

void msr2LpsrVisitor::visitEnd (S_msrBarline& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrBarline" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrBarCheck& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrBarCheck" << endl;

  fCurrentVoiceClone->
    appendBarCheckToVoice (elt);
}

void msr2LpsrVisitor::visitEnd (S_msrBarCheck& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrBarCheck" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrBarnumberCheck& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrBarnumberCheck" << endl;

  fCurrentVoiceClone->
    appendBarnumberCheckToVoice (elt);
}

void msr2LpsrVisitor::visitEnd (S_msrBarnumberCheck& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrBarnumberCheck" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrBreak& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrBreak" << endl;

  fCurrentVoiceClone->
    appendBreakToVoice (elt);
}

void msr2LpsrVisitor::visitEnd (S_msrBreak& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrBreak" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrRepeat& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrRepeat" << endl;

  // create an LPSR repeat
  fCurrentRepeatClone =
    elt->createEmptyClone (
      fCurrentVoiceClone);

/* JMI
  fCurrentVoiceClone->
    appendRepeatToVoice (fCurrentRepeatClone);
 // JMI ???   appendElementToVoice (fCurrentRepeatClone);
*/

  fOnGoingRepeat = true;
}

void msr2LpsrVisitor::visitEnd (S_msrRepeat& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrRepeat" << endl;
      
  fCurrentRepeatClone = 0;
  fOnGoingRepeat = false;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrRepeatending& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrRepeatending" << endl;
/* JMI
  S_msrRepeatending
    repeatending =
      elt->createEmptyClone (fCurrentRepeatClone);

  fCurrentRepeatClone->
    addRepeatending (repeatending);
    */
}

void msr2LpsrVisitor::visitEnd (S_msrRepeatending& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrRepeatending" << endl;
}

//________________________________________________________________________
/*void msr2LpsrVisitor::visitStart (S_msrComment& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrComment" << endl;
}

void msr2LpsrVisitor::visitEnd (S_msrComment& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrComment" << endl;
}
*/
//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrVarValAssoc& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrVarValAssoc" << endl;

  string variableName     = elt->getVariableName ();
  string variableValueAux = elt->getVariableValue ();
  int    inputLineNumber  = elt->getInputLineNumber ();
  string variableValue;

  // escape quotes if any
  for_each (
    variableValueAux.begin(),
    variableValueAux.end(),
    stringQuoteEscaper (variableValue));

  if (variableName == "work-number")
    fLpsrScore->
      getHeader()->setWorkNumber (
        inputLineNumber, variableValue);
  else
  if (variableName == "work-title")
    fLpsrScore->
      getHeader()->setWorkTitle (
        inputLineNumber, variableValue);
  else
  if (variableName == "movement-number")
    fLpsrScore->
      getHeader()->setMovementNumber (
        inputLineNumber, variableValue);
  else
  if (variableName == "movement-title")
    fLpsrScore->
      getHeader()->setMovementTitle (
        inputLineNumber, variableValue);
  else
  if (variableName == "creator")
    fLpsrScore->
      getHeader()->addCreator (
        inputLineNumber, variableName, variableValue);
  if (variableName == "rights")
    fLpsrScore->
      getHeader()->setRights (
        inputLineNumber, variableValue);
  else
  if (variableName == "software")
    fLpsrScore->
      getHeader()->addSoftware (
        inputLineNumber, variableValue);
  else
  if (variableName == "encoding-date")
    fLpsrScore->
      getHeader()->setEncodingDate (
        inputLineNumber, variableValue);
}

void msr2LpsrVisitor::visitEnd (S_msrVarValAssoc& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrVarValAssoc" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrLayout& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrLayout" << endl;

  idtr++;
}

void msr2LpsrVisitor::visitEnd (S_msrLayout& elt)
{
  idtr--;

  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrLayout" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrMidi& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrMidi" << endl;
}

void msr2LpsrVisitor::visitEnd (S_msrMidi& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrMidi" << endl;
}


} // namespace
