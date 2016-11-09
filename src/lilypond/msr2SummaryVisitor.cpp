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

#include "msr.h"
#include "msr2SummaryVisitor.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
msr2SummaryVisitor::msr2SummaryVisitor (
  S_msrOptions& msrOpts)
  /* JMI
    : gCurrentMusicXMLLocation (
      0, // before first line on MusicXML contents
      0, // in case of an anacrusis
      0) // at the beginning of a measure
      */
{
  fMsrOptions = msrOpts;

  /* JMI
    : gCurrentMusicXMLLocation (
      0, // before first line on MusicXML contents
      0, // in case of an anacrusis
      0) // at the beginning of a measure
      */
  gCurrentMusicXMLLocation.fInputLineNumber = 0;
  gCurrentMusicXMLLocation.fMeasureNumber = 0; // in case of an anacrusis
  gCurrentMusicXMLLocation.fPositionInMeasure = 1;

  fStandaloneNotesCounter = 0;
  fRestNotesCounter = 0;
  fChordNotesCounter = 0;
  fTupletNotesCounter = 0;

  fChordsCounter = 0;
};
  

/*
  fCurrentMusicXMLDivisions = 0;

  // create the MSR score
  fMsrScore =
    msrScore::create (fMsrOptions, 0);

  fCurrentTimeStaffNumber = 1; // it may be absent
  
  fCurrentLyricsNumber = 0;
  fCurrentLyricsChunkType = msrLyricsChunk::k_NoChunk;

  fOnGoingChord = false;
  
  fOnGoingTuplet = false;

  fCurrentBackupDuration = -1;

  fOnGoingNote = false;

  fOnGoingChord = false;
  
  fOnGoingSlur = false;

  fOnGoingDirection = false;

  fOnGoingBackup  = false;
  fOnGoingForward = false;
  */

msr2SummaryVisitor::~msr2SummaryVisitor ()
{}

//________________________________________________________________________
void msr2SummaryVisitor::printSummaryFromMsrScore (
  const S_msrScore& score)
{
  if (score) {    
    // create a msrScore browser
 // JMI   msrBrowser<msrElement> browser (this);
    msrBrowser<msrScore> browser (this);

    // browse the score with the browser
    browser.browse (*score);
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrScore& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrScore" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrScore& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrScore" << endl;

  idtr++;
  
  cerr <<
    idtr << "there are " <<
      fStandaloneNotesCounter << " standalone notes" << endl <<
    idtr << "there are " <<
      fRestNotesCounter << " rest notes" << endl <<
    idtr << "there are " <<
      fChordNotesCounter << " chord notes" << endl <<
    idtr << "there are " <<
      fTupletNotesCounter << " tuplet notes" << endl;

  cerr <<
    idtr << "there are " <<
      fChordsCounter << " chords" << endl;

  idtr--;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrPartgroup& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrPartgroup" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrPartgroup& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrPartgroup" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrPart& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrPart" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrPart& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrPart" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrStaff& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrStaff" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrStaff& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrStaff" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrClef& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrClef" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrClef& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrClef" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrKey& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrKey" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrKey& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrKey" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrTime& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrTime" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrTime& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrTime" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrTempo& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrTempo" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrTempo& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrTempo" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrVoice& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrVoice" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrVoice& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrVoice" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrLyrics& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrLyrics" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrLyrics& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrLyrics" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrLyricsChunk& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrLyricsChunk" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrLyricsChunk& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrLyricsChunk" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrSequentialMusic& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrSequentialMusic" << endl;

  int sequenceElementsSize =
    elt->getSequentialMusicElements ().size();

  cerr <<
    "SequentialMusic" <<
    " contains " << sequenceElementsSize;
  if (sequenceElementsSize == 1)
    cerr << " element";
  else
    cerr << " elements";
  
  if (! sequenceElementsSize)
    cerr << " (No actual notes)";
  cerr << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrSequentialMusic& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrSequentialMusic" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrParallelMusic& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrParallelMusic" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrParallelMusic& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrParallelMusic" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrDuration& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrDuration" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrDuration& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrDuration" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrArticulation& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrArticulation" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrArticulation& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrArticulation" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrDynamics& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrDynamics" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrDynamics& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrDynamics" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrWedge& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrWedge" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrWedge& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrWedge" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrNote& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting ";

  switch (elt->getNoteKind ()) {
    case msrNote::kStandaloneNote:
      cerr << "standalone";
      fStandaloneNotesCounter++;
      break;
    case msrNote::kRestNote:
      cerr << "rest";
      fRestNotesCounter++;
      break;
    case msrNote::kChordMemberNote:
      cerr << "chord member";
      fChordNotesCounter++;
      break;
    case msrNote::kTupletMemberNote:
      cerr << "tuplet member";
      fTupletNotesCounter++;
      break;
  } // switch
  cerr << " msrNote" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrNote& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting ";

  switch (elt->getNoteKind ()) {
    case msrNote::kStandaloneNote:
      cerr << "standalone";
      break;
    case msrNote::kRestNote:
      cerr << "rest";
      break;
    case msrNote::kChordMemberNote:
      cerr << "chord member";
      break;
    case msrNote::kTupletMemberNote:
      cerr << "tuplet member";
      break;
  } // switch
  cerr << " msrNote" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrBeam& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrBeam" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrBeam& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrBeam" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrChord& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrChord" << endl;

  fChordsCounter++;
}

void msr2SummaryVisitor::visitEnd (S_msrChord& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrChord" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrTuplet& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrTuplet" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrTuplet& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrTuplet" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrSlur& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrSlur" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrSlur& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrSlur" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrBarLine& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrBarLine" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrBarLine& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrBarLine" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrBarNumberCheck& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrBarNumberCheck" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrBarNumberCheck& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrBarNumberCheck" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrBreak& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrBreak" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrBreak& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrBreak" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrRepeat& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrRepeat" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrRepeat& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrRepeat" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrComment& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrComment" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrComment& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrComment" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrVarValAssoc& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrVarValAssoc" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrVarValAssoc& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrVarValAssoc" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrHeader& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrHeader" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrHeader& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrHeader" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrPaper& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrPaper" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrPaper& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrPaper" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrLayout& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrLayout" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrLayout& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrLayout" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrMidi& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrMidi" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrMidi& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrMidi" << endl;
}


} // namespace
