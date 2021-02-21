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

#include <string>

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
                            istream& inputStream);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlScanner (
                            istream& inputStream);

    virtual               ~msdlScanner ();

    // set and get
    // ------------------------------------------------------

    bool                  getInputIsEmpty () const
                              { return fInputIsEmpty; }

    bool                  getSourceIsLexicallyCorrect () const
                              { return fSourceIsLexicallyCorrect; }

  public:

    // public services
    // ------------------------------------------------------


    msdrTokenKind         fetchNextToken (
                            msdlIgnoreSeparatorTokensKind
                              ignoreSeparatorTokens = kIgnoreSeparatorTokensNo);

    void                  scanAllTheInputAtOnce (
                            msdlIgnoreSeparatorTokensKind
                              ignoreSeparatorTokens = kIgnoreSeparatorTokensNo);

    void                  translateAllTheInputToKeywordsLanguage ();

  public:

    // print
    // ------------------------------------------------------

    string                currentPositionAsString () const;

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

    istream&              fInputStream;
    bool                  fInputIsEmpty;

    string                fInputString;     // modern machines can cope with large data
    unsigned int          fInputStringSize; // denormalization for speed

    S_msdlScannerWaeHandler
                          fScannerWaeHandler;

#ifdef TRACING_IS_ENABLED
    bool                  fTraceTokens;
    bool                  fTraceTokensDetails;
#endif

    int                   fCurrentPositionInInput;
    char                  fCurrentCharacter;
    bool                  fNextCharacterIsAvailable;

    int                   fCurrentLineNumber;
    int                   fCurrentLineSize;
    int                   fCurrentPositionInLine;
                            // 0 before the first character in the line,
                            // then from 1 to the length of the line

    int                   fCurrentTokenPositionInInput;
    int                   fCurrentTokenPositionInLine;

    msdrToken             fCurrentToken;
    msdrTokenKind&        fCurrentTokenKind;
    msdrTokenDescription& fCurrentTokenDescription;
    int                   fTokensCounter;

    msdlKeywordsLanguageKind
                          fKeywordsInputLanguageKind;

    bool                  fAppendTokensToList;
    msdrTokensList        fTokensList;

    bool                  fSourceIsLexicallyCorrect;
};
typedef SMARTP<msdlScanner> S_msdlScanner;
EXP ostream& operator<< (ostream& os, const msdlScanner& elt);


}


#endif
