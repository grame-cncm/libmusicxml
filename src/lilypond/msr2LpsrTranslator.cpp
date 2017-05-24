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

#include "msr2LpsrTranslator.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
msr2LpsrTranslator::msr2LpsrTranslator (
  ostream&       os,
  S_msrScore     mScore)
    : fOstream (os)
{
  // the MSR score we're visiting
  fVisitedMsrScore = mScore;

  // identification
  fOnGoingIdentification = false;

  // header
  fWorkNumberKnown       = false;
  fWorkTitleKnown        = false;
  fMovementNumberKnown   = false;
  fMovementTitleKnown    = false;
   
  // staves
  fOnGoingStaff          = false;

  // voices
  fOnGoingHarmonyVoice   = false;

  // repeats
  fRepeatHasBeenCreatedForCurrentPartClone = false;
  fOnGoingRepeat         = false;

  // notes
  fOnGoingNote           = false;

  // double tremolos
  fOnGoingDoubleTremolo  = false;

  // chords
  fOnGoingChord          = false;
  
  // stanzas
  fOnGoingStanza         = false;

  // syllables
  fOnGoingSyllableExtend = false;
};
  
msr2LpsrTranslator::~msr2LpsrTranslator ()
{}

//________________________________________________________________________
void msr2LpsrTranslator::buildLpsrScoreFromMsrScore ()
{
  if (fVisitedMsrScore) {    
    // create a msrScore browser
    msrBrowser<msrScore> browser (this);

    // browse the score with the browser
    browser.browse (*fVisitedMsrScore);
  }
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrScore& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrScore" <<
      endl;

  // create an empty clone of fVisitedMsrScore for use by the LPSR score
  // not sharing the visitiged MSR score allows cleaner data handling
  // and optimisations of the LPSR data
  fCurrentMsrScoreClone =
    fVisitedMsrScore->
      createScoreBareClone ();

  // create the LPSR score
  fLpsrScore =
    lpsrScore::create (
      0, // input line number
      fCurrentMsrScoreClone);
      
  // fetch score header
  fLpsrScoreHeader =
    fLpsrScore-> getHeader();

/* JMI
  // push it onto this visitors's stack,
  // making it the current partgroup block
  fPartgroupBlocksStack.push (
    partgroupBlock);
    */
}

void msr2LpsrTranslator::visitEnd (S_msrScore& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrScore" <<
      endl;

  // try to set header title and subtitle
  if (fWorkTitleKnown && fMovementTitleKnown) {
    // we have both the work and movement titles available
    fLpsrScoreHeader->
      changeWorkTitleVariableName ("title");
    fLpsrScoreHeader->
      changeMovementTitleVariableName ("subtitle");
  }
  else if (fWorkTitleKnown) {
    // we only have the work title available
    fLpsrScoreHeader->
      changeWorkTitleVariableName ("title");
  }
  else if (fMovementTitleKnown) {
    // we only have the movement title available
    fLpsrScoreHeader->
      changeMovementTitleVariableName ("title");
  }

  // try to set header subsubtitle
  if (fMovementNumberKnown) {
    fLpsrScoreHeader->
      changeMovementNumberVariableName ("subsubtitle");
  }
  
  // try to set header opus
  if (fWorkNumberKnown) {
    fLpsrScoreHeader->
      changeWorkNumberVariableName ("opus");
  }
  
/* JMI
  // get top level pargroup block from the stack
  S_lpsrPartgroupBlock
    partgroupBlock =
      fPartgroupBlocksStack.top ();

  // pop it from the stack
  fPartgroupBlocksStack.top ();

  // the stack should now be empty
  if (fPartgroupBlocksStack.size())
    msrInternalError (
      1,
      "the partgroup block stack is not exmpty at the end of the visit");
   */ 
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrIdentification& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrIdentification" <<
      endl;

  idtr++;

  fOnGoingIdentification = true;
}

void msr2LpsrTranslator::visitEnd (S_msrIdentification& elt)
{
  fOnGoingIdentification = false;
  
  idtr--;

  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrIdentification" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrPageGeometry& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrPageGeometry" <<
      endl;

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

  // get LPSR score block layout
  S_lpsrLayout
    scoreBlockLayout =
      fLpsrScore->getScoreBlock ()->getScoreBlockLayout ();

  // create the score block layout staff-size Scheme assoc
  stringstream s;
  s << globalStaffSize;
  S_lpsrSchemeVarValAssoc
    assoc =
      lpsrSchemeVarValAssoc::create (
        0, // JMI
        lpsrSchemeVarValAssoc::kCommented,
        "layout-set-staff-size",
        s.str(),
        "Uncomment and adapt next line as needed (default is 20)",
        lpsrSchemeVarValAssoc::kWithEndl);

  // populate score block layout
  scoreBlockLayout->
    addSchemeVarValAssoc (assoc);

/* JMI
    void    setBetweenSystemSpace (float val) { fBetweenSystemSpace = val; }
    float   getBetweenSystemSpace () const    { return fBetweenSystemSpace; }

    void    setPageTopSpace       (float val) { fPageTopSpace = val; }
   */
}

void msr2LpsrTranslator::visitEnd (S_msrPageGeometry& elt)
{  
  idtr--;

  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrPageGeometry" <<
      endl;
}


//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrCredit& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrCredit" <<
      endl;

  fCurrentCredit = elt;

  // set elt as credit of the MSR score part of the LPSR score
  fLpsrScore->
    getMsrScore ()->
      setCredit (fCurrentCredit);
}

void msr2LpsrTranslator::visitEnd (S_msrCredit& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrCredit" <<
      endl;

  fCurrentCredit = 0;
}

void msr2LpsrTranslator::visitStart (S_msrCreditWords& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrCreditWords" <<
      endl;

  // don't append it to the current credit, since the latter is no clone
  /* JMI
  fCurrentCredit->
    appendCreditWordsToCredit (
      elt);
      */
}

void msr2LpsrTranslator::visitEnd (S_msrCreditWords& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrCreditWords" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrPartgroup& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrPartgroup" <<
      endl;

  // create a partgroup clone
  // current partgroup clone, i.e. the top of the stack,
  // is the uplink of the new one if it exists

  S_msrPartgroup
    partgroupClone =
      elt->createPartgroupBareClone (
        fPartgroupsStack.size ()
          ? fPartgroupsStack.top ()
          : 0,
        fLpsrScore->getMsrScore ());

  // push it onto this visitors's stack,
  // making it the current partgroup block
  if (gGeneralOptions->fTracePartgroups)
    cerr << idtr <<
      "--> pushing part group clone " <<
      partgroupClone->getPartgroupCombinedName () <<
      " onto stack" <<
      endl;

  fPartgroupsStack.push (
    partgroupClone);
  
/*
  // add it to the MSR score clone
  fCurrentMsrScoreClone->
    addPartgroupToScore (fCurrentPartgroupClone);
*/

  // create a partgroup block refering to the part group clone
  S_lpsrPartgroupBlock
    partgroupBlock =
      lpsrPartgroupBlock::create (
        partgroupClone);

  // push it onto this visitors's stack,
  // making it the current partgroup block
  fPartgroupBlocksStack.push (
    partgroupBlock);
  
  // get the LPSR store block
  S_lpsrScoreBlock
    scoreBlock =
      fLpsrScore->getScoreBlock ();

  // don't append the pargroup block to the score block now:
  // this will be done when it gets popped from the stack
}

