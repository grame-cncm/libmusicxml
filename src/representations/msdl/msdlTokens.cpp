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
#include "msdl2msrOah.h"

#include "msrOah.h"
#include "msr2lpsrOah.h"

#include "msdlKeywords.h"
#include "msdlTokens.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
string msdlTokenKindAsString (
  msdlTokenKind tokenKind)
{
  stringstream s;

  switch (tokenKind) {
    case msdlTokenKind::k_NoToken:
      s << "*noToken*";
      break;

    case msdlTokenKind::k_TokenEOF:
      s << "tokenEOF";
      break;

    case msdlTokenKind::k_TokenMalformed:
      s << "*malformedToken*";
      break;

    // separators, for use by MSDL whole input translation
    // ------------------------------------

    case msdlTokenKind::kTokenSpace:
      s << "tokenSpace";
      break;
    case msdlTokenKind::kTokenTab:
      s << "tokenTab";
      break;
    case msdlTokenKind::kTokenCarriageReturn:
      s << "tokenCarriageReturn";
      break;

    case msdlTokenKind::kTokenEndOfLine:
      s << "tokenEndOfLine";
      break;

    case msdlTokenKind::kTokenParenthesizedComment:
      s << "tokenParenthesizedComment";
      break;

    case msdlTokenKind::kTokenCommentToEndOfLine:
      s << "tokenCommentToEndOfLine";
      break;

    // language-independent tokens
    // ------------------------------------

    case msdlTokenKind::kTokenEqualSign:
      s << "tokenEqualSign";
      break;

    case msdlTokenKind::kTokenComma:
      s << "tokenComma";
      break;
    case msdlTokenKind::kTokenColon:
      s << "tokenColon";
      break;
    case msdlTokenKind::kTokenSemiColon:
      s << "tokenSemiColon";
      break;

    case msdlTokenKind::kTokenPlus:
      s << "tokenPlus";
      break;
    case msdlTokenKind::kTokenMinus:
      s << "tokenMinus";
      break;
    case msdlTokenKind::kTokenStar:
      s << "tokenStar";
      break;
    case msdlTokenKind::kTokenSlash:
      s << "tokenSlash";
      break;
    case msdlTokenKind::kTokenPercent:
      s << "tokenPercent";
      break;

    case msdlTokenKind::kTokenConcat:
      s << "tokenConcat";
      break;

    case msdlTokenKind::kTokenQuestionMark:
      s << "tokenQuestionMark";
      break;

    case msdlTokenKind::kTokenTilda:
      s << "tokenTilda";
      break;
    case msdlTokenKind::kTokenLeftAngle:
      s << "tokenLeftAngle";
      break;
    case msdlTokenKind::kTokenRightAngle:
      s << "tokenRightAngle";
      break;

    case msdlTokenKind::kTokenLeftParenthesis:
      s << "tokenLeftParenthesis";
      break;
    case msdlTokenKind::kTokenRightParenthesis:
      s << "tokenRightParenthesis";
      break;

    case msdlTokenKind::kTokenLeftBracket:
      s << "tokenLeftBracket";
      break;
    case msdlTokenKind::kTokenRightBracket:
      s << "tokenRightBracket";
      break;

    case msdlTokenKind::kTokenQuote:
      s << "tokenQuote";
      break;
    case msdlTokenKind::kTokenDot:
      s << "tokenDot";
      break;

    case msdlTokenKind::kTokenMeasure:
      s << "tokenMeasure";
      break;
    case msdlTokenKind::kTokenDoubleBar:
      s << "tokenDoubleBar";
      break;
    case msdlTokenKind::kTokenFinalBar:
      s << "tokenFinalBar";
      break;

    case msdlTokenKind::kTokenRepeatStart:
      s << "tokenRepeatStart";
      break;
    case msdlTokenKind::kTokenRepeatEnd:
      s << "tokenRepeatEnd";
      break;

    case msdlTokenKind::kTokenInteger:
      s << "tokenInteger";
      break;
//    case msdlTokenKind::kTokenDouble:
//      s << "tokenDouble";
//      break;

    case msdlTokenKind::kTokenString:
      s << "tokenString";
      break;

    case msdlTokenKind::kTokenName:
      s << "tokenName";
      break;
    case msdlTokenKind::kTokenIdentifier:
      s << "tokeIdentifier";
      break;

    // language-dependent keywords
    // ------------------------------------

    case msdlTokenKind::kTokenTitle:
      s << "tokenTitle";
      break;
    case msdlTokenKind::kTokenComposer:
      s << "tokenComposer";
      break;
    case msdlTokenKind::kTokenOpus:
      s << "tokenOpus";
      break;

    case msdlTokenKind::kTokenPitches:
      s << "tokenPitches";
      break;

    case msdlTokenKind::kTokenAnacrusis:
      s << "tokenAnacrusis";
      break;

    case msdlTokenKind::kTokenBook:
      s << "tokenBook";
      break;
    case msdlTokenKind::kTokenScore:
      s << "tokenScore";
      break;
    case msdlTokenKind::kTokenPartGroup:
      s << "tokenPartGroup";
      break;
    case msdlTokenKind::kTokenPart:
      s << "tokenPart";
      break;
    case msdlTokenKind::kTokenMusic:
      s << "tokenMusic";
      break;
    case msdlTokenKind::kTokenFragment:
      s << "tokenFragment";
      break;

    case msdlTokenKind::kTokenClef:
      s << "tokenClef";
      break;

    case msdlTokenKind::kTokenKey:
      s << "tokenKey";
      break;
    case msdlTokenKind::kTokenTreble:
      s << "tokenTreble";
      break;
    case msdlTokenKind::kTokenSoprano:
      s << "tokenSoprano";
      break;
    case msdlTokenKind::kTokenAlto:
      s << "tokenAlto";
      break;
    case msdlTokenKind::kTokenTenor:
      s << "tokenTenor";
      break;
    case msdlTokenKind::kTokenBaryton:
      s << "tokenBaryton";
      break;
    case msdlTokenKind::kTokenBass:
      s << "tokenBass";
      break;

    case msdlTokenKind::kTokenTime:
      s << "tokenTime";
      break;
  } // switch

  return s.str ();
}

