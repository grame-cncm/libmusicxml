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

#include "version.h"

#include "generalOah.h"

#include "msdlParserWaeHandlers.h"

#include "utilities.h"

#include "msdlParser.h"

#include "waeExceptions.h"

#include "msdl2msrOah.h"


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
  fTraceSyntax        =
    gGlobalMsdl2msrOahGroup->getTraceSyntax ();
  fTraceSyntaxDetails =
    gGlobalMsdl2msrOahGroup->getTraceSyntaxDetails ();

  fTraceSyntaxErrorRecovery =
    gGlobalMsdl2msrOahGroup->getTraceSyntaxErrorRecovery ();

  fTraceSyntaxErrorRecoveryDetails =
    gGlobalMsdl2msrOahGroup->getTraceSyntaxErrorRecoveryDetails ();
#endif

  // user language
  fUserLanguageKind =
    gGlobalMsdl2msrOahGroup->
      getMsdlUserLanguageKind ();

  // keywords language
  fKeywordsInputLanguageKind =
    gGlobalMsdl2msrOahGroup->
      getMsdlKeywordsInputLanguageKind ();

  // pitches language
  fPitchesLanguageKind =
    gGlobalMsrOahGroup->
      getMsrQuarterTonesPitchesLanguageKind ();

  // input source name
  fInputSourceName = inputSourceName;

  // warnings and errors
  switch (gGlobalMsdl2msrOahGroup->getMsdlUserLanguageKind ()) {
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

  // the empty token kinds set
  fEmptyTokenKindsSet =
    msdlTokenKindsSet::create ();;

  // the token kinds set containing only f_TokenEOF
  fTokenEOFTokenKindsSet =
    msdlTokenKindsSet::create ({
      msdlTokenKind::k_TokenEOF});

 // Note
  fNoteFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenName});

  fNoteFOLLOW =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenMeasure});

  fPitchFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenName});

  fOctaveIndicationFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenComma,
      msdlTokenKind::kTokenQuote});

  fNoteDurationFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenInteger,
      msdlTokenKind::kTokenName});

  // Measure
  fMeasureFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenMeasure});

  fMeasureFOLLOW =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenRightBracket});

  // MeasuresSequence
  fMeasuresSequenceFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenMeasure});

  fMeasuresSequenceFOLLOW =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenRightBracket});

  // Music
  fMusicFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenMusic});

  // Part
  fPartFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenPart,
      msdlTokenKind::kTokenMusic});
  fPartFOLLOW = fTokenEOFTokenKindsSet;

  // PartGroup
  fPartGroupFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenPartGroup,
      msdlTokenKind::kTokenPart,
      msdlTokenKind::kTokenMusic});
  fPartGroupFOLLOW = fTokenEOFTokenKindsSet;

  // Score
  fScoreFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenScore,
      msdlTokenKind::kTokenPartGroup,
      msdlTokenKind::kTokenPart,
      msdlTokenKind::kTokenMusic});
  fScoreFOLLOW = fTokenEOFTokenKindsSet;

  // Book
  fBookFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenBook,
      msdlTokenKind::kTokenScore,
      msdlTokenKind::kTokenPartGroup,
      msdlTokenKind::kTokenPart,
      msdlTokenKind::kTokenMusic});
  fBookFOLLOW = fTokenEOFTokenKindsSet;

  // Structure
  fStructureFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenBook,
      msdlTokenKind::kTokenScore,
      msdlTokenKind::kTokenPartGroup,
      msdlTokenKind::kTokenPart,
      msdlTokenKind::kTokenMusic});

  fStructureFOLLOW = fEmptyTokenKindsSet;

  // Identification
  fIdentificationFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenTitle,
      msdlTokenKind::kTokenComposer,
      msdlTokenKind::kTokenOpus});

  fIdentificationFOLLOW =
    fStructureFIRST
      +
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenPitches});
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
    " [" << context << "]" <<
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
  if (fTraceSyntaxErrorRecoveryDetails) {
    gLogStream <<
      endl <<
      "-->isCurrentTokenKindInSetsStack()" <<
      context << ": tokens stack contents (" <<
      singularOrPlural (
        tokensSetsStackSize, "set", "sets") <<
      "):" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      " [" << context << "]" <<
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
      if (fTraceSyntaxErrorRecoveryDetails) {
        gLogStream <<
          endl <<
          "-->isCurrentTokenKindInSetsStack()" <<
          ", fCurrentToken: " << currentTokenAsString () <<
          " [" << context << "]" <<
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
  string        fileName,
  int           lineNumber,
  msdlTokenKind tokenKind,
  string        context)
{
#ifdef TRACING_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    gLogStream <<
      endl <<
      "-->checkMandatoryTokenKind()" <<
      ", @" << baseName (fileName) << ":" << lineNumber <<
      ", tokenKind: " << msdlTokenKindAsString (tokenKind) <<
      " [" << context << "]" <<
      endl;
  }
#endif

  bool result = false;

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
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

      while (true) {
        // let's ignore fCurrentToken
#ifdef TRACING_IS_ENABLED
        if (fTraceSyntaxErrorRecovery) {
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
  if (fTraceSyntaxErrorRecoveryDetails) {
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
  string              fileName,
  int                 lineNumber,
  S_msdlTokenKindsSet tokenKindsSet,
  string              context)
{
#ifdef TRACING_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    gLogStream <<
      endl <<
      "-->checkMandatoryTokenKindsSet()" <<
      ", @" << baseName (fileName) << ":" << lineNumber <<
      ", context: [" << context << "]" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      ", tokenKindsSet: " << tokenKindsSet->asString () <<
      endl;
  }
#endif

  bool result = false;

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
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
        if (fTraceSyntaxErrorRecovery) {
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
  if (fTraceSyntaxErrorRecoveryDetails) {
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
  string        fileName,
  int           lineNumber,
  msdlTokenKind tokenKind,
  string        context)
{
#ifdef TRACING_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    gLogStream <<
      endl <<
      "-->checkOptionalTokenKind()" <<
      ", @" << baseName (fileName) << ":" << lineNumber <<
      ", tokenKind: " << msdlTokenKindAsString (tokenKind) <<
  //    " context: " << context <<
      endl;
  }
#endif

  bool result = false;

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
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

    else if (false) { // JMI
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
        if (fTraceSyntaxErrorRecovery) {
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
  if (fTraceSyntaxErrorRecoveryDetails) {
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
  string              fileName,
  int                 lineNumber,
  S_msdlTokenKindsSet tokenKindsSet,
  string              context)
{
#ifdef TRACING_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    gLogStream <<
      endl <<
      "-->checkOptionalTokenKindsSet()" <<
      ", @" << baseName (fileName) << ":" << lineNumber <<
      ", context: [" << context << "]" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      ", tokenKindsSet: " << tokenKindsSet->asString () <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
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
          if (fTraceSyntaxErrorRecovery) {
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
  if (fTraceSyntaxErrorRecoveryDetails) {
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

  s << measureNumber;

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

void msdlParser::createIdentificationIfNeeded (int inputLineNumber)
{
  // did we handle the Identification already?
  if (! fMsrIdentification) {
/* JMI
    fParserWaeHandler->
      multipleIdentifications ();
*/

    int inputLineNumber =
      fScanner.getCurrentLineNumber ();

    // create the MSR identification
    fMsrIdentification =
      msrIdentification::create (inputLineNumber);

    // append the MSDL compiler as software to it
    string
      software =
        "MSDL compiler"
          +
        currentVersionNumber ();

    fMsrIdentification->
      msrIdentification::appendSoftware (
        inputLineNumber,
        software);

    // set the encoding date in it
    string
      translationDate =
        gGlobalGeneralOahGroup->getTranslationDateFull ();

#ifdef TRACING_IS_ENABLED
    if (fTraceSyntax) {
      gLogStream <<
        "--- createIdentificationIfNeeded()" <<
        ", translationDate: " << translationDate <<
        endl;
    }
#endif

    fMsrIdentification->
      setEncodingDate (
        inputLineNumber,
        translationDate);
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
  // ignore separator tokens
  fIgnoreSeparatorTokensKind = kIgnoreSeparatorTokensYes;

  gGlobalMsdl2msrOahGroup->
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
    if (fTraceSyntax) {
      gLogStream <<
        "==> parse()" <<
        endl;
    }
#endif

    // do the parsing of the main non-terminal notions
    Specification (
      fTokenEOFTokenKindsSet);

    // are we at the and of the input?
    if (checkMandatoryTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::k_TokenEOF,
      "Specification")
    ) {
#ifdef TRACING_IS_ENABLED
      if (fTraceSyntax) {
        gLogStream <<
          endl <<
          "<== parse()" <<
          ", msdlTokenKind::k_TokenEOF has been reached" <<
          endl << endl;
      }
#endif
    }

#ifdef TRACING_IS_ENABLED
    if (fTraceSyntax) {
      if (fSourceIsSyntacticallyCorrect) {
        fParserWaeHandler->
          inputIsSyntacticallyCorrect ();
      }
      else {
        fParserWaeHandler->
          inputIsSyntacticallyIncorrect ();
      }
    }
#endif

    // should we display the MSR score?
    if (gGlobalMsrOahGroup->getDisplayMsr ()) {
      gLogStream <<
        endl <<
        "*** Built MSR score: ***" <<
        endl <<
        "========================" <<
        endl << endl;

      ++gIndenter;

      if (fCurrentScore) {
        gLogStream <<
          fCurrentScore <<
          endl;
      }
      else {
        gLogStream << "NONE" << endl;
      }

      --gIndenter;
    }

    --gIndenter;
  }
}

// --------------------------------------------------------------------------
//  msdlParser::Specification
// --------------------------------------------------------------------------

void msdlParser::Specification (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Specification");
  }
#endif

  ++gIndenter;

  // the first token is already available

  // the optional Identification section
  if (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fIdentificationFIRST,
      "Specification")
  ) {
    Identification (
      fStructureFIRST
        +
      msdlTokenKindsSet::create ({
        msdlTokenKind::kTokenPitches,
        msdlTokenKind::kTokenAnacrusis}));
  }

  // the optional PitchesLanguage section
  if (
    checkOptionalTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::kTokenPitches,
      "Specification")
  ) {
	  PitchesLanguage (
      fStructureFIRST
        +
      msdlTokenKindsSet::create ({
        msdlTokenKind::kTokenAnacrusis}));
	}

  // the optional Anacrusis section
  if (
    checkOptionalTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::kTokenAnacrusis,
      "Specification")
  ) {
    Anacrusis (
      fStructureFIRST);
  }

  // the mandatory Structure section
  if (
    checkMandatoryTokenKindsSet (
      __FILE__, __LINE__,
      fStructureFIRST,
      "Specification")
  ) {
    Structure (
      fEmptyTokenKindsSet);
  }

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }
}

// --------------------------------------------------------------------------
//  msdlParser::Identification
// --------------------------------------------------------------------------

void msdlParser::Identification (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "=================================================================" <<
      endl <<
      "--> Identification()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Identification");
  }
#endif

  ++gIndenter;

  createIdentificationIfNeeded (
    fCurrentToken.getTokenLineNumber ());

  // there can be a title, a composer and an opus
  while (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fIdentificationFIRST,
      "Identification")
  ) {
    // do the Identification parsing
    switch (fCurrentTokenKind) {
      case msdlTokenKind::kTokenTitle :
        Title (fIdentificationFOLLOW);
        break;
      case msdlTokenKind::kTokenComposer :
        Composer (fIdentificationFOLLOW);
        break;
      case msdlTokenKind::kTokenOpus :
        Opus (fIdentificationFOLLOW);
        break;
      default:
        ;
    } // switch
  } // while

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "<-- Identification()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl <<
      "=================================================================" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Title
// --------------------------------------------------------------------------

void msdlParser::Title (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "=================================================================" <<
      endl <<
      "--> Title()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Title");
  }
