/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <iostream>
#include <sstream>
#include <cassert>

#include <vector>
#include <map>

#include <iomanip>      // setw, ...

#include "rational.h"

#include "utilities.h"

#include "messagesHandling.h"

#include "msrBasicTypes.h"

#include "generalOptions.h"

#include "xml2lyOptionsHandling.h"


using namespace std;

namespace MusicXML2 {


#define TRACE_MSR_BASIC_TYPES 0


// dotted durations
//______________________________________________________________________________
msrDottedDuration::msrDottedDuration ()
{
  fDuration   = k_NoDuration;
  fDotsNumber = -1;
}

msrDottedDuration::msrDottedDuration (
  msrDurationKind duration,
  int             dotsNumber)
{
  fDuration   = duration;
  fDotsNumber = dotsNumber;
}

rational msrDottedDuration::dottedDurationAsWholeNotes (
  int inputLineNumber) const
{
  // convert duration into whole notes
  rational result =
    msrDurationKindAsWholeNotes (
      fDuration);

  // take dots into account if any
  if (fDotsNumber > 0) {
    int dots = fDotsNumber;

    while (dots > 0) {
      result *=
        rational (3, 2);
      result.rationalise ();

      dots--;
    } // while
  }

  return result;
}

void msrDottedDuration::print (ostream& os)
{
  const int fieldWidth = 11;

  os << left <<
    setw (fieldWidth) <<
    "duration" << " = " <<
    msrDurationKindAsString (fDuration) <<
    endl <<
    setw (fieldWidth) <<
    "dotsNumber" << " = " << fDotsNumber <<
    endl;
};

ostream& operator<< (ostream& os, msrDottedDuration elt)
{
  elt.print (os);
  return os;
}

// intervals
//______________________________________________________________________________
int msrIntervalKindAsSemiTones (
  msrIntervalKind intervalKind)
{
  int result = -1;

  switch (intervalKind) {
    case kDiminishedUnisson:
      result = -1;
      break;
    case kUnisson:
      result = 0;
      break;
    case kAugmentedUnison:
      result = 1;
      break;
      
    case kDiminishedSecond:
      result = 0;
      break;
    case kMinorSecond:
      result = 1;
      break;
    case kMajorSecond:
      result = 2;
      break;
    case kAugmentedSecond:
      result = 4;
      break;
      
    case kDiminishedThird:
      result = 2;
      break;
    case kMinorThird:
      result = 3;
      break;
    case kMajorThird:
      result = 4;
      break;
    case kAugmentedThird:
      result = 5;
      break;
      
    case kDiminishedFourth:
      result = 4;
      break;
    case kPerFourth:
      result = 5;
      break;
    case kAugmentedFourth:
      result = 6;
      break;
      
    case kDiminishedFifth:
      result = 6;
      break;
    case kPerFifth:
      result = 7;
      break;
    case kAugmentedFifth:
      result = 8;
      break;

    case kDiminishedSixth:
      result = 7;
      break;
    case kMinorSixth:
      result = 8;
      break;
    case kMajorSixth:
      result = 9;
      break;
    case kAugmentedSixth:
      result = 10;
      break;
      
    case kDiminishedSeventh:
      result = 9;
      break;
    case kMinorSeventh:
      result = 10;
      break;
    case kMajorSeventh:
      result = 11;
      break;
    case kAugmentedSeventh:
      result = 12;
      break;

    case kDiminishedOctave:
      result = 11;
      break;
    case kPerOctave:
      result = 12;
      break;
    case kAugmentedOctave:
      result = 13;
      break;
      
    case kDiminishedNinth:
      result = 12;
      break;
    case kMinorNinth:
      result = 13;
      break;
    case kMajorNinth:
      result = 14;
      break;
    case kAugmentedNinth:
      result = 15;
      break;
      
    case kDiminishedTenth:
      result = 14;
      break;
    case kMinorTenth:
      result = 15;
      break;
    case kMajorTenth:
      result = 16;
      break;
    case kAugmentedTenth:
      result = 17;
      break;
      
    case kDiminishedEleventh:
      result = 16;
      break;
    case kPerEleventh:
      result = 17;
      break;
    case kAugmentedEleventh:
      result = 18;
      break;
      
    case kDiminishedTwelfth:
      result = 18;
      break;
    case kPerTwelfth:
      result = 19;
      break;
    case kAugmentedTwelfth:
      result = 20;
      break;

    case kDiminishedThirteenth:
      result = 19;
      break;
    case kMinorThirteenth:
      result = 20;
      break;
    case kMajorThirteenth:
      result = 21;
      break;
    case kAugmentedThirteenth:
      result = 22;
      break;
  } // switch

  return result;
}

int msrIntervalAsQuarterTones (
  msrIntervalKind intervalKind)
{
  return
    msrIntervalKindAsSemiTones (intervalKind) * 2;
}

//______________________________________________________________________________
string msrIntervalKindAsString (
  msrIntervalKind intervalKind)
{
  string result;

  switch (intervalKind) {
    case kDiminishedUnisson:
      result = "DiminishedUnisson";
      break;
    case kUnisson:
      result = "Unisson";
      break;
    case kAugmentedUnison:
      result = "AugmentedUnison";
      break;
      
    case kDiminishedSecond:
      result = "DiminishedSecond";
      break;
    case kMinorSecond:
      result = "MinorSecond";
      break;
    case kMajorSecond:
      result = "MajorSecond";
      break;
    case kAugmentedSecond:
      result = "AugmentedSecond";
      break;
      
    case kDiminishedThird:
      result = "DiminishedThird";
      break;
    case kMinorThird:
      result = "MinorThird";
      break;
    case kMajorThird:
      result = "MajorThird";
      break;
    case kAugmentedThird:
      result = "AugmentedThird";
      break;
      
    case kDiminishedFourth:
      result = "DiminishedFourth";
      break;
    case kPerFourth:
      result = "PerFourth";
      break;
    case kAugmentedFourth:
      result = "AugmentedFourth";
      break;
      
    case kDiminishedFifth:
      result = "DiminishedFifth";
      break;
    case kPerFifth:
      result = "PerFifth";
      break;
    case kAugmentedFifth:
      result = "AugmentedFifth";
      break;

    case kDiminishedSixth:
      result = "DiminishedSixth";
      break;
    case kMinorSixth:
      result = "MinorSixth";
      break;
    case kMajorSixth:
      result = "MajorSixth";
      break;
    case kAugmentedSixth:
      result = "AugmentedSixth";
      break;
      
    case kDiminishedSeventh:
      result = "DiminishedSeventh";
      break;
    case kMinorSeventh:
      result = "MinorSeventh";
      break;
    case kMajorSeventh:
      result = "MajorSeventh";
      break;
    case kAugmentedSeventh:
      result = "AugmentedSeventh";
      break;

    case kDiminishedOctave:
      result = "DiminishedOctave";
      break;
    case kPerOctave:
      result = "PerOctave";
      break;
    case kAugmentedOctave:
      result = "AugmentedOctave";
      break;
      
    case kDiminishedNinth:
      result = "DiminishedNinth";
      break;
    case kMinorNinth:
      result = "MinorNinth";
      break;
    case kMajorNinth:
      result = "MajorNinth";
      break;
    case kAugmentedNinth:
      result = "AugmentedNinth";
      break;
      
    case kDiminishedTenth:
      result = "DiminishedTenth";
      break;
    case kMinorTenth:
      result = "MinorTenth";
      break;
    case kMajorTenth:
      result = "MajorTenth";
      break;
    case kAugmentedTenth:
      result = "AugmentedTenth";
      break;
      
    case kDiminishedEleventh:
      result = "DiminishedEleventh";
      break;
    case kPerEleventh:
      result = "PerEleventh";
      break;
    case kAugmentedEleventh:
      result = "AugmentedEleventh";
      break;
      
    case kDiminishedTwelfth:
      result = "DiminishedTwelfth";
      break;
    case kPerTwelfth:
      result = "PerTwelfth";
      break;
    case kAugmentedTwelfth:
      result = "AugmentedTwelfth";
      break;

    case kDiminishedThirteenth:
      result = "DiminishedThirteenth";
      break;
    case kMinorThirteenth:
      result = "MinorThirteenth";
      break;
    case kMajorThirteenth:
      result = "MajorThirteenth";
      break;
    case kAugmentedThirteenth:
      result = "AugmentedThirteenth";
      break;
  } // switch

  return result;
}

string msrIntervalAsShortString (
  msrIntervalKind intervalKind)
{
  string result;
  
  switch (intervalKind) {
    case kDiminishedUnisson:
      result = "bu";
      break;
    case kUnisson:
      result = "u";
      break;
    case kAugmentedUnison:
      result = "#u";
      break;
      
    case kDiminishedSecond:
      result = "bb2";
      break;
    case kMinorSecond:
      result = "b2";
      break;
    case kMajorSecond:
      result = "2";
      break;
    case kAugmentedSecond:
      result = "#2";
      break;
      
    case kDiminishedThird:
      result = "bb3";
      break;
    case kMinorThird:
      result = "b3";
      break;
    case kMajorThird:
      result = "3";
      break;
    case kAugmentedThird:
      result = "#3";
      break;
      
    case kDiminishedFourth:
      result = "b4";
      break;
    case kPerFourth:
      result = "4";
      break;
    case kAugmentedFourth:
      result = "#4";
      break;
      
    case kDiminishedFifth:
      result = "b5";
      break;
    case kPerFifth:
      result = "5";
      break;
    case kAugmentedFifth:
      result = "#4";
      break;

    case kDiminishedSixth:
      result = "bb6";
      break;
    case kMinorSixth:
      result = "b6";
      break;
    case kMajorSixth:
      result = "6";
      break;
    case kAugmentedSixth:
      result = "#6";
      break;
      
    case kDiminishedSeventh:
      result = "b7";
      break;
    case kMinorSeventh:
      result = "7";
      break;
    case kMajorSeventh:
      result = "∆7";
      break;
    case kAugmentedSeventh:
      result = "∆∆7";
      break;

    case kDiminishedOctave:
      result = "b8";
      break;
    case kPerOctave:
      result = "8";
      break;
    case kAugmentedOctave:
      result = "#8";
      break;
      
    case kDiminishedNinth:
      result = "bb9";
      break;
    case kMinorNinth:
      result = "b9";
      break;
    case kMajorNinth:
      result = "9";
      break;
    case kAugmentedNinth:
      result = "#9";
      break;
      
    case kDiminishedTenth:
      result = "bb10";
      break;
    case kMinorTenth:
      result = "b10";
      break;
    case kMajorTenth:
      result = "10";
      break;
    case kAugmentedTenth:
      result = "#10";
      break;
      
    case kDiminishedEleventh:
      result = "b11";
      break;
    case kPerEleventh:
      result = "11";
      break;
    case kAugmentedEleventh:
      result = "#11";
      break;
      
    case kDiminishedTwelfth:
      result = "b12";
      break;
    case kPerTwelfth:
      result = "12";
      break;
    case kAugmentedTwelfth:
      result = "#12";
      break;

    case kDiminishedThirteenth:
      result = "bb13";
      break;
    case kMinorThirteenth:
      result = "b13";
      break;
    case kMajorThirteenth:
      result = "13";
      break;
    case kAugmentedThirteenth:
      result = "#13";
      break;
  } // switch

  return result;
}

msrIntervalKind invertIntervalKind (
  msrIntervalKind intervalKind)
{
  msrIntervalKind result;
  
  switch (intervalKind) {
    case kDiminishedUnisson:
      result = kAugmentedUnison;
      break;
    case kUnisson:
      result = kUnisson;
      break;
    case kAugmentedUnison:
      result = kDiminishedOctave;
      break;
      
    case kDiminishedSecond:
      result = kAugmentedSeventh;
      break;
    case kMinorSecond:
      result = kMajorSeventh;
      break;
    case kMajorSecond:
      result = kMinorSeventh;
      break;
    case kAugmentedSecond:
      result = kDiminishedSeventh;
      break;
      
    case kDiminishedThird:
      result = kAugmentedSixth;
      break;
    case kMinorThird:
      result = kMajorSixth;
      break;
    case kMajorThird:
      result = kMinorSixth;
      break;
    case kAugmentedThird:
      result = kDiminishedSixth;
      break;
      
    case kDiminishedFourth:
      result = kAugmentedFifth;
      break;
    case kPerFourth:
      result = kPerFifth;
      break;
    case kAugmentedFourth:
      result = kDiminishedFifth;
      break;
      
    case kDiminishedFifth:
      result = kAugmentedFourth;
      break;
    case kPerFifth:
      result = kPerFourth;
      break;
    case kAugmentedFifth:
      result = kDiminishedFourth;
      break;

    case kDiminishedSixth:
      result = kAugmentedThird;
      break;
    case kMinorSixth:
      result = kMajorThird;
      break;
    case kMajorSixth:
      result = kMinorThird;
      break;
    case kAugmentedSixth:
      result = kDiminishedThird;
      break;
      
    case kDiminishedSeventh:
      result = kAugmentedSecond;
      break;
    case kMinorSeventh:
      result = kMajorSecond;
      break;
    case kMajorSeventh:
      result = kMinorSecond;
      break;
    case kAugmentedSeventh:
      result = kDiminishedSecond;
      break;

    case kDiminishedOctave:
      result = kAugmentedUnison;
      break;
    case kPerOctave:
      result = kPerOctave;
      break;
    case kAugmentedOctave:
      result = kDiminishedUnisson;
      break;
      
    case kDiminishedNinth:
      result = kAugmentedSeventh;
      break;
    case kMinorNinth:
      result = kMajorSeventh;
      break;
    case kMajorNinth:
      result = kMinorSeventh;
      break;
    case kAugmentedNinth:
      result = kDiminishedSeventh;
      break;
      
    case kDiminishedTenth:
      result = kAugmentedSixth;
      break;
    case kMinorTenth:
      result = kMajorSixth;
      break;
    case kMajorTenth:
      result = kMinorSixth;
      break;
    case kAugmentedTenth:
      result = kDiminishedSixth;
      break;
      
    case kDiminishedEleventh:
      result = kAugmentedFifth;
      break;
    case kPerEleventh:
      result = kPerFifth;
      break;
    case kAugmentedEleventh:
      result = kDiminishedFifth;
      break;
      
    case kDiminishedTwelfth:
      result = kAugmentedFourth;
      break;
    case kPerTwelfth:
      result = kPerFourth;
      break;
    case kAugmentedTwelfth:
      result = kDiminishedFourth;
      break;

    case kDiminishedThirteenth:
      result = kAugmentedThird;
      break;
    case kMinorThirteenth:
      result = kMajorThird;
      break;
    case kMajorThirteenth:
      result = kMinorThird;
      break;
    case kAugmentedThirteenth:
      result = kDiminishedThird;
      break;
  } // switch

  return result;
}

/// harmonies
//______________________________________________________________________________
string msrHarmonyKindAsString (
  msrHarmonyKind harmonyKind)
{
  string result;
  
  switch (harmonyKind) {
    case k_NoHarmony:
      result = "Harmony???";
      break;

    case kMajorHarmony:
      result = "Major";
      break;
    case kMinorHarmony:
      result = "Minor";
      break;
    case kAugmentedHarmony:
      result = "Augmented";
      break;
    case kDiminishedHarmony:
      result = "Diminished";
      break;

    case kDominantHarmony:
      result = "Dominant";
      break;
    case kMajorSeventhHarmony:
      result = "MajorSeventh";
      break;
    case kMinorSeventhHarmony:
      result = "MinorSeventh";
      break;
    case kDiminishedSeventhHarmony:
      result = "DiminishedSeventh";
      break;
    case kAugmentedSeventhHarmony:
      result = "AugmentedSeventh";
      break;
    case kHalfDiminishedHarmony:
      result = "HalfDiminished";
      break;
    case kMajorMinorHarmony:
      result = "MajorMinor";
      break;

    case kMajorSixthHarmony:
      result = "MajorSixth";
      break;
    case kMinorSixthHarmony:
      result = "MinorSixth";
      break;

    case kDominantNinthHarmony:
      result = "DominantNinth";
      break;
    case kMajorNinthHarmony:
      result = "MajorNinth";
      break;
    case kMinorNinthHarmony:
      result = "MinorNinth";
      break;

    case kDominantEleventhHarmony:
      result = "DominantEleventh";
      break;
    case kMajorEleventhHarmony:
      result = "MajorEleventh";
      break;
    case kMinorEleventhHarmony:
      result = "MinorEleventh";
      break;

    case kDominantThirteenthHarmony:
      result = "DominantThirteenth";
      break;
    case kMajorThirteenthHarmony:
      result = "MajorThirteenth";
      break;
    case kMinorThirteenthHarmony:
      result = "MinorThirteenth";
      break;

    case kSuspendedSecondHarmony:
      result = "SuspendedSecond";
      break;
    case kSuspendedFourthHarmony:
      result = "SuspendedFourth";
      break;
      
    case kNeapolitanHarmony:
      result = "Neapolitan";
      break;
    case kItalianHarmony:
      result = "Italian";
      break;
    case kFrenchHarmony:
      result = "French";
      break;
    case kGermanHarmony:
      result = "German";
      break;

    case kPedalHarmony:
      result = "Pedal";
      break;
    case kPowerHarmony:
      result = "Power";
      break;
    case kTristanHarmony:
      result = "Tristan";
      break;
      
    case kOtherHarmony:
      result = "Other";
      break;
    case kNoneHarmony:
      result = "None";
      break;
  } // switch

  return result;
}

string msrHarmonyKindAsShortString (
  msrHarmonyKind harmonyKind)
{
  string result;
  
  switch (harmonyKind) {
    case k_NoHarmony:
      result = "Harmony???";
      break;

    case kMajorHarmony:
      result = "";
      break;
    case kMinorHarmony:
      result = "m";
      break;
    case kAugmentedHarmony:
      result = "+";
      break;
    case kDiminishedHarmony:
      result = "dim";
      break;

    case kDominantHarmony:
      result = "7";
      break;
    case kMajorSeventhHarmony:
      result = "∆7";
      break;
    case kMinorSeventhHarmony:
      result = "m7";
      break;
    case kDiminishedSeventhHarmony:
      result = "7/5-";
      break;
    case kAugmentedSeventhHarmony:
      result = "7/5+";
      break;
    case kHalfDiminishedHarmony:
      result = "ø7";
      break;
    case kMajorMinorHarmony:
      result = "m∆7";
      break;

    case kMajorSixthHarmony:
      result = "6";
      break;
    case kMinorSixthHarmony:
      result = "m6";
      break;

    case kDominantNinthHarmony:
      result = "9";
      break;
    case kMajorNinthHarmony:
      result = "∆7/9";
      break;
    case kMinorNinthHarmony:
      result = "m∆7/9";
      break;

    case kDominantEleventhHarmony:
      result = "11";
      break;
    case kMajorEleventhHarmony:
      result = "∆7/9/11";
      break;
    case kMinorEleventhHarmony:
      result = "m∆7/9/11";
      break;

    case kDominantThirteenthHarmony:
      result = "13";
      break;
    case kMajorThirteenthHarmony:
      result = "∆7/9/11/13";
      break;
    case kMinorThirteenthHarmony:
      result = "m∆7/9/11/13";
      break;

    case kSuspendedSecondHarmony:
      result = "sus2";
      break;
    case kSuspendedFourthHarmony:
      result = "sus4";
      break;
      
    case kNeapolitanHarmony:
      result = "Neapolitan";
      break;
    case kItalianHarmony:
      result = "Italian";
      break;
    case kFrenchHarmony:
      result = "French";
      break;
    case kGermanHarmony:
      result = "German";
      break;

    case kPedalHarmony:
      result = "Pedal";
      break;
    case kPowerHarmony:
      result = "Power";
      break;
    case kTristanHarmony:
      result = "Tristan";
      break;

    case kOtherHarmony:
      result = "Other";
      break;
    case kNoneHarmony:
      result = "None";
      break;
  } // switch

  return result;
}

void initializeChordIntervalsMap ()
{
  for (int i = k_NoHarmony; i <= kNoneHarmony; i++) {
    msrHarmonyKind
      harmonyKind =
        msrHarmonyKind (i);

    gChordIntervalsMap [harmonyKind] =
      msrChordIntervals::create (
        0, // input line number
        harmonyKind);
  } // for
}

void printChordIntervalsMap ()
{
  gLogIOstream <<
    "Harmonies chord intervals:" <<
    " (" << gChordIntervalsMap.size () << ")" <<
    endl;

  gIndenter++;

  for (int i = k_NoHarmony; i <= kNoneHarmony; i++) {
    msrHarmonyKind
      harmonyKind =
        msrHarmonyKind (i);

    gLogIOstream <<
// JMI      "i:" << i << " " <<
      msrHarmonyKindAsString (harmonyKind) << ":" <<
      endl;

    gIndenter++;
    
    S_msrChordIntervals
      chordIntervals =
        gChordIntervalsMap [harmonyKind];

    if (chordIntervals) {
      gLogIOstream <<
        chordIntervals <<
        endl;
    }
    else {
      gLogIOstream <<
        "no intervals" <<
        endl;
    }

    gIndenter--;
    
    gLogIOstream <<
      endl;
  } // for

  gIndenter--;

  gLogIOstream <<
    endl;
}

// notes names languages
// ------------------------------------------------------

map<string, msrQuarterTonesPitchesLanguageKind>
  gQuarterTonesPitchesLanguageKindsMap;

map<msrQuarterTonesPitchKind, string> gNederlandsPitchName;
map<msrQuarterTonesPitchKind, string> gCatalanPitchName;
map<msrQuarterTonesPitchKind, string> gDeutschPitchName;
map<msrQuarterTonesPitchKind, string> gEnglishPitchName;
map<msrQuarterTonesPitchKind, string> gEspanolPitchName;
map<msrQuarterTonesPitchKind, string> gFrancaisPitchName;
map<msrQuarterTonesPitchKind, string> gItalianoPitchName;
map<msrQuarterTonesPitchKind, string> gNorskPitchName;
map<msrQuarterTonesPitchKind, string> gPortuguesPitchName;
map<msrQuarterTonesPitchKind, string> gSuomiPitchName;
map<msrQuarterTonesPitchKind, string> gSvenskaPitchName;
map<msrQuarterTonesPitchKind, string> gVlaamsPitchName;

void initializeQuarterTonesPitchesLanguageKinds ()
{
  gQuarterTonesPitchesLanguageKindsMap ["nederlands"] = kNederlands;
  gQuarterTonesPitchesLanguageKindsMap ["catalan"]    = kCatalan;
  gQuarterTonesPitchesLanguageKindsMap ["deutsch"]    = kDeutsch;
  gQuarterTonesPitchesLanguageKindsMap ["english"]    = kEnglish;
  gQuarterTonesPitchesLanguageKindsMap ["espanol"]    = kEspanol;
  gQuarterTonesPitchesLanguageKindsMap ["italiano"]   = kItaliano;
  gQuarterTonesPitchesLanguageKindsMap ["francais"]   = kFrancais;
  gQuarterTonesPitchesLanguageKindsMap ["norsk"]      = kNorsk;
  gQuarterTonesPitchesLanguageKindsMap ["portugues"]  = kPortugues;
  gQuarterTonesPitchesLanguageKindsMap ["suomi"]      = kSuomi;
  gQuarterTonesPitchesLanguageKindsMap ["svenska"]    = kSvenska;
  gQuarterTonesPitchesLanguageKindsMap ["vlaams"]     = kVlaams;

  // nederlands
  gNederlandsPitchName [k_NoQuarterTonesPitch]  = "";
  gNederlandsPitchName [k_Rest_QTP]                 = "r";
  
  gNederlandsPitchName [kA_DoubleFlat_QTP]  = "aeses";
  gNederlandsPitchName [kA_SesquiFlat_QTP]  = "aeseh";
  gNederlandsPitchName [kA_Flat_QTP]        = "aes";
  gNederlandsPitchName [kA_SemiFlat_QTP]    = "aeh";
  gNederlandsPitchName [kA_Natural_QTP]     = "a";
  gNederlandsPitchName [kA_SemiSharp_QTP]   = "aih";
  gNederlandsPitchName [kA_Sharp_QTP]       = "ais";
  gNederlandsPitchName [kA_SesquiSharp_QTP] = "aisih";
  gNederlandsPitchName [kA_DoubleSharp_QTP] = "aisis";
    
  gNederlandsPitchName [kB_DoubleFlat_QTP]  = "beses";
  gNederlandsPitchName [kB_SesquiFlat_QTP]  = "beseh";
  gNederlandsPitchName [kB_Flat_QTP]        = "bes";
  gNederlandsPitchName [kB_SemiFlat_QTP]    = "beh";
  gNederlandsPitchName [kB_Natural_QTP]     = "b";
  gNederlandsPitchName [kB_SemiSharp_QTP]   = "bih";
  gNederlandsPitchName [kB_Sharp_QTP]       = "bis";
  gNederlandsPitchName [kB_SesquiSharp_QTP] = "bisih";
  gNederlandsPitchName [kB_DoubleSharp_QTP] = "bisis";
    
  gNederlandsPitchName [kC_DoubleFlat_QTP]  = "ceses";
  gNederlandsPitchName [kC_SesquiFlat_QTP]  = "ceseh";
  gNederlandsPitchName [kC_Flat_QTP]        = "ces";
  gNederlandsPitchName [kC_SemiFlat_QTP]    = "ceh";
  gNederlandsPitchName [kC_Natural_QTP]     = "c";
  gNederlandsPitchName [kC_SemiSharp_QTP]   = "cih";
  gNederlandsPitchName [kC_Sharp_QTP]       = "cis";
  gNederlandsPitchName [kC_SesquiSharp_QTP] = "cisih";
  gNederlandsPitchName [kC_DoubleSharp_QTP] = "cisis";
    
  gNederlandsPitchName [kD_DoubleFlat_QTP]  = "deses";
  gNederlandsPitchName [kD_SesquiFlat_QTP]  = "deseh";
  gNederlandsPitchName [kD_Flat_QTP]        = "des";
  gNederlandsPitchName [kD_SemiFlat_QTP]    = "deh";
  gNederlandsPitchName [kD_Natural_QTP]     = "d";
  gNederlandsPitchName [kD_SemiSharp_QTP]   = "dih";
  gNederlandsPitchName [kD_Sharp_QTP]       = "dis";
  gNederlandsPitchName [kD_SesquiSharp_QTP] = "disih";
  gNederlandsPitchName [kD_DoubleSharp_QTP] = "disis";

  gNederlandsPitchName [kE_DoubleFlat_QTP]  = "eeses";
  gNederlandsPitchName [kE_SesquiFlat_QTP]  = "eeseh";
  gNederlandsPitchName [kE_Flat_QTP]        = "ees";
  gNederlandsPitchName [kE_SemiFlat_QTP]    = "eeh";
  gNederlandsPitchName [kE_Natural_QTP]     = "e";
  gNederlandsPitchName [kE_SemiSharp_QTP]   = "eih";
  gNederlandsPitchName [kE_Sharp_QTP]       = "eis";
  gNederlandsPitchName [kE_SesquiSharp_QTP] = "eisih";
  gNederlandsPitchName [kE_DoubleSharp_QTP] = "eisis";
    
  gNederlandsPitchName [kF_DoubleFlat_QTP]  = "feses";
  gNederlandsPitchName [kF_SesquiFlat_QTP]  = "feseh";
  gNederlandsPitchName [kF_Flat_QTP]        = "fes";
  gNederlandsPitchName [kF_SemiFlat_QTP]    = "feh";
  gNederlandsPitchName [kF_Natural_QTP]     = "f";
  gNederlandsPitchName [kF_SemiSharp_QTP]   = "fih";
  gNederlandsPitchName [kF_Sharp_QTP]       = "fis";
  gNederlandsPitchName [kF_SesquiSharp_QTP] = "fisih";
  gNederlandsPitchName [kF_DoubleSharp_QTP] = "fisis";
    
  gNederlandsPitchName [kG_DoubleFlat_QTP]  = "geses";
  gNederlandsPitchName [kG_SesquiFlat_QTP]  = "geseh";
  gNederlandsPitchName [kG_Flat_QTP]        = "ges";
  gNederlandsPitchName [kG_SemiFlat_QTP]    = "geh";
  gNederlandsPitchName [kG_Natural_QTP]     = "g";
  gNederlandsPitchName [kG_SemiSharp_QTP]   = "gih";
  gNederlandsPitchName [kG_Sharp_QTP]       = "gis";
  gNederlandsPitchName [kG_SesquiSharp_QTP] = "gisih";
  gNederlandsPitchName [kG_DoubleSharp_QTP] = "gisis";

  // catalan
  gCatalanPitchName [k_NoQuarterTonesPitch]  = "";
  gCatalanPitchName [k_Rest_QTP]                 = "r";
  
  gCatalanPitchName [kA_DoubleFlat_QTP]  = "labb";
  gCatalanPitchName [kA_SesquiFlat_QTP]  = "labSesquiFlat???";
  gCatalanPitchName [kA_Flat_QTP]        = "lab";
  gCatalanPitchName [kA_SemiFlat_QTP]    = "aSemiFlat???";
  gCatalanPitchName [kA_Natural_QTP]     = "la";
  gCatalanPitchName [kA_SemiSharp_QTP]   = "aSemiSharp???";
  gCatalanPitchName [kA_Sharp_QTP]       = "lad";
  gCatalanPitchName [kA_SesquiSharp_QTP] = "laSesquiSharp???";
  gCatalanPitchName [kA_DoubleSharp_QTP] = "ladd";
    
  gCatalanPitchName [kB_DoubleFlat_QTP]  = "sibb";
  gCatalanPitchName [kB_SesquiFlat_QTP]  = "sibSesquiFlat???";
  gCatalanPitchName [kB_Flat_QTP]        = "sib";
  gCatalanPitchName [kB_SemiFlat_QTP]    = "bSemiFlat???";
  gCatalanPitchName [kB_Natural_QTP]     = "b";
  gCatalanPitchName [kB_SemiSharp_QTP]   = "bSemiSharp???";
  gCatalanPitchName [kB_Sharp_QTP]       = "sid";
  gCatalanPitchName [kB_SesquiSharp_QTP] = "siSesquiSharp???";
  gCatalanPitchName [kB_DoubleSharp_QTP] = "sidd";
    
  gCatalanPitchName [kC_DoubleFlat_QTP]  = "dobb";
  gCatalanPitchName [kC_SesquiFlat_QTP]  = "doSesquiFlat???";
  gCatalanPitchName [kC_Flat_QTP]        = "dob";
  gCatalanPitchName [kC_SemiFlat_QTP]    = "cSemiFlat???";
  gCatalanPitchName [kC_Natural_QTP]     = "do";
  gCatalanPitchName [kC_SemiSharp_QTP]   = "cSemiSharp???";
  gCatalanPitchName [kC_Sharp_QTP]       = "dod";
  gCatalanPitchName [kC_SesquiSharp_QTP] = "doSesquiSharp???";
  gCatalanPitchName [kC_DoubleSharp_QTP] = "dodd";
    
  gCatalanPitchName [kD_DoubleFlat_QTP]  = "rebb";
  gCatalanPitchName [kD_SesquiFlat_QTP]  = "reSesquiFlat???";
  gCatalanPitchName [kD_Flat_QTP]        = "reb";
  gCatalanPitchName [kD_SemiFlat_QTP]    = "dSemiFlat???";
  gCatalanPitchName [kD_Natural_QTP]     = "re";
  gCatalanPitchName [kD_SemiSharp_QTP]   = "dSemiSharp???";
  gCatalanPitchName [kD_Sharp_QTP]       = "red";
  gCatalanPitchName [kD_SesquiSharp_QTP] = "reSesquiSharp???";
  gCatalanPitchName [kD_DoubleSharp_QTP] = "redd";

  gCatalanPitchName [kE_DoubleFlat_QTP]  = "mibb";
  gCatalanPitchName [kE_SesquiFlat_QTP]  = "miSesquiFlat???";
  gCatalanPitchName [kE_Flat_QTP]        = "mib";
  gCatalanPitchName [kE_SemiFlat_QTP]    = "eSemiFlat???";
  gCatalanPitchName [kE_Natural_QTP]     = "mi";
  gCatalanPitchName [kE_SemiSharp_QTP]   = "eSemiSharp???";
  gCatalanPitchName [kE_Sharp_QTP]       = "mid";
  gCatalanPitchName [kE_SesquiSharp_QTP] = "miSesquiSharp???";
  gCatalanPitchName [kE_DoubleSharp_QTP] = "midd";
    
  gCatalanPitchName [kF_DoubleFlat_QTP]  = "fabb";
  gCatalanPitchName [kF_SesquiFlat_QTP]  = "faSesquiFlat???";
  gCatalanPitchName [kF_Flat_QTP]        = "fab";
  gCatalanPitchName [kF_SemiFlat_QTP]    = "fSemiFlat???";
  gCatalanPitchName [kF_Natural_QTP]     = "fa";
  gCatalanPitchName [kF_SemiSharp_QTP]   = "fSemiSharp???";
  gCatalanPitchName [kF_Sharp_QTP]       = "fad";
  gCatalanPitchName [kF_SesquiSharp_QTP] = "faSesquiSharp???";
  gCatalanPitchName [kF_DoubleSharp_QTP] = "fadd";
    
  gCatalanPitchName [kG_DoubleFlat_QTP]  = "solbb";
  gCatalanPitchName [kG_SesquiFlat_QTP]  = "solSesquiFlat???";
  gCatalanPitchName [kG_Flat_QTP]        = "solb";
  gCatalanPitchName [kG_SemiFlat_QTP]    = "gSemiFlat???";
  gCatalanPitchName [kG_Natural_QTP]     = "sol";
  gCatalanPitchName [kG_SemiSharp_QTP]   = "gSemiSharp???";
  gCatalanPitchName [kG_Sharp_QTP]       = "sold";
  gCatalanPitchName [kG_SesquiSharp_QTP] = "solSesquiSharp???";
  gCatalanPitchName [kG_DoubleSharp_QTP] = "soldd";

  // deutsch
  gDeutschPitchName [k_NoQuarterTonesPitch]  = "";
  gDeutschPitchName [k_Rest_QTP]                 = "r";
  
  gDeutschPitchName [kA_DoubleFlat_QTP]  = "asas";
  gDeutschPitchName [kA_SesquiFlat_QTP]  = "asah";
  gDeutschPitchName [kA_Flat_QTP]        = "as";
  gDeutschPitchName [kA_SemiFlat_QTP]    = "aeh";
  gDeutschPitchName [kA_Natural_QTP]     = "a";
  gDeutschPitchName [kA_SemiSharp_QTP]   = "aih";
  gDeutschPitchName [kA_Sharp_QTP]       = "ais";
  gDeutschPitchName [kA_SesquiSharp_QTP] = "aisih";
  gDeutschPitchName [kA_DoubleSharp_QTP] = "aisis";
    
  gDeutschPitchName [kB_DoubleFlat_QTP]  = "heses";
  gDeutschPitchName [kB_SesquiFlat_QTP]  = "heseh";
  gDeutschPitchName [kB_Flat_QTP]        = "b";
  gDeutschPitchName [kB_SemiFlat_QTP]    = "beh";
  gDeutschPitchName [kB_Natural_QTP]     = "h";
  gDeutschPitchName [kB_SemiSharp_QTP]   = "hih";
  gDeutschPitchName [kB_Sharp_QTP]       = "his";
  gDeutschPitchName [kB_SesquiSharp_QTP] = "hisih";
  gDeutschPitchName [kB_DoubleSharp_QTP] = "hisis";
    
  gDeutschPitchName [kC_DoubleFlat_QTP]  = "ceses";
  gDeutschPitchName [kC_SesquiFlat_QTP]  = "ceseh";
  gDeutschPitchName [kC_Flat_QTP]        = "ces";
  gDeutschPitchName [kC_SemiFlat_QTP]    = "ceh";
  gDeutschPitchName [kC_Natural_QTP]     = "c";
  gDeutschPitchName [kC_SemiSharp_QTP]   = "cih";
  gDeutschPitchName [kC_Sharp_QTP]       = "cis";
  gDeutschPitchName [kC_SesquiSharp_QTP] = "cisih";
  gDeutschPitchName [kC_DoubleSharp_QTP] = "cisis";
    
  gDeutschPitchName [kD_DoubleFlat_QTP]  = "deses";
  gDeutschPitchName [kD_SesquiFlat_QTP]  = "deseh";
  gDeutschPitchName [kD_Flat_QTP]        = "des";
  gDeutschPitchName [kD_SemiFlat_QTP]    = "deh";
  gDeutschPitchName [kD_Natural_QTP]     = "d";
  gDeutschPitchName [kD_SemiSharp_QTP]   = "dih";
  gDeutschPitchName [kD_Sharp_QTP]       = "dis";
  gDeutschPitchName [kD_SesquiSharp_QTP] = "disih";
  gDeutschPitchName [kD_DoubleSharp_QTP] = "disis";

  gDeutschPitchName [kE_DoubleFlat_QTP]  = "eses";
  gDeutschPitchName [kE_SesquiFlat_QTP]  = "esseh";
  gDeutschPitchName [kE_Flat_QTP]        = "es";
  gDeutschPitchName [kE_SemiFlat_QTP]    = "eeh";
  gDeutschPitchName [kE_Natural_QTP]     = "e";
  gDeutschPitchName [kE_SemiSharp_QTP]   = "eih";
  gDeutschPitchName [kE_Sharp_QTP]       = "eis";
  gDeutschPitchName [kE_SesquiSharp_QTP] = "eisih";
  gDeutschPitchName [kE_DoubleSharp_QTP] = "eisis";
    
  gDeutschPitchName [kF_DoubleFlat_QTP]  = "feses";
  gDeutschPitchName [kF_SesquiFlat_QTP]  = "feseh";
  gDeutschPitchName [kF_Flat_QTP]        = "fes";
  gDeutschPitchName [kF_SemiFlat_QTP]    = "feh";
  gDeutschPitchName [kF_Natural_QTP]     = "f";
  gDeutschPitchName [kF_SemiSharp_QTP]   = "fih";
  gDeutschPitchName [kF_Sharp_QTP]       = "fis";
  gDeutschPitchName [kF_SesquiSharp_QTP] = "fisih";
  gDeutschPitchName [kF_DoubleSharp_QTP] = "fisis";
    
  gDeutschPitchName [kG_DoubleFlat_QTP]  = "geses";
  gDeutschPitchName [kG_SesquiFlat_QTP]  = "geseh";
  gDeutschPitchName [kG_Flat_QTP]        = "ges";
  gDeutschPitchName [kG_SemiFlat_QTP]    = "geh";
  gDeutschPitchName [kG_Natural_QTP]     = "g";
  gDeutschPitchName [kG_SemiSharp_QTP]   = "gih";
  gDeutschPitchName [kG_Sharp_QTP]       = "gis";
  gDeutschPitchName [kG_SesquiSharp_QTP] = "gisih";
  gDeutschPitchName [kG_DoubleSharp_QTP] = "gisis";

  // english
  gEnglishPitchName [k_NoQuarterTonesPitch]  = "";
  gEnglishPitchName [k_Rest_QTP]                 = "r";
  
  gEnglishPitchName [kA_DoubleFlat_QTP]  = "aff";
  gEnglishPitchName [kA_SesquiFlat_QTP]  = "atqf";
  gEnglishPitchName [kA_Flat_QTP]        = "af";
  gEnglishPitchName [kA_SemiFlat_QTP]    = "aqf";
  gEnglishPitchName [kA_Natural_QTP]     = "a";
  gEnglishPitchName [kA_SemiSharp_QTP]   = "aqs";
  gEnglishPitchName [kA_Sharp_QTP]       = "as";
  gEnglishPitchName [kA_SesquiSharp_QTP] = "atqs";
  gEnglishPitchName [kA_DoubleSharp_QTP] = "a";
    
  gEnglishPitchName [kB_DoubleFlat_QTP]  = "bfqf";
  gEnglishPitchName [kB_SesquiFlat_QTP]  = "btqf";
  gEnglishPitchName [kB_Flat_QTP]        = "bf";
  gEnglishPitchName [kB_SemiFlat_QTP]    = "bqf";
  gEnglishPitchName [kB_Natural_QTP]     = "b";
  gEnglishPitchName [kB_SemiSharp_QTP]   = "bqs";
  gEnglishPitchName [kB_Sharp_QTP]       = "bs";
  gEnglishPitchName [kB_SesquiSharp_QTP] = "btqs";
  gEnglishPitchName [kB_DoubleSharp_QTP] = "bx";
    
  gEnglishPitchName [kC_DoubleFlat_QTP]  = "cff";
  gEnglishPitchName [kC_SesquiFlat_QTP]  = "ctqf";
  gEnglishPitchName [kC_Flat_QTP]        = "cf";
  gEnglishPitchName [kC_SemiFlat_QTP]    = "cqf";
  gEnglishPitchName [kC_Natural_QTP]     = "c";
  gEnglishPitchName [kC_SemiSharp_QTP]   = "cqs";
  gEnglishPitchName [kC_Sharp_QTP]       = "cs";
  gEnglishPitchName [kC_SesquiSharp_QTP] = "ctqs";
  gEnglishPitchName [kC_DoubleSharp_QTP] = "cx";
    
  gEnglishPitchName [kD_DoubleFlat_QTP]  = "dff";
  gEnglishPitchName [kD_SesquiFlat_QTP]  = "dtqf";
  gEnglishPitchName [kD_Flat_QTP]        = "df";
  gEnglishPitchName [kD_SemiFlat_QTP]    = "dqf";
  gEnglishPitchName [kD_Natural_QTP]     = "d";
  gEnglishPitchName [kD_SemiSharp_QTP]   = "dqs";
  gEnglishPitchName [kD_Sharp_QTP]       = "ds";
  gEnglishPitchName [kD_SesquiSharp_QTP] = "dtqs";
  gEnglishPitchName [kD_DoubleSharp_QTP] = "dx";

  gEnglishPitchName [kE_DoubleFlat_QTP]  = "eff";
  gEnglishPitchName [kE_SesquiFlat_QTP]  = "etqf";
  gEnglishPitchName [kE_Flat_QTP]        = "ef";
  gEnglishPitchName [kE_SemiFlat_QTP]    = "eqf";
  gEnglishPitchName [kE_Natural_QTP]     = "e";
  gEnglishPitchName [kE_SemiSharp_QTP]   = "eqs";
  gEnglishPitchName [kE_Sharp_QTP]       = "es";
  gEnglishPitchName [kE_SesquiSharp_QTP] = "etqs";
  gEnglishPitchName [kE_DoubleSharp_QTP] = "ex";
    
  gEnglishPitchName [kF_DoubleFlat_QTP]  = "fff";
  gEnglishPitchName [kF_SesquiFlat_QTP]  = "ftqf";
  gEnglishPitchName [kF_Flat_QTP]        = "ff";
  gEnglishPitchName [kF_SemiFlat_QTP]    = "fqf";
  gEnglishPitchName [kF_Natural_QTP]     = "f";
  gEnglishPitchName [kF_SemiSharp_QTP]   = "fqs";
  gEnglishPitchName [kF_Sharp_QTP]       = "fs";
  gEnglishPitchName [kF_SesquiSharp_QTP] = "ftqs";
  gEnglishPitchName [kF_DoubleSharp_QTP] = "fx";
    
  gEnglishPitchName [kG_DoubleFlat_QTP]  = "gff";
  gEnglishPitchName [kG_SesquiFlat_QTP]  = "gtqf";
  gEnglishPitchName [kG_Flat_QTP]        = "gf";
  gEnglishPitchName [kG_SemiFlat_QTP]    = "gqf";
  gEnglishPitchName [kG_Natural_QTP]     = "g";
  gEnglishPitchName [kG_SemiSharp_QTP]   = "gqs";
  gEnglishPitchName [kG_Sharp_QTP]       = "gs";
  gEnglishPitchName [kG_SesquiSharp_QTP] = "gtqs";
  gEnglishPitchName [kG_DoubleSharp_QTP] = "gx";

  // espanol
  gEspanolPitchName [k_NoQuarterTonesPitch]  = "";
  gEspanolPitchName [k_Rest_QTP]                 = "r";
  
  gEspanolPitchName [kA_DoubleFlat_QTP]  = "labb";
  gEspanolPitchName [kA_SesquiFlat_QTP]  = "latcb";
  gEspanolPitchName [kA_Flat_QTP]        = "lab";
  gEspanolPitchName [kA_SemiFlat_QTP]    = "lacb";
  gEspanolPitchName [kA_Natural_QTP]     = "la";
  gEspanolPitchName [kA_SemiSharp_QTP]   = "lacs";
  gEspanolPitchName [kA_Sharp_QTP]       = "las";
  gEspanolPitchName [kA_SesquiSharp_QTP] = "latcs";
  gEspanolPitchName [kA_DoubleSharp_QTP] = "lax";
    
  gEspanolPitchName [kB_DoubleFlat_QTP]  = "sibb";
  gEspanolPitchName [kB_SesquiFlat_QTP]  = "sitcb";
  gEspanolPitchName [kB_Flat_QTP]        = "sib";
  gEspanolPitchName [kB_SemiFlat_QTP]    = "sicb";
  gEspanolPitchName [kB_Natural_QTP]     = "si";
  gEspanolPitchName [kB_SemiSharp_QTP]   = "sics";
  gEspanolPitchName [kB_Sharp_QTP]       = "sis";
  gEspanolPitchName [kB_SesquiSharp_QTP] = "sitcs";
  gEspanolPitchName [kB_DoubleSharp_QTP] = "six";
    
  gEspanolPitchName [kC_DoubleFlat_QTP]  = "dobb";
  gEspanolPitchName [kC_SesquiFlat_QTP]  = "dotcb";
  gEspanolPitchName [kC_Flat_QTP]        = "dob";
  gEspanolPitchName [kC_SemiFlat_QTP]    = "docb";
  gEspanolPitchName [kC_Natural_QTP]     = "do";
  gEspanolPitchName [kC_SemiSharp_QTP]   = "docs";
  gEspanolPitchName [kC_Sharp_QTP]       = "dos";
  gEspanolPitchName [kC_SesquiSharp_QTP] = "dotcs";
  gEspanolPitchName [kC_DoubleSharp_QTP] = "dox";
    
  gEspanolPitchName [kD_DoubleFlat_QTP]  = "rebb";
  gEspanolPitchName [kD_SesquiFlat_QTP]  = "retcb";
  gEspanolPitchName [kD_Flat_QTP]        = "reb";
  gEspanolPitchName [kD_SemiFlat_QTP]    = "recb";
  gEspanolPitchName [kD_Natural_QTP]     = "re";
  gEspanolPitchName [kD_SemiSharp_QTP]   = "recs";
  gEspanolPitchName [kD_Sharp_QTP]       = "res";
  gEspanolPitchName [kD_SesquiSharp_QTP] = "retcs";
  gEspanolPitchName [kD_DoubleSharp_QTP] = "rex";

  gEspanolPitchName [kE_DoubleFlat_QTP]  = "mibb";
  gEspanolPitchName [kE_SesquiFlat_QTP]  = "mitcb";
  gEspanolPitchName [kE_Flat_QTP]        = "mib";
  gEspanolPitchName [kE_SemiFlat_QTP]    = "micb";
  gEspanolPitchName [kE_Natural_QTP]     = "mi";
  gEspanolPitchName [kE_SemiSharp_QTP]   = "mics";
  gEspanolPitchName [kE_Sharp_QTP]       = "mis";
  gEspanolPitchName [kE_SesquiSharp_QTP] = "mitcs";
  gEspanolPitchName [kE_DoubleSharp_QTP] = "mix";
    
  gEspanolPitchName [kF_DoubleFlat_QTP]  = "fabb";
  gEspanolPitchName [kF_SesquiFlat_QTP]  = "fatcb";
  gEspanolPitchName [kF_Flat_QTP]        = "fab";
  gEspanolPitchName [kF_SemiFlat_QTP]    = "facb";
  gEspanolPitchName [kF_Natural_QTP]     = "fa";
  gEspanolPitchName [kF_SemiSharp_QTP]   = "facs";
  gEspanolPitchName [kF_Sharp_QTP]       = "fas";
  gEspanolPitchName [kF_SesquiSharp_QTP] = "fatcs";
  gEspanolPitchName [kF_DoubleSharp_QTP] = "fax";
    
  gEspanolPitchName [kG_DoubleFlat_QTP]  = "solbb";
  gEspanolPitchName [kG_SesquiFlat_QTP]  = "soltcb";
  gEspanolPitchName [kG_Flat_QTP]        = "solb";
  gEspanolPitchName [kG_SemiFlat_QTP]    = "solcb";
  gEspanolPitchName [kG_Natural_QTP]     = "sol";
  gEspanolPitchName [kG_SemiSharp_QTP]   = "solcs";
  gEspanolPitchName [kG_Sharp_QTP]       = "sols";
  gEspanolPitchName [kG_SesquiSharp_QTP] = "soltcs";
  gEspanolPitchName [kG_DoubleSharp_QTP] = "solx";

  // francais
  gFrancaisPitchName [k_NoQuarterTonesPitch]  = "";
  gFrancaisPitchName [k_Rest_QTP]                 = "r";
  
  gFrancaisPitchName [kA_DoubleFlat_QTP]  = "labb";
  gFrancaisPitchName [kA_SesquiFlat_QTP]  = "labtqt";
  gFrancaisPitchName [kA_Flat_QTP]        = "lab";
  gFrancaisPitchName [kA_SemiFlat_QTP]    = "labqt";
  gFrancaisPitchName [kA_Natural_QTP]     = "la";
  gFrancaisPitchName [kA_SemiSharp_QTP]   = "lasqt";
  gFrancaisPitchName [kA_Sharp_QTP]       = "lad";
  gFrancaisPitchName [kA_SesquiSharp_QTP] = "lastqt";
  gFrancaisPitchName [kA_DoubleSharp_QTP] = "lass";
    
  gFrancaisPitchName [kB_DoubleFlat_QTP]  = "sibb";
  gFrancaisPitchName [kB_SesquiFlat_QTP]  = "sibtqt";
  gFrancaisPitchName [kB_Flat_QTP]        = "sib";
  gFrancaisPitchName [kB_SemiFlat_QTP]    = "sibqt";
  gFrancaisPitchName [kB_Natural_QTP]     = "si";
  gFrancaisPitchName [kB_SemiSharp_QTP]   = "sisqt";
  gFrancaisPitchName [kB_Sharp_QTP]       = "sid";
  gFrancaisPitchName [kB_SesquiSharp_QTP] = "sistqt";
  gFrancaisPitchName [kB_DoubleSharp_QTP] = "siss";
    
  gFrancaisPitchName [kC_DoubleFlat_QTP]  = "dobb";
  gFrancaisPitchName [kC_SesquiFlat_QTP]  = "dobtqt";
  gFrancaisPitchName [kC_Flat_QTP]        = "dob";
  gFrancaisPitchName [kC_SemiFlat_QTP]    = "dobqt";
  gFrancaisPitchName [kC_Natural_QTP]     = "do";
  gFrancaisPitchName [kC_SemiSharp_QTP]   = "dosqt";
  gFrancaisPitchName [kC_Sharp_QTP]       = "dod";
  gFrancaisPitchName [kC_SesquiSharp_QTP] = "dostqt";
  gFrancaisPitchName [kC_DoubleSharp_QTP] = "doss";
    
  gFrancaisPitchName [kD_DoubleFlat_QTP]  = "rebb";
  gFrancaisPitchName [kD_SesquiFlat_QTP]  = "rebtqt";
  gFrancaisPitchName [kD_Flat_QTP]        = "reb";
  gFrancaisPitchName [kD_SemiFlat_QTP]    = "rebqt";
  gFrancaisPitchName [kD_Natural_QTP]     = "re";
  gFrancaisPitchName [kD_SemiSharp_QTP]   = "resqt";
  gFrancaisPitchName [kD_Sharp_QTP]       = "red";
  gFrancaisPitchName [kD_SesquiSharp_QTP] = "restqt";
  gFrancaisPitchName [kD_DoubleSharp_QTP] = "ress";

  gFrancaisPitchName [kE_DoubleFlat_QTP]  = "mibb";
  gFrancaisPitchName [kE_SesquiFlat_QTP]  = "mibtqt";
  gFrancaisPitchName [kE_Flat_QTP]        = "mib";
  gFrancaisPitchName [kE_SemiFlat_QTP]    = "mibqt";
  gFrancaisPitchName [kE_Natural_QTP]     = "mi";
  gFrancaisPitchName [kE_SemiSharp_QTP]   = "misqt";
  gFrancaisPitchName [kE_Sharp_QTP]       = "mid";
  gFrancaisPitchName [kE_SesquiSharp_QTP] = "mistqt";
  gFrancaisPitchName [kE_DoubleSharp_QTP] = "miss";
    
  gFrancaisPitchName [kF_DoubleFlat_QTP]  = "fabb";
  gFrancaisPitchName [kF_SesquiFlat_QTP]  = "fabtqt";
  gFrancaisPitchName [kF_Flat_QTP]        = "fab";
  gFrancaisPitchName [kF_SemiFlat_QTP]    = "fabqt";
  gFrancaisPitchName [kF_Natural_QTP]     = "fa";
  gFrancaisPitchName [kF_SemiSharp_QTP]   = "fasqt";
  gFrancaisPitchName [kF_Sharp_QTP]       = "fad";
  gFrancaisPitchName [kF_SesquiSharp_QTP] = "fastqt";
  gFrancaisPitchName [kF_DoubleSharp_QTP] = "fass";
    
  gFrancaisPitchName [kG_DoubleFlat_QTP]  = "solbb";
  gFrancaisPitchName [kG_SesquiFlat_QTP]  = "solbtqt";
  gFrancaisPitchName [kG_Flat_QTP]        = "solb";
  gFrancaisPitchName [kG_SemiFlat_QTP]    = "solbqt";
  gFrancaisPitchName [kG_Natural_QTP]     = "sol";
  gFrancaisPitchName [kG_SemiSharp_QTP]   = "solsqt";
  gFrancaisPitchName [kG_Sharp_QTP]       = "sold";
  gFrancaisPitchName [kG_SesquiSharp_QTP] = "solstqt";
  gFrancaisPitchName [kG_DoubleSharp_QTP] = "solss";

  // italiano
  gItalianoPitchName [k_NoQuarterTonesPitch]  = "";
  gItalianoPitchName [k_Rest_QTP]                 = "r";
  
  gItalianoPitchName [kA_DoubleFlat_QTP]  = "labb";
  gItalianoPitchName [kA_SesquiFlat_QTP]  = "labsb";
  gItalianoPitchName [kA_Flat_QTP]        = "lab";
  gItalianoPitchName [kA_SemiFlat_QTP]    = "lasb";
  gItalianoPitchName [kA_Natural_QTP]     = "la";
  gItalianoPitchName [kA_SemiSharp_QTP]   = "lasd";
  gItalianoPitchName [kA_Sharp_QTP]       = "lad";
  gItalianoPitchName [kA_SesquiSharp_QTP] = "ladsd";
  gItalianoPitchName [kA_DoubleSharp_QTP] = "ladd";
    
  gItalianoPitchName [kB_DoubleFlat_QTP]  = "sibb";
  gItalianoPitchName [kB_SesquiFlat_QTP]  = "sibsb";
  gItalianoPitchName [kB_Flat_QTP]        = "sib";
  gItalianoPitchName [kB_SemiFlat_QTP]    = "sisb";
  gItalianoPitchName [kB_Natural_QTP]     = "si";
  gItalianoPitchName [kB_SemiSharp_QTP]   = "sisd";
  gItalianoPitchName [kB_Sharp_QTP]       = "sid";
  gItalianoPitchName [kB_SesquiSharp_QTP] = "sidsd";
  gItalianoPitchName [kB_DoubleSharp_QTP] = "sidd";
    
  gItalianoPitchName [kC_DoubleFlat_QTP]  = "dobb";
  gItalianoPitchName [kC_SesquiFlat_QTP]  = "dobsb";
  gItalianoPitchName [kC_Flat_QTP]        = "dob";
  gItalianoPitchName [kC_SemiFlat_QTP]    = "dosb";
  gItalianoPitchName [kC_Natural_QTP]     = "do";
  gItalianoPitchName [kC_SemiSharp_QTP]   = "dosd";
  gItalianoPitchName [kC_Sharp_QTP]       = "dod";
  gItalianoPitchName [kC_SesquiSharp_QTP] = "dodsd";
  gItalianoPitchName [kC_DoubleSharp_QTP] = "dodd";
    
  gItalianoPitchName [kD_DoubleFlat_QTP]  = "rebb";
  gItalianoPitchName [kD_SesquiFlat_QTP]  = "rebsb";
  gItalianoPitchName [kD_Flat_QTP]        = "reb";
  gItalianoPitchName [kD_SemiFlat_QTP]    = "resb";
  gItalianoPitchName [kD_Natural_QTP]     = "re";
  gItalianoPitchName [kD_SemiSharp_QTP]   = "resd";
  gItalianoPitchName [kD_Sharp_QTP]       = "red";
  gItalianoPitchName [kD_SesquiSharp_QTP] = "redsd";
  gItalianoPitchName [kD_DoubleSharp_QTP] = "redd";

  gItalianoPitchName [kE_DoubleFlat_QTP]  = "mibb";
  gItalianoPitchName [kE_SesquiFlat_QTP]  = "mibsb";
  gItalianoPitchName [kE_Flat_QTP]        = "mib";
  gItalianoPitchName [kE_SemiFlat_QTP]    = "misb";
  gItalianoPitchName [kE_Natural_QTP]     = "mi";
  gItalianoPitchName [kE_SemiSharp_QTP]   = "misd";
  gItalianoPitchName [kE_Sharp_QTP]       = "mid";
  gItalianoPitchName [kE_SesquiSharp_QTP] = "midsd";
  gItalianoPitchName [kE_DoubleSharp_QTP] = "midd";
    
  gItalianoPitchName [kF_DoubleFlat_QTP]  = "fabb";
  gItalianoPitchName [kF_SesquiFlat_QTP]  = "fabsb";
  gItalianoPitchName [kF_Flat_QTP]        = "fab";
  gItalianoPitchName [kF_SemiFlat_QTP]    = "fasb";
  gItalianoPitchName [kF_Natural_QTP]     = "fa";
  gItalianoPitchName [kF_SemiSharp_QTP]   = "fasd";
  gItalianoPitchName [kF_Sharp_QTP]       = "fad";
  gItalianoPitchName [kF_SesquiSharp_QTP] = "fadsd";
  gItalianoPitchName [kF_DoubleSharp_QTP] = "fadd";
    
  gItalianoPitchName [kG_DoubleFlat_QTP]  = "solbb";
  gItalianoPitchName [kG_SesquiFlat_QTP]  = "solbsb";
  gItalianoPitchName [kG_Flat_QTP]        = "solb";
  gItalianoPitchName [kG_SemiFlat_QTP]    = "solsb";
  gItalianoPitchName [kG_Natural_QTP]     = "sol";
  gItalianoPitchName [kG_SemiSharp_QTP]   = "solsd";
  gItalianoPitchName [kG_Sharp_QTP]       = "sold";
  gItalianoPitchName [kG_SesquiSharp_QTP] = "soldsd";
  gItalianoPitchName [kG_DoubleSharp_QTP] = "soldd";

  // norsk
  gNorskPitchName [k_NoQuarterTonesPitch]  = "";
  gNorskPitchName [k_Rest_QTP]                 = "r";
  
  gNorskPitchName [kA_DoubleFlat_QTP]  = "aeses";
  gNorskPitchName [kA_SesquiFlat_QTP]  = "aSesquiFlat???";
  gNorskPitchName [kA_Flat_QTP]        = "aes";
  gNorskPitchName [kA_SemiFlat_QTP]    = "aSemiFlat???";
  gNorskPitchName [kA_Natural_QTP]     = "a";
  gNorskPitchName [kA_SemiSharp_QTP]   = "aSemiSharp???";
  gNorskPitchName [kA_Sharp_QTP]       = "ais";
  gNorskPitchName [kA_SesquiSharp_QTP] = "aSesquiSharp???";
  gNorskPitchName [kA_DoubleSharp_QTP] = "aisis";
    
  gNorskPitchName [kB_DoubleFlat_QTP]  = "beses";
  gNorskPitchName [kB_SesquiFlat_QTP]  = "bSesquiFlat???";
  gNorskPitchName [kB_Flat_QTP]        = "bes";
  gNorskPitchName [kB_SemiFlat_QTP]    = "bSemiFlat???";
  gNorskPitchName [kB_Natural_QTP]     = "b";
  gNorskPitchName [kB_SemiSharp_QTP]   = "bSemiSharp???";
  gNorskPitchName [kB_Sharp_QTP]       = "bis";
  gNorskPitchName [kB_SesquiSharp_QTP] = "bSesquiSharp???";
  gNorskPitchName [kB_DoubleSharp_QTP] = "bisis";
    
  gNorskPitchName [kC_DoubleFlat_QTP]  = "ceses";
  gNorskPitchName [kC_SesquiFlat_QTP]  = "cSesquiFlat???";
  gNorskPitchName [kC_Flat_QTP]        = "ces";
  gNorskPitchName [kC_SemiFlat_QTP]    = "cSemiFlat???";
  gNorskPitchName [kC_Natural_QTP]     = "c";
  gNorskPitchName [kC_SemiSharp_QTP]   = "cSemiSharp???";
  gNorskPitchName [kC_Sharp_QTP]       = "cis";
  gNorskPitchName [kC_SesquiSharp_QTP] = "cSesquiSharp???";
  gNorskPitchName [kC_DoubleSharp_QTP] = "cisis";
    
  gNorskPitchName [kD_DoubleFlat_QTP]  = "deses";
  gNorskPitchName [kD_SesquiFlat_QTP]  = "dSesquiFlat???";
  gNorskPitchName [kD_Flat_QTP]        = "des";
  gNorskPitchName [kD_SemiFlat_QTP]    = "dSemiFlat???";
  gNorskPitchName [kD_Natural_QTP]     = "d";
  gNorskPitchName [kD_SemiSharp_QTP]   = "dSemiSharp???";
  gNorskPitchName [kD_Sharp_QTP]       = "dis";
  gNorskPitchName [kD_SesquiSharp_QTP] = "dSesquiSharp???";
  gNorskPitchName [kD_DoubleSharp_QTP] = "disis";

  gNorskPitchName [kE_DoubleFlat_QTP]  = "eeses";
  gNorskPitchName [kE_SesquiFlat_QTP]  = "eSesquiFlat???";
  gNorskPitchName [kE_Flat_QTP]        = "ees";
  gNorskPitchName [kE_SemiFlat_QTP]    = "eSemiFlat???";
  gNorskPitchName [kE_Natural_QTP]     = "e";
  gNorskPitchName [kE_SemiSharp_QTP]   = "eSemiSharp???";
  gNorskPitchName [kE_Sharp_QTP]       = "eis";
  gNorskPitchName [kE_SesquiSharp_QTP] = "eSesquiSharp???";
  gNorskPitchName [kE_DoubleSharp_QTP] = "eisis";
    
  gNorskPitchName [kF_DoubleFlat_QTP]  = "feses";
  gNorskPitchName [kF_SesquiFlat_QTP]  = "fSesquiFlat???";
  gNorskPitchName [kF_Flat_QTP]        = "fes";
  gNorskPitchName [kF_SemiFlat_QTP]    = "fSemiFlat???";
  gNorskPitchName [kF_Natural_QTP]     = "f";
  gNorskPitchName [kF_SemiSharp_QTP]   = "fSemiSharp???";
  gNorskPitchName [kF_Sharp_QTP]       = "fis";
  gNorskPitchName [kF_SesquiSharp_QTP] = "fSesquiSharp???";
  gNorskPitchName [kF_DoubleSharp_QTP] = "fisis";
    
  gNorskPitchName [kG_DoubleFlat_QTP]  = "geses";
  gNorskPitchName [kG_SesquiFlat_QTP]  = "gSesquiFlat???";
  gNorskPitchName [kG_Flat_QTP]        = "ges";
  gNorskPitchName [kG_SemiFlat_QTP]    = "gSemiFlat???";
  gNorskPitchName [kG_Natural_QTP]     = "g";
  gNorskPitchName [kG_SemiSharp_QTP]   = "gSemiSharp???";
  gNorskPitchName [kG_Sharp_QTP]       = "gis";
  gNorskPitchName [kG_SesquiSharp_QTP] = "gSesquiSharp???";
  gNorskPitchName [kG_DoubleSharp_QTP] = "gisis";

  // portugues
  gPortuguesPitchName [k_NoQuarterTonesPitch]  = "";
  gPortuguesPitchName [k_Rest_QTP]                 = "r";
  
  gPortuguesPitchName [kA_DoubleFlat_QTP]  = "labb";
  gPortuguesPitchName [kA_SesquiFlat_QTP]  = "labtqt";
  gPortuguesPitchName [kA_Flat_QTP]        = "lab";
  gPortuguesPitchName [kA_SemiFlat_QTP]    = "lasb";
  gPortuguesPitchName [kA_Natural_QTP]     = "la";
  gPortuguesPitchName [kA_SemiSharp_QTP]   = "lasd";
  gPortuguesPitchName [kA_Sharp_QTP]       = "lad";
  gPortuguesPitchName [kA_SesquiSharp_QTP] = "ladsd";
  gPortuguesPitchName [kA_DoubleSharp_QTP] = "ladd";
    
  gPortuguesPitchName [kB_DoubleFlat_QTP]  = "sibb";
  gPortuguesPitchName [kB_SesquiFlat_QTP]  = "sibtqt";
  gPortuguesPitchName [kB_Flat_QTP]        = "sib";
  gPortuguesPitchName [kB_SemiFlat_QTP]    = "sisb";
  gPortuguesPitchName [kB_Natural_QTP]     = "si";
  gPortuguesPitchName [kB_SemiSharp_QTP]   = "sisd";
  gPortuguesPitchName [kB_Sharp_QTP]       = "sid";
  gPortuguesPitchName [kB_SesquiSharp_QTP] = "sidsd";
  gPortuguesPitchName [kB_DoubleSharp_QTP] = "sidd";
    
  gPortuguesPitchName [kC_DoubleFlat_QTP]  = "dobb";
  gPortuguesPitchName [kC_SesquiFlat_QTP]  = "dobtqt";
  gPortuguesPitchName [kC_Flat_QTP]        = "dob";
  gPortuguesPitchName [kC_SemiFlat_QTP]    = "dosb";
  gPortuguesPitchName [kC_Natural_QTP]     = "do";
  gPortuguesPitchName [kC_SemiSharp_QTP]   = "dosd";
  gPortuguesPitchName [kC_Sharp_QTP]       = "dod";
  gPortuguesPitchName [kC_SesquiSharp_QTP] = "dodsd";
  gPortuguesPitchName [kC_DoubleSharp_QTP] = "dodd";
    
  gPortuguesPitchName [kD_DoubleFlat_QTP]  = "rebb";
  gPortuguesPitchName [kD_SesquiFlat_QTP]  = "rebtqt";
  gPortuguesPitchName [kD_Flat_QTP]        = "reb";
  gPortuguesPitchName [kD_SemiFlat_QTP]    = "resb";
  gPortuguesPitchName [kD_Natural_QTP]     = "re";
  gPortuguesPitchName [kD_SemiSharp_QTP]   = "resd";
  gPortuguesPitchName [kD_Sharp_QTP]       = "red";
  gPortuguesPitchName [kD_SesquiSharp_QTP] = "redsd";
  gPortuguesPitchName [kD_DoubleSharp_QTP] = "redd";

  gPortuguesPitchName [kE_DoubleFlat_QTP]  = "mibb";
  gPortuguesPitchName [kE_SesquiFlat_QTP]  = "mibtqt";
  gPortuguesPitchName [kE_Flat_QTP]        = "mib";
  gPortuguesPitchName [kE_SemiFlat_QTP]    = "misb";
  gPortuguesPitchName [kE_Natural_QTP]     = "mi";
  gPortuguesPitchName [kE_SemiSharp_QTP]   = "misd";
  gPortuguesPitchName [kE_Sharp_QTP]       = "mid";
  gPortuguesPitchName [kE_SesquiSharp_QTP] = "midsd";
  gPortuguesPitchName [kE_DoubleSharp_QTP] = "midd";
    
  gPortuguesPitchName [kF_DoubleFlat_QTP]  = "fabb";
  gPortuguesPitchName [kF_SesquiFlat_QTP]  = "fabtqt";
  gPortuguesPitchName [kF_Flat_QTP]        = "fab";
  gPortuguesPitchName [kF_SemiFlat_QTP]    = "fasb";
  gPortuguesPitchName [kF_Natural_QTP]     = "fa";
  gPortuguesPitchName [kF_SemiSharp_QTP]   = "fasd";
  gPortuguesPitchName [kF_Sharp_QTP]       = "fad";
  gPortuguesPitchName [kF_SesquiSharp_QTP] = "fadsd";
  gPortuguesPitchName [kF_DoubleSharp_QTP] = "fadd";
    
  gPortuguesPitchName [kG_DoubleFlat_QTP]  = "solbb";
  gPortuguesPitchName [kG_SesquiFlat_QTP]  = "solbtqt";
  gPortuguesPitchName [kG_Flat_QTP]        = "solb";
  gPortuguesPitchName [kG_SemiFlat_QTP]    = "solsb";
  gPortuguesPitchName [kG_Natural_QTP]     = "sol";
  gPortuguesPitchName [kG_SemiSharp_QTP]   = "solsd";
  gPortuguesPitchName [kG_Sharp_QTP]       = "sold";
  gPortuguesPitchName [kG_SesquiSharp_QTP] = "soldsd";
  gPortuguesPitchName [kG_DoubleSharp_QTP] = "soldd";

  // suomi
  gSuomiPitchName [k_NoQuarterTonesPitch]  = "";
  gSuomiPitchName [k_Rest_QTP]                 = "r";
  
  gSuomiPitchName [kA_DoubleFlat_QTP]  = "asas";
  gSuomiPitchName [kA_SesquiFlat_QTP]  = "aSesquiFlat???";
  gSuomiPitchName [kA_Flat_QTP]        = "as";
  gSuomiPitchName [kA_SemiFlat_QTP]    = "aSemiFlat???";
  gSuomiPitchName [kA_Natural_QTP]     = "a";
  gSuomiPitchName [kA_SemiSharp_QTP]   = "aSemiSharp???";
  gSuomiPitchName [kA_Sharp_QTP]       = "ais";
  gSuomiPitchName [kA_SesquiSharp_QTP] = "aSesquiSharp???";
  gSuomiPitchName [kA_DoubleSharp_QTP] = "aisis";
    
  gSuomiPitchName [kB_DoubleFlat_QTP]  = "bes";
  gSuomiPitchName [kB_SesquiFlat_QTP]  = "bSesquiFlat???";
  gSuomiPitchName [kB_Flat_QTP]        = "b";
  gSuomiPitchName [kB_SemiFlat_QTP]    = "bSemiFlat???";
  gSuomiPitchName [kB_Natural_QTP]     = "h";
  gSuomiPitchName [kB_SemiSharp_QTP]   = "bSemiSharp???";
  gSuomiPitchName [kB_Sharp_QTP]       = "his";
  gSuomiPitchName [kB_SesquiSharp_QTP] = "bSesquiSharp???";
  gSuomiPitchName [kB_DoubleSharp_QTP] = "hisis";
    
  gSuomiPitchName [kC_DoubleFlat_QTP]  = "ceses";
  gSuomiPitchName [kC_SesquiFlat_QTP]  = "cSesquiFlat???";
  gSuomiPitchName [kC_Flat_QTP]        = "ces";
  gSuomiPitchName [kC_SemiFlat_QTP]    = "cSemiFlat???";
  gSuomiPitchName [kC_Natural_QTP]     = "c";
  gSuomiPitchName [kC_SemiSharp_QTP]   = "cSemiSharp???";
  gSuomiPitchName [kC_Sharp_QTP]       = "cis";
  gSuomiPitchName [kC_SesquiSharp_QTP] = "cSesquiSharp???";
  gSuomiPitchName [kC_DoubleSharp_QTP] = "cisis";
    
  gSuomiPitchName [kD_DoubleFlat_QTP]  = "deses";
  gSuomiPitchName [kD_SesquiFlat_QTP]  = "dSesquiFlat???";
  gSuomiPitchName [kD_Flat_QTP]        = "des";
  gSuomiPitchName [kD_SemiFlat_QTP]    = "dSemiFlat???";
  gSuomiPitchName [kD_Natural_QTP]     = "d";
  gSuomiPitchName [kD_SemiSharp_QTP]   = "dSemiSharp???";
  gSuomiPitchName [kD_Sharp_QTP]       = "dis";
  gSuomiPitchName [kD_SesquiSharp_QTP] = "dSesquiSharp???";
  gSuomiPitchName [kD_DoubleSharp_QTP] = "disis";

  gSuomiPitchName [kE_DoubleFlat_QTP]  = "eses";
  gSuomiPitchName [kE_SesquiFlat_QTP]  = "eSesquiFlat???";
  gSuomiPitchName [kE_Flat_QTP]        = "es";
  gSuomiPitchName [kE_SemiFlat_QTP]    = "eSemiFlat???";
  gSuomiPitchName [kE_Natural_QTP]     = "e";
  gSuomiPitchName [kE_SemiSharp_QTP]   = "eSemiSharp???";
  gSuomiPitchName [kE_Sharp_QTP]       = "eis";
  gSuomiPitchName [kE_SesquiSharp_QTP] = "eSesquiSharp???";
  gSuomiPitchName [kE_DoubleSharp_QTP] = "eisis";
    
  gSuomiPitchName [kF_DoubleFlat_QTP]  = "feses";
  gSuomiPitchName [kF_SesquiFlat_QTP]  = "fSesquiFlat???";
  gSuomiPitchName [kF_Flat_QTP]        = "fes";
  gSuomiPitchName [kF_SemiFlat_QTP]    = "fSemiFlat???";
  gSuomiPitchName [kF_Natural_QTP]     = "f";
  gSuomiPitchName [kF_SemiSharp_QTP]   = "fSemiSharp???";
  gSuomiPitchName [kF_Sharp_QTP]       = "fis";
  gSuomiPitchName [kF_SesquiSharp_QTP] = "fSesquiSharp???";
  gSuomiPitchName [kF_DoubleSharp_QTP] = "fisis";
    
  gSuomiPitchName [kG_DoubleFlat_QTP]  = "geses";
  gSuomiPitchName [kG_SesquiFlat_QTP]  = "gSesquiFlat???";
  gSuomiPitchName [kG_Flat_QTP]        = "ges";
  gSuomiPitchName [kG_SemiFlat_QTP]    = "gSemiFlat???";
  gSuomiPitchName [kG_Natural_QTP]     = "g";
  gSuomiPitchName [kG_SemiSharp_QTP]   = "gSemiSharp???";
  gSuomiPitchName [kG_Sharp_QTP]       = "gis";
  gSuomiPitchName [kG_SesquiSharp_QTP] = "gSesquiSharp???";
  gSuomiPitchName [kG_DoubleSharp_QTP] = "gisis";

  // svenska
  gSvenskaPitchName [k_NoQuarterTonesPitch]  = "";
  gSvenskaPitchName [k_Rest_QTP]                 = "r";
  
  gSvenskaPitchName [kA_DoubleFlat_QTP]  = "assess";
  gSvenskaPitchName [kA_SesquiFlat_QTP]  = "aSesquiFlat???";
  gSvenskaPitchName [kA_Flat_QTP]        = "ass";
  gSvenskaPitchName [kA_SemiFlat_QTP]    = "aSemiFlat???";
  gSvenskaPitchName [kA_Natural_QTP]     = "a";
  gSvenskaPitchName [kA_SemiSharp_QTP]   = "aSemiSharp???";
  gSvenskaPitchName [kA_Sharp_QTP]       = "aiss";
  gSvenskaPitchName [kA_SesquiSharp_QTP] = "aSesquiSharp???";
  gSvenskaPitchName [kA_DoubleSharp_QTP] = "aississ";
    
  gSvenskaPitchName [kB_DoubleFlat_QTP]  = "hessess";
  gSvenskaPitchName [kB_SesquiFlat_QTP]  = "bSesquiFlat???";
  gSvenskaPitchName [kB_Flat_QTP]        = "b";
  gSvenskaPitchName [kB_SemiFlat_QTP]    = "bSemiFlat???";
  gSvenskaPitchName [kB_Natural_QTP]     = "h";
  gSvenskaPitchName [kB_SemiSharp_QTP]   = "bSemiSharp???";
  gSvenskaPitchName [kB_Sharp_QTP]       = "hiss";
  gSvenskaPitchName [kB_SesquiSharp_QTP] = "bSesquiSharp???";
  gSvenskaPitchName [kB_DoubleSharp_QTP] = "hississ";
    
  gSvenskaPitchName [kC_DoubleFlat_QTP]  = "cessess";
  gSvenskaPitchName [kC_SesquiFlat_QTP]  = "cSesquiFlat???";
  gSvenskaPitchName [kC_Flat_QTP]        = "cess";
  gSvenskaPitchName [kC_SemiFlat_QTP]    = "cSemiFlat???";
  gSvenskaPitchName [kC_Natural_QTP]     = "c";
  gSvenskaPitchName [kC_SemiSharp_QTP]   = "cSemiSharp???";
  gSvenskaPitchName [kC_Sharp_QTP]       = "ciss";
  gSvenskaPitchName [kC_SesquiSharp_QTP] = "cSesquiSharp???";
  gSvenskaPitchName [kC_DoubleSharp_QTP] = "cississ";
    
  gSvenskaPitchName [kD_DoubleFlat_QTP]  = "dessess";
  gSvenskaPitchName [kD_SesquiFlat_QTP]  = "dSesquiFlat???";
  gSvenskaPitchName [kD_Flat_QTP]        = "dess";
  gSvenskaPitchName [kD_SemiFlat_QTP]    = "dSemiFlat???";
  gSvenskaPitchName [kD_Natural_QTP]     = "d";
  gSvenskaPitchName [kD_SemiSharp_QTP]   = "dSemiSharp???";
  gSvenskaPitchName [kD_Sharp_QTP]       = "diss";
  gSvenskaPitchName [kD_SesquiSharp_QTP] = "dSesquiSharp???";
  gSvenskaPitchName [kD_DoubleSharp_QTP] = "dississ";

  gSvenskaPitchName [kE_DoubleFlat_QTP]  = "essess";
  gSvenskaPitchName [kE_SesquiFlat_QTP]  = "eSesquiFlat???";
  gSvenskaPitchName [kE_Flat_QTP]        = "ess";
  gSvenskaPitchName [kE_SemiFlat_QTP]    = "eSemiFlat???";
  gSvenskaPitchName [kE_Natural_QTP]     = "e";
  gSvenskaPitchName [kE_SemiSharp_QTP]   = "eSemiSharp???";
  gSvenskaPitchName [kE_Sharp_QTP]       = "eiss";
  gSvenskaPitchName [kE_SesquiSharp_QTP] = "eSesquiSharp???";
  gSvenskaPitchName [kE_DoubleSharp_QTP] = "eississ";
    
  gSvenskaPitchName [kF_DoubleFlat_QTP]  = "fessess";
  gSvenskaPitchName [kF_SesquiFlat_QTP]  = "fSesquiFlat???";
  gSvenskaPitchName [kF_Flat_QTP]        = "fess";
  gSvenskaPitchName [kF_SemiFlat_QTP]    = "fSemiFlat???";
  gSvenskaPitchName [kF_Natural_QTP]     = "f";
  gSvenskaPitchName [kF_SemiSharp_QTP]   = "fSemiSharp???";
  gSvenskaPitchName [kF_Sharp_QTP]       = "fiss";
  gSvenskaPitchName [kF_SesquiSharp_QTP] = "fSesquiSharp???";
  gSvenskaPitchName [kF_DoubleSharp_QTP] = "fississ";
    
  gSvenskaPitchName [kG_DoubleFlat_QTP]  = "gessess";
  gSvenskaPitchName [kG_SesquiFlat_QTP]  = "gSesquiFlat???";
  gSvenskaPitchName [kG_Flat_QTP]        = "gess";
  gSvenskaPitchName [kG_SemiFlat_QTP]    = "gSemiFlat???";
  gSvenskaPitchName [kG_Natural_QTP]     = "g";
  gSvenskaPitchName [kG_SemiSharp_QTP]   = "gSemiSharp???";
  gSvenskaPitchName [kG_Sharp_QTP]       = "giss";
  gSvenskaPitchName [kG_SesquiSharp_QTP] = "gSesquiSharp???";
  gSvenskaPitchName [kG_DoubleSharp_QTP] = "gississ";

  // vlaams
  gVlaamsPitchName [k_NoQuarterTonesPitch]  = "none";
  gVlaamsPitchName [k_Rest_QTP]                 = "r";
  
  gVlaamsPitchName [kA_DoubleFlat_QTP]  = "labb";
  gVlaamsPitchName [kA_SesquiFlat_QTP]  = "laSesquiFlat???";
  gVlaamsPitchName [kA_Flat_QTP]        = "lab";
  gVlaamsPitchName [kA_SemiFlat_QTP]    = "laSemiFlat???";
  gVlaamsPitchName [kA_Natural_QTP]     = "la";
  gVlaamsPitchName [kA_SemiSharp_QTP]   = "laSemiSharp???";
  gVlaamsPitchName [kA_Sharp_QTP]       = "lak";
  gVlaamsPitchName [kA_SesquiSharp_QTP] = "laSesquiSharp???";
  gVlaamsPitchName [kA_DoubleSharp_QTP] = "lakk";
    
  gVlaamsPitchName [kB_DoubleFlat_QTP]  = "sibb";
  gVlaamsPitchName [kB_SesquiFlat_QTP]  = "siSesquiFlat???";
  gVlaamsPitchName [kB_Flat_QTP]        = "sib";
  gVlaamsPitchName [kB_SemiFlat_QTP]    = "siSemiFlat???";
  gVlaamsPitchName [kB_Natural_QTP]     = "si";
  gVlaamsPitchName [kB_SemiSharp_QTP]   = "siSemiSharp???";
  gVlaamsPitchName [kB_Sharp_QTP]       = "sik";
  gVlaamsPitchName [kB_SesquiSharp_QTP] = "siSesquiSharp???";
  gVlaamsPitchName [kB_DoubleSharp_QTP] = "sikk";
    
  gVlaamsPitchName [kC_DoubleFlat_QTP]  = "dobb";
  gVlaamsPitchName [kC_SesquiFlat_QTP]  = "doSesquiFlat???";
  gVlaamsPitchName [kC_Flat_QTP]        = "dob";
  gVlaamsPitchName [kC_SemiFlat_QTP]    = "doSemiFlat???";
  gVlaamsPitchName [kC_Natural_QTP]     = "do";
  gVlaamsPitchName [kC_SemiSharp_QTP]   = "doSemiSharp???";
  gVlaamsPitchName [kC_Sharp_QTP]       = "dok";
  gVlaamsPitchName [kC_SesquiSharp_QTP] = "doSesquiSharp???";
  gVlaamsPitchName [kC_DoubleSharp_QTP] = "dokk";
    
  gVlaamsPitchName [kD_DoubleFlat_QTP]  = "rebb";
  gVlaamsPitchName [kD_SesquiFlat_QTP]  = "reSesquiFlat???";
  gVlaamsPitchName [kD_Flat_QTP]        = "reb";
  gVlaamsPitchName [kD_SemiFlat_QTP]    = "reSemiFlat???";
  gVlaamsPitchName [kD_Natural_QTP]     = "re";
  gVlaamsPitchName [kD_SemiSharp_QTP]   = "reSemiSharp???";
  gVlaamsPitchName [kD_Sharp_QTP]       = "rek";
  gVlaamsPitchName [kD_SesquiSharp_QTP] = "reSesquiSharp???";
  gVlaamsPitchName [kD_DoubleSharp_QTP] = "rekk";

  gVlaamsPitchName [kE_DoubleFlat_QTP]  = "mibb";
  gVlaamsPitchName [kE_SesquiFlat_QTP]  = "miSesquiFlat???";
  gVlaamsPitchName [kE_Flat_QTP]        = "mib";
  gVlaamsPitchName [kE_SemiFlat_QTP]    = "miSemiFlat???";
  gVlaamsPitchName [kE_Natural_QTP]     = "mi";
  gVlaamsPitchName [kE_SemiSharp_QTP]   = "miSemiSharp???";
  gVlaamsPitchName [kE_Sharp_QTP]       = "mik";
  gVlaamsPitchName [kE_SesquiSharp_QTP] = "miSesquiSharp???";
  gVlaamsPitchName [kE_DoubleSharp_QTP] = "mikk";
    
  gVlaamsPitchName [kF_DoubleFlat_QTP]  = "fabb";
  gVlaamsPitchName [kF_SesquiFlat_QTP]  = "faSesquiFlat???";
  gVlaamsPitchName [kF_Flat_QTP]        = "fab";
  gVlaamsPitchName [kF_SemiFlat_QTP]    = "faSemiFlat???";
  gVlaamsPitchName [kF_Natural_QTP]     = "fa";
  gVlaamsPitchName [kF_SemiSharp_QTP]   = "faSemiSharp???";
  gVlaamsPitchName [kF_Sharp_QTP]       = "fak";
  gVlaamsPitchName [kF_SesquiSharp_QTP] = "faSesquiSharp???";
  gVlaamsPitchName [kF_DoubleSharp_QTP] = "fakk";
    
  gVlaamsPitchName [kG_DoubleFlat_QTP]  = "solbb";
  gVlaamsPitchName [kG_SesquiFlat_QTP]  = "solSesquiFlat???";
  gVlaamsPitchName [kG_Flat_QTP]        = "solb";
  gVlaamsPitchName [kG_SemiFlat_QTP]    = "solSemiFlat???";
  gVlaamsPitchName [kG_Natural_QTP]     = "sol";
  gVlaamsPitchName [kG_SemiSharp_QTP]   = "solSemiSharp???";
  gVlaamsPitchName [kG_Sharp_QTP]       = "solk";
  gVlaamsPitchName [kG_SesquiSharp_QTP] = "solSesquiSharp???";
  gVlaamsPitchName [kG_DoubleSharp_QTP] = "solkk";
}

string msrDiatonicPitchKindAsString (
  msrDiatonicPitchKind diatonicPitchKind)
{
  string result;

  switch (diatonicPitchKind) {
    case k_NoDiatonicPitch:
      result = "k_NoDiatonicPitch";
      break;

    case kA:
      result = "A";
      break;
    case kB:
      result = "B";
      break;
    case kC:
      result = "C";
      break;
    case kD:
      result = "D";
      break;
    case kE:
      result = "E";
      break;
    case kF:
      result = "F";
      break;
    case kG:
      result = "G";
      break;
  } // switch
  return result;
}

msrDiatonicPitchKind msrDiatonicPitchKindFromString (
  char diatonicNoteName)
{
  msrDiatonicPitchKind result = k_NoDiatonicPitch;
  
  switch (diatonicNoteName) {
    case 'a':
    case 'A':
      result = kA;
       break;
    case 'b':
    case 'B':
      result = kB;
      break;
    case 'c':
    case 'C':
      result = kC;
      break;
    case 'd':
    case 'D':
      result = kD;
      break;
    case 'e':
    case 'E':
      result = kE;
      break;
    case 'f':
    case 'F':
      result = kF;
      break;
    case 'g':
    case 'G':
      result = kG;
      break;
    default: {}
  } // switch

  return result;
}

string msrDiatonicPitchKindAsString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrDiatonicPitchKind               diatonicPitchKind)
{
  string result;
  
  switch (diatonicPitchKind) {
    case k_NoDiatonicPitch:
      result = "k_NoDiatonicPitch";
      break;

    case kA:
      result = "a";
      break;
    case kB:
      result = "b";
      break;
    case kC:
      result = "c";
      break;
    case kD:
      result = "d";
      break;
    case kE:
      result = "e";
      break;
    case kF:
      result = "f";
      break;
    case kG:
      result = "g";
      break;
  } // switch

  return result;
}