string msdlTokenDescriptionKindAsString (
  msdlTokenDescriptionKind tokenDescriptionKind)
{
  string result;

  switch (tokenDescriptionKind) {
    case msdlTokenDescriptionKind::kTokenKeyword:
      result = "tokenDescriptionKeyword";
      break;
    case msdlTokenDescriptionKind::kTokenInteger:
      result = "tokenDescriptionInteger";
      break;
    case msdlTokenDescriptionKind::kTokenDouble:
      result = "tokenDescriptionDouble";
      break;
    case msdlTokenDescriptionKind::kTokenCharacter:
      result = "tokenDescriptionCharacter";
    case msdlTokenDescriptionKind::kTokenString:
      result = "tokenDescriptionString";
      break;
  } // switch

  return result;
}

string msdlTokenKindAsMsdlString (
  msdlTokenKind            tokenKind,
  msdlKeywordsLanguageKind languageKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsdl2msrOahGroup->getTraceTokens ()) {
    gLogStream <<
      "==> msdlTokenKindAsMsdlString()" <<
      ", tokenKind" << " : " << msdlTokenKindAsString (tokenKind) <<
      ", languageKind: " << msdlKeywordsLanguageKindAsString (languageKind) <<
      endl;
  }
#endif

  string result;

  switch (tokenKind) {
    case msdlTokenKind::k_NoToken:
      result = "*NoToken*";
      break;

    case msdlTokenKind::k_TokenEOF:
      result = "EOF";
      break;

    case msdlTokenKind::k_TokenMalformed:
      result = "*TokenMalformed*";
      break;

    // separators, for use by MSDL whole input translation
    // ------------------------------------

    case msdlTokenKind::kTokenSpace:
      result = "SPACE";
      break;
    case msdlTokenKind::kTokenTab:
      result = "TAB";
      break;
    case msdlTokenKind::kTokenCarriageReturn:
      result = "CarriageReturn";
      break;

    case msdlTokenKind::kTokenEndOfLine:
      result = "EndOfLine";
      break;

    case msdlTokenKind::kTokenParenthesizedComment:
      result =
        "aParenthesizedPercentComment";
      result += "aParenthesizedStartComment";
      break;

    case msdlTokenKind::kTokenCommentToEndOfLine:
      result =
        "aPercentCommentToEndOfLine";
      result += "aStartCommentToEndOfLine";
      break;

    // language-independent tokens
    // ------------------------------------

    case msdlTokenKind::kTokenEqualSign:
      result = "=";
      break;

    case msdlTokenKind::kTokenComma:
      result = ",";
      break;
    case msdlTokenKind::kTokenColon:
      result = ":";
      break;
    case msdlTokenKind::kTokenSemiColon:
      result = ";";
      break;

    case msdlTokenKind::kTokenPlus:
      result = "+";
      break;
    case msdlTokenKind::kTokenMinus:
      result = "-";
      break;
    case msdlTokenKind::kTokenStar:
      result = "*";
      break;
    case msdlTokenKind::kTokenSlash:
      result = "/";
      break;
    case msdlTokenKind::kTokenPercent:
      result = "%";
      break;

    case msdlTokenKind::kTokenConcat:
      result = "!!";
      break;

    case msdlTokenKind::kTokenQuestionMark:
      result = "?";
      break;

    case msdlTokenKind::kTokenTilda:
      result = "~";
      break;

    case msdlTokenKind::kTokenLeftAngle:
      result = "<";
      break;
    case msdlTokenKind::kTokenRightAngle:
      result = ">";
      break;

    case msdlTokenKind::kTokenLeftParenthesis:
      result = "(";
      break;
    case msdlTokenKind::kTokenRightParenthesis:
      result = ")";
      break;

    case msdlTokenKind::kTokenLeftBracket:
      result = "{";
      break;
    case msdlTokenKind::kTokenRightBracket:
      result = "}";
      break;

    case msdlTokenKind::kTokenQuote:
      result = "'";
      break;
    case msdlTokenKind::kTokenDot:
      result = ".";
      break;

    case msdlTokenKind::kTokenMeasure:
      result = "|";
      break;
    case msdlTokenKind::kTokenDoubleBar:
      result = "||";
      break;
    case msdlTokenKind::kTokenFinalBar:
      result = "|||";
      break;

    case msdlTokenKind::kTokenRepeatStart:
      result = "|||:";
      break;
    case msdlTokenKind::kTokenRepeatEnd:
      result = ":|||";
      break;

    case msdlTokenKind::kTokenInteger:
      result = "<integer>";
      break;
//    case msdlTokenKind::kTokenDouble:
//      result = "<double>";
//      break;

    case msdlTokenKind::kTokenString:
      result = "<string>";
      break;

    case msdlTokenKind::kTokenName:
      result = "<name>";
      break;
    case msdlTokenKind::kTokenIdentifier:
      result = "<identifier>";
      break;

    // language-dependent keywords
    // ------------------------------------

    default:
      {
        msdlKeywordKind
          keywordKind =
            msdlKeywordKindFromTokenKind (
              tokenKind);

        result =
          msdlKeywordKindAsMsdlString (
            languageKind,
            keywordKind);
      }
  } // switch

  return result;
}

