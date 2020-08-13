#include "libmusicxml.h"

using namespace std;

using namespace MusicXML2;


int main (int argc, char *argv[])
{
//	xmlErr err = musicxmlstring2guido ("", false, cout);

	optionsVector options;
  xmlErr        err;

//	options.push_back (make_pair ("-t=oah", ""));
	options.push_back (make_pair ("-t=passes", ""));

  indentedOstream outIndentedOstream (cout, indenter::gIndenter);
  indentedOstream errIndentedOstream (cerr, indenter::gIndenter);

  if (false) {
  	options.push_back (make_pair ("-help", ""));
  	err = musicxmlstring2lilypond ("", options, outIndentedOstream, errIndentedOstream);
    }
  else {
    err = musicxmlstring2lilypond ("basic/HelloWorld.xml", options, outIndentedOstream, errIndentedOstream);
  }

//	err = musicxmlstring2braille ("", options, cout, cerr);
//	err = musicxmlstring2musicxml ("", options, cout, cerr);

  if (err) {
  	cout << "err = " << err << endl;
  	return err;
  }

	return 0;
}
