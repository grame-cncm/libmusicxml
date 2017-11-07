/*

  Copyright (C) 2003-2008  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the MusicXML Library use.
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <stdlib.h>
#include <string.h>
#ifndef WIN32
#include <signal.h>
#include <string.h>
#endif

#include "libmusicxml.h"

using namespace std;
using namespace MusicXML2;

static void usage() {
	cerr << "usage: musicxml2guido [options]  <musicxml file>" << endl;
	cerr << "       reads stdin when <musicxml file> is '-'" << endl;
	cerr << "       option: --autobars don't generates barlines" << endl;
	exit(1);
}

#ifndef WIN32

static void _sigaction(int signal, siginfo_t *si, void *arg)
{
    cerr << "Signal #" << signal << " catched!" << endl;
    exit(-2);
}

static void catchsigs()
{
	struct sigaction sa;

    memset(&sa, 0, sizeof(struct sigaction));
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = _sigaction;
    sa.sa_flags   = SA_SIGINFO;
    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGILL, &sa, NULL);
    sigaction(SIGFPE, &sa, NULL);
}

#else
static void catchsigs()	{}
#endif

//_______________________________________________________________________________
int main(int argc, char *argv[]) 
{
	catchsigs();

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
		err = musicxmlfd2guido(stdin, generateBars, cout);
	else
		err = musicxmlfile2guido(file, generateBars, cout);
	if (err) {
		cout << "conversion failed" << endl;
		return -1;
	}
	return 0;
}
