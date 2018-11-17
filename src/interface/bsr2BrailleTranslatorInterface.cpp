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


#include "bsr2BrailleTranslator.h"

#include "bsr2BrailleTranslatorInterface.h"


using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
void generateBrailleCodeFromBsrScore (
  const S_bsrScore bScore,
  S_bsrOptions     bsrOpts,
  indentedOstream& logIOstream,
  ostream&         brailleCodeIOstream)
{
  // sanity check
  msrAssert (
    bScore != 0,
    "bScore is null");
    
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
      "Pass 4: writing the BSR as Braille code" <<
      endl <<
      separator <<
      endl;
  }
#endif

  // create an bsr2BrailleTranslator
  bsr2BrailleTranslator
    translator (
      bsrOpts,
      logIOstream,
      brailleCodeIOstream,
      bScore);
  
  // build the Braille score    
  translator.generateBrailleCodeFromBsrScore ();

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 4",
    "translate BSR to Braille",
    timingItem::kMandatory,
    startClock,
    endClock);
}


}