string msrQuarterTonesPitchesLanguageKindAsString (
  msrQuarterTonesPitchesLanguageKind languageKind)
{
  string result;
  
  switch (languageKind) {
    case kNederlands:
      result = "nederlands";
      break;
    case kCatalan:
      result = "catalan";
      break;
    case kDeutsch:
      result = "deutsch";
      break;
    case kEnglish:
      result = "english";
      break;
    case kEspanol:
      result = "espanol";
      break;
    case kFrancais:
      result = "francais";
      break;
    case kItaliano:
      result = "italiano";
      break;
    case kNorsk:
      result = "norsk";
      break;
    case kPortugues:
      result = "portugues";
      break;
    case kSuomi:
      result = "suomi";
      break;
    case kSvenska:
      result = "svenska";
      break;
    case kVlaams:
      result = "vlaams";
      break;
  } // switch

  return result;
}

msrAlterationKind msrAlterationKindFromMusicXMLAlter (
  float alter)
{

/*
  The alter element represents chromatic alteration
  in number of semitones (e.g., -1 for flat, 1 for sharp).
  Decimal values like 0.5 (quarter tone sharp) are used for microtones:

  semi-sharp semi-flat sesqui-sharp sesqui-flat double-sharp double-flat
      +0.5      -0.5        +1.5       -1.5         +2.0        -2.0
*/

  msrAlterationKind result;
  
  if      (alter == 0 ) {
    result = kNatural;
  }
  
  else if (alter == -1 ) {
    result = kFlat;
  }
  
  else if (alter == 1 ) {
    result = kSharp;
  }
  
  else if (alter == -0.5 ) {
    result = kSemiFlat;
  }
  
  else if (alter == +0.5 ) {
    result = kSemiSharp;
  }
  
  else if (alter == -1.5 ) {
    result = kSesquiFlat;
  }
  
  else if (alter == +1.5 ) {
    result = kSesquiSharp;
  }
  
  else if (alter == -2 ) {
    result = kDoubleFlat;
  }
  
  else if (alter == +2 ) {
    result = kDoubleSharp;
  }
  
  else {
    result = k_NoAlteration;
  }

  return result;
}

