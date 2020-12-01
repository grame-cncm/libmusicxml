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
enum msrXMLLangKind {
  kXMLLangIt, kXMLLangEn, kXMLLangDe, kXMLLangFr, kXMLLangJa, kXMLLangLa };

EXP msrXMLLangKind msrXMLLangKindFromString (
  int    inputLineNumber,
  string XMLLangString);

EXP string msrXMLLangKindAsString (
  msrXMLLangKind XMLLangKind);

// diatonic pitches
//______________________________________________________________________________
enum msrDiatonicPitchKind {
  // starting at C for LilyPond relative octave calculations
  kC, kD, kE, kF, kG, kA, kB,
  k_NoDiatonicPitch };

EXP msrDiatonicPitchKind msrDiatonicPitchKindFromString (
  char diatonicNoteName);

EXP string msrDiatonicPitchKindAsString (
  msrDiatonicPitchKind diatonicPitchKind);

// alterations
//______________________________________________________________________________
enum msrAlterationKind {
  k_NoAlteration,

  kTripleFlat, kDoubleFlat, kSesquiFlat, kFlat, kSemiFlat,
  kNatural,
  kSemiSharp, kSharp, kSesquiSharp, kDoubleSharp, kTripleSharp };

EXP msrAlterationKind msrAlterationKindFromMusicXMLAlter (
  float alter);

EXP float msrMusicXMLAlterFromAlterationKind (
  msrAlterationKind alterationKind);

EXP string msrAlterationKindAsString (
  msrAlterationKind alterationKind);

// accidentals
//______________________________________________________________________________
enum msrAccidentalKind {
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
  kAccidentalSlashFlat,kAccidentalDoubleSlashFlat,
  kAccidentalSharp_1, kAccidentalSharp_2,
  kAccidentalSharp_3,  kAccidentalSharp_5,
  kAccidentalFlat_1, kAccidentalFlat_2,
  kAccidentalFlat_3, kAccidentalFlat_4,
  kAccidentalSori, kAccidentalKoron,

  kAccidentalOther};

EXP string accidentalKindAsString (
  msrAccidentalKind accidentalKind);

EXP string accidentalKindAsMusicXMLString (
  msrAccidentalKind accidentalKind);

// editorial accidentals
//______________________________________________________________________________

enum msrEditorialAccidentalKind {
  kEditorialAccidentalYes, kEditorialAccidentalNo };

EXP string editorialAccidentalKindAsString (
  msrEditorialAccidentalKind noteEditorialAccidentalKind);

// cautionary accidentals
//______________________________________________________________________________

enum msrCautionaryAccidentalKind {
  kCautionaryAccidentalYes, kCautionaryAccidentalNo };

EXP string cautionaryAccidentalKindAsString (
  msrCautionaryAccidentalKind noteCautionaryAccidentalKind);

// semi tones pitches
//______________________________________________________________________________
enum msrSemiTonesPitchKind {
  k_NoSemiTonesPitch_STP,

  kC_TripleFlat_STP,
  kC_DoubleFlat_STP, kC_Flat_STP,
  kC_Natural_STP,
  kC_Sharp_STP, kC_DoubleSharp_STP,
  kC_TripleSharp_STP,

  kD_TripleFlat_STP,
  kD_DoubleFlat_STP, kD_Flat_STP,
  kD_Natural_STP,
  kD_Sharp_STP, kD_DoubleSharp_STP,
  kD_TripleSharp_STP,

  kE_TripleFlat_STP,
  kE_DoubleFlat_STP, kE_Flat_STP,
  kE_Natural_STP,
  kE_Sharp_STP, kE_DoubleSharp_STP,
  kE_TripleSharp_STP,

  kF_TripleFlat_STP,
  kF_DoubleFlat_STP, kF_Flat_STP,
  kF_Natural_STP,
  kF_Sharp_STP, kF_DoubleSharp_STP,
  kF_TripleSharp_STP,

  kG_TripleFlat_STP,
  kG_DoubleFlat_STP, kG_Flat_STP,
  kG_Natural_STP,
  kG_Sharp_STP, kG_DoubleSharp_STP,
  kG_TripleSharp_STP,

