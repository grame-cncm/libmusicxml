/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrBasicTypes___
#define ___msrBasicTypes___

#include <vector>
#include <list>
#include <map>

#include "smartpointer.h"

#include "rational.h"


namespace MusicXML2
{

// input line numbers
//______________________________________________________________________________

#define K_NO_INPUT_LINE_NUMBER 0

// names lists max length
// ------------------------------------------------------

#define K_NAMES_LIST_MAX_LENGTH 50

// XMLLang
//______________________________________________________________________________
enum class msrXMLLangKind {
  kXMLLangIt, kXMLLangEn, kXMLLangDe, kXMLLangFr, kXMLLangJa, kXMLLangLa
};

EXP msrXMLLangKind msrXMLLangKindFromString (
  int    inputLineNumber,
  string XMLLangString);

string msrXMLLangKindAsString (
  msrXMLLangKind XMLLangKind);

// diatonic pitches
//______________________________________________________________________________
enum class msrDiatonicPitchKind {
  k_NoDiatonicPitch,

  // starting at C for LilyPond relative octave calculations
  kDiatonicPitchC,
  kDiatonicPitchD, kDiatonicPitchE, kDiatonicPitchF,
  kDiatonicPitchG, kDiatonicPitchA, kDiatonicPitchB
};

EXP msrDiatonicPitchKind msrDiatonicPitchKindFromString (
  char diatonicNoteName);

string msrDiatonicPitchKindAsString (
  msrDiatonicPitchKind diatonicPitchKind);

// alterations
//______________________________________________________________________________
enum class msrAlterationKind {
  k_NoAlteration,

  kAlterationTripleFlat, kAlterationDoubleFlat, kAlterationSesquiFlat,
  kAlterationFlat, kAlterationSemiFlat,
  kAlterationNatural,
  kAlterationSemiSharp, kAlterationSharp, kAlterationSesquiSharp,
  kAlterationDoubleSharp, kAlterationTripleSharp
};

EXP msrAlterationKind msrAlterationKindFromMusicXMLAlter (
  float alter);

EXP float msrMusicXMLAlterFromAlterationKind (
  msrAlterationKind alterationKind);

string msrAlterationKindAsString (
  msrAlterationKind alterationKind);

// accidentals
//______________________________________________________________________________
enum class msrAccidentalKind {
  kAccidentalNone,

  kAccidentalSharp, kAccidentalNatural,
  kAccidentalFlat, kAccidentalDoubleSharp,
  kAccidentalSharpSharp,
  kAccidentalFlatFlat, kAccidentalNaturalSharp,
  kAccidentalNaturalFlat, kAccidentalQuarterFlat,
  kAccidentalQuarterSharp,kAccidentalThreeQuartersFlat,
  kAccidentalThreeQuartersSharp,

  kAccidentalSharpDown, kAccidentalSharpUp,
  kAccidentalNaturalDown, kAccidentalNaturalUp,
  kAccidentalFlatDown, kAccidentalFlatUp,
  kAccidentalTripleSharp, kAccidentalTripleFlat,
  kAccidentalSlashQuarterSharp, kAccidentalSlashSharp,
  kAccidentalSlashFlat, kAccidentalDoubleSlashFlat,
  kAccidentalSharp_1, kAccidentalSharp_2,
  kAccidentalSharp_3,  kAccidentalSharp_5,
  kAccidentalFlat_1, kAccidentalFlat_2,
  kAccidentalFlat_3, kAccidentalFlat_4,
  kAccidentalSori, kAccidentalKoron,

  kAccidentalOther
};

string msrAccidentalKindAsString (
  msrAccidentalKind accidentalKind);

string msrAccidentalKindAsMusicXMLString (
  msrAccidentalKind accidentalKind);

// editorial accidentals
//______________________________________________________________________________

enum class msrEditorialAccidentalKind {
  kEditorialAccidentalYes, kEditorialAccidentalNo
};

string msrEditorialAccidentalKindAsString (
  msrEditorialAccidentalKind noteEditorialAccidentalKind);

// cautionary accidentals
//______________________________________________________________________________

enum class msrCautionaryAccidentalKind {
  kCautionaryAccidentalYes, kCautionaryAccidentalNo
};

string msrCautionaryAccidentalKindAsString (
  msrCautionaryAccidentalKind noteCautionaryAccidentalKind);

// semi tones pitches
//______________________________________________________________________________
enum class msrSemiTonesPitchKind {
  kSTP_NoSemiTonesPitch,

  kSTP_C_TripleFlat,
  kSTP_C_DoubleFlat, kSTP_C_Flat,
  kSTP_C_Natural,
  kSTP_C_Sharp, kSTP_C_DoubleSharp,
  kSTP_C_TripleSharp,

  kSTP_D_TripleFlat,
  kSTP_D_DoubleFlat, kSTP_D_Flat,
  kSTP_D_Natural,
  kSTP_D_Sharp, kSTP_D_DoubleSharp,
  kSTP_D_TripleSharp,

  kSTP_E_TripleFlat,
  kSTP_E_DoubleFlat, kSTP_E_Flat,
  kSTP_E_Natural,
  kSTP_E_Sharp, kSTP_E_DoubleSharp,
  kSTP_E_TripleSharp,

  kSTP_F_TripleFlat,
  kSTP_F_DoubleleFlat, kSTP_F_Flat,
  kSTP_F_Natural,
  kSTP_F_Sharp, kSTP_F_DoubleSharp,
  kSTP_F_TripleSharp,

  kSTP_G_TripleFlat,
  kSTP_G_DoubleFlat, kSTP_G_Flat,
  kSTP_G_Natural,
  kSTP_G_Sharp, kSTP_G_DoubleSharp,
  kSTP_G_TripleSharp,

  kSTP_A_TripleFlat,
  kSTP_A_DoubleFlat, kSTP_A_Flat,
  kSTP_A_Natural,
  kSTP_A_Sharp, kSTP_A_DoubleSharp,
  kSTP_A_TripleSharp,

  kSTP_B_TripleFlat,
  kSTP_B_DoubleFlat, kSTP_B_Flat,
  kSTP_B_Natural,
  kSTP_B_Sharp, kSTP_B_DoubleSharp,
  kSTP_B_TripleSharp
};

string msrSemiTonesPitchKindAsString (
  msrSemiTonesPitchKind semiTonesPitchKind);

// quarter tones pitches
//______________________________________________________________________________
enum class msrQuarterTonesPitchKind {
  k_NoQuarterTonesPitch,

  kQTP_Rest, kQTP_Skip,

  kQTP_A_TripleFlat,
  kQTP_A_DoubleFlat, kQTP_A_SesquiFlat, kQTP_A_Flat, kQTP_A_SemiFlat,
  kQTP_A_Natural,
  kQTP_A_SemiSharp, kQTP_A_Sharp, kQTP_A_SesquiSharp, kQTP_A_DoubleSharp,
  kQTP_A_TripleSharp,

  kQTP_B_TripleFlat,
  kQTP_B_DoubleFlat, kQTP_B_SesquiFlat, kQTP_B_Flat, kQTP_B_SemiFlat,
  kQTP_B_Natural,
  kQTP_B_SemiSharp, kQTP_B_Sharp, kQTP_B_SesquiSharp, kQTP_B_DoubleSharp,
  kQTP_B_TripleSharp,

  kQTP_C_TripleFlat,
  kQTP_C_DoubleFlat, kQTP_C_SesquiFlat, kQTP_C_Flat, kQTP_C_SemiFlat,
  kQTP_C_Natural,
  kQTP_C_SemiSharp, kQTP_C_Sharp, kQTP_C_SesquiSharp, kQTP_C_DoubleSharp,
  kQTP_C_TripleSharp,

  kQTP_D_TripleFlat,
  kQTP_D_DoubleFlat, kQTP_D_SesquiFlat, kQTP_D_Flat, kQTP_D_SemiFlat,
  kQTP_D_Natural,
  kQTP_D_SemiSharp, kQTP_D_Sharp, kQTP_D_SesquiSharp, kQTP_D_DoubleSharp,
  kQTP_D_TripleSharp,

