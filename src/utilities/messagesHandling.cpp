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

    gLogOstream <<
      "#### msgAssert failure: " << messageIfFalse <<
      ", exiting." <<
      endl;

    gIndenter.setIndent (saveIndent);

    throw msrAssertException (messageIfFalse);
  }
}

//______________________________________________________________________________
void oahWarning (string warningMessage)
{
  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  gLogOstream <<
    "*** WARNING in the options and help: " <<
    warningMessage <<
    endl;

  gIndenter.setIndent (saveIndent);
}

void oahError (string errorMessage)
{
  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  gLogOstream <<
    "### ERROR in the options and help: " <<
    errorMessage <<
    endl;

  gIndenter.setIndent (saveIndent);

  throw msrOahException (errorMessage);
}

//______________________________________________________________________________
void msgWarning (
  string context,
  string inputSourceName,
  int    inputLineNumber,
  string message)
{
  if (! gGlobalGeneralOahGroup->fQuiet) {
    int saveIndent = gIndenter.getIndent ();

    gIndenter.resetToZero ();

    gLogOstream <<
      "*** " << context << " warning *** " <<
      inputSourceName << ":" << inputLineNumber << ": " <<message <<
      endl;

    gWarningsInputLineNumbers.insert (inputLineNumber);

    gIndenter.setIndent (saveIndent);
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
  if (! gGlobalGeneralOahGroup->fQuiet) {
    if (gGlobalGeneralOahGroup->fDisplaySourceCodePosition) {
      gLogOstream <<
        baseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
        " ";
    }

    if (! gGlobalGeneralOahGroup->fDontShowErrors) {
      int saveIndent = gIndenter.getIndent ();

      gIndenter.resetToZero ();

      gLogOstream <<
        "### " << context << " ERROR ### " <<
        inputSourceName << ":" << inputLineNumber << ": " << message <<
        endl;

      gIndenter.setIndent (saveIndent);

      gErrorsInputLineNumbers.insert (inputLineNumber);
    }
  }

  throw msrMsrException (message);
}

//______________________________________________________________________________
void msrUnsupported (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message)
{
  if (! (gGlobalGeneralOahGroup->fQuiet && gGlobalGeneralOahGroup->fDontShowErrors)) {
    int saveIndent = gIndenter.getIndent ();

    gIndenter.resetToZero ();

    if (gGlobalGeneralOahGroup->fDisplaySourceCodePosition) {
      gLogOstream <<
        baseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
        " ";
    }

    gLogOstream <<
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
  msgError (
    "MSR INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

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

  if (! gGlobalGeneralOahGroup->fDontShowErrors) {
    if (! gGlobalGeneralOahGroup->fDontExitOnErrors) { // JMI
      throw msrMusicXMLException (message);
    }
    else {
      throw msrMusicXMLException (message);
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

  if (! gGlobalGeneralOahGroup->fDontShowErrors) { // JMI
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
  msgError (
    "BSR INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  throw bsrInternalException (message);
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

  if (! gGlobalGeneralOahGroup->fDontShowErrors) { // JMI
    throw bmmlException (message);
  }

  throw lpsrMusicXMLException (message);
}

void bmmlInternalError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message)
{
  msgError (
    "BMML INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

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

  if (! gGlobalGeneralOahGroup->fDontShowErrors) { // JMI
    throw meiException (message);
  }

  throw lpsrMusicXMLException (message);
}

void meiInternalError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message)
{
  msgError (
    "MEI INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  throw meiInternalException (message);
}

//______________________________________________________________________________
/*
void msrStreamsWarning (
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string  message)
{
  if (! (gGlobalGeneralOahGroup->fQuiet && gGlobalGeneralOahGroup->fDontShowErrors)) {
    if (gGlobalGeneralOahGroup->fDisplaySourceCodePosition) {
      gLogOstream <<
        baseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
        " ";
    }

    gLogOstream <<
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
  if (! (gGlobalGeneralOahGroup->fQuiet && gGlobalGeneralOahGroup->fDontShowErrors)) {
    if (gGlobalGeneralOahGroup->fDisplaySourceCodePosition) {
      gLogOstream <<
        baseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
        " ";
    }

    gLogOstream <<
      "### " << "MSR STREAMS" << " ERROR ### " <<
      "fake line number" << ":" << inputLineNumber << ": " << message <<
      endl;
  }

  throw msrStreamsException (message);
}
*/

//______________________________________________________________________________
std::set<int> gWarningsInputLineNumbers;
std::set<int> gErrorsInputLineNumbers;

void displayWarningsAndErrorsInputLineNumbers ()
{
  int warningsInputLineNumbersSize =
    gWarningsInputLineNumbers.size ();

  if (warningsInputLineNumbersSize && ! gGlobalGeneralOahGroup->fQuiet) {
    gLogOstream <<
      "Warning message(s) were issued for input " <<
      singularOrPluralWithoutNumber (
        warningsInputLineNumbersSize, "line", "lines") <<
      " ";

    set<int>::const_iterator
      iBegin = gWarningsInputLineNumbers.begin (),
      iEnd   = gWarningsInputLineNumbers.end (),
      i      = iBegin;
    for ( ; ; ) {
      gLogOstream << (*i);
      if (++i == iEnd) break;
      gLogOstream << ", ";
    } // for

    gLogOstream << endl;
  }

  int errorsInputLineNumbersSize =
    gErrorsInputLineNumbers.size ();

  if (errorsInputLineNumbersSize) {
    gLogOstream <<
      endl <<
      "Error message(s) were issued for input " <<
      singularOrPluralWithoutNumber (
        errorsInputLineNumbersSize, "line", "lines") <<
      " ";

    set<int>::const_iterator
      iBegin = gErrorsInputLineNumbers.begin (),
      iEnd   = gErrorsInputLineNumbers.end (),
      i      = iBegin;
    for ( ; ; ) {
      gLogOstream << (*i);
      if (++i == iEnd) break;
      gLogOstream << ", ";
    } // for

    gLogOstream << endl;
  }
}


}
