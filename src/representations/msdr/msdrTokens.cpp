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
#include <climits>      // INT_MIN, INT_MAX

#include <regex>

#include "waeMessagesHandling.h"

#include "msr2summaryVisitor.h"
*/

#include "waeExceptions.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

// #include "msdrOah.h"
#include "msrOah.h"
#include "msr2lpsrOah.h"

#include "msdrKeywords.h"
#include "msdrTokens.h"


using namespace std;

namespace MusicXML2
{

string msdrTokenKindAsString (
  msdrTokenKind tokenKind)
{
  stringstream s;

  switch (tokenKind) {
    case k_NoToken:
      s << "*noToken*";
      break;

    // language-independent tokens
    // ------------------------------------

    case kTokenBlanks:
      s << "tokenBlanks";
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

    case kTokenSingleQuotedString:
      s << "tokenSingleQuotedString";
      break;
    case kTokenDoubleQuotedString:
      s << "tokenDoubleQuotedString";
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
    case kTokenBaritone:
      s << "tokenBaritone";
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

msdrKeywordKind msdrTokenDescription::getKeywordKind () const
{
  msdrKeywordKind result = k_NoMsdrKeywordKind;

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
    "MSDL token" <<
    ", tokenDescriptionKind = " <<
    msdrTokenDescriptionKindAsString (fTokenDescriptionKind) <<
    ": ";

  switch (fTokenDescriptionKind) {
    case kTokenDescriptionKeyword:
      s << msdrKeywordKindAsString (fKeywordKind);
      break;

    case kTokenDescriptionInteger:
      s << fInteger;
      break;

    case kTokenDescriptionDouble:
      s << fDouble;
      break;

    case kTokenDescriptionString:
      s << "\"" << fString << "\"";
      break;
  } // switch

  return s.str ();
}

void msdrTokenDescription::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const msdrTokenDescription& elt)
{
  elt.print (os);
  return os;
}

//_______________________________________________________________________________
msdrToken::msdrToken (
  msdrTokenKind tokenKind)
    : fTokenKind (tokenKind)
{}

msdrToken::msdrToken (
  msdrTokenKind   tokenKind,
  msdrKeywordKind value)
    : fTokenKind (tokenKind)
{
  fTokenDescription.setKeywordKind (value);
}

msdrToken::msdrToken (
  msdrTokenKind tokenKind,
  int           value)
    : fTokenKind (tokenKind)
{
  fTokenDescription.setInteger (value);
}

msdrToken::msdrToken (
  msdrTokenKind tokenKind,
  string        value)
    : fTokenKind (tokenKind)
{
  fTokenDescription.setString (value);
}

msdrToken::msdrToken (
  msdrTokenKind tokenKind,
  double        value)
    : fTokenKind (tokenKind)
{
  fTokenDescription.setDouble (value);
}

msdrToken::~msdrToken ()
{}

string msdrToken::asString () const
{
  stringstream s;

  s <<
    "MSDL token" <<
    ", tokenKind: " <<
    msdrTokenKindAsString (fTokenKind) <<
    ", fTokenDescription: " <<
    fTokenDescription.asString ();

  return s.str ();
}

//_______________________________________________________________________________
string msdrToken::asMsdlString () const
{
  string result;

  stringstream s;

  switch (fTokenKind) {
    case k_NoToken:
      // should not occur
      break;

    // language-independent tokens
    // ------------------------------------

    case kTokenBlanks:
    case kTokenEndOfLine:
      s << fTokenDescription.getString ();
      break;

    case kTokenParenthesizedComment:
      s << "{%" << fTokenDescription.getString () << "%}";
      break;

    case kTokenCommentToEndOfLine:
      s << "%%" << fTokenDescription.getString ();
      break;

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

    case kTokenSingleQuotedString:
      s << "\"" << fTokenDescription.getString () << "\"";
      break;
    case kTokenDoubleQuotedString:
      s << "'" << fTokenDescription.getString () << "'";
      break;

    case kTokenIdentifier:
      s << fTokenDescription.getString ();
      break;

    // language-dependent keywords
    // ------------------------------------

    default:
      s <<
        msdrKeywordKindAsString (fTokenDescription.getKeywordKind ());
  } // switch

  return result;
}

void msdrToken::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
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
    "MsdrTokensList" <<
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
