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

msrXMLLangKind msrXMLLangKindFromString (
  int    inputLineNumber,
  string XMLLangString);

string msrXMLLangKindAsString (
  msrXMLLangKind XMLLangKind);

// diatonic pitches
//______________________________________________________________________________
enum msrDiatonicPitchKind {
  // starting at C for LilyPond relative octave calculations
  kC, kD, kE, kF, kG, kA, kB,
  k_NoDiatonicPitch };

msrDiatonicPitchKind msrDiatonicPitchKindFromString (
  char diatonicNoteName);

string msrDiatonicPitchKindAsString (
  msrDiatonicPitchKind diatonicPitchKind);

// alterations
//______________________________________________________________________________
enum msrAlterationKind {
  k_NoAlteration,

  kTripleFlat, kDoubleFlat, kSesquiFlat, kFlat, kSemiFlat,
  kNatural,
  kSemiSharp, kSharp, kSesquiSharp, kDoubleSharp, kTripleSharp };

msrAlterationKind msrAlterationKindFromMusicXMLAlter (
  float alter);

string msrAlterationKindAsString (
  msrAlterationKind alterationKind);

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

string msrSemiTonesPitchKindAsString (
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

void setDiatonicPitchKindAndAlterationKind (
  msrQuarterTonesPitchKind quarterTonesPitchKind,
  msrDiatonicPitchKind&    diatonicPitchKind,
  msrAlterationKind&       alterationKind);

msrQuarterTonesPitchKind quarterTonesPitchKindFromDiatonicPitchAndAlteration (
  int                  inputLineNumber,
  msrDiatonicPitchKind diatonicPitchKind,
  msrAlterationKind    alterationKind);

msrDiatonicPitchKind diatonicPitchKindFromQuarterTonesPitchKind (
  int                      inputLineNumber,
  msrQuarterTonesPitchKind quarterTonesPitchKind);

msrQuarterTonesPitchKind quarterTonesPitchKindFromSemiTonesPitchKind (
  msrSemiTonesPitchKind semiTonesPitchKind);

msrSemiTonesPitchKind semiTonesPitchKindFromQuarterTonesPitchKind (
  msrQuarterTonesPitchKind quarterTonesPitchKind);

msrSemiTonesPitchKind semiTonesPitchKindFromString (
  string theString);

// alterations preferences
//______________________________________________________________________________
enum msrAlterationPreferenceKind {
  kPreferFlat, kPreferNatural, kPreferSharp };

msrQuarterTonesPitchKind msrSemiTonesPitchKindAsQuarterTonesPitchKind (
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

int msrIntervalKindAsSemiTones (
  msrIntervalKind intervalKind);

int msrIntervalKindAsQuarterTones (
  msrIntervalKind intervalKind);

string msrIntervalKindAsString (
  msrIntervalKind intervaKindl);

string msrIntervalKindAsShortString (
  msrIntervalKind intervalKind);

msrIntervalKind invertIntervalKind (
  msrIntervalKind intervalKind);

int intervalKindAsSemitones (
  msrIntervalKind intervalKind);

/* JMI
msrSemiTonesPitchKind noteAtIntervalKindFromNote (
  msrIntervalKind             intervalKind,
  msrSemiTonesPitchKind       semiTonesPitchKind,
  msrAlterationPreferenceKind alterationPreferenceKind);
*/

msrSemiTonesPitchKind noteAtIntervalFromSemiTonesPitch (
  int                   inputLineNumber,
  msrIntervalKind       intervalKind,
  msrSemiTonesPitchKind semiTonesPitchKind);

msrQuarterTonesPitchKind noteAtIntervalFromQuarterTonesPitch (
  int                      inputLineNumber,
  msrIntervalKind          intervalKind,
  msrQuarterTonesPitchKind quarterTonesPitchKind);

msrIntervalKind intervalBetweenSemiTonesPitches (
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

string clefKindAsString (
  msrClefKind clefKind);

msrClefKind clefKindFromString (string theString);

string existingClefKinds (int namesListMaxLength);
string existingClefKindsNames (int namesListMaxLength);

extern map<string, msrClefKind>
  gClefKindsMap;

extern list<string>
  gClefKindsNamesList;

void initializeClefKinds ();

// harmonies
//______________________________________________________________________________
enum msrHarmonyKind {
  k_NoHarmony,

  // MusicXML chords

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

  // jazz-specific chords

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

string msrHarmonyKindAsString (
  msrHarmonyKind harmonyKind);

string msrHarmonyKindAsShortString (
  msrHarmonyKind harmonyKind);

string msrHarmonyKindShortName (
  msrHarmonyKind harmonyKind);

msrHarmonyKind msrHarmonyKindFromString (
  string theString);

string existingHarmonyKinds (int namesListMaxLength);
string existingHarmonyKindsNames (int namesListMaxLength);

// constant
const int K_HARMONY_NO_INVERSION = -1;

extern map<string, msrHarmonyKind>
  gHarmonyKindsMap;

extern list<string>
  gHarmonyKindsNamesList;

void initializeHarmonyKinds ();

// quarter tones pitches languages
//______________________________________________________________________________
enum msrQuarterTonesPitchesLanguageKind {
  kNederlands, kCatalan, kDeutsch, kEnglish, kEspanol, kFrancais,
  kItaliano, kNorsk, kPortugues, kSuomi, kSvenska, kVlaams };

string msrQuarterTonesPitchesLanguageKindAsString (
  msrQuarterTonesPitchesLanguageKind languageKind);

string msrDiatonicPitchKindAsString ( // JMI
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrDiatonicPitchKind               diatonicPitchKind);

string msrQuarterTonesPitchKindAsString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrQuarterTonesPitchKind           quarterTonesPitchKind);

msrQuarterTonesPitchKind msrQuarterTonesPitchKindFromString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  string                             quarterTonesPitchName);

/* JMI
string msrSemiTonesPitchKindAsString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrSemiTonesPitchKind              semiTonesPitchKind);
  */

string msrSemiTonesPitchKindAsFlatsAndSharps (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrSemiTonesPitchKind              semiTonesPitchKind);

/* JMI
string msrQuarterTonesPitchKindAsFlatsAndSharps (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrQuarterTonesPitchKind           quarterTonesPitchKind);
*/

// enharmonies
//______________________________________________________________________________
msrSemiTonesPitchKind enharmonicSemiTonesPitch (
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

msrDurationKind msrDurationKindFromString (
  int    inputLineNumber,
  string durationString);

rational msrDurationKindAsWholeNotes (
  msrDurationKind durationKind);

msrDurationKind wholeNotesAsDurationKind (rational wholeNotes);

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

    // services
    // ------------------------------------------------------

    void                  incrDotsNumber ()
                              { fDotsNumber++; }

    rational              dottedDurationAsWholeNotes (
                            int inputLineNumber) const;

    // visitors
    // ------------------------------------------------------

/* JMI
    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);
*/

  public:

    // ------------------------------------------------------

    string                asString () const;

    //virtual
    void                  print (ostream& os) const;

  private:

    // fields
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

    // services
    // ------------------------------------------------------

    string                semiTonesPitchKindAsString () const;
    string                semiTonesPitchKindAsShortString () const;

    string                asString () const;

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // fields
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
  gMsrLengthUnitKindsMap;

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

    // services
    // ------------------------------------------------------

                          bool operator== (
                            const msrLength& other) const
                              {
                                return
                                  fLengthUnitKind == other.fLengthUnitKind
                                   &&
                                  fLengthValue == other.fLengthValue;
                              }

                          bool operator!= (
                            const msrLength& other) const
                              { return ! ((*this) == other); }

    void                  convertToLengthUnit (
                            msrLengthUnitKind lengthUnitKind);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os) const;

  private:

    // fields
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
  gMsrMarginTypeKindsMap;

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

    // services
    // ------------------------------------------------------

                          bool operator== (
                            const msrMargin& other) const
                              {
                                return
                                  fMarginTypeKind == other.fMarginTypeKind
                                   &&
                                  fMarginLength == other.fMarginLength;
                              }

                          bool operator!= (
                            const msrMargin& other) const
                              { return ! ((*this) == other); }

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------


    msrMarginTypeKind     fMarginTypeKind;
    msrLength             fMarginLength;
};
typedef SMARTP<msrMargin> S_msrMargin;
EXP ostream& operator<< (ostream& os, const S_msrMargin& elt);

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

    float                 getFontNumericSize ();

    // services
    // ------------------------------------------------------

    string                fontSizeKindAsString () const;

    string                fontSizeAsString () const;

  public:

    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // fields
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
  kPrintObjectYes, kPrintObjectNo};

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

    // services
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

    // visitors
    // ------------------------------------------------------

