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
#include <fstream>      // ofstream, ofstream::open(), ofstream::close()

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "versions.h"

#include "generalOah.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "mxmlTreeOah.h"

#include "mxmlTree.h"

#include "msr2mxmlTreeInterface.h"

#include "msr2mxmlTreeTranslator.h"

using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
Sxmlelement convertMsrScoreToMxmltree (
  const S_msrScore           theMsrScore,
  S_msrOahGroup              msrOpts,
  string                     passNumber,
  string                     passDescription,
  timingItem::timingItemKind timingItemKind)
{
  // sanity check
  msgAssert (
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
      passNumber + ": translating the MSR into an mxmlTree" <<
      endl <<
      separator <<
      endl;
  }
#endif

  // create an msr2mxmlTreeTranslator
  msr2mxmlTreeTranslator
    translator (
      theMsrScore);

  // build the mxmlTree
  Sxmlelement
    resultingMxmlTree =
      translator.buildMxmltreeFromMsrScore ();

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    passNumber,
    passDescription,
    timingItem::kMandatory,
    startClock,
    endClock);

  if (gGlobalMxmlTreeOahGroup->getTraceMusicXMLTree ()) {
    gLogStream <<
      endl <<
      "<!-- ----------------------------------------------------------- -->" <<
      endl <<
      "convertMsrScoreToMxmltree(), resultingMxmlTree contains:" <<
      endl << endl;

    ++gIndenter;

    printMxmlxTree (resultingMxmlTree, gLogStream);

    gLogStream <<
      endl;

    --gIndenter;

    gLogStream <<
      "<!-- ----------------------------------------------------------- -->" <<
      endl << endl;
  }

  // return it
  return resultingMxmlTree;
}


}
