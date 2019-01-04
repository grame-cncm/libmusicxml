/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <iostream>

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "versions.h"

#include "messagesHandling.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif


#include "mxmlTree2MsrSkeletonBuilderInterface.h"

#include "mxmlTree2MsrSkeletonBuilder.h"

#include "msr2Summary.h"

using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
S_msrScore buildMsrSkeletonFromElementsTree (
  S_msrOptions&    msrOpts,
  Sxmlelement      mxmlTree,
  indentedOstream& logIOstream)
{
  // sanity check
  msrAssert (
    mxmlTree != 0,
    "mxmlTree is null");
    
  clock_t startClock = clock ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePasses) {
    string separator =
      "%--------------------------------------------------------------";
  
    logIOstream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      "Pass 2a: translating the xmlelement tree into an MSR skeleton" <<
      endl;
    
    logIOstream <<
      separator <<
      endl;
  }
#endif
  
  // create an mxmlTree2MsrSkeletonBuilder
  mxmlTree2MsrSkeletonBuilder
    skeletonBuilder (
      logIOstream);

  // build the MSR score
  skeletonBuilder.browseMxmlTree (
    mxmlTree);

  // fetch the score skeleton
  S_msrScore
    scoreSkeleton =
      skeletonBuilder.getMsrScore ();

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 2a",
    "build the MSR skeleton",
    timingItem::kMandatory,
    startClock,
    endClock);

    
  if (msrOpts->fDisplayMsr) {
    // display the MSR skeleton
    displayMsrSkeleton (
      msrOpts,
      scoreSkeleton,
      logIOstream);
  }

  return scoreSkeleton;
}

//_______________________________________________________________________________
void displayMsrSkeleton (
  S_msrOptions&    msrOpts,
  S_msrScore       mScore,
  indentedOstream& logIOstream)
{
  // sanity check
  msrAssert (
    mScore != 0,
    "mScore is null");
    
  clock_t startClock = clock ();
  
  string separator =
    "%--------------------------------------------------------------";
  
  logIOstream <<
    endl <<
    separator <<
    endl <<
    gTab <<
    "Optional pass: displaying the MSR skeleton as text" <<
    endl <<
    separator <<
    endl <<
    endl <<
    mScore;

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "",
    "display the MSR skeleton",
    timingItem::kOptional,
    startClock,
    endClock);
}

/* JMI
//_______________________________________________________________________________
void displayMsrSkeletonSummary (
  S_msrOptions&    msrOpts,
  S_msrScore       mScore,
  indentedOstream& logIOstream)
{
  // sanity check
  msrAssert (
    mScore != 0,
    "mScore is null");
    
  clock_t startClock = clock ();
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePasses) {
    string separator =
      "%--------------------------------------------------------------";
    
    logIOstream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      "Optional pass: outputting a summary of the MSR" <<
      endl <<
      separator <<
      endl;
  }
#endif
   
  // create an msr2SummaryVisitor visitor
  msr2SummaryVisitor
    summaryVisitor (
      msrOpts,
      logIOstream);

  summaryVisitor.printSummaryFromMsrScore (
    mScore);
  
  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "",
    "display MSR skeleton summary",
    timingItem::kOptional,
    startClock,
    endClock);
}
*/


}
