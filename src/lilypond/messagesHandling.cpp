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

#include "mxmlOptions.h"
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
      endl <<
      "#### msrAssert failure: " << messageIfFalse <<
      endl <<
      "Aborting." <<
      endl <<
      endl <<
      flush;
     
    assert(condition);
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
    const int fieldWidth = 18;
  
    gLogIOstream <<
      endl <<
      "!!! " << context << " WARNING !!!" <<
      endl;  
  
    gLogIOstream << left <<
      gTab << gTab <<
      setw (fieldWidth) <<
      "input source name" << " : " <<
      inputSourceName <<
      endl <<
      gTab << gTab <<
      setw (fieldWidth) <<
      "input line" << " : " << inputLineNumber <<
      endl;
  
    gLogIOstream <<
      message <<
      endl <<
      endl;
  }
}

//______________________________________________________________________________
void lpsrMusicXMLWarning (
  int    inputLineNumber,
  string message)
{
  msrWarning (
    "LPSR",
    gGeneralOptions->fInputSourceName,
    inputLineNumber,
    message);
}

//______________________________________________________________________________
void msrMusicXMLWarning (
  int    inputLineNumber,
  string message)
{
  msrWarning (
    "MusicXML",
    gGeneralOptions->fInputSourceName,
    inputLineNumber,
    message);
}

//______________________________________________________________________________
void msrInternalWarning (
  int    inputLineNumber,
  string message)
{
  msrWarning (
    "INTERNAL",
    gGeneralOptions->fInputSourceName,
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
    const int fieldWidth = 22;
  
    gLogIOstream <<
      endl <<
      "### " << context << " ERROR ###" <<
      endl;
    
    gLogIOstream << left <<
      gTab << gTab <<
      setw (fieldWidth) <<
      "input source name" << " : " <<
      inputSourceName <<
      endl <<
      gTab << gTab <<
      setw (fieldWidth) <<
      "input line" << " : " <<
      inputLineNumber <<
      endl <<
      gTab << gTab <<
      setw (fieldWidth) <<
      "source code file name" << " : " <<
      baseName (sourceCodeFileName) <<
      endl <<
      gTab << gTab <<
      setw (fieldWidth) <<
      "source code input line" << " : " <<
      sourceCodeLineNumber <<
      endl;
  
    gLogIOstream <<
      gIndenter.indentMultiLineString (
        message)  <<
      endl <<
      endl;
  }

  if (! gGeneralOptions->fIgnoreErrors)
    abort ();
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

    abort ();
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


}
