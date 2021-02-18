/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdrTokens___
#define ___msdrTokens___

#include <list>

#include "smartpointer.h"

#include "msdrKeywords.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
enum msdrTokenKind {
  k_TokenEOF,  // 0, which is expected by yyparse() at the end of the input

  k_NoToken,

  k_TokenMalformed, // for error recovery

  // language-independent tokens
  // ------------------------------------
  kTokenSpace,
  kTokenTab,
  kTokenCarriageReturn,

  kTokenEndOfLine,

  kTokenParenthesizedComment,
  kTokenCommentToEndOfLine,

  kTokenEqualSign,

  kTokenComma,
  kTokenColon,
  kTokenSemiColon,

  kTokenPlus,
  kTokenMinus,
  kTokenStar,
  kTokenSlash,
  kTokenPercent,

  kTokenConcat,

  kTokenQuestionMark,

  kTokenLeftParenthesis,
  kTokenRightParenthesis,

  kTokenLeftBracket,
  kTokenRightBracket,

  kTokenQuote,
  kTokenDot,

  kTokenMeasure,
  kTokenDoubleBar,
  kTokenFinalBar,

  kTokenRepeatStart,
  kTokenRepeatEnd,

  kTokenInteger,
  kTokenDouble,

  kTokenString,

  kTokenIdentifier,

  // language-dependent keywords
  // ------------------------------------

  kTokenTitle,
  kTokenComposer,
  kTokenOpus,

  kTokenPitches,

  kTokenAnacrusis,

  kTokenBook,
  kTokenScore,
  kTokenPartGroup,
  kTokenPart,
  kTokenStaff,
  kTokenVoice,
  kTokenFragment,

  kTokenClef,

  kTokenKey,
  kTokenTreble,
  kTokenSoprano,
  kTokenAlto,
  kTokenBaritone,
  kTokenBass,

  kTokenTime
};

string msdrTokenKindAsString (
  msdrTokenKind tokenKind);

enum msdrTokenDescriptionKind {
  kTokenDescriptionKeyword,
  kTokenDescriptionInteger,
  kTokenDescriptionDouble,
  kTokenDescriptionCharacter,
  kTokenDescriptionString
};

string msdrTokenDescriptionKindAsString (
  msdrTokenDescriptionKind tokenDescriptionKind);

//________________________________________________________________________
class EXP msdrTokenDescription : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdrTokenDescription> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdrTokenDescription ();

    virtual               ~msdrTokenDescription ();

    // set and get
    // ------------------------------------------------------

    void                  setKeywordKind (msdrKeywordKind value)
                              {
                                fTokenDescriptionKind = kTokenDescriptionKeyword;
                                fKeywordKind          = value;
                              }

    msdrKeywordKind       getKeywordKind () const;

    void                  setInteger (int value)
                              {
                                fTokenDescriptionKind = kTokenDescriptionInteger;
                                fInteger              = value;
                              }

    int                   getInteger () const;

    void                  setDouble (double value)
                              {
                                fTokenDescriptionKind = kTokenDescriptionDouble;
                                fDouble               = value;
                              }

    double                getDouble () const;

    void                  setCharacter (char value)
                              {
                                fTokenDescriptionKind = kTokenDescriptionCharacter;
                                fCharacter            = value;
                              }

    char                  getCharacter () const;

    void                  setString (string value)
                              {
                                fTokenDescriptionKind = kTokenDescriptionString;
                                fString               = value;
                              }

    string                getString () const;

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    // MSDL token descritption with variants handling (variants are not in C++-11)
    msdrTokenDescriptionKind
                          fTokenDescriptionKind;

    msdrKeywordKind       fKeywordKind;

    int                   fInteger;
    double                fDouble;

    char                  fCharacter;
    string                fString;
};
typedef SMARTP<msdrTokenDescription> S_msdrTokenDescription;
EXP ostream& operator<< (ostream& os, const msdrTokenDescription& elt);

//________________________________________________________________________
class EXP msdrToken : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdrToken> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdrToken ();

                          msdrToken (
                            msdrTokenKind tokenKind);

                          msdrToken (
                            msdrTokenKind   tokenKind,
                            msdrKeywordKind value);

                          msdrToken (
                            msdrTokenKind tokenKind,
                            int           value);

                          msdrToken (
                            msdrTokenKind tokenKind,
                            string        value);

                          msdrToken (
                            msdrTokenKind tokenKind,
                            double        value);

    virtual               ~msdrToken ();

    // set and get
    // ------------------------------------------------------

    void                  setTokenKind (msdrTokenKind tokenKind)
                              { fTokenKind = tokenKind; }

    msdrTokenKind         getTokenKind () const
                              { return fTokenKind; }

    msdrTokenKind&        getTokenKindToModify ()
                              { return fTokenKind; }

    void                  setTokenDescription (msdrTokenDescription& tokenDescription)
                              { fTokenDescription = tokenDescription; }

    const msdrTokenDescription&
                          getTokenDescription () const
                              { return fTokenDescription; }

    msdrTokenDescription& getTokenDescriptionToModify ()
                              { return fTokenDescription; }

    void                  setTokenLineNumber (int tokenLineNumber)
                              { fTokenLineNumber = tokenLineNumber; }

    int                   getTokenLineNumber () const
                              { return fTokenLineNumber; }

    void                  setTokenPositionInLine (int tokenPositionInLine)
                              { fTokenPositionInLine = tokenPositionInLine; }

    int                   getTokenPositionInLine () const
                              { return fTokenPositionInLine; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    string                asMsdlString () const;

    void                  print (ostream& os) const;

  private:

    // private methods
    // ------------------------------------------------------

    void                  initializeToken ();

  private:

    // private fields
    // ------------------------------------------------------

    // the MSDL token
    msdrTokenKind         fTokenKind;

    // its descritption with variants handling
    msdrTokenDescription  fTokenDescription;

    int                   fTokenLineNumber;
    int                   fTokenPositionInLine;
};
typedef SMARTP<msdrToken> S_msdrToken;
EXP ostream& operator<< (ostream& os, const msdrToken& elt);

//________________________________________________________________________
class EXP msdrTokensList : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdrTokensList> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdrTokensList ();

    virtual               ~msdrTokensList ();

  public:

    // set and get
    // ------------------------------------------------------

    const list<msdrToken>&
                          getTokensList () const
                              { return fTokensList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendTokenToTokensList (
                            const msdrToken& token);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    list<msdrToken>       fTokensList;
};
typedef SMARTP<msdrTokensList> S_msdrTokensList;
EXP ostream& operator<< (ostream& os, const S_msdrTokensList& elt);


}


#endif