#endif

  ++gIndenter;

  // consume the title token
  fetchNextToken ();

  if (checkMandatoryTokenKind (
    __FILE__, __LINE__,
    msdlTokenKind::kTokenString,
    "Title")
  ) {
    // get the title
    string title = fCurrentToken.getTokenDescription ().getString ();

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
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

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "<-- Title()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl <<
      "=================================================================" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Composer
// --------------------------------------------------------------------------

void msdlParser::Composer (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "=================================================================" <<
      endl <<
      "--> Composer()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Composer");
  }
#endif

  ++gIndenter;

  // consume the composer token
  fetchNextToken ();

  if (checkMandatoryTokenKind (
    __FILE__, __LINE__,
    msdlTokenKind::kTokenString,
    "Composer")
  ) {
    // get the composer
    string composer = fCurrentToken.getTokenDescription ().getString ();

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
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

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "=================================================================" <<
      endl <<
      "<-- Composer()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Opus
// --------------------------------------------------------------------------

void msdlParser::Opus (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "--> Opus()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl <<
      "=================================================================" <<
      endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Opus");
  }
#endif

  ++gIndenter;

  // consume the opus token
  fetchNextToken ();

  if (checkMandatoryTokenKind (
    __FILE__, __LINE__,
    msdlTokenKind::kTokenString,
    "Opus")
  ) {
    // get the opus
    string opus = fCurrentToken.getTokenDescription ().getString ();

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
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

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "<-- Opus()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl <<
      "=================================================================" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::PitchesLanguage
// --------------------------------------------------------------------------

void msdlParser::PitchesLanguage (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "=================================================================" <<
      endl <<
      "--> PitchesLanguage()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }

  ++gIndenter;

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("PitchesLanguage");
  }
#endif

  if (
    checkOptionalTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::kTokenPitches,
      "PitchesLanguage")
  ) {
//  if (fCurrentTokenKind == msdlTokenKind::kTokenPitches) {
    // consume the pitches token
    fetchNextToken ();

    if (checkMandatoryTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::kTokenName,
      "PitchesLanguage")
    ) {
      // get the pitches language name
      string pitchesLanguageName = fCurrentToken.getTokenDescription ().getString ();

#ifdef TRACING_IS_ENABLED
      if (fTraceSyntax) {
        gLogStream <<
          "=== parse()" <<
          ", pitchesLanguageName: \"" << pitchesLanguageName << "\"" <<
          endl;
      }
#endif

      // set the pitches language name // also in the MSR identification ??? JMI
      fPitchesLanguageKind =
        msrQuarterTonesPitchesLanguageKindFromString (
          pitchesLanguageName);

#ifdef TRACING_IS_ENABLED
      if (fTraceSyntax) {
        gLogStream <<
          "=== parse()" <<
          ", fPitchesLanguageKind: \"" <<
          msrQuarterTonesPitchesLanguageKindAsString (fPitchesLanguageKind) <<
          "\"" <<
          endl;
      }
#endif

      fetchNextToken ();
    }
  }

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "<-- PitchesLanguage()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl <<
      "=================================================================" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Anacrusis
