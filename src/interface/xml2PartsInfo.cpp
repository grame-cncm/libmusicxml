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

#include "msrGeneralOptions.h"

#include "xml2PartsInfo.h"
#include "xml2PartsInfoExtractor.h"

using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
static S_xmlPartsInfo xml2PartsInfo (
  SXMLFile&              xmlfile,
  S_xmlPartsInfoOptions& partsInfoOptions,
  ostream&               os,
  const char*            file) 
{
  // build xmlelement tree from the file contents
  Sxmlelement elemsTree = xmlfile->elements();

  S_xmlPartsInfo partsInfo;
  
  if (elemsTree) {
    partsInfo =
      extractPartsInfoFromXMLElementTree (
        partsInfoOptions, elemsTree);

    if (partsInfoOptions->fDisplayPartsInfo)
      displayXmlPartsInfo (
        partsInfoOptions, partsInfo, os);
  }

  return partsInfo;
}

//_______________________________________________________________________________
EXP S_xmlPartsInfo musicxmlFile2PartsInfo (
  const char*            file,
  S_xmlPartsInfoOptions& partsInfoOptions,
  ostream&               os) 
{
  if (gGeneralOptions->fTrace) {
    string separator =
      "%------------------------------------------------------";
    
    cerr <<
      endl <<
      idtr << separator <<
      endl <<
      "Pass 1: building xmlemement tree from \"" << file << "\"" <<
      endl <<
      idtr << separator <<
      endl;
  }

  xmlreader r;
  SXMLFile  xmlFile;

  xmlFile = r.read (file);
  
  S_xmlPartsInfo partsInfo;

  if (xmlFile)
    partsInfo =
      xml2PartsInfo (
        xmlFile, partsInfoOptions, os, file);
  
  return partsInfo;
}

//_______________________________________________________________________________
EXP S_xmlPartsInfo musicxmlFd2PartsInfo (
  FILE*                  fd,
  S_xmlPartsInfoOptions& partsInfoOptions,
  ostream&               os) 
{
  if (gGeneralOptions->fTrace) {
    string separator =
      "%---------------------------------------------------";
    
    cerr <<
      endl <<
      idtr << separator <<
      endl <<
      "Pass 1: building xmlemement tree from standard input" <<
      endl <<
      idtr << separator <<
      endl;
  }

  xmlreader r;
  SXMLFile  xmlFile;

  xmlFile = r.read (fd);
  
  S_xmlPartsInfo partsInfo;

  if (xmlFile) {
    partsInfo =
      xml2PartsInfo (
        xmlFile, partsInfoOptions, os, 0);
  }
  
  return partsInfo;
}

//_______________________________________________________________________________
EXP S_xmlPartsInfo musicxmlString2PartsInfo (
  const char*            buffer,
  S_xmlPartsInfoOptions& partsInfoOptions,
  ostream&               os) 
{
  if (gGeneralOptions->fTrace) {
    string separator =
      "%-----------------------------------------------------";
    
    cerr <<
      endl <<
      idtr << separator <<
      endl <<
      idtr << "Pass 1: building xmlemement tree from a buffer" <<
      endl <<
      idtr << separator <<
      endl;
  }
  
  xmlreader r;
  SXMLFile  xmlFile;
  
  xmlFile = r.readbuff (buffer);
  
  S_xmlPartsInfo partsInfo;

  if (xmlFile) {
    partsInfo =
      xml2PartsInfo (
        xmlFile, partsInfoOptions, os, 0);
  }
  
  return partsInfo;
}

//_______________________________________________________________________________
S_xmlPartsInfo extractPartsInfoFromXMLElementTree (
  S_xmlPartsInfoOptions& partsInfoOptions,
  Sxmlelement            xmlTree)
{
  if (gGeneralOptions->fTrace) {
    string separator =
      "%----------------------------------------------";
    
    cerr <<
      endl <<
      idtr << separator <<
      endl <<
      "Pass 2: building a MSR from the xmlelement tree" <<
      endl <<
      idtr << separator <<
      endl;
  }
  
  // create an xml2PartsInfoVisitor
  xml2PartsInfoExtractor extractor (partsInfoOptions);

  // build the MSR score
  S_xmlPartsInfo
    partsInfo =
      extractor.extractPartsInfoFromXMLElementTree (xmlTree);

  return partsInfo;
}

//_______________________________________________________________________________
void displayXmlPartsInfo (
  S_xmlPartsInfoOptions& partsInfoOptions,
  S_xmlPartsInfo         partsInfo,
  ostream&               os)
{
  if (gGeneralOptions->fTrace) {
    string separator =
      "%----------------------------------------------";
    
    cerr <<
      endl <<
      idtr << separator <<
      endl <<
      "Optional pass: outputting the parts info" <<
      endl <<
      idtr << separator <<
      endl;
      // ------------------------------------------------------
  }
   
  if (gGeneralOptions->fTrace) os << "%{" << std::endl;
  
  os << partsInfo;
  
  if (gGeneralOptions->fTrace) os << "%}" << std::endl;
}


}
