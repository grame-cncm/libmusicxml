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
EXP int         musicxml2LpsrVersionNbr () { return 010; }
EXP const char* musicxml2LpsrVersionStr () { return "0.1.0"; }

//_______________________________________________________________________________
void initializeStringToLpsrNoteNamesLanguage ()
{
  gLpsrNoteNamesLanguageMap["dutch"] =     kNederlands;
  gLpsrNoteNamesLanguageMap["catalan"] =   kCatalan;
  gLpsrNoteNamesLanguageMap["deutsch"] =   kDeutsch;
  gLpsrNoteNamesLanguageMap["english"] =   kEnglish;
  gLpsrNoteNamesLanguageMap["espanol"] =   kEspanol;
  gLpsrNoteNamesLanguageMap["italiano"] =  kItaliano;
  gLpsrNoteNamesLanguageMap["francais"] =  kFrancais;
  gLpsrNoteNamesLanguageMap["norsk"] =     kNorsk;
  gLpsrNoteNamesLanguageMap["portugues"] = kPortugues;
  gLpsrNoteNamesLanguageMap["suomi"] =     kSuomi;
  gLpsrNoteNamesLanguageMap["svenska"] =   kSvenska;
  gLpsrNoteNamesLanguageMap["vlaams"] =    kVlaams;
}

LpsrNoteNamesLanguage getLpsrNoteNamesLanguage (std::string lang)
{
  return gLpsrNoteNamesLanguageMap[lang];
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

S_lpsrScore buildLpsrScoreFromTree (
  S_translationSettings& ts,
  Sxmlelement            xmlTree)
{
    // browse the part contents for the first time with a xml2LpsrScoreVisitor
  if (ts->fTrace)
    cerr << "Building a LPSR score from the MusicXML tree" << endl;

  // create an xml2LpsrVisitor
  xml2LpsrScoreVisitor visitor (ts);
      
  // a global variable is needed so that lpsr::Element.print() 
  // can decide whether to print:
  //   - the LPSR structure
  //   - MusicXML text
  //   - LilyPond source code
  // use the visitor to build a LPSR score from the xmlelement tree
  // choosing kLPSR for the trace of the score build
  lpsrGlobalVariables::setCodeGenerationKind (lpsrGlobalVariables::kLPSR);

  S_lpsrScore score =
    visitor.buildMsrScoreFromXMLElementTree (xmlTree);

  string separator = "%----------------------------------------";

  // output the score resulting from the conversion 
  // thru lpsrElement::printLpsrStructure()
  if (ts->fTrace) 
    cerr << 
      separator << endl <<
      "%Outputting the LPSR ccore" << endl <<
      separator << endl;
  
  // choosing kLPSR to print the score
  lpsrGlobalVariables::setCodeGenerationKind (lpsrGlobalVariables::kLPSR);
   
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
static xmlErr xml2Lpsr(
  SXMLFile&              xmlfile,
  S_translationSettings& ts,
  ostream&               out,
  const char* file) 
{
  // build xmlelement tree from the file contents
  Sxmlelement elemsTree = xmlfile->elements();
  
  if (elemsTree) {
    S_lpsrScore score =
      buildLpsrScoreFromTree (ts, elemsTree);
      
    /*
    // create an xml2LpsrVisitor
    xml2LpsrVisitor v(ts);
    
    // use the visitor to convert the xmlelement tree into a lpsrElement tree
    if (ts->fTrace)
      cerr << "Launching conversion of xmlelement tree to LilyPond semantic representation tree" << endl;
    S_lpsrElement    lpsr = v.convertToLpsr(elemsTree);
    
    string separator = "%----------------------------------------";
 
    // output the structure of the lpsrElement tree resulting from the conversion 
    // thru lpsrElement::printLpsrStructure()
    if (ts->fTrace) 
      cerr << 
        separator << endl <<
        "%Outputting the LilyPond semantic representation tree" << endl <<
        separator << endl;
    
    // a global variable is needed so that lpsr::Element.print() 
    // can decide whether to print:
    //   - the LPSR structure
    //   - MusicXML text
    //   - LilyPond source code
    lpsrGlobalVariables::setCodeGenerationKind (lpsrGlobalVariables::kLPSR);
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
EXP xmlErr musicxmlFile2Lpsr(
  const char             *file,
  S_translationSettings& ts,
  ostream&               out) 
{
  if (ts->fTrace) cerr << "Building xmlemement tree from \"" << file << "\"" << endl;

  xmlreader r;
  SXMLFile  xmlFile;

  xmlFile = r.read(file);
  
  if (xmlFile) {
    return xml2Lpsr (xmlFile, ts, out, file);
  }
  
  return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlFd2Lpsr(
  FILE                   *fd,
  S_translationSettings& ts,
  ostream&               out) 
{
  if (ts->fTrace) cerr << "Building xmlemement tree from standard input" << endl;

  xmlreader r;
  SXMLFile  xmlFile;

  xmlFile = r.read(fd);
  
  if (xmlFile) {
    return xml2Lpsr (xmlFile, ts, out, 0);
  }
  
  return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlString2Lpsr(
  const char             *buffer,
  S_translationSettings& ts,
  ostream&               out) 
{
  if (ts->fTrace) cerr << "Building xmlemement tree from a buffer" << endl;
  
  xmlreader r;
  SXMLFile  xmlFile;
  
  xmlFile = r.readbuff(buffer);
  
  if (xmlFile) {
    return xml2Lpsr (xmlFile, ts, out, 0);
  }
  
  return kInvalidFile;
}

}
