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

#include <climits>      // INT_MIN, INT_MAX

#include <vector>
#include <map>

#include <iomanip>      // setw, ...

#include "rational.h"

#include "utilities.h"

#include "messagesHandling.h"

#include "msrBasicTypes.h"

#include "generalOptions.h"

#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "msrOptions.h"
#include "lpsrOptions.h"

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

// tuplet factors
//______________________________________________________________________________
msrTupletFactor::msrTupletFactor ()
{
  fTupletActualNotes = -1;
  fTupletNormalNotes = -1;
}

msrTupletFactor::msrTupletFactor (
  int tupletActualNotes,
  int tupletNormalNotes)
{
  fTupletActualNotes = tupletActualNotes;
  fTupletNormalNotes = tupletNormalNotes;
}

msrTupletFactor::msrTupletFactor (
  rational rationalTupletFactor)
{
  fTupletActualNotes = rationalTupletFactor.getNumerator ();
  fTupletNormalNotes = rationalTupletFactor.getDenominator ();
}

string msrTupletFactor::asString () const
{
  stringstream s;

  s <<
    "'" <<
    "tupletActualNotes: " << fTupletActualNotes <<
    ", tupletNormalNotes" << " = " << fTupletNormalNotes <<
    "'";
  
  return s.str ();
}

void msrTupletFactor::print (ostream& os)
{
  const int fieldWidth = 11;

  os << left <<
    setw (fieldWidth) <<
    "tupletActualNotes" << " = " << fTupletActualNotes <<
    endl <<
    setw (fieldWidth) <<
    "tupletNormalNotes" << " = " << fTupletNormalNotes <<
    endl;
};

ostream& operator<< (ostream& os, msrTupletFactor elt)
{
  elt.print (os);
  return os;
}

// intervals
//______________________________________________________________________________
int msrIntervalKindAsSemiTones (
  msrIntervalKind intervalKind)
{
  int result = INT_MIN;

  switch (intervalKind) {
    case k_NoIntervalKind:
      break;
      
    case kDiminishedUnisson:
      result = -1;
      break;
    case kPerfectUnison:
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
    case kPerfectFourth:
      result = 5;
      break;
    case kAugmentedFourth:
      result = 6;
      break;
      
    case kDiminishedFifth:
      result = 6;
      break;
    case kPerfectFifth:
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
    case kPerfectOctave:
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
    case kPerfectEleventh:
      result = 17;
      break;
    case kAugmentedEleventh:
      result = 18;
      break;
      
    case kDiminishedTwelfth:
      result = 18;
      break;
    case kPerfectTwelfth:
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
    case k_NoIntervalKind:
      result = "noInterval";
      break;

    case kDiminishedUnisson:
      result = "diminishedUnisson";
      break;
    case kPerfectUnison:
      result = "perfectUnison";
      break;
    case kAugmentedUnison:
      result = "augmentedUnison";
      break;
      
    case kDiminishedSecond:
      result = "diminishedSecond";
      break;
    case kMinorSecond:
      result = "minorSecond";
      break;
    case kMajorSecond:
      result = "majorSecond";
      break;
    case kAugmentedSecond:
      result = "augmentedSecond";
      break;
      
    case kDiminishedThird:
      result = "diminishedThird";
      break;
    case kMinorThird:
      result = "minorThird";
      break;
    case kMajorThird:
      result = "majorThird";
      break;
    case kAugmentedThird:
      result = "augmentedThird";
      break;
      
    case kDiminishedFourth:
      result = "diminishedFourth";
      break;
    case kPerfectFourth:
      result = "perfectFourth";
      break;
    case kAugmentedFourth:
      result = "augmentedFourth";
      break;
      
    case kDiminishedFifth:
      result = "diminishedFifth";
      break;
    case kPerfectFifth:
      result = "perfectFifth";
      break;
    case kAugmentedFifth:
      result = "augmentedFifth";
      break;

    case kDiminishedSixth:
      result = "diminishedSixth";
      break;
    case kMinorSixth:
      result = "minorSixth";
      break;
    case kMajorSixth:
      result = "majorSixth";
      break;
    case kAugmentedSixth:
      result = "augmentedSixth";
      break;
      
    case kDiminishedSeventh:
      result = "diminishedSeventh";
      break;
    case kMinorSeventh:
      result = "minorSeventh";
      break;
    case kMajorSeventh:
      result = "majorSeventh";
      break;
    case kAugmentedSeventh:
      result = "augmentedSeventh";
      break;

    case kDiminishedOctave:
      result = "diminishedOctave";
      break;
    case kPerfectOctave:
      result = "perfectOctave";
      break;
    case kAugmentedOctave:
      result = "augmentedOctave";
      break;
      
    case kDiminishedNinth:
      result = "diminishedNinth";
      break;
    case kMinorNinth:
      result = "minorNinth";
      break;
    case kMajorNinth:
      result = "majorNinth";
      break;
    case kAugmentedNinth:
      result = "sugmentedNinth";
      break;
      
    case kDiminishedTenth:
      result = "diminishedTenth";
      break;
    case kMinorTenth:
      result = "minorTenth";
      break;
    case kMajorTenth:
      result = "majorTenth";
      break;
    case kAugmentedTenth:
      result = "augmentedTenth";
      break;
      
    case kDiminishedEleventh:
      result = "diminishedEleventh";
      break;
    case kPerfectEleventh:
      result = "perfectEleventh";
      break;
    case kAugmentedEleventh:
      result = "augmentedEleventh";
      break;
      
    case kDiminishedTwelfth:
      result = "diminishedTwelfth";
      break;
    case kPerfectTwelfth:
      result = "perfectTwelfth";
      break;
    case kAugmentedTwelfth:
      result = "augmentedTwelfth";
      break;

    case kDiminishedThirteenth:
      result = "diminishedThirteenth";
      break;
    case kMinorThirteenth:
      result = "minorThirteenth";
      break;
    case kMajorThirteenth:
      result = "majorThirteenth";
      break;
    case kAugmentedThirteenth:
      result = "augmentedThirteenth";
      break;
  } // switch

  return result;
}

string msrIntervalAsShortString (
  msrIntervalKind intervalKind)
{
  string result;
  
  switch (intervalKind) {
    case k_NoIntervalKind:
      result = "noInterval";
      break;

    case kDiminishedUnisson:
      result = "bu";
      break;
    case kPerfectUnison:
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
    case kPerfectFourth:
      result = "4";
      break;
    case kAugmentedFourth:
      result = "#4";
      break;
      
    case kDiminishedFifth:
      result = "b5";
      break;
    case kPerfectFifth:
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
    case kPerfectOctave:
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
    case kPerfectEleventh:
      result = "11";
      break;
    case kAugmentedEleventh:
      result = "#11";
      break;
      
    case kDiminishedTwelfth:
      result = "b12";
      break;
    case kPerfectTwelfth:
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
  msrIntervalKind result = k_NoIntervalKind;
  
  switch (intervalKind) {
    case k_NoIntervalKind:
      break;

    case kDiminishedUnisson:
      result = kAugmentedUnison;
      break;
    case kPerfectUnison:
      result = kPerfectUnison;
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
    case kPerfectFourth:
      result = kPerfectFifth;
      break;
    case kAugmentedFourth:
      result = kDiminishedFifth;
      break;
      
    case kDiminishedFifth:
      result = kAugmentedFourth;
      break;
    case kPerfectFifth:
      result = kPerfectFourth;
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
    case kPerfectOctave:
      result = kPerfectOctave;
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
    case kPerfectEleventh:
      result = kPerfectFifth;
      break;
    case kAugmentedEleventh:
      result = kDiminishedFifth;
      break;
      
    case kDiminishedTwelfth:
      result = kAugmentedFourth;
      break;
    case kPerfectTwelfth:
      result = kPerfectFourth;
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

int intervalKindAsSemitones (
  msrIntervalKind intervalKind)
{
  int result = INT_MIN;
  
  switch (intervalKind) {
    case k_NoIntervalKind:
      break;

    case kDiminishedUnisson:
      result = -1;
      break;
    case kPerfectUnison:
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
      result = 3;
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
    case kPerfectFourth:
      result = 5;
      break;
    case kAugmentedFourth:
      result = 6;
      break;
      
    case kDiminishedFifth:
      result = 7;
      break;
    case kPerfectFifth:
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
    case kPerfectOctave:
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
    case kPerfectEleventh:
      result = 17;
      break;
    case kAugmentedEleventh:
      result = 18;
      break;
      
    case kDiminishedTwelfth:
      result = 18;
      break;
    case kPerfectTwelfth:
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


/* JMI
msrSemiTonesPitchKind noteAtIntervalKindFromNote (
  msrIntervalKind             intervalKind,
  msrSemiTonesPitchKind       semiTonesPitchKind,
  msrAlterationPreferenceKind alterationPreferenceKind)
{
  msrSemiTonesPitchKind result = k_NoSemiTonesPitch_STP;

  switch (semiTonesPitchKind) {
    case k_NoSemiTonesPitch_STP:
      result = k_NoSemiTonesPitch_STP;
      break;

    case kB_Sharp_STP:
      switch (intervalKind) {
        case kDiminishedUnisson:
          switch (alterationPreferenceKind) {
            case kPreferFlat:
              result = kC_Sharp_STP;
              break;
            case kPreferNatural:
              result = kC_Sharp_STP;
              break;
            case kPreferSharp:
              result = kC_Sharp_STP;
              break;
          } // switch
          break;
        case kPerfectUnison:
          break;
        case kAugmentedUnison:
          break;
          
        case kDiminishedSecond:
          break;
        case kMinorSecond:
          break;
        case kMajorSecond:
          break;
        case kAugmentedSecond:
          break;
          
        case kDiminishedThird:
          break;
        case kMinorThird:
          break;
        case kMajorThird:
          break;
        case kAugmentedThird:
          break;
          
        case kDiminishedFourth:
          break;
        case kPerfectFourth:
          break;
        case kAugmentedFourth:
          break;
          
        case kDiminishedFifth:
          break;
        case kPerfectFifth:
          break;
        case kAugmentedFifth:
          break;
    
        case kDiminishedSixth:
          break;
        case kMinorSixth:
          break;
        case kMajorSixth:
          break;
        case kAugmentedSixth:
          break;
          
        case kDiminishedSeventh:
          break;
        case kMinorSeventh:
          break;
        case kMajorSeventh:
          break;
        case kAugmentedSeventh:
          break;
    
        case kDiminishedOctave:
          break;
        case kPerfectOctave:
          break;
        case kAugmentedOctave:
          break;
          
        case kDiminishedNinth:
          break;
        case kMinorNinth:
          break;
        case kMajorNinth:
          break;
        case kAugmentedNinth:
          break;
          
        case kDiminishedTenth:
          break;
        case kMinorTenth:
          break;
        case kMajorTenth:
          break;
        case kAugmentedTenth:
          break;
          
        case kDiminishedEleventh:
          break;
        case kPerfectEleventh:
          break;
        case kAugmentedEleventh:
          break;
          
        case kDiminishedTwelfth:
          break;
        case kPerfectTwelfth:
          break;
        case kAugmentedTwelfth:
          break;
    
        case kDiminishedThirteenth:
          break;
        case kMinorThirteenth:
          break;
        case kMajorThirteenth:
          break;
        case kAugmentedThirteenth:
          break;
      } // switch
      break;
          
    case kC_Natural_STP:
      break;
    case kD_DoubleFlat_STP:
      break;
      
    case kC_Sharp_STP:
      break;
    case kB_DoubleSharp_STP:
      break;
    case kD_Flat_STP:
      break;

    case kD_Natural_STP:
      break;
    case kC_DoubleSharp_STP:
      break;
    case kE_DoubleFlat_STP:
      break;

    case kD_Sharp_STP:
      break;
    case kE_Flat_STP:
      break;
      
    case kE_Natural_STP:
      break;
    case kD_DoubleSharp_STP:
      break;
    case kF_Flat_STP:
      break;
      
    case kF_Natural_STP:
      break;
    case kE_Sharp_STP:
      break;
    case kG_DoubleFlat_STP:
      break;
      
    case kF_Sharp_STP:
      break;
    case kE_DoubleSharp_STP:
      break;
    case kG_Flat_STP:
      break;
      
    case kG_Natural_STP:
      break;
    case kF_DoubleSharp_STP:
      break;
    case kA_DoubleFlat_STP:
      break;
      
    case kG_Sharp_STP:
      break;
    case kA_Flat_STP:
      break;
      
    case kA_Natural_STP:
      break;
    case kG_DoubleSharp_STP:
      break;
    case kB_DoubleFlat_STP:
      break;
      
    case kA_Sharp_STP:
      break;
    case kB_Flat_STP:
      break;

    case kB_Natural_STP:
      break;
    case kA_DoubleSharp_STP:
      break;
    case kC_Flat_STP:
      break;
  } // switch

  return result;
}
*/

msrSemiTonesPitchKind noteAtIntervalFromSemiTonesPitch (
  int                   inputLineNumber,
  msrIntervalKind       intervalKind,
  msrSemiTonesPitchKind semiTonesPitchKind)
{
  msrSemiTonesPitchKind result = k_NoSemiTonesPitch_STP;

  switch (semiTonesPitchKind) {
    case k_NoSemiTonesPitch_STP:
      result = k_NoSemiTonesPitch_STP;
      break;
          
    case kC_Flat_STP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kC_DoubleFlat_STP;
          break;
        case kPerfectUnison:
          result = kC_Flat_STP;
          break;
        case kAugmentedUnison:
          result = kC_Natural_STP;
          break;
          
        case kDiminishedSecond:
          result = kD_TripleFlat_STP;
          break;
        case kMinorSecond:
          result = kD_DoubleFlat_STP;
          break;
        case kMajorSecond:
          result = kD_Flat_STP;
          break;
        case kAugmentedSecond:
          result = kD_Natural_STP;
          break;
          
        case kDiminishedThird:
          result = kE_DoubleFlat_STP;
          break;
        case kMinorThird:
          result = kE_TripleFlat_STP;
          break;
        case kMajorThird:
          result = kE_Flat_STP;
          break;
        case kAugmentedThird:
          result = kE_Natural_STP;
          break;
          
        case kDiminishedFourth:
          result = kF_DoubleFlat_STP;
          break;
        case kPerfectFourth:
          result = kF_Flat_STP;
          break;
        case kAugmentedFourth:
          result = kF_Natural_STP;
          break;
          
        case kDiminishedFifth:
          result = kG_DoubleFlat_STP;
          break;
        case kPerfectFifth:
          result = kG_Flat_STP;
          break;
        case kAugmentedFifth:
          result = kG_Natural_STP;
          break;
    
        case kDiminishedSixth:
          result = kA_TripleFlat_STP;
          break;
        case kMinorSixth:
          result = kA_DoubleFlat_STP;
          break;
        case kMajorSixth:
          result = kA_Flat_STP;
          break;
        case kAugmentedSixth:
          result = kA_Natural_STP;
          break;
          
        case kDiminishedSeventh:
          result = kB_TripleFlat_STP;
          break;
        case kMinorSeventh:
          result = kB_DoubleFlat_STP;
          break;
        case kMajorSeventh:
          result = kB_Flat_STP;
          break;
        case kAugmentedSeventh:
          result = kB_Natural_STP;
          break;
    
        case kDiminishedOctave:
          result = kC_DoubleFlat_STP;
          break;
        case kPerfectOctave:
          result = kC_Flat_STP;
          break;
        case kAugmentedOctave:
          result = kC_Natural_STP;
          break;
          
        case kDiminishedNinth:
          result = kD_TripleFlat_STP;
          break;
        case kMinorNinth:
          result = kD_DoubleFlat_STP;
          break;
        case kMajorNinth:
          result = kD_Flat_STP;
          break;
        case kAugmentedNinth:
          result = kD_Natural_STP;
          break;
          
        case kDiminishedTenth:
          result = kE_TripleFlat_STP;
          break;
        case kMinorTenth:
          result = kE_DoubleFlat_STP;
          break;
        case kMajorTenth:
          result = kE_Flat_STP;
          break;
        case kAugmentedTenth:
          result = kE_Natural_STP;
          break;
          
        case kDiminishedEleventh:
          result = kF_DoubleFlat_STP;
          break;
        case kPerfectEleventh:
          result = kF_Flat_STP;
          break;
        case kAugmentedEleventh:
          result = kF_Natural_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kG_DoubleFlat_STP;
          break;
        case kPerfectTwelfth:
          result = kG_Flat_STP;
          break;
        case kAugmentedTwelfth:
          result = kG_Natural_STP;
          break;
    
        case kDiminishedThirteenth:
          result = kA_TripleFlat_STP;
          break;
        case kMinorThirteenth:
          result = kA_DoubleFlat_STP;
          break;
        case kMajorThirteenth:
          result = kA_Flat_STP;
          break;
        case kAugmentedThirteenth:
          result = kA_Natural_STP;
          break;
      } // switch
      break;

    case kC_Natural_STP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kC_Flat_STP;
          break;
        case kPerfectUnison:
          result = kC_Natural_STP;
          break;
        case kAugmentedUnison:
          result = kC_Sharp_STP;
          break;
          
        case kDiminishedSecond:
          result = kD_DoubleFlat_STP;
          break;
        case kMinorSecond:
          result = kD_Flat_STP;
          break;
        case kMajorSecond:
          result = kD_Natural_STP;
          break;
        case kAugmentedSecond:
          result = kD_Sharp_STP;
          break;
          
        case kDiminishedThird:
          result = kE_DoubleFlat_STP;
          break;
        case kMinorThird:
          result = kE_Flat_STP;
          break;
        case kMajorThird:
          result = kE_Natural_STP;
          break;
        case kAugmentedThird:
          result = kE_Sharp_STP;
          break;
          
        case kDiminishedFourth:
          result = kF_Flat_STP;
          break;
        case kPerfectFourth:
          result = kF_Natural_STP;
          break;
        case kAugmentedFourth:
          result = kF_Sharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kG_Flat_STP;
          break;
        case kPerfectFifth:
          result = kG_Natural_STP;
          break;
        case kAugmentedFifth:
          result = kG_Sharp_STP;
          break;
    
        case kDiminishedSixth:
          result = kA_DoubleFlat_STP;
          break;
        case kMinorSixth:
          result = kA_Flat_STP;
          break;
        case kMajorSixth:
          result = kA_Natural_STP;
          break;
        case kAugmentedSixth:
          result = kA_Sharp_STP;
          break;
          
        case kDiminishedSeventh:
          result = kB_DoubleFlat_STP;
          break;
        case kMinorSeventh:
          result = kB_Flat_STP;
          break;
        case kMajorSeventh:
          result = kB_Natural_STP;
          break;
        case kAugmentedSeventh:
          result = kB_Sharp_STP;
          break;
    
        case kDiminishedOctave:
          result = kC_Flat_STP;
          break;
        case kPerfectOctave:
          result = kC_Natural_STP;
          break;
        case kAugmentedOctave:
          result = kC_Sharp_STP;
          break;
          
        case kDiminishedNinth:
          result = kD_DoubleFlat_STP;
          break;
        case kMinorNinth:
          result = kD_Flat_STP;
          break;
        case kMajorNinth:
          result = kD_Natural_STP;
          break;
        case kAugmentedNinth:
          result = kD_Sharp_STP;
          break;
          
        case kDiminishedTenth:
          result = kE_DoubleFlat_STP;
          break;
        case kMinorTenth:
          result = kE_Flat_STP;
          break;
        case kMajorTenth:
          result = kE_Natural_STP;
          break;
        case kAugmentedTenth:
          result = kE_Sharp_STP;
          break;
          
        case kDiminishedEleventh:
          result = kF_Flat_STP;
          break;
        case kPerfectEleventh:
          result = kF_Natural_STP;
          break;
        case kAugmentedEleventh:
          result = kF_Sharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kG_Flat_STP;
          break;
        case kPerfectTwelfth:
          result = kG_Natural_STP;
          break;
        case kAugmentedTwelfth:
          result = kG_Sharp_STP;
          break;
    
        case kDiminishedThirteenth:
          result = kA_DoubleFlat_STP;
          break;
        case kMinorThirteenth:
          result = kA_Flat_STP;
          break;
        case kMajorThirteenth:
          result = kA_Natural_STP;
          break;
        case kAugmentedThirteenth:
          result = kG_Sharp_STP;
          break;
      } // switch
      break;
      
    case kC_Sharp_STP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kC_Natural_STP;
          break;
        case kPerfectUnison:
          result = kC_Sharp_STP;
          break;
        case kAugmentedUnison:
          result = kC_DoubleSharp_STP;
          break;
          
        case kDiminishedSecond:
          result = kD_Flat_STP;
          break;
        case kMinorSecond:
          result = kD_Natural_STP;
          break;
        case kMajorSecond:
          result = kD_Sharp_STP;
          break;
        case kAugmentedSecond:
          result = kD_DoubleSharp_STP;
          break;
          
        case kDiminishedThird:
          result = kE_Flat_STP;
          break;
        case kMinorThird:
          result = kE_Natural_STP;
          break;
        case kMajorThird:
          result = kE_Sharp_STP;
          break;
        case kAugmentedThird:
          result = kE_DoubleSharp_STP;
          break;
          
        case kDiminishedFourth:
          result = kF_Natural_STP;
          break;
        case kPerfectFourth:
          result = kF_Sharp_STP;
          break;
        case kAugmentedFourth:
          result = kF_DoubleSharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kG_Natural_STP;
          break;
        case kPerfectFifth:
          result = kG_Sharp_STP;
          break;
        case kAugmentedFifth:
          result = kG_DoubleSharp_STP;
          break;
    
        case kDiminishedSixth:
          result = kA_Flat_STP;
          break;
        case kMinorSixth:
          result = kA_Natural_STP;
          break;
        case kMajorSixth:
          result = kA_Sharp_STP;
          break;
        case kAugmentedSixth:
          result = kA_DoubleSharp_STP;
          break;
          
        case kDiminishedSeventh:
          result = kB_Flat_STP;
          break;
        case kMinorSeventh:
          result = kB_Natural_STP;
          break;
        case kMajorSeventh:
          result = kB_Sharp_STP;
          break;
        case kAugmentedSeventh:
          result = kB_DoubleSharp_STP;
          break;
    
        case kDiminishedOctave:
          result = kC_Natural_STP;
          break;
        case kPerfectOctave:
          result = kC_Sharp_STP;
          break;
        case kAugmentedOctave:
          result = kC_DoubleSharp_STP;
          break;
          
        case kDiminishedNinth:
          result = kD_Flat_STP;
          break;
        case kMinorNinth:
          result = kD_Natural_STP;
          break;
        case kMajorNinth:
          result = kD_Sharp_STP;
          break;
        case kAugmentedNinth:
          result = kD_DoubleSharp_STP;
          break;
          
        case kDiminishedTenth:
          result = kE_Flat_STP;
          break;
        case kMinorTenth:
          result = kE_Natural_STP;
          break;
        case kMajorTenth:
          result = kE_Sharp_STP;
          break;
        case kAugmentedTenth:
          result = kE_DoubleSharp_STP;
          break;
          
        case kDiminishedEleventh:
          result = kF_Natural_STP;
          break;
        case kPerfectEleventh:
          result = kF_Sharp_STP;
          break;
        case kAugmentedEleventh:
          result = kF_DoubleSharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kG_Natural_STP;
          break;
        case kPerfectTwelfth:
          result = kG_Sharp_STP;
          break;
        case kAugmentedTwelfth:
          result = kG_DoubleSharp_STP;
          break;
    
        case kDiminishedThirteenth:
          result = kA_Flat_STP;
          break;
        case kMinorThirteenth:
          result = kA_Natural_STP;
          break;
        case kMajorThirteenth:
          result = kA_Sharp_STP;
          break;
        case kAugmentedThirteenth:
          result = kA_DoubleSharp_STP;
          break;
      } // switch
      break;

    case kD_Flat_STP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kD_DoubleFlat_STP;
          break;
        case kPerfectUnison:
          result = kD_Flat_STP;
          break;
        case kAugmentedUnison:
          result = kD_Natural_STP;
          break;
          
        case kDiminishedSecond:
          result = kE_TripleFlat_STP;
          break;
        case kMinorSecond:
          result = kE_DoubleFlat_STP;
          break;
        case kMajorSecond:
          result = kE_Flat_STP;
          break;
        case kAugmentedSecond:
          result = kE_Natural_STP;
          break;
          
        case kDiminishedThird:
          result = kF_DoubleFlat_STP;
          break;
        case kMinorThird:
          result = kF_Flat_STP;
          break;
        case kMajorThird:
          result = kF_Natural_STP;
          break;
        case kAugmentedThird:
          result = kF_Sharp_STP;
          break;
          
        case kDiminishedFourth:
          result = kG_DoubleFlat_STP;
          break;
        case kPerfectFourth:
          result = kG_Flat_STP;
          break;
        case kAugmentedFourth:
          result = kG_Natural_STP;
          break;
          
        case kDiminishedFifth:
          result = kA_DoubleFlat_STP;
          break;
        case kPerfectFifth:
          result = kA_Flat_STP;
          break;
        case kAugmentedFifth:
          result = kA_Natural_STP;
          break;
    
        case kDiminishedSixth:
          result = kB_TripleFlat_STP;
          break;
        case kMinorSixth:
          result = kB_DoubleFlat_STP;
          break;
        case kMajorSixth:
          result = kB_Flat_STP;
          break;
        case kAugmentedSixth:
          result = kB_Natural_STP;
          break;
          
        case kDiminishedSeventh:
          result = kC_DoubleFlat_STP;
          break;
        case kMinorSeventh:
          result = kC_Flat_STP;
          break;
        case kMajorSeventh:
          result = kC_Natural_STP;
          break;
        case kAugmentedSeventh:
          result = kC_Sharp_STP;
          break;
    
        case kDiminishedOctave:
          result = kD_DoubleFlat_STP;
          break;
        case kPerfectOctave:
          result = kD_Flat_STP;
          break;
        case kAugmentedOctave:
          result = kD_Natural_STP;
          break;
          
        case kDiminishedNinth:
          result = kE_TripleFlat_STP;
          break;
        case kMinorNinth:
          result = kE_DoubleFlat_STP;
          break;
        case kMajorNinth:
          result = kE_Flat_STP;
          break;
        case kAugmentedNinth:
          result = kE_Natural_STP;
          break;
          
        case kDiminishedTenth:
          result = kF_DoubleFlat_STP;
          break;
        case kMinorTenth:
          result = kF_Flat_STP;
          break;
        case kMajorTenth:
          result = kF_Natural_STP;
          break;
        case kAugmentedTenth:
          result = kF_Sharp_STP;
          break;
          
        case kDiminishedEleventh:
          result = kG_DoubleFlat_STP;
          break;
        case kPerfectEleventh:
          result = kG_Flat_STP;
          break;
        case kAugmentedEleventh:
          result = kG_Natural_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kA_DoubleFlat_STP;
          break;
        case kPerfectTwelfth:
          result = kA_Flat_STP;
          break;
        case kAugmentedTwelfth:
          result = kA_Natural_STP;
          break;
    
        case kDiminishedThirteenth:
          result = kB_TripleFlat_STP;
          break;
        case kMinorThirteenth:
          result = kB_DoubleFlat_STP;
          break;
        case kMajorThirteenth:
          result = kB_Flat_STP;
          break;
        case kAugmentedThirteenth:
          result = kB_Natural_STP;
          break;
      } // switch
      break;

    case kD_Natural_STP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kD_Flat_STP;
          break;
        case kPerfectUnison:
          result = kD_Natural_STP;
          break;
        case kAugmentedUnison:
          result = kD_Sharp_STP;
          break;
          
        case kDiminishedSecond:
          result = kE_DoubleFlat_STP;
          break;
        case kMinorSecond:
          result = kE_Flat_STP;
          break;
        case kMajorSecond:
          result = kE_Natural_STP;
          break;
        case kAugmentedSecond:
          result = kE_Sharp_STP;
          break;
          
        case kDiminishedThird:
          result = kF_Flat_STP;
          break;
        case kMinorThird:
          result = kF_Natural_STP;
          break;
        case kMajorThird:
          result = kF_Sharp_STP;
          break;
        case kAugmentedThird:
          result = kF_DoubleSharp_STP;
          break;
          
        case kDiminishedFourth:
          result = kG_Flat_STP;
          break;
        case kPerfectFourth:
          result = kG_Natural_STP;
          break;
        case kAugmentedFourth:
          result = kG_Sharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kA_Flat_STP;
          break;
        case kPerfectFifth:
          result = kA_Natural_STP;
          break;
        case kAugmentedFifth:
          result = kA_Sharp_STP;
          break;
    
        case kDiminishedSixth:
          result = kB_DoubleFlat_STP;
          break;
        case kMinorSixth:
          result = kB_Flat_STP;
          break;
        case kMajorSixth:
          result = kB_Natural_STP;
          break;
        case kAugmentedSixth:
          result = kB_Sharp_STP;
          break;
          
        case kDiminishedSeventh:
          result = kC_Flat_STP;
          break;
        case kMinorSeventh:
          result = kC_Natural_STP;
          break;
        case kMajorSeventh:
          result = kC_Sharp_STP;
          break;
        case kAugmentedSeventh:
          result = kC_DoubleSharp_STP;
          break;
    
        case kDiminishedOctave:
          result = kD_Flat_STP;
          break;
        case kPerfectOctave:
          result = kD_Natural_STP;
          break;
        case kAugmentedOctave:
          result = kD_Sharp_STP;
          break;
          
        case kDiminishedNinth:
          result = kE_DoubleFlat_STP;
          break;
        case kMinorNinth:
          result = kE_Flat_STP;
          break;
        case kMajorNinth:
          result = kE_Natural_STP;
          break;
        case kAugmentedNinth:
          result = kE_Sharp_STP;
          break;
          
        case kDiminishedTenth:
          result = kF_Flat_STP;
          break;
        case kMinorTenth:
          result = kF_Natural_STP;
          break;
        case kMajorTenth:
          result = kF_Sharp_STP;
          break;
        case kAugmentedTenth:
          result = kF_DoubleSharp_STP;
          break;
          
        case kDiminishedEleventh:
          result = kG_Flat_STP;
          break;
        case kPerfectEleventh:
          result = kG_Natural_STP;
          break;
        case kAugmentedEleventh:
          result = kG_Sharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kA_Flat_STP;
          break;
        case kPerfectTwelfth:
          result = kA_Natural_STP;
          break;
        case kAugmentedTwelfth:
          result = kA_Sharp_STP;
          break;
    
        case kDiminishedThirteenth:
          result = kB_DoubleFlat_STP;
          break;
        case kMinorThirteenth:
          result = kB_Flat_STP;
          break;
        case kMajorThirteenth:
          result = kB_Natural_STP;
          break;
        case kAugmentedThirteenth:
          result = kB_Sharp_STP;
          break;
      } // switch
      break;

    case kD_Sharp_STP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kD_Natural_STP;
          break;
        case kPerfectUnison:
          result = kD_Sharp_STP;
          break;
        case kAugmentedUnison:
          result = kD_DoubleSharp_STP;
          break;
          
        case kDiminishedSecond:
          result = kE_Flat_STP;
          break;
        case kMinorSecond:
          result = kE_Natural_STP;
          break;
        case kMajorSecond:
          result = kE_Sharp_STP;
          break;
        case kAugmentedSecond:
          result = kE_DoubleSharp_STP;
          break;
          
        case kDiminishedThird:
          result = kF_Flat_STP;
          break;
        case kMinorThird:
          result = kF_Natural_STP;
          break;
        case kMajorThird:
          result = kF_Sharp_STP;
          break;
        case kAugmentedThird:
          result = kF_DoubleSharp_STP;
          break;
          
        case kDiminishedFourth:
          result = kG_Natural_STP;
          break;
        case kPerfectFourth:
          result = kG_Sharp_STP;
          break;
        case kAugmentedFourth:
          result = kG_DoubleSharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kA_Natural_STP;
          break;
        case kPerfectFifth:
          result = kA_Sharp_STP;
          break;
        case kAugmentedFifth:
          result = kA_DoubleSharp_STP;
          break;
    
        case kDiminishedSixth:
          result = kB_Flat_STP;
          break;
        case kMinorSixth:
          result = kB_Natural_STP;
          break;
        case kMajorSixth:
          result = kB_Sharp_STP;
          break;
        case kAugmentedSixth:
          result = kB_DoubleSharp_STP;
          break;
          
        case kDiminishedSeventh:
          result = kC_Natural_STP;
          break;
        case kMinorSeventh:
          result = kC_Sharp_STP;
          break;
        case kMajorSeventh:
          result = kC_DoubleSharp_STP;
          break;
        case kAugmentedSeventh:
          result = kC_TripleSharp_STP;
          break;
    
        case kDiminishedOctave:
          result = kD_Natural_STP;
          break;
        case kPerfectOctave:
          result = kD_Sharp_STP;
          break;
        case kAugmentedOctave:
          result = kD_DoubleSharp_STP;
          break;
          
        case kDiminishedNinth:
          result = kE_Flat_STP;
          break;
        case kMinorNinth:
          result = kE_Natural_STP;
          break;
        case kMajorNinth:
          result = kE_Sharp_STP;
          break;
        case kAugmentedNinth:
          result = kE_DoubleSharp_STP;
          break;
          
        case kDiminishedTenth:
          result = kF_Natural_STP;
          break;
        case kMinorTenth:
          result = kF_Sharp_STP;
          break;
        case kMajorTenth:
          result = kF_DoubleSharp_STP;
          break;
        case kAugmentedTenth:
          result = kF_DoubleSharp_STP;
          break;
          
        case kDiminishedEleventh:
          result = kG_Natural_STP;
          break;
        case kPerfectEleventh:
          result = kG_Sharp_STP;
          break;
        case kAugmentedEleventh:
          result = kG_DoubleSharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kA_Natural_STP;
          break;
        case kPerfectTwelfth:
          result = kA_Sharp_STP;
          break;
        case kAugmentedTwelfth:
          result = kA_DoubleSharp_STP;
          break;
    
        case kDiminishedThirteenth:
          result = kB_Flat_STP;
          break;
        case kMinorThirteenth:
          result = kB_Natural_STP;
          break;
        case kMajorThirteenth:
          result = kB_Sharp_STP;
          break;
        case kAugmentedThirteenth:
          result = kB_DoubleSharp_STP;
          break;
      } // switch
      break;

    case kE_Flat_STP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kE_DoubleFlat_STP;
          break;
        case kPerfectUnison:
          result = kE_Flat_STP;
          break;
        case kAugmentedUnison:
          result = kE_Natural_STP;
          break;
          
        case kDiminishedSecond:
          result = kF_DoubleFlat_STP;
          break;
        case kMinorSecond:
          result = kF_Flat_STP;
          break;
        case kMajorSecond:
          result = kF_Natural_STP;
          break;
        case kAugmentedSecond:
          result = kF_Sharp_STP;
          break;
          
        case kDiminishedThird:
          result = kG_DoubleFlat_STP;
          break;
        case kMinorThird:
          result = kG_Flat_STP;
          break;
        case kMajorThird:
          result = kG_Natural_STP;
          break;
        case kAugmentedThird:
          result = kG_Sharp_STP;
          break;
          
        case kDiminishedFourth:
          result = kA_DoubleFlat_STP;
          break;
        case kPerfectFourth:
          result = kA_Flat_STP;
          break;
        case kAugmentedFourth:
          result = kA_Natural_STP;
          break;
          
        case kDiminishedFifth:
          result = kB_DoubleFlat_STP;
          break;
        case kPerfectFifth:
          result = kB_Flat_STP;
          break;
        case kAugmentedFifth:
          result = kB_Natural_STP;
          break;
    
        case kDiminishedSixth:
          result = kC_DoubleFlat_STP;
          break;
        case kMinorSixth:
          result = kC_Flat_STP;
          break;
        case kMajorSixth:
          result = kC_Natural_STP;
          break;
        case kAugmentedSixth:
          result = kC_Sharp_STP;
          break;
          
        case kDiminishedSeventh:
          result = kD_DoubleFlat_STP;
          break;
        case kMinorSeventh:
          result = kD_Flat_STP;
          break;
        case kMajorSeventh:
          result = kD_Natural_STP;
          break;
        case kAugmentedSeventh:
          result = kD_Sharp_STP;
          break;
    
        case kDiminishedOctave:
          result = kE_DoubleFlat_STP;
          break;
        case kPerfectOctave:
          result = kE_Flat_STP;
          break;
        case kAugmentedOctave:
          result = kE_Natural_STP;
          break;
          
        case kDiminishedNinth:
          result = kF_DoubleFlat_STP;
          break;
        case kMinorNinth:
          result = kF_Flat_STP;
          break;
        case kMajorNinth:
          result = kF_Natural_STP;
          break;
        case kAugmentedNinth:
          result = kF_Sharp_STP;
          break;
          
        case kDiminishedTenth:
          result = kG_DoubleFlat_STP;
          break;
        case kMinorTenth:
          result = kG_Flat_STP;
          break;
        case kMajorTenth:
          result = kG_Natural_STP;
          break;
        case kAugmentedTenth:
          result = kG_Sharp_STP;
          break;
          
        case kDiminishedEleventh:
          result = kA_DoubleFlat_STP;
          break;
        case kPerfectEleventh:
          result = kA_Flat_STP;
          break;
        case kAugmentedEleventh:
          result = kA_Natural_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kB_DoubleFlat_STP;
          break;
        case kPerfectTwelfth:
          result = kB_Flat_STP;
          break;
        case kAugmentedTwelfth:
          result = kB_Natural_STP;
          break;
    
        case kDiminishedThirteenth:
          result = kC_DoubleFlat_STP;
          break;
        case kMinorThirteenth:
          result = kC_Flat_STP;
          break;
        case kMajorThirteenth:
          result = kC_Natural_STP;
          break;
        case kAugmentedThirteenth:
          result = kC_Sharp_STP;
          break;
      } // switch
      break;
      
    case kE_Natural_STP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kE_Flat_STP;
          break;
        case kPerfectUnison:
          result = kE_Natural_STP;
          break;
        case kAugmentedUnison:
          result = kE_Sharp_STP;
          break;
          
        case kDiminishedSecond:
          result = kF_Flat_STP;
          break;
        case kMinorSecond:
          result = kF_Natural_STP;
          break;
        case kMajorSecond:
          result = kF_Sharp_STP;
          break;
        case kAugmentedSecond:
          result = kF_DoubleSharp_STP;
          break;
          
        case kDiminishedThird:
          result = kG_Flat_STP;
          break;
        case kMinorThird:
          result = kG_Natural_STP;
          break;
        case kMajorThird:
          result = kG_Sharp_STP;
          break;
        case kAugmentedThird:
          result = kG_DoubleSharp_STP;
          break;
          
        case kDiminishedFourth:
          result = kA_Flat_STP;
          break;
        case kPerfectFourth:
          result = kA_Natural_STP;
          break;
        case kAugmentedFourth:
          result = kA_Sharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kB_Natural_STP;
          break;
        case kPerfectFifth:
          result = kB_Sharp_STP;
          break;
        case kAugmentedFifth:
          result = kB_DoubleSharp_STP;
          break;
    
        case kDiminishedSixth:
          result = kC_Flat_STP;
          break;
        case kMinorSixth:
          result = kC_Natural_STP;
          break;
        case kMajorSixth:
          result = kC_Sharp_STP;
          break;
        case kAugmentedSixth:
          result = kC_DoubleSharp_STP;
          break;
          
        case kDiminishedSeventh:
          result = kD_Flat_STP;
          break;
        case kMinorSeventh:
          result = kD_Natural_STP;
          break;
        case kMajorSeventh:
          result = kD_Sharp_STP;
          break;
        case kAugmentedSeventh:
          result = kD_DoubleSharp_STP;
          break;
    
        case kDiminishedOctave:
          result = kE_Flat_STP;
          break;
        case kPerfectOctave:
          result = kE_Natural_STP;
          break;
        case kAugmentedOctave:
          result = kE_Sharp_STP;
          break;
          
        case kDiminishedNinth:
          result = kF_Flat_STP;
          break;
        case kMinorNinth:
          result = kF_Natural_STP;
          break;
        case kMajorNinth:
          result = kF_Sharp_STP;
          break;
        case kAugmentedNinth:
          result = kF_DoubleSharp_STP;
          break;
          
        case kDiminishedTenth:
          result = kG_Flat_STP;
          break;
        case kMinorTenth:
          result = kG_Natural_STP;
          break;
        case kMajorTenth:
          result = kG_Sharp_STP;
          break;
        case kAugmentedTenth:
          result = kG_DoubleSharp_STP;
          break;
          
        case kDiminishedEleventh:
          result = kA_Flat_STP;
          break;
        case kPerfectEleventh:
          result = kA_Natural_STP;
          break;
        case kAugmentedEleventh:
          result = kA_Sharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kB_Natural_STP;
          break;
        case kPerfectTwelfth:
          result = kB_Sharp_STP;
          break;
        case kAugmentedTwelfth:
          result = kB_DoubleSharp_STP;
          break;
    
        case kDiminishedThirteenth:
          result = kC_Flat_STP;
          break;
        case kMinorThirteenth:
          result = kC_Natural_STP;
          break;
        case kMajorThirteenth:
          result = kC_Sharp_STP;
          break;
        case kAugmentedThirteenth:
          result = kC_DoubleSharp_STP;
          break;
      } // switch
      break;

    case kE_Sharp_STP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kE_Natural_STP;
          break;
        case kPerfectUnison:
          result = kE_Sharp_STP;
          break;
        case kAugmentedUnison:
          result = kE_DoubleSharp_STP;
          break;
          
        case kDiminishedSecond:
          result = kF_Natural_STP;
          break;
        case kMinorSecond:
          result = kF_Sharp_STP;
          break;
        case kMajorSecond:
          result = kF_DoubleSharp_STP;
          break;
        case kAugmentedSecond:
          result = kF_TripleSharp_STP;
          break;
          
        case kDiminishedThird:
          result = kG_Natural_STP;
          break;
        case kMinorThird:
          result = kG_Sharp_STP;
          break;
        case kMajorThird:
          result = kG_DoubleSharp_STP;
          break;
        case kAugmentedThird:
          result = kG_TripleSharp_STP;
          break;
          
        case kDiminishedFourth:
          result = kA_Natural_STP;
          break;
        case kPerfectFourth:
          result = kA_Sharp_STP;
          break;
        case kAugmentedFourth:
          result = kA_DoubleSharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kB_Natural_STP;
          break;
        case kPerfectFifth:
          result = kB_Sharp_STP;
          break;
        case kAugmentedFifth:
          result = kB_DoubleSharp_STP;
          break;
    
        case kDiminishedSixth:
          result = kC_Natural_STP;
          break;
        case kMinorSixth:
          result = kC_Sharp_STP;
          break;
        case kMajorSixth:
          result = kC_DoubleSharp_STP;
          break;
        case kAugmentedSixth:
          result = kC_TripleSharp_STP;
          break;
          
        case kDiminishedSeventh:
          result = kD_Natural_STP;
          break;
        case kMinorSeventh:
          result = kD_Sharp_STP;
          break;
        case kMajorSeventh:
          result = kD_DoubleSharp_STP;
          break;
        case kAugmentedSeventh:
          result = kD_TripleSharp_STP;
          break;
    
        case kDiminishedOctave:
          result = kE_Natural_STP;
          break;
        case kPerfectOctave:
          result = kE_Sharp_STP;
          break;
        case kAugmentedOctave:
          result = kE_DoubleSharp_STP;
          break;
          
        case kDiminishedNinth:
          result = kF_Natural_STP;
          break;
        case kMinorNinth:
          result = kF_Sharp_STP;
          break;
        case kMajorNinth:
          result = kF_DoubleSharp_STP;
          break;
        case kAugmentedNinth:
          result = kF_TripleSharp_STP;
          break;
          
        case kDiminishedTenth:
          result = kG_Natural_STP;
          break;
        case kMinorTenth:
          result = kG_Sharp_STP;
          break;
        case kMajorTenth:
          result = kG_DoubleSharp_STP;
          break;
        case kAugmentedTenth:
          result = kG_TripleSharp_STP;
          break;
          
        case kDiminishedEleventh:
          result = kA_Natural_STP;
          break;
        case kPerfectEleventh:
          result = kA_Sharp_STP;
          break;
        case kAugmentedEleventh:
          result = kA_DoubleSharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kB_Natural_STP;
          break;
        case kPerfectTwelfth:
          result = kB_Sharp_STP;
          break;
        case kAugmentedTwelfth:
          result = kB_DoubleSharp_STP;
          break;
    
        case kDiminishedThirteenth:
          result = kC_Natural_STP;
          break;
        case kMinorThirteenth:
          result = kC_Sharp_STP;
          break;
        case kMajorThirteenth:
          result = kC_DoubleSharp_STP;
          break;
        case kAugmentedThirteenth:
          result = kC_TripleSharp_STP;
          break;
      } // switch
      break;

    case kF_Flat_STP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kF_DoubleFlat_STP;
          break;
        case kPerfectUnison:
          result = kF_Flat_STP;
          break;
        case kAugmentedUnison:
          result = kF_Natural_STP;
          break;
          
        case kDiminishedSecond:
          result = kG_TripleFlat_STP;
          break;
        case kMinorSecond:
          result = kG_DoubleFlat_STP;
          break;
        case kMajorSecond:
          result = kG_Flat_STP;
          break;
        case kAugmentedSecond:
          result = kG_Natural_STP;
          break;
          
        case kDiminishedThird:
          result = kA_TripleFlat_STP;
          break;
        case kMinorThird:
          result = kA_DoubleFlat_STP;
          break;
        case kMajorThird:
          result = kA_Flat_STP;
          break;
        case kAugmentedThird:
          result = kA_Natural_STP;
          break;
          
        case kDiminishedFourth:
          result = kB_TripleFlat_STP;
          break;
        case kPerfectFourth:
          result = kB_DoubleFlat_STP;
          break;
        case kAugmentedFourth:
          result = kB_Flat_STP;
          break;
          
        case kDiminishedFifth:
          result = kC_DoubleFlat_STP;
          break;
        case kPerfectFifth:
          result = kC_Flat_STP;
          break;
        case kAugmentedFifth:
          result = kC_Natural_STP;
          break;
    
        case kDiminishedSixth:
          result = kD_TripleFlat_STP;
          break;
        case kMinorSixth:
          result = kD_DoubleFlat_STP;
          break;
        case kMajorSixth:
          result = kD_Flat_STP;
          break;
        case kAugmentedSixth:
          result = kD_Natural_STP;
          break;
          
        case kDiminishedSeventh:
          result = kE_TripleFlat_STP;
          break;
        case kMinorSeventh:
          result = kE_DoubleFlat_STP;
          break;
        case kMajorSeventh:
          result = kE_Flat_STP;
          break;
        case kAugmentedSeventh:
          result = kE_Natural_STP;
          break;
    
        case kDiminishedOctave:
          result = kF_DoubleFlat_STP;
          break;
        case kPerfectOctave:
          result = kF_Flat_STP;
          break;
        case kAugmentedOctave:
          result = kF_Natural_STP;
          break;
          
        case kDiminishedNinth:
          result = kG_TripleFlat_STP;
          break;
        case kMinorNinth:
          result = kG_DoubleFlat_STP;
          break;
        case kMajorNinth:
          result = kG_Flat_STP;
          break;
        case kAugmentedNinth:
          result = kG_Natural_STP;
          break;
          
        case kDiminishedTenth:
          result = kA_TripleFlat_STP;
          break;
        case kMinorTenth:
          result = kA_DoubleFlat_STP;
          break;
        case kMajorTenth:
          result = kA_Flat_STP;
          break;
        case kAugmentedTenth:
          result = kA_Natural_STP;
          break;
          
        case kDiminishedEleventh:
          result = kB_TripleFlat_STP;
          break;
        case kPerfectEleventh:
          result = kB_DoubleFlat_STP;
          break;
        case kAugmentedEleventh:
          result = kB_Flat_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kC_DoubleFlat_STP;
          break;
        case kPerfectTwelfth:
          result = kC_Flat_STP;
          break;
        case kAugmentedTwelfth:
          result = kC_Natural_STP;
          break;
    
        case kDiminishedThirteenth:
          result = kD_TripleFlat_STP;
          break;
        case kMinorThirteenth:
          result = kD_DoubleFlat_STP;
          break;
        case kMajorThirteenth:
          result = kD_Flat_STP;
          break;
        case kAugmentedThirteenth:
          result = kD_Natural_STP;
          break;
      } // switch
      break;
      
    case kF_Natural_STP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kF_Flat_STP;
          break;
        case kPerfectUnison:
          result = kF_Natural_STP;
          break;
        case kAugmentedUnison:
          result = kF_Sharp_STP;
          break;
          
        case kDiminishedSecond:
          result = kG_DoubleFlat_STP;
          break;
        case kMinorSecond:
          result = kG_Flat_STP;
          break;
        case kMajorSecond:
          result = kG_Natural_STP;
          break;
        case kAugmentedSecond:
          result = kG_Sharp_STP;
          break;
          
        case kDiminishedThird:
          result = kA_DoubleFlat_STP;
          break;
        case kMinorThird:
          result = kA_Flat_STP;
          break;
        case kMajorThird:
          result = kA_Natural_STP;
          break;
        case kAugmentedThird:
          result = kA_Sharp_STP;
          break;
          
        case kDiminishedFourth:
          result = kB_DoubleFlat_STP;
          break;
        case kPerfectFourth:
          result = kB_Flat_STP;
          break;
        case kAugmentedFourth:
          result = kB_Natural_STP;
          break;
          
        case kDiminishedFifth:
          result = kC_Flat_STP;
          break;
        case kPerfectFifth:
          result = kC_Natural_STP;
          break;
        case kAugmentedFifth:
          result = kC_Sharp_STP;
          break;
    
        case kDiminishedSixth:
          result = kD_DoubleFlat_STP;
          break;
        case kMinorSixth:
          result = kD_Flat_STP;
          break;
        case kMajorSixth:
          result = kD_Natural_STP;
          break;
        case kAugmentedSixth:
          result = kD_Sharp_STP;
          break;
          
        case kDiminishedSeventh:
          result = kE_DoubleFlat_STP;
          break;
        case kMinorSeventh:
          result = kE_Flat_STP;
          break;
        case kMajorSeventh:
          result = kE_Natural_STP;
          break;
        case kAugmentedSeventh:
          result = kE_Sharp_STP;
          break;
    
        case kDiminishedOctave:
          result = kF_Flat_STP;
          break;
        case kPerfectOctave:
          result = kF_Natural_STP;
          break;
        case kAugmentedOctave:
          result = kF_Sharp_STP;
          break;
          
        case kDiminishedNinth:
          result = kG_DoubleFlat_STP;
          break;
        case kMinorNinth:
          result = kG_Flat_STP;
          break;
        case kMajorNinth:
          result = kG_Natural_STP;
          break;
        case kAugmentedNinth:
          result = kG_Sharp_STP;
          break;
          
        case kDiminishedTenth:
          result = kA_DoubleFlat_STP;
          break;
        case kMinorTenth:
          result = kA_Flat_STP;
          break;
        case kMajorTenth:
          result = kA_Natural_STP;
          break;
        case kAugmentedTenth:
          result = kA_Sharp_STP;
          break;
          
        case kDiminishedEleventh:
          result = kB_DoubleFlat_STP;
          break;
        case kPerfectEleventh:
          result = kB_Flat_STP;
          break;
        case kAugmentedEleventh:
          result = kB_Natural_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kC_Flat_STP;
          break;
        case kPerfectTwelfth:
          result = kC_Natural_STP;
          break;
        case kAugmentedTwelfth:
          result = kC_Sharp_STP;
          break;
    
        case kDiminishedThirteenth:
          result = kD_DoubleFlat_STP;
          break;
        case kMinorThirteenth:
          result = kD_Flat_STP;
          break;
        case kMajorThirteenth:
          result = kD_Natural_STP;
          break;
        case kAugmentedThirteenth:
          result = kD_Sharp_STP;
          break;
      } // switch
      break;
      
    case kF_Sharp_STP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kF_Natural_STP;
          break;
        case kPerfectUnison:
          result = kF_Sharp_STP;
          break;
        case kAugmentedUnison:
          result = kF_DoubleSharp_STP;
          break;
          
        case kDiminishedSecond:
          result = kG_Flat_STP;
          break;
        case kMinorSecond:
          result = kG_Natural_STP;
          break;
        case kMajorSecond:
          result = kG_Sharp_STP;
          break;
        case kAugmentedSecond:
          result = kG_DoubleSharp_STP;
          break;
          
        case kDiminishedThird:
          result = kA_Flat_STP;
          break;
        case kMinorThird:
          result = kA_Natural_STP;
          break;
        case kMajorThird:
          result = kA_Sharp_STP;
          break;
        case kAugmentedThird:
          result = kA_DoubleSharp_STP;
          break;
          
        case kDiminishedFourth:
          result = kB_Sharp_STP;
          break;
        case kPerfectFourth:
          result = kB_DoubleSharp_STP;
          break;
        case kAugmentedFourth:
          result = kB_TripleSharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kC_Natural_STP;
          break;
        case kPerfectFifth:
          result = kC_Sharp_STP;
          break;
        case kAugmentedFifth:
          result = kC_DoubleSharp_STP;
          break;
    
        case kDiminishedSixth:
          result = kD_Flat_STP;
          break;
        case kMinorSixth:
          result = kD_Natural_STP;
          break;
        case kMajorSixth:
          result = kD_Sharp_STP;
          break;
        case kAugmentedSixth:
          result = kD_DoubleSharp_STP;
          break;
          
        case kDiminishedSeventh:
          result = kE_Flat_STP;
          break;
        case kMinorSeventh:
          result = kE_Natural_STP;
          break;
        case kMajorSeventh:
          result = kE_Sharp_STP;
          break;
        case kAugmentedSeventh:
          result = kE_DoubleSharp_STP;
          break;
    
        case kDiminishedOctave:
          result = kF_Natural_STP;
          break;
        case kPerfectOctave:
          result = kF_Sharp_STP;
          break;
        case kAugmentedOctave:
          result = kF_DoubleSharp_STP;
          break;
          
        case kDiminishedNinth:
          result = kG_Flat_STP;
          break;
        case kMinorNinth:
          result = kG_Natural_STP;
          break;
        case kMajorNinth:
          result = kG_Sharp_STP;
          break;
        case kAugmentedNinth:
          result = kG_DoubleSharp_STP;
          break;
          
        case kDiminishedTenth:
          result = kA_Natural_STP;
          break;
        case kMinorTenth:
          result = kA_Natural_STP;
          break;
        case kMajorTenth:
          result = kA_Sharp_STP;
          break;
        case kAugmentedTenth:
          result = kA_DoubleSharp_STP;
          break;
          
        case kDiminishedEleventh:
          result = kB_Sharp_STP;
          break;
        case kPerfectEleventh:
          result = kB_DoubleSharp_STP;
          break;
        case kAugmentedEleventh:
          result = kB_TripleSharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kC_Natural_STP;
          break;
        case kPerfectTwelfth:
          result = kC_Sharp_STP;
          break;
        case kAugmentedTwelfth:
          result = kC_DoubleSharp_STP;
          break;
    
        case kDiminishedThirteenth:
          result = kE_Flat_STP;
          break;
        case kMinorThirteenth:
          result = kD_Natural_STP;
          break;
        case kMajorThirteenth:
          result = kD_Sharp_STP;
          break;
        case kAugmentedThirteenth:
          result = kD_DoubleSharp_STP;
          break;
      } // switch
      break;

    case kG_Flat_STP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kG_DoubleFlat_STP;
          break;
        case kPerfectUnison:
          result = kG_Flat_STP;
          break;
        case kAugmentedUnison:
          result = kG_Natural_STP;
          break;
          
        case kDiminishedSecond:
          result = kA_TripleFlat_STP;
          break;
        case kMinorSecond:
          result = kA_DoubleFlat_STP;
          break;
        case kMajorSecond:
          result = kA_Flat_STP;
          break;
        case kAugmentedSecond:
          result = kA_Natural_STP;
          break;
          
        case kDiminishedThird:
          result = kB_TripleFlat_STP;
          break;
        case kMinorThird:
          result = kB_DoubleFlat_STP;
          break;
        case kMajorThird:
          result = kB_Flat_STP;
          break;
        case kAugmentedThird:
          result = kB_Natural_STP;
          break;
          
        case kDiminishedFourth:
          result = kC_DoubleFlat_STP;
          break;
        case kPerfectFourth:
          result = kC_Flat_STP;
          break;
        case kAugmentedFourth:
          result = kC_Natural_STP;
          break;
          
        case kDiminishedFifth:
          result = kD_DoubleFlat_STP;
          break;
        case kPerfectFifth:
          result = kD_Flat_STP;
          break;
        case kAugmentedFifth:
          result = kD_Natural_STP;
          break;
    
        case kDiminishedSixth:
          result = kE_TripleFlat_STP;
          break;
        case kMinorSixth:
          result = kE_DoubleFlat_STP;
          break;
        case kMajorSixth:
          result = kE_Flat_STP;
          break;
        case kAugmentedSixth:
          result = kE_Natural_STP;
          break;
          
        case kDiminishedSeventh:
          result = kF_DoubleFlat_STP;
          break;
        case kMinorSeventh:
          result = kF_Flat_STP;
          break;
        case kMajorSeventh:
          result = kF_Natural_STP;
          break;
        case kAugmentedSeventh:
          result = kF_Sharp_STP;
          break;
    
        case kDiminishedOctave:
          result = kG_DoubleFlat_STP;
          break;
        case kPerfectOctave:
          result = kG_Flat_STP;
          break;
        case kAugmentedOctave:
          result = kG_Natural_STP;
          break;
          
        case kDiminishedNinth:
          result = kA_TripleFlat_STP;
          break;
        case kMinorNinth:
          result = kA_DoubleFlat_STP;
          break;
        case kMajorNinth:
          result = kA_Flat_STP;
          break;
        case kAugmentedNinth:
          result = kA_Natural_STP;
          break;
          
        case kDiminishedTenth:
          result = kB_TripleFlat_STP;
          break;
        case kMinorTenth:
          result = kB_DoubleFlat_STP;
          break;
        case kMajorTenth:
          result = kB_Flat_STP;
          break;
        case kAugmentedTenth:
          result = kB_Natural_STP;
          break;
          
        case kDiminishedEleventh:
          result = kC_DoubleFlat_STP;
          break;
        case kPerfectEleventh:
          result = kC_Flat_STP;
          break;
        case kAugmentedEleventh:
          result = kC_Natural_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kD_DoubleFlat_STP;
          break;
        case kPerfectTwelfth:
          result = kD_Flat_STP;
          break;
        case kAugmentedTwelfth:
          result = kD_Natural_STP;
          break;
    
        case kDiminishedThirteenth:
          result = kE_TripleFlat_STP;
          break;
        case kMinorThirteenth:
          result = kE_DoubleFlat_STP;
          break;
        case kMajorThirteenth:
          result = kE_Flat_STP;
          break;
        case kAugmentedThirteenth:
          result = kE_Natural_STP;
          break;
      } // switch
      break;
      
    case kG_Natural_STP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kG_Flat_STP;
          break;
        case kPerfectUnison:
          result = kG_Natural_STP;
          break;
        case kAugmentedUnison:
          result = kG_Sharp_STP;
          break;
          
        case kDiminishedSecond:
          result = kA_DoubleFlat_STP;
          break;
        case kMinorSecond:
          result = kA_Flat_STP;
          break;
        case kMajorSecond:
          result = kA_Natural_STP;
          break;
        case kAugmentedSecond:
          result = kA_Sharp_STP;
          break;
          
        case kDiminishedThird:
          result = kB_DoubleFlat_STP;
          break;
        case kMinorThird:
          result = kB_Flat_STP;
          break;
        case kMajorThird:
          result = kB_Natural_STP;
          break;
        case kAugmentedThird:
          result = kB_Sharp_STP;
          break;
          
        case kDiminishedFourth:
          result = kC_Flat_STP;
          break;
        case kPerfectFourth:
          result = kC_Natural_STP;
          break;
        case kAugmentedFourth:
          result = kC_Sharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kD_Flat_STP;
          break;
        case kPerfectFifth:
          result = kD_Natural_STP;
          break;
        case kAugmentedFifth:
          result = kD_Sharp_STP;
          break;
    
        case kDiminishedSixth:
          result = kE_DoubleFlat_STP;
          break;
        case kMinorSixth:
          result = kE_Flat_STP;
          break;
        case kMajorSixth:
          result = kE_Natural_STP;
          break;
        case kAugmentedSixth:
          result = kE_Sharp_STP;
          break;
          
        case kDiminishedSeventh:
          result = kF_Flat_STP;
          break;
        case kMinorSeventh:
          result = kF_Natural_STP;
          break;
        case kMajorSeventh:
          result = kF_Sharp_STP;
          break;
        case kAugmentedSeventh:
          result = kF_DoubleSharp_STP;
          break;
    
        case kDiminishedOctave:
          result = kG_Flat_STP;
          break;
        case kPerfectOctave:
          result = kG_Natural_STP;
          break;
        case kAugmentedOctave:
          result = kG_Sharp_STP;
          break;
          
        case kDiminishedNinth:
          result = kA_DoubleFlat_STP;
          break;
        case kMinorNinth:
          result = kA_Flat_STP;
          break;
        case kMajorNinth:
          result = kA_Natural_STP;
          break;
        case kAugmentedNinth:
          result = kA_Sharp_STP;
          break;
          
        case kDiminishedTenth:
          result = kB_DoubleFlat_STP;
          break;
        case kMinorTenth:
          result = kB_Flat_STP;
          break;
        case kMajorTenth:
          result = kB_Natural_STP;
          break;
        case kAugmentedTenth:
          result = kB_Sharp_STP;
          break;
          
        case kDiminishedEleventh:
          result = kC_Flat_STP;
          break;
        case kPerfectEleventh:
          result = kC_Natural_STP;
          break;
        case kAugmentedEleventh:
          result = kC_Sharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kD_Flat_STP;
          break;
        case kPerfectTwelfth:
          result = kD_Natural_STP;
          break;
        case kAugmentedTwelfth:
          result = kD_Sharp_STP;
          break;
    
        case kDiminishedThirteenth:
          result = kE_DoubleFlat_STP;
          break;
        case kMinorThirteenth:
          result = kE_Flat_STP;
          break;
        case kMajorThirteenth:
          result = kE_Natural_STP;
          break;
        case kAugmentedThirteenth:
          result = kE_Sharp_STP;
          break;
      } // switch
      break;
      
    case kG_Sharp_STP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kG_Natural_STP;
          break;
        case kPerfectUnison:
          result = kG_Sharp_STP;
          break;
        case kAugmentedUnison:
          result = kG_DoubleSharp_STP;
          break;
          
        case kDiminishedSecond:
          result = kA_Flat_STP;
          break;
        case kMinorSecond:
          result = kA_Natural_STP;
          break;
        case kMajorSecond:
          result = kA_Sharp_STP;
          break;
        case kAugmentedSecond:
          result = kA_DoubleSharp_STP;
          break;
          
        case kDiminishedThird:
          result = kB_Flat_STP;
          break;
        case kMinorThird:
          result = kB_Natural_STP;
          break;
        case kMajorThird:
          result = kB_Sharp_STP;
          break;
        case kAugmentedThird:
          result = kB_DoubleSharp_STP;
          break;
          
        case kDiminishedFourth:
          result = kC_Natural_STP;
          break;
        case kPerfectFourth:
          result = kC_Sharp_STP;
          break;
        case kAugmentedFourth:
          result = kC_DoubleSharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kD_Natural_STP;
          break;
        case kPerfectFifth:
          result = kD_Sharp_STP;
          break;
        case kAugmentedFifth:
          result = kD_DoubleSharp_STP;
          break;
    
        case kDiminishedSixth:
          result = kE_Flat_STP;
          break;
        case kMinorSixth:
          result = kE_Natural_STP;
          break;
        case kMajorSixth:
          result = kE_Sharp_STP;
          break;
        case kAugmentedSixth:
          result = kE_DoubleSharp_STP;
          break;
          
        case kDiminishedSeventh:
          result = kF_Natural_STP;
          break;
        case kMinorSeventh:
          result = kF_Sharp_STP;
          break;
        case kMajorSeventh:
          result = kF_DoubleSharp_STP;
          break;
        case kAugmentedSeventh:
          result = kF_TripleSharp_STP;
          break;
    
        case kDiminishedOctave:
          result = kG_Natural_STP;
          break;
        case kPerfectOctave:
          result = kG_Sharp_STP;
          break;
        case kAugmentedOctave:
          result = kG_DoubleSharp_STP;
          break;
          
        case kDiminishedNinth:
          result = kA_Flat_STP;
          break;
        case kMinorNinth:
          result = kA_Natural_STP;
          break;
        case kMajorNinth:
          result = kA_Sharp_STP;
          break;
        case kAugmentedNinth:
          result = kA_DoubleSharp_STP;
          break;
          
        case kDiminishedTenth:
          result = kB_Flat_STP;
          break;
        case kMinorTenth:
          result = kB_Natural_STP;
          break;
        case kMajorTenth:
          result = kB_Sharp_STP;
          break;
        case kAugmentedTenth:
          result = kB_DoubleSharp_STP;
          break;
          
        case kDiminishedEleventh:
          result = kC_Natural_STP;
          break;
        case kPerfectEleventh:
          result = kC_Sharp_STP;
          break;
        case kAugmentedEleventh:
          result = kC_DoubleSharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kD_Natural_STP;
          break;
        case kPerfectTwelfth:
          result = kD_Sharp_STP;
          break;
        case kAugmentedTwelfth:
          result = kD_DoubleSharp_STP;
          break;
    
        case kDiminishedThirteenth:
          result = kE_Flat_STP;
          break;
        case kMinorThirteenth:
          result = kE_Natural_STP;
          break;
        case kMajorThirteenth:
          result = kE_Sharp_STP;
          break;
        case kAugmentedThirteenth:
          result = kE_DoubleSharp_STP;
          break;
      } // switch
      break;

    case kA_Flat_STP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kA_DoubleFlat_STP;
          break;
        case kPerfectUnison:
          result = kA_Flat_STP;
          break;
        case kAugmentedUnison:
          result = kA_Natural_STP;
          break;
          
        case kDiminishedSecond:
          result = kB_TripleFlat_STP;
          break;
        case kMinorSecond:
          result = kB_DoubleFlat_STP;
          break;
        case kMajorSecond:
          result = kB_Flat_STP;
          break;
        case kAugmentedSecond:
          result = kB_Natural_STP;
          break;
          
        case kDiminishedThird:
          result = kC_DoubleFlat_STP;
          break;
        case kMinorThird:
          result = kC_Flat_STP;
          break;
        case kMajorThird:
          result = kC_Natural_STP;
          break;
        case kAugmentedThird:
          result = kC_Sharp_STP;
          break;
          
        case kDiminishedFourth:
          result = kD_DoubleFlat_STP;
          break;
        case kPerfectFourth:
          result = kD_Flat_STP;
          break;
        case kAugmentedFourth:
          result = kD_Natural_STP;
          break;
          
        case kDiminishedFifth:
          result = kE_DoubleFlat_STP;
          break;
        case kPerfectFifth:
          result = kE_Flat_STP;
          break;
        case kAugmentedFifth:
          result = kE_Natural_STP;
          break;
    
        case kDiminishedSixth:
          result = kF_DoubleFlat_STP;
          break;
        case kMinorSixth:
          result = kF_Flat_STP;
          break;
        case kMajorSixth:
          result = kF_Natural_STP;
          break;
        case kAugmentedSixth:
          result = kF_Sharp_STP;
          break;
          
        case kDiminishedSeventh:
          result = kG_DoubleFlat_STP;
          break;
        case kMinorSeventh:
          result = kG_Flat_STP;
          break;
        case kMajorSeventh:
          result = kG_Natural_STP;
          break;
        case kAugmentedSeventh:
          result = kG_Sharp_STP;
          break;
    
        case kDiminishedOctave:
          result = kA_DoubleFlat_STP;
          break;
        case kPerfectOctave:
          result = kA_Flat_STP;
          break;
        case kAugmentedOctave:
          result = kA_Natural_STP;
          break;
          
        case kDiminishedNinth:
          result = kB_TripleFlat_STP;
          break;
        case kMinorNinth:
          result = kB_DoubleFlat_STP;
          break;
        case kMajorNinth:
          result = kB_Flat_STP;
          break;
        case kAugmentedNinth:
          result = kB_Natural_STP;
          break;
          
        case kDiminishedTenth:
          result = kC_DoubleFlat_STP;
          break;
        case kMinorTenth:
          result = kC_Flat_STP;
          break;
        case kMajorTenth:
          result = kC_Natural_STP;
          break;
        case kAugmentedTenth:
          result = kC_Sharp_STP;
          break;
          
        case kDiminishedEleventh:
          result = kD_DoubleFlat_STP;
          break;
        case kPerfectEleventh:
          result = kD_Flat_STP;
          break;
        case kAugmentedEleventh:
          result = kD_Natural_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kE_DoubleFlat_STP;
          break;
        case kPerfectTwelfth:
          result = kE_Flat_STP;
          break;
        case kAugmentedTwelfth:
          result = kE_Natural_STP;
          break;
    
        case kDiminishedThirteenth:
          result = kF_TripleFlat_STP;
          break;
        case kMinorThirteenth:
          result = kF_DoubleFlat_STP;
          break;
        case kMajorThirteenth:
          result = kF_Flat_STP;
          break;
        case kAugmentedThirteenth:
          result = kF_Natural_STP;
          break;
      } // switch
      break;
      
    case kA_Natural_STP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kA_Flat_STP;
          break;
        case kPerfectUnison:
          result = kA_Natural_STP;
          break;
        case kAugmentedUnison:
          result = kA_Sharp_STP;
          break;
          
        case kDiminishedSecond:
          result = kB_DoubleFlat_STP;
          break;
        case kMinorSecond:
          result = kB_Flat_STP;
          break;
        case kMajorSecond:
          result = kB_Natural_STP;
          break;
        case kAugmentedSecond:
          result = kB_Sharp_STP;
          break;
          
        case kDiminishedThird:
          result = kC_Flat_STP;
          break;
        case kMinorThird:
          result = kC_Natural_STP;
          break;
        case kMajorThird:
          result = kC_Sharp_STP;
          break;
        case kAugmentedThird:
          result = kC_DoubleSharp_STP;
          break;
          
        case kDiminishedFourth:
          result = kD_Flat_STP;
          break;
        case kPerfectFourth:
          result = kD_Natural_STP;
          break;
        case kAugmentedFourth:
          result = kD_Sharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kE_Flat_STP;
          break;
        case kPerfectFifth:
          result = kE_Natural_STP;
          break;
        case kAugmentedFifth:
          result = kE_Sharp_STP;
          break;
    
        case kDiminishedSixth:
          result = kF_Flat_STP;
          break;
        case kMinorSixth:
          result = kF_Natural_STP;
          break;
        case kMajorSixth:
          result = kF_Sharp_STP;
          break;
        case kAugmentedSixth:
          result = kF_DoubleSharp_STP;
          break;
          
        case kDiminishedSeventh:
          result = kG_Flat_STP;
          break;
        case kMinorSeventh:
          result = kG_Natural_STP;
          break;
        case kMajorSeventh:
          result = kG_Sharp_STP;
          break;
        case kAugmentedSeventh:
          result = kG_DoubleSharp_STP;
          break;
    
        case kDiminishedOctave:
          result = kA_Flat_STP;
          break;
        case kPerfectOctave:
          result = kA_Natural_STP;
          break;
        case kAugmentedOctave:
          result = kA_Sharp_STP;
          break;
          
        case kDiminishedNinth:
          result = kB_DoubleFlat_STP;
          break;
        case kMinorNinth:
          result = kB_Flat_STP;
          break;
        case kMajorNinth:
          result = kB_Natural_STP;
          break;
        case kAugmentedNinth:
          result = kB_Sharp_STP;
          break;
          
        case kDiminishedTenth:
          result = kC_Flat_STP;
          break;
        case kMinorTenth:
          result = kC_Natural_STP;
          break;
        case kMajorTenth:
          result = kC_Sharp_STP;
          break;
        case kAugmentedTenth:
          result = kC_DoubleSharp_STP;
          break;
          
        case kDiminishedEleventh:
          result = kD_Flat_STP;
          break;
        case kPerfectEleventh:
          result = kD_Natural_STP;
          break;
        case kAugmentedEleventh:
          result = kD_Sharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kE_Flat_STP;
          break;
        case kPerfectTwelfth:
          result = kE_Natural_STP;
          break;
        case kAugmentedTwelfth:
          result = kE_Sharp_STP;
          break;
    
        case kDiminishedThirteenth:
          result = kF_Flat_STP;
          break;
        case kMinorThirteenth:
          result = kF_Natural_STP;
          break;
        case kMajorThirteenth:
          result = kF_Sharp_STP;
          break;
        case kAugmentedThirteenth:
          result = kF_DoubleSharp_STP;
          break;
      } // switch
      break;
      
    case kA_Sharp_STP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kA_Natural_STP;
          break;
        case kPerfectUnison:
          result = kA_Sharp_STP;
          break;
        case kAugmentedUnison:
          result = kA_DoubleSharp_STP;
          break;
          
        case kDiminishedSecond:
          result = kB_Flat_STP;
          break;
        case kMinorSecond:
          result = kB_Natural_STP;
          break;
        case kMajorSecond:
          result = kB_Sharp_STP;
          break;
        case kAugmentedSecond:
          result = kB_DoubleSharp_STP;
          break;
          
        case kDiminishedThird:
          result = kC_Natural_STP;
          break;
        case kMinorThird:
          result = kC_Sharp_STP;
          break;
        case kMajorThird:
          result = kC_DoubleSharp_STP;
          break;
        case kAugmentedThird:
          result = kC_TripleSharp_STP;
          break;
          
        case kDiminishedFourth:
          result = kD_Natural_STP;
          break;
        case kPerfectFourth:
          result = kD_Sharp_STP;
          break;
        case kAugmentedFourth:
          result = kD_DoubleSharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kE_Natural_STP;
          break;
        case kPerfectFifth:
          result = kE_Sharp_STP;
          break;
        case kAugmentedFifth:
          result = kE_DoubleSharp_STP;
          break;
    
        case kDiminishedSixth:
          result = kF_Natural_STP;
          break;
        case kMinorSixth:
          result = kF_Sharp_STP;
          break;
        case kMajorSixth:
          result = kF_DoubleSharp_STP;
          break;
        case kAugmentedSixth:
          result = kF_TripleSharp_STP;
          break;
          
        case kDiminishedSeventh:
          result = kG_Natural_STP;
          break;
        case kMinorSeventh:
          result = kG_Sharp_STP;
          break;
        case kMajorSeventh:
          result = kG_DoubleSharp_STP;
          break;
        case kAugmentedSeventh:
          result = kG_TripleSharp_STP;
          break;
    
        case kDiminishedOctave:
          result = kA_Natural_STP;
          break;
        case kPerfectOctave:
          result = kA_Sharp_STP;
          break;
        case kAugmentedOctave:
          result = kA_DoubleSharp_STP;
          break;
          
        case kDiminishedNinth:
          result = kB_Flat_STP;
          break;
        case kMinorNinth:
          result = kB_Natural_STP;
          break;
        case kMajorNinth:
          result = kB_Sharp_STP;
          break;
        case kAugmentedNinth:
          result = kB_DoubleSharp_STP;
          break;
          
        case kDiminishedTenth:
          result = kC_Natural_STP;
          break;
        case kMinorTenth:
          result = kC_Sharp_STP;
          break;
        case kMajorTenth:
          result = kC_DoubleSharp_STP;
          break;
        case kAugmentedTenth:
          result = kC_TripleSharp_STP;
          break;
          
        case kDiminishedEleventh:
          result = kD_Natural_STP;
          break;
        case kPerfectEleventh:
          result = kD_Sharp_STP;
          break;
        case kAugmentedEleventh:
          result = kD_DoubleSharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kE_Natural_STP;
          break;
        case kPerfectTwelfth:
          result = kE_Sharp_STP;
          break;
        case kAugmentedTwelfth:
          result = kE_DoubleSharp_STP;
          break;
    
        case kDiminishedThirteenth:
          result = kF_Natural_STP;
          break;
        case kMinorThirteenth:
          result = kF_Sharp_STP;
          break;
        case kMajorThirteenth:
          result = kF_DoubleSharp_STP;
          break;
        case kAugmentedThirteenth:
          result = kF_TripleSharp_STP;
          break;
      } // switch
      break;

    case kB_Flat_STP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kB_DoubleFlat_STP;
          break;
        case kPerfectUnison:
          result = kB_Flat_STP;
          break;
        case kAugmentedUnison:
          result = kB_Natural_STP;
          break;
          
        case kDiminishedSecond:
          result = kC_DoubleFlat_STP;
          break;
        case kMinorSecond:
          result = kC_Flat_STP;
          break;
        case kMajorSecond:
          result = kC_Natural_STP;
          break;
        case kAugmentedSecond:
          result = kC_Sharp_STP;
          break;
          
        case kDiminishedThird:
          result = kD_DoubleFlat_STP;
          break;
        case kMinorThird:
          result = kD_Flat_STP;
          break;
        case kMajorThird:
          result = kD_Natural_STP;
          break;
        case kAugmentedThird:
          result = kD_Sharp_STP;
          break;
          
        case kDiminishedFourth:
          result = kE_DoubleFlat_STP;
          break;
        case kPerfectFourth:
          result = kE_Flat_STP;
          break;
        case kAugmentedFourth:
          result = kE_Natural_STP;
          break;
          
        case kDiminishedFifth:
          result = kF_Flat_STP;
          break;
        case kPerfectFifth:
          result = kF_Natural_STP;
          break;
        case kAugmentedFifth:
          result = kF_Sharp_STP;
          break;
    
        case kDiminishedSixth:
          result = kG_DoubleFlat_STP;
          break;
        case kMinorSixth:
          result = kG_Flat_STP;
          break;
        case kMajorSixth:
          result = kG_Natural_STP;
          break;
        case kAugmentedSixth:
          result = kG_Sharp_STP;
          break;
          
        case kDiminishedSeventh:
          result = kA_DoubleFlat_STP;
          break;
        case kMinorSeventh:
          result = kA_Flat_STP;
          break;
        case kMajorSeventh:
          result = kA_Natural_STP;
          break;
        case kAugmentedSeventh:
          result = kA_Sharp_STP;
          break;
    
        case kDiminishedOctave:
          result = kB_DoubleFlat_STP;
          break;
        case kPerfectOctave:
          result = kB_Flat_STP;
          break;
        case kAugmentedOctave:
          result = kB_Natural_STP;
          break;
          
        case kDiminishedNinth:
          result = kC_DoubleFlat_STP;
          break;
        case kMinorNinth:
          result = kC_Flat_STP;
          break;
        case kMajorNinth:
          result = kC_Natural_STP;
          break;
        case kAugmentedNinth:
          result = kC_Sharp_STP;
          break;
          
        case kDiminishedTenth:
          result = kD_DoubleFlat_STP;
          break;
        case kMinorTenth:
          result = kD_Flat_STP;
          break;
        case kMajorTenth:
          result = kD_Natural_STP;
          break;
        case kAugmentedTenth:
          result = kD_Sharp_STP;
          break;
          
        case kDiminishedEleventh:
          result = kE_DoubleFlat_STP;
          break;
        case kPerfectEleventh:
          result = kE_Flat_STP;
          break;
        case kAugmentedEleventh:
          result = kE_Natural_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kF_Flat_STP;
          break;
        case kPerfectTwelfth:
          result = kF_Natural_STP;
          break;
        case kAugmentedTwelfth:
          result = kF_Sharp_STP;
          break;
    
        case kDiminishedThirteenth:
          result = kG_DoubleFlat_STP;
          break;
        case kMinorThirteenth:
          result = kG_Flat_STP;
          break;
        case kMajorThirteenth:
          result = kG_Natural_STP;
          break;
        case kAugmentedThirteenth:
          result = kG_Sharp_STP;
          break;
      } // switch
      break;

    case kB_Natural_STP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kB_Flat_STP;
          break;
        case kPerfectUnison:
          result = kB_Natural_STP;
          break;
        case kAugmentedUnison:
          result = kB_Sharp_STP;
          break;
          
        case kDiminishedSecond:
          result = kC_Flat_STP;
          break;
        case kMinorSecond:
          result = kC_Natural_STP;
          break;
        case kMajorSecond:
          result = kC_Sharp_STP;
          break;
        case kAugmentedSecond:
          result = kC_DoubleSharp_STP;
          break;
          
        case kDiminishedThird:
          result = kD_Flat_STP;
          break;
        case kMinorThird:
          result = kD_Natural_STP;
          break;
        case kMajorThird:
          result = kD_Sharp_STP;
          break;
        case kAugmentedThird:
          result = kD_DoubleSharp_STP;
          break;
          
        case kDiminishedFourth:
          result = kE_Flat_STP;
          break;
        case kPerfectFourth:
          result = kE_Natural_STP;
          break;
        case kAugmentedFourth:
          result = kE_Sharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kF_Natural_STP;
          break;
        case kPerfectFifth:
          result = kF_Sharp_STP;
          break;
        case kAugmentedFifth:
          result = kF_DoubleSharp_STP;
          break;
    
        case kDiminishedSixth:
          result = kG_Flat_STP;
          break;
        case kMinorSixth:
          result = kG_Natural_STP;
          break;
        case kMajorSixth:
          result = kG_Sharp_STP;
          break;
        case kAugmentedSixth:
          result = kG_DoubleSharp_STP;
          break;
          
        case kDiminishedSeventh:
          result = kA_Flat_STP;
          break;
        case kMinorSeventh:
          result = kA_Natural_STP;
          break;
        case kMajorSeventh:
          result = kA_Sharp_STP;
          break;
        case kAugmentedSeventh:
          result = kA_DoubleSharp_STP;
          break;
    
        case kDiminishedOctave:
          result = kB_Flat_STP;
          break;
        case kPerfectOctave:
          result = kB_Natural_STP;
          break;
        case kAugmentedOctave:
          result = kB_Sharp_STP;
          break;
          
        case kDiminishedNinth:
          result = kC_Flat_STP;
          break;
        case kMinorNinth:
          result = kC_Natural_STP;
          break;
        case kMajorNinth:
          result = kC_Sharp_STP;
          break;
        case kAugmentedNinth:
          result = kC_DoubleSharp_STP;
          break;
          
        case kDiminishedTenth:
          result = kD_Flat_STP;
          break;
        case kMinorTenth:
          result = kD_Natural_STP;
          break;
        case kMajorTenth:
          result = kD_Sharp_STP;
          break;
        case kAugmentedTenth:
          result = kD_DoubleSharp_STP;
          break;
          
        case kDiminishedEleventh:
          result = kE_Flat_STP;
          break;
        case kPerfectEleventh:
          result = kE_Natural_STP;
          break;
        case kAugmentedEleventh:
          result = kE_Sharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kF_Natural_STP;
          break;
        case kPerfectTwelfth:
          result = kF_Sharp_STP;
          break;
        case kAugmentedTwelfth:
          result = kF_DoubleSharp_STP;
          break;
    
        case kDiminishedThirteenth:
          result = kG_Flat_STP;
          break;
        case kMinorThirteenth:
          result = kG_Natural_STP;
          break;
        case kMajorThirteenth:
          result = kG_Sharp_STP;
          break;
        case kAugmentedThirteenth:
          result = kG_DoubleSharp_STP;
          break;
      } // switch
      break;

    case kB_Sharp_STP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kB_Natural_STP;
          break;
        case kPerfectUnison:
          result = kB_Sharp_STP;
          break;
        case kAugmentedUnison:
          result = kB_DoubleSharp_STP;
          break;
          
        case kDiminishedSecond:
          result = kC_Natural_STP;
          break;
        case kMinorSecond:
          result = kC_Sharp_STP;
          break;
        case kMajorSecond:
          result = kC_DoubleSharp_STP;
          break;
        case kAugmentedSecond:
          result = kC_TripleSharp_STP;
          break;
          
        case kDiminishedThird:
          result = kD_Natural_STP;
          break;
        case kMinorThird:
          result = kD_Sharp_STP;
          break;
        case kMajorThird:
          result = kD_DoubleSharp_STP;
          break;
        case kAugmentedThird:
          result = kD_TripleSharp_STP;
          break;
          
        case kDiminishedFourth:
          result = kE_Natural_STP;
          break;
        case kPerfectFourth:
          result = kE_Sharp_STP;
          break;
        case kAugmentedFourth:
          result = kE_DoubleSharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kF_Sharp_STP;
          break;
        case kPerfectFifth:
          result = kF_DoubleSharp_STP;
          break;
        case kAugmentedFifth:
          result = kF_TripleSharp_STP;
          break;
    
        case kDiminishedSixth:
          result = kG_Natural_STP;
          break;
        case kMinorSixth:
          result = kG_Sharp_STP;
          break;
        case kMajorSixth:
          result = kG_DoubleSharp_STP;
          break;
        case kAugmentedSixth:
          result = kG_TripleSharp_STP;
          break;
          
        case kDiminishedSeventh:
          result = kA_Natural_STP;
          break;
        case kMinorSeventh:
          result = kA_Sharp_STP;
          break;
        case kMajorSeventh:
          result = kA_DoubleSharp_STP;
          break;
        case kAugmentedSeventh:
          result = kA_TripleSharp_STP;
          break;
    
        case kDiminishedOctave:
          result = kB_Natural_STP;
          break;
        case kPerfectOctave:
          result = kB_Sharp_STP;
          break;
        case kAugmentedOctave:
          result = kB_DoubleSharp_STP;
          break;
          
        case kDiminishedNinth:
          result = kC_Natural_STP;
          break;
        case kMinorNinth:
          result = kC_Sharp_STP;
          break;
        case kMajorNinth:
          result = kC_DoubleSharp_STP;
          break;
        case kAugmentedNinth:
          result = kC_TripleSharp_STP;
          break;
          
        case kDiminishedTenth:
          result = kD_Natural_STP;
          break;
        case kMinorTenth:
          result = kD_Sharp_STP;
          break;
        case kMajorTenth:
          result = kD_DoubleSharp_STP;
          break;
        case kAugmentedTenth:
          result = kD_TripleSharp_STP;
          break;
          
        case kDiminishedEleventh:
          result = kE_Natural_STP;
          break;
        case kPerfectEleventh:
          result = kE_Sharp_STP;
          break;
        case kAugmentedEleventh:
          result = kE_DoubleSharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kF_Sharp_STP;
          break;
        case kPerfectTwelfth:
          result = kF_DoubleSharp_STP;
          break;
        case kAugmentedTwelfth:
          result = kF_TripleSharp_STP;
          break;
    
        case kDiminishedThirteenth:
          result = kG_Natural_STP;
          break;
        case kMinorThirteenth:
          result = kG_Sharp_STP;
          break;
        case kMajorThirteenth:
          result = kG_DoubleSharp_STP;
          break;
        case kAugmentedThirteenth:
          result = kG_TripleSharp_STP;
          break;
      } // switch
      break;

      default:
      {
        // computing intervals on double or triple flats/sharps
        // is not considered useful, hence not supported
        stringstream s;

        s <<
          "Sorry, computing intervals from semitones pitch '" <<
          msrSemiTonesPitchKindAsString (semiTonesPitchKind) <<
          "(" << semiTonesPitchKind << ")" <<
          "' is not supported"
          ", line = " << inputLineNumber;

        msrLimitation (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
  } // switch

  return result;
}

msrQuarterTonesPitchKind noteAtIntervalFromQuarterTonesPitch (
  int                      inputLineNumber,
  msrIntervalKind          intervalKind,
  msrQuarterTonesPitchKind quarterTonesPitchKind)
{
  msrQuarterTonesPitchKind result = k_NoQuarterTonesPitch_QTP;

  switch (quarterTonesPitchKind) {
    case k_NoQuarterTonesPitch_QTP:
      result = k_NoQuarterTonesPitch_QTP;
      break;
          
    case kC_Flat_QTP:
      break;

    case kC_Natural_QTP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kC_Flat_QTP;
          break;
        case kPerfectUnison:
          result = kC_Natural_QTP;
          break;
        case kAugmentedUnison:
          result = kC_Sharp_QTP;
          break;
          
        case kDiminishedSecond:
          result = kD_DoubleFlat_QTP;
          break;
        case kMinorSecond:
          result = kD_Flat_QTP;
          break;
        case kMajorSecond:
          result = kD_Natural_QTP;
          break;
        case kAugmentedSecond:
          result = kD_Sharp_QTP;
          break;
          
        case kDiminishedThird:
          result = kE_DoubleFlat_QTP;
          break;
        case kMinorThird:
          result = kE_Flat_QTP;
          break;
        case kMajorThird:
          result = kE_Natural_QTP;
          break;
        case kAugmentedThird:
          result = kE_Sharp_QTP;
          break;
          
        case kDiminishedFourth:
          result = kF_Flat_QTP;
          break;
        case kPerfectFourth:
          result = kF_Natural_QTP;
          break;
        case kAugmentedFourth:
          result = kF_Sharp_QTP;
          break;
          
        case kDiminishedFifth:
          result = kG_Flat_QTP;
          break;
        case kPerfectFifth:
          result = kG_Natural_QTP;
          break;
        case kAugmentedFifth:
          result = kG_Sharp_QTP;
          break;
    
        case kDiminishedSixth:
          result = kA_DoubleFlat_QTP;
          break;
        case kMinorSixth:
          result = kA_Flat_QTP;
          break;
        case kMajorSixth:
          result = kA_Natural_QTP;
          break;
        case kAugmentedSixth:
          result = kA_Sharp_QTP;
          break;
          
        case kDiminishedSeventh:
          result = kB_DoubleFlat_QTP;
          break;
        case kMinorSeventh:
          result = kB_Flat_QTP;
          break;
        case kMajorSeventh:
          result = kB_Natural_QTP;
          break;
        case kAugmentedSeventh:
          result = kB_Sharp_QTP;
          break;
    
        case kDiminishedOctave:
          result = kC_Flat_QTP;
          break;
        case kPerfectOctave:
          result = kC_Natural_QTP;
          break;
        case kAugmentedOctave:
          result = kC_Sharp_QTP;
          break;
          
        case kDiminishedNinth:
          result = kD_DoubleFlat_QTP;
          break;
        case kMinorNinth:
          result = kD_Flat_QTP;
          break;
        case kMajorNinth:
          result = kD_Natural_QTP;
          break;
        case kAugmentedNinth:
          result = kD_Sharp_QTP;
          break;
          
        case kDiminishedTenth:
          result = kE_DoubleFlat_QTP;
          break;
        case kMinorTenth:
          result = kE_Flat_QTP;
          break;
        case kMajorTenth:
          result = kE_Natural_QTP;
          break;
        case kAugmentedTenth:
          result = kE_Sharp_QTP;
          break;
          
        case kDiminishedEleventh:
          result = kF_Flat_QTP;
          break;
        case kPerfectEleventh:
          result = kF_Natural_QTP;
          break;
        case kAugmentedEleventh:
          result = kF_Sharp_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kG_Flat_QTP;
          break;
        case kPerfectTwelfth:
          result = kG_Natural_QTP;
          break;
        case kAugmentedTwelfth:
          result = kG_Sharp_QTP;
          break;
    
        case kDiminishedThirteenth:
          result = kA_DoubleFlat_QTP;
          break;
        case kMinorThirteenth:
          result = kA_Flat_QTP;
          break;
        case kMajorThirteenth:
          result = kA_Natural_QTP;
          break;
        case kAugmentedThirteenth:
          result = kG_Sharp_QTP;
          break;
      } // switch
      break;
      
    case kC_Sharp_QTP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kC_Natural_QTP;
          break;
        case kPerfectUnison:
          result = kC_Sharp_QTP;
          break;
        case kAugmentedUnison:
          result = kC_DoubleSharp_QTP;
          break;
          
        case kDiminishedSecond:
          result = kD_Flat_QTP;
          break;
        case kMinorSecond:
          result = kD_Natural_QTP;
          break;
        case kMajorSecond:
          result = kD_Sharp_QTP;
          break;
        case kAugmentedSecond:
          result = kD_DoubleSharp_QTP;
          break;
          
        case kDiminishedThird:
          result = kE_Flat_QTP;
          break;
        case kMinorThird:
          result = kE_Natural_QTP;
          break;
        case kMajorThird:
          result = kE_Sharp_QTP;
          break;
        case kAugmentedThird:
          result = kE_DoubleSharp_QTP;
          break;
          
        case kDiminishedFourth:
          result = kF_Natural_QTP;
          break;
        case kPerfectFourth:
          result = kF_Sharp_QTP;
          break;
        case kAugmentedFourth:
          result = kF_DoubleSharp_QTP;
          break;
          
        case kDiminishedFifth:
          result = kG_Natural_QTP;
          break;
        case kPerfectFifth:
          result = kG_Sharp_QTP;
          break;
        case kAugmentedFifth:
          result = kG_DoubleSharp_QTP;
          break;
    
        case kDiminishedSixth:
          result = kA_Flat_QTP;
          break;
        case kMinorSixth:
          result = kA_Natural_QTP;
          break;
        case kMajorSixth:
          result = kA_Sharp_QTP;
          break;
        case kAugmentedSixth:
          result = kA_DoubleSharp_QTP;
          break;
          
        case kDiminishedSeventh:
          result = kB_Flat_QTP;
          break;
        case kMinorSeventh:
          result = kB_Natural_QTP;
          break;
        case kMajorSeventh:
          result = kB_Sharp_QTP;
          break;
        case kAugmentedSeventh:
          result = kB_DoubleSharp_QTP;
          break;
    
        case kDiminishedOctave:
          result = kC_Natural_QTP;
          break;
        case kPerfectOctave:
          result = kC_Sharp_QTP;
          break;
        case kAugmentedOctave:
          result = kC_DoubleSharp_QTP;
          break;
          
        case kDiminishedNinth:
          result = kD_Flat_QTP;
          break;
        case kMinorNinth:
          result = kD_Natural_QTP;
          break;
        case kMajorNinth:
          result = kD_Sharp_QTP;
          break;
        case kAugmentedNinth:
          result = kD_DoubleSharp_QTP;
          break;
          
        case kDiminishedTenth:
          result = kE_Flat_QTP;
          break;
        case kMinorTenth:
          result = kE_Natural_QTP;
          break;
        case kMajorTenth:
          result = kE_Sharp_QTP;
          break;
        case kAugmentedTenth:
          result = kE_DoubleSharp_QTP;
          break;
          
        case kDiminishedEleventh:
          result = kF_Natural_QTP;
          break;
        case kPerfectEleventh:
          result = kF_Sharp_QTP;
          break;
        case kAugmentedEleventh:
          result = kF_DoubleSharp_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kG_Natural_QTP;
          break;
        case kPerfectTwelfth:
          result = kG_Sharp_QTP;
          break;
        case kAugmentedTwelfth:
          result = kG_DoubleSharp_QTP;
          break;
    
        case kDiminishedThirteenth:
          result = kA_Flat_QTP;
          break;
        case kMinorThirteenth:
          result = kA_Natural_QTP;
          break;
        case kMajorThirteenth:
          result = kA_Sharp_QTP;
          break;
        case kAugmentedThirteenth:
          result = kA_DoubleSharp_QTP;
          break;
      } // switch
      break;

    case kD_Flat_QTP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kD_DoubleFlat_QTP;
          break;
        case kPerfectUnison:
          result = kD_Flat_QTP;
          break;
        case kAugmentedUnison:
          result = kD_Natural_QTP;
          break;
          
        case kDiminishedSecond:
          result = kE_TripleFlat_QTP;
          break;
        case kMinorSecond:
          result = kE_DoubleFlat_QTP;
          break;
        case kMajorSecond:
          result = kE_Flat_QTP;
          break;
        case kAugmentedSecond:
          result = kE_Natural_QTP;
          break;
          
        case kDiminishedThird:
          result = kF_DoubleFlat_QTP;
          break;
        case kMinorThird:
          result = kF_Flat_QTP;
          break;
        case kMajorThird:
          result = kF_Natural_QTP;
          break;
        case kAugmentedThird:
          result = kF_Sharp_QTP;
          break;
          
        case kDiminishedFourth:
          result = kG_DoubleFlat_QTP;
          break;
        case kPerfectFourth:
          result = kG_Flat_QTP;
          break;
        case kAugmentedFourth:
          result = kG_Natural_QTP;
          break;
          
        case kDiminishedFifth:
          result = kA_DoubleFlat_QTP;
          break;
        case kPerfectFifth:
          result = kA_Flat_QTP;
          break;
        case kAugmentedFifth:
          result = kA_Natural_QTP;
          break;
    
        case kDiminishedSixth:
          result = kB_TripleFlat_QTP;
          break;
        case kMinorSixth:
          result = kB_DoubleFlat_QTP;
          break;
        case kMajorSixth:
          result = kB_Flat_QTP;
          break;
        case kAugmentedSixth:
          result = kB_Natural_QTP;
          break;
          
        case kDiminishedSeventh:
          result = kC_DoubleFlat_QTP;
          break;
        case kMinorSeventh:
          result = kC_Flat_QTP;
          break;
        case kMajorSeventh:
          result = kC_Natural_QTP;
          break;
        case kAugmentedSeventh:
          result = kC_Sharp_QTP;
          break;
    
        case kDiminishedOctave:
          result = kD_DoubleFlat_QTP;
          break;
        case kPerfectOctave:
          result = kD_Flat_QTP;
          break;
        case kAugmentedOctave:
          result = kD_Natural_QTP;
          break;
          
        case kDiminishedNinth:
          result = kE_TripleFlat_QTP;
          break;
        case kMinorNinth:
          result = kE_DoubleFlat_QTP;
          break;
        case kMajorNinth:
          result = kE_Flat_QTP;
          break;
        case kAugmentedNinth:
          result = kE_Natural_QTP;
          break;
          
        case kDiminishedTenth:
          result = kF_DoubleFlat_QTP;
          break;
        case kMinorTenth:
          result = kF_Flat_QTP;
          break;
        case kMajorTenth:
          result = kF_Natural_QTP;
          break;
        case kAugmentedTenth:
          result = kF_Sharp_QTP;
          break;
          
        case kDiminishedEleventh:
          result = kG_DoubleFlat_QTP;
          break;
        case kPerfectEleventh:
          result = kG_Flat_QTP;
          break;
        case kAugmentedEleventh:
          result = kG_Natural_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kA_DoubleFlat_QTP;
          break;
        case kPerfectTwelfth:
          result = kA_Flat_QTP;
          break;
        case kAugmentedTwelfth:
          result = kA_Natural_QTP;
          break;
    
        case kDiminishedThirteenth:
          result = kB_TripleFlat_QTP;
          break;
        case kMinorThirteenth:
          result = kB_DoubleFlat_QTP;
          break;
        case kMajorThirteenth:
          result = kB_Flat_QTP;
          break;
        case kAugmentedThirteenth:
          result = kB_Natural_QTP;
          break;
      } // switch
      break;

    case kD_Natural_QTP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kD_Flat_QTP;
          break;
        case kPerfectUnison:
          result = kD_Natural_QTP;
          break;
        case kAugmentedUnison:
          result = kD_Sharp_QTP;
          break;
          
        case kDiminishedSecond:
          result = kE_DoubleFlat_QTP;
          break;
        case kMinorSecond:
          result = kE_Flat_QTP;
          break;
        case kMajorSecond:
          result = kE_Natural_QTP;
          break;
        case kAugmentedSecond:
          result = kE_Sharp_QTP;
          break;
          
        case kDiminishedThird:
          result = kF_Flat_QTP;
          break;
        case kMinorThird:
          result = kF_Natural_QTP;
          break;
        case kMajorThird:
          result = kF_Sharp_QTP;
          break;
        case kAugmentedThird:
          result = kF_DoubleSharp_QTP;
          break;
          
        case kDiminishedFourth:
          result = kG_Flat_QTP;
          break;
        case kPerfectFourth:
          result = kG_Natural_QTP;
          break;
        case kAugmentedFourth:
          result = kG_Sharp_QTP;
          break;
          
        case kDiminishedFifth:
          result = kA_Flat_QTP;
          break;
        case kPerfectFifth:
          result = kA_Natural_QTP;
          break;
        case kAugmentedFifth:
          result = kA_Sharp_QTP;
          break;
    
        case kDiminishedSixth:
          result = kB_DoubleFlat_QTP;
          break;
        case kMinorSixth:
          result = kB_Flat_QTP;
          break;
        case kMajorSixth:
          result = kB_Natural_QTP;
          break;
        case kAugmentedSixth:
          result = kB_Sharp_QTP;
          break;
          
        case kDiminishedSeventh:
          result = kC_Flat_QTP;
          break;
        case kMinorSeventh:
          result = kC_Natural_QTP;
          break;
        case kMajorSeventh:
          result = kC_Sharp_QTP;
          break;
        case kAugmentedSeventh:
          result = kC_DoubleSharp_QTP;
          break;
    
        case kDiminishedOctave:
          result = kD_Flat_QTP;
          break;
        case kPerfectOctave:
          result = kD_Natural_QTP;
          break;
        case kAugmentedOctave:
          result = kD_Sharp_QTP;
          break;
          
        case kDiminishedNinth:
          result = kE_DoubleFlat_QTP;
          break;
        case kMinorNinth:
          result = kE_Flat_QTP;
          break;
        case kMajorNinth:
          result = kE_Natural_QTP;
          break;
        case kAugmentedNinth:
          result = kE_Sharp_QTP;
          break;
          
        case kDiminishedTenth:
          result = kF_Flat_QTP;
          break;
        case kMinorTenth:
          result = kF_Natural_QTP;
          break;
        case kMajorTenth:
          result = kF_Sharp_QTP;
          break;
        case kAugmentedTenth:
          result = kF_DoubleSharp_QTP;
          break;
          
        case kDiminishedEleventh:
          result = kG_Flat_QTP;
          break;
        case kPerfectEleventh:
          result = kG_Natural_QTP;
          break;
        case kAugmentedEleventh:
          result = kG_Sharp_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kA_Flat_QTP;
          break;
        case kPerfectTwelfth:
          result = kA_Natural_QTP;
          break;
        case kAugmentedTwelfth:
          result = kA_Sharp_QTP;
          break;
    
        case kDiminishedThirteenth:
          result = kB_DoubleFlat_QTP;
          break;
        case kMinorThirteenth:
          result = kB_Flat_QTP;
          break;
        case kMajorThirteenth:
          result = kB_Natural_QTP;
          break;
        case kAugmentedThirteenth:
          result = kB_Sharp_QTP;
          break;
      } // switch
      break;

    case kD_Sharp_QTP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kD_Natural_QTP;
          break;
        case kPerfectUnison:
          result = kD_Sharp_QTP;
          break;
        case kAugmentedUnison:
          result = kD_DoubleSharp_QTP;
          break;
          
        case kDiminishedSecond:
          result = kE_Flat_QTP;
          break;
        case kMinorSecond:
          result = kE_Natural_QTP;
          break;
        case kMajorSecond:
          result = kE_Sharp_QTP;
          break;
        case kAugmentedSecond:
          result = kE_DoubleSharp_QTP;
          break;
          
        case kDiminishedThird:
          result = kF_Flat_QTP;
          break;
        case kMinorThird:
          result = kF_Natural_QTP;
          break;
        case kMajorThird:
          result = kF_Sharp_QTP;
          break;
        case kAugmentedThird:
          result = kF_DoubleSharp_QTP;
          break;
          
        case kDiminishedFourth:
          result = kG_Natural_QTP;
          break;
        case kPerfectFourth:
          result = kG_Sharp_QTP;
          break;
        case kAugmentedFourth:
          result = kG_DoubleSharp_QTP;
          break;
          
        case kDiminishedFifth:
          result = kA_Natural_QTP;
          break;
        case kPerfectFifth:
          result = kA_Sharp_QTP;
          break;
        case kAugmentedFifth:
          result = kA_DoubleSharp_QTP;
          break;
    
        case kDiminishedSixth:
          result = kB_Flat_QTP;
          break;
        case kMinorSixth:
          result = kB_Natural_QTP;
          break;
        case kMajorSixth:
          result = kB_Sharp_QTP;
          break;
        case kAugmentedSixth:
          result = kB_DoubleSharp_QTP;
          break;
          
        case kDiminishedSeventh:
          result = kC_Natural_QTP;
          break;
        case kMinorSeventh:
          result = kC_Sharp_QTP;
          break;
        case kMajorSeventh:
          result = kC_DoubleSharp_QTP;
          break;
        case kAugmentedSeventh:
          result = kC_TripleSharp_QTP;
          break;
    
        case kDiminishedOctave:
          result = kD_Natural_QTP;
          break;
        case kPerfectOctave:
          result = kD_Sharp_QTP;
          break;
        case kAugmentedOctave:
          result = kD_DoubleSharp_QTP;
          break;
          
        case kDiminishedNinth:
          result = kE_Flat_QTP;
          break;
        case kMinorNinth:
          result = kE_Natural_QTP;
          break;
        case kMajorNinth:
          result = kE_Sharp_QTP;
          break;
        case kAugmentedNinth:
          result = kE_DoubleSharp_QTP;
          break;
          
        case kDiminishedTenth:
          result = kF_Natural_QTP;
          break;
        case kMinorTenth:
          result = kF_Sharp_QTP;
          break;
        case kMajorTenth:
          result = kF_DoubleSharp_QTP;
          break;
        case kAugmentedTenth:
          result = kF_DoubleSharp_QTP;
          break;
          
        case kDiminishedEleventh:
          result = kG_Natural_QTP;
          break;
        case kPerfectEleventh:
          result = kG_Sharp_QTP;
          break;
        case kAugmentedEleventh:
          result = kG_DoubleSharp_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kA_Natural_QTP;
          break;
        case kPerfectTwelfth:
          result = kA_Sharp_QTP;
          break;
        case kAugmentedTwelfth:
          result = kA_DoubleSharp_QTP;
          break;
    
        case kDiminishedThirteenth:
          result = kB_Flat_QTP;
          break;
        case kMinorThirteenth:
          result = kB_Natural_QTP;
          break;
        case kMajorThirteenth:
          result = kB_Sharp_QTP;
          break;
        case kAugmentedThirteenth:
          result = kB_DoubleSharp_QTP;
          break;
      } // switch
      break;

    case kE_Flat_QTP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kE_DoubleFlat_QTP;
          break;
        case kPerfectUnison:
          result = kE_Flat_QTP;
          break;
        case kAugmentedUnison:
          result = kE_Natural_QTP;
          break;
          
        case kDiminishedSecond:
          result = kF_DoubleFlat_QTP;
          break;
        case kMinorSecond:
          result = kF_Flat_QTP;
          break;
        case kMajorSecond:
          result = kF_Natural_QTP;
          break;
        case kAugmentedSecond:
          result = kF_Sharp_QTP;
          break;
          
        case kDiminishedThird:
          result = kG_DoubleFlat_QTP;
          break;
        case kMinorThird:
          result = kG_Flat_QTP;
          break;
        case kMajorThird:
          result = kG_Natural_QTP;
          break;
        case kAugmentedThird:
          result = kG_Sharp_QTP;
          break;
          
        case kDiminishedFourth:
          result = kA_DoubleFlat_QTP;
          break;
        case kPerfectFourth:
          result = kA_Flat_QTP;
          break;
        case kAugmentedFourth:
          result = kA_Natural_QTP;
          break;
          
        case kDiminishedFifth:
          result = kB_DoubleFlat_QTP;
          break;
        case kPerfectFifth:
          result = kB_Flat_QTP;
          break;
        case kAugmentedFifth:
          result = kB_Natural_QTP;
          break;
    
        case kDiminishedSixth:
          result = kC_DoubleFlat_QTP;
          break;
        case kMinorSixth:
          result = kC_Flat_QTP;
          break;
        case kMajorSixth:
          result = kC_Natural_QTP;
          break;
        case kAugmentedSixth:
          result = kC_Sharp_QTP;
          break;
          
        case kDiminishedSeventh:
          result = kD_DoubleFlat_QTP;
          break;
        case kMinorSeventh:
          result = kD_Flat_QTP;
          break;
        case kMajorSeventh:
          result = kD_Natural_QTP;
          break;
        case kAugmentedSeventh:
          result = kD_Sharp_QTP;
          break;
    
        case kDiminishedOctave:
          result = kE_DoubleFlat_QTP;
          break;
        case kPerfectOctave:
          result = kE_Flat_QTP;
          break;
        case kAugmentedOctave:
          result = kE_Natural_QTP;
          break;
          
        case kDiminishedNinth:
          result = kF_DoubleFlat_QTP;
          break;
        case kMinorNinth:
          result = kF_Flat_QTP;
          break;
        case kMajorNinth:
          result = kF_Natural_QTP;
          break;
        case kAugmentedNinth:
          result = kF_Sharp_QTP;
          break;
          
        case kDiminishedTenth:
          result = kG_DoubleFlat_QTP;
          break;
        case kMinorTenth:
          result = kG_Flat_QTP;
          break;
        case kMajorTenth:
          result = kG_Natural_QTP;
          break;
        case kAugmentedTenth:
          result = kG_Sharp_QTP;
          break;
          
        case kDiminishedEleventh:
          result = kA_DoubleFlat_QTP;
          break;
        case kPerfectEleventh:
          result = kA_Flat_QTP;
          break;
        case kAugmentedEleventh:
          result = kA_Natural_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kB_DoubleFlat_QTP;
          break;
        case kPerfectTwelfth:
          result = kB_Flat_QTP;
          break;
        case kAugmentedTwelfth:
          result = kB_Natural_QTP;
          break;
    
        case kDiminishedThirteenth:
          result = kC_DoubleFlat_QTP;
          break;
        case kMinorThirteenth:
          result = kC_Flat_QTP;
          break;
        case kMajorThirteenth:
          result = kC_Natural_QTP;
          break;
        case kAugmentedThirteenth:
          result = kC_Sharp_QTP;
          break;
      } // switch
      break;
      
    case kE_Natural_QTP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kE_Flat_QTP;
          break;
        case kPerfectUnison:
          result = kE_Natural_QTP;
          break;
        case kAugmentedUnison:
          result = kE_Sharp_QTP;
          break;
          
        case kDiminishedSecond:
          result = kF_Flat_QTP;
          break;
        case kMinorSecond:
          result = kF_Natural_QTP;
          break;
        case kMajorSecond:
          result = kF_Sharp_QTP;
          break;
        case kAugmentedSecond:
          result = kF_DoubleSharp_QTP;
          break;
          
        case kDiminishedThird:
          result = kG_Flat_QTP;
          break;
        case kMinorThird:
          result = kG_Natural_QTP;
          break;
        case kMajorThird:
          result = kG_Sharp_QTP;
          break;
        case kAugmentedThird:
          result = kG_DoubleSharp_QTP;
          break;
          
        case kDiminishedFourth:
          result = kA_Flat_QTP;
          break;
        case kPerfectFourth:
          result = kA_Natural_QTP;
          break;
        case kAugmentedFourth:
          result = kA_Sharp_QTP;
          break;
          
        case kDiminishedFifth:
          result = kB_Natural_QTP;
          break;
        case kPerfectFifth:
          result = kB_Sharp_QTP;
          break;
        case kAugmentedFifth:
          result = kB_DoubleSharp_QTP;
          break;
    
        case kDiminishedSixth:
          result = kC_Flat_QTP;
          break;
        case kMinorSixth:
          result = kC_Natural_QTP;
          break;
        case kMajorSixth:
          result = kC_Sharp_QTP;
          break;
        case kAugmentedSixth:
          result = kC_DoubleSharp_QTP;
          break;
          
        case kDiminishedSeventh:
          result = kD_Flat_QTP;
          break;
        case kMinorSeventh:
          result = kD_Natural_QTP;
          break;
        case kMajorSeventh:
          result = kD_Sharp_QTP;
          break;
        case kAugmentedSeventh:
          result = kD_DoubleSharp_QTP;
          break;
    
        case kDiminishedOctave:
          result = kE_Flat_QTP;
          break;
        case kPerfectOctave:
          result = kE_Natural_QTP;
          break;
        case kAugmentedOctave:
          result = kE_Sharp_QTP;
          break;
          
        case kDiminishedNinth:
          result = kF_Flat_QTP;
          break;
        case kMinorNinth:
          result = kF_Natural_QTP;
          break;
        case kMajorNinth:
          result = kF_Sharp_QTP;
          break;
        case kAugmentedNinth:
          result = kF_DoubleSharp_QTP;
          break;
          
        case kDiminishedTenth:
          result = kG_Flat_QTP;
          break;
        case kMinorTenth:
          result = kG_Natural_QTP;
          break;
        case kMajorTenth:
          result = kG_Sharp_QTP;
          break;
        case kAugmentedTenth:
          result = kG_DoubleSharp_QTP;
          break;
          
        case kDiminishedEleventh:
          result = kA_Flat_QTP;
          break;
        case kPerfectEleventh:
          result = kA_Natural_QTP;
          break;
        case kAugmentedEleventh:
          result = kA_Sharp_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kB_Natural_QTP;
          break;
        case kPerfectTwelfth:
          result = kB_Sharp_QTP;
          break;
        case kAugmentedTwelfth:
          result = kB_DoubleSharp_QTP;
          break;
    
        case kDiminishedThirteenth:
          result = kC_Flat_QTP;
          break;
        case kMinorThirteenth:
          result = kC_Natural_QTP;
          break;
        case kMajorThirteenth:
          result = kC_Sharp_QTP;
          break;
        case kAugmentedThirteenth:
          result = kC_DoubleSharp_QTP;
          break;
      } // switch
      break;

    case kE_Sharp_QTP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kE_Natural_QTP;
          break;
        case kPerfectUnison:
          result = kE_Sharp_QTP;
          break;
        case kAugmentedUnison:
          result = kE_DoubleSharp_QTP;
          break;
          
        case kDiminishedSecond:
          result = kF_Natural_QTP;
          break;
        case kMinorSecond:
          result = kF_Sharp_QTP;
          break;
        case kMajorSecond:
          result = kF_DoubleSharp_QTP;
          break;
        case kAugmentedSecond:
          result = kF_TripleSharp_QTP;
          break;
          
        case kDiminishedThird:
          result = kG_Natural_QTP;
          break;
        case kMinorThird:
          result = kG_Sharp_QTP;
          break;
        case kMajorThird:
          result = kG_DoubleSharp_QTP;
          break;
        case kAugmentedThird:
          result = kG_TripleSharp_QTP;
          break;
          
        case kDiminishedFourth:
          result = kA_Natural_QTP;
          break;
        case kPerfectFourth:
          result = kA_Sharp_QTP;
          break;
        case kAugmentedFourth:
          result = kA_DoubleSharp_QTP;
          break;
          
        case kDiminishedFifth:
          result = kB_Natural_QTP;
          break;
        case kPerfectFifth:
          result = kB_Sharp_QTP;
          break;
        case kAugmentedFifth:
          result = kB_DoubleSharp_QTP;
          break;
    
        case kDiminishedSixth:
          result = kC_Natural_QTP;
          break;
        case kMinorSixth:
          result = kC_Sharp_QTP;
          break;
        case kMajorSixth:
          result = kC_DoubleSharp_QTP;
          break;
        case kAugmentedSixth:
          result = kC_TripleSharp_QTP;
          break;
          
        case kDiminishedSeventh:
          result = kD_Natural_QTP;
          break;
        case kMinorSeventh:
          result = kD_Sharp_QTP;
          break;
        case kMajorSeventh:
          result = kD_DoubleSharp_QTP;
          break;
        case kAugmentedSeventh:
          result = kD_TripleSharp_QTP;
          break;
    
        case kDiminishedOctave:
          result = kE_Natural_QTP;
          break;
        case kPerfectOctave:
          result = kE_Sharp_QTP;
          break;
        case kAugmentedOctave:
          result = kE_DoubleSharp_QTP;
          break;
          
        case kDiminishedNinth:
          result = kF_Natural_QTP;
          break;
        case kMinorNinth:
          result = kF_Sharp_QTP;
          break;
        case kMajorNinth:
          result = kF_DoubleSharp_QTP;
          break;
        case kAugmentedNinth:
          result = kF_TripleSharp_QTP;
          break;
          
        case kDiminishedTenth:
          result = kG_Natural_QTP;
          break;
        case kMinorTenth:
          result = kG_Sharp_QTP;
          break;
        case kMajorTenth:
          result = kG_DoubleSharp_QTP;
          break;
        case kAugmentedTenth:
          result = kG_TripleSharp_QTP;
          break;
          
        case kDiminishedEleventh:
          result = kA_Natural_QTP;
          break;
        case kPerfectEleventh:
          result = kA_Sharp_QTP;
          break;
        case kAugmentedEleventh:
          result = kA_DoubleSharp_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kB_Natural_QTP;
          break;
        case kPerfectTwelfth:
          result = kB_Sharp_QTP;
          break;
        case kAugmentedTwelfth:
          result = kB_DoubleSharp_QTP;
          break;
    
        case kDiminishedThirteenth:
          result = kC_Natural_QTP;
          break;
        case kMinorThirteenth:
          result = kC_Sharp_QTP;
          break;
        case kMajorThirteenth:
          result = kC_DoubleSharp_QTP;
          break;
        case kAugmentedThirteenth:
          result = kC_TripleSharp_QTP;
          break;
      } // switch
      break;

    case kF_Flat_QTP:
      break;
      
    case kF_Natural_QTP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kF_Flat_QTP;
          break;
        case kPerfectUnison:
          result = kF_Natural_QTP;
          break;
        case kAugmentedUnison:
          result = kF_Sharp_QTP;
          break;
          
        case kDiminishedSecond:
          result = kG_DoubleFlat_QTP;
          break;
        case kMinorSecond:
          result = kG_Flat_QTP;
          break;
        case kMajorSecond:
          result = kG_Natural_QTP;
          break;
        case kAugmentedSecond:
          result = kG_Sharp_QTP;
          break;
          
        case kDiminishedThird:
          result = kA_DoubleFlat_QTP;
          break;
        case kMinorThird:
          result = kA_Flat_QTP;
          break;
        case kMajorThird:
          result = kA_Natural_QTP;
          break;
        case kAugmentedThird:
          result = kA_Sharp_QTP;
          break;
          
        case kDiminishedFourth:
          result = kB_DoubleFlat_QTP;
          break;
        case kPerfectFourth:
          result = kB_Flat_QTP;
          break;
        case kAugmentedFourth:
          result = kB_Natural_QTP;
          break;
          
        case kDiminishedFifth:
          result = kC_Flat_QTP;
          break;
        case kPerfectFifth:
          result = kC_Natural_QTP;
          break;
        case kAugmentedFifth:
          result = kC_Sharp_QTP;
          break;
    
        case kDiminishedSixth:
          result = kD_DoubleFlat_QTP;
          break;
        case kMinorSixth:
          result = kD_Flat_QTP;
          break;
        case kMajorSixth:
          result = kD_Natural_QTP;
          break;
        case kAugmentedSixth:
          result = kD_Sharp_QTP;
          break;
          
        case kDiminishedSeventh:
          result = kE_DoubleFlat_QTP;
          break;
        case kMinorSeventh:
          result = kE_Flat_QTP;
          break;
        case kMajorSeventh:
          result = kE_Natural_QTP;
          break;
        case kAugmentedSeventh:
          result = kE_Sharp_QTP;
          break;
    
        case kDiminishedOctave:
          result = kF_Flat_QTP;
          break;
        case kPerfectOctave:
          result = kF_Natural_QTP;
          break;
        case kAugmentedOctave:
          result = kF_Sharp_QTP;
          break;
          
        case kDiminishedNinth:
          result = kG_DoubleFlat_QTP;
          break;
        case kMinorNinth:
          result = kG_Flat_QTP;
          break;
        case kMajorNinth:
          result = kG_Natural_QTP;
          break;
        case kAugmentedNinth:
          result = kG_Sharp_QTP;
          break;
          
        case kDiminishedTenth:
          result = kA_DoubleFlat_QTP;
          break;
        case kMinorTenth:
          result = kA_Flat_QTP;
          break;
        case kMajorTenth:
          result = kA_Natural_QTP;
          break;
        case kAugmentedTenth:
          result = kA_Sharp_QTP;
          break;
          
        case kDiminishedEleventh:
          result = kB_DoubleFlat_QTP;
          break;
        case kPerfectEleventh:
          result = kB_Flat_QTP;
          break;
        case kAugmentedEleventh:
          result = kB_Natural_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kC_Flat_QTP;
          break;
        case kPerfectTwelfth:
          result = kC_Natural_QTP;
          break;
        case kAugmentedTwelfth:
          result = kC_Sharp_QTP;
          break;
    
        case kDiminishedThirteenth:
          result = kD_DoubleFlat_QTP;
          break;
        case kMinorThirteenth:
          result = kD_Flat_QTP;
          break;
        case kMajorThirteenth:
          result = kD_Natural_QTP;
          break;
        case kAugmentedThirteenth:
          result = kD_Sharp_QTP;
          break;
      } // switch
      break;
      
    case kF_Sharp_QTP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kF_Natural_QTP;
          break;
        case kPerfectUnison:
          result = kF_Sharp_QTP;
          break;
        case kAugmentedUnison:
          result = kF_DoubleSharp_QTP;
          break;
          
        case kDiminishedSecond:
          result = kG_Flat_QTP;
          break;
        case kMinorSecond:
          result = kG_Natural_QTP;
          break;
        case kMajorSecond:
          result = kG_Sharp_QTP;
          break;
        case kAugmentedSecond:
          result = kG_DoubleSharp_QTP;
          break;
          
        case kDiminishedThird:
          result = kA_Flat_QTP;
          break;
        case kMinorThird:
          result = kA_Natural_QTP;
          break;
        case kMajorThird:
          result = kA_Sharp_QTP;
          break;
        case kAugmentedThird:
          result = kA_DoubleSharp_QTP;
          break;
          
        case kDiminishedFourth:
          result = kB_Sharp_QTP;
          break;
        case kPerfectFourth:
          result = kB_DoubleSharp_QTP;
          break;
        case kAugmentedFourth:
          result = kB_TripleSharp_QTP;
          break;
          
        case kDiminishedFifth:
          result = kC_Natural_QTP;
          break;
        case kPerfectFifth:
          result = kC_Sharp_QTP;
          break;
        case kAugmentedFifth:
          result = kC_DoubleSharp_QTP;
          break;
    
        case kDiminishedSixth:
          result = kD_Flat_QTP;
          break;
        case kMinorSixth:
          result = kD_Natural_QTP;
          break;
        case kMajorSixth:
          result = kD_Sharp_QTP;
          break;
        case kAugmentedSixth:
          result = kD_DoubleSharp_QTP;
          break;
          
        case kDiminishedSeventh:
          result = kE_Flat_QTP;
          break;
        case kMinorSeventh:
          result = kE_Natural_QTP;
          break;
        case kMajorSeventh:
          result = kE_Sharp_QTP;
          break;
        case kAugmentedSeventh:
          result = kE_DoubleSharp_QTP;
          break;
    
        case kDiminishedOctave:
          result = kF_Natural_QTP;
          break;
        case kPerfectOctave:
          result = kF_Sharp_QTP;
          break;
        case kAugmentedOctave:
          result = kF_DoubleSharp_QTP;
          break;
          
        case kDiminishedNinth:
          result = kG_Flat_QTP;
          break;
        case kMinorNinth:
          result = kG_Natural_QTP;
          break;
        case kMajorNinth:
          result = kG_Sharp_QTP;
          break;
        case kAugmentedNinth:
          result = kG_DoubleSharp_QTP;
          break;
          
        case kDiminishedTenth:
          result = kA_Natural_QTP;
          break;
        case kMinorTenth:
          result = kA_Natural_QTP;
          break;
        case kMajorTenth:
          result = kA_Sharp_QTP;
          break;
        case kAugmentedTenth:
          result = kA_DoubleSharp_QTP;
          break;
          
        case kDiminishedEleventh:
          result = kB_Sharp_QTP;
          break;
        case kPerfectEleventh:
          result = kB_DoubleSharp_QTP;
          break;
        case kAugmentedEleventh:
          result = kB_TripleSharp_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kC_Natural_QTP;
          break;
        case kPerfectTwelfth:
          result = kC_Sharp_QTP;
          break;
        case kAugmentedTwelfth:
          result = kC_DoubleSharp_QTP;
          break;
    
        case kDiminishedThirteenth:
          result = kE_Flat_QTP;
          break;
        case kMinorThirteenth:
          result = kD_Natural_QTP;
          break;
        case kMajorThirteenth:
          result = kD_Sharp_QTP;
          break;
        case kAugmentedThirteenth:
          result = kD_DoubleSharp_QTP;
          break;
      } // switch
      break;

    case kG_Flat_QTP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kG_DoubleFlat_QTP;
          break;
        case kPerfectUnison:
          result = kG_Flat_QTP;
          break;
        case kAugmentedUnison:
          result = kG_Natural_QTP;
          break;
          
        case kDiminishedSecond:
          result = kA_TripleFlat_QTP;
          break;
        case kMinorSecond:
          result = kA_DoubleFlat_QTP;
          break;
        case kMajorSecond:
          result = kA_Flat_QTP;
          break;
        case kAugmentedSecond:
          result = kA_Natural_QTP;
          break;
          
        case kDiminishedThird:
          result = kB_TripleFlat_QTP;
          break;
        case kMinorThird:
          result = kB_DoubleFlat_QTP;
          break;
        case kMajorThird:
          result = kB_Flat_QTP;
          break;
        case kAugmentedThird:
          result = kB_Natural_QTP;
          break;
          
        case kDiminishedFourth:
          result = kC_DoubleFlat_QTP;
          break;
        case kPerfectFourth:
          result = kC_Flat_QTP;
          break;
        case kAugmentedFourth:
          result = kC_Natural_QTP;
          break;
          
        case kDiminishedFifth:
          result = kD_DoubleFlat_QTP;
          break;
        case kPerfectFifth:
          result = kD_Flat_QTP;
          break;
        case kAugmentedFifth:
          result = kD_Natural_QTP;
          break;
    
        case kDiminishedSixth:
          result = kE_TripleFlat_QTP;
          break;
        case kMinorSixth:
          result = kE_DoubleFlat_QTP;
          break;
        case kMajorSixth:
          result = kE_Flat_QTP;
          break;
        case kAugmentedSixth:
          result = kE_Natural_QTP;
          break;
          
        case kDiminishedSeventh:
          result = kF_DoubleFlat_QTP;
          break;
        case kMinorSeventh:
          result = kF_Flat_QTP;
          break;
        case kMajorSeventh:
          result = kF_Natural_QTP;
          break;
        case kAugmentedSeventh:
          result = kF_Sharp_QTP;
          break;
    
        case kDiminishedOctave:
          result = kG_DoubleFlat_QTP;
          break;
        case kPerfectOctave:
          result = kG_Flat_QTP;
          break;
        case kAugmentedOctave:
          result = kG_Natural_QTP;
          break;
          
        case kDiminishedNinth:
          result = kA_TripleFlat_QTP;
          break;
        case kMinorNinth:
          result = kA_DoubleFlat_QTP;
          break;
        case kMajorNinth:
          result = kA_Flat_QTP;
          break;
        case kAugmentedNinth:
          result = kA_Natural_QTP;
          break;
          
        case kDiminishedTenth:
          result = kB_TripleFlat_QTP;
          break;
        case kMinorTenth:
          result = kB_DoubleFlat_QTP;
          break;
        case kMajorTenth:
          result = kB_Flat_QTP;
          break;
        case kAugmentedTenth:
          result = kB_Natural_QTP;
          break;
          
        case kDiminishedEleventh:
          result = kC_DoubleFlat_QTP;
          break;
        case kPerfectEleventh:
          result = kC_Flat_QTP;
          break;
        case kAugmentedEleventh:
          result = kC_Natural_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kD_DoubleFlat_QTP;
          break;
        case kPerfectTwelfth:
          result = kD_Flat_QTP;
          break;
        case kAugmentedTwelfth:
          result = kD_Natural_QTP;
          break;
    
        case kDiminishedThirteenth:
          result = kE_TripleFlat_QTP;
          break;
        case kMinorThirteenth:
          result = kE_DoubleFlat_QTP;
          break;
        case kMajorThirteenth:
          result = kE_Flat_QTP;
          break;
        case kAugmentedThirteenth:
          result = kE_Natural_QTP;
          break;
      } // switch
      break;
      
    case kG_Natural_QTP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kG_Flat_QTP;
          break;
        case kPerfectUnison:
          result = kG_Natural_QTP;
          break;
        case kAugmentedUnison:
          result = kG_Sharp_QTP;
          break;
          
        case kDiminishedSecond:
          result = kA_DoubleFlat_QTP;
          break;
        case kMinorSecond:
          result = kA_Flat_QTP;
          break;
        case kMajorSecond:
          result = kA_Natural_QTP;
          break;
        case kAugmentedSecond:
          result = kA_Sharp_QTP;
          break;
          
        case kDiminishedThird:
          result = kB_DoubleFlat_QTP;
          break;
        case kMinorThird:
          result = kB_Flat_QTP;
          break;
        case kMajorThird:
          result = kB_Natural_QTP;
          break;
        case kAugmentedThird:
          result = kB_Sharp_QTP;
          break;
          
        case kDiminishedFourth:
          result = kC_Flat_QTP;
          break;
        case kPerfectFourth:
          result = kC_Natural_QTP;
          break;
        case kAugmentedFourth:
          result = kC_Sharp_QTP;
          break;
          
        case kDiminishedFifth:
          result = kD_Flat_QTP;
          break;
        case kPerfectFifth:
          result = kD_Natural_QTP;
          break;
        case kAugmentedFifth:
          result = kD_Sharp_QTP;
          break;
    
        case kDiminishedSixth:
          result = kE_DoubleFlat_QTP;
          break;
        case kMinorSixth:
          result = kE_Flat_QTP;
          break;
        case kMajorSixth:
          result = kE_Natural_QTP;
          break;
        case kAugmentedSixth:
          result = kE_Sharp_QTP;
          break;
          
        case kDiminishedSeventh:
          result = kF_Flat_QTP;
          break;
        case kMinorSeventh:
          result = kF_Natural_QTP;
          break;
        case kMajorSeventh:
          result = kF_Sharp_QTP;
          break;
        case kAugmentedSeventh:
          result = kF_DoubleSharp_QTP;
          break;
    
        case kDiminishedOctave:
          result = kG_Flat_QTP;
          break;
        case kPerfectOctave:
          result = kG_Natural_QTP;
          break;
        case kAugmentedOctave:
          result = kG_Sharp_QTP;
          break;
          
        case kDiminishedNinth:
          result = kA_DoubleFlat_QTP;
          break;
        case kMinorNinth:
          result = kA_Flat_QTP;
          break;
        case kMajorNinth:
          result = kA_Natural_QTP;
          break;
        case kAugmentedNinth:
          result = kA_Sharp_QTP;
          break;
          
        case kDiminishedTenth:
          result = kB_DoubleFlat_QTP;
          break;
        case kMinorTenth:
          result = kB_Flat_QTP;
          break;
        case kMajorTenth:
          result = kB_Natural_QTP;
          break;
        case kAugmentedTenth:
          result = kB_Sharp_QTP;
          break;
          
        case kDiminishedEleventh:
          result = kC_Flat_QTP;
          break;
        case kPerfectEleventh:
          result = kC_Natural_QTP;
          break;
        case kAugmentedEleventh:
          result = kC_Sharp_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kD_Flat_QTP;
          break;
        case kPerfectTwelfth:
          result = kD_Natural_QTP;
          break;
        case kAugmentedTwelfth:
          result = kD_Sharp_QTP;
          break;
    
        case kDiminishedThirteenth:
          result = kE_DoubleFlat_QTP;
          break;
        case kMinorThirteenth:
          result = kE_Flat_QTP;
          break;
        case kMajorThirteenth:
          result = kE_Natural_QTP;
          break;
        case kAugmentedThirteenth:
          result = kE_Sharp_QTP;
          break;
      } // switch
      break;
      
    case kG_Sharp_QTP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kG_Natural_QTP;
          break;
        case kPerfectUnison:
          result = kG_Sharp_QTP;
          break;
        case kAugmentedUnison:
          result = kG_DoubleSharp_QTP;
          break;
          
        case kDiminishedSecond:
          result = kA_Flat_QTP;
          break;
        case kMinorSecond:
          result = kA_Natural_QTP;
          break;
        case kMajorSecond:
          result = kA_Sharp_QTP;
          break;
        case kAugmentedSecond:
          result = kA_DoubleSharp_QTP;
          break;
          
        case kDiminishedThird:
          result = kB_Flat_QTP;
          break;
        case kMinorThird:
          result = kB_Natural_QTP;
          break;
        case kMajorThird:
          result = kB_Sharp_QTP;
          break;
        case kAugmentedThird:
          result = kB_DoubleSharp_QTP;
          break;
          
        case kDiminishedFourth:
          result = kC_Natural_QTP;
          break;
        case kPerfectFourth:
          result = kC_Sharp_QTP;
          break;
        case kAugmentedFourth:
          result = kC_DoubleSharp_QTP;
          break;
          
        case kDiminishedFifth:
          result = kD_Natural_QTP;
          break;
        case kPerfectFifth:
          result = kD_Sharp_QTP;
          break;
        case kAugmentedFifth:
          result = kD_DoubleSharp_QTP;
          break;
    
        case kDiminishedSixth:
          result = kE_Flat_QTP;
          break;
        case kMinorSixth:
          result = kE_Natural_QTP;
          break;
        case kMajorSixth:
          result = kE_Sharp_QTP;
          break;
        case kAugmentedSixth:
          result = kE_DoubleSharp_QTP;
          break;
          
        case kDiminishedSeventh:
          result = kF_Natural_QTP;
          break;
        case kMinorSeventh:
          result = kF_Sharp_QTP;
          break;
        case kMajorSeventh:
          result = kF_DoubleSharp_QTP;
          break;
        case kAugmentedSeventh:
          result = kF_TripleSharp_QTP;
          break;
    
        case kDiminishedOctave:
          result = kG_Natural_QTP;
          break;
        case kPerfectOctave:
          result = kG_Sharp_QTP;
          break;
        case kAugmentedOctave:
          result = kG_DoubleSharp_QTP;
          break;
          
        case kDiminishedNinth:
          result = kA_Flat_QTP;
          break;
        case kMinorNinth:
          result = kA_Natural_QTP;
          break;
        case kMajorNinth:
          result = kA_Sharp_QTP;
          break;
        case kAugmentedNinth:
          result = kA_DoubleSharp_QTP;
          break;
          
        case kDiminishedTenth:
          result = kB_Flat_QTP;
          break;
        case kMinorTenth:
          result = kB_Natural_QTP;
          break;
        case kMajorTenth:
          result = kB_Sharp_QTP;
          break;
        case kAugmentedTenth:
          result = kB_DoubleSharp_QTP;
          break;
          
        case kDiminishedEleventh:
          result = kC_Natural_QTP;
          break;
        case kPerfectEleventh:
          result = kC_Sharp_QTP;
          break;
        case kAugmentedEleventh:
          result = kC_DoubleSharp_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kD_Natural_QTP;
          break;
        case kPerfectTwelfth:
          result = kD_Sharp_QTP;
          break;
        case kAugmentedTwelfth:
          result = kD_DoubleSharp_QTP;
          break;
    
        case kDiminishedThirteenth:
          result = kE_Flat_QTP;
          break;
        case kMinorThirteenth:
          result = kE_Natural_QTP;
          break;
        case kMajorThirteenth:
          result = kE_Sharp_QTP;
          break;
        case kAugmentedThirteenth:
          result = kE_DoubleSharp_QTP;
          break;
      } // switch
      break;

    case kA_Flat_QTP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kA_DoubleFlat_QTP;
          break;
        case kPerfectUnison:
          result = kA_Flat_QTP;
          break;
        case kAugmentedUnison:
          result = kA_Natural_QTP;
          break;
          
        case kDiminishedSecond:
          result = kB_TripleFlat_QTP;
          break;
        case kMinorSecond:
          result = kB_DoubleFlat_QTP;
          break;
        case kMajorSecond:
          result = kB_Flat_QTP;
          break;
        case kAugmentedSecond:
          result = kB_Natural_QTP;
          break;
          
        case kDiminishedThird:
          result = kC_DoubleFlat_QTP;
          break;
        case kMinorThird:
          result = kC_Flat_QTP;
          break;
        case kMajorThird:
          result = kC_Natural_QTP;
          break;
        case kAugmentedThird:
          result = kC_Sharp_QTP;
          break;
          
        case kDiminishedFourth:
          result = kD_DoubleFlat_QTP;
          break;
        case kPerfectFourth:
          result = kD_Flat_QTP;
          break;
        case kAugmentedFourth:
          result = kD_Natural_QTP;
          break;
          
        case kDiminishedFifth:
          result = kE_DoubleFlat_QTP;
          break;
        case kPerfectFifth:
          result = kE_Flat_QTP;
          break;
        case kAugmentedFifth:
          result = kE_Natural_QTP;
          break;
    
        case kDiminishedSixth:
          result = kF_DoubleFlat_QTP;
          break;
        case kMinorSixth:
          result = kF_Flat_QTP;
          break;
        case kMajorSixth:
          result = kF_Natural_QTP;
          break;
        case kAugmentedSixth:
          result = kF_Sharp_QTP;
          break;
          
        case kDiminishedSeventh:
          result = kG_DoubleFlat_QTP;
          break;
        case kMinorSeventh:
          result = kG_Flat_QTP;
          break;
        case kMajorSeventh:
          result = kG_Natural_QTP;
          break;
        case kAugmentedSeventh:
          result = kG_Sharp_QTP;
          break;
    
        case kDiminishedOctave:
          result = kA_DoubleFlat_QTP;
          break;
        case kPerfectOctave:
          result = kA_Flat_QTP;
          break;
        case kAugmentedOctave:
          result = kA_Natural_QTP;
          break;
          
        case kDiminishedNinth:
          result = kB_TripleFlat_QTP;
          break;
        case kMinorNinth:
          result = kB_DoubleFlat_QTP;
          break;
        case kMajorNinth:
          result = kB_Flat_QTP;
          break;
        case kAugmentedNinth:
          result = kB_Natural_QTP;
          break;
          
        case kDiminishedTenth:
          result = kC_DoubleFlat_QTP;
          break;
        case kMinorTenth:
          result = kC_Flat_QTP;
          break;
        case kMajorTenth:
          result = kC_Natural_QTP;
          break;
        case kAugmentedTenth:
          result = kC_Sharp_QTP;
          break;
          
        case kDiminishedEleventh:
          result = kD_DoubleFlat_QTP;
          break;
        case kPerfectEleventh:
          result = kD_Flat_QTP;
          break;
        case kAugmentedEleventh:
          result = kD_Natural_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kE_DoubleFlat_QTP;
          break;
        case kPerfectTwelfth:
          result = kE_Flat_QTP;
          break;
        case kAugmentedTwelfth:
          result = kE_Natural_QTP;
          break;
    
        case kDiminishedThirteenth:
          result = kF_TripleFlat_QTP;
          break;
        case kMinorThirteenth:
          result = kF_DoubleFlat_QTP;
          break;
        case kMajorThirteenth:
          result = kF_Flat_QTP;
          break;
        case kAugmentedThirteenth:
          result = kF_Natural_QTP;
          break;
      } // switch
      break;
      
    case kA_Natural_QTP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kA_Flat_QTP;
          break;
        case kPerfectUnison:
          result = kA_Natural_QTP;
          break;
        case kAugmentedUnison:
          result = kA_Sharp_QTP;
          break;
          
        case kDiminishedSecond:
          result = kB_DoubleFlat_QTP;
          break;
        case kMinorSecond:
          result = kB_Flat_QTP;
          break;
        case kMajorSecond:
          result = kB_Natural_QTP;
          break;
        case kAugmentedSecond:
          result = kB_Sharp_QTP;
          break;
          
        case kDiminishedThird:
          result = kC_Flat_QTP;
          break;
        case kMinorThird:
          result = kC_Natural_QTP;
          break;
        case kMajorThird:
          result = kC_Sharp_QTP;
          break;
        case kAugmentedThird:
          result = kC_DoubleSharp_QTP;
          break;
          
        case kDiminishedFourth:
          result = kD_Flat_QTP;
          break;
        case kPerfectFourth:
          result = kD_Natural_QTP;
          break;
        case kAugmentedFourth:
          result = kD_Sharp_QTP;
          break;
          
        case kDiminishedFifth:
          result = kE_Flat_QTP;
          break;
        case kPerfectFifth:
          result = kE_Natural_QTP;
          break;
        case kAugmentedFifth:
          result = kE_Sharp_QTP;
          break;
    
        case kDiminishedSixth:
          result = kF_Flat_QTP;
          break;
        case kMinorSixth:
          result = kF_Natural_QTP;
          break;
        case kMajorSixth:
          result = kF_Sharp_QTP;
          break;
        case kAugmentedSixth:
          result = kF_DoubleSharp_QTP;
          break;
          
        case kDiminishedSeventh:
          result = kG_Flat_QTP;
          break;
        case kMinorSeventh:
          result = kG_Natural_QTP;
          break;
        case kMajorSeventh:
          result = kG_Sharp_QTP;
          break;
        case kAugmentedSeventh:
          result = kG_DoubleSharp_QTP;
          break;
    
        case kDiminishedOctave:
          result = kA_Flat_QTP;
          break;
        case kPerfectOctave:
          result = kA_Natural_QTP;
          break;
        case kAugmentedOctave:
          result = kA_Sharp_QTP;
          break;
          
        case kDiminishedNinth:
          result = kB_DoubleFlat_QTP;
          break;
        case kMinorNinth:
          result = kB_Flat_QTP;
          break;
        case kMajorNinth:
          result = kB_Natural_QTP;
          break;
        case kAugmentedNinth:
          result = kB_Sharp_QTP;
          break;
          
        case kDiminishedTenth:
          result = kC_Flat_QTP;
          break;
        case kMinorTenth:
          result = kC_Natural_QTP;
          break;
        case kMajorTenth:
          result = kC_Sharp_QTP;
          break;
        case kAugmentedTenth:
          result = kC_DoubleSharp_QTP;
          break;
          
        case kDiminishedEleventh:
          result = kD_Flat_QTP;
          break;
        case kPerfectEleventh:
          result = kD_Natural_QTP;
          break;
        case kAugmentedEleventh:
          result = kD_Sharp_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kE_Flat_QTP;
          break;
        case kPerfectTwelfth:
          result = kE_Natural_QTP;
          break;
        case kAugmentedTwelfth:
          result = kE_Sharp_QTP;
          break;
    
        case kDiminishedThirteenth:
          result = kF_Flat_QTP;
          break;
        case kMinorThirteenth:
          result = kF_Natural_QTP;
          break;
        case kMajorThirteenth:
          result = kF_Sharp_QTP;
          break;
        case kAugmentedThirteenth:
          result = kF_DoubleSharp_QTP;
          break;
      } // switch
      break;
      
    case kA_Sharp_QTP:
      break;

    case kB_Flat_QTP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kB_DoubleFlat_QTP;
          break;
        case kPerfectUnison:
          result = kB_Flat_QTP;
          break;
        case kAugmentedUnison:
          result = kB_Natural_QTP;
          break;
          
        case kDiminishedSecond:
          result = kC_DoubleFlat_QTP;
          break;
        case kMinorSecond:
          result = kC_Flat_QTP;
          break;
        case kMajorSecond:
          result = kC_Natural_QTP;
          break;
        case kAugmentedSecond:
          result = kC_Sharp_QTP;
          break;
          
        case kDiminishedThird:
          result = kD_DoubleFlat_QTP;
          break;
        case kMinorThird:
          result = kD_Flat_QTP;
          break;
        case kMajorThird:
          result = kD_Natural_QTP;
          break;
        case kAugmentedThird:
          result = kD_Sharp_QTP;
          break;
          
        case kDiminishedFourth:
          result = kE_DoubleFlat_QTP;
          break;
        case kPerfectFourth:
          result = kE_Flat_QTP;
          break;
        case kAugmentedFourth:
          result = kE_Natural_QTP;
          break;
          
        case kDiminishedFifth:
          result = kF_Flat_QTP;
          break;
        case kPerfectFifth:
          result = kF_Natural_QTP;
          break;
        case kAugmentedFifth:
          result = kF_Sharp_QTP;
          break;
    
        case kDiminishedSixth:
          result = kG_DoubleFlat_QTP;
          break;
        case kMinorSixth:
          result = kG_Flat_QTP;
          break;
        case kMajorSixth:
          result = kG_Natural_QTP;
          break;
        case kAugmentedSixth:
          result = kG_Sharp_QTP;
          break;
          
        case kDiminishedSeventh:
          result = kA_DoubleFlat_QTP;
          break;
        case kMinorSeventh:
          result = kA_Flat_QTP;
          break;
        case kMajorSeventh:
          result = kA_Natural_QTP;
          break;
        case kAugmentedSeventh:
          result = kA_Sharp_QTP;
          break;
    
        case kDiminishedOctave:
          result = kB_DoubleFlat_QTP;
          break;
        case kPerfectOctave:
          result = kB_Flat_QTP;
          break;
        case kAugmentedOctave:
          result = kB_Natural_QTP;
          break;
          
        case kDiminishedNinth:
          result = kC_DoubleFlat_QTP;
          break;
        case kMinorNinth:
          result = kC_Flat_QTP;
          break;
        case kMajorNinth:
          result = kC_Natural_QTP;
          break;
        case kAugmentedNinth:
          result = kC_Sharp_QTP;
          break;
          
        case kDiminishedTenth:
          result = kD_DoubleFlat_QTP;
          break;
        case kMinorTenth:
          result = kD_Flat_QTP;
          break;
        case kMajorTenth:
          result = kD_Natural_QTP;
          break;
        case kAugmentedTenth:
          result = kD_Sharp_QTP;
          break;
          
        case kDiminishedEleventh:
          result = kE_DoubleFlat_QTP;
          break;
        case kPerfectEleventh:
          result = kE_Flat_QTP;
          break;
        case kAugmentedEleventh:
          result = kE_Natural_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kF_Flat_QTP;
          break;
        case kPerfectTwelfth:
          result = kF_Natural_QTP;
          break;
        case kAugmentedTwelfth:
          result = kF_Sharp_QTP;
          break;
    
        case kDiminishedThirteenth:
          result = kG_DoubleFlat_QTP;
          break;
        case kMinorThirteenth:
          result = kG_Flat_QTP;
          break;
        case kMajorThirteenth:
          result = kG_Natural_QTP;
          break;
        case kAugmentedThirteenth:
          result = kG_Sharp_QTP;
          break;
      } // switch
      break;

    case kB_Natural_QTP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kB_Flat_QTP;
          break;
        case kPerfectUnison:
          result = kB_Natural_QTP;
          break;
        case kAugmentedUnison:
          result = kB_Sharp_QTP;
          break;
          
        case kDiminishedSecond:
          result = kC_Flat_QTP;
          break;
        case kMinorSecond:
          result = kC_Natural_QTP;
          break;
        case kMajorSecond:
          result = kC_Sharp_QTP;
          break;
        case kAugmentedSecond:
          result = kC_DoubleSharp_QTP;
          break;
          
        case kDiminishedThird:
          result = kD_Flat_QTP;
          break;
        case kMinorThird:
          result = kD_Natural_QTP;
          break;
        case kMajorThird:
          result = kD_Sharp_QTP;
          break;
        case kAugmentedThird:
          result = kD_DoubleSharp_QTP;
          break;
          
        case kDiminishedFourth:
          result = kE_Flat_QTP;
          break;
        case kPerfectFourth:
          result = kE_Natural_QTP;
          break;
        case kAugmentedFourth:
          result = kE_Sharp_QTP;
          break;
          
        case kDiminishedFifth:
          result = kF_Natural_QTP;
          break;
        case kPerfectFifth:
          result = kF_Sharp_QTP;
          break;
        case kAugmentedFifth:
          result = kF_DoubleSharp_QTP;
          break;
    
        case kDiminishedSixth:
          result = kG_Flat_QTP;
          break;
        case kMinorSixth:
          result = kG_Natural_QTP;
          break;
        case kMajorSixth:
          result = kG_Sharp_QTP;
          break;
        case kAugmentedSixth:
          result = kG_DoubleSharp_QTP;
          break;
          
        case kDiminishedSeventh:
          result = kA_Flat_QTP;
          break;
        case kMinorSeventh:
          result = kA_Natural_QTP;
          break;
        case kMajorSeventh:
          result = kA_Sharp_QTP;
          break;
        case kAugmentedSeventh:
          result = kA_DoubleSharp_QTP;
          break;
    
        case kDiminishedOctave:
          result = kB_Flat_QTP;
          break;
        case kPerfectOctave:
          result = kB_Natural_QTP;
          break;
        case kAugmentedOctave:
          result = kB_Sharp_QTP;
          break;
          
        case kDiminishedNinth:
          result = kC_Flat_QTP;
          break;
        case kMinorNinth:
          result = kC_Natural_QTP;
          break;
        case kMajorNinth:
          result = kC_Sharp_QTP;
          break;
        case kAugmentedNinth:
          result = kC_DoubleSharp_QTP;
          break;
          
        case kDiminishedTenth:
          result = kD_Flat_QTP;
          break;
        case kMinorTenth:
          result = kD_Natural_QTP;
          break;
        case kMajorTenth:
          result = kD_Sharp_QTP;
          break;
        case kAugmentedTenth:
          result = kD_DoubleSharp_QTP;
          break;
          
        case kDiminishedEleventh:
          result = kE_Flat_QTP;
          break;
        case kPerfectEleventh:
          result = kE_Natural_QTP;
          break;
        case kAugmentedEleventh:
          result = kE_Sharp_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kF_Natural_QTP;
          break;
        case kPerfectTwelfth:
          result = kF_Sharp_QTP;
          break;
        case kAugmentedTwelfth:
          result = kF_DoubleSharp_QTP;
          break;
    
        case kDiminishedThirteenth:
          result = kG_Flat_QTP;
          break;
        case kMinorThirteenth:
          result = kG_Natural_QTP;
          break;
        case kMajorThirteenth:
          result = kG_Sharp_QTP;
          break;
        case kAugmentedThirteenth:
          result = kG_DoubleSharp_QTP;
          break;
      } // switch
      break;

    case kB_Sharp_QTP:
      switch (intervalKind) {
        case k_NoIntervalKind:
          break;

        case kDiminishedUnisson:
          result = kB_Natural_QTP;
          break;
        case kPerfectUnison:
          result = kB_Sharp_QTP;
          break;
        case kAugmentedUnison:
          result = kB_DoubleSharp_QTP;
          break;
          
        case kDiminishedSecond:
          result = kC_Natural_QTP;
          break;
        case kMinorSecond:
          result = kC_Sharp_QTP;
          break;
        case kMajorSecond:
          result = kC_DoubleSharp_QTP;
          break;
        case kAugmentedSecond:
          result = kC_TripleSharp_QTP;
          break;
          
        case kDiminishedThird:
          result = kD_Natural_QTP;
          break;
        case kMinorThird:
          result = kD_Sharp_QTP;
          break;
        case kMajorThird:
          result = kD_DoubleSharp_QTP;
          break;
        case kAugmentedThird:
          result = kD_TripleSharp_QTP;
          break;
          
        case kDiminishedFourth:
          result = kE_Natural_QTP;
          break;
        case kPerfectFourth:
          result = kE_Sharp_QTP;
          break;
        case kAugmentedFourth:
          result = kE_DoubleSharp_QTP;
          break;
          
        case kDiminishedFifth:
          result = kF_Sharp_QTP;
          break;
        case kPerfectFifth:
          result = kF_DoubleSharp_QTP;
          break;
        case kAugmentedFifth:
          result = kF_TripleSharp_QTP;
          break;
    
        case kDiminishedSixth:
          result = kG_Natural_QTP;
          break;
        case kMinorSixth:
          result = kG_Sharp_QTP;
          break;
        case kMajorSixth:
          result = kG_DoubleSharp_QTP;
          break;
        case kAugmentedSixth:
          result = kG_TripleSharp_QTP;
          break;
          
        case kDiminishedSeventh:
          result = kA_Natural_QTP;
          break;
        case kMinorSeventh:
          result = kA_Sharp_QTP;
          break;
        case kMajorSeventh:
          result = kA_DoubleSharp_QTP;
          break;
        case kAugmentedSeventh:
          result = kA_TripleSharp_QTP;
          break;
    
        case kDiminishedOctave:
          result = kB_Natural_QTP;
          break;
        case kPerfectOctave:
          result = kB_Sharp_QTP;
          break;
        case kAugmentedOctave:
          result = kB_DoubleSharp_QTP;
          break;
          
        case kDiminishedNinth:
          result = kC_Natural_QTP;
          break;
        case kMinorNinth:
          result = kC_Sharp_QTP;
          break;
        case kMajorNinth:
          result = kC_DoubleSharp_QTP;
          break;
        case kAugmentedNinth:
          result = kC_TripleSharp_QTP;
          break;
          
        case kDiminishedTenth:
          result = kD_Natural_QTP;
          break;
        case kMinorTenth:
          result = kD_Sharp_QTP;
          break;
        case kMajorTenth:
          result = kD_DoubleSharp_QTP;
          break;
        case kAugmentedTenth:
          result = kD_TripleSharp_QTP;
          break;
          
        case kDiminishedEleventh:
          result = kE_Natural_QTP;
          break;
        case kPerfectEleventh:
          result = kE_Sharp_QTP;
          break;
        case kAugmentedEleventh:
          result = kE_DoubleSharp_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kF_Sharp_QTP;
          break;
        case kPerfectTwelfth:
          result = kF_DoubleSharp_QTP;
          break;
        case kAugmentedTwelfth:
          result = kF_TripleSharp_QTP;
          break;
    
        case kDiminishedThirteenth:
          result = kG_Natural_QTP;
          break;
        case kMinorThirteenth:
          result = kG_Sharp_QTP;
          break;
        case kMajorThirteenth:
          result = kG_DoubleSharp_QTP;
          break;
        case kAugmentedThirteenth:
          result = kG_TripleSharp_QTP;
          break;
      } // switch
      break;

      default:
      {
        // computing intervals on double or triple flats/sharps
        // is not considered useful, hence not supported
        stringstream s;

        s <<
          "Sorry, computing intervals from quartertones pitch '" <<
          msrQuarterTonesPitchKindAsString (
            gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
            quarterTonesPitchKind) <<
          "(" << quarterTonesPitchKind << ")" <<
          "' is not supported"
          ", line = " << inputLineNumber;

        msrLimitation (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
  } // switch

  return result;
}

msrIntervalKind intervalBetweenSemiTonesPitches (
  msrSemiTonesPitchKind semiTonesPitch1,
  msrSemiTonesPitchKind semiTonesPitch2)
{
  msrIntervalKind result = k_NoIntervalKind;

  msrSemiTonesPitchKind
    workSemiTonesPitch1 = semiTonesPitch1,
    workSemiTonesPitch2 = semiTonesPitch2;

  // order the semitones pitches so that
  // workSemiTonesPitch1 is greater or equal to workSemiTonesPitch2
  // according to the enum type
  bool invertInterval = false;
  
  if (semiTonesPitch1 < semiTonesPitch2) {
    workSemiTonesPitch1 = semiTonesPitch2;
    workSemiTonesPitch2 = semiTonesPitch1;
    invertInterval = true;
  }
    
  switch (workSemiTonesPitch1) {
    case k_NoSemiTonesPitch_STP:
      break;
      
    case kC_TripleFlat_STP:
      switch (workSemiTonesPitch2) {
        case k_NoSemiTonesPitch_STP:
          break;

 /*
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

*/
         
        case kC_TripleFlat_STP:
          break;
        case kC_DoubleFlat_STP:
          break;
        case kC_Flat_STP:
          break;
        case kC_Natural_STP:
          break;
        case kC_Sharp_STP:
          break;
        case kC_DoubleSharp_STP:
          break;
        case kC_TripleSharp_STP:
          break;
    
        case kD_TripleFlat_STP:
          break;
        case kD_DoubleFlat_STP:
          break;
        case kD_Flat_STP:
          break;
        case kD_Natural_STP:
          break;      
        case kD_Sharp_STP:
          break;
        case kD_DoubleSharp_STP:
          break;
        case kD_TripleSharp_STP:
          break;
          
        case kE_TripleFlat_STP:
          break;
        case kE_DoubleFlat_STP:
          break;
        case kE_Flat_STP:
          break;
        case kE_Natural_STP:
          break;
        case kE_Sharp_STP:
          break;
        case kE_DoubleSharp_STP:
          break;
        case kE_TripleSharp_STP:
          break;
          
        case kF_TripleFlat_STP:
          break;
        case kF_DoubleFlat_STP:
          break;
        case kF_Flat_STP:
          break;
        case kF_Natural_STP:
          break;
        case kF_Sharp_STP:
          break;      
        case kF_DoubleSharp_STP:
          break;
        case kF_TripleSharp_STP:
          break;
          
        case kG_TripleFlat_STP:
          break;
        case kG_DoubleFlat_STP:
          break;
        case kG_Flat_STP:
          break;
        case kG_Natural_STP:
          break;
        case kG_Sharp_STP:
          break;
        case kG_DoubleSharp_STP:
          break;
        case kG_TripleSharp_STP:
          break;
    
        case kA_TripleFlat_STP:
          break;
        case kA_DoubleFlat_STP:
          break;
        case kA_Flat_STP:
          break;
        case kA_Natural_STP:
          break;
        case kA_Sharp_STP:
          break;
        case kA_DoubleSharp_STP:
          break;
        case kA_TripleSharp_STP:
          break;
    
        case kB_TripleFlat_STP:
          break;
        case kB_DoubleFlat_STP:
          break;      
        case kB_Flat_STP:
          break;
        case kB_Natural_STP:
          break;
        case kB_Sharp_STP:
          break;
        case kB_DoubleSharp_STP:
          break;
        case kB_TripleSharp_STP:
          break;
      } // switch
      break;
     
    case kC_DoubleFlat_STP:
      break;
    case kC_Flat_STP:
      break;
    case kC_Natural_STP:
      break;
    case kC_Sharp_STP:
      break;
    case kC_DoubleSharp_STP:
      break;
    case kC_TripleSharp_STP:
      break;

    case kD_TripleFlat_STP:
      break;
    case kD_DoubleFlat_STP:
      break;
    case kD_Flat_STP:
      break;
    case kD_Natural_STP:
      break;      
    case kD_Sharp_STP:
      break;
    case kD_DoubleSharp_STP:
      break;
    case kD_TripleSharp_STP:
      break;
      
    case kE_TripleFlat_STP:
      break;
    case kE_DoubleFlat_STP:
      break;
    case kE_Flat_STP:
      break;
    case kE_Natural_STP:
      break;
    case kE_Sharp_STP:
      break;
    case kE_DoubleSharp_STP:
      break;
    case kE_TripleSharp_STP:
      break;
      
    case kF_TripleFlat_STP:
      break;
    case kF_DoubleFlat_STP:
      break;
    case kF_Flat_STP:
      break;
    case kF_Natural_STP:
      break;
    case kF_Sharp_STP:
      break;      
    case kF_DoubleSharp_STP:
      break;
    case kF_TripleSharp_STP:
      break;
      
    case kG_TripleFlat_STP:
      break;
    case kG_DoubleFlat_STP:
      break;
    case kG_Flat_STP:
      break;
    case kG_Natural_STP:
      break;
    case kG_Sharp_STP:
      break;
    case kG_DoubleSharp_STP:
      break;
    case kG_TripleSharp_STP:
      break;

    case kA_TripleFlat_STP:
      break;
    case kA_DoubleFlat_STP:
      break;
    case kA_Flat_STP:
      break;
    case kA_Natural_STP:
      break;
    case kA_Sharp_STP:
      break;
    case kA_DoubleSharp_STP:
      break;
    case kA_TripleSharp_STP:
      break;

    case kB_TripleFlat_STP:
      break;
    case kB_DoubleFlat_STP:
      break;      
    case kB_Flat_STP:
      break;
    case kB_Natural_STP:
      break;
    case kB_Sharp_STP:
      break;

    case kB_DoubleSharp_STP:
      switch (workSemiTonesPitch2) {
        case k_NoSemiTonesPitch_STP:
          break;

 /*
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

*/
         
        case kC_TripleFlat_STP:
          break;
        case kC_DoubleFlat_STP:
          break;
        case kC_Flat_STP:
          break;
        case kC_Natural_STP:
          break;
        case kC_Sharp_STP:
          break;
        case kC_DoubleSharp_STP:
          break;
        case kC_TripleSharp_STP:
          break;
    
        case kD_TripleFlat_STP:
          break;
        case kD_DoubleFlat_STP:
          break;
        case kD_Flat_STP:
          break;
        case kD_Natural_STP:
          break;      
        case kD_Sharp_STP:
          break;
        case kD_DoubleSharp_STP:
          break;
        case kD_TripleSharp_STP:
          break;
          
        case kE_TripleFlat_STP:
          break;
        case kE_DoubleFlat_STP:
          break;
        case kE_Flat_STP:
          break;
        case kE_Natural_STP:
          break;
        case kE_Sharp_STP:
          break;
        case kE_DoubleSharp_STP:
          break;
        case kE_TripleSharp_STP:
          break;
          
        case kF_TripleFlat_STP:
          break;
        case kF_DoubleFlat_STP:
          break;
        case kF_Flat_STP:
          break;
        case kF_Natural_STP:
          break;
        case kF_Sharp_STP:
          break;      
        case kF_DoubleSharp_STP:
          break;
        case kF_TripleSharp_STP:
          break;
          
        case kG_TripleFlat_STP:
          break;
        case kG_DoubleFlat_STP:
          break;
        case kG_Flat_STP:
          break;
        case kG_Natural_STP:
          break;
        case kG_Sharp_STP:
          break;
        case kG_DoubleSharp_STP:
          break;
        case kG_TripleSharp_STP:
          break;
    
        case kA_TripleFlat_STP:
          break;
        case kA_DoubleFlat_STP:
          break;
        case kA_Flat_STP:
          break;
        case kA_Natural_STP:
          break;
        case kA_Sharp_STP:
          break;
        case kA_DoubleSharp_STP:
          break;
        case kA_TripleSharp_STP:
          break;
    
        case kB_TripleFlat_STP:
          break;
        case kB_DoubleFlat_STP:
          break;      
        case kB_Flat_STP:
          break;
        case kB_Natural_STP:
          break;
        case kB_Sharp_STP:
          result = kAugmentedUnison;
          break;
        case kB_DoubleSharp_STP:
          result = kPerfectUnison;
          break;
        case kB_TripleSharp_STP:
          result = kPerfectUnison;
          break;
      } // switch
      break;

    case kB_TripleSharp_STP:
      switch (workSemiTonesPitch2) {
        case k_NoSemiTonesPitch_STP:
          break;

 /*
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

*/
         
        case kC_TripleFlat_STP:
          break;
        case kC_DoubleFlat_STP:
          break;
        case kC_Flat_STP:
          break;
        case kC_Natural_STP:
          break;
        case kC_Sharp_STP:
          break;
        case kC_DoubleSharp_STP:
          break;
        case kC_TripleSharp_STP:
          break;
    
        case kD_TripleFlat_STP:
          break;
        case kD_DoubleFlat_STP:
          break;
        case kD_Flat_STP:
          break;
        case kD_Natural_STP:
          break;      
        case kD_Sharp_STP:
          break;
        case kD_DoubleSharp_STP:
          break;
        case kD_TripleSharp_STP:
          break;
          
        case kE_TripleFlat_STP:
          break;
        case kE_DoubleFlat_STP:
          break;
        case kE_Flat_STP:
          break;
        case kE_Natural_STP:
          break;
        case kE_Sharp_STP:
          break;
        case kE_DoubleSharp_STP:
          break;
        case kE_TripleSharp_STP:
          break;
          
        case kF_TripleFlat_STP:
          break;
        case kF_DoubleFlat_STP:
          break;
        case kF_Flat_STP:
          break;
        case kF_Natural_STP:
          break;
        case kF_Sharp_STP:
          break;      
        case kF_DoubleSharp_STP:
          break;
        case kF_TripleSharp_STP:
          break;
          
        case kG_TripleFlat_STP:
          break;
        case kG_DoubleFlat_STP:
          break;
        case kG_Flat_STP:
          break;
        case kG_Natural_STP:
          break;
        case kG_Sharp_STP:
          break;
        case kG_DoubleSharp_STP:
          break;
        case kG_TripleSharp_STP:
          break;
    
        case kA_TripleFlat_STP:
          break;
        case kA_DoubleFlat_STP:
          break;
        case kA_Flat_STP:
          break;
        case kA_Natural_STP:
          break;
        case kA_Sharp_STP:
          break;
        case kA_DoubleSharp_STP:
          break;
        case kA_TripleSharp_STP:
          break;
    
        case kB_TripleFlat_STP:
          break;
        case kB_DoubleFlat_STP:
          break;      
        case kB_Flat_STP:
          break;
        case kB_Natural_STP:
          break;
        case kB_Sharp_STP:
          break;
        case kB_DoubleSharp_STP:
          result = kAugmentedUnison;
          break;
        case kB_TripleSharp_STP:
          result = kPerfectUnison;
          break;
      } // switch
      break;
  } // switch

  if (invertInterval) {
    result = invertIntervalKind (result);
  }
  
  return result;
}

// harmonies
//______________________________________________________________________________
string msrHarmonyKindAsString (
  msrHarmonyKind harmonyKind)
{
  string result;
  
  switch (harmonyKind) {
    case k_NoHarmony:
      result = "noHarmony";
      break;

    // MusicXML chords

    case kMajorHarmony:
      result = "major";
      break;
    case kMinorHarmony:
      result = "minor";
      break;
    case kAugmentedHarmony:
      result = "augmented";
      break;
    case kDiminishedHarmony:
      result = "diminished";
      break;

    case kDominantHarmony:
      result = "dominant";
      break;
    case kMajorSeventhHarmony:
      result = "majorSeventh";
      break;
    case kMinorSeventhHarmony:
      result = "minorSeventh";
      break;
    case kDiminishedSeventhHarmony:
      result = "diminishedSeventh";
      break;
    case kAugmentedSeventhHarmony:
      result = "augmentedSeventh";
      break;
    case kHalfDiminishedHarmony:
      result = "halfDiminished";
      break;
    case kMinorMajorSeventhHarmony:
      result = "minorMajorSeventhHarmony";
      break;

    case kMajorSixthHarmony:
      result = "majorSixth";
      break;
    case kMinorSixthHarmony:
      result = "minorSixth";
      break;

    case kDominantNinthHarmony:
      result = "dominantNinth";
      break;
    case kMajorNinthHarmony:
      result = "majorNinth";
      break;
    case kMinorNinthHarmony:
      result = "minorNinth";
      break;

    case kDominantEleventhHarmony:
      result = "dominantEleventh";
      break;
    case kMajorEleventhHarmony:
      result = "majorEleventh";
      break;
    case kMinorEleventhHarmony:
      result = "minorEleventh";
      break;

    case kDominantThirteenthHarmony:
      result = "dominantThirteenth";
      break;
    case kMajorThirteenthHarmony:
      result = "majorThirteenth";
      break;
    case kMinorThirteenthHarmony:
      result = "minorThirteenth";
      break;

    case kSuspendedSecondHarmony:
      result = "suspendedSecond";
      break;
    case kSuspendedFourthHarmony:
      result = "suspendedFourth";
      break;
      
    case kNeapolitanHarmony:
      result = "neapolitan";
      break;
    case kItalianHarmony:
      result = "italian";
      break;
    case kFrenchHarmony:
      result = "french";
      break;
    case kGermanHarmony:
      result = "german";
      break;

    case kPedalHarmony:
      result = "pedal";
      break;
    case kPowerHarmony:
      result = "power";
      break;
    case kTristanHarmony:
      result = "tristan";
      break;
      
    // jazz-specific chords

    case kMinorMajorNinth: // -maj9, minmaj9
      result = "minorMajorNinth";
      break;
    
    case kDominantSuspendedFourthHarmony: // 7sus4, domsus4
      result = "dominantSuspendedFourthHarmony";
      break;
    case kDominantAugmentedFifthHarmony: // 7#5, domaug5
      result = "dominantAugmentedFifthHarmony";
      break;
    case kDominantMinorNinthHarmony: // 7b9, dommin9
      result = "dominantMinorNinthHarmony";
      break;
    case kDominantAugmentedNinthDiminishedFifthHarmony: // 7#9b5, domaug9dim5
      result = "dominantAugmentedNinthDiminishedFifthHarmony";
      break;
    case kDominantAugmentedNinthAugmentedFifthHarmony: // 7#9#5, domaug9aug5
      result = "dominantAugmentedNinthAugmentedFifthHarmony";
      break;
    case kDominantAugmentedEleventhHarmony: // 7#11, domaug11
      result = "dominantAugmentedEleventhHarmony";
      break;
    
    case kMajorSeventhAugmentedEleventhHarmony: // maj7#11, maj7aug11
      result = "majorSeventhAugmentedEleventhHarmony";
      break;

    // other
    
    case kOtherHarmony:
      result = "other";
      break;
    case kNoneHarmony:
      result = "none";
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
      result = "noHarmony";
      break;

    // MusicXML chords

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
    case kMinorMajorSeventhHarmony:
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
      result = "∆7/11";
      break;
    case kMinorEleventhHarmony:
      result = "m∆7/11";
      break;

    case kDominantThirteenthHarmony:
      result = "13";
      break;
    case kMajorThirteenthHarmony:
      result = "∆7/13";
      break;
    case kMinorThirteenthHarmony:
      result = "m∆7/13";
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

    // jazz-specific chords

    case kMinorMajorNinth: // -maj9, minmaj9
      result = "m∆9";
      break;
    
    case kDominantSuspendedFourthHarmony: // 7sus4, domsus4
      result = "7sus4";
      break;
    case kDominantAugmentedFifthHarmony: // 7#5, domaug5
      result = "7#5";
      break;
    case kDominantMinorNinthHarmony: // 7b9, dommin9
      result = "7b9";
      break;
    case kDominantAugmentedNinthDiminishedFifthHarmony: // 7#9b5, domaug9dim5
      result = "7#9b5";
      break;
    case kDominantAugmentedNinthAugmentedFifthHarmony: // 7#9#5, domaug9aug5
      result = "7#9#5";
      break;
    case kDominantAugmentedEleventhHarmony: // 7#11, domaug11
      result = "7#11";
      break;
    
    case kMajorSeventhAugmentedEleventhHarmony: // maj7#11, maj7aug11
      result = "maj7#11";
      break;

    // other
    
    case kOtherHarmony:
      result = "Other";
      break;
    case kNoneHarmony:
      result = "None";
      break;
  } // switch

  return result;
}

string msrHarmonyKindShortName (
  msrHarmonyKind harmonyKind)
{
  string result;
  
  switch (harmonyKind) {
    case k_NoHarmony:
      result = "noHarmony";
      break;

    // MusicXML chords

    case kMajorHarmony:
      result = "maj";
      break;
    case kMinorHarmony:
      result = "min";
      break;
    case kAugmentedHarmony:
      result = "aug";
      break;
    case kDiminishedHarmony:
      result = "dim";
      break;

    case kDominantHarmony:
      result = "dom";
      break;
    case kMajorSeventhHarmony:
      result = "maj";
      break;
    case kMinorSeventhHarmony:
      result = "min7";
      break;
    case kDiminishedSeventhHarmony:
      result = "dim7";
      break;
    case kAugmentedSeventhHarmony:
      result = "aug7";
      break;
    case kHalfDiminishedHarmony:
      result = "halfdim";
      break;
    case kMinorMajorSeventhHarmony:
      result = "minmaj7";
      break;

    case kMajorSixthHarmony:
      result = "maj6";
      break;
    case kMinorSixthHarmony:
      result = "min6";
      break;

    case kDominantNinthHarmony:
      result = "dom9";
      break;
    case kMajorNinthHarmony:
      result = "maj9";
      break;
    case kMinorNinthHarmony:
      result = "min9";
      break;

    case kDominantEleventhHarmony:
      result = "dom11";
      break;
    case kMajorEleventhHarmony:
      result = "maj11";
      break;
    case kMinorEleventhHarmony:
      result = "min11";
      break;

    case kDominantThirteenthHarmony:
      result = "dom13";
      break;
    case kMajorThirteenthHarmony:
      result = "maj13";
      break;
    case kMinorThirteenthHarmony:
      result = "min13";
      break;

    case kSuspendedSecondHarmony:
      result = "sus2";
      break;
    case kSuspendedFourthHarmony:
      result = "sus4";
      break;
      
    case kNeapolitanHarmony:
      result = "neapolitan";
      break;
    case kItalianHarmony:
      result = "ttalian";
      break;
    case kFrenchHarmony:
      result = "french";
      break;
    case kGermanHarmony:
      result = "german";
      break;

    case kPedalHarmony:
      result = "pedal";
      break;
    case kPowerHarmony:
      result = "power";
      break;
    case kTristanHarmony:
      result = "tristan";
      break;

    // jazz-specific chords

    case kMinorMajorNinth: // -maj9, minmaj9
      result = "minmaj9";
      break;
    
    case kDominantSuspendedFourthHarmony: // 7sus4, domsus4
      result = "domsus4";
      break;
    case kDominantAugmentedFifthHarmony: // 7#5, domaug5
      result = "domaug5";
      break;
    case kDominantMinorNinthHarmony: // 7b9, dommin9
      result = "dommin9";
      break;
    case kDominantAugmentedNinthDiminishedFifthHarmony: // 7#9b5, domaug9dim5
      result = "domaug9dim5";
      break;
    case kDominantAugmentedNinthAugmentedFifthHarmony: // 7#9#5, domaug9aug5
      result = "domaug9aug5";
      break;
    case kDominantAugmentedEleventhHarmony: // 7#11, domaug11
      result = "domaug11";
      break;
    
    case kMajorSeventhAugmentedEleventhHarmony: // maj7#11, maj7aug11
      result = "maj7aug11";
      break;

    // other
    
    case kOtherHarmony:
      result = "other";
      break;
    case kNoneHarmony:
      result = "none";
      break;
  } // switch

  return result;
}

msrHarmonyKind msrHarmonyKindFromString (
  string theString)
{
  msrHarmonyKind result = k_NoHarmony;

  // MusicXML chords

  if (theString == "maj") {
    result = kMajorHarmony;
  }
  else if (theString == "min") {
    result = kMinorHarmony;
  }
  else if (theString == "aug") {
    result = kAugmentedHarmony;
  }
  else if (theString == "dim") {
    result = kDiminishedHarmony;
  }
  else if (theString == "dom") {
    result = kDominantHarmony;
  }
  else if (theString == "maj7") {
    result = kMajorSeventhHarmony;
  }
  else if (theString == "min7") {
    result = kMinorSeventhHarmony;
  }
  else if (theString == "dim7") {
    result = kDiminishedSeventhHarmony;
  }
  else if (theString == "aug7") {
    result = kAugmentedSeventhHarmony;
  }
  else if (theString == "halfdim") {
    result = kHalfDiminishedHarmony;
  }
  else if (theString == "minmaj7") {
    result = kMinorMajorSeventhHarmony;
  }
  else if (theString == "maj6") {
    result = kMajorSixthHarmony;
  }
  else if (theString == "min6") {
    result = kMinorSixthHarmony;
  }
  else if (theString == "dom9") {
    result = kDominantNinthHarmony;
  }
  else if (theString == "maj9") {
    result = kMajorNinthHarmony;
  }
  else if (theString == "min9") {
    result = kMinorNinthHarmony;
  }
  else if (theString == "dom11") {
    result = kDominantEleventhHarmony;
  }
  else if (theString == "maj11") {
    result = kMajorEleventhHarmony;
  }
  else if (theString == "min11") {
    result = kMinorEleventhHarmony;
  }
  else if (theString == "dom13") {
    result = kDominantThirteenthHarmony;
  }
  else if (theString == "maj13") {
    result = kMajorThirteenthHarmony;
  }
  else if (theString == "min13") {
    result = kMinorThirteenthHarmony;
  }
  else if (theString == "sus2") {
    result = kSuspendedSecondHarmony;
  }
  else if (theString == "sus4") {
    result = kSuspendedFourthHarmony;
  }
  else if (theString == "neapolitan") {
    result = kNeapolitanHarmony;
  }
  else if (theString == "italian") {
    result = kItalianHarmony;
  }
  else if (theString == "french") {
    result = kFrenchHarmony;
  }
  else if (theString == "german") {
    result = kGermanHarmony;
  }
  else if (theString == "pedal") {
    result = kPedalHarmony;
  }
  else if (theString == "power") {
    result = kPowerHarmony;
  }
  else if (theString == "tristan") {
    result = kTristanHarmony;
  }

  // jazz-specific chords

  else if (theString == "minmaj9") {
    result = kMinorMajorNinth;                              // -maj9, minmaj9
  }
  else if (theString == "domsus4") {
    result = kDominantSuspendedFourthHarmony;               // 7sus4, domsus4
  }
  else if (theString == "domaug5") {
    result = kDominantAugmentedFifthHarmony;                // 7#5, domaug5
  }
  else if (theString == "dommin9") {
    result = kDominantMinorNinthHarmony;                    // 7b9, dommin9
  }
  else if (theString == "domaug9dim5") {
    result = kDominantAugmentedNinthDiminishedFifthHarmony; // 7#9b5, domaug9dim5
  }
  else if (theString == "domaug9aug5") {
    result = kDominantAugmentedNinthAugmentedFifthHarmony;  // 7#9#5, domaug9aug5
  }
  else if (theString == "domaug11") {
    result = kDominantAugmentedEleventhHarmony;             // 7#11, domaug11
  }
  else if (theString == "maj7aug11") {  
    result = kMajorSeventhAugmentedEleventhHarmony;         // maj7#11, maj7aug11
  }

  return result;
}

void initializeChordStructuresMap ()
{
  for (int i = k_NoHarmony; i <= kNoneHarmony; i++) {
    msrHarmonyKind
      harmonyKind =
        msrHarmonyKind (i);

    // create the chord structure
    S_msrChordStructure
      chordStructure =
        msrChordStructure::create (
          harmonyKind);

    // register it in the map
    gChordStructuresMap [harmonyKind] =
      chordStructure;
  } // for
}

void printChordStructuresMap ()
{
  gLogIOstream <<
    "Harmonies chords structures:" <<
    " (" << gChordStructuresMap.size () << ")" <<
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
    
    S_msrChordStructure
      chordStructure =
        gChordStructuresMap [harmonyKind];

    if (chordStructure) {
      gLogIOstream <<
        chordStructure <<
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

map<msrQuarterTonesPitchKind, string> gNederlandsPitchNamesMap;
map<msrQuarterTonesPitchKind, string> gCatalanPitchNamesMap;
map<msrQuarterTonesPitchKind, string> gDeutschPitchNamesMap;
map<msrQuarterTonesPitchKind, string> gEnglishPitchNamesMap;
map<msrQuarterTonesPitchKind, string> gEspanolPitchNamesMap;
map<msrQuarterTonesPitchKind, string> gFrancaisPitchNamesMap;
map<msrQuarterTonesPitchKind, string> gItalianoPitchNamesMap;
map<msrQuarterTonesPitchKind, string> gNorskPitchNamesMap;
map<msrQuarterTonesPitchKind, string> gPortuguesPitchNamesMap;
map<msrQuarterTonesPitchKind, string> gSuomiPitchNamesMap;
map<msrQuarterTonesPitchKind, string> gSvenskaPitchNamesMap;
map<msrQuarterTonesPitchKind, string> gVlaamsPitchNamesMap;

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
  gNederlandsPitchNamesMap [k_NoQuarterTonesPitch_QTP]  = "";
  gNederlandsPitchNamesMap [k_Rest_QTP]                 = "r";
  
  gNederlandsPitchNamesMap [kA_DoubleFlat_QTP]  = "aeses";
  gNederlandsPitchNamesMap [kA_SesquiFlat_QTP]  = "aeseh";
  gNederlandsPitchNamesMap [kA_Flat_QTP]        = "aes";
  gNederlandsPitchNamesMap [kA_SemiFlat_QTP]    = "aeh";
  gNederlandsPitchNamesMap [kA_Natural_QTP]     = "a";
  gNederlandsPitchNamesMap [kA_SemiSharp_QTP]   = "aih";
  gNederlandsPitchNamesMap [kA_Sharp_QTP]       = "ais";
  gNederlandsPitchNamesMap [kA_SesquiSharp_QTP] = "aisih";
  gNederlandsPitchNamesMap [kA_DoubleSharp_QTP] = "aisis";
    
  gNederlandsPitchNamesMap [kB_DoubleFlat_QTP]  = "beses";
  gNederlandsPitchNamesMap [kB_SesquiFlat_QTP]  = "beseh";
  gNederlandsPitchNamesMap [kB_Flat_QTP]        = "bes";
  gNederlandsPitchNamesMap [kB_SemiFlat_QTP]    = "beh";
  gNederlandsPitchNamesMap [kB_Natural_QTP]     = "b";
  gNederlandsPitchNamesMap [kB_SemiSharp_QTP]   = "bih";
  gNederlandsPitchNamesMap [kB_Sharp_QTP]       = "bis";
  gNederlandsPitchNamesMap [kB_SesquiSharp_QTP] = "bisih";
  gNederlandsPitchNamesMap [kB_DoubleSharp_QTP] = "bisis";
    
  gNederlandsPitchNamesMap [kC_DoubleFlat_QTP]  = "ceses";
  gNederlandsPitchNamesMap [kC_SesquiFlat_QTP]  = "ceseh";
  gNederlandsPitchNamesMap [kC_Flat_QTP]        = "ces";
  gNederlandsPitchNamesMap [kC_SemiFlat_QTP]    = "ceh";
  gNederlandsPitchNamesMap [kC_Natural_QTP]     = "c";
  gNederlandsPitchNamesMap [kC_SemiSharp_QTP]   = "cih";
  gNederlandsPitchNamesMap [kC_Sharp_QTP]       = "cis";
  gNederlandsPitchNamesMap [kC_SesquiSharp_QTP] = "cisih";
  gNederlandsPitchNamesMap [kC_DoubleSharp_QTP] = "cisis";
    
  gNederlandsPitchNamesMap [kD_DoubleFlat_QTP]  = "deses";
  gNederlandsPitchNamesMap [kD_SesquiFlat_QTP]  = "deseh";
  gNederlandsPitchNamesMap [kD_Flat_QTP]        = "des";
  gNederlandsPitchNamesMap [kD_SemiFlat_QTP]    = "deh";
  gNederlandsPitchNamesMap [kD_Natural_QTP]     = "d";
  gNederlandsPitchNamesMap [kD_SemiSharp_QTP]   = "dih";
  gNederlandsPitchNamesMap [kD_Sharp_QTP]       = "dis";
  gNederlandsPitchNamesMap [kD_SesquiSharp_QTP] = "disih";
  gNederlandsPitchNamesMap [kD_DoubleSharp_QTP] = "disis";

  gNederlandsPitchNamesMap [kE_DoubleFlat_QTP]  = "eeses";
  gNederlandsPitchNamesMap [kE_SesquiFlat_QTP]  = "eeseh";
  gNederlandsPitchNamesMap [kE_Flat_QTP]        = "ees";
  gNederlandsPitchNamesMap [kE_SemiFlat_QTP]    = "eeh";
  gNederlandsPitchNamesMap [kE_Natural_QTP]     = "e";
  gNederlandsPitchNamesMap [kE_SemiSharp_QTP]   = "eih";
  gNederlandsPitchNamesMap [kE_Sharp_QTP]       = "eis";
  gNederlandsPitchNamesMap [kE_SesquiSharp_QTP] = "eisih";
  gNederlandsPitchNamesMap [kE_DoubleSharp_QTP] = "eisis";
    
  gNederlandsPitchNamesMap [kF_DoubleFlat_QTP]  = "feses";
  gNederlandsPitchNamesMap [kF_SesquiFlat_QTP]  = "feseh";
  gNederlandsPitchNamesMap [kF_Flat_QTP]        = "fes";
  gNederlandsPitchNamesMap [kF_SemiFlat_QTP]    = "feh";
  gNederlandsPitchNamesMap [kF_Natural_QTP]     = "f";
  gNederlandsPitchNamesMap [kF_SemiSharp_QTP]   = "fih";
  gNederlandsPitchNamesMap [kF_Sharp_QTP]       = "fis";
  gNederlandsPitchNamesMap [kF_SesquiSharp_QTP] = "fisih";
  gNederlandsPitchNamesMap [kF_DoubleSharp_QTP] = "fisis";
    
  gNederlandsPitchNamesMap [kG_DoubleFlat_QTP]  = "geses";
  gNederlandsPitchNamesMap [kG_SesquiFlat_QTP]  = "geseh";
  gNederlandsPitchNamesMap [kG_Flat_QTP]        = "ges";
  gNederlandsPitchNamesMap [kG_SemiFlat_QTP]    = "geh";
  gNederlandsPitchNamesMap [kG_Natural_QTP]     = "g";
  gNederlandsPitchNamesMap [kG_SemiSharp_QTP]   = "gih";
  gNederlandsPitchNamesMap [kG_Sharp_QTP]       = "gis";
  gNederlandsPitchNamesMap [kG_SesquiSharp_QTP] = "gisih";
  gNederlandsPitchNamesMap [kG_DoubleSharp_QTP] = "gisis";

  // catalan
  gCatalanPitchNamesMap [k_NoQuarterTonesPitch_QTP]  = "";
  gCatalanPitchNamesMap [k_Rest_QTP]                 = "r";
  
  gCatalanPitchNamesMap [kA_DoubleFlat_QTP]  = "labb";
  gCatalanPitchNamesMap [kA_SesquiFlat_QTP]  = "labSesquiFlat???";
  gCatalanPitchNamesMap [kA_Flat_QTP]        = "lab";
  gCatalanPitchNamesMap [kA_SemiFlat_QTP]    = "aSemiFlat???";
  gCatalanPitchNamesMap [kA_Natural_QTP]     = "la";
  gCatalanPitchNamesMap [kA_SemiSharp_QTP]   = "aSemiSharp???";
  gCatalanPitchNamesMap [kA_Sharp_QTP]       = "lad";
  gCatalanPitchNamesMap [kA_SesquiSharp_QTP] = "laSesquiSharp???";
  gCatalanPitchNamesMap [kA_DoubleSharp_QTP] = "ladd";
    
  gCatalanPitchNamesMap [kB_DoubleFlat_QTP]  = "sibb";
  gCatalanPitchNamesMap [kB_SesquiFlat_QTP]  = "sibSesquiFlat???";
  gCatalanPitchNamesMap [kB_Flat_QTP]        = "sib";
  gCatalanPitchNamesMap [kB_SemiFlat_QTP]    = "bSemiFlat???";
  gCatalanPitchNamesMap [kB_Natural_QTP]     = "b";
  gCatalanPitchNamesMap [kB_SemiSharp_QTP]   = "bSemiSharp???";
  gCatalanPitchNamesMap [kB_Sharp_QTP]       = "sid";
  gCatalanPitchNamesMap [kB_SesquiSharp_QTP] = "siSesquiSharp???";
  gCatalanPitchNamesMap [kB_DoubleSharp_QTP] = "sidd";
    
  gCatalanPitchNamesMap [kC_DoubleFlat_QTP]  = "dobb";
  gCatalanPitchNamesMap [kC_SesquiFlat_QTP]  = "doSesquiFlat???";
  gCatalanPitchNamesMap [kC_Flat_QTP]        = "dob";
  gCatalanPitchNamesMap [kC_SemiFlat_QTP]    = "cSemiFlat???";
  gCatalanPitchNamesMap [kC_Natural_QTP]     = "do";
  gCatalanPitchNamesMap [kC_SemiSharp_QTP]   = "cSemiSharp???";
  gCatalanPitchNamesMap [kC_Sharp_QTP]       = "dod";
  gCatalanPitchNamesMap [kC_SesquiSharp_QTP] = "doSesquiSharp???";
  gCatalanPitchNamesMap [kC_DoubleSharp_QTP] = "dodd";
    
  gCatalanPitchNamesMap [kD_DoubleFlat_QTP]  = "rebb";
  gCatalanPitchNamesMap [kD_SesquiFlat_QTP]  = "reSesquiFlat???";
  gCatalanPitchNamesMap [kD_Flat_QTP]        = "reb";
  gCatalanPitchNamesMap [kD_SemiFlat_QTP]    = "dSemiFlat???";
  gCatalanPitchNamesMap [kD_Natural_QTP]     = "re";
  gCatalanPitchNamesMap [kD_SemiSharp_QTP]   = "dSemiSharp???";
  gCatalanPitchNamesMap [kD_Sharp_QTP]       = "red";
  gCatalanPitchNamesMap [kD_SesquiSharp_QTP] = "reSesquiSharp???";
  gCatalanPitchNamesMap [kD_DoubleSharp_QTP] = "redd";

  gCatalanPitchNamesMap [kE_DoubleFlat_QTP]  = "mibb";
  gCatalanPitchNamesMap [kE_SesquiFlat_QTP]  = "miSesquiFlat???";
  gCatalanPitchNamesMap [kE_Flat_QTP]        = "mib";
  gCatalanPitchNamesMap [kE_SemiFlat_QTP]    = "eSemiFlat???";
  gCatalanPitchNamesMap [kE_Natural_QTP]     = "mi";
  gCatalanPitchNamesMap [kE_SemiSharp_QTP]   = "eSemiSharp???";
  gCatalanPitchNamesMap [kE_Sharp_QTP]       = "mid";
  gCatalanPitchNamesMap [kE_SesquiSharp_QTP] = "miSesquiSharp???";
  gCatalanPitchNamesMap [kE_DoubleSharp_QTP] = "midd";
    
  gCatalanPitchNamesMap [kF_DoubleFlat_QTP]  = "fabb";
  gCatalanPitchNamesMap [kF_SesquiFlat_QTP]  = "faSesquiFlat???";
  gCatalanPitchNamesMap [kF_Flat_QTP]        = "fab";
  gCatalanPitchNamesMap [kF_SemiFlat_QTP]    = "fSemiFlat???";
  gCatalanPitchNamesMap [kF_Natural_QTP]     = "fa";
  gCatalanPitchNamesMap [kF_SemiSharp_QTP]   = "fSemiSharp???";
  gCatalanPitchNamesMap [kF_Sharp_QTP]       = "fad";
  gCatalanPitchNamesMap [kF_SesquiSharp_QTP] = "faSesquiSharp???";
  gCatalanPitchNamesMap [kF_DoubleSharp_QTP] = "fadd";
    
  gCatalanPitchNamesMap [kG_DoubleFlat_QTP]  = "solbb";
  gCatalanPitchNamesMap [kG_SesquiFlat_QTP]  = "solSesquiFlat???";
  gCatalanPitchNamesMap [kG_Flat_QTP]        = "solb";
  gCatalanPitchNamesMap [kG_SemiFlat_QTP]    = "gSemiFlat???";
  gCatalanPitchNamesMap [kG_Natural_QTP]     = "sol";
  gCatalanPitchNamesMap [kG_SemiSharp_QTP]   = "gSemiSharp???";
  gCatalanPitchNamesMap [kG_Sharp_QTP]       = "sold";
  gCatalanPitchNamesMap [kG_SesquiSharp_QTP] = "solSesquiSharp???";
  gCatalanPitchNamesMap [kG_DoubleSharp_QTP] = "soldd";

  // deutsch
  gDeutschPitchNamesMap [k_NoQuarterTonesPitch_QTP]  = "";
  gDeutschPitchNamesMap [k_Rest_QTP]                 = "r";
  
  gDeutschPitchNamesMap [kA_DoubleFlat_QTP]  = "asas";
  gDeutschPitchNamesMap [kA_SesquiFlat_QTP]  = "asah";
  gDeutschPitchNamesMap [kA_Flat_QTP]        = "as";
  gDeutschPitchNamesMap [kA_SemiFlat_QTP]    = "aeh";
  gDeutschPitchNamesMap [kA_Natural_QTP]     = "a";
  gDeutschPitchNamesMap [kA_SemiSharp_QTP]   = "aih";
  gDeutschPitchNamesMap [kA_Sharp_QTP]       = "ais";
  gDeutschPitchNamesMap [kA_SesquiSharp_QTP] = "aisih";
  gDeutschPitchNamesMap [kA_DoubleSharp_QTP] = "aisis";
    
  gDeutschPitchNamesMap [kB_DoubleFlat_QTP]  = "heses";
  gDeutschPitchNamesMap [kB_SesquiFlat_QTP]  = "heseh";
  gDeutschPitchNamesMap [kB_Flat_QTP]        = "b";
  gDeutschPitchNamesMap [kB_SemiFlat_QTP]    = "beh";
  gDeutschPitchNamesMap [kB_Natural_QTP]     = "h";
  gDeutschPitchNamesMap [kB_SemiSharp_QTP]   = "hih";
  gDeutschPitchNamesMap [kB_Sharp_QTP]       = "his";
  gDeutschPitchNamesMap [kB_SesquiSharp_QTP] = "hisih";
  gDeutschPitchNamesMap [kB_DoubleSharp_QTP] = "hisis";
    
  gDeutschPitchNamesMap [kC_DoubleFlat_QTP]  = "ceses";
  gDeutschPitchNamesMap [kC_SesquiFlat_QTP]  = "ceseh";
  gDeutschPitchNamesMap [kC_Flat_QTP]        = "ces";
  gDeutschPitchNamesMap [kC_SemiFlat_QTP]    = "ceh";
  gDeutschPitchNamesMap [kC_Natural_QTP]     = "c";
  gDeutschPitchNamesMap [kC_SemiSharp_QTP]   = "cih";
  gDeutschPitchNamesMap [kC_Sharp_QTP]       = "cis";
  gDeutschPitchNamesMap [kC_SesquiSharp_QTP] = "cisih";
  gDeutschPitchNamesMap [kC_DoubleSharp_QTP] = "cisis";
    
  gDeutschPitchNamesMap [kD_DoubleFlat_QTP]  = "deses";
  gDeutschPitchNamesMap [kD_SesquiFlat_QTP]  = "deseh";
  gDeutschPitchNamesMap [kD_Flat_QTP]        = "des";
  gDeutschPitchNamesMap [kD_SemiFlat_QTP]    = "deh";
  gDeutschPitchNamesMap [kD_Natural_QTP]     = "d";
  gDeutschPitchNamesMap [kD_SemiSharp_QTP]   = "dih";
  gDeutschPitchNamesMap [kD_Sharp_QTP]       = "dis";
  gDeutschPitchNamesMap [kD_SesquiSharp_QTP] = "disih";
  gDeutschPitchNamesMap [kD_DoubleSharp_QTP] = "disis";

  gDeutschPitchNamesMap [kE_DoubleFlat_QTP]  = "eses";
  gDeutschPitchNamesMap [kE_SesquiFlat_QTP]  = "esseh";
  gDeutschPitchNamesMap [kE_Flat_QTP]        = "es";
  gDeutschPitchNamesMap [kE_SemiFlat_QTP]    = "eeh";
  gDeutschPitchNamesMap [kE_Natural_QTP]     = "e";
  gDeutschPitchNamesMap [kE_SemiSharp_QTP]   = "eih";
  gDeutschPitchNamesMap [kE_Sharp_QTP]       = "eis";
  gDeutschPitchNamesMap [kE_SesquiSharp_QTP] = "eisih";
  gDeutschPitchNamesMap [kE_DoubleSharp_QTP] = "eisis";
    
  gDeutschPitchNamesMap [kF_DoubleFlat_QTP]  = "feses";
  gDeutschPitchNamesMap [kF_SesquiFlat_QTP]  = "feseh";
  gDeutschPitchNamesMap [kF_Flat_QTP]        = "fes";
  gDeutschPitchNamesMap [kF_SemiFlat_QTP]    = "feh";
  gDeutschPitchNamesMap [kF_Natural_QTP]     = "f";
  gDeutschPitchNamesMap [kF_SemiSharp_QTP]   = "fih";
  gDeutschPitchNamesMap [kF_Sharp_QTP]       = "fis";
  gDeutschPitchNamesMap [kF_SesquiSharp_QTP] = "fisih";
  gDeutschPitchNamesMap [kF_DoubleSharp_QTP] = "fisis";
    
  gDeutschPitchNamesMap [kG_DoubleFlat_QTP]  = "geses";
  gDeutschPitchNamesMap [kG_SesquiFlat_QTP]  = "geseh";
  gDeutschPitchNamesMap [kG_Flat_QTP]        = "ges";
  gDeutschPitchNamesMap [kG_SemiFlat_QTP]    = "geh";
  gDeutschPitchNamesMap [kG_Natural_QTP]     = "g";
  gDeutschPitchNamesMap [kG_SemiSharp_QTP]   = "gih";
  gDeutschPitchNamesMap [kG_Sharp_QTP]       = "gis";
  gDeutschPitchNamesMap [kG_SesquiSharp_QTP] = "gisih";
  gDeutschPitchNamesMap [kG_DoubleSharp_QTP] = "gisis";

  // english
  gEnglishPitchNamesMap [k_NoQuarterTonesPitch_QTP]  = "";
  gEnglishPitchNamesMap [k_Rest_QTP]                 = "r";
  
  gEnglishPitchNamesMap [kA_DoubleFlat_QTP]  = "aff";
  gEnglishPitchNamesMap [kA_SesquiFlat_QTP]  = "atqf";
  gEnglishPitchNamesMap [kA_Flat_QTP]        = "af";
  gEnglishPitchNamesMap [kA_SemiFlat_QTP]    = "aqf";
  gEnglishPitchNamesMap [kA_Natural_QTP]     = "a";
  gEnglishPitchNamesMap [kA_SemiSharp_QTP]   = "aqs";
  gEnglishPitchNamesMap [kA_Sharp_QTP]       = "as";
  gEnglishPitchNamesMap [kA_SesquiSharp_QTP] = "atqs";
  gEnglishPitchNamesMap [kA_DoubleSharp_QTP] = "a";
    
  gEnglishPitchNamesMap [kB_DoubleFlat_QTP]  = "bfqf";
  gEnglishPitchNamesMap [kB_SesquiFlat_QTP]  = "btqf";
  gEnglishPitchNamesMap [kB_Flat_QTP]        = "bf";
  gEnglishPitchNamesMap [kB_SemiFlat_QTP]    = "bqf";
  gEnglishPitchNamesMap [kB_Natural_QTP]     = "b";
  gEnglishPitchNamesMap [kB_SemiSharp_QTP]   = "bqs";
  gEnglishPitchNamesMap [kB_Sharp_QTP]       = "bs";
  gEnglishPitchNamesMap [kB_SesquiSharp_QTP] = "btqs";
  gEnglishPitchNamesMap [kB_DoubleSharp_QTP] = "bx";
    
  gEnglishPitchNamesMap [kC_DoubleFlat_QTP]  = "cff";
  gEnglishPitchNamesMap [kC_SesquiFlat_QTP]  = "ctqf";
  gEnglishPitchNamesMap [kC_Flat_QTP]        = "cf";
  gEnglishPitchNamesMap [kC_SemiFlat_QTP]    = "cqf";
  gEnglishPitchNamesMap [kC_Natural_QTP]     = "c";
  gEnglishPitchNamesMap [kC_SemiSharp_QTP]   = "cqs";
  gEnglishPitchNamesMap [kC_Sharp_QTP]       = "cs";
  gEnglishPitchNamesMap [kC_SesquiSharp_QTP] = "ctqs";
  gEnglishPitchNamesMap [kC_DoubleSharp_QTP] = "cx";
    
  gEnglishPitchNamesMap [kD_DoubleFlat_QTP]  = "dff";
  gEnglishPitchNamesMap [kD_SesquiFlat_QTP]  = "dtqf";
  gEnglishPitchNamesMap [kD_Flat_QTP]        = "df";
  gEnglishPitchNamesMap [kD_SemiFlat_QTP]    = "dqf";
  gEnglishPitchNamesMap [kD_Natural_QTP]     = "d";
  gEnglishPitchNamesMap [kD_SemiSharp_QTP]   = "dqs";
  gEnglishPitchNamesMap [kD_Sharp_QTP]       = "ds";
  gEnglishPitchNamesMap [kD_SesquiSharp_QTP] = "dtqs";
  gEnglishPitchNamesMap [kD_DoubleSharp_QTP] = "dx";

  gEnglishPitchNamesMap [kE_DoubleFlat_QTP]  = "eff";
  gEnglishPitchNamesMap [kE_SesquiFlat_QTP]  = "etqf";
  gEnglishPitchNamesMap [kE_Flat_QTP]        = "ef";
  gEnglishPitchNamesMap [kE_SemiFlat_QTP]    = "eqf";
  gEnglishPitchNamesMap [kE_Natural_QTP]     = "e";
  gEnglishPitchNamesMap [kE_SemiSharp_QTP]   = "eqs";
  gEnglishPitchNamesMap [kE_Sharp_QTP]       = "es";
  gEnglishPitchNamesMap [kE_SesquiSharp_QTP] = "etqs";
  gEnglishPitchNamesMap [kE_DoubleSharp_QTP] = "ex";
    
  gEnglishPitchNamesMap [kF_DoubleFlat_QTP]  = "fff";
  gEnglishPitchNamesMap [kF_SesquiFlat_QTP]  = "ftqf";
  gEnglishPitchNamesMap [kF_Flat_QTP]        = "ff";
  gEnglishPitchNamesMap [kF_SemiFlat_QTP]    = "fqf";
  gEnglishPitchNamesMap [kF_Natural_QTP]     = "f";
  gEnglishPitchNamesMap [kF_SemiSharp_QTP]   = "fqs";
  gEnglishPitchNamesMap [kF_Sharp_QTP]       = "fs";
  gEnglishPitchNamesMap [kF_SesquiSharp_QTP] = "ftqs";
  gEnglishPitchNamesMap [kF_DoubleSharp_QTP] = "fx";
    
  gEnglishPitchNamesMap [kG_DoubleFlat_QTP]  = "gff";
  gEnglishPitchNamesMap [kG_SesquiFlat_QTP]  = "gtqf";
  gEnglishPitchNamesMap [kG_Flat_QTP]        = "gf";
  gEnglishPitchNamesMap [kG_SemiFlat_QTP]    = "gqf";
  gEnglishPitchNamesMap [kG_Natural_QTP]     = "g";
  gEnglishPitchNamesMap [kG_SemiSharp_QTP]   = "gqs";
  gEnglishPitchNamesMap [kG_Sharp_QTP]       = "gs";
  gEnglishPitchNamesMap [kG_SesquiSharp_QTP] = "gtqs";
  gEnglishPitchNamesMap [kG_DoubleSharp_QTP] = "gx";

  // espanol
  gEspanolPitchNamesMap [k_NoQuarterTonesPitch_QTP]  = "";
  gEspanolPitchNamesMap [k_Rest_QTP]                 = "r";
  
  gEspanolPitchNamesMap [kA_DoubleFlat_QTP]  = "labb";
  gEspanolPitchNamesMap [kA_SesquiFlat_QTP]  = "latcb";
  gEspanolPitchNamesMap [kA_Flat_QTP]        = "lab";
  gEspanolPitchNamesMap [kA_SemiFlat_QTP]    = "lacb";
  gEspanolPitchNamesMap [kA_Natural_QTP]     = "la";
  gEspanolPitchNamesMap [kA_SemiSharp_QTP]   = "lacs";
  gEspanolPitchNamesMap [kA_Sharp_QTP]       = "las";
  gEspanolPitchNamesMap [kA_SesquiSharp_QTP] = "latcs";
  gEspanolPitchNamesMap [kA_DoubleSharp_QTP] = "lax";
    
  gEspanolPitchNamesMap [kB_DoubleFlat_QTP]  = "sibb";
  gEspanolPitchNamesMap [kB_SesquiFlat_QTP]  = "sitcb";
  gEspanolPitchNamesMap [kB_Flat_QTP]        = "sib";
  gEspanolPitchNamesMap [kB_SemiFlat_QTP]    = "sicb";
  gEspanolPitchNamesMap [kB_Natural_QTP]     = "si";
  gEspanolPitchNamesMap [kB_SemiSharp_QTP]   = "sics";
  gEspanolPitchNamesMap [kB_Sharp_QTP]       = "sis";
  gEspanolPitchNamesMap [kB_SesquiSharp_QTP] = "sitcs";
  gEspanolPitchNamesMap [kB_DoubleSharp_QTP] = "six";
    
  gEspanolPitchNamesMap [kC_DoubleFlat_QTP]  = "dobb";
  gEspanolPitchNamesMap [kC_SesquiFlat_QTP]  = "dotcb";
  gEspanolPitchNamesMap [kC_Flat_QTP]        = "dob";
  gEspanolPitchNamesMap [kC_SemiFlat_QTP]    = "docb";
  gEspanolPitchNamesMap [kC_Natural_QTP]     = "do";
  gEspanolPitchNamesMap [kC_SemiSharp_QTP]   = "docs";
  gEspanolPitchNamesMap [kC_Sharp_QTP]       = "dos";
  gEspanolPitchNamesMap [kC_SesquiSharp_QTP] = "dotcs";
  gEspanolPitchNamesMap [kC_DoubleSharp_QTP] = "dox";
    
  gEspanolPitchNamesMap [kD_DoubleFlat_QTP]  = "rebb";
  gEspanolPitchNamesMap [kD_SesquiFlat_QTP]  = "retcb";
  gEspanolPitchNamesMap [kD_Flat_QTP]        = "reb";
  gEspanolPitchNamesMap [kD_SemiFlat_QTP]    = "recb";
  gEspanolPitchNamesMap [kD_Natural_QTP]     = "re";
  gEspanolPitchNamesMap [kD_SemiSharp_QTP]   = "recs";
  gEspanolPitchNamesMap [kD_Sharp_QTP]       = "res";
  gEspanolPitchNamesMap [kD_SesquiSharp_QTP] = "retcs";
  gEspanolPitchNamesMap [kD_DoubleSharp_QTP] = "rex";

  gEspanolPitchNamesMap [kE_DoubleFlat_QTP]  = "mibb";
  gEspanolPitchNamesMap [kE_SesquiFlat_QTP]  = "mitcb";
  gEspanolPitchNamesMap [kE_Flat_QTP]        = "mib";
  gEspanolPitchNamesMap [kE_SemiFlat_QTP]    = "micb";
  gEspanolPitchNamesMap [kE_Natural_QTP]     = "mi";
  gEspanolPitchNamesMap [kE_SemiSharp_QTP]   = "mics";
  gEspanolPitchNamesMap [kE_Sharp_QTP]       = "mis";
  gEspanolPitchNamesMap [kE_SesquiSharp_QTP] = "mitcs";
  gEspanolPitchNamesMap [kE_DoubleSharp_QTP] = "mix";
    
  gEspanolPitchNamesMap [kF_DoubleFlat_QTP]  = "fabb";
  gEspanolPitchNamesMap [kF_SesquiFlat_QTP]  = "fatcb";
  gEspanolPitchNamesMap [kF_Flat_QTP]        = "fab";
  gEspanolPitchNamesMap [kF_SemiFlat_QTP]    = "facb";
  gEspanolPitchNamesMap [kF_Natural_QTP]     = "fa";
  gEspanolPitchNamesMap [kF_SemiSharp_QTP]   = "facs";
  gEspanolPitchNamesMap [kF_Sharp_QTP]       = "fas";
  gEspanolPitchNamesMap [kF_SesquiSharp_QTP] = "fatcs";
  gEspanolPitchNamesMap [kF_DoubleSharp_QTP] = "fax";
    
  gEspanolPitchNamesMap [kG_DoubleFlat_QTP]  = "solbb";
  gEspanolPitchNamesMap [kG_SesquiFlat_QTP]  = "soltcb";
  gEspanolPitchNamesMap [kG_Flat_QTP]        = "solb";
  gEspanolPitchNamesMap [kG_SemiFlat_QTP]    = "solcb";
  gEspanolPitchNamesMap [kG_Natural_QTP]     = "sol";
  gEspanolPitchNamesMap [kG_SemiSharp_QTP]   = "solcs";
  gEspanolPitchNamesMap [kG_Sharp_QTP]       = "sols";
  gEspanolPitchNamesMap [kG_SesquiSharp_QTP] = "soltcs";
  gEspanolPitchNamesMap [kG_DoubleSharp_QTP] = "solx";

  // francais
  gFrancaisPitchNamesMap [k_NoQuarterTonesPitch_QTP]  = "";
  gFrancaisPitchNamesMap [k_Rest_QTP]                 = "r";
  
  gFrancaisPitchNamesMap [kA_DoubleFlat_QTP]  = "labb";
  gFrancaisPitchNamesMap [kA_SesquiFlat_QTP]  = "labtqt";
  gFrancaisPitchNamesMap [kA_Flat_QTP]        = "lab";
  gFrancaisPitchNamesMap [kA_SemiFlat_QTP]    = "labqt";
  gFrancaisPitchNamesMap [kA_Natural_QTP]     = "la";
  gFrancaisPitchNamesMap [kA_SemiSharp_QTP]   = "lasqt";
  gFrancaisPitchNamesMap [kA_Sharp_QTP]       = "lad";
  gFrancaisPitchNamesMap [kA_SesquiSharp_QTP] = "lastqt";
  gFrancaisPitchNamesMap [kA_DoubleSharp_QTP] = "lass";
    
  gFrancaisPitchNamesMap [kB_DoubleFlat_QTP]  = "sibb";
  gFrancaisPitchNamesMap [kB_SesquiFlat_QTP]  = "sibtqt";
  gFrancaisPitchNamesMap [kB_Flat_QTP]        = "sib";
  gFrancaisPitchNamesMap [kB_SemiFlat_QTP]    = "sibqt";
  gFrancaisPitchNamesMap [kB_Natural_QTP]     = "si";
  gFrancaisPitchNamesMap [kB_SemiSharp_QTP]   = "sisqt";
  gFrancaisPitchNamesMap [kB_Sharp_QTP]       = "sid";
  gFrancaisPitchNamesMap [kB_SesquiSharp_QTP] = "sistqt";
  gFrancaisPitchNamesMap [kB_DoubleSharp_QTP] = "siss";
    
  gFrancaisPitchNamesMap [kC_DoubleFlat_QTP]  = "dobb";
  gFrancaisPitchNamesMap [kC_SesquiFlat_QTP]  = "dobtqt";
  gFrancaisPitchNamesMap [kC_Flat_QTP]        = "dob";
  gFrancaisPitchNamesMap [kC_SemiFlat_QTP]    = "dobqt";
  gFrancaisPitchNamesMap [kC_Natural_QTP]     = "do";
  gFrancaisPitchNamesMap [kC_SemiSharp_QTP]   = "dosqt";
  gFrancaisPitchNamesMap [kC_Sharp_QTP]       = "dod";
  gFrancaisPitchNamesMap [kC_SesquiSharp_QTP] = "dostqt";
  gFrancaisPitchNamesMap [kC_DoubleSharp_QTP] = "doss";
    
  gFrancaisPitchNamesMap [kD_DoubleFlat_QTP]  = "rebb";
  gFrancaisPitchNamesMap [kD_SesquiFlat_QTP]  = "rebtqt";
  gFrancaisPitchNamesMap [kD_Flat_QTP]        = "reb";
  gFrancaisPitchNamesMap [kD_SemiFlat_QTP]    = "rebqt";
  gFrancaisPitchNamesMap [kD_Natural_QTP]     = "re";
  gFrancaisPitchNamesMap [kD_SemiSharp_QTP]   = "resqt";
  gFrancaisPitchNamesMap [kD_Sharp_QTP]       = "red";
  gFrancaisPitchNamesMap [kD_SesquiSharp_QTP] = "restqt";
  gFrancaisPitchNamesMap [kD_DoubleSharp_QTP] = "ress";

  gFrancaisPitchNamesMap [kE_DoubleFlat_QTP]  = "mibb";
  gFrancaisPitchNamesMap [kE_SesquiFlat_QTP]  = "mibtqt";
  gFrancaisPitchNamesMap [kE_Flat_QTP]        = "mib";
  gFrancaisPitchNamesMap [kE_SemiFlat_QTP]    = "mibqt";
  gFrancaisPitchNamesMap [kE_Natural_QTP]     = "mi";
  gFrancaisPitchNamesMap [kE_SemiSharp_QTP]   = "misqt";
  gFrancaisPitchNamesMap [kE_Sharp_QTP]       = "mid";
  gFrancaisPitchNamesMap [kE_SesquiSharp_QTP] = "mistqt";
  gFrancaisPitchNamesMap [kE_DoubleSharp_QTP] = "miss";
    
  gFrancaisPitchNamesMap [kF_DoubleFlat_QTP]  = "fabb";
  gFrancaisPitchNamesMap [kF_SesquiFlat_QTP]  = "fabtqt";
  gFrancaisPitchNamesMap [kF_Flat_QTP]        = "fab";
  gFrancaisPitchNamesMap [kF_SemiFlat_QTP]    = "fabqt";
  gFrancaisPitchNamesMap [kF_Natural_QTP]     = "fa";
  gFrancaisPitchNamesMap [kF_SemiSharp_QTP]   = "fasqt";
  gFrancaisPitchNamesMap [kF_Sharp_QTP]       = "fad";
  gFrancaisPitchNamesMap [kF_SesquiSharp_QTP] = "fastqt";
  gFrancaisPitchNamesMap [kF_DoubleSharp_QTP] = "fass";
    
  gFrancaisPitchNamesMap [kG_DoubleFlat_QTP]  = "solbb";
  gFrancaisPitchNamesMap [kG_SesquiFlat_QTP]  = "solbtqt";
  gFrancaisPitchNamesMap [kG_Flat_QTP]        = "solb";
  gFrancaisPitchNamesMap [kG_SemiFlat_QTP]    = "solbqt";
  gFrancaisPitchNamesMap [kG_Natural_QTP]     = "sol";
  gFrancaisPitchNamesMap [kG_SemiSharp_QTP]   = "solsqt";
  gFrancaisPitchNamesMap [kG_Sharp_QTP]       = "sold";
  gFrancaisPitchNamesMap [kG_SesquiSharp_QTP] = "solstqt";
  gFrancaisPitchNamesMap [kG_DoubleSharp_QTP] = "solss";

  // italiano
  gItalianoPitchNamesMap [k_NoQuarterTonesPitch_QTP]  = "";
  gItalianoPitchNamesMap [k_Rest_QTP]                 = "r";
  
  gItalianoPitchNamesMap [kA_DoubleFlat_QTP]  = "labb";
  gItalianoPitchNamesMap [kA_SesquiFlat_QTP]  = "labsb";
  gItalianoPitchNamesMap [kA_Flat_QTP]        = "lab";
  gItalianoPitchNamesMap [kA_SemiFlat_QTP]    = "lasb";
  gItalianoPitchNamesMap [kA_Natural_QTP]     = "la";
  gItalianoPitchNamesMap [kA_SemiSharp_QTP]   = "lasd";
  gItalianoPitchNamesMap [kA_Sharp_QTP]       = "lad";
  gItalianoPitchNamesMap [kA_SesquiSharp_QTP] = "ladsd";
  gItalianoPitchNamesMap [kA_DoubleSharp_QTP] = "ladd";
    
  gItalianoPitchNamesMap [kB_DoubleFlat_QTP]  = "sibb";
  gItalianoPitchNamesMap [kB_SesquiFlat_QTP]  = "sibsb";
  gItalianoPitchNamesMap [kB_Flat_QTP]        = "sib";
  gItalianoPitchNamesMap [kB_SemiFlat_QTP]    = "sisb";
  gItalianoPitchNamesMap [kB_Natural_QTP]     = "si";
  gItalianoPitchNamesMap [kB_SemiSharp_QTP]   = "sisd";
  gItalianoPitchNamesMap [kB_Sharp_QTP]       = "sid";
  gItalianoPitchNamesMap [kB_SesquiSharp_QTP] = "sidsd";
  gItalianoPitchNamesMap [kB_DoubleSharp_QTP] = "sidd";
    
  gItalianoPitchNamesMap [kC_DoubleFlat_QTP]  = "dobb";
  gItalianoPitchNamesMap [kC_SesquiFlat_QTP]  = "dobsb";
  gItalianoPitchNamesMap [kC_Flat_QTP]        = "dob";
  gItalianoPitchNamesMap [kC_SemiFlat_QTP]    = "dosb";
  gItalianoPitchNamesMap [kC_Natural_QTP]     = "do";
  gItalianoPitchNamesMap [kC_SemiSharp_QTP]   = "dosd";
  gItalianoPitchNamesMap [kC_Sharp_QTP]       = "dod";
  gItalianoPitchNamesMap [kC_SesquiSharp_QTP] = "dodsd";
  gItalianoPitchNamesMap [kC_DoubleSharp_QTP] = "dodd";
    
  gItalianoPitchNamesMap [kD_DoubleFlat_QTP]  = "rebb";
  gItalianoPitchNamesMap [kD_SesquiFlat_QTP]  = "rebsb";
  gItalianoPitchNamesMap [kD_Flat_QTP]        = "reb";
  gItalianoPitchNamesMap [kD_SemiFlat_QTP]    = "resb";
  gItalianoPitchNamesMap [kD_Natural_QTP]     = "re";
  gItalianoPitchNamesMap [kD_SemiSharp_QTP]   = "resd";
  gItalianoPitchNamesMap [kD_Sharp_QTP]       = "red";
  gItalianoPitchNamesMap [kD_SesquiSharp_QTP] = "redsd";
  gItalianoPitchNamesMap [kD_DoubleSharp_QTP] = "redd";

  gItalianoPitchNamesMap [kE_DoubleFlat_QTP]  = "mibb";
  gItalianoPitchNamesMap [kE_SesquiFlat_QTP]  = "mibsb";
  gItalianoPitchNamesMap [kE_Flat_QTP]        = "mib";
  gItalianoPitchNamesMap [kE_SemiFlat_QTP]    = "misb";
  gItalianoPitchNamesMap [kE_Natural_QTP]     = "mi";
  gItalianoPitchNamesMap [kE_SemiSharp_QTP]   = "misd";
  gItalianoPitchNamesMap [kE_Sharp_QTP]       = "mid";
  gItalianoPitchNamesMap [kE_SesquiSharp_QTP] = "midsd";
  gItalianoPitchNamesMap [kE_DoubleSharp_QTP] = "midd";
    
  gItalianoPitchNamesMap [kF_DoubleFlat_QTP]  = "fabb";
  gItalianoPitchNamesMap [kF_SesquiFlat_QTP]  = "fabsb";
  gItalianoPitchNamesMap [kF_Flat_QTP]        = "fab";
  gItalianoPitchNamesMap [kF_SemiFlat_QTP]    = "fasb";
  gItalianoPitchNamesMap [kF_Natural_QTP]     = "fa";
  gItalianoPitchNamesMap [kF_SemiSharp_QTP]   = "fasd";
  gItalianoPitchNamesMap [kF_Sharp_QTP]       = "fad";
  gItalianoPitchNamesMap [kF_SesquiSharp_QTP] = "fadsd";
  gItalianoPitchNamesMap [kF_DoubleSharp_QTP] = "fadd";
    
  gItalianoPitchNamesMap [kG_DoubleFlat_QTP]  = "solbb";
  gItalianoPitchNamesMap [kG_SesquiFlat_QTP]  = "solbsb";
  gItalianoPitchNamesMap [kG_Flat_QTP]        = "solb";
  gItalianoPitchNamesMap [kG_SemiFlat_QTP]    = "solsb";
  gItalianoPitchNamesMap [kG_Natural_QTP]     = "sol";
  gItalianoPitchNamesMap [kG_SemiSharp_QTP]   = "solsd";
  gItalianoPitchNamesMap [kG_Sharp_QTP]       = "sold";
  gItalianoPitchNamesMap [kG_SesquiSharp_QTP] = "soldsd";
  gItalianoPitchNamesMap [kG_DoubleSharp_QTP] = "soldd";

  // norsk
  gNorskPitchNamesMap [k_NoQuarterTonesPitch_QTP]  = "";
  gNorskPitchNamesMap [k_Rest_QTP]                 = "r";
  
  gNorskPitchNamesMap [kA_DoubleFlat_QTP]  = "aeses";
  gNorskPitchNamesMap [kA_SesquiFlat_QTP]  = "aSesquiFlat???";
  gNorskPitchNamesMap [kA_Flat_QTP]        = "aes";
  gNorskPitchNamesMap [kA_SemiFlat_QTP]    = "aSemiFlat???";
  gNorskPitchNamesMap [kA_Natural_QTP]     = "a";
  gNorskPitchNamesMap [kA_SemiSharp_QTP]   = "aSemiSharp???";
  gNorskPitchNamesMap [kA_Sharp_QTP]       = "ais";
  gNorskPitchNamesMap [kA_SesquiSharp_QTP] = "aSesquiSharp???";
  gNorskPitchNamesMap [kA_DoubleSharp_QTP] = "aisis";
    
  gNorskPitchNamesMap [kB_DoubleFlat_QTP]  = "beses";
  gNorskPitchNamesMap [kB_SesquiFlat_QTP]  = "bSesquiFlat???";
  gNorskPitchNamesMap [kB_Flat_QTP]        = "bes";
  gNorskPitchNamesMap [kB_SemiFlat_QTP]    = "bSemiFlat???";
  gNorskPitchNamesMap [kB_Natural_QTP]     = "b";
  gNorskPitchNamesMap [kB_SemiSharp_QTP]   = "bSemiSharp???";
  gNorskPitchNamesMap [kB_Sharp_QTP]       = "bis";
  gNorskPitchNamesMap [kB_SesquiSharp_QTP] = "bSesquiSharp???";
  gNorskPitchNamesMap [kB_DoubleSharp_QTP] = "bisis";
    
  gNorskPitchNamesMap [kC_DoubleFlat_QTP]  = "ceses";
  gNorskPitchNamesMap [kC_SesquiFlat_QTP]  = "cSesquiFlat???";
  gNorskPitchNamesMap [kC_Flat_QTP]        = "ces";
  gNorskPitchNamesMap [kC_SemiFlat_QTP]    = "cSemiFlat???";
  gNorskPitchNamesMap [kC_Natural_QTP]     = "c";
  gNorskPitchNamesMap [kC_SemiSharp_QTP]   = "cSemiSharp???";
  gNorskPitchNamesMap [kC_Sharp_QTP]       = "cis";
  gNorskPitchNamesMap [kC_SesquiSharp_QTP] = "cSesquiSharp???";
  gNorskPitchNamesMap [kC_DoubleSharp_QTP] = "cisis";
    
  gNorskPitchNamesMap [kD_DoubleFlat_QTP]  = "deses";
  gNorskPitchNamesMap [kD_SesquiFlat_QTP]  = "dSesquiFlat???";
  gNorskPitchNamesMap [kD_Flat_QTP]        = "des";
  gNorskPitchNamesMap [kD_SemiFlat_QTP]    = "dSemiFlat???";
  gNorskPitchNamesMap [kD_Natural_QTP]     = "d";
  gNorskPitchNamesMap [kD_SemiSharp_QTP]   = "dSemiSharp???";
  gNorskPitchNamesMap [kD_Sharp_QTP]       = "dis";
  gNorskPitchNamesMap [kD_SesquiSharp_QTP] = "dSesquiSharp???";
  gNorskPitchNamesMap [kD_DoubleSharp_QTP] = "disis";

  gNorskPitchNamesMap [kE_DoubleFlat_QTP]  = "eeses";
  gNorskPitchNamesMap [kE_SesquiFlat_QTP]  = "eSesquiFlat???";
  gNorskPitchNamesMap [kE_Flat_QTP]        = "ees";
  gNorskPitchNamesMap [kE_SemiFlat_QTP]    = "eSemiFlat???";
  gNorskPitchNamesMap [kE_Natural_QTP]     = "e";
  gNorskPitchNamesMap [kE_SemiSharp_QTP]   = "eSemiSharp???";
  gNorskPitchNamesMap [kE_Sharp_QTP]       = "eis";
  gNorskPitchNamesMap [kE_SesquiSharp_QTP] = "eSesquiSharp???";
  gNorskPitchNamesMap [kE_DoubleSharp_QTP] = "eisis";
    
  gNorskPitchNamesMap [kF_DoubleFlat_QTP]  = "feses";
  gNorskPitchNamesMap [kF_SesquiFlat_QTP]  = "fSesquiFlat???";
  gNorskPitchNamesMap [kF_Flat_QTP]        = "fes";
  gNorskPitchNamesMap [kF_SemiFlat_QTP]    = "fSemiFlat???";
  gNorskPitchNamesMap [kF_Natural_QTP]     = "f";
  gNorskPitchNamesMap [kF_SemiSharp_QTP]   = "fSemiSharp???";
  gNorskPitchNamesMap [kF_Sharp_QTP]       = "fis";
  gNorskPitchNamesMap [kF_SesquiSharp_QTP] = "fSesquiSharp???";
  gNorskPitchNamesMap [kF_DoubleSharp_QTP] = "fisis";
    
  gNorskPitchNamesMap [kG_DoubleFlat_QTP]  = "geses";
  gNorskPitchNamesMap [kG_SesquiFlat_QTP]  = "gSesquiFlat???";
  gNorskPitchNamesMap [kG_Flat_QTP]        = "ges";
  gNorskPitchNamesMap [kG_SemiFlat_QTP]    = "gSemiFlat???";
  gNorskPitchNamesMap [kG_Natural_QTP]     = "g";
  gNorskPitchNamesMap [kG_SemiSharp_QTP]   = "gSemiSharp???";
  gNorskPitchNamesMap [kG_Sharp_QTP]       = "gis";
  gNorskPitchNamesMap [kG_SesquiSharp_QTP] = "gSesquiSharp???";
  gNorskPitchNamesMap [kG_DoubleSharp_QTP] = "gisis";

  // portugues
  gPortuguesPitchNamesMap [k_NoQuarterTonesPitch_QTP]  = "";
  gPortuguesPitchNamesMap [k_Rest_QTP]                 = "r";
  
  gPortuguesPitchNamesMap [kA_DoubleFlat_QTP]  = "labb";
  gPortuguesPitchNamesMap [kA_SesquiFlat_QTP]  = "labtqt";
  gPortuguesPitchNamesMap [kA_Flat_QTP]        = "lab";
  gPortuguesPitchNamesMap [kA_SemiFlat_QTP]    = "lasb";
  gPortuguesPitchNamesMap [kA_Natural_QTP]     = "la";
  gPortuguesPitchNamesMap [kA_SemiSharp_QTP]   = "lasd";
  gPortuguesPitchNamesMap [kA_Sharp_QTP]       = "lad";
  gPortuguesPitchNamesMap [kA_SesquiSharp_QTP] = "ladsd";
  gPortuguesPitchNamesMap [kA_DoubleSharp_QTP] = "ladd";
    
  gPortuguesPitchNamesMap [kB_DoubleFlat_QTP]  = "sibb";
  gPortuguesPitchNamesMap [kB_SesquiFlat_QTP]  = "sibtqt";
  gPortuguesPitchNamesMap [kB_Flat_QTP]        = "sib";
  gPortuguesPitchNamesMap [kB_SemiFlat_QTP]    = "sisb";
  gPortuguesPitchNamesMap [kB_Natural_QTP]     = "si";
  gPortuguesPitchNamesMap [kB_SemiSharp_QTP]   = "sisd";
  gPortuguesPitchNamesMap [kB_Sharp_QTP]       = "sid";
  gPortuguesPitchNamesMap [kB_SesquiSharp_QTP] = "sidsd";
  gPortuguesPitchNamesMap [kB_DoubleSharp_QTP] = "sidd";
    
  gPortuguesPitchNamesMap [kC_DoubleFlat_QTP]  = "dobb";
  gPortuguesPitchNamesMap [kC_SesquiFlat_QTP]  = "dobtqt";
  gPortuguesPitchNamesMap [kC_Flat_QTP]        = "dob";
  gPortuguesPitchNamesMap [kC_SemiFlat_QTP]    = "dosb";
  gPortuguesPitchNamesMap [kC_Natural_QTP]     = "do";
  gPortuguesPitchNamesMap [kC_SemiSharp_QTP]   = "dosd";
  gPortuguesPitchNamesMap [kC_Sharp_QTP]       = "dod";
  gPortuguesPitchNamesMap [kC_SesquiSharp_QTP] = "dodsd";
  gPortuguesPitchNamesMap [kC_DoubleSharp_QTP] = "dodd";
    
  gPortuguesPitchNamesMap [kD_DoubleFlat_QTP]  = "rebb";
  gPortuguesPitchNamesMap [kD_SesquiFlat_QTP]  = "rebtqt";
  gPortuguesPitchNamesMap [kD_Flat_QTP]        = "reb";
  gPortuguesPitchNamesMap [kD_SemiFlat_QTP]    = "resb";
  gPortuguesPitchNamesMap [kD_Natural_QTP]     = "re";
  gPortuguesPitchNamesMap [kD_SemiSharp_QTP]   = "resd";
  gPortuguesPitchNamesMap [kD_Sharp_QTP]       = "red";
  gPortuguesPitchNamesMap [kD_SesquiSharp_QTP] = "redsd";
  gPortuguesPitchNamesMap [kD_DoubleSharp_QTP] = "redd";

  gPortuguesPitchNamesMap [kE_DoubleFlat_QTP]  = "mibb";
  gPortuguesPitchNamesMap [kE_SesquiFlat_QTP]  = "mibtqt";
  gPortuguesPitchNamesMap [kE_Flat_QTP]        = "mib";
  gPortuguesPitchNamesMap [kE_SemiFlat_QTP]    = "misb";
  gPortuguesPitchNamesMap [kE_Natural_QTP]     = "mi";
  gPortuguesPitchNamesMap [kE_SemiSharp_QTP]   = "misd";
  gPortuguesPitchNamesMap [kE_Sharp_QTP]       = "mid";
  gPortuguesPitchNamesMap [kE_SesquiSharp_QTP] = "midsd";
  gPortuguesPitchNamesMap [kE_DoubleSharp_QTP] = "midd";
    
  gPortuguesPitchNamesMap [kF_DoubleFlat_QTP]  = "fabb";
  gPortuguesPitchNamesMap [kF_SesquiFlat_QTP]  = "fabtqt";
  gPortuguesPitchNamesMap [kF_Flat_QTP]        = "fab";
  gPortuguesPitchNamesMap [kF_SemiFlat_QTP]    = "fasb";
  gPortuguesPitchNamesMap [kF_Natural_QTP]     = "fa";
  gPortuguesPitchNamesMap [kF_SemiSharp_QTP]   = "fasd";
  gPortuguesPitchNamesMap [kF_Sharp_QTP]       = "fad";
  gPortuguesPitchNamesMap [kF_SesquiSharp_QTP] = "fadsd";
  gPortuguesPitchNamesMap [kF_DoubleSharp_QTP] = "fadd";
    
  gPortuguesPitchNamesMap [kG_DoubleFlat_QTP]  = "solbb";
  gPortuguesPitchNamesMap [kG_SesquiFlat_QTP]  = "solbtqt";
  gPortuguesPitchNamesMap [kG_Flat_QTP]        = "solb";
  gPortuguesPitchNamesMap [kG_SemiFlat_QTP]    = "solsb";
  gPortuguesPitchNamesMap [kG_Natural_QTP]     = "sol";
  gPortuguesPitchNamesMap [kG_SemiSharp_QTP]   = "solsd";
  gPortuguesPitchNamesMap [kG_Sharp_QTP]       = "sold";
  gPortuguesPitchNamesMap [kG_SesquiSharp_QTP] = "soldsd";
  gPortuguesPitchNamesMap [kG_DoubleSharp_QTP] = "soldd";

  // suomi
  gSuomiPitchNamesMap [k_NoQuarterTonesPitch_QTP]  = "";
  gSuomiPitchNamesMap [k_Rest_QTP]                 = "r";
  
  gSuomiPitchNamesMap [kA_DoubleFlat_QTP]  = "asas";
  gSuomiPitchNamesMap [kA_SesquiFlat_QTP]  = "aSesquiFlat???";
  gSuomiPitchNamesMap [kA_Flat_QTP]        = "as";
  gSuomiPitchNamesMap [kA_SemiFlat_QTP]    = "aSemiFlat???";
  gSuomiPitchNamesMap [kA_Natural_QTP]     = "a";
  gSuomiPitchNamesMap [kA_SemiSharp_QTP]   = "aSemiSharp???";
  gSuomiPitchNamesMap [kA_Sharp_QTP]       = "ais";
  gSuomiPitchNamesMap [kA_SesquiSharp_QTP] = "aSesquiSharp???";
  gSuomiPitchNamesMap [kA_DoubleSharp_QTP] = "aisis";
    
  gSuomiPitchNamesMap [kB_DoubleFlat_QTP]  = "bes";
  gSuomiPitchNamesMap [kB_SesquiFlat_QTP]  = "bSesquiFlat???";
  gSuomiPitchNamesMap [kB_Flat_QTP]        = "b";
  gSuomiPitchNamesMap [kB_SemiFlat_QTP]    = "bSemiFlat???";
  gSuomiPitchNamesMap [kB_Natural_QTP]     = "h";
  gSuomiPitchNamesMap [kB_SemiSharp_QTP]   = "bSemiSharp???";
  gSuomiPitchNamesMap [kB_Sharp_QTP]       = "his";
  gSuomiPitchNamesMap [kB_SesquiSharp_QTP] = "bSesquiSharp???";
  gSuomiPitchNamesMap [kB_DoubleSharp_QTP] = "hisis";
    
  gSuomiPitchNamesMap [kC_DoubleFlat_QTP]  = "ceses";
  gSuomiPitchNamesMap [kC_SesquiFlat_QTP]  = "cSesquiFlat???";
  gSuomiPitchNamesMap [kC_Flat_QTP]        = "ces";
  gSuomiPitchNamesMap [kC_SemiFlat_QTP]    = "cSemiFlat???";
  gSuomiPitchNamesMap [kC_Natural_QTP]     = "c";
  gSuomiPitchNamesMap [kC_SemiSharp_QTP]   = "cSemiSharp???";
  gSuomiPitchNamesMap [kC_Sharp_QTP]       = "cis";
  gSuomiPitchNamesMap [kC_SesquiSharp_QTP] = "cSesquiSharp???";
  gSuomiPitchNamesMap [kC_DoubleSharp_QTP] = "cisis";
    
  gSuomiPitchNamesMap [kD_DoubleFlat_QTP]  = "deses";
  gSuomiPitchNamesMap [kD_SesquiFlat_QTP]  = "dSesquiFlat???";
  gSuomiPitchNamesMap [kD_Flat_QTP]        = "des";
  gSuomiPitchNamesMap [kD_SemiFlat_QTP]    = "dSemiFlat???";
  gSuomiPitchNamesMap [kD_Natural_QTP]     = "d";
  gSuomiPitchNamesMap [kD_SemiSharp_QTP]   = "dSemiSharp???";
  gSuomiPitchNamesMap [kD_Sharp_QTP]       = "dis";
  gSuomiPitchNamesMap [kD_SesquiSharp_QTP] = "dSesquiSharp???";
  gSuomiPitchNamesMap [kD_DoubleSharp_QTP] = "disis";

  gSuomiPitchNamesMap [kE_DoubleFlat_QTP]  = "eses";
  gSuomiPitchNamesMap [kE_SesquiFlat_QTP]  = "eSesquiFlat???";
  gSuomiPitchNamesMap [kE_Flat_QTP]        = "es";
  gSuomiPitchNamesMap [kE_SemiFlat_QTP]    = "eSemiFlat???";
  gSuomiPitchNamesMap [kE_Natural_QTP]     = "e";
  gSuomiPitchNamesMap [kE_SemiSharp_QTP]   = "eSemiSharp???";
  gSuomiPitchNamesMap [kE_Sharp_QTP]       = "eis";
  gSuomiPitchNamesMap [kE_SesquiSharp_QTP] = "eSesquiSharp???";
  gSuomiPitchNamesMap [kE_DoubleSharp_QTP] = "eisis";
    
  gSuomiPitchNamesMap [kF_DoubleFlat_QTP]  = "feses";
  gSuomiPitchNamesMap [kF_SesquiFlat_QTP]  = "fSesquiFlat???";
  gSuomiPitchNamesMap [kF_Flat_QTP]        = "fes";
  gSuomiPitchNamesMap [kF_SemiFlat_QTP]    = "fSemiFlat???";
  gSuomiPitchNamesMap [kF_Natural_QTP]     = "f";
  gSuomiPitchNamesMap [kF_SemiSharp_QTP]   = "fSemiSharp???";
  gSuomiPitchNamesMap [kF_Sharp_QTP]       = "fis";
  gSuomiPitchNamesMap [kF_SesquiSharp_QTP] = "fSesquiSharp???";
  gSuomiPitchNamesMap [kF_DoubleSharp_QTP] = "fisis";
    
  gSuomiPitchNamesMap [kG_DoubleFlat_QTP]  = "geses";
  gSuomiPitchNamesMap [kG_SesquiFlat_QTP]  = "gSesquiFlat???";
  gSuomiPitchNamesMap [kG_Flat_QTP]        = "ges";
  gSuomiPitchNamesMap [kG_SemiFlat_QTP]    = "gSemiFlat???";
  gSuomiPitchNamesMap [kG_Natural_QTP]     = "g";
  gSuomiPitchNamesMap [kG_SemiSharp_QTP]   = "gSemiSharp???";
  gSuomiPitchNamesMap [kG_Sharp_QTP]       = "gis";
  gSuomiPitchNamesMap [kG_SesquiSharp_QTP] = "gSesquiSharp???";
  gSuomiPitchNamesMap [kG_DoubleSharp_QTP] = "gisis";

  // svenska
  gSvenskaPitchNamesMap [k_NoQuarterTonesPitch_QTP]  = "";
  gSvenskaPitchNamesMap [k_Rest_QTP]                 = "r";
  
  gSvenskaPitchNamesMap [kA_DoubleFlat_QTP]  = "assess";
  gSvenskaPitchNamesMap [kA_SesquiFlat_QTP]  = "aSesquiFlat???";
  gSvenskaPitchNamesMap [kA_Flat_QTP]        = "ass";
  gSvenskaPitchNamesMap [kA_SemiFlat_QTP]    = "aSemiFlat???";
  gSvenskaPitchNamesMap [kA_Natural_QTP]     = "a";
  gSvenskaPitchNamesMap [kA_SemiSharp_QTP]   = "aSemiSharp???";
  gSvenskaPitchNamesMap [kA_Sharp_QTP]       = "aiss";
  gSvenskaPitchNamesMap [kA_SesquiSharp_QTP] = "aSesquiSharp???";
  gSvenskaPitchNamesMap [kA_DoubleSharp_QTP] = "aississ";
    
  gSvenskaPitchNamesMap [kB_DoubleFlat_QTP]  = "hessess";
  gSvenskaPitchNamesMap [kB_SesquiFlat_QTP]  = "bSesquiFlat???";
  gSvenskaPitchNamesMap [kB_Flat_QTP]        = "b";
  gSvenskaPitchNamesMap [kB_SemiFlat_QTP]    = "bSemiFlat???";
  gSvenskaPitchNamesMap [kB_Natural_QTP]     = "h";
  gSvenskaPitchNamesMap [kB_SemiSharp_QTP]   = "bSemiSharp???";
  gSvenskaPitchNamesMap [kB_Sharp_QTP]       = "hiss";
  gSvenskaPitchNamesMap [kB_SesquiSharp_QTP] = "bSesquiSharp???";
  gSvenskaPitchNamesMap [kB_DoubleSharp_QTP] = "hississ";
    
  gSvenskaPitchNamesMap [kC_DoubleFlat_QTP]  = "cessess";
  gSvenskaPitchNamesMap [kC_SesquiFlat_QTP]  = "cSesquiFlat???";
  gSvenskaPitchNamesMap [kC_Flat_QTP]        = "cess";
  gSvenskaPitchNamesMap [kC_SemiFlat_QTP]    = "cSemiFlat???";
  gSvenskaPitchNamesMap [kC_Natural_QTP]     = "c";
  gSvenskaPitchNamesMap [kC_SemiSharp_QTP]   = "cSemiSharp???";
  gSvenskaPitchNamesMap [kC_Sharp_QTP]       = "ciss";
  gSvenskaPitchNamesMap [kC_SesquiSharp_QTP] = "cSesquiSharp???";
  gSvenskaPitchNamesMap [kC_DoubleSharp_QTP] = "cississ";
    
  gSvenskaPitchNamesMap [kD_DoubleFlat_QTP]  = "dessess";
  gSvenskaPitchNamesMap [kD_SesquiFlat_QTP]  = "dSesquiFlat???";
  gSvenskaPitchNamesMap [kD_Flat_QTP]        = "dess";
  gSvenskaPitchNamesMap [kD_SemiFlat_QTP]    = "dSemiFlat???";
  gSvenskaPitchNamesMap [kD_Natural_QTP]     = "d";
  gSvenskaPitchNamesMap [kD_SemiSharp_QTP]   = "dSemiSharp???";
  gSvenskaPitchNamesMap [kD_Sharp_QTP]       = "diss";
  gSvenskaPitchNamesMap [kD_SesquiSharp_QTP] = "dSesquiSharp???";
  gSvenskaPitchNamesMap [kD_DoubleSharp_QTP] = "dississ";

  gSvenskaPitchNamesMap [kE_DoubleFlat_QTP]  = "essess";
  gSvenskaPitchNamesMap [kE_SesquiFlat_QTP]  = "eSesquiFlat???";
  gSvenskaPitchNamesMap [kE_Flat_QTP]        = "ess";
  gSvenskaPitchNamesMap [kE_SemiFlat_QTP]    = "eSemiFlat???";
  gSvenskaPitchNamesMap [kE_Natural_QTP]     = "e";
  gSvenskaPitchNamesMap [kE_SemiSharp_QTP]   = "eSemiSharp???";
  gSvenskaPitchNamesMap [kE_Sharp_QTP]       = "eiss";
  gSvenskaPitchNamesMap [kE_SesquiSharp_QTP] = "eSesquiSharp???";
  gSvenskaPitchNamesMap [kE_DoubleSharp_QTP] = "eississ";
    
  gSvenskaPitchNamesMap [kF_DoubleFlat_QTP]  = "fessess";
  gSvenskaPitchNamesMap [kF_SesquiFlat_QTP]  = "fSesquiFlat???";
  gSvenskaPitchNamesMap [kF_Flat_QTP]        = "fess";
  gSvenskaPitchNamesMap [kF_SemiFlat_QTP]    = "fSemiFlat???";
  gSvenskaPitchNamesMap [kF_Natural_QTP]     = "f";
  gSvenskaPitchNamesMap [kF_SemiSharp_QTP]   = "fSemiSharp???";
  gSvenskaPitchNamesMap [kF_Sharp_QTP]       = "fiss";
  gSvenskaPitchNamesMap [kF_SesquiSharp_QTP] = "fSesquiSharp???";
  gSvenskaPitchNamesMap [kF_DoubleSharp_QTP] = "fississ";
    
  gSvenskaPitchNamesMap [kG_DoubleFlat_QTP]  = "gessess";
  gSvenskaPitchNamesMap [kG_SesquiFlat_QTP]  = "gSesquiFlat???";
  gSvenskaPitchNamesMap [kG_Flat_QTP]        = "gess";
  gSvenskaPitchNamesMap [kG_SemiFlat_QTP]    = "gSemiFlat???";
  gSvenskaPitchNamesMap [kG_Natural_QTP]     = "g";
  gSvenskaPitchNamesMap [kG_SemiSharp_QTP]   = "gSemiSharp???";
  gSvenskaPitchNamesMap [kG_Sharp_QTP]       = "giss";
  gSvenskaPitchNamesMap [kG_SesquiSharp_QTP] = "gSesquiSharp???";
  gSvenskaPitchNamesMap [kG_DoubleSharp_QTP] = "gississ";

  // vlaams
  gVlaamsPitchNamesMap [k_NoQuarterTonesPitch_QTP]  = "none";
  gVlaamsPitchNamesMap [k_Rest_QTP]                 = "r";
  
  gVlaamsPitchNamesMap [kA_DoubleFlat_QTP]  = "labb";
  gVlaamsPitchNamesMap [kA_SesquiFlat_QTP]  = "laSesquiFlat???";
  gVlaamsPitchNamesMap [kA_Flat_QTP]        = "lab";
  gVlaamsPitchNamesMap [kA_SemiFlat_QTP]    = "laSemiFlat???";
  gVlaamsPitchNamesMap [kA_Natural_QTP]     = "la";
  gVlaamsPitchNamesMap [kA_SemiSharp_QTP]   = "laSemiSharp???";
  gVlaamsPitchNamesMap [kA_Sharp_QTP]       = "lak";
  gVlaamsPitchNamesMap [kA_SesquiSharp_QTP] = "laSesquiSharp???";
  gVlaamsPitchNamesMap [kA_DoubleSharp_QTP] = "lakk";
    
  gVlaamsPitchNamesMap [kB_DoubleFlat_QTP]  = "sibb";
  gVlaamsPitchNamesMap [kB_SesquiFlat_QTP]  = "siSesquiFlat???";
  gVlaamsPitchNamesMap [kB_Flat_QTP]        = "sib";
  gVlaamsPitchNamesMap [kB_SemiFlat_QTP]    = "siSemiFlat???";
  gVlaamsPitchNamesMap [kB_Natural_QTP]     = "si";
  gVlaamsPitchNamesMap [kB_SemiSharp_QTP]   = "siSemiSharp???";
  gVlaamsPitchNamesMap [kB_Sharp_QTP]       = "sik";
  gVlaamsPitchNamesMap [kB_SesquiSharp_QTP] = "siSesquiSharp???";
  gVlaamsPitchNamesMap [kB_DoubleSharp_QTP] = "sikk";
    
  gVlaamsPitchNamesMap [kC_DoubleFlat_QTP]  = "dobb";
  gVlaamsPitchNamesMap [kC_SesquiFlat_QTP]  = "doSesquiFlat???";
  gVlaamsPitchNamesMap [kC_Flat_QTP]        = "dob";
  gVlaamsPitchNamesMap [kC_SemiFlat_QTP]    = "doSemiFlat???";
  gVlaamsPitchNamesMap [kC_Natural_QTP]     = "do";
  gVlaamsPitchNamesMap [kC_SemiSharp_QTP]   = "doSemiSharp???";
  gVlaamsPitchNamesMap [kC_Sharp_QTP]       = "dok";
  gVlaamsPitchNamesMap [kC_SesquiSharp_QTP] = "doSesquiSharp???";
  gVlaamsPitchNamesMap [kC_DoubleSharp_QTP] = "dokk";
    
  gVlaamsPitchNamesMap [kD_DoubleFlat_QTP]  = "rebb";
  gVlaamsPitchNamesMap [kD_SesquiFlat_QTP]  = "reSesquiFlat???";
  gVlaamsPitchNamesMap [kD_Flat_QTP]        = "reb";
  gVlaamsPitchNamesMap [kD_SemiFlat_QTP]    = "reSemiFlat???";
  gVlaamsPitchNamesMap [kD_Natural_QTP]     = "re";
  gVlaamsPitchNamesMap [kD_SemiSharp_QTP]   = "reSemiSharp???";
  gVlaamsPitchNamesMap [kD_Sharp_QTP]       = "rek";
  gVlaamsPitchNamesMap [kD_SesquiSharp_QTP] = "reSesquiSharp???";
  gVlaamsPitchNamesMap [kD_DoubleSharp_QTP] = "rekk";

  gVlaamsPitchNamesMap [kE_DoubleFlat_QTP]  = "mibb";
  gVlaamsPitchNamesMap [kE_SesquiFlat_QTP]  = "miSesquiFlat???";
  gVlaamsPitchNamesMap [kE_Flat_QTP]        = "mib";
  gVlaamsPitchNamesMap [kE_SemiFlat_QTP]    = "miSemiFlat???";
  gVlaamsPitchNamesMap [kE_Natural_QTP]     = "mi";
  gVlaamsPitchNamesMap [kE_SemiSharp_QTP]   = "miSemiSharp???";
  gVlaamsPitchNamesMap [kE_Sharp_QTP]       = "mik";
  gVlaamsPitchNamesMap [kE_SesquiSharp_QTP] = "miSesquiSharp???";
  gVlaamsPitchNamesMap [kE_DoubleSharp_QTP] = "mikk";
    
  gVlaamsPitchNamesMap [kF_DoubleFlat_QTP]  = "fabb";
  gVlaamsPitchNamesMap [kF_SesquiFlat_QTP]  = "faSesquiFlat???";
  gVlaamsPitchNamesMap [kF_Flat_QTP]        = "fab";
  gVlaamsPitchNamesMap [kF_SemiFlat_QTP]    = "faSemiFlat???";
  gVlaamsPitchNamesMap [kF_Natural_QTP]     = "fa";
  gVlaamsPitchNamesMap [kF_SemiSharp_QTP]   = "faSemiSharp???";
  gVlaamsPitchNamesMap [kF_Sharp_QTP]       = "fak";
  gVlaamsPitchNamesMap [kF_SesquiSharp_QTP] = "faSesquiSharp???";
  gVlaamsPitchNamesMap [kF_DoubleSharp_QTP] = "fakk";
    
  gVlaamsPitchNamesMap [kG_DoubleFlat_QTP]  = "solbb";
  gVlaamsPitchNamesMap [kG_SesquiFlat_QTP]  = "solSesquiFlat???";
  gVlaamsPitchNamesMap [kG_Flat_QTP]        = "solb";
  gVlaamsPitchNamesMap [kG_SemiFlat_QTP]    = "solSemiFlat???";
  gVlaamsPitchNamesMap [kG_Natural_QTP]     = "sol";
  gVlaamsPitchNamesMap [kG_SemiSharp_QTP]   = "solSemiSharp???";
  gVlaamsPitchNamesMap [kG_Sharp_QTP]       = "solk";
  gVlaamsPitchNamesMap [kG_SesquiSharp_QTP] = "solSesquiSharp???";
  gVlaamsPitchNamesMap [kG_DoubleSharp_QTP] = "solkk";
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
  msrQuarterTonesPitchesLanguageKind languageKind, // JMI
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

  msrAlterationKind result = k_NoAlteration;
  
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
  
  else if (alter == -3 ) {
    result = kTripleFlat;
  }
  
  else if (alter == +3 ) {
    result = kTripleSharp;
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
      result = "noAlteration";
      break;

    case kTripleFlat:
      result = "TripleFlat";
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
    case kTripleSharp:
      result = "TripleSharp";
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
    case k_NoQuarterTonesPitch_QTP:
      diatonicPitchKind = kA; // any value would fit
      alterationKind    = k_NoAlteration;
      break;

    case kA_TripleFlat_QTP:
      diatonicPitchKind = kA;
      alterationKind    = kTripleFlat;
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
    case kA_TripleSharp_QTP:
      diatonicPitchKind = kA;
      alterationKind    = kTripleSharp;
      break;

    case kB_TripleFlat_QTP:
      diatonicPitchKind = kB;
      alterationKind    = kTripleFlat;
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
    case kB_TripleSharp_QTP:
      diatonicPitchKind = kB;
      alterationKind    = kTripleSharp;
      break;

    case kC_TripleFlat_QTP:
      diatonicPitchKind = kC;
      alterationKind    = kTripleFlat;
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
    case kC_TripleSharp_QTP:
      diatonicPitchKind = kC;
      alterationKind    = kTripleSharp;
      break;

    case kD_TripleFlat_QTP:
      diatonicPitchKind = kD;
      alterationKind    = kTripleFlat;
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
    case kD_TripleSharp_QTP:
      diatonicPitchKind = kD;
      alterationKind    = kTripleSharp;
      break;

    case kE_TripleFlat_QTP:
      diatonicPitchKind = kE;
      alterationKind    = kTripleFlat;
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
    case kE_TripleSharp_QTP:
      diatonicPitchKind = kE;
      alterationKind    = kTripleSharp;
      break;

    case kF_TripleFlat_QTP:
      diatonicPitchKind = kF;
      alterationKind    = kTripleFlat;
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
    case kF_TripleSharp_QTP:
      diatonicPitchKind = kF;
      alterationKind    = kTripleSharp;
      break;

    case kG_TripleFlat_QTP:
      diatonicPitchKind = kG;
      alterationKind    = kTripleFlat;
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
    case kG_TripleSharp_QTP:
      diatonicPitchKind = kG;
      alterationKind    = kTripleSharp;
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
      k_NoQuarterTonesPitch_QTP;
  
  switch (diatonicPitchKind) {
    case kA:
      switch (alterationKind) {
        case kTripleFlat:
          result = kA_TripleFlat_QTP;
          break;
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
        case kTripleSharp:
          result = kA_TripleSharp_QTP;
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
        case kTripleFlat:
          result = kB_TripleFlat_QTP;
          break;
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
        case kTripleSharp:
          result = kB_TripleSharp_QTP;
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
        case kTripleFlat:
          result = kC_TripleFlat_QTP;
          break;
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
        case kTripleSharp:
          result = kC_TripleSharp_QTP;
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
        case kTripleFlat:
          result = kD_TripleFlat_QTP;
          break;
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
        case kTripleSharp:
          result = kD_TripleSharp_QTP;
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
        case kTripleFlat:
          result = kE_TripleFlat_QTP;
          break;
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
        case kTripleSharp:
          result = kE_TripleSharp_QTP;
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
        case kTripleFlat:
          result = kF_TripleFlat_QTP;
          break;
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
        case kTripleSharp:
          result = kF_TripleSharp_QTP;
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
        case kTripleFlat:
          result = kG_TripleFlat_QTP;
          break;
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
        case kTripleSharp:
          result = kG_TripleSharp_QTP;
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
        result = k_NoQuarterTonesPitch_QTP;
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
  msrDiatonicPitchKind result = k_NoDiatonicPitch;
  
  switch (quarterTonesPitchKind) {
    case kA_TripleFlat_QTP:
    case kA_DoubleFlat_QTP:
    case kA_SesquiFlat_QTP:
    case kA_Flat_QTP:
    case kA_SemiFlat_QTP:
    case kA_Natural_QTP:
    case kA_SemiSharp_QTP:
    case kA_Sharp_QTP:
    case kA_SesquiSharp_QTP:
    case kA_DoubleSharp_QTP:
    case kA_TripleSharp_QTP:
      result = kA;
      break;

    case kB_TripleFlat_QTP:
    case kB_DoubleFlat_QTP:
    case kB_SesquiFlat_QTP:
    case kB_Flat_QTP:
    case kB_SemiFlat_QTP:
    case kB_Natural_QTP:
    case kB_SemiSharp_QTP:
    case kB_Sharp_QTP:
    case kB_SesquiSharp_QTP:
    case kB_DoubleSharp_QTP:
    case kB_TripleSharp_QTP:
      result = kB;
      break;

    case kC_TripleFlat_QTP:
    case kC_DoubleFlat_QTP:
    case kC_SesquiFlat_QTP:
    case kC_Flat_QTP:
    case kC_SemiFlat_QTP:
    case kC_Natural_QTP:
    case kC_SemiSharp_QTP:
    case kC_Sharp_QTP:
    case kC_SesquiSharp_QTP:
    case kC_DoubleSharp_QTP:
    case kC_TripleSharp_QTP:
      result = kC;
      break;

    case kD_TripleFlat_QTP:
    case kD_DoubleFlat_QTP:
    case kD_SesquiFlat_QTP:
    case kD_Flat_QTP:
    case kD_SemiFlat_QTP:
    case kD_Natural_QTP:
    case kD_SemiSharp_QTP:
    case kD_Sharp_QTP:
    case kD_SesquiSharp_QTP:
    case kD_DoubleSharp_QTP:
    case kD_TripleSharp_QTP:
      result = kD;
      break;

    case kE_TripleFlat_QTP:
    case kE_DoubleFlat_QTP:
    case kE_SesquiFlat_QTP:
    case kE_Flat_QTP:
    case kE_SemiFlat_QTP:
    case kE_Natural_QTP:
    case kE_SemiSharp_QTP:
    case kE_Sharp_QTP:
    case kE_SesquiSharp_QTP:
    case kE_DoubleSharp_QTP:
    case kE_TripleSharp_QTP:
      result = kE;
      break;

    case kF_TripleFlat_QTP:
    case kF_DoubleFlat_QTP:
    case kF_SesquiFlat_QTP:
    case kF_Flat_QTP:
    case kF_SemiFlat_QTP:
    case kF_Natural_QTP:
    case kF_SemiSharp_QTP:
    case kF_Sharp_QTP:
    case kF_SesquiSharp_QTP:
    case kF_DoubleSharp_QTP:
    case kF_TripleSharp_QTP:
      result = kF;
      break;

    case kG_TripleFlat_QTP:
    case kG_DoubleFlat_QTP:
    case kG_SesquiFlat_QTP:
    case kG_Flat_QTP:
    case kG_SemiFlat_QTP:
    case kG_Natural_QTP:
    case kG_SemiSharp_QTP:
    case kG_Sharp_QTP:
    case kG_SesquiSharp_QTP:
    case kG_DoubleSharp_QTP:
    case kG_TripleSharp_QTP:
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
      
    case k_NoQuarterTonesPitch_QTP:
      {
        result = k_NoDiatonicPitch;
        
        /* JMI
        stringstream s;

        s <<
          "cannot get the diatonic pitch of a k_NoQuarterTonesPitch_QTP"
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

msrQuarterTonesPitchKind quarterTonesPitchKindFromSemiTonesPitchKind (
  msrSemiTonesPitchKind semiTonesPitchKind)
{
  msrQuarterTonesPitchKind result = k_NoQuarterTonesPitch_QTP;
  
  switch (semiTonesPitchKind) {
    case k_NoSemiTonesPitch_STP:
      break;
      
    case kC_TripleFlat_STP:
      result = kC_TripleFlat_QTP;
      break;
    case kC_DoubleFlat_STP:
      result = kC_DoubleFlat_QTP;
      break;
    case kC_Flat_STP:
      result = kC_Flat_QTP;
      break;
    case kC_Natural_STP:
      result = kC_Natural_QTP;
      break;
    case kC_Sharp_STP:
      result = kC_Sharp_QTP;
      break;
    case kC_DoubleSharp_STP:
      result = kC_DoubleSharp_QTP;
      break;
    case kC_TripleSharp_STP:
      result = kC_TripleSharp_QTP;
      break;
      
    case kD_TripleFlat_STP:
      result = kD_TripleFlat_QTP;
      break;
    case kD_DoubleFlat_STP:
      result = kD_DoubleFlat_QTP;
      break;
    case kD_Flat_STP:
      result = kD_Flat_QTP;
      break;
    case kD_Natural_STP:
      result = kD_Natural_QTP;
      break;
    case kD_Sharp_STP:
      result = kD_Sharp_QTP;
      break;
    case kD_DoubleSharp_STP:
      result = kD_DoubleSharp_QTP;
      break;
    case kD_TripleSharp_STP:
      result = kD_TripleSharp_QTP;
      break;
      
    case kE_TripleFlat_STP:
      result = kE_TripleFlat_QTP;
      break;
    case kE_DoubleFlat_STP:
      result = kE_DoubleFlat_QTP;
      break;
    case kE_Flat_STP:
      result = kE_Flat_QTP;
      break;
    case kE_Natural_STP:
      result = kE_Natural_QTP;
      break;
    case kE_Sharp_STP:
      result = kE_Sharp_QTP;
      break;
    case kE_DoubleSharp_STP:
      result = kE_DoubleSharp_QTP;
      break;
    case kE_TripleSharp_STP:
      result = kE_TripleSharp_QTP;
      break;
      
    case kF_TripleFlat_STP:
      result = kF_TripleFlat_QTP;
      break;
    case kF_DoubleFlat_STP:
      result = kF_DoubleFlat_QTP;
      break;
    case kF_Flat_STP:
      result = kF_Flat_QTP;
      break;
    case kF_Natural_STP:
      result = kF_Natural_QTP;
      break;
    case kF_Sharp_STP:
      result = kF_Sharp_QTP;
      break;
    case kF_DoubleSharp_STP:
      result = kF_DoubleSharp_QTP;
      break;
    case kF_TripleSharp_STP:
      result = kF_TripleSharp_QTP;
      break;
      
    case kG_TripleFlat_STP:
      result = kG_TripleFlat_QTP;
      break;
    case kG_DoubleFlat_STP:
      result = kG_DoubleFlat_QTP;
      break;
    case kG_Flat_STP:
      result = kG_Flat_QTP;
      break;
    case kG_Natural_STP:
      result = kG_Natural_QTP;
      break;
    case kG_Sharp_STP:
      result = kG_Sharp_QTP;
      break;
    case kG_DoubleSharp_STP:
      result = kG_DoubleSharp_QTP;
      break;
    case kG_TripleSharp_STP:
      result = kG_TripleSharp_QTP;
      break;
      
    case kA_TripleFlat_STP:
      result = kA_TripleFlat_QTP;
      break;
    case kA_DoubleFlat_STP:
      result = kA_DoubleFlat_QTP;
      break;
    case kA_Flat_STP:
      result = kA_Flat_QTP;
      break;
    case kA_Natural_STP:
      result = kA_Natural_QTP;
      break;
    case kA_Sharp_STP:
      result = kA_Sharp_QTP;
      break;
    case kA_DoubleSharp_STP:
      result = kA_DoubleSharp_QTP;
      break;
    case kA_TripleSharp_STP:
      result = kA_TripleSharp_QTP;
      break;
      
    case kB_TripleFlat_STP:
      result = kB_TripleFlat_QTP;
      break;
    case kB_DoubleFlat_STP:
      result = kB_DoubleFlat_QTP;
      break;
    case kB_Flat_STP:
      result = kB_Flat_QTP;
      break;
    case kB_Natural_STP:
      result = kB_Natural_QTP;
      break;
    case kB_Sharp_STP:
      result = kB_Sharp_QTP;
      break;
    case kB_DoubleSharp_STP:
      result = kB_DoubleSharp_QTP;
      break;
    case kB_TripleSharp_STP:
      result = kB_TripleSharp_QTP;
      break;
  } // switch

  return result;
}

msrSemiTonesPitchKind semiTonesPitchKindFromQuarterTonesPitchKind (
  msrQuarterTonesPitchKind quarterTonesPitchKind)
{
  msrSemiTonesPitchKind result = k_NoSemiTonesPitch_STP;
  
    switch (quarterTonesPitchKind) {
      case k_NoQuarterTonesPitch_QTP:
        break;

      case k_Rest_QTP:
        break;
  
      case kA_Flat_QTP:
        result = kA_Flat_STP;
        break;
      case kA_Natural_QTP:
        result = kA_Flat_STP;
        break;
      case kA_Sharp_QTP:
        result = kA_Sharp_STP;
        break;

      case kB_Flat_QTP:
        result = kB_Flat_STP;
        break;
      case kB_Natural_QTP:
        result = kB_Natural_STP;
        break;
      case kB_Sharp_QTP:
        result = kB_Sharp_STP;
        break;

      case kC_Flat_QTP:
        result = kC_Flat_STP;
        break;
      case kC_Natural_QTP:
        result = kC_Natural_STP;
        break;
      case kC_Sharp_QTP:
        result = kC_Sharp_STP;
        break;

      case kD_Flat_QTP:
        result = kD_Flat_STP;
        break;
      case kD_Natural_QTP:
        result = kD_Natural_STP;
        break;
      case kD_Sharp_QTP:
        result = kD_Sharp_STP;
        break;

      case kE_Flat_QTP:
        result = kE_Flat_STP;
        break;
      case kE_Natural_QTP:
        result = kE_Natural_STP;
        break;
      case kE_Sharp_QTP:
        result = kE_Sharp_STP;
        break;

      case kF_Flat_QTP:
        result = kF_Flat_STP;
        break;
      case kF_Natural_QTP:
        result = kF_Natural_STP;
        break;
      case kF_Sharp_QTP:
        result = kF_Sharp_STP;
        break;

      case kG_Flat_QTP:
        result = kG_Flat_STP;
        break;
      case kG_Natural_QTP:
        result = kG_Natural_STP;
        break;
      case kG_Sharp_QTP:
        result = kG_Sharp_STP;
        break;
  
      default:
        ;
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
      result = gNederlandsPitchNamesMap [quarterTonesPitchKind];
      break;
    case kCatalan:
      result = gCatalanPitchNamesMap [quarterTonesPitchKind];
      break;
    case kDeutsch:
      result = gDeutschPitchNamesMap [quarterTonesPitchKind];
      break;
    case kEnglish:
      result = gEnglishPitchNamesMap [quarterTonesPitchKind];
      break;
    case kEspanol:
      result = gEspanolPitchNamesMap [quarterTonesPitchKind];
      break;
    case kFrancais:
      result = gFrancaisPitchNamesMap [quarterTonesPitchKind];
      break;
    case kItaliano:
      result = gItalianoPitchNamesMap [quarterTonesPitchKind];
      break;
    case kNorsk:
      result = gNorskPitchNamesMap [quarterTonesPitchKind];
      break;
    case kPortugues:
      result = gPortuguesPitchNamesMap [quarterTonesPitchKind];
      break;
    case kSuomi:
      result = gSuomiPitchNamesMap [quarterTonesPitchKind];
      break;
    case kSvenska:
      result = gSvenskaPitchNamesMap [quarterTonesPitchKind];
      break;
    case kVlaams:
      result = gVlaamsPitchNamesMap [quarterTonesPitchKind];
      break;
  } // switch

  return result;
}

msrQuarterTonesPitchKind msrQuarterTonesPitchKindFromString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  string                             quarterTonesPitchName)
{
  msrQuarterTonesPitchKind result = k_NoQuarterTonesPitch_QTP;

  map<msrQuarterTonesPitchKind, string> *pitchNamesMapPTR;

  // is quarterTonesPitchName in the part renaming map?
  switch (languageKind) {
    case kNederlands:
      pitchNamesMapPTR = &gNederlandsPitchNamesMap;
      break;
    case kCatalan:
      pitchNamesMapPTR = &gCatalanPitchNamesMap;
      break;
    case kDeutsch:
      pitchNamesMapPTR = &gDeutschPitchNamesMap;
      break;
    case kEnglish:
      pitchNamesMapPTR = &gEnglishPitchNamesMap;
      break;
    case kEspanol:
      pitchNamesMapPTR = &gEspanolPitchNamesMap;
      break;
    case kFrancais:
      pitchNamesMapPTR = &gFrancaisPitchNamesMap;
      break;
    case kItaliano:
      pitchNamesMapPTR = &gItalianoPitchNamesMap;
      break;
    case kNorsk:
      pitchNamesMapPTR = &gNorskPitchNamesMap;
      break;
    case kPortugues:
      pitchNamesMapPTR = &gPortuguesPitchNamesMap;
      break;
    case kSuomi:
      pitchNamesMapPTR = &gSuomiPitchNamesMap;
      break;
    case kSvenska:
      pitchNamesMapPTR = &gSvenskaPitchNamesMap;
      break;
    case kVlaams:
      pitchNamesMapPTR = &gVlaamsPitchNamesMap;
      break;
  } // switch

  // is quarterTonesPitchName present in the map?
  if (gQuarterTonesPitchesLanguageKindsMap.size ()) {
    map<msrQuarterTonesPitchKind, string>::const_iterator
      iBegin = (*pitchNamesMapPTR).begin (),
      iEnd   = (*pitchNamesMapPTR).end (),
      i      = iBegin;
    for ( ; ; ) {
      if ((*i).second == quarterTonesPitchName) {
        result = (*i).first;
        break;
      }
      if (++i == iEnd) break;
    } // for
  }
  
  return result;
}

msrSemiTonesPitchKind semiTonesPitchKindFromString (
  string theString)
{
  msrSemiTonesPitchKind result;
  
  // fetch the quarternotes pitches kind
  msrQuarterTonesPitchKind
    quarterTonesPitchKindFromString =
      msrQuarterTonesPitchKindFromString (
        gLpsrOptions->
          fLpsrQuarterTonesPitchesLanguageKind,
        theString);

  // fetch the semitones pitches kind
  result =
    semiTonesPitchKindFromQuarterTonesPitchKind (
      quarterTonesPitchKindFromString);

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
  
  switch (semiTonesPitchKind) {
    case k_NoSemiTonesPitch_STP:
      result = "k_NoSemiTonesPitch_STP";
      break;
      
    case kC_TripleFlat_STP:
      result = "C_TripleFlat_STP";
      break;
    case kC_DoubleFlat_STP:
      result = "C_DoubleFlat_STP";
      break;
    case kC_Flat_STP:
      result = "C_Flat_STP";
      break;
    case kC_Natural_STP:
      result = "C_Natural_STP";
      break;
    case kC_Sharp_STP:
      result = "C_Sharp_STP";
      break;
    case kC_DoubleSharp_STP:
      result = "C_DoubleSharp_STP";
      break;
    case kC_TripleSharp_STP:
      result = "C_TripleSharp_STP";
      break;

    case kD_TripleFlat_STP:
      result = "D_TripleFlat_STP";
      break;
    case kD_DoubleFlat_STP:
      result = "D_DoubleFlat_STP";
      break;
    case kD_Flat_STP:
      result = "D_Flat_STP";
      break;
    case kD_Natural_STP:
      result = "D_Natural_STP";
      break;      
    case kD_Sharp_STP:
      result = "D_Sharp_STP";
      break;
    case kD_DoubleSharp_STP:
      result = "D_DoubleSharp_STP";
      break;
    case kD_TripleSharp_STP:
      result = "D_TripleSharp_STP";
      break;
      
    case kE_TripleFlat_STP:
      result = "E_TripleFlat_STP";
      break;
    case kE_DoubleFlat_STP:
      result = "E_DoubleFlat_STP";
      break;
    case kE_Flat_STP:
      result = "E_Flat_STP";
      break;
    case kE_Natural_STP:
      result = "E_Natural_STP";
      break;
    case kE_Sharp_STP:
      result = "E_Sharp_STP";
      break;
    case kE_DoubleSharp_STP:
      result = "E_DoubleSharp_STP";
      break;
    case kE_TripleSharp_STP:
      result = "E_TripleSharp_STP";
      break;
      
    case kF_TripleFlat_STP:
      result = "F_TripleFlat_STP";
      break;
    case kF_DoubleFlat_STP:
      result = "F_DoubleFlat_STP";
      break;
    case kF_Flat_STP:
      result = "F_Flat_STP";
      break;
    case kF_Natural_STP:
      result = "F_Natural_STP";
      break;
    case kF_Sharp_STP:
      result = "F_Sharp_STP";
      break;      
    case kF_DoubleSharp_STP:
      result = "F_DoubleSharp_STP";
      break;
    case kF_TripleSharp_STP:
      result = "F_TripleSharp_STP";
      break;
      
    case kG_TripleFlat_STP:
      result = "G_TripleFlat_STP";
      break;
    case kG_DoubleFlat_STP:
      result = "G_DoubleFlat_STP";
      break;
    case kG_Flat_STP:
      result = "G_Flat_STP";
      break;
    case kG_Natural_STP:
      result = "G_Natural_STP";
      break;
    case kG_Sharp_STP:
      result = "G_Sharp_STP";
      break;
    case kG_DoubleSharp_STP:
      result = "G_DoubleSharp_STP";
      break;
    case kG_TripleSharp_STP:
      result = "G_TripleSharp_STP";
      break;

    case kA_TripleFlat_STP:
      result = "A_TripleFlat_STP";
      break;
    case kA_DoubleFlat_STP:
      result = "A_DoubleFlat_STP";
      break;
    case kA_Flat_STP:
      result = "A_Flat_STP";
      break;
    case kA_Natural_STP:
      result = "A_Natural_STP";
      break;
    case kA_Sharp_STP:
      result = "A_Sharp_STP";
      break;
    case kA_DoubleSharp_STP:
      result = "A_DoubleSharp_STP";
      break;
    case kA_TripleSharp_STP:
      result = "A_TripleSharp_STP";
      break;

    case kB_TripleFlat_STP:
      result = "B_TripleFlat_STP";
      break;
    case kB_DoubleFlat_STP:
      result = "B_DoubleFlat_STP";
      break;      
    case kB_Flat_STP:
      result = "B_Flat_STP";
      break;
    case kB_Natural_STP:
      result = "B_Natural_STP";
      break;
    case kB_Sharp_STP:
      result = "B_Sharp_STP";
      break;
    case kB_DoubleSharp_STP:
      result = "B_DoubleSharp_STP";
      break;
    case kB_TripleSharp_STP:
      result = "B_TripleSharp_STP";
      break;
  } // switch

  return result;
}

msrQuarterTonesPitchKind msrSemiTonesPitchKindAsQuarterTonesPitchKind (
  msrSemiTonesPitchKind       semiTonesPitchKind,
  msrAlterationPreferenceKind alterationPreferenceKind)
{
  msrQuarterTonesPitchKind result = k_NoQuarterTonesPitch_QTP;

  /* JMI
  switch (semiTonesPitchKind) {
    case k_NoSemiTonesPitch_STP:
      result = k_NoQuarterTonesPitch_QTP;
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


  /* JMI
  switch (semiTonesPitchKind) {
    case k_NoSemiTonesPitch_STP:
      result = k_NoQuarterTonesPitch_QTP;
      break;

    case kC_Natural_STP: // kB_Sharp_STP, kD_DoubleFlat_STP
      result = kC_Natural_QTP;
      break;
      
    case kC_Sharp_STP: // kB_DoubleSharp_STP, kD_Flat_STP
      switch (alterationPreferenceKind) {
        case kPreferFlat:
          result = kD_Flat_QTP;
          break;
        case kPreferSharp:
          result = kC_Sharp_QTP;
          break;
      } // switch
      break;

    case kD_Natural_STP: // kC_DoubleSharp_STP, kE_DoubleFlat_STP
      result = kD_Natural_QTP;
      break;

    case kD_Sharp_STP: // kE_Flat_STP
      switch (alterationPreferenceKind) {
        case kPreferFlat:
          result = kE_Flat_QTP;
          break;
        case kPreferSharp:
          result = kD_Sharp_QTP;
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
        case kPreferFlat:
          result = kG_Flat_QTP;
          break;
        case kPreferSharp:
          result = kF_Sharp_QTP;
          break;
      } // switch
      break;
      
    case kG_Natural_STP: // kF_DoubleSharp_STP, kA_DoubleFlat_STP
      result = kG_Natural_QTP;
      break;
      
    case kG_Sharp_STP: // kA_Flat_STP
      switch (alterationPreferenceKind) {
        case kPreferFlat:
          result = kA_Flat_QTP;
          break;
        case kPreferSharp:
          result = kG_Sharp_QTP;
          break;
      } // switch
      break;
      
    case kA_Natural_STP: // kG_DoubleSharp_STP, kB_DoubleFlat_STP
      result = kA_Natural_QTP;
      break;
      
    case kA_Sharp_STP: // kB_Flat_STP
      switch (alterationPreferenceKind) {
        case kPreferFlat:
          result = kB_Flat_QTP;
          break;
        case kPreferSharp:
          result = kA_Sharp_QTP;
          break;
      } // switch
      break;

    case kB_Natural_STP: // kA_DoubleSharp_STP, kC_Flat_STP
      result = kB_Natural_QTP;
      break;
  } // switch
  */

  

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
    case kC_Flat_STP:
      result = kB_Natural_STP;
      break;

    case kC_Natural_STP:
      break;
      
    case kC_Sharp_STP:
      result = kD_Flat_STP;
      break;
    case kD_Flat_STP:
      result = kC_Sharp_STP;
      break;

    case kD_Natural_STP:
      break;

    case kD_Sharp_STP:
      result = kE_Flat_STP;
      break;
    case kE_Flat_STP:
      result = kD_Sharp_STP;
      break;
      
    case kE_Natural_STP:
      break;
      
    case kF_Natural_STP:
      break;
      
    case kF_Sharp_STP:
      result = kG_Flat_STP;
      break;
    case kG_Flat_STP:
      result = kF_Sharp_STP;
      break;
      
    case kG_Natural_STP:
      break;
      
    case kG_Sharp_STP:
      result = kA_Flat_STP;
      break;
    case kA_Flat_STP:
      result = kG_Sharp_STP;
      break;
      
    case kA_Natural_STP:
      break;
      
    case kA_Sharp_STP:
      result = kB_Flat_STP;
      break;
    case kB_Flat_STP:
      result = kA_Sharp_STP;
      break;

    case kB_Natural_STP:
      break;

    case kB_Sharp_STP:
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
  fFontSizeKind = kFontSizeNumeric;
  fFontNumericSize = fontNumericSize;
}

msrFontSize::~msrFontSize ()
{}
  
string msrFontSize::fontSizeKindAsString (
  msrFontSize::msrFontSizeKind fontSizeKind)
{
  string result;

  switch (fontSizeKind) {
    case msrFontSize::kFontSizeNone:
      result = "fontSizeNone";
      break;
    case msrFontSize::kFontSizeXXSmall:
      result = "fontSizeXXSmall";
      break;
    case msrFontSize::kFontSizeXSmall:
      result = "fontSizeXSmall";
      break;
    case msrFontSize::kFontSizeSmall:
      result = "fontSizeSmall";
      break;
    case msrFontSize::kFontSizeMedium:
      result = "fontSizeMedium";
      break;
    case msrFontSize::kFontSizeLarge:
      result = "fontSizeLarge";
      break;
    case msrFontSize::kFontSizeXLarge:
      result = "fontSizeXLarge";
      break;
    case msrFontSize::kFontSizeXXLarge:
      result = "fontSizeXXLarge";
      break;
    case msrFontSize::kFontSizeNumeric:
      result = "fontSizeNumeric";
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
    case msrFontSize::kFontSizeNone:
    case msrFontSize::kFontSizeXXSmall:
    case msrFontSize::kFontSizeXSmall:
    case msrFontSize::kFontSizeSmall:
    case msrFontSize::kFontSizeMedium:
    case msrFontSize::kFontSizeLarge:
    case msrFontSize::kFontSizeXLarge:
    case msrFontSize::kFontSizeXXLarge:
      result = fontSizeKindAsString (fFontSizeKind);
      break;
      
    case msrFontSize::kFontSizeNumeric:
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
    case msrFontSize::kFontSizeNone:
    case msrFontSize::kFontSizeXXSmall:
    case msrFontSize::kFontSizeXSmall:
    case msrFontSize::kFontSizeSmall:
    case msrFontSize::kFontSizeMedium:
    case msrFontSize::kFontSizeLarge:
    case msrFontSize::kFontSizeXLarge:
    case msrFontSize::kFontSizeXXLarge:
      {
        stringstream s;

        s <<
          "attempting to get font numeric size for a " <<
          fontSizeKindAsString (fFontSizeKind);

        msrInternalError (
          gXml2lyOptions->fInputSourceName,
          NO_INPUT_LINE_NUMBER, // JMI
          __FILE__, __LINE__,
          s.str ());
      }
      break;
      
    case msrFontSize::kFontSizeNumeric:
      result = fFontNumericSize;
      break;
    } // switch

  return result;
}

void msrFontSize::print (ostream& os)
{
  switch (fFontSizeKind) {
    case msrFontSize::kFontSizeNone:
    case msrFontSize::kFontSizeXXSmall:
    case msrFontSize::kFontSizeXSmall:
    case msrFontSize::kFontSizeSmall:
    case msrFontSize::kFontSizeMedium:
    case msrFontSize::kFontSizeLarge:
    case msrFontSize::kFontSizeXLarge:
    case msrFontSize::kFontSizeXXLarge:
      os <<
        msrFontSize::fontSizeKindAsString (fFontSizeKind);
      break;
      
    case msrFontSize::kFontSizeNumeric:
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
    case kFontStyleNone:
      result = "fontStyleNone";
      break;
    case kFontStyleNormal:
      result = "fontStyleNormal";
      break;
    case KFontStyleItalic:
      result = "fontStyleItalic";
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
    case kFontWeightNone:
      result = "fontWeightNone";
      break;
    case kFontWeightNormal:
      result = "fontWeightNormal";
      break;
    case kFontWeightBold:
      result = "fontWeightBold";
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
    case kJustifyNone:
      result = "justifyNone";
      break;
    case kJustifyLeft:
      result = "justifyLeft";
      break;
    case kJustifyCenter:
      result = "justifyCenter";
      break;
    case kJustifyRight:
      result = "justifyRight";
      break;
    } // switch

  return result;
}

string msrVerticalAlignmentKindAsString (
  msrVerticalAlignmentKind verticalAlignmentKind)
{
  string result;
  
  switch (verticalAlignmentKind) {
    case kVerticalAlignmentNone:
      result = "verticalAlignmentNone";
      break;
    case kVerticalAlignmentTop:
      result = "verticalAlignmentTop";
      break;
    case kVerticalAlignmentMiddle:
      result = "verticalAlignmentMiddle";
      break;
    case kVerticalAlignmentBottom:
      result = "verticalAlignmentBottom";
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
    case kDirectionNone:
      result = "directionNone";
      break;

    case kDirectionUp:
      result = "directionUp";
      break;
    case kDirectionDown:
      result = "directionDown";
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
    case msrPlacementKind::kPlacementNone:
      result = "placementNone";
      break;
    case msrPlacementKind::kPlacementAbove:
      result = "placementAbove";
      break;
    case msrPlacementKind::kPlacementBelow:
      result = "placementBelow";
      break;
  } // switch

  return result;
}

// durations
//______________________________________________________________________________
msrDurationKind msrDurationKindFromString (
  int    inputLineNumber,
  string durationString)
{
  msrDurationKind result = k_NoDuration;

  if      (durationString == "maxima") {
    result = kMaxima;
  }
  else if (durationString == "long") {
    result = kLong;
  }
  else if (durationString == "breve") {
    result = kBreve;
  } 
  else if (durationString == "whole") {
    result = kWhole;
  } 
  else if (durationString == "half") {
    result = kHalf;
  } 
  else if (durationString == "quarter") {
    result = kQuarter;
  } 
  else if (durationString == "eighth") {
    result = kEighth;
  } 
  else if (durationString == "16th") {
    result = k16th;
  } 
  else if (durationString == "32nd") {
    result = k32nd;
  } 
  else if (durationString == "64th") {
    result = k64th;
  } 
  else if (durationString == "128th") {
    result = k128th;
  } 
  else if (durationString == "256th") {
    result = k256th;
  } 
  else if (durationString == "512th") {
    result = k512th;
  } 
  else if (durationString == "1024th") {
    result = k1024th;
  }
  else {
    stringstream s;
    
    s <<
      "durationString \"" << durationString <<
      "\" is unknown";

    msrMusicXMLError (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

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
      result = "noDuration";
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
int msrDurationBinaryLogarithm (int duration)
{
  int result = INT_MIN;

/*
with MusicXML's limitation to 1024th of a whole note,
valid denominators binary logarithms, i.e. their exponent, are:
*/

  switch (duration) {
    case 1:
      result = 0;
      break;
    case 2:
      result = 1;
      break;
    case 4:
      result = 2;
      break;
    case 8:
      result = 3;
      break;
    case 16:
      result = 4;
      break;
    case 32:
      result = 5;
      break;
    case 64:
      result = 6;
      break;
    case 128:
      result = 7;
      break;
    case 256:
      result = 8;
      break;
    case 512:
      result = 9;
      break;
    case 1024:
      result = 10;
      break;

    default:
      ;
  } // switch

  return result;
}

//_______________________________________________________________________________
int msrNumberOfDots (int n)
{
  int  result = INT_MIN;

   switch (n) {
    case 1:
      result = 0;
      break;
    case 3:
      result = 1;
      break;
    case 7:
      result = 2;
      break;
    case 15:
      result = 3;
      break;
    case 31:
      result = 4;
      break;
    case 63:
      result = 5;
      break;
    case 127:
      result = 6;
      break;
    case 255:
      result = 7;
      break;
    case 511:
      result = 8;
      break;
    case 1023:
      result = 9;
      break;

    default:
      ;
    } // switch

  return result;
}

//_______________________________________________________________________________
string wholeNotesAsMsrString (
  int      inputLineNumber,
  rational wholeNotes,
  int&     dotsNumber)
{
#define DEBUG_WHOLE_NOTES 0

  if (DEBUG_WHOLE_NOTES) {
    gLogIOstream <<
      "--> wholeNotes: " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }

  int
    numerator    = wholeNotes.getNumerator (),
    denominator  = wholeNotes.getDenominator ();

  if (DEBUG_WHOLE_NOTES) {
    gLogIOstream <<
      "--> numerator:   " << numerator <<
      endl <<
      "--> denominator: " << denominator <<
      endl <<
      endl;
  }

  if (numerator == 0) { // JMI TEMP
    dotsNumber = 0;
    return "zero";
  }
  
  msrAssert (
    numerator > 0,
    "numerator is not positive");
    
  wholeNotes.rationalise ();

  if (DEBUG_WHOLE_NOTES) {
    gLogIOstream <<
      "--> wholeNotes rationalised: " << wholeNotes <<
      endl;
  }

  bool
    rationalHasBeenSimplified =
      wholeNotes.getNumerator () != numerator; // denominators could be used too

  if (rationalHasBeenSimplified) {
    numerator    = wholeNotes.getNumerator (),
    denominator  = wholeNotes.getDenominator ();
  }

  bool
    integralNumberOfWholeNotes = denominator == 1;

  if (DEBUG_WHOLE_NOTES) {
    gLogIOstream <<
      "--> rationalHasBeenSimplified: " <<
      booleanAsString (
        rationalHasBeenSimplified) <<
      endl <<
      "--> integralNumberOfWholeNotes: " <<
      booleanAsString (
        integralNumberOfWholeNotes) <<
      endl <<
      endl;
  }

  /*
    augmentation dots add half the preceding duration or increment to the duration:
    they constitue a series of frations or the form '(2^n-1) / 2^n',
    starting with 3/2, 7/4, 15/8,
    that tends towards 2 while always remaining less than two.
    
    with MusicXML's limitation to 1024th of a whole note,
    with LilyPond's limitation to 128th of a whole note,
    valid numerators are:
  */

  int  numeratorDots = msrNumberOfDots (numerator);

  if (DEBUG_WHOLE_NOTES) {
    gLogIOstream <<
      "--> numeratorDots " << " : " << numeratorDots <<
      endl <<
      endl;
  }

  /*
    valid denominators are powers of 2
    
    the rational representing a dotted duration has to be brought
    to a value less than two, as explained above
    
    this is done by changing it denominator in the resulting string:
    
     whole notes        string
         3/1              \breve.
         3/2              1.
         3/4              2.
         3/8              4.
    
         7/1              \longa..
         7/2              \breve..
         7/4              1..
         7/8              2..         
    
    since such resulting denominators can be fractions of wholes notes
    as well as multiple thereof,
    we'll be better of using binary logarithms for the computations
  */

  int durationLog = msrDurationBinaryLogarithm (denominator);

  if (durationLog == INT_MIN) {
    string result;
    
    {
      string durationToUse = "64"; // JMI
  
      stringstream s;
      
      s <<
        durationToUse <<
        "*" <<
        durationToUse <<
        "/" <<
        numerator;
  
      result = s.str ();
    }

    {
      stringstream s;
          
      s <<
        "denominator " << denominator <<
        " is no power of two between 1 and 128" <<
   //     " is no power of 2 between 1 and 1024" <<
        ", whole notes duration " <<
        numerator << "/" << denominator;
  
      if (rationalHasBeenSimplified) {
        s <<
          " (" << numerator << "/" << denominator << ")" <<
        endl;
      }
  
      s <<
        " cannot be represented as a dotted power of 2" <<
        ", " <<
        result <<
        " will be used";
  
   //   msrMusicXMLError ( JMI
      msrMusicXMLWarning (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
    //    __FILE__, __LINE__,
        s.str ());
    }

    return result;
  }

  if (DEBUG_WHOLE_NOTES) {
    gLogIOstream <<
      "--> durationLog" << " : " <<
      durationLog <<
      endl <<
      endl;
  }

  // bring the resulting fraction to be less that two if needed
  if (integralNumberOfWholeNotes) {
    // adapt the duration to avoid even numerators if can be,
    // since dotted durations cannot be recognized otherwise
    // 6/1 thus becomes 3 \breve, hence '\longa.'
    while (numerator % 2 == 0) {
      numerator /= 2;
      durationLog -= 1;

      if (DEBUG_WHOLE_NOTES) {
        gLogIOstream <<
          "--> numerator" << " : " <<
          numerator <<
          endl <<
          "--> durationLog " << " : " <<
          durationLog <<
          endl <<
          endl;
      }
    } // while

    // update the number of dots
    numeratorDots = msrNumberOfDots (numerator);
  }

  // take care of the dots
  int multiplyingFactor = 1;

  if (numeratorDots >= 0 && durationLog >= numeratorDots) {
    // take the dots into account
    durationLog -= numeratorDots;

    if (DEBUG_WHOLE_NOTES) {
      gLogIOstream <<
        "--> durationLog" << " : " <<
        durationLog <<
        endl <<
        "--> multiplyingFactor " << " : " <<
        multiplyingFactor <<
        endl <<
        endl;
    }
  }
  else {
    // set the multiplying factor
    multiplyingFactor = numerator;

    if (DEBUG_WHOLE_NOTES) {
      gLogIOstream <<
        "--> durationLog" << " : " <<
        durationLog <<
        endl <<
        "--> multiplyingFactor " << " : " <<
        multiplyingFactor <<
        endl <<
        endl;
    }
    
    while (multiplyingFactor >= 2) {
      // double duration
      durationLog--;
      
      // adapt multiplying factor
      multiplyingFactor /= 2;

      if (DEBUG_WHOLE_NOTES) {
        gLogIOstream <<
          "--> durationLog" << " : " <<
          durationLog <<
          endl <<
          "--> multiplyingFactor " << " : " <<
          multiplyingFactor <<
          endl <<
          endl;
      }
    } // while
  }

  if (DEBUG_WHOLE_NOTES) {
    gLogIOstream <<
      "--> numerator " << " : " <<
      numerator <<
      endl <<
      "--> numeratorDots " << " : " <<
      numeratorDots <<
      endl <<
      "--> durationLog" << " : " <<
      durationLog <<
      endl <<
      "--> multiplyingFactor " << " : " <<
      multiplyingFactor <<
      endl <<
      endl;
  }
  
  // generate the code for the duration
  stringstream s;

  switch (durationLog) {
    case -3:
      s << "\\maxima";
      break;
    case -2:
      s << "\\longa";
      break;
    case -1:
      s << "\\breve";
      break;

    default:
      s << (1 << durationLog);
  } // switch

  // append the dots if any
  if (numeratorDots > 0) {
    for (int i = 0; i < numeratorDots; i++) {
      s << ".";
    } // for
  }

  if (multiplyingFactor != 1) {
    // append the multiplying factor
    if (integralNumberOfWholeNotes) {
      s <<
        "*" << multiplyingFactor;
    }
    else {
      s <<
        "*" << multiplyingFactor << "/" << 1; // ??? denominator;
    }
  }
  
  if (DEBUG_WHOLE_NOTES) {
    gLogIOstream <<
      "--> return:" <<
      endl <<
      "  --> s.str ()     " << " : \"" << s.str () << "\"" <<
      endl <<
      "  --> numeratorDots" << " : " << numeratorDots <<
      endl <<
      endl;
  }
  
  // return the result
  dotsNumber = numeratorDots;
  
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

string multipleRestWholeNotesAsMsrString (
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

  if (numberOfWholeNotes != 1) {
    s <<
      "*" << numberOfWholeNotes;
  }

  return s.str ();
}

// measure style
//______________________________________________________________________________
string msrSlashTypeKindAsString (
  msrSlashTypeKind slashTypeKind)
{
  string result;

  switch (slashTypeKind) {
    case k_NoSlashType:
      result = "noSlashType";
      break;
    case kSlashTypeStart:
      result = "slashTypeStart";
      break;
    case kSlashTypeStop:
      result = "slashTypeStop";
      break;
  } // switch

  return result;
}

string msrSlashUseDotsKindAsString (
  msrSlashUseDotsKind slashUseDotsKind)
{
  string result;

  switch (slashUseDotsKind) {
    case k_NoSlashUseDots:
      result = "noSlashUseDots";
      break;
    case kSlashUseDotsYes:
      result = "slashUseDotsYes";
      break;
    case kSlashUseDotsNo:
      result = "slashUseDotsNo";
      break;
  } // switch

  return result;
}

string msrSlashUseStemsKindAsString (
  msrSlashUseStemsKind slashUseStemsKind)
{
  string result;

  switch (slashUseStemsKind) {
    case k_NoSlashUseStems:
      result = "noSlashUseStems";
      break;
    case kSlashUseStemsYes:
      result = "slashUseStemsYes";
      break;
    case kSlashUseStemsNo:
      result = "slashUseStemsNo";
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
    case k_NoTremoloType:
      result = "noTremoloType";
      break;
    case kTremoloTypeSingle:
      result = "tremoloTypeSingle";
      break;
    case kTremoloTypeStart:
      result = "tremoloTypeStart";
      break;
    case kTremoloTypeStop:
      result = "tremoloTypeStop";
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
      result = "noTechnicalType";
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
      result = "noSpannerType";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
S_msrChordInterval msrChordInterval::create (
// JMI  int             inputLineNumber,
  msrIntervalKind chordIntervalIntervalKind,
  int             chordIntervalRelativeOctave)
{
  msrChordInterval* o =
    new msrChordInterval (
 //     inputLineNumber,
 //     chordIntervalNumber,
      chordIntervalIntervalKind,
      chordIntervalRelativeOctave);
  assert(o!=0);

  return o;
}

msrChordInterval::msrChordInterval (
// JMI  int             inputLineNumber,
  msrIntervalKind chordIntervalIntervalKind,
  int             chordIntervalRelativeOctave)
  // JMI  : msrElement (inputLineNumber)
{  
  fChordIntervalIntervalKind = chordIntervalIntervalKind;

  fChordIntervalRelativeOctave = chordIntervalRelativeOctave;

  if (TRACE_MSR_BASIC_TYPES) {
    gLogIOstream <<
      "==> Creating chord item '" <<
      chordIntervalAsString () <<
      "'" <<
      endl;
  }
}

msrChordInterval::~msrChordInterval ()
{}

S_msrChordInterval msrChordInterval::createChordIntervalNewbornClone ()
{
  S_msrChordInterval
    newbornClone =
      msrChordInterval::create (
        fChordIntervalIntervalKind,
        fChordIntervalRelativeOctave);
  
  return newbornClone;
}

string msrChordInterval::chordIntervalAsString () const
{
  stringstream s;

  const int fieldWidth = 19;
  
  s << left <<
    "ChordInterval" <<
    ": " <<
    setw (fieldWidth) <<
    msrIntervalKindAsString (fChordIntervalIntervalKind) <<
    "chordIntervalRelativeOctave: " << fChordIntervalRelativeOctave;

  return s.str ();
}

string msrChordInterval::chordIntervalAsShortString () const
{
  return
    msrIntervalKindAsString (fChordIntervalIntervalKind);
}


void msrChordInterval::normalizeInterval ()
{
  // bring the interval below an octave
  if (fChordIntervalIntervalKind > kAugmentedSeventh) {
    fChordIntervalIntervalKind =
      msrIntervalKind (
        fChordIntervalIntervalKind - kAugmentedSeventh);

    fChordIntervalRelativeOctave += 1;
  }
}

void msrChordInterval::deNormalizeInterval ()
{
  // bring the interval above the octave if its relative octave is 1
  if (
    fChordIntervalRelativeOctave == 1
      &&
    fChordIntervalIntervalKind <= kAugmentedSeventh) {
    fChordIntervalIntervalKind =
      msrIntervalKind (
        fChordIntervalIntervalKind + kAugmentedSeventh);

    fChordIntervalRelativeOctave -= 1;
  }
}

S_msrChordInterval msrChordInterval::intervalDifference (
  S_msrChordInterval otherChordInterval)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceExtraChords) {
    gLogIOstream <<
      endl <<
      "--> computing intervalDifference betwwen '" <<
      asShortString () <<
      "' and '" <<
      otherChordInterval->asShortString () <<
      "'" <<
      endl;
  }
#endif
  
  msrIntervalKind resultIntervalKind   = k_NoIntervalKind;

  S_msrChordInterval
    operand1 =
      this->createChordIntervalNewbornClone (),
    operand2 =
      otherChordInterval->createChordIntervalNewbornClone ();
    
  // normalize both intervals
  operand1->
    normalizeInterval ();
  operand2->
    normalizeInterval ();
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceExtraChords) {
    gLogIOstream <<
      "--> normalized operands are '" <<
      operand1->asShortString () <<
      "' and '" <<
      operand2->asShortString () <<
      "'" <<
      endl;
  }
#endif
  
  // fetch the relative octaves
  int
    relativeOctave1 = operand1->fChordIntervalRelativeOctave,
    relativeOctave2 = operand2->fChordIntervalRelativeOctave;

  // fetch the interval kind
  msrIntervalKind
    intervalKind1 =
      operand1->fChordIntervalIntervalKind,
    intervalKind2 =
      operand2->fChordIntervalIntervalKind;

  // order the operands so that
  // relativeOctave1 is greater or equal to relativeOctave2
  bool permuteRelativeOctaves = false;
  
  if (relativeOctave1 < relativeOctave2) {
    int saveRelativeOctave1 = relativeOctave1;
    
    relativeOctave1 = relativeOctave2;
    relativeOctave2 = saveRelativeOctave1;
        
    permuteRelativeOctaves = true;
  }

  // order the intervals so that
  // intervalKind1 is greater or equal to intervalKind2
  // according to the enum type
  bool invertInterval = false;
  
  if (intervalKind1 < intervalKind2) {
    msrIntervalKind saveIntervalKind1 = intervalKind1;

    intervalKind1 = intervalKind2;
    intervalKind2 = saveIntervalKind1;
        
    invertInterval = true;
  }
    
  // compute the resulting relative octaves difference
  int
    resultRelativeOctave =
      relativeOctave1 - relativeOctave2;
  if (invertInterval) {
    resultRelativeOctave--;
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceExtraChords) {
    gLogIOstream <<
      "--> permuteRelativeOctaves = " <<
      booleanAsString (permuteRelativeOctaves) <<
      ", invertInterval = " <<
      booleanAsString (invertInterval) <<
      ", resultRelativeOctave = " <<
      resultRelativeOctave <<
      endl;
  }
#endif
  
  // compute resulting interval Kind
  switch (intervalKind1) {
    case k_NoIntervalKind:
      break;
      
    case kDiminishedUnisson:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kPerfectUnison:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;

    case kAugmentedUnison:
      switch (intervalKind2) {
        case kDiminishedUnisson:
     // JMI     resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;

      
    case kDiminishedSecond:
      switch (intervalKind2) {
        case kDiminishedUnisson:
   // JMI       resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
  // JMI        resultIntervalKind = kDiminishedSecond;
          break;
        case kAugmentedUnison:
    // JMI      resultIntervalKind = kAugmentedUnison;
          break;
        case kDiminishedSecond:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kMinorSecond:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kMajorSecond;
          break;
        case kPerfectUnison:
          resultIntervalKind = kMinorSecond;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedSecond;
          break;
        case kDiminishedSecond:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kMinorSecond:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kMajorSecond:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedSecond;
          break;
        case kPerfectUnison:
          resultIntervalKind = kMajorSecond;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kMinorSecond;
          break;
        case kDiminishedSecond:
          resultIntervalKind = kDiminishedSecond;
          break;
        case kMinorSecond:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kMajorSecond:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kAugmentedSecond:
      switch (intervalKind2) {
        case kDiminishedUnisson:
  // JMI        resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
     // JMI     resultIntervalKind = kAugmentedSecond;
          break;
        case kAugmentedUnison:
  // JMI        resultIntervalKind = kMajorSecond;
          break;
        case kDiminishedSecond:
          resultIntervalKind = kAugmentedSecond;
          break;
        case kMinorSecond:
     // JMI     resultIntervalKind = kMajorSecond;
          break;
        case kMajorSecond:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
      
    case kDiminishedThird:
      switch (intervalKind2) {
        case kDiminishedUnisson:
   // JMI       resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kDiminishedThird;
          break;
        case kAugmentedUnison:
     // JMI     resultIntervalKind = kDiminishedOctave;
          break;
        case kDiminishedSecond:
          resultIntervalKind = kAugmentedSecond;
          break;
        case kMinorSecond:
          resultIntervalKind = kMinorSecond;
          break;
        case kMajorSecond:
          resultIntervalKind = kDiminishedSecond;
          break;
        case kAugmentedSecond:
     // JMI     resultIntervalKind = kAugmentedUnison;
          break;
        case kDiminishedThird:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kMinorThird:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kMajorThird;
          break;
        case kPerfectUnison:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedThird;
          break;
        case kDiminishedSecond:
          resultIntervalKind = kAugmentedSecond;
          break;
        case kMinorSecond:
          resultIntervalKind = kMajorSecond;
          break;
        case kMajorSecond:
          resultIntervalKind = kMinorSecond;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedSecond;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kMinorThird:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kMajorThird:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedThird;
          break;
        case kPerfectUnison:
          resultIntervalKind = kMajorThird;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedSecond:
    // JMI      resultIntervalKind = kPerfectUnison;
          break;
        case kMinorSecond:
          resultIntervalKind = kAugmentedSecond;
          break;
        case kMajorSecond:
          resultIntervalKind = kMajorSecond;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kMinorSecond;
          break;
        case kDiminishedThird:
  // JMI        resultIntervalKind = kMajorSecond;
          break;
        case kMinorThird:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kMajorThird:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kAugmentedThird:
      switch (intervalKind2) {
        case kDiminishedUnisson:
   // JMI       resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kAugmentedThird;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kMajorThird;
          break;
        case kDiminishedSecond:
    //      resultIntervalKind = kPerfectUnison;
          break;
        case kMinorSecond:
   // JMI      resultIntervalKind = kMajorSeventh;
          break;
        case kMajorSecond:
          resultIntervalKind = kDiminishedSecond;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kMajorSecond;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedSecond;
          break;
        case kMinorThird:
          resultIntervalKind = kMajorSecond;
          break;
        case kMajorThird:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kAugmentedThird:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;

      
    case kDiminishedFourth:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kPerfectFourth;
          break;
        case kPerfectUnison:
          resultIntervalKind = kDiminishedFourth;
          break;
        case kAugmentedUnison:
   // JMI       resultIntervalKind = kPerfectFourth;
          break;
        case kDiminishedSecond:
          resultIntervalKind = kMajorThird;
          break;
        case kMinorSecond:
          resultIntervalKind = kMinorThird;
          break;
        case kMajorSecond:
          resultIntervalKind = kAugmentedThird;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedSeventh;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedSecond;
          break;
        case kMinorThird:
          resultIntervalKind = kMinorSecond;
          break;
        case kMajorThird:
          resultIntervalKind = kDiminishedSecond;
          break;
        case kAugmentedThird:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kDiminishedFourth:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kPerfectFourth:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedFourth;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectFourth;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedFourth;
          break;
        case kDiminishedSecond:
   // JMI       resultIntervalKind = kPerfectUnison;
          break;
        case kMinorSecond:
          resultIntervalKind = kMajorThird;
          break;
        case kMajorSecond:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedThird;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedSecond;
          break;
        case kMinorThird:
          resultIntervalKind = kMajorSecond;
          break;
        case kMajorThird:
          resultIntervalKind = kMinorSecond;
          break;
        case kAugmentedThird:
          resultIntervalKind = kDiminishedSecond;
          break;
        case kDiminishedFourth:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectFourth:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kAugmentedFourth:
      switch (intervalKind2) {
        case kDiminishedUnisson:
   // JMI       resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kAugmentedFourth;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kPerfectFourth;
          break;
        case kDiminishedSecond:
   // JMI       resultIntervalKind = kPerfectUnison;
          break;
        case kMinorSecond:
          resultIntervalKind = kAugmentedThird;
          break;
        case kMajorSecond:
          resultIntervalKind = kMajorThird;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedThird:
     // JMI     resultIntervalKind = kMinorThird;
          break;
        case kMinorThird:
          resultIntervalKind = kAugmentedSecond;
          break;
        case kMajorThird:
          resultIntervalKind = kMajorSecond;
          break;
        case kAugmentedThird:
          resultIntervalKind = kMinorSecond;
          break;
        case kDiminishedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFourth:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kAugmentedFourth:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;

      
    case kDiminishedFifth:
      switch (intervalKind2) {
        case kDiminishedUnisson:
 // JMI         resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kDiminishedFifth;
          break;
        case kAugmentedUnison:
  // JMI        resultIntervalKind = kDiminishedOctave;
          break;
        case kDiminishedSecond:
          resultIntervalKind = kAugmentedFourth;
          break;
        case kMinorSecond:
          resultIntervalKind = kPerfectFourth;
          break;
        case kMajorSecond:
          resultIntervalKind = kDiminishedFourth;
          break;
        case kAugmentedSecond:
     // JMI     resultIntervalKind = kDiminishedSeventh;
          break;
        case kDiminishedThird:
          resultIntervalKind = kMajorThird;
          break;
        case kMinorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kMajorThird:
          resultIntervalKind = kDiminishedThird;
          break;
        case kAugmentedThird:
      // JMI    resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFourth:
          resultIntervalKind = kMajorSecond;
          break;
        case kPerfectFourth:
          resultIntervalKind = kDiminishedSecond;
          break;
        case kAugmentedFourth:
  // JMI        resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFifth:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kPerfectFifth:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedFifth;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectFifth;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedFifth;
          break;
        case kDiminishedSecond:
    // JMI      resultIntervalKind = kPerfectUnison;
          break;
        case kMinorSecond:
          resultIntervalKind = kAugmentedFourth;
          break;
        case kMajorSecond:
          resultIntervalKind = kPerfectFourth;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedFourth;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedThird;
          break;
        case kMinorThird:
          resultIntervalKind = kMajorThird;
          break;
        case kMajorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedThird:
          resultIntervalKind = kDiminishedThird;
          break;
        case kDiminishedFourth:
          resultIntervalKind = kAugmentedSecond;
          break;
        case kPerfectFourth:
          resultIntervalKind = kMajorSecond;
          break;
        case kAugmentedFourth:
          resultIntervalKind = kMinorSecond;
          break;
        case kDiminishedFifth:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectFifth:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kAugmentedFifth:
      switch (intervalKind2) {
        case kDiminishedUnisson:
  // JMI        resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kAugmentedFifth;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kPerfectFifth;
          break;
        case kDiminishedSecond:
          resultIntervalKind = kDiminishedFifth;
          break;
        case kMinorSecond:
   // JMI       resultIntervalKind = kMajorSeventh;
          break;
        case kMajorSecond:
          resultIntervalKind = kAugmentedFifth;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kPerfectFifth;
          break;
        case kDiminishedThird:
  // JMI        resultIntervalKind = kAugmentedSixth;
          break;
        case kMinorThird:
          resultIntervalKind = kAugmentedThird;
          break;
        case kMajorThird:
          resultIntervalKind = kMajorThird;
          break;
        case kAugmentedThird:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFourth:
 // JMI         resultIntervalKind = kMinorThird;
          break;
        case kPerfectFourth:
          resultIntervalKind = kAugmentedSecond;
          break;
        case kAugmentedFourth:
          resultIntervalKind = kMajorSecond;
          break;
        case kDiminishedFifth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFifth:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kAugmentedFifth:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;


    case kDiminishedSixth:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kMinorSixth;
          break;
        case kPerfectUnison:
          resultIntervalKind = kDiminishedSixth;
          break;
        case kAugmentedUnison:
   // JMI       resultIntervalKind = kMinorSixth;
          break;
        case kDiminishedSecond:
          resultIntervalKind = kMajorSecond;
          break;
        case kMinorSecond:
          resultIntervalKind = kAugmentedSecond;
          break;
        case kMajorSecond:
  // JMI        resultIntervalKind = kMinorSeventh;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedSeventh;
          break;
        case kDiminishedThird:
          resultIntervalKind = kPerfectFourth;
          break;
        case kMinorThird:
          resultIntervalKind = kAugmentedFourth;
          break;
        case kMajorThird:
    // JMI      resultIntervalKind = kDiminishedThird;
          break;
        case kAugmentedThird:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFourth:
          resultIntervalKind = kMajorThird;
          break;
        case kPerfectFourth:
          resultIntervalKind = kAugmentedThird;
          break;
        case kAugmentedFourth:
     // JMI     resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFifth:
         resultIntervalKind = kMajorSecond;
          break;
        case kPerfectFifth:
         resultIntervalKind = kAugmentedSecond;
          break;
        case kAugmentedFifth:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kDiminishedSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kMinorSixth:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kMajorSixth;
          break;
        case kPerfectUnison:
          resultIntervalKind = kMinorSixth;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedSixth;
          break;
        case kDiminishedSecond:
          resultIntervalKind = kAugmentedFifth;
          break;
        case kMinorSecond:
          resultIntervalKind = kPerfectFifth;
          break;
        case kMajorSecond:
          resultIntervalKind = kDiminishedFifth;
          break;
        case kAugmentedSecond:
   // JMI       resultIntervalKind = kDiminishedSeventh;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedFourth;
          break;
        case kMinorThird:
          resultIntervalKind = kPerfectFourth;
          break;
        case kMajorThird:
          resultIntervalKind = kDiminishedFourth;
          break;
        case kAugmentedThird:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFourth:
          resultIntervalKind = kMajorThird;
          break;
        case kPerfectFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedFourth:
          resultIntervalKind = kDiminishedThird;
          break;
        case kDiminishedFifth:
          resultIntervalKind = kMajorSecond;
          break;
        case kPerfectFifth:
          resultIntervalKind = kMinorSecond;
          break;
        case kAugmentedFifth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kDiminishedSixth:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kMinorSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kMajorSixth:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedSixth;
          break;
        case kPerfectUnison:
          resultIntervalKind = kMajorSixth;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kMinorSixth;
          break;
        case kDiminishedSecond:
          resultIntervalKind = kDiminishedSixth;
          break;
        case kMinorSecond:
          resultIntervalKind = kAugmentedFifth;
          break;
        case kMajorSecond:
          resultIntervalKind = kPerfectFifth;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedFifth;
          break;
        case kDiminishedThird:
   // JMI       resultIntervalKind = kAugmentedSixth;
          break;
        case kMinorThird:
          resultIntervalKind = kAugmentedFourth;
          break;
        case kMajorThird:
          resultIntervalKind = kPerfectFourth;
          break;
        case kAugmentedThird:
          resultIntervalKind = kDiminishedFourth;
          break;
        case kDiminishedFourth:
          resultIntervalKind = kAugmentedThird;
          break;
        case kPerfectFourth:
          resultIntervalKind = kMajorThird;
          break;
        case kAugmentedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFifth:
          resultIntervalKind = kAugmentedSecond;
          break;
        case kPerfectFifth:
          resultIntervalKind = kMajorSecond;
          break;
        case kAugmentedFifth:
          resultIntervalKind = kDiminishedSecond;
          break;
        case kDiminishedSixth:
   // JMI       resultIntervalKind = kPerfectUnison;
          break;
        case kMinorSixth:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kMajorSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kAugmentedSixth:
      switch (intervalKind2) {
        case kDiminishedUnisson:
   //       resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kAugmentedSixth;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kMajorSixth;
          break;
        case kDiminishedSecond:
    // JMI      resultIntervalKind = kPerfectUnison;
          break;
        case kMinorSecond:
    // JMI      resultIntervalKind = kMajorSeventh;
          break;
        case kMajorSecond:
          resultIntervalKind = kAugmentedFifth;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kPerfectFifth;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedSixth;
          break;
        case kMinorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kMajorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedThird:
          resultIntervalKind = kPerfectFourth;
          break;
        case kDiminishedFourth:
     // JMI     resultIntervalKind = kDiminishedFifth;
          break;
        case kPerfectFourth:
          resultIntervalKind = kAugmentedThird;
          break;
        case kAugmentedFourth:
          resultIntervalKind = kMajorThird;
          break;
        case kDiminishedFifth:
    // JMI      resultIntervalKind = kMinorThird;
          break;
        case kPerfectFifth:
          resultIntervalKind = kAugmentedSecond;
          break;
        case kAugmentedFifth:
          resultIntervalKind = kMajorSecond;
          break;
        case kDiminishedSixth:
    // JMI      resultIntervalKind = kPerfectUnison;
          break;
        case kMinorSixth:
    // JMI      resultIntervalKind = kPerfectUnison;
          break;
        case kMajorSixth:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kAugmentedSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;

      
    case kDiminishedSeventh:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kMinorSeventh;
          break;
        case kPerfectUnison:
          resultIntervalKind = kDiminishedSeventh;
          break;
        case kAugmentedUnison:
     // JMI     resultIntervalKind = kDiminishedOctave;
          break;
        case kDiminishedSecond:
          resultIntervalKind = kMajorSixth;
          break;
        case kMinorSecond:
          resultIntervalKind = kMinorSixth;
          break;
        case kMajorSecond:
          resultIntervalKind = kDiminishedSixth;
          break;
        case kAugmentedSecond:
     // JMI     resultIntervalKind = kDiminishedSeventh;
          break;
        case kDiminishedThird:
          resultIntervalKind = kPerfectFifth;
          break;
        case kMinorThird:
          resultIntervalKind = kDiminishedFifth;
          break;
        case kMajorThird:
     //     resultIntervalKind = kMinorThird;
          break;
        case kAugmentedThird:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFourth:
          resultIntervalKind = kPerfectFourth;
          break;
        case kPerfectFourth:
          resultIntervalKind = kDiminishedFourth;
          break;
        case kAugmentedFourth:
     // JMI     resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFifth:
          resultIntervalKind = kMajorThird;
          break;
        case kPerfectFifth:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedFifth:
          resultIntervalKind = kDiminishedThird;
          break;
        case kDiminishedSixth:
    // JMI      resultIntervalKind = kPerfectUnison;
          break;
        case kMinorSixth:
   // JMI       resultIntervalKind = kPerfectUnison;
          break;
        case kMajorSixth:
          resultIntervalKind = kAugmentedSecond;
          break;
        case kAugmentedSixth:
          resultIntervalKind = kMajorSecond;
          break;
        case kDiminishedSeventh:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kMinorSeventh:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kMajorSeventh;
          break;
        case kPerfectUnison:
          resultIntervalKind = kMinorSeventh;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedSeventh;
          break;
        case kDiminishedSecond:
     // JMI     resultIntervalKind = kPerfectUnison;
          break;
        case kMinorSecond:
          resultIntervalKind = kMajorSixth;
          break;
        case kMajorSecond:
          resultIntervalKind = kMinorSixth;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedSixth;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedFifth;
          break;
        case kMinorThird:
          resultIntervalKind = kPerfectFifth;
          break;
        case kMajorThird:
          resultIntervalKind = kDiminishedFifth;
          break;
        case kAugmentedThird:
  // JMI        resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFourth:
          resultIntervalKind = kAugmentedFourth;
          break;
        case kPerfectFourth:
          resultIntervalKind = kPerfectFourth;
          break;
        case kAugmentedFourth:
          resultIntervalKind = kDiminishedFourth;
          break;
        case kDiminishedFifth:
          resultIntervalKind = kMajorThird;
          break;
        case kPerfectFifth:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedFifth:
          resultIntervalKind = kDiminishedThird;
          break;
        case kDiminishedSixth:
          resultIntervalKind = kAugmentedSecond;
          break;
        case kMinorSixth:
          resultIntervalKind = kMajorSecond;
          break;
        case kMajorSixth:
          resultIntervalKind = kMinorSecond;
          break;
        case kAugmentedSixth:
          resultIntervalKind = kDiminishedSecond;
          break;
        case kDiminishedSeventh:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kMinorSeventh:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kMajorSeventh:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedSeventh;
          break;
        case kPerfectUnison:
          resultIntervalKind = kMajorSeventh;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kMinorSeventh;
          break;
        case kDiminishedSecond:
          resultIntervalKind = kDiminishedSeventh;
          break;
        case kMinorSecond:
          resultIntervalKind = kAugmentedSixth;
          break;
        case kMajorSecond:
          resultIntervalKind = kMajorSixth;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kMinorSixth;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedSixth;
          break;
        case kMinorThird:
          resultIntervalKind = kAugmentedFifth;
          break;
        case kMajorThird:
          resultIntervalKind = kPerfectFifth;
          break;
        case kAugmentedThird:
          resultIntervalKind = kDiminishedFifth;
          break;
        case kDiminishedFourth:
  // JMI        resultIntervalKind = kMinorThird;
          break;
        case kPerfectFourth:
          resultIntervalKind = kAugmentedFourth;
          break;
        case kAugmentedFourth:
          resultIntervalKind = kPerfectFourth;
          break;
        case kDiminishedFifth:
          resultIntervalKind = kAugmentedThird;
          break;
        case kPerfectFifth:
          resultIntervalKind = kMajorThird;
          break;
        case kAugmentedFifth:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedSixth:
 // JMI         resultIntervalKind = kPerfectUnison;
          break;
        case kMinorSixth:
          resultIntervalKind = kAugmentedSecond;
          break;
        case kMajorSixth:
          resultIntervalKind = kMajorSecond;
          break;
        case kAugmentedSixth:
          resultIntervalKind = kMinorSecond;
          break;
        case kDiminishedSeventh:
          resultIntervalKind = kDiminishedSecond;
          break;
        case kMinorSeventh:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kMajorSeventh:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kAugmentedSeventh:
      switch (intervalKind2) {
        case kDiminishedUnisson:
 // JMI         resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
    // JMI      resultIntervalKind = kAugmentedSeventh;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kAugmentedSeventh;
          break;
        case kDiminishedSecond:
    // JMI      resultIntervalKind = kPerfectUnison;
          break;
        case kMinorSecond:
     // JMI    resultIntervalKind = kMajorSeventh;
          break;
        case kMajorSecond:
          resultIntervalKind = kAugmentedSixth;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kMajorSixth;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedSixth;
          break;
        case kMinorThird:
      // JMI    resultIntervalKind = kMinorThird;
          break;
        case kMajorThird:
          resultIntervalKind = kAugmentedFifth;
          break;
        case kAugmentedThird:
          resultIntervalKind = kPerfectFifth;
          break;
        case kDiminishedFourth:
    // JMI      resultIntervalKind = kDiminishedFifth;
          break;
        case kPerfectFourth:
     // JMI     resultIntervalKind = kAugmentedFourth;
          break;
        case kAugmentedFourth:
          resultIntervalKind = kAugmentedFourth;
          break;
        case kDiminishedFifth:
  // JMI        resultIntervalKind = kMinorThird;
          break;
        case kPerfectFifth:
          resultIntervalKind = kAugmentedFourth;
          break;
        case kAugmentedFifth:
          resultIntervalKind = kPerfectFourth;
          break;
        case kDiminishedSixth:
  // JMI        resultIntervalKind = kPerfectUnison;
          break;
        case kMinorSixth:
 // JMI         resultIntervalKind = kPerfectUnison;
          break;
        case kMajorSixth:
          resultIntervalKind = kAugmentedSecond;
          break;
        case kAugmentedSixth:
          resultIntervalKind = kMajorSecond;
          break;
        case kDiminishedSeventh:
   // JMI       resultIntervalKind = kPerfectUnison;
          break;
        case kMinorSeventh:
    // JMI      resultIntervalKind = kPerfectUnison;
          break;
        case kMajorSeventh:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kAugmentedSeventh:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;

    default:
      ;
  } // switch

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceExtraChords) {
    gLogIOstream <<
      "--> base resultIntervalKind = '" <<
      msrIntervalKindAsString (resultIntervalKind) <<
      "'" <<
      endl;
  }
#endif
  
  // take interval inversion into account if relevant
  if (invertInterval) {
    resultIntervalKind =
      invertIntervalKind (resultIntervalKind);
  }
  
  // take relative octabes permutation into account if relevant
  if (permuteRelativeOctaves) {
    resultIntervalKind =
      invertIntervalKind (resultIntervalKind);
  }

  // create the result
  S_msrChordInterval
    result =
      msrChordInterval::create (
        resultIntervalKind,
        resultRelativeOctave);

  // denormalize it, in order to get intervals
  // greater than an augmented seventh if applicable
  result->deNormalizeInterval ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceExtraChords) {
    gLogIOstream <<
      "--> result = '" <<
      result->asShortString () <<
      "'" <<
      endl <<
      endl;
  }
#endif
  
  // return it;
  return result;
}

S_msrChordInterval msrChordInterval::intervalSum (
  S_msrChordInterval otherChordInterval)
{
  msrIntervalKind resultIntervalKind   = k_NoIntervalKind;
  int             resultRelativeOctave = 0;

  msrIntervalKind
    intervalKind1 = fChordIntervalIntervalKind,
    intervalKind2 = otherChordInterval->fChordIntervalIntervalKind;
    
  int
    relativeOctave1 = fChordIntervalRelativeOctave,
    relativeOctave2 = otherChordInterval->fChordIntervalRelativeOctave;

  relativeOctave1 = relativeOctave2; // TEMP, JMI
  relativeOctave2 = relativeOctave1; // TEMP, JMI
  
  // order the intervals so that
  // intervalKind1 is greater or equal to intervalKind2
  // according to the enum type
  bool invertInterval = false;
  
  if (intervalKind1 < intervalKind2) {
    intervalKind1 = intervalKind2;
    intervalKind2 = intervalKind1;
    invertInterval = true;
  }
    
  switch (intervalKind1) {
    case k_NoIntervalKind:
      break;
      
    case kDiminishedUnisson:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kPerfectUnison:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kAugmentedUnison:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kDiminishedSecond:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedOctave;
          break;
        case kDiminishedSecond:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kMinorSecond:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedOctave;
          break;
        case kMinorSecond:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kMajorSecond:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedOctave;
          break;
        case kMinorSecond:
          resultIntervalKind = kMajorSeventh;
          break;
        case kMajorSecond:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kAugmentedSecond:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedOctave;
          break;
        case kMinorSecond:
          resultIntervalKind = kMajorSeventh;
          break;
        case kMajorSecond:
          resultIntervalKind = kMinorSeventh;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kDiminishedThird:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedOctave;
          break;
        case kMinorSecond:
          resultIntervalKind = kMajorSeventh;
          break;
        case kMajorSecond:
          resultIntervalKind = kMinorSeventh;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedSeventh;
          break;
        case kDiminishedThird:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kMinorThird:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedOctave;
          break;
        case kMinorSecond:
          resultIntervalKind = kMajorSeventh;
          break;
        case kMajorSecond:
          resultIntervalKind = kMinorSeventh;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedSeventh;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedSixth;
          break;
        case kMinorThird:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kMajorThird:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedOctave;
          break;
        case kMinorSecond:
          resultIntervalKind = kMajorSeventh;
          break;
        case kMajorSecond:
          resultIntervalKind = kMinorSeventh;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedSeventh;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedSixth;
          break;
        case kMinorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kMajorThird:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kAugmentedThird:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedOctave;
          break;
        case kMinorSecond:
          resultIntervalKind = kMajorSeventh;
          break;
        case kMajorSecond:
          resultIntervalKind = kMinorSeventh;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedSeventh;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedSixth;
          break;
        case kMinorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kMajorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedThird:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kDiminishedFourth:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedOctave;
          break;
        case kMinorSecond:
          resultIntervalKind = kMajorSeventh;
          break;
        case kMajorSecond:
          resultIntervalKind = kMinorSeventh;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedSeventh;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedSixth;
          break;
        case kMinorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kMajorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedThird:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFourth:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kPerfectFourth:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedOctave;
          break;
        case kMinorSecond:
          resultIntervalKind = kMajorSeventh;
          break;
        case kMajorSecond:
          resultIntervalKind = kMinorSeventh;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedSeventh;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedSixth;
          break;
        case kMinorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kMajorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedThird:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFourth:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kAugmentedFourth:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedOctave;
          break;
        case kMinorSecond:
          resultIntervalKind = kMajorSeventh;
          break;
        case kMajorSecond:
          resultIntervalKind = kMinorSeventh;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedSeventh;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedSixth;
          break;
        case kMinorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kMajorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedThird:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedFourth:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kDiminishedFifth:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedOctave;
          break;
        case kMinorSecond:
          resultIntervalKind = kMajorSeventh;
          break;
        case kMajorSecond:
          resultIntervalKind = kMinorSeventh;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedSeventh;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedSixth;
          break;
        case kMinorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kMajorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedThird:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFifth:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kPerfectFifth:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedOctave;
          break;
        case kMinorSecond:
          resultIntervalKind = kMajorSeventh;
          break;
        case kMajorSecond:
          resultIntervalKind = kMinorSeventh;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedSeventh;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedSixth;
          break;
        case kMinorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kMajorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedThird:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFifth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFifth:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kAugmentedFifth:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedOctave;
          break;
        case kMinorSecond:
          resultIntervalKind = kMajorSeventh;
          break;
        case kMajorSecond:
          resultIntervalKind = kMinorSeventh;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedSeventh;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedSixth;
          break;
        case kMinorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kMajorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedThird:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFifth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFifth:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedFifth:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;

    case kDiminishedSixth:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedOctave;
          break;
        case kMinorSecond:
          resultIntervalKind = kMajorSeventh;
          break;
        case kMajorSecond:
          resultIntervalKind = kMinorSeventh;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedSeventh;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedSixth;
          break;
        case kMinorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kMajorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedThird:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFifth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFifth:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedFifth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kDiminishedSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kMinorSixth:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedOctave;
          break;
        case kMinorSecond:
          resultIntervalKind = kMajorSeventh;
          break;
        case kMajorSecond:
          resultIntervalKind = kMinorSeventh;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedSeventh;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedSixth;
          break;
        case kMinorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kMajorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedThird:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFifth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFifth:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedFifth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kDiminishedSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kMinorSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kMajorSixth:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedOctave;
          break;
        case kMinorSecond:
          resultIntervalKind = kMajorSeventh;
          break;
        case kMajorSecond:
          resultIntervalKind = kMinorSeventh;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedSeventh;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedSixth;
          break;
        case kMinorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kMajorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedThird:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFifth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFifth:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedFifth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kDiminishedSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kMinorSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kMajorSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kAugmentedSixth:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedOctave;
          break;
        case kMinorSecond:
          resultIntervalKind = kMajorSeventh;
          break;
        case kMajorSecond:
          resultIntervalKind = kMinorSeventh;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedSeventh;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedSixth;
          break;
        case kMinorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kMajorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedThird:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFifth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFifth:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedFifth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kDiminishedSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kMinorSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kMajorSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kDiminishedSeventh:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedOctave;
          break;
        case kMinorSecond:
          resultIntervalKind = kMajorSeventh;
          break;
        case kMajorSecond:
          resultIntervalKind = kMinorSeventh;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedSeventh;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedSixth;
          break;
        case kMinorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kMajorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedThird:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFifth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFifth:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedFifth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kDiminishedSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kMinorSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kMajorSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kDiminishedSeventh:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kMinorSeventh:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedOctave;
          break;
        case kMinorSecond:
          resultIntervalKind = kMajorSeventh;
          break;
        case kMajorSecond:
          resultIntervalKind = kMinorSeventh;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedSeventh;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedSixth;
          break;
        case kMinorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kMajorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedThird:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFifth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFifth:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedFifth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kDiminishedSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kMinorSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kMajorSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kDiminishedSeventh:
          resultIntervalKind = kPerfectUnison;
          break;
        case kMinorSeventh:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kMajorSeventh:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedOctave;
          break;
        case kMinorSecond:
          resultIntervalKind = kMajorSeventh;
          break;
        case kMajorSecond:
          resultIntervalKind = kMinorSeventh;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedSeventh;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedSixth;
          break;
        case kMinorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kMajorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedThird:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFifth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFifth:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedFifth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kDiminishedSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kMinorSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kMajorSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kDiminishedSeventh:
          resultIntervalKind = kPerfectUnison;
          break;
        case kMinorSeventh:
          resultIntervalKind = kPerfectUnison;
          break;
        case kMajorSeventh:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;
      
    case kAugmentedSeventh:
      switch (intervalKind2) {
        case kDiminishedUnisson:
          resultIntervalKind = kAugmentedUnison;
          break;
        case kPerfectUnison:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedUnison:
          resultIntervalKind = kDiminishedOctave;
          break;
        case kMinorSecond:
          resultIntervalKind = kMajorSeventh;
          break;
        case kMajorSecond:
          resultIntervalKind = kMinorSeventh;
          break;
        case kAugmentedSecond:
          resultIntervalKind = kDiminishedSeventh;
          break;
        case kDiminishedThird:
          resultIntervalKind = kAugmentedSixth;
          break;
        case kMinorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kMajorThird:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedThird:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedFourth:
          resultIntervalKind = kMinorThird;
          break;
        case kDiminishedFifth:
          resultIntervalKind = kMinorThird;
          break;
        case kPerfectFifth:
          resultIntervalKind = kMinorThird;
          break;
        case kAugmentedFifth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kDiminishedSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kMinorSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kMajorSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedSixth:
          resultIntervalKind = kPerfectUnison;
          break;
        case kDiminishedSeventh:
          resultIntervalKind = kPerfectUnison;
          break;
        case kMinorSeventh:
          resultIntervalKind = kPerfectUnison;
          break;
        case kMajorSeventh:
          resultIntervalKind = kPerfectUnison;
          break;
        case kAugmentedSeventh:
          resultIntervalKind = kPerfectUnison;
          break;
        default:
          ;
      } // switch
      break;

    default:
      ;
  } // switch
  

  if (invertInterval) {
    resultIntervalKind =
      invertIntervalKind (resultIntervalKind);
  }
  
  return
    msrChordInterval::create (
      resultIntervalKind,
      resultRelativeOctave);
}

/* JMI
void msrChordInterval::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrChordInterval::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrChordInterval>*
    p =
      dynamic_cast<visitor<S_msrChordInterval>*> (v)) {
        S_msrChordInterval elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrChordInterval::visitStart ()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrChordInterval::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrChordInterval::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrChordInterval>*
    p =
      dynamic_cast<visitor<S_msrChordInterval>*> (v)) {
        S_msrChordInterval elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrChordInterval::visitEnd ()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrChordInterval::browseData (basevisitor* v)
{}
*/

ostream& operator<< (ostream& os, const S_msrChordInterval& elt)
{
  elt->print (os);
  return os;
}

string msrChordInterval::asString ()
{
  stringstream s;

  s <<
    "ChordInterval '" <<
    msrIntervalKindAsString (fChordIntervalIntervalKind) <<
    ", rel. oct. " <<
    fChordIntervalRelativeOctave <<
    "'";
  
  return s.str ();
}

string msrChordInterval::asShortString ()
{
  stringstream s;

  s <<
    "'" <<
    msrIntervalKindAsString (fChordIntervalIntervalKind) <<
    ", rel.oct. " <<
    fChordIntervalRelativeOctave <<
    "'";
  
  return s.str ();
}

void msrChordInterval::print (ostream& os)
{  
  os <<
    "ChordInterval" <<
    endl;

  gIndenter++;

  const int fieldWidth = 22;
  
  os << left <<
  /* JMI
    setw (fieldWidth) <<
    "chordIntervalNumber" << " : " << fChordIntervalNumber <<
    endl <<
    */
    setw (fieldWidth) <<
    "chordIntervalIntervalKind" << " : " <<
      msrIntervalKindAsString (fChordIntervalIntervalKind) <<
    endl <<
    setw (fieldWidth) <<
    "chordIntervalRelativeOctave" << " : " << fChordIntervalRelativeOctave <<
  /* JMI
    ", line: " << fInputLineNumber <<
    */
    endl;

  gIndenter--;
}

//______________________________________________________________________________
map<msrHarmonyKind, S_msrChordStructure>
  gChordStructuresMap;

S_msrChordStructure msrChordStructure::createBare (
  msrHarmonyKind chordStructureHarmonyKind)
{
  msrChordStructure* o =
    new msrChordStructure (
      chordStructureHarmonyKind);
  assert(o!=0);

  return o;
}

S_msrChordStructure msrChordStructure::create (
  msrHarmonyKind chordStructureHarmonyKind)
{
  S_msrChordStructure o =
    createBare (
      chordStructureHarmonyKind);

  o->
    populateChordStructure ();
    
  return o;
}

msrChordStructure::msrChordStructure (
  msrHarmonyKind chordStructureHarmonyKind)
{
  fChordStructureHarmonyKind = chordStructureHarmonyKind;

  if (TRACE_MSR_BASIC_TYPES) {
    gLogIOstream <<
      "==> Creating chord intervals '" <<
      chordStructureAsString () <<
      "'" <<
      endl;
  }
}

S_msrChordStructure msrChordStructure::createChordStructureNewbornClone ()
{
  S_msrChordStructure
    newbornClone =
      createBare (
        fChordStructureHarmonyKind);
  
  return newbornClone;
}

void msrChordStructure::populateChordStructure ()
{
  // append chord items to chord intervals
  switch (fChordStructureHarmonyKind) {
    case k_NoHarmony:
      break;

    // MusicXML chords

    case kMajorHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
      }
      break;
    
    case kMinorHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
      }
      break;
    
    case kAugmentedHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kAugmentedFifth)
          );
      }
      break;
    
    case kDiminishedHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kDiminishedFifth)
          );
      }
      break;
    
    case kDominantHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorSeventh)
          );
      }
      break;
    
    case kMajorSeventhHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorSeventh)
          );
      }
      break;
    
     case kMinorSeventhHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorSeventh)
          );
      }
      break;
    
    case kDiminishedSeventhHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kDiminishedFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kDiminishedSeventh)
          );
      }
      break;
    
    case kAugmentedSeventhHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kAugmentedFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorSeventh)
          );
      }
      break;
    
    case kHalfDiminishedHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kDiminishedFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorSeventh)
          );
      }
      break;
    
     case kMinorMajorSeventhHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorSeventh)
          );
      }
      break;
    
    case kMajorSixthHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorSixth)
          );
      }
      break;
    
    case kMinorSixthHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorSixth)
          );
      }
      break;
    
    case kDominantNinthHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorSeventh)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorNinth)
          );
      }
      break;

    case kMajorNinthHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorSeventh)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorNinth)
          );
      }
      break;
    
    case kMinorNinthHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorSeventh)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorNinth)
          );
      }
      break;
    
    case kDominantEleventhHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorSeventh)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectEleventh)
          );
      }
      break;

    case kMajorEleventhHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorSeventh)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectEleventh)
          );
      }
      break;
    
    case kMinorEleventhHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorSeventh)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectEleventh)
          );
      }
      break;
    
    case kDominantThirteenthHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorSeventh)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorThirteenth)
          );
      }
      break;

    case kMajorThirteenthHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorSeventh)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorThirteenth)
          );
      }
      break;
    
    case kMinorThirteenthHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorSeventh)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorThirteenth)
          );
      }
      break;
    
    case kSuspendedSecondHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorSecond)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
      }
      break;
    
    case kSuspendedFourthHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFourth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
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
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kDiminishedSixth)
          );
      }
      break;
    
    case kItalianHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kAugmentedSixth)
          );
      }
      break;
    
    case kFrenchHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kAugmentedFourth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kAugmentedSixth)
          );
      }
      break;
    
    case kGermanHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kAugmentedSixth)
          );
      }
      break;
    
    case kPedalHarmony:
      break;
      
    case kPowerHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
      }
      break;
    
    case kTristanHarmony:
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kAugmentedSecond)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kAugmentedFourth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kAugmentedSixth)
          );
      }
      break;
    
    // jazz-specific chords

    case kMinorMajorNinth: // -maj9, minmaj9
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorSeventh)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorNinth)
          );
      }
      break;

    case kDominantSuspendedFourthHarmony: // 7sus4, domsus4
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFourth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorSeventh)
          );
      }
      break;

    case kDominantAugmentedFifthHarmony: // 7#5, domaug5
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFourth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kAugmentedFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorSeventh)
          );
      }
      break;

    case kDominantMinorNinthHarmony: // 7b9, dommin9
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorSeventh)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorNinth)
          );
      }
      break;

    case kDominantAugmentedNinthDiminishedFifthHarmony: // 7#9b5, domaug9dim5
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kDiminishedFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorSeventh)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kAugmentedNinth)
          );
      }
      break;

    case kDominantAugmentedNinthAugmentedFifthHarmony: // 7#9#5, domaug9aug5
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorThird)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kAugmentedFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorSeventh)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kAugmentedNinth)
          );
      }
      break;

    case kDominantAugmentedEleventhHarmony: // 7#11, domaug11
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFourth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMinorSeventh)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kAugmentedEleventh)
          );
      }
      break;

    case kMajorSeventhAugmentedEleventhHarmony: // maj7#11, maj7aug11
      {
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectUnison)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFourth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kPerfectFifth)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kMajorSeventh)
          );
        appendChordIntervalToChordStructure (
          msrChordInterval::create (
            kAugmentedEleventh)
          );
      }
      break;

    // other
    
    case kOtherHarmony:
      break;
      
    case kNoneHarmony:
      break;
  } // switch

/* JMI
  // register chord intervals in map
  gChordStructuresMap [fChordStructureHarmonyKind] = this;
  */
}

msrChordStructure::~msrChordStructure ()
{}

void msrChordStructure::appendChordIntervalToChordStructure (
  S_msrChordInterval chordInterval)
{
  // set the input line number and chord item number // JMI

  // append the chord item
  fChordStructureIntervals.push_back (
    chordInterval);
}

string msrChordStructure::chordStructureAsString () const
{
  stringstream s;

  s <<
    "ChordStructure" <<
    ", " <<
    msrHarmonyKindAsString (fChordStructureHarmonyKind) <<
    ", " <<
    singularOrPlural (
      fChordStructureIntervals.size (), "item", "items");

  return s.str ();
}

/* JMI
void msrChordStructure::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrChordStructure::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrChordStructure>*
    p =
      dynamic_cast<visitor<S_msrChordStructure>*> (v)) {
        S_msrChordStructure elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrChordStructure::visitStart ()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrChordStructure::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrChordStructure::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrChordStructure>*
    p =
      dynamic_cast<visitor<S_msrChordStructure>*> (v)) {
        S_msrChordStructure elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrChordStructure::visitEnd ()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrChordStructure::browseData (basevisitor* v)
{}
*/

ostream& operator<< (ostream& os, const S_msrChordStructure& elt)
{
  elt->print (os);
  return os;
}

void msrChordStructure::print (ostream& os)
{  
  os <<
    "ChordStructure" <<
    ", harmonyKind: " <<
    msrHarmonyKindAsString (fChordStructureHarmonyKind) <<
    ", " <<
    singularOrPlural (
      fChordStructureIntervals.size (), "interval", "intervals") <<
  /* JMI
    ", line: " << fInputLineNumber <<
    */
    endl;

  gIndenter++;
    
  if (fChordStructureIntervals.size ()) {
    vector<S_msrChordInterval>::const_reverse_iterator
      iBegin = fChordStructureIntervals.crbegin (),
      iEnd   = fChordStructureIntervals.crend (),
      i      = iBegin;
    
    for ( ; ; ) {
      S_msrChordInterval
        chordInterval = (*i);

      gLogIOstream <<
        chordInterval->chordIntervalAsShortString () <<
        endl;

      if (++i == iEnd) break;
    } // for
  }
  else {
    gLogIOstream <<
      "no intervals" <<
      endl;
  }

  gIndenter--;
}

S_msrChordInterval msrChordStructure::bassChordIntervalForChordInversion (
  int inputLineNumber,
  int inversionNumber)
{
  /*
    Inversion is a number indicating which inversion is used:
    0 for root position, 1 for first inversion, etc.
  */

  S_msrChordInterval result;

  if (
    inversionNumber < 0
      ||
    inversionNumber > int (fChordStructureIntervals.size ()) - 1 ) {
    stringstream s;

    s <<
      "Sorry, inversion number '" <<
      inversionNumber <<
      "' does not exist for chord intevals '" <<
      msrHarmonyKindAsString (fChordStructureHarmonyKind) <<
      "', line " << inputLineNumber;

    msrLimitation (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return fChordStructureIntervals [inversionNumber];
}

S_msrChordStructure msrChordStructure::invertChordStructure (int inversion)
{
  if (inversion == 0) {
    return this;
  }
    
  // create an empty object
  S_msrChordStructure
    result =
      this->
        createChordStructureNewbornClone ();

  unsigned int
    chordStructureIntervalsSize =
      fChordStructureIntervals.size ();

  if (TRACE_MSR_BASIC_TYPES) {
    gLogIOstream <<
      "==> invertChordStructure (), inversion = " << inversion <<
      ", original chordStructureIntervalsSize = " << chordStructureIntervalsSize <<
      endl;
  }
  
  if (chordStructureIntervalsSize) {
    // add the first items
    for (unsigned int i = inversion; i < chordStructureIntervalsSize; i++) {
      S_msrChordInterval
        chordIntervalClone =
          fChordStructureIntervals [i]->
            createChordIntervalNewbornClone ();

      if (TRACE_MSR_BASIC_TYPES) {
        gLogIOstream <<
          "--> adding first item to result:" <<
          endl;
        gIndenter++;
        gLogIOstream <<
          chordIntervalClone <<
          endl;
        gIndenter--;
      }

      result->
        appendChordIntervalToChordStructure (
          chordIntervalClone);

      if (TRACE_MSR_BASIC_TYPES) {
        gLogIOstream <<
          "==> result chord structure after adding first item :" <<
          endl;
  
        gIndenter++;
        gLogIOstream <<
          result <<
          endl;
        gIndenter--;
      }
    } // while
    
    // add  the octaviate last items
    for (int i = 0; i < inversion; i++) {
      S_msrChordInterval
        chordIntervalClone =
          fChordStructureIntervals [i]->
            createChordIntervalNewbornClone ();

      chordIntervalClone->
        incrementChordIntervalRelativeOctave ();
        
      if (TRACE_MSR_BASIC_TYPES) {
        gLogIOstream <<
          "--> adding last item to resultlast item :" <<
          endl;
        gIndenter++;
        gLogIOstream <<
          chordIntervalClone <<
          endl;
        gIndenter--;
      }
        
      result->
        appendChordIntervalToChordStructure (
          chordIntervalClone);

      if (TRACE_MSR_BASIC_TYPES) {
        gLogIOstream <<
          "==> result chord structure after  after adding last item:" <<
          endl;
  
        gIndenter++;
        gLogIOstream <<
          result <<
          endl;
        gIndenter--;
      }
    } // for
  }

  return result;
}

void msrChordStructure::printAllChordsStructures (ostream& os)
{
  os <<
    "All the known chords structures are:" <<
    endl <<
    endl;

  gIndenter++;
  
  for (
    msrHarmonyKind harmonyKind = kMajorHarmony;
    harmonyKind <= kMajorSeventhAugmentedEleventhHarmony;
    harmonyKind = msrHarmonyKind (harmonyKind + 1)) {
    // create the chord intervals
    S_msrChordStructure
      chordStructure =
        msrChordStructure::create (
          harmonyKind);

    // print it
    os <<
      chordStructure <<
      endl;
  } // for

  gIndenter--;
}

list<msrSemiTonesPitchKind> buildSemiTonesChord (
  msrHarmonyKind        harmonyKind,
  msrSemiTonesPitchKind rootNote)
{
  list<msrSemiTonesPitchKind> result;

  // create the chord intervals
  S_msrChordStructure
    chordStructure =
      msrChordStructure::create (
        harmonyKind);

  // add the root to the chord
  result.push_back (rootNote);

  // add the other notes to the chord
  const vector<S_msrChordInterval>&
    chordStructureIntervals =
      chordStructure->
        getChordStructureIntervals ();

  for (unsigned int i = 1; i << chordStructureIntervals.size (); i++) {
    result.push_back (rootNote);
  } // for

  return result;
}

//______________________________________________________________________________
S_msrChordPitch msrChordPitch::create (
//      int             inputLineNumber,
// JMI      int             chordPitchNumber,
  msrSemiTonesPitchKind chordPitchSemitonePitchKind,
  int                   chordPitchRelativeOctave)
{
  msrChordPitch* o =
    new msrChordPitch (
 //     inputLineNumber,
 //     chordPitchNumber,
      chordPitchSemitonePitchKind,
      chordPitchRelativeOctave);
  assert(o!=0);

  return o;
}

msrChordPitch::msrChordPitch (
//      int             inputLineNumber,
// JMI      int             chordPitchNumber,
  msrSemiTonesPitchKind chordPitchSemitonePitchKind,
  int                   chordPitchRelativeOctave)
  // JMI  : msrElement (inputLineNumber)
{
//  fChordSemitonePitchNumber       = chordPitchNumber;
  
  fChordPitchSemitonePitchKind = chordPitchSemitonePitchKind;

  fChordPitchRelativeOctave = chordPitchRelativeOctave;

  if (TRACE_MSR_BASIC_TYPES) {
    gLogIOstream <<
      "==> Creating chord item '" <<
      chordPitchAsString () <<
      "'" <<
      endl;
  }
}

msrChordPitch::~msrChordPitch ()
{}

S_msrChordPitch msrChordPitch::createChordPitchNewbornClone ()
{
  S_msrChordPitch
    newbornClone =
      msrChordPitch::create (
   //      0, // JMI fInputLineNumber
  //      fChordPitchNumber,
        fChordPitchSemitonePitchKind,
        fChordPitchRelativeOctave);
  
  return newbornClone;
}

string msrChordPitch::chordPitchAsString () const
{
  stringstream s;

  const int fieldWidth = 19;
  
  s << left <<
    "ChordPitch" <<
 //   " " << fChordPitchNumber <<
    ": " <<
    setw (fieldWidth) <<
    msrSemiTonesPitchKindAsString (fChordPitchSemitonePitchKind) <<
    ", chordPitchRelativeOctave: " << fChordPitchRelativeOctave;

  return s.str ();
}

/* JMI
void msrChordPitch::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrChordPitch::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrChordPitch>*
    p =
      dynamic_cast<visitor<S_msrChordPitch>*> (v)) {
        S_msrChordPitch elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrChordPitch::visitStart ()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrChordPitch::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrChordPitch::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrChordPitch>*
    p =
      dynamic_cast<visitor<S_msrChordPitch>*> (v)) {
        S_msrChordPitch elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrChordPitch::visitEnd ()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrChordPitch::browseData (basevisitor* v)
{}
*/

ostream& operator<< (ostream& os, const S_msrChordPitch& elt)
{
  elt->print (os);
  return os;
}

void msrChordPitch::print (ostream& os)
{  
  os <<
    "ChordPitch" <<
    endl;

  gIndenter++;

  const int fieldWidth = 22;
  
  os << left <<
  /* JMI
    setw (fieldWidth) <<
    "chordPitchNumber" << " : " << fChordPitchNumber <<
    endl <<
    */
    setw (fieldWidth) <<
    "chordPitchSemitonePitchKind" << " : " <<
      msrSemiTonesPitchKindAsString (fChordPitchSemitonePitchKind) <<
    endl <<
    setw (fieldWidth) <<
    "chordPitchRelativeOctave" << " : " << fChordPitchRelativeOctave <<
  /* JMI
    ", line: " << fInputLineNumber <<
    */
    endl;

  gIndenter--;
}

//______________________________________________________________________________
S_msrChordContents msrChordContents::create (
// JMI  int                   inputLineNumber,
  msrSemiTonesPitchKind chordContentsRootNote,
  msrHarmonyKind        chordContentsHarmonyKind)
{
  msrChordContents* o =
    new msrChordContents (
 //     inputLineNumber,
      chordContentsRootNote,
      chordContentsHarmonyKind);
  assert(o!=0);

  return o;
}

msrChordContents::msrChordContents (
// JMI  int                   inputLineNumber,
  msrSemiTonesPitchKind chordContentsRootNote,
  msrHarmonyKind        chordContentsHarmonyKind)
{
  fChordContentsRootNote    = chordContentsRootNote;
  fChordContentsHarmonyKind = chordContentsHarmonyKind;

  if (TRACE_MSR_BASIC_TYPES) {
    gLogIOstream <<
      "==> Creating chordContents '" <<
      chordContentsAsString () <<
      "'" <<
      endl;
  }

  // create the root chord pitch
  S_msrChordPitch
    rootChordPitch =
      msrChordPitch::create (
        fChordContentsRootNote,
        0); // relative octave
        
  // add it to the chord pitches
  fChordContentsChordPitches.push_back (rootChordPitch);

  // add the other notes to the chord pitches
  S_msrChordStructure
    chordStructure =
      msrChordStructure::create (
        fChordContentsHarmonyKind);
      
  const vector<S_msrChordInterval>&
    chordIntervals =
      chordStructure->
        getChordStructureIntervals ();

  for (unsigned int i = 1; i < chordIntervals.size (); i++) {
    // get the interval
    msrIntervalKind
      intervalKind =
        chordIntervals [i]->
          getChordIntervalIntervalKind ();

    // fetch the semitone pitch
    msrSemiTonesPitchKind
      semiTonePitch =
        noteAtIntervalFromSemiTonesPitch (
          0, // ??? JM                   inputLineNumber,
          intervalKind,
          fChordContentsRootNote);
    
    // create the chord pitch
    S_msrChordPitch
      chordPitch =
        msrChordPitch::create (
          semiTonePitch,
          0); // relative octave
          
    // add it to the chord pitches
    fChordContentsChordPitches.push_back (chordPitch);
  } // for
}

msrChordContents::~msrChordContents ()
{}

string msrChordContents::chordContentsAsString () const
{
  stringstream s;

  s <<
    "ChordContents" <<
    ", " <<
    msrHarmonyKindAsString (fChordContentsHarmonyKind) <<
    ", " <<
    singularOrPlural (
      fChordContentsChordPitches.size (), "chord pitch", "chord pitches");

  return s.str ();
}

msrSemiTonesPitchKind msrChordContents::bassSemiTonesPitchKindForChordInversion (
  int inputLineNumber,
  int inversionNumber)
{
  /*
    Inversion is a number indicating which inversion is used:
    0 for root position, 1 for first inversion, etc.
  */

  if (
    inversionNumber < 0
      ||
    inversionNumber > int (fChordContentsChordPitches.size ()) - 1 ) {
    stringstream s;

    s <<
      "Sorry, inversion number '" <<
      inversionNumber <<
      "' does not exist for chord notes '" <<
      msrHarmonyKindAsString (fChordContentsHarmonyKind) <<
      "', line " << inputLineNumber;

    msrLimitation (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return
    fChordContentsChordPitches [inversionNumber]->
      getChordPitchSemitonePitchKind ();
}

void msrChordContents::printAllChordsContents (
  ostream&              os,
  msrSemiTonesPitchKind rootSemiTonesPitchKind)
{
  // fetch the root quartertones pitch kind
  msrQuarterTonesPitchKind
    rootQuarterTonesPitchKind =
      quarterTonesPitchKindFromSemiTonesPitchKind (
        rootSemiTonesPitchKind);
        
  os <<
    "All the known chords contents with diatonic root '" <<
    msrQuarterTonesPitchKindAsString (
      gLpsrOptions->
        fLpsrQuarterTonesPitchesLanguageKind,
      rootQuarterTonesPitchKind) <<
      /* JMI
    "' (" <<
    msrSemiTonesPitchKindAsString (
      rootSemiTonesPitchKind) <<
    ")" <<
    */
    " in language '" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      gLpsrOptions->
        fLpsrQuarterTonesPitchesLanguageKind) <<
    "' 'are:" <<
    endl <<
    endl;

  gIndenter++;
  
  for (
    msrHarmonyKind harmonyKind = kMajorHarmony;
    harmonyKind <= kMajorSeventhAugmentedEleventhHarmony;
    harmonyKind = msrHarmonyKind (harmonyKind + 1)
  ) {
    os <<
      msrHarmonyKindAsString (harmonyKind) <<
      ":" <<
      endl;

    gIndenter++;

    // create the chord intervals
    S_msrChordStructure
      chordStructure =
        msrChordStructure::create (
          harmonyKind);

    // fetch the intervals items for these intervals
    // with rootSemiTonesPitchKind as root
    const vector <S_msrChordInterval>&
      chordStructureIntervals =
        chordStructure->
          getChordStructureIntervals ();

    if (chordStructureIntervals.size ()) {
      // fetch the notes for these intervals
      vector<S_msrChordInterval>::const_reverse_iterator
        iBegin = chordStructureIntervals.crbegin (),
        iEnd   = chordStructureIntervals.crend (),
        i      = iBegin;
  
      for ( ; ; ) {
        S_msrChordInterval
          chordInterval = (*i);
  
        msrIntervalKind
          intervalKind =
            chordInterval->
              getChordIntervalIntervalKind ();
  
        // fetch the semitones pitch kind
        msrSemiTonesPitchKind
          noteSemiTonesPitchKind =
            noteAtIntervalFromSemiTonesPitch (
              NO_INPUT_LINE_NUMBER,
              intervalKind,
              rootSemiTonesPitchKind);

        // fetch the quartertones pitch kind
        msrQuarterTonesPitchKind
          noteQuarterTonesPitchKind =
            quarterTonesPitchKindFromSemiTonesPitchKind (
              noteSemiTonesPitchKind);

        // print it
        const int fieldWidth2 = 8;
        
        os << left <<
          setw (fieldWidth2) <<
          msrQuarterTonesPitchKindAsString (
            gLpsrOptions->
              fLpsrQuarterTonesPitchesLanguageKind,
            noteQuarterTonesPitchKind) <<
          " : " <<
          msrIntervalKindAsString (intervalKind) <<
          endl;
  
        if (++i == iEnd) break;
        
        // no endl here
      } // for
    }

  os <<
    endl;
    
  gIndenter--;
  } // for

  gIndenter--;
}

/* JMI
void msrChordContents::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrChordContents::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrChordContents>*
    p =
      dynamic_cast<visitor<S_msrChordContents>*> (v)) {
        S_msrChordContents elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrChordContents::visitStart ()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrChordContents::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrChordContents::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrChordContents>*
    p =
      dynamic_cast<visitor<S_msrChordContents>*> (v)) {
        S_msrChordContents elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrChordContents::visitEnd ()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrChordContents::browseData (basevisitor* v)
{}
*/

ostream& operator<< (ostream& os, const S_msrChordContents& elt)
{
  elt->print (os);
  return os;
}

void msrChordContents::print (ostream& os)
{  
  os <<
    "ChordContents" <<
  /* JMI
    ", line: " << fInputLineNumber <<
    */
    endl;

  gIndenter++;

  const int fieldWidth = 17;
  
  os << left <<
    setw (fieldWidth) <<
    "chordContentsRootNote" << " : " <<
    msrSemiTonesPitchKindAsString (fChordContentsRootNote) <<
    endl <<
    setw (fieldWidth) <<
    "chordContentsHarmonyKind" << " : " <<
    msrHarmonyKindAsString (fChordContentsHarmonyKind) <<
    endl;

  if (fChordContentsChordPitches.size ()) {
    os <<
    singularOrPlural (
      fChordContentsChordPitches.size (), "chord pitch", "chord pitches") <<
    ":" <<
    endl;

    gIndenter++;

    for (unsigned int i = 0; i < fChordContentsChordPitches.size (); i++) {
      S_msrChordPitch
        chordPitch =
          fChordContentsChordPitches [i];

      os <<
        chordPitch <<
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

//______________________________________________________________________________
void printChordDetails (
  ostream&              os,
  msrSemiTonesPitchKind rootSemiTonesPitchKind,
  msrHarmonyKind        harmonyKind)
{
  // fetch the quartertones pitch kind
  msrQuarterTonesPitchKind
    rootQuarterTonesPitchKind =
      quarterTonesPitchKindFromSemiTonesPitchKind (
        rootSemiTonesPitchKind);

  string
    rootQuarterTonesPitchKindAsString =
      msrQuarterTonesPitchKindAsString (
        gLpsrOptions->
          fLpsrQuarterTonesPitchesLanguageKind,
        rootQuarterTonesPitchKind);

  string
    harmonyKindShortName =
      msrHarmonyKindShortName (
        harmonyKind);
      
  // print the deails
  os <<
    "The details of chord '" <<
    rootQuarterTonesPitchKindAsString <<
    " " <<
    harmonyKindShortName <<
    "' are:" <<
    endl <<
    endl;

  gIndenter++;

  // create the chord intervals
  S_msrChordStructure
    chordStructure =
      msrChordStructure::create (
        harmonyKind);

  // fetch the intervals items for these intervals
  // with rootSemiTonesPitchKind as root
  const vector <S_msrChordInterval>&
    chordStructureIntervals =
      chordStructure->
        getChordStructureIntervals ();

  // loop on all the inversion
  int chordStructureIntervalsNumber =
    chordStructureIntervals.size ();

  if (chordStructureIntervalsNumber) {
    for (int inversion = 0; inversion < chordStructureIntervalsNumber; inversion++) {
      // invert the chord structure
      S_msrChordStructure
        invertedChordStructure =
          chordStructure->
            invertChordStructure (inversion);

      if (TRACE_MSR_BASIC_TYPES) {
        os <<
          "==> inversion = " << inversion <<
          ", initial invertedChordStructure:" <<
          endl;

        gIndenter++;
        os <<
          invertedChordStructure <<
          endl;
        gIndenter--;
      }

      // get the inverted chord structure intervals
      const vector <S_msrChordInterval>&
        invertedChordStructureIntervals =
          invertedChordStructure->
            getChordStructureIntervals ();
    
      // fetch the notes for these intervals
      /* JMI
      vector<S_msrChordInterval>::const_iterator
        iBegin = invertedChordStructureIntervals.begin (),
        iEnd   = invertedChordStructureIntervals.end (),
        i      = iBegin;
        */
      vector<S_msrChordInterval>::const_reverse_iterator
        iBegin = invertedChordStructureIntervals.crbegin (),
        iEnd   = invertedChordStructureIntervals.crend (),
        i      = iBegin;

      os <<
        "Chord '" <<
        rootQuarterTonesPitchKindAsString <<
        " " <<
        harmonyKindShortName <<
        "'";

      if (inversion == 0) {
        os <<
          " fundamental state";
      }
      else {
        os <<
          " inversion" << inversion;
      }

      os <<
        " contents, "<<
        invertedChordStructureIntervals.size () <<
        " intervals:" <<
        endl;
        
      gIndenter++;

      for ( ; ; ) {
        S_msrChordInterval
          chordInterval = (*i);
  
        msrIntervalKind
          intervalKind =
            chordInterval->
              getChordIntervalIntervalKind ();
              
        const int fieldWidth1 = 17;
        
        os << left <<
          setw (fieldWidth1) <<
          msrIntervalKindAsString (intervalKind) <<
          ": ";
  
        // fetch the semitones pitch kind
        msrSemiTonesPitchKind
          noteSemiTonesPitchKind =
            noteAtIntervalFromSemiTonesPitch (
              NO_INPUT_LINE_NUMBER,
              intervalKind,
              rootSemiTonesPitchKind);
  
        // fetch the quartertones pitch kind
        msrQuarterTonesPitchKind
          noteQuarterTonesPitchKind =
            quarterTonesPitchKindFromSemiTonesPitchKind (
              noteSemiTonesPitchKind);
  
        // print it
        const int fieldWidth2 = 8;
  
        os << left <<
          setw (fieldWidth2) <<
          msrQuarterTonesPitchKindAsString (
            gLpsrOptions->
              fLpsrQuarterTonesPitchesLanguageKind,
            noteQuarterTonesPitchKind) <<
            /* JMI
          ", octave " << relativeOctave <<
          " (" <<
          msrSemiTonesPitchKindAsString (
            noteSemiTonesPitchKind) <<
          ")" <<
          */
          endl;
  
        if (++i == iEnd) break;
        
        // no endl here
      } // for

      gIndenter--;
  
      os <<
        endl;
    } // for
  }

  gIndenter--;
}

//______________________________________________________________________________
void printChordAnalysis (
  ostream&              os,
  msrSemiTonesPitchKind rootSemiTonesPitchKind,
  msrHarmonyKind        harmonyKind,
  int                   inversion)
{
  // fetch the quartertones pitch kind
  msrQuarterTonesPitchKind
    rootQuarterTonesPitchKind =
      quarterTonesPitchKindFromSemiTonesPitchKind (
        rootSemiTonesPitchKind);

  string
    rootQuarterTonesPitchKindAsString =
      msrQuarterTonesPitchKindAsString (
        gLpsrOptions->
          fLpsrQuarterTonesPitchesLanguageKind,
        rootQuarterTonesPitchKind);

  string
    harmonyKindShortName =
      msrHarmonyKindShortName (
        harmonyKind);
      
  // print the deails
  os <<
    "The analysis of chord '" <<
    rootQuarterTonesPitchKindAsString <<
    " " <<
    harmonyKindShortName <<
    "' inversion " <<
    inversion <<
    " is:" <<
    endl <<
    endl;

  gIndenter++;

  // create the chord intervals
  S_msrChordStructure
    chordStructure =
      msrChordStructure::create (
        harmonyKind);

  // fetch the intervals items for these intervals
  // with rootSemiTonesPitchKind as root
  const vector <S_msrChordInterval>&
    chordStructureIntervals =
      chordStructure->
        getChordStructureIntervals ();

  // loop on all the inversion
  int chordStructureIntervalsNumber =
    chordStructureIntervals.size ();

  if (chordStructureIntervalsNumber) {
    if (inversion < chordStructureIntervalsNumber) {
      // invert the chord structure
      S_msrChordStructure
        invertedChordStructure =
          chordStructure->
            invertChordStructure (inversion);

      if (TRACE_MSR_BASIC_TYPES) {
        os <<
          "==> inversion = " << inversion <<
          ", initial invertedChordStructure:" <<
          endl;

        gIndenter++;
        os <<
          invertedChordStructure <<
          endl;
        gIndenter--;
      }

      // get the inverted chord structure intervals
      const vector <S_msrChordInterval>&
        invertedChordStructureIntervals =
          invertedChordStructure->
            getChordStructureIntervals ();
    
      // print the chord contents
      {
        os <<
          "Chord '" <<
          rootQuarterTonesPitchKindAsString <<
          " " <<
          harmonyKindShortName <<
          "'";
  
        if (inversion == 0) {
          os <<
            " fundamental state";
        }
        else {
          os <<
            " inversion" << inversion;
        }
  
        os <<
          " contents, "<<
          invertedChordStructureIntervals.size () <<
          " intervals:" <<
          endl;
          
        gIndenter++;
  
        vector<S_msrChordInterval>::const_reverse_iterator
          iBegin = invertedChordStructureIntervals.crbegin (),
          iEnd   = invertedChordStructureIntervals.crend (),
          i      = iBegin;
  
        for ( ; ; ) {
          S_msrChordInterval
            chordInterval = (*i);
    
          msrIntervalKind
            intervalKind =
              chordInterval->
                getChordIntervalIntervalKind ();
  
          // fetch the semitones pitch kind
          msrSemiTonesPitchKind
            noteSemiTonesPitchKind =
              noteAtIntervalFromSemiTonesPitch (
                NO_INPUT_LINE_NUMBER,
                intervalKind,
                rootSemiTonesPitchKind);
    
          // fetch the quartertones pitch kind
          msrQuarterTonesPitchKind
            noteQuarterTonesPitchKind =
              quarterTonesPitchKindFromSemiTonesPitchKind (
                noteSemiTonesPitchKind);
    
          // print it
          const int fieldWidth2 = 5;
    
          os << left <<
            setw (fieldWidth2) <<
            msrQuarterTonesPitchKindAsString (
              gLpsrOptions->
                fLpsrQuarterTonesPitchesLanguageKind,
              noteQuarterTonesPitchKind) <<
            " : " <<
            msrIntervalKindAsString (intervalKind) <<
            endl;
    
          if (++i == iEnd) break;
          
          // no endl here
        } // for
  
        gIndenter--;
    
        os <<
          endl;
      }
        
      // print the chord's inner intervals
      {
        os <<
          "Chord '" <<
          rootQuarterTonesPitchKindAsString <<
          " " <<
          harmonyKindShortName <<
          "'";
  
        if (inversion == 0) {
          os <<
            " fundamental state";
        }
        else {
          os <<
            " inversion" << inversion;
        }
  
        os <<
          " inner intervals:" <<
          endl;
        
        gIndenter++;

        int tritonsCounter = 0;
        
        vector<S_msrChordInterval>::const_iterator
          iBegin1 = invertedChordStructureIntervals.begin (),
          iEnd1   = invertedChordStructureIntervals.end () - 1,
          i1      = iBegin1;
  
        for ( ; ; ) {
          S_msrChordInterval
            chordInterval1 = (*i1);
    
          msrIntervalKind
            intervalKind1 =
              chordInterval1->
                getChordIntervalIntervalKind ();
                  
          // fetch the semitones pitch kind
          msrSemiTonesPitchKind
            noteSemiTonesPitchKind1 =
              noteAtIntervalFromSemiTonesPitch (
                NO_INPUT_LINE_NUMBER,
                intervalKind1,
                rootSemiTonesPitchKind);
    
          // fetch the quartertones pitch kind
          msrQuarterTonesPitchKind
            noteQuarterTonesPitchKind1 =
              quarterTonesPitchKindFromSemiTonesPitchKind (
                noteSemiTonesPitchKind1);
    
          // print the invervals
          vector<S_msrChordInterval>::const_iterator
            iBegin2 = i1 + 1,
            iEnd2   = invertedChordStructureIntervals.end (),
            i2      = iBegin2;
    
          for ( ; ; ) {
            S_msrChordInterval
              chordInterval2 = (*i2);
      
            msrIntervalKind
              intervalKind2 =
                chordInterval2->
                  getChordIntervalIntervalKind ();
                      
            const int fieldWidth1 = 5;
      
            // fetch the semitones pitch kind
            msrSemiTonesPitchKind
              noteSemiTonesPitchKind2 =
                noteAtIntervalFromSemiTonesPitch (
                  NO_INPUT_LINE_NUMBER,
                  intervalKind2,
                  rootSemiTonesPitchKind);
      
            // fetch the quartertones pitch kind
            msrQuarterTonesPitchKind
              noteQuarterTonesPitchKind2 =
                quarterTonesPitchKindFromSemiTonesPitchKind (
                  noteSemiTonesPitchKind2);

            // compute the inner interval
            S_msrChordInterval
              interInterval =
                chordInterval2->
                  intervalDifference (
                    chordInterval1);

            msrIntervalKind
              innerIntervalKind =
                interInterval->
                  getChordIntervalIntervalKind ();

            // is this interval a triton?
            switch (innerIntervalKind) {
              case kAugmentedFourth:
              case kDiminishedFifth:
                tritonsCounter++;
                break;
              default:
                ;
            } // switch
                
            // print it
            gIndenter++;

            const int fieldWidth2 = 20;

            os << left <<
              setw (fieldWidth1) <<
              msrQuarterTonesPitchKindAsString (
                gLpsrOptions->
                  fLpsrQuarterTonesPitchesLanguageKind,
                noteQuarterTonesPitchKind1) <<
              
              " -> " <<
              
              setw (fieldWidth1) <<
              msrQuarterTonesPitchKindAsString (
                gLpsrOptions->
                  fLpsrQuarterTonesPitchesLanguageKind,
                noteQuarterTonesPitchKind2) <<

              " : " <<
              
              setw (fieldWidth2) << // JMI
              msrIntervalKindAsString (innerIntervalKind) <<

              "(" <<
              setw (fieldWidth2) <<
              msrIntervalKindAsString (intervalKind1) <<
              " -> " <<
              msrIntervalKindAsString (intervalKind2) <<
              ")" <<           

              endl;

            gIndenter--;
                  
            if (++i2 == iEnd2) break;
          } // for
          
          if (++i1 == iEnd1) break;
          
          os <<
            endl;
        } // for
  
        gIndenter--;
    

        if (tritonsCounter > 0) {
          os <<
            "This chord contains " <<
            singularOrPlural (
              tritonsCounter, "triton", "tritons") <<
            endl;
        }

        os <<
          endl;
      }
    }

    else {
      os <<
        "Chord '" <<
        rootQuarterTonesPitchKindAsString <<
        " " <<
        harmonyKindShortName <<
        "' has only " <<
        chordStructureIntervalsNumber <<
        " intervals, inversion " <<
        inversion <<
        " does not exist" <<
        endl <<
        endl;
    }
  }

  gIndenter--;
}

//______________________________________________________________________________
string msrScoreNotationKindAsString (
  msrScoreNotationKind scoreNotationKind)
{
  string result;
  
  switch (scoreNotationKind) {
    case kScoreNotationWestern:
      result = "scoreNotationWestern";
    case kScoreNotationJianpu:
      result = "scoreNotationJianpu";
    case kScoreNotationDiatonicAccordion:
      result = "scoreNotationDiatonicAccordion";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
void initializeMSRBasicTypes ()
{
  // languages handling
  // ------------------------------------------------------

  initializeQuarterTonesPitchesLanguageKinds ();
  
  // chord structures handling
  // ------------------------------------------------------

  initializeChordStructuresMap ();
}


}
