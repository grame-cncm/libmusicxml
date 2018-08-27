/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __messagesHandling__
#define __messagesHandling__

#include <iomanip> // for 'setw()'
#include <set>     // for 'setw()'


namespace MusicXML2 
{

//______________________________________________________________________________
void msrAssert (
  bool   condition,
  std::string messageIfFalse);

//______________________________________________________________________________
void msrWarning (
  std::string context,
  std::string inputSourceName,
  int         inputLineNumber,
  std::string message);

//______________________________________________________________________________
void msrMusicXMLWarning (
  std::string inputSourceName,
  int         inputLineNumber,
  std::string message);

//______________________________________________________________________________
void lpsrMusicXMLWarning (
  std::string inputSourceName,
  int         inputLineNumber,
  std::string message);

//______________________________________________________________________________
void msrInternalWarning (
  std::string inputSourceName,
  int         inputLineNumber,
  std::string message);

//______________________________________________________________________________
void msrError (
  std::string context,
  std::string inputSourceName,
  int    inputLineNumber,
  std::string sourceCodeFileName,
  int    sourceCodeLineNumber,
  std::string message);

//______________________________________________________________________________
void msrMusicXMLError (
  std::string inputSourceName,
  int    inputLineNumber,
  std::string sourceCodeFileName,
  int    sourceCodeLineNumber,
  std::string message);

//______________________________________________________________________________
void lpsrMusicXMLError (
  std::string inputSourceName,
  int    inputLineNumber,
  std::string sourceCodeFileName,
  int    sourceCodeLineNumber,
  std::string message);

//______________________________________________________________________________
void msrInternalError (
  std::string inputSourceName,
  int    inputLineNumber,
  std::string sourceCodeFileName,
  int    sourceCodeLineNumber,
  std::string message);

//______________________________________________________________________________  
void msrLimitation (
  std::string inputSourceName,
  int    inputLineNumber,
  std::string sourceCodeFileName,
  int    sourceCodeLineNumber,
  std::string message);

//______________________________________________________________________________
void msrStreamsError (
  int         inputLineNumber,
  std::string sourceCodeFileName,
  int         sourceCodeLineNumber,
  std::string message);

void msrStreamsWarning (
  int         inputLineNumber,
  std::string sourceCodeFileName,
  int         sourceCodeLineNumber,
  std::string message);

//______________________________________________________________________________
extern std::set<int> gWarningsInputLineNumbers;
extern std::set<int> gErrorsInputLineNumbers;

void displayWarningsAndErrorsInputLineNumbers ();

}


#endif
