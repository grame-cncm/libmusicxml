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
  S_translationSettings& ts)
  /* JMI
    : gCurrentMusicXMLLocation (
      0, // before first line on MusicXML contents
      0, // in case of an anacrusis
      0) // at the beginning of a measure
      */
{
  fTranslationSettings = ts;

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
    msrScore::create (fTranslationSettings, 0);

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
void msr2SummaryVisitor::printSummaryFromMsrElementTree (
         const
  S_msrElement& msrTree)
{
  if (msrTree) {
    /* JMI
    // create the msr tree browser
    S_msrBrowser
      browser =
        msrBrowser::create (
          fTranslationSettings, this);
  
    // browse the score with the visitor
    browser->browse (msrTree);
    */
  }
}


//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrScore& elt)
{
  if (fTranslationSettings->fTrace)
    cerr << idtr <<
      "Start visiting msrScore" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrScore& elt)
{
  if (fTranslationSettings->fTrace)
    cerr << idtr <<
      "End visiting msrScore" << endl;
}

} // namespace
