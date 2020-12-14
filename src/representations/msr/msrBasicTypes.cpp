/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iostream>
#include <sstream>
#include <cassert>

#include <climits>      // INT_MIN, INT_MAX
#include <iomanip>      // setw, ...

#include <regex>

#include "rational.h"

#include "utilities.h"

#include "messagesHandling.h"

#include "msrBasicTypes.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "msrOah.h"
#include "lpsrOah.h"


namespace MusicXML2
{

// XMLLang
//______________________________________________________________________________
msrXMLLangKind msrXMLLangKindFromString (
  int    inputLineNumber,
  string XMLLangString)
{
  msrXMLLangKind
    result = kXMLLangIt; // default value

  if      (XMLLangString == "it")
    result = kXMLLangIt;
  else if (XMLLangString == "en")
    result = kXMLLangEn;
  else if (XMLLangString == "de")
    result = kXMLLangDe;
  else if (XMLLangString == "fr")
    result = kXMLLangFr;
  else if (XMLLangString == "ja")
    result = kXMLLangJa;
  else if (XMLLangString == "la")
    result = kXMLLangLa;
  else {
    if (XMLLangString.size ()) {
      stringstream s;

      s <<
        "xml:lang value '" << XMLLangString <<
        "' should be 'it', 'en', 'de' or 'fr'";

      msrMusicXMLError (
        gGlobalOahOahGroup->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  return result;
}

string msrXMLLangKindAsString (
  msrXMLLangKind XMLLangKind)
{
  string result;

  switch (XMLLangKind) {
    case kXMLLangIt:
      result = "it";
      break;
    case kXMLLangEn:
      result = "en";
      break;
    case kXMLLangDe:
      result = "de";
      break;
    case kXMLLangFr:
      result = "fr";
    case kXMLLangJa:
      result = "ja";
      break;
    case kXMLLangLa:
      result = "la";
      break;
  } // switch

  return result;
}

// dotted durations
//______________________________________________________________________________
msrDottedDuration::msrDottedDuration ()
{
  fDurationKind = k_NoDuration;
  fDotsNumber   = -1;
}

msrDottedDuration::msrDottedDuration (
  msrDurationKind durationKind,
  int             dotsNumber)
{
  fDurationKind = durationKind;
  fDotsNumber   = dotsNumber;
}

msrDottedDuration::~msrDottedDuration ()
{}

rational msrDottedDuration::dottedDurationAsWholeNotes (
  int inputLineNumber) const
{
  // convert duration into whole notes
  rational
    result =
      msrDurationKindAsWholeNotes (
        fDurationKind);

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

string msrDottedDuration::asString () const
{
  stringstream s;

  s <<
     msrDurationKindAsString (fDurationKind);

  for (int i = 1; i <= fDotsNumber; i++) {
    s << ".";
  } // for

  return s.str ();
}

void msrDottedDuration::print (ostream& os) const
{
  const int fieldWidth = 11;

  os << left <<
    setw (fieldWidth) <<
    "durationKind" << " : " <<
    msrDurationKindAsString (fDurationKind) <<
    endl <<
    setw (fieldWidth) <<
    "dotsNumber" << " : " << fDotsNumber <<
    endl;
};

ostream& operator<< (ostream& os, const msrDottedDuration& elt)
{
  elt.print (os);
  return os;
}

// semitone pitches and absolute octave
//______________________________________________________________________________
S_msrSemiTonesPitchAndOctave msrSemiTonesPitchAndOctave::create (
  msrSemiTonesPitchKind semiTonesPitchKind,
  int                   relativeOctave)
{
  msrSemiTonesPitchAndOctave* o =
    new msrSemiTonesPitchAndOctave (
      semiTonesPitchKind,
      relativeOctave);
  assert (o!=0);

  return o;
}

S_msrSemiTonesPitchAndOctave msrSemiTonesPitchAndOctave::createFromString (
  int    inputLineNumber,
  string theString)
{
  S_msrSemiTonesPitchAndOctave result;

  // decipher theString with a three-number regular expression
  string regularExpression (
    "([[:lower:]]+)"
    "([,\']*)");

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for transposition string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl <<
      smSize << " elements: ";

      for (unsigned i = 0; i < smSize; ++i) {
        gLogStream <<
          "[" << sm [i] << "] ";
      } // for

      gLogStream << endl;
    }
#endif

  if (smSize == 3) {
    // found a well-formed specification,
    // need to check its ',' and '\'' contents
    string
      pitch            = sm [1],
      octaveIndication = sm [2];

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      gLogStream <<
        "--> pitch = \"" << pitch << "\", " <<
        "--> octaveIndication = \"" << octaveIndication << "\"" <<
        endl;
    }
#endif

    // fetch semitones pitch
    msrSemiTonesPitchKind
      semiTonesPitchKind =
        semiTonesPitchKindFromString (
          pitch);

    // handling ',' and '\'' in octave indication
    // middle C, LilyPond's c', starts octave 4,
    // thus a single pitch without any octave indication is in octave 3
    int octave = 3;
    for (unsigned int i = 0; i < octaveIndication.size (); i++) {
      switch (octaveIndication [i]) {
        case ',':
          if (octave > 3) {
            // a '\'' has been found previously
            stringstream s;

            s <<
              "argument \"" << theString <<
              "\" contains a ',' after a '\\'";

            oahError (s.str ());
          }

          octave--;
          break;
        case '\'':
          if (octave < 3) {
            // a ',' has been found previously
            stringstream s;

            s <<
              "argument \"" << theString <<
              "\" contains a '\\'' after a ','";

            oahError (s.str ());
          }

          octave++;
          break;
        default:
          ;
      } // switch
    } // for

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      gLogStream <<
        "--> semiTonesPitchKind = \"" <<
          msrSemiTonesPitchKindAsString (
            semiTonesPitchKind) << "\", " <<
        "--> octave = " << octave <<
        endl;
    }
#endif

    // create the semiTonesPitchAndOctave
    result =
      msrSemiTonesPitchAndOctave::create (
       semiTonesPitchKind,
       octave);
  }

  else {
    stringstream s;

    s <<
      "semitones pitch and octave argument '" << theString <<
      "' is ill-formed";

    msrMusicXMLError (
//    msrMusicXMLWarning ( //  JMI
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

msrSemiTonesPitchAndOctave::msrSemiTonesPitchAndOctave (
  msrSemiTonesPitchKind semiTonesPitchKind,
  int                   relativeOctave)
{
  fSemiTonesPitchKind = semiTonesPitchKind;
  fOctave            = relativeOctave;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceHarmoniesDetails ()) {
    gLogStream <<
      "==> Creating pitch and octave '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif
}

msrSemiTonesPitchAndOctave::~msrSemiTonesPitchAndOctave ()
{}

S_msrSemiTonesPitchAndOctave msrSemiTonesPitchAndOctave::createSemiTonesPitchAndOctaveNewbornClone ()
{
  S_msrSemiTonesPitchAndOctave
    newbornClone =
      msrSemiTonesPitchAndOctave::create (
        fSemiTonesPitchKind,
        fOctave);

  return newbornClone;
}

string msrSemiTonesPitchAndOctave::asString () const
{
  stringstream s;

  s << left <<
    "SemiTonesPitchAndOctave" <<
    ": " <<
    "semiTonesPitchKind: " <<
    msrSemiTonesPitchKindAsString (fSemiTonesPitchKind) <<
    ", octave: " << fOctave;

  return s.str ();
}

void msrSemiTonesPitchAndOctave::print (ostream& os) const
{
  os <<
    "SemiTonesPitchAndOctave" <<
    endl;

  gIndenter++;

  const int fieldWidth = 22;

  os << left <<
    setw (fieldWidth) <<
    "semiTonesPitchKind" << " : " <<
      msrSemiTonesPitchKindAsString (fSemiTonesPitchKind) <<
    endl <<
    setw (fieldWidth) <<
    "octave" << " : " << fOctave <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrSemiTonesPitchAndOctave& elt)
{
  elt->print (os);
  return os;
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
      gGlobalOahOahGroup->getInputSourceName (),
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

string msrDurationKindAsMusicXMLType (msrDurationKind durationKind)
{
  string result;

  switch (durationKind) {
    case k_NoDuration:
      result = "noDuration";
      break;

    case k1024th:
      result = "1024th";
      break;
    case k512th:
      result = "512th";
      break;
    case k256th:
      result = "256th";
      break;
    case k128th:
      result = "128th";
      break;
    case k64th:
      result = "64th";
      break;
    case k32nd:
      result = "32nd";
      break;
    case k16th:
      result = "16th";
      break;
    case kEighth:
      result = "eighth";
      break;
    case kQuarter:
      result = "quarter";
      break;
    case kHalf:
      result = "half";
      break;
    case kWhole:
      result = "whole";
      break;
    case kBreve:
      result = "breve";
      break;
    case kLong:
      result = "long";
      break;
    case kMaxima:
      result = "maxima";
      break;
  } // switch

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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceWholeNotes ()) {
    gLogStream <<
      "--> wholeNotesAsMsrString() 1 -------------------------------------" <<
      ", wholeNotes: " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  int
    numerator    = wholeNotes.getNumerator (),
    denominator  = wholeNotes.getDenominator ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceWholeNotes ()) {
    gLogStream <<
      "--> numerator:   " << numerator <<
      endl <<
      "--> denominator: " << denominator <<
      endl << endl;
  }
#endif

  if (numerator == 0) { // JMI TEMP
    dotsNumber = 0;
    return "zero";
  }
  else if (numerator < 0) {
    // sanity check
    stringstream s;

    s <<
      "numerator is not positive in wholeNotesAsMsrString()" <<
      ", wholeNotes = " << wholeNotes;

 //   msrMusicXMLError ( JMI
    msrMusicXMLWarning (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
  //    __FILE__, __LINE__,
      s.str ());

    return "???";
  }

  wholeNotes.rationalise ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
      "--> wholeNotes rationalised: " << wholeNotes <<
      endl;
  }
#endif

  bool
    rationalHasBeenSimplified =
      wholeNotes.getNumerator () != numerator; // denominators could be used too

  if (rationalHasBeenSimplified) {
    numerator    = wholeNotes.getNumerator (),
    denominator  = wholeNotes.getDenominator ();
  }

  bool
    integralNumberOfWholeNotes = denominator == 1;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
      "--> rationalHasBeenSimplified: " <<
      booleanAsString (
        rationalHasBeenSimplified) <<
      endl <<
      "--> integralNumberOfWholeNotes: " <<
      booleanAsString (
        integralNumberOfWholeNotes) <<
      endl << endl;
  }
#endif

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
      "--> numeratorDots " << " : " << numeratorDots <<
      endl << endl;
  }
#endif

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

  int denominatorDurationLog =
    msrDurationBinaryLogarithm (denominator);

  if (denominatorDurationLog == INT_MIN) {
    string result;

    {
      stringstream s;

      s <<
        1 <<
        "*" <<
        numerator <<
        "/" <<
        denominator;

      result = s.str ();
    }

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceWholeNotesDetails ()) {
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
        gGlobalOahOahGroup->getInputSourceName (),
        inputLineNumber,
    //    __FILE__, __LINE__,
        s.str ());
    }
#endif

    return result;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
      "--> denominatorDurationLog" << " : " <<
      denominatorDurationLog <<
      endl << endl;
  }
#endif

  // bring the resulting fraction to be less that two if needed
  if (integralNumberOfWholeNotes) {
    // adapt the duration to avoid even numerators if can be,
    // since dotted durations cannot be recognized otherwise
    // 6/1 thus becomes 3 \breve, hence '\longa.'
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceWholeNotesDetails ()) {
      gLogStream <<
        "--> integralNumberOfWholeNotes,"
        " bringing the faction to be less that 2" <<
        endl;
    }
#endif

    while (numerator % 2 == 0) {
      numerator /= 2;
      denominatorDurationLog -= 1;

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceWholeNotesDetails ()) {
        gLogStream <<
          "--> numerator" << " : " <<
          numerator <<
          endl <<
          "--> denominatorDurationLog " << " : " <<
          denominatorDurationLog <<
          endl << endl;
      }
#endif
    } // while

    // update the number of dots
    numeratorDots = msrNumberOfDots (numerator);
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
      "--> numerator" << " : " <<
      numerator <<
      endl <<
      "--> denominatorDurationLog" << " : " <<
      denominatorDurationLog <<
      endl <<
      "--> numeratorDots " << " : " <<
      numeratorDots <<
      endl << endl;
  }
#endif

  // take care of the dots
  int multiplyingFactor = 1;

  if (numeratorDots >= 0 && denominatorDurationLog >= numeratorDots) {
    // take the dots into account
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceWholeNotesDetails ()) {
      gLogStream <<
        "--> taking the dots into account" <<
        endl;
    }
#endif

    denominatorDurationLog -= numeratorDots;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceWholeNotesDetails ()) {
      gLogStream <<
        "--> denominatorDurationLog" << " : " <<
        denominatorDurationLog <<
        endl <<
        "--> multiplyingFactor " << " : " <<
        multiplyingFactor <<
        endl << endl;
    }
#endif
  }
  else {
    // set the multiplying factor
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceWholeNotesDetails ()) {
      gLogStream <<
        "--> setting the multiplying factor" <<
        endl;
    }
#endif

    // 5/8 becomes 8*5

    multiplyingFactor = numerator;
    numerator = 1;

    /* JMI
    multiplyingFactor = numerator;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceWholeNotesDetails ()) {
      gLogStream <<
        "--> denominatorDurationLog" << " : " <<
        denominatorDurationLog <<
        endl <<
        "--> multiplyingFactor " << " : " <<
        multiplyingFactor <<
        endl << endl;
    }
#endif

    while (multiplyingFactor >= 2) {
      // double duration
      denominatorDurationLog--;

      // adapt multiplying factor
      multiplyingFactor /= 2;

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceWholeNotesDetails ()) {
        gLogStream <<
          "--> denominatorDurationLog" << " : " <<
          denominatorDurationLog <<
          endl <<
          "--> multiplyingFactor " << " : " <<
          multiplyingFactor <<
          endl << endl;
      }
#endif
    } // while
    */
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
      "--> numerator " << " : " <<
      numerator <<
      endl <<
      "--> numeratorDots " << " : " <<
      numeratorDots <<
      endl <<
      "--> denominatorDurationLog" << " : " <<
      denominatorDurationLog <<
      endl <<
      "--> multiplyingFactor " << " : " <<
      multiplyingFactor <<
      endl << endl;
  }
#endif

  // generate the code for the duration
  stringstream s;

  switch (denominatorDurationLog) {
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
      s << (1 << denominatorDurationLog);
  } // switch

  // append the dots if any
  if (numeratorDots > 0) {
    for (int i = 0; i < numeratorDots; i++) {
      s << ".";
    } // for
  }

  if (multiplyingFactor != 1) {
    // append the multiplying factor
    s <<
      "*" << multiplyingFactor;

    /* JMI
    if (integralNumberOfWholeNotes) {
      s <<
        "*" << multiplyingFactor;
    }
    else {
      s <<
        "*" << multiplyingFactor << "/" << 1; // ??? denominator;
    }
    */
  }

  string result = s.str ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceWholeNotes ()) {
    gLogStream <<
      "--> wholeNotesAsMsrString() 2 -------------------------------------" <<
     ", result: \"" << result << "\"" <<
      ", numeratorDots" << " : " << numeratorDots <<
      endl;
  }
#endif

  // return the result
  dotsNumber = numeratorDots;

  return result;
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

