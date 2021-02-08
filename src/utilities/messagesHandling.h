/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___messagesHandling___
#define ___messagesHandling___

#include <iomanip> // for 'setw()'
#include <set>     // for 'setw()'

#include "exports.h"

#include "exceptions.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
EXP void msgAssert (
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  bool   condition,
  string messageIfFalse);

//______________________________________________________________________________
EXP void oahWarning (string warningMessage);

EXP void oahError (string errorMessage);

EXP void oahInternalError (string errorMessage);

//______________________________________________________________________________
EXP void msrUnsupported (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message);

//______________________________________________________________________________
EXP void msrInternalWarning (
  string inputSourceName,
  int    inputLineNumber,
  string message);

EXP void msrInternalError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message);

//______________________________________________________________________________
EXP void msrWarning (
  string inputSourceName,
  int    inputLineNumber,
  string message);

EXP void msrError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message);

//______________________________________________________________________________
EXP void msrMusicXMLWarning (
  string inputSourceName,
  int    inputLineNumber,
  string message);

EXP void msrMusicXMLError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message);

//______________________________________________________________________________
EXP void msrMusicXMLWarning (
  string inputSourceName,
  int    inputLineNumber,
  string message);

EXP void msrMusicXMLError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message);

//______________________________________________________________________________
EXP void lpsrLilypondWarning (
  string inputSourceName,
  int    inputLineNumber,
  string message);

EXP void lpsrLilypondError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message);

//______________________________________________________________________________
EXP void lpsrMusicXMLWarning (
  string inputSourceName,
  int    inputLineNumber,
  string message);

EXP void lpsrMusicXMLError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message);

//______________________________________________________________________________
EXP void bsrWarning (
  string inputSourceName,
  int    inputLineNumber,
  string message);

EXP void bsrInternalError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message);

//______________________________________________________________________________
EXP void msdlWarning (
  string inputSourceName,
  int    inputLineNumber,
  string message);

EXP void msdlError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message);

EXP void msdlInternalError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message);

//______________________________________________________________________________
EXP void bmmlWarning (
  string inputSourceName,
  int    inputLineNumber,
  string message);

EXP void bmmlError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message);

EXP void bmmlInternalError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message);

//______________________________________________________________________________
EXP void meiWarning (
  string inputSourceName,
  int    inputLineNumber,
  string message);

EXP void meiError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message);

EXP void meiInternalError (
  string inputSourceName,
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message);

//______________________________________________________________________________
EXP extern set<int> gGlobalWarningsInputLineNumbers;
EXP extern set<int> gGlobalErrorsInputLineNumbers;

EXP void displayWarningsAndErrorsInputLineNumbers ();

}


#endif

//______________________________________________________________________________
/*
EXP void msrStreamsError (
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message);

EXP void msrStreamsWarning (
  int    inputLineNumber,
  string sourceCodeFileName,
  int    sourceCodeLineNumber,
  string message);
*/

