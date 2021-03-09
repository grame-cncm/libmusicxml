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

#include <climits>      // INT_MIN, INT_MAX

#include "utilities.h"

#include "msrBasicTypes.h"
#include "lpsrBasicTypes.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "lpsrOah.h"

#include "waeMessagesHandling.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
int lpsrDurationBinaryLogarithm (int duration)
{
  int result = INT_MIN;

/*
with LilyPond's limitation to 128th of a whole note,
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
    case 256: // JMI
      result = 8;
      break;

    default:
      ;
  } // switch

  return result;
}

//_______________________________________________________________________________
int lpsrNumberOfDots (int n)
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

    default:
      ;
    } // switch

  return result;
}

//_______________________________________________________________________________
string wholeNotesAsLilypondString (
  int      inputLineNumber,
  rational wholeNotes,
  int&     dotsNumber)
{
  // this algorithm is inspired by musicxml2ly

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceWholeNotes ()) {
    gLogStream <<
      "--> wholeNotesAsLilypondString() 1 -------------------------------------" <<
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

/* JMI
  if (false && numerator <= 0) { // JMI
    stringstream s;

    s <<
     "%{ZERO_LENGTH: " <<
     wholeNotes << // JMI
     ", line " <<
     inputLineNumber <<
     " %}";

    return s.str ();
  }
*/

  // sanity check
  if (numerator <= 0) {
    stringstream s;

    s <<
      "numerator is not positive in wholeNotesAsLilypondString()" <<
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
    augmentation dots add half the preceding increment to the duration:
    they constitue a series of frations or the form '(2^n-1) / 2^n',
    starting with 3/2, 7/4, 15/8,
    that tends towards 2 while always remaining less than two.

    with MusicXML's limitation to 1024th of a whole note,
    with LilyPond's limitation to 128th of a whole note,
    valid numerators are:
  */

  int  numeratorDots = lpsrNumberOfDots (numerator);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
      "--> numeratorDots " << " : " << numeratorDots <<
      endl << endl;
  }
#endif

/*
    default:
      if (! integralNumberOfWholeNotes) {
  //      multiplyingFactorIsToBeUsed = true;
      }
      else {
        // there can be integral numbers of whole notes up to 15,
        // corresponding to a \maxima...
          stringstream s;

          s <<
            "numerator " << numerator <<
            " is not one of 1, 3, 7, 15, 31, 63 or 127" <<
       //     " is not one of 1, 3, 7, 15, 31, 63, 127, 255, 511 or 1023" <<
            ", whole notes duration " <<
            numerator << "/" << denominator;

          if (rationalHasBeenSimplified) {
            s <<
              " (" << numerator << "/" << denominator << ")" <<
            endl;
          }

          s <<
            " it will be represented using a multiplying factor";

          msrMusicXMLWarning (
            gGlobalOahOahGroup->getInputSourceName (),
            inputLineNumber,
            s.str ());

  //      multiplyingFactorIsToBeUsed = true;
      }
  } // switch
    */

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
    lpsrDurationBinaryLogarithm (denominator);

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
    // 6/1 thus becomes 3\breve, hence \longa.
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
    numeratorDots = lpsrNumberOfDots (numerator);
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
      --denominatorDurationLog;

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
    for (int i = 0; i < numeratorDots; ++i) {
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
      "--> wholeNotesAsLilypondString() 2 -------------------------------------" <<
     ", result: \"" << result << "\"" <<
      ", numeratorDots" << " : " << numeratorDots <<
      endl;
  }
#endif

  // return the result
  dotsNumber = numeratorDots;

  return result;
}

string wholeNotesAsLilypondString (
  int      inputLineNumber,
  rational wholeNotes)
{
  int dotsNumber; // not used

  return
    wholeNotesAsLilypondString (
      inputLineNumber,
      wholeNotes,
      dotsNumber);
}

//_______________________________________________________________________________
string dottedDurationAsLilypondString (
  int               inputLineNumber,
  msrDottedDuration dottedDuration)
{
  return
    wholeNotesAsLilypondString (
      inputLineNumber,
      dottedDuration.dottedDurationAsWholeNotes (
        inputLineNumber));
}

string dottedDurationAsLilypondStringWithoutBackSlash (
  int               inputLineNumber,
  msrDottedDuration dottedDuration)
{
  string result =
    wholeNotesAsLilypondString (
      inputLineNumber,
      dottedDuration.dottedDurationAsWholeNotes (
        inputLineNumber));

  if (result [0] == '\\') {
    result = result.substr (1);
  }

  return result;
}

