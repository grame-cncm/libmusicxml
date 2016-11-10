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
// JMI #include "xml_tree_browser.h"

#include "versions.h"

#include "musicxml2msr.h"

#include "xml2MsrScoreVisitor.h"
#include "msr2SummaryVisitor.h"

using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
EXP int         musicxml2MsrVersionNbr () { return 010; }
EXP const char* musicxml2MsrVersionStr () { return "0.1.0"; }

//_______________________________________________________________________________

S_msrScore buildMsrScoreFromElementsTree (
  S_msrOptions& msrOpts,
  Sxmlelement   xmlTree)
{
    // browse the part contents for the first time with a xml2MsrScoreVisitor
  if (msrOpts->fTrace)
    cerr << idtr <<
      "Building a MSR score from the xmlelement tree" << endl;

  idtr++;
  
  // create an xml2MsrVisitor
  xml2MsrScoreVisitor visitor (msrOpts);

  // build the MSR score
  S_msrScore
    msrScore =
      visitor.buildMsrScoreFromXMLElementTree (xmlTree);

  idtr--;

  return msrScore;
}

//_______________________________________________________________________________
void displayMsrScore (
  S_msrOptions& msrOpts,
  S_msrScore    msrScore,
  ostream&      os)
{
  string separator = "%----------------------------------------";

  // output the score resulting from the conversion 
  if (msrOpts->fTrace) 
    os << idtr <<
      separator << endl <<
      "%Outputting a hierarchical view of the MSR score" << endl <<
      separator << endl;
  
  if (msrOpts->fTrace) os << "%{" << endl;
  os << msrScore;
  if (msrOpts->fTrace) os << "%}" << endl;
  
  os << separator << endl;
}

//_______________________________________________________________________________
void displayMsrScoreSummary (
  S_msrOptions& msrOpts,
  S_msrScore    msrScore,
  ostream&      os)
{
  string separator = "%----------------------------------------";

  // output the score resulting from the conversion 
  if (msrOpts->fTrace) 
    os << idtr <<
      separator << endl <<
      "%Outputting a summary of the MSR score" << endl <<
      separator << endl;
   
  // create an msr2SummaryVisitor visitor
  msr2SummaryVisitor visitor (msrOpts, os);

  if (msrOpts->fTrace) os << "%{" << std::endl;
  
  visitor.printSummaryFromMsrScore (msrScore);
  
  if (msrOpts->fTrace) os << "%}" << std::endl;
  
  os << separator << std::endl;
}

/* JMI
//_______________________________________________________________________________
void displayScoreSummaryWithoutVisitor (
  S_msrOptions& msrOpts,
  S_msrScore    msrScore)
{
  string separator = "%----------------------------------------";

  // output the score resulting from the conversion 
  // thru msrElement::printMsrStructure()
  if (msrOpts->fTrace) 
    cerr << idtr <<
      separator << endl <<
      "%Outputting a summary of the MSR score" << endl <<
      separator << endl;
  
  // choosing kScoreSummary to print the score
  msrGlobalVariables::setDisplayKind (
    msrGlobalVariables::kScoreSummary);
   
  if (msrOpts->fTrace) cerr << "{%" << std::endl;
  cerr << msrScore;
  if (msrOpts->fTrace) cerr << "%}" << std::endl;
  
  cerr << separator << std::endl;
}
*/

//_______________________________________________________________________________
/*
 * The method that converts the file contents to LilyPond code
 * and  writes the result to the output stream
*/
static xmlErr xml2Msr (
  SXMLFile&     xmlfile,
  S_msrOptions& msrOpts,
  ostream&      os,
  const char*   file) 
{
  // build xmlelement tree from the file contents
  Sxmlelement elemsTree = xmlfile->elements();
  
  if (elemsTree) {
    S_msrScore
      msrScore =
        buildMsrScoreFromElementsTree (msrOpts, elemsTree);

    if (msrOpts->fDisplayMSR)
      displayMsrScore (msrOpts, msrScore, os);

 //   if (msrOpts->fDisplayMSRScoreSummary)
 //     displayScoreSummary (msrOpts, score); // JMI
    if (msrOpts->fDisplayMSRScoreSummary)
      displayMsrScoreSummary (msrOpts, msrScore, os);

    return kNoErr;
  }

  return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlFile2Msr (
  const char*   file,
  S_msrOptions& msrOpts,
  ostream&      os) 
{
  if (msrOpts->fTrace)
    cerr << idtr <<
      "Building xmlemement tree from \"" << file << "\"" << endl;

  xmlreader r;
  SXMLFile  xmlFile;

  xmlFile = r.read(file);
  
  if (xmlFile) {
    return xml2Msr (xmlFile, msrOpts, os, file);
  }
  
  return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlFd2Msr (
  FILE*         fd,
  S_msrOptions& msrOpts,
  ostream&      os) 
{
  if (msrOpts->fTrace)
    cerr << idtr <<
      "Building xmlemement tree from standard input" << endl;

  xmlreader r;
  SXMLFile  xmlFile;

  xmlFile = r.read(fd);
  
  if (xmlFile) {
    return xml2Msr (xmlFile, msrOpts, os, 0);
  }
  
  return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlString2Msr (
  const char*   buffer,
  S_msrOptions& msrOpts,
  ostream&      os) 
{
  if (msrOpts->fTrace)
    cerr << idtr <<
      "Building xmlemement tree from a buffer" << endl;
  
  xmlreader r;
  SXMLFile  xmlFile;
  
  xmlFile = r.readbuff(buffer);
  
  if (xmlFile) {
    return xml2Msr (xmlFile, msrOpts, os, 0);
  }
  
  return kInvalidFile;
}


}