  kA_TripleFlat_STP,
  kA_DoubleFlat_STP, kA_Flat_STP,
  kA_Natural_STP,
  kA_Sharp_STP, kA_DoubleSharp_STP,
  kA_TripleSharp_STP,

  kB_TripleFlat_STP,
  kB_DoubleFlat_STP, kB_Flat_STP,
  kB_Natural_STP,
  kB_Sharp_STP, kB_DoubleSharp_STP,
  kB_TripleSharp_STP };

EXP string msrSemiTonesPitchKindAsString (
  msrSemiTonesPitchKind semiTonesPitchKind);

// quarter tones pitches
//______________________________________________________________________________
enum msrQuarterTonesPitchKind {
  k_NoQuarterTonesPitch_QTP,

  k_Rest_QTP,

  kA_TripleFlat_QTP,
  kA_DoubleFlat_QTP, kA_SesquiFlat_QTP, kA_Flat_QTP, kA_SemiFlat_QTP,
  kA_Natural_QTP,
  kA_SemiSharp_QTP, kA_Sharp_QTP, kA_SesquiSharp_QTP, kA_DoubleSharp_QTP,
  kA_TripleSharp_QTP,

  kB_TripleFlat_QTP,
  kB_DoubleFlat_QTP, kB_SesquiFlat_QTP, kB_Flat_QTP, kB_SemiFlat_QTP,
  kB_Natural_QTP,
  kB_SemiSharp_QTP, kB_Sharp_QTP, kB_SesquiSharp_QTP, kB_DoubleSharp_QTP,
  kB_TripleSharp_QTP,

  kC_TripleFlat_QTP,
  kC_DoubleFlat_QTP, kC_SesquiFlat_QTP, kC_Flat_QTP, kC_SemiFlat_QTP,
  kC_Natural_QTP,
  kC_SemiSharp_QTP, kC_Sharp_QTP, kC_SesquiSharp_QTP, kC_DoubleSharp_QTP,
  kC_TripleSharp_QTP,

  kD_TripleFlat_QTP,
  kD_DoubleFlat_QTP, kD_SesquiFlat_QTP, kD_Flat_QTP, kD_SemiFlat_QTP,
  kD_Natural_QTP,
  kD_SemiSharp_QTP, kD_Sharp_QTP, kD_SesquiSharp_QTP, kD_DoubleSharp_QTP,
  kD_TripleSharp_QTP,

  kE_TripleFlat_QTP,
  kE_DoubleFlat_QTP, kE_SesquiFlat_QTP, kE_Flat_QTP, kE_SemiFlat_QTP,
  kE_Natural_QTP,
  kE_SemiSharp_QTP, kE_Sharp_QTP, kE_SesquiSharp_QTP, kE_DoubleSharp_QTP,
  kE_TripleSharp_QTP,

  kF_TripleFlat_QTP,
  kF_DoubleFlat_QTP, kF_SesquiFlat_QTP, kF_Flat_QTP, kF_SemiFlat_QTP,
  kF_Natural_QTP,
  kF_SemiSharp_QTP, kF_Sharp_QTP, kF_SesquiSharp_QTP, kF_DoubleSharp_QTP,
  kF_TripleSharp_QTP,

  kG_TripleFlat_QTP,
  kG_DoubleFlat_QTP, kG_SesquiFlat_QTP, kG_Flat_QTP, kG_SemiFlat_QTP,
  kG_Natural_QTP,
  kG_SemiSharp_QTP, kG_Sharp_QTP, kG_SesquiSharp_QTP, kG_DoubleSharp_QTP,
  kG_TripleSharp_QTP };

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
  string theString);

// alterations preferences
//______________________________________________________________________________
enum msrAlterationPreferenceKind {
  kPreferFlat, kPreferNatural, kPreferSharp };

EXP msrQuarterTonesPitchKind msrSemiTonesPitchKindAsQuarterTonesPitchKind (
  msrSemiTonesPitchKind       semiTonesPitchKind,
  msrAlterationPreferenceKind alterationPreferenceKind);

