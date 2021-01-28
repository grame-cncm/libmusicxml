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

#include "mxmlTree2msrTranslatorInterface.h"

#include "mxmlTree2msrTranslator.h"

#include "msr2summaryVisitor.h"
#include "msr2namesVisitor.h"

using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
void populateMsrSkeletonFromMxmlTree (
  Sxmlelement mxmlTree,
  S_msrScore  scoreSkeletonToBePopulated,
  string      passNumber,
  string      passDescription)
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    mxmlTree != nullptr,
    "populateMsrSkeletonFromMxmlTree(): mxmlTree is null");

  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    scoreSkeletonToBePopulated != nullptr,
    "scoreSkeletonToBePopulated is null");

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
      passNumber << ": translating the mxmlElement tree into a MSR" <<
      endl;

    gLogStream <<
      separator <<
      endl;
  }
#endif

  // create an mxmlTree2msrTranslator
  mxmlTree2msrTranslator
    translator (
      scoreSkeletonToBePopulated);

  // browse the mxmlTree
  translator.browseMxmlTree (
    mxmlTree);

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    passNumber,
    passDescription,
    timingItem::kMandatory,
    startClock,
    endClock);

  // check indentation
  if (gIndenter != 0) {
    gLogStream <<
      "### populateMsrSkeletonFromMxmlTree gIndenter final value: " <<
      gIndenter.getIndent () <<
      " ###" <<
      endl;

    gIndenter.resetToZero ();
  }

  // display the populated MSR score if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplayMsr ()) {
    displayPopulatedMsrScore_OptionalPass (
      scoreSkeletonToBePopulated);
  }

  else if (gGlobalMsrOahGroup->getDisplayMsrShort ()) {
    displayPopulatedMsrScoreShort_OptionalPass (
      scoreSkeletonToBePopulated);
  }

  // display the populated MSR score summary if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplayMsrSummary ()) {
    // display the score summary
    displayPopulatedMsrScoreSummary (
      scoreSkeletonToBePopulated,
      gGlobalMsrOahGroup);
  }

  // display the populated MSR score names if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplayMsrNames ()) {
    // display the score name
    displayPopulatedMsrScoreNames (
      scoreSkeletonToBePopulated,
      gGlobalMsrOahGroup);
  }
}

//_______________________________________________________________________________
void displayPopulatedMsrScore_OptionalPass (
  S_msrScore theMsrScore)
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    theMsrScore != nullptr,
    "theMsrScore is null");

  // start the clock
  clock_t startClock =  clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    endl <<
    separator <<
    endl <<
    gTab <<
    "Optional pass: displaying the populated MSR as text" <<
    endl <<
    separator <<
    endl << endl <<
    theMsrScore;

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    "",
    "Display the MSR as text",
    timingItem::kOptional,
    startClock,
    endClock);

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

//_______________________________________________________________________________
void displayPopulatedMsrScoreShort_OptionalPass (
  S_msrScore theMsrScore)
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    theMsrScore != nullptr,
    "theMsrScore is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    endl <<
    separator <<
    endl <<
    gTab <<
    "Optional pass: displaying the populated MSR as text, short version" <<
    endl <<
    separator <<
    endl << endl;

  theMsrScore->printShort (gLogStream);

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    "",
    "Display the MSR as short text",
    timingItem::kOptional,
    startClock,
    endClock);

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

//_______________________________________________________________________________
void displayPopulatedMsrScoreSummary (
  S_msrScore     theMsrScore,
  S_msrOahGroup& msrOpts)
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    theMsrScore != nullptr,
    "theMsrScore is null");

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
      "Optional pass: outputting a summary of the populated MSR" <<
      endl <<
      separator <<
      endl << endl;
  }
#endif

  // create an msr2summaryVisitor visitor
  msr2summaryVisitor
    summaryVisitor (
      msrOpts);

  summaryVisitor.printSummaryFromMsrScore (
    theMsrScore);

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    "",
    "display MSR summary",
    timingItem::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayPopulatedMsrScoreNames (
  S_msrScore     theMsrScore,
  S_msrOahGroup& msrOpts)
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    theMsrScore != nullptr,
    "theMsrScore is null");

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
      "Optional pass: outputting the names in the populated MSR" <<
      endl <<
      separator <<
      endl << endl;
  }
#endif

  // create an msr2namesVisitor visitor
  msr2namesVisitor
    namesVisitor (
      msrOpts);

  namesVisitor.printNamesFromMsrScore (
    theMsrScore);

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    "",
    "display MSR names",
    timingItem::kOptional,
    startClock,
    endClock);
}


}
