/*

  MusicXML Library
  Copyright (C) 2006  Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#include <iostream>
#include "xmlfile.h"
#include "xmlvisitor.h"
#include "tree_browser.h"

using namespace std; 
using namespace MusicXML2; 
namespace MusicXML2 
{

//______________________________________________________________________________
SXMLFile TXMLFile::create ()  { TXMLFile* o = new TXMLFile; assert(o!=0); return o; }

//______________________________________________________________________________
TDocType::TDocType (const string start) : fStartElement(start), fPublic(true) 
{
	fPubLitteral = "-//Recordare//DTD MusicXML 2.0 Partwise//EN";
	if (start == "score-partwise") {
		fSysLitteral = "http://www.musicxml.org/dtds/partwise.dtd";
	}
	else if (start == "score-timewise") {
		fSysLitteral = "http://www.musicxml.org/dtds/timewise.dtd";
	}
}

//______________________________________________________________________________
void TDocType::print (ostream& s) 
{
	s	<< endl << "<!DOCTYPE " << fStartElement
		<< (fPublic ? " PUBLIC " : " SYSTEM ") 
		<< "\"" << fPubLitteral << "\"\n\t\t\t\"" 
		<< fSysLitteral << "\">";	
}

//______________________________________________________________________________
void TXMLDecl::print (ostream& s) 
{
	s	<< "<?xml version=\"" << fVersion << "\"";
	if (!fEncoding.empty())			s << " encoding=\"" << fEncoding  << "\"";
	if (fStandalone != kundefined)	s << " standalone=\"" << (fStandalone ? "yes" : "no") << "\"";
	s << "?>";
}

//______________________________________________________________________________
void TXMLFile::print (ostream& stream) 
{
	if (fXMLDecl) fXMLDecl->print(stream);
	if (fDocType) fDocType->print(stream);
	xmlvisitor v(stream);
	tree_browser<xmlelement> browser(&v);
	browser.browse(*elements());
}

}