// intervals
//______________________________________________________________________________
enum msrIntervalKind {
  k_NoIntervalKind,

  kDiminishedUnisson, kPerfectUnison, kAugmentedUnison,

  kDiminishedSecond, kMinorSecond, kMajorSecond, kAugmentedSecond,

  kDiminishedThird, kMinorThird, kMajorThird, kAugmentedThird,

  kDiminishedFourth, kPerfectFourth, kAugmentedFourth,

  kDiminishedFifth, kPerfectFifth, kAugmentedFifth,

  kDiminishedSixth, kMinorSixth, kMajorSixth, kAugmentedSixth,

  kDiminishedSeventh, kMinorSeventh, kMajorSeventh, kAugmentedSeventh,

  kDiminishedOctave, kPerfectOctave, kAugmentedOctave,

  kDiminishedNinth, kMinorNinth, kMajorNinth, kAugmentedNinth,

  kDiminishedTenth, kMinorTenth, kMajorTenth, kAugmentedTenth,

  kDiminishedEleventh, kPerfectEleventh, kAugmentedEleventh,

  kDiminishedTwelfth, kPerfectTwelfth, kAugmentedTwelfth,

  kDiminishedThirteenth, kMinorThirteenth, kMajorThirteenth, kAugmentedThirteenth };

EXP int msrIntervalKindAsSemiTones (
  msrIntervalKind intervalKind);

EXP int msrIntervalKindAsQuarterTones (
  msrIntervalKind intervalKind);

EXP string msrIntervalKindAsString (
  msrIntervalKind intervaKindl);

EXP string msrIntervalKindAsShortString (
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

// clefs
//______________________________________________________________________________

//______________________________________________________________________________
// data types

enum msrClefKind {
  k_NoClef,
  kTrebleClef,
  kSopranoClef, kMezzoSopranoClef, kAltoClef, kTenorClef, kBaritoneClef, kBassClef,
  kTrebleLine1Clef,
  kTrebleMinus15Clef, kTrebleMinus8Clef, kTreblePlus8Clef, kTreblePlus15Clef,
  kBassMinus15Clef, kBassMinus8Clef, kBassPlus8Clef, kBassPlus15Clef,
  kVarbaritoneClef,
  kTablature4Clef, kTablature5Clef, kTablature6Clef, kTablature7Clef,
  kPercussionClef,
  kJianpuClef };

EXP string clefKindAsString (
  msrClefKind clefKind);

EXP msrClefKind clefKindFromString (string theString);

EXP string existingClefKinds (int namesListMaxLength);
EXP string existingClefKindsNames (int namesListMaxLength);

extern map<string, msrClefKind>
  gGlobalClefKindsMap;

extern list<string>
  gClefKindsNamesList;

void initializeClefKinds ();

// harmonies
//______________________________________________________________________________
enum msrHarmonyKind {
  k_NoHarmony,

  // MusicXML harmonies

  kMajorHarmony, kMinorHarmony,
  kAugmentedHarmony, kDiminishedHarmony,

  kDominantHarmony,
  kMajorSeventhHarmony, kMinorSeventhHarmony,
  kDiminishedSeventhHarmony, kAugmentedSeventhHarmony,
  kHalfDiminishedHarmony,
  kMinorMajorSeventhHarmony,

  kMajorSixthHarmony, kMinorSixthHarmony,

  kDominantNinthHarmony,
  kMajorNinthHarmony, kMinorNinthHarmony,

  kDominantEleventhHarmony, kMajorEleventhHarmony, kMinorEleventhHarmony,

  kDominantThirteenthHarmony, kMajorThirteenthHarmony, kMinorThirteenthHarmony,

  kSuspendedSecondHarmony, kSuspendedFourthHarmony,

  kNeapolitanHarmony, kItalianHarmony, kFrenchHarmony, kGermanHarmony,

  kPedalHarmony, kPowerHarmony, kTristanHarmony,

  // jazz-specific harmonies

  kMinorMajorNinth,                              // -maj9, minmaj9

  kDominantSuspendedFourthHarmony,               // 7sus4, domsus4
  kDominantAugmentedFifthHarmony,                // 7#5, domaug5
  kDominantMinorNinthHarmony,                    // 7b9, dommin9
  kDominantAugmentedNinthDiminishedFifthHarmony, // 7#9b5, domaug9dim5
  kDominantAugmentedNinthAugmentedFifthHarmony,  // 7#9#5, domaug9aug5
  kDominantAugmentedEleventhHarmony,             // 7#11, domaug11

  kMajorSeventhAugmentedEleventhHarmony,         // maj7#11, maj7aug11

  // other

  kOtherHarmony,

  kNoneHarmony };

EXP string msrHarmonyKindAsString (
  msrHarmonyKind harmonyKind);

EXP string msrHarmonyKindAsShortString (
  msrHarmonyKind harmonyKind);

EXP string msrHarmonyKindShortName (
  msrHarmonyKind harmonyKind);

EXP msrHarmonyKind msrHarmonyKindFromString (
  string theString);

EXP string existingHarmonyKinds (int namesListMaxLength);
EXP string existingHarmonyKindsNames (int namesListMaxLength);

// constant
const int K_HARMONY_NO_INVERSION = -1;

EXP extern map<string, msrHarmonyKind>
  gGlobalHarmonyKindsMap;

EXP extern list<string>
  gHarmonyKindsNamesList;

void initializeHarmonyKinds ();

// quarter tones pitches languages
//______________________________________________________________________________
enum msrQuarterTonesPitchesLanguageKind {
  kNederlands, kCatalan, kDeutsch, kEnglish, kEspanol, kFrancais,
  kItaliano, kNorsk, kPortugues, kSuomi, kSvenska, kVlaams };

EXP string msrQuarterTonesPitchesLanguageKindAsString (
  msrQuarterTonesPitchesLanguageKind languageKind);

EXP string msrDiatonicPitchKindAsString ( // JMI
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrDiatonicPitchKind               diatonicPitchKind);

EXP string msrQuarterTonesPitchKindAsString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrQuarterTonesPitchKind           quarterTonesPitchKind);