  kQTP_E_TripleFlat,
  kQTP_E_DoubleFlat, kQTP_E_SesquiFlat, kQTP_E_Flat, kQTP_E_SemiFlat,
  kQTP_E_Natural,
  kQTP_E_SemiSharp, kQTP_E_Sharp, kQTP_E_SesquiSharp, kQTP_E_DoubleSharp,
  kQTP_E_TripleSharp,

  kQTP_F_TripleFlat,
  kQTP_F_DoubleFlat, kQTP_F_SesquiFlat, kQTP_F_Flat, kQTP_F_SemiFlat,
  kQTP_F_Natural,
  kQTP_F_SemiSharp, kQTP_F_Sharp, kQTP_F_SesquiSharp, kQTP_F_DoubleSharp,
  kQTP_F_TripleSharp,

  kQTP_G_TripleFlat,
  kQTP_G_DoubleFlat, kQTP_G_SesquiFlat, kQTP_G_Flat, kQTP_G_SemiFlat,
  kQTP_G_Natural,
  kQTP_G_SemiSharp, kQTP_G_Sharp, kQTP_G_SesquiSharp, kQTP_G_DoubleSharp,
  kQTP_G_TripleSharp
};

EXP string msrQuarterTonesPitchKindAsString (
  msrQuarterTonesPitchKind quarterTonesPitchKind);

EXP void fetchDiatonicPitchKindAndAlterationKindFromQuarterTonesPitchKind (
  int                      inputLineNumber,
  msrQuarterTonesPitchKind quarterTonesPitchKind,
  msrDiatonicPitchKind&    diatonicPitchKind,
  msrAlterationKind&       alterationKind);

EXP msrQuarterTonesPitchKind quarterTonesPitchKindFromDiatonicPitchAndAlteration (
  int                  inputLineNumber,
  msrDiatonicPitchKind diatonicPitchKind,
  msrAlterationKind    alterationKind);

EXP msrDiatonicPitchKind diatonicPitchKindFromQuarterTonesPitchKind (
  int                      inputLineNumber,
  msrQuarterTonesPitchKind quarterTonesPitchKind);

EXP msrAlterationKind alterationKindFromQuarterTonesPitchKind (
  int                      inputLineNumber,
  msrQuarterTonesPitchKind quarterTonesPitchKind);

EXP msrQuarterTonesPitchKind quarterTonesPitchKindFromSemiTonesPitchKind (
  msrSemiTonesPitchKind semiTonesPitchKind);

EXP msrSemiTonesPitchKind semiTonesPitchKindFromQuarterTonesPitchKind (
  msrQuarterTonesPitchKind quarterTonesPitchKind);

EXP msrSemiTonesPitchKind semiTonesPitchKindFromString (
  const string& theString);

// alterations preferences
//______________________________________________________________________________
enum class msrAlterationPreferenceKind {
  kAlterationPreferenceFlat, kAlterationPreferenceNatural, kAlterationPreferenceSharp
};

EXP msrQuarterTonesPitchKind msrSemiTonesPitchKindAsQuarterTonesPitchKind (
  msrSemiTonesPitchKind       semiTonesPitchKind,
  msrAlterationPreferenceKind alterationPreferenceKind);

// note
//______________________________________________________________________________

/*
  Measures can contain either:
    individual notes;
    tuplets;
    chords;
    double tremolos.

  Tuplets can contain either:
    individual notes;
    other tuplets;
    chords.

  Chords contain:
    individual notes.

  Grace notes groups can contain either (no tuplets here):
    individual notes;
    chords.

  Double tremolos can contain either:
    individual notes;
    chords.

  Notes can have attached:
    grace notes groups (before and after the note).

  Attempting to use classes to describe this graph would be a nightmare:
  we thus use enum types and uplinks to handles the variants.
*/

enum class msrNoteKind {
  k_NoNote,

  // in measures
  kNoteRegularInMeasure,
  kNoteRestInMeasure,
  kNoteSkipInMeasure, // an invisible rest
  kNoteUnpitchedInMeasure,

  // in chords
  kNoteRegularInChord,

  // in tuplets
  kNoteRegularInTuplet,
  kNoteRestInTuplet,
  kNoteUnpitchedInTuplet,

  // in grace notes groups
  kNoteRegularInGraceNotesGroup,
  kNoteSkipInGraceNotesGroup, // used to circumvent LilyPond issue #34

  // in chords in grace notes groups
  kNoteInChordInGraceNotesGroup,

  // in tuplets in grace notes groups
  kNoteInTupletInGraceNotesGroup,

  // in double-tremolos
  kNoteInDoubleTremolo
};

EXP string noteKindAsString (
  msrNoteKind noteKind);

// chords
//______________________________________________________________________________

enum class msrChordInKind {
  k_NoChordIn,

  kChordInMeasure,
  kChordInTuplet,
  kChordInGraceNotesGroup
};

EXP string chordKindAsString (
  msrChordInKind chordKind);

// tuplets
//______________________________________________________________________________

enum class msrTupletInKind {
  k_NoTupletIn,

  kTupletInMeasure,
  kTupletInTuplet
};

EXP string tupletKindAsString (
  msrTupletInKind tupletKind);

// beams
//______________________________________________________________________________

/*
  MusicXML beam hooks:
    see https://forums.makemusic.com/viewtopic.php?f=12&t=1397
    and the examples at https://github.com/grame-cncm/libmusicxml/tree/lilypond/files

  Michael Good Recordare LLC:
    An example of a "backward hook" is the 16th-note beam when a dotted eighth is beamed to a 16th.
    A "forward hook" would be the 16th note beam when a 16th is beamed to a dotted eighth.
    So in both these cases, the hook would be beam number 2 (the 16th beam).

    In most cases, the hooks follow what would be "normal" beaming for the given durations.
    But this is not always true, especially when there are secondary beam breaks.
    The Ave Maria example has both hooks and secondary beam breaks in measure 6 of the voice part,
    though not simultaneously.

    Say you beamed a dotted-eighth, sixteenth, sixteenth, dotted-eighth pattern together.
    If you broke the 16th beam, you would use "backward hook" followed by "forward hook".
    If not, you would use "begin" followed by "end".
*/

enum class msrBeamKind {
  k_NoBeam,

  kBeamBegin, kBeamContinue, kBeamEnd,
  kBeamForwardHook, kBeamBackwardHook
};

EXP string msrBeamKindAsString (
  msrBeamKind beamKind);

// ties
// ------------------------------------------------------

enum class msrTieKind {
  kTieNone,
  kTieStart, kTieContinue, kTieStop
};

string msrTieKindAsString (
  msrTieKind tieKind);

// slurs
// ------------------------------------------------------

enum class msrSlurTypeKind {
  k_NoSlur,

  kSlurTypeRegularStart, kSlurTypePhrasingStart,
  kSlurTypeContinue,
  kSlurTypeRegularStop, kSlurTypePhrasingStop
};

string msrSlurTypeKindAsString (
  msrSlurTypeKind slurTypeKind);

// intervals
//______________________________________________________________________________
enum class msrIntervalKind {
  k_NoIntervalKind,

  kIntervalDiminishedUnisson, kIntervalPerfectUnisson,
  kIntervalAugmentedUnisson,

  kIntervalDiminishedSecond, kIntervalMinorSecond,
  kIntervalMajorSecond, kIntervalAugmentedSecond,

  kIntervalDiminishedThird, kIntervalMinorThird,
  kIntervalMajorThird, kIntervalAugmentedThird,

  kIntervalDiminishedFourth, kIntervalPerfectFourth,
  kIntervalAugmentedFourth,

  kIntervalDiminishedFifth, kIntervalPerfectFifth,
  kIntervalAugmentedFifth,

  kIntervalDiminishedSixth, kIntervalMinorSixth,
  kIntervalMajorSixth, kIntervalAugmentedSixth,

