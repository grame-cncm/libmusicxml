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

#include "musicXMLOptions.h"
#include "generalOptions.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
void msrAssert (
  bool   condition,
  string messageIfFalse)
{
  if (! condition) {
    gLogIOstream <<
      "#### msrAssert failure: " << messageIfFalse <<
      ", aborting." <<
      endl;
     
    abort ();
  }
}

//______________________________________________________________________________
void msrWarning (
  string context,
  string inputSourceName,
  int    inputLineNumber,
  string message)
{
  if (! gGeneralOptions->fQuiet) {
    gLogIOstream <<
      "*** " << context << " warning *** " <<
      inputSourceName << ":" << inputLineNumber << ": " <<message <<
      endl;

    gWarningsInputLineNumbers.insert (inputLineNumber);
  }
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

//______________________________________________________________________________
void msrError (
  string context,
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message)
{
  if (! gGeneralOptions->fQuiet) {
    if (gGeneralOptions->fDisplaySourceCodePosition) {
      gLogIOstream <<
        baseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
        " ";
    }

    if (! gGeneralOptions->fIgnoreErrors) {
      gLogIOstream <<
        "### " << context << " ERROR ### " <<
        inputSourceName << ":" << inputLineNumber << ": " << message <<
        endl;
  
      gErrorsInputLineNumbers.insert (inputLineNumber);
    }
  }
}

//______________________________________________________________________________
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

  if (! gGeneralOptions->fIgnoreErrors) {
    if (gGeneralOptions->fAbortOnErrors) {
      abort ();
    }
    else {
      exit (15);
    }
  }
}

//______________________________________________________________________________
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

  if (! gGeneralOptions->fIgnoreErrors) {
    exit (16);
  }
}

//______________________________________________________________________________
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

  abort ();
}

void msrLimitation (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message)
{
  if (! (gGeneralOptions->fQuiet && gGeneralOptions->fIgnoreErrors)) {  
    if (gGeneralOptions->fDisplaySourceCodePosition) {
      gLogIOstream <<
        baseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
        " ";
    }

    gLogIOstream <<
      "### MSR LIMITATION ### " <<
      inputSourceName << ":" << inputLineNumber << ": " << message <<
      endl;

    abort ();
  }
}

//______________________________________________________________________________
void msrStreamsError (
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string  message)
{
  if (! (gGeneralOptions->fQuiet && gGeneralOptions->fIgnoreErrors)) {  
    if (gGeneralOptions->fDisplaySourceCodePosition) {
      gLogIOstream <<
        baseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
        " ";
    }

    gLogIOstream <<
      "### " << "MSR STREAMS" << " ERROR ### " <<
      "fake line number" << ":" << inputLineNumber << ": " << message <<
      endl;
  }

  abort ();
}

void msrStreamsWarning (
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string  message)
{
  if (! (gGeneralOptions->fQuiet && gGeneralOptions->fIgnoreErrors)) {  
    if (gGeneralOptions->fDisplaySourceCodePosition) {
      gLogIOstream <<
        baseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
        " ";
    }

    gLogIOstream <<
      "*** " << "MSR STREAMS" << " warning *** " <<
      " ### " << "MSR STREAMS" << " ERROR ### " <<
      "fake line number" << ":" << inputLineNumber << ": " << message <<
      endl;
  }

  abort ();
}

//______________________________________________________________________________
std::set<int> gWarningsInputLineNumbers;
std::set<int> gErrorsInputLineNumbers;

void displayWarningsAndErrorsInputLineNumbers ()
{
  int warningsInputLineNumbersSize =
    gWarningsInputLineNumbers.size ();
    
  if (warningsInputLineNumbersSize && ! gGeneralOptions->fQuiet) {
    gLogIOstream <<
      "Warning message(s) were issued for input " <<
      singularOrPluralWithoutNumber (
        warningsInputLineNumbersSize, "line", "lines") <<
      " ";
      
    set<int>::const_iterator
      iBegin = gWarningsInputLineNumbers.begin (),
      iEnd   = gWarningsInputLineNumbers.end (),
      i      = iBegin;
    for ( ; ; ) {
      gLogIOstream << (*i);
      if (++i == iEnd) break;
      gLogIOstream << ", ";
    } // for

    gLogIOstream <<
      endl;
  }
  
  int errorsInputLineNumbersSize =
    gErrorsInputLineNumbers.size ();

  if (errorsInputLineNumbersSize) {
    gLogIOstream <<
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
      gLogIOstream << (*i);
      if (++i == iEnd) break;
      gLogIOstream << ", ";
    } // for

    gLogIOstream <<
      endl;
  }
}

}
