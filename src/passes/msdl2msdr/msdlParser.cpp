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

#include "msdlParserWaeHandlers.h"

#include "utilities.h"

#include "msdlParser.h"

#include "waeExceptions.h"

#include "msdl2msdrOah.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
S_msdlParser msdlParser::create (
  string   inputSourceName,
  istream& inputStream)
{
  msdlParser* o =
    new msdlParser (
      inputSourceName,
      inputStream);
  assert (o != nullptr);
  return o;
}

msdlParser::msdlParser (
  string   inputSourceName,
  istream& inputStream)
    : fScanner (
        inputSourceName,
        inputStream),
      fCurrentToken (
        fScanner.getCurrentToken ())
{
  // trace
#ifdef TRACING_IS_ENABLED
  fTraceSyntax        = gGlobalMsdl2msdrOahGroup->getTraceSyntax ();
  fTraceSyntaxDetails = gGlobalMsdl2msdrOahGroup->getTraceSyntaxDetails ();
#endif

  // user language
  fUserLanguageKind =
    gGlobalMsdl2msdrOahGroup->
      getMsdlUserLanguageKind ();

  // keywords language
  fKeywordsInputLanguageKind =
    gGlobalMsdl2msdrOahGroup->
      getMsdlKeywordsInputLanguageKind ();

  // input source name
  fInputSourceName = inputSourceName;

  // warnings and errors
  switch (gGlobalMsdl2msdrOahGroup->getMsdlUserLanguageKind ()) {
    case kUserLanguageEnglish:
      fParserWaeHandler = msdlParserEnglishWaeHandler::create ();
      break;
    case kUserLanguageFrench:
      fParserWaeHandler = msdlParserFrenchWaeHandler::create ();
      break;
    case kUserLanguageItalian:
      fParserWaeHandler = msdlParserItalianWaeHandler::create ();
      break;
    case kUserLanguageGerman:
      fParserWaeHandler = msdlParserGermanWaeHandler::create ();
      break;
    case kUserLanguageSpanish:
      fParserWaeHandler = msdlParserSpanishWaeHandler::create ();
      break;
    case kUserLanguageDutch:
      fParserWaeHandler = msdlParserDutchWaeHandler::create ();
      break;
  } // switch

  // tokens
  initializeTokensHandling ();

  // syntax correctness
  fEmptyTokenKindsSet =
    msdlTokenKindsSet::create ();

  fSourceIsSyntacticallyCorrect = true;

  // the MSR being built
  fAnacrusis = false;

  fCurrentStaffNumber = 0;
  fCurrentVoiceNumber = 0;
  fCurrentMeasureNumber = 0;
}

void msdlParser::initializeTokensHandling ()
{
  fIgnoreSeparatorTokensKind = kIgnoreSeparatorTokensNo; // just to initialize it

  fIdentificationFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenTitle,
      msdlTokenKind::kTokenComposer,
      msdlTokenKind::kTokenOpus});

  fStructureFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenBook,
      msdlTokenKind::kTokenScore,
      msdlTokenKind::kTokenPartGroup,
      msdlTokenKind::kTokenPart,
      msdlTokenKind::kTokenMusic});

  fBookFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenBook,
      msdlTokenKind::kTokenScore,
      msdlTokenKind::kTokenPartGroup,
      msdlTokenKind::kTokenPart,
      msdlTokenKind::kTokenMusic});

  fScoreFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenScore,
      msdlTokenKind::kTokenPartGroup,
      msdlTokenKind::kTokenPart,
      msdlTokenKind::kTokenMusic});

  fPartGroupFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenPartGroup,
      msdlTokenKind::kTokenPart,
      msdlTokenKind::kTokenMusic});

  fPartFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenPart,
      msdlTokenKind::kTokenMusic});

  fMusicFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenMusic});
}

msdlParser::~ msdlParser ()
{}

//________________________________________________________________________
void msdlParser::displayTokenKindsSetsStack (string context)
{
  unsigned int
    tokensSetsStackSize = fMsdlTokensSetsStack.size ();

  gLogStream <<
    endl <<
    context << ": tokens stack contents (" <<
    singularOrPlural (
      tokensSetsStackSize, "set", "sets") <<
    "):" <<
    " context: " << context <<
    endl;

  if (tokensSetsStackSize) {
    list<S_msdlTokenKindsSet>::const_iterator
      iBegin = fMsdlTokensSetsStack.begin (),
      iEnd   = fMsdlTokensSetsStack.end (),
      i      = iBegin;

    ++gIndenter;

    for ( ; ; ) {
      S_msdlTokenKindsSet tokenKindsSet = (*i);

      // sanity check
      msgAssert (
        __FILE__, __LINE__,
        tokenKindsSet != nullptr,
        "tokenKindsSet is null");

      gLogStream <<
        "v " <<
        tokenKindsSet->asString () <<
        endl;

      if (++i == iEnd) break;
      // no endl here
    } // for

    --gIndenter;
  }

  gLogStream <<
    "    ---------------------------------------" <<
    endl;
}

//________________________________________________________________________
string msdlParser::currentTokenAsString () const
{
  return
    fCurrentToken.asString ();
}

string msdlParser::currentTokenAsMsdlString () const
{
  return
    fCurrentToken.asMsdlString (
      fKeywordsInputLanguageKind,
      kCommentsTypePercent);
}

//________________________________________________________________________
bool msdlParser::isCurrentTokenKindInSetsStack (string context)
{
  bool result = false;

  unsigned int
    tokensSetsStackSize = fMsdlTokensSetsStack.size ();

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntaxDetails) {
    gLogStream <<
      endl <<
      "-->isCurrentTokenKindInSetsStack()" <<
      context << ": tokens stack contents (" <<
      singularOrPlural (
        tokensSetsStackSize, "set", "sets") <<
      "):" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      " context: " << context <<
      endl;
  }
