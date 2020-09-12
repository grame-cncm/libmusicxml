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

#include "generalOah.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "msr2LpsrInterface.h"

#include "msrVoice2MasterVoiceCreator.h"

using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
S_msrVoice buildMsrMasterFromMsrVoice (
  const S_msrVoice mVoice,
  S_msrOah         msrOpts,
  ostream&         logOstream)
{
  // sanity check
  msgAssert (
    mVoice != 0,
    "mVoice is null");

  clock_t startClock = clock ();

#ifdef TRACE_OAH
  if (gGlobalTraceOah->getTracePasses ()) {
    string separator =
      "%--------------------------------------------------------------";

    logOstream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      "Pass 2c: creating a master voice from voice \"" <<
      mVoice->asShortString () <<
      "\"" <<
      endl <<
      separator <<
      endl;
  }
#endif

  // create an msrVoice2Master
  msrVoice2MasterVoiceCreator
    creator (
      logOstream,
      mVoice);

  // build the LPSR score
  creator.buildMsrMasterFromMsrVoice ();

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 2c",
    "create master voice",
    timingItem::kOptional,
    startClock,
    endClock);

  // get the master score
  S_msrVoice
    masterVoice =
      creator.getMasterVoice ();

  // return it
  return masterVoice;
}

//_______________________________________________________________________________
void displayMsrMasterVoice (
  const S_msrVoice masterVoice,
  S_msrOah         msrOpts,
  ostream&         logOstream)
{
  // sanity check
  msgAssert (
    masterVoice != 0,
    "masterVoice is null");

  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  logOstream <<
    separator <<
    endl <<
    gTab <<
    "Optional pass: displaying master voice \"" <<
    masterVoice->asShortString () <<
    "\" as text" <<
    endl <<
    separator <<
    endl <<
    endl <<
    masterVoice <<
    separator <<
    endl <<
    endl;

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "",
    "display the master voice",
    timingItem::kOptional,
    startClock,
    endClock);
}


}
