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

#include "generalOah.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "bsr2BrailleOah.h"

#include "msr2BsrInterface.h"

#include "msr2BsrTranslator.h"

using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
S_bsrScore buildBsrScoreFromMsrScore (
  const S_msrScore mScore,
  S_msrOah         msrOpts,
  S_bsrOah         bsrOpts,
  ostream&         logOstream)
{
  // sanity check
  msgAssert (
    mScore != 0,
    "mScore is null");

  clock_t startClock = clock ();

#ifdef TRACE_OAH
  if (gGlobalTraceOah->getTracePasses ()) {
    string separator =
      "%--------------------------------------------------------------";

    logOstream <<
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
      logOstream,
      mScore);

  // build the BSR score
  translator.buildBsrScoreFromMsrScore ();
  logOstream << endl;

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
S_bsrScore convertMsrScoreToBsrScore (
  S_msrScore mScore,
  string     passNumber)
{
  S_bsrScore bScore;

  if (! gGlobalBsr2BrailleOah->fNoBrailleCode) {
    bScore =
      buildBsrScoreFromMsrScore (
        mScore,
        gGlobalMsrOah,
        gGlobalBsrOah,
        gLogOstream);
  }

  if (gIndenter != 0) {
    if (! gGlobalGeneralOah->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after pass 3a: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gGlobalOahOah->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }

  if (! bScore) {
    string message =
      "### Conversion from MSR to BSR failed ###";

    gLogOstream <<
      message <<
      endl <<
      endl;

    throw msrScoreToBsrScoreException (message);
  }

  return bScore;
}


}