void msr2LpsrTranslator::visitEnd (S_msrPartgroup& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrPartgroup" <<
      endl;

  S_msrPartgroup
    currentPartgroup =
      fPartgroupsStack.top ();
      
  if (fPartgroupsStack.size () == 1) {
    // add the current pargroup clone to the MSR score clone
    // if it is the top-level one, i.e it's alone in the stack
    
    if (gGeneralOptions->fTracePartgroups)
      cerr << idtr <<
        "--> adding part group clone " <<
        currentPartgroup->getPartgroupCombinedName () <<
        " to MSR score" <<
        endl;

    fCurrentMsrScoreClone->
      addPartgroupToScore (currentPartgroup);

    fPartgroupsStack.pop ();
  }

  else {

    // pop current partgroup from this visitors's stack
    if (gGeneralOptions->fTracePartgroups)
      fOstream << idtr <<
        "--> popping part group clone " <<
        fPartgroupsStack.top ()->getPartgroupCombinedName () <<
        " from stack" <<
        endl;

    fPartgroupsStack.pop ();

    // append the current part group to the one one level higher,
    // i.e. the new current part group
    fPartgroupsStack.top ()->
      appendSubPartgroupToPartgroup (
        currentPartgroup);
  }

  // get the LPSR store block
  S_lpsrScoreBlock
    scoreBlock =
      fLpsrScore->getScoreBlock ();
      
  S_lpsrPartgroupBlock
    currentPartgroupBlock =
      fPartgroupBlocksStack.top ();
      
  if (fPartgroupBlocksStack.size () == 1) {
    // add the current pargroup clone to the LPSR score's parallel music
    // if it is the top-level one, i.e it's alone in the stack
    
    if (gGeneralOptions->fTracePartgroups)
      cerr << idtr <<
        "--> adding part group block clone for part group " <<
        currentPartgroupBlock->
          getPartgroup ()->
            getPartgroupCombinedName () <<
        " to LPSR score" <<
        endl;

    // append the current pargroup block to the score block
    // if it is the top-level one, i.e it's alone in the stack
   // JMI BOF if (fPartgroupBlocksStack.size () == 1)
      scoreBlock->
        appendPartgroupBlockToParallelMusic (
          fPartgroupBlocksStack.top ());
          
    // pop current partgroup block from this visitors's stack,
    // only now to restore the appearence order
    fPartgroupBlocksStack.pop ();
  }

  else {
    // pop current partgroup block from this visitors's stack
    if (gGeneralOptions->fTracePartgroups)
      fOstream << idtr <<
        "--> popping part group block clone for part group " <<
        currentPartgroupBlock->
          getPartgroup ()->
            getPartgroupCombinedName () <<
        " from stack" <<
        endl;

    fPartgroupBlocksStack.pop ();

    // append the current part group block to the one one level higher,
    // i.e. the new current part group block
    fPartgroupBlocksStack.top ()->
      appendElementToPartgroupBlock (
        currentPartgroupBlock);
  }

}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrPart& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrPart" <<
      endl;

  idtr++;

  // create a part clone
  fCurrentPartClone =
    elt->createPartBareClone (
      fPartgroupsStack.top ());

  // setup it durations to divisions data
  fCurrentPartClone->
    setupDurationsDivisions (
      elt->getPartDivisionsPerQuarterNote ());
    
  // add it to the partgroup clone
  if (gGeneralOptions->fTraceParts)
    cerr << idtr <<
      "Adding part clone " <<
      fCurrentPartClone->getPartCombinedName () <<
      " to part group clone \"" <<
      fPartgroupsStack.top ()->getPartgroupCombinedName () <<
      "\"" <<
      endl;

  fPartgroupsStack.top ()->
    addPartToPartgroup (fCurrentPartClone);

  // create a part block
  fCurrentPartBlock =
    lpsrPartBlock::create (
      fCurrentPartClone);

  // append it to the current partgroup block
  if (gGeneralOptions->fTraceParts)
    cerr << idtr <<
      "Appending part block " <<
      fPartgroupsStack.top ()->getPartgroupCombinedName () <<
      " to stack" <<
      endl;

  fPartgroupBlocksStack.top ()->
    appendElementToPartgroupBlock (fCurrentPartBlock);
}

void msr2LpsrTranslator::visitEnd (S_msrPart& elt)
{
  idtr--;

  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrPart" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrStaff& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrStaff" <<
      endl;

  idtr++;

  switch (elt->getStaffKind ()) {
    case msrStaff::kRegularStaff:
      {
        // create a staff clone
        fCurrentStaffClone =
          elt->createStaffBareClone (
            fCurrentPartClone);
          
        // add it to the part clone
        fCurrentPartClone->
          addStaffToPartCloneByItsNumber (
            fCurrentStaffClone);
      
        // create a staff block
        fCurrentStaffBlock =
          lpsrStaffBlock::create (
            fCurrentStaffClone);
      
        string
          partName =
            fCurrentPartClone->getPartName (),
          partAbbreviation =
            fCurrentPartClone->getPartAbbreviation ();
      
        string staffBlockInstrumentName;
        string staffBlockShortInstrumentName;
      
        // don't set instrument name nor short instrument name // JMI
        // if the staff belongs to a piano part where they're already set
        if (! partName.size ())
          staffBlockInstrumentName = partName;
        if (! partAbbreviation.size ())
          staffBlockShortInstrumentName = partAbbreviation;
      
        if (staffBlockInstrumentName.size ())
          fCurrentStaffBlock->
            setStaffBlockInstrumentName (staffBlockInstrumentName);
            
        if (staffBlockShortInstrumentName.size ())
          fCurrentStaffBlock->
            setStaffBlockShortInstrumentName (staffBlockShortInstrumentName);
              
        // append the staff block to the current part block
        fCurrentPartBlock->
          appendElementToPartBlock (fCurrentStaffBlock);
      
        fOnGoingStaff = true;
      }
      break;
      
    case msrStaff::kTablatureStaff:
      // JMI
      {
        // create a staff clone
        fCurrentStaffClone =
          elt->createStaffBareClone (
            fCurrentPartClone);
          
        // add it to the part clone
        fCurrentPartClone->
          addStaffToPartCloneByItsNumber (
            fCurrentStaffClone);
      
        // create a staff block
        fCurrentStaffBlock =
          lpsrStaffBlock::create (
            fCurrentStaffClone);
      
        string
          partName =
            fCurrentPartClone->getPartName (),
          partAbbreviation =
            fCurrentPartClone->getPartAbbreviation ();
      
        string staffBlockInstrumentName;
        string staffBlockShortInstrumentName;
      
        // don't set instrument name nor short instrument name // JMI
        // if the staff belongs to a piano part where they're already set
        if (! partName.size ())
          staffBlockInstrumentName = partName;
        if (! partAbbreviation.size ())
          staffBlockShortInstrumentName = partAbbreviation;
      
        if (staffBlockInstrumentName.size ())
          fCurrentStaffBlock->
            setStaffBlockInstrumentName (staffBlockInstrumentName);
            
        if (staffBlockShortInstrumentName.size ())
          fCurrentStaffBlock->
            setStaffBlockShortInstrumentName (staffBlockShortInstrumentName);
              
        // append the staff block to the current part block
        fCurrentPartBlock->
          appendElementToPartBlock (fCurrentStaffBlock);
      
        fOnGoingStaff = true;
      }
      break;
      
    case msrStaff::kPercussionStaff:
      // JMI
      {
        // create a staff clone
        fCurrentStaffClone =
          elt->createStaffBareClone (
            fCurrentPartClone);
          
        // add it to the part clone
        fCurrentPartClone->
          addStaffToPartCloneByItsNumber (
            fCurrentStaffClone);
      
        // create a staff block
        fCurrentStaffBlock =
          lpsrStaffBlock::create (
            fCurrentStaffClone);
      
        string
          partName =
            fCurrentPartClone->getPartName (),
          partAbbreviation =
            fCurrentPartClone->getPartAbbreviation ();
      
        string staffBlockInstrumentName;
        string staffBlockShortInstrumentName;
      
        // don't set instrument name nor short instrument name // JMI
        // if the staff belongs to a piano part where they're already set
        if (! partName.size ())
          staffBlockInstrumentName = partName;
        if (! partAbbreviation.size ())
          staffBlockShortInstrumentName = partAbbreviation;
      
        if (staffBlockInstrumentName.size ())
          fCurrentStaffBlock->
            setStaffBlockInstrumentName (staffBlockInstrumentName);
            
        if (staffBlockShortInstrumentName.size ())
          fCurrentStaffBlock->
            setStaffBlockShortInstrumentName (staffBlockShortInstrumentName);
              
        // append the staff block to the current part block
        fCurrentPartBlock->
          appendElementToPartBlock (fCurrentStaffBlock);
      
        fOnGoingStaff = true;
      }
      break;
      
    case msrStaff::kHarmonyStaff:
      {
        // don't create a harmony staff clone here,
        // since it has been created already in the part clone
        fCurrentStaffClone =
          fCurrentPartClone->
            getPartHarmonyStaff ();
            
        /* JMI
        // create a staff clone
        fCurrentStaffClone =
          elt->createStaffBareClone (fCurrentPartClone);
          
        // add it to the part clone
        fCurrentPartClone->
          addStaffToPartClone (fCurrentStaffClone);
*/

      /* JMI
        // create a staff block
        fCurrentStaffBlock =
          lpsrStaffBlock::create (
            fCurrentStaffClone);
      
        string
          partName =
            fCurrentPartClone->getPartName (),
          partAbbreviation =
            fCurrentPartClone->getPartAbbreviation ();
      
        string staffBlockInstrumentName;
        string staffBlockShortInstrumentName;
      
        // don't set instrument name nor short instrument name
        // if the staff belongs to a piano part where they're already set
        if (! partName.size ())
          staffBlockInstrumentName = partName;
        if (! partAbbreviation.size ())
          staffBlockShortInstrumentName = partAbbreviation;
      
        if (staffBlockInstrumentName.size ())
          fCurrentStaffBlock->
            setStaffBlockInstrumentName (staffBlockInstrumentName);
            
        if (staffBlockShortInstrumentName.size ())
          fCurrentStaffBlock->
            setStaffBlockShortInstrumentName (staffBlockShortInstrumentName);
              
        // append the staff block to the current part block
        fCurrentPartBlock->
          appendElementToPartBlock (fCurrentStaffBlock);
      */
      
        fOnGoingStaff = true;
      }
      break;
  } // switch
}

