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
#include <fstream>

#include "versions.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "msdlParser.h"

#include "msdl2msdrOah.h"

#include "msdl2msdrInterface.h"


using namespace std;

namespace MusicXML2
{
//_______________________________________________________________________________
xmlErr scanTheMdslInput (
  string   inputSourceName,
  istream& inputStream,
  string   passNumber,
  string   passDescription)
{
  ++gIndenter;

  // create an MSDL scanner
  msdlScanner
    scanner (
      inputSourceName,
      inputStream);

  // should we translate the keywords?
  msdlKeywordsLanguageKind
    keywordsTranslationLanguageKind =
      gGlobalMsdl2msdrOahGroup->
        getMsdlKeywordsTranslationLanguageKind ();

  switch (keywordsTranslationLanguageKind) {
    case msdlKeywordsLanguageKind::k_NoKeywordsLanguage:
      {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTraceOahGroup->getTracePasses ()) {
          string separator =
            "%--------------------------------------------------------------";

          gLogStream <<
            endl <<
            separator <<
            endl <<
            gTab <<
            passNumber << ": " << passDescription <<
            endl <<
            separator <<
            endl;
        }
#endif

        // should we ignore separator tokens?
        bool
          ignoreSeparatorTokens =
            gGlobalMsdl2msdrOahGroup->
              getIgnoreSeparatorTokens ();

        msdlIgnoreSeparatorTokensKind
          ignoreSeparatorTokensKind =
            ignoreSeparatorTokens
              ? kIgnoreSeparatorTokensYes
              : kIgnoreSeparatorTokensNo;

        scanner.scanAllTheInputAtOnce (
          ignoreSeparatorTokensKind);
      }
      break;

    default:
      {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTracePasses ()) {
        string separator =
          "%--------------------------------------------------------------";

        gLogStream <<
          endl <<
          separator <<
          endl <<
          gTab <<
          passNumber << ": translating the MSDL input into another keywords language" <<
          endl <<
          separator <<
          endl;
      }
#endif

      // what comments type should we use in the translation output?
      msdlCommentsTypeKind
        commentsTypeKind =
          gGlobalMsdl2msdrOahGroup->
            getMsdlCommentsTypeTranslationKind ();

      scanner.translateAllTheInputToKeywordsLanguage (
        keywordsTranslationLanguageKind,
        commentsTypeKind);
      }
  } // switch

  --gIndenter;

  return kNoErr;
}

//_______________________________________________________________________________
xmlErr parseTheMdslInput (
  string   inputSourceName,
  istream& inputStream,
  string   passNumber,
  string   passDescription)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePasses ()) {
    string separator =
      "%--------------------------------------------------------------";

    gLogStream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      passNumber << ": " << passDescription <<
      endl <<
      separator <<
      endl;
  }
#endif

  // create an msdlParser
  msdlParser
    parser (
      inputSourceName,
      inputStream);

  // to the parsing
  parser.parse ();

  if (! parser.getSourceIsSyntacticallyCorrect ()) {
    string message =
      "### Conversion from MSDL to MSDR failed ###";

    gLogStream <<
      message <<
      endl;

    throw msgMsdlToMsdrScoreException (message);
  }

  return kNoErr;
}

//_______________________________________________________________________________
EXP xmlErr istream2msdrWithHandler (
  string       inputSourceName,
  istream&     inputStream,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
  // start the clock
  clock_t startClock = clock ();

  // has quiet mode been requested?
  // ------------------------------------------------------

  if (gGlobalGeneralOahGroup->getQuiet ()) {
    // disable all trace and display options
    handler->
      enforceHandlerQuietness ();
  }

  // should we perform lexical analysis only?
  // ------------------------------------------------------

  string passNumber = "Pass 1";
  string passDescription;

  bool
    lexicalAnalysisOnly =
      gGlobalMsdl2msdrOahGroup->
        getLexicalAnalysisOnly ();

  if (lexicalAnalysisOnly) {
    // YES, don't parse the input, merely scan it
    passDescription =
      "perform lexical analysis of the MSDL input";

    scanTheMdslInput (
      inputSourceName,
      inputStream,
      passNumber,
      passDescription);
  }

  else {
    // NO, parse the input
    passDescription =
      "translating the MSDL input into an MSDR score";

    parseTheMdslInput (
      inputSourceName,
      inputStream,
      passNumber,
      passDescription);
  }

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    passNumber,
    passDescription,
    timingItem::kMandatory,
    startClock,
    endClock);

  if (gIndenter != 0) {
    if (! gGlobalGeneralOahGroup->getQuiet ()) {
      stringstream s;

      s <<
        "gIndenter value after pass " << passNumber << ": " <<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        inputSourceName, // JMI ???gGlobalOahOahGroup->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }

  // check indentation
  if (gIndenter != 0) {
    gLogStream <<
      "### msdlFile2msdrWithHandler gIndenter final value: " <<
      gIndenter.getIndent () <<
      " ###" <<
      endl;

    gIndenter.resetToZero ();
  }

  return kNoErr;
}

//_______________________________________________________________________________
EXP xmlErr msdlFile2msdrWithHandler (
  string       fileName,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
  // open input file
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePasses ()) {
    err <<
      "Opening file \"" << fileName << "\" for writing" <<
      endl;
  }
#endif

  ifstream
    inputStream (
      fileName,
      ifstream::in);

  if (! inputStream.is_open ()) {
    stringstream s;

    s <<
      "Could not open MSDL input file \"" <<
      fileName <<
      "\" for reading, quitting";

    string message = s.str ();

    gLogStream <<
      message <<
      endl;

    throw msgMsdlToMsdrScoreException (message);
  }

  return
    istream2msdrWithHandler (
      fileName,
      inputStream,
      out,
      err,
      handler);
}