// --------------------------------------------------------------------------

void msdlParser::Anacrusis (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "=================================================================" <<
      endl <<
      "--> Anacrusis()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }

  ++gIndenter;

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Anacrusis");
  }
#endif

  if (
    checkOptionalTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::kTokenAnacrusis,
      "Anacrusis")
  ) {
//  if (fCurrentTokenKind == msdlTokenKind::kTokenAnacrusis) {
#ifdef TRACING_IS_ENABLED
    if (fTraceSyntax) {
      gLogStream <<
        "=== parse()" <<
        ", anacrusis: present" <<
        endl;
    }
#endif

    // consume the anacrusis token
    fetchNextToken ();
  }

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "<-- Anacrusis()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl <<
      "=================================================================" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Structure
// --------------------------------------------------------------------------

void msdlParser::Structure (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "=================================================================" <<
      endl <<
      "--> Structure()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Structure");
  }
#endif

  ++gIndenter;

  // do the job
  while (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fStructureFIRST,
      "Structure")
  ) {
    // do the Identification parsing
    switch (fCurrentTokenKind) {
      case msdlTokenKind::kTokenBook :
        Book (fStructureFOLLOW);
        break;
      case msdlTokenKind::kTokenScore :
        Score (fStructureFOLLOW);
        break;
      case msdlTokenKind::kTokenPartGroup :
        PartGroup (fStructureFOLLOW);
        break;
      case msdlTokenKind::kTokenPart :
        Part (fStructureFOLLOW);
        break;
      case msdlTokenKind::kTokenMusic :
        Music (fStructureFOLLOW);
        break;
      default:
        ;
    } // switch
  } // while

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "<-- Structure()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl <<
      "=================================================================" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Book
