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
#ifndef WIN32
#include <signal.h>
#include <string.h>
#endif

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

using namespace std;
using namespace MusicXML2;

//#define STRREAD

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
	
	for (int i=1; i<argc; i++) {
		xmlreader r;
#ifdef STRREAD
		char * buff = read(argv[i]);
		SXMLFile file = r.readbuff(buff);
#else
		SXMLFile file = r.read(argv[i]);
#endif
		if (file) {
			Sxmlelement st = file->elements();
			file->print(cout);
			cout << endl;
		}
		else return -1;		// likely a syntax error
	}
	return 0;
}
