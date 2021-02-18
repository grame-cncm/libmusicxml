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

#include "msdrKeywords.h"
#include "msdrTokens.h"

#include "msdlScannerWaeHandlers.h"


using namespace std;

namespace MusicXML2
{

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


    msdrTokenKind         nextTokenPlease ();

    void                  scan ();

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

    char                  nextCharacterPlease ();

    string                currentCharacterAsString () const;

    void                  handleSlash ();
    void                  handlePercent ();

    void                  acceptAString ();

    void                  appendChunkToCurrentString (string chunk)
                              { fCurrentString += chunk; }

    void                  acceptAName ();

    void                  acceptAnInteger ();

    virtual void          lexicalError (string message);

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

    unsigned int          fCurrentPositionInInput;
    char                  fCurrentCharacter;
    bool                  fNextCharacterIsAvailable;

    unsigned int          fCurrentLineNumber;
    unsigned int          fCurrentLineSize;
    unsigned int          fCurrentPositionInLine;
                            // 0 before the first character in the line,
                            // then from 1 to the length of the line

    unsigned int          fCurrentTokenPositionInInput;
    unsigned int          fCurrentTokenPositionInLine;

    msdrToken             fCurrentToken;
    msdrTokenKind&        fCurrentTokenKind;
    msdrTokenDescription& fCurrentTokenDescription;
    int                   fTokensCounter;

    bool                  fAppendTokensToList;
    msdrTokensList        fTokensList;

    string                fCurrentString; // to concatenate chunks into

    bool                  fSourceIsLexicallyCorrect;
};
typedef SMARTP<msdlScanner> S_msdlScanner;
EXP ostream& operator<< (ostream& os, const msdlScanner& elt);


}


#endif
