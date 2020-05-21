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

float 	libMusicXMLAdapter::musicxml2lilypondVersion()		{ return ::musicxml2lilypondVersion(); }
string 	libMusicXMLAdapter::musicxml2lilypondVersionStr()	{ return ::musicxml2lilypondVersionStr(); }

float 	libMusicXMLAdapter::musicxml2brailleVersion()		{ return ::musicxml2brailleVersion(); }
string 	libMusicXMLAdapter::musicxml2brailleVersionStr()	{ return ::musicxml2brailleVersionStr(); }

string	libMusicXMLAdapter::string2guido(const string& buff, bool generateBars)
{
	stringstream sstr;
	return ::musicxmlstring2guido (buff.c_str(), generateBars, sstr) ? "" : sstr.str();
}

static void string2StringV (const string& args, vector<string> strings)
{
	string str = args;
	while (true) {
		size_t pos = str.find_first_of (' ', 0);
		if (pos == string::npos) break;
		if (pos > 1) strings.push_back(str.substr(0, pos));
		str = str.substr(pos+1, string::npos);
	}
}

static bool args2Options (const string& argsStr, optionsVector& options)
{
	vector<string> args;
	string2StringV (argsStr, args);
cerr << "args2Options: vstring size: " << args.size() << endl;
	string curOption;
	for (auto str: args) {
		if (curOption.empty()) {	// wait for option
			if (str[0] == '-') curOption = str;
			else return false;
		}
		else {
			if (str[0] == '-') {
				// option without value
				options.push_back (make_pair(curOption, ""));
				curOption = str;
			}
			else {
				options.push_back (make_pair(curOption, str));
				curOption.clear();
			}
		}
	}
	return true;
}

static string args2String (const string& args)
{
	stringstream sstr;
	for (auto str: args)
		sstr << str << " ";
	return sstr.str();
}

static string options2String (const optionsVector& options)
{
	stringstream sstr;
	for (auto p: options)
		sstr << p.first << " " << p.second << " ";
	return sstr.str();
}

string	libMusicXMLAdapter::string2lily(const string& buff, const string& args)
{
cerr << "libMusicXMLAdapter::string2lily args: " << args << endl;
	stringstream sstr;
	optionsVector options;
	if (!args2Options (args, options)) {
		cerr << "musicxmlstring2braille: bad option sequence: " << args << endl;
		return "";
	}
cerr << "libMusicXMLAdapter::string2lily options: " << options.size() << ": " << options2String(options) << endl;
	return ::musicxmlstring2lilypond (buff.c_str(), options, sstr) ? "" : sstr.str();
}

string	libMusicXMLAdapter::string2braille(const string& buff, const string& args)
{
	stringstream sstr;
	optionsVector options;
	if (!args2Options (args, options)) {
		cerr << "musicxmlstring2braille: bad option sequence: " << args << endl;
		return "";
	}
cerr << "libMusicXMLAdapter::musicxmlstring2braille options: " << options.size() << ": " << options2String(options)  << endl;
	return ::musicxmlstring2braille (buff.c_str(), options, sstr) ? "" : sstr.str();
}

string	libMusicXMLAdapter::xmlStringTranspose(const string& buff, int interval)
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