  kIntervalDiminishedSeventh, kIntervalMinorSeventh,
  kIntervalMajorSeventh, kIntervalAugmentedSeventh,

  kIntervalDiminishedOctave, kIntervalPerfectOctave,
  kIntervalAugmentedOctave,

  kIntervalDiminishedNinth, kIntervalMinorNinth,
  kIntervalMajorNinth, kIntervalAugmentedNinth,

  kIntervalDiminishedTenth, kIntervalMinorTenth,
  kIntervalMajorTenth, kIntervalAugmentedTenth,

  kIntervalDiminishedEleventh, kIntervalPerfectEleventh,
  kIntervalAugmentedEleventh,

  kIntervalDiminishedTwelfth, kIntervalPerfectTwelfth,
  kIntervalAugmentedTwelfth,

  kIntervalDiminishedThirteenth, kIntervalMinorThirteenth,
  kIntervalMajorThirteenth, kIntervalAugmentedThirteenth
};

EXP int msrIntervalKindAsSemiTones (
  msrIntervalKind intervalKind);

EXP int msrIntervalKindAsQuarterTones (
  msrIntervalKind intervalKind);

string msrIntervalKindAsString (
  msrIntervalKind intervaKindl);

string msrIntervalKindAsShortString (
  msrIntervalKind intervalKind);

EXP msrIntervalKind invertIntervalKind (
  msrIntervalKind intervalKind);

EXP int intervalKindAsSemitones (
  msrIntervalKind intervalKind);

/* JMI
EXP msrSemiTonesPitchKind noteAtIntervalKindFromNote (
  msrIntervalKind             intervalKind,
  msrSemiTonesPitchKind       semiTonesPitchKind,
  msrAlterationPreferenceKind alterationPreferenceKind);
*/

EXP msrSemiTonesPitchKind noteAtIntervalFromSemiTonesPitch (
  int                   inputLineNumber,
  msrIntervalKind       intervalKind,
  msrSemiTonesPitchKind semiTonesPitchKind);

EXP msrQuarterTonesPitchKind noteAtIntervalFromQuarterTonesPitch (
  int                      inputLineNumber,
  msrIntervalKind          intervalKind,
  msrQuarterTonesPitchKind quarterTonesPitchKind);

EXP msrIntervalKind intervalBetweenSemiTonesPitches (
  msrSemiTonesPitchKind semiTonesPitch1,
  msrSemiTonesPitchKind semiTonesPitch2);

// staves
// ------------------------------------------------------

enum class msrStaffKind {
  kStaffRegular,
  kStaffTablature,
  kStaffHarmony,
  kStaffFiguredBass,
  kStaffDrum,
  kStaffRythmic
};

string msrStaffKindAsString (
  msrStaffKind staffKind);

// voices
//______________________________________________________________________________
enum class msrVoiceKind {
  kVoiceRegular,
  kVoiceHarmony,    // for MusicXML <harmony/>, LilyPond ChordNames
  kVoiceFiguredBass // for MusicXML <figured-bass/>, LilyPond FiguredBass
};

string msrVoiceKindAsString (
  msrVoiceKind voiceKind);

// measures
//______________________________________________________________________________
enum class msrMeasureKind {
  kMeasureKindUnknown,
  kMeasureKindRegular,
  kMeasureKindAnacrusis,
  kMeasureKindIncompleteStandalone,
  kMeasureKindIncompleteLastInRepeatCommonPart,
  kMeasureKindIncompleteLastInRepeatHookedEnding,
  kMeasureKindIncompleteLastInRepeatHooklessEnding,
  kMeasureKindIncompleteNextMeasureAfterCommonPart,
  kMeasureKindIncompleteNextMeasureAfterHookedEnding,
  kMeasureKindIncompleteNextMeasureAfterHooklessEnding,
  kMeasureKindOvercomplete,
  kMeasureKindCadenza,
  kMeasureKindMusicallyEmpty
};

string msrMeasureKindAsString (
  msrMeasureKind measureKind);

enum class msrMeasureImplicitKind {
  kMeasureImplicitKindYes,
  kMeasureImplicitKindNo
};

string msrMeasureImplicitKindAsString (
  msrMeasureImplicitKind measureImplicitKind);

// clefs
//______________________________________________________________________________

enum class msrClefKind {
  k_NoClef,

  kClefTreble,
  kClefSoprano, kClefMezzoSoprano, kClefAlto, kClefTenor, kClefBaritone, kClefBass,
  kClefTrebleLine1,
  kClefTrebleMinus15, kClefTrebleMinus8, kClefTreblePlus8, kClefTreblePlus15,

  kClefBassMinus15, kClefBassMinus8, kClefBassPlus8, kClefBassPlus15,

  kClefVarbaritone,

  kClefTablature4, kClefTablature5, kClefTablature6, kClefTablature7,

  kClefPercussion,

  kClefJianpu
};

string msrClefKindAsString (
  msrClefKind clefKind);

EXP msrClefKind clefKindFromString (
  int    inputLineNumber,
  string clefString);

string existingClefKinds (unsigned int namesListMaxLength);
string existingClefKindsNames (unsigned int namesListMaxLength);

extern map<string, msrClefKind>
  gGlobalClefKindsMap;

extern list<string>
  gClefKindsNamesList;

void initializeClefKinds ();

// keys
//______________________________________________________________________________

enum class msrKeyKind {
  kKeyTraditional, kKeyHumdrumScot
};

string msrKeyKindAsString (
  msrKeyKind keyKind);

enum class msrModeKind {
  k_NoMode,

  kModeMajor, kModeMinor,
  kModeIonian, kModeDorian, kModePhrygian, kModeLydian,
  kModeMixolydian, kModeAeolian, kModeLocrian
};

string msrModeKindAsString (
  msrModeKind modeKind);

msrModeKind modeKindFromString (
  int   inputLineNumber,
  string modeString);

// times
//______________________________________________________________________________
enum class msrTimeSymbolKind {
  kTimeSymbolNone,
  kTimeSymbolCommon,
  kTimeSymbolCut,
  kTimeSymbolNote,
  kTimeSymbolDottedNote,
  kTimeSymbolSingleNumber,
  kTimeSymbolSenzaMisura
};

string msrTimeSymbolKindAsString (
  msrTimeSymbolKind timeSymbolKind);

enum class msrTimeSeparatorKind {
  kTimeSeparatorNone,
  kTimeSeparatorHorizontal,
  kTimeSeparatorDiagonal,
  kTimeSeparatorVertical,
  kTimeSeparatorAdjacent
};

string msrTimeSeparatorKindAsString (
  msrTimeSeparatorKind timeSeparatorKind);

enum class msrTimeRelationKind {
  kTimeRelationNone,
  kTimeRelationParentheses,
  kTimeRelationBracket,
  kTimeRelationEquals,
  kTimeRelationSlash,
  kTimeRelationSpace,
  kTimeRelationHyphen
};

string msrTimeRelationKindAsString (
  msrTimeRelationKind timeRelationKind);

// repeats
//______________________________________________________________________________
enum class msrRepeatEndingKind {
  kEndingHooked,
  kEndingHookless
};

string msrRepeatEndingKindAsString (
  msrRepeatEndingKind repeatEndingKind);

// harmonies
//______________________________________________________________________________
enum class msrHarmonyKind {
  k_NoHarmony,

  // MusicXML harmonies

  kHarmonyMajor, kHarmonyMinor,
  kHarmonyAugmented, kHarmonyDiminished,

  kHarmonyDominant,
  kHarmonyMajorSeventh, kHarmonyMinorSeventh,
  kHarmonyDiminishedSeventh, kHarmonyAugmentedSeventh,
  kHarmonyHalfDiminished,
  kHarmonyMinorMajorSeventh,

  kHarmonyMajorSixth, kHarmonyMinorSixth,

  kHarmonyDominantNinth,
  kHarmonyMajorNinth, kHarmonyMinorNinth,