/* JMI
    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);
*/

  public:

    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    int                   fTupletActualNotes;
    int                   fTupletNormalNotes;
};
EXP ostream& operator<< (ostream& os, const msrTupletFactor& elt);

// chords intervals
//______________________________________________________________________________
class msrChordInterval;
typedef SMARTP<msrChordInterval> S_msrChordInterval;

class msrChordInterval : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrChordInterval> create (
      msrIntervalKind chordIntervalIntervalKind,
      int             chordIntervalRelativeOctave = 0);
                        // 0: up to the thirteenth,
                        // no relative octave is needed

    SMARTP<msrChordInterval> createChordIntervalNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrChordInterval (
      msrIntervalKind chordIntervalIntervalKind,
      int             chordIntervalRelativeOctave);

    virtual ~msrChordInterval ();

  public:

    // set and get
    // ------------------------------------------------------

    msrIntervalKind       getChordIntervalIntervalKind () const
                              { return fChordIntervalIntervalKind; }

    void                  incrementChordIntervalRelativeOctave ()
                              { fChordIntervalRelativeOctave++; }

    void                  decrementChordIntervalRelativeOctave ()
                              { fChordIntervalRelativeOctave--; }

    int                   getChordIntervalRelativeOctave () const
                              { return fChordIntervalRelativeOctave; }

    // services
    // ------------------------------------------------------

    int                   chordIntervalAsSemitones () const
                              {
                                return
                                  msrIntervalKindAsSemiTones (
                                    fChordIntervalIntervalKind);
                              }

    string                chordIntervalIntervalKindAsString () const;
    string                chordIntervalIntervalKindAsShortString () const;

    string                chordIntervalAsString () const;
    string                chordIntervalAsShortString () const;

    /*
      we handle intervals up to the thirteenth in chords,
      but operations on intervals are easier to compute
      in a normal form limited to an octave, hence:
    */
    void                  normalizeInterval ();
    void                  deNormalizeInterval ();

    S_msrChordInterval    intervalDifference (
                            S_msrChordInterval otherChordInterval);
    S_msrChordInterval    intervalSum (
                            S_msrChordInterval otherChordInterval);

    string                asString () const;
    string                asShortString () const;

    // visitors
    // ------------------------------------------------------

