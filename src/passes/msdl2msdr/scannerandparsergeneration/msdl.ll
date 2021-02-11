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

#include "msdrKeywords.h"
#include "msdrTokens.h"

// NO JMI #include <FlexLexer.h>
#include "msdlBisonParser.hpp"


// JMI using namespace std;
using namespace MusicXML2;


//#define YY_NO_UNISTD_H

// JMI extern int msdllval;


/* ---------------------------------------------------------------------- */
/* The current token description                                          */
/* ---------------------------------------------------------------------- */

#undef yyFlexLexer
#define yyFlexLexer msdlFlexLexer

bool gTraduire =  true;

msdrTokenKind returnToken (
  int           inputLineNumber,
  string        tokenText,
  msdrTokenKind tokenKind)
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
      setw (tokenNameWidth) <<
      msdrTokenKindAsString (tokenKind);

    switch (tokenKind) {
      // language-independant tokens
      // ------------------------------------

      case kTokenSpaces:
        break;
      case kTokenEndOfLine:
        doWriteTokenText = false;
        break;

      case kTokenParenthesizedComment:
        break;
      case kTokenCommentToEndOfLine:
        break;

      case kTokenEqualSign:
        doWriteTokenText = false;
        break;

      case kTokenComma:
        doWriteTokenText = false;
        break;
      case kTokenColon:
        doWriteTokenText = false;
        break;
      case kTokenSemiColon:
        doWriteTokenText = false;
        break;

      case kTokenLeftParenthesis :
        doWriteTokenText = false;
        break;
      case kTokenRightParenthesis :
        doWriteTokenText = false;
        break;

      case kTokenLeftBracket:
        doWriteTokenText = false;
        break;
      case kTokenRightBracket:
        doWriteTokenText = false;
        break;

      case kTokenDot:
        doWriteTokenText = false;
        break;

      case kTokenMeasure:
        doWriteTokenText = false;
        break;
      case kTokenDoubleBar:
        doWriteTokenText = false;
        break;

      case kTokenInteger:
        break;
      case kTokenDouble:
        break;

      case kTokenSingleQuotedString:
        break;
      case kTokenDoubleQuotedString:
        break;

      case kTokenOtherCaracter:
        break;

      case kTokenIdentifier:
        break;

      default:
        // language-dependant keywords
        // ------------------------------------
        // JMI ???; // don't care, those are flex internals
        cout << " (keyword)";
    } // switch

    if (doWriteTokenText) {
      cout << left <<
        "\"" << tokenText << "\"";
    }

    cout << endl;
    }

  return tokenKind;
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

space		                 [ \t]
spaces                   {space}+

endOfLine                [\x0a\x0d]
spaceOrEndOfLine         [ \t\x0a\x0d]

singleQuotedStringChar   [^']
doubleQuotedStringChar   [^"]

digit		                 [0-9]
decimalInteger           {digit}+

exponent			 					 [eE][+-]?{decimalInteger}
letter		               [a-zA-ZˆŽ™Œ¾Ï‚Î®çèéëêô]
firstchar	               [a-zA-Z_]
namechar	               [a-zA-Z0-9_]
name                     {firstchar}{namechar}*


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
              returnToken (lineno (), YYText (), kTokenParenthesizedComment);
          }


"%"				{
            BEGIN COMMENT_TO_END_OF_LINE_MODE;
          }

<COMMENT_TO_END_OF_LINE_MODE>{endOfLine} {
            BEGIN INITIAL;
          }

<COMMENT_TO_END_OF_LINE_MODE>.*	{
            return
              returnToken (lineno (), YYText (), kTokenCommentToEndOfLine);
          }


{spaces}  {
            return
              returnToken (lineno (), YYText (), kTokenSpaces);
          }

{endOfLine}	{
            return
              returnToken (lineno (), YYText (), kTokenEndOfLine);
          }


"="       {
            return
              returnToken (lineno (), YYText (), kTokenEqualSign);
          }


","       {
            return
              returnToken (lineno (), YYText (), kTokenComma);
          }

":"       {
            return
              returnToken (lineno (), YYText (), kTokenColon);
          }

";"       {
            return
              returnToken (lineno (), YYText (), kTokenSemiColon);
          }


"("       {
            return
              returnToken (lineno (), YYText (), kTokenLeftParenthesis);
          }

")"       {
            return
              returnToken (lineno (), YYText (), kTokenRightParenthesis);
          }


"{"       {
            return
              returnToken (lineno (), YYText (), kTokenLeftBracket);
          }

"}"       {
            return
              returnToken (lineno (), YYText (), kTokenRightBracket);
          }


"."       {
            return
              returnToken (lineno (), YYText (), kTokenDot);
          }


"|"       {
            return
              returnToken (lineno (), YYText (), kTokenMeasure);
          }

"||"      {
            return
              returnToken (lineno (), YYText (), kTokenDoubleBar);
          }


{decimalInteger} {
            return
              returnToken (lineno (), YYText (), kTokenInteger);
          }

{decimalInteger}"."{decimalInteger}({exponent})? |
{decimalInteger}({exponent})?	{
            // yylval.fNombre = atof (yytext);
            return
              returnToken (lineno (), YYText (), kTokenDouble);
          }


"'"{singleQuotedStringChar}*"'" {
            return
              returnToken (lineno (), YYText (), kTokenSingleQuotedString);
          }

"\""{doubleQuotedStringChar}*"\"" {
            return
              returnToken (lineno (), YYText (), kTokenDoubleQuotedString);
          }


{name}    {
/*
            msdlKeywordKind
              keywordKind =
                msdlKeywordKindFromString (YYText ());

            if (keywordKind == k_NoMsdlKeywordKind) {
              return
                returnToken (lineno (), YYText (), kTokenIdentifier);
            }
            else {
              return
                returnToken (lineno (), YYText (), KEYWORD);
            }
*/
            return
              returnToken (lineno (), YYText (), kTokenIdentifier);
          }

.         {
            return
              returnToken (lineno (), YYText (), kTokenOtherCaracter);
          }

<<EOF>>		{
            yyterminate();
          }


%%


/* ---------------------------------------------------------------------- */
/* Service code                                                           */
/* ---------------------------------------------------------------------- */

#ifdef LEXER_MAIN

int main (int argc, char * argv[])
{
/*
	if (argc > 1) {
		testreader r;

		return readfile (argv[1], &r) ? 0 : 1;
	}
*/

  msdlFlexLexer lexer;

  lexer.switch_streams (&std::cin, &std::cout);

  while (lexer.yylex ());

  std::cout << "MSDL LEXICAL ANALYSIS SUCCEEDED." << std::endl;
  return 0;
}

#endif