string multipleRestMeasuresWholeNotesAsMsrString (
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

// moments
//______________________________________________________________________________
msrMoment::msrMoment ()
{
  fWrittenPositionInMeseasure = rational (-1, 1);
  fSoundingRelativeOffset     = rational (-1, 1);
}

msrMoment::msrMoment (
  rational writtenPositionInMeseasure,
  rational soundingRelativeOffset)
{
  fWrittenPositionInMeseasure = writtenPositionInMeseasure;
  fSoundingRelativeOffset     = soundingRelativeOffset;
}

msrMoment::msrMoment (
  rational writtenPositionInMeseasure)
{
  fWrittenPositionInMeseasure = writtenPositionInMeseasure;
  fSoundingRelativeOffset     = rational (0, 1);
}

msrMoment::~msrMoment ()
{}

void msrMoment::testMsrMomentComparisons (ostream& os)
{
	msrMoment m0 (rational (3, 4));
	msrMoment m1 (rational (3, 4), rational (-1, 16));
	msrMoment m2 (rational (3, 4), rational (2, 16));

  os <<
    "m1: " << m1 << endl <<
    "m0: " << m0 << endl <<
    "m2: " << m2 << endl <<
    endl <<

    "m0 == m0: " << booleanAsString (m0 == m0) << endl <<
    "m1 == m1: " << booleanAsString (m1 == m1) << endl <<
    "m2 == m2: " << booleanAsString (m2 == m2) << endl <<
    endl <<

    "m0 != m0: " << booleanAsString (m0 != m0) << endl <<
    "m1 != m1: " << booleanAsString (m1 != m1) << endl <<
    "m2 != m2: " << booleanAsString (m2 != m2) << endl <<
    endl <<

    "m0 < m0: " << booleanAsString (m0 < m0) << endl <<
    "m1 < m1: " << booleanAsString (m1 < m1) << endl <<
    "m2 < m2: " << booleanAsString (m2 < m2) << endl <<
    endl <<

    "m0 <= m0: " << booleanAsString (m0 <= m0) << endl <<
    "m1 <= m1: " << booleanAsString (m1 <= m1) << endl <<
    "m2 <= m2: " << booleanAsString (m2 <= m2) << endl <<
    endl <<

    "m0 >= m0: " << booleanAsString (m0 >= m0) << endl <<
    "m1 >= m1: " << booleanAsString (m1 >= m1) << endl <<
    "m2 >= m2: " << booleanAsString (m2 >= m2) << endl <<
    endl <<

    "m0 > m0: " << booleanAsString (m0 > m0) << endl <<
    "m1 > m1: " << booleanAsString (m1 > m1) << endl <<
    "m2 > m2: " << booleanAsString (m2 > m2) << endl <<
    endl << endl <<

    "m1 == m0: " << booleanAsString (m1 == m0) << endl <<
    "m1 == m1: " << booleanAsString (m1 == m1) << endl <<
    "m1 == m2: " << booleanAsString (m1 == m2) << endl <<
    endl <<

    "m1 != m0: " << booleanAsString (m1 != m0) << endl <<
    "m1 != m1: " << booleanAsString (m1 != m1) << endl <<
    "m1 != m2: " << booleanAsString (m1 != m2) << endl <<
    endl <<

    "m1 < m0: " << booleanAsString (m1 < m0) << endl <<
    "m1 < m1: " << booleanAsString (m1 < m1) << endl <<
    "m1 < m2: " << booleanAsString (m1 < m2) << endl <<
    endl <<

    "m1 <= m0: " << booleanAsString (m1 <= m0) << endl <<
    "m1 <= m1: " << booleanAsString (m1 <= m1) << endl <<
    "m1 <= m2: " << booleanAsString (m1 <= m2) << endl <<
    endl <<

    "m1 >= m0: " << booleanAsString (m1 >= m0) << endl <<
    "m1 >= m1: " << booleanAsString (m1 >= m1) << endl <<
    "m1 >= m2: " << booleanAsString (m1 >= m2) << endl <<
    endl <<

    "m1 > m0: " << booleanAsString (m1 > m0) << endl <<
    "m1 > m1: " << booleanAsString (m1 > m1) << endl <<
    "m1 > m2: " << booleanAsString (m1 > m2) << endl <<
    endl <<

    endl;

/* output:
*/
}

bool msrMoment::operator== (const msrMoment& other) const
{
  bool result;

  if (fWrittenPositionInMeseasure != other.fWrittenPositionInMeseasure) {
    result = false;
  }
  else {
    result = fSoundingRelativeOffset == other.fSoundingRelativeOffset;
  }

  return result;
}

bool msrMoment::operator< (const msrMoment& other) const
{
  bool result;

  if (fWrittenPositionInMeseasure > other.fWrittenPositionInMeseasure) {
    result = false;
  }
  else if (fWrittenPositionInMeseasure == other.fWrittenPositionInMeseasure) {
    result = fSoundingRelativeOffset < other.fSoundingRelativeOffset;
  }
  else {
    result = true;
  }

  return result;
}

bool msrMoment::operator> (const msrMoment& other) const
{
  bool result;

  if (fWrittenPositionInMeseasure < other.fWrittenPositionInMeseasure) {
    result = false;
  }
  else if (fWrittenPositionInMeseasure == other.fWrittenPositionInMeseasure) {
    result = fSoundingRelativeOffset > other.fSoundingRelativeOffset;
  }
  else {
    result = true;
  }

  return result;
}

string msrMoment::asString () const
{
  stringstream s;

  s <<
    "[Moment [" <<
    fWrittenPositionInMeseasure << ", " << fSoundingRelativeOffset <<
    "]";

  return s.str ();
}

void msrMoment::print (ostream& os) const
{
  os <<
    "Moment" <<
    endl;

  gIndenter++;

  const int fieldWidth = 26;

  os << left <<
    setw (fieldWidth) <<
    "writtenPositionInMeseasure" << " : " << fWrittenPositionInMeseasure <<
    endl <<
    setw (fieldWidth) <<
    "soundingRelativeOffset" << " : " << fSoundingRelativeOffset <<
    endl;

  gIndenter--;
};

ostream& operator<< (ostream& os, const msrMoment& elt)
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

msrTupletFactor::~msrTupletFactor ()
{}

string msrTupletFactor::asString () const
{
  stringstream s;

  s <<
    "[TupletFactor" <<
    ", tupletActualNotes: " << fTupletActualNotes <<
    ", tupletNormalNotes: " << fTupletNormalNotes <<
    "]";

  return s.str ();
}

void msrTupletFactor::print (ostream& os) const
{
  os <<
    "TupletFactor" <<
    endl;

  gIndenter++;

  const int fieldWidth = 11;

  os << left <<
    setw (fieldWidth) <<
    "tupletActualNotes" << " : " << fTupletActualNotes <<
    endl <<
    setw (fieldWidth) <<
    "tupletNormalNotes" << " : " << fTupletNormalNotes <<
    endl;

  gIndenter--;
};

ostream& operator<< (ostream& os, const msrTupletFactor& elt)
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

        msrUnsupported (
          gGlobalOahOahGroup->getInputSourceName (),
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
            gGlobalMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
            quarterTonesPitchKind) <<
          "(" << quarterTonesPitchKind << ")" <<
          "' is not supported"
          ", line = " << inputLineNumber;

        msrUnsupported (
          gGlobalOahOahGroup->getInputSourceName (),
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

// clefs
//______________________________________________________________________________
map<string, msrClefKind>
  gGlobalClefKindsMap;

list<string>
  gClefKindsNamesList;

string clefKindAsString (
  msrClefKind clefKind)
{
  string result;

  switch (clefKind) {
    case k_NoClef:
      result = "none";
      break;
    case kTrebleClef:
      result = "treble";
      break;
    case kSopranoClef:
      result = "soprano";
      break;
    case kMezzoSopranoClef:
      result = "mezzo soprano";
      break;
    case kAltoClef:
      result = "alto";
      break;
    case kTenorClef:
      result = "tenor";
      break;
    case kBaritoneClef:
      result = "baritone";
      break;
    case kBassClef:
      result = "bass";
      break;
    case kTrebleLine1Clef:
      result = "treble line 1";
      break;
    case kTrebleMinus15Clef:
      result = "treble -15";
      break;
    case kTrebleMinus8Clef:
      result = "treble -8";
      break;
    case kTreblePlus8Clef:
      result = "treble +8";
      break;
    case kTreblePlus15Clef:
      result = "treble +15";
      break;
    case kBassMinus15Clef:
      result = "bass -15";
      break;
    case kBassMinus8Clef:
      result = "bass -8";
      break;
    case kBassPlus8Clef:
      result = "bass +8";
      break;
    case kBassPlus15Clef:
      result = "bass +15";
      break;
    case kVarbaritoneClef:
      result = "varbaritone";
      break;
    case kTablature4Clef:
      result = "tablature 4 lines";
      break;
    case kTablature5Clef:
      result = "tablature 5 lines";
      break;
    case kTablature6Clef:
      result = "tablature 6 lines";
      break;
    case kTablature7Clef:
      result = "tablature 7 lines";
      break;
    case kPercussionClef:
      result = "percussion";
      break;
    case kJianpuClef:
      result = "jiǎnpǔ";
      break;
  } // switch

  return result;
}

msrClefKind clefKindFromString (
  string theString)
{
  msrClefKind result = k_NoClef;

  if      (theString == "treble")
    result = kTrebleClef;
  else if (theString == "soprano")
    result = kSopranoClef;
  else if (theString == "mezzosoprano")
    result = kMezzoSopranoClef;
  else if (theString == "alto")
    result = kAltoClef;
  else if (theString == "tenor")
    result = kTenorClef;
  else if (theString == "baritone")
    result = kBaritoneClef;
  else if (theString == "bass")
    result = kBassClef;
  else if (theString == "treble1")
    result = kTrebleLine1Clef;
  else if (theString == "treble-15")
    result = kTrebleMinus15Clef;
  else if (theString == "treble-8")
    result = kTrebleMinus8Clef;
  else if (theString == "treble+8")
    result = kTreblePlus8Clef;
  else if (theString == "treble+15")
    result = kTreblePlus15Clef;
  else if (theString == "bass-15")
    result = kBassMinus15Clef;
  else if (theString == "bass-8")
    result = kBassMinus8Clef;
  else if (theString == "bass+8")
    result = kBassPlus8Clef;
  else if (theString == "bass+15")
    result = kBassPlus15Clef;
  else if (theString == "varbaritone")
    result = kVarbaritoneClef;
  else if (theString == "tab4")
    result = kTablature4Clef;
  else if (theString == "tab5")
    result = kTablature5Clef;
  else if (theString == "tab6")
    result = kTablature6Clef;
  else if (theString == "tab7")
    result = kTablature7Clef;
  else if (theString == "percussion")
    result = kPercussionClef;
  else if (theString == "jianpu")
    result = kJianpuClef;

  return result;
}

void initializeClefKinds ()
{
  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
  //  gGlobalClefKindsMap [gClefKindsNamesList.back ()] = kMajorClef;

    gClefKindsNamesList.push_back ("treble");
    gGlobalClefKindsMap ["treble"] = kTrebleClef;
    gClefKindsNamesList.push_back ("soprano");
    gGlobalClefKindsMap ["soprano"] = kSopranoClef;
    gClefKindsNamesList.push_back ("mezzosoprano");
    gGlobalClefKindsMap ["mezzosoprano"] = kMezzoSopranoClef;
    gClefKindsNamesList.push_back ("alto");
    gGlobalClefKindsMap ["alto"] = kAltoClef;
    gClefKindsNamesList.push_back ("tenor");
    gGlobalClefKindsMap ["tenor"] = kTenorClef;
    gClefKindsNamesList.push_back ("baritone");
    gGlobalClefKindsMap ["baritone"] = kBaritoneClef;
    gClefKindsNamesList.push_back ("bass");
    gGlobalClefKindsMap ["bass"] = kBassClef;
    gClefKindsNamesList.push_back ("treble1");
    gGlobalClefKindsMap ["treble1"] = kTrebleLine1Clef;
    gClefKindsNamesList.push_back ("treble-15");
    gGlobalClefKindsMap ["treble-15"] = kTrebleMinus15Clef;
    gClefKindsNamesList.push_back ("treble-8");
    gGlobalClefKindsMap ["treble-8"] = kTrebleMinus8Clef;
    gClefKindsNamesList.push_back ("treble+8");
    gGlobalClefKindsMap ["treble+8"] = kTreblePlus8Clef;
    gClefKindsNamesList.push_back ("treble+15");
    gGlobalClefKindsMap ["treble+15"] = kTreblePlus15Clef;
    gClefKindsNamesList.push_back ("bass-15");
    gGlobalClefKindsMap ["bass-15"] = kBassMinus15Clef;
    gClefKindsNamesList.push_back ("bass-8");
    gGlobalClefKindsMap ["bass-8"] = kBassMinus8Clef;
    gClefKindsNamesList.push_back ("bass+8");
    gGlobalClefKindsMap ["bass+8"] = kBassPlus8Clef;
    gClefKindsNamesList.push_back ("bass+15");
    gGlobalClefKindsMap ["bass+15"] = kBassPlus15Clef;
    gClefKindsNamesList.push_back ("varbaritone");
    gGlobalClefKindsMap ["varbaritone"] = kVarbaritoneClef;
    gClefKindsNamesList.push_back ("tab4");
    gGlobalClefKindsMap ["tab4"] = kTablature4Clef;
    gClefKindsNamesList.push_back ("tab5");
    gGlobalClefKindsMap ["tab5"] = kTablature5Clef;
    gClefKindsNamesList.push_back ("tab6");
    gGlobalClefKindsMap ["tab6"] = kTablature6Clef;
    gClefKindsNamesList.push_back ("tab7");
    gGlobalClefKindsMap ["tab7"] = kTablature7Clef;
    gClefKindsNamesList.push_back ("percussion");
    gGlobalClefKindsMap ["percussion"] = kPercussionClef;
    gClefKindsNamesList.push_back ("jianpu");
    gGlobalClefKindsMap ["jianpu"] = kJianpuClef;

    pThisMethodHasBeenRun = true;
  }
}

string existingClefKinds (int namesListMaxLength)
{
  stringstream s;

  int
    clefKindsMapSize =
      gGlobalClefKindsMap.size ();

  if (clefKindsMapSize) {
    int
      nextToLast =
        clefKindsMapSize - 1;

    int count = 0;
    int cumulatedLength = 0;

    for (
      map<string, msrClefKind>::const_iterator i =
        gGlobalClefKindsMap.begin ();
      i != gGlobalClefKindsMap.end ();
      i++
    ) {
      string theString = (*i).first;

      count++;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
      }

      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != clefKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

string existingClefKindsNames (int namesListMaxLength)
{
  stringstream s;

  int
    clefKindsNamesMapSize =
      gClefKindsNamesList.size ();

  if (clefKindsNamesMapSize) {
    int
      nextToLast =
        clefKindsNamesMapSize - 1;

    int count = 0;
    int cumulatedLength = 0;

    for (
      list<string>::const_iterator i =
        gClefKindsNamesList.begin ();
      i != gClefKindsNamesList.end ();
      i++
    ) {
      string theString = (*i);

      count++;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
      }

      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != clefKindsNamesMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

// keys
// ------------------------------------------------------

string keyKindAsString (
  msrKeyKind keyKind)
{
  string result;

  switch (keyKind) {
    case kTraditionalKind:
      result = "traditional";
      break;
    case kHumdrumScotKind:
      result = "Humdrum/Scot";
      break;
  } // switch

  return result;
}

string keyModeKindAsString (
  msrKeyModeKind keyModeKind)
{
  string result;

  switch (keyModeKind) {
    case kMajorMode:
      result = "major";
      break;
    case kMinorMode:
      result = "minor";
      break;
    case kIonianMode:
      result = "ionian";
      break;
    case kDorianMode:
      result = "dorian";
      break;
    case kPhrygianMode:
      result = "phrygian";
      break;
    case kLydianMode:
      result = "lydian";
      break;
    case kMixolydianMode:
      result = "mixolydian";
      break;
    case kAeolianMode:
      result = "aeolian";
      break;
    case kLocrianMode:
      result = "locrian";
      break;
  } // switch

  return result;
}

// times
//______________________________________________________________________________
string timeSymbolKindAsString (
  msrTimeSymbolKind timeSymbolKind)
{
  string result;

  switch (timeSymbolKind) {
    case kTimeSymbolCommon:
      result = "timeSymbolCommon";
      break;
    case kTimeSymbolCut:
      result = "timeSymbolCut";
      break;
    case kTimeSymbolNote:
      result = "timeSymbolNote";
      break;
    case kTimeSymbolDottedNote:
      result = "timeSymbolDottedNote";
      break;
    case kTimeSymbolSingleNumber:
      result = "timeSymbolSingleNumber";
      break;
    case kTimeSymbolSenzaMisura:
      result = "timeSymbolSenzaMisura";
      break;
    case kTimeSymbolNone:
      result = "timeSymbolNone";
      break;
  } // switch

  return result;
}

string timeSeparatorKindAsString (
  msrTimeSeparatorKind timeSeparatorKind)
{
  string result;

  switch (timeSeparatorKind) {
    case kTimeSeparatorNone:
      result = "timeSeparatorNone";
      break;
    case kTimeSeparatorHorizontal:
      result = "timeSeparatorHorizontal";
      break;
    case kTimeSeparatorDiagonal:
      result = "timeSeparatorDiagonal";
      break;
    case kTimeSeparatorVertical:
      result = "timeSeparatorVertical";
      break;
    case kTimeSeparatorAdjacent:
      result = "timeSeparatorAdjacent";
      break;
  } // switch

  return result;
}

string timeRelationKindAsString (
  msrTimeRelationKind timeRelationKind)
{
  string result;

  switch (timeRelationKind) {
    case kTimeRelationNone:
      result = "timeRelationNone";
      break;
    case kTimeRelationParentheses:
      result = "timeRelationParentheses";
      break;
    case kTimeRelationBracket:
      result = "timeRelationBracket";
      break;
    case kTimeRelationEquals:
      result = "timeRelationEquals";
      break;
    case kTimeRelationSlash:
      result = "timeRelationSlash";
      break;
    case kTimeRelationSpace:
      result = "timeRelationSpace";
      break;
    case kTimeRelationHyphen:
      result = "timeRelationHyphen";
      break;
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
      result = "noHarmony";
      break;

    // MusicXML harmonies

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

    // jazz-specific Harmonies

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

    // MusicXML harmonies

    case kMajorHarmony:
      result = "M";
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

    // jazz-specific Harmonies

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

    // MusicXML harmonies

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

    // jazz-specific Harmonies

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

  // MusicXML harmonies

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

  // jazz-specific Harmonies

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

map<string, msrHarmonyKind>
  gGlobalHarmonyKindsMap;

list<string>
  gHarmonyKindsNamesList;

void initializeHarmonyKinds ()
{
  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
    // MusicXML harmonies

    gHarmonyKindsNamesList.push_back ("maj"); // JMI
    gGlobalHarmonyKindsMap [gHarmonyKindsNamesList.back ()] = kMajorHarmony;
    gGlobalHarmonyKindsMap ["min"] = kMinorHarmony;
    gGlobalHarmonyKindsMap ["aug"] = kAugmentedHarmony;
    gGlobalHarmonyKindsMap ["dim"] = kDiminishedHarmony;
    gGlobalHarmonyKindsMap ["dom"] = kDominantHarmony;
    gGlobalHarmonyKindsMap ["maj7"] = kMajorSeventhHarmony;
    gGlobalHarmonyKindsMap ["min7"] = kMinorSeventhHarmony;
    gGlobalHarmonyKindsMap ["dim7"] = kDiminishedSeventhHarmony;
    gGlobalHarmonyKindsMap ["aug7"] = kAugmentedSeventhHarmony;
    gGlobalHarmonyKindsMap ["halfdim"] = kHalfDiminishedHarmony;
    gGlobalHarmonyKindsMap ["minmaj7"] = kMinorMajorSeventhHarmony;
    gGlobalHarmonyKindsMap ["maj6"] = kMajorSixthHarmony;
    gGlobalHarmonyKindsMap ["min6"] = kMinorSixthHarmony;
    gGlobalHarmonyKindsMap ["dom9"] = kDominantNinthHarmony;
    gGlobalHarmonyKindsMap ["maj9"] = kMajorNinthHarmony;
    gGlobalHarmonyKindsMap ["min9"] = kMinorNinthHarmony;
    gGlobalHarmonyKindsMap ["dom11"] = kDominantEleventhHarmony;
    gGlobalHarmonyKindsMap ["maj11"] = kMajorEleventhHarmony;
    gGlobalHarmonyKindsMap ["min11"] = kMinorEleventhHarmony;
    gGlobalHarmonyKindsMap ["dom13"] = kDominantThirteenthHarmony;
    gGlobalHarmonyKindsMap ["maj13"] = kMajorThirteenthHarmony;
    gGlobalHarmonyKindsMap ["min13"] = kMinorThirteenthHarmony;
    gGlobalHarmonyKindsMap ["sus2"] = kSuspendedSecondHarmony;
    gGlobalHarmonyKindsMap ["sus4"] = kSuspendedFourthHarmony;
    gGlobalHarmonyKindsMap ["neapolitan"] = kNeapolitanHarmony;
    gGlobalHarmonyKindsMap ["italian"] = kItalianHarmony;
    gGlobalHarmonyKindsMap ["french"] = kFrenchHarmony;
    gGlobalHarmonyKindsMap ["german"] = kGermanHarmony;
    gGlobalHarmonyKindsMap ["pedal"] = kPedalHarmony;
    gGlobalHarmonyKindsMap ["power"] = kPowerHarmony;
    gGlobalHarmonyKindsMap ["tristan"] = kTristanHarmony;

    // jazz-specific Harmonies

    gGlobalHarmonyKindsMap ["minmaj9"] = kMinorMajorNinth;
    gGlobalHarmonyKindsMap ["domsus4"] = kDominantSuspendedFourthHarmony;
    gGlobalHarmonyKindsMap ["domaug5"] = kDominantAugmentedFifthHarmony;
    gGlobalHarmonyKindsMap ["dommin9"] = kDominantMinorNinthHarmony;
    gGlobalHarmonyKindsMap ["domaug9dim5"] = kDominantAugmentedNinthDiminishedFifthHarmony;
    gGlobalHarmonyKindsMap ["domaug9aug5"] = kDominantAugmentedNinthAugmentedFifthHarmony;
    gGlobalHarmonyKindsMap ["domaug11"] = kDominantAugmentedEleventhHarmony;
    gGlobalHarmonyKindsMap ["maj7aug11"] = kMajorSeventhAugmentedEleventhHarmony;

    pThisMethodHasBeenRun = true;
  }
}

string existingHarmonyKinds (int namesListMaxLength)
{
  stringstream s;

  int
    harmonyKindsMapSize =
      gGlobalHarmonyKindsMap.size ();

  if (harmonyKindsMapSize) {
    int
      nextToLast =
        harmonyKindsMapSize - 1;

    int count = 0;
    int cumulatedLength = 0;

    for (
      map<string, msrHarmonyKind>::const_iterator i =
        gGlobalHarmonyKindsMap.begin ();
      i != gGlobalHarmonyKindsMap.end ();
      i++
    ) {
      string theString = (*i).first;

      count++;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
      }

      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != harmonyKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

string existingHarmonyKindsNames (int namesListMaxLength)
{
  stringstream s;

  int
    harmonyKindsNamesListSize =
      gHarmonyKindsNamesList.size ();

  if (harmonyKindsNamesListSize) {
    int
      nextToLast =
        harmonyKindsNamesListSize - 1;

    int count = 0;
    int cumulatedLength = 0;

    for (
      list<string>::const_iterator i =
        gHarmonyKindsNamesList.begin ();
      i != gHarmonyKindsNamesList.end ();
      i++
    ) {
      string theString = (*i);

      count++;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
      }

      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != harmonyKindsNamesListSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

void initializeHarmonyStructuresMap ()
{
  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
    for (int i = k_NoHarmony; i <= kNoneHarmony; i++) {
      msrHarmonyKind
        harmonyKind =
          msrHarmonyKind (i);

      // create the harmony structure
      S_msrHarmonyStructure
        harmonyStructure =
          msrHarmonyStructure::create (
            harmonyKind);

      // register it in the map
      gGlobalHarmonyStructuresMap [harmonyKind] =
        harmonyStructure;
    } // for

    pThisMethodHasBeenRun = true;
  }
}

void printHarmonyStructuresMap ()
{
  gLogStream <<
    "Harmonies harmonies structures:" <<
    " (" << gGlobalHarmonyStructuresMap.size () << ")" <<
    endl;

  gIndenter++;

  for (int i = k_NoHarmony; i <= kNoneHarmony; i++) {
    msrHarmonyKind
      harmonyKind =
        msrHarmonyKind (i);

    gLogStream <<
// JMI      "i:" << i << " " <<
      msrHarmonyKindAsString (harmonyKind) << ":" <<
      endl;

    gIndenter++;

    S_msrHarmonyStructure
      harmonyStructure =
        gGlobalHarmonyStructuresMap [harmonyKind];

    if (harmonyStructure) {
      gLogStream <<
        harmonyStructure <<
        endl;
    }
    else {
      gLogStream <<
        "no intervals" <<
        endl;
    }

    gIndenter--;

    gLogStream << endl;
  } // for

  gIndenter--;

  gLogStream << endl;
}

// notes names languages
// ------------------------------------------------------

map<string, msrQuarterTonesPitchesLanguageKind>
  gGlobalQuarterTonesPitchesLanguageKindsMap;

map<msrQuarterTonesPitchKind, string> gGlobalNederlandsPitchesNamesMap;
map<msrQuarterTonesPitchKind, string> gGlobalCatalanPitchesNamesMap;
map<msrQuarterTonesPitchKind, string> gGlobalDeutschPitchesNamesMap;
map<msrQuarterTonesPitchKind, string> gGlobalEnglishPitchesNamesMap;
map<msrQuarterTonesPitchKind, string> gGlobalEspanolPitchesNamesMap;
map<msrQuarterTonesPitchKind, string> gGlobalFrancaisPitchesNamesMap;
map<msrQuarterTonesPitchKind, string> gGlobalItalianoPitchesNamesMap;
map<msrQuarterTonesPitchKind, string> gGlobalNorskPitchesNamesMap;
map<msrQuarterTonesPitchKind, string> gGlobalPortuguesPitchesNamesMap;
map<msrQuarterTonesPitchKind, string> gGlobalSuomiPitchesNamesMap;
map<msrQuarterTonesPitchKind, string> gGlobalSvenskaPitchesNamesMap;
map<msrQuarterTonesPitchKind, string> gGlobalVlaamsPitchesNamesMap;

void initializeQuarterTonesPitchesLanguageKinds ()
{
  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
    gGlobalQuarterTonesPitchesLanguageKindsMap ["nederlands"] = kNederlands;
    gGlobalQuarterTonesPitchesLanguageKindsMap ["catalan"]    = kCatalan;
    gGlobalQuarterTonesPitchesLanguageKindsMap ["deutsch"]    = kDeutsch;
    gGlobalQuarterTonesPitchesLanguageKindsMap ["english"]    = kEnglish;
    gGlobalQuarterTonesPitchesLanguageKindsMap ["espanol"]    = kEspanol;
    gGlobalQuarterTonesPitchesLanguageKindsMap ["italiano"]   = kItaliano;
    gGlobalQuarterTonesPitchesLanguageKindsMap ["francais"]   = kFrancais;
    gGlobalQuarterTonesPitchesLanguageKindsMap ["norsk"]      = kNorsk;
    gGlobalQuarterTonesPitchesLanguageKindsMap ["portugues"]  = kPortugues;
    gGlobalQuarterTonesPitchesLanguageKindsMap ["suomi"]      = kSuomi;
    gGlobalQuarterTonesPitchesLanguageKindsMap ["svenska"]    = kSvenska;
    gGlobalQuarterTonesPitchesLanguageKindsMap ["vlaams"]     = kVlaams;

    initializeNederlandsPitchNamesMap ();
    initializeCatalanPitchNamesMap ();
    initializeDeutschPitchNamesMap ();
    initializeEnglishPitchNamesMap ();
    initializeEspanolPitchNamesMap ();
    initializeFrancaisPitchNamesMap ();
    initializeItalianoPitchNamesMap ();
    initializeNorskPitchNamesMap ();
    initializePortuguesPitchNamesMap ();
    initializeSuomiPitchNamesMap ();
    initializeSvenskaPitchNamesMap ();
    initializeVlaamsPitchNamesMap ();

    pThisMethodHasBeenRun = true;
  }
}

void initializeNederlandsPitchNamesMap ()
{
  // nederlands
  gGlobalNederlandsPitchesNamesMap [k_NoQuarterTonesPitch_QTP]  = "noQuarterTonePitch";
  gGlobalNederlandsPitchesNamesMap [k_Rest_QTP]                 = "r";

  gGlobalNederlandsPitchesNamesMap [kA_DoubleFlat_QTP]  = "aeses";
  gGlobalNederlandsPitchesNamesMap [kA_SesquiFlat_QTP]  = "aeseh";
  gGlobalNederlandsPitchesNamesMap [kA_Flat_QTP]        = "aes";
  gGlobalNederlandsPitchesNamesMap [kA_SemiFlat_QTP]    = "aeh";
  gGlobalNederlandsPitchesNamesMap [kA_Natural_QTP]     = "a";
  gGlobalNederlandsPitchesNamesMap [kA_SemiSharp_QTP]   = "aih";
  gGlobalNederlandsPitchesNamesMap [kA_Sharp_QTP]       = "ais";
  gGlobalNederlandsPitchesNamesMap [kA_SesquiSharp_QTP] = "aisih";
  gGlobalNederlandsPitchesNamesMap [kA_DoubleSharp_QTP] = "aisis";

  gGlobalNederlandsPitchesNamesMap [kB_DoubleFlat_QTP]  = "beses";
  gGlobalNederlandsPitchesNamesMap [kB_SesquiFlat_QTP]  = "beseh";
  gGlobalNederlandsPitchesNamesMap [kB_Flat_QTP]        = "bes";
  gGlobalNederlandsPitchesNamesMap [kB_SemiFlat_QTP]    = "beh";
  gGlobalNederlandsPitchesNamesMap [kB_Natural_QTP]     = "b";
  gGlobalNederlandsPitchesNamesMap [kB_SemiSharp_QTP]   = "bih";
  gGlobalNederlandsPitchesNamesMap [kB_Sharp_QTP]       = "bis";
  gGlobalNederlandsPitchesNamesMap [kB_SesquiSharp_QTP] = "bisih";
  gGlobalNederlandsPitchesNamesMap [kB_DoubleSharp_QTP] = "bisis";

  gGlobalNederlandsPitchesNamesMap [kC_DoubleFlat_QTP]  = "ceses";
  gGlobalNederlandsPitchesNamesMap [kC_SesquiFlat_QTP]  = "ceseh";
  gGlobalNederlandsPitchesNamesMap [kC_Flat_QTP]        = "ces";
  gGlobalNederlandsPitchesNamesMap [kC_SemiFlat_QTP]    = "ceh";
  gGlobalNederlandsPitchesNamesMap [kC_Natural_QTP]     = "c";
  gGlobalNederlandsPitchesNamesMap [kC_SemiSharp_QTP]   = "cih";
  gGlobalNederlandsPitchesNamesMap [kC_Sharp_QTP]       = "cis";
  gGlobalNederlandsPitchesNamesMap [kC_SesquiSharp_QTP] = "cisih";
  gGlobalNederlandsPitchesNamesMap [kC_DoubleSharp_QTP] = "cisis";

  gGlobalNederlandsPitchesNamesMap [kD_DoubleFlat_QTP]  = "deses";
  gGlobalNederlandsPitchesNamesMap [kD_SesquiFlat_QTP]  = "deseh";
  gGlobalNederlandsPitchesNamesMap [kD_Flat_QTP]        = "des";
  gGlobalNederlandsPitchesNamesMap [kD_SemiFlat_QTP]    = "deh";
  gGlobalNederlandsPitchesNamesMap [kD_Natural_QTP]     = "d";
  gGlobalNederlandsPitchesNamesMap [kD_SemiSharp_QTP]   = "dih";
  gGlobalNederlandsPitchesNamesMap [kD_Sharp_QTP]       = "dis";
  gGlobalNederlandsPitchesNamesMap [kD_SesquiSharp_QTP] = "disih";
  gGlobalNederlandsPitchesNamesMap [kD_DoubleSharp_QTP] = "disis";

  gGlobalNederlandsPitchesNamesMap [kE_DoubleFlat_QTP]  = "eeses";
  gGlobalNederlandsPitchesNamesMap [kE_SesquiFlat_QTP]  = "eeseh";
  gGlobalNederlandsPitchesNamesMap [kE_Flat_QTP]        = "ees";
  gGlobalNederlandsPitchesNamesMap [kE_SemiFlat_QTP]    = "eeh";
  gGlobalNederlandsPitchesNamesMap [kE_Natural_QTP]     = "e";
  gGlobalNederlandsPitchesNamesMap [kE_SemiSharp_QTP]   = "eih";
  gGlobalNederlandsPitchesNamesMap [kE_Sharp_QTP]       = "eis";
  gGlobalNederlandsPitchesNamesMap [kE_SesquiSharp_QTP] = "eisih";
  gGlobalNederlandsPitchesNamesMap [kE_DoubleSharp_QTP] = "eisis";

  gGlobalNederlandsPitchesNamesMap [kF_DoubleFlat_QTP]  = "feses";
  gGlobalNederlandsPitchesNamesMap [kF_SesquiFlat_QTP]  = "feseh";
  gGlobalNederlandsPitchesNamesMap [kF_Flat_QTP]        = "fes";
  gGlobalNederlandsPitchesNamesMap [kF_SemiFlat_QTP]    = "feh";
  gGlobalNederlandsPitchesNamesMap [kF_Natural_QTP]     = "f";
  gGlobalNederlandsPitchesNamesMap [kF_SemiSharp_QTP]   = "fih";
  gGlobalNederlandsPitchesNamesMap [kF_Sharp_QTP]       = "fis";
  gGlobalNederlandsPitchesNamesMap [kF_SesquiSharp_QTP] = "fisih";
  gGlobalNederlandsPitchesNamesMap [kF_DoubleSharp_QTP] = "fisis";

  gGlobalNederlandsPitchesNamesMap [kG_DoubleFlat_QTP]  = "geses";
  gGlobalNederlandsPitchesNamesMap [kG_SesquiFlat_QTP]  = "geseh";
  gGlobalNederlandsPitchesNamesMap [kG_Flat_QTP]        = "ges";
  gGlobalNederlandsPitchesNamesMap [kG_SemiFlat_QTP]    = "geh";
  gGlobalNederlandsPitchesNamesMap [kG_Natural_QTP]     = "g";
  gGlobalNederlandsPitchesNamesMap [kG_SemiSharp_QTP]   = "gih";
  gGlobalNederlandsPitchesNamesMap [kG_Sharp_QTP]       = "gis";
  gGlobalNederlandsPitchesNamesMap [kG_SesquiSharp_QTP] = "gisih";
  gGlobalNederlandsPitchesNamesMap [kG_DoubleSharp_QTP] = "gisis";
}

void initializeCatalanPitchNamesMap ()
{
  // catalan
  gGlobalCatalanPitchesNamesMap [k_NoQuarterTonesPitch_QTP]  = "noQuarterTonePitch";
  gGlobalCatalanPitchesNamesMap [k_Rest_QTP]                 = "r";

  gGlobalCatalanPitchesNamesMap [kA_DoubleFlat_QTP]  = "labb";
  gGlobalCatalanPitchesNamesMap [kA_SesquiFlat_QTP]  = "labSesquiFlat???";
  gGlobalCatalanPitchesNamesMap [kA_Flat_QTP]        = "lab";
  gGlobalCatalanPitchesNamesMap [kA_SemiFlat_QTP]    = "aSemiFlat???";
  gGlobalCatalanPitchesNamesMap [kA_Natural_QTP]     = "la";
  gGlobalCatalanPitchesNamesMap [kA_SemiSharp_QTP]   = "aSemiSharp???";
  gGlobalCatalanPitchesNamesMap [kA_Sharp_QTP]       = "lad";
  gGlobalCatalanPitchesNamesMap [kA_SesquiSharp_QTP] = "laSesquiSharp???";
  gGlobalCatalanPitchesNamesMap [kA_DoubleSharp_QTP] = "ladd";

  gGlobalCatalanPitchesNamesMap [kB_DoubleFlat_QTP]  = "sibb";
  gGlobalCatalanPitchesNamesMap [kB_SesquiFlat_QTP]  = "sibSesquiFlat???";
  gGlobalCatalanPitchesNamesMap [kB_Flat_QTP]        = "sib";
  gGlobalCatalanPitchesNamesMap [kB_SemiFlat_QTP]    = "bSemiFlat???";
  gGlobalCatalanPitchesNamesMap [kB_Natural_QTP]     = "b";
  gGlobalCatalanPitchesNamesMap [kB_SemiSharp_QTP]   = "bSemiSharp???";
  gGlobalCatalanPitchesNamesMap [kB_Sharp_QTP]       = "sid";
  gGlobalCatalanPitchesNamesMap [kB_SesquiSharp_QTP] = "siSesquiSharp???";
  gGlobalCatalanPitchesNamesMap [kB_DoubleSharp_QTP] = "sidd";

  gGlobalCatalanPitchesNamesMap [kC_DoubleFlat_QTP]  = "dobb";
  gGlobalCatalanPitchesNamesMap [kC_SesquiFlat_QTP]  = "doSesquiFlat???";
  gGlobalCatalanPitchesNamesMap [kC_Flat_QTP]        = "dob";
  gGlobalCatalanPitchesNamesMap [kC_SemiFlat_QTP]    = "cSemiFlat???";
  gGlobalCatalanPitchesNamesMap [kC_Natural_QTP]     = "do";
  gGlobalCatalanPitchesNamesMap [kC_SemiSharp_QTP]   = "cSemiSharp???";
  gGlobalCatalanPitchesNamesMap [kC_Sharp_QTP]       = "dod";
  gGlobalCatalanPitchesNamesMap [kC_SesquiSharp_QTP] = "doSesquiSharp???";
  gGlobalCatalanPitchesNamesMap [kC_DoubleSharp_QTP] = "dodd";

  gGlobalCatalanPitchesNamesMap [kD_DoubleFlat_QTP]  = "rebb";
  gGlobalCatalanPitchesNamesMap [kD_SesquiFlat_QTP]  = "reSesquiFlat???";
  gGlobalCatalanPitchesNamesMap [kD_Flat_QTP]        = "reb";
  gGlobalCatalanPitchesNamesMap [kD_SemiFlat_QTP]    = "dSemiFlat???";
  gGlobalCatalanPitchesNamesMap [kD_Natural_QTP]     = "re";
  gGlobalCatalanPitchesNamesMap [kD_SemiSharp_QTP]   = "dSemiSharp???";
  gGlobalCatalanPitchesNamesMap [kD_Sharp_QTP]       = "red";
  gGlobalCatalanPitchesNamesMap [kD_SesquiSharp_QTP] = "reSesquiSharp???";
  gGlobalCatalanPitchesNamesMap [kD_DoubleSharp_QTP] = "redd";

  gGlobalCatalanPitchesNamesMap [kE_DoubleFlat_QTP]  = "mibb";
  gGlobalCatalanPitchesNamesMap [kE_SesquiFlat_QTP]  = "miSesquiFlat???";
  gGlobalCatalanPitchesNamesMap [kE_Flat_QTP]        = "mib";
  gGlobalCatalanPitchesNamesMap [kE_SemiFlat_QTP]    = "eSemiFlat???";
  gGlobalCatalanPitchesNamesMap [kE_Natural_QTP]     = "mi";
  gGlobalCatalanPitchesNamesMap [kE_SemiSharp_QTP]   = "eSemiSharp???";
  gGlobalCatalanPitchesNamesMap [kE_Sharp_QTP]       = "mid";
  gGlobalCatalanPitchesNamesMap [kE_SesquiSharp_QTP] = "miSesquiSharp???";
  gGlobalCatalanPitchesNamesMap [kE_DoubleSharp_QTP] = "midd";

  gGlobalCatalanPitchesNamesMap [kF_DoubleFlat_QTP]  = "fabb";
  gGlobalCatalanPitchesNamesMap [kF_SesquiFlat_QTP]  = "faSesquiFlat???";
  gGlobalCatalanPitchesNamesMap [kF_Flat_QTP]        = "fab";
  gGlobalCatalanPitchesNamesMap [kF_SemiFlat_QTP]    = "fSemiFlat???";
  gGlobalCatalanPitchesNamesMap [kF_Natural_QTP]     = "fa";
  gGlobalCatalanPitchesNamesMap [kF_SemiSharp_QTP]   = "fSemiSharp???";
  gGlobalCatalanPitchesNamesMap [kF_Sharp_QTP]       = "fad";
  gGlobalCatalanPitchesNamesMap [kF_SesquiSharp_QTP] = "faSesquiSharp???";
  gGlobalCatalanPitchesNamesMap [kF_DoubleSharp_QTP] = "fadd";

  gGlobalCatalanPitchesNamesMap [kG_DoubleFlat_QTP]  = "solbb";
  gGlobalCatalanPitchesNamesMap [kG_SesquiFlat_QTP]  = "solSesquiFlat???";
  gGlobalCatalanPitchesNamesMap [kG_Flat_QTP]        = "solb";
  gGlobalCatalanPitchesNamesMap [kG_SemiFlat_QTP]    = "gSemiFlat???";
  gGlobalCatalanPitchesNamesMap [kG_Natural_QTP]     = "sol";
  gGlobalCatalanPitchesNamesMap [kG_SemiSharp_QTP]   = "gSemiSharp???";
  gGlobalCatalanPitchesNamesMap [kG_Sharp_QTP]       = "sold";
  gGlobalCatalanPitchesNamesMap [kG_SesquiSharp_QTP] = "solSesquiSharp???";
  gGlobalCatalanPitchesNamesMap [kG_DoubleSharp_QTP] = "soldd";
}

void initializeDeutschPitchNamesMap ()
{
  // deutsch
  gGlobalDeutschPitchesNamesMap [k_NoQuarterTonesPitch_QTP]  = "noQuarterTonePitch";
  gGlobalDeutschPitchesNamesMap [k_Rest_QTP]                 = "r";

  gGlobalDeutschPitchesNamesMap [kA_DoubleFlat_QTP]  = "asas";
  gGlobalDeutschPitchesNamesMap [kA_SesquiFlat_QTP]  = "asah";
  gGlobalDeutschPitchesNamesMap [kA_Flat_QTP]        = "as";
  gGlobalDeutschPitchesNamesMap [kA_SemiFlat_QTP]    = "aeh";
  gGlobalDeutschPitchesNamesMap [kA_Natural_QTP]     = "a";
  gGlobalDeutschPitchesNamesMap [kA_SemiSharp_QTP]   = "aih";
  gGlobalDeutschPitchesNamesMap [kA_Sharp_QTP]       = "ais";
  gGlobalDeutschPitchesNamesMap [kA_SesquiSharp_QTP] = "aisih";
  gGlobalDeutschPitchesNamesMap [kA_DoubleSharp_QTP] = "aisis";

  gGlobalDeutschPitchesNamesMap [kB_DoubleFlat_QTP]  = "heses";
  gGlobalDeutschPitchesNamesMap [kB_SesquiFlat_QTP]  = "heseh";
  gGlobalDeutschPitchesNamesMap [kB_Flat_QTP]        = "b";
  gGlobalDeutschPitchesNamesMap [kB_SemiFlat_QTP]    = "beh";
  gGlobalDeutschPitchesNamesMap [kB_Natural_QTP]     = "h";
  gGlobalDeutschPitchesNamesMap [kB_SemiSharp_QTP]   = "hih";
  gGlobalDeutschPitchesNamesMap [kB_Sharp_QTP]       = "his";
  gGlobalDeutschPitchesNamesMap [kB_SesquiSharp_QTP] = "hisih";
  gGlobalDeutschPitchesNamesMap [kB_DoubleSharp_QTP] = "hisis";

  gGlobalDeutschPitchesNamesMap [kC_DoubleFlat_QTP]  = "ceses";
  gGlobalDeutschPitchesNamesMap [kC_SesquiFlat_QTP]  = "ceseh";
  gGlobalDeutschPitchesNamesMap [kC_Flat_QTP]        = "ces";
  gGlobalDeutschPitchesNamesMap [kC_SemiFlat_QTP]    = "ceh";
  gGlobalDeutschPitchesNamesMap [kC_Natural_QTP]     = "c";
  gGlobalDeutschPitchesNamesMap [kC_SemiSharp_QTP]   = "cih";
  gGlobalDeutschPitchesNamesMap [kC_Sharp_QTP]       = "cis";
  gGlobalDeutschPitchesNamesMap [kC_SesquiSharp_QTP] = "cisih";
  gGlobalDeutschPitchesNamesMap [kC_DoubleSharp_QTP] = "cisis";

  gGlobalDeutschPitchesNamesMap [kD_DoubleFlat_QTP]  = "deses";
  gGlobalDeutschPitchesNamesMap [kD_SesquiFlat_QTP]  = "deseh";
  gGlobalDeutschPitchesNamesMap [kD_Flat_QTP]        = "des";
  gGlobalDeutschPitchesNamesMap [kD_SemiFlat_QTP]    = "deh";
  gGlobalDeutschPitchesNamesMap [kD_Natural_QTP]     = "d";
  gGlobalDeutschPitchesNamesMap [kD_SemiSharp_QTP]   = "dih";
  gGlobalDeutschPitchesNamesMap [kD_Sharp_QTP]       = "dis";
  gGlobalDeutschPitchesNamesMap [kD_SesquiSharp_QTP] = "disih";
  gGlobalDeutschPitchesNamesMap [kD_DoubleSharp_QTP] = "disis";

  gGlobalDeutschPitchesNamesMap [kE_DoubleFlat_QTP]  = "eses";
  gGlobalDeutschPitchesNamesMap [kE_SesquiFlat_QTP]  = "esseh";
  gGlobalDeutschPitchesNamesMap [kE_Flat_QTP]        = "es";
  gGlobalDeutschPitchesNamesMap [kE_SemiFlat_QTP]    = "eeh";
  gGlobalDeutschPitchesNamesMap [kE_Natural_QTP]     = "e";
  gGlobalDeutschPitchesNamesMap [kE_SemiSharp_QTP]   = "eih";
  gGlobalDeutschPitchesNamesMap [kE_Sharp_QTP]       = "eis";
  gGlobalDeutschPitchesNamesMap [kE_SesquiSharp_QTP] = "eisih";
  gGlobalDeutschPitchesNamesMap [kE_DoubleSharp_QTP] = "eisis";

  gGlobalDeutschPitchesNamesMap [kF_DoubleFlat_QTP]  = "feses";
  gGlobalDeutschPitchesNamesMap [kF_SesquiFlat_QTP]  = "feseh";
  gGlobalDeutschPitchesNamesMap [kF_Flat_QTP]        = "fes";
  gGlobalDeutschPitchesNamesMap [kF_SemiFlat_QTP]    = "feh";
  gGlobalDeutschPitchesNamesMap [kF_Natural_QTP]     = "f";
  gGlobalDeutschPitchesNamesMap [kF_SemiSharp_QTP]   = "fih";
  gGlobalDeutschPitchesNamesMap [kF_Sharp_QTP]       = "fis";
  gGlobalDeutschPitchesNamesMap [kF_SesquiSharp_QTP] = "fisih";
  gGlobalDeutschPitchesNamesMap [kF_DoubleSharp_QTP] = "fisis";

  gGlobalDeutschPitchesNamesMap [kG_DoubleFlat_QTP]  = "geses";
  gGlobalDeutschPitchesNamesMap [kG_SesquiFlat_QTP]  = "geseh";
  gGlobalDeutschPitchesNamesMap [kG_Flat_QTP]        = "ges";
  gGlobalDeutschPitchesNamesMap [kG_SemiFlat_QTP]    = "geh";
  gGlobalDeutschPitchesNamesMap [kG_Natural_QTP]     = "g";
  gGlobalDeutschPitchesNamesMap [kG_SemiSharp_QTP]   = "gih";
  gGlobalDeutschPitchesNamesMap [kG_Sharp_QTP]       = "gis";
  gGlobalDeutschPitchesNamesMap [kG_SesquiSharp_QTP] = "gisih";
  gGlobalDeutschPitchesNamesMap [kG_DoubleSharp_QTP] = "gisis";
}

void initializeEnglishPitchNamesMap ()
{
  // english
  gGlobalEnglishPitchesNamesMap [k_NoQuarterTonesPitch_QTP]  = "noQuarterTonePitch";
  gGlobalEnglishPitchesNamesMap [k_Rest_QTP]                 = "r";

  gGlobalEnglishPitchesNamesMap [kA_DoubleFlat_QTP]  = "aff";
  gGlobalEnglishPitchesNamesMap [kA_SesquiFlat_QTP]  = "atqf";
  gGlobalEnglishPitchesNamesMap [kA_Flat_QTP]        = "af";
  gGlobalEnglishPitchesNamesMap [kA_SemiFlat_QTP]    = "aqf";
  gGlobalEnglishPitchesNamesMap [kA_Natural_QTP]     = "a";
  gGlobalEnglishPitchesNamesMap [kA_SemiSharp_QTP]   = "aqs";
  gGlobalEnglishPitchesNamesMap [kA_Sharp_QTP]       = "as";
  gGlobalEnglishPitchesNamesMap [kA_SesquiSharp_QTP] = "atqs";
  gGlobalEnglishPitchesNamesMap [kA_DoubleSharp_QTP] = "ax";

  gGlobalEnglishPitchesNamesMap [kB_DoubleFlat_QTP]  = "bfqf";
  gGlobalEnglishPitchesNamesMap [kB_SesquiFlat_QTP]  = "btqf";
  gGlobalEnglishPitchesNamesMap [kB_Flat_QTP]        = "bf";
  gGlobalEnglishPitchesNamesMap [kB_SemiFlat_QTP]    = "bqf";
  gGlobalEnglishPitchesNamesMap [kB_Natural_QTP]     = "b";
  gGlobalEnglishPitchesNamesMap [kB_SemiSharp_QTP]   = "bqs";
  gGlobalEnglishPitchesNamesMap [kB_Sharp_QTP]       = "bs";
  gGlobalEnglishPitchesNamesMap [kB_SesquiSharp_QTP] = "btqs";
  gGlobalEnglishPitchesNamesMap [kB_DoubleSharp_QTP] = "bx";

  gGlobalEnglishPitchesNamesMap [kC_DoubleFlat_QTP]  = "cff";
  gGlobalEnglishPitchesNamesMap [kC_SesquiFlat_QTP]  = "ctqf";
  gGlobalEnglishPitchesNamesMap [kC_Flat_QTP]        = "cf";
  gGlobalEnglishPitchesNamesMap [kC_SemiFlat_QTP]    = "cqf";
  gGlobalEnglishPitchesNamesMap [kC_Natural_QTP]     = "c";
  gGlobalEnglishPitchesNamesMap [kC_SemiSharp_QTP]   = "cqs";
  gGlobalEnglishPitchesNamesMap [kC_Sharp_QTP]       = "cs";
  gGlobalEnglishPitchesNamesMap [kC_SesquiSharp_QTP] = "ctqs";
  gGlobalEnglishPitchesNamesMap [kC_DoubleSharp_QTP] = "cx";

  gGlobalEnglishPitchesNamesMap [kD_DoubleFlat_QTP]  = "dff";
  gGlobalEnglishPitchesNamesMap [kD_SesquiFlat_QTP]  = "dtqf";
  gGlobalEnglishPitchesNamesMap [kD_Flat_QTP]        = "df";
  gGlobalEnglishPitchesNamesMap [kD_SemiFlat_QTP]    = "dqf";
  gGlobalEnglishPitchesNamesMap [kD_Natural_QTP]     = "d";
  gGlobalEnglishPitchesNamesMap [kD_SemiSharp_QTP]   = "dqs";
  gGlobalEnglishPitchesNamesMap [kD_Sharp_QTP]       = "ds";
  gGlobalEnglishPitchesNamesMap [kD_SesquiSharp_QTP] = "dtqs";
  gGlobalEnglishPitchesNamesMap [kD_DoubleSharp_QTP] = "dx";

  gGlobalEnglishPitchesNamesMap [kE_DoubleFlat_QTP]  = "eff";
  gGlobalEnglishPitchesNamesMap [kE_SesquiFlat_QTP]  = "etqf";
  gGlobalEnglishPitchesNamesMap [kE_Flat_QTP]        = "ef";
  gGlobalEnglishPitchesNamesMap [kE_SemiFlat_QTP]    = "eqf";
  gGlobalEnglishPitchesNamesMap [kE_Natural_QTP]     = "e";
  gGlobalEnglishPitchesNamesMap [kE_SemiSharp_QTP]   = "eqs";
  gGlobalEnglishPitchesNamesMap [kE_Sharp_QTP]       = "es";
  gGlobalEnglishPitchesNamesMap [kE_SesquiSharp_QTP] = "etqs";
  gGlobalEnglishPitchesNamesMap [kE_DoubleSharp_QTP] = "ex";

  gGlobalEnglishPitchesNamesMap [kF_DoubleFlat_QTP]  = "fff";
  gGlobalEnglishPitchesNamesMap [kF_SesquiFlat_QTP]  = "ftqf";
  gGlobalEnglishPitchesNamesMap [kF_Flat_QTP]        = "ff";
  gGlobalEnglishPitchesNamesMap [kF_SemiFlat_QTP]    = "fqf";
  gGlobalEnglishPitchesNamesMap [kF_Natural_QTP]     = "f";
  gGlobalEnglishPitchesNamesMap [kF_SemiSharp_QTP]   = "fqs";
  gGlobalEnglishPitchesNamesMap [kF_Sharp_QTP]       = "fs";
  gGlobalEnglishPitchesNamesMap [kF_SesquiSharp_QTP] = "ftqs";
  gGlobalEnglishPitchesNamesMap [kF_DoubleSharp_QTP] = "fx";

  gGlobalEnglishPitchesNamesMap [kG_DoubleFlat_QTP]  = "gff";
  gGlobalEnglishPitchesNamesMap [kG_SesquiFlat_QTP]  = "gtqf";
  gGlobalEnglishPitchesNamesMap [kG_Flat_QTP]        = "gf";
  gGlobalEnglishPitchesNamesMap [kG_SemiFlat_QTP]    = "gqf";
  gGlobalEnglishPitchesNamesMap [kG_Natural_QTP]     = "g";
  gGlobalEnglishPitchesNamesMap [kG_SemiSharp_QTP]   = "gqs";
  gGlobalEnglishPitchesNamesMap [kG_Sharp_QTP]       = "gs";
  gGlobalEnglishPitchesNamesMap [kG_SesquiSharp_QTP] = "gtqs";
  gGlobalEnglishPitchesNamesMap [kG_DoubleSharp_QTP] = "gx";
}

void initializeEspanolPitchNamesMap ()
{
  // espanol
  gGlobalEspanolPitchesNamesMap [k_NoQuarterTonesPitch_QTP]  = "noQuarterTonePitch";
  gGlobalEspanolPitchesNamesMap [k_Rest_QTP]                 = "r";

  gGlobalEspanolPitchesNamesMap [kA_DoubleFlat_QTP]  = "labb";
  gGlobalEspanolPitchesNamesMap [kA_SesquiFlat_QTP]  = "latcb";
  gGlobalEspanolPitchesNamesMap [kA_Flat_QTP]        = "lab";
  gGlobalEspanolPitchesNamesMap [kA_SemiFlat_QTP]    = "lacb";
  gGlobalEspanolPitchesNamesMap [kA_Natural_QTP]     = "la";
  gGlobalEspanolPitchesNamesMap [kA_SemiSharp_QTP]   = "lacs";
  gGlobalEspanolPitchesNamesMap [kA_Sharp_QTP]       = "las";
  gGlobalEspanolPitchesNamesMap [kA_SesquiSharp_QTP] = "latcs";
  gGlobalEspanolPitchesNamesMap [kA_DoubleSharp_QTP] = "lax";

  gGlobalEspanolPitchesNamesMap [kB_DoubleFlat_QTP]  = "sibb";
  gGlobalEspanolPitchesNamesMap [kB_SesquiFlat_QTP]  = "sitcb";
  gGlobalEspanolPitchesNamesMap [kB_Flat_QTP]        = "sib";
  gGlobalEspanolPitchesNamesMap [kB_SemiFlat_QTP]    = "sicb";
  gGlobalEspanolPitchesNamesMap [kB_Natural_QTP]     = "si";
  gGlobalEspanolPitchesNamesMap [kB_SemiSharp_QTP]   = "sics";
  gGlobalEspanolPitchesNamesMap [kB_Sharp_QTP]       = "sis";
  gGlobalEspanolPitchesNamesMap [kB_SesquiSharp_QTP] = "sitcs";
  gGlobalEspanolPitchesNamesMap [kB_DoubleSharp_QTP] = "six";

  gGlobalEspanolPitchesNamesMap [kC_DoubleFlat_QTP]  = "dobb";
  gGlobalEspanolPitchesNamesMap [kC_SesquiFlat_QTP]  = "dotcb";
  gGlobalEspanolPitchesNamesMap [kC_Flat_QTP]        = "dob";
  gGlobalEspanolPitchesNamesMap [kC_SemiFlat_QTP]    = "docb";
  gGlobalEspanolPitchesNamesMap [kC_Natural_QTP]     = "do";
  gGlobalEspanolPitchesNamesMap [kC_SemiSharp_QTP]   = "docs";
  gGlobalEspanolPitchesNamesMap [kC_Sharp_QTP]       = "dos";
  gGlobalEspanolPitchesNamesMap [kC_SesquiSharp_QTP] = "dotcs";
  gGlobalEspanolPitchesNamesMap [kC_DoubleSharp_QTP] = "dox";

  gGlobalEspanolPitchesNamesMap [kD_DoubleFlat_QTP]  = "rebb";
  gGlobalEspanolPitchesNamesMap [kD_SesquiFlat_QTP]  = "retcb";
  gGlobalEspanolPitchesNamesMap [kD_Flat_QTP]        = "reb";
  gGlobalEspanolPitchesNamesMap [kD_SemiFlat_QTP]    = "recb";
  gGlobalEspanolPitchesNamesMap [kD_Natural_QTP]     = "re";
  gGlobalEspanolPitchesNamesMap [kD_SemiSharp_QTP]   = "recs";
  gGlobalEspanolPitchesNamesMap [kD_Sharp_QTP]       = "res";
  gGlobalEspanolPitchesNamesMap [kD_SesquiSharp_QTP] = "retcs";
  gGlobalEspanolPitchesNamesMap [kD_DoubleSharp_QTP] = "rex";

  gGlobalEspanolPitchesNamesMap [kE_DoubleFlat_QTP]  = "mibb";
  gGlobalEspanolPitchesNamesMap [kE_SesquiFlat_QTP]  = "mitcb";
  gGlobalEspanolPitchesNamesMap [kE_Flat_QTP]        = "mib";
  gGlobalEspanolPitchesNamesMap [kE_SemiFlat_QTP]    = "micb";
  gGlobalEspanolPitchesNamesMap [kE_Natural_QTP]     = "mi";
  gGlobalEspanolPitchesNamesMap [kE_SemiSharp_QTP]   = "mics";
  gGlobalEspanolPitchesNamesMap [kE_Sharp_QTP]       = "mis";
  gGlobalEspanolPitchesNamesMap [kE_SesquiSharp_QTP] = "mitcs";
  gGlobalEspanolPitchesNamesMap [kE_DoubleSharp_QTP] = "mix";

  gGlobalEspanolPitchesNamesMap [kF_DoubleFlat_QTP]  = "fabb";
  gGlobalEspanolPitchesNamesMap [kF_SesquiFlat_QTP]  = "fatcb";
  gGlobalEspanolPitchesNamesMap [kF_Flat_QTP]        = "fab";
  gGlobalEspanolPitchesNamesMap [kF_SemiFlat_QTP]    = "facb";
  gGlobalEspanolPitchesNamesMap [kF_Natural_QTP]     = "fa";
  gGlobalEspanolPitchesNamesMap [kF_SemiSharp_QTP]   = "facs";
  gGlobalEspanolPitchesNamesMap [kF_Sharp_QTP]       = "fas";
  gGlobalEspanolPitchesNamesMap [kF_SesquiSharp_QTP] = "fatcs";
  gGlobalEspanolPitchesNamesMap [kF_DoubleSharp_QTP] = "fax";

  gGlobalEspanolPitchesNamesMap [kG_DoubleFlat_QTP]  = "solbb";
  gGlobalEspanolPitchesNamesMap [kG_SesquiFlat_QTP]  = "soltcb";
  gGlobalEspanolPitchesNamesMap [kG_Flat_QTP]        = "solb";
  gGlobalEspanolPitchesNamesMap [kG_SemiFlat_QTP]    = "solcb";
  gGlobalEspanolPitchesNamesMap [kG_Natural_QTP]     = "sol";
  gGlobalEspanolPitchesNamesMap [kG_SemiSharp_QTP]   = "solcs";
  gGlobalEspanolPitchesNamesMap [kG_Sharp_QTP]       = "sols";
  gGlobalEspanolPitchesNamesMap [kG_SesquiSharp_QTP] = "soltcs";
  gGlobalEspanolPitchesNamesMap [kG_DoubleSharp_QTP] = "solx";
}

void initializeFrancaisPitchNamesMap ()
{
  // francais
  gGlobalFrancaisPitchesNamesMap [k_NoQuarterTonesPitch_QTP]  = "noQuarterTonePitch";
  gGlobalFrancaisPitchesNamesMap [k_Rest_QTP]                 = "r";

  gGlobalFrancaisPitchesNamesMap [kA_DoubleFlat_QTP]  = "labb";
  gGlobalFrancaisPitchesNamesMap [kA_SesquiFlat_QTP]  = "labtqt";
  gGlobalFrancaisPitchesNamesMap [kA_Flat_QTP]        = "lab";
  gGlobalFrancaisPitchesNamesMap [kA_SemiFlat_QTP]    = "labqt";
  gGlobalFrancaisPitchesNamesMap [kA_Natural_QTP]     = "la";
  gGlobalFrancaisPitchesNamesMap [kA_SemiSharp_QTP]   = "lasqt";
  gGlobalFrancaisPitchesNamesMap [kA_Sharp_QTP]       = "lad";
  gGlobalFrancaisPitchesNamesMap [kA_SesquiSharp_QTP] = "lastqt";
  gGlobalFrancaisPitchesNamesMap [kA_DoubleSharp_QTP] = "lass";

  gGlobalFrancaisPitchesNamesMap [kB_DoubleFlat_QTP]  = "sibb";
  gGlobalFrancaisPitchesNamesMap [kB_SesquiFlat_QTP]  = "sibtqt";
  gGlobalFrancaisPitchesNamesMap [kB_Flat_QTP]        = "sib";
  gGlobalFrancaisPitchesNamesMap [kB_SemiFlat_QTP]    = "sibqt";
  gGlobalFrancaisPitchesNamesMap [kB_Natural_QTP]     = "si";
  gGlobalFrancaisPitchesNamesMap [kB_SemiSharp_QTP]   = "sisqt";
  gGlobalFrancaisPitchesNamesMap [kB_Sharp_QTP]       = "sid";
  gGlobalFrancaisPitchesNamesMap [kB_SesquiSharp_QTP] = "sistqt";
  gGlobalFrancaisPitchesNamesMap [kB_DoubleSharp_QTP] = "siss";

  gGlobalFrancaisPitchesNamesMap [kC_DoubleFlat_QTP]  = "dobb";
  gGlobalFrancaisPitchesNamesMap [kC_SesquiFlat_QTP]  = "dobtqt";
  gGlobalFrancaisPitchesNamesMap [kC_Flat_QTP]        = "dob";
  gGlobalFrancaisPitchesNamesMap [kC_SemiFlat_QTP]    = "dobqt";
  gGlobalFrancaisPitchesNamesMap [kC_Natural_QTP]     = "do";
  gGlobalFrancaisPitchesNamesMap [kC_SemiSharp_QTP]   = "dosqt";
  gGlobalFrancaisPitchesNamesMap [kC_Sharp_QTP]       = "dod";
  gGlobalFrancaisPitchesNamesMap [kC_SesquiSharp_QTP] = "dostqt";
  gGlobalFrancaisPitchesNamesMap [kC_DoubleSharp_QTP] = "doss";

  gGlobalFrancaisPitchesNamesMap [kD_DoubleFlat_QTP]  = "rebb";
  gGlobalFrancaisPitchesNamesMap [kD_SesquiFlat_QTP]  = "rebtqt";
  gGlobalFrancaisPitchesNamesMap [kD_Flat_QTP]        = "reb";
  gGlobalFrancaisPitchesNamesMap [kD_SemiFlat_QTP]    = "rebqt";
  gGlobalFrancaisPitchesNamesMap [kD_Natural_QTP]     = "re";
  gGlobalFrancaisPitchesNamesMap [kD_SemiSharp_QTP]   = "resqt";
  gGlobalFrancaisPitchesNamesMap [kD_Sharp_QTP]       = "red";
  gGlobalFrancaisPitchesNamesMap [kD_SesquiSharp_QTP] = "restqt";
  gGlobalFrancaisPitchesNamesMap [kD_DoubleSharp_QTP] = "ress";

  gGlobalFrancaisPitchesNamesMap [kE_DoubleFlat_QTP]  = "mibb";
  gGlobalFrancaisPitchesNamesMap [kE_SesquiFlat_QTP]  = "mibtqt";
  gGlobalFrancaisPitchesNamesMap [kE_Flat_QTP]        = "mib";
  gGlobalFrancaisPitchesNamesMap [kE_SemiFlat_QTP]    = "mibqt";
  gGlobalFrancaisPitchesNamesMap [kE_Natural_QTP]     = "mi";
  gGlobalFrancaisPitchesNamesMap [kE_SemiSharp_QTP]   = "misqt";
  gGlobalFrancaisPitchesNamesMap [kE_Sharp_QTP]       = "mid";
  gGlobalFrancaisPitchesNamesMap [kE_SesquiSharp_QTP] = "mistqt";
  gGlobalFrancaisPitchesNamesMap [kE_DoubleSharp_QTP] = "miss";

  gGlobalFrancaisPitchesNamesMap [kF_DoubleFlat_QTP]  = "fabb";
  gGlobalFrancaisPitchesNamesMap [kF_SesquiFlat_QTP]  = "fabtqt";
  gGlobalFrancaisPitchesNamesMap [kF_Flat_QTP]        = "fab";
  gGlobalFrancaisPitchesNamesMap [kF_SemiFlat_QTP]    = "fabqt";
  gGlobalFrancaisPitchesNamesMap [kF_Natural_QTP]     = "fa";
  gGlobalFrancaisPitchesNamesMap [kF_SemiSharp_QTP]   = "fasqt";
  gGlobalFrancaisPitchesNamesMap [kF_Sharp_QTP]       = "fad";
  gGlobalFrancaisPitchesNamesMap [kF_SesquiSharp_QTP] = "fastqt";
  gGlobalFrancaisPitchesNamesMap [kF_DoubleSharp_QTP] = "fass";

  gGlobalFrancaisPitchesNamesMap [kG_DoubleFlat_QTP]  = "solbb";
  gGlobalFrancaisPitchesNamesMap [kG_SesquiFlat_QTP]  = "solbtqt";
  gGlobalFrancaisPitchesNamesMap [kG_Flat_QTP]        = "solb";
  gGlobalFrancaisPitchesNamesMap [kG_SemiFlat_QTP]    = "solbqt";
  gGlobalFrancaisPitchesNamesMap [kG_Natural_QTP]     = "sol";
  gGlobalFrancaisPitchesNamesMap [kG_SemiSharp_QTP]   = "solsqt";
  gGlobalFrancaisPitchesNamesMap [kG_Sharp_QTP]       = "sold";
  gGlobalFrancaisPitchesNamesMap [kG_SesquiSharp_QTP] = "solstqt";
  gGlobalFrancaisPitchesNamesMap [kG_DoubleSharp_QTP] = "solss";
}

void initializeItalianoPitchNamesMap ()
{
  // italiano
  gGlobalItalianoPitchesNamesMap [k_NoQuarterTonesPitch_QTP]  = "noQuarterTonePitch";
  gGlobalItalianoPitchesNamesMap [k_Rest_QTP]                 = "r";

  gGlobalItalianoPitchesNamesMap [kA_DoubleFlat_QTP]  = "labb";
  gGlobalItalianoPitchesNamesMap [kA_SesquiFlat_QTP]  = "labsb";
  gGlobalItalianoPitchesNamesMap [kA_Flat_QTP]        = "lab";
  gGlobalItalianoPitchesNamesMap [kA_SemiFlat_QTP]    = "lasb";
  gGlobalItalianoPitchesNamesMap [kA_Natural_QTP]     = "la";
  gGlobalItalianoPitchesNamesMap [kA_SemiSharp_QTP]   = "lasd";
  gGlobalItalianoPitchesNamesMap [kA_Sharp_QTP]       = "lad";
  gGlobalItalianoPitchesNamesMap [kA_SesquiSharp_QTP] = "ladsd";
  gGlobalItalianoPitchesNamesMap [kA_DoubleSharp_QTP] = "ladd";

  gGlobalItalianoPitchesNamesMap [kB_DoubleFlat_QTP]  = "sibb";
  gGlobalItalianoPitchesNamesMap [kB_SesquiFlat_QTP]  = "sibsb";
  gGlobalItalianoPitchesNamesMap [kB_Flat_QTP]        = "sib";
  gGlobalItalianoPitchesNamesMap [kB_SemiFlat_QTP]    = "sisb";
  gGlobalItalianoPitchesNamesMap [kB_Natural_QTP]     = "si";
  gGlobalItalianoPitchesNamesMap [kB_SemiSharp_QTP]   = "sisd";
  gGlobalItalianoPitchesNamesMap [kB_Sharp_QTP]       = "sid";
  gGlobalItalianoPitchesNamesMap [kB_SesquiSharp_QTP] = "sidsd";
  gGlobalItalianoPitchesNamesMap [kB_DoubleSharp_QTP] = "sidd";

  gGlobalItalianoPitchesNamesMap [kC_DoubleFlat_QTP]  = "dobb";
  gGlobalItalianoPitchesNamesMap [kC_SesquiFlat_QTP]  = "dobsb";
  gGlobalItalianoPitchesNamesMap [kC_Flat_QTP]        = "dob";
  gGlobalItalianoPitchesNamesMap [kC_SemiFlat_QTP]    = "dosb";
  gGlobalItalianoPitchesNamesMap [kC_Natural_QTP]     = "do";
  gGlobalItalianoPitchesNamesMap [kC_SemiSharp_QTP]   = "dosd";
  gGlobalItalianoPitchesNamesMap [kC_Sharp_QTP]       = "dod";
  gGlobalItalianoPitchesNamesMap [kC_SesquiSharp_QTP] = "dodsd";
  gGlobalItalianoPitchesNamesMap [kC_DoubleSharp_QTP] = "dodd";

  gGlobalItalianoPitchesNamesMap [kD_DoubleFlat_QTP]  = "rebb";
  gGlobalItalianoPitchesNamesMap [kD_SesquiFlat_QTP]  = "rebsb";
  gGlobalItalianoPitchesNamesMap [kD_Flat_QTP]        = "reb";
  gGlobalItalianoPitchesNamesMap [kD_SemiFlat_QTP]    = "resb";
  gGlobalItalianoPitchesNamesMap [kD_Natural_QTP]     = "re";
  gGlobalItalianoPitchesNamesMap [kD_SemiSharp_QTP]   = "resd";
  gGlobalItalianoPitchesNamesMap [kD_Sharp_QTP]       = "red";
  gGlobalItalianoPitchesNamesMap [kD_SesquiSharp_QTP] = "redsd";
  gGlobalItalianoPitchesNamesMap [kD_DoubleSharp_QTP] = "redd";

  gGlobalItalianoPitchesNamesMap [kE_DoubleFlat_QTP]  = "mibb";
  gGlobalItalianoPitchesNamesMap [kE_SesquiFlat_QTP]  = "mibsb";
  gGlobalItalianoPitchesNamesMap [kE_Flat_QTP]        = "mib";
  gGlobalItalianoPitchesNamesMap [kE_SemiFlat_QTP]    = "misb";
  gGlobalItalianoPitchesNamesMap [kE_Natural_QTP]     = "mi";
  gGlobalItalianoPitchesNamesMap [kE_SemiSharp_QTP]   = "misd";
  gGlobalItalianoPitchesNamesMap [kE_Sharp_QTP]       = "mid";
  gGlobalItalianoPitchesNamesMap [kE_SesquiSharp_QTP] = "midsd";
  gGlobalItalianoPitchesNamesMap [kE_DoubleSharp_QTP] = "midd";

  gGlobalItalianoPitchesNamesMap [kF_DoubleFlat_QTP]  = "fabb";
  gGlobalItalianoPitchesNamesMap [kF_SesquiFlat_QTP]  = "fabsb";
  gGlobalItalianoPitchesNamesMap [kF_Flat_QTP]        = "fab";
  gGlobalItalianoPitchesNamesMap [kF_SemiFlat_QTP]    = "fasb";
  gGlobalItalianoPitchesNamesMap [kF_Natural_QTP]     = "fa";
  gGlobalItalianoPitchesNamesMap [kF_SemiSharp_QTP]   = "fasd";
  gGlobalItalianoPitchesNamesMap [kF_Sharp_QTP]       = "fad";
  gGlobalItalianoPitchesNamesMap [kF_SesquiSharp_QTP] = "fadsd";
  gGlobalItalianoPitchesNamesMap [kF_DoubleSharp_QTP] = "fadd";

  gGlobalItalianoPitchesNamesMap [kG_DoubleFlat_QTP]  = "solbb";
  gGlobalItalianoPitchesNamesMap [kG_SesquiFlat_QTP]  = "solbsb";
  gGlobalItalianoPitchesNamesMap [kG_Flat_QTP]        = "solb";
  gGlobalItalianoPitchesNamesMap [kG_SemiFlat_QTP]    = "solsb";
  gGlobalItalianoPitchesNamesMap [kG_Natural_QTP]     = "sol";
  gGlobalItalianoPitchesNamesMap [kG_SemiSharp_QTP]   = "solsd";
  gGlobalItalianoPitchesNamesMap [kG_Sharp_QTP]       = "sold";
  gGlobalItalianoPitchesNamesMap [kG_SesquiSharp_QTP] = "soldsd";
  gGlobalItalianoPitchesNamesMap [kG_DoubleSharp_QTP] = "soldd";
}

void initializeNorskPitchNamesMap ()
{
  // norsk
  gGlobalNorskPitchesNamesMap [k_NoQuarterTonesPitch_QTP]  = "noQuarterTonePitch";
  gGlobalNorskPitchesNamesMap [k_Rest_QTP]                 = "r";

  gGlobalNorskPitchesNamesMap [kA_DoubleFlat_QTP]  = "aeses";
  gGlobalNorskPitchesNamesMap [kA_SesquiFlat_QTP]  = "aSesquiFlat???";
  gGlobalNorskPitchesNamesMap [kA_Flat_QTP]        = "aes";
  gGlobalNorskPitchesNamesMap [kA_SemiFlat_QTP]    = "aSemiFlat???";
  gGlobalNorskPitchesNamesMap [kA_Natural_QTP]     = "a";
  gGlobalNorskPitchesNamesMap [kA_SemiSharp_QTP]   = "aSemiSharp???";
  gGlobalNorskPitchesNamesMap [kA_Sharp_QTP]       = "ais";
  gGlobalNorskPitchesNamesMap [kA_SesquiSharp_QTP] = "aSesquiSharp???";
  gGlobalNorskPitchesNamesMap [kA_DoubleSharp_QTP] = "aisis";

  gGlobalNorskPitchesNamesMap [kB_DoubleFlat_QTP]  = "beses";
  gGlobalNorskPitchesNamesMap [kB_SesquiFlat_QTP]  = "bSesquiFlat???";
  gGlobalNorskPitchesNamesMap [kB_Flat_QTP]        = "bes";
  gGlobalNorskPitchesNamesMap [kB_SemiFlat_QTP]    = "bSemiFlat???";
  gGlobalNorskPitchesNamesMap [kB_Natural_QTP]     = "b";
  gGlobalNorskPitchesNamesMap [kB_SemiSharp_QTP]   = "bSemiSharp???";
  gGlobalNorskPitchesNamesMap [kB_Sharp_QTP]       = "bis";
  gGlobalNorskPitchesNamesMap [kB_SesquiSharp_QTP] = "bSesquiSharp???";
  gGlobalNorskPitchesNamesMap [kB_DoubleSharp_QTP] = "bisis";

  gGlobalNorskPitchesNamesMap [kC_DoubleFlat_QTP]  = "ceses";
  gGlobalNorskPitchesNamesMap [kC_SesquiFlat_QTP]  = "cSesquiFlat???";
  gGlobalNorskPitchesNamesMap [kC_Flat_QTP]        = "ces";
  gGlobalNorskPitchesNamesMap [kC_SemiFlat_QTP]    = "cSemiFlat???";
  gGlobalNorskPitchesNamesMap [kC_Natural_QTP]     = "c";
  gGlobalNorskPitchesNamesMap [kC_SemiSharp_QTP]   = "cSemiSharp???";
  gGlobalNorskPitchesNamesMap [kC_Sharp_QTP]       = "cis";
  gGlobalNorskPitchesNamesMap [kC_SesquiSharp_QTP] = "cSesquiSharp???";
  gGlobalNorskPitchesNamesMap [kC_DoubleSharp_QTP] = "cisis";

  gGlobalNorskPitchesNamesMap [kD_DoubleFlat_QTP]  = "deses";
  gGlobalNorskPitchesNamesMap [kD_SesquiFlat_QTP]  = "dSesquiFlat???";
  gGlobalNorskPitchesNamesMap [kD_Flat_QTP]        = "des";
  gGlobalNorskPitchesNamesMap [kD_SemiFlat_QTP]    = "dSemiFlat???";
  gGlobalNorskPitchesNamesMap [kD_Natural_QTP]     = "d";
  gGlobalNorskPitchesNamesMap [kD_SemiSharp_QTP]   = "dSemiSharp???";
  gGlobalNorskPitchesNamesMap [kD_Sharp_QTP]       = "dis";
  gGlobalNorskPitchesNamesMap [kD_SesquiSharp_QTP] = "dSesquiSharp???";
  gGlobalNorskPitchesNamesMap [kD_DoubleSharp_QTP] = "disis";

  gGlobalNorskPitchesNamesMap [kE_DoubleFlat_QTP]  = "eeses";
  gGlobalNorskPitchesNamesMap [kE_SesquiFlat_QTP]  = "eSesquiFlat???";
  gGlobalNorskPitchesNamesMap [kE_Flat_QTP]        = "ees";
  gGlobalNorskPitchesNamesMap [kE_SemiFlat_QTP]    = "eSemiFlat???";
  gGlobalNorskPitchesNamesMap [kE_Natural_QTP]     = "e";
  gGlobalNorskPitchesNamesMap [kE_SemiSharp_QTP]   = "eSemiSharp???";
  gGlobalNorskPitchesNamesMap [kE_Sharp_QTP]       = "eis";
  gGlobalNorskPitchesNamesMap [kE_SesquiSharp_QTP] = "eSesquiSharp???";
  gGlobalNorskPitchesNamesMap [kE_DoubleSharp_QTP] = "eisis";

  gGlobalNorskPitchesNamesMap [kF_DoubleFlat_QTP]  = "feses";
  gGlobalNorskPitchesNamesMap [kF_SesquiFlat_QTP]  = "fSesquiFlat???";
  gGlobalNorskPitchesNamesMap [kF_Flat_QTP]        = "fes";
  gGlobalNorskPitchesNamesMap [kF_SemiFlat_QTP]    = "fSemiFlat???";
  gGlobalNorskPitchesNamesMap [kF_Natural_QTP]     = "f";
  gGlobalNorskPitchesNamesMap [kF_SemiSharp_QTP]   = "fSemiSharp???";
  gGlobalNorskPitchesNamesMap [kF_Sharp_QTP]       = "fis";
  gGlobalNorskPitchesNamesMap [kF_SesquiSharp_QTP] = "fSesquiSharp???";
  gGlobalNorskPitchesNamesMap [kF_DoubleSharp_QTP] = "fisis";

  gGlobalNorskPitchesNamesMap [kG_DoubleFlat_QTP]  = "geses";
  gGlobalNorskPitchesNamesMap [kG_SesquiFlat_QTP]  = "gSesquiFlat???";
  gGlobalNorskPitchesNamesMap [kG_Flat_QTP]        = "ges";
  gGlobalNorskPitchesNamesMap [kG_SemiFlat_QTP]    = "gSemiFlat???";
  gGlobalNorskPitchesNamesMap [kG_Natural_QTP]     = "g";
  gGlobalNorskPitchesNamesMap [kG_SemiSharp_QTP]   = "gSemiSharp???";
  gGlobalNorskPitchesNamesMap [kG_Sharp_QTP]       = "gis";
  gGlobalNorskPitchesNamesMap [kG_SesquiSharp_QTP] = "gSesquiSharp???";
  gGlobalNorskPitchesNamesMap [kG_DoubleSharp_QTP] = "gisis";
}

void initializePortuguesPitchNamesMap ()
{
  // portugues
  gGlobalPortuguesPitchesNamesMap [k_NoQuarterTonesPitch_QTP]  = "noQuarterTonePitch";
  gGlobalPortuguesPitchesNamesMap [k_Rest_QTP]                 = "r";

  gGlobalPortuguesPitchesNamesMap [kA_DoubleFlat_QTP]  = "labb";
  gGlobalPortuguesPitchesNamesMap [kA_SesquiFlat_QTP]  = "labtqt";
  gGlobalPortuguesPitchesNamesMap [kA_Flat_QTP]        = "lab";
  gGlobalPortuguesPitchesNamesMap [kA_SemiFlat_QTP]    = "lasb";
  gGlobalPortuguesPitchesNamesMap [kA_Natural_QTP]     = "la";
  gGlobalPortuguesPitchesNamesMap [kA_SemiSharp_QTP]   = "lasd";
  gGlobalPortuguesPitchesNamesMap [kA_Sharp_QTP]       = "lad";
  gGlobalPortuguesPitchesNamesMap [kA_SesquiSharp_QTP] = "ladsd";
  gGlobalPortuguesPitchesNamesMap [kA_DoubleSharp_QTP] = "ladd";

  gGlobalPortuguesPitchesNamesMap [kB_DoubleFlat_QTP]  = "sibb";
  gGlobalPortuguesPitchesNamesMap [kB_SesquiFlat_QTP]  = "sibtqt";
  gGlobalPortuguesPitchesNamesMap [kB_Flat_QTP]        = "sib";
  gGlobalPortuguesPitchesNamesMap [kB_SemiFlat_QTP]    = "sisb";
  gGlobalPortuguesPitchesNamesMap [kB_Natural_QTP]     = "si";
  gGlobalPortuguesPitchesNamesMap [kB_SemiSharp_QTP]   = "sisd";
  gGlobalPortuguesPitchesNamesMap [kB_Sharp_QTP]       = "sid";
  gGlobalPortuguesPitchesNamesMap [kB_SesquiSharp_QTP] = "sidsd";
  gGlobalPortuguesPitchesNamesMap [kB_DoubleSharp_QTP] = "sidd";

  gGlobalPortuguesPitchesNamesMap [kC_DoubleFlat_QTP]  = "dobb";
  gGlobalPortuguesPitchesNamesMap [kC_SesquiFlat_QTP]  = "dobtqt";
  gGlobalPortuguesPitchesNamesMap [kC_Flat_QTP]        = "dob";
  gGlobalPortuguesPitchesNamesMap [kC_SemiFlat_QTP]    = "dosb";
  gGlobalPortuguesPitchesNamesMap [kC_Natural_QTP]     = "do";
  gGlobalPortuguesPitchesNamesMap [kC_SemiSharp_QTP]   = "dosd";
  gGlobalPortuguesPitchesNamesMap [kC_Sharp_QTP]       = "dod";
  gGlobalPortuguesPitchesNamesMap [kC_SesquiSharp_QTP] = "dodsd";
  gGlobalPortuguesPitchesNamesMap [kC_DoubleSharp_QTP] = "dodd";

  gGlobalPortuguesPitchesNamesMap [kD_DoubleFlat_QTP]  = "rebb";
  gGlobalPortuguesPitchesNamesMap [kD_SesquiFlat_QTP]  = "rebtqt";
  gGlobalPortuguesPitchesNamesMap [kD_Flat_QTP]        = "reb";
  gGlobalPortuguesPitchesNamesMap [kD_SemiFlat_QTP]    = "resb";
  gGlobalPortuguesPitchesNamesMap [kD_Natural_QTP]     = "re";
  gGlobalPortuguesPitchesNamesMap [kD_SemiSharp_QTP]   = "resd";
  gGlobalPortuguesPitchesNamesMap [kD_Sharp_QTP]       = "red";
  gGlobalPortuguesPitchesNamesMap [kD_SesquiSharp_QTP] = "redsd";
  gGlobalPortuguesPitchesNamesMap [kD_DoubleSharp_QTP] = "redd";

  gGlobalPortuguesPitchesNamesMap [kE_DoubleFlat_QTP]  = "mibb";
  gGlobalPortuguesPitchesNamesMap [kE_SesquiFlat_QTP]  = "mibtqt";
  gGlobalPortuguesPitchesNamesMap [kE_Flat_QTP]        = "mib";
  gGlobalPortuguesPitchesNamesMap [kE_SemiFlat_QTP]    = "misb";
  gGlobalPortuguesPitchesNamesMap [kE_Natural_QTP]     = "mi";
  gGlobalPortuguesPitchesNamesMap [kE_SemiSharp_QTP]   = "misd";
  gGlobalPortuguesPitchesNamesMap [kE_Sharp_QTP]       = "mid";
  gGlobalPortuguesPitchesNamesMap [kE_SesquiSharp_QTP] = "midsd";
  gGlobalPortuguesPitchesNamesMap [kE_DoubleSharp_QTP] = "midd";

  gGlobalPortuguesPitchesNamesMap [kF_DoubleFlat_QTP]  = "fabb";
  gGlobalPortuguesPitchesNamesMap [kF_SesquiFlat_QTP]  = "fabtqt";
  gGlobalPortuguesPitchesNamesMap [kF_Flat_QTP]        = "fab";
  gGlobalPortuguesPitchesNamesMap [kF_SemiFlat_QTP]    = "fasb";
  gGlobalPortuguesPitchesNamesMap [kF_Natural_QTP]     = "fa";
  gGlobalPortuguesPitchesNamesMap [kF_SemiSharp_QTP]   = "fasd";
  gGlobalPortuguesPitchesNamesMap [kF_Sharp_QTP]       = "fad";
  gGlobalPortuguesPitchesNamesMap [kF_SesquiSharp_QTP] = "fadsd";
  gGlobalPortuguesPitchesNamesMap [kF_DoubleSharp_QTP] = "fadd";

  gGlobalPortuguesPitchesNamesMap [kG_DoubleFlat_QTP]  = "solbb";
  gGlobalPortuguesPitchesNamesMap [kG_SesquiFlat_QTP]  = "solbtqt";
  gGlobalPortuguesPitchesNamesMap [kG_Flat_QTP]        = "solb";
  gGlobalPortuguesPitchesNamesMap [kG_SemiFlat_QTP]    = "solsb";
  gGlobalPortuguesPitchesNamesMap [kG_Natural_QTP]     = "sol";
  gGlobalPortuguesPitchesNamesMap [kG_SemiSharp_QTP]   = "solsd";
  gGlobalPortuguesPitchesNamesMap [kG_Sharp_QTP]       = "sold";
  gGlobalPortuguesPitchesNamesMap [kG_SesquiSharp_QTP] = "soldsd";
  gGlobalPortuguesPitchesNamesMap [kG_DoubleSharp_QTP] = "soldd";
}

void initializeSuomiPitchNamesMap ()
{
  // suomi
  gGlobalSuomiPitchesNamesMap [k_NoQuarterTonesPitch_QTP]  = "noQuarterTonePitch";
  gGlobalSuomiPitchesNamesMap [k_Rest_QTP]                 = "r";

  gGlobalSuomiPitchesNamesMap [kA_DoubleFlat_QTP]  = "asas";
  gGlobalSuomiPitchesNamesMap [kA_SesquiFlat_QTP]  = "aSesquiFlat???";
  gGlobalSuomiPitchesNamesMap [kA_Flat_QTP]        = "as";
  gGlobalSuomiPitchesNamesMap [kA_SemiFlat_QTP]    = "aSemiFlat???";
  gGlobalSuomiPitchesNamesMap [kA_Natural_QTP]     = "a";
  gGlobalSuomiPitchesNamesMap [kA_SemiSharp_QTP]   = "aSemiSharp???";
  gGlobalSuomiPitchesNamesMap [kA_Sharp_QTP]       = "ais";
  gGlobalSuomiPitchesNamesMap [kA_SesquiSharp_QTP] = "aSesquiSharp???";
  gGlobalSuomiPitchesNamesMap [kA_DoubleSharp_QTP] = "aisis";

  gGlobalSuomiPitchesNamesMap [kB_DoubleFlat_QTP]  = "bes";
  gGlobalSuomiPitchesNamesMap [kB_SesquiFlat_QTP]  = "bSesquiFlat???";
  gGlobalSuomiPitchesNamesMap [kB_Flat_QTP]        = "b";
  gGlobalSuomiPitchesNamesMap [kB_SemiFlat_QTP]    = "bSemiFlat???";
  gGlobalSuomiPitchesNamesMap [kB_Natural_QTP]     = "h";
  gGlobalSuomiPitchesNamesMap [kB_SemiSharp_QTP]   = "bSemiSharp???";
  gGlobalSuomiPitchesNamesMap [kB_Sharp_QTP]       = "his";
  gGlobalSuomiPitchesNamesMap [kB_SesquiSharp_QTP] = "bSesquiSharp???";
  gGlobalSuomiPitchesNamesMap [kB_DoubleSharp_QTP] = "hisis";

  gGlobalSuomiPitchesNamesMap [kC_DoubleFlat_QTP]  = "ceses";
  gGlobalSuomiPitchesNamesMap [kC_SesquiFlat_QTP]  = "cSesquiFlat???";
  gGlobalSuomiPitchesNamesMap [kC_Flat_QTP]        = "ces";
  gGlobalSuomiPitchesNamesMap [kC_SemiFlat_QTP]    = "cSemiFlat???";
  gGlobalSuomiPitchesNamesMap [kC_Natural_QTP]     = "c";
  gGlobalSuomiPitchesNamesMap [kC_SemiSharp_QTP]   = "cSemiSharp???";
  gGlobalSuomiPitchesNamesMap [kC_Sharp_QTP]       = "cis";
  gGlobalSuomiPitchesNamesMap [kC_SesquiSharp_QTP] = "cSesquiSharp???";
  gGlobalSuomiPitchesNamesMap [kC_DoubleSharp_QTP] = "cisis";

  gGlobalSuomiPitchesNamesMap [kD_DoubleFlat_QTP]  = "deses";
  gGlobalSuomiPitchesNamesMap [kD_SesquiFlat_QTP]  = "dSesquiFlat???";
  gGlobalSuomiPitchesNamesMap [kD_Flat_QTP]        = "des";
  gGlobalSuomiPitchesNamesMap [kD_SemiFlat_QTP]    = "dSemiFlat???";
  gGlobalSuomiPitchesNamesMap [kD_Natural_QTP]     = "d";
  gGlobalSuomiPitchesNamesMap [kD_SemiSharp_QTP]   = "dSemiSharp???";
  gGlobalSuomiPitchesNamesMap [kD_Sharp_QTP]       = "dis";
  gGlobalSuomiPitchesNamesMap [kD_SesquiSharp_QTP] = "dSesquiSharp???";
  gGlobalSuomiPitchesNamesMap [kD_DoubleSharp_QTP] = "disis";

  gGlobalSuomiPitchesNamesMap [kE_DoubleFlat_QTP]  = "eses";
  gGlobalSuomiPitchesNamesMap [kE_SesquiFlat_QTP]  = "eSesquiFlat???";
  gGlobalSuomiPitchesNamesMap [kE_Flat_QTP]        = "es";
  gGlobalSuomiPitchesNamesMap [kE_SemiFlat_QTP]    = "eSemiFlat???";
  gGlobalSuomiPitchesNamesMap [kE_Natural_QTP]     = "e";
  gGlobalSuomiPitchesNamesMap [kE_SemiSharp_QTP]   = "eSemiSharp???";
  gGlobalSuomiPitchesNamesMap [kE_Sharp_QTP]       = "eis";
  gGlobalSuomiPitchesNamesMap [kE_SesquiSharp_QTP] = "eSesquiSharp???";
  gGlobalSuomiPitchesNamesMap [kE_DoubleSharp_QTP] = "eisis";

  gGlobalSuomiPitchesNamesMap [kF_DoubleFlat_QTP]  = "feses";
  gGlobalSuomiPitchesNamesMap [kF_SesquiFlat_QTP]  = "fSesquiFlat???";
  gGlobalSuomiPitchesNamesMap [kF_Flat_QTP]        = "fes";
  gGlobalSuomiPitchesNamesMap [kF_SemiFlat_QTP]    = "fSemiFlat???";
  gGlobalSuomiPitchesNamesMap [kF_Natural_QTP]     = "f";
  gGlobalSuomiPitchesNamesMap [kF_SemiSharp_QTP]   = "fSemiSharp???";
  gGlobalSuomiPitchesNamesMap [kF_Sharp_QTP]       = "fis";
  gGlobalSuomiPitchesNamesMap [kF_SesquiSharp_QTP] = "fSesquiSharp???";
  gGlobalSuomiPitchesNamesMap [kF_DoubleSharp_QTP] = "fisis";

  gGlobalSuomiPitchesNamesMap [kG_DoubleFlat_QTP]  = "geses";
  gGlobalSuomiPitchesNamesMap [kG_SesquiFlat_QTP]  = "gSesquiFlat???";
  gGlobalSuomiPitchesNamesMap [kG_Flat_QTP]        = "ges";
  gGlobalSuomiPitchesNamesMap [kG_SemiFlat_QTP]    = "gSemiFlat???";
  gGlobalSuomiPitchesNamesMap [kG_Natural_QTP]     = "g";
  gGlobalSuomiPitchesNamesMap [kG_SemiSharp_QTP]   = "gSemiSharp???";
  gGlobalSuomiPitchesNamesMap [kG_Sharp_QTP]       = "gis";
  gGlobalSuomiPitchesNamesMap [kG_SesquiSharp_QTP] = "gSesquiSharp???";
  gGlobalSuomiPitchesNamesMap [kG_DoubleSharp_QTP] = "gisis";
}

void initializeSvenskaPitchNamesMap ()
{
  // svenska
  gGlobalSvenskaPitchesNamesMap [k_NoQuarterTonesPitch_QTP]  = "noQuarterTonePitch";
  gGlobalSvenskaPitchesNamesMap [k_Rest_QTP]                 = "r";

  gGlobalSvenskaPitchesNamesMap [kA_DoubleFlat_QTP]  = "assess";
  gGlobalSvenskaPitchesNamesMap [kA_SesquiFlat_QTP]  = "aSesquiFlat???";
  gGlobalSvenskaPitchesNamesMap [kA_Flat_QTP]        = "ass";
  gGlobalSvenskaPitchesNamesMap [kA_SemiFlat_QTP]    = "aSemiFlat???";
  gGlobalSvenskaPitchesNamesMap [kA_Natural_QTP]     = "a";
  gGlobalSvenskaPitchesNamesMap [kA_SemiSharp_QTP]   = "aSemiSharp???";
  gGlobalSvenskaPitchesNamesMap [kA_Sharp_QTP]       = "aiss";
  gGlobalSvenskaPitchesNamesMap [kA_SesquiSharp_QTP] = "aSesquiSharp???";
  gGlobalSvenskaPitchesNamesMap [kA_DoubleSharp_QTP] = "aississ";

  gGlobalSvenskaPitchesNamesMap [kB_DoubleFlat_QTP]  = "hessess";
  gGlobalSvenskaPitchesNamesMap [kB_SesquiFlat_QTP]  = "bSesquiFlat???";
  gGlobalSvenskaPitchesNamesMap [kB_Flat_QTP]        = "b";
  gGlobalSvenskaPitchesNamesMap [kB_SemiFlat_QTP]    = "bSemiFlat???";
  gGlobalSvenskaPitchesNamesMap [kB_Natural_QTP]     = "h";
  gGlobalSvenskaPitchesNamesMap [kB_SemiSharp_QTP]   = "bSemiSharp???";
  gGlobalSvenskaPitchesNamesMap [kB_Sharp_QTP]       = "hiss";
  gGlobalSvenskaPitchesNamesMap [kB_SesquiSharp_QTP] = "bSesquiSharp???";
  gGlobalSvenskaPitchesNamesMap [kB_DoubleSharp_QTP] = "hississ";

  gGlobalSvenskaPitchesNamesMap [kC_DoubleFlat_QTP]  = "cessess";
  gGlobalSvenskaPitchesNamesMap [kC_SesquiFlat_QTP]  = "cSesquiFlat???";
  gGlobalSvenskaPitchesNamesMap [kC_Flat_QTP]        = "cess";
  gGlobalSvenskaPitchesNamesMap [kC_SemiFlat_QTP]    = "cSemiFlat???";
  gGlobalSvenskaPitchesNamesMap [kC_Natural_QTP]     = "c";
  gGlobalSvenskaPitchesNamesMap [kC_SemiSharp_QTP]   = "cSemiSharp???";
  gGlobalSvenskaPitchesNamesMap [kC_Sharp_QTP]       = "ciss";
  gGlobalSvenskaPitchesNamesMap [kC_SesquiSharp_QTP] = "cSesquiSharp???";
  gGlobalSvenskaPitchesNamesMap [kC_DoubleSharp_QTP] = "cississ";

  gGlobalSvenskaPitchesNamesMap [kD_DoubleFlat_QTP]  = "dessess";
  gGlobalSvenskaPitchesNamesMap [kD_SesquiFlat_QTP]  = "dSesquiFlat???";
  gGlobalSvenskaPitchesNamesMap [kD_Flat_QTP]        = "dess";
  gGlobalSvenskaPitchesNamesMap [kD_SemiFlat_QTP]    = "dSemiFlat???";
  gGlobalSvenskaPitchesNamesMap [kD_Natural_QTP]     = "d";
  gGlobalSvenskaPitchesNamesMap [kD_SemiSharp_QTP]   = "dSemiSharp???";
  gGlobalSvenskaPitchesNamesMap [kD_Sharp_QTP]       = "diss";
  gGlobalSvenskaPitchesNamesMap [kD_SesquiSharp_QTP] = "dSesquiSharp???";
  gGlobalSvenskaPitchesNamesMap [kD_DoubleSharp_QTP] = "dississ";

  gGlobalSvenskaPitchesNamesMap [kE_DoubleFlat_QTP]  = "essess";
  gGlobalSvenskaPitchesNamesMap [kE_SesquiFlat_QTP]  = "eSesquiFlat???";
  gGlobalSvenskaPitchesNamesMap [kE_Flat_QTP]        = "ess";
  gGlobalSvenskaPitchesNamesMap [kE_SemiFlat_QTP]    = "eSemiFlat???";
  gGlobalSvenskaPitchesNamesMap [kE_Natural_QTP]     = "e";
  gGlobalSvenskaPitchesNamesMap [kE_SemiSharp_QTP]   = "eSemiSharp???";
  gGlobalSvenskaPitchesNamesMap [kE_Sharp_QTP]       = "eiss";
  gGlobalSvenskaPitchesNamesMap [kE_SesquiSharp_QTP] = "eSesquiSharp???";
  gGlobalSvenskaPitchesNamesMap [kE_DoubleSharp_QTP] = "eississ";

  gGlobalSvenskaPitchesNamesMap [kF_DoubleFlat_QTP]  = "fessess";
  gGlobalSvenskaPitchesNamesMap [kF_SesquiFlat_QTP]  = "fSesquiFlat???";
  gGlobalSvenskaPitchesNamesMap [kF_Flat_QTP]        = "fess";
  gGlobalSvenskaPitchesNamesMap [kF_SemiFlat_QTP]    = "fSemiFlat???";
  gGlobalSvenskaPitchesNamesMap [kF_Natural_QTP]     = "f";
  gGlobalSvenskaPitchesNamesMap [kF_SemiSharp_QTP]   = "fSemiSharp???";
  gGlobalSvenskaPitchesNamesMap [kF_Sharp_QTP]       = "fiss";
  gGlobalSvenskaPitchesNamesMap [kF_SesquiSharp_QTP] = "fSesquiSharp???";
  gGlobalSvenskaPitchesNamesMap [kF_DoubleSharp_QTP] = "fississ";

  gGlobalSvenskaPitchesNamesMap [kG_DoubleFlat_QTP]  = "gessess";
  gGlobalSvenskaPitchesNamesMap [kG_SesquiFlat_QTP]  = "gSesquiFlat???";
  gGlobalSvenskaPitchesNamesMap [kG_Flat_QTP]        = "gess";
  gGlobalSvenskaPitchesNamesMap [kG_SemiFlat_QTP]    = "gSemiFlat???";
  gGlobalSvenskaPitchesNamesMap [kG_Natural_QTP]     = "g";
  gGlobalSvenskaPitchesNamesMap [kG_SemiSharp_QTP]   = "gSemiSharp???";
  gGlobalSvenskaPitchesNamesMap [kG_Sharp_QTP]       = "giss";
  gGlobalSvenskaPitchesNamesMap [kG_SesquiSharp_QTP] = "gSesquiSharp???";
  gGlobalSvenskaPitchesNamesMap [kG_DoubleSharp_QTP] = "gississ";
}

void initializeVlaamsPitchNamesMap ()
{
  // vlaams
  gGlobalVlaamsPitchesNamesMap [k_NoQuarterTonesPitch_QTP]  = "noQuarterTonePitch";
  gGlobalVlaamsPitchesNamesMap [k_Rest_QTP]                 = "r";

  gGlobalVlaamsPitchesNamesMap [kA_DoubleFlat_QTP]  = "labb";
  gGlobalVlaamsPitchesNamesMap [kA_SesquiFlat_QTP]  = "laSesquiFlat???";
  gGlobalVlaamsPitchesNamesMap [kA_Flat_QTP]        = "lab";
  gGlobalVlaamsPitchesNamesMap [kA_SemiFlat_QTP]    = "laSemiFlat???";
  gGlobalVlaamsPitchesNamesMap [kA_Natural_QTP]     = "la";
  gGlobalVlaamsPitchesNamesMap [kA_SemiSharp_QTP]   = "laSemiSharp???";
  gGlobalVlaamsPitchesNamesMap [kA_Sharp_QTP]       = "lak";
  gGlobalVlaamsPitchesNamesMap [kA_SesquiSharp_QTP] = "laSesquiSharp???";
  gGlobalVlaamsPitchesNamesMap [kA_DoubleSharp_QTP] = "lakk";

  gGlobalVlaamsPitchesNamesMap [kB_DoubleFlat_QTP]  = "sibb";
  gGlobalVlaamsPitchesNamesMap [kB_SesquiFlat_QTP]  = "siSesquiFlat???";
  gGlobalVlaamsPitchesNamesMap [kB_Flat_QTP]        = "sib";
  gGlobalVlaamsPitchesNamesMap [kB_SemiFlat_QTP]    = "siSemiFlat???";
  gGlobalVlaamsPitchesNamesMap [kB_Natural_QTP]     = "si";
  gGlobalVlaamsPitchesNamesMap [kB_SemiSharp_QTP]   = "siSemiSharp???";
  gGlobalVlaamsPitchesNamesMap [kB_Sharp_QTP]       = "sik";
  gGlobalVlaamsPitchesNamesMap [kB_SesquiSharp_QTP] = "siSesquiSharp???";
  gGlobalVlaamsPitchesNamesMap [kB_DoubleSharp_QTP] = "sikk";

  gGlobalVlaamsPitchesNamesMap [kC_DoubleFlat_QTP]  = "dobb";
  gGlobalVlaamsPitchesNamesMap [kC_SesquiFlat_QTP]  = "doSesquiFlat???";
  gGlobalVlaamsPitchesNamesMap [kC_Flat_QTP]        = "dob";
  gGlobalVlaamsPitchesNamesMap [kC_SemiFlat_QTP]    = "doSemiFlat???";
  gGlobalVlaamsPitchesNamesMap [kC_Natural_QTP]     = "do";
  gGlobalVlaamsPitchesNamesMap [kC_SemiSharp_QTP]   = "doSemiSharp???";
  gGlobalVlaamsPitchesNamesMap [kC_Sharp_QTP]       = "dok";
  gGlobalVlaamsPitchesNamesMap [kC_SesquiSharp_QTP] = "doSesquiSharp???";
  gGlobalVlaamsPitchesNamesMap [kC_DoubleSharp_QTP] = "dokk";

  gGlobalVlaamsPitchesNamesMap [kD_DoubleFlat_QTP]  = "rebb";
  gGlobalVlaamsPitchesNamesMap [kD_SesquiFlat_QTP]  = "reSesquiFlat???";
  gGlobalVlaamsPitchesNamesMap [kD_Flat_QTP]        = "reb";
  gGlobalVlaamsPitchesNamesMap [kD_SemiFlat_QTP]    = "reSemiFlat???";
  gGlobalVlaamsPitchesNamesMap [kD_Natural_QTP]     = "re";
  gGlobalVlaamsPitchesNamesMap [kD_SemiSharp_QTP]   = "reSemiSharp???";
  gGlobalVlaamsPitchesNamesMap [kD_Sharp_QTP]       = "rek";
  gGlobalVlaamsPitchesNamesMap [kD_SesquiSharp_QTP] = "reSesquiSharp???";
  gGlobalVlaamsPitchesNamesMap [kD_DoubleSharp_QTP] = "rekk";

  gGlobalVlaamsPitchesNamesMap [kE_DoubleFlat_QTP]  = "mibb";
  gGlobalVlaamsPitchesNamesMap [kE_SesquiFlat_QTP]  = "miSesquiFlat???";
  gGlobalVlaamsPitchesNamesMap [kE_Flat_QTP]        = "mib";
  gGlobalVlaamsPitchesNamesMap [kE_SemiFlat_QTP]    = "miSemiFlat???";
  gGlobalVlaamsPitchesNamesMap [kE_Natural_QTP]     = "mi";
  gGlobalVlaamsPitchesNamesMap [kE_SemiSharp_QTP]   = "miSemiSharp???";
  gGlobalVlaamsPitchesNamesMap [kE_Sharp_QTP]       = "mik";
  gGlobalVlaamsPitchesNamesMap [kE_SesquiSharp_QTP] = "miSesquiSharp???";
  gGlobalVlaamsPitchesNamesMap [kE_DoubleSharp_QTP] = "mikk";

  gGlobalVlaamsPitchesNamesMap [kF_DoubleFlat_QTP]  = "fabb";
  gGlobalVlaamsPitchesNamesMap [kF_SesquiFlat_QTP]  = "faSesquiFlat???";
  gGlobalVlaamsPitchesNamesMap [kF_Flat_QTP]        = "fab";
  gGlobalVlaamsPitchesNamesMap [kF_SemiFlat_QTP]    = "faSemiFlat???";
  gGlobalVlaamsPitchesNamesMap [kF_Natural_QTP]     = "fa";
  gGlobalVlaamsPitchesNamesMap [kF_SemiSharp_QTP]   = "faSemiSharp???";
  gGlobalVlaamsPitchesNamesMap [kF_Sharp_QTP]       = "fak";
  gGlobalVlaamsPitchesNamesMap [kF_SesquiSharp_QTP] = "faSesquiSharp???";
  gGlobalVlaamsPitchesNamesMap [kF_DoubleSharp_QTP] = "fakk";

  gGlobalVlaamsPitchesNamesMap [kG_DoubleFlat_QTP]  = "solbb";
  gGlobalVlaamsPitchesNamesMap [kG_SesquiFlat_QTP]  = "solSesquiFlat???";
  gGlobalVlaamsPitchesNamesMap [kG_Flat_QTP]        = "solb";
  gGlobalVlaamsPitchesNamesMap [kG_SemiFlat_QTP]    = "solSemiFlat???";
  gGlobalVlaamsPitchesNamesMap [kG_Natural_QTP]     = "sol";
  gGlobalVlaamsPitchesNamesMap [kG_SemiSharp_QTP]   = "solSemiSharp???";
  gGlobalVlaamsPitchesNamesMap [kG_Sharp_QTP]       = "solk";
  gGlobalVlaamsPitchesNamesMap [kG_SesquiSharp_QTP] = "solSesquiSharp???";
  gGlobalVlaamsPitchesNamesMap [kG_DoubleSharp_QTP] = "solkk";
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

// alterations
//______________________________________________________________________________
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

float msrMusicXMLAlterFromAlterationKind (
  msrAlterationKind alterationKind)
{
  float result = -111.0;;

  switch (alterationKind) {
    case k_NoAlteration:
      break;

    case kTripleFlat:
      result = -3;
      break;
    case kDoubleFlat:
      result = -2;
      break;
    case kSesquiFlat:
      result = -1.5;
      break;
    case kFlat:
      result = -1;
      break;
    case kSemiFlat:
      result = -0.5;
      break;
    case kNatural:
      result = 0;
      break;
    case kSemiSharp:
      result = +0.5;
      break;
    case kSharp:
      result = 1;
      break;
    case kSesquiSharp:
      result = +1.5;
      break;
    case kDoubleSharp:
      result = +2;
      break;
    case kTripleSharp:
      result = +3;
      break;
  } // switch

  return result;
}

string msrAlterationKindAsString (
  msrAlterationKind alterationKind)
{
  string result;

  switch (alterationKind) {
    case k_NoAlteration:
      result = "noAlteration";
      break;

    case kTripleFlat:
      result = "tripleFlat";
      break;
    case kDoubleFlat:
      result = "doubleFlat";
      break;
    case kSesquiFlat:
      result = "sesquiFlat";
      break;
    case kFlat:
      result = "flat";
      break;
    case kSemiFlat:
      result = "semiFlat";
      break;
    case kNatural:
      result = "natural";
      break;
    case kSemiSharp:
      result = "semiSharp";
      break;
    case kSharp:
      result = "sharp";
      break;
    case kSesquiSharp:
      result = "sesquiSharp";
      break;
    case kDoubleSharp:
      result = "doubleSharp";
      break;
    case kTripleSharp:
      result = "tripleSharp";
      break;
  } // switch

  return result;
}

// accidentals
//______________________________________________________________________________
string accidentalKindAsString (
  msrAccidentalKind accidentalKind)
{
  string result;

  switch (accidentalKind) {
    case kAccidentalNone:
      result = "accidentalNone";
      break;

    case kAccidentalSharp:
      result = "accidentalSharp";
      break;
    case kAccidentalNatural:
      result = "accidentalNatural";
      break;
    case kAccidentalFlat:
      result = "accidentalFlat";
      break;
    case kAccidentalDoubleSharp:
      result = "accidentalDoubleSharp";
      break;
    case kAccidentalSharpSharp:
      result = "accidentalSharpSharp";
      break;
    case kAccidentalFlatFlat:
      result = "accidentalFlatFlat";
      break;
    case kAccidentalNaturalSharp:
      result = "accidentalNaturalSharp";
      break;
    case kAccidentalNaturalFlat:
      result = "accidentalNaturalFlat";
      break;
    case kAccidentalQuarterFlat:
      result = "accidentalQuarterFlat";
      break;
    case kAccidentalQuarterSharp:
      result = "accidentalQuarterSharp";
      break;
    case kAccidentalThreeQuartersFlat:
      result = "accidentalThreeQuartersFlat";
      break;
    case kAccidentalThreeQuartersSharp:
      result = "accidentalThreeQuartersSharp";
      break;

    case kAccidentalSharpDown:
      result = "accidentalSharpDown";
      break;
    case kAccidentalSharpUp:
      result = "accidentalSharpUp";
      break;
    case kAccidentalNaturalDown:
      result = "accidentalNaturalDown";
      break;
    case kAccidentalNaturalUp:
      result = "accidentalNaturalUp";
      break;
    case kAccidentalFlatDown:
      result = "accidentalFlatDown";
      break;
    case kAccidentalFlatUp:
      result = "accidentalFlatUp";
      break;
    case kAccidentalTripleSharp:
      result = "accidentalTripleSharp";
      break;
    case kAccidentalTripleFlat:
      result = "accidentalTripleFlat";
      break;
    case kAccidentalSlashQuarterSharp:
      result = "accidentalSlashQuarterSharp";
      break;
    case kAccidentalSlashSharp:
      result = "accidentalSlashSharp";
      break;
    case kAccidentalSlashFlat:
      result = "accidentalSlashFlat";
      break;
    case kAccidentalDoubleSlashFlat:
      result = "accidentaldoubleSlashFlat";
      break;
    case kAccidentalSharp_1:
      result = "accidentalSharp_1";
      break;
    case kAccidentalSharp_2:
      result = "accidentalSharp_2";
      break;
    case kAccidentalSharp_3:
      result = "accidentalSharp_3";
      break;
    case kAccidentalSharp_5:
      result = "accidentalSharp_5";
      break;
    case kAccidentalFlat_1:
      result = "accidentalFlat_1";
      break;
    case kAccidentalFlat_2:
      result = "accidentalFlat_2";
      break;
    case kAccidentalFlat_3:
      result = "accidentalFlat_3";
      break;
    case kAccidentalFlat_4:
      result = "accidentalFlat_4";
      break;
    case kAccidentalSori:
      result = "accidentalSori";
      break;
    case kAccidentalKoron:
      result = "accidentalKoron";
      break;

    case kAccidentalOther:
      result = "accidentalOther";
      break;
  } // switch

  return result;
}

string accidentalKindAsMusicXMLString (
  msrAccidentalKind accidentalKind)
{
  string result;

  switch (accidentalKind) {
    case kAccidentalNone:
      break;

    case kAccidentalSharp:
      result = "sharp";
      break;
    case kAccidentalNatural:
      result = "natural";
      break;
    case kAccidentalFlat:
      result = "flat";
      break;
    case kAccidentalDoubleSharp:
      result = "double-sharp";
      break;
    case kAccidentalSharpSharp:
      result = "sharp-sharp";
      break;
    case kAccidentalFlatFlat:
      result = "flat-flat";
      break;
    case kAccidentalNaturalSharp:
      result = "natural-sharp";
      break;
    case kAccidentalNaturalFlat:
      result = "natural-flat";
      break;
    case kAccidentalQuarterFlat:
      result = "quarter-flat";
      break;
    case kAccidentalQuarterSharp:
      result = "quarter-sharp";
      break;
    case kAccidentalThreeQuartersFlat:
      result = "three-quarters-flat";
      break;
    case kAccidentalThreeQuartersSharp:
      result = "three-quarters-sharp";
      break;

    case kAccidentalSharpDown:
      result = "sharp-down";
      break;
    case kAccidentalSharpUp:
      result = "sharp-up";
      break;
    case kAccidentalNaturalDown:
      result = "natural-down";
      break;
    case kAccidentalNaturalUp:
      result = "natural-up";
      break;
    case kAccidentalFlatDown:
      result = "flat-down";
      break;
    case kAccidentalFlatUp:
      result = "flat-up";
      break;
    case kAccidentalTripleSharp:
      result = "triple-sharp";
      break;
    case kAccidentalTripleFlat:
      result = "triple-flat";
      break;
    case kAccidentalSlashQuarterSharp:
      result = "slash-quarter-sharp";
      break;
    case kAccidentalSlashSharp:
      result = "slash-sharp";
      break;
    case kAccidentalSlashFlat:
      result = "slash-flat";
      break;
    case kAccidentalDoubleSlashFlat:
      result = "double-slash-flat";
      break;
    case kAccidentalSharp_1:
      result = "sharp-1";
      break;
    case kAccidentalSharp_2:
      result = "sharp-2";
      break;
    case kAccidentalSharp_3:
      result = "sharp-3";
      break;
    case kAccidentalSharp_5:
      result = "sharp-5";
      break;
    case kAccidentalFlat_1:
      result = "flat-1";
      break;
    case kAccidentalFlat_2:
      result = "flat-2";
      break;
    case kAccidentalFlat_3:
      result = "flat-3";
      break;
    case kAccidentalFlat_4:
      result = "flat-4";
      break;
    case kAccidentalSori:
      result = "sori";
      break;
    case kAccidentalKoron:
      result = "koron";
      break;

    case kAccidentalOther:
      result = "other";
      break;
  } // switch

  return result;
}

// editorial accidentals
//______________________________________________________________________________

string editorialAccidentalKindAsString (
  msrEditorialAccidentalKind noteEditorialAccidentalKind)
{
  string result;

  switch (noteEditorialAccidentalKind) {
    case kEditorialAccidentalYes:
      result = "noteEditorialAccidentalYes";
      break;
    case kEditorialAccidentalNo:
      result = "noteEditorialAccidentalNo";
      break;
  } // switch

  return result;
}

// cautionary accidentals
//______________________________________________________________________________

string cautionaryAccidentalKindAsString (
  msrCautionaryAccidentalKind noteCautionaryAccidentalKind)
{
  string result;

  switch (noteCautionaryAccidentalKind) {
    case kCautionaryAccidentalYes:
      result = "noteCautionaryAccidentalYes";
      break;
    case kCautionaryAccidentalNo:
      result = "noteCautionaryAccidentalNo";
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

void fetchDiatonicPitchKindAndAlterationKindFromQuarterTonesPitchKind (
  int                      inputLineNumber,
  msrQuarterTonesPitchKind quarterTonesPitchKind,
  msrDiatonicPitchKind&    diatonicPitchKind,
  msrAlterationKind&       alterationKind)
{
  diatonicPitchKind =
    diatonicPitchKindFromQuarterTonesPitchKind (
      inputLineNumber,
      quarterTonesPitchKind);

  alterationKind  =
    alterationKindFromQuarterTonesPitchKind (
      inputLineNumber,
      quarterTonesPitchKind);
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
              gGlobalOahOahGroup->getInputSourceName (),
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
              gGlobalOahOahGroup->getInputSourceName (),
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
              gGlobalOahOahGroup->getInputSourceName (),
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
              gGlobalOahOahGroup->getInputSourceName (),
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
              gGlobalOahOahGroup->getInputSourceName (),
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
              gGlobalOahOahGroup->getInputSourceName (),
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
              gGlobalOahOahGroup->getInputSourceName (),
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
          gGlobalOahOahGroup->getInputSourceName (),
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
          gGlobalOahOahGroup->getInputSourceName (),
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
          gGlobalOahOahGroup->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
          */
      }
  } // switch

  return result;
}

msrAlterationKind alterationKindFromQuarterTonesPitchKind (
  int                      inputLineNumber,
  msrQuarterTonesPitchKind quarterTonesPitchKind)
{
  msrAlterationKind result = k_NoAlteration;

  switch (quarterTonesPitchKind) {
    case kA_TripleFlat_QTP:
    case kB_TripleFlat_QTP:
    case kC_TripleFlat_QTP:
    case kD_TripleFlat_QTP:
    case kE_TripleFlat_QTP:
    case kF_TripleFlat_QTP:
    case kG_TripleFlat_QTP:
      result = kTripleFlat;
      break;

    case kA_DoubleFlat_QTP:
    case kB_DoubleFlat_QTP:
    case kC_DoubleFlat_QTP:
    case kD_DoubleFlat_QTP:
    case kE_DoubleFlat_QTP:
    case kF_DoubleFlat_QTP:
    case kG_DoubleFlat_QTP:
      result = kDoubleFlat;
      break;

    case kA_SesquiFlat_QTP:
    case kB_SesquiFlat_QTP:
    case kC_SesquiFlat_QTP:
    case kD_SesquiFlat_QTP:
    case kE_SesquiFlat_QTP:
    case kF_SesquiFlat_QTP:
    case kG_SesquiFlat_QTP:
      result = kSesquiFlat;
      break;

    case kA_Flat_QTP:
    case kB_Flat_QTP:
    case kC_Flat_QTP:
    case kD_Flat_QTP:
    case kE_Flat_QTP:
    case kF_Flat_QTP:
    case kG_Flat_QTP:
      result = kFlat;
      break;

    case kA_SemiFlat_QTP:
    case kB_SemiFlat_QTP:
    case kC_SemiFlat_QTP:
    case kD_SemiFlat_QTP:
    case kE_SemiFlat_QTP:
    case kF_SemiFlat_QTP:
    case kG_SemiFlat_QTP:
      result = kSemiFlat;
      break;

    case kA_Natural_QTP:
    case kB_Natural_QTP:
    case kC_Natural_QTP:
    case kD_Natural_QTP:
    case kE_Natural_QTP:
    case kF_Natural_QTP:
    case kG_Natural_QTP:
      result = kNatural;
      break;

    case kA_SemiSharp_QTP:
    case kB_SemiSharp_QTP:
    case kC_SemiSharp_QTP:
    case kD_SemiSharp_QTP:
    case kE_SemiSharp_QTP:
    case kF_SemiSharp_QTP:
    case kG_SemiSharp_QTP:
      result = kSemiSharp;
      break;

    case kA_Sharp_QTP:
    case kB_Sharp_QTP:
    case kC_Sharp_QTP:
    case kD_Sharp_QTP:
    case kE_Sharp_QTP:
    case kF_Sharp_QTP:
    case kG_Sharp_QTP:
      result = kSharp;
      break;

    case kA_SesquiSharp_QTP:
    case kB_SesquiSharp_QTP:
    case kC_SesquiSharp_QTP:
    case kD_SesquiSharp_QTP:
    case kE_SesquiSharp_QTP:
    case kF_SesquiSharp_QTP:
    case kG_SesquiSharp_QTP:
      result = kSesquiSharp;
      break;

    case kA_DoubleSharp_QTP:
    case kB_DoubleSharp_QTP:
    case kC_DoubleSharp_QTP:
    case kD_DoubleSharp_QTP:
    case kE_DoubleSharp_QTP:
    case kF_DoubleSharp_QTP:
    case kG_DoubleSharp_QTP:
      result = kDoubleSharp;
      break;

    case kA_TripleSharp_QTP:
    case kB_TripleSharp_QTP:
    case kC_TripleSharp_QTP:
    case kD_TripleSharp_QTP:
    case kE_TripleSharp_QTP:
    case kF_TripleSharp_QTP:
    case kG_TripleSharp_QTP:
      result = kTripleSharp;
      break;

    case k_Rest_QTP:
      {
        stringstream s;

        s <<
          "cannot get the alteration kind of a rest"
          ", line = " << inputLineNumber;

        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

    case k_NoQuarterTonesPitch_QTP:
      {
        result = k_NoAlteration;

        /* JMI
        stringstream s;

        s <<
          "cannot get the diatonic pitch of a k_NoQuarterTonesPitch_QTP"
          ", line = " << inputLineNumber;

        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
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
        result = kA_Natural_STP;
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
      result = gGlobalNederlandsPitchesNamesMap [quarterTonesPitchKind];
      break;
    case kCatalan:
      result = gGlobalCatalanPitchesNamesMap [quarterTonesPitchKind];
      break;
    case kDeutsch:
      result = gGlobalDeutschPitchesNamesMap [quarterTonesPitchKind];
      break;
    case kEnglish:
      result = gGlobalEnglishPitchesNamesMap [quarterTonesPitchKind];
      break;
    case kEspanol:
      result = gGlobalEspanolPitchesNamesMap [quarterTonesPitchKind];
      break;
    case kFrancais:
      result = gGlobalFrancaisPitchesNamesMap [quarterTonesPitchKind];
      break;
    case kItaliano:
      result = gGlobalItalianoPitchesNamesMap [quarterTonesPitchKind];
      break;
    case kNorsk:
      result = gGlobalNorskPitchesNamesMap [quarterTonesPitchKind];
      break;
    case kPortugues:
      result = gGlobalPortuguesPitchesNamesMap [quarterTonesPitchKind];
      break;
    case kSuomi:
      result = gGlobalSuomiPitchesNamesMap [quarterTonesPitchKind];
      break;
    case kSvenska:
      result = gGlobalSvenskaPitchesNamesMap [quarterTonesPitchKind];
      break;
    case kVlaams:
      result = gGlobalVlaamsPitchesNamesMap [quarterTonesPitchKind];
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

  // select the relevant pitch names map
  switch (languageKind) {
    case kNederlands:
      pitchNamesMapPTR = &gGlobalNederlandsPitchesNamesMap;
      break;
    case kCatalan:
      pitchNamesMapPTR = &gGlobalCatalanPitchesNamesMap;
      break;
    case kDeutsch:
      pitchNamesMapPTR = &gGlobalDeutschPitchesNamesMap;
      break;
    case kEnglish:
      pitchNamesMapPTR = &gGlobalEnglishPitchesNamesMap;
      break;
    case kEspanol:
      pitchNamesMapPTR = &gGlobalEspanolPitchesNamesMap;
      break;
    case kFrancais:
      pitchNamesMapPTR = &gGlobalFrancaisPitchesNamesMap;
      break;
    case kItaliano:
      pitchNamesMapPTR = &gGlobalItalianoPitchesNamesMap;
      break;
    case kNorsk:
      pitchNamesMapPTR = &gGlobalNorskPitchesNamesMap;
      break;
    case kPortugues:
      pitchNamesMapPTR = &gGlobalPortuguesPitchesNamesMap;
      break;
    case kSuomi:
      pitchNamesMapPTR = &gGlobalSuomiPitchesNamesMap;
      break;
    case kSvenska:
      pitchNamesMapPTR = &gGlobalSvenskaPitchesNamesMap;
      break;
    case kVlaams:
      pitchNamesMapPTR = &gGlobalVlaamsPitchesNamesMap;
      break;
  } // switch

  // is quarterTonesPitchName present in the map?
  if (pitchNamesMapPTR->size ()) {
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
        gGlobalLpsrOahGroup->
          getLpsrQuarterTonesPitchesLanguageKind (),
        theString);

  // fetch the semitones pitches kind
  result =
    semiTonesPitchKindFromQuarterTonesPitchKind (
      quarterTonesPitchKindFromString);

  return result;
}

string existingQuarterTonesPitchesLanguageKinds (int namesListMaxLength)
{
  stringstream s;

  int
    quarterTonesPitchesLanguageKindsMapSize =
      gGlobalQuarterTonesPitchesLanguageKindsMap.size ();

  if (quarterTonesPitchesLanguageKindsMapSize) {
    int
      nextToLast =
        quarterTonesPitchesLanguageKindsMapSize - 1;

    int count = 0;
    int cumulatedLength = 0;

    for (
      map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator i =
        gGlobalQuarterTonesPitchesLanguageKindsMap.begin ();
      i != gGlobalQuarterTonesPitchesLanguageKindsMap.end ();
      i++
    ) {
      string theString = (*i).first;

      count++;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
      }

      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != quarterTonesPitchesLanguageKindsMapSize) {
        s << ", ";
      }
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

string msrSemiTonesPitchKindAsFlatsAndSharps (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrSemiTonesPitchKind              semiTonesPitchKind)
{
  string result;

/* JMI
  msrDiatonicPitchKind
    diatonicPitchKind =
      diatonicPitchKindFromQuarterTonesPitchKind (
        inputLineNumber,
        quarterTonesPitchKind);
        */

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

// length units
//______________________________________________________________________________

map<string, msrLengthUnitKind>
  gGlobalMsrLengthUnitKindsMap;

string msrLengthUnitKindAsString (
  msrLengthUnitKind lengthUnitKind)
{
  string result;

  // no CamelCase here, these strings are used in the command line options

  switch (lengthUnitKind) {
    case kInchUnit:
      result = "in";
      break;
    case kCentimeterUnit:
      result = "cm";
      break;
    case kMillimeterUnit: // default value
      result = "mm";
      break;
  } // switch

  return result;
}

void initializeMsrLengthUnitKindsMap ()
{
  // register the LilyPond score output kinds
  // --------------------------------------

  // no CamelCase here, these strings are used in the command line options

  gGlobalMsrLengthUnitKindsMap ["in"] = kInchUnit;
  gGlobalMsrLengthUnitKindsMap ["cm"] = kCentimeterUnit;
  gGlobalMsrLengthUnitKindsMap ["mm"] = kMillimeterUnit;
}

string existingMsrLengthUnitKinds (int namesListMaxLength)
{
  stringstream s;

  int
    msrLengthUnitKindsMapSize =
      gGlobalMsrLengthUnitKindsMap.size ();

  if (msrLengthUnitKindsMapSize) {
    int
      nextToLast =
        msrLengthUnitKindsMapSize - 1;

    int count = 0;
    int cumulatedLength = 0;

    for (
      map<string, msrLengthUnitKind>::const_iterator i =
        gGlobalMsrLengthUnitKindsMap.begin ();
      i != gGlobalMsrLengthUnitKindsMap.end ();
      i++
    ) {
      string theString = (*i).first;

      count++;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
      }

      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != msrLengthUnitKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

// lengths
//______________________________________________________________________________
S_msrLength msrLength::create (
  msrLengthUnitKind lengthUnitKind,
  float             lengthValue)
{
  msrLength * o =
    new msrLength (
      lengthUnitKind,
      lengthValue);
  assert (o!=0);

  return o;
}

msrLength::msrLength (
  msrLengthUnitKind lengthUnitKind,
  float             lengthValue)
{
  fLengthUnitKind = lengthUnitKind;
  fLengthValue    = lengthValue;
}

msrLength::msrLength ()
{
  fLengthUnitKind = kMillimeterUnit;
  fLengthValue    = 0.0;
}

msrLength::~msrLength ()
{}

void msrLength::convertToLengthUnit (
  msrLengthUnitKind lengthUnitKind)
{
  if (fLengthUnitKind != lengthUnitKind) {
    switch (lengthUnitKind) {
      case kInchUnit:
        switch (fLengthUnitKind) {
          case kInchUnit:
            break;
          case kCentimeterUnit:
            fLengthValue /= 2.54f;
            break;
          case kMillimeterUnit:
            fLengthValue /= 25.4f;
            break;
        } // switch
        break;

      case kCentimeterUnit:
        switch (fLengthUnitKind) {
          case kInchUnit:
            fLengthValue *= 2.54f;
            break;
          case kCentimeterUnit:
            break;
          case kMillimeterUnit:
            fLengthValue /= 10;
            break;
        } // switch
        break;

      case kMillimeterUnit:
        switch (fLengthUnitKind) {
          case kInchUnit:
            fLengthValue *= 25.4f;
            break;
          case kCentimeterUnit:
            fLengthValue *= 10;
            break;
          case kMillimeterUnit:
            break;
        } // switch
        break;
    } // switch

    fLengthUnitKind = lengthUnitKind;
  }
}

string msrLength::asString () const
{
  stringstream s;

  s <<
    "'" <<
    setprecision (4) <<
    fLengthValue <<
    " " <<
    msrLengthUnitKindAsString (fLengthUnitKind) <<
    "'";

  return s.str ();
}

void msrLength::print (ostream& os) const
{
  os <<
    asString (); // JMI
};

ostream& operator<< (ostream& os, const msrLength& elt)
{
  elt.print (os);
  return os;
}

ostream& operator<< (ostream& os, const S_msrLength& elt)
{
  elt->print (os);
  return os;
}

// margins types
//______________________________________________________________________________

map<string, msrMarginTypeKind>
  gGlobalMsrMarginTypeKindsMap;

string msrMarginTypeKindAsString (
  msrMarginTypeKind marginTypeKind)
{
  string result;

  // no CamelCase here, these strings are used in the command line options

  switch (marginTypeKind) {
    case kOddMargin:
      result = "odd";
      break;
    case kEvenMargin:
      result = "even";
      break;
    case kBothMargins: // default value
      result = "both";
      break;
  } // switch

  return result;
}

void initializeMsrMarginTypeKindsMap ()
{
  // register the LilyPond score output kinds
  // --------------------------------------

  // no CamelCase here, these strings are used in the command line options

  gGlobalMsrMarginTypeKindsMap ["odd"] = kOddMargin;
  gGlobalMsrMarginTypeKindsMap ["even"] = kEvenMargin;
  gGlobalMsrMarginTypeKindsMap ["both"] = kBothMargins;
}

string existingMsrMarginTypeKinds (int namesListMaxLength)
{
  stringstream s;

  int
    msrMarginTypeKindsMapSize =
      gGlobalMsrMarginTypeKindsMap.size ();

  if (msrMarginTypeKindsMapSize) {
    int
      nextToLast =
        msrMarginTypeKindsMapSize - 1;

    int count = 0;
    int cumulatedLength = 0;

    for (
      map<string, msrMarginTypeKind>::const_iterator i =
        gGlobalMsrMarginTypeKindsMap.begin ();
      i != gGlobalMsrMarginTypeKindsMap.end ();
      i++
    ) {
      string theString = (*i).first;

      count++;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
      }

      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != msrMarginTypeKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

// margins
//______________________________________________________________________________
S_msrMargin msrMargin::create (
  msrMarginTypeKind marginTypeKind,
  msrLength         marginLength)
{
  msrMargin * o =
    new msrMargin (
      marginTypeKind,
      marginLength);
  assert (o!=0);

  return o;
}

msrMargin::msrMargin (
  msrMarginTypeKind marginTypeKind,
  msrLength         marginLength)
{
  fMarginTypeKind = marginTypeKind;
  fMarginLength   = marginLength;
}

msrMargin::~msrMargin ()
{}

string msrMargin::asString () const
{
  stringstream s;

  s <<
    "[Margin, " <<
    setprecision (4) <<
    fMarginLength.asString () <<
    " " <<
    msrMarginTypeKindAsString (fMarginTypeKind) <<
    "]";

  return s.str ();
}

void msrMargin::print (ostream& os) const
{
  os <<
    asString ();
};

ostream& operator<< (ostream& os, const S_msrMargin& elt)
{
  elt->print (os);
  return os;
}

// margins groups
//______________________________________________________________________________
S_msrMarginsGroup msrMarginsGroup::create (
  msrMarginTypeKind marginTypeKind)
{
  msrMarginsGroup * o =
    new msrMarginsGroup (
      marginTypeKind);
  assert (o!=0);

  return o;
}

msrMarginsGroup::msrMarginsGroup (
  msrMarginTypeKind marginTypeKind)
{
  fMarginsGroupTypeKind = marginTypeKind;
}

msrMarginsGroup::~msrMarginsGroup ()
{}

void msrMarginsGroup::setLeftMargin (
  int         inputLineNumber,
  S_msrMargin val)
{
  msrMarginTypeKind
    marginTypeKind =
      val->getMarginTypeKind ();

  if (marginTypeKind != fMarginsGroupTypeKind) {
    stringstream s;

    s <<
      "setting a " <<
      msrMarginTypeKindAsString (marginTypeKind) <<
      " left margin in a " <<
      msrMarginTypeKindAsString (fMarginsGroupTypeKind) <<
      " margins group";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fLeftMargin = val;
}

void msrMarginsGroup::setRightMargin (
  int         inputLineNumber,
  S_msrMargin val)
{
  msrMarginTypeKind
    marginTypeKind =
      val->getMarginTypeKind ();

  if (marginTypeKind != fMarginsGroupTypeKind) {
    stringstream s;

    s <<
      "setting a " <<
      msrMarginTypeKindAsString (marginTypeKind) <<
      " right margin in a " <<
      msrMarginTypeKindAsString (fMarginsGroupTypeKind) <<
      " margins group";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fRightMargin = val;
}

void msrMarginsGroup::setTopMargin (
  int         inputLineNumber,
  S_msrMargin val)
{
  msrMarginTypeKind
    marginTypeKind =
      val->getMarginTypeKind ();

  if (marginTypeKind != fMarginsGroupTypeKind) {
    stringstream s;

    s <<
      "setting a " <<
      msrMarginTypeKindAsString (marginTypeKind) <<
      " top margin in a " <<
      msrMarginTypeKindAsString (fMarginsGroupTypeKind) <<
      " margins group";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fTopMargin = val;
}

void msrMarginsGroup::setBottomMargin (
  int         inputLineNumber,
  S_msrMargin val)
{
  msrMarginTypeKind
    marginTypeKind =
      val->getMarginTypeKind ();

  if (marginTypeKind != fMarginsGroupTypeKind) {
    stringstream s;

    s <<
      "setting a " <<
      msrMarginTypeKindAsString (marginTypeKind) <<
      " bottom margin in a " <<
      msrMarginTypeKindAsString (fMarginsGroupTypeKind) <<
      " margins group";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fBottomMargin = val;
}

/* JMI
void msrMarginsGroup::acceptIn (basevisitor* v) {
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMarginsGroup::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrMarginsGroup>*
    p =
      dynamic_cast<visitor<S_msrMarginsGroup>*> (v)) {
        S_msrMarginsGroup elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMarginsGroup::visitStart ()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrMarginsGroup::acceptOut (basevisitor* v) {
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMarginsGroup::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMarginsGroup>*
    p =
      dynamic_cast<visitor<S_msrMarginsGroup>*> (v)) {
        S_msrMarginsGroup elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMarginsGroup::visitEnd ()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrMarginsGroup::browseData (basevisitor* v)
{}
*/

string msrMarginsGroup::asString () const
{
  stringstream s;

  s <<
    "[MarginsGroup, " <<
    msrMarginTypeKindAsString (fMarginsGroupTypeKind) <<
    ", leftMargin: " << fLeftMargin->asString () <<
    ", rightMargin: " << fRightMargin->asString () <<
    ", topMargin: " << fTopMargin->asString () <<
    ", bottomMargin: " << fBottomMargin->asString () <<
    "]";

  return s.str ();
}

void msrMarginsGroup::print (ostream& os) const
{

  os <<
    "MarginsGroup" <<
    ", marginsGroupTypeKind: " <<
    msrMarginTypeKindAsString (fMarginsGroupTypeKind) <<
    endl;

  const int fieldWidth = 13;

  gIndenter++;

  // margins
  os << left <<
    setw (fieldWidth) <<
    "leftMargin" << " : ";
    if (fLeftMargin) {
      os << fLeftMargin;
    }
    else {
      os << "none";
    }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "rightMargin" << " : ";
    if (fRightMargin) {
      os << fRightMargin;
    }
    else {
      os << "none";
    }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "topMargin" << " : ";
    if (fTopMargin) {
      os << fTopMargin;
    }
    else {
      os << "none";
    }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "bottomMargin" << " : ";
    if (fBottomMargin) {
      os << fBottomMargin;
    }
    else {
      os << "none";
    }
  os << endl;

  gIndenter--;
};

ostream& operator<< (ostream& os, const S_msrMarginsGroup& elt)
{
  elt->print (os);
  return os;
}

// font size
//______________________________________________________________________________
S_msrFontSize msrFontSize::create (
  msrFontSizeKind fontSizeKind)
{
  msrFontSize * o =
    new msrFontSize (
      fontSizeKind);
  assert (o!=0);

  return o;
}

S_msrFontSize msrFontSize::create (
  float numericFontSize)
{
  msrFontSize * o =
    new msrFontSize (
      numericFontSize);
  assert (o!=0);

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

float msrFontSize::getFontNumericSize () const
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
          gGlobalOahOahGroup->getInputSourceName (),
          K_NO_INPUT_LINE_NUMBER, // JMI
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

void msrFontSize::print (ostream& os) const
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
msrFontStyleKind msrFontStyleKindFromString (
  int    inputLineNumber,
  string fontStyleString)
{
  msrFontStyleKind result = kFontStyleNone; // default value

  if      (fontStyleString == "normal")
    result = kFontStyleNormal;
  else if (fontStyleString == "italic")
    result = KFontStyleItalic;
  else {
    if (fontStyleString.size ()) {
      stringstream s;

      s <<
        "style value " << fontStyleString <<
        " should be 'normal' or 'italic'";

      msrMusicXMLError (
        gGlobalOahOahGroup->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  return result;
}

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
msrFontWeightKind msrFontWeightKindFromString (
  int    inputLineNumber,
  string fontWeightString)
{
  msrFontWeightKind result = kFontWeightNone; // default value

  if      (fontWeightString == "normal")
    result = kFontWeightNormal;
  else if (fontWeightString == "bold")
    result = kFontWeightBold;
  else {
    if (fontWeightString.size ()) {
      stringstream s;

      s <<
        "font weight value " << fontWeightString <<
        " should be 'normal' or 'bold'";

      msrMusicXMLError (
        gGlobalOahOahGroup->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  return result;
}

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
msrJustifyKind msrJustifyKindFromString (
  int    inputLineNumber,
  string justifyString)
{
  msrJustifyKind result = kJustifyNone; // default value

  if      (justifyString == "left")
    result = kJustifyLeft;
  else if (justifyString == "center")
    result = kJustifyCenter;
  else if (justifyString == "right")
    result = kJustifyRight;
  else {
    if (justifyString.size ()) {
      stringstream s;

      s <<
        "justify value " << justifyString <<
        " should be 'left', 'center' or 'right'";

      msrMusicXMLError (
        gGlobalOahOahGroup->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  return result;
}

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

msrHorizontalAlignmentKind msrHorizontalAlignmentKindFromString (
  int    inputLineNumber,
  string horizontalAlignmentString)
{
  msrHorizontalAlignmentKind
    result = kHorizontalAlignmentNone; // default value

  if      (horizontalAlignmentString == "left")
    result = kHorizontalAlignmentLeft;
  else if (horizontalAlignmentString == "center")
    result = kHorizontalAlignmentCenter;
  else if (horizontalAlignmentString == "right")
    result = kHorizontalAlignmentRight;
  else {
    if (horizontalAlignmentString.size ()) {
      stringstream s;

      s <<
        "halign value " << horizontalAlignmentString <<
        " should be 'left', 'center' or 'right'";

      msrMusicXMLError (
        gGlobalOahOahGroup->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  return result;
}

string msrHorizontalAlignmentKindAsString (
  msrHorizontalAlignmentKind horizontalAlignmentKind)
{
  string result;

  switch (horizontalAlignmentKind) {
    case kHorizontalAlignmentNone:
      result = "horizontalAlignmentNone";
      break;
    case kHorizontalAlignmentLeft:
      result = "horizontalAlignmentLeft";
      break;
    case kHorizontalAlignmentCenter:
      result = "horizontalAlignmentCenter";
      break;
    case kHorizontalAlignmentRight:
      result = "horizontalAlignmentRight";
      break;
    } // switch

  return result;
}

msrVerticalAlignmentKind msrVerticalAlignmentKindFromString (
  int    inputLineNumber,
  string verticalAlignmentString)
{
  msrVerticalAlignmentKind
    result = kVerticalAlignmentNone; // default value

  if      (verticalAlignmentString == "top")
    result = kVerticalAlignmentTop;
  else if (verticalAlignmentString == "middle")
    result = kVerticalAlignmentMiddle;
  else if (verticalAlignmentString == "bottom")
    result = kVerticalAlignmentBottom;
  else {
    if (verticalAlignmentString.size ()) {
      stringstream s;

      s <<
        "valign value " << verticalAlignmentString <<
        " should be 'top', 'middle' or 'bottom'";

      msrMusicXMLError (
        gGlobalOahOahGroup->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

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


// print object
//______________________________________________________________________________
msrPrintObjectKind msrPrintObjectKindFromString (
  int    inputLineNumber,
  string printObjectString)
{
  msrPrintObjectKind result = kPrintObjectNone; // default value KAKA JMI

  if      (printObjectString == "yes")
    result = kPrintObjectYes;
  else if (printObjectString == "no")
    result = kPrintObjectNo;
  else {
    if (printObjectString.size ()) {
      stringstream s;

      s <<
        "printObject \"" << printObjectString <<
        "\" should be 'above' or 'below'";

      msrMusicXMLError (
        gGlobalOahOahGroup->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  return result;
}

string msrPrintObjectKindAsString (
  msrPrintObjectKind printObjectKind)
{
  string result;

  switch (printObjectKind) {
    case msrPrintObjectKind::kPrintObjectNone:
      result = "printObjectNone";
      break;
    case msrPrintObjectKind::kPrintObjectYes:
      result = "printObjectYes";
      break;
    case msrPrintObjectKind::kPrintObjectNo:
      result = "printObjectNo";
      break;
  } // switch

  return result;
}

// placement
//______________________________________________________________________________
msrPlacementKind msrPlacementKindFromString (
  int    inputLineNumber,
  string placementString)
{
  msrPlacementKind result = kPlacementNone; // default value

  if      (placementString == "above")
    result = kPlacementAbove;
  else if (placementString == "below")
    result = kPlacementBelow;
  else {
    if (placementString.size ()) {
      stringstream s;

      s <<
        "placement \"" << placementString <<
        "\" should be 'above' or 'below'";

      msrMusicXMLError (
        gGlobalOahOahGroup->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  return result;
}

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

// measure style
//______________________________________________________________________________
msrUseDotsKind msrUseDotsFromString (
  int    inputLineNumber,
  string useDotsString)
{
  msrUseDotsKind result = kUseDotsNo; // default value

  if      (useDotsString == "yes")
    result = kUseDotsYes;
  else if (useDotsString == "no")
    result = kUseDotsNo;
  else {
    if (useDotsString.size ()) {
      stringstream s;

      s <<
        "use-dots \"" << useDotsString <<
        "\" is unknown";

      msrMusicXMLError (
        gGlobalOahOahGroup->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  return result;
}

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

string msrUseDotsKindAsString (
  msrUseDotsKind useDotsKind)
{
  string result;

  switch (useDotsKind) {
    case k_NoUseDots:
      result = "noUseDots";
      break;
    case kUseDotsYes:
      result = "useDotsYes";
      break;
    case kUseDotsNo:
      result = "useDotsNo";
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
    case k_NoSpannerType:
      result = "noSpannerType";
      break;
    case kSpannerTypeStart:
      result = "spannerTypeStart";
      break;
    case kSpannerTypeStop:
      result = "spannerTypeStop";
      break;
    case kSpannerTypeContinue:
      result = "spannerTypeContinue";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
S_msrHarmonyInterval msrHarmonyInterval::create (
// JMI  int             inputLineNumber,
  msrIntervalKind harmonyIntervalIntervalKind,
  int             harmonyIntervalRelativeOctave)
{
  msrHarmonyInterval* o =
    new msrHarmonyInterval (
 //     inputLineNumber,
 //     harmonyIntervalNumber,
      harmonyIntervalIntervalKind,
      harmonyIntervalRelativeOctave);
  assert (o!=0);

  return o;
}

msrHarmonyInterval::msrHarmonyInterval (
// JMI  int             inputLineNumber,
  msrIntervalKind harmonyIntervalIntervalKind,
  int             harmonyIntervalRelativeOctave)
  // JMI  : msrElement (inputLineNumber)
{
  fHarmonyIntervalIntervalKind = harmonyIntervalIntervalKind;

  fHarmonyIntervalRelativeOctave = harmonyIntervalRelativeOctave;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceHarmoniesDetails ()) {
    gLogStream <<
      "==> Creating harmony item '" <<
      harmonyIntervalAsString () <<
      "'" <<
      endl;
  }
#endif
}

msrHarmonyInterval::~msrHarmonyInterval ()
{}

S_msrHarmonyInterval msrHarmonyInterval::createHarmonyIntervalNewbornClone ()
{
  S_msrHarmonyInterval
    newbornClone =
      msrHarmonyInterval::create (
        fHarmonyIntervalIntervalKind,
        fHarmonyIntervalRelativeOctave);

  return newbornClone;
}

string msrHarmonyInterval::harmonyIntervalAsString () const
{
  stringstream s;

  const int fieldWidth = 19;

  s << left <<
    "HarmonyInterval" <<
    ": " <<
    setw (fieldWidth) <<
    msrIntervalKindAsString (fHarmonyIntervalIntervalKind) <<
    "harmonyIntervalRelativeOctave: " << fHarmonyIntervalRelativeOctave;

  return s.str ();
}

string msrHarmonyInterval::harmonyIntervalAsShortString () const
{
  return
    msrIntervalKindAsString (fHarmonyIntervalIntervalKind);
}


void msrHarmonyInterval::normalizeInterval ()
{
  // bring the interval below an octave
  if (fHarmonyIntervalIntervalKind > kAugmentedSeventh) {
    fHarmonyIntervalIntervalKind =
      msrIntervalKind (
        fHarmonyIntervalIntervalKind - kAugmentedSeventh);

    fHarmonyIntervalRelativeOctave += 1;
  }
}

void msrHarmonyInterval::deNormalizeInterval ()
{
  // bring the interval above the octave if its relative octave is 1
  if (
    fHarmonyIntervalRelativeOctave == 1
      &&
    fHarmonyIntervalIntervalKind <= kAugmentedSeventh) {
    fHarmonyIntervalIntervalKind =
      msrIntervalKind (
        fHarmonyIntervalIntervalKind + kAugmentedSeventh);

    fHarmonyIntervalRelativeOctave -= 1;
  }
}

S_msrHarmonyInterval msrHarmonyInterval::intervalDifference (
  S_msrHarmonyInterval otherHarmonyInterval)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceExtraHarmonies ()) {
    gLogStream <<
      endl <<
      "--> computing intervalDifference betwwen '" <<
      asShortString () <<
      "' and '" <<
      otherHarmonyInterval->asShortString () <<
      "'" <<
      endl;
  }
#endif

  msrIntervalKind resultIntervalKind   = k_NoIntervalKind;

  S_msrHarmonyInterval
    operand1 =
      this->createHarmonyIntervalNewbornClone (),
    operand2 =
      otherHarmonyInterval->createHarmonyIntervalNewbornClone ();

  // normalize both intervals
  operand1->
    normalizeInterval ();
  operand2->
    normalizeInterval ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceExtraHarmonies ()) {
    gLogStream <<
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
    relativeOctave1 = operand1->fHarmonyIntervalRelativeOctave,
    relativeOctave2 = operand2->fHarmonyIntervalRelativeOctave;

  // fetch the interval kind
  msrIntervalKind
    intervalKind1 =
      operand1->fHarmonyIntervalIntervalKind,
    intervalKind2 =
      operand2->fHarmonyIntervalIntervalKind;

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceExtraHarmonies ()) {
    gLogStream <<
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceExtraHarmonies ()) {
    gLogStream <<
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
  S_msrHarmonyInterval
    result =
      msrHarmonyInterval::create (
        resultIntervalKind,
        resultRelativeOctave);

  // denormalize it, in order to get intervals
  // greater than an augmented seventh if applicable
  result->deNormalizeInterval ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceExtraHarmonies ()) {
    gLogStream <<
      "--> result = '" <<
      result->asShortString () <<
      "'" <<
      endl << endl;
  }
#endif

  // return it;
  return result;
}

S_msrHarmonyInterval msrHarmonyInterval::intervalSum (
  S_msrHarmonyInterval otherHarmonyInterval)
{
  msrIntervalKind resultIntervalKind   = k_NoIntervalKind;
  int             resultRelativeOctave = 0;

  msrIntervalKind
    intervalKind1 = fHarmonyIntervalIntervalKind,
    intervalKind2 = otherHarmonyInterval->fHarmonyIntervalIntervalKind;

  int
    relativeOctave1 = fHarmonyIntervalRelativeOctave,
    relativeOctave2 = otherHarmonyInterval->fHarmonyIntervalRelativeOctave;

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
    msrHarmonyInterval::create (
      resultIntervalKind,
      resultRelativeOctave);
}

/* JMI
void msrHarmonyInterval::acceptIn (basevisitor* v) {
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmonyInterval::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrHarmonyInterval>*
    p =
      dynamic_cast<visitor<S_msrHarmonyInterval>*> (v)) {
        S_msrHarmonyInterval elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmonyInterval::visitStart ()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrHarmonyInterval::acceptOut (basevisitor* v) {
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmonyInterval::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrHarmonyInterval>*
    p =
      dynamic_cast<visitor<S_msrHarmonyInterval>*> (v)) {
        S_msrHarmonyInterval elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmonyInterval::visitEnd ()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrHarmonyInterval::browseData (basevisitor* v)
{}
*/

string msrHarmonyInterval::asString () const
{
  stringstream s;

  s <<
    "HarmonyInterval '" <<
    msrIntervalKindAsString (fHarmonyIntervalIntervalKind) <<
    ", rel. oct. " <<
    fHarmonyIntervalRelativeOctave <<
    "'";

  return s.str ();
}

string msrHarmonyInterval::asShortString () const
{
  stringstream s;

  s <<
    "'" <<
    msrIntervalKindAsString (fHarmonyIntervalIntervalKind) <<
    ", rel.oct. " <<
    fHarmonyIntervalRelativeOctave <<
    "'";

  return s.str ();
}

void msrHarmonyInterval::print (ostream& os) const
{
  os <<
    "HarmonyInterval" <<
    endl;

  gIndenter++;

  const int fieldWidth = 22;

  os << left <<
  /* JMI
    setw (fieldWidth) <<
    "harmonyIntervalNumber" << " : " << fHarmonyIntervalNumber <<
    endl <<
    */
    setw (fieldWidth) <<
    "harmonyIntervalIntervalKind" << " : " <<
      msrIntervalKindAsString (fHarmonyIntervalIntervalKind) <<
    endl <<
    setw (fieldWidth) <<
    "harmonyIntervalRelativeOctave" << " : " << fHarmonyIntervalRelativeOctave <<
  /* JMI
    ", line: " << fInputLineNumber <<
    */
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrHarmonyInterval& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
map<msrHarmonyKind, S_msrHarmonyStructure>
  gGlobalHarmonyStructuresMap;

S_msrHarmonyStructure msrHarmonyStructure::createBare (
  msrHarmonyKind harmonyStructureHarmonyKind)
{
  msrHarmonyStructure* o =
    new msrHarmonyStructure (
      harmonyStructureHarmonyKind);
  assert (o!=0);

  return o;
}

S_msrHarmonyStructure msrHarmonyStructure::create (
  msrHarmonyKind harmonyStructureHarmonyKind)
{
  S_msrHarmonyStructure o =
    createBare (
      harmonyStructureHarmonyKind);

  o->
    populateHarmonyStructure ();

  return o;
}

msrHarmonyStructure::msrHarmonyStructure (
  msrHarmonyKind harmonyStructureHarmonyKind)
{
  fHarmonyStructureHarmonyKind = harmonyStructureHarmonyKind;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceHarmoniesDetails ()) {
    gLogStream <<
      "==> Creating harmony intervals '" <<
      harmonyStructureAsString () <<
      "'" <<
      endl;
  }
#endif
}

S_msrHarmonyStructure msrHarmonyStructure::createHarmonyStructureNewbornClone ()
{
  S_msrHarmonyStructure
    newbornClone =
      createBare (
        fHarmonyStructureHarmonyKind);

  return newbornClone;
}

void msrHarmonyStructure::populateHarmonyStructure ()
{
  // append harmony items to harmony intervals
  switch (fHarmonyStructureHarmonyKind) {
    case k_NoHarmony:
      break;

    // MusicXML harmonies

    case kMajorHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
      }
      break;

    case kMinorHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
      }
      break;

    case kAugmentedHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kAugmentedFifth)
          );
      }
      break;

    case kDiminishedHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kDiminishedFifth)
          );
      }
      break;

    case kDominantHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorSeventh)
          );
      }
      break;

    case kMajorSeventhHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorSeventh)
          );
      }
      break;

     case kMinorSeventhHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorSeventh)
          );
      }
      break;

    case kDiminishedSeventhHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kDiminishedFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kDiminishedSeventh)
          );
      }
      break;

    case kAugmentedSeventhHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kAugmentedFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorSeventh)
          );
      }
      break;

    case kHalfDiminishedHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kDiminishedFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorSeventh)
          );
      }
      break;

     case kMinorMajorSeventhHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorSeventh)
          );
      }
      break;

    case kMajorSixthHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorSixth)
          );
      }
      break;

    case kMinorSixthHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorSixth)
          );
      }
      break;

    case kDominantNinthHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorNinth)
          );
      }
      break;

    case kMajorNinthHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorNinth)
          );
      }
      break;

    case kMinorNinthHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorNinth)
          );
      }
      break;

    case kDominantEleventhHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectEleventh)
          );
      }
      break;

    case kMajorEleventhHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectEleventh)
          );
      }
      break;

    case kMinorEleventhHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectEleventh)
          );
      }
      break;

    case kDominantThirteenthHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorThirteenth)
          );
      }
      break;

    case kMajorThirteenthHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorThirteenth)
          );
      }
      break;

    case kMinorThirteenthHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorThirteenth)
          );
      }
      break;

    case kSuspendedSecondHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorSecond)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
      }
      break;

    case kSuspendedFourthHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFourth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
      }
      break;

