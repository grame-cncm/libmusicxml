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

#include <climits>      // INT_MIN, INT_MAX

#include "utilities.h"

#include "messagesHandling.h"
#include "generalOptions.h"

#include "xml2lyOptionsHandling.h"

#include "lpsrBasicTypes.h"


using namespace std;

namespace MusicXML2 {

#define TRACE_LPSR_BASIC_TYPES 0

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

#define DEBUG_WHOLE_NOTES 0

  if (DEBUG_WHOLE_NOTES) {
    gLogIOstream <<
      "--> -------------------------------------" <<
      endl <<
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
    augmentation dots add half the preceding increment to the duration:
    they constitue a series of frations or the form '(2^n-1) / 2^n',
    starting with 3/2, 7/4, 15/8,
    that tends towards 2 while always remaining less than two.
    
    with MusicXML's limitation to 1024th of a whole note,
    with LilyPond's limitation to 128th of a whole note,
    valid numerators are:
  */

  int  numeratorDots = lpsrNumberOfDots (numerator);

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
            gXml2lyOptions->fInputSourceName,
            inputLineNumber,
            s.str ());

  //      multiplyingFactorIsToBeUsed = true;
      }
  } // switch
    */

  if (DEBUG_WHOLE_NOTES) {
    gLogIOstream <<
      "--> numeratorDots " << " : " <<
      numeratorDots <<
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

  int durationLog = lpsrDurationBinaryLogarithm (denominator);

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
    // 6/1 thus becomes 3\breve, hence \longa.
    while (numerator % 2 == 0) {
      numerator /= 2;
      durationLog -= 1;
    } // while

    // update the number of dots
    numeratorDots = lpsrNumberOfDots (numerator);
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
string multipleRestWholeNoteAsLilypondString (
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

  os <<
  // JMI  quoteStringIfNonAlpha (
    quoteString (
      contents);
} 

// accidental styles
//______________________________________________________________________________

map<string, lpsrAccidentalStyleKind>
  gLpsrAccidentalStyleKindsMap;

string lpsrAccidentalStyleKindAsString (
  lpsrAccidentalStyleKind styleKind)
{
  string result;
  
  switch (styleKind) {
    case kDefaultStyle: // default value
      result = "DefaultStyle";
      break;
    case kVoiceStyle:
      result = "VoiceStyle";
      break;
    case kModernStyle:
      result = "ModernStyle";
      break;
    case kModernCautionaryStyle:
      result = "ModernCautionaryStyle";
      break;
    case kModernVoiceStyle:
      result = "ModernVoiceStyle";
      break;
    case kModernVoiceCautionaryStyle:
      result = "ModernVoiceCautionaryStyle";
      break;
    case kPianoStyle:
      result = "PianoStyle";
      break;
    case kPianoCautionaryStyle:
      result = "PianoCautionaryStyle";
      break;
    case kNeoModernStyle:
      result = "NeoModernStyle";
      break;
    case kNeoModernCautionaryStyle:
      result = "NeoModernCautionaryStyle";
      break;
    case kNeoModernVoiceStyle:
      result = "NeoModernVoiceStyle";
      break;
    case kNeoModernVoiceCautionaryStyle:
      result = "NeoModernVoiceVautionaryStyle";
      break;
    case kDodecaphonicStyle:
      result = "DodecaphonicStyle";
      break;
    case kDodecaphonicNoRepeatStyle:
      result = "DodecaphonicNoRepeatStyle";
      break;
    case kDodecaphonicFirstStyle:
      result = "DodecaphonicFirstStyle";
      break;
    case kTeachingStyle:
      result = "TeachingStyle";
      break;
    case kNoResetStyle:
      result = "NoResetStyle";
      break;
    case kForgetStyle:
      result = "ForgetStyle";
      break;
  } // switch

  return result;
}

string lpsrAccidentalStyleKindAsLilypondString (
  lpsrAccidentalStyleKind styleKind)
{
  string result;
  
  switch (styleKind) {
    case kDefaultStyle: // default value
      result = "default";
      break;
    case kVoiceStyle:
      result = "voice";
      break;
    case kModernStyle:
      result = "modern";
      break;
    case kModernCautionaryStyle:
      result = "modern-cautionary";
      break;
    case kModernVoiceStyle:
      result = "modern-voice";
      break;
    case kModernVoiceCautionaryStyle:
      result = "modern-voice-cautionary";
      break;
    case kPianoStyle:
      result = "piano";
      break;
    case kPianoCautionaryStyle:
      result = "piano-cautionary";
      break;
    case kNeoModernStyle:
      result = "neo-modern";
      break;
    case kNeoModernCautionaryStyle:
      result = "neo-modern-cautionary";
      break;
    case kNeoModernVoiceStyle:
      result = "neo-modern-voice";
      break;
    case kNeoModernVoiceCautionaryStyle:
      result = "neo-modern--voice-cautionary";
      break;
    case kDodecaphonicStyle:
      result = "dodecaphonic";
      break;
    case kDodecaphonicNoRepeatStyle:
      result = "dodecaphonic-no-repeat";
      break;
    case kDodecaphonicFirstStyle:
      result = "dodecaphonic-first";
      break;
    case kTeachingStyle:
      result = "teaching";
      break;
    case kNoResetStyle:
      result = "no-reset";
      break;
    case kForgetStyle:
      result = "forget";
      break;
  } // switch

  return result;
}

void initializeLpsrAccidentalStyleKindsMap ()
{
  // register the LilyPond accidental styles
  // --------------------------------------

  gLpsrAccidentalStyleKindsMap ["default"] = kDefaultStyle;
  gLpsrAccidentalStyleKindsMap ["voice"] = kVoiceStyle;
  gLpsrAccidentalStyleKindsMap ["modern"] = kModernStyle;
  gLpsrAccidentalStyleKindsMap ["modern-cautionary"] = kModernCautionaryStyle;
  gLpsrAccidentalStyleKindsMap ["modern-voice"] = kModernVoiceStyle;
  gLpsrAccidentalStyleKindsMap ["modern-voice-cautionary"] = kModernVoiceCautionaryStyle;
  gLpsrAccidentalStyleKindsMap ["piano"] = kPianoStyle;
  gLpsrAccidentalStyleKindsMap ["piano-cautionary"] = kPianoCautionaryStyle;
  gLpsrAccidentalStyleKindsMap ["neo-modern"] = kNeoModernStyle;
  gLpsrAccidentalStyleKindsMap ["neo-modern-cautionary"] = kNeoModernCautionaryStyle;
  gLpsrAccidentalStyleKindsMap ["neo-modern-voice"] = kNeoModernVoiceStyle;
  gLpsrAccidentalStyleKindsMap ["neo-modern-voice-cautionary"] = kNeoModernVoiceCautionaryStyle;
  gLpsrAccidentalStyleKindsMap ["dodecaphonic"] = kDodecaphonicStyle;
  gLpsrAccidentalStyleKindsMap ["dodecaphonic-no-repeat"] = kDodecaphonicNoRepeatStyle;
  gLpsrAccidentalStyleKindsMap ["dodecaphonic-first"] = kDodecaphonicFirstStyle;
  gLpsrAccidentalStyleKindsMap ["teaching"] = kTeachingStyle;
  gLpsrAccidentalStyleKindsMap ["no-reset"] = kNoResetStyle;
  gLpsrAccidentalStyleKindsMap ["forget"] = kForgetStyle;
}

string existingLpsrAccidentalStyleKinds ()
{
  stringstream s;

  if (gLpsrAccidentalStyleKindsMap.size ()) {
    map<string, lpsrAccidentalStyleKind>::const_iterator
      iBegin = gLpsrAccidentalStyleKindsMap.begin (),
      iEnd   = gLpsrAccidentalStyleKindsMap.end (),
      i      = iBegin;
    for ( ; ; ) {
      gLogIOstream <<
        (*i).first <<
        endl;
      if ((*i).second != kDefaultStyle) {
        s << (*i).first;
      }
      if (++i == iEnd) break;
      if ((*i).second != kDefaultStyle) {
        s << " ";
      }
    } // for
  }

  return s.str ();
}

// chords languages
//______________________________________________________________________________

map<string, lpsrChordsLanguageKind>
  gLpsrChordsLanguageKindsMap;

void initializeLpsrChordsLanguageKindsMap ()
{
  gLpsrChordsLanguageKindsMap ["Ignatzek"]   = k_IgnatzekChords; // default
  gLpsrChordsLanguageKindsMap ["german"]     = k_GermanChords;
  gLpsrChordsLanguageKindsMap ["semiGerman"] = k_SemiGermanChords;
  gLpsrChordsLanguageKindsMap ["italian"]    = k_ItalianChords;
  gLpsrChordsLanguageKindsMap ["french"]     = k_FrenchChords;
}

string lpsrChordsLanguageKindAsString (
  lpsrChordsLanguageKind languageKind)
{
  string result;
  
  switch (languageKind) {
    case k_IgnatzekChords: // default value
      result = "IgnatzekChords";
      break;
    case k_GermanChords:
      result = "german";
      break;
    case k_SemiGermanChords:
      result = "semiGerman";
      break;
    case k_ItalianChords:
      result = "italian";
      break;
    case k_FrenchChords:
      result = "french";
      break;
  } // switch

  return result;
}

string existingLpsrChordsLanguageKinds ()
{
  stringstream s;
  
  if (gLpsrChordsLanguageKindsMap.size ()) {
    map<string, lpsrChordsLanguageKind>::const_iterator
      iBegin = gLpsrChordsLanguageKindsMap.begin (),
      iEnd   = gLpsrChordsLanguageKindsMap.end (),
      i      = iBegin;
    for ( ; ; ) {
      if ((*i).second != k_IgnatzekChords) {
        s << (*i).first;
      }
      if (++i == iEnd) break;
      if ((*i).second != k_IgnatzekChords) {
        s << " ";
      }
    } // for
  }
  
  return s.str ();
}

//______________________________________________________________________________
void initializeLPSRBasicTypes ()
{
  // LPSR accidental styles handling
  // ------------------------------------------------------

  initializeLpsrAccidentalStyleKindsMap ();

  // LPSR chords languages handling
  // ------------------------------------------------------

  initializeLpsrChordsLanguageKindsMap ();
}


}
