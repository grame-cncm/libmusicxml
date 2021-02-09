%{
/* ---------------------------------------------------------------------- */
/* Code global to "yylex"                                                 */
/* ---------------------------------------------------------------------- */

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

/*
From FlexLexer.h:

// This file defines FlexLexer, an abstract class which specifies the
// external interface provided to flex C++ lexer objects, and yyFlexLexer,
// which defines a particular lexer class.
//
// If you want to create multiple lexer classes, you use the -P flag
// to rename each yyFlexLexer to some other xxFlexLexer.  You then
// include <FlexLexer.h> in your other sources once per lexer class:
//
//      #undef yyFlexLexer
//      #define yyFlexLexer xxFlexLexer
//      #include <FlexLexer.h>
//
//      #undef yyFlexLexer
//      #define yyFlexLexer zzFlexLexer
//      #include <FlexLexer.h>
//      ...
*/

#include <iomanip>
#include <iostream>

// JMI #include "msdlKeywords.h"

#include "msdlBisonParser.hpp"


using namespace std;
// JMI using namespace MusicXML2;


#define YY_NO_UNISTD_H

extern int msdllval;

/* JMI
static int utf16     = 0;
static int bigendian = 1;
static int start     = 1;
*/

/* ---------------------------------------------------------------------- */
/* The current token description                                          */
/* ---------------------------------------------------------------------- */

/* JMI

Äunion msdrTokenDescription
{
	double							    fDoubleNumber;
	string						    	fIdent;
	string						    	fString;
};

*/

#undef yyFlexLexer
#define yyFlexLexer msdlFlexLexer
// NO JMI #include <FlexLexer.h>

bool gTraduire =  true;

int returnToken (
  int    inputLineNumber,
  string tokenText,
  int    tokenNumber)
{
  // write a trace of the token?
  if (gTraduire) {
    const unsigned int lineNumberWidth =  4;
    const unsigned int tokenNameWidth  = 23;

    cout <<
      "--> line " <<
      right <<
      setw (lineNumberWidth) <<
      inputLineNumber <<
      ": ";

    bool doWriteTokenText = tokenText.size () > 0;

    cout <<
      left <<
      setw (tokenNameWidth);

    switch (tokenNumber) {
      case PARENTHESIZED_COMMENT:
        cout << "PARENTHESIZED_COMMENT";
        break;
      case COMMENT_TO_END_OF_LINE:
        cout << "COMMENT_TO_END_OF_LINE";
        break;

      case LEFT_BRACKET:
        cout << "LEFT_BRACKET";
        doWriteTokenText = false;
        break;
      case RIGHT_BRACKET:
        cout << "RIGHT_BRACKET";
        doWriteTokenText = false;
        break;

      case DOT:
        cout << "DOT";
        doWriteTokenText = false;
        break;

      case KEYWORD:
        cout << "KEYWORD";
        break;

      case NAME:
        cout << "NAME";
        break;

      case INTEGER:
        cout << "INTEGER";
        break;
      case DOUBLE:
        cout << "DOUBLE";
        break;

      case SINGLE_QUOTED_STRING:
        cout << "SINGLE_QUOTED_STRING";
        break;
      case DOUBLE_QUOTED_STRING:
        cout << "DOUBLE_QUOTED_STRING";
        break;

      case SPACES:
        cout << "SPACES";
        break;

      case END_OF_LINE:
        cout << "END_OF_LINE";
        doWriteTokenText = false;
        break;

      case EQUAL_SIGN:
        cout << "EQUAL_SIGN";
        doWriteTokenText = false;
        break;

      case COMMA:
        cout << "COMMA";
        doWriteTokenText = false;
        break;
      case COLON:
        cout << "COLON";
        doWriteTokenText = false;
        break;
      case SEMI_COLON:
        cout << "SEMI_COLON";
        doWriteTokenText = false;
        break;

      case END_OF_MEASURE:
        cout << "END_OF_MEASURE";
        doWriteTokenText = false;
        break;

      case DOUBLE_BAR:
        cout << "DOUBLE_BAR";
        doWriteTokenText = false;
        break;

      case OTHER_CHARACTER:
        cout << "OTHER_CHARACTER";
        break;

      default:
        ; // don't care, those are flex internals
    } // switch

    if (doWriteTokenText) {
      cout << left <<
        "\"" << tokenText << "\"";
    }

    cout << endl;
    }

  return tokenNumber;
}

%}


/* ---------------------------------------------------------------------- */
/* Options                                                                */
/* ---------------------------------------------------------------------- */

%option c++

%option 8bit

%option nodefault

%option warn
%option nounput nomain noinput noyywrap yylineno

%option prefix="msdl"

%option header-file="msdlFlexLexer.h"


%{
%}