//_______________________________________________________________________________
string restMeasuresWholeNoteAsLilypondString (
  int      inputLineNumber,
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

//_______________________________________________________________________________
void writeTextsListAsLilypondString (
  const list<string>& textsList,
  ostream&            os)
{
  string contents;

  if (textsList.size ()) {
    list<string>::const_iterator
      iBegin = textsList.begin (),
      iEnd   = textsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      contents += (*i);
      if (++i == iEnd) break;
  // JMI    os << ", ";
    } // for
  }

  size_t found = contents.find("\"");
  if (found != std::string::npos) {
    os << doubleQuoteString (contents);
  }
  else {
    os << doubleQuoteStringIfNonAlpha (contents); // JMI
  }
}

// score output kinds
//______________________________________________________________________________

map<string, lpsrScoreOutputKind>
  gGlobalLpsrScoreOutputKindsMap;

string lpsrScoreOutputKindAsString (
  lpsrScoreOutputKind scoreOutputKind)
{
  string result;

  // no CamelCase here, these strings are used in the command line options

  switch (scoreOutputKind) {
    case kScoreOnly: // default value
      result = "score-only";
      break;
    case kScoreAndParts:
      result = "score-and-parts";
      break;
    case kPartsAndScore:
      result = "parts-and-score";
      break;
    case kPartsOnly:
      result = "parts-only";
      break;
    case kScoreAndPartsOneFile:
      result = "score-and-parts-one-file";
      break;
    case kPartsAndScoreOneFile:
      result = "parts-and-score-one-file";
      break;
    case kPartsOnlyOneFile:
      result = "parts-only-one-file";
      break;
  } // switch

  return result;
}

void initializeLpsrScoreOutputKindsMap ()
{
  // register the LilyPond score output kinds
  // --------------------------------------

  // no CamelCase here, these strings are used in the command line options

  gGlobalLpsrScoreOutputKindsMap ["score-only"] = kScoreOnly;
  gGlobalLpsrScoreOutputKindsMap ["score-and-parts"] = kScoreAndParts;
  gGlobalLpsrScoreOutputKindsMap ["parts-and-score"] = kPartsAndScore;
  gGlobalLpsrScoreOutputKindsMap ["parts-only"] = kPartsOnly;
  gGlobalLpsrScoreOutputKindsMap ["score-and-parts-one-file"] = kScoreAndPartsOneFile;
  gGlobalLpsrScoreOutputKindsMap ["parts-and-score-one-file"] = kPartsAndScoreOneFile;
  gGlobalLpsrScoreOutputKindsMap ["parts-only-one-file"] = kPartsOnlyOneFile;
}