EXP msrQuarterTonesPitchKind msrQuarterTonesPitchKindFromString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  string                             quarterTonesPitchName);

/* JMI
EXP string msrSemiTonesPitchKindAsString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrSemiTonesPitchKind              semiTonesPitchKind);
  */

EXP string msrSemiTonesPitchKindAsFlatsAndSharps (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrSemiTonesPitchKind              semiTonesPitchKind);

/* JMI
string msrQuarterTonesPitchKindAsFlatsAndSharps (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrQuarterTonesPitchKind           quarterTonesPitchKind);
*/

// enharmonies
//______________________________________________________________________________
EXP msrSemiTonesPitchKind enharmonicSemiTonesPitch (
  msrSemiTonesPitchKind       semiTonesPitchKind,
  msrAlterationPreferenceKind alterationPreferenceKind);

// durations
//______________________________________________________________________________
enum msrDurationKind {
  // from longest to shortest for the algorithms
  kMaxima, kLong, kBreve, kWhole, kHalf,
  kQuarter,
  kEighth, k16th, k32nd, k64th, k128th, k256th, k512th, k1024th,
  k_NoDuration };

EXP msrDurationKind msrDurationKindFromString (
  int    inputLineNumber,
  string durationString);

EXP rational msrDurationKindAsWholeNotes (
  msrDurationKind durationKind);

EXP msrDurationKind wholeNotesAsDurationKind (rational wholeNotes);

EXP string msrDurationKindAsMusicXMLType (msrDurationKind durationKind);

EXP string msrDurationKindAsString (msrDurationKind durationKind);

// whole notes
//______________________________________________________________________________
EXP string wholeNotesAsMsrString (
  int      inputLineNumber,
  rational wholeNotes,
  int&     dotsNumber);

EXP string wholeNotesAsMsrString (
  int      inputLineNumber,
  rational wholeNotes);

EXP string multipleRestMeasuresWholeNotesAsMsrString (
  int      inputLineNumber, // JMI
  rational wholeNotes);

// dotted durations
//______________________________________________________________________________
class msrDottedDuration
{
// JMI  protected:
  public:

    // constructors/destructor
    // ------------------------------------------------------

