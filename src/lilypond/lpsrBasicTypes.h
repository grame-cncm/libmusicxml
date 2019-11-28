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

string lpsrScoreOutputKindAsString (
  lpsrScoreOutputKind scoreOutputKind);

extern map<string, lpsrScoreOutputKind>
  gLpsrScoreOutputKindsMap;

string existingLpsrScoreOutputKinds (int namesListMaxLength);

void initializeLpsrScoreOutputKindsMap ();

// paper units
//______________________________________________________________________________

enum lpsrPaperUnitKind {
  kInchUnit, kCentimeterUnit, kMillimeterUnit };

string lpsrPaperUnitKindAsString (
  lpsrPaperUnitKind paperUnitKind);

extern map<string, lpsrPaperUnitKind>
  gLpsrPaperUnitKindsMap;

string existingLpsrPaperUnitKinds (int namesListMaxLength);

void initializeLpsrPaperUnitKindsMap ();

// octave entry
//______________________________________________________________________________
enum lpsrOctaveEntryKind {
  kOctaveEntryRelative, // default value
  kOctaveEntryAbsolute,
  kOctaveEntryFixed };

string lpsrOctaveEntryKindAsString (
  lpsrOctaveEntryKind octaveEntryKind);

extern map<string, lpsrOctaveEntryKind>
  gLpsrOctaveEntryKindsMap;

string existingLpsrOctaveEntryKinds (int namesListMaxLength);

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

string lpsrAccidentalStyleKindAsString (
  lpsrAccidentalStyleKind accidentalStyleKind);

string lpsrAccidentalStyleKindAsLilypondString (
  lpsrAccidentalStyleKind accidentalStyleKind);

extern map<string, lpsrAccidentalStyleKind>
  gLpsrAccidentalStyleKindsMap;

string existingLpsrAccidentalStyleKinds (int namesListMaxLength);

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

string existingLpsrChordsLanguageKinds (int namesListMaxLength);

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

// rests measures
//______________________________________________________________________________
string restMeasuresWholeNoteAsLilypondString (
  int      inputLineNumber,
  rational wholeNotes);

// texts lists
//______________________________________________________________________________
void writeTextsListAsLilypondString (
  const list<string>& textsList,
  ostream&            os);

// pitches and octaves
//______________________________________________________________________________
string msrSemiTonesPitchKindAsLilypondString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrSemiTonesPitchKind              semiTonesPitchKind);

string msrSemiTonesPitchAndOctaveAsLilypondString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  S_msrSemiTonesPitchAndOctave       quarterTonesPitchAndOctave);

// lyrics alignment
//______________________________________________________________________________
enum lpsrLyricsAlignmentKind {
  kLyricsAlignmentAutomatic, // default value
  kLyricsAlignmentManual };

string lpsrLyricsAlignmentKindAsString (
  lpsrLyricsAlignmentKind lyricsAlignmentKind);

extern map<string, lpsrLyricsAlignmentKind>
  gLpsrLyricsAlignmentKindsMap;

string existingLpsrLyricsAlignmentKinds (int namesListMaxLength);

void initializeLpsrLyricsAlignmentKindsMap ();

// initialization
//______________________________________________________________________________
void initializeLPSRBasicTypes ();


} // namespace MusicXML2


#endif
