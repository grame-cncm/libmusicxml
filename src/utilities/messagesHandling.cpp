/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "utilities.h"

#include "enableAbortToDebugErrors.h"

#include "messagesHandling.h"

#include "generalOah.h"


using namespace std;

namespace MusicXML2
{
//______________________________________________________________________________
void msgAssert (
  bool   condition,
  string messageIfFalse)
{
  if (! condition) {
    int saveIndent = gIndenter.getIndent ();

    gIndenter.resetToZero ();

    gLogStream <<
      "#### msgAssert failure: " << messageIfFalse <<
      ", quitting." <<
      endl;

    gIndenter.setIndent (saveIndent);

    throw msrAssertException (messageIfFalse);
  }
}

//______________________________________________________________________________
void msgWarning (
  string context,
  string inputSourceName,
  int    inputLineNumber,
  string message)
{
  if (! gGlobalGeneralOahGroup->getQuiet ()) {
    int saveIndent = gIndenter.getIndent ();

    gIndenter.resetToZero ();

    gLogStream <<
      "*** " << context << " warning *** " <<
      inputSourceName << ":" << inputLineNumber << ": " <<message <<
      endl;

    gGlobalWarningsInputLineNumbers.insert (inputLineNumber);

    gIndenter.setIndent (saveIndent);
  }
}

// private function
void msgErrorWithoutException (
  string context,
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message)
{
  if (! gGlobalGeneralOahGroup->getQuiet ()) {
    if (gGlobalGeneralOahGroup->getDisplaySourceCodePosition ()) {
      gLogStream <<
        baseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
        " ";
    }

    if (! gGlobalGeneralOahGroup->getDontShowErrors ()) {
      int saveIndent = gIndenter.getIndent ();

      gIndenter.resetToZero ();

      gLogStream <<
        "### " << context << " ERROR ### " <<
        inputSourceName << ":" << inputLineNumber << ": " << message <<
        endl;

      gIndenter.setIndent (saveIndent);

      gGlobalErrorsInputLineNumbers.insert (inputLineNumber);
    }
  }
}

void msgError (
  string context,
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message)
{
  msgErrorWithoutException (
    context,
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  throw msrMsrException (message);
}

//______________________________________________________________________________
void oahWarning (string warningMessage)
{
  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  gLogStream <<
    "*** WARNING in the options and help: " <<
    warningMessage <<
    endl;

  gIndenter.setIndent (saveIndent);
}

void oahError (string errorMessage)
{
  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  gLogStream <<
    "### ERROR in the options and help: " <<
    errorMessage <<
    endl;

#ifdef ABORT_TO_DEBUG_ERRORS
  abort ();
#endif

  gIndenter.setIndent (saveIndent);

  throw msrOahException (errorMessage);
}

void oahInternalError (string errorMessage)
{
  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  gLogStream <<
    "### INTERNAL ERROR in the options and help: " <<
    errorMessage <<
    endl;

#ifdef ABORT_TO_DEBUG_ERRORS
  abort ();
#endif

  gIndenter.setIndent (saveIndent);

  throw msrOahException (errorMessage);
}

//______________________________________________________________________________
void msrUnsupported (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message)
{
  if (! (gGlobalGeneralOahGroup->getQuiet () && gGlobalGeneralOahGroup->getDontShowErrors ())) {
    int saveIndent = gIndenter.getIndent ();

    gIndenter.resetToZero ();

    if (gGlobalGeneralOahGroup->getDisplaySourceCodePosition ()) {
      gLogStream <<
        baseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
        " ";
    }

    gLogStream <<
      "### MSR LIMITATION ### " <<
      inputSourceName << ":" << inputLineNumber << ": " << message <<
      endl;

    gIndenter.setIndent (saveIndent);
  }

  throw msrMsrUnsupportedException (message);
}

//______________________________________________________________________________
void msrInternalWarning (
  string inputSourceName,
  int    inputLineNumber,
  string message)
{
  msgWarning (
    "MSR INTERNAL",
    inputSourceName,
    inputLineNumber,
    message);
}

void msrInternalError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message)
{
  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  msgErrorWithoutException (
    "MSR INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

#ifdef ABORT_TO_DEBUG_ERRORS
  abort ();
#endif

  gIndenter.setIndent (saveIndent);

  throw msrMsrInternalException (message);
}

//______________________________________________________________________________
void msrMusicXMLWarning (
  string inputSourceName,
  int    inputLineNumber,
  string message)
{
  msgWarning (
    "MusicXML",
    inputSourceName,
    inputLineNumber,
    message);
}

void msrMusicXMLError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message)
{
  msgError (
    "MusicXML",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gGlobalGeneralOahGroup->getDontShowErrors ()) {
    if (! gGlobalGeneralOahGroup->getDontQuitOnErrors ()) { // JMI
      throw msrMusicXMLException (message);
    }
    else {
      throw msrMusicXMLException (message);
    }
  }
}

//______________________________________________________________________________
void lpsrLilypondWarning (
  string inputSourceName,
  int    inputLineNumber,
  string message)
{
  msgWarning (
    "LilyPond",
    inputSourceName,
    inputLineNumber,
    message);
}

void lpsrLilypondError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message)
{
  msgError (
    "LilyPond",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gGlobalGeneralOahGroup->getDontShowErrors ()) {
    if (! gGlobalGeneralOahGroup->getDontQuitOnErrors ()) { // JMI
      throw lpsrScoreToLilypondException (message);
    }
    else {
      throw lpsrScoreToLilypondException (message);
    }
  }
}