// --------------------------------------------------------------------------

void msdlParser::Book (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "=================================================================" <<
      endl <<
      "--> Book()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Book");
  }
#endif

  ++gIndenter;

  // create the book if needed
  createBookIfNeeded (
    fCurrentToken.getTokenLineNumber ());

  // do the job
  while (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
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

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "<-- Book()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl <<
      "=================================================================" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Score
// --------------------------------------------------------------------------

void msdlParser::Score (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "=================================================================" <<
      endl <<
      "--> Score()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Score");
  }
#endif

  ++gIndenter;

  // create the score if needed
  createScoreIfNeeded (
    fCurrentToken.getTokenLineNumber ());

  // do the job
  while (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
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

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "<-- Score()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl <<
      "=================================================================" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::PartGroup
// --------------------------------------------------------------------------

void msdlParser::PartGroup (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "=================================================================" <<
      endl <<
      "--> PartGroup()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("PartGroup");
  }
#endif

  ++gIndenter;

  // create the part group if needed
  createPartGroupIfNeeded (
    fCurrentToken.getTokenLineNumber ());

  // do the job
  while (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
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

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "<-- PartGroup()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl <<
      "=================================================================" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Part
// --------------------------------------------------------------------------

void msdlParser::Part (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "=================================================================" <<
      endl <<
      "--> Part()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Part");
  }
#endif

  ++gIndenter;

  // create the part if needed
  createPartIfNeeded (
    fCurrentToken.getTokenLineNumber ());

  // do the job
  while (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
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

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "<-- Part()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl <<
      "=================================================================" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Music
// --------------------------------------------------------------------------

void msdlParser::Music (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "=================================================================" <<
      endl <<
      "--> Music()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Music");
  }
#endif

  ++gIndenter;

  // consume the music token
  fetchNextToken ();

/* JMI
  // there can be a music name
  if (
    checkOptionalTokenKind (
      msdlTokenKind::kTokenName,
      "Music")
  ) {
    // get the music name
    string musicName = fCurrentToken.getTokenDescription ().getString ();

#ifdef TRACING_IS_ENABLED
    if (fTraceSyntax) {
      gLogStream <<
        "=== parse()" <<
        ", Music: \"" << musicName << "\"" <<
        endl;
    }
#endif
  }
*/

  // there should be a left bracket
  if (checkMandatoryTokenKind (
    __FILE__, __LINE__,
    msdlTokenKind::kTokenLeftBracket,
    "Music")
  ) {
    fetchNextToken ();
  }

  // there can be measures
  if (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fMeasuresSequenceFIRST,
      "Music")
  ) {
    MeasuresSequence (
      fMeasuresSequenceFOLLOW);
  }

  // there should be a right bracket
  if (checkMandatoryTokenKind (
    __FILE__, __LINE__,
    msdlTokenKind::kTokenRightBracket,
    "Music")
  ) {
    fetchNextToken ();
  }

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "=================================================================" <<
      endl <<
      "<-- Music()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Fragment
