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

#include "msr2MeitreeInterface.h"

#include "msr2MeitreeTranslator.h"

using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
Sxmlelement buildMeitreeFromMsrScore (
  const S_msrScore           mScore,
  S_msrOah                   msrOpts,
  ostream&                   logOstream,
  string                     passNumber,
  timingItem::timingItemKind timingItemKind)
{
  // sanity check
  msrAssert (
    mScore != 0,
    "mScore is null");

  clock_t startClock = clock ();

#ifdef TRACE_OAH
  if (gTraceOah->getTracePasses ()) {
    string separator =
      "%--------------------------------------------------------------";

    logOstream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      "translating the MSR into an meitree" <<
      endl <<
      separator <<
      endl;
  }
#endif

  // create an msr2MeitreeTranslator
  msr2MeitreeTranslator
    translator (
      logOstream,
      mScore);

  // build the meitree score
  translator.buildMeitreeFromMsrScore ();

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    passNumber,
    "translate the MSR to an meitree",
    timingItem::kMandatory,
    startClock,
    endClock);

  // get the meitree
  Sxmlelement
    meiTree =
      translator.getScorePartWiseElement ();

  // return it
  return meiTree;
}

//_______________________________________________________________________________
void displayMeitreeScore ( // unused JMI
  const Sxmlelement mxmlTree,
  S_msrOah          msrOpts,
  ostream&          logOstream)
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
    "Optional pass: displaying the Meitree as text" <<
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
    "display the Meitree",
    timingItem::kOptional,
    startClock,
    endClock);
}


}