#endif

  // the stack cannot be empty JMI ???
  // since msdlTokenKind::kTokenEOF is in the set at the bottom of the stack

  if (tokensSetsStackSize) {
    list<S_msdlTokenKindsSet>::const_iterator
      iBegin = fMsdlTokensSetsStack.begin (),
      iEnd   = fMsdlTokensSetsStack.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msdlTokenKindsSet stoppersSet = ((*i));

  #ifdef TRACING_IS_ENABLED
  //    if (fTraceSyntaxDetails) {
      if (fTraceSyntax) {
        gLogStream <<
          endl <<
          "-->isCurrentTokenKindInSetsStack()" <<
          ", fCurrentToken: " << currentTokenAsMsdlString () <<
          " context: " << context <<
          endl;

        displayTokenKindsSetsStack ("---isCurrentTokenKindInSetsStack()");
      }
  #endif

      if (stoppersSet->containsTokenKind (fCurrentTokenKind)) {
        // found it
        break;
      }

      if (++i == iEnd) break;
      // no endl here
    } // for
  }

  return result;
}

// --------------------------------------------------------------------------
//  msdlParser::fetchNextToken
// --------------------------------------------------------------------------

void msdlParser::fetchNextToken ()
{
  fCurrentTokenKind =
    fScanner.fetchNextToken (
      fIgnoreSeparatorTokensKind);

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "*** fetchNextToken()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }
#endif
}

//________________________________________________________________________
//  msdlParser::checkMandatoryTokenKind
// --------------------------------------------------------------------------

bool msdlParser::checkMandatoryTokenKind (
  msdlTokenKind tokenKind,
  string        context)
{
#ifdef TRACING_IS_ENABLED
  if (fTraceSyntaxDetails) {
    gLogStream <<
      endl <<
      "-->checkMandatoryTokenKind()" <<
      ", tokenKind: " << msdlTokenKindAsString (tokenKind) <<
      " context: " << context <<
      endl;
  }
#endif

  bool result = false;

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("checkMandatoryTokenKind()");
  }
#endif

  // is the current token kind tokenKind?
  if (fCurrentTokenKind == tokenKind) {
    // yes, we found it
    result = true;
  }

  else {
    // no, error, fCurrentToken is not the one we expected
    fParserWaeHandler->
      unexpectedToken (fCurrentToken, context);

    if (isCurrentTokenKindInSetsStack (context)) {
      // don't overtake it, we stop at it
    }

    else {
      // consume tokens if needed until one is found that is either
      // tokenKind or in the stopperTokensSet sets in the stack,
      // in order to re-synchronize the parser with a token it is expecting

      // the stack cannot be empty
      // since msdlTokenKind::kTokenEOF is in the set at the bottom of the stack

      displayTokenKindsSetsStack ("checkMandatoryTokenKind()");

      while (true) {
        // let's ignore fCurrentToken
#ifdef TRACING_IS_ENABLED
        if (fTraceSyntaxDetails) {
          fParserWaeHandler->
            ignoringToken (fCurrentToken, context);
        }
#endif

        // fetch next token
        fetchNextToken ();

        if (fCurrentTokenKind == tokenKind) {
          // we found it
          result = true;
          break;
        }
      } // while
    }
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntaxDetails) {
    gLogStream <<
      endl <<
      "<-- checkMandatoryTokenKind()" <<
      ", context: [" << context << "]" <<
      ", tokenKind: " << msdlTokenKindAsString (tokenKind) <<
      ", result: " << booleanAsString (result) <<
      endl;
  }
#endif

  return result;
}

//________________________________________________________________________
//  msdlParser::checkMandatoryTokenKindsSet
// --------------------------------------------------------------------------

bool msdlParser:: checkMandatoryTokenKindsSet (
  S_msdlTokenKindsSet tokenKindsSet,
  string              context)
{
#ifdef TRACING_IS_ENABLED
  if (fTraceSyntaxDetails) {
    gLogStream <<
      endl <<
      "-->checkMandatoryTokenKindsSet()" <<
      ", context: [" << context << "]" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      ", tokenKindsSet: " << tokenKindsSet->asString () <<
      endl;
  }
#endif

  bool result = false;

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("checkMandatoryTokenKindsSet()");
  }
#endif

  // is the current token kind in tokenKindsSet?
  if (tokenKindsSet->containsTokenKind (fCurrentTokenKind)) {
    // fCurrentTokenKind is in tokenKindsSet
    result = true;
  }

  else {
    // no, error, fCurrentToken is not the one we expected
    fParserWaeHandler->
      unexpectedToken (fCurrentToken, context);

    if (isCurrentTokenKindInSetsStack (context)) {
      // don't overtake it, we stop at it
    }

    else {
      // consume tokens if needed until one is found that is either
      // tokenKind or in the stopperTokensSet sets in the stack,
      // in order to re-synchronize the parser with a token it is expecting

      // the stack cannot be empty
      // since msdlTokenKind::kTokenEOF is in the set at the bottom of the stack

      while (true) {
        // let's ignore fCurrentToken
#ifdef TRACING_IS_ENABLED
        if (fTraceSyntaxDetails) {
          fParserWaeHandler->
            ignoringToken (fCurrentToken, context);
        }
#endif
        fetchNextToken ();

        if (tokenKindsSet->containsTokenKind (fCurrentTokenKind)) {
          // fCurrentTokenKind is in tokenKindsSet
          result = true;
          break;
        }
      } // while
    }
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntaxDetails) {
    gLogStream <<
      endl <<
      "<-- checkMandatoryTokenKindsSet()" <<
      ", context: [" << context << "]" <<
      ", tokenKindsSet: " << tokenKindsSet->asString () <<
      ", result: " << booleanAsString (result) <<
      endl;
  }
