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

#include "musicXMLTree2MsrInterface.h"

#include "musicXMLTree2MsrTranslator.h"

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
      buildMSRFromElementsTree (
        msrOpts,
        elemsTree);

    // create an indented output stream for the log
    indentedOstream
      logindentedOstream (
        cerr, gIndenter);
    
    if (msrOpts->fDisplayMsr)
      // display the MSR
      displayMSR (
        msrOpts,
        mScore,
        logindentedOstream);

    if (msrOpts->fDisplayMsrSummary)
      // display the MSR summary
      displayMSRSummary (
        msrOpts,
        mScore,
        logindentedOstream);
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

  // create an indented output stream for the log
  indentedOstream
    logindentedOstream ( // JMI ???
      os, gIndenter);
  
  if (gGeneralOptions->fTraceGeneral) {
    string separator =
      "%--------------------------------------------------------------";
    
    gLogIOstream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      "Pass 1: building the xmlelement tree from \"" << file << "\"" <<
      endl <<
      separator <<
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
    mScore =
      xml2Msr (
        xmlFile,
        msrOpts,
        logindentedOstream,
        file);  

  return mScore;
}

//_______________________________________________________________________________
EXP S_msrScore musicxmlFd2Msr (
  FILE*         fd,
  S_msrOptions& msrOpts,
  ostream&      os) 
{
  clock_t startClock = clock();

  // create an indented output stream for the log
  indentedOstream
    logindentedOstream (
      os, gIndenter);
  
  if (gGeneralOptions->fTraceGeneral) {
    string separator =
      "%--------------------------------------------------------------";
    
    logindentedOstream <<
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
    mScore =
      xml2Msr (
        xmlFile,
        msrOpts,
        logindentedOstream,
        0);
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

  // create an indented output stream for the log
  indentedOstream
    logindentedOstream (
      os, gIndenter);
  
  if (gGeneralOptions->fTraceGeneral) {
    string separator =
      "%--------------------------------------------------------------";
    
    logindentedOstream <<
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
    mScore =
      xml2Msr (
        xmlFile,
        msrOpts,
        logindentedOstream,
        0);
  }
  return mScore;
}

//_______________________________________________________________________________
S_msrScore buildMSRFromElementsTree (
  S_msrOptions& msrOpts,
  Sxmlelement   xmlTree)
{
  clock_t startClock = clock();

  // create an indented output stream for the log
  indentedOstream
    logindentedOstream (
      cerr, gIndenter);
  
  if (gGeneralOptions->fTraceGeneral) {
    string separator =
      "%--------------------------------------------------------------";
  
    logindentedOstream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      "Pass 2: translating the xmlelement tree into a MSR" <<
      endl;
    
    logindentedOstream <<
      separator <<
      endl <<
      endl;
  }
  
  // create an musicXMLTree2MsrTranslator
  musicXMLTree2MsrTranslator
    translator (
      logindentedOstream);

  // build the MSR score
  S_msrScore
    mScore =
      translator.buildMsrScoreFromXMLElementTree (
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
  S_msrOptions&         msrOpts,
  S_msrScore            mScore,
  indentedOstream& ios)
{
  clock_t startClock = clock();
  
  string separator =
    "%--------------------------------------------------------------";
  
  ios <<
    endl <<
    separator <<
    endl <<
    gTab <<
    "Optional pass: displaying the MSR as text" <<
    endl <<
    separator <<
    endl <<
    endl;

  ios <<
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
  S_msrOptions&         msrOpts,
  S_msrScore            mScore,
  indentedOstream& ios)
{
  clock_t startClock = clock();
  
  if (gGeneralOptions->fTraceGeneral) {
    string separator =
      "%--------------------------------------------------------------";
    
    ios <<
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
      ios);

  if (gGeneralOptions->fTraceGeneral)
    ios <<
      "%{" <<
      endl;
  
  visitor.printSummaryFromMsrScore (
    mScore);
  
  if (gGeneralOptions->fTraceGeneral)
    ios <<
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
