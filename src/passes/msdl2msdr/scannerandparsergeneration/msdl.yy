%{

/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

/*
	MSDL grammar definition.
*/


#include <stdio.h>
#include <string.h>
#include <iostream>

#include "msdlReader.h"

#include "msdlFlexLexer.h"


using namespace std;

int yyline = 1;

#ifdef __cplusplus
extern "C" {
#endif

//int		yyparse (void);
void	yyerror(const char *s);

int		msdlwrap();

bool	readMsdlFile   (const char * file, msdlReader * theMsdlReader);
bool	readMsdlStream (FILE * file, msdlReader * theMsdlReader);
bool	readMsdlBuffer (const char * buffer, msdlReader * theMsdlReader);

#ifdef __cplusplus
}
#endif

int   yylex (void);
extern char * msdltext;
extern int msdllineno;
extern FILE * msdlin;

#define YYERROR_VERBOSE
#define ERROR(str)	{ yyerror(str); YYABORT; }
#define MAXLEN	1024
#define VLEN	256
char attributeName[MAXLEN];
char attributeVal[MAXLEN];

char msdlversion[VLEN];
char msdlencoding[MAXLEN];
int msdlStandalone = -1;

char eltName[MAXLEN];
char doctypeStart[MAXLEN];
char doctypePub[MAXLEN];
char doctypeSys[MAXLEN];

msdlReader * gMsdlReader;

static void init (msdlReader * theMsdlReader) {
	gMsdlReader = theMsdlReader;
	msdlStandalone = -1;
	eltName[0]		= 0;
	attributeName[0] = 0;
	attributeVal[0] = 0;
	msdlversion[0]   = 0;
	msdlencoding[0]  = 0;
	doctypeStart[0]	= 0;
	doctypePub[0]	= 0;
	doctypeSys[0]	= 0;
}

static char * unquote (char * text) {
	int n = strlen(text);
	if (n > 0) {
		text[n-1]=0;
		return &text[1];
	}
	return text;
}

static void store (char * dst, const char * text) {
	strcpy (dst, text);
}

int		msdlwrap()		{ return(1); }

%}


%start Description

%token
  PARENTHESIZED_COMMENT COMMENT_TO_END_OF_LINE

  LEFT_BRACKET RIGHT_BRACKET

  DOT

  SCORE
  PART_GROUP
  GROUP
  STAFF
  VOICE
  FRAGMENT

  ANACRUSIS

  CLEF
  TREBLE
  ALTO
  TENOR
  BARYTON
  BASS

  KEY

  TIME

  KEYWORD

  NAME

  INTEGER DOUBLE

  SINGLE_QUOTED_STRING DOUBLE_QUOTED_STRING

  SPACES END_OF_LINE

  EQUAL_SIGN

  COMMA COLON SEMI_COLON

  END_OF_MEASURE

  DOUBLE_BAR
  FINAL_BAR

  OTHER_CHARACTER
;


%% /* beginning of rules section */


Description	: CommentsOrOthers ;

CommentsOrOthers 	:
  CommentOrOther
| CommentsOrOthers CommentOrOther;

CommentOrOther	:
  PARENTHESIZED_COMMENT | COMMENT_TO_END_OF_LINE
| NAME
| KEYWORD
| SINGLE_QUOTED_STRING | DOUBLE_QUOTED_STRING
| SPACES


%%


/* ---------------------------------------------------------------------- */
/* Service code                                                           */
/* ---------------------------------------------------------------------- */

#define yy_delete_buffer	msdl_delete_buffer
#define yy_scan_string		msdl_scan_string

bool readMsdlBuffer (const char * buffer, msdlReader * theMsdlReader)
{
	if (! buffer) return false;		// error for empty buffers

	init (theMsdlReader);
	YY_BUFFER_STATE b;

  // Copy string into new buffer and Switch buffers
  b = yy_scan_string (buffer);

  // Parse the string
	int ret = yyparse ();

    // Delete the new buffer
	yy_delete_buffer (b);

	BEGIN(INITIAL);
 	return ret==0;
}

bool readMsdlFile (const char * file, msdlReader * theMsdlReader)
{
	FILE * fd = fopen (file, "r");
	if (!fd) {
		cerr << "can't open file '" << file << "'" << endl;
		return false;
	}
	init (theMsdlReader);
	msdlrestart(fd);
	msdlin = fd;

  int ret;

#ifdef LEX_ONLY

  FlexLexer* lexer = new yyFlexLexer;
  while (lexer->yylex () != 0);
  ret = 0;

#else

 	ret = yyparse();

#endif


 	fclose (fd);
// JMI	BEGIN(INITIAL);
 	return ret==0;
}

bool readMsdlStream (FILE * fd, msdlReader * theMsdlReader)
{
	if (! fd) return false;

	init( theMsdlReader);

	msdlrestart (fd);
	msdlin = fd;

 	int ret = yyparse();

// JMI	BEGIN(INITIAL);
 	return ret==0;
}

void	yyerror(const char *s)	{ gMsdlReader->error (s, msdllineno); }


// the reader class
//______________________________________________________________________________
class testMsdlReader : public msdlReader
{
	public:

		bool	                msdlDecl (
		                        const char* version, const char *encoding, int standalone)
		                          {
                                cout << "msdlDecl: " << version << " " << encoding << " " << standalone << endl;
                                return true;
                              }

		bool	                docType (
		                        const char* start, bool status, const char *pub, const char *sys)
		                          {
                                cout << "docType: " << start << " " << (status ? "PUBLIC" : "SYSTEM") << " " << pub << " " << sys << endl;
                                return true;
                              }

		bool	newElement (const char* eltName) {
			cout << "newElement: " << eltName << endl;
			return true;
		}
		bool	newAttribute (const char* eltName, const char *val) {
			cout << "    newAttribute: " << eltName << "=" << val << endl;
			return true;
		}
		void	setValue (const char* value) {
			cout << "  -> value: " << value << endl;
		}
		bool	endElement (const char* eltName) {
			cout << "endElement: " << eltName << endl;
			return true;
		}
		void	error (const char* s, int lineno) {
			cerr << s  << " on line " << lineno << endl;
		}
};


#ifdef PARSER_MAIN

// the main() function
//______________________________________________________________________________
int main (int argc, char * argv [])
{
	if (argc > 1) {
		testMsdlReader theMsdlReader;

		return
		  readMsdlFile (argv[1], &theMsdlReader)
		    ? 0
		    : 1;
	}

 	return 0;
}

#endif