void msr2LpsrTranslator::visitEnd (S_msrStaff& elt)
{
  idtr--;

  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting S_msrStaff" <<
      endl;

  switch (elt->getStaffKind ()) {
    case msrStaff::kRegularStaff:
      {
        fOnGoingStaff = false;
      }
      break;
      
    case msrStaff::kTablatureStaff:
      // JMI
      break;
      
    case msrStaff::kPercussionStaff:
      // JMI
      break;
      
    case msrStaff::kHarmonyStaff:
      // JMI
      break;
  } // switch
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrStafftuning& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrStafftuning" <<
      endl;
  
  // create a staff tuning clone
  fCurrentStafftuningClone =
    elt->
      createStafftuningBareClone ();

  // add it to the staff clone
  fCurrentStaffClone->
    addStafftuningToStaff (
      fCurrentStafftuningClone);

  // create a staff tuning block
  S_lpsrNewStafftuningBlock
    newStafftuningBlock =
      lpsrNewStafftuningBlock::create (
        fCurrentStafftuningClone->getInputLineNumber (),
        fCurrentStafftuningClone);

  // append it to the current staff block
  fCurrentStaffBlock->
    appendElementToStaffBlock (newStafftuningBlock);
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrVoice& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrVoice" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  idtr++;

  switch (elt->getVoiceKind ()) {
    
    case msrVoice::kRegularVoice:
      // create a voice clone
      fCurrentVoiceClone =
        elt->createVoiceBareClone (
          fCurrentStaffClone);
    
    /* JMI
      // re-number it's first measure to 1 if needed
      if (! fCurrentVoiceClone->getMeasureZeroHasBeenMetInVoice ())
        fCurrentVoiceClone->forceVoiceMeasureNumberTo (1);
      */
        
      // add it to the staff clone
      fCurrentStaffClone->
        registerVoiceInStaff (
          inputLineNumber, fCurrentVoiceClone);
    
      // append the voice clone to the LPSR score elements list
      fLpsrScore ->
        appendVoiceToScoreElements (fCurrentVoiceClone);
    
      // append a use of the voice to the current staff block
      fCurrentStaffBlock->
        appendVoiceUseToStaffBlock (fCurrentVoiceClone);
      break;
      
    case msrVoice::kHarmonyVoice:
      {
        // fetch harmony voice
        fCurrentVoiceClone =
          fCurrentPartClone->
            getPartHarmonyVoice ();

        // append the voice clone to the LPSR score elements list
        fLpsrScore ->
          appendVoiceToScoreElements (fCurrentVoiceClone);
    
        // create a ChordNames context command
        string voiceName =
          elt->getVoiceName ();

        string partCombinedName =
          elt->getVoiceDirectPartUplink ()->
            getPartCombinedName ();
                        
        if (gGeneralOptions->fTraceHarmonies)
          cerr << idtr <<
            "Creating a ChordNames context for \"" << voiceName <<
            "\" in part " << partCombinedName <<
            endl;

        S_lpsrContext
          chordNamesContext =
            lpsrContext::create (
              inputLineNumber,
              lpsrContext::kExistingContext,
              "ChordNames",
              voiceName);

        // append it to the current part block
        if (gGeneralOptions->fTraceHarmonies)
          cerr << idtr <<
            "Appending the ChordNames context for \"" << voiceName <<
            "\" in part " << partCombinedName <<
            endl;

        fCurrentPartBlock->
          appendElementToPartBlock (
            chordNamesContext);

        fOnGoingHarmonyVoice = true;
      }
      break;
      
    case msrVoice::kMasterVoice:
      // JMI
      break;
  } // switch

  // clear the voice notes map
  fVoiceNotesMap.clear ();

  fFirstNoteCloneInVoice = 0;
}

void msr2LpsrTranslator::visitEnd (S_msrVoice& elt)
{
  idtr--;

  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrVoice" <<
      endl;

  switch (elt->getVoiceKind ()) {
    case msrVoice::kRegularVoice:
      // JMI
      break;
      
    case msrVoice::kHarmonyVoice:
      fOnGoingHarmonyVoice = false;
      break;
      
    case msrVoice::kMasterVoice:
      // JMI
      break;
  } // switch
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrSegment& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrSegment" <<
      endl;

  // fetch the current segment clone
  fCurrentSegmentClone =
    fCurrentVoiceClone->
      getVoiceLastSegment ();

/* JMI
  // create a clone of the segment
  fCurrentSegmentClone =
    elt->createSegmentBareClone (
      fCurrentVoiceClone);

  // append it to the current voice
  fCurrentVoiceClone->
    appendSegmentToVoice (
      fCurrentSegmentClone);
      */
}

void msr2LpsrTranslator::visitEnd (S_msrSegment& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrSegment" <<
      endl;

  // forget current segment clone
  fCurrentSegmentClone = 0;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrHarmony& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrHarmony '" <<
      elt->harmonyAsString () <<
      "'" <<
      endl;

  if (fOnGoingNote) {
    // register the harmony in the current note clone
    fCurrentNoteClone->
      setNoteHarmony (elt);

  // don't append the harmony to the part harmony,
  // this will be done below
  }
  
  else if (fOnGoingChord) {
    // register the harmony in the current chord clone
    fCurrentChordClone->
      setChordHarmony (elt); // JMI
  }
  
  else if (fOnGoingHarmonyVoice) { // JMI
    // register the harmony in the part clone harmony
    fCurrentPartClone->
      appendHarmonyToPartClone (
        fCurrentVoiceClone,
        elt);
  }
}

void msr2LpsrTranslator::visitEnd (S_msrHarmony& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrHarmony '" <<
      elt->harmonyAsString () <<
      "'" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrMeasure& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrMeasure" <<
      endl;

/* JMI
  fCurrentPartClone->
    setPartMeasureNumber (
      elt->getInputLineNumber (),
      elt->getMeasureNumber ());
    */

  // measure 1 is created by default initially
  
  // create a clone of the measure
  fCurrentMeasureClone =
    elt->
      createMeasureBareClone (
        fCurrentSegmentClone);
      
  // append it to the current voice clone
  fCurrentSegmentClone->
    appendMeasureToSegment (
      fCurrentMeasureClone);
}

