/*

  Copyright (C) 2003-2008  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the MusicXML Library use.
*/

#include <fstream>      // setw()), set::precision(), ...
#include <sstream>

#include "libmusicxml.h"

using namespace std;
using namespace MusicXML2;

//_______________________________________________________________________________

static void string2StringsV (int argc, char *argv[], vector<string>& strings)
{
	for (int i=1; i<argc; i++) {
		strings.push_back(argv[i]);
	}
}

static bool args2Options (int argc, char *argv[], optionsVector& options)
{
	vector<string> args;
	string2StringsV (argc, argv, args);
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
				options.push_back (make_pair (curOption, ""));
				curOption = str;
			}
			else {
				options.push_back (make_pair(curOption, str));
				curOption = "";
			}
		}
	}
	if (curOption.size())
		options.push_back (make_pair (curOption, ""));
	return true;
}

//_______________________________________________________________________________
int main (int argc, char *argv[])
{
	optionsVector options;
	options.push_back (make_pair ("-help", ""));
	xmlErr err = musicxmlstring2lilypond ("", options, cout, cerr);
	cout << "xml2ly2 ret=" << err;
	return 0;
}

//int main (int argc, char *argv[])
//{
//	optionsVector options;
//	const char* file = 0;
//	if (argc > 1)
//		file = argv[argc-1];
//	else {
//		cerr << "usage: xml2ly2 [options] file" << endl;
//		return -1;
//	}
//	args2Options (argc, argv, options);
//cerr << "read file " << file << endl;
//	std::ifstream t(file);
//	std::stringstream buffer;
//	buffer << t.rdbuf();
//	if (buffer.str().size()) {
//		xmlErr err = musicxmlstring2lilypond (buffer.str().c_str(), options, cout, cerr);
//		cerr << "done result: " << err << endl;
//	}
//	else {
//		cerr << "cannot read file " << file << endl;
//		xmlErr err = musicxmlstring2lilypond ("", options, cout, cerr);
//		return -1;
//	}
//	return 0;
//}

