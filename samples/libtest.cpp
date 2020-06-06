#include "libmusicxml.h"

using namespace std;

using namespace MusicXML2;


int main (int argc, char *argv[])
{
//	xmlErr err = musicxmlstring2guido ("", false, cout);

	optionsVector options;

//	options.push_back (make_pair ("-t=oah", ""));
//	options.push_back (make_pair ("-t=passes", ""));
	options.push_back (make_pair ("-help", ""));

//	xmlErr err = musicxmlstring2lilypond ("", options, cout, cerr);
//	xmlErr err = musicxmlstring2braille ("", options, cout, cerr);
	xmlErr err = musicxmlstring2musicxml ("", options, cout, cerr);

  if (err) {
  	cout << "err = " << err << endl;
  	return err;
  }

	return 0;
}
