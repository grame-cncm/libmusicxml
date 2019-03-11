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

#include <iostream>

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "versions.h"

#include "generalOptions.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "msr2BsrInterface.h"

#include "msr2BsrTranslator.h"

using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
S_bsrScore buildBsrScoreFromMsrScore (
  const S_msrScore mScore,
  S_msrOptions     msrOpts,
  S_bsrOptions     bsrOpts,
  indentedOstream& logIOstream)
{
  // sanity check
  msrAssert (
    mScore != 0,
    "mScore is null");
    
  clock_t startClock = clock ();
      
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePasses) {
    string separator =
      "%--------------------------------------------------------------";
  
    logIOstream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      "Pass 3a: translating the MSR into a BSR" <<
      endl <<
      separator <<
      endl;
  }
#endif
 
  // create an msr2BsrTranslator
  msr2BsrTranslator
    translator (
      logIOstream,
      mScore);
      
  // build the BSR score
  translator.buildBsrScoreFromMsrScore ();
  logIOstream <<
    endl;

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 3a",
    "translate MSR to BSR",
    timingItem::kMandatory,
    startClock,
    endClock);

  // get the BSR score
  S_bsrScore
    bScore =
      translator.getBsrScore ();

  // return it
  return bScore;
}

//_______________________________________________________________________________
void displayFirstBsrScore (
  const S_bsrScore bScore,
  S_msrOptions     msrOpts,
  S_bsrOptions     bsrOpts,
  indentedOstream& logIOstream)
{
  // sanity check
  msrAssert (
    bScore != 0,
    "bScore is null");
    
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  logIOstream <<
    separator <<
    endl <<
    gTab <<
    "Optional pass: displaying the first BSR as text" <<
    endl <<
    separator <<
    endl <<
    endl <<
    bScore <<
    separator <<
    endl <<
    endl;

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "",
    "display the first BSR",
    timingItem::kOptional,
    startClock,
    endClock);
}


}