  kHarmonyDominantEleventh, kHarmonyMajorEleventh, kHarmonyMinorEleventh,

  kHarmonyDominantThirteenth, kHarmonyMajorThirteenth, kHarmonyMinorThirteenth,

  kHarmonySuspendedSecond, kHarmonySuspendedFourth,

  kHarmonyNeapolitan, kHarmonyItalian, kHarmonyFrench, kHarmonyGerman,

  kHarmonyPedal, kHarmonyPower, kHarmonyTristan,

  // jazz-specific harmonies

  kHarmonyMinorMajorNinth,                       // -maj9, minmaj9

  kHarmonyDominantSuspendedFourth,               // 7sus4, domsus4
  kHarmonyDominantAugmentedFifth,                // 7#5, domaug5
  kHarmonyDominantMinorNinth,                    // 7b9, dommin9
  kHarmonyDominantAugmentedNinthDiminishedFifth, // 7#9b5, domaug9dim5
  kIntervkDominantAugmentedNinthAugmentedFifthHarmonyalAugmentedThirteenth,  // 7#9#5, domaug9aug5
  kHarmonyDominantAugmentedEleventh,             // 7#11, domaug11

  kHarmonyMajorSeventhAugmentedEleventh,         // maj7#11, maj7aug11

  // other

  kHarmonyOther,

  kHarmonyNone,

  // aliases
  // ------------------------------------

  AllFirst = k_NoHarmony,
  AllLast  = kHarmonyNone,

  TrueHarmoniesFirst = kHarmonyMajor,
  TrueHarmoniesLast  = kHarmonyMajorSeventhAugmentedEleventh
};

string msrHarmonyKindAsString (
  msrHarmonyKind harmonyKind);

string msrHarmonyKindAsShortString (
  msrHarmonyKind harmonyKind);

string msrHarmonyKindShortName (
  msrHarmonyKind harmonyKind);

EXP msrHarmonyKind msrHarmonyKindFromString (
  const string& theString);

string existingHarmonyKinds (unsigned int namesListMaxLength);
string existingHarmonyKindsNames (unsigned int namesListMaxLength);

// constant
const int K_HARMONY_NO_INVERSION = -1;

EXP extern map<string, msrHarmonyKind>
  gGlobalHarmonyKindsMap;

EXP extern list<string>
  gHarmonyKindsNamesList;

void initializeHarmonyKinds ();

//________________________________________________________________________
template< typename T >
class EnumTrueHarmonies
{
  public:

    class Iterator
    {
      public:

        Iterator (int value)
          : fIterationIndex (value)
            {}

        T operator* (void) const
            { return (T) fIterationIndex; }

        void operator++ (void)
            {  ++fIterationIndex; }

        bool operator != (Iterator rhs)
            {  return fIterationIndex != rhs.fIterationIndex; }

      private:

        int fIterationIndex;
     };
};

template< typename T >
typename EnumTrueHarmonies<T>::Iterator begin (EnumTrueHarmonies<T>)
{
  return typename EnumTrueHarmonies<T>::Iterator ((int)T::TrueHarmoniesFirst);
}

template< typename T >
typename EnumTrueHarmonies<T>::Iterator end (EnumTrueHarmonies<T>)
{
  return typename EnumTrueHarmonies<T>::Iterator (((int)T::TrueHarmoniesLast) + 1);
}

// quarter tones pitches languages
//______________________________________________________________________________
enum class msrQuarterTonesPitchesLanguageKind {
  kQTPNederlands, kQTPCatalan, kQTPDeutsch, kQTPEnglish,
  kQTPEspanol, kQTPFrancais, kQTPItaliano, kQTPNorsk,
  kQTPPortugues, kQTPSuomi, kQTPSvenska, kQTPVlaams, kQTPArabic
};

string msrQuarterTonesPitchesLanguageKindAsString (
  msrQuarterTonesPitchesLanguageKind languageKind);

msrQuarterTonesPitchesLanguageKind msrQuarterTonesPitchesLanguageKindFromString (
  string theString);

string msrDiatonicPitchKindAsString ( // JMI
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrDiatonicPitchKind               diatonicPitchKind);

string msrQuarterTonesPitchKindAsStringInLanguage (
  msrQuarterTonesPitchKind           quarterTonesPitchKind,
  msrQuarterTonesPitchesLanguageKind languageKind);

EXP msrQuarterTonesPitchKind quarterTonesPitchKindFromString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  string                             quarterTonesPitchName);

/* JMI
 msrSemiTonesPitchKindAsString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrSemiTonesPitchKind              semiTonesPitchKind);
  */

string semiTonesPitchKindAsFlatsAndSharps (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrSemiTonesPitchKind              semiTonesPitchKind);

/* JMI
string msrQuarterTonesPitchKindAsFlatsAndSharps (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrQuarterTonesPitchKind           quarterTonesPitchKind);
*/

string existingMsrQuarterTonesPitchesLanguageKinds (
  unsigned int namesListMaxLength);

// enharmonies
//______________________________________________________________________________
EXP msrSemiTonesPitchKind enharmonicSemiTonesPitch (
  msrSemiTonesPitchKind       semiTonesPitchKind,
  msrAlterationPreferenceKind alterationPreferenceKind);

// octaves
//______________________________________________________________________________
enum class msrOctaveKind {
  k_NoOctave,

  kOctave0, kOctave1, kOctave2, kOctave3,
  kOctave4, // that of middle C
  kOctave5, kOctave6, kOctave7, kOctave8, kOctave9
};

EXP int octaveNumberFromOctaveKind (msrOctaveKind octaveKind);

// successor and predecessor
msrOctaveKind octaveSucc (msrOctaveKind octaveKind);
msrOctaveKind octavePred (msrOctaveKind octaveKind);

/*
   // Declare prefix and postfix decrement operators.
   Point& operator--();       // Prefix decrement operator.
   Point operator--(int);     // Postfix decrement operator.
*/

// prefix operators
msrOctaveKind& operator++ (msrOctaveKind& octaveKind);
msrOctaveKind& operator-- (msrOctaveKind& octaveKind);

// postfix operators
msrOctaveKind operator++ (msrOctaveKind& octaveKind, int);
msrOctaveKind operator-- (msrOctaveKind& octaveKind, int);

EXP msrOctaveKind msrOctaveKindFromNumber (
  int inputLineNumber,
  int octaveNumber);

EXP msrOctaveKind msrOctaveKindFromCommasOrQuotes (
  int    inputLineNumber,
  string octaveIndication);

string msrOctaveKindAsString (msrOctaveKind octaveKind);

// octave entry
//______________________________________________________________________________
enum class msrOctaveEntryKind {
  kOctaveEntryRelative,
  kOctaveEntryAbsolute,
  kOctaveEntryFixed
};

string msrOctaveEntryKindAsString (
  msrOctaveEntryKind octaveEntryKind);

extern map<string, msrOctaveEntryKind>
  gGlobalMsrOctaveEntryKindsMap;

string existingMsrOctaveEntryKinds (unsigned int namesListMaxLength);

void initializeMsrOctaveEntryKindsMap ();

// durations
//______________________________________________________________________________
enum class msrDurationKind {
  k_NoDuration,

  // from longest to shortest for the algorithms
  kMaxima, kLong, kBreve, kWhole, kHalf,
  kQuarter,
  kEighth, k16th, k32nd, k64th, k128th, k256th, k512th, k1024th
};

EXP msrDurationKind msrDurationKindFromMusicXMLString (
  int    inputLineNumber,
  string durationString);

EXP msrDurationKind msrDurationKindFromInteger (
  int inputLineNumber,
  int durationInteger);

EXP msrDurationKind msrDurationKindFromString (
  int    inputLineNumber,
  string durationString);

EXP rational rationalFromDurationKindAndDotsNumber (
  msrDurationKind durationKind,
  int             dotsNumber);

EXP rational msrDurationKindAsWholeNotes (
  msrDurationKind durationKind);

