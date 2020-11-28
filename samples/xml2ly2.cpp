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
/* JMI
static string options2string (const optionsVector& options)
{
	stringstream sstr;
	for (auto p: options)
		sstr << "'" << p.first << "': '" << p.second << "' ";
	return sstr.str();
}
*/

//_______________________________________________________________________________
int main (int argc, char *argv[])
{
  // the executable name
  // ------------------------------------------------------
  string executableName = argv [0];

	optionsVector options;

//	options.push_back (make_pair ("-insider", ""));
	options.push_back (make_pair ("-trace-oah", ""));
//	options.push_back (make_pair ("-display-options-values", ""));
//	options.push_back (make_pair ("-global-staff-size", "30"));

//	options.push_back (make_pair ("-help", ""));
	options.push_back (make_pair ("-name-help", "global-staff-size"));
//	options.push_back (make_pair ("-hmxmlcg", ""));
//	options.push_back (make_pair ("-help-midi-grp", ""));
//  cerr << "main - string2lily options: " << options.size() << ": " << options2string(options) << endl;

  cerr << endl << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl << endl;

	xmlErr err = musicxmlstring2lilypond ("", options, cout, cerr);
	cout << "xml2ly2 ret 1 = " << err << endl;

  if (true) {
    cerr << endl << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl << endl;

    xmlErr err2 = musicxmlstring2lilypond ("", options, cout, cerr);
    cout << "xml2ly2 ret 2 = " << err2 << endl;
  }

	return 0;
}
