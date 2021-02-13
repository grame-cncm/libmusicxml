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

#include <iomanip>
#include <iostream>

#include "msdrKeywords.h"
#include "msdrTokens.h"

#include "msdlDriver.h"
#include "msdlParser.h"


using namespace MusicXML2;

/* ---------------------------------------------------------------------- */
/* The current token description                                          */
/* ---------------------------------------------------------------------- */

#undef yyFlexLexer
#define yyFlexLexer msdlFlexLexer

bool gTraduire =  true;

void handleMsdrToken (
  int           inputLineNumber,
  string        tokenText,
  msdrTokenKind tokenKind)
{
  if (gTraduire) {
    // write a trace of the token?
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

      case kTokenBlanks:
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

  // append the MSDL token to the tokens list JMI
}

%}


/* ---------------------------------------------------------------------- */
/* Options                                                                */
/* ---------------------------------------------------------------------- */

/*
%option c++

#define YYPARSE_PARAM scanner
#define YYLEX_PARAM   scanner

%option debug
*/

%option 8bit

%option nodefault

%option warn
%option nounput nomain noinput noyywrap yylineno

%option interactive

%option prefix="msdl"


%{
%}


/* ---------------------------------------------------------------------- */
/* Regular expressions                                                    */
/* ---------------------------------------------------------------------- */

blank		                 [ \t\r]
blanks                   {blank}+

endOfLine                [\x0a\x0d]
blankOrEndOfLine         [ \t\r\x0a\x0d]

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

%{
  // Code run each time a pattern is matched.
//  # define YY_USER_ACTION  loc.columns (yyleng);
%}


%s PARENTHESIZED_COMMENT_MODE COMMENT_TO_END_OF_LINE_MODE


%%	/* The language terminals and corresponding semantic actions */


%{
  /* -------------------------------------------------------------------- */
  /* Code local to "yylex"                                                */
  /* -------------------------------------------------------------------- */

  // A handy shortcut to the location held by the driver.
//JMI  NO, code will be included in msdlBisonParser.cpp
//  msdl::location& loc = drv.getLocation ();

  // Code run each time yylex is called.
//  loc.step ();
%}


"%{"			{
            BEGIN PARENTHESIZED_COMMENT_MODE;
          }

<PARENTHESIZED_COMMENT_MODE>"%}"	{
            BEGIN INITIAL;
          }

<PARENTHESIZED_COMMENT_MODE>([^%]|"%"[^}])*	{
            handleMsdrToken (msdllineno, msdltext, kTokenParenthesizedComment) ;

            return msdl::parser::token::MSDL_PARENTHESIZED_COMMENT;
          }


"%"				{
            BEGIN COMMENT_TO_END_OF_LINE_MODE;
          }

<COMMENT_TO_END_OF_LINE_MODE>{endOfLine} {
            BEGIN INITIAL;
          }

<COMMENT_TO_END_OF_LINE_MODE>.*	{
            handleMsdrToken (msdllineno, msdltext, kTokenParenthesizedComment) ;

            return MSDL_COMMENT_TO_END_OF_LINE;
          }


{blanks}  {
            handleMsdrToken (msdllineno, msdltext, kTokenBlanks) ;

         //   loc.step ();
            return MSDL_SPACES;
          }

{endOfLine}	{
            handleMsdrToken (msdllineno, msdltext, kTokenEndOfLine) ;

       //     loc.lines (yyleng); loc.step ();
            return MSDL_END_OF_LINE;
          }


"="       {
            handleMsdrToken (msdllineno, msdltext, kTokenEqualSign) ;

            return MSDL_EQUAL_SIGN;
          }


","       {
            handleMsdrToken (msdllineno, msdltext, kTokenComma) ;

            return MSDL_COMMA;
          }

":"       {
            handleMsdrToken (msdllineno, msdltext, kTokenColon) ;

            return MSDL_COLON;
          }

";"       {
            handleMsdrToken (msdllineno, msdltext, kTokenSemiColon) ;

            return MSDL_SEMI_COLON;
          }


"("       {
            handleMsdrToken (msdllineno, msdltext, kTokenLeftParenthesis) ;

            return MSDL_LEFT_PARENTHESIS;
          }

")"       {
            handleMsdrToken (msdllineno, msdltext, kTokenRightParenthesis) ;

            return MSDL_RIGHT_PARENTHESIS;
          }


"{"       {
            handleMsdrToken (msdllineno, msdltext, kTokenLeftBracket) ;

            return MSDL_LEFT_BRACKET;
          }

"}"       {
            handleMsdrToken (msdllineno, msdltext, kTokenRightBracket) ;

            return MSDL_RIGHT_BRACKET;
          }


"."       {
            handleMsdrToken (msdllineno, msdltext, kTokenDot) ;

            return MSDL_DOT;
          }


"|"       {
            handleMsdrToken (msdllineno, msdltext, kTokenMeasure) ;

            return MSDL_MEASURE;
          }

"||"      {
            handleMsdrToken (msdllineno, msdltext, kTokenDoubleBar) ;

            return MSDL_DOUBLE_BAR;
          }


{decimalInteger} {
            handleMsdrToken (msdllineno, msdltext, kTokenInteger) ;

            return MSDL_INTEGER;
          }

{decimalInteger}"."{decimalInteger}({exponent})? |
{decimalInteger}({exponent})?	{
            handleMsdrToken (msdllineno, msdltext, kTokenDouble) ;

            // yylval.fNombre = atof (yytext);
            return MSDL_DOUBLE;
          }


"'"{singleQuotedStringChar}*"'" {
            handleMsdrToken (msdllineno, msdltext, kTokenSingleQuotedString) ;

            return MSDL_SINGLE_QUOTED_STRING;
          }

"\""{doubleQuotedStringChar}*"\"" {
            handleMsdrToken (msdllineno, msdltext, kTokenDoubleQuotedString) ;

            return MSDL_DOUBLE_QUOTED_STRING;
          }


{name}    {
/*
            msdlKeywordKind
              keywordKind =
                msdlKeywordKindFromString (msdltext);

            if (keywordKind == k_NoMsdlKeywordKind) {
              return
            }
            else {
              return
            }
*/
            handleMsdrToken (msdllineno, msdltext, kTokenIdentifier) ;

            return MSDL_IDENTIFIER;
          }

.         {
            handleMsdrToken (msdllineno, msdltext, k_NoToken); // JMI

            return MSDL_OTHER_CHARACTER;
          }

<<EOF>>		{
            yyterminate();
          }


%%


/* ---------------------------------------------------------------------- */
/* Service code                                                           */
/* ---------------------------------------------------------------------- */

void
msdlDriver::scanBegin ()
{
  yy_flex_debug = fTraceScanning;

  if (fFile.empty () || fFile == "-") {
    yyin = stdin;
  }
  else if (!(yyin = fopen (fFile.c_str (), "r"))) {
      std::cerr << "cannot open " << fFile << ": " << strerror (errno) << '\n';
      exit (EXIT_FAILURE);
  }
}

void
msdlDriver::scanEnd ()
{
  fclose (yyin);
}

/*
#ifdef LEXER_MAIN

int main (int argc, char * argv[])
{
//*
	if (argc > 1) {
		testreader r;

		return readfile (argv[1], &r) ? 0 : 1;
	}
//* /

  msdlFlexLexer lexer;

  lexer.switch_streams (&std::cin, &std::cout);

  while (lexer.yylex ());

  std::cout << "MSDL LEXICAL ANALYSIS SUCCEEDED." << std::endl;
  return 0;
}

#endif
*/