#endif

  return result;
}

//________________________________________________________________________
//  msdlParser::checkOptionalTokenKind
// --------------------------------------------------------------------------

bool msdlParser::checkOptionalTokenKind (
  msdlTokenKind tokenKind,
  string        context)
{
#ifdef TRACING_IS_ENABLED
  if (fTraceSyntaxDetails) {
    gLogStream <<
      endl <<
      "-->checkOptionalTokenKind()" <<
      ", tokenKind: " << msdlTokenKindAsString (tokenKind) <<
  //    " context: " << context <<
      endl;
  }
#endif

  bool result = false;

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("checkOptionalTokenKind()");
  }
#endif

  if (fCurrentTokenKind == msdlTokenKind::k_TokenEOF) {
    result = false;
  }

  else {
    // is the current token kind tokenKind?
    if (fCurrentTokenKind == tokenKind) {
      // yes
      result = true;
    }

    else {
      // no

      // consume tokens until one is found that is either
      // tokenKind or in the stopperTokensSet sets in the stack,
      // in order to re-synchronize the parser with a token it is expecting

      // the stack cannot be empty
      // since msdlTokenKind::kTokenEOF is in the set at the bottom of the stack

      list<S_msdlTokenKindsSet>::const_iterator
        iBegin = fMsdlTokensSetsStack.begin (),
        iEnd   = fMsdlTokensSetsStack.end (),
        i      = iBegin;

      ++gIndenter;

      // this loop cannot run forever
      // since msdlTokenKind::kTokenEOF is in the set at the bottom of the stack
      for ( ; ; ) {
        fetchNextToken ();

        if (fCurrentTokenKind == tokenKind) {
          result = true;
          break;
        }

        else {
          S_msdlTokenKindsSet stoppersSet = ((*i));

          if (stoppersSet->containsTokenKind (fCurrentTokenKind)) {
            // fCurrentTokenKind is in this set
            break;
          }
        }

  #ifdef TRACING_IS_ENABLED
        if (fTraceSyntaxDetails) {
          fParserWaeHandler->
            ignoringToken (fCurrentToken, context);
        }
  #endif

        if (++i == iEnd) break;
        // no endl here
      } // for

      --gIndenter;
    }
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntaxDetails) {
    gLogStream <<
      endl <<
      "<-- checkOptionalTokenKind()" <<
      ", tokenKind: " << msdlTokenKindAsString (tokenKind) <<
  //    ", context: " << context <<
      ", result: " << booleanAsString (result) <<
      endl;
  }
#endif

  return result;
}

//________________________________________________________________________
//  msdlParser::checkOptionalTokenKindsSet
// --------------------------------------------------------------------------

bool msdlParser:: checkOptionalTokenKindsSet (
  S_msdlTokenKindsSet tokenKindsSet,
  string              context)
{
#ifdef TRACING_IS_ENABLED
  if (fTraceSyntaxDetails) {
    gLogStream <<
      endl <<
      "-->checkOptionalTokenKindsSet()" <<
      ", context: [" << context << "]" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      ", tokenKindsSet: " << tokenKindsSet->asString () <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("checkOptionalTokenKindsSet()");
  }
#endif

  bool result = false;

  if (fCurrentTokenKind == msdlTokenKind::k_TokenEOF) {
    result = false;
  }

  else {
    // is the current token kind in tokenKindsSet?
    if (tokenKindsSet->containsTokenKind (fCurrentTokenKind)) {
      // fCurrentTokenKind is in tokenKindsSet
      result = true;
    }

    else {
      // no, fCurrentToken is not the one we expected

      if (isCurrentTokenKindInSetsStack (context)) {
        // don't overtake it, we stop at it
      }

      else {
        // consume tokens if needed until one is found that is either
        // tokenKind or in the stopperTokensSet sets in the stack,
        // in order to re-synchronize the parser with a token it is expecting

        // the stack cannot be empty
        // since msdlTokenKind::kTokenEOF is in the set at the bottom of the stack

        while (false) { // JMI BLARK
          // let's ignore fCurrentToken
  #ifdef TRACING_IS_ENABLED
          if (fTraceSyntaxDetails) {
            fParserWaeHandler->
              ignoringToken (fCurrentToken, context);
          }
  #endif
          fetchNextToken ();

          if (tokenKindsSet->containsTokenKind (fCurrentTokenKind)) {
            // fCurrentTokenKind is in tokenKindsSet
            result = true;
            break;
          }
        } // while
      }
    }
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntaxDetails) {
    gLogStream <<
      endl <<
      "<-- checkOptionalTokenKindsSet()" <<
      ", context: [" << context << "]" <<
      ", tokenKindsSet: " << tokenKindsSet->asString () <<
      ", result: " << booleanAsString (result) <<
      endl;
  }
#endif

  return result;
}

// --------------------------------------------------------------------------
// private score building methods
// --------------------------------------------------------------------------

void msdlParser::createMeasureNumber (
  int inputLineNumber,
  int measureNumber)
{
  createVoiceIfNeeded (inputLineNumber);

  stringstream s;

  s << ++fCurrentMeasureNumber;

  // create the measure and append it to the voice
  fCurrentMeasure =
    fCurrentVoice->
      createMeasureAndAppendItToVoice (
        inputLineNumber,
        s.str (),
        kMeasureImplicitKindNo);
}

