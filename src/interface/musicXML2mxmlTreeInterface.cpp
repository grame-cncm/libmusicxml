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

#include <iomanip> // for setw()

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "traceOptions.h"
#include "musicXMLOptions.h"

#include "musicXML2mxmlTreeInterface.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
EXP Sxmlelement musicXMLFile2mxmlTree (
  const char*       fileName,
  S_musicXMLOptions mxmlOpts,
  indentedOstream&  logIOstream) 
{
  clock_t startClock = clock ();

  if (gTraceOptions->fTraceBasic) {
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
      endl <<
      endl;
  }

  xmlreader r;
  
  SXMLFile xmlFile = r.read (fileName);
  if (! xmlFile)
    return Sxmlelement (0);

  if (false) { // JMI
    gLogIOstream <<
      "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
      endl;
      
    xmlFile->print (gLogIOstream);

    gLogIOstream <<
      endl;
  }
  
  // get the xmlDecl
  TXMLDecl * xmlDecl = xmlFile->getXMLDecl ();
  
  if (false) { // JMI
    gLogIOstream <<
      "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
      "getXMLDecl =" <<
      endl;
    xmlDecl->print (gLogIOstream);
  }
  
  string fXMLVersion = xmlDecl->getVersion ();
  string fXMLEncoding = xmlDecl->getEncoding ();
  int     fXMLStandalone = xmlDecl->getStandalone ();

  const int fieldWidth = 17;
  
  gLogIOstream <<
    "XML Declaration:" <<
    endl;

  gIndenter++;
  
  gLogIOstream << left <<
    setw (fieldWidth) <<
    "fXMLVersion" << " = \"" << fXMLVersion << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fXMLEncoding" << " = \"" << fXMLEncoding << "\"" <<
    endl  <<
    setw (fieldWidth) <<
    "fXMLStandalone" << " = \"" << fXMLStandalone << "\"" <<
    endl <<
    endl;

  gIndenter--;

  // get the docType
  TDocType * docType = xmlFile->getDocType ();
  
  if (false) { // JMI
    gLogIOstream <<
      "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
      "getDocType =" <<
      endl;
    docType->print (gLogIOstream);
  }

  gLogIOstream <<
    "Document Type:" <<
    endl;

  gIndenter++;
  
  std::string fXMLStartElement = docType->getStartElement ();
  bool    fXMLPublic = docType->getPublic ();
  std::string fXMLPubLitteral = docType->getPubLitteral ();
  std::string fXMLSysLitteral = docType->getSysLitteral ();

  gLogIOstream << left <<
    setw (fieldWidth) <<
    "fXMLStartElement" << " = \"" << fXMLStartElement << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fXMLPublic" << " = \"" << fXMLPublic << "\"" <<
    endl  <<
    setw (fieldWidth) <<
    "fXMLPubLitteral" << " = \"" << fXMLPubLitteral << "\"" <<
    endl  <<
    setw (fieldWidth) <<
    "fXMLSysLitteral" << " = \"" << fXMLSysLitteral << "\"" <<
    endl <<
    endl;
    
  gIndenter--;
    
  // should the encoding be converted to UTF-8?
  string desiredEncoding = "UTF-8";
  
  if (fXMLEncoding != desiredEncoding) {
    gLogIOstream <<
      "Converting MusicXML data from \"" <<
      fXMLEncoding <<
      "\" to " <<
      desiredEncoding <<
      "\"" <<
      endl;
  }
    
  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 1",
    "build xmlelement tree from file",
    timingItem::kMandatory,
    startClock,
    endClock);
  
  // fetch mxmlTree
  Sxmlelement mxmlTree = xmlFile->elements ();

  return mxmlTree;
}

//_______________________________________________________________________________
EXP Sxmlelement musicXMLFd2mxmlTree (
  FILE*             fd,
  S_musicXMLOptions mxmlOpts,
  indentedOstream&  logIOstream) 
{
  clock_t startClock = clock ();
  
  if (gTraceOptions->fTraceBasic) {
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
  
  SXMLFile xmlFile = r.read (fd);
  if (! xmlFile)
    return Sxmlelement (0);

  if (false) { // JMI
    gLogIOstream <<
      "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
      endl;
      
    xmlFile->print (gLogIOstream);

    gLogIOstream <<
      endl;
  }
  
  // get the xmlDecl
  TXMLDecl * xmlDecl = xmlFile->getXMLDecl ();
  
  if (false) { // JMI
    gLogIOstream <<
      "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
      "getXMLDecl =" <<
      endl;
    xmlDecl->print (gLogIOstream);
  }
  
  string fXMLVersion = xmlDecl->getVersion ();
  string fXMLEncoding = xmlDecl->getEncoding ();
  int     fXMLStandalone = xmlDecl->getStandalone ();

  const int fieldWidth = 17;
  
  gLogIOstream <<
    "XML Declaration:" <<
    endl;

  gIndenter++;
  
  gLogIOstream << left <<
    setw (fieldWidth) <<
    "fXMLVersion" << " = \"" << fXMLVersion << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fXMLEncoding" << " = \"" << fXMLEncoding << "\"" <<
    endl  <<
    setw (fieldWidth) <<
    "fXMLStandalone" << " = \"" << fXMLStandalone << "\"" <<
    endl <<
    endl;

  gIndenter--;

  // get the docType
  TDocType * docType = xmlFile->getDocType ();
  
  if (false) { // JMI
    gLogIOstream <<
      "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
      "getDocType =" <<
      endl;
    docType->print (gLogIOstream);
  }

  gLogIOstream <<
    "Document Type:" <<
    endl;

  gIndenter++;
  
  std::string fXMLStartElement = docType->getStartElement ();
  bool    fXMLPublic = docType->getPublic ();
  std::string fXMLPubLitteral = docType->getPubLitteral ();
  std::string fXMLSysLitteral = docType->getSysLitteral ();

  gLogIOstream << left <<
    setw (fieldWidth) <<
    "fXMLStartElement" << " = \"" << fXMLStartElement << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fXMLPublic" << " = \"" << fXMLPublic << "\"" <<
    endl  <<
    setw (fieldWidth) <<
    "fXMLPubLitteral" << " = \"" << fXMLPubLitteral << "\"" <<
    endl  <<
    setw (fieldWidth) <<
    "fXMLSysLitteral" << " = \"" << fXMLSysLitteral << "\"" <<
    endl <<
    endl;
    
  gIndenter--;

  // should the encoding be converted to UTF-8?
  string desiredEncoding = "UTF-8";
  
  if (fXMLEncoding != desiredEncoding) {
    gLogIOstream <<
      "Converting MusicXML data from \"" <<
      fXMLEncoding <<
      "\" to \"" <<
      desiredEncoding <<
      "\"" <<
      endl;
  }
    
  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 1",
    "build xmlelement tree from stdin",
    timingItem::kMandatory,
    startClock,
    endClock);

  // fetch mxmlTree
  Sxmlelement mxmlTree = xmlFile->elements ();

  return mxmlTree;
}

//_______________________________________________________________________________
EXP Sxmlelement musicXMLString2mxmlTree (
  const char*       buffer,
  S_musicXMLOptions mxmlOpts,
  indentedOstream&  logIOstream) 
{
  clock_t startClock = clock ();

  if (gTraceOptions->fTraceBasic) {
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
  
  SXMLFile xmlFile = r.readbuff (buffer);

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 1",
    "build xmlelement tree from buffer",
    timingItem::kMandatory,
    startClock,
    endClock);
  
  // fetch mxmlTree
  Sxmlelement mxmlTree = xmlFile->elements();

  return mxmlTree;
}


} // namespace