string existingLpsrScoreOutputKinds (unsigned int namesListMaxLength)
{
  stringstream s;

  unsigned int
    lpsrScoreOutputKindsMapSize =
      gGlobalLpsrScoreOutputKindsMap.size ();

  if (lpsrScoreOutputKindsMapSize) {
    unsigned int
      nextToLast =
        lpsrScoreOutputKindsMapSize - 1;

    unsigned int count = 0;
    unsigned int cumulatedLength = 0;

    for (
      map<string, lpsrScoreOutputKind>::const_iterator i =
        gGlobalLpsrScoreOutputKindsMap.begin ();
      i != gGlobalLpsrScoreOutputKindsMap.end ();
      ++i
    ) {
      string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
      }

      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != lpsrScoreOutputKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

// octave entry kinds
//______________________________________________________________________________

map<string, lpsrOctaveEntryKind>
  gGlobalLpsrOctaveEntryKindsMap;

string lpsrOctaveEntryKindAsString (
  lpsrOctaveEntryKind octaveEntryKind)
{
  string result;

  // no CamelCase here, these strings are used in the command line options

  switch (octaveEntryKind) {
    case kOctaveEntryRelative: // default value
      result = "relative";
      break;
    case kOctaveEntryAbsolute:
      result = "absolute";
      break;
    case kOctaveEntryFixed:
      result = "fixed";
      break;
  } // switch

  return result;
}

void initializeLpsrOctaveEntryKindsMap ()
{
  // register the LilyPond score output kinds
  // --------------------------------------

  // no CamelCase here, these strings are used in the command line options

  gGlobalLpsrOctaveEntryKindsMap ["relative"] = kOctaveEntryRelative;
  gGlobalLpsrOctaveEntryKindsMap ["absolute"] = kOctaveEntryAbsolute;
  gGlobalLpsrOctaveEntryKindsMap ["fixed"] = kOctaveEntryFixed;
}

string existingLpsrOctaveEntryKinds (unsigned int namesListMaxLength)
{
  stringstream s;

  unsigned int
    lpsrOctaveEntryKindsMapSize =
      gGlobalLpsrOctaveEntryKindsMap.size ();

  if (lpsrOctaveEntryKindsMapSize) {
    unsigned int
      nextToLast =
        lpsrOctaveEntryKindsMapSize - 1;

    unsigned int count = 0;
    unsigned int cumulatedLength = 0;

    for (
      map<string, lpsrOctaveEntryKind>::const_iterator i =
        gGlobalLpsrOctaveEntryKindsMap.begin ();
      i != gGlobalLpsrOctaveEntryKindsMap.end ();
      ++i
    ) {
      string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
      }

      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != lpsrOctaveEntryKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

// accidental styles
//______________________________________________________________________________

map<string, lpsrAccidentalStyleKind>
  gGlobalLpsrAccidentalStyleKindsMap;

string lpsrAccidentalStyleKindAsString (
  lpsrAccidentalStyleKind accidentalStyleKind)
{
  string result;

  switch (accidentalStyleKind) {
    case kDefault: // default value
      result = "default";
      break;
    case kVoice:
      result = "voice";
      break;
    case kModern:
      result = "modern";
      break;
    case kModernCautionary:
      result = "modernCautionary";
      break;
    case kModernVoice:
      result = "modernVoice";
      break;
    case kModernVoiceCautionary:
      result = "modernVoiceCautionary";
      break;
    case kPiano:
      result = "piano";
      break;
    case kPianoCautionary:
      result = "pianoCautionary";
      break;
    case kNeoModern:
      result = "neoModern";
      break;
    case kNeoModernCautionary:
      result = "neoModernCautionary";
      break;
    case kNeoModernVoice:
      result = "neoModernVoice";
      break;
    case kNeoModernVoiceCautionary:
      result = "neoModernVoiceCautionary";
      break;
    case kDodecaphonic:
      result = "dodecaphonic";
      break;
    case kDodecaphonicNoRepeat:
      result = "dodecaphonicNoRepeat";
      break;
    case kDodecaphonicFirst:
      result = "dodecaphonicFirst";
      break;
    case kTeaching:
      result = "teaching";
      break;
    case kNoReset:
      result = "noReset";
      break;
    case kForget:
      result = "Forget";
      break;
  } // switch

  return result;
}

string lpsrAccidentalStyleKindAsLilypondString (
  lpsrAccidentalStyleKind accidentalStyleKind)
{
  string result;

  switch (accidentalStyleKind) {
    case kDefault: // default value
      result = "default";
      break;
    case kVoice:
      result = "voice";
      break;
    case kModern:
      result = "modern";
      break;
    case kModernCautionary:
      result = "modern-cautionary";
      break;
    case kModernVoice:
      result = "modern-voice";
      break;
    case kModernVoiceCautionary:
      result = "modern-voice-cautionary";
      break;
    case kPiano:
      result = "piano";
      break;
    case kPianoCautionary:
      result = "piano-cautionary";
      break;
    case kNeoModern:
      result = "neo-modern";
      break;
    case kNeoModernCautionary:
      result = "neo-modern-cautionary";
      break;
    case kNeoModernVoice:
      result = "neo-modern-voice";
      break;
    case kNeoModernVoiceCautionary:
      result = "neo-modern--voice-cautionary";
      break;
    case kDodecaphonic:
      result = "dodecaphonic";
      break;
    case kDodecaphonicNoRepeat:
      result = "dodecaphonic-no-repeat";
      break;
    case kDodecaphonicFirst:
      result = "dodecaphonic-first";
      break;
    case kTeaching:
      result = "teaching";
      break;
    case kNoReset:
      result = "no-reset";
      break;
    case kForget:
      result = "forget";
      break;
  } // switch

  return result;
}

void initializeLpsrAccidentalStyleKindsMap ()
{
  // register the LilyPond accidental styles
  // --------------------------------------

  gGlobalLpsrAccidentalStyleKindsMap ["default"] = kDefault;
  gGlobalLpsrAccidentalStyleKindsMap ["voice"] = kVoice;
  gGlobalLpsrAccidentalStyleKindsMap ["modern"] = kModern;
  gGlobalLpsrAccidentalStyleKindsMap ["modern-cautionary"] = kModernCautionary;
  gGlobalLpsrAccidentalStyleKindsMap ["modern-voice"] = kModernVoice;
  gGlobalLpsrAccidentalStyleKindsMap ["modern-voice-cautionary"] = kModernVoiceCautionary;
  gGlobalLpsrAccidentalStyleKindsMap ["piano"] = kPiano;
  gGlobalLpsrAccidentalStyleKindsMap ["piano-cautionary"] = kPianoCautionary;
  gGlobalLpsrAccidentalStyleKindsMap ["neo-modern"] = kNeoModern;
  gGlobalLpsrAccidentalStyleKindsMap ["neo-modern-cautionary"] = kNeoModernCautionary;
  gGlobalLpsrAccidentalStyleKindsMap ["neo-modern-voice"] = kNeoModernVoice;
  gGlobalLpsrAccidentalStyleKindsMap ["neo-modern-voice-cautionary"] = kNeoModernVoiceCautionary;
  gGlobalLpsrAccidentalStyleKindsMap ["dodecaphonic"] = kDodecaphonic;
  gGlobalLpsrAccidentalStyleKindsMap ["dodecaphonic-no-repeat"] = kDodecaphonicNoRepeat;
  gGlobalLpsrAccidentalStyleKindsMap ["dodecaphonic-first"] = kDodecaphonicFirst;
  gGlobalLpsrAccidentalStyleKindsMap ["teaching"] = kTeaching;
  gGlobalLpsrAccidentalStyleKindsMap ["no-reset"] = kNoReset;
  gGlobalLpsrAccidentalStyleKindsMap ["forget"] = kForget;
}

string existingLpsrAccidentalStyleKinds (unsigned int namesListMaxLength)
{
  stringstream s;

  unsigned int
    accidentalStyleKindsMapSize =
      gGlobalLpsrAccidentalStyleKindsMap.size ();

  if (accidentalStyleKindsMapSize) {
    unsigned int
      nextToLast =
        accidentalStyleKindsMapSize - 1;

    unsigned int count = 0;
    unsigned int cumulatedLength = 0;

    for (
      map<string, lpsrAccidentalStyleKind>::const_iterator i =
        gGlobalLpsrAccidentalStyleKindsMap.begin ();
      i != gGlobalLpsrAccidentalStyleKindsMap.end ();
      ++i
    ) {
      string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
      }

      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != accidentalStyleKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

// chords languages
//______________________________________________________________________________

map<string, lpsrChordsLanguageKind>
  gGlobalLpsrChordsLanguageKindsMap;

void initializeLpsrChordsLanguageKindsMap ()
{
  gGlobalLpsrChordsLanguageKindsMap ["ignatzek"]   = kChordsIgnatzek; // default
  gGlobalLpsrChordsLanguageKindsMap ["german"]     = kChordsGerman;
  gGlobalLpsrChordsLanguageKindsMap ["semiGerman"] = kChordsSemiGerman;
  gGlobalLpsrChordsLanguageKindsMap ["italian"]    = kChordsItalian;
  gGlobalLpsrChordsLanguageKindsMap ["french"]     = kChordsFrench;
}

string lpsrChordsLanguageKindAsString (
  lpsrChordsLanguageKind languageKind)
{
  string result;

  switch (languageKind) {
    case kChordsIgnatzek: // default value
      result = "ignatzek";
      break;
    case kChordsGerman:
      result = "german";
      break;
    case kChordsSemiGerman:
      result = "semiGerman";
      break;
    case kChordsItalian:
      result = "italian";
      break;
    case kChordsFrench:
      result = "french";
      break;
  } // switch

  return result;
}

string existingLpsrChordsLanguageKinds (unsigned int namesListMaxLength)
{
  stringstream s;

  unsigned int
    lpsrChordsLanguageKindsMapSize =
      gGlobalLpsrChordsLanguageKindsMap.size ();

  if (lpsrChordsLanguageKindsMapSize) {
    unsigned int nextToLast =
      lpsrChordsLanguageKindsMapSize - 1;

    unsigned int count = 0;
    unsigned int cumulatedLength = 0;

    for (
      map<string, lpsrChordsLanguageKind>::const_iterator i =
        gGlobalLpsrChordsLanguageKindsMap.begin ();
      i != gGlobalLpsrChordsLanguageKindsMap.end ();
      ++i
    ) {
      string theString = (*i).first;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
        break;
      }

      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != lpsrChordsLanguageKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

//______________________________________________________________________________
string msrSemiTonesPitchAndOctaveAsLilypondString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  S_msrSemiTonesPitchAndOctave       semiTonesPitchAndOctave)
{
  stringstream s;

  msrQuarterTonesPitchKind
    quarterTonesPitchKind =
      quarterTonesPitchKindFromSemiTonesPitchKind (
        semiTonesPitchAndOctave->
          getSemiTonesPitchKind ());

  map<msrQuarterTonesPitchKind, string>
    *pitchNamesMapPTR =
      &gGlobalNederlandsPitchesNamesMap; // default LilyPond value

  // is quarterTonesPitchName in the part renaming map?
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

  s << (*pitchNamesMapPTR) [quarterTonesPitchKind];

  switch (semiTonesPitchAndOctave->getOctaveKind ()) {
    case msrOctaveKind::k_NoOctave:
      s << "*** noOctave ***";
      break;
    case msrOctaveKind::kOctave0:
      s << ",,,";
      break;
    case msrOctaveKind::kOctave1:
      s << ",,";
      break;
    case msrOctaveKind::kOctave2:
      s << ",";
      break;
    case msrOctaveKind::kOctave3:
      s << "";
      break;
    case msrOctaveKind::kOctave4:
      s << "'";
      break;
    case msrOctaveKind::kOctave5:
      s << "''";
      break;
    case msrOctaveKind::kOctave6:
      s << "'''";
      break;
    case msrOctaveKind::kOctave7:
      s << "''''";
      break;
    case msrOctaveKind::kOctave8:
      s << "'''''";
      break;
    case msrOctaveKind::kOctave9:
      s << "''''''";
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceNotesOctaveEntry ()) {
    s <<
      " %{ " <<
      semiTonesPitchAndOctave->asString () <<
      " %}";
  }
#endif

  return s.str ();
}

/* JMI
string msrSemiTonesPitchAndOctaveAsLilypondString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  S_msrSemiTonesPitchAndOctave       semiTonesPitchAndOctave)
{
  msrQuarterTonesPitchKind
    quarterTonesPitchKind =
      quarterTonesPitchKindFromSemiTonesPitchKind (
        semiTonesPitchKind);

  return
    msrQuarterTonesPitchAndOctaveAsLilypondString (
      languageKind,
      quarterTonesPitchKind);

  return "??????";
}
*/

// lyrics durations
//______________________________________________________________________________

map<string, lpsrLyricsDurationsKind>
  gGlobalLpsrLyricsDurationsKindsMap;

string lpsrLyricsDurationsKindAsString (
  lpsrLyricsDurationsKind lyricsDurationsKind)
{
  string result;

  // no CamelCase here, these strings are used in the command line options

  switch (lyricsDurationsKind) {
    case kLyricsDurationsImplicit:
      result = "lyricsDurationsImplicit";
      break;
    case kLyricsDurationsExplicit:
      result = "lyricsDurationsExplicit";
      break;
  } // switch

  return result;
}

void initializeLpsrLyricsDurationsKindsMap ()
{
  // register the LilyPond score output kinds
  // --------------------------------------

  // no CamelCase here, these strings are used in the command line options

  gGlobalLpsrLyricsDurationsKindsMap ["implicit"] = kLyricsDurationsImplicit;
  gGlobalLpsrLyricsDurationsKindsMap ["explicit"] = kLyricsDurationsExplicit;
}

string existingLpsrLyricsDurationsKinds (unsigned int namesListMaxLength)
{
  stringstream s;

  unsigned int
    lpsrLyricsDurationsKindsMapSize =
      gGlobalLpsrLyricsDurationsKindsMap.size ();

  if (lpsrLyricsDurationsKindsMapSize) {
    unsigned int
      nextToLast =
        lpsrLyricsDurationsKindsMapSize - 1;

    unsigned int count = 0;
    unsigned int cumulatedLength = 0;

    for (
      map<string, lpsrLyricsDurationsKind>::const_iterator i =
        gGlobalLpsrLyricsDurationsKindsMap.begin ();
      i != gGlobalLpsrLyricsDurationsKindsMap.end ();
      ++i
    ) {
      string theString = (*i).first;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
        break;
      }

      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != lpsrLyricsDurationsKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

//______________________________________________________________________________
void initializeLPSRBasicTypes ()
{
  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
#ifdef TRACING_IS_ENABLED
    if (getTraceOah () && ! gGlobalGeneralOahGroup->getQuiet ()) {
      gLogStream <<
        "Initializing LPSR basic types handling" <<
        endl;
    }
#endif

    // LPSR score output handling
    // ------------------------------------------------------

    initializeLpsrScoreOutputKindsMap ();

    // LPSR octave entry handling
    // ------------------------------------------------------

    initializeLpsrOctaveEntryKindsMap ();

    // LPSR accidental styles handling
    // ------------------------------------------------------

    initializeLpsrAccidentalStyleKindsMap ();

    // LPSR chords languages handling
    // ------------------------------------------------------

    initializeLpsrChordsLanguageKindsMap ();

    // LPSR lyrics durations kinds handling
    // ------------------------------------------------------

    initializeLpsrLyricsDurationsKindsMap ();

    pThisMethodHasBeenRun = true;
  }
}


}
