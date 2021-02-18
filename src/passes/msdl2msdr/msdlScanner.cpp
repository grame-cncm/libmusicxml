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
  fCurrentPositionInInput = 0;

  fNextCharacterIsAvailable = false;
  fCurrentCharacter = '@'; // just to get it initialized

  fCurrentLineNumber = 1;
  fCurrentLineSize = 0;
  fCurrentPositionInLine = 1;       // user, not string,  view

  fCurrentTokenPositionInInput = 0;
  fCurrentTokenPositionInLine  = 1; // user, not string, view

  fTokensCounter = 0;

  fSourceIsLexicallyCorrect = true;

  fScannerWaeHandler = msdlScannerFrenchWaeHandler::create ();

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
  if (fTraceTokens) {
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
//  msdlScanner::nextCharacterPlease
// --------------------------------------------------------------------------

char msdlScanner::nextCharacterPlease ()
{
#ifdef TRACING_IS_ENABLED
  if (fTraceTokensDetails) {
    gLogStream <<
      "--> nextCharacterPlease()" <<
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

  if (! fNextCharacterIsAvailable) {
    // has the end of input been reached?
    if (fCurrentPositionInInput == fInputStringSize) {
      fCurrentCharacter = EOF;
    }
    else {
      // fetch new current character
      fCurrentCharacter = fInputString [fCurrentPositionInInput];
    }

    ++fCurrentPositionInInput;
    ++fCurrentPositionInLine;

    // fNextCharacterIsAvailable remains true;
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceTokensDetails) {
    gLogStream <<
      "<-- nextCharacterPlease() returns: " <<
      currentCharacterAsString () <<
      endl;
  }
#endif

  return fCurrentCharacter;
}

// --------------------------------------------------------------------------
//  msdlScanner::nextTokenPlease
// --------------------------------------------------------------------------

msdrTokenKind msdlScanner::nextTokenPlease ()
{
  ++fTokensCounter;

#ifdef TRACING_IS_ENABLED
  if (fTraceTokens) {
    gLogStream <<
      fTokensCounter << " ==> nextTokenPlease()" <<
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

  fCurrentTokenPositionInInput = fCurrentPositionInInput;
  fCurrentTokenPositionInLine  = fCurrentPositionInLine;

  ++gIndenter;

  // fetch the first character
  nextCharacterPlease ();

  switch (fCurrentCharacter) {
    case EOF:
#ifdef TRACING_IS_ENABLED
      if (fTraceTokensDetails) {
        gLogStream << "EOF has been reached" << endl;
      }
#endif

      fCurrentTokenKind = k_TokenEOF;
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
      fCurrentTokenKind = kTokenEndOfLine;
      fNextCharacterIsAvailable = false;

      ++fCurrentLineNumber;
      fCurrentPositionInLine = 1; // user, not string, view
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
      fNextCharacterIsAvailable = false;
      nextCharacterPlease ();

      if (fCurrentCharacter == '|') {
        nextCharacterPlease (); // ':|' up to this point

        if (fCurrentCharacter == '|') { // ':||' up to this point
          fCurrentTokenKind = kTokenFinalBar;
        }
        else {
          fScannerWaeHandler->illegalCharacters (":|");
        }
      }
      else {
        fCurrentTokenKind = kTokenColon;
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
      fNextCharacterIsAvailable = false;
      nextCharacterPlease ();

      if (fCurrentCharacter == '!') { // '!!' up to this point
        fCurrentTokenKind = kTokenConcat;
      }
      else {
        fScannerWaeHandler->illegalCharacter ('!');
        nextCharacterPlease ();
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
      fNextCharacterIsAvailable = false;
      nextCharacterPlease ();

      if (fCurrentCharacter == '|') {
        fCurrentTokenKind = kTokenDoubleBar;
      }
      else {
        fCurrentTokenKind = kTokenMeasure;
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

        stringstream s;

        s << fCurrentCharacter;

        fCurrentTokenKind = k_TokenMalformed;
        fCurrentTokenDescription.setString (s.str ());

        fNextCharacterIsAvailable = false;
      }
  } // switch (fCurrentCharacter)

  --gIndenter;

#ifdef TRACING_IS_ENABLED
  if (fTraceTokens) {
    gLogStream <<
      fTokensCounter << " <== nextTokenPlease()" <<
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
  nextCharacterPlease (); // consume the '/'

  switch (fCurrentCharacter) {
    case '*': // '/*' up to this point, parenthesized comment
      {
       nextCharacterPlease ();

        unsigned commentStartPositionInInput = fCurrentTokenPositionInInput;

        do {
          while (fCurrentCharacter != '*') {
            if (fCurrentCharacter == '\n') {
              fNextCharacterIsAvailable = false;
              nextCharacterPlease ();
            } // while

            nextCharacterPlease ();
          } // while

          nextCharacterPlease ();
        } // do
        while (fCurrentCharacter != '/');

        fNextCharacterIsAvailable = false;

gLogStream <<
  "commentStartPositionInInput: " << commentStartPositionInInput << endl <<
  "fCurrentPositionInInput: " << fCurrentPositionInInput << endl;

        string commentString =
          fInputString.substr (
            commentStartPositionInInput + 2,
            fCurrentPositionInInput - commentStartPositionInInput - 4);

        fCurrentTokenKind = kTokenParenthesizedComment;
        fCurrentTokenDescription.setString (commentString);
      }
      break;

    case '/': // '//' up to this point, comment to end of line
      {
        nextCharacterPlease (); // consume the second '/'

        unsigned commentStartPositionInInput = fCurrentTokenPositionInInput;

        while (fCurrentCharacter != '\n') {
          nextCharacterPlease ();
        } // while

        fNextCharacterIsAvailable = true;

        string commentString =
          fInputString.substr (
            commentStartPositionInInput + 2,
            fCurrentPositionInInput - commentStartPositionInInput - 1);

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
  nextCharacterPlease (); // consume the '%'

  switch (fCurrentCharacter) {
    case '{':  // '%{' up to this point, parenthesized comment
      {
        nextCharacterPlease ();

        unsigned commentStartPositionInInput = fCurrentTokenPositionInInput;

        do {
          while (fCurrentCharacter != '%') {
            if (fCurrentCharacter == '\n') {
              fNextCharacterIsAvailable = false;
              nextCharacterPlease ();
            } // while

            nextCharacterPlease ();
          } // while

          nextCharacterPlease ();
        } // do
        while (fCurrentCharacter != '}');

        fNextCharacterIsAvailable = false;

gLogStream <<
  "commentStartPositionInInput: " << commentStartPositionInInput << endl <<
  "fCurrentPositionInInput: " << fCurrentPositionInInput << endl;

        string commentString =
          fInputString.substr (
            commentStartPositionInInput + 2,
            fCurrentPositionInInput - commentStartPositionInInput - 4);

        fCurrentTokenKind = kTokenParenthesizedComment;
        fCurrentTokenDescription.setString (commentString);
      }
      break;

    case '%': // '%%' up to this point, comment to end of line
      {
        nextCharacterPlease (); // consume the second '%'

        unsigned commentStartPositionInInput = fCurrentTokenPositionInInput;

        while (fCurrentCharacter != '\n') {
          nextCharacterPlease ();
        } // while

        fNextCharacterIsAvailable = true;

        string commentString =
          fInputString.substr (
            commentStartPositionInInput + 2,
            fCurrentPositionInInput - commentStartPositionInInput - 1);

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
    char character = nextCharacterPlease ();

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

  // we've gone one character too far
  fNextCharacterIsAvailable = true;

  string nameString =
    fInputString.substr (
      nameStartPositionInInput,
      fCurrentPositionInInput - nameStartPositionInInput - 1);

  fCurrentTokenKind = kTokenIdentifier;
  fCurrentTokenDescription.setString (nameString);

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
    char character = nextCharacterPlease ();

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

  // we've gone one character too far
  fNextCharacterIsAvailable = true;

  string integerString =
    fInputString.substr (
      integerStartPositionInInput,
      fCurrentPositionInInput - integerStartPositionInInput - 1);

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
  // accept all the string characters, the double quote is available

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

  // forget previous current string
  fCurrentString = "";

  nextCharacterPlease (); // consume the '"'

  int chunkStartPositionInInput =
    stringStartPositionInInput;

  bool endOfStringHasBeenReached = false;

  while (true) { // infinite loop
    switch (fCurrentCharacter) {
      case '"':
        endOfStringHasBeenReached = true;
        break;

      case '\\':
        {
          // append the current chunk to fCurrentString
          string currentChunk =
            fInputString.substr (
              stringStartPositionInInput,
              fCurrentTokenPositionInInput - stringStartPositionInInput);
          appendChunkToCurrentString (currentChunk);

          nextCharacterPlease (); // consume the '\'

          // handle the escaped character
          switch (fCurrentCharacter) {
            case '"':
              appendChunkToCurrentString ("\"");
              break;
            case '\\':
              appendChunkToCurrentString ("\\");
              break;
            case 'n':
              appendChunkToCurrentString ("\n");
              break;
            case 't':
              appendChunkToCurrentString ("\t");
              break;
            default:
              fScannerWaeHandler->
                illegalEscapedCharacterInString (fCurrentCharacter);
              fNextCharacterIsAvailable = false;
          } //

          chunkStartPositionInInput = fCurrentTokenPositionInInput + 1;
        }
        break;

      case '\n':
        nextCharacterPlease ();

        ++fCurrentLineNumber;
        fCurrentPositionInLine = 1; // user, not string, view
        break;

      default:
        ; // nothing
      } //  switch

    if (endOfStringHasBeenReached)
      break;

    nextCharacterPlease ();
    } // while

  // append the current chunk to fCurrentString
  string currentChunk =
    fInputString.substr (
      stringStartPositionInInput,
      fCurrentPositionInInput - stringStartPositionInInput);
  appendChunkToCurrentString (currentChunk);

  fCurrentTokenKind = kTokenString;
  fCurrentTokenDescription.setString (fCurrentString);

  --gIndenter;

#ifdef TRACING_IS_ENABLED
  if (fTraceTokensDetails) {
    gLogStream <<
      endl <<
      "Accepting a string, found: \"" << fCurrentString << "\"" <<
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
//  msdlScanner::lexicalError
// --------------------------------------------------------------------------

void msdlScanner::lexicalError (string message)
{
  stringstream s;

  s <<
    "LexicalError on character: " <<
    fCurrentCharacter <<
    message;

  gLogStream << s.str () << endl;

  fSourceIsLexicallyCorrect = false;

  if (gGlobalMsdl2msdrOahGroup->getQuitOnLexicalErrors ()) {
    throw msgMsdlToMsdrScoreException (message);
  }
}

// --------------------------------------------------------------------------
//  msdlScanner::scan
// --------------------------------------------------------------------------

void msdlScanner::scan ()
{
#ifdef TRACING_IS_ENABLED
  if (fTraceTokens) {
    gLogStream <<
      "==> scan()" <<
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

  ++gIndenter;

  int numberOfTokens = 0;

  try {
    msdrTokenKind tokenKind = k_NoToken;

//    nextCharacterPlease (); // fetch the first character

    do { // loop till end of stream
      tokenKind = nextTokenPlease ();
      ++numberOfTokens;

#ifdef TRACING_IS_ENABLED
      if (false && fTraceTokens) {
        unsigned int fieldWidth = 23;

        gLogStream <<
          endl <<
          "--- scan()" <<
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
    } // do
    while (tokenKind != k_TokenEOF);
  }
  catch (out_of_range& e) {
    displayException (e, gOutputStream);
  }
  catch (exception& e) {
    displayException (e, gOutputStream);
  }

  --gIndenter;

  if (fSourceIsLexicallyCorrect) {
    gLogStream <<
      "Input is lexically correct, contains " <<
      numberOfTokens <<
      " tokens" <<
      endl;
  }
  else {
    gLogStream <<
      "### Input is lexically incorrect, contains " <<
      numberOfTokens <<
      " tokens" <<
      " ###" <<
      endl;
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceTokens) {
    gLogStream <<
      endl <<
      "<== scan()" <<
      endl;
  }
#endif
}


}
