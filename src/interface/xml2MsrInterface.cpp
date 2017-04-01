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

#include "xml2Msr.h"

#include "xml2MsrTranslator.h"
#include "msr2Summary.h"

using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
static S_msrScore xml2Msr (
  SXMLFile&     xmlfile,
  S_msrOptions& msrOpts,
  ostream&      os,
  const char*   file) 
{
  // build xmlelement tree from the file contents
  Sxmlelement elemsTree = xmlfile->elements();

  S_msrScore mScore;
  
  if (elemsTree) {
    // build the MSR
    mScore =
      buildMSRFromElementsTree (msrOpts, elemsTree);

    if (msrOpts->fDisplayMSR)
      // display the MSR
      displayMSR (msrOpts, mScore, cerr);

    if (msrOpts->fDisplayMSRSummary)
      // display the MSR summary
      displayMSRSummary (msrOpts, mScore, cerr);
  }

  return mScore;
}

//_______________________________________________________________________________
EXP S_msrScore musicxmlFile2Msr (
  const char*   file,
  S_msrOptions& msrOpts,
  ostream&      os) 
{
  clock_t startClock = clock();

  if (gGeneralOptions->fTraceGeneral) {
    string separator =
      "%--------------------------------------------------------------";
    
    cerr <<
      endl <<
      idtr << separator <<
      endl <<
      "Pass 1: building the xmlelement tree from \"" << file << "\"" <<
      endl <<
      idtr << separator <<
      endl;
  }

  xmlreader r;
  SXMLFile  xmlFile;

  xmlFile = r.read (file);

  clock_t endClock = clock();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 1: build xmlelement tree from file",
    timingItem::kMandatory,
    startClock,
    endClock);
  
  S_msrScore mScore;

  if (xmlFile)
    mScore = xml2Msr (xmlFile, msrOpts, os, file);  

  return mScore;
}

//_______________________________________________________________________________
EXP S_msrScore musicxmlFd2Msr (
  FILE*         fd,
  S_msrOptions& msrOpts,
  ostream&      os) 
{
  clock_t startClock = clock();

  if (gGeneralOptions->fTraceGeneral) {
    string separator =
      "%--------------------------------------------------------------";
    
    cerr <<
      endl <<
      idtr << separator <<
      endl <<
      "Pass 1: building the xmlelement tree from standard input" <<
      endl <<
      idtr << separator <<
      endl;
  }

  xmlreader r;
  SXMLFile  xmlFile;

  xmlFile = r.read (fd);

  clock_t endClock = clock();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 1: build xmlelement tree from stdin",
    timingItem::kMandatory,
    startClock,
    endClock);
  
  S_msrScore mScore;

  if (xmlFile) {
    mScore = xml2Msr (xmlFile, msrOpts, os, 0);
  }
  
  return mScore;
}

//_______________________________________________________________________________
EXP S_msrScore musicxmlString2Msr (
  const char*   buffer,
  S_msrOptions& msrOpts,
  ostream&      os) 
{
  clock_t startClock = clock();

  if (gGeneralOptions->fTraceGeneral) {
    string separator =
      "%--------------------------------------------------------------";
    
    cerr <<
      endl <<
      idtr << separator <<
      endl <<
      idtr << "Pass 1: building the xmlelement tree from a buffer" <<
      endl <<
      idtr << separator <<
      endl;
  }
  
  xmlreader r;
  SXMLFile  xmlFile;
  
  xmlFile = r.readbuff (buffer);

  clock_t endClock = clock();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 1: build xmlelement tree from buffer",
    timingItem::kMandatory,
    startClock,
    endClock);
  
  S_msrScore mScore;

  if (xmlFile) {
    mScore = xml2Msr (xmlFile, msrOpts, os, 0);
  }
  return mScore;
}

//_______________________________________________________________________________
S_msrScore buildMSRFromElementsTree (
  S_msrOptions& msrOpts,
  Sxmlelement   xmlTree)
{
  clock_t startClock = clock();

  if (gGeneralOptions->fTraceGeneral) {
    string separator =
      "%--------------------------------------------------------------";
    
    cerr <<
      endl <<
      idtr << separator <<
      endl <<
      "Pass 2: translating the xmlelement tree into a MSR" <<
      endl;
    if (msrOpts->fDisplayMSR)
      cerr <<
        "(dpqn: divisions per quarter note)" <<
        endl;
    cerr <<
      idtr << separator <<
      endl;
  }
  
  // create an xml2MsrTranslator
  xml2MsrTranslator translator;

  // build the MSR score
  S_msrScore
    mScore =
      translator.buildMsrScoreFromXMLElementTree (xmlTree);

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
  S_msrOptions& msrOpts,
  S_msrScore    mScore,
  ostream&      os)
{
  clock_t startClock = clock();

  if (gGeneralOptions->fTraceGeneral) {
    string separator =
      "%--------------------------------------------------------------";
    
    cerr <<
      endl <<
      idtr << separator <<
      endl <<
      "Optional pass: displaying the MSR as text" <<
      endl <<
      "(dpqn: divisions per quarter note)" <<
      endl <<
      "(dpfm: divisions per full measure)" <<
      endl <<
      idtr << separator <<
      endl;
  }

  if (gGeneralOptions->fTraceGeneral) os << "%{" << endl;
  os << mScore;
  if (gGeneralOptions->fTraceGeneral) os << "%}" << endl;

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
  S_msrOptions& msrOpts,
  S_msrScore    mScore,
  ostream&      os)
{
  clock_t startClock = clock();

  if (gGeneralOptions->fTraceGeneral) {
    string separator =
      "%--------------------------------------------------------------";
    
    cerr <<
      endl <<
      idtr << separator <<
      endl <<
      "Optional pass: outputting a summary of the MSR" <<
      endl <<
      idtr << separator <<
      endl;
  }
   
  // create an msr2SummaryVisitor visitor
  msr2SummaryVisitor visitor (msrOpts, os);

  if (gGeneralOptions->fTraceGeneral) os << "%{" << std::endl;
  
  visitor.printSummaryFromMsrScore (mScore);
  
  if (gGeneralOptions->fTraceGeneral) os << "%}" << std::endl;

  clock_t endClock = clock();

  // register time spent
  timing::gTiming.appendTimingItem (
    "        display MSR summary",
    timingItem::kOptional,
    startClock,
    endClock);
}


}
