/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <fstream>      // ofstream, ofstream::open(), ofstream::close()

#include "messagesHandling.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "generalOah.h"

#include "utilities.h"

#include "bsr2brailleOah.h"

#include "bsr2brailleTranslator.h"

#include "bsr2brailleTranslatorInterface.h"


using namespace std;

namespace MusicXML2
{
//_______________________________________________________________________________
void convertBsrScoreToBrailleText (
  const S_bsrScore originalBsrScore,
  S_bsrOahGroup    bsrOpts,
  string           passNumber,
  ostream&         brailleOutputStream)
{
  // sanity check
  msgAssert (
    originalBsrScore != nullptr,
    "originalBsrScore is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePasses ()) {
    gLogStream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      passNumber << ": writing the BSR as Braille code" <<
      endl <<
      separator <<
      endl;
  }
#endif

  // create a bsr2brailleTranslator
  bsr2brailleTranslator
    translator (
      originalBsrScore,
      bsrOpts,
      brailleOutputStream);

  // generate the braille music code
  translator.generateBrailleCodeFromBsrScore ();

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    passNumber,
    "translate BSR to Braille music",
    timingItem::kMandatory,
    startClock,
    endClock);

  // check indentation
  if (gIndenter != 0) {
    gLogStream <<
      "### convertBsrScoreToBrailleText gIndenter final value: " <<
      gIndenter.getIndent () <<
      " ###" <<
      endl;

    gIndenter.resetToZero ();
  }
}


}
