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
    mScore =
      buildMsrScoreFromElementsTree (msrOpts, elemsTree);

    if (msrOpts->fDisplayMSR)
      displayMsrScore (msrOpts, mScore, cerr);

    if (msrOpts->fDisplayMSRScoreSummary)
      displayMsrScoreSummary (msrOpts, mScore, cerr);
  }

  return mScore;
}

//_______________________________________________________________________________
EXP S_msrScore musicxmlFile2Msr (
  const char*   file,
  S_msrOptions& msrOpts,
  ostream&      os) 
{
  time_t starttime;
  time (&starttime);

  if (gGeneralOptions->fTrace) {
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
  
  S_msrScore mScore;

  if (xmlFile)
    mScore = xml2Msr (xmlFile, msrOpts, os, file);  

  time_t endtime;
  time (&endtime);

  // register time spent
  timing::gTiming.addTimingItem (
    "Pass 1: build xmlelement tree from file",
    timingItem::kMandatory,
    starttime,
    endtime);

  return mScore;
}

//_______________________________________________________________________________
EXP S_msrScore musicxmlFd2Msr (
  FILE*         fd,
  S_msrOptions& msrOpts,
  ostream&      os) 
{
  time_t starttime;
  time (&starttime);

  if (gGeneralOptions->fTrace) {
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
  
  S_msrScore mScore;

  if (xmlFile) {
    mScore = xml2Msr (xmlFile, msrOpts, os, 0);
  }

  time_t endtime;
  time (&endtime);

  // register time spent
  timing::gTiming.addTimingItem (
    "Pass 1: build xmlelement tree from stdin",
    timingItem::kMandatory,
    starttime,
    endtime);
  
  return mScore;
}

//_______________________________________________________________________________
EXP S_msrScore musicxmlString2Msr (
  const char*   buffer,
  S_msrOptions& msrOpts,
  ostream&      os) 
{
  time_t starttime;
  time (&starttime);

  if (gGeneralOptions->fTrace) {
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
  
  S_msrScore mScore;

  if (xmlFile) {
    mScore = xml2Msr (xmlFile, msrOpts, os, 0);
  }

  time_t endtime;
  time (&endtime);

  // register time spent
  timing::gTiming.addTimingItem (
    "Pass 1: build xmlelement tree from buffer",
    timingItem::kMandatory,
    starttime,
    endtime);
  
  return mScore;
}

//_______________________________________________________________________________
S_msrScore buildMsrScoreFromElementsTree (
  S_msrOptions& msrOpts,
  Sxmlelement   xmlTree)
{
  time_t starttime;
  time (&starttime);

  if (gGeneralOptions->fTrace) {
    string separator =
      "%--------------------------------------------------------------";
    
    cerr <<
      endl <<
      idtr << separator <<
      endl <<
      "Pass 2: translating the xmlelement tree into a MSR" <<
      endl <<
      idtr << separator <<
      endl;
  }
  
  // create an xml2MsrTranslator
  xml2MsrTranslator translator;

  // build the MSR score
  S_msrScore
    mScore =
      translator.buildMsrScoreFromXMLElementTree (xmlTree);

  time_t endtime;
  time (&endtime);

  // register time spent
  timing::gTiming.addTimingItem (
    "Pass 2: build the MSR",
    timingItem::kMandatory,
    starttime,
    endtime);

  return mScore;
}

//_______________________________________________________________________________
void displayMsrScore (
  S_msrOptions& msrOpts,
  S_msrScore    mScore,
  ostream&      os)
{
  time_t starttime;
  time (&starttime);

  if (gGeneralOptions->fTrace) {
    string separator =
      "%--------------------------------------------------------------";
    
    cerr <<
      endl <<
      idtr << separator <<
      endl <<
      "Optional pass: displaying the MSR as text" <<
      endl <<
      idtr << separator <<
      endl;
  }

  if (gGeneralOptions->fTrace) os << "%{" << endl;
  os << mScore;
  if (gGeneralOptions->fTrace) os << "%}" << endl;

  time_t endtime;
  time (&endtime);

  // register time spent
  timing::gTiming.addTimingItem (
    "        display the MSR",
    timingItem::kOptional,
    starttime,
    endtime);
}

//_______________________________________________________________________________
void displayMsrScoreSummary (
  S_msrOptions& msrOpts,
  S_msrScore    mScore,
  ostream&      os)
{
  time_t starttime;
  time (&starttime);

  if (gGeneralOptions->fTrace) {
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

  if (gGeneralOptions->fTrace) os << "%{" << std::endl;
  
  visitor.printSummaryFromMsrScore (mScore);
  
  if (gGeneralOptions->fTrace) os << "%}" << std::endl;

  time_t endtime;
  time (&endtime);

  // register time spent
  timing::gTiming.addTimingItem (
    "        display MSR summary",
    timingItem::kOptional,
    starttime,
    endtime);
}


}