/*
* kNeapolitan f aes des' in:
*
* c e g c' -> f f aes des' -> d g d b -> c e g c'

they are three different pre-dominant harmonies that are taught to American undergrads in a sophomore theory course.

in E major:
Italian = C E A#
French = C E F# A#
German = C E G A#
Tristan = C D# F# A#

in all of them, the C and A# in theory want to fan out to B (the dominant).  This is, of course, in theory - Wagner’s use of the Tristan harmony, which he clearly named his opera after, has the A# moving down to A, or the 7th of the dominant (I’m transposing to fit w/ the example above).  Wagner obviously did not pay much attention during his sophomore music theory course…
*/

    case kNeapolitanHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kDiminishedSixth)
          );
      }
      break;

    case kItalianHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kAugmentedSixth)
          );
      }
      break;

    case kFrenchHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kAugmentedFourth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kAugmentedSixth)
          );
      }
      break;

    case kGermanHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kAugmentedSixth)
          );
      }
      break;

    case kPedalHarmony:
      break;

    case kPowerHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
      }
      break;

    case kTristanHarmony:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kAugmentedSecond)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kAugmentedFourth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kAugmentedSixth)
          );
      }
      break;

    // jazz-specific harmonies

    case kMinorMajorNinth: // -maj9, minmaj9
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorNinth)
          );
      }
      break;

    case kDominantSuspendedFourthHarmony: // 7sus4, domsus4
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFourth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorSeventh)
          );
      }
      break;

    case kDominantAugmentedFifthHarmony: // 7#5, domaug5
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFourth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kAugmentedFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorSeventh)
          );
      }
      break;

    case kDominantMinorNinthHarmony: // 7b9, dommin9
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorNinth)
          );
      }
      break;

    case kDominantAugmentedNinthDiminishedFifthHarmony: // 7#9b5, domaug9dim5
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kDiminishedFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kAugmentedNinth)
          );
      }
      break;

    case kDominantAugmentedNinthAugmentedFifthHarmony: // 7#9#5, domaug9aug5
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kAugmentedFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kAugmentedNinth)
          );
      }
      break;

    case kDominantAugmentedEleventhHarmony: // 7#11, domaug11
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFourth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMinorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kAugmentedEleventh)
          );
      }
      break;

    case kMajorSeventhAugmentedEleventhHarmony: // maj7#11, maj7aug11
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectUnison)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFourth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            kMajorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
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
  // register harmony intervals in map
  gGlobalHarmonyStructuresMap [fHarmonyStructureHarmonyKind] = this;
  */
}

