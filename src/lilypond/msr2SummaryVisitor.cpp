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
}

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
void msr2SummaryVisitor::visitStart (S_msrSequentialMusic& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrSequentialMusic" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrSequentialMusic& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrSequentialMusic" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrNote& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> Start visiting msrNote" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrNote& elt)
{
  if (fMsrOptions->fDebug)
    cerr << idtr <<
      "--> End visiting msrNote" << endl;
}


} // namespace
