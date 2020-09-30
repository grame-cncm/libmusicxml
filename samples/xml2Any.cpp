/*

  Copyright (C) 2003-2008  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the MusicXML Library use.
*/

#include <fstream>      // ifstream, ...
#include <sstream>

#include "libmusicxml.h"


using namespace std;
using namespace MusicXML2;

//_______________________________________________________________________________

static void string2StringsV (int argc, char *argv[], vector<string>& strings)
{
	for (int i=1; i<argc; i++) {
		strings.push_back (argv [i]);
	} // for
}

static void printOptions (optionsVector& options)
{
  cerr << "printOptions: options size: " << options.size() << endl;
	cerr << "==> options:" << endl;
	for (auto option: options) {
	  cerr << "   \"" << option.first << "\" \"" << option.second << "\"" << endl;
	} // for
	cerr << endl;
}

static bool args2Options (int argc, char *argv[], optionsVector& options)
{
	vector<string> args;
	string2StringsV (argc, argv, args);

  cerr << "args2Options: args size: " << args.size() << endl;
	cerr << "==> args:" << endl;
	for (auto str: args) {
	  cerr << "   " << str << endl;
	} // for
	cerr << endl;

	string curOption;

	for (int i = 0; i < args.size ()-1; i++) {
	  string str = args[i];

	  cerr << "--> curOption: " << curOption << endl;
	  cerr << "--> str      : " << str << endl;

		if (curOption.empty ()) {	// wait for option
			if (str [0] == '-') {
			  curOption = str;
      }
			else {
			  return false;
      }
		}

		else {
			if (str[0] == '-') {
				// option without value
				options.push_back (make_pair (curOption, ""));
				curOption = str;
			}
			else {
			  // option with value
				options.push_back (make_pair(curOption, str));
				curOption = "";
			}
		}

// JMI		printOptions (options);
	} // for

	if (curOption.size())
		options.push_back (make_pair (curOption, ""));

	return true;
}

//_______________________________________________________________________________
enum generatedCodeKind {
  kNoGeneratedCode,
  kLilyPond, kBrailleMusic, kMusicXML };

string generatedCodeKindAsString (generatedCodeKind kind)
{
  string result;

  switch (kind) {
    case kNoGeneratedCode:
      result = "*NoGeneratedCode*";
      break;
    case kLilyPond:
      result = "LilyPond";
      break;
    case kBrailleMusic:
      result = "BrailleMusic";
      break;
    case kMusicXML:
      result = "MusicXML";
      break;
  } // switch

  return result;
}

//_______________________________________________________________________________
generatedCodeKind gGeneratedCodeKind = kNoGeneratedCode;

void registerGeneratedCodeKind (generatedCodeKind kind)
{
  if (gGeneratedCodeKind != kNoGeneratedCode) {
    cerr << "only one of '-lilypond', '-braille' and '-musicxml' can be used" << endl;
    exit (2);
  }
  else {
    gGeneratedCodeKind = kind;
  }
}

//_______________________________________________________________________________
int main (int argc, char *argv[])
{
	optionsVector options;
	const char* file = 0;

	if (argc > 1)
		file = argv [argc - 1];
	else {
		cerr <<
		  "usage: " <<
		  argv [0] <<
		  " [-lilypond | -braille | -musicxml] [options] -|fileName" <<
		  endl;
		return -1;
	}

  // fetch the options from argc/argv
	args2Options (argc, argv, options);
	printOptions (options);

  // take generatedCodeKind options into account if any
	optionsVector optionsToKeep;

	for (auto option: options) {
	  if (option.first == "-lilypond") {
	    registerGeneratedCodeKind (kLilyPond);
	  }
	  else if (option.first == "-braille") {
	    registerGeneratedCodeKind (kBrailleMusic);
	  }
	  else if (option.first == "-musicxml") {
	    registerGeneratedCodeKind (kMusicXML);
	  }
	  else {
	    optionsToKeep.push_back (option);
	  }
	} // for
	printOptions (optionsToKeep);

  // the default is '-lilypond'
  if (gGeneratedCodeKind == kNoGeneratedCode) {
    gGeneratedCodeKind = kLilyPond;
  }

  // use insider options instead of the regular ones?
  bool insiderOptions = false;

  // should we generate LilyPond, braille music or MusicXML?
  cerr <<
    "==> generatedCodeKind: " <<
    generatedCodeKindAsString (gGeneratedCodeKind) <<
    endl;

  if (string (file) == "-") {
    xmlErr err = kNoErr;

    // MusicXML data comes from standard input
    switch (gGeneratedCodeKind) {
      case kNoGeneratedCode:
        // should not occur
        break;
      case kLilyPond:
        err = musicxmlfd2lilypond (
          stdin, insiderOptions, optionsToKeep, cout, cerr);
        break;
      case kBrailleMusic:
        err = musicxmlfd2braille (
          stdin, insiderOptions, optionsToKeep, cout, cerr);
        break;
      case kMusicXML:
        err = musicxmlfd2musicxml (
          stdin, insiderOptions, optionsToKeep, cout, cerr);
        break;
    } // switch

    cout << "xml2ly3 ret 1 = " << err << endl;
  }

  else {
    // MusicXML data comes from a file
    cerr << "read file '" << file << "'" << endl;
    std::ifstream t (file);
    std::stringstream buffer;
    buffer << t.rdbuf ();
  //	cerr << "==> buffer:" << endl << buffer.str () << endl;

    if (buffer.str ().size ()) {
      xmlErr err = kNoErr;

      switch (gGeneratedCodeKind) {
        case kNoGeneratedCode:
          // should not occur
          break;
        case kLilyPond:
          err = musicxmlstring2lilypond (
            buffer.str().c_str(), insiderOptions, optionsToKeep, cout, cerr);
          break;
        case kBrailleMusic:
          err = musicxmlstring2braille (
            buffer.str().c_str(), insiderOptions, optionsToKeep, cout, cerr);
          break;
        case kMusicXML:
          err = musicxmlstring2musicxml (
            buffer.str().c_str(), insiderOptions, optionsToKeep, cout, cerr);
          break;
      } // switch

      cout << "xml2ly3 ret 2 = " << err << endl;
    }

    else {
      cerr << "cannot read file '" << file << "'" << endl;
      // handle the optionsToKeep anyway, just for help

      xmlErr err = kNoErr;

      switch (gGeneratedCodeKind) {
        case kNoGeneratedCode:
          // should not occur
          break;
        case kLilyPond:
          err = musicxmlstring2lilypond (
            buffer.str().c_str(), insiderOptions, optionsToKeep, cout, cerr);
          break;
        case kBrailleMusic:
          err = musicxmlstring2braille (
            buffer.str().c_str(), insiderOptions, optionsToKeep, cout, cerr);
          break;
        case kMusicXML:
          err = musicxmlstring2musicxml (
            buffer.str().c_str(), insiderOptions, optionsToKeep, cout, cerr);
          break;
      } // switch

      cout << "xml2ly3 ret 3 = " << err << endl;
      return -1;
    }
  }

	return 0;
}