//_______________________________________________________________________________
EXP xmlErr msdlString2msdrWithHandler (
  const char * buffer,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
  stringstream
    inputStream (
      buffer,
      ios_base::in);

  return
    istream2msdrWithHandler (
      "buffer",
      inputStream,
      out,
      err,
    handler);
}

/* JMI
//_______________________________________________________________________________
void displayMsdrScore (
  const S_msdrScore theMsdrScore,
  S_msdlOahGroup     msdlOpts,
  S_msdrOahGroup    msdrOpts)
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    theMsdrScore != nullptr,
    "theMsdrScore is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    separator <<
    endl <<
    gTab <<
    "Optional pass: displaying the MSDR as text" <<
    endl <<
    separator <<
    endl << endl <<
    theMsdrScore <<
    separator <<
    endl << endl;

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    "",
    "Display the MSDR as text",
    timingItem::kOptional,
    startClock,
    endClock);
}

void displayMsdrScoreShort (
  const S_msdrScore theMsdrScore,
  S_msdlOahGroup     msdlOpts,
  S_msdrOahGroup    msdrOpts)
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    theMsdrScore != nullptr,
    "theMsdrScore is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    separator <<
    endl <<
    gTab <<
    "Optional pass: displaying the MSDR as text, short version" <<
    endl <<
    separator <<
    endl << endl;

  theMsdrScore->printShort (gLogStream);

  gLogStream <<
    separator <<
    endl << endl;

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    "",
    "Display the MSDR as text, short version",
    timingItem::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayMsdrScore_OptionalPass (
  const S_msdrScore theMsdrScore,
  S_msdlOahGroup     msdlOpts,
  S_msdrOahGroup    msdrOpts)
{
  // display it
  displayMsdrScore (
    theMsdrScore,
    msdlOpts,
    msdrOpts);

  if (gIndenter != 0) {
    if (! gGlobalGeneralOahGroup->getQuiet ()) {
      stringstream s;

      s <<
        "gIndenter value after MSDR score display: " <<
        gIndenter.getIndent ();

      msdlMusicXMLWarning (
        gGlobalOahOahGroup->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

void displayMsdrScoreShort_OptionalPass (
  const S_msdrScore theMsdrScore,
  S_msdlOahGroup     msdlOpts,
  S_msdrOahGroup    msdrOpts)
{
  // display it
  displayMsdrScoreShort (
    theMsdrMsdrScore,
    msdlOpts,
    msdrOpts);

  if (gIndenter != 0) {
    if (! gGlobalGeneralOahGroup->getQuiet ()) {
      stringstream s;

      s <<
        "gIndenter value after MSDR score short display: " <<
        gIndenter.getIndent ();

      msdlMusicXMLWarning (
        gGlobalOahOahGroup->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}
*/


}

/* JMI
  // the MSR score
  // ------------------------------------------------------

  S_msrScore theMsrScore;

  // perform syntactical and semantic analysis (pass 1)
  // ------------------------------------------------------

  try {
    theMsrScore =
      convertMsdlScoreToMsdrScore (
        fileName,
        inputStream,
        "Pass 1",
        "Perform lexical analysis");
  }
  catch (msgMxmlTreeToMsrException& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }
  catch (std::exception& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalXml2lyInsiderOahGroup->getQuitAfterPass2a ()) {
    err <<
      endl <<
      "Quitting after pass 2a as requested" <<
      endl;

    return kNoErr;
  }
*/

