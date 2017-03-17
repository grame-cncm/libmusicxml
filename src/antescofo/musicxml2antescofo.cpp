/*
  Copyright (C) 2013 Thomas Coffy

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  thomas.coffy@gmail.com
  http://repmus.ircam.fr/antescofo
*/


#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <iostream>
#include "libmusicxml.h"
#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"
#include "xml2antescofovisitor.h"
#include "antescofowriter.h"

using namespace std;

namespace MusicXML2 
{


	

//_______________________________________________________________________________
static xmlErr xml2antescofo(SXMLFile& xmlfile, bool generateBars, ostream& out, const char* file) 
{
	Sxmlelement st = xmlfile->elements();
	if (st) {
		antescofowriter w;

		xml2antescofovisitor v(w, true, true, generateBars);
		Santescofoelement as = v.convert(st);
		if (file) {
			out << "; Antescofo partition converted from '" << file << "'" << endl
				<< ";  using libmusicxml v." << musicxmllibVersionStr() << endl;
		}
		else out << ";  Antescofo code converted using libmusicxml v." << musicxmllibVersionStr() << endl;
		out << "; and the embedded xml2antescofo converter v." << musicxml2antescofoVersionStr() << endl;
		//out << as << endl;

		w.print();
		return kNoErr;
	}
	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfile2antescofo(const char *file, bool generateBars, ostream& out) 
{
	xmlreader r;
	SXMLFile xmlfile;
	xmlfile = r.read(file);
	if (xmlfile) {
		return xml2antescofo(xmlfile, generateBars, out, file);
	}
	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfd2antescofo(FILE * fd, bool generateBars, ostream& out) 
{
	xmlreader r;
	SXMLFile xmlfile;
	xmlfile = r.read(fd);
	if (xmlfile) {
		return xml2antescofo(xmlfile, generateBars, out, 0);
	}
	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlstring2antescofo(const char * buffer, bool generateBars, ostream& out) 
{
	xmlreader r;
	SXMLFile xmlfile;
	xmlfile = r.readbuff(buffer);
	if (xmlfile) {
		return xml2antescofo(xmlfile, generateBars, out, 0);
	}
	return kInvalidFile;
}

}