EXP msrDurationKind wholeNotesAsDurationKind (rational wholeNotes);

string msrDurationKindAsMusicXMLType (msrDurationKind durationKind);

string msrDurationKindAsString (msrDurationKind durationKind);

// whole notes
//______________________________________________________________________________
string wholeNotesAsMsrString (
  int      inputLineNumber,
  rational wholeNotes,
  int&     dotsNumber);

string wholeNotesAsMsrString (
  int      inputLineNumber,
  rational wholeNotes);

string multipleRestMeasuresWholeNotesAsMsrString (
  int      inputLineNumber, // JMI
  rational wholeNotes);

// dotted durations
//______________________________________________________________________________
class EXP msrDottedDuration
{
// JMI  protected:
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrDottedDuration ();

                          msrDottedDuration (
                            msrDurationKind durationKind,
                            int             dotsNumber);

    virtual               ~msrDottedDuration ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setDurationKind (msrDurationKind durationKind)
                              { fDurationKind = durationKind; }

    msrDurationKind       getDurationKind () const
                              { return fDurationKind; }

    void                  setDotsNumber (int dotsNumber)
                              { fDotsNumber = dotsNumber; }

    int                   getDotsNumber () const
                              { return fDotsNumber; }

  public:

    // public services
    // ------------------------------------------------------

    void                  incrDotsNumber ()
                              { ++fDotsNumber; }

    rational              dottedDurationAsWholeNotes_FOR_TEMPO (
                            int inputLineNumber) const;

    rational              dottedDurationAsWholeNotes (
                            int inputLineNumber) const;

  public:

    // visitors
    // ------------------------------------------------------

/* JMI
    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;
*/

  public:

    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    msrDurationKind       fDurationKind;
    int                   fDotsNumber;
};
EXP ostream& operator<< (ostream& os, const msrDottedDuration& elt);

// semitone pitches and octave
// can be used as absolute, relative or fixed reference
//______________________________________________________________________________
class EXP msrSemiTonesPitchAndOctave : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSemiTonesPitchAndOctave> create (
                            msrSemiTonesPitchKind semiTonesPitchKind,
                            msrOctaveKind         octaveKind);

    SMARTP<msrSemiTonesPitchAndOctave> createSemiTonesPitchAndOctaveNewbornClone ();

    // creation from  a string
    // ------------------------------------------------------

    static SMARTP<msrSemiTonesPitchAndOctave> createFromString (
                            int    inputLineNumber,
                            const string& theString);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSemiTonesPitchAndOctave (
                            msrSemiTonesPitchKind semiTonesPitchKind,
                            msrOctaveKind         octaveKind);

    virtual               ~msrSemiTonesPitchAndOctave ();

  public:

    // set and get
    // ------------------------------------------------------

    msrSemiTonesPitchKind getSemiTonesPitchKind () const
                              { return fSemiTonesPitchKind; }

    void                  incrementOctaveKind ();
    void                  decrementOctaveKind ();

    msrOctaveKind         getOctaveKind () const
                              { return fOctaveKind; }

  public:

    // public services
    // ------------------------------------------------------

    string                asString () const;

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    msrSemiTonesPitchKind fSemiTonesPitchKind;
    msrOctaveKind         fOctaveKind;
};
typedef SMARTP<msrSemiTonesPitchAndOctave> S_msrSemiTonesPitchAndOctave;
EXP ostream& operator<< (ostream& os, const S_msrSemiTonesPitchAndOctave& elt);

// semitone pitches and octave
// can be used as absolute, relative or fixed reference
//______________________________________________________________________________
class EXP msrQuarterTonesPitchAndOctave : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrQuarterTonesPitchAndOctave> create (
                            msrQuarterTonesPitchKind quarterTonesPitchKind,
                            msrOctaveKind            octaveKind);

    SMARTP<msrQuarterTonesPitchAndOctave> createQuarterTonesPitchAndOctaveNewbornClone ();

    // creation from  a string
    // ------------------------------------------------------

    static SMARTP<msrQuarterTonesPitchAndOctave> createFromString (
                            int    inputLineNumber,
                            const string& theString);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrQuarterTonesPitchAndOctave (
                            msrQuarterTonesPitchKind quarterTonesPitchKind,
                            msrOctaveKind            octaveKind);

    virtual               ~msrQuarterTonesPitchAndOctave ();

  public:

    // set and get
    // ------------------------------------------------------

    msrQuarterTonesPitchKind
                          getQuarterTonesPitchKind () const
                              { return fQuarterTonesPitchKind; }

    void                  incrementOctaveKind ();
    void                  decrementOctaveKind ();

    msrOctaveKind         getOctaveKind () const
                              { return fOctaveKind; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    msrQuarterTonesPitchKind
                          fQuarterTonesPitchKind;
    msrOctaveKind         fOctaveKind;
};
typedef SMARTP<msrQuarterTonesPitchAndOctave> S_msrQuarterTonesPitchAndOctave;
EXP ostream& operator<< (ostream& os, const S_msrQuarterTonesPitchAndOctave& elt);

// length units
//______________________________________________________________________________

enum class msrLengthUnitKind {
  kUnitInch, kUnitCentimeter, kUnitMillimeter
};

string msrLengthUnitKindAsString (
  msrLengthUnitKind lengthUnitKind);

string existingMsrLengthUnitKinds (unsigned int namesListMaxLength);

extern map<string, msrLengthUnitKind>
  gGlobalMsrLengthUnitKindsMap;

void initializeMsrLengthUnitKindsMap ();

// lengths
//______________________________________________________________________________

class EXP msrLength : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrLength> create (
                            msrLengthUnitKind lengthUnitKind,
                            float             lengthValue);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrLength (
                            msrLengthUnitKind lengthUnitKind,
                            float             lengthValue);

                          msrLength ();

    virtual               ~msrLength ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setLengthUnitKind (msrLengthUnitKind value)
                              { fLengthUnitKind = value; }

    msrLengthUnitKind     getLengthUnitKind () const
                              { return fLengthUnitKind; }

    void                  setLengthValue (float value)
                              { fLengthValue = value; }

    float                 getLengthValue () const
                              { return fLengthValue; };

  public:

    // public services
    // ------------------------------------------------------

    bool                  operator== (const msrLength& other) const
                              {
                                // JMI convert to same length unit kind before comparing BLARK
                                return
                                  fLengthUnitKind == other.fLengthUnitKind
                                   &&
                                  fLengthValue == other.fLengthValue;
                              }

    bool                  operator!= (const msrLength& other) const
                              { return ! ((*this) == other); }

    void                  convertToLengthUnit (
                            msrLengthUnitKind lengthUnitKind);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    msrLengthUnitKind     fLengthUnitKind;

    float                 fLengthValue;
};
typedef SMARTP<msrLength> S_msrLength;
EXP ostream& operator<< (ostream& os, const msrLength& elt);
EXP ostream& operator<< (ostream& os, const S_msrLength& elt);

// margins types
//______________________________________________________________________________

enum class msrMarginTypeKind {
  kMarginOdd, kMarginEven, kMarginBoth
};

string msrMarginTypeKindAsString (
  msrMarginTypeKind marginTypeKind);

string existingMsrMarginTypeKinds (unsigned int namesListMaxLength);

extern map<string, msrMarginTypeKind>
  gGlobalMsrMarginTypeKindsMap;

void initializeMsrMarginTypeKindsMap ();

// margins
//______________________________________________________________________________

