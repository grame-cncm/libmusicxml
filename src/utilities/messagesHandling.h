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


namespace MusicXML2
{

//______________________________________________________________________________
EXP void msgAssert (
  bool   condition,
  std::string messageIfFalse);

//______________________________________________________________________________
EXP void msgWarning (
  std::string context,
  std::string inputSourceName,
  int         inputLineNumber,
  std::string message);

EXP void msgError (
  std::string context,
  std::string inputSourceName,
  int         inputLineNumber,
  std::string sourceCodeFileName,
  int         sourceCodeLineNumber,
  std::string message);

//______________________________________________________________________________
EXP void oahWarning (std::string warningMessage);

EXP void oahError (std::string errorMessage);

EXP void oahInternalError (string errorMessage);

//______________________________________________________________________________
EXP void msrUnsupported (
  std::string inputSourceName,
  int         inputLineNumber,
  std::string sourceCodeFileName,
  int         sourceCodeLineNumber,
  std::string message);

//______________________________________________________________________________
EXP void msrInternalWarning (
  std::string inputSourceName,
  int         inputLineNumber,
  std::string message);

EXP void msrInternalError (
  std::string inputSourceName,
  int         inputLineNumber,
  std::string sourceCodeFileName,
  int         sourceCodeLineNumber,
  std::string message);

//______________________________________________________________________________
EXP void msrMusicXMLWarning (
  std::string inputSourceName,
  int         inputLineNumber,
  std::string message);

EXP void msrMusicXMLError (
  std::string inputSourceName,
  int         inputLineNumber,
  std::string sourceCodeFileName,
  int         sourceCodeLineNumber,
  std::string message);

//______________________________________________________________________________
EXP void lpsrMusicXMLWarning (
  std::string inputSourceName,
  int         inputLineNumber,
  std::string message);

EXP void lpsrMusicXMLError (
  std::string inputSourceName,
  int         inputLineNumber,
  std::string sourceCodeFileName,
  int         sourceCodeLineNumber,
  std::string message);

//______________________________________________________________________________
EXP void bsrWarning (
  std::string inputSourceName,
  int         inputLineNumber,
  std::string message);

EXP void bsrInternalError (
  std::string inputSourceName,
  int         inputLineNumber,
  std::string sourceCodeFileName,
  int         sourceCodeLineNumber,
  std::string message);

//______________________________________________________________________________
EXP void bmmlWarning (
  std::string inputSourceName,
  int         inputLineNumber,
  std::string message);

EXP void bmmlError (
  std::string inputSourceName,
  int         inputLineNumber,
  std::string sourceCodeFileName,
  int         sourceCodeLineNumber,
  std::string message);

EXP void bmmlInternalError (
  std::string inputSourceName,
  int         inputLineNumber,
  std::string sourceCodeFileName,
  int         sourceCodeLineNumber,
  std::string message);

//______________________________________________________________________________
EXP void meiWarning (
  std::string inputSourceName,
  int         inputLineNumber,
  std::string message);

EXP void meiError (
  std::string inputSourceName,
  int         inputLineNumber,
  std::string sourceCodeFileName,
  int         sourceCodeLineNumber,
  std::string message);

EXP void meiInternalError (
  std::string inputSourceName,
  int         inputLineNumber,
  std::string sourceCodeFileName,
  int         sourceCodeLineNumber,
  std::string message);

//______________________________________________________________________________
/*
EXP void msrStreamsError (
  int         inputLineNumber,
  std::string sourceCodeFileName,
  int         sourceCodeLineNumber,
  std::string message);

EXP void msrStreamsWarning (
  int         inputLineNumber,
  std::string sourceCodeFileName,
  int         sourceCodeLineNumber,
  std::string message);
*/

//______________________________________________________________________________
EXP extern std::set<int> gGlobalWarningsInputLineNumbers;
EXP extern std::set<int> gGlobalErrorsInputLineNumbers;

EXP void displayWarningsAndErrorsInputLineNumbers ();

}


#endif