/* ---------------------------------------------------------------------- */
/* Regular expressions                                                    */
/* ---------------------------------------------------------------------- */

letter		               [a-zA-ZˆŽ™Œ¾Ï‚Î®çèéëêô]
firstchar	               [a-zA-Z_]
namechar	               [a-zA-Z0-9_]
name                     {firstchar}{namechar}*

digit		                 [0-9]
decimalInteger           {digit}+

space		                 [ \t]
spaces                   {space}+

endOfLine                [\x0a\x0d]
spaceOrEndOfLine         [ \t\x0a\x0d]

singleQuotedStringChar   [^']
doubleQuotedStringChar   [^"]

exponent			 					 [eE][+-]?{decimalInteger}


/* ---------------------------------------------------------------------- */
/* Start conditions                                                       */
/* ---------------------------------------------------------------------- */

%S PARENTHESIZED_COMMENT_MODE COMMENT_TO_END_OF_LINE_MODE


%{
  // Code run each time a pattern is matched.
// JMI  # define YY_USER_ACTION  loc.columns (yyleng);
%}


%%	/* The language terminals and corresponding semantic actions */


%{
  /* -------------------------------------------------------------------- */
  /* Code local to "yylex"                                                */
  /* -------------------------------------------------------------------- */
%}


"%{"			{
            BEGIN PARENTHESIZED_COMMENT_MODE;
          }

<PARENTHESIZED_COMMENT_MODE>"%}"	{
            BEGIN INITIAL;
          }

<PARENTHESIZED_COMMENT_MODE>([^%]|"%"[^}])*	{
            return
              returnToken (lineno (), YYText (), PARENTHESIZED_COMMENT);
          }


"%"				{
            BEGIN COMMENT_TO_END_OF_LINE_MODE;
          }

<COMMENT_TO_END_OF_LINE_MODE>{endOfLine} {
            BEGIN INITIAL;
          }

<COMMENT_TO_END_OF_LINE_MODE>.*	{
            return
              returnToken (lineno (), YYText (), COMMENT_TO_END_OF_LINE);
          }

{name}    {
/*
            msdlKeywordKind
              keywordKind =
                msdlKeywordKindFromString (YYText ());

            if (keywordKind == k_NoMsdlKeywordKind) {
              return
                returnToken (lineno (), YYText (), NAME);
            }
            else {
              return
                returnToken (lineno (), YYText (), KEYWORD);
            }
*/
            return
              returnToken (lineno (), YYText (), NAME);
          }

{decimalInteger} {
            return
              returnToken (lineno (), YYText (), INTEGER);
          }

{decimalInteger}"."{decimalInteger}({exponent})? |
{decimalInteger}({exponent})?	{
            // yylval.fNombre = atof (yytext);
            return
              returnToken (lineno (), YYText (), DOUBLE);
          }

{spaces}  {
            return
              returnToken (lineno (), YYText (), SPACES);
          }

{endOfLine}	{
            return
              returnToken (lineno (), YYText (), END_OF_LINE);
          }

"'"{singleQuotedStringChar}*"'" {
            return
              returnToken (lineno (), YYText (), SINGLE_QUOTED_STRING);
          }

"\""{doubleQuotedStringChar}*"\"" {
            return
              returnToken (lineno (), YYText (), DOUBLE_QUOTED_STRING);
          }


"."       {
            return
              returnToken (lineno (), YYText (), DOT);
          }

","       {
            return
              returnToken (lineno (), YYText (), COMMA);
          }

"="       {
            return
              returnToken (lineno (), YYText (), EQUAL_SIGN);
          }

":"       {
            return
              returnToken (lineno (), YYText (), COLON);
          }

";"       {
            return
              returnToken (lineno (), YYText (), SEMI_COLON);
          }

"{"       {
            return
              returnToken (lineno (), YYText (), LEFT_BRACKET);
          }

"}"       {
            return
              returnToken (lineno (), YYText (), RIGHT_BRACKET);
          }

"||"      {
            return
              returnToken (lineno (), YYText (), DOUBLE_BAR);
          }

"|"       {
            return
              returnToken (lineno (), YYText (), END_OF_MEASURE);
          }

.         {
            return
              returnToken (lineno (), YYText (), OTHER_CHARACTER);
          }

<<EOF>>		{
            yyterminate();
          }


%%


/* ---------------------------------------------------------------------- */
/* Service code                                                           */
/* ---------------------------------------------------------------------- */

#ifdef LEXER_MAIN

int main (void)
{
  msdlFlexLexer lexer;

  lexer.switch_streams (&std::cin, &std::cout);

#ifdef LEX_ONLY
  // JMI
#endif

  while (lexer.yylex ());

  std::cout << "MSDL LEXICAL ANALYSIS SUCCEEDED." << std::endl;
  return 0;
}

#endif

