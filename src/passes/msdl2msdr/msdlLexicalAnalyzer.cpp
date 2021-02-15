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

#include "msdlLexicalEnglishWaeHandler.h"
#include "msdlLexicalFrenchWaeHandler.h"

#include "msdlLexicalAnalyzer.h"

#include "waeExceptions.h"

#include "msdl2msdrOah.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
msdlLexicalAnalyzer::msdlLexicalAnalyzer (
  istream& inputStream)
    : fInputStream (inputStream)
{
  fCurrentCharacter = '$';
  fCurrentPositionInLine = 0;

  fCurrentLineSize = 0;
  fCurrentLineNumber = 1;

  fCurrentInteger = 0;
  fCurrentDouble  = 0.0;

  fSourceIsLexicallyCorrect = true;

  fLexicalWaeHandler = msdlLexicalFrenchWaeHandler::create ();

#ifdef TRACING_IS_ENABLED
  fTraceTokens = gGlobalMsdl2msdrOahGroup->getTraceTokens ();
#endif

  fAppendTokensToList = false;
}

msdlLexicalAnalyzer::~ msdlLexicalAnalyzer ()
{}

void msdlLexicalAnalyzer::nextCharacterPlease ()
{
  // should we get the next line of input?
  if (fCurrentPositionInLine == fCurrentLineSize) {
    getline (fInputStream, fCurrentLine);
    fCurrentLineSize = fCurrentLine.size ();

    // did that reach the end of the input?
    if (fInputStream.eof ()) {
      cout << "[EOF reached]\n";
    }
    else {
      cout << "[error reading]\n";
    }
  }

  fCurrentCharacter = fCurrentLine [fCurrentPositionInLine];
  ++fCurrentPositionInLine;
}

void msdlLexicalAnalyzer::oneCharacterBackwards ()
{
  fInputStream.putback (fCurrentCharacter);
}

// --------------------------------------------------------------------------
//  msdlLexicalAnalyzer::currentPositionAsString
// --------------------------------------------------------------------------

