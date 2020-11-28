/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "utilities.h"
#include "messagesHandling.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "generalOah.h"
#include "oahOah.h"

#include "bsr2brailleOah.h"

#include "bsr2bsrFinalizer.h"

#include "bsr2bsrFinalizerInterface.h"


using namespace std;

namespace MusicXML2
{
//_______________________________________________________________________________
S_bsrScore convertBsrFirstScoreToFinalizedBsrScore (
  const S_bsrScore bScore,
  S_bsrOahGroup         bsrOpts,
  string           passNumber)
{
  S_bsrScore finalizedBsrScore;

  if (! gGlobalBsr2brailleOahGroup->getNoBrailleCode ()) {
    // sanity check
    msgAssert (
      bScore != 0,
      "bScore is null");

    S_bsrScore result;

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
        passNumber << ": finalizing the BSR as a new BSR" <<
        endl <<
        separator <<
        endl;
    }
#endif

    // create an bsr2bsrFinalizer
    bsr2bsrFinalizer
      translator (
        bsrOpts,
        bScore);

    // build the finalized BSR score
    translator.generateFinalizedBsrScoreFromBsrScore ();
    gLogStream << endl;

    // get the result
    result = translator.getBsrScore ();

    // register time spent
    clock_t endClock = clock ();

    timing::gGlobalTiming.appendTimingItem (
      passNumber,
      "finalize BSR as a new BSR",
      timingItem::kMandatory,
      startClock,
      endClock);

    return result;
  }

  if (gIndenter != 0) {
    if (! gGlobalGeneralOahGroup->getQuiet ()) {
      stringstream s;

      s <<
        "gIndenter value after " << passNumber << ": " <<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gGlobalOahOahGroup->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }

  if (! finalizedBsrScore) {
    string message =
      "### Conversion from first BSR to finalized BSR failed ###";

    gLogStream <<
      message <<
      endl << endl;

    throw bsrScoreToFinalizedBsrScoreException (message);
  }

  // check indentation
  if (gIndenter != 0) {
    gLogStream <<
      "### convertBsrFirstScoreToFinalizedBsrScore gIndenter final value: " <<
      gIndenter.getIndent () <<
      " ###" <<
      endl << endl;

    gIndenter.resetToZero ();
  }

  return finalizedBsrScore;
}

//_______________________________________________________________________________
void displayFirstBsrScore (
  const S_bsrScore bScore,
  S_msrOahGroup         msrOpts,
  S_bsrOahGroup         bsrOpts)
{
  // sanity check
  msgAssert (
    bScore != 0,
    "bScore is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    separator <<
    endl <<
    gTab <<
    "Optional pass: displaying the first BSR as text" <<
    endl <<
    separator <<
    endl <<
    endl <<
    bScore <<
    separator <<
    endl <<
    endl;

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    "",
    "display the first BSR as text",
    timingItem::kOptional,
    startClock,
    endClock);
}

void displayFirstBsrScoreShort (
  const S_bsrScore bScore,
  S_msrOahGroup         msrOpts,
  S_bsrOahGroup         bsrOpts)
{
  // sanity check
  msgAssert (
    bScore != 0,
    "bScore is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    separator <<
    endl <<
    gTab <<
    "Optional pass: displaying the first BSR as text, short version" <<
    endl <<
    separator <<
    endl <<
    endl;

  bScore->printShort (gLogStream);

  gLogStream <<
    separator <<
    endl <<
    endl;

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    "",
    "display the first BSR as text, short version",
    timingItem::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayBsrFirstScore_OptionalPass (
  S_bsrScore bScore,
  S_msrOahGroup   msrOpts,
  S_bsrOahGroup   bsrOpts)
{
  // display it
  displayFirstBsrScore (
    bScore,
    msrOpts,
    bsrOpts);

  if (gIndenter != 0) {
    if (! gGlobalGeneralOahGroup->getQuiet ()) {
      stringstream s;

      s <<
        "gIndenter value after first BSR score display: " <<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gGlobalOahOahGroup->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

void displayBsrFirstScoreShort_OptionalPass (
  S_bsrScore bScore,
  S_msrOahGroup   msrOpts,
  S_bsrOahGroup   bsrOpts)
{
  // display it
  displayFirstBsrScoreShort (
    bScore,
    msrOpts,
    bsrOpts);

  if (gIndenter != 0) {
    if (! gGlobalGeneralOahGroup->getQuiet ()) {
      stringstream s;

      s <<
        "gIndenter value after first BSR score short display: " <<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gGlobalOahOahGroup->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

//_______________________________________________________________________________
void displayFinalizedBsrScore (
  const S_bsrScore bScore,
  S_msrOahGroup         msrOpts,
  S_bsrOahGroup         bsrOpts)
{
  // sanity check
  msgAssert (
    bScore != 0,
    "bScore is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
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

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    "",
    "display the finalized BSR as text",
    timingItem::kOptional,
    startClock,
    endClock);
}

void displayFinalizedBsrScoreShort (
  const S_bsrScore bScore,
  S_msrOahGroup         msrOpts,
  S_bsrOahGroup         bsrOpts)
{
  // sanity check
  msgAssert (
    bScore != 0,
    "bScore is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    separator <<
    endl <<
    gTab <<
    "Optional pass: displaying the finalized BSR as text, short version" <<
    endl <<
    separator <<
    endl;

  bScore->printShort (gLogStream);

  gLogStream <<
    separator <<
    endl <<
    endl;

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    "",
    "display the finalized BSR as text, short version",
    timingItem::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayFinalizedBsrScore_OptionalPass (
  S_bsrScore bScore,
  S_msrOahGroup   msrOpts,
  S_bsrOahGroup   bsrOpts)
{
  // display it
  displayFinalizedBsrScore (
    bScore,
    msrOpts,
    bsrOpts);

  if (gIndenter != 0) {
    if (! gGlobalGeneralOahGroup->getQuiet ()) {
      stringstream s;

      s <<
        "gIndenter value after BSR score display: " <<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gGlobalOahOahGroup->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

void displayFinalizedBsrScoreShort_OptionalPass (
  S_bsrScore bScore,
  S_msrOahGroup   msrOpts,
  S_bsrOahGroup   bsrOpts)
{
  // display it
  displayFinalizedBsrScoreShort (
    bScore,
    msrOpts,
    bsrOpts);

  if (gIndenter != 0) {
    if (! gGlobalGeneralOahGroup->getQuiet ()) {
      stringstream s;

      s <<
        "gIndenter value after BSR score short display: " <<
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
