/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsrBasicTypes___
#define ___lpsrBasicTypes___

#include <list>
#include <map>

#include "smartpointer.h"

#include "rational.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

// accidental styles
//______________________________________________________________________________
enum lpsrAccidentalStyleKind {
  kDefaultStyle,
  kVoiceStyle,
  kModernStyle,
  kModernCautionaryStyle, kModernVoiceStyle, kModernVoiceCautionaryStyle,
  kPianoStyle, kPianoCautionaryStyle,
  kNeoModernStyle,
  kNeoModernCautionaryStyle, kNeoModernVoiceStyle, kNeoModernVoiceCautionaryStyle,
  kDodecaphonicStyle, kDodecaphonicNoRepeatStyle,
  kDodecaphonicFirstStyle, kTeachingStyle, kNoResetStyle, kForgetStyle };

string lpsrAccidentalStyleKindAsString (
  lpsrAccidentalStyleKind styleKind);

string lpsrAccidentalStyleKindAsLilypondString (
  lpsrAccidentalStyleKind styleKind);

extern map<string, lpsrAccidentalStyleKind>
  gLpsrAccidentalStyleKindsMap;

string existingLpsrAccidentalStyleKinds ();

void initializeLpsrAccidentalStyleKindsMap ();

// chords languages
//______________________________________________________________________________
enum lpsrChordsLanguageKind {
  k_IgnatzekChords, // LilyPond default
  k_GermanChords, k_SemiGermanChords, k_ItalianChords, k_FrenchChords };
  
string lpsrChordsLanguageKindAsString (
  lpsrChordsLanguageKind languageKind);

extern map<string, lpsrChordsLanguageKind>
  gLpsrChordsLanguageKindsMap;

string existingLpsrChordsLanguageKinds ();

void initializeLpsrChordsLanguageKindsMap ();

// whole notes
//______________________________________________________________________________
string wholeNotesAsLilypondString (
  int      inputLineNumber,
  rational wholeNotes,
  int&     dotsNumber);

string wholeNotesAsLilypondString (
  int      inputLineNumber,
  rational wholeNotes);

// dotted durations
//______________________________________________________________________________

string dottedDurationAsLilypondString (
  int               inputLineNumber,
  msrDottedDuration dottedDuration);

string dottedDurationAsLilypondStringWithoutBackSlash (
  int               inputLineNumber,
  msrDottedDuration dottedDuration);

// multiple rests
//______________________________________________________________________________
string multipleRestWholeNoteAsLilypondString (
  int      inputLineNumber, // JMI
  rational wholeNotes);

// texts lists
//______________________________________________________________________________
void writeTextsListAsLilypondString (
  const list<string>& textsList,
  ostream&            os);

//______________________________________________________________________________
void initializeLPSRBasicTypes ();


} // namespace MusicXML2


#endif
