/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iostream>
#include <sstream>
#include <string>

#include "adapter.h"
#include "libmusicxml.h"
#include "xml.h"
#include "xmlreader.h"
#include "xml_tree_browser.h"
#include "transposition.h"

using namespace std;
using namespace MusicXML2;

float 	libMusicXMLAdapter::libVersion()		{ return ::musicxmllibVersion(); }
string 	libMusicXMLAdapter::libVersionStr()		{ return ::musicxmllibVersionStr(); }


float 	libMusicXMLAdapter::musicxml2guidoVersion()		{ return ::musicxml2guidoVersion(); }
string 	libMusicXMLAdapter::musicxml2guidoVersionStr()	{ return ::musicxml2guidoVersionStr(); }


string	libMusicXMLAdapter::string2guido(const std::string& buff, bool generateBars)
{
	stringstream sstr;
	return ::musicxmlstring2guido (buff.c_str(), generateBars, sstr) ? "" : sstr.str();
}

string	libMusicXMLAdapter::xmlStringTranspose(const std::string& buff, int interval)
{
	std::ostringstream oss2;
    xmlreader r;
    SXMLFile xmlfile;
    xmlfile = r.readbuff(buff.c_str());
    Sxmlelement elts = xmlfile->elements();
    transposition t(interval);
    xml_tree_browser tb(&t);
    tb.browse(*elts);
    xmlfile->print(oss2);
    return oss2.str();
}
