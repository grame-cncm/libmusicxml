/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iostream>

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "versions.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "bsr2brailleOah.h"

#include "msr2bsrInterface.h"

#include "msr2bsrTranslator.h"

using namespace std;

namespace MusicXML2
{
//_______________________________________________________________________________
S_bsrScore convertMsrScoreToBsrScore (
  S_msrScore mScore,
  S_msrOahGroup   msrOpts,
  S_bsrOahGroup   bsrOpts,
  string     passNumber)
{
  S_bsrScore bScore;

  if (! gGlobalBsr2brailleOahGroup->getNoBrailleCode ()) {
    // sanity check
    msgAssert (
      mScore != 0,
      "mScore is null");

    // start the clock
  clock_t startClock = clock ();

  #ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTracePasses ()) {
      string separator =
        "%--------------------------------------------------------------";

      gLogStream <<
        endl <<
        separator <<
        endl <<
        gTab <<
        passNumber << ": translating the MSR into a BSR" <<
        endl <<
        separator <<
        endl;
    }
  #endif

    // create an msr2bsrTranslator
    msr2bsrTranslator
      translator (
        gLogStream,
        mScore);

    // build the BSR score
    translator.buildBsrScoreFromMsrScore ();
    gLogStream << endl;

    clock_t endClock = clock ();

    // register time spent
    timing::gGlobalTiming.appendTimingItem (
      passNumber,
      "translate MSR to BSR",
      timingItem::kMandatory,
      startClock,
      endClock);

    // get the BSR score
    bScore =
      translator.getBsrScore ();
  }

  if (gIndenter != 0) {
    if (! gGlobalGeneralOahGroup->getQuiet ()) {
      stringstream s;

      s <<
        "gIndenter value after pass 3a: " <<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gGlobalOahOahGroup->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }

  if (! bScore) {
    string message =
      "### Conversion from MSR to BSR failed ###";

    gLogStream <<
      message <<
      endl << endl;

    throw msrScoreToBsrScoreException (message);
  }

  // check indentation
  if (gIndenter != 0) {
    gLogStream <<
      "### convertMsrScoreToBsrScore gIndenter final value: " <<
      gIndenter.getIndent () <<
      " ###" <<
      endl << endl;

    gIndenter.resetToZero ();
  }

  return bScore;
}


}
