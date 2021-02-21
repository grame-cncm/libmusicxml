/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

/*
#include <iostream>

#include "versions.h"
*/

#include "waeMessagesHandling.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "msdl2msdrTokensListTranslatorInterface.h"

#include "msdl2msdrTokensListTranslator.h"

//#include "msr2summaryVisitor.h"

using namespace std;

namespace MusicXML2
{
//_______________________________________________________________________________
S_msdrTokensList convertMsdlToMsdlTokensList (
  S_msrOahGroup& msrOpts,
  string         passNumber,
  string         passDescription)
{
/*
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    mxmlTree != nullptr,
    "convertMsdlToMsdlTokensList(): mxmlTree is null");
*/

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
      passNumber << ": translating the mxmlElement tree into an MSDL tokens list" <<
      endl;

    gLogStream <<
      separator <<
      endl;
  }
#endif

  S_msdrTokensList tokensList;

  // create an msdl2msdrTokensListTranslator
  msdl2msdrTokensListTranslator translator;

  // build the MSDL tokens list
  translator.buildMsdlTokensList ();

  // fetch the MSDL tokens list
  tokensList =
    translator.getMsdlTokensList ();

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


  if (! tokensList) {
    string message =
      "### Conversion from mxmlTree to an MSDL tokens list failed ###";

    gLogStream <<
      message <<
      endl;

    throw msgMsrScoreToBsrScoreException (message);
  }

  if (msrOpts->getDisplayMsrSkeleton ()) {
    // display the MSDL tokens list
    displayMsdlTokensList (
      msrOpts,
      tokensList,
      passNumber,
      passDescription);
  }

  return tokensList;
}

//_______________________________________________________________________________
void displayMsdlTokensList (
  S_msrOahGroup&   msrOpts,
  S_msdrTokensList theMsdlTokensList,
  string           passNumber,
  string           passDescription)
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    theMsdlTokensList != nullptr,
    "theMsdlTokensList is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    endl <<
    separator <<
    endl <<
    gTab <<
    "Optional pass: displaying the MSDL tokens list as text" <<
    endl <<
    separator <<
    endl << endl <<
    theMsdlTokensList;

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    "",
    "Display the MSDL tokens list as text",
    timingItem::kOptional,
    startClock,
    endClock);
}


}