msrHarmonyStructure::~msrHarmonyStructure ()
{}

void msrHarmonyStructure::appendHarmonyIntervalToHarmonyStructure (
  S_msrHarmonyInterval harmonyInterval)
{
  // set the input line number and harmony item number // JMI

  // append the harmony item
  fHarmonyStructureIntervals.push_back (
    harmonyInterval);
}

/* JMI
void msrHarmonyStructure::acceptIn (basevisitor* v) {
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmonyStructure::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrHarmonyStructure>*
    p =
      dynamic_cast<visitor<S_msrHarmonyStructure>*> (v)) {
        S_msrHarmonyStructure elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmonyStructure::visitStart ()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrHarmonyStructure::acceptOut (basevisitor* v) {
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmonyStructure::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrHarmonyStructure>*
    p =
      dynamic_cast<visitor<S_msrHarmonyStructure>*> (v)) {
        S_msrHarmonyStructure elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmonyStructure::visitEnd ()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrHarmonyStructure::browseData (basevisitor* v)
{}
*/

S_msrHarmonyInterval msrHarmonyStructure::bassHarmonyIntervalForHarmonyInversion (
  int inputLineNumber,
  int inversionNumber)
{
  /*
    Inversion is a number indicating which inversion is used:
    0 for root position, 1 for first inversion, etc.
  */

  S_msrHarmonyInterval result;

  if (
    inversionNumber < 0
      ||
    inversionNumber > int (fHarmonyStructureIntervals.size ()) - 1 ) {
    stringstream s;

    s <<
      "Sorry, inversion number '" <<
      inversionNumber <<
      "' does not exist for harmony intervals '" <<
      msrHarmonyKindAsString (fHarmonyStructureHarmonyKind) <<
      "', line " << inputLineNumber;

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return fHarmonyStructureIntervals [inversionNumber];
}

S_msrHarmonyStructure msrHarmonyStructure::invertHarmonyStructure (int inversion)
{
  if (inversion == 0) {
    return this;
  }

  // create an empty object
  S_msrHarmonyStructure
    result =
      this->
        createHarmonyStructureNewbornClone ();

  unsigned int
    harmonyStructureIntervalsSize =
      fHarmonyStructureIntervals.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceHarmoniesDetails ()) {
    gLogStream <<
      "==> invertHarmonyStructure (), inversion = " << inversion <<
      ", original harmonyStructureIntervalsSize = " << harmonyStructureIntervalsSize <<
      endl;
  }
#endif

  if (harmonyStructureIntervalsSize) {
    // add the first items
    for (unsigned int i = inversion; i < harmonyStructureIntervalsSize; i++) {
      S_msrHarmonyInterval
        harmonyIntervalClone =
          fHarmonyStructureIntervals [i]->
            createHarmonyIntervalNewbornClone ();

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceHarmoniesDetails ()) {
        gLogStream <<
          "--> adding first item to result:" <<
          endl;
        gIndenter++;
        gLogStream <<
          harmonyIntervalClone <<
          endl;
        gIndenter--;
      }
#endif

      result->
        appendHarmonyIntervalToHarmonyStructure (
          harmonyIntervalClone);

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceHarmoniesDetails ()) {
        gLogStream <<
          "==> result harmony structure after adding first item :" <<
          endl;

        gIndenter++;
        gLogStream <<
          result <<
          endl;
        gIndenter--;
      }
#endif
    } // for

    // add  the octaviate last items
    for (int i = 0; i < inversion; i++) {
      S_msrHarmonyInterval
        harmonyIntervalClone =
          fHarmonyStructureIntervals [i]->
            createHarmonyIntervalNewbornClone ();

      harmonyIntervalClone->
        incrementHarmonyIntervalRelativeOctave ();

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceHarmoniesDetails ()) {
        gLogStream <<
          "--> adding last item to resultlast item :" <<
          endl;
        gIndenter++;
        gLogStream <<
          harmonyIntervalClone <<
          endl;
        gIndenter--;
      }
