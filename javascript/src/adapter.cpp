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
