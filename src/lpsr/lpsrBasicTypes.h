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

// score output kinds
//______________________________________________________________________________
enum lpsrScoreOutputKind {
  kScoreOnly, // default value
  kScoreAndParts,
  kPartsAndScore,
  kPartsOnly,
  kScoreAndPartsOneFile,
  kPartsAndScoreOneFile,
  kPartsOnlyOneFile };

EXP string lpsrScoreOutputKindAsString (
  lpsrScoreOutputKind scoreOutputKind);

extern map<string, lpsrScoreOutputKind>
  gGlobalLpsrScoreOutputKindsMap;

string existingLpsrScoreOutputKinds (int namesListMaxLength);

void initializeLpsrScoreOutputKindsMap ();

// octave entry
//______________________________________________________________________________
enum lpsrOctaveEntryKind {
  kOctaveEntryRelative, // default value
  kOctaveEntryAbsolute,
  kOctaveEntryFixed };

EXP string lpsrOctaveEntryKindAsString (
  lpsrOctaveEntryKind octaveEntryKind);

extern map<string, lpsrOctaveEntryKind>
  gGlobalLpsrOctaveEntryKindsMap;

EXP string existingLpsrOctaveEntryKinds (int namesListMaxLength);

void initializeLpsrOctaveEntryKindsMap ();

// accidental styles
//______________________________________________________________________________
enum lpsrAccidentalStyleKind {
  kDefault,
  kVoice,
  kModern,
  kModernCautionary, kModernVoice, kModernVoiceCautionary,
  kPiano, kPianoCautionary,
  kNeoModern,
  kNeoModernCautionary, kNeoModernVoice, kNeoModernVoiceCautionary,
  kDodecaphonic, kDodecaphonicNoRepeat,
  kDodecaphonicFirst, kTeaching, kNoReset, kForget };

EXP string lpsrAccidentalStyleKindAsString (
  lpsrAccidentalStyleKind accidentalStyleKind);

EXP string lpsrAccidentalStyleKindAsLilypondString (
  lpsrAccidentalStyleKind accidentalStyleKind);

extern map<string, lpsrAccidentalStyleKind>
  gGlobalLpsrAccidentalStyleKindsMap;

EXP string existingLpsrAccidentalStyleKinds (int namesListMaxLength);

void initializeLpsrAccidentalStyleKindsMap ();

// chords languages
//______________________________________________________________________________
enum lpsrChordsLanguageKind {
  k_IgnatzekChords, // LilyPond default
  k_GermanChords, k_SemiGermanChords, k_ItalianChords, k_FrenchChords };

EXP string lpsrChordsLanguageKindAsString (
  lpsrChordsLanguageKind languageKind);

extern map<string, lpsrChordsLanguageKind>
  gGlobalLpsrChordsLanguageKindsMap;

EXP string existingLpsrChordsLanguageKinds (int namesListMaxLength);

void initializeLpsrChordsLanguageKindsMap ();

// whole notes
//______________________________________________________________________________
EXP string wholeNotesAsLilypondString (
  int      inputLineNumber,
  rational wholeNotes,
  int&     dotsNumber);

EXP string wholeNotesAsLilypondString (
  int      inputLineNumber,
  rational wholeNotes);

// dotted durations
//______________________________________________________________________________

EXP string dottedDurationAsLilypondString (
  int               inputLineNumber,
  msrDottedDuration dottedDuration);

EXP string dottedDurationAsLilypondStringWithoutBackSlash (
  int               inputLineNumber,
  msrDottedDuration dottedDuration);

// rests measures
//______________________________________________________________________________
EXP string restMeasuresWholeNoteAsLilypondString (
  int      inputLineNumber,
  rational wholeNotes);

// texts lists
//______________________________________________________________________________
EXP void writeTextsListAsLilypondString (
  const list<string>& textsList,
  ostream&            os);

// pitches and octaves
//______________________________________________________________________________
EXP string msrSemiTonesPitchKindAsLilypondString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrSemiTonesPitchKind              semiTonesPitchKind);

EXP string msrSemiTonesPitchAndOctaveAsLilypondString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  S_msrSemiTonesPitchAndOctave       quarterTonesPitchAndOctave);

// lyrics durations
//______________________________________________________________________________
enum lpsrLyricsDurationsKind {
  kLyricsDurationsImplicit, // default value
  kLyricsDurationsExplicit };

EXP string lpsrLyricsDurationsKindAsString (
  lpsrLyricsDurationsKind lyricsDurationsKind);

extern map<string, lpsrLyricsDurationsKind>
  gGlobalLpsrLyricsDurationsKindsMap;

EXP string existingLpsrLyricsDurationsKinds (int namesListMaxLength);

void initializeLpsrLyricsDurationsKindsMap ();

// initialization
//______________________________________________________________________________
void initializeLPSRBasicTypes ();


} // namespace MusicXML2


#endif