msdlTokenKind msdlTokenKindFromKeywordKind (
  msdlKeywordKind keywordKind)
{
  msdlTokenKind result = msdlTokenKind::k_NoToken;

  switch (keywordKind) {
    case msdlKeywordKind::k_NoKeywordKind:
      result = msdlTokenKind::k_NoToken;
      break;

    case msdlKeywordKind::kKeywordTitle:
      result = msdlTokenKind::kTokenTitle;
      break;
    case msdlKeywordKind::kKeywordComposer:
      result = msdlTokenKind::kTokenComposer;
      break;
    case msdlKeywordKind::kKeywordOpus:
      result = msdlTokenKind::kTokenOpus;
      break;

    case msdlKeywordKind::kKeywordPitches:
      result = msdlTokenKind::kTokenPitches;
      break;

    case msdlKeywordKind::kKeywordBook:
      result = msdlTokenKind::kTokenBook;
      break;
    case msdlKeywordKind::kKeywordScore:
      result = msdlTokenKind::kTokenScore;
      break;
    case msdlKeywordKind::kKeywordPartGroup:
      result = msdlTokenKind::kTokenPartGroup;
      break;
    case msdlKeywordKind::kKeywordPart:
      result = msdlTokenKind::kTokenPart;
      break;
    case msdlKeywordKind::kKeywordMusic:
      result = msdlTokenKind::kTokenMusic;
      break;
    case msdlKeywordKind::kKeywordFragment:
      result = msdlTokenKind::kTokenFragment;
      break;

    case msdlKeywordKind::kKeywordAnacrusis:
      result = msdlTokenKind::kTokenAnacrusis;
      break;

    case msdlKeywordKind::kKeywordClef:
      result = msdlTokenKind::kTokenClef;
      break;
    case msdlKeywordKind::kKeywordTreble:
      result = msdlTokenKind::kTokenTreble;
      break;
    case msdlKeywordKind::kKeywordSoprano:
      result = msdlTokenKind::kTokenSoprano;
      break;
    case msdlKeywordKind::kKeywordAlto:
      result = msdlTokenKind::kTokenAlto;
      break;
    case msdlKeywordKind::kKeywordTenor:
      result = msdlTokenKind::kTokenTenor;
      break;
    case msdlKeywordKind::kKeywordBaryton:
      result = msdlTokenKind::kTokenBaryton;
      break;
    case msdlKeywordKind::kKeywordBass:
      result = msdlTokenKind::kTokenBass;
      break;

    case msdlKeywordKind::kKeywordKey:
      result = msdlTokenKind::kTokenKey;
      break;

    case msdlKeywordKind::kKeywordTime:
      result = msdlTokenKind::kTokenTime;
      break;
  } // switch

  return result;
}

string existingTokensInLanguage (
  msdlKeywordsLanguageKind keywordsLanguageKind,
  unsigned int             namesListMaxLength)
{
  string result;

  map<msdlKeywordKind, string> *keywordsNamesMapPTR = nullptr;

  // select the relevant keywords names map
  switch (keywordsLanguageKind) {
    case msdlKeywordsLanguageKind::k_NoKeywordsLanguage:
      // should not occur
      break;

    case msdlKeywordsLanguageKind::kKeywordsEnglish:
      keywordsNamesMapPTR = &gGlobalEnglishKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsFrench:
      keywordsNamesMapPTR = &gGlobalFrenchKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsItalian:
      keywordsNamesMapPTR = &gGlobalItalianKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsGerman:
      keywordsNamesMapPTR = &gGlobalGermanKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsSpanish:
      keywordsNamesMapPTR = &gGlobalSpanishKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsNederlands:
      keywordsNamesMapPTR = &gGlobalDutchKeywordsNamesMap;
      break;
  } // switch

  if (keywordsNamesMapPTR) {
    unsigned int
      nextToLast =
        (int) msdlTokenKind::NonSeparatorsLast - 2;

    unsigned int count = 0;
    unsigned int cumulatedLength = 0;

    stringstream s;

    for (auto e: EnumNonSeparators<msdlTokenKind> ()) {
      string
        nonSeparatorTokenAsMsdlString  =
          msdlTokenKindAsMsdlString (
            e,
            keywordsLanguageKind);

      ++count;

      cumulatedLength += nonSeparatorTokenAsMsdlString.size ();
      if (
        cumulatedLength >= namesListMaxLength
          ||
        e == msdlTokenKind::TranslatableFirst
          ||
        e == msdlTokenKind::LanguageDependentFirst
      ) {
        s << endl;
        cumulatedLength = 0;
      }

      s << nonSeparatorTokenAsMsdlString;

      if (count <= nextToLast) {
        s << " ";
      }
/*
      if ((int) i < (int) msdlTokenKind::LastIterable - 2) { JMI KEEP
        s << ", ";
      }
      else {
        s << " and ";
      }
*/
    } // for

    result = s.str ();
  }

  return result;
}

