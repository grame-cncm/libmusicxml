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
  /* JMI
    : gCurrentMusicXMLLocation (
      0, // before first line on MusicXML contents
      0, // in case of an anacrusis
      0) // at the beginning of a measure
      */
{
  fMsrOptions      = msrOpts;
  fLpsrOptions     = lpsrOpts;
  fVisitedMsrScore = mScore;

  /* JMI
    : gCurrentMusicXMLLocation (
      0, // before first line on MusicXML contents
      0, // in case of an anacrusis
      0) // at the beginning of a measure
      */
  gCurrentMusicXMLLocation.fInputLineNumber = 0;
  gCurrentMusicXMLLocation.fMeasureNumber = 0; // in case of an anacrusis
  gCurrentMusicXMLLocation.fPositionInMeasure = 1;

  fOnGoingIdentification = false;
  
  fOnGoingStaff          = false;
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
  fCurrentMsrScoreClone =
    fVisitedMsrScore->createEmptyClone ();

  // create the LPSR score
  fLpsrScore =
    lpsrScore::create (
      fMsrOptions, fLpsrOptions, 0, fCurrentMsrScoreClone);
/*
  // push it onto this visitors's stack,
  // making it the current partgroup command
  fPartgroupCommandsStack.push (
    partgroupCommand);
    */
}

