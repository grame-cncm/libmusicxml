/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <climits>      // INT_MIN, INT_MAX

#include "bsrScores.h"

#include "version.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "xml2brlOptionsHandling.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
S_bsrScore bsrScore::create (
  int            inputLineNumber,
  S_msrScore     mScore)
{
  bsrScore* o = new bsrScore (
    inputLineNumber, mScore);
  assert(o!=0);
  return o;
}

bsrScore::bsrScore (
  int            inputLineNumber,
  S_msrScore     mScore)
    : bsrElement (inputLineNumber)
{
  fMsrScore = mScore;

  // allow for unlimited lines and pages lengthes initially,
  // until the limits are enforced when refining the BSR
  fBrailleLineLength = INT_MAX;
  fBraillePageLength = INT_MAX;
}

bsrScore::~bsrScore ()
{}

void bsrScore::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrScore::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_bsrScore>*
    p =
      dynamic_cast<visitor<S_bsrScore>*> (v)) {
        S_bsrScore elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrScore::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrScore::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrScore::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrScore>*
    p =
      dynamic_cast<visitor<S_bsrScore>*> (v)) {
        S_bsrScore elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrScore::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void bsrScore::browseData (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrScore::browseData ()" <<
      endl;
  }

  if (fTranscriptionNotes) {
    // browse the input source name comment
    msrBrowser<bsrTranscriptionNotes> browser (v);
    browser.browse (*fTranscriptionNotes);
  }

  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% <== bsrScore::browseData ()" <<
      endl;
  }
}

void bsrScore::print (ostream& os)
{
  os <<
    "BSR Score" <<
    endl <<
    endl;

  gIndenter++;

  // print the MSR structure (without the voices)
  fMsrScore->
    printSummary (os);
  os <<
    endl;

  // print the transcription notes if any
  const int fieldWidth = 19;
  
  if (fTranscriptionNotes || gBsrOptions->fDisplayBsrDetails) {
    os <<
      setw (fieldWidth) <<
      "TranscriptionNotes" << " : " <<
      endl;
        
    gIndenter++;

    if (fTranscriptionNotes) {
      os <<
        fTranscriptionNotes <<
        endl;
    }
    else {
      os <<
        "none" <<
        endl;
    }
      
    gIndenter--;
  }
  os <<
    endl;

  // print the lines and pages maximum lengthes
  os <<
    setw (fieldWidth) <<
    "BrailleLineLength" << " : " << fBrailleLineLength <<
    endl <<
    setw (fieldWidth) <<
    "BraillePageLength" << " : " << fBraillePageLength <<
    endl;
        
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrScore& scr)
{
  scr->print (os);
  return os;
}


}
