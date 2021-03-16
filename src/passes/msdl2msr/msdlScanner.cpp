/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iostream>
#include <iomanip>  // for setw()
#include <cstdlib>  // for stod()

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "msdlScannerWaeHandlers.h"

#include "utilities.h"

#include "msdlScanner.h"

#include "waeExceptions.h"

#include "msdl2msrOah.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
string msdlIgnoreSeparatorTokensKindAsString (
  msdlIgnoreSeparatorTokensKind ignoreSeparatorTokensKind);
void displayStreamState (const ios& stream) {
  gLogStream <<
    "good () : " << stream.good () <<
    endl <<
    "eof ()  : " << stream.eof () <<
    endl <<
    "fail () : " << stream.fail () <<
    endl <<
    "bad ()  : " << stream.bad () <<
    endl;
}

//________________________________________________________________________
string msdlIgnoreSeparatorTokensKindAsString (
  msdlIgnoreSeparatorTokensKind ignoreSeparatorTokensKind)
{
  string result;

  switch (ignoreSeparatorTokensKind) {
    case msdlIgnoreSeparatorTokensKind::kIgnoreSeparatorTokensNo:
      result = "ignoreSeparatorTokensNo";
      break;
    case msdlIgnoreSeparatorTokensKind::kIgnoreSeparatorTokensYes:
      result = "ignoreSeparatorTokensYes";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
S_msdlScanner msdlScanner::create (
  string   inputSourceName,
  istream& inputStream)
{
  msdlScanner* o =
    new msdlScanner (
      inputSourceName,
      inputStream);
  assert (o != nullptr);
  return o;
}

msdlScanner::msdlScanner (
  string   inputSourceName,
  istream& inputStream)
    : fInputStream (
        inputStream),
      fCurrentToken (
        ),
      fCurrentTokenKind (
        fCurrentToken.getTokenKindToModify ()),
      fCurrentTokenDescription (
        fCurrentToken.getTokenDescriptionToModify ())
{
  // trace
#ifdef TRACING_IS_ENABLED
  fTraceTokens        = gGlobalMsdl2msrOahGroup->getTraceTokens ();
  fTraceTokensDetails = gGlobalMsdl2msrOahGroup->getTraceTokensDetails ();
#endif

  // input source name
  fInputSourceName = inputSourceName;

  // user language
  fUserLanguageKind =
    gGlobalMsdl2msrOahGroup->getMsdlUserLanguageKind ();

  // input stream
  fInputIsEmpty = true;

  // input string
  fInputStringSize = 0;

  // lines
  fCurrentLineNumber = 1;
  fCurrentLineSize = -1;

  fCurrentLinePositionInInput = 0;

  fCurrentPositionInLine = 0;

  // characters
  fCurrentPositionInInput = -1;

  fCurrentCharacter = '@'; // any illegal character, just to get it initialized
  fNextCharacterIsAvailable = false;

  // tokens
  fCurrentTokenPositionInInput = -1;

  fCurrentTokenLineNumber = 1;
  fCurrentTokenPositionInLine  = 0;

  fTokensCounter = 0;

  // keywords
  fKeywordsInputLanguageKind =
    gGlobalMsdl2msrOahGroup->getMsdlKeywordsInputLanguageKind ();

  // tokens list
  fAppendTokensToList = false;

  // warnings and errors
  switch (fUserLanguageKind) {
    case msdlUserLanguageKind::kUserLanguageEnglish:
      fScannerWaeHandler = msdlScannerEnglishWaeHandler::create ();
      break;
    case msdlUserLanguageKind::kUserLanguageFrench:
      fScannerWaeHandler = msdlScannerFrenchWaeHandler::create ();
      break;
    case msdlUserLanguageKind::kUserLanguageItalian:
      fScannerWaeHandler = msdlScannerItalianWaeHandler::create ();
      break;
    case msdlUserLanguageKind::kUserLanguageGerman:
      fScannerWaeHandler = msdlScannerGermanWaeHandler::create ();
      break;
    case msdlUserLanguageKind::kUserLanguageSpanish:
      fScannerWaeHandler = msdlScannerSpanishWaeHandler::create ();
      break;
    case msdlUserLanguageKind::kUserLanguageDutch:
      fScannerWaeHandler = msdlScannerDutchWaeHandler::create ();
      break;
  } // switch

  // lexical correctness
  fSourceIsLexicallyCorrect = true;

  populateInputString (fInputStream);
}

msdlScanner::~ msdlScanner ()
{}

// --------------------------------------------------------------------------
//  msdlScanner::populateInputString
// --------------------------------------------------------------------------

void msdlScanner::populateInputString (istream& inputStream)
{
#ifdef TRACING_IS_ENABLED
  if (fTraceTokensDetails) {
    gLogStream <<
      "Populating the input string" <<
      endl;
  }
#endif

  for ( ; ; ) {
    char character;

    inputStream.get (character);

    if (inputStream.eof () || inputStream.fail ()) {
      break;
    }

    fInputString.push_back (character);
  } // for

  fInputStringSize = int (fInputString.size ());

  fInputIsEmpty = fInputStringSize == 0;

#ifdef TRACING_IS_ENABLED
  if (fTraceTokensDetails) {
    gLogStream <<
      "inputStringSize: " << fInputStringSize <<
      endl <<
      "inputString: \"" <<
      fInputString <<
      "\"" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlScanner::currentCharacterAsString
// --------------------------------------------------------------------------

string msdlScanner::currentCharacterAsString () const
{
  stringstream s;

  if (fCurrentCharacter == EOF) {
    s << "EOF";
  }
  else {
    s << "'" << fCurrentCharacter << "'";
  }

  return s.str ();
}

// --------------------------------------------------------------------------
//  msdlScanner::fetchNextCharacter
// --------------------------------------------------------------------------

char msdlScanner::fetchNextCharacter ()
{
#ifdef TRACING_IS_ENABLED
  if (fTraceTokensDetails) {
    gLogStream <<
      "--> fetchNextCharacter()" <<
      ", fCurrentLineNumber: " <<
      fCurrentLineNumber <<
      ", fNextCharacterIsAvailable: " <<
      booleanAsString (fNextCharacterIsAvailable) <<
      ", fCurrentPositionInInput: " <<
      fCurrentPositionInInput <<
      ", fCurrentTokenPositionInInput: " <<
      fCurrentTokenPositionInInput <<
      ", fCurrentLinePositionInInput: " <<
      fCurrentLinePositionInInput <<
      ", fCurrentPositionInLine: " <<
      fCurrentPositionInLine <<
      endl;
  }
#endif

  // has the end of input been reached?
  if (fCurrentPositionInInput == fInputStringSize - 1) {
    fCurrentCharacter = EOF;
  }
  else {
    // update positions
    ++fCurrentPositionInInput;
    fCurrentLinePositionInInput = fCurrentPositionInInput + 1;
    ++fCurrentPositionInLine;

    // fetch new current character
    fCurrentCharacter = fInputString [fCurrentPositionInInput];
  }

  fNextCharacterIsAvailable = true;

#ifdef TRACING_IS_ENABLED
  if (fTraceTokensDetails) {
    gLogStream <<
      "<-- fetchNextCharacter() returns: " <<
      currentCharacterAsString () <<
      endl;
  }
#endif

  return fCurrentCharacter;
}

// --------------------------------------------------------------------------
//  msdlScanner::handleEndOfLine
// --------------------------------------------------------------------------

void msdlScanner::handleEndOfLine (string context)
{
#ifdef TRACING_IS_ENABLED
  if (fTraceTokensDetails) {
    gLogStream <<
      "--> handleEndOfLine(): " <<
      ", context: " << context <<
      ", fCurrentLineNumber: " << fCurrentLineNumber <<
      ", fCurrentLinePositionInInput: " << fCurrentLinePositionInInput <<
      ", fCurrentPositionInLine: " << fCurrentPositionInLine <<
      endl;
  }
#endif

  ++fCurrentLineNumber;
  fCurrentLinePositionInInput = fCurrentPositionInInput + 1;

  fCurrentPositionInLine = 0;

#ifdef TRACING_IS_ENABLED
  if (fTraceTokensDetails) {
    gLogStream <<
      "<-- handleEndOfLine(): " <<
      ", context: " << context <<
      ", fCurrentLineNumber: " << fCurrentLineNumber <<
      ", fCurrentLinePositionInInput: " << fCurrentLinePositionInInput <<
      ", fCurrentPositionInLine: " << fCurrentPositionInLine <<
      endl << endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlScanner::fetchNextToken
// --------------------------------------------------------------------------

msdlTokenKind msdlScanner::fetchNextToken (
  msdlIgnoreSeparatorTokensKind
    ignoreSeparatorTokens)
{
  ++fTokensCounter;

#ifdef TRACING_IS_ENABLED
  if (fTraceTokens) {
    gLogStream <<
      endl <<
      fTokensCounter << " ==> fetchNextToken()" <<
      ", ignoreSeparatorTokens: " <<
      msdlIgnoreSeparatorTokensKindAsString (ignoreSeparatorTokens) <<
      endl;

    ++gIndenter;

    unsigned int fieldWidth = 28;

    gLogStream << left <<
      setw (fieldWidth) <<
      "fNextCharacterIsAvailable: " << " : " <<
      booleanAsString (fNextCharacterIsAvailable) <<
      endl <<
      setw (fieldWidth) <<
      "fCurrentPositionInInput" << " : " << fCurrentPositionInInput <<
      endl <<
      setw (fieldWidth) <<
      "fCurrentPositionInLine" << " : " << fCurrentPositionInLine <<
      endl <<
      setw (fieldWidth) <<
      "fCurrentCharacter" << " : '" << currentCharacterAsString () <<
      endl <<
      setw (fieldWidth) <<
      "fNextCharacterIsAvailable" << " : " << booleanAsString (fNextCharacterIsAvailable) <<
      endl;

    --gIndenter;
  }
#endif

  ++gIndenter;

  // fetch the first token character if needed
  if (! fNextCharacterIsAvailable) {
    fetchNextCharacter ();
  }

  // do the actual work at least once, ignoring separator tokens if needed
  for ( ; ; ) {

    // put the token position aside
    fCurrentTokenPositionInInput = fCurrentPositionInInput;

    fCurrentTokenLineNumber     = fCurrentLineNumber;
    fCurrentTokenPositionInLine = fCurrentPositionInLine;

    switch (fCurrentCharacter) {
      case EOF:
#ifdef TRACING_IS_ENABLED
        if (fTraceTokensDetails) {
          gLogStream << "EOF has been reached" << endl;
        }
#endif

        fCurrentTokenKind = msdlTokenKind::k_TokenEOF;
        fNextCharacterIsAvailable = true; // don't go ahead anymore
        break;

      case ' ' :
        fCurrentTokenKind = msdlTokenKind::kTokenSpace;
        fNextCharacterIsAvailable = false;
        break;
      case '\t' :
        fCurrentTokenKind = msdlTokenKind::kTokenTabSeparator;
        fNextCharacterIsAvailable = false;
        break;
      case '\xD' :
        fCurrentTokenKind = msdlTokenKind::kTokenCarriageReturn;
        fNextCharacterIsAvailable = false;
        break;

      case '\n' :
        fCurrentTokenKind = msdlTokenKind::kTokenEndOfLine;
        fNextCharacterIsAvailable = false;

        handleEndOfLine ("regular \\n");
        break;

      case '/':
        handleSlash ();
        break;

      case '%':
        handlePercent ();
        break;

      case '=' :
        fCurrentTokenKind = msdlTokenKind::kTokenEqualSign;
        fNextCharacterIsAvailable = false;
        break;

      case ',' :
        fCurrentTokenKind = msdlTokenKind::kTokenComma;
        fNextCharacterIsAvailable = false;
        break;

      case ':' :
        fetchNextCharacter ();

        if (fCurrentCharacter == '|') {
          fetchNextCharacter (); // ':|' up to this point

          if (fCurrentCharacter == '|') { // ':||' up to this point
            fCurrentTokenKind = msdlTokenKind::kTokenFinalBar;
            fNextCharacterIsAvailable = false;
          }
          else {
            fScannerWaeHandler->illegalCharacters (":|");
            fSourceIsLexicallyCorrect = false;
            fNextCharacterIsAvailable = true;
          }
        }
        else {
          fCurrentTokenKind = msdlTokenKind::kTokenColon;
          fNextCharacterIsAvailable = true;
        }
        break;

      case ';' :
        fCurrentTokenKind = msdlTokenKind::kTokenSemiColon;
        fNextCharacterIsAvailable = false;
        break;

      case '+' :
        fCurrentTokenKind = msdlTokenKind::kTokenPlus;
        fNextCharacterIsAvailable = false;
        break;
      case '-' :
        fCurrentTokenKind = msdlTokenKind::kTokenMinus;
        fNextCharacterIsAvailable = false;
        break;
      case '*' :
        fCurrentTokenKind = msdlTokenKind::kTokenStar;
        fNextCharacterIsAvailable = false;
        break;

      case '!':
        fetchNextCharacter ();

        if (fCurrentCharacter == '!') { // '!!' up to this point
          fCurrentTokenKind = msdlTokenKind::kTokenConcat;
          fNextCharacterIsAvailable = false;
        }
        else {
          fScannerWaeHandler->illegalCharacter ('!');
          fSourceIsLexicallyCorrect = false;
          fNextCharacterIsAvailable = true;
        }
        break;

      case '?' :
        fCurrentTokenKind = msdlTokenKind::kTokenQuestionMark;
        fNextCharacterIsAvailable = false;
        break;

      case '~' :
        fCurrentTokenKind = msdlTokenKind::kTokenTilda;
        fNextCharacterIsAvailable = false;
        break;

      case '<' :
        fCurrentTokenKind = msdlTokenKind::kTokenLeftAngle;
        fNextCharacterIsAvailable = false;
        break;
      case '>' :
        fCurrentTokenKind = msdlTokenKind::kTokenRightAngle;
        fNextCharacterIsAvailable = false;
        break;

      case '(' :
        fCurrentTokenKind = msdlTokenKind::kTokenLeftParenthesis;
        fNextCharacterIsAvailable = false;
        break;
      case ')' :
        fCurrentTokenKind = msdlTokenKind::kTokenRightParenthesis;
        fNextCharacterIsAvailable = false;
        break;

      case '{' :
        fCurrentTokenKind = msdlTokenKind::kTokenLeftBracket;
        fNextCharacterIsAvailable = false;
        break;
      case '}' :
        fCurrentTokenKind = msdlTokenKind::kTokenRightBracket;
        fNextCharacterIsAvailable = false;
        break;

     case '\'':
        fCurrentTokenKind = msdlTokenKind::kTokenQuote;
        fNextCharacterIsAvailable = false;
        break;

      case '.' :
        fCurrentTokenKind = msdlTokenKind::kTokenDot;
        fNextCharacterIsAvailable = false;
        break;

       case '|' :
        fetchNextCharacter ();

        if (fCurrentCharacter == '|') { // '||' up to this point
          fetchNextCharacter ();

          if (fCurrentCharacter == '|') { // '|||' up to this point
            fCurrentTokenKind = msdlTokenKind::kTokenFinalBar;
            fNextCharacterIsAvailable = false;
          }
          else {
            fCurrentTokenKind = msdlTokenKind::kTokenDoubleBar;
            fNextCharacterIsAvailable = true;
          }
        }
        else {
          fCurrentTokenKind = msdlTokenKind::kTokenMeasure;
          fNextCharacterIsAvailable = true;
        }
        break;

      case '"': //  double-quoted string
        acceptAString ();
        break;

      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        acceptAnInteger ();
        break;

      default:
        if (isalpha (fCurrentCharacter)) { // name
          acceptAName ();
        }

        else { // illegal character
          fScannerWaeHandler->illegalCharacter (fCurrentCharacter);
          fSourceIsLexicallyCorrect = false;

          stringstream s;

          s << fCurrentCharacter;

          fCurrentTokenKind = msdlTokenKind::k_TokenMalformed;
          fCurrentTokenDescription.setString (s.str ());

          fNextCharacterIsAvailable = false;
        }
    } // switch (fCurrentCharacter)

#ifdef TRACING_IS_ENABLED
    if (fTraceTokensDetails) {
      gLogStream <<
        fCurrentToken.asString () <<
        endl;
    }
#endif

    // should we loop once more?
    bool onceMore = false;

    switch (fCurrentTokenKind) {
      case msdlTokenKind::k_TokenEOF:
        break;

      case msdlTokenKind::k_NoToken:
        // should not occur
        break;

      case msdlTokenKind::k_TokenMalformed:
        onceMore = true; // false for error recovery??? JMI
        break;

      // separators, for use by MSDL whole input translation
      // ------------------------------------

      case msdlTokenKind::kTokenSpace:
      case msdlTokenKind::kTokenTabSeparator:
      case msdlTokenKind::kTokenCarriageReturn:
      case msdlTokenKind::kTokenEndOfLine:
      case msdlTokenKind::kTokenParenthesizedComment:
      case msdlTokenKind::kTokenCommentToEndOfLine:
        onceMore = ignoreSeparatorTokens == msdlIgnoreSeparatorTokensKind::kIgnoreSeparatorTokensYes;
        break;

      default:
        ;
    } // switch

#ifdef TRACING_IS_ENABLED
    if (fTraceTokensDetails) {
      gLogStream <<
        "--- fetchNextToken(): onceMore = " << booleanAsString (onceMore) <<
        endl;
    }
#endif

    if (onceMore) {
      if (! fNextCharacterIsAvailable) {
        // fetch the next character
        fetchNextCharacter ();
      }

      // loop once more
    }

    else {
      break; // quit the loop
    }

  } // for

  --gIndenter;

  // register current token position
  fCurrentToken.setTokenLineNumber (fCurrentTokenLineNumber);
  fCurrentToken.setTokenPositionInLine (fCurrentTokenPositionInLine);

#ifdef TRACING_IS_ENABLED
  if (fTraceTokens) {
    gLogStream <<
      endl <<
      "#" << fTokensCounter << " <== fetchNextToken()" <<
      endl;

    ++gIndenter;

    gLogStream <<
      "fCurrentToken: " <<
      endl;

    ++gIndenter;
    gLogStream << fCurrentToken;
    --gIndenter;

    unsigned int fieldWidth = 28;

    gLogStream << left <<
      setw (fieldWidth) <<
      "fNextCharacterIsAvailable: " << " : " <<
      booleanAsString (fNextCharacterIsAvailable) <<
      endl <<
      setw (fieldWidth) <<
      "fCurrentPositionInInput" << " : " << fCurrentPositionInInput <<
      endl <<
      setw (fieldWidth) <<
      "fCurrentPositionInLine" << " : " << fCurrentPositionInLine <<
      endl <<
      setw (fieldWidth) <<
      "fCurrentCharacter" << " : '" << currentCharacterAsString () <<
      endl <<
      setw (fieldWidth) <<
      "fNextCharacterIsAvailable" << " : " << booleanAsString (fNextCharacterIsAvailable) <<
      endl;

    --gIndenter;
  }
#endif

  // should the current token be appended to the tokens list?
  if (fAppendTokensToList) {
    fTokensList.appendTokenToTokensList (fCurrentToken);
  }

  return fCurrentTokenKind;
}

// --------------------------------------------------------------------------
//  msdlScanner::handlePercent
// --------------------------------------------------------------------------

void msdlScanner::handlePercent ()
{
  // consume the '%'
  fetchNextCharacter ();

  switch (fCurrentCharacter) {
    case '{':  // '%{' up to this point, parenthesized comment
      {
        fetchNextCharacter ();

        int percentParenthesizedCommentStartPositionInInput = fCurrentTokenPositionInInput;

        do {
          while (fCurrentCharacter != '%') {
            if (fCurrentCharacter == '\n') {
              handleEndOfLine ("percent parenthesized comment");

              fNextCharacterIsAvailable = false;
            } // while

            // consume this comment character
            fetchNextCharacter ();
          } // while

          // consume the comment character
          fetchNextCharacter ();
        } // do
        while (fCurrentCharacter != '}');

        // the end of the comment has been reached
        fNextCharacterIsAvailable = false;

#ifdef TRACING_IS_ENABLED
        if (fTraceTokensDetails) {
          gLogStream <<
            "percentParenthesizedCommentStartPositionInInput: " <<
            percentParenthesizedCommentStartPositionInInput <<
            endl <<
            "fCurrentPositionInInput: " <<
            fCurrentPositionInInput <<
            endl;
        }
#endif

        string commentString =
          fInputString.substr (
            percentParenthesizedCommentStartPositionInInput + 2,
            fCurrentPositionInInput - percentParenthesizedCommentStartPositionInInput - 3);

        fCurrentTokenKind = msdlTokenKind::kTokenParenthesizedComment;
        fCurrentTokenDescription.setString (commentString);
      }
      break;

    default: // '%' up to this point, comment to end of line
      {
        int parcentCommentToEndOfLineStartPositionInInput = fCurrentTokenPositionInInput;

        while (fCurrentCharacter != '\n') {
          // consume this comment character
          fetchNextCharacter ();
        } // while

        // the end of the comment to end of line has been overtaken
        fNextCharacterIsAvailable = true;

        string commentString =
          fInputString.substr (
            parcentCommentToEndOfLineStartPositionInInput + 1,
            fCurrentPositionInInput - parcentCommentToEndOfLineStartPositionInInput - 1);

        fCurrentTokenKind = msdlTokenKind::kTokenCommentToEndOfLine;
        fCurrentTokenDescription.setString (commentString);

// JMI        handleEndOfLine ("percent comment to end of line");
      }
      break;
  } // switch
}

// --------------------------------------------------------------------------
//  msdlScanner::handleSlash
// --------------------------------------------------------------------------

void msdlScanner::handleSlash ()
{
  // consume the '/'
  fetchNextCharacter ();

  switch (fCurrentCharacter) {
    case '*': // '/*' up to this point, parenthesized comment
      {
       fetchNextCharacter ();

        unsigned slashParenthesizedCommentStartPositionInInput = fCurrentTokenPositionInInput;

        do {
          while (fCurrentCharacter != '*') {
            if (fCurrentCharacter == '\n') {
              handleEndOfLine ("star parenthesized comment");

//              fNextCharacterIsAvailable = false;
              fetchNextCharacter ();
            } // while

            // consume this comment character
            fetchNextCharacter ();
          } // while

          // consume the '*'
          fetchNextCharacter ();
        } // do
        while (fCurrentCharacter != '/');

        fNextCharacterIsAvailable = false;

#ifdef TRACING_IS_ENABLED
        if (fTraceTokensDetails) {
          gLogStream <<
            "slashParenthesizedCommentStartPositionInInput: " <<
            slashParenthesizedCommentStartPositionInInput << endl <<
            "fCurrentPositionInInput: " <<
            fCurrentPositionInInput <<
            endl;
        }
#endif

        string commentString =
          fInputString.substr (
            slashParenthesizedCommentStartPositionInInput + 2,
            fCurrentPositionInInput - slashParenthesizedCommentStartPositionInInput - 3);

        fCurrentTokenKind = msdlTokenKind::kTokenParenthesizedComment;
        fCurrentTokenDescription.setString (commentString);

        fNextCharacterIsAvailable = false;
      }
      break;

    case '/': // '//' up to this point, comment to end of line
      {
        // consume the second '/'
        fetchNextCharacter ();

        unsigned slashCommentToEndOfLineStartPositionInInput = fCurrentTokenPositionInInput;

        while (fCurrentCharacter != '\n') {
          // consume the '\n'
          fetchNextCharacter ();
        } // while

        // the end of the comment to end of line has been overtaken
        fNextCharacterIsAvailable = true;

        string commentString =
          fInputString.substr (
            slashCommentToEndOfLineStartPositionInInput + 2,
            fCurrentPositionInInput - slashCommentToEndOfLineStartPositionInInput - 3);

        fCurrentTokenKind = msdlTokenKind::kTokenCommentToEndOfLine;
        fCurrentTokenDescription.setString (commentString);

// JMI        handleEndOfLine ("slash comment to end of line");
      }
      break;

    default:
      fCurrentTokenKind = msdlTokenKind::kTokenSlash;
      fNextCharacterIsAvailable = false;
  } // switch
}

// --------------------------------------------------------------------------
//  msdlScanner::acceptAName
// --------------------------------------------------------------------------

void msdlScanner::acceptAName ()
{
/*
  pitches such as c2 prevent identifiers from being tokens in MSDL
  as is usual in programming languages,
  so we handle actual identifiers such ar part1 at the syntax level
*/

  // accept all alphanumeric characters, the first character is available

  unsigned int
    nameStartPositionInInput =
      fCurrentTokenPositionInInput;

#ifdef TRACING_IS_ENABLED
  if (fTraceTokensDetails) {
    gLogStream << left <<
      "Accepting a name" <<
      ", nameStartPositionInInput: " <<
      nameStartPositionInInput <<
      endl;
  }
#endif

  ++gIndenter;

  // a name can be a keyword

  bool nonNameCharacterFound = false;

  while (! nonNameCharacterFound) {
    char character = fetchNextCharacter ();

    switch (character) {
      case EOF:
        nonNameCharacterFound = true;
        break;

      case '_':
        break;

      default:
        nonNameCharacterFound = ! isalpha (fCurrentCharacter);
    } // switch
  } // while

  // the end of the name has been overtaken
  fNextCharacterIsAvailable = true;

  string nameString =
    fInputString.substr (
      nameStartPositionInInput,
      fCurrentPositionInInput - nameStartPositionInInput);

#ifdef TRACING_IS_ENABLED
  if (fTraceTokensDetails) {
    gLogStream <<
      "--- acceptAName()" <<
      ", nameString: \"" << nameString << "\"" <<
      endl;
  }
#endif
  // is nameString the name of a keyword?
  msdlKeywordKind
    keyWordKind =
      msdlKeywordKindFromString (
        fKeywordsInputLanguageKind,
        nameString);

#ifdef TRACING_IS_ENABLED
  if (fTraceTokensDetails) {
    gLogStream <<
      "--- acceptAName()" <<
      ", nameString: \"" << nameString << "\"" <<
      ", keyWordKind: \"" << msdlKeywordKindAsString (keyWordKind) << "\"" <<
      endl;
  }
#endif

  switch (keyWordKind) {
    case msdlKeywordKind::k_NoKeywordKind: // no, it is a name
      fCurrentTokenKind = msdlTokenKind::kTokenName;
      fCurrentTokenDescription.setString (nameString);
      break;

      // language-dependent keywords
      // ------------------------------------

    case msdlKeywordKind::kKeywordTitle:
      fCurrentTokenKind = msdlTokenKind::kTokenTitle;
      break;
    case msdlKeywordKind::kKeywordComposer:
      fCurrentTokenKind = msdlTokenKind::kTokenComposer;
      break;
    case msdlKeywordKind::kKeywordOpus:
      fCurrentTokenKind = msdlTokenKind::kTokenOpus;
      break;

    case msdlKeywordKind::kKeywordPitches:
      fCurrentTokenKind = msdlTokenKind::kTokenPitches;
      break;
    case msdlKeywordKind::kKeywordOctaves:
      fCurrentTokenKind = msdlTokenKind::kTokenOctaves;
      break;

    case msdlKeywordKind::kKeywordAnacrusis:
      fCurrentTokenKind = msdlTokenKind::kTokenAnacrusis;
      break;

    case msdlKeywordKind::kKeywordBook:
      fCurrentTokenKind = msdlTokenKind::kTokenBook;
      break;
    case msdlKeywordKind::kKeywordScore:
      fCurrentTokenKind = msdlTokenKind::kTokenScore;
      break;
    case msdlKeywordKind::kKeywordPartGroup:
      fCurrentTokenKind = msdlTokenKind::kTokenPartGroup;
      break;
    case msdlKeywordKind::kKeywordPart:
      fCurrentTokenKind = msdlTokenKind::kTokenPart;
      break;
    case msdlKeywordKind::kKeywordMusic:
      fCurrentTokenKind = msdlTokenKind::kTokenMusic;
      break;
    case msdlKeywordKind::kKeywordFragment:
      fCurrentTokenKind = msdlTokenKind::kTokenFragment;
      break;

    case msdlKeywordKind::kKeywordClef:
      fCurrentTokenKind = msdlTokenKind::kTokenClef;
      break;
    case msdlKeywordKind::kKeywordTreble:
      fCurrentTokenKind = msdlTokenKind::kTokenTreble;
      break;
    case msdlKeywordKind::kKeywordSoprano:
      fCurrentTokenKind = msdlTokenKind::kTokenSoprano;
      break;
    case msdlKeywordKind::kKeywordAlto:
      fCurrentTokenKind = msdlTokenKind::kTokenAlto;
      break;
    case msdlKeywordKind::kKeywordTenor:
      fCurrentTokenKind = msdlTokenKind::kTokenTenor;
      break;
    case msdlKeywordKind::kKeywordBaryton:
      fCurrentTokenKind = msdlTokenKind::kTokenBaryton;
      break;
    case msdlKeywordKind::kKeywordBass:
      fCurrentTokenKind = msdlTokenKind::kTokenBass;
      break;
    case msdlKeywordKind::kKeywordTab:
      fCurrentTokenKind = msdlTokenKind::kTokenTab;
      break;

    case msdlKeywordKind::kKeywordKey:
      fCurrentTokenKind = msdlTokenKind::kTokenKey;
      break;

    case msdlKeywordKind::kKeywordTime:
      fCurrentTokenKind = msdlTokenKind::kTokenTime;
      break;
  } // switch

  // set the token description keyword kind if relevant
  if (fCurrentTokenKind != msdlTokenKind::kTokenName) {
    fCurrentTokenDescription.setKeywordKind (keyWordKind);
  }

  --gIndenter;

#ifdef TRACING_IS_ENABLED
  if (fTraceTokensDetails) {
    gLogStream <<
      "Accepting a name, found: \"" << nameString << "\"" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlScanner::acceptAnInteger
// --------------------------------------------------------------------------

void msdlScanner::acceptAnInteger ()
{
  // accept digits sequence, the first digit is available

  unsigned int
    integerStartPositionInInput =
      fCurrentTokenPositionInInput;

#ifdef TRACING_IS_ENABLED
  if (fTraceTokensDetails) {
    gLogStream << left <<
      "Accepting an integer" <<
      ", integerStartPositionInInput: " <<
      integerStartPositionInInput <<
      endl;
  }
#endif

  ++gIndenter;

  bool nonIntegerCharacterFound = false;

  while (! nonIntegerCharacterFound) {
    char character = fetchNextCharacter ();

    switch (character) {
      case EOF:
        nonIntegerCharacterFound = true;
        break;

      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        break;

      default:
        nonIntegerCharacterFound = true;
    } // switch
  } // while

  // the end of the integer has been overtaken
  fNextCharacterIsAvailable = true;

  string integerString =
    fInputString.substr (
      integerStartPositionInInput,
      fCurrentPositionInInput - integerStartPositionInInput);

  fCurrentTokenKind =  msdlTokenKind::kTokenInteger;
  fCurrentTokenDescription.setInteger (stoi (integerString));

  --gIndenter;

#ifdef TRACING_IS_ENABLED
  if (fTraceTokensDetails) {
    gLogStream <<
      "Accepting an integer, found: \"" << integerString << "\"" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlScanner::acceptAString
// --------------------------------------------------------------------------
void msdlScanner::acceptAString ()
{
  // accept all the string characters
  // we're on the initial double quote

  unsigned int
    stringStartPositionInInput =
      fCurrentTokenPositionInInput;

#ifdef TRACING_IS_ENABLED
  if (fTraceTokensDetails) {
    gLogStream << left <<
      endl <<
      "Accepting a string" <<
      ", stringStartPositionInInput: " << stringStartPositionInInput <<
      endl;
  }
#endif

  ++gIndenter;

  // the string we're building
  string theString;

  // consume the '"'
  fetchNextCharacter ();

  bool endOfStringHasBeenReached = false;

  int chunkStartPositionInInput =
    stringStartPositionInInput + 1;

  while (! endOfStringHasBeenReached) {
#ifdef TRACING_IS_ENABLED
  if (fTraceTokensDetails) {
    gLogStream <<
      "--- acceptAString()" <<
      ", fCurrentCharacter: " <<
      currentCharacterAsString () <<
      ", fNextCharacterIsAvailable: " <<
      booleanAsString (fNextCharacterIsAvailable) <<
      ", chunkStartPositionInInput: " <<
      chunkStartPositionInInput <<
      ", fCurrentPositionInInput: " <<
      fCurrentPositionInInput <<
      ", fCurrentPositionInLine: " <<
      fCurrentPositionInLine <<
      endl;
  }
#endif

    switch (fCurrentCharacter) {
      case '"':
        fNextCharacterIsAvailable = false;
        endOfStringHasBeenReached = true;
        break;

      case '\\': // escaped characted
        {
          // append the current chunk to theString
#ifdef TRACING_IS_ENABLED
          if (fTraceTokensDetails) {
            gLogStream <<
              "--- acceptAString()" <<
              ", chunkStartPositionInInput: " <<
              chunkStartPositionInInput <<
              ", fCurrentPositionInInput: " <<
              fCurrentPositionInInput <<
              ", fCurrentPositionInInput - chunkStartPositionInInput: " <<
              fCurrentPositionInInput - chunkStartPositionInInput <<
              endl;
          }
#endif

          string currentChunk =
            fInputString.substr (
              chunkStartPositionInInput,
              fCurrentPositionInInput - chunkStartPositionInInput);

#ifdef TRACING_IS_ENABLED
          if (fTraceTokensDetails) {
            gLogStream <<
              "--- acceptAString(), escaped character" <<
              ", currentChunk: \"" <<
              currentChunk <<
              "\"" <<
              endl;
          }
#endif

          theString += currentChunk;
#ifdef TRACING_IS_ENABLED
          if (fTraceTokensDetails) {
            gLogStream <<
              "--- acceptAString(), theString with chunk appended" <<
              ", theString: \"" <<
              theString <<
              "\"" <<
              endl;
          }
#endif

          // consume the escape '\'
          fetchNextCharacter ();

          // append the escaped character to theString
#ifdef TRACING_IS_ENABLED
          if (fTraceTokensDetails) {
            gLogStream <<
              ", escaped character: " <<
              currentCharacterAsString () <<
              endl;
          }
#endif

          switch (fCurrentCharacter) {
            case '"':
              theString += '"';
              break;
            case '\\':
              theString += '\\';
              break;
            case 'n':
              handleEndOfLine ("string 1");

              theString += '\n';
              break;
            case 't':
              theString += '\t';
              break;
            default:
              fScannerWaeHandler->
                illegalEscapedCharacterInString (fCurrentCharacter);
              fSourceIsLexicallyCorrect = false;
              fNextCharacterIsAvailable = false;
          } // switch

#ifdef TRACING_IS_ENABLED
          if (fTraceTokensDetails) {
            gLogStream <<
              "--- acceptAString(), theString with escaped character appended" <<
              ", theString: \"" <<
              theString <<
              "\"" <<
              endl;
          }
#endif

          // starting a new chunk
          chunkStartPositionInInput = fCurrentPositionInInput;
        }
        break;

// JMI multi-line strings ???
      case '\n':
        handleEndOfLine ("string 2");

        theString += "\n";
        fNextCharacterIsAvailable = false;

        fCurrentPositionInLine = 0;
        break;

      default:
        ; // the currentChunk will receive this character
      } //  switch

    if (endOfStringHasBeenReached) {
      break;
    }

    // consume this string character
    fetchNextCharacter ();
  } // while

  // append the last chunk to theString
  string currentChunk =
    fInputString.substr (
      chunkStartPositionInInput,
      fCurrentPositionInInput - chunkStartPositionInInput);
  theString += currentChunk;

  fCurrentTokenKind = msdlTokenKind::kTokenString;
  fCurrentTokenDescription.setString (theString);

  --gIndenter;

#ifdef TRACING_IS_ENABLED
  if (fTraceTokensDetails) {
    gLogStream <<
      endl <<
      "Accepting a string, found: \"" << theString << "\"" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlScanner::currentLocationAsString
// --------------------------------------------------------------------------

string msdlScanner::currentLocationAsString () const
{
  string separateur = // JMI
          "-----------------------------------------------------";

  stringstream s;

#ifdef TRACING_IS_ENABLED
  if (fTraceTokens) {
    s <<
      "==> fCurrentTokenPositionInInput:" << fCurrentTokenPositionInInput <<
      ", fCurrentPositionInInput:" << fCurrentPositionInInput <<
      endl;
  }
#endif

  // the line indication
  string lineIndication;

  {
    stringstream s;

    s <<
      fInputSourceName << ":" << fCurrentLineNumber << " ";

    lineIndication = s.str ();
  }

  // the line beginning
  int
    lineBeginningLength =
      fNextCharacterIsAvailable
        ? fCurrentPositionInInput - fCurrentTokenPositionInInput - 1
        : fCurrentPositionInInput - fCurrentTokenPositionInInput;

  string
    lineBeginning =
      fInputString.substr (
        fCurrentTokenPositionInInput,
        lineBeginningLength);

#ifdef TRACING_IS_ENABLED
  if (fTraceTokens) {
    s <<
      "==> lineBeginningLength:" << lineBeginningLength <<
      endl <<
      "==> lineBeginning:" <<
      endl <<
      "|" <<
      lineBeginning <<
      "|" <<
      endl;
  }
#endif

  s <<
    lineIndication <<
    lineBeginning <<
    endl <<
    replicateString (" ", lineIndication.size ()) <<
    replicateString (" ", lineBeginning.size ()) <<
    "^" <<
    endl;

  return s.str ();
}

// --------------------------------------------------------------------------
//  msdlScanner::scanAllTheInputAtOnce
// --------------------------------------------------------------------------

void msdlScanner::scanAllTheInputAtOnce (
  msdlIgnoreSeparatorTokensKind
    ignoreSeparatorTokens)
{
#ifdef TRACING_IS_ENABLED
  if (fTraceTokensDetails) {
    gLogStream <<
      "==> scanAllTheInputAtOnce()";
  }

  if (fTraceTokensDetails) {
    gLogStream <<
      ", ignoreSeparatorTokens: " <<
      msdlIgnoreSeparatorTokensKindAsString (ignoreSeparatorTokens) <<
      endl;
  }
  else {
    gLogStream << endl;
  }
#endif

  if (fInputIsEmpty) {
#ifdef TRACING_IS_ENABLED
    gLogStream <<
      "Input is empty " <<
      endl;
#endif
  }

  else {
    // let's go ahead

    ++gIndenter;

    int numberOfTokens = 0;

    try {
      msdlTokenKind tokenKind = msdlTokenKind::k_NoToken;

      // fetch the first input character
      fetchNextCharacter ();

      do { // loop till end of stream
        tokenKind = fetchNextToken (ignoreSeparatorTokens);
        ++numberOfTokens;

#ifdef TRACING_IS_ENABLED
        if (fTraceTokensDetails) {
          unsigned int fieldWidth = 28;

          gLogStream <<
            endl <<
            "--- scanAllTheInputAtOnce()" <<
            endl;

          ++gIndenter;

          gLogStream << left <<
            setw (fieldWidth) <<
            "fCurrentTokenPositionInInput" << " : " << fCurrentTokenPositionInInput <<
            endl <<
            setw (fieldWidth) <<
            "fCurrentCharacter" << " : " << currentCharacterAsString () <<
            endl <<
            setw (fieldWidth) <<
            "fNextCharacterIsAvailable" << " : " << booleanAsString (fNextCharacterIsAvailable) <<
            endl <<

            setw (fieldWidth) <<
            "fCurrentToken" << " : " <<
            endl;

          ++gIndenter;
          gLogStream << fCurrentToken << endl;
          --gIndenter;

          --gIndenter;
        }
#endif
      }
      while (tokenKind != msdlTokenKind::k_TokenEOF); // do
    }
    catch (out_of_range& e) {
      displayException (e, gOutputStream);
    }
    catch (exception& e) {
      displayException (e, gOutputStream);
    }

    if (fSourceIsLexicallyCorrect) {
      fScannerWaeHandler->
        inputIsLexicallyCorrect (numberOfTokens);
    }
    else {
      fScannerWaeHandler->
        inputIsLexicallyIncorrect (numberOfTokens);
    }

    --gIndenter;
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceTokensDetails) {
    gLogStream <<
      endl <<
      "<== scanAllTheInputAtOnce()" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlScanner::translateAllTheInputToKeywordsLanguage
// --------------------------------------------------------------------------

void msdlScanner::translateAllTheInputToKeywordsLanguage (
  msdlKeywordsLanguageKind keywordsTranslationLanguage,
  msdlCommentsTypeKind     commentsTypeKind)
{
#ifdef TRACING_IS_ENABLED
  if (fTraceTokens) {
    gLogStream <<
      "==> translateAllTheInputToKeywordsLanguage()" <<
      ", keywordsTranslationLanguage: " <<
      msdlKeywordsLanguageKindAsString (keywordsTranslationLanguage) <<
      endl;
  }
#endif

  if (fInputIsEmpty) {
#ifdef TRACING_IS_ENABLED
    gLogStream <<
      "Input is empty " <<
      endl;
#endif

    return;
  }

  // let's go ahead

  // is the MSDL keywords translation language needed?
  if (keywordsTranslationLanguage == fKeywordsInputLanguageKind) {
    fScannerWaeHandler->
      inputIsAlreadyInMsdlKeywordsLanguage (
        keywordsTranslationLanguage);

    return;
  }

  int numberOfTokens = 0;

  try {
    msdlTokenKind tokenKind = msdlTokenKind::k_NoToken;

    // fetch the first input character
    fetchNextCharacter ();

    do { // loop till end of stream
      // fetch the next token
      tokenKind = fetchNextToken (msdlIgnoreSeparatorTokensKind::kIgnoreSeparatorTokensNo);
      ++numberOfTokens;

#ifdef TRACING_IS_ENABLED
      if (fTraceTokens) {
        unsigned int fieldWidth = 23;

        gLogStream <<
          endl <<
          "--- translateAllTheInputToKeywordsLanguage()" <<
          endl;

        ++gIndenter;

        gLogStream << left <<
          setw (fieldWidth) <<
          "fCurrentToken" << " : " <<
          endl;

        ++gIndenter;
        gLogStream << fCurrentToken << endl;
        --gIndenter;

        --gIndenter;

        gLogStream << left <<
          setw (fieldWidth) <<
          "fCurrentTokenPositionInInput" << " : " << fCurrentTokenPositionInInput <<
          endl <<
          setw (fieldWidth) <<
          "fCurrentCharacter" << " : '" << currentCharacterAsString () <<
          endl <<
          setw (fieldWidth) <<
          "fNextCharacterIsAvailable" << " : " << booleanAsString (fNextCharacterIsAvailable) <<
          endl <<

          setw (fieldWidth) <<
          "fCurrentToken" << " : " <<
          endl;

        ++gIndenter;
        gLogStream << fCurrentToken << endl;
        --gIndenter;

        --gIndenter;
      }
#endif

      // write current token translated to standard output
//      gOutputStream <<
      cout << // JMI
        fCurrentToken.asMsdlString (
          keywordsTranslationLanguage,
          commentsTypeKind);
    }
    while (tokenKind != msdlTokenKind::k_TokenEOF); // do
  }
  catch (out_of_range& e) {
    displayException (e, gOutputStream);
  }
  catch (exception& e) {
    displayException (e, gOutputStream);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceTokens) {
    gLogStream <<
      endl <<
      "<== translateAllTheInputToKeywordsLanguage()" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlScanner::scanWholeInputAtOnce
// --------------------------------------------------------------------------

void msdlScanner::scanWholeInputAtOnce ()
{
#ifdef TRACING_IS_ENABLED
  if (fTraceTokensDetails) {
    gLogStream <<
      "==> scanWholeInputAtOnce()" <<
      endl;
  }
#endif

  if (fInputIsEmpty) {
#ifdef TRACING_IS_ENABLED
    gLogStream <<
      "Input is empty " <<
      endl;
#endif

    return;
  }

  // let's go ahead

  // should we ignore separator tokens?
  bool
    ignoreSeparatorTokens =
      gGlobalMsdl2msrOahGroup->
        getIgnoreSeparatorTokens ();

  msdlIgnoreSeparatorTokensKind
    ignoreSeparatorTokensKind =
      ignoreSeparatorTokens
        ? msdlIgnoreSeparatorTokensKind::kIgnoreSeparatorTokensYes
        : msdlIgnoreSeparatorTokensKind::kIgnoreSeparatorTokensNo;

  // should we translate the keywords?
  msdlKeywordsLanguageKind
    keywordsTranslationLanguageKind =
      gGlobalMsdl2msrOahGroup->
        getMsdlKeywordsTranslationLanguageKind ();

  // what comments type should we use in the translation output?
  msdlCommentsTypeKind
    commentsTypeKind =
      gGlobalMsdl2msrOahGroup->
        getMsdlCommentsTypeTranslationKind ();

  // do the job
  switch (keywordsTranslationLanguageKind) {
    case msdlKeywordsLanguageKind::k_NoKeywordsLanguage:
      scanAllTheInputAtOnce (
        ignoreSeparatorTokensKind);
      break;

    default:
      translateAllTheInputToKeywordsLanguage (
        keywordsTranslationLanguageKind,
        commentsTypeKind);
  } // switch
}


}
