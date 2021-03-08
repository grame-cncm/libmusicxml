/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdlTokens___
#define ___msdlTokens___

#include <list>
#include <set>

#include "smartpointer.h"

#include "msdlKeywords.h"

#include "msdlOah.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
enum class msdlTokenKind {
  k_TokenEOF,  // 0, which is expected by yyparse() at the end of the input JMI

  k_NoToken,

  k_TokenMalformed, // for error recovery

  // separators, for use by MSDL whole input translation
  // ------------------------------------

  kTokenSpace,
  kTokenTab,
  kTokenCarriageReturn,

  kTokenEndOfLine,

  kTokenParenthesizedComment,
  kTokenCommentToEndOfLine,

  // language-independent tokens
  // ------------------------------------

  kTokenEqualSign,

  kTokenComma,
  kTokenColon,
  kTokenSemiColon,

  kTokenPlus,
  kTokenMinus,
  kTokenStar,
  kTokenSlash,
  kTokenPercent, // don't keep JMI ???

  kTokenConcat,

  kTokenQuestionMark,

  kTokenTilda,

  kTokenLeftAngle,
  kTokenRightAngle,

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

  // translatable tokens
  // ------------------------------------

  kTokenInteger,
//  kTokenDouble,

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
  kTokenMusic, // single- or multi-voice staff
  kTokenFragment,

  kTokenClef,
  kTokenTreble,
  kTokenSoprano,
  kTokenAlto,
  kTokenTenor,
  kTokenBaryton,
  kTokenBass,

  kTokenKey,

  kTokenTime,

  // aliases
  // ------------------------------------

  AllFirst = k_TokenEOF,
  AllLast  = kTokenTime,

  NonSeparatorsFirst = kTokenEqualSign,
  NonSeparatorsLast  = kTokenTime,

  TranslatableFirst = kTokenInteger,
  TranslatableLast  = kTokenIdentifier,

  LanguageIndependentFirst = kTokenEqualSign,
  LanguageIndependentLast  = kTokenIdentifier,

  LanguageDependentFirst = kTokenTitle,
  LanguageDependentLast  = kTokenTime
};

string msdlTokenKindAsString (
  msdlTokenKind tokenKind);

string msdlTokenKindAsMsdlString (
  msdlTokenKind            tokenKind,
  msdlKeywordsLanguageKind languageKind);

msdlTokenKind msdlTokenKindFromKeywordKind (
  msdlKeywordKind keywordKind);

string existingTokensInLanguage (
  msdlKeywordsLanguageKind keywordsLanguageKind,
  unsigned int             namesListMaxLength);

//________________________________________________________________________
template< typename T >
class EnumNonSeparators
{
  public:

    class Iterator
    {
      public:

        Iterator (int value)
          : fIterationIndex (value)
            {}

        T operator* (void) const
            { return (T) fIterationIndex; }

        void operator++ (void)
            {  ++fIterationIndex; }

        bool operator != (Iterator rhs)
            {  return fIterationIndex != rhs.fIterationIndex; }

      private:

        int fIterationIndex;
     };
};

template< typename T >
typename EnumNonSeparators<T>::Iterator begin (EnumNonSeparators<T>)
{
  return typename EnumNonSeparators<T>::Iterator ((int)T::NonSeparatorsFirst);
}

template< typename T >
typename EnumNonSeparators<T>::Iterator end (EnumNonSeparators<T>)
{
  return typename EnumNonSeparators<T>::Iterator (((int)T::NonSeparatorsLast) + 1);
}

//________________________________________________________________________
template< typename T >
class EnumLanguageIndependent
{
  public:

    class Iterator
    {
      public:

        Iterator (int value)
          : fIterationIndex (value)
            {}

        T operator* (void) const
            { return (T) fIterationIndex; }

        void operator++ (void)
            {  ++fIterationIndex; }

        bool operator != (Iterator rhs)
            {  return fIterationIndex != rhs.fIterationIndex; }

      private:

        int fIterationIndex;
     };
};

template< typename T >
typename EnumLanguageIndependent<T>::Iterator begin (EnumLanguageIndependent<T>)
{
  return typename EnumLanguageIndependent<T>::Iterator ((int)T::LanguageIndependentFirst);
}

template< typename T >
typename EnumLanguageIndependent<T>::Iterator end (EnumLanguageIndependent<T>)
{
  return typename EnumLanguageIndependent<T>::Iterator (((int)T::LanguageIndependentLast) + 1);
}

//________________________________________________________________________
template< typename T >
class EnumLanguageDependent
{
  public:

    class Iterator
    {
      public:

        Iterator (int value)
          : fIterationIndex (value)
            {}

        T operator* (void) const
            { return (T) fIterationIndex; }

        void operator++ (void)
            {  ++fIterationIndex; }

        bool operator != (Iterator rhs)
            {  return fIterationIndex != rhs.fIterationIndex; }

      private:

        int fIterationIndex;
     };
};

template< typename T >
typename EnumLanguageDependent<T>::Iterator begin (EnumLanguageDependent<T>)
{
  return typename EnumLanguageDependent<T>::Iterator ((int)T::LanguageDependentFirst);
}

template< typename T >
typename EnumLanguageDependent<T>::Iterator end (EnumLanguageDependent<T>)
{
  return typename EnumLanguageDependent<T>::Iterator (((int)T::LanguageDependentLast) + 1);
}

//________________________________________________________________________
class msdlTokenKindsSet;
typedef SMARTP<msdlTokenKindsSet> S_msdlTokenKindsSet;

class msdlTokenKindsSet : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlTokenKindsSet> create ();

    static SMARTP<msdlTokenKindsSet> create (
                            initializer_list<msdlTokenKind> elements);

    static SMARTP<msdlTokenKindsSet> create (
                            msdlTokenKindsSet& tokenKindsSet);

    SMARTP<msdlTokenKindsSet> createClone ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlTokenKindsSet ();

                          msdlTokenKindsSet (
                            initializer_list<msdlTokenKind> elements);

                          msdlTokenKindsSet (
                            msdlTokenKindsSet& tokenKindsSet);

    virtual               ~msdlTokenKindsSet ();

    // set and get
    // ------------------------------------------------------

    const set<msdlTokenKind>&
                          getTokenKindsSet () const
                              { return fTokenKindsSet; }

    int                   getTokenKindsSetSize () const
                              { return fTokenKindsSet.size (); }

  public:

    // public services
    // ------------------------------------------------------

    void                  addTokenKind (msdlTokenKind value)
                              { fTokenKindsSet.insert (value); }

    void                  removeTokenKind (msdlTokenKind value)
                              { fTokenKindsSet.erase (value); }

    bool                  containsTokenKind (msdlTokenKind value) const
                              { return fTokenKindsSet.count (value) != 0; }

    void                  addElementsFrom (S_msdlTokenKindsSet tokenKindsSet);

    void                  removeElementsFrom (S_msdlTokenKindsSet tokenKindsSet);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    set<msdlTokenKind>    fTokenKindsSet;
};
EXP ostream& operator<< (ostream& os, const msdlTokenKindsSet& elt);

EXP S_msdlTokenKindsSet operator+= (
  S_msdlTokenKindsSet aTokenKindsSet,
  S_msdlTokenKindsSet anotherTokenKindsSet);

EXP S_msdlTokenKindsSet operator-= (
  S_msdlTokenKindsSet aTokenKindsSet,
  S_msdlTokenKindsSet anotherTokenKindsSet);

EXP S_msdlTokenKindsSet operator+ (
  S_msdlTokenKindsSet aTokenKindsSet,
  S_msdlTokenKindsSet anotherTokenKindsSet);

EXP S_msdlTokenKindsSet operator- (
  S_msdlTokenKindsSet aTokenKindsSet,
  S_msdlTokenKindsSet anotherTokenKindsSet);

/*
  see https://stackoverflow.com/questions/1448396/how-to-use-enums-as-flags-in-c


// initializer_list::begin/end
#include <iostream>          // std::cout
#include <string>            // std::string
#include <sstream>           // std::stringstream
#include <initializer_list>  // std::initializer_list

struct myclass {
  std::string str;
  myclass(std::initializer_list<int> args) {
    std::stringstream ss;
    std::initializer_list<int>::iterator it;  // same as: const int* it
    for ( it=args.begin(); it!=args.end(); ++it) ss << ' ' << *it;
    str = ss.str();
  }
};

int main ()
{
  myclass myobject {10, 20, 30};
  std::cout << "myobject contains:" << myobject.str << '\n';
  return 0;
}

EXP tokenKindsSet& operator|= (tokenKindsSet& ts, msdlTokenKind value) const
{
  ts.addTokenKind (value);
  return ts;
}


// prefix operators
msrOctaveKind& operator++ (msrOctaveKind& octaveKind);
msrOctaveKind& operator-- (msrOctaveKind& octaveKind);

// postfix operators
msrOctaveKind operator++ (msrOctaveKind& octaveKind, int);
msrOctaveKind operator-- (msrOctaveKind& octaveKind, int);
*/