//________________________________________________________________________
S_msdlTokenKindsSet msdlTokenKindsSet::create ()
{
  // create the set
  msdlTokenKindsSet* o = new
    msdlTokenKindsSet ();
  assert (o != nullptr);
  return o;
}

S_msdlTokenKindsSet msdlTokenKindsSet::create (
  initializer_list<msdlTokenKind> elements)
{
  // create the set
  msdlTokenKindsSet* o = new
    msdlTokenKindsSet (
      elements);
  assert (o != nullptr);
  return o;
}

S_msdlTokenKindsSet msdlTokenKindsSet::create (
  msdlTokenKindsSet &tokenKindsSet)
{
  // create the set
  msdlTokenKindsSet* o = new
    msdlTokenKindsSet (
      tokenKindsSet);
  assert (o != nullptr);
  return o;
}

S_msdlTokenKindsSet msdlTokenKindsSet::createClone ()
{
  S_msdlTokenKindsSet
    result =
      msdlTokenKindsSet::create (*this);

  if (fTokenKindsSet.size ()) {
    for (
      set<msdlTokenKind>::const_iterator i =
        fTokenKindsSet.begin ();
      i != fTokenKindsSet.end ();
      ++i
    ) {
      result->fTokenKindsSet.insert ((*i));
    } // for
  }

#ifdef TRACING_IS_ENABLED
  if (false && gGlobalMsdl2msrOahGroup->getTraceTokens ()) {
    gLogStream <<
      "<== createClone()" <<
      ", result:" << result->asString () <<
      endl;
  }
#endif

  return result;
}

msdlTokenKindsSet::msdlTokenKindsSet ()
{}

msdlTokenKindsSet::msdlTokenKindsSet (
  initializer_list<msdlTokenKind> elements)
{
  // populate the set
  initializer_list<msdlTokenKind>::const_iterator it;

  for (it = elements.begin (); it != elements.end (); ++it) {
    fTokenKindsSet.insert ((*it));
  } // for
}

msdlTokenKindsSet::msdlTokenKindsSet (
  msdlTokenKindsSet& tokenKindsSet)
  : fTokenKindsSet (
    tokenKindsSet.fTokenKindsSet)
{}

msdlTokenKindsSet::~msdlTokenKindsSet ()
{}

void msdlTokenKindsSet::addElementsFrom (S_msdlTokenKindsSet tokenKindsSet)
{
  if (tokenKindsSet->fTokenKindsSet.size ()) {
    for (
      set<msdlTokenKind>::const_iterator i =
        tokenKindsSet->fTokenKindsSet.begin ();
      i != tokenKindsSet->fTokenKindsSet.end ();
      ++i
    ) {
      fTokenKindsSet.insert ((*i));
    } // for
  }
}

void msdlTokenKindsSet::removeElementsFrom (S_msdlTokenKindsSet tokenKindsSet)
{
  if (tokenKindsSet->fTokenKindsSet.size ()) {
    for (
      set<msdlTokenKind>::const_iterator i =
        tokenKindsSet->fTokenKindsSet.begin ();
      i != tokenKindsSet->fTokenKindsSet.end ();
      ++i
    ) {
      fTokenKindsSet.erase ((*i));
    } // for
  }
}

string msdlTokenKindsSet::asString () const
{
  stringstream s;

  s << "[";

  if (fTokenKindsSet.size ()) {
    set<msdlTokenKind>::const_iterator
      iBegin = fTokenKindsSet.begin (),
      iEnd   = fTokenKindsSet.end (),
      i      = iBegin;

    for ( ; ; ) {
      s <<
        msdlTokenKindAsString ((*i));
        /*
        .asMsdlString (
          fUserLanguageKind,
          kCommentsTypePercent);
*/

      if (++i == iEnd) break;

      s << ", ";
    } // for
  }

  s << "]";

  return s.str ();
}

void msdlTokenKindsSet::print (ostream& os) const
{
  os <<
    "msdlTokenKindsSet:";

  if (fTokenKindsSet.size ()) {
    set<msdlTokenKind>::const_iterator
      iBegin = fTokenKindsSet.begin (),
      iEnd   = fTokenKindsSet.end (),
      i      = iBegin;

    for ( ; ; ) {
      os <<
        msdlTokenKindAsString ((*i));
        /*
        .asMsdlString (
          fUserLanguageKind,
          kCommentsTypePercent);
*/

      if (++i == iEnd) break;

      os << ", ";
    } // for
  }
  else {
    os << " empty" << endl;
  }
}

ostream& operator<< (ostream& os, const S_msdlTokenKindsSet& elt)
{
  elt->print (os);
  return os;
}

EXP S_msdlTokenKindsSet operator+= (
  S_msdlTokenKindsSet aTokenKindsSet,
  msdlTokenKind       aTokenKind)
{
  aTokenKindsSet->
    addTokenKind (
      aTokenKind);

  return aTokenKindsSet;
}

EXP S_msdlTokenKindsSet operator+= (
  S_msdlTokenKindsSet aTokenKindsSet,
  S_msdlTokenKindsSet anotherTokenKindsSet)
{
  aTokenKindsSet->
    addElementsFrom (
      anotherTokenKindsSet);

  return aTokenKindsSet;
}

EXP S_msdlTokenKindsSet operator-= (
  S_msdlTokenKindsSet aTokenKindsSet,
  msdlTokenKind       aTokenKind)
{
  aTokenKindsSet->
    removeTokenKind (
      aTokenKind);

  return aTokenKindsSet;
}

