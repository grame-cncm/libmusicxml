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

#include "generalOptions.h"

#include "mxmlTree2MsrTranslatorInterface.h"

#include "mxmlTree2MsrTranslator.h"

#include "msr2Summary.h"

using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
static S_msrScore populateMSRSkeletonFromFile (
  S_msrOptions&    msrOpts,
  Sxmlelement      elemsTree,
  S_msrScore       scoreSkeleton,
  indentedOstream& logIOstream) 
{
  if (scoreSkeleton) {
    // populate the MSR skeleton
    scoreSkeleton =
      populateMSRSkeletonFromElementsTree (
        msrOpts,
        elemsTree,
        logIOstream);
    
    if (msrOpts->fDisplayMsr)
      // display the MSR
      displayMSR (
        msrOpts,
        scoreSkeleton,
        logIOstream);

    if (msrOpts->fDisplayMsrSummary)
      // display the MSR summary
      displayMSRSummary (
        msrOpts,
        scoreSkeleton,
        logIOstream);
  }

  // return the populated skeleton
  return scoreSkeleton;
}

//_______________________________________________________________________________
EXP S_msrScore mxmlFile2Msr (
  const char*      fileName,
  S_msrOptions&    msrOpts,
  indentedOstream& logIOstream) 
{
  clock_t startClock = clock();

  if (gGeneralOptions->fTraceGeneral) {
    string separator =
      "%--------------------------------------------------------------";
    
    gLogIOstream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      "Pass 1: building the xmlelement tree from \"" << fileName << "\"" <<
      endl <<
      separator <<
      endl;
  }

  xmlreader r;
  
  SXMLFile xmlFile = r.read (fileName);

  clock_t endClock = clock();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 1: build xmlelement tree from file",
    timingItem::kMandatory,
    startClock,
    endClock);
  
  S_msrScore mScore;

  if (xmlFile)
    mScore =
      xml2Msr (
        xmlFile,
        msrOpts,
        logIOstream);  

  return mScore;
}

//_______________________________________________________________________________
S_msrScore populateMSRSkeletonFromElementsTree (
  S_msrOptions&    msrOpts,
  Sxmlelement      elemsTree,
  S_msrScore       msrSkeleton,
  indentedOstream& logIOstream)
{
  clock_t startClock = clock();

  if (gGeneralOptions->fTraceGeneral) {
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
      endl <<
      endl;
  }
  
  // create an musicXMLTree2MsrTranslator
  musicXMLTree2MsrTranslator
    translator (
      logIOstream);

  // build the MSR score
  S_msrScore
    mScore =
      translator.buildMsrScoreFromXMLElementTree (
        xmlTree);

  clock_t endClock = clock();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 2b: build the MSR",
    timingItem::kMandatory,
    startClock,
    endClock);

  return mScore;
}

//_______________________________________________________________________________
void displayMSRSkeleton (
  S_msrOptions&    msrOpts,
  S_msrScore       mScore,
  indentedOstream& logIOstream)
{
  clock_t startClock = clock();
  
  string separator =
    "%--------------------------------------------------------------";
  
  logIOstream <<
    endl <<
    separator <<
    endl <<
    gTab <<
    "Optional pass: displaying the populated MSR as text" <<
    endl <<
    separator <<
    endl <<
    endl;

  logIOstream <<
    "%{" <<
    endl <<
      
    mScore <<

    "%}" <<
    endl;

  clock_t endClock = clock();

  // register time spent
  timing::gTiming.appendTimingItem (
    "        display the populated MSR",
    timingItem::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayMSRSkeletonSummary (
  S_msrOptions&    msrOpts,
  S_msrScore       mScore,
  indentedOstream& logIOstream)
{
  clock_t startClock = clock();
  
  if (gGeneralOptions->fTraceGeneral) {
    string separator =
      "%--------------------------------------------------------------";
    
    logIOstream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      "Optional pass: outputting a summary of the populated MSR" <<
      endl <<
      separator <<
      endl;
  }
   
  // create an msr2SummaryVisitor visitor
  msr2SummaryVisitor
    visitor (
      msrOpts,
      logIOstream);

  if (gGeneralOptions->fTraceGeneral)
    logIOstream <<
      "%{" <<
      endl;
  
  visitor.printSummaryFromMsrScore (
    mScore);
  
  if (gGeneralOptions->fTraceGeneral)
    logIOstream <<
      "%}" <<
      endl;

  clock_t endClock = clock();

  // register time spent
  timing::gTiming.appendTimingItem (
    "        display populated MSR summary",
    timingItem::kOptional,
    startClock,
    endClock);
}


}