void msr2LpsrTranslator::finalizeMeasure ( // JMI
  int          inputLineNumber,
  S_msrMeasure originalMeasure,
  S_msrMeasure measureClone)
{
  // fetch the voice
  S_msrVoice
    voice =
      measureClone->getMeasureSegmentUplink ()->
        getSegmentVoiceUplink ();
    
  // fetch the part measure position high tide
  int partMeasurePositionHighTide =
    measureClone->getMeasureDirectPartUplink ()->
      getPartMeasurePositionHighTide ();

  int measureNumber =
    measureClone->getMeasureNumber ();
    
  int measurePosition =
    measureClone->getMeasurePosition ();

  int measureDivisionsPerFullMeasure =
    measureClone->getMeasureDivisionsPerFullMeasure ();
    
  if (gGeneralOptions->fTraceMeasures) {
    cerr <<
      idtr <<
        "Finalizing measure " << measureNumber <<
        " in voice \"" << voice->getVoiceName () <<
        "\", line " << inputLineNumber <<
        endl <<
      idtr <<
        "measurePosition = " << measurePosition <<
        endl <<
      idtr <<
        "partMeasurePositionHighTide = " <<
        partMeasurePositionHighTide <<
        endl;
  }

  msrMeasure::msrMeasureKind measureKind; // JMI
 // JMI     fMeasureKind = kFullMeasure; // may be changed afterwards
    
  // positions start at 1

  if (measurePosition == measureDivisionsPerFullMeasure + 1) { // JMI
    measureKind =
      msrMeasure::kFullMeasure;
  }
      
  else if (measurePosition <= measureDivisionsPerFullMeasure) { // JMI
    /*
    if (fSegmentMeasuresList.size () == 1) {
      // this is the first measure in the segment
      measureKind =
        msrMeasure::kIncompleteLeftMeasure;
    }
    
    else {
      // this is the last measure in the segment
      measureKind =
        msrMeasure::kIncompleteRightMeasure;
    }
    */
    measureKind =
      msrMeasure::kIncompleteLeftMeasure; // JMI
  }

  else if (measurePosition > measureDivisionsPerFullMeasure + 1) {
    measureKind =
      msrMeasure::kOverfullMeasure;
  }

  if (false) { // JMI ??? measureKind != originalMeasure->getMeasureKind ()) {
    stringstream s;

    s <<
      "line " << inputLineNumber << ":" <<
      endl <<
      "clone measure:" <<
      endl <<
      measureClone <<
      endl <<
      "differs from original measure:" <<
      originalMeasure;

    msrInternalError (
      inputLineNumber,
      s.str());
  }
}

void msr2LpsrTranslator::visitEnd (S_msrMeasure& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrMeasure" <<
      endl;

  finalizeMeasure (
    elt->getInputLineNumber (),
    elt, // original measure
    fCurrentMeasureClone);

  bool doCreateABarCheck = false;
  
  switch (elt->getMeasureKind ()) {
    
    case msrMeasure::kFullMeasure:
      // is the measure full? (positions start at 1)
      if (
        elt->getMeasureLength ()
          >=
        elt->getMeasureDivisionsPerFullMeasure ()) {
        doCreateABarCheck = true;
      }
      break;
      
    case msrMeasure::kIncompleteLeftMeasure:
      doCreateABarCheck = true;
      break;
      
    case msrMeasure::kIncompleteRightMeasure:
      break;
      
    case msrMeasure::kOverfullMeasure:
      // JMI
      break;
      
    case msrMeasure::kEmptyMeasure:
      // JMI
      break;
  } // switch

  if (doCreateABarCheck) {
    // create a bar check
    S_msrBarCheck
      barCheck =
        msrBarCheck::create (
          elt->getInputLineNumber (),
          elt->getMeasureNumber () + 1);
              
    // append it to the current voice clone
    fCurrentVoiceClone->
      appendBarCheckToVoice (barCheck);
  }
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrStanza& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream <<
      endl << endl <<
      idtr <<
        "--> Start visiting msrStanza" <<
      endl;

  idtr++;

//  if (elt->getStanzaTextPresent ()) { // JMI
    fCurrentStanzaClone =
      elt->createStanzaBareClone (
        fCurrentVoiceClone);
    
    // append the stanza clone to the LPSR score elements list
    fLpsrScore ->
      appendStanzaToScoreElements (fCurrentStanzaClone);
  
    // append a use of the stanza to the current staff block
    fCurrentStaffBlock ->
      appendLyricsUseToStaffBlock (fCurrentStanzaClone);
//  }
//  else
  //  fCurrentStanzaClone = 0; // JMI

  fOnGoingStanza = true;
}

void msr2LpsrTranslator::visitEnd (S_msrStanza& elt)
{
  idtr--;
  
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrStanza" <<
      endl << endl <<
      endl;

  fOnGoingStanza = true;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrSyllable& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrSyllable" <<
      endl;

  // create the syllable clone
  fCurrentSyllableClone =
    elt->createSyllableBareClone (
      fCurrentPartClone);

  // add it to the current stanza clone or current note clone

  if (fOnGoingStanza) // fCurrentStanzaClone JM
    // visiting a syllable as a stanza member
    fCurrentStanzaClone->
      appendSyllableToStanza (fCurrentSyllableClone);
  
  if (fOnGoingNote) { // JMI
    // visiting a syllable as attached to a note
    fCurrentNoteClone->
      appendSyllableToNote (fCurrentSyllableClone);
  }

  // get elt's note uplink
  S_msrNote
    eltNoteUplink =
      fVoiceNotesMap [elt->getSyllableNoteUplink ()];
    
  if (eltNoteUplink) {
    // set syllable clone's note uplink to the clone of elt's note uplink
    if (gGeneralOptions->fTraceLyrics)
      fOstream <<
        idtr <<
        "--> setting syllable note uplink " <<
        fCurrentSyllableClone->syllableAsString () <<
        " to " << eltNoteUplink->noteAsShortString () <<
        endl;

    fCurrentSyllableClone->
      setSyllableNoteUplink (
        eltNoteUplink);
  }
    
  // a syllable ends the sysllable extend range if any
  if (fOnGoingSyllableExtend) {
    // create melisma end command
    S_lpsrMelismaCommand
      melismaCommand =
        lpsrMelismaCommand::create (
          elt->getInputLineNumber (),
          lpsrMelismaCommand::kMelismaEnd);

    // append it to current voice clone
    fCurrentVoiceClone->
      appendOtherElementToVoice (melismaCommand);

    fOnGoingSyllableExtend = false;
  }
}

void msr2LpsrTranslator::visitEnd (S_msrSyllable& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrSyllable" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrClef& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrClef" <<
      endl;

  fCurrentVoiceClone->
    appendClefToVoice (elt);
}

void msr2LpsrTranslator::visitEnd (S_msrClef& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrClef" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrKey& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrKey" <<
      endl;

  fCurrentVoiceClone->
    appendKeyToVoice (elt);
}

void msr2LpsrTranslator::visitEnd (S_msrKey& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrKey" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrTime& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrTime" <<
      endl;

  fCurrentVoiceClone->
    appendTimeToVoice (elt);
}

void msr2LpsrTranslator::visitEnd (S_msrTime& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrTime" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrTempo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrTempo" <<
      endl;

  fCurrentVoiceClone->
    appendTempoToVoice (elt);
}

void msr2LpsrTranslator::visitEnd (S_msrTempo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrTempo" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrArticulation& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrArticulation" <<
      endl;

  if (fOnGoingNote) {
    // don't add articulations to chord member notes
    if (fCurrentNoteClone->getNoteKind () != msrNote::kChordMemberNote)
      fCurrentNoteClone->
        addArticulationToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      addArticulationToChord (elt);
  }
}

