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
#include "msrOptions.h"

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
    case kPerUnison:
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
    case kPerUnison:
      result = "PerUnison";
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
    case kPerUnison:
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
    case kPerUnison:
      result = kPerUnison;
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

msrIntervalKind intervalKindSum (
  msrIntervalKind intervalKind1,
  msrIntervalKind intervalKind2)
{
  msrIntervalKind result;

  switch (intervalKind1) { // JMI TODO
    case kDiminishedUnisson:
      result = kAugmentedUnison;
      break;
    case kPerUnison:
      result = kPerUnison;
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

msrIntervalKind intervalKindDifference (
  msrIntervalKind intervalKind1,
  msrIntervalKind intervalKind2)
{
  msrIntervalKind result;

  switch (intervalKind1) { // JMI TODO
    case kDiminishedUnisson:
      result = kAugmentedUnison;
      break;
    case kPerUnison:
      result = kPerUnison;
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
        case kPerUnison:
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
        case kPerFourth:
          break;
        case kAugmentedFourth:
          break;
          
        case kDiminishedFifth:
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          break;
        case kAugmentedEleventh:
          break;
          
        case kDiminishedTwelfth:
          break;
        case kPerTwelfth:
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
      break;

    case kC_Natural_STP:
      switch (intervalKind) {
        case kDiminishedUnisson:
          result = kC_Flat_STP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kF_Natural_STP;
          break;
        case kAugmentedFourth:
          result = kF_Sharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kG_Flat_STP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kF_Natural_STP;
          break;
        case kAugmentedEleventh:
          result = kF_Sharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kG_Flat_STP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kC_Natural_STP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kF_Sharp_STP;
          break;
        case kAugmentedFourth:
          result = kF_DoubleSharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kG_Natural_STP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kF_Sharp_STP;
          break;
        case kAugmentedEleventh:
          result = kF_DoubleSharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kG_Natural_STP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kD_DoubleFlat_STP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kG_Flat_STP;
          break;
        case kAugmentedFourth:
          result = kG_Natural_STP;
          break;
          
        case kDiminishedFifth:
          result = kA_DoubleFlat_STP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kG_Flat_STP;
          break;
        case kAugmentedEleventh:
          result = kG_Natural_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kA_DoubleFlat_STP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kD_Flat_STP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kG_Natural_STP;
          break;
        case kAugmentedFourth:
          result = kG_Sharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kA_Flat_STP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kG_Natural_STP;
          break;
        case kAugmentedEleventh:
          result = kG_Sharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kA_Flat_STP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kD_Natural_STP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kG_Sharp_STP;
          break;
        case kAugmentedFourth:
          result = kG_DoubleSharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kA_Natural_STP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kG_Sharp_STP;
          break;
        case kAugmentedEleventh:
          result = kG_DoubleSharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kA_Natural_STP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kE_DoubleFlat_STP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kA_Flat_STP;
          break;
        case kAugmentedFourth:
          result = kA_Natural_STP;
          break;
          
        case kDiminishedFifth:
          result = kB_DoubleFlat_STP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kA_Flat_STP;
          break;
        case kAugmentedEleventh:
          result = kA_Natural_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kB_DoubleFlat_STP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kE_Flat_STP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kA_Natural_STP;
          break;
        case kAugmentedFourth:
          result = kA_Sharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kB_Natural_STP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kA_Natural_STP;
          break;
        case kAugmentedEleventh:
          result = kA_Sharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kB_Natural_STP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kE_Natural_STP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kA_Sharp_STP;
          break;
        case kAugmentedFourth:
          result = kA_DoubleSharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kB_Natural_STP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kA_Sharp_STP;
          break;
        case kAugmentedEleventh:
          result = kA_DoubleSharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kB_Natural_STP;
          break;
        case kPerTwelfth:
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
      break;
      
    case kF_Natural_STP:
      switch (intervalKind) {
        case kDiminishedUnisson:
          result = kF_Flat_STP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kB_Flat_STP;
          break;
        case kAugmentedFourth:
          result = kB_Natural_STP;
          break;
          
        case kDiminishedFifth:
          result = kC_Flat_STP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kB_Flat_STP;
          break;
        case kAugmentedEleventh:
          result = kB_Natural_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kC_Flat_STP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kF_Natural_STP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kB_DoubleSharp_STP;
          break;
        case kAugmentedFourth:
          result = kB_TripleSharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kC_Natural_STP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kB_DoubleSharp_STP;
          break;
        case kAugmentedEleventh:
          result = kB_TripleSharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kC_Natural_STP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kG_DoubleFlat_STP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kC_Flat_STP;
          break;
        case kAugmentedFourth:
          result = kC_Natural_STP;
          break;
          
        case kDiminishedFifth:
          result = kD_DoubleFlat_STP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kC_Flat_STP;
          break;
        case kAugmentedEleventh:
          result = kC_Natural_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kD_DoubleFlat_STP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kG_Flat_STP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kC_Natural_STP;
          break;
        case kAugmentedFourth:
          result = kC_Sharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kD_Flat_STP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kC_Natural_STP;
          break;
        case kAugmentedEleventh:
          result = kC_Sharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kD_Flat_STP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kG_Natural_STP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kC_Sharp_STP;
          break;
        case kAugmentedFourth:
          result = kC_DoubleSharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kD_Natural_STP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kC_Sharp_STP;
          break;
        case kAugmentedEleventh:
          result = kC_DoubleSharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kD_Natural_STP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kA_DoubleFlat_STP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kD_Flat_STP;
          break;
        case kAugmentedFourth:
          result = kD_Natural_STP;
          break;
          
        case kDiminishedFifth:
          result = kE_DoubleFlat_STP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kD_Flat_STP;
          break;
        case kAugmentedEleventh:
          result = kD_Natural_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kE_DoubleFlat_STP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kA_Flat_STP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kD_Natural_STP;
          break;
        case kAugmentedFourth:
          result = kD_Sharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kE_Flat_STP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kD_Natural_STP;
          break;
        case kAugmentedEleventh:
          result = kD_Sharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kE_Flat_STP;
          break;
        case kPerTwelfth:
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
      break;

    case kB_Flat_STP:
      switch (intervalKind) {
        case kDiminishedUnisson:
          result = kB_DoubleFlat_STP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kE_Flat_STP;
          break;
        case kAugmentedFourth:
          result = kE_Natural_STP;
          break;
          
        case kDiminishedFifth:
          result = kF_Flat_STP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kE_Flat_STP;
          break;
        case kAugmentedEleventh:
          result = kE_Natural_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kF_Flat_STP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kB_Flat_STP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kE_Natural_STP;
          break;
        case kAugmentedFourth:
          result = kE_Sharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kF_Natural_STP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kE_Natural_STP;
          break;
        case kAugmentedEleventh:
          result = kE_Sharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kF_Natural_STP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kB_Natural_STP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kE_Sharp_STP;
          break;
        case kAugmentedFourth:
          result = kE_DoubleSharp_STP;
          break;
          
        case kDiminishedFifth:
          result = kF_Sharp_STP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kE_Sharp_STP;
          break;
        case kAugmentedEleventh:
          result = kE_DoubleSharp_STP;
          break;
          
        case kDiminishedTwelfth:
          result = kF_Sharp_STP;
          break;
        case kPerTwelfth:
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
          "Sorry, computing intervals from pitch '" <<
          msrSemiTonesPitchKindAsString (semiTonesPitchKind) <<
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
        case kDiminishedUnisson:
          result = kC_Flat_QTP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kF_Natural_QTP;
          break;
        case kAugmentedFourth:
          result = kF_Sharp_QTP;
          break;
          
        case kDiminishedFifth:
          result = kG_Flat_QTP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kF_Natural_QTP;
          break;
        case kAugmentedEleventh:
          result = kF_Sharp_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kG_Flat_QTP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kC_Natural_QTP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kF_Sharp_QTP;
          break;
        case kAugmentedFourth:
          result = kF_DoubleSharp_QTP;
          break;
          
        case kDiminishedFifth:
          result = kG_Natural_QTP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kF_Sharp_QTP;
          break;
        case kAugmentedEleventh:
          result = kF_DoubleSharp_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kG_Natural_QTP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kD_DoubleFlat_QTP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kG_Flat_QTP;
          break;
        case kAugmentedFourth:
          result = kG_Natural_QTP;
          break;
          
        case kDiminishedFifth:
          result = kA_DoubleFlat_QTP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kG_Flat_QTP;
          break;
        case kAugmentedEleventh:
          result = kG_Natural_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kA_DoubleFlat_QTP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kD_Flat_QTP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kG_Natural_QTP;
          break;
        case kAugmentedFourth:
          result = kG_Sharp_QTP;
          break;
          
        case kDiminishedFifth:
          result = kA_Flat_QTP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kG_Natural_QTP;
          break;
        case kAugmentedEleventh:
          result = kG_Sharp_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kA_Flat_QTP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kD_Natural_QTP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kG_Sharp_QTP;
          break;
        case kAugmentedFourth:
          result = kG_DoubleSharp_QTP;
          break;
          
        case kDiminishedFifth:
          result = kA_Natural_QTP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kG_Sharp_QTP;
          break;
        case kAugmentedEleventh:
          result = kG_DoubleSharp_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kA_Natural_QTP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kE_DoubleFlat_QTP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kA_Flat_QTP;
          break;
        case kAugmentedFourth:
          result = kA_Natural_QTP;
          break;
          
        case kDiminishedFifth:
          result = kB_DoubleFlat_QTP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kA_Flat_QTP;
          break;
        case kAugmentedEleventh:
          result = kA_Natural_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kB_DoubleFlat_QTP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kE_Flat_QTP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kA_Natural_QTP;
          break;
        case kAugmentedFourth:
          result = kA_Sharp_QTP;
          break;
          
        case kDiminishedFifth:
          result = kB_Natural_QTP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kA_Natural_QTP;
          break;
        case kAugmentedEleventh:
          result = kA_Sharp_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kB_Natural_QTP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kE_Natural_QTP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kA_Sharp_QTP;
          break;
        case kAugmentedFourth:
          result = kA_DoubleSharp_QTP;
          break;
          
        case kDiminishedFifth:
          result = kB_Natural_QTP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kA_Sharp_QTP;
          break;
        case kAugmentedEleventh:
          result = kA_DoubleSharp_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kB_Natural_QTP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kF_Flat_QTP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kB_Flat_QTP;
          break;
        case kAugmentedFourth:
          result = kB_Natural_QTP;
          break;
          
        case kDiminishedFifth:
          result = kC_Flat_QTP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kB_Flat_QTP;
          break;
        case kAugmentedEleventh:
          result = kB_Natural_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kC_Flat_QTP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kF_Natural_QTP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kB_DoubleSharp_QTP;
          break;
        case kAugmentedFourth:
          result = kB_TripleSharp_QTP;
          break;
          
        case kDiminishedFifth:
          result = kC_Natural_QTP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kB_DoubleSharp_QTP;
          break;
        case kAugmentedEleventh:
          result = kB_TripleSharp_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kC_Natural_QTP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kG_DoubleFlat_QTP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kC_Flat_QTP;
          break;
        case kAugmentedFourth:
          result = kC_Natural_QTP;
          break;
          
        case kDiminishedFifth:
          result = kD_DoubleFlat_QTP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kC_Flat_QTP;
          break;
        case kAugmentedEleventh:
          result = kC_Natural_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kD_DoubleFlat_QTP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kG_Flat_QTP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kC_Natural_QTP;
          break;
        case kAugmentedFourth:
          result = kC_Sharp_QTP;
          break;
          
        case kDiminishedFifth:
          result = kD_Flat_QTP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kC_Natural_QTP;
          break;
        case kAugmentedEleventh:
          result = kC_Sharp_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kD_Flat_QTP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kG_Natural_QTP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kC_Sharp_QTP;
          break;
        case kAugmentedFourth:
          result = kC_DoubleSharp_QTP;
          break;
          
        case kDiminishedFifth:
          result = kD_Natural_QTP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kC_Sharp_QTP;
          break;
        case kAugmentedEleventh:
          result = kC_DoubleSharp_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kD_Natural_QTP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kA_DoubleFlat_QTP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kD_Flat_QTP;
          break;
        case kAugmentedFourth:
          result = kD_Natural_QTP;
          break;
          
        case kDiminishedFifth:
          result = kE_DoubleFlat_QTP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kD_Flat_QTP;
          break;
        case kAugmentedEleventh:
          result = kD_Natural_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kE_DoubleFlat_QTP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kA_Flat_QTP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kD_Natural_QTP;
          break;
        case kAugmentedFourth:
          result = kD_Sharp_QTP;
          break;
          
        case kDiminishedFifth:
          result = kE_Flat_QTP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kD_Natural_QTP;
          break;
        case kAugmentedEleventh:
          result = kD_Sharp_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kE_Flat_QTP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kB_DoubleFlat_QTP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kE_Flat_QTP;
          break;
        case kAugmentedFourth:
          result = kE_Natural_QTP;
          break;
          
        case kDiminishedFifth:
          result = kF_Flat_QTP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kE_Flat_QTP;
          break;
        case kAugmentedEleventh:
          result = kE_Natural_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kF_Flat_QTP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kB_Flat_QTP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kE_Natural_QTP;
          break;
        case kAugmentedFourth:
          result = kE_Sharp_QTP;
          break;
          
        case kDiminishedFifth:
          result = kF_Natural_QTP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kE_Natural_QTP;
          break;
        case kAugmentedEleventh:
          result = kE_Sharp_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kF_Natural_QTP;
          break;
        case kPerTwelfth:
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
        case kDiminishedUnisson:
          result = kB_Natural_QTP;
          break;
        case kPerUnison:
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
        case kPerFourth:
          result = kE_Sharp_QTP;
          break;
        case kAugmentedFourth:
          result = kE_DoubleSharp_QTP;
          break;
          
        case kDiminishedFifth:
          result = kF_Sharp_QTP;
          break;
        case kPerFifth:
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
        case kPerOctave:
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
        case kPerEleventh:
          result = kE_Sharp_QTP;
          break;
        case kAugmentedEleventh:
          result = kE_DoubleSharp_QTP;
          break;
          
        case kDiminishedTwelfth:
          result = kF_Sharp_QTP;
          break;
        case kPerTwelfth:
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
          "Sorry, computing intervals from pitch '" <<
          msrQuarterTonesPitchKindAsString (
            gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
            quarterTonesPitchKind) <<
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

// harmonies
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
  
  else if (alter == -3 ) {
    result = kTripleFlat;
  }
  
  else if (alter == +3 ) {
    result = kTripleSharp;
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
  msrDiatonicPitchKind result;
  
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
  int                   inputLineNumber,
  msrSemiTonesPitchKind semiTonesPitchKind)
{
  msrQuarterTonesPitchKind result = k_NoQuarterTonesPitch_QTP;
  
  switch (semiTonesPitchKind) {
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
    case k_NoSemiTonesPitch_STP:
      result = "NoWellTemperedPitch???";
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
    case k_NoSemiTonesPitch_STP:
      result = "k_NoSemiTonesPitch_STP???";
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
  msrIntervalKind chordItemIntervalKind,
  int             chordItemRelativeOctave)
{
  msrChordItem* o =
    new msrChordItem (
      inputLineNumber,
      chordItemNumber,
      chordItemIntervalKind,
      chordItemRelativeOctave);
  assert(o!=0);

  return o;
}

msrChordItem::msrChordItem (
  int             inputLineNumber,
  int             chordItemNumber,
  msrIntervalKind chordItemIntervalKind,
  int             chordItemRelativeOctave)
  // JMI  : msrElement (inputLineNumber)
{
  fChordItemNumber       = chordItemNumber;
  
  fChordItemIntervalKind = chordItemIntervalKind;

  fChordItemRelativeOctave = chordItemRelativeOctave;

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

  const int fieldWidth = 19;
  
  s << left <<
    "ChordItem" <<
    " " << fChordItemNumber <<
    ": " <<
    setw (fieldWidth) <<
    msrIntervalKindAsString (fChordItemIntervalKind) + "," <<
    "chordItemRelativeOctave: " << fChordItemRelativeOctave;

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
    endl;

  gIndenter++;

  const int fieldWidth = 22;
  
  os << left <<
    setw (fieldWidth) <<
    "chordItemNumber" << " : " << fChordItemNumber <<
    endl <<
    setw (fieldWidth) <<
    "chordItemIntervalKind" << " : " <<
      msrIntervalKindAsString (fChordItemIntervalKind) <<
    endl <<
    setw (fieldWidth) <<
    "chordItemRelativeOctave" << " : " << fChordItemRelativeOctave <<
  /* JMI
    ", line: " << fInputLineNumber <<
    */
    endl;

  gIndenter--;
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
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth,
            0) // relative octave
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
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMinorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth,
            0) // relative octave
          );
      }
      break;
    
    case kAugmentedHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kAugmentedFifth,
            0) // relative octave
          );
      }
      break;
    
    case kDiminishedHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMinorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kDiminishedFifth,
            0) // relative octave
          );
      }
      break;
    
    case kDominantHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMinorSeventh,
            0) // relative octave
          );
      }
      break;
    
    case kMajorSeventhHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMajorSeventh,
            0) // relative octave
          );
      }
      break;
    
     case kMinorSeventhHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMinorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMinorSeventh,
            0) // relative octave
          );
      }
      break;
    
    case kDiminishedSeventhHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMinorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kDiminishedFifth,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kDiminishedSeventh,
            0) // relative octave
          );
      }
      break;
    
    case kAugmentedSeventhHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kAugmentedFifth,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMinorSeventh,
            0) // relative octave
          );
      }
      break;
    
    case kHalfDiminishedHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMinorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kDiminishedFifth,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMinorSeventh,
            0) // relative octave
          );
      }
      break;
    
     case kMajorMinorHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMinorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMajorSeventh,
            0) // relative octave
          );
      }
      break;
    
    case kMajorSixthHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMajorSixth,
            0) // relative octave
          );
      }
      break;
    
    case kMinorSixthHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMinorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMajorSixth,
            0) // relative octave
          );
      }
      break;
    
    case kDominantNinthHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMinorSeventh,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            5, // chord item number
            kMajorSeventh,
            0) // relative octave
          );
      }
      break;

    case kMajorNinthHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMajorSeventh,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            5, // chord item number
            kMajorNinth,
            0) // relative octave
          );
      }
      break;
    
    case kMinorNinthHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMinorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMinorSeventh,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            5, // chord item number
            kMajorNinth,
            0) // relative octave
          );
      }
      break;
    
    case kDominantEleventhHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMinorSeventh,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            5, // chord item number
            kMajorSeventh,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            6, // chord item number
            kPerEleventh,
            0) // relative octave
          );
      }
      break;

    case kMajorEleventhHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMajorSeventh,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            5, // chord item number
            kMajorNinth,
            0) // relative octave
          );
         appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            6, // chord item number
            kPerEleventh,
            0) // relative octave
          );
      }
      break;
    
    case kMinorEleventhHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMinorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMinorSeventh,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            5, // chord item number
            kMajorNinth,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            6, // chord item number
            kPerEleventh,
            0) // relative octave
          );
      }
      break;
    
    case kDominantThirteenthHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMinorSeventh,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            5, // chord item number
            kMajorSeventh,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            6, // chord item number
            kPerEleventh,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            7, // chord item number
            kMajorThirteenth,
            0) // relative octave
          );
      }
      break;

    case kMajorThirteenthHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMajorSeventh,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            5, // chord item number
            kMajorNinth,
            0) // relative octave
          );
         appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            6, // chord item number
            kPerEleventh,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            7, // chord item number
            kMajorThirteenth,
            0) // relative octave
          );
      }
      break;
    
    case kMinorThirteenthHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMinorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kMinorSeventh,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            5, // chord item number
            kMajorNinth,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            6, // chord item number
            kPerEleventh,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            7, // chord item number
            kMajorThirteenth,
            0) // relative octave
          );
      }
      break;
    
    case kSuspendedSecondHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorSecond,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth,
            0) // relative octave
          );
      }
      break;
    
    case kSuspendedFourthHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kPerFourth,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth,
            0) // relative octave
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
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMinorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kDiminishedSixth,
            0) // relative octave
          );
      }
      break;
    
    case kItalianHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kAugmentedSixth,
            0) // relative octave
          );
      }
      break;
    
    case kFrenchHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kAugmentedFourth,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kAugmentedSixth,
            0) // relative octave
          );
      }
      break;
    
    case kGermanHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kMajorThird,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kPerFifth,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kAugmentedSixth,
            0) // relative octave
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
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kPerFifth,
            0) // relative octave
          );
      }
      break;
    
    case kTristanHarmony:
      {
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            1, // chord item number
            kPerUnison,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            2, // chord item number
            kAugmentedSecond,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            3, // chord item number
            kAugmentedFourth,
            0) // relative octave
          );
        appendChordItemToChordIntervals (
          msrChordItem::create (
            0, // input line number
            4, // chord item number
            kAugmentedSixth,
            0) // relative octave
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

S_msrChordItem msrChordIntervals::bassChordItemForChordInversion (
  int inputLineNumber,
  int inversionNumber)
{
  /*
    Inversion is a number indicating which inversion is used:
    0 for root position, 1 for first inversion, etc.
  */

  S_msrChordItem result;

  if (
    inversionNumber < 0
      ||
    inversionNumber > int (fChordIntervalsItems.size ()) - 1 ) {
    stringstream s;

    s <<
      "Sorry, inversion number '" <<
      inversionNumber <<
      "' does not exist for chord intevals '" <<
      msrHarmonyKindAsString (fChordIntervalsHarmonyKind) <<
      "', line " << inputLineNumber;

    msrLimitation (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return fChordIntervalsItems [inversionNumber];
}

void msrChordIntervals::printAllChordsIntervals (ostream& os)
{
  os <<
    "All the known chords intervals are:" <<
    endl <<
    endl;

  gIndenter++;
  
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
    os <<
      chordIntervals <<
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

msrSemiTonesPitchKind msrChordNotes::bassSemiTonesPitchKindForChordInversion (
  int inputLineNumber,
  int inversionNumber)
{
  /*
    Inversion is a number indicating which inversion is used:
    0 for root position, 1 for first inversion, etc.
  */

  S_msrChordItem result;

  if (
    inversionNumber < 0
      ||
    inversionNumber > int (fChordSemiTonesPitches.size ()) - 1 ) {
    stringstream s;

    s <<
      "Sorry, inversion number '" <<
      inversionNumber <<
      "' does not exist for chord notes '" <<
      msrHarmonyKindAsString (fChordHarmonyKind) <<
      "', line " << inputLineNumber;

    msrLimitation (
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return fChordSemiTonesPitches [inversionNumber];
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


}
