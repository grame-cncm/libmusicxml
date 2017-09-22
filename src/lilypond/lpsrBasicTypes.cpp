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

/*
#include <iostream>
#include <sstream>
#include <stdlib.h>     / * abort, NULL * /
#include <cassert>

#include <vector>
#include <map>

#include <iomanip>      // setw, ...

#include "rational.h"

#include "utilities.h"
*/

#include "lpsrBasicTypes.h"


using namespace std;

namespace MusicXML2 {

// useful shortcut macros
#define idtr indenter::gIndenter
#define tab  indenter::gIndenter.getSpacer ()

// intervals
//______________________________________________________________________________
int msrIntervalAsSemiTones (
  msrInterval interval)
{
  int result = -1;

  switch (interval) {
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

string msrIntervalAsString (
  msrInterval interval)
{
  string result;

  switch (interval) {
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
  msrInterval interval)
{
  string result;
  
  switch (interval) {
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

// intervals
//______________________________________________________________________________
string harmonyKindAsString (
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

string harmonyKindAsShortString (
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
  cerr << idtr <<
    "Harmonies chord intervals:" <<
    " (" << gChordIntervalsMap.size () << ")" <<
    endl;

  idtr++;

  for (int i = k_NoHarmony; i <= kNoneHarmony; i++) {
    msrHarmonyKind
      harmonyKind =
        msrHarmonyKind (i);

    cerr << idtr <<
// JMI      "i:" << i << " " <<
      harmonyKindAsString (harmonyKind) << ":" <<
      endl;

    idtr++;
    
    S_msrChordIntervals
      chordIntervals =
        gChordIntervalsMap [harmonyKind];

    if (chordIntervals) {
      cerr <<
        chordIntervals <<
        endl;
    }
    else {
      cerr << idtr <<
        "no intervals" <<
        endl;
    }

    idtr--;
    
    cerr <<
      endl;
  } // for

  idtr--;

  cerr <<
    endl;
}

// notes names languages
// ------------------------------------------------------

map<string, msrQuarterTonesPitchesLanguage>
  gQuarterTonesPitchesLanguagesMap;

map<msrQuarterTonesPitch, string> gNederlandsPitchName;
map<msrQuarterTonesPitch, string> gCatalanPitchName;
map<msrQuarterTonesPitch, string> gDeutschPitchName;
map<msrQuarterTonesPitch, string> gEnglishPitchName;
map<msrQuarterTonesPitch, string> gEspanolPitchName;
map<msrQuarterTonesPitch, string> gFrancaisPitchName;
map<msrQuarterTonesPitch, string> gItalianoPitchName;
map<msrQuarterTonesPitch, string> gNorskPitchName;
map<msrQuarterTonesPitch, string> gPortuguesPitchName;
map<msrQuarterTonesPitch, string> gSuomiPitchName;
map<msrQuarterTonesPitch, string> gSvenskaPitchName;
map<msrQuarterTonesPitch, string> gVlaamsPitchName;

void initializeQuarterTonesPitchesLanguages ()
{
  gQuarterTonesPitchesLanguagesMap ["nederlands"] = kNederlands;
  gQuarterTonesPitchesLanguagesMap ["catalan"]    = kCatalan;
  gQuarterTonesPitchesLanguagesMap ["deutsch"]    = kDeutsch;
  gQuarterTonesPitchesLanguagesMap ["english"]    = kEnglish;
  gQuarterTonesPitchesLanguagesMap ["espanol"]    = kEspanol;
  gQuarterTonesPitchesLanguagesMap ["italiano"]   = kItaliano;
  gQuarterTonesPitchesLanguagesMap ["francais"]   = kFrancais;
  gQuarterTonesPitchesLanguagesMap ["norsk"]      = kNorsk;
  gQuarterTonesPitchesLanguagesMap ["portugues"]  = kPortugues;
  gQuarterTonesPitchesLanguagesMap ["suomi"]      = kSuomi;
  gQuarterTonesPitchesLanguagesMap ["svenska"]    = kSvenska;
  gQuarterTonesPitchesLanguagesMap ["vlaams"]     = kVlaams;

  // nederlands
  gNederlandsPitchName [k_NoQuarterTonesPitch]  = "-";
  gNederlandsPitchName [k_Rest]                 = "r";
  
  gNederlandsPitchName [k_aDoubleFlat]  = "aeses";
  gNederlandsPitchName [k_aSesquiFlat]  = "aeseh";
  gNederlandsPitchName [k_aFlat]        = "aes";
  gNederlandsPitchName [k_aSemiFlat]    = "aeh";
  gNederlandsPitchName [k_aNatural]     = "a";
  gNederlandsPitchName [k_aSemiSharp]   = "aih";
  gNederlandsPitchName [k_aSharp]       = "ais";
  gNederlandsPitchName [k_aSesquiSharp] = "aisih";
  gNederlandsPitchName [k_aDoubleSharp] = "aisis";
    
  gNederlandsPitchName [k_bDoubleFlat]  = "beses";
  gNederlandsPitchName [k_bSesquiFlat]  = "beseh";
  gNederlandsPitchName [k_bFlat]        = "bes";
  gNederlandsPitchName [k_bSemiFlat]    = "beh";
  gNederlandsPitchName [k_bNatural]     = "b";
  gNederlandsPitchName [k_bSemiSharp]   = "bih";
  gNederlandsPitchName [k_bSharp]       = "bis";
  gNederlandsPitchName [k_bSesquiSharp] = "bisih";
  gNederlandsPitchName [k_bDoubleSharp] = "bisis";
    
  gNederlandsPitchName [k_cDoubleFlat]  = "ceses";
  gNederlandsPitchName [k_cSesquiFlat]  = "ceseh";
  gNederlandsPitchName [k_cFlat]        = "ces";
  gNederlandsPitchName [k_cSemiFlat]    = "ceh";
  gNederlandsPitchName [k_cNatural]     = "c";
  gNederlandsPitchName [k_cSemiSharp]   = "cih";
  gNederlandsPitchName [k_cSharp]       = "cis";
  gNederlandsPitchName [k_cSesquiSharp] = "cisih";
  gNederlandsPitchName [k_cDoubleSharp] = "cisis";
    
  gNederlandsPitchName [k_dDoubleFlat]  = "deses";
  gNederlandsPitchName [k_dSesquiFlat]  = "deseh";
  gNederlandsPitchName [k_dFlat]        = "des";
  gNederlandsPitchName [k_dSemiFlat]    = "deh";
  gNederlandsPitchName [k_dNatural]     = "d";
  gNederlandsPitchName [k_dSemiSharp]   = "dih";
  gNederlandsPitchName [k_dSharp]       = "dis";
  gNederlandsPitchName [k_dSesquiSharp] = "disih";
  gNederlandsPitchName [k_dDoubleSharp] = "disis";

  gNederlandsPitchName [k_eDoubleFlat]  = "eeses";
  gNederlandsPitchName [k_eSesquiFlat]  = "eeseh";
  gNederlandsPitchName [k_eFlat]        = "ees";
  gNederlandsPitchName [k_eSemiFlat]    = "eeh";
  gNederlandsPitchName [k_eNatural]     = "e";
  gNederlandsPitchName [k_eSemiSharp]   = "eih";
  gNederlandsPitchName [k_eSharp]       = "eis";
  gNederlandsPitchName [k_eSesquiSharp] = "eisih";
  gNederlandsPitchName [k_eDoubleSharp] = "eisis";
    
  gNederlandsPitchName [k_fDoubleFlat]  = "feses";
  gNederlandsPitchName [k_fSesquiFlat]  = "feseh";
  gNederlandsPitchName [k_fFlat]        = "fes";
  gNederlandsPitchName [k_fSemiFlat]    = "feh";
  gNederlandsPitchName [k_fNatural]     = "f";
  gNederlandsPitchName [k_fSemiSharp]   = "fih";
  gNederlandsPitchName [k_fSharp]       = "fis";
  gNederlandsPitchName [k_fSesquiSharp] = "fisih";
  gNederlandsPitchName [k_fDoubleSharp] = "fisis";
    
  gNederlandsPitchName [k_gDoubleFlat]  = "geses";
  gNederlandsPitchName [k_gSesquiFlat]  = "geseh";
  gNederlandsPitchName [k_gFlat]        = "ges";
  gNederlandsPitchName [k_gSemiFlat]    = "geh";
  gNederlandsPitchName [k_gNatural]     = "g";
  gNederlandsPitchName [k_gSemiSharp]   = "gih";
  gNederlandsPitchName [k_gSharp]       = "gis";
  gNederlandsPitchName [k_gSesquiSharp] = "gisih";
  gNederlandsPitchName [k_gDoubleSharp] = "gisis";

  // catalan
  gCatalanPitchName [k_NoQuarterTonesPitch]  = "-";
  gCatalanPitchName [k_Rest]                 = "r";
  
  gCatalanPitchName [k_aDoubleFlat]  = "labb";
  gCatalanPitchName [k_aSesquiFlat]  = "labSesquiFlat???";
  gCatalanPitchName [k_aFlat]        = "lab";
  gCatalanPitchName [k_aSemiFlat]    = "aSemiFlat???";
  gCatalanPitchName [k_aNatural]     = "la";
  gCatalanPitchName [k_aSemiSharp]   = "aSemiSharp???";
  gCatalanPitchName [k_aSharp]       = "lad";
  gCatalanPitchName [k_aSesquiSharp] = "laSesquiSharp???";
  gCatalanPitchName [k_aDoubleSharp] = "ladd";
    
  gCatalanPitchName [k_bDoubleFlat]  = "sibb";
  gCatalanPitchName [k_bSesquiFlat]  = "sibSesquiFlat???";
  gCatalanPitchName [k_bFlat]        = "sib";
  gCatalanPitchName [k_bSemiFlat]    = "bSemiFlat???";
  gCatalanPitchName [k_bNatural]     = "b";
  gCatalanPitchName [k_bSemiSharp]   = "bSemiSharp???";
  gCatalanPitchName [k_bSharp]       = "sid";
  gCatalanPitchName [k_bSesquiSharp] = "siSesquiSharp???";
  gCatalanPitchName [k_bDoubleSharp] = "sidd";
    
  gCatalanPitchName [k_cDoubleFlat]  = "dobb";
  gCatalanPitchName [k_cSesquiFlat]  = "doSesquiFlat???";
  gCatalanPitchName [k_cFlat]        = "dob";
  gCatalanPitchName [k_cSemiFlat]    = "cSemiFlat???";
  gCatalanPitchName [k_cNatural]     = "do";
  gCatalanPitchName [k_cSemiSharp]   = "cSemiSharp???";
  gCatalanPitchName [k_cSharp]       = "dod";
  gCatalanPitchName [k_cSesquiSharp] = "doSesquiSharp???";
  gCatalanPitchName [k_cDoubleSharp] = "dodd";
    
  gCatalanPitchName [k_dDoubleFlat]  = "rebb";
  gCatalanPitchName [k_dSesquiFlat]  = "reSesquiFlat???";
  gCatalanPitchName [k_dFlat]        = "reb";
  gCatalanPitchName [k_dSemiFlat]    = "dSemiFlat???";
  gCatalanPitchName [k_dNatural]     = "re";
  gCatalanPitchName [k_dSemiSharp]   = "dSemiSharp???";
  gCatalanPitchName [k_dSharp]       = "red";
  gCatalanPitchName [k_dSesquiSharp] = "reSesquiSharp???";
  gCatalanPitchName [k_dDoubleSharp] = "redd";

  gCatalanPitchName [k_eDoubleFlat]  = "mibb";
  gCatalanPitchName [k_eSesquiFlat]  = "miSesquiFlat???";
  gCatalanPitchName [k_eFlat]        = "mib";
  gCatalanPitchName [k_eSemiFlat]    = "eSemiFlat???";
  gCatalanPitchName [k_eNatural]     = "mi";
  gCatalanPitchName [k_eSemiSharp]   = "eSemiSharp???";
  gCatalanPitchName [k_eSharp]       = "mid";
  gCatalanPitchName [k_eSesquiSharp] = "miSesquiSharp???";
  gCatalanPitchName [k_eDoubleSharp] = "midd";
    
  gCatalanPitchName [k_fDoubleFlat]  = "fabb";
  gCatalanPitchName [k_fSesquiFlat]  = "faSesquiFlat???";
  gCatalanPitchName [k_fFlat]        = "fab";
  gCatalanPitchName [k_fSemiFlat]    = "fSemiFlat???";
  gCatalanPitchName [k_fNatural]     = "fa";
  gCatalanPitchName [k_fSemiSharp]   = "fSemiSharp???";
  gCatalanPitchName [k_fSharp]       = "fad";
  gCatalanPitchName [k_fSesquiSharp] = "faSesquiSharp???";
  gCatalanPitchName [k_fDoubleSharp] = "fadd";
    
  gCatalanPitchName [k_gDoubleFlat]  = "solbb";
  gCatalanPitchName [k_gSesquiFlat]  = "solSesquiFlat???";
  gCatalanPitchName [k_gFlat]        = "solb";
  gCatalanPitchName [k_gSemiFlat]    = "gSemiFlat???";
  gCatalanPitchName [k_gNatural]     = "sol";
  gCatalanPitchName [k_gSemiSharp]   = "gSemiSharp???";
  gCatalanPitchName [k_gSharp]       = "sold";
  gCatalanPitchName [k_gSesquiSharp] = "solSesquiSharp???";
  gCatalanPitchName [k_gDoubleSharp] = "soldd";

  // deutsch
  gDeutschPitchName [k_NoQuarterTonesPitch]  = "-";
  gDeutschPitchName [k_Rest]                 = "r";
  
  gDeutschPitchName [k_aDoubleFlat]  = "asas";
  gDeutschPitchName [k_aSesquiFlat]  = "asah";
  gDeutschPitchName [k_aFlat]        = "as";
  gDeutschPitchName [k_aSemiFlat]    = "aeh";
  gDeutschPitchName [k_aNatural]     = "a";
  gDeutschPitchName [k_aSemiSharp]   = "aih";
  gDeutschPitchName [k_aSharp]       = "ais";
  gDeutschPitchName [k_aSesquiSharp] = "aisih";
  gDeutschPitchName [k_aDoubleSharp] = "aisis";
    
  gDeutschPitchName [k_bDoubleFlat]  = "heses";
  gDeutschPitchName [k_bSesquiFlat]  = "heseh";
  gDeutschPitchName [k_bFlat]        = "b";
  gDeutschPitchName [k_bSemiFlat]    = "beh";
  gDeutschPitchName [k_bNatural]     = "h";
  gDeutschPitchName [k_bSemiSharp]   = "hih";
  gDeutschPitchName [k_bSharp]       = "his";
  gDeutschPitchName [k_bSesquiSharp] = "hisih";
  gDeutschPitchName [k_bDoubleSharp] = "hisis";
    
  gDeutschPitchName [k_cDoubleFlat]  = "ceses";
  gDeutschPitchName [k_cSesquiFlat]  = "ceseh";
  gDeutschPitchName [k_cFlat]        = "ces";
  gDeutschPitchName [k_cSemiFlat]    = "ceh";
  gDeutschPitchName [k_cNatural]     = "c";
  gDeutschPitchName [k_cSemiSharp]   = "cih";
  gDeutschPitchName [k_cSharp]       = "cis";
  gDeutschPitchName [k_cSesquiSharp] = "cisih";
  gDeutschPitchName [k_cDoubleSharp] = "cisis";
    
  gDeutschPitchName [k_dDoubleFlat]  = "deses";
  gDeutschPitchName [k_dSesquiFlat]  = "deseh";
  gDeutschPitchName [k_dFlat]        = "des";
  gDeutschPitchName [k_dSemiFlat]    = "deh";
  gDeutschPitchName [k_dNatural]     = "d";
  gDeutschPitchName [k_dSemiSharp]   = "dih";
  gDeutschPitchName [k_dSharp]       = "dis";
  gDeutschPitchName [k_dSesquiSharp] = "disih";
  gDeutschPitchName [k_dDoubleSharp] = "disis";

  gDeutschPitchName [k_eDoubleFlat]  = "eses";
  gDeutschPitchName [k_eSesquiFlat]  = "esseh";
  gDeutschPitchName [k_eFlat]        = "es";
  gDeutschPitchName [k_eSemiFlat]    = "eeh";
  gDeutschPitchName [k_eNatural]     = "e";
  gDeutschPitchName [k_eSemiSharp]   = "eih";
  gDeutschPitchName [k_eSharp]       = "eis";
  gDeutschPitchName [k_eSesquiSharp] = "eisih";
  gDeutschPitchName [k_eDoubleSharp] = "eisis";
    
  gDeutschPitchName [k_fDoubleFlat]  = "feses";
  gDeutschPitchName [k_fSesquiFlat]  = "feseh";
  gDeutschPitchName [k_fFlat]        = "fes";
  gDeutschPitchName [k_fSemiFlat]    = "feh";
  gDeutschPitchName [k_fNatural]     = "f";
  gDeutschPitchName [k_fSemiSharp]   = "fih";
  gDeutschPitchName [k_fSharp]       = "fis";
  gDeutschPitchName [k_fSesquiSharp] = "fisih";
  gDeutschPitchName [k_fDoubleSharp] = "fisis";
    
  gDeutschPitchName [k_gDoubleFlat]  = "geses";
  gDeutschPitchName [k_gSesquiFlat]  = "geseh";
  gDeutschPitchName [k_gFlat]        = "ges";
  gDeutschPitchName [k_gSemiFlat]    = "geh";
  gDeutschPitchName [k_gNatural]     = "g";
  gDeutschPitchName [k_gSemiSharp]   = "gih";
  gDeutschPitchName [k_gSharp]       = "gis";
  gDeutschPitchName [k_gSesquiSharp] = "gisih";
  gDeutschPitchName [k_gDoubleSharp] = "gisis";

  // english
  gEnglishPitchName [k_NoQuarterTonesPitch]  = "-";
  gEnglishPitchName [k_Rest]                 = "r";
  
  gEnglishPitchName [k_aDoubleFlat]  = "aff";
  gEnglishPitchName [k_aSesquiFlat]  = "atqf";
  gEnglishPitchName [k_aFlat]        = "af";
  gEnglishPitchName [k_aSemiFlat]    = "aqf";
  gEnglishPitchName [k_aNatural]     = "a";
  gEnglishPitchName [k_aSemiSharp]   = "aqs";
  gEnglishPitchName [k_aSharp]       = "as";
  gEnglishPitchName [k_aSesquiSharp] = "atqs";
  gEnglishPitchName [k_aDoubleSharp] = "a";
    
  gEnglishPitchName [k_bDoubleFlat]  = "bfqf";
  gEnglishPitchName [k_bSesquiFlat]  = "btqf";
  gEnglishPitchName [k_bFlat]        = "bf";
  gEnglishPitchName [k_bSemiFlat]    = "bqf";
  gEnglishPitchName [k_bNatural]     = "b";
  gEnglishPitchName [k_bSemiSharp]   = "bqs";
  gEnglishPitchName [k_bSharp]       = "bs";
  gEnglishPitchName [k_bSesquiSharp] = "btqs";
  gEnglishPitchName [k_bDoubleSharp] = "bx";
    
  gEnglishPitchName [k_cDoubleFlat]  = "cff";
  gEnglishPitchName [k_cSesquiFlat]  = "ctqf";
  gEnglishPitchName [k_cFlat]        = "cf";
  gEnglishPitchName [k_cSemiFlat]    = "cqf";
  gEnglishPitchName [k_cNatural]     = "c";
  gEnglishPitchName [k_cSemiSharp]   = "cqs";
  gEnglishPitchName [k_cSharp]       = "cs";
  gEnglishPitchName [k_cSesquiSharp] = "ctqs";
  gEnglishPitchName [k_cDoubleSharp] = "cx";
    
  gEnglishPitchName [k_dDoubleFlat]  = "dff";
  gEnglishPitchName [k_dSesquiFlat]  = "dtqf";
  gEnglishPitchName [k_dFlat]        = "df";
  gEnglishPitchName [k_dSemiFlat]    = "dqf";
  gEnglishPitchName [k_dNatural]     = "d";
  gEnglishPitchName [k_dSemiSharp]   = "dqs";
  gEnglishPitchName [k_dSharp]       = "ds";
  gEnglishPitchName [k_dSesquiSharp] = "dtqs";
  gEnglishPitchName [k_dDoubleSharp] = "dx";

  gEnglishPitchName [k_eDoubleFlat]  = "eff";
  gEnglishPitchName [k_eSesquiFlat]  = "etqf";
  gEnglishPitchName [k_eFlat]        = "ef";
  gEnglishPitchName [k_eSemiFlat]    = "eqf";
  gEnglishPitchName [k_eNatural]     = "e";
  gEnglishPitchName [k_eSemiSharp]   = "eqs";
  gEnglishPitchName [k_eSharp]       = "es";
  gEnglishPitchName [k_eSesquiSharp] = "etqs";
  gEnglishPitchName [k_eDoubleSharp] = "ex";
    
  gEnglishPitchName [k_fDoubleFlat]  = "fff";
  gEnglishPitchName [k_fSesquiFlat]  = "ftqf";
  gEnglishPitchName [k_fFlat]        = "ff";
  gEnglishPitchName [k_fSemiFlat]    = "fqf";
  gEnglishPitchName [k_fNatural]     = "f";
  gEnglishPitchName [k_fSemiSharp]   = "fqs";
  gEnglishPitchName [k_fSharp]       = "fs";
  gEnglishPitchName [k_fSesquiSharp] = "ftqs";
  gEnglishPitchName [k_fDoubleSharp] = "fx";
    
  gEnglishPitchName [k_gDoubleFlat]  = "gff";
  gEnglishPitchName [k_gSesquiFlat]  = "gtqf";
  gEnglishPitchName [k_gFlat]        = "gf";
  gEnglishPitchName [k_gSemiFlat]    = "gqf";
  gEnglishPitchName [k_gNatural]     = "g";
  gEnglishPitchName [k_gSemiSharp]   = "gqs";
  gEnglishPitchName [k_gSharp]       = "gs";
  gEnglishPitchName [k_gSesquiSharp] = "gtqs";
  gEnglishPitchName [k_gDoubleSharp] = "gx";

  // espanol
  gEspanolPitchName [k_NoQuarterTonesPitch]  = "-";
  gEspanolPitchName [k_Rest]                 = "r";
  
  gEspanolPitchName [k_aDoubleFlat]  = "labb";
  gEspanolPitchName [k_aSesquiFlat]  = "latcb";
  gEspanolPitchName [k_aFlat]        = "lab";
  gEspanolPitchName [k_aSemiFlat]    = "lacb";
  gEspanolPitchName [k_aNatural]     = "la";
  gEspanolPitchName [k_aSemiSharp]   = "lacs";
  gEspanolPitchName [k_aSharp]       = "las";
  gEspanolPitchName [k_aSesquiSharp] = "latcs";
  gEspanolPitchName [k_aDoubleSharp] = "lax";
    
  gEspanolPitchName [k_bDoubleFlat]  = "sibb";
  gEspanolPitchName [k_bSesquiFlat]  = "sitcb";
  gEspanolPitchName [k_bFlat]        = "sib";
  gEspanolPitchName [k_bSemiFlat]    = "sicb";
  gEspanolPitchName [k_bNatural]     = "si";
  gEspanolPitchName [k_bSemiSharp]   = "sics";
  gEspanolPitchName [k_bSharp]       = "sis";
  gEspanolPitchName [k_bSesquiSharp] = "sitcs";
  gEspanolPitchName [k_bDoubleSharp] = "six";
    
  gEspanolPitchName [k_cDoubleFlat]  = "dobb";
  gEspanolPitchName [k_cSesquiFlat]  = "dotcb";
  gEspanolPitchName [k_cFlat]        = "dob";
  gEspanolPitchName [k_cSemiFlat]    = "docb";
  gEspanolPitchName [k_cNatural]     = "do";
  gEspanolPitchName [k_cSemiSharp]   = "docs";
  gEspanolPitchName [k_cSharp]       = "dos";
  gEspanolPitchName [k_cSesquiSharp] = "dotcs";
  gEspanolPitchName [k_cDoubleSharp] = "dox";
    
  gEspanolPitchName [k_dDoubleFlat]  = "rebb";
  gEspanolPitchName [k_dSesquiFlat]  = "retcb";
  gEspanolPitchName [k_dFlat]        = "reb";
  gEspanolPitchName [k_dSemiFlat]    = "recb";
  gEspanolPitchName [k_dNatural]     = "re";
  gEspanolPitchName [k_dSemiSharp]   = "recs";
  gEspanolPitchName [k_dSharp]       = "res";
  gEspanolPitchName [k_dSesquiSharp] = "retcs";
  gEspanolPitchName [k_dDoubleSharp] = "rex";

  gEspanolPitchName [k_eDoubleFlat]  = "mibb";
  gEspanolPitchName [k_eSesquiFlat]  = "mitcb";
  gEspanolPitchName [k_eFlat]        = "mib";
  gEspanolPitchName [k_eSemiFlat]    = "micb";
  gEspanolPitchName [k_eNatural]     = "mi";
  gEspanolPitchName [k_eSemiSharp]   = "mics";
  gEspanolPitchName [k_eSharp]       = "mis";
  gEspanolPitchName [k_eSesquiSharp] = "mitcs";
  gEspanolPitchName [k_eDoubleSharp] = "mix";
    
  gEspanolPitchName [k_fDoubleFlat]  = "fabb";
  gEspanolPitchName [k_fSesquiFlat]  = "fatcb";
  gEspanolPitchName [k_fFlat]        = "fab";
  gEspanolPitchName [k_fSemiFlat]    = "facb";
  gEspanolPitchName [k_fNatural]     = "fa";
  gEspanolPitchName [k_fSemiSharp]   = "facs";
  gEspanolPitchName [k_fSharp]       = "fas";
  gEspanolPitchName [k_fSesquiSharp] = "fatcs";
  gEspanolPitchName [k_fDoubleSharp] = "fax";
    
  gEspanolPitchName [k_gDoubleFlat]  = "solbb";
  gEspanolPitchName [k_gSesquiFlat]  = "soltcb";
  gEspanolPitchName [k_gFlat]        = "solb";
  gEspanolPitchName [k_gSemiFlat]    = "solcb";
  gEspanolPitchName [k_gNatural]     = "sol";
  gEspanolPitchName [k_gSemiSharp]   = "solcs";
  gEspanolPitchName [k_gSharp]       = "sols";
  gEspanolPitchName [k_gSesquiSharp] = "soltcs";
  gEspanolPitchName [k_gDoubleSharp] = "solx";

  // francais
  gFrancaisPitchName [k_NoQuarterTonesPitch]  = "-";
  gFrancaisPitchName [k_Rest]                 = "r";
  
  gFrancaisPitchName [k_aDoubleFlat]  = "labb";
  gFrancaisPitchName [k_aSesquiFlat]  = "labtqt";
  gFrancaisPitchName [k_aFlat]        = "lab";
  gFrancaisPitchName [k_aSemiFlat]    = "labqt";
  gFrancaisPitchName [k_aNatural]     = "la";
  gFrancaisPitchName [k_aSemiSharp]   = "lasqt";
  gFrancaisPitchName [k_aSharp]       = "lad";
  gFrancaisPitchName [k_aSesquiSharp] = "lastqt";
  gFrancaisPitchName [k_aDoubleSharp] = "lass";
    
  gFrancaisPitchName [k_bDoubleFlat]  = "sibb";
  gFrancaisPitchName [k_bSesquiFlat]  = "sibtqt";
  gFrancaisPitchName [k_bFlat]        = "sib";
  gFrancaisPitchName [k_bSemiFlat]    = "sibqt";
  gFrancaisPitchName [k_bNatural]     = "si";
  gFrancaisPitchName [k_bSemiSharp]   = "sisqt";
  gFrancaisPitchName [k_bSharp]       = "sid";
  gFrancaisPitchName [k_bSesquiSharp] = "sistqt";
  gFrancaisPitchName [k_bDoubleSharp] = "siss";
    
  gFrancaisPitchName [k_cDoubleFlat]  = "dobb";
  gFrancaisPitchName [k_cSesquiFlat]  = "dobtqt";
  gFrancaisPitchName [k_cFlat]        = "dob";
  gFrancaisPitchName [k_cSemiFlat]    = "dobqt";
  gFrancaisPitchName [k_cNatural]     = "do";
  gFrancaisPitchName [k_cSemiSharp]   = "dosqt";
  gFrancaisPitchName [k_cSharp]       = "dod";
  gFrancaisPitchName [k_cSesquiSharp] = "dostqt";
  gFrancaisPitchName [k_cDoubleSharp] = "doss";
    
  gFrancaisPitchName [k_dDoubleFlat]  = "rebb";
  gFrancaisPitchName [k_dSesquiFlat]  = "rebtqt";
  gFrancaisPitchName [k_dFlat]        = "reb";
  gFrancaisPitchName [k_dSemiFlat]    = "rebqt";
  gFrancaisPitchName [k_dNatural]     = "re";
  gFrancaisPitchName [k_dSemiSharp]   = "resqt";
  gFrancaisPitchName [k_dSharp]       = "red";
  gFrancaisPitchName [k_dSesquiSharp] = "restqt";
  gFrancaisPitchName [k_dDoubleSharp] = "ress";

  gFrancaisPitchName [k_eDoubleFlat]  = "mibb";
  gFrancaisPitchName [k_eSesquiFlat]  = "mibtqt";
  gFrancaisPitchName [k_eFlat]        = "mib";
  gFrancaisPitchName [k_eSemiFlat]    = "mibqt";
  gFrancaisPitchName [k_eNatural]     = "mi";
  gFrancaisPitchName [k_eSemiSharp]   = "misqt";
  gFrancaisPitchName [k_eSharp]       = "mid";
  gFrancaisPitchName [k_eSesquiSharp] = "mistqt";
  gFrancaisPitchName [k_eDoubleSharp] = "miss";
    
  gFrancaisPitchName [k_fDoubleFlat]  = "fabb";
  gFrancaisPitchName [k_fSesquiFlat]  = "fabtqt";
  gFrancaisPitchName [k_fFlat]        = "fab";
  gFrancaisPitchName [k_fSemiFlat]    = "fabqt";
  gFrancaisPitchName [k_fNatural]     = "fa";
  gFrancaisPitchName [k_fSemiSharp]   = "fasqt";
  gFrancaisPitchName [k_fSharp]       = "fad";
  gFrancaisPitchName [k_fSesquiSharp] = "fastqt";
  gFrancaisPitchName [k_fDoubleSharp] = "fass";
    
  gFrancaisPitchName [k_gDoubleFlat]  = "solbb";
  gFrancaisPitchName [k_gSesquiFlat]  = "solbtqt";
  gFrancaisPitchName [k_gFlat]        = "solb";
  gFrancaisPitchName [k_gSemiFlat]    = "solbqt";
  gFrancaisPitchName [k_gNatural]     = "sol";
  gFrancaisPitchName [k_gSemiSharp]   = "solsqt";
  gFrancaisPitchName [k_gSharp]       = "sold";
  gFrancaisPitchName [k_gSesquiSharp] = "solstqt";
  gFrancaisPitchName [k_gDoubleSharp] = "solss";

  // italiano
  gItalianoPitchName [k_NoQuarterTonesPitch]  = "-";
  gItalianoPitchName [k_Rest]                 = "r";
  
  gItalianoPitchName [k_aDoubleFlat]  = "labb";
  gItalianoPitchName [k_aSesquiFlat]  = "labsb";
  gItalianoPitchName [k_aFlat]        = "lab";
  gItalianoPitchName [k_aSemiFlat]    = "lasb";
  gItalianoPitchName [k_aNatural]     = "la";
  gItalianoPitchName [k_aSemiSharp]   = "lasd";
  gItalianoPitchName [k_aSharp]       = "lad";
  gItalianoPitchName [k_aSesquiSharp] = "ladsd";
  gItalianoPitchName [k_aDoubleSharp] = "ladd";
    
  gItalianoPitchName [k_bDoubleFlat]  = "sibb";
  gItalianoPitchName [k_bSesquiFlat]  = "sibsb";
  gItalianoPitchName [k_bFlat]        = "sib";
  gItalianoPitchName [k_bSemiFlat]    = "sisb";
  gItalianoPitchName [k_bNatural]     = "si";
  gItalianoPitchName [k_bSemiSharp]   = "sisd";
  gItalianoPitchName [k_bSharp]       = "sid";
  gItalianoPitchName [k_bSesquiSharp] = "sidsd";
  gItalianoPitchName [k_bDoubleSharp] = "sidd";
    
  gItalianoPitchName [k_cDoubleFlat]  = "dobb";
  gItalianoPitchName [k_cSesquiFlat]  = "dobsb";
  gItalianoPitchName [k_cFlat]        = "dob";
  gItalianoPitchName [k_cSemiFlat]    = "dosb";
  gItalianoPitchName [k_cNatural]     = "do";
  gItalianoPitchName [k_cSemiSharp]   = "dosd";
  gItalianoPitchName [k_cSharp]       = "dod";
  gItalianoPitchName [k_cSesquiSharp] = "dodsd";
  gItalianoPitchName [k_cDoubleSharp] = "dodd";
    
  gItalianoPitchName [k_dDoubleFlat]  = "rebb";
  gItalianoPitchName [k_dSesquiFlat]  = "rebsb";
  gItalianoPitchName [k_dFlat]        = "reb";
  gItalianoPitchName [k_dSemiFlat]    = "resb";
  gItalianoPitchName [k_dNatural]     = "re";
  gItalianoPitchName [k_dSemiSharp]   = "resd";
  gItalianoPitchName [k_dSharp]       = "red";
  gItalianoPitchName [k_dSesquiSharp] = "redsd";
  gItalianoPitchName [k_dDoubleSharp] = "redd";

  gItalianoPitchName [k_eDoubleFlat]  = "mibb";
  gItalianoPitchName [k_eSesquiFlat]  = "mibsb";
  gItalianoPitchName [k_eFlat]        = "mib";
  gItalianoPitchName [k_eSemiFlat]    = "misb";
  gItalianoPitchName [k_eNatural]     = "mi";
  gItalianoPitchName [k_eSemiSharp]   = "misd";
  gItalianoPitchName [k_eSharp]       = "mid";
  gItalianoPitchName [k_eSesquiSharp] = "midsd";
  gItalianoPitchName [k_eDoubleSharp] = "midd";
    
  gItalianoPitchName [k_fDoubleFlat]  = "fabb";
  gItalianoPitchName [k_fSesquiFlat]  = "fabsb";
  gItalianoPitchName [k_fFlat]        = "fab";
  gItalianoPitchName [k_fSemiFlat]    = "fasb";
  gItalianoPitchName [k_fNatural]     = "fa";
  gItalianoPitchName [k_fSemiSharp]   = "fasd";
  gItalianoPitchName [k_fSharp]       = "fad";
  gItalianoPitchName [k_fSesquiSharp] = "fadsd";
  gItalianoPitchName [k_fDoubleSharp] = "fadd";
    
  gItalianoPitchName [k_gDoubleFlat]  = "solbb";
  gItalianoPitchName [k_gSesquiFlat]  = "solbsb";
  gItalianoPitchName [k_gFlat]        = "solb";
  gItalianoPitchName [k_gSemiFlat]    = "solsb";
  gItalianoPitchName [k_gNatural]     = "sol";
  gItalianoPitchName [k_gSemiSharp]   = "solsd";
  gItalianoPitchName [k_gSharp]       = "sold";
  gItalianoPitchName [k_gSesquiSharp] = "soldsd";
  gItalianoPitchName [k_gDoubleSharp] = "soldd";

  // norsk
  gNorskPitchName [k_NoQuarterTonesPitch]  = "-";
  gNorskPitchName [k_Rest]                 = "r";
  
  gNorskPitchName [k_aDoubleFlat]  = "aeses";
  gNorskPitchName [k_aSesquiFlat]  = "aSesquiFlat???";
  gNorskPitchName [k_aFlat]        = "aes";
  gNorskPitchName [k_aSemiFlat]    = "aSemiFlat???";
  gNorskPitchName [k_aNatural]     = "a";
  gNorskPitchName [k_aSemiSharp]   = "aSemiSharp???";
  gNorskPitchName [k_aSharp]       = "ais";
  gNorskPitchName [k_aSesquiSharp] = "aSesquiSharp???";
  gNorskPitchName [k_aDoubleSharp] = "aisis";
    
  gNorskPitchName [k_bDoubleFlat]  = "beses";
  gNorskPitchName [k_bSesquiFlat]  = "bSesquiFlat???";
  gNorskPitchName [k_bFlat]        = "bes";
  gNorskPitchName [k_bSemiFlat]    = "bSemiFlat???";
  gNorskPitchName [k_bNatural]     = "b";
  gNorskPitchName [k_bSemiSharp]   = "bSemiSharp???";
  gNorskPitchName [k_bSharp]       = "bis";
  gNorskPitchName [k_bSesquiSharp] = "bSesquiSharp???";
  gNorskPitchName [k_bDoubleSharp] = "bisis";
    
  gNorskPitchName [k_cDoubleFlat]  = "ceses";
  gNorskPitchName [k_cSesquiFlat]  = "cSesquiFlat???";
  gNorskPitchName [k_cFlat]        = "ces";
  gNorskPitchName [k_cSemiFlat]    = "cSemiFlat???";
  gNorskPitchName [k_cNatural]     = "c";
  gNorskPitchName [k_cSemiSharp]   = "cSemiSharp???";
  gNorskPitchName [k_cSharp]       = "cis";
  gNorskPitchName [k_cSesquiSharp] = "cSesquiSharp???";
  gNorskPitchName [k_cDoubleSharp] = "cisis";
    
  gNorskPitchName [k_dDoubleFlat]  = "deses";
  gNorskPitchName [k_dSesquiFlat]  = "dSesquiFlat???";
  gNorskPitchName [k_dFlat]        = "des";
  gNorskPitchName [k_dSemiFlat]    = "dSemiFlat???";
  gNorskPitchName [k_dNatural]     = "d";
  gNorskPitchName [k_dSemiSharp]   = "dSemiSharp???";
  gNorskPitchName [k_dSharp]       = "dis";
  gNorskPitchName [k_dSesquiSharp] = "dSesquiSharp???";
  gNorskPitchName [k_dDoubleSharp] = "disis";

  gNorskPitchName [k_eDoubleFlat]  = "eeses";
  gNorskPitchName [k_eSesquiFlat]  = "eSesquiFlat???";
  gNorskPitchName [k_eFlat]        = "ees";
  gNorskPitchName [k_eSemiFlat]    = "eSemiFlat???";
  gNorskPitchName [k_eNatural]     = "e";
  gNorskPitchName [k_eSemiSharp]   = "eSemiSharp???";
  gNorskPitchName [k_eSharp]       = "eis";
  gNorskPitchName [k_eSesquiSharp] = "eSesquiSharp???";
  gNorskPitchName [k_eDoubleSharp] = "eisis";
    
  gNorskPitchName [k_fDoubleFlat]  = "feses";
  gNorskPitchName [k_fSesquiFlat]  = "fSesquiFlat???";
  gNorskPitchName [k_fFlat]        = "fes";
  gNorskPitchName [k_fSemiFlat]    = "fSemiFlat???";
  gNorskPitchName [k_fNatural]     = "f";
  gNorskPitchName [k_fSemiSharp]   = "fSemiSharp???";
  gNorskPitchName [k_fSharp]       = "fis";
  gNorskPitchName [k_fSesquiSharp] = "fSesquiSharp???";
  gNorskPitchName [k_fDoubleSharp] = "fisis";
    
  gNorskPitchName [k_gDoubleFlat]  = "geses";
  gNorskPitchName [k_gSesquiFlat]  = "gSesquiFlat???";
  gNorskPitchName [k_gFlat]        = "ges";
  gNorskPitchName [k_gSemiFlat]    = "gSemiFlat???";
  gNorskPitchName [k_gNatural]     = "g";
  gNorskPitchName [k_gSemiSharp]   = "gSemiSharp???";
  gNorskPitchName [k_gSharp]       = "gis";
  gNorskPitchName [k_gSesquiSharp] = "gSesquiSharp???";
  gNorskPitchName [k_gDoubleSharp] = "gisis";

  // portugues
  gPortuguesPitchName [k_NoQuarterTonesPitch]  = "-";
  gPortuguesPitchName [k_Rest]                 = "r";
  
  gPortuguesPitchName [k_aDoubleFlat]  = "labb";
  gPortuguesPitchName [k_aSesquiFlat]  = "labtqt";
  gPortuguesPitchName [k_aFlat]        = "lab";
  gPortuguesPitchName [k_aSemiFlat]    = "lasb";
  gPortuguesPitchName [k_aNatural]     = "la";
  gPortuguesPitchName [k_aSemiSharp]   = "lasd";
  gPortuguesPitchName [k_aSharp]       = "lad";
  gPortuguesPitchName [k_aSesquiSharp] = "ladsd";
  gPortuguesPitchName [k_aDoubleSharp] = "ladd";
    
  gPortuguesPitchName [k_bDoubleFlat]  = "sibb";
  gPortuguesPitchName [k_bSesquiFlat]  = "sibtqt";
  gPortuguesPitchName [k_bFlat]        = "sib";
  gPortuguesPitchName [k_bSemiFlat]    = "sisb";
  gPortuguesPitchName [k_bNatural]     = "si";
  gPortuguesPitchName [k_bSemiSharp]   = "sisd";
  gPortuguesPitchName [k_bSharp]       = "sid";
  gPortuguesPitchName [k_bSesquiSharp] = "sidsd";
  gPortuguesPitchName [k_bDoubleSharp] = "sidd";
    
  gPortuguesPitchName [k_cDoubleFlat]  = "dobb";
  gPortuguesPitchName [k_cSesquiFlat]  = "dobtqt";
  gPortuguesPitchName [k_cFlat]        = "dob";
  gPortuguesPitchName [k_cSemiFlat]    = "dosb";
  gPortuguesPitchName [k_cNatural]     = "do";
  gPortuguesPitchName [k_cSemiSharp]   = "dosd";
  gPortuguesPitchName [k_cSharp]       = "dod";
  gPortuguesPitchName [k_cSesquiSharp] = "dodsd";
  gPortuguesPitchName [k_cDoubleSharp] = "dodd";
    
  gPortuguesPitchName [k_dDoubleFlat]  = "rebb";
  gPortuguesPitchName [k_dSesquiFlat]  = "rebtqt";
  gPortuguesPitchName [k_dFlat]        = "reb";
  gPortuguesPitchName [k_dSemiFlat]    = "resb";
  gPortuguesPitchName [k_dNatural]     = "re";
  gPortuguesPitchName [k_dSemiSharp]   = "resd";
  gPortuguesPitchName [k_dSharp]       = "red";
  gPortuguesPitchName [k_dSesquiSharp] = "redsd";
  gPortuguesPitchName [k_dDoubleSharp] = "redd";

  gPortuguesPitchName [k_eDoubleFlat]  = "mibb";
  gPortuguesPitchName [k_eSesquiFlat]  = "mibtqt";
  gPortuguesPitchName [k_eFlat]        = "mib";
  gPortuguesPitchName [k_eSemiFlat]    = "misb";
  gPortuguesPitchName [k_eNatural]     = "mi";
  gPortuguesPitchName [k_eSemiSharp]   = "misd";
  gPortuguesPitchName [k_eSharp]       = "mid";
  gPortuguesPitchName [k_eSesquiSharp] = "midsd";
  gPortuguesPitchName [k_eDoubleSharp] = "midd";
    
  gPortuguesPitchName [k_fDoubleFlat]  = "fabb";
  gPortuguesPitchName [k_fSesquiFlat]  = "fabtqt";
  gPortuguesPitchName [k_fFlat]        = "fab";
  gPortuguesPitchName [k_fSemiFlat]    = "fasb";
  gPortuguesPitchName [k_fNatural]     = "fa";
  gPortuguesPitchName [k_fSemiSharp]   = "fasd";
  gPortuguesPitchName [k_fSharp]       = "fad";
  gPortuguesPitchName [k_fSesquiSharp] = "fadsd";
  gPortuguesPitchName [k_fDoubleSharp] = "fadd";
    
  gPortuguesPitchName [k_gDoubleFlat]  = "solbb";
  gPortuguesPitchName [k_gSesquiFlat]  = "solbtqt";
  gPortuguesPitchName [k_gFlat]        = "solb";
  gPortuguesPitchName [k_gSemiFlat]    = "solsb";
  gPortuguesPitchName [k_gNatural]     = "sol";
  gPortuguesPitchName [k_gSemiSharp]   = "solsd";
  gPortuguesPitchName [k_gSharp]       = "sold";
  gPortuguesPitchName [k_gSesquiSharp] = "soldsd";
  gPortuguesPitchName [k_gDoubleSharp] = "soldd";

  // suomi
  gSuomiPitchName [k_NoQuarterTonesPitch]  = "-";
  gSuomiPitchName [k_Rest]                 = "r";
  
  gSuomiPitchName [k_aDoubleFlat]  = "asas";
  gSuomiPitchName [k_aSesquiFlat]  = "aSesquiFlat???";
  gSuomiPitchName [k_aFlat]        = "as";
  gSuomiPitchName [k_aSemiFlat]    = "aSemiFlat???";
  gSuomiPitchName [k_aNatural]     = "a";
  gSuomiPitchName [k_aSemiSharp]   = "aSemiSharp???";
  gSuomiPitchName [k_aSharp]       = "ais";
  gSuomiPitchName [k_aSesquiSharp] = "aSesquiSharp???";
  gSuomiPitchName [k_aDoubleSharp] = "aisis";
    
  gSuomiPitchName [k_bDoubleFlat]  = "bes";
  gSuomiPitchName [k_bSesquiFlat]  = "bSesquiFlat???";
  gSuomiPitchName [k_bFlat]        = "b";
  gSuomiPitchName [k_bSemiFlat]    = "bSemiFlat???";
  gSuomiPitchName [k_bNatural]     = "h";
  gSuomiPitchName [k_bSemiSharp]   = "bSemiSharp???";
  gSuomiPitchName [k_bSharp]       = "his";
  gSuomiPitchName [k_bSesquiSharp] = "bSesquiSharp???";
  gSuomiPitchName [k_bDoubleSharp] = "hisis";
    
  gSuomiPitchName [k_cDoubleFlat]  = "ceses";
  gSuomiPitchName [k_cSesquiFlat]  = "cSesquiFlat???";
  gSuomiPitchName [k_cFlat]        = "ces";
  gSuomiPitchName [k_cSemiFlat]    = "cSemiFlat???";
  gSuomiPitchName [k_cNatural]     = "c";
  gSuomiPitchName [k_cSemiSharp]   = "cSemiSharp???";
  gSuomiPitchName [k_cSharp]       = "cis";
  gSuomiPitchName [k_cSesquiSharp] = "cSesquiSharp???";
  gSuomiPitchName [k_cDoubleSharp] = "cisis";
    
  gSuomiPitchName [k_dDoubleFlat]  = "deses";
  gSuomiPitchName [k_dSesquiFlat]  = "dSesquiFlat???";
  gSuomiPitchName [k_dFlat]        = "des";
  gSuomiPitchName [k_dSemiFlat]    = "dSemiFlat???";
  gSuomiPitchName [k_dNatural]     = "d";
  gSuomiPitchName [k_dSemiSharp]   = "dSemiSharp???";
  gSuomiPitchName [k_dSharp]       = "dis";
  gSuomiPitchName [k_dSesquiSharp] = "dSesquiSharp???";
  gSuomiPitchName [k_dDoubleSharp] = "disis";

  gSuomiPitchName [k_eDoubleFlat]  = "eses";
  gSuomiPitchName [k_eSesquiFlat]  = "eSesquiFlat???";
  gSuomiPitchName [k_eFlat]        = "es";
  gSuomiPitchName [k_eSemiFlat]    = "eSemiFlat???";
  gSuomiPitchName [k_eNatural]     = "e";
  gSuomiPitchName [k_eSemiSharp]   = "eSemiSharp???";
  gSuomiPitchName [k_eSharp]       = "eis";
  gSuomiPitchName [k_eSesquiSharp] = "eSesquiSharp???";
  gSuomiPitchName [k_eDoubleSharp] = "eisis";
    
  gSuomiPitchName [k_fDoubleFlat]  = "feses";
  gSuomiPitchName [k_fSesquiFlat]  = "fSesquiFlat???";
  gSuomiPitchName [k_fFlat]        = "fes";
  gSuomiPitchName [k_fSemiFlat]    = "fSemiFlat???";
  gSuomiPitchName [k_fNatural]     = "f";
  gSuomiPitchName [k_fSemiSharp]   = "fSemiSharp???";
  gSuomiPitchName [k_fSharp]       = "fis";
  gSuomiPitchName [k_fSesquiSharp] = "fSesquiSharp???";
  gSuomiPitchName [k_fDoubleSharp] = "fisis";
    
  gSuomiPitchName [k_gDoubleFlat]  = "geses";
  gSuomiPitchName [k_gSesquiFlat]  = "gSesquiFlat???";
  gSuomiPitchName [k_gFlat]        = "ges";
  gSuomiPitchName [k_gSemiFlat]    = "gSemiFlat???";
  gSuomiPitchName [k_gNatural]     = "g";
  gSuomiPitchName [k_gSemiSharp]   = "gSemiSharp???";
  gSuomiPitchName [k_gSharp]       = "gis";
  gSuomiPitchName [k_gSesquiSharp] = "gSesquiSharp???";
  gSuomiPitchName [k_gDoubleSharp] = "gisis";

  // svenska
  gSvenskaPitchName [k_NoQuarterTonesPitch]  = "-";
  gSvenskaPitchName [k_Rest]                 = "r";
  
  gSvenskaPitchName [k_aDoubleFlat]  = "assess";
  gSvenskaPitchName [k_aSesquiFlat]  = "aSesquiFlat???";
  gSvenskaPitchName [k_aFlat]        = "ass";
  gSvenskaPitchName [k_aSemiFlat]    = "aSemiFlat???";
  gSvenskaPitchName [k_aNatural]     = "a";
  gSvenskaPitchName [k_aSemiSharp]   = "aSemiSharp???";
  gSvenskaPitchName [k_aSharp]       = "aiss";
  gSvenskaPitchName [k_aSesquiSharp] = "aSesquiSharp???";
  gSvenskaPitchName [k_aDoubleSharp] = "aississ";
    
  gSvenskaPitchName [k_bDoubleFlat]  = "hessess";
  gSvenskaPitchName [k_bSesquiFlat]  = "bSesquiFlat???";
  gSvenskaPitchName [k_bFlat]        = "b";
  gSvenskaPitchName [k_bSemiFlat]    = "bSemiFlat???";
  gSvenskaPitchName [k_bNatural]     = "h";
  gSvenskaPitchName [k_bSemiSharp]   = "bSemiSharp???";
  gSvenskaPitchName [k_bSharp]       = "hiss";
  gSvenskaPitchName [k_bSesquiSharp] = "bSesquiSharp???";
  gSvenskaPitchName [k_bDoubleSharp] = "hississ";
    
  gSvenskaPitchName [k_cDoubleFlat]  = "cessess";
  gSvenskaPitchName [k_cSesquiFlat]  = "cSesquiFlat???";
  gSvenskaPitchName [k_cFlat]        = "cess";
  gSvenskaPitchName [k_cSemiFlat]    = "cSemiFlat???";
  gSvenskaPitchName [k_cNatural]     = "c";
  gSvenskaPitchName [k_cSemiSharp]   = "cSemiSharp???";
  gSvenskaPitchName [k_cSharp]       = "ciss";
  gSvenskaPitchName [k_cSesquiSharp] = "cSesquiSharp???";
  gSvenskaPitchName [k_cDoubleSharp] = "cississ";
    
  gSvenskaPitchName [k_dDoubleFlat]  = "dessess";
  gSvenskaPitchName [k_dSesquiFlat]  = "dSesquiFlat???";
  gSvenskaPitchName [k_dFlat]        = "dess";
  gSvenskaPitchName [k_dSemiFlat]    = "dSemiFlat???";
  gSvenskaPitchName [k_dNatural]     = "d";
  gSvenskaPitchName [k_dSemiSharp]   = "dSemiSharp???";
  gSvenskaPitchName [k_dSharp]       = "diss";
  gSvenskaPitchName [k_dSesquiSharp] = "dSesquiSharp???";
  gSvenskaPitchName [k_dDoubleSharp] = "dississ";

  gSvenskaPitchName [k_eDoubleFlat]  = "essess";
  gSvenskaPitchName [k_eSesquiFlat]  = "eSesquiFlat???";
  gSvenskaPitchName [k_eFlat]        = "ess";
  gSvenskaPitchName [k_eSemiFlat]    = "eSemiFlat???";
  gSvenskaPitchName [k_eNatural]     = "e";
  gSvenskaPitchName [k_eSemiSharp]   = "eSemiSharp???";
  gSvenskaPitchName [k_eSharp]       = "eiss";
  gSvenskaPitchName [k_eSesquiSharp] = "eSesquiSharp???";
  gSvenskaPitchName [k_eDoubleSharp] = "eississ";
    
  gSvenskaPitchName [k_fDoubleFlat]  = "fessess";
  gSvenskaPitchName [k_fSesquiFlat]  = "fSesquiFlat???";
  gSvenskaPitchName [k_fFlat]        = "fess";
  gSvenskaPitchName [k_fSemiFlat]    = "fSemiFlat???";
  gSvenskaPitchName [k_fNatural]     = "f";
  gSvenskaPitchName [k_fSemiSharp]   = "fSemiSharp???";
  gSvenskaPitchName [k_fSharp]       = "fiss";
  gSvenskaPitchName [k_fSesquiSharp] = "fSesquiSharp???";
  gSvenskaPitchName [k_fDoubleSharp] = "fississ";
    
  gSvenskaPitchName [k_gDoubleFlat]  = "gessess";
  gSvenskaPitchName [k_gSesquiFlat]  = "gSesquiFlat???";
  gSvenskaPitchName [k_gFlat]        = "gess";
  gSvenskaPitchName [k_gSemiFlat]    = "gSemiFlat???";
  gSvenskaPitchName [k_gNatural]     = "g";
  gSvenskaPitchName [k_gSemiSharp]   = "gSemiSharp???";
  gSvenskaPitchName [k_gSharp]       = "giss";
  gSvenskaPitchName [k_gSesquiSharp] = "gSesquiSharp???";
  gSvenskaPitchName [k_gDoubleSharp] = "gississ";

  // vlaams
  gVlaamsPitchName [k_NoQuarterTonesPitch]  = "none";
  gVlaamsPitchName [k_Rest]                 = "r";
  
  gVlaamsPitchName [k_aDoubleFlat]  = "labb";
  gVlaamsPitchName [k_aSesquiFlat]  = "laSesquiFlat???";
  gVlaamsPitchName [k_aFlat]        = "lab";
  gVlaamsPitchName [k_aSemiFlat]    = "laSemiFlat???";
  gVlaamsPitchName [k_aNatural]     = "la";
  gVlaamsPitchName [k_aSemiSharp]   = "laSemiSharp???";
  gVlaamsPitchName [k_aSharp]       = "lak";
  gVlaamsPitchName [k_aSesquiSharp] = "laSesquiSharp???";
  gVlaamsPitchName [k_aDoubleSharp] = "lakk";
    
  gVlaamsPitchName [k_bDoubleFlat]  = "sibb";
  gVlaamsPitchName [k_bSesquiFlat]  = "siSesquiFlat???";
  gVlaamsPitchName [k_bFlat]        = "sib";
  gVlaamsPitchName [k_bSemiFlat]    = "siSemiFlat???";
  gVlaamsPitchName [k_bNatural]     = "si";
  gVlaamsPitchName [k_bSemiSharp]   = "siSemiSharp???";
  gVlaamsPitchName [k_bSharp]       = "sik";
  gVlaamsPitchName [k_bSesquiSharp] = "siSesquiSharp???";
  gVlaamsPitchName [k_bDoubleSharp] = "sikk";
    
  gVlaamsPitchName [k_cDoubleFlat]  = "dobb";
  gVlaamsPitchName [k_cSesquiFlat]  = "doSesquiFlat???";
  gVlaamsPitchName [k_cFlat]        = "dob";
  gVlaamsPitchName [k_cSemiFlat]    = "doSemiFlat???";
  gVlaamsPitchName [k_cNatural]     = "do";
  gVlaamsPitchName [k_cSemiSharp]   = "doSemiSharp???";
  gVlaamsPitchName [k_cSharp]       = "dok";
  gVlaamsPitchName [k_cSesquiSharp] = "doSesquiSharp???";
  gVlaamsPitchName [k_cDoubleSharp] = "dokk";
    
  gVlaamsPitchName [k_dDoubleFlat]  = "rebb";
  gVlaamsPitchName [k_dSesquiFlat]  = "reSesquiFlat???";
  gVlaamsPitchName [k_dFlat]        = "reb";
  gVlaamsPitchName [k_dSemiFlat]    = "reSemiFlat???";
  gVlaamsPitchName [k_dNatural]     = "re";
  gVlaamsPitchName [k_dSemiSharp]   = "reSemiSharp???";
  gVlaamsPitchName [k_dSharp]       = "rek";
  gVlaamsPitchName [k_dSesquiSharp] = "reSesquiSharp???";
  gVlaamsPitchName [k_dDoubleSharp] = "rekk";

  gVlaamsPitchName [k_eDoubleFlat]  = "mibb";
  gVlaamsPitchName [k_eSesquiFlat]  = "miSesquiFlat???";
  gVlaamsPitchName [k_eFlat]        = "mib";
  gVlaamsPitchName [k_eSemiFlat]    = "miSemiFlat???";
  gVlaamsPitchName [k_eNatural]     = "mi";
  gVlaamsPitchName [k_eSemiSharp]   = "miSemiSharp???";
  gVlaamsPitchName [k_eSharp]       = "mik";
  gVlaamsPitchName [k_eSesquiSharp] = "miSesquiSharp???";
  gVlaamsPitchName [k_eDoubleSharp] = "mikk";
    
  gVlaamsPitchName [k_fDoubleFlat]  = "fabb";
  gVlaamsPitchName [k_fSesquiFlat]  = "faSesquiFlat???";
  gVlaamsPitchName [k_fFlat]        = "fab";
  gVlaamsPitchName [k_fSemiFlat]    = "faSemiFlat???";
  gVlaamsPitchName [k_fNatural]     = "fa";
  gVlaamsPitchName [k_fSemiSharp]   = "faSemiSharp???";
  gVlaamsPitchName [k_fSharp]       = "fak";
  gVlaamsPitchName [k_fSesquiSharp] = "faSesquiSharp???";
  gVlaamsPitchName [k_fDoubleSharp] = "fakk";
    
  gVlaamsPitchName [k_gDoubleFlat]  = "solbb";
  gVlaamsPitchName [k_gSesquiFlat]  = "solSesquiFlat???";
  gVlaamsPitchName [k_gFlat]        = "solb";
  gVlaamsPitchName [k_gSemiFlat]    = "solSemiFlat???";
  gVlaamsPitchName [k_gNatural]     = "sol";
  gVlaamsPitchName [k_gSemiSharp]   = "solSemiSharp???";
  gVlaamsPitchName [k_gSharp]       = "solk";
  gVlaamsPitchName [k_gSesquiSharp] = "solSesquiSharp???";
  gVlaamsPitchName [k_gDoubleSharp] = "solkk";
}

string msrDiatonicPitchAsString (
  msrDiatonicPitch diatonicPitch)
{
  string result;

  switch (diatonicPitch) {
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
    case k_NoDiatonicPitch:
      result = "k_NoDiatonicPitch";
      break;
  } // switch
  return result;
}

msrDiatonicPitch msrDiatonicPitchFromString (
  char diatonicNoteName)
{
  msrDiatonicPitch result;
  
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

string msrDiatonicPitchAsString (
  msrQuarterTonesPitchesLanguage language,
  msrDiatonicPitch              diatonicPitch)
{
  string result;
  
  switch (diatonicPitch) {
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
    case k_NoDiatonicPitch:
      result = "k_NoDiatonicPitch";
      break;
  } // switch

  return result;
}

string msrQuarterTonesPitchesLanguageAsString (
  msrQuarterTonesPitchesLanguage language)
{
  string result;
  
  switch (language) {
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

msrAlteration msrAlterationFromMusicXMLAlter (
  float alter)
{

/*
  The alter element represents chromatic alteration
  in number of semitones (e.g., -1 for flat, 1 for sharp).
  Decimal values like 0.5 (quarter tone sharp) are used for microtones:

  semi-sharp semi-flat sesqui-sharp sesqui-flat double-sharp double-flat
      +0.5      -0.5        +1.5       -1.5         +2.0        -2.0
*/

  msrAlteration result;
  
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

string msrAlterationAsString (
  msrAlteration alteration)
{
  string result;
  
  switch (alteration) {
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
    case k_NoAlteration:
      result = "alteration???";
      break;
  } // switch

  return result;
}

void setDiatonicPitchAndAlteration (
  msrQuarterTonesPitch quarterTonesPitch,
  msrDiatonicPitch&    diatonicPitch,
  msrAlteration&       alteration)
{
  switch (quarterTonesPitch) {
    case k_aDoubleFlat:
      diatonicPitch = kA;
      alteration    = kDoubleFlat;
      break;
    case k_aSesquiFlat:
      diatonicPitch = kA;
      alteration    = kSesquiFlat;
      break;
    case k_aFlat:
      diatonicPitch = kA;
      alteration    = kFlat;
      break;
    case k_aSemiFlat:
      diatonicPitch = kA;
      alteration    = kSemiFlat;
      break;
    case k_aNatural:
      diatonicPitch = kA;
      alteration    = kNatural;
      break;
    case k_aSemiSharp:
      diatonicPitch = kA;
      alteration    = kSemiSharp;
      break;
    case k_aSharp:
      diatonicPitch = kA;
      alteration    = kSharp;
      break;
    case k_aSesquiSharp:
      diatonicPitch = kA;
      alteration    = kSesquiSharp;
      break;
    case k_aDoubleSharp:
      diatonicPitch = kA;
      alteration    = kDoubleSharp;
      break;

    case k_bDoubleFlat:
      diatonicPitch = kB;
      alteration    = kDoubleFlat;
      break;
    case k_bSesquiFlat:
      diatonicPitch = kB;
      alteration    = kSesquiFlat;
      break;
    case k_bFlat:
      diatonicPitch = kB;
      alteration    = kFlat;
      break;
    case k_bSemiFlat:
      diatonicPitch = kB;
      alteration    = kSemiFlat;
      break;
    case k_bNatural:
      diatonicPitch = kB;
      alteration    = kNatural;
      break;
    case k_bSemiSharp:
      diatonicPitch = kB;
      alteration    = kSemiSharp;
      break;
    case k_bSharp:
      diatonicPitch = kB;
      alteration    = kSharp;
      break;
    case k_bSesquiSharp:
      diatonicPitch = kB;
      alteration    = kSesquiSharp;
      break;
    case k_bDoubleSharp:
      diatonicPitch = kB;
      alteration    = kDoubleSharp;
      break;

    case k_cDoubleFlat:
      diatonicPitch = kC;
      alteration    = kDoubleFlat;
      break;
    case k_cSesquiFlat:
      diatonicPitch = kC;
      alteration    = kSesquiFlat;
      break;
    case k_cFlat:
      diatonicPitch = kC;
      alteration    = kFlat;
      break;
    case k_cSemiFlat:
      diatonicPitch = kC;
      alteration    = kSemiFlat;
      break;
    case k_cNatural:
      diatonicPitch = kC;
      alteration    = kNatural;
      break;
    case k_cSemiSharp:
      diatonicPitch = kC;
      alteration    = kSemiSharp;
      break;
    case k_cSharp:
      diatonicPitch = kC;
      alteration    = kSharp;
      break;
    case k_cSesquiSharp:
      diatonicPitch = kC;
      alteration    = kSesquiSharp;
      break;
    case k_cDoubleSharp:
      diatonicPitch = kC;
      alteration    = kDoubleSharp;
      break;

    case k_dDoubleFlat:
      diatonicPitch = kD;
      alteration    = kDoubleFlat;
      break;
    case k_dSesquiFlat:
      diatonicPitch = kD;
      alteration    = kSesquiFlat;
      break;
    case k_dFlat:
      diatonicPitch = kD;
      alteration    = kFlat;
      break;
    case k_dSemiFlat:
      diatonicPitch = kD;
      alteration    = kSemiFlat;
      break;
    case k_dNatural:
      diatonicPitch = kD;
      alteration    = kNatural;
      break;
    case k_dSemiSharp:
      diatonicPitch = kD;
      alteration    = kSemiSharp;
      break;
    case k_dSharp:
      diatonicPitch = kD;
      alteration    = kSharp;
      break;
    case k_dSesquiSharp:
      diatonicPitch = kD;
      alteration    = kSesquiSharp;
      break;
    case k_dDoubleSharp:
      diatonicPitch = kD;
      alteration    = kDoubleSharp;
      break;

    case k_eDoubleFlat:
      diatonicPitch = kE;
      alteration    = kDoubleFlat;
      break;
    case k_eSesquiFlat:
      diatonicPitch = kE;
      alteration    = kSesquiFlat;
      break;
    case k_eFlat:
      diatonicPitch = kE;
      alteration    = kFlat;
      break;
    case k_eSemiFlat:
      diatonicPitch = kE;
      alteration    = kSemiFlat;
      break;
    case k_eNatural:
      diatonicPitch = kE;
      alteration    = kNatural;
      break;
    case k_eSemiSharp:
      diatonicPitch = kE;
      alteration    = kSemiSharp;
      break;
    case k_eSharp:
      diatonicPitch = kE;
      alteration    = kSharp;
      break;
    case k_eSesquiSharp:
      diatonicPitch = kE;
      alteration    = kSesquiSharp;
      break;
    case k_eDoubleSharp:
      diatonicPitch = kE;
      alteration    = kDoubleSharp;
      break;

    case k_fDoubleFlat:
      diatonicPitch = kF;
      alteration    = kDoubleFlat;
      break;
    case k_fSesquiFlat:
      diatonicPitch = kF;
      alteration    = kSesquiFlat;
      break;
    case k_fFlat:
      diatonicPitch = kF;
      alteration    = kFlat;
      break;
    case k_fSemiFlat:
      diatonicPitch = kF;
      alteration    = kSemiFlat;
      break;
    case k_fNatural:
      diatonicPitch = kF;
      alteration    = kNatural;
      break;
    case k_fSemiSharp:
      diatonicPitch = kF;
      alteration    = kSemiSharp;
      break;
    case k_fSharp:
      diatonicPitch = kF;
      alteration    = kSharp;
      break;
    case k_fSesquiSharp:
      diatonicPitch = kF;
      alteration    = kSesquiSharp;
      break;
    case k_fDoubleSharp:
      diatonicPitch = kF;
      alteration    = kDoubleSharp;
      break;

    case k_gDoubleFlat:
      diatonicPitch = kG;
      alteration    = kDoubleFlat;
      break;
    case k_gSesquiFlat:
      diatonicPitch = kG;
      alteration    = kSesquiFlat;
      break;
    case k_gFlat:
      diatonicPitch = kG;
      break;
    case k_gSemiFlat:
      diatonicPitch = kG;
      alteration    = kSemiFlat;
      break;
    case k_gNatural:
      diatonicPitch = kG;
      alteration    = kNatural;
      break;
    case k_gSemiSharp:
      diatonicPitch = kG;
      alteration    = kSemiSharp;
      break;
    case k_gSharp:
      diatonicPitch = kG;
      alteration    = kSharp;
      break;
    case k_gSesquiSharp:
      diatonicPitch = kG;
      alteration    = kSesquiSharp;
      break;
    case k_gDoubleSharp:
      diatonicPitch = kG;
      alteration    = kDoubleSharp;
      break;
      
    case k_Rest:
      diatonicPitch = kA; // any value would fit JMI
      alteration    = k_NoAlteration;
      
      break;
    case k_NoQuarterTonesPitch:
      diatonicPitch = kA; // any value would fit
      alteration    = k_NoAlteration;
      break;
  } // switch
}

msrQuarterTonesPitch quarterTonesPitchFromDiatonicPitchAndAlteration (
  int              inputLineNumber,
  msrDiatonicPitch diatonicPitch,
  msrAlteration    alteration)
{
  msrQuarterTonesPitch result;
  
  switch (diatonicPitch) {
    case kA:
      switch (alteration) {
        case kDoubleFlat:
          result = k_aDoubleFlat;
          break;
        case kSesquiFlat:
          result = k_aSesquiFlat;
          break;
        case kFlat:
          result = k_aFlat;
          break;
        case kSemiFlat:
          result = k_aSemiFlat;
          break;
        case kNatural:
          result = k_aNatural;
          break;
        case kSemiSharp:
          result = k_aSemiSharp;
          break;
        case kSharp:
          result = k_aSharp;
          break;
        case kSesquiSharp:
          result = k_aSesquiSharp;
          break;
        case kDoubleSharp:
          result = k_aDoubleSharp;
          break;
        case k_NoAlteration:
          {
            stringstream s;

            s <<
              "'A' alteration has not been set"
              ", line = " << inputLineNumber;

            basicInternalError (
              inputLineNumber,
              s.str());
          }
          break;
      } // switch
      break;
      
    case kB:
      switch (alteration) {
        case kDoubleFlat:
          result = k_bDoubleFlat;
          break;
        case kSesquiFlat:
          result = k_bSesquiFlat;
          break;
        case kFlat:
          result = k_bFlat;
          break;
        case kSemiFlat:
          result = k_bSemiFlat;
          break;
        case kNatural:
          result = k_bNatural;
          break;
        case kSemiSharp:
          result = k_bSemiSharp;
          break;
        case kSharp:
          result = k_bSharp;
          break;
        case kSesquiSharp:
          result = k_bSesquiSharp;
          break;
        case kDoubleSharp:
          result = k_bDoubleSharp;
          break;
        case k_NoAlteration:
          {
            stringstream s;

            s <<
              "'B' alteration has not been set"
              ", line = " << inputLineNumber;

            basicInternalError (
              inputLineNumber,
              s.str());
          }
          break;
      } // switch
      break;
      
      break;
      
    case kC:
      switch (alteration) {
        case kDoubleFlat:
          result = k_cDoubleFlat;
          break;
        case kSesquiFlat:
          result = k_cSesquiFlat;
          break;
        case kFlat:
          result = k_cFlat;
          break;
        case kSemiFlat:
          result = k_cSemiFlat;
          break;
        case kNatural:
          result = k_cNatural;
          break;
        case kSemiSharp:
          result = k_cSemiSharp;
          break;
        case kSharp:
          result = k_cSharp;
          break;
        case kSesquiSharp:
          result = k_cSesquiSharp;
          break;
        case kDoubleSharp:
          result = k_cDoubleSharp;
          break;
        case k_NoAlteration:
          {
            stringstream s;

            s <<
              "'C' alteration has not been set"
              ", line = " << inputLineNumber;

            basicInternalError (
              inputLineNumber,
              s.str());
          }
          break;
      } // switch
      break;
      
    case kD:
      switch (alteration) {
        case kDoubleFlat:
          result = k_dDoubleFlat;
          break;
        case kSesquiFlat:
          result = k_dSesquiFlat;
          break;
        case kFlat:
          result = k_dFlat;
          break;
        case kSemiFlat:
          result = k_dSemiFlat;
          break;
        case kNatural:
          result = k_dNatural;
          break;
        case kSemiSharp:
          result = k_dSemiSharp;
          break;
        case kSharp:
          result = k_dSharp;
          break;
        case kSesquiSharp:
          result = k_dSesquiSharp;
          break;
        case kDoubleSharp:
          result = k_dDoubleSharp;
          break;
        case k_NoAlteration:
          {
            stringstream s;

            s <<
              "'D' alteration has not been set"
              ", line = " << inputLineNumber;

            basicInternalError (
              inputLineNumber,
              s.str());
          }
          break;
      } // switch
      break;
      
    case kE:
      switch (alteration) {
        case kDoubleFlat:
          result = k_eDoubleFlat;
          break;
        case kSesquiFlat:
          result = k_eSesquiFlat;
          break;
        case kFlat:
          result = k_eFlat;
          break;
        case kSemiFlat:
          result = k_eSemiFlat;
          break;
        case kNatural:
          result = k_eNatural;
          break;
        case kSemiSharp:
          result = k_eSemiSharp;
          break;
        case kSharp:
          result = k_eSharp;
          break;
        case kSesquiSharp:
          result = k_eSesquiSharp;
          break;
        case kDoubleSharp:
          result = k_eDoubleSharp;
          break;
        case k_NoAlteration:
          {
            stringstream s;

            s <<
              "'E' alteration has not been set"
              ", line = " << inputLineNumber;

            basicInternalError (
              inputLineNumber,
              s.str());
          }
          break;
      } // switch
      break;
      
    case kF:
      switch (alteration) {
        case kDoubleFlat:
          result = k_fDoubleFlat;
          break;
        case kSesquiFlat:
          result = k_fSesquiFlat;
          break;
        case kFlat:
          result = k_fFlat;
          break;
        case kSemiFlat:
          result = k_fSemiFlat;
          break;
        case kNatural:
          result = k_fNatural;
          break;
        case kSemiSharp:
          result = k_fSemiSharp;
          break;
        case kSharp:
          result = k_fSharp;
          break;
        case kSesquiSharp:
          result = k_fSesquiSharp;
          break;
        case kDoubleSharp:
          result = k_fDoubleSharp;
          break;
        case k_NoAlteration:
          {
            stringstream s;

            s <<
              "'F' alteration has not been set"
              ", line = " << inputLineNumber;

            basicInternalError (
              inputLineNumber,
              s.str());
          }
          break;
      } // switch
      break;
      
    case kG:
      switch (alteration) {
        case kDoubleFlat:
          result = k_gDoubleFlat;
          break;
        case kSesquiFlat:
          result = k_gSesquiFlat;
          break;
        case kFlat:
          result = k_gFlat;
          break;
        case kSemiFlat:
          result = k_gSemiFlat;
          break;
        case kNatural:
          result = k_gNatural;
          break;
        case kSemiSharp:
          result = k_gSemiSharp;
          break;
        case kSharp:
          result = k_gSharp;
          break;
        case kSesquiSharp:
          result = k_gSesquiSharp;
          break;
        case kDoubleSharp:
          result = k_gDoubleSharp;
          break;
        case k_NoAlteration:
          {
            stringstream s;

            s <<
              "'G' alteration has not been set"
              ", line = " << inputLineNumber;

            basicInternalError (
              inputLineNumber,
              s.str());
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

        basicInternalError (
          inputLineNumber,
          s.str());
          */
      }
      break;    
  } // switch

  return result;
}

msrDiatonicPitch msrDiatonicPitchFromQuarterTonesPitch (
  int                  inputLineNumber,
  msrQuarterTonesPitch quarterTonesPitch)
{
  msrDiatonicPitch result;
  
  switch (quarterTonesPitch) {
    case k_aDoubleFlat:
    case k_aSesquiFlat:
    case k_aFlat:
    case k_aSemiFlat:
    case k_aNatural:
    case k_aSemiSharp:
    case k_aSharp:
    case k_aSesquiSharp:
    case k_aDoubleSharp:
      result = kA;
      break;

    case k_bDoubleFlat:
    case k_bSesquiFlat:
    case k_bFlat:
    case k_bSemiFlat:
    case k_bNatural:
    case k_bSemiSharp:
    case k_bSharp:
    case k_bSesquiSharp:
    case k_bDoubleSharp:
      result = kB;
      break;

    case k_cDoubleFlat:
    case k_cSesquiFlat:
    case k_cFlat:
    case k_cSemiFlat:
    case k_cNatural:
    case k_cSemiSharp:
    case k_cSharp:
    case k_cSesquiSharp:
    case k_cDoubleSharp:
      result = kC;
      break;

    case k_dDoubleFlat:
    case k_dSesquiFlat:
    case k_dFlat:
    case k_dSemiFlat:
    case k_dNatural:
    case k_dSemiSharp:
    case k_dSharp:
    case k_dSesquiSharp:
    case k_dDoubleSharp:
      result = kD;
      break;

    case k_eDoubleFlat:
    case k_eSesquiFlat:
    case k_eFlat:
    case k_eSemiFlat:
    case k_eNatural:
    case k_eSemiSharp:
    case k_eSharp:
    case k_eSesquiSharp:
    case k_eDoubleSharp:
      result = kE;
      break;

    case k_fDoubleFlat:
    case k_fSesquiFlat:
    case k_fFlat:
    case k_fSemiFlat:
    case k_fNatural:
    case k_fSemiSharp:
    case k_fSharp:
    case k_fSesquiSharp:
    case k_fDoubleSharp:
      result = kF;
      break;

    case k_gDoubleFlat:
    case k_gSesquiFlat:
    case k_gFlat:
    case k_gSemiFlat:
    case k_gNatural:
    case k_gSemiSharp:
    case k_gSharp:
    case k_gSesquiSharp:
    case k_gDoubleSharp:
      result = kG;
      break;

    case k_Rest:
      {
        stringstream s;

        s <<
          "cannot get the diatonic pitch of a rest"
          ", line = " << inputLineNumber;

        basicInternalError (
          inputLineNumber,
          s.str());
      }
      
    case k_NoQuarterTonesPitch:
      {
        result = k_NoDiatonicPitch;
        
        /* JMI
        stringstream s;

        s <<
          "cannot get the diatonic pitch of a k_NoQuarterTonesPitch"
          ", line = " << inputLineNumber;

        basicInternalError (
          inputLineNumber,
          s.str());
          */
      }
  } // switch

  return result;
}

string msrQuarterTonesPitchAsString (
  msrQuarterTonesPitchesLanguage language,
  msrQuarterTonesPitch           quarterTonesPitch)
{
   string result;
  
  switch (language) {
    case kNederlands:
      result = gNederlandsPitchName [quarterTonesPitch];
      break;
    case kCatalan:
      result = gCatalanPitchName [quarterTonesPitch];
      break;
    case kDeutsch:
      result = gDeutschPitchName [quarterTonesPitch];
      break;
    case kEnglish:
      result = gEnglishPitchName [quarterTonesPitch];
      break;
    case kEspanol:
      result = gEspanolPitchName [quarterTonesPitch];
      break;
    case kFrancais:
      result = gFrancaisPitchName [quarterTonesPitch];
      break;
    case kItaliano:
      result = gItalianoPitchName [quarterTonesPitch];
      break;
    case kNorsk:
      result = gNorskPitchName [quarterTonesPitch];
      break;
    case kPortugues:
      result = gPortuguesPitchName [quarterTonesPitch];
      break;
    case kSuomi:
      result = gSuomiPitchName [quarterTonesPitch];
      break;
    case kSvenska:
      result = gSvenskaPitchName [quarterTonesPitch];
      break;
    case kVlaams:
      result = gVlaamsPitchName [quarterTonesPitch];
      break;
  } // switch

  return result;
}

string existingQuarterTonesPitchesLanguages ()
{
  stringstream s;
  
  map<string, msrQuarterTonesPitchesLanguage>::const_iterator
    iBegin = gQuarterTonesPitchesLanguagesMap.begin(),
    iEnd   = gQuarterTonesPitchesLanguagesMap.end(),
    i      = iBegin;
  for ( ; ; ) {
    s << (*i).first;
    if (++i == iEnd) break;
    s << " ";
  } // for

  return s.str();
}

// durations
//______________________________________________________________________________
rational msrDurationAsWholeNotes (msrDuration duration)
{
  rational result;

  switch (duration) {
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
    case k_NoDuration:
      result = rational (0, 1);
      break;
  } // switch

  return result;
}

string msrDurationAsString (msrDuration duration)
{
  string result;

  switch (duration) {
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
    case k_NoDuration:
      result = "k_NoDuration???";
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
  cout<<
    "--> wholeNotes: " << wholeNotes <<
    ", line " << inputLineNumber <<
    endl;
#endif

  wholeNotes.rationalise ();

#ifdef DEBUG_WHOLE_NOTES
  cerr <<
    "--> wholeNotes rationalised: " << wholeNotes <<
    endl;
#endif

  int
    numerator    = wholeNotes.getNumerator (),
    denominator  = wholeNotes.getDenominator ();

#ifdef DEBUG_WHOLE_NOTES
  cerr <<
    "--> numerator: " << numerator <<
    endl <<
    "--> denominator: " << denominator <<
    endl <<
    endl;
#endif

  // sanity check
  basicAssert (
    numerator != 0,
    "numerator is 0");
    
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
          case 8:
            s << "maxima";
            break;
          case 12:
            s << "maxima";
            numberOfDots += 1;
            break;
          default:
            s <<
              numerator << "/" << denominator <<
              " whole notes cannot be represented as an MSR string";
  
            basicInternalError (
              inputLineNumber,
              s.str());
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
    s.str();
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

//______________________________________________________________________________
S_msrChordItem msrChordItem::create (
  int         inputLineNumber,
  int         chordItemNumber,
  msrInterval chordItemInterval)
{
  msrChordItem* o =
    new msrChordItem (
      inputLineNumber,
      chordItemNumber,
      chordItemInterval);
  assert(o!=0);

  return o;
}

msrChordItem::msrChordItem (
  int         inputLineNumber,
  int         chordItemNumber,
  msrInterval chordItemInterval)
  // JMI  : msrElement (inputLineNumber)
{
  fChordItemNumber   = chordItemNumber;
  fChordItemInterval = chordItemInterval;

// JMI  if (gGeneralOptions->fTraceHarmonies) {
    cerr << idtr <<
      "==> Creating chord item '" <<
      chordItemAsString () <<
      "'" <<
      endl;
 // }
}

msrChordItem::~msrChordItem()
{}

/* JMI
S_msrChordItem msrChordItem::createHarmonyNewbornClone (
  S_msrPart containingPart)
{
  if (gGeneralOptions->fTraceHarmonies) {
    cerr << idtr <<
      "==> Creating a newborn clone of harmony degree '" <<
      harmonyKindAsShortString () <<
      "'" <<
      endl;
  }

  // sanity check
  basicAssert(
    containingPart != 0,
    "containingPart is null");
    
  S_msrChordItem
    newbornClone =
      msrChordItem::create (
        fInputLineNumber,
        fChordItemValue,
        fChordItemAlteration,
        fChordItemTypeKind);
        
  return newbornClone;
}

S_msrChordItem msrChordItem::createHarmonyDeepCopy (
  S_msrPart containingPart)
{
  if (gGeneralOptions->fTraceHarmonies) {
    cerr << idtr <<
      "==> Creating a deep copy of harmony degree '" <<
      harmonyKindAsShortString () <<
      "'" <<
      endl;
  }

  // sanity check
  basicAssert(
    containingPart != 0,
    "containingPart is null");
    
  S_msrChordItem
    harmonyDeepCopy =
      msrChordItem::create (
        fInputLineNumber,
        fChordItemValue,
        fChordItemAlteration,
        fChordItemTypeKind);
        
  return harmonyDeepCopy;
}
*/

string msrChordItem::chordItemAsString () const
{
  stringstream s;

  s <<
    "ChordItem" <<
    " " << fChordItemNumber <<
    ": " << msrIntervalAsString (fChordItemInterval);

  return s.str();
}

/* JMI
void msrChordItem::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrChordItem::acceptIn()" <<
      endl;
      
  if (visitor<S_msrChordItem>*
    p =
      dynamic_cast<visitor<S_msrChordItem>*> (v)) {
        S_msrChordItem elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrChordItem::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrChordItem::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrChordItem::acceptOut()" <<
      endl;

  if (visitor<S_msrChordItem>*
    p =
      dynamic_cast<visitor<S_msrChordItem>*> (v)) {
        S_msrChordItem elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
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
    ", interval: " << msrIntervalAsString (fChordItemInterval) <<
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

// JMI  if (gGeneralOptions->fTraceHarmonies) {
    cerr << idtr <<
      "==> Creating chord intervals '" <<
      chordIntervalsAsString () <<
      "'" <<
      endl;
//  }

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
        cerr <<
          endl <<
          idtr << "*****************" <<
          endl <<
          idtr << "msrChordIntervals::initializeChordIntervals(), this =" <<
          endl <<
          idtr;
        print (cerr);
        cerr <<
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

msrChordIntervals::~msrChordIntervals()
{}

/* JMI
S_msrChordIntervals msrChordIntervals::createHarmonyNewbornClone (
  S_msrPart containingPart)
{
  if (gGeneralOptions->fTraceHarmonies) {
    cerr << idtr <<
      "==> Creating a newborn clone of harmony degree '" <<
      harmonyKindAsShortString () <<
      "'" <<
      endl;
  }

  // sanity check
  basicAssert(
    containingPart != 0,
    "containingPart is null");
    
  S_msrChordIntervals
    newbornClone =
      msrChordIntervals::create (
        fInputLineNumber,
        fChordIntervalsValue,
        fChordIntervalsAlteration,
        fChordIntervalsTypeKind);
        
  return newbornClone;
}

S_msrChordIntervals msrChordIntervals::createHarmonyDeepCopy (
  S_msrPart containingPart)
{
  if (gGeneralOptions->fTraceHarmonies) {
    cerr << idtr <<
      "==> Creating a deep copy of harmony degree '" <<
      harmonyKindAsShortString () <<
      "'" <<
      endl;
  }

  // sanity check
  basicAssert(
    containingPart != 0,
    "containingPart is null");
    
  S_msrChordIntervals
    harmonyDeepCopy =
      msrChordIntervals::create (
        fInputLineNumber,
        fChordIntervalsValue,
        fChordIntervalsAlteration,
        fChordIntervalsTypeKind);
        
  return harmonyDeepCopy;
}
*/

string msrChordIntervals::chordIntervalsAsString () const
{
  stringstream s;

  s <<
    "ChordIntervals" <<
    ", " <<
    harmonyKindAsString (fChordIntervalsHarmonyKind) <<
    ", " <<
    singularOrPlural (
      fChordIntervalsItems.size (), "item", "items");

  return s.str();
}

/* JMI
void msrChordIntervals::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrChordIntervals::acceptIn()" <<
      endl;
      
  if (visitor<S_msrChordIntervals>*
    p =
      dynamic_cast<visitor<S_msrChordIntervals>*> (v)) {
        S_msrChordIntervals elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrChordIntervals::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrChordIntervals::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrChordIntervals::acceptOut()" <<
      endl;

  if (visitor<S_msrChordIntervals>*
    p =
      dynamic_cast<visitor<S_msrChordIntervals>*> (v)) {
        S_msrChordIntervals elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
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
  os << idtr <<
    "ChordIntervals" <<
    ", harmonyKind: " <<
    harmonyKindAsString (fChordIntervalsHarmonyKind) <<
    ", " <<
    singularOrPlural (
      fChordIntervalsItems.size (), "item", "items") <<
  /* JMI
    ", line: " << fInputLineNumber <<
    */
    endl;

  if (fChordIntervalsItems.size ()) {
    for (unsigned int i = 0; i < fChordIntervalsItems.size (); i++) {
      S_msrChordItem
        chordItem =
          fChordIntervalsItems [i];

      cerr << idtr <<
        chordItem->chordItemAsString () <<
        endl;
    } // for
  }
  else {
    cerr << idtr <<
      "no intervals" <<
      endl;
  }
}


}