// alterations
//______________________________________________________________________________
string msrAlterationKindAsString (
  msrAlterationKind alterationKind)
{
  string result;
  
  switch (alterationKind) {
    case k_NoAlteration:
      result = "alteration???";
      break;

    case kDoubleFlat:
      result = "DoubleFlat";
      break;
    case kSesquiFlat:
      result = "SesquiFlat";
      break;
    case kFlat:
      result = "Flat";
      break;
    case kSemiFlat:
      result = "SemiFlat";
      break;
    case kNatural:
      result = "Natural";
      break;
    case kSemiSharp:
      result = "SemiSharp";
      break;
    case kSharp:
      result = "Sharp";
      break;
    case kSesquiSharp:
      result = "SesquiSharp";
      break;
    case kDoubleSharp:
      result = "DoubleSharp";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
void setDiatonicPitchAndAlterationKind (
  msrQuarterTonesPitchKind quarterTonesPitchKind,
  msrDiatonicPitchKind&    diatonicPitchKind,
  msrAlterationKind&       alterationKind)
{
  switch (quarterTonesPitchKind) {
    case k_NoQuarterTonesPitch:
      diatonicPitchKind = kA; // any value would fit
      alterationKind    = k_NoAlteration;
      break;

    case kA_DoubleFlat_QTP:
      diatonicPitchKind = kA;
      alterationKind    = kDoubleFlat;
      break;
    case kA_SesquiFlat_QTP:
      diatonicPitchKind = kA;
      alterationKind    = kSesquiFlat;
      break;
    case kA_Flat_QTP:
      diatonicPitchKind = kA;
      alterationKind    = kFlat;
      break;
    case kA_SemiFlat_QTP:
      diatonicPitchKind = kA;
      alterationKind    = kSemiFlat;
      break;
    case kA_Natural_QTP:
      diatonicPitchKind = kA;
      alterationKind    = kNatural;
      break;
    case kA_SemiSharp_QTP:
      diatonicPitchKind = kA;
      alterationKind    = kSemiSharp;
      break;
    case kA_Sharp_QTP:
      diatonicPitchKind = kA;
      alterationKind    = kSharp;
      break;
    case kA_SesquiSharp_QTP:
      diatonicPitchKind = kA;
      alterationKind    = kSesquiSharp;
      break;
    case kA_DoubleSharp_QTP:
      diatonicPitchKind = kA;
      alterationKind    = kDoubleSharp;
      break;

    case kB_DoubleFlat_QTP:
      diatonicPitchKind = kB;
      alterationKind    = kDoubleFlat;
      break;
    case kB_SesquiFlat_QTP:
      diatonicPitchKind = kB;
      alterationKind    = kSesquiFlat;
      break;
    case kB_Flat_QTP:
      diatonicPitchKind = kB;
      alterationKind    = kFlat;
      break;
    case kB_SemiFlat_QTP:
      diatonicPitchKind = kB;
      alterationKind    = kSemiFlat;
      break;
    case kB_Natural_QTP:
      diatonicPitchKind = kB;
      alterationKind    = kNatural;
      break;
    case kB_SemiSharp_QTP:
      diatonicPitchKind = kB;
      alterationKind    = kSemiSharp;
      break;
    case kB_Sharp_QTP:
      diatonicPitchKind = kB;
      alterationKind    = kSharp;
      break;
    case kB_SesquiSharp_QTP:
      diatonicPitchKind = kB;
      alterationKind    = kSesquiSharp;
      break;
    case kB_DoubleSharp_QTP:
      diatonicPitchKind = kB;
      alterationKind    = kDoubleSharp;
      break;

    case kC_DoubleFlat_QTP:
      diatonicPitchKind = kC;
      alterationKind    = kDoubleFlat;
      break;
    case kC_SesquiFlat_QTP:
      diatonicPitchKind = kC;
      alterationKind    = kSesquiFlat;
      break;
    case kC_Flat_QTP:
      diatonicPitchKind = kC;
      alterationKind    = kFlat;
      break;
    case kC_SemiFlat_QTP:
      diatonicPitchKind = kC;
      alterationKind    = kSemiFlat;
      break;
    case kC_Natural_QTP:
      diatonicPitchKind = kC;
      alterationKind    = kNatural;
      break;
    case kC_SemiSharp_QTP:
      diatonicPitchKind = kC;
      alterationKind    = kSemiSharp;
      break;
    case kC_Sharp_QTP:
      diatonicPitchKind = kC;
      alterationKind    = kSharp;
      break;
    case kC_SesquiSharp_QTP:
      diatonicPitchKind = kC;
      alterationKind    = kSesquiSharp;
      break;
    case kC_DoubleSharp_QTP:
      diatonicPitchKind = kC;
      alterationKind    = kDoubleSharp;
      break;

    case kD_DoubleFlat_QTP:
      diatonicPitchKind = kD;
      alterationKind    = kDoubleFlat;
      break;
    case kD_SesquiFlat_QTP:
      diatonicPitchKind = kD;
      alterationKind    = kSesquiFlat;
      break;
    case kD_Flat_QTP:
      diatonicPitchKind = kD;
      alterationKind    = kFlat;
      break;
    case kD_SemiFlat_QTP:
      diatonicPitchKind = kD;
      alterationKind    = kSemiFlat;
      break;
    case kD_Natural_QTP:
      diatonicPitchKind = kD;
      alterationKind    = kNatural;
      break;
    case kD_SemiSharp_QTP:
      diatonicPitchKind = kD;
      alterationKind    = kSemiSharp;
      break;
    case kD_Sharp_QTP:
      diatonicPitchKind = kD;
      alterationKind    = kSharp;
      break;
    case kD_SesquiSharp_QTP:
      diatonicPitchKind = kD;
      alterationKind    = kSesquiSharp;
      break;
    case kD_DoubleSharp_QTP:
      diatonicPitchKind = kD;
      alterationKind    = kDoubleSharp;
      break;

    case kE_DoubleFlat_QTP:
      diatonicPitchKind = kE;
      alterationKind    = kDoubleFlat;
      break;
    case kE_SesquiFlat_QTP:
      diatonicPitchKind = kE;
      alterationKind    = kSesquiFlat;
      break;
    case kE_Flat_QTP:
      diatonicPitchKind = kE;
      alterationKind    = kFlat;
      break;
    case kE_SemiFlat_QTP:
      diatonicPitchKind = kE;
      alterationKind    = kSemiFlat;
      break;
    case kE_Natural_QTP:
      diatonicPitchKind = kE;
      alterationKind    = kNatural;
      break;
    case kE_SemiSharp_QTP:
      diatonicPitchKind = kE;
      alterationKind    = kSemiSharp;
      break;
    case kE_Sharp_QTP:
      diatonicPitchKind = kE;
      alterationKind    = kSharp;
      break;
    case kE_SesquiSharp_QTP:
      diatonicPitchKind = kE;
      alterationKind    = kSesquiSharp;
      break;
    case kE_DoubleSharp_QTP:
      diatonicPitchKind = kE;
      alterationKind    = kDoubleSharp;
      break;

    case kF_DoubleFlat_QTP:
      diatonicPitchKind = kF;
      alterationKind    = kDoubleFlat;
      break;
    case kF_SesquiFlat_QTP:
      diatonicPitchKind = kF;
      alterationKind    = kSesquiFlat;
      break;
    case kF_Flat_QTP:
      diatonicPitchKind = kF;
      alterationKind    = kFlat;
      break;
    case kF_SemiFlat_QTP:
      diatonicPitchKind = kF;
      alterationKind    = kSemiFlat;
      break;
    case kF_Natural_QTP:
      diatonicPitchKind = kF;
      alterationKind    = kNatural;
      break;
    case kF_SemiSharp_QTP:
      diatonicPitchKind = kF;
      alterationKind    = kSemiSharp;
      break;
    case kF_Sharp_QTP:
      diatonicPitchKind = kF;
      alterationKind    = kSharp;
      break;
    case kF_SesquiSharp_QTP:
      diatonicPitchKind = kF;
      alterationKind    = kSesquiSharp;
      break;
    case kF_DoubleSharp_QTP:
      diatonicPitchKind = kF;
      alterationKind    = kDoubleSharp;
      break;

    case kG_DoubleFlat_QTP:
      diatonicPitchKind = kG;
      alterationKind    = kDoubleFlat;
      break;
    case kG_SesquiFlat_QTP:
      diatonicPitchKind = kG;
      alterationKind    = kSesquiFlat;
      break;
    case kG_Flat_QTP:
      diatonicPitchKind = kG;
      break;
    case kG_SemiFlat_QTP:
      diatonicPitchKind = kG;
      alterationKind    = kSemiFlat;
      break;
    case kG_Natural_QTP:
      diatonicPitchKind = kG;
      alterationKind    = kNatural;
      break;
    case kG_SemiSharp_QTP:
      diatonicPitchKind = kG;
      alterationKind    = kSemiSharp;
      break;
    case kG_Sharp_QTP:
      diatonicPitchKind = kG;
      alterationKind    = kSharp;
      break;
    case kG_SesquiSharp_QTP:
      diatonicPitchKind = kG;
      alterationKind    = kSesquiSharp;
      break;
    case kG_DoubleSharp_QTP:
      diatonicPitchKind = kG;
      alterationKind    = kDoubleSharp;
      break;
      
    case k_Rest_QTP:
      diatonicPitchKind = kA; // any value would fit JMI
      alterationKind    = k_NoAlteration;
      
      break;
  } // switch
}

msrQuarterTonesPitchKind quarterTonesPitchKindFromDiatonicPitchAndAlteration (
  int                  inputLineNumber,
  msrDiatonicPitchKind diatonicPitchKind,
  msrAlterationKind    alterationKind)
{
  msrQuarterTonesPitchKind
    result =
      k_NoQuarterTonesPitch;
  
  switch (diatonicPitchKind) {
    case kA:
      switch (alterationKind) {
        case kDoubleFlat:
          result = kA_DoubleFlat_QTP;
          break;
        case kSesquiFlat:
          result = kA_SesquiFlat_QTP;
          break;
        case kFlat:
          result = kA_Flat_QTP;
          break;
        case kSemiFlat:
          result = kA_SemiFlat_QTP;
          break;
        case kNatural:
          result = kA_Natural_QTP;
          break;
        case kSemiSharp:
          result = kA_SemiSharp_QTP;
          break;
        case kSharp:
          result = kA_Sharp_QTP;
          break;
        case kSesquiSharp:
          result = kA_SesquiSharp_QTP;
          break;
        case kDoubleSharp:
          result = kA_DoubleSharp_QTP;
          break;
        case k_NoAlteration:
          {
            stringstream s;

            s <<
              "'A' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              gXml2lyOptions->fInputSourceName,
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
      } // switch
      break;
      
    case kB:
      switch (alterationKind) {
        case kDoubleFlat:
          result = kB_DoubleFlat_QTP;
          break;
        case kSesquiFlat:
          result = kB_SesquiFlat_QTP;
          break;
        case kFlat:
          result = kB_Flat_QTP;
          break;
        case kSemiFlat:
          result = kB_SemiFlat_QTP;
          break;
        case kNatural:
          result = kB_Natural_QTP;
          break;
        case kSemiSharp:
          result = kB_SemiSharp_QTP;
          break;
        case kSharp:
          result = kB_Sharp_QTP;
          break;
        case kSesquiSharp:
          result = kB_SesquiSharp_QTP;
          break;
        case kDoubleSharp:
          result = kB_DoubleSharp_QTP;
          break;
        case k_NoAlteration:
          {
            stringstream s;

            s <<
              "'B' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              gXml2lyOptions->fInputSourceName,
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
      } // switch
      break;
      
      break;
      
    case kC:
      switch (alterationKind) {
        case kDoubleFlat:
          result = kC_DoubleFlat_QTP;
          break;
        case kSesquiFlat:
          result = kC_SesquiFlat_QTP;
          break;
        case kFlat:
          result = kC_Flat_QTP;
          break;
        case kSemiFlat:
          result = kC_SemiFlat_QTP;
          break;
        case kNatural:
          result = kC_Natural_QTP;
          break;
        case kSemiSharp:
          result = kC_SemiSharp_QTP;
          break;
        case kSharp:
          result = kC_Sharp_QTP;
          break;
        case kSesquiSharp:
          result = kC_SesquiSharp_QTP;
          break;
        case kDoubleSharp:
          result = kC_DoubleSharp_QTP;
          break;
        case k_NoAlteration:
          {
            stringstream s;

            s <<
              "'C' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              gXml2lyOptions->fInputSourceName,
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
      } // switch
      break;
      
    case kD:
      switch (alterationKind) {
        case kDoubleFlat:
          result = kD_DoubleFlat_QTP;
          break;
        case kSesquiFlat:
          result = kD_SesquiFlat_QTP;
          break;
        case kFlat:
          result = kD_Flat_QTP;
          break;
        case kSemiFlat:
          result = kD_SemiFlat_QTP;
          break;
        case kNatural:
          result = kD_Natural_QTP;
          break;
        case kSemiSharp:
          result = kD_SemiSharp_QTP;
          break;
        case kSharp:
          result = kD_Sharp_QTP;
          break;
        case kSesquiSharp:
          result = kD_SesquiSharp_QTP;
          break;
        case kDoubleSharp:
          result = kD_DoubleSharp_QTP;
          break;
        case k_NoAlteration:
          {
            stringstream s;

            s <<
              "'D' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              gXml2lyOptions->fInputSourceName,
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
      } // switch
      break;
      
    case kE:
      switch (alterationKind) {
        case kDoubleFlat:
          result = kE_DoubleFlat_QTP;
          break;
        case kSesquiFlat:
          result = kE_SesquiFlat_QTP;
          break;
        case kFlat:
          result = kE_Flat_QTP;
          break;
        case kSemiFlat:
          result = kE_SemiFlat_QTP;
          break;
        case kNatural:
          result = kE_Natural_QTP;
          break;
        case kSemiSharp:
          result = kE_SemiSharp_QTP;
          break;
        case kSharp:
          result = kE_Sharp_QTP;
          break;
        case kSesquiSharp:
          result = kE_SesquiSharp_QTP;
          break;
        case kDoubleSharp:
          result = kE_DoubleSharp_QTP;
          break;
        case k_NoAlteration:
          {
            stringstream s;

            s <<
              "'E' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              gXml2lyOptions->fInputSourceName,
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
      } // switch
      break;
      
    case kF:
      switch (alterationKind) {
        case kDoubleFlat:
          result = kF_DoubleFlat_QTP;
          break;
        case kSesquiFlat:
          result = kF_SesquiFlat_QTP;
          break;
        case kFlat:
          result = kF_Flat_QTP;
          break;
        case kSemiFlat:
          result = kF_SemiFlat_QTP;
          break;
        case kNatural:
          result = kF_Natural_QTP;
          break;
        case kSemiSharp:
          result = kF_SemiSharp_QTP;
          break;
        case kSharp:
          result = kF_Sharp_QTP;
          break;
        case kSesquiSharp:
          result = kF_SesquiSharp_QTP;
          break;
        case kDoubleSharp:
          result = kF_DoubleSharp_QTP;
          break;
        case k_NoAlteration:
          {
            stringstream s;

            s <<
              "'F' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              gXml2lyOptions->fInputSourceName,
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
      } // switch
      break;
      
    case kG:
      switch (alterationKind) {
        case kDoubleFlat:
          result = kG_DoubleFlat_QTP;
          break;
        case kSesquiFlat:
          result = kG_SesquiFlat_QTP;
          break;
        case kFlat:
          result = kG_Flat_QTP;
          break;
        case kSemiFlat:
          result = kG_SemiFlat_QTP;
          break;
        case kNatural:
          result = kG_Natural_QTP;
          break;
        case kSemiSharp:
          result = kG_SemiSharp_QTP;
          break;
        case kSharp:
          result = kG_Sharp_QTP;
          break;
        case kSesquiSharp:
          result = kG_SesquiSharp_QTP;
          break;
        case kDoubleSharp:
          result = kG_DoubleSharp_QTP;
          break;
        case k_NoAlteration:
          {
            stringstream s;

            s <<
              "'G' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              gXml2lyOptions->fInputSourceName,
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
      } // switch
      break;

    case k_NoDiatonicPitch:
      {
        result = k_NoQuarterTonesPitch;
        /* JMI
        stringstream s;

        s <<
          "cannot convert k_NoDiatonicPitch to a quarter tones pitch"
          ", line = " << inputLineNumber;

        msrInternalError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
          */
      }
      break;    
  } // switch

  return result;
}

msrDiatonicPitchKind diatonicPitchKindFromQuarterTonesPitchKind (
  int                      inputLineNumber,
  msrQuarterTonesPitchKind quarterTonesPitchKind)
{
  msrDiatonicPitchKind result;
  
  switch (quarterTonesPitchKind) {
    case kA_DoubleFlat_QTP:
    case kA_SesquiFlat_QTP:
    case kA_Flat_QTP:
    case kA_SemiFlat_QTP:
    case kA_Natural_QTP:
    case kA_SemiSharp_QTP:
    case kA_Sharp_QTP:
    case kA_SesquiSharp_QTP:
    case kA_DoubleSharp_QTP:
      result = kA;
      break;

    case kB_DoubleFlat_QTP:
    case kB_SesquiFlat_QTP:
    case kB_Flat_QTP:
    case kB_SemiFlat_QTP:
    case kB_Natural_QTP:
    case kB_SemiSharp_QTP:
    case kB_Sharp_QTP:
    case kB_SesquiSharp_QTP:
    case kB_DoubleSharp_QTP:
      result = kB;
      break;

    case kC_DoubleFlat_QTP:
    case kC_SesquiFlat_QTP:
    case kC_Flat_QTP:
    case kC_SemiFlat_QTP:
    case kC_Natural_QTP:
    case kC_SemiSharp_QTP:
    case kC_Sharp_QTP:
    case kC_SesquiSharp_QTP:
    case kC_DoubleSharp_QTP:
      result = kC;
      break;

    case kD_DoubleFlat_QTP:
    case kD_SesquiFlat_QTP:
    case kD_Flat_QTP:
    case kD_SemiFlat_QTP:
    case kD_Natural_QTP:
    case kD_SemiSharp_QTP:
    case kD_Sharp_QTP:
    case kD_SesquiSharp_QTP:
    case kD_DoubleSharp_QTP:
      result = kD;
      break;

    case kE_DoubleFlat_QTP:
    case kE_SesquiFlat_QTP:
    case kE_Flat_QTP:
    case kE_SemiFlat_QTP:
    case kE_Natural_QTP:
    case kE_SemiSharp_QTP:
    case kE_Sharp_QTP:
    case kE_SesquiSharp_QTP:
    case kE_DoubleSharp_QTP:
      result = kE;
      break;

    case kF_DoubleFlat_QTP:
    case kF_SesquiFlat_QTP:
    case kF_Flat_QTP:
    case kF_SemiFlat_QTP:
    case kF_Natural_QTP:
    case kF_SemiSharp_QTP:
    case kF_Sharp_QTP:
    case kF_SesquiSharp_QTP:
    case kF_DoubleSharp_QTP:
      result = kF;
      break;

    case kG_DoubleFlat_QTP:
    case kG_SesquiFlat_QTP:
    case kG_Flat_QTP:
    case kG_SemiFlat_QTP:
    case kG_Natural_QTP:
    case kG_SemiSharp_QTP:
    case kG_Sharp_QTP:
    case kG_SesquiSharp_QTP:
    case kG_DoubleSharp_QTP:
      result = kG;
      break;

    case k_Rest_QTP:
      {
        stringstream s;

        s <<
          "cannot get the diatonic pitch of a rest"
          ", line = " << inputLineNumber;

        msrInternalError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      
    case k_NoQuarterTonesPitch:
      {
        result = k_NoDiatonicPitch;
        
        /* JMI
        stringstream s;

        s <<
          "cannot get the diatonic pitch of a k_NoQuarterTonesPitch"
          ", line = " << inputLineNumber;

        msrInternalError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
          */
      }
  } // switch

  return result;
}

string msrQuarterTonesPitchKindAsString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrQuarterTonesPitchKind           quarterTonesPitchKind)
{
  string result;
  
  switch (languageKind) {
    case kNederlands:
      result = gNederlandsPitchName [quarterTonesPitchKind];
      break;
    case kCatalan:
      result = gCatalanPitchName [quarterTonesPitchKind];
      break;
    case kDeutsch:
      result = gDeutschPitchName [quarterTonesPitchKind];
      break;
    case kEnglish:
      result = gEnglishPitchName [quarterTonesPitchKind];
      break;
    case kEspanol:
      result = gEspanolPitchName [quarterTonesPitchKind];
      break;
    case kFrancais:
      result = gFrancaisPitchName [quarterTonesPitchKind];
      break;
    case kItaliano:
      result = gItalianoPitchName [quarterTonesPitchKind];
      break;
    case kNorsk:
      result = gNorskPitchName [quarterTonesPitchKind];
      break;
    case kPortugues:
      result = gPortuguesPitchName [quarterTonesPitchKind];
      break;
    case kSuomi:
      result = gSuomiPitchName [quarterTonesPitchKind];
      break;
    case kSvenska:
      result = gSvenskaPitchName [quarterTonesPitchKind];
      break;
    case kVlaams:
      result = gVlaamsPitchName [quarterTonesPitchKind];
      break;
  } // switch

  return result;
}

string existingQuarterTonesPitchesLanguageKinds ()
{
  stringstream s;

  if (gQuarterTonesPitchesLanguageKindsMap.size ()) {
    map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator
      iBegin = gQuarterTonesPitchesLanguageKindsMap.begin (),
      iEnd   = gQuarterTonesPitchesLanguageKindsMap.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first;
      if (++i == iEnd) break;
      s << " ";
    } // for
  }
  
  return s.str ();
}

// semi tones pitches
//______________________________________________________________________________

string msrSemiTonesPitchKindAsString (
  msrSemiTonesPitchKind semiTonesPitchKind)
{
  string result;

  /* JMI
  switch (semiTonesPitchKind) {
    case k_NoWelTemperedPitch_STP:
      result = "NoWelTemperedPitch???";
      break;

    case kC_Natural_STP: // kB_Sharp_STP, kD_DoubleFlat_STP
      result = "C_Natural_B_Sharp_D_DoubleFlat";
      break;
      
    case kC_Sharp_STP: // kB_DoubleSharp_STP, kD_Flat_STP
      result = "C_Sharp_B_DoubleSharp_D_Flat";
      break;

    case kD_Natural_STP: // kC_DoubleSharp_STP, kE_DoubleFlat_STP
      result = "D_Natural_C_DoubleSharp_E_DoubleFlat";
      break;

    case kD_Sharp_STP: // kE_Flat_STP
      result = "D_Sharp_E_Flat";
      break;
      
    case kE_Natural_STP: // kD_DoubleSharp_STP, kF_Flat_STP
      result = "E_Natural_D_DoubleSharp_F_Flat";
      break;
      
    case kF_Natural_STP: // kE_Sharp_STP, kG_DoubleFlat_STP
      result = "F_Natural_E_Sharp_G_DoubleFlat";
      break;
      
    case kF_Sharp_STP: // kE_DoubleSharp_STP, kG_Flat_STP
      result = "F_Sharp_E_DoubleSharp_G_Flat";
      break;
      
    case kG_Natural_STP: // kF_DoubleSharp_STP, kA_DoubleFlat_STP
      result = "G_Natural_F_DoubleSharp_A_DoubleFlat";
      break;
      
    case kG_Sharp_STP: // kA_Flat_STP
      result = "G_Sharp_A_Flat";
      break;
      
    case kA_Natural_STP: // kG_DoubleSharp_STP, kB_DoubleFlat_STP
      result = "A_Natural_G_DoubleSharp_B_DoubleFlat";
      break;
      
    case kA_Sharp_STP: // kB_Flat_STP
      result = "A_Sharp_B_Flat";
      break;

    case kB_Natural_STP: // kA_DoubleSharp_STP, kC_Flat_STP
      result = "B_Natural_A_DoubleSharp_C_Flat";
      break;
  } // switch
  */
  
  switch (semiTonesPitchKind) {
    case k_NoWelTemperedPitch_STP:
      result = "NoWelTemperedPitch???";
      break;

    case kB_Sharp_STP:
      result = "B_Sharp_STP";
      break;
    case kC_Natural_STP:
      result = "C_Natural_STP";
      break;
    case kD_DoubleFlat_STP:
      result = "D_DoubleFlat_STP";
      break;
      
    case kC_Sharp_STP:
      result = "C_Sharp_STP";
      break;
    case kB_DoubleSharp_STP:
      result = "B_DoubleSharp_STP";
      break;
    case kD_Flat_STP:
      result = "D_Flat_STP";
      break;

    case kD_Natural_STP:
      result = "D_Natural_STP";
      break;
    case kC_DoubleSharp_STP:
      result = "C_DoubleSharp_STP";
      break;
    case kE_DoubleFlat_STP:
      result = "E_DoubleFlat_STP";
      break;

    case kD_Sharp_STP:
      result = "D_Sharp_STP";
      break;
    case kE_Flat_STP:
      result = "E_Flat_STP";
      break;
      
    case kE_Natural_STP: // , 
      result = "E_Natural_STP";
      break;
    case kD_DoubleSharp_STP:
      result = "D_DoubleSharp_STP";
      break;
    case kF_Flat_STP:
      result = "F_Flat_STP";
      break;
      
    case kF_Natural_STP:
      result = "F_Natural_STP";
      break;
    case kE_Sharp_STP:
      result = "E_Sharp_STP";
      break;
    case kG_DoubleFlat_STP:
      result = "G_DoubleFlat_STP";
      break;
      
    case kF_Sharp_STP:
      result = "F_Sharp_STP";
      break;
    case kE_DoubleSharp_STP:
      result = "E_DoubleSharp_STP";
      break;
    case kG_Flat_STP:
      result = "G_Flat_STP";
      break;
      
    case kG_Natural_STP:
      result = "G_Natural_STP";
      break;
    case kF_DoubleSharp_STP:
      result = "F_DoubleSharp_STP";
      break;
    case kA_DoubleFlat_STP:
      result = "A_DoubleFlat_STP";
      break;
      
    case kG_Sharp_STP:
      result = "G_Sharp_STP";
      break;
    case kA_Flat_STP:
      result = "A_Flat_STP";
      break;
      
    case kA_Natural_STP:
      result = "A_Natural_STP";
      break;
    case kG_DoubleSharp_STP:
      result = "G_DoubleSharp_STP";
      break;
    case kB_DoubleFlat_STP:
      result = "B_DoubleFlat_STP";
      break;
      
    case kA_Sharp_STP:
      result = "A_Sharp_STP";
      break;
    case kB_Flat_STP:
      result = "B_Flat_STP";
      break;

    case kB_Natural_STP:
      result = "B_Natural_STP";
      break;
    case kA_DoubleSharp_STP:
      result = "A_DoubleSharp_STP";
      break;
    case kC_Flat_STP:
      result = "C_Flat_STP";
      break;
  } // switch

  return result;
}

msrQuarterTonesPitchKind msrSemiTonesPitchKindAsQuarterTonesPitchKind (
  msrSemiTonesPitchKind       semiTonesPitchKind,
  msrAlterationPreferenceKind alterationPreferenceKind)
{
  msrQuarterTonesPitchKind result;

  /* JMI
  switch (semiTonesPitchKind) {
    case k_NoWelTemperedPitch_STP:
      result = k_NoQuarterTonesPitch;
      break;

    case kC_Natural_STP: // kB_Sharp_STP, kD_DoubleFlat_STP
      result = kC_Natural;
      break;
      
    case kC_Sharp_STP: // kB_DoubleSharp_STP, kD_Flat_STP
      switch (alterationPreferenceKind) {
        case kPreferSharp:
          result = kC_Sharp;
          break;
        case kPreferFlat:
          result = kD_Flat;
          break;
      } // switch
      break;

    case kD_Natural_STP: // kC_DoubleSharp_STP, kE_DoubleFlat_STP
      result = kD_Natural;
      break;

    case kD_Sharp_STP: // kE_Flat_STP
      switch (alterationPreferenceKind) {
        case kPreferSharp:
          result = kD_Sharp;
          break;
        case kPreferFlat:
          result = kE_Flat;
          break;
      } // switch
      break;
      
    case kE_Natural_STP: // kD_DoubleSharp_STP, kF_Flat_STP
      result = kE_Natural;
      break;
      
    case kF_Natural_STP: // kE_Sharp_STP, kG_DoubleFlat_STP
      result = kF_Natural;
      break;
      
    case kF_Sharp_STP: // kE_DoubleSharp_STP, kG_Flat_STP
      switch (alterationPreferenceKind) {
        case kPreferSharp:
          result = kF_Sharp;
          break;
        case kPreferFlat:
          result = kG_Flat;
          break;
      } // switch
      break;
      
    case kG_Natural_STP: // kF_DoubleSharp_STP, kA_DoubleFlat_STP
      result = kG_Natural;
      break;
      
    case kG_Sharp_STP: // kA_Flat_STP
      switch (alterationPreferenceKind) {
        case kPreferSharp:
          result = kG_Sharp;
          break;
        case kPreferFlat:
          result = kA_Flat;
          break;
      } // switch
      break;
      
    case kA_Natural_STP: // kG_DoubleSharp_STP, kB_DoubleFlat_STP
      result = kA_Natural;
      break;
      
    case kA_Sharp_STP: // kB_Flat_STP
      switch (alterationPreferenceKind) {
        case kPreferSharp:
          result = kA_Sharp;
          break;
        case kPreferFlat:
          result = kB_Flat;
          break;
      } // switch
      break;

    case kB_Natural_STP: // kA_DoubleSharp_STP, kC_Flat_STP
      result = kB_Natural;
      break;
  } // switch
  */
  
  switch (semiTonesPitchKind) {
    case k_NoWelTemperedPitch_STP:
      result = k_NoQuarterTonesPitch;
      break;

    case kC_Natural_STP: // kB_Sharp_STP, kD_DoubleFlat_STP
      result = kC_Natural_QTP;
      break;
      
    case kC_Sharp_STP: // kB_DoubleSharp_STP, kD_Flat_STP
      switch (alterationPreferenceKind) {
        case kPreferSharp:
          result = kC_Sharp_QTP;
          break;
        case kPreferFlat:
          result = kD_Flat_QTP;
          break;
      } // switch
      break;

    case kD_Natural_STP: // kC_DoubleSharp_STP, kE_DoubleFlat_STP
      result = kD_Natural_QTP;
      break;

    case kD_Sharp_STP: // kE_Flat_STP
      switch (alterationPreferenceKind) {
        case kPreferSharp:
          result = kD_Sharp_QTP;
          break;
        case kPreferFlat:
          result = kE_Flat_QTP;
          break;
      } // switch
      break;
      
    case kE_Natural_STP: // kD_DoubleSharp_STP, kF_Flat_STP
      result = kE_Natural_QTP;
      break;
      
    case kF_Natural_STP: // kE_Sharp_STP, kG_DoubleFlat_STP
      result = kF_Natural_QTP;
      break;
      
    case kF_Sharp_STP: // kE_DoubleSharp_STP, kG_Flat_STP
      switch (alterationPreferenceKind) {
        case kPreferSharp:
          result = kF_Sharp_QTP;
          break;
        case kPreferFlat:
          result = kG_Flat_QTP;
          break;
      } // switch
      break;
      
    case kG_Natural_STP: // kF_DoubleSharp_STP, kA_DoubleFlat_STP
      result = kG_Natural_QTP;
      break;
      
    case kG_Sharp_STP: // kA_Flat_STP
      switch (alterationPreferenceKind) {
        case kPreferSharp:
          result = kG_Sharp_QTP;
          break;
        case kPreferFlat:
          result = kA_Flat_QTP;
          break;
      } // switch
      break;
      
    case kA_Natural_STP: // kG_DoubleSharp_STP, kB_DoubleFlat_STP
      result = kA_Natural_QTP;
      break;
      
    case kA_Sharp_STP: // kB_Flat_STP
      switch (alterationPreferenceKind) {
        case kPreferSharp:
          result = kA_Sharp_QTP;
          break;
        case kPreferFlat:
          result = kB_Flat_QTP;
          break;
      } // switch
      break;

    case kB_Natural_STP: // kA_DoubleSharp_STP, kC_Flat_STP
      result = kB_Natural_QTP;
      break;
  } // switch

  return result;
}

// enharmonies
//______________________________________________________________________________
msrSemiTonesPitchKind enharmonicSemiTonesPitch (
  msrSemiTonesPitchKind       semiTonesPitchKind,
  msrAlterationPreferenceKind alterationPreferenceKind)
{
  msrSemiTonesPitchKind result = semiTonesPitchKind;
  
  switch (semiTonesPitchKind) {
   case kC_Flat_STP: // kA_DoubleSharp_STP, kC_Flat_STP
      result = kB_Natural_STP;
      break;

   case kC_Natural_STP: // kB_Sharp_STP, kD_DoubleFlat_STP
      break;
      
    case kC_Sharp_STP: // kB_DoubleSharp_STP, kD_Flat_STP
      result = kD_Flat_STP;
      break;
    case kD_Flat_STP: // kB_DoubleSharp_STP, kD_Flat_STP
      result = kC_Sharp_STP;
      break;

    case kD_Natural_STP: // kC_DoubleSharp_STP, kE_DoubleFlat_STP
      break;

    case kD_Sharp_STP: // kE_Flat_STP
      result = kE_Flat_STP;
      break;
    case kE_Flat_STP: // kE_Flat_STP
      result = kD_Sharp_STP;
      break;
      
    case kE_Natural_STP: // kD_DoubleSharp_STP, kF_Flat_STP
      break;
      
    case kF_Natural_STP: // kE_Sharp_STP, kG_DoubleFlat_STP
      break;
      
    case kF_Sharp_STP: // kE_DoubleSharp_STP, kG_Flat_STP
      result = kG_Flat_STP;
      break;
    case kG_Flat_STP: // kE_DoubleSharp_STP, kG_Flat_STP
      result = kF_Sharp_STP;
      break;
      
    case kG_Natural_STP: // kF_DoubleSharp_STP, kA_DoubleFlat_STP
      break;
      
    case kG_Sharp_STP: // kA_Flat_STP
      result = kA_Flat_STP;
      break;
    case kA_Flat_STP: // kA_Flat_STP
      result = kG_Sharp_STP;
      break;
      
    case kA_Natural_STP: // kG_DoubleSharp_STP, kB_DoubleFlat_STP
      break;
      
    case kA_Sharp_STP: // kB_Flat_STP
      result = kB_Flat_STP;
      break;
    case kB_Flat_STP: // kB_Flat_STP
      result = kA_Sharp_STP;
      break;

    case kB_Natural_STP: // kA_DoubleSharp_STP, kC_Flat_STP
      break;

    case kB_Sharp_STP: // kB_Sharp_STP, kD_DoubleFlat_STP
      result = kC_Natural_STP;
      break;

    default:
      ;
  } // switch

  return result;
}

// font size
//______________________________________________________________________________
S_msrFontSize msrFontSize::create (
  msrFontSizeKind fontSizeKind)
{
  msrFontSize * o =
    new msrFontSize (
      fontSizeKind);
  assert(o!=0);

  return o;
}

S_msrFontSize msrFontSize::create (
  float numericFontSize)
{
  msrFontSize * o =
    new msrFontSize (
      numericFontSize);
  assert(o!=0);

  return o;
}

msrFontSize::msrFontSize (
  msrFontSizeKind fontSizeKind)
{
  fFontSizeKind = fontSizeKind;
}

msrFontSize::msrFontSize (
  float fontNumericSize)
{
  fFontSizeKind = kNumericFontSize;
  fFontNumericSize = fontNumericSize;
}

msrFontSize::~msrFontSize ()
{}
  
string msrFontSize::fontSizeKindAsString (
  msrFontSize::msrFontSizeKind fontSizeKind)
{
  string result;

  switch (fontSizeKind) {
    case msrFontSize::k_NoFontSize:
      result = "font size: none";
      break;

    case msrFontSize::kXXSmallFontSize:
      result = "font size: xx-small";
      break;
    case msrFontSize::kXSmallFontSize:
      result = "font size: x-small";
      break;
    case msrFontSize::kSmallFontSize:
      result = "font size: small";
      break;
    case msrFontSize::kMediumFontSize:
      result = "font size: medium";
      break;
    case msrFontSize::kLargeFontSize:
      result = "font size: large";
      break;
    case msrFontSize::kXLargeFontSize:
      result = "font size: x-large";
      break;
    case msrFontSize::kXXLargeFontSize:
      result = "font size: xx-large";
      break;
    case msrFontSize::kNumericFontSize:
      result = "font size: numeric";
      break;
    } // switch

  return result;
}

string msrFontSize::fontSizeKindAsString () const
{
  return
    fontSizeKindAsString (fFontSizeKind);
}

string msrFontSize::fontSizeAsString () const
{
  string result;
  
  switch (fFontSizeKind) {
    case msrFontSize::k_NoFontSize:
    case msrFontSize::kXXSmallFontSize:
    case msrFontSize::kXSmallFontSize:
    case msrFontSize::kSmallFontSize:
    case msrFontSize::kMediumFontSize:
    case msrFontSize::kLargeFontSize:
    case msrFontSize::kXLargeFontSize:
    case msrFontSize::kXXLargeFontSize:
      result = fontSizeKindAsString (fFontSizeKind);
      break;
      
    case msrFontSize::kNumericFontSize:
      result =
        to_string (fFontNumericSize);
      break;
    } // switch

  return result;
}

float msrFontSize::getFontNumericSize ()
{
  float result = 12; // JMI
  
  switch (fFontSizeKind) {
    case msrFontSize::k_NoFontSize:
    case msrFontSize::kXXSmallFontSize:
    case msrFontSize::kXSmallFontSize:
    case msrFontSize::kSmallFontSize:
    case msrFontSize::kMediumFontSize:
    case msrFontSize::kLargeFontSize:
    case msrFontSize::kXLargeFontSize:
    case msrFontSize::kXXLargeFontSize:
      {
        stringstream s;

        s <<
          "attempting to get font numeric size for a " <<
          fontSizeKindAsString (fFontSizeKind);

        msrInternalError (
          gXml2lyOptions->fInputSourceName,
          0, // JMI
          __FILE__, __LINE__,
          s.str ());
      }
      break;
      
    case msrFontSize::kNumericFontSize:
      result = fFontNumericSize;
      break;
    } // switch

  return result;
}

void msrFontSize::print (ostream& os)
{
  switch (fFontSizeKind) {
    case msrFontSize::k_NoFontSize:
    case msrFontSize::kXXSmallFontSize:
    case msrFontSize::kXSmallFontSize:
    case msrFontSize::kSmallFontSize:
    case msrFontSize::kMediumFontSize:
    case msrFontSize::kLargeFontSize:
    case msrFontSize::kXLargeFontSize:
    case msrFontSize::kXXLargeFontSize:
      os <<
        msrFontSize::fontSizeKindAsString (fFontSizeKind);
      break;
      
    case msrFontSize::kNumericFontSize:
      os <<
        fFontNumericSize;
      break;
    } // switch
}

// font style
//______________________________________________________________________________
string msrFontStyleKindAsString (
  msrFontStyleKind fontStyleKind)
{
  string result;
  
  switch (fontStyleKind) {
    case k_NoFontStyle:
      result = "font style: none";
      break;

    case kNormalFontStyle:
      result = "font style: normal";
      break;
    case KItalicFontStyle:
      result = "font style: italic";
      break;
    } // switch

  return result;
}

// font weight
//______________________________________________________________________________
string msrFontWeightKindAsString (
  msrFontWeightKind fontWeightKind)
{
  string result;
  
  switch (fontWeightKind) {
    case k_NoFontWeight:
      result = "font weight: none";
      break;

    case kNormalFontWeight:
      result = "font weight: normal";
      break;
    case kBoldFontWeight:
      result = "font weight: bold";
      break;
    } // switch

  return result;
}

// alignement
//______________________________________________________________________________
string msrJustifyKindAsString (
  msrJustifyKind justifyKind)
{
  string result;
  
  switch (justifyKind) {
    case k_NoJustify:
      result = "justify: none";
      break;

    case kLeftJustify:
      result = "justify: left";
      break;
    case kCenterJustify:
      result = "justify: center";
      break;
    case kRightJustify:
      result = "justify: right";
      break;
    } // switch

  return result;
}

string msrVerticalAlignmentKindAsString (
  msrVerticalAlignmentKind verticalAlignmentKind)
{
  string result;
  
  switch (verticalAlignmentKind) {
    case k_NoVerticalAlignment:
      result = "vertical alignment: none";
      break;

    case kTopVerticalAlignment:
      result = "vertical alignment: top";
      break;
    case kMiddleVerticalAlignment:
      result = "vertical alignment: middle";
      break;
    case kBottomVerticalAlignment:
      result = "vertical alignment: bottom";
      break;
    } // switch

  return result;
}

// direction
//______________________________________________________________________________
string msrDirectionKindAsString (
  msrDirectionKind directionKind)
{
  string result;
  
  switch (directionKind) {
    case k_NoDirection:
      result = "direction: none";
      break;

    case kUpDirection:
      result = "direction: up";
      break;
    case kDownDirection:
      result = "direction: down";
      break;
    } // switch

  return result;
}


// placement
//______________________________________________________________________________
string msrPlacementKindAsString (
  msrPlacementKind placementKind)
{
  string result;
  
  switch (placementKind) {
    case msrPlacementKind::k_NoPlacement:
      result = "placement: none";
      break;
    case msrPlacementKind::kAbovePlacement:
      result = "placement: above";
      break;
    case msrPlacementKind::kBelowPlacement:
      result = "placement: below";
      break;
    } // switch

  return result;
}

// durations
//______________________________________________________________________________
rational msrDurationKindAsWholeNotes (msrDurationKind durationKind)
{
  rational result;

  switch (durationKind) {
    case k_NoDuration:
      result = rational (0, 1);
      break;

    case k1024th:
      result = rational (1, 1024);
      break;
    case k512th:
      result = rational (1, 512);
      break;
    case k256th:
      result = rational (1, 256);
      break;
    case k128th:
      result = rational (1, 128);
      break;
    case k64th:
      result = rational (1, 64);
      break;
    case k32nd:
      result = rational (1, 32);
      break;
    case k16th:
      result = rational (1, 16);
      break;
    case kEighth:
      result = rational (1, 8);
      break;
    case kQuarter:
      result = rational (1, 4);
      break;
    case kHalf:
      result = rational (1, 2);
      break;
    case kWhole:
      result = rational (1, 1);
      break;
    case kBreve:
      result = rational (2, 1);
      break;
    case kLong:
      result = rational (4, 1);
      break;
    case kMaxima:
      result = rational (8, 1);
      break;
  } // switch

  return result;
}

msrDurationKind wholeNotesAsDurationKind (rational wholeNotes)
{
  msrDurationKind result = k_NoDuration;
  
  if (wholeNotes.getNumerator () == 1) {
    switch (wholeNotes.getDenominator ()) {
      case 1:
        result = kWhole;
        break;
      case 2:
        result = kHalf;
        break;
      case 4:
        result = kQuarter;
        break;
      case 8:
        result = kEighth;
        break;
      case 16:
        result = k16th;
        break;
      case 32:
        result = k32nd;
        break;
      case 64:
        result = k64th;
        break;
      case 128:
        result = k128th;
        break;
      case 256:
        result = k256th;
        break;
      case 512:
        result = k512th;
        break;
      case 1024:
        result = k1024th;
        break;
      default:
        ;
    } // switch
  }

  else if (wholeNotes.getDenominator () == 1) {
    switch (wholeNotes.getNumerator ()) {
      case 2:
        result = kBreve;
        break;
      case 4:
        result = kLong;
        break;
      case 8:
        result = kMaxima;
        break;
      default:
        ;
    } // switch
  }

  return result;
}

string msrDurationKindAsString (msrDurationKind durationKind)
{
  string result;

  switch (durationKind) {
    case k_NoDuration:
      result = "k_NoDuration???";
      break;

    case k1024th:
      result = "1024";
      break;
    case k512th:
      result = "512";
      break;
    case k256th:
      result = "256";
      break;
    case k128th:
      result = "128";
      break;
    case k64th:
      result = "64";
      break;
    case k32nd:
      result = "32";
      break;
    case k16th:
      result = "16";
      break;
    case kEighth:
      result = "8";
      break;
    case kQuarter:
      result = "4";
      break;
    case kHalf:
      result = "2";
      break;
    case kWhole:
      result = "1";
      break;
    case kBreve:
      result = "Breve";
      break;
    case kLong:
      result = "Long";
      break;
    case kMaxima:
      result = "Maxima";
      break;
  } // switch

  return result;
}

//_______________________________________________________________________________
string wholeNotesAsMsrString (
  int      inputLineNumber,
  rational wholeNotes,
  int&     dotsNumber)
{
//#define DEBUG_WHOLE_NOTES

#ifdef DEBUG_WHOLE_NOTES
  gLotIOsteam <<
    "--> wholeNotes: " << wholeNotes <<
    ", line " << inputLineNumber <<
    endl;
#endif

  wholeNotes.rationalise ();

#ifdef DEBUG_WHOLE_NOTES
  gLogIOstream <<
    "--> wholeNotes rationalised: " << wholeNotes <<
    endl;
#endif

  int
    numerator    = wholeNotes.getNumerator (),
    denominator  = wholeNotes.getDenominator ();

#ifdef DEBUG_WHOLE_NOTES
  gLogIOstream <<
    "--> numerator: " << numerator <<
    endl <<
    "--> denominator: " << denominator <<
    endl <<
    endl;
#endif

  if (numerator == 0) {
    return "0/1";
  }

  if (numerator == 1) {
    // a number of ??? JMI notes
    return to_string (denominator);
  }

  int
    numberOfDots = 0;

  stringstream s;

  // handle the quarter note fraction if any
  for ( ; ; ) {
    if (denominator <= 4) {
      break;
    }
    
    if (numerator == 1) {
      // a number of ??? JMI notes
      s << denominator;
      break;
    }
    
    if (numerator % 2 == 1) {
      numberOfDots += 1;
      
      numerator = (numerator - 1) / 2;
      denominator /= 2;
      
      rational r (numerator, denominator);
      r.rationalise ();
      
      numerator   = r.getNumerator (),
      denominator = r.getDenominator ();
    }
  } // for

  if (denominator <= 4) {
    // handle the 'above quarter note' part
    for ( ; ; ) {
      if (numerator == 1) {
        // a number of whole notes
        s << denominator;
        break;
      }
      
      if (denominator == 1) {
        // a number of whole notes
        switch (numerator) {
          case 1:
            s << "1";
            break;
          case 2:
            s << "breve";
            break;
          case 3:
            s << "breve";
            numberOfDots += 1;
            break;
          case 4:
            s << "long";
            break;
          case 6:
            s << "long";
            numberOfDots += 1;
            break;
          case 7:
            s << "long";
            numberOfDots += 2;
            break;
          case 8:
            s << "maxima";
            break;
          case 12:
            s << "maxima";
            numberOfDots += 1;
            break;
          case 14:
            s << "maxima";
            numberOfDots += 2;
            break;
          default:
          /* JMI
            s <<
              numerator << "/" << denominator <<
              " whole notes cannot be represented as an MSR string";
  
            msrInternalWarning (
              gXml2lyOptions->fInputSourceName,
              inputLineNumber,
              s.str ());
              */
            s <<
              numerator << "/" << denominator << "???";
        } // switch
        break;
      }
      
      if (numerator % 2 == 1) {
        // a number of quarter or half notes
        numberOfDots += 1;
        
        numerator = (numerator - 1) / 2;
        denominator /= 2;
        
        rational r (numerator, denominator);
        r.rationalise ();
        
        numerator   = r.getNumerator (),
        denominator = r.getDenominator ();
      }
    } // for
  }

  // append the dots if any
#ifdef DEBUG_WHOLE_NOTES
    s <<
      " %{" << numerator << "/" << denominator << "%} ";
#endif
    
  for (int i = 0; i < numberOfDots; i++) {
    s << ".";
  } // for
  
  // return the result
  dotsNumber = numberOfDots;
  
  return
    s.str ();
}

string wholeNotesAsMsrString (
  int      inputLineNumber,
  rational wholeNotes)
{
  int dotsNumber; // not used

  return
    wholeNotesAsMsrString (
      inputLineNumber,
      wholeNotes,
      dotsNumber);
}

string multipleRestWholeNoteAsMsrString (
  int      inputLineNumber, // JMI
  rational wholeNotes)
{
  stringstream s;
  
  rational
    denominatorAsFraction =
      rational (
        1,
        wholeNotes.getDenominator ());

  int numberOfWholeNotes =
    wholeNotes.getNumerator ();
    
  s <<
    wholeNotesAsLilypondString (
      inputLineNumber,
      denominatorAsFraction);

  if (numberOfWholeNotes != 1)
    s <<
      "*" << numberOfWholeNotes;

  return s.str ();
}

// measure style
//______________________________________________________________________________
string msrSlashTypeKindAsString (
  msrSlashTypeKind slashTypeKind)
{
  string result;

  switch (slashTypeKind) {
    case kSlashTypeStart:
      result = "SlashTypeStart";
      break;
    case kSlashTypeStop:
      result = "SlashTypeStop";
      break;
  } // switch

  return result;
}

string msrSlashUseDotsKindAsString (
  msrSlashUseDotsKind slashUseDotsKind)
{
  string result;

  switch (slashUseDotsKind) {
    case kSlashUseDotsYes:
      result = "SlashUseDotsYes";
      break;
    case kSlashUseDotsNo:
      result = "SlashUseDotsNo";
      break;
  } // switch

  return result;
}

string msrSlashUseStemsKindAsString (
  msrSlashUseStemsKind slashUseStemsKind)
{
  string result;

  switch (slashUseStemsKind) {
    case kSlashUseStemsYes:
      result = "SlashUseStemsYes";
      break;
    case kSlashUseStemsNo:
      result = "SlashUseStemsNo";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
string msrLineTypeKindAsString (
  msrLineTypeKind lineTypeKind)
{
  string result;
  
  switch (lineTypeKind) {
    case kLineTypeSolid:
      result = "lineTypeSolid";
      break;
    case kLineTypeDashed:
      result = "lineTypeDashed";
      break;
    case kLineTypeDotted:
      result = "lineTypeDotted";
      break;
    case kLineTypeWavy:
      result = "lineTypeWavy";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
string msrTremoloTypeKindAsString (
  msrTremoloTypeKind tremoloTypeKind)
{
  string result;
  
  switch (tremoloTypeKind) {
    case kTremoloTypeSingle:
      result = "tremoloTypeSingle";
      break;
    case kTremoloTypeStart:
      result = "tremoloTypeStart";
      break;
    case kTremoloTypeStop:
      result = "tremoloTypeStop";
      break;
    case k_NoTremoloType:
      result = "k_NoTremoloType???";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
string msrTechnicalTypeKindAsString (
  msrTechnicalTypeKind technicalTypeKind)
{
  string result;
  
  switch (technicalTypeKind) {
    case kTechnicalTypeStart:
      result = "technicalTypeStart";
      break;
    case kTechnicalTypeStop:
      result = "technicalTypeStop";
      break;
    case k_NoTechnicalType:
      result = "k_NoTechnicalType???";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
string msrSpannerTypeKindAsString (
  msrSpannerTypeKind spannerTypeKind)
{
  string result;
  
  switch (spannerTypeKind) {
    case kSpannerTypeStart:
      result = "spannerTypeStart";
      break;
    case kSpannerTypeStop:
      result = "spannerTypeStop";
      break;
    case kSpannerTypeContinue:
      result = "spannerTypeContinue";
      break;
    case k_NoSpannerType:
      result = "k_NoSpannerType???";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
S_msrChordItem msrChordItem::create (
  int             inputLineNumber,
  int             chordItemNumber,
  msrIntervalKind chordItemIntervalKind)
{
  msrChordItem* o =
    new msrChordItem (
      inputLineNumber,
      chordItemNumber,
      chordItemIntervalKind);
  assert(o!=0);

  return o;
}

msrChordItem::msrChordItem (
  int             inputLineNumber,
  int             chordItemNumber,
  msrIntervalKind chordItemIntervalKind)
  // JMI  : msrElement (inputLineNumber)
{
  fChordItemNumber       = chordItemNumber;
  fChordItemIntervalKind = chordItemIntervalKind;

  if (TRACE_MSR_BASIC_TYPES) {
    gLogIOstream <<
      "==> Creating chord item '" <<
      chordItemAsString () <<
      "'" <<
      endl;
  }
}

msrChordItem::~msrChordItem()
{}


string msrChordItem::chordItemAsString () const
{
  stringstream s;

  s <<
    "ChordItem" <<
    " " << fChordItemNumber <<
    ": " << msrIntervalKindAsString (fChordItemIntervalKind);

  return s.str ();
}

/* JMI
void msrChordItem::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrChordItem::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrChordItem>*
    p =
      dynamic_cast<visitor<S_msrChordItem>*> (v)) {
        S_msrChordItem elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrChordItem::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrChordItem::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrChordItem::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrChordItem>*
    p =
      dynamic_cast<visitor<S_msrChordItem>*> (v)) {
        S_msrChordItem elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrChordItem::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrChordItem::browseData (basevisitor* v)
{}
*/

ostream& operator<< (ostream& os, const S_msrChordItem& elt)
{
  elt->print (os);
  return os;
}

void msrChordItem::print (ostream& os)
{  
  os <<
    "ChordItem" <<
    ", number: " << fChordItemNumber <<
    ", interval: " << msrIntervalKindAsString (fChordItemIntervalKind) <<
  /* JMI
    ", line: " << fInputLineNumber <<
    */
    endl;
}

//______________________________________________________________________________
map<msrHarmonyKind, S_msrChordIntervals>
  gChordIntervalsMap;

S_msrChordIntervals msrChordIntervals::create (
  int            inputLineNumber,
  msrHarmonyKind chordIntervalsHarmonyKind)
{
  msrChordIntervals* o =
    new msrChordIntervals (
      inputLineNumber,
      chordIntervalsHarmonyKind);
  assert(o!=0);

  return o;
}

msrChordIntervals::msrChordIntervals (
  int            inputLineNumber,
  msrHarmonyKind chordIntervalsHarmonyKind)
   // JMI : msrElement (inputLineNumber)
{
  fChordIntervalsHarmonyKind = chordIntervalsHarmonyKind;

  if (TRACE_MSR_BASIC_TYPES) {
    gLogIOstream <<
      "==> Creating chord intervals '" <<
      chordIntervalsAsString () <<
      "'" <<
      endl;
  }

  initializeChordIntervals ();
}

void msrChordIntervals::initializeChordIntervals ()
{
  // append chord items to chord intervals
  switch (fChordIntervalsHarmonyKind) {
    case k_NoHarmony:
      break;

    case kMajorHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth)
          );
/* JMI
        gLogIOstream <<
          endl <<
          "*****************" <<
          endl <<
          "msrChordIntervals::initializeChordIntervals(), this =" <<
          endl;
        print (gLogIOstream);
        gLogIOstream <<
          endl;
          */
      }
      break;
    
    case kMinorHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMinorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth)
          );
      }
      break;
    
    case kAugmentedHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kAugmentedFifth)
          );
      }
      break;
    
    case kDiminishedHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMinorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kDiminishedFifth)
          );
      }
      break;
    
    case kDominantHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMinorSeventh)
          );
      }
      break;
    
    case kMajorSeventhHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMajorSeventh)
          );
      }
      break;
    
     case kMinorSeventhHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMinorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMinorSeventh)
          );
      }
      break;
    
    case kDiminishedSeventhHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMinorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kDiminishedFifth)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kDiminishedSeventh)
          );
      }
      break;
    
    case kAugmentedSeventhHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kAugmentedFifth)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMinorSeventh)
          );
      }
      break;
    
    case kHalfDiminishedHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMinorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kDiminishedFifth)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMinorSeventh)
          );
      }
      break;
    
     case kMajorMinorHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMinorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMajorSeventh)
          );
      }
      break;
    
    case kMajorSixthHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMajorSixth)
          );
      }
      break;
    
    case kMinorSixthHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMinorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMajorSixth)
          );
      }
      break;
    
    case kDominantNinthHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMinorSeventh)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            5, // chord item number
            kMajorSeventh)
          );
      }
      break;

    case kMajorNinthHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMajorSeventh)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            5, // chord item number
            kMajorNinth)
          );
      }
      break;
    
    case kMinorNinthHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMinorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMinorSeventh)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            5, // chord item number
            kMajorNinth)
          );
      }
      break;
    
    case kDominantEleventhHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMinorSeventh)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            5, // chord item number
            kMajorSeventh)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            6, // chord item number
            kPerEleventh)
          );
      }
      break;

    case kMajorEleventhHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMajorSeventh)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            5, // chord item number
            kMajorNinth)
          );
         appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            6, // chord item number
            kPerEleventh)
          );
      }
      break;
    
    case kMinorEleventhHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMinorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMinorSeventh)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            5, // chord item number
            kMajorNinth)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            6, // chord item number
            kPerEleventh)
          );
      }
      break;
    
    case kDominantThirteenthHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMinorSeventh)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            5, // chord item number
            kMajorSeventh)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            6, // chord item number
            kPerEleventh)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            7, // chord item number
            kMajorThirteenth)
          );
      }
      break;

    case kMajorThirteenthHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMajorSeventh)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            5, // chord item number
            kMajorNinth)
          );
         appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            6, // chord item number
            kPerEleventh)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            7, // chord item number
            kMajorThirteenth)
          );
      }
      break;
    
    case kMinorThirteenthHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMinorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMinorSeventh)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            5, // chord item number
            kMajorNinth)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            6, // chord item number
            kPerEleventh)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            7, // chord item number
            kMajorThirteenth)
          );
      }
      break;
    
    case kSuspendedSecondHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorSecond)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth)
          );
      }
      break;
    
    case kSuspendedFourthHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kPerFourth)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth)
          );
      }
      break;