void msdlParser::createVoiceIfNeeded (int inputLineNumber)
{
  if (! fCurrentStaff) {
    createStaffIfNeeded (inputLineNumber);

    // create the voice
    fCurrentVoice =
      msrVoice::createRegularVoice (
        inputLineNumber,
        ++fCurrentVoiceNumber,
        fCurrentStaff);

    // append it to the staff
    fCurrentStaff->
      registerVoiceInStaff (
        inputLineNumber,
        fCurrentVoice);
  }
}

void msdlParser::createStaffIfNeeded (int inputLineNumber)
{
  if (! fCurrentStaff) {
    createPartIfNeeded (inputLineNumber);

    // create the staff
    fCurrentStaff =
      msrStaff::create (
        __LINE__,
        kStaffRegular,
        ++fCurrentStaffNumber,
        fCurrentPart);

    // append it to the part
    fCurrentPart ->
      addStaffToPartCloneByItsNumber ( // JMI NOT clone???
        fCurrentStaff);
  }
}

void msdlParser::createPartIfNeeded (int inputLineNumber)
{
  if (! fCurrentPart) {
    createPartGroupIfNeeded (inputLineNumber);

    // create the part
    fCurrentPart =
      msrPart::create (
        inputLineNumber,
        "Part???",
        fCurrentPartGroup);

    // append it to the part group
    fCurrentPartGroup->
      appendPartToPartGroup (
        fCurrentPart);
  }
}

void msdlParser::createPartGroupIfNeeded (int inputLineNumber)
{
  if (! fCurrentPartGroup) {
    createScoreIfNeeded (inputLineNumber);

    // create the part group
    fCurrentPartGroup =
      msrPartGroup::create (
        inputLineNumber,
          1, 1,
          "OnlyPartGroup",
          nullptr,
          fCurrentScore);

    // add the part group to the score
    fCurrentScore->
      addPartGroupToScore (fCurrentPartGroup);
  }
}

void msdlParser::createScoreIfNeeded (int inputLineNumber)
{
  if (! fCurrentScore) {
    // create the score
    createBookIfNeeded (inputLineNumber);

    // create the score
    fCurrentScore =
      msrScore::create (inputLineNumber);

    if (! fMsrIdentification) {
      // create the identification
      fMsrIdentification =
        msrIdentification::create (
          inputLineNumber);

      // register it in the score
      fCurrentScore->
        setIdentification (
          fMsrIdentification);

      // set the identification's software
      fMsrIdentification->
        appendSoftware (
          inputLineNumber,
          "MSDL compiler");
    }

    // add the score to the book
    fCurrentBook->
      addBookElementToBook (fCurrentScore);
  }
}

void msdlParser::createBookIfNeeded (int inputLineNumber)
{
  if (! fCurrentBook) {
    fCurrentBook =
      msrBook::create (inputLineNumber);
  }
}

// --------------------------------------------------------------------------
//  msdlParser::parse
// --------------------------------------------------------------------------

void msdlParser::parse ()
{
/* JMI
  gOutputStream << "gOutputStream" << endl;
  gLogStream << "gLogStream" << endl;
  cout << "cout" << endl;

  gLogStream <<
    tokenKindsSetAsString (msdlTokenKindsSet {msdlTokenKind::k_TokenEOF, msdlTokenKind::kTokenPlus}) <<
    endl;

  fMsdlTokensSetsStack.push_front (
    msdlTokenKindsSet {msdlTokenKind::k_TokenEOF, msdlTokenKind::kTokenPlus});
  fMsdlTokensSetsStack.push_front (
    msdlTokenKindsSet {msdlTokenKind::kTokenEndOfLine, msdlTokenKind::kTokenIdentifier, msdlTokenKind::kTokenInteger});

  displayTokenKindsSetsStack ("FII");

  msdlTokenKind myTokenKinds[] = {
      msdlTokenKind::kTokenTitle,
      msdlTokenKind::kTokenComposer,
      msdlTokenKind::kTokenOpus};
    };

  msdlTokenKindsSet myset (myTokenKinds, myTokenKinds + 2);

  cout << "myset contains:";
  for (
    msdlTokenKindsSet::iterator it=myset.begin();
    it!=myset.end();
    ++it
  ) {
    cout << ' ' << *it;
  } // for
  cout << endl;
*/

  // ignore separator tokens
  fIgnoreSeparatorTokensKind = kIgnoreSeparatorTokensYes;

  gGlobalMsdl2msdrOahGroup->
    setIgnoreSeparatorTokens (true); // JMI ???

  // fetch the first token
  fetchNextToken ();

  if (fCurrentTokenKind == msdlTokenKind::k_TokenEOF) {
#ifdef TRACING_IS_ENABLED
    gLogStream <<
      "Input doesn't contain any token" <<
      endl;
#endif
  }

  else {
    // let's go ahead

    ++gIndenter;

#ifdef TRACING_IS_ENABLED
    if (true || fTraceSyntax) {
      gLogStream <<
        "==> parse()" <<
        endl;
    }
#endif

    // do the parsing of the main non-terminal notions
    Specification (
      msdlTokenKindsSet::create ({
        msdlTokenKind::k_TokenEOF}));

    // are we at the and of the input?
    if (checkMandatoryTokenKind (msdlTokenKind::k_TokenEOF, "Specification")) {
#ifdef TRACING_IS_ENABLED
      if (true || fTraceSyntax) {
        gLogStream <<
          endl <<
          "<== parse()" <<
          ", msdlTokenKind::k_TokenEOF has been reached" <<
          endl << endl;
      }
#endif
    }

    if (fSourceIsSyntacticallyCorrect) {
      fParserWaeHandler->
        inputIsSyntacticallyCorrect ();
    }
    else {
      fParserWaeHandler->
        inputIsSyntacticallyIncorrect ();
    }

    --gIndenter;
  }
}

