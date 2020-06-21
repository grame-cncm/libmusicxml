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
void msrAssert (
  bool   condition,
  string messageIfFalse)
{
  if (! condition) {
    int saveIndent = gIndenter.getIndent ();

    gIndenter.resetToZero ();

    gLogOstream <<
      "#### msrAssert failure: " << messageIfFalse <<
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
void msrWarning (
  string context,
  string inputSourceName,
  int    inputLineNumber,
  string message)
{
  if (! gGeneralOah->fQuiet) {
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

void msrError (
  string context,
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message)
{
  if (! gGeneralOah->fQuiet) {
    if (gGeneralOah->fDisplaySourceCodePosition) {
      gLogOstream <<
        baseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
        " ";
    }

    if (! gGeneralOah->fDontShowErrors) {
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

void msrUnsupported (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message)
{
  if (! (gGeneralOah->fQuiet && gGeneralOah->fDontShowErrors)) {
    int saveIndent = gIndenter.getIndent ();

    gIndenter.resetToZero ();

    if (gGeneralOah->fDisplaySourceCodePosition) {
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
void msrMusicXMLWarning (
  string inputSourceName,
  int    inputLineNumber,
  string message)
{
  msrWarning (
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
  msrError (
    "MusicXML",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gGeneralOah->fDontShowErrors) {
    if (! gGeneralOah->fDontExitOnErrors) { // JMI
      throw msrMusicXMLException (message);
    }
    else {
      throw msrMusicXMLException (message);
    }
  }
}

//______________________________________________________________________________
void msrInternalWarning (
  string inputSourceName,
  int    inputLineNumber,
  string message)
{
  msrWarning (
    "INTERNAL",
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
  msrError (
    "MSR INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  throw msrMsrInternalException (message);
}

//______________________________________________________________________________
void lpsrMusicXMLWarning (
  string inputSourceName,
  int    inputLineNumber,
  string message)
{
  msrWarning (
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
  msrError (
    "LPSR",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gGeneralOah->fDontShowErrors) { // JMI
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
  msrWarning (
    "BSR",
    inputSourceName,
    inputLineNumber,
    message);
}

//______________________________________________________________________________
void bsrInternalError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message)
{
  msrError (
    "BSR INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  throw bsrInternalException (message);
}

//______________________________________________________________________________
/*
void msrStreamsWarning (
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string  message)
{
  if (! (gGeneralOah->fQuiet && gGeneralOah->fDontShowErrors)) {
    if (gGeneralOah->fDisplaySourceCodePosition) {
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
  if (! (gGeneralOah->fQuiet && gGeneralOah->fDontShowErrors)) {
    if (gGeneralOah->fDisplaySourceCodePosition) {
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

  if (warningsInputLineNumbersSize && ! gGeneralOah->fQuiet) {
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
