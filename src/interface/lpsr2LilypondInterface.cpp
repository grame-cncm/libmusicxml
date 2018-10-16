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

#include "messagesHandling.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif


#include "lpsr2LilypondTranslator.h"

#include "lpsr2LilypondInterface.h"


using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
void generateLilypondCodeFromLpsrScore (
  const S_lpsrScore lpScore,
  S_msrOptions      msrOpts,
  S_lpsrOptions     lpsrOpts,
  indentedOstream&  logIOstream,
  indentedOstream&  lilypondCodeIOstream)
{
  // sanity check
  msrAssert (
    lpScore != 0,
    "lpScore is null");
    
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePasses) {
    logIOstream <<
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
      msrOpts,
      lpsrOpts,
      logIOstream,
      lilypondCodeIOstream,
      lpScore);
  
  // build the LPSR score    
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