/*
* kNeapolitan f aes des' in:
*
* c e g c' -> f f aes des' -> d g d b -> c e g c'

they are three different pre-dominant chords that are taught to American undergrads in a sophomore theory course.

in E major:
Italian = C E A#
French = C E F# A#
German = C E G A#
Tristan = C D# F# A#

in all of them, the C and A# in theory want to fan out to B (the dominant).  This is, of course, in theory - Wagner’s use of the Tristan chord, which he clearly named his opera after, has the A# moving down to A, or the 7th of the dominant (I’m transposing to fit w/ the example above).  Wagner obviously did not pay much attention during his sophomore music theory course…
*/      

    case kNeapolitanHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMinorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kDiminishedSixth)
          );
      }
      break;
    
    case kItalianHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kAugmentedSixth)
          );
      }
      break;
    
    case kFrenchHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kAugmentedFourth)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kAugmentedSixth)
          );
      }
      break;
    
    case kGermanHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kAugmentedSixth)
          );
      }
      break;
    
    case kPedalHarmony:
      break;
      
    case kPowerHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kPerFifth)
          );
      }
      break;
    
    case kTristanHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kUnisson)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kAugmentedSecond)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kAugmentedFourth)
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kAugmentedSixth)
          );
      }
      break;
    
    case kOtherHarmony:
      break;
      
    case kNoneHarmony:
      break;
  } // switch

