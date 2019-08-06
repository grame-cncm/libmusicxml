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

#include "lpsr2LilypondTranslator.h"

#include "lpsr2LilypondInterface.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
void generateLilypondCodeFromLpsrScore (
  const S_lpsrScore lpScore,
  S_msrOah      msrOpts,
  S_lpsrOah     lpsrOpts,
  indentedOstream&  logOstream,
  indentedOstream&  lilypondCodeOstream)
{
  // sanity check
  msrAssert (
    lpScore != 0,
    "lpScore is null");

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
      "Pass 4: writing the LPSR as LilyPond code" <<
      endl <<
      separator <<
      endl;
  }
#endif

  // create an lpsr2LilypondTranslator
  lpsr2LilypondTranslator
    translator (
      lpScore,
      msrOpts,
      lpsrOpts,
      logOstream,
      lilypondCodeOstream);

  // build the LilyPond score
  translator.generateLilypondCodeFromLpsrScore ();

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 4",
    "translate LPSR to LilyPond",
    timingItem::kMandatory,
    startClock,
    endClock);
}


}