void msr2LpsrTranslator::visitEnd (S_msrArticulation& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrArticulation" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrTechnical& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrTechnical" <<
      endl;

  if (fOnGoingNote) {
    // don't add technicals to chord member notes
    if (fCurrentNoteClone->getNoteKind () != msrNote::kChordMemberNote)
      fCurrentNoteClone->
        addTechnicalToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      addTechnicalToChord (elt);
  }

  // doest the score need the 'tongue' function?
  switch (elt->getTechnicalKind ()) {
    case msrTechnical::kArrow:
      break;
    case msrTechnical::kDoubleTongue:
      fLpsrScore->
        setTongueSchemeFunctionNeeded ();
      break;
    case msrTechnical::kDownBow:
      break;
    case msrTechnical::kFingernails:
      break;
    case msrTechnical::kHarmonic:
      break;
    case msrTechnical::kHeel:
      break;
    case msrTechnical::kHole:
      break;
    case msrTechnical::kOpenString:
      break;
    case msrTechnical::kSnapPizzicato:
      break;
    case msrTechnical::kStopped:
      break;
    case msrTechnical::kTap:
      break;
    case msrTechnical::kThumbPosition:
      break;
    case msrTechnical::kToe:
      break;
    case msrTechnical::kTripleTongue:
      fLpsrScore->
        setTongueSchemeFunctionNeeded ();
      break;
    case msrTechnical::kUpBow:
      break;
  } // switch
}

void msr2LpsrTranslator::visitEnd (S_msrTechnical& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrTechnical" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrTechnicalWithInteger& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrTechnicalWithInteger" <<
      endl;

  if (fOnGoingNote) {
    // don't add technicalWithIntegers to chord member notes
    if (fCurrentNoteClone->getNoteKind () != msrNote::kChordMemberNote)
      fCurrentNoteClone->
        addTechnicalWithIntegerToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      addTechnicalWithIntegerToChord (elt);
  }
}

void msr2LpsrTranslator::visitEnd (S_msrTechnicalWithInteger& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrTechnicalWithInteger" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrTechnicalWithString& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrTechnicalWithString" <<
      endl;

  if (fOnGoingNote) {
    // don't add technicalWithStrings to chord member notes
    if (fCurrentNoteClone->getNoteKind () != msrNote::kChordMemberNote)
      fCurrentNoteClone->
        addTechnicalWithStringToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      addTechnicalWithStringToChord (elt);
  }
}

void msr2LpsrTranslator::visitEnd (S_msrTechnicalWithString& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrTechnicalWithString" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrOrnament& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrOrnament" <<
      endl;

  if (fOnGoingNote) {
    // don't add ornaments to chord member notes
    if (fCurrentNoteClone->getNoteKind () != msrNote::kChordMemberNote)
      fCurrentNoteClone->
        addOrnamentToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      addOrnamentToChord (elt);
  }
}

void msr2LpsrTranslator::visitEnd (S_msrOrnament& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrOrnament" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrSingleTremolo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrSingleTremolo" <<
      endl;

  if (fOnGoingNote) {
    // don't add single tremolos to chord member notes
    if (fCurrentNoteClone->getNoteKind () != msrNote::kChordMemberNote)
      fCurrentNoteClone->
        addSingleTremoloToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      addSingleTremoloToChord (elt);
  }
}

void msr2LpsrTranslator::visitEnd (S_msrSingleTremolo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrSingleTremolo" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrDoubleTremolo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrDoubleTremolo" <<
      endl;

  // create a double tremolo clone
  fCurrentDoubleTremoloClone =
    elt->createDoubleTremoloBareClone (
      fCurrentVoiceClone);
  
  fOnGoingDoubleTremolo = true;
}

void msr2LpsrTranslator::visitEnd (S_msrDoubleTremolo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrSingleTremolo" <<
      endl;

  // append the current double tremolo clone to the current voice clone
  fCurrentVoiceClone->
    appendDoubleTremoloToVoice (
      fCurrentDoubleTremoloClone);

  // forget about it
  fCurrentDoubleTremoloClone = 0;
  
  fOnGoingDoubleTremolo = false;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrDynamics& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrDynamics" <<
      endl;

  if (fOnGoingNote) {
    // don't add dynamics to chord member notes
    if (fCurrentNoteClone->getNoteKind () != msrNote::kChordMemberNote)
      fCurrentNoteClone->
        addDynamicsToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      addDynamicsToChord (elt);
  }
}

void msr2LpsrTranslator::visitEnd (S_msrDynamics& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrDynamics" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrOtherDynamics& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrOtherDynamics" <<
      endl;

  if (fOnGoingNote) {
    // don't add other dynamics to chord member notes
    if (fCurrentNoteClone->getNoteKind () != msrNote::kChordMemberNote)
      fCurrentNoteClone->
        addOtherDynamicsToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      addOtherDynamicsToChord (elt);
  }
}

void msr2LpsrTranslator::visitEnd (S_msrOtherDynamics& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrOtherDynamics" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrWords& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrWords" <<
      endl;

  if (fOnGoingNote) {
    // don't add words to chord member notes
    if (fCurrentNoteClone->getNoteKind () != msrNote::kChordMemberNote)
      fCurrentNoteClone->
        addWordsToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      addWordsToChord (elt);
  }
}

void msr2LpsrTranslator::visitEnd (S_msrWords& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrWords" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrSlur& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrSlur" <<
      endl;

  if (fOnGoingNote) {
    // don't add slurs to chord member notes
    if (fCurrentNoteClone->getNoteKind () != msrNote::kChordMemberNote)
      fCurrentNoteClone->
        addSlurToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      addSlurToChord (elt);
  }
}

void msr2LpsrTranslator::visitEnd (S_msrSlur& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrSlur" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrLigature& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrLigature" <<
      endl;

  if (fOnGoingNote) {
    // don't add ligatures to chord member notes
    if (fCurrentNoteClone->getNoteKind () != msrNote::kChordMemberNote)
      fCurrentNoteClone->
        addLigatureToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      addLigatureToChord (elt);
  }
}

void msr2LpsrTranslator::visitEnd (S_msrLigature& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrLigature" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrWedge& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrWedge" <<
      endl;

  if (fOnGoingNote) {
    // don't add wedges to chord member notes
    if (fCurrentNoteClone->getNoteKind () != msrNote::kChordMemberNote)
      fCurrentNoteClone->
        addWedgeToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      addWedgeToChord (elt);
  }
}

void msr2LpsrTranslator::visitEnd (S_msrWedge& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrWedge" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrGracenotes& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrGracenotes" <<
      endl;

  bool doCreateAGraceNoteClone = true;
  
// JMI  bool createSkipGracenotesInOtherVoices = false;
  
  if (fFirstNoteCloneInVoice) {
    // there is at least a note before these grace notes in the voice
    
    if (
      fCurrentNoteClone->getNoteHasATrill ()
        &&
      fCurrentNoteClone->getNoteIsFollowedByGracenotes ()) {
      // fPendingAftergracenotes already contains
      // the aftergracenotes to use
      
      if (gGeneralOptions->fTraceGracenotes)
        cerr << idtr <<
          "Optimising grace notes " << 
          elt->gracenotesAsShortString () <<
          "into after grace notes" <<
          endl;

      // append the after grace notes to the current voice clone
      fCurrentVoiceClone->
        appendAftergracenotesToVoice (
          fPendingAftergracenotes);
      
      doCreateAGraceNoteClone = false;
    }
  }

  if (doCreateAGraceNoteClone) {
    // create a clone of this gracenotes
    fCurrentGracenotesClone =
      elt->
        createGracenotesBareClone (
          fCurrentVoiceClone);
  
    // append it to the current voice clone
    fCurrentVoiceClone->
      appendGracenotesToVoice (
        fCurrentGracenotesClone);
  
   // JMI XXL find good criterion for this

    // these grace notes are at the beginning of a segment JMI
//    doCreateAGraceNoteClone = true; // JMI

    // bug 34 in LilyPond should be worked aroud by creating
    // skip grance notes in the other voices of the part

    // create skip gracenotes clone
    if (gGeneralOptions->fTraceGracenotes)
      cerr << idtr <<
        "--> creating a skip clone of grace notes " <<
        elt->gracenotesAsShortString () <<
        " to work around LilyPond issue 34" <<
        endl;
  
    S_msrGracenotes
      skipGracenotes =
        elt->
          createSkipGracenotesClone (
            fCurrentVoiceClone);

    // prepend it to the other voices in the part
    prependSkipGracenotesToPartOtherVoices (
      fCurrentPartClone,
      fCurrentVoiceClone,
      skipGracenotes);
  }
}

