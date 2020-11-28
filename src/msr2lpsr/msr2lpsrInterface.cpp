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

#include "lpsr2lilypondOah.h"

#include "msr2lpsrInterface.h"

#include "msr2lpsrTranslator.h"

#include "messagesHandling.h"


using namespace std;

namespace MusicXML2
{
//_______________________________________________________________________________
S_lpsrScore convertMsrScoreToLpsrScore (
  S_msrScore mScore,
  S_msrOahGroup   msrOpts,
  S_lpsrOahGroup  lpsrOpts,
  string     passNumber)
{
  S_lpsrScore lpScore;

  if (gGlobalLpsr2lilypondOahGroup->getNoLilypondCode ()) {
    gLogStream <<
      "Option '-nolpc, -no-lilypond-code' is set, no LPSR is created" <<
      endl << endl;
  }
  else {
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
        passNumber << ": translating the MSR into a LPSR" <<
        endl <<
        separator <<
        endl;
    }
  #endif

    // create an msr2lpsrTranslator
    msr2lpsrTranslator
      translator (
        mScore);

    // build the LPSR score
    translator.buildLpsrScoreFromMsrScore ();

    clock_t endClock = clock ();

    // register time spent
    timing::gGlobalTiming.appendTimingItem (
      passNumber,
      "translate MSR to LPSR",
      timingItem::kMandatory,
      startClock,
      endClock);

    // get the LPSR score
    lpScore =
      translator.getLpsrScore ();
  }

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

  if (! lpScore && ! gGlobalLpsr2lilypondOahGroup->getNoLilypondCode ()) {
    string message =
      "### Conversion from MSR to LPSR failed ###";

    gLogStream <<
      message <<
      endl << endl;

    throw msrScoreToLpsrScoreException (message);
  }

  // check indentation
  if (gIndenter != 0) {
    gLogStream <<
      "### convertMsrScoreToLpsrScore gIndenter final value: " <<
      gIndenter.getIndent () <<
      " ###" <<
      endl << endl;

    gIndenter.resetToZero ();
  }

  return lpScore;
}

//_______________________________________________________________________________
void displayLpsrScore (
  const S_lpsrScore lpScore,
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
    "Optional pass: displaying the LPSR as text" <<
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
    "display the LPSR as text",
    timingItem::kOptional,
    startClock,
    endClock);
}

void displayLpsrScoreShort (
  const S_lpsrScore lpScore,
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
    "Optional pass: displaying the LPSR as text, short version" <<
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
    "display the LPSR as text, short version",
    timingItem::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayLpsrScore_OptionalPass (
  S_lpsrScore lpScore,
  S_msrOahGroup    msrOpts,
  S_lpsrOahGroup   lpsrOpts)
{
  // display it
  displayLpsrScore (
    lpScore,
    msrOpts,
    lpsrOpts);

  if (gIndenter != 0) {
    if (! gGlobalGeneralOahGroup->getQuiet ()) {
      stringstream s;

      s <<
        "gIndenter value after LPSR score display: " <<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gGlobalOahOahGroup->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

void displayLpsrScoreShort_OptionalPass (
  S_lpsrScore lpScore,
  S_msrOahGroup    msrOpts,
  S_lpsrOahGroup   lpsrOpts)
{
  // display it
  displayLpsrScoreShort (
    lpScore,
    msrOpts,
    lpsrOpts);

  if (gIndenter != 0) {
    if (! gGlobalGeneralOahGroup->getQuiet ()) {
      stringstream s;

      s <<
        "gIndenter value after LPSR score short display: " <<
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