// --------------------------------------------------------------------------
//  msdlParser::Specification
// --------------------------------------------------------------------------

void msdlParser::Specification (S_msdlTokenKindsSet stopperTokensSet)
{
  fMsdlTokensSetsStack.push_front (stopperTokensSet);

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("Specification");
  }
#endif

  // the first token is already available

  // the optional Identification section
  S_msdlTokenKindsSet
    identificationFOLLOW =
      msdlTokenKindsSet::create ({
        msdlTokenKind::kTokenPitches,
        msdlTokenKind::kTokenAnacrusis});

  if (
    checkOptionalTokenKindsSet (
      fIdentificationFIRST,
      "Specification")
  ) {
    Identification (identificationFOLLOW);
  }

  // the optional Pitches section
  S_msdlTokenKindsSet
    pitchesFOLLOW =
      msdlTokenKindsSet::create ({
        msdlTokenKind::kTokenAnacrusis});

  if (
    checkOptionalTokenKind (
      msdlTokenKind::kTokenPitches,
      "Specification")
  ) {
	  Pitches (pitchesFOLLOW);
	}

  // the optional Anacrusis section
  // Anacrusis is the last
  // non-terminal notion in this production,
  // hence the empty FOLLOW set
  S_msdlTokenKindsSet
    anacrusisFOLLOW = fEmptyTokenKindsSet;

  if (
    checkOptionalTokenKind (
      msdlTokenKind::kTokenAnacrusis,
      "Specification")
  ) {
    Anacrusis (identificationFOLLOW);
  }

  // the mandatory Structure section
  if (
    checkMandatoryTokenKindsSet (
      fStructureFIRST,
      "Specification")
  ) {
    Structure (identificationFOLLOW);
  }

	fMsdlTokensSetsStack.pop_front ();
}

// --------------------------------------------------------------------------
//  msdlParser::Identification
// --------------------------------------------------------------------------

void msdlParser::Identification (S_msdlTokenKindsSet stopperTokensSet)
{
  fMsdlTokensSetsStack.push_front (stopperTokensSet);

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "--> Identification()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }

  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("Identification");
  }
#endif

  // did we handle the Identification already?
  if (fMsrIdentification) {
    fParserWaeHandler->
      multipleIdentifications ();
  }

  else {
    int inputLineNumber =
      fScanner.getCurrentLineNumber ();

    // create the MSR identification
    fMsrIdentification =
      msrIdentification::create (inputLineNumber);

    // append the MSDL compiler as software to it
    fMsrIdentification->
      msrIdentification::appendSoftware (
        inputLineNumber,
        "MSDL compiler");
  }

  while (
    checkOptionalTokenKindsSet (
      fIdentificationFIRST,
      "Identification")
  ) {
    // do the Identification parsing
    switch (fCurrentTokenKind) {
      case msdlTokenKind::kTokenTitle :
        Title (stopperTokensSet);
        break;
      case msdlTokenKind::kTokenComposer :
        Composer (stopperTokensSet);
        break;
      case msdlTokenKind::kTokenOpus :
        Opus (stopperTokensSet);
        break;
      default:
        ;
    } // switch
  } // while

	fMsdlTokensSetsStack.pop_front ();

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "<-- Identification()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Title
// --------------------------------------------------------------------------

void msdlParser::Title (S_msdlTokenKindsSet stopperTokensSet)
{
  fMsdlTokensSetsStack.push_front (stopperTokensSet);

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "--> Title()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }

  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("Title");
  }
#endif

  // consume the title token
  fetchNextToken ();

  if (checkMandatoryTokenKind (msdlTokenKind::kTokenString, "Title")) {
    // get the title
    string title = fCurrentToken.getTokenDescription ().getString ();

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      "=== parse()" <<
      ", title: \"" << title << "\"" <<
      endl;
  }
#endif

    // set the title in the MSR identification
    fMsrIdentification->setWorkTitle (
      fCurrentToken.getTokenLineNumber (),
      title);

    fetchNextToken ();
  }
  else {
//    fParserWaeHandler->stringExpectedAsTitle ();
  }

	fMsdlTokensSetsStack.pop_front ();

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      "<-- Title()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Composer
// --------------------------------------------------------------------------

void msdlParser::Composer (S_msdlTokenKindsSet stopperTokensSet)
{
  fMsdlTokensSetsStack.push_front (stopperTokensSet);

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "--> Composer()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }

  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("Composer");
  }
#endif

  // consume the composer token
  fetchNextToken ();

  if (checkMandatoryTokenKind (msdlTokenKind::kTokenString, "Composer")) {
    // get the composer
    string composer = fCurrentToken.getTokenDescription ().getString ();

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      "=== parse()" <<
      ", composer: \"" << composer << "\"" <<
      endl;
  }
#endif

    // append the composer to the MSR identification
    fMsrIdentification->appendComposer (
      fCurrentToken.getTokenLineNumber (),
      composer);

    fetchNextToken ();
  }
  else {
//    fParserWaeHandler->stringExpectedAsComposer ();
  }

	fMsdlTokensSetsStack.pop_front ();

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      "<-- Composer()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Opus
// --------------------------------------------------------------------------

void msdlParser::Opus (S_msdlTokenKindsSet stopperTokensSet)
{
  fMsdlTokensSetsStack.push_front (stopperTokensSet);

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "--> Opus()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }

  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("Opus");
  }
#endif

  // consume the opus token
  fetchNextToken ();

  if (checkMandatoryTokenKind (msdlTokenKind::kTokenString, "Opus")) {
    // get the opus
    string opus = fCurrentToken.getTokenDescription ().getString ();

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      "=== parse()" <<
      ", opus: \"" << opus << "\"" <<
      endl;
  }