void msr2LpsrTranslator::visitEnd (S_msrGracenotes& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrGracenotes" <<
      endl;

  // forget about these grace notes if any
  fCurrentGracenotesClone = 0;

  if (fPendingAftergracenotes) {
    // remove the current aftergracenotes note clone
    // from the current voice clone
    fCurrentVoiceClone->
      removeNoteFromVoice (
        elt->getInputLineNumber (),
        fCurrentAftergracenotesNote);
        
    fCurrentAftergracenotesNote = 0;
  
    // forget these after grace notes if any
    fPendingAftergracenotes = 0;
  }
}

void msr2LpsrTranslator::prependSkipGracenotesToPartOtherVoices (
  S_msrPart       partClone,
  S_msrVoice      voiceClone,
  S_msrGracenotes skipGracenotes)
{
    if (gGeneralOptions->fTraceGracenotes)
      cerr << idtr <<
        "--> prepending a skip gracenotes clone " <<
        skipGracenotes->gracenotesAsShortString () <<
        " to voices other than \"" <<
        voiceClone->getVoiceName () << "\"" <<
        " in part " <<
        partClone->getPartCombinedName () <<
        endl;
  
  map<int, S_msrStaff>
    partStavesMap =
      partClone->
        getPartStavesMap ();

  for (
    map<int, S_msrStaff>::const_iterator i=partStavesMap.begin();
    i!=partStavesMap.end();
    i++) {

    map<int, S_msrVoice>
      staffAllVoicesMap =
        (*i).second->
          getStaffAllVoicesMap ();
          
    for (
      map<int, S_msrVoice>::iterator j=staffAllVoicesMap.begin();
      j!=staffAllVoicesMap.end();
      j++) {

      S_msrVoice voice = (*j).second;
      
      if (voice != voiceClone) {
        // prepend skip grace notes to voice
        voice->
          prependGracenotesToVoice (
            skipGracenotes);
      }
    } // for

  } // for
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrNote& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fOstream << idtr <<
      "--> Start visiting " <<
      elt->noteAsString () <<
      endl;
  }

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  // create the clone
  fCurrentNoteClone =
    elt->createNoteBareClone (
      fCurrentPartClone);

  // register clone in this tranlastors' voice notes map
  fVoiceNotesMap [elt] = fCurrentNoteClone; // JMI XXL
  
  if (! fFirstNoteCloneInVoice)
    fFirstNoteCloneInVoice =
      fCurrentNoteClone;

  // can we optiomize gracenotes into aftergracenotes?
  if (
    elt->getNoteHasATrill ()
      &&
    elt->getNoteIsFollowedByGracenotes ()) {
      
    // yes, create the after grace notes
    fPendingAftergracenotes =
      msrAftergracenotes::create (
        inputLineNumber,
        fCurrentPartClone,
        fCurrentNoteClone,
        false, // aftergracenoteIsSlashed, may be updated later
        fCurrentVoiceClone);

    // register current aftergracenotes note
    fCurrentAftergracenotesNote =
      fCurrentNoteClone;
  }

  fOnGoingNote = true;
}

void msr2LpsrTranslator::visitEnd (S_msrNote& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fOstream << idtr <<
      "--> End visiting " <<
      elt->noteAsString () <<
      endl;
  }

  switch (fCurrentNoteClone->getNoteKind ()) {
    
    case msrNote::k_NoNoteKind:
      break;
      
    case msrNote::kStandaloneNote:
      if (gGeneralOptions->fTraceNotes) {
        cerr << idtr <<
          "Appending " <<
          fCurrentNoteClone->noteAsString () << " to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          endl;
      }
          
      fCurrentVoiceClone->
        appendNoteToVoiceClone (fCurrentNoteClone);
      break;
      
    case msrNote::kDoubleTremoloMemberNote:
      if (fOnGoingDoubleTremolo) {
        
        if (fCurrentNoteClone->getNoteIsFirstNoteInADoubleTremolo ()) {
          if (gGeneralOptions->fTraceNotes) {
            cerr <<  idtr <<
              "--> setting standalone note '" <<
              fCurrentNoteClone->noteAsString () <<
              "', line " << fCurrentNoteClone->getInputLineNumber () <<
              ", as double tremolo first element" <<
              " in voice \"" <<
              fCurrentVoiceClone->getVoiceName () <<
              "\"" <<
              endl;
          }
              
          fCurrentDoubleTremoloClone->
            setDoubleTremoloNoteFirstElement (
              fCurrentNoteClone);
        }
        
        else if (fCurrentNoteClone->getNoteIsSecondNoteInADoubleTremolo ()) {
          if (gGeneralOptions->fTraceNotes) {
            cerr <<  idtr <<
              "--> setting standalone note '" <<
              fCurrentNoteClone->noteAsString () <<
              "', line " << fCurrentNoteClone->getInputLineNumber () <<
              ", as double tremolo second element" <<
              " in voice \"" <<
              fCurrentVoiceClone->getVoiceName () <<
              "\"" <<
              endl;
          }
              
          fCurrentDoubleTremoloClone->
            setDoubleTremoloNoteSecondElement (
              fCurrentNoteClone);
        }
        
        else {
          stringstream s;

          s <<
            "note '" << fCurrentNoteClone->noteAsShortString () <<
            "' belongs to a double tremolo, but is not marked as such";

          msrInternalError (
            elt->getInputLineNumber (),
            s.str());
        }
      }

      else {
        stringstream s;

        s <<
          "double tremolo note '" << fCurrentNoteClone->noteAsShortString () <<
          "' met outside of a double tremolo";

        msrInternalError (
          elt->getInputLineNumber (),
          s.str());
      }
      break;
      
    case msrNote::kGraceNote:
      if (fCurrentGracenotesClone) {
        if (gGeneralOptions->fTraceGracenotes || gGeneralOptions->fTraceNotes) {
          cerr <<  idtr <<
            "Appending note " <<
            fCurrentNoteClone->notePitchAsString () <<
            ":" << fCurrentNoteClone->getNoteSoundingDivisions () <<
            " to the grace notes in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"" <<
            endl;
        }
  
        fCurrentGracenotesClone->
          appendNoteToGracenotes (
            fCurrentNoteClone);
      }

      else if (fPendingAftergracenotes) {
        if (gGeneralOptions->fTraceGracenotes || gGeneralOptions->fTraceNotes) {
          cerr <<  idtr <<
            "Appending note " <<
            fCurrentNoteClone->notePitchAsString () <<
            ":" << fCurrentNoteClone->getNoteSoundingDivisions () <<
            " to the after grace notes in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"" <<
            endl;
        }
  
        fPendingAftergracenotes->
          appendNoteToAftergracenotes (
            fCurrentNoteClone);
      }
      
      else {
        stringstream s;

        s <<
          "both fCurrentGracenotesClone and fPendingAftergracenotes are null," <<
          endl <<
          "cannot handle grace note'" <<
          elt->noteAsString () <<
          "'";

        msrInternalError (
          elt->getInputLineNumber (),
          s.str());
      }
      break;
      
    case msrNote::kRestNote:
      fCurrentVoiceClone->
        appendNoteToVoiceClone (fCurrentNoteClone);
      break;
      
    case msrNote::kSkipNote: // JMI
      fCurrentVoiceClone->
        appendNoteToVoiceClone (fCurrentNoteClone);
      break;
      
    case msrNote::kChordMemberNote:
      if (fOnGoingChord)
        fCurrentChordClone->
          addAnotherNoteToChord (fCurrentNoteClone);
      else {
        stringstream s;

        s <<
          "msr2LpsrTranslator:::visitEnd (S_msrNote& elt): chord member note " <<
          elt->noteAsString () <<
          " appears outside of a chord";

        msrInternalError (
          elt->getInputLineNumber (),
          s.str());
        }
      break;
      
    case msrNote::kTupletMemberNote:
      fTupletClonesStack.top ()->
        addNoteToTuplet (fCurrentNoteClone);
      break;
  } // switch

  // handle melisma
  msrSyllable::msrSyllableExtendKind
    noteSyllableExtendKind =
      elt->getNoteSyllableExtendKind ();

  switch (noteSyllableExtendKind) {
    case msrSyllable::kStandaloneSyllableExtend:
      {
        // create melisma start command
        S_lpsrMelismaCommand
          melismaCommand =
            lpsrMelismaCommand::create (
              elt->getInputLineNumber (),
              lpsrMelismaCommand::kMelismaStart);
    
        // append it to current voice clone
        fCurrentVoiceClone->
          appendOtherElementToVoice (melismaCommand);

        // append 

        fOnGoingSyllableExtend = true;
      }
      break;
    case msrSyllable::kStartSyllableExtend:
      break;
    case msrSyllable::kContinueSyllableExtend:
      break;
    case msrSyllable::kStopSyllableExtend:
      break;
    case msrSyllable::k_NoSyllableExtend:
      break;
  } // switch

  fOnGoingNote = false;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrOctaveShift& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrOctaveShift" <<
      endl;

  fCurrentVoiceClone->
    appendOctaveShiftToVoice (elt);
}