string msdlLexicalAnalyzer::currentPositionAsString () const
{
  string result;

  string separateur =
          "-----------------------------------------------------";

#ifdef TRACING_IS_ENABLED
  if (fTraceTokens) {
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
//  msdlLexicalAnalyzer::lexicalError
// --------------------------------------------------------------------------

void msdlLexicalAnalyzer::lexicalError (string message)
{
  stringstream s;

  s <<
    "LexicalError on character: " <<
    fCurrentCharacter <<
    message;

  cout << s.str () << endl;

  fSourceIsLexicallyCorrect = false;

  if (gGlobalMsdl2msdrOahGroup->getQuitOnLexicalErrors ()) {
    throw msgMsdlToMsdrScoreException (message);
  }
}

// --------------------------------------------------------------------------
//  msdlLexicalAnalyzer::handleAnEndOfLine
// --------------------------------------------------------------------------

void
msdlLexicalAnalyzer::handleAnEndOfLine ()
{
  ++fCurrentLineNumber;
 }

// --------------------------------------------------------------------------
//  msdlLexicalAnalyzer::EchoDesCaracteresAcceptes
// --------------------------------------------------------------------------

void
msdlLexicalAnalyzer::EchoDesCaracteresAcceptes ()
{
}

// --------------------------------------------------------------------------
//  msdlLexicalAnalyzer::nextTokenPlease
// --------------------------------------------------------------------------

msdrTokenKind msdlLexicalAnalyzer::nextTokenPlease ()
{
  // the first character is already available
  fTokenStartPosition = fCurrentPositionInLine;
  nextCharacterPlease ();

// LOOP_START:

  switch (fCurrentCharacter) {
    case ' ' :
    case '\t' :
    case '\xD' :
#ifdef TRACING_IS_ENABLED
      if (fTraceTokens) {
        cout << fCurrentCharacter;
      }
#endif

      nextCharacterPlease ();
      fTokenStartPosition = fCurrentPositionInLine;

      fCurrentTokenKind = kTokenBlanks;
      break;

    case '\n' :
#ifdef TRACING_IS_ENABLED
      if (fTraceTokens) {
        cout << fCurrentCharacter;
      }
#endif

      nextCharacterPlease ();
      handleAnEndOfLine ();

      fTokenStartPosition = fCurrentPositionInLine;

      fCurrentTokenKind = kTokenEndOfLine;
      break;

    case '/':
      nextCharacterPlease ();

      switch (fCurrentCharacter) {
        case '*': // '/*' up to this point, parenthesized comment
          nextCharacterPlease ();

          do {
            while (fCurrentCharacter != '*') { // '**' up to this point
              if (fCurrentCharacter == '\n') {
                handleAnEndOfLine ();
              } // while

              nextCharacterPlease ();
            } // while

            nextCharacterPlease ();
          } // do
          while (fCurrentCharacter != '/');

#ifdef TRACING_IS_ENABLED
          if (fTraceTokens) {
            EchoDesCaracteresAcceptes ();
          }
#endif

          nextCharacterPlease ();

          fTokenStartPosition = fCurrentPositionInLine;

          fCurrentTokenKind = kTokenParenthesizedComment;
          break;

        default:
          oneCharacterBackwards ();
          fCurrentTokenKind = kTokenSlash;
        } // switch
      break;

    case '%':
      nextCharacterPlease ();

      switch (fCurrentCharacter) {
        case '{':  // '%{' up to this point, parenthesized comment
          nextCharacterPlease ();

          do {
            while (fCurrentCharacter != '%') {
              if (fCurrentCharacter == '\n') {
                handleAnEndOfLine ();
              } // while

              nextCharacterPlease ();
            } // while

            nextCharacterPlease ();
          } // do
          while (fCurrentCharacter != '}');

#ifdef TRACING_IS_ENABLED
          if (fTraceTokens) {
            EchoDesCaracteresAcceptes ();
          }
#endif

          nextCharacterPlease ();

          fTokenStartPosition = fCurrentPositionInLine;

          fCurrentTokenKind = kTokenParenthesizedComment;
          break;

        default: // '%' up to this point, comment to end of line
          do {
            nextCharacterPlease ();
          } // do
          while (fCurrentCharacter != '\n');

          oneCharacterBackwards ();

#ifdef TRACING_IS_ENABLED
          if (fTraceTokens) {
            EchoDesCaracteresAcceptes ();
          }
#endif

          fTokenStartPosition = fCurrentPositionInLine;

          fCurrentTokenKind = kTokenCommentToEndOfLine;
        } // switch
      break;

    case '=' :
      fCurrentTokenKind = kTokenEqualSign;
      break;

    case ',' :
      fCurrentTokenKind = kTokenComma;
      break;

    case ':' :
      nextCharacterPlease ();

      if (fCurrentCharacter == '|') {
        nextCharacterPlease (); // ':|' up to this point

        if (fCurrentCharacter == '|') { // ':||' up to this point
          fCurrentTokenKind = kTokenFinalBar;
        }
        else {
          fLexicalWaeHandler->illegalCharacters (":|");
        }
      }
      else {
        fCurrentTokenKind = kTokenColon;
      }
      break;

    case ';' :
      fCurrentTokenKind = kTokenSemiColon;
      break;

    case '+' :
      fCurrentTokenKind = kTokenPlus;
      break;
    case '-' :
      fCurrentTokenKind = kTokenMinus;
      break;
    case '*' :
      fCurrentTokenKind = kTokenStar;
      break;

    case '!':
      nextCharacterPlease ();

      if (fCurrentCharacter == '!') { // '!!' up to this point
        fCurrentTokenKind = kTokenConcat;
      }
      else {
#ifdef TRACING_IS_ENABLED
        if (fTraceTokens) {
          cout << fCurrentCharacter;
        }
 #endif
       fLexicalWaeHandler->illegalCharacter ('!');

        nextCharacterPlease ();
        fTokenStartPosition = fCurrentPositionInLine;

        // goto LOOP_START;
      }
      break;

    case '?' :
      fCurrentTokenKind = kTokenQuestionMark;
      break;

    case '(' :
      fCurrentTokenKind = kTokenLeftParenthesis;
      break;
    case ')' :
      fCurrentTokenKind = kTokenRightParenthesis;
      break;

    case '{' :
      fCurrentTokenKind = kTokenLeftBracket;
      break;
    case '}' :
      fCurrentTokenKind = kTokenRightBracket;
      break;

    case '.' :
      fCurrentTokenKind = kTokenDot;
      break;

     case '|' :
      nextCharacterPlease ();

      if (fCurrentCharacter == '|') {
        fCurrentTokenKind = kTokenDoubleBar;
      }
      else {
        fCurrentTokenKind = kTokenMeasure;
      }
      break;

   case '\'': //  single-quoted string
#ifdef TRACING_IS_ENABLED
      if (fTraceTokens) {
        cout << fCurrentCharacter;
      }
#endif

      fCurrentTokenKind = acceptASingleQuotedString ();
      break;

    case '"': //  double-quoted string
#ifdef TRACING_IS_ENABLED
      if (fTraceTokens) {
        cout << fCurrentCharacter;
      }
#endif

      fCurrentTokenKind = acceptADoubleQuotedString ();
      break;

    default:

      if (isalpha (fCurrentCharacter)) { //  name
        fCurrentTokenKind = acceptAName ();
      }

      else if (isdigit (fCurrentCharacter)) { // number
        fCurrentTokenKind = acceptANumber ();
      }

      else { // illegal character
        fLexicalWaeHandler->illegalCharacter (fCurrentCharacter);

#ifdef TRACING_IS_ENABLED
        if (fTraceTokens) {
          cout << fCurrentCharacter;
        }
#endif

        nextCharacterPlease ();
        fTokenStartPosition = fCurrentPositionInLine;

        // goto LOOP_START;
      }
    }  //  switch (fCurrentCharacter)

#ifdef TRACING_IS_ENABLED
  if (fTraceTokens) {
    ++gIndenter;
    cout <<
      msdrTokenKindAsString (fCurrentTokenKind) << endl;
    --gIndenter;
  }
#endif

/*
  if (fTraceTokens) {
    switch (fCurrentTokenKind) {
      case kTokenIdentifier :
      case t_FIN :
        break;
      default :
        EchoDesCaracteresAcceptes ();
      }  // switch
    }  // if
*/
  return fCurrentTokenKind;
}


// --------------------------------------------------------------------------
//  msdlLexicalAnalyzer::acceptAName
// --------------------------------------------------------------------------

msdrTokenKind msdlLexicalAnalyzer::acceptAName ()
{
  // a name can be an identifier or a keyword
  do { // accepting all alphanumeric characters and '_'
    nextCharacterPlease ();
  } // do
  while (isalnum (fCurrentCharacter) || fCurrentCharacter == '_');

  oneCharacterBackwards ();

/*
  unsigned int longueurIdent =
    fCurrentPositionInLine - fTokenStartPosition + 1;
*/
  return kTokenIdentifier;
}


// --------------------------------------------------------------------------
//  msdlLexicalAnalyzer::acceptANumber
// --------------------------------------------------------------------------

msdrTokenKind msdlLexicalAnalyzer::acceptANumber ()
{
  // accept digits sequence
  do {
    nextCharacterPlease ();
  } // do
  while (isdigit (fCurrentCharacter));

  bool doubleNumber = true;

  switch (fCurrentCharacter) {
    case '.' :
      nextCharacterPlease ();

      if (isdigit (fCurrentCharacter)) {
        do { //  on consomme tous les chiffres
          nextCharacterPlease ();
        } // do
        while (isdigit (fCurrentCharacter));

        if (
          (fCurrentCharacter == 'E') || (fCurrentCharacter == 'e')
        ) {
          acceptAnExponent ();
        }
      }
      else {
        fLexicalWaeHandler->digitExpectedAfterDecimalDot (fCurrentCharacter);
      }
      break;

    case 'E' :
    case 'e' :
      acceptAnExponent ();
      break;

    default:
      doubleNumber = false;
  }  //  switch

  oneCharacterBackwards ();

  int longueurNombre = // JMI
    fCurrentPositionInLine - fTokenStartPosition + 1;

  if (doubleNumber) {
    fCurrentDouble = stod (fCurrentName);
    return kTokenDouble;
  }
  else {
    fCurrentInteger = stoi (fCurrentName);
    return kTokenDouble;
  }
}

// --------------------------------------------------------------------------
//  msdlLexicalAnalyzer::acceptAnExponent
// --------------------------------------------------------------------------

void msdlLexicalAnalyzer::acceptAnExponent ()
{
  // here, fCurrentCharacter == '+' || (fCurrentCharacter == '-'
  nextCharacterPlease ();

  if (isdigit (fCurrentCharacter)) {
    do { // on consomme tous les chiffres
      nextCharacterPlease ();
    } // do
    while (isdigit (fCurrentCharacter));
  }

  else {
    fLexicalWaeHandler->digitExpectedInExponent (fCurrentCharacter);
  }
}


// --------------------------------------------------------------------------
//  msdlLexicalAnalyzer::acceptASingleQuotedString
// --------------------------------------------------------------------------
msdrTokenKind msdlLexicalAnalyzer::acceptASingleQuotedString ()
{
  bool    finChaineAtteinte = false;

  nextCharacterPlease ();  // consomme le '\''

  fTokenStartPosition = fCurrentPositionInLine;

  int positionDebutDuTroncon =
    fTokenStartPosition;

  // forget previous current string
  fCurrentString = "";

  while (true) // boucle infinie
    {
    switch (fCurrentCharacter) {
      case '\'':
        finChaineAtteinte = true;
        break;

      case '\\':
        {
          // on appond le tronon courant a fCurrentString
/*
          appendFragmentToCurrentString (
            fProducteurDeCaracteres ->
              ExtraireLaChaine (
                positionDebutDuTroncon,
                fCurrentPositionInLine - positionDebutDuTroncon ));
*/

          nextCharacterPlease ();

          switch (fCurrentCharacter) {
            case '"':
              appendFragmentToCurrentString ("\"");
              break;
            case '\\':
              appendFragmentToCurrentString ("\\");
              break;
            case 'n':
              appendFragmentToCurrentString ("\n");
              break;
            case 't':
              appendFragmentToCurrentString ("\t");
              break;
            default:
              fLexicalWaeHandler->illegalEscapedCharacterInString (fCurrentCharacter);
              oneCharacterBackwards ();
          } //

          positionDebutDuTroncon = fCurrentPositionInLine + 1;
        }
        break;

      case '\n':
        handleAnEndOfLine ();
        break;

      default:
        ; // nothing
      } //  switch

    if (finChaineAtteinte)
      break;

    nextCharacterPlease ();
    }  //    while

  // on appond le dernier tronon a fCurrentString
/*
  appendFragmentToCurrentString (
    fProducteurDeCaracteres ->
      ExtraireLaChaine (
        positionDebutDuTroncon,
        fCurrentPositionInLine - positionDebutDuTroncon ));
*/
  return kTokenSingleQuotedString;
}

// --------------------------------------------------------------------------
//  msdlLexicalAnalyzer::acceptADoubleQuotedString
// --------------------------------------------------------------------------
msdrTokenKind msdlLexicalAnalyzer::acceptADoubleQuotedString ()
{
  bool finChaineAtteinte = false;

  nextCharacterPlease (); // consume le '"'

  fTokenStartPosition = fCurrentPositionInLine;

  int positionDebutDuTroncon =
    fTokenStartPosition;

  // forget previous current string
  fCurrentString = "";

  while (true) // infinite loop
    {
    switch (fCurrentCharacter) {
      case '"':
        finChaineAtteinte = true;
        break;

      case '\\':
        {
          //append current chunk to fCurrentString
/*
          appendFragmentToCurrentString (
            fProducteurDeCaracteres ->
              ExtraireLaChaine (
                positionDebutDuTroncon,
                fCurrentPositionInLine - positionDebutDuTroncon ));
*/
          nextCharacterPlease ();

          switch (fCurrentCharacter) {
            case '"':
              appendFragmentToCurrentString ("\"");
              break;
            case '\\':
              appendFragmentToCurrentString ("\\");
              break;
            case 'n':
              appendFragmentToCurrentString ("\n");
              break;
            case 't':
              appendFragmentToCurrentString ("\t");
              break;
            default:
              stringstream s;

              fLexicalWaeHandler->illegalEscapedCharacterInString (fCurrentCharacter);
              oneCharacterBackwards ();
          } //

          positionDebutDuTroncon = fCurrentPositionInLine + 1;
        }
        break;

      case '\n':
        handleAnEndOfLine ();
        break;

      default:
        ; // nothing
      } //  switch

    if (finChaineAtteinte)
      break;

    nextCharacterPlease ();
    }  //    while

  // on appond le dernier tronon a fCurrentString
/*
  appendFragmentToCurrentString (
    fProducteurDeCaracteres ->
      ExtraireLaChaine (
        positionDebutDuTroncon,
        fCurrentPositionInLine - positionDebutDuTroncon ));
*/
  return kTokenDoubleQuotedString;
}


void msdlLexicalAnalyzer::handleMsdrToken (
  int           inputLineNumber,
  string        tokenText,
  msdrTokenKind tokenKind)
{
#ifdef TRACING_IS_ENABLED
  // write a trace of the token?
  if (fTraceTokens) {
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
      // language-independent tokens
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
        // language-dependent keywords
        // ------------------------------------

        // JMI ???; // don't care, those are flex internals
        cout << " (keyword)";
    } // switch

    if (doWriteTokenText) {
      cout << left <<
        "\"" << tokenText << "\"";
    }
  }
#endif

  // append the MSDL token to the tokens list if needed
  if (fAppendTokensToList) {
    fTokensList.appendTokenToTokensList (fCurrentToken);
  }
}

void msdlLexicalAnalyzer::performLexicalAnalysis ()
{
  try {
    while (true) { // loop till end of file
      nextTokenPlease ();
    } // while
  }
  catch (msgMsdlToMsdrScoreException& e) {
    displayException (e, gOutputStream);
//    return kInvalidOption;
  }
  catch (exception& e) {
    displayException (e, gOutputStream);
//    return kInvalidFile;
  }

  if (fSourceIsLexicallyCorrect) {
    cout << "MSDL specification is lexically correct" << endl;
  }
  else {
    cout << "### MSDL specification is lexically incorrect ###" << endl;
  }
}


}

/*
        case '\': // '//' up to this point, comment to end of line JMI
          do {
            nextCharacterPlease ();
          } // do
          while (fCurrentCharacter != '\n');

          oneCharacterBackwards ();

#ifdef TRACING_IS_ENABLED
          if (fTraceTokens) {
            EchoDesCaracteresAcceptes ();
          }
#endif

          nextCharacterPlease ();

          fTokenStartPosition = fCurrentPositionInLine;

          fCurrentTokenKind = kTokenCommentToEndOfLine;
          break;

*/