//______________________________________________________________________________
void lpsrMusicXMLWarning (
  string inputSourceName,
  int    inputLineNumber,
  string message)
{
  msgWarning (
    "LPSR",
    inputSourceName,
    inputLineNumber,
    message);
}

void lpsrMusicXMLError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message)
{
  msgError (
    "LPSR",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gGlobalGeneralOahGroup->getDontShowErrors ()) { // JMI
    throw lpsrMusicXMLException (message);
  }

  throw lpsrMusicXMLException (message);
}

//______________________________________________________________________________
void bsrWarning (
  string inputSourceName,
  int    inputLineNumber,
  string message)
{
  msgWarning (
    "BSR",
    inputSourceName,
    inputLineNumber,
    message);
}

void bsrInternalError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message)
{
  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  msgErrorWithoutException (
    "BSR INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

#ifdef ABORT_TO_DEBUG_ERRORS
  abort ();
#endif

  gIndenter.setIndent (saveIndent);

  throw bsrInternalException (message);
}

//______________________________________________________________________________
EXP void msplWarning (
  std::string inputSourceName,
  int         inputLineNumber,
  std::string message)
{
  msgWarning (
    "MSPL",
    inputSourceName,
    inputLineNumber,
    message);

}

EXP void msplError (
  std::string inputSourceName,
  int         inputLineNumber,
  std::string sourceCodeFileName,
  int         sourceCodeLineNumber,
  std::string message)
{
  msgError (
    "MSPL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gGlobalGeneralOahGroup->getDontShowErrors ()) { // JMI
    throw msplException (message);
  }

  throw msplException (message);
}

EXP void msplInternalError (
  std::string inputSourceName,
  int         inputLineNumber,
  std::string sourceCodeFileName,
  int         sourceCodeLineNumber,
  std::string message)
{
  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  msgErrorWithoutException (
    "MSPL INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

#ifdef ABORT_TO_DEBUG_ERRORS
  abort ();
#endif

  gIndenter.setIndent (saveIndent);

  throw msplInternalException (message);
}

//______________________________________________________________________________
void bmmlWarning (
  string inputSourceName,
  int    inputLineNumber,
  string message)
{
  msgWarning (
    "BMML",
    inputSourceName,
    inputLineNumber,
    message);
}

void bmmlError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message)
{
  msgError (
    "BMML",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gGlobalGeneralOahGroup->getDontShowErrors ()) { // JMI
    throw bmmlException (message);
  }

  throw bmmlException (message);
}

void bmmlInternalError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message)
{
  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  msgErrorWithoutException (
    "BMML INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

#ifdef ABORT_TO_DEBUG_ERRORS
  abort ();
#endif

  gIndenter.setIndent (saveIndent);

  throw bmmlInternalException (message);
}

//______________________________________________________________________________
void meiWarning (
  string inputSourceName,
  int    inputLineNumber,
  string message)
{
  msgWarning (
    "MEI",
    inputSourceName,
    inputLineNumber,
    message);
}

void meiError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message)
{
  msgError (
    "MEI",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gGlobalGeneralOahGroup->getDontShowErrors ()) { // JMI
    throw meiException (message);
  }

  throw meiException (message);
}