class EXP msrMargin : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrMargin> create (
                            msrMarginTypeKind marginTypeKind,
                            msrLength         marginLength);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMargin (
                            msrMarginTypeKind marginTypeKind,
                            msrLength         marginLength);

                          msrMargin ();

    virtual               ~msrMargin ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMarginTypeKind (msrMarginTypeKind& value)
                              { fMarginTypeKind = value; }

    msrMarginTypeKind     getMarginTypeKind () const
                              { return fMarginTypeKind; }

    void                  setMarginLength (msrLength& value)
                              { fMarginLength = value; }

    msrLength             getMarginLength () const
                              { return fMarginLength; };

  public:

    // public services
    // ------------------------------------------------------

    bool                  operator== (
                            const msrMargin& other) const
                              {
                                return
                                  fMarginTypeKind == other.fMarginTypeKind
                                   &&
                                  fMarginLength == other.fMarginLength;
                              }

    bool                  operator!= (
                            const msrMargin& other) const
                              { return ! ((*this) == other); }

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------


    msrMarginTypeKind     fMarginTypeKind;
    msrLength             fMarginLength;
};
typedef SMARTP<msrMargin> S_msrMargin;
EXP ostream& operator<< (ostream& os, const S_msrMargin& elt);

// margins groups
//______________________________________________________________________________

class EXP msrMarginsGroup : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrMarginsGroup> create (
                            msrMarginTypeKind marginTypeKind);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMarginsGroup (
                            msrMarginTypeKind marginTypeKind);

                          msrMarginsGroup ();

    virtual               ~msrMarginsGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // margins type
    msrMarginTypeKind     getMarginsGroupTypeKind () const
                              { return fMarginsGroupTypeKind; }

    // margins
    void                  setLeftMargin (
                            int         inputLineNumber,
                            S_msrMargin val);

    S_msrMargin           getLeftMargin () const
                              { return fLeftMargin; }

    void                  setRightMargin (
                            int         inputLineNumber,
                            S_msrMargin val);

    S_msrMargin           getRightMargin () const
                              { return fRightMargin; }

    void                  setTopMargin (
                            int         inputLineNumber,
                            S_msrMargin val);

    S_msrMargin           getTopMargin () const
                              { return fTopMargin; }

    void                  setBottomMargin (
                            int         inputLineNumber,
                            S_msrMargin val);

    S_msrMargin           getBottomMargin () const
                              { return fBottomMargin; }

  public:

    // public services
    // ------------------------------------------------------

  public:

/* JMI
    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;
*/

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    // margins type
    msrMarginTypeKind     fMarginsGroupTypeKind;

    // margins
    S_msrMargin           fLeftMargin;
    S_msrMargin           fRightMargin;
    S_msrMargin           fTopMargin;
    S_msrMargin           fBottomMargin;
};
typedef SMARTP<msrMarginsGroup> S_msrMarginsGroup;
EXP ostream& operator<< (ostream& os, const S_msrMarginsGroup& elt);

// fonts
//______________________________________________________________________________
// data types
// ------------------------------------------------------

enum class msrFontSizeKind {
  kFontSizeNone,
  kFontSizeXXSmall, kFontSizeXSmall, kFontSizeSmall,
  kFontSizeMedium,
  kFontSizeLarge, kFontSizeXLarge, kFontSizeXXLarge,
  kFontSizeNumeric
};

string msrFontSizeKindAsString (
  msrFontSizeKind fontSizeKind);

class EXP msrFontSize : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFontSize> create (
                            msrFontSizeKind fontSizeKind);

    static SMARTP<msrFontSize> create (
                            float fontNumericSize);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrFontSize (
                            msrFontSizeKind fontSizeKind);

                          msrFontSize (
                            float numericFontSize);

    virtual               ~msrFontSize ();

  public:

    // set and get
    // ------------------------------------------------------

    msrFontSizeKind       getFontSizeKind () const
                              { return fFontSizeKind; }

    float                 getFontNumericSize () const;

  public:

    // public services
    // ------------------------------------------------------

    string                fontSizeAsString () const;

  public:

    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    msrFontSizeKind       fFontSizeKind;

    float                 fFontNumericSize;
                            // only relevant when
                            // fFontSizeKind == kNumericFontSize
};
typedef SMARTP<msrFontSize> S_msrFontSize;
EXP ostream& operator<< (ostream& os, const S_msrFontSize& elt);

enum class msrFontStyleKind {
  kFontStyleNone,
  kFontStyleNormal, KFontStyleItalic
};

msrFontStyleKind msrFontStyleKindFromString (
  int    inputLineNumber,
  string fontStyleString);

string msrFontStyleKindAsString (
  msrFontStyleKind fontStyleKind);

enum class msrFontWeightKind {
  kFontWeightNone,
  kFontWeightNormal, kFontWeightBold
};

msrFontWeightKind msrFontWeightKindFromString (
  int    inputLineNumber,
  string fontWeightString);

string msrFontWeightKindAsString (
  msrFontWeightKind fontWeightKind);

// alignement
//______________________________________________________________________________
/*
  The justify entity is used to indicate left, center, or
  right justification. The default value varies for different
  elements. For elements where the justify attribute is present
  but the halign attribute is not, the justify attribute
  indicates horizontal alignment as well as justification.
*/

enum class msrJustifyKind {
  kJustifyNone,
  kJustifyLeft, kJustifyCenter, kJustifyRight
};

msrJustifyKind msrJustifyKindFromString (
  int    inputLineNumber,
  string justifyString);

string msrJustifyKindAsString (
  msrJustifyKind justifyKind);

enum class msrHorizontalAlignmentKind {
  kHorizontalAlignmentNone,
  kHorizontalAlignmentLeft, kHorizontalAlignmentCenter, kHorizontalAlignmentRight
};

msrHorizontalAlignmentKind msrHorizontalAlignmentKindFromString (
  int    inputLineNumber,
  string horizontalAlignString);

string msrHorizontalAlignmentKindAsString (
  msrHorizontalAlignmentKind horizontalAlignmentKind);

enum class msrVerticalAlignmentKind {
  kVerticalAlignmentNone,
  kVerticalAlignmentTop, kVerticalAlignmentMiddle, kVerticalAlignmentBottom
};

msrVerticalAlignmentKind msrVerticalAlignmentKindFromString (
  int    inputLineNumber,
  string verticalAlignmentString);

string msrVerticalAlignmentKindAsString (
  msrVerticalAlignmentKind verticalAlignmentKind);

// direction
//______________________________________________________________________________
enum class msrDirectionKind {
  kDirectionNone,
  kDirectionUp, kDirectionDown
};

string msrDirectionKindAsString (
  msrDirectionKind directionKind);

// print object
//______________________________________________________________________________
enum class msrPrintObjectKind {
  kPrintObjectNone,
  kPrintObjectYes,
  kPrintObjectNo
};

msrPrintObjectKind msrPrintObjectKindFromString (
  int    inputLineNumber,
  string printObjectString);

string msrPrintObjectKindAsString (
  msrPrintObjectKind printObjectKind);

// placement
//______________________________________________________________________________
enum class msrPlacementKind {
  k_NoPlacement,

  kPlacementAbove, kPlacementBelow
};

msrPlacementKind msrPlacementKindFromString (
  int    inputLineNumber,
  string placementString);

string msrPlacementKindAsString (
  msrPlacementKind placementKind);

// dynamics
// ------------------------------------------------------

enum class msrDynamicsKind {
  k_NoDynamics,

  kDynamicsF, kDynamicsFF, kDynamicsFFF,
  kDynamicsFFFF, kDynamicsFFFFF, kDynamicsFFFFFF,

  kDynamicsP, kDynamicsPP, kDynamicsPPP, kDynamicsPPPP,
  kDynamicsPPPPP, kDynamicsPPPPPP,

  kDynamicsMF, kDynamicsMP, kDynamicsFP, kDynamicsFZ, kDynamicsRF,
  kDynamicsSF, kDynamicsRFZ, kDynamicsSFZ, kDynamicsSFP,
  kDynamicsSFPP, kDynamicsSFFZ
};

string msrDynamicsKindAsString (
  msrDynamicsKind dynamicsKind);

msrDynamicsKind dynamicsFromString (string theString);

// measure style
//______________________________________________________________________________
enum class msrSlashTypeKind {
  k_NoSlashType,

  kSlashTypeStart, kSlashTypeStop
};

string msrSlashTypeKindAsString (
  msrSlashTypeKind slashTypeKind);

