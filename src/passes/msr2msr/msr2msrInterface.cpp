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

#include "mxmlTree2msrTranslatorInterface.h"

#include "messagesHandling.h"


using namespace std;

namespace MusicXML2
{
//_______________________________________________________________________________
S_msrScore convertMsrScoreToMsrScore (
  S_msrScore        originalMsrScore,
  S_msrOahGroup     msrOpts,
  S_msr2msrOahGroup msr2msrOpts,
  string            passNumber,
  string            passDescription)
{
  // sanity check
  msgAssert (
    originalMsrScore != nullptr,
    "originalMsrScore is null");

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
      originalMsrScore);

  // build the resulting MSR score
  S_msrScore
    resultingMsrScore =
      translator.buildMsrScoreFromMsrScore ();

  clock_t endClock = clock ();

  // register time spent
  timing::gGlobalTiming.appendTimingItem (
    passNumber,
    passDescription,
    timingItem::kMandatory,
    startClock,
    endClock);

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
      endl;

    gIndenter.resetToZero ();
  }

  // display the resulting MSR score if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplayMsr ()) {
    displayMsrScore_OptionalPass (
      resultingMsrScore);
  }

  else if (gGlobalMsrOahGroup->getDisplayMsrShort ()) {
    displayMsrScoreShort_OptionalPass (
      resultingMsrScore,
      gGlobalMsrOahGroup);
  }

  return resultingMsrScore;
}

//_______________________________________________________________________________
void displayMsrScore (
  const S_msrScore  originalMsrScore,
  S_msrOahGroup     msrOpts,
  S_msr2msrOahGroup msr2msrOpts)
{
  // sanity check
  msgAssert (
    originalMsrScore != nullptr,
    "originalMsrScore is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    separator <<
    endl <<
    gTab <<
    "Optional pass: displaying the MSR as text in msr2msr" <<
    endl <<
    separator <<
    endl << endl <<
    originalMsrScore <<
    separator <<
    endl << endl;

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    "",
    "Display the MSR as text",
    timingItem::kOptional,
    startClock,
    endClock);
}

void displayMsrScoreShort (
  const S_msrScore  originalMsrScore,
  S_msrOahGroup     msrOpts,
  S_msr2msrOahGroup msr2msrOpts)
{
  // sanity check
  msgAssert (
    originalMsrScore != nullptr,
    "originalMsrScore is null");

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
    endl << endl;

  originalMsrScore->printShort (gLogStream);

  gLogStream <<
    separator <<
    endl << endl;

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    "",
    "Display the MSR as text, short version",
    timingItem::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayMsrScore_OptionalPass (
  const S_msrScore  originalMsrScore,
  S_msrOahGroup     msrOpts,
  S_msr2msrOahGroup msr2msrOpts)
{
  // display it
  displayMsrScore (
    originalMsrScore,
    msrOpts,
    msr2msrOpts);

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
  S_msrScore       originalMsrScore,
  S_msrOahGroup    msrOpts,
  S_msr2msrOahGroup msr2msrOpts)
{
  // display it
  displayMsrScoreShort (
    originalMsrScore,
    msrOpts,
    msr2msrOpts);

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
