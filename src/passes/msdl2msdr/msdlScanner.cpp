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

#include "msdl2msdrOah.h"


using namespace std;

namespace MusicXML2
{

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
    case kIgnoreSeparatorTokensNo:
      result = "ignoreSeparatorTokensNo";
      break;
    case kIgnoreSeparatorTokensYes:
      result = "ignoreSeparatorTokensYes";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
msdlScanner::msdlScanner (
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
  fCurrentPositionInInput = -1;

  fNextCharacterIsAvailable = false;
  fCurrentCharacter = '@'; // just to get it initialized

  fCurrentLineNumber = 1;
  fCurrentLineSize = -1;
  fCurrentPositionInLine = -1;

  fCurrentTokenPositionInInput = -1;
  fCurrentTokenPositionInLine  = -1;

  fTokensCounter = 0;

  fKeywordsInputLanguageKind = kKeywordsEnglish, // MSDL default

  fSourceIsLexicallyCorrect = true;

  // create the MSDL scanner WAE handler
  switch (gGlobalMsdl2msdrOahGroup->getMsdlKeywordsLanguageKind ()) {
    case k_NoKeywordLanguage:
      // should not occur
      break;

    case kKeywordsEnglish:
      fScannerWaeHandler = msdlScannerEnglishWaeHandler::create ();
      break;
    case kKeywordsFrench:
      fScannerWaeHandler = msdlScannerFrenchWaeHandler::create ();
      break;
    case kKeywordsItalian:
      fScannerWaeHandler = msdlScannerItalianWaeHandler::create ();
      break;
    case kKeywordsGerman:
      fScannerWaeHandler = msdlScannerGermanWaeHandler::create ();
      break;
    case kKeywordsSpanish:
      fScannerWaeHandler = msdlScannerSpanishWaeHandler::create ();
      break;
    case kKeywordsNederlands:
      fScannerWaeHandler = msdlScannerDutchWaeHandler::create ();
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  fTraceTokens        = gGlobalMsdl2msdrOahGroup->getTraceTokens ();
  fTraceTokensDetails = gGlobalMsdl2msdrOahGroup->getTraceTokensDetails ();
#endif

  fAppendTokensToList = false;

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
  if (fTraceTokens) {
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

  fInputStringSize = fInputString.size ();

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
      ", fNextCharacterIsAvailable: " <<
      booleanAsString (fNextCharacterIsAvailable) <<
      ", fCurrentPositionInInput: " <<
      fCurrentPositionInInput <<
      ", fCurrentTokenPositionInInput: " <<
      fCurrentTokenPositionInInput <<
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
//  msdlScanner::fetchNextToken
// --------------------------------------------------------------------------

void msdlScanner::handleEndOfLine ()
{
  ++fCurrentLineNumber;
  fCurrentPositionInLine = -1;
}

// --------------------------------------------------------------------------
//  msdlScanner::fetchNextToken
// --------------------------------------------------------------------------

msdrTokenKind msdlScanner::fetchNextToken (
  msdlIgnoreSeparatorTokensKind
    ignoreSeparatorTokens)
{
  ++fTokensCounter;

#ifdef TRACING_IS_ENABLED
  if (fTraceTokens) {
    gLogStream <<
      fTokensCounter << " ==> fetchNextToken()" <<
      ", ignoreSeparatorTokens: " <<
      msdlIgnoreSeparatorTokensKindAsString (ignoreSeparatorTokens) <<
      endl;
  }

  if (fTraceTokensDetails) {
    ++gIndenter;

    unsigned int fieldWidth = 23;

    gLogStream << left <<
      setw (fieldWidth) <<
      ", fNextCharacterIsAvailable: " << " : " <<
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
      endl;

    --gIndenter;
  }
#endif

  ++gIndenter;

  // fetch the first token character if needed
  if (! fNextCharacterIsAvailable) {
    fetchNextCharacter ();
  }

  // set the token positions
  fCurrentTokenPositionInInput = fCurrentPositionInInput;
  fCurrentTokenPositionInLine  = fCurrentPositionInLine;

  // do the actual work at least once, ignoring separator tokens if needed
  for ( ; ; ) {

    switch (fCurrentCharacter) {
      case EOF:
#ifdef TRACING_IS_ENABLED
        if (fTraceTokensDetails) {
          gLogStream << "EOF has been reached" << endl;
        }
#endif

        fCurrentTokenKind = k_TokenEOF;
        fNextCharacterIsAvailable = true; // don't go ahead anymore
        break;

      case ' ' :
        fCurrentTokenKind = kTokenSpace;
        fNextCharacterIsAvailable = false;
        break;
      case '\t' :
        fCurrentTokenKind = kTokenTab;
        fNextCharacterIsAvailable = false;
        break;
      case '\xD' :
        fCurrentTokenKind = kTokenCarriageReturn;
        fNextCharacterIsAvailable = false;
        break;

      case '\n' :
        handleEndOfLine ();

        fCurrentTokenKind = kTokenEndOfLine;
        fNextCharacterIsAvailable = false;
        break;

      case '/':
        handleSlash ();
        break;

      case '%':
        handlePercent ();
        break;

      case '=' :
        fCurrentTokenKind = kTokenEqualSign;
        fNextCharacterIsAvailable = false;
        break;

      case ',' :
        fCurrentTokenKind = kTokenComma;
        fNextCharacterIsAvailable = false;
        break;

      case ':' :
        fetchNextCharacter ();

        if (fCurrentCharacter == '|') {
          fetchNextCharacter (); // ':|' up to this point

          if (fCurrentCharacter == '|') { // ':||' up to this point
            fCurrentTokenKind = kTokenFinalBar;
            fNextCharacterIsAvailable = false;
          }
          else {
            fScannerWaeHandler->illegalCharacters (":|");
            fSourceIsLexicallyCorrect = false;
            fNextCharacterIsAvailable = false;
          }
        }
        else {
          fCurrentTokenKind = kTokenColon;
          fNextCharacterIsAvailable = false;
        }
        break;

      case ';' :
        fCurrentTokenKind = kTokenSemiColon;
        fNextCharacterIsAvailable = false;
        break;

      case '+' :
        fCurrentTokenKind = kTokenPlus;
        fNextCharacterIsAvailable = false;
        break;
      case '-' :
        fCurrentTokenKind = kTokenMinus;
        fNextCharacterIsAvailable = false;
        break;
      case '*' :
        fCurrentTokenKind = kTokenStar;
        fNextCharacterIsAvailable = false;
        break;

      case '!':
        fetchNextCharacter ();

        if (fCurrentCharacter == '!') { // '!!' up to this point
          fCurrentTokenKind = kTokenConcat;
          fNextCharacterIsAvailable = false;
        }
        else {
          fScannerWaeHandler->illegalCharacter ('!');
          fSourceIsLexicallyCorrect = false;
          fNextCharacterIsAvailable = false;
        }
        break;

      case '?' :
        fCurrentTokenKind = kTokenQuestionMark;
        fNextCharacterIsAvailable = false;
        break;

      case '(' :
        fCurrentTokenKind = kTokenLeftParenthesis;
        fNextCharacterIsAvailable = false;
        break;
      case ')' :
        fCurrentTokenKind = kTokenRightParenthesis;
        fNextCharacterIsAvailable = false;
        break;

      case '{' :
        fCurrentTokenKind = kTokenLeftBracket;
        fNextCharacterIsAvailable = false;
        break;
      case '}' :
        fCurrentTokenKind = kTokenRightBracket;
        fNextCharacterIsAvailable = false;
        break;

     case '\'':
        fCurrentTokenKind = kTokenQuote;
        fNextCharacterIsAvailable = false;
        break;

      case '.' :
        fCurrentTokenKind = kTokenDot;
        fNextCharacterIsAvailable = false;
        break;

       case '|' :
        fetchNextCharacter ();

        if (fCurrentCharacter == '|') { // '||' up to this point
          fetchNextCharacter ();

          if (fCurrentCharacter == '|') { // '|||' up to this point
            fCurrentTokenKind = kTokenFinalBar;
            fNextCharacterIsAvailable = false;
          }
          else {
            fCurrentTokenKind = kTokenDoubleBar;
            fNextCharacterIsAvailable = false;
          }
        }
        else {
          fCurrentTokenKind = kTokenMeasure;
          fNextCharacterIsAvailable = false;
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

          fCurrentTokenKind = k_TokenMalformed;
          fCurrentTokenDescription.setString (s.str ());

          fNextCharacterIsAvailable = false;
        }
    } // switch (fCurrentCharacter)

#ifdef TRACING_IS_ENABLED
    if (fTraceTokensDetails) {
      gLogStream <<
        "--- fetchNextToken(): fCurrentTokenKind = " <<
        msdrTokenKindAsString (fCurrentTokenKind) <<
        endl;
    }
#endif

    // should we loop once more?
    bool onceMore = false;

    switch (fCurrentTokenKind) {
      case k_TokenEOF:
        break;

      case k_NoToken:
        // should not occur
        break;

      case k_TokenMalformed:
        onceMore = true; // false for error recovery??? JMI
        break;

      // language-independent tokens
      // ------------------------------------

      case kTokenSpace:
      case kTokenTab:
      case kTokenCarriageReturn:
      case kTokenEndOfLine:
      case kTokenParenthesizedComment:
      case kTokenCommentToEndOfLine:
        onceMore = ignoreSeparatorTokens == kIgnoreSeparatorTokensYes;
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
      // set the token positions
      fCurrentTokenPositionInInput = fCurrentPositionInInput;
      fCurrentTokenPositionInLine  = fCurrentPositionInLine;

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

#ifdef TRACING_IS_ENABLED
  if (fTraceTokens) {
    gLogStream <<
      fTokensCounter << " <== fetchNextToken()" <<
      endl;

    ++gIndenter;

    gLogStream <<
      "fCurrentToken: " <<
      endl;

    ++gIndenter;
    gLogStream << fCurrentToken;
    --gIndenter;

    gLogStream << endl; // trace separator

    --gIndenter;
  }
#endif

  fCurrentToken.setTokenLineNumber (fCurrentLineNumber);
  fCurrentToken.setTokenPositionInLine (fCurrentTokenPositionInLine);

  if (fAppendTokensToList) {
    fTokensList.appendTokenToTokensList (fCurrentToken);
  }

  return fCurrentTokenKind;
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

        unsigned commentStartPositionInInput = fCurrentTokenPositionInInput;

        do {
          while (fCurrentCharacter != '*') {
            if (fCurrentCharacter == '\n') {
              handleEndOfLine ();

              fNextCharacterIsAvailable = false;
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
            "commentStartPositionInInput: " <<
            commentStartPositionInInput << endl <<
            "fCurrentPositionInInput: " <<
            fCurrentPositionInInput <<
            endl;
        }
#endif

        string commentString =
          fInputString.substr (
            commentStartPositionInInput + 2,
            fCurrentPositionInInput - commentStartPositionInInput - 3);

        fCurrentTokenKind = kTokenParenthesizedComment;
        fCurrentTokenDescription.setString (commentString);

        fNextCharacterIsAvailable = false;
      }
      break;

    case '/': // '//' up to this point, comment to end of line
      {
        // consume the second '/'
        fetchNextCharacter ();

        unsigned commentStartPositionInInput = fCurrentTokenPositionInInput;

        while (fCurrentCharacter != '\n') {
          handleEndOfLine ();

          // consume the '\n'
          fetchNextCharacter ();
        } // while

        // the end of the comment to end of line has been reached
        fNextCharacterIsAvailable = true;

        string commentString =
          fInputString.substr (
            commentStartPositionInInput + 2,
            fCurrentPositionInInput - commentStartPositionInInput - 2);

        fCurrentTokenKind = kTokenCommentToEndOfLine;
        fCurrentTokenDescription.setString (commentString);
      }
      break;

    default:
      fCurrentTokenKind = kTokenSlash;
      fNextCharacterIsAvailable = false;
  } // switch
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

        unsigned commentStartPositionInInput = fCurrentTokenPositionInInput;

        do {
          while (fCurrentCharacter != '%') {
            if (fCurrentCharacter == '\n') {
              handleEndOfLine ();

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
            "commentStartPositionInInput: " <<
            commentStartPositionInInput <<
            endl <<
            "fCurrentPositionInInput: " <<
            fCurrentPositionInInput <<
            endl;
        }
#endif

        string commentString =
          fInputString.substr (
            commentStartPositionInInput + 2,
            fCurrentPositionInInput - commentStartPositionInInput - 3);

        fCurrentTokenKind = kTokenParenthesizedComment;
        fCurrentTokenDescription.setString (commentString);
      }
      break;

    case '%': // '%%' up to this point, comment to end of line
      {
        // consume the second '%'
        fetchNextCharacter ();

        unsigned commentStartPositionInInput = fCurrentTokenPositionInInput;

        while (fCurrentCharacter != '\n') {
          // consume this comment character
          fetchNextCharacter ();
        } // while

        // the end of the comment to end of line has been reached
        handleEndOfLine ();

        fNextCharacterIsAvailable = true;

        string commentString =
          fInputString.substr (
            commentStartPositionInInput + 2,
            fCurrentPositionInInput - commentStartPositionInInput - 2);

        fCurrentTokenKind = kTokenCommentToEndOfLine;
        fCurrentTokenDescription.setString (commentString);
      }
      break;

    default:
      fCurrentTokenKind = kTokenPercent;
      fNextCharacterIsAvailable = false;
  } // switch
}

// --------------------------------------------------------------------------
//  msdlScanner::acceptAName
// --------------------------------------------------------------------------

void msdlScanner::acceptAName ()
{
  // accept all alphanumeric characters and '_', the first letter is available

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

  // a name can be an identifier or a keyword

  bool nonNameCharacterFound = false;

  while (! nonNameCharacterFound) {
    char character = fetchNextCharacter ();

    switch (character) {
      case EOF:
        nonNameCharacterFound = true;
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
      case '_':
        break;

      default:
        nonNameCharacterFound = ! isalpha (fCurrentCharacter);
    } // switch
  } // while

  // the end of the name has been reached
  fNextCharacterIsAvailable = true;

  string nameString =
    fInputString.substr (
      nameStartPositionInInput,
      fCurrentPositionInInput - nameStartPositionInInput);

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
    case k_NoMsdlKeywordKind: // no, it is an identifier
      fCurrentTokenKind = kTokenIdentifier;
      fCurrentTokenDescription.setString (nameString);
      break;

      // language-dependent keywords
      // ------------------------------------

    case kMsdlKeywordTitle:
      fCurrentTokenKind = kTokenTitle;
      break;
    case kMsdlKeywordComposer:
      fCurrentTokenKind = kTokenComposer;
      break;
    case kMsdlKeywordOpus:
      fCurrentTokenKind = kTokenOpus;
      break;

    case kMsdlKeywordPitches:
      fCurrentTokenKind = kTokenPitches;
      break;

    case kMsdlKeywordAnacrusis:
      fCurrentTokenKind = kTokenAnacrusis;
      break;

    case kMsdlKeywordBook:
      fCurrentTokenKind = kTokenBook;
      break;
    case kMsdlKeywordScore:
      fCurrentTokenKind = kTokenScore;
      break;
    case kMsdlKeywordPartGroup:
      fCurrentTokenKind = kTokenPartGroup;
      break;
    case kMsdlKeywordPart:
      fCurrentTokenKind = kTokenPart;
      break;
    case kMsdlKeywordStaff:
      fCurrentTokenKind = kTokenStaff;
      break;
    case kMsdlKeywordVoice:
      fCurrentTokenKind = kTokenVoice;
      break;
    case kMsdlKeywordFragment:
      fCurrentTokenKind = kTokenFragment;
      break;

    case kMsdlKeywordClef:
      fCurrentTokenKind = kTokenClef;
      break;
    case kMsdlKeywordTreble:
      fCurrentTokenKind = kTokenTreble;
      break;
    case kMsdlKeywordSoprano:
      fCurrentTokenKind = kTokenSoprano;
      break;
    case kMsdlKeywordAlto:
      fCurrentTokenKind = kTokenAlto;
      break;
    case kMsdlKeywordTenor:
      fCurrentTokenKind = kTokenTenor;
      break;
    case kMsdlKeywordBaryton:
      fCurrentTokenKind = kTokenBaryton;
      break;
    case kMsdlKeywordBass:
      fCurrentTokenKind = kTokenBass;
      break;

    case kMsdlKeywordKey:
      fCurrentTokenKind = kTokenKey;
      break;

    case kMsdlKeywordTime:
      fCurrentTokenKind = kTokenTime;
      break;
  } // switch

  // set the token description keyword kind if relevant
  if (fCurrentTokenKind != kTokenIdentifier) {
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

  // the end of the integer has been reached
  fNextCharacterIsAvailable = true;

  string integerString =
    fInputString.substr (
      integerStartPositionInInput,
      fCurrentPositionInInput - integerStartPositionInInput);

  fCurrentTokenKind =  kTokenInteger;
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
              handleEndOfLine ();

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
        handleEndOfLine ();

        theString += "\n";
        fNextCharacterIsAvailable = false;

        ++fCurrentLineNumber;
        fCurrentPositionInLine = -1;
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

  fCurrentTokenKind = kTokenString;
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
//  msdlScanner::currentPositionAsString
// --------------------------------------------------------------------------

string msdlScanner::currentPositionAsString () const
{
  string result;

  string separateur =
          "-----------------------------------------------------";

#ifdef TRACING_IS_ENABLED
  if (fTraceTokensDetails) {
  /*
    return
      MiseEnForme (
        gLANGUE_Lexique ->
          FormatPositionLexicaleCourante (),
        fCurrentLineNumber,
        positionCourante - fDebutLigneCourante + 1,
        fCurrentCharacter,
        fCurrentCharacter,
        separateur,
        fTamponAuxiliaire,
        separateur );
        */
  }
#endif

  /*
    return
      MiseEnForme (
        "[L%3d:C%3d]",
        fCurrentLineNumber,
        positionCourante - fDebutLigneCourante + 1 );
      */

  return result;
}

// --------------------------------------------------------------------------
//  msdlScanner::scanAllTheInputAtOnce
// --------------------------------------------------------------------------

void msdlScanner::scanAllTheInputAtOnce (
  msdlIgnoreSeparatorTokensKind
    ignoreSeparatorTokens)
{
#ifdef TRACING_IS_ENABLED
  if (fTraceTokens) {
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

    return;
  }

  // let's go ahead

  ++gIndenter;

  int numberOfTokens = 0;

  try {
    msdrTokenKind tokenKind = k_NoToken;

    // fetch the first input character
    fetchNextCharacter ();

    do { // loop till end of stream
      tokenKind = fetchNextToken (ignoreSeparatorTokens);
      ++numberOfTokens;

#ifdef TRACING_IS_ENABLED
      if (false && fTraceTokens) {
        unsigned int fieldWidth = 23;

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
          "fCurrentCharacter" << " : '" << currentCharacterAsString () <<
          endl <<

          setw (fieldWidth) <<
          "fCurrentToken" << " : " <<
          endl;

        ++gIndenter;
        gLogStream << fCurrentToken << endl;
        --gIndenter;
/*
        gLogStream << left <<
          setw (fieldWidth) <<
          "fCurrentTokenKind" << " : " << msdrTokenKindAsString (fCurrentTokenKind) <<
          endl;
*/
        --gIndenter;
      }
#endif
    }
    while (tokenKind != k_TokenEOF); // do
  }
  catch (out_of_range& e) {
    displayException (e, gOutputStream);
  }
  catch (exception& e) {
    displayException (e, gOutputStream);
  }

  --gIndenter;

  if (fSourceIsLexicallyCorrect) {
    fScannerWaeHandler->
      inputIsLexicallyCorrect (numberOfTokens);
  }
  else {
    fScannerWaeHandler->
      inputIsLexicallyIncorrect (numberOfTokens);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceTokens) {
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

void msdlScanner::translateAllTheInputToKeywordsLanguage ()
{
#ifdef TRACING_IS_ENABLED
  if (fTraceTokens) {
    gLogStream <<
      "==> translateAllTheInputToKeywordsLanguage()" <<
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

  // create the MSDL keywords translation language
  msdlKeywordsLanguageKind
    keywordsTranslationLanguage =
      gGlobalMsdl2msdrOahGroup->
        getMsdlKeywordsTranslationLanguageKind ();

  if (keywordsTranslationLanguage == fKeywordsInputLanguageKind) {
    fScannerWaeHandler->
      inputIsAlreadyInMsdlKeywordsLanguage (
        keywordsTranslationLanguage);

    return;
  }

  int numberOfTokens = 0;

  try {
    msdrTokenKind tokenKind = k_NoToken;

    // fetch the first input character
    fetchNextCharacter ();

    do { // loop till end of stream
      // get the next token
      tokenKind = fetchNextToken (kIgnoreSeparatorTokensNo);
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
        /*
          setw (fieldWidth) <<
          "fCurrentTokenPositionInInput" << " : " << fCurrentTokenPositionInInput <<
          endl <<
          setw (fieldWidth) <<
          "fCurrentCharacter" << " : '" << currentCharacterAsString () <<
          endl <<
*/
          setw (fieldWidth) <<
          "fCurrentToken" << " : " <<
          endl;

        ++gIndenter;
        gLogStream << fCurrentToken << endl;
        --gIndenter;
/*
        gLogStream << left <<
          setw (fieldWidth) <<
          "fCurrentTokenKind" << " : " << msdrTokenKindAsString (fCurrentTokenKind) <<
          endl;
*/
        --gIndenter;
      }
#endif

      // write it translated to standard output
//      gOutputStream <<
      cout << // JMI
        fCurrentToken.asMsdlString (keywordsTranslationLanguage);
    }
    while (tokenKind != k_TokenEOF); // do
  }
  catch (out_of_range& e) {
    displayException (e, gOutputStream);
  }
  catch (exception& e) {
    displayException (e, gOutputStream);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceTokens) {
    if (fSourceIsLexicallyCorrect) {
      fScannerWaeHandler->
        inputIsLexicallyCorrect (numberOfTokens);
    }
    else {
      fScannerWaeHandler->
        inputIsLexicallyIncorrect (numberOfTokens);
    }

    gLogStream <<
      endl <<
      "<== translateAllTheInputToKeywordsLanguage()" <<
      endl;
  }
#endif
}


}