/* JMI
    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);
*/

  public:

    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    msrIntervalKind       fChordIntervalIntervalKind;

    int                   fChordIntervalRelativeOctave;
};
typedef SMARTP<msrChordInterval> S_msrChordInterval;
EXP ostream& operator<< (ostream& os, const S_msrChordInterval& elt);

// chords structure
//______________________________________________________________________________
class msrChordStructure;
typedef SMARTP<msrChordStructure> S_msrChordStructure;

class msrChordStructure : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrChordStructure> createBare (
      msrHarmonyKind chordStructureHarmonyKind);

    static SMARTP<msrChordStructure> create (
      msrHarmonyKind chordStructureHarmonyKind);

    SMARTP<msrChordStructure> createChordStructureNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrChordStructure (
      msrHarmonyKind chordStructureHarmonyKind);

    virtual ~msrChordStructure ();

  private:

    // initialization
    // ------------------------------------------------------

  public:

    // set and get
    // ------------------------------------------------------

    msrHarmonyKind        getChordStructureHarmonyKind () const
                              { return fChordStructureHarmonyKind; }

    const vector <S_msrChordInterval>&
                          getChordStructureIntervals () const
                              { return fChordStructureIntervals; }

    // services
    // ------------------------------------------------------

    void                  appendChordIntervalToChordStructure (
                            S_msrChordInterval chordInterval);

    void                  populateChordStructure ();

    string                chordStructureAsString () const;
    string                chordStructureAsShortString () const;

    S_msrChordInterval    bassChordIntervalForChordInversion (
                            int inputLineNumber,
                            int inversionNumber);

    S_msrChordStructure   invertChordStructure (int inversion);

    static void           printAllChordsStructures (ostream& os);

    // visitors
    // ------------------------------------------------------