/* JMI
  // register chord intervals in map
  gChordIntervalsMap [fChordIntervalsHarmonyKind] = this;
  */
}

msrChordIntervals::~msrChordIntervals ()
{}

void msrChordIntervals::appendChordItemToChordIntervals (
  S_msrChordItem chordItem)
{
  // set the input line number and chord item number // JMI

  // append the chord item
  fChordIntervalsItems.push_back (
    chordItem);
}

string msrChordIntervals::chordIntervalsAsString () const
{
  stringstream s;

  s <<
    "ChordIntervals" <<
    ", " <<
    msrHarmonyKindAsString (fChordIntervalsHarmonyKind) <<
    ", " <<
    singularOrPlural (
      fChordIntervalsItems.size (), "item", "items");

  return s.str ();
}

/* JMI
void msrChordIntervals::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrChordIntervals::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrChordIntervals>*
    p =
      dynamic_cast<visitor<S_msrChordIntervals>*> (v)) {
        S_msrChordIntervals elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrChordIntervals::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrChordIntervals::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrChordIntervals::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrChordIntervals>*
    p =
      dynamic_cast<visitor<S_msrChordIntervals>*> (v)) {
        S_msrChordIntervals elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrChordIntervals::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrChordIntervals::browseData (basevisitor* v)
{}
*/

