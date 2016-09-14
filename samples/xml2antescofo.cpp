/*

  Copyright (C) 2013 Thomas Coffy

  This file is provided as an example of the MusicXML Library use.
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <stdlib.h>
#include <string.h>
#include "libmusicxml.h"

using namespace std;
using namespace MusicXML2;

void usage() {
	cerr << "usage: musicxml2antescofo [options]  <musicxml file>" << endl;
	cerr << "       reads stdin when <musicxml file> is '-'" << endl;
	cerr << "       option: --autobars don't generates barlines" << endl;
	exit(1);
}

//_______________________________________________________________________________
int main(int argc, char *argv[]) 
{
	bool generateBars = true;
	char * file = argv[1];
	if (argc == 3) {
		if (!strcmp(argv[1], "--autobars")) {
			generateBars = false;
			file = argv[2];
		}
		else usage();
	}
	else if (argc != 2) usage();

	xmlErr err = kNoErr;
	if (!strcmp(file, "-"))
		err = musicxmlfd2antescofo(stdin, generateBars, cout);
	else
		err = musicxmlfile2antescofo(file, generateBars, cout);
	if (err) {
		cout << "conversion failed" << endl;
	}
	return 0;
}
