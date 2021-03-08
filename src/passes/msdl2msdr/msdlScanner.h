/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msdlScanner__
#define __msdlScanner__

#include "smartpointer.h"

#include "msdlKeywords.h"
#include "msdlTokens.h"

#include "msdlScannerWaeHandlers.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
enum msdlIgnoreSeparatorTokensKind {
  kIgnoreSeparatorTokensNo,
  kIgnoreSeparatorTokensYes
};

string msdlIgnoreSeparatorTokensKindAsString (
  msdlIgnoreSeparatorTokensKind ignoreSeparatorTokensKind);

//________________________________________________________________________
class msdlScanner : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlScanner> create (
                            string   inputSourceName,
                            istream& inputStream);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlScanner (
                            string   inputSourceName,
                            istream& inputStream);

    virtual               ~msdlScanner ();

    // set and get
    // ------------------------------------------------------

    int                   getCurrentLineNumber () const
                              { return fCurrentLineNumber; }

    bool                  getInputIsEmpty () const
                              { return fInputIsEmpty; }

    const msdlToken&      getCurrentToken () const
                              { return fCurrentToken; }

    bool                  getSourceIsLexicallyCorrect () const
                              { return fSourceIsLexicallyCorrect; }

  public:

    // public services
    // ------------------------------------------------------


    msdlTokenKind         fetchNextToken (
                            msdlIgnoreSeparatorTokensKind
                              ignoreSeparatorTokens);

    void                  scanWholeInputAtOnce (); // for tests

    void                  scanAllTheInputAtOnce (
                            msdlIgnoreSeparatorTokensKind
                              ignoreSeparatorTokens);

    void                  translateAllTheInputToKeywordsLanguage (
                            msdlKeywordsLanguageKind keywordsTranslationLanguage,
                            msdlCommentsTypeKind     commentsTypeKind);

  public:

    // print
    // ------------------------------------------------------

    string                currentLocationAsString () const;

    string                asString () const;

    void                  print (ostream& os) const;

  private:

    // private services
    // ------------------------------------------------------

    void                  populateInputString (istream& inputStream);

    char                  fetchNextCharacter ();

     void                 handleEndOfLine ();

    string                currentCharacterAsString () const;

    void                  handleSlash ();
    void                  handlePercent ();

    void                  acceptAString ();

    void                  acceptAName ();

    void                  acceptAnInteger ();

  private:

    // private fields
    // ------------------------------------------------------

    // trace
#ifdef TRACING_IS_ENABLED
    bool                  fTraceTokens;
    bool                  fTraceTokensDetails;
#endif

    // input source name
    string                fInputSourceName;

    // user language
    msdlUserLanguageKind  fUserLanguageKind;

    // input stream
    istream&              fInputStream;
    bool                  fInputIsEmpty;

    // input string
    string                fInputString;     // modern machines can cope with large data
    int                   fInputStringSize; // denormalization for speed

    // lines
    int                   fCurrentLineNumber;
    int                   fCurrentLinePositionInInput;
    int                   fCurrentLineSize;
    int                   fCurrentPositionInLine;
                            // -1 before the first character in the line,
                            // then from 0 to the length of the line minus 1

    // characters
    int                   fCurrentPositionInInput;
    char                  fCurrentCharacter;
    bool                  fNextCharacterIsAvailable;

    // tokens
    int                   fCurrentTokenPositionInInput;
    int                   fCurrentTokenPositionInLine;

    msdlToken             fCurrentToken;
    msdlTokenKind&        fCurrentTokenKind;
    msdlTokenDescription& fCurrentTokenDescription;

    int                   fTokensCounter;

    // keywords
    msdlKeywordsLanguageKind
                          fKeywordsInputLanguageKind;

    // tokens list
    bool                  fAppendTokensToList;
    msdlTokensList        fTokensList;

    // warnings and errors
    S_msdlScannerWaeHandler
                          fScannerWaeHandler;

    // lexical correctness
    bool                  fSourceIsLexicallyCorrect;
};
typedef SMARTP<msdlScanner> S_msdlScanner;
EXP ostream& operator<< (ostream& os, const msdlScanner& elt);


}


#endif
