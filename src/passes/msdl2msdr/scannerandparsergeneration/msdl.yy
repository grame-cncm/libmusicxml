%{
/*
  Basic xml grammar definition
  This is a basic definition of the xml grammar necessary to cover
  the MusicXML format. It is a simplified form based on the XML document
  grammar as defined in
  "XML in a nutshell - 2nd edition" E.R.Harold and W.S.Means,
  O'Reilly, June 2002, pp:366--371
*/


#include <stdio.h>
#include <string.h>
#include <iostream>

#include "msdrTokens.h"
#include "msdlFlexLexer.cpp"
  // the lexical analyzer code, including things such as
  // yy_scan_string, yy_delete_buffer, msdlrestart and BEGIN(INITIAL);


class reader
{
	public:
				 reader() {}
		virtual ~reader() {};

		virtual void	error (const char* s, int lineno) = 0;
};


int yyline = 1;

#ifdef __cplusplus
extern "C" {
#endif
//int		yyparse (void);
void	yyerror(const char *s);
int		msdlwrap();
bool	readfile   (const char * file, reader * r);
bool	readstream (FILE * file, reader * r);
bool	readbuffer (const char * buffer, reader * r);
#ifdef __cplusplus
}
#endif

int   yylex (void);
extern char * msdltext;
extern int msdllineno;
extern FILE * msdlin;

//#define YYERROR_VERBOSE
//#define ERROR(str)	{ yyerror(str); YYABORT; }

reader * gReader;

static void init (reader * r) {
	gReader = r;
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


%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.7.4"
%defines


%define api.token.raw

// JMI %define api.token.constructor
// JMI %define api.value.type variant
%define parse.assert

%code requires {
  #include <string>

  using namespace std;

  class msdlDriver;

//  msdlDriver & drv;
}

// The parsing context.
%param {msdlDriver & drv}

%locations

%define parse.trace
%define parse.error detailed
%define parse.lac full

%code {
  #include "msdlDriver.h"
}


/* Description du terminal courant */
/* ------------------------------- */

%union {
	int  								  fInteger;
	double								fDouble;
	string*								fIdentifier;
	string*								fString;
	}


%token
  SPACES                  _("spaces")
  END_OF_LINE             _("end of line")

  PARENTHESIZED_COMMENT   _("parenthesized comment")
  COMMENT_TO_END_OF_LINE  _("comment to end of line")

  EQUAL_SIGN              "="

  COMMA                   ","
  COLON                   ";"
  SEMI_COLON              ":"

  LEFT_PARENTHESIS        "("
  RIGHT_PARENTHESIS       ")"

  LEFT_BRACKET            "["
  RIGHT_BRACKET           "]"

  DOT                     "."

  MEASURE                 "|"
  DOUBLE_BAR              "||"

  INTEGER                 _("integer")
  DOUBLE                  _("double")

  SINGLE_QUOTED_STRING    _("single quoted string")
  DOUBLE_QUOTED_STRING    _("double quoted string")

  IDENTIFIER              _("identifier")

  OTHER_CHARACTER         _("other character")
;

%start description


%%              /* beginning of rules section */

description	:
  declarations
    { cout << "description" << endl; }
;

declarations :
  declarations declaration
| declaration
;

declaration :
  other
    { cout << "declaration" << endl; }
;

other :
  "spaces"
| "end of line"

| "parenthesized comment"
| "comment to end of line"

| "="

| ","
| ":"
| ";"

| "("
| ")"

| "["
| "]"

| "."

| "|"
| "||"

| "integer"
| "double"

| "single quoted string"
| "double quoted string"

| "identifier"

| "other character"
;



%%

/* ---------------------------------------------------------------------- */
/* Service code                                                           */
/* ---------------------------------------------------------------------- */

#define yy_delete_buffer	msdl_delete_buffer
#define yy_scan_string		msdl_scan_string

bool readbuffer (const char * buffer, reader * r)
{
	if (!buffer) return false;		// error for empty buffers

	init(r);
	YY_BUFFER_STATE b;
    // Copy string into new buffer and Switch buffers
    b = yy_scan_string (buffer);
    // Parse the string
	int ret = yyparse();
    // Delete the new buffer
//	yy_delete_buffer(b);
	BEGIN(INITIAL);
 	return ret==0;
}

bool readfile (const char * file, reader * r)
{
	FILE * fd = fopen (file, "r");
	if (!fd) {
		cerr << "can't open file '" << file << "'" << endl;
		return false;
	}
	init(r);
//	msdlrestart(fd);
	msdlin = fd;
 	int ret = yyparse();
 	fclose (fd);
	BEGIN(INITIAL);
 	return ret==0;
}

bool readstream (FILE * fd, reader * r)
{
	if (!fd) return false;
	init(r);
//	msdlrestart(fd);
	msdlin = fd;
 	int ret = yyparse();
	BEGIN(INITIAL);
 	return ret==0;
}

void	yyerror(const char *s)	{ gReader->error (s, msdllineno); }


#ifdef PARSER_MAIN

class testreader : public reader
{
	public:
		void	error (const char* s, int lineno) {
			cerr << s  << " on line " << lineno << endl;
		}
};


int main (int argc, char * argv [])
{
  testreader r;

  switch (argc) {
    case 0:
  		return readstream (stdin, &r) ? 0 : 1;
      break;
    case 1:
  		return readfile (argv [1], &r) ? 0 : 1;
      break;
    default:
      ;
  } // switch

  std::cout << "MSDL LEXICAL AND SYNTACTICAL ANALYSIS SUCCEEDED." << std::endl;
  return 0;
}

#endif

