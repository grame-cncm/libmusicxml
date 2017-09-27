/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __lpsrBasicTypes__
#define __lpsrBasicTypes__

#include <list>
#include <map>

#include "smartpointer.h"

#include "rational.h"

#include "msrBasicTypes.h"


using namespace std;

namespace MusicXML2 
{

/*!
\internal
\brief A macro to exit when a condition if false
*/
// a macro to abort with code source line information,
// as assert() itself
//______________________________________________________________________________
#define basicLpsrAssert( condition, messageIfFalse ) \
{ \
  if (! condition) { \
    cerr << \
      endl << \
      "#### " << messageIfFalse << \
      endl << endl << \
      flush; \
    assert(condition); \
  } \
}

/*!
\internal
\brief A macro to emit error messages regarding MSR and exit
*/
//______________________________________________________________________________
#define basicLpsrInternalError( inputLineNumber, message ) \
{ \
  cerr << endl << endl; \
\
  cerr << \
    "[[[ basic INTERNAL ERROR ]]], " << \
 /* JMI   gGeneralOptions->fInputSourceName << */ \
    ", input line " << inputLineNumber << ":" << \
    endl << \
\
    message << \
    endl << endl << \
    idtr; \
\
  assert(false); \
}

// chords languages
//______________________________________________________________________________
enum lpsrChordsLanguage {
  k_IgnatzekChords, // LilyPond default
  k_GermanChords, k_SemiGermanChords, k_ItalianChords, k_FrenchChords };
  
string lpsrChordsLanguageAsString (
  lpsrChordsLanguage language);

extern map<string, lpsrChordsLanguage>
  gLpsrChordsLanguagesMap;

string existingLpsrChordsLanguages ();

void initializeLpsrChordsLanguages ();

// whole notes
//______________________________________________________________________________
string wholeNotesAsLilypondString (
  int      inputLineNumber,
  rational wholeNotes,
  int&     dotsNumber);

string wholeNotesAsLilypondString (
  int      inputLineNumber,
  rational wholeNotes);

// notes
//______________________________________________________________________________
  /* JMI
void writeNoteAsLilypondString (
  S_msrNote note,
  ostream&  os);
*/

// texts lists
//______________________________________________________________________________
void writeTextsListAsLilypondString (
  const list<string>& textsList,
  ostream&            os);


} // namespace MusicXML2


#endif