ostream& operator<< (ostream& os, const S_msrChordIntervals& elt)
{
  elt->print (os);
  return os;
}

void msrChordIntervals::print (ostream& os)
{  
  os <<
    "ChordIntervals" <<
    ", harmonyKind: " <<
    msrHarmonyKindAsString (fChordIntervalsHarmonyKind) <<
    ", " <<
    singularOrPlural (
      fChordIntervalsItems.size (), "item", "items") <<
  /* JMI
    ", line: " << fInputLineNumber <<
    */
    endl;

  gIndenter++;
    
  if (fChordIntervalsItems.size ()) {
    for (unsigned int i = 0; i < fChordIntervalsItems.size (); i++) {
      S_msrChordItem
        chordItem =
          fChordIntervalsItems [i];

      gLogIOstream <<
        chordItem->chordItemAsString () <<
        endl;
    } // for
  }
  else {
    gLogIOstream <<
      "no intervals" <<
      endl;
  }

  gIndenter--;
}

void msrChordIntervals::printAllChordIntervals ()
{
  for (
    msrHarmonyKind harmonyKind = kMajorHarmony;
    harmonyKind <= kTristanHarmony;
    harmonyKind = msrHarmonyKind (harmonyKind + 1)) {
    // create the chord intervals
    S_msrChordIntervals
      chordIntervals =
        msrChordIntervals::create (
          0, // inputLineNumber
          harmonyKind);

    // print it
    gLogIOstream <<
      chordIntervals <<
      endl;
  } // for
}