void msr2LpsrTranslator::visitEnd (S_msrOctaveShift& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrOctaveShift" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrStem& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrStem" <<
      endl;
}

void msr2LpsrTranslator::visitEnd (S_msrStem& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrStem" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrBeam& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrBeam" <<
      endl;
}

void msr2LpsrTranslator::visitEnd (S_msrBeam& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrBeam" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrChord& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrChord" <<
      endl;

  fCurrentChordClone =
    elt->createChordBareClone (
      fCurrentPartClone);

  if (fTupletClonesStack.size ()) {
    // a chord in a tuplet is handled as part of the tuplet JMI
    fTupletClonesStack.top ()->
      addChordToTuplet (
        fCurrentChordClone);
  }

  else if (fOnGoingDoubleTremolo) {
    if (elt->getChordIsFirstChordInADoubleTremolo ()) {
      // replace double tremolo's first element by chord
      fCurrentDoubleTremoloClone->
        setDoubleTremoloChordFirstElement (
          elt);
    }
    
    else if (elt->getChordIsSecondChordInADoubleTremolo ()) {
      // replace double tremolo's second element by chord
      fCurrentDoubleTremoloClone->
        setDoubleTremoloChordSecondElement (
          elt);
    }
    
    else {
      stringstream s;

      s <<
        "chord '" << elt->chordAsString () <<
        "' belongs to a double tremolo, but is not marked as such";

      msrInternalError (
        elt->getInputLineNumber (),
        s.str());
    }
  }
  
  else {
    // appending the chord to the voice clone at once
    fCurrentVoiceClone->
      appendChordToVoice (
        fCurrentChordClone);
  }

  fOnGoingChord = true;
}

void msr2LpsrTranslator::visitEnd (S_msrChord& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrChord" <<
      endl;

  fOnGoingChord = false;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrTuplet& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrTuplet" <<
      endl;

  // create the tuplet clone
  S_msrTuplet
    tupletClone =
      elt->createTupletBareClone ();

  // register it in this visitor
  if (gGeneralOptions->fTraceTuplets)
    cerr << idtr <<
      "++> pushing tuplet '" <<
      tupletClone->tupletAsString () <<
      "' to tuplets stack" <<
      endl;
      
  fTupletClonesStack.push (tupletClone);
}

void msr2LpsrTranslator::visitEnd (S_msrTuplet& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrTuplet" <<
      endl;

  if (gGeneralOptions->fTraceTuplets)
    cerr << idtr <<
      "--> popping tuplet '" <<
      elt->tupletAsString () <<
      "' from tuplets stack" <<
      endl;
      
    fTupletClonesStack.pop ();

  if (fTupletClonesStack.size ()) {
    
    // tuplet is a nested tuplet
    if (gGeneralOptions->fTraceTuplets)
      cerr << idtr <<
        "=== adding nested tuplet '" <<
      elt->tupletAsString () <<
        "' to stack top tuplet '" <<
      fTupletClonesStack.top ()->tupletAsString () <<
      "'" <<
      endl;
    
    fTupletClonesStack.top ()->
      addTupletToTupletClone (elt);
  }
  
  else {
    // tuplet is a top level tuplet
    
    if (gGeneralOptions->fTraceTuplets)
      cerr << idtr <<
        "=== adding top level tuplet '" <<
      elt->tupletAsString () <<
      "' to voice" <<
      fCurrentVoiceClone->getVoiceName () <<
      endl;
      
    fCurrentVoiceClone->
      appendTupletToVoice (elt);
  }  
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrTie& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrTie" <<
      endl;

  // we don't need the kTieStop or kContinue for LilyPond
  if (elt->getTieKind () == msrTie::kStartTie)
    fCurrentNoteClone->
      setNoteTie (elt);
}

void msr2LpsrTranslator::visitEnd (S_msrTie& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrTie" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrSegno& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrSegno" <<
      endl;

  fCurrentVoiceClone->
    appendSegnoToVoice (elt);
}

void msr2LpsrTranslator::visitStart (S_msrCoda& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrCoda" <<
      endl;

  fCurrentVoiceClone->
    appendCodaToVoice (elt);
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrEyeglasses& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting eyeglasses" <<
      endl;

  fCurrentVoiceClone->
    appendEyeglassesToVoice (elt);
}

void msr2LpsrTranslator::visitStart (S_msrPedal& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting pedal" <<
      endl;

  fCurrentVoiceClone->
    appendPedalToVoice (elt);
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrBarCheck& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrBarCheck" <<
      endl;

  fCurrentVoiceClone->
    appendBarCheckToVoice (elt);
}

void msr2LpsrTranslator::visitEnd (S_msrBarCheck& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrBarCheck" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrBarnumberCheck& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrBarnumberCheck" <<
      endl;

  fCurrentVoiceClone->
    appendBarnumberCheckToVoice (elt);
}

void msr2LpsrTranslator::visitEnd (S_msrBarnumberCheck& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrBarnumberCheck" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrBreak& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrBreak" <<
      endl;

  fCurrentVoiceClone->
    appendBreakToVoice (elt);
}

void msr2LpsrTranslator::visitEnd (S_msrBreak& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrBreak" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrRepeat& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrRepeat" <<
      endl;

/* JMI
  // create a repeat clone
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Creating a repeat bare clone" <<
      ", line " << elt->getInputLineNumber () <<
      ", in voice \"" <<
      elt->
        getRepeatVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  */
  
  fOnGoingRepeat = true;
}

void msr2LpsrTranslator::visitEnd (S_msrRepeat& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrRepeat" <<
      endl;

  fRepeatHasBeenCreatedForCurrentPartClone = false;
  
  fOnGoingRepeat = false;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrRepeatending& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrRepeatending" <<
      endl;
}

void msr2LpsrTranslator::visitEnd (S_msrRepeatending& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrRepeatending" <<
      endl;

  // forget current repeat ending clone
  fCurrentRepeatendingClone = 0;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrMeasureRepeat& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrMeasureRepeat" <<
      endl;

  fOstream << idtr <<
    "\repeat percent { " <<
    endl;

  idtr++;
}