#endif

    // set the opus in the MSR identification
    fMsrIdentification->setOpus (
      fCurrentToken.getTokenLineNumber (),
      opus);

    fetchNextToken ();
  }
  else {
//    fParserWaeHandler->stringExpectedAsOpus ();
  }

	fMsdlTokensSetsStack.pop_front ();

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      "<-- Opus()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Pitches
// --------------------------------------------------------------------------

void msdlParser::Pitches (S_msdlTokenKindsSet stopperTokensSet)
{
  fMsdlTokensSetsStack.push_front (stopperTokensSet);

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "--> Pitches()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }

  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("Pitches");
  }
#endif

  if (checkOptionalTokenKind (msdlTokenKind::kTokenPitches, "Pitches")) {
//  if (fCurrentTokenKind == msdlTokenKind::kTokenPitches) {
    // consume the pitches token
    fetchNextToken ();

    if (checkMandatoryTokenKind (msdlTokenKind::kTokenIdentifier, "Pitches")) {
      // get the pitches
      string pitches = fCurrentToken.getTokenDescription ().getString ();

#ifdef TRACING_IS_ENABLED
      if (true || fTraceSyntax) {
        gLogStream <<
          "=== parse()" <<
          ", pitches: \"" << pitches << "\"" <<
          endl;
      }
#endif

      // set the pitches in the MSR identification ??? JMI
      fPitchesLanguageKind =
        msdlPitchesLanguageKindFromString (
          pitches);

      fetchNextToken ();
    }
  }

	fMsdlTokensSetsStack.pop_front ();

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      "<-- Pitches()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Anacrusis
// --------------------------------------------------------------------------

void msdlParser::Anacrusis (S_msdlTokenKindsSet stopperTokensSet)
{
  fMsdlTokensSetsStack.push_front (stopperTokensSet);

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "--> Anacrusis()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }

  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("Anacrusis");
  }
#endif

  if (checkOptionalTokenKind (msdlTokenKind::kTokenAnacrusis, "Pitches")) {
//  if (fCurrentTokenKind == msdlTokenKind::kTokenAnacrusis) {
#ifdef TRACING_IS_ENABLED
    if (true || fTraceSyntax) {
      gLogStream <<
        "=== parse()" <<
        ", anacrusis: present" <<
        endl;
    }
#endif

    // consume the anacrusis token
    fetchNextToken ();
  }

	fMsdlTokensSetsStack.pop_front ();

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      "<-- Anacrusis()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Structure
// --------------------------------------------------------------------------

void msdlParser::Structure (S_msdlTokenKindsSet stopperTokensSet)
{
  fMsdlTokensSetsStack.push_front (stopperTokensSet);

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "--> Structure()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }

  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("Structure");
  }
#endif

  // did we handle the Identification already?
  if (fMsrIdentification) {
    fParserWaeHandler->
      multipleIdentifications ();
  }

  else {
    int inputLineNumber =
      fScanner.getCurrentLineNumber ();

    // create the MSR identification
    fMsrIdentification =
      msrIdentification::create (inputLineNumber);

    // append the MSDL compiler as software to it
    fMsrIdentification->
      msrIdentification::appendSoftware (
        inputLineNumber,
        "MSDL compiler");
  }

  // do the job
  while (
    checkOptionalTokenKindsSet (
      fStructureFIRST,
      "Structure")
  ) {
    // do the Identification parsing
    switch (fCurrentTokenKind) {
      case msdlTokenKind::kTokenBook :
        Book (stopperTokensSet);
        break;
      case msdlTokenKind::kTokenScore :
        Score (stopperTokensSet);
        break;
      case msdlTokenKind::kTokenPartGroup :
        PartGroup (stopperTokensSet);
        break;
      case msdlTokenKind::kTokenPart :
        Part (stopperTokensSet);
        break;
      case msdlTokenKind::kTokenMusic :
        Music (stopperTokensSet);
        break;
      default:
        ;
    } // switch
  } // while

	fMsdlTokensSetsStack.pop_front ();

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "<-- Structure()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Book
// --------------------------------------------------------------------------

void msdlParser::Book (S_msdlTokenKindsSet stopperTokensSet)
{
  fMsdlTokensSetsStack.push_front (stopperTokensSet);

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "--> Book()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }

  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("Book");
  }
#endif

  // did we handle the Identification already?
  if (fMsrIdentification) {
    fParserWaeHandler->
      multipleIdentifications ();
  }

  else {
    int inputLineNumber =
      fScanner.getCurrentLineNumber ();

    // create the MSR identification
    fMsrIdentification =
      msrIdentification::create (inputLineNumber);

    // append the MSDL compiler as software to it
    fMsrIdentification->
      msrIdentification::appendSoftware (
        inputLineNumber,
        "MSDL compiler");
  }

  // create the book if needed
  createBookIfNeeded (
    fCurrentToken.getTokenLineNumber ());

  // do the job
  while (
    checkOptionalTokenKindsSet (
      fBookFIRST,
      "Book")
  ) {
    // do the Identification parsing
    switch (fCurrentTokenKind) {
      case msdlTokenKind::kTokenScore :
        Score (stopperTokensSet);
        break;
      case msdlTokenKind::kTokenPartGroup :
        PartGroup (stopperTokensSet);
        break;
      case msdlTokenKind::kTokenPart :
        Part (stopperTokensSet);
        break;
      case msdlTokenKind::kTokenMusic :
        Music (stopperTokensSet);
        break;
      default:
        ;
    } // switch
  } // while

	fMsdlTokensSetsStack.pop_front ();

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "<-- Book()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Score
// --------------------------------------------------------------------------