list<msrSemiTonesPitchKind> buildSemiTonesChord (
  msrHarmonyKind        harmonyKind,
  msrSemiTonesPitchKind rootNote)
{
  list<msrSemiTonesPitchKind> result;

  // create the chord intervals
  S_msrChordIntervals
    chordIntervals =
      msrChordIntervals::create (
        0, // inputLineNumber
        harmonyKind);

  // add the root to the chord
  result.push_back (rootNote);

  // add the other notes to the chord
  const vector<S_msrChordItem>&
    chordIntervalsItems =
      chordIntervals->getChordIntervalsItems ();

  for (unsigned int i = 1; i << chordIntervalsItems.size (); i++) {
    result.push_back (rootNote);
  } // for

  return result;
}

//______________________________________________________________________________
S_msrChordNotes msrChordNotes::create (
  int                   inputLineNumber,
  msrSemiTonesPitchKind chordRootNote,
  msrHarmonyKind        chordHarmonyKind)
{
  msrChordNotes* o =
    new msrChordNotes (
      inputLineNumber,
      chordRootNote,
      chordHarmonyKind);
  assert(o!=0);

  return o;
}

msrChordNotes::msrChordNotes (
  int                   inputLineNumber,
  msrSemiTonesPitchKind chordRootNote,
  msrHarmonyKind        chordHarmonyKind)
   // JMI : msrElement (inputLineNumber)
{
  fChordRootNote    = chordRootNote;
  fChordHarmonyKind = chordHarmonyKind;

  if (TRACE_MSR_BASIC_TYPES) {
    gLogIOstream <<
      "==> Creating chordNotes '" <<
      chordNotesAsString () <<
      "'" <<
      endl;
  }

  // add the root to the chord notes
  fChordSemiTonesPitches.push_back (fChordRootNote);

  // add the other notes to the chord notes
  S_msrChordIntervals
    chordIntervals =
      msrChordIntervals::create (
        inputLineNumber,
        fChordHarmonyKind);
      
  const vector<S_msrChordItem>&
    chordItems =
      chordIntervals->getChordIntervalsItems ();

  for (unsigned int i = 1; i < chordItems.size (); i++) {
    msrSemiTonesPitchKind
      note = fChordRootNote;
      
    fChordSemiTonesPitches.push_back (note);
  } // for
}

