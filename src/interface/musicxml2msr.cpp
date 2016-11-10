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

S_msrScore buildMsrScoreFromTree (
  S_msrOptions& msrOpts,
  Sxmlelement            xmlTree)
{
    // browse the part contents for the first time with a xml2MsrScoreVisitor
  if (msrOpts->fTrace)
    cerr << idtr <<
      "Building a MSR score from the xmlelement tree" << endl;

  idtr++;
  
  // create an xml2MsrVisitor
  xml2MsrScoreVisitor visitor (msrOpts);
      
  // a global variable is needed so that msr::Element.print() 
  // can decide whether to print:
  //   - the MSR structure
  //   - MusicXML text
  //   - LilyPond source code
  // use the visitor to build a MSR score from the xmlelement tree
  // choosing kMSR for the trace of the score build
  msrGlobalVariables::setDisplayKind (
    msrGlobalVariables::kMSR);

  S_msrScore
    score =
      visitor.buildMsrScoreFromXMLElementTree (xmlTree);

  idtr--;

  return score;
}

//_______________________________________________________________________________
void displayMSRScoreSummaryWithVisitor (
  S_msrOptions& msrOpts,
  S_msrScore    score,
  ostream&      os)
{
  string separator = "%----------------------------------------";

  // output the score resulting from the conversion 
  if (msrOpts->fTrace) 
    os << idtr <<
      separator << endl <<
      "%Outputting a summary of the MSR score" << endl <<
      separator << endl;
   
  idtr++;

  // create an msr2SummaryVisitor visitor
  msr2SummaryVisitor visitor (msrOpts, os);

  if (msrOpts->fTrace) os << "%{" << std::endl;
  
  visitor.printSummaryFromMsrScore (score);
  
  if (msrOpts->fTrace) os << "%}" << std::endl;
  
  os << separator << std::endl;

  idtr--;
}

//_______________________________________________________________________________
void displayMSRScore (
  S_msrOptions& msrOpts,
  S_msrScore    score,
  ostream&      os)
{
  string separator = "%----------------------------------------";

  // output the score resulting from the conversion 
  // thru msrElement::printMsrStructure()
  if (msrOpts->fTrace) 
    os << idtr <<
      separator << endl <<
      "%Outputting a hierarchical view of the MSR score" << endl <<
      separator << endl;
  
  // choosing kMSR to print the score
  msrGlobalVariables::setDisplayKind (
    msrGlobalVariables::kMSR);
   
  if (msrOpts->fTrace) os << "%{" << endl;
  os << score;
  if (msrOpts->fTrace) os << "%}" << endl;
  
  os << separator << endl;
}

//_______________________________________________________________________________
void displayScoreSummary (
  S_msrOptions& msrOpts,
  S_msrScore    score)
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
  cerr << score;
  if (msrOpts->fTrace) cerr << "%}" << std::endl;
  
  cerr << separator << std::endl;
}

//_______________________________________________________________________________
/*
 * The method that converts the file contents to LilyPond code
 * and  writes the result to the output stream
*/
static xmlErr xml2Msr(
  SXMLFile&     xmlfile,
  S_msrOptions& msrOpts,
  ostream&      os,
  const char* file) 
{
  // build xmlelement tree from the file contents
  Sxmlelement elemsTree = xmlfile->elements();
  
  if (elemsTree) {
    S_msrScore score =
      buildMsrScoreFromTree (msrOpts, elemsTree);

    if (msrOpts->fDisplayMSR)
      displayMSRScore (msrOpts, score, os);

    if (msrOpts->fDisplayMSRScoreSummary)
      displayScoreSummary (msrOpts, score); // TEMP
    if (msrOpts->fDisplayMSRScoreSummary)
      displayMSRScoreSummaryWithVisitor (msrOpts, score, os);

    /*
    // create an xml2MsrVisitor
    xml2MsrVisitor v(msrOpts);
    
    // use the visitor to convert the xmlelement tree into a msrElement tree
    if (msrOpts->fTrace)
      cerr << "Launching conversion of xmlelement tree to LilyPond semantic representation tree" << endl;
    S_msrElement    msr = v.convertToMsr(elemsTree);
    
    string separator = "%----------------------------------------";
 
    // output the structure of the msrElement tree resulting from the conversion 
    // thru msrElement::printMsrStructure()
    if (msrOpts->fTrace) 
      cerr << 
        separator << endl <<
        "%Outputting the LilyPond semantic representation tree" << endl <<
        separator << endl;
    
    // a global variable is needed so that msr::Element.print() 
    // can decide whether to print:
    //   - the MSR structure
    //   - MusicXML text
    //   - LilyPond source code
    msrGlobalVariables::setDisplayKind (msrGlobalVariables::kMSR);
    if (msrOpts->fTrace) cerr << "{%" << std::endl;
    std::cerr << 
      msr << std::endl;
    if (msrOpts->fTrace) cerr << "%}" << std::endl;
    std::cerr << endl << separator << std::endl;

    if (msrOpts->fTrace) 
      cerr << 
        "%Outputting the LilyPond source code" << endl <<
        separator << endl << endl;

    // output the general information about the conversion
    os << "%{" << std::endl;
    if (file) {
      os <<
        "  LilyPond code converted from \"" << file << "\"" << std::endl;
    }
    else 
      os << 
        "  LilyPond code converted from standard input" << std::endl; 
    os << 
      "  using libmusicxml2" << //<< musicxmllibVersionStr() <<
      " and its embedded xml2lilypond converter " << //<< musicxml2msrVersionStr() <<
      std::endl;
      
    cout << "  Options were: ";
    if (msrOpts->fSelectedOptions.size() == 0)
      os << "none";
    else
      os << std::endl << "    " << msrOpts->fSelectedOptions;
    os << std::endl;
    os << "%}" << std::endl << std::endl;
      
    // output the LilyPond semantic representation tree resulting from the conversion 
    msrGlobalVariables::setDisplayKind (msrGlobalVariables::kLilypondCode);
    os <<
      msr << endl;
    
    if (msrOpts->fTrace) 
      cerr << separator << endl;
   */
   
    return kNoErr;

  }

  return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlFile2Msr(
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
EXP xmlErr musicxmlFd2Msr(
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
EXP xmlErr musicxmlString2Msr(
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