//________________________________________________________________________
enum class msdlTokenDescriptionKind {
  kTokenKeyword,
  kTokenInteger,
  kTokenDouble,
  kTokenCharacter,
  kTokenString
};

string msdlTokenDescriptionKindAsString (
  msdlTokenDescriptionKind tokenDescriptionKind);

//________________________________________________________________________
class EXP msdlTokenDescription : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlTokenDescription> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlTokenDescription ();

    virtual               ~msdlTokenDescription ();

    // set and get
    // ------------------------------------------------------

    void                  setKeywordKind (msdlKeywordKind value)
                              {
                                fTokenDescriptionKind =
                                  msdlTokenDescriptionKind::kTokenKeyword;
                                fKeywordKind          = value;
                              }

    msdlKeywordKind       getKeywordKind () const;

    void                  setInteger (int value)
                              {
                                fTokenDescriptionKind =
                                  msdlTokenDescriptionKind::kTokenInteger;
                                fInteger              = value;
                              }

    int                   getInteger () const;

    void                  setDouble (double value)
                              {
                                fTokenDescriptionKind =
                                  msdlTokenDescriptionKind::kTokenDouble;
                                fDouble               = value;
                              }

    double                getDouble () const;

    void                  setCharacter (char value)
                              {
                                fTokenDescriptionKind =
                                  msdlTokenDescriptionKind::kTokenCharacter;
                                fCharacter            = value;
                              }

    char                  getCharacter () const;

    void                  setString (string value)
                              {
                                fTokenDescriptionKind =
                                  msdlTokenDescriptionKind::kTokenString;
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

    // MSDL token description with variants handling (variants are not in C++-11)
    msdlTokenDescriptionKind
                          fTokenDescriptionKind;

    msdlKeywordKind       fKeywordKind;

    int                   fInteger;
    double                fDouble;

    char                  fCharacter;
    string                fString;
};
typedef SMARTP<msdlTokenDescription> S_msdlTokenDescription;
EXP ostream& operator<< (ostream& os, const msdlTokenDescription& elt);

//________________________________________________________________________
class EXP msdlToken : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlToken> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlToken ();

                          msdlToken (
                            msdlTokenKind tokenKind);

                          msdlToken (
                            msdlTokenKind   tokenKind,
                            msdlKeywordKind value);

                          msdlToken (
                            msdlTokenKind tokenKind,
                            int           value);

                          msdlToken (
                            msdlTokenKind tokenKind,
                            string        value);

                          msdlToken (
                            msdlTokenKind tokenKind,
                            double        value);

    virtual               ~msdlToken ();

    // set and get
    // ------------------------------------------------------

    void                  setTokenKind (msdlTokenKind tokenKind)
                              { fTokenKind = tokenKind; }

    msdlTokenKind         getTokenKind () const
                              { return fTokenKind; }

    msdlTokenKind&        getTokenKindToModify ()
                              { return fTokenKind; }

    void                  setTokenDescription (msdlTokenDescription& tokenDescription)
                              { fTokenDescription = tokenDescription; }

    const msdlTokenDescription&
                          getTokenDescription () const
                              { return fTokenDescription; }

    msdlTokenDescription& getTokenDescriptionToModify ()
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

    string                asMsdlString (
                            msdlKeywordsLanguageKind languageKind,
                            msdlCommentsTypeKind     commentsTypeKind) const;

    void                  print (ostream& os) const;

  private:

    // private methods
    // ------------------------------------------------------

    void                  initializeToken ();

  private:

    // private fields
    // ------------------------------------------------------

    // the MSDL token
    msdlTokenKind         fTokenKind;

    // its description with variants handling
    msdlTokenDescription  fTokenDescription;

    int                   fTokenLineNumber;
    int                   fTokenPositionInLine;
};
typedef SMARTP<msdlToken> S_msdlToken;
EXP ostream& operator<< (ostream& os, const msdlToken& elt);

//________________________________________________________________________
class EXP msdlTokensList : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlTokensList> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlTokensList ();

    virtual               ~msdlTokensList ();

  public:

    // set and get
    // ------------------------------------------------------

    const list<msdlToken>&
                          getTokensList () const
                              { return fTokensList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendTokenToTokensList (
                            const msdlToken& token);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    string                asMsdlString (
                            msdlKeywordsLanguageKind languageKind,
                            msdlCommentsTypeKind     commentsTypeKind) const;

    void                  print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    list<msdlToken>       fTokensList;
};
typedef SMARTP<msdlTokensList> S_msdlTokensList;
EXP ostream& operator<< (ostream& os, const S_msdlTokensList& elt);


}


#endif