#endif

      result->
        appendHarmonyIntervalToHarmonyStructure (
          harmonyIntervalClone);

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceHarmoniesDetails ()) {
        gLogStream <<
          "==> result harmony structure after  after adding last item:" <<
          endl;

        gIndenter++;
        gLogStream <<
          result <<
          endl;
        gIndenter--;
      }
#endif
    } // for
  }

  return result;
}

list<msrSemiTonesPitchKind> buildSemiTonesChord (
  msrHarmonyKind        harmonyKind,
  msrSemiTonesPitchKind rootNote)
{
  list<msrSemiTonesPitchKind> result;

  // create the harmony intervals
  S_msrHarmonyStructure
    harmonyStructure =
      msrHarmonyStructure::create (
        harmonyKind);

  // add the root to the harmony
  result.push_back (rootNote);

  // add the other notes to the harmony
  const vector<S_msrHarmonyInterval>&
    harmonyStructureIntervals =
      harmonyStructure->
        getHarmonyStructureIntervals ();

  for (unsigned int i = 1; i << harmonyStructureIntervals.size (); i++) {
    result.push_back (rootNote);
  } // for

  return result;
}

string msrHarmonyStructure::harmonyStructureAsString () const
{
  stringstream s;

  s <<
    "HarmonyStructure" <<
    ", " <<
    msrHarmonyKindAsString (fHarmonyStructureHarmonyKind) <<
    ", " <<
    singularOrPlural (
      fHarmonyStructureIntervals.size (), "item", "items");

  return s.str ();
}