void msdlParser::Score (S_msdlTokenKindsSet stopperTokensSet)
{
  fMsdlTokensSetsStack.push_front (stopperTokensSet);

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "--> Score()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }

  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("Score");
  }
#endif

  // did we handle the Identification already?
  if (fMsrIdentification) {
    fParserWaeHandler->
      multipleIdentifications ();
  }

  else {
    int inputLineNumber =
      fScanner.getCurrentLineNumber ();

    // create the MSR identification
    fMsrIdentification =
      msrIdentification::create (inputLineNumber);

    // append the MSDL compiler as software to it
    fMsrIdentification->
      msrIdentification::appendSoftware (
        inputLineNumber,
        "MSDL compiler");
  }

  // create the score if needed
  createScoreIfNeeded (
    fCurrentToken.getTokenLineNumber ());

  // do the job
  while (
    checkOptionalTokenKindsSet (
      fScoreFIRST,
      "Score")
  ) {
    // do the Identification parsing
    switch (fCurrentTokenKind) {
      case msdlTokenKind::kTokenPartGroup :
        PartGroup (stopperTokensSet);
        break;
      case msdlTokenKind::kTokenPart :
        Part (stopperTokensSet);
        break;
      case msdlTokenKind::kTokenMusic :
        Music (stopperTokensSet);
        break;
      default:
        ;
    } // switch
  } // while

	fMsdlTokensSetsStack.pop_front ();

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "<-- Score()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::PartGroup
// --------------------------------------------------------------------------

void msdlParser::PartGroup (S_msdlTokenKindsSet stopperTokensSet)
{
  fMsdlTokensSetsStack.push_front (stopperTokensSet);

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "--> PartGroup()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }

  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("PartGroup");
  }
#endif

  // did we handle the Identification already?
  if (fMsrIdentification) {
    fParserWaeHandler->
      multipleIdentifications ();
  }

  else {
    int inputLineNumber =
      fScanner.getCurrentLineNumber ();

    // create the MSR identification
    fMsrIdentification =
      msrIdentification::create (inputLineNumber);

    // append the MSDL compiler as software to it
    fMsrIdentification->
      msrIdentification::appendSoftware (
        inputLineNumber,
        "MSDL compiler");
  }

  // create the part group if needed
  createPartGroupIfNeeded (
    fCurrentToken.getTokenLineNumber ());

  // do the job
  while (
    checkOptionalTokenKindsSet (
      fPartGroupFIRST,
      "PartGroup")
  ) {
    // do the Identification parsing
    switch (fCurrentTokenKind) {
      case msdlTokenKind::kTokenPart :
        Part (stopperTokensSet);
        break;
      case msdlTokenKind::kTokenMusic :
        Music (stopperTokensSet);
        break;
      default:
        ;
    } // switch
  } // while

	fMsdlTokensSetsStack.pop_front ();

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "<-- PartGroup()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Part
// --------------------------------------------------------------------------

void msdlParser::Part (S_msdlTokenKindsSet stopperTokensSet)
{
  fMsdlTokensSetsStack.push_front (stopperTokensSet);

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "--> Part()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }

  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("Part");
  }
#endif

  // did we handle the Identification already?
  if (fMsrIdentification) {
    fParserWaeHandler->
      multipleIdentifications ();
  }

  else {
    int inputLineNumber =
      fScanner.getCurrentLineNumber ();

    // create the MSR identification
    fMsrIdentification =
      msrIdentification::create (inputLineNumber);

    // append the MSDL compiler as software to it
    fMsrIdentification->
      msrIdentification::appendSoftware (
        inputLineNumber,
        "MSDL compiler");
  }

  // create the part if needed
  createPartIfNeeded (
    fCurrentToken.getTokenLineNumber ());

  // do the job
  while (
    checkOptionalTokenKindsSet (
      fPartFIRST,
      "Part")
  ) {
    // do the Identification parsing
    switch (fCurrentTokenKind) {
      case msdlTokenKind::kTokenMusic :
        Music (stopperTokensSet);
        break;
      default:
        ;
    } // switch
  } // while

	fMsdlTokensSetsStack.pop_front ();

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "<-- Part()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Music
// --------------------------------------------------------------------------

void msdlParser::Music (S_msdlTokenKindsSet stopperTokensSet)
{
  fMsdlTokensSetsStack.push_front (stopperTokensSet);

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "--> Music()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }

  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("Music");
  }
#endif

  // consume the music token
  fetchNextToken ();

  if (checkOptionalTokenKind (msdlTokenKind::kTokenMeasure, "Music")) {
//  if (fCurrentTokenKind == msdlTokenKind::kTokenMusic) {
    MeasuresSequence (stopperTokensSet);

    if (checkMandatoryTokenKind (msdlTokenKind::kTokenIdentifier, "Music")) {
      // get the music name
      string musicName = fCurrentToken.getTokenDescription ().getString ();

#ifdef TRACING_IS_ENABLED
      if (true || fTraceSyntax) {
        gLogStream <<
          "=== parse()" <<
          ", Music: \"" << musicName << "\"" <<
          endl;
      }
#endif

/*
      // set the music in the MSR identification ??? JMI
      fMusicLanguageKind =
        msdlMusicLanguageKindFromString (
          musicName);

      fetchNextToken ();
*/    }
  }

	fMsdlTokensSetsStack.pop_front ();

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      "<-- Music()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Fragment
// --------------------------------------------------------------------------

void msdlParser::Fragment (S_msdlTokenKindsSet stopperTokensSet)
{
  fMsdlTokensSetsStack.push_front (stopperTokensSet);

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "--> Fragment()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }

  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("Fragment");
  }
