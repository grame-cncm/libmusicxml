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
	MSDL lexical definition.
*/

#include <iostream>

#include <stdio.h>

#include "msdlParse.hpp"

using namespace std;


#define YY_NO_UNISTD_H

extern int msdllval;

static int utf16 = 0;
static int bigendian = 1;
static int start = 1;

static int wgetc(FILE * fd) {
	int c = getc(fd);
	if (start) {
		if (c == 0xff) {
			utf16 = 1; bigendian = 0;
			getc(fd); c = getc(fd);
		}
		else if (c == 0xfe) {
			utf16 = 1; bigendian = 1;
			getc(fd); c = getc(fd);
		}
		start = 0;
	}
	if (utf16) {
		if (bigendian) c = getc(fd);
		else getc(fd);
	}

	return c;
}

static size_t wfread (void * buf, size_t size, size_t nmemb, FILE * fd) {
	char * ptr = (char *)buf;
	size_t n=0;
	while (nmemb--) {
		*ptr++ = wgetc(fd);
		if (feof(fd) || ferror(fd) ) break;
		n++;
	}
	return n;
}

void lexinit(FILE* fd) {
	utf16 = 0;
	bigendian = 1;
	start = 1;
	yyrestart(fd);
}

void lexend() {
	if (YY_CURRENT_BUFFER) {
		msdl_delete_buffer (YY_CURRENT_BUFFER);
	}
}

#define getc	wgetc
#define fread	wfread
#define register		// to get rid of the -Wdeprecated-register

%}


/* ---------------------------------------------------------------------- */
/* Regular expressions */
/* ---------------------------------------------------------------------- */

letter		               [a-zA-Z™ΎΟ‚Ξ®ηθιλκτ]
digit		                 [0-9]

space		                 [ \t]
endOfLine                [\x0a\x0d]
spaceOrEndOfLine         [ \t\x0a\x0d]

firstchar	               [a-zA-Z_]
namechar	               [a-zA-Z0-9_]

singleQuotedStringChar   [^']
doubleQuotedStringChar   [^"]


/* ---------------------------------------------------------------------- */
/* Options */
/* ---------------------------------------------------------------------- */

%option yylineno
%option never-interactive
%option nounput

%S PARENTHESIZED_COMMENT COMMENT_TO_END_OF_LINE


%%	/* Les terminaux du langage et les actions correspondantes */


%{
/* Variables locales  "yylex" */

bool gTraduire =  true;

void AppondreAuSourceCumule ();
%}


"%{"						  {
                      BEGIN PARENTHESIZED_COMMENT;
                  }

<PARENTHESIZED_COMMENT>"%}"	{
                      BEGIN INITIAL;
                  }
<PARENTHESIZED_COMMENT>([^%]|"%"[^}])*	{
										if (gTraduire)
											cout << "PARENTHESIZED_COMMENT: \"" << yytext << "\"" << endl << endl;

                    return PARENTHESIZED_COMMENT;
                  }


"%"				    		{
										AppondreAuSourceCumule ();

										BEGIN COMMENT_TO_END_OF_LINE;
									}

<COMMENT_TO_END_OF_LINE>{endOfLine}	{
										AppondreAuSourceCumule ();

//										CompterFinDeLigne ();

										BEGIN INITIAL;
									}

<COMMENT_TO_END_OF_LINE>.*	{
										AppondreAuSourceCumule ();

										if (gTraduire)
											cout << "COMMENT_TO_END_OF_LINE: \"" << yytext << "\"" << endl << endl;

										/*
										on accepte tout caractere qui n'est pas
										une {endOfLine}
										*/
									}


{firstchar}{namechar}* {
										if (gTraduire)
											cout << "IDENTIFIER: \"" << yytext << "\"" << endl << endl;

                    return IDENTIFIER;
                  }

{digit}+ {
										if (gTraduire)
											cout << "INTEGER: " << yytext << "" << endl << endl;

                    return INTEGER;
                  }

{space}+					{
										if (gTraduire)
											cout << "SPACES: \"" << yytext << "\"" << endl << endl;

                    return SPACES;
                  }

{endOfLine}	      {
										if (gTraduire)
											cout << "END_OF_LINE" << endl << endl;

                    return END_OF_LINE;
                  }

"'"{singleQuotedStringChar}*"'" {
										if (gTraduire)
											cout << "QUOTED_STRING: \"" << yytext << "\"" << endl << endl;

                    return SINGLE_QUOTED_STRING;
                  }

"\""{doubleQuotedStringChar}*"\"" {
										if (gTraduire)
											cout << "QUOTED_STRING: \"" << yytext << "\"" << endl << endl;

                    return DOUBLE_QUOTED_STRING;
                  }


"="               {
										if (gTraduire)
											cout << "EQUAL_SIGN" << endl << endl;

                    return EQUAL_SIGN;
                  }

"{"               {
										if (gTraduire)
											cout << "LEFT_BRACKET" << endl << endl;

                    return LEFT_BRACKET;
                  }

"}"               {
										if (gTraduire)
											cout << "RIGHT_BRACKET" << endl << endl;

                    return RIGHT_BRACKET;
                  }

"|"               {
										if (gTraduire)
											cout << "END_OF_BAR: \"" << yytext <<  "\"" << endl << endl;

                    return END_OF_BAR;
                  }

{spaceOrEndOfLine}	{
                    /* extra spaceOrEndOfLine is ignored */
                  }

.        {
										if (gTraduire)
											cout << "*** OTHER_CHARACTER ***: \"" << yytext << "\"" << endl << endl;

                    return OTHER_CHARACTER;
                  }

<<EOF>>		        {
                    yyterminate();
                  }


%%


void AppondreAuSourceCumule ()
{
}
