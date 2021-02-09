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

#include "msdl2msdrTokensListTranslator.h"

#include "msdrTokensList2msrTranslatorInterface.h"

#include "msdrTokensList2msrTranslator.h"

/* JMI
#include "msr2summaryVisitor.h"
#include "msr2namesVisitor.h"
*/

using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
void createMsrFromMsdrTokensList (
  S_msdrTokensList tokensList,
  string           passNumber,
  string           passDescription)
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    tokensList != nullptr,
    "createMsrFromMsdrTokensList(): tokensList is null");

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
      passNumber << ": create the MSR from the MSDL tokens list" <<
      endl;

    gLogStream <<
      separator <<
      endl;
  }
#endif

/* JMI
  // create an msdrTokensList2msrTranslator
  msdl2msdrTokensListTranslator
    translator ();

  // browse the mxmlTree
  translator.buildMsdrTokensList ();
*/

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

/* JMI
  S_msdrTokensList tokensList = translator.getMsdrTokensList ();


  // display the MSR score if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplayMsr ()) {
    displayPopulatedMsrScore_OptionalPass (
      tokensList);
  }

  else if (gGlobalMsrOahGroup->getDisplayMsrShort ()) {
    displayPopulatedMsrScoreShort_OptionalPass (
      tokensList);
  }

  // display the populated MSR score summary if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplayMsrSummary ()) {
    // display the score summary
    displayPopulatedMsrScoreSummary (
      tokensList,
      gGlobalMsrOahGroup);
  }

  // display the populated MSR score names if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplayMsrNames ()) {
    // display the score name
    displayPopulatedMsrScoreNames (
      tokensList,
      gGlobalMsrOahGroup);
  }
  */
}

//_______________________________________________________________________________
void displayCreatedMsrScore_OptionalPass (
  S_msdrTokensList tokensList)
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    tokensList != nullptr,
    "tokensList is null");

  // start the clock
  clock_t startClock =  clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    endl <<
    separator <<
    endl <<
    gTab <<
    "Optional pass: displaying the created MSR as text" <<
    endl <<
    separator <<
    endl << endl <<
    tokensList;

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


}