// --------------------------------------------------------------------------

void msdlParser::Fragment (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "=================================================================" <<
      endl <<
      "--> Fragment()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Fragment");
  }
#endif

  ++gIndenter;

  // consume the fragment token
  fetchNextToken ();

  // there should be a left bracket
  if (checkMandatoryTokenKind (
    __FILE__, __LINE__,
    msdlTokenKind::kTokenLeftBracket,
    "Fragment")
  ) {
    fetchNextToken ();
  }

  if (
    checkOptionalTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::kTokenMeasure,
      "Fragment")
  ) {
    MeasuresSequence (
      fMeasuresSequenceFOLLOW);

    if (checkMandatoryTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::kTokenName,
      "Fragment")
    ) {
      // get the fragment name
      string fragmentName = fCurrentToken.getTokenDescription ().getString ();

#ifdef TRACING_IS_ENABLED
      if (fTraceSyntax) {
        gLogStream <<
          "=== parse()" <<
          ", Fragment: \"" << fragmentName << "\"" <<
          endl;
      }
#endif

/*
      // set the fragment in the MSR identification ??? JMI
      fMusicLanguageKind =
        msdlMusicLanguageKindFromString (
          musicName);

      fetchNextToken ();
*/    }
  }

  // there should be a right bracket
  if (checkMandatoryTokenKind (
    __FILE__, __LINE__,
    msdlTokenKind::kTokenRightBracket,
    "Fragment")
  ) {
    fetchNextToken ();
  }

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "<-- Fragment()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl <<
      "=================================================================" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::MeasuresSequence