    msrDottedDuration ();

    msrDottedDuration (
      msrDurationKind durationKind,
      int             dotsNumber);

    virtual ~msrDottedDuration ();

  public:

    // set and get
    // ------------------------------------------------------

    msrDurationKind       getDurationKind () const
                              { return fDurationKind; }

    int                   getDotsNumber () const
                              { return fDotsNumber; }

  public:

    // public services
    // ------------------------------------------------------

    void                  incrDotsNumber ()
                              { fDotsNumber++; }

    rational              dottedDurationAsWholeNotes (
                            int inputLineNumber) const;

  public:

    // visitors
    // ------------------------------------------------------

/* JMI
    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;
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
class msrSemiTonesPitchAndOctave : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSemiTonesPitchAndOctave> create (
      msrSemiTonesPitchKind semiTonesPitchKind,
      int                   octave);

    SMARTP<msrSemiTonesPitchAndOctave> createSemiTonesPitchAndOctaveNewbornClone ();

    // creation from  a string
    // ------------------------------------------------------

    static SMARTP<msrSemiTonesPitchAndOctave> createFromString (
      int    inputLineNumber,
      string theString);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSemiTonesPitchAndOctave (
      msrSemiTonesPitchKind semiTonesPitchKind,
      int                   octave);

    virtual ~msrSemiTonesPitchAndOctave ();

  public:

    // set and get
    // ------------------------------------------------------

    msrSemiTonesPitchKind getSemiTonesPitchKind () const
                              { return fSemiTonesPitchKind; }

    void                  incrementOctave ()
                              { fOctave++; }

    void                  decrementOctave ()
                              { fOctave--; }

    int                   getOctave () const
                              { return fOctave; }

  public:

    // public services
    // ------------------------------------------------------

    string                semiTonesPitchKindAsString () const;
    string                semiTonesPitchKindAsShortString () const;

    string                asString () const;

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    msrSemiTonesPitchKind fSemiTonesPitchKind;
    int                   fOctave;
};
typedef SMARTP<msrSemiTonesPitchAndOctave> S_msrSemiTonesPitchAndOctave;
EXP ostream& operator<< (ostream& os, const S_msrSemiTonesPitchAndOctave& elt);

// length units
//______________________________________________________________________________

enum msrLengthUnitKind {
  kInchUnit, kCentimeterUnit, kMillimeterUnit };

string msrLengthUnitKindAsString (
  msrLengthUnitKind lengthUnitKind);

string existingMsrLengthUnitKinds (int namesListMaxLength);

extern map<string, msrLengthUnitKind>
  gGlobalMsrLengthUnitKindsMap;

void initializeMsrLengthUnitKindsMap ();

// lengths
//______________________________________________________________________________

class msrLength : public smartable
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

    virtual ~msrLength ();

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
EXP ostream& operator<< (ostream& os, const S_msrLength& elt);

// margins types
//______________________________________________________________________________

enum msrMarginTypeKind {
  kOddMargin, kEvenMargin, kBothMargins };

string msrMarginTypeKindAsString (
  msrMarginTypeKind marginTypeKind);

string existingMsrMarginTypeKinds (int namesListMaxLength);

extern map<string, msrMarginTypeKind>
  gGlobalMsrMarginTypeKindsMap;

void initializeMsrMarginTypeKindsMap ();

// margins
//______________________________________________________________________________

class msrMargin : public smartable
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

    virtual ~msrMargin ();

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

class msrMarginsGroup : public smartable
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

    virtual ~msrMarginsGroup ();

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

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;
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
class msrFontSize : public smartable
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrFontSizeKind {
      kFontSizeNone,
      kFontSizeXXSmall, kFontSizeXSmall, kFontSizeSmall,
      kFontSizeMedium,
      kFontSizeLarge, kFontSizeXLarge, kFontSizeXXLarge,
      kFontSizeNumeric };

    static string fontSizeKindAsString (
      msrFontSizeKind fontSizeKind);

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

    virtual ~msrFontSize ();

  public:

    // set and get
    // ------------------------------------------------------

    msrFontSizeKind       getFontSizeKind () const
                              { return fFontSizeKind; }

    float                 getFontNumericSize () const;

  public:

    // public services
    // ------------------------------------------------------

    string                fontSizeKindAsString () const;

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

enum msrFontStyleKind {
  kFontStyleNone,
  kFontStyleNormal, KFontStyleItalic };

msrFontStyleKind msrFontStyleKindFromString (
  int    inputLineNumber,
  string fontStyleString);

string msrFontStyleKindAsString (
  msrFontStyleKind fontStyleKind);

enum msrFontWeightKind {
  kFontWeightNone,
  kFontWeightNormal, kFontWeightBold };

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

enum msrJustifyKind {
  kJustifyNone,
  kJustifyLeft, kJustifyCenter, kJustifyRight };

msrJustifyKind msrJustifyKindFromString (
  int    inputLineNumber,
  string justifyString);

string msrJustifyKindAsString (
  msrJustifyKind justifyKind);

enum msrHorizontalAlignmentKind {
  kHorizontalAlignmentNone,
  kHorizontalAlignmentLeft, kHorizontalAlignmentCenter, kHorizontalAlignmentRight };

msrHorizontalAlignmentKind msrHorizontalAlignmentKindFromString (
  int    inputLineNumber,
  string horizontalAlignString);

string msrHorizontalAlignmentKindAsString (
  msrHorizontalAlignmentKind horizontalAlignmentKind);

enum msrVerticalAlignmentKind {
  kVerticalAlignmentNone,
  kVerticalAlignmentTop, kVerticalAlignmentMiddle, kVerticalAlignmentBottom };

msrVerticalAlignmentKind msrVerticalAlignmentKindFromString (
  int    inputLineNumber,
  string verticalAlignmentString);

string msrVerticalAlignmentKindAsString (
  msrVerticalAlignmentKind verticalAlignmentKind);

// direction
//______________________________________________________________________________
enum msrDirectionKind {
  kDirectionNone,
  kDirectionUp, kDirectionDown };

string msrDirectionKindAsString (
  msrDirectionKind directionKind);

// print object
//______________________________________________________________________________
enum msrPrintObjectKind {
  kPrintObjectNone,
  kPrintObjectYes,
  kPrintObjectNo};

msrPrintObjectKind msrPrintObjectKindFromString (
  int    inputLineNumber,
  string printObjectString);

string msrPrintObjectKindAsString (
  msrPrintObjectKind printObjectKind);

// placement
//______________________________________________________________________________
enum msrPlacementKind {
  kPlacementNone,
  kPlacementAbove, kPlacementBelow };

msrPlacementKind msrPlacementKindFromString (
  int    inputLineNumber,
  string placementString);

string msrPlacementKindAsString (
  msrPlacementKind placementKind);

// measure style
//______________________________________________________________________________
enum msrSlashTypeKind {
  k_NoSlashType,
  kSlashTypeStart, kSlashTypeStop };

string msrSlashTypeKindAsString (
  msrSlashTypeKind slashTypeKind);

enum msrUseDotsKind {
  k_NoUseDots,
  kUseDotsYes, kUseDotsNo };

msrUseDotsKind msrUseDotsFromString (
  int    inputLineNumber,
  string useDotsString);

string msrUseDotsKindAsString (
  msrUseDotsKind useDotsKind);

enum msrSlashUseStemsKind {
  k_NoSlashUseStems,
  kSlashUseStemsYes, kSlashUseStemsNo };

string msrSlashUseStemsKindAsString (
  msrSlashUseStemsKind slashUseStemsKind);

// line types
//______________________________________________________________________________
enum msrLineTypeKind {
  kLineTypeSolid, kLineTypeDashed,
  kLineTypeDotted, kLineTypeWavy };

string msrLineTypeKindAsString (
  msrLineTypeKind lineTypeKind);

// tremolo types
//______________________________________________________________________________
enum msrTremoloTypeKind {
  k_NoTremoloType,
  kTremoloTypeSingle, kTremoloTypeStart, kTremoloTypeStop };

string msrTremoloTypeKindAsString (
  msrTremoloTypeKind tremoloTypeKind);

// technical types
//______________________________________________________________________________
enum msrTechnicalTypeKind { // JMI ???
  k_NoTechnicalType,
  kTechnicalTypeStart, kTechnicalTypeStop };

string msrTechnicalTypeKindAsString (
  msrTechnicalTypeKind technicalTypeKind);

// spanner types
//______________________________________________________________________________
enum msrSpannerTypeKind {
  k_NoSpannerType,
  kSpannerTypeStart, kSpannerTypeContinue, kSpannerTypeStop };

string msrSpannerTypeKindAsString (
  msrSpannerTypeKind spannerTypeKind);

// moments
//______________________________________________________________________________
class msrMoment
{
  public:

    // constants
    // ------------------------------------------------------

    #define K_NO_POSITION rational (-222222, 1)

    #define K_NO_MOMENT   msrMoment (K_NO_POSITION, K_NO_POSITION)

  public:

    // constructors/destructor
    // ------------------------------------------------------

    msrMoment ();

    msrMoment (
      rational positionInMeasure,
      rational relativeOffset);

    msrMoment (
      rational positionInMeasure);

    virtual ~msrMoment ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setPositionInMeasure (rational actualNotes)
                              { fPositionInMeasure = actualNotes; }

    rational              getPositionInMeasure () const
                              { return fPositionInMeasure; }

    void                  setRelativeOffset (rational normalNotes)
                              { fRelativeOffset = normalNotes; }

    rational              getRelativeOffset () const
                              { return fRelativeOffset; }

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
    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;
*/