void msr2LpsrVisitor::visitEnd (S_msrScore& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrScore" << endl;

//  fLpsrScore->
//    setScoreCommand (fPartgroupCommandsStack.pop ());

/*
  // get top level pargroup command from the stack
  S_lpsrPartgroupCommand
    partgroupCommand =
      fPartgroupCommandsStack.top ();

  // pop it from the stack
  fPartgroupCommandsStack.top ();

  // the stack should now be empty
  if (fPartgroupCommandsStack.size())
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
      fLpsrScore->getScoreCommand ()->getScoreCommandLayout ();

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
  fCurrentMsrPartgroupClone =
    elt->createEmptyClone ();

  // add it to the MSR score clone
  fCurrentMsrScoreClone->
    addPartgroupToScore (fCurrentMsrPartgroupClone);

  // create a partgroup command
  S_lpsrPartgroupCommand
    partgroupCommand =
      lpsrPartgroupCommand::create (
        fMsrOptions, fLpsrOptions, fCurrentMsrPartgroupClone);

  // push it onto this visitors's stack,
  // making it the current partgroup command
  fPartgroupCommandsStack.push (
    partgroupCommand);
  
  // get the LPSR store command
  S_lpsrScoreCommand
    scoreCommand =
      fLpsrScore->getScoreCommand ();

  // append the pargroup clone to the score command
  scoreCommand->
    appendPartgroupCommandToParallelMusic (
      partgroupCommand);

  // append the partgroup command to the LPSR score command

/*
 *   fCurrentMsrVoiceClone =
    elt->createEmptyClone (fCurrentMsrStaffClone);
    
  fCurrentMsrStaffClone->
    addVoiceToStaff (fCurrentMsrVoiceClone);

  // append the voice to the LPSR score elements list
  fLpsrScore ->
    appendVoiceToScoreElements (fCurrentMsrVoiceClone);

  // append the voice use to the LPSR score command
  fLpsrScore ->
    appendVoiceUseToStoreCommand (fCurrentMsrVoiceClone);
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
  fPartgroupCommandsStack.pop ();
      
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrPart& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrPart" << endl;

  idtr++;

  // create a part clone
  fCurrentMsrPartClone =
    elt->createEmptyClone (fCurrentMsrPartgroupClone);

  // add it to the partgroup clone
  fCurrentMsrPartgroupClone->
    addPartToPartgroup (fCurrentMsrPartClone);

  // create a part command
  fCurrentPartCommand =
    lpsrPartCommand::create (
      fMsrOptions, fLpsrOptions, fCurrentMsrPartClone);

  // append it to the current partgroup command
  fPartgroupCommandsStack.top ()->
    appendElementToPartgroupCommand (fCurrentPartCommand);
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
  fCurrentMsrStaffClone =
    elt->createEmptyClone (fCurrentMsrPartClone);
    
  // add it to the part clone
  fCurrentMsrPartClone->
    addStaffToPart (fCurrentMsrStaffClone);

  // create a staff command
  fCurrentStaffCommand =
    lpsrStaffCommand::create (
      fMsrOptions, fLpsrOptions, fCurrentMsrStaffClone);

  // append it to the current part command
  fCurrentPartCommand->
    appendElementToPartCommand (fCurrentStaffCommand);

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
  fCurrentMsrVoiceClone =
    elt->createEmptyClone (fCurrentMsrStaffClone);
    
  // add it to the staff clone
  fCurrentMsrStaffClone->
    addVoiceToStaff (fCurrentMsrVoiceClone);

  // append the voice to the LPSR score elements list
  fLpsrScore ->
    appendVoiceToScoreElements (fCurrentMsrVoiceClone);

  // append a use of the voice to the current staff command
  fCurrentStaffCommand->
    appendVoiceUseToStaffCommand (fCurrentMsrVoiceClone);
}

void msr2LpsrVisitor::visitEnd (S_msrVoice& elt)
{
  idtr--;

  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrVoice" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrLyrics& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrLyrics" << endl;

  idtr++;

//  if (elt->getLyricsTextPresent ()) { // JMI
    fCurrentMsrLyricsClone =
      elt->createEmptyClone (fCurrentMsrVoiceClone);
  
    // don't add the lyrics to fCurrentMsrVoiceClone
  
    // append the lyrics to the LPSR score elements list
    fLpsrScore ->
      appendLyricsToScoreElements (fCurrentMsrLyricsClone);
  
    // append a use of the lyrics to the current staff command
    fCurrentStaffCommand ->
      appendLyricsUseToStaffCommand (fCurrentMsrLyricsClone);
//  }
//  else
  //  fCurrentMsrLyricsClone = 0; // JMI
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

  fCurrentMsrLyricschunkClone =
    elt->createEmptyClone ();
    
// JMI  fCurrentMsrLyricsClone->
    //addChunkToLyrics (fCurrentMsrLyricschunkClone);
  fCurrentMsrLyricsClone->
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

  fCurrentMsrVoiceClone->
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

  fCurrentMsrVoiceClone->
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

  fCurrentMsrVoiceClone->
    appendTimeToVoice (elt);
}

void msr2LpsrVisitor::visitEnd (S_msrTime& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrTime" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrTempo& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrTempo" << endl;

  fCurrentMsrSequentialMusicClone->
    appendElementToSequentialMusic (elt);
}

void msr2LpsrVisitor::visitEnd (S_msrTempo& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrTempo" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrSequentialMusic& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrSequentialMusic" << endl;

  // a sequential music instance has already JMI
  // been created for fCurrentMsrVoiceClone
  fCurrentMsrSequentialMusicClone =
    fCurrentMsrVoiceClone->getVoiceSequentialMusic ();
}

void msr2LpsrVisitor::visitEnd (S_msrSequentialMusic& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrSequentialMusic" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrParallelMusic& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrParallelMusic" << endl;
}

void msr2LpsrVisitor::visitEnd (S_msrParallelMusic& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrParallelMusic" << endl;
}

//________________________________________________________________________
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
      fCurrentMsrSequentialMusicClone->
        appendElementToSequentialMusic (elt);
      break;
      
    case msrNote::kRestNote:
      fCurrentMsrSequentialMusicClone->
        appendElementToSequentialMusic (elt);
      break;
      
    case msrNote::kChordMemberNote:
      fCurrentMsrChordClone->
        addNoteToChord (elt);
      break;
      
    case msrNote::kTupletMemberNote:
      fCurrentMsrTupletClone->
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

  fCurrentMsrChordClone =
    elt->createEmptyClone ();

    /* JMI
  fCurrentMsrVoiceClone->
    appendChordToVoice (fCurrentMsrChordClone);
    */

  fCurrentMsrSequentialMusicClone->
    appendElementToSequentialMusic (fCurrentMsrChordClone);
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

  fCurrentMsrTupletClone =
    elt->createEmptyClone ();

  fCurrentMsrSequentialMusicClone->
    appendElementToSequentialMusic (fCurrentMsrTupletClone);
}

void msr2LpsrVisitor::visitEnd (S_msrTuplet& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrTuplet" << endl;
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
void msr2LpsrVisitor::visitStart (S_msrBarLine& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrBarLine" << endl;
}

void msr2LpsrVisitor::visitEnd (S_msrBarLine& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrBarLine" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrBarNumberCheck& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrBarNumberCheck" << endl;
}

void msr2LpsrVisitor::visitEnd (S_msrBarNumberCheck& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrBarNumberCheck" << endl;
}

//________________________________________________________________________
void msr2LpsrVisitor::visitStart (S_msrBreak& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrBreak" << endl;
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
}

void msr2LpsrVisitor::visitEnd (S_msrRepeat& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrRepeat" << endl;
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
