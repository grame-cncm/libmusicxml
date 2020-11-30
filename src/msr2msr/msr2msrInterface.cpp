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

#include "msr2msrOah.h"

#include "msr2msrInterface.h"

#include "msr2msrTranslator.h"

#include "messagesHandling.h"


using namespace std;

namespace MusicXML2
{
//_______________________________________________________________________________
S_msrScore convertMsrScoreToMsrScore (
  S_msrScore mScore,
  S_msrOahGroup   msrOpts,
  S_lpsrOahGroup  lpsrOpts,
  string     passNumber)
{
  // sanity check
  msgAssert (
    mScore != 0,
    "mScore is null");

  S_msrScore lpScore;

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
      passNumber << ": translating the MSR into another MSR" <<
      endl <<
      separator <<
      endl;
  }
#endif

  // create an msr2msrTranslator
  msr2msrTranslator
    translator (
      mScore);

  // build the MSR score
  translator.buildMsrScoreFromMsrScore ();

  clock_t endClock = clock ();

  // register time spent
  timing::gGlobalTiming.appendTimingItem (
    passNumber,
    "translate MSR to MSR",
    timingItem::kMandatory,
    startClock,
    endClock);

  // get the MSR score
  S_msrScore
    resultingMsrScore =
      translator.getResultingMsrScore ();

  if (gIndenter != 0) {
    if (! gGlobalGeneralOahGroup->getQuiet ()) {
      stringstream s;

      s <<
        "gIndenter value after pass 3: " <<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gGlobalOahOahGroup->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }

  // check indentation
  if (gIndenter != 0) {
    gLogStream <<
      "### convertMsrScoreToMsrScore gIndenter final value: " <<
      gIndenter.getIndent () <<
      " ###" <<
      endl << endl;

    gIndenter.resetToZero ();
  }

  return lpScore;
}

//_______________________________________________________________________________
void displayMsrScore (
  const S_msrScore lpScore,
  S_msrOahGroup          msrOpts,
  S_lpsrOahGroup         lpsrOpts)
{
  // sanity check
  msgAssert (
    lpScore != 0,
    "lpScore is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    separator <<
    endl <<
    gTab <<
    "Optional pass: displaying the MSR as text" <<
    endl <<
    separator <<
    endl <<
    endl <<
    lpScore <<
    separator <<
    endl <<
    endl;

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    "",
    "display the MSR as text",
    timingItem::kOptional,
    startClock,
    endClock);
}

void displayMsrScoreShort (
  const S_msrScore lpScore,
  S_msrOahGroup          msrOpts,
  S_lpsrOahGroup         lpsrOpts)
{
  // sanity check
  msgAssert (
    lpScore != 0,
    "lpScore is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    separator <<
    endl <<
    gTab <<
    "Optional pass: displaying the MSR as text, short version" <<
    endl <<
    separator <<
    endl <<
    endl;

  lpScore->printShort (gLogStream);

  gLogStream <<
    separator <<
    endl <<
    endl;

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    "",
    "display the MSR as text, short version",
    timingItem::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayMsrScore_OptionalPass (
  S_msrScore lpScore,
  S_msrOahGroup    msrOpts,
  S_lpsrOahGroup   lpsrOpts)
{
  // display it
  displayMsrScore (
    lpScore,
    msrOpts,
    lpsrOpts);

  if (gIndenter != 0) {
    if (! gGlobalGeneralOahGroup->getQuiet ()) {
      stringstream s;

      s <<
        "gIndenter value after MSR score display: " <<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gGlobalOahOahGroup->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

void displayMsrScoreShort_OptionalPass (
  S_msrScore lpScore,
  S_msrOahGroup    msrOpts,
  S_lpsrOahGroup   lpsrOpts)
{
  // display it
  displayMsrScoreShort (
    lpScore,
    msrOpts,
    lpsrOpts);

  if (gIndenter != 0) {
    if (! gGlobalGeneralOahGroup->getQuiet ()) {
      stringstream s;

      s <<
        "gIndenter value after MSR score short display: " <<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gGlobalOahOahGroup->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}


}
