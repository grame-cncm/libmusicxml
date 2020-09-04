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

#include "generalOah.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "bsr2BrailleOah.h"

#include "bsr2BsrFinalizer.h"

#include "bsr2BsrFinalizerInterface.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
void displayFirstBsrScore (
  const S_bsrScore bScore,
  S_msrOah         msrOpts,
  S_bsrOah         bsrOpts,
  ostream&         logOstream)
{
  // sanity check
  msrAssert (
    bScore != 0,
    "bScore is null");

  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  logOstream <<
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

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "",
    "display the first BSR as text",
    timingItem::kOptional,
    startClock,
    endClock);
}

void displayFirstBsrScoreShort (
  const S_bsrScore bScore,
  S_msrOah         msrOpts,
  S_bsrOah         bsrOpts,
  ostream&         logOstream)
{
  // sanity check
  msrAssert (
    bScore != 0,
    "bScore is null");

  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  logOstream <<
    separator <<
    endl <<
    gTab <<
    "Optional pass: displaying the first BSR as text, short version" <<
    endl <<
    separator <<
    endl <<
    endl;

  bScore->printShort (logOstream);

  logOstream <<
    separator <<
    endl <<
    endl;

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "",
    "display the first BSR as text, short version",
    timingItem::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayBsrFirstScore_OptionalPass (
  S_bsrScore bScore,
  S_msrOah   msrOpts,
  S_bsrOah   bsrOpts)
{
  // display it
  displayFirstBsrScore (
    bScore,
    msrOpts,
    bsrOpts,
    gLogOstream);

  if (gIndenter != 0) {
    if (! gGeneralOah->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after first BSR score display: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gOahOah->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

void displayBsrFirstScoreShort_OptionalPass (
  S_bsrScore bScore,
  S_msrOah   msrOpts,
  S_bsrOah   bsrOpts)
{
  // display it
  displayFirstBsrScoreShort (
    bScore,
    msrOpts,
    bsrOpts,
    gLogOstream);

  if (gIndenter != 0) {
    if (! gGeneralOah->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after first BSR score short display: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gOahOah->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

//_______________________________________________________________________________
void displayFinalizedBsrScore_OptionalPass (
  S_bsrScore bScore,
  S_msrOah   msrOpts,
  S_bsrOah   bsrOpts)
{
  // display it
  displayFinalizedBsrScore (
    bScore,
    msrOpts,
    bsrOpts,
    gLogOstream);

  if (gIndenter != 0) {
    if (! gGeneralOah->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after BSR score display: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gOahOah->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

void displayFinalizedBsrScoreShort_OptionalPass (
  S_bsrScore bScore,
  S_msrOah   msrOpts,
  S_bsrOah   bsrOpts)
{
  // display it
  displayFinalizedBsrScoreShort (
    bScore,
    msrOpts,
    bsrOpts,
    gLogOstream);

  if (gIndenter != 0) {
    if (! gGeneralOah->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after BSR score short display: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gOahOah->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

//_______________________________________________________________________________
S_bsrScore generateFinalizedBsrScoreFromBsrScore (
  const S_bsrScore bScore,
  S_bsrOah         bsrOpts,
  ostream&         logOstream,
  string           passNumber)
{
  // sanity check
  msrAssert (
    bScore != 0,
    "bScore is null");

  S_bsrScore result;

  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

#ifdef TRACE_OAH
  if (gTraceOah->getTracePasses ()) {
    logOstream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      "Pass 3b: finalizing the BSR as a new BSR" <<
      endl <<
      separator <<
      endl;
  }
#endif

  // create an bsr2BsrFinalizer
  bsr2BsrFinalizer
    translator (
      bsrOpts,
      logOstream,
      bScore);

  // build the finalized BSR score
  translator.generateFinalizedBsrScoreFromBsrScore ();
  logOstream << endl;

  // get the result
  result = translator.getBsrScore ();

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 3b",
    "finalize BSR as a new BSR",
    timingItem::kMandatory,
    startClock,
    endClock);

  return result;
}

//_______________________________________________________________________________
void displayFinalizedBsrScore (
  const S_bsrScore bScore,
  S_msrOah         msrOpts,
  S_bsrOah         bsrOpts,
  ostream&         logOstream)
{
  // sanity check
  msrAssert (
    bScore != 0,
    "bScore is null");

  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  logOstream <<
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

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "",
    "display the finalized BSR as text",
    timingItem::kOptional,
    startClock,
    endClock);
}

void displayFinalizedBsrScoreShort (
  const S_bsrScore bScore,
  S_msrOah         msrOpts,
  S_bsrOah         bsrOpts,
  ostream&         logOstream)
{
  // sanity check
  msrAssert (
    bScore != 0,
    "bScore is null");

  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  logOstream <<
    separator <<
    endl <<
    gTab <<
    "Optional pass: displaying the finalized BSR as text, short version" <<
    endl <<
    separator <<
    endl;

  bScore->printShort (logOstream);

  logOstream <<
    separator <<
    endl <<
    endl;

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "",
    "display the finalized BSR as text, short version",
    timingItem::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
S_bsrScore convertBsrScoreToFinalizedBsrScore (
  S_bsrScore bScore,
  string     passNumber)
{
  S_bsrScore finalizedBsrScore;

  if (! gBsr2BrailleOah->fNoBrailleCode) {
    finalizedBsrScore =
      generateFinalizedBsrScoreFromBsrScore (
        bScore,
        gBsrOah,
        gLogOstream,
        passNumber);
  }

  if (gIndenter != 0) {
    if (! gGeneralOah->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after pass 3b: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gOahOah->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }

  if (! finalizedBsrScore) {
    string message =
      "### Conversion from first BSR to finalized BSR failed ###";

    gLogOstream <<
      message <<
      endl <<
      endl;

    throw bsrScoreToFinalizedBsrScoreException (message);
  }

  return finalizedBsrScore;
}


}