void msrHarmonyStructure::print (ostream& os) const
{
  os <<
    "HarmonyStructure" <<
    ", harmonyStructureHarmonyKind: " <<
    msrHarmonyKindAsString (fHarmonyStructureHarmonyKind) <<
    ", " <<
    singularOrPlural (
      fHarmonyStructureIntervals.size (), "interval", "intervals") <<
  /* JMI
    ", line: " << fInputLineNumber <<
    */
    endl;

  gIndenter++;

  if (fHarmonyStructureIntervals.size ()) {
    vector<S_msrHarmonyInterval>::const_reverse_iterator
      iBegin = fHarmonyStructureIntervals.crbegin (),
      iEnd   = fHarmonyStructureIntervals.crend (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrHarmonyInterval
        harmonyInterval = (*i);

      gLogStream <<
        harmonyInterval->harmonyIntervalAsShortString () <<
        endl;

      if (++i == iEnd) break;
    } // for
  }
  else {
    gLogStream <<
      "no intervals" <<
      endl;
  }

  gIndenter--;
}

void msrHarmonyStructure::printAllHarmoniesStructures (ostream& os)
{
  os <<
    "All the known harmonies structures are:" <<
    endl << endl;

  gIndenter++;

  for (
    msrHarmonyKind harmonyKind = kMajorHarmony;
    harmonyKind <= kMajorSeventhAugmentedEleventhHarmony;
    harmonyKind = msrHarmonyKind (harmonyKind + 1)) {
    // create the harmony intervals
    S_msrHarmonyStructure
      harmonyStructure =
        msrHarmonyStructure::create (
          harmonyKind);

    // print it
    os <<
      harmonyStructure <<
      endl;
  } // for

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrHarmonyStructure& elt)
{
  elt->print (os);
  return os;
}