// --------------------------------------------------------------------------

void msdlParser::MeasuresSequence (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "=================================================================" <<
      endl <<
      "--> MeasuresSequence()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("MeasuresSequence");
  }
#endif

  ++gIndenter;

  while (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fMeasuresSequenceFIRST,
      "MeasuresSequence")
  ) {
    Measure (
      fMeasureFOLLOW);
  } // while

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "<-- MeasuresSequence()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl <<
      "=================================================================" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Measure
// --------------------------------------------------------------------------

void msdlParser::Measure (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "=================================================================" <<
      endl <<
      "--> Measure()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Measure");
  }
#endif

  ++gIndenter;

  // there should be a '|'
  if (checkMandatoryTokenKind (
    __FILE__, __LINE__,
    msdlTokenKind::kTokenMeasure,
    "Measure")
  ) {
    MeasureNumber (stopperTokensSet);
  }

  // accept a sequence of notes and bars
  while (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fNoteFIRST
        +
      msdlTokenKind::kTokenFinalBar,
      "Measure")
  ) {
    if (fCurrentTokenKind == msdlTokenKind::kTokenFinalBar) {
      // consume final bar
      fetchNextToken ();
    }
    else {
      Note (
        fNoteFOLLOW);
    }
  } // while

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "<-- Measure()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl <<
      "=================================================================" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::MeasureNumber
// --------------------------------------------------------------------------

void msdlParser::MeasureNumber (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "=================================================================" <<
      endl <<
      "--> MeasureNumber()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("MeasureNumber");
  }
#endif

  ++gIndenter;

  // consume the measure token
  fetchNextToken ();

  int measureNumber = 1; // TEMP JMI

  if (checkMandatoryTokenKind (
    __FILE__, __LINE__,
    msdlTokenKind::kTokenInteger,
    "MeasureNumber")
  ) {
//  if (fCurrentTokenKind == msdlTokenKind::kTokenInteger) {
    // consume the left parenthesis token

    // get the measure number
    measureNumber = fCurrentToken.getTokenDescription ().getInteger ();

#ifdef TRACING_IS_ENABLED
    if (fTraceSyntax) {
      gLogStream <<
        "=== parse()" <<
        ", measureNumber: \"" << measureNumber << "\"" <<
        endl;
    }
#endif

    // consume the measure integer token
    fetchNextToken ();
  }

  // create a measure
  createMeasureNumber (
    fCurrentToken.getTokenLineNumber (),
    measureNumber); // JMI ++fCurrentMeasureNumber);

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "<-- MeasureNumber()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl <<
      "=================================================================" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Clef
// --------------------------------------------------------------------------

void msdlParser::Clef (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "=================================================================" <<
      endl <<
      "--> Clef()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Clef");
  }
#endif

  ++gIndenter;



  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "<-- Clef()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl <<
      "=================================================================" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Key
// --------------------------------------------------------------------------

void msdlParser::Key (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "=================================================================" <<
      endl <<
      "--> Key()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Key");
  }
#endif

  ++gIndenter;



  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "<-- Key()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl <<
      "=================================================================" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Time
// --------------------------------------------------------------------------

void msdlParser::Time (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "=================================================================" <<
      endl <<
      "--> Time()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Time");
  }
#endif

  ++gIndenter;



  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "<-- Time()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl <<
      "=================================================================" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Note
// --------------------------------------------------------------------------

void msdlParser::Note (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "=================================================================" <<
      endl <<
      "--> Note()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Note");
  }
