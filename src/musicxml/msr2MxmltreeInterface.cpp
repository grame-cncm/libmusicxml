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

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "mxmlTree.h"

#include "msr2MxmltreeInterface.h"

#include "msr2MxmltreeTranslator.h"

using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
Sxmlelement buildMxmltreeFromMsrScore (
  const S_msrScore mScore,
  S_msrOah         msrOpts,
  indentedOstream& logOstream)
{
  // sanity check
  msrAssert (
    mScore != 0,
    "mScore is null");

  clock_t startClock = clock ();

#ifdef TRACE_OAH
  if (gTraceOah->fTracePasses) {
    string separator =
      "%--------------------------------------------------------------";

    logOstream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      "Optional pass: translating the MSR into an mxmltree" <<
      endl <<
      separator <<
      endl;
  }
#endif

  // create an msr2MxmltreeTranslator
  msr2MxmltreeTranslator
    translator (
      logOstream,
      mScore);

  // build the mxmltree score
  translator.buildMxmltreeFromMsrScore ();

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Optional pass",
    "translate the MSR to an mxmltree",
    timingItem::kMandatory,
    startClock,
    endClock);

  // get the mxmltree
  Sxmlelement
    mxmlTree =
      translator.getScorePartWiseElement ();

  // return it
  return mxmlTree;
}

//_______________________________________________________________________________
void displayMxmltreeScore ( // unused JMI
  const Sxmlelement mxmlTree,
  S_msrOah          msrOpts,
  indentedOstream&  logOstream)
{
  // sanity check
  msrAssert (
    mxmlTree != 0,
    "mxmlTree is null");

  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  logOstream <<
    separator <<
    endl <<
    gTab <<
    "Optional pass: displaying the Mxmltree as text" <<
    endl <<
    separator <<
    endl <<
    endl <<
// JMI    mxmlTree <<
    separator <<
    endl <<
    endl;

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "",
    "display the Mxmltree",
    timingItem::kOptional,
    startClock,
    endClock);
}


}
