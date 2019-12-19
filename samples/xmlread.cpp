/*

  Copyright (C) 2003-2008  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the MusicXML Library use.
*/

#ifdef WIN32
# pragma warning (disable : 4786)
#endif

#include <stdlib.h>
#include <iostream>
#include <fstream>
#ifndef WIN32
#include <libgen.h>
#include <signal.h>
#include <string.h>
#endif

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

using namespace std;
using namespace MusicXML2;

//#define STRREAD

static void usage (char* name)
{
#ifndef WIN32
	const char* tool = basename (name);
#else
	const char* tool = name;
#endif
	cerr << "usage: " << tool << " [options] <musicxml file>" << endl;
	cerr << "       read a musicxml file and re-write it" << endl;
	cerr << "       options:" << endl;
	cerr << "           	-o <file>: output file name" << endl;
	cerr << "       writes to standard output when -o option is missing." << endl;
	exit(1);
}


static const char* getOption (int argc, char *argv[], const std::string& option, const char* defaultValue)
{
	for (int i = 1; i < argc - 1; i++) {
		if (option == argv[i])
			return argv[i + 1];
	}
	return defaultValue;
}

static const char* getFile (int argc, char *argv[])
{
	return argv[argc-1];
}

#ifndef WIN32
//-------------------------------------------------------------------------------
// signal handlers
//-------------------------------------------------------------------------------
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
#ifdef STRREAD
static char * read(const char* file)
{
	FILE * fd = fopen(file, "rb");
	if (!fd) return 0;
	fseek(fd, 0, SEEK_END);
	long size = ftell (fd);
	char * buff = (char*)malloc(size+1);
	fseek(fd, 0, SEEK_SET );
	fread(buff, size, 1, fd);
	buff[size] = 0;
	fclose(fd);
	return buff;
}
#endif

//_______________________________________________________________________________
int main(int argc, char *argv[]) 
{
	catchsigs();

	const char* outfile = getOption (argc, argv, "-o", 0);
	if ((outfile && (argc != 4)) && (argc != 2))
		usage (argv[0]);
	const char* filename = getFile (argc, argv);

	ostream* out;
	if (outfile)
		out = new ofstream (outfile);
	else out = &cout;

	xmlreader r;
	SXMLFile file = r.read(filename);
	if (file) {
//		Sxmlelement st = file->elements();
		file->print(*out);
		*out << endl;
	}
	else return -1;		// likely a syntax error

	return 0;
}