#endif

  ++gIndenter;

  int inputLineNumber =
    fCurrentToken.getTokenLineNumber ();

  // there should be a pitch name
  msrQuarterTonesPitchKind
    noteQuarterTonesPitchKind = k_NoQuarterTonesPitch_QTP;

  if (checkMandatoryTokenKind (
    __FILE__, __LINE__,
    msdlTokenKind::kTokenName,
    "Note")
  ) {
    // get the title
    string pitchName = fCurrentToken.getTokenDescription ().getString ();

#ifdef TRACING_IS_ENABLED
    if (fTraceSyntax) {
      gLogStream <<
        "=== parse()" <<
        ", pitch name: \"" << pitchName << "\"" <<
        endl;
    }
#endif

/* JMI
    noteQuarterTonesPitchKind =
      quarterTonesPitchKindFromString (
        fPitchesLanguageKind,
        pitchName);
*/

#ifdef TRACING_IS_ENABLED
    if (fTraceSyntax) {
      gLogStream <<
        "=== parse()" <<
        ", note quarter tones pitch kind: \"" <<
        msrQuarterTonesPitchKindAsString (noteQuarterTonesPitchKind) <<
        "\"" <<
        endl;
    }
#endif

    // consume the pitch indentifier
    fetchNextToken ();
  }

  // is there an octave indication?
  msrOctaveKind
    noteOctaveKind = msrOctaveKind::k_NoOctave;

  if (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fOctaveIndicationFIRST,
      "Note")
  ) {
    noteOctaveKind =
      OctaveIndication (
        stopperTokensSet);
  }

  // are there dots?
  msrDottedDuration noteDottedDuration;
  int               dotsNumber = 0;

  if (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fNoteDurationFIRST,
      "Note")
  ) {
    NoteDuration (
      noteDottedDuration,
      dotsNumber,
      stopperTokensSet);
  }

  // create the note
  stringstream s;

  s << fCurrentMeasureNumber;

  string currentMeasureNumberAsString = s.str ();

  rational
    noteSoundingWholeNotes =
      noteDottedDuration.dottedDurationAsWholeNotes (
        inputLineNumber),
    noteDisplayWholeNotes =
      noteDottedDuration.dottedDurationAsWholeNotes (
        inputLineNumber);

  S_msrNote
    note =
      msrNote::createRegularNote (
        __LINE__,
        currentMeasureNumberAsString,
        noteQuarterTonesPitchKind,
        noteOctaveKind,
        noteSoundingWholeNotes, // soundingWholeNotes
        noteDisplayWholeNotes , // displayWholeNotes
        dotsNumber);

  // append it to the current measure
  fCurrentMeasure->
    appendNoteOrPaddingToMeasure (note);

 --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "<-- Note()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl <<
      "=================================================================" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Pitch
// --------------------------------------------------------------------------

void msdlParser::Pitch (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "=================================================================" <<
      endl <<
      "--> Pitch()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Pitch");
  }
#endif

  ++gIndenter;

  // there should be an indentifier
  if (checkMandatoryTokenKind (
    __FILE__, __LINE__,
    msdlTokenKind::kTokenName,
    "Pitch")
  ) {
    // get the title
    string pitchName = fCurrentToken.getTokenDescription ().getString ();

#ifdef TRACING_IS_ENABLED
    if (fTraceSyntax) {
      gLogStream <<
        "=== parse()" <<
        ", pitch name: \"" << pitchName << "\"" <<
        endl;
    }
#endif

    // consume the pitch indentifier
    fetchNextToken ();
  }

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "<-- Pitch()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl <<
      "=================================================================" <<
      endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::OctaveIndication
// --------------------------------------------------------------------------

msrOctaveKind msdlParser::OctaveIndication (S_msdlTokenKindsSet stopperTokensSet)
{
  msrOctaveKind result = msrOctaveKind::k_NoOctave;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "=================================================================" <<
      endl <<
      "--> OctaveIndication()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("OctaveIndication");
  }
#endif

  ++gIndenter;

  // there can be commas and single quotes
  int commasCounter = 0;
  int quotesCounter = 0;

  while (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fOctaveIndicationFIRST,
      "OctaveIndication")
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

    // consume the comma or quote
    fetchNextToken ();
  } // while

