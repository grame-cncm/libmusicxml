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

#include "utilities.h"

#include "messagesHandling.h"
#include "generalOptions.h"

#include "xml2lyOptionsHandling.h"

#include "lpsrBasicTypes.h"


using namespace std;

namespace MusicXML2 {

#define TRACE_LPSR_BASIC_TYPES 0

//_______________________________________________________________________________
string wholeNotesAsLilypondString (
  int      inputLineNumber,
  rational wholeNotes,
  int&     dotsNumber)
{  
#define DEBUG_WHOLE_NOTES 1

  if (DEBUG_WHOLE_NOTES) {
    gLogIOstream <<
      "--> wholeNotes: " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }

  wholeNotes.rationalise ();

  if (DEBUG_WHOLE_NOTES) {
    gLogIOstream <<
      "--> wholeNotes rationalised: " << wholeNotes <<
      endl;
  }

  int
    numerator    = wholeNotes.getNumerator (),
    denominator  = wholeNotes.getDenominator ();

  if (DEBUG_WHOLE_NOTES) {
    gLogIOstream <<
      "--> numerator: " << numerator <<
      endl <<
      "--> denominator: " << denominator <<
      endl <<
      endl;
  }

  if (numerator == 0) {
    return "256"; // JMI ???
  }

  if (numerator == 1) {
    return to_string (denominator);
  }

  int
    workNumerator   = numerator,
    workDenominator = denominator,
    workDotsNumber  = 0;

  stringstream s;

  // handle the 'smaller than quarter note' part if any
  for ( ; ; ) {
    if (workDenominator <= 4) {
      break;
    }
    
    if (workDenominator == 1) {
      s <<
        workDenominator;
      break;
    }
    
    if (workNumerator % 2 == 1) {
      workDotsNumber += 1;
      
      workNumerator = (workNumerator - 1) / 2;
      workDenominator /= 2;
      
      rational r (workNumerator, workDenominator);
      r.rationalise ();
      
      workNumerator   = r.getNumerator (),
      workDenominator = r.getDenominator ();
    }

    if (DEBUG_WHOLE_NOTES) {
      gLogIOstream <<
        "--> workNumerator" << " : " << workNumerator <<
        endl <<
        "--> workDenominator" << " : " << workDenominator <<
        endl <<
        "--> workDotsNumber" << " : " << workDotsNumber <<
        endl <<
        "--> s.str ()" << " : \"" << s.str () << "\"" <<
        endl <<
        endl;
    }
  } // for
  
  if (workDenominator <= 4) {
    // handle the 'larger than quarter note' part

    int accumulatedValue = 1;

    accumulatedValue++; // JMI
    
    for ( ; ; ) {
      if (workNumerator == 1) {
        // wholeNotes is a number of whole notes
        if (workDenominator * 4 == numerator) {
          // wholeNotes is a dotted note duration
          s <<
            workDenominator;
        }
        else {
          // wholeNotes is NOT a dotted note duration,
          // use a multiplying factor and exit
          stringstream ss;
          
          ss <<
            workDenominator /*1*/ <<
            "*" <<
            numerator << "/" << denominator; // JMI

          if (DEBUG_WHOLE_NOTES) {
            gLogIOstream <<
              "--> return:" <<
              endl <<
              "--> ss.str ()" << " : \"" << ss.str () << "\"" <<
              endl <<
            endl;
          }

          return ss.str ();
        }
        break;
      }
      
      if (workDenominator == 1) {
        // a number of whole notes
        switch (workNumerator) {
          case 1:
            s << "1";
            break;
          case 2:
            s << "\\breve";
            break;
          case 3:
            s << "\\breve";
            workDotsNumber += 1;
            break;
          case 4:
            s << "\\longa";
            break;
          case 6:
            s << "\\longa";
            workDotsNumber += 1;
            break;
          case 7:
            s << "\\longa";
            workDotsNumber += 2;
            break;
          case 8:
            s << "\\maxima";
            break;
          case 12:
            s << "\\maxima";
            workDotsNumber += 1;
            break;
          case 14:
            s << "\\maxima";
            workDotsNumber += 2;
            break;
          default:
            s <<
              workNumerator << "/" << workDenominator <<
              " whole notes cannot be represented as an MSR string";
  
            msrInternalError (
              gXml2lyOptions->fInputSourceName,
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
        } // switch
        break;
      }
      
      if (DEBUG_WHOLE_NOTES) {
        gLogIOstream <<
          "--> workNumerator % 2" << " : " << workNumerator % 2 <<
          endl;
      }

      if (workNumerator % 2 == 1) {
        // a number of quarter or half notes
        workDotsNumber += 1;
        
        workNumerator = (workNumerator - 1) / 2;
        workDenominator /= 2;
        
        if (DEBUG_WHOLE_NOTES) {
          gLogIOstream <<
            "--> workNumerator" << " : " << workNumerator <<
            endl <<
            "--> workDenominator" << " : " << workDenominator <<
            endl <<
            "--> workDotsNumber" << " : " << workDotsNumber <<
            endl <<
            endl;
        }

        rational r (workNumerator, workDenominator);
        r.rationalise ();
        
        workNumerator   = r.getNumerator (),
        workDenominator = r.getDenominator ();

        if (DEBUG_WHOLE_NOTES) {
          gLogIOstream <<
            "--> workNumerator" << " : " << workNumerator <<
            endl <<
            "--> workDenominator" << " : " << workDenominator <<
            endl <<
            "--> workDotsNumber" << " : " << workDotsNumber <<
            endl <<
            "--> s.str ()" << " : \"" << s.str () << "\"" <<
            endl <<
            endl;
        }
      }
    } // for
  }  

  // append the dots if any
  /* JMI
  if (false) {
    s <<
      " %{" << numerator << "/" << denominator << "%} ";
  }
  */
    
  for (int i = 0; i < workDotsNumber; i++) {
    s << ".";
  } // for
  
  // return the result
  dotsNumber = workDotsNumber;
  
  if (DEBUG_WHOLE_NOTES) {
    gLogIOstream <<
      "--> return:" <<
      endl <<
      "--> s.str ()" << " : \"" << s.str () << "\"" <<
      endl <<
      endl;
  }
  
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

  if (numberOfWholeNotes != 1)
    s <<
      "*" << numberOfWholeNotes;

  return s.str ();
}

//_______________________________________________________________________________
  /* JMI
void writeNoteAsLilypondString (
  S_msrNote note,
  ostream&  os)
{
  // print the note ligatures if any
  list<S_msrLigature>
    noteLigatures =
      note->getNoteLigatures ();

  if (noteLigatures.size ()) {
    list<S_msrLigature>::const_iterator i;
    for (
      i=noteLigatures.begin ();
      i!=noteLigatures.end ();
      i++) {
        
      switch ((*i)->getLigatureKind ()) {
        case msrLigature::k_NoLigature:
          break;
        case msrLigature::kStartLigature:
          os << "\\[" " ";
          break;
        case msrLigature::kContinueLigature:
          break;
        case msrLigature::kStopLigature:
   // JMI       os << "\\]" " ";
          break;
      } // switch
      fMusicOlec++;
    } // for
  }

  // get note stem kind 
  msrStem::msrStemKind
    stemKind =
      fCurrentStem
        ? fCurrentStem->getStemKind ()
        : msrStem::k_NoStem;

  // handle note kind before printing note itself
  switch (note->getNoteKind ()) {
    
    case msrNote::k_NoNoteKind:
      break;
        
    case msrNote::kRestNote:      
      break;
      
    case msrNote::kSkipNote:      
      break;
      
    case msrNote::kStandaloneNote:
      {
        // should the stem be omitted?
        if (note->getNoteIsStemless ()) {
          os <<
            endl <<
            "\\stemNeutral" " "; // JMI ""\\once\\omit Stem" " ";
        }

        // should stem direction be generated?
        if (gLilypondOptions->fStems) {
      
          if (fCurrentStem) {
            // should stem direction be generated?
            if (stemKind != fCurrentStemKind) {
              switch (stemKind) {
                case msrStem::k_NoStem:
                  os << "\\stemNeutral" " ";
                  break;
                case msrStem::kStemUp:
                  os << "\\stemUp" " ";
                  break;
                case msrStem::kStemDown:
                  os << "\\stemDown" " ";
                  break;
                case msrStem::kStemNone:
                  break;
                case msrStem::kStemDouble: // JMI ???
                  break;
              } // switch
            }
            fMusicOlec++;
      
            // is there a stem kind change?
            if (stemKind != fCurrentStemKind)
              fCurrentStemKind = stemKind;
          }
        }
      }
      break;

    case msrNote::kDoubleTremoloMemberNote:
      {
        // should the stem be omitted?
        if (note->getNoteIsStemless ()) {
          os <<
            endl <<
            "\\stemNeutral"; // JMI ""\\once\\omit Stem" " ";
        }

        // should stem direction be generated?
        if (gLilypondOptions->fStems) {
      
          if (fCurrentStem) {
            // should stem direction be generated?
            if (stemKind != fCurrentStemKind) {
              switch (stemKind) {
                case msrStem::k_NoStem:
                  os << "\\stemNeutral" " ";
                  break;
                case msrStem::kStemUp:
                  os << "\\stemUp" " ";
                  break;
                case msrStem::kStemDown:
                  os << "\\stemDown" " ";
                  break;
                case msrStem::kStemNone:
                  break;
                case msrStem::kStemDouble: // JMI ???
                  break;
              } // switch
            }
            fMusicOlec++;
      
            // is there a stem kind change?
            if (stemKind != fCurrentStemKind)
              fCurrentStemKind = stemKind;
          }
        }
      }
      break;

    case msrNote::kGraceNote:
      break;
      
    case msrNote::kChordMemberNote:
     // don't omit stems for chord member note JMI
     break;
      
    case msrNote::kTupletMemberNote:
      break;
  } // switch

  // print the note itself
  switch (note->getNoteKind ()) {
    
    case msrNote::k_NoNoteKind:
      break;
        
    case msrNote::kRestNote:
      {
        // get pitched rest status
        bool noteIsAPitchedRest =
          note->noteIsAPitchedRest ();
  
        if (noteIsAPitchedRest) {
          os <<
            pitchedRestAsLilypondString (note);
        }
  
        else {
          // print the rest name
          os <<
            string (
              note->getNoteOccupiesAFullMeasure ()
                ? "R"
                : "r");
        }
          
        // print the rest duration
        os <<
          lilypondizeDurationString (
            note->
  // JMI            getNoteSkipOrRestSoundingWholeNotesAsMsrString ());
              noteSoundingWholeNotesAsMsrString ());
  
        // is the rest pitched?
        if (noteIsAPitchedRest) {
          os <<
            " " "\\rest";

          // this note is the new relative octave reference
          // (the display quarter tone pitch and octave
          // have been copied to the note octave
          // in the msrNote::msrNote() constructor,
          // since the note octave is used in relative code generation)
          fRelativeOctaveReference = note;
        }
        else {
          // an unpitched rest is no relative octave reference,
          // the preceding one is kept
        }
      }
      break;
      
    case msrNote::kSkipNote:      
      // print the skip name
      os << "s";
      
      // print the skip duration
      os <<
        lilypondizeDurationString (
          note->
  // JMI            getNoteSkipOrRestSoundingWholeNotesAsMsrString ());
              noteSoundingWholeNotesAsMsrString ());

      // a rest is no relative octave reference,
      // the preceding one is kept
      break;
      
    case msrNote::kStandaloneNote:
      // print the note name
      os <<
        noteAsLilypondString (note);
      
      // print the note duration
      os <<
        lilypondizeDurationString (
          note->
            noteSoundingWholeNotesAsMsrString ());

      // handle delayed ornaments if any
      if (note->getNoteHasADelayedOrnament ())
        // c2*2/3 ( s2*1/3\turn JMI
        os <<
          "*" <<
          gLilypondOptions->fDelayedOrnamentFractionNumerator <<
          "/" <<
          gLilypondOptions->fDelayedOrnamentFractionDenominator;
      
      // print the tie if any
      {
        S_msrTie noteTie = note->getNoteTie ();
      
        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kStartTie) {
            os << " ~";
          }
        }
      }

      // this note is the new relative octave reference
      fRelativeOctaveReference = note;
      break;

    case msrNote::kDoubleTremoloMemberNote:
      // print the note name
      os <<
        noteAsLilypondString (note);
      
      // print the note duration, i.e. the double tremolo elements duration
      os <<
        fCurrentDoubleTremoloElementsLpsrDuration; // JMI

      // handle delayed ornaments if any
      if (note->getNoteHasADelayedOrnament ())
        // c2*2/3 ( s2*1/3\turn JMI
        os <<
          "*" <<
          gLilypondOptions->fDelayedOrnamentFractionNumerator <<
          "/" <<
          gLilypondOptions->fDelayedOrnamentFractionDenominator;
      
      // print the tie if any
      {
        S_msrTie noteTie = note->getNoteTie ();
      
        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kStartTie) {
            os << " ~";
          }
        }
      }

      // this note is the new relative octave reference
      fRelativeOctaveReference = note;
      break;

    case msrNote::kGraceNote:
      // print the note name
      os <<
        noteAsLilypondString (note);
      
      // print the grace note's graphic duration
      os <<
        lilypondizeDurationString (
          note->
            noteGraphicDurationAsMsrString ());

      // print the dots if any JMI ???
      for (int i = 0; i < note->getNoteDotsNumber (); i++) {
        os << ".";
      } // for
      
      // print the tie if any
      {
        S_msrTie noteTie = note->getNoteTie ();
      
        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kStartTie) {
            os << "~ ";
          }
        }
      }

      // this note is the new relative octave reference
      fRelativeOctaveReference = note;
      break;
      
    case msrNote::kChordMemberNote:
      // print the note name
      os <<
        noteAsLilypondString (note);
      
      // don't print the note duration,
      // it will be printed for the chord itself

      // inside chords, a note is relative to the preceding one
      fRelativeOctaveReference = note;
      break;
      
    case msrNote::kTupletMemberNote:
      if (! gLilypondOptions->fTupletsOnALine) {
        os <<
          endl;
      }
        
      // print the note name
      if (note->getNoteIsARest ()) {
        os <<
          string (
            note->getNoteOccupiesAFullMeasure ()
              ? "R"
              : "r");
      }
      else {
        os <<
          noteAsLilypondString (note);
      }
      
      // print the note duration
      / * JMI
      os <<
        lilypondizeDurationString (
          note->
            getNoteTupletNoteSoundingWholeNotesAsMsrString ()); // ??? JMI
            * /

      // print the tie if any
      {
        S_msrTie noteTie = note->getNoteTie ();
      
        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kStartTie) {
            os << "~ ";
          }
        }
      }

      // a rest is no relative octave reference,
      if (! note->getNoteIsARest ())
        // this note is the new relative octave reference
        fRelativeOctaveReference = note;
      break;
  } // switch

  os << " ";

  fMusicOlec++;

  // was there an unmetered (stemless) section?
  if (stemKind != fCurrentStemKind) {
    switch (stemKind) {
      case msrStem::kStemNone:
        break;
        
      case msrStem::k_NoStem:
      case msrStem::kStemUp:
      case msrStem::kStemDown:
      case msrStem::kStemDouble:
        fOnGoingStemNone =false;
        break;
    } // switch
  }
}
  */

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
      if ((*i).second != kDefaultStyle)
        s << (*i).first;
      if (++i == iEnd) break;
      if ((*i).second != kDefaultStyle)
        s << " ";
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
      if ((*i).second != k_IgnatzekChords)
        s << (*i).first;
      if (++i == iEnd) break;
      if ((*i).second != k_IgnatzekChords)
        s << " ";
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
