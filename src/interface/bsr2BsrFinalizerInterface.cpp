/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "messagesHandling.h"

#include "generalOah.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "bsr2BsrFinalizer.h"

#include "bsr2BsrFinalizerInterface.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
S_bsrScore generateFinalizedBsrScoreFromBsrScore (
  const S_bsrScore bScore,
  S_bsrOah     bsrOpts,
  indentedOstream& logOstream)
{
  // sanity check
  msrAssert (
    bScore != 0,
    "bScore is null");

  S_bsrScore result;

  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

#ifdef TRACE_OAH
  if (gTraceOah->fTracePasses) {
    logOstream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      "Pass 3b: finalizing the BSR as a new BSR" <<
      endl <<
      separator <<
      endl;
  }
#endif

  // create an bsr2BsrFinalizer
  bsr2BsrFinalizer
    translator (
      bsrOpts,
      logOstream,
      bScore);

  // build the finalized BSR score
  translator.generateFinalizedBsrScoreFromBsrScore ();
  logOstream << endl;

  // get the result
  result = translator.getBsrScore ();

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 3b",
    "finalize BSR as a new BSR",
    timingItem::kMandatory,
    startClock,
    endClock);

  return result;
}

//_______________________________________________________________________________
void displayFinalizedBsrScore (
  const S_bsrScore bScore,
  S_msrOah     msrOpts,
  S_bsrOah     bsrOpts,
  indentedOstream& logOstream)
{
  // sanity check
  msrAssert (
    bScore != 0,
    "bScore is null");

  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  logOstream <<
    separator <<
    endl <<
    gTab <<
    "Optional pass: displaying the finalized BSR as text" <<
    endl <<
    separator <<
    endl <<
    bScore <<
    separator <<
    endl <<
    endl;

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "",
    "display the finalized BSR",
    timingItem::kOptional,
    startClock,
    endClock);
}


}