enum class msrUseDotsKind {
  k_NoUseDots,

  kUseDotsYes, kUseDotsNo
};

msrUseDotsKind msrUseDotsFromString (
  int    inputLineNumber,
  string useDotsString);

string msrUseDotsKindAsString (
  msrUseDotsKind useDotsKind);

enum class msrSlashUseStemsKind {
  k_NoSlashUseStems,

  kSlashUseStemsYes, kSlashUseStemsNo
};

string msrSlashUseStemsKindAsString (
  msrSlashUseStemsKind slashUseStemsKind);

// line types
//______________________________________________________________________________
enum class msrLineTypeKind {
  kLineTypeSolid, kLineTypeDashed,
  kLineTypeDotted, kLineTypeWavy
};

string msrLineTypeKindAsString (
  msrLineTypeKind lineTypeKind);

// tremolo types
//______________________________________________________________________________
enum class msrTremoloTypeKind {
  k_NoTremoloType,

  kTremoloTypeSingle, kTremoloTypeStart, kTremoloTypeStop
};

string msrTremoloTypeKindAsString (
  msrTremoloTypeKind tremoloTypeKind);

// technical types
//______________________________________________________________________________
enum class msrTechnicalTypeKind { // JMI ???
  k_NoTechnicalType,

  kTechnicalTypeStart, kTechnicalTypeStop
};

string msrTechnicalTypeKindAsString (
  msrTechnicalTypeKind technicalTypeKind);

// spanner types
//______________________________________________________________________________
enum class msrSpannerTypeKind {
  k_NoSpannerType,

  kSpannerTypeStart, kSpannerTypeContinue, kSpannerTypeStop
};

string msrSpannerTypeKindAsString (
  msrSpannerTypeKind spannerTypeKind);

// moments
//______________________________________________________________________________
class EXP msrMoment
{
  public:

    // constants
    // ------------------------------------------------------

    #define K_NO_POSITION rational (-22, 1)

    #define K_NO_MOMENT   msrMoment (K_NO_POSITION, K_NO_POSITION)

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMoment ();

                          msrMoment (
                            rational writtenPositionInMeseasur,
                            rational soundingRelativeOffset);

                          msrMoment (
                            rational writtenPositionInMeseasur);

    virtual               ~msrMoment ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setWrittenPositionInMeseasure (rational position)
                              { fWrittenPositionInMeseasure = position; }

    rational              getWrittenPositionInMeseasure () const
                              { return fWrittenPositionInMeseasure; }

    void                  setSoundingRelativeOffset (rational offset)
                              { fSoundingRelativeOffset = offset; }

    rational              getSoundingRelativeOffset () const
                              { return fSoundingRelativeOffset; }

  public:

    // public services
    // ------------------------------------------------------

    bool                  operator== (const msrMoment& other) const;

    bool                  operator!= (const msrMoment& other) const
                              { return ! ((*this) == other); }

    bool                  operator< (const msrMoment& other) const;

    bool                  operator>= (const msrMoment& other) const
                              { return ! ((*this) < other); }

    bool                  operator> (const msrMoment& other) const;

    bool                  operator<= (const msrMoment& other) const
                              { return ! ((*this) > other); }

    static void           testMsrMomentComparisons (ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

/* JMI
    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;
*/

  public:

    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    rational              fWrittenPositionInMeseasure;
    rational              fSoundingRelativeOffset;
};
EXP ostream& operator<< (ostream& os, const msrMoment& elt);

// tuplet factors
//______________________________________________________________________________
class EXP msrTupletFactor
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTupletFactor ();

                          msrTupletFactor (
                            int tupletActualNotes,
                            int tupletNormalNotes);

                          msrTupletFactor (
                            rational rationalTupletFactor);

    virtual               ~msrTupletFactor ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setTupletActualNotes (int actualNotes)
                              { fTupletActualNotes = actualNotes; }

    int                   getTupletActualNotes () const
                              { return fTupletActualNotes; }

    void                  setTupletNormalNotes (int normalNotes)
                              { fTupletNormalNotes = normalNotes; }

    int                   getTupletNormalNotes () const
                              { return fTupletNormalNotes; }

  public:

    // public services
    // ------------------------------------------------------

    bool                  isEqualToOne () const
                              {
                                return
                                  fTupletActualNotes == fTupletNormalNotes;
                              }

    rational              asRational () const
                            {
                              return
                                rational (
                                  fTupletActualNotes,
                                  fTupletNormalNotes);
                            }

  public:

    // visitors
    // ------------------------------------------------------

/* JMI
    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;
*/

  public:

    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fTupletActualNotes;
    int                   fTupletNormalNotes;
};
EXP ostream& operator<< (ostream& os, const msrTupletFactor& elt);

// harmonies intervals
//______________________________________________________________________________
class msrHarmonyInterval;
typedef SMARTP<msrHarmonyInterval> S_msrHarmonyInterval;

class EXP msrHarmonyInterval : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrHarmonyInterval> create (
                            msrIntervalKind harmonyIntervalIntervalKind,
                            int             harmonyIntervalRelativeOctave = 0);
                                              // 0: up to the thirteenth,
                                              // no relative octave is needed

    SMARTP<msrHarmonyInterval> createHarmonyIntervalNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrHarmonyInterval (
                            msrIntervalKind harmonyIntervalIntervalKind,
                            int             harmonyIntervalRelativeOctave);

    virtual               ~msrHarmonyInterval ();

  public:

    // set and get
    // ------------------------------------------------------

    msrIntervalKind       getHarmonyIntervalIntervalKind () const
                              { return fHarmonyIntervalIntervalKind; }

    void                  incrementHarmonyIntervalRelativeOctave ()
                              { ++fHarmonyIntervalRelativeOctave; }

    void                  decrementHarmonyIntervalRelativeOctave ()
                              { --fHarmonyIntervalRelativeOctave; }

    int                   getHarmonyIntervalRelativeOctave () const
                              { return fHarmonyIntervalRelativeOctave; }

  public:

    // public services
    // ------------------------------------------------------

    int                   harmonyIntervalAsSemitones () const
                              {
                                return
                                  msrIntervalKindAsSemiTones (
                                    fHarmonyIntervalIntervalKind);
                              }

    string                harmonyIntervalAsString () const;
    string                harmonyIntervalAsShortString () const;

    /*
      we handle intervals up to the thirteenth in harmonies,
      but operations on intervals are easier to compute
      in a normal form limited to an octave, hence:
    */
    void                  normalizeInterval ();
    void                  deNormalizeInterval ();

    S_msrHarmonyInterval    intervalDifference (
                            S_msrHarmonyInterval otherHarmonyInterval);
    S_msrHarmonyInterval    intervalSum (
                            S_msrHarmonyInterval otherHarmonyInterval);

    string                asString () const;
    string                asShortString () const;

  public:

    // visitors
    // ------------------------------------------------------

/* JMI
    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;
*/

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    msrIntervalKind       fHarmonyIntervalIntervalKind;

    int                   fHarmonyIntervalRelativeOctave;
};
typedef SMARTP<msrHarmonyInterval> S_msrHarmonyInterval;
EXP ostream& operator<< (ostream& os, const S_msrHarmonyInterval& elt);

// harmonies structure
//______________________________________________________________________________
class msrHarmonyStructure;
typedef SMARTP<msrHarmonyStructure> S_msrHarmonyStructure;