#ifdef TRACING_IS_ENABLED
    if (fTraceSyntax) {
      gLogStream <<
        "=== parse()" <<
        ", commasCounter: " << commasCounter <<
        ", quotesCounter: " << quotesCounter <<
        endl;
    }
#endif

  // sanity check
  if (commasCounter > 0 && quotesCounter > 0) {
    fParserWaeHandler->
      mixedCommasAndQuotesInOctaveIndication ();
  }

  // compute result - the octave starting at Middle C (c') has number 4
  if (commasCounter > 0) {
    result =
      msrOctaveKind (
        (int) msrOctaveKind::kOctave3 + commasCounter);
  }
  else if (quotesCounter > 0) {
    result =
      msrOctaveKind (
        (int) msrOctaveKind::kOctave3 - quotesCounter);
  }
  else {
    result = msrOctaveKind::kOctave3;
  }

#ifdef TRACING_IS_ENABLED
    if (fTraceSyntax) {
      gLogStream <<
        "=== parse()" <<
        ", result: " << msrOctaveKindAsString (result) <<
        endl;
    }
#endif

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "<-- OctaveIndication()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl <<
      "=================================================================" <<
      endl;
  }
#endif

  return result;
}

// --------------------------------------------------------------------------
//  msdlParser::NoteDuration
// --------------------------------------------------------------------------

void msdlParser::NoteDuration (
  msrDottedDuration&  dottedDuration,
  int                 dotsNumber,
  S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      endl <<
      "=================================================================" <<
      endl <<
      "--> NoteDuration()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("NoteDuration");
  }
#endif

  ++gIndenter;

  // there should be an integer or a name such as "maxima"
  msrDurationKind durationKind = msrDurationKind::k_NoDuration;

  if (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fNoteDurationFIRST,
      "NoteDuration")
  ) {
    if (fCurrentTokenKind == msdlTokenKind::kTokenInteger) {
      // get the fraction
      int durationInteger = fCurrentToken.getTokenDescription ().getInteger ();

      durationKind =
        msrDurationKindFromInteger (
          fCurrentToken.getTokenLineNumber (),
          durationInteger);

#ifdef TRACING_IS_ENABLED
      if (fTraceSyntax) {
        gLogStream <<
          "=== parse()" <<
          ", durationInteger: \"" << durationInteger << "\"" <<
          ", durationKind: \"" << msrDurationKindAsString (durationKind) << "\"" <<
          endl;
      }
#endif
    }

    else if (checkMandatoryTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::kTokenName,
      "NoteDuration")
    ) {
      // get the duration name
      string durationName = fCurrentToken.getTokenDescription ().getString ();

      durationKind =
        msrDurationKindFromString (
          fCurrentToken.getTokenLineNumber (),
          durationName);

#ifdef TRACING_IS_ENABLED
      if (fTraceSyntax) {
        gLogStream <<
          "=== parse()" <<
          ", durationName: \"" << durationName << "\"" <<
          ", durationKind: \"" << msrDurationKindAsString (durationKind) << "\"" <<
          endl;
      }
#endif
    }

    // sanity check
    switch (durationKind) {
      case msrDurationKind::k_NoDuration:
        fParserWaeHandler->
          malformedNoteDuration ();
        break;
      default:
        ;
    } // switch

    // consume the duration integer or name
    fetchNextToken ();
  } // while

  // there can be dots
  dotsNumber = 0;

  while (
    checkOptionalTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::kTokenDot,
      "NoteDuration")
  ) {
    ++dotsNumber;
    // consume the dot
    fetchNextToken ();
  } // while

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "=== parse()" <<
      ", dotsNumber: \"" << dotsNumber << "\"" <<
      endl;
  }
#endif

  // compute the dotted duration
  dottedDuration =
    msrDottedDuration (
      durationKind,
      dotsNumber);

#ifdef TRACING_IS_ENABLED
    if (fTraceSyntax) {
      gLogStream <<
        "=== parse()" <<
        ", dottedDuration: " << dottedDuration.asString () <<
        endl;
    }
#endif

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "<-- NoteDuration()" <<
      ", dottedDuration: " << dottedDuration.asString () <<
      endl;
  }
#endif
}


}