/* JMI
    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);
*/

  public:

    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    msrHarmonyKind        fChordStructureHarmonyKind;
    vector<S_msrChordInterval>
                          fChordStructureIntervals;
};
typedef SMARTP<msrChordStructure> S_msrChordStructure;
EXP ostream& operator<< (ostream& os, const S_msrChordStructure& elt);

// chords contents
//______________________________________________________________________________
class msrChordContents : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrChordContents> create (
 // JMI     int                   inputLineNumber,
      msrSemiTonesPitchKind chordContentsRootNote,
      msrHarmonyKind        chordContentsHarmonyKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrChordContents (
 // JMI     int                   inputLineNumber,
      msrSemiTonesPitchKind chordContentsRootNote,
      msrHarmonyKind        chordContentsHarmonyKind);

    virtual ~msrChordContents ();

  public:

    // set and get
    // ------------------------------------------------------

    msrSemiTonesPitchKind
                          getChordContentsRootNote () const
                              { return fChordContentsRootNote; }

    msrHarmonyKind        getChordContentsHarmonyKind () const
                              { return fChordContentsHarmonyKind; }

    const vector<S_msrSemiTonesPitchAndOctave>&
                          getChordElementsVector () const
                              { return fChordElementsVector; }

    // services
    // ------------------------------------------------------

    string                chordContentsAsString () const;
    string                chordContentsAsShortString () const;

    msrSemiTonesPitchKind bassSemiTonesPitchKindForChordInversion (
                            int inputLineNumber,
                            int inversionNumber);

    static void           printAllChordsContents (
                            ostream&              os,
                            msrSemiTonesPitchKind semiTonesPitchKind);

    // visitors
    // ------------------------------------------------------
/* JMI
    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);
*/

  public:

    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    msrSemiTonesPitchKind fChordContentsRootNote;
    msrHarmonyKind        fChordContentsHarmonyKind;

    vector<S_msrSemiTonesPitchAndOctave>
                          fChordElementsVector;
};
typedef SMARTP<msrChordContents> S_msrChordContents;
EXP ostream& operator<< (ostream& os, const S_msrChordContents& elt);

// chords details and analysis
//______________________________________________________________________________
void printChordDetails (
  ostream&              os,
  msrSemiTonesPitchKind rootSemiTonesPitchKind,
  msrHarmonyKind        harmonyKind);

void printChordAnalysis (
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

    // services
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

    std::string           asString () const;

    void                  print (ostream& os) const;

  private:

    // fields
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

    // services
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

    // fields
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
extern map<msrHarmonyKind, S_msrChordStructure>
  gChordStructuresMap;

void initializeChordStructuresMap ();

void printChordStructuresMap ();

extern map<string, msrQuarterTonesPitchesLanguageKind>
  gQuarterTonesPitchesLanguageKindsMap;

extern map<msrQuarterTonesPitchKind, string> gNederlandsPitchNamesMap;
extern map<msrQuarterTonesPitchKind, string> gCatalanPitchNamesMap;
extern map<msrQuarterTonesPitchKind, string> gDeutschPitchNamesMap;
extern map<msrQuarterTonesPitchKind, string> gEnglishPitchNamesMap;
extern map<msrQuarterTonesPitchKind, string> gEspanolPitchNamesMap;
extern map<msrQuarterTonesPitchKind, string> gFrancaisPitchNamesMap;
extern map<msrQuarterTonesPitchKind, string> gItalianoPitchNamesMap;
extern map<msrQuarterTonesPitchKind, string> gNorskPitchNamesMap;
extern map<msrQuarterTonesPitchKind, string> gPortuguesPitchNamesMap;
extern map<msrQuarterTonesPitchKind, string> gSuomiPitchNamesMap;
extern map<msrQuarterTonesPitchKind, string> gSvenskaPitchNamesMap;
extern map<msrQuarterTonesPitchKind, string> gVlaamsPitchNamesMap;

void initializeQuarterTonesPitchesLanguageKinds ();

string existingQuarterTonesPitchesLanguageKinds (int namesListMaxLength);

// initialization
//______________________________________________________________________________
void initializeMSRBasicTypes ();


} // namespace MusicXML2


#endif
