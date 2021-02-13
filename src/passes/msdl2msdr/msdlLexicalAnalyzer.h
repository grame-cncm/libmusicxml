/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msdlLexicalAnalyzer__
#define __msdlLexicalAnalyzer__

#include <string>

#include "smartpointer.h"

#include "msdrKeywords.h"
#include "msdrTokens.h"

#include "msdlLexicalWaeHandler.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
class msdlLexicalAnalyzer : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlLexicalAnalyzer> create (
                            istream& inputStream);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlLexicalAnalyzer (
                            istream& inputStream);

    virtual               ~msdlLexicalAnalyzer ();

    // set and get
    // ------------------------------------------------------

    double                getCurrenInteger () const
                              { return fCurrentInteger; }

    double                getCurrenDouble () const
                              { return fCurrentDouble; }

    string                getCurrentName () const
                              { return fCurrentName; }

    string                getCurrentString () const
                              { return fCurrentString; }

    bool                  getSourceIsLexicallyCorrect ();

  public:

    // public services
    // ------------------------------------------------------

    void                  analyze ();

  public:

    // print
    // ------------------------------------------------------

    string                currentPositionAsString () const;

    string                asString () const;

    void                  print (ostream& os) const;

  private:

    // private services
    // ------------------------------------------------------

    void                  nextCharacterPlease ();
    void                  oneCharacterBackwards ();

    void                  appendFragmentToCurrentString (string fragment)
                              { fCurrentString += fragment; }

    msdrTokenKind         acceptATerminal ();

    msdrTokenKind         acceptASingleQuotedString ();
    msdrTokenKind         acceptADoubleQuotedString ();

    msdrTokenKind         acceptAName ();

    msdrTokenKind         acceptANumber ();
    void                  acceptAnExponent ();

    void                  handleAnEndOfLine ();

    void                  EchoDesCaracteresAcceptes ();

    virtual void          lexicalError (string message);

    void                  handleMsdrToken (
                            int           inputLineNumber,
                            string        tokenText,
                            msdrTokenKind tokenKind);

  private:

    // private fields
    // ------------------------------------------------------

    istream&              fInputStream;

    S_msdlLexicalWaeHandler
                          fLexicalWaeHandler;

    string                fCurrentLine;
    unsigned int          fCurrentLineSize;
    unsigned int          fCurrentLineNumber;

    char                  fCurrentCharacter;
    unsigned int          fCurrentPositionInLine;

    bool                  fAFirstTokenHasAlreadyBeenAccepted;
    unsigned int          fTokenStartPosition;
    msdrTokenKind         fCurrentTokenKind;

    int                   fCurrentInteger;
    double                fCurrentDouble;
    string                fCurrentName;
    string                fCurrentString;

    bool                  fSourceIsLexicallyCorrect;
};
typedef SMARTP<msdlLexicalAnalyzer> S_msdlLexicalAnalyzer;
EXP ostream& operator<< (ostream& os, const msdlLexicalAnalyzer& elt);


}


#endif