class EXP msrHarmonyStructure : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrHarmonyStructure> createBare (
                            msrHarmonyKind harmonyStructureHarmonyKind);

    static SMARTP<msrHarmonyStructure> create (
                            msrHarmonyKind harmonyStructureHarmonyKind);

    SMARTP<msrHarmonyStructure> createHarmonyStructureNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrHarmonyStructure (
                            msrHarmonyKind harmonyStructureHarmonyKind);

    virtual               ~msrHarmonyStructure ();

  private:

    // private initialization
    // ------------------------------------------------------

  public:

    // set and get
    // ------------------------------------------------------

    msrHarmonyKind        getHarmonyStructureHarmonyKind () const
                              { return fHarmonyStructureHarmonyKind; }

    const vector <S_msrHarmonyInterval>&
                          getHarmonyStructureIntervals () const
                              { return fHarmonyStructureIntervals; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendHarmonyIntervalToHarmonyStructure (
                            S_msrHarmonyInterval harmonyInterval);

    void                  populateHarmonyStructure ();

    string                harmonyStructureAsString () const;
    string                harmonyStructureAsShortString () const;

    S_msrHarmonyInterval    bassHarmonyIntervalForHarmonyInversion (
                            int inputLineNumber,
                            int inversionNumber);

    S_msrHarmonyStructure   invertHarmonyStructure (int inversion);

    static void           printAllHarmoniesStructures (ostream& os);

  public:

    // visitors
    // ------------------------------------------------------
/* JMI
    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;
*/

  public:

    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    msrHarmonyKind        fHarmonyStructureHarmonyKind;
    vector<S_msrHarmonyInterval>
                          fHarmonyStructureIntervals;
};
typedef SMARTP<msrHarmonyStructure> S_msrHarmonyStructure;
EXP ostream& operator<< (ostream& os, const S_msrHarmonyStructure& elt);

// harmonies contents
//______________________________________________________________________________
class EXP msrHarmonyContents : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrHarmonyContents> create (
                       // JMI     int                   inputLineNumber,
                            msrSemiTonesPitchKind harmonyContentsRootNote,
                            msrHarmonyKind        harmonyContentsHarmonyKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrHarmonyContents (
                       // JMI     int                   inputLineNumber,
                            msrSemiTonesPitchKind harmonyContentsRootNote,
                            msrHarmonyKind        harmonyContentsHarmonyKind);

    virtual               ~msrHarmonyContents ();

  public:

    // set and get
    // ------------------------------------------------------

    msrSemiTonesPitchKind
                          getHarmonyContentsRootNote () const
                              { return fHarmonyContentsRootNote; }

    msrHarmonyKind        getHarmonyContentsHarmonyKind () const
                              { return fHarmonyContentsHarmonyKind; }

    const vector<S_msrSemiTonesPitchAndOctave>&
                          getHarmonyElementsVector () const
                              { return fHarmonyElementsVector; }

  public:

    // public services
    // ------------------------------------------------------

    string                harmonyContentsAsString () const;
    string                harmonyContentsAsShortString () const;

    msrSemiTonesPitchKind bassSemiTonesPitchKindForHarmonyInversion (
                            int inputLineNumber,
                            int inversionNumber);

    static void           printAllHarmoniesContents (
                            ostream&              os,
                            msrSemiTonesPitchKind semiTonesPitchKind);

  public:

    // visitors
    // ------------------------------------------------------
/* JMI
    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;
*/

  public:

    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    msrSemiTonesPitchKind fHarmonyContentsRootNote;
    msrHarmonyKind        fHarmonyContentsHarmonyKind;

    vector<S_msrSemiTonesPitchAndOctave>
                          fHarmonyElementsVector;
};
typedef SMARTP<msrHarmonyContents> S_msrHarmonyContents;
EXP ostream& operator<< (ostream& os, const S_msrHarmonyContents& elt);

// harmonies details and analysis
//______________________________________________________________________________
void printHarmonyDetails (
  ostream&              os,
  msrSemiTonesPitchKind rootSemiTonesPitchKind,
  msrHarmonyKind        harmonyKind);

void printHarmonyAnalysis (
  ostream&              os,
  msrSemiTonesPitchKind rootSemiTonesPitchKind,
  msrHarmonyKind        harmonyKind,
  int                   inversion);

// RGB colors
//______________________________________________________________________________
class EXP msrRGBColor
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrRGBColor ();

                          msrRGBColor (
                            float theR,
                            float theG,
                            float theB);

                          msrRGBColor (
                            const string& theString);

    // set and get
    // ------------------------------------------------------

    float                 getR () const
                              { return fR; }

    float                 getG () const
                              { return fG; }

    float                 getB () const
                              { return fB; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    string                asString (int precision = 3) const;

    string                asSpaceSeparatedString (int precision = 3) const;

    void                  print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    float                 fR;
    float                 fG;
    float                 fB;
};
EXP ostream& operator<< (ostream& os, const msrRGBColor& elt);

// AlphaRGB colors
//______________________________________________________________________________
class EXP msrAlphaRGBColor
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrAlphaRGBColor (
                            string colorRGB,
                            string colorAlpha);

                          msrAlphaRGBColor (
                            string colorRGB);

    virtual               ~msrAlphaRGBColor ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getColorRGB () const
                              { return fColorRGB; }

    string                getColorAlpha () const
                              { return fColorAlpha; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    string                fColorRGB;   // hexadecimal, 6 digits
    string                fColorAlpha; // hexadecimal, 2 digits
};
EXP ostream& operator<< (ostream& os, const msrAlphaRGBColor& elt);

// score notation kinds
//______________________________________________________________________________
enum class msrScoreNotationKind {
  kScoreNotationWestern,
  kScoreNotationJianpu,
  kScoreNotationDiatonicAccordion
};

string msrScoreNotationKindAsString (
  msrScoreNotationKind lilypondScoreNotationKind);

// global variables
//______________________________________________________________________________
extern map<msrHarmonyKind, S_msrHarmonyStructure>
  gGlobalHarmonyStructuresMap;

void initializeHarmonyStructuresMap ();

void printHarmonyStructuresMap ();

extern map<string, msrQuarterTonesPitchesLanguageKind>
  gGlobalQuarterTonesPitchesLanguageKindsMap;

extern map<msrQuarterTonesPitchKind, string> gGlobalNederlandsPitchesNamesMap;
extern map<msrQuarterTonesPitchKind, string> gGlobalCatalanPitchesNamesMap;
extern map<msrQuarterTonesPitchKind, string> gGlobalDeutschPitchesNamesMap;
extern map<msrQuarterTonesPitchKind, string> gGlobalEnglishPitchesNamesMap;
extern map<msrQuarterTonesPitchKind, string> gGlobalEspanolPitchesNamesMap;
extern map<msrQuarterTonesPitchKind, string> gGlobalFrancaisPitchesNamesMap;
extern map<msrQuarterTonesPitchKind, string> gGlobalItalianoPitchesNamesMap;
extern map<msrQuarterTonesPitchKind, string> gGlobalNorskPitchesNamesMap;
extern map<msrQuarterTonesPitchKind, string> gGlobalPortuguesPitchesNamesMap;
extern map<msrQuarterTonesPitchKind, string> gGlobalSuomiPitchesNamesMap;
extern map<msrQuarterTonesPitchKind, string> gGlobalSvenskaPitchesNamesMap;
extern map<msrQuarterTonesPitchKind, string> gGlobalVlaamsPitchesNamesMap;
extern map<msrQuarterTonesPitchKind, string> gGlobalArabicPitchesNamesMap;

void initializeQuarterTonesPitchesLanguageKinds ();

// the reference for note pitches names is LilyPond's scm/define-note-names.scm
void initializeNederlandsPitchNamesMap ();
void initializeCatalanPitchNamesMap ();
void initializeDeutschPitchNamesMap ();
void initializeEnglishPitchNamesMap ();
void initializeEspanolPitchNamesMap ();
void initializeFrancaisPitchNamesMap ();
void initializeItalianoPitchNamesMap ();
void initializeNorskPitchNamesMap ();
void initializePortuguesPitchNamesMap ();
void initializeSuomiPitchNamesMap ();
void initializeSvenskaPitchNamesMap ();
void initializeVlaamsPitchNamesMap ();
void initializeArabicPitchNamesMap ();

string existingQuarterTonesPitchesLanguageKinds (unsigned int namesListMaxLength);

// initialization
//______________________________________________________________________________
void initializeMSRBasicTypes ();


} // namespace MusicXML2


#endif