EXP S_msdlTokenKindsSet operator-= (
  S_msdlTokenKindsSet aTokenKindsSet,
  S_msdlTokenKindsSet anotherTokenKindsSet)
{
  aTokenKindsSet->
    removeElementsFrom (
      anotherTokenKindsSet);

  return aTokenKindsSet;
}

EXP S_msdlTokenKindsSet operator+ (
  S_msdlTokenKindsSet aTokenKindsSet,
  msdlTokenKind       aTokenKind)
{
  S_msdlTokenKindsSet
    result =
      aTokenKindsSet->createClone ();

  result->
    addTokenKind (
      aTokenKind);

#ifdef TRACING_IS_ENABLED
  if (false && gGlobalMsdl2msrOahGroup->getTraceTokens ()) {
    gLogStream <<
      "<== S_msdlTokenKindsSet operator+()" <<
      ", result:" << result->asString () <<
      endl;
  }
#endif

  return result;
}

EXP S_msdlTokenKindsSet operator+ (
  S_msdlTokenKindsSet aTokenKindsSet,
  S_msdlTokenKindsSet anotherTokenKindsSet)
{
  S_msdlTokenKindsSet
    result =
      aTokenKindsSet->createClone ();

  result += anotherTokenKindsSet;

#ifdef TRACING_IS_ENABLED
  if (false && gGlobalMsdl2msrOahGroup->getTraceTokens ()) {
    gLogStream <<
      "<== S_msdlTokenKindsSet operator+()" <<
      ", result:" << result->asString () <<
      endl;
  }
#endif

  return result;
}

EXP S_msdlTokenKindsSet operator- (
  S_msdlTokenKindsSet aTokenKindsSet,
  msdlTokenKind       aTokenKind)
{
  S_msdlTokenKindsSet
    result =
      aTokenKindsSet->createClone ();

  result->
    removeTokenKind (
      aTokenKind);

  return result;
}

EXP S_msdlTokenKindsSet operator- (
  S_msdlTokenKindsSet aTokenKindsSet,
  S_msdlTokenKindsSet anotherTokenKindsSet)
{
  S_msdlTokenKindsSet
    result =
      aTokenKindsSet->createClone ();

  result -= anotherTokenKindsSet;

  return result;
}

//_______________________________________________________________________________
S_msdlTokenDescription msdlTokenDescription::create ()
{
  msdlTokenDescription* o =
    new msdlTokenDescription ();
  assert (o != nullptr);
  return o;
}

msdlTokenDescription::msdlTokenDescription ()
{}

msdlTokenDescription::~msdlTokenDescription ()
{}