/* JMI
//______________________________________________________________________________
S_msrSemiTonesPitchAndAbsoluteOctave msrSemiTonesPitchAndAbsoluteOctave::create (
  msrSemiTonesPitchKind semiTonesPitchKind,
  int                   absoluteOctave)
{
  msrSemiTonesPitchAndAbsoluteOctave* o =
    new msrSemiTonesPitchAndAbsoluteOctave (
      semiTonesPitchKind,
      absoluteOctave);
  assert (o!=0);

  return o;
}

msrSemiTonesPitchAndAbsoluteOctave::msrSemiTonesPitchAndAbsoluteOctave (
  msrSemiTonesPitchKind semiTonesPitchKind,
  int                   absoluteOctave)
{
  fSemiTonesPitchKind = semiTonesPitchKind;

  fAbsoluteOctave = absoluteOctave;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceHarmoniesDetails ()) {
    gLogStream <<
      "==> Creating harmony item '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif
}

msrSemiTonesPitchAndAbsoluteOctave::~msrSemiTonesPitchAndAbsoluteOctave ()
{}

S_msrSemiTonesPitchAndAbsoluteOctave msrSemiTonesPitchAndAbsoluteOctave::createSemiTonesPitchAndAbsoluteOctaveNewbornClone ()
{
  S_msrSemiTonesPitchAndAbsoluteOctave
    newbornClone =
      msrSemiTonesPitchAndAbsoluteOctave::create (
        fSemiTonesPitchKind,
        fAbsoluteOctave);

  return newbornClone;
}

string msrSemiTonesPitchAndAbsoluteOctave::asString () const
{
  stringstream s;

  const int fieldWidth = 19;

  s << left <<
    "SemiTonesPitchAndAbsoluteOctave" <<
    ": " <<
    setw (fieldWidth) <<
    msrSemiTonesPitchKindAsString (fSemiTonesPitchKind) <<
    ", absoluteOctave: " << fAbsoluteOctave;

  return s.str ();
}

void msrSemiTonesPitchAndAbsoluteOctave::print (ostream& os) const
{
  os <<
    "SemiTonesPitchAndAbsoluteOctave" <<
    endl;

  gIndenter++;

  const int fieldWidth = 22;

  os << left <<
    setw (fieldWidth) <<
    "semiTonesPitchKind" << " : " <<
      msrSemiTonesPitchKindAsString (fSemiTonesPitchKind) <<
    endl <<
    setw (fieldWidth) <<
    "absoluteOctave" << " : " << fAbsoluteOctave <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrSemiTonesPitchAndAbsoluteOctave& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrSemiTonesPitchAndRelativeOctave msrSemiTonesPitchAndRelativeOctave::create (
  msrSemiTonesPitchKind semiTonesPitchKind,
  int                   relativeOctave)
{
  msrSemiTonesPitchAndRelativeOctave* o =
    new msrSemiTonesPitchAndRelativeOctave (
      semiTonesPitchKind,
      relativeOctave);
  assert (o!=0);

  return o;
}

msrSemiTonesPitchAndRelativeOctave::msrSemiTonesPitchAndRelativeOctave (
  msrSemiTonesPitchKind semiTonesPitchKind,
  int                   relativeOctave)
{
  fSemiTonesPitchKind = semiTonesPitchKind;

  fRelativeOctave = relativeOctave;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceHarmoniesDetails ()) {
    gLogStream <<
      "==> Creating harmony item '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif
}

msrSemiTonesPitchAndRelativeOctave::~msrSemiTonesPitchAndRelativeOctave ()
{}

S_msrSemiTonesPitchAndRelativeOctave msrSemiTonesPitchAndRelativeOctave::createSemiTonesPitchAndRelativeOctaveNewbornClone ()
{
  S_msrSemiTonesPitchAndRelativeOctave
    newbornClone =
      msrSemiTonesPitchAndRelativeOctave::create (
        fSemiTonesPitchKind,
        fRelativeOctave);

  return newbornClone;
}

string msrSemiTonesPitchAndRelativeOctave::asString () const
{
  stringstream s;

  const int fieldWidth = 19;

  s << left <<
    "SemiTonesPitchAndRelativeOctave" <<
    ": " <<
    setw (fieldWidth) <<
    msrSemiTonesPitchKindAsString (fSemiTonesPitchKind) <<
    ", relativeOctave: " << fRelativeOctave;

  return s.str ();
}

void msrSemiTonesPitchAndRelativeOctave::print (ostream& os) const
{
  os <<
    "SemiTonesPitchAndRelativeOctave" <<
    endl;

  gIndenter++;

  const int fieldWidth = 22;

  os << left <<
    setw (fieldWidth) <<
    "semiTonesPitchKind" << " : " <<
      msrSemiTonesPitchKindAsString (fSemiTonesPitchKind) <<
    endl <<
    setw (fieldWidth) <<
    "relativeOctave" << " : " << fRelativeOctave <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrSemiTonesPitchAndRelativeOctave& elt)
{
  elt->print (os);
  return os;
}
*/