msrChordNotes::~msrChordNotes ()
{}

string msrChordNotes::chordNotesAsString () const
{
  stringstream s;

  s <<
    "ChordNotes" <<
    ", " <<
    msrHarmonyKindAsString (fChordHarmonyKind) <<
    ", " <<
    singularOrPlural (
      fChordSemiTonesPitches.size (), "note", "notes");

  return s.str ();
}

/* JMI
void msrChordNotes::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrChordNotes::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrChordNotes>*
    p =
      dynamic_cast<visitor<S_msrChordNotes>*> (v)) {
        S_msrChordNotes elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrChordNotes::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrChordNotes::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrChordNotes::acceptOut()" <<
      endl;
  }

  if (visitor<S_msrChordNotes>*
    p =
      dynamic_cast<visitor<S_msrChordNotes>*> (v)) {
        S_msrChordNotes elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrChordNotes::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrChordNotes::browseData (basevisitor* v)
{}
*/

ostream& operator<< (ostream& os, const S_msrChordNotes& elt)
{
  elt->print (os);
  return os;
}

void msrChordNotes::print (ostream& os)
{  
  os <<
    "ChordNotes" <<
  /* JMI
    ", line: " << fInputLineNumber <<
    */
    endl;

  gIndenter++;

  const int fieldWidth = 17;
  
  os << left <<
    setw (fieldWidth) <<
    "chordRootNote" << " : " <<
    msrSemiTonesPitchKindAsString (fChordRootNote) <<
    endl <<
    setw (fieldWidth) <<
    "chordHarmonyKind" << " : " <<
    msrHarmonyKindAsString (fChordHarmonyKind) <<
    endl;

  if (fChordSemiTonesPitches.size ()) {
    os <<
    singularOrPlural (
      fChordSemiTonesPitches.size (), "note", "notes") <<
    ":" <<
    endl;

    gIndenter++;

    for (unsigned int i = 0; i < fChordSemiTonesPitches.size (); i++) {
      msrSemiTonesPitchKind
        semiTonesPitchKind =
          fChordSemiTonesPitches [i];

      os <<
        msrSemiTonesPitchKindAsString (semiTonesPitchKind) <<
        endl;
    } // for
    
    gIndenter--;
  }
  else {
    os <<
      "no notes" <<
      endl;
  }

  gIndenter--;
}


}
