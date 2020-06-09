/*

  Copyright (C) 2003-2008  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the MusicXML Library use.
*/

#include <iostream>
#include <fstream>
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
	cerr << "       options: --autobars don't generates barlines" << endl;
	cerr << "                --version print version and exit" << endl;
    cerr << "                -o file   : write output to file" << endl;
    cerr << "                --begin <int>   : starting measure number (default: 0)" << endl;
    cerr << "                --end <int>   : end on measure (default: infinite)" << endl;
	cerr << "                --part <int>   : The part number to convert. 0 for all. (default: 0)" << endl;
	cerr << "                -h --help : print this help" << endl;
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
static char* fileOpt(int argc, char *argv[], const string& option)
{
	for (int i=1; i<argc;i++) {
		if (option == argv[i]) return (++i < argc) ? argv[i] : 0;
	}
	return 0;
}

static int intOpt(int argc, char *argv[], const string& option)
{
    for (int i=1; i<argc;i++) {
        if (option == argv[i]) return (++i < argc) ? atoi(argv[i]) : 0;
    }
    return 0;
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
	if (checkOpt (argc, argv, "-h") || checkOpt (argc, argv, "--help")) usage();
    
    int beginMeasure = intOpt(argc, argv, "--begin"),
        endMeasure = intOpt(argc, argv, "--end"),
        partFilter = intOpt(argc, argv, "--part");

	bool generateBars = !checkOpt (argc, argv, "--autobars");
	const char * file = argv[argc-1];
	const char * outfile = fileOpt (argc, argv, "-o");
	ostream * out = &cout;
	fstream fout;
	if (outfile) {
		fout.open (outfile, ios_base::out);
		if (!fout.is_open()) {
			cerr << "can't open output file " << outfile << endl;
			return -2;
		}
		out = &fout;
	}

	xmlErr err = kNoErr;
	if (!strcmp(file, "-"))
		err = musicxmlfd2guido(stdin, generateBars, beginMeasure, endMeasure, partFilter, *out);
	else
		err = musicxmlfile2guido(file, generateBars, beginMeasure, endMeasure, partFilter, *out);
	if (err == kUnsupported)
		cerr << "unsupported xml format" << endl;
	else if (err ) {
		cerr << "conversion failed" << endl;
		return -1;
	}

	if (outfile) fout.close ();
	return 0;
}
