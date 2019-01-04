/*

  Copyright (C) 2003-2008  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the MusicXML Library use.
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <string>

#include <stdlib.h>
#include <string.h>
#ifndef WIN32
#include <signal.h>
#endif

#include "libmusicxml.h"

using namespace std;
using namespace MusicXML2;

static void usage() {
	cerr << "usage: xml2guido [options]  <musicxml file>" << endl;
	cerr << "       reads stdin when <musicxml file> is '-'" << endl;
	cerr << "       option: --autobars don't generates barlines" << endl;
	cerr << "       option: --version print version and exit" << endl;
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
static bool checkOpt(int argc, char *argv[], const string& option)
{
	for (int i=1; i<argc;i++) {
		if (option == argv[i]) return true;
	}
	return false;
}

//_______________________________________________________________________________
static void versionInfo()
{
	cout << "xml2guido version " << musicxml2guidoVersionStr() << endl;
	cout << "Using libmusicxml version " << musicxmllibVersionStr() << endl;
	exit (0);
}

//_______________________________________________________________________________
int main(int argc, char *argv[])
{
	catchsigs();

	bool version = checkOpt (argc, argv, "--version") || checkOpt (argc, argv, "-v");
	if (version) versionInfo();

	if (argc < 2) usage();

	bool generateBars = checkOpt (argc, argv, "--autobars");
	char * file = argv[argc-1];

	xmlErr err = kNoErr;
	if (!strcmp(file, "-"))
		err = musicxmlfd2guido(stdin, generateBars, cout);
	else
		err = musicxmlfile2guido(file, generateBars, cout);
	if (err == kUnsupported)
		cerr << "unsupported xml format" << endl;
	else if (err == kUnsupported) {
		cerr << "conversion failed" << endl;
		return -1;
	}
	return 0;
}
