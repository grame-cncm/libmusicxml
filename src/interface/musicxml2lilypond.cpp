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
#include "xml2lilypondvisitor.h"

#include "versions.h"

using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
EXP int         musicxml2lilypondVersion()    { return 010; }
EXP const char* musicxml2lilypondVersionStr() { return "0.1.0"; }

//_______________________________________________________________________________
/*
 * The method that converts the file contents to LilyPond code
 * and  writes the result to the output stream
*/
static xmlErr xml2lilypond(
  SXMLFile& xmlfile,
  translationSwitches ts,
  ostream& out,
  const char* file) 
{
  // build xmlelement tree from the file contents
  Sxmlelement st = xmlfile->elements();
  
  if (st) {
    // create an xml2lilypondvisitor
    xml2lilypondvisitor v(ts);
    
    // use the visitor to convert the xmlelement tree into a lilypondelement tree
    if (ts.fTrace)
      cerr << "Launching conversion of xmlelement tree to lilypondelement tree" << endl;
    Slilypondelement    ly = v.convertToLilyPond(st);
    
    string separator = "%----------------------------------------";
    if (ts.fTrace) 
      cerr << 
        "Outputting LilyPond source code" << endl <<
        separator << endl << endl;

    // output the general information about the conversion
    out << "%{" << std::endl;
    if (file) { // USER
      out <<
        "  LilyPond code converted from \"" << file << "\"" << std::endl;
    }
    else 
      out << 
        "  LilyPond code converted from standard input" << std::endl; 
    out << 
      "  using libmusicxml2" << 
      //<< musicxmllibVersionStr() <<
      " and its embedded xml2lilypond converter " <<
      //<< musicxml2lilypondVersionStr() <<
      std::endl <<
      "  Options were: ";
    if (ts.fSelectedOptions.size() == 0)
      out << "none";
    else
      out << std::endl << "    " << ts.fSelectedOptions;
    out <<
      std::endl <<
      "%}" << 
      std::endl << std::endl;
      
    // output the lilypondelement tree resulting from the conversion 
    // thru lilypondelement::printLilyPondCode()
    out << ly << endl;
    
    if (ts.fTrace) 
      cerr << separator << endl;
   
    return kNoErr;
  }
  return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfile2lilypond(
  const char *file, translationSwitches ts, ostream& out) 
{
  if (ts.fTrace) cerr << "Building xmlemement tree from \"" << file << "\"" << endl;
  xmlreader r;
  SXMLFile xmlfile;
  xmlfile = r.read(file);
  
  if (xmlfile) {
    return xml2lilypond(xmlfile, ts, out, file);
  }
  
  return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfd2lilypond(
  FILE * fd, translationSwitches ts, ostream& out) 
{
  if (ts.fTrace) cerr << "Building xmlemement tree from standard input" << endl;
 xmlreader r;
  SXMLFile xmlfile;
  xmlfile = r.read(fd);
  
  if (xmlfile) {
    return xml2lilypond(xmlfile, ts, out, 0);
  }
  
  return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlstring2lilypond(
  const char * buffer, translationSwitches ts, ostream& out) 
{
  if (ts.fTrace) cerr << "Building xmlemement tree from a buffer" << endl;
  xmlreader r;
  SXMLFile xmlfile;
  xmlfile = r.readbuff(buffer);
  
  if (xmlfile) {
    return xml2lilypond(xmlfile, ts, out, 0);
  }
  
  return kInvalidFile;
}

}
