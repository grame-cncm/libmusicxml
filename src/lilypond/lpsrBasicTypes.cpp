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
#include <stdlib.h>     / * abort, NULL * /
#include <cassert>

#include <vector>
#include <map>

#include <iomanip>      // setw, ...

#include "rational.h"

*/

#include <iostream>
#include <sstream>

#include "utilities.h"

#include "lpsrBasicTypes.h"


using namespace std;

namespace MusicXML2 {

// useful shortcut macros
#define idtr indenter::gIndenter
#define tab  indenter::gIndenter.getSpacer ()

#define TRACE_LPSR_BASIC_TYPES 0

//_______________________________________________________________________________
string wholeNotesAsLilypondString (
  int      inputLineNumber,
  rational wholeNotes,
  int&     dotsNumber)
{  
  wholeNotes.rationalise ();

  int
    numerator    = wholeNotes.getNumerator (),
    denominator  = wholeNotes.getDenominator ();

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
            s << "\\breve";
            break;
          case 3:
            s << "\\breve";
            numberOfDots += 1;
            break;
          case 4:
            s << "\\longa";
            break;
          case 6:
            s << "\\longa";
            numberOfDots += 1;
            break;
          case 8:
            s << "\\maxima";
            break;
          case 12:
            s << "\\maxima";
            numberOfDots += 1;
            break;
          default:
            s <<
              numerator << "/" << denominator <<
              " whole notes cannot be represented as an MSR string";
  
            basicLpsrInternalError (
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
  if (false) {
    s <<
      " %{" << numerator << "/" << denominator << "%} ";
  }
    
  for (int i = 0; i < numberOfDots; i++) {
    s << ".";
  } // for
  
  // return the result
  dotsNumber = numberOfDots;
  
  return
    s.str();
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
  /* JMI
void writeNoteAsLilypondString (
  S_msrNote note,
  ostream&  os)
{
  // print the note ligatures if any
  list<S_msrLigature>
    noteLigatures =
      note->getNoteLigatures ();

  if (noteLigatures.size()) {
    list<S_msrLigature>::const_iterator i;
    for (
      i=noteLigatures.begin();
      i!=noteLigatures.end();
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
            idtr <<
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
            idtr <<
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
          endl <<
          idtr;
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
      iBegin = textsList.begin(),
      iEnd   = textsList.end(),
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

map<string, lpsrAccidentalStyle>
  gLpsrAccidentalStylesMap;

string lpsrAccidentalStyleAsString (
  lpsrAccidentalStyle style)
{
  string result;
  
  switch (style) {
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

string lpsrAccidentalStyleAsLilypondString (
  lpsrAccidentalStyle style)
{
  string result;
  
  switch (style) {
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

void initializeLpsrAccidentalStylesMap ()
{
  // register the LilyPond accidental styles
  // --------------------------------------

  gLpsrAccidentalStylesMap ["default"] = kDefaultStyle;
  gLpsrAccidentalStylesMap ["voice"] = kVoiceStyle;
  gLpsrAccidentalStylesMap ["modern"] = kModernStyle;
  gLpsrAccidentalStylesMap ["modern-cautionary"] = kModernCautionaryStyle;
  gLpsrAccidentalStylesMap ["modern-voice"] = kModernVoiceStyle;
  gLpsrAccidentalStylesMap ["modern-voice-cautionary"] = kModernVoiceCautionaryStyle;
  gLpsrAccidentalStylesMap ["piano"] = kPianoStyle;
  gLpsrAccidentalStylesMap ["piano-cautionary"] = kPianoCautionaryStyle;
  gLpsrAccidentalStylesMap ["neo-modern"] = kNeoModernStyle;
  gLpsrAccidentalStylesMap ["neo-modern-cautionary"] = kNeoModernCautionaryStyle;
  gLpsrAccidentalStylesMap ["neo-modern-voice"] = kNeoModernVoiceStyle;
  gLpsrAccidentalStylesMap ["neo-modern-voice-cautionary"] = kNeoModernVoiceCautionaryStyle;
  gLpsrAccidentalStylesMap ["dodecaphonic"] = kDodecaphonicStyle;
  gLpsrAccidentalStylesMap ["dodecaphonic-no-repeat"] = kDodecaphonicNoRepeatStyle;
  gLpsrAccidentalStylesMap ["dodecaphonic-first"] = kDodecaphonicFirstStyle;
  gLpsrAccidentalStylesMap ["teaching"] = kTeachingStyle;
  gLpsrAccidentalStylesMap ["no-reset"] = kNoResetStyle;
  gLpsrAccidentalStylesMap ["forget"] = kForgetStyle;
}

string existingLpsrAccidentalStyles ()
{
  stringstream s;

  if (gLpsrAccidentalStylesMap.size ()) {
    map<string, lpsrAccidentalStyle>::const_iterator
      iBegin = gLpsrAccidentalStylesMap.begin(),
      iEnd   = gLpsrAccidentalStylesMap.end(),
      i      = iBegin;
    for ( ; ; ) {
      cerr <<
        (*i).first <<
        endl;
      if ((*i).second != kDefaultStyle)
        s << (*i).first;
      if (++i == iEnd) break;
      if ((*i).second != kDefaultStyle)
        s << " ";
    } // for
  }

  return s.str();
}

// chords languages
//______________________________________________________________________________

map<string, lpsrChordsLanguage>
  gLpsrChordsLanguagesMap;

void initializeLpsrChordsLanguagesMap ()
{
  gLpsrChordsLanguagesMap ["Ignatzek"]   = k_IgnatzekChords; // default
  gLpsrChordsLanguagesMap ["german"]     = k_GermanChords;
  gLpsrChordsLanguagesMap ["semiGerman"] = k_SemiGermanChords;
  gLpsrChordsLanguagesMap ["italian"]    = k_ItalianChords;
  gLpsrChordsLanguagesMap ["french"]     = k_FrenchChords;
}

string lpsrChordsLanguageAsString (
  lpsrChordsLanguage language)
{
  string result;
  
  switch (language) {
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

string existingLpsrChordsLanguages ()
{
  stringstream s;
  
  if (gLpsrChordsLanguagesMap.size ()) {
    map<string, lpsrChordsLanguage>::const_iterator
      iBegin = gLpsrChordsLanguagesMap.begin(),
      iEnd   = gLpsrChordsLanguagesMap.end(),
      i      = iBegin;
    for ( ; ; ) {
      if ((*i).second != k_IgnatzekChords)
        s << (*i).first;
      if (++i == iEnd) break;
      if ((*i).second != k_IgnatzekChords)
        s << " ";
    } // for
  }
  
  return s.str();
}


}
