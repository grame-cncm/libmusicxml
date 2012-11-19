/*

  MusicXML Library
  Copyright (C) 2003-2009  Grame

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

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <iostream>
#include "libmusicxml.h"
#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"
#include "xml2guidovisitor.h"

using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
static xmlErr xml2guido(SXMLFile& xmlfile, bool generateBars, ostream& out, const char* file) 
{
	Sxmlelement st = xmlfile->elements();
	if (st) {
		xml2guidovisitor v(true, true, generateBars);
		Sguidoelement gmn = v.convert(st);
		if (file) {
			out << "(*\n  gmn code converted from '" << file << "'"
				<< "\n  using libmusicxml v." << musicxmllibVersionStr();
		}
		else out << "(*\n  gmn code converted using libmusicxml v." << musicxmllibVersionStr();
		out << "\n  and the embedded xml2guido converter v." << musicxml2guidoVersionStr()
			<< "\n*)" << endl;
		out << gmn << endl;
		return kNoErr;
	}
	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfile2guido(const char *file, bool generateBars, ostream& out) 
{
	xmlreader r;
	SXMLFile xmlfile;
	xmlfile = r.read(file);
	if (xmlfile) {
		return xml2guido(xmlfile, generateBars, out, file);
	}
	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfd2guido(FILE * fd, bool generateBars, ostream& out) 
{
	xmlreader r;
	SXMLFile xmlfile;
	xmlfile = r.read(fd);
	if (xmlfile) {
		return xml2guido(xmlfile, generateBars, out, 0);
	}
	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlstring2guido(const char * buffer, bool generateBars, ostream& out) 
{
	xmlreader r;
	SXMLFile xmlfile;
	xmlfile = r.readbuff(buffer);
	if (xmlfile) {
		return xml2guido(xmlfile, generateBars, out, 0);
	}
	return kInvalidFile;
}

}