void msr2LpsrTranslator::visitEnd (S_msrMeasureRepeat& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrMeasureRepeat" <<
      endl;

  idtr--;
  
  fOstream << idtr <<
    "}" <<
    endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrBarline& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrBarline" <<
      endl;
  
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  switch (elt->getBarlineCategory ()) {
    
    case msrBarline::kStandaloneBar:
      {
        if (gGeneralOptions->fTraceRepeats)
          cerr << idtr <<
            "--> handling kStandaloneBar in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"" <<
            endl;

        // append the barline to the current voice clone
        fCurrentVoiceClone->
          appendBarlineToVoice (elt);
      }
      break;
      
    case msrBarline::kRepeatStart:
      {
        if (gGeneralOptions->fTraceRepeats)
          cerr << idtr <<
            "--> handling kRepeatStart in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"" <<
            endl;
    
        // append the barline to the current voice clone
        fCurrentVoiceClone->
          appendBarlineToVoice (elt);
      }
      break;

    case msrBarline::kRepeatEnd:
      {
        if (gGeneralOptions->fTraceRepeats)
          cerr << idtr <<
            "--> handling kRepeatEnd in voice " <<
            fCurrentVoiceClone->getVoiceName () << "\"" <<
            endl;
  
        // append the barline to the current voice clone
        fCurrentVoiceClone->
          appendBarlineToVoice (elt);

        // append the repeat clone to the current part clone
        if (gGeneralOptions->fTraceRepeats)
          cerr << idtr <<
            "Appending a repeat to part clone " <<
            fCurrentPartClone->getPartCombinedName () << "\"" <<
            endl;
  
        fCurrentPartClone-> // no test needed JMI
          createAndAppendRepeatToPart (
            inputLineNumber);
        }
      break;
            
    case msrBarline::kHookedEndingStart:
      {
        if (gGeneralOptions->fTraceRepeats)
          cerr << idtr <<
            "--> handling kHookedEndingStart in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"" <<
            endl;

        if (! fRepeatHasBeenCreatedForCurrentPartClone) {
          // append a repeat clone to the current part clone
          if (gGeneralOptions->fTraceRepeats)
            cerr << idtr <<
              "Appending a repeat to part clone " <<
              fCurrentPartClone->getPartCombinedName () << "\"" <<
              endl;
    
          fCurrentPartClone-> // no null test needed JMI
            createAndAppendRepeatToPart (
              inputLineNumber);
  
          fRepeatHasBeenCreatedForCurrentPartClone = true;
        }

        // append the barline to the current voice clone
        fCurrentVoiceClone->
          appendBarlineToVoice (elt);
      }
      break;
      
    case msrBarline::kHookedEndingEnd:
      {
        if (gGeneralOptions->fTraceRepeats)
          cerr << idtr <<
            "--> handling kHookedEndingEnd in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"" <<
            endl;
  
        // append the barline to the current voice clone
        fCurrentVoiceClone->
          appendBarlineToVoice (elt);
    
        if (gGeneralOptions->fTraceRepeats)
          cerr << idtr <<
            "Appending a hooked repeat ending clone to voice clone \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"" <<
            endl;

        fCurrentPartClone->
          appendRepeatendingToPart (
            inputLineNumber,
            elt->getEndingNumber (),
            msrRepeatending::kHookedEnding);
      }
      break;
      
    case msrBarline::kHooklessEndingStart:
      {
        if (gGeneralOptions->fTraceRepeats)
          cerr << idtr <<
            "--> handling kHooklessEndingStart in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"" <<
            endl;
  
        // append the barline to the current voice clone
        fCurrentVoiceClone->
          appendBarlineToVoice (elt);
      }
      break;

    case msrBarline::kHooklessEndingEnd:
      {
        if (gGeneralOptions->fTraceRepeats)
          cerr << idtr <<
            "--> handling kHooklessEndingEnd in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"" <<
            endl;
  
        // append the barline to the current voice clone
        fCurrentVoiceClone->
          appendBarlineToVoice (elt);

        if (gGeneralOptions->fTraceRepeats)
          cerr << idtr <<
            "Appending a hookless repeat ending clone to voice clone \" " <<
            fCurrentVoiceClone->getVoiceName () << "\"" <<
            endl;

        fCurrentPartClone->
          appendRepeatendingToPart (
            inputLineNumber,
            elt->getEndingNumber (),
            msrRepeatending::kHookedEnding);
      }
      break;
  } // switch
}

void msr2LpsrTranslator::visitEnd (S_msrBarline& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrBarline" <<
      endl;
}

//________________________________________________________________________
/*void msr2LpsrTranslator::visitStart (S_msrComment& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrComment" <<
      endl;
}

void msr2LpsrTranslator::visitEnd (S_msrComment& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtrfTraceVisitors
      "--> End visiting msrComment" <<
      endl;
}
*/
//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrVarValAssoc& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrVarValAssoc" <<
      endl;

  string variableName     = elt->getVariableName ();
  string variableValueAux = elt->getVariableValue ();
  int    inputLineNumber  = elt->getInputLineNumber ();
  string variableValue;

  // escape quotes if any
  for_each (
    variableValueAux.begin(),
    variableValueAux.end(),
    stringQuoteEscaper (variableValue));

  if      (variableName == "work-number") {
    fLpsrScoreHeader->
      setWorkNumber (
        inputLineNumber, variableValue);

    fWorkNumberKnown = true;
  }
  
  else if (variableName == "work-title") {
    fLpsrScoreHeader->
      setWorkTitle (
        inputLineNumber, variableValue);
        
    fWorkTitleKnown = true;
  }
  
  else if (variableName == "movement-number") {
    fLpsrScoreHeader->
      setMovementNumber (
        inputLineNumber, variableValue);

    fMovementNumberKnown = true;
  }
  
  else if (variableName == "movement-title") {
    fLpsrScoreHeader->
      setMovementTitle (
        inputLineNumber, variableValue);
        
    fMovementTitleKnown = true;
  }
  
  else if (variableName == "composer") {
    S_lpsrLilypondVarValAssoc
      assoc =
        fLpsrScoreHeader->
          addComposer (
            inputLineNumber, variableName, variableValue);
  }
  
  else if (variableName == "arranger") {
    S_lpsrLilypondVarValAssoc
      assoc =
        fLpsrScoreHeader->
          addArranger (
            inputLineNumber, variableName, variableValue);
  }
  
  else if (variableName == "poet") {
    S_lpsrLilypondVarValAssoc
      assoc =
        fLpsrScoreHeader->
          addLyricist ( // JMI
            inputLineNumber, "poet", variableValue);
  }

  else if (variableName == "lyricist") {
    S_lpsrLilypondVarValAssoc
      assoc =
        fLpsrScoreHeader->
          addLyricist (
            inputLineNumber, "lyricist", variableValue); // JMI ???
  }

  else if (variableName == "rights") {
    fLpsrScoreHeader->setRights (
        inputLineNumber, variableValue);

    fLpsrScoreHeader->
      changeRightsTitleVariableName ("copyright");
  }
  
  else if (variableName == "software") {
    fLpsrScoreHeader->addSoftware (
      inputLineNumber, variableValue);
  }
  
  else if (variableName == "encoding-date") {
    fLpsrScoreHeader->setEncodingDate (
      inputLineNumber, variableValue);
  }

  else {
    stringstream s;

    s <<
      "### msrVarValAssoc name '" << variableName << "'" <<
      " is not handled";

    msrMusicXMLWarning (
      elt->getInputLineNumber (),
      s.str());
  }
}

void msr2LpsrTranslator::visitEnd (S_msrVarValAssoc& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrVarValAssoc" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrLayout& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrLayout" <<
      endl;

  idtr++;
}

void msr2LpsrTranslator::visitEnd (S_msrLayout& elt)
{
  idtr--;

  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrLayout" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrMidi& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrMidi" <<
      endl;
}

void msr2LpsrTranslator::visitEnd (S_msrMidi& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrMidi" <<
      endl;
}

//________________________________________________________________________
void msr2LpsrTranslator::visitStart (S_msrRehearsal& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrRehearsal" <<
      endl;

  fCurrentVoiceClone->
    appendRehearsalToVoice (elt);
}

void msr2LpsrTranslator::visitEnd (S_msrRehearsal& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrRehearsal" <<
      endl;
}


} // namespace


/* JMI
 *   fCurrentVoiceClone =
    elt->createVoiceBareClone (fCurrentStaffClone);
    
  fCurrentStaffClone->
    registerVoiceInStaff (fCurrentVoiceClone);

  // append the voice to the LPSR score elements list
  fLpsrScore ->
    appendVoiceToScoreElements (fCurrentVoiceClone);

  // append the voice use to the LPSR score block
  fLpsrScore ->
    appendVoiceUseToStoreBlock (fCurrentVoiceClone);
*/


