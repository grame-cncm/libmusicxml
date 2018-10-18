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

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "mxmlTree2MsrTranslatorInterface.h"

#include "mxmlTree2MsrTranslator.h"

#include "msr2Summary.h"
#include "msr2Names.h"

using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
void populateMsrSkeletonFromMxmlTree (
  S_msrOptions&    msrOpts,
  Sxmlelement      mxmlTree,
  S_msrScore       scoreSkeleton,
  indentedOstream& logIOstream)
{
  // sanity check
  msrAssert (
    mxmlTree != 0,
    "mxmlTree is null");
    
  // sanity check
  msrAssert (
    scoreSkeleton != 0,
    "scoreSkeleton is null");
    
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
      "Pass 2b: translating the xmlelement tree into a MSR" <<
      endl;
    
    logIOstream <<
      separator <<
      endl;
  }
#endif
  
  // create an mxmlTree2MsrTranslator
  mxmlTree2MsrTranslator
    translator (
      scoreSkeleton,
      logIOstream);

  // browse the mxmlTree
  translator.browseMxmlTree (
    mxmlTree);

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 2b",
    "build the MSR",
    timingItem::kMandatory,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayMSRPopulatedScore (
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
    "Optional pass: displaying the MSR as text" <<
    endl <<
    separator <<
    endl <<
    endl <<
    mScore;

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "",
    "display the MSR",
    timingItem::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayMSRPopulatedScoreSummary (
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
      endl <<
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
    "display MSR summary",
    timingItem::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayMSRPopulatedScoreNames (
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
      "Optional pass: outputting the names in the MSR" <<
      endl <<
      separator <<
      endl <<
      endl;
  }
#endif
   
  // create an msr2NamesVisitor visitor
  msr2NamesVisitor
    namesVisitor (
      msrOpts,
      logIOstream);

  namesVisitor.printNamesFromMsrScore (
    mScore);
  
  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "",
    "display MSR names",
    timingItem::kOptional,
    startClock,
    endClock);
}


}
