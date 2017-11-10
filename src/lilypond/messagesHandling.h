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

#include "iomanip" // for 'setw()'


namespace MusicXML2 
{

/*!
\addtogroup msr
@{
*/

/*!
\internal
\brief A macro to exit when a condition if false
*/
// a macro to abort with code source line information,
// as assert() itself
//______________________________________________________________________________

<<<<<<< HEAD
void msrAssert (
  bool   condition,
  std::string messageIfFalse);
=======
#define msrAssert( condition, messageIfFalse ) \
{ \
  if (! (condition) ) { \
    cerr << \
      endl << \
      "#### msrAssert failure: " << messageIfFalse << \
      endl << \
      "Aborting." << \
      endl << \
      endl << \
      flush; \
      \
    assert(condition); \
  } \
}
>>>>>>> a9badbde74ce22074ff142d1a9265c7a168a65d6

//______________________________________________________________________________
void msrWarning (
  std::string context,
  std::string inputSourceName,
  int    inputLineNumber,
  std::string message);

//______________________________________________________________________________
void msrMusicXMLWarning (
  int    inputLineNumber,
  std::string message);

//______________________________________________________________________________
void lpsrMusicXMLWarning (
  int    inputLineNumber,
  std::string message);

//______________________________________________________________________________
void msrInternalWarning (
  int    inputLineNumber,
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

/*!
\internal
\brief A macro to emit error messages regarding MusicXML data and exit
*/
//______________________________________________________________________________
void lpsrMusicXMLError (
  std::string inputSourceName,
  int    inputLineNumber,
  std::string sourceCodeFileName,
  int    sourceCodeLineNumber,
  std::string message);

/*!
\internal
\brief A macro to emit error messages regarding MSR and exit
*/
//______________________________________________________________________________

void msrInternalError (
  std::string inputSourceName,
  int    inputLineNumber,
  std::string sourceCodeFileName,
  int    sourceCodeLineNumber,
  std::string message);


/*! @} */

}


#endif