//______________________________________________________________________________
S_msrHarmonyContents msrHarmonyContents::create (
// JMI  int                   inputLineNumber,
  msrSemiTonesPitchKind harmonyContentsRootNote,
  msrHarmonyKind        harmonyContentsHarmonyKind)
{
  msrHarmonyContents* o =
    new msrHarmonyContents (
 //     inputLineNumber,
      harmonyContentsRootNote,
      harmonyContentsHarmonyKind);
  assert (o!=0);

  return o;
}

msrHarmonyContents::msrHarmonyContents (
// JMI  int                   inputLineNumber,
  msrSemiTonesPitchKind harmonyContentsRootNote,
  msrHarmonyKind        harmonyContentsHarmonyKind)
{
  fHarmonyContentsRootNote    = harmonyContentsRootNote;
  fHarmonyContentsHarmonyKind = harmonyContentsHarmonyKind;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceHarmoniesDetails ()) {
    gLogStream <<
      "==> Creating harmonyContents '" <<
      harmonyContentsAsString () <<
      "'" <<
      endl;
  }
#endif

  // create the root harmony element
  S_msrSemiTonesPitchAndOctave
    rootChordElement =
      msrSemiTonesPitchAndOctave::create (
        fHarmonyContentsRootNote,
        0); // relative octave

  // add it to the harmony elements
  fHarmonyElementsVector.push_back (rootChordElement);

  // add the other notes to the harmony elements
  S_msrHarmonyStructure
    harmonyStructure =
      msrHarmonyStructure::create (
        fHarmonyContentsHarmonyKind);

  const vector<S_msrHarmonyInterval>&
    harmonyIntervals =
      harmonyStructure->
        getHarmonyStructureIntervals ();

  for (unsigned int i = 1; i < harmonyIntervals.size (); i++) {
    // get the interval
    msrIntervalKind
      intervalKind =
        harmonyIntervals [i]->
          getHarmonyIntervalIntervalKind ();

    // fetch the semitone pitch
    msrSemiTonesPitchKind
      semiTonePitch =
        noteAtIntervalFromSemiTonesPitch (
          0, // ??? JM                   inputLineNumber,
          intervalKind,
          fHarmonyContentsRootNote);

    // create the harmony element
    S_msrSemiTonesPitchAndOctave
      harmonyElement =
        msrSemiTonesPitchAndOctave::create (
          semiTonePitch,
          0); // relative octave

    // add it to the harmony elements
    fHarmonyElementsVector.push_back (harmonyElement);
  } // for
}

msrHarmonyContents::~msrHarmonyContents ()
{}

string msrHarmonyContents::harmonyContentsAsString () const
{
  stringstream s;

  s <<
    "HarmonyContents" <<
    ", " <<
    msrHarmonyKindAsString (fHarmonyContentsHarmonyKind) <<
    ", " <<
    singularOrPlural (
      fHarmonyElementsVector.size (), "harmony element", "harmony elements");

  return s.str ();
}

msrSemiTonesPitchKind msrHarmonyContents::bassSemiTonesPitchKindForHarmonyInversion (
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
    inversionNumber > int (fHarmonyElementsVector.size ()) - 1 ) {
    stringstream s;

    s <<
      "Sorry, inversion number '" <<
      inversionNumber <<
      "' does not exist for harmony notes '" <<
      msrHarmonyKindAsString (fHarmonyContentsHarmonyKind) <<
      "', line " << inputLineNumber;

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return
    fHarmonyElementsVector [inversionNumber]->
      getSemiTonesPitchKind ();
}

void msrHarmonyContents::printAllHarmoniesContents (
  ostream&              os,
  msrSemiTonesPitchKind rootSemiTonesPitchKind)
{
  // fetch the root quartertones pitch kind
  msrQuarterTonesPitchKind
    rootQuarterTonesPitchKind =
      quarterTonesPitchKindFromSemiTonesPitchKind (
        rootSemiTonesPitchKind);

  os <<
    "All the known harmonies contents with diatonic root '" <<
    msrQuarterTonesPitchKindAsString (
      gGlobalLpsrOahGroup->
        getLpsrQuarterTonesPitchesLanguageKind (),
      rootQuarterTonesPitchKind) <<
      /* JMI
    "' (" <<
    msrSemiTonesPitchKindAsString (
      rootSemiTonesPitchKind) <<
    ")" <<
    */
    "' in language '" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      gGlobalLpsrOahGroup->
        getLpsrQuarterTonesPitchesLanguageKind ()) <<
    "' 'are:" <<
    endl << endl;

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

    // create the harmony intervals
    S_msrHarmonyStructure
      harmonyStructure =
        msrHarmonyStructure::create (
          harmonyKind);

    // fetch the intervals items for these intervals
    // with rootSemiTonesPitchKind as root
    const vector <S_msrHarmonyInterval>&
      harmonyStructureIntervals =
        harmonyStructure->
          getHarmonyStructureIntervals ();

    if (harmonyStructureIntervals.size ()) {
      // fetch the notes for these intervals
      vector<S_msrHarmonyInterval>::const_reverse_iterator
        iBegin = harmonyStructureIntervals.crbegin (),
        iEnd   = harmonyStructureIntervals.crend (),
        i      = iBegin;

      for ( ; ; ) {
        S_msrHarmonyInterval
          harmonyInterval = (*i);

        msrIntervalKind
          intervalKind =
            harmonyInterval->
              getHarmonyIntervalIntervalKind ();

        // fetch the semitones pitch kind
        msrSemiTonesPitchKind
          noteSemiTonesPitchKind =
            noteAtIntervalFromSemiTonesPitch (
              K_NO_INPUT_LINE_NUMBER,
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
            gGlobalLpsrOahGroup->
              getLpsrQuarterTonesPitchesLanguageKind (),
            noteQuarterTonesPitchKind) <<
          " : " <<
          msrIntervalKindAsString (intervalKind) <<
          endl;

        if (++i == iEnd) break;

        // no endl here
      } // for
    }

  os << endl;

  gIndenter--;
  } // for

  gIndenter--;
}

/* JMI
void msrHarmonyContents::acceptIn (basevisitor* v) {
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmonyContents::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrHarmonyContents>*
    p =
      dynamic_cast<visitor<S_msrHarmonyContents>*> (v)) {
        S_msrHarmonyContents elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmonyContents::visitStart ()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrHarmonyContents::acceptOut (basevisitor* v) {
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmonyContents::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrHarmonyContents>*
    p =
      dynamic_cast<visitor<S_msrHarmonyContents>*> (v)) {
        S_msrHarmonyContents elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmonyContents::visitEnd ()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrHarmonyContents::browseData (basevisitor* v)
{}
*/

void msrHarmonyContents::print (ostream& os) const
{
  os <<
    "HarmonyContents" <<
  /* JMI
    ", line: " << fInputLineNumber <<
    */
    endl;

  gIndenter++;

  const int fieldWidth = 17;

  os << left <<
    setw (fieldWidth) <<
    "harmonyContentsRootNote" << " : " <<
    msrSemiTonesPitchKindAsString (fHarmonyContentsRootNote) <<
    endl <<
    setw (fieldWidth) <<
    "harmonyContentsHarmonyKind" << " : " <<
    msrHarmonyKindAsString (fHarmonyContentsHarmonyKind) <<
    endl;

  if (fHarmonyElementsVector.size ()) {
    os <<
    singularOrPlural (
      fHarmonyElementsVector.size (), "harmony element", "harmony elements") <<
    ":" <<
    endl;

    gIndenter++;

    for (unsigned int i = 0; i < fHarmonyElementsVector.size (); i++) {
      S_msrSemiTonesPitchAndOctave
        harmonyElement =
          fHarmonyElementsVector [i];

      os <<
        harmonyElement <<
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

ostream& operator<< (ostream& os, const S_msrHarmonyContents& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void printHarmonyDetails (
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
        gGlobalLpsrOahGroup->
          getLpsrQuarterTonesPitchesLanguageKind (),
        rootQuarterTonesPitchKind);

  string
    harmonyKindShortName =
      msrHarmonyKindShortName (
        harmonyKind);

  // print the deails
  os <<
    "The details of harmony '" <<
    rootQuarterTonesPitchKindAsString <<
    " " <<
    harmonyKindShortName <<
    "' are:" <<
    endl << endl;

  gIndenter++;

  // create the harmony intervals
  S_msrHarmonyStructure
    harmonyStructure =
      msrHarmonyStructure::create (
        harmonyKind);

  // fetch the intervals items for these intervals
  // with rootSemiTonesPitchKind as root
  const vector <S_msrHarmonyInterval>&
    harmonyStructureIntervals =
      harmonyStructure->
        getHarmonyStructureIntervals ();

  // loop on all the inversion
  int harmonyStructureIntervalsNumber =
    harmonyStructureIntervals.size ();

  if (harmonyStructureIntervalsNumber) {
    for (int inversion = 0; inversion < harmonyStructureIntervalsNumber; inversion++) {
      // invert the harmony structure
      S_msrHarmonyStructure
        invertedHarmonyStructure =
          harmonyStructure->
            invertHarmonyStructure (inversion);

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceHarmoniesDetails ()) {
        os <<
          "==> inversion = " << inversion <<
          ", initial invertedHarmonyStructure:" <<
          endl;

        gIndenter++;
        os <<
          invertedHarmonyStructure <<
          endl;
        gIndenter--;
      }
#endif

      // get the inverted harmony structure intervals
      const vector <S_msrHarmonyInterval>&
        invertedHarmonyStructureIntervals =
          invertedHarmonyStructure->
            getHarmonyStructureIntervals ();

      // fetch the notes for these intervals
      /* JMI
      vector<S_msrHarmonyInterval>::const_iterator
        iBegin = invertedHarmonyStructureIntervals.begin (),
        iEnd   = invertedHarmonyStructureIntervals.end (),
        i      = iBegin;
        */
      vector<S_msrHarmonyInterval>::const_reverse_iterator
        iBegin = invertedHarmonyStructureIntervals.crbegin (),
        iEnd   = invertedHarmonyStructureIntervals.crend (),
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
          " inversion " << inversion;
      }

      os <<
        " contents, " <<
        invertedHarmonyStructureIntervals.size () <<
        " intervals:" <<
        endl;

      gIndenter++;

      for ( ; ; ) {
        S_msrHarmonyInterval
          harmonyInterval = (*i);

        msrIntervalKind
          intervalKind =
            harmonyInterval->
              getHarmonyIntervalIntervalKind ();

        const int fieldWidth1 = 17;

        os << left <<
          setw (fieldWidth1) <<
          msrIntervalKindAsString (intervalKind) <<
          ": ";

        // fetch the semitones pitch kind
        msrSemiTonesPitchKind
          noteSemiTonesPitchKind =
            noteAtIntervalFromSemiTonesPitch (
              K_NO_INPUT_LINE_NUMBER,
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
            gGlobalLpsrOahGroup->
              getLpsrQuarterTonesPitchesLanguageKind (),
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

      os << endl;
    } // for
  }

  gIndenter--;
}

//______________________________________________________________________________
void printHarmonyAnalysis (
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
        gGlobalLpsrOahGroup->
          getLpsrQuarterTonesPitchesLanguageKind (),
        rootQuarterTonesPitchKind);

  string
    harmonyKindShortName =
      msrHarmonyKindShortName (
        harmonyKind);

  // print the deails
  os <<
    "The analysis of harmony '" <<
    rootQuarterTonesPitchKindAsString <<
    " " <<
    harmonyKindShortName <<
    "' inversion " <<
    inversion <<
    " is:" <<
    endl << endl;

  gIndenter++;

  // create the harmony intervals
  S_msrHarmonyStructure
    harmonyStructure =
      msrHarmonyStructure::create (
        harmonyKind);

  // fetch the intervals items for these intervals
  // with rootSemiTonesPitchKind as root
  const vector <S_msrHarmonyInterval>&
    harmonyStructureIntervals =
      harmonyStructure->
        getHarmonyStructureIntervals ();

  // loop on all the inversion
  int harmonyStructureIntervalsNumber =
    harmonyStructureIntervals.size ();

  if (harmonyStructureIntervalsNumber) {
    if (inversion < harmonyStructureIntervalsNumber) {
      // invert the harmony structure
      S_msrHarmonyStructure
        invertedHarmonyStructure =
          harmonyStructure->
            invertHarmonyStructure (inversion);

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceHarmoniesDetails ()) {
        os <<
          "==> inversion = " << inversion <<
          ", initial invertedHarmonyStructure:" <<
          endl;

        gIndenter++;
        os <<
          invertedHarmonyStructure <<
          endl;
        gIndenter--;
      }
#endif

      // get the inverted harmony structure intervals
      const vector <S_msrHarmonyInterval>&
        invertedHarmonyStructureIntervals =
          invertedHarmonyStructure->
            getHarmonyStructureIntervals ();

      // print the harmony contents
      {
        os <<
          "Harmony '" <<
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
            " inversion " << inversion;
        }

        os <<
          " contents, " <<
          invertedHarmonyStructureIntervals.size () <<
          " intervals:" <<
          endl;

        gIndenter++;

        vector<S_msrHarmonyInterval>::const_reverse_iterator
          iBegin = invertedHarmonyStructureIntervals.crbegin (),
          iEnd   = invertedHarmonyStructureIntervals.crend (),
          i      = iBegin;

        for ( ; ; ) {
          S_msrHarmonyInterval
            harmonyInterval = (*i);

          msrIntervalKind
            intervalKind =
              harmonyInterval->
                getHarmonyIntervalIntervalKind ();

          // fetch the semitones pitch kind
          msrSemiTonesPitchKind
            noteSemiTonesPitchKind =
              noteAtIntervalFromSemiTonesPitch (
                K_NO_INPUT_LINE_NUMBER,
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
              gGlobalLpsrOahGroup->
                getLpsrQuarterTonesPitchesLanguageKind (),
              noteQuarterTonesPitchKind) <<
            " : " <<
            msrIntervalKindAsString (intervalKind) <<
            endl;

          if (++i == iEnd) break;

          // no endl here
        } // for

        gIndenter--;

        os << endl;
      }

      // print the harmony's inner intervals
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
            " inversion " << inversion;
        }

        os <<
          " inner intervals:" <<
          endl;

        gIndenter++;

        int tritonsCounter = 0;

        vector<S_msrHarmonyInterval>::const_iterator
          iBegin1 = invertedHarmonyStructureIntervals.begin (),
          iEnd1   = invertedHarmonyStructureIntervals.end () - 1,
          i1      = iBegin1;

        for ( ; ; ) {
          S_msrHarmonyInterval
            harmonyInterval1 = (*i1);

          msrIntervalKind
            intervalKind1 =
              harmonyInterval1->
                getHarmonyIntervalIntervalKind ();

          // fetch the semitones pitch kind
          msrSemiTonesPitchKind
            noteSemiTonesPitchKind1 =
              noteAtIntervalFromSemiTonesPitch (
                K_NO_INPUT_LINE_NUMBER,
                intervalKind1,
                rootSemiTonesPitchKind);

          // fetch the quartertones pitch kind
          msrQuarterTonesPitchKind
            noteQuarterTonesPitchKind1 =
              quarterTonesPitchKindFromSemiTonesPitchKind (
                noteSemiTonesPitchKind1);

          // print the invervals
          vector<S_msrHarmonyInterval>::const_iterator
            iBegin2 = i1 + 1,
            iEnd2   = invertedHarmonyStructureIntervals.end (),
            i2      = iBegin2;

          for ( ; ; ) {
            S_msrHarmonyInterval
              harmonyInterval2 = (*i2);

            msrIntervalKind
              intervalKind2 =
                harmonyInterval2->
                  getHarmonyIntervalIntervalKind ();

            const int fieldWidth1 = 5;

            // fetch the semitones pitch kind
            msrSemiTonesPitchKind
              noteSemiTonesPitchKind2 =
                noteAtIntervalFromSemiTonesPitch (
                  K_NO_INPUT_LINE_NUMBER,
                  intervalKind2,
                  rootSemiTonesPitchKind);

            // fetch the quartertones pitch kind
            msrQuarterTonesPitchKind
              noteQuarterTonesPitchKind2 =
                quarterTonesPitchKindFromSemiTonesPitchKind (
                  noteSemiTonesPitchKind2);

            // compute the inner interval
            S_msrHarmonyInterval
              interInterval =
                harmonyInterval2->
                  intervalDifference (
                    harmonyInterval1);

            msrIntervalKind
              innerIntervalKind =
                interInterval->
                  getHarmonyIntervalIntervalKind ();

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
                gGlobalLpsrOahGroup->
                  getLpsrQuarterTonesPitchesLanguageKind (),
                noteQuarterTonesPitchKind1) <<

              " -> " <<

              setw (fieldWidth1) <<
              msrQuarterTonesPitchKindAsString (
                gGlobalLpsrOahGroup->
                  getLpsrQuarterTonesPitchesLanguageKind (),
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

          os << endl;
        } // for

        gIndenter--;


        if (tritonsCounter > 0) {
          os <<
            "This harmony contains " <<
            singularOrPlural (
              tritonsCounter, "triton", "tritons") <<
            endl;
        }

        os << endl;
      }
    }

    else {
      os <<
        "Chord '" <<
        rootQuarterTonesPitchKindAsString <<
        " " <<
        harmonyKindShortName <<
        "' has only " <<
        harmonyStructureIntervalsNumber <<
        " intervals, inversion " <<
        inversion <<
        " does not exist" <<
        endl << endl;
    }
  }

  gIndenter--;
}

// RGB colors
//______________________________________________________________________________
msrRGBColor::msrRGBColor ()
{
  // initializing to negative values for isEmpty()
  fR = -1.0;
  fG = -1.0;
  fB = -1.0;
}

msrRGBColor::msrRGBColor (
  float theR,
  float theG,
  float theB)
{
  fR = theR;
  fG = theG;
  fB = theB;
}

msrRGBColor::msrRGBColor (
  std::string theString)
{
  string regularExpression (
    "([[:digit:]]*.[[:digit:]]*)"
    ","
    "([[:digit:]]*.[[:digit:]]*)"
    ","
    "([[:digit:]]*.[[:digit:]]*)");

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for RGB color string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;
  }
#endif

  if (smSize == 4) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      for (unsigned i = 0; i < smSize; ++i) {
        gLogStream <<
          "[" << sm [i] << "] ";
      } // for
      gLogStream << endl;
    }
#endif
  }

  else {
    stringstream s;

    s <<
      "msrRGBColor string '" << theString <<
      "' is ill-formed";

    oahError (s.str ());
  }

  string
    RString = sm [1],
    GString = sm [2],
    BString = sm [3];

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "--> RString = \"" << RString << "\", " <<
      "--> GString = \"" << GString << "\"" <<
      "--> BString = \"" << BString << "\"" <<
      endl;
  }
#endif

  // are these strings alright?
  {
    stringstream s;

    s << RString;
    s >> fR;

    if (fR < 0.0 || fR > 1.0) {
      gLogStream <<
        "### ERROR: the R component " << fR <<
        " is not in the [0.0..1.0] interval in RGB color '" << theString << "'" <<
        endl;
    }
  }
  {
    stringstream s;

    s << GString;
    s >> fG;

    if (fG < 0.0 || fG > 1.0) {
      gLogStream <<
        "### ERROR: the G component " << fG <<
        " is not in the [0.0..1.0] interval in RGB color '" << theString << "'" <<
        endl;
    }
  }
  {
    stringstream s;

    s << BString;
    s >> fB;

    if (fB < 0.0 || fB > 1.0) {
      gLogStream <<
        "### ERROR: the B component " << fB <<
        " is not in the [0.0..1.0] interval in RGB color '" << theString << "'" <<
        endl;
    }
  }

}

string msrRGBColor::asString (int precision) const
{
  stringstream s;

  s <<
    setprecision (precision) <<
    "'" <<
    fR <<
    "," <<
    fG <<
    "," <<
    fB <<
    "'";

  return s.str ();
}

string msrRGBColor::asSpaceSeparatedString (int precision) const
{
  stringstream s;

  s <<
    setprecision (precision) <<
    fR <<
    " " <<
    fG <<
    " " <<
    fB;

  return s.str ();
}

void msrRGBColor::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
};

ostream& operator<< (ostream& os, const msrRGBColor& elt)
{
  elt.print (os);
  return os;
}

// AlphaRGB colors
//______________________________________________________________________________
msrAlphaRGBColor::msrAlphaRGBColor (
  string colorRGB,
  string colorAlpha)
{
  fColorRGB   = colorRGB;
  fColorAlpha = colorAlpha;
}

msrAlphaRGBColor::msrAlphaRGBColor (
  string colorRGB)
{
  fColorRGB   = colorRGB;
  fColorAlpha = "FF";
}

msrAlphaRGBColor::~msrAlphaRGBColor ()
{}

string msrAlphaRGBColor::asString () const
{
  stringstream s;

  s << "color: " <<
    "colorRGB = \"" << fColorRGB <<
    "\", colorAlpha = \"" << fColorAlpha <<
    "\"";

  return s.str ();
}

void msrAlphaRGBColor::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
};

ostream& operator<< (ostream& os, const msrAlphaRGBColor& elt)
{
  elt.print (os);
  return os;
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
  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah () && ! gGlobalGeneralOahGroup->getQuiet ()) {
      gLogStream <<
        "Initializing MSR basic types handling" <<
        endl;
  }
#endif

    // languages handling
    // ------------------------------------------------------

    initializeQuarterTonesPitchesLanguageKinds ();

    // clefs handling
    // ------------------------------------------------------

    initializeClefKinds ();

    // harmonies handling
    // ------------------------------------------------------

    initializeHarmonyKinds ();

    // harmony structures handling
    // ------------------------------------------------------

    initializeHarmonyStructuresMap ();

    // MSR lengths handling
    // ------------------------------------------------------

    initializeMsrLengthUnitKindsMap ();

    // MSR margins types handling
    // ------------------------------------------------------

    initializeMsrMarginTypeKindsMap ();

    pThisMethodHasBeenRun = true;
  }
}


}

/* JMI
#include <algorithm>    // for_each

    // Iterate over a map using std::for_each and Lambda function
    for_each (
      gGlobalQuarterTonesPitchesLanguageKindsMap.begin(),
      gGlobalQuarterTonesPitchesLanguageKindsMap.end(),
        [] (pair<string, gGlobalQuarterTonesPitchesLanguageKindsMap> element) {
          string theString = (element).first;

          count++;

          cumulatedLength += theString.size ();
          if (cumulatedLength >= namesListMaxLength) {
            s << "\n";
            cumulatedLength = 0;
          }

          s << theString;

          if (count == nextToLast) {
            s << " and ";
          }
          else {
            s << ", ";
          }
        }
    ); // foreach
*/