#endif



	fMsdlTokensSetsStack.pop_front ();

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      "<-- Fragment()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::MeasuresSequence
// --------------------------------------------------------------------------

void msdlParser::MeasuresSequence (S_msdlTokenKindsSet stopperTokensSet)
{
  fMsdlTokensSetsStack.push_front (stopperTokensSet);

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "--> MeasuresSequence()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }

  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("MeasuresSequence");
  }
#endif

  while (checkMandatoryTokenKind (msdlTokenKind::kTokenMeasure, "MeasuresSequence")) {
    Measure (stopperTokensSet);
  }

	fMsdlTokensSetsStack.pop_front ();

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      "<-- MeasuresSequence()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Measure
// --------------------------------------------------------------------------

void msdlParser::Measure (S_msdlTokenKindsSet stopperTokensSet)
{
  fMsdlTokensSetsStack.push_front (stopperTokensSet);

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "--> Measure()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }

  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("Measure");
  }
#endif

  if (checkMandatoryTokenKind (msdlTokenKind::kTokenMeasure, "Measure")) {
    MeasureNumber (stopperTokensSet);
  }

	fMsdlTokensSetsStack.pop_front ();

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      "<-- Measure()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::MeasureNumber
// --------------------------------------------------------------------------

void msdlParser::MeasureNumber (S_msdlTokenKindsSet stopperTokensSet)
{
  fMsdlTokensSetsStack.push_front (stopperTokensSet);

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "--> MeasureNumber()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }

  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("MeasureNumber");
  }
#endif

  // consume the measure token
  fetchNextToken ();

  if (checkMandatoryTokenKind (msdlTokenKind::kTokenInteger, "MeasureNumber")) {
//  if (fCurrentTokenKind == msdlTokenKind::kTokenInteger) {
    // consume the left parenthesis token

    // get the measure number
    int measureNumber = fCurrentToken.getTokenDescription ().getInteger ();

#ifdef TRACING_IS_ENABLED
    if (true || fTraceSyntax) {
      gLogStream <<
        "=== parse()" <<
        ", measureNumber: \"" << measureNumber << "\"" <<
        endl;
    }
#endif

    // consume the measure integer token
    fetchNextToken ();
  }

	fMsdlTokensSetsStack.pop_front ();

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      "<-- MeasureNumber()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Clef
// --------------------------------------------------------------------------

void msdlParser::Clef (S_msdlTokenKindsSet stopperTokensSet)
{
  fMsdlTokensSetsStack.push_front (stopperTokensSet);

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "--> Clef()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }

  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("Clef");
  }
#endif



	fMsdlTokensSetsStack.pop_front ();

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      "<-- Clef()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Key
// --------------------------------------------------------------------------

void msdlParser::Key (S_msdlTokenKindsSet stopperTokensSet)
{
  fMsdlTokensSetsStack.push_front (stopperTokensSet);

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "--> Key()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }

  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("Key");
  }
#endif



	fMsdlTokensSetsStack.pop_front ();

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      "<-- Key()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Time
// --------------------------------------------------------------------------

void msdlParser::Time (S_msdlTokenKindsSet stopperTokensSet)
{
  fMsdlTokensSetsStack.push_front (stopperTokensSet);

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "--> Time()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }

  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("Time");
  }
#endif



	fMsdlTokensSetsStack.pop_front ();

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      "<-- Time()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Pitch
// --------------------------------------------------------------------------

void msdlParser::Pitch (S_msdlTokenKindsSet stopperTokensSet)
{
  fMsdlTokensSetsStack.push_front (stopperTokensSet);

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      endl <<
      "--> Pitch()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }

  if (fTraceSyntaxDetails) {
    displayTokenKindsSetsStack ("Anacrusis");
  }
#endif

  // there should be an indentifier
  if (checkMandatoryTokenKind (msdlTokenKind::kTokenIdentifier, "Pitch")) {
      // get the title
    string pitchName = fCurrentToken.getTokenDescription ().getString ();

//  if (fCurrentTokenKind == msdlTokenKind::kTokenAnacrusis) {
#ifdef TRACING_IS_ENABLED
    if (true || fTraceSyntax) {
      gLogStream <<
        "=== parse()" <<
        ", pitch name: \"" << pitchName << "\"" <<
        endl;
    }
#endif

    // consume the pitch indentifier
    fetchNextToken ();
  }

  // there can be commas and single quotes
  S_msdlTokenKindsSet
    octaveIndicationFIRST =
      msdlTokenKindsSet::create({
        msdlTokenKind::kTokenComma,
        msdlTokenKind::kTokenQuote});

  int commasCounter = 0;
  int quotesCounter = 0;

  while (
    checkOptionalTokenKindsSet (
      octaveIndicationFIRST,
      "Identification")
  ) {
    // do the Identification parsing
    switch (fCurrentTokenKind) {
      case msdlTokenKind::kTokenComma :
        ++commasCounter;
        break;
      case msdlTokenKind::kTokenQuote :
        ++quotesCounter;
        break;
      default:
        ;
    } // switch
  } // while

#ifdef TRACING_IS_ENABLED
    if (true || fTraceSyntax) {
      gLogStream <<
        "=== parse()" <<
        ", commasCounter: " << commasCounter <<
        ", quotesCounter: " << quotesCounter <<
        endl;
    }
#endif

  // sanity check
  if (commasCounter > 0 && quotesCounter > 0) {
    // error
  }

	fMsdlTokensSetsStack.pop_front ();

#ifdef TRACING_IS_ENABLED
  if (true || fTraceSyntax) {
    gLogStream <<
      "<-- Pitch()" <<
      ", fCurrentToken: " << currentTokenAsMsdlString () <<
      endl;
  }
#endif
}


}