msdlKeywordKind msdlTokenDescription::getKeywordKind () const
{
  msdlKeywordKind result = msdlKeywordKind::k_NoKeywordKind;

  switch (fTokenDescriptionKind) {
    case msdlTokenDescriptionKind::kTokenKeyword:
      result = fKeywordKind;
      break;

    case msdlTokenDescriptionKind::kTokenInteger:
      {
        string message =
          "MSDL token desr contains an integer, not a keyword";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenDouble:
      {
        string message =
          "MSDL token desr contains a double, not an keyword";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenCharacter:
      {
        string message =
          "MSDL token desr contains a character, not an keyword";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenString:
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

int msdlTokenDescription::getInteger () const
{
  int result = 0;

  switch (fTokenDescriptionKind) {
    case msdlTokenDescriptionKind::kTokenKeyword:
      {
        string message =
          "MSDL token desr contains a keyword, not an integer";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenInteger:
      result = fInteger;
      break;

    case msdlTokenDescriptionKind::kTokenDouble:
      {
        string message =
          "MSDL token desr contains a double, not an integer";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenCharacter:
      {
        string message =
          "MSDL token desr contains a character, not an integer";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenString:
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

double msdlTokenDescription::getDouble () const
{
  double result = 0.0;

  switch (fTokenDescriptionKind) {
    case msdlTokenDescriptionKind::kTokenKeyword:
      {
        string message =
          "MSDL token desr contains a keyword, not a double";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenInteger:
      {
        string message =
          "MSDL token desr contains an integer, not a double";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenDouble:
      result = fDouble;
      break;

    case msdlTokenDescriptionKind::kTokenCharacter:
      {
        string message =
          "MSDL token desr contains a character, not a double";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenString:
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

char msdlTokenDescription::getCharacter () const
{
  char result = '@';

  switch (fTokenDescriptionKind) {
    case msdlTokenDescriptionKind::kTokenKeyword:
      {
        string message =
          "MSDL token desr contains a keyword, not a character";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenInteger:
      {
        string message =
          "MSDL token desr contains an integer, not a character";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenDouble:
      {
        string message =
          "MSDL token desr contains a double, not a character";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenCharacter:
      result = fCharacter;
      break;

    case msdlTokenDescriptionKind::kTokenString:
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

string msdlTokenDescription::getString () const
{
  string result;

  switch (fTokenDescriptionKind) {
    case msdlTokenDescriptionKind::kTokenKeyword:
      {
        string message =
          "MSDL token desr contains a keyword, not a string";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenInteger:
      {
        string message =
          "MSDL token desr contains an integer, not a string";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenDouble:
      {
        string message =
          "MSDL token desr contains a double, not a string";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenCharacter:
      result = fCharacter;
      break;

    case msdlTokenDescriptionKind::kTokenString:
      result = fString;
      break;
  } // switch

  return result;
}

string msdlTokenDescription::asString () const
{
  stringstream s;

  s <<
    "[MSDL tokenDescription" <<
    ", tokenDescriptionKind = " <<
    msdlTokenDescriptionKindAsString (fTokenDescriptionKind) <<
    ": ";

  switch (fTokenDescriptionKind) {
    case msdlTokenDescriptionKind::kTokenKeyword:
      s << msdlKeywordKindAsString (fKeywordKind);
      break;

    case msdlTokenDescriptionKind::kTokenInteger:
      s << fInteger;
      break;

    case msdlTokenDescriptionKind::kTokenDouble:
      s << fDouble;
      break;

    case msdlTokenDescriptionKind::kTokenCharacter:
      s << "'" << fString << "'";
      break;

    case msdlTokenDescriptionKind::kTokenString:
      s << "\"" << fString << "\"";
      break;
  } // switch

  s << "]";

  return s.str ();
}

void msdlTokenDescription::print (ostream& os) const
{
  os <<
    msdlTokenDescriptionKindAsString (fTokenDescriptionKind) <<
    ": ";

  switch (fTokenDescriptionKind) {
    case msdlTokenDescriptionKind::kTokenKeyword:
      os << msdlKeywordKindAsString (fKeywordKind);
      break;

    case msdlTokenDescriptionKind::kTokenInteger:
      os << fInteger;
      break;

    case msdlTokenDescriptionKind::kTokenDouble:
      os << fDouble;
      break;

    case msdlTokenDescriptionKind::kTokenCharacter:
      os << "'" << fString << "'";
      break;

    case msdlTokenDescriptionKind::kTokenString:
      os << "\"" << fString << "\"";
      break;
  } // switch

  os << "]";
}

ostream& operator<< (ostream& os, const msdlTokenDescription& elt)
{
  elt.print (os);
  return os;
}

//_______________________________________________________________________________
msdlToken::msdlToken ()
    : fTokenKind (msdlTokenKind::k_NoToken)
{
  initializeToken ();
}

msdlToken::msdlToken (
  msdlTokenKind tokenKind)
    : fTokenKind (tokenKind)
{
  initializeToken ();
}

msdlToken::msdlToken (
  msdlTokenKind   tokenKind,
  msdlKeywordKind value)
    : fTokenKind (tokenKind)
{
  initializeToken ();

  fTokenDescription.setKeywordKind (value);
}

msdlToken::msdlToken (
  msdlTokenKind tokenKind,
  int           value)
    : fTokenKind (tokenKind)
{
  initializeToken ();

  fTokenDescription.setInteger (value);
}

msdlToken::msdlToken (
  msdlTokenKind tokenKind,
  string        value)
    : fTokenKind (tokenKind)
{
  initializeToken ();

  fTokenDescription.setString (value);
}

msdlToken::msdlToken (
  msdlTokenKind tokenKind,
  double        value)
    : fTokenKind (tokenKind)
{
  initializeToken ();

  fTokenDescription.setDouble (value);
}

msdlToken::~msdlToken ()
{}

void msdlToken::initializeToken ()
{
  fTokenLineNumber     = 0;
  fTokenPositionInLine = 0;
}

string msdlToken::asString () const
{
  stringstream s;

  s << left <<
    "[ " <<
    "@" << fTokenLineNumber <<
    ":" << fTokenPositionInLine <<
    " " <<
    msdlTokenKindAsString (fTokenKind) <<
    " ";

  switch (fTokenKind) {
    case msdlTokenKind::k_NoToken:
      // should not occur
      break;

    case msdlTokenKind::k_TokenEOF:
      s << "END_OF_FILE";
      break;

    case msdlTokenKind::k_TokenMalformed:
      s << "\"" << fTokenDescription.getString () << "\"";
      break;

    // separators, for use by MSDL whole input translation
    // ------------------------------------

    case msdlTokenKind::kTokenSpace:
      s << "SPACE";
      break;
    case msdlTokenKind::kTokenTab:
      s << "TAB";
      break;
    case msdlTokenKind::kTokenCarriageReturn:
      s << "CARRIAGE_RETURN";
      break;

    case msdlTokenKind::kTokenEndOfLine:
      s << "END_OF_LINE";
      break;

    case msdlTokenKind::kTokenParenthesizedComment:
      s << "\"" << fTokenDescription.getString () << "\"";
      break;

    case msdlTokenKind::kTokenCommentToEndOfLine:
      s << "\"" << fTokenDescription.getString () << "\"";
      break;

    // language-independent tokens
    // ------------------------------------

    case msdlTokenKind::kTokenEqualSign:
      s << "=";
      break;

    case msdlTokenKind::kTokenComma:
      s << ",";
      break;
    case msdlTokenKind::kTokenColon:
      s << ":";
      break;
    case msdlTokenKind::kTokenSemiColon:
      s << ";";
      break;

    case msdlTokenKind::kTokenPlus:
      s << "+";
      break;
    case msdlTokenKind::kTokenMinus:
      s << "-";
      break;
    case msdlTokenKind::kTokenStar:
      s << "*";
      break;
    case msdlTokenKind::kTokenSlash:
      s << "/";
      break;
    case msdlTokenKind::kTokenPercent:
      s << "%";
      break;

    case msdlTokenKind::kTokenConcat:
      s << "!!";
      break;

    case msdlTokenKind::kTokenQuestionMark:
      s << "?";
      break;

    case msdlTokenKind::kTokenTilda:
      s << "~";
      break;

    case msdlTokenKind::kTokenLeftAngle:
      s << "<";
      break;
    case msdlTokenKind::kTokenRightAngle:
      s << ">";
      break;

    case msdlTokenKind::kTokenLeftParenthesis:
      s << "(";
      break;
    case msdlTokenKind::kTokenRightParenthesis:
      s << ")";
      break;

    case msdlTokenKind::kTokenLeftBracket:
      s << "{";
      break;
    case msdlTokenKind::kTokenRightBracket:
      s << "}";
      break;

    case msdlTokenKind::kTokenQuote:
      s << "'";
      break;
    case msdlTokenKind::kTokenDot:
      s << ".";
      break;

    case msdlTokenKind::kTokenMeasure:
      s << "|";
      break;
    case msdlTokenKind::kTokenDoubleBar:
      s << "||";
      break;
    case msdlTokenKind::kTokenFinalBar:
      s << "|||";
      break;

    case msdlTokenKind::kTokenRepeatStart:
      s << "|||:";
      break;
    case msdlTokenKind::kTokenRepeatEnd:
      s << ":|||";
      break;

    case msdlTokenKind::kTokenInteger:
      s << fTokenDescription.getInteger ();
      break;
//    case msdlTokenKind::kTokenDouble:
//      s << fTokenDescription.getDouble ();
//      break;

    case msdlTokenKind::kTokenString:
      s << "\"" << fTokenDescription.getString () << "\"";
      break;

    case msdlTokenKind::kTokenName:
      s << "\"" << fTokenDescription.getString () << "\"";
      break;
    case msdlTokenKind::kTokenIdentifier:
      s << "\"" << fTokenDescription.getString () << "\"";
      break;

    // language-dependent keywords
    // ------------------------------------

    default:
      s <<
        msdlKeywordKindAsString (fTokenDescription.getKeywordKind ());
  } // switch

  s << " ]";

  return s.str ();
}

string msdlToken::asMsdlString (
  msdlKeywordsLanguageKind languageKind,
  msdlCommentsTypeKind     commentsTypeKind) const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsdl2msrOahGroup->getTraceTokens ()) {
    gLogStream <<
      "==> asMsdlString()" <<
      ", tokenKind" << " : " << msdlTokenKindAsString (fTokenKind) <<
      ", languageKind: " << msdlKeywordsLanguageKindAsString (languageKind) <<
      endl;
  }
#endif

  stringstream s;

  switch (fTokenKind) {
    case msdlTokenKind::k_NoToken:
      // should not occur
      break;

    case msdlTokenKind::k_TokenEOF:
      s << "TokenEOF";
      break;

    case msdlTokenKind::k_TokenMalformed:
      s << "*TokenMalformed*";
      break;

    // separators, for use by MSDL whole input translation
    // ------------------------------------

    case msdlTokenKind::kTokenSpace:
      s << " ";
      break;
    case msdlTokenKind::kTokenTab:
      s << "TokenTab";
      break;
    case msdlTokenKind::kTokenCarriageReturn:
      s << "TokenCarriageReturn";
      break;

    case msdlTokenKind::kTokenEndOfLine:
      s << endl;
      break;

    case msdlTokenKind::kTokenParenthesizedComment:
      switch (commentsTypeKind) {
        case kCommentsTypePercent:
          s << "%{" << fTokenDescription.getString () << "%}";
          break;
        case kCommentsTypeStar:
          s << "/*" << fTokenDescription.getString () << "*/";
          break;
      } // switch
      break;

    case msdlTokenKind::kTokenCommentToEndOfLine:
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

    case msdlTokenKind::kTokenEqualSign:
      s << "=";
      break;

    case msdlTokenKind::kTokenComma:
      s << ",";
      break;
    case msdlTokenKind::kTokenColon:
      s << ":";
      break;
    case msdlTokenKind::kTokenSemiColon:
      s << ";";
      break;

    case msdlTokenKind::kTokenPlus:
      s << "+";
      break;
    case msdlTokenKind::kTokenMinus:
      s << "-";
      break;
    case msdlTokenKind::kTokenStar:
      s << "*";
      break;
    case msdlTokenKind::kTokenSlash:
      s << "/";
      break;
    case msdlTokenKind::kTokenPercent:
      s << "%";
      break;

    case msdlTokenKind::kTokenConcat:
      s << "!!";
      break;

    case msdlTokenKind::kTokenQuestionMark:
      s << "?";
      break;

    case msdlTokenKind::kTokenTilda:
      s << "~";
      break;

    case msdlTokenKind::kTokenLeftAngle:
      s << "<";
      break;
    case msdlTokenKind::kTokenRightAngle:
      s << ">";
      break;

    case msdlTokenKind::kTokenLeftParenthesis:
      s << "(";
      break;
    case msdlTokenKind::kTokenRightParenthesis:
      s << ")";
      break;

    case msdlTokenKind::kTokenLeftBracket:
      s << "{";
      break;
    case msdlTokenKind::kTokenRightBracket:
      s << "}";
      break;

    case msdlTokenKind::kTokenQuote:
      s << "'";
      break;
    case msdlTokenKind::kTokenDot:
      s << ".";
      break;

    case msdlTokenKind::kTokenMeasure:
      s << "|";
      break;
    case msdlTokenKind::kTokenDoubleBar:
      s << "||";
      break;
    case msdlTokenKind::kTokenFinalBar:
      s << "|||";
      break;

    case msdlTokenKind::kTokenRepeatStart:
      s << "|||:";
      break;
    case msdlTokenKind::kTokenRepeatEnd:
      s << ":|||";
      break;

    case msdlTokenKind::kTokenInteger:
      s << fTokenDescription.getInteger ();
      break;
//    case msdlTokenKind::kTokenDouble:
//      s << fTokenDescription.getDouble ();
//      break;

    case msdlTokenKind::kTokenString:
      s << "\"" << fTokenDescription.getString () << "\"";
      break;

    case msdlTokenKind::kTokenName:
      s << fTokenDescription.getString ();
      break;
    case msdlTokenKind::kTokenIdentifier:
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

void msdlToken::print (ostream& os) const
{
  os << left <<
    msdlTokenKindAsString (fTokenKind) <<
    " @" << fTokenLineNumber <<
    ":" << fTokenPositionInLine <<
    endl;

  switch (fTokenKind) {
    case msdlTokenKind::k_NoToken:
      // should not occur
      break;

    case msdlTokenKind::k_TokenEOF:
      os << "END_OF_FILE";
      break;

    case msdlTokenKind::k_TokenMalformed:
      os << "\"" << fTokenDescription.getString () << "\"";
      break;

    // separators, for use by MSDL whole input translation
    // ------------------------------------

    case msdlTokenKind::kTokenSpace:
      os << "SPACE";
      break;
    case msdlTokenKind::kTokenTab:
      os << "TAB";
      break;
    case msdlTokenKind::kTokenCarriageReturn:
      os << "CARRIAGE_RETURN";
      break;

    case msdlTokenKind::kTokenEndOfLine:
      os << "END_OF_LINE";
      break;

    case msdlTokenKind::kTokenParenthesizedComment:
      os << "\"" << fTokenDescription.getString () << "\"";
      break;

    case msdlTokenKind::kTokenCommentToEndOfLine:
      os << "\"" << fTokenDescription.getString () << "\"";
      break;

    // language-independent tokens
    // ------------------------------------

    case msdlTokenKind::kTokenEqualSign:
      os << "=";
      break;

    case msdlTokenKind::kTokenComma:
      os << ",";
      break;
    case msdlTokenKind::kTokenColon:
      os << ":";
      break;
    case msdlTokenKind::kTokenSemiColon:
      os << ";";
      break;

    case msdlTokenKind::kTokenPlus:
      os << "+";
      break;
    case msdlTokenKind::kTokenMinus:
      os << "-";
      break;
    case msdlTokenKind::kTokenStar:
      os << "*";
      break;
    case msdlTokenKind::kTokenSlash:
      os << "/";
      break;
    case msdlTokenKind::kTokenPercent:
      os << "%";
      break;

    case msdlTokenKind::kTokenConcat:
      os << "!!";
      break;

    case msdlTokenKind::kTokenQuestionMark:
      os << "?";
      break;

    case msdlTokenKind::kTokenTilda:
      os << "~";
      break;

    case msdlTokenKind::kTokenLeftAngle:
      os << "<";
      break;
    case msdlTokenKind::kTokenRightAngle:
      os << ">";
      break;

    case msdlTokenKind::kTokenLeftParenthesis:
      os << "(";
      break;
    case msdlTokenKind::kTokenRightParenthesis:
      os << ")";
      break;

    case msdlTokenKind::kTokenLeftBracket:
      os << "{";
      break;
    case msdlTokenKind::kTokenRightBracket:
      os << "}";
      break;

    case msdlTokenKind::kTokenQuote:
      os << "'";
      break;
    case msdlTokenKind::kTokenDot:
      os << ".";
      break;

    case msdlTokenKind::kTokenMeasure:
      os << "|";
      break;
    case msdlTokenKind::kTokenDoubleBar:
      os << "||";
      break;
    case msdlTokenKind::kTokenFinalBar:
      os << "|||";
      break;

    case msdlTokenKind::kTokenRepeatStart:
      os << "|||:";
      break;
    case msdlTokenKind::kTokenRepeatEnd:
      os << ":|||";
      break;

    case msdlTokenKind::kTokenInteger:
      os << fTokenDescription.getInteger ();
      break;
//    case msdlTokenKind::kTokenDouble:
//      os << fTokenDescription.getDouble ();
//      break;

    case msdlTokenKind::kTokenString:
      os << "\"" << fTokenDescription.getString () << "\"";
      break;

    case msdlTokenKind::kTokenName:
      os << "\"" << fTokenDescription.getString () << "\"";
      break;
    case msdlTokenKind::kTokenIdentifier:
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

ostream& operator<< (ostream& os, const msdlToken& elt)
{
  elt.print (os);
  return os;
}

//_______________________________________________________________________________
S_msdlTokensList msdlTokensList::create ()
{
  msdlTokensList* o =
    new msdlTokensList ();
  assert (o != nullptr);
  return o;
}

msdlTokensList::msdlTokensList ()
{}

msdlTokensList::~msdlTokensList ()
{}

void msdlTokensList::appendTokenToTokensList (
  const msdlToken& token)
{
  fTokensList.push_back (token);
}

string msdlTokensList::asString () const
{
  stringstream s;

  s <<
    "MSDL tokens list" <<
    singularOrPluralWithoutNumber (
      fTokensList.size (), "element", "elements");

  return s.str ();
}

void msdlTokensList::print (ostream& os) const
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

    list<msdlToken>::const_iterator
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

ostream& operator<< (ostream& os, const S_msdlTokensList& elt)
{
  elt->print (os);
  return os;
}


} // namespace