  public:

    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    rational              fPositionInMeasure;
    rational              fRelativeOffset;
};
EXP ostream& operator<< (ostream& os, const msrMoment& elt);

// tuplet factors
//______________________________________________________________________________
class msrTupletFactor
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

    virtual ~msrTupletFactor ();

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
    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;
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

class msrHarmonyInterval : public smartable
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

    virtual ~msrHarmonyInterval ();

  public:

    // set and get
    // ------------------------------------------------------

    msrIntervalKind       getHarmonyIntervalIntervalKind () const
                              { return fHarmonyIntervalIntervalKind; }

    void                  incrementHarmonyIntervalRelativeOctave ()
                              { fHarmonyIntervalRelativeOctave++; }

    void                  decrementHarmonyIntervalRelativeOctave ()
                              { fHarmonyIntervalRelativeOctave--; }

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

    string                harmonyIntervalIntervalKindAsString () const;
    string                harmonyIntervalIntervalKindAsShortString () const;

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
    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;
*/

  public:

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

class msrHarmonyStructure : public smartable
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

    virtual ~msrHarmonyStructure ();

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
    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;
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
class msrHarmonyContents : public smartable
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

    virtual ~msrHarmonyContents ();

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
    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;
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
class msrRGBColor {
  public:

    // constructors/destructor
    // ------------------------------------------------------

    msrRGBColor ();

    msrRGBColor (
      float theR,
      float theG,
      float theB);

    msrRGBColor (
      std::string theString);

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

    bool                  isEmpty () const
                              {
                                return
                                  fR < 0.0
                                    &&
                                  fG < 0.0
                                    &&
                                  fB < 0.0;
                              }

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

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
class msrAlphaRGBColor
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

    msrAlphaRGBColor (
      string colorRGB,
      string colorAlpha);

    msrAlphaRGBColor (
      string colorRGB);

    virtual ~msrAlphaRGBColor ();

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

    bool                  isEmpty () const
                              {
                                return
                                  fColorRGB.size () == 0
                                    &&
                                  fColorAlpha.size () == 0;
                              }

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
enum msrScoreNotationKind {
  kScoreNotationWestern,
  kScoreNotationJianpu,
  kScoreNotationDiatonicAccordion };

string msrScoreNotationKindAsString (
  msrScoreNotationKind scoreNotationKind);

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

void initializeQuarterTonesPitchesLanguageKinds ();

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

string existingQuarterTonesPitchesLanguageKinds (int namesListMaxLength);

// initialization
//______________________________________________________________________________
void initializeMSRBasicTypes ();


} // namespace MusicXML2


#endif
