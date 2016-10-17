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
#include "xml_tree_browser.h"

#include "versions.h"
#include "msr.h"
#include "musicxml2lpsr.h"
#include "xml2MsrScoreVisitor.h"


using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
EXP int         musicxml2MsrVersionNbr () { return 010; }
EXP const char* musicxml2MsrVersionStr () { return "0.1.0"; }

//_______________________________________________________________________________
void initializeStringToMsrNoteNamesLanguage ()
{
  gMsrNoteNamesLanguageMap["dutch"] =     kNederlands;
  gMsrNoteNamesLanguageMap["catalan"] =   kCatalan;
  gMsrNoteNamesLanguageMap["deutsch"] =   kDeutsch;
  gMsrNoteNamesLanguageMap["english"] =   kEnglish;
  gMsrNoteNamesLanguageMap["espanol"] =   kEspanol;
  gMsrNoteNamesLanguageMap["italiano"] =  kItaliano;
  gMsrNoteNamesLanguageMap["francais"] =  kFrancais;
  gMsrNoteNamesLanguageMap["norsk"] =     kNorsk;
  gMsrNoteNamesLanguageMap["portugues"] = kPortugues;
  gMsrNoteNamesLanguageMap["suomi"] =     kSuomi;
  gMsrNoteNamesLanguageMap["svenska"] =   kSvenska;
  gMsrNoteNamesLanguageMap["vlaams"] =    kVlaams;
}

MsrNoteNamesLanguage getMsrNoteNamesLanguage (std::string lang)
{
  return gMsrNoteNamesLanguageMap[lang];
}

//_______________________________________________________________________________
S_translationSettings translationSettings::create()
{
  translationSettings* o = new translationSettings(); assert(o!=0);
  return o;
}

translationSettings::translationSettings() {}
translationSettings::~translationSettings() {}

//_______________________________________________________________________________

S_lpsrScore buildMsrScoreFromTree (
  S_translationSettings& ts,
  Sxmlelement            xmlTree)
{
    // browse the part contents for the first time with a xml2MsrScoreVisitor
  if (ts->fTrace)
    cerr << "Building a MSR score from the MusicXML tree" << endl;

  // create an xml2MsrVisitor
  xml2MsrScoreVisitor visitor (ts);
      
  // a global variable is needed so that lpsr::Element.print() 
  // can decide whether to print:
  //   - the MSR structure
  //   - MusicXML text
  //   - LilyPond source code
  // use the visitor to build a MSR score from the xmlelement tree
  // choosing kMSR for the trace of the score build
  lpsrGlobalVariables::setCodeGenerationKind (lpsrGlobalVariables::kMSR);

  S_lpsrScore score =
    visitor.buildMsrScoreFromXMLElementTree (xmlTree);

  string separator = "%----------------------------------------";

  // output the score resulting from the conversion 
  // thru lpsrElement::printMsrStructure()
  if (ts->fTrace) 
    cerr << 
      separator << endl <<
      "%Outputting the MSR ccore" << endl <<
      separator << endl;
  
  // choosing kMSR to print the score
  lpsrGlobalVariables::setCodeGenerationKind (lpsrGlobalVariables::kMSR);
   
  if (ts->fTrace) cerr << "{%" << std::endl;
  cerr << score;
  if (ts->fTrace) cerr << "%}" << std::endl;
  
  cerr << separator << std::endl;

  return score;
}

//_______________________________________________________________________________
/*
 * The method that converts the file contents to LilyPond code
 * and  writes the result to the output stream
*/
static xmlErr xml2Msr(
  SXMLFile&              xmlfile,
  S_translationSettings& ts,
  ostream&               out,
  const char* file) 
{
  // build xmlelement tree from the file contents
  Sxmlelement elemsTree = xmlfile->elements();
  
  if (elemsTree) {
    S_lpsrScore score =
      buildMsrScoreFromTree (ts, elemsTree);
      
    /*
    // create an xml2MsrVisitor
    xml2MsrVisitor v(ts);
    
    // use the visitor to convert the xmlelement tree into a lpsrElement tree
    if (ts->fTrace)
      cerr << "Launching conversion of xmlelement tree to LilyPond semantic representation tree" << endl;
    S_lpsrElement    lpsr = v.convertToMsr(elemsTree);
    
    string separator = "%----------------------------------------";
 
    // output the structure of the lpsrElement tree resulting from the conversion 
    // thru lpsrElement::printMsrStructure()
    if (ts->fTrace) 
      cerr << 
        separator << endl <<
        "%Outputting the LilyPond semantic representation tree" << endl <<
        separator << endl;
    
    // a global variable is needed so that lpsr::Element.print() 
    // can decide whether to print:
    //   - the MSR structure
    //   - MusicXML text
    //   - LilyPond source code
    lpsrGlobalVariables::setCodeGenerationKind (lpsrGlobalVariables::kMSR);
    if (ts->fTrace) cerr << "{%" << std::endl;
    std::cerr << 
      lpsr << std::endl;
    if (ts->fTrace) cerr << "%}" << std::endl;
    std::cerr << endl << separator << std::endl;

    if (ts->fTrace) 
      cerr << 
        "%Outputting the LilyPond source code" << endl <<
        separator << endl << endl;

    // output the general information about the conversion
    out << "%{" << std::endl;
    if (file) {
      out <<
        "  LilyPond code converted from \"" << file << "\"" << std::endl;
    }
    else 
      out << 
        "  LilyPond code converted from standard input" << std::endl; 
    out << 
      "  using libmusicxml2" << //<< musicxmllibVersionStr() <<
      " and its embedded xml2lilypond converter " << //<< musicxml2lpsrVersionStr() <<
      std::endl;
      
    cout << "  Options were: ";
    if (ts->fSelectedOptions.size() == 0)
      out << "none";
    else
      out << std::endl << "    " << ts->fSelectedOptions;
    out << std::endl;
    out << "%}" << std::endl << std::endl;
      
    // output the LilyPond semantic representation tree resulting from the conversion 
    lpsrGlobalVariables::setCodeGenerationKind (lpsrGlobalVariables::kLilypondCode);
    out <<
      lpsr << endl;
    
    if (ts->fTrace) 
      cerr << separator << endl;
   */
   
    return kNoErr;

  }

  return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlFile2Msr(
  const char             *file,
  S_translationSettings& ts,
  ostream&               out) 
{
  if (ts->fTrace) cerr << "Building xmlemement tree from \"" << file << "\"" << endl;

  xmlreader r;
  SXMLFile  xmlFile;

  xmlFile = r.read(file);
  
  if (xmlFile) {
    return xml2Msr (xmlFile, ts, out, file);
  }
  
  return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlFd2Msr(
  FILE                   *fd,
  S_translationSettings& ts,
  ostream&               out) 
{
  if (ts->fTrace) cerr << "Building xmlemement tree from standard input" << endl;

  xmlreader r;
  SXMLFile  xmlFile;

  xmlFile = r.read(fd);
  
  if (xmlFile) {
    return xml2Msr (xmlFile, ts, out, 0);
  }
  
  return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlString2Msr(
  const char             *buffer,
  S_translationSettings& ts,
  ostream&               out) 
{
  if (ts->fTrace) cerr << "Building xmlemement tree from a buffer" << endl;
  
  xmlreader r;
  SXMLFile  xmlFile;
  
  xmlFile = r.readbuff(buffer);
  
  if (xmlFile) {
    return xml2Msr (xmlFile, ts, out, 0);
  }
  
  return kInvalidFile;
}

}
