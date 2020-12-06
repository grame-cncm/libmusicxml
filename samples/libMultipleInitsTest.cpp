/*

  Copyright (C) 2003-2008  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the MusicXML Library use.
*/

#include <fstream>      // setw()), set::precision(), ...
#include <sstream>

#include "libxml2ly.h"

#include "oahBasicTypes.h"

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

string xmlErrToString (xmlErr err)
{
  string result;

  switch (err) {
    case kNoErr:
      result = "noErr";
      break;
    case kInvalidFile:
      result = "invalidFile";
      break;
    case kInvalidOption:
      result = "invalidOption";
      break;
    case kUnsupported:
      result = "unsupported";
      break;
  } // switch

  return result;
}

//_______________________________________________________________________________
int main (int argc, char *argv[])
{
  // the executable name
  // ------------------------------------------------------
  string executableName = argv [0];

  cout << "Launching " << executableName << endl;

  // the options vector
  // ------------------------------------------------------
	optionsVector theOptionsVector;

//	theOptionsVector.push_back (make_pair ("-insider", ""));
	theOptionsVector.push_back (make_pair ("-trace-oah", ""));
//	theOptionsVector.push_back (make_pair ("-display-theOptionsVector-values", ""));
//	theOptionsVector.push_back (make_pair ("-global-staff-size", "30"));

//	theOptionsVector.push_back (make_pair ("-help", ""));
	theOptionsVector.push_back (make_pair ("-name-help", "global-staff-size"));
//	theOptionsVector.push_back (make_pair ("-hmxmlcg", ""));
//	theOptionsVector.push_back (make_pair ("-help-midi-group", ""));
//  cerr << "main - string2lily theOptionsVector: " << theOptionsVector.size() << ": " << theOptionsVector2string(theOptionsVector) << endl;

	theOptionsVector.push_back (make_pair ("-version", ""));

  displayOptionsVector (theOptionsVector, cerr);

  // first run
  // ------------------------------------------------------

  cerr << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" <<
    endl << endl;
	xmlErr
	  err1 =
      musicxmlfile2lilypond (
        "/Users/menu//libmusicxml-git/files/musicxml/basic/HelloWorld.xml",
        theOptionsVector,
        cout,
        cerr);
	cout << "xml2ly2 ret 1 = " << xmlErrToString (err1) << endl;

  // second run
  // ------------------------------------------------------

  cerr << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" <<
    endl << endl;

  xmlErr
    err2 =
      musicxmlfile2lilypond (
        "NO_FILE_NAME2",
        theOptionsVector,
        cout,
        cerr);
  cout << "xml2ly2 ret 2 = " << xmlErrToString (err2) << endl;

	return 0;
}
