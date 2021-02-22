/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "waeExceptions.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

// #include "msdlOah.h"
#include "msdl2msdrOah.h"

#include "msrOah.h"
#include "msr2lpsrOah.h"

#include "msdlKeywords.h"
#include "msdlTokens.h"


using namespace std;

namespace MusicXML2
{

string msdrTokenKindAsString (
  msdrTokenKind tokenKind)
{
  stringstream s;

  switch (tokenKind) {
    case k_TokenEOF:
      s << "EOFToken";
      break;

    case k_NoToken:
      s << "*noToken*";
      break;

    case k_TokenMalformed:
      s << "*malformedToken*";
      break;

    // separators, for use by MSDL whole input translation
    // ------------------------------------

    case kTokenSpace:
      s << "tokenSpace";
      break;
    case kTokenTab:
      s << "tokenTab";
      break;
    case kTokenCarriageReturn:
      s << "tokenCarriageReturn";
      break;

    case kTokenEndOfLine:
      s << "tokenEndOfLine";
      break;

    case kTokenParenthesizedComment:
      s << "tokenParenthesizedComment";
      break;

    case kTokenCommentToEndOfLine:
      s << "tokenCommentToEndOfLine";
      break;

    // language-independent tokens
    // ------------------------------------

    case kTokenEqualSign:
      s << "tokenEqualSign";
      break;

    case kTokenComma:
      s << "tokenComma";
      break;
    case kTokenColon:
      s << "tokenColon";
      break;
    case kTokenSemiColon:
      s << "tokenSemiColon";
      break;

    case kTokenPlus:
      s << "tokenPlus";
      break;
    case kTokenMinus:
      s << "tokenMinus";
      break;
    case kTokenStar:
      s << "tokenStar";
      break;
    case kTokenSlash:
      s << "tokenSlash";
      break;
    case kTokenPercent:
      s << "tokenPercent";
      break;

    case kTokenConcat:
      s << "tokenConcat";
      break;

    case kTokenQuestionMark:
      s << "tokenQuestionMark";
      break;

    case kTokenLeftParenthesis:
      s << "tokenLeftParenthesis";
      break;
    case kTokenRightParenthesis:
      s << "tokenRightParenthesis";
      break;

    case kTokenLeftBracket:
      s << "tokenLeftBracket";
      break;
    case kTokenRightBracket:
      s << "tokenRightBracket";
      break;

    case kTokenQuote:
      s << "tokenQuote";
      break;
    case kTokenDot:
      s << "tokenDot";
      break;

    case kTokenMeasure:
      s << "tokenMeasure";
      break;
    case kTokenDoubleBar:
      s << "tokenDoubleBar";
      break;
    case kTokenFinalBar:
      s << "tokenFinalBar";
      break;

    case kTokenRepeatStart:
      s << "tokenRepeatStart";
      break;
    case kTokenRepeatEnd:
      s << "tokenRepeatEnd";
      break;

    case kTokenInteger:
      s << "tokenInteger";
      break;
    case kTokenDouble:
      s << "tokenDouble";
      break;

    case kTokenString:
      s << "tokenString";
      break;

    case kTokenIdentifier:
      s << "tokenIdentifier";
      break;

    // language-dependent keywords
    // ------------------------------------

    case kTokenTitle:
      s << "tokenTitle";
      break;
    case kTokenComposer:
      s << "tokenComposer";
      break;
    case kTokenOpus:
      s << "tokenOpus";
      break;

    case kTokenPitches:
      s << "tokenPitches";
      break;

    case kTokenAnacrusis:
      s << "tokenAnacrusis";
      break;

    case kTokenBook:
      s << "tokenBook";
      break;
    case kTokenScore:
      s << "tokenScore";
      break;
    case kTokenPartGroup:
      s << "tokenPartGroup";
      break;
    case kTokenPart:
      s << "tokenPart";
      break;
    case kTokenStaff:
      s << "tokenStaff";
      break;
    case kTokenVoice:
      s << "tokenVoice";
      break;
    case kTokenFragment:
      s << "tokenFragment";
      break;

    case kTokenClef:
      s << "tokenClef";
      break;

    case kTokenKey:
      s << "tokenKey";
      break;
    case kTokenTreble:
      s << "tokenTreble";
      break;
    case kTokenSoprano:
      s << "tokenSoprano";
      break;
    case kTokenAlto:
      s << "tokenAlto";
      break;
    case kTokenTenor:
      s << "tokenTenor";
      break;
    case kTokenBaryton:
      s << "tokenBaryton";
      break;
    case kTokenBass:
      s << "tokenBass";
      break;

    case kTokenTime:
      s << "tokenTime";
      break;
  } // switch

  return s.str ();
}

string msdrTokenDescriptionKindAsString (msdrTokenDescriptionKind tokenDescriptionKind)
{
  string result;

  switch (tokenDescriptionKind) {
    case kTokenDescriptionKeyword:
      result = "tokenDescriptionKeyword";
      break;
    case kTokenDescriptionInteger:
      result = "tokenDescriptionInteger";
      break;
    case kTokenDescriptionDouble:
      result = "tokenDescriptionDouble";
      break;
    case kTokenDescriptionCharacter:
      result = "tokenDescriptionCharacter";
    case kTokenDescriptionString:
      result = "tokenDescriptionString";
      break;
  } // switch

  return result;
}

//_______________________________________________________________________________
S_msdrTokenDescription msdrTokenDescription::create ()
{
  msdrTokenDescription* o =
    new msdrTokenDescription ();
  assert (o != nullptr);
  return o;
}

msdrTokenDescription::msdrTokenDescription ()
{}

msdrTokenDescription::~msdrTokenDescription ()
{}

msdlKeywordKind msdrTokenDescription::getKeywordKind () const
{
  msdlKeywordKind result = k_NoMsdlKeywordKind;

  switch (fTokenDescriptionKind) {
    case kTokenDescriptionKeyword:
      result = fKeywordKind;
      break;

    case kTokenDescriptionInteger:
      {
        string message =
          "MSDL token desr contains an integer, not a keyword";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case kTokenDescriptionDouble:
      {
        string message =
          "MSDL token desr contains a double, not an keyword";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case kTokenDescriptionCharacter:
      {
        string message =
          "MSDL token desr contains a character, not an keyword";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case kTokenDescriptionString:
      {
        string message =
          "MSDL token desr contains a string, not an keyword";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;
  } // switch

  return result;
}

int msdrTokenDescription::getInteger () const
{
  int result = 0;

  switch (fTokenDescriptionKind) {
    case kTokenDescriptionKeyword:
      {
        string message =
          "MSDL token desr contains a keyword, not an integer";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case kTokenDescriptionInteger:
      result = fInteger;
      break;

    case kTokenDescriptionDouble:
      {
        string message =
          "MSDL token desr contains a double, not an integer";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case kTokenDescriptionCharacter:
      {
        string message =
          "MSDL token desr contains a character, not an integer";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case kTokenDescriptionString:
      {
        string message =
          "MSDL token desr contains a string, not an integer";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;
  } // switch

  return result;
}

double msdrTokenDescription::getDouble () const
{
  double result = 0.0;

  switch (fTokenDescriptionKind) {
    case kTokenDescriptionKeyword:
      {
        string message =
          "MSDL token desr contains a keyword, not a double";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case kTokenDescriptionInteger:
      {
        string message =
          "MSDL token desr contains an integer, not a double";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case kTokenDescriptionDouble:
      result = fDouble;
      break;

    case kTokenDescriptionCharacter:
      {
        string message =
          "MSDL token desr contains a character, not a double";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case kTokenDescriptionString:
      {
        string message =
          "MSDL token desr contains a string, not a double";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;
  } // switch


  return result;
}

char msdrTokenDescription::getCharacter () const
{
  char result;

  switch (fTokenDescriptionKind) {
    case kTokenDescriptionKeyword:
      {
        string message =
          "MSDL token desr contains a keyword, not a character";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case kTokenDescriptionInteger:
      {
        string message =
          "MSDL token desr contains an integer, not a character";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case kTokenDescriptionDouble:
      {
        string message =
          "MSDL token desr contains a double, not a character";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case kTokenDescriptionCharacter:
      result = fCharacter;
      break;

    case kTokenDescriptionString:
      {
        string message =
          "MSDL token desr contains a double, not a character";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
  } // switch

  return result;
}

string msdrTokenDescription::getString () const
{
  string result;

  switch (fTokenDescriptionKind) {
    case kTokenDescriptionKeyword:
      {
        string message =
          "MSDL token desr contains a keyword, not a string";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case kTokenDescriptionInteger:
      {
        string message =
          "MSDL token desr contains an integer, not a string";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case kTokenDescriptionDouble:
      {
        string message =
          "MSDL token desr contains a double, not a string";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case kTokenDescriptionCharacter:
      result = fCharacter;
      break;

    case kTokenDescriptionString:
      result = fString;
      break;
  } // switch

  return result;
}

string msdrTokenDescription::asString () const
{
  stringstream s;

  s <<
    "[MSDL tokenDescription" <<
    ", tokenDescriptionKind = " <<
    msdrTokenDescriptionKindAsString (fTokenDescriptionKind) <<
    ": ";

  switch (fTokenDescriptionKind) {
    case kTokenDescriptionKeyword:
      s << msdlKeywordKindAsString (fKeywordKind);
      break;

    case kTokenDescriptionInteger:
      s << fInteger;
      break;

    case kTokenDescriptionDouble:
      s << fDouble;
      break;

    case kTokenDescriptionCharacter:
      s << "'" << fString << "'";
      break;

    case kTokenDescriptionString:
      s << "\"" << fString << "\"";
      break;
  } // switch

  s << "]";

  return s.str ();
}

void msdrTokenDescription::print (ostream& os) const
{
  os <<
    msdrTokenDescriptionKindAsString (fTokenDescriptionKind) <<
    ": ";

  switch (fTokenDescriptionKind) {
    case kTokenDescriptionKeyword:
      os << msdlKeywordKindAsString (fKeywordKind);
      break;

    case kTokenDescriptionInteger:
      os << fInteger;
      break;

    case kTokenDescriptionDouble:
      os << fDouble;
      break;

    case kTokenDescriptionCharacter:
      os << "'" << fString << "'";
      break;

    case kTokenDescriptionString:
      os << "\"" << fString << "\"";
      break;
  } // switch

  os << "]";
}

ostream& operator<< (ostream& os, const msdrTokenDescription& elt)
{
  elt.print (os);
  return os;
}

//_______________________________________________________________________________
msdrToken::msdrToken ()
    : fTokenKind (k_NoToken)
{
  initializeToken ();
}

msdrToken::msdrToken (
  msdrTokenKind tokenKind)
    : fTokenKind (tokenKind)
{
  initializeToken ();
}

msdrToken::msdrToken (
  msdrTokenKind   tokenKind,
  msdlKeywordKind value)
    : fTokenKind (tokenKind)
{
  initializeToken ();

  fTokenDescription.setKeywordKind (value);
}

msdrToken::msdrToken (
  msdrTokenKind tokenKind,
  int           value)
    : fTokenKind (tokenKind)
{
  initializeToken ();

  fTokenDescription.setInteger (value);
}

msdrToken::msdrToken (
  msdrTokenKind tokenKind,
  string        value)
    : fTokenKind (tokenKind)
{
  initializeToken ();

  fTokenDescription.setString (value);
}

msdrToken::msdrToken (
  msdrTokenKind tokenKind,
  double        value)
    : fTokenKind (tokenKind)
{
  initializeToken ();

  fTokenDescription.setDouble (value);
}

msdrToken::~msdrToken ()
{}

void msdrToken::initializeToken ()
{
  fTokenLineNumber     = 0;
  fTokenPositionInLine = 0;
}

string msdrToken::asString () const
{
  stringstream s;

  s <<
    msdrTokenKindAsString (fTokenKind) <<
    ", fTokenLineNumber: " << fTokenLineNumber <<
    ", fTokenPositionInLine: " << fTokenPositionInLine <<
    endl;

  switch (fTokenKind) {
    case k_TokenEOF:
      s << "END_OF_FILE";
      break;

    case k_NoToken:
      // should not occur
      break;

    case k_TokenMalformed:
      s << "\"" << fTokenDescription.getString () << "\"";
      break;

    // separators, for use by MSDL whole input translation
    // ------------------------------------

    case kTokenSpace:
      s << "SPACE";
      break;
    case kTokenTab:
      s << "TAB";
      break;
    case kTokenCarriageReturn:
      s << "CARRIAGE_RETURN";
      break;

    case kTokenEndOfLine:
      s << "END_OF_LINE";
      break;

    case kTokenParenthesizedComment:
      s << "\"" << fTokenDescription.getString () << "\"";
      break;

    case kTokenCommentToEndOfLine:
      s << "\"" << fTokenDescription.getString () << "\"";
      break;

    // language-independent tokens
    // ------------------------------------

    case kTokenEqualSign:
      s << "=";
      break;

    case kTokenComma:
      s << ",";
      break;
    case kTokenColon:
      s << ":";
      break;
    case kTokenSemiColon:
      s << ";";
      break;

    case kTokenPlus:
      s << "+";
      break;
    case kTokenMinus:
      s << "-";
      break;
    case kTokenStar:
      s << "*";
      break;
    case kTokenSlash:
      s << "/";
      break;
    case kTokenPercent:
      s << "%";
      break;

    case kTokenConcat:
      s << "!!";
      break;

    case kTokenQuestionMark:
      s << "?";
      break;

    case kTokenLeftParenthesis:
      s << "(";
      break;
    case kTokenRightParenthesis:
      s << ")";
      break;

    case kTokenLeftBracket:
      s << "{";
      break;
    case kTokenRightBracket:
      s << "}";
      break;

    case kTokenQuote:
      s << "'";
      break;
    case kTokenDot:
      s << ".";
      break;

    case kTokenMeasure:
      s << "|";
      break;
    case kTokenDoubleBar:
      s << "||";
      break;
    case kTokenFinalBar:
      s << "|||";
      break;

    case kTokenRepeatStart:
      s << "|||:";
      break;
    case kTokenRepeatEnd:
      s << ":|||";
      break;

    case kTokenInteger:
      s << fTokenDescription.getInteger ();
      break;
    case kTokenDouble:
      s << fTokenDescription.getDouble ();
      break;

    case kTokenString:
      s << "\"" << fTokenDescription.getString () << "\"";
      break;

    case kTokenIdentifier:
      s << "\"" << fTokenDescription.getString () << "\"";
      break;

    // language-dependent keywords
    // ------------------------------------

    default:
      s <<
        msdlKeywordKindAsString (fTokenDescription.getKeywordKind ());
  } // switch

  return s.str ();
}

string msdrToken::asMsdlString (
  msdlKeywordsLanguageKind languageKind,
  msdlCommentsTypeKind     commentsTypeKind) const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsdl2msdrOahGroup->getTraceTokens ()) {
    gLogStream <<
      "==> asMsdlString()" <<
      ", tokenKind" << " : " << msdrTokenKindAsString (fTokenKind) <<
      ", languageKind: " << msdlKeywordsLanguageKindAsString (languageKind) <<
      endl;
  }
#endif

  stringstream s;

  switch (fTokenKind) {
    case k_TokenEOF:
      // nothing to do
      break;

    case k_NoToken:
      // should not occur
      break;

    case k_TokenMalformed:
      s << "*TokenMalformed*";
      break;

    // separators, for use by MSDL whole input translation
    // ------------------------------------

    case kTokenSpace:
      s << " ";
      break;
    case kTokenTab:
      s << "TokenTab";
      break;
    case kTokenCarriageReturn:
      s << "TokenCarriageReturn";
      break;

    case kTokenEndOfLine:
      s << endl;
      break;

    case kTokenParenthesizedComment:
      switch (commentsTypeKind) {
        case kCommentsTypePercent:
          s << "%{" << fTokenDescription.getString () << "%}";
          break;
        case kCommentsTypeStar:
          s << "/*" << fTokenDescription.getString () << "*/";
          break;
      } // switch
      break;

    case kTokenCommentToEndOfLine:
      switch (commentsTypeKind) {
        case kCommentsTypePercent:
          s << "%" << fTokenDescription.getString ();
          break;
        case kCommentsTypeStar:
          s << "//" << fTokenDescription.getString ();
          break;
      } // switch
      break;

    // language-independent tokens
    // ------------------------------------

    case kTokenEqualSign:
      s << "=";
      break;

    case kTokenComma:
      s << ",";
      break;
    case kTokenColon:
      s << ":";
      break;
    case kTokenSemiColon:
      s << ";";
      break;

    case kTokenPlus:
      s << "+";
      break;
    case kTokenMinus:
      s << "-";
      break;
    case kTokenStar:
      s << "*";
      break;
    case kTokenSlash:
      s << "/";
      break;
    case kTokenPercent:
      s << "%";
      break;

    case kTokenConcat:
      s << "!!";
      break;

    case kTokenQuestionMark:
      s << "?";
      break;

    case kTokenLeftParenthesis:
      s << "(";
      break;
    case kTokenRightParenthesis:
      s << ")";
      break;

    case kTokenLeftBracket:
      s << "{";
      break;
    case kTokenRightBracket:
      s << "}";
      break;

    case kTokenQuote:
      s << "'";
      break;
    case kTokenDot:
      s << ".";
      break;

    case kTokenMeasure:
      s << "|";
      break;
    case kTokenDoubleBar:
      s << "||";
      break;
    case kTokenFinalBar:
      s << "|||";
      break;

    case kTokenRepeatStart:
      s << "|||:";
      break;
    case kTokenRepeatEnd:
      s << ":|||";
      break;

    case kTokenInteger:
      s << fTokenDescription.getInteger ();
      break;
    case kTokenDouble:
      s << fTokenDescription.getDouble ();
      break;

    case kTokenString:
      s << "\"" << fTokenDescription.getString () << "\"";
      break;

    case kTokenIdentifier:
      s << fTokenDescription.getString ();
      break;

    // language-dependent keywords
    // ------------------------------------

    default:
      s <<
        msdlKeywordKindAsMsdlString (
          languageKind,
          fTokenDescription.getKeywordKind ());
  } // switch

  return s.str ();
}

void msdrToken::print (ostream& os) const
{
  os <<
    msdrTokenKindAsString (fTokenKind) <<
    ", fTokenLineNumber: " << fTokenLineNumber <<
    ", fTokenPositionInLine: " << fTokenPositionInLine <<
    endl;

  switch (fTokenKind) {
    case k_TokenEOF:
      os << "END_OF_FILE";
      break;

    case k_NoToken:
      // should not occur
      break;

    case k_TokenMalformed:
      os << "\"" << fTokenDescription.getString () << "\"";
      break;

    // separators, for use by MSDL whole input translation
    // ------------------------------------

    case kTokenSpace:
      os << "SPACE";
      break;
    case kTokenTab:
      os << "TAB";
      break;
    case kTokenCarriageReturn:
      os << "CARRIAGE_RETURN";
      break;

    case kTokenEndOfLine:
      os << "END_OF_LINE";
      break;

    case kTokenParenthesizedComment:
      os << "\"" << fTokenDescription.getString () << "\"";
      break;

    case kTokenCommentToEndOfLine:
      os << "\"" << fTokenDescription.getString () << "\"";
      break;

    // language-independent tokens
    // ------------------------------------

    case kTokenEqualSign:
      os << "=";
      break;

    case kTokenComma:
      os << ",";
      break;
    case kTokenColon:
      os << ":";
      break;
    case kTokenSemiColon:
      os << ";";
      break;

    case kTokenPlus:
      os << "+";
      break;
    case kTokenMinus:
      os << "-";
      break;
    case kTokenStar:
      os << "*";
      break;
    case kTokenSlash:
      os << "/";
      break;
    case kTokenPercent:
      os << "%";
      break;

    case kTokenConcat:
      os << "!!";
      break;

    case kTokenQuestionMark:
      os << "?";
      break;

    case kTokenLeftParenthesis:
      os << "(";
      break;
    case kTokenRightParenthesis:
      os << ")";
      break;

    case kTokenLeftBracket:
      os << "{";
      break;
    case kTokenRightBracket:
      os << "}";
      break;

    case kTokenQuote:
      os << "'";
      break;
    case kTokenDot:
      os << ".";
      break;

    case kTokenMeasure:
      os << "|";
      break;
    case kTokenDoubleBar:
      os << "||";
      break;
    case kTokenFinalBar:
      os << "|||";
      break;

    case kTokenRepeatStart:
      os << "|||:";
      break;
    case kTokenRepeatEnd:
      os << ":|||";
      break;

    case kTokenInteger:
      os << fTokenDescription.getInteger ();
      break;
    case kTokenDouble:
      os << fTokenDescription.getDouble ();
      break;

    case kTokenString:
      os << "\"" << fTokenDescription.getString () << "\"";
      break;

    case kTokenIdentifier:
      os << "\"" << fTokenDescription.getString () << "\"";
      break;

    // language-dependent keywords
    // ------------------------------------

    default:
      os <<
        msdlKeywordKindAsString (fTokenDescription.getKeywordKind ());
  } // switch

  os << endl;
}

ostream& operator<< (ostream& os, const msdrToken& elt)
{
  elt.print (os);
  return os;
}

//_______________________________________________________________________________
S_msdrTokensList msdrTokensList::create ()
{
  msdrTokensList* o =
    new msdrTokensList ();
  assert (o != nullptr);
  return o;
}

msdrTokensList::msdrTokensList ()
{}

msdrTokensList::~msdrTokensList ()
{}

void msdrTokensList::appendTokenToTokensList (
  const msdrToken& token)
{
  fTokensList.push_back (token);
}

string msdrTokensList::asString () const
{
  stringstream s;

  s <<
    "MSDL tokens list" <<
    singularOrPluralWithoutNumber (
      fTokensList.size (), "element", "elements");

  return s.str ();
}

void msdrTokensList::print (ostream& os) const
{
  os <<
    "MsdlTokensList" <<
    endl;

  ++gIndenter;

  unsigned
    tokensListSize =
      fTokensList.size ();

  if (tokensListSize) {
    ++gIndenter;

    list<msdrToken>::const_iterator
      iBegin = fTokensList.begin (),
      iEnd   = fTokensList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << ((*i));
      if (++i == iEnd) break;
 // JMI      os << endl;
    } // for

    --gIndenter;
  }
  else {
    os << "empty" << endl;
  }

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msdrTokensList& elt)
{
  elt->print (os);
  return os;
}


} // namespace