void meiInternalError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message)
{
  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  msgErrorWithoutException (
    "MEI INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

#ifdef ABORT_TO_DEBUG_ERRORS
  abort ();
#endif

  gIndenter.setIndent (saveIndent);

  throw meiInternalException (message);
}

//______________________________________________________________________________
std::set<int> gGlobalWarningsInputLineNumbers;
std::set<int> gGlobalErrorsInputLineNumbers;

void displayWarningsAndErrorsInputLineNumbers ()
{
  int warningsInputLineNumbersSize =
    gGlobalWarningsInputLineNumbers.size ();

  if (warningsInputLineNumbersSize && ! gGlobalGeneralOahGroup->getQuiet ()) {
    gLogStream <<
      "Warning message(s) were issued for input " <<
      singularOrPluralWithoutNumber (
        warningsInputLineNumbersSize, "line", "lines") <<
      " ";

    set<int>::const_iterator
      iBegin = gGlobalWarningsInputLineNumbers.begin (),
      iEnd   = gGlobalWarningsInputLineNumbers.end (),
      i      = iBegin;
    for ( ; ; ) {
      gLogStream << (*i);
      if (++i == iEnd) break;
      gLogStream << ", ";
    } // for

    gLogStream << endl;
  }

  int errorsInputLineNumbersSize =
    gGlobalErrorsInputLineNumbers.size ();

  if (errorsInputLineNumbersSize) {
    gLogStream <<
      endl <<
      "Error message(s) were issued for input " <<
      singularOrPluralWithoutNumber (
        errorsInputLineNumbersSize, "line", "lines") <<
      " ";

    set<int>::const_iterator
      iBegin = gGlobalErrorsInputLineNumbers.begin (),
      iEnd   = gGlobalErrorsInputLineNumbers.end (),
      i      = iBegin;
    for ( ; ; ) {
      gLogStream << (*i);
      if (++i == iEnd) break;
      gLogStream << ", ";
    } // for

    gLogStream << endl;
  }
}


}


//______________________________________________________________________________
/*
void msrStreamsWarning (
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string  message)
{
  if (! (gGlobalGeneralOahGroup->getQuiet () && gGlobalGeneralOahGroup->getDontShowErrors ())) {
    if (gGlobalGeneralOahGroup->getDisplaySourceCodePosition ()) {
      gLogStream <<
        baseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
        " ";
    }

    gLogStream <<
      "*** " << "MSR STREAMS" << " warning *** " <<
      " ### " << "MSR STREAMS" << " ERROR ### " <<
      "fake line number" << ":" << inputLineNumber << ": " << message <<
      endl;
  }
}

void msrStreamsError (
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string  message)
{
  if (! (gGlobalGeneralOahGroup->getQuiet () && gGlobalGeneralOahGroup->getDontShowErrors ())) {
    if (gGlobalGeneralOahGroup->getDisplaySourceCodePosition ()) {
      gLogStream <<
        baseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
        " ";
    }

    gLogStream <<
      "### " << "MSR STREAMS" << " ERROR ### " <<
      "fake line number" << ":" << inputLineNumber << ": " << message <<
      endl;
  }

  throw msrStreamsException (message);
}
*/

