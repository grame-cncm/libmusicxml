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

#include "musicXMLTree2MsrSkeletonInterface.h"

#include "musicXMLTree2MsrSkeleton.h"

#include "msr2Summary.h"

using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
static S_msrScore xml2Msr (
  SXMLFile&        xmlfile,
  S_msrOptions&    msrOpts,
  indentedOstream& logIOstream) 
{
  // build xmlelement tree from the file contents
  Sxmlelement elemsTree = xmlfile->elements();

  S_msrScore mScore;
  
  if (elemsTree) {
    // build the MSR
    mScore =
      buildMSRFromElementsTree (
        msrOpts,
        elemsTree,
        logIOstream);
    
    if (msrOpts->fDisplayMsr)
      // display the MSR
      displayMSR (
        msrOpts,
        mScore,
        logIOstream);

    if (msrOpts->fDisplayMsrSummary)
      // display the MSR summary
      displayMSRSummary (
        msrOpts,
        mScore,
        logIOstream);
  }

  return mScore;
}

//_______________________________________________________________________________
EXP S_msrScore musicxmlFile2Msr (
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
EXP S_msrScore musicxmlFd2Msr (
  FILE*            fd,
  S_msrOptions&    msrOpts,
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
      "Pass 1: building the xmlelement tree from standard input" <<
      endl <<
      separator <<
      endl;
  }

  xmlreader r;
  
  SXMLFile  xmlFile = r.read (fd);

  clock_t endClock = clock();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 1: build xmlelement tree from stdin",
    timingItem::kMandatory,
    startClock,
    endClock);
  
  S_msrScore mScore;

  if (xmlFile) {
    mScore =
      xml2Msr (
        xmlFile,
        msrOpts,
        logIOstream);
  }
  
  return mScore;
}

//_______________________________________________________________________________
EXP S_msrScore musicxmlString2Msr (
  const char*      buffer,
  S_msrOptions&    msrOpts,
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
      "Pass 1: building the xmlelement tree from a buffer" <<
      endl <<
      separator <<
      endl;
  }
  
  xmlreader r;
  
  SXMLFile  xmlFile = r.readbuff (buffer);

  clock_t endClock = clock();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 1: build xmlelement tree from buffer",
    timingItem::kMandatory,
    startClock,
    endClock);
  
  S_msrScore mScore;

  if (xmlFile) {
    mScore =
      xml2Msr (
        xmlFile,
        msrOpts,
        logIOstream);
  }
  return mScore;
}

//_______________________________________________________________________________
S_msrScore buildMSRFromElementsTree (
  S_msrOptions&    msrOpts,
  Sxmlelement      xmlTree,
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
      "Pass 2: translating the xmlelement tree into a MSR" <<
      endl;
    
    logIOstream <<
      separator <<
      endl <<
      endl;
  }
  
  // create an musicXMLTree2MsrSkeleton
  musicXMLTree2MsrSkeleton
    skeletonBuilder (
      logIOstream);

  // build the MSR score
  S_msrScore
    mScore =
      skeletonBuilder.buildMsrScoreFromXMLElementTree (
        xmlTree);

  clock_t endClock = clock();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 2: build the MSR",
    timingItem::kMandatory,
    startClock,
    endClock);

  return mScore;
}

//_______________________________________________________________________________
void displayMSR (
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
    "Optional pass: displaying the MSR as text" <<
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
    "        display the MSR",
    timingItem::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayMSRSummary (
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
      "Optional pass: outputting a summary of the MSR" <<
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
    "        display MSR summary",
    timingItem::kOptional,
    startClock,
    endClock);
}


}
